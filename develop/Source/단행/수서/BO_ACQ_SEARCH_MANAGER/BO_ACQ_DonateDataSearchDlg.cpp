// BO_ACQ_DonateDataSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_DonateDataSearchDlg.h"
#include "../../../����/COMMON_DONATOR_MANAGER/DonatorNameGetter_Search.h"
#include "../../../����/COMMON_DONATOR_MANAGER/DonatorNameGetterDlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/RequisitionNo_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_DonateDataSearchDlg dialog


CBO_ACQ_DonateDataSearchDlg::CBO_ACQ_DonateDataSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBO_SearchDlg(CBO_ACQ_DonateDataSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_DonateDataSearchDlg)
	//}}AFX_DATA_INIT
	m_bShowMode = FALSE;
	m_pCM = NULL;
	m_strSelDivNo = _T("");
	m_strDonatorKey = _T("");

	// openbase flag	
	CFileFind ff;
	if ( ff.FindFile(_T("..\\cfg\\_OPENBASE")) )
	{
		m_bOpenbase = TRUE;
	}
	else
	{
		m_bOpenbase = FALSE;
	}

}
CBO_ACQ_DonateDataSearchDlg::~CBO_ACQ_DonateDataSearchDlg()
{
}

VOID CBO_ACQ_DonateDataSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_DonateDataSearchDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_DonateDataSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_DonateDataSearchDlg)
	ON_BN_CLICKED(IDC_btnFIND_DONATOR, OnbtnFINDDONATOR)
	ON_BN_CLICKED(IDC_btnSEARCH_DIVISIONNO_VIEW, OnbtnSEARCHDIVISIONNOVIEW)
	ON_EN_KILLFOCUS(IDC_edtSEARCH_ACQ_YEAR, OnKillfocusedtSEARCHACQYEAR)
	ON_EN_KILLFOCUS(IDC_edtSEARCH_DIVISIONNO, OnKillfocusedtSEARCHDIVISIONNO)
	ON_CBN_SELCHANGE(IDC_cmbSEARCH_CLASS_NO_TYPE, OnSelchangecmbSEARCHCLASSNOTYPE)
	ON_BN_CLICKED(IDC_btnSEARCH_SHOWHIDE, OnbtnSEARCHSHOWHIDE)
	ON_BN_CLICKED(IDC_btnSEARCH_CLEAR, OnbtnSEARCHCLEAR)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_EN_SETFOCUS(IDC_edtSEARCH_AUTHOR, OnSetfocusedtSEARCHAUTHOR)
	ON_EN_SETFOCUS(IDC_edtSEARCH_DONATOR, OnSetfocusedtSEARCHDONATOR)
	ON_EN_SETFOCUS(IDC_edtSEARCH_PUBLISHER, OnSetfocusedtSEARCHPUBLISHER)
	ON_EN_SETFOCUS(IDC_edtSEARCH_TITLE, OnSetfocusedtSEARCHTITLE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_DonateDataSearchDlg message handlers

BOOL CBO_ACQ_DonateDataSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_���������ڷ�˻�")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM����
	m_pCM = FindCM(_T("CM_���������ڷ�˻�"));		
	if(!Init())
	{
		ESLAfxMessageBox(_T("�˻�ȭ���� �ʱ�ȭ�� �� �����ϴ�."));
		return FALSE;
	}
	
	GetWindowRect(m_rcWindow);
	ShowControlByShowMode();
	//============================================
	//20080603 ADD BY PJW : ���Գ��� ���� 4�ڸ� �̻� �����ʵ��� ����
	//CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtSEARCH_ACQ_YEAR);
	//pAcqYear->SetLimitText(4);
	//============================================
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_DonateDataSearchDlg::OnbtnFINDDONATOR() 
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

BOOL CBO_ACQ_DonateDataSearchDlg::Init()
{
	CString inputType[5][2] = 
	{
		_T(" "), _T("�������"),
		_T("0"), _T("ȭ���Է�"),
		_T("1"), _T("��Ȱ�����"),
		_T("2"), _T("����ڷ����"),
		_T("3"), _T("���Ϲ���")
	};
	CMulitComboBoxManager mgr(this);
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("�������"), inputType, _T("�Է±���_�ڵ�"), 5);

	//�������� ����
	//=====================================================
	//2008.07.16 UPDATE BY PJW : ���������ڵ带 ����Ʈ������ �������� ����
// 	SetSubjectCodeData(m_pCM->CM_ALIAS, _T("�з�ǥ����_�ڵ�"), _T("��������_�ڵ�"));
	CString strSubCode;
	m_pInfo->pCodeMgr->GetDefaultCode(_T("KDC��������ȣ����"), strSubCode);
	if( strSubCode.IsEmpty() )
	{
		strSubCode.Format(_T("  "));
	}
	mgr.SetSubjectCodeData(m_pCM->CM_ALIAS, _T("�з�ǥ����_�ڵ�"), _T("��������_�ڵ�"), strSubCode);
	//=====================================================
	return TRUE;
}

