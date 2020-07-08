// BOVolumeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOVolumeDlg.h"

#include "BOAppendixDlg.h"

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
	m_bShowHoldInfoButton = TRUE;


	m_pGrid_Volume = NULL;
	m_pGrid_VolumeHold = NULL;
	m_pGrid_Book = NULL;
	m_pGrid_BookHold = NULL;

	m_bReadOnly = FALSE;
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
	ON_BN_CLICKED(IDC_bAPPENDIX_INFO, OnbAPPENDIXINFO)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VOLUME_BOOK, OnSelchangeTabVolumeBook)
	ON_BN_CLICKED(IDC_bVIEW_HOLD_INFO, OnbVIEWHOLDINFO)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bCLEARSCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_b505TAG, Onb505TAG)
	ON_EN_KILLFOCUS(IDC_eREG_NO, OnKillfocuseREGNO)
	ON_BN_CLICKED(IDC_bISBN_CHECK, OnbISBNCHECK)
	ON_EN_KILLFOCUS(IDC_eREG_NO_SHOW, OnKillfocuseREGNOSHOW)
	ON_BN_CLICKED(IDC_bSELECT_REP_BOOK, OnbSELECTREPBOOK)
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
	INT ids;

	// DM ���� 
	if (m_pDM_Book == NULL)
	{
		m_pDM_Book = FindDM(_T("DM_����_��_å����"));
	}

	m_pDM_Volume = FindDM(_T("DM_����_��_������"));
	
	m_pDM_BookHold = FindDM(_T("DM_����_��_����å����"));
	m_pDM_VolumeHold = FindDM(_T("DM_����_��_���������"));

	if (m_pDM_Book == NULL ||
		m_pDM_Volume == NULL ||
		m_pDM_BookHold == NULL ||
		m_pDM_VolumeHold == NULL
		) 
		return -1;

	// CM ����
	m_pCM = FindCM(_T("CM_����_��_å����"));
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


	// �� DM ����
	
	// �������� ������ ������ �����Ѵ�.
	if (!GetDupIdxKey().IsEmpty()) 
	{
		// ���� å �����
		CString strOption;
		strOption.Format(_T("SPECIES_KEY = %s"), GetDupIdxKey());
		ids = m_pDM_BookHold->RefreshDataManager(strOption);
		{
			AfxMessageBox(_T("RefreshDataManager �� Ȯ�����ּ���"));
			return -1;
		}

		// ���� �� �����
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_BookHold, m_pDM_VolumeHold);

		m_bShowHoldInfo = TRUE;
	}

	// Open Mode �� ���� ...
	if (GetOpenMode() == 1100)
	{
		m_pFormApi->SetAppMode(MODE_INSERT);

		// ��Ϲ�ȣ�� �Է� ���Ѵ�.
		GetDlgItem(IDC_eREG_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_eREG_NO_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
	}
	else if (GetOpenMode() == -1100)
	{
		m_pFormApi->SetAppMode(MODE_UPDATE);

		// ��Ϲ�ȣ�� �Է� ���Ѵ�.
		GetDlgItem(IDC_eREG_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_eREG_NO_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
	}
	
	// - ����-��-�Է� : 2100
	else if (GetOpenMode() == 2100)
	{
		m_bShowHoldInfoButton = FALSE;
		m_pFormApi->SetAppMode(MODE_INSERT);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_b505TAG)->ShowWindow(SW_SHOW);

		// å �׸����� DM ������ ������ �ٲ۴�.
		CString strLinkDM;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DM_����_��_å����"), strLinkDM);
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
		// å �׸����� DM ������ ������ �ٲ۴�.
		CString strLinkDM;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DM_����_��_å����"), strLinkDM);
		m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
		m_pGrid_Book->InitColumnLinkDMAndAlias();
		m_pGrid_Book->InitColumnInfo();

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
		//��Ϲ�ȣ �Է��� �� ���� ��ǥå���� ��ư�� ������ �ʰ��Ѵ�.
		GetDlgItem(IDC_eREG_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_eREG_NO_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
		m_bShowHoldInfoButton = FALSE;
		if (m_strSpeciesKey.IsEmpty()) return -1;
		// DM �����Ѵ�.
		CString strOption;
		if (m_strPurchaseKey.IsEmpty())
			strOption.Format(_T("SPECIES_KEY = %s AND REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY REPRESENT_BOOK_YN DESC"), m_strSpeciesKey);
		else
			strOption.Format(_T("SPECIES_KEY = %s AND ACCU_KEY = %s AND REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY REPRESENT_BOOK_YN DESC"), m_strSpeciesKey, m_strPurchaseKey);
		ids = m_pDM_Book->RefreshDataManager(strOption);
		if (ids < 0)
			return -1;

		// å �׸����� DM ������ ������ �ٲ۴�.
		CString strLinkDM = m_pDM_Book->DB_MGR_ALIAS;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DM_����_��_å����"), strLinkDM);
		m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
		m_pGrid_Book->InitColumnLinkDMAndAlias();
		m_pGrid_Book->InitColumnInfo();

		m_bReadOnly = TRUE;
	}

	return 0;
}

INT CBOVolumeDlg::Display()
{
	if (m_nSelectedRow < 0) return -1;

	CESL_ControlMgr *pCM = FindCM(_T("CM_����_��_å����"));
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

	ShowRegCodeField();
	ShowRegNo();
	OnKillfocuseREGCLASSCODE();

	if (m_bReadOnly)
		DisplayControlReadOnly();

    return 0;
}

INT CBOVolumeDlg::ClearScreen()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_����_��_å����"));
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
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	if (strRegNo.GetLength() < 3) 
// 		strRegNo.Empty();
// 	else
// 		strRegNo = strRegNo.Mid(2);
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;

	if (strRegNo.GetLength() <= nRegCodeLength) 
		strRegNo.Empty();
	else
		strRegNo = strRegNo.Mid(nRegCodeLength);
	//===================================================================

	GetDlgItem(IDC_eREG_NO_SHOW)->SetWindowText(strRegNo);

	return 0;
}

