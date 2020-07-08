// BO_ACQ_RequestDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_RequestDataMgr.h"
#include "Request_PrintMgr.h"
#include "Request_DataMgr.h"
#include "../../../����/BO_ACQ_API/BO_ACQ_Business_Api.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/DivisionNo_Select_Dlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/RequisitionNo_Select_Dlg.h"
#include "../../../����/BO_SPECIES_API/GroupInfo.h"
#include "../../../����/BO_ACQ_COMMON/RequestMailMgr.h"
#include "../../../����/Duplicate_Data_Search/DUP_KEY_SETTING.h"
#include "../../../����/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_RequestDataMgr dialog


CBO_ACQ_RequestDataMgr::CBO_ACQ_RequestDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_RequestDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_RequestDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pSearchDlg = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;
}

CBO_ACQ_RequestDataMgr::~CBO_ACQ_RequestDataMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
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
	if(m_pDuplicateSearchMgr)
	{
		delete m_pDuplicateSearchMgr;
		m_pDuplicateSearchMgr = NULL;
	}
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
}

VOID CBO_ACQ_RequestDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_RequestDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_RequestDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_RequestDataMgr)
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
// CBO_ACQ_RequestDataMgr message handlers

VOID CBO_ACQ_RequestDataMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	DestroyWindow();
}

