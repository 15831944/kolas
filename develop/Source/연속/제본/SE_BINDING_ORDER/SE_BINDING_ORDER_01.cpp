// SE_BINDING_ORDER_01.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_ORDER_01.h"
#include "SE_BINDING_ORDER_02.h"
#include "SE_BINDING_ORDER_03.h"

#include "../../../ESL/KOLAS3_MAIN/KOLAS3_MAINApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_ORDER_01 dialog

CSE_BINDING_ORDER_01::CSE_BINDING_ORDER_01(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pDlg = NULL;
}

CSE_BINDING_ORDER_01::~CSE_BINDING_ORDER_01()
{
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
}

VOID CSE_BINDING_ORDER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_ORDER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_BINDING_ORDER_01, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_ORDER_01)
	ON_WM_SIZE()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_ORDER_01 message handlers

BOOL CSE_BINDING_ORDER_01::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CSE_BINDING_ORDER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_ORDER_01")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
	
	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CSE_BINDING_ORDER_02(this);
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

VOID CSE_BINDING_ORDER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_BASE);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);
}

VOID CSE_BINDING_ORDER_01::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
	}
}

VOID CSE_BINDING_ORDER_01::OnSendQuery( WPARAM w, LPARAM l )
{
	INT ids;

	m_Query			= m_pDlg->m_strQuery;

	if( !m_Query.Compare(_T("NO_DATA") ) ) {

		ids = AllControlDisplay( _T("CM_SE_BINDING_ORDER_01"), -1);
		if( ids ) return ;

		AfxMessageBox(_T("�˻������ �����ϴ�."));
		return ;
	}

	ids = BaseDisplay();
	if( ids <  0 ) AfxMessageBox( _T("Display Error!") );
	if( ids == 0 ) {

		ids = AllControlDisplay( _T("CM_SE_BINDING_ORDER_01"), -1);
		if( ids ) return ;

		AfxMessageBox(_T("�˻������ �����ϴ�."));
		return ;
	}

	if( ids > 0 )
		m_pDlg->ShowWindow(SW_HIDE);
}

// optionchange & sorting
INT CSE_BINDING_ORDER_01::BaseDisplay() 
{
	INT ids, nRowCnt, i;
	INT nTotPrice, nPrice;
	
	CString sSpeciesKey, sBindVolCnt;
	CString sBindingPrice, sTotBindingPrice;
	CString sData, sKey1, sKey2;
	CString sSeqNo, sYear;

	nPrice = nTotPrice = 0;

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_ORDER_01"));
	if( pDataMgr == NULL ) return -1;

	ids = pDataMgr->RefreshDataManager( m_Query );
	if( ids < 0 ) return -1;

	nRowCnt = pDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ ) {

		// ��������
		ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("ACQ_YEAR"), sYear, i );
		if( ids < 0 ) return -1;
		
		ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("BINDING_SEQ_NO"), sSeqNo, i );
		if( ids < 0 ) return -1;

		sData = sYear + _T("-") + sSeqNo;
		ids = SetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("ORDER_NO"), sData, i );
		if( ids < 0 ) return -1;

		// ��������
		ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("BINDING_PRICE"), sBindingPrice, i );
		if( ids < 0 ) return -1;

		nPrice = _ttoi( (TCHAR*)LPCTSTR(sBindingPrice) );

		nTotPrice += nPrice;

	}
	
	for( i = 0 ; i < nRowCnt ; i++ ) {
		sTotBindingPrice.Format( _T("%d"), nTotPrice );
		ids = SetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("TOT_BINDING_PRICE"), sTotBindingPrice, i );
	}


	ids = AllControlDisplay( _T("CM_SE_BINDING_ORDER_01"), -1);
	if( ids ) return -1;

	CString str;
	str.Format( _T("%d ���� �ڷᰡ �˻��Ǿ����ϴ�.") ,nRowCnt );
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));

	return nRowCnt;
}

VOID CSE_BINDING_ORDER_01::ModifyBindPrice()
{
	INT ids, nRowCnt;

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_ORDER_01") );
	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	ids = pGrid->SelectMakeList() ;
	if( ids ) {
		AfxMessageBox( _T("SelectMakeList Error!") );
		return ;
	}

	nRowCnt = pGrid->SelectGetCount();
	if( nRowCnt == 0 ) {
		AfxMessageBox( _T("�ڷḦ �����Ͻʽÿ�.") );
		return ;
	}
	if( nRowCnt <  0 ) {
		AfxMessageBox( _T("�׸����� ���������� �������µ� �����Ͽ����ϴ�.") );
		return ;
	}

	CSE_BINDING_ORDER_03 dlg(this);
	dlg.DoModal();
}

