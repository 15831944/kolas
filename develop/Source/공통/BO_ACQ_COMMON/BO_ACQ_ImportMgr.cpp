// BO_ACQ_ImportMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_ImportMgr.h"
#include "../BO_ACQ_API/BO_ACQ_Business_Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_ImportMgr dialog


CBO_ACQ_ImportMgr::CBO_ACQ_ImportMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_ImportMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_ImportMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentDM = NULL;
	m_pParentBookDM = NULL;
	m_pDM = NULL;
	m_pCM = NULL;
	m_pSearchDlg = NULL;
	m_nAcqType = ACQ_TYPE_INVALID;
	m_pGrid = NULL;
	m_nSearchType = SEARCH_TYPE_INVALID;
	m_pGroups = NULL;
	m_arrWorkNo.RemoveAll();
	m_pGroup = NULL;
	m_pBOSpeciesApi = NULL;
	m_nSpeciesType = SPECIES_TYPE_INVALID;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;
	m_nRow = -1;
	m_pBookDM = NULL;
}

CBO_ACQ_ImportMgr::~CBO_ACQ_ImportMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	if(m_pBOSpeciesApi)
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
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

VOID CBO_ACQ_ImportMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_ImportMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_ImportMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_ImportMgr)
	ON_BN_CLICKED(IDC_btnACQIMP_SEARCH, OnbtnACQIMPSEARCH)
	ON_CBN_SELCHANGE(IDC_cmbACQIMP_DATA_TYPE, OnSelchangecmbACQIMPDATATYPE)
	ON_BN_CLICKED(IDC_btnACQIMP_GROUP_CREATE, OnbtnACQIMPGROUPCREATE)
	ON_CBN_SELCHANGE(IDC_cmbACQIMP_GROUP_NO, OnSelchangecmbACQIMPGROUPNO)
	ON_CBN_SELCHANGE(IDC_cmbACQIMP_WORK_NO, OnSelchangecmbACQIMPWORKNO)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnIMPORT_DATA_SORT, OnbtnDATASORT)
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_ImportMgr message handlers

VOID CBO_ACQ_ImportMgr::OnbtnACQIMPSEARCH() 
{
	// TODO: Add your control notification handler code here
	if(m_pSearchDlg == NULL)
	{
		ESLAfxMessageBox(_T("�˻�ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		return ;
	}
	m_pSearchDlg->ShowSearchDlg();	
}

BOOL CBO_ACQ_ImportMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_�����ڷ����")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	GetWindowRect(&m_rcInitial);
	
	m_pCM = FindCM(_T("CM_�����ڷ����"));
	
	INT ids = ShowCtrlByAcqType();
	if( ids < 0 ) return FALSE;

	CString str64;
	GetManageValue(_T("��Ÿ"), _T("����"), _T("����"), _T(""), str64);
	
	m_pBOSpeciesApi = new CBO_SPECIES(this);
	
	CMulitComboBoxManager mgr(this);
	CStringArray arrTmp;
	
	arrTmp.Add(_T("�����ڷ�"));	
	arrTmp.Add(_T("��Ȱ���ڷ�"));
	
	if(str64 == _T("Y"))	arrTmp.Add(_T("��������ڷ�"));
	else				arrTmp.Add(_T("��ġ����ڷ�"));
	
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("�����ڷ�"), &arrTmp, _T("�ڷᱸ��"));

	OnSelchangecmbACQIMPDATATYPE();

	m_pGroup = m_pGroups->GetCurSelGroupInfo();
	CString strAcqYear = m_pGroup->GetAcqYear();
	if(strAcqYear.IsEmpty()) strAcqYear = GetTodayYear();
	m_pCM->SetControlMgrData(_T("���Գ⵵"), strAcqYear);
	CStringArray tmpArray;
	m_pGroups->GetGroupInfo(&tmpArray, strAcqYear);
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &tmpArray, _T("�׷�����"));
	if(m_nAcqType == PURCHASE)
	{
		CStringArray * pArray = m_pGroup->GetArrayWorkNo();
		mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetWorkNo(), pArray, _T("�۾���ȣ"));
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CBO_ACQ_ImportMgr::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

