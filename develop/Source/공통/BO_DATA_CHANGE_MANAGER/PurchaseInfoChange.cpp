// PurchaseInfoChange.cpp : implementation file
//

#include "stdafx.h"
#include "PurchaseInfoChange.h"
#include "EXCHANGERATE_MGR.h"
#include "DataChangeManager.h"
#include "PurchaseInfoChangeCondition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPurchaseInfoChange dialog


CPurchaseInfoChange::CPurchaseInfoChange(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CPurchaseInfoChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPurchaseInfoChange)
	m_bCurrencyCode = TRUE;
	m_bDiscountRate = TRUE;
	m_bExchangeRate = TRUE;
	m_bProvider = TRUE;
	m_bRemark = TRUE;
	m_bBookCount = TRUE;
	//20070604 ADD PJW ��ó üũ�ڽ��߰�
	m_bApplicant = TRUE;

	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM_BOOK = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;
	m_nSpeciesType = 0;
}

CPurchaseInfoChange::~CPurchaseInfoChange()
{
}

VOID CPurchaseInfoChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPurchaseInfoChange)
	DDX_Control(pDX, IDC_cmbPC_EXCHANGERATE, m_Price);
	DDX_Check(pDX, IDC_chkCURRENCY_CODE, m_bCurrencyCode);
	DDX_Check(pDX, IDC_chkDISCOUNT_RATE, m_bDiscountRate);
	DDX_Check(pDX, IDC_chkPROVIDER, m_bProvider);
	DDX_Check(pDX, IDC_chkREMARK, m_bRemark);
	DDX_Check(pDX, IDC_chkBOOKCOUNT, m_bBookCount);
	//20070604 ADD PJW ��ó üũ�ڽ��߰�
	DDX_Check(pDX, IDC_chkAPPLICANT, m_bApplicant);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPurchaseInfoChange, CDialog)
	//{{AFX_MSG_MAP(CPurchaseInfoChange)
	ON_BN_CLICKED(IDC_chkCURRENCY_CODE, OnchkCURRENCYCODE)
	ON_BN_CLICKED(IDC_chkDISCOUNT_RATE, OnchkDISCOUNTRATE)
	ON_BN_CLICKED(IDC_chkPROVIDER, OnchkPROVIDER)
	ON_BN_CLICKED(IDC_chkREMARK, OnchkREMARK)
	ON_CBN_SELCHANGE(IDC_cmbPC_CURRENCY_CLASS, OnSelchangecmbPCCURRENCYCLASS)
	ON_BN_CLICKED(IDC_btnPC_EXCHANGE_RATE_MGR, OnbtnPCEXCHANGERATEMGR)
	ON_EN_KILLFOCUS(IDC_edtPC_ORDER_DISCOUNT_RATE, OnKillfocusedtPCORDERDISCOUNTRATE)
	ON_BN_CLICKED(IDC_chkBOOKCOUNT, OnchkBOOKCOUNT)
	ON_BN_CLICKED(IDC_chkAPPLICANT, OnchkAPPLICANT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPurchaseInfoChange message handlers

BOOL CPurchaseInfoChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_������������")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM����
	m_pCM = FindCM(_T("CM_������������"));		
	
	OnSelchangecmbPCCURRENCYCLASS();
	OnchkCURRENCYCODE();
	OnchkDISCOUNTRATE();
	OnchkPROVIDER();
	OnchkREMARK();
	ShowControlBySpeciesType();
	OnchkBOOKCOUNT();
	//20070604 ADD PJW ��ó üũ�ڽ��߰�
	OnchkAPPLICANT();

//	CESL_Mgr * m_pParentMgr;
	CString str64;
//	m_pParentMgr->GetManageValue( _T("��Ÿ"), _T("����"), _T("����"), _T("����"), str64 );
	GetManageValue( _T("��Ÿ"), _T("����"), _T("����"), _T("����"), str64 );
	if ( str64 != _T("Y") )
	{
		
		//GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(FALSE);
		//GetDlgItem(IDC_chkAPPLICANT)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(FALSE);
		//20070604 ADD PJW ��ó üũ�ڽ��߰�
		GetDlgItem(IDC_chkAPPLICANT)->ShowWindow(FALSE);
		GetDlgItem(IDC_cAPPLICAND_DEPT)->ShowWindow(FALSE);
	}

	EnableThemeDialogTexture(GetSafeHwnd());	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CPurchaseInfoChange::OnchkCURRENCYCODE() 
{
	m_bCurrencyCode = !m_bCurrencyCode;
	m_bExchangeRate = !m_bExchangeRate;

	if(m_bCurrencyCode) 
	{
		GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(TRUE);
		GetDlgItem(IDC_cmbPC_CURRENCY_CLASS_PRE)->EnableWindow(TRUE);
		GetDlgItem(IDC_cmbPC_EXCHANGERATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnPC_EXCHANGE_RATE_MGR)->EnableWindow(TRUE);	
	}
	else 
	{
		GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbPC_CURRENCY_CLASS_PRE)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbPC_EXCHANGERATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnPC_EXCHANGE_RATE_MGR)->EnableWindow(FALSE);	
	}
	UpdateData(FALSE);
}


