// BO_ACQ_RequestSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_RequestSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_RequestSearchDlg dialog


CBO_ACQ_RequestSearchDlg::CBO_ACQ_RequestSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBO_SearchDlg(CBO_ACQ_RequestSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_RequestSearchDlg)
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_bShowMode = FALSE;
	m_strIs5163.Empty();
}
CBO_ACQ_RequestSearchDlg::~CBO_ACQ_RequestSearchDlg()
{
}

VOID CBO_ACQ_RequestSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_RequestSearchDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_RequestSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_RequestSearchDlg)
	ON_BN_CLICKED(IDC_btnREQUEST_SHOWHIDE, OnbtnREQUESTSHOWHIDE)
	ON_BN_CLICKED(IDC_btnREQUEST_CLEAR, OnbtnREQUESTCLEAR)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_edtREQUEST_PUBLICATE_YEAR, OnKillfocusedtREQUESTPUBLICATEYEAR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_RequestSearchDlg message handlers

BOOL CBO_ACQ_RequestSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	if(InitESL_Mgr(_T("K2UPGRADE_��ġ����ڷ�˻�")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	// 2005-01-21
	GetManageValue( _T("��Ÿ"), _T("����"), _T("5163"), _T("5163"), m_strIs5163 );

	m_pCM = FindCM(_T("CM_��ġ����ڷ�˻�"));	
	
	CString furnish_status[5][2] = {
		_T(" "), _T("�������"),
		_T("1"), _T("��û��"),
		_T("2"), _T("ó����"),
		_T("3"), _T("������"),
		_T("4"), _T("��ҵ�")
	};

	CMulitComboBoxManager mgr(this);
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("�������"), furnish_status, _T("��ġ����"), 5);

	// 2005-01-22 �켱�����ڷ� �޺��ڽ� �߰�.
	CString priority_catalog[3][2] = {
		_T(" "), _T("�������"),
		_T("N"), _T("�Ϲ��ڷ�"),
		_T("Y"), _T("�켱�����ڷ�"),
	};
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("�������"), priority_catalog, _T("�켱�����ڷ�"), 3);

	GetWindowRect(m_rcWindow);
	ShowControlByShowMode();
	

	//������ ��� �˻� ���� ����..
	CString str64;
	GetManageValue( _T("��Ÿ"), _T("����"), _T("����"), _T("����"), str64 );
	if ( str64 == _T("Y") )
		SetWindowText( _T("��������ڷ�˻�") );
	else
		SetWindowText( _T("��ġ����ڷ�˻�") );

	//============================================
	//20080603 ADD BY PJW : ���ǳ��� ���� 4�ڸ� �̻� �����ʵ��� ����
//	CEdit * pPubYear = (CEdit*)GetDlgItem(IDC_edtREQUEST_PUBLICATE_YEAR);
//	pPubYear->SetLimitText(4);
	//============================================


	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_RequestSearchDlg::ShowControlByShowMode()
{
	INT i = 0;
	//INT nCodeResourcesHeight = 85;
	//JOB.2018.0069 �󼼰˻� ISBN �߰� 
	INT nCodeResourcesHeight = 105;

	CWnd *pWnd = NULL;

	if (m_bShowMode)
	{
		GetDlgItem(IDC_btnREQUEST_SHOWHIDE)->SetWindowText(_T("�����˻�"));
	}
	else
	{
		GetDlgItem(IDC_btnREQUEST_SHOWHIDE)->SetWindowText(_T("�󼼰˻�"));
	}


	CRect rcWindow;
	GetWindowRect(rcWindow);

	CArray<INT, INT> ArrResourceID;	ArrResourceID.RemoveAll();
	ArrResourceID.Add(IDC_stcREQUEST_HIDE);
	ArrResourceID.Add(IDC_stcREQUEST_TITLE);
	ArrResourceID.Add(IDC_stcREQUEST_AUTHOR);
	ArrResourceID.Add(IDC_stcREQUEST_PUBLISHER);
	ArrResourceID.Add(IDC_stcREQUEST_PUBYEAR);
	//JOB.2018.0069 �󼼰˻� ISBN �߰� 
	ArrResourceID.Add(IDC_stcREQUEST_ISBN);

	ArrResourceID.Add(IDC_edtREQUEST_TITLE);
	ArrResourceID.Add(IDC_edtREQUEST_AUTHOR);
	ArrResourceID.Add(IDC_edtREQUEST_PUBLISHER);
	ArrResourceID.Add(IDC_edtREQUEST_PUBLICATE_YEAR);
	//JOB.2018.0069 �󼼰˻� ISBN �߰� 
	ArrResourceID.Add(IDC_edtREQUEST_ISBN);
	
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

	// 2005-01-21 �۽¹�
	// �켱�����ڷḦ �߰��ϸ鼭 5163�� ��쿡�� 
	// ��û�ڶ��� EditBox(IDC_edtAPPLICANT_II)�� CheckBox(IDC_chkPRIORITY_CATALOG)�� �����ش�.
	if( _T("Y") == m_strIs5163)
	{
		pWnd = GetDlgItem(IDC_edtAPPLICANT);
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_edtUSERNO);
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_stcUSERNO);
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_edtAPPLICANT_II);
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_stcPRIORITY_CATALOG);
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_cmbPRIORITY_CATALOG);
		pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		pWnd = GetDlgItem(IDC_edtAPPLICANT_II);
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_stcPRIORITY_CATALOG);
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbPRIORITY_CATALOG);
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_edtAPPLICANT);
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_edtUSERNO);
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_stcUSERNO);
		pWnd->ShowWindow(SW_SHOW);
	}

	Invalidate();
	UpdateWindow();
}

