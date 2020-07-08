// BO_ACQ_RecycleSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_RecycleSearchDlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/Group_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_RecycleSearchDlg dialog


CBO_ACQ_RecycleSearchDlg::CBO_ACQ_RecycleSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBO_SearchDlg(CBO_ACQ_RecycleSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_RecycleSearchDlg)
	m_nMyData = -1;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_bShowMode = FALSE;

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
CBO_ACQ_RecycleSearchDlg::~CBO_ACQ_RecycleSearchDlg()
{
}

VOID CBO_ACQ_RecycleSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_RecycleSearchDlg)
	DDX_Radio(pDX, IDC_radRECYC_MYDATA, m_nMyData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_RecycleSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_RecycleSearchDlg)
	ON_BN_CLICKED(IDC_btnRECYCLE_SHOWHIDE, OnbtnRECYCLESHOWHIDE)
	ON_BN_CLICKED(IDC_btnRECYC_GROUP_VIEW, OnbtnRECYCGROUPVIEW)
	ON_BN_CLICKED(IDC_btnRECYCLE_CLEAR, OnbtnRECYCLECLEAR)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_EN_SETFOCUS(IDC_edtRECYC_AUTHOR, OnSetfocusedtRECYCAUTHOR)
	ON_EN_SETFOCUS(IDC_edtRECYC_PUBLISHER, OnSetfocusedtRECYCPUBLISHER)
	ON_EN_SETFOCUS(IDC_edtRECYC_TITLE, OnSetfocusedtRECYCTITLE)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_edtRECYC_ACQ_YEAR, OnKillfocusedtRECYCACQYEAR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_RecycleSearchDlg message handlers

