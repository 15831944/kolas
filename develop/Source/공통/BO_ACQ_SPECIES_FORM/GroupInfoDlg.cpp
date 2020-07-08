// GroupInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GroupInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupInfoDlg dialog


CGroupInfoDlg::CGroupInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CInfoDlg(pParent, CGroupInfoDlg::IDD)
{
	//{{AFX_DATA_INIT(CGroupInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pGroup = NULL;
}

CGroupInfoDlg::~CGroupInfoDlg()
{

}



VOID CGroupInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupInfoDlg)
	ON_EN_KILLFOCUS(IDC_edtBSFORM_CREATE_YEAR, OnKillfocusedtBSFORMCREATEYEAR)
	ON_CBN_SELCHANGE(IDC_cmbBSFORM_GROUP_NAME, OnSelchangecmbBSFORMGROUPNAME)
	ON_BN_CLICKED(IDC_btnBSFORM_GROUP_INSERT, OnbtnBSFORMGROUPINSERT)
	ON_CBN_SELCHANGE(IDC_cmbBSGROUP_ACQ_CODE, OnSelchangecmbBSFORMACQCODE)
	ON_MESSAGE(SET_TAB_ORDER, OnSetTabOrder)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupInfoDlg message handlers

BOOL CGroupInfoDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CGroupInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_����_��_�׷�����")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM����
	m_pCM = FindCM(_T("CM_����_��_�׷�����"));	
	this->CONNECTION_INFO = GetDM()->CONNECTION_INFO;
	GetDM()->InsertRow(-1);
	if(!Init())
	{
		ESLAfxMessageBox(_T("�׷����� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CGroupInfoDlg::Init()
{
	//����/������ȣ ���� ���� �� �ʱ� ������ ����
	CString acqCode[4][2] = 
	{
		_T(" "), _T("�������"),
		_T("1"), _T("����"),
		_T("2"), _T("����"),
		_T("3"), _T("��ȯ")
	};
	CString strTmpData;
	if(GetAcqType() == ACQ_TYPE_INVALID) strTmpData = _T("");
	else strTmpData.Format(_T("%d"), GetAcqType());
	m_pCM->SetControlMgrData(_T("BP_���Ա���_�ڵ�"), strTmpData);

	if(GetOpType() == SINSERT)
	{
		if(GetGroupCollection())
		{
			strTmpData = GetGroupCollection()->GetAcqYear();
			if(strTmpData.IsEmpty()) strTmpData = GetTodayYear();
			m_pCM->SetControlMgrData(_T("BP_���Գ⵵"), strTmpData);
		}
	}
	
	if(GetSpeciesType() == BO_BASIC || GetSpeciesType() == BO_IMPORT)
	{

		if(GetGroupCollection()->GetHaveGroupInfo())
		{
			((CEdit*)GetDlgItem(IDC_edtBSFORM_CREATE_YEAR))->SetReadOnly(TRUE);
			m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
			GetDM()->SetCellData(_T("BP_���Գ⵵"), m_pGroup->GetAcqYear(), 0);
		}

		GetDlgItem(IDC_cmbBSGROUP_ACQ_CODE)->EnableWindow(FALSE);
		MakeGroupNameList();
		OnKillfocusedtBSFORMCREATEYEAR();
		OnSelchangecmbBSFORMGROUPNAME();
	}
	else if(GetSpeciesType() == BO_REG_INVALID)
	{
		GetDlgItem(IDC_stcGROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBSFORM_CREATE_YEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcGROUP_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbBSFORM_GROUP_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnBSFORM_GROUP_INSERT)->ShowWindow(SW_HIDE);
		if(GetOpType() != SINSERT)
			GetDlgItem(IDC_cmbBSGROUP_ACQ_CODE)->EnableWindow(FALSE);
	}

	return TRUE;
}

INT CGroupInfoDlg::MakeGroupNameList()
{
	// -- ���Գ⵵�� ���� �׷��̸�/��ȣ ������ �����Ѵ�.
	CString strAcqYear, strGroupNo, strGroupName;
	INT ids;
	GetDM()->GetCellData(_T("BP_���Գ⵵"), 0, strAcqYear);
	GetDM()->GetCellData(_T("BP_�׷��ȣ"), 0, strGroupNo);
	if(strAcqYear.IsEmpty())
		strAcqYear = GetTodayYear();
	if(!GetGroupCollection()->GetHaveGroupInfo())
		GetGroupCollection()->MakeGroupInfo(strAcqYear);
	if(GetGroupCollection()->GetGroupCount() > 0)
	{
		ids = GetGroupCollection()->SetCurSelGroupInfo(strAcqYear, strGroupNo, _T(""));
		if(ids < 0) return 0;
		m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
		if(m_pGroup)
		{
			CStringArray Array;
			CString strCurSelInfo;
			GetGroupCollection()->GetGroupInfo(&Array, strCurSelInfo);
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &Array, _T("�׷��̸�"));
		}
	}
	
	return 0;
}

//DEL INT CGroupInfoDlg::MakeLastSerialNo()
//DEL {
//DEL 	CString strAcqYear, strGroupNo, strWorkNo;
//DEL 	GetDM()->GetCellData(_T("BP_���Գ⵵"), 0, strAcqYear);
//DEL 	GetDM()->GetCellData(_T("BP_�׷��ȣ"), 0, strGroupNo);
//DEL 	
//DEL 	CGroupMgr groupMgr(this);
//DEL 	groupMgr.InitTempData();
//DEL 	INT tmpData = (INT)ACQ_TYPE_INVALID;
//DEL 	groupMgr.SetTempData(_T("ACQ_TYPE"), tmpData);
//DEL 	groupMgr.SetTempData(_T("ACQ_YEAR"), strAcqYear.GetBuffer(0));
//DEL 	groupMgr.SetTempData(_T("Group_Info"), strGroupNo.GetBuffer(0));
//DEL 	groupMgr.SetTempData(_T("Work_Info"), _T(""));
//DEL 	INT ids = groupMgr.SPFExecutePath(_T("�������Ϸù�ȣ����"));
//DEL 	if(ids < 0) return ids;
//DEL 
//DEL 	CString strLastNo;
//DEL 	groupMgr.GetTempData(_T("Last_Serial_No"), strLastNo);
//DEL 	GetDM()->SetCellData(_T("BP_�����Ϸù�ȣ"), strLastNo, 0);
//DEL 	return 0;
//DEL }

INT CGroupInfoDlg::FindSelIndex(CStringArray * parrGroupNo, CString strGroupNo)
{
	INT cnt = parrGroupNo->GetSize();
	CString strTmpData;
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = parrGroupNo->GetAt(i);
		if(strGroupNo == strTmpData) return i;
	}
	return -1;
}

VOID CGroupInfoDlg::OnKillfocusedtBSFORMCREATEYEAR() 
{
	// TODO: Add your control notification handler code here
	if(GetDlgItem(IDC_edtBSFORM_CREATE_YEAR)->GetStyle() & ES_READONLY) return ;

	CString strAcqYear, strSrcAcqYear;
	m_pCM->GetControlMgrData(_T("BP_���Գ⵵"), strAcqYear);
	GetDM()->GetCellData(_T("BP_���Գ⵵"), 0, strSrcAcqYear);
	if(strSrcAcqYear != strAcqYear)
	{
		GetDM()->SetCellData(_T("BP_���Գ⵵"), strAcqYear, 0);
		GetGroupCollection()->RemoveAll();
		INT ids = MakeGroupNameList();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("�׷������� ������ �� �����ϴ�."));
			return;
		}		
	}
}

