// DivisionNo_Mgr.cpp : implementation file
//

#include "stdafx.h"
#include "DivisionNo_Mgr.h"
#include "DivisionNo_Divide_Dlg.h"
#include "DivisionNo_Select_Dlg.h"
#include "GroupPair.h"
#include "MergeGroup_Select_Dlg.h"
#include "Group_Delete_Dlg.h"
#include "DivisionNo_Status_Change_Dlg.h"
#include "BO_BudgetMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Mgr dialog


CDivisionNo_Mgr::CDivisionNo_Mgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDivisionNo_Mgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDivisionNo_Mgr)
	m_nMyData = -1;
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pWorkNoDM = NULL;
	m_strStatus = _T("");
}

CDivisionNo_Mgr::~CDivisionNo_Mgr()
{
}

VOID CDivisionNo_Mgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDivisionNo_Mgr)
	DDX_Radio(pDX, IDC_radDIVNO_MYDATA, m_nMyData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDivisionNo_Mgr, CDialog)
	//{{AFX_MSG_MAP(CDivisionNo_Mgr)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnDIVNO_SEARCH, OnbtnDIVNOSEARCH)
	ON_BN_CLICKED(IDC_btnDIVNO_INSERT, OnbtnDIVNOINSERT)
	ON_BN_CLICKED(IDC_btnDIVNO_UPDATE, OnbtnDIVNOUPDATE)
	ON_BN_CLICKED(IDC_btnDIVNO_DELETE, OnbtnDIVNODELETE)
	ON_BN_CLICKED(IDC_btnDIVNO_INFO, OnbtnDIVNOINFO)
	ON_BN_CLICKED(IDC_btnDIVNO_COPY, OnbtnDIVNOCOPY)
	ON_BN_CLICKED(IDC_btnDIVNO_DIVIDE, OnbtnDIVNODIVIDE)
	ON_BN_CLICKED(IDC_btnDIVNO_MERGE, OnbtnDIVNOMERGE)
	ON_CBN_SELCHANGE(IDC_cmbDIVNO_WORK_CODE, OnSelchangecmbDIVNOWORKCODE)
	ON_BN_CLICKED(IDC_btnDIVNO_STATUS_CHANGE, OnbtnDIVNOSTATUSCHANGE)
	ON_CBN_SELCHANGE(IDC_cmbDIVNO_WORK_STATUS, OnSelchangecmbDIVNOWORKSTATUS)
	ON_BN_CLICKED(IDC_btnDIVNO_DELETE_SUB, OnbtnDIVNODELETESUB)
	ON_BN_CLICKED(IDC_btnDIVNO_INSERT_SUB, OnbtnDIVNOINSERTSUB)
	ON_BN_CLICKED(IDC_btnDIVNO_UPDATE_SUB, OnbtnDIVNOUPDATESUB)
	ON_BN_CLICKED(IDC_btnBUDGET_CANCEL, OnbtnBUDGETCANCEL)
	ON_BN_CLICKED(IDC_btnBUDGET_EXECUTION, OnbtnBUDGETEXECUTION)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Mgr message handlers

VOID CDivisionNo_Mgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CDivisionNo_Mgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_������������")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_������������"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	if( m_pCM == NULL || m_pGrid == NULL )
	{
		ESLAfxMessageBox( _T("[CM_������������]�� ������ �� �����ϴ�.") );
		return FALSE;
	}

	m_pDM = FindDM(_T("DM_������������"));
	m_pWorkNoDM = FindDM(_T("DM_�۾���ȣ����"));
	if( m_pDM == NULL || m_pWorkNoDM == NULL )
	{
		ESLAfxMessageBox( _T("[DM_������������/�۾���ȣ����]�� ������ �� �����ϴ�.") );
		return FALSE;
	}
	
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'BUDGETMNG'"), m_strUseBudgetYN);
	ShowControlResource(); 
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("������������ ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_nMyData = 0;
	UpdateData(FALSE);

	CButton* pBtn = (CButton*) GetDlgItem(IDC_btnDIVNO_STATUS_CHANGE);
	pBtn->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  
}

VOID CDivisionNo_Mgr::OnbtnDIVNOSEARCH() 
{
	INT ids = MakeDM();
	if( ids < 0 )		return;

	DisplayGrid(_T("MainGrid"));	
}

