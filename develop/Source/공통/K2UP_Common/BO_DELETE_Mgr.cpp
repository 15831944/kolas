// BO_DELETE_Mgr.cpp: implementation of the CBO_DELETE_Mgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_DELETE_Mgr.h"
#include "BO_DELETE.h"
#include "../BO_SPECIES_API/BO_SPECIES.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_DELETE_Mgr::CBO_DELETE_Mgr(CESL_Mgr* pWnd):CSPF_Work(pWnd)
{

}

CBO_DELETE_Mgr::~CBO_DELETE_Mgr()
{

}

INT CBO_DELETE_Mgr::SPFExecutePath(CString strPath)
{
	CSPF_Work::SPFExecutePath(strPath);

	if(strPath.GetLength() == 0)
		return -1;

	if(strPath == _T("����ȭ��������"))
		return DeleteData(strPath);
	else if(strPath == _T("����ȭ��å����"))
		return DeleteData(strPath);
	else if(strPath == _T("�̳��ڷ�������"))
		return DeleteData(strPath);
	else if(strPath == _T("�̳��ڷ�å����"))
		return DeleteData(strPath);
	else if(strPath == _T("�ߺ��ڷ����"))
		return DeleteData(strPath);
	else if(strPath == _T("�����ڷ����"))
		return DeleteData(strPath);

	return -1;
}

// ���� �׸��忡�� ������ �׸��� �����մϴ�. ���� �۾��� ������ �����ϴ� ȭ���� ����ϸ�
// ������ �۾��� �����Ѵ�.(MoveData)
// MoveData�޼ҵ尡 ������ ���� �����Ѵ�.
// return ���˻�
// -1000 : ���޵� CM/Grid�˸��ƽ� �� DM�����ڰ� �ùٸ��� �ʽ��ϴ�.
// -1001 : ���� ����Ʈ�� ������ �� �����ϴ�.
// -1002 : DM���� �� KEY�� ������ �� �����ϴ�.
// -1003 : ������ �ڷḦ ����ó���� �� �����ϴ�.
// -1004 : ������ �ڷḦ DM���� ������ �� �����ϴ�.
// 0 <= ���� : ���� ó���� ����
INT CBO_DELETE_Mgr::DeleteData(CString strPath)
{
	BOOL isSpecies;
	CString CMAlias, GridAlias, DMAlias;

	this->GetTempData(_T("IsSpecies"), isSpecies);
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("GridAlias"), GridAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);

	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(strPath == _T("����ȭ��å����") || strPath == _T("�̳��ڷ�å����"))
	{
		//�ٷλ���å���� �̵�
		return 0;
	}
	INT ids = 0;
	if(IDYES == m_pParentMgr->ESLAfxMessageBox(_T("�����Ͻ� �ڷḦ �����Ͻðڽ��ϱ�?"), MB_YESNO))
		ids = MoveData(FALSE, CMAlias, GridAlias, pDM, strPath);		
	else return 0;
	return ids;
