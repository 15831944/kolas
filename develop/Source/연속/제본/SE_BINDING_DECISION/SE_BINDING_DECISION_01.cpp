// SE_BINDING_DECISION_01.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_DECISION_01.h"
#include "SE_BINDING_DECISION_02.h"
#include "SE_BINDING_DECISION_03.h"
#include "SE_BINDING_DECISION_04.h"
#include "SE_BINDING_DECISION_07.h"
#include "..\..\����\SE_API\SeApi.h"

#include "../../../ESL/KOLAS3_MAIN/KOLAS3_MAINApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_DECISION_01 dialog

CSE_BINDING_DECISION_01::CSE_BINDING_DECISION_01(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_nPreSeqNoIdx = 0;
	m_pDlg = NULL;
}

CSE_BINDING_DECISION_01::~CSE_BINDING_DECISION_01()
{
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
}

VOID CSE_BINDING_DECISION_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_DECISION_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_BINDING_DECISION_01, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_DECISION_01)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_eACQUISIT_YEAR, OnChangeeACQUISITYEAR)
	ON_CBN_SELCHANGE(IDC_cORDER_NO, OnSelchangecORDERNO)
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_CBN_DROPDOWN(IDC_cORDER_NO, OnDropdowncORDERNO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_DECISION_01 message handlers

BOOL CSE_BINDING_DECISION_01::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CSE_BINDING_DECISION_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_DECISION_01")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

	//
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_01"));
	if( pDataMgr == NULL ) return -1;

	// =================================================================================================
	// ���ó�¥�� ���Ѵ�.
	// =================================================================================================
	CSeApi api(this);
	m_sTodayDate = api.GetTodayDate();
	m_sAcquisitYear = api.GetCurrentYear();
	
	/*
	CTime time = pDataMgr->GetDBTime();
	m_sTodayDate.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	m_sAcquisitYear = m_sTodayDate.Left(4);
	*/

	CEdit* pEdit;
	pEdit = (CEdit*) GetDlgItem(IDC_eACQUISIT_YEAR);
	if ( pEdit != NULL ) pEdit->SetWindowText(m_sAcquisitYear);
	

	// =================================================================================================
	// ��Ʈ�ѿ� �������� ������
	// =================================================================================================
	INT ids = SelectBaljuChasu();
	if( ids ) {
		AfxMessageBox( _T("SelectBaljuChasu Error!") );
	}
	
	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CSE_BINDING_DECISION_02(this);
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

VOID CSE_BINDING_DECISION_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	//CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	//if(pTab->GetSafeHwnd() == NULL) return;
	//pTab->MoveWindow(5, 5, cx-10, cy-10);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_BASE);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(-2, 35, cx+2, cy);
}

VOID CSE_BINDING_DECISION_01::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
	}
}

VOID CSE_BINDING_DECISION_01::OnSendQuery( WPARAM w, LPARAM l )
{
	INT ids;

	// Mod 2003.06.28 WSW 
	//ids = GetOrderSeqNo();
	//if( ids ) return;

	m_Query	= m_pDlg->m_strQuery;

	if( !m_Query.Compare(_T("NO_DATA") ) ) {

		ids = AllControlDisplay( _T("CM_SE_BINDING_DECISION_01"), -1);
		if( ids ) return ;

		AfxMessageBox(_T("�˻������ �����ϴ�."));
		return ;
	}

	ids = BaseDisplay();
	if( ids <  0 ) AfxMessageBox( _T("Display Error!") );
	if( ids == 0 ) {

		ids = AllControlDisplay( _T("CM_SE_BINDING_DECISION_01"), -1);
		if( ids ) return ;

		AfxMessageBox(_T("�˻������ �����ϴ�."));
		return ;
	}

	if( ids > 0 )
		m_pDlg->ShowWindow(SW_HIDE);
}

