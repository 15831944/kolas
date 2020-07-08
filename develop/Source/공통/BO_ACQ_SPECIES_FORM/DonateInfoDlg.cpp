 // DonateInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DonateInfoDlg.h"
#include "../COMMON_DONATOR_MANAGER/DonatorNameGetter_Search.h"
#include "../COMMON_DONATOR_MANAGER/DonatorNameGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonateInfoDlg dialog


CDonateInfoDlg::CDonateInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CInfoDlg(pParent, CDonateInfoDlg::IDD)
{
	//{{AFX_DATA_INIT(CDonateInfoDlg)
	m_nRegYN = -1;
	m_strPriceChar = _T("");
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pCmbPriceChar = NULL;
	m_strDonatorInfoKey = _T("");
	m_strBeforeDonatorName = _T("");
	m_strBeforeDonatorInfoKey = _T("");
}

CDonateInfoDlg::~CDonateInfoDlg()
{
	
}

VOID CDonateInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonateInfoDlg)
	DDX_Radio(pDX, IDC_radREG_YES, m_nRegYN);
	DDX_CBString(pDX, IDC_cmbBSFORM_PRICE_CHARACTER, m_strPriceChar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonateInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CDonateInfoDlg)
	ON_BN_CLICKED(IDC_btnBSFORM_SEARCH_DONATOR, OnbtnBSFORMSEARCHDONATOR)
	ON_BN_CLICKED(IDC_radREG_YES, OnradREGYES)
	ON_BN_CLICKED(IDC_radREG_NO, OnradREGNO)
	ON_CBN_SELCHANGE(IDC_cmbBSFORM_PRICE_TYPE, OnSelchangecmbBSFORMPRICETYPE)
	ON_CBN_KILLFOCUS(IDC_cmbBSFORM_PRICE_CHARACTER, OnKillfocuscmbBSFORMPRICECHARACTER)
	ON_CBN_SELCHANGE(IDC_cmbBSFORM_PRICE_CHARACTER, OnSelchangecmbBSFORMPRICECHARACTER)
	ON_BN_CLICKED(IDC_btnBSFORM_BEFORE_DONATOR, OnbtnBSFORMBEFOREDONATOR)
	ON_MESSAGE(SET_TAB_ORDER, OnSetTabOrder)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonateInfoDlg message handlers

