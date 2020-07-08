// OrderMailMgr.cpp : implementation file
//

#include "stdafx.h"
#include "OrderMailMgr.h"
#include "../BO_ACQ_GROUP_MANAGER/DivisionNo_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderMailMgr dialog


COrderMailMgr::COrderMailMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(COrderMailMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderMailMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDM_BOOK= NULL;
	m_pDM_VOL= NULL;
	m_pGroupCollection = NULL;
	m_pParentGroupCollection = NULL;
	m_pGroup= NULL;
	m_nOpenMode = 0;
	m_nOperationType = 0;
	m_nOpenType = 0;
	m_pComboMgr = NULL; 
	m_strAcqYear.Empty();
	m_pDataProcessingDlg = NULL;
	m_nRow = -1;

}

COrderMailMgr::~COrderMailMgr()
{
	if(m_pGroupCollection)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = NULL;
	}
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

VOID COrderMailMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderMailMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderMailMgr, CDialog)
	//{{AFX_MSG_MAP(COrderMailMgr)
	ON_BN_CLICKED(IDC_btnOMAIL_DIVNO_VIEW, OnbtnOMAILDIVNOVIEW)
	ON_BN_CLICKED(IDC_btnOMAIL_SENDMAIL, OnbtnOMAILSENDMAIL)
	ON_CBN_SELCHANGE(IDC_cmbOMAIL_DIVNO, OnSelchangecmbOMAILDIVNO)
	ON_CBN_SELCHANGE(IDC_cmbOMAIL_PROVIDER, OnSelchangecmbOMAILPROVIDER)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderMailMgr message handlers

BOOL COrderMailMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_�ֹ����ϰ���")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	GetWindowRect(&m_rcInitial);
	m_pCM = FindCM(_T("CM_�ֹ�����"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pDM_BOOK = FindDM(_T("DM_�ֹ�����_å"));
	m_pDM_VOL = FindDM(_T("DM_�ֹ�����_��"));
	SetCONNECTION_INFO(m_pDM_VOL->CONNECTION_INFO);
	m_pGroupCollection = new CGroupCollection(this);
	m_pComboMgr = new CMulitComboBoxManager(this);
	
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	ShowCtrlByOpenMode();
	ShowCtrlByOperationType();

	if(!Init())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	PostMessage(WM_SIZE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID COrderMailMgr::OnbtnOMAILDIVNOVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strGroupInfo;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	
	CDivisionNo_List_Dlg dlg(this);
	if(m_nOperationType == CANCEL)
		dlg.SetWorkStatus(_T("���Դ��"));
	else if(m_nOperationType == ORDER)
		dlg.SetWorkStatus(_T("�ֹ�"));
	dlg.SetAcquisitYear(strAcqYear);
	dlg.SetGroupCollection(m_pGroupCollection);
	if(IDOK == dlg.DoModal()) 
	{
		strGroupInfo = dlg.GetSelDivNo();
		if(!strGroupInfo.IsEmpty()) 
		{
			m_pGroup = NULL;
			m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
			strGroupInfo = m_pGroup->GetGroupInfo();
			strAcqYear = m_pGroup->GetAcqYear();
		}
		else return ;

		InitOrderMailData();
	}
}
VOID COrderMailMgr::OnbtnOMAILSENDMAIL() 
{
	// TODO: Add your control notification handler code here

	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("����ó"));
	INT nCurSel = pCmb->GetCurSel();

	if (nCurSel < 0) 
	{
		ESLAfxMessageBox(_T("������ �ڷᳪ ������ �������� �ʽ��ϴ�."));
		return;
	}

	CString strProvider = m_arrProvider.GetAt(nCurSel);
	CK2UP_MailMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM_VOL);
	mgr.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	mgr.SetTempData(_T("COPYDM_ALIAS"), _T("DM_�ֹ�����_����"));
	mgr.SetTempData(_T("PROVIDER"), strProvider.GetBuffer(0));
	mgr.SetTempData(_T("OPERATION_TYPE"), m_nOperationType);
	mgr.SetTempPtr(_T("PROVIDER_ARRAY"), (DWORD*)&m_arrProvider);
	INT ids = mgr.SPFExecutePath(_T("�ֹ����Ϲ߼�"));
	if(ids < 0)
	{
		if(m_nOperationType == ORDER)
			ESLAfxMessageBox(_T("�ֹ������� �߼��� �� �����ϴ�."));
		else
			ESLAfxMessageBox(_T("�ֹ���Ҹ����� �߼��� �� �����ϴ�."));
		return;
	}
	else if( ids == 1000)
	{
		ESLAfxMessageBox(_T("���� �߼��� �ùٸ� ���� ���ڵ� ������ �����ϴ�."));
		return ;
	}
}