VOID CDivisionNo_Mgr::OnbtnDIVNOINSERT() 
{
	// TODO: Add your control notification handler code here
	if(InsertDivNoInfo() == 0)
		DisplayGrid(_T("MainGrid"));
}

VOID CDivisionNo_Mgr::OnbtnDIVNOUPDATE() 
{
	// TODO: Add your control notification handler code here
	UpdateDivNo(SUPDATE);
}

VOID CDivisionNo_Mgr::OnbtnDIVNODELETE() 
{
	INT ids = -1 ;
	CESL_DataMgr* pDM_DELETE = NULL ;
	CESL_Grid* pGrid = NULL ;
	CGroupMgr mgr(this) ;

	ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox( _T("������ �ڷᰡ �����ϴ�.") );
		return;
	}	

	pDM_DELETE = FindDM(_T("DM_������������"));
	if( pDM_DELETE == NULL )
	{
		ESLAfxMessageBox( _T("[DM_������������]�� ������ �� �����ϴ�.") );
		return;
	}	

	mgr.InitTempData();
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("DELETE_DM"), (DWORD*)pDM_DELETE);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);

	ids = mgr.SPFExecutePath(_T("��������"));
	if( ids < 0 )
	{
		return;
	}
	else if(ids == 1000)
	{
		ESLAfxMessageBox(_T("������ ���������� �����ϴ�."));
		return;
	}
	else if(ids == 0) DisplayGrid(_T("MainGrid"));	
}

VOID CDivisionNo_Mgr::OnbtnDIVNOINFO() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CDivisionNo_DataView_Dlg dlg(this);
	dlg.SetMainCM(m_pCM);
	dlg.SetMainDM(m_pDM);
	dlg.DoModal();	
}

INT CDivisionNo_Mgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData;
	if(strDMFieldAlias == _T("UDF_�۾����¼���"))
	{	
		strTmpData = m_pDM->GetCellData(_T("�۾�����"), nRow);
		if(strTmpData == _T("0")) str = _T("���Դ��");
		else if(strTmpData == _T("1")) str = _T("�ֹ�");
		else if(strTmpData == _T("2")) str = _T("�˼��Ϸ�");
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("��������"))
	{	
		if(str.IsEmpty()) str = _T("�������");
		else if(str == _T("1")) str = _T("���缭");
		else if(str == _T("2")) str = _T("���缭");
		else if(str == _T("3")) str = _T("�񵵼�");
	}
	else if(m_strUseBudgetYN == _T("Y"))
	{
		if(strDMFieldAlias == _T("UDF_�����������"))
		{
			strTmpData = m_pDM->GetCellData(_T("�����������"), nRow);
			if(strTmpData == _T("0")) str = _T("������");
			else if(strTmpData == _T("1")) str = _T("����");
			else str = _T("������");
			m_pDM->SetCellData(strDMFieldAlias, str, nRow);
		}
	}
	return 0;
}

VOID CDivisionNo_Mgr::DisplayGrid(CString gridAlias)
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(gridAlias);
	if(!pGrid) return;
	pGrid->Display();
}

