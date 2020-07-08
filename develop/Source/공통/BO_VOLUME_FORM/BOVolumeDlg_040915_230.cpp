// BOVolumeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOVolumeDlg.h"
#include "BOISBNAdditionDlg.h"

//#include _T("BOAppendixDlg.h")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOVolumeDlg dialog
CBOVolumeDlg::CBOVolumeDlg(CESL_Mgr* pParent /*=NULL*/, CESL_DataMgr *pDM_Book /*= NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOVolumeDlg)
	//}}AFX_DATA_INIT

	m_pDM_Book = pDM_Book;
	m_nOpenMode = -1;

	m_pFormApi = NULL;
	m_pFormApi = new CBOFormApi(this);
	
	m_pBOSpeciesApi = NULL;

	m_pDM_Volume = NULL;
	m_pCM = NULL;

	m_nSelectedGrid = -1;
	m_nSelectedRow = -1;

	m_pDM_BookHold = NULL;
	m_pDM_VolumeHold = NULL;


	m_bShowHoldInfo = FALSE;
	m_bShowHoldInfoButton = FALSE;


	m_pGrid_Volume = NULL;
	m_pGrid_VolumeHold = NULL;
	m_pGrid_Book = NULL;
	m_pGrid_BookHold = NULL;

	m_bReadOnly = FALSE;

	m_nTabOrder = VOLUME_FIRST;
}

CBOVolumeDlg::~CBOVolumeDlg()
{
	if (m_pFormApi != NULL)
	{
		delete m_pFormApi;
		m_pFormApi = NULL;
	}

	if (m_pBOSpeciesApi != NULL)
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}	
}

BOOL CBOVolumeDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBOVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOVolumeDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOVolumeDlg, CDialog)
	//{{AFX_MSG_MAP(CBOVolumeDlg)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_eREG_CLASS_CODE, OnKillfocuseREGCLASSCODE)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bINSERT, OnbINSERT)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VOLUME_BOOK, OnSelchangeTabVolumeBook)
	ON_BN_CLICKED(IDC_bVIEW_HOLD_INFO, OnbVIEWHOLDINFO)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bCLEARSCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_b505TAG, Onb505TAG)
	ON_BN_CLICKED(IDC_bISBN_CHECK, OnbISBNCHECK)
	ON_BN_CLICKED(IDC_bSELECT_REP_BOOK, OnbSELECTREPBOOK)
	ON_BN_CLICKED(IDC_bISBN_INSERT, OnbISBNINSERT)
	ON_EN_KILLFOCUS(IDC_eCOPY_CODE, OnKillfocuseCOPYCODE)
	ON_BN_CLICKED(IDC_btnAPPLY_SERIES, OnbtnAPPLYSERIES)
	ON_CBN_SELCHANGE(IDC_cMEDIA_CLASS, OnSelchangecMEDIACLASS)
	ON_BN_CLICKED(IDC_btnVOL_FIND_FILE1, OnbtnVOLFINDFILE1)
	ON_BN_CLICKED(IDC_btnVOL_FIND_FILE3, OnbtnVOLFINDFILE3)
	ON_BN_CLICKED(IDC_btnVOL_FIND_FILE2, OnbtnVOLFINDFILE2)
	ON_BN_CLICKED(IDC_btnVOL_EBOOK_VIEW, OnbtnVOLEBOOKVIEW)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_EN_KILLFOCUS(IDC_eBOOK_PRICE, OnKillfocuseBOOKPRICE)
	//}}AFX_MSG_MAP
	ON_MESSAGE(MSG_GRID_DISPLAY, OnGridDisplay)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBOVolumeDlg Operations
INT CBOVolumeDlg::SetParentInfo(CString strSpeciesKey, CString strPurchaseKey)
{
	if (strSpeciesKey.IsEmpty()) return -1;
	
	m_strSpeciesKey = strSpeciesKey;
	m_strPurchaseKey = strPurchaseKey;

	return 0;
}

INT CBOVolumeDlg::SetHoldInfoUI(BOOL bShowHoldInfo, BOOL bShowHoldInfoButton)
{
	m_bShowHoldInfo = bShowHoldInfo;
	m_bShowHoldInfoButton = bShowHoldInfoButton;

	return 0;
}

INT CBOVolumeDlg::Init()
{
	// �� API
	if (m_pBOSpeciesApi) 
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}
	m_pBOSpeciesApi = new CBO_SPECIES(this);
	m_pBOSpeciesApi->m_nOpenMode = GetOpenMode();

	// DM ���� 
	if (m_pDM_Book == NULL)
	{
		m_pDM_Book = FindDM(_T("DMUP_����_��_å����"));
	}

	m_pDM_Volume = FindDM(_T("DMUP_����_��_������"));
	
	m_pDM_BookHold = FindDM(_T("DMUP_����_��_����å����"));
	m_pDM_VolumeHold = FindDM(_T("DMUP_����_��_���������"));

	if (m_pDM_Book == NULL ||
		m_pDM_Volume == NULL ||
		m_pDM_BookHold == NULL ||
		m_pDM_VolumeHold == NULL
		) 
		return -1;

	// CM ����
	m_pCM = FindCM(_T("CMUP_����_��_å����"));
	if (m_pCM == NULL) return -1;

	// Grid ����

	m_pGrid_Volume		= (CESL_Grid*)m_pCM->FindControl(_T("GRID_VOLUME"));
	m_pGrid_VolumeHold	= (CESL_Grid*)m_pCM->FindControl(_T("GRID_VOLUME_HOLD"));
	m_pGrid_Book		= (CESL_Grid*)m_pCM->FindControl(_T("GRID_BOOK"));
	m_pGrid_BookHold	= (CESL_Grid*)m_pCM->FindControl(_T("GRID_BOOK_HOLD"));

	if (m_pGrid_Volume == NULL ||
		m_pGrid_VolumeHold == NULL ||
		m_pGrid_Book == NULL||
		m_pGrid_BookHold == NULL
		)
	return -1;	

	// DB�������� ����
	this->SetCONNECTION_INFO(m_pDM_Book->CONNECTION_INFO);
	
	// �� DM ����
	
	// �������� ������ ������ �����Ѵ�.
	if (!GetDupIdxKey().IsEmpty()) 
	{
		// ���� å �����
		CString strOption;
		strOption.Format(_T("B.REC_KEY = E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.BOOK_APPENDIX_FLAG='B'"), GetDupIdxKey());
		m_pFormApi->MakeBookDM(strOption, m_pDM_BookHold);
			
		// ���� �� �����
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_BookHold, m_pDM_VolumeHold);
		m_bShowHoldInfoButton = TRUE;
		m_bShowHoldInfo = TRUE;
	}

	// Open Mode �� ���� ...
	if (GetOpenMode() == 1100)
	{
		m_pFormApi->SetAppMode(MODE_INSERT);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
	}
	else if (GetOpenMode() == -1100)
	{
		m_pFormApi->SetAppMode(MODE_UPDATE);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
	}
	
	// - ����-��-�Է� : 2100
	else if (GetOpenMode() == 2100)
	{
		m_bShowHoldInfoButton = FALSE;
		m_pFormApi->SetAppMode(MODE_INSERT);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_b505TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcUSE_LIMIT_CODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cUSE_LIMIT_CODE)->ShowWindow(SW_SHOW);

		// å �׸����� DM ������ ������ �ٲ۴�.
		CString strLinkDM;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DMUP_����_��_å����"), strLinkDM);
		m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
		m_pGrid_Book->InitColumnLinkDMAndAlias();
		m_pGrid_Book->InitColumnInfo();
	}
	// - ����-��-���� : -2100
	else if (GetOpenMode() == -2100)
	{
		m_bShowHoldInfoButton = FALSE;
		m_pFormApi->SetAppMode(MODE_UPDATE);

		GetDlgItem(IDC_b505TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcUSE_LIMIT_CODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cUSE_LIMIT_CODE)->ShowWindow(SW_SHOW);
		// å �׸����� DM ������ ������ �ٲ۴�.
		CString strLinkDM;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DMUP_����_��_å����"), strLinkDM);
		m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
		m_pGrid_Book->InitColumnLinkDMAndAlias();
		m_pGrid_Book->InitColumnInfo();
		//������� �°� ���Ϸù�ȣ�� �����Ѵ�.
		//AdjustCurrentBookDM(m_pDM_Book);
	}
	// ����-��-�Է�
	else if (GetOpenMode() == 3100)
	{
		GetDlgItem(IDC_b505TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
	}
	// ����-��-����
	else if (GetOpenMode() == -3100)
	{
		GetDlgItem(IDC_b505TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_SHOW);
	}
	// - ��Ȱ�� ���� : -7100
	else if (GetOpenMode() == -7100)
	{
		m_bReadOnly = TRUE;
	}
	// - ������ �󼼺��� : 8000
	else if (GetOpenMode() == 8000)
	{
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnAPPLY_SERIES)->ShowWindow(SW_HIDE);
		m_bShowHoldInfoButton = FALSE;
		if (m_strSpeciesKey.IsEmpty()) return -1;
		// DM �����Ѵ�.
		CString strOption;
		INT ids;

		if (m_strPurchaseKey.IsEmpty() || m_strPurchaseKey == _T("ALL"))
			strOption.Format(_T("B.REC_KEY = E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY B.REPRESENT_BOOK_YN DESC"), m_strSpeciesKey);
		else
			strOption.Format(_T("B.REC_KEY = E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.ACQ_KEY = %s AND B.REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY B.REPRESENT_BOOK_YN DESC"), m_strSpeciesKey, m_strPurchaseKey);
		//ids = m_pDM_Book->RefreshDataManager(strOption);
		ids = m_pFormApi->MakeBookDM(strOption, m_pDM_Book);
		if (ids < 0) return -1;

		GetDlgItem(IDC_btnVOL_FIND_FILE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnVOL_FIND_FILE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnVOL_FIND_FILE3)->ShowWindow(SW_HIDE);
		
		// å �׸����� DM ������ ������ �ٲ۴�.
		CString strLinkDM = m_pDM_Book->DB_MGR_ALIAS;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DMUP_����_��_å����"), strLinkDM);
		m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
		m_pGrid_Book->InitColumnLinkDMAndAlias();
		m_pGrid_Book->InitColumnInfo();

		m_bReadOnly = TRUE;
	}
	// - �����߱� �� ������ �󼼺��� : -8000
	else if (GetOpenMode() == -8000)
	{
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnAPPLY_SERIES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_bCLEARSCREEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnVOL_FIND_FILE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnVOL_FIND_FILE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnVOL_FIND_FILE3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_bINSERT)->EnableWindow(FALSE);
		GetDlgItem(IDC_bDELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_b505TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcUSE_LIMIT_CODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cUSE_LIMIT_CODE)->ShowWindow(SW_SHOW);

		m_bShowHoldInfoButton = FALSE;
		
		// å �׸����� DM ������ ������ �ٲ۴�.
		CString strLinkDM = m_pDM_Book->DB_MGR_ALIAS;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DMUP_����_��_å����"), strLinkDM);
		m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
		m_pGrid_Book->InitColumnLinkDMAndAlias();
		m_pGrid_Book->InitColumnInfo();
	}
	//������ȣ �ο���� ����
	m_cCopyCodeMgr.Init(m_pDM_Book->CONNECTION_INFO);
	m_pFormApi->SetCopyCodeMgr(&m_cCopyCodeMgr);
	
	return 0;
}

INT CBOVolumeDlg::Display()
{
	if (m_nSelectedRow < 0) return -1;

	CESL_ControlMgr *pCM = FindCM(_T("CMUP_����_��_å����"));
	if (pCM == NULL) return -1;

	CESL_Control_Element * pCE = NULL;

	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		if (pCE->CTRL_TYPE == _T("G")) continue;

		pCE->Display(m_nSelectedRow);
	}

	m_pFormApi->DisplayISBN(pCM, _T("BB_����ISBN"));
	ShowRegCodeField();
//	ShowRegNo();
	OnKillfocuseREGCLASSCODE();

	if (m_bReadOnly)
		DisplayControlReadOnly();


	CString strMediaCode;
	m_pCM->GetControlMgrData(_T("BB_��ü����_�ڵ�"), strMediaCode);
	if(strMediaCode != m_strPrevMediaCode && (strMediaCode == _T("EB") || m_strPrevMediaCode == _T("EB")))
		ShowCodeResourceByMediaCode(strMediaCode);
	m_strPrevMediaCode = strMediaCode;
	
	ShowPrevNextButton();
	
    return 0;
}

INT CBOVolumeDlg::ClearScreen()
{
	CESL_ControlMgr *pCM = FindCM(_T("CMUP_����_��_å����"));
	if (pCM == NULL) return -1;

	CESL_Control_Element * pCE = NULL;

	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		if (pCE->CTRL_TYPE == _T("G")) continue;

		pCE->Clear();
	}

	ShowRegCodeField();
	OnKillfocuseREGCLASSCODE();

	return 0;
}

INT CBOVolumeDlg::ShowRegCodeField()
{
	// ��ϱ��� �����ֱ�
	CString strRegCodeField;

	CESL_DataMgr *pDM = GetActiveDM();
	if (pDM == NULL) return -1;

	// �� ����
	if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
	{
		CString strVol = pDM->GetCellData(_T("BB_�����"), m_nSelectedRow);
		CArray<CRegCode, CRegCode> arrRegCode;
		if (m_nSelectedGrid == GRID_VOLUME)
			m_pFormApi->GetRegCodeInfo(m_pDM_Book, &arrRegCode, strVol);
		else if (m_nSelectedGrid == GRID_VOLUME_HOLD)
			m_pFormApi->GetRegCodeInfo(m_pDM_BookHold, &arrRegCode, strVol);
		CRegCode regCode;
		CString strTemp;
		
		INT nArrCount = arrRegCode.GetSize();
		if (nArrCount == 0) 
		{
			if (strRegCodeField.IsEmpty()) strRegCodeField = _T("EM");
			GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegCodeField);
			return 0;
		}
		for (INT i = 0; i < nArrCount - 1; i++)
		{
			regCode = arrRegCode.GetAt(i);
			strTemp.Format(_T("%s%d/"), regCode.m_strRegCode, regCode.m_nCount);
			strRegCodeField += strTemp;
		}
		regCode = arrRegCode.GetAt(nArrCount - 1);
		strTemp.Format(_T("%s%d"), regCode.m_strRegCode, regCode.m_nCount);
		strRegCodeField += strTemp;
		
		if (strRegCodeField.IsEmpty()) strRegCodeField = _T("EM");
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegCodeField);
	}
	// å����
	else
	{
		strRegCodeField = pDM->GetCellData(_T("BB_��ϱ���_�ڵ�"), m_nSelectedRow);
		CString strBookAppFlag = pDM->GetCellData(_T("BB_å�η��÷���"), m_nSelectedRow);
		if (strBookAppFlag != _T("A"))
			if (strRegCodeField.IsEmpty()) strRegCodeField = _T("EM");
		
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegCodeField);
	}

	return 0;
}

INT CBOVolumeDlg::ShowRegNo()
{
	CESL_DataMgr *pDM = GetActiveDM();
	if (pDM == NULL) return -1;
	
	CString strRegNo = pDM->GetCellData(_T("BB_��Ϲ�ȣ"), m_nSelectedRow);
	if (strRegNo.GetLength() < 3) 
		strRegNo.Empty();
	else
		strRegNo = strRegNo.Mid(2);

	return 0;
}

BOOL CBOVolumeDlg::IsDuplicateVolSortNo(CString strVolSortNo, INT nSelectedRow)
{
	CString strOriginVolSortNo;
	INT nRowCount = m_pDM_Volume->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if(idx == nSelectedRow) continue;
		m_pDM_Volume->GetCellData(_T("BB_���Ϸù�ȣ"), idx, strOriginVolSortNo);
		if(strVolSortNo == strOriginVolSortNo) return TRUE;
	}

	return FALSE;
}

BOOL CBOVolumeDlg::IsDuplicateVolume(CString strVol, INT nSelectedRow)
{
	//if (strVol.IsEmpty()) return FALSE;
	
	CString strOriginVol;
	INT nRowCount = m_pDM_Volume->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if(idx == nSelectedRow) continue;
		m_pDM_Volume->GetCellData(_T("BB_�����"), idx, strOriginVol);
		if (strVol == strOriginVol) 
		{
			return TRUE;
		}
	}
	return FALSE;
}

INT CBOVolumeDlg::MakeStandardBookFromCM(CStringArray *pArrAbortList /*= NULL*/)
{
	CESL_ControlMgr *pCM = FindCM(_T("CMUP_����_��_å����"));
	if (pCM == NULL) return -1;

	// ������ �ο찡 ����մ��� üũ
	DWORD *row;
	INT nRowCount = m_pDM_Book->GetRowCount();
	INT nIdx = nRowCount -1;
	POSITION pos = m_pDM_Book->data.FindIndex(nRowCount - 1);
	if (pos)
	{
		row = (DWORD*)m_pDM_Book->data.GetAt(pos);
		if (row != NULL)
		{
			TCHAR *pCell = NULL;
			INT nColCount = m_pDM_Book->RefList.GetCount();
			for (INT idx = 0; idx < nColCount; idx++)
			{
				pCell = (TCHAR*)row[idx];
				if (pCell == NULL) continue;
				
				if (_tcscmp(pCell, _T("")) != 0)
				{
					nIdx = - 1;
					break;
				}
			}
		}
	}

	m_pFormApi->MakeDMFromCM(pCM, m_pDM_Book, nIdx, _T(""), pArrAbortList);

	nIdx = m_pDM_Book->GetRowCount() - 1;

	// DB Flag ����
	CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_INSERT);
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, nIdx);

	//�̿����ѱ��� ����, å �ڷ���� ���� 
	CString strWorkingStatus;
	INT nRepresentBookIdx = -1;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nRepresentBookIdx);
	if(nRepresentBookIdx < 0) nRepresentBookIdx = 0;
	m_pDM_Book->SetCellData(_T("BB_�̿����ѱ���_�ڵ�"), m_pDM_Book->GetCellData(_T("BB_�̿����ѱ���_�ڵ�"), nRepresentBookIdx), nIdx);
	//�ڷ���� ���� �谡 ������ �ڷῡ ���Ͽ� �ұ� �Է��� ��� �谡����ڷᰡ �ƴѰ�� ������ �谡�ڷ� ���¸� ������ �Ѵ�.
	strWorkingStatus = m_pDM_Book->GetCellData(_T("BB_�ڷ����"), nRepresentBookIdx);
	if(strWorkingStatus.Left(3) == _T("BOL") && strWorkingStatus != _T("BOL111O")) strWorkingStatus = _T("BOL112N");
	m_pDM_Book->SetCellData(_T("BB_�ڷ����"), strWorkingStatus, nIdx);

	// å�����϶� ������ȣ �߰�
	if (m_nSelectedGrid == GRID_BOOK || m_nSelectedGrid == GRID_BOOK_HOLD)
	{
		CString strVol = m_pDM_Book->GetCellData(_T("BB_�����"), nIdx);
		INT nDupNumber = m_pFormApi->GetLastDupCodeNumber(m_pDM_Book, strVol);

		CString strDupCode;
		strDupCode.Format(_T("%d"), ++nDupNumber);
		m_pDM_Book->SetCellData(_T("BB_������ȣ"), strDupCode, nIdx);
	}

	return 0;
}

