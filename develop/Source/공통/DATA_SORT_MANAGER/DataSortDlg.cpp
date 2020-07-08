// DataSortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataSortDlg.h"
#include "DataSortElementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSortDlg dialog


CDataSortDlg::CDataSortDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDataSortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataSortDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMainDM = NULL;
	m_pComboMgr  = NULL;
	m_pCM = NULL;
	m_pItemSet = NULL;
	m_nOpenMode = 0;
}

CDataSortDlg::~CDataSortDlg()
{
	if(m_pComboMgr) 
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
}

VOID CDataSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataSortDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataSortDlg, CDialog)
	//{{AFX_MSG_MAP(CDataSortDlg)
	ON_BN_CLICKED(IDC_btnSET_ORDER_ELEMENT, OnbtnSETORDERELEMENT)
	ON_BN_CLICKED(IDC_btnSAVE_DEFAULT_VALUE, OnbtnSAVEDEFAULTVALUE)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_1, OnSelchangecmbORDERSORT1)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_2, OnSelchangecmbORDERSORT2)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_3, OnSelchangecmbORDERSORT3)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_4, OnSelchangecmbORDERSORT4)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_COND_1, OnSelchangecmbORDERSORTCOND1)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_COND_2, OnSelchangecmbORDERSORTCOND2)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_COND_3, OnSelchangecmbORDERSORTCOND3)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_COND_4, OnSelchangecmbORDERSORTCOND4)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataSortDlg message handlers

BOOL CDataSortDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_�ֹ��Ϸù�ȣ")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_�ֹ��Ϸù�ȣ"));
	
	if(m_pItemSet == NULL)
	{
		ESLAfxMessageBox(_T("�ʱⰪ ������ �ùٸ��� �ʽ��ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!Init())
	{
		ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDataSortDlg::Init()
{
	m_pComboMgr = new CMulitComboBoxManager(this);
	
	if(m_nOpenMode == 0) return FALSE;
	if(m_nOpenMode < 0) GetDlgItem(IDOK)->ShowWindow(FALSE);
	else if(m_nOpenMode > 0) GetDlgItem(IDOK)->ShowWindow(TRUE);
	
	if(!m_pItemSet->IsInit()) return FALSE;
	
	if(m_pItemSet->GetDataCount() == 0)
	{
		ESLAfxMessageBox(_T("������ �����׸������� �����ϴ�. ���� �����׸��� ������ �ֽʽÿ�."));
		GetDlgItem(IDC_btnSAVE_DEFAULT_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return TRUE;
	}
	INT ids = SetComboData();
	if(ids < 0) return FALSE;
	
	return TRUE;
}

INT CDataSortDlg::SetComboData()
{
	CString strInitValue;
	CStringArray tmpArr;	tmpArr.RemoveAll();
	m_pItemSet->SortDM(_T("�켱����"), FALSE);
	INT ids = m_pItemSet->GetSortItemOfPriority(&tmpArr, 1, strInitValue);
	if(ids < 0) return ids;
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strInitValue, &tmpArr, _T("��1����"));
	ids = m_pItemSet->GetSortItemOfPriority(&tmpArr, 2, strInitValue);
	if(ids < 0) return ids;
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strInitValue, &tmpArr, _T("��2����"));
	ids = m_pItemSet->GetSortItemOfPriority(&tmpArr, 3, strInitValue);
	if(ids < 0) return ids;
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strInitValue, &tmpArr, _T("��3����"));
	ids = m_pItemSet->GetSortItemOfPriority(&tmpArr, 4, strInitValue);
	if(ids < 0) return ids;
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strInitValue, &tmpArr, _T("��4����"));
	
	CString strSortCond;
	tmpArr.RemoveAll();	
	tmpArr.Add(_T("��������"));		tmpArr.Add(_T("��������"));
	if(m_pItemSet->GetSortDirection(1) == ESL_DATAMGR_SORT_DESC) strSortCond = _T("��������");
	else strSortCond = _T("��������");
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strSortCond, &tmpArr, _T("��1��������"));
	if(m_pItemSet->GetSortDirection(2) == ESL_DATAMGR_SORT_DESC) strSortCond = _T("��������");
	else strSortCond = _T("��������");
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strSortCond, &tmpArr, _T("��2��������"));
	if(m_pItemSet->GetSortDirection(3) == ESL_DATAMGR_SORT_DESC) strSortCond = _T("��������");
	else strSortCond = _T("��������");
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strSortCond, &tmpArr, _T("��3��������"));
	if(m_pItemSet->GetSortDirection(4) == ESL_DATAMGR_SORT_DESC) strSortCond = _T("��������");
	else strSortCond = _T("��������");
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strSortCond, &tmpArr, _T("��4��������"));

	return 0;
}

