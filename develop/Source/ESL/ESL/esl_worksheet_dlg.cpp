// ESL_WorkSheet_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ESL_WorkSheet_dlg.h"

#include "ESL_DataMgr.h"

#include "ESL_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ESL_Mgr.h"

/////////////////////////////////////////////////////////////////////////////
// CESL_WorkSheet_dlg dialog

BEGIN_EVENTSINK_MAP(CESL_WorkSheet_dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CESL_WorkSheet_dlg)
	ON_EVENT(CESL_WorkSheet_dlg, IDC_MSHFLEXGRID_2, -605 /* MouseDown */, OnMouseDownMshflexgrid2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CESL_WorkSheet_dlg, IDC_MSHFLEXGRID_1, 72 /* LeaveCell */, OnLeaveCellMshflexgrid1, VTS_NONE)
	ON_EVENT(CESL_WorkSheet_dlg, IDC_MSHFLEXGRID_1, 71 /* EnterCell */, OnEnterCellMshflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

CESL_WorkSheet_dlg::CESL_WorkSheet_dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CESL_WorkSheet_dlg::IDD, pParent)
{
	m_wnd = pParent;
	//{{AFX_DATA_INIT(CESL_WorkSheet_dlg)
	//}}AFX_DATA_INIT

	// �⺻ ũ��
	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = 500;
	m_rect.bottom = 500;

	m_cell_ratio_grid_1_1 = 40; // 60% (GRID1�� ALias�÷��� ����)
}

CESL_WorkSheet_dlg::~CESL_WorkSheet_dlg()
{
	if( m_ctrlInfo != NULL )
	{
		delete [] m_ctrlInfo;
		m_ctrlInfo = NULL;
	}

	if( NULL != m_multiComboBox )
	{
		delete m_multiComboBox;
		m_multiComboBox = NULL;
	}

}

VOID CESL_WorkSheet_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESL_WorkSheet_dlg)
	DDX_Control(pDX, IDOK_2, m_ok_2);
	DDX_Control(pDX, IDC_TAB_COM, m_tab);
	DDX_Control(pDX, IDC_MSHFLEXGRID_1, m_grid_1);
	DDX_Control(pDX, IDC_MSHFLEXGRID_2, m_grid_2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CESL_WorkSheet_dlg, CDialog)
	//{{AFX_MSG_MAP(CESL_WorkSheet_dlg)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_COM, OnSelchangeTab)
	ON_BN_CLICKED(IDOK_2, On2)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_MUUTICOMBO_BOX, OnSelchangeMuuticomboBox)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESL_WorkSheet_dlg message handlers

	/*
		< m_pInfo >
		
		// DB ���� ����
		CString CONNECTION_INFO;

		// User ���� ����
		CString USER_PK;
		CString USER_ID;
		CString USER_NAME;
		CString USER_PRIVILEGE;

		// Code Mgr ���� ����
		CESL_CodeMgr *pCodeMgr;

		CESL_DataMgr *pESLMgr;
		CESL_DataMgr *pESLDataMgr;
		CESL_DataMgr *pESLControlMgr;
			SM : 
				pk
				esl_mgr_alias
				engineer
				mgr_pk
				mgr_type

			DM : 
				pk
				db_mgr_alias
				connection_info
				tbl_name
				condition
				extra_condition
				field_alias
				field_name
				field_type
				init_value
				data_type
				
			CM : 
				0	pk
				1	ctrl_mgr_alias
				2	ctrl_alias
				3	ctrl_type
				4	ctrl_id
				5	ctrl_style
				6	init_value
				7	list_data
				8	link_data
				9	validation
				10	requirecheck
	*/

BOOL CESL_WorkSheet_dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_editBox = NULL;
	m_dateBox = NULL;
	m_ctrlInfo = NULL;
	m_multiComboBox = NULL;
	m_change = true;

	CComboBox *comboBox = ( CComboBox * )GetDlgItem( IDC_MUUTICOMBO_BOX );
	comboBox->ShowWindow( false );

	CESL_Mgr *pMgr = (CESL_Mgr*)m_wnd;
	m_pCM = pMgr->FindCM(m_cmAlias);
	if (m_pCM == NULL) {
		;
	}

	SetTimer( 1905, 100, NULL );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// �� ������ �̸� ����
