// BO_LOC_3710.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3710.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3710 dialog


CBO_LOC_3710::CBO_LOC_3710(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3710)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CBO_LOC_3710::~CBO_LOC_3710()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CBO_LOC_3710::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3710)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3710, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3710)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3710 message handlers

BOOL CBO_LOC_3710::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



BOOL CBO_LOC_3710::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CBO_LOC_3710::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
