// SE_BINDING_TERMIN_01.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_TERMIN_01.h"
#include "SE_BINDING_TERMIN_02.h"
#include "SE_BINDING_TERMIN_03.h"

#include "../../../ESL/KOLAS3_MAIN/KOLAS3_MAINApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_TERMIN_01 dialog

CSE_BINDING_TERMIN_01::CSE_BINDING_TERMIN_01(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pDlg = NULL;
	m_nSearchOption = 0;
}

CSE_BINDING_TERMIN_01::~CSE_BINDING_TERMIN_01()
{
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
}

VOID CSE_BINDING_TERMIN_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_TERMIN_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_BINDING_TERMIN_01, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_TERMIN_01)
	ON_WM_SIZE()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_TERMIN_01 message handlers
BOOL CSE_BINDING_TERMIN_01::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CSE_BINDING_TERMIN_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_TERMIN_01")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

	//InitializeTabCtrl();
	
	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CSE_BINDING_TERMIN_02(this);
		m_pDlg->pMain = this->pMain;
		m_pDlg->pParentMgr = (CESL_Mgr*)this;
		m_pDlg->m_pInfo = this->m_pInfo;
		m_pDlg->Create(this);
		m_pDlg->ShowWindow(FALSE);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_BINDING_TERMIN_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_BASE);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);
	
}

VOID CSE_BINDING_TERMIN_01::InitializeTabCtrl()
{
	INT i, nItemCnt;
	CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB);

	nItemCnt = pTabCtrl->GetItemCount();

	for( i = nItemCnt-1 ; i >= 0 ; i-- )
		pTabCtrl->DeleteItem(i);

	TC_ITEM TabCtrlItem;
	CRect rect;
	
	TCHAR *title[] = { _T("�����˼����") , _T("�����˼��ڷ�") , _T("�˼������ڷ�") };

	TabCtrlItem.mask = TCIF_TEXT;

	for( i = 0 ; i < 3 ; i++ ) {

		TabCtrlItem.pszText = title[i];
		pTabCtrl->InsertItem(i, &TabCtrlItem);
	}
}

VOID CSE_BINDING_TERMIN_01::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
	}

}

VOID CSE_BINDING_TERMIN_01::OnSendQuery( WPARAM w, LPARAM l )
{
	INT ids;

	m_Query	= m_pDlg->m_strQuery;
	m_nSearchOption = m_pDlg->m_nSearchOption;

	if( !m_Query.Compare(_T("NO_DATA") ) ) {

		ids = AllControlDisplay( _T("CM_SE_BINDING_TERMIN_01"), -1);
		if( ids ) return ;

		AfxMessageBox(_T("�˻������ �����ϴ�."));
		return ;
	}

	ids = BaseDisplay();
	if( ids <  0 ) AfxMessageBox( _T("Display Error!") );
	if( ids == 0 ) {

		ids = AllControlDisplay( _T("CM_SE_BINDING_TERMIN_01"), -1);
		if( ids ) return ;

		AfxMessageBox(_T("�˻������ �����ϴ�."));
		return ;
	}

	if( ids > 0 )
		m_pDlg->ShowWindow(SW_HIDE);
}

VOID CSE_BINDING_TERMIN_01::ModifyBinding()
{
	INT ids, nRowCnt;

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_TERMIN_01") );
	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	ids = pGrid->SelectMakeList() ;
	if( ids ) {
		AfxMessageBox( _T("SelectMakeList Error!") );
		return;
	}

	nRowCnt = pGrid->SelectGetCount();
	if( nRowCnt == 0 ) {
		AfxMessageBox( _T("�ڷḦ �����Ͻʽÿ�.") );
		return;
	}
	if( nRowCnt <  0 ) {
		AfxMessageBox( _T("�׸����� ���������� �������µ� �����Ͽ����ϴ�.") );
		return;
	}

	pGrid->RefreshSelectedRows();

	CSE_BINDING_TERMIN_03 dlg( this );

	dlg.DoModal();

	ids = BaseDisplay();
	if( ids < 0 ) {
		AfxMessageBox( _T("Display Error!") );
		return;
	}

	pGrid->ReshowSelectedRows();
}