INT CBO_ACQ_ImportMgr::ShowCtrlByAcqType()
{
	if(m_nAcqType == ACQ_TYPE_INVALID)
	{
		ESLAfxMessageBox(_T("���Ա����� ������� �ʾҽ��ϴ�."));
		EndDialog(IDCANCEL);
		return -1;
	}

	if(m_nAcqType == PURCHASE)
	{
		this->SetWindowText(_T("���Դ���ڷ����"));
		GetDlgItem(IDC_btnACQIMP_GROUP_CREATE)->SetWindowText(_T("�ű���������"));
		GetDlgItem(IDC_cmbACQIMP_WORK_NO)->EnableWindow(TRUE);
		GetDlgItem(IDC_stcGROUP_INFO)->SetWindowText(_T("������ȣ"));
	}
	else if(m_nAcqType == DONATE)
	{
		this->SetWindowText(_T("���������ڷ����"));
		GetDlgItem(IDC_btnACQIMP_GROUP_CREATE)->SetWindowText(_T("������ȣ����"));
		GetDlgItem(IDC_cmbACQIMP_WORK_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_stcGROUP_INFO)->SetWindowText(_T("������ȣ"));
	}
	return 0;
}

VOID CBO_ACQ_ImportMgr::OnSelchangecmbACQIMPDATATYPE() 
{
	SetInfoByImpDataType();

	PostMessage(WM_SIZE, 0, 0);
}

VOID CBO_ACQ_ImportMgr::SetInfoByImpDataType()
{
	CESL_Grid * pGrid1, *pGrid2;
	pGrid1 = pGrid2 = NULL;
	CString strDataType;
	m_pCM->GetControlMgrData(_T("�ڷᱸ��"), strDataType);
	if(strDataType.IsEmpty()) 
	{
		strDataType = _T("�����ڷ�");
	}
	
	if(strDataType == _T("�����ڷ�"))
	
	{
		m_pDM = FindDM(_T("DM_�����ڷ����_��"));
		m_pBookDM = FindDM(_T("DM_�����ڷ����"));
		m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Basic"));
		m_nSearchType = BO_ACQ_GROUP;
		m_nSpeciesType = BO_DATA_IMPORT;
		pGrid1 = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Recycle"));
		pGrid2 = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Request"));
		InitSortItemOfBasicData();
	}
	else if(strDataType == _T("��Ȱ���ڷ�"))
	{
		m_pDM = FindDM(_T("DM_��Ȱ���ڷ����_��"));
		m_pBookDM = FindDM(_T("DM_��Ȱ���ڷ����"));
		m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Recycle"));
		m_nSearchType = BO_ACQ_RECYCLE;
		m_nSpeciesType = BO_RECYCLE;
		pGrid1 = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Basic"));
		pGrid2 = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Request"));
		InitSortItemOfRecycleData();
	}
	else if(strDataType == _T("��ġ����ڷ�"))
	{
		m_pDM = FindDM(_T("DM_��ġ����ڷ����"));
		m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Request"));
		m_nSearchType = BO_ACQ_REQUEST;
		m_nSpeciesType = BO_REQUEST;
		pGrid1 = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Basic"));
		pGrid2 = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Recycle"));
		InitSortItemOfRequestData();
	}
	m_pGrid->ShowWindow(SW_SHOW);
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	pGrid1->ShowWindow(SW_HIDE);
	pGrid2->ShowWindow(SW_HIDE);
	this->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	SetSearchDlgBySearchType();
}

VOID CBO_ACQ_ImportMgr::SetSearchDlgBySearchType()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}

	m_pSearchDlg = new CSearchMgr(this, m_nSearchType, ACQ_TYPE_INVALID, m_nSpeciesType, m_pDM->CONNECTION_INFO);
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
}