VOID CESL_WorkSheet_dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	m_tab.InsertItem( 0, _T("�� ��") );
	m_tab.InsertItem( 1, _T("�� ��") );	

	ViewTabControl( 0 );

	UpdateData( true );
	CRect rect;

	m_grid_1.GetWindowRect( &rect );
	m_grid_1.SetRowHeight( 0, 300 );	
	m_grid_1.SetColWidth( 0, 0, 500 );
	m_grid_1.SetColWidth( 1, 0, ( INT )( rect.Width() * ( ( double )m_cell_ratio_grid_1_1 / 100.0 ) * 15 ) - 900 );
	m_grid_1.SetColWidth( 2, 0, ( INT )( rect.Width() * ( ( double )( 100 - m_cell_ratio_grid_1_1 ) / 100.0 ) ) * 15 );
	m_grid_1.SetColAlignmentBand( 1, 0, 4 );
	m_grid_1.SetColAlignmentBand( 0, 2, 4 );
	m_grid_1.SetRow( 1 );
	m_grid_1.SetCol( 2 );
	
	m_grid_2.GetWindowRect(&rect);
	m_grid_2.SetColWidth( 0, 0, 500 );
	m_grid_2.SetColWidth( 1, 0, 1000 );
	m_grid_2.SetColWidth( 2, 0, ( rect.Width() * 15 ) - 500 - 1000 - 400 );
	m_grid_2.SetColAlignmentBand( 0, 0, 4 );
	m_grid_2.SetColAlignmentBand( 0, 1, 4 );
	m_grid_2.SetRow( 1 );
	m_grid_2.SetCol( 1 );
}

// WorkSheet �Է� type�� ���´�. 
INT CESL_WorkSheet_dlg::GetWorkSheetType()
{
	CButton *r1 = ( CButton * )GetDlgItem( IDC_RADIO1_COM );
	CButton *r2 = ( CButton * )GetDlgItem( IDC_RADIO2_COM );
	CButton *r3 = ( CButton * )GetDlgItem( IDC_RADIO3_COM );
	CButton *r4 = ( CButton * )GetDlgItem( IDC_RADIO4_COM );
	
	if( r1->GetCheck() )
		return 1;

	if( r2->GetCheck() )
		return 2;

	if( r3->GetCheck() )
		return 3;

	if( r4->GetCheck() )
		return 4;

	return 0;	
}

VOID CESL_WorkSheet_dlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	
	ViewTabControl( m_tab.GetCurSel() );
}

// ù��° �Ǵ� �ι�° ���� �����ش�. 
VOID CESL_WorkSheet_dlg::ViewTabControl( INT flag )
{
	CButton *r1 = ( CButton * )GetDlgItem( IDC_RADIO1_COM );
	CButton *r2 = ( CButton * )GetDlgItem( IDC_RADIO2_COM );
	CButton *r3 = ( CButton * )GetDlgItem( IDC_RADIO3_COM );
	CButton *r4 = ( CButton * )GetDlgItem( IDC_RADIO4_COM );
	
	switch( flag )
	{
		case 0 : // �Է�
			
			SetFirstTabControl();
			SetControls();

			m_grid_1.ShowWindow( true );

			m_grid_2.ShowWindow( false );
			m_ok_2.ShowWindow( false );
			r1->ShowWindow( false );
			r2->ShowWindow( false );
			r3->ShowWindow( false );
			r4->ShowWindow( false );

			break;

		case 1 : // ����
			
			MoveGridCell( 1 );
			SetSecondTabControl();

			m_grid_2.ShowWindow( true );
			m_ok_2.ShowWindow( true );
			r1->ShowWindow( true );
			r2->ShowWindow( true );
			r3->ShowWindow( true );
			r4->ShowWindow( true );
			
			m_grid_1.ShowWindow( false );

			OnLeaveCellMshflexgrid1();
			
			break;
	}
}

// �Է�ȭ���ϰ�� �����Է°��� �׸��忡 �����Ѵ�.
VOID CESL_WorkSheet_dlg::SetControls()
{
	for( INT i = 0; i < m_controlCnt; i++ )
		if( _T("M") != m_ctrlInfo[ i ].ctrl_type )
			SetGrid1String( i + 1, 2, m_ctrlInfo[ i ].value );
		else
			SetGrid1String( i + 1, 2, m_ctrlInfo[ i ].desc  );
}

// ��ü control�� ������ �޾ƿ´�. 
INT CESL_WorkSheet_dlg::GetAllControlCount()
{
	return m_pCM->Control_List.GetCount();
}

// ��ü control�� alias�� �޾ƿ´�. 
INT CESL_WorkSheet_dlg::GetAllControlAlias( CString *alias )
{
	INT index = 0;
	INT cecount = m_pCM->Control_List.GetCount();
	CESL_Control_Element * pCE = NULL;
	POSITION pos;
	pos = m_pCM->Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = NULL;
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		if (pCE) {
			alias[index++] = pCE->CTRL_ALIAS;
		}
	}
	return 0;
}

// �ι�° ��Ʈ���� �����Ѵ�.
INT CESL_WorkSheet_dlg::SetSecondTabControl()
{
	SetRadioButton( m_workSheetType );

	INT rows; 
	CString *allControlAlias;
	allControlAlias = GetAllControlInfo( rows );	
	
	SetGrid_2( rows, allControlAlias );

	CheckViewControlList();
	
	delete [] allControlAlias;

	return 0;
}

// �ι�° ���� ��������, ������ ������ ������ control���� üũ�Ѵ�. 
INT CESL_WorkSheet_dlg::CheckViewControlList()
{
	INT count;
	CString *alias;
	alias = GetViewControInfo( count );

	for( INT i = 1; i < m_grid_2.GetRows(); i++ )
		SetGrid2String( i, 1, _T("V") );
	
	CString tmp;
	for( i = 1; i < m_grid_2.GetRows(); i++ )
		for( INT j = 0; j < count; j++ )
			if( alias[ j ] == GetGrid2String( i, 2 ) && alias[ j ])
				SetGrid2String( i, 1, _T("") );
			
	delete [] alias;
	
	return 0;
}

