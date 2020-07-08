// IndexRestructSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IndexRestructSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexRestructSearchDlg dialog


CIndexRestructSearchDlg::CIndexRestructSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CErrorAdjustSearchMgr(CIndexRestructSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexRestructSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
}

CIndexRestructSearchDlg::~CIndexRestructSearchDlg()
{
	
}

VOID CIndexRestructSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexRestructSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexRestructSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CIndexRestructSearchDlg)
	ON_CBN_SELCHANGE(IDC_cmbSEARCH_CLASS, OnSelchangecmbSEARCHCLASS)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexRestructSearchDlg message handlers

BOOL CIndexRestructSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_�����籸��˻�")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	m_pCM = FindCM(_T("CM_�����籸��˻�"));
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("�����籸�� �˻� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CIndexRestructSearchDlg::Init()
{
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�˻�����"));
	if(pCMB == NULL) return FALSE;
	pCMB->ResetContent();
	pCMB->AddString(_T("���������˻�"));
	pCMB->AddString(_T("����������ȣ�˻�"));
	pCMB->AddString(_T("�����׷��ȣ�˻�"));
	pCMB->AddString(_T("���ι�ȣ�˻�"));
	
	pCMB->SetCurSel(0);
	ShowCtrlBySearchClass();
	
	return TRUE;
}

INT CIndexRestructSearchDlg::ShowCtrlBySearchClass()
{
	CString strSearchClass, strYear;
	BOOL bWorkCodeShowYN = FALSE;
	BOOL bAcqCodeShowYN = FALSE;
	m_pCM->GetControlMgrData(_T("�˻�����"), strSearchClass);
		
	if(strSearchClass == _T("���������˻�"))
	{
		strYear = _T("���Գ⵵");
		m_strGroupNo = _T("������ȣ");
		bWorkCodeShowYN = TRUE;
		bAcqCodeShowYN = FALSE;
	}	
	else if(strSearchClass == _T("����������ȣ�˻�"))
	{
		strYear = _T("���Գ⵵");
		m_strGroupNo = _T("������ȣ");
		bWorkCodeShowYN = FALSE;
		bAcqCodeShowYN = FALSE;
	}
	else if(strSearchClass == _T("�����׷��ȣ�˻�"))
	{
		strYear = _T("���Գ⵵");
		m_strGroupNo = _T("�׷��ȣ");
		bWorkCodeShowYN = FALSE;
		bAcqCodeShowYN = TRUE;
	}
	else if(strSearchClass == _T("���ι�ȣ�˻�"))
	{
		strYear = _T("���γ⵵");
		m_strGroupNo = _T("���ι�ȣ");
		bWorkCodeShowYN = FALSE;
		bAcqCodeShowYN = FALSE;
	}
	
	GetDlgItem(IDC_STATIC_GROUP_YEAR)->SetWindowText(strYear);
	GetDlgItem(IDC_STATIC_WORK_CODE)->ShowWindow(bWorkCodeShowYN);
	GetDlgItem(IDC_cmbWORK_CODE)->ShowWindow(bWorkCodeShowYN);
	GetDlgItem(IDC_STATIC_ACQ_CODE)->ShowWindow(bAcqCodeShowYN);
	GetDlgItem(IDC_cmbACQ_CODE)->ShowWindow(bAcqCodeShowYN);
	GetDlgItem(IDC_STATIC_GROUP_NO)->SetWindowText(m_strGroupNo);
	
	return 0;
}

VOID CIndexRestructSearchDlg::OnSelchangecmbSEARCHCLASS() 
{
	// TODO: Add your control notification handler code here
	ShowCtrlBySearchClass();
}

VOID CIndexRestructSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	INT ids;
	
	if(m_strGroupNo == _T("������ȣ"))
		ids = MakeSearchQueryByDivNo();
	else if(m_strGroupNo == _T("������ȣ"))
		ids = MakeSearchQueryByReceiptNo();
	else if(m_strGroupNo == _T("���ι�ȣ"))
		ids = MakeSearchQueryByAccessionNo();
	else if(m_strGroupNo == _T("�׷��ȣ"))
		ids = MakeSearchQueryByGroupNo();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�˻� ���Ǹ� ������ �� �����ϴ�."));
		return;
	}
	
	SendQueryCreateMsg();	
}