VOID CGroupInfoDlg::OnSelchangecmbBSFORMGROUPNAME() 
{
	// TODO: Add your control notification handler code here
	CString strGroupName, strLastNo;
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�׷��̸�"));
	INT nCurSel = pCmb->GetCurSel();
	GetGroupCollection()->SetCurSel(nCurSel);
	m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
}

VOID CGroupInfoDlg::OnbtnBSFORMGROUPINSERT() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strAddGroupName, strAddLastNo, strAddGroupNo;
	GetDM()->GetCellData(_T("BP_���Գ⵵"), 0, strAcqYear);
	CGroup_Insert_Dlg dlg(this);
	dlg.SetOpenMode(DINSERT);
	dlg.SetAcqYear(strAcqYear);
	if(IDOK == dlg.DoModal())
	{
		dlg.GetInsertInfo(strAddGroupNo, strAddGroupName, strAddLastNo);
		GetGroupCollection()->AddGroupInfo(strAcqYear, strAddGroupName, strAddGroupNo);
		m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
		if(m_pGroup)
		{
			m_pGroup->SetLastSerialNo(_ttoi(strAddLastNo.GetBuffer(0)));
			m_pGroup->SetHaveLastSerialNo(TRUE);
			CStringArray Array;
			GetGroupCollection()->GetGroupInfo(&Array, strAddLastNo);
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strAddGroupName, &Array, _T("�׷��̸�"));
		}
	}
}

INT CGroupInfoDlg::ValidateData()
{
	return 0;
}