INT CBOVolumeDlg::ApplyRegCode()
{
	// ��ϱ��п� �°� å �� ����
	// �� ����
	if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
	{
		
		CString strRegCodeField;
		GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCodeField);

		CArray<CRegCode, CRegCode> arrRegCode;
		INT ids = m_pFormApi->FetchRegCode(strRegCodeField, &arrRegCode);
		if (ids < 0)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
			return -1;
		}

		m_strRegCodeField = strRegCodeField;
		INT nRegCodeCount = arrRegCode.GetSize();
		if (nRegCodeCount <= 0 ) return -1;

		CString strVol;
		GetControlData(_T("CMUP_����_��_å����"), _T("BB_�����"), strVol);

		m_pFormApi->MakeBookFromRegCode(&arrRegCode, m_pDM_Book, strVol);
		// å�� Working Status�� �ִ´�.
		INT nRowCount = m_pDM_Book->GetRowCount();
		INT nRepresentBookIdx;
		m_pFormApi->GetRepresentationBook(m_pDM_Book, nRepresentBookIdx);
		if(nRepresentBookIdx < 0) nRepresentBookIdx = 0;
		
		CString strBookWorkingStatus = _T("");
		for (INT idx = 0; idx < nRowCount; idx++)
		{
			// å�� �ƴϸ� �Ѿ��.
			if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
			// å�� �̹� �۾����°� ������ ���¶�� continue
			m_pDM_Book->GetCellData(_T("BB_�ڷ����"), idx, strBookWorkingStatus);
			if(strBookWorkingStatus.IsEmpty())
				m_pDM_Book->SetCellData(_T("BB_�ڷ����"), m_pDM_Book->GetCellData(_T("BB_�ڷ����"), nRepresentBookIdx), idx);
			if(strBookWorkingStatus.Left(3) == _T("BOR") && m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("I"))
				m_pDM_Book->SetCellData(_T("BB_�ڷ����"), _T("BOR111N"), idx);
		}		
	}
	// å����
	else
	{
		CString strRegCode, strVol, strSeShelfCode, strCopyCode;
		GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCode);

		// �´� ��ϱ������� üũ
		if (!m_pFormApi->IsValidRegCode(strRegCode))
		{
			AfxMessageBox(_T("��ϱ��а��� �ùٸ��� �ʽ��ϴ�."));
			return -1;
		}
		// å�η� �÷��� �߰�
		m_pDM_Book->SetCellData(_T("BB_å�η��÷���"), _T("B"), m_nSelectedRow);
		//��ϱ��м���
		m_pDM_Book->SetCellData(_T("BB_��ϱ���_�ڵ�"), strRegCode, m_nSelectedRow);
/*		// ������ȣ ����
		INT ids = SetCopyCode(m_pDM_Book, m_nSelectedRow);
		if(ids < 0) return -1;
*/	}

	return 0;
}

INT CBOVolumeDlg::ApplySaveData()
{
 	CESL_ControlMgr *pCM = FindCM(_T("CMUP_����_��_å����"));
	if (pCM == NULL) return -1;

	INT ids;
	// ����-��-����-�Է� || // ����-��-����-���� || // - ����-��-���� : -2100
	if (GetOpenMode() == 1100 || GetOpenMode() == -1100 || GetOpenMode() == 2100 || GetOpenMode() == -2100 || GetOpenMode() == -8000)
	{
		// �� ���¿��� ������ ������
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			ids = SaveVolume(pCM);
			if(ids < 0) return ids;
		}
		// å ���¿��� ������ ������
		else
		{
			ids = SaveBook(pCM);
			if(ids < 0) return ids;
		}
	}
	
	return 0;
}

INT CBOVolumeDlg::ApplyInsertData()
{
	INT ids;
	// ����-��-����-�Է� || // ����-��-����-���� || // - ����-��-���� : -2100
	if (GetOpenMode() == 1100 || GetOpenMode() == -1100 || GetOpenMode() == 2100 || GetOpenMode() == -2100)
	{
		// �źθ���Ʈ ����� (å���� �������ϰ�..)
		m_pFormApi->InitDefaultIgnoreAlias();
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��ǥå����"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��Ϲ�ȣ"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_������ȣ"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_EBOOKKEY"));

		// �� ����
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			ids = InsertVolume();
			if(ids < 0) return ids;
		}
		// å����
		else
		{
			ids = InsertBook();
			if(ids < 0) return ids;
		}
	}
	return 0;
}

INT CBOVolumeDlg::ApplyDeleteData()
{
	INT ids;
	// ����-��-����-�Է� || // ����-��-����-���� || // - ����-��-���� : -2100
	if (GetOpenMode() == 1100 || GetOpenMode() == -1100 || GetOpenMode() == 2100 ||  GetOpenMode() == -2100)
	{
		// �� ����
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			ids = DeleteVolume();
			if(ids < 0) return ids;
		}
		// å����
		else
		{
			ids = DeleteBook();
			if(ids < 0) return ids;
		}
	}

	if (m_nSelectedRow < 0) m_nSelectedRow = 0;

	// �������� �ƴϸ� ��ǥå ���� �ڵ� ����
	if (GetDupIdxKey().IsEmpty())
		m_pFormApi->SetRepresentationBook(m_pDM_Book);
	
	return 0;
}

CESL_DataMgr *CBOVolumeDlg::GetActiveDM()
{
	CESL_DataMgr *pDM = NULL;
	if (m_nSelectedGrid == GRID_VOLUME)
	{
		pDM = m_pDM_Volume;
	}
	else if (m_nSelectedGrid == GRID_VOLUME_HOLD)
	{
		pDM = m_pDM_VolumeHold;
	}
	else if (m_nSelectedGrid == GRID_BOOK)
	{
		pDM = m_pDM_Book;
	}
	else if (m_nSelectedGrid == GRID_BOOK_HOLD)
	{
		pDM = m_pDM_BookHold;
	}

	return pDM;
}

INT CBOVolumeDlg::LinkCMWithActiveDM()
{
	CESL_DataMgr *pDM = GetActiveDM();
	if (pDM == NULL) return -1;

	return LinkCMWithDM(m_pCM, pDM);

}

INT CBOVolumeDlg::LinkCMWithDM(CESL_ControlMgr *pCM, CESL_DataMgr *pDM)
{
	if (pCM == NULL || pDM == NULL) return -1;
	
	// DM�� Alias ���ϱ�
	CString strDMAlias = pDM->DB_MGR_ALIAS;


	// CM�� ��Ʈ�� ���鼭 link ��ų�� ã�� DM�� Alias�� �ٲ۴�.
	CESL_Control_Element *pCE = NULL;
	POSITION pos = pCM->Control_List.GetHeadPosition();

	INT ids = -1;
	CString strLinkData;
	CString strLinkDM;
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		
		// Grid�� ��� �Ѿ��.
		if (pCE->CTRL_TYPE == _T("G")) continue;

		 strLinkData = pCE->LINK_DATA;
		
		 ids = GetLinkDM(strLinkData, strLinkDM);
		 if (ids < 0) continue;

		 strLinkData.Replace(strLinkDM, strDMAlias);

		 pCE->LINK_DATA = strLinkData;
	}

	return 0;
}

