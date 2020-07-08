// SE_CENTERUPLOAD_01.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CENTERUPLOAD_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CENTERUPLOAD_01 dialog


CSE_CENTERUPLOAD_01::CSE_CENTERUPLOAD_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CENTERUPLOAD_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CENTERUPLOAD_01::~CSE_CENTERUPLOAD_01()
{
	
}

BOOL CSE_CENTERUPLOAD_01::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CSE_CENTERUPLOAD_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CENTERUPLOAD_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CENTERUPLOAD_01, CDialog)
	//{{AFX_MSG_MAP(CSE_CENTERUPLOAD_01)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CENTERUPLOAD_01 message handlers

BOOL CSE_CENTERUPLOAD_01::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(InitESL_Mgr(_T("SM_SE_CENTERUPLOAD"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}
 
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_CENTERUPLOAD_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid * pGrid = (CMSHFlexGrid *)GetDlgItem(IDC_MainGrid_SE) ;
   	if (pGrid->GetSafeHwnd() == NULL) return;
    pGrid->MoveWindow(0, 0, cx, cy);

}


INT CSE_CENTERUPLOAD_01::ShowModifyDlg ()
{

	CESL_DataMgr *m_pDM = FindDM(_T("DM_SE_CENTERUPLOAD_01")) ;
	if(m_pDM == NULL) 
	{
		AfxMessageBox (_T("FindDM(DM_SE_CENTERUPLOAD_01) Error~!")) ;
		return -1;
	}
    
    CESL_ControlMgr *m_pCM = FindCM(_T("CM_SE_CENTERUPLOAD_01")) ;
	if(m_pCM == NULL)
	{
		AfxMessageBox(_T("FindCM(CM_SE_CENTERUPLOAD_01) Error~!")) ;
		return -2 ;
	}

	CESL_Grid* m_pGrid = (CESL_Grid*)m_pCM->FindControl (_T("MainGrid")) ;
    if(m_pCM == NULL)
	{
		AfxMessageBox(_T("FindControl(MainGrid) Error~!")) ;
		return -2 ;
	}
 
	m_strBookWhere += _T("AND B.WORKING_STATUS IN ('SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL511O')") ;

    m_MarcAdjLaunCher.SetParentInfo(m_pDM, m_pGrid, _T("REC_KEY"), m_strBookWhere, m_pGrid->GetRow() - 1, TRUE);
   
	// ����  
	m_MarcAdjLaunCher.LaunchMarcAdjust(GetParentFrame(), this, _T("SEC111N"), MNG_MODIFY, MODE_SERIAL);
	

    

	return 0 ;
}


INT CSE_CENTERUPLOAD_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}

VOID CSE_CENTERUPLOAD_01::Print_List() 
{
	INT ids;
	CString ReportAlias ;
	

	ReportAlias = _T("���;��ε�_����");
	m_Table_01 = _T("���̺�_7");
	m_Table_02 = _T("���̺�_18") ;

	
	
	CESL_DataMgr *pDM = FindDM(_T("DM_SE_CENTERUPLOAD_01"));
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
  	
	pSIReportManager->SetDataMgr(0, pDM);

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

VOID CSE_CENTERUPLOAD_01::Print_SetLastPageLine(VOID)
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
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(m_Table_01);
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(m_Table_02);
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
HBRUSH CSE_CENTERUPLOAD_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
