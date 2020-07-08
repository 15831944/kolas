// LOC_STATISTICS.cpp : implementation file
//

#include "stdafx.h"
#include "LOC_STATISTICS.h"
// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2018.07.11 ADD BY KHY: ����� ����Ʈ ����
/*//NEW------------------------------------------------------------------------*/
#include "LocStatisticsList.h"
/*//END------------------------------------------------------------------------*/
// KOL.RED.2018.211 ---------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLOC_STATISTICS dialog


CLOC_STATISTICS::CLOC_STATISTICS(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLOC_STATISTICS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nTYPE = 1; //default

	m_pCM = NULL;
	m_pDM_QUERY = NULL;
	m_pDM_RESULT = NULL;
	m_pDM_SUM = NULL;
	m_pDM_PRINT = NULL;
	m_pDM_KDC = NULL;
	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2018.07.11 ADD BY KHY: ����� ����Ʈ ����
/*//NEW------------------------------------------------------------------------*/
	m_pDM_LIST = NULL;
/*//END------------------------------------------------------------------------*/
	// KOL.RED.2018.211 ---------------------------------------------------------------------------
	m_pGrid = NULL;


	m_pApi = NULL;
	m_pAxisWnd = NULL;
	m_pTmpDialog = NULL;
	
	m_pCondWnd = NULL;
	m_pCondWnd1 = NULL;
	m_pCondWnd2 = NULL;
	m_pCondWnd3 = NULL;



	m_strClassName = _T("");
	m_strPrintClassName = _T("");
}

CLOC_STATISTICS::~CLOC_STATISTICS()
{
	if(m_pApi)
	{
		delete m_pApi;
		m_pApi = NULL;
	}

	if(m_pAxisWnd)
	{
		delete m_pAxisWnd;
		m_pAxisWnd = NULL;
	}


	if(m_pTmpDialog)
	{
		delete m_pTmpDialog;
		m_pTmpDialog = NULL;
	}
	
	if(m_pCondWnd1)
	{
		delete m_pCondWnd1;
		m_pCondWnd1 = NULL;
	}
	
	if(m_pCondWnd2)
	{
		delete m_pCondWnd2;
		m_pCondWnd2 = NULL;
	}
	
	if(m_pCondWnd3)
	{
		delete m_pCondWnd3;
		m_pCondWnd3 = NULL;
	}
}

VOID CLOC_STATISTICS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLOC_STATISTICS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLOC_STATISTICS, CDialog)
	//{{AFX_MSG_MAP(CLOC_STATISTICS)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_chkDUP_DEL, OnchkDUPDEL)
	ON_BN_CLICKED(IDC_chkSUMZERODEL, OnchkSUMZERODEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RUN_S, OnRunS)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLOC_STATISTICS message handlers

BOOL CLOC_STATISTICS::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CLOC_STATISTICS::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	CStatic* pStatic1 = (CStatic*)GetDlgItem(IDC_staTITLE);
	CStatic* pStatic2 = (CStatic*)GetDlgItem(IDC_staTITLE2);


	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	if(pStatic1->GetSafeHwnd() == NULL){
		return;
	}
	if(pStatic2->GetSafeHwnd() == NULL){
		return;
	}

	CRect Bar1,Bar2;

	pStatic1->GetWindowRect( &Bar1 );
	pStatic2->GetWindowRect( &Bar2 );

	ScreenToClient( Bar1 );
	ScreenToClient( Bar2 );
	
	//pGrid->MoveWindow(-2, 35, cx+2, cy);
	pGrid->MoveWindow(5, Bar1.Height()+Bar2.Height()+1+20, cx-10, cy-( Bar1.Height()+Bar2.Height()+1)-30 );
	pStatic1->MoveWindow(0, Bar1.top , cx, Bar1.Height());
	pStatic2->MoveWindow(0, Bar2.top , cx, Bar2.Height());
}

