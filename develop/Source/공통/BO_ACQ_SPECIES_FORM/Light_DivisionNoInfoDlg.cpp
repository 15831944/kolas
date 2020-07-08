// Light_DivisionNoInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Light_DivisionNoInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLight_DivisionNoInfoDlg dialog


CLight_DivisionNoInfoDlg::CLight_DivisionNoInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CInfoDlg(pParent, CLight_DivisionNoInfoDlg::IDD)
{
	//{{AFX_DATA_INIT(CLight_DivisionNoInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pGroup = NULL;
}


VOID CLight_DivisionNoInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLight_DivisionNoInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLight_DivisionNoInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CLight_DivisionNoInfoDlg)
	ON_EN_KILLFOCUS(IDC_edtBSDIVNO_ACQ_YEAR, OnKillfocusedtBSDIVNOACQYEAR)
	ON_CBN_SELCHANGE(IDC_cmbBSDIVNO_GROUP_NO, OnSelchangecmbBSDIVNOGROUPNO)
	ON_CBN_SELCHANGE(IDC_cmbBSDIVNO_WORK_NO, OnSelchangecmbBSDIVNOWORKNO)
	ON_BN_CLICKED(IDC_btnBSDIVNO_CREATE, OnbtnBSDIVNOCREATE)
	ON_MESSAGE(CHANGE_WORK_CODE, OnChangeWorkCode)
	ON_MESSAGE(SET_TAB_ORDER, OnSetTabOrder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLight_DivisionNoInfoDlg message handlers

BOOL CLight_DivisionNoInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_����_��_����������")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM����
	m_pCM = FindCM(_T("CM_����_��_����������"));	
	this->SetCONNECTION_INFO(GetDM()->CONNECTION_INFO);
	
	m_pCM->AllControlDisplay();
	if(GetDM()->GetRowCount() == 0) 
	{
		GetDM()->InsertRow(-1);
		GetDM()->SetCellData(_T("BP_������������"), _T("1"), 0);
	}
	if(!Init())
	{
		ESLAfxMessageBox(_T("����/���� ���� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CLight_DivisionNoInfoDlg::Init()
{
	CString strTmpData;
	strTmpData.Format(_T("%d"), GetAcqType());
	m_pCM->SetControlMgrData(_T("BP_���Ա���_�ڵ�"), strTmpData);
	
	CESL_ControlComboBox * pElement = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_���Ա���_�ڵ�"));
	pElement->EnableWindow(FALSE);
	
	if(GetOpType() == SINSERT)
	{
		if(GetGroupCollection())
		{
			strTmpData = GetGroupCollection()->GetAcqYear();
			if(strTmpData.IsEmpty()) strTmpData = GetTodayYear();
			m_pCM->SetControlMgrData(_T("BP_���Գ⵵"), strTmpData);
		}
	}
	//����/������ȣ ���� ��Ʈ�� ����
	SetCtrlByAcqCode();
	//����/������ȣ ���� ���� �� �ʱ� ������ ����
	if(GetGroupCollection()->GetHaveGroupInfo())
	{
		((CEdit*)GetDlgItem(IDC_edtBSDIVNO_ACQ_YEAR))->SetReadOnly(TRUE);
	}
	
	INT ids = MakeGroupNoList();
	if(ids < 0) return FALSE;
	
	//�������������� �����Ѵ�. ����ÿ� ����ڰ� �ٸ� ������ �����ߴ����� �˾Ƴ��� ���ؼ� �̴�.
	//InsertWorkNo�޼ҵ� ����
	if(m_pGroup != NULL && GetOpType() != SINSERT)
		GetDM()->SetCellData(_T("BP_������������"), m_pGroup->GetWorkCode(), 0);
	
	return TRUE;
}

INT CLight_DivisionNoInfoDlg::MakeGroupNoList()
{
	CString strAcqYear, strGroupNo, strGroupName, strWorkNo, strWorkCode;
	GetDM()->GetCellData(_T("BP_���Գ⵵"), 0, strAcqYear);
	GetDM()->GetCellData(_T("BP_�׷��ȣ"), 0, strGroupNo);
	GetDM()->GetCellData(_T("BP_�۾���ȣ"), 0, strWorkNo);
	GetDM()->GetCellData(_T("BP_������������"), 0, strWorkCode);
	if(strAcqYear.IsEmpty())
		strAcqYear = GetTodayYear();
	if(GetGroupCollection()->GetGroupCount() == 0)
	{
		INT ids = GetGroupCollection()->MakeGroupInfo(strAcqYear, strGroupNo, strWorkCode);
		if(ids < 0) return -1;
		GetGroupCollection()->AddGroupInfo(strAcqYear, _T("�������"), strWorkCode);
	}
	
	m_pGroup = NULL;
	CStringArray tmpArray;	tmpArray.RemoveAll();
	
	if(GetGroupCollection()->GetGroupCount() > 0)
	{
		GetGroupCollection()->SetCurSelGroupInfo(strAcqYear, strGroupNo, strWorkNo, strWorkCode);
		m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
		if(m_pGroup)
		{
			CString strCurSelInfo;
			GetGroupCollection()->GetGroupInfo(&tmpArray, strCurSelInfo);
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &tmpArray, _T("BP_�׷��ȣ"));
			if(m_pGroup->GetGroupInfo() != _T("�������"))
			{
				CStringArray * pArray = m_pGroup->GetArrayWorkNo();
				SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetWorkNo(-1), pArray, _T("BP_�۾���ȣ"));
			}
			else
			{
				CStringArray WorkNoArray;	WorkNoArray.RemoveAll();
				SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &WorkNoArray, _T("BP_�۾���ȣ"));
			}
			SendPurchaseSeqNoChangeMessage();
		}
		else
		{
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("BP_�׷��ȣ"));
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("BP_�۾���ȣ"));
		}
	}
	else
	{
		SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("BP_�׷��ȣ"));
		SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("BP_�۾���ȣ"));
	}
	return 0;
}

