// BudgetMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetMgr.h"
#include "BudgetListDlg.h"
#include "BudgetApi.h"
#include "../../ESL\SIReportMgr\SIReportManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetMgr dialog


CBudgetMgr::CBudgetMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM =			NULL;
	m_pGrid =		NULL;
	m_pCM =			NULL;
	m_pDlg =		NULL;
	m_nCurrentIdx	= -1;
	m_strSIAlias	= _T("");


}

CBudgetMgr::~CBudgetMgr()
{	
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
	
}

VOID CBudgetMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BOOL CBudgetMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CBudgetMgr, CDialog)
	//{{AFX_MSG_MAP(CBudgetMgr)
	ON_WM_SIZE()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetMgr message handlers

BOOL CBudgetMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr( _T("�������") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : �������") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_�������"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���"));
	m_pDM = FindDM(_T("DM_�������"));
	
	if(!m_pDM || !m_pGrid || !m_pCM) {
		AfxMessageBox( _T("DM,Grid Error : DM_�������,�׸���") );
		return FALSE;
	}
	
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CBudgetSearch(this);
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

VOID CBudgetMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridBUDGET_MGR);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);	
}


VOID CBudgetMgr::InsertProcedure()
{
	CBudgetListDlg dlg(this);
	dlg.m_nOpenMode = 1;
	dlg.m_pParentDM = m_pDM;
	dlg.m_pParentGrid = m_pGrid;
	dlg.DoModal();

	m_pCM->AllControlDisplay();
	
}

VOID CBudgetMgr::ModifyProcedure()
{
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("���� �� ������ �����ϴ�!"));
		return;		
	}
	else
	{

		CBudgetListDlg dlg(this);
		dlg.m_nOpenMode = -1;


		m_pGrid->SelectMakeList();
		INT cnt = m_pGrid->SelectGetCount();

		if(cnt>0) dlg.m_nMoveOption = 2;
		else dlg.m_nMoveOption = 1;

		dlg.m_nCurrentIdx = m_nCurrentIdx;
		dlg.m_pParentDM = m_pDM;
		dlg.m_pParentGrid = m_pGrid;

		dlg.DoModal();
	}
	m_pCM->AllControlDisplay();
}


VOID CBudgetMgr::DeleteProcedure()
{
	m_pGrid->SelectMakeList();

	INT idx = m_pGrid->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("������ �׸��� �����ϴ�!"));
	    return;
	}
	
	CString msg;
	INT nCount = m_pGrid->SelectGetCount();
	msg.Format(_T("[ %d ] ���� ������ ���� �Ͻðڽ��ϱ�?"),nCount);


	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return; 
	
	BeginWaitCursor();

	CBudgetApi api(this);
	CString strBUDGET_COMPILATION_KEY;


	CString strTITLE;
	INT ids = 0;
	CArray<INT,INT> arrayDeletedRow;

	while(idx>=0)
	{
		strBUDGET_COMPILATION_KEY = m_pDM->GetCellData(_T("REC_KEY"),idx);
		

		ids = api.DeleteBudgetList(m_pDM,
								   strBUDGET_COMPILATION_KEY);
							 
	
		if(ids<0)
		{
			EndWaitCursor();
			strTITLE = m_pDM->GetCellData(_T("��������"),idx);

			msg.Format(_T("[ %s ] ������ �����ϴ� �� ������ �߻��� �۾��� �ߴ��մϴ�!"),strTITLE);
			AfxMessageBox(msg);
			break;
		}
			
		arrayDeletedRow.Add(idx);

		idx = m_pGrid->SelectGetNext();
	}
	
	
	INT nSize = arrayDeletedRow.GetSize();
	INT nDeletedRow;

	for(INT i=0;i<nSize;i++)
	{
		nDeletedRow = arrayDeletedRow.GetAt(nSize-i-1);
		m_pDM->DeleteRow(nDeletedRow);
	}
	
	m_pGrid->Display();
	EndWaitCursor();

	msg.Format(_T("[ %d ] ���� ������ �����Ͽ����ϴ�!"),nSize);
	AfxMessageBox(msg);
	return; 	

}