VOID CBO_ACQ_ImportMgr::OnbtnACQIMPGROUPCREATE() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strAddGroupNo, strAddWorkNo, strAddLastNo, strWorkCode;
	CMulitComboBoxManager mgr(this);
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	if(m_nAcqType == PURCHASE)
	{
		CDivisionNo_Insert_Dlg dlg(this);
		dlg.SetOpenMode(DINSERT);
		dlg.SetAcqYear(strAcqYear);
		dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		dlg.SetWorkStatus(_T("���Դ��"));
		if(IDOK == dlg.DoModal())
		{
			dlg.GetInsertInfo(strAddGroupNo, strAddWorkNo, strAddLastNo, strWorkCode);
			m_pGroups->AddGroupInfo(strAcqYear, strAddGroupNo, strWorkCode);
			m_pGroup = m_pGroups->GetCurSelGroupInfo();
			m_pGroup->SetLastSerialNo(_ttoi(strAddLastNo.GetBuffer(0)));
			CStringArray tmpArray;
			m_pGroups->GetGroupInfo(&tmpArray, strAddLastNo);
			mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strAddGroupNo, &tmpArray, _T("�׷�����"));
			mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strAddWorkNo, m_pGroup->GetArrayWorkNo(), _T("�۾���ȣ"));
		}
	}
	else if(m_nAcqType == DONATE)
	{
		CRequisitionNo_Insert_Dlg dlg(this);
		dlg.SetOpenMode(DINSERT);
		dlg.SetAcqYear(strAcqYear);
		if(IDOK == dlg.DoModal())
		{
			dlg.GetInsertInfo(strAddGroupNo);
			m_pGroups->AddGroupInfo(strAcqYear, strAddGroupNo);
			m_pGroup = m_pGroups->GetCurSelGroupInfo();
			CStringArray tmpArray;
			m_pGroups->GetGroupInfo(&tmpArray, strAddGroupNo);
			mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strAddGroupNo, &tmpArray, _T("�׷�����"));
		}
	}
}

INT CBO_ACQ_ImportMgr::MakeWorkNoList()
{
	CString strWorkNo, strAcqYear, strGroupNo;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("�׷�����"), strGroupNo);
	CGroupMgr groupMgr(this);
	groupMgr.InitTempData();
	groupMgr.SetTempData(_T("ACQ_YEAR"), strAcqYear.GetBuffer(0));
	groupMgr.SetTempData(_T("Group_No"), strGroupNo.GetBuffer(0));
	groupMgr.SetTempPtr(_T("Work_No"), (DWORD*)&m_arrWorkNo);
	INT ids = groupMgr.SPFExecutePath(_T("�۾���ȣ��������"));
	if(ids < 0) return ids;
	else if(ids == 0)
	{
		m_arrWorkNo.Add(_T("���λ���"));
	}
	else
	{
		m_arrWorkNo.Add(_T("������"));
	}
	CMulitComboBoxManager mgr(this);
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &m_arrWorkNo, _T("�۾���ȣ"));
	
	ids = MakeLastSerialNo();
	
	return ids;
}

INT CBO_ACQ_ImportMgr::MakeLastSerialNo()
{
	CGroupMgr groupMgr(this);
	CString strWorkNo, strAcqYear, strGroupNo;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("�׷�����"), strGroupNo);
	m_pCM->GetControlMgrData(_T("�۾���ȣ"), strWorkNo);
	if(strWorkNo == _T("������")) strWorkNo = _T("0");
	groupMgr.InitTempData();
	groupMgr.SetTempData(_T("ACQ_TYPE"), m_nAcqType);
	groupMgr.SetTempData(_T("ACQ_YEAR"), strAcqYear.GetBuffer(0));
	groupMgr.SetTempData(_T("Group_Info"), strGroupNo.GetBuffer(0));
	groupMgr.SetTempData(_T("Work_Info"), strWorkNo.GetBuffer(0));
	INT ids = groupMgr.SPFExecutePath(_T("�������Ϸù�ȣ����"));
	if(ids < 0) return ids;

	CString strLastNo;
	groupMgr.GetTempData(_T("Last_Serial_No"), strLastNo);

	m_strLastSerialNo = strLastNo;
	
	return 0;
}