INT CBOVolumeDlg::GetLinkDM(CString strLinkData, CString &strLinkDM)
{
	if (strLinkData.IsEmpty()) return -1;

	if (strLinkData.GetAt(0) == ':')
	{
		INT nFind = strLinkData.Find('.', 1);
		if (nFind < 0) return -1;

		strLinkDM = strLinkData.Mid(1, nFind - 1);
	}

	return 0;
}

CESL_Grid *CBOVolumeDlg::GetActiveGrid()
{
	CESL_Grid *pGrid = NULL;
	if (m_nSelectedGrid == GRID_VOLUME)
		pGrid = m_pGrid_Volume;
	else if (m_nSelectedGrid == GRID_VOLUME_HOLD)
		pGrid = m_pGrid_VolumeHold;
	else if (m_nSelectedGrid == GRID_BOOK)
		pGrid = m_pGrid_Book;
	else if (m_nSelectedGrid == GRID_BOOK_HOLD)
		pGrid = m_pGrid_BookHold;
	
	return pGrid;
}

INT CBOVolumeDlg::SetActiveGrid(INT nSelectedGrid)
{
	if (nSelectedGrid == -1) return -1;
	
	m_nSelectedGrid = nSelectedGrid;

	if (m_nSelectedRow < 0) return -1;
	
	GridAllClearReverse();

	DisplayButtons();
	
	// CM�� ActiveDM�� �������ش�.
	LinkCMWithActiveDM();
	
	return 0;
}

INT CBOVolumeDlg::GridAllClearReverse()
{
	CESL_Control_Element *pCE = NULL;
	POSITION pos = m_pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;

		if (pCE->CTRL_TYPE == _T("G")) 
		{
			GridClearReverse((CESL_Grid*)pCE->Control);
		}
	}

	return 0;
}

INT CBOVolumeDlg::GridClearReverse(CESL_Grid *pGrid)
{
	if (pGrid == NULL) return -1;

	COLORREF crWhite = RGB(255, 255, 255);
	COLORREF crBlack = RGB(0, 0, 0);

	INT nReverse = pGrid->m_nReverse;

	INT nCol = pGrid->GetCol();
	INT nRow = pGrid->GetRow();

	INT nCols = pGrid->GetCols(0);

	if (nReverse >= 0 && nReverse < pGrid->GetRows() - 1)
	{
		pGrid->SetRow(nReverse + 1);
		for (INT i = 1; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(crWhite);
			pGrid->SetCellForeColor(crBlack);
		}
	}

	pGrid->SetRow(nRow);
	pGrid->SetCol(nCol);
		
	return 0;
}

INT CBOVolumeDlg::DisplayButtons()
{
	INT nButtonCount = 0;
	INT nArrButtonCtrl[16];
	
	nArrButtonCtrl[nButtonCount++] = IDC_bSAVE;
	nArrButtonCtrl[nButtonCount++] = IDC_bDELETE;
	nArrButtonCtrl[nButtonCount++] = IDC_bINSERT;
	nArrButtonCtrl[nButtonCount++] = IDC_bCLEARSCREEN;
	nArrButtonCtrl[nButtonCount++] = IDC_b505TAG;
	nArrButtonCtrl[nButtonCount++] = IDC_btnAPPLY_SERIES;

	CWnd *pWnd = NULL;
	
	for (INT i = 0; i < nButtonCount; i++)
	{
		pWnd = GetDlgItem(nArrButtonCtrl[i]);
		if (pWnd == NULL) continue;

		if (m_bReadOnly)// && nArrButtonCtrl[i] != IDC_bAPPENDIX_INFO)
		{			
			pWnd->EnableWindow(FALSE);
		}
		else if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_BOOK)
		{
			pWnd->EnableWindow(TRUE);
		}			
		else if (m_nSelectedGrid == GRID_VOLUME_HOLD || m_nSelectedGrid == GRID_BOOK_HOLD)
			pWnd->EnableWindow(FALSE);
		else
			return -1;
	}

	//OpenMode�� -8000�̸� �߰�/����/����ó��/ȭ������� Disable
	if(GetOpenMode() == -8000)
	{
		GetDlgItem(IDC_bINSERT)->EnableWindow(FALSE);
		GetDlgItem(IDC_bDELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnAPPLY_SERIES)->EnableWindow(FALSE);
	}
	return 0;
}

INT CBOVolumeDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	strGridAlias.TrimLeft();		strGridAlias.TrimRight();
	CESL_DataMgr * pDM = GetActiveDM();
	if(!pDM) return 0;
	if(m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD) 
		return 0;

	if(strDMFieldAlias == _T("UDF_å�ڷ����"))
	{
		CString strWorkingStatus = pDM->GetCellData(_T("BB_�ڷ����"), nRow);
		if(strWorkingStatus == _T("BOT111O")) str = _T("�����ڷ�");
		else if(strWorkingStatus == _T("BOT112O")) str = _T("�����ڷ�");
		else if(strWorkingStatus == _T("BOA111N")) str = _T("���Դ���ڷ�");
		else if(strWorkingStatus == _T("BOA112N")) str = _T("�ֹ��ڷ�");
		else if(strWorkingStatus == _T("BOA113O")) str = _T("�̳�ǰ�ڷ�");
		else if(strWorkingStatus == _T("BOA114O")) str = _T("�����ΰ����ڷ�");
		else if(strWorkingStatus == _T("BOA211O")) str = _T("���������ڷ�");
		else if(strWorkingStatus == _T("BOA212O")) str = _T("�����̵���ڷ�");
		else if(strWorkingStatus == _T("BOR111N")) str = _T("����ΰ��ڷ�");
		else if(strWorkingStatus == _T("BOR112N")) str = _T("����ڷ�");
		else if(strWorkingStatus == _T("BOR113O")) str = _T("�����ι�ȣ�ο��ڷ�");
		else if(strWorkingStatus == _T("BOC111O")) str = _T("�����������ڷ�");
		else if(strWorkingStatus == _T("BOC112O")) str = _T("�з�����ڷ�");
		else if(strWorkingStatus == _T("BOC113N")) str = _T("��������ڷ�");
		else if(strWorkingStatus == _T("BOC114O")) str = _T("�����ΰ����ڷ�");
		else if(strWorkingStatus == _T("BOL111O")) str = _T("�����ΰ��ڷ�");
		else if(strWorkingStatus == _T("BOL112N")) str = _T("�谡�ڷ�");
		else if(strWorkingStatus == _T("BOL113O")) str = _T("���������ڷ�");
		else if(strWorkingStatus == _T("BOL114O")) str = _T("�ļ��ڷ�");
		else if(strWorkingStatus == _T("BOL115O")) str = _T("�����ڷ�");
		else if(strWorkingStatus == _T("BOL211O")) str = _T("���ܴ����ڷ�");
		else if(strWorkingStatus == _T("BOL212O")) str = _T("���������ڷ�");
		//=====================================================
		// 18.09.27 JMJ KOL.RED.2018.008
		else if(strWorkingStatus == _T("BOL411O")) str = _T("����å�η����������ڷ�");
		//=====================================================
		else if(strWorkingStatus == _T("BOL213O")) str = _T("Ư�������ڷ�");
		else if(strWorkingStatus == _T("BOL214O")) str = _T("��޴����ڷ�");
		else if(strWorkingStatus == _T("BOL215O")) str = _T("����ڷ�");
		pDM->SetCellData(_T("UDF_å�ڷ����"), str, nRow);
	}
	return 0;
}

INT CBOVolumeDlg::DisplayActiveGrid(INT nRow, BOOL bSetTopRow /*=FALSE*/)
{
	CESL_Grid *pGrid  = GetActiveGrid();
	if (pGrid == NULL) return - 1;
	CESL_DataMgr *pDM = GetActiveDM();
	INT nRows = pDM->GetRowCount();
	INT idx;
/*
	// å�η��÷��װ� B �ΰ�� ��ũ�� A�� ��� _T("V")�� ǥ��
	CString strBookAppendixFlag;
	
	if (pDM == NULL) return -1;

	// å�η��÷��װ� B �ΰ�� ��ũ�� A�� ��� _T("V")�� ǥ��
	INT nRows = pDM->GetRowCount();
	for (INT idx = 0; idx < nRows; idx++)
	{
		strBookAppendixFlag = pDM->GetCellData(_T("BB_å�η��÷���"), idx);
		if (strBookAppendixFlag == _T("B"))
			strBookAppendixFlag.Empty();
		else if (strBookAppendixFlag == _T("A"))
			strBookAppendixFlag = _T("V");

		pDM->SetCellData(_T("BB_�ηϿ���"), strBookAppendixFlag, idx);

	}
*/
	// å ����
	//SortBookDM(pDM);
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	pGrid->Display();
	
	// QUERY FLAG�� D�� �ڷ�� Height�� 0���� ����
	for (idx = 0; idx < nRows; idx++)
	{
		if (pDM->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D"))
		{
			pGrid->SetRowHeight(idx + 1, 0);			
		}
	}

	pGrid->GetRows();

	if (m_nSelectedGrid == GRID_BOOK || m_nSelectedGrid == GRID_BOOK_HOLD)
	{
		// ��ǥå �ε��� ��������
		// KOL.UDF.022 ��ť���ڵ� ����
		INT nIdx =-1;

		if (m_nSelectedGrid == GRID_BOOK)
			m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
		else if (m_nSelectedGrid == GRID_BOOK_HOLD)
			m_pFormApi->GetRepresentationBook(m_pDM_BookHold, nIdx);

		if  (nIdx >= 0)
			SetGridRowBackColor(pGrid, nIdx);
		//�׸��� ��� ����ȭ ����
		
		if (nRow != nIdx)
			pGrid->SetReverse(nRow);
	}
	else
		pGrid->SetReverse(nRow);
	//�׸��� ��� ����ȭ ����
	if (bSetTopRow)
			pGrid->SetTopRow(nRow + 1);
	return 0;
}

INT CBOVolumeDlg::DisplayGrids()
{
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab == NULL) return -1;

	INT nCurSel = pTab->GetCurSel();
	if (nCurSel == -1) return -1;


	CRect rcStaticGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcStaticGrid);
	ScreenToClient(rcStaticGrid);

	m_pGrid_Volume->ShowWindow(SW_HIDE);
	m_pGrid_VolumeHold->ShowWindow(SW_HIDE);
	m_pGrid_Book->ShowWindow(SW_HIDE);
	m_pGrid_BookHold->ShowWindow(SW_HIDE);

	if (nCurSel == 0)
	{
		if (m_bShowHoldInfo)
		{
			m_pGrid_VolumeHold->ShowWindow(SW_SHOW);
			m_pGrid_VolumeHold->MoveWindow(rcStaticGrid.left, rcStaticGrid.top, rcStaticGrid.Width(),rcStaticGrid.Height() / 2);

			m_pGrid_Volume->ShowWindow(SW_SHOW);
			m_pGrid_Volume->MoveWindow(rcStaticGrid.left, rcStaticGrid.top + (rcStaticGrid.Height() / 2), rcStaticGrid.Width(),rcStaticGrid.Height() / 2);
		}
		else
		{
			m_pGrid_Volume->ShowWindow(SW_SHOW);
			m_pGrid_Volume->MoveWindow(rcStaticGrid.left, rcStaticGrid.top, rcStaticGrid.Width(), rcStaticGrid.Height());
		}

		SetActiveGrid(GRID_VOLUME);
	}
	else if (nCurSel == 1)
	{
		if (m_bShowHoldInfo)
		{
			m_pGrid_BookHold->ShowWindow(SW_SHOW);
			m_pGrid_BookHold->MoveWindow(rcStaticGrid.left, rcStaticGrid.top, rcStaticGrid.Width(),rcStaticGrid.Height() / 2);

			m_pGrid_Book->ShowWindow(SW_SHOW);
			m_pGrid_Book->MoveWindow(rcStaticGrid.left, rcStaticGrid.top + (rcStaticGrid.Height() / 2), rcStaticGrid.Width(),rcStaticGrid.Height() / 2);
		}
		else
		{
			m_pGrid_Book->ShowWindow(SW_SHOW);
			m_pGrid_Book->MoveWindow(rcStaticGrid.left, rcStaticGrid.top, rcStaticGrid.Width(), rcStaticGrid.Height());
		
		}

		SetActiveGrid(GRID_BOOK);
	}

	return 0;
}

