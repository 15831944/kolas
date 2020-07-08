// BO_LOC_1360.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1360.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1360 dialog


CBO_LOC_1360::CBO_LOC_1360( CESL_DataMgr *pDM, CMSHFlexGrid *pGrid, CESL_Mgr *pParent /*=NULL*/ )
	: CESL_Mgr( IDD, pParent )
{
	pParentDM = pDM;
	pParentGrid = pGrid;
	
	//{{AFX_DATA_INIT(CBO_LOC_1360)
	// KOL.UDF.022
	m_sAssignObjectKey = _T(""); 
	
	m_resizeValues[ 0 ].ID = IDC_TAB1;
	m_resizeValues[ 1 ].ID = IDC_gridMAIN_01;
	m_resizeValues[ 2 ].ID = IDC_gridMAIN_02;
	//}}AFX_DATA_INIT
}

BOOL CBO_LOC_1360::Create( CWnd *pParentWnd )
{	
	return CDialog::Create( IDD, pParentWnd ); 
}	
	
VOID CBO_LOC_1360::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1360)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1360, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1360)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_bSEARCH_USAGE_INFO, OnbSEARCHUSAGEINFO)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bFIND, OnbFIND)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1360 message handlers

BOOL CBO_LOC_1360::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("BO_LOC_1360") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error!") );
		return false;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_1360::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	if( bShow )
	{
		MakeDM();
		ShowBookList();

		SetResizeValues();
	}
}

VOID CBO_LOC_1360::MakeDM()
{
	INT rowCnt = pParentDM->GetRowCount();
	
	const INT DMAliasCnt = 42;
	CString DMAlias[ DMAliasCnt ] = {
		_T("IBS_û��_������ȣ"),
		_T("IBS_û��_�з���ȣ"),
		_T("IBS_û��_������ȣ"),
		_T("BB_��/��������"),
		_T("BB_��������"),
		_T("��ü����"),
		_T("����"),
		_T("BB_�����"),
		_T("BB_åKEY"),
		_T("BB_��Ϲ�ȣ"),
		_T("BB_��ǥå��"),
		_T("IBS_û��_��ġ��ȣ_�ڵ�"),
		_T("BB_�谡��������"),
		_T("BB_�谡�����������"),
		_T("BB_�谡�������"),
		_T("BB_���ʹ谡����"),
		_T("BB_�谡��ȣ"),
		_T("BB_�ڷ�Ǹ�"),
		_T("BB_�谡����������"),
		_T("BB_�̿����ѱ���"),
		_T("BB_��/����"),
		_T("IBS_û��_��å��ȣ"),
		_T("BB_����"),
		_T("BB_�谡����"),
		_T("BS_MARC"),
		_T("����"),
		_T("��ǥ��"),
		_T("��������"),
		_T("������"),
		_T("�����ȣ"),
		_T("���ı���"),
		_T("������"),
		_T("BS_��KEY"),
		_T("���»���"),
		_T("û����ȣ"),
		_T("PAGE"),
		_T("PHYSICAL_PROPERTY"),
		_T("BOOK_SIZE"),
		_T("ACCOMP_MAT"),
		_T("���Ա���"),
		_T("�뵵����"),
		_T("�뵵���л���") };
	CString data[ DMAliasCnt ];
	
	//CESL_DataMgr *pDM_T = FindDM( _T("DM_BO_LOC1360_����") );
	CESL_DataMgr *pDM_F = FindDM( _T("DM_BO_LOC1360_������") );
		
	INT useageBookCnt;
	for( INT i = 0; i < rowCnt; i++ )
		if( _T("V") == pParentGrid->GetTextMatrix( i + 1, 1 ) )
		{
			useageBookCnt = _ttoi( pParentDM->GetCellData( _T("�뵵���л���"), i ) );
			pParentDM->GetCellData( DMAlias, DMAliasCnt, data, i );

			if( !useageBookCnt )
				pDM_F->SetCellData( DMAlias, DMAliasCnt, data, -1 );
				

			/*
			// �����ڷ��ϰ��
			if( useageBookCnt )
				pDM_T->SetCellData( DMAlias, DMAliasCnt, data, -1 );
			// �����ڷᰡ �ƴҰ��
			else
				pDM_F->SetCellData( DMAlias, DMAliasCnt, data, -1 );
			*/
		}
}