BOOL CDonateInfoDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CDonateInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("K2UPGRADE_����_��_��������")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	
	//CM����
	m_pCM = FindCM(_T("CM_����_��_��������"));	
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_����_��_��������]�� ������ �� �����ϴ�."));
		return FALSE;
	}

	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("�������� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

BOOL CDonateInfoDlg::Init()
{
	CESL_ControlComboBox* pCombo = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_�ֹ�����Ÿ��"));

	m_pCmbPriceChar = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_���ݼ���"));
	if( m_pCmbPriceChar == NULL )	return FALSE;

	if( pCombo )  pCombo->SetCurSel(0);

	if( GetOpType() != SINSERT && 
		(GetSpeciesType() == BO_DONATE || GetSpeciesType() == BO_NONDONATE) )
	{
		GetDlgItem(IDC_radREG_YES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_radREG_NO )->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_radREG_YES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_radREG_NO )->ShowWindow(SW_HIDE);
	}

	if( GetOpType() == SINSERT )
	{
		/************************************************************
		 ************************************************************/
		//20080424 ADD PJW : �Է��Ͽ� ���� 
		CString strInputDate;
		CString strDate; 
		CTime t = CTime::GetCurrentTime();
		strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
		GetDM()->SetCellData(_T("BP_�Է�����"), strDate, 0);
		m_pCM->SetControlMgrData(_T("BP_�Է�����")		, strInputDate);
		m_pCM->AllControlDisplay(0);
		/************************************************************
		 ************************************************************/
	}

	switch( GetSpeciesType() )
	{
	case BO_DONATE:
	{
		GetDlgItem(IDC_stcBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		SetWorkingStatus(_T("BOA211O"));
		m_nRegYN = 0;
	}
		break;

	case BO_NONDONATE:
	{
		SetWorkingStatus(_T("BOA212O"));
		m_nRegYN = 1;
	}
		break;

	case BO_REGOBJ:
	{
		GetDlgItem(IDC_stcBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		SetWorkingStatus(_T("BOA211O"));
		m_nRegYN = 0;
	}
		break;
	default:
	{
		GetDlgItem(IDC_stcBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
	}
		break;
	}

	UpdateData(FALSE);

	return TRUE;
}

// ������ �˻� ��ư
VOID CDonateInfoDlg::OnbtnBSFORMSEARCHDONATOR() 
{
	CDonatorNameGetter_Search searchDlg(this);
	
	searchDlg.SetOpenMode(-1000);

	CString strPrice;
	CString strCharacter;
	CString strOrderPriceInfo;
	CString strCause;
	CString strName;
	CString strInputDate;
	CString strCurrency;

	m_pCM->GetControlMgrData(_T("BP_����")			, strPrice);
	m_pCM->GetControlMgrData(_T("BP_���ݼ���")		, strCharacter);
	m_pCM->GetControlMgrData(_T("BP_����������")	, strOrderPriceInfo);
	m_pCM->GetControlMgrData(_T("BP_�����δ����")	, strCause);
	m_pCM->GetControlMgrData(_T("BP_�����ڸ�")		, strName);
	m_pCM->GetControlMgrData(_T("BP_�Է�����")		, strInputDate);
	m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�")		, strCurrency);

	GetDM()->SetCellData(_T("BP_����")			, strPrice			, 0);
	GetDM()->SetCellData(_T("BP_���ݼ���")		, strCharacter		, 0);
	GetDM()->SetCellData(_T("BP_����������")	, strOrderPriceInfo	, 0);
	GetDM()->SetCellData(_T("BP_�����δ����")	, strCause			, 0);
	GetDM()->SetCellData(_T("BP_�����ڸ�")		, strName			, 0);
	GetDM()->SetCellData(_T("BP_�Է�����")		, strInputDate		, 0);
	GetDM()->SetCellData(_T("BP_ȭ�󱸺�")		, strCurrency		, 0);

	if( IDOK == searchDlg.DoModal() )
	{
		CDonatorNameGetterDlg searchResult(this);
		if( IDOK == searchResult.DoModal() )
		{
			GetDM()->SetCellData(_T("BP_�����ڸ�"), searchResult.GetDonatorName(), 0);
			GetDM()->SetCellData(_T("BP_����������KEY"), searchResult.GetDonatorKey(), 0);
			//�˻��Ǿ����� ������ ����Ű ����� ����
			m_strDonatorInfoKey = searchResult.GetDonatorKey();
			//20070903 ADD PJW : �Է��Ͽ� ���� 
			if( strInputDate == _T("") )
			{
				CString strDate;
				CTime t = CTime::GetCurrentTime();
				strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
				GetDM()->SetCellData(_T("BP_�Է�����"), strDate, 0);
			}
		}
	}	

	// 2007.04.12 ADD REM BY PDJ
	// ������ ��ư�� ������ �ݱ� ������ DM�� �ִ� ������ �ʱ�ȭ �ȴ�.
	// �׷��Ƿ� Allcontrol �ϸ� �ȵ�.
	m_pCM->AllControlDisplay(0);
}

INT CDonateInfoDlg::ValidateData()
{
	INT nPos;
	CString strPrice, strTmpData, strDonator, strRecKey, strQuery;

	m_pCM->GetControlMgrData(_T("BP_����"), strPrice);
	
	nPos = strPrice.Find(_T("."));
	if( nPos > 0 )	strTmpData = strPrice.Left(nPos);
	else strTmpData = strPrice;

	if( strTmpData.GetLength() > 8 )
	{
		ESLAfxMessageBox(_T("���������� �Ҽ����� �ڷᰡ 8�ڸ� �̻��� �� �����ϴ�."));
		return -1;
	}

	if( nPos > 0 )
	{
		strTmpData = strPrice.Mid(nPos+1);
		if( strTmpData.GetLength() > 2 ) 
		{
			ESLAfxMessageBox(_T("���������� �Ҽ������� ��°�ڸ������� �Է� �����մϴ�."));
			return -1;
		}	
	}

	m_pCM->GetControlMgrData(_T("BP_�����ڸ�"), strDonator);
	// 2005-11-14 Ȳ���� �߰�
	// ������ ���� ���̺��� �������̸����� �˻��Ѵ�. ������ �̸��� �ߺ����� ��� �� �����Ƿ� ..
	// �߰����� ó���� �ʿ��ϴ�. 
	if( strDonator.IsEmpty() == FALSE )
	{
		CESL_DataMgr pDM;
		pDM.SetCONNECTION_INFO(GetDM()->CONNECTION_INFO);
		strQuery.Format(_T("SELECT REC_KEY FROM CO_DONATOR_TBL WHERE DONATOR = '%s'"), strDonator);
		
		pDM.RestructDataManager(strQuery);
		if(pDM.GetRowCount() == 1)
		{
			m_strDonatorInfoKey = pDM.GetCellData(0,0);
		}
		else if( pDM.GetRowCount() > 1 )
		{

			if(m_strDonatorInfoKey.IsEmpty() == TRUE )
			{
				if(GetOpType() == SINSERT)
				{
					AfxMessageBox(_T("�������̸��� �ߺ��˴ϴ�. \n �˻��Ͽ� ������ �ֽʽÿ�"));
					return -1;
				}
				else
				{
					GetDM()->GetCellData(_T("BP_����������KEY"),0,m_strDonatorInfoKey);
				}
			}
			else
			{
				CString strDonInfoKey;
				for(INT i = 0; i < pDM.GetRowCount() ; i++)
				{
					if( m_strDonatorInfoKey == pDM.GetCellData(i,0))
						break;
					if( i == pDM.GetRowCount()-1 )
						m_strDonatorInfoKey = _T("");
				}
			}
		}
		else
		{
			//m_strDonatorInfoKey == _T("");
			AfxMessageBox(_T("�������� �ʴ� ������ ������ ������ �� �����ϴ�."));
			return -1;
		}
	}
	else 
	{
		m_strDonatorInfoKey = _T("");
	}
	/*if( strDonator.IsEmpty() == FALSE )
	{
		strQuery.Format(_T("SELECT REC_KEY FROM CO_DONATOR_TBL WHERE DONATOR = '%s'"), strDonator);
		GetDM()->GetOneValueQuery(strQuery, m_strDonatorInfoKey);
	}
	else 
	{
		m_strDonatorInfoKey = _T("");
	}*/

	if( GetDonatorInfo() == TRUE && m_strDonatorInfoKey.IsEmpty() == TRUE )
	{
		ESLAfxMessageBox(_T("���������� �Է��� �ʼ� �����Դϴ�."));
		return -1;
	}
	else
	{
		m_strBeforeDonatorName = strDonator;
		m_strBeforeDonatorInfoKey = m_strDonatorInfoKey;

		//====================================================================
		//2009.05.20 ADD BY LKS : �������ڵ� �����ֵ��� ����
		m_pCM->GetControlMgrData(_T("BP_�Է�����"), m_strBeforeDonateDate, 0);
		//====================================================================
	}

	return 0;
}

INT CDonateInfoDlg::ApplyData()
{
	INT nBookCount, i, ids;
	CString strVolPriceSum;
	
	GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);

	nBookCount = GetBookDM()->GetRowCount();
	for ( i=0; i<nBookCount; i++ )
	{
		// ���ݼ��� ����
		GetBookDM()->SetCellData(_T("BB_���ݼ���"), m_strPriceChar, i);
	}

	GetDM()->SetCellData(_T("BP_����������KEY"), m_strDonatorInfoKey, 0);
	
	if( GetFormApi()->GetVolumeCount(GetBookDM()) == 1 )
	{
		nBookCount = GetBookDM()->GetRowCount();
		for( i=0; i<nBookCount; i++)
		{
			GetBookDM()->SetCellData(_T("BB_����������"), GetDM()->GetCellData(_T("BP_����������"), 0), i);
		}
	}

	if( GetApplyPriceInfo() == TRUE )
	{
		ids = ApplyCalculation();
		if( ids < 0 )
		{
			AfxMessageBox(_T("�������� ���� ����"));
			return -1;
		}
		else if( ids > 0 )
		{
			AfxMessageBox(_T("�Ǻ������� ������ �����ִ� ���� �ֽ��ϴ�."));
			return -1;
		}
	}
	//�������� ����
	GetVolPriceSum(strVolPriceSum);
	GetDM()->SetCellData(_T("BP_����"), strVolPriceSum, 0);
	
	return 0;
}

INT CDonateInfoDlg::ApplyCalculation()
{
	INT ids, nPrice;
	DOUBLE dPrice;
	CString strPriceType, strTemp, str950$b, strTotalPrice;
	CStringArray arrIgnoreList;
	

	ids = GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);
	if( ids < 0 )	return -1;	
	
	ids = m_pCM->GetControlMgrData(_T("BP_�ֹ�����Ÿ��"), strPriceType);
	if( ids < 0 )	return -1;
	
	CESL_ControlComboBox* pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_�ֹ�����Ÿ��"));

	ids = m_pCM->GetControlMgrData(_T("BP_����"), strTemp);
	if( ids < 0 )	return -1;
	
	nPrice = _ttoi(strTemp);
	
	// 2004.09.14
	//dPrice = (DOUBLE)atof(strTemp.GetBuffer(0));

	TCHAR* pEndPtr = NULL;
	dPrice = _tcstod(strTemp.GetBuffer(0), &pEndPtr);

	// ���� == �������� ��
	if( strPriceType == _T("������") && 
		( GetApplyVolPriceFromSpeciesPrice() == FALSE || GetFormApi()->GetVolumeCount(GetBookDM()) == 1) )
	{
		// ������ 1/n���� ������ ������ �ǿ� �����Ų��.
		// 2004.09.14  nPrice->dPrice
		// 2004.12.06
		m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strTemp);
		ids = GetFormApi()->ApplySpeciesPriceToVolPrice(GetBookDM(), dPrice, strTotalPrice);
		if( ids < 0 )	return -1;

		SetApplyVolPriceFromSpeciesPrice(TRUE);
	}
	// ���� == ���� ���� ������ ���Ѱ�
	else if (strPriceType == _T("�ǰ���"))
	{
		if( GetFormApi()->GetVolumeCount(GetBookDM()) > 1 )
		{
			if( IDNO == ESLAfxMessageBox(_T("�����Ͻ� �ǰ����� �� �ǿ� �����Ͻðڽ��ϱ�?"), MB_YESNO) )
			{
				pCmb->SetCurSel(0);
				Display(0);
				GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);
				return 0;
			}
		}
		// 2004.09.14 nPrice->dPrice
		// 2004.12.06
		m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strTemp);
		ids = GetFormApi()->ApplyVolPrice(GetBookDM(), dPrice, strTotalPrice, str950$b);
		if( ids < 0 )	return -1;
		//m_pCM->SetControlMgrData(_T("BP_����"), str950$b);
		m_pCM->SetControlMgrData(_T("BP_����"), GetBookDM()->GetCellData(_T("BB_����"), 0));

		GetDM()->SetCellData(_T("BP_����"), str950$b, 0);
		//if(pCmb) pCmb->SetCurSel(0);
		Display(0);
	}
	// ���ǰ�ϰ�� �ǰ����� ��� 0���� �����Ѵ�.
	// 2005-12-15 ������ �߰�	
	else if( m_strPriceChar == _T("���ǰ") )
	{
		m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strTemp);
		ids = GetFormApi()->ApplyVolPrice(GetBookDM(), dPrice, strTotalPrice, str950$b);
		if( ids < 0 )	return -1;

		m_pCM->SetControlMgrData(_T("BP_����"), GetBookDM()->GetCellData(_T("BB_����"), 0));

		GetDM()->SetCellData(_T("BP_����"), str950$b, 0);	
		
		Display(0);
	}

	
	arrIgnoreList.Add(_T("BP_����"));
	ids = GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0, _T(""), &arrIgnoreList);
	
		return 0;
}

