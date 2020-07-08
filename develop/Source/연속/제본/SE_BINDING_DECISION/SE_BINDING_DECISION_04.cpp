// SE_BINDING_DECISION_04.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_DECISION_04.h"
#include "SE_BINDING_DECISION_05.h"
#include "SE_BINDING_DECISION_06.h"
//#include _T("Test.h")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_DECISION_04 dialog

CSE_BINDING_DECISION_04::CSE_BINDING_DECISION_04(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_nIdx = 0;
	m_nBindBookIdx = 0;
	m_sAcquisitCode = _T("1");

	// update button click ����
	m_bModify = FALSE;

	// 
	m_bInterBindYn = FALSE;

	m_sAcqYear = _T("");

	// 
	m_nPreBookIdx = 0;

	// bokbon count
	m_nMaxCopyBookCnt = 0;

	// bokbonTab index
	m_nBokbonTabIndex = 0;

	// Binding VolTitle type
	m_nBindVolTitleType = 0;

	//
	m_nHeadPosition = m_nTailPosition = 0;
}

CSE_BINDING_DECISION_04::~CSE_BINDING_DECISION_04()
{
}

VOID CSE_BINDING_DECISION_04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_DECISION_04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_BINDING_DECISION_04, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_DECISION_04)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bBALJU_DATA, OnbBALJUDATA)
	ON_BN_CLICKED(IDC_bMISS_DATA, OnbMISSDATA)
	ON_BN_CLICKED(IDC_bUPDATE, OnbUPDATE)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bBINDING_PRINT, OnbBINDINGPRINT)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_BOKBON, OnSelchangeTabBokbon)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSE_BINDING_DECISION_04, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_BINDING_DECISION_04)
	ON_EVENT(CSE_BINDING_DECISION_04, IDC_LIST_BINGIND_BOOK, -600 /* Click */, OnClickListBingindBook, VTS_NONE)
	ON_EVENT(CSE_BINDING_DECISION_04, IDC_LIST_BOOK, -600 /* Click */, OnClickListBook, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_DECISION_04 message handlers

BOOL CSE_BINDING_DECISION_04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_DECISION_04")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

	InitDialog();

	INT ids = MoveIndex( _CURRENT );
	if( ids ) {
		AfxMessageBox (_T("MoveIndex() Error~!"));
		return FALSE ;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_BINDING_DECISION_04::InitDialog()
{
	INT ids;

	// Mod 2003.06.28 wsw
	// init
	m_ViewerControl2.InitViewer(IDC_LIST_DETAIL, this);

	// Tag column�� Data Column�� ũ�⸦ �����ش�.
	m_ViewerControl2.SetViewerWidth(24, 75);

	// Mod 2003.06.28 wsw
	// mod grid
	CESL_ControlMgr * pCtrlMgr = FindCM(_T("CM_SE_BINDING_DECISION_04_BIND"));
	if( !pCtrlMgr ) return;
	
	CESL_Grid *pBindGrid = (CESL_Grid *)pCtrlMgr->FindControl(_T("BindGrid"));
	if( !pBindGrid ) return;

	pBindGrid->m_bGridEdit = TRUE;
	pBindGrid->SELEDITOR_AddColumn(3);
	pBindGrid->SELEDITOR_AddColumn(4);

	// å���� �׸���� ���ľȵǰ�..
	CESL_ControlMgr * pCtrlMgr2 = FindCM(_T("CM_SE_BINDING_DECISION_04_BOOK"));
	if( !pCtrlMgr ) return;
	
	CESL_Grid *pBookGrid = (CESL_Grid *)pCtrlMgr2->FindControl(_T("BookGrid"));
	if( !pBookGrid ) return;

	ids = pBookGrid->SetSort( FALSE );
	if( ids < 0 ) return;


	// Mod 2003.06.28 wsw
	//((CEdit*) GetDlgItem(IDC_eBIND_VOL_TITLE))	-> SetReadOnly(TRUE);
	//((CEdit*) GetDlgItem(IDC_eREMARKS))			-> SetReadOnly(TRUE);	
	// ���� column �Ⱥ��̰�..
	//CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_DECISION_04_BOOK") );
	//CESL_Grid		*pBookGrid = (CESL_Grid*) pCM->FindControl ( _T("BookGrid") ) ;
	//pBookGrid->SetColumnInfo( 0, _T("����"), 0 );
}

//VOID CSE_BINDING_DECISION_04::OnbTEST()
//{
//	CTest dlg( this );
//	dlg.DoModal();
//}

VOID CSE_BINDING_DECISION_04::OnbBALJUDATA() 
{
	INT ids;
	CString sTitle;

	ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("TITLE"), sTitle, m_nIdx );
	if( ids < 0 ) return;

	CSE_BINDING_DECISION_06 dlg( this );
	dlg.SetSpeciesKey( (TCHAR*)LPCTSTR(m_sSpeciesKey), sTitle );

	dlg.DoModal();
}

VOID CSE_BINDING_DECISION_04::OnbMISSDATA() 
{
	INT ids;
	CString sTitle;

	ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("TITLE"), sTitle, m_nIdx );
	if( ids < 0 ) return;

	CSE_BINDING_DECISION_05 dlg( this );
	dlg.SetSpeciesKey( (TCHAR*)LPCTSTR(m_sSpeciesKey), sTitle );
	
	dlg.DoModal();
}


VOID CSE_BINDING_DECISION_04::OnbUPDATE() 
{
	INT ids, nErrRow;
	CString sMsg;

	//
	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_DECISION_04_BOOK") );
	CESL_Grid		*pBookGrid = (CESL_Grid*) pCM->FindControl ( _T("BookGrid") ) ;

	/* test
	INT nRowCount;

	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BOOK_INFO"));
	if( pDataMgr == NULL ) return;

	nRowCount = pDataMgr->GetRowCount();

	pBookGrid->SetReverse( i  );
	pBookGrid->SetReverse( i +1 );
	*/
	
	if( m_bModify == FALSE ) {
		
		// 1. ��� ��ư ��Ȱ��, bind_book grid ��Ȱ��ȭ, book_ grid Ȱ��ȭ
		GetDlgItem(IDC_TAB_BOKBON)		->EnableWindow(FALSE);
		GetDlgItem(IDC_bBALJU_DATA)		->EnableWindow(FALSE);
		GetDlgItem(IDC_bMISS_DATA)		->EnableWindow(FALSE);
		GetDlgItem(IDC_bBINDING_PRINT)	->EnableWindow(FALSE);
		GetDlgItem(IDC_bPREV)			->EnableWindow(FALSE);
		GetDlgItem(IDC_bNEXT)			->EnableWindow(FALSE);
		GetDlgItem(IDC_bSAVE)			->EnableWindow(FALSE);

		//pBookGrid->SetColumnInfo( 0, _T("����"), 50 );

		// 2. BIND_BOOK_INFO DataMgr�� ������ BOOK_LIST list_ctrl�� �� �Ѹ���
		ids = DoModifyBookDisp();
		if( ids ) {
			AfxMessageBox( _T("DoModifyBookDisp Error!") );
			return;
		}

		m_bModify = TRUE;
	}
	else {

		// 

		/*
		INT nRowCount;

		CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BOOK_INFO"));
		if( pDataMgr == NULL ) return;

		nRowCount = pDataMgr->GetRowCount();

		for( INT i = 0 ; i < nRowCount ; i++ ) {
			pBookGrid->SetReverse( i  );
			pBookGrid->SetReverse( i +1 );

		}
		*/

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 1. S ~ E ���� üũ
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ids = CheckStartEnd( nErrRow );
		if( ids == -100 ) {
			// E is Prev! \n %dth row!
			sMsg.Format( _T("'E'�� ���� �� �� �����ϴ�.(%d��°)") , nErrRow );
			AfxMessageBox( sMsg );
			return;
		}
		else if( ids == -200 ) {
			// S ~ E wrong!
			sMsg.Format( _T("'S'~'E'������ Ʋ�Ƚ��ϴ�.(%d��°)") , nErrRow );
			AfxMessageBox( sMsg );
			return;
		}
		else if( ids == -300 ) {
			// S ~ E wrong!
			sMsg.Format( _T("'S'~'E'������ Ʋ�Ƚ��ϴ�.(%d��°)") , nErrRow );
			AfxMessageBox( sMsg );
			return;
		}
		else if( ids < 0 ) {
			AfxMessageBox( _T("ReMakeBindInfo Error!") );
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 2. S ~ E  Bind_Info �����ϱ�..
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ids = ReMakeBindInfo();
		if( ids ) {
			AfxMessageBox( _T("ReMakeBindInfo Error!") );
			return;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 3. ����å������ �ٽ� �����.
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ids = DoModifyBindInfo();
		if( ids ) {
			AfxMessageBox( _T("DoModifyBindInfo Error!") );
			return;
		}

		// Display
		ids = DisplayBookInfo( 0, TRUE );
		if( ids ) {
			AfxMessageBox( _T("DisplayBookInfo Error!") );
			return;
		}

		ids = DisplayBookInfo( 0 );
		if( ids ) {
			AfxMessageBox( _T("DisplayBookInfo Error!") );
			return;
		}

		// Button
		GetDlgItem(IDC_TAB_BOKBON)		->EnableWindow(TRUE);
		GetDlgItem(IDC_bBALJU_DATA)		->EnableWindow(TRUE);
		GetDlgItem(IDC_bMISS_DATA)		->EnableWindow(TRUE);
		GetDlgItem(IDC_bBINDING_PRINT)	->EnableWindow(TRUE);
		GetDlgItem(IDC_bPREV)			->EnableWindow(TRUE);
		GetDlgItem(IDC_bNEXT)			->EnableWindow(TRUE);
		GetDlgItem(IDC_bSAVE)			->EnableWindow(TRUE);

		//pBookGrid->SetColumnInfo( 0, _T("����"), 0 );
		//pListBook->SetColumnWidth( 1, 0 );

		m_bModify = FALSE;

		MoveIndex( 'z' );
	}	
}

VOID CSE_BINDING_DECISION_04::OnClickListBingindBook() 
{
	INT		ids;
	CString sDecision;

	CESL_ControlMgr * pCtrlMgr = FindCM(_T("CM_SE_BINDING_DECISION_04_BIND"));
	if( !pCtrlMgr ) return;
	
	CESL_Grid *pBindGrid = (CESL_Grid *)pCtrlMgr->FindControl(_T("BindGrid"));
	if( !pBindGrid ) return;
	
	INT nRow = -1;
	INT nCol = -1;

	nCol = pBindGrid->GetCol();

	if( nCol < 0 ) return;
	if( m_bModify == TRUE ) return;

	nRow = pBindGrid->GetIdx();//SelectGetIdx();
	
	if( nCol == 1 ) {

		pBindGrid->GetAt( nRow, nCol-1, sDecision );

		if( sDecision == _T("����") ) {
			ids = DoBindDecideYn( nRow, _T("") );
			if( ids ) return;
		}
		else {
			ids = DoBindDecideYn( nRow, _T("����") );
			if( ids ) return;
		}
	}

	ids = DisplayBookInfo( nRow );
	if( ids ) return;

	pBindGrid->SetReverse(nRow);

	// binding_book index
	m_nBindBookIdx = nRow;

}

VOID CSE_BINDING_DECISION_04::OnClickListBook() 
{
	CString sDecision;

	if( !m_bModify ) return;

	CESL_ControlMgr * pCtrlMgr = FindCM(_T("CM_SE_BINDING_DECISION_04_BOOK"));
	if( !pCtrlMgr ) return;
	
	CESL_Grid *pBookGrid = (CESL_Grid *)pCtrlMgr->FindControl(_T("BookGrid"));
	if( !pBookGrid ) return;
	
	INT nCol = -1;

	nCol = pBookGrid->GetCol();

	if( nCol < 0 ) return;

	if( nCol == 1 ) {

		//iRow = pBookGrid->GetRow();

		//�˾� �޴� �����
		CreatePopUpButton( IDC_LIST_BOOK );	
	}

	//SetGridColor( pBookGrid->GetRow() );
}

VOID CSE_BINDING_DECISION_04::CreatePopUpButton( UINT f_inButtonType )
{
EFS_BEGIN

    CMenu      m_ContextMenu;
	CPoint	point;

	::GetCursorPos( &point );

    m_ContextMenu.CreatePopupMenu();
	switch( f_inButtonType ) {
		case IDC_LIST_BOOK:	// ����
			m_ContextMenu.AppendMenu		( MF_STRING, 4000, _T("����(S)") );
			m_ContextMenu.AppendMenu		( MF_STRING, 4001, _T("��  (E)") );
			m_ContextMenu.AppendMenu		( MF_STRING, 4002, _T("����") );
			m_ContextMenu.AppendMenu		( MF_SEPARATOR);
			m_ContextMenu.AppendMenu		( MF_STRING, 4003, _T("���ǵ��(R)") );
			m_ContextMenu.TrackPopupMenu	( TPM_LEFTALIGN, point.x, point.y, this);
			break;
		default:
			break;
	}

	return;

EFS_END
}

VOID CSE_BINDING_DECISION_04::SelectProc( UINT nIDC)
{
	INT SelectCnt, ids;
	CString strTmp;

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_SE_BINDING_DECISION_04_BOOK"));
	pCM->FindControl(_T("BookGrid"));
	
	CESL_Grid * pBookGrid = NULL;
	pBookGrid = (CESL_Grid*)pCM->FindControl(_T("BookGrid"));
	
	//pBookGrid->MakeSelectedRowList( FALSE );

	switch( nIDC ) {

	case 4000:
		SelectCnt = pBookGrid->GetIdx();
		pBookGrid->SetAt( SelectCnt, 0, _T("S") );
			
		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), _T("S"), SelectCnt );
		if( ids < 0 ) return;
		break;

	case 4001:
		SelectCnt = pBookGrid->GetIdx();
			
		pBookGrid->SetAt( SelectCnt, 0, _T("E") );
			
		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), _T("E"), SelectCnt );
		if( ids < 0 ) return;
		break;

	case 4002:
		SelectCnt = pBookGrid->GetIdx();

		pBookGrid->SetAt( SelectCnt, 0, _T("") );

		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), _T(""), SelectCnt );
		if( ids < 0 ) return;
		break;

	case 4003:
		SelectCnt = pBookGrid->GetIdx();
		pBookGrid->SetAt( SelectCnt, 0, _T("R") );

		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), _T("R"), SelectCnt );
		if( ids < 0 ) return;
		break;
	}
}

VOID CSE_BINDING_DECISION_04::InitializeTabCtrl()
{
	INT i, nItemCnt;
	CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_BOKBON);

	nItemCnt = pTabCtrl->GetItemCount();

	for( i = nItemCnt-1 ; i >= 0 ; i-- )
		pTabCtrl->DeleteItem(i);

	TC_ITEM TabCtrlItem;
	CRect rect;
	
	TCHAR *title[] = { _T("���� 1") , _T("���� 2") , _T("���� 3") , _T("���� 4") , _T("���� 5"),
					   _T("���� 6") , _T("���� 7") , _T("���� 8") , _T("���� 9") , _T("���� 10"),
					   _T("���� 11"), _T("���� 12"), _T("���� 13"), _T("���� 14"), _T("���� 15")
	};

	TabCtrlItem.mask = TCIF_TEXT;

	// MOD wsw 2003.06.30 copy_code
	//for( i = 0 ; i < m_nMaxCopyBookCnt ; i++ ) {
	for( i = 0 ; i <= m_nMaxCopyBookCnt ; i++ ) {

		TabCtrlItem.pszText = title[i];
		pTabCtrl->InsertItem(i, &TabCtrlItem);
	}
}

