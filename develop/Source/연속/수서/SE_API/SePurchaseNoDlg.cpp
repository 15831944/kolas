// SePurchaseNoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseNoDlg.h"
#include "SePurchaseNoInsertDlg.h"
#include "..\..\..\����\BO_ACQ_GROUP_MANAGER\GroupCollection.h"
#include "..\..\..\����\BO_ACQ_GROUP_MANAGER\BO_BudgetMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseNoDlg dialog


CSePurchaseNoDlg::CSePurchaseNoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePurchaseNoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseNoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_strYear = _T("");
	m_strPurchaseNo = _T("");
	m_strWorking_status = _T("");
	m_nCurrentIdx = -1;
	m_nOpenMode = 1; //default
}
CSePurchaseNoDlg::~CSePurchaseNoDlg()
{
}


VOID CSePurchaseNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseNoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseNoDlg, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseNoDlg)
	ON_BN_CLICKED(IDC_btnPUR_NO_MGR_SEARCH, OnbtnPURNOMGRSEARCH)
	ON_BN_CLICKED(IDC_btnPUR_NO_MGR_INSERT, OnbtnPURNOMGRINSERT)
	ON_BN_CLICKED(IDC_btnPUR_NO_MGR_DELETE, OnbtnPURNOMGRDELETE)
	ON_BN_CLICKED(IDC_btnBUDGET_EXECUTION, OnbtnBUDGETEXECUTION)
	ON_BN_CLICKED(IDC_btnBUDGET_CANCEL, OnbtnBUDGETCANCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseNoDlg message handlers

BOOL CSePurchaseNoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("����_����_������ȣ"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_������ȣ") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_����_������ȣ"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���"));
	m_pDM = FindDM(_T("DM_����_������ȣ"));
	

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : ����_����_������ȣ") );
		return FALSE;
	}	
	
	//�ڷ���¸� ���� ���ش�.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_NO_MGR_STATUS);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("���Դ��") );
		pCombo->InsertString( 1, _T("�ֹ���") );
		pCombo->InsertString( 2, _T("��ü") );
		pCombo->SetCurSel( 0 );
	}

	//�ʱ� ������ ���� �ش�
	RefreshAll();
	


	//������ �Է°� �˻� �ܿ��� _T("����")��ư�� ������ �ʰ� �Ѵ�
	//if(-1 == m_nOpenMode) GetDlgItem(IDOK)->ShowWindow(FALSE);
	//if(m_nOpenMode<0) GetDlgItem(IDOK)->ShowWindow(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSePurchaseNoDlg::OnbtnPURNOMGRSEARCH() 
{
	// TODO: Add your control notification handler code here
	RefreshAll();

}