BOOL CBOVolumeDlg::IsDuplicateVolume(CString strVol)
{
	//if (strVol.IsEmpty()) return FALSE;
	
	CString strOriginVol;
	INT nRowCount = m_pDM_Volume->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		m_pDM_Volume->GetCellData(_T("BB_�����"), idx, strOriginVol);
		if (strVol == strOriginVol) return TRUE;
	}

	return FALSE;
}

INT CBOVolumeDlg::MakeStandardBookFromCM(CStringArray *pArrAbortList /*= NULL*/)
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_����_��_å����"));
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

	// å�����϶� ������ȣ �߰�
	if (m_nSelectedGrid == GRID_BOOK || m_nSelectedGrid == GRID_BOOK_HOLD)
	{
		CString strVol = m_pDM_Book->GetCellData(_T("BB_�����"), nIdx);
		INT nDupNumber = m_pFormApi->GetLastDupCodeNumber(m_pDM_Book, strVol);

		CString strDupCode;
		strDupCode.Format(_T("c%d"), ++nDupNumber);
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
		GetControlData(_T("CM_����_��_å����"), _T("BB_�����"), strVol);

		m_pFormApi->MakeBookFromRegCode(&arrRegCode, m_pDM_Book, strVol);
	}
	// å����
	else
	{
		CString strRegCode;
		GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCode);

		// �´� ��ϱ������� üũ
		if (!m_pFormApi->IsValidRegCode(strRegCode))
		{
			AfxMessageBox(_T("��ϱ��а��� �ùٸ��� �ʽ��ϴ�."));
			return -1;
		}
		m_pDM_Book->SetCellData(_T("BB_��ϱ���_�ڵ�"), strRegCode, m_nSelectedRow);
		
		// å�η� �÷��� �߰�
		m_pDM_Book->SetCellData(_T("BB_å�η��÷���"), _T("B"), m_nSelectedRow);
	}

	return 0;
}