// optionchange & sorting
INT CSE_BINDING_DECISION_01::BaseDisplay() 
{
	INT ids = -1;
	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_01"));
	if( pDataMgr == NULL ) return -1;

	ids = pDataMgr->RefreshDataManager( m_Query );
	if( ids < 0 ) return -1;

	INT nRowCnt = pDataMgr->GetRowCount();

	CString sSpeciesKey,sQuery,sBindVolCnt;


	// set binding_vol_cnt
	for(INT i = 0 ; i < nRowCnt ; i++ ) {
	
		ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("REC_KEY"), sSpeciesKey, i );
		if( ids ) return -1;

		sQuery.Format( _T("SELECT COUNT(D.REC_KEY)")
			           _T("FROM SE_BOOK_TBL D WHERE D.SPECIES_KEY = %s ")
					   _T("AND D.BINDED_BOOK_YN <> 'Y' AND D.WORKING_STATUS = 'SEB111N'")
					   ,sSpeciesKey );
		
		ids = pDataMgr->GetOneValueQuery( sQuery, sBindVolCnt);

		ids = SetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("BINDING_VOL_CNT"), sBindVolCnt, i );
		if( ids ) return -1;
	}
	
	ids = AllControlDisplay( _T("CM_SE_BINDING_DECISION_01"), -1);
	if( ids ) return -1;

	CString str;
	str.Format( _T("%d ���� �ڷᰡ �˻��Ǿ����ϴ�.") ,nRowCnt );
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));

	return nRowCnt;
}

VOID CSE_BINDING_DECISION_01::DoOrderSerialNo()
{
	CSE_BINDING_DECISION_07 dlg( this );

	dlg.SetAcqYear(m_sAcquisitYear);
	
	if( IDCANCEL == dlg.DoModal() ) 
		return;

	GetDlgItem(IDC_eACQUISIT_YEAR)	->SetWindowText(dlg.GetAcqYear());
	GetDlgItem(IDC_eORDER_NO)		->SetWindowText(dlg.GetSeqNo());
}

VOID CSE_BINDING_DECISION_01::DoBindingInfo()
{

	INT ids, nRowCnt;
	
	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_DECISION_01") );
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

	CSE_BINDING_DECISION_03 dlg(this);
	if( IDOK == dlg.DoModal() ) {
		ids = BaseDisplay();
		if( ids ) return ;
	}
}

VOID CSE_BINDING_DECISION_01::DoBindingDecision()
{
	INT ids, nRowCnt;

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_DECISION_01") );
	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;
	//=========================================================================
	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_01"));

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

	// Mod 2003.06.28 WSW 
	CString sSeqNo;
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_01"), _T("SEQNO"), sSeqNo, -1 );
	if( ids < 0 ) return ;

	if( sSeqNo.IsEmpty() ) {
		AfxMessageBox( _T("�۾��� �����ϱ� ���� ���������� �����ؾ� �մϴ�.") );
		return;
	}
	//2018�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
	//KOL.RED.019-008 JY ���Ӱ��๰ ��� ����ȭ
	//=============================================================================================
	int prvCnt,aftCnt = -1;
	
	prvCnt = pDataMgr->GetRowCount();
	
	pGrid->RefreshSelectedRows();

	CSE_BINDING_DECISION_04 dlg( this );
	dlg.SetLastSerialNo( m_LastSerialNo );
	dlg.SetAcqYear(m_sAcquisitYear);
	dlg.DoModal();

	ids = BaseDisplay();
	if( ids < 0 ) {
		AfxMessageBox( _T("Display Error!") );
		return;
	}

	aftCnt = pDataMgr->GetRowCount();

	if(prvCnt == aftCnt) pGrid->ReshowSelectedRows();
	
	//=====================================================
	/*
	pGrid->RefreshSelectedRows();

	CSE_BINDING_DECISION_04 dlg( this );
	dlg.SetLastSerialNo( m_LastSerialNo );
	dlg.SetAcqYear(m_sAcquisitYear);
	dlg.DoModal();	
	
	ids = BaseDisplay();
	if( ids < 0 ) {
		AfxMessageBox( _T("Display Error!") );
		return;
	}
	pGrid->ReshowSelectedRows();
	*/
	//=====================================================	
}