INT CBOVolumeDlg::DisplayContorls()
{
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab == NULL) return -1;

	INT nCurSel = pTab->GetCurSel();
	if (nCurSel == -1) return -1;

	CWnd *pWnd = NULL;

	// SHOW, HIDE �������� ��ư
	if (m_bShowHoldInfoButton)
		GetDlgItem(IDC_bVIEW_HOLD_INFO)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_bVIEW_HOLD_INFO)->ShowWindow(SW_HIDE);

	
	// SHOW, HIDE Grid
	DisplayGrids();

	// Ctrl Endable / Disable
	INT nBookOnlyCtrlCount = 0;
	INT nVolumeOnlyCtrlCount = 0;

	INT nArrBookOnlyCtrl[64];
	INT nArrVolumeOnlyCtrl[64];

	// å������ ���̴� Ctrl
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_eCOPY_CODE;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cSHELF_CODE;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cMANAGE_CLASS;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_bSELECT_REP_BOOK;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_btnVOL_FIND_FILE1;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_btnVOL_FIND_FILE2;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_btnVOL_FIND_FILE3;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_edtVOL_EBOOK_EXECUTE_FILE;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cUSE_LIMIT_CODE;

	// �ǿ����� ���̴� Ctrl
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_cMEDIA_CLASS;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eVOL;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eVOLTITLE;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eAUTHOR;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePUBLISH_YEAR;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_edtVOL_SORT_NO;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eEA_ISBN;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eISBN2;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eISBN3;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePAGE_CNT;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePHYSICAL_PROPERTIES;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eSIZE;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eAPPENDIX;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eBOOK_PRICE;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePRICE_OTHRE_INFO;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_bISBN_INSERT;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_bISBN_CHECK;
		
	TCHAR tszClassName[16];
	tszClassName[0] = 0;

	for (INT i = 0; i < nBookOnlyCtrlCount; i++)
	{
		pWnd = GetDlgItem(nArrBookOnlyCtrl[i]);
		if (pWnd == NULL) continue;
		GetClassName(pWnd->GetSafeHwnd(), tszClassName, 100);

		if (nCurSel == 0)
		{
			if (_tcscmp(tszClassName, _T("Edit")) == 0)
				((CEdit*)pWnd)->SetReadOnly(TRUE);
			else
				pWnd->EnableWindow(FALSE);
		}
		else if (nCurSel == 1)
		{
			if (_tcscmp(tszClassName, _T("Edit")) == 0)
				((CEdit*)pWnd)->SetReadOnly(FALSE);
			else
				pWnd->EnableWindow(TRUE);
		}
	}

	for (i = 0; i < nVolumeOnlyCtrlCount; i++)
	{
		pWnd = GetDlgItem(nArrVolumeOnlyCtrl[i]);
		if (pWnd == NULL) continue;

		GetClassName(pWnd->GetSafeHwnd(), tszClassName, 100);
		if (nCurSel == 0)
		{
			if (_tcscmp(tszClassName, _T("Edit")) == 0)
				((CEdit*)pWnd)->SetReadOnly(FALSE);
			else
				pWnd->EnableWindow(TRUE);
		}
		else if (nCurSel == 1)
		{
			if (_tcscmp(tszClassName, _T("Edit")) == 0)
				((CEdit*)pWnd)->SetReadOnly(TRUE);
			else
				pWnd->EnableWindow(FALSE);
		}
	}

	// ȭ��������ư
	pWnd = GetDlgItem(IDC_bCLEARSCREEN);
	if(nCurSel == 0)
	{
		if(pWnd) pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		if(pWnd) pWnd->ShowWindow(SW_HIDE);
	}
	// ����ó����ư
	pWnd = GetDlgItem(IDC_btnAPPLY_SERIES);
	if(nCurSel == 0)
	{
		if(pWnd) pWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		if(pWnd) pWnd->ShowWindow(SW_SHOW);
	}
		
	
	// �����ư
	if (m_nSelectedGrid == GRID_VOLUME_HOLD || m_nSelectedGrid == GRID_BOOK_HOLD)
	{
		GetDlgItem(IDC_bSAVE)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_bSAVE)->ShowWindow(SW_SHOW);
	}

	return 0;
}

INT CBOVolumeDlg::DisplayControlReadOnly()
{
	//��� ��Ʈ���� readonly��Ų��. �� �׸���� ���ܵž���Ѵ�.
	m_pCM->AllControlReadOnly();
	//��/å �׸���� readonly�� Ǯ���ش�
	m_pGrid_Book->EnableWindow(TRUE);
	m_pGrid_Volume->EnableWindow(TRUE);

	INT nButtonCount = 0;
	INT nArrButtonCtrl[16];
	
	nArrButtonCtrl[nButtonCount++] = IDC_bSAVE;
	nArrButtonCtrl[nButtonCount++] = IDC_bDELETE;
	nArrButtonCtrl[nButtonCount++] = IDC_bCLEARSCREEN;
	nArrButtonCtrl[nButtonCount++] = IDC_bINSERT;
	nArrButtonCtrl[nButtonCount++] = IDC_b505TAG;
	nArrButtonCtrl[nButtonCount++] = IDC_bISBN_INSERT;
	nArrButtonCtrl[nButtonCount++] = IDC_bISBN_CHECK;
	nArrButtonCtrl[nButtonCount++] = IDC_btnAPPLY_SERIES;	
//	nArrButtonCtrl[nButtonCount++] = IDC_eAPPLICANT;

	CWnd *pWnd = NULL;

	for (INT i = 0; i < nButtonCount; i++)
	{
		pWnd = GetDlgItem(nArrButtonCtrl[i]);
		if (pWnd == NULL) continue;
		pWnd->EnableWindow(FALSE);
	}

	return 0;
}

INT CBOVolumeDlg::SetGridRowBackColor(CESL_Grid *pGrid, INT nRow)
{
	if (pGrid == NULL) return -1;

	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = pGrid->GetCol();
	INT nCols = pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(red);
			pGrid->SetCellForeColor(black);
		}
	}
	
	pGrid->SetCol(nCol);

	return 0;
}

BOOL CBOVolumeDlg::IsRegCodeModified()
{
	INT nRegCodeCount = -1;
	CString strRegCodeField;
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCodeField);

	CArray<CRegCode, CRegCode> arrRegCode;
	INT ids = m_pFormApi->FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0)
	{
		MessageBox(_T("���� ��ϱ��� �Դϴ�."));
		return TRUE;
	}
	
	nRegCodeCount = arrRegCode.GetSize();
	if (nRegCodeCount < 1) 
	{
		return TRUE;
	}
	
	CString strVol;
	m_pCM->GetControlMgrData(_T("BB_�����"), strVol);

	CESL_DataMgr *pDM = NULL;

	if (m_nSelectedGrid == GRID_VOLUME)
		pDM = m_pDM_Book;
	else if (m_nSelectedGrid == GRID_VOLUME_HOLD)
		pDM = m_pDM_BookHold;
	
	if (m_pFormApi->IsRegCodeModify(&arrRegCode, pDM, strVol))
	{
		AfxMessageBox(_T("��ϱ����� ������ �ɼ� �ֽ��ϴ�. å�������� �������ּ���"));
		Display();
		return TRUE;
	}

	return FALSE;
}

INT CBOVolumeDlg::SortBookDM(CESL_DataMgr *pDM_Book)
{
	if (pDM_Book == NULL) return -1;

	// ���Ϸù�ȣ�� �����Ѵ�.
	//pDM_Book->SORT(_T("BB_���Ϸù�ȣ"));
	Sort_BookDM(pDM_Book);

	CString strVol;

	// ���ĵ� ����� ���ο��� �ηϰ����ؼ� �����Ѵ�.
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!m_pFormApi->IsBook(pDM_Book, idx)) continue;

		// ������� �����´�.
		strVol = pDM_Book->GetCellData(_T("BB_�����"), idx);

		INT nStart = idx;

		// ������� �ƴҶ����� ����
		while (idx < nRowCount)
		{
			if (!m_pFormApi->IsBook(pDM_Book, idx, strVol)) break;

			idx++;
		}

		INT nFinish = idx - 1;

		
		pDM_Book->SORT(_T("BB_å�η��÷���"), nStart, nFinish, FALSE);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBOVolumeDlg message handlers

BOOL CBOVolumeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Initialize Esl Manger	
	INT ids = InitESL_Mgr(_T("K2UPGRADE_����_��"));
	if (ids < 0) 
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	GetWindowRect(m_rcWindow);

	// Initialize Data Mangager
	ids = Init();
	if (ids < 0)
	{
		AfxMessageBox(_T("Init Data Manger Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_nSelectedRow = 0;
	//AllControlDisplay(_T("CMUP_����_��_å����"));
	
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab != NULL)
	{
		pTab->InsertItem(0, _T("�� ����"));
		pTab->InsertItem(1, _T("å ����"));
		if(m_nTabOrder == VOLUME_FIRST)
		{	
			pTab->SetCurFocus(1);
			pTab->SetCurFocus(0);
		}
		else if(m_nTabOrder == BOOK_FIRST)
		{
			pTab->SetCurFocus(0);
			pTab->SetCurFocus(1);
		}
	}

	// Status Bar For Grab HAHA;;
	//m_statusBarCtrl.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP , CRect(0, 0, 0, 0), this, WM_USER + 1000);

	if (m_bShowHoldInfo)
	{
		m_bShowHoldInfo = FALSE;
		OnbVIEWHOLDINFO();
	}

	m_pCM->GetControlMgrData(_T("BB_��ü����_�ڵ�"), m_strPrevMediaCode);
	ShowCodeResourceByMediaCode(m_strPrevMediaCode);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOVolumeDlg::OnDestroy() 
{

	// ������� ���� ��� ���� �߰��Ǿ��� å������ VOL_SORT_NO�� �����Ͽ��� �Ѵ�.
	if(m_pDM_VolumeHold != NULL && m_pDM_BookHold != NULL)
	{
		if(m_pDM_VolumeHold->GetRowCount() > 0 && m_pDM_BookHold->GetRowCount() > 0)
		{
			INT ids = AdjustCurrentBookDMByHoldBook(m_pDM_VolumeHold, m_pDM_BookHold, m_pDM_Book);
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("���� �߰��Ǿ��� å������ ���Ϸù�ȣ�� ������ �� �����ϴ�."));
			}
		}
	}
	CDialog::OnDestroy();
}

VOID CBOVolumeDlg::OnKillfocuseREGCLASSCODE() 
{
	CEdit* pEditClassCode = (CEdit*)GetDlgItem(IDC_eREG_CLASS_CODE);
	if (pEditClassCode == NULL) return;

	// ���� �¸��̸� üũ���� �ʴ´�.
	if (pEditClassCode->GetStyle() & ES_READONLY) return;

	INT nRegCodeCount = -1;
	CString strRegCodeField;
	pEditClassCode->GetWindowText(strRegCodeField);

	CArray<CRegCode, CRegCode> arrRegCode;
	INT ids = m_pFormApi->FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0)
	{
		MessageBox(_T("���� ��ϱ��� �Դϴ�."));
		goto ERR;
	}
	
	nRegCodeCount = arrRegCode.GetSize();
	if (nRegCodeCount < 1) 
	{
		goto ERR;
	}

	//  �ǻ���
	if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
	{
		INT nBookCnt = 0;
		for (INT i = 0; i < nRegCodeCount; i++)
		{
			nBookCnt += arrRegCode.GetAt(i).m_nCount;
		}

		CString strBookCnt;
		strBookCnt.Format(_T("%d"), nBookCnt);

		GetDlgItem(IDC_eBOOK_CNT)->SetWindowText(strBookCnt);
	}
	// å����
	else
	{
		CRegCode regCode;
		regCode = arrRegCode.GetAt(0);

		if (regCode.m_nCount > 1) goto ERR;
		
		m_strRegCodeField = regCode.m_strRegCode;
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegCodeField);
		CString strBookCnt;
		strBookCnt.Format(_T("%d"), 1);
		GetDlgItem(IDC_eBOOK_CNT)->SetWindowText(strBookCnt);
	}

	m_strRegCodeField = strRegCodeField;

ERR:
	pEditClassCode->SetWindowText(m_strRegCodeField);
	return;
}

BEGIN_EVENTSINK_MAP(CBOVolumeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBOVolumeDlg)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_VOLUME, -605 /* MouseDown */, OnMouseDownGridVolume, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_BOOK, -605 /* MouseDown */, OnMouseDownGridBook, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_VOLUME_HOLD, -605 /* MouseDown */, OnMouseDownGridVolumeHold, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_BOOK_HOLD, -605 /* MouseDown */, OnMouseDownGridBookHold, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_VOLUME, 69 /* SelChange */, OnSelChangeGridVolume, VTS_NONE)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_BOOK, 69 /* SelChange */, OnSelChangeGridBook, VTS_NONE)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_VOLUME_HOLD, 69 /* SelChange */, OnSelChangeGridVolumeHold, VTS_NONE)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_BOOK_HOLD, 69 /* SelChange */, OnSelChangeGridBookHold, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBOVolumeDlg::OnbINSERT() 
{
	if (ApplyInsertData() < 0)
		return;

	DisplayContorls();
	// ������  ���� ȭ�鿡 �ѷ��ֱ�
	CESL_DataMgr *pDM = GetActiveDM();
	if (pDM != NULL)
	{
		//m_nSelectedRow = pDM->GetRowCount() - 1;

		DisplayActiveGrid(m_nSelectedRow, TRUE);
	}
	
	Display();
}	


VOID CBOVolumeDlg::OnbSAVE() 
{
	if (m_nSelectedRow < 0) return;
	
	if (ApplySaveData() < 0)
		return;
	// ���õ� �� ���� ȭ�鿡 �ѷ��ֱ�
	DisplayActiveGrid(m_nSelectedRow, TRUE);
	
	PostMessage(WM_COMMAND, IDC_btnNEXT, BN_CLICKED);
}