VOID COrderMailMgr::OnSelchangecmbOMAILDIVNO() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�׷�����"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroupCollection->SetCurSel(nCurSel);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();

	INT ids = MakeDM();
	if(ids < 0) return ;
}

VOID COrderMailMgr::OnSelchangecmbOMAILPROVIDER() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("����ó"));
	INT nCurSel = pCmb->GetCurSel();
	CString strProvider = m_arrProvider.GetAt(nCurSel);
	MakeDM(strProvider);
	CString strTmpData = _T("��������ó : ") + m_arrProviderDesc.GetAt(nCurSel);
	m_wndStatusBarCtrl.SetText(strTmpData, 0, 0);
	//m_pCM->SetControlMgrData(_T("��������ó"), m_arrProviderDesc.GetAt(nCurSel));
}

VOID COrderMailMgr::SetParentGroupCollection(CGroupCollection *pGroupCollection)
{
	if(pGroupCollection == NULL)
	{
		ESLAfxMessageBox(_T("�׷������� �����ϴ�."));
		return;
	}
	m_pParentGroupCollection = pGroupCollection;
	m_nOpenMode = 1;
}

VOID COrderMailMgr::SetRecKeyList(CString strReckeyList)
{
	m_strReckeyList = strReckeyList;
	m_nOpenMode = -1;
}

VOID COrderMailMgr::SetOperationType(INT nOpType)
{
	m_nOperationType = nOpType;
}

VOID COrderMailMgr::ShowCtrlByOpenMode()
{
	CString value;
	GetManageValue(_T("����"), _T("����"), _T("�ֹ������ڵ��߼�"), _T(""), value);
	if(m_nOpenType == AUTO && value == _T("N"))
	{
		EndDialog(IDCANCEL);
	}

	if(m_nOpenMode < 0)
	{
		((CEdit*)GetDlgItem(IDC_edtOMAIL_ACQYEAR))->SetReadOnly(TRUE);
		GetDlgItem(IDC_cmbOMAIL_DIVNO)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbOMAIL_PROVIDER)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnOMAIL_DIVNO_VIEW)->EnableWindow(FALSE);
	}
	else 
	{
		if(m_pGroupCollection->IsEmpty() && m_nOpenType == MANUAL)
		{
			((CEdit*)GetDlgItem(IDC_edtOMAIL_ACQYEAR))->SetReadOnly(FALSE);
			GetDlgItem(IDC_btnOMAIL_DIVNO_VIEW)->EnableWindow(TRUE);
		}
		else 
		{
			((CEdit*)GetDlgItem(IDC_edtOMAIL_ACQYEAR))->SetReadOnly(TRUE);
			GetDlgItem(IDC_btnOMAIL_DIVNO_VIEW)->EnableWindow(FALSE);
		}
	}
}

VOID COrderMailMgr::ShowCtrlByOperationType()
{
	if(m_nOperationType == ORDER)
	{
		SetWindowText(_T("�ֹ����Ϻ�����"));
	}
	else if(m_nOperationType == CANCEL)
	{
		SetWindowText(_T("�ֹ���Ҹ��Ϻ�����"));
	}
}

INT COrderMailMgr::MakeDM(CString strProviderCode/*=_T("")*/)
{
	if(m_pGroup)
	{
		CString strGroupInfo = m_pGroup->GetGroupInfo();
		CString strAcqYear = m_pGroup->GetAcqYear();
	}
	CK2UP_MailMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("PROVIDER"), strProviderCode.GetBuffer(0));
	mgr.SetTempPtr(_T("DM_BOOK"), (DWORD*)m_pDM_BOOK);
	mgr.SetTempPtr(_T("DM_VOL"), (DWORD*)m_pDM_VOL);
	mgr.SetSelectSpeciesKey(m_strSelectSpeciesKey);

	INT ids = -1;
	if(m_nOpenMode < 0)
	{
		if(m_strReckeyList.IsEmpty()) return 0;
		mgr.SetTempData(_T("KEY_LIST"), m_strReckeyList.GetBuffer(0));
		ids = mgr.SPFExecutePath(_T("åKEYDM����"));
	}
	else if(m_nOpenMode > 0)
	{
		if(m_pGroup == NULL) return 0;
		mgr.SetTempPtr(_T("GROUP_INFO"), (DWORD*)m_pGroup);
		ids = mgr.SPFExecutePath(_T("�׷�DM����"));
	}
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�ֹ��ڷḦ ������ �� �����ϴ�."));
		return ids;
	}
	mgr.GetTempData(_T("QUERY"), m_strQuery);
	
	SetTotalInfo();
	SortByGroupInfo();
	DisplayGrid();
	return 0;
}

