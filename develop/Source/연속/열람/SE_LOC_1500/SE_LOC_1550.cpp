// SE_LOC_1550.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1550.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1550 dialog


CSE_LOC_1550::CSE_LOC_1550(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1550)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1550::~CSE_LOC_1550()
{
}

VOID CSE_LOC_1550::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1550)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1550, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1550)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnSAVE, OnbtnSAVE)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1550 message handlers

BOOL CSE_LOC_1550::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}
BOOL CSE_LOC_1550::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1550")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pSpfWork = new CBL_SE_LOC_15(this);
	
	DisplayDefaultValue();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/// ȭ���� ��,å �谡 ����ڸ� �����ش�.
INT CSE_LOC_1550::DisplayDefaultValue()
{
	INT ids;

	CString sMainDMAlias = _T("DM_SE_LOC_1500");

	CESL_DataMgr *pMainDM = FindDM( sMainDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayDefaultValue") );

	CESL_Grid *pMainGrid = (CESL_Grid*)FindControl(_T("CM_SE_LOC_1500"),_T("���α׸���"));
	if( pMainGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DisplayDefaultValue") );

	CString sSpeciesCnt,sBookCnt;
	ids = CLocCommonAPI::GetDMEqualRemoveCnt( this  , pMainDM , _T("������KEY") , pMainGrid , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DisplayDefaultValue") ); 

	pMainGrid->SelectMakeList();

	sBookCnt.Format(_T("%d") , pMainGrid->SelectGetCount() );

	CString sCMAlias = _T("CM_SE_LOC_1550");
	ids = SetControlData( sCMAlias , _T("����") , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DisplayDefaultValue") ); 

	ids = SetControlData( sCMAlias , _T("å��") , sBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("DisplayDefaultValue") ); 

	return 0;
}


VOID CSE_LOC_1550::OnClose() 
{
	if( m_pSpfWork != NULL ) delete m_pSpfWork;
	m_pSpfWork = NULL;
	
	CDialog::OnClose();
}

VOID CSE_LOC_1550::OnbtnSAVE() 
{
	m_pSpfWork->SPFExcutePath( RECORD_TMP_REG_NO );
	
}

VOID CSE_LOC_1550::OnbtnCLOSE() 
{
	OnCancel();	
}

HBRUSH CSE_LOC_1550::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
