// TransferMgr.cpp: implementation of the CTransferMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransferMgr.h"
#include "..\SE_API\SeApi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransferMgr::CTransferMgr(CESL_Mgr * pParentMgr):CSPF_Work(pParentMgr)
{
	m_pDM = m_pParentMgr->FindDM(_T("DM_����_����_�ڷ��ΰ�"));
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_����_����_�ڷ��ΰ�"));
	if(pCM) m_pGrid = (CESL_Grid*)pCM->FindControl(_T("�׸���"));
}

CTransferMgr::~CTransferMgr()
{

}

INT CTransferMgr::SPFExecutePath(CString strPath)
{
	if(strPath.GetLength() == 0)
		return -1;
	
	if(strPath == _T("�ǽð��ΰ�"))
		return TransferToRealTime();
	if(strPath == _T("�ǽð��谡"))
		return TransferToShelf();
	if(strPath == _T("��ϴ��ó��"))
		return TransferToReg();
	if(strPath == _T("�������ó��"))
		return TransferToBinding();

	return 0;
}


INT CTransferMgr::TransferToRealTime()
{
	//�ǽð� �ΰ�(SEL111O)
	m_pGrid->SelectMakeList();
	INT nCount = m_pGrid->SelectGetCount();
	if(0 == nCount) {
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return 0;
	}

	INT nIdx = m_pGrid->SelectGetHeadPosition();
	
	CString strWorkingStatus;

	while(nIdx>=0)
	{
		strWorkingStatus = _T("");
		strWorkingStatus = m_pDM->GetCellData(_T("SB_�ڷ����"),nIdx);
		
		if(_T("SEL112N") == strWorkingStatus) {
			AfxMessageBox(_T("���õ� �׸��� �̹� �ǽð� �谡ó���� �ڷᰡ �ֽ��ϴ�! \n Ȯ���� �ֽʽÿ�!"));
			return 0;
		}
	
		nIdx = m_pGrid->SelectGetNext();
	}


	CString msg;

	msg.Format(_T("%d ���� �ڷḦ �ǽð� �ΰ�ó�� �Ͻðڽ��ϱ�?"),nCount);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;

	INT ids = -1;
	CSeApi api(m_pParentMgr);
	CString strBookKey;
	
	m_pParentMgr->BeginWaitCursor();
	nIdx = m_pGrid->SelectGetHeadPosition();

	while(nIdx>=0)
	{
		strBookKey = _T("");
		strBookKey = m_pDM->GetCellData(_T("SB_åKEY"),nIdx);
		
		ids = 0;
		if(!strBookKey.IsEmpty()) {
			ids = api.BOOK_TRANSFER_TO_REAL_TIME(m_pDM,strBookKey);
		}
		
		if(ids<0)
		{
			msg.Format(_T("%d ��° �׸��� �۾��� �����ϴ� �� ������ ���� \n �۾��� �ߴ��մϴ�."),nIdx);
			AfxMessageBox(msg);
			m_pParentMgr->EndWaitCursor();
			return -1;
		}
		else
		{
			m_pDM->SetCellData(_T("UDF_�ڷ����"),_T("�ǽð��ΰ��ڷ�"),nIdx);
			m_pDM->SetCellData(_T("SB_�ڷ����"),_T("SEL111O"),nIdx);
			m_pDM->SetCellData(_T("SB_�ΰ迩��"),_T("Y"),nIdx);
			m_pGrid->DisplayLine(nIdx);

		}


		nIdx = m_pGrid->SelectGetNext();
	}
	
	m_pParentMgr->EndWaitCursor();
	

	//�ΰ� ó���� �ڷ�� ����
	nIdx = m_pGrid->SelectGetTailPosition();
	
	while(nIdx>=0)
	{
		m_pDM->DeleteRow(nIdx);
		nIdx = m_pGrid->SelectGetPrev();
	}
	
	m_pGrid->Display();

	msg.Format(_T("%d ���� �ڷḦ �ǽð� �ΰ�ó�� �Ͽ����ϴ�!"),nCount);
	AfxMessageBox(msg);

	


	return 0;
}