VOID CLight_DivisionNoInfoDlg::SetCtrlByAcqCode()
{
	GetDlgItem(IDC_strBSFORM_GROUP_NO)->SetWindowText(_T("������ȣ"));
	GetDlgItem(IDC_strBSFORM_WORK_NO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_cmbBSDIVNO_WORK_NO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_btnBSDIVNO_CREATE)->SetWindowText(_T("������ȣ����"));
}

VOID CLight_DivisionNoInfoDlg::SendPurchaseSeqNoChangeMessage()
{
	if(m_pGroup == NULL) return;
	CString strWorkCode;
	strWorkCode = m_pGroup->GetWorkCode();
	::PostMessage(pParentMgr->GetSafeHwnd(), PURCHASE_SEQ_NO_CHANGE, _ttoi(strWorkCode.GetBuffer(0)), 0);
}

VOID CLight_DivisionNoInfoDlg::OnKillfocusedtBSDIVNOACQYEAR() 
{
	// TODO: Add your control notification handler code here
	if(GetDlgItem(IDC_edtBSDIVNO_ACQ_YEAR)->GetStyle() & ES_READONLY) return ;
	
	CString strAcqYear, strSrcAcqYear;
	m_pCM->GetControlMgrData(_T("BP_���Գ⵵"), strAcqYear);
	if(strAcqYear.IsEmpty()) 
	{
		ESLAfxMessageBox(_T("���Գ⵵ ������ ������ �ֽʽÿ�."));
		return ;
	}
	GetDM()->GetCellData(_T("BP_���Գ⵵"), 0, strSrcAcqYear);
	if(strSrcAcqYear != strAcqYear)
	{
		GetDM()->SetCellData(_T("BP_�׷��ȣ"), _T(""), 0);
		GetDM()->SetCellData(_T("BP_���Գ⵵"), strAcqYear, 0);
		GetGroupCollection()->RemoveAll();
		INT ids = MakeGroupNoList();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("���������� ������ �� �����ϴ�."));
			return;
		}		
	}	
}

VOID CLight_DivisionNoInfoDlg::OnSelchangecmbBSDIVNOGROUPNO() 
{
	// TODO: Add your control notification handler code here
	CString strWorkCode;
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_�׷��ȣ"));

	INT nCurSel = pCmb->GetCurSel();
	if(nCurSel < 0) return ;
	GetGroupCollection()->SetCurSel(nCurSel);
	m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
	if(m_pGroup && m_pGroup->GetGroupInfo() != _T("�������"))
	{
		CStringArray * pArray = m_pGroup->GetArrayWorkNo();
		SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetWorkNo(-1), pArray, _T("BP_�۾���ȣ"));
		strWorkCode = m_pGroup->GetWorkCode();
		SendPurchaseSeqNoChangeMessage();
	}
	else
	{
		CStringArray tmpArray;	tmpArray.RemoveAll();
		SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("BP_�۾���ȣ"));
	}
}