// optionchange & sorting
INT CSE_BINDING_TERMIN_01::BaseDisplay() 
{
	INT ids, nRowCnt, i;
	INT nTotPrice, nPrice;
	
	CString sSpeciesKey, sBindVolCnt;
	CString sBindingPrice, sTotBindingPrice;
	CString sSeqNo, sOrderYear, sOrderNo;
	CString sData, sKey1, sKey2;

	nPrice = nTotPrice = 0;
	
	/*
	CESL_DataMgr* pKeyListDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_01_KEYLIST"));
	if( pKeyListDataMgr == NULL ) return -1;
	*/

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_01"));
	if( pDataMgr == NULL ) return -1;

	ids = pDataMgr->RefreshDataManager( m_Query );
	if( ids < 0 ) return -1;

	nRowCnt = pDataMgr->GetRowCount();
	//nKeyListRowCnt = pKeyListDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ ) {
	
		/*
		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("SPECIES_KEY"), sKey1, i );
		if( ids < 0 ) return -1;
		
		for( j = 0 ; j < nKeyListRowCnt ; j++ ) {
		
			ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01_KEYLIST"), _T("REC_KEY"), sKey2, j );
			if( ids < 0 ) return -1;

			if( sKey1 == sKey2 ) {
			
				ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01_KEYLIST"), _T("TITLE"), sData, j );
				if( ids < 0 ) return -1;

				ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("TITLE"), sData, i );
				if( ids < 0 ) return -1;

				ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01_KEYLIST"), _T("TITLE_INFO"), sData, j );
				if( ids < 0 ) return -1;

				ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("TITLE_INFO"), sData, i );
				if( ids < 0 ) return -1;
				
				ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01_KEYLIST"), _T("AUTHOR"), sData, j );
				if( ids < 0 ) return -1;

				ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("AUTHOR"), sData, i );
				if( ids < 0 ) return -1;

				ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01_KEYLIST"), _T("PUBLISHER"), sData, j );
				if( ids < 0 ) return -1;

				ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("PUBLISHER"), sData, i );
				if( ids < 0 ) return -1;

				ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01_KEYLIST"), _T("ST_ISSN"), sData, j );
				if( ids < 0 ) return -1;

				ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("ISSN"), sData, i );
				if( ids < 0 ) return -1;
			}
		}
		*/

		// ��������
		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("BINDING_YEAR"), sOrderYear, i );
		if( ids < 0 ) return -1;
		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("BINDING_SEQ_NO"), sSeqNo, i );
		if( ids < 0 ) return -1;
		sOrderNo = sOrderYear + _T(" - ") + sSeqNo;

		ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("ORDER_SEQ_NO"), sOrderNo, i );
		if( ids < 0 ) return -1;

		// ��������
		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("BINDING_PRICE"), sBindingPrice, i );
		if( ids < 0 ) return -1;

		nPrice = _ttoi( (TCHAR*)LPCTSTR(sBindingPrice) );

		nTotPrice += nPrice;
	}
	
	for( i = 0 ; i < nRowCnt ; i++ ) {
		sTotBindingPrice.Format( _T("%d"), nTotPrice );
		ids = SetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("TOT_BINDING_PRICE"), sTotBindingPrice, i );
		if( ids < 0 ) return -1;
	}

	ids = AllControlDisplay( _T("CM_SE_BINDING_TERMIN_01"), -1);
	if( ids ) return -1;

	CString str;
	str.Format( _T("%d ���� �ڷᰡ �˻��Ǿ����ϴ�.") ,nRowCnt );
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));

	return nRowCnt;
}

