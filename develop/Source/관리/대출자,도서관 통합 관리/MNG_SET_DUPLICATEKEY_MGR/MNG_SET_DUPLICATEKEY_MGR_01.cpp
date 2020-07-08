// MNG_SET_DUPLICATEKEY_MGR_01.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_SET_DUPLICATEKEY_MGR_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_SET_DUPLICATEKEY_MGR_01 dialog


CMNG_SET_DUPLICATEKEY_MGR_01::CMNG_SET_DUPLICATEKEY_MGR_01( CESL_Mgr *pParent /*=NULL*/ )
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CMNG_SET_DUPLICATEKEY_MGR_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CMNG_SET_DUPLICATEKEY_MGR_01::~CMNG_SET_DUPLICATEKEY_MGR_01()
{
	
}


BOOL CMNG_SET_DUPLICATEKEY_MGR_01::Create( CWnd *pParentWnd )
{	
	return CDialog::Create( IDD, pParentWnd ); 
}	
	
VOID CMNG_SET_DUPLICATEKEY_MGR_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_SET_DUPLICATEKEY_MGR_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_SET_DUPLICATEKEY_MGR_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_SET_DUPLICATEKEY_MGR_01)
	ON_WM_SHOWWINDOW()
	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_SET_DUPLICATEKEY_MGR_01 message handlers

BOOL CMNG_SET_DUPLICATEKEY_MGR_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/* ������
2 : ����
3 : ����
alias : ���;��ε��ߺ�Ű����
value name : Ű��ȣ
vlaue_1 : 1

*/

VOID CMNG_SET_DUPLICATEKEY_MGR_01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow )
		ShowManageValue();
}

VOID CMNG_SET_DUPLICATEKEY_MGR_01::ShowManageValue()
{
	// T : ���, F : �Ȼ��
	// �ֹι�ȣ, �ڵ�����ȣ, �����ڹ�ȣ, �̸�
	
	CString manageValue;
	GetManageValue( _T("����"), _T("����"), _T("���;��ε��ߺ�Ű����"), _T("�ߺ�Ű"), manageValue );

	SetManageCheckBtns( manageValue );
}

VOID CMNG_SET_DUPLICATEKEY_MGR_01::SetManageCheckBtns( CString manageValue )
{
	const INT cnt = 4;
	UINT id[ cnt ] = {
		IDC_CHECK1, IDC_CHECK2, IDC_CHECK3, IDC_CHECK4 };
	CButton *pCheck;
	for( INT i = 0; i < cnt; i++ )
		if( _T("T") == manageValue.Mid( i, 1 ) )
		{
			pCheck = ( CButton * )GetDlgItem( id[ i ] );
			pCheck->SetCheck( true );
		}
}

VOID CMNG_SET_DUPLICATEKEY_MGR_01::UpdateManageValue()
{
	if( SetManageValue( _T("����"), _T("����"), _T("���;��ε��ߺ�Ű����"), _T("�ߺ�Ű"), TakeManageValue() ) )
		MessageBox( _T("Error : SetManageValue()...") );
	else
		MessageBox( _T("���泻���� ����Ǿ����ϴ�.") );
}

CString CMNG_SET_DUPLICATEKEY_MGR_01::TakeManageValue()
{
	CString manageValue;
	
	const INT cnt = 4;
	UINT id[ cnt ] = {
		IDC_CHECK1, IDC_CHECK2, IDC_CHECK3, IDC_CHECK4 };
	CButton *pCheck;
	for( INT i = 0; i < cnt; i++ )
	{
		pCheck = ( CButton * )GetDlgItem( id[ i ] );
		
		if( pCheck->GetCheck() )
			manageValue += _T("T");
		else
			manageValue += _T("F");
	}

	return manageValue;
}



HBRUSH CMNG_SET_DUPLICATEKEY_MGR_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