INT CDivisionNo_Mgr::MakeDM() 
{
	CString strCreator, strAcqYear, strWorkCode, strWorkStatus;
	UpdateData(TRUE);

	CButton *pButton1 = (CButton*)GetDlgItem(IDC_btnDIVNO_MERGE);
	CButton *pButton2 = (CButton*)GetDlgItem(IDC_btnDIVNO_COPY);
	CButton *pButton3 = (CButton*)GetDlgItem(IDC_btnDIVNO_DIVIDE);
	CButton *pButton4 = (CButton*)GetDlgItem(IDC_btnDIVNO_STATUS_CHANGE);

	if(m_nMyData == 0)
	{
		strCreator = m_pInfo->USER_NAME;
		pButton1->EnableWindow(TRUE);
		pButton2->EnableWindow(TRUE);
		pButton3->EnableWindow(TRUE);
		pButton4->EnableWindow(TRUE);
	}
	else
	{
		pButton1->EnableWindow(FALSE);
		pButton2->EnableWindow(FALSE);
		pButton3->EnableWindow(FALSE);
		pButton4->EnableWindow(FALSE);
	}

	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);
	m_pCM->GetControlMgrData(_T("�۾�����"), strWorkStatus);
	
	if(strWorkStatus == _T("0")) strWorkStatus = _T("���Դ��");
	else if(strWorkStatus == _T("1")) strWorkStatus = _T("�ֹ�");
	else if(strWorkStatus == _T("2")) strWorkStatus = _T("�˼��Ϸ�");
	else strWorkStatus = _T("��ü�ڷ�");
	
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempData(_T("Status"), strWorkStatus.GetBuffer(0));
	mgr.SetTempData(_T("AcquisitYear"), strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("Creator"), strCreator.GetBuffer(0));
	mgr.SetTempData(_T("WorkCode"), strWorkCode.GetBuffer(0));
	mgr.SetTempData(_T("DIVNO_MGR"), TRUE);
	INT ids = mgr.SPFExecutePath(_T("DM����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("���������� ������ �� �����ϴ�."));
		return -1;
	}
	CString strQuery;
	CGroupCollection GroupCollection(this);
	
	mgr.GetTempData(_T("����"), strQuery);
	mgr.InitTempData();	mgr.InitTempPtr();
	mgr.SetTempData(_T("����"), strQuery.GetBuffer(0));
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("GroupCollection"), (DWORD*)&GroupCollection);
	mgr.SetTempData(_T("SPECIES_TYPE"), (INT)SPECIES_TYPE_INVALID);
	mgr.SetTempData(_T("WORK_CODE"), strWorkCode.GetBuffer(0));
	mgr.SetTempData(_T("ACQ_YEAR"), strAcqYear.GetBuffer(0));
	ids = mgr.SPFExecutePath(_T("������ȣå������"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("å�������� ���� �� �����ϴ�."));
		return -1;
	}
	
	return ids;
}

VOID CDivisionNo_Mgr::OnbtnDIVNOCOPY() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	CString strMsg, strWorkCode, strWorkStatus;
	m_pCM->GetControlMgrData(_T("�۾�����"), strWorkStatus);
	if(strWorkStatus == _T("2"))
		ESLAfxMessageBox(_T("��Ϲ�ȣ�� �ο��� �ڷ�� ������� �ʽ��ϴ�."));

	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	ids = pGrid->SelectMakeList();
	INT i= pGrid->SelectGetHeadPosition();
	CGroupInfo SourceGroup, TargetGroup;
	CGroupPair mgr(this);
	mgr.SetGroupType(DIVNO);
	mgr.SetMainDM(m_pDM);
	while(i >= 0)
	{
		strWorkCode = m_pDM->GetCellData(_T("��������"), i);
		ids = InsertDivNoInfo(strWorkCode);
		if(ids == 0)
		{
			SetGroupInfo(&SourceGroup, i);
			SourceGroup.SetRecKey(_T("1"), m_pDM);
			SetGroupInfo(&TargetGroup, m_pDM->GetRowCount()-1);
			mgr.SetTargetGroup(&TargetGroup);
			mgr.SetSourceGroup(&SourceGroup);
			ids = mgr.CopyGroup();
			if(ids < 0)
			{
				strMsg.Format(_T("%s�� %s�� ������ ������ �� �����ϴ�."), SourceGroup.GetAcqYear(), SourceGroup.GetGroupInfo());
				ESLAfxMessageBox(strMsg);
			}
			mgr.UpdateGroupBookCnt(&TargetGroup);
			strMsg.Format(_T("%s�� %s�� ������ �����Ͽ����ϴ�."), SourceGroup.GetAcqYear(), SourceGroup.GetGroupInfo());
			ESLAfxMessageBox(strMsg);
		}

		i = pGrid->SelectGetNext();
	}
	
DisplayGrid(_T("MainGrid"));
}