BOOL CLOC_STATISTICS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr( _T("����_����_���") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_���") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_����_����_���"));
	m_pDM_QUERY = FindDM(_T("DM_����_����_���_����"));
	m_pDM_RESULT = FindDM(_T("DM_����_����_���_���"));
	m_pDM_SUM = FindDM(_T("DM_����_����_���_�հ�"));
	m_pDM_PRINT = FindDM(_T("DM_����_����_���_���"));
	m_pDM_KDC = FindDM(_T("DM_����_����_���_�з���ȣ"));
	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2018.07.11 ADD BY KHY: ����� ����Ʈ ����
/*//NEW------------------------------------------------------------------------*/
	m_pDM_LIST = FindDM(_T("DM_����_����_���_�����"));;
/*//END------------------------------------------------------------------------*/
	// KOL.RED.2018.211 ---------------------------------------------------------------------------
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("������׸���"));

	if(!m_pCM || !m_pDM_QUERY || !m_pDM_RESULT || !m_pDM_SUM || !m_pDM_PRINT || !m_pDM_KDC || !m_pGrid)
	{
		AfxMessageBox( _T("CM,DM Error : ����_����_���") );
		return FALSE;
	}
	

	InitControl();

	CreateChildDlg();

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CLOC_STATISTICS::InitControl()
{
	CComboBox* pCombo = NULL;
	pCombo = (CComboBox*)m_pCM->FindControl(_T("�������"));
	if(!pCombo) return;
	
	if(1 == m_nTYPE)
	{
		pCombo->InsertString(0,_T("��ϱ��к����"));
		pCombo->InsertString(1,_T("�ڷᱸ�к����"));
		pCombo->InsertString(2,_T("����󵵺����"));
		pCombo->InsertString(3,_T("��ġ��ȣ�����"));
	}
	else if(2 == m_nTYPE)
	{
		pCombo->InsertString(0,_T("��ϱ��к����"));
		pCombo->InsertString(1,_T("�ڷ�Ǳ��к����"));
		pCombo->InsertString(2,_T("���Ա��к����"));
		pCombo->InsertString(3,_T("�谡���º����"));
		pCombo->InsertString(4,_T("����󵵺����"));
		pCombo->InsertString(5,_T("��ġ��ȣ�����"));
	}
	else if(3 == m_nTYPE)
	{
		pCombo->InsertString(0,_T("�ڷᱸ�к����"));
		pCombo->InsertString(1,_T("�ڷ�Ǳ��к����"));
		pCombo->InsertString(2,_T("�谡���º����"));
		pCombo->InsertString(3,_T("����󵵺����"));
	}

	pCombo->SetCurSel(0);
	
	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_chkSUMZERODEL);
	if(pButton) pButton->SetCheck(1);

}

VOID CLOC_STATISTICS::CreateChildDlg()
{
	
	//��ǥ ������ 
	m_pAxisWnd = new CLocStatisticsAxis(this);
	m_pAxisWnd->m_nTYPE = m_nTYPE;

	//��� API
	m_pApi = new CLocStatisticsApi(this);
	m_pApi->m_pDM_QUERY = m_pDM_QUERY;
	m_pApi->m_pDM_RESULT = m_pDM_RESULT;
	m_pApi->m_pDM_SUM = m_pDM_SUM;
	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2018.07.11 ADD BY KHY: ����� ����Ʈ ����
/*//NEW------------------------------------------------------------------------*/
	m_pApi->m_pDM_LIST = m_pDM_LIST;
/*//END------------------------------------------------------------------------*/
	// KOL.RED.2018.211 ---------------------------------------------------------------------------
	m_pApi->m_nTYPE = m_nTYPE;


	if(1 == m_nTYPE)
	{
		m_pCondWnd1 = new CLocStatisticsCond1(this);
		m_pCondWnd1->m_pApi = m_pApi;
	}
	else if(2 == m_nTYPE)
	{
		m_pCondWnd2 = new CLocStatisticsCond2(this);
		m_pCondWnd2->m_pApi = m_pApi;
	}
	else if(3 == m_nTYPE)
	{
		m_pCondWnd3 = new CLocStatisticsCond3(this);
		m_pCondWnd3->m_pApi = m_pApi;

	}
	

}

