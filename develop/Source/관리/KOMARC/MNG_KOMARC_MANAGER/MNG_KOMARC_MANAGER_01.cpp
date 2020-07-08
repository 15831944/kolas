// MNG_KOMARC_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_KOMARC_MANAGER_01.h"

#include "..\KOMarcGetter\KOMarcGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_KOMARC_MANAGER_01 dialog


CMNG_KOMARC_MANAGER_01::CMNG_KOMARC_MANAGER_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_KOMARC_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_KOMARC_MANAGER_01::~CMNG_KOMARC_MANAGER_01()
{
	
}

BOOL CMNG_KOMARC_MANAGER_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_KOMARC_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_KOMARC_MANAGER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMNG_KOMARC_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_KOMARC_MANAGER_01)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_KOMarcGetter, OnKOMarcGetter)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_KOMARC_MANAGER_01 message handlers

BOOL CMNG_KOMARC_MANAGER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_KOMARC_MANAGER_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CODE_MANAGER_01") );
		return false;
	}
		
	CArray< CString, CString >RemoveListArray;
	RemoveListArray.RemoveAll();
	
	CESL_Grid *pGrid;

	const INT gridCnt = 8;
	CString gridAlias[ gridCnt ] = {
		_T("�׸���_0"), _T("�׸���_1"), _T("�׸���_2"), _T("�׸���_3"), _T("�׸���_4"), _T("�׸���_5"), _T("�׸���_6"), _T("�׸���_7") };

	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid =  ( CESL_Grid * )FindControl( _T("CM_MNG_KOMARC_MANAGER_01"), gridAlias[ i ] );
		
		if( pGrid )
		{
			//RemoveListArray.Add( _T("") );

			pGrid->InitPopupMenuItem();
			pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &RemoveListArray );
		}
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_KOMARC_MANAGER_01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	
}

VOID CMNG_KOMARC_MANAGER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	const INT gridCnt = 8;
	UINT gridID[ gridCnt ] = {
		IDC_mngGRID_1, IDC_mngGRID_2, IDC_mngGRID_3, IDC_mngGRID_4, IDC_mngGRID_5, IDC_mngGRID_6, IDC_mngGRID_7, IDC_mngGRID_8 };

	CMSHFlexGrid *pGrid;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ i ] );
		if( NULL == pGrid->GetSafeHwnd() )
			continue;
		
		//pGrid->MoveWindow( 15, 30, cx - 25, cy - 40 );
		pGrid->MoveWindow(-2, 35, cx+2, cy);
	}
}

INT CMNG_KOMARC_MANAGER_01::FindData( CString condition[] )
{
	m_type = _ttoi( condition[ 0 ] );
	
	CESL_DataMgr *pDM;
	CString dmAlias[ 8 ] = {
		_T("DM_MNG_KOMARC_MANAGER_01_01"), _T("DM_MNG_KOMARC_MANAGER_01_02"), _T("DM_MNG_KOMARC_MANAGER_01_03"),
		_T("DM_MNG_KOMARC_MANAGER_01_04"), _T("DM_MNG_KOMARC_MANAGER_01_05"), _T("DM_MNG_KOMARC_MANAGER_01_06"),
		_T("DM_MNG_KOMARC_MANAGER_01_07"), _T("DM_MNG_KOMARC_MANAGER_01_08") };
	pDM = FindDM( dmAlias[ m_type ] );

	pDM->RefreshDataManager( MakeSelectQuery( condition ) );
	if( !pDM->GetRowCount() )
		AfxMessageBox( _T("�˻������ �����ϴ�.") );
	
	ShowGrid();
	
	return 0;
}


CString CMNG_KOMARC_MANAGER_01::MakeSelectQuery( CString condition[] )
{
	CString where, tmp;
	
	const INT fieldCnt = 4;
	CString fieldName[ fieldCnt ] = {
		_T("CLASS_CODE"), _T("NUMBER_CODE"), _T("INDEX_DESCRIPTION"), _T("MANAGE_CLASS") };

	condition[ 2 ].Replace( _T(" "), _T("") );
	for( INT i = 0; i < fieldCnt; i++ )
	{
		AllTrim( condition[ i + 1 ] );
		
		if( condition[ i + 1 ].GetLength() )
		{
			if( i )
				tmp.Format( _T(" %s LIKE '%%%s%%' AND"), fieldName[ i ], condition[ i + 1 ] );
			else
				tmp.Format( _T(" %s LIKE '%s%%' AND"), fieldName[ i ], condition[ i + 1 ] );
			
			where += tmp;
		}
	}

	if( _T("AND") == where.Right( 3 ) )
		tmp = where.Left( where.GetLength() - 3 );

	return tmp;
}

VOID CMNG_KOMARC_MANAGER_01::AllTrim( CString &data )
{
	data.TrimLeft();
	data.TrimRight();
}

VOID CMNG_KOMARC_MANAGER_01::ShowGrid( INT option )
{
	if( -1 == option )
		option = m_type;
	else
		m_type = option;


	const INT gridCnt = 8;
	UINT gridID[ gridCnt ] = {
		IDC_mngGRID_1, IDC_mngGRID_2, IDC_mngGRID_3, IDC_mngGRID_4, IDC_mngGRID_5, IDC_mngGRID_6, IDC_mngGRID_7, IDC_mngGRID_8 };
	bool show[ gridCnt ] = { false, false, false, false, false, false, false, false };
	show[ option ] = true;

	CMSHFlexGrid *pGrid;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ i ] );
		pGrid->ShowWindow( show[ i ] );
	}
	
	CString gridAlias;
	gridAlias.Format( _T("�׸���_%d"), option );
	ControlDisplay( _T("CM_MNG_KOMARC_MANAGER_01"), gridAlias );

	ShowCurrentCodeName();
}