INT CTransferMgr::TransferToReg()
{
	//��ϴ��(SER111N) ó��
	m_pGrid->SelectMakeList();
	INT nCount = m_pGrid->SelectGetCount();
	if(0 == nCount) {
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return 0;
	}

	INT nIdx = m_pGrid->SelectGetHeadPosition();
	
	CString strWorkingStatus;

	while(nIdx>=0)
	{
		strWorkingStatus = _T("");
		strWorkingStatus = m_pDM->GetCellData(_T("SB_�ڷ����"),nIdx);
		
		if(_T("SEL112N") == strWorkingStatus) {
			AfxMessageBox(_T("���õ� �׸��� �̹� �ǽð� �谡ó���� �ڷᰡ �ֽ��ϴ�! \n Ȯ���� �ֽʽÿ�!"));
			return 0;
		}
	
		nIdx = m_pGrid->SelectGetNext();
	}

	
	CString msg;

	msg.Format(_T("%d ���� �ڷḦ ��ϴ�� ó�� �Ͻðڽ��ϱ�?"),nCount);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;

	INT ids = -1;
	CSeApi api(m_pParentMgr);
	CString strBookKey;
	
	m_pParentMgr->BeginWaitCursor();
	nIdx = m_pGrid->SelectGetHeadPosition();

	while(nIdx>=0)
	{
		strBookKey = _T("");
		strBookKey = m_pDM->GetCellData(_T("SB_åKEY"),nIdx);
		
		if(!strBookKey.IsEmpty()) {
			ids = api.BOOK_TRANSFER_TO_REG(m_pDM,strBookKey);
		}
		
		if(ids<0)
		{
			msg.Format(_T("%d ��° �׸��� �۾��� �����ϴ� �� ������ ���� \n �۾��� �ߴ��մϴ�."),nIdx);
			AfxMessageBox(msg);
			m_pParentMgr->EndWaitCursor();
			return -1;
		}
		else
		{
			m_pDM->SetCellData(_T("UDF_�ڷ����"),_T("����ΰ��ڷ�"),nIdx);
			m_pDM->SetCellData(_T("SB_�ڷ����"),_T("SER111N"),nIdx);
			m_pDM->SetCellData(_T("SB_�ΰ迩��"),_T("Y"),nIdx);
			m_pGrid->DisplayLine(nIdx);

		}

		nIdx = m_pGrid->SelectGetNext();
	}
	
	m_pParentMgr->EndWaitCursor();
	
	//�ΰ� ó���� �ڷ�� ����
	nIdx = m_pGrid->SelectGetTailPosition();
	
	while(nIdx>=0)
	{
		m_pDM->DeleteRow(nIdx);
		nIdx = m_pGrid->SelectGetPrev();
	}
	
	m_pGrid->Display();

	msg.Format(_T("%d ���� �ڷḦ ��ϴ�� ó�� �Ͽ����ϴ�!"),nCount);
	AfxMessageBox(msg); 

	return 0;
}

INT CTransferMgr::TransferToBinding()
{
	//���� ���(SEB111N) ó��
	m_pGrid->SelectMakeList();
	INT nCount = m_pGrid->SelectGetCount();
	if(0 == nCount) {
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return 0;
	}


	INT nIdx = m_pGrid->SelectGetHeadPosition();
	
	CString strWorkingStatus;

	while(nIdx>=0)
	{
		strWorkingStatus = _T("");
		strWorkingStatus = m_pDM->GetCellData(_T("SB_�ڷ����"),nIdx);
		
		if(_T("SEL112N") == strWorkingStatus) {
			AfxMessageBox(_T("���õ� �׸��� �̹� �ǽð� �谡ó���� �ڷᰡ �ֽ��ϴ�! \n Ȯ���� �ֽʽÿ�!"));
			return 0;
		}
	
		nIdx = m_pGrid->SelectGetNext();
	}

	
	CString msg;

	msg.Format(_T("%d ���� �ڷḦ ������� ó�� �Ͻðڽ��ϱ�?"),nCount);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;

	INT ids = -1;
	CSeApi api(m_pParentMgr);
	CString strBookKey;
	
	m_pParentMgr->BeginWaitCursor();
	nIdx = m_pGrid->SelectGetHeadPosition();


	while(nIdx>=0)
	{
		strBookKey = _T("");
		strBookKey = m_pDM->GetCellData(_T("SB_åKEY"),nIdx);
		
		if(!strBookKey.IsEmpty()) {
			ids = api.BOOK_TRANSFER_TO_BINDING(m_pDM,strBookKey);
		}
		
		if(ids<0)
		{
			msg.Format(_T("%d ��° �׸��� �۾��� �����ϴ� �� ������ ���� \n �۾��� �ߴ��մϴ�."),nIdx);
			AfxMessageBox(msg);
			m_pParentMgr->EndWaitCursor();
			return -1;
		}
		else
		{
			m_pDM->SetCellData(_T("UDF_�ڷ����"),_T("�����ΰ��ڷ�"),nIdx);
			m_pDM->SetCellData(_T("SB_�ڷ����"),_T("SEB111N"),nIdx);
			m_pDM->SetCellData(_T("SB_�ΰ迩��"),_T("Y"),nIdx);
			m_pGrid->DisplayLine(nIdx);

		}

		nIdx = m_pGrid->SelectGetNext();
	}
	
	m_pParentMgr->EndWaitCursor();
	
	msg.Format(_T("%d ���� �ڷḦ ������� ó�� �Ͽ����ϴ�!"),nCount);


	//�ΰ� ó���� �ڷ�� ����
	nIdx = m_pGrid->SelectGetTailPosition();
	
	while(nIdx>=0)
	{
		m_pDM->DeleteRow(nIdx);
		nIdx = m_pGrid->SelectGetPrev();
	}
	
	m_pGrid->Display();

	AfxMessageBox(msg);

	return 0;
}

