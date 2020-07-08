// SpeciesDataMgr.cpp: implementation of the CSpeciesDataMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpeciesDataMgr.h"
#include "../INCLUDE/BO_ACQ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("������API"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpeciesDataMgr::CSpeciesDataMgr(CESL_Mgr* pParent /*=NULL*/)
{
	m_pParentMgr = pParent;
	m_pSpeciesDM = NULL;
	m_bDMCreate = FALSE;
	m_strSpeciesKey = _T("");

	nIndexSpeciesRelation = 0;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_���ı���_�ڵ�");			nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_��������");				nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_������ϱ���_�ڵ�");		nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_�����ڷᱸ��");			nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_�з�ǥ����");				nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_�ڷᱸ��_�ڵ�");			nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_���");					nIndexSpeciesRelation++;
	
}

CSpeciesDataMgr::~CSpeciesDataMgr()
{
	if (m_pSpeciesDM && m_bDMCreate) 
	{
		delete m_pSpeciesDM;
		m_pSpeciesDM = NULL;
	}
}

//��Key ����.
INT CSpeciesDataMgr::CreateRecKey(CString &strSpecieskey, CString &strAccukey, INT nSpeciesType)
{
	INT ids;
	ids = m_pSpeciesDM->MakeRecKey(strSpecieskey); 
	if(ids < 0) return -1;
	m_strSpeciesKey = strSpecieskey;

	SPECIES_TYPE speciesType = SPECIES_TYPE_INVALID;
	speciesType = (SPECIES_TYPE)nSpeciesType;
	if(speciesType == BO_BASIC || speciesType == BO_REG_INVALID) strAccukey = _T("-1");
	else 
	{
		ids = m_pSpeciesDM->MakeRecKey(strAccukey); 
		if(ids < 0) return -1;
	}
	return 0;
}

INT CSpeciesDataMgr::GetReckey(INT nRowIdx, CString strReckey)
{
	INT ids = -1;
	ids = m_pSpeciesDM->GetCellData(_T("BS_��KEY"), nRowIdx, strReckey);
	if(ids < 0) return -1;
	
	return 0;
}