VOID CBO_ACQ_DonateDataSearchDlg::OnbtnSEARCHDIVISIONNOVIEW() 
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
		OnKillfocusedtSEARCHDIVISIONNO();
		OnOK();
	}	
}

VOID CBO_ACQ_DonateDataSearchDlg::OnKillfocusedtSEARCHACQYEAR() 
{
	// TODO: Add your control notification handler code here
	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtSEARCH_ACQ_YEAR);
	//============================================
	//20080603 ADD BY PJW : ���Գ��� ���� 4�ڸ��� �����ʾ��� ��� 4�ڸ� ���ڸ� �����ش� 
//	CString strAcqbYear;
//	INT nYear;
//	pAcqYear->GetWindowText(strAcqbYear);
//	if( strAcqbYear.IsEmpty() )
//	{
//		return;
//	}
//	else
//	{
//		nYear = _ttoi(strAcqbYear);
//	}
//	if( nYear < 1000 || nYear > 9999 )
//	{
//  	ESLAfxMessageBox(_T("1000 ~ 9999 ������ ���� �Է����ּ���"));
//		pAcqYear->SetWindowText(_T(""));
//	}
	//============================================
	if (pAcqYear->GetStyle() & ES_READONLY) return;	
	OnKillfocusedtSEARCHDIVISIONNO();	
}

VOID CBO_ACQ_DonateDataSearchDlg::OnKillfocusedtSEARCHDIVISIONNO() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("�׷��ȣ"), m_strSelDivNo);
	m_strSelDivNo = CheckValidateGroupNo(m_pCM, m_strSelDivNo, strAcqYear);
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), m_strSelDivNo);
}

VOID CBO_ACQ_DonateDataSearchDlg::OnSelchangecmbSEARCHCLASSNOTYPE() 
{
	// TODO: Add your control notification handler code here
	
	//================================================================
	//2008.07.16 UPDATE BY PJW : �з�ǥ���п� ���� ���������� ������ ������ ����Ʈ���� �������� ����
// 	SetSubjectCodeData(m_pCM->CM_ALIAS, _T("�з�ǥ����_�ڵ�"), _T("��������_�ڵ�"));
	CString strSubCode;
	CMulitComboBoxManager mgr(this);
	m_pInfo->pCodeMgr->GetDefaultCode(_T("KDC��������ȣ����"), strSubCode);
	strSubCode.TrimLeft();	strSubCode.TrimRight();
	if( strSubCode.IsEmpty() )
	{
		strSubCode.Format(_T("  "));
	}

	mgr.SetSubjectCodeData(m_pCM->CM_ALIAS, _T("�з�ǥ����_�ڵ�"), _T("��������_�ڵ�"), strSubCode);
	//===============================================================
}

BOOL CBO_ACQ_DonateDataSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		GetDlgItem(IDOK)->SetFocus();
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	else if (pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_edtSEARCH_DIVISIONNO)->GetSafeHwnd())
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

VOID CBO_ACQ_DonateDataSearchDlg::OnbtnSEARCHSHOWHIDE() 
{
	// TODO: Add your control notification handler code here
	if (m_bShowMode)
	{
		m_bShowMode = FALSE;
	}
	else
	{
		m_bShowMode = TRUE;
	}	
	ShowControlByShowMode();	
}