// �׸��忡�� �Է��� control�׸� list������ �޾ƿ´�. 
CString * CESL_WorkSheet_dlg::GetViewControInfo( INT &count )
{
	CString query;
	query.Format( _T("SELECT ALIAS FROM ESL_WORKSHEET_SUB_TBL WHERE CM_KEY = '%s'"), GetCMKey() );

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
	
	dm.RestructDataManager( query );

	count = dm.GetRowCount();
	CString *alias = new CString[ count ];

	for( INT i = 0; i < count; i++ )
		dm.GetCellData( i, 0, alias[ i ] );
	
	return alias;
}

// ���� �׸����� ����� �����Ѵ�. 
INT CESL_WorkSheet_dlg::SetGrid_2(INT rows, CString *alias)
{
	m_grid_2.SetRows( rows + 1 );
	m_grid_2.SetCols( 0, 3 );

	SetGrid2String( 0, 1, _T("����") );
	SetGrid2String( 0, 2, _T("Alias") );

	CString row;
	for( INT i = 0; i< rows; i++ )
	{
		row.Format( _T("%d"), i + 1 );
		SetGrid2String( i + 1, 0, row );
		SetGrid2String( i + 1, 2, alias[ i ] );
	}
	
	return 0;
}

// ���� �׸��尡 Ŭ��������,
VOID CESL_WorkSheet_dlg::OnMouseDownMshflexgrid2( short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	
	if( 1 != m_grid_2.GetCol() || 0 == m_grid_2.GetRow() )
		return;
	
	CString check;
	check = m_grid_2.GetText();

	if( check == _T("V") )
		m_grid_2.SetText( _T("") );
	else
		m_grid_2.SetText( _T("V") );
}

// ���� �׸����� ( x, y )�� ���ڿ��� �ִ´�. 
VOID CESL_WorkSheet_dlg::SetGrid2String( INT row, INT col, CString string )
{
	m_grid_2.SetRow( row );
	m_grid_2.SetCol( col );
	m_grid_2.SetText( string );
}

// �Է� �׸����� ( x, y )�� ���ڿ��� �ִ´�. 
VOID CESL_WorkSheet_dlg::SetGrid1String( INT row, INT col, CString string )
{
	m_grid_1.SetRow( row );
	m_grid_1.SetCol( col );
	m_grid_1.SetText( string );	
}

// ���� �׸����� ( x, y )���� ���ڿ��� �޾ƿ´�.
CString CESL_WorkSheet_dlg::GetGrid2String(INT row, INT col)
{
	m_grid_2.SetRow( row );
	m_grid_2.SetCol( col );

	return m_grid_2.GetText();
}

// �Է� �׸����� ( x, y )���� ���ڿ��� �޾ƿ´�.
CString CESL_WorkSheet_dlg::GetGrid1String(INT row, INT col)
{
	m_grid_1.SetRow( row );
	m_grid_1.SetCol( col );

	return m_grid_2.GetText();
}

// ��ü control�� ������ alias�� ���´�.
CString * CESL_WorkSheet_dlg::GetAllControlInfo( INT &count )
{
	count = GetAllControlCount();
	CString *alias;
	alias = new CString[ count ];

	GetAllControlAlias( alias );

	return alias;
}

// Radio Button üũ�ϱ�.
VOID CESL_WorkSheet_dlg::SetRadioButton( INT flag )
{
	CButton *r1 = ( CButton * )GetDlgItem( IDC_RADIO1_COM );
	CButton *r2 = ( CButton * )GetDlgItem( IDC_RADIO2_COM );
	CButton *r3 = ( CButton * )GetDlgItem( IDC_RADIO3_COM );
	CButton *r4 = ( CButton * )GetDlgItem( IDC_RADIO4_COM );

	switch( flag )
	{
		case 1 :
			r1->SetCheck( true );
			break;
		case 2 :
			r2->SetCheck( true );
			break;
		case 3 :
			r3->SetCheck( true );
			break;
		case 4 :
			r4->SetCheck( true );
			break;
		default :
			break;
	}
}

// ���� -> Ȯ��
VOID CESL_WorkSheet_dlg::On2() 
{
	// TODO: Add your control notification handler code here
		
	INT uncheckedRowsCount = 0;
	CString *uncheckedRowAlias = GetUncheckedRowInfo( uncheckedRowsCount );

	DeleteBeforeConfigureData();
	SaveConfigureData( uncheckedRowAlias, uncheckedRowsCount );
	
	delete [] uncheckedRowAlias;

	MessageBox( _T("��������� ����Ǿ����ϴ�.") , _T("Warnning!"), MB_ICONEXCLAMATION);
	
	m_change = true;
	if( m_ctrlInfo != NULL )
	{
		delete [] m_ctrlInfo;
		m_ctrlInfo = NULL;
	}

}

