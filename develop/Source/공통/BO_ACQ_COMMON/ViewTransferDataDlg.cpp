// ViewTransferDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ViewTransferDataDlg.h"
#include "../BO_ACQ_GROUP_MANAGER/GroupCollection.h"
#include "../BO_ACQ_GROUP_MANAGER/DivisionNo_List_Dlg.h"
#include "../BO_ACQ_PRINT_MGR/BasicPrintMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTransferDataDlg dialog


CViewTransferDataDlg::CViewTransferDataDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CViewTransferDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewTransferDataDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pSortItemSet = NULL;
	m_nRow = -1;
}

CViewTransferDataDlg::~CViewTransferDataDlg()
{
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
}

VOID CViewTransferDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewTransferDataDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewTransferDataDlg, CDialog)
	//{{AFX_MSG_MAP(CViewTransferDataDlg)
	ON_BN_CLICKED(IDC_btnTVIEW_DIVNO, OnbtnTVIEWDIVNO)
	ON_BN_CLICKED(IDC_btnTVIEW_SEARCH, OnbtnTVIEWSEARCH)
	ON_BN_CLICKED(IDC_btnTVIEW_PRINT, OnbtnTVIEWPRINT)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnTVIEW_SORT, OnbtnTVIEWSORT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTransferDataDlg message handlers

BOOL CViewTransferDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_�˼��Ϸ��ڷẸ��")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	GetWindowRect(&m_rcInitial);
	m_pCM = FindCM(_T("CM_�˼��Ϸ��ڷẸ��"));
	m_pDM = FindDM(_T("DM_�˼��Ϸ��ڷẸ��"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	
	InitSortItem();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CViewTransferDataDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode;
	INT nGroupNo, nSerialNo, nWorkNo;
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
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_����ó��ȣ����"))
	{
		strTmpData = m_pDM->GetCellData(_T("BP_����ó��ȣ"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����ó����"), strTmpData, str);
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_��������"))
	{
		strTmpData = m_pDM->GetCellData(_T("IBS_�з�ǥ����"), nRow);
		
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

		m_pDM->SetCellData(_T("UDF_�������м���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_����"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pDM->SetCellData(_T("UDF_��ȯ����"), str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_���ְ���"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pDM->SetCellData(_T("UDF_��ȯ���ΰ���"), str, nRow);
	}
	return 0;
}

VOID CViewTransferDataDlg::OnbtnTVIEWDIVNO() 
{
	// TODO: Add your control notification handler code here
	this->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	CGroupCollection GroupCollection(this);
	GroupCollection.SetAcqType(PURCHASE);
	CString strAcqYear, strSelDivNo, strWorkCode;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);
	CDivisionNo_List_Dlg dlg(this);
	dlg.SetAcquisitYear(strAcqYear);
	dlg.SetWorkStatus(_T("�˼��Ϸ�"));
	dlg.SetWorkCode(strWorkCode);
	dlg.SetGroupCollection(&GroupCollection);
	if(IDOK == dlg.DoModal())
	{
		strSelDivNo = dlg.GetSelDivNo();
		m_pCM->SetControlMgrData(_T("��������"), strSelDivNo);
		m_pCM->SetControlMgrData(_T("���Գ⵵"), GroupCollection.GetAcqYear());
		m_pCM->SetControlMgrData(_T("��������"), GroupCollection.GetWorkCode());
		OnbtnTVIEWSEARCH();
	}
}

VOID CViewTransferDataDlg::OnbtnTVIEWSEARCH() 
{
	CString strSelGroup, strAcqYear, strQuery, strWorkCode;
	m_pCM->GetControlMgrData(_T("��������"), strSelGroup);
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);
	if (strSelGroup.IsEmpty())
	{
		ESLAfxMessageBox(_T("���������� �Է��Ͽ� �ֽʽÿ�.")); 
		return;
	}

	if(strWorkCode.IsEmpty())
	{
		strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND I.SPECIES_CLASS IN ('0', '1') AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO IN (%s) AND I.WORK_CODE IS NULL AND I.WORKING_STATUS = 'BOR111N'"),
			strAcqYear, strSelGroup);
	}
	else
	{
		strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND I.SPECIES_CLASS IN ('0', '1') AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO IN (%s) AND I.WORK_CODE = '%s' AND I.WORKING_STATUS = 'BOR111N'"),
			strAcqYear, strSelGroup, strWorkCode);
	}

	m_pDM->RefreshDataManager(strQuery);
	SortByGroupInfo();
	m_pGrid->Display();
}

VOID CViewTransferDataDlg::OnbtnTVIEWPRINT() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strGroupInfo;
	CBasicPrintMgr PrintMgr(this);
	PrintMgr.InitPrintData();
	m_pCM->GetControlMgrData(_T("��������"), strGroupInfo);
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
		
	PrintMgr.AddPrintData(_T("������ȣ"), strGroupInfo);
	PrintMgr.AddPrintData(_T("���Գ⵵"), strAcqYear);
	PrintMgr.SetPrintDM(m_pDM);
	PrintMgr.SetPrintType(0);
	PrintMgr.SetSIReportFilename(_T("K2UP_�˼��Ϸ��ڷ���_����"));
	PrintMgr.Print();
}

BOOL CViewTransferDataDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDC_btnTVIEW_SEARCH, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

INT CViewTransferDataDlg::SortByGroupInfo()
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
VOID CViewTransferDataDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		
		MoveWindow(rcWnd);
		return;
	}
	
	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			
		
		MoveWindow(rcWnd);
		return;
	}

	CRect rect;
	GetClientRect(rect);
	rect.top += 54;
	rect.bottom -= 4;
	rect.left += 4;
	rect.right -= 4;
	
	m_pGrid->MoveWindow(rect);		
}

VOID CViewTransferDataDlg::OnbtnTVIEWSORT() 
{
	// TODO: Add your control notification handler code here
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
	if(IDOK == dlg.DoModal() && 1 > 0)
		m_pGrid->Display();	
}

BOOL CViewTransferDataDlg::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BP_���Գ⵵"));
	pTmpData->Add(_T("IBS_��������"));
	pTmpData->Add(_T("BP_�׷��ȣ"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("BP_�����Ϸù�ȣ"));
	pTmpData->Add(_T("BB_���Ϸù�ȣ"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_�������м���"), _T("BP_��������"));
	pMap->SetAt(_T("UDF_�Է±��м���"), _T("BP_�Է±���"));
	pMap->SetAt(_T("UDF_����ó��ȣ����"), _T("BP_����ó��ȣ"));
	
	m_pSortItemSet->ReadSortItem();
	return TRUE;
}

HBRUSH CViewTransferDataDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