BOOL CBO_ACQ_RequestSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_ACQ_RequestSearchDlg::OnOK() 
{
// 	CString strManageCode;
	CString status  = _T("");
	CString sosok = _T("");
	CString requestor = _T("");
	CString req_date1 = _T("");
	CString req_date2 = _T("");
	CString title = _T("");
	CString author = _T("");
	CString publisher = _T("");
	CString pub_year = _T("");
	CString Priority_YN = _T("");  // 2005-01-21 �켱�����ڷ� �߰�.
	CString strUserNo = _T(""); // 2006-12-06 �����ڹ�ȣ �߰�
	//JOB.2018.0069 �󼼰˻� ISBN �߰�
	CString strISBN =_T("");

	m_pCM->GetControlMgrData(_T("��ġ����"), status);
	m_pCM->GetControlMgrData(_T("��û����1"), req_date1);
	m_pCM->GetControlMgrData(_T("��û����2"), req_date2);
// 	m_pCM->GetControlMgrData(_T("��������"), strManageCode);
	// 2005-01-21 �۽¹�
	if( _T("Y") == m_strIs5163 )
	{
		m_pCM->GetControlMgrData(_T("��û��2"), requestor);
		m_pCM->GetControlMgrData(_T("�켱�����ڷ�"), Priority_YN);
	}
	else
	{
		m_pCM->GetControlMgrData(_T("��û��"), requestor);
		m_pCM->GetControlMgrData(_T("�����ڹ�ȣ"), strUserNo);
	}

	if(m_bShowMode)
	{
		m_pCM->GetControlMgrData(_T("��ǥ��"), title);
		m_pCM->GetControlMgrData(_T("������"), author);
		m_pCM->GetControlMgrData(_T("������"), publisher);
		m_pCM->GetControlMgrData(_T("�����"), pub_year);
		//JOB.2018.0069 �󼼰˻� ISBN �߰�
		m_pCM->GetControlMgrData(_T("ISBN"), strISBN);
	}
	// 2005-01-21 �۽¹�
	/*if(_T("Y") == m_strIs5163)
	{
		INT nCheck = -1;
		CMulitComboBoxManager mgr(this);
		mgr.GetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("�������"), furnish_status, _T("��ġ����"), 5);
		if(0 == nCheck)	Priority_YN = _T("N");
		else Priority_YN = _T("Y");
	}
	*/

	status.TrimLeft();		status.TrimRight();
// 	strManageCode.TrimLeft();		strManageCode.TrimRight();
	req_date1.TrimLeft();	req_date1.TrimRight();	
	req_date2.TrimLeft();	req_date2.TrimRight();	
	pub_year.TrimLeft();	pub_year.TrimRight();
	title.TrimLeft();		title.TrimRight();
	author.TrimLeft();		author.TrimRight();		
	publisher.TrimLeft();	publisher.TrimRight();
	requestor.TrimLeft();	requestor.TrimRight();
	Priority_YN.TrimLeft();	Priority_YN.TrimRight();  // 2005-01-21
	//JOB.2018.0069 �󼼰˻� ISBN �߰�
	strISBN.TrimLeft();		strISBN.TrimRight();

	pMakeSearchData->GetKeyWord(title);	
	pMakeSearchData->GetKeyWord(author);
	pMakeSearchData->GetKeyWord(publisher);
	//2005-09-29 Ȳ���� : ��ġ��������� �ٸ� ���Ա������� �ΰ�
	//�Ǿ �˻��� �Ǿ�� �Ѵ�.
	CString strWhere = _T("REC_KEY IS NOT NULL");
	//CString strWhere = _T("REC_KEY IS NOT NULL AND ACQ_CODE = '1' ");
	CString strOption = _T("");
	CString result;
	//����� �� �˻�
	INT ids = pMakeSearchData->IsValidYearData(pub_year);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("����� ������ �ùٸ��� �ʽ��ϴ�."));
		m_pCM->SetControlMgrData(_T("�����"), GetTodayYear());
		return;
	}

	if(!status.IsEmpty())
	{
		strOption.Format(_T(" AND FURNISH_STATUS = '%s'"), status);
		strWhere += strOption;
	}