VOID CSE_BINDING_DECISION_04::SetParentSelectedInfo(
													TCHAR *f_DbAlias,        // DB   alias
													TCHAR *f_CtrlAlias,      // CTRL alias
													TCHAR *f_GridAlias,      // GRID alias
													TCHAR *f_pReckey		 // RecKey alias
												)
{
    m_pParDbAlias   = f_DbAlias;
    m_pParCtrlAlias = f_CtrlAlias;
    m_pParGridAlias = f_GridAlias;
    m_pParReckey    = f_pReckey;
}

VOID CSE_BINDING_DECISION_04::InitDisplay()
{
	INT ids;
	CString sMsg;
	CString strTitle_Info;

	m_nPreBookIdx = 0;
	m_bInterBindYn = FALSE;

	CESL_ControlMgr * pCtrlMgr = FindCM(_T("CM_SE_BINDING_DECISION_01"));
	if( !pCtrlMgr ) return;
	
	CESL_Grid *pMainGrid = (CESL_Grid *)pCtrlMgr->FindControl(_T("MainGrid"));
	if( !pMainGrid ) return;
	
	m_nIdx = pMainGrid->SelectGetIdx();
	
	GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("TITLE_INFO"), strTitle_Info, m_nIdx );
	GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("REC_KEY"), m_sSpeciesKey, m_nIdx );

	ids = DoDeleteDataMgr();
	if( ids ) return;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. �������å(�Ʒ�ȭ��) 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = GetBindBookInfo();
	if( ids == -100 ) {

		
		//sMsg.Format( _T("%d��° �ڷᰡ �������å�� �������� �ʽ��ϴ�."), m_nIdx+1 );
		sMsg.Format( _T("[ %s ] �ڷ��� �������å�� �������� �ʽ��ϴ�! \n Ȯ���Ͽ� �ֽʽÿ�."), strTitle_Info);

		AfxMessageBox( sMsg );
		CDialog::OnOK();
		return;
	}

	if( ids ) {
		AfxMessageBox( _T("GetBindBookInfo Error!") );
		return;
	}

	// copy book find
	ids = GroupBookMgr();
	if( ids ) return;

	// bokbon tab init
	InitializeTabCtrl();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 3. ��������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = GetBindFomat();
	if( ids == -100 ) {
	
		//sMsg.Format( _T("%d��° �ڷᰡ ���������� �������� �ʽ��ϴ�."), m_nIdx+1 );
		sMsg.Format( _T("[ %s ] �ڷ��� ���������� �������� �ʽ��ϴ�! \n Ȯ���Ͽ��ֽʽÿ�."), strTitle_Info);

		AfxMessageBox( sMsg );
		CDialog::OnOK();
		return;
	}
	if( ids ) {
		AfxMessageBox( _T("GetBindFomat Error!") );
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ ó�� ����߰��ؾߵ�
	// 4. ����å(��ȭ��)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = GetBindBook();
	if( ids ) {
		AfxMessageBox( _T("GetBindBook Error!") );
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. ������ ���..
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = DisplaySpecies();
	if( ids ) return;	
	
}

VOID CSE_BINDING_DECISION_04::SaveInitDisplay()
{
	
	//�����Ŀ� �ϴ� initial process
	INT ids;
	CString sMsg;
	CString strTitle_Info;


	m_nPreBookIdx = 0;
	m_bInterBindYn = FALSE;

	CESL_ControlMgr * pCtrlMgr = FindCM(_T("CM_SE_BINDING_DECISION_01"));
	if( !pCtrlMgr ) return;
	
	CESL_Grid *pMainGrid = (CESL_Grid *)pCtrlMgr->FindControl(_T("MainGrid"));
	if( !pMainGrid ) return;

	m_nIdx = pMainGrid->SelectGetIdx();
	
	GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("TITLE_INFO"), strTitle_Info, m_nIdx );
	GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("REC_KEY"), m_sSpeciesKey, m_nIdx );


	ids = DoDeleteDataMgr();
	if( ids ) return;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. �������å(�Ʒ�ȭ��) 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = GetBindBookInfo();
	if(-100 == ids) {

		sMsg.Format(_T("[ %s ] �ڷ�� ���̻� ������� å�� �����ϴ�!"),strTitle_Info);
		AfxMessageBox(sMsg);
		
		//å���� �׸��带 �����
		CESL_ControlMgr * pCtrlMgr = FindCM(_T("CM_SE_BINDING_DECISION_04_BOOK"));
		if( !pCtrlMgr ) return;
		
		CESL_Grid *pBookGrid = (CESL_Grid *)pCtrlMgr->FindControl(_T("BookGrid"));
		if( !pBookGrid ) return;

		pBookGrid->Display();

	}

	// copy book find
	GroupBookMgr();

	// bokbon tab init
	InitializeTabCtrl();
		
	GetBindFomat();
	GetBindBook();
	DisplaySpecies();
	
	
	/*
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 3. ��������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = GetBindFomat();
	if( ids == -100 ) {
		//UserMessage( _T("MSG_QUE_052"), 0, MB_OK, 0 );
		sMsg.Format( _T("%d��° �ڷᰡ ���������� �������� �ʽ��ϴ�."), m_nIdx+1 );
		AfxMessageBox( sMsg );
		CDialog::OnOK();
		return;
	}
	if( ids ) {
		AfxMessageBox( _T("GetBindFomat Error!") );
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ ó�� ����߰��ؾߵ�
	// 4. ����å(��ȭ��)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = GetBindBook();
	if( ids ) {
		AfxMessageBox( _T("GetBindBook Error!") );
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. ������ ���..
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = DisplaySpecies();
	if( ids ) return;	
	*/
	
}

// MOD wsw 2003.06.30 copy_code
/*
INT CSE_BINDING_DECISION_04::GroupBookMgr()
{
	INT i, ids, nBookRowCnt;

	CString sCopyCode;

	CESL_DataMgr *pBookDataMgr = FindDM( _T("DM_SE_BOOK_INFO") );
	if( pBookDataMgr == NULL ) return -1;

	nBookRowCnt = pBookDataMgr->GetRowCount();

	for( i = 0 ; i < nBookRowCnt ; i++ ) {

		// DM_SE_BOOK_INFO -> DM_SE_BOOK_INFO_TEMP
		ids = CopyDataManager( _T("DM_SE_BOOK_INFO"), i, _T("DM_SE_BOOK_INFO_TEMP"), -1 );
		if( ids ) return -1;
	}

	ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("COPY_CODE"), sCopyCode, --i );
	if( ids ) return -1;
	
	m_nMaxCopyBookCnt = _ttoi((TCHAR*)LPCTSTR(sCopyCode));

	return 0;
}
*/

INT CSE_BINDING_DECISION_04::GroupBookMgr()
{
	INT i, j, ids, nBookRowCnt;
	INT nCopyCnt;

	CString sCheckYn;//, szDepth[4];
	CString sVolSortNo, sPreVolSortNo;
	CString sDepth;

	CESL_DataMgr *pBookDataMgr = FindDM( _T("DM_SE_BOOK_INFO") );
	if( pBookDataMgr == NULL ) return -1;

	nBookRowCnt = pBookDataMgr->GetRowCount();
	
	nCopyCnt = 0 ;

	for( i = 0 ; i < nBookRowCnt ; i++ ) {

		for( j = i ; j < nBookRowCnt ; j++ ) {

			// first
			if( j == 0 ) {
			
				ids = CopyDataManager( _T("DM_SE_BOOK_INFO"), 0, _T("DM_SE_BOOK_INFO_TEMP"), -1 );
				if( ids ) return -1;

				// check
				ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("CHECK_YN"),  _T("V"), 0 );
				if( ids ) return -1;
				ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("VOL_SORT_NO"),  sVolSortNo, 0 );
				if( ids ) return -1;

				// depth
				sDepth.Format( _T("%d"), i );
				ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("COPY_DEPTH"),  sDepth, 0 );
				if( ids ) return -1;

				nCopyCnt++;
				
				sPreVolSortNo = sVolSortNo;

				continue;
			}

			ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("CHECK_YN"),  sCheckYn, j );
			if( ids ) return -1;

			if( sCheckYn.Left(1) == _T("V") )
				continue;

			ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("VOL_SORT_NO"),  sVolSortNo, j );
			if( ids ) return -1;

			if( sPreVolSortNo == sVolSortNo ) {

				sPreVolSortNo = sVolSortNo;
				continue;
			}
			
			ids = CopyDataManager( _T("DM_SE_BOOK_INFO"), j, _T("DM_SE_BOOK_INFO_TEMP"), -1 );
			if( ids ) return -1;

			ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("CHECK_YN"),  _T("V"), j );
			if( ids ) return -1;

			// depth
			sDepth.Format( _T("%d"), i );
			ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("COPY_DEPTH"),  sDepth, nCopyCnt );
			if( ids ) return -1;

			nCopyCnt++;

			sPreVolSortNo = sVolSortNo;
		}
	}
	
	m_nMaxCopyBookCnt = _ttoi( (TCHAR*)LPCTSTR(sDepth) );

	return 0;
}

INT CSE_BINDING_DECISION_04::CopyDataManager( CString f_sSrcDm, INT f_nSrcIdx, CString f_sDesDm, INT f_nDesIdx )
{
	INT ids, nCurRow, col;

	const INT nAliasCnt = 25;
	CString sAlias[nAliasCnt] = {
		_T("ACQ_CODE"),				//0	
		_T("ACQ_DATE"),				//1	
		_T("ACQ_YEAR"),				//2	
		_T("BINDING_BOOK_KEY"),		//3	
		_T("BIND_INFO"),			//4	
		_T("BIND_INFO_DISP"),		//5	
		_T("BIND_STATUS"),			//6	

		// MOD wsw 2003.06.30 copy_code
		_T("COPY_DEPTH"),			//7	
		//_T("COPY_CODE"),			//7	
		_T("REC_KEY"),				//8	
		_T("REMARK"),				//9	
		_T("SEPARATE_SHELF_CODE"),	//10
		_T("SHELF_CHANGE_DATE"),	//11
		_T("SHELF_DATE"),			//12
		_T("SHELF_LOC_CODE"),		//13
		_T("SPECIES_KEY"),			//14
		_T("USE_LIMIT_CODE"),		//15
		_T("VOL_CODE"),				//16
		_T("VOL_KEY"),				//17
		_T("WORKER"),				//18
		_T("WORKING_STATUS"),		//19
		_T("CHECK_YN"),				//20
		_T("OTHER_VOL_TITLE"),		//21
		_T("PUBLISH_DATE"),			//22
		_T("VOL_SORT_NO"),			//23
		_T("VOL_TITLE")				//24
	};
	CString sData;

	CESL_DataMgr *pDesDataMgr = FindDM(f_sDesDm);
	if( pDesDataMgr == NULL ) return -2;

	pDesDataMgr->InsertRow(-1);
	nCurRow = pDesDataMgr->GetRowCount() - 1;

	for( col = 0; col < nAliasCnt; col++ ) {
		ids = GetDataMgrData( f_sSrcDm, sAlias[col], sData, f_nSrcIdx );
		if( ids < 0 ) return -3;

		ids = SetDataMgrData( f_sDesDm, sAlias[col], sData, nCurRow );
		if( ids < 0 ) return -3;
	}

	/*
	CESL_DataMgr *pSrcDataMgr = FindDM(f_sSrcDm);
	if( pSrcDataMgr == NULL ) return -1;

	CESL_DataMgr *pDesDataMgr = FindDM(f_sDesDm);
	if( pDesDataMgr == NULL ) return -2;

	pDesDataMgr->InsertRow(-1);
	nCurRow = pDesDataMgr->GetRowCount() - 1;

	for( col = 0; col < pSrcDataMgr->m_nCols; col++ )
		pDesDataMgr->SetCellData( nCurRow, col, pSrcDataMgr->GetCellData(f_nSrcIdx, col) );
	*/

	return 0;
}

INT CSE_BINDING_DECISION_04::DisplaySpecies()
{
	INT ids;

	CString sTmp;

	ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("TITLE_INFO"), sTmp, m_nIdx );
	if( ids < 0 ) return -1;
	m_ViewerControl2.SetControlData( _T("��    ��"), sTmp, TRUE );
	
	ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("PUBLISHER"), sTmp, m_nIdx );
	if( ids < 0 ) return -2;	
	m_ViewerControl2.SetControlData( _T("�� �� ��"), sTmp, TRUE );

	ids = GetDataMgrData( _T("DM_SE_BINDING_INFO"), _T("BINDING_TITLE"), sTmp, 0 );
	if( ids < 0 ) return -3;
	m_ViewerControl2.SetControlData( _T("��������"), sTmp, TRUE );
	
	ids = GetDataMgrData( _T("DM_SE_BINDING_INFO"), _T("BINDING_UNIT"), sTmp, 0 );
	if( ids < 0 ) return -4;
	m_ViewerControl2.SetControlData( _T("��������"), sTmp, TRUE );
	
	ids = GetDataMgrData( _T("DM_SE_BINDING_INFO"), _T("BINDING_UNIT_VALUE"), sTmp, 0 );
	if( ids < 0 ) return -5;
	m_ViewerControl2.SetControlData( _T("�� �� ��"), sTmp, TRUE );

	// 
	// SeqNo
	CString sYear, sSeqNo;
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_01"), _T("ACQ_YEAR"), sYear, -1 );
	if( ids < 0 ) return -6;
	
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_01"), _T("SEQNO"), sSeqNo, -1 );
	if( ids < 0 ) return -6;
	
	sTmp = sYear + _T("-") + sSeqNo;
	m_ViewerControl2.SetControlData( _T("��������"), sTmp, TRUE );

	m_ViewerControl2.Display();


	CESL_ControlMgr * pCtrlMgr = FindCM(_T("CM_SE_BINDING_DECISION_04_BOOK"));
	if( !pCtrlMgr ) return -1;
	
	CESL_Grid *pBookGrid = (CESL_Grid *)pCtrlMgr->FindControl(_T("BookGrid"));
	if( !pBookGrid ) return -1;
	
	// Mod wsw 2003.06.28
	/*
	CString sTitle, sPublisher;

	ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("TITLE"), sTitle, m_nIdx );
	if( ids < 0 ) return -1;
	GetDlgItem(IDC_eTITLE)->SetWindowText(sTitle);

	ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("PUBLISHER"), sPublisher, m_nIdx );
	if( ids < 0 ) return -1;
	GetDlgItem(IDC_ePUBLISHER)->SetWindowText(sPublisher);
	*/

	return 0;
}

// DataMgr Delete
INT CSE_BINDING_DECISION_04::DoDeleteDataMgr()
{
	INT i, j, ids, nRowCnt;

	const INT nDelCnt = 4;
	TCHAR* pDataMgrList[nDelCnt] = {
			_T("DM_SE_BIND_BOOK_INFO"),
			_T("DM_SE_BOOK_INFO"),
			_T("DM_SE_BOOK_INFO_TEMP"),
			_T("DM_SE_BINDING_INFO")
	};

	CESL_DataMgr* pDataMgr;

	for( i = 0 ; i < nDelCnt ; i++ ) {

		pDataMgr = FindDM( pDataMgrList[i] );
		if( pDataMgr == NULL ) return -1;

		nRowCnt = pDataMgr->GetRowCount();

		for( j = nRowCnt-1 ; j >= 0 ; j-- ) {
			ids = pDataMgr->DeleteRow(j);
			if( ids <0) return -2;
		}
	}

	return 0;
}

