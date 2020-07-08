// BO_ACQ_RecycleDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_RecycleDataMgr.h"
#include "../../../����/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
#include "../../../����/BO_ACQ_API/BO_ACQ_Business_Api.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/DivisionNo_Select_Dlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/RequisitionNo_Select_Dlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/Group_Select_Dlg.h"
#include "../../../����/BO_SPECIES_API/GroupInfo.h"
#include "../../../����/BO_DATA_CHANGE_MANAGER/CodeChange.h"
#include "../../../����/BO_COMMON/BO_Species_Detail_Viewer.h"
#include "SBL.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_RecycleDataMgr dialog


CBO_ACQ_RecycleDataMgr::CBO_ACQ_RecycleDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_RecycleDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_RecycleDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pPrintDM = NULL;
	m_pPrintObjDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pSearchDlg = NULL;
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;
	m_nRow = -1;
}

CBO_ACQ_RecycleDataMgr::~CBO_ACQ_RecycleDataMgr()
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
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}

}

VOID CBO_ACQ_RecycleDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_RecycleDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_RecycleDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_RecycleDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_BN_CLICKED(IDC_btnALLDELETE, OnbtnALLDELETE)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_RecycleDataMgr message handlers

VOID CBO_ACQ_RecycleDataMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	DestroyWindow();
}

BOOL CBO_ACQ_RecycleDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_��Ȱ���ڷ����")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_��Ȱ���ڷ����"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pDM = FindDM(_T("DM_��Ȱ���ڷ����"));
	m_pPrintDM = FindDM(_T("DM_��Ȱ���ڷ����_��"));
	m_pPrintObjDM = FindDM(_T("DM_��Ȱ���ڷ����_��¿�"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("��Ȱ���ڷ����");
	}
	
	CButton *pBtn = (CButton *)GetDlgItem(IDC_btnALLDELETE);
	pBtn->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtRECYC_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcRECYC_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcRECYC_GROUPNAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtRECYC_GROUPNAME)->ShowWindow(SW_HIDE);

	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_RECYCLE, ACQ_TYPE_INVALID, BO_RECYCLE, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_RECYCLE, ACQ_TYPE_INVALID, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	InitSortItem();
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	CString strMsg = _T("��Ȱ�� �ڷḦ �����մϴ�.");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	SetRecycleDataInfo();
	InitPopupMenuItem();

	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_RecycleDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CRect rect;
	CButton *pBtn = (CButton*)GetDlgItem(IDC_stcRECYC_GROUP_CREATE_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtRECYC_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(100, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcRECYC_GROUPNAME);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(185, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtRECYC_GROUPNAME);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(240, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	pBtn = (CButton *)GetDlgItem(IDC_btnALLDELETE);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(410, 10, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridRECYCLE_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	
	pGrid->MoveWindow(-2, 35, cx+2, cy-33);		
}

BOOL CBO_ACQ_RecycleDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_RecycleDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CBO_ACQ_RecycleDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();

	m_pPrintDlg->PrivacyPrintLogYN = TRUE;
}

VOID CBO_ACQ_RecycleDataMgr::ShowInfoDlg(INT nOpenMode)
{
	m_pGrid->SelectMakeList();
	if(nOpenMode == 2000)
	{
		if( m_pGrid->SelectGetCount() > 0 )
		{
			m_pGrid->SetAt( m_pGrid->GetIdx() , 0 , _T("V") );
			m_pGrid->SelectMakeList();
		}
	}
	
	CBO_Species_Detail_Viewer viewer(this);
	viewer.SetOpenMode(nOpenMode);
	viewer.SetParentDM(m_pPrintDM);
	viewer.SetParentGrid(m_pGrid);
	viewer.DoModal();
}

VOID CBO_ACQ_RecycleDataMgr::DeleteRecycleData()
{
	ShowDataProcessingDlg();
	
	CRecycleMgr pMgr(this);
	pMgr.InitTempData();	pMgr.InitTempPtr();
	pMgr.SetTempData(_T("CMAlias"), m_pCM->CM_ALIAS.GetBuffer(0));
	pMgr.SetTempData(_T("DMAlias"), m_pPrintDM->DB_MGR_ALIAS.GetBuffer(0));
	pMgr.SetTempData(_T("BookDMAlias"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
	pMgr.SetTempData(_T("GridAlias"), _T("MainGrid"));
	pMgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg);
	INT ids = pMgr.SPFExecutePath(_T("��Ȱ�����"));
	DeleteDataProcessingDlg();
	CString errstr;
	if(ids < 0)
	{
		switch(ids)
		{
		case -1000:
			errstr = _T("�������� �ڷ��� ���Ÿ� ���� DM�� ã�� �� �����ϴ�. ");
			break;
		case -1001:
			errstr = _T("������ �ڷᰡ �����ϴ�.");
			break;
		case -1003:
			errstr = _T("���� ������ ������ �� �����ϴ�.");
			break;
		case -1004:
			errstr = _T("DM���� ���� �ο츦 ������ �� �����ϴ�.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}
	else if(ids == 0)
		ESLAfxMessageBox(_T("������ �ڷ���� ����ó���� �Ϸ��Ͽ����ϴ�."));
	else
		return;
	DisplayGrid();
	SetRecycleDataInfo();
}

VOID CBO_ACQ_RecycleDataMgr::PurchaseRecycleData()
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
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_RECYCLE);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BOOKDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GroupInfo"), (DWORD*)groupInfo);
	ids = api.SPFExecutePath(_T("����ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �� �� �����ϴ�."));
		return;
	}
	ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �Ͽ����ϴ�."));
	SetRecycleDataInfo();
	DisplayGrid();
}

VOID CBO_ACQ_RecycleDataMgr::DonateRecycleData()
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
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_RECYCLE);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BOOKDM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GroupInfo"), (DWORD*)groupInfo);
	ids = api.SPFExecutePath(_T("����ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �� �� �����ϴ�."));
		return;
	}
	ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �Ͽ����ϴ�."));

	DisplayGrid();
	SetRecycleDataInfo();
}