VOID CBOVolumeDlg::OnbDELETE() 
{
	if (m_nSelectedRow < 0) return;
	
	if (ApplyDeleteData() < 0)
		return;
	
	DisplayContorls();

	// ���� �� ���� ȭ�鿡 �ѷ��ֱ�
	DisplayActiveGrid(m_nSelectedRow, TRUE);
	
	Display();
}

VOID CBOVolumeDlg::OnbAPPENDIXINFO() 
{
/*	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab == NULL) return;

	CBOAppendixDlg dlg(this, m_pDM_Book);
	dlg.SetFormApi(m_pFormApi);
	dlg.SetReadOnly(m_bReadOnly);

	CESL_DataMgr *pDM = GetActiveDM();

	CString strVol;
	if (pDM)
		 strVol = pDM->GetCellData(_T("BB_�����"), m_nSelectedRow);

	INT nCurSel = pTab->GetCurSel();
	if (nCurSel == 0)
	{
		dlg.SetAppendixMode(0, strVol);	
	}
	else if (nCurSel == 1)
	{
		dlg.SetAppendixMode(1, strVol, m_nSelectedRow);
	}
	
	dlg.DoModal();

	Display();

	DisplayActiveGrid(m_nSelectedRow, TRUE);
*/
}

VOID CBOVolumeDlg::OnSelchangeTabVolumeBook(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab == NULL) return;

	INT nCurSel = pTab->GetCurSel();
	if (nCurSel == 0)
	{
		// �� ���� �ٽ� ����
		m_pDM_Volume->FreeData();
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
		m_pDM_Volume->SORT(_T("BB_���Ϸù�ȣ"));
		m_nSelectedRow = 0;
	}
	else if (nCurSel == 1)
	{
		//m_pDM_Book->SORT(_T("BB_���Ϸù�ȣ"));
		Sort_BookDM(m_pDM_Book);
		m_nSelectedRow = GetFirstRowBookDM(m_pDM_Book);
		if(m_nSelectedRow < 0)
			return ;
	}

	DisplayContorls();

	// �����ڷḦ �����ش�.
	INT nSelectedGrid = m_nSelectedGrid;

	if (nSelectedGrid == GRID_VOLUME)
	{
		SetActiveGrid(GRID_VOLUME_HOLD);
		DisplayActiveGrid(0);
	}
	else if (nSelectedGrid == GRID_BOOK)
	{
		SetActiveGrid(GRID_BOOK_HOLD);
		DisplayActiveGrid(0);
	}
	

	// �⺻ ����
	SetActiveGrid(nSelectedGrid);

	Display();

	DisplayActiveGrid(m_nSelectedRow);
	
	*pResult = 0;
}

VOID CBOVolumeDlg::OnbVIEWHOLDINFO() 
{
	if (m_bShowHoldInfo)
	{
		m_bShowHoldInfo = FALSE;
		GetDlgItem(IDC_bVIEW_HOLD_INFO)->SetWindowText(_T("�����ڷẸ��"));
	}
	else
	{
		m_bShowHoldInfo = TRUE;
		GetDlgItem(IDC_bVIEW_HOLD_INFO)->SetWindowText(_T("�����ڷ�����"));
	}

	m_nSelectedRow = 0;
	DisplayContorls();
	Display();

	DisplayActiveGrid(m_nSelectedRow);

}

VOID CBOVolumeDlg::OnMouseDownGridVolume(short Button, short Shift, long x, long y) 
{
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_VOLUME);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;
	
	if (SetActiveGrid(GRID_VOLUME) < 0) return;
	pGrid->SetReverse(m_nSelectedRow);

	Display();
}

VOID CBOVolumeDlg::OnMouseDownGridBook(short Button, short Shift, long x, long y) 
{
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_BOOK);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;

	if (SetActiveGrid(GRID_BOOK) < 0) return;
	
	// ��ǥå �ε��� ��������
	INT nIdx;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
	if (m_nSelectedRow == nIdx)
		SetGridRowBackColor(pGrid, m_nSelectedRow);
	else
		pGrid->SetReverse(m_nSelectedRow);
	
	Display();
}

VOID CBOVolumeDlg::OnMouseDownGridVolumeHold(short Button, short Shift, long x, long y) 
{
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_VOLUME_HOLD);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;

	if (SetActiveGrid(GRID_VOLUME_HOLD) < 0) return;
	pGrid->SetReverse(m_nSelectedRow);

	Display();
}

VOID CBOVolumeDlg::OnMouseDownGridBookHold(short Button, short Shift, long x, long y) 
{
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_BOOK_HOLD);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;
	
	if (SetActiveGrid(GRID_BOOK_HOLD) < 0) return;
	pGrid->SetReverse(m_nSelectedRow);

	Display();
}
/*
VOID CBOVolumeDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL) return;
	
	// StatusBar Ctrl
	/*if (m_statusBarCtrl.GetSafeHwnd() != NULL)
		m_statusBarCtrl.SendMessage(WM_SIZE, 0, 0);
		*/
/*
	CRect rcWnd;
	GetWindowRect(rcWnd);

	if (rcWnd.Width() < m_rcWindow.Width())
	{
		rcWnd.right = rcWnd.left + m_rcWindow.Width();
		if (rcWnd.Height() < m_rcWindow.Height())
			rcWnd.bottom = rcWnd.top + m_rcWindow.Height();	

		MoveWindow(rcWnd);
		return;
	}

	//if (rcWnd.Height() < m_rcWindow.Height())
	if (m_rcWindow.top >= 0 && rcWnd.Height() != m_rcWindow.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcWindow.Height();	
		if (rcWnd.Width() < m_rcWindow.Width())
			rcWnd.right = rcWnd.left + m_rcWindow.Width();			

		MoveWindow(rcWnd);
		return;
	}

	// Grid�� ������¡ --> IDC_STATIC_GRID

	CRect rcClient;
	// Tab
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pWnd)
	{
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		pWnd->SetWindowPos(this, rcClient.left, rcClient.top, cx - 22, cy - 271, SWP_NOZORDER);
	}
	// Static Grid
	pWnd = GetDlgItem(IDC_STATIC_GRID);
	if (pWnd)
	{
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		pWnd->SetWindowPos(this, rcClient.left, rcClient.top, cx - 32, cy - 301, SWP_NOZORDER);
	}

	DisplayGrids();
}
*/
VOID CBOVolumeDlg::OnbCLOSE() 
{
	// �������� �ƴϸ� ��ǥå ���� �ڵ� ����
	if (GetDupIdxKey().IsEmpty())
		m_pFormApi->SetRepresentationBook(m_pDM_Book);

	CDialog::OnOK();
}

VOID CBOVolumeDlg::OnbCLEARSCREEN() 
{
	ClearScreen();
}

VOID CBOVolumeDlg::Onb505TAG() 
{
	// �����Ѵ�.
	OnbSAVE();

	// TODO : �����徾 �� ���̱�
	pParentMgr->SendMessage(WM_USER + 32015);
}

VOID CBOVolumeDlg::OnbISBNCHECK() 
{
	// ���� üũ
	CString strEaISBNFrom;
	CString strEaISBNTo;
	GetDlgItem(IDC_eEA_ISBN)->GetWindowText(strEaISBNFrom);
	if (strEaISBNFrom.IsEmpty()) return;

	INT ids = m_pFormApi->CheckISBN(strEaISBNFrom, strEaISBNTo);
	if (ids < 0) return;
	
	GetDlgItem(IDC_eEA_ISBN)->SetWindowText(strEaISBNTo);	
}

LRESULT CBOVolumeDlg::OnGridDisplay(WPARAM wParam, LPARAM lParam)
{
	CESL_Grid *pGrid  = GetActiveGrid();
	if (pGrid == NULL) return - 1;

	CString strBookAppendixFlag;
	CESL_DataMgr *pDM = GetActiveDM();
	if (pDM == NULL) return -1;

	// QUERY FLAG�� D�� �ڷ�� Height�� 0���� ����
	INT nRows = pDM->GetRowCount();
	for (INT idx = 0; idx < nRows; idx++)
	{
		if (pDM->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D"))
		{
			pGrid->SetRowHeight(idx + 1, 0);			
		}
	}

	// ��ǥå �����ش�.
	INT nIdx;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
	
	if  (nIdx >= 0 && m_nSelectedGrid == GRID_BOOK)
		SetGridRowBackColor(pGrid, nIdx);

	return 0;
}

VOID CBOVolumeDlg::OnbSELECTREPBOOK() 
{
	// TODO: Add your control notification handler code here
	if(m_nSelectedRow < 0) 
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	if(IDYES == ESLAfxMessageBox(_T("������ �ڷḦ ��ǥå���� �����Ͻðڽ��ϱ�?"), MB_YESNO))
	{
		INT nIdx;
		m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
		if(m_nSelectedRow == nIdx)
		{
			ESLAfxMessageBox(_T("�̹� ��ǥå���� ������ �ڷ��Դϴ�."));
			return;
		}
		INT ids = m_pFormApi->SetRepresentationBook(m_pDM_Book, m_nSelectedRow);
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("������ �ڷḦ ��ǥå���� ������ �� �����ϴ�."));
			return;
		}
		m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("U"), nIdx);
		if(m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), m_nSelectedRow).GetLength() == 0)
			m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("U"), m_nSelectedRow);
		DisplayActiveGrid(nIdx);
		DisplayActiveGrid(m_nSelectedRow);
		
	}
}

VOID CBOVolumeDlg::OnbISBNINSERT() 
{
	// TODO: Add your control notification handler code here
	CString strAdditionISBN = m_pDM_Book->GetCellData(_T("BB_�߰�����ISBN"), m_nSelectedRow);
	CString strAdditionAddCode = m_pDM_Book->GetCellData(_T("BB_�߰�����ISBN�ΰ���ȣ"), m_nSelectedRow);
	CString strAdditionMissISBN = m_pDM_Book->GetCellData(_T("BB_�߰����ǿ���ISBN"), m_nSelectedRow);

	CBOISBNAdditionDlg dlg(this);
	dlg.SetAdditionISBN(strAdditionISBN);
	dlg.SetAdditionAddCode(strAdditionAddCode);
	dlg.SetAdditionMissISBN(strAdditionMissISBN);

	if (dlg.DoModal() == IDOK)
	{
		INT rowCnt = m_pDM_Book->GetRowCount();
		CString strVolSortNo = m_pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), m_nSelectedRow);
		for(INT i=0 ; i<rowCnt ; i++)
		{
			if(strVolSortNo == m_pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), i))
			{
				m_pDM_Book->SetCellData(_T("BB_�߰�����ISBN"), dlg.GetAdditionISBN(), i);
				m_pDM_Book->SetCellData(_T("BB_�߰�����ISBN�ΰ���ȣ"), dlg.GetAdditionAddCode(), i);
				m_pDM_Book->SetCellData(_T("BB_�߰����ǿ���ISBN"), dlg.GetAdditionMissISBN(), i);
			}
		}
	}	
}

VOID CBOVolumeDlg::OnKillfocuseCOPYCODE() 
{
	// TODO: Add your control notification handler code here
//	if(GetOpenMode() < 0)
//	{
	CString strCopyCode;
	m_pCM->GetControlMgrData(_T("BB_������ȣ"), strCopyCode); 

	CString strVolSortNo = m_pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), m_nSelectedRow);
	INT ids = SetCopyCode(m_pDM_Book, m_nSelectedRow, strCopyCode);
	if(ids < 0)
		m_pCM->SetControlMgrData(_T("BB_������ȣ"), m_pDM_Book->GetCellData(_T("BB_������ȣ"), m_nSelectedRow));
/*
	if(CheckDuplicateCopyCode(strVolSortNo, strCopyCode, m_nSelectedRow))
	{
		ESLAfxMessageBox(_T("������ ������ȣ�� �ֽ��ϴ�."));
		m_pCM->SetControlMgrData(_T("BB_������ȣ"), m_pDM_Book->GetCellData(_T("BB_������ȣ"), m_nSelectedRow));
	}
	}
*/
}

BOOL CBOVolumeDlg::CheckDuplicateCopyCode(CESL_DataMgr * pDM_BOOK, CString strVolSortNo, CString strCopyCode, INT nSelectedRow)
{
	if(pDM_BOOK == NULL) return TRUE;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	CString strTmpData;
	if(strCopyCode.IsEmpty()) strCopyCode = _T("1");
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i);
		if(i == nSelectedRow) continue;
		if(strTmpData == strVolSortNo)
		{
			strTmpData = pDM_BOOK->GetCellData(_T("BB_������ȣ"), i);
			if(strTmpData.IsEmpty()) strTmpData = _T("1");
			if(strTmpData == strCopyCode)
				return TRUE;
		}
	}
	return FALSE;
}

VOID CBOVolumeDlg::OnbtnAPPLYSERIES() 
{
	// TODO: Add your control notification handler code here
	if (MessageBox(_T("å�� �������� ����ϴ�. ����Ͻðڽ��ϱ�?"), NULL, MB_YESNO | MB_ICONQUESTION) == IDNO) return;

	// ��� å�� 1�� 1å���� �����.
	CString strVol;
	m_pFormApi->ApplyCompleteWorks(m_pDM_Book, strVol);

	MessageBox(_T("å�� �������� ��������ϴ�."));

	Display();
	DisplayActiveGrid(m_nSelectedRow);
}