// .................................................................................................
// �˼�ó�� : SER111N(����ΰ��ڷ�), BINDING_STATUS(3)
// .................................................................................................
INT CSE_BINDING_TERMIN_01::ProcTermin1()
{
	INT i, ids, nRowCnt, nIdx;
	CString sRecKey, sSql;

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_TERMIN_01") );
	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	ids = pGrid->SelectMakeList() ;
	if( ids ) {
		AfxMessageBox( _T("SelectMakeList Error!") );
		return -1;
	}

	nRowCnt = pGrid->SelectGetCount();
	if( nRowCnt == 0 ) {
		AfxMessageBox( _T("�ڷḦ �����Ͻʽÿ�.") );
		return -2;
	}
	if( nRowCnt <  0 ) {
		AfxMessageBox( _T("�׸����� ���������� �������µ� �����Ͽ����ϴ�.") );
		return -3;
	}

	if(3 == m_nSearchOption) //���� �˼��ڷ�� �˻�
	{
		AfxMessageBox( _T("�̹� �˼��Ϸ�� �ڷ��Դϴ�!") );
		return -4;		
	}

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_01"));
	if( pDataMgr == NULL ) return -4;

	ids = pDataMgr->StartFrame();
	if( ids < 0 ) return -5;

	for( i = nRowCnt-1 ; i >= 0 ; i-- ) {

		if( i == nRowCnt-1 ) {
			nIdx = pGrid->SelectGetTailPosition();
			if( nIdx == -1 ) break;
		}
		else {
			nIdx = pGrid->SelectGetPrev();
			if( nIdx == -1 ) break;
		}
		
		// WORKING_STATUS, BINDING_STATUS
		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("REC_KEY"), sRecKey, nIdx );
		if( ids < 0 ) return -6;
		
		//���� ���� ����
		// ����å
		sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SER111N', REG_TRANSFER_DATE = '%s' WHERE REC_KEY = %s;"), GetTodayDate(), sRecKey );
		pDataMgr->AddFrame(sSql);

		// ���å
		sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SER111N', REG_TRANSFER_DATE = '%s' WHERE BINDING_BOOK_KEY = %s;"), GetTodayDate(), sRecKey );
		pDataMgr->AddFrame(sSql);

		sSql.Format( _T("UPDATE SE_BINDING_BOOK_TBL SET BINDING_STATUS = '3' WHERE BOOK_KEY = %s;"), sRecKey );
		pDataMgr->AddFrame(sSql);
	}
	

	ids = pDataMgr->SendFrame();
	pDataMgr->EndFrame();
	if( ids < 0 ) {
		AfxMessageBox( _T("�˼�ó���� �����Ͽ����ϴ�!") );
		return -9;
	}

	
	//modified by loveisverb 2004-03-02
	//��� �ش� ���� ���� �ڷᰡ �˼� ó�� �Ǿ����� ���� ������ WORKING_STATUS�� 2(�˼��Ϸ�)�� �ٲ۴� 
	


	CString sMsg;
	sMsg.Format( _T("%d���� �ڷḦ �˼�ó�� �Ͽ����ϴ�."), nRowCnt );
	AfxMessageBox(sMsg);

	ids = BaseDisplay();
	if( ids < 0 ) {
		AfxMessageBox( _T("Display Error!") );
		return -11;
	}

	return 0;
}