// ���� ���� ������ ��� �����. 
INT CESL_WorkSheet_dlg::DeleteBeforeConfigureData()
{
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	CString query;
	CString cmKey = GetCMKey();

	if( cmKey.GetLength() > 0 )
	{
		query.Format( _T("DELETE FROM ESL_WORKSHEET_TBL WHERE PK = %s;"), cmKey );
		dm.ExecuteQuery( query, 1);

		query.Format( _T("DELETE FROM ESL_WORKSHEET_SUB_TBL WHERE CM_KEY = %s;"), cmKey );
		dm.ExecuteQuery( query, 1);
	}
	
	return 0;
}

// ESL_WORKSHEET_TBL�� CM_KEY�� ���´�. 
CString CESL_WorkSheet_dlg::GetCMKey()
{
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	CString query, cmKey;

	query.Format( _T("SELECT PK FROM ESL_WORKSHEET_TBL WHERE CM_ALIAS = '%s'"), m_cmAlias );
	dm.GetOneValueQuery( query, cmKey );

	return cmKey;
}

// ������ ������ DB�� �����Ѵ�. 
INT CESL_WorkSheet_dlg::SaveConfigureData( CString *alias, INT count )
{
	/*
	CREATE TABLE ESL_WORKSHEET_TBL
	(
		PK			INTEGER	NOT NULL,
		TYPE		INTEGER,
		K2U_USER	VARCHAR( 255 ),
		CM_ALIAS	VARCHAR( 255 )
	);
	*/

	CString pk = GetPK();
	
	const INT fieldCount = 4;
	
	CString fieldName[ fieldCount ] = {
		_T("PK"), _T("TYPE"), _T("K2U_USER"), _T("CM_ALIAS") };
	
	CString fieldType[ fieldCount ] = {
		_T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING") };
	
	CString fieldData[ fieldCount ];
	INT workSheetType = GetWorkSheetType();
	fieldData[ 0 ].Format( _T("%s"), pk );
	fieldData[ 1 ].Format( _T("%d"), workSheetType );
	fieldData[ 2 ].Format( _T("%s"), m_pInfo->USER_ID );
	fieldData[ 3 ].Format( _T("%s"), m_cmAlias );

	CESL_Mgr *pEslMgr = ( CESL_Mgr * )m_wnd;
	pEslMgr->m_workSheetType = workSheetType;
	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	dm.StartFrame();
	dm.InitDBFieldData();

	for( INT i = 0; i < fieldCount; i++ )
		dm.AddDBFieldData( fieldName[ i ], fieldType[ i ], fieldData[ i ] );
	dm.MakeInsertFrame( _T("ESL_WORKSHEET_TBL") );

	if( dm.SendFrame() < 0 )
	{
		MessageBox( _T("Error : CESL_WorkSheet_dlg::SaveConfigureData( CString *alias, INT count )") , _T("Warnning!"), MB_ICONEXCLAMATION);
		return -1;
	}

	dm.EndFrame();

	if( SaveConfigureSubData( pk, alias, count ) )
	{
		MessageBox( _T("Error : CESL_WorkSheet_dlg::SaveConfigureSubData(CString pk, CString *alias, INT count)") , _T("Warnning!"), MB_ICONEXCLAMATION);
		return -2;
	}
	
	return 0;
}

INT CESL_WorkSheet_dlg::SaveConfigureSubData( CString pk, CString *alias, INT count )
{
	/*
	CREATE TABLE ESL_WORKSHEET_SUB_TBL
	(
		PK			INTEGER NOT NULL,
		CM_KEY		INTEGER,
		ALIAS		VARCHAR( 255 )
	);
	*/

	const INT fieldCount = 3;

	CString fieldName[ fieldCount ] = {
		_T("PK"), _T("CM_KEY"), _T("ALIAS") };

	CString fieldType[ fieldCount ] = {
		_T("NUMERIC"), _T("NUMERIC"), _T("STRING") };

	CString fieldData[ fieldCount ];

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	for( INT i = 0; i < count; i++ )
	{
		dm.StartFrame();
		dm.InitDBFieldData();

		fieldData[ 0 ].Format( _T("%s"), GetPK() );
		fieldData[ 1 ].Format( _T("%s"), pk );
		fieldData[ 2 ].Format( _T("%s"), alias[ i ] );		
		
		for( INT j = 0; j < fieldCount; j++ )
			dm.AddDBFieldData( fieldName[ j ], fieldType[ j ], fieldData[ j ] );
		dm.MakeInsertFrame( _T("ESL_WORKSHEET_SUB_TBL") );

		if( dm.SendFrame() < 0 )
			return -1;
		dm.EndFrame();
	}

	return 0;
}

// Primary key�� ���´�. 
CString CESL_WorkSheet_dlg::GetPK()
{
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	CString pk;
	dm.MakeRecKey( pk );

	return pk;
}

// ���� �׸��忡�� üũ���� ���� row�� ������ �� ��ȣ�� �޾ƿ´�. 
CString * CESL_WorkSheet_dlg::GetUncheckedRowInfo( INT &rowCount )
{
	CString *alias;
	rowCount = GetUncheckedRowCount();
	alias = new CString[ rowCount ];
	
	INT index = 0;
	for( INT i = 1; i < m_grid_2.GetRows(); i++ )
		if( _T("V") != GetGrid2String( i, 1 ) )
			alias[ index++ ].Format( _T("%s"), GetGrid2String( i, 2 ) );
	
	return alias;
}

// ���� �׸��忡�� üũ���� ���� ���� ������ �޾ƿ´�. 
INT CESL_WorkSheet_dlg::GetUncheckedRowCount()
{
	INT count = 0;
	
	for( INT i = 1; i < m_grid_2.GetRows(); i++ )
		if( _T("V") != GetGrid2String( i, 1 ) )
			count++;

	return count;
}

// ù��° ��Ʈ���� �����Ѵ�.
INT CESL_WorkSheet_dlg::SetFirstTabControl()
{
	if( false == m_change )
		return 0;

	INT count;
	CString *alias;
	alias = GetViewControlInfo( count );
	m_controlCnt = count;

	if( NULL != m_ctrlInfo )
	{
		delete m_ctrlInfo;
		m_ctrlInfo = NULL;
	}
	m_ctrlInfo = new ControlInfo[ count ];
			
	m_change = false;
	
	GetInputControlInfo( alias, count );
	GetControlValue();
	SetInputGridCaption( alias, count );
	
	delete [] alias;

	return 0;
}

// �׸����� ���õ� cell�� rect�� ���Ѵ�. 
CRect CESL_WorkSheet_dlg::GetGridCellRect()
{
	CWnd *wnd = ( CWnd * )GetDlgItem( IDC_MSHFLEXGRID_1 );

	CRect rect, gridRect;
	RECT parentRect;
	wnd->GetWindowRect( &gridRect );
	wnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( wnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );
	
	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left	= gridRect.left +	( long )( ( float )m_grid_1.GetCellLeft()	/ 15.0f );
	rect.top	= gridRect.top +	( long )( ( float )m_grid_1.GetCellTop()	/ 15.0f );
	rect.right	= rect.left +		( long )( ( float )m_grid_1.GetCellWidth()	/ 15.0f );
	rect.bottom = rect.top +		( long )( ( float )m_grid_1.GetCellHeight() / 15.0f );

	return rect;
}

// row�࿡ EditBox�� �����Ѵ�.
INT CESL_WorkSheet_dlg::CreateEditBox( INT row )
{
	if( NULL != m_editBox )
	{
		delete m_editBox;
		m_editBox = NULL;
	}
	m_editBox = new CESL_GridSelEditor;
		
	CWnd *wnd = ( CWnd * )GetDlgItem( IDC_MSHFLEXGRID_1 );
	
	m_editBox->row = row + 2;
	m_editBox->col = 2;
	m_editBox->parent = GetDlgItem( IDC_MSHFLEXGRID_1 );
	m_editBox->Create( ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_CHILD, CRect( 0, 0, 10, 20 ), wnd->GetParent(), 645 );

	CRect rect = GetGridCellRect();
	m_editBox->MoveWindow( rect.left - 1, rect.top - 1, rect.Width(), rect.Height() );

	m_editBox->ShowWindow( true );
	m_editBox->SetFocus();

	m_editBox->SetWindowText( m_ctrlInfo[ row - 1 ].value );

	return 0;
}

// row�࿡ DateBox�� �����Ѵ�.
INT CESL_WorkSheet_dlg::CreateDateBox( INT row )
{
	if( NULL != m_dateBox )
	{
		delete m_dateBox;
		m_dateBox = NULL;
	}
	m_dateBox = new CDateTimeCtrl;
	
	CWnd *wnd = ( CWnd * )GetDlgItem( IDC_MSHFLEXGRID_1 );

	m_dateBox->Create( DTS_APPCANPARSE | DTS_LONGDATEFORMAT | WS_CLIPSIBLINGS | WS_OVERLAPPED, CRect( 0, 0, 10, 20 ), wnd, 646 );
						
	CRect rect = GetGridCellRect();
	m_dateBox->MoveWindow( rect.left - 24, rect.top - 44, rect.Width() + 2, rect.Height() + 2 );

	m_dateBox->ShowWindow( true );
	m_dateBox->SetFocus();
	
	SetBeforeDate( row );

	return 0;
}

// row�࿡ MultiComboBox�� �����Ѵ�.
INT CESL_WorkSheet_dlg::CreateMultiComboBox( INT row )
{
	CESL_Control_Element *ctrlElement = GetCtrlElement( row );

	if( NULL != m_multiComboBox )
		m_multiComboBox->ELEMENT = ctrlElement;
	else
	{
		m_multiComboBox = new CESL_ControlMultiComboBox( ctrlElement );	
		m_multiComboBox->SubclassDlgItem( IDC_MUUTICOMBO_BOX, this );
	}
	
	// WJChoi - (Start) - �޺��ڽ��� ����Ÿ�� ������ ��� �����ش�. 
	// �ϴ�. CJStruct List�� ��� ��������
	INT nRow = m_multiComboBox->m_jcbList.GetCount();
	CJStruct *pRow;
	for( INT i = 0; i < nRow; i++ )
	{
		pRow = ( CJStruct * )m_multiComboBox->m_jcbList.RemoveHead();
		if( pRow )
			delete pRow;
	}
	m_multiComboBox->m_jcbList.RemoveAll();
	// JComboBox�� �ʱ�ȭ �ϰ�
	m_multiComboBox->PrevRow = -1;
	m_multiComboBox->PrevCol = -1;
	m_multiComboBox->m_nMaxRowCnt = 0;

	// CComboBox�� �ʱ�ȭ �Ѵ�.
	if( m_multiComboBox->GetCount() > 0 )
		m_multiComboBox->ResetContent();
	// WJChoi - ( END ) - �޺��ڽ��� ����Ÿ�� ������ ��� �����ش�. 

	m_multiComboBox->InitMultiComboBoxRef();

	INT rowCount = m_multiComboBox->GetRowCount();

	CRect rect = GetGridCellRect();
	CComboBox *comboBox = ( CComboBox * )GetDlgItem( IDC_MUUTICOMBO_BOX );
	comboBox->MoveWindow( rect.left, rect.top, rect.Width() + 2, rect.Height() * rowCount );
	
	m_multiComboBox->ShowWindow( true );
	
	delete ctrlElement;
	
	return 0;
}

// ������ �Էµ� ��¥���� ���� �����Ǵ� dateBox�� �ִ´�. 
VOID CESL_WorkSheet_dlg::SetBeforeDate( INT row )
{
	CString str = m_ctrlInfo[ row - 1 ].value;
	if( !str.GetLength() )
		return;

	COleDateTime day;
	INT y = 0, m = 0, d = 0;

	_stscanf( str.GetBuffer( 0 ), _T("%d/%d/%d\0"), &y, &m, &d );
	day.SetDate( y, m, d );
	m_dateBox->SetTime( day );
	UpdateData( true );
}

// MultiComboBox�� �����ϱ� ���� CESL_Control_Element �� �޾ƿ´�.
CESL_Control_Element * CESL_WorkSheet_dlg::GetCtrlElement( INT index )
{
	CWnd *wnd = ( CWnd * )GetDlgItem( IDC_MSHFLEXGRID_1 );
	
	CESL_Control_Element * ctrlElement;
	ctrlElement = new CESL_Control_Element( this );

	ctrlElement->WorkWnd		= m_wnd;
	ctrlElement->CM_ALIAS		= m_cmAlias;
	ctrlElement->CTRL_ID		= IDC_MUUTICOMBO_BOX;
	ctrlElement->CTRL_TYPE		= _T("M");
	ctrlElement->CTRL_STYLE		= m_ctrlInfo[ index - 1 ].ctrl_style;
	ctrlElement->INIT_VALUE		= m_ctrlInfo[ index - 1 ].init_value;
	ctrlElement->LIST_DATA		= m_ctrlInfo[ index - 1 ].list_data;
	ctrlElement->LINK_DATA		= m_ctrlInfo[ index - 1 ].link_data;
	ctrlElement->VALIDATION		= m_ctrlInfo[ index - 1 ].validation;
	ctrlElement->REQUIRECHECK	= m_ctrlInfo[ index - 1 ].requirecheck;

	return ctrlElement;
}

// �׸��忡�� ���� ���������,
VOID CESL_WorkSheet_dlg::OnEnterCellMshflexgrid1() 
{
	// TODO: Add your control notification handler code here

	INT row = m_grid_1.GetRow();
	INT col = m_grid_1.GetCol();

	if( col != 2 )
		return;

	switch( GetControlType( row ) )
	{
		case 'E' :
			CreateEditBox( row );
			break;

		case 'D' :
			CreateDateBox( row );
			break;

		case 'M' :
			CreateMultiComboBox( row );
			break;
	}	
}

// �׸��� �Ǵ� control�迭�� row��° control type�� ���´�. 
TCHAR CESL_WorkSheet_dlg::GetControlType( INT row, INT option )
{
	// option == 1 : �׸��� ��	( default )
	// option == 0 : ControlInfo class�� index

	TCHAR type;
	wmemcpy( &type, &m_ctrlInfo[ row - 1 ].ctrl_type.GetBuffer( 0 )[ 0 ], 1 );

	return type;
}


// �Է� �׸����� ���ȣ, ������, alias�� �ִ´�. 
INT CESL_WorkSheet_dlg::SetInputGridCaption( CString *alias, INT count )
{
	if( count < 1 )
		m_grid_1.SetRows( 2 );
	else
		m_grid_1.SetRows( count + 1 );
	m_grid_1.SetCols( 0, 3 );

	SetGrid1String( 0, 1, _T("Alias") );
	SetGrid1String( 0, 2, _T("Value") );

	CString tmp;
	for( INT i = 0; i < count; i++ )
	{
		tmp.Format( _T("%d"), i + 1 );
		SetGrid1String( i + 1, 0, tmp );
		SetGrid1String( i + 1, 1, alias[ i ] );

		m_grid_1.SetRowHeight( i + 1, 400 );
	}

	return 0;
}

// �Է¹��� control�� ������ ���´�. 
INT CESL_WorkSheet_dlg::GetInputControlInfo( CString *alias, INT count )
{
	INT index = 0;
	INT cecount = m_pCM->Control_List.GetCount();
	CESL_Control_Element * pCE = NULL;
	POSITION pos;
	pos = m_pCM->Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = NULL;
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		
		for( INT j = 0; j < count; j++ )
			// KOL.UDF.022
			//if( pCE->CTRL_ALIAS == alias[ j ] && pCE )
			if( &pCE != NULL && pCE->CTRL_ALIAS == alias[ j ] )
			{
				m_ctrlInfo[ index ].ctrl_alias	= pCE->CTRL_ALIAS;
				m_ctrlInfo[ index ].ctrl_type	= pCE->CTRL_TYPE;
				m_ctrlInfo[ index ].ctrl_id		= pCE->CTRL_ID;
				m_ctrlInfo[ index ].ctrl_style	= pCE->CTRL_STYLE;
				m_ctrlInfo[ index ].init_value	= pCE->INIT_VALUE;
				m_ctrlInfo[ index ].list_data	= pCE->LIST_DATA;
				m_ctrlInfo[ index ].link_data	= pCE->LINK_DATA;
				m_ctrlInfo[ index ].validation	= pCE->VALIDATION;
				m_ctrlInfo[ index ].requirecheck= pCE->REQUIRECHECK;

				index++;
			}
		}	
	
	return 0;
}