BOOL CBOVolumeDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYUP && 
		pMsg->hwnd != GetActiveGrid()->GetSafeHwnd() && 
		pMsg->wParam == VK_UP)
	{
		PostMessage(WM_COMMAND, IDC_btnPREV, BN_CLICKED);
		return TRUE;
	}
	if ( pMsg->message == WM_KEYUP && 
		pMsg->hwnd != GetActiveGrid()->GetSafeHwnd() && 
		pMsg->wParam == VK_DOWN)
	{
		PostMessage(WM_COMMAND, IDC_btnNEXT, BN_CLICKED);
		return TRUE;
	}
		
	if ( pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
		return CESL_Mgr::PreTranslateMessage(pMsg);
	}

	if (pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_eBOOK_PRICE)->GetSafeHwnd())
    {
		INT nVirtKey = (INT) pMsg->wParam;
		if ((nVirtKey < '0' || nVirtKey > '9') && nVirtKey != '.' && nVirtKey != 8)
		{
			ESLAfxMessageBox(_T("���ڸ� �Է��Ͻ� �� �ֽ��ϴ�.")); 
            return TRUE;
		}
    }

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBOVolumeDlg::OnSelChangeGridVolume() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_VOLUME);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;
	
	if (SetActiveGrid(GRID_VOLUME) < 0) return;
	pGrid->SetReverse(m_nSelectedRow);

	Display();	
}

VOID CBOVolumeDlg::OnSelChangeGridBook() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_BOOK);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;

	if (SetActiveGrid(GRID_BOOK) < 0) return;
	
	// ��ǥå �ε��� ��������
	INT nIdx;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
	if (m_nSelectedRow == nIdx)
		SetGridRowBackColor(pGrid, m_nSelectedRow);
	else
		pGrid->SetReverse(m_nSelectedRow);
	
	Display();	
}

VOID CBOVolumeDlg::OnSelChangeGridVolumeHold() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_VOLUME_HOLD);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;

	if (SetActiveGrid(GRID_VOLUME_HOLD) < 0) return;
	pGrid->SetReverse(m_nSelectedRow);

	Display();	
}

VOID CBOVolumeDlg::OnSelChangeGridBookHold() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_BOOK_HOLD);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;
	
	if (SetActiveGrid(GRID_BOOK_HOLD) < 0) return;
	pGrid->SetReverse(m_nSelectedRow);

	Display();	
}

INT CBOVolumeDlg::Sort_BookDM(CESL_DataMgr * pDM)
{
	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_��ϱ���_�ڵ�");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_������ȣ");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return 0;
}

INT CBOVolumeDlg::MergeVolume(INT nVolSelectedRow, CString strVol, CString strVolSortNo)
{
	CString strDupValue, strDupAlias, strTmpData, strSrcValue;
	if(!strVol.IsEmpty())
	{
		strDupValue = strVol;
		strDupAlias = _T("BB_�����");
	}
	if(!strVolSortNo.IsEmpty())
	{
		strDupValue = strVolSortNo;
		strDupAlias = _T("BB_���Ϸù�ȣ");
	}
	
	INT nVolumeCnt = m_pDM_Volume->GetRowCount();
	for(INT i=0 ; i<nVolumeCnt ; i++)
	{
		strTmpData = m_pDM_Volume->GetCellData(strDupAlias, i);
		if(strTmpData == strDupValue)
			break;
	}
	strSrcValue = m_pDM_Volume->GetCellData(strDupAlias, nVolSelectedRow);

	INT nBookCnt = m_pDM_Book->GetRowCount();
	for(INT j=0 ; j<nBookCnt ; j++)
	{
		strTmpData = m_pDM_Book->GetCellData(strDupAlias, j);
		if(strTmpData == strSrcValue)
		{
			m_pDM_Book->SetCellData(_T("BB_�����"), m_pDM_Volume->GetCellData(_T("BB_�����"), i), j);
			m_pDM_Book->SetCellData(_T("BB_���Ϸù�ȣ"), m_pDM_Volume->GetCellData(_T("BB_���Ϸù�ȣ"), i), j);
			m_cCopyCodeMgr.SetSrcInfo(
				m_pDM_Volume->GetCellData(_T("BB_�����"), i),
				m_pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), j),
				m_pDM_Book->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), j));

			INT nCopyCode = m_pBOSpeciesApi->GetLastDupCodeNumber(m_pDM_Book, &m_cCopyCodeMgr, j, 0, nBookCnt-1, _T("-1"));
			if(nCopyCode != 0) strTmpData.Format(_T("%d"), nCopyCode+1);
			else strTmpData.Empty();
			m_pDM_Book->SetCellData(_T("BB_������ȣ"), strTmpData, j);
		}
	}

	m_pDM_Volume->FreeData();
	m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
	m_pDM_Volume->SORT(_T("BB_���Ϸù�ȣ"));
	ESLAfxMessageBox(_T("�����Ͽ����ϴ�."));
	m_nSelectedRow = 0;
	return 0;
}

CString CBOVolumeDlg::GetVol(CESL_DataMgr *pDM_Book, CString strModifiedVolSortNo)
{
	INT nRowCnt = pDM_Book->GetRowCount();
	CString strVolSortNo;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strVolSortNo = pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), i);
		if(strModifiedVolSortNo == strVolSortNo)
		{
			return pDM_Book->GetCellData(_T("BB_�����"), i);
		}
	}
	return _T("NOT_FOUND");
}

CString CBOVolumeDlg::GetVolSortNo(CESL_DataMgr *pDM_Book, CString strModifiedVol)
{
	INT nRowCnt = pDM_Book->GetRowCount();
	CString strVol;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strVol = pDM_Book->GetCellData(_T("BB_�����"), i);
		if(strModifiedVol == strVol)
		{
			return pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), i);
		}
	}
	return _T("");
}

INT CBOVolumeDlg::GetFirstRowBookDM(CESL_DataMgr *pDM_Book)
{
	if(pDM_Book == NULL) return -1;
	INT nRowCnt = pDM_Book->GetRowCount();
	if(nRowCnt == 0) return -1;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(pDM_Book->GetCellData(_T("BB_DB_FLAG"), i) == _T("D")) continue;
		if(pDM_Book->GetCellData(_T("BB_å�η��÷���"), i) == _T("A"))continue;
		return i;
	}
	return -1;
}

INT CBOVolumeDlg::SetCopyCode(CESL_DataMgr *pDM)
{
	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];
	strAlias[nCondColumns] = _T("BB_��Ϲ�ȣ");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if(m_cCopyCodeMgr.GetRegCodeCheckYN())
		strAlias[nCondColumns] = _T("BB_��ϱ���_�ڵ�");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if(m_cCopyCodeMgr.GetSeShelfCodeCheckYN())
		strAlias[nCondColumns] = _T("BB_��ġ��ȣ_�ڵ�");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	INT nRowCnt = pDM->GetRowCount();
	CString strRegNo, strRegCode, strSeShelfCode, strVol, strTmpData;
	INT nLastCopyCode;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strRegNo = pDM->GetCellData(_T("BB_��Ϲ�ȣ"), i);
		strRegCode = pDM->GetCellData(_T("BB_��ϱ���_�ڵ�"), i);
		strSeShelfCode = pDM->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), i);
		strVol = pDM->GetCellData(_T("BB_�����"), i);
		m_cCopyCodeMgr.SetSrcInfo(strVol, strRegCode, strSeShelfCode);
		if(GetOpenMode() == 2100 || GetOpenMode() == -2100)
			nLastCopyCode = m_pFormApi->GetLastDupCodeNumber(pDM, i, i, strRegNo);
		else
			nLastCopyCode = m_pFormApi->GetLastDupCodeNumber(pDM, i, i);
		if(nLastCopyCode == 0) strTmpData = _T("");
		else strTmpData.Format(_T("%d"), ++nLastCopyCode);
		pDM->SetCellData(_T("BB_������ȣ"), strTmpData, i);
	}
	return 0;
}

INT CBOVolumeDlg::SetCopyCode(CESL_DataMgr *pDM, INT nCurSel, CString strCopyCode)
{
	INT nRowCnt = pDM->GetRowCount();
	CString strRegNo, strRegCode, strSeShelfCode, strVol, strTmpData;
	strRegCode = pDM->GetCellData(_T("BB_��ϱ���_�ڵ�"), nCurSel);
	strSeShelfCode = pDM->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), nCurSel);
	strVol = pDM->GetCellData(_T("BB_�����"), nCurSel);
	if(strCopyCode.IsEmpty() || strCopyCode == _T("0") || strCopyCode == _T("1")) strCopyCode = _T("");
	m_cCopyCodeMgr.SetSrcInfo(strVol, strRegCode, strSeShelfCode);
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{		
		if(i == nCurSel) continue;
		// �������� �˻��Ѵ�.
		if (!m_cCopyCodeMgr.IsCopy(pDM, i)) continue;
		strTmpData = pDM->GetCellData(_T("BB_������ȣ"), i);
		if(strTmpData == strCopyCode) 
		{
			ESLAfxMessageBox(_T("�ߺ��� ������ȣ�� �ֽ��ϴ�. ������ȣ�� Ȯ���� �ֽʽÿ�."));
			return -1;
		}
	}
	if(strCopyCode == _T("1")) 
		pDM->SetCellData(_T("BB_������ȣ"), _T(""), nCurSel);
	return 0;
}

VOID CBOVolumeDlg::ShowCodeResourceByMediaCode(CString strMediaCode)
{
EFS_BEGIN
	BOOL bShowEBookInfo;
	INT i = 0;
	INT nCodeResourcesHeight = 65;
	CWnd *pWnd = NULL;

	if(strMediaCode == _T("EB")) bShowEBookInfo = TRUE;
	else bShowEBookInfo = FALSE;

	CRect rcWindow;
	GetWindowRect(rcWindow);
	if (bShowEBookInfo && rcWindow == m_rcWindow) return;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	nArrCodeResourceID[nCount++] = IDC_stcVOL_EBOOK_GROUP;
	nArrCodeResourceID[nCount++] = IDC_stcVOL_EBOOK_COVER_FILE;
	nArrCodeResourceID[nCount++] = IDC_edtVOL_EBOOK_COVER_FILE;
	nArrCodeResourceID[nCount++] = IDC_btnVOL_FIND_FILE1;
	nArrCodeResourceID[nCount++] = IDC_btnVOL_FIND_FILE2;
	nArrCodeResourceID[nCount++] = IDC_btnVOL_FIND_FILE3;
	nArrCodeResourceID[nCount++] = IDC_stcVOL_EBOOK_FILE;
	nArrCodeResourceID[nCount++] = IDC_edtVOL_EBOOK_FILE;
	nArrCodeResourceID[nCount++] = IDC_stcVOL_EBOOK_EXECUTE_FILE;
	nArrCodeResourceID[nCount++] = IDC_edtVOL_EBOOK_EXECUTE_FILE;
	nArrCodeResourceID[nCount++] = IDC_btnVOL_EBOOK_VIEW;

	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (bShowEBookInfo)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	// Move Other Resources
	nCount = 0;
	INT nArrOtherResources[256];
	nArrOtherResources[nCount++] = IDC_bCLEARSCREEN;
	nArrOtherResources[nCount++] = IDC_b505TAG;
	nArrOtherResources[nCount++] = IDC_bINSERT;
	nArrOtherResources[nCount++] = IDC_btnNEXT;
	nArrOtherResources[nCount++] = IDC_btnPREV;
	nArrOtherResources[nCount++] = IDC_bSAVE;
	nArrOtherResources[nCount++] = IDC_bDELETE;
	nArrOtherResources[nCount++] = IDC_bCLOSE;
	nArrOtherResources[nCount++] = IDC_btnAPPLY_SERIES;
	nArrOtherResources[nCount++] = IDC_STATIC_GROUP2;
	
	CRect rcResource;
	for (i = 0; i <nCount ; i++)
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->GetWindowRect(rcResource);
			ScreenToClient(rcResource);
			if (bShowEBookInfo)
			{
				rcResource.top += nCodeResourcesHeight;
				rcResource.bottom += nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
			else
			{
				rcResource.top -= nCodeResourcesHeight;
				rcResource.bottom -= nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
		}
	}

	// Resize Window Size
	GetWindowRect(rcResource);
	if (bShowEBookInfo)
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CBOVolumeDlg::OnSelchangecMEDIACLASS() 
{
	// TODO: Add your control notification handler code here
	CString strMediaCode;
	m_pCM->GetControlMgrData(_T("BB_��ü����_�ڵ�"), strMediaCode);
	if(strMediaCode != m_strPrevMediaCode && (strMediaCode == _T("EB") || m_strPrevMediaCode == _T("EB")))
		ShowCodeResourceByMediaCode(strMediaCode);
	m_strPrevMediaCode = strMediaCode;	
}

VOID CBOVolumeDlg::OnbtnVOLFINDFILE1() 
{
	// TODO: Add your control notification handler code here
	m_pFormApi->EBookFileOpen(this, EBOOK_COVER, m_pCM);
	// CM �� �ִ� ������ ����
	m_pFormApi->MakeDMFromCM(m_pCM, m_pDM_Book, m_nSelectedRow);
}

VOID CBOVolumeDlg::OnbtnVOLFINDFILE3() 
{
	// TODO: Add your control notification handler code here
	m_pFormApi->EBookFileOpen(this, EBOOK_EXECUTER, m_pCM);
	// CM �� �ִ� ������ ����
	m_pFormApi->MakeDMFromCM(m_pCM, m_pDM_Book, m_nSelectedRow);
}

VOID CBOVolumeDlg::OnbtnVOLFINDFILE2() 
{
	// TODO: Add your control notification handler code here
	m_pFormApi->EBookFileOpen(this, EBOOK_FILE, m_pCM);
	// CM �� �ִ� ������ ����
	m_pFormApi->MakeDMFromCM(m_pCM, m_pDM_Book, m_nSelectedRow);
}

VOID CBOVolumeDlg::OnbtnVOLEBOOKVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strExeFile, strEBookFile, strBookKey;
	m_pCM->GetControlMgrData(_T("BB_��������"), strEBookFile);
	m_pCM->GetControlMgrData(_T("BB_��������"), strExeFile);
	strBookKey = m_pDM_Book->GetCellData(_T("BB_åKEY"), m_nSelectedRow);
	m_pFormApi->EBookView(strBookKey, strExeFile, strEBookFile);
}

