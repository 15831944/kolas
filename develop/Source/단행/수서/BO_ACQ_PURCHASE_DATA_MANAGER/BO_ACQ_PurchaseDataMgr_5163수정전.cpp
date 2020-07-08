// BO_ACQ_PurchaseDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_PurchaseDataMgr.h"
#include "BO_ACQ_OrderDlg.h"
#include "BO_ACQ_ReviewDlg.h"
#include "PurchaseDataPrintDlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/DivisionNo_Select_Dlg.h"
#include "../../../����/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
#include "../../../����/BO_ACQ_COMMON/BO_ACQ_ImportMgr.h"
#include "../../../����/BO_ACQ_COMMON/OrderMailMgr.h"
#include "../../../����/Duplicate_Data_Search/DUP_KEY_SETTING.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PurchaseDataMgr dialog


CBO_ACQ_PurchaseDataMgr::CBO_ACQ_PurchaseDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_PurchaseDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_PurchaseDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pPrintDM = NULL;
	m_pPrintObjDM = NULL;
	m_pSubTotalPrintDM = NULL;
	m_pSearchDlg = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_strDivNo = _T("");
	m_strAcqYear = _T("");
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_bPurchaseCheckYN = FALSE;
	m_pSortItemSet = NULL;
	m_nRow = -1;
}

CBO_ACQ_PurchaseDataMgr::~CBO_ACQ_PurchaseDataMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}	
	if(m_pDuplicateSearchMgr)
	{
		delete m_pDuplicateSearchMgr;
		m_pDuplicateSearchMgr = NULL;
	}
	if(m_pPrintDlg)
	{
		delete m_pPrintDlg;
		m_pPrintDlg = NULL;
	}
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
}

VOID CBO_ACQ_PurchaseDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_PurchaseDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_PurchaseDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_PurchaseDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnShowSearchResult)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(DUPLICATE_DATA_DELETE, OnDuplicateDataDelete)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PurchaseDataMgr message handlers

VOID CBO_ACQ_PurchaseDataMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CBO_ACQ_PurchaseDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_���Դ���ڷ����")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pDM = FindDM(_T("DM_�����ڷ����"));
	m_pPrintDM = FindDM(_T("DM_�����ڷ����_��"));
	m_pPrintObjDM = FindDM(_T("DM_�����ڷ����_��¿�"));
	m_pSubTotalPrintDM = FindDM(_T("DM_�ֹ��ڷ����_���"));
	m_pCM = FindCM(_T("CM_�����ڷ����"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	
	GetDlgItem(IDC_stcPURCHASE_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcPURCHASE_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);	
	
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_PURCHASE, PURCHASE, BO_PURCHASE, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_PURCHASE, PURCHASE, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	InitSortItem();
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	SetPurchaseDataInfo();
	InitPopupMenuItem();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_PurchaseDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CButton *pBtn = (CButton *)GetDlgItem(IDC_stcPURCHASE_DATA);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->MoveWindow(5, 5, cx-10, 30);

	CRect rect;
	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_GROUP_CREATE_YEAR);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(150, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(205, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridPURCHASE_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(5, 40, cx-10, cy-45);		
}

VOID CBO_ACQ_PurchaseDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) 
	{
		m_pPrintDlg->Create(this);
	}
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowPurchaseDataDlg(OPERATION_TYPE nOpType)
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	CSpecies_Form dlg(this);
	dlg.SetAcqType(PURCHASE);
	dlg.SetSpeciesType(BO_PURCHASE);
	dlg.SetOpType(nOpType);
	if(nOpType != SINSERT)
	{
		if(0 == m_pPrintDM->GetRowCount()) 
		{
			ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
			return ;
		}
	}
	dlg.SetSpeciesKeyAlias(_T("IBS_��KEY"));
	dlg.SetAccuKeyAlias(_T("BP_��������KEY"));
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentType(SPECIES);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	SetPurchaseDataInfo();
	if(nOpType == SINSERT)
	{
		INT nCurCnt = m_pPrintDM->GetRowCount();
		if(nCurCnt > nRowCnt)
		{
			m_pGrid->DisplayNumber();
			m_pGrid->SetTopRow(nRowCnt+1);
		}
	}
}

