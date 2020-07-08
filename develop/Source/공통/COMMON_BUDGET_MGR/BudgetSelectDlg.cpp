// BudgetSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetSelectDlg dialog


CBudgetSelectDlg::CBudgetSelectDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDM = NULL;

	m_nCurrentIdx = -1;
	m_strBUDGET_COMPILATION_KEY = _T("");
}

CBudgetSelectDlg::~CBudgetSelectDlg()
{
}

VOID CBudgetSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBudgetSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CBudgetSelectDlg)
	ON_BN_CLICKED(IDC_btnBUDGET_SELECT_SEARCH, OnbtnBUDGETSELECTSEARCH)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetSelectDlg message handlers

BOOL CBudgetSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(InitESL_Mgr( _T("�������_������_����") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : �������_������_����") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_������_����"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���"));
	m_pDM = FindDM(_T("DM_������_����"));
	
	if(!m_pDM || !m_pCM ||!m_pGrid) {
		AfxMessageBox( _T("CM,DM,Grid Error : CM_������_����,DM_������_����,�׸���") );
		return FALSE;
	}

	m_pCM->SetControlMgrData( _T("����������"), _T("1"));
	OnbtnBUDGETSELECTSEARCH();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CBudgetSelectDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBudgetSelectDlg)
	ON_EVENT(CBudgetSelectDlg, IDC_gridBUDGET_SELECT, -600 /* Click */, OnClickgridBUDGETSELECT, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBudgetSelectDlg::OnClickgridBUDGETSELECT() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			m_nCurrentIdx = row-1;
		}
	
	}
}

VOID CBudgetSelectDlg::OnbtnBUDGETSELECTSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	CString strYear,strClass, strStatus, strSubClass;
	m_pCM->GetControlMgrData(_T("�������⵵"),strYear);
	m_pCM->GetControlMgrData(_T("����������"),strClass);
	m_pCM->GetControlMgrData(_T("����������"),strStatus);
	m_pCM->GetControlMgrData(_T("��������������"),strSubClass);


	CString strQuery;
	CString tmpquery;

	BOOL bNeedAnd= FALSE;
	if(!strYear.IsEmpty())
	{
		tmpquery.Format(_T("BUDGET_YEAR = '%s'"),strYear);
		strQuery += tmpquery;
		bNeedAnd= TRUE;
	}

	if(!strClass.IsEmpty())
	{
		if(bNeedAnd) strQuery += _T(" AND ");
		tmpquery.Format(_T("BUDGET_CLASS = '%s'"),strClass);
		strQuery += tmpquery;
	}

	if(!strStatus.IsEmpty())
	{
		if(bNeedAnd) strQuery += _T(" AND ");
		tmpquery.Format(_T("BUDGET_STATUS = '%s'"),strStatus);
		strQuery += tmpquery;
	}

	if(!strSubClass.IsEmpty())
	{
		if(bNeedAnd) strQuery += _T(" AND ");
		tmpquery.Format(_T("BUDGET_SUB_CLASS = '%s'"),strSubClass);
		strQuery += tmpquery;
	}

	m_pDM->RefreshDataManager(strQuery);

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("��������������"),_T("UDF_��������������"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("����������"),_T("UDF_����������"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("����������"),_T("UDF_����������"));
	
	m_nCurrentIdx = -1;
	m_pGrid->Display();
}

VOID CBudgetSelectDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	// ���� �Լ�
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return;
	}

	m_strBUDGET_COMPILATION_KEY = m_pDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);

	CDialog::OnOK();
}

HBRUSH CBudgetSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