INT CSE_BINDING_DECISION_01::GetOrderSeqNo()
{
	INT ids;

	CString sOrderNO, sSeqNo, sWorker, sStatus;
	GetDlgItem(IDC_cORDER_NO) -> GetWindowText(sOrderNO);

	sOrderNO.TrimLeft();	
	sOrderNO.TrimRight();
	
	if(sOrderNO.Compare( _T("���κο�") ) == 0) {
		
		INT iReturn = AfxMessageBox( _T("���������� ���� �ο��Ͻðڽ��ϱ�?"), MB_YESNO );
		if(iReturn == IDYES) {

			// ==========================================================================
			// 1. ���ο� ���������� �ο��ϱ� ���� ���� �ֱٿ� �ο��� ���������� ���´�.
			// ==========================================================================
			CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_ORDER_NO"));
			if( pDataMgr == NULL ) return -1;
			
			// =================================================================================
			// REC_KEY�� ���´�.
			// =================================================================================
			CString sRecKey;
			ids = pDataMgr->MakeRecKey( sRecKey );
			if(ids < 0) return -2;

			sWorker = _T("");
			sStatus = _T("");
			// ===================================================================================================
			// Get SeqNo
			// ===================================================================================================

			CString sSql, sResult;
			sSql.Format( _T("SELECT NVL(MAX(SEQ_NO) + 1, 1) FROM SE_BINDING_SEQ_TBL ")
						 _T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s'"), m_sAcquisitYear );

			ids = pDataMgr->GetOneValueQuery( sSql, sSeqNo );
			if(ids < 0) return -4;

			sSql.Format( _T("INSERT INTO SE_BINDING_SEQ_TBL ")
						 _T("(REC_KEY, ACQ_YEAR, SEQ_NO, SEQ_NO_CREATE_DATE, SEQ_NO_CREATE_WORKER, WORKING_STATUS,MANAGE_CODE) ")
						 _T("VALUES (%s, '%s', %s, '%s', '%s', '%s',UDF_MANAGE_CODE);"),
						 sRecKey, m_sAcquisitYear, sSeqNo, m_sTodayDate, sWorker, sStatus );
			
			pDataMgr->StartFrame();
			pDataMgr->AddFrame(sSql);
			ids = pDataMgr->SendFrame();
			pDataMgr->EndFrame();
			if(ids < 0) return -4;


			// ===================================================================================================
			// �޺��� �ִºκ�
			// ===================================================================================================
			CString sBaljuChasu( sSeqNo ); 
			
			SelectBaljuChasu();
			((CComboBox*)GetDlgItem(IDC_cORDER_NO)) -> SelectString(0,sSeqNo);

			// =================================================================================
			// 5. ���ο� ���������� ����ڿ��� �����ش�.
			// =================================================================================
			CString sMessage = _T("���ο� �������� <") + sSeqNo + _T("��>�� �ο��Ǿ����ϴ�.");
			AfxMessageBox(sMessage);
		} else {
			CString sBaljuChasu;
			GetDlgItem(IDC_cORDER_NO) -> GetWindowText(sSeqNo);
			sSeqNo.TrimLeft();
			sSeqNo.TrimRight();

			if(sSeqNo.Compare( _T("���κο�") )==0) {
				AfxMessageBox( _T("�۾��� �����ϱ� ���� ���������� �����ؾ� �մϴ�.") );

				((CComboBox *)GetDlgItem(IDC_cORDER_NO))->SetCurSel(m_nPreSeqNoIdx);
				
				return -100;
			}
		}
	} 
	else {
		// DB_������������
	}

	return 0;
}

VOID CSE_BINDING_DECISION_01::OnChangeeACQUISITYEAR() 
{
	INT ids;
	CString sTemp;

	GetDlgItem(IDC_eACQUISIT_YEAR) -> GetWindowText(sTemp);
	sTemp.TrimLeft();
	sTemp.TrimRight();

	m_sAcquisitYear = sTemp;

	// ��Ʈ�ѿ� �������� ������
	ids = SelectBaljuChasu();
	if( ids ) {
		AfxMessageBox( _T("SelectBaljuChasu Error") );
		return;
	}
}

INT CSE_BINDING_DECISION_01::SelectBaljuChasu()
{
EFS_BEGIN
	CComboBox* pCombo;
	CString sSql, sNo;

	INT ids, nRowCnt;

	if( m_sAcquisitYear.IsEmpty() ) return -1;

	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_ORDER_NO"));
	if( pDataMgr == NULL ) return -2;

	// �������� ������ ������´�. WORKING_STATUS <> '1' AND WORKING_STATUS <> '2' 
	sSql.Format( _T("ACQ_YEAR = '%s' AND ( WORKING_STATUS IS NULL OR WORKING_STATUS = '0' )"), m_sAcquisitYear );

	ids = pDataMgr->RefreshDataManager( sSql );
	if( ids ) {
		AfxMessageBox( _T("RefreshDataManager Error!") ) ;
		return -3;
	}

	nRowCnt = pDataMgr->GetRowCount();
	
	/*
	if( nRowCnt > 0 ) {
		ids = pDataMgr->SORT( _T("SEQ_NO"), TRUE );
		if( ids ) return -4;
	}
	*/

	pCombo = (CComboBox*)GetDlgItem(IDC_cORDER_NO);
	pCombo -> ResetContent();

	pCombo -> AddString( _T("���κο�") );

	for(INT i = 0 ; i < nRowCnt ; i++) {

		ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_NO"), _T("SEQ_NO"), sNo, i );
		if( ids ) return -5;

		// �ߺ��� ���ֹ�ȣ �������� ����.
		if (pCombo -> FindStringExact(0, sNo) == -1) {
			pCombo -> AddString(sNo);
		}
	}

	pCombo -> SelectString( 0, _T("���κο�") );

	return 0;
EFS_END
return -1;
}

VOID CSE_BINDING_DECISION_01::OnSelchangecORDERNO() 
{
	INT ids;
	CString sSeqNo, sSql;

	//GetOrderSeqNo();

	ids = GetControlData( _T("CM_SE_BINDING_DECISION_01"), _T("SEQNO"), sSeqNo, -1 );
	if( ids < 0 ) return;

	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_ORDER_NO"));
	if( pDataMgr == NULL ) return;

	// set LastSerialNo
	sSql.Format( _T("SELECT NVL(LAST_SERIAL_NO + 1, 1) FROM SE_BINDING_SEQ_TBL ")
				 _T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = '%s' "), m_sAcquisitYear, sSeqNo );

	ids = pDataMgr->GetOneValueQuery( sSql, m_LastSerialNo );
	if( ids < 0 ) return;
}

VOID CSE_BINDING_DECISION_01::OnDropdowncORDERNO() 
{
	m_nPreSeqNoIdx	= ((CComboBox *)GetDlgItem(IDC_cORDER_NO))->GetCurSel();
}

INT CSE_BINDING_DECISION_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}

VOID CSE_BINDING_DECISION_01::Print_SetLastPageLine(VOID)
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
		//AfxMessageBox(_T("������ ������ ã�� �� �����ϴ�."));
		return;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// ���� ������ ������ ������ �ҷ��´�.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) {
		//AfxMessageBox(_T("�������� �ε� ���� ���Ͽ����ϴ�."));
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
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_25"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_37"));
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

