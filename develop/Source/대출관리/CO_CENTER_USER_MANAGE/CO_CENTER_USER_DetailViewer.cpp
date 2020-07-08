// CO_CENTER_USER_DetailViewer.cpp : implementation file
//

#include "stdafx.h"
//#include _T("co_center_user_manage.h")
#include "CO_CENTER_USER_DetailViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_DetailViewer dialog


CCO_CENTER_USER_DetailViewer::CCO_CENTER_USER_DetailViewer( CESL_Mgr *pParent /*=NULL*/ )
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_DetailViewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CCO_CENTER_USER_DetailViewer::~CCO_CENTER_USER_DetailViewer()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}
BOOL CCO_CENTER_USER_DetailViewer::Create( CWnd *pParentWnd )
{	
	return CDialog::Create( IDD, pParentWnd ); 
}	

VOID CCO_CENTER_USER_DetailViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_DetailViewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_DetailViewer, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_DetailViewer)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bCENTER_USER_BEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bCENTER_USER_NEXT, OnbNEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_DetailViewer message handlers

VOID CCO_CENTER_USER_DetailViewer::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow )
		Display( 0 );
}

VOID CCO_CENTER_USER_DetailViewer::SetRowIndex( CArray< INT, INT >&rowIndex )
{
	for( INT i = 0; i < rowIndex.GetSize(); i++ )
		m_rowIndex.Add( rowIndex.GetAt( i ) );
}

VOID CCO_CENTER_USER_DetailViewer::Display( INT flag )
{
	m_grid.Clear();
	m_index += flag;
	
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );

	const INT cnt = 34;
	CString alias[ cnt ];
	GetAlias( alias );
	CString data[ cnt ];

	pDM->GetCellData( alias, cnt, data, m_rowIndex.GetAt( m_index ) );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("�ڷ�Ǳ���"), alias[ 31 ] );
	CString tmpTitle;
	for( INT i = 0; i < cnt; i++ )
		switch( i )
		{
			case  6 :
			case  8 :
			case 18 :
			case 31 :
			case 32 :
				continue;
				break;
			case  5 :
				m_grid.SetControlData( _T("�����ID"), data[ i ] );
				break;
			case 10 :
				m_grid.SetControlData( _T("���(+), ����(-)"), data[ i ] );
				break;
			case 11 :
				m_grid.SetControlData( alias[ i ], ConvertCivilNum( data[ i ] ) );
				break;
			case 12 :
			case 13 :
			case 14 :
			case 15 :
			case 16 :
			case 17 :
				tmpTitle = alias[ i ];
				tmpTitle.Replace( _T("_"), _T(" ") );
				m_grid.SetControlData( tmpTitle, data[ i ] );
				break;
			case 29 :
				m_grid.SetControlData( alias[ i ], _T("H") == data[ i ] ? _T("����") : _T("�ٹ���") );
				break;
			default :
				m_grid.SetControlData( alias[ i ], data[ i ] );
				break;
		}		
	m_grid.Display();

	SetMoveBtnEanble();

	CString sParentCMAlias = _T("CM_CO_CENTER_USER_MANAGER_01");
	CString sControlAlias = _T("�����̿��������׸���");	
	
	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) return ;
	
	CESL_Grid *pGrid = ( CESL_Grid * )( pCM->FindControl( sControlAlias ) );
	if( pGrid == NULL ) 
		return ;

	pGrid->SetReverse( m_rowIndex.GetAt( m_index ) );	
}

BOOL CCO_CENTER_USER_DetailViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	
	m_grid.InitViewer( IDC_gridCENTERUSER, this );
	m_grid.SetViewerWidth( 35 , 60 );
	
	m_index = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCO_CENTER_USER_DetailViewer::GetAlias( CString alias[] )
{
	alias[  0 ] = _T("�����������̸�");
	alias[  1 ] = _T("������");
	alias[  2 ] = _T("���������������̸�");
	alias[  3 ] = _T("����������");
	alias[  4 ] = _T("�����ڹ�ȣ");
	alias[  5 ] = _T("������ID");
	alias[  6 ] = _T("��й�ȣ");
	alias[  7 ] = _T("�̸�");
	alias[  8 ] = _T("�̸�����");
	alias[  9 ] = _T("�������");
	alias[ 10 ] = _T("������");
	alias[ 11 ] = _T("�ֹε�Ϲ�ȣ");
	alias[ 12 ] = _T("�ּ�_��");
	alias[ 13 ] = _T("�ּ�_�ٹ���");
	alias[ 14 ] = _T("�����ȣ_��");
	alias[ 15 ] = _T("�����ȣ_�ٹ���");
	alias[ 16 ] = _T("��ȭ��ȣ_��");
	alias[ 17 ] = _T("��ȭ��ȣ_�ٹ���");
	alias[ 18 ] = _T("������ȣ");
	alias[ 19 ] = _T("�ٹ����μ���");
	alias[ 20 ] = _T("�ٹ������޸�");
	alias[ 21 ] = _T("�ڵ���");
	alias[ 22 ] = _T("E-MAIL");
	alias[ 23 ] = _T("�����");
	alias[ 24 ] = _T("������");
	alias[ 25 ] = _T("�б�");
	alias[ 26 ] = _T("�г�");
	alias[ 27 ] = _T("��");
	alias[ 28 ] = _T("����");
	alias[ 29 ] = _T("����߼�����");
	alias[ 30 ] = _T("���");
	alias[ 31 ] = _T("�ڷ���ڵ�");
	alias[ 32 ] = _T("�̿��ڻ����̸�");
	alias[ 33 ] = _T("ȸ�����нǿ���");
}

VOID CCO_CENTER_USER_DetailViewer::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	Display( -1 );
}

VOID CCO_CENTER_USER_DetailViewer::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	Display( 1 );
}

VOID CCO_CENTER_USER_DetailViewer::SetMoveBtnEanble()
{
	CButton *pBtn = NULL;
	const INT cnt = 2;
	bool flag[ cnt ] = { 
		true, true };
	UINT id[ cnt ] = {
		IDC_bCENTER_USER_BEFORE, IDC_bCENTER_USER_NEXT };

	if( !m_index )
		flag[ 0 ] = false;

	if( m_index == m_rowIndex.GetSize() - 1 )
		flag[ 1 ] = false;

	for( INT i = 0; i < cnt; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( id[ i ] );
		pBtn->EnableWindow( flag[ i ] );
	}
}

CString CCO_CENTER_USER_DetailViewer::ConvertCivilNum( CString civilNum )
{
	// 2009.10.08 UPDATE BY KSJ : �ֹι�ȣ ǥ�� ��� ����
//DEL 	civilNum.Replace( _T("-"), _T("") );
//DEL 	if( 13 != civilNum.GetLength() )
//DEL 		return civilNum;
//DEL 
//DEL 	CString frontNum = civilNum.Left( 6 );
//DEL 	civilNum.Format( _T("%s-*******"), frontNum );
	CLocCommonAPI::ChangeSecuriyCivilNo( this , civilNum );
	
	return civilNum;
}

HBRUSH CCO_CENTER_USER_DetailViewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// ������� �߰�

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