VOID CLOC_STATISTICS::ShowTmpDlg()
{

	if(1 == m_nTYPE)
	{
		m_pCondWnd = m_pCondWnd1;
	}
	else if(2 == m_nTYPE)
	{
		m_pCondWnd = m_pCondWnd2;
	}
	else if(3 == m_nTYPE)
	{
		m_pCondWnd = m_pCondWnd3;	

	}

	//�� ���� ����
	CString strtmp;
	CString class_name;

	CComboBox* pCombo = NULL;
	pCombo = (CComboBox*)m_pCM->FindControl(_T("�������"));
	if(!pCombo) return;

	pCombo->GetWindowText(strtmp);


	if(_T("����󵵺����") == strtmp) {
		class_name = _T("������ڵ�");
		m_strPrintClassName = _T("����󵵺�");
	}
	else if(_T("�ڷᱸ�к����") == strtmp) {
		class_name = _T("�ڷᱸ��");
		m_strPrintClassName = _T("�ڷᱸ�к�");
	}
	else if(_T("��ϱ��к����") == strtmp) {
		class_name = _T("��ϱ���");
		m_strPrintClassName = _T("��ϱ��к�");
	}
	else if(_T("�ڷ�Ǳ��к����") == strtmp) {
		class_name = _T("�ڷ�Ǳ���");
		m_strPrintClassName = _T("�ڷ�Ǳ��к�");
	}
	else if(_T("�谡���º����") == strtmp) {
		class_name = _T("�谡����");
		m_strPrintClassName = _T("�谡���º�");
	}
	else if(_T("��ġ��ȣ�����") == strtmp) {
		class_name = _T("��ġ��ȣ");
		m_strPrintClassName = _T("��ġ��ȣ��");
	}
	else if(_T("���Ա��к����") == strtmp) {
		class_name = _T("�Լ�����");
		m_strPrintClassName = _T("���Ա��к�");
	}


	m_pAxisWnd->m_strClassName = class_name;
	m_strClassName = class_name; //���� ���� ����

	if(m_pAxisWnd->m_bCreated)
	{
		m_pAxisWnd->SetClassCondition();
	}

	if(!m_pTmpDialog)
	{
		m_pTmpDialog = new CTmpDialog(this, m_pCondWnd ,m_pAxisWnd);
		m_pTmpDialog->m_nTYPE = m_nTYPE;

		if (m_pTmpDialog->Create(this))
		{
			m_pTmpDialog->ShowWindow(SW_SHOW);
			m_pTmpDialog->UpdateWindow();
			m_pTmpDialog->CenterWindow();
		
		}

	}
	else
	{
			m_pTmpDialog->ShowWindow(SW_SHOW);
			m_pTmpDialog->UpdateWindow();
			m_pTmpDialog->CenterWindow();
	}
}

LRESULT CLOC_STATISTICS::OnRunS(WPARAM wParam, LPARAM lParam) 
{
	INT ids;
	
	AfxGetApp()->DoWaitCursor( 1 );
	ids = RunStatistics();
	AfxGetApp()->DoWaitCursor( -1 );
	
	return 0;
}

INT CLOC_STATISTICS::RunStatistics()
{
	
	m_pApi->RunStatisticsProc(m_strClassName);

	GridDisplay();

	return 0;
}