INT CDonateInfoDlg::Display(INT idx)
{
	INT nIdx = 0;
	CESL_ControlEdit* pEdt		 = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_����"));
	CESL_ControlComboBox* pCombo = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_�ֹ�����Ÿ��"));

	GetFormApi()->GetRepresentationBook(GetBookDM(), nIdx);

	if (GetFormApi()->GetVolumeCount(GetBookDM()) == 1)
	{
		if( nIdx < 0 )	nIdx = 0;

		GetDM()->SetCellData(_T("BP_����������"), GetBookDM()->GetCellData(_T("BB_����������"), nIdx), idx);

		//Ȯ�尳�� ���׿� �°� ����
		if ( GetBookDM()->GetCellData(_T("BB_����"), nIdx) != _T("0.00")	&&
			 GetBookDM()->GetCellData(_T("BB_����"), nIdx) != _T("0.0")		&&
			 GetBookDM()->GetCellData(_T("BB_����"), nIdx) != _T("0")		 )
		{		
			GetDM()->SetCellData(_T("BP_����"), GetBookDM()->GetCellData(_T("BB_����"), nIdx), idx);
		}
		else
		{
			GetDM()->SetCellData(_T("BP_����"), _T(""), idx);
		}

		if( pEdt )		pEdt->SetReadOnly(FALSE);
		if( pCombo )	pCombo->EnableWindow(TRUE);
	}
	else
	{
		if( pCombo )	pCombo->EnableWindow(FALSE);

		if( pCombo->GetCurSel() == 0 ) 
		{
			CString strVolPriceSum;
			GetVolPriceSum(strVolPriceSum);
			GetDM()->SetCellData(_T("BP_����"), strVolPriceSum, idx);
			if( pEdt )	pEdt->SetReadOnly(TRUE);
		}
		else 
		{
			CString strVolPrice;
			if( nIdx < 0 )	nIdx = 0;
			GetBookDM()->GetCellData(_T("BB_����"), nIdx, strVolPrice);
			GetDM()->SetCellData(_T("BP_����"), strVolPrice, idx);
			if( pEdt )	pEdt->SetReadOnly(FALSE);
		}		
	}

	// ���ݼ����� �ѷ��ش�.
	m_strPriceChar = GetBookDM()->GetCellData(_T("BB_���ݼ���"), nIdx);
	m_strPriceChar.TrimLeft();		m_strPriceChar.TrimRight();	
	GetDM()->SetCellData(_T("BP_���ݼ���"), m_strPriceChar, idx);
	UpdateData(FALSE);
	OnKillfocuscmbBSFORMPRICECHARACTER();

	//===================================================
	//2009.09.14 ADD BY PJW : �����ڴ� �˻��ؼ� �Է� �޵��� �Ѵ�.
	CEdit* pEditDonator = (CEdit*)GetDlgItem(IDC_edtBSFORM_DONATOR);
	pEditDonator->SetReadOnly(TRUE);
	//===================================================
	
	return m_pCM->AllControlDisplay(idx);
}

