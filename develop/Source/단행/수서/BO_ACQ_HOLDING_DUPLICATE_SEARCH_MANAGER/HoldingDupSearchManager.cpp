// HoldingDupSearchManager.cpp : implementation file
//

#include "stdafx.h"
#include "HoldingDupSearchManager.h"
#include "HoldDupSearchSelect.h"
#include "..\..\..\����\Duplicate_Data_Search\DUP_KEY_SETTING.h"
#include "..\..\..\����\BO_ACQ_API\BO_ACQ_Business_Api.h"
#include "../../../esl/kolas3_main/kolas3_mainapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoldingDupSearchManager dialog


CHoldingDupSearchManager::CHoldingDupSearchManager(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CHoldingDupSearchManager::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHoldingDupSearchManager)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pSearchDlg = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;
	m_nRow = -1;
}

CHoldingDupSearchManager::~CHoldingDupSearchManager()
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

VOID CHoldingDupSearchManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHoldingDupSearchManager)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHoldingDupSearchManager, CDialog)
	//{{AFX_MSG_MAP(CHoldingDupSearchManager)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnShowSearchResult)
	ON_MESSAGE(DUPLICATE_DATA_DELETE, OnDuplicateDataDelete)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHoldingDupSearchManager message handlers

BOOL CHoldingDupSearchManager::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_��������")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pDM = FindDM(_T("DM_���Ժ�������"));
	// 2004.11.16 �켱�����ڷ� ��뿩�ο� ���� CM�� �ٸ� �ɷ� �����Ͽ� ����Ѵ�.
	CString strPriorityCatalogYN;
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), strPriorityCatalogYN);
	m_pCM = FindCM(_T("CM_���Ժ�������"));
	if ( m_pCM == NULL ) {
		AfxMessageBox( _T("CM_���Ժ������� error") );
		return -2;
	}
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	// 2005-01-13
	// ���� �ڸ��� �������� �����´�.
	GetManageValue(_T("����"), _T("����"), _T("���������������"), _T(""), m_strPosNumber);

	GetDlgItem(IDC_stcHOLD_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtHOLD_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcHOLD_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtHOLD_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);	
	
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_HOLD_SEARCH, PURCHASE, BO_PURCHASE, m_pDM->CONNECTION_INFO);
	InitSortItem();
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	SetHoldSearchDataInfo();
	InitPopupMenuItem();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CHoldingDupSearchManager::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

VOID CHoldingDupSearchManager::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CButton *pBtn = (CButton *)GetDlgItem(IDC_stcHOLD_DUP_SEARCH);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->MoveWindow(5, 5, cx-10, 30);

	CRect rect;
	pBtn = (CButton*)GetDlgItem(IDC_stcHOLD_GROUP_CREATE_YEAR);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtHOLD_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcHOLD_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(150, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtHOLD_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(205, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridHOLD_DUP_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 40, cx-10, cy-45);		
	pGrid->MoveWindow(-2, 35, cx+2, cy);
}

VOID CHoldingDupSearchManager::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CHoldingDupSearchManager::ShowPrintDlg()
{

}

VOID CHoldingDupSearchManager::DuplicateSearch()
{
	BOOL bGridSelectError = FALSE;
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) 
	{
		bGridSelectError = TRUE;
	}
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		bGridSelectError = TRUE;
	}
	if(bGridSelectError)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	INT cnt = m_pSearchDlg->GetGroupCollection()->GetGroupCount();
	if(cnt > 1)
	{
		CHoldDupSearchSelect dlg(this);
		dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
		dlg.SetParentCM(m_pCM);
		dlg.SetParentDM(m_pDM);
		dlg.DoModal();
		return ;
	}
	else if(cnt == 0)
	{
		ESLAfxMessageBox(_T("������ ���������� �����ϴ�."));
		return;
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

	m_pDuplicateSearchMgr->ShowWorkDuplicateSearchByDivno(_T("������������"),
														m_pDM->DB_MGR_ALIAS,
														m_pCM->CM_ALIAS,
														_T("MainGrid"),
														m_pSearchDlg->GetGroupCollection()->GetCurSelGroupInfo(),
														m_pSearchDlg->GetGroupCollection());
}

