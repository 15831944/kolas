// SE_BINDING_TERMIN_03.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_TERMIN_03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_TERMIN_03 dialog


CSE_BINDING_TERMIN_03::CSE_BINDING_TERMIN_03(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_nHeadPosition = m_nTailPosition = 0;

}

CSE_BINDING_TERMIN_03::~CSE_BINDING_TERMIN_03()
{
}

VOID CSE_BINDING_TERMIN_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_TERMIN_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_BINDING_TERMIN_03, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_TERMIN_03)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bADD, OnbADD)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bUPDATE_SPINE_TITLE, OnbUPDATESPINETITLE)
	ON_BN_CLICKED(IDC_bUPDATE_BESIDE, OnbUPDATEBESIDE)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_TERMIN_03 message handlers

BOOL CSE_BINDING_TERMIN_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_TERMIN_03")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

	((CEdit*) GetDlgItem(IDC_eBIND_VOL_TITLE))	-> SetReadOnly(TRUE);
	((CEdit*) GetDlgItem(IDC_eREMARKS))			-> SetReadOnly(TRUE);	

	INT ids = MoveIndex( _CURRENT );
	if( ids ) {
		AfxMessageBox (_T("MoveIndex() Error~!"));
		return FALSE ;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_BINDING_TERMIN_03::OnbPREV() 
{
	INT ids;

	ids = MoveIndex(_PREV);
	if( ids ) return;
}

VOID CSE_BINDING_TERMIN_03::OnbNEXT() 
{
	INT ids;

	ids = MoveIndex(_NEXT);
	if( ids ) return;
}

INT CSE_BINDING_TERMIN_03::MoveIndex(TCHAR to)
{
   INT          nRowIndex, ids;

	CESL_ControlMgr *pCM		= FindCM( _T("CM_SE_BINDING_TERMIN_01") );
	CESL_Grid		*pGridMgr	= (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	// ..............................................................................................................
	// 1. �⺻ȭ���� index�� ���� ó���� �Ѵ�.
	// ..............................................................................................................

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

	// ..............................................................................................................
	// 2. ���������� ǥ���Ѵ�.
	// ..............................................................................................................
    //pGridMgr->SetListIndexSelection( );

	// ..............................................................................................................
    // 3. ����,���� ��ư�� Enable/Diable
	// ..............................................................................................................
	//Prev_Next_ButtonStatus( _T("DISP_BASE"), m_pParCtrlAlias, m_pParGridAlias, IDC_bPREV, IDC_bNEXT);

	// ..............................................................................................................
    // 4. �̵��� ���ڵ� �ε����� ���Ѵ�.
	// ..............................................................................................................
	m_nIdx = pGridMgr->SelectGetIdx();
    if ( m_nIdx < 0 ) return -8;

	InitDisplay();

	//parent grid set reverse
	pGridMgr->SetReverse(m_nIdx);

    return 0;
}

// DB_BIND_BOOK_INFO	-> DM_SE_BINDING_TERMIN_BINDBOOK
// DB_BOOK_INFO			-> DM_SE_BINDING_TERMIN_BOOK
VOID CSE_BINDING_TERMIN_03::OnbADD() 
{
	INT i, ids, nRowCnt, nRowIndex;

	CESL_DataMgr *pBindBookDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BINDBOOK"));
	if( pBindBookDataMgr == NULL ) return ;

	nRowCnt = pBindBookDataMgr->GetRowCount();
	if( nRowCnt == 0 ) {
		AfxMessageBox( _T("�����ڷᰡ �������� �ʽ��ϴ�.") );
		return;
	}

	CESL_ControlMgr *pCM		= FindCM( _T("CM_SE_ACQUISIT_BOOK") );
	CESL_Grid		*pGridMgr	= (CESL_Grid*) pCM->FindControl ( _T("AcqGrid") ) ;
	
	ids = pGridMgr->SelectMakeList();
	if( ids ) return;
	
	nRowCnt = pGridMgr->SelectGetCount();
	if( nRowCnt == 0 ) {
		AfxMessageBox( _T("�ڷḦ �����Ͻʽÿ�.") );
		return;
	}
	else if( nRowCnt <  0 ) {
		AfxMessageBox( _T("�׸����� ���������� �������µ� �����Ͽ����ϴ�.") );
		return;
	}

	for( i = nRowCnt-1 ; i >= 0 ; i-- ) {

		if( i == nRowCnt-1 ) {
			nRowIndex = pGridMgr->SelectGetTailPosition();
			if( nRowIndex == -1 ) break;
		}
		else {
			nRowIndex = pGridMgr->SelectGetPrev();
			if( nRowIndex == -1 ) break;
		}

		// Add
		ids = DoAddProc(nRowIndex);
        if( ids ) return;
	}

	//DM sorting
	CESL_DataMgr *pBindInfoDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"));
	if( pBindInfoDataMgr == NULL ) return;

	ids = pBindInfoDataMgr->SORT( _T("VOL_SORT_NO") );
	if( ids ) return;

	CESL_DataMgr *pBookDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BOOK"));
	if( pBookDataMgr == NULL ) return;

	ids = pBookDataMgr->SORT( _T("VOL_SORT_NO") );
	if( ids ) return;

	
	//���� ��ȣ���� ���� ����� �ش�
	MakeBindingVolTitle();
	

	// display
	ids = AllControlDisplay( _T("CM_SE_ORDER_BOOK"), -1 );
	if( ids ) return;

	ids = AllControlDisplay( _T("CM_SE_ORDER_BOOK"), 0 );
	if( ids ) return;

	ids = AllControlDisplay( _T("CM_SE_ACQUISIT_BOOK"), -1 );
	if( ids ) return;
}

VOID CSE_BINDING_TERMIN_03::OnbDELETE() 
{
	INT i, ids, nRowCnt, nRowIndex;

	CESL_ControlMgr *pCM		= FindCM( _T("CM_SE_ORDER_BOOK") );
	CESL_Grid		*pGridMgr	= (CESL_Grid*) pCM->FindControl ( _T("OrderGrid") ) ;
	
	ids = pGridMgr->SelectMakeList();
	if( ids ) return;
	
	nRowCnt = pGridMgr->SelectGetCount();
	if( nRowCnt == 0 ) {
		AfxMessageBox( _T("�ڷḦ �����Ͻʽÿ�.") );
		return;
	}
	else if( nRowCnt <  0 ) {
		AfxMessageBox( _T("�׸����� ���������� �������µ� �����Ͽ����ϴ�.") );
		return;
	}

	for( i = nRowCnt-1 ; i >= 0 ; i-- ) {
		
		if( i == nRowCnt-1 ) {
			nRowIndex = pGridMgr->SelectGetTailPosition();
			if( nRowIndex == -1 ) break;
		}
		else {
			nRowIndex = pGridMgr->SelectGetPrev();
			if( nRowIndex == -1 ) break;
		}

		// sub
		ids = DoSubProc(nRowIndex);
        if( ids ) {
			AfxMessageBox( _T("DoAddProc error!") );
			return;
		}
	}

	//
	CESL_DataMgr *pBindInfoDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"));
	if( pBindInfoDataMgr == NULL ) return ;

	ids = pBindInfoDataMgr->SORT( _T("VOL_SORT_NO") );
	if( ids ) return;


	//DM sorting
	CESL_DataMgr *pBookDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BOOK"));
	if( pBookDataMgr == NULL ) return ;

	ids = pBookDataMgr->SORT( _T("VOL_SORT_NO") );
	if( ids ) return;
	

	//���� ��ȣ���� ���� ����� �ش�
	MakeBindingVolTitle();

	// display
	ids = AllControlDisplay( _T("CM_SE_ORDER_BOOK"), -1 );
	if( ids ) return;

	ids = AllControlDisplay( _T("CM_SE_ORDER_BOOK"), 0 );
	if( ids ) return;

	ids = AllControlDisplay( _T("CM_SE_ACQUISIT_BOOK"), -1 );
	if( ids ) return;
}

VOID CSE_BINDING_TERMIN_03::OnbUPDATESPINETITLE() 
{
	INT ids, iStatus;

	iStatus = IsDlgButtonChecked(IDC_bUPDATE_SPINE_TITLE);
	
	if(iStatus == 0) {
		((CEdit*) GetDlgItem(IDC_eBIND_VOL_TITLE)) -> SetReadOnly(TRUE);

		CString sBuffer;
		GetDlgItem(IDC_eBIND_VOL_TITLE) -> GetWindowText(sBuffer);

		// put
		ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK"), _T("VOL_TITLE"), sBuffer, 0 );
		if( ids ) return;

	}
	else {
		((CEdit*) GetDlgItem(IDC_eBIND_VOL_TITLE)) -> SetReadOnly(FALSE);
		GetDlgItem(IDC_eBIND_VOL_TITLE) -> SetFocus();
	}
}

VOID CSE_BINDING_TERMIN_03::OnbUPDATEBESIDE() 
{
	INT ids, iStatus;
	
	iStatus = IsDlgButtonChecked(IDC_bUPDATE_BESIDE);

	if(iStatus == 0) {
		((CEdit*) GetDlgItem(IDC_eREMARKS)) -> SetReadOnly(TRUE);

		CString sBuffer;
		GetDlgItem(IDC_eREMARKS) -> GetWindowText(sBuffer);

		// put
		ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK"), _T("REMARK"), sBuffer, 0 );
		if( ids ) return;

	}
	else {
		((CEdit*) GetDlgItem(IDC_eREMARKS)) -> SetReadOnly(FALSE);
		GetDlgItem(IDC_eREMARKS) -> SetFocus();
	}	
}

VOID CSE_BINDING_TERMIN_03::OnbSAVE() 
{
	INT i, j, ids, nRowCnt;
	CString sModYn, sRecKey, sSql, sData;

	const INT nBookFieldCnt = 6;
	TCHAR* pBookAlias[nBookFieldCnt] = {
			_T("REC_KEY"),
			_T("BINDING_BOOK_KEY"),
			_T("ACQ_CODE"),
			_T("ACQ_YEAR"),
			_T("WORKING_STATUS"),
			_T("REMARK")
	};

	TCHAR* pFieldType[nBookFieldCnt] = {
			_T("STRING"),
			_T("STRING"),
			_T("STRING"),
			_T("STRING"),
			_T("STRING"),
			_T("STRING")
	};

	// SendFrame
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_01"));
	if( pDataMgr == NULL ) return;

	//
	CESL_DataMgr *pBindInfoDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"));
	if( pBindInfoDataMgr == NULL ) return;

	ids = pBindInfoDataMgr->SORT( _T("VOL_SORT_NO") );
	if( ids < 0 ) return;
	
	// ..............................................................................................................
	// update bindbook info(����å�� ���ǵ�)
	// ..............................................................................................................
	pDataMgr->StartFrame();

	nRowCnt = pBindInfoDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ ) {
		
		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"), _T("MOD_YN"), sModYn, i );
		if( ids < 0 ) return;

		if( sModYn == _T('Y') ) {
			
			for( j = 0 ; j < nBookFieldCnt ; j++ )  {
			
				ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"), pBookAlias[j], sData, i );
				if( ids < 0 ) return ;
				
				ids = pDataMgr->AddDBFieldData( pBookAlias[j], pFieldType[j], sData );
				if( ids < 0 ) return;

				if( j == 0 ) sRecKey = sData;
			}

			ids = pDataMgr->MakeUpdateFrame( _T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
			if( ids < 0 ) return;
		}
	}

	// ..............................................................................................................
	// update change book info
	// ..............................................................................................................
	CESL_DataMgr *pBookDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BOOK"));
	if( pBookDataMgr == NULL ) return;

	nRowCnt = pBookDataMgr->GetRowCount();
	
	for( i = 0 ; i < nRowCnt ; i++ ) {

		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BOOK"), _T("MOD_YN"), sModYn, i );
		if( ids < 0 ) return;

		if( sModYn == _T('Y') ) {
		
			for( j = 0 ; j < nBookFieldCnt ; j++ )  {
				
				ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BOOK"), pBookAlias[j], sData, i );
				if( ids < 0 ) return ;
				
				ids = pDataMgr->AddDBFieldData( pBookAlias[j], pFieldType[j], sData );
				if( ids < 0 ) return;

				if( j == 0 ) sRecKey = sData;
			}

			ids = pDataMgr->MakeUpdateFrame( _T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
			if( ids < 0 ) return;
		}
	}
	
	// ..............................................................................................................
	// update bindbook(����å)
	// ..............................................................................................................
	CString sBindingKey, sBindTitle, sRemark;
	CString sVolKey, sBookKeys, sTmpKey;

	CESL_DataMgr *pBindBookDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BINDBOOK"));
	if( pBindBookDataMgr == NULL ) return;

	nRowCnt = pBindBookDataMgr->GetRowCount();

	if( nRowCnt > 0 ) {

		// Get BindKey
		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK"), _T("REC_KEY"), sBindingKey, 0 );
		if( ids < 0 ) return;

		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK"), _T("VOL_KEY"), sVolKey, 0 );
		if( ids < 0 ) return;

		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK"), _T("VOL_TITLE"), sBindTitle, 0 );
		if( ids < 0 ) return;

		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK"), _T("REMARK"), sRemark, 0 );
		if( ids < 0 ) return;
	}

	// ..............................................................................................................
	// start vol info ~ end vol info  ( 0��°���� nRowCnt-1��°���� )
	// ..............................................................................................................
	nRowCnt = pBindInfoDataMgr->GetRowCount();

	if( nRowCnt > 0 ) {

		for( i = 0 ; i < nRowCnt ; i++ ) {

			ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"), _T("REC_KEY"), sTmpKey, i );
			if( ids < 0 ) return;

			if( i == 0 )	sBookKeys += sTmpKey;
			else 			sBookKeys += _T(", ") + sTmpKey;
		}
		
		//modified by loveisverb
		//ȭ�鿡�� ������ ������ ����
		CString strBindTitle,strRemark;
		CESL_ControlMgr* pCM = FindCM(_T("CM_SE_ORDER_BOOK"));
		if(pCM) {
			pCM->GetControlMgrData(_T("VOL_TITLE"),strBindTitle);
			pCM->GetControlMgrData(_T("REMARK"),strRemark);
		}
	
		// ������ȣ �� ��� update ( VOL_TITLE )
		sSql.Format( _T("UPDATE SE_VOL_TBL SET VOL_TITLE = '%s', REMARK = '%s' WHERE REC_KEY = %s;"), strBindTitle, strRemark, sVolKey );
		pDataMgr->AddFrame(sSql);


		// ����å�� update ����..

		// �������� update ( BOOK_KEYS, BINDING_PRICE?? )
		sSql.Format( _T("UPDATE SE_BINDING_BOOK_TBL SET BOOK_KEYS = '%s' WHERE BOOK_KEY = %s;"), sBookKeys, sBindingKey );
		pDataMgr->AddFrame(sSql);

	}
	// ..............................................................................................................
	// delete BindBook: �����ڷḦ �� ����������� �������...
	// ..............................................................................................................
	else {

		nRowCnt = pBindBookDataMgr->GetRowCount();

		if( nRowCnt > 0 ) {

			// Get BindKey
			ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK"), _T("REC_KEY"), sBindingKey, 0 );
			if( ids < 0 ) return;
			
			sSql.Format( _T("DELETE SE_BOOK_TBL WHERE REC_KEY = %s;"), sBindingKey );
			pDataMgr->AddFrame(sSql);
		}
	}

	// send frame, end frame
	ids = pDataMgr->SendFrame();
	pDataMgr->EndFrame();
	if( ids < 0 ) {
		AfxMessageBox( _T("���忡 �����Ͽ����ϴ�!") );
		return;
	}


	InitDisplay();

	AfxMessageBox( _T("�����ϼ̽��ϴ�.") );
}

INT CSE_BINDING_TERMIN_03::DoAddProc( INT f_nIdx )
{
	INT ids, nRowCnt;

	CString sBindindKey;

	// ..............................................................................................................
	// copy base 
	// ..............................................................................................................
	ids = CopyDataManager( _T("DM_SE_BINDING_TERMIN_BOOK"), f_nIdx, _T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"), -1 );
	if( ids ) return -1;

	CESL_DataMgr *pBindInfoDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"));
	if( pBindInfoDataMgr == NULL ) return -2;

	nRowCnt = pBindInfoDataMgr->GetRowCount();

	// ..............................................................................................................
	// input etc
	// ..............................................................................................................

	// put STATUS_D
	ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"), _T("STATUS_D"), _T("�˼����"), nRowCnt-1 );
	if( ids ) return -3;

	// put MOD_YN
	ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"), _T("MOD_YN"), _T("Y"), nRowCnt-1 );
	if( ids ) return -4;

	// put STATUS
	ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"), _T("WORKING_STATUS"), _T("SEB113N"), nRowCnt-1 );
	if( ids ) return -6;

	// put BINDING_BOOK_KEY
	ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK"), _T("REC_KEY"), sBindindKey, 0 );
	if( ids ) return -7;
	ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"), _T("BINDING_BOOK_KEY"), sBindindKey, nRowCnt-1 );
	if( ids ) return -8;

	// ..............................................................................................................
	// change bindbook
	// ..............................................................................................................
	CString sRowCnt;
	sRowCnt.Format( _T("%d"), nRowCnt );

	ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK"), _T("ORDER_BOOK_CNT"), sRowCnt, 0 );
	if( ids ) return -9;

	// ..............................................................................................................
	// delete base
	// ..............................................................................................................
	CESL_DataMgr *pBookDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BOOK"));
	if( pBookDataMgr == NULL ) return -10;

	ids = pBookDataMgr->DeleteRow(f_nIdx);
	if( ids ) return -11;

	return 0;
}

INT CSE_BINDING_TERMIN_03::CopyDataManager( CString f_sSrcDm, INT f_nSrcIdx, CString f_sDesDm, INT f_nDesIdx )
{
	INT ids, nCurRow, col;

	const INT nAliasCnt = 10;
	TCHAR* sAlias[nAliasCnt] = {
			_T("VOL_TITLE"),
			_T("OTHER_VOL_TITLE"),
			_T("PUBLISH_DATE"),
			_T("REC_KEY"),
			_T("BINDING_BOOK_KEY"),
			_T("ACQ_CODE"),
			_T("ACQ_YEAR"),
			_T("WORKING_STATUS"),
			_T("REMARK"),
			_T("VOL_SORT_NO")
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

	return 0;
}

INT CSE_BINDING_TERMIN_03::DoSubProc( INT f_nIdx )
{
	INT ids, nRowCnt;

	// ..............................................................................................................
	// copy base 
	// ..............................................................................................................
	ids = CopyDataManager( _T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"), f_nIdx, _T("DM_SE_BINDING_TERMIN_BOOK"), -1 );
	if( ids ) return -1;

	CESL_DataMgr *pBookDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BOOK"));
	if( pBookDataMgr == NULL ) return -2;

	nRowCnt = pBookDataMgr->GetRowCount();
	
	// ..............................................................................................................
	// input etc
	// ..............................................................................................................
	
	// put STATUS_D
	ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BOOK"), _T("STATUS_D"), _T("�������"), nRowCnt-1 );
	if( ids ) return -3;

	// put MOD_YN
	ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BOOK"), _T("MOD_YN"), _T("Y"), nRowCnt-1 );
	if( ids ) return -4;

	// put STATUS
	ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BOOK"), _T("WORKING_STATUS"), _T("SEB111N"), nRowCnt-1 );
	if( ids ) return -6;

	// put BINDING_BOOK_KEY
	ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BOOK"), _T("BINDING_BOOK_KEY"), _T(""), nRowCnt-1 );
	if( ids ) return -7;

	// ..............................................................................................................
	// delete base
	// ..............................................................................................................
	CESL_DataMgr *pBindInfoDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"));
	if( pBindInfoDataMgr == NULL ) return -8;

	ids = pBindInfoDataMgr->DeleteRow(f_nIdx);
	if( ids ) return -9;

	nRowCnt = pBindInfoDataMgr->GetRowCount();

	// ..............................................................................................................
	// change bindbook
	// ..............................................................................................................
	CString sRowCnt;
	sRowCnt.Format( _T("%d"), nRowCnt );

	ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK"), _T("ORDER_BOOK_CNT"), sRowCnt, 0 );
	if( ids ) return -10;

	return 0;
}

VOID CSE_BINDING_TERMIN_03::InitDisplay()
{
	INT ids, i, nRowCnt;

	CString sBindingKey, sSpeciesKey, sStatus, sData;
	CString strQuery;

	// binding book info
	ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("REC_KEY"), sBindingKey, m_nIdx );
	if( ids < 0 ) return;
	
	
	strQuery.Format( _T("V.REC_KEY = B.VOL_KEY AND B.BINDING_BOOK_KEY = %s ")
					 , sBindingKey );
	

	CESL_DataMgr *pBindInfoDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"));
	if( pBindInfoDataMgr == NULL ) return;

	ids = pBindInfoDataMgr->RefreshDataManager( strQuery );
	if( ids < 0 ) return;

	nRowCnt = pBindInfoDataMgr->GetRowCount();


	//modified by loveisverb
	//���� ��ȣ��,��� ����â
	if(nRowCnt>0)
	{
		CString strResult[2];
		strQuery.Format(_T("SELECT V.VOL_TITLE, V.REMARK FROM SE_VOL_TBL V ")
						_T("WHERE V.REC_KEY IN (SELECT VOL_KEY FROM SE_BOOK_TBL WHERE REC_KEY = %s)"),sBindingKey);

		pBindInfoDataMgr->GetValuesQuery(strQuery,strResult,2);
		
		CESL_ControlMgr* pCM = FindCM(_T("CM_SE_ORDER_BOOK"));
		
		pCM->SetControlMgrData(_T("VOL_TITLE"),strResult[0]);
		pCM->SetControlMgrData(_T("REMARK"),strResult[1]);

	}


	for( i = 0 ; i < nRowCnt ; i++ ) {

		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"), _T("WORKING_STATUS"), sStatus, i );
		if( ids < 0 ) return;
		
		if ( sStatus == _T("SEB111N") ) {

			ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"), _T("STATUS_D"), _T("�������"), i );
			if( ids < 0 ) return;
		}
		else if( sStatus == _T("SEB113N") ) {
		
			ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"), _T("STATUS_D"), _T("�˼����"), i );
			if( ids < 0 ) return;
		}
	}
		



	// jupsu info
	ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("SPECIES_KEY"), sSpeciesKey, m_nIdx );
	if( ids < 0 ) return;

	strQuery.Format( _T("V.REC_KEY = B.VOL_KEY AND B.SPECIES_KEY = %s ")
					 _T(" AND B.WORKING_STATUS = 'SEB111N' "), sSpeciesKey );

	CESL_DataMgr *pBookDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BOOK"));
	if( pBookDataMgr == NULL ) return;

	ids = pBookDataMgr->RefreshDataManager( strQuery );
	if( ids < 0 ) return;

	nRowCnt = pBookDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ ) {

		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BOOK"), _T("WORKING_STATUS"), sStatus, i );
		if( ids < 0 ) return;

		if ( sStatus == _T("SEB111N") ) {

			ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BOOK"), _T("STATUS_D"), _T("�������"), i );
			if( ids < 0 ) return;
		}
		else if( sStatus == _T("SEB113N") ) {
	
			ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BOOK"), _T("STATUS_D"), _T("�˼����"), i );
			if( ids < 0 ) return;
		}
	}

	// bind book
	strQuery.Format( _T("B.REC_KEY = %s "), sBindingKey);

	CESL_DataMgr *pBindBookDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BINDBOOK"));
	if( pBindBookDataMgr == NULL ) return;

	ids = pBindBookDataMgr->RefreshDataManager( strQuery );
	if( ids < 0 ) return;

	nRowCnt = pBindBookDataMgr->GetRowCount();

	if( nRowCnt > 0 ) {

		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK"), _T("ORDER_BOOK_CNT"), sData, 0 );
		if( ids ) return;
	
		ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_BINDBOOK"), _T("FIRST_ORDER_BOOK_CNT"), sData, 0 );
		if( ids ) return;
	}


	ids = pBindInfoDataMgr->SORT( _T("VOL_SORT_NO") );
	if( ids ) return;

	ids = pBindBookDataMgr->SORT( _T("VOL_SORT_NO") );
	if( ids ) return;

	//
	ids = AllControlDisplay( _T("CM_SE_ORDER_BOOK"), -1 );
	if( ids ) return;

	ids = AllControlDisplay( _T("CM_SE_ORDER_BOOK"), 0 );
	if( ids ) return;

	ids = AllControlDisplay( _T("CM_SE_ACQUISIT_BOOK"), -1 );
	if( ids ) return;
}

BEGIN_EVENTSINK_MAP(CSE_BINDING_TERMIN_03, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_BINDING_TERMIN_03)
	ON_EVENT(CSE_BINDING_TERMIN_03, IDC_LIST_BALJU, -600 /* Click */, OnClickListBalju, VTS_NONE)
	ON_EVENT(CSE_BINDING_TERMIN_03, IDC_LIST_JUPSU, -600 /* Click */, OnClickListJupsu, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_BINDING_TERMIN_03::MakeBindingVolTitle()
{
	//written by loveisverb
	CESL_DataMgr *pBindInfoDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_BINDBOOK_INFO"));
	CESL_ControlMgr* pCM = FindCM(_T("CM_SE_ORDER_BOOK"));
	if(!pBindInfoDataMgr || !pCM) return;
	
	INT nRowCnt = pBindInfoDataMgr->GetRowCount();

	if(0 == nRowCnt) 
	{
		pCM->SetControlMgrData(_T("VOL_TITLE"),_T(""));
	}
	else if(1 == nRowCnt)
	{
		CString strVoltitle;
		strVoltitle = pBindInfoDataMgr->GetCellData(_T("VOL_TITLE"),0);
		pCM->SetControlMgrData(_T("VOL_TITLE"),strVoltitle);
	}
	else if(nRowCnt>1)
	{
		CString strVoltitle1,strVoltitle2;
		CString strBindVolTitle;
		strVoltitle1 = pBindInfoDataMgr->GetCellData(_T("VOL_TITLE"),0);
		strVoltitle2 = pBindInfoDataMgr->GetCellData(_T("VOL_TITLE"),nRowCnt-1);
		strBindVolTitle = strVoltitle1 + _T(" - ") + strVoltitle2;
		pCM->SetControlMgrData(_T("VOL_TITLE"),strBindVolTitle);
	}
}

VOID CSE_BINDING_TERMIN_03::OnClickListBalju() 
{
	/*

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	INT ids;

	// ���� row ǥ��
	ids = ECO_GridLineMgrFunc( _T("CM_SE_ORDER_BOOK"), _T("OrderGrid"), ECO_MakeSelectedRowList, TRUE );
	if( ids ) {
		//RTN_VOID( _T("ECO_GridLineMgrFunc Error!") );
		AfxMessageBox( _T("") );
		return;
	}
	ids = ECO_GridLineMgrFunc( _T("CM_SE_ORDER_BOOK"), _T("OrderGrid"), ECO_SetListIndexSelection );
	if( ids ) {
		//RTN_VOID(_T("ECO_GridLineMgrFunc Error!"));
		AfxMessageBox( _T("") );
		return;
	}	
	CECO_ControlListCtrl *pList = (CECO_ControlListCtrl*)GetDlgItem(IDC_LIST_BALJU);

	if( pNMListView->iSubItem == 1 ) {

		if( pList->GetItemText(pNMListView->iItem, pNMListView->iSubItem) == _T("V") ) {
			pList->SetItemText(pNMListView->iItem, pNMListView->iSubItem, _T("") );
		}
		else {
			pList->SetItemText(pNMListView->iItem, pNMListView->iSubItem, _T("V") );
		}
	}
	
	*pResult = 0;
	*/
}

VOID CSE_BINDING_TERMIN_03::OnClickListJupsu() 
{
	/*
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	INT ids;

	// ���� row ǥ��
	ids = ECO_GridLineMgrFunc( _T("CM_SE_ACQUISIT_BOOK"), _T("AcqGrid"), ECO_MakeSelectedRowList, TRUE );
	if( ids ) {
		//RTN_VOID( _T("ECO_GridLineMgrFunc Error!") );
		AfxMessageBox( _T("") );
		return;
	}
	ids = ECO_GridLineMgrFunc( _T("CM_SE_ACQUISIT_BOOK"), _T("AcqGrid"), ECO_SetListIndexSelection );
	if( ids ) {
		//RTN_VOID(_T("ECO_GridLineMgrFunc Error!"));
		AfxMessageBox( _T("") );
		return;
	}
	CECO_ControlListCtrl *pList = (CECO_ControlListCtrl*)GetDlgItem(IDC_LIST_JUPSU);

	if( pNMListView->iSubItem == 1 ) {

		if( pList->GetItemText(pNMListView->iItem, pNMListView->iSubItem) == _T("V") ) {
			pList->SetItemText(pNMListView->iItem, pNMListView->iSubItem, _T("") );
		}
		else {
			pList->SetItemText(pNMListView->iItem, pNMListView->iSubItem, _T("V") );
		}
	}
	
	*pResult = 0;
	*/
}


HBRUSH CSE_BINDING_TERMIN_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