INT CTransferMgr::TransferToShelf()
{
	//�ǽð� �谡(SEL112N);
	m_pGrid->SelectMakeList();
	INT nCount = m_pGrid->SelectGetCount();
	if(0 == nCount) {
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return 0;
	}


	INT nIdx = m_pGrid->SelectGetHeadPosition();
	
	CString strWorkingStatus;

	while(nIdx>=0)
	{
		strWorkingStatus = _T("");
		strWorkingStatus = m_pDM->GetCellData(_T("SB_�ڷ����"),nIdx);
		
		if(_T("SEL112N") == strWorkingStatus) {
			AfxMessageBox(_T("���õ� �׸��� �̹� �ǽð� �谡ó���� �ڷᰡ �ֽ��ϴ�! \n Ȯ���� �ֽʽÿ�!"));
			return 0;
		}
	
		nIdx = m_pGrid->SelectGetNext();
	}

	
	CString msg;

	msg.Format(_T("%d ���� �ڷḦ �ǽð� �谡 ó�� �Ͻðڽ��ϱ�?"),nCount);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;

	INT ids = -1;
	CSeApi api(m_pParentMgr);
	CString strBookKey;
	CString strVolKey;
	CString strSpeciesKey;
	CString strPrevWorkingStatus;

	m_pParentMgr->BeginWaitCursor();
	nIdx = m_pGrid->SelectGetHeadPosition();


	while(nIdx>=0)
	{
		strBookKey = _T("");
		strVolKey = _T("");
		strSpeciesKey = _T("");

		strBookKey = m_pDM->GetCellData(_T("SB_åKEY"),nIdx);
		strVolKey = m_pDM->GetCellData(_T("SB_��KEY"),nIdx);
		strSpeciesKey = m_pDM->GetCellData(_T("SB_��KEY"),nIdx);
		strPrevWorkingStatus = _T("SEL111O");


		ids = api.BOOK_TRANSFER_TO_SHELF(m_pDM,
										strPrevWorkingStatus,
										strSpeciesKey,
										strVolKey,
										strBookKey);

		if(ids<0)
		{
			msg.Format(_T("%d ��° �׸��� �۾��� �����ϴ� �� ������ ���� \n �۾��� �ߴ��մϴ�."),nIdx);
			AfxMessageBox(msg);
			m_pParentMgr->EndWaitCursor();
			return -1;
		}
		else
		{
			m_pDM->SetCellData(_T("UDF_�ڷ����"),_T("�ǽð��谡�ڷ�"),nIdx);
			m_pDM->SetCellData(_T("SB_�ڷ����"),_T("SEL112N"),nIdx);
			m_pDM->SetCellData(_T("SB_�ΰ迩��"),_T("Y"),nIdx);
			m_pGrid->DisplayLine(nIdx);

		}

		nIdx = m_pGrid->SelectGetNext();
	}
	
	m_pParentMgr->EndWaitCursor();
	
	msg.Format(_T("%d ���� �ڷḦ �ǽð� �谡 ó�� �Ͽ����ϴ�!"),nCount);

	//�ΰ� ó���� �ڷ�� ����
	nIdx = m_pGrid->SelectGetTailPosition();
	
	while(nIdx>=0)
	{
		m_pDM->DeleteRow(nIdx);
		nIdx = m_pGrid->SelectGetPrev();
	}
	
	m_pGrid->Display();

	AfxMessageBox(msg);
	return 0;
}