INT COrderMailMgr::ExtractProvider()
{
	if(m_strQuery.IsEmpty()) return 0;
	CK2UP_MailMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("PROVIDER_ARRAY"), (DWORD*)&m_arrProvider);
	mgr.SetTempPtr(_T("PROVIDER_DESC_ARRAY"), (DWORD*)&m_arrProviderDesc);
	mgr.SetTempData(_T("QUERY"), m_strQuery.GetBuffer(0));
	INT ids = mgr.SPFExecutePath(_T("����ó����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("����ó ������ ������ �� �����ϴ�."));
		return ids;
	}
	return 0;
}

VOID COrderMailMgr::SetOpenType(INT nOpenType)
{
	m_nOpenType = nOpenType;
}

BOOL COrderMailMgr::Init()
{
	if(m_pParentGroupCollection != NULL)
	{
		m_pParentGroupCollection->CopyData(m_pGroupCollection);
		m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
	}

	m_wndStatusBarCtrl.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP , CRect(0, 0, 0, 0), this, WM_USER + 1000);
	m_wndStatusBarCtrl.SetMinHeight(22);
	
	const INT nParts = 5;
	CRect rect;
	m_wndStatusBarCtrl.GetClientRect(&rect);
	INT widths[nParts] = { 200, 300, 500, 700, -1 };
	m_wndStatusBarCtrl.SetParts(nParts, widths);
	
	INT ids = InitOrderMailData();
	if(ids < 0) return FALSE;

	return TRUE;
}

