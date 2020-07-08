// SE_LOC_1310.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1310.h"

#include "ShelfChangeReasonList.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1310 dialog


CSE_LOC_1310::CSE_LOC_1310(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1310)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1310::~CSE_LOC_1310()
{
}

VOID CSE_LOC_1310::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1310)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSE_LOC_1310, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1310)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnDETAIL, OnbtnDETAIL)
	ON_BN_CLICKED(IDC_btnFIND, OnbtnFIND)
	ON_BN_CLICKED(IDC_radCondition2, OnradCondition2)
	ON_BN_CLICKED(IDC_btnINIT, OnbtnINIT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1310 message handlers

VOID CSE_LOC_1310::OnClose() 
{
	EFS_BEGIN

	// TODO: Add your message handler code here and/or call default
	
	//DestroyWindow();
	CDialog::OnCancel();

	EFS_END
}

BOOL CSE_LOC_1310::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1310")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	SetWindowSize( CString(_T("�����˻�")) );
	((CButton*)GetDlgItem( IDC_radCondition1 ))->SetCheck(1); 
	
	SetShelfChangeReasonControl();

	SetShelfWorkControl();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

BOOL CSE_LOC_1310::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CSE_LOC_1310::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CSE_LOC_1310::OnbtnDETAIL() 
{
	EFS_BEGIN

	CString sCaption;
	GetDlgItem( IDC_btnDETAIL )->GetWindowText( sCaption );
	SetWindowSize( sCaption );	

	EFS_END
}

///--------------------------------------------------------------------------------------
/// Function Name : SetWindowSize( CString &sCaption )
/// Purpose : �������� ����� �����ϱ� ���Ͽ� 
///                 ���� ũ��� IDC_staVIEWDETAIL , IDC_staVIEWSIMPLE �� ũ�⸦ �����Ͽ� �����Ѵ�.
/// 
/// Parameters : _T("�󼼰˻�") , _T("�����˻�") �� �Է� ����
///
VOID CSE_LOC_1310::SetWindowSize( CString &sCaption )
{
	EFS_BEGIN

	CRect ChangeWndPos;		/// ����� ��Ʈ�� ����� ������ ���� �ν��Ͻ�
	CRect WndPos;				  /// ���� �������� ��ġ�� ������ ���� �ν��Ͻ�	
	
	INT nIDC;
	
	TCHAR* szCaption[2] = { _T("�󼼰˻�") , _T("�����˻�") };

	// ����� _T("�󼼰˻�")���
	if( sCaption.Compare( szCaption[0] ) == 0 )
	{
		GetDlgItem( IDC_btnDETAIL )->SetWindowText( CString(szCaption[1]) );
		nIDC = IDC_staVIEWDETAIL;
	}
	// ����� _T("�����˻�")���
	else
	{
		GetDlgItem( IDC_btnDETAIL )->SetWindowText( CString(szCaption[0]) );
		nIDC = IDC_staVIEWSIMPLE;
	}

	// ���� �������� ��ġ�� ����
	GetWindowRect( &WndPos );
	
	// ����� ��ġ�� ����
	GetDlgItem( nIDC )->GetWindowRect( &ChangeWndPos );
	
	ClientToScreen(&WndPos);
	ClientToScreen(&ChangeWndPos);

	// ����� ��ġ�� ������
	WndPos.SetRect( WndPos.left , WndPos.top , WndPos.right , ChangeWndPos.bottom );
	
	ScreenToClient(&WndPos);

	MoveWindow( WndPos );	

	EFS_END
}

VOID CSE_LOC_1310::OnbtnFIND() 
{
	EFS_BEGIN

	INT ids;
	
	CBL_SE_LOC_13 SpfWork( this );
	if( ((CButton*)GetDlgItem( IDC_radCondition1 ))->GetCheck() == 1 )
	{
		ids = SpfWork.SPFExcutePath( _T("�谡�ڷ����_�˻�") );
	}
	else
	{
		ids = SpfWork.SPFExcutePath( _T("�谡�ڷ����_���Ϸΰ˻�") );
	}
	

	EFS_END
}

// �谡 ���� ���� ��Ʈ�� �����ִ� �Լ�
VOID CSE_LOC_1310::SetShelfChangeReasonControl()
{
	EFS_BEGIN


	/*
	CString sCMAlias = _T("CM_SE_LOC_1310");
	CString sControlAlias = _T("�谡�������");
	
	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("CSE_LOC_1310::SetShelfChangeResonControl()") );

	CComboBox *pComboBox = (CComboBox*)(pCM->FindControl(sControlAlias));
	if( pComboBox == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("CSE_LOC_1310::SetShelfChangeResonControl()") );

	CString *sGetData;
	sGetData = NULL;

	INT nRowCount;

	INT nReasonCnt = GetShelfChangeReasonCount();

	sGetData = new CString[nReasonCnt];

	ids = GetShelfChangeReasonList( sGetData , nRowCount );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("CSE_LOC_1310::SetShelfChangeResonControl()") );

	for( INT i = 0 ; i < nRowCount ; i++ )
	{	
		pComboBox->AddString( sGetData[i] );
	}
	
	pComboBox->SetCurSel(0);

	if( sGetData != NULL ) delete []sGetData;

	*/

	EFS_END
}


INT CSE_LOC_1310::GetShelfChangeReasonList( CString sGetData[] , INT &nRowCount )
{
	EFS_BEGIN

	CShelfChangeReasonList List(this);
	List.GetList( sGetData , nRowCount );

	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1310::GetShelfChangeReasonCount()
{
	EFS_BEGIN

	CShelfChangeReasonList List(this);
	return List.GetListCount();

	EFS_END
	return -1;

}
VOID CSE_LOC_1310::OnradCondition2() 
{
	EFS_BEGIN

	CBL_SE_LOC_13 SpfWork( this );
	INT ids = SpfWork.SPFExcutePath( _T("�谡�ڷ����_���Ϸΰ˻�") );	

	EFS_END
}

BOOL CSE_LOC_1310::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
	{
		OnCancel();
		return TRUE;
	}
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		OnbtnFIND();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}

VOID CSE_LOC_1310::OnbtnINIT() 
{
	EFS_BEGIN

	CString sCMAlias;

	sCMAlias = _T("CM_SE_LOC_1310");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnINIT") );

	pCM->AllControlClear();
	((CComboBox*)GetDlgItem(IDC_cmbSHELFSTAT))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_cmbCODECHANGE))->SetCurSel(0);

	EFS_END
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-16
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// �谡�������޺� �ڽ��� ����� �����Ѵ�.
// [  PARAMETERS  ]
//    NULL     
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
// 1. ����� DM�� ã�´�.
// 2. DM�� ������ �޺��ڽ��� �����Ѵ�.
//--------------------------------------------------------------------------------------------
INT CSE_LOC_1310::SetShelfWorkControl()
{
	INT ids;

	//-----------------------------------------------------------------------------
	// 1. ����� DM�� ã�´�.  
	//-----------------------------------------------------------------------------
	CString sDMAlias = _T("DM_MNG_ACCESS_CONTROL_1000_�����");
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if ( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1310::SetShelfWorkControl") );
	ids = pDM->RefreshDataManager( _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_1310::SetShelfWorkControl") );

	INT nRowCnt = pDM->GetRowCount();
	if ( nRowCnt < 1 ) return 0;

	//-----------------------------------------------------------------------------
	// 2. DM�� ������ �޺��ڽ��� �����Ѵ�.  
	//-----------------------------------------------------------------------------
	CString sCMAlias = _T("CM_SE_LOC_1310");
	CString sControlAlias = _T("�谡������");
	
	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_1310::SetShelfWorkControl") );

	CComboBox *pComboBox = (CComboBox*)(pCM->FindControl(sControlAlias));
	if( pComboBox == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_1310::SetShelfWorkControl") );

	CString sData = _T("�������");
	pComboBox->AddString( sData );

	for ( INT i = 0;i < nRowCnt;i++ )
	{
		ids = pDM->GetCellData( _T("USER_ID") , i , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_1310::SetShelfWorkControl") );
		sData.TrimLeft();sData.TrimRight();
		if ( sData.IsEmpty() ) continue;

		pComboBox->AddString( sData );
	}

	pComboBox->SetCurSel( 0 );
	return 0;
}
HBRUSH CSE_LOC_1310::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
