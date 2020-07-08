// BudgetListDept.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetListDept.h"
#include "BudgetApi.h"
#include "BudgetDeptPrint.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetListDept dialog


CBudgetListDept::CBudgetListDept(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetListDept)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nOpenMode = 1; //default
	m_pCM = NULL;
	m_pDM = NULL;
	m_pDM_EXE_SUB = NULL;
	m_pGrid = NULL;  
	m_strBUDGET_COMPILATION_KEY = _T("");
	m_nCurrentIdx = -1;
	m_pArrayDeleteKey = NULL;
	m_sBudgetTotal = _T("");
}
CBudgetListDept::~CBudgetListDept()
{
}

BOOL CBudgetListDept::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CBudgetListDept::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetListDept)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBudgetListDept, CDialog)
	//{{AFX_MSG_MAP(CBudgetListDept)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_COURCE_INSERT, OnbtnBUDGETLISTCOURCEINSERT)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_COURCE_MODIFY, OnbtnBUDGETLISTCOURCEMODIFY)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_COURCE_DELETE, OnbtnBUDGETLISTCOURCEDELETE)
	ON_BN_CLICKED(IDC_PRINT, OnPrint)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetListDept message handlers

BOOL CBudgetListDept::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr( _T("�������_������_�кκ�") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : �������_������_�кκ�") );
		return false;
	}
	
	
	m_pCM = FindCM(_T("CM_������_�кκ�"));
	m_pDM = FindDM(_T("DM_����������_�кκ�"));
	m_pDM_EXE_SUB = FindDM(_T("DM_�������ຸ��"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���"));

	if(!m_pCM || !m_pDM || !m_pGrid || !m_pDM_EXE_SUB) {
		AfxMessageBox( _T("CM,DM,Grid Error : CM_������_�кκ�,DM_����������_�кκ�,�׸���") );
		return FALSE;
	}
	
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	if(InitOpenMode()<0)
	{
		AfxMessageBox(_T("InitOpenMode FAIL!"));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBudgetListDept::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData1, strTmpData2;
	
	if(strDMFieldAlias == _T("�����Ҵ��"))
	{	
		strTmpData1 = m_pDM->GetCellData(_T("UDF_�����Ҵ��"),nRow);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData1, strTmpData2);
		m_pDM->SetCellData(_T("�����Ҵ��"), strTmpData2, nRow);
		
	}
	else if(strDMFieldAlias == _T("�������"))
	{
		strTmpData1 = m_pDM->GetCellData(_T("UDF_�������"),nRow);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData1, strTmpData2);
		m_pDM->SetCellData(_T("�������"), strTmpData2, nRow);		
	}
	else if(strDMFieldAlias == _T("UDF_�����Ҵ��"))
	{	
		strTmpData1 = m_pDM->GetCellData(_T("�����Ҵ��"),nRow);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData1, strTmpData2);
		m_pDM->SetCellData(_T("UDF_�����Ҵ��"), strTmpData2, nRow);
	}

	return 0;
}

INT CBudgetListDept::InitOpenMode()
{

	
	if(m_nOpenMode<0)
	{
	
		if(!m_pArrayDeleteKey)
		{
			AfxMessageBox(_T("����KEY ARRAY�� ã�� �� �����ϴ�!"));
			return -1;

		}

	}
	

	return 0;
}

INT CBudgetListDept::ShowAllData()
{
	CString sTemp;
	INT ii = 0;
	INT iTotal = 0;

	if(m_nCurrentIdx<0) return 0;
	
	for ( INT nIdx = 0 ; nIdx < m_pDM->GetRowCount() ; nIdx++ )
	{
		m_pDM->SetCellData( _T("UDF_�к�"), m_pDM->GetCellData( _T("�ڵ�"), nIdx ) , nIdx );
		sTemp = m_pDM->GetCellData( _T("�����Ҵ��"), nIdx);
		if(!sTemp.IsEmpty())
		{	
			ii = _ttoi(sTemp);
			iTotal += ii;
		}
	}
	if(iTotal != 0)
	{
		m_sBudgetTotal.Format(_T("%d"),iTotal);
		for( INT i=m_sBudgetTotal.GetLength()-1,j=0 ; i>=0 ; i--,j++ )
		{
			if( j != 0 && (j%3) == 0 ) 
			{ 
				m_sBudgetTotal.Insert( i+1,_T(",") ); i--;j++;
			}
		}
		SetDlgItemText(IDC_edtBUDGET_LIST_DEPT_ACCOUNT2,m_sBudgetTotal);
	}
	else
		SetDlgItemText(IDC_edtBUDGET_LIST_DEPT_ACCOUNT2,_T(""));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("�к�"),_T("UDF_�к�"));
	

	m_pCM->AllControlDisplay();
	m_pGrid->Display();
	m_pGrid->SetReverse(m_nCurrentIdx);

	return 0;
}

VOID CBudgetListDept::SetAllData(INT row, CString strUDF)
{
	const INT cnt = 2;
	CString alias[cnt];
	CString data[cnt];

	alias[0] = _T("�кκ�");
	alias[1] = _T("�����Ҵ��");


	for(INT i=0;i<cnt;i++)
	{
		m_pCM->GetControlMgrData(alias[i],data[i]);
	}

	m_pDM->SetCellData(_T("�ڵ�"),data[0],row);
	m_pDM->SetCellData(_T("UDF_�к�"),data[0],row);
	m_pDM->SetCellData(_T("�����Ҵ��"),data[1],row);
	m_pDM->SetCellData(_T("UDF_����"),strUDF,row);
	m_pDM->SetCellData(_T("��������"),_T("D"),row);

	m_nCurrentIdx =	row;
	ShowAllData();
}