INT CBOVolumeDlg::ApplySaveData()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_����_��_å����"));
	if (pCM == NULL) return -1;

	INT ids;
	// ����-��-����-�Է� || // ����-��-����-���� || // - ����-��-���� : -2100
	if (GetOpenMode() == 1100 || GetOpenMode() == -1100 || GetOpenMode() == 2100 || GetOpenMode() == -2100)
	{
		// �� ���¿��� ������ ������
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			// ��ϱ��� �������� üũ
			if (IsRegCodeModified()) return -1;
			
			
			// ������ ������ ���� ������� �ִ��� üũ
			CString strVol = m_pDM_Volume->GetCellData(_T("BB_�����"), m_nSelectedRow);
			CString strModifiedVol;
			GetControlData(_T("CM_����_��_å����"), _T("BB_�����"), strModifiedVol);

			if (strVol != strModifiedVol && IsDuplicateVolume(strModifiedVol))
			{
				AfxMessageBox(_T("���� �̸��� ��/������ �����մϴ�."));
				return -1;
			}

			INT nBookRowCount = m_pDM_Book->GetRowCount();

			// �źθ���Ʈ ����� (å���� �������ϰ�..)
			m_pFormApi->InitDefaultIgnoreAlias();
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��ǥå����"));
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��Ϲ�ȣ"));
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_������ȣ"));
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_�谡��ġ��ȣ"));
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��ġ��ȣ_�ڵ�"));
					
			// å�� ����
			for (INT idx = 0; idx < nBookRowCount; idx++)
			{
				// å�� �ƴϸ� �Ѿ��.
				if (!m_pFormApi->IsBook(m_pDM_Book, idx, strVol)) continue;
				
				// CM �� �ִ� ������ ����
				m_pFormApi->MakeDMFromCM(pCM, m_pDM_Book, idx, _T(""));

				// å�η� �÷��� �߰�
				m_pDM_Book->SetCellData(_T("BB_å�η��÷���"), _T("B"), idx);

				// DB Flag
				CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
				m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
			}

			// ��ϱ����ڵ� ����
			ApplyRegCode();

			// ������ �ٽ� ����
			m_pDM_Volume->FreeData();
			m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
		}
		// å ���¿��� ������ ������
		else
		{
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
		}
	}

	return 0;
}

INT CBOVolumeDlg::ApplyInsertData()
{
	// ����-��-����-�Է� || // ����-��-����-���� || // - ����-��-���� : -2100
	if (GetOpenMode() == 1100 || GetOpenMode() == -1100 || GetOpenMode() == 2100 || GetOpenMode() == -2100)
	{
		// �źθ���Ʈ ����� (å���� �������ϰ�..)
		m_pFormApi->InitDefaultIgnoreAlias();
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��ǥå����"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��Ϲ�ȣ"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_������ȣ"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));

		// �� ����
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			// �߰��� ������ ���� ������� �ִ��� üũ
			CString strVol;
			GetControlData(_T("CM_����_��_å����"), _T("BB_�����"), strVol);

			if (IsDuplicateVolume(strVol))
			{
				AfxMessageBox(_T("���� �̸��� ��/������ �����մϴ�."));
				return -1;
			}		
		}
		// å����
		else
		{
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_�谡��ġ��ȣ"));
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��ġ��ȣ_�ڵ�"));
		}

		INT ids = -1;

		// ǥ�� å �����
		ids = MakeStandardBookFromCM(NULL);
		if (ids < 0) return -1;

		// å�����϶� SelectedRow ������ å �η� �÷��� �߰�
		if (m_nSelectedGrid == GRID_BOOK || m_nSelectedGrid == GRID_BOOK_HOLD)
		{
			m_nSelectedRow = m_pDM_Book->GetRowCount() - 1;
			// å�η� �÷��� �߰�
			m_pDM_Book->SetCellData(_T("BB_å�η��÷���"), _T("B"), m_nSelectedRow);
		}

		// ��ϱ��� �ʵ� ����
		ids = ApplyRegCode();
		if (ids < 0) return -1;

		// �� ���¿��� �߰��� ������
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			// ������ �ٽ� ����
			m_pDM_Volume->FreeData();
			m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
		}
	}

	return 0;
}