VOID CLOC_STATISTICS::GridDisplay()
{
	INT nRow = m_pDM_RESULT->GetRowCount();
	if(nRow<2) return;
		
	
	//�ߺ� ��� �� 0�� �׸������ ���θ� �ľ��Ѵ�
	bool bDupDel = TRUE;
	bool bZeroShow = TRUE;

	if( ((CButton*)GetDlgItem(IDC_chkDUP_DEL))->GetCheck() == 1 )
		bDupDel = FALSE;
	
	if( ((CButton*)GetDlgItem(IDC_chkSUMZERODEL))->GetCheck() == 1 )
		bZeroShow = FALSE;
	

	//���� ��� DM�� ���DM�� copy�Ѵ�
	CopyResultDMToPrintDM(bDupDel,bZeroShow);

	m_pGrid->ShowWindow(FALSE);
	
	INT nPrintRow = m_pDM_PRINT->GetRowCount();
	INT nGridRow,nGridCol;
	nGridCol = 13;
	nGridRow = nPrintRow+1;
	
	m_pGrid->SetRows(nGridRow);
	m_pGrid->SetCols(0,nGridCol);
	m_pGrid->SetRowHeight(0,300);
	
	CString strData;
	CString strAlias;

	
	//column header(�з���ȣ) 
	m_pGrid->SetRow(0);

	for(INT i=1;i<13;i++)
	{		
		m_pGrid->SetCol(i);

		strAlias.Format(_T("%d"),i);
		strData = m_pDM_KDC->GetCellData(strAlias,0);
		m_pGrid->SetColWidth(i, 0, 1000);
		m_pGrid->SetCellAlignment(4);
		m_pGrid->SetCellFontBold(TRUE);
		m_pGrid->SetText(strData);
	}	
		
	
	for(i=0;i<nPrintRow;i++)
	{
		m_pGrid->SetRow(i+1);
		m_pGrid->SetRowHeight(i+1,600);

		//row header(����)
		m_pGrid->SetCol(0);
		m_pGrid->SetCellFontBold(TRUE);
		m_pGrid->SetCellAlignment(4);
		strData = m_pDM_PRINT->GetCellData(_T("������"),i);
		m_pGrid->SetText(strData);

		
		//������ 
		for(INT j=1;j<12;j++)
		{		
			m_pGrid->SetCol(j);

			strAlias.Format(_T("%d"),j);
			strData = m_pDM_PRINT->GetCellData(strAlias,i);
			m_pGrid->SetCellAlignment(4);
			m_pGrid->SetText(strData);

		}	

		//�� �հ� 
		m_pGrid->SetCol(nGridCol-1);
		strData = m_pDM_PRINT->GetCellData(_T("�հ�"),i);
		m_pGrid->SetCellAlignment(4);
		m_pGrid->SetText(strData);
	}


	m_pGrid->ShowWindow(TRUE);

}

VOID CLOC_STATISTICS::OnchkDUPDEL() 
{
	// TODO: Add your control notification handler code here
	GridDisplay();
}

VOID CLOC_STATISTICS::OnchkSUMZERODEL() 
{
	// TODO: Add your control notification handler code here
	GridDisplay();
}


INT CLOC_STATISTICS::SetGridCellAlignment(CESL_Grid* pGrid)
{
	//��� ����
	INT nColCnt = pGrid->GetCols(0);
	INT nRowCnt = pGrid->GetRows();

	
	
	for( INT i = 0 ; i < nColCnt ; i++ )
	{
		if( i == 0 )
			pGrid->SetColAlignment(i,1);
		else
			pGrid->SetColAlignment(i,4);

		pGrid->SetRow(0);
		pGrid->SetCol(i);
		pGrid->SetCellFontBold(TRUE);
		pGrid->SetCellAlignment(4);

	}

	for( INT j = nRowCnt-1 ; j >= 0 ; j-- )
	{
		pGrid->SetCol(0);
		pGrid->SetRow(j);
		pGrid->SetCellFontBold(TRUE);	
	}

	return 0;
}

VOID CLOC_STATISTICS::MakeOutputFormat(CString& strData)
{
	if(strData.GetLength() == 0) return ;
	
	INT nSpecies,nVol,nBook;
	nSpecies = nVol =  nBook = 0;

	
	INT idx, startidx, lastidx;
	idx = startidx = 0;
	lastidx = strData.GetLength();
	idx = strData.Find(_T(","), startidx);
	
	//�� 
	if(idx > 0) nSpecies = _ttoi((strData.Left(idx)).GetBuffer(0));

	startidx = idx + 1;
	idx = strData.Find(_T(","), startidx);
	
	//��
	if(idx > 0)	nVol += _ttoi((strData.Mid(startidx, idx - 1)).GetBuffer(0));

	startidx = idx + 1;
	
	//å
	nBook = _ttoi((strData.Right(lastidx - startidx)).GetBuffer(0));
	
	strData.Format(_T("%d\n[%d]\n(%d)"),nSpecies,nVol,nBook);
}

