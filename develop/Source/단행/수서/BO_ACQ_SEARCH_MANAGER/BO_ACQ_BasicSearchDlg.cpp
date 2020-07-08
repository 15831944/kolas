// BO_ACQ_BasicSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_BasicSearchDlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/Group_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_BasicSearchDlg dialog


CBO_ACQ_BasicSearchDlg::CBO_ACQ_BasicSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBO_SearchDlg(CBO_ACQ_BasicSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_BasicSearchDlg)
	//}}AFX_DATA_INIT
	m_bShowMode = FALSE;
	m_pCM = NULL;
	m_strGroupName = _T("");

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
CBO_ACQ_BasicSearchDlg::~CBO_ACQ_BasicSearchDlg()
{
}

VOID CBO_ACQ_BasicSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_BasicSearchDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_BasicSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_BasicSearchDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnGROUP_VIEW, OnbtnGROUPVIEW)
	ON_BN_CLICKED(IDC_btnBASIC_SHOWHIDE, OnbtnBASICSHOWHIDE)
	ON_EN_KILLFOCUS(IDC_edtBASIC_GROUP_NAME, OnKillfocusedtBASICGROUPNAME)
	ON_BN_CLICKED(IDC_btnBASIC_CLEAR, OnbtnBASICCLEAR)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_EN_SETFOCUS(IDC_edtBASIC_TITLE, OnSetfocusedtBASICTITLE)
	ON_EN_SETFOCUS(IDC_edtBASIC_PUBLISHER, OnSetfocusedtBASICPUBLISHER)
	ON_EN_SETFOCUS(IDC_edtBASIC_AUTHOR, OnSetfocusedtBASICAUTHOR)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_edtBASIC_PUBYEAR, OnKillfocusedtBASICPUBYEAR)
	ON_EN_KILLFOCUS(IDC_edtBASIC_GROUP_YEAR, OnKillfocusedtBASICGROUPYEAR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_BasicSearchDlg message handlers

VOID CBO_ACQ_BasicSearchDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CBO_ACQ_BasicSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_��������ڷ�˻�")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM����
	m_pCM = FindCM(_T("CM_��������ڷ�˻�"));	

	if(!Init())
	{
		ESLAfxMessageBox(_T("�˻�ȭ���� �ʱ�ȭ�� �� �����ϴ�."));
		return FALSE;
	}

	GetWindowRect(m_rcWindow);
	ShowControlByShowMode();
	//============================================
	//20080530 ADD BY PJW : �����,���Գ��� ���� 4�ڸ� �̻� �����ʵ��� ����
	CEdit * pPubYear = (CEdit*)GetDlgItem(IDC_edtBASIC_PUBYEAR);
	CEdit * pGroupYear = (CEdit*)GetDlgItem(IDC_edtBASIC_GROUP_YEAR);
	pPubYear->SetLimitText(4);
	pGroupYear->SetLimitText(4);
	//============================================

	EnableThemeDialogTexture(GetSafeHwnd());


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_ACQ_BasicSearchDlg::Init()
{
	return TRUE;
}

VOID CBO_ACQ_BasicSearchDlg::ShowControlByShowMode()
{
	INT i = 0;
	INT nCodeResourcesHeight = 150;
	CWnd *pWnd = NULL;

	if (m_bShowMode)
	{
		GetDlgItem(IDC_btnBASIC_SHOWHIDE)->SetWindowText(_T("�����˻�"));
	}
	else
	{
		GetDlgItem(IDC_btnBASIC_SHOWHIDE)->SetWindowText(_T("�󼼰˻�"));
	}

	CRect rcWindow;
	GetWindowRect(rcWindow);

	CArray<INT, INT> ArrResourceID;	ArrResourceID.RemoveAll();
	ArrResourceID.Add(IDC_stcGROUP1);
	ArrResourceID.Add(IDC_stcAUTHOR);
	ArrResourceID.Add(IDC_edtBASIC_AUTHOR);
	ArrResourceID.Add(IDC_stcPUBLISHER);
	ArrResourceID.Add(IDC_edtBASIC_PUBLISHER);
	ArrResourceID.Add(IDC_stcPUBYEAR);
	ArrResourceID.Add(IDC_edtBASIC_PUBYEAR);
	ArrResourceID.Add(IDC_stcISBN);
	ArrResourceID.Add(IDC_edtBASIC_ISBN);
	ArrResourceID.Add(IDC_stcGROUP2);
	ArrResourceID.Add(IDC_stcFORMCODE);
	ArrResourceID.Add(IDC_cmbBASIC_FORM_CODE);
	ArrResourceID.Add(IDC_stcUSEOBJCODE);
	ArrResourceID.Add(IDC_cmbBASIC_USER_OBJ_CODE);
	ArrResourceID.Add(IDC_stcWORK_CODE);
	ArrResourceID.Add(IDC_cmbBASIC_JOB_CLASS);
	ArrResourceID.Add(IDC_stcCLASSNOTYPE);
	ArrResourceID.Add(IDC_cmbBASIC_CLASS_NO_TYPE);

	// Show / Hide Code Resource
	for (i = 0 ; i < ArrResourceID.GetSize() ; i++)
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

VOID CBO_ACQ_BasicSearchDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CBO_ACQ_BasicSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	// CM���� ����ڰ� ������ �˻��������� �˻� ������ �����Ѵ�.
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
	CString m_SubregNo = _T("");
	
	INT ids ;
	CString strWhere;
	CString strWorkingStatus = _T("");
	if(GetOpenMode() == BO_ACQ_BASIC) strWorkingStatus = _T("BOT111O");
	else if(GetOpenMode() == BO_ACQ_IMPORT) strWorkingStatus = _T("BOT112O");
	
	strWhere.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY ")
		_T("AND B.SEQ_NO = G.GROUP_SEQ(+) AND I.SPECIES_CLASS IN ('0', '1') ")
		_T("AND (B.ACQ_CODE='0' OR B.ACQ_CODE IS NULL) AND B.BOOK_APPENDIX_FLAG='B' ")
		//===================================================
		//2009.03.06 UPDATE BY PJW : DM�� MANAGE_CODE�� ���� ������ WHERE�������� MANAGE_CODE�����Ѵ�.
// 		_T("AND B.WORKING_STATUS='%s' AND I.MANAGE_CODE = UDF_MANAGE_CODE"), strWorkingStatus);
		_T("AND B.WORKING_STATUS='%s' "), strWorkingStatus);
		//===================================================
		
	
	CString errstr, result;
	
	m_pCM->GetControlMgrData(_T("�׷�����⵵"), m_Acquisit_Year);
	TrimAll(m_Acquisit_Year);
	ids = pMakeSearchData->IsValidYearData(m_Acquisit_Year);
	if( ids < 0)
	{
		errstr.Format(_T("�Է��Ͻ� ���Գ⵵ ��(%s)�� �ùٸ��� �ʽ��ϴ�."), m_Acquisit_Year);
		ESLAfxMessageBox(errstr.GetBuffer(0));
		m_pCM->SetControlMgrData(_T("�׷�����⵵"), GetTodayYear());		
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
	m_pCM->GetControlMgrData(_T("����"), m_Title);
	m_pCM->GetControlMgrData(_T("����"), m_Author);
	m_pCM->GetControlMgrData(_T("������"), m_Publisher);
	m_pCM->GetControlMgrData(_T("ISBN"), m_ISBN);
	m_pCM->GetControlMgrData(_T("���ı���_�ڵ�"), m_Form_Code);
	m_pCM->GetControlMgrData(_T("�̿��󱸺�_�ڵ�"), m_Use_Obj_Code);
	m_pCM->GetControlMgrData(_T("���ı���_�ڵ�"), m_Form_Code);
	m_pCM->GetControlMgrData(_T("��������_�ڵ�"), m_Work_Code);
	m_pCM->GetControlMgrData(_T("�з�ǥ����_�ڵ�"), m_Classfy_Code);
	m_pCM->GetControlMgrData(_T("������ϱ���_�ڵ�"), m_SubregNo);
	TrimAll(m_Title);
	TrimAll(m_Author);
	TrimAll(m_PubYear);
	TrimAll(m_ISBN);
	TrimAll(m_Form_Code);
	TrimAll(m_Use_Obj_Code);
	TrimAll(m_strGroupName);
	TrimAll(m_Work_Code);
	TrimAll(m_Classfy_Code);
	TrimAll(m_SubregNo);
		
	//��ǥ��, ������, ������ ���ξ� ������������ ��ȯ
	pMakeSearchData->GetKeyWord(m_Title);	pMakeSearchData->ChangeHanjaToHangul(m_Title);
	pMakeSearchData->GetKeyWord(m_Author);	pMakeSearchData->ChangeHanjaToHangul(m_Author);
	pMakeSearchData->GetKeyWord(m_Publisher);	pMakeSearchData->ChangeHanjaToHangul(m_Publisher);
	pMakeSearchData->GetKeyWord(m_ISBN);

	CString strOption;
	
	//�Էµ� �׷����� �˻�
	//m_strGroupName = CheckValidateGroupNo(m_pCM, m_strGroupName, m_Acquisit_Year);

	//�׷� �÷��� ����
	MakeGroupInfo(m_Acquisit_Year, m_strGroupName);

//�׷��������
	CString strGroupWhereStmt;// = _T("SELECT GROUP_SEQ FROM BO_ACQ_GROUP_TBL WHERE REC_KEY IS NOT NULL");
	if(!m_Acquisit_Year.IsEmpty())
	{
		strOption.Format(_T(" AND G.CREATE_YEAR = '%s'"), m_Acquisit_Year);
		strGroupWhereStmt += strOption;
	}
	if(!m_strGroupName.IsEmpty())
	{
		strOption = _T(" AND (")+MakeGroupInfoOrQuery(m_strGroupName)+_T(")");
		strGroupWhereStmt += strOption;
	}
//å���� ����	
	CString strBookWhereStmt;
	strBookWhereStmt.Format(_T("SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY IS NOT NULL AND MANAGE_CODE = UDF_MANAGE_CODE AND WORKING_STATUS = '%s'"), strWorkingStatus);
	strOption = MakeDateCondition_Stmt(_T("B.INPUT_DATE"), m_Date1, m_Date2);
	if(strOption == _T("-1")) return ;
	strWhere += strOption;
	if(!m_Acquisit_Year.IsEmpty())
	{
		strOption.Format(_T(" AND B.ACQ_YEAR = '%s'"), m_Acquisit_Year);
		strWhere += strOption;
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
	if (!(m_Work_Code.IsEmpty()) && m_bShowMode)
	{
		strOption.Format(_T(" AND I.WORK_CODE = '%s'"), m_Work_Code);
		strWhere += strOption;
	}
	if (!(m_Classfy_Code.IsEmpty()) && m_bShowMode)
	{
		strOption.Format(_T(" AND I.CLASS_NO_TYPE = '%s'"), m_Classfy_Code);
		strWhere += strOption;			
	}
	if (!(m_SubregNo.IsEmpty()) && m_bShowMode)
	{
		strOption.Format(_T(" AND I.SUB_REG_CODE = '%s'"), m_SubregNo);
		strWhere += strOption;			
	}
	//������ �׷�/å/���� ������ �����Ѵ�.
/*
	if(m_strGroupName.IsEmpty())
		strWhere += _T(" AND B.SEQ_NO = 0");
		//strWhere += _T(" AND I.REC_KEY IN (") + strBookWhereStmt + _T(" AND SEQ_NO = 0)");
	else
*/
	if(!m_strGroupName.IsEmpty())
		strWhere += strGroupWhereStmt;
	SetWhereStmt(strWhere);
	// ���� ������ �θ𿡰� �޽����� ������.
	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

BOOL CBO_ACQ_BasicSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		GetDlgItem(IDOK)->SetFocus();
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_ACQ_BasicSearchDlg::OnbtnGROUPVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strCreateYear;
	m_pCM->GetControlMgrData(_T("�׷�����⵵"), strCreateYear);
	
	CGroup_List_Dlg dlg(this);
	dlg.SetSpeciesType(GetSpeciesType());
	dlg.SetGroupCollection(m_pGroups);
	dlg.SetAcqYear(strCreateYear);
	dlg.DoModal();
	m_strGroupName = dlg.GetSelInfo();
	if(!m_strGroupName.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("�׷��̸�"), m_strGroupName);
		m_pCM->SetControlMgrData(_T("�׷�����⵵"), m_pGroups->GetAcqYear());
		OnKillfocusedtBASICGROUPNAME();
		OnOK();
	}
	
}

VOID CBO_ACQ_BasicSearchDlg::OnbtnBASICSHOWHIDE() 
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

VOID CBO_ACQ_BasicSearchDlg::OnKillfocusedtBASICGROUPNAME() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear;
	m_pCM->GetControlMgrData(_T("�׷�����⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("�׷��̸�"), m_strGroupName);
	
	m_strGroupName = CheckValidateGroupNo(m_pCM, m_strGroupName, strAcqYear);	
	m_pCM->SetControlMgrData(_T("�׷��̸�"), m_strGroupName);
}
/*
HBRUSH CBO_ACQ_BasicSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
*/

VOID CBO_ACQ_BasicSearchDlg::ShowGroupInfo()
{
	CString strAcqYear;
	strAcqYear = m_pGroups->GetAcqYear();
	if(strAcqYear.IsEmpty()) strAcqYear = GetTodayYear();
	m_pGroups->GetGroupInfo(m_strGroupName);
	if(m_strGroupName == _T("�������")) m_strGroupName = _T("");
	m_pCM->SetControlMgrData(_T("�׷��̸�"), m_strGroupName);
	m_pCM->SetControlMgrData(_T("�׷�����⵵"), strAcqYear);
	GetDlgItem(IDC_edtBASIC_GROUP_NAME)->SetFocus();
}

VOID CBO_ACQ_BasicSearchDlg::OnbtnBASICCLEAR() 
{
	// TODO: Add your control notification handler code here
	ClearScreen(m_pCM);
	m_pGroups->RemoveAll();
	ShowGroupInfo();
}

VOID CBO_ACQ_BasicSearchDlg::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	ShowDataSortDlg();
}

VOID CBO_ACQ_BasicSearchDlg::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton*)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;
}