INT CBOVolumeDlg::ApplyDeleteData()
{
	// ����-��-����-�Է� || // ����-��-����-���� || // - ����-��-���� : -2100
	if (GetOpenMode() == 1100 || GetOpenMode() == -1100 || GetOpenMode() == 2100 ||  GetOpenMode() == -2100)
	{
		// �� ����
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			INT nVolCount = m_pDM_Volume->GetRowCount();
			if (nVolCount == 1) 
			{
				MessageBox(_T("������ ���̹Ƿ� ������ �� �����ϴ�."));
				return -1;
			}

			CString strVol;
			GetControlData(_T("CM_����_��_å����"), _T("BB_�����"), strVol);
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
		}
		// å����
		else
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
			GetControlData(_T("CM_����_��_å����"), _T("BB_������ηϸ�"), strVolTitle);
			CString strMsg;
			strMsg.Format(_T("�Ǽ����� [%s]�� å�� �����Ͻðڽ��ϱ�?"), strVolTitle);
			if (MessageBox(strMsg, NULL, MB_ICONQUESTION | MB_YESNO) == IDNO) return -1;
			
			// å DM ���� ������ �ο츦 �����.
			m_pFormApi->DeleteBookDMRow(m_pDM_Book, m_nSelectedRow--);
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
	nArrButtonCtrl[nButtonCount++] = IDC_bAPPENDIX_INFO;

	CWnd *pWnd = NULL;

	CString strBookAppFlag;
	if (m_nSelectedGrid == GRID_BOOK)
		strBookAppFlag = m_pDM_Book->GetCellData(_T("BB_å�η��÷���"), m_nSelectedRow);

	// �η��̸� �߰�, ��ϱ��� �ڵ带 Disable
	if (strBookAppFlag == _T("A"))
	{
		((CEdit*)GetDlgItem(IDC_eREG_CLASS_CODE))->SetReadOnly(TRUE);
		GetDlgItem(IDC_bINSERT)->EnableWindow(FALSE);
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_eREG_CLASS_CODE))->SetReadOnly(FALSE);
		GetDlgItem(IDC_bINSERT)->EnableWindow(TRUE);
	}

	

	for (INT i = 0; i < nButtonCount; i++)
	{
		pWnd = GetDlgItem(nArrButtonCtrl[i]);
		if (pWnd == NULL) continue;

		if (m_bReadOnly && nArrButtonCtrl[i] != IDC_bAPPENDIX_INFO)
		{			
			pWnd->EnableWindow(FALSE);
		}
		else if (m_nSelectedGrid == GRID_VOLUME)
			pWnd->EnableWindow(TRUE);
		else if (m_nSelectedGrid == GRID_BOOK)
		{
			if (strBookAppFlag == _T("A"))
				pWnd->EnableWindow(FALSE);
			else
				pWnd->EnableWindow(TRUE);
		}			
		else if (m_nSelectedGrid == GRID_VOLUME_HOLD || m_nSelectedGrid == GRID_BOOK_HOLD)
			pWnd->EnableWindow(FALSE);
		else
			return -1;
	}

	return 0;
}

INT CBOVolumeDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	strGridAlias.TrimLeft();		strGridAlias.TrimRight();
	CESL_DataMgr * pDM = GetActiveDM();
	if(!pDM) return 0;
	if(strGridAlias == _T("GRID_VOLUME") || strGridAlias == _T("GRID_VOLUME_HOLD")) 
		return 0;

	if(strDMFieldAlias == _T("UDF_å�ڷ����"))
	{
		CString strWorkingStatus = pDM->GetCellData(_T("BB_�ڷ����"), nRow);
		if(strWorkingStatus == _T("BOA111N")) str = _T("���Դ���ڷ�");
		else if(strWorkingStatus == _T("BOA112N")) str = _T("�ֹ������ڷ�");
		else if(strWorkingStatus == _T("BOA113N")) str = _T("�˼�����ڷ�");
		else if(strWorkingStatus == _T("BOA114O")) str = _T("�̳�ǰ�ڷ�");
		else if(strWorkingStatus == _T("BOA131N")) str = _T("��������ڷ�");
		else if(strWorkingStatus == _T("BOA132N")) str = _T("�����δ��ڷ�");
		else if(strWorkingStatus == _T("BOA141N")) str = _T("�˼��Ϸ��ڷ�");
		else if(strWorkingStatus == _T("BOA142N")) str = _T("�����Ϸ��ڷ�");
		else if(strWorkingStatus == _T("BOC111N")) str = _T("����ΰ��ڷ�");
		else if(strWorkingStatus == _T("BOC112O")) str = _T("�з������");
		else if(strWorkingStatus == _T("BOC113N")) str = _T("������ڷ�");
		else if(strWorkingStatus == _T("BOL111O")) str = _T("��ġ����ڷ�");
		else if(strWorkingStatus == _T("BOL112N")) str = _T("��ġ�ڷ�");
		else if(strWorkingStatus == _T("BOL221O")) str = _T("���ܴ����ڷ�");
		else if(strWorkingStatus == _T("BOL222O")) str = _T("���������ڷ�");
		else if(strWorkingStatus == _T("BOL223O")) str = _T("Ư�������ڷ�");
		else if(strWorkingStatus == _T("BOL224O")) str = _T("��޴����ڷ�");
		else if(strWorkingStatus == _T("BOL225O")) str = _T("����ڷ�");
		else if(strWorkingStatus == _T("BOL251N")) str = _T("���������ڷ�");
		else if(strWorkingStatus == _T("BOL252N")) str = _T("�н��ڷ�");
		else if(strWorkingStatus == _T("BOL253N")) str = _T("�����ڷ�");
		else if(strWorkingStatus == _T("BOL217O")) str = _T("�������ڷ�");
		
		pDM->SetCellData(_T("UDF_å�ڷ����"), str, nRow);
	}
	return 0;
}

