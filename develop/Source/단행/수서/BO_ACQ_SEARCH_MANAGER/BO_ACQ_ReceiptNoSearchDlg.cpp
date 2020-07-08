// BO_ACQ_ReceiptNoSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/RequisitionNo_List_Dlg.h"
#include "BO_ACQ_ReceiptNoSearchDlg.h"
#include "../../../����/COMMON_DONATOR_MANAGER/DonatorNameGetter_Search.h"
#include "../../../����/COMMON_DONATOR_MANAGER/DonatorNameGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_ReceiptNoSearchDlg dialog


CBO_ACQ_ReceiptNoSearchDlg::CBO_ACQ_ReceiptNoSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBO_SearchDlg(CBO_ACQ_ReceiptNoSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_ReceiptNoSearchDlg)
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_strDonatorKey = _T("");
}
CBO_ACQ_ReceiptNoSearchDlg::~CBO_ACQ_ReceiptNoSearchDlg()
{
}

VOID CBO_ACQ_ReceiptNoSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_ReceiptNoSearchDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_ReceiptNoSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_ReceiptNoSearchDlg)
	ON_EN_KILLFOCUS(IDC_edtDIVSEARCH_ACQYEAR, OnKillfocusedtDIVSEARCHACQYEAR)
	ON_EN_KILLFOCUS(IDC_edtDIVSEARCH_NO, OnKillfocusedtDIVSEARCHNO)
	ON_BN_CLICKED(IDC_btnDIVSEARCH_VIEW, OnbtnDIVSEARCHVIEW)
	ON_BN_CLICKED(IDC_btnDIVSEARCH_DONATOR_SEARCH, OnbtnDIVSEARCHDONATORSEARCH)
	ON_BN_CLICKED(IDC_btnDIVSEARCH_CLEAR, OnbtnDIVSEARCHCLEAR)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_EN_SETFOCUS(IDC_edtDIVSEARCH_DONATOR, OnSetfocusedtDIVSEARCHDONATOR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_ReceiptNoSearchDlg message handlers

BOOL CBO_ACQ_ReceiptNoSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_������ȣ�˻�")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM����
	m_pCM = FindCM(_T("CM_������ȣ�˻�"));	
	

	GetWindowRect(m_rcWindow);
	if(!Init())
	{
		ESLAfxMessageBox(_T("�˻�ȭ���� �ʱ�ȭ�� �� �����ϴ�."));
		return FALSE;
	}	

	//============================================
	//20080603 ADD BY PJW : ���Գ��� ���� 4�ڸ� �̻� �����ʵ��� ����
//	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtDIVSEARCH_ACQYEAR);
//	pAcqYear->SetLimitText(4);
	//============================================

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_ACQ_ReceiptNoSearchDlg::Init()
{
	CString acqCode[5][2] = 
	{
		_T("1"), _T("����"),
		_T("2"), _T("����"),
	};
	CMulitComboBoxManager mgr(this);
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("����"), acqCode, _T("���Ա���"), 2);
	CString strAcqCode = _T("2");
	m_pCM->SetControlMgrData(_T("���Ա���"), strAcqCode);
	CESL_ControlMultiComboBox * pElement = (CESL_ControlMultiComboBox*)m_pCM->FindControl(_T("���Ա���"));
	pElement->EnableWindow(FALSE);
	return TRUE;
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnKillfocusedtDIVSEARCHACQYEAR() 
{
	//============================================
	//20080603 ADD BY PJW : ���Գ��� ���� 4�ڸ��� �����ʾ��� ��� 4�ڸ� ���ڸ� �����ش� 
//	CString strAcqYear;
//	INT nYear;
//	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtDIVSEARCH_ACQYEAR);
//	pAcqYear->GetWindowText(strAcqYear);
//	if( strAcqYear.IsEmpty() )
//	{
//		return;
//	}
//	else
//	{
//		nYear = _ttoi(strAcqYear);
//	}
//	if( nYear < 1000 || nYear > 9999 )
//	{
//		ESLAfxMessageBox(_T("1000 ~ 9999 ������ ���� �Է����ּ���"));
//		pAcqYear->SetWindowText(_T(""));
//	}
	//============================================	

	OnKillfocusedtDIVSEARCHNO();
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnKillfocusedtDIVSEARCHNO() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("�׷��ȣ"), m_strSelDivNo);
	
	m_strSelDivNo = CheckValidateGroupNo(m_pCM, m_strSelDivNo, strAcqYear);
	
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), m_strSelDivNo);
}