// .................................................................................................
// �˼�ó�� : SEB114N(����ΰ����ڷ�), BINDING_STATUS(3)
// .................................................................................................
INT CSE_BINDING_TERMIN_01::ProcTermin2()
{
	INT i, ids, nRowCnt, nIdx;
	CString sRecKey, sSql;

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_TERMIN_01") );
	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	ids = pGrid->SelectMakeList() ;
	if( ids ) {
		AfxMessageBox( _T("SelectMakeList Error!") );
		return -1;
	}

	nRowCnt = pGrid->SelectGetCount();
	if( nRowCnt == 0 ) {
		AfxMessageBox( _T("�ڷḦ �����Ͻʽÿ�.") );
		return -2;
	}
	if( nRowCnt <  0 ) {
		AfxMessageBox( _T("�׸����� ���������� �������µ� �����Ͽ����ϴ�.") );
		return -3;
	}

	if(3 == m_nSearchOption) //���� �˼��ڷ�� �˻�
	{
		AfxMessageBox( _T("�̹� �˼��Ϸ�� �ڷ��Դϴ�!") );
		return -4;		
	}

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_01"));
	if( pDataMgr == NULL ) return -4;

	ids = pDataMgr->StartFrame();
	if( ids < 0 ) return -5;
	
	for( i = nRowCnt-1 ; i >= 0 ; i-- ) {

		if( i == nRowCnt-1 ) {
			nIdx = pGrid->SelectGetTailPosition();
			if( nIdx == -1 ) break;
		}
		else {
			nIdx = pGrid->SelectGetPrev();
			if( nIdx == -1 ) break;
		}

		// WORKING_STATUS, BINDING_STATUS
		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("REC_KEY"), sRecKey, nIdx );
		if( ids < 0 ) return -6;

		// ����å
		sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SEB114N' WHERE REC_KEY = %s;"), sRecKey );
		pDataMgr->AddFrame(sSql);


		// ���å
		sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SEB114N' WHERE BINDING_BOOK_KEY = %s;"), sRecKey );
		pDataMgr->AddFrame(sSql);

		sSql.Format( _T("UPDATE SE_BINDING_BOOK_TBL SET BINDING_STATUS = '3' WHERE BOOK_KEY = %s;"), sRecKey );
		pDataMgr->AddFrame(sSql);

	}

	ids = pDataMgr->SendFrame();
	pDataMgr->EndFrame();
	if( ids < 0 ) {
		AfxMessageBox( _T("�˼�ó��(���)�� ���� �Ͽ����ϴ�!") );
		return -9;
	}


	CString sMsg;
	sMsg.Format( _T("%d���� �ڷḦ �˼�ó�� �Ͽ����ϴ�."), nRowCnt );
	AfxMessageBox(sMsg);

	ids = BaseDisplay();
	if( ids < 0 ) {
		AfxMessageBox( _T("Display Error!") );
		return -11;
	}

	return 0;
}

// .................................................................................................
// �˼���� : SEB113N(��������), BINDING_STATUS(2)
// .................................................................................................
INT CSE_BINDING_TERMIN_01::ProcTerminCancel()
{
	INT i, ids, nRowCnt, nIdx;
	CString sRecKey, sSql;

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_TERMIN_01") );
	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	ids = pGrid->SelectMakeList() ;
	if( ids < 0 ) {
		AfxMessageBox( _T("SelectMakeList Error!") );
		return -1;
	}

	nRowCnt = pGrid->SelectGetCount();
	if( nRowCnt == 0 ) {
		AfxMessageBox( _T("�ڷḦ �����Ͻʽÿ�.") );
		return -2;
	}
	if( nRowCnt <  0 ) {
		AfxMessageBox( _T("�׸����� ���������� �������µ� �����Ͽ����ϴ�.") );
		return -3;
	}

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_01"));
	if( pDataMgr == NULL ) return -4;

	ids = pDataMgr->StartFrame();
	if( ids < 0 ) return -5;

	for( i = nRowCnt-1 ; i >= 0 ; i-- ) {

		if( i == nRowCnt-1 ) {
			nIdx = pGrid->SelectGetTailPosition();
			if( nIdx == -1 ) break;
		}
		else {
			nIdx = pGrid->SelectGetPrev();
			if( nIdx == -1 ) break;
		}

		// WORKING_STATUS, BINDING_STATUS
		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("REC_KEY"), sRecKey, nIdx );
		if( ids < 0 ) return -6;

		// ����å
		sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SEB113N', REG_TRANSFER_DATE = '' WHERE REC_KEY = %s;"), sRecKey );
		pDataMgr->AddFrame(sSql);


		// ���å
		sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SEB113N', REG_TRANSFER_DATE = '' WHERE BINDING_BOOK_KEY = %s;"), sRecKey );
		pDataMgr->AddFrame(sSql);

		sSql.Format( _T("UPDATE SE_BINDING_BOOK_TBL SET BINDING_STATUS = '2' WHERE BOOK_KEY = %s;"), sRecKey );
		pDataMgr->AddFrame(sSql);

	
	}

	ids = pDataMgr->SendFrame();
	pDataMgr->EndFrame();
	if( ids < 0 ) {
		AfxMessageBox( _T("�˼���ҿ� �����Ͽ��ϴ�!") );
		return -9;
	}

	CString sMsg;
	sMsg.Format( _T("%d���� �ڷḦ �˼���� �Ͽ����ϴ�."), nRowCnt );
	AfxMessageBox(sMsg);

	ids = BaseDisplay();
	if( ids < 0 ) {
		AfxMessageBox( _T("Display Error!") );
		return -11;
	}

	return 0;
}