INT CIndexRestructSearchDlg::MakeSearchQueryByDivNo()
{
	CString strAcqYear, strWorkCode, strGroupNo1, strGroupNo2, strQuery, strOption, strBookQuery;
	m_pCM->GetControlMgrData(_T("�׷�⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);
	m_pCM->GetControlMgrData(_T("�׷��ȣ1"), strGroupNo1);
	m_pCM->GetControlMgrData(_T("�׷��ȣ2"), strGroupNo2);
	BOOL bQueryCreate = FALSE;
	
	strBookQuery.Format(_T("SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE ACQ_CODE='1'"));
	strQuery.Format(_T("I.REC_KEY=S.REC_KEY AND I.SPECIES_CLASS IN ('0', '1')"));
	if(!strAcqYear.IsEmpty())
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND ACQ_YEAR='%s'"), strAcqYear);
		strBookQuery += strOption;
	}
	if(!strWorkCode.IsEmpty())
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND I.WORK_CODE='%s'"), strWorkCode);
		strQuery += strOption;
	}
	INT nGroupNo1, nGroupNo2, nTmpData;
	nGroupNo1 = nGroupNo2 = -1;
	if(!strGroupNo1.IsEmpty()) nGroupNo1 = _ttoi(strGroupNo1.GetBuffer(0));
	if(!strGroupNo2.IsEmpty()) nGroupNo2 = _ttoi(strGroupNo2.GetBuffer(0));
	if(nGroupNo1 > nGroupNo2) 
	{
		nTmpData = nGroupNo2;
		nGroupNo2 = nGroupNo1;
		nGroupNo1 = nTmpData;
	}
	
	if(nGroupNo1 >= 0 && nGroupNo2 < 0)  
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND SEQ_NO=%d"), nGroupNo1);
		strBookQuery += strOption;
	}
	else if(nGroupNo2 >= 0 && nGroupNo1 < 0) 
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND SEQ_NO=%d"), nGroupNo2);
		strBookQuery += strOption;
	}
	else if(nGroupNo1 >= 0 && nGroupNo2 >= 0)
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND SEQ_NO BETWEEN %d AND %d"), nGroupNo1, nGroupNo2);
		strBookQuery += strOption;
	}
	
	if(!bQueryCreate) return -1;
	
	strOption.Format(_T(" AND I.REC_KEY IN (%s)"), strBookQuery);
	strQuery += strOption;
	m_strQuery = strQuery;
	
	return 0;
}

INT CIndexRestructSearchDlg::MakeSearchQueryByReceiptNo()
{
	CString strAcqYear, strGroupNo1, strGroupNo2, strQuery, strBookQuery, strOption;
	m_pCM->GetControlMgrData(_T("�׷�⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("�׷��ȣ1"), strGroupNo1);
	m_pCM->GetControlMgrData(_T("�׷��ȣ2"), strGroupNo2);
	BOOL bQueryCreate = FALSE;
	
	strBookQuery.Format(_T("SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE ACQ_CODE='2'"));
	strQuery.Format(_T("I.REC_KEY=S.REC_KEY AND I.SPECIES_CLASS IN ('0', '1')"));
	if(!strAcqYear.IsEmpty())
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND ACQ_YEAR='%s'"), strAcqYear);
		strBookQuery += strOption;
	}
	
	INT nGroupNo1, nGroupNo2, nTmpData;
	nGroupNo1 = nGroupNo2 = -1;
	if(!strGroupNo1.IsEmpty()) nGroupNo1 = _ttoi(strGroupNo1.GetBuffer(0));
	if(!strGroupNo2.IsEmpty()) nGroupNo2 = _ttoi(strGroupNo2.GetBuffer(0));
	if(nGroupNo1 > nGroupNo2) 
	{
		nTmpData = nGroupNo2;
		nGroupNo2 = nGroupNo1;
		nGroupNo1 = nTmpData;
	}
	
	if(nGroupNo1 >= 0 && nGroupNo2 < 0)  
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND SEQ_NO=%d"), nGroupNo1);
		strBookQuery += strOption;
	}
	else if(nGroupNo2 >= 0 && nGroupNo1 < 0) 
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND SEQ_NO=%d"), nGroupNo2);
		strBookQuery += strOption;
	}
	else if(nGroupNo1 >= 0 && nGroupNo2 >= 0)
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND SEQ_NO BETWEEN %d AND %d"), nGroupNo1, nGroupNo2);
		strBookQuery += strOption;
	}
	
	if(!bQueryCreate) return -1;
	
	strOption.Format(_T(" AND I.REC_KEY IN (%s)"), strBookQuery);
	strQuery += strOption;
	m_strQuery = strQuery;
	
	return 0;
}