VOID CBO_ACQ_PurchaseDataMgr::DeletePurchaseData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_PURCHASE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("����Ÿ��"), (INT)SPECIES_DELETE);
	api.SetTempData(_T("å���˸��ƽ�"), _T("BP_����å��"));
	api.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_��ȯ����������"));
	api.SetTempData(_T("��������"), _T(""));
	api.SetTempPtr(_T("��Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("��������Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("���Ϸù�ȣ����Ʈ"), NULL);
	ids = api.SPFExecutePath(_T("����"));
	if(ids == 0)
	{
		SetPurchaseDataInfo();
		DisplayGrid();	
	}
}

VOID CBO_ACQ_PurchaseDataMgr::ReviewPurchaseData()
{
	CString strGroupNo, strSortDesc;
	
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� ���������� �����ϴ�."));
		return;
	}
	//�����ݷ����� ��ü�ڷ��� ��� Ư�������� ������ ���� ����
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("��ü�ڷ�"))
	{
		ESLAfxMessageBox(_T("���� ������ ������ �����ϴ�. �˻��� ���Ͽ� Ư�� ������ ������ �ֽʽÿ�."));
		return;
	}
	
	CBO_ACQ_ReviewDlg dlg(this);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	if(IDCANCEL == dlg.DoModal()) return ;

	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupNo);
	m_pCM->GetControlMgrData(_T("�׷��ȣ"), strGroupNo);
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();	
	api.SetTempData(_T("�׷��ȣ"), strGroupNo.GetBuffer(0));
	api.SetTempData(_T("���Գ⵵"), m_pSearchDlg->GetGroupCollection()->GetAcqYear().GetBuffer(0));
	api.SetTempData(_T("��������"), m_pSearchDlg->GetGroupCollection()->GetWorkCode().GetBuffer(0));
	INT ids = api.SPFExecutePath(_T("����ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �� �� �����ϴ�."));
		return;
	}
	ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �Ͽ����ϴ�."));
	if(dlg.m_bReviewResultPrint)
	{	
		ShowDataProcessingDlg();
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CMakePrintDMMgr PrintMgr(this);
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("����ƮŸ��"), BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO);
		PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("BP_��������KEY"));
		PrintMgr.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
		PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_����"));
		PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_���ΰ�"));
		PrintMgr.SetTempData(_T("åDM�˸��ƽ�"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
		PrintMgr.SetTempData(_T("KOLAS2����"), FALSE);
		PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), FALSE);
		PrintMgr.SetTempData(_T("��µ�����Ÿ��"), 0);
		PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("�׷��÷���"), (DWORD*)m_pSearchDlg->GetGroupCollection());
		PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pSubTotalPrintDM);
		PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
		PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T(""));
		
		ids = PrintMgr.SPFExecutePath(_T("�Ұ��ڷ����"));
		DeleteDataProcessingDlg();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
			return ;
		}
		if(ids == 1000) return ;
		
		m_pPrintDlg->CreatePrintMgr();
		m_pPrintDlg->SetPrintDM(m_pSubTotalPrintDM);
		m_pPrintDlg->SetSIReportFileName(_T("K2UP_���ǰ���ڷ���"));
		m_pPrintDlg->PrintPaper();
	}
}