// Default ���� ���� ��Ų��.
INT CSpeciesDataMgr::SetDefaultValue(INT nRowIdx, INT nOpenMode, INT nSpeciesType)
{
	if(!m_pSpeciesDM) return -1;

	SPECIES_TYPE sType = SPECIES_TYPE_INVALID;
	sType = (SPECIES_TYPE)nSpeciesType;

	CString worker = m_pParentMgr->m_pInfo->USER_ID;
	if(nOpenMode > 0)
	{
		m_pSpeciesDM->SetCellData(_T("BS_�Է���"), GetTodayDate(), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_�۾����±���"),_T("1"), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_��������"),_T("N"), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_�����۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_������������KEY"), _T("-1"), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_�۾���"), worker, nRowIdx);
		if(sType == BO_REG_INVALID || sType == BO_REG_PURCHASE || sType == BO_REG_DONATE)
			m_pSpeciesDM->SetCellData(_T("BS_����ΰ�����"), GetTodayDate(), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_�����ȣ"), _T(""), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_��������"), _T(""), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_DDC�з���ȣ"), _T(""), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_DDC����"), _T(""), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_KDC�з���ȣ"), _T(""), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_KDC����"), _T(""), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_�谡��ġ��ȣ"), _T(""), nRowIdx);
	}	
	m_pSpeciesDM->SetCellData(_T("BS_�������۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	
	return 0;
}

CString CSpeciesDataMgr::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CSpeciesDataMgr::INDEX_to_SPECIES(CIndexDataMgr *IndexMgr, INT nIndexRow, INT nSpeciesRow)
{
	INT ids;
	CString strAlias, strData;

	for (INT i = 0; i < nIndexSpeciesRelation; i++) {
		strAlias = IndexMgr->IndexSpeciesRelation_INDEX[i];
		ids = IndexMgr->m_pIndexDM->GetCellData(strAlias, nIndexRow, strData);
		ids = m_pSpeciesDM->SetCellData(IndexSpeciesRelation_SPECIES[i], strData, nSpeciesRow);
	}
	return 0;
}

INT CSpeciesDataMgr::SetDataMgr(CESL_DataMgr *pDM)
{
	if(m_pParentMgr == NULL) return -1;
	if(pDM == NULL)
	{
		m_pSpeciesDM = new CESL_DataMgr;
		if(m_pSpeciesDM == NULL) return -1;
		m_pSpeciesDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
		m_pSpeciesDM->InitDMPKFromFile(_T("DMUP_����_��_��������"));
		m_pSpeciesDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
		m_bDMCreate = TRUE;
	}
	else
	{
		m_pSpeciesDM = pDM;
	}
	return 0;
}

//���� ������ �̿����� ���� �ڵ� ����
INT CSpeciesDataMgr::Get_Priority_USELIMIT_CODE(CESL_DataMgr * pDM_BOOK, CESL_DataMgr * pDM_ACCU, CArray <CString, CString> RemoveAliasList,
												INT nRowIdx, INT mode /*=-1*/, CString strPrev_USELIMIT_CODE /*=_T("")*/)
{
	//mode ���� -1 : ���� ���� �̿����� ������ �����ϴ� ��� 1 : ���� ���� �̿����� ������ �缳�� �ϴ� ���
	CString strUSELIMIT_CODE, strTmpData, strDBFlag;
	strUSELIMIT_CODE.Empty();
	INT rowCnt = pDM_BOOK->GetRowCount();
	INT nPriority = -1;
	INT nMaxPriority = -1;
	if(mode > 0 && strPrev_USELIMIT_CODE.GetLength() > 0)
	{
		if(strPrev_USELIMIT_CODE == _T("GM")) nMaxPriority = 50;
		else if(strPrev_USELIMIT_CODE == _T("IZ")) nMaxPriority = 40;
		else if(strPrev_USELIMIT_CODE == _T("CA")) nMaxPriority = 30;
		else if(strPrev_USELIMIT_CODE == _T("CD")) nMaxPriority = 20;
		else if(strPrev_USELIMIT_CODE == _T("CL")) nMaxPriority = 10;
	}

	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, strDBFlag);
		if(strDBFlag == _T("D")) continue;
		pDM_BOOK->GetCellData(_T("BB_�̿����ѱ���_�ڵ�"), i, strTmpData);
		if(strTmpData.IsEmpty()) continue;
		strTmpData.TrimLeft();	strTmpData.TrimRight();		strTmpData.MakeUpper();
		if(strTmpData == _T("GM")) nPriority = 50;
		else if(strTmpData == _T("IZ")) nPriority = 40;
		else if(strTmpData == _T("CA")) nPriority = 30;
		else if(strTmpData == _T("CD")) nPriority = 20;
		else if(strTmpData == _T("CL")) nPriority = 10;
		if(nMaxPriority < nPriority) nMaxPriority = nPriority;
	}
	if(nMaxPriority < 0) return 0;
	else if(nMaxPriority == 50) strUSELIMIT_CODE = _T("GM");
	else if(nMaxPriority == 40) strUSELIMIT_CODE = _T("IZ");
	else if(nMaxPriority == 30) strUSELIMIT_CODE = _T("CA");
	else if(nMaxPriority == 20) strUSELIMIT_CODE = _T("CD");
	else if(nMaxPriority == 10) strUSELIMIT_CODE = _T("CL");

	m_pSpeciesDM->SetCellData(_T("BS_�̿����ѱ���_�ڵ�"), strUSELIMIT_CODE, nRowIdx);
	m_pSpeciesDM->SetCellData(_T("BS_��KEY"), m_strSpeciesKey, nRowIdx);
	m_pSpeciesDM->SetCellData(_T("BS_���Գ⵵"), pDM_ACCU->GetCellData(_T("BP_���Գ⵵"), nRowIdx), nRowIdx);
	//�������м���
	m_pSpeciesDM->SetCellData(_T("BS_��������_�ڵ�"), pDM_ACCU->GetCellData(_T("BP_��������_�ڵ�"), nRowIdx), nRowIdx);
	RemoveAliasList.RemoveAll();
	m_pSpeciesDM->InitDBFieldData();
	m_pSpeciesDM->MakeQueryByDM(RemoveAliasList, m_pSpeciesDM, _T("BO_SPECIES_TBL"), nRowIdx, m_pSpeciesDM);

	return 0;
}