/*	
	CBO_DELETE pDlg(m_pParentMgr);
	if(IDOK == pDlg.DoModal())
	{
		ids = MoveData(pDlg.m_IsDelete, CMAlias, GridAlias, pDM, strPath);		
	}
	return ids;
*/
}
// ���ڷ� ���޵� CM/GRID/DM���� ������ �ڷḦ DM���� ����(��Ȱ��/����)ó���մϴ�.
// return �� �˻�
// -1000 : ���޵� CM/Grid�˸��ƽ� �� DM�����ڰ� �ùٸ��� �ʽ��ϴ�.
// -1001 : ���� ����Ʈ�� ������ �� �����ϴ�.
// -1002 : DM���� �� KEY�� ������ �� �����ϴ�.
// -1003 : ������ �ڷḦ ����ó���� �� �����ϴ�.
// -1004 : ������ �ڷḦ DM���� ������ �� �����ϴ�.
// 0 <= ���� : ���� ó���� ����
INT CBO_DELETE_Mgr::MoveData(BOOL delType, CString CMAlias, CString GridAlias, CESL_DataMgr *pDM, CString strPath)
{

	if(CMAlias.IsEmpty() || GridAlias.IsEmpty() || !pDM) return 1000;

	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if(ids < 0) return -1001;

	INT i = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
	if(i < 0) return -1001;

	CString reckey, tmpcnt;
	CBO_SPECIES api(m_pParentMgr);
	api.SetOpenMode(-7000);
	api.InitDataMgr();

	INT count = 0;
	CString reckeyList = _T("");
	CStringList idxList;
	idxList.RemoveAll();		m_SkeyList.RemoveAll();
	while(i >= 0)
	{
		ids = pDM->GetCellData(_T("IBS_��KEY"), i, reckey);
		m_SkeyList.AddTail(reckey);
		if(reckeyList.IsEmpty())
			reckeyList = reckey;
		else
			reckeyList += _T(",") + reckey;

		if(delType)
			ids = api.BO_SPECIES_DELETE_OR_RECYCLE(_T("PURCHASE"), _T("DELETE"), reckey);
		else
			ids = api.BO_SPECIES_DELETE_OR_RECYCLE(_T("PURCHASE"), _T("RECYCLE"), reckey);
		if(ids < 0) return -1003;
	
		tmpcnt.Format(_T("%d"), i);
		idxList.AddTail(tmpcnt);

		i = m_pParentMgr->SelectGetNext(CMAlias, GridAlias);
		count ++;
	}

	ids = this->DeleteSelectRow(pDM, idxList, strPath);
	if(ids < 0) return -1004;
	
	CString updateFurnishTbl;
	updateFurnishTbl.Format(
		_T("UPDATE BO_FURNISH_TBL SET FURNISH_STATUS='6' WHERE REC_KEY IN (SELECT B.FURNISH_DATA_KEY FROM BO_SPECIES_TBL S, BO_BOOK_TBL B WHERE S.REC_KEY=B.SPECIES_KEY AND S.REC_KEY IN (%s));"),
		reckeyList
	);
	pDM->StartFrame();
	pDM->AddFrame(updateFurnishTbl);
	ids = pDM->SendFrame();
	if(ids < 0) return -1005;
	pDM->EndFrame();

	return count;	
}

INT CBO_DELETE_Mgr::DeleteSelectRow(CESL_DataMgr * pDM, CStringList &idxlist, CString strPath)
{
	if(idxlist.IsEmpty() || !pDM) return -1000;
	POSITION pos = idxlist.GetTailPosition();
	CString tmpstr;
	INT rowidx, ids;
	while(pos != NULL)
	{
		tmpstr = idxlist.GetPrev(pos);
		rowidx = _ttoi(tmpstr.GetBuffer(0));
		if(strPath == _T("�ߺ��ڷ����"))
			DeleteDuplicateRow(pDM, rowidx);
		ids = pDM->DeleteRow(rowidx);
		if(ids < 0)
			return -1001;
	}
	return 0;
}

INT CBO_DELETE_Mgr::DeleteDuplicateRow(CESL_DataMgr *pDM, INT rowidx)
{
	INT idx, cnt;
	CString tmpstr;
	pDM->GetCellData(_T("�ĺ���ȣ"), rowidx, tmpstr);
	if(tmpstr != _T("**"))
	{
		cnt = pDM->GetRowCount();
		for(INT i=rowidx+1 ; i<cnt ; i++)
		{
			pDM->GetCellData(_T("�ĺ���ȣ"), i, tmpstr);
			if(tmpstr != _T("**"))
			{
				idx = i;
				break;
			}
		}
		if(i == cnt)
			idx = cnt;
		idx--;
		while(idx > rowidx)
		{
			pDM->DeleteRow(idx--);
		}
	}
	return 0;
}

VOID CBO_DELETE_Mgr::CopySpeciesRecKey(CStringList &skeyList)
{
	skeyList.AddHead(&m_SkeyList);
}
