// SE_BINDING_TRANSFER_01.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_TRANSFER_01.h"
#include "SE_BINDING_TRANSFER_02.h"

#include "../../../ESL/KOLAS3_MAIN/KOLAS3_MAINApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_TRANSFER_01 dialog

CSE_BINDING_TRANSFER_01::CSE_BINDING_TRANSFER_01(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pDlg = NULL;
}

CSE_BINDING_TRANSFER_01::~CSE_BINDING_TRANSFER_01()
{
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
}

VOID CSE_BINDING_TRANSFER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_TRANSFER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_BINDING_TRANSFER_01, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_TRANSFER_01)
	ON_WM_SIZE()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_TRANSFER_01 message handlers

BOOL CSE_BINDING_TRANSFER_01::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CSE_BINDING_TRANSFER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_TRANSFER_01")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_SE_BINDING_TRANSFER_01"));
	if(!pDM) return FALSE;
	
	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CSE_BINDING_TRANSFER_02(this);
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

VOID CSE_BINDING_TRANSFER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_BASE);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 5, cx-10, cy-10);
	pGrid->MoveWindow(0, 0, cx, cy);
	
}

VOID CSE_BINDING_TRANSFER_01::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
	}
}

VOID CSE_BINDING_TRANSFER_01::OnSendQuery( WPARAM w, LPARAM l )
{
	INT ids;

	m_Query	= m_pDlg->m_strQuery;

	if( !m_Query.Compare(_T("NO_DATA") ) ) {

		ids = AllControlDisplay( _T("CM_SE_BINDING_TRANSFER_01"), -1);
		if( ids ) return ;

		AfxMessageBox(_T("�˻������ �����ϴ�."));
		return ;
	}

	ids = BaseDisplay();
	if( ids <  0 ) AfxMessageBox( _T("Display Error!") );
	if( ids == 0 ) {

		ids = AllControlDisplay( _T("CM_SE_BINDING_TRANSFER_01"), -1);
		if( ids ) return ;

		AfxMessageBox(_T("�˻������ �����ϴ�."));
		return ;
	}

	if( ids > 0 )
		m_pDlg->ShowWindow(SW_HIDE);
}

// optionchange & sorting
INT CSE_BINDING_TRANSFER_01::BaseDisplay() 
{
	INT ids, nRowCnt, i;
	
	CString sSpeciesKey, sBindVolCnt;
	CString sQuery, sData, sKey1, sKey2;
	CString sSeqNo, sYear;


	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_TRANSFER_01"));
	if( pDataMgr == NULL ) return -1;

	ids = pDataMgr->RefreshDataManager( m_Query );
	if( ids < 0 ) return -1;

	nRowCnt = pDataMgr->GetRowCount();
	
	CString strRFIDSerial;

	for( i = 0 ; i < nRowCnt ; i++ ) {
		
		// ��������
		ids = GetDataMgrData( _T("DM_SE_BINDING_TRANSFER_01"), _T("BINDING_YEAR"), sYear, i );
		if( ids < 0 ) return -1;
		
		ids = GetDataMgrData( _T("DM_SE_BINDING_TRANSFER_01"), _T("BINDING_SEQ_NO"), sSeqNo, i );
		if( ids < 0 ) return -1;

		sData = sYear + _T("-") + sSeqNo;
		ids = SetDataMgrData( _T("DM_SE_BINDING_TRANSFER_01"), _T("ORDER_SEQ_NO"), sData, i );
		if( ids < 0 ) return -1;


		//RFID����
		strRFIDSerial = _T("");
		strRFIDSerial = pDataMgr->GetCellData(_T("SB_RFID"),i);
		strRFIDSerial.TrimLeft();
		strRFIDSerial.TrimRight();
		
		if(strRFIDSerial.IsEmpty())
		{
			pDataMgr->SetCellData(_T("UDF_RFID"),_T("N"),i);
		}
		else
		{
			pDataMgr->SetCellData(_T("UDF_RFID"),_T("Y"),i);
		}

	}
	

	ids = AllControlDisplay( _T("CM_SE_BINDING_TRANSFER_01"), -1);
	if( ids ) return -1;

	CString str;
	str.Format( _T("%d ���� �ڷᰡ �˻��Ǿ����ϴ�.") ,nRowCnt );
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));

	return nRowCnt;
}