VOID CBO_ACQ_DonateDataSearchDlg::ShowControlByShowMode()
{
	INT i = 0;
	INT nCodeResourcesHeight = 224;
	CWnd *pWnd = NULL;

	if (m_bShowMode)
	{
		GetDlgItem(IDC_btnSEARCH_SHOWHIDE)->SetWindowText(_T("�����˻�"));
	}
	else
	{
		GetDlgItem(IDC_btnSEARCH_SHOWHIDE)->SetWindowText(_T("�󼼰˻�"));
	}


	CRect rcWindow;
	GetWindowRect(rcWindow);

	CArray<INT, INT> ArrResourceID;	ArrResourceID.RemoveAll();
	ArrResourceID.Add(IDC_stcGROUP1);
	ArrResourceID.Add(IDC_stcAUTHOR);
	ArrResourceID.Add(IDC_edtSEARCH_AUTHOR);
	ArrResourceID.Add(IDC_stcPUBLISHER);
	ArrResourceID.Add(IDC_edtSEARCH_PUBLISHER);
	ArrResourceID.Add(IDC_stcPUBYEAR);
	ArrResourceID.Add(IDC_edtSEARCH_PUBYEAR);
	ArrResourceID.Add(IDC_stcISBN);
	ArrResourceID.Add(IDC_edtSEARCH_ISBN);
	ArrResourceID.Add(IDC_stcGROUP2);
	ArrResourceID.Add(IDC_radSEARCH_MYDATA);
	ArrResourceID.Add(IDC_radSEARCH_ALL);
	ArrResourceID.Add(IDC_stcSEARCH_DONATOR);
	ArrResourceID.Add(IDC_edtSEARCH_DONATOR);
	ArrResourceID.Add(IDC_btnFIND_DONATOR);
	ArrResourceID.Add(IDC_stcGROUP3);
	ArrResourceID.Add(IDC_stcFORMCODE);
	ArrResourceID.Add(IDC_cmbSEARCH_FORM_CODE);
	ArrResourceID.Add(IDC_stcUSEOBJCODE);
	ArrResourceID.Add(IDC_cmbSEARCH_USER_OBJ_CODE);
	ArrResourceID.Add(IDC_stcINPUTTYPE);
	ArrResourceID.Add(IDC_cmbSEARCH_INPUT_TYPE);
	ArrResourceID.Add(IDC_stcSUBJECTCODE);
	ArrResourceID.Add(IDC_cmbSEARCH_SUBJECT_CODE);
	ArrResourceID.Add(IDC_stcWORK_CODE);
	ArrResourceID.Add(IDC_cmbSEARCH_JOB_CLASS);
	ArrResourceID.Add(IDC_stcCLASSNOTYPE);
	ArrResourceID.Add(IDC_cmbSEARCH_CLASS_NO_TYPE);

	// Show / Hide Code Resource
	for (i = 0; i < ArrResourceID.GetSize() ; i++)
	{
		pWnd = GetDlgItem(ArrResourceID.GetAt(i));
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (m_bShowMode)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	if (m_bShowMode)
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);

	ArrResourceID.RemoveAll();
	ArrResourceID.Add(IDC_stcSORT_CONDITION_GROUP);
	ArrResourceID.Add(IDC_chkSORT_YN);
	ArrResourceID.Add(IDC_btnSET_SORT_CONDITION);
	for(i=0 ; i<ArrResourceID.GetSize() ; i++)
	{
		pWnd = GetDlgItem(ArrResourceID.GetAt(i));
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->GetWindowRect(&rcWindow);
			ScreenToClient(rcWindow);
			
			if (!m_bShowMode)
			{
				rcWindow.top -= nCodeResourcesHeight;
				rcWindow.bottom -= nCodeResourcesHeight;
				pWnd->MoveWindow(rcWindow);
			}
			else 
			{
				rcWindow.top += nCodeResourcesHeight;
				rcWindow.bottom += nCodeResourcesHeight;
				pWnd->MoveWindow(rcWindow);
			}
		}		
	}
	
	//================================================================
	//2008.07.16 ADD BY PJW : �з�ǥ���п� ���� ���������� ������ ������ ����Ʈ���� �������� ����
	CString strSubCode;
	if( m_bShowMode == TRUE )	
	{
		CMulitComboBoxManager mgr(this);
		m_pInfo->pCodeMgr->GetDefaultCode(_T("KDC��������ȣ����"), strSubCode);
		if( strSubCode.IsEmpty() )
		{
			strSubCode.Format(_T("  "));
		}
		mgr.SetSubjectCodeData(m_pCM->CM_ALIAS, _T("�з�ǥ����_�ڵ�"), _T("��������_�ڵ�"), strSubCode);//_T("  "));
	}
	//================================================================

	Invalidate();
	UpdateWindow();
}