INT CBO_ACQ_ImportMgr::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(_T(" "), 0);
	CString strTmpData;
	while(pos > 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+1);
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		if(!CheckIsHave(pTargetArray, strTmpData))
			pTargetArray->Add(strTmpData);
		pos = strSource.Find(_T(" "), 0);
	}
	((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(!CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	return 0;
}

BOOL CBO_ACQ_ImportMgr::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;
}

VOID CBO_ACQ_ImportMgr::OnSelchangecmbACQIMPGROUPNO() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�׷�����"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroups->SetCurSel(nCurSel);
	m_pGroup = m_pGroups->GetCurSelGroupInfo();
	if(m_nAcqType == PURCHASE)
	{
		CStringArray * pArray = m_pGroup->GetArrayWorkNo();
		CMulitComboBoxManager mgr(this);
		mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetWorkNo(), pArray, _T("�۾���ȣ"));
	}
}

VOID CBO_ACQ_ImportMgr::OnSelchangecmbACQIMPWORKNO() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�۾���ȣ"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroups->SetCurSelWorkNo(nCurSel);
}

VOID CBO_ACQ_ImportMgr::OnOK() 
{
	// TODO: Add extra validation here
	
		INT ids = -1;
	if(m_nAcqType == PURCHASE)
		ids = PurchaseData();
	else if(m_nAcqType == DONATE)
		ids = DonateData();

	m_pGrid->SelectMakeList();
	INT nSelectCnt; 
	nSelectCnt = m_pGrid->SelectGetCount();
	if( nSelectCnt < 1 )
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�.."));
		return ;
	}

	if(ids < 0)
	{
		if(m_nAcqType == PURCHASE)
			ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �� �� �����ϴ�."));
		else if(m_nAcqType == DONATE)
			ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �� �� �����ϴ�."));
		return ;
	}
	if(m_nAcqType == PURCHASE)
		ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �Ͽ����ϴ�."));
	else if(m_nAcqType == DONATE)
		ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �Ͽ����ϴ�."));
	DisplayGrid(m_pGrid);
	m_pParentGrid->Display();
}

INT CBO_ACQ_ImportMgr::PurchaseData()
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	CGroupInfo * pGroup = NULL;

	m_pDM->StartFrame();
	
	CString strWorkNo = m_pGroup->GetWorkNo();
	if(strWorkNo == _T("���λ���")) 
	{
		INT ids = m_pGroups->InsertWorkNo(m_pDM);
		if(ids < 0) return -1;
	}
	if(m_pGroup->GetGroupInfo() == _T("�������"))	
		pGroup = NULL;
	else
		pGroup = m_pGroup;

	INT ids = -1;
	if(m_nSearchType == BO_ACQ_GROUP)
	{
		api.SetTempData(_T("SPECIES_TYPE"), (INT)SPECIES_TYPE_INVALID);
		api.SetTempPtr(_T("ParentDM"), (DWORD*)m_pParentDM);
		api.SetTempPtr(_T("ParentBookDM"), (DWORD*)m_pParentBookDM);
		api.SetTempPtr(_T("ParentGrid"), (DWORD*)m_pParentGrid);
		api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
		api.SetTempPtr(_T("BOOKDM"), (DWORD*)NULL);
		api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
		api.SetTempPtr(_T("GroupInfo"), (DWORD*)pGroup);
	}
	else if(m_nSearchType == BO_ACQ_RECYCLE)
	{
		api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_RECYCLE);
		api.SetTempPtr(_T("ParentDM"), (DWORD*)m_pParentDM);
		api.SetTempPtr(_T("ParentBookDM"), (DWORD*)m_pParentBookDM);
		api.SetTempPtr(_T("ParentGrid"), (DWORD*)m_pParentGrid);
		api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
		api.SetTempPtr(_T("BOOKDM"), (DWORD*)NULL);
		api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
		api.SetTempPtr(_T("GroupInfo"), (DWORD*)pGroup);
	}
	else if(m_nSearchType == BO_ACQ_REQUEST)
	{
		CStringArray arrFurnishKey;
		api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_REQUEST);
		api.SetTempPtr(_T("ParentDM"), (DWORD*)m_pParentDM);
		api.SetTempPtr(_T("ParentBookDM"), (DWORD*)m_pParentBookDM);
		api.SetTempPtr(_T("ParentGrid"), (DWORD*)m_pParentGrid);
		api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
		api.SetTempPtr(_T("BOOKDM"), (DWORD*)NULL);
		api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
		api.SetTempPtr(_T("GroupInfo"), (DWORD*)pGroup);
		api.SetTempPtr(_T("FurnishKey"), (DWORD*)&arrFurnishKey);
	}
	ids = api.SPFExecutePath(_T("����ó��_�θ�����"));		

	return ids; 
}