VOID CBO_LOC_1360::ShowBookList( INT curSel )
{
	CESL_DataMgr *pDM_T = FindDM( _T("DM_BO_LOC1360_����") );
	CESL_DataMgr *pDM_F = FindDM( _T("DM_BO_LOC1360_������") );

	const INT cnt = 2;
	CString title[ cnt ];
	title[ 0 ].Format( _T(" ���� ����ڷ� ") );
	title[ 1 ].Format( _T(" ���� �ڷ� ") );
	
	m_tab.DeleteAllItems();
	for( INT i = 0; i < cnt; i++ )
		m_tab.InsertItem( i, title[ i ] );

	ControlDisplay( _T("CM_BO_LOC_1360"), _T("�����׸���") );
	ControlDisplay( _T("CM_BO_LOC_1360"), _T("�������׸���") );

	if( -1 == curSel )
		if( pDM_F->GetRowCount() )
			curSel = 0;
		else
			curSel = 1;

	m_tab.SetCurSel( curSel );
	ShowTabCtrls( curSel );
}

VOID CBO_LOC_1360::ShowTabCtrls( INT curSel )
{
	const INT tabCtrlsCnt_0 = 1;
	UINT ID_0[ tabCtrlsCnt_0 ] = {
		IDC_gridMAIN_02 };
		
	const INT tabCtrlsCnt_1 = 3;
	UINT ID_1[ tabCtrlsCnt_1 ] = {
		IDC_gridMAIN_01,
		IDC_bFIND,
		IDC_bMODIFY};

	bool flag[ 2 ] = { false, false };
	flag[ curSel ] = true;
		
	CWnd *pWnd = NULL;
	INT i = 0;
	for( i = 0; i < tabCtrlsCnt_0; i++ )
	{
		pWnd = GetDlgItem( ID_0[ i ] );
		pWnd->ShowWindow( flag[ 0 ] );
	}
	for( i = 0; i < tabCtrlsCnt_1; i++ )
	{
		pWnd = GetDlgItem( ID_1[ i ] );
		pWnd->ShowWindow( flag[ 1 ] );
	}

	if( curSel )
		SetDlgItemText( IDOK, _T("���� ����") );
	else
		SetDlgItemText( IDOK, _T("�� ��") );

	const INT enableCtrlCnt = 5;
	UINT eanbleCntrlID[ enableCtrlCnt ] = {
		IDC_bSEARCH_USAGE_INFO, IDC_eSUBJECT, IDC_ePROFESSOR, IDC_COMBO1, IDC_eREMARK };
	for( i = 0; i < enableCtrlCnt; i++ )
	{
		pWnd = GetDlgItem( eanbleCntrlID[ i ] );
		
		if( i )
			pWnd->EnableWindow( flag[ 1 ] );
		else
			pWnd->EnableWindow( flag[ 0 ] );
	}
}

VOID CBO_LOC_1360::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	ShowTabCtrls( m_tab.GetCurSel() );
	
	*pResult = 0;
}

VOID CBO_LOC_1360::OnOK() 
{
	// TODO: Add extra validation here
	
	switch( m_tab.GetCurSel() )
	{
		case 0 :	// ����
			SetUsageCode();
			break;
		case 1 :	// ��������
			RemoveUsageSet();
			break;
		default :
			return;
			break;
	}
	
	//CDialog::OnOK();
}

VOID CBO_LOC_1360::OnbSEARCHUSAGEINFO() 
{
	// TODO: Add your control notification handler code here
	
	if( !m_tab.GetCurSel() )
		SetAssignObjectInfo();
}

VOID CBO_LOC_1360::SetAssignObjectInfo()
{
	CAssignObjectList AssignObjectListDlg( this );
	if( IDOK != AssignObjectListDlg.DoModal() )
		return;

	CAssignObject *AssignObject = AssignObjectListDlg.GetAssignObject();
	SetDlgItemText( IDC_eSUBJECT, AssignObject->GetSubject() );
	SetDlgItemText( IDC_ePROFESSOR, AssignObject->GetProfessor() );
	SetDlgItemText( IDC_eREMARK, AssignObject->GetRemark() );
	SetControlData( _T("CM_BO_LOC_1360"), _T("�а�"), AssignObject->GetCourseCode() );
	m_sAssignObjectKey = AssignObject->GetKey();
}