VOID CPurchaseInfoChange::OnchkDISCOUNTRATE() 
{
	// TODO: Add your control notification handler code here
	m_bDiscountRate = !m_bDiscountRate;
	if(m_bDiscountRate) GetDlgItem(IDC_edtPC_ORDER_DISCOUNT_RATE)->EnableWindow(TRUE);
	else GetDlgItem(IDC_edtPC_ORDER_DISCOUNT_RATE)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}


VOID CPurchaseInfoChange::OnchkPROVIDER() 
{
	// TODO: Add your control notification handler code here
	m_bProvider = !m_bProvider;
	if(m_bProvider) GetDlgItem(IDC_cmbPC_PROVIDER)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbPC_PROVIDER)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

VOID CPurchaseInfoChange::OnchkREMARK() 
{
	// TODO: Add your control notification handler code here
	m_bRemark = !m_bRemark;
	if(m_bRemark) GetDlgItem(IDC_edtPC_REMARKS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_edtPC_REMARKS)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

VOID CPurchaseInfoChange::OnchkBOOKCOUNT() 
{
	// TODO: Add your control notification handler code here
	m_bBookCount = !m_bBookCount;
	if(m_bBookCount) GetDlgItem(IDC_edtBOOKCOUNT)->EnableWindow(TRUE);
	else GetDlgItem(IDC_edtBOOKCOUNT)->EnableWindow(FALSE);
	UpdateData(FALSE);	
}

//20070604 ADD PJW ��ó üũ�ڽ��߰�
VOID CPurchaseInfoChange::OnchkAPPLICANT()
{
	// TODO: Add your control notification handler code here
	m_bApplicant = !m_bApplicant;
	if(m_bApplicant) GetDlgItem(IDC_cAPPLICAND_DEPT)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cAPPLICAND_DEPT)->EnableWindow(FALSE);
	UpdateData(FALSE);	
}


VOID CPurchaseInfoChange::OnOK() 
{
	UpdateData(TRUE);  // 2005-02-03
	INT ids = ValidateData();
	if(ids < 0) return ;

	CString strExchangeRate;
	CComboBox *pCombo;

	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbPC_EXCHANGERATE );
	pCombo->GetWindowText( strExchangeRate );

	CDataChangeManager mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM_BOOK);
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)m_pMainGrid);
	mgr.SetExchangeRate( strExchangeRate );

	CPurchaseInfoChangeCondition cont;
	cont.SetCurrencyCode(m_bCurrencyCode);
	cont.SetDiscountRate(m_bDiscountRate);
	cont.SetExchangeRate(m_bExchangeRate);
	cont.SetProvider(m_bProvider);
	cont.SetRemark(m_bRemark);
	cont.SetBookCount(m_bBookCount);
	//20070604 ADD PJW ��ó üũ�ڽ��߰�
	cont.SetApplicant(m_bApplicant);

	
	mgr.SetTempPtr(_T("Condition"), (DWORD*)&cont);
	ids = mgr.SPFExecutePath(_T("������������"));
	if(ids < 0)
	{
		//����ó��
		ESLAfxMessageBox(_T("�����Ͻ� �ڷ��� ���������� ������ �� �����ϴ�."));
		CDialog::OnCancel();
		return ;
	}	
	
	ESLAfxMessageBox(_T("�����Ͻ� �ڷ��� ���������� �����Ͽ����ϴ�."));
	CDialog::OnOK();	
}