VOID CBO_ACQ_DonateDataSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString m_Use_Obj_Code = _T("");
	CString m_Form_Code = _T("");
	CString m_ISBN = _T("");
	CString m_Publisher = _T("");
	CString m_Author = _T("");
	CString m_Title = _T("");
	CString m_Date2 = _T("");
	CString m_Date1 = _T("");
	CString m_Acquisit_Year = _T("");
	CString m_Classfy_Code = _T("");
	CString m_Work_Code = _T("");
	CString m_PubYear = _T("");
	CString m_Subject_Code = _T("");
	CString m_Input_Type = _T("");
	CString m_Donator = _T("");
	CString m_SubRegNo = _T("");
		
	INT ids ;
	// IDX_BO_TBL�� SPECIES_CLASS 0:������, 1:�۾���, 2:��Ȱ��, 3:������
	CString strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND P.DONATOR_KEY = C.REC_KEY(+) AND I.SPECIES_CLASS IN ('0', '1') AND I.MANAGE_CODE = UDF_MANAGE_CODE");
	CString strWorkingStatus = _T("");
	if(GetOpenMode() == BO_ACQ_DONATE) strWorkingStatus = _T("BOA211O");
	else if(GetOpenMode() == BO_ACQ_NONDONATE) strWorkingStatus = _T("BOA212O");

	CString errstr, result;
	
	m_pCM->GetControlMgrData(_T("���Գ⵵"), m_Acquisit_Year);
	TrimAll(m_Acquisit_Year);
	
	ids = pMakeSearchData->IsValidYearData(m_Acquisit_Year);
	if( ids < 0)
	{
		errstr.Format(_T("�Է��Ͻ� ���Գ⵵ ��(%s)�� �ùٸ��� �ʽ��ϴ�."), m_Acquisit_Year);
		ESLAfxMessageBox(errstr.GetBuffer(0));
		m_pCM->SetControlMgrData(_T("���Գ⵵"), GetTodayYear());		
		return;
	}
	
	m_pCM->GetControlMgrData(_T("�Է���1"), m_Date1);
	m_pCM->GetControlMgrData(_T("�Է���2"), m_Date2);
	TrimAll(m_Date1);		TrimAll(m_Date2);
	if(m_Date1.IsEmpty() && !m_Date2.IsEmpty())
	{
		this->ESLAfxMessageBox(_T("�˻� �������� �Է��� �ּ���."));
		return;
	}
	m_pCM->GetControlMgrData(_T("�����"), m_PubYear);
	TrimAll(m_PubYear);
	ids = pMakeSearchData->IsValidYearData(m_PubYear);
	if( ids < 0)
	{
		errstr.Format(_T("�Է��Ͻ� ����⵵ ��(%s)�� �ùٸ��� �ʽ��ϴ�."), m_PubYear);
		ESLAfxMessageBox(errstr.GetBuffer(0));
		m_pCM->SetControlMgrData(_T("�����"), GetTodayYear());
		return;
	}

	m_pCM->GetControlMgrData(_T("�׷��ȣ"), m_strSelDivNo);
	TrimAll(m_strSelDivNo);
	//�Էµ� �׷����� �˻�
	CString strMsg;
	//m_strSelDivNo = CheckValidateGroupNo(m_pCM, m_strSelDivNo, m_Acquisit_Year);
/*	if(m_strSelDivNo.IsEmpty()) 
	{
		strMsg = _T("������ȣ������ ������ �ֽʽÿ�.");
		ESLAfxMessageBox(strMsg);
		return;
	}
*/	// �׷��÷��� ����
	if(!m_strSelDivNo.IsEmpty())
		this->MakeGroupInfo(m_Acquisit_Year, m_strSelDivNo);

	m_pCM->GetControlMgrData(_T("����"), m_Title);
	m_pCM->GetControlMgrData(_T("����"), m_Author);
	m_pCM->GetControlMgrData(_T("������"), m_Publisher);
	m_pCM->GetControlMgrData(_T("ISBN"), m_ISBN);
	m_pCM->GetControlMgrData(_T("���ı���_�ڵ�"), m_Form_Code);
	m_pCM->GetControlMgrData(_T("�̿��󱸺�_�ڵ�"), m_Use_Obj_Code);
	m_pCM->GetControlMgrData(_T("��������_�ڵ�"), m_Subject_Code);
	m_pCM->GetControlMgrData(_T("�Է±���_�ڵ�"), m_Input_Type);
	m_pCM->GetControlMgrData(_T("������"), m_Donator);
	// 2005-03-10
	m_pCM->GetControlMgrData(_T("�з�ǥ����_�ڵ�"), m_Classfy_Code);
	m_pCM->GetControlMgrData(_T("��������_�ڵ�"), m_Work_Code);
	m_pCM->GetControlMgrData(_T("������ϱ���_�ڵ�"), m_SubRegNo);
	
	TrimAll(m_Title);
	TrimAll(m_Author);
	TrimAll(m_PubYear);
	TrimAll(m_ISBN);
	TrimAll(m_Form_Code);
	TrimAll(m_Use_Obj_Code);
	TrimAll(m_Subject_Code);
	TrimAll(m_Input_Type);
	TrimAll(m_Donator);
	// 2005-03-10
	TrimAll(m_Classfy_Code);
	TrimAll(m_Work_Code);
	TrimAll(m_SubRegNo);

	//��ǥ��, ������, ������ ���ξ� ������������ ��ȯ
	pMakeSearchData->GetKeyWord(m_Title);		pMakeSearchData->ChangeHanjaToHangul(m_Title);
	pMakeSearchData->GetKeyWord(m_Author);	pMakeSearchData->ChangeHanjaToHangul(m_Author);
	pMakeSearchData->GetKeyWord(m_Publisher);	pMakeSearchData->ChangeHanjaToHangul(m_Publisher);
	pMakeSearchData->GetKeyWord(m_ISBN);

	CString strOption, strAcqCode;

