#include "stdafx.h"
#include "BO_LOC_3191.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBO_LOC_3191::CBO_LOC_3191(CESL_Mgr* pParent /*=NULL*/ , CLoanShareManager* pLoanShareManager ,  CLonaManageValue *pLona/*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{	
	m_chkClose = FALSE;	
	m_pLona = pLona;
	m_pLoanShareManager = pLoanShareManager;
	
	m_pSpfWork = NULL;
}

CBO_LOC_3191::~CBO_LOC_3191()
{
	
	if( m_pSpfWork != NULL )
		delete m_pSpfWork;
	m_pSpfWork = NULL;	
}

VOID CBO_LOC_3191::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CBO_LOC_3191)
	DDX_Check(pDX, IDC_chkAUTOCLOSE, m_chkClose);
	//}}AFX_DATA_MAP	

	EFS_END
}

BEGIN_MESSAGE_MAP(CBO_LOC_3191, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3191)
	ON_BN_CLICKED(IDC_btnAUTO_JOB_CLOSE, OnbtnAUTOJOBCLOSE)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBO_LOC_3191::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	if(InitESL_Mgr(_T("BO_LOC_3191")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_chkClose = TRUE;
	UpdateData(FALSE);

	m_pSpfWork = new CBL_LOC_LOAN_PROC( this , m_pLoanShareManager , m_pLona );

	RunProc();

	SetTimer( 1 , 1000 , NULL );

	return TRUE;  
	              

	EFS_END
	return FALSE;

}

BOOL CBO_LOC_3191::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

INT CBO_LOC_3191::RunProc()
{
	EFS_BEGIN

	m_pSpfWork->SPFExcutePath( _T("ȭ��ε�_�ڵ���ɼ���") );

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3191::OnbtnAUTOJOBCLOSE() 
{
	EFS_BEGIN

	bool bThreadState = m_pSpfWork->GetThreadState();

	if( !bThreadState )
	{
		INT ids = MessageBox( _T("�ߴ��Ͻðڽ��ϱ�?") , _T("���� ���� �ڵ� �ϰ� ���� & ���� ������ ���� ���� �ڷ� �ڵ� ���� ���") , MB_YESNO | MB_ICONQUESTION );
		if( ids == IDYES ) m_pSpfWork->SetStopThread();
	}
	else
		OnCancel();
	

	EFS_END
}

VOID CBO_LOC_3191::OnTimer(UINT nIDEvent) 
{
	EFS_BEGIN

	if( nIDEvent == 1 )
	{
		KillTimer(1);

		UpdateData();

		INT bThreadEndState = m_pSpfWork->GetThreadEndState();
		if( (bThreadEndState == 0) && m_chkClose )
		{
			OnCancel();
		}
		else if( bThreadEndState == 1 )
		{
			return;
		}
		else
		{
			SetTimer( 1 , 1000 , NULL );
		}

	}
	
	CDialog::OnTimer(nIDEvent);

	EFS_END
}

HBRUSH CBO_LOC_3191::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
