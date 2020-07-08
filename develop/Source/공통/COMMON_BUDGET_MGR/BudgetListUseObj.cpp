// BudgetListUseObj.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetListUseObj.h"
#include "BudgetApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CBudgetListUseObj dialog


CBudgetListUseObj::CBudgetListUseObj(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetListUseObj)
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
CBudgetListUseObj::~CBudgetListUseObj()
{
}

BOOL CBudgetListUseObj::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBudgetListUseObj::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetListUseObj)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBudgetListUseObj, CDialog)
	//{{AFX_MSG_MAP(CBudgetListUseObj)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_MAT_MODIFY, OnbtnBUDGETLISTMATMODIFY)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_MAT_DELETE, OnbtnBUDGETLISTMATDELETE)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_MAT_INSERT, OnbtnBUDGETLISTMATINSERT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetListUseObj message handlers

BOOL CBudgetListUseObj::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("�������_������_�̿��󱸺�") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : �������_������_�̿��󱸺�") );
		return false;
	}
	
	
	m_pCM = FindCM(_T("CM_������_�̿��󱸺�"));
	m_pDM = FindDM(_T("DM_����������_�̿��󱸺�"));
	m_pDM_EXE_SUB = FindDM(_T("DM_�������ຸ��"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���"));

	if(!m_pCM || !m_pDM || !m_pGrid || !m_pDM_EXE_SUB) {
		AfxMessageBox( _T("CM,DM,Grid Error : CM_������_�̿��󱸺�,DM_����������_�̿��󱸺�,�׸���,DM_�������ຸ��") );
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

INT CBudgetListUseObj::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
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

VOID CBudgetListUseObj::OnbtnBUDGETLISTMATINSERT() 
{

	//�߰� �Լ� 
	//DM�� UDF_���¸� _T("I")�� �ؼ� �߰��Ѵ� 
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
	m_pCM->GetControlMgrData(_T("�̿��󱸺�"),strCode,-1);
	ids = api.CheckDupCode(m_pDM, strCode);
	if(ids < 0 )
		return;
	
	m_pDM->InsertRow(-1);
	INT row = m_pDM->GetRowCount()-1;
	SetAllData(row, _T("I")); //insert	
}

VOID CBudgetListUseObj::OnbtnBUDGETLISTMATMODIFY() 
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

VOID CBudgetListUseObj::OnbtnBUDGETLISTMATDELETE() 
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
	strTmp = m_pDM->GetCellData(_T("UDF_�ڷᱸ��"),m_nCurrentIdx);

	CString msg;
	msg.Format(_T("�ڷᱸ���� [%s]�� �����Ҵ��� �����ϰڽ��ϱ�?"), strTmp);
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

VOID CBudgetListUseObj::SetAllData(INT row, CString strUDF)
{
	const INT cnt = 2;
	CString alias[cnt];
	CString data[cnt];

	alias[0] = _T("�̿��󱸺�");
	alias[1] = _T("�����Ҵ��");


	for(INT i=0;i<cnt;i++)
	{
		m_pCM->GetControlMgrData(alias[i],data[i]);
	}

	m_pDM->SetCellData(_T("�ڵ�"),data[0],row);
	m_pDM->SetCellData(_T("UDF_�̿��󱸺�"),data[0],row);
	m_pDM->SetCellData(_T("�����Ҵ��"),data[1],row);
	m_pDM->SetCellData(_T("UDF_����"),strUDF,row);
	m_pDM->SetCellData(_T("��������"),_T("O"),row);

	m_nCurrentIdx =	row;
	ShowAllData();
}

INT CBudgetListUseObj::RefreshAll()
{
	if(m_strBUDGET_COMPILATION_KEY.IsEmpty())
	{
		AfxMessageBox(_T("������KEY�� ã�� �� �����ϴ�!"));
		return -1;
	}


	CString strWhere;
	strWhere.Format(_T("BUDGET_COMPILATION_KEY = %s AND CLASS_KIND = 'USE' AND SUB_TYPE = 'O'"),m_strBUDGET_COMPILATION_KEY);
	m_pDM->RefreshDataManager(strWhere);

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("�̿��󱸺�"),_T("UDF_�̿��󱸺�"));

	if(m_pDM->GetRowCount()>0) m_nCurrentIdx = 0; 


	//�������� �������� ���� 
	CBudgetApi api(this);
	api.ApplySubExecutionInfo(m_pDM,m_pDM_EXE_SUB,_T("O"));

	ShowAllData();

	return 0;

}

INT CBudgetListUseObj::InitOpenMode()
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

INT CBudgetListUseObj::ShowAllData()
{
	CString sTemp;
	INT ii = 0;
	INT iTotal = 0;

	if(m_nCurrentIdx<0) return 0;
	
	for ( INT nIdx = 0 ; nIdx < m_pDM->GetRowCount() ; nIdx++ )
	{
		m_pDM->SetCellData( _T("UDF_�뵵����"), m_pDM->GetCellData( _T("�ڵ�"), nIdx ) , nIdx );
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
		SetDlgItemText(IDC_edtBUDGET_LIST_USE_OBJ_ACCOUNT2,m_sBudgetTotal);
	}
	else
		SetDlgItemText(IDC_edtBUDGET_LIST_USE_OBJ_ACCOUNT2,_T(""));
	m_pDM->SetCellData( _T("UDF_�뵵����"), m_pDM->GetCellData( _T("�ڵ�"), m_nCurrentIdx ) , m_nCurrentIdx );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("�̿��󱸺�"),_T("UDF_�̿��󱸺�"));
	

	m_pCM->AllControlDisplay();
	m_pGrid->Display();
	m_pGrid->SetReverse(m_nCurrentIdx);

	return 0;
}



BEGIN_EVENTSINK_MAP(CBudgetListUseObj, CDialog)
    //{{AFX_EVENTSINK_MAP(CBudgetListUseObj)
	ON_EVENT(CBudgetListUseObj, IDC_gridBUDGET_LIST_USEOBJ, -600 /* Click */, OnClickgridBUDGETLISTUSEOBJ, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBudgetListUseObj::OnClickgridBUDGETLISTUSEOBJ() 
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

HBRUSH CBudgetListUseObj::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