VOID CLight_DivisionNoInfoDlg::OnSelchangecmbBSDIVNOWORKNO() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_�۾���ȣ"));
	INT nCurSel = pCmb->GetCurSel();
	GetGroupCollection()->SetCurSelWorkNo(nCurSel);	
}

VOID CLight_DivisionNoInfoDlg::OnbtnBSDIVNOCREATE() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strAddGroupNo, strAddWorkNo, strAddLastNo, strWorkCode, strAddAcqCode;
	GetDM()->GetCellData(_T("BP_���Գ⵵"), 0, strAcqYear);
	if(strAcqYear.IsEmpty()) strAcqYear = GetTodayYear();
	CDivisionNo_Insert_Dlg dlg(this);
	dlg.SetOpenMode(DINSERT);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetCONNECTION_INFO(GetDM()->CONNECTION_INFO);
	dlg.SetWorkStatus(_T("���Դ��"));
	dlg.SetWorkCode(GetGroupCollection()->GetWorkCode());
	if(IDOK == dlg.DoModal())
	{
		dlg.GetInsertInfo(strAddGroupNo, strAddWorkNo, strAddLastNo, strWorkCode);
		GetGroupCollection()->AddGroupInfo(strAcqYear, strAddGroupNo, strWorkCode);
		m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
		if(m_pGroup) m_pGroup->SetLastSerialNo(_ttoi(strAddLastNo.GetBuffer(0)));
		CStringArray tmpArray;
		GetGroupCollection()->GetGroupInfo(&tmpArray, strAddLastNo);
		SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strAddGroupNo, &tmpArray, _T("BP_�׷��ȣ"));
		GetGroupCollection()->GetWorkNoInfo(&tmpArray);
		SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strAddWorkNo, &tmpArray, _T("BP_�۾���ȣ"));
		SendPurchaseSeqNoChangeMessage();
	}
}

BOOL CLight_DivisionNoInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
		else if(pMsg->wParam == VK_TAB && pMsg->hwnd == GetDlgItem(IDC_btnBSDIVNO_CREATE)->GetSafeHwnd())
		{
			pParentMgr->PostMessage(SET_TAB_ORDER, 0, 0);
			return TRUE;
		}
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