VOID CBO_ACQ_PurchaseDataMgr::OrderPurcahseData()
{
	CBO_ACQ_Business_Api api(this);
	
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� ���������� �����ϴ�."));
		return;
	}
	//�����ݷ����� ��ü�ڷ��� ��� Ư�������� ������ ���� ����
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("��ü�ڷ�"))
	{
		ESLAfxMessageBox(_T("���� ������ ������ �����ϴ�. �˻��� ���Ͽ� Ư�� ������ ������ �ֽʽÿ�."));
		return;
	}

	//�׷��÷��� ������ �����Ѵ�.
	CGroupCollection GroupCollection(this);
	m_pSearchDlg->GetGroupCollection()->CopyData(&GroupCollection);
	
	CheckGiveOrderSerialNo(&GroupCollection);
	api.CheckValidationOrderProcess(m_pDM, &GroupCollection);
	if(GroupCollection.GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("�ֹ��ϱ� ���� ���������� �����ϴ�."));
		return;
	}	

	CBO_ACQ_OrderDlg dlg(this);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetGroupCollection(&GroupCollection);
	if(IDCANCEL == dlg.DoModal()) return ;
	
	api.InitTempData();	
	api.SetTempPtr(_T("�׷�����"), (DWORD*)&GroupCollection);
	CString strData = GroupCollection.GetAcqYear();
	api.SetTempData(_T("���Գ⵵"), strData.GetBuffer(0));
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids = api.SPFExecutePath(_T("�ֹ�ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ �ֹ�ó�� �� �� �����ϴ�."));
		return;
	}
	
	ShowOrderCancelMailMgr(ORDER, AUTO, &GroupCollection);

	ESLAfxMessageBox(_T("������ �ڷḦ �ֹ�ó�� �Ͽ����ϴ�."));
	m_pSearchDlg->GetGroupCollection()->DeleteThisGroupInfo(&GroupCollection);
	api.InitTempData();
	api.SetTempPtr(_T("DM_BOOK"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("DM_SPECIES"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("DM_VOLUME"), (DWORD*)NULL);
	api.SetTempPtr(_T("GROUPINFO"), (DWORD*)&GroupCollection);
	ids = api.SPFExecutePath(_T("�׷��ڷ����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�ֹ��ڷḦ ������ �� �����ϴ�."));
		return ;
	}

	SetPurchaseDataInfo();
	DisplayGrid();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowChangeCodeDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	CCodeChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.SetAcqType(1);
	if(IDOK == dlg.DoModal())
	{
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_PurchaseDataMgr::ShowChangePurchaseInfoDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	CPurchaseInfoChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	if(IDOK == dlg.DoModal())
	{
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_PurchaseDataMgr::ShowDivNoMgrDlg()
{
	CDivisionNo_Mgr mgr(this);
	mgr.SetWorkStatus(_T("���Դ��"));
	mgr.DoModal();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowPriceSumDlg()
{
	CPriceSumDlg pDlg(this);
	pDlg.SetOpenMode(K2UPGRADE);
	pDlg.SetSpeciesType(BO_PURCHASE);
	pDlg.SetParentDM(m_pPrintDM);
	pDlg.SetParentGrid(m_pGrid);
	pDlg.SetSpeciesYN(TRUE);
	pDlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
	pDlg.DoModal();
}

VOID CBO_ACQ_PurchaseDataMgr::CopyPurchaseDataSearch()	
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	if(!m_pDuplicateSearchMgr)
	{
		m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
		m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	}

	if(m_pDuplicateSearchMgr->GetRunning())		
	{
		ESLAfxMessageBox(_T("�̹� �ߺ����簡 �������Դϴ�."));
		return ;
	}		
	m_pDuplicateSearchMgr->ShowWorkDuplicateSearch(_T("�ߺ�����"), m_pPrintDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"), m_pSearchDlg->GetGroupCollection());
}

VOID CBO_ACQ_PurchaseDataMgr::DupPurchaseDataSearch()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	if(!m_pDuplicateSearchMgr)
	{
		m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
		m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	}

	if(m_pDuplicateSearchMgr->GetRunning())		
	{
		ESLAfxMessageBox(_T("�̹� �������簡 �������Դϴ�."));
		return ;
	}		
	m_pDuplicateSearchMgr->ShowHoldDuplicateSearch(_T("��������"), m_pPrintDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"));
}

BOOL CBO_ACQ_PurchaseDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

UINT CBO_ACQ_PurchaseDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("���Դ���ڷḦ �������Դϴ�."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		SetPurchaseDataInfo();
		DisplayGrid();
		return 0;
	}
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	INT ids = mgr.SPFExecutePath(_T("åTo��������"));
	DeleteDataProcessingDlg();
	if(ids < 0) return ids;
	m_pSortItemSet->DataSort(m_pPrintDM, !m_pSearchDlg->GetDataSortYN());
	SetPurchaseDataInfo();
	//SortByGroupInfo();	
	DisplayGrid();
	
	return 0;
}


UINT CBO_ACQ_PurchaseDataMgr::OnDuplicateDataDelete(LPARAM lParam, WPARAM wParam)
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("��DM"), NULL);
	api.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("����������"), (DWORD*)m_pDuplicateSearchMgr->GetAcqKeyArray());
	api.SetTempData(_T("��Alias"), _T("IBS_��KEY"));
	INT ids = api.SPFExecutePath(_T("DM�ڷ����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ DM���� ������ �� �����ϴ�."));
		return 0;
	}
	DisplayGrid();
	SetPurchaseDataInfo();
	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode;
	INT nGroupNo, nWorkNo, nSerialNo;
	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = m_pPrintDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("IBS_�ǻ���"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = m_pPrintDM->GetCellData(_T("IBS_��ǥ��"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_ǥ������"), strTmpData, nRow);
		ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_������"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == m_pPrintDM->GetRowCount()-1) m_nRow = -1;
	}

	if(strDMFieldAlias == _T("UDF_��������"))
	{	
		strAcqYear = m_pPrintDM->GetCellData(_T("BP_���Գ⵵"), nRow);
		strWorkCode = m_pPrintDM->GetCellData(_T("IBS_��������"), nRow);
		strGroupNo = m_pPrintDM->GetCellData(_T("BP_�׷��ȣ"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		strWorkNo = m_pPrintDM->GetCellData(_T("BP_�۾���ȣ"), nRow);
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		strSerialNo = m_pPrintDM->GetCellData(_T("BP_�Ϸù�ȣ"), nRow);
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
		strGroupNo.Format(_T("%04d-%03d-%05d"), nGroupNo, nWorkNo, nSerialNo);
		m_pPrintDM->SetCellData(_T("UDF_�����������"), strGroupNo, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_�Է±��м���"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("BP_�Է±���"), nRow);
		if(strTmpData == _T("0")) str = _T("ȭ���Է�");
		else if(strTmpData == _T("1")) str = _T("��Ȱ�����");
		else if(strTmpData == _T("2")) str = _T("����ڷ����");
		else if(strTmpData == _T("3")) str = _T("���Ϲ���");
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_��������"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("IBS_�з�ǥ����"), nRow);
		if(strTmpData.IsEmpty()) str = _T("");
		else if(strTmpData == _T("1"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC��������ȣ����"), str, str);
		}
		else if(strTmpData == _T("2"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC��������ȣ����"), str, str);
		}
		m_pPrintDM->SetCellData(_T("UDF_�������м���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_����ó��ȣ����"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("BP_����ó��ȣ"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����ó����"), strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_����������"))
	{
		CString strTmpData;
		INT nBookCnt = 0;
		DOUBLE dPrice, dDisc_Rate, dDisc_Price;
		strTmpData = m_pPrintDM->GetCellData(_T("BP_������"), nRow);
		dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
		//strTmpData = m_pPrintDM->GetCellData(_T("UDF_������"), nRow);
		strTmpData = m_pPrintDM->GetCellData(_T("BP_����"), nRow);
		dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
		//2004.09.14 ȭ�󱸺а��� �Ҽ��� 2�ڸ����� ��Ÿ����.
		//strTmpData.Format(_T("%.0f"), dDisc_Price);
		strTmpData.Format(_T("%.2f"), dDisc_Price);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pPrintDM->SetCellData(_T("UDF_����������"), strTmpData, nRow);

		// 2004.09.14
		str.Replace(_T(",."), _T("."));
		m_pPrintDM->SetCellData(_T("UDF_��ȯ����������"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_������"))
	{
		str.TrimLeft();		str.TrimRight();
		// 2004.09.14  ȭ�󱸺а��� �Ҽ��� 2�ڸ����� ��Ÿ����.
		DOUBLE strTemp;
		strTemp = (DOUBLE)atof(m_pPrintDM->GetCellData(_T("BP_����"), nRow));
		str.Format(_T("%.2f"), strTemp);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		str.Replace(_T(",."), _T("."));  // 2004.09.14
		m_pPrintDM->SetCellData(_T("UDF_��ȯ������"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_�ܰ�"))
	{
		// 2004.09.14  ȭ�󱸺а��� �Ҽ��� 2�ڸ����� ��Ÿ����.
		DOUBLE strTemp;
		strTemp = (DOUBLE)atof(str);
		str.Format(_T("%.2f"), strTemp);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		str.Replace(_T(",."), _T("."));  // 2004.09.14
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CBO_ACQ_PurchaseDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pPrintDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pPrintDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

VOID CBO_ACQ_PurchaseDataMgr::SetPurchaseDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d�� %då�� ���� ��� �ڷᰡ �˻��Ǿ����ϴ�."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), strMsg);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

INT CBO_ACQ_PurchaseDataMgr::ShowDataPurchaseImport()
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	m_pCM->GetControlMgrData(_T("�׷��ȣ"), m_strDivNo);
	CBO_ACQ_ImportMgr importDlg(this);
	importDlg.SetAcqType(PURCHASE);
	CGroupCollection * pGroups = m_pSearchDlg->GetGroupCollection();
	if(pGroups->GetGroupCount() == 0)
	{
		if(IDYES == ESLAfxMessageBox(_T("������ ���������� �����ϴ�. ������ �����Ͻðڽ��ϱ�?"), MB_YESNO))
		{
			CDivisionNo_List_Dlg dlg(this);
			dlg.SetWorkStatus(_T("���Դ��"));
			dlg.SetAcquisitYear(pGroups->GetAcqYear());
			dlg.SetGroupCollection(pGroups);
			dlg.DoModal();
		}
		else if(m_pInfo->GetKIIUPClientVersion() == 'L')
		{
			pGroups->AddGroupInfo(pGroups->GetAcqYear(), _T("�������"), pGroups->GetWorkCode());
		}
		else return 0;
	}
	else if(m_pInfo->GetKIIUPClientVersion() == 'L')
	{
		pGroups->AddGroupInfo(pGroups->GetAcqYear(), _T("�������"), pGroups->GetWorkCode());
	}
	
	if(pGroups->IsEmpty()) return 0;
	importDlg.SetGroups(m_pSearchDlg->GetGroupCollection());
	importDlg.SetParentDM(m_pPrintDM);
	importDlg.SetParentBookDM(m_pDM);
	importDlg.SetParentGrid(m_pGrid);
	importDlg.DoModal();

	if(m_pInfo->GetKIIUPClientVersion() == 'L')
	{
		CGroupInfo Group;
		Group.SetAcqYear(pGroups->GetAcqYear());
		Group.SetGroupInfo(_T("�������"));
		Group.SetWorkCode(pGroups->GetWorkCode());
		INT ids = pGroups->DeleteThisGroupInfo(&Group);
		if(ids < 0) return -1;
	}
	
	SetPurchaseDataInfo();
	INT nCurCnt = m_pPrintDM->GetRowCount();
	if(nCurCnt > nRowCnt)
	{
		m_pGrid->DisplayNumber();
		m_pGrid->SetTopRow(nRowCnt+1);
	}
	return 0;
}

VOID CBO_ACQ_PurchaseDataMgr::SetCheckOrderInvalidData(CArray <INT, INT> * pArrayInvalidData)
{
	INT cnt = pArrayInvalidData->GetSize();
	if(cnt == 0) return;
	for(INT i=0 ; i<cnt ; i++)
	{
		SetGridRowBackColor(m_pGrid, pArrayInvalidData->GetAt(i));
	}
}

INT CBO_ACQ_PurchaseDataMgr::SetGridRowBackColor(CESL_Grid *pGrid, INT nRow)
{
	if (pGrid == NULL) return -1;

	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = pGrid->GetCol();
	INT nCols = pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(red);
			pGrid->SetCellForeColor(black);
		}
		pGrid->SetTextMatrix(nRow+1, 1, _T("V"));
	}
	
	pGrid->SetCol(nCol);

	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::CheckSelectDataValid()
{
	if(m_pPrintDM->GetRowCount() == 0) return -1;

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

UINT CBO_ACQ_PurchaseDataMgr::OnShowSearchResult(LPARAM lParam, WPARAM wParam)
{
	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();
	return 0;
}

UINT CBO_ACQ_PurchaseDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	if(lParam == BO_ACQ_PURCHASE_DATA)
	{
		return PrintPurchaseData(lParam);
	}
	else if(lParam == BO_ACQ_PURCHASE_DATA_REGCODE)
	{
		return PrintPurchaseSubTotalBookData(lParam);
	}
	else 
	{
		return PrintPurchaseSubTotalData(lParam, wParam);
	}
	
	return -1;
}

INT CBO_ACQ_PurchaseDataMgr::PrintPurchaseSubTotalData(INT nPrintType, INT nExtraInfo)
{
	INT ids = -1;
	BOOL bExtraInfo = FALSE;
	if(nExtraInfo == 1000) bExtraInfo = TRUE;
	
	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	CString strDivAlias = _T("BP_��������KEY");
	CMakePrintDMMgr PrintMgr(this);
	if(m_pPrintDlg->GetPrintDataType() == 2)
	{
		ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return -1;
		}
	}
	ShowDataProcessingDlg();
	
	PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
	PrintMgr.SetTempData(_T("����ƮŸ��"), nPrintType);
	PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("BP_��������KEY"));
	PrintMgr.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
	PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_����"));
	PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_���ΰ�"));
	PrintMgr.SetTempData(_T("åDM�˸��ƽ�"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
	PrintMgr.SetTempData(_T("KOLAS2����"), bExtraInfo);
	PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
	PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
	PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
	PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
	PrintMgr.SetTempPtr(_T("�׷��÷���"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pSubTotalPrintDM);
	PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
	
	if(nPrintType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || nPrintType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO ||
		nPrintType == BO_ACQ_ORDER_DATA_DIVNO)
		PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T(""));
	else if(nPrintType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER || nPrintType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER ||
		nPrintType == BO_ACQ_ORDER_DATA_PROVIDER)
		PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BP_����ó��ȣ"));
	else if(nPrintType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE || nPrintType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE ||
		nPrintType == BO_ACQ_ORDER_DATA_SUBJECTCODE)
		PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BP_��������"));
	else 
	{
		ESLAfxMessageBox(_T("��¹� ������ ������ �� �����ϴ�."));
		DeleteDataProcessingDlg();
		return -1;
	}
	ids = PrintMgr.SPFExecutePath(_T("�Ұ��ڷ����"));
	DeleteDataProcessingDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
		return -1;
	}
	if(ids == 1000)
		return 0;
	
	m_pPrintDlg->SetPrintDM(m_pSubTotalPrintDM);
	m_pPrintDlg->Print();
	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::PrintPurchaseSubTotalBookData(INT nPrintType)
{
	INT ids = -1;
	CESL_DataMgr PrintBookDM;
	PrintBookDM.InitDMPKFromFile(m_pDM->DB_MGR_ALIAS);
	PrintBookDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	PrintBookDM.SetMarcMgr(m_pInfo->pMarcMgr);

	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	CString strDivAlias = _T("BP_��������KEY");
	CMakePrintDMMgr PrintMgr(this);
	if(m_pPrintDlg->GetPrintDataType() == 2)
	{
		ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return -1;
		}
	}
	ShowDataProcessingDlg();

	PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
	PrintMgr.SetTempData(_T("����ƮŸ��"), nPrintType);
	PrintMgr.SetTempData(_T("���о˸��ƽ�"), strDivAlias.GetBuffer(0));
	PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("BB_����"));
	PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_���ΰ�"));
	PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BB_��ϱ���"));
	PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
	PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
	PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
	PrintMgr.SetTempPtr(_T("�׷��÷���"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
	PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pDM);
	PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)&PrintBookDM);
	PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
	ids = PrintMgr.SPFExecutePath(_T("å�Ұ��ڷ����"));
	DeleteDataProcessingDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
		return -1;
	}
	if(ids == 1000)
		return 0;
	
	m_pPrintDlg->SetPrintDM(&PrintBookDM);
	m_pPrintDlg->Print();
	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::PrintPurchaseData(INT nPrintType)
{
	INT ids = -1;
	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	CString strDivAlias = _T("BP_��������KEY");
	CMakePrintDMMgr PrintMgr(this);
	if(m_pPrintDlg->GetPrintDataType() == 2)
	{
		ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return -1;
		}
	}
	ShowDataProcessingDlg();
	
	PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
	PrintMgr.SetTempData(_T("����ƮŸ��"), nPrintType);
	PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("BP_��������KEY"));
	PrintMgr.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
	PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_������"));
	PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_����������"));
	PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
	PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
	PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
	PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
	PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pDM);
	PrintMgr.SetTempPtr(_T("�������DM"), (DWORD*)m_pPrintDM);
	PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pPrintObjDM);
	PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
	ids = PrintMgr.SPFExecutePath(_T("�Ϲݸ���Ʈ���"));
	DeleteDataProcessingDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
		return -1;
	}
	if(ids == 1000)
		return 0;
	
	m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
	m_pPrintDlg->Print();
	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::CheckGiveOrderSerialNo(CGroupCollection *pGroupCollection)
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("�׷�����"), (DWORD*)pGroupCollection);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids = api.SPFExecutePath(_T("�ֹ��Ϸù�ȣȮ��"));
	return ids;
}

VOID CBO_ACQ_PurchaseDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	if(m_pInfo->GetKIIUPClientVersion() != 'L')
	{
		RemoveListArray.Add(_T("��������"));
	}
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
	for(INT i=0 ; i<RemoveListArray.GetSize() ; i++)
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&RemoveListArray[i], TRUE);
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_PurchaseDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_PurchaseDataMgr)
	ON_EVENT(CBO_ACQ_PurchaseDataMgr, IDC_gridPURCHASE_DATA, -601 /* DblClick */, OnDblClickgridPURCHASEDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_PurchaseDataMgr::OnDblClickgridPURCHASEDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return ;
	ShowPurchaseDataDlg(GUPDATE);
}

VOID CBO_ACQ_PurchaseDataMgr::DupKeySetting()
{
	CDUP_KEY_SETTING Dlg(this, _T(".\\duplicate_search_key.txt"));
	Dlg.DoModal();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowOrderCancelMailMgr(INT nOpType, INT nOpenType, CGroupCollection * pGroupCollection)
{
	COrderMailMgr mailMgr(this);
	if(pGroupCollection == NULL) pGroupCollection = m_pSearchDlg->GetGroupCollection();

	mailMgr.SetParentGroupCollection(pGroupCollection);
	mailMgr.SetOperationType(nOpType);
	mailMgr.SetOpenType(nOpenType);
	mailMgr.SetAcqYear(pGroupCollection->GetAcqYear());
	mailMgr.DoModal();
}

INT CBO_ACQ_PurchaseDataMgr::MakePrintDM()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0) return -1;
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("��ü�ڷ�"))
	{
		ESLAfxMessageBox(_T("���� ������ ������ �����ϴ�. �˻��� ���Ͽ� Ư�� ������ ������ �ֽʽÿ�."));
		return -1;
	}
	CESL_DataMgr * pDM = FindDM(_T("DM_�����ڷ����_���"));
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("��¿�DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	INT ids = api.SPFExecutePath(_T("��¿�DM����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
		return -1;
	}
	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::MakeOrderPrintDM(CString strSortField)
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0) return 0;
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("��ü�ڷ�"))
	{
		ESLAfxMessageBox(_T("���� ������ ������ �����ϴ�. �˻��� ���Ͽ� Ư�� ������ ������ �ֽʽÿ�."));
		return -1;
	}
	CESL_DataMgr * pDM = FindDM(_T("DM_�ֹ��ڷ����_���"));
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("��¿�DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	api.SetTempData(_T("SORT_FIELD"), strSortField.GetBuffer(0));
	INT ids = api.SPFExecutePath(_T("�ֹ������¿�DM����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
		return -1;
	}
	return 0;
}

VOID CBO_ACQ_PurchaseDataMgr::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(this);

	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(this))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//���α׷��� ��Ʈ�� �ʱ�ȭ
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
//	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CBO_ACQ_PurchaseDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