INT COrderMailMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CESL_DataMgr * pDM = m_pDM_VOL;
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode;
	INT nGroupNo, nSerialNo, nWorkNo;

	TCHAR* pEndPtr = NULL;

	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = m_pDM_VOL->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM_VOL->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM_VOL->SetCellData(_T("IBS_�ǻ���"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = m_pDM_VOL->GetCellData(_T("IBS_��ǥ��"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM_VOL->SetCellData(_T("UDF_ǥ������"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == m_pDM_VOL->GetRowCount()-1) m_nRow = -1;
	}

	if(strDMFieldAlias == _T("UDF_��������"))
	{	
		strAcqYear = m_pDM_VOL->GetCellData(_T("BP_���Գ⵵"), nRow);
		strWorkCode = m_pDM_VOL->GetCellData(_T("IBS_��������"), nRow);
		strGroupNo = m_pDM_VOL->GetCellData(_T("BP_�׷��ȣ"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		strWorkNo = m_pDM_VOL->GetCellData(_T("BP_�۾���ȣ"), nRow);
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		strSerialNo = m_pDM_VOL->GetCellData(_T("BP_�����Ϸù�ȣ"), nRow);
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
		m_pDM_VOL->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_��������"))
	{
		strTmpData = m_pDM_VOL->GetCellData(_T("IBS_�з�ǥ����"), nRow);
		if(strTmpData.IsEmpty()) str = _T("");
		else if(strTmpData == _T("1"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC��������ȣ����"), str, str);
		}
		else if(strTmpData == _T("2"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC��������ȣ����"), str, str);
		}
		m_pDM_VOL->SetCellData(_T("UDF_�������м���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_����ó��ȣ����"))
	{
		strTmpData = m_pDM_VOL->GetCellData(_T("BP_����ó��ȣ"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����ó����"), strTmpData, str);
		m_pDM_VOL->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_������"))
	{
		strTmpData = m_pDM_VOL->GetCellData(strDMFieldAlias, nRow);
		if(strTmpData.IsEmpty()) 
		{
			str = _T("0");
			m_pDM_VOL->SetCellData(strDMFieldAlias, _T("0"), nRow);
		}
	}
	else if(strDMFieldAlias == _T("UDF_��ȯ���ΰ���"))
	{
		CString strTmpData;
		DOUBLE dPrice, dDisc_Rate, dDisc_Price;
		INT nBookCnt = 0;
		strTmpData = m_pDM_VOL->GetCellData(_T("BP_������"), nRow);
		dDisc_Rate = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		strTmpData = m_pDM_VOL->GetCellData(_T("BB_����"), nRow);
		strTmpData.Replace(_T(","), _T(""));
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		strTmpData = m_pDM_VOL->GetCellData(_T("UDF_å��"), nRow);
		nBookCnt = _ttoi(strTmpData.GetBuffer(0));
		dPrice = dPrice*nBookCnt;
		dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
		strTmpData.Format(_T("%.0f"), dDisc_Price);
		m_pDM_VOL->SetCellData(_T("UDF_���ΰ�"), strTmpData, nRow);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pDM_VOL->SetCellData(strDMFieldAlias, str, nRow);
		
	}
	else if(strDMFieldAlias == _T("UDF_��ȯ����"))
	{
		CString strTmpData;
		INT nBookCnt = 0;
		DOUBLE dPrice;
		strTmpData = m_pDM_VOL->GetCellData(_T("BB_����"), nRow);

		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		strTmpData = m_pDM_VOL->GetCellData(_T("UDF_å��"), nRow);
		nBookCnt = _ttoi(strTmpData.GetBuffer(0));
		strTmpData.Format(_T("%.0f"), dPrice*nBookCnt);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pDM_VOL->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}
 
VOID COrderMailMgr::DisplayGrid()
{
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid->Display();
}

INT COrderMailMgr::SetTotalInfo()
{
	if(m_pDM_BOOK == NULL) return -1;
	INT nRowCnt = m_pDM_BOOK->GetRowCount();
	DOUBLE dPrice, dDisc_Price, dDisc_Rate, dTotalPrice, dDisc_TotalPrice;
	dTotalPrice = dDisc_TotalPrice = 0.0;
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = m_pDM_BOOK->GetCellData(_T("BP_������"), i);

		TCHAR* pEndPtr = NULL;
		dDisc_Rate = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		strTmpData = m_pDM_BOOK->GetCellData(_T("BB_����"), i);

		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
		dTotalPrice += dPrice;
		dDisc_TotalPrice += dDisc_Price;
	}

	strTmpData.Format(_T("��å�� : %d"), nRowCnt);
	m_wndStatusBarCtrl.SetText(strTmpData, 1, 0);

	strTmpData.Format(_T("%.0f"), dTotalPrice);
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
	strTmpData = _T("�����հ� : ") + strTmpData;
	m_wndStatusBarCtrl.SetText(strTmpData, 2, 0);

	strTmpData.Format(_T("%.0f"), dDisc_TotalPrice);
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
	strTmpData = _T("���ΰ��հ� : ") + strTmpData;
	m_wndStatusBarCtrl.SetText(strTmpData, 3, 0);
	
	return 0;
}

INT COrderMailMgr::InitOrderMailData()
{
	if(m_pGroup)
	{
		CString strGroupInfo = m_pGroup->GetGroupInfo();
		CString strAcqYear = m_pGroup->GetAcqYear();
	}
	INT ids = MakeDM();

	if(ids < 0) return -1;
	ids = ExtractProvider();
	if(ids < 0) return -1;
	
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("��ü�ڷ�"), &m_arrProviderDesc, _T("����ó"));
	if(m_pGroupCollection->GetGroupCount() > 0)
	{
		CStringArray tmpArray;
		CString strTmpData;
		m_pGroupCollection->GetGroupInfo(&tmpArray, strTmpData);
		m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, tmpArray.GetAt(0), &tmpArray, _T("�׷�����"));
	}

	CK2UP_MailMgr mgr(this);
	m_strAcqYear = m_pGroupCollection->GetAcqYear();
	if(m_strAcqYear.IsEmpty()) m_strAcqYear = mgr.GetTodayYear();
	
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_strAcqYear);
	CString strTmpData;
	m_pCM->GetControlMgrData(_T("����ó"), strTmpData);
	strTmpData = _T("��������ó : ") + strTmpData;
	m_wndStatusBarCtrl.SetText(strTmpData, 0, 0);

	return 0;
}

VOID COrderMailMgr::ShowDataProcessingDlg()
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

VOID COrderMailMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

INT COrderMailMgr::SortByGroupInfo()
{
	if(m_pDM_VOL == NULL) return 0;
	if(m_pDM_VOL->GetRowCount() == 0) return 0;

	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_���Գ⵵");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�׷��ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�۾���ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�Ϸù�ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pDM_VOL->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return 0;
}

VOID COrderMailMgr::OnSize(UINT nType, INT cx, INT cy) 
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
	rect.top += 60;
	rect.bottom -= 26;
	rect.left += 4;
	rect.right -= 4;
	
	m_pGrid->MoveWindow(rect);			
	// StatusBar Ctrl
	if (m_wndStatusBarCtrl.GetSafeHwnd() != NULL)
		m_wndStatusBarCtrl.SendMessage(WM_SIZE, 0, 0);
}

HBRUSH COrderMailMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