VOID CSE_BINDING_DECISION_04::OnbNEXT() 
{
	INT ids;

	ids = MoveIndex(_NEXT);
	if( ids ) return;
}

VOID CSE_BINDING_DECISION_04::OnbPREV() 
{
	INT ids;

	ids = MoveIndex(_PREV);
	if( ids ) return;
}

INT CSE_BINDING_DECISION_04::MoveIndex(TCHAR to)
{
    INT          nRowIndex, ids;

	CESL_ControlMgr *pCM		= FindCM( _T("CM_SE_BINDING_DECISION_01") );
	CESL_Grid		*pGridMgr	= (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. �⺻ȭ���� index�� ���� ó���� �Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    switch( to ) {
        case _NOMOVE:
            nRowIndex = pGridMgr->SelectGetIdx( );
            if ( nRowIndex < 0 ) return -2;

			if( m_nHeadPosition == nRowIndex ) GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bPREV)->EnableWindow(TRUE);	
			if( m_nTailPosition == nRowIndex ) GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bNEXT)->EnableWindow(TRUE);	
            break;
        case _CURRENT:
			ids = pGridMgr->SelectMakeList();
            if ( ids < 0 ) return -3;
			
			m_nTailPosition = pGridMgr->SelectGetTailPosition();
			m_nHeadPosition = pGridMgr->SelectGetHeadPosition();
            
			if( m_nHeadPosition == m_nTailPosition )GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
			GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);

            break;
        case _PREV:
            nRowIndex = pGridMgr->SelectGetPrev( );
            if ( nRowIndex < 0 ) return -4;
		
			if( m_nHeadPosition == nRowIndex ) GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bPREV)->EnableWindow(TRUE);	
			if( m_nTailPosition == nRowIndex ) GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bNEXT)->EnableWindow(TRUE);	
            break;
        case _NEXT:
            nRowIndex = pGridMgr->SelectGetNext( );
            if ( nRowIndex < 0 ) return -4;
		
			if( m_nHeadPosition == nRowIndex ) GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bPREV)->EnableWindow(TRUE);	
			if( m_nTailPosition == nRowIndex ) GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
			else					  		   GetDlgItem(IDC_bNEXT)->EnableWindow(TRUE);	
            break;
        default:
            return -6;
            break;
    }

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. ���������� ǥ���Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //pGridMgr->SetListIndexSelection( );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 3. ����,���� ��ư�� Enable/Diable
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Prev_Next_ButtonStatus( _T("DISP_DICIDE"), m_pParCtrlAlias, m_pParGridAlias, IDC_bPREV, IDC_bNEXT);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 4. �̵��� ���ڵ� �ε����� ���Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_nIdx = pGridMgr->SelectGetIdx();
    if ( m_nIdx < 0 ) return -8;
	
	pGridMgr->SetReverse(m_nIdx);

	if( to != 'z' ) InitDisplay();

    return 0;
}

INT CSE_BINDING_DECISION_04::DoBindDecideYn( INT f_idx, TCHAR* f_pData )
{
	INT	ids, i, nBookRowCnt;
	CString sBindInfo1, sBindInfo2;

	//..........................................................................................................
	// 1. ����å�� �ݿ�
	//..........................................................................................................
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_STATUS"), (CString)f_pData, f_idx );
	if( ids < 0 ) return -1;

	//..........................................................................................................
	// 2. �������å�� �ݿ� : BIND_INFO �� ���� å�� �ݿ���Ŵ
	//..........................................................................................................
	ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_INFO"), sBindInfo1, f_idx );
	if( ids < 0 ) return -1;

	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BOOK_INFO_TEMP"));
	if( pDataMgr == NULL ) return -1;

	nBookRowCnt = pDataMgr->GetRowCount();

	for( i = 0 ; i < nBookRowCnt ; i++ ) {
		
		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BIND_INFO"), sBindInfo2, i );
		if( ids < 0 ) return -1;

		if( sBindInfo1 == sBindInfo2 ) {
				
			ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BIND_STATUS"), (CString)f_pData, i );
			if( ids < 0 ) return -1;
		}
	}

	return 0;
}

INT CSE_BINDING_DECISION_04::DisplayBookInfo( INT f_nidx, BOOL f_bModYn )
{
	INT ids, i, nBookRowCnt;
	INT nFirstIdx, nLastIdx;

	CString sBindInfo, sBookInfo;

	CESL_DataMgr *pBookDataMgr = FindDM(_T("DM_SE_BOOK_INFO"));
	if( pBookDataMgr == NULL ) return -1;

	nBookRowCnt = pBookDataMgr->GetRowCount();

	//..........................................................................................................
	if( !f_bModYn ) {

		for( i = nBookRowCnt-1 ; i >= 0 ; i-- )
			pBookDataMgr->DeleteRow(i);

		// first idx
		nFirstIdx = GetCopyBookfirstIdx();
		
		// last idx
		nLastIdx = GetCopyBookLastIdx();
		if( nLastIdx == -1 ) return 0;

		for( i = nFirstIdx ; i <= nLastIdx ; i++ ) {
						
			ids = CopyDataManager( _T("DM_SE_BOOK_INFO_TEMP"), i, _T("DM_SE_BOOK_INFO"), -1 );
			if( ids ) return -1;
		}
	}	
	//..........................................................................................................


	//..........................................................................................................
	// ���������� ���� �͸� display�Ѵ�. (bind_info)
	//..........................................................................................................
	ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_INFO"), sBindInfo, f_nidx );
	if( ids < 0 ) return -2;

	//..........................................................................................................
	// �����۾��� �Ͼ ��쿡��..
	//..........................................................................................................
	if( f_bModYn ) {
		ids = AllControlDisplay( _T("CM_SE_BINDING_DECISION_04_BIND"), -1 );
		if( ids < 0 ) return -3;
	}
	ids = AllControlDisplay( _T("CM_SE_BINDING_DECISION_04_BIND"), f_nidx );
	if( ids < 0 ) return -3;

	CESL_ControlMgr * pCtrlMgr = FindCM(_T("CM_SE_BINDING_DECISION_04_BOOK"));
	if( !pCtrlMgr ) return -5;
	
	CESL_Grid *pBookGrid = (CESL_Grid *)pCtrlMgr->FindControl(_T("BookGrid"));
	if( !pBookGrid ) return -6;

	//..........................................................................................................
	// Display BookGrid
	//..........................................................................................................
	INT j, nBookIdx;
	const INT nCol = 5;
	TCHAR *pColAlias[nCol] = {
		_T("BIND_INFO_DISP"),	_T("VOL_TITLE"),	_T("OTHER_VOL_TITLE"),
		_T("PUBLISH_DATE"),		_T("ACQ_DATE")
	};
	CString sData;

	ids = pBookGrid->RemoveAll2();

	if( ids < 0 ) return -7;

	nBookIdx = 0 ;
	
	for( i = 0 ; i < nBookRowCnt ; i++ ) {
		
		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO"), sBookInfo, i );
		if( ids < 0 ) return -7;

		if( sBindInfo == sBookInfo ) {
		
			for( j = 0 ; j < nCol ; j++ ) {

				ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), pColAlias[j], sData, i );
				if( ids ) return -8;

				ids = pBookGrid->SetAt( nBookIdx, j, sData );
				if( ids ) return -8;
			}
			nBookIdx++;
		}
	}

	/*
	//ids = AllControlDisplay( _T("CM_SE_BINDING_DECISION_04_BOOK"), -1 );
    //if( ids < 0 ) return -4;

	for( i = nBookRowCnt-1 ; i >= 0 ; i-- ) {

		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO"), sBookInfo, i );
		if( ids < 0 ) return -7;

		if( _tcscmp( sBindInfo, sBookInfo ) != 0 ) {

			//ids = pBookGrid->RemoveAt(i);
			pBookGrid->RemoveItem(i);
			if( ids < 0 ) return -7;
		}
	}
	*/
	// SetListIndexSelection 
	
	// Resourcefree

	return 0;
}

INT CSE_BINDING_DECISION_04::GetBindBookInfo()
{
	INT ids, nRowCnt;
	CString strQuery;

	strQuery.Format( _T(" V.REC_KEY = B.VOL_KEY AND B.SPECIES_KEY = %s ")
				     _T(" AND B.WORKING_STATUS = 'SEB111N' ")
					 , m_sSpeciesKey );
	
    CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BOOK_INFO"));
	if( pDataMgr == NULL ) {
		AfxMessageBox( _T("Init DM Error!") ) ;
		return -1;
	}

	ids = pDataMgr->RefreshDataManager( strQuery );
	if( ids < 0 ) {
		AfxMessageBox( _T("RefreshDataManager Error!") ) ;
		return -2;
	}

	nRowCnt = pDataMgr->GetRowCount();
	if( nRowCnt <  0 ) return -3;
	if( nRowCnt == 0 ) return -100;

	return 0;
}

INT CSE_BINDING_DECISION_04::GetBindFomat()
{
	INT ids, nRowCnt;
	CString strQuery;

	strQuery.Format( _T(" SPECIES_KEY = %s "),m_sSpeciesKey );
	
    CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_INFO"));
	if( pDataMgr == NULL ) {
		AfxMessageBox( _T("Init DM Error!") ) ;
		return -1;
	}

	ids = pDataMgr->RefreshDataManager( strQuery );
	if( ids < 0 ) {
		AfxMessageBox( _T("RefreshDataManager Error!") ) ;
		return -2;
	}

	nRowCnt = pDataMgr->GetRowCount();
	if( nRowCnt < 0  ) return -1;
	if( nRowCnt == 0 ) return -100;

	// Mod wsw 2003.06.28 
	// �������� ���÷���
	//ids = AllControlDisplay( _T("CM_SE_BINDING_INFO"), 0 );
	//if( ids < 0 ) return -2;


	// binding vol title type
	/*
	ids = ECO_GetDialogManagerData( _T(':'), _T("DB_BINDING_FORMAT"), _T("USE_VOL_TITLE_YN"), 0, szVolTitleYn );
	if( ids ) return -3;

	ids = ECO_GetDialogManagerData( _T(':'), _T("DB_BINDING_FORMAT"), _T("USE_OTHER_VOL_TITLE_YN"), 0, szOtherVolTitleYn );
	if( ids ) return -3;

	if( szVolTitleYn[0] != _T('1') && szOtherVolTitleYn[0] == _T('1') ) {
		m_nBindVolTitleType = 1;
	}
	else if( szVolTitleYn[0] == _T('1') && szOtherVolTitleYn[0] == _T('1') ) {
		m_nBindVolTitleType = 2;
	}
	*/

	return 0;
}

INT CSE_BINDING_DECISION_04::GetBindBook()
{
	INT ids, nBookCnt;
	INT iBookIdx, iBookLastIdx;	// å�ε���(�ر׸���)
	INT iBindBookIdx;			// ����å�ε���(���׸���)

	INT nBind_UnitType;			// å(�μ�)(0), �ִ���(1), ������(2)
	INT nBind_Unit;				// ����������
	
	// ������������ �������������� �о�´�.
	CString sBindingUnit, sBindingUnitValue;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. BINDING_UNIT_TYPE�� å�μ��� ���ؼ���..
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = GetDataMgrData( _T("DM_SE_BINDING_INFO"), _T("BINDING_UNIT"), sBindingUnit, 0 );
	if( ids < 0 ) return -1;

	ids = GetDataMgrData( _T("DM_SE_BINDING_INFO"), _T("BINDING_UNIT_VALUE"), sBindingUnitValue, 0 );
	if( ids < 0 ) return -1;

	nBind_Unit = _ttoi( (TCHAR*)LPCTSTR(sBindingUnitValue));

	if		( sBindingUnit == _T("å(�μ�)") ) nBind_UnitType = 0;
	else if	( sBindingUnit == _T("�ִ���")   ) nBind_UnitType = 1;
	else if	( sBindingUnit == _T("������")   ) nBind_UnitType = 2;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. ������������ �������� �����.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// first idx
	iBookIdx = GetCopyBookfirstIdx();
	m_nPreBookIdx = iBookIdx;
	
	// last idx
	iBookLastIdx = GetCopyBookLastIdx();

	// BookCnt = last idx - first idx + 1
	nBookCnt = iBookLastIdx - iBookIdx + 1;

	iBookLastIdx++;

	for( iBindBookIdx = 0 ; iBookIdx < iBookLastIdx ; iBookIdx++ ) {
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 1. �⺻���� å������ �ֱ� : iBindBookIdx��° INSERT�ϱ�
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ids = DoBaseBookInfo( iBookIdx );
		if( ids ) return -1;
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 2. �������� ���� ����å�� �ֱ�, ����å�� ���� ���� �ֱ�.
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ids = DoBaseBindInfo( iBookIdx, iBindBookIdx, nBind_UnitType, nBind_Unit, nBookCnt );
		if( ids ) return -1;
		
		// ����å�ε��� ����...
		iBindBookIdx++;

		// �������� �����Ѱ��� ���ش�.
		iBookIdx--;
	}

	// ����å ���� ���÷���
	ids = AllControlDisplay( _T("CM_SE_BINDING_DECISION_04_BIND"), -1 );
	if( ids < 0 ) return -1;
	ids = AllControlDisplay( _T("CM_SE_BINDING_DECISION_04_BIND"), 0 );
	if( ids < 0 ) return -1;

	// �������å ���� ���÷���
	ids = DisplayBookInfo( 0 );
	if( ids ) return -1;

	return 0;
}

INT CSE_BINDING_DECISION_04::GetCopyBookfirstIdx()
{
	INT i, ids, nRowCnt;

	CString sCopyCode;

    CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BOOK_INFO_TEMP"));
	if( pDataMgr == NULL ) {
		AfxMessageBox( _T("") ) ;
		return -1;
	}

	nRowCnt = pDataMgr->GetRowCount();

	//m_nBokbonTabIndex
	for( i = 0 ; i < nRowCnt ; i++ ) {

		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("COPY_DEPTH"), sCopyCode , i );
		if( ids < 0 ) return -1;
		
		// MOD wsw 2003.06.30 copy_code
		//ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("COPY_CODE"), sCopyCode , i );
		//if( ids < 0 ) return -1;

		if( _ttoi((TCHAR*)LPCTSTR(sCopyCode)) == m_nBokbonTabIndex ) 
			break;

		// MOD wsw 2003.06.30 copy_code
		//if( _ttoi((TCHAR*)LPCTSTR(sCopyCode))-1 == m_nBokbonTabIndex ) 
		//	break;

	}

	return i;
}