VOID CBO_ACQ_RecycleDataMgr::RestoreRecycleData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_RECYCLE);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BOOKDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GroupInfo"), (DWORD*)NULL);
	ids = api.SPFExecutePath(_T("�ڷẹ��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ���� �� �� �����ϴ�."));
		return;
	}

	DisplayGrid();
	SetRecycleDataInfo();
}

UINT CBO_ACQ_RecycleDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	BEGIN_SBL();

	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("��Ȱ���ڷḦ �������Դϴ�."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		SetRecycleDataInfo();
		DisplayGrid();
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return 0;
	}
	
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	INT ids = mgr.SPFExecutePath(_T("åTo��"));
	DeleteDataProcessingDlg();
	if(ids < 0) return ids;
	m_pSortItemSet->DataSort(m_pPrintDM, !m_pSearchDlg->GetDataSortYN());
	SetRecycleDataInfo();
	DisplayGrid();

	INT nRowCount = m_pPrintDM->GetRowCount();
	END_SBL(nRowCount);
	
	return 0;
}

UINT CBO_ACQ_RecycleDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	if(lParam == BO_ACQ_RECYCLE_DATA)
	{
		CString strDivAlias = _T("IBS_��KEY");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			INT ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
				return -1;
			}
		}

		ShowDataProcessingDlg();
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("����ƮŸ��"), lParam);
		PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("IBS_��KEY"));
		PrintMgr.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
		PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_������"));
		PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T(""));
		PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
		PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pDM);
		PrintMgr.SetTempPtr(_T("�������DM"), (DWORD*)m_pPrintDM);
		PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pPrintObjDM);
		PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
		INT ids = PrintMgr.SPFExecutePath(_T("�Ϲݸ���Ʈ���"));
		DeleteDataProcessingDlg();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
			return -1;
		}
		m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
		m_pPrintDlg->Print();
	}
	return 0;
}