BOOL CBO_ACQ_RecycleSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_��Ȱ���ڷ�˻�")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_��Ȱ���ڷ�˻�"));	
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("��Ȱ�� �˻� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	GetWindowRect(m_rcWindow);
	ShowControlByShowMode();
	
	//============================================
	//20080603 ADD BY PJW : ���Գ��� ���� 4�ڸ� �̻� �����ʵ��� ����
//	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtRECYC_ACQ_YEAR);
//	pAcqYear->SetLimitText(4);
	//============================================

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_RecycleSearchDlg::ShowControlByShowMode()
{
	INT i = 0;
	INT nCodeResourcesHeight = 193;
	CWnd *pWnd = NULL;

	if (m_bShowMode)
		GetDlgItem(IDC_btnRECYCLE_SHOWHIDE)->SetWindowText(_T("�����˻�"));
	else
		GetDlgItem(IDC_btnRECYCLE_SHOWHIDE)->SetWindowText(_T("�󼼰˻�"));


	CRect rcWindow;
	GetWindowRect(rcWindow);

	CArray<INT, INT> ArrResourceID;	ArrResourceID.RemoveAll();
	ArrResourceID.Add(IDC_stcRECYCLE_HIDE1);
	ArrResourceID.Add(IDC_stcRECYCLE_AUTHOR);
	ArrResourceID.Add(IDC_stcRECYCLE_PUBLISHER);
	ArrResourceID.Add(IDC_stcRECYCLE_ISBN);
	ArrResourceID.Add(IDC_stcRECYCLE_PUBYEAR);
	ArrResourceID.Add(IDC_edtRECYC_PUBYEAR);
	ArrResourceID.Add(IDC_edtRECYC_ISBN1);
	ArrResourceID.Add(IDC_edtRECYC_PUBLISHER);
	ArrResourceID.Add(IDC_edtRECYC_AUTHOR);
	ArrResourceID.Add(IDC_stcRECYCLE_HIDE2);
	ArrResourceID.Add(IDC_stcRECYCLE_WORKSTATUS);
	ArrResourceID.Add(IDC_stcRECYCLE_FORMCODE);
	ArrResourceID.Add(IDC_stcRECYCLE_INPUTTYPE);
	ArrResourceID.Add(IDC_stcRECYCLE_USEOBJCODE);
	ArrResourceID.Add(IDC_radRECYC_MYDATA);
	ArrResourceID.Add(IDC_radRECYC_ALL);
	ArrResourceID.Add(IDC_cmbRECYC_USE_OBJ_CODE);
	ArrResourceID.Add(IDC_cmbRECYC_INPUT_TYPE);
	ArrResourceID.Add(IDC_cmbRECYC_FORM_CODE);
	ArrResourceID.Add(IDC_cmbRECYC_LAST_WORKING_STATUS);
	ArrResourceID.Add(IDC_stcWORK_CODE);
	ArrResourceID.Add(IDC_cmbRECYC_WORK_CODE);
	ArrResourceID.Add(IDC_stc_CLASS_NO_TYPE);
	ArrResourceID.Add(IDC_cmbRECYC_CLASS_NO_CODE);
	
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
	
	Invalidate();
	UpdateWindow();
}

BOOL CBO_ACQ_RecycleSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_ACQ_RecycleSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CString workCode, classCodeType, inputType, UseObjCode, FormCode, pub_year, errstr, result;
	CString title, delDate1, delDate2, author, publisher, isbn1, isbn2, workingst, dataClass, strAcqYear, strGroupName ;
	
	m_pCM->GetControlMgrData(_T("��ǥ��"), title);
	m_pCM->GetControlMgrData(_T("��¥1"), delDate1);
	m_pCM->GetControlMgrData(_T("��¥2"), delDate2);
	delDate1.TrimLeft();		delDate1.TrimRight();
	delDate2.TrimLeft();		delDate2.TrimRight();
	if(delDate1.IsEmpty() && !delDate2.IsEmpty())
	{
		ESLAfxMessageBox(_T("�˻� �������� �Է��� �ּ���."));
		return ;
	}
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	TrimAll(strAcqYear);
	INT ids = pMakeSearchData->IsValidYearData(strAcqYear);
	if( ids < 0)
	{
		errstr.Format(_T("�Է��Ͻ� ���Գ⵵ ��(%s)�� �ùٸ��� �ʽ��ϴ�."), strAcqYear);
		ESLAfxMessageBox(errstr.GetBuffer(0));
		m_pCM->SetControlMgrData(_T("���Գ⵵"), GetTodayYear());		
		return;
	}
	m_pCM->GetControlMgrData(_T("�׷��̸�"), strGroupName);
	//�Էµ� �׷����� �˻�
	strGroupName = CheckValidateGroupNo(m_pCM, strGroupName, strAcqYear);
	//�׷� �÷��� ����
	MakeGroupInfo(strAcqYear, strGroupName);

	if(m_bShowMode)
	{
		m_pCM->GetControlMgrData(_T("ISBN1"), isbn1);
		m_pCM->GetControlMgrData(_T("�����"),pub_year);
		pub_year.TrimLeft();		pub_year.TrimRight();
		INT ids = pMakeSearchData->IsValidYearData(pub_year);
		if( ids < 0)
		{
			errstr.Format(_T("�Է��Ͻ� �Լ��⵵ ��(%s)�� �ùٸ��� �ʽ��ϴ�."), pub_year);
			ESLAfxMessageBox(errstr.GetBuffer(0));
			m_pCM->SetControlMgrData(_T("���Գ⵵"), GetTodayYear());		
			return ;
		}
		m_pCM->GetControlMgrData(_T("������"), author);
		m_pCM->GetControlMgrData(_T("������"), publisher);
		m_pCM->GetControlMgrData(_T("�����۾�����"), workingst);
		m_pCM->GetControlMgrData(_T("�Է±���"), inputType);
		m_pCM->GetControlMgrData(_T("�̿��󱸺�"), UseObjCode);
		m_pCM->GetControlMgrData(_T("���ı���"), FormCode);
		m_pCM->GetControlMgrData(_T("��������"), workCode);
		m_pCM->GetControlMgrData(_T("�з�ǥ����"), classCodeType);
	}
	workCode.TrimLeft();		workCode.TrimRight();
	classCodeType.TrimLeft();	classCodeType.TrimRight();
	isbn1.TrimLeft();			isbn1.TrimRight();
	workingst.TrimLeft();		workingst.TrimRight();
	inputType.TrimLeft();		inputType.TrimRight();
	UseObjCode.TrimLeft();		UseObjCode.TrimRight();
	FormCode.TrimLeft();		FormCode.TrimRight();
	title.TrimLeft();			title.TrimRight();
	author.TrimLeft();			author.TrimRight();
	publisher.TrimLeft();		publisher.TrimRight();

	//��ǥ��, ������, ������ ���ξ� ������������ ��ȯ
	pMakeSearchData->GetKeyWord(title);	pMakeSearchData->ChangeHanjaToHangul(title);
	pMakeSearchData->GetKeyWord(author);	pMakeSearchData->ChangeHanjaToHangul(author);
	pMakeSearchData->GetKeyWord(publisher);	pMakeSearchData->ChangeHanjaToHangul(publisher);
	pMakeSearchData->GetKeyWord(isbn1);
	
	INT dataType = 0;
	CString strWhere, strOption;
	workingst = ConvertWorkingStatusDescToCode(workingst);

	//===================================================
	//2009.03.06 UPDATE BY PJW : DM�� MANAGE_CODE�� ���� ������ WHERE�������� MANAGE_CODE�����Ѵ�.
// 	strWhere = _T("(I.SPECIES_CLASS = '2' OR I.SPECIES_CLASS = '3') AND I.REC_KEY = S.REC_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND I.REC_KEY = B.SPECIES_KEY AND I.REC_KEY = P.SPECIES_KEY(+) AND P.REC_KEY = F.ACQ_KEY(+)");
	strWhere = _T("(I.SPECIES_CLASS = '2' OR I.SPECIES_CLASS = '3') AND I.REC_KEY = S.REC_KEY AND I.REC_KEY = B.SPECIES_KEY AND I.REC_KEY = P.SPECIES_KEY(+) AND P.REC_KEY = F.ACQ_KEY(+)");
	//===================================================
	
	dataType = 1;
	
	//�׷��������
	CString strGroupWhereStmt = _T("SELECT GROUP_SEQ FROM BO_ACQ_GROUP_TBL WHERE REC_KEY IS NOT NULL");
	if(!strAcqYear.IsEmpty())
	{
		strOption.Format(_T(" AND CREATE_YEAR = '%s'"), strAcqYear);
		strGroupWhereStmt += strOption;
	}
	if(!strGroupName.IsEmpty())
	{
		strOption = _T(" AND (")+MakeGroupInfoOrQuery(strGroupName)+_T(")");
		strGroupWhereStmt += strOption;
	}
	strOption = MakeDateCondition_Stmt(_T("I.DELETE_DATE"), delDate1, delDate2);
	if(strOption == _T("-1")) return ;
	strWhere += strOption;
	//CString strBookWhereStmt = _T("SELECT SPECIES_KEY FROM BO_DELETE_BOOK_TBL WHERE BOOK_APPENDIX_FLAG='B'");
	if(!workingst.IsEmpty() && m_bShowMode)
	{
		strWhere += workingst;
	}
	
	if(!classCodeType.IsEmpty() && classCodeType != _T("�������"))
	{
		strOption.Format(_T(" AND I.CLASS_NO_TYPE = '%s'"), classCodeType);
		strWhere += strOption;
	}
//#ifdef _ORA_CATSEARCH_
	if(m_pInfo->GetSearchIndexType() == ORA_CATSEARCH)
	{
		if(!title.IsEmpty())
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_TITLE, '%s*', NULL)>0"), title);
			strWhere += strOption;
		}

		if(!author.IsEmpty() && m_bShowMode)
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_AUTHOR, '%s*', NULL)>0"), author);
			strWhere += strOption;
		}

		if(!publisher.IsEmpty() && m_bShowMode)
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_PUBLISHER, '%s*', NULL)>0"), publisher);
			strWhere += strOption;
		}

		if(!isbn1.IsEmpty() && m_bShowMode)
		{
			strOption.Format(_T(" AND CATSEARCH(I.ST_CODE, '%s*', NULL)>0"), isbn1);
			strWhere += strOption;
		}
	}