VOID CSE_BINDING_DECISION_01::Print_List() 
{
  INT ids;
  CString ReportAlias ;
  ReportAlias = _T("K2UP_�����������");

  CESL_DataMgr *pDM = FindDM( _T("DM_SE_BINDING_DECISION_01") );
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

INT CSE_BINDING_DECISION_01::DoIndexSearch(CString f_strWhere, CStringList& f_slistKey)
{
	INT i, ids, nRowCnt;
	CString strRecKey, strInQuery;

    CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_01_KEYLIST"));
	if( pDataMgr == NULL ) {
		AfxMessageBox( _T("") ) ;
		return -1;
	}

	ids = pDataMgr->RefreshDataManager( f_strWhere );
	if( ids ) {
		AfxMessageBox( _T("RefreshDataManager Error!") ) ;
		return -1;
	}
	
	nRowCnt = pDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ ) {
		pDataMgr->GetCellData(_T("REC_KEY"), i, strRecKey);
		f_slistKey.AddTail( strRecKey );
	}

	return 0;
}

CString CSE_BINDING_DECISION_01::MakeReckeyInQuery(CStringList &f_keyList)
{
	CString Inquery;
	INT cnt = f_keyList.GetCount();
	
	if(cnt == 0) return _T("");
	
	POSITION pos = f_keyList.GetHeadPosition();
	Inquery.Format(_T("%s"), f_keyList.GetNext(pos));
	CString subquery;

	while(pos != NULL)
	{
		subquery.Format(_T(",%s"), f_keyList.GetNext(pos));
		Inquery += subquery;
	}
	CString tmpstr ;
	tmpstr.Format(_T("(%s)"), Inquery);

	return tmpstr;

}



HBRUSH CSE_BINDING_DECISION_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}