// ������ ���� ��Ʈ�ѿ� �ִ� ������ �޾ƿ´�.
INT CESL_WorkSheet_dlg::GetControlValue()
{
	for( INT i = 0; i < m_controlCnt; i++ )
		if( 'M' != m_ctrlInfo[ i ].ctrl_type )
			m_ctrlMgr->GetControlMgrData( m_ctrlInfo[ i ].ctrl_alias, m_ctrlInfo[ i ].value );
		else
		{
			m_ctrlMgr->GetControlMgrData( m_ctrlInfo[ i ].ctrl_alias, m_ctrlInfo[ i ].value, -1, 0 );
			m_ctrlMgr->GetControlMgrData( m_ctrlInfo[ i ].ctrl_alias, m_ctrlInfo[ i ].desc,  -1, 1 );
		}
	
	return 0;
}

// �Է�ȭ�鿡 ������ control��� ������ ���´�. 
CString * CESL_WorkSheet_dlg::GetViewControlInfo( INT &count )
{
	INT allControlCount;
	CString *allControlAlias;
	allControlAlias = GetAllControlInfo( allControlCount );

	INT hiddenControlCount;
	CString *hiddenControlAlias;
	hiddenControlAlias = GetViewControInfo( hiddenControlCount );

	count = allControlCount - hiddenControlCount;
	CString *alias = new CString[ count ];
	
	INT index = 0;
	bool check = false;
	for( INT i = 0; i < allControlCount; i++ )
	{
		for( INT j = 0; j < hiddenControlCount; j++ )
			if( allControlAlias[ i ] == hiddenControlAlias[ j ] )
			{
				check = true;
				continue;
			}

		if( false == check )
			alias[ index++ ] = allControlAlias[ i ];
		check = false;
	}

	delete [] allControlAlias;
	delete [] hiddenControlAlias;

	return alias;
}