// .................................................................................................
// ??? �˼����� : SEB113N(��������),  BINDING_STATUS(4)
// .................................................................................................
INT CSE_BINDING_TERMIN_01::ProcTerminExp()
{
	INT i, ids, nRowCnt, nIdx;
	CString sRecKey, sSql;

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_TERMIN_01") );
	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	ids = pGrid->SelectMakeList() ;
	if( ids < 0 ) {
		AfxMessageBox( _T("SelectMakeList Error!") );
		return -1;
	}

	nRowCnt = pGrid->SelectGetCount();
	if( nRowCnt == 0 ) {
		AfxMessageBox( _T("�ڷḦ �����Ͻʽÿ�.") );
		return -2;
	}
	if( nRowCnt <  0 ) {
		AfxMessageBox( _T("�׸����� ���������� �������µ� �����Ͽ����ϴ�.") );
		return -3;
	}

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_01"));
	if( pDataMgr == NULL ) return -4;

	ids = pDataMgr->StartFrame();
	if( ids < 0 ) return -5;

	for( i = nRowCnt-1 ; i >= 0 ; i-- ) {

		if( i == nRowCnt-1 ) {
			nIdx = pGrid->SelectGetTailPosition();
			if( nIdx == -1 ) break;
		}
		else {
			nIdx = pGrid->SelectGetPrev();
			if( nIdx == -1 ) break;
		}

		// WORKING_STATUS, BINDING_STATUS
		ids = GetDataMgrData( _T("DM_SE_BINDING_TERMIN_01"), _T("REC_KEY"), sRecKey, nIdx );
		if( ids < 0 ) return -6;

		// ����å
		sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SEB113N' WHERE REC_KEY = %s;"), sRecKey );
		pDataMgr->AddFrame(sSql);


		// ���å
		sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SEB113N' WHERE BINDING_BOOK_KEY = %s;"), sRecKey );
		pDataMgr->AddFrame(sSql);

		sSql.Format( _T("UPDATE SE_BINDING_BOOK_TBL SET BINDING_STATUS = '4' WHERE BOOK_KEY = %s;"), sRecKey );
		pDataMgr->AddFrame(sSql);
	}

	ids = pDataMgr->SendFrame();
	pDataMgr->EndFrame();
	if( ids < 0 ) {
	
		AfxMessageBox( _T("�˼����⿡ ���� �Ͽ����ϴ�") );
		return -9;
	}


	CString sMsg;
	sMsg.Format( _T("%d���� �ڷḦ �˼����� �Ͽ����ϴ�."), nRowCnt );
	AfxMessageBox(sMsg);

	ids = BaseDisplay();
	if( ids < 0 ) {
		AfxMessageBox( _T("Display Error!") );
		return -11;
	}

	return 0;
}

INT CSE_BINDING_TERMIN_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}

VOID CSE_BINDING_TERMIN_01::Print_SetLastPageLine(VOID)
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
		// 2010.03.25 DELETE BY KSJ : Excel ������� �ٲ�鼭 Page ��ȣ �ǹ̾�������
		//DEL AfxMessageBox(_T("������ ������ ã�� �� �����ϴ�."));
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
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_5"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_47"));
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

CString CSE_BINDING_TERMIN_01::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

VOID CSE_BINDING_TERMIN_01::Print_List() 
{
  INT ids;
  CString ReportAlias ;
  ReportAlias = _T("K2UP_�����˼�");

  CESL_DataMgr *pDM = FindDM( _T("DM_SE_BINDING_TERMIN_01") );
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
    // ���� �Է� ///////////////////////////////////////////////////////////////
	
	pSIReportManager->SetDataMgr(0, pDM);


	//PRINT//////////////////////////////////////////////////////////////////////////
   /* 
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
   */

	ids = pSIReportManager->MakeXMLPageFiles();
	// ������ ������ �׸�
    Print_SetLastPageLine();
	ids = pSIReportManager->Print2();
    if (pSIReportManager) {
	delete pSIReportManager;
	pSIReportManager = NULL;
    }

	return ;
}
HBRUSH CSE_BINDING_TERMIN_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