INT CSE_BINDING_DECISION_04::GetCopyBookLastIdx()
{
	INT i, ids, nRowCnt;
	CString sCopyCode;

    CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BOOK_INFO_TEMP"));
	if( pDataMgr == NULL ) {
		AfxMessageBox( _T("") ) ;
		return -1;
	}

	nRowCnt = pDataMgr->GetRowCount();

	//m_nBokbonTabIndex
	for( i = 0 ; i < nRowCnt ; i++ ) {
	
		// MOD wsw 2003.06.30 copy_code
		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("COPY_DEPTH"), sCopyCode , i );
		if( ids < 0 ) return -1;
		//ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("COPY_CODE"), sCopyCode , i );
		//if( ids < 0 ) return -1;

		// MOD wsw 2003.06.30 copy_code
		if( _ttoi( (TCHAR*)LPCTSTR(sCopyCode) ) == (m_nBokbonTabIndex+1) )
			break;
		//if( _ttoi( (TCHAR*)LPCTSTR(sCopyCode) )-1 == (m_nBokbonTabIndex+1) )
		//	break;

	}
	
	return --i;
}

INT CSE_BINDING_DECISION_04::DoBaseBookInfo( INT iBookIdx )
{
	INT i, ids, nRowCnt, nCurRow;

	const INT nAliasCnt = 24;
	TCHAR *pAlias[nAliasCnt] = {
		//book
		_T("REC_KEY"),					//0		NUMERIC,
		_T("SPECIES_KEY"),				//1		NUMERIC,
		_T("VOL_KEY"),					//2		NUMERIC,
		_T("ACQ_CODE"),					//3		STRING,	
		_T("ACQ_YEAR"),					//4		STRING,	
		_T("RECEIPT_NO"),				//5		NUMERIC,
		_T("RECEIPT_SERIAL_NO"),		//6		NUMERIC,
		_T("BOOK_TYPE"),				//7		STRING,
		_T("BINDED_BOOK_YN"),			//8		STRING,
		_T("USE_LIMIT_CODE"),           //9		STRING,	
		_T("MANAGE_CODE"),				//10	STRING,
		_T("INPUT_DATE"),				//11	DATE,
		_T("ACQ_DATE"),					//12	DATE,	
		_T("IMM_LOC_TRANSFER_DATE"),	//13	DATE,	
		_T("SHELF_DATE"),				//14	DATE,	
		_T("SHELF_LOC_CODE"),			//15	STRING,
		_T("SHELF_CHANGE_DATE"),		//16	DATE,	
		_T("SHELF_CHANGE_BEFORE_LOC"),  //17	STRING,
		_T("SHELF_WORKER"),				//18	STRING,
		_T("SHELF_CHANGE_REASON_CODE"),	//19	STRING,
		_T("SHELF_CHANGE_ETC_REASON"),  //20	STRING,
		_T("REG_NO"),					//21	STRING,
		_T("CLASS_NO_TYPE"),			//22	STRING,
		_T("SEPARATE_SHELF_CODE")		//23	STRING,
		//_T("CLASS_NO"),				//24	STRING,
		//_T("BOOK_CODE"),				//25	STRING,
		//_T("VOL_CODE"),				//26	STRING,
		//_T("COPY_CODE"),				//27	STRING,
		//_T("LABEL_TYPE"),				//28	STRING,
		//_T("REMARK"),					//29	STRING,
		//_T("WORKER"),					//30	STRING,
		//_T("BOOK_CHECK_YN")			//31	STRING,
	};

	CString sTemp, sRecKey;

    CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BIND_BOOK_INFO"));
	if( pDataMgr == NULL ) {
		AfxMessageBox( _T("") ) ;
		return -1;
	}
	
	pDataMgr->InsertRow(-1);
	nCurRow = pDataMgr->GetRowCount() - 1;

	// å�� ���� ī���ϱ�.
	for( i = 0 ; i < nAliasCnt ; i++ ) {
		
		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), pAlias[i], sTemp, iBookIdx );
		if( ids < 0 ) return -2;

		ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), pAlias[i], sTemp, nCurRow );
		if( ids < 0 ) return -3;
	}

	// REC_KEY ����
	ids = pDataMgr->MakeRecKey( sRecKey );
	if( ids < 0 ) return -4;

	nRowCnt = pDataMgr->GetRowCount();

	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("REC_KEY"), sRecKey, nRowCnt-1 );
	if( ids < 0 ) return -5;

	// ��Ÿ �׸�� �����ϱ�....

	return 0;
}

INT CSE_BINDING_DECISION_04::GetBookIndex( INT f_iBookIdx, 
										   INT f_iBindBookIdx, 
										   INT f_nBind_UnitType, 
										   INT f_nBind_Unit, 
										   INT f_nBookCnt )
{
	INT i, ids, nUnit;
	CString sPublishDate;

	INT nCurWeek, nFirstWeek;
	CString sCurPubDate, sPrePubDateMon;

	nUnit = f_nBind_Unit;

	nCurWeek = nFirstWeek = 0;

	if( f_nBind_UnitType == 1 ) 
	{

		for( i = f_iBookIdx ; i < f_nBookCnt ; i++ ) {

			ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("PUBLISH_DATE"), sPublishDate, i );
			if( ids < 0 ) return -1;

			sCurPubDate = sPublishDate;

			CTime Time( _ttoi(sCurPubDate.Mid(0,4)),
						_ttoi(sCurPubDate.Mid(5,2)),
						_ttoi(sCurPubDate.Mid(8,2)),0,0,0);
			nCurWeek = Time.GetDayOfWeek();
			
			if( (i != f_iBookIdx) && (nCurWeek == 2) && (--nUnit == 0) ) 
				break;
			
		}

		i--;
	}
	else
	{

		for( i = f_iBookIdx ; i < f_nBookCnt ; i++ ) {
			
			ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("PUBLISH_DATE"), sPublishDate, i );
			if( ids < 0 ) return -2;

			sCurPubDate = sPublishDate;
			
			if( ( i != f_iBookIdx ) && 
				( sCurPubDate.Mid(5,2) != sPrePubDateMon ) )
			{
				nUnit--;

				if( nUnit == 0 ) {
					i;
					break;
				}
			}

			sPrePubDateMon = sCurPubDate.Mid(5,2);
		}

		i--;
	}

	return i;
}

// å�μ��� ����å�� ����
INT CSE_BINDING_DECISION_04::DoBaseBindInfo( INT& f_iBookIdx, 
										 INT f_iBindBookIdx, 
										 INT f_nBind_UnitType, 
										 INT f_nBind_Unit, 
										 INT f_nBookCnt       )
{
	INT ids, i;
	INT nTemp, nBookIdxTerm;	// nBookIdxTerm : ������������ ���� ���� �������å�� ����
	INT nBookIdx;				// �������å�� ���� index(�ؿ� ȭ��)

	CString sVolTitle, sOtherVolTile, sBindingKey;
	CString	strBindVolTitle;	// ������ȣ��
	CString strTempBindVolTitle;//
	CString strBind_Info;		// ��������
	
	nBookIdxTerm = 0;
	
	//=====================================================================================================
	// 1. iBindBookIdx��° ���۱�ȣ �ֱ�	    iBookIdx��° å����
	//=====================================================================================================
	ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("VOL_TITLE"), sVolTitle, f_iBookIdx );
	if( ids < 0 ) return -1;

	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("START_VOL_TITLE"), sVolTitle, f_iBindBookIdx );
	if( ids < 0 ) return -2;

	if( m_nBindVolTitleType == 1 ) {

		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("OTHER_VOL_TITLE"), sOtherVolTile, f_iBookIdx );
		if( ids < 0 ) return -3;

		strBindVolTitle = sOtherVolTile + _T(" - ");
	}
	// vol title + other vol title
	else if( m_nBindVolTitleType == 2 ) {

		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("OTHER_VOL_TITLE"), sOtherVolTile, f_iBookIdx );
		if( ids < 0 ) return -4;

		strBindVolTitle = sVolTitle + sOtherVolTile + _T(" - ");
	}
	// vol title
	else {

		strBindVolTitle = sVolTitle + _T(" - ");
	}

	//=====================================================================================================
	// 2. ����ȣ�� ������� ó�� ��ƾ �ֱ�.. (���������� ��ŭ ���ڶ���..)
	//=====================================================================================================
	if( f_nBind_UnitType == 0 ) {

		nTemp = ( f_iBindBookIdx+1 ) * f_nBind_Unit;
		if( f_nBookCnt < nTemp ) 
			nBookIdxTerm = nTemp - f_nBookCnt;

		nBookIdx = f_iBookIdx+(f_nBind_Unit-1) - nBookIdxTerm;
	}
	else {
		
		nBookIdx = GetBookIndex( f_iBookIdx, f_iBindBookIdx, f_nBind_UnitType, f_nBind_Unit, f_nBookCnt );
		if( nBookIdx < 0 ) return -5;
	}
	
	//=====================================================================================================
	// 2.1. iBindBookIdx��° ���Rȣ �ֱ�			iBookIdx+(nBind_Unit-1)��° å����
	//=====================================================================================================
	ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("VOL_TITLE"), sVolTitle, nBookIdx );
	if( ids < 0 ) return -6;

	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("END_VOL_TITLE"), sVolTitle, f_iBindBookIdx );
	if( ids < 0 ) return -7;
	
	// other vol title
	if( m_nBindVolTitleType == 1 ) {

		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("OTHER_VOL_TITLE"), sOtherVolTile, f_iBookIdx );
		if( ids < 0 ) return -1;

		strTempBindVolTitle = sOtherVolTile;
		strBindVolTitle += strTempBindVolTitle;
	}
	// vol title + other vol title
	else if( m_nBindVolTitleType == 2 ) {

		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("OTHER_VOL_TITLE"), sOtherVolTile, f_iBookIdx );
		if( ids < 0 ) return -8;

		strTempBindVolTitle = sVolTitle + _T(" ") + sOtherVolTile;
		strBindVolTitle += strTempBindVolTitle;
	}
	// vol title
	else {

		strTempBindVolTitle = sVolTitle;
		strBindVolTitle += strTempBindVolTitle;
	}

	//=====================================================================================================
	// 2.2 ������ȣ�� �ֱ�..
	//=====================================================================================================
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("VOL_TITLE"), strBindVolTitle, f_iBindBookIdx );
	if( ids < 0 ) return -9;

	//=====================================================================================================
	// üũ�غ���.. DB���� �������� ���� ID_FLAG -> 'U' �ƴϸ� 'I'��..
	//=====================================================================================================
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("ID_FLAG"), _T("I"), f_iBindBookIdx );
	if( ids < 0 ) return -10;

	//=====================================================================================================
	// 3. ��Ÿ ���� �ֱ�..
	//=====================================================================================================
	
	/*
	// BOOK_TYPE
	// modified by loveisverb(2003-10-01)
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BOOK_TYPE"), _T("B"), f_iBindBookIdx );
	if( ids < 0 ) return -9;
	*/

	//modified by loveisverb 2004-01-28
	// title
	CString sTitle;
	ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("TITLE_INFO"), sTitle, m_nIdx );
	if( ids < 0 ) return -9;

	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("TITLE"), sTitle, f_iBindBookIdx );
	if( ids < 0 ) return -9;

	// BINDING_PRICE
	CString sBindingPrice;

	ids = GetDataMgrData( _T("DM_SE_BINDING_INFO"), _T("BINDING_PRICE"), sBindingPrice, 0 );
	if( ids < 0 ) return -11;

	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_PRICE"), sBindingPrice, f_iBindBookIdx );
	if( ids < 0 ) return -12;

	// �������� �ٲٱ� binding_status -> '1';
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_STATUS"), _T("1"), f_iBindBookIdx );
	if( ids < 0 ) return -13;

	// å���� �ٲٱ� status -> 'SEB112N';
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("WORKING_STATUS"), _T("SEB112N"), f_iBindBookIdx );
	if( ids < 0 ) return -14;

	// BINDED_BOOK_YN (����å����) �ֱ�..
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDED_BOOK_YN"), _T("N"), f_iBindBookIdx );
	if( ids < 0 ) return -15;
	
	// BIND_BOOK PUT BINDING_YEAR
	CString sTodayDate;
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BIND_BOOK_INFO"));
	if( pDataMgr == NULL ) return -1;

	CTime time = pDataMgr->GetDBTime();
	sTodayDate.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );

	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_YEAR"), sTodayDate.Left(4), f_iBindBookIdx );
	if( ids < 0 ) return -16;
	
	// SeqNo
	CString sSeqNo, sOrderNoView;
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_01"), _T("SEQNO"), sSeqNo, -1 );
	if( ids < 0 ) return -17;
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_SEQ_NO"), sSeqNo, f_iBindBookIdx );
	if( ids < 0 ) return -18;
	
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("LAST_NO"), m_sLastSerialNo, f_iBindBookIdx );
	if( ids < 0 ) return -18;

	// ������ȣ
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("RECEIPT_NO"), sSeqNo, f_iBindBookIdx );
	if( ids < 0 ) return -18;

	// �����Ϸù�ȣ
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("RECEIPT_SERIAL_NO"), m_sLastSerialNo, f_iBindBookIdx );
	if( ids < 0 ) return -18;

	//sOrderNoView = sTodayDate.Left(4) + _T("-") + sSeqNo + _T("-") + m_sLastSerialNo;
	// 
	//ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("ORDER_NO_VIEW"), sOrderNoView, f_iBindBookIdx );
	//if( ids < 0 ) return -18;

	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_CONF_DATE"), sTodayDate, f_iBindBookIdx );
	if( ids < 0 ) return -19;

	//=====================================================================================================
	// 4. ���������ֱ�..
	//=====================================================================================================
	strBind_Info.Format( _T("%d"), f_iBindBookIdx );
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_INFO"), strBind_Info, f_iBindBookIdx );
	if( ids < 0 ) return -20;

	// binding_key
	ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("REC_KEY"), sBindingKey, f_iBindBookIdx );
	if( ids < 0 ) return -21;


	//=====================================================================================================
	// 5. å������ ���� ���� �ֱ� binding_info(��������) = iBindBookIdx�� ��
	//=====================================================================================================
	CString sBookKeys, sTemp;
	for( i = m_nPreBookIdx ; i <= nBookIdx  ; i++ ) {

		// f_iBookIdx å���� ������������ŭ ������
		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BIND_INFO"), strBind_Info, f_iBookIdx );
		if( ids < 0 ) return -22;

		// ����åŰ �ֱ�..
		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BINDING_BOOK_KEY"), sBindingKey, f_iBookIdx );
		if( ids < 0 ) return -23;

		// å���� �ֱ�.. status -> 'SEB112N'
		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("WORKING_STATUS"), _T("SEB112N"), f_iBookIdx );
		if( ids < 0 ) return -24;
	
		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("REC_KEY"), sTemp, f_iBookIdx );
		if( ids < 0 ) return -25;

		if( i == m_nPreBookIdx )	sBookKeys = sTemp;
		else						sBookKeys += _T(",") + sTemp;

		f_iBookIdx++;
	}

	// BOOK_KEYS
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BOOK_KEYS"), sBookKeys, f_iBindBookIdx );
	if( ids < 0 ) return -26;

	m_nPreBookIdx = nBookIdx + 1;

	return 0;
}

