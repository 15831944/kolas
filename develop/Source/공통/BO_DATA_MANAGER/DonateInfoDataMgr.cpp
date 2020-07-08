// DonateInfoDataMgr.cpp: implementation of the CDonateInfoDataMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DonateInfoDataMgr.h"
#include "../INCLUDE/BO_ACQ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDonateInfoDataMgr::CDonateInfoDataMgr(CESL_Mgr* pParent /*=NULL*/)
{
	m_pParentMgr = pParent;
	m_pDonateInfoDM = NULL;
	m_bDMCreate = FALSE;
}

CDonateInfoDataMgr::~CDonateInfoDataMgr()
{
	if (m_pDonateInfoDM && m_bDMCreate) 
	{
		delete m_pDonateInfoDM;
		m_pDonateInfoDM = NULL;
	}
}

INT CDonateInfoDataMgr::SetDefaultValue(CESL_DataMgr *pDM_ACCU, CESL_DataMgr *pDM_BOOK, INT nRowIdx, INT nOpenMode, INT nSpeciesType)
{
	INT ids = -1;
	SPECIES_TYPE sType = (SPECIES_TYPE)nSpeciesType;

	if (pDM_ACCU && pDM_BOOK) 
		if(sType == BO_DONATE || sType == BO_NONDONATE || sType == BO_REG_DONATE || (sType == BO_REGOBJ && pDM_BOOK->GetCellData(_T("BB_���Ա���_�ڵ�"), 0) == _T("2")))
			ids = 0;
	
	if(ids < 0 ) return -1;

	CString ORDER_BOOK_CNT, INPUT_TYPE, INPUT_DATE;
	ORDER_BOOK_CNT.Format(_T("%d"), GetBookCount(pDM_BOOK));
	INPUT_TYPE = _T("0");
	if (nOpenMode == 1100 || nOpenMode == 3100 || nOpenMode == 1200) INPUT_TYPE = _T("0");
	if (nOpenMode == -6000) INPUT_TYPE = _T("1");
	//if (m_nOpenMode == -5000) INPUT_TYPE = _T("2");
	if (nOpenMode == 5000 || nOpenMode == 5100) INPUT_TYPE = _T("2");
	if (nOpenMode == 4000) INPUT_TYPE = _T("3");
	INPUT_DATE = GetTodayDate();

	m_pDonateInfoDM->SetCellData(_T("BP_����å��"), ORDER_BOOK_CNT, nRowIdx);
	if(nOpenMode > 0)
	{
		m_pDonateInfoDM->SetCellData(_T("BP_�Է±���"), INPUT_TYPE, nRowIdx);
		m_pDonateInfoDM->SetCellData(_T("BP_�Է�����"), INPUT_DATE, nRowIdx);
		m_pDonateInfoDM->SetCellData(_T("BP_���Գ⵵"), GetTodayYear(), nRowIdx);
		m_pDonateInfoDM->SetCellData(_T("BP_��������"), GetTodayDate(), nRowIdx);
		m_pDonateInfoDM->SetCellData(_T("BP_�˼���"), GetTodayDate(), nRowIdx);
	}
	return 0;
}
INT CDonateInfoDataMgr::SetDataMgr(CESL_DataMgr *pDM)
{
	if(m_pParentMgr == NULL) return -1;
	if(pDM == NULL)
	{
		m_pDonateInfoDM = new CESL_DataMgr;
		if(m_pDonateInfoDM == NULL) return -1;
		m_pDonateInfoDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
		m_pDonateInfoDM->InitDMPKFromFile(_T("DMUP_����_��_��������"));
		m_pDonateInfoDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
		m_bDMCreate = TRUE;
	}
	else
	{
		m_pDonateInfoDM = pDM;
	}
	return 0;

}

INT CDonateInfoDataMgr::SetSpeciesKey(CString &strSpeciesKey, INT nRowIdx)
{
	INT ids = -1;
	ids = m_pDonateInfoDM->SetCellData(_T("BP_��KEY"), strSpeciesKey, nRowIdx);
	if(ids < 0) return -1;

	return 0;
}

INT CDonateInfoDataMgr::GetBookCount(CESL_DataMgr *pDM_Book, CString strRegCode /*= _T("")*/, LPCTSTR lpszVol /*=NULL*/)
{
	if (pDM_Book == NULL) return -1;

	INT nReturnCnt = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// å�� ���� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx)) continue;

		// ��ϱ��� ��
		if (!strRegCode.IsEmpty() && lpszVol != _T("BB_��ǰ����"))
		{
			if (pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), idx) != strRegCode) continue;
		}
		else if(!strRegCode.IsEmpty() && lpszVol == _T("BB_��ǰ����"))
		{
			if (pDM_Book->GetCellData(lpszVol, idx) != strRegCode) continue;	
		}
			
		nReturnCnt++;
	}

	return nReturnCnt;
}
BOOL CDonateInfoDataMgr::IsBook(CESL_DataMgr *pDM_Book, INT idx, LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_å�η��÷���"), idx) == _T("A")) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D")) return FALSE;

	// ������� ������ üũ
	if (lpszVol != NULL)
		if (pDM_Book->GetCellData(_T("BB_�����"), idx) != lpszVol) return FALSE;

	return TRUE;
}
CString CDonateInfoDataMgr::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}
CString CDonateInfoDataMgr::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}