INT CBOVolumeDlg::DisplayActiveGrid(INT nRow, BOOL bSetTopRow /*=FALSE*/)
{
	CESL_Grid *pGrid  = GetActiveGrid();
	if (pGrid == NULL) return - 1;

	// å�η��÷��װ� B �ΰ�� ��ũ�� A�� ��� _T("V")�� ǥ��
	CString strBookAppendixFlag;
	CESL_DataMgr *pDM = GetActiveDM();
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

	// å ����
	SortBookDM(pDM);
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

		if (nRow != nIdx)
			pGrid->SetReverse(nRow);
	}
	else
		pGrid->SetReverse(nRow);

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
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_eREG_NO;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_eREG_NO_SHOW;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_eCOPY_CODE;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cSHELF_LOC_CODE;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cSHELF_CODE;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cUSE_LIMIT_CLASS;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cMANAGE_CLASS;
	//��ǥå ������ư
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_bSELECT_REP_BOOK;

	// �ǿ����� ���̴� Ctrl
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_cMEDIA_CLASS;
	//nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_cUSE_LIMIT_CLASS;
	//nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_cMANAGE_CLASS;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eVOL;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eVOLTITLE;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eAUTHOR;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePUBLISH_YEAR;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eEA_ISBN;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eISBN2;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eISBN3;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePAGE_CNT;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePHYSICAL_PROPERTIES;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eSIZE;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eAPPENDIX;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eBOOK_PRICE;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePRICE_OTHRE_INFO;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eAPPLICANT;
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
	//nArrButtonCtrl[nButtonCount++] = IDC_bAPPENDIX_INFO;
	nArrButtonCtrl[nButtonCount++] = IDC_bCLEARSCREEN;
	nArrButtonCtrl[nButtonCount++] = IDC_bINSERT;
	nArrButtonCtrl[nButtonCount++] = IDC_b505TAG;
	nArrButtonCtrl[nButtonCount++] = IDC_bISBN_INSERT;
	nArrButtonCtrl[nButtonCount++] = IDC_bISBN_CHECK;
	nArrButtonCtrl[nButtonCount++] = IDC_eAPPLICANT;

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

	// ������� �����Ѵ�.
	pDM_Book->SORT(_T("BB_�����"));

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
	INT ids = InitESL_Mgr(_T("SM_����_��"));
	if (ids < 0) 
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// Initialize Data Mangager
	ids = Init();
	if (ids < 0)
	{
		AfxMessageBox(_T("Init Data Manger Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_nSelectedRow = 0;
	//AllControlDisplay(_T("CM_����_��_å����"));
	
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab != NULL)
	{
		pTab->InsertItem(0, _T("�� ����"));
		pTab->InsertItem(1, _T("å ����"));

		pTab->SetCurFocus(1);
		pTab->SetCurFocus(0);
	}

	// Status Bar For Grab HAHA;;
	//m_statusBarCtrl.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP , CRect(0, 0, 0, 0), this, WM_USER + 1000);

	GetWindowRect(m_rcWindow);

	if (m_bShowHoldInfo)
	{
		m_bShowHoldInfo = FALSE;
		OnbVIEWHOLDINFO();
	}

	//Display();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOVolumeDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
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

VOID CBOVolumeDlg::OnKillfocuseREGNOSHOW() 
{
	// �Ǻ������� ����
	if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD) return;

	CEdit* pEditRegNoShow = (CEdit*)GetDlgItem(IDC_eREG_NO_SHOW);
	if (pEditRegNoShow == NULL) return;

	// ���� �¸��̸� üũ���� �ʴ´�.
	if (pEditRegNoShow->GetStyle() & ES_READONLY) return;
	
	CString strQuery;
	CString strResult;

	INT idx = -1;
	INT nRowCount = -1;
	CString strRegCode;
	CString strRegNo;
	CString strRegNoField;
	CString strSelectRowRecKey;

	// ��Ϻ�ȣ �����´�.
	pEditRegNoShow->GetWindowText(strRegNo);
	if (strRegNo.IsEmpty()) return;

	INT nRegNo = _ttoi(strRegNo);

	// ��ϱ��а����´�.
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCode);
	if (strRegCode.IsEmpty()) return;
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	strRegNo.Format(_T("%010d"), nRegNo);
// 	pEditRegNoShow->SetWindowText(strRegNo);
// 	strRegNoField = strRegCode + strRegNo;

	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	strRegNoField = pParentMgr->m_pInfo->MakeRegNo(strRegCode,strRegNo);
	strRegNo = strRegNoField.Mid(nRegCodeLength);
	
	pEditRegNoShow->SetWindowText(strRegNo);
	//===================================================================
	// ��������ȣ���� Ŭ��� �޽��������ش�.
	CTime t(CTime::GetCurrentTime());
	
	strQuery.Format(_T("SELECT COUNT(*) FROM CO_LAST_NUMBER_TBL ")
					_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '%s' AND MAKE_YEAR = '%d' AND '%d' > LAST_NUMBER")
					, strRegCode, t.GetYear(), nRegNo);
	m_pDM_Book->GetOneValueQuery(strQuery, strResult);
	if  (_ttoi(strResult) > 0)
		goto OVER_NO;


	// ���� BOOK_DM���� �ߺ��� üũ�Ѵ�.
	nRowCount = m_pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		// ���� DM Row�̸� �Ѿ��.
		if (idx == m_nSelectedRow) continue;

		// å�� �ƴϸ� �Ѿ��.
		// if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;

		// ���Ѵ�.
		if (m_pDM_Book->GetCellData(_T("BB_��Ϲ�ȣ"), idx) == strRegNoField) goto FIND_DUP;
	}

	// DB���� ��Ϲ�ȣ �ߺ��� üũ�Ѵ�.
	strQuery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE REG_NO = '%s'"), strRegNoField);
	m_pDM_Book->GetOneValueQuery(strQuery, strResult);
	
	strSelectRowRecKey = m_pDM_Book->GetCellData(_T("BB_åKEY"), m_nSelectedRow);
	if(!strResult.IsEmpty() && strSelectRowRecKey != strResult) goto FIND_DUP;
	
	/*if (_ttoi(strResult) > 0)
		goto FIND_DUP;
	*/
	// ���� ��Ϲ�ȣ ��Ʈ�ѿ� ��Ϲ�ȣ�� �ִ´�.
	GetDlgItem(IDC_eREG_NO)->SetWindowText(strRegNoField);
	
	return;