INT CBudgetMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CBudgetApi api(this);
	CString strTmpData1, strTmpData2;
	
	if(strDMFieldAlias == _T("UDF_��������"))
	{	
		
		strTmpData1 = m_pDM->GetCellData(_T("��������"),nRow);
		if(strTmpData1.IsEmpty() == FALSE)
			m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData1, strTmpData2);
		m_pDM->SetCellData(_T("UDF_��������"), strTmpData2, nRow);
	}

	return 0;
}

VOID CBudgetMgr::SearchProcedure()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
	}
}

VOID CBudgetMgr::OnSendQuery(WPARAM w,LPARAM l)
{

	m_pDM->RefreshDataManager(m_pDlg->m_strQuery);

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("����������"),_T("UDF_����������"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("����������"),_T("UDF_����������"));


	m_pGrid->Display();
	m_nCurrentIdx = -1;
	
	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();

	if(0 == nRowCnt)
	{
		CString str;
		str.Format(_T("�˻� ����� �����ϴ�."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
		AfxMessageBox(str);
	}
	else
	{
		CString str;
		str.Format(_T(" %d ���� �ڷᰡ �˻��Ǿ����ϴ�."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
	}
	m_pCM->AllControlDisplay();


}

BEGIN_EVENTSINK_MAP(CBudgetMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBudgetMgr)
	ON_EVENT(CBudgetMgr, IDC_gridBUDGET_MGR, -600 /* Click */, OnClickgridBUDGETMGR, VTS_NONE)
	ON_EVENT(CBudgetMgr, IDC_gridBUDGET_MGR, -601 /* DblClick */, OnDblClickgridBUDGETMGR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBudgetMgr::OnClickgridBUDGETMGR() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pDM->GetRowCount()) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_nCurrentIdx = row-1;
	}
}

VOID CBudgetMgr::OnDblClickgridBUDGETMGR() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pGrid->GetMouseRow()) return;
	ModifyProcedure();
}