INT CIndexRestructSearchDlg::MakeSearchQueryByAccessionNo()
{
	CString strAcqYear, strGroupNo1, strGroupNo2, strQuery, strOption, strBookQuery;
	m_pCM->GetControlMgrData(_T("�׷�⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("�׷��ȣ1"), strGroupNo1);
	m_pCM->GetControlMgrData(_T("�׷��ȣ2"), strGroupNo2);
	

	strQuery.Format(_T("I.REC_KEY=S.REC_KEY AND I.SPECIES_CLASS IN ('0', '1')"));
	
	if(!strAcqYear.IsEmpty())
	{
		strOption.Format(_T(" AND ACCESSION_REC_MAKE_YEAR='%s'"), strAcqYear);
		strBookQuery += strOption;
	}
	INT nGroupNo1, nGroupNo2, nTmpData;
	nGroupNo1 = nGroupNo2 = -1;
	if(!strGroupNo1.IsEmpty()) nGroupNo1 = _ttoi(strGroupNo1.GetBuffer(0));
	if(!strGroupNo2.IsEmpty()) nGroupNo2 = _ttoi(strGroupNo2.GetBuffer(0));
	if(nGroupNo1 > nGroupNo2) 
	{
		nTmpData = nGroupNo2;
		nGroupNo2 = nGroupNo1;
		nGroupNo1 = nTmpData;
	}
	
	if(nGroupNo1 >= 0 && nGroupNo2 < 0)  
	{
		strOption.Format(_T(" AND ACCESSION_REC_NO=%d"), nGroupNo1);
		strBookQuery += strOption;
	}
	else if(nGroupNo2 >= 0 && nGroupNo1 < 0) 
	{
		strOption.Format(_T(" AND ACCESSION_REC_NO=%d"), nGroupNo2);
		strBookQuery += strOption;
	}
	else if(nGroupNo1 >= 0 && nGroupNo2 >= 0)
	{
		strOption.Format(_T(" AND ACCESSION_REC_NO BETWEEN %d AND %d"), nGroupNo1, nGroupNo2);
		strBookQuery += strOption;
	}
	
	if(!strBookQuery.IsEmpty())
	{
		strOption.Format(_T(" AND I.REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE REC_KEY IS NOT NULL %s))"), strBookQuery);
		strQuery += strOption;
	}
	else 
	{
		return -1;
	}
	
	m_strQuery = strQuery;
	
	return 0;
}

INT CIndexRestructSearchDlg::MakeSearchQueryByGroupNo()
{
	CString strAcqYear, strWorkCode, strGroupNo1, strGroupNo2, strQuery, strOption, strBookQuery;
	m_pCM->GetControlMgrData(_T("�׷�⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("���Ա���"), strWorkCode);
	if(strWorkCode.IsEmpty() || strWorkCode == _T("1") || strWorkCode == _T("2"))
	{
		ESLAfxMessageBox(_T("�����׷쿡 �ش��ϴ� ���Ա��������� �ƴմϴ�."));
		return -1;
	}
	m_pCM->GetControlMgrData(_T("�׷��ȣ1"), strGroupNo1);
	m_pCM->GetControlMgrData(_T("�׷��ȣ2"), strGroupNo2);
	BOOL bQueryCreate = FALSE;
	
	strBookQuery.Format(_T("SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE ACQ_CODE='%s'"), strWorkCode);
	strQuery.Format(_T("I.REC_KEY=S.REC_KEY AND I.SPECIES_CLASS IN ('0', '1')"));
	if(!strAcqYear.IsEmpty())
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND ACQ_YEAR='%s'"), strAcqYear);
		strBookQuery += strOption;
	}
	INT nGroupNo1, nGroupNo2, nTmpData;
	nGroupNo1 = nGroupNo2 = -1;
	if(!strGroupNo1.IsEmpty()) nGroupNo1 = _ttoi(strGroupNo1.GetBuffer(0));
	if(!strGroupNo2.IsEmpty()) nGroupNo2 = _ttoi(strGroupNo2.GetBuffer(0));
	if(nGroupNo1 > nGroupNo2) 
	{
		nTmpData = nGroupNo2;
		nGroupNo2 = nGroupNo1;
		nGroupNo1 = nTmpData;
	}
	
	if(nGroupNo1 >= 0 && nGroupNo2 < 0)  
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND SEQ_NO=%d"), nGroupNo1);
		strBookQuery += strOption;
	}
	else if(nGroupNo2 >= 0 && nGroupNo1 < 0) 
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND SEQ_NO=%d"), nGroupNo2);
		strBookQuery += strOption;
	}
	else if(nGroupNo1 >= 0 && nGroupNo2 >= 0)
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND SEQ_NO BETWEEN %d AND %d"), nGroupNo1, nGroupNo2);
		strBookQuery += strOption;
	}
	
	if(!bQueryCreate) return -1;
	
	strOption.Format(_T(" AND I.REC_KEY IN (%s)"), strBookQuery);
	strQuery += strOption;
	m_strQuery = strQuery;
	
	return 0;
}


HBRUSH CIndexRestructSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