BOOL CBO_ACQ_ReceiptNoSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		GetDlgItem(IDOK)->SetFocus();
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	else if (pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_edtDIVSEARCH_NO)->GetSafeHwnd())
    {
		INT nVirtKey = (INT) pMsg->wParam;
		if ((nVirtKey < '0' || nVirtKey > '9') && nVirtKey != ',' && nVirtKey != ' ' && nVirtKey != 8)
		{
			ESLAfxMessageBox(_T("���ڸ� �Է��Ͻ� �� �ֽ��ϴ�.")); 
            return TRUE;
		}
    }
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnbtnDIVSEARCHVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear = m_pGroups->GetAcqYear();
	if(strAcqYear.IsEmpty())
		m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);

	CRequisitionNo_List_Dlg dlg(this);
	dlg.SetSpeciesType(GetSpeciesType());
	dlg.SetAcqYear(strAcqYear);
	dlg.SetGroupCollection(m_pGroups);
	if(IDOK == dlg.DoModal())
	{
		m_strSelDivNo = dlg.GetSelInfo();
		m_pCM->SetControlMgrData(_T("�׷��ȣ"), m_strSelDivNo);
		m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pGroups->GetAcqYear());
		OnOK();
	}	
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	// CM���� ����ڰ� ������ �˻��������� �˻� ������ �����Ѵ�.
	CString strAcqYear, strAcqCode, strDonator, strDate1, strDate2;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("���Ա���"), strAcqCode);
	m_pCM->GetControlMgrData(_T("������"), strDonator);
	m_pCM->GetControlMgrData(_T("�׷��ȣ"), m_strSelDivNo);
	m_pCM->GetControlMgrData(_T("�����1"), strDate1); 
	m_pCM->GetControlMgrData(_T("�����2"), strDate2); 
	
	TrimAll(strAcqYear);	TrimAll(strAcqCode);
	TrimAll(strDonator);
	TrimAll(m_strSelDivNo);
	
	//�Էµ� �׷����� �˻�
	CString strMsg;
	//�׷��÷��� ����
	if(!m_strSelDivNo.IsEmpty())
		MakeGroupInfo(strAcqYear, m_strSelDivNo);

	INT ids ;
	// IDX_BO_TBL�� SPECIES_CLASS 0:������, 1:�۾���, 2:��Ȱ��, 3:������
	/*********************************************************************************
	 *********************************************************************************/
	//20080218 UPDATE BY PJW : ������ȣ���⿡�� ���� å���� �˻�å���� �����.
	//                         P.SPECIES_KEY -> B.SPECIES_KEY �� ����
	//CString strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND P.DONATOR_KEY = C.REC_KEY(+) AND I.SPECIES_CLASS IN('0', '1')");
	CString strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = B.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND P.DONATOR_KEY = C.REC_KEY(+) AND I.SPECIES_CLASS IN('0', '1') AND I.MANAGE_CODE = UDF_MANAGE_CODE");
	/*********************************************************************************
	 *********************************************************************************/
	
	CString strWorkingStatus = _T("BOA211O");
	CString strOption;
	CString errstr, result;
	
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	TrimAll(strAcqYear);
	ids = pMakeSearchData->IsValidYearData(strAcqYear);
	if( ids < 0)
	{
		errstr.Format(_T("�Է��Ͻ� ���Գ⵵ ��(%s)�� �ùٸ��� �ʽ��ϴ�."), strAcqYear);
		ESLAfxMessageBox(errstr.GetBuffer(0));
		m_pCM->SetControlMgrData(_T("���Գ⵵"), GetTodayYear());		
		return;
	}