VOID CDivisionNo_Mgr::OnbtnDIVNODIVIDE() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CString strWorkStatus;
	m_pCM->GetControlMgrData(_T("�۾�����"), strWorkStatus);
	if(strWorkStatus == _T("0")) strWorkStatus = _T("���Դ��");
	else if(strWorkStatus == _T("1")) strWorkStatus = _T("�ֹ�");
	else if(strWorkStatus == _T("2")) strWorkStatus = _T("�˼��Ϸ�");
	else return ;

	CDivisionNo_Divide_Dlg	dlg(this);
	dlg.SetParentDM(m_pDM);
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	dlg.SetParentGrid(pGrid);
	dlg.SetWorkingStatus(strWorkStatus);
	dlg.DoModal();

	OnbtnDIVNOSEARCH();

	DisplayGrid(_T("MainGrid"));
}

INT CDivisionNo_Mgr::CheckSelectDataValid()
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	if(m_pDM->GetRowCount() == 0) return -1;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CDivisionNo_Mgr::OnbtnDIVNOMERGE() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}	
	
	CString strData;
	m_pCM->GetControlMgrData(_T("�����������"), strData);

	CGroupCollection GroupCollection(this);
	GroupCollection.SetAcqType(PURCHASE);
	GroupCollection.RemoveAll();
	SetSelGroupCollection(&GroupCollection);

	CGroupInfo * pGroup = NULL;

	CString strWorkStatrus, strAcqYear;
	m_pCM->GetControlMgrData(_T("�۾�����"), strWorkStatrus);
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	CMergeGroup_Select_Dlg dlg(this);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetWorkStatus(strWorkStatrus);
	dlg.SetDM(m_pDM);
	dlg.SetGroupType(DIVNO);
	dlg.SetGroupCollection(&GroupCollection);
	if(IDOK == dlg.DoModal())	
	{
		CGroupPair mgr(this);
		pGroup = dlg.GetSelGroup();
		mgr.SetTargetGroup(pGroup);
		mgr.SetSourceGroupCollection(&GroupCollection);
		mgr.SetGroupType(DIVNO);
		mgr.SetMainDM(m_pDM);
		INT ids = mgr.MergeGroup();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("������ �ڷḦ ������ �� �����ϴ�."));
			return ;
		}

		CBO_BudgetMgr BudgetMgr(this);
		BudgetMgr.SetDMOut(m_pDM);
		BudgetMgr.SetExecuteType(strData);
		BUDGET_ERROR_TYPE res = BudgetMgr.CheckDuplicateGroupKey(pGroup);
		if(res == BUDGET_ERROR)
		{
			BOOL bError = FALSE;
			m_pDM->StartFrame();

			BudgetMgr.SetGroup(pGroup);
			
			if(!bError && strWorkStatrus == _T("1"))
			{
				res = BudgetMgr.DeleteBudgetExecutionInfoByGroup(pGroup);
				if(res != BUDGET_SUCCESS && res != BUDGET_IGNORE)
				{
					bError = TRUE;
				}
				else
				{
					res = BudgetMgr.MakePreBudgetExecutionInfo();
				}				
			}
			else if(!bError && strWorkStatrus == _T("2"))
			{
				res = BudgetMgr.MakeBudgetExecutionInfo();
			}
			if(res != BUDGET_SUCCESS && res != BUDGET_IGNORE)
			{
				bError = TRUE;
			}
			if(!bError)
			{
				ids = m_pDM->SendFrame();
				if(ids < 0) bError = TRUE;
				ids = m_pDM->EndFrame();
				if(ids < 0) bError = TRUE;
			}
			if(bError)
			{
				ESLAfxMessageBox(_T("���������� ���� �������������� ������ �� �����ϴ�."));
				return ;
			}
		}
		
		DisplayGrid(_T("MainGrid"));
	}
}