VOID CPurchaseInfoChange::OnSelchangecmbPCCURRENCYCLASS() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	if(!m_bCurrencyCode) return ;
	CDataChangeManager mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	
	// �ֱ� ȯ�� ���� ��ſ� ���뿩�ΰ� Y�� ȯ�������� �����Ѵ�.
	// 2005-09-28 ������ -> ����� �䱸
	//
	INT ids = mgr.SPFExecutePath(_T("�ֱ�ȯ����������"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ ȭ�󱸺п� ȯ�������� �����ϴ�. ȯ�������� ������ �ֽʽÿ�."));
		GetDlgItem(IDC_btnPC_EXCHANGE_RATE_MGR)->SetFocus();
		m_Price.ResetContent();
		return;
	}
	CString strExchangeRate;
	mgr.GetTempData(_T("ExchangeRate"), strExchangeRate);
	

	//ȯ������ �ݿ�
	//
	m_Price.ResetContent();

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	CString year;

	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	year = result;

	CString strCurrencyCode;
	m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strCurrencyCode);
	
	CString query, startdate, enddate, key;
	query.Format(_T("select rec_key from cd_code_tbl where class = 23 and code like '%s%%'"), strCurrencyCode); 
	dm.GetOneValueQuery(query, key);
	startdate.Format(_T("%s/%s/%s"), year, _T("01"), _T("01"));
	enddate.Format(_T("%s/%s/%s"), year, _T("12"), _T("31"));
	query.Format(_T("SELECT EXCHANGE_RATE FROM CO_EXCHANGE_RATE_TBL WHERE USER_CODE_MNG_KEY = %s AND INPUT_DATE >= '%s' AND INPUT_DATE <= '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), key, startdate, enddate);
//DEL 	query.Format(_T("SELECT EXCHANGE_RATE FROM CO_EXCHANGE_RATE_TBL WHERE USER_CODE_MNG_KEY = %s AND INPUT_DATE >= '%s' AND INPUT_DATE <= '%s'"), key, startdate, enddate);
	ids = dm.RestructDataManager(query);

	// ���뿩�ο� 'Y'�� �� ȯ���� ���� �Ѵ�.
	// 2005-09-28 ������ - ����� �䱸
	//		
	for ( INT nIdx = 0 ;nIdx < dm.GetRowCount() ; nIdx++ )
	{
		m_Price.InsertString( nIdx, dm.GetCellData( nIdx, 0 ));
		if(dm.GetCellData(nIdx,0) == strExchangeRate) ids = nIdx;
	}

	if(strExchangeRate.IsEmpty()) m_Price.SetCurSel(0);
	else
	{
		m_Price.SetCurSel(ids);
	}	
	//
	m_pCM->SetControlMgrData(_T("BP_ȯ��"), strExchangeRate);
}

VOID CPurchaseInfoChange::OnbtnPCEXCHANGERATEMGR() 
{
	// TODO: Add your control notification handler code here
	CString strCurrencyCode;
	m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strCurrencyCode);
	if(m_bCurrencyCode == 0  || strCurrencyCode.IsEmpty() || strCurrencyCode == _T("") )
	{
		ESLAfxMessageBox(_T("ȭ�󱸺��� ������ �ֽʽÿ�."));
		return ;
	}
	CEXCHANGERATE_MGR dlg(this, strCurrencyCode);
	CString exchangerate;
	if(IDOK == dlg.DoModal())
	{
		exchangerate = dlg.m_exchangerate;
		m_pCM->SetControlMgrData(_T("BP_ȯ��"), exchangerate);
	}

	OnSelchangecmbPCCURRENCYCLASS();
	return ;	
}