FIND_DUP:
	MessageBox(_T("��Ϲ�ȣ�� �ߺ��˴ϴ�. �ٽ� �Է��� �ּ���"));
	goto SET_REG_NO_SHOW;

OVER_NO:
	MessageBox(_T("��Ϲ�ȣ�� ������ ��ȣ���� Ů�ϴ�. �ٽ� �Է��� �ּ���"));
	goto SET_REG_NO_SHOW;
	
SET_REG_NO_SHOW:
	strRegNo = m_pDM_Book->GetCellData(_T("BB_��Ϲ�ȣ"), m_nSelectedRow);
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	if(!strRegNo.IsEmpty()) strRegNo = strRegNo.Right(10);
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	
	if(!strRegNo.IsEmpty()) strRegNo = strRegNo.Right(12 - nRegCodeLength);
	//===================================================================
	GetDlgItem(IDC_eREG_NO_SHOW)->SetWindowText(strRegNo);
	pEditRegNoShow->SetFocus();			
	return;

}


BEGIN_EVENTSINK_MAP(CBOVolumeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBOVolumeDlg)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_VOLUME, -605 /* MouseDown */, OnMouseDownGridVolume, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_BOOK, -605 /* MouseDown */, OnMouseDownGridBook, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_VOLUME_HOLD, -605 /* MouseDown */, OnMouseDownGridVolumeHold, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_BOOK_HOLD, -605 /* MouseDown */, OnMouseDownGridBookHold, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
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
	
	Display();
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
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
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
}

VOID CBOVolumeDlg::OnSelchangeTabVolumeBook(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab == NULL) return;

	m_nSelectedRow = 0;

	DisplayContorls();

	INT nCurSel = pTab->GetCurSel();

	if (nCurSel == 0)
	{
		// �� ���� �ٽ� ����
		m_pDM_Volume->FreeData();
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
	}
	else if (nCurSel == 1)
	{
	
	}

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