// ����
VOID CBO_LOC_1360::SetUsageCode()
{
	if( m_sAssignObjectKey.IsEmpty() )
	{
		AfxMessageBox( _T("���������� �Է��Ͽ� �ֽʽÿ�") );
		return;
	}
	
	CString from, to;
	GetControlData( _T("CM_BO_LOC_1360"), _T("��������_����"), from );
	GetControlData( _T("CM_BO_LOC_1360"), _T("��������_����"), to );
	if( from.IsEmpty() || to.IsEmpty() )
	{
		AfxMessageBox( _T("�����Ⱓ�� �Է��Ͽ� �ֽʽÿ�") );
		return;
	}
	
	CArray< INT, INT >checkedRow;
	INT cnt = GetCheckedDMRow( ( CMSHFlexGrid * )GetDlgItem( IDC_gridMAIN_02 ), checkedRow );
	if( !cnt )
	{
		AfxMessageBox( _T("������ �ڷḦ �����Ͽ� �ֽʽÿ�") );
		return;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC1360_������") );
	
	INT ids = 0;
	
	ids = pDM->StartFrame();
	if( ids < 0 )
	{
		AfxMessageBox( _T("Error : DB Operation...StartFrame") );
		return;
	}
	pDM->InitDBFieldData();

	CString bookKey, query;
	INT idx = 0;
	for( INT i = cnt - 1; i > -1; i-- )
	{
		idx = checkedRow.GetAt( i );
		bookKey = pDM->GetCellData( _T("BB_åKEY"), idx );
		
		query.Format( _T("UPDATE BO_BOOK_TBL SET USAGE_CODE = 'AS' WHERE REC_KEY = %s;"), bookKey );
		pDM->AddFrame( query );
		query.Format( 
					_T("INSERT INTO CO_ASSIGN_INFO_TBL( ")
					_T("REC_KEY, ASSIGN_OBJECT_KEY, ASSIGN_START_DATE, ASSIGN_END_DATE, BOOK_KEY, ASSIGN_STATUS, FIRST_WORK ) ")
					_T("VALUES( ESL_SEQ.NEXTVAL, %s, '%s', '%s', %s, 'A', '%s' );"),
					m_sAssignObjectKey, from, to, bookKey, GetWorkLogMsg( _T("�����ڷ������ϱ�"), __WFILE__, __LINE__ ) );
		pDM->AddFrame( query );

		MoveBookInfo( idx, true );
	}

	ids = pDM->SendFrame();
	if( ids < 0 )
	{
		AfxMessageBox( _T("Error : DB Operation...SendFrame") );
		return;
	}

	ids = pDM->EndFrame();
	if( ids < 0 )
	{
		AfxMessageBox( _T("Error : DB Operation...EndFrame") );
		return;
	}

	AfxMessageBox( _T("���õ� �ڷḦ �����ڷ�� ��ȯ�Ͽ����ϴ�.") );
	ShowBookList( 0 );
}



INT CBO_LOC_1360::GetCheckedDMRow( CMSHFlexGrid *pGrid, CArray< INT, INT >&checkedRow )
{
	for( INT i = 1; i < pGrid->GetRows(); i++ )
		if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
			checkedRow.Add( i - 1 );
	return checkedRow.GetSize();
}

VOID CBO_LOC_1360::MoveBookInfo( INT idx, bool flag )
{
	return;
	
	// true  == flag : �������ڷ� -> �����ڷ�
	// false == flag : �����ڷ� -> �������ڷ�

	CESL_DataMgr *pDM_From = NULL;
	CESL_DataMgr *pDM_To = NULL;
	if( flag )
	{
		pDM_From = FindDM( _T("DM_BO_LOC1360_������") );
		pDM_To = FindDM( _T("DM_BO_LOC1360_����") );
	}
	else
	{
		pDM_From = FindDM( _T("DM_BO_LOC1360_����") );
		pDM_To = FindDM( _T("DM_BO_LOC1360_������") );
	}
	
	const INT DMAliasCnt = 4;
	CString DMAlias[ DMAliasCnt ] = {
		_T("BB_åKEY"), _T("BB_��Ϲ�ȣ"), _T("û����ȣ"), _T("��ǥ��") };
	CString data[ DMAliasCnt ];

	if( pDM_From->GetRowCount() - 1 < idx )
	{
		AfxMessageBox( _T("Error : MoveBookList( INT idx, bool flag )") );
		return;
	}
	
	pDM_From->GetCellData( DMAlias, DMAliasCnt, data, idx );
	pDM_To->SetCellData( DMAlias, DMAliasCnt, data, -1 );
	pDM_From->DeleteRow( idx );
}

VOID CBO_LOC_1360::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	ResizeWindow( cx, cy );
}