INT CBOVolumeDlg::AdjustCurrentBookDMByHoldBook(CESL_DataMgr *pDM_VolumeHold, CESL_DataMgr * pDM_BookHold, CESL_DataMgr *pDM_Book)
{
	if(pDM_VolumeHold == NULL || pDM_BookHold == NULL ||  pDM_VolumeHold == NULL) return -1;


	m_pFormApi->InitDefaultIgnoreAlias();
	CString strVol, strVolSortNo, strCopyCode;
	INT nRowCnt = pDM_Book->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strVol = pDM_Book->GetCellData(_T("BB_�����"), i);
		strVolSortNo = m_pBOSpeciesApi->FindVolSortNo(pDM_VolumeHold, strVol, -1);
		pDM_Book->SetCellData(_T("BB_���Ϸù�ȣ"), strVolSortNo, i);
		strVolSortNo = pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), i);
		strCopyCode = pDM_Book->GetCellData(_T("BB_������ȣ"), i);
		if(CheckDuplicateCopyCode(pDM_BookHold, strVolSortNo, strCopyCode, -1))
		{
			strVol = pDM_Book->GetCellData(_T("BB_�����"), i);
			INT nCopyCode = m_pFormApi->GetLastDupCodeNumber(pDM_BookHold, strVol);
			strCopyCode.Format(_T("%d"), nCopyCode+1);
			pDM_Book->SetCellData(_T("BB_������ȣ"), strCopyCode, i);			
		}
		
		m_pFormApi->CopyDM(pDM_Book, i, pDM_BookHold, -1);
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(pDM_BookHold, pDM_VolumeHold);
	}
	return 0;
}

INT CBOVolumeDlg::AdjustCurrentBookDMByHoldBook(CESL_DataMgr *pDM_BOOK)
{
	if(!Init())
	{
		ESLAfxMessageBox(_T("�ʱ�ȭ ����"));
		return -1;
	}
	CESL_DataMgr DM_VolumeHold;
	DM_VolumeHold.InitDMPKFromFile(_T("DMUP_����_��_���������"));
	DM_VolumeHold.SetCONNECTION_INFO(pDM_BOOK->CONNECTION_INFO);
	DM_VolumeHold.SetMarcMgr(pParentMgr->m_pInfo->pMarcMgr);	
	CESL_DataMgr DM_BookHold;
	DM_BookHold.InitDMPKFromFile(_T("DMUP_����_��_����å����"));
	DM_BookHold.SetCONNECTION_INFO(pDM_BOOK->CONNECTION_INFO);
	DM_BookHold.SetMarcMgr(pParentMgr->m_pInfo->pMarcMgr);

	if (!GetDupIdxKey().IsEmpty()) 
	{
		// ���� å �����
		CString strOption;
		strOption.Format(_T("B.REC_KEY = E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.BOOK_APPENDIX_FLAG='B'"), GetDupIdxKey());
		m_pFormApi->MakeBookDM(strOption, &DM_BookHold);
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(&DM_BookHold, &DM_VolumeHold);
	}
	else return -1;

	INT ids = AdjustCurrentBookDMByHoldBook(&DM_VolumeHold, &DM_BookHold, pDM_BOOK);
	return ids;
}

INT CBOVolumeDlg::AdjustCurrentBookDM(CESL_DataMgr *pDM_BOOK)
{
	if(pDM_BOOK == NULL) return -1;
	
	m_pFormApi->InitDefaultIgnoreAlias();
	CString strVol, strVolSortNo, strCopyCode;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = _T("BB_�����");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_������ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	pDM_BOOK->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strVol = pDM_BOOK->GetCellData(_T("BB_�����"), i);
		strVolSortNo = m_pBOSpeciesApi->FindVolSortNo(pDM_BOOK, strVol, i);
		pDM_BOOK->SetCellData(_T("BB_���Ϸù�ȣ"), strVolSortNo, i);
		strVolSortNo = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i);
		strCopyCode = pDM_BOOK->GetCellData(_T("BB_������ȣ"), i);
		if(CheckDuplicateCopyCode(pDM_BOOK, strVolSortNo, strCopyCode, -1))
		{
			strVol = pDM_BOOK->GetCellData(_T("BB_�����"), i);
			INT nCopyCode = m_pFormApi->GetLastDupCodeNumber(pDM_BOOK, strVol);
			strCopyCode.Format(_T("%d"), nCopyCode+1);
			pDM_BOOK->SetCellData(_T("BB_������ȣ"), strCopyCode, i);			
		}
	}
	return 0;
}

VOID CBOVolumeDlg::OnbtnPREV() 
{
	// TODO: Add your control notification handler code here
	if(m_nSelectedRow > 0)
		m_nSelectedRow--;

	Display();
	GetActiveGrid()->SetReverse(m_nSelectedRow);
}

VOID CBOVolumeDlg::OnbtnNEXT() 
{
	// TODO: Add your control notification handler code here
	CESL_DataMgr * pDM = GetActiveDM();
	if(m_nSelectedRow < pDM->GetRowCount()-1)
		m_nSelectedRow++;

	Display();
	GetActiveGrid()->SetReverse(m_nSelectedRow);
}

VOID CBOVolumeDlg::ShowPrevNextButton()
{
	GetDlgItem(IDC_btnNEXT)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnPREV)->EnableWindow(TRUE);
	if(m_nSelectedRow == 0) 
		GetDlgItem(IDC_btnPREV)->EnableWindow(FALSE);
	if(m_nSelectedRow == GetActiveDM()->GetRowCount()-1)
		GetDlgItem(IDC_btnNEXT)->EnableWindow(FALSE);
}

VOID CBOVolumeDlg::OnKillfocuseBOOKPRICE() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlEdit * pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BB_����"));
	if(!pEdt) return;
	if(pEdt->GetStyle() == ES_READONLY) return;	

	CString strPrice;
	m_pCM->GetControlMgrData(_T("BB_����"), strPrice);
	CString strTmpData;
	INT nPos = strPrice.Find(_T("."));
	if(nPos > 0) strTmpData = strPrice.Left(nPos);
	else strTmpData = strPrice;
	if(strTmpData.GetLength() > 8)
	{
		ESLAfxMessageBox(_T("���������� �Ҽ��� �����ڷᰡ 8�ڸ� �̻��� �� �����ϴ�."));
		GetDlgItem(IDC_eBOOK_PRICE)->SetFocus();
		return ;
	}
}

INT CBOVolumeDlg::SaveVolume(CESL_ControlMgr * pCM)
{
	// ��ϱ��� �������� üũ
	if (IsRegCodeModified()) return -1;			
	
	// ������ ������ ���� ������� �ִ��� üũ
	CString strVol = m_pDM_Volume->GetCellData(_T("BB_�����"), m_nSelectedRow);
	CString strVolSortNo = m_pDM_Volume->GetCellData(_T("BB_���Ϸù�ȣ"), m_nSelectedRow);
	CString strModifiedVol, strModifiedVolSortNo;
	GetControlData(_T("CMUP_����_��_å����"), _T("BB_�����"), strModifiedVol);
	GetControlData(_T("CMUP_����_��_å����"), _T("BB_���Ϸù�ȣ"), strModifiedVolSortNo);
	//��ȣ�Ϸù�ȣ�� ���� ��� ������ �Ϸù�ȣ�� �����Ͽ� �����Ѵ�.
	if(strModifiedVolSortNo.IsEmpty())
	{
		m_pFormApi->GetLastVolSortNo(m_pDM_Book, strModifiedVolSortNo);
		SetControlData(_T("CMUP_����_��_å����"), _T("BB_���Ϸù�ȣ"), strModifiedVolSortNo);
	}
	
	if (IsDuplicateVolume(strModifiedVol, m_nSelectedRow))//, strModifiedVolSortNo))
	{
		if(IDYES == ESLAfxMessageBox(_T("���� �̸��� ��/������ �����մϴ�. �������� ���� �Ͻðڽ��ϱ�?"), MB_YESNO))
		{
			MergeVolume(m_nSelectedRow, strModifiedVol, _T(""));
			return 0;
		}
		return -1;
	}
	
	if(IsDuplicateVolSortNo(strModifiedVolSortNo, m_nSelectedRow))
	{
		if(IDYES == ESLAfxMessageBox(_T("���� ��ȣ�Ϸù�ȣ�� �����մϴ�. �������� ���� �Ͻðڽ��ϱ�?"), MB_YESNO))
		{
			MergeVolume(m_nSelectedRow, _T(""), strModifiedVolSortNo);
			return 0;
		}
		return -1;
	}
	
	// ������ ������ ����� �����ϱ�
	//ApplyPublishDate();
	
	INT nBookRowCount = m_pDM_Book->GetRowCount();
	
	// �źθ���Ʈ ����� (å���� �������ϰ�..)
	m_pFormApi->InitDefaultIgnoreAlias();
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��ǥå����"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��Ϲ�ȣ"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_EBOOKKEY"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_������ȣ"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_�谡��ġ��ȣ"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));

	// ��ϱ����ڵ� ����
	ApplyRegCode();

	CStringArray ArrIgnore;	ArrIgnore.RemoveAll();
	ArrIgnore.Add(_T("BB_��ġ��ȣ_�ڵ�"));
	ArrIgnore.Add(_T("BB_��������_�ڵ�"));
	ArrIgnore.Add(_T("BB_�̿����ѱ���_�ڵ�"));
	
	// å�� ����
	CString strDBFlag;
	for (INT idx = 0; idx < nBookRowCount; idx++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx, strVol)) continue;
		
		strDBFlag = m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		// CM �� �ִ� ������ ����
		if(strDBFlag == _T("N"))
			m_pFormApi->MakeDMFromCM(pCM, m_pDM_Book, idx, _T(""));
		else
			m_pFormApi->MakeDMFromCM(pCM, m_pDM_Book, idx, _T(""), &ArrIgnore);
		
		// å�η� �÷��� �߰�
		m_pDM_Book->SetCellData(_T("BB_å�η��÷���"), _T("B"), idx);
		
		// DB Flag
		if(strDBFlag == _T("N"))
			m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);
		else
		{
			CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
			m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		}
	}
	
	// ������ �ٽ� ����
	m_pDM_Volume->FreeData();
	m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
	m_pDM_Volume->SORT(_T("BB_���Ϸù�ȣ"));
	ESLAfxMessageBox(_T("�������� �����Ͽ����ϴ�."));

	return 0;
}