//å���� ����	
	strOption.Format(_T(" AND B.WORKING_STATUS = '%s' AND B.ACQ_CODE = '2'"), strWorkingStatus);
	strWhere += strOption;
	
	strOption = MakeDateCondition_Stmt(_T("B.INPUT_DATE"), m_Date1, m_Date2);
	if(strOption == _T("-1")) return ;
	strWhere += strOption;

//������������ ����
	CString strAccuWhereStmt;
	if(!m_Subject_Code.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND P.SUBJECT_CODE = '%s'"), m_Subject_Code);
		strAccuWhereStmt += strOption;
	}
	if(!m_Classfy_Code.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND I.CLASS_NO_TYPE = '%s'"), m_Classfy_Code);
		strAccuWhereStmt += strOption;
	}
	if(!m_Input_Type.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND P.INPUT_TYPE = '%s'"), m_Input_Type);
		strAccuWhereStmt += strOption;
	}
	if(!m_Acquisit_Year.IsEmpty())
	{
		strOption.Format(_T(" AND P.ACQ_YEAR = '%s'"), m_Acquisit_Year);
		strAccuWhereStmt += strOption;
	}

	if(!m_Donator.IsEmpty() && m_bShowMode)
	{
		CESL_DataMgr dm;
		CString strDonatorCnt, sQuery;
		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		sQuery.Format(_T("SELECT COUNT(REC_KEY) FROM CO_DONATOR_TBL WHERE DONATOR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),m_Donator);
		dm.GetOneValueQuery(sQuery,strDonatorCnt);
		INT nCnt = _ttoi(strDonatorCnt);
		
		if(m_strDonatorKey.IsEmpty() && nCnt > 1 )
		{
			AfxMessageBox(_T("�ߺ��Ǿ��ų� ���� �������Դϴ�. \n\r �˻��� ���� �������ּ���!"));
			return;
		}

		if(m_strDonatorKey.IsEmpty())
		{
			strOption.Format(_T(" AND P.DONATOR_NAME LIKE '%s%%' "), m_Donator);
			strAccuWhereStmt += strOption;
		}
		else
		{
			strOption.Format(_T(" AND P.DONATOR_NAME LIKE '%s%%' AND P.DONATOR_KEY = '%s' "), m_Donator, m_strDonatorKey);
			strAccuWhereStmt += strOption;
		}
	}	
	if(!m_strSelDivNo.IsEmpty())
	{
		strOption = _T(" AND (") + MakeGroupInfoOrQuery(m_strSelDivNo) + _T(")");
		strAccuWhereStmt += strOption;
	}
	
//���ΰ��� ����
//#ifdef _ORA_CATSEARCH_
	if(m_pInfo->GetSearchIndexType() == ORA_CATSEARCH)
	{
		if(!(m_Title.IsEmpty()))
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_TITLE, '%s*', NULL)>0"), m_Title);
			strWhere += strOption;
		}
		if(!(m_Author.IsEmpty()) && m_bShowMode)
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_AUTHOR, '%s*', NULL)>0"), m_Author);
			strWhere += strOption;
		}
		if(!(m_Publisher.IsEmpty()) && m_bShowMode)
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_PUBLISHER, '%s*', NULL)>0"), m_Publisher);
			strWhere += strOption;
		}
		if(!(m_ISBN.IsEmpty()) && m_bShowMode)
		{
			strOption.Format(_T(" AND CATSEARCH(I.ST_CODE, '%s*', NULL)>0"), m_ISBN);
			strWhere += strOption;
		}
	}
