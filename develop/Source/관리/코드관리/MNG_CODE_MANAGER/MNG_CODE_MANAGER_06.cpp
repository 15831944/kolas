// MNG_CODE_MANAGER_06.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CODE_MANAGER_06.h"
#include "MNG_CODE_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_06 dialog


CMNG_CODE_MANAGER_06::CMNG_CODE_MANAGER_06(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_MANAGER_06)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bModify = false;
}

BOOL CMNG_CODE_MANAGER_06::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CODE_MANAGER_06::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_MANAGER_06)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_MANAGER_06, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_MANAGER_06)
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_06 message handlers

BOOL CMNG_CODE_MANAGER_06::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if( InitESL_Mgr( _T("MNG_CODE_MANAGER_06") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CODE_MANAGER_04") );
		return false;
	}

	m_index = -1;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ����
VOID CMNG_CODE_MANAGER_06::OnOK() 
{
	// TODO: Add extra validation here
	
	CString data[ 20 ];
	if( GetCodeData( data ) )
	{
		AfxMessageBox( _T("�ڵ尪�� �Է��� �ֽʽÿ�") );
		return;
	}
	
	CMNG_CODE_MANAGER_01 *pParent = ( CMNG_CODE_MANAGER_01 * )pParentMgr;
	INT ids;
	ids = pParent->SaveCodeData( data, m_index );
	if( ids < 0 )	
	{
		AfxMessageBox( _T("�ڵ� �Է¿� �����Ͽ����ϴ�.") );
		return;
	}	
	else if ( ids == 100 )
	{
		AfxMessageBox( _T("�ڵ尡 �ߺ��Ǿ����ϴ�."));
		return;
	}
	
	m_bModify = true;
	switch( m_type )
	{
		case 1 :			
			AfxMessageBox( _T("�ڵ尡 ����Ǿ����ϴ�.") );
			CDialog::OnOK();
			break;
		case 2 :
			AfxMessageBox( _T("�ڵ尡 �����Ǿ����ϴ�.") );
	}	
}

// �ݱ�
VOID CMNG_CODE_MANAGER_06::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

// ����
VOID CMNG_CODE_MANAGER_06::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	DisplayModifyData( -1 );
}

// ����
VOID CMNG_CODE_MANAGER_06::OnbNEXT() 
{
	// TODO: Add your control notification handler code here

	DisplayModifyData( 1 );	
}

VOID CMNG_CODE_MANAGER_06::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	switch( m_type )
	{
		case 1 :
			SetWindowText( _T("�ڵ� �Է�") );

			CButton *pBtn;
			pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
			pBtn->ShowWindow( false );
			pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
			pBtn->ShowWindow( false );

			break;
		case 2 :
			SetWindowText( _T("�ڵ� ����") );
			DisplayModifyData( 0 );
			break;
		default :
			break;
	}
	
}

VOID CMNG_CODE_MANAGER_06::DisplayModifyData(INT flag)
{
	switch( flag )	
	{
		case 0 :

			SelectMakeList( _T("CM_MNG_CODE_MANAGER_01"), _T("�׸���") );
			m_index = SelectGetHeadPosition( _T("CM_MNG_CODE_MANAGER_01"), _T("�׸���") );

			if( m_index < 0 )
			{
				AfxMessageBox( _T("���õ� �ڵ尡 �����ϴ�.") );
				PostMessage( WM_CLOSE );
				return;
			}

			//m_idxCnt = GetSelectedRowCount();

			break;

		case -1 :

			m_index = SelectGetPrev( _T("CM_MNG_CODE_MANAGER_01"), _T("�׸���") );

			if( -1 == m_index )
			{
				m_index = SelectGetHeadPosition( _T("CM_MNG_CODE_MANAGER_01"), _T("�׸���") );
				AfxMessageBox( _T(" �� ó�� �ڵ��Դϴ�.") );
				return;
			}

			break;

		case 1 :

			m_index = SelectGetNext( _T("CM_MNG_CODE_MANAGER_01"), _T("�׸���") );
			if( -1 == m_index )
			{
				m_index = SelectGetTailPosition( _T("CM_MNG_CODE_MANAGER_01"), _T("�׸���") );
				AfxMessageBox( _T("������ �ڵ��Դϴ�.") );
				return;
			}

			break;

		default :

			break;
	}

	AllControlDisplay( _T("CM_MNG_CODE_MANAGER_06"), m_index );
}

INT CMNG_CODE_MANAGER_06::GetCodeData(CString data[])
{
	CEdit *pEdt;

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->GetWindowText( data[ 0 ] );

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->GetWindowText( data[ 1 ] );

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT3 );
	pEdt->GetWindowText( data[ 2 ] );

	if( !data[ 0 ].GetLength() )
		return -1;
	
	return 0;
}
