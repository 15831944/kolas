// DonateDataSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DonateDataSearchDlg.h"
#include "../BO_ACQ_API/BO_ACQ_Business_Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonateDataSearchDlg dialog


CDonateDataSearchDlg::CDonateDataSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonateDataSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonateDataSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCreate = FALSE;
	m_pCM = NULL;
	m_strDonateDate1.Empty();
	m_strDonateDate2.Empty();
}

CDonateDataSearchDlg::~CDonateDataSearchDlg()
{
	
}


VOID CDonateDataSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonateDataSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonateDataSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CDonateDataSearchDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonateDataSearchDlg message handlers

BOOL CDonateDataSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_�������ں������ڷ�˻�")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_�������ں������ڷ�˻�"));

	if(!m_strDonateDate1.IsEmpty())
		m_pCM->SetControlMgrData(_T("��������1"), m_strDonateDate1);
	if(!m_strDonateDate2.IsEmpty())
		m_pCM->SetControlMgrData(_T("��������2"), m_strDonateDate2);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDonateDataSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strTmpData;
	m_pCM->GetControlMgrData(_T("��������1"), m_strDonateDate1);
	m_pCM->GetControlMgrData(_T("��������2"), m_strDonateDate2);

	if(m_strDonateDate1.IsEmpty() && !m_strDonateDate2.IsEmpty())
	{
		ESLAfxMessageBox(_T("���� �������ڸ� �Է��� �ֽʽÿ�."));
		return;
	}


	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
	CDialog::OnOK();
}

VOID CDonateDataSearchDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CDonateDataSearchDlg::Create(CWnd *pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CDonateDataSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if(GetDlgItem(IDCANCEL)->m_hWnd == pMsg->hwnd)
		{
			GetDlgItem(IDCANCEL)->SetFocus();
			PostMessage(WM_COMMAND, IDCANCEL, BN_CLICKED);
		}
		else
		{
			GetDlgItem(IDOK)->SetFocus();
			PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CDonateDataSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