VOID CBOVolumeDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL) return;
	
	// StatusBar Ctrl
	/*if (m_statusBarCtrl.GetSafeHwnd() != NULL)
		m_statusBarCtrl.SendMessage(WM_SIZE, 0, 0);
		*/

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

VOID CBOVolumeDlg::OnbCLOSE() 
{
	// �������� �ƴϸ� ��ǥå ���� �ڵ� ����
	if (GetDupIdxKey().IsEmpty())
		m_pFormApi->SetRepresentationBook(m_pDM_Book);

	// ������ȣ �ڵ� �ϰ� ����
	m_pFormApi->AutoApplyDupCode(m_pDM_Book);

	// �ηϵ�Ϲ�ȣ �ڵ� �ϰ� ����
	m_pFormApi->AutoApplyAppendixRegNo(m_pDM_Book);
	
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

VOID CBOVolumeDlg::OnKillfocuseREGNO() 
{
	// 2003.03.06 ���̻� ������� ���� : ��Ϲ�ȣ�� ���ڷ� ������ OnKillfocusREGNOSHOW�� ����ٶ�
	
	/*
	// �Ǻ������� ����
	if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD) return;

	CEdit* pEditRegClassNo = (CEdit*)GetDlgItem(IDC_eREG_NO);
	if (pEditRegClassNo == NULL) return;
	
	// ���� �¸��̸� üũ���� �ʴ´�.
	if (pEditRegClassNo->GetStyle() & ES_READONLY) return;

	CString strRegNoField;
	pEditRegClassNo->GetWindowText(strRegNoField);
	if (strRegNoField.IsEmpty()) return;


	// ��ϱ����� üũ�Ѵ�.
	CString strRegCode = strRegNoField.Left(2);
	if (!m_pFormApi->IsValidRegCode(strRegCode))
	{
		MessageBox(_T("������� �ʴ� ��ϱ����Դϴ�."));
		pEditRegClassNo->SetFocus();			
	}

	if (strRegNoField.GetLength() < 3) return;
	
	//��Ϲ�ȣ üũ
	
	INT idx = -1;
	INT nRowCount = -1;
	CString strQuery;
	CString strResult;

	CString strRegNo = strRegNoField.Mid(2);
	INT nRegNo = _ttoi(strRegNo);
	
	strRegNoField.Format(_T("%s%010d"), strRegCode,	nRegNo);
	pEditRegClassNo->SetWindowText(strRegNoField);

	// ��������ȣ���� Ŭ��� �޽��������ش�.
	CTime t(CTime::GetCurrentTime());
	
	strQuery.Format(_T("SELECT COUNT(*) FROM CO_LAST_NUMBER_TBL ")
					_T("WHERE DIVISION_VALUE = '%s' AND MAKE_YEAR = '%d' AND '%d' > LAST_NUMBER")
					, strRegCode, t.GetYear(), nRegNo);
	m_pDM_Book->GetOneValueQuery(strQuery, strResult);
	if  (_ttoi(strResult) > 0)
		goto OVER_NO;


	// ���� BOOK_DM���� �ߺ��� üũ�Ѵ�.
	nRowCount = m_pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		// ���� DM Row�̸� �Ѿ��.
		if (idx == m_nSelectedRow) continue;

		// å�� �ƴϸ� �Ѿ��.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;

		// ���Ѵ�.
		if (m_pDM_Book->GetCellData(_T("BB_��Ϲ�ȣ"), idx) == strRegNoField) goto FIND_DUP;
	}

	// DB���� ��Ϲ�ȣ �ߺ��� üũ�Ѵ�.
	strQuery.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE REG_NO = '%s'"), strRegNoField);
	m_pDM_Book->GetOneValueQuery(strQuery, strResult);

	if (_ttoi(strResult) > 0)
		goto FIND_DUP;	
	
	return;

FIND_DUP:
	MessageBox(_T("��Ϲ�ȣ�� �ߺ��˴ϴ�. �ٽ� �Է��� �ּ���"));
	pEditRegClassNo->SetFocus();			
	return;

OVER_NO:
	MessageBox(_T("��Ϲ�ȣ�� ������ ��ȣ���� Ů�ϴ�. �ٽ� �Է��� �ּ���"));
	pEditRegClassNo->SetFocus();			
	return;
	*/
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