VOID CBO_LOC_1360::ResizeWindow( INT cx, INT cy )
{
	CWnd *pWnd = NULL;
	CRect rect;
	for( INT i = 0; i < RESIZE_CTRL_CNT; i++ )
	{
		pWnd = GetDlgItem( m_resizeValues[ i ].ID );
		if( !pWnd->GetSafeHwnd() )
			continue;

		pWnd->GetWindowRect( &rect );
		ScreenToClient( rect );
		pWnd->MoveWindow( 
						rect.left,
						rect.top,
						cx - m_resizeValues[ i ].width,
						cy - m_resizeValues[ i ].height );
	}
	
}

VOID CBO_LOC_1360::SetResizeValues()
{
	if( !this->GetSafeHwnd() )
	{
		AfxMessageBox( _T("Error : SetResizeValues()...") );
		return;
	}
	
	m_resizeValues[ 0 ].ID = IDC_TAB1;
	m_resizeValues[ 1 ].ID = IDC_gridMAIN_01;
	m_resizeValues[ 2 ].ID = IDC_gridMAIN_02;

	CRect wndRect, rect;
	this->GetWindowRect( wndRect );
	ScreenToClient( wndRect );
	CWnd *pWnd = NULL;
	for( INT i = 0; i <	RESIZE_CTRL_CNT; i++ )
	{
		pWnd = GetDlgItem( m_resizeValues[ i ].ID );
		pWnd->GetWindowRect( &rect );
		ScreenToClient( rect );

		m_resizeValues[ i ].width = wndRect.Width() - rect.Width() - 8;
		m_resizeValues[ i ].height = wndRect.Height() - rect.Height() - 27;
	}
}


/*
������� �ڷ� �׸���
	�谡����
	��Ϲ�ȣ
	û����ȣ
	����
	������
	��/����
	�Ǽ���
	������
�����ڷ� �׸���
	�����Ⱓ
	�̷º��� : button	
*/

VOID CBO_LOC_1360::OnbFIND() 
{
	// TODO: Add your control notification handler code here
	
	SearchAssignInfo();
}

VOID CBO_LOC_1360::SearchAssignInfo()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC1360_����") );
	pDM->RefreshDataManager( GetCondition() );

	// û����ȣ �����
	CLocCommonAPI::MakeShelfCode( this, _T("DM_BO_LOC1360_����") );
	// �谡���� ����
	CString workingStatus;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		workingStatus = pDM->GetCellData( _T("�谡����"), i );
		CLocCommonAPI::GetWorkingStatusDescToCode( workingStatus, 1 );
		pDM->SetCellData( _T("�谡����"), workingStatus, i );
	}

	ControlDisplay( _T("CM_BO_LOC_1360"), _T("�����׸���") );
	
}

CString CBO_LOC_1360::GetCondition()
{
	const INT cnt = 5;
	CString data[ cnt ];
	GetDlgItemText( IDC_eSUBJECT, data[ 0 ] );
	GetDlgItemText( IDC_ePROFESSOR, data[ 1 ] );
	GetControlData( _T("CM_BO_LOC_1360"), _T("��������_����"), data[ 2 ] );
	GetControlData( _T("CM_BO_LOC_1360"), _T("��������_����"), data[ 3 ] );
	GetControlData( _T("CM_BO_LOC_1360"), _T("�а�"), data[ 4 ] );
	
	//CO_ASSIGN_INFO_TBL I
	//CO_ASSIGN_OBJECT_TBL O

	CString where, buf;
	
	CIndex index;
	if( !data[ 0 ].IsEmpty() )
	{
		index.MakeIndex( data[ 0 ] );
		buf.Format( _T("O.IDX_SUBJECT_NAME LIKE '%%%s%%' AND "), data[ 0 ] );
		where += buf;
	}

	if( !data[ 1 ].IsEmpty() )
	{
		index.MakeIndex( data[ 1 ] );
		buf.Format( _T("O.IDX_PROFESSOR_NAME LIKE '%%%s%%' AND "), data[ 1 ] );
		where += buf;
	}

	if( !data[ 2 ].IsEmpty() )
	{
		buf.Format( _T("I.ASSIGN_START_DATE = TO_DATE( '%s', 'YYYY/MM/DD' ) AND "), data[ 2 ] );
		where += buf;
	}

	if( !data[ 3 ].IsEmpty() )
	{
		buf.Format( _T("I.ASSIGN_END_DATE = TO_DATE( '%s', 'YYYY/MM/DD' ) AND "), data[ 3 ] );
		where += buf;
	}

	if( !data[ 4 ].IsEmpty() )
	{
		buf.Format( _T("O.COURSE_CODE = '%s' AND "), data[ 4 ] );
		where += buf;
	}

	if( _T(" AND ") == where.Right( 5 ) )
		where = where.Left( where.GetLength() - 5 );
	
	return where;
}