VOID CBO_ACQ_BasicSearchDlg::OnSetfocusedtBASICTITLE() 
{
	SetIME(IDC_edtBASIC_TITLE, 1);	
}

VOID CBO_ACQ_BasicSearchDlg::OnSetfocusedtBASICPUBLISHER() 
{
	SetIME(IDC_edtBASIC_PUBLISHER, 1);	
}

VOID CBO_ACQ_BasicSearchDlg::OnSetfocusedtBASICAUTHOR() 
{
	SetIME(IDC_edtBASIC_AUTHOR, 1);	
}

HBRUSH CBO_ACQ_BasicSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}


void CBO_ACQ_BasicSearchDlg::OnKillfocusedtBASICPUBYEAR() 
{
	//============================================
	//20080530 ADD BY PJW : ������� ���� 4�ڸ��� �����ʾ��� ��� 4�ڸ� ���ڸ� �����ش� 
	CString strPubYear;
	INT nYear;
	CEdit * pPubYear = (CEdit*)GetDlgItem(IDC_edtBASIC_PUBYEAR);
	pPubYear->GetWindowText(strPubYear);
	if( strPubYear.IsEmpty() )
	{
		return;
	}
	else
	{
		nYear = _ttoi(strPubYear);
	}
	if( nYear < 1000 || nYear > 9999 )
	{

		ESLAfxMessageBox(_T("1000 ~ 9999 ������ ���� �Է����ּ���"));
		pPubYear->SetWindowText(_T(""));
	}
	//============================================
}

void CBO_ACQ_BasicSearchDlg::OnKillfocusedtBASICGROUPYEAR() 
{
	//============================================
	//20080530 ADD BY PJW : �׷�������� ���� 4�ڸ��� �����ʾ��� ��� 4�ڸ� ���ڸ� �����ش� 
	CString strGroupYear;
	INT nYear;
	CEdit * pGroupYear = (CEdit*)GetDlgItem(IDC_edtBASIC_GROUP_YEAR);
	pGroupYear->GetWindowText(strGroupYear);
	
	if( strGroupYear.IsEmpty() )
	{
		return;
	}
	else
	{
		nYear = _ttoi(strGroupYear);
	}
	if( nYear < 1000 || nYear > 9999 )
	{

		ESLAfxMessageBox(_T("1000 ~ 9999 ������ ���� �Է����ּ���"));
		pGroupYear->SetWindowText(_T(""));
	}
	//============================================
	
}
