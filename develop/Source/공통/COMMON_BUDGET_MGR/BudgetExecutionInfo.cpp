// BudgetExecutionInfo.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetExecutionInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetExecutionInfo dialog


CBudgetExecutionInfo::CBudgetExecutionInfo(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetExecutionInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pCM = NULL;
	m_pParentDM = NULL;	
	m_pParentGrid = NULL;
	m_nCurrentIdx = -1;

	m_nOpenMode = 1; //default
}

CBudgetExecutionInfo::~CBudgetExecutionInfo()
{
}

VOID CBudgetExecutionInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetExecutionInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBudgetExecutionInfo, CDialog)
	//{{AFX_MSG_MAP(CBudgetExecutionInfo)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetExecutionInfo message handlers

BOOL CBudgetExecutionInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("�������_������_��������_��������") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : �������_������_��������_��������") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_��������_��������"));

	if(!m_pCM || !m_pParentDM || !m_pParentGrid) {
		AfxMessageBox( _T("CM,DM,Grid Error : CM_��������_��������,DM_��������,�׸���") );
		return FALSE;
	}

	if(m_nOpenMode<0)
	{
		
		if(m_nCurrentIdx<0)
		{
			AfxMessageBox( _T("���õ� �ڷᰡ �����ϴ�!") );
			EndDialog(0);
			return FALSE;

		}
		
		m_pCM->AllControlDisplay(m_nCurrentIdx);
	
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBudgetExecutionInfo::OnOK() 
{
	// TODO: Add extra validation here
	
	CString strBudget;
	//strBudget = m_pCM->GetControlMgrData(_T("���࿹��"),strBudget);
	strBudget.Format(_T("%d"),m_pCM->GetControlMgrData(_T("���࿹��"),strBudget));

	if(strBudget.IsEmpty())
	{
		AfxMessageBox( _T("���࿹���� �Է��� �ֽʽÿ�!") );
		return;
	}

	CString strDate;
	//strDate = m_pCM->GetControlMgrData(_T("������"),strDate);
	strDate.Format(_T("%d"), m_pCM->GetControlMgrData(_T("������"),strDate));

	if(strDate.IsEmpty())
	{
		AfxMessageBox( _T("�������� �Է��� �ֽʽÿ�!") );
		return;
	}
	

	if(m_nOpenMode>0)
	{
		m_pParentDM->InsertRow(-1);
		m_nCurrentIdx = m_pParentDM->GetRowCount()-1;
	}


	const INT cnt = 6;
	CString alias[cnt];
	CString data[cnt];

	alias[0] = _T("���࿹��");
	alias[1] = _T("�������");
	alias[2] = _T("������");
	alias[3] = _T("����Ÿ��");
	alias[4] = _T("�����۾���");
	alias[5] = _T("���");	

	for(INT i=0;i<cnt;i++)
	{
		m_pCM->GetControlMgrData(alias[i],data[i]);
	}

	m_pParentDM->SetCellData(_T("���࿹��"),data[0],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("�������"),data[1],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("UDF_�������"),data[1],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("������"),data[2],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("����Ÿ��"),data[3],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("UDF_����Ÿ��"),data[3],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("�����۾���"),data[4],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("���"),data[5],m_nCurrentIdx);

	if(m_nOpenMode>0)
	{
		m_pParentDM->SetCellData(_T("UDF_����"),_T("I"),m_nCurrentIdx);
	}
	else
	{
		CString strUDF = m_pParentDM->GetCellData(_T("UDF_����"),m_nCurrentIdx);
		if(_T("I") != strUDF) m_pParentDM->SetCellData(_T("UDF_����"),_T("U"),m_nCurrentIdx);
	}


	CDialog::OnOK();
}

HBRUSH CBudgetExecutionInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