VOID CBO_LOC_1360::OnbMODIFY() 
{
	// TODO: Add your control notification handler code here
	
	ModifyAssignInfo();
}

VOID CBO_LOC_1360::ModifyAssignInfo()
{
	CBO_LOC_1361 UpdateDlg( FindDM( _T("DM_BO_LOC1360_����") ), this );
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridMAIN_01 );
	INT cnt = GetCheckedDMRow( pGrid, UpdateDlg.m_checkedRow );
	if( !cnt )
	{
		AfxMessageBox( _T("������ �ڷḦ �����Ͽ� �ֽʽÿ�") );
		return;
	}
	
	UpdateDlg.DoModal();
	ShowBookList( 1 );

	for( INT i = 0 ; i < cnt; i++ )
		pGrid->SetTextMatrix( UpdateDlg.m_checkedRow.GetAt( i ) + 1, 1, _T("V") );
}

// ���� ����
VOID CBO_LOC_1360::RemoveUsageSet()
{
	CArray< INT, INT >checkedRow;
	INT cnt = GetCheckedDMRow( ( CMSHFlexGrid * )GetDlgItem( IDC_gridMAIN_01 ), checkedRow );
	if( !cnt )
	{
		AfxMessageBox( _T("���������� �ڷḦ �����Ͽ� �ֽʽÿ�") );
		return;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC1360_����") );
	INT ids = 0;
	ids = pDM->StartFrame();
	if( ids < 0 )
	{
		AfxMessageBox( _T("Error : DB Operation...StartFrame ") );
		return;
	}
	pDM->InitDBFieldData();
	
	CString bookKey, query;
	INT idx = 0;
	for( INT i = cnt - 1; i > -1; i-- )
	{
		idx = checkedRow.GetAt( i );
		bookKey = pDM->GetCellData( _T("BOOK_KEY"), idx );

		query.Format( _T("UPDATE BO_BOOK_TBL SET USAGE_CODE = NULL WHERE REC_KEY = %s;"), bookKey );
		pDM->AddFrame( query );
		query.Format( _T("UPDATE CO_ASSIGN_INFO_TBL SET ASSIGN_STATUS = 'C' WHERE BOOK_KEY = %s;"), bookKey );
		pDM->AddFrame( query );

		pDM->DeleteRow( idx );
	}

	ids = pDM->SendFrame();
	if( ids < 0 )
	{
		AfxMessageBox( _T("Error : DB Operation...SendFrame") );
		return;
	}

	ids = pDM->EndFrame();
	if( ids < 0 )
	{
		AfxMessageBox( _T("Error : DB Operation...EndFrame") );
		return;
	}

	AfxMessageBox( _T("���õ� �ڷḦ �������� �Ͽ����ϴ�.") );
	ShowBookList( 1 );	
}

VOID CBO_LOC_1360::SetGridReverse( INT DMRow )
{
	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_1360") );
	CESL_Grid *pGrid = ( CESL_Grid * )( pCM->FindControl( _T("�����׸���") ) );
	pGrid->SetReverse( DMRow );
}

BEGIN_EVENTSINK_MAP(CBO_LOC_1360, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_1360)
	ON_EVENT(CBO_LOC_1360, IDC_gridMAIN_01, -601 /* DblClick */, OnDblClickgridMAIN01, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_1360::OnDblClickgridMAIN01() 
{
	// TODO: Add your control notification handler code here
	
	ViewAssignInfoHistroy();
}


VOID CBO_LOC_1360::ViewAssignInfoHistroy()
{
	CAssignInfoLog dlg( this );
	dlg.SetMainDM( FindDM( _T("DM_BO_LOC1360_����") ) );
	dlg.SetMainGrid( ( CESL_Grid * )( ( FindCM( _T("CM_BO_LOC_1360") ) )->FindControl( _T("�����׸���") ) ) );
	dlg.SetBookKeyAlias( _T("BOOK_KEY") );

	dlg.DoModal();
}