VOID CSE_BINDING_DECISION_04::OnbSAVE() 
{
	INT ids;
	BOOL bOnlyRegYn = FALSE;
	
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_01"));
	if( pDataMgr == NULL ) return;

	// �׸��忡�� ������ ���� �޸𸮿� 
	ids = DoGirdToMem();
	if( ids < 0 ) return;

	// 0. check proc
	ids = CheckSelectedYn();
	if( ids == -100 ) {
		bOnlyRegYn = TRUE;
	}
	else if( ids ) {
		AfxMessageBox( _T("������ ������ �����ϴ�!") );
		return;
	}

	pDataMgr->StartFrame();

	// 1. ������ȣ ����
	ids = DoSaveBindingVol();
	if( ids ) return;

	// 2. ����å ����
	ids = DoSaveBindingBook();
	if( ids ) return;

	// 3. �������� ����
	ids = DoSaveBindingInfo();
	if( ids ) return;

	// 4. �������å ����
	ids = DoSaveBook();
	if( ids ) return;

	// 5. ����������ȣ ����
	ids = DoSaveSeq();
	if( ids ) return;

	ids = pDataMgr->SendFrame();
    pDataMgr->EndFrame();
	if( ids < 0 ) {
        AfxMessageBox(_T("���忡 �����Ͽ����ϴ�!"));
        return;
    }

	SaveInitDisplay();

	AfxMessageBox( _T("����Ǿ����ϴ�.") );
}

INT CSE_BINDING_DECISION_04::DoGirdToMem()
{
	INT i, nBindRowCnt, ids;
	CString sTmp;

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_DECISION_04_BIND") );
	CESL_Grid		*pBindGrid = (CESL_Grid*) pCM->FindControl ( _T("BindGrid") ) ;


	CESL_DataMgr *pBindDataMgr = FindDM(_T("DM_SE_BIND_BOOK_INFO"));
	if( pBindDataMgr == NULL ) return -1;

	nBindRowCnt = pBindDataMgr->GetRowCount();

	for( i = 0 ; i < nBindRowCnt ; i++ ) {
		// Mod 2003.06.28 wsw 
		// ������ȣ��
		pBindGrid->GetAt( i, 3, sTmp );
		ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("VOL_TITLE"), sTmp, i );
		if( ids < 0 ) return -2;

		// ���
		pBindGrid->GetAt( i, 4, sTmp );
		ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("REMARK"), sTmp, i );
		if( ids < 0 ) return -3;
	}
	return 0;
}

INT CSE_BINDING_DECISION_04::DoSaveSeq()
{
	INT ids, nRowCnt;

	CString sAcqYear, sSeqNo;

	// frame ��
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_01"));
	if( pDataMgr == NULL ) return -1;

	CESL_DataMgr *pBindBookDataMgr = FindDM(_T("DM_SE_BIND_BOOK_INFO"));
	if( pBindBookDataMgr == NULL ) return -1;

	nRowCnt = pBindBookDataMgr->GetRowCount();

	if( nRowCnt < 0 ) return -1;

//	ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("ACQ_YEAR"), sAcqYear, 0 );
//	if( ids < 0 ) return -2;

	ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_SEQ_NO"), sSeqNo, 0 );
	if( ids < 0 ) return -3;

	if(/* sAcqYear.IsEmpty() || */sSeqNo.IsEmpty() ) 
		return -4;
	
	CString sSql;
	/*sSql.Format( _T("UPDATE SE_BINDING_SEQ_TBL SET WORKING_STATUS = '0', LAST_SERIAL_NO = '%s' ")
				 _T("WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s;"),
				 m_sLastSerialNo, sAcqYear, sSeqNo ); */
	sSql.Format( _T("UPDATE SE_BINDING_SEQ_TBL SET WORKING_STATUS = '0', LAST_SERIAL_NO = '%s' ")
				 _T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s;"),
				 m_sLastSerialNo, m_sAcqYear, sSeqNo );
	
//  modified by loveisverb 2004-01-28

//	pDataMgr->StartFrame();
	pDataMgr->AddFrame(sSql);
//	ids = pDataMgr->SendFrame();
//	pDataMgr->EndFrame();

//	if( ids < 0 ) return -5;

	return 0;
}

// 
INT CSE_BINDING_DECISION_04::CheckSelectedYn()
{
	INT ids, i, nRowCnt;
	BOOL bRegYn, bDesYn;
	CString sSelected;

	bRegYn = bDesYn = FALSE;

	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BIND_BOOK_INFO"));
	if( pDataMgr == NULL ) return -1;

	nRowCnt = pDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ ) {

		ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_STATUS"), sSelected, i );
		if( ids < 0 ) return -1;
		
		if		( sSelected == _T("����") )	bDesYn = TRUE;
		else if	( sSelected == _T("R") )	bRegYn = TRUE;
	}

	if		( (bRegYn == FALSE) && (bDesYn == FALSE) ) 	return -1;
	else if	( (bRegYn == TRUE)  && (bDesYn == FALSE) ) 	return -100;

	return 0;
}

INT CSE_BINDING_DECISION_04::GetBookCntOFBind( INT f_nIndex, INT &f_nBookCnt )
{
	INT ids, nBookRowCnt, i;
	CString sBindStatus1, sBindStatus2;

	ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_INFO"), sBindStatus1, f_nIndex );
	if( ids < 0 ) return -1;

	CESL_DataMgr *pBookMgr = FindDM(_T("DM_SE_BOOK_INFO"));
	if( pBookMgr == NULL ) return -2;

	nBookRowCnt = pBookMgr->GetRowCount();
	f_nBookCnt = 0;

	for( i = 0 ; i < nBookRowCnt ; i++ ) {
		
		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO"), sBindStatus2, i );
		if( ids < 0 ) return -3;

		if( sBindStatus1 == sBindStatus2 )
			f_nBookCnt++;
	}

	return 0;
}

INT CSE_BINDING_DECISION_04::DoSaveBindingVol()
{
	const INT nFieldCnt = 17;
	TCHAR* pFieldAlias[nFieldCnt] = {
		// vol
		_T("VOL_KEY"),					//0		NUMERIC,
		_T("SPECIES_KEY"),				//1		NUMERIC,
		_T("VOL_TYPE"),					//2		STRING
		_T("TITLE"),					//3		STRING,
		_T("FORM_CODE"),				//4		STRING,
		_T("MEDIA_CODE"),				//5		STRING,
		_T("VOL_TITLE"),				//6		STRING,
		_T("IDX_VOL_TITLE"),			//7		STRING,
		_T("PUBLISH_DATE"),				//8		STRING,
		_T("CURRENCY_CODE"),			//9		STRING,
		_T("PRICE"),					//10	NUMERIC,
		_T("PRICE_CHARACTER"),			//11	STRING,
		_T("PAGE"),						//12	STRING,
		_T("PHYSICAL_PROPERTY"),		//13	STRING,
		_T("BOOK_SIZE"),				//14	STRING,
		_T("ACCOMP_MAT"),				//15	STRING,
		_T("REMARK")					//16	STRING,
	};

	TCHAR* pFieldType[nFieldCnt] = {
		_T("NUMERIC"),//0	
		_T("NUMERIC"),//1	
		_T("STRING"),//2	
		_T("STRING"),//3	
		_T("STRING"),//4	
		_T("STRING"),//5	
		_T("STRING"),//6	
		_T("STRING"),//7	
		_T("STRING"),//8	
		_T("STRING"),//9	
		_T("NUMERIC"),//10
		_T("STRING"),//11
		_T("STRING"),//12
		_T("STRING"),//13
		_T("STRING"),//14
		_T("STRING"),//15
		_T("STRING")//16
	};
					
	INT ids, i, j, nRowCnt;

	CString sFlag1, sFlag2;
	CString sTodayDate, sBindingPubYear, sData, sRecKey;

	//
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_01"));
	if( pDataMgr == NULL ) return -1;

	CESL_DataMgr *pVolDataMgr = FindDM(_T("DM_SE_BIND_BOOK_INFO"));
	if( pDataMgr == NULL ) return -1;

	nRowCnt = pVolDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++) {

		//
		pDataMgr->InitDBFieldData();

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ID_FLAG : I(insert), U(update) and BIND_STATUS = '����' ���ڷῡ ���ؼ��� �۾���
		// ���� �˼������ �ڷᰡ ���������� �ڷῡ�� ���̰� ���� �ʾ���.. ID_FLAG�� U�� �Ǵ°��
		// : -> �˼��ʿ� ���ͱ������ ��ü(��Ȯ��)
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("ID_FLAG"), sFlag1, i );
		if( ids < 0 ) return -8;
		
		ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_STATUS"), sFlag2, i );
		if( ids < 0 ) return -9;

		// �ű� ���������ڷ�
		if( sFlag1 == _T("I") && sFlag2 == _T("����") ) {

			ids = pVolDataMgr->MakeRecKey( sRecKey );
			if( ids < 0 ) return -1;

			ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("VOL_KEY"), sRecKey, i );
			if( ids < 0 ) return -1;
			
			ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("VOL_TYPE"), _T("B"), i );
			if( ids < 0 ) return -1;
		}
		// ���������� �ڷᵵ ���̰� �ϸ� �̺κ� �߰��������..
		else if( sFlag1 == _T("U") && sFlag2 == _T("����") ){

		}
		// ���ǵ���ڷ��ΰ��..
		else if( sFlag1 == _T("I") && sFlag2 == _T("R") ){

			break;
		}

		// ������ȣ �����
		for( j = 0 ; j < nFieldCnt ; j++ ) {
		
			ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), pFieldAlias[j], sData, i );
			if( ids < 0 ) return -2;
			
			// REC_KEY
			if( j == 0 ) {
			
				sRecKey = sData;
								
				ids = pDataMgr->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), sRecKey );
				if( ids < 0) return -3;

				continue;
			}
			
			//loveisverb 2004-05-17 �߰�
			if(pFieldAlias[j] == _T("MEDIA_CODE")) //������ȣ�� ��ü���� �߰�
			{
				ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("MEDIA_CODE"), sData, m_nIdx );
				if( ids < 0) return -1;
			}

			// Query �� DB Field�� �߰��Ѵ�.
			ids = pDataMgr->AddDBFieldData( pFieldAlias[j], pFieldType[j], sData );
			if( ids < 0) return -3;
		}

		// �ű� ���������ڷ�
		if( sFlag1 == _T("I") && sFlag2 == _T("����") ) {

			ids = pDataMgr->MakeInsertFrame( _T("SE_VOL_TBL") );
			if( ids < 0 ) return -11;
		}
		// �����˼��ڷ�� ������ �����Ƿ� ���� �ʿ����..
		else if( sFlag1 == _T("U") && sFlag2 == _T("����") ){

			ids = pDataMgr->MakeUpdateFrame( _T("SE_VOL_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
			if( ids < 0 ) return -12;
		}
		// ���ǵ���ڷ��ΰ��..
		else if( sFlag1 == _T("I") && sFlag2 == _T("R") ){

			ids = DoRegProc( i );
			if( ids ) return -13;
		}
	}

	return 0;
}

INT CSE_BINDING_DECISION_04::DoSaveBindingInfo()
{
	const INT nFieldCnt = 9;
	TCHAR* pFieldAlias[nFieldCnt] = {
		//book
		_T("BINDING_INFO_KEY"),			//0		NUMERIC,
		_T("REC_KEY"),					//1		NUMERIC,
		_T("BINDING_STATUS"),			//2		STRING,
		_T("BINDING_YEAR"),				//3		STRING,
		_T("BINDING_SEQ_NO"),			//4		NUMERIC,
		_T("BOOK_KEYS"),				//5		STRING,
		_T("BINDING_PRICE"),			//6		NUMERIC,
		_T("BINDING_CONF_DATE"),		//7		STRING,
		_T("REMARK")					//8		STRING,
	};									

	TCHAR* pFieldType[nFieldCnt] = {
		_T("NUMERIC"),
		_T("NUMERIC"),
		_T("STRING"),
		_T("STRING"),
		_T("NUMERIC"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING")
	};						
					
	INT ids, i, j, nRowCnt;

	CString sFlag1, sFlag2;
	CString sTodayDate, sBindingPubYear, sData, sRecKey;

	// frame ��
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_01"));
	if( pDataMgr == NULL ) return -1;

	//
	CESL_DataMgr *pBindDataMgr = FindDM(_T("DM_SE_BIND_BOOK_INFO"));
	if( pDataMgr == NULL ) return -1;

	nRowCnt = pBindDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++) {

		//
		pDataMgr->InitDBFieldData();

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ID_FLAG : I(insert), U(update) and BIND_STATUS = '����' ���ڷῡ ���ؼ��� �۾���
		// ���� �˼������ �ڷᰡ ���������� �ڷῡ�� ���̰� ���� �ʾ���.. ID_FLAG�� U�� �Ǵ°��
		// : -> �˼��ʿ� ���ͱ������ ��ü(��Ȯ��)
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("ID_FLAG"), sFlag1, i );
		if( ids < 0 ) return -8;
		
		ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_STATUS"), sFlag2, i );
		if( ids < 0 ) return -9;

		// �ű� ���������ڷ�
		if( sFlag1 == _T("I") && sFlag2 == _T("����") ) {

			ids = pBindDataMgr->MakeRecKey( sRecKey );
			if( ids < 0 ) return -1;

			ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_INFO_KEY"), sRecKey, i );
			if( ids < 0 ) return -1;
		}
		// �����˼��ڷ�� ������ �����Ƿ� ���� �ʿ����..
		else if( sFlag1 == _T("U") && sFlag2 == _T("����") ){

		}
		// ���ǵ���ڷ��ΰ��..
		else if( sFlag1 == _T("I") && sFlag2 == _T("R") ){

			break;
		}

		// ������ȣ �����
		for( j = 0 ; j < nFieldCnt ; j++ ) {
		
			ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), pFieldAlias[j], sData, i );
			if( ids < 0 ) return -2;

			if( j == 0 ) {
			
				sRecKey = sData;
								
				ids = pDataMgr->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), sRecKey );
				if( ids < 0) return -3;

				continue;
			}
			else if ( j == 1 ){
			
				ids = pDataMgr->AddDBFieldData( _T("BOOK_KEY"), _T("NUMERIC"), sData );
				if( ids < 0) return -3;

				continue;
			}

			// Query �� DB Field�� �߰��Ѵ�.
			ids = pDataMgr->AddDBFieldData( pFieldAlias[j], pFieldType[j], sData );
			if( ids < 0) return -3;
		}

		// �ű� ���������ڷ�
		if( sFlag1 == _T("I") && sFlag2 == _T("����") ) {

			ids = pDataMgr->MakeInsertFrame( _T("SE_BINDING_BOOK_TBL") );
			if( ids < 0 ) return -11;
		}
		// �����˼��ڷ�� ������ �����Ƿ� ���� �ʿ����..
		else if( sFlag1 == _T("U") && sFlag2 == _T("����") ){

			ids = pDataMgr->MakeUpdateFrame( _T("SE_BINDING_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
			if( ids < 0 ) return -12;
		}
		// ���ǵ���ڷ��ΰ��..
		else if( sFlag1 == _T("I") && sFlag2 == _T("R") ){

			ids = DoRegProc( i );
			if( ids ) return -13;
		}
	}

	return 0;
}