VOID CDonateInfoDlg::OnradREGYES() 
{
	UpdateData(TRUE);

	ShowCtrlByRegYN();
}

VOID CDonateInfoDlg::OnradREGNO() 
{
	UpdateData(TRUE);

	ShowCtrlByRegYN();
}

VOID CDonateInfoDlg::ShowCtrlByRegYN()
{
	INT cnt, i;

	if( m_nRegYN == 0 )
	{
		GetDlgItem(IDC_stcBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		SetWorkingStatus(_T("BOA211O"));//���������ڷ�
		::PostMessage(pParentMgr->GetSafeHwnd(), WORKING_STATUS_CHANGE, (DWORD)BO_DONATE, 0);
	}
	else if( m_nRegYN == 1 )
	{
		GetDlgItem(IDC_stcBSFORM_NONDONATE_REASON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON)->ShowWindow(SW_SHOW);
		SetWorkingStatus(_T("BOA212O"));//�����δ��ڷ�
		::PostMessage(pParentMgr->GetSafeHwnd(), WORKING_STATUS_CHANGE, (DWORD)BO_NONDONATE, 0);
	}
	
	cnt = GetBookDM()->GetRowCount();
	for( i=0 ; i<cnt ; i++ )
	{
		if( GetBookDM()->GetCellData(_T("BB_DB_FLAG"), i).IsEmpty() )
		{
			GetBookDM()->SetCellData(_T("BB_DB_FLAG"), _T("U"), i);
		}

		GetBookDM()->SetCellData(_T("BB_�ڷ����"), GetWorkingStatus(), i);
	}
}

INT CDonateInfoDlg::ClearScreen()
{
	CESL_ControlEdit * pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_����"));
	if(pEdt) pEdt->SetReadOnly(FALSE);
	return m_pCM->AllControlClear();
}

BOOL CDonateInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
		else if(pMsg->wParam == VK_TAB)
		{
			if(pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON)->GetSafeHwnd())
			{
				pParentMgr->PostMessage(SET_TAB_ORDER, 0, 1);	
				return TRUE;
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_PRICE_OTHER_INFO)->GetSafeHwnd() && !GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON	)->IsWindowVisible())
			{
				pParentMgr->PostMessage(SET_TAB_ORDER, 0, 1);	
				return TRUE;
			}
		}
	}
	if (pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_PRICE)->GetSafeHwnd())
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