//#else
	else if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		m_arrECOSEARCH.RemoveAll();
		if(!title.IsEmpty())
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*') "), title);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}

		if(!author.IsEmpty() && m_bShowMode)
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*') "), author);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}

		if(!publisher.IsEmpty() && m_bShowMode)
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_IDX_PUBLISHER, '%s*') "), publisher);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}
		
		if(!isbn1.IsEmpty() && m_bShowMode)
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_ST_CODE, '%s*') "), isbn1);
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

	if(!workCode.IsEmpty())
	{
		strOption.Format(_T(" AND I.WORK_CODE = '%s'"), workCode);
		strWhere += strOption;
	}
	if(m_nMyData == 0 && m_bShowMode)
	{
		strOption.Format(_T(" AND S.WORKER = '%s'"), m_pInfo->USER_ID);
		strWhere += strOption;
	}
	if(!inputType.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND P.INPUT_TYPE = '%s'"), inputType);
		strWhere += strOption;
	}

	if(!UseObjCode.IsEmpty() && UseObjCode != _T("0") && m_bShowMode)
	{
		strOption.Format(_T(" AND I.USE_OBJ_CODE = '%s'"), UseObjCode);
		strWhere += strOption;
	}
	if(!FormCode.IsEmpty() && FormCode != _T("0") && m_bShowMode)
	{
		strOption.Format(_T(" AND I.FORM_CODE = '%s'"), FormCode);
		strWhere += strOption;
	}
	if(!pub_year.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND I.PUB_YEAR = '%s'"), pub_year);
		strWhere += strOption;
	}

	if(!strAcqYear.IsEmpty())
	{
		strOption.Format(_T(" AND B.ACQ_YEAR = '%s'"), strAcqYear);
		strWhere +=  strOption;
	}
	