//=================================================================
// [�Լ�����]
//	����[�ڷ���� 0] ���� ���������� �����ش�.
// [MINI SPEC]
//  1. ��Ʈ�ѷ� ���� �����⵵�� ������ ���� ������ �����Ѵ�.
//  2. ��Ʈ�ѷ� ���� �۾��� ������ ���� ������ �����Ѵ�.
//  3. DM�� �����Ͽ� ���� ���������� �����ش�. 
//  4. ������� ��ư ��뿩�ο� ���� ��ư�� �׸��带 �����.
//=================================================================
VOID CSePurchaseNoDlg::RefreshAll()
{
	//-------------------------------------------------------------
	// 1. ��Ʈ�ѷ� ���� �����⵵�� ������ ���� ������ �����Ѵ�.
	//-------------------------------------------------------------
	CString strYEAR;
	m_pCM->GetControlMgrData(_T("�����⵵"),strYEAR);
	
	if(strYEAR.IsEmpty())
	{
		AfxMessageBox(_T("�����⵵�� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}	
	
	CString strQuery = _T("");
	CString strtmpquery = _T("");
	strtmpquery.Format(_T("N.ACQ_YEAR = '%s'"),strYEAR);
	strQuery += strtmpquery;

	//-------------------------------------------------------------
	// 2. ��Ʈ�ѷ� ���� �۾��� ������ ���� ������ �����Ѵ�.
	//-------------------------------------------------------------
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_NO_MGR_STATUS);
	INT nCurSel = 0; 
	if(pCombo) nCurSel = pCombo->GetCurSel();
	
	if(nCurSel != 2) //��ü 
	{
		strQuery += _T(" AND ");
		strtmpquery.Format(_T("N.WORKING_STATUS = '%d'"),nCurSel);
		strQuery += strtmpquery;
	}

	//-------------------------------------------------------------
	// 3. DM�� �����Ͽ� ���� ���������� �����ش�. 
	//-------------------------------------------------------------
	m_pDM->RefreshDataManager(strQuery);
	
	CString strWORKING_STATUS;
	
	for(INT i=0;i<m_pDM->GetRowCount();i++)
	{
		strWORKING_STATUS = _T("");
		strWORKING_STATUS = m_pDM->GetCellData(_T("�۾�����"),i);
		if(_T("0") == strWORKING_STATUS) m_pDM->SetCellData(_T("UDF_�۾�����"),_T("���Դ��"),i);
		else if(_T("1") == strWORKING_STATUS)  m_pDM->SetCellData(_T("UDF_�۾�����"),_T("�ֹ���"),i);
	}

	m_pGrid->Display();

	//-------------------------------------------------------------------
	// 2005-10-11 Ȳ����: ������� ��ư�� �׸����÷��� ��뿩�ο� ���� ��ư�� �����.
	//-------------------------------------------------------------------
	CString sBudgetYN;
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'BUDGETMNG'"), sBudgetYN);
	if(sBudgetYN == _T("N"))
	{
		GetDlgItem(IDC_btnBUDGET_EXECUTION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnBUDGET_CANCEL)->ShowWindow(SW_HIDE);
		//========================================
		//20080606 UPDATE BY PJW : ��ư��ġ����
		GetDlgItem(IDC_btnPUR_NO_MGR_SEARCH)->MoveWindow(330,17,70,22);
		GetDlgItem(IDC_btnPUR_NO_MGR_INSERT)->MoveWindow(420,17,70,22);
		GetDlgItem(IDC_btnPUR_NO_MGR_DELETE)->MoveWindow(500,17,70,22);
		GetDlgItem(IDC_btnBUDGET_EXECUTION)->MoveWindow(420,47,70,22);
		GetDlgItem(IDC_btnBUDGET_CANCEL)->MoveWindow(500,47,70,22);
		//========================================


		m_pGrid->SetColWidth(1,0,600);
		m_pGrid->SetColWidth(4,0,2200);
		m_pGrid->SetColWidth(8,0,0);
	}
}

//=================================================================
// [�Լ�����]
//	�Է¹�ư���� �����Է�ȭ���� �θ���.
// [MINI SPEC]
//  1. �����⵵ �˻�
//  2. �����Է� ���̾�α׸� �����ϰ� �ش� �����⵵�� �����Ѵ�.
//=================================================================
VOID CSePurchaseNoDlg::OnbtnPURNOMGRINSERT() 
{
	//-------------------------------------------------------------
	// 1. �����⵵ �˻�
	//-------------------------------------------------------------
	CString strYEAR;
	m_pCM->GetControlMgrData(_T("�����⵵"),strYEAR);
	
	if(strYEAR.IsEmpty())
	{
		AfxMessageBox(_T("�����⵵�� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}	
	
	//-------------------------------------------------------------
	// 2. �����Է� ���̾�α׸� �����ϰ� �ش� �����⵵�� �����Ѵ�.
	//-------------------------------------------------------------
	CSePurchaseNoInsertDlg dlg(this);
	dlg.m_strYEAR = strYEAR;
	
	if(dlg.DoModal() != IDOK) return;

	RefreshAll();

}

//=================================================================
// [�Լ�����]
//	���õ� ������ �����Ѵ�.
// [MINI SPEC]
//  1. ������ �����ߴ��� �˻�
//  2. �۾����°� ���Դ���� �������� �˻��Ѵ�.
//  3. ���õ� ������ �����⵵/������ȣ/�۾����������� ����� ����
//=================================================================
VOID CSePurchaseNoDlg::OnOK() 
{
	// TODO: Add extra validation here

	//-------------------------------------------------------------
	// 1. ������ �����ߴ��� �˻�
	//-------------------------------------------------------------
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return;
	}


	//-------------------------------------------------------------
	// 2. �۾����°� ���Դ���� �������� �˻��Ѵ�.
	//-------------------------------------------------------------
	CString strWORKING_STATUS = _T("");
	strWORKING_STATUS = m_pDM->GetCellData(_T("�۾�����"),m_nCurrentIdx);

	
	if(m_nOpenMode>0)
	{
		if(strWORKING_STATUS != _T("0"))
		{
			AfxMessageBox(_T("�̹� �ֹ��� ������ ������ ������ �� �����ϴ�!"));
			return;
		}
	}

	//-------------------------------------------------------------
	// 3. ���õ� ������ �����⵵/������ȣ/�۾����������� ����� ����
	//-------------------------------------------------------------
	m_strYear = m_pDM->GetCellData(_T("�����⵵"),m_nCurrentIdx);
	m_strPurchaseNo = m_pDM->GetCellData(_T("������ȣ"),m_nCurrentIdx);
	m_strWorking_status = m_pDM->GetCellData(_T("�۾�����"),m_nCurrentIdx);

	if(m_strYear.IsEmpty() || m_strPurchaseNo.IsEmpty())
	{
		AfxMessageBox(_T("���� ������ ã�� �� �����ϴ�!"));
		m_strYear = _T("");
		m_strPurchaseNo = _T("");
		return;
	}


	CDialog::OnOK();
}

BEGIN_EVENTSINK_MAP(CSePurchaseNoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSePurchaseNoDlg)
	ON_EVENT(CSePurchaseNoDlg, IDC_gridPUR_NO_MGR, -600 /* Click */, OnClickgridPURNOMGR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSePurchaseNoDlg::OnClickgridPURNOMGR() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pDM->GetRowCount()) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_nCurrentIdx = row-1;
	}
}

//=================================================================
// [�Լ�����]
//  ������ �����Ѵ�.
// [MINI SPEC]
//  1. ������ ���������� �����Ѵ�.
//  2. ������� ��뿩�θ� ���Ѵ�.
//  3. ������ �������� ������ �Ʒ��� ���� �Ѵ�. (�׷��� DM row�� ��Ī�� �ȴ�.)
//  4. ���������� �Ǿ��ִ� ������� ���� ��Ұ� ����Ǿ�� �Ѵ�.
//  5. ������ ���� ������ �������� �ʴ´�.
//=================================================================
VOID CSePurchaseNoDlg::OnbtnPURNOMGRDELETE() 
{
	
	/*// TODO: Add your control notification handler code here
	if(0 == m_pDM->GetRowCount() || m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return;
	}
	
	CString strSpeciesNo;
	INT nSpeciesNo = 0;
	strSpeciesNo = m_pDM->GetCellData(_T("UDF_����"),m_nCurrentIdx);
	if(!strSpeciesNo.IsEmpty()) nSpeciesNo = _ttoi(strSpeciesNo.GetBuffer(0));

	if(nSpeciesNo != 0)
	{
		AfxMessageBox(_T("�ش� ������ �����ϴ� ���� �־� ���� �� �� �����ϴ�!"));
		return;
	}
	
	//������ȣ
	CString strPurchaseNo;
	strPurchaseNo = m_pDM->GetCellData(_T("������ȣ"),m_nCurrentIdx);
	

	//�����⵵
	CString strYear;
	strYear = m_pDM->GetCellData(_T("�����⵵"),m_nCurrentIdx);
	

	CString msg;
	msg.Format(_T("[ %s ] �⵵�� [ %s ] ������ �����Ͻðڽ��ϱ�?"),strYear,strPurchaseNo);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;


	CString strREC_KEY;
	strREC_KEY = m_pDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
	
	CString strQuery;
	m_pDM->StartFrame();
	
	strQuery.Format(_T("DELETE FROM SE_PURCHASE_SEQ_NO_TBL WHERE REC_KEY = %s;"),strREC_KEY);
	m_pDM->AddFrame(strQuery);

	strQuery.Format(_T("DELETE FROM SE_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY = %s;"),strREC_KEY);
	m_pDM->AddFrame(strQuery);
	
	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	
	if(ids<0)
	{
		AfxMessageBox(_T("������ �����Ͽ����ϴ�!"));
	}
	else
	{
		AfxMessageBox(_T("�����Ͽ����ϴ�!"));
		m_pDM->DeleteRow(m_nCurrentIdx);
		m_pGrid->Display();
	}
	*/
	//2005-10-20 Ȳ����: 1. ������ �����ؾ� ó���� �� �ֵ��� ����
	//                   2. �������� �� ������ ���� ó��..

	//-------------------------------------------------------------
	// 1. ������ ���������� �����Ѵ�.
	//-------------------------------------------------------------
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	CString strSpeciesNo, strBudgetYN, msg;
	INT nSpeciesNo = 0;
	INT nCount = 0;

	//-------------------------------------------------------------
	// 2. ������� ��뿩�θ� ���Ѵ�.
	//-------------------------------------------------------------
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'BUDGETMNG'"),strBudgetYN);

	//-------------------------------------------------------------
	// 3. ������ �������� ������ �Ʒ��� ���� �Ѵ�. (�׷��� DM row�� ��Ī�� �ȴ�.)
	//-------------------------------------------------------------
	INT i = m_pGrid->SelectGetTailPosition();
	
	INT nRows = m_pGrid->SelectGetCount();
	for(INT j = 0; j<nRows ; j++)
	{
		//������ȣ
		CString strPurchaseNo;
		strPurchaseNo = m_pDM->GetCellData(_T("������ȣ"),i);
		
		//�����⵵
		CString strYear;
		strYear = m_pDM->GetCellData(_T("�����⵵"),i);
	
		CString strData;
		//-------------------------------------------------------------
		// 4. ���������� �Ǿ��ִ� ������� ���� ��Ұ� ����Ǿ�� �Ѵ�.
		//-------------------------------------------------------------
		if(strBudgetYN == _T("Y"))
		{
			m_pDM->GetCellData(_T("UDF_�����������"),i,strData);
			if( strData == _T("����"))
			{
				msg.Format(_T("[ %s ]�⵵ [ %s ]������ �������� ��� �� ���������մϴ�."), strYear, strPurchaseNo);
				ESLAfxMessageBox(msg);
				i = m_pGrid->SelectGetPrev();
				continue;
			}
		}

		//-------------------------------------------------------------
		// 5. ������ ���� ������ �������� �ʴ´�.
		//-------------------------------------------------------------
		strSpeciesNo = m_pDM->GetCellData(_T("UDF_����"),i);

		if(!strSpeciesNo.IsEmpty()) nSpeciesNo = _ttoi(strSpeciesNo.GetBuffer(0));

		if(nSpeciesNo != 0)
		{
			msg.Format(_T("[ %s ]�⵵ [ %s ]������ �����ϴ� ���� �־� ������ �� �����ϴ�."), strYear, strPurchaseNo);
			AfxMessageBox(msg);
			i = m_pGrid->SelectGetPrev();
			continue;
		}
		
		msg.Format(_T("[ %s ] �⵵�� [ %s ] ������ �����Ͻðڽ��ϱ�?"),strYear,strPurchaseNo);
		if(AfxMessageBox(msg,MB_YESNO) != IDYES) 
		{
			i = m_pGrid->SelectGetPrev();
			continue;
		}
		
		CString strREC_KEY;
		strREC_KEY = m_pDM->GetCellData(_T("REC_KEY"),i);
		
		CString strQuery;
		m_pDM->StartFrame();
		
		strQuery.Format(_T("DELETE FROM SE_PURCHASE_SEQ_NO_TBL WHERE REC_KEY = %s;"),strREC_KEY);
		m_pDM->AddFrame(strQuery);

		strQuery.Format(_T("DELETE FROM SE_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY = %s;"),strREC_KEY);
		m_pDM->AddFrame(strQuery);
		
		ids = m_pDM->SendFrame();
		m_pDM->EndFrame();
		
		if(ids<0)
		{
			msg.Format(_T("[ %s ] �⵵�� [ %s ] ������ ���� �����߽��ϴ�."),strYear,strPurchaseNo);
			AfxMessageBox(msg);
		}
		else
		{
			m_pDM->DeleteRow(i);
			nCount++;
		}

		i = m_pGrid->SelectGetPrev();
	}
	
	if(nCount > 0)
	{
		msg.Format(_T("������ [ %d ]�� �߿� [ %d ]���� ������ �����Ͽ����ϴ�."), nRows, nCount);
		ESLAfxMessageBox(msg);
		m_pGrid->Display();
	}
}

//=================================================================
// [�Լ�����]
//  ������ ������ ���� �����Ѵ�.
// [MINI SPEC]
//  1.���� ��������´�.
//  2. ����������¸� ��� ���� ���θ� �����Ѵ�.
//=================================================================
VOID CSePurchaseNoDlg::OnbtnBUDGETEXECUTION() 
{
	//-------------------------------------------------------------
	// 1.���� ��������´�.
	//-------------------------------------------------------------
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	if ( m_pGrid->SelectGetCount() > 1 ) 
	{
		AfxMessageBox ( _T("�Ѱ��� ������ ������ �ּ���..") );
		return;
	}

	// �������� ���� �޾ƿ�
	CString strData;
	m_pCM->GetControlMgrData(_T("�����������"), strData);

	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	CGroupCollection GroupCollection(this);	GroupCollection.RemoveAll();
	GroupCollection.SetAcqType(PURCHASE);
	CString strMsg;


	INT i = m_pGrid->SelectGetHeadPosition();

//	if ( m_pGrid->SelectGetCount() > 1 ) 
//	{
//		AfxMessageBox ( _T("�Ѱ��� ������ ������ �ּ���..") );
//		return;
//	}

	while(i >= 0) 
	{
		ids = GroupCollection.AddGroupInfo(
				m_pDM->GetCellData(_T("�����⵵"), i),
				m_pDM->GetCellData(_T("������ȣ"), i));
		if(ids < 0) return ;
		ArrIdx.Add(i);
		i = m_pGrid->SelectGetNext();
	}

	if(GroupCollection.GetGroupCount() == 0) return ;

	m_pDM->StartFrame();
	CBO_BudgetMgr BudgetMgr(this);
	BudgetMgr.SetDMOut(m_pDM);
	BudgetMgr.SetGroupCollection(&GroupCollection);
	BudgetMgr.SetExecuteType(_T("1"));
	BudgetMgr.SetMode(_T("SE"));


	BUDGET_ERROR_TYPE res;
	res = BudgetMgr.MakeBudgetExecutionInfo();	
	// 2005-01-06 BUDGET_IGNORE�� ��쿡�� �������� ����̹Ƿ� �����Ѵ�.
	// if(res != BUDGET_SUCCESS && res != BUDGET_IGNORE)
	if(res != BUDGET_SUCCESS)
		return ;

	ids = m_pDM->SendFrame();
	if(ids < 0) return ;
	ids = m_pDM->EndFrame();
	if(ids < 0) return ;

	for(i=0 ; i<ArrIdx.GetSize() ; i++)
	{
		m_pDM->SetCellData(_T("������������KEY"), BudgetMgr.GetBudgetExeKey(), ArrIdx.GetAt(i));
		m_pDM->SetCellData(_T("�����������"), strData, ArrIdx.GetAt(i));
		if ( BudgetMgr.GetExecuteType() == _T("1") )
			m_pDM->SetCellData(_T("UDF_�����������"), _T("����"), ArrIdx.GetAt(i));
		else
			m_pDM->SetCellData(_T("UDF_�����������"), _T("������"), ArrIdx.GetAt(i));

		m_pGrid->DisplayLine(ArrIdx.GetAt(i++));
	}

	AfxMessageBox ( _T("�Ϸ�Ǿ����ϴ�.") );
	return ;
	
}

//=================================================================
// [�Լ�����]
//  ������ ������ �������� ����Ѵ�.
// [MINI SPEC]
//  1.���� ��������´�.
//=================================================================
VOID CSePurchaseNoDlg::OnbtnBUDGETCANCEL() 
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	CGroupCollection GroupCollection(this);	GroupCollection.RemoveAll();
	GroupCollection.SetAcqType(PURCHASE);
	CString strMsg;
	INT i = m_pGrid->SelectGetHeadPosition();
	while(i >= 0) 
	{
		if(!m_pDM->GetCellData(_T("������������KEY"), i).IsEmpty())
		{
			ids = GroupCollection.AddGroupInfo(
					m_pDM->GetCellData(_T("�����⵵"), i),
					m_pDM->GetCellData(_T("������ȣ"), i),
					m_pDM->GetCellData(_T("��������"), i));
			if(ids < 0) return ;
			ArrIdx.Add(i);
		}
		else
		{
			strMsg.Format(
				_T("[%s:%s]������ �������������� �����ϴ�."),
				m_pDM->GetCellData(_T("�����⵵"), i),
				m_pDM->GetCellData(_T("������ȣ"), i));
			m_pGrid->SetReverse(i);
			ESLAfxMessageBox(strMsg);
		}
		i = m_pGrid->SelectGetNext();
	}

	if(GroupCollection.GetGroupCount() == 0) return ;

	m_pDM->StartFrame();
	CBO_BudgetMgr BudgetMgr(this);
	BudgetMgr.SetDMOut(m_pDM);
	BudgetMgr.SetMode(_T("SE"));
	BudgetMgr.SetGroupCollection(&GroupCollection);
	BUDGET_ERROR_TYPE res = BudgetMgr.DeleteBudgetExecutionInfo();
	if(res != BUDGET_SUCCESS && res != BUDGET_IGNORE)
		return ;
	
	ids = m_pDM->SendFrame();
	if(ids < 0) return ;
	ids = m_pDM->EndFrame();
	if(ids < 0) return ;

	for(i=0 ; i<ArrIdx.GetSize() ; i++)
	{
		m_pDM->SetCellData(_T("������������KEY"), BudgetMgr.GetBudgetExeKey(), ArrIdx.GetAt(i));
		m_pDM->SetCellData(_T("�����������"), _T(""), ArrIdx.GetAt(i));
		m_pGrid->DisplayLine(ArrIdx.GetAt(i));
	}

	AfxMessageBox ( _T("���������� ���������� ��ҵǾ����ϴ�.") );
	RefreshAll();
	return ;
	
}

//=================================================================
// [�Լ�����]
//  ������ �������� ���� ������  ���ʺ��� �����Ѵ�. 
//=================================================================
INT CSePurchaseNoDlg::CheckSelectDataValid()
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���"));
	if(m_pDM->GetRowCount() == 0) return -1;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

HBRUSH CSePurchaseNoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