INT CSE_BINDING_DECISION_04::DoSaveBindingBook()
{
	const INT nFieldCnt = 25;
	TCHAR* pFieldAlias[nFieldCnt] = {
		//book
		_T("REC_KEY"),					//0		NUMERIC,
		_T("SPECIES_KEY"),				//1		NUMERIC,
		_T("VOL_KEY"),					//2		NUMERIC,
		_T("ACQ_CODE"),					//3		STRING,	
		_T("ACQ_YEAR"),					//4		STRING,	
		_T("RECEIPT_NO"),				//5		NUMERIC,
		_T("RECEIPT_SERIAL_NO"),		//6		NUMERIC,
		_T("BOOK_TYPE"),				//7		STRING,
		_T("BINDED_BOOK_YN"),			//8		STRING,
		_T("USE_LIMIT_CODE"),           //9		STRING,	
		_T("MANAGE_CODE"),				//10	STRING,
		_T("INPUT_DATE"),				//11	DATE,
		_T("ACQ_DATE"),					//12	DATE,	
		_T("IMM_LOC_TRANSFER_DATE"),	//13	DATE,	
		_T("SHELF_DATE"),				//14	DATE,	
		_T("SHELF_LOC_CODE"),			//15	STRING,
		_T("SHELF_CHANGE_DATE"),		//16	DATE,	
		_T("SHELF_CHANGE_BEFORE_LOC"),  //17	STRING,
		_T("SHELF_WORKER"),				//18	STRING,
		_T("SHELF_CHANGE_REASON_CODE"),	//19	STRING,
		_T("SHELF_CHANGE_ETC_REASON"),  //20	STRING,
		_T("REG_NO"),					//21	STRING,
		_T("CLASS_NO_TYPE"),			//22	STRING,
		_T("SEPARATE_SHELF_CODE"),		//23	STRING,
		_T("REG_CODE")					//24	STRING,
		//_T("CLASS_NO"),				//24	STRING,
		//_T("BOOK_CODE"),				//25	STRING,
		//_T("VOL_CODE"),				//26	STRING,
		//_T("COPY_CODE"),				//27	STRING,
		//_T("LABEL_TYPE"),				//28	STRING,
		//_T("REMARK"),					//29	STRING,
		//_T("WORKER"),					//30	STRING,
		//_T("BOOK_CHECK_YN")			//31	STRING,
	};
	TCHAR* pFieldType[nFieldCnt] = {
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),	
		_T("STRING"),	
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),	
		_T("STRING"),	
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),	
		_T("STRING"),	
		_T("STRING"),	
		_T("STRING"),
		_T("STRING"),	
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING")
		//_T("STRING"),
		//_T("STRING"),
		//_T("STRING"),
		//_T("STRING"),
		//_T("STRING"),
		//_T("STRING"),
		//_T("STRING"),
		//_T("STRING")
	};

	INT ids, i, j, nRowCnt;

	CString sFlag1, sFlag2;
	CString sTodayDate, sBindingPubYear, sData, sRecKey;

	// frame ��
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_01"));
	if( pDataMgr == NULL ) return -1;

	//
	CESL_DataMgr *pBindBookDataMgr = FindDM(_T("DM_SE_BIND_BOOK_INFO"));
	if( pBindBookDataMgr == NULL ) return -1;

	// ���ó�¥�� ���Ѵ�.
	CTime time = pBindBookDataMgr->GetDBTime();
	sTodayDate.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );

	nRowCnt = pBindBookDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++) {

		pDataMgr->InitDBFieldData();

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ID_FLAG : I(insert), U(update) and BIND_STATUS = '����' ���ڷῡ ���ؼ��� �۾���
		// ���� �˼������ �ڷᰡ ���������� �ڷῡ�� ���̰� ���� �ʾ���.. ID_FLAG�� U�� �Ǵ°��
		// : -> �˼��ʿ� ���ͱ������ ��ü(��Ȯ��)
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("ID_FLAG"), sFlag1, i );
		if( ids < 0 ) return -2;
		
		ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_STATUS"), sFlag2, i );
		if( ids < 0 ) return -3;

		if( sFlag2 == _T("") ) {
			continue;
		}

		// ���ǵ���ڷ��ΰ��..
		else if( sFlag1 == _T("I") && sFlag2 == _T("R") ){

			ids = DoRegProc( i );
			if( ids ) return -4;
		}

		// ����å �����
		for( j = 0 ; j < nFieldCnt ; j++ ) {
		
			ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), pFieldAlias[j], sData, i );
			if( ids < 0 ) return -5;

			if( j == 0 ) sRecKey = sData;
			
			//loveisverb 2004-05-17 �߰�
			if(pFieldAlias[j] == _T("REG_CODE")) //����å�� ��ϱ��� ����
			{
				ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("REG_CODE"), sData, m_nIdx );
				if( ids < 0) return -1;
			}
			else if(pFieldAlias[j] == _T("MANAGE_CODE")) //����å�� �������� ����
			{
				ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("MANAGE_CODE"), sData, m_nIdx );
				if( ids < 0) return -1;
			}
			else if(pFieldAlias[j] == _T("BOOK_TYPE")) //����å�� å���� ����
			{
				sData = _T("B");
			}
			else if(pFieldAlias[j] == _T("ACQ_YEAR")) //�������� �⵵
			{
				sData = m_sAcqYear;
			}
			//2018�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
			//KOL.RED.019-007 JY ���Ӱ��๰ ��� ����ȭ
			//=============================================================================================
			else if(pFieldAlias[j] == _T("INPUT_DATE")){
				sData = sTodayDate;
			}
			//=============================================================================================

			// Query �� DB Field�� �߰��Ѵ�.
			ids = pDataMgr->AddDBFieldData( pFieldAlias[j], pFieldType[j], sData );
			if( ids < 0) return -6;
		}

		// WORKING_STATUS
		ids = pDataMgr->AddDBFieldData( _T("WORKING_STATUS"), _T("STRING"), _T("SEB112N") );
		if( ids < 0) return -7;

		// PREV_WORKING_STATUS
		ids = pDataMgr->AddDBFieldData( _T("PREV_WORKING_STATUS"), _T("STRING"), _T("SEB111N") );
		if( ids < 0) return -8;

		// RECEIPT_SERIAL_NO
		//ids = pDataMgr->AddDBFieldData( _T("RECEIPT_SERIAL_NO"), _T("STRING"), m_sLastSerialNo );
		//if( ids < 0) return -8;

		// �ű� ���������ڷ�
		if( sFlag1 == _T("I") && sFlag2 == _T("����") ) {

			ids = pDataMgr->MakeInsertFrame( _T("SE_BOOK_TBL") );
			if( ids < 0 ) return -9;
		}
		// ���������ڷ�� ������ �����Ƿ� ���� �ʿ����..
		else if( sFlag1 == _T("U") && sFlag2 == _T("����") ){

			ids = pDataMgr->MakeUpdateFrame( _T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
			if( ids < 0 ) return -10;
		}
	}

	return 0;
}

// ���ǵ�� ���ν���
INT CSE_BINDING_DECISION_04::DoRegProc( INT f_nIdx )
{
	INT ids, i, nRowCnt;
	
	CString sBookInfo;	// DM_SE_BIND_BOOK_INFO DataMgr�� BIND_INFO
	CString sBindInfo;	// DM_SE_BOOK_INFO DataMgr�� BIND_INFO
	CString sQuery, sRecKey;

	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BIND_BOOK_INFO"));
	if( pDataMgr == NULL ) return -1;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DM_SE_BIND_BOOK_INFO�� BIND_INFO == DM_SE_BOOK_INFO�� BIND_INFO �� DM_SE_BOOK_INFO�� �ڷḦ update
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_INFO"), sBookInfo, f_nIdx );
	if( ids < 0 ) return -2;

	nRowCnt = pDataMgr->GetRowCount();
	

	for( i = 0 ; i < nRowCnt ; i++ ) {

		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BIND_INFO"), sBindInfo, i );
		if( ids < 0 ) return -3;			

		if( sBookInfo == sBindInfo ) {
		
			ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("REC_KEY"), sRecKey, i );
			if( ids < 0 ) return -4;
	
			sQuery.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SER111N', BOOK_TYPE = 'C' WHERE REC_KEY = %s;")
				            , sRecKey );

			pDataMgr->StartFrame();
			pDataMgr->AddFrame(sQuery);
			ids = pDataMgr->SendFrame();
			pDataMgr->EndFrame();

			if( ids < 0 ) return -5;

			break;
		}
	}

	return 0;
}

INT CSE_BINDING_DECISION_04::DoSaveBook()
{
	const INT nFieldCnt = 3;
	TCHAR* pFieldAlias[nFieldCnt] = {
			_T("REC_KEY"),
			_T("BINDING_BOOK_KEY"),
			_T("WORKING_STATUS")			
	};

	TCHAR* pFieldType[nFieldCnt] = {
			_T("STRING"),
			_T("STRING"),
			_T("STRING")
	};

	INT i, j, ids, nRowCnt;
	CString sData, sRecKey, sFlag;

	// frame ��
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_01"));
	if( pDataMgr == NULL ) return -1;
	
	//
	CESL_DataMgr *pBookDataMgr = FindDM(_T("DM_SE_BOOK_INFO_TEMP"));
	if( pBookDataMgr == NULL ) return -1;

	nRowCnt = pBookDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ ) {

		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BIND_STATUS"), sFlag, i );
		if( ids < 0 ) return -2;

		if( sFlag == _T("����") ) {

			//
			pDataMgr->InitDBFieldData();

			for( j = 0 ; j < nFieldCnt ; j++ ) {

				ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), pFieldAlias[j], sData, i );
				if( ids < 0 ) return -3;

				// Query �� DB Field�� �߰��Ѵ�.
				ids = pDataMgr->AddDBFieldData( pFieldAlias[j], pFieldType[j], sData );
				if( ids < 0) return -4;

				if( j == 0 ) sRecKey = sData;
			}
			
			ids = pDataMgr->AddDBFieldData( _T("BINDED_BOOK_YN"), _T("STRING"), _T("Y") );
			if( ids < 0) return -4;
			
			ids = pDataMgr->MakeUpdateFrame( _T("SE_BOOK_TBL"), _T("REC_KEY"), _T("STRING"), sRecKey );
			if( ids < 0 ) return -5;
		}
	}

	return 0;
}

INT CSE_BINDING_DECISION_04::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}

VOID CSE_BINDING_DECISION_04::Print_SetLastPageLine(VOID)
{
	INT ids;

	// XML �� WorkForm��ü�� �����Ѵ�.
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	
	if (pXMLIOMgr == NULL) {
		AfxMessageBox(_T("XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}
	if (pPageWorkForm == NULL) {
		AfxMessageBox(_T("�߰� XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}

	// ���� ������ ������ ��ȣ�� �����´�.
	INT nMaxTmpPageNum = pXMLIOMgr->GetMaxTmpPageNum();
	if (nMaxTmpPageNum <= 0) {
		AfxMessageBox(_T("������ ������ ã�� �� �����ϴ�."));
		return;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// ���� ������ ������ ������ �ҷ��´�.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) {
		AfxMessageBox(_T("�������� �ε� ���� ���Ͽ����ϴ�."));
		return;
	}
	// �ҷ��� WorkForm�߿��� First Sheet�� �����´�.(�߰� ������ ���Ͽ��� Sheet�� �Ѱ����̴�.)
	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) {
		AfxMessageBox(_T("�������� ã�� �� �����ϴ�."));
		return;
	}
	// ������ ������ ó���� �̹� �Ǿ� �ִ��� ����. �̹� ó���Ǿ� �ִٸ� �ι� �� �ʿ� ����.
	CSIReportObject * pObject = pSheet->FindObject(_T("�߰��ȸ�����������"));
	if (pObject) return;

	// �ܺ����̺� �� �ݺ� ���̺��� �����´�.(���̺� �̸��� ��Ȳ�� ���� ������ ��)
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_33"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_6"));
	if (pIn == NULL) return;
	if (pOut == NULL) return;

	CRect InLeft, InRight, OutLeft, OutRight, NewRect;

	CSIReportObject_Table_Cell *pCell;

	// �ݺ����̺��� ������ ������ ���� ó���� ������ ���������� Rect�� ���ϰ�
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, 0);
	if (pCell) InLeft = pCell->GetRect();
	
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, pIn->col_count-1);
	if (pCell) InRight = pCell->GetRect();

	// �ܺ����̺��� ������ ������ ���� ó�� ���� Rect�� ���Ѵ�.
	pCell = NULL;
	pCell = pOut->GetCell(pOut->row_count-1, 0);
	if (pCell) OutLeft = pCell->GetRect();

	// �ܺ����̺��� �ݺ����̺��� ������ ������ �������� �߰����ش�.
	if (InLeft.bottom < OutLeft.bottom) {
		// ������ �������� Rect�� ����Ѵ�.
		NewRect.top = InLeft.bottom;
		NewRect.bottom = InLeft.bottom;
		NewRect.left = InLeft.left;
		NewRect.right = InRight.right;

		// ������ �������� �߰� ������ ���Ͽ� �߰��Ѵ�.
		CSIReportObject_Line *pLine = (CSIReportObject_Line *)pSheet->AddNewObject(OBJECT_LINE);
		if (pLine) {
			pLine->SetName(_T("�߰��ȸ�����������"));
			pLine->SetRect(NewRect);
		}

		// ����� �߰������� ������ �����Ѵ�.
		ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	}

	// ������ Object�� ����
	if (pXMLIOMgr) {
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}
	if (pPageWorkForm) {
		delete pPageWorkForm;
		pPageWorkForm = NULL;
	}
	return;
}

VOID CSE_BINDING_DECISION_04::Print_List() 
{
  INT ids;
  CString ReportAlias ;
  ReportAlias = _T("K2UP_������ǥ");

  CESL_DataMgr *pDM = FindDM( _T("DM_SE_BINDING_DECISION_04_PRINT") );
	if(pDM == NULL) {
		AfxMessageBox (_T("FindDM() Error~!"));
		return ;
	}

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return ;
	}
	
	
	pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(ReportAlias);
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return ;
	}

	//
	pSIReportManager->m_nDefaultPaperType = PAPER_A4_PORTRAIT;
	ids = pSIReportManager->DeleteTempPageFiles();

	CSIReportWorkSheet *pSheet = pSIReportManager->GetSheet(0);

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ ) {
		CString sTmp;
		sTmp.Format( _T("%d"), i );
		
		pSIReportManager->SetTempVariable( _T("����") , pDM->GetCellData( _T("NO"), i ) );
		pSIReportManager->SetTempVariable( _T("����") , pDM->GetCellData( _T("TITLE"), i ) );
		pSIReportManager->SetTempVariable( _T("��ȣ") , pDM->GetCellData( _T("VOL_TITLE"), i ) );
		pSIReportManager->SetTempVariable( _T("å��") , pDM->GetCellData( _T("BOOK_CNT"), i ) );
		pSIReportManager->SetTempVariable( _T("���") , pDM->GetCellData( _T("REMARK"), i ) );
		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// ��ũ�� Manual �ݺ� ���
	}	

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
    // ���� �Է� ///////////////////////////////////////////////////////////////
	
	//pSIReportManager->SetDataMgr(0, pDM);


	//PRINT//////////////////////////////////////////////////////////////////////////
    /*
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
	*/
   
/*
	ids = pSIReportManager->MakeXMLPageFiles();
	// ������ ������ �׸�
    Print_SetLastPageLine();
	ids = pSIReportManager->Print2();
    if (pSIReportManager) {
	delete pSIReportManager;
	pSIReportManager = NULL;
    }
*/
	return ;
}

VOID CSE_BINDING_DECISION_04::OnbBINDINGPRINT() 
{
	INT ids = MakePrintDM();
	if( ids ) return;

	Print_List();
}