/*
	if(strGroupName.IsEmpty())
		strWhere += _T(" AND B.SEQ_NO = 0");
	else
		strWhere += _T(" AND B.SEQ_NO IN (") + strGroupWhereStmt + _T(")");
*/
	if(!strGroupName.IsEmpty())
		strWhere += _T(" AND B.SEQ_NO IN (") + strGroupWhereStmt + _T(")");

	SetWhereStmt(strWhere);
	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

VOID CBO_ACQ_RecycleSearchDlg::OnbtnRECYCLESHOWHIDE() 
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

VOID CBO_ACQ_RecycleSearchDlg::OnbtnRECYCGROUPVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strCreateYear;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strCreateYear);
	CGroup_List_Dlg dlg(this);
	dlg.SetSpeciesType(GetSpeciesType());
	dlg.SetGroupCollection(m_pGroups);
	dlg.SetAcqYear(strCreateYear);
	dlg.DoModal();
	m_strGroupName = dlg.GetSelInfo();
	if(!m_strGroupName.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("�׷��̸�"), m_strGroupName);
		m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pGroups->GetAcqYear());
		OnOK();
	}	
}

BOOL CBO_ACQ_RecycleSearchDlg::Init()
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
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("�������"), inputType, _T("�Է±���"), 5);

	CString strWorkingStatus[5] =
	{
		_T("�������"), _T("�����ڷ�"), _T("����ڷ�"), _T("�����ڷ�"), _T("�����ڷ�")
	};
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("�������"), strWorkingStatus, _T("�����۾�����"), 5);
	return TRUE;
}

CString CBO_ACQ_RecycleSearchDlg::ConvertWorkingStatusDescToCode(CString strDesc)
{
	CString strCode;
	if(strDesc == _T("�������")) strCode = _T("");
	else if(strDesc == _T("�����ڷ�")) strCode = _T(" AND B.WORKING_STATUS > 'BOA' AND B.WORKING_STATUS < 'BOC'");
	else if(strDesc == _T("����ڷ�")) strCode = _T(" AND B.WORKING_STATUS > 'BOR' AND B.WORKING_STATUS < 'BOT'");
	else if(strDesc == _T("�����ڷ�")) strCode = _T(" AND B.WORKING_STATUS > 'BOC' AND B.WORKING_STATUS < 'BOL'");
	else if(strDesc == _T("�����ڷ�")) strCode = _T(" AND B.WORKING_STATUS > 'BOL' AND B.WORKING_STATUS < 'BOR'");
	return strCode;		
}

VOID CBO_ACQ_RecycleSearchDlg::ShowGroupInfo()
{
	CString strAcqYear;
	strAcqYear = m_pGroups->GetAcqYear();
	if(strAcqYear.IsEmpty()) strAcqYear = GetTodayYear();
	m_pGroups->GetGroupInfo(m_strGroupName);
	if(m_strGroupName == _T("�������")) m_strGroupName = _T("");
	m_pCM->SetControlMgrData(_T("�׷��̸�"), m_strGroupName);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), strAcqYear);
	GetDlgItem(IDC_edtRECYC_GROUP_NAME)->SetFocus();
}

VOID CBO_ACQ_RecycleSearchDlg::OnbtnRECYCLECLEAR() 
{
	// TODO: Add your control notification handler code here
	ClearScreen(m_pCM);
	m_pGroups->RemoveAll();
	ShowGroupInfo();
}

VOID CBO_ACQ_RecycleSearchDlg::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	ShowDataSortDlg();
}

VOID CBO_ACQ_RecycleSearchDlg::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton *)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;	
}

VOID CBO_ACQ_RecycleSearchDlg::OnSetfocusedtRECYCAUTHOR() 
{
	SetIME(IDC_edtRECYC_AUTHOR, 1);
}

VOID CBO_ACQ_RecycleSearchDlg::OnSetfocusedtRECYCPUBLISHER() 
{
	SetIME(IDC_edtRECYC_PUBLISHER, 1);
}

VOID CBO_ACQ_RecycleSearchDlg::OnSetfocusedtRECYCTITLE() 
{
	SetIME(IDC_edtRECYC_TITLE, 1);
}

HBRUSH CBO_ACQ_RecycleSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
void CBO_ACQ_RecycleSearchDlg::OnKillfocusedtRECYCACQYEAR() 
{
	//============================================
	//20080603 ADD BY PJW : ���Գ��� ���� 4�ڸ��� �����ʾ��� ��� 4�ڸ� ���ڸ� �����ش� 
//	CString strAcqYear;
//	INT nYear;
//	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtRECYC_ACQ_YEAR);
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
	
}