// SER111N(����ΰ��ڷ�)
/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �������>>����ΰ��ڷῡ�� ������� �ΰ��ϴ� �۾��� �����Ѵ�.
//	 �۾����°��� SEB114N -> SER111N ���� ����ȴ�.
//
// [ �̴Ͻ��� ]
// 1. ����� CM �� Grid�� �����Ѵ�.
// 2. Grid�� ����üũ�� Row���� List�� �����Ѵ�.
// 3. Grid�� üũ�� Row�� ������ ���´�.
// 4. ����� DM�� �����Ѵ�.
// 5. ������ Grid �ڷ��(Row)�� �� ó�� ���� �ε����� �����´�.
// 6. Query�� �߼� �غ��Ѵ�.
// 7. ������ �ڷ���� REC_KEY�� ������ �ڷ���¸� �����ϴ� �������� �ۼ��Ѵ�.
//		- SE_BOOK_TBL : SEB114N -> SER111N���� ����
// 8. �ۼ��� �������� �����Ѵ�.
// 9. ������ �ݴ´�.
// 10. ����ΰ�ó���� �ڷ���� Grid���� �����Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
INT CSE_BINDING_TRANSFER_01::ProcTrans() 
{
	//=======================================================================
	// ���� ����
	//=======================================================================
	INT ids, nRowCnt, nIdx, i=0;
	CString sRecKey, sSql, sMsg;

	//=======================================================================
	// 1. ����� CM �� Grid�� �����Ѵ�.
	//=======================================================================
	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_TRANSFER_01") );	
	if( pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_SE_BINDING_TRANSFER_01]�� ������ �� �����ϴ�."));
		return -1;
	}

	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;
	if( pGrid == NULL )
	{
		ESLAfxMessageBox(_T("[CM_SE_BINDING_TRANSFER_01::MainGrid]�� ������ �� �����ϴ�."));
		return -1;
	}


	//=======================================================================
	// 2. Grid�� ����üũ�� Row���� List�� �����Ѵ�.
	//=======================================================================
	ids = pGrid->SelectMakeList() ;
	if( ids < 0 ) 
	{
		ESLAfxMessageBox( _T("���� ó���� �����Ͽ����ϴ�.") );
		return -1;
	}

	//=======================================================================
	// 3. Grid�� üũ�� Row�� ������ ���´�.
	//=======================================================================
	nRowCnt = pGrid->SelectGetCount();
	if( nRowCnt == 0 ) 
	{
		ESLAfxMessageBox( _T("�ڷḦ �����Ͻʽÿ�.") );
		return -2;
	}
	if( nRowCnt <  0 ) 
	{
		ESLAfxMessageBox( _T("�׸����� ���������� �������µ� �����Ͽ����ϴ�.") );
		return -3;
	}


	//=======================================================================
	// 4. ����� DM�� �����Ѵ�.
	//=======================================================================
	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_TRANSFER_01"));
	if( pDataMgr == NULL ) 
	{
		ESLAfxMessageBox( _T("[DM_SE_BINDING_TRANSFER_01]�� ������ �� �����ϴ�.") );
		return -4;
	}


	//=======================================================================
	// 5. ������ Grid �ڷ��(Row)�� �� ó�� ���� �ε����� �����´�.
	//=======================================================================
	nIdx = pGrid->SelectGetHeadPosition();
	if( nIdx == -1 ) 
	{
		ESLAfxMessageBox( _T("ù �������� ���� ���߽��ϴ�.") );
		return -5;
		}

	//=======================================================================
	// 6. Query�� �߼� �غ��Ѵ�.
	//=======================================================================
	ids = pDataMgr->StartFrame();
	if( ids < 0 ) 
	{
		ESLAfxMessageBox( _T("�ΰ�ó���� �����Ͽ����ϴ�!") );
		return -9;
		}
		


	//=======================================================================
	// 7. ������ �ڷ���� REC_KEY�� ������ �ڷ���¸� �����ϴ� �������� �ۼ��Ѵ�.
	//		- SE_BOOK_TBL : SEB114N -> SER111N���� ����
	//=======================================================================
	for( i=0; i<nRowCnt; i++)
	{
		// Grid ������ �ڷ��� �ε����� ���������� �о�Դ��� ���Ѵ�.
		if( nIdx < 0 )
		{
			ESLAfxMessageBox( _T("�ΰ�ó���� �����Ͽ����ϴ�.") );
			pDataMgr->EndFrame();
			return -9;
		}
		// WORKING_STATUS, BINDING_STATUS
		ids = GetDataMgrData( _T("DM_SE_BINDING_TRANSFER_01"), _T("REC_KEY"), sRecKey, nIdx );
		if( ids < 0 ) return -6;

		// ����å
		sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SER111N', REG_TRANSFER_DATE = '%s' WHERE REC_KEY = %s;"), GetTodayDate(), sRecKey );
		pDataMgr->AddFrame(sSql);


		// ���å
		sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SER111N', REG_TRANSFER_DATE = '%s' WHERE BINDING_BOOK_KEY = %s;"), GetTodayDate(), sRecKey );
		pDataMgr->AddFrame(sSql);

		nIdx = pGrid->SelectGetNext();		
	}


	//=======================================================================
	// 8. �ۼ��� �������� �����Ѵ�.
	//=======================================================================
	ids = pDataMgr->SendFrame();
	if( ids < 0 ) 
	{
		ESLAfxMessageBox( _T("�ΰ�ó���� �����Ͽ����ϴ�!") );
		return -9;
	}


	//=======================================================================
	// 9. ������ �ݴ´�.
	//=======================================================================
	pDataMgr->EndFrame();
	if( ids < 0 ) 
	{
		ESLAfxMessageBox( _T("�ΰ�ó���� �����Ͽ����ϴ�!") );
		return -9;
	}


	//=======================================================================
	// 9. ����ΰ�ó���� ���� �޽����� �����ش�.
	//=======================================================================
	sMsg.Format( _T("%d���� �ڷḦ �ΰ�ó�� �Ͽ����ϴ�."), nRowCnt );
	AfxMessageBox(sMsg);


	//=======================================================================
	// 10. ����ΰ�ó���� �ڷ���� Grid���� �����Ѵ�.
	//=======================================================================
	ids = BaseDisplay();
	if( ids < 0 ) 
	{
		ESLAfxMessageBox( _T("ȭ�� ���ſ� �����Ͽ����ϴ�.") );
		return -11;
	}

	return 0;
}


INT CSE_BINDING_TRANSFER_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}

VOID CSE_BINDING_TRANSFER_01::Print_SetLastPageLine(VOID)
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

VOID CSE_BINDING_TRANSFER_01::Print_List() 
{
  INT ids;
  CString ReportAlias ;
  ReportAlias = _T("K2UP_�����ΰ���");

  CESL_DataMgr *pDM = FindDM( _T("DM_SE_BINDING_TRANSFER_01") );
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

CString CSE_BINDING_TRANSFER_01::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}
HBRUSH CSE_BINDING_TRANSFER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