// 0 : ����ó�� , 1 : �������
INT CSE_BINDING_ORDER_01::DoOrder( INT f_nMode )
{
	INT i, ids, nIdx, nRowCnt;
	TCHAR  *szMsg[2] = { _T("����ó��"), _T("�������") };
	CString sMsg, sBindingStatus;

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_ORDER_01") );
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

	ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("BINDING_STATUS"), sBindingStatus, 0 );
	if( ids ) return -4;

	if( f_nMode == 0 && sBindingStatus == _T("2") ) {
		AfxMessageBox( _T("�̹� ���ֵ� �ڷ��Դϴ�.") );
		return -5;
	}
	else if( f_nMode == 1 && sBindingStatus == _T("1") ) {
		AfxMessageBox( _T("���ִ���ڷḦ ������� �Ͻ� �� �����ϴ�.") );
		return -5;
	}

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_ORDER_01"));
	if( pDataMgr == NULL ) return -6;

	for( i = 0 ; i < nRowCnt ; i++ ) {

		if( i == 0 ) {
			nIdx = pGrid->SelectGetHeadPosition();
			if( nIdx == -1 ) break;
		}
		else {
			nIdx = pGrid->SelectGetNext();
			if( nIdx == -1 ) break;
		}

		ids = ProcBindingOrder( nIdx, f_nMode );
		if( ids ) {
			sMsg.Format( _T("%d��° ���� �ڷ��� %s�� �����Ͽ����ϴ�."), i, szMsg[f_nMode] );
			AfxMessageBox( sMsg );
			return -7;
		}

	}

	sMsg.Format( _T("%d���� �ڷᰡ %s �Ǿ����ϴ�."), nRowCnt, szMsg[f_nMode] );
	AfxMessageBox( sMsg );

	BaseDisplay();

	return 0;
}

INT CSE_BINDING_ORDER_01::ProcBindingOrder( INT f_nIdx, INT f_nMode )
{
	INT ids;
	CString sSql, sBookKey, sBindingInfoKey, sTodayDate;

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_ORDER_01"));
	if( pDataMgr == NULL ) return -1;

	ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("REC_KEY"), sBookKey, f_nIdx );
	if( ids < 0 ) return -2;

	ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("BINDING_INFO_KEY"), sBindingInfoKey, f_nIdx );
	if( ids < 0 ) return -3;

	ids = pDataMgr->StartFrame();
	if( ids < 0 ) return -4;

	// ====================================================================================================
	// 1. SE_BOOK_TBL�� WORKING_STATUS, PREV_WORKING_STATUS
	// ====================================================================================================
	CString sWorkingStatus[2]		= { _T("SEB113N"), _T("SEB112N") };
	CString sPrevWorkingStatus[2]	= { _T("SEB112N"), _T("SEB111N") };
	
	sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s' ")
				 _T("WHERE REC_KEY = %s;"), sWorkingStatus[f_nMode], sPrevWorkingStatus[f_nMode], sBookKey );

	pDataMgr->AddFrame( sSql );


	// 1.1. ���å���� ���°��� UPDATE
	sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s' ")
				 _T("WHERE BINDING_BOOK_KEY = %s;"), sWorkingStatus[f_nMode], sPrevWorkingStatus[f_nMode], sBookKey );

	pDataMgr->AddFrame( sSql );

	// ====================================================================================================
	// 2. SE_BINDING_BOOK_TBL�� BINDING_STATUS, BINDING_YEAR, BINDING_ORDER_DATE, BINDING_PRICE
	// ====================================================================================================
	CTime time = pDataMgr->GetDBTime();
	sTodayDate.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );

	CString sBindingStatus[2]	= { _T("2"), _T("1") };
	CString sBindingYear;
	CString sBindingOrderDate;
	CString sBindingPrice;

	if( f_nMode == 0 ) {
		sBindingYear		= sTodayDate.Left(4);
		sBindingOrderDate	= sTodayDate;
	}
	else if( f_nMode == 1 ) {
		sBindingYear		= _T("");
		sBindingOrderDate	= _T("");
	}
		
	ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("BINDING_PRICE"), sBindingPrice, f_nIdx );
	if( ids < 0 ) return -6;

	sSql.Format( _T("UPDATE SE_BINDING_BOOK_TBL SET BINDING_STATUS = '%s', BINDING_YEAR = '%s', ")
				 _T("BINDING_ORDER_DATE = '%s', BINDING_PRICE = '%s' WHERE REC_KEY = %s;")
				 , sBindingStatus[f_nMode], sBindingYear, sBindingOrderDate, sBindingPrice, sBindingInfoKey );

	pDataMgr->AddFrame( sSql );

	// ====================================================================================================
	// 3. SE_BINDING_SEQ_TBL�� WORKING_STATUS�� '0' -> '1' (����) �Ǵ� '1' -> '0' (�������)
	// ====================================================================================================
	CString sAcqYear, sBindingSeqNo;
	CString sStatus[2]	= { _T("1"), _T("0") };

	ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("ACQ_YEAR"), sAcqYear, f_nIdx );
	if( ids < 0 ) return -6;

	ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("BINDING_SEQ_NO"), sBindingSeqNo, f_nIdx );
	if( ids < 0 ) return -6;

	sSql.Format( _T("UPDATE SE_BINDING_SEQ_TBL SET WORKING_STATUS = '%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = '%s';")
				, sStatus[f_nMode], sAcqYear, sBindingSeqNo );

	pDataMgr->AddFrame( sSql );
	
	ids = pDataMgr->SendFrame();
	pDataMgr->EndFrame();
	if( ids < 0 ) return -8;


	return 0;
}

INT CSE_BINDING_ORDER_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}

VOID CSE_BINDING_ORDER_01::Print_SetLastPageLine(VOID)
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
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_12"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_20"));
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

VOID CSE_BINDING_ORDER_01::Print_List() 
{
  INT ids;
  CString ReportAlias ;
  ReportAlias = _T("K2UP_��������");

  CESL_DataMgr *pDM = FindDM( _T("DM_SE_BINDING_ORDER_01") );
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
HBRUSH CSE_BINDING_ORDER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