INT CLight_DivisionNoInfoDlg::InsertWorkNoInfo(CESL_DataMgr * pDM_OUT)
{
	BOOL bDivNoChange = FALSE;
	BOOL bWorkNoChange = FALSE;
	CString strTmpData;
	INT nPNum = -1;
	INT nBNum = -1;

	CString strAcqYear, strDivNo, strWorkNo, strWorkCode;
	GetDM()->GetCellData(_T("BP_���Գ⵵"), 0, strAcqYear);
	GetDM()->GetCellData(_T("BP_������������"), 0, strWorkCode);
	GetDM()->GetCellData(_T("BP_�׷��ȣ"), 0, strDivNo);
	GetDM()->GetCellData(_T("BP_�۾���ȣ"), 0, strWorkNo);
//	if(strWorkNo == _T("0")) strWorkNo = _T("������");

	if(GetOpType() == SINSERT) bDivNoChange = TRUE;

	if(strAcqYear != m_pGroup->GetAcqYear())
	{
		GetDM()->SetCellData(_T("BP_���Գ⵵"), m_pGroup->GetAcqYear(), 0);
		bDivNoChange = TRUE;
		bWorkNoChange = TRUE;
	}
	if(strWorkCode != m_pGroup->GetWorkCode())
	{
		bDivNoChange = TRUE;
		bWorkNoChange = TRUE;
	}
	if(strDivNo != m_pGroup->GetGroupInfo())
	{
		if(m_pGroup->GetGroupInfo() == _T("�������"))
			GetDM()->SetCellData(_T("BP_�׷��ȣ"), _T(""), 0);
		else 
			GetDM()->SetCellData(_T("BP_�׷��ȣ"), m_pGroup->GetGroupInfo(), 0);
		bDivNoChange = TRUE;
		bWorkNoChange = TRUE;
	}
	if(strWorkNo != m_pGroup->GetWorkNo() || GetOpType() == SINSERT)
	{
		if(m_pGroup->GetWorkNo() == _T("���λ���"))
		{
			GetGroupCollection()->InsertWorkNo(pDM_OUT);
			GetDM()->SetCellData(_T("BP_�۾���ȣ"), m_pGroup->GetWorkNo(), 0);
			nPNum = m_pGroup->GetLastSerialNo();
			strTmpData.Format(_T("%d"), ++nPNum);
			GetDM()->SetCellData(_T("BP_�����Ϸù�ȣ"), strTmpData, 0);
			bWorkNoChange = TRUE;
		}
		else 
		{
			GetDM()->SetCellData(_T("BP_�۾���ȣ"), m_pGroup->GetWorkNo(), 0);
			nPNum = m_pGroup->GetLastSerialNo();
			strTmpData.Format(_T("%d"), ++nPNum);
			GetDM()->SetCellData(_T("BP_�����Ϸù�ȣ"), strTmpData, 0);
			bWorkNoChange = TRUE;
		}
	}
	
	if(bWorkNoChange)
	{
		if(m_pGroup->GetGroupInfo() != _T("�������"))
		{
			GetGroupCollection()->UpDateLastSerialNo(pDM_OUT, nPNum);
			m_pGroup->SetLastSerialNo(nPNum);
		}
	}
	
	CString strAcqType = _T("1");
	
	//åDM�� ����-������ȣ ������ Sort�ؾ� �Ѵ�.
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");	nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_������ȣ");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	GetBookDM()->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	INT nRowCnt = GetBookDM()->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = GetBookDM()->GetCellData(_T("BB_DB_FLAG"), i);
		if(strTmpData == _T("D")) continue;

		if(GetOpType() == SINSERT)
		{
			if(strTmpData != _T("I")) GetBookDM()->SetCellData(_T("BB_DB_FLAG"), _T("I"), i);
			GetBookDM()->SetCellData(_T("BB_���Ա���_�ڵ�"), strAcqType, i);
			GetBookDM()->SetCellData(_T("BB_���Գ⵵"), m_pGroup->GetAcqYear(), i);
			if(m_pGroup->GetGroupInfo() == _T("�������"))
			{
				GetBookDM()->SetCellData(_T("BB_�׷��ȣ"), _T(""), i);
				GetBookDM()->SetCellData(_T("BB_�Ϸù�ȣ"), _T("ESL_SEQ.NEXTVAL"), i);
			}
			else 
			{
				GetBookDM()->SetCellData(_T("BB_�׷��ȣ"), m_pGroup->GetGroupInfo(), i);
				strTmpData.Format(_T("%d"), nPNum);
				GetBookDM()->SetCellData(_T("BB_�Ϸù�ȣ"), strTmpData, i);
			}
			
		}
		else if(GetOpType() != SINSERT)
		{
			if(strTmpData == _T("I"))
			{
				GetBookDM()->SetCellData(_T("BB_���Ա���_�ڵ�"), strAcqType, i);
				GetBookDM()->SetCellData(_T("BB_���Գ⵵"), m_pGroup->GetAcqYear(), i);
				if(m_pGroup->GetGroupInfo() == _T("�������"))
				{
					GetBookDM()->SetCellData(_T("BB_�׷��ȣ"), _T(""), i);
					GetBookDM()->SetCellData(_T("BB_�Ϸù�ȣ"), _T("ESL_SEQ.NEXTVAL"), i);
				}
				else 
				{
					GetBookDM()->SetCellData(_T("BB_�׷��ȣ"), m_pGroup->GetGroupInfo(), i);
					strTmpData.Format(_T("%d"), nPNum);
					GetBookDM()->SetCellData(_T("BB_�Ϸù�ȣ"), strTmpData, i);
				}
			}
			else if(strTmpData == _T("U"))
			{
				GetBookDM()->SetCellData(_T("BB_���Ա���_�ڵ�"), strAcqType, i);
				GetBookDM()->SetCellData(_T("BB_���Գ⵵"), m_pGroup->GetAcqYear(), i);
				if(m_pGroup->GetGroupInfo() == _T("�������"))
				{
					GetBookDM()->SetCellData(_T("BB_�׷��ȣ"), _T(""), i);
				}
				else 
				{
					GetBookDM()->SetCellData(_T("BB_�׷��ȣ"), m_pGroup->GetGroupInfo(), i);
				}
			}
		}
	}
	return 0;
}