VOID CBudgetMgr::PrintProcedure()
{
EFS_BEGIN

	// ���� DM - DM_���������ȹ
	// ��� DM - DM_���������ȹ���
	// DM �ʱ�ȭ
	//
	INT nRow, nCol, i,j;
	long double dSum=0;
	CString szQuery, szTemp;
	SYSTEMTIME time;
	GetSystemTime(&time);
	CESL_DataMgr *pSrcDM, *pTgtDM;
	pSrcDM = pTgtDM = NULL;

	pSrcDM = FindDM(_T("DM_���������ȹ"));
	pTgtDM = FindDM(_T("DM_���������ȹ���"));

	if(pSrcDM==NULL || pTgtDM==NULL)
	{
		ESLAfxMessageBox(_T("DM Load Failed"));
		return;
	}

	szQuery.Format(_T("CS.BUDGET_COMPILATION_KEY = CC.REC_KEY")
					_T(" AND CC.BUDGET_YEAR = '%d'")
					_T(" GROUP BY BUDGET_CLASS, CODE")
					_T(" ORDER BY BUDGET_CLASS"), time.wYear);

	pSrcDM->RefreshDataManager(szQuery);
	pTgtDM->FreeData();
	
	// SIReport ���Ŀ� �´� �� Ʋ�� �����Ѵ�.
	// �к��ڵ� 8��, �Ұ� 1�� - �� 9���� �ʵ尡 �ʿ�
	// 
	for(i=0; i<BUDGETPLAN_ROWCNT; i++)
	{
		pTgtDM->InsertRow(-1);
	}

	// ���� DM�� ���� ��� DM�� �ε����� ���� �����Ѵ�.
	//
	for(i=0;i<pSrcDM->GetRowCount(); i++)
	{
		nRow = Match_Row_Idx(pSrcDM->GetCellData(i,0));
		nCol = Match_Col_Idx(pSrcDM->GetCellData(i,1));
		pTgtDM->SetCellData(nRow, nCol, pSrcDM->GetCellData(i,2));
	}

	// �հ踦 ���Ѵ�.
	//
	for(i=0; i<BUDGETPLAN_ROWCNT; i++)
	{
		dSum = 0;
		for(j=1; j<BUDGETPLAN_COLCNT; j++)
		{
			dSum+=_ttoi64(pTgtDM->GetCellData(i,j));			
		}
		szTemp.Format(_T("%.0f"), dSum);
		pTgtDM->SetCellData(i,0, szTemp);
	}
	for(i=0; i<BUDGETPLAN_COLCNT; i++)
	{
		dSum=0;
		for(j=0; j<BUDGETPLAN_ROWCNT-1; j++)
		{
			dSum+=_ttoi64(pTgtDM->GetCellData(j,i));
		}
		szTemp.Format(_T("%.0f"), dSum);
		pTgtDM->SetCellData(j, i, szTemp);
	}

	// �ڸ����� ����ش�.
	//
	for(i=0; i<BUDGETPLAN_ROWCNT; i++)
	{
		for(j=0;j<BUDGETPLAN_COLCNT; j++)
		{
			pTgtDM->SetCellData(i,j, AddComma(pTgtDM->GetCellData(i,j)));
		}
	}

	// ��� DM�� SIReport�� ����Ѵ�.
	//
	m_strSIAlias = _T("K2UP_���������ȹ");
	if(!PrintToSIReport())
	{
		ESLAfxMessageBox(_T("����� �Ϸ� ���� ���Ͽ����ϴ�."));
	}

EFS_END
}

INT CBudgetMgr::Match_Row_Idx(CString szIdx)
{
	if(szIdx == _T("01"))			// �ι�����ó
	{
		return 0;
	}
	else if(szIdx == _T("02"))		// ��ȸ����ó
	{
		return 1;
	}
	else if(szIdx == _T("03"))		// ����ó
	{
		return 2;
	}
	else if(szIdx == _T("04"))		// ����ó
	{
		return 3;
	}
	else if(szIdx == _T("05"))		// ü���а�
	{
		return 4;
	}
	else if(szIdx == _T("06"))		// ������ó
	{
		return 5;
	}
	else if(szIdx == _T("07"))		// ������
	{
		return 6;
	}
	else if(szIdx == _T("09"))		// ���絵��
	{
		return 7;
	}


	return -1;
}

INT CBudgetMgr::Match_Col_Idx(CString szIdx)
{
	INT nIdx = _ttoi(szIdx);
	if( 0 < nIdx && 5 >  nIdx )
	{
		return nIdx;
	}
	return -1;
}

BOOL CBudgetMgr::PrintToSIReport()
{
	INT ids;
	CESL_DataMgr *pDM_Dst;

	//��� ���� - SiReport �� ���
	//SI����Ʈ CLASS ����
	//
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	
	if (!pSIReportManager) 
	{
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return FALSE;
	}
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	pSIReportManager->DeleteTempPageFiles();

	//DB���� SI����Ʈ ���� ����
	//
	ids = pSIReportManager->GetSIReportFileFromDB(m_strSIAlias);
	if (ids < 0) 
	{
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return FALSE;
	}

	// ������ �� ����
	// DM ����
	//
	if(m_strSIAlias = _T("K2UP_���������ȹ"))
	{
		pDM_Dst = FindDM ( _T("DM_���������ȹ���") );
	}
	
	if ( pDM_Dst == NULL ) 		return FALSE;
	
	pSIReportManager->SetDataMgr(0, pDM_Dst );
	pSIReportManager->MAKEPAGE_DrawWorkForm(-1);
	
	//Print_SetLastPageLine();

	ids = pSIReportManager->Print2();
	if (pSIReportManager) 
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
	
	return TRUE;
}