VOID CLOC_STATISTICS::MakeAliasArray(CArray<CString,CString>& arrayAlias)
{
	//��� DM���� ���� �հ谡 0�� �ƴ� alias�� array�� �����
	INT nRowCnt = m_pDM_RESULT->GetRowCount();
	INT nSumRowIdx = nRowCnt - 2; //������ �ι�° -> �ߺ� �հ� 
	
	CString strAlias;
	CString strData;


	for(INT i=1;i<12;i++)
	{
		
		strAlias.Format(_T("%d"),i);
		strData = m_pDM_RESULT->GetCellData(strAlias,nSumRowIdx);		
		
		if(IsValueExist(strData))
		{
			arrayAlias.Add(strAlias);
		}
	}

}

BOOL CLOC_STATISTICS::IsValueExist(CString strData)
{
	//������ ���� �����ϴ��� ���� 
	if(strData.GetLength() == 0) return FALSE;
	
	INT nSpecies,nVol,nBook;
	nSpecies = nVol =  nBook = 0;

	
	INT idx, startidx, lastidx;
	idx = startidx = 0;
	lastidx = strData.GetLength();
	idx = strData.Find(_T(","), startidx);
	
	if(idx<0) return FALSE;

	//�� 
	if(idx > 0) nSpecies = _ttoi((strData.Left(idx)).GetBuffer(0));

	startidx = idx + 1;
	idx = strData.Find(_T(","), startidx);
	
	//��
	if(idx > 0)	nVol += _ttoi((strData.Mid(startidx, idx - 1)).GetBuffer(0));

	startidx = idx + 1;
	
	//å
	nBook = _ttoi((strData.Right(lastidx - startidx)).GetBuffer(0));
	
	
	if(nSpecies != 0 || nVol != 0 || nBook != 0)
	{
		return TRUE;
	}
	
	
	return FALSE;
}

INT CLOC_STATISTICS::CopyResultDMToPrintDM(BOOL bDupDel,BOOL bZeroShow)
{
	
	m_pDM_PRINT->FreeData();
	m_pDM_KDC->FreeData();
	m_pDM_KDC->InsertRow(-1);
		

	CString strAlias,strData;

	//�з���ȣ�� ī��
	for(INT i=1;i<12;i++)
	{
		strAlias.Format(_T("%d"),i);
		strData = m_pDM_RESULT->GetCellData(strAlias,0);
		m_pDM_KDC->SetCellData(strAlias,strData,0);
	}

	m_pDM_KDC->SetCellData(_T("12"),_T("�հ�"),0);


	//�����ʹ� ��� DM�� ī��
	INT nRowCnt = m_pDM_RESULT->GetRowCount();
	CString strSum;
	INT nPrintRowIdx;

	for(i=1;i<nRowCnt-2;i++)
	{
		
		if(!bZeroShow) // �հ谡 0�� ���� �����Ѵ� 
		{
			strData = m_pDM_RESULT->GetCellData(_T("�ߺ��հ�"),i);
			if(!IsValueExist(strData)) continue;
		}
		
		
		m_pDM_PRINT->InsertRow(-1);
		nPrintRowIdx = m_pDM_PRINT->GetRowCount()-1;
		
		//������ 
		strData = m_pDM_RESULT->GetCellData(_T("������"),i);
		m_pDM_PRINT->SetCellData(_T("������"),strData,nPrintRowIdx);
	

		//������ 
		for(INT j=1;j<12;j++)
		{
			
			strAlias.Format(_T("%d"),j);
			strData = m_pDM_RESULT->GetCellData(strAlias,i);
			MakeOutputFormat(strData);
			m_pDM_PRINT->SetCellData(strAlias,strData,nPrintRowIdx);
		}	
		

		//�հ�
		if(bDupDel)
		{
			strData = m_pDM_RESULT->GetCellData(_T("�հ�"),i);
		}
		else
		{
			strData = m_pDM_RESULT->GetCellData(_T("�ߺ��հ�"),i);
		}
		
		MakeOutputFormat(strData);
		m_pDM_PRINT->SetCellData(_T("�հ�"),strData,nPrintRowIdx);
	}
	
	//���հ� 
	INT nResultRowIdx;

	m_pDM_PRINT->InsertRow(-1);
	nPrintRowIdx = m_pDM_PRINT->GetRowCount()-1;

	if(bDupDel)
	{
		nResultRowIdx = nRowCnt - 1;
	}
	else
	{
		nResultRowIdx = nRowCnt - 2;
	}
	
	for(i=1;i<12;i++)
	{
			
		strAlias.Format(_T("%d"),i);
		strData = m_pDM_RESULT->GetCellData(strAlias,nResultRowIdx);
		MakeOutputFormat(strData);
		m_pDM_PRINT->SetCellData(strAlias,strData,nPrintRowIdx);
	}	
	

	//�Ѱ� 
	if(bDupDel)
	{
		strData = m_pDM_RESULT->GetCellData(_T("�հ�"),nResultRowIdx);
	}
	else
	{
		strData = m_pDM_RESULT->GetCellData(_T("�ߺ��հ�"),nResultRowIdx);
	}
	
	MakeOutputFormat(strData);
	m_pDM_PRINT->SetCellData(_T("�հ�"),strData,nPrintRowIdx);
	m_pDM_PRINT->SetCellData(_T("������"),_T("�հ�"),nPrintRowIdx);

	return 0;
}