INT CDivisionNo_Mgr::InsertDivNoInfo(CString strWorkCode/*=_T("")*/)
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	CString strAcqYear, strWorkStatus;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("�۾�����"), strWorkStatus);
	if(strWorkStatus == _T("0")) strWorkStatus = _T("���Դ��");
	else if(strWorkStatus == _T("1")) strWorkStatus = _T("�ֹ�");
	else if(strWorkStatus == _T("2")) strWorkStatus = _T("�˼��Ϸ�");
	else return -1;

	CESL_DataMgr * pWorkNoDM = FindDM(_T("DM_�۾���ȣ����"));

	m_pDM->TBL_NAME = _T("BO_PURCHASE_SEQ_NO_TBL S");

	CDivisionNo_Insert_Dlg dlg(this);
	dlg.SetDivNoDMPtr(m_pDM);
	dlg.SetWorkNoDMPtr(pWorkNoDM);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetWorkStatus(strWorkStatus);
	dlg.SetWorkCode(strWorkCode);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetParentGrid(pGrid);
	if(!strWorkCode.IsEmpty()) dlg.SetOpenMode(DINSERT);
	if(IDOK == dlg.DoModal()) 
	{
		m_pDM->TBL_NAME = _T("BO_PURCHASE_SEQ_NO_TBL S, CO_BUDGET_EXECUTION_TBL B");
		return 0;
	}	
	m_pDM->TBL_NAME = _T("BO_PURCHASE_SEQ_NO_TBL S, CO_BUDGET_EXECUTION_TBL B");

	return -1;
}

INT CDivisionNo_Mgr::SetGroupInfo(CGroupInfo *pGroup, INT idx)
{
	pGroup->SetAcqYear(m_pDM->GetCellData(_T("���Գ⵵"), idx));
	pGroup->SetGroupInfo(m_pDM->GetCellData(_T("������ȣ"), idx));
	pGroup->SetLastWorkNo(m_pDM->GetCellData(_T("�������۾���ȣ"), idx));
	pGroup->SetWorkCode(m_pDM->GetCellData(_T("��������"), idx));
	return 0;
}

INT CDivisionNo_Mgr::SetSelGroupCollection(CGroupCollection * pGroupCollection)
{
	CString strMsg, strAcqYear, strGroupInfo, strWorkCode;
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	INT ids = pGrid->SelectMakeList();
	INT i = pGrid->SelectGetHeadPosition();
	while(i>=0)
	{
		strAcqYear	 = m_pDM->GetCellData(_T("���Գ⵵"), i);
		strGroupInfo = m_pDM->GetCellData(_T("������ȣ"), i);
		strWorkCode  = m_pDM->GetCellData(_T("��������"), i);
		if( !pGroupCollection->IsSameWorkCode(m_pDM->GetCellData(_T("��������"), i)) )
		{
			strMsg.Format(_T("%s-%s-%s ������ ���յ� �� �����ϴ�."), strAcqYear, strWorkCode, strGroupInfo);
			ESLAfxMessageBox(strMsg);
		}
		else pGroupCollection->AddGroupInfo(strAcqYear, strGroupInfo, strWorkCode);

		i = pGrid->SelectGetNext();
	}
	return 0;
}

BEGIN_EVENTSINK_MAP(CDivisionNo_Mgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CDivisionNo_Mgr)
	ON_EVENT(CDivisionNo_Mgr, IDC_gridDIVNO_LIST, -601 /* DblClick */, OnDblClickgridDIVNOLIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDivisionNo_Mgr::OnDblClickgridDIVNOLIST() 
{
	// TODO: Add your control notification handler code here
	UpdateDivNo(GUPDATE);
}

VOID CDivisionNo_Mgr::OnSelchangecmbDIVNOWORKCODE() 
{
	// TODO: Add your control notification handler code here
	INT ids = MakeDM();
	if(ids < 0) return;
	DisplayGrid(_T("MainGrid"));
}

VOID CDivisionNo_Mgr::UpdateDivNo(OPERATION_TYPE nOpType)
{
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return ;
	}
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	if(pGrid == NULL) return ;
	
	CDivisionNo_Update_Dlg dlg(this);
	dlg.SetMainGrid(pGrid);
	dlg.SetOpType(nOpType);
	dlg.DoModal();
}