// ã�� ������ control�� ���� ���� class�� ���� �޸� ����
VOID CESL_WorkSheet_dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}


// �Է±׸��忡�� ���� ������,
VOID CESL_WorkSheet_dlg::OnLeaveCellMshflexgrid1() 
{
	// TODO: Add your control notification handler code here
	
	CString str;
	COleDateTime day;

	INT row = m_grid_1.GetRow();
	INT col = m_grid_1.GetCol();

	if( 2 != col )
		return;

	switch( GetControlType( row ) )
	{
		case 'E' :
			
			if( NULL == m_editBox )
				return;

			m_editBox->GetWindowText( str );
			SetGrid1String( row, col, str );

			delete m_editBox;
			m_editBox = NULL;

			m_ctrlInfo[ row - 1 ].value = str;
			
			break;
	
		case 'D' :

			if( NULL == m_dateBox )
				return;

			m_dateBox->GetTime( day );
			
			m_ctrlInfo[ row - 1 ].value.Format( _T("%04d/%02d/%02d"), day.GetYear(), day.GetMonth(), day.GetDay() );
			SetGrid1String( row, col, m_ctrlInfo[ row - 1 ].value );
			
			delete m_dateBox;
			m_dateBox = NULL;

			break;
		
		case 'M' :

			m_multiComboBox->GetData( m_ctrlInfo[ row - 1 ].value, -1, 0 );
			m_multiComboBox->GetData( m_ctrlInfo[ row - 1 ].desc,  -1, 1 );

			SetGrid1String( row, col, m_ctrlInfo[ row - 1 ].desc );
			
			m_multiComboBox->ShowWindow( false );
			
			break;
		
		default :
			break;
	}
}

