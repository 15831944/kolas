// MNG_KOMARC_MANAGER_02.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_KOMARC_MANAGER_02.h"
#include "MNG_KOMARC_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_KOMARC_MANAGER_02 dialog


CMNG_KOMARC_MANAGER_02::CMNG_KOMARC_MANAGER_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_KOMARC_MANAGER_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_KOMARC_MANAGER_02::~CMNG_KOMARC_MANAGER_02()
{
	
}
BOOL CMNG_KOMARC_MANAGER_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_KOMARC_MANAGER_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_KOMARC_MANAGER_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_KOMARC_MANAGER_02, CDialog)
	//{{AFX_MSG_MAP(CMNG_KOMARC_MANAGER_02)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)	
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_KOMARC_MANAGER_02 message handlers

BOOL CMNG_KOMARC_MANAGER_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	InitSetControls();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_KOMARC_MANAGER_02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	

	SetEditProperty();
	CleanCondition();
}

VOID CMNG_KOMARC_MANAGER_02::InitSetControls()
{
	const INT listCnt = 8;
	CString comboList[ listCnt ] = {
		_T("���౹��ȣ"), _T("�ѱ����к�ȣ"), _T("���к�ȣ"), _T("�ѱ����α����ȣ"), _T("�ѱ��������к�ȣ"), _T("�ܱ��������к�ȣ"), _T("������ȣ"), _T("�ѱ���������ȣ") };
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	for( INT i = 0; i < listCnt; i++ )
		pCombo->InsertString( i, comboList[ i ] );
	pCombo->SetCurSel( 0 );
}

// �˻�
VOID CMNG_KOMARC_MANAGER_02::OnOK() 
{
	// TODO: Add extra validation here
	
	CWaitCursor mouse;
	
	const INT conditionCnt = 5;
	CString condition[ conditionCnt ];
	GetCondition( condition );
	
	CMNG_KOMARC_MANAGER_01 *pParent = ( CMNG_KOMARC_MANAGER_01 * )pParentMgr;
	if( pParent->FindData( condition ) )
	{
		AfxMessageBox( _T("Error : CMNG_KOMARC_MANAGER_01::FindData( CString codition[] )") );
		return;
	}
	
	mouse.Restore();

	// CDialog::OnOK();
}

INT CMNG_KOMARC_MANAGER_02::GetCondition( CString condition[] )
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	condition[ 0 ].Format( _T("%d"), pCombo->GetCurSel() );

	CEdit *pEdt;
	const INT edtCnt = 4;
	UINT edtID[ edtCnt ] = { IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4 };
	for( INT i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->GetWindowText( condition[ i + 1 ] );
	}
	
	return 0;
}

// ���౹��ȣ		0
// �ѱ����к�ȣ		1
// ���к�ȣ		2
// �ѱ����α����ȣ	3
// �ѱ��������к�ȣ	4
// �ܱ��������к�ȣ	5
// ������ȣ			6
// �ѱ���������ȣ	7

VOID CMNG_KOMARC_MANAGER_02::SetEditProperty()
{
	const INT edtCnt = 2;
	bool enableOption[ edtCnt ] = { false, false };

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	switch( pCombo->GetCurSel() )
	{
		case 1 :
		case 3 :
		case 7 :
			enableOption[ 1 ] = true;
			break;
		case 4 :
			enableOption[ 0 ] = true;
			break;
		default :
			break;
	}

	CEdit *pEdt;
	UINT edtID[ edtCnt ] = { IDC_EDIT2, IDC_EDIT4 };

	for( INT i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->EnableWindow( enableOption[ i ] );
	}
}

VOID CMNG_KOMARC_MANAGER_02::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here

	SetEditProperty();
}

VOID CMNG_KOMARC_MANAGER_02::CleanCondition()
{
	CEdit *pEdt;
	const INT edtCnt = 4;
	UINT edtID[ edtCnt ] = {
		IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4 };
	for( INT i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->SetWindowText( _T("") );
	}
}

HBRUSH CMNG_KOMARC_MANAGER_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