VOID CDivisionNo_Mgr::OnbtnDIVNOSTATUSCHANGE() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CArray<INT, INT> arrIdx;	arrIdx.RemoveAll();
	CString strTmpData, strDivNo, strAcqYear, strSelDivNo, strWorkCode, strWorkStatus;
	INT nTmpData = -1;
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	INT i = pGrid->SelectGetHeadPosition();
	while(i >= 0)
	{
		strTmpData = m_pDM->GetCellData(_T("å��"), i);
		strDivNo = m_pDM->GetCellData(_T("������ȣ"), i);
		nTmpData = _ttoi(strTmpData.GetBuffer(0));
		if(nTmpData != 0) 
		{
			pGrid->SetReverse(i);
			strTmpData.Format(_T("�����Ͻ� %s������ �������� �ƴϹǷ� ���º����� �� �����ϴ�."), strDivNo);
			pGrid->SetAt(i, 0, _T(""));
			ESLAfxMessageBox(strTmpData);
			i = pGrid->SelectGetNext();
			continue;
		}
		if(strSelDivNo.IsEmpty()) strSelDivNo = strDivNo;
		else strSelDivNo += _T(", ") + strDivNo;
		arrIdx.Add(i);
		i = pGrid->SelectGetNext();
	}

	if(arrIdx.GetSize() == 0)
	{
		ESLAfxMessageBox(_T("���º����� ������ �����ϴ�."));
		return;
	}
	
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);
	m_pCM->GetControlMgrData(_T("�۾�����"), strWorkStatus);
	if(strWorkStatus == _T("0")) strWorkStatus = _T("���Դ��");
	else if(strWorkStatus == _T("1")) strWorkStatus = _T("�ֹ�");
	else if(strWorkStatus == _T("2")) strWorkStatus = _T("�˼��Ϸ�");
	else return ;
	
	CDivisionNo_Status_Change_Dlg dlg(this);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetWorkCode(strWorkCode);
	dlg.SetWorkStatus(strWorkStatus);
	dlg.SetSelIdx(&arrIdx);
	dlg.SetDM(m_pDM);
	dlg.SetDivNo(strSelDivNo);
	if(IDOK == dlg.DoModal())
	{
		ESLAfxMessageBox(_T("�����Ͻ� ������ ���¸� �����Ͽ����ϴ�."));
		pGrid->Display();
	}
}

VOID CDivisionNo_Mgr::OnSelchangecmbDIVNOWORKSTATUS() 
{
	// TODO: Add your control notification handler code here
	CString strWorkStatus;
	CString str64;
	m_pCM->GetControlMgrData(_T("�۾�����"), strWorkStatus);
	strWorkStatus.TrimLeft();	strWorkStatus.TrimRight();

		GetDlgItem(IDC_btnDIVNO_MERGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnDIVNO_DIVIDE)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnDIVNO_STATUS_CHANGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnDIVNO_COPY)->EnableWindow(TRUE);
	
	GetManageValue( _T("��Ÿ"), _T("����"), _T("����"), _T("����"), str64 );
	if( strWorkStatus == _T("0") && str64 != _T("Y") )	GetDlgItem(IDC_btnBUDGET_EXECUTION)->EnableWindow(FALSE);
	else							GetDlgItem(IDC_btnBUDGET_EXECUTION)->EnableWindow(TRUE);


	INT ids = MakeDM();
	if(ids < 0) return;
	DisplayGrid(_T("MainGrid"));	
}

BOOL CDivisionNo_Mgr::Init()
{
	INT ids;
	CString str64;
	CString strInitData;
	CString strData[4][2] = {	_T(" "), _T("��ü�ڷ�"),
		_T("0"), _T("���Դ��"),
		_T("1"), _T("�ֹ�"),
		_T("2"), _T("�˼��Ϸ�")
	};
	CString strExecData[2][2] = {	_T("0"), _T("������"),
									_T("1"), _T("����")
								};
	CMulitComboBoxManager mgr(this);
	
	ids = mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, m_strStatus, strData, _T("�۾�����"), 4);
	if( ids < 0 )	return FALSE;

	ids = mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("����"), strExecData, _T("�����������"), 2); 	
	if( ids < 0 )	return FALSE;

	CString strWorkStatus;
	m_pCM->GetControlMgrData(_T("�۾�����"), strWorkStatus);
	strWorkStatus.TrimLeft();	strWorkStatus.TrimRight();

	GetManageValue( _T("��Ÿ"), _T("����"), _T("����"), _T("����"), str64 );
	if( strWorkStatus == _T("0") && str64 != _T("Y") )	GetDlgItem(IDC_btnBUDGET_EXECUTION)->EnableWindow(FALSE);
	else							GetDlgItem(IDC_btnBUDGET_EXECUTION)->EnableWindow(TRUE);
	
	return TRUE;
}