INT CBO_ACQ_ImportMgr::DonateData()
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();

	INT ids = -1;
	if(m_nSearchType == BO_ACQ_GROUP)
	{
		api.SetTempData(_T("SPECIES_TYPE"), (INT)SPECIES_TYPE_INVALID);
		api.SetTempPtr(_T("ParentDM"), (DWORD*)m_pParentDM);
		api.SetTempPtr(_T("ParentBookDM"), (DWORD*)m_pParentBookDM);
		api.SetTempPtr(_T("ParentGrid"), (DWORD*)m_pParentGrid);
		api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
		api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
		api.SetTempPtr(_T("GroupInfo"), (DWORD*)m_pGroup);
	}
	else if(m_nSearchType == BO_ACQ_RECYCLE)
	{
		api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_RECYCLE);
		api.SetTempPtr(_T("ParentDM"), (DWORD*)m_pParentDM);
		api.SetTempPtr(_T("ParentBookDM"), (DWORD*)m_pParentBookDM);
		api.SetTempPtr(_T("ParentGrid"), (DWORD*)m_pParentGrid);
		api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
		api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
		api.SetTempPtr(_T("GroupInfo"), (DWORD*)m_pGroup);
	}
	else if(m_nSearchType == BO_ACQ_REQUEST)
	{
		CStringArray arrFurnishKey;
		api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_REQUEST);
		api.SetTempPtr(_T("ParentDM"), (DWORD*)m_pParentDM);
		api.SetTempPtr(_T("ParentBookDM"), (DWORD*)m_pParentBookDM);
		api.SetTempPtr(_T("ParentGrid"), (DWORD*)m_pParentGrid);
		api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
		api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
		api.SetTempPtr(_T("GroupInfo"), (DWORD*)m_pGroup);
		api.SetTempPtr(_T("FurnishKey"), (DWORD*)&arrFurnishKey);
	}
	ids = api.SPFExecutePath(_T("����ó��_�θ�����"));

	return ids; 
}

UINT CBO_ACQ_ImportMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery, strMsg;
	ShowDataProcessingDlg();
	if(m_nSpeciesType == BO_DATA_IMPORT)
	{
		strMsg = _T("�����ڷḦ �������Դϴ�.");		
	}
	else if(m_nSpeciesType == BO_RECYCLE)
		strMsg = _T("��Ȱ���ڷḦ �������Դϴ�.");
	else if(m_nSpeciesType == BO_REQUEST)
		strMsg = _T("��ġ����ڷḦ �������Դϴ�.");

	CESL_DataMgr * pDM = NULL;
	if(m_nSpeciesType == BO_REQUEST) pDM = m_pDM;
	else pDM = m_pBookDM;

	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pSearchDlg->GetWhereStmt(strQuery);
	pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	INT rowCnt = pDM->GetRowCount();
	if(rowCnt == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		DeleteDataProcessingDlg();
		return 0;
	}

	if(m_nSpeciesType == BO_RECYCLE || m_nSpeciesType == BO_DATA_IMPORT)
	{
		m_pDM->FreeData();
		CBO_ACQ_Business_Api mgr(this);
		mgr.InitTempPtr();
		mgr.SetTempPtr(_T("åDM"), (DWORD*)pDM);
		mgr.SetTempPtr(_T("��DM"), (DWORD*)m_pDM);
		mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
		INT ids = mgr.SPFExecutePath(_T("åTo��"));
	}

	DeleteDataProcessingDlg();
	m_pSortItemSet->DataSort(m_pDM, !m_pSearchDlg->GetDataSortYN());
	DisplayGrid(m_pGrid);
	return 0;
}