INT CSE_BINDING_DECISION_04::MakePrintDM()
{
	INT i, ids, nRowCnt, nCurRow;
	CString sYear, sNo, sData;

	CESL_DataMgr *pPrintDM = FindDM(_T("DM_SE_BINDING_DECISION_04_PRINT"));
	if( pPrintDM == NULL ) return -1;

	CESL_DataMgr *pBindDataMgr = FindDM(_T("DM_SE_BIND_BOOK_INFO"));
	if( pBindDataMgr == NULL ) return -1;

	nRowCnt = pBindDataMgr->GetRowCount();

	// 2005-04-04 by SM5ong
	// ���� ������ �ڷ� ���� �÷��� 
	BOOL bBinding = FALSE;

	pPrintDM->FreeData();
	for( i = 0 ; i < nRowCnt ; i++ ) {

		ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_STATUS"), sData, i );
		if( ids < 0 ) return -2;

		if( sData == _T("����") ) {			
			pPrintDM->InsertRow(-1);
			nCurRow = pPrintDM->GetRowCount()-1;

			// ����
			ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("TITLE"), sData, i );
			if( ids < 0 ) return -2;
			ids = SetDataMgrData( _T("DM_SE_BINDING_DECISION_04_PRINT"), _T("TITLE"), sData, nCurRow );
			if( ids < 0 ) return -2;

			// ������ȣ��
			ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("VOL_TITLE"), sData, i );
			if( ids < 0 ) return -2;
			ids = SetDataMgrData( _T("DM_SE_BINDING_DECISION_04_PRINT"), _T("VOL_TITLE"), sData, nCurRow );
			if( ids < 0 ) return -2;

			//ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_ORDER_DATE"), sYear, i );
			ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("ACQ_YEAR"), sYear, i );
			if( ids < 0 ) return -2;
			ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_SEQ_NO"), sNo, i );
			if( ids < 0 ) return -2;
			sData = sYear + _T("-") + sNo;
			ids = SetDataMgrData( _T("DM_SE_BINDING_DECISION_04_PRINT"), _T("NO"), sData, nCurRow );
			if( ids < 0 ) return -2;

			// ������
			ids = GetDataMgrData( _T("DM_SE_BINDING_INFO"), _T("BINDING_UNIT_VALUE"), sData, 0 );
			if( ids < 0 ) return -5;
			ids = SetDataMgrData( _T("DM_SE_BINDING_DECISION_04_PRINT"), _T("BOOK_CNT"), sData, nCurRow );
			if( ids < 0 ) return -2;

			// REMARK
			ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("REMARK"), sData, i );
			if( ids < 0 ) return -2;
			ids = SetDataMgrData( _T("DM_SE_BINDING_DECISION_04_PRINT"), _T("REMARK"), sData, nCurRow );
			if( ids < 0 ) return -2;

			// ������ �ڷᰡ �ϳ��� ������ �������� �÷��׸� TRUE�� �����Ѵ�.
			bBinding = TRUE;

		}
	}

	// ���� ������ �ڷᰡ ������ ������ �ڷᰡ ���ٴ� �޽��� �����ְ� �����Ѵ�.
	if(!bBinding)
	{
		AfxMessageBox(_T("���� ������ �ڷᰡ �����ϴ�."));
		return -1;
	}

	return 0;
}

// S ~ E üũ
INT CSE_BINDING_DECISION_04::CheckStartEnd( INT& f_nErrRow )
{
	INT ids, i, nRowCnt;
	BOOL bFirst = FALSE;

	CString sBindInfoDispPrev, sBindInfoDispCur;

	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BOOK_INFO"));
	if( pDataMgr == NULL ) return -1;

	nRowCnt = pDataMgr->GetRowCount();

	for( i = 0 ;  i < nRowCnt ; i++ ) {
		
		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), sBindInfoDispCur, i );
		if( ids < 0 ) return -2;

		// E���� S�� �����;ߵ�
		if( sBindInfoDispCur == _T('S') && !bFirst )	
			bFirst = TRUE;

		if( sBindInfoDispCur == _T('E') && !bFirst ) {
			f_nErrRow = i+1;
			return -100;
		}
		
		if( sBindInfoDispCur == _T('E') && bFirst ) {
			bFirst = FALSE;
		}

		// ������ �׳� �Ѿ��.
		if( sBindInfoDispCur.IsEmpty() ) 
			continue;
		
		/*
		// ���ǵ���ΰ��� �׻� 'E' �����̰ų�, ó���� �;��Ѵ�. 'S'�������� �ü� ����.
		if( sBindInfoDispCur == _T('R') ) {
			
			if( sBindInfoDispPrev[0]  == _T('S') ) {

				f_nErrRow = i+1;
				return -200;
			}
			
			continue;
		}
		*/

		// ���ǵ���ΰ��� �׻� 'E' �����̰ų�, ó���� �;��Ѵ�. 'S'�������� �ü� ����.
		if( sBindInfoDispCur == _T('R') ) {
			
			if(!sBindInfoDispPrev.IsEmpty()) {
			
				if( sBindInfoDispPrev[0]  == _T('S') ) {
						f_nErrRow = i+1;
						return -200;
				}
			}
	
			continue;
		}



		// S ~ E ������ �߸��Ǿ��ֽ��ϴ�.
		if( sBindInfoDispCur == sBindInfoDispPrev ) {
			
			f_nErrRow = i+1;
			return -200;
		}

		sBindInfoDispPrev = sBindInfoDispCur;
	}

	// 'E' is empty!!
	if( bFirst ) {
		f_nErrRow = i;
		return -300;
	}

	return 0;
}

// Bind_Info �籸��
INT CSE_BINDING_DECISION_04::ReMakeBindInfo()
{
	INT ids, i, j, nRowCnt;
	
	CString sBindInfoDisp;

	INT nBind_Info = 0;			// ��������
	CString strBind_Info;		// ��������
	CString strBind_Info_Temp;	// ��������(�����ΰ��)

	INT nFirstIdx ,nLastIdx;

	// ========================================================================================================
	// 1. BIND_BOOK DataMgr �ʱ�ȭ
	// ========================================================================================================
	CESL_DataMgr *pBindDataMgr = FindDM(_T("DM_SE_BIND_BOOK_INFO"));
	if( pBindDataMgr == NULL ) return -1;

	nRowCnt = pBindDataMgr->GetRowCount();

	for( i = nRowCnt-1 ; i >= 0 ; i-- ) {
	
		ids = pBindDataMgr->DeleteRow(i);
		if( ids < 0 ) return -2;
	}

	// first idx
	nFirstIdx = GetCopyBookfirstIdx();
	
	// last idx
	nLastIdx = GetCopyBookLastIdx();

	// ========================================================================================================
	// 2. BOOK DataMgr �ʱ�ȭ
	// ========================================================================================================
	CESL_DataMgr *pBookDataMgr = FindDM(_T("DM_SE_BOOK_INFO"));
	if( pBookDataMgr == NULL ) return -1;

	nRowCnt = pBookDataMgr->GetRowCount();

	for( i = 0, j = 0 ; i < nRowCnt ; i++ ) {

		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), sBindInfoDisp, i );
		if( ids < 0 ) return -3;

		// ���ǵ���ΰ�� 
		if( sBindInfoDisp == _T('R') ) {

			strBind_Info.Format( _T("%d"), nBind_Info++ );

			ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO"), strBind_Info, i );
			if( ids < 0 ) return -4;

			ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), sBindInfoDisp, i );
			if( ids < 0 ) return -5;

			// DM_SE_BOOK_INFO_TEMP
			ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BIND_INFO"), strBind_Info, nFirstIdx+i );
			if( ids < 0 ) return -6;

			ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BIND_INFO_DISP"), sBindInfoDisp, nFirstIdx+i );
			if( ids < 0 ) return -7;

			continue;
		}
		// S �ΰ��
		else if( sBindInfoDisp == _T('S') ) {
			
			j = i;		

			// E�� ���������� ����
			while( 1 ) {
				
				i++;
				if( i == nRowCnt ) break;

				ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), sBindInfoDisp, i );
				if( ids < 0 ) return -1;

				if( sBindInfoDisp == _T('E') )
					break;

			} //while

			strBind_Info.Format( _T("%d"), nBind_Info++ );

			while( j <= i ) {
				
				ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO"), strBind_Info, j );
				if( ids < 0 ) return -1;

				ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BIND_INFO"), strBind_Info, nFirstIdx + j );
				if( ids < 0 ) return -1;

				j++;

			} //while
			
		} //elseif
		// �����ΰ��
		else if( sBindInfoDisp == _T("") ) {

			strBind_Info.Format( _T("%d"), nBind_Info++ );

			ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO"), strBind_Info, i );
			if( ids < 0 ) return -1;

			ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BIND_INFO"), strBind_Info, nFirstIdx+i );
			if( ids < 0 ) return -1;
		} //elseif
	} //for

	return 0;
}

// ������ å���� �Ѹ��� �κ�
INT CSE_BINDING_DECISION_04::DoModifyBookDisp()
{
	INT ids, i, j, nRowCnt;
	CString sBindInfoPrev, sBindInfoCur;

	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BOOK_INFO"));
	if( pDataMgr == NULL ) return -1;
	
	nRowCnt = pDataMgr->GetRowCount();

	for( i = 0, j =  0 ;  i <  nRowCnt ; i++ ) {

		j = i;

		// while BIND_INFO is change(bind_info�� �ٲ𶧱��� i����)
		while( 1 ) {
		
			ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO"), sBindInfoCur, i );
			if( ids < 0 ) return -2;

			sBindInfoPrev = sBindInfoCur;

			i++;
			if( i == nRowCnt ) break;

			ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO"), sBindInfoCur, i );
			if( ids < 0 ) return -2;

			if( sBindInfoPrev != sBindInfoCur )
				break;
		}

		//if( i == nRowCnt ) break;
	
		// ���� index�� bind_info�� �ٲ�����̹Ƿ�.. ��ĭ�ڷ�
		i--;

		// ���ǵ���̰ų�, ���������� ���°��.
		if( i == j ) {

			//ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_INFO"), sBindInfo, j );
			//ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BIND_INFO_DISP"), _T(""), j );
		}
		else {
			
			ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), _T("S"), j );
			if( ids < 0 ) return -3;

			ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), _T("E"), i );
			if( ids < 0 ) return -3;
		}
	}

	ids = AllControlDisplay( _T("CM_SE_BINDING_DECISION_04_BOOK"), -1 );
	if( ids ) return -1;

	return 0;
}

INT CSE_BINDING_DECISION_04::DoModifyBindInfo()
{
	INT ids, i, j, idxBind, nRowCnt;
	CString sBindInfoPrev, sBindInfoCur;

	// 1. BIND_BOOK_INFO�� ���� �ٽ� BIND_BOOK�� �����Ѵ�.
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BOOK_INFO"));
	if( pDataMgr == NULL ) return -1;

	nRowCnt = pDataMgr->GetRowCount();

	CString sTemp;
	ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("VOL_TITLE"), sTemp, 0 );
	if( ids < 0 ) return -1;

	idxBind = 0;

	for( i = 0, j =  0 ;  i <  nRowCnt ; i++ ) {

		j = i;

		// while BIND_INFO is change(bind_info�� �ٲ𶧱��� i����)
		while( 1 ) {

			ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO"), sBindInfoCur, i );
			if( ids < 0 ) return -2;

			sBindInfoPrev = sBindInfoCur;

			i++;
			if( i == nRowCnt ) break;

			ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO"), sBindInfoCur, i );
			if( ids < 0 ) return -2;

			if( sBindInfoPrev != sBindInfoCur )
				break;
		}

		// ���� index�� bind_info�� �ٲ�����̹Ƿ�.. ��ĭ�ڷ�
		i--;

		// ���� index == j , �� index == i ���ؼ� ����å������ �����.


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 1. �⺻���� å������ �ֱ� : iBindBookIdx��° INSERT�ϱ�
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ids = DoBaseBookInfo( j );
		if( ids ) return -1;
		
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 2. �������� ���� ����å�� �ֱ�, ����å�� ���� ���� �ֱ�.
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ids = DoBaseBindInfo_M( idxBind, j, i);
		if( ids ) return -1;
		
		idxBind++;
	}

	return 0;
}

// Bind Modify �ΰ�� ����å ����
INT CSE_BINDING_DECISION_04::DoBaseBindInfo_M( INT f_IdxBind, INT f_iBookStart, INT f_iBookEnd )
{
	INT ids, i;

	CString sVolTitle, sBindingKey, sBindInfoDisp; // S, E, R
	CString sTemp;

	CString	strBindVolTitle;	// ������ȣ��
	CString strTempBindVolTitle;//

	CString strBind_Info;		// ��������


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// f_iBookStart == f_iBookEnd && BIND_INFO_DISP == _T("R") -> ���ǵ��
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if( f_iBookStart == f_iBookEnd ) {
		
		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), sBindInfoDisp, f_iBookStart );
		if( ids < 0 ) return -1;
	
		if( sBindInfoDisp == _T('R') ) {
			
			ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_STATUS"), sBindInfoDisp, f_IdxBind );
			if( ids < 0 ) return -1;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. iBindBookIdx��° ���۱�ȣ �ֱ�	    iBookIdx��° å����
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("VOL_TITLE"), sVolTitle, f_iBookStart );
	if( ids < 0 ) return -1;
	
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("START_VOL_TITLE"), sVolTitle, f_IdxBind );
	if( ids < 0 ) return -1;
	
	strBindVolTitle = sVolTitle + _T(" - ");
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2.1. iBindBookIdx��° ���Rȣ �ֱ�			iBookIdx+(nBind_Unit-1)��° å����
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("VOL_TITLE"), sVolTitle, f_iBookEnd );
	if( ids < 0 ) return -1;
	
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("END_VOL_TITLE"), sVolTitle, f_IdxBind );
	if( ids < 0 ) return -1;

	strTempBindVolTitle = sVolTitle;
	strBindVolTitle += strTempBindVolTitle;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2.2 ������ȣ�� �ֱ�..
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("VOL_TITLE"), strBindVolTitle, f_IdxBind );
	if( ids < 0 ) return -1;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// include7 üũ�غ���.. DB���� �������� ���� ID_FLAG -> 'U' �ƴϸ� 'I'��..
	//                      
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("ID_FLAG"), _T("I"), f_IdxBind );
	if( ids < 0 ) return -1;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// title
	// modified by loveisverb 2004-01-28
	CString sTitle;
	ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("TITLE_INFO"), sTitle, m_nIdx );
	if( ids < 0 ) return -9;

	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("TITLE"), sTitle, f_IdxBind );
	if( ids < 0 ) return -9;


	// BINDING_PRICE
	CString sBindingPrice;

	ids = GetDataMgrData( _T("DM_SE_BINDING_INFO"), _T("BINDING_PRICE"), sBindingPrice, 0 );
	if( ids < 0 ) return -11;

	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_PRICE"), sBindingPrice, f_IdxBind );
	if( ids < 0 ) return -12;

	// �������� �ٲٱ� binding_status -> '1';
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_STATUS"), _T("1"), f_IdxBind );
	if( ids < 0 ) return -1;

	// å���� �ٲٱ� status -> 'SEB112N';
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("WORKING_STATUS"), _T("SEB112N"), f_IdxBind );
	if( ids < 0 ) return -1;

	// 4. BINDED_BOOK_YN (����å����) �ֱ�..
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDED_BOOK_YN"), _T("N"), f_IdxBind );
	if( ids < 0 ) return -1;

	// BIND_BOOK PUT BINDING_YEAR
	CString sTodayDate;
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BIND_BOOK_INFO"));
	if( pDataMgr == NULL ) return -1;

	CTime time = pDataMgr->GetDBTime();
	sTodayDate.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );

	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_YEAR"), sTodayDate.Left(4), f_IdxBind );
	if( ids < 0 ) return -16;
	
	// SeqNo
	CString sSeqNo;
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_01"), _T("SEQNO"), sSeqNo, -1 );
	if( ids < 0 ) return -17;
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_SEQ_NO"), sSeqNo, f_IdxBind );
	if( ids < 0 ) return -18;

	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("LAST_NO"), m_sLastSerialNo, f_IdxBind );
	if( ids < 0 ) return -18;

	// ������ȣ
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("RECEIPT_NO"), sSeqNo, f_IdxBind );
	if( ids < 0 ) return -18;

	// �����Ϸù�ȣ
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("RECEIPT_SERIAL_NO"), m_sLastSerialNo, f_IdxBind );
	if( ids < 0 ) return -18;

	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BINDING_CONF_DATE"), sTodayDate, f_IdxBind );
	if( ids < 0 ) return -19;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 3. ���������ֱ�..
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	strBind_Info.Format( _T("%d"), f_IdxBind );
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BIND_INFO"), strBind_Info, f_IdxBind );
	if( ids < 0 ) return -1;

	// binding_key
	ids = GetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("REC_KEY"), sBindingKey, f_IdxBind );
	if( ids < 0 ) return -1;
	
	INT nFirstIdx ,nLastIdx;

	// first idx
	nFirstIdx = GetCopyBookfirstIdx();
	
	// last idx
	nLastIdx = GetCopyBookLastIdx();


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 4. å������ ���� ���� �ֱ� 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString sBookKeys;
	for( i = f_iBookStart ; i <= f_iBookEnd ; i++ ) {

		// i å���� ������������ŭ ������
		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO"), strBind_Info, i );
		if( ids < 0 ) return -1;

		// ����åŰ �ֱ�..
		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BINDING_BOOK_KEY"), sBindingKey, i );
		if( ids < 0 ) return -1;

		// å���� �ֱ�.. status -> 'SEB112N'
		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("WORKING_STATUS"), _T("SEB112N"), i );
		if( ids < 0 ) return -1;

		// nFirstIdx+i å���� ������������ŭ ������
		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BIND_INFO"), strBind_Info, nFirstIdx+i );
		if( ids < 0 ) return -1;

		// ����åŰ �ֱ�..
		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("BINDING_BOOK_KEY"), sBindingKey, nFirstIdx+i );
		if( ids < 0 ) return -1;

		// å���� �ֱ�.. WORKING_STATUS -> 'SEB112N'
		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("WORKING_STATUS"), _T("SEB112N"), nFirstIdx+i );
		if( ids < 0 ) return -1;

		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO_TEMP"), _T("REC_KEY"), sTemp, i );
		if( ids < 0 ) return -25;

		if( i == f_iBookStart )	sBookKeys = sTemp;
		else					sBookKeys += _T(",") + sTemp;

	}
	
	// BOOK_KEYS
	ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("BOOK_KEYS"), sBookKeys, f_IdxBind );
	if( ids < 0 ) return -26;

	return 0;
}