VOID CDataSortDlg::OnbtnSETORDERELEMENT() 
{
	// TODO: Add your control notification handler code here
	CDataSortElementDlg dlg(this);
	dlg.SetSortItemSet(m_pItemSet);
	if(IDOK == dlg.DoModal())
	{
		GetDlgItem(IDC_btnSAVE_DEFAULT_VALUE)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		INT ids = SetComboData();
		if(ids < 0) return ;
	}
}

VOID CDataSortDlg::OnbtnSAVEDEFAULTVALUE() 
{
	// TODO: Add your control notification handler code here
	INT ids = m_pItemSet->WriteSortItem();
	if(ids < 0)
		ESLAfxMessageBox(_T("�����Ͻ� �����׸� �⺻���� ������ �� �����ϴ�."));
	else
		ESLAfxMessageBox(_T("�����Ͻ� �����׸� �⺻���� �����Ͽ����ϴ�."));
}

VOID CDataSortDlg::OnOK() 
{
	INT ids = m_pItemSet->DataSort(m_pMainDM);
	if(ids < 0)
		ESLAfxMessageBox(_T("�ڷḦ ������ �� �����ϴ�."));
	else
		ESLAfxMessageBox(_T("�ڷḦ �����Ͽ����ϴ�."));
	
	CDialog::OnOK();
}

VOID CDataSortDlg::OnSelchangecmbORDERSORT1() 
{
	// TODO: Add your control notification handler code here
	RestructSortItem(_T("��1����"), 1);
}

VOID CDataSortDlg::OnSelchangecmbORDERSORT2() 
{
	// TODO: Add your control notification handler code here
	RestructSortItem(_T("��2����"), 2);
}

VOID CDataSortDlg::OnSelchangecmbORDERSORT3() 
{
	// TODO: Add your control notification handler code here
	RestructSortItem(_T("��3����"), 3);	
}

VOID CDataSortDlg::OnSelchangecmbORDERSORT4() 
{
	// TODO: Add your control notification handler code here
	RestructSortItem(_T("��4����"), 4);
}

VOID CDataSortDlg::OnSelchangecmbORDERSORTCOND1() 
{
	// TODO: Add your control notification handler code here
	SetSortDirection(_T("��1��������"), 1);
}

VOID CDataSortDlg::OnSelchangecmbORDERSORTCOND2() 
{
	// TODO: Add your control notification handler code here
	SetSortDirection(_T("��2��������"), 2);
}

VOID CDataSortDlg::OnSelchangecmbORDERSORTCOND3() 
{
	// TODO: Add your control notification handler code here
	SetSortDirection(_T("��3��������"), 3);
}

VOID CDataSortDlg::OnSelchangecmbORDERSORTCOND4() 
{
	// TODO: Add your control notification handler code here
	SetSortDirection(_T("��4��������"), 4);
}

INT CDataSortDlg::RestructSortItem(CString strAlias, INT nPriority)
{
	CString strTmpData;
	m_pCM->GetControlMgrData(strAlias, strTmpData);
	INT ids = m_pItemSet->SetSortPriority(strTmpData, nPriority);
	if(ids < 0)
	{
		ESLAfxMessageBox(strAlias + _T("�� ������ �� �����ϴ�."));	
		return -1;
	}
	ids = SetComboData();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�����׸��� ����� �� �����ϴ�."));
		return -1;
	}
	return 0;
}

INT CDataSortDlg::SetSortDirection(CString strAlias, INT nPriority)
{
	CString strTmpData;
	INT nSortDirection;
	m_pCM->GetControlMgrData(strAlias, strTmpData);
	if(strTmpData == _T("��������")) nSortDirection = ESL_DATAMGR_SORT_DESC;
	else nSortDirection = ESL_DATAMGR_SORT_ASC;
	INT ids = m_pItemSet->SetSortDirection(nSortDirection, nPriority);
	if(ids < 0)
	{
		ESLAfxMessageBox(strAlias + _T("�� ������ �� �����ϴ�."));	
		return -1;
	}	
	return 0;
}

HBRUSH CDataSortDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