INT * CMNG_KOMARC_MANAGER_01::GetCheckedInfo( INT &cnt, INT option )
{
	if( -1 == option )
		option = m_type;
	
	cnt = 0;

	const INT gridCnt = 8;
	UINT gridID[ gridCnt ] = {
		IDC_mngGRID_1, IDC_mngGRID_2, IDC_mngGRID_3, IDC_mngGRID_4, IDC_mngGRID_5, IDC_mngGRID_6, IDC_mngGRID_7, IDC_mngGRID_8 };
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ option ] );

	INT curRow = pGrid->GetRow();
	INT curCol = pGrid->GetCol();

	INT rowCnt = pGrid->GetRows();
	pGrid->SetCol( 1 );
	for( INT i = 1; i < rowCnt; i++ )
	{
		pGrid->SetRow( i );
		if( _T("V") == pGrid->GetText() )
			cnt++;
	}

	INT *checkedDMRow;
	checkedDMRow = ( INT * )calloc( cnt, sizeof( INT ) );

	INT idx = 0;
	for( i = 1; i < rowCnt; i++ )
	{
		pGrid->SetRow( i );
		if( _T("V") == pGrid->GetText() )
			checkedDMRow[ idx++ ] = i - 1;
	}

	return checkedDMRow;
}

VOID CMNG_KOMARC_MANAGER_01::DeleteData()
{
	INT checkedCnt;
	INT *checkedDMRow = GetCheckedInfo( checkedCnt );

	if( !checkedCnt )
	{
		AfxMessageBox( _T("���õ� ����� �����ϴ�.") );
		return;
	}
	
	CString notice;
	notice.Format( _T("���õ� %d���� �ڷḦ �����Ͻðڽ��ϱ�?"), checkedCnt );
	if( IDNO == AfxMessageBox( notice, MB_YESNO ) )
	{
		free( checkedDMRow );
		return;
	}
	
	CESL_DataMgr *pDM;
	CString dmAlias[ 8 ] = {
		_T("DM_MNG_KOMARC_MANAGER_01_01"), _T("DM_MNG_KOMARC_MANAGER_01_02"), _T("DM_MNG_KOMARC_MANAGER_01_03"),
		_T("DM_MNG_KOMARC_MANAGER_01_04"), _T("DM_MNG_KOMARC_MANAGER_01_05"), _T("DM_MNG_KOMARC_MANAGER_01_06"),
		_T("DM_MNG_KOMARC_MANAGER_01_07"), _T("DM_MNG_KOMARC_MANAGER_01_08") };
	pDM = FindDM( dmAlias[ m_type ] );

	CString query, pk, table;
	table = GetCurrentTable();
	for( INT i = checkedCnt - 1; i > -1; i-- )
	{
		pDM->GetCellData( _T("REC_KEY"), checkedDMRow[ i ], pk );
		query.Format( _T("DELETE FROM %s WHERE REC_KEY = %s;"), table, pk );
		pDM->DeleteRow( checkedDMRow[ i ] );
		pDM->ExecuteQuery( query, 1 );
	}

	ShowGrid();
	AfxMessageBox( _T("�����Ǿ����ϴ�.") );
	free( checkedDMRow );	
}

CString CMNG_KOMARC_MANAGER_01::GetCurrentTable()
{
	CString table[ 8 ] = { 
		_T("CO_PUBLISH_COUNTRY_CODE_TBL"), 
		_T("CO_KOR_UNIVERSITY_CODE_TBL"), 
		_T("CO_LANG_CODE_TBL"),
		_T("CO_GOVERNMENT_CODE_TBL"),
		_T("CO_KOR_REGION_CODE_TBL"),
		_T("CO_FOREIGN_REGION_CODE_TBL"),
		_T("CO_COUNTRY_CODE_TBL"),
		_T("CO_LIBRARY_CODE_ADMN_TBL") };

	return table[ m_type ];
}

VOID CMNG_KOMARC_MANAGER_01::ShowCurrentCodeName()
{
	CButton *pBtn = ( CButton * )GetDlgItem( IDC_CODE_NAME );

	const INT listCnt = 8;
	CString comboList[ listCnt ] = {
		_T("���౹��ȣ"), _T("�ѱ����к�ȣ"), _T("���к�ȣ"), _T("�ѱ����α����ȣ"), _T("�ѱ��������к�ȣ"), _T("�ܱ��������к�ȣ"), _T("������ȣ"), _T("�ѱ���������ȣ") };

	pBtn->SetWindowText( comboList[ m_type ] );
}

VOID CMNG_KOMARC_MANAGER_01::OnKOMarcGetter() 
{
	// TODO: Add your control notification handler code here
	
	CKOMarcGetterDlg dlg( this );
	dlg.InitSetProperty( 4, _T("") );
	
	if( IDOK == dlg.DoModal() )
		AfxMessageBox( dlg.m_code );
}

// ���౹��ȣ		0
// �ѱ����к�ȣ		1
// ���к�ȣ		2
// �ѱ����α����ȣ	3
// �ѱ��������к�ȣ	4
// �ܱ��������к�ȣ	5
// ������ȣ			6
// �ѱ���������ȣ	7

HBRUSH CMNG_KOMARC_MANAGER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
