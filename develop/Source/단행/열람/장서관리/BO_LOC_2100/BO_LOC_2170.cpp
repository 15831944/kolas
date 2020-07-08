// BO_LOC_2170.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_2170.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2170 dialog


CBO_LOC_2170::CBO_LOC_2170(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_2170)
	m_pSpfWork = NULL;
	//}}AFX_DATA_INIT
}

CBO_LOC_2170::~CBO_LOC_2170()
{
	if( m_pSpfWork != NULL)
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}
}

VOID CBO_LOC_2170::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_2170)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_2170, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_2170)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2170 message handlers

BOOL CBO_LOC_2170::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("BO_LOC_2170")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pSpfWork = new CBLBookCheckProc(this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2170::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	INT ids = m_pSpfWork->SPFExcutePath( BOOK_CHECK_END );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnShowWindow") );

	EFS_END
}

VOID CBO_LOC_2170::OnClose() 
{
	EFS_BEGIN

	if( m_pSpfWork != NULL ) delete m_pSpfWork;
	m_pSpfWork = NULL;

	CDialog::OnClose();

	EFS_END
}


BOOL CBO_LOC_2170::Create(CWnd* pParentWnd) 
{	
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2170::OnbtnCLOSE() 
{
	EFS_BEGIN

	bool bThreadState = m_pSpfWork->GetThreadState();

	if( !bThreadState )
	{
		INT ids = MessageBox( _T("���˿Ϸ� ���Դϴ�. �ߴ��Ͻðڽ��ϱ�?") , _T("���� �Ϸ�") , MB_YESNO | MB_ICONQUESTION );
		if( ids == IDYES ) m_pSpfWork->SetStopThread();
	}
	else
		OnCancel();

	EFS_END
}

HBRUSH CBO_LOC_2170::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