INT CBOVolumeDlg::SaveBook(CESL_ControlMgr * pCM)
{
	BOOL bReSetCopyCode = FALSE;
	INT ids;
	CString strRegNo, strRegCode;
	CString strVol, strVolSortNo, strShelfCode;
	CString strModifiedRegCode, strModifiedVol, strModifiedVolSortNo, strModifiedShelfCode, strTmpData;
	
	strRegCode = m_pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), m_nSelectedRow);
	strRegNo = m_pDM_Book->GetCellData(_T("BB_��Ϲ�ȣ"), m_nSelectedRow);
	strVol = m_pDM_Book->GetCellData(_T("BB_�����"), m_nSelectedRow);
	strVolSortNo = m_pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), m_nSelectedRow);
	strShelfCode = m_pDM_Book->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), m_nSelectedRow);	
	GetControlData(_T("CMUP_����_��_å����"), _T("BB_�����"), strModifiedVol);
	GetControlData(_T("CMUP_����_��_å����"), _T("BB_���Ϸù�ȣ"), strModifiedVolSortNo);
	GetControlData(_T("CMUP_����_��_å����"), _T("BB_��ġ��ȣ_�ڵ�"), strModifiedShelfCode);
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strModifiedRegCode);
	
	m_cCopyCodeMgr.SetSrcInfo(strModifiedVol, strModifiedRegCode, strModifiedShelfCode);
	
	// ��Ϲ�ȣ�� �ο��� �ڷ�� ��ϱ����� �����ϸ� �ȵ�
	if(!strRegNo.IsEmpty() && strRegNo.Left(2) != strModifiedRegCode) 
	{
		ESLAfxMessageBox(_T("��Ϲ�ȣ�� �ο��� �ڷ�� ��ϱ����� ������ �� �����ϴ�."));
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegNo.Left(2));
		return 0;
	}
	
	// ���� ���Ϸù�ȣ�� ������ ���Ϸù�ȣ�� �ٸ���� - ������� �����Ѵ�.
	// ���� ������� ������ ������� �ٸ���� - ���Ϸù�ȣ�� �����Ѵ�.
	if(strVolSortNo != strModifiedVolSortNo)
	{
		strTmpData = GetVol(m_pDM_Book, strModifiedVolSortNo);
		if(strTmpData != _T("NOT_FOUND")) strModifiedVol = strTmpData;
		SetControlData(_T("CMUP_����_��_å����"), _T("BB_�����"), strModifiedVol);
	}
	else if(strVol != strModifiedVol)
	{
		strModifiedVolSortNo = GetVolSortNo(m_pDM_Book, strModifiedVol);
		if(strModifiedVolSortNo.IsEmpty())
			m_pFormApi->GetLastVolSortNo(m_pDM_Book, strModifiedVolSortNo);
		SetControlData(_T("CMUP_����_��_å����"), _T("BB_���Ϸù�ȣ"), strModifiedVolSortNo);
		bReSetCopyCode = TRUE;
	}
	if(strShelfCode != strModifiedShelfCode && m_cCopyCodeMgr.GetSeShelfCodeCheckYN() && m_pFormApi->GetVolumeCount(m_pDM_Book) > 1)
	{
		m_pDM_Book->SetCellData(_T("BB_��ġ��ȣ_�ڵ�"), strModifiedShelfCode, m_nSelectedRow);
		bReSetCopyCode = TRUE;
	}
	if(strRegCode != strModifiedRegCode && m_cCopyCodeMgr.GetRegCodeCheckYN() && m_pDM_Book->GetRowCount() > 1)
	{
		m_pDM_Book->SetCellData(_T("BB_��ϱ���_�ڵ�"), strModifiedRegCode, m_nSelectedRow);
		bReSetCopyCode = TRUE;
	}
	if(bReSetCopyCode)
	{
		if(IDYES == ESLAfxMessageBox(_T("������ȣ �ο����� ������ ����Ǿ����ϴ�.\n������ȣ�� �缳�� �Ͻðڽ��ϱ�?"), MB_YESNO))
		{
			INT nCopyCode = m_pBOSpeciesApi->GetLastDupCodeNumber(m_pDM_Book, &m_cCopyCodeMgr, m_nSelectedRow, 0, m_nSelectedRow, _T("-1"));
			if(nCopyCode != 0) strTmpData.Format(_T("%d"), nCopyCode+1);
			else strTmpData.Empty();
			m_pCM->SetControlMgrData(_T("BB_������ȣ"), strTmpData);
			m_pDM_Book->SetCellData(_T("BB_������ȣ"), strTmpData, m_nSelectedRow);
		}
	}
	m_pFormApi->InitDefaultIgnoreAlias();
	
	// CM �� �ִ� ������ ����
	ids = m_pFormApi->MakeDMFromCM(pCM, m_pDM_Book, m_nSelectedRow);
	if (ids < 0) return -1;
	
	// å�η� �÷��� �߰�
	m_pDM_Book->SetCellData(_T("BB_å�η��÷���"), _T("B"), m_nSelectedRow);
	
	// DB Flag ����
	CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), m_nSelectedRow));
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, m_nSelectedRow);
	
	// ��ϱ����ڵ� ����
	ApplyRegCode();
	ESLAfxMessageBox(_T("å������ �����Ͽ����ϴ�."));
	return 0;
}

INT CBOVolumeDlg::InsertVolume()
{
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��������"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_�̹�������"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��������"));
	// �߰��� ������ ���� ������� �ִ��� üũ
	CString strVol, strVolSortNo;
	GetControlData(_T("CMUP_����_��_å����"), _T("BB_�����"), strVol);
	GetControlData(_T("CMUP_����_��_å����"), _T("BB_���Ϸù�ȣ"), strVolSortNo);
	//��ȣ�Ϸù�ȣ�� ���� ��� ������ �Ϸù�ȣ�� �����Ͽ� �����Ѵ�.
	if (IsDuplicateVolume(strVol, -1))//
	{
		AfxMessageBox(_T("���� �̸��� ��/������ �����մϴ�."));
		return -1;
	}
	
	if(strVolSortNo.IsEmpty() || IsDuplicateVolSortNo(strVolSortNo, -1))
	{
		m_pFormApi->GetLastVolSortNo(m_pDM_Book, strVolSortNo);
		SetControlData(_T("CMUP_����_��_å����"), _T("BB_���Ϸù�ȣ"), strVolSortNo);
	}
	
	// ������ ������ ����� �����ϱ�
	//ApplyPublishDate();
	INT ids = -1;
	// ǥ�� å �����
	ids = MakeStandardBookFromCM(NULL);
	if (ids < 0) return -1;
	// ��ϱ��� �ʵ� ����
	ids = ApplyRegCode();
	if (ids < 0) return -1;
	m_pDM_Volume->FreeData();
	m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
	m_pDM_Volume->SORT(_T("BB_���Ϸù�ȣ"));
	
	ESLAfxMessageBox(_T("�������� �߰��Ͽ����ϴ�."));
	return 0;
}

INT CBOVolumeDlg::InsertBook()
{
	CString strRegCode, strSeShelfCode, strVol, strTmpData, strVolSortNo;
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCode);
	if (!m_pFormApi->IsValidRegCode(strRegCode))
	{
		AfxMessageBox(_T("��ϱ��а��� �ùٸ��� �ʽ��ϴ�."));
		return -1;
	}
	CStringArray arrIgnorelist;
	CString strWorkingStatus;
	arrIgnorelist.Add(_T("BB_��ϱ���_�ڵ�"));
	arrIgnorelist.Add(_T("BB_��������_�ڵ�"));
	arrIgnorelist.Add(_T("BB_��ġ��ȣ_�ڵ�"));
	arrIgnorelist.Add(_T("BB_������ȣ"));
	arrIgnorelist.Add(_T("BB_��������"));
	arrIgnorelist.Add(_T("BB_DB_FLAG"));
	arrIgnorelist.Add(_T("BB_åKEY"));
	arrIgnorelist.Add(_T("BB_��ǥå����"));
	m_pFormApi->CopyDM(m_pDM_Book, m_nSelectedRow, m_pDM_Book, -1, _T(""), &arrIgnorelist);
	m_nSelectedRow = m_pDM_Book->GetRowCount() - 1;
	//�ڷ���� ���� �谡 ������ �ڷῡ ���Ͽ� �ұ� �Է��� ��� �谡����ڷᰡ �ƴѰ�� ������ �谡�ڷ� ���¸� ������ �Ѵ�.
	strWorkingStatus = m_pDM_Book->GetCellData(_T("BB_�ڷ����"), m_nSelectedRow);
	if(strWorkingStatus.Left(3) == _T("BOL") && strWorkingStatus != _T("BOL111O"))
	{
		strWorkingStatus = _T("BOL112N");
		m_pDM_Book->SetCellData(_T("BB_�ڷ����"), strWorkingStatus, m_nSelectedRow);			
	}
	else if(strWorkingStatus.Left(3) == _T("BOR"))
	{
		strWorkingStatus = _T("BOR111N");
		m_pDM_Book->SetCellData(_T("BB_�ڷ����"), strWorkingStatus, m_nSelectedRow);			
	}
	m_pDM_Book->SetCellData(_T("BB_��ϱ���_�ڵ�"), strRegCode, m_nSelectedRow);
	m_pCM->GetControlMgrData(_T("BB_��������_�ڵ�"), strTmpData);
	m_pDM_Book->SetCellData(_T("BB_��������_�ڵ�"), strTmpData, m_nSelectedRow);
	m_pCM->GetControlMgrData(_T("BB_��ġ��ȣ_�ڵ�"), strSeShelfCode);
	m_pDM_Book->SetCellData(_T("BB_��ġ��ȣ_�ڵ�"), strSeShelfCode, m_nSelectedRow);
	m_pCM->GetControlMgrData(_T("BB_������ȣ"), strTmpData);	
	m_pCM->GetControlMgrData(_T("BB_�����"), strVol);	
	BOOL bReSetCopyCode = FALSE;
	if(strVol != m_pDM_Book->GetCellData(_T("BB_�����"), m_nSelectedRow))
	{
		m_pFormApi->GetLastVolSortNo(m_pDM_Book, strVolSortNo);
		m_pCM->SetControlMgrData(_T("BB_���Ϸù�ȣ"), strVolSortNo);
		m_pDM_Book->SetCellData(_T("BB_�����"), strVol, m_nSelectedRow);
		m_pDM_Book->SetCellData(_T("BB_���Ϸù�ȣ"), strVolSortNo, m_nSelectedRow);
		bReSetCopyCode = TRUE;
	}
	strVolSortNo = m_pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), m_nSelectedRow);

	m_cCopyCodeMgr.SetSrcInfo(strVol, strRegCode, strSeShelfCode);
	INT nCopyCode = m_pBOSpeciesApi->GetLastDupCodeNumber(m_pDM_Book, &m_cCopyCodeMgr, m_nSelectedRow, 0, m_nSelectedRow, _T("-1"));
	if(nCopyCode != 0) strTmpData.Format(_T("%d"), nCopyCode+1);
	else strTmpData.Empty();
	m_pCM->SetControlMgrData(_T("BB_������ȣ"), strTmpData);
	m_pDM_Book->SetCellData(_T("BB_������ȣ"), strTmpData, m_nSelectedRow);
/*
	if(CheckDuplicateCopyCode(m_pDM_Book, strVolSortNo, strTmpData, m_nSelectedRow))
	{
		strTmpData = m_pDM_Book->GetCellData(_T("BB_�����"), m_nSelectedRow);
		INT nCopyCode = m_pFormApi->GetLastDupCodeNumber(m_pDM_Book, strTmpData);
		strTmpData.Format(_T("%d"), nCopyCode+1);
		m_pCM->SetControlMgrData(_T("BB_������ȣ"), strTmpData);
		m_pDM_Book->SetCellData(_T("BB_������ȣ"), strTmpData, m_nSelectedRow);
	}
*/	
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), m_nSelectedRow);
	m_pDM_Book->SetCellData(_T("BB_��ǥå����"), _T("N"), m_nSelectedRow);
	ESLAfxMessageBox(_T("å������ �߰��Ͽ����ϴ�."));
	return 0;
}

INT CBOVolumeDlg::DeleteVolume()
{
	INT nVolCount = m_pDM_Volume->GetRowCount();
	if (nVolCount == 1) 
	{
		MessageBox(_T("������ ���̹Ƿ� ������ �� �����ϴ�."));
		return -1;
	}
	
	CString strVol;
	GetControlData(_T("CMUP_����_��_å����"), _T("BB_�����"), strVol);
	if (strVol.IsEmpty()) return -1;
	
	// ���� ������ ������ üũ�Ѵ�.
	if (m_pFormApi->CheckDeleteVolume(m_pDM_Book, strVol) < 0)
	{
		MessageBox(_T("�����Ҽ� ���� ������ å�� �����մϴ�. ������ �� �����ϴ�."));
		return -1;
	}
	
	CString strMsg;
	strMsg.Format(_T("��/���ڰ� [%s]�� ���� �����Ͻðڽ��ϱ�?"), strVol);
	if (MessageBox(strMsg, NULL, MB_ICONQUESTION | MB_YESNO) == IDNO) return -1;
	
	// å DM ���� �ǿ� �����ϴ� å�� �����.
	m_pFormApi->DeleteVolume(m_pDM_Book, strVol);
	
	// �� DM ���� ���� Row�� �����.
	m_pDM_Volume->DeleteRow(m_nSelectedRow--);
	return 0;
}

INT CBOVolumeDlg::DeleteBook()
{
	//INT nBookCount = m_pDM_Book->GetRowCount();
	INT nBookCount = m_pFormApi->GetBookCount(m_pDM_Book);
	if (nBookCount == 1)
	{
		AfxMessageBox(_T("������ å�̹Ƿ� ������ �� �����ϴ�."));
		return -1;
	}
	
	// ���� ������ å���� üũ�Ѵ�.
	if (m_pFormApi->CheckDeleteBook(m_pDM_Book, m_nSelectedRow) < 0)
	{
		MessageBox(_T("�����Ҽ� ���� ������ å�� �����մϴ�. ������ �� �����ϴ�."));
		return -1;
	}
	
	CString strVolTitle;
	GetControlData(_T("CMUP_����_��_å����"), _T("BB_������ηϸ�"), strVolTitle);
	CString strMsg;
	strMsg.Format(_T("�Ǽ����� [%s]�� å�� �����Ͻðڽ��ϱ�?"), strVolTitle);
	if (MessageBox(strMsg, NULL, MB_ICONQUESTION | MB_YESNO) == IDNO) return -1;
	
	// å DM ���� ������ �ο츦 �����.
	m_pFormApi->DeleteBookDMRow(m_pDM_Book, m_nSelectedRow--);
	return 0;
}