// �׸��� �ȿ����� key ó��.
BOOL CESL_WorkSheet_dlg::PreTranslateMessage( MSG* pMsg ) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( m_tab.GetCurSel() )
		return CDialog::PreTranslateMessage( pMsg );
	
	if( WM_KEYDOWN == pMsg->message )
		switch( pMsg->wParam )
		{
			case VK_TAB :
				
				INT row, col;
				GetGridFocus( row, col );
				if( 2 != col )
					return CDialog::PreTranslateMessage( pMsg );

				MoveGridCell();
				return true;
	
				break;

			case VK_ESCAPE :
				
				OnLeaveCellMshflexgrid1();
				MoveGridCell( 1 );
				
				return true;

				break;
			
			default :
				break;
		}
	
	return CDialog::PreTranslateMessage( pMsg );
}

// �׸����� ���� ���õ� ����ġ�� �޾ƿ´�. 
VOID CESL_WorkSheet_dlg::GetGridFocus( INT &row, INT &col )
{
	row = m_grid_1.GetRow();
	col = m_grid_1.GetCol();
}

// row��, col���� ��ġ�ϰ� �Ѵ�. 
VOID CESL_WorkSheet_dlg::SetGridFocus( INT row, INT col )
{
	m_grid_1.SetRow( row );
	m_grid_1.SetCol( col );
}