INT CPurchaseInfoChange::ValidateData()
{
	CString strTmpData, strDiscRate;
	//if(!m_bDiscountRate) return 0;

	// 2005-02-02 �������̳� ȯ���� üũ�� �Ǿ� ���� ��� å���� üũ�� �Ǿ� ������ ������ �� ���ٴ� �޽��� ���� �����Ѵ�.
	if( (m_bDiscountRate || m_bExchangeRate) && m_bBookCount)
	{
		ESLAfxMessageBox(_T("������/ȯ���� å���� ���ÿ� ������ �� �����ϴ�."));
		return -1;
	}

	// 2005-02-02 å���� 1���� ������ �ȵȴ�.
	if(m_bBookCount)
	{
		m_pCM->GetControlMgrData(_T("BB_å��"), strTmpData);
		if(strTmpData.IsEmpty())
		{
			ESLAfxMessageBox(_T("å���� �Է��Ͻʽÿ�."));
			return -1;
		}
		if(_ttoi(strTmpData) < 1)
		{
			ESLAfxMessageBox(_T("å���� 1���� ���� �� �����ϴ�."));
			m_pCM->SetControlMgrData(_T("BB_å��"), _T("1"));
			return -1;
		}
	}
	
	if(m_bDiscountRate)
	{
		m_pCM->GetControlMgrData(_T("BP_������"), strDiscRate);
		if(strDiscRate.IsEmpty()) 
		{
			ESLAfxMessageBox(_T("�������� �Է��Ͻʽÿ�."));
			return -1;
		}
		INT nPos = strDiscRate.Find(_T("."));
		if(nPos > 0) strTmpData = strDiscRate.Left(nPos);
		if(strTmpData.GetLength() > 3)
		{
			ESLAfxMessageBox(_T("������������ �Ҽ����� �ڷᰡ 3�ڸ� �̻��� �� �����ϴ�."));
			return -1;
		}

		DOUBLE dTmpData;
		//dTmpData = (DOUBLE)atof(strDiscRate.GetBuffer(0));
		TCHAR* pEndPtr = NULL;
		dTmpData = _tcstod(strDiscRate.GetBuffer(0), &pEndPtr);

		if(dTmpData > 100.0)
		{
			ESLAfxMessageBox(_T("������ ������ 100���� Ŭ �� �����ϴ�."));
			m_pCM->SetControlMgrData(_T("BP_������"), _T("0"));
			return -1;
		}
	}

	if(m_bExchangeRate)
	{
//		m_pCM->GetControlMgrData(_T("BP_ȯ��"), strTmpData);
//		if(strTmpData.IsEmpty()) 
//		{
//			ESLAfxMessageBox(_T("ȯ���� �Է��Ͻʽÿ�."));
//			return -1;
//		}
		//����
		CString strTmp;
		CComboBox *pCombo;

		pCombo = ( CComboBox * )GetDlgItem( IDC_cmbPC_EXCHANGERATE );
		pCombo->GetWindowText( strTmp );
		if(strTmp.IsEmpty()) 
		{
			ESLAfxMessageBox(_T("ȯ���� �Է��Ͻʽÿ�."));
			return -1;
		}

//		for ( INT nIdx = 0 ; nIdx < m_pDM->GetRowCount() ; nIdx++ ) 
//		{
//			m_pDM->SetCellData( _T("BP_������"), strTmp, nIdx ) ;
//		}
		//BP_������
	}

	return 0;
}



VOID CPurchaseInfoChange::OnKillfocusedtPCORDERDISCOUNTRATE() 
{
	/*
	UpdateData(FALSE);
	ValidateData();
	*/
}

BOOL CPurchaseInfoChange::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_edtPC_ORDER_DISCOUNT_RATE)->GetSafeHwnd() || pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_edtBOOKCOUNT)->GetSafeHwnd() )
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



VOID CPurchaseInfoChange::ShowControlBySpeciesType()
{
	if(1 == m_nSpeciesType)
	{
		GetDlgItem(IDC_chkBOOKCOUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_edtBOOKCOUNT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_chkBOOKCOUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtBOOKCOUNT)->EnableWindow(FALSE);
	}
}

HBRUSH CPurchaseInfoChange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