// �����(�Է���)�κ��� ���� ����  ========================================= ���� 10.10
	// 2004.11.30 C.INPUT_DATE -> P.INPUT_DATE
	// strOption = MakeDateCondition_Stmt(_T("C.INPUT_DATE"), strDate1, strDate2);
	strOption = MakeDateCondition_Stmt(_T("P.INPUT_DATE"), strDate1, strDate2);
	if(strOption == _T("-1")) return ;
	strWhere += strOption;

	CString strAccuWhereStmt;
	if(GetOpenMode() != BO_DONATE_PRINT)
	{
		strOption.Format(_T(" AND B.WORKING_STATUS = '%s' AND B.ACQ_CODE='2'"), strWorkingStatus);
		strAccuWhereStmt += strOption;
	}
//������������ ����
	if(!strAcqYear.IsEmpty())
	{
		strOption.Format(_T(" AND P.ACQ_YEAR = '%s'"), strAcqYear);
		strAccuWhereStmt += strOption;
	}
	
	if(!strDonator.IsEmpty())
	{
		//strOption.Format(_T(" AND P.DONATOR_NAME LIKE '%s%%'"), strDonator);
		//strAccuWhereStmt += strOption;
		CESL_DataMgr dm;
		CString strDonatorCnt, sQuery;
		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		sQuery.Format(_T("SELECT COUNT(REC_KEY) FROM CO_DONATOR_TBL WHERE DONATOR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),strDonator);
		dm.GetOneValueQuery(sQuery,strDonatorCnt);
		INT nCnt = _ttoi(strDonatorCnt);
		
		if(m_strDonatorKey.IsEmpty() && nCnt > 1 )
		{
			AfxMessageBox(_T("�ߺ��Ǿ��ų� ���� �������Դϴ�. \n\r �˻��� ���� �������ּ���!"));
			return;
		}

		if(m_strDonatorKey.IsEmpty())
		{
			strOption.Format(_T(" AND P.DONATOR_NAME LIKE '%s%%' "), strDonator);
			strAccuWhereStmt += strOption;
		}
		else
		{
			strOption.Format(_T(" AND P.DONATOR_NAME LIKE '%s%%' AND P.DONATOR_KEY = '%s' "), strDonator, m_strDonatorKey);
			strAccuWhereStmt += strOption;
		}
	}	
	if(!m_strSelDivNo.IsEmpty())
	{
		strOption = _T(" AND (") + this->MakeGroupInfoOrQuery(m_strSelDivNo) + _T(")");
		strAccuWhereStmt += strOption;
	}
	
	//��������
	strWhere += strAccuWhereStmt;
	SetWhereStmt(strWhere);

	// ���� ������ �θ𿡰� �޽����� ������.
	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);	
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnbtnDIVSEARCHDONATORSEARCH() 
{
	// TODO: Add your control notification handler code here
	CDonatorNameGetter_Search searchDlg(this);
	searchDlg.SetOpenMode(-1000);
	if(IDOK == searchDlg.DoModal())
	{
		CDonatorNameGetterDlg searchResult(this);
		if(IDOK == searchResult.DoModal())
		{
			m_pCM->SetControlMgrData(_T("������"), searchResult.GetDonatorName());
			m_strDonatorKey = searchResult.GetDonatorKey();
		}
	}	
}

VOID CBO_ACQ_ReceiptNoSearchDlg::ShowGroupInfo()
{
	CString strAcqYear;
	strAcqYear = m_pGroups->GetAcqYear();
	if(strAcqYear.IsEmpty()) strAcqYear = GetTodayYear();
	m_pGroups->GetGroupInfo(m_strSelDivNo);
	if(m_strSelDivNo == _T("��ü�ڷ�")) m_strSelDivNo = _T("");
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), m_strSelDivNo);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), strAcqYear);
	GetDlgItem(IDC_edtDIVSEARCH_NO)->SetFocus();
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnbtnDIVSEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	ClearScreen(m_pCM);
	//20071101 ADD PJW : ȭ������� ��ư������ ȭ�� �ʱ�ȭ �߰� Init();
	Init();
	m_pGroups->RemoveAll();
	ShowGroupInfo();
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	ShowDataSortDlg();
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton *)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;	
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnSetfocusedtDIVSEARCHDONATOR() 
{
	SetIME(IDC_edtDIVSEARCH_DONATOR, 1);
}

HBRUSH CBO_ACQ_ReceiptNoSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}