VOID CLOC_STATISTICS::PrintData()
{
	INT nRowCnt = m_pDM_PRINT->GetRowCount();
	
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("����� �ڷᰡ �����ϴ�!"));
		return;
	}

	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM_PRINT->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("���ӿ��������"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return ;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	pSIReportManager->SetDataMgr(0, m_pDM_PRINT, 0);
	
	
	CString strTitle;
	if(1 == m_nTYPE)
	{
		strTitle.Format(_T("�����۾������(%s)"),m_strPrintClassName);
	}
	else if(2 == m_nTYPE)
	{
		strTitle.Format(_T("���Ӽ��������(%s)"),m_strPrintClassName);
	}	
	else if(3 == m_nTYPE)
	{
		strTitle.Format(_T("���ӽǽð��������(%s)"),m_strPrintClassName);
	}	
	
	pSIReportManager->SetTempVariable(_T("����"),strTitle);
	

	CString strAlias,strData;
	for(INT i=1;i<13;i++)
	{
		strAlias.Format(_T("%d"),i);
		strData = m_pDM_KDC->GetCellData(strAlias,0);
		pSIReportManager->SetTempVariable(strAlias,strData);
	}
	
	pSIReportManager->SetTempVariable(_T("����"),_T("*ǥ�ù��\n\n����\n[�Ǽ�]\n(å��)"));


	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}


}
HBRUSH CLOC_STATISTICS::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2018.07.11 ADD BY KHY: ����� ����Ʈ ����
/*//NEW------------------------------------------------------------------------*/	

BEGIN_EVENTSINK_MAP(CLOC_STATISTICS, CESL_Mgr)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_PROC_0400_08)
	ON_EVENT(CLOC_STATISTICS, IDC_gridMAIN, -600 /* Click */, OnClickMshflexgrid, VTS_NONE)	
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CLOC_STATISTICS::OnClickMshflexgrid() 
{
EFS_BEGIN
	//��ȸ�� ��������� ������ ����� ����Ʈ�� ���� �ʿ䰡 ����
	if(m_pDM_RESULT->GetRowCount() < 1)
	{
		return ;
	}

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	CString sXValue, sYValue;

	// YColumn �ڵ尪 Ȯ��
	sXValue = pGrid->GetTextMatrix( 0 , nCol );	
	sYValue = pGrid->GetTextMatrix( nRow , 0 );	

	INT ids = m_pApi->RunStatisticsListProc(m_strClassName, sXValue, sYValue);
	if(ids >= 0)
	{
		if(m_pApi->m_pDM_LIST != NULL && m_pApi->m_pDM_LIST->GetRowCount() > 0)
		{
			CLocStatisticsList dlg(this, m_pApi->m_pDM_LIST);
			dlg.DoModal();
		}
		else
		{
			MessageBox(_T("��� ��� ����Ʈ�� ���� ���� �ʽ��ϴ�."));
		}
	}
EFS_END
}
/*//END------------------------------------------------------------------------*/
// KOL.RED.2018.211 ---------------------------------------------------------------------------