INT CGroupInfoDlg::ApplyData()
{
//	OnSelchangecmbBSFORMGROUPNAME();
	if(m_pGroup != NULL)
	{
		GetDM()->SetCellData(_T("BP_���Գ⵵"), m_pGroup->GetAcqYear(), 0);
		GetDM()->SetCellData(_T("BP_�׷��ȣ"), m_pGroup->GetGroupNo(), 0);
	}
	return 0;
}

INT CGroupInfoDlg::Display(INT idx)
{
	m_pCM->AllControlDisplay(idx);
	
	CString strAcqYear, strGroupNo;
	GetDM()->GetCellData(_T("BP_���Գ⵵"), 0, strAcqYear);
	GetDM()->GetCellData(_T("BP_�׷��ȣ"), 0, strGroupNo);
	if(strAcqYear.IsEmpty())
		strAcqYear = GetTodayYear();
	//���Գ⵵����
	m_pCM->SetControlMgrData(_T("BP_���Գ⵵"), strAcqYear);
	
	INT ids;
	if(GetGroupCollection()->GetGroupCount() > 0)
	{
		ids = GetGroupCollection()->SetCurSelGroupInfo(strAcqYear, strGroupNo, _T(""));
		if(ids < 0) return 0;
		m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
		if(m_pGroup)
		{
			CStringArray Array;
			CString strCurSelInfo;
			GetGroupCollection()->GetGroupInfo(&Array, strCurSelInfo);
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &Array, _T("�׷��̸�"));
		}
	}
	return 0;
}

VOID CGroupInfoDlg::OnSelchangecmbBSFORMACQCODE() 
{
	// TODO: Add your control notification handler code here
	CString strAcqCode;
	m_pCM->GetControlMgrData(_T("BP_���Ա���_�ڵ�"), strAcqCode);
	if(strAcqCode == _T("1")) SetAcqType(PURCHASE);
	else if(strAcqCode == _T("2")) SetAcqType(DONATE);
	else if(strAcqCode.IsEmpty()) SetAcqType(ACQ_TYPE_INVALID);
	else SetAcqType(OTHER_ACQ_TYPE);
	
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("BP_���Ա���_�ڵ�"));
	if(pCMB) pCMB->ShowDropDown(FALSE);
	
	::PostMessage(pParentMgr->GetSafeHwnd(), ACQ_TYPE_CHANGE, _ttoi(strAcqCode.GetBuffer(0)), 0);	
}