VOID CDonateInfoDlg::OnSelchangecmbBSFORMPRICETYPE() 
{
	Display(0);	
}

INT CDonateInfoDlg::AllControlClear()
{
	return GetFormApi()->AllControlClear(m_pCM, NULL);
}

UINT CDonateInfoDlg::OnSetTabOrder(LPARAM lParam, WPARAM wParam)
{
	GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->SetFocus();
	return 0;
}



VOID CDonateInfoDlg::OnKillfocuscmbBSFORMPRICECHARACTER() 
{
	INT i, idx;
	CString strData;		

	UpdateData(TRUE);
	m_strPriceChar.TrimLeft();	m_strPriceChar.TrimRight();	

	// ���� ���õǾ��� �ε����� -1 �̶�� ���� �߰����̴�.
	// �����߰��� �ܾ �޺��ȿ� ���ٸ� �߰��Ѵ�.
	idx = m_pCmbPriceChar->GetCurSel();
	//================================
	//20080630 ADD BY PJW : ""�� ������ ""�� �߰����ش�.
	for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
	{
		m_pCmbPriceChar->GetLBText(i, strData);
		if( strData == _T("") )		break;
	}
	if( i >= m_pCmbPriceChar->GetCount() )
	{
		m_pCmbPriceChar->AddString(_T(""));
	}
	//================================
	if( idx < 0 )
	{
		for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
		{
			m_pCmbPriceChar->GetLBText(i, strData);
			if( strData == m_strPriceChar )		break;
		}

		if( i >= m_pCmbPriceChar->GetCount() )
		{
			m_pCmbPriceChar->AddString(m_strPriceChar);
		}
	}
	
	// �߰��� �ܾ ��Ʈ�Ǿ����Ƿ� �ٽ� �״ܾ ã�� �ε����� �����Ѵ�.
	for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
	{
		m_pCmbPriceChar->GetLBText(i, strData);
		if( strData == m_strPriceChar )	break;
	}

	m_pCmbPriceChar->SetCurSel(i);

	OnSelchangecmbBSFORMPRICECHARACTER();
}