INT CBO_ACQ_ImportMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData, strPriceInfo, strPrice, strCurrencyCode, strAlias;
	if(m_nRow != nRow && m_nSpeciesType != BO_REQUEST)
	{
		CMarc marc;
		INT ids;
		strTmpData = m_pDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);

		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetCurrencyCode(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_ȭ�󱸺�"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetPriceInfo(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_����"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("IBS_�ǻ���"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = m_pDM->GetCellData(_T("IBS_��ǥ��"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_ǥ������"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_������"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == m_pDM->GetRowCount()-1) m_nRow = -1;
	}
	
	if(strDMFieldAlias == _T("UDF_�ڷ���¼���"))
	{
		CString strWorkingStatus = m_pDM->GetCellData(_T("IBS_�ڷ����"), nRow);
		if(strWorkingStatus.GetLength() == 0) str = _T("�����ڷ�");
		else 
		{
			strTmpData = strWorkingStatus.Left(3);
			strTmpData.MakeUpper();
			if(strTmpData == _T("BOA")) str = _T("�����ڷ�");
			else if(strTmpData == _T("BOR")) str = _T("����ڷ�");
			else if(strTmpData == _T("BOC")) str = _T("�����ڷ�");
			else if(strTmpData == _T("BOL")) str = _T("�����ڷ�");
			else if(strTmpData == _T("B0L")) str = _T("�����ڷ�");
		}
		m_pDM->SetCellData(_T("UDF_�ڷ���¼���"), str, nRow);
	}
	if(strDMFieldAlias == _T("��ġ����"))
	{
		if(str.GetLength() == 0) str = _T("�������");
		else if(str == _T("1")) str = _T("��û��");
		else if(str == _T("2")) str = _T("ó����");
		else if(str == _T("3")) str = _T("������");
		else if(str == _T("4")) str = _T("��ҵ�");
		m_pDM->SetCellData(_T("��ġ���¼���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_����"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pDM->SetCellData(_T("UDF_��ȯ����"), str, nRow);
	}
	return 0;	
}

VOID CBO_ACQ_ImportMgr::DisplayGrid(CESL_Grid * pGrid)
{
	pGrid->Display();
}


VOID CBO_ACQ_ImportMgr::ShowDataProcessingDlg()
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

VOID CBO_ACQ_ImportMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}
VOID CBO_ACQ_ImportMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

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

	if(m_pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rect;
	GetClientRect(rect);
	rect.top += 74;
	rect.bottom -= 4;
	rect.left += 4;
	rect.right -= 4;
	
	m_pGrid->MoveWindow(rect);		
}

BOOL CBO_ACQ_ImportMgr::InitSortItemOfBasicData()
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
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid_Basic"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("UDF_�׷��"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_�������м���"), _T("BS_��������"));
	m_pSortItemSet->ReadSortItem();
	return TRUE;
}

BOOL CBO_ACQ_ImportMgr::InitSortItemOfRequestData()
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
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid_Request"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("��ġ���¼���"), _T("��ġ����"));
	m_pSortItemSet->ReadSortItem();
	return TRUE;
}

BOOL CBO_ACQ_ImportMgr::InitSortItemOfRecycleData()
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
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid_Recycle"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("UDF_�׷��"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_�ڷ���¼���"), _T("IBS_�ڷ����"));
	m_pSortItemSet->ReadSortItem();
	return TRUE;
}

VOID CBO_ACQ_ImportMgr::OnbtnDATASORT() 
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
	if(IDOK == dlg.DoModal())
		m_pGrid->Display();
}

HBRUSH CBO_ACQ_ImportMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