VOID CHoldingDupSearchManager::DuplicateSearchComplete()
{
	CString strMsg, strTmpData;
	CStringArray tmpArray;
	m_pSearchDlg->GetGroupCollection()->GetDetailGroupInfo(&tmpArray);
	INT cnt = tmpArray.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strTmpData.IsEmpty()) strTmpData = tmpArray.GetAt(i);
		else strTmpData += _T(", ") + tmpArray.GetAt(i);
	}
	strMsg.Format(_T("��������:%s\n�������� �Ϸ�ó���Ͻðڽ��ϱ�?"), strTmpData);
	if(IDYES == ESLAfxMessageBox(strMsg, MB_YESNO))
	{
		INT ids = m_pSearchDlg->GetGroupCollection()->UpdateDupSearchCompleteYN(TRUE, m_pDM);
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("�������� �Ϸ�ó���� ������ �� �����ϴ�."));
			return ;
		}

		// 2004.12.21 �������� �Ϸ� �� ����Ϸ�Ǿ��ٴ� �޽��� ����ش�.
		ESLAfxMessageBox(_T("�������簡 �Ϸ�Ǿ����ϴ�."));
		return ;
	}
}

VOID CHoldingDupSearchManager::DupKeySetting()
{
	CDUP_KEY_SETTING Dlg(this, _T(".\\duplicate_search_key.txt"));
	Dlg.DoModal();
}

BOOL CHoldingDupSearchManager::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CHoldingDupSearchManager::SetHoldSearchDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pDM->GetRowCount();
	INT nBookCnt = GetBookCount();
	strMsg.Format(_T("%d�� %då�� �������� ��� �ڷᰡ �˻��Ǿ����ϴ�."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), strMsg);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

UINT CHoldingDupSearchManager::OnDuplicateDataDelete(LPARAM lParam, WPARAM wParam)
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("��DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("��DM"), NULL);
	api.SetTempPtr(_T("åDM"), NULL);
	api.SetTempPtr(_T("����������"), (DWORD*)m_pDuplicateSearchMgr->GetAcqKeyArray());
	api.SetTempData(_T("��Alias"), _T("IBS_��KEY"));
	INT ids = api.SPFExecutePath(_T("DM�ڷ����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ DM���� ������ �� �����ϴ�."));
		return 0;
	}
	DisplayGrid();
	return 0;	
}

UINT CHoldingDupSearchManager::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("���Դ���ڷḦ �������Դϴ�."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	if(m_pDM->GetRowCount() == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		DisplayGrid();
		SetHoldSearchDataInfo();
		return 0;
	}
	SetHoldSearchDataInfo();
	m_pSortItemSet->DataSort(m_pDM, !m_pSearchDlg->GetDataSortYN());
	//SortByGroupInfo();
	DeleteDataProcessingDlg();
	DisplayGrid();
	
	return 0;
}