INT CBO_ACQ_RecycleDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		CString strTmpData = m_pPrintDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return 0;
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

	if(strDMFieldAlias == _T("UDF_�ڷ���¼���"))
	{
		CString strWorkingStatus = m_pPrintDM->GetCellData(_T("IBS_�ڷ����"), nRow);
		if(strWorkingStatus.GetLength() == 0) str = _T("�����ڷ�");
		else 
		{
			strWorkingStatus = strWorkingStatus.Left(3);
			strWorkingStatus.MakeUpper();
			if(strWorkingStatus == _T("BOA")) str = _T("�����ڷ�");
			else if(strWorkingStatus == _T("BOR")) str = _T("����ڷ�");
			else if(strWorkingStatus == _T("BOC")) str = _T("�����ڷ�");
			else if(strWorkingStatus == _T("BOL")) str = _T("�����ڷ�");
		}
		m_pPrintDM->SetCellData(_T("UDF_�ڷ���¼���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("BS_��������"))
	{
		CString strTmpData;
		strTmpData = m_pPrintDM->GetCellData(_T("IBS_�з�ǥ����"), nRow);
		
		if		( strTmpData == _T("1") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC��������ȣ����"), str, str);
		}
		else if	( strTmpData == _T("2") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC��������ȣ����"), str, str);
		}
		else if	( strTmpData == _T("3") || strTmpData == _T("7") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("LC������ȣ"), str, str);
		}
		else
		{
			str = _T("");
		}

		m_pPrintDM->SetCellData(_T("UDF_�������м���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_������"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(_T("UDF_��ȯ����"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_�ܰ�"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CBO_ACQ_RecycleDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

VOID CBO_ACQ_RecycleDataMgr::OnbtnALLDELETE() 
{
	// TODO: Add your control notification handler code here
	ShowDataProcessingDlg();
	CRecycleMgr pMgr(this);
	pMgr.InitTempData();	pMgr.InitTempPtr();
	pMgr.SetTempPtr(_T("SOURCE_DM"), (DWORD*)m_pPrintDM);
	pMgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg);
	INT ids = pMgr.SPFExecutePath(_T("��Ȱ���ϰ�����"));
	DeleteDataProcessingDlg();
	CString errstr;
	if(ids < 0)
	{
		switch(ids)
		{
		case -1000:
			errstr = _T("������ ���� DM�� ã�� �� �����ϴ�.");
			break;
		case -1001:
			errstr = _T("������ ���� DM�� �籸���� �� �����ϴ�.");
			break;
		case -1002:
			errstr = _T("���� ������ ������ �� �����ϴ�.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}
	else if( ids == 0)
		ESLAfxMessageBox(_T("��� ��Ȱ�� �ڷḦ ���� �Ͽ����ϴ�."));	
	else 
		return;
	m_pPrintDM->FreeData();
	m_pDM->FreeData();
	DisplayGrid();
	SetRecycleDataInfo();	
}

VOID CBO_ACQ_RecycleDataMgr::RecycleDataImport(BOOL isPurchase) 
{
	CRecycleMgr pMgr(this);
	pMgr.InitTempData();
	pMgr.SetTempData(_T("CMAlias"), _T("CM_��Ȱ��"));
	pMgr.SetTempData(_T("DMAlias"), _T("DM_��Ȱ��_��"));
	pMgr.SetTempData(_T("GridAlias"), _T("MainGrid"));
	INT ids;
	if(isPurchase)
		ids = pMgr.SPFExecutePath(_T("��Ȱ�뱸��ó��"));
	else
		ids = pMgr.SPFExecutePath(_T("��Ȱ�����ó��"));

	CString errstr ;
	if(ids < 0)
	{
		switch(ids)
		{
		case -1000:
			errstr = _T("����ó���� ���� DM�� ã�� �� �����ϴ�.");
			break;
		case -1001:
			errstr = _T("������ �ڷᰡ �����ϴ�.");
			break;
		case -1002:
			errstr = _T("�����ڷḦ ����ó���� �� �����ϴ�.");
			break;
		case -1003:
			errstr = _T("����ó���Ǿ��� �ڷḦ ������ �� �����ϴ�.");
			break;
		case -1004:
			errstr = _T("����ó���� �ڷ��� DM�ο츦 ������ �� �����ϴ�.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}
	else if (ids == 0)
		ESLAfxMessageBox(_T("�����Ͻ� �ڷḦ ����ó�� �Ͽ����ϴ�."));
	
	SetRecycleDataInfo();
}

VOID CBO_ACQ_RecycleDataMgr::SetRecycleDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d�� %då�� ��Ȱ���ڷᰡ �˻��Ǿ����ϴ�."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));

	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("�׷��̸�"), strMsg);
	m_pCM->SetControlMgrData(_T("�׷�����⵵"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

INT CBO_ACQ_RecycleDataMgr::CheckSelectDataValid()
{
	if(m_pPrintDM->GetRowCount() == 0) return -1;

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CBO_ACQ_RecycleDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

VOID CBO_ACQ_RecycleDataMgr::ShowChangeCodeDlg()
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
	dlg.SetAcqType(0);
	dlg.SetSubjectCodeEnable(FALSE);
	dlg.SetDeleteData(TRUE);
	if(IDOK == dlg.DoModal())
	{
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_RecycleDataMgr::ShowGroupMgrDlg()
{
	CGroup_Mgr mgr(this);
	mgr.SetOpenMode(1);
	mgr.DoModal();
}

VOID CBO_ACQ_RecycleDataMgr::ChangeGroup()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	CGroupInfo TargetGroup;
	CGroup_Select_Dlg dlg(this);
	dlg.SetAcqYear(m_pSearchDlg->GetGroupCollection()->GetAcqYear());
	dlg.SetSpeciesType(BO_RECYCLE);

	if(IDOK == dlg.DoModal())
	{
		TargetGroup.SetAcqYear(dlg.GetSelGroup()->GetAcqYear());
		TargetGroup.SetGroupInfo(dlg.GetSelGroup()->GetGroupInfo());
		TargetGroup.SetGroupNo(dlg.GetSelGroup()->GetGroupNo());
	}
	else return ;
	
	CString strQuery, strResult;
	strQuery.Format(_T("SELECT GROUP_LAST_SERIAL_NO FROM BO_ACQ_GROUP_TBL WHERE CREATE_YEAR='%s' AND GROUP_SEQ=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),
		TargetGroup.GetAcqYear(), TargetGroup.GetGroupNo());
	m_pPrintDM->GetOneValueQuery(strQuery, strResult);
	INT nLastSerialNo = _ttoi(strResult.GetBuffer(0));
	TargetGroup.SetLastSerialNo(nLastSerialNo);

	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GROUP"), (DWORD*)&TargetGroup);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_RECYCLE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)ACQ_TYPE_INVALID);
	ids = api.SPFExecutePath(_T("�׷���������"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�׷������� ������ �� �����ϴ�."));

		return ;
	}
	ESLAfxMessageBox(_T("�׷������� �����Ͽ����ϴ�."));

	m_pSearchDlg->GetGroupCollection()->AddGroupInfo(&TargetGroup);
	SetRecycleDataInfo();
	m_pGrid->SetProtectSelectColumn(TRUE);
	DisplayGrid();
	m_pGrid->SetProtectSelectColumn(FALSE);
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_RecycleDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_RecycleDataMgr)
	ON_EVENT(CBO_ACQ_RecycleDataMgr, IDC_gridRECYCLE_DATA, -601 /* DblClick */, OnDblClickgridRECYCLEDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_RecycleDataMgr::OnDblClickgridRECYCLEDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0 ) return ;

	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return;
	}
	ShowInfoDlg(2000);
}

INT CBO_ACQ_RecycleDataMgr::GetBookCount()
{
	INT nRowCnt = m_pDM->GetRowCount();
	INT nBookCnt = 0;
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = m_pDM->GetCellData(_T("BP_�ѱǼ�"), i);
		nBookCnt += _ttoi(strTmpData.GetBuffer(0));
	}
	return nBookCnt;
}

VOID CBO_ACQ_RecycleDataMgr::ShowDataProcessingDlg()
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
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CBO_ACQ_RecycleDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

BOOL CBO_ACQ_RecycleDataMgr::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("UDF_�׷��"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_�ڷ���¼���"), _T("IBS_�ڷ����"));
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}

VOID CBO_ACQ_RecycleDataMgr::ShowSortDlg(INT nType/*=1*/)
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

UINT CBO_ACQ_RecycleDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

HBRUSH CBO_ACQ_RecycleDataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}