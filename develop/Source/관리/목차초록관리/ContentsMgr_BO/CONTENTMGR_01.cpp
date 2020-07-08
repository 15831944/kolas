// CONTENTMGR_01.cpp : implementation file
//

#include "stdafx.h"
#include "CONTENTMGR_01.h"
#include "CONTENTMGR_02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_01 dialog


CCONTENTMGR_01::CCONTENTMGR_01(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CCONTENTMGR_01::~CCONTENTMGR_01()
{

}

BOOL CCONTENTMGR_01::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CCONTENTMGR_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCONTENTMGR_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCONTENTMGR_01, CDialog)
	//{{AFX_MSG_MAP(CCONTENTMGR_01)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_01 message handlers




VOID CCONTENTMGR_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_Grid_CONTENT);
	if (pGrid->GetSafeHwnd() == NULL) return;
    pGrid->MoveWindow(0, 0, cx, cy);
}

BOOL CCONTENTMGR_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("SM_CONTENTMGR_MAIN")) <0 )
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}

	AfxInitRichEdit();


    m_TocEditdlg.MyCreate(this,CRect(0,10,11,11),_DAN_SINGLE);
  	m_ChorokDlg.MyCreate(this,&m_TocEditdlg,CRect(0,10,11,11));
	

    m_CurrentCM   = _T("CM_CONTENTMGR_MAIN") ;
	m_CurrentDM   = _T("DM_BO_CONTENTMGR_SPECIES") ;
	m_CurrentGrid = _T("MainGrid_CONTENT");    

	AllControlDisplay (m_CurrentCM ,-1) ;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CCONTENTMGR_01::OnInsert_TOC_ABS ()
{
	CCONTENTMGR_02 dlg(0,this);
	dlg.DoModal() ;
    
	return 0 ;
}

INT CCONTENTMGR_01::OnEdit_TOC_ABS ()
{
	CCONTENTMGR_02 dlg(1,this);
	dlg.DoModal() ;
    
	return 0 ;
}


VOID CCONTENTMGR_01::View(INT nType)
{
	// Initial
    BOOL bPrev,bNext;
	CESL_DataMgr* pDataMgr;
	
	pDataMgr = FindDM(_T("DM_�����ʷ�")) ;

	CString strShow,strRecKey,strYN;

	CESL_ControlMgr *pCM = FindCM(m_CurrentCM) ;
	if(pCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return ;
	}

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (m_CurrentGrid) ;
	if(pGrid == NULL) {
		AfxMessageBox(_T("FindControl() Error~!")) ;
		return ;
	}
    INT RowIndex ; 
     
	pGrid->SelectMakeList ();
	

	if(pGrid->SelectGetCount () == 0)
	{
		AfxMessageBox(_T("�׸��� ������ �ֽʽÿ�~!")) ;
		return ;
	}


	RowIndex = pGrid->GetIdx () ;

	CString Species_Key ;
    CString sOption ;
    INT ids ;
	ids = GetDataMgrData (m_CurrentDM ,_T("IBS_��KEY"),Species_Key,RowIndex) ;
    sOption.Format (_T("toc.SPECIES_KEY = %s"),Species_Key) ;

    ids = pDataMgr->RefreshDataManager (sOption); 
    
	bPrev = FALSE ;
	bNext = FALSE ;

	if(nType==0){

        pDataMgr->GetCellData (_T("����"),0,strShow);

		//Test �� ���� �� �� ////////////////////////////
		//strShow = _T("<id>KMO199700001\r\n<call>01$a021.7$b��26e$c4\r\n<count>\r\n<date>1997\r\n<pub>�����߾ӵ�����\r\n<sertitle>\r\n<title>����������ȭ\r\n<body>����\r\nǥ���� = 0\r\n�߰��� = i\r\n���� = ii\r\n��1�� ���� = 1\r\n ��1�� ��Ȳ = 2\r\n ��2�� ������ = 5\r\n��2�� ���� = 20\r\n ��1�� �ذ��� = 20\r\n ��2�� ���İ�ȹ = 30\r\n��3�� ��� = 55\r\n������ = 155\r\n</body>\r\n<table>ǥ����\r\n<ǥ1>�ѱ��缭���� ��ġ��Ȳ = 15\r\n<ǥ2>�Ϻ��� �б������� �缭���� ��ġ���� = 22\r\n</table>\r\n<figure>�׸�����\r\n[�׸�1]�缭������ ���ɺ� �з� = 8\r\n[����1]�б� ������ = 12\r\n</figure>\r\n<body>Contents\r\ntitle page = 0\r\nReward = i\r\ncontents = ii\r\nchapter 1 preface = 1\r\n section 1 introduction = 2\r\n section 2 problem = 5\r\n</body>") ;
		/////////////////////////////////////////////////
		
		m_TocEditdlg.SetTocFromString(strShow);
		m_TocEditdlg.ShowViewer(bPrev,bNext);

	}else{
		pDataMgr->GetCellData (_T("�ʷ�"),0,strShow);
		m_ChorokDlg.SetChorok(strShow);
		m_ChorokDlg.ShowViewer(bPrev,bNext);
	}
	return;
    AfxMessageBox(_T("���⿡ �����Ͽ����ϴ�."));
	
}


INT CCONTENTMGR_01::OnViewMoc()
{
    View(0) ;
	return 0 ;						
}

INT CCONTENTMGR_01::OnViewCho()
{
    View(1);
	return 0 ;
}

INT CCONTENTMGR_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}

VOID CCONTENTMGR_01::Print_List() 
{
  INT ids;
  CString ReportAlias ;
  ReportAlias = _T("�����ʷ�_����");

  CESL_DataMgr *pDM = FindDM(m_CurrentDM );
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
	//=====================================================
	//2009.05.21 UPDATE BY PJW : ���������� ������� �� �����Ѵ�.
// 	Print_SetLastPageLine();
	CString strChkFile = _T("..\\CFG\\RptStyle\\�����ʷ�_����.xls");
	CFileFind fFind;
	if( !fFind.FindFile(strChkFile) )
	{		
		Print_SetLastPageLine();
	}
	//=====================================================
    
	ids = pSIReportManager->Print2();
    if (pSIReportManager) {
	delete pSIReportManager;
	pSIReportManager = NULL;
    }

	return ;
}

VOID CCONTENTMGR_01::Print_SetLastPageLine(VOID)
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
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_48"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_29"));
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

BEGIN_EVENTSINK_MAP(CCONTENTMGR_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CCONTENTMGR_01)
	ON_EVENT(CCONTENTMGR_01, IDC_Grid_CONTENT, -601 /* DblClick */, OnDblClickGridCONTENT, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CCONTENTMGR_01::OnDblClickGridCONTENT() 
{
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_Grid_CONTENT);
	if( pGrid->GetMouseRow() > 0 )
	{
		CESL_DataMgr *pDM = FindDM(m_CurrentDM );
		if(pDM == NULL)
		{
			AfxMessageBox(_T("FindDM() Error~!"));
			return ;
		}

		if(pDM->GetRowCount () ==0)
		{
			AfxMessageBox(_T("���� �׸��� �����ϴ�."));
			return ;
		}
		
		CCONTENTMGR_02 dlg(0,this);
		dlg.SetDbClickMode (true);
		dlg.DoModal() ;
	}
}


HBRUSH CCONTENTMGR_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