BOOL CDivisionNo_Mgr::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if (GetDlgItem(IDC_edtDIVNO_ACQYEAR)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnDIVNO_SEARCH, BN_CLICKED);
		else if (GetDlgItem(IDC_radDIVNO_MYDATA)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnDIVNO_SEARCH, BN_CLICKED);
		else if (GetDlgItem(IDC_radDIVNO_ALLDATA)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnDIVNO_SEARCH, BN_CLICKED);
		else if (GetDlgItem(IDC_cmbDIVNO_WORK_STATUS)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnDIVNO_SEARCH, BN_CLICKED);
		else if (GetDlgItem(IDC_cmbDIVNO_WORK_CODE)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnDIVNO_SEARCH, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CDivisionNo_Mgr::OnbtnDIVNODELETESUB() 
{
	OnbtnDIVNODELETE();
}

VOID CDivisionNo_Mgr::OnbtnDIVNOINSERTSUB() 
{
	OnbtnDIVNOINSERT();
}

VOID CDivisionNo_Mgr::OnbtnDIVNOUPDATESUB() 
{
	OnbtnDIVNOUPDATE();
}

VOID CDivisionNo_Mgr::OnbtnBUDGETEXECUTION() 
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	CString strData;
	m_pCM->GetControlMgrData(_T("�����������"), strData);

	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	CGroupCollection GroupCollection(this);	GroupCollection.RemoveAll();
	GroupCollection.SetAcqType(PURCHASE);
	CString strMsg;
	INT i = m_pGrid->SelectGetHeadPosition();
	while(i >= 0) 
	{
		ids = GroupCollection.AddGroupInfo(
				m_pDM->GetCellData(_T("���Գ⵵"), i),
				m_pDM->GetCellData(_T("������ȣ"), i),
				m_pDM->GetCellData(_T("��������"), i));
		if(ids < 0) return ;
		ArrIdx.Add(i);
		i = m_pGrid->SelectGetNext();
	}

	if(GroupCollection.GetGroupCount() == 0) return ;

	m_pDM->StartFrame();
	CBO_BudgetMgr BudgetMgr(this);
	BudgetMgr.SetDMOut(m_pDM);
	BudgetMgr.SetGroupCollection(&GroupCollection);
	BudgetMgr.SetExecuteType(strData);
	BudgetMgr.SetMode(_T("MO"));
	BUDGET_ERROR_TYPE res;
	res = BudgetMgr.MakeBudgetExecutionInfo();	

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
		m_pGrid->DisplayLine(ArrIdx.GetAt(i));
	}

	AfxMessageBox ( _T("�������� ó���Ǿ����ϴ�.") );
	return ;
}

VOID CDivisionNo_Mgr::OnbtnBUDGETCANCEL() 
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
					m_pDM->GetCellData(_T("���Գ⵵"), i),
					m_pDM->GetCellData(_T("������ȣ"), i),
					m_pDM->GetCellData(_T("��������"), i));
			if(ids < 0) return ;
			ArrIdx.Add(i);
		}
		else
		{
			strMsg.Format(
				_T("[%s:%s]������ �������������� �����ϴ�."),
				m_pDM->GetCellData(_T("���Գ⵵"), i),
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
	BudgetMgr.SetGroupCollection(&GroupCollection);
	BudgetMgr.SetMode(_T("MO"));
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
	return ;	
}

VOID CDivisionNo_Mgr::ShowControlResource()
{
	if(m_strUseBudgetYN == _T("Y"))
	{
		GetDlgItem(IDC_btnDIVNO_INSERT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnDIVNO_UPDATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnDIVNO_DELETE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnDIVNO_INSERT_SUB)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnDIVNO_UPDATE_SUB)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnDIVNO_DELETE_SUB)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnBUDGET_EXECUTION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnBUDGET_CANCEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbEXECUTION)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_btnDIVNO_INSERT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnDIVNO_UPDATE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnDIVNO_DELETE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnDIVNO_INSERT_SUB)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnDIVNO_UPDATE_SUB)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnDIVNO_DELETE_SUB)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnBUDGET_EXECUTION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnBUDGET_CANCEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbEXECUTION)->ShowWindow(SW_HIDE);
	}
}





HBRUSH CDivisionNo_Mgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