// ���� ���� �̵��Ѵ�. 
VOID CESL_WorkSheet_dlg::MoveGridCell( INT option )
{
	INT rows, row, col;
	rows = m_grid_1.GetRows();
	GetGridFocus( row, col );
	
	OnLeaveCellMshflexgrid1();

	switch( option )
	{
		case 0 :
			if( row + 1 == rows )
				SetGridFocus( 1, 2 );
			else
				SetGridFocus( row + 1, 2 );

			break;
		
		case 1 :
			SetGridFocus( 0, 0 );

			break;
	}

	OnEnterCellMshflexgrid1();	
}


// ��Ƽ�޺��� ���� ���������,
VOID CESL_WorkSheet_dlg::OnSelchangeMuuticomboBox() 
{
	// TODO: Add your control notification handler code here
	m_grid_1.SetEnabled( true );	
	
	if( NULL != m_multiComboBox )
	{
		//delete m_multiComboBox;
		//m_multiComboBox = NULL;
	}	
}

// ���� �׸��忡 �Էµ� ������ �����Ѵ�. 
INT CESL_WorkSheet_dlg::SaveWorkSheetData()
{
	MoveGridCell( 1 );
	
	for( INT i = 0; i < m_controlCnt; i++ )
		m_ctrlMgr->SetControlMgrData( m_ctrlInfo[ i ].ctrl_alias, m_ctrlInfo[ i ].value, -1, 0 );

	return 0;
}

// worksheet�� ����Ÿ�� �ٽ� �о�´�
INT CESL_WorkSheet_dlg::RefreshWorkSheetData( VOID )
{
	GetControlValue();
	SetControls();
	
	return 0;
}

VOID CESL_WorkSheet_dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == 1905 )
	{
		KillTimer( 1905 );
		m_grid_1.SetRow( 1 );
		m_grid_1.SetCol( 0 );
		m_grid_1.SetFocus();
		UpdateData( false );
	}

	CDialog::OnTimer(nIDEvent);
}

INT CESL_WorkSheet_dlg::SetRect(CRect rect)
{
	m_rect = rect;
	return 0;
}

CRect CESL_WorkSheet_dlg::GetRect(VOID)
{
	return m_rect;
}

INT CESL_WorkSheet_dlg::ReSizeWindow(VOID)
{
	CWnd *pWnd;
	
	pWnd = GetDlgItem( IDC_TAB_COM );
	if( pWnd )
		pWnd->MoveWindow( &m_rect, true );

	pWnd = GetDlgItem( IDC_STATIC1111 );
	if( pWnd )
		pWnd->MoveWindow( m_rect.left + 10, m_rect.top + 20, m_rect.Width() - 20, m_rect.Height() - 30, true );

	pWnd = GetDlgItem( IDC_MSHFLEXGRID_1 );
	if( pWnd )
		pWnd->MoveWindow( m_rect.left + 20, m_rect.top + 40, m_rect.Width() - 40, m_rect.Height() - 60, true );

	pWnd = GetDlgItem( IDC_MSHFLEXGRID_2 );
	if( pWnd )
		pWnd->MoveWindow( m_rect.left + 20, m_rect.top + 130, m_rect.Width() - 40, m_rect.Height() - 190, true );
	
	pWnd = GetDlgItem( IDOK_2 );
	if( pWnd )
		pWnd->MoveWindow( m_rect.right - 20 - 50, m_rect.bottom - 50, 50, 30, true );

	return 0;
}