INT CLight_DivisionNoInfoDlg::Display(INT idx)
{
	m_pCM->AllControlDisplay(idx);
	CString strAcqYear, strGroupNo, strGroupName, strWorkNo, strWorkCode;
	
	GetDM()->GetCellData(_T("BP_���Գ⵵"), 0, strAcqYear);
	GetDM()->GetCellData(_T("BP_�׷��ȣ"), 0, strGroupNo);
	GetDM()->GetCellData(_T("BP_�۾���ȣ"), 0, strWorkNo);
	GetDM()->GetCellData(_T("BP_������������"), 0, strWorkCode);
	if(strAcqYear.IsEmpty())
		strAcqYear = GetTodayYear();
	
	//���Գ⵵����
	m_pCM->SetControlMgrData(_T("BP_���Գ⵵"), strAcqYear);
	
	if(GetGroupCollection()->GetGroupCount() > 0)
	{
		GetGroupCollection()->SetCurSelGroupInfo(strAcqYear, strGroupNo, strWorkNo, strWorkCode);
		m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
		if(m_pGroup)
		{
			CStringArray tmpArray;
			CString strCurSelInfo;
			GetGroupCollection()->GetGroupInfo(&tmpArray, strCurSelInfo);
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &tmpArray, _T("BP_�׷��ȣ"));
			if(m_pGroup->GetGroupInfo() != _T("�������"))
			{
				CStringArray * pArray = m_pGroup->GetArrayWorkNo();
				SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetWorkNo(-1), pArray, _T("BP_�۾���ȣ"));
				SendPurchaseSeqNoChangeMessage();
			}
		}
	}
	return 0;
}

VOID CLight_DivisionNoInfoDlg::OnChangeWorkCode(LPARAM lParam, WPARAM wParam)
{
	GetGroupCollection()->RemoveAll();
	CString strWorkCode;
	strWorkCode.Format(_T("%d"), lParam);
	if(strWorkCode == _T("0")) strWorkCode = _T("");
	GetDM()->SetCellData(_T("BP_������������"), strWorkCode, 0);
	//GetDM()->SetCellData(_T("BP_�׷��ȣ"), _T(""), 0);
	MakeGroupNoList();
}

INT CLight_DivisionNoInfoDlg::ValidateData()
{
	if(m_pGroup == NULL)
	{
		ESLAfxMessageBox(_T("���������� �������� �ʾҽ��ϴ�."));
		return -1;
	}
	
	BOOL bError = FALSE;
	if(m_pGroup->GetGroupInfo() == _T("�������")) return 0;
	
	// ���� ������ ������ ���°� ����Ǿ����� �˻��ؾ� �Ѵ�.
	CString strQuery, strValue, strMsg;
	strQuery.Format(
		_T("SELECT MAX(WORKING_STATUS) FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),
//DEL 		_T("SELECT MAX(WORKING_STATUS) FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE='%s'"),
		m_pGroup->GetAcqYear(), m_pGroup->GetGroupInfo(), m_pGroup->GetWorkCode());
	GetDM()->GetOneValueQuery(strQuery, strValue);
	if(strValue != _T("0")) 
	{
		bError = TRUE;
		strMsg.Format(_T("������ ���� [%s]�� ���� ���°� ����Ǿ����ϴ�.\n���������� �缳�� �� �ֽʽÿ�."), m_pGroup->GetGroupInfo());
		ESLAfxMessageBox(strMsg);
		GetGroupCollection()->RemoveAt(-1);
	}
	
	if(bError)
	{
		CStringArray tmpArray;
		m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
		GetGroupCollection()->GetGroupInfo(&tmpArray, strMsg);
		SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &tmpArray, _T("BP_�׷��ȣ"));
		if(GetAcqType() == PURCHASE)
		{
			CStringArray * pArray = m_pGroup->GetArrayWorkNo();
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetWorkNo(-1), pArray, _T("BP_�۾���ȣ"));
			SendPurchaseSeqNoChangeMessage();
		}
	}
	
	if(bError) return -1;
	return 0;
}

CGroupInfo * CLight_DivisionNoInfoDlg::GetGroupInfo()
{
	return m_pGroup;
}

BOOL CLight_DivisionNoInfoDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}	

INT CLight_DivisionNoInfoDlg::ApplyData()
{
	CString strWorkNo, strMsg;
	if(m_pGroup == NULL)
	{
		strMsg = _T("������ ���������� �����ϴ�.");
		ESLAfxMessageBox(strMsg);
		return -1;
	}
	
	GetDM()->SetCellData(_T("BP_���Գ⵵"), m_pGroup->GetAcqYear(), 0);
	GetDM()->SetCellData(_T("BP_�׷��ȣ"), m_pGroup->GetGroupInfo(), 0);
	
	return 0;
}

INT CLight_DivisionNoInfoDlg::UpdateWorkSerialNo(CESL_DataMgr * pDM_OUT)
{
	return 0;
}

UINT CLight_DivisionNoInfoDlg::OnSetTabOrder(LPARAM lParam, WPARAM wParam)
{
	GetDlgItem(IDC_edtBSDIVNO_ACQ_YEAR)->SetFocus();
	
	return 0;
}