VOID CSE_BINDING_DECISION_04::OnSelchangeTabBokbon(NMHDR* pNMHDR, LRESULT* pResult) 
{
	INT nIndex;
	CRect rect;

	CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_BOKBON);
	nIndex = pTabCtrl->GetCurSel();

	m_nBokbonTabIndex = nIndex;

	InitDisplay();

	pTabCtrl->SetCurSel(m_nBokbonTabIndex);

	*pResult = 0;
}

VOID CSE_BINDING_DECISION_04::SetLastSerialNo( CString f_sLastSerialNo )
{
	m_sLastSerialNo = f_sLastSerialNo;
}

/*
VOID CSE_BINDING_DECISION_04::OnClickListBook() 
{
	INT ids, nRowCount;
	CString sDispInfo;

	CESL_ControlMgr * pCtrlMgr = FindCM(_T("CM_SE_BINDING_DECISION_04_BOOK"));
	if( !pCtrlMgr ) return;
	
	CESL_Grid *pBookGrid = (CESL_Grid *)pCtrlMgr->FindControl(_T("BookGrid"));
	if( !pBookGrid ) return;
	
	INT nRow = -1;
	INT nCol = -1;

	nCol = pBookGrid->GetCol();
	nRow = pBookGrid->GetRow();

	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BOOK_INFO"));
	if( pDataMgr == NULL ) return;

	nRowCount = pDataMgr->GetRowCount();

	if( m_bModify == FALSE ) return;

	if( nRowCount < 1 )	return;
	if( nRow	  < 0 )	return;

	if ( nCol == 1 ) {

		ids = GetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), sDispInfo, nRow );
		if( ids ) return;

		// Start -> End -> ''
		if		( sDispInfo == _T("S") ) sDispInfo = _T("E");
		else if ( sDispInfo == _T("E") ) sDispInfo = _T("");
		else if ( sDispInfo == _T("")  ) sDispInfo = _T("S");
		else return;
		
		ids = SetDataMgrData( _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), sDispInfo, nRow );
		if( ids ) return;

		// grid
		pBookGrid->SetAt( nRow , nCol-1 ,sDispInfo );
		if( ids ) return;
	}
	else {

		
	}

	// temp
	SetGridColor( nRow );
}
*/

/*
VOID CSE_BINDING_DECISION_04::SetGridColor( INT f_nIdx )
{
	INT i, ids, nRowCount;
	TCHAR cDispInfo[8];

	CESL_ControlMgr * pCtrlMgr = FindCM(_T("CM_SE_BINDING_DECISION_04_BOOK"));
	if( !pCtrlMgr ) return;
	
	CESL_Grid *pGrid = (CESL_Grid *)pCtrlMgr->FindControl(_T("BookGrid"));
	if( !pGrid ) return;

	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BOOK_INFO"));
	if( pDataMgr == NULL ) return;

	nRowCount = pDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCount ; i++ ) {
		pGrid->SetRowBackColor( i+1, RGB( 255, 255, 255 ) );
	}

	// Set Grid Color
	ids = ECO_GetDialogManagerData( _T(':'), _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), f_nIdx, cDispInfo );
	if ( ids ) RTN_VOID( _T("ECO_GetDialogManagerData Error!") );

	if( _tcscmp( cDispInfo, _T("S") ) == 0 ) {
	
		for( i = f_nIdx ; i < nRowCount ; i++ ) {

			ids = ECO_GetDialogManagerData( _T(':'), _T("DM_SE_BOOK_INFO"),
											_T("BIND_INFO_DISP"), i, cDispInfo );
			if ( ids ) RTN_VOID( _T("ECO_GetDialogManagerData Error!") );
		
			if( _tcscmp( cDispInfo, _T("E") ) == 0 ) {
				pGrid->SetRowBackColor( i+1, RGB( 235, 235, 255 ) );
				break;
			}
			else if( i != f_nIdx && _tcscmp( cDispInfo, _T("S") ) == 0 ) {
				break;
			}

			// Set Grid Color
			pGrid->SetRowBackColor( i+1, RGB( 235, 235, 255 ) );
		}
	}
	else if( _tcscmp( cDispInfo, _T("E") ) == 0 ) {

		for( i = f_nIdx ; i >= 0 ; i-- ) {

			ids = ECO_GetDialogManagerData( _T(':'), _T("DM_SE_BOOK_INFO"),
											_T("BIND_INFO_DISP"), i, cDispInfo );
			if ( ids ) RTN_VOID( _T("ECO_GetDialogManagerData Error!") );
		
			if( _tcscmp( cDispInfo, _T("S") ) == 0 ) {
				pGrid->SetRowBackColor( i+1, RGB( 235, 235, 255 ) );
				break;
			}				
			else if( i != f_nIdx && _tcscmp( cDispInfo, _T("E") ) == 0 ) {
				break;
			}

			// Set Grid Color
			pGrid->SetRowBackColor( i+1, RGB( 235, 235, 255 ) );
		}
	}
	else if( _tcscmp( cDispInfo, _T("") ) == 0 ) {
		
		for( i = f_nIdx ; i >= 0 ; i-- ) {
			ids = ECO_GetDialogManagerData( _T(':'), _T("DM_SE_BOOK_INFO"),
											_T("BIND_INFO_DISP"), i, cDispInfo );
			if ( ids ) RTN_VOID( _T("ECO_GetDialogManagerData Error!") );
		
			if( _tcscmp( cDispInfo, _T("S") ) == 0 ) break;
		}
		
		if( i < 0 ) i = 0 ;

		for( i = i ; i < nRowCount ; i++ ) {
			ids = ECO_GetDialogManagerData( _T(':'), _T("DM_SE_BOOK_INFO"),
											_T("BIND_INFO_DISP"), i, cDispInfo );
			if ( ids ) RTN_VOID( _T("ECO_GetDialogManagerData Error!") );
		
			if( _tcscmp( cDispInfo, _T("E") ) == 0 ) {
				pGrid->SetRowBackColor( i+1, RGB( 235, 235, 255 ) );
				break;
			}
			else if( i != i && _tcscmp( cDispInfo, _T("S") ) == 0 ) {
				break;
			}

			// Set Grid Color
			pGrid->SetRowBackColor( i+1, RGB( 235, 235, 255 ) );
		}
	}

	ids = AllControlDisplay( _T("CTRL_DICIDE_01"), -1, TRUE );
	if( ids ) return;
}
*/

/*
VOID CSE_BINDING_DECISION_04::OnRclickListBook(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
	CMenu	m_ContextMenu;
	CPoint	point;
	CRect	rect;

	::GetCursorPos( &point );
	GetWindowRect( &rect );

	INT ids, nRow, nCol, nRowCount;

	nRow = pNMListView->iItem;
	nCol = pNMListView->iSubItem;

	ids = ECO_GetRecordCount( _T(':'), _T("DM_SE_BOOK_INFO_TEMP"), NULL, &nRowCount );
	if ( ids ) RTN_VOID( _T("ECO_GetRecordCount Error !") );
	
	if( nRowCount	< 1 )	return;
	if( nRow		< 0 )	return;

	if( nCol == 1 ) {
		
		// Context Menu Display.
		m_ContextMenu.CreatePopupMenu	();
		m_ContextMenu.AppendMenu		( MF_STRING, 4000, _T("(S)tart") );
		m_ContextMenu.AppendMenu		( MF_STRING, 4001, _T("(E)nd") );
		m_ContextMenu.AppendMenu		( MF_STRING, 4002, _T("Empty") );
		m_ContextMenu.AppendMenu		( MF_SEPARATOR);
		m_ContextMenu.AppendMenu		( MF_STRING, 4003, _T("(R)egister") );
		m_ContextMenu.TrackPopupMenu	( TPM_LEFTALIGN, point.x, point.y, this);
	}

	*pResult = 0;
}

*/

/*
VOID CSE_BINDING_DECISION_04::SelectProc( UINT nIDC)
{
	INT i, SelectCnt, ids;
	CString strTmp;

	CECO_ControlGrid *m_f1VCF = (CECO_ControlGrid*)GetDlgItem(IDC_LIST_BOOK);
	if( NULL == m_f1VCF ) return;

	m_f1VCF->MakeSelectedRowList( FALSE );

	switch( nIDC ) {

	case 4000:
		SelectCnt = m_f1VCF->GetSelectedCnt();
		for( i = 0 ; i < SelectCnt; i++ ) {
			
			m_f1VCF->SetTextRC( m_f1VCF->SelectedListIndex[i], 1, _T("S") );
			
			ids = ECO_PutDialogManagerData( _T(':'), _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), 
				                                     m_f1VCF->SelectedListIndex[i]-1, _T("S") );
			if( ids ) RTN_VOID( _T("ECO_PutDialogManagerData Error!") );

		}
		break;

	case 4001:
		SelectCnt = m_f1VCF->GetSelectedCnt();
		for( i = 0 ; i < SelectCnt ; i++ ) {
			
			m_f1VCF->SetTextRC( m_f1VCF->SelectedListIndex[i], 1, _T("E") );
			ids = ECO_PutDialogManagerData( _T(':'), _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), 
				                                     m_f1VCF->SelectedListIndex[i]-1, _T("E") );
			if( ids ) RTN_VOID( _T("ECO_PutDialogManagerData Error!") );

		}
		break;

	case 4002:
		SelectCnt = m_f1VCF->GetSelectedCnt();
		for( i = 0 ; i < SelectCnt ; i++ ) {
	
			m_f1VCF->SetTextRC( m_f1VCF->SelectedListIndex[i], 1, _T("") );
			ids = ECO_PutDialogManagerData( _T(':'), _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), 
				                                     m_f1VCF->SelectedListIndex[i]-1, _T("") );
			if( ids ) RTN_VOID( _T("ECO_PutDialogManagerData Error!") );

		}
		break;

	case 4003:
		SelectCnt = m_f1VCF->GetSelectedCnt();
		for( i = 0 ; i < SelectCnt ; i++ ) {
	
			m_f1VCF->SetTextRC( m_f1VCF->SelectedListIndex[i], 1, _T("R") );
			ids = ECO_PutDialogManagerData( _T(':'), _T("DM_SE_BOOK_INFO"), _T("BIND_INFO_DISP"), 
				                                     m_f1VCF->SelectedListIndex[i]-1, _T("R") );
			if( ids ) RTN_VOID( _T("ECO_PutDialogManagerData Error!") );

		}
		break;
	}
}
*/

// binding_title
/*
VOID CSE_BINDING_DECISION_04::OnbUPDATESPINETITLE() 
{
	// Mod 2003.06.28 wsw
	INT ids, iStatus;

	iStatus = IsDlgButtonChecked(IDC_bUPDATE_SPINE_TITLE);
	
	if(iStatus == 0) {
		((CEdit*) GetDlgItem(IDC_eBIND_VOL_TITLE)) -> SetReadOnly(TRUE);

		CString sBuffer;
		GetDlgItem(IDC_eBIND_VOL_TITLE) -> GetWindowText(sBuffer);

		// put
		ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("VOL_TITLE"), sBuffer, m_nBindBookIdx );
		if( ids < 0 ) return;

	}
	else {
		((CEdit*) GetDlgItem(IDC_eBIND_VOL_TITLE)) -> SetReadOnly(FALSE);
		GetDlgItem(IDC_eBIND_VOL_TITLE) -> SetFocus();
	}
}
*/

// remark
/*
VOID CSE_BINDING_DECISION_04::OnbUPDATEBESIDE() 
{
	// Mod 2003.06.28 wsw
	INT ids, iStatus;
	
	iStatus = IsDlgButtonChecked(IDC_bUPDATE_BESIDE);

	if(iStatus == 0) {
		((CEdit*) GetDlgItem(IDC_eREMARKS)) -> SetReadOnly(TRUE);

		CString sBuffer;
		GetDlgItem(IDC_eREMARKS) -> GetWindowText(sBuffer);

		// put
		ids = SetDataMgrData( _T("DM_SE_BIND_BOOK_INFO"), _T("REMARK"), sBuffer, m_nBindBookIdx );
		if( ids < 0 ) return;
	}
	else {
		((CEdit*) GetDlgItem(IDC_eREMARKS)) -> SetReadOnly(FALSE);
		GetDlgItem(IDC_eREMARKS) -> SetFocus();
	}
}
*/

HBRUSH CSE_BINDING_DECISION_04::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