INT CBudgetMgr::SIReportEventHandler(CSIReportEventInfo* EventInfo)
{
	CSIReportManager *pSIReportManager = ( CSIReportManager * )EventInfo->m_pSIReportManager;
	if( !pSIReportManager )
		return -1;
	else
		return 0;
}

VOID CBudgetMgr::Print_SetLastPageLine()
{
EFS_BEGIN

	INT ids;

	// XML �� WorkForm��ü�� �����Ѵ�.
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	
	if (pXMLIOMgr == NULL) 
	{
		AfxMessageBox(_T("XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}
	if (pPageWorkForm == NULL) 
	{
		AfxMessageBox(_T("�߰� XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}

	// ���� ������ ������ ��ȣ�� �����´�.
	INT nMaxTmpPageNum = pXMLIOMgr->GetMaxTmpPageNum();
	if (nMaxTmpPageNum <= 0) 
	{
		AfxMessageBox(_T("������ ������ ã�� �� �����ϴ�."));
		return;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// ���� ������ ������ ������ �ҷ��´�.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) 
	{
		AfxMessageBox(_T("�������� �ε� ���� ���Ͽ����ϴ�."));
		return;
	}
	// �ҷ��� WorkForm�߿��� First Sheet�� �����´�.(�߰� ������ ���Ͽ��� Sheet�� �Ѱ����̴�.)
	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) 
	{
		AfxMessageBox(_T("�������� ã�� �� �����ϴ�."));
		return;
	}
	// ������ ������ ó���� �̹� �Ǿ� �ִ��� ����. �̹� ó���Ǿ� �ִٸ� �ι� �� �ʿ� ����.
	CSIReportObject * pObject = pSheet->FindObject(_T("�߰��ȸ�����������"));
	if (pObject) return;
	// �ܺ����̺� �� �ݺ� ���̺��� �����´�.(���̺� �̸��� ��Ȳ�� ���� ������ ��)
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_27"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_271"));
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
	if (InLeft.bottom < OutLeft.bottom) 
	{
		// ������ �������� Rect�� ����Ѵ�.
		NewRect.top = InLeft.bottom;
		NewRect.bottom = InLeft.bottom;
		NewRect.left = InLeft.left;
		NewRect.right = InRight.right;

		// ������ �������� �߰� ������ ���Ͽ� �߰��Ѵ�.
		CSIReportObject_Line *pLine = (CSIReportObject_Line *)pSheet->AddNewObject(OBJECT_LINE);
		if (pLine) 
		{
			pLine->SetName(_T("�߰��ȸ�����������"));
			pLine->SetRect(NewRect);
		}

		// ����� �߰������� ������ �����Ѵ�.
		ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	}

	// ������ Object�� ����
	if (pXMLIOMgr) 
	{
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}
	if (pPageWorkForm) 
	{
		delete pPageWorkForm;
		pPageWorkForm = NULL;
	}
	return;

EFS_END

}

CString CBudgetMgr::AddComma(CString szVal)
{	
	INT nLen = szVal.GetLength();
	if(nLen <= 3) return szVal;
	INT nCnt = 0;
	CString szResult;
	CString szTemp = ReverseString(szVal);

	for(INT i=0; i<nLen; i++)
	{		
		if( nCnt ==3 )
		{
			nCnt = 0;
			szResult+=',';
		}
		szResult += szTemp.GetAt(i);
		nCnt++;
	}

	return (ReverseString(szResult));
}

CString CBudgetMgr::ReverseString(CString str)
{
	CString szResult = str;
	INT nLen = str.GetLength();
	for(INT i=0; i<nLen; i++)
	{
		szResult.SetAt(i, str.GetAt(nLen-i-1));
	}

	return szResult;
}

HBRUSH CBudgetMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