VOID CDonateInfoDlg::OnSelchangecmbBSFORMPRICECHARACTER() 
{
	INT idx;
	CESL_ControlEdit* pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_����"));
	CESL_ControlMultiComboBox* pMcmb = (CESL_ControlMultiComboBox*)m_pCM->FindControl(_T("BP_ȭ�󱸺�"));
	
	if( pEdt == NULL || pMcmb == NULL ) return;
	
	idx = m_pCmbPriceChar->GetCurSel();
	if( idx < 0 )	return;

	UpdateData(TRUE);

	m_pCmbPriceChar->GetLBText(idx, m_strPriceChar);
	if( m_strPriceChar == _T("���ǰ") )	
	{
		pEdt->SetData(_T(""));
		pEdt->SetReadOnly(TRUE);
		//==========================================
		//20080630 UPDATE BY PJW : SetCurSel(0)���ϸ� ��������� ������ ������ �ִ�.
		//pMcmb->SetCurSel(0);
		INT nIdx;
		nIdx = pMcmb->SelectString(_T("    "));
		pMcmb->SetCurSel(nIdx);
		//==========================================
		pMcmb->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->EnableWindow(FALSE);
	}
	else
	{
		pMcmb->EnableWindow(TRUE);
		// ����ó������ ���� �ڷḸ ������ �����ϴ�.
		if( GetFormApi()->GetVolumeCount(GetBookDM()) == 1 )
		{
			pEdt->SetReadOnly(FALSE);
			GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->EnableWindow(TRUE);
		}
	}
	UpdateData(FALSE);
}
//2005-11-14 Ȳ���� �߰� : ���� ������ ��ư�� ����Ѵ�.
VOID CDonateInfoDlg::OnbtnBSFORMBEFOREDONATOR() 
{
	INT nRow;
	nRow = GetDM()->GetRowCount();
	if(1 > nRow)
	{
		AfxMessageBox(_T("���� ������������ �������� �ʽ��ϴ�"));
		return;
	}
	else
	{
		CString strDonatorKey, strDonatorName, strQuery;
		strDonatorKey.Empty();
		strDonatorName.Empty();
		
/*		GetDM()->GetCellData(_T("BP_����������KEY"),nRow-1, strDonatorKey);
		if( strDonatorKey.IsEmpty() )
		{
			strDonatorName = m_strBeforeDonatorName;
		}
		else
		{
			m_strDonatorInfoKey = strDonatorKey;
			strQuery.Format(_T("SELECT DONATOR FROM CO_DONATOR_TBL WHERE REC_KEY = '%s'"), strDonatorKey);
			GetDM()->GetOneValueQuery(strQuery, strDonatorName);
		}*/
		strDonatorName = m_strBeforeDonatorName;
		m_strDonatorInfoKey = m_strBeforeDonatorInfoKey;
		GetDlgItem(IDC_edtBSFORM_DONATOR)->SetWindowText(strDonatorName);

		//====================================================================
		//2009.05.20 ADD BY LKS : �������ڵ� �����ֵ��� ����
		m_pCM->SetControlMgrData(_T("BP_�Է�����"), m_strBeforeDonateDate);
		//====================================================================
	}	
}

HBRUSH CDonateInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}