INT CGroupInfoDlg::UpdateWorkSerialNo(CESL_DataMgr *pDM_OUT)
{
	CString strAcqYear, strDivNo, strWorkNo, strLastSerialNo;
	GetDM()->GetCellData(_T("BP_���Գ⵵"), 0, strAcqYear);
	GetDM()->GetCellData(_T("BP_�׷��ȣ"), 0, strDivNo);
	GetDM()->GetCellData(_T("BP_�����Ϸù�ȣ"), 0, strLastSerialNo);

	INT cnt = 0;
	INT rowCnt = GetBookDM()->GetRowCount();
	INT nNUM = 0;
	if( m_pGroup ) nNUM = m_pGroup->GetLastSerialNo((INT)GetAcqType(), GetDM());
	CString strTmpData, strDBFLAG, strAcqCode;
	if(GetAcqType() == ACQ_TYPE_INVALID) strAcqCode = _T("");
	else if(GetAcqType() == EXCHANGE) strAcqCode = _T("3");

	//åDM�� ����-������ȣ ������ Sort�ؾ� �Ѵ�.
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_������ȣ");						nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	GetBookDM()->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);

	for(INT i=0 ; i<rowCnt ; i++)
	{
		strDBFLAG = GetBookDM()->GetCellData(_T("BB_DB_FLAG"), i);
		if(strDBFLAG == _T("D")) continue;
		if(GetOpType() == SINSERT)
		{
			if(strTmpData != _T("I")) GetBookDM()->SetCellData(_T("BB_DB_FLAG"), _T("I"), i);
			if(m_pGroup)
			{
				if(m_pGroup->GetGroupInfo() != _T("�������"))
				{
					GetBookDM()->SetCellData(_T("BB_���Ա���_�ڵ�"), strAcqCode, i);
					GetBookDM()->SetCellData(_T("BB_�׷��ȣ"), m_pGroup->GetGroupNo(), i);
					if(m_pGroup->GetGroupNo() != _T("0"))
						strTmpData.Format(_T("%d"), ++nNUM);
					else
						strTmpData.Format(_T("%d"), nNUM);
					GetBookDM()->SetCellData(_T("BB_�Ϸù�ȣ"), strTmpData, i);
					GetBookDM()->SetCellData(_T("BB_���Գ⵵"), GetTodayYear(), i);
				}
				else 
				{
					GetBookDM()->SetCellData(_T("BB_���Ա���_�ڵ�"), strAcqCode, i);
					GetBookDM()->SetCellData(_T("BB_�׷��ȣ"), _T("0"), i);
					strTmpData.Format(_T("%d"), nNUM);
					GetBookDM()->SetCellData(_T("BB_�Ϸù�ȣ"), strTmpData, i);
					GetBookDM()->SetCellData(_T("BB_���Գ⵵"), GetTodayYear(), i);
				}
			}
			else
			{
				GetBookDM()->SetCellData(_T("BB_���Ա���_�ڵ�"), strAcqCode, i);
				GetBookDM()->SetCellData(_T("BB_�׷��ȣ"), _T("0"), i);
				//strTmpData.Format(_T("%d"), nNUM);
				strTmpData = _T("esl_seq.NEXTVAL");
				GetBookDM()->SetCellData(_T("BB_�Ϸù�ȣ"), strTmpData, i);
				GetBookDM()->SetCellData(_T("BB_���Գ⵵"), GetTodayYear(), i);
			}
		}
		else if(GetOpType() != SINSERT)
		{
			if(m_pGroup)
			{
				strTmpData = GetBookDM()->GetCellData(_T("BB_�׷��ȣ"), i);
				if(strTmpData == strDivNo && strDBFLAG != _T("I")) continue;
				else
				{
					GetBookDM()->SetCellData(_T("BB_���Ա���_�ڵ�"), strAcqCode, i);
					GetBookDM()->SetCellData(_T("BB_�׷��ȣ"), m_pGroup->GetGroupNo(), i);
					if(m_pGroup->GetGroupNo() != _T("0")) 
						strTmpData.Format(_T("%d"), ++nNUM);
					else
						strTmpData.Format(_T("%d"), nNUM);
					GetBookDM()->SetCellData(_T("BB_�Ϸù�ȣ"), strTmpData, i);
					GetBookDM()->SetCellData(_T("BB_���Գ⵵"), GetTodayYear(), i);
					if(strDBFLAG.IsEmpty()) GetBookDM()->SetCellData(_T("BB_DB_FLAG"), _T("U"), i);
				}
			}
			else 
			{
				if(strDBFLAG == _T("I")) 
				{
					GetBookDM()->SetCellData(_T("BB_���Ա���_�ڵ�"), strAcqCode, i);
					GetBookDM()->SetCellData(_T("BB_�׷��ȣ"), _T("0"), i);
					//strTmpData.Format(_T("%d"), nNUM);
					strTmpData = _T("esl_seq.NEXTVAL");
					GetBookDM()->SetCellData(_T("BB_�Ϸù�ȣ"), strTmpData, i);
					GetBookDM()->SetCellData(_T("BB_���Գ⵵"), GetTodayYear(), i);
				}
			}
		}
	}

	if(m_pGroup)
	{
		m_pGroup->SetLastSerialNo(nNUM);
		m_pGroup->UpdateLastSerialNo(_T(""), this, pDM_OUT);
	}

	return 0;
}

INT CGroupInfoDlg::InsertWorkNoInfo(CESL_DataMgr * pDM_OUT)
{
//	if(GetSpeciesType() == BO_BASIC || GetSpeciesType() == BO_IMPORT )
		return UpdateWorkSerialNo(pDM_OUT);
//	return 0;
}

BOOL CGroupInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
		else if(pMsg->wParam == VK_TAB)
		{
			if(pMsg->hwnd == GetDlgItem(IDC_btnBSFORM_GROUP_INSERT)->GetSafeHwnd())
			{
				pParentMgr->PostMessage(SET_TAB_ORDER, 0, 0);
				return TRUE;
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_cmbBSGROUP_ACQ_CODE)->GetSafeHwnd() && !GetDlgItem(IDC_btnBSFORM_GROUP_INSERT)->IsWindowVisible())
			{	
				pParentMgr->PostMessage(SET_TAB_ORDER, 0, 0);
				return TRUE;
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

CGroupInfo * CGroupInfoDlg::GetGroupInfo()
{
	return m_pGroup;
}

UINT CGroupInfoDlg::OnSetTabOrder(LPARAM lParam, WPARAM wParam)
{
	if(GetDlgItem(IDC_cmbBSGROUP_ACQ_CODE)->IsWindowEnabled())
		GetDlgItem(IDC_cmbBSGROUP_ACQ_CODE)->SetFocus();
	else
		GetDlgItem(IDC_edtBSFORM_CREATE_YEAR)->SetFocus();
	
	return 0;
}

HBRUSH CGroupInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