BOOL CBO_ACQ_RequestDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_��ġ�������")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_��ġ����ڷ����"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pDM = FindDM(_T("DM_��ġ����ڷ����"));
	m_pPrintDM = FindDM(_T("DM_��ġ����ڷ����_��¿�"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_REQUEST, ACQ_TYPE_INVALID, BO_REQUEST, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_REQUEST, ACQ_TYPE_INVALID, this);
	InitSortItem();
	
	CString strMsg = _T("��ġ��� �ڷḦ �����մϴ�.");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	InitPopupMenuItem();

	if(m_pInfo->GetSMSUseMode() < 1)
	{
		CRequestManager mgr(this);
		mgr.SetUDFCheck(m_pDM, _T("SMS���ſ���"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_RequestDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridREQUEST_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(5, 5, cx-10, cy-10);	
}

BOOL CBO_ACQ_RequestDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_RequestDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();
}

VOID CBO_ACQ_RequestDataMgr::ShowRequestDataDlg(OPERATION_TYPE nOpType)
{
	if(nOpType != SINSERT)
	{
		if(m_pDM->GetRowCount() == 0)
		{
			ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
			return ;
		}
	}
	INT nRowCnt = m_pDM->GetRowCount();
	CRequest_DataMgr dlg(this);
	dlg.SetOpType(nOpType);
	dlg.DoModal();
	SetRequestDataInfo();
	if(nOpType == SINSERT)
	{
		INT nCurCnt = m_pDM->GetRowCount();
		if(nCurCnt > nRowCnt)
		{
			m_pGrid->DisplayNumber();
			m_pGrid->SetTopRow(nRowCnt+1);
		}
	}
}

VOID CBO_ACQ_RequestDataMgr::DeleteRequestData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	if(IDCANCEL == ESLAfxMessageBox(_T("�����Ͻ� �ڷḦ �����Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		return;
	CRequestManager pMgr(this);
	pMgr.InitTempData();
	pMgr.SetTempData(_T("DMAlias"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
	pMgr.SetTempData(_T("CMAlias"), m_pCM->CM_ALIAS.GetBuffer(0));

	ids = pMgr.SPFExecutePath(_T("��ġ�������"));
	CString errstr;
	if(ids < 0)
	{
		switch(ids)
		{
		case -1000:
			errstr = _T("��ġ����ڷ� ������ ���� DM�� ã�� �� �����ϴ�.");
			break;
		case -1001:
			errstr = _T("���� ����� ������ �� �����ϴ�.");
			break;
		case -1002:
			errstr = _T("������ �ڷḦ DB���� �����ϴµ� ������ �߻��Ͽ����ϴ�.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}
	errstr = _T("������ ��ġ����ڷḦ ���� �Ͽ����ϴ�.");
	ESLAfxMessageBox(errstr.GetBuffer(0));
					
	DisplayGrid();
	SetRequestDataInfo();
}

VOID CBO_ACQ_RequestDataMgr::PurchaseRequestData()
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
	
	CGroupInfo * groupInfo = dlg.GetSelGroup();
	CStringArray ArrayFurnishKey;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_REQUEST);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("BOOKDM"), (DWORD*)NULL);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GroupInfo"), (DWORD*)groupInfo);
	api.SetTempPtr(_T("FurnishKey"), (DWORD*)&ArrayFurnishKey);
	ids = api.SPFExecutePath(_T("����ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �� �� �����ϴ�."));
		return;
	}
	ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �Ͽ����ϴ�."));

	CRequestMailMgr mailMgr(this);
	mailMgr.SetOpenCode(1000);
	mailMgr.SetParentDM(m_pDM);
	mailMgr.SetFurnishKeyList(&ArrayFurnishKey);
	mailMgr.DoModal();

	DisplayGrid();
}

VOID CBO_ACQ_RequestDataMgr::DonateRequestData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	CRequisitionNo_Select_Dlg dlg(this);
	if(IDCANCEL == dlg.DoModal()) return ;
	
	CGroupInfo * groupInfo = dlg.GetSelGroup();
	CStringArray ArrayFurnishKey;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_REQUEST);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GroupInfo"), (DWORD*)groupInfo);
	api.SetTempPtr(_T("FurnishKey"), (DWORD*)&ArrayFurnishKey);
	ids = api.SPFExecutePath(_T("����ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �� �� �����ϴ�."));
		return;
	}
	ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �Ͽ����ϴ�."));

	CRequestMailMgr mailMgr(this);
	mailMgr.SetOpenCode(1000);
	mailMgr.SetParentDM(m_pDM);
	mailMgr.SetFurnishKeyList(&ArrayFurnishKey);
	mailMgr.DoModal();

	DisplayGrid();
}

VOID CBO_ACQ_RequestDataMgr::ShowRequestStatusDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	CRequest_StatusMgr dlg(this);
	if(IDOK == dlg.DoModal())
		DisplayGrid();
}

VOID CBO_ACQ_RequestDataMgr::ShowRequestMailMgrDlg()
{
	CRequestMailMgr mgr(this);
	mgr.SetOpenCode(-1000);
	mgr.SetParentDM(m_pDM);
	mgr.SetParentGrid(m_pGrid);
	mgr.DoModal();
	DisplayGrid();
}

UINT CBO_ACQ_RequestDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strMsg;
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("��ġ����ڷḦ �������Դϴ�."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		SetRequestDataInfo();
		DisplayGrid();
		return 0;
	}
	SetRequestDataInfo();
	m_pSortItemSet->DataSort(m_pDM, !m_pSearchDlg->GetDataSortYN());
	DeleteDataProcessingDlg();
	DisplayGrid();
	
	return 0;
}

VOID CBO_ACQ_RequestDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

INT CBO_ACQ_RequestDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(strDMFieldAlias == _T("��ġ����"))
	{
		if(str.GetLength() == 0) str = _T("�������");
		else if(str == _T("1")) str = _T("��û��");
		else if(str == _T("2")) str = _T("ó����");
		else if(str == _T("3")) str = _T("������");
		else if(str == _T("4")) str = _T("��ҵ�");
		m_pDM->SetCellData(_T("��ġ���¼���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("����"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
	}
	else if(strDMFieldAlias == _T("�ֹε�Ϲ�ȣ"))
	{
		str.TrimLeft();	str.TrimRight();
		CString strLowNum = str.Right(7);
		str.Replace(strLowNum, _T("*******"));
	}
	return 0;
}

VOID CBO_ACQ_RequestDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_RequestDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_RequestDataMgr)
	ON_EVENT(CBO_ACQ_RequestDataMgr, IDC_gridREQUEST_DATA, -601 /* DblClick */, OnDblClickgridREQUESTDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_RequestDataMgr::OnDblClickgridREQUESTDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return;
	if(m_pDM->GetRowCount() == 0) return ;
	ShowRequestDataDlg(GUPDATE);
}