VOID CBO_ACQ_PurchaseDataMgr::GiveOrderSerialNo()
{
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return;
	}

	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� ���������� �����ϴ�."));
		return;
	}
	
	//�����Ϸù�ȣ �ο� ��ƾ
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();	
	api.SetTempPtr(_T("SpeciesDM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	INT ids = api.SPFExecutePath(_T("�����Ϸù�ȣ�ο�"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�ֹ��Ϸù�ȣ�� �ο��� �� �����ϴ�."));
		return;
	}
	else if(ids > 0)
	{
		CString strMsg;
		strMsg.Format(_T("���������� �ο����� ���� [%d]���� �ڷῡ ���Ͽ��� �ֹ��Ϸù�ȣ�� �ο����� �ʾҽ��ϴ�.\n")
			_T("�ش� �ڷ�� ���������ο��� �ֹ��Ϸù�ȣ�� �ο��� �� �ֽ��ϴ�."), ids);
		ESLAfxMessageBox(strMsg);
	}
	else
		ESLAfxMessageBox(_T("�ֹ��Ϸù�ȣ�� �ο� �Ͽ����ϴ�."));
	DisplayGrid();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowSortDlg(INT nType/*=1*/)
{
	if(m_pSortItemSet == NULL || m_pPrintDM == NULL) 
	{
		ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		return;
	}
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(m_pPrintDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(nType);
	if(IDOK == dlg.DoModal() && nType > 0)
		DisplayGrid();
}

UINT CBO_ACQ_PurchaseDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

BOOL CBO_ACQ_PurchaseDataMgr::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BP_���Գ⵵"));
	pTmpData->Add(_T("IBS_��������"));
	pTmpData->Add(_T("BP_�׷��ȣ"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("BP_�۾���ȣ"));
	pTmpData->Add(_T("BP_�Ϸù�ȣ"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_�������м���"), _T("BP_��������"));
	pMap->SetAt(_T("UDF_�Է±��м���"), _T("BP_�Է±���"));
	pMap->SetAt(_T("UDF_����ó��ȣ����"), _T("BP_����ó��ȣ"));
	
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}

VOID CBO_ACQ_PurchaseDataMgr::ChangeGroupInfo()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CDivisionNo_Select_Dlg dlg(this);
	dlg.SetWorkStatus(_T("���Դ��"));
	if(m_pInfo->GetKIIUPClientVersion() == 'L') dlg.SetAllowNULLGroup(TRUE);
	if(IDCANCEL == dlg.DoModal()) return ;
	
	CGroupInfo * pTargetGroup = dlg.GetSelGroup();

	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GROUP"), (DWORD*)pTargetGroup);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_PURCHASE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	ids = api.SPFExecutePath(_T("�׷���������"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("���������� ������ �� �����ϴ�."));
		return ;
	}
	ESLAfxMessageBox(_T("�����Ͻ� �ڷῡ ���Ͽ� ���������� �����Ͽ����ϴ�."));
	
	if(pTargetGroup && !m_pSearchDlg->GetGroupCollection()->IsHaveGroupInfo(pTargetGroup))
		m_pSearchDlg->GetGroupCollection()->AddGroupInfo(pTargetGroup);
	SetPurchaseDataInfo();
	m_pGrid->SetProtectSelectColumn(TRUE);
	DisplayGrid();
	m_pGrid->SetProtectSelectColumn(FALSE);
}