VOID CBudgetListDept::OnbtnBUDGETLISTCOURCEINSERT() 
{
	// TODO: Add your control notification handler code here
	INT ids = -1;
	CBudgetApi api(this);
	CString strAcount, strCode;

	m_pCM->GetControlMgrData(_T("�����Ҵ��"),strAcount);
	if( m_nOpenMode != 1 )
	{
		ids = api.CheckCompilationAcount(1,m_pDM,strAcount,m_strBUDGET_COMPILATION_KEY,-1);
		if(ids < 0 )
			return;
	}
	m_pCM->GetControlMgrData(_T("�кκ�"),strCode,-1);
	ids = api.CheckDupCode(m_pDM, strCode);
	if(ids < 0 )
		return;

	m_pDM->InsertRow(-1);
	INT row = m_pDM->GetRowCount()-1;
	SetAllData(row, _T("I")); //insert
}

VOID CBudgetListDept::OnbtnBUDGETLISTCOURCEMODIFY() 
{
	// TODO: Add your control notification handler code here
	// 2005-02-17 �˻��� �ڷᰡ ������ �����Ѵ�.
	INT nCnt = m_pDM->GetRowCount();
	if(nCnt < 1)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return ;
	}
	
	CBudgetApi api(this);
	CString strAcount;
	m_pCM->GetControlMgrData(_T("�����Ҵ��"),strAcount);
	if( m_nOpenMode != 1 )
	{
		INT ids = api.CheckCompilationAcount(0,m_pDM,strAcount,m_strBUDGET_COMPILATION_KEY, m_nCurrentIdx);
		if(ids < 0 )
			return;
	}
	
	CString strUDF;
	strUDF = m_pDM->GetCellData(_T("UDF_����"),m_nCurrentIdx);

	if(_T("I") == strUDF)
	{
		SetAllData(m_nCurrentIdx, _T("I")); //insert
	}
	else 
	{
		SetAllData(m_nCurrentIdx, _T("U")); //update
	}
}

VOID CBudgetListDept::OnbtnBUDGETLISTCOURCEDELETE() 
{
	// TODO: Add your control notification handler code here
	// 2005-02-17 �˻��� �ڷᰡ ������ �����Ѵ�.
	INT nCnt = m_pDM->GetRowCount();
	if(nCnt < 1)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return ;
	}

	CString strTmp;
	strTmp = m_pDM->GetCellData(_T("UDF_�к�"),m_nCurrentIdx);

	CString msg;
	msg.Format(_T(" [%s] ��ó��  �����Ҵ��� �����ϰڽ��ϱ�?"), strTmp);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;


	CString strUDF;
	strUDF = m_pDM->GetCellData(_T("UDF_����"),m_nCurrentIdx);

	if(_T("I") != strUDF) 
	{
		CString strREC_KEY;
		strREC_KEY = m_pDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
		m_pArrayDeleteKey->Add(strREC_KEY); //update
	}
	
	m_pDM->DeleteRow(m_nCurrentIdx);

	// 2005-02-17 �� ������ �����͸� ������ ��쿡�� �� �����Ͱ� ���������Ƿ� �� ������ Set�Ѵ�.
	if( (m_nCurrentIdx != 0) && (nCnt == (m_nCurrentIdx+1)) )
		m_nCurrentIdx--;

	ShowAllData();

}

INT CBudgetListDept::RefreshAll()
{
	if(m_strBUDGET_COMPILATION_KEY.IsEmpty())
	{
		AfxMessageBox(_T("������KEY�� ã�� �� �����ϴ�!"));
		return -1;
	}

	CString strWhere;
	strWhere.Format(_T("BUDGET_COMPILATION_KEY = %s AND CLASS_KIND = 'DEP' AND SUB_TYPE = 'D'"),m_strBUDGET_COMPILATION_KEY);
	m_pDM->RefreshDataManager(strWhere);

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("�к�"),_T("UDF_�к�"));

	if(m_pDM->GetRowCount()>0) m_nCurrentIdx = 0; 

	//�������� �������� ���� 
	CBudgetApi api(this);
	api.ApplySubExecutionInfo(m_pDM,m_pDM_EXE_SUB,_T("D"));

	ShowAllData();

	return 0;

}

BEGIN_EVENTSINK_MAP(CBudgetListDept, CDialog)
    //{{AFX_EVENTSINK_MAP(CBudgetListDept)
	ON_EVENT(CBudgetListDept, IDC_gridBUDGET_LIST_DEPT, -600 /* Click */, OnClickgridBUDGETLISTDEPT, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBudgetListDept::OnClickgridBUDGETLISTDEPT() 
{
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			m_nCurrentIdx = row-1;
			ShowAllData();
		}
	
	}
}

VOID CBudgetListDept::OnPrint() 
{
	// TODO: Add your control notification handler code here
	CBudgetDeptPrint dlg(this);
	
	dlg.SetParentInfo(m_pDM);
	dlg.DoModal();
}

HBRUSH CBudgetListDept::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
