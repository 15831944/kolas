// BO_ACQ_Donate_Statistics_Mgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_Donate_Statistics_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_Donate_Statistics_Mgr dialog

CBO_ACQ_Donate_Statistics_Mgr::CBO_ACQ_Donate_Statistics_Mgr(CESL_Mgr* pParent /*=NULL*/)
	: CStatistics(CBO_ACQ_Donate_Statistics_Mgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_Donate_Statistics_Mgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_ACQ_Donate_Statistics_Mgr::~CBO_ACQ_Donate_Statistics_Mgr()
{
}

VOID CBO_ACQ_Donate_Statistics_Mgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_Donate_Statistics_Mgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_Donate_Statistics_Mgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_Donate_Statistics_Mgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(CHANGE_CLASS_NO_TYPE, OnChangeClassNoType)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_Donate_Statistics_Mgr message handlers

VOID CBO_ACQ_Donate_Statistics_Mgr::OnClose() 
{
	DestroyWindow();
}

BOOL CBO_ACQ_Donate_Statistics_Mgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strIs90 = _T(""), strSM;
	
	GetManageValue(_T("��Ÿ"), _T("����"), _T("�����"), _T(""), strIs90);

	strSM = _T("K2UPGRADE_�������");
	if( InitESL_Mgr(strSM) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	{
	SetCM(FindCM(_T("CM_�������")));
	SetDM(FindDM(_T("DM_����ڷ�")));
	}
	
	SetGrid((CESL_Grid*)GetCM()->FindControl(_T("MainGrid")));

	GetGrid()->SetSort(FALSE);
	
	GetDlgItem(IDC_stcDONATE_STATISTICS_YEAR		)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtDONATE_STATISTICS_YEAR		)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcDONATE_STATISTICS				)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtDONATE_STATISTICS				)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcDONATE_STATISTICS_CONDITION	)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtDONATE_STATISTICS_CONDITION	)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcDONATE_STATISTICS_UNIT		)->ShowWindow(SW_HIDE);
	
	CreateConditionDlg(DONATE);

	InitPopupMenuItem();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

VOID CBO_ACQ_Donate_Statistics_Mgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	CButton *pBtn = (CButton*)GetDlgItem(IDC_stcDONATE_STATISTICS_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtDONATE_STATISTICS_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcDONATE_STATISTICS);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(170, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtDONATE_STATISTICS);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(210, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	pBtn = (CButton*)GetDlgItem(IDC_stcDONATE_STATISTICS_CONDITION);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(390, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtDONATE_STATISTICS_CONDITION);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(430, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	pBtn = (CButton*)GetDlgItem(IDC_stcDONATE_STATISTICS_UNIT);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(670, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridDONATE_STATISTICS_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(-2, 40, cx+2, cy-38);
}

UINT CBO_ACQ_Donate_Statistics_Mgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	SetStatisticsInfo();
	DisplayGrid();
	return 0;
}

UINT CBO_ACQ_Donate_Statistics_Mgr::OnChangeClassNoType(LPARAM lParam, WPARAM wParam)
{
	ChangeColumnHeaderOfSubjectCode();
	return 0;
}

HBRUSH CBO_ACQ_Donate_Statistics_Mgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