VOID CBO_ACQ_RequestDataMgr::SetRequestDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
		strMsg.Format(_T("�˻��� ��ġ����ڷᰡ �����ϴ�."));
	else
		strMsg.Format(_T("%d���� ��ġ����ڷᰡ �˻��Ǿ����ϴ�."), nRowCnt);
	
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
}

INT CBO_ACQ_RequestDataMgr::CheckSelectDataValid()
{
	if(m_pDM->GetRowCount() == 0) return -1;

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CBO_ACQ_RequestDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

VOID CBO_ACQ_RequestDataMgr::DupRequestDataSearch()
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
	m_pDuplicateSearchMgr->ShowHoldDuplicateSearch(_T("��ġ����ڷẹ������"), m_pDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"));
}

UINT CBO_ACQ_RequestDataMgr::OnShowSearchResult(LPARAM lParam, WPARAM wParam)
{
	CString strTmpData;
	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();
	return 0;
}

UINT CBO_ACQ_RequestDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	if(lParam == BO_ACQ_REQUEST_DATA)
	{
		CESL_DataMgr * pDM = m_pDM;
		CString strDivAlias = _T("��ġ���KEY");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			INT ids = PrintMgr.GetGridSelectData(m_pGrid, m_pDM, strDivAlias, &MapDivValue);
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
				return -1;
			}
			pDM = m_pPrintDM;
		}
		ShowDataProcessingDlg();
		
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("����ƮŸ��"), lParam);
		PrintMgr.SetTempData(_T("���о˸��ƽ�"), strDivAlias.GetBuffer(0));
		PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
		PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("����DM"), (DWORD*)m_pDM);
		PrintMgr.SetTempPtr(_T("�������DM"), (DWORD*)m_pPrintDM);
		PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)NULL);
		PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
		INT ids = PrintMgr.SPFExecutePath(_T("��ġ����ڷ����"));
		DeleteDataProcessingDlg();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
			return -1;
		}
		m_pPrintDlg->SetPrintDM(pDM);
		m_pPrintDlg->Print();
/*

		CMakePrintDMMgr PrintMgr(this);
		
		PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pDM);
		INT ids = PrintMgr.SPFExecutePath(_T("�Ϸù�ȣ����"));
		if(ids < 0) 
		{
			ESLAfxMessageBox(_T("��µ����Ϳ� ������ �ֽ��ϴ�."));
			return -1;
		}
		m_pPrintDlg->SetPrintDM(m_pDM);
		m_pPrintDlg->Print();
*/
	}
	else if(lParam == BO_ACQ_REQUEST_MAIL_SEND_LIST)
	{
		CRequest_MailSendList dlg(this);
		dlg.SetPrintDlg(m_pPrintDlg);
		dlg.DoModal();
	}
	return 0;
}

VOID CBO_ACQ_RequestDataMgr::DupKeySetting()
{
	CDUP_KEY_SETTING Dlg(this, _T(".\\duplicate_search_key.txt"));
	Dlg.DoModal();
}

VOID CBO_ACQ_RequestDataMgr::ShowDataProcessingDlg()
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

VOID CBO_ACQ_RequestDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

UINT CBO_ACQ_RequestDataMgr::OnDuplicateDataDelete(LPARAM lParam, WPARAM wParam)
{
	CString strMsg;
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("��ġ����ڷḦ �������Դϴ�."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		SetRequestDataInfo();
		DisplayGrid();
		return 0;
	}
	SetRequestDataInfo();
	DeleteDataProcessingDlg();
	DisplayGrid();
	
	return 0;
}

BOOL CBO_ACQ_RequestDataMgr::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("��ġ���¼���"), _T("��ġ����"));
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}

VOID CBO_ACQ_RequestDataMgr::ShowSortDlg(INT nType/*=1*/)
{
	if(m_pSortItemSet == NULL || m_pDM == NULL) 
	{
		ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		return;
	}
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(m_pDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(nType);
	if(IDOK == dlg.DoModal() && nType > 0)
		DisplayGrid();
}

UINT CBO_ACQ_RequestDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}