//
//	if(!strManageCode.IsEmpty())
//	{
//		strOption.Format(_T(" AND MANAGE_CODE = '%s'"), strManageCode);
//		strWhere += strOption;
//	}

	strOption = MakeDateCondition_Stmt(_T("APPLICANT_DATE"), req_date1, req_date2);
	if(strOption == _T("-1")) return ;

	strWhere += strOption;

	// ��û�� �˻��õ� �������� �˻��ؾ� �Ѵ�.
	// 2006.01.04 ������
	if( requestor.IsEmpty() == FALSE )
	{
		if( strWhere.IsEmpty() == FALSE )
		{
			strOption.Format(_T(" AND IDX_APPLICANT_NAME LIKE '%s%%'"), requestor);
		}
		else
		{
			strOption.Format(_T("IDX_APPLICANT_NAME LIKE '%s%%'"), requestor);
		}
		strWhere += strOption;
	}
	
	// �����ڹ�ȣ �߰�
	if( strUserNo.IsEmpty() == FALSE )
	{
		if( strWhere.IsEmpty() == FALSE )
		{
			strOption.Format(_T(" AND USER_NO = '%s'"), strUserNo);
		}
		else
		{
			strOption.Format(_T("USER_NO = '%s'"), strUserNo);
		}
		strWhere += strOption;
	}

	if( title.IsEmpty() == FALSE )
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND INDEX_TITLE LIKE '%s%%'"), title);
		}
		else
		{
			strOption.Format(_T(" INDEX_TITLE LIKE '%s%%'"), title);
		}
		strWhere += strOption;
	}

	if( author.IsEmpty() == FALSE )
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND INDEX_AUTHOR LIKE '%s%%'"), author);
		}
		else
		{
			strOption.Format(_T("INDEX_AUTHOR LIKE '%s%%'"), author);
		}
		strWhere += strOption;
	}
	
	if(!(publisher.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND INDEX_PUBLISHER LIKE '%s%%'"), publisher);
		}
		else
		{
			strOption.Format(_T("INDEX_PUBLISHER LIKE '%s%%'"), publisher);
		}
		strWhere += strOption;
	}

	if(!(pub_year.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND PUBLISH_YEAR = '%s'"), pub_year);
		}
		else
		{
			strOption.Format(_T("PUBLISH_YEAR = '%s'"), pub_year);
		}
		strWhere += strOption;
	}

	// 2005-01-22 �켱�����ڷ�
	if(!(Priority_YN.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
			strWhere += _T(" AND");
		if(_T("Y") == Priority_YN)
		{
			strOption.Format(_T(" PRIORITY_YN = '%s'"), Priority_YN);
		}
		else
		{
			strOption.Format(_T(" (PRIORITY_YN = '%s' OR PRIORITY_YN IS NULL)"), Priority_YN);
		}
		strWhere += strOption;
	}
	
	//JOB.2018.0069 �󼼰˻� ISBN �߰�
	if(strISBN.IsEmpty() == FALSE){
		if(strWhere.IsEmpty() == FALSE) 
			strWhere += _T(" AND");
		
		strOption.Format(_T(" EA_ISBN= '%s'"),strISBN);
		strWhere += strOption;
	}

	SetWhereStmt(strWhere);
	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

VOID CBO_ACQ_RequestSearchDlg::OnbtnREQUESTSHOWHIDE() 
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

VOID CBO_ACQ_RequestSearchDlg::ShowGroupInfo()
{
	return ;
}

VOID CBO_ACQ_RequestSearchDlg::OnbtnREQUESTCLEAR() 
{
	// TODO: Add your control notification handler code here
	ClearScreen(m_pCM);
	ShowGroupInfo();
}

VOID CBO_ACQ_RequestSearchDlg::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	ShowDataSortDlg();
}

VOID CBO_ACQ_RequestSearchDlg::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton *)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;	
}

HBRUSH CBO_ACQ_RequestSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
void CBO_ACQ_RequestSearchDlg::OnKillfocusedtREQUESTPUBLICATEYEAR() 
{
	//============================================
	//20080603 ADD BY PJW : ���ǳ��� ���� 4�ڸ��� �����ʾ��� ��� 4�ڸ� ���ڸ� �����ش� 
//	CString strPubYear;
//	INT nYear;
//	CEdit * pPubYear = (CEdit*)GetDlgItem(IDC_edtREQUEST_PUBLICATE_YEAR);
//	pPubYear->GetWindowText(strPubYear);
//	if( strPubYear.IsEmpty() )
//	{
//		return;
//	}
//	else
//	{
//		nYear = _ttoi(strPubYear);
//	}
//	if( nYear < 1000 || nYear > 9999 )
//	{
//		ESLAfxMessageBox(_T("1000 ~ 9999 ������ ���� �Է����ּ���"));
//		pPubYear->SetWindowText(_T(""));
//	}
	//============================================	
	
}