//#else
	else if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		m_arrECOSEARCH.RemoveAll();
		if(!(m_Title.IsEmpty()))
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*') "), m_Title);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}
		if(!(m_Author.IsEmpty()) && m_bShowMode)
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*') "), m_Author);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}
		if(!(m_Publisher.IsEmpty()) && m_bShowMode)
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_IDX_PUBLISHER, '%s*') "), m_Publisher);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}
		if(!(m_ISBN.IsEmpty()) && m_bShowMode)
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_ST_CODE, '%s*') "), m_ISBN);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}

		if ( m_bOpenbase )
		{			
			for ( int i=0 ; i<m_arrECOSEARCH.GetSize() ; i++ )
			{
				strOption = m_arrECOSEARCH.GetAt(i);
				strOption.Replace(_T("AND"), _T("INTERSECT"));
				strWhere += strOption;
			}			
		}
		else
		{		
			ids = GetECOSEARCHQuery(strOption);
			if(ids < 0) 
			{
				ESLAfxMessageBox(_T("�˻� ������ ������ �� �����ϴ�."));
				return;
			}
			if(!strOption.IsEmpty())
			{
				strWhere += _T(" AND I.REC_KEY IN ( ") + strOption + _T(")");
			}
		}
	}
//#endif

	if(!(m_PubYear.IsEmpty()) && m_bShowMode)
	{
		strOption.Format(_T(" AND I.PUB_YEAR = '%s'"), m_PubYear);
		strWhere += strOption;
	}
	if(!m_Form_Code.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND I.FORM_CODE = '%s'"), m_Form_Code);
		strWhere += strOption;
	}
	if(!m_Use_Obj_Code.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND I.USE_OBJ_CODE = '%s'"), m_Use_Obj_Code);
		strWhere += strOption;
	}
	if (!(m_Work_Code.IsEmpty()) && m_bShowMode && GetAcqCode() == DONATE)
	{
		strOption.Format(_T(" AND I.WORK_CODE = '%s'"), m_Work_Code);
		strWhere += strOption;
	}
	if (!(m_SubRegNo.IsEmpty()) && m_bShowMode)
	{
		strOption.Format(_T(" AND I.SUB_REG_CODE = '%s'"), m_SubRegNo);
		strWhere += strOption;			
	}
	

	// ����/��������/å ���� ���� ����
	strWhere += strAccuWhereStmt;
	SetWhereStmt(strWhere);
	// ���� ������ �θ𿡰� �޽����� ������.
	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

VOID CBO_ACQ_DonateDataSearchDlg::ShowGroupInfo()
{
	CString strAcqYear;
	strAcqYear = m_pGroups->GetAcqYear();
	if(strAcqYear.IsEmpty()) strAcqYear = GetTodayYear();
	m_pGroups->GetGroupInfo(m_strSelDivNo);
	if(m_strSelDivNo == _T("��ü�ڷ�")) m_strSelDivNo = _T("");
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), m_strSelDivNo);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), strAcqYear);	
	GetDlgItem(IDC_edtSEARCH_DIVISIONNO)->SetFocus();
}

VOID CBO_ACQ_DonateDataSearchDlg::OnbtnSEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	ClearScreen(m_pCM);
	m_pGroups->RemoveAll();
	ShowGroupInfo();
}

VOID CBO_ACQ_DonateDataSearchDlg::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	ShowDataSortDlg();
}

VOID CBO_ACQ_DonateDataSearchDlg::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton *)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;	
}

VOID CBO_ACQ_DonateDataSearchDlg::OnSetfocusedtSEARCHAUTHOR() 
{
	SetIME(IDC_edtSEARCH_AUTHOR, 1);
}

VOID CBO_ACQ_DonateDataSearchDlg::OnSetfocusedtSEARCHDONATOR() 
{
	SetIME(IDC_edtSEARCH_DONATOR, 1);
}

VOID CBO_ACQ_DonateDataSearchDlg::OnSetfocusedtSEARCHPUBLISHER() 
{
	SetIME(IDC_edtSEARCH_PUBLISHER, 1);
}

VOID CBO_ACQ_DonateDataSearchDlg::OnSetfocusedtSEARCHTITLE() 
{
	SetIME(IDC_edtSEARCH_TITLE, 1);
}

HBRUSH CBO_ACQ_DonateDataSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