INT CHoldingDupSearchManager::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode;
	INT nGroupNo, nWorkNo, nSerialNo;

	TCHAR* pEndPtr = NULL;

	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = m_pDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("IBS_�ǻ���"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = m_pDM->GetCellData(_T("IBS_��ǥ��"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_ǥ������"), strTmpData, nRow);
		ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_������"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == m_pDM->GetRowCount()-1) m_nRow = -1;
	}
	
	if(strDMFieldAlias == _T("UDF_��������"))
	{	
		strAcqYear = m_pDM->GetCellData(_T("BP_���Գ⵵"), nRow);
		strWorkCode = m_pDM->GetCellData(_T("IBS_��������"), nRow);
		strGroupNo = m_pDM->GetCellData(_T("BP_�׷��ȣ"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		strWorkNo = m_pDM->GetCellData(_T("BP_�۾���ȣ"), nRow);
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		strSerialNo = m_pDM->GetCellData(_T("BP_�Ϸù�ȣ"), nRow);
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_�Է±��м���"))
	{
		strTmpData = m_pDM->GetCellData(_T("BP_�Է±���"), nRow);
		if(strTmpData == _T("0")) str = _T("ȭ���Է�");
		else if(strTmpData == _T("1")) str = _T("��Ȱ�����");
		else if(strTmpData == _T("2")) str = _T("����ڷ����");
		else if(strTmpData == _T("3")) str = _T("���Ϲ���");
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_��������"))
	{
		strTmpData = m_pDM->GetCellData(_T("IBS_�з�ǥ����"), nRow);
		if(strTmpData.IsEmpty()) str = _T("");
		else if(strTmpData == _T("1"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC��������ȣ����"), str, str);
		}
		else if(strTmpData == _T("2"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC��������ȣ����"), str, str);
		}
		m_pDM->SetCellData(_T("UDF_�������м���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_����ó��ȣ����"))
	{
		strTmpData = m_pDM->GetCellData(_T("BP_����ó��ȣ"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����ó����"), strTmpData, str);
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_����"))
	{
		// 2004.09.14  ȭ�󱸺а��� �Ҽ��� 2�ڸ����� ��Ÿ����.
		DOUBLE strTemp;
		//strTemp = (DOUBLE)atof(m_pDM->GetCellData(_T("BP_����"), nRow));
		strTemp = _tcstod(m_pDM->GetCellData(_T("BP_����"), nRow), &pEndPtr);

		str = TruncPrice(m_pDM, strTemp);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pDM->SetCellData(_T("UDF_��ȯ������"), str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_���ְ���"))
	{
		// 2004.09.14  ȭ�󱸺а��� �Ҽ��� 2�ڸ����� ��Ÿ����.
		DOUBLE strTemp;
		//strTemp = (DOUBLE)atof(str);
		strTemp = _tcstod(str.GetBuffer(0), &pEndPtr);

		str = TruncPrice(m_pDM, strTemp);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pDM->SetCellData(_T("UDF_��ȯ����������"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_�ܰ�"))
	{
		// 2004.09.14  ȭ�󱸺а��� �Ҽ��� 2�ڸ����� ��Ÿ����.
		DOUBLE strTemp;
		str.Replace(_T(","), _T(""));
		//strTemp = (DOUBLE)atof(str);
		strTemp = _tcstod(str.GetBuffer(0), &pEndPtr);

		str = TruncPrice(m_pDM, strTemp);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CHoldingDupSearchManager::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

UINT CHoldingDupSearchManager::OnShowSearchResult(LPARAM lParam, WPARAM wParam)
{
	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();
	return 0;
}

VOID CHoldingDupSearchManager::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);		
}

INT CHoldingDupSearchManager::GetBookCount()
{
	INT nRowCnt = m_pDM->GetRowCount();
	INT nBookCnt = 0;
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = m_pDM->GetCellData(_T("BP_����å��"), i);
		nBookCnt += _ttoi(strTmpData.GetBuffer(0));
	}
	return nBookCnt;
}

VOID CHoldingDupSearchManager::ShowDataProcessingDlg()
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

VOID CHoldingDupSearchManager::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

/*
INT CHoldingDupSearchManager::SortByGroupInfo()
{
	if(m_pDM == NULL) return 0;
	if(m_pDM->GetRowCount() == 0) return 0;

	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_���Գ⵵");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�׷��ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�۾���ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�Ϸù�ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return 0;
}
*/

VOID CHoldingDupSearchManager::ShowSortDlg()
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
	dlg.SetOpenMode(1);
	if(IDOK == dlg.DoModal())
		DisplayGrid();
}

BOOL CHoldingDupSearchManager::InitSortItem()
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

CString CHoldingDupSearchManager::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	// 2004.12.01 ������å�� ���� ��������������� �о�ͼ� ������ �ڸ�����ŭ ǥ���ϵ��� �Ѵ�.
	CString strTmpData, strTmp, strValue;
	INT nPos = -1;

	strTmp.Format(_T("%.2f"), dPrice);
	nPos = strTmp.Find(_T("."));

	// 2005-01-13
	if(m_strPosNumber == _T("1"))  
		strTmpData = strTmp.Left(nPos+2);
	else if(m_strPosNumber == _T("2"))  
		strTmpData = strTmp.Left(nPos+3);
	else  // m_strPosNumber == _T("0") || m_strPosNumber.Empty()
		strTmpData = strTmp.Left(nPos);

	return strTmpData;
}

HBRUSH CHoldingDupSearchManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}