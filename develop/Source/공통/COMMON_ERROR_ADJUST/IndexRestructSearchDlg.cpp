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
	: CESL_Mgr(CIndexRestructSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexRestructSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCreate = FALSE;
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
	pCMB->AddString(_T("���������˻�"));
	pCMB->AddString(_T("����������ȣ�˻�"));
	pCMB->AddString(_T("���ι�ȣ�˻�"));

	pCMB->SetCurSel(0);
	ShowCtrlBySearchClass();

	return TRUE;
}

INT CIndexRestructSearchDlg::ShowCtrlBySearchClass()
{
	CString strSearchClass, strYear;
	BOOL bShowYN = FALSE;
	m_pCM->GetControlMgrData(_T("�˻�����"), strSearchClass);
	ESLAfxMessageBox(strSearchClass);

	if(strSearchClass == _T("���������˻�"))
	{
		strYear = _T("���Գ⵵");
		m_strGroupNo = _T("������ȣ");
		bShowYN = TRUE;
	}	
	else if(strSearchClass == _T("����������ȣ�˻�"))
	{
		strYear = _T("���Գ⵵");
		m_strGroupNo = _T("������ȣ");
		bShowYN = FALSE;
	}
	else if(strSearchClass == _T("���ι�ȣ�˻�"))
	{
		strYear = _T("���γ⵵");
		m_strGroupNo = _T("���ι�ȣ");
		bShowYN = FALSE;
	}

	GetDlgItem(IDC_STATIC_GROUP_YEAR)->SetWindowText(strYear);
	GetDlgItem(IDC_STATIC_WORK_CODE)->ShowWindow(bShowYN);
	GetDlgItem(IDC_cmbWORK_CODE)->ShowWindow(bShowYN);
	GetDlgItem(IDC_STATIC_GROUP_NO)->SetWindowText(m_strGroupNo);
	
	return 0;
}

VOID CIndexRestructSearchDlg::OnSelchangecmbSEARCHCLASS() 
{
	// TODO: Add your control notification handler code here
	ShowCtrlBySearchClass();
}

BOOL CIndexRestructSearchDlg::Create(CWnd *pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

VOID CIndexRestructSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	// KOL.UDF.022 ��ť���ڵ� ����
	INT ids = -1;
	if(m_strGroupNo == _T("������ȣ"))
		ids = MakeSearchQueryByDivNo();
	else if(m_strGroupNo == _T("������ȣ"))
		ids = MakeSearchQueryByReceiptNo();
	else if(m_strGroupNo == _T("���ι�ȣ"))
		ids = MakeSearchQueryByAccessionNo();

	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�˻� ���Ǹ� ������ �� �����ϴ�."));
		return;
	}

	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

INT CIndexRestructSearchDlg::MakeSearchQueryByDivNo()
{
	CString strAcqYear, strWorkCode, strGroupNo1, strGroupNo2, strQuery, strOption;
	m_pCM->GetControlMgrData(_T("�׷�⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);
	m_pCM->GetControlMgrData(_T("�׷��ȣ1"), strGroupNo1);
	m_pCM->GetControlMgrData(_T("�׷��ȣ2"), strGroupNo2);
	BOOL bQueryCreate = FALSE;

	strQuery.Format(_T("I.REC_KEY=S.REC_KEY AND I.REC_KEY=B.SPECIES_KEY AND B.ACQ_CODE='1' AND I.SPECIES_CLASS IN ('0', '1')"));
	if(!strAcqYear.IsEmpty())
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND B.ACQ_YEAR='%s'"), strAcqYear);
		strQuery += strOption;
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
		strOption.Format(_T(" AND B.SEQ_NO=%d"), nGroupNo1);
		strQuery += strOption;
	}
	else if(nGroupNo2 >= 0 && nGroupNo1 < 0) 
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND B.SEQ_NO=%d"), nGroupNo2);
		strQuery += strOption;
	}
	else if(nGroupNo1 >= 0 && nGroupNo2 >= 0)
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND B.SEQ_NO BETWEEN %d AND %d"), nGroupNo1, nGroupNo2);
		strQuery += strOption;
	}

	if(!bQueryCreate) return -1;

	m_strQuery = strQuery;

	return 0;
}

INT CIndexRestructSearchDlg::MakeSearchQueryByReceiptNo()
{
	CString strAcqYear, strGroupNo1, strGroupNo2, strQuery, strOption;
	m_pCM->GetControlMgrData(_T("�׷�⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("�׷��ȣ1"), strGroupNo1);
	m_pCM->GetControlMgrData(_T("�׷��ȣ2"), strGroupNo2);
	BOOL bQueryCreate = FALSE;

	strQuery.Format(_T("I.REC_KEY=S.REC_KEY AND I.REC_KEY=B.SPECIES_KEY AND B.ACQ_CODE='2' AND I.SPECIES_CLASS IN ('0', '1')"));
	if(!strAcqYear.IsEmpty())
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND B.ACQ_YEAR='%s'"), strAcqYear);
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
		strOption.Format(_T(" AND B.SEQ_NO=%d"), nGroupNo1);
		strQuery += strOption;
	}
	else if(nGroupNo2 >= 0 && nGroupNo1 < 0) 
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND B.SEQ_NO=%d"), nGroupNo2);
		strQuery += strOption;
	}
	else if(nGroupNo1 >= 0 && nGroupNo2 >= 0)
	{
		bQueryCreate = TRUE;
		strOption.Format(_T(" AND B.SEQ_NO BETWEEN %d AND %d"), nGroupNo1, nGroupNo2);
		strQuery += strOption;
	}
	
	if(!bQueryCreate) return -1;
	
	m_strQuery = strQuery;
	
	return 0;
}

INT CIndexRestructSearchDlg::MakeSearchQueryByAccessionNo()
{
	CString strAcqYear, strGroupNo1, strGroupNo2, strQuery, strOption, strBookQuery;
	m_pCM->GetControlMgrData(_T("�׷�⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("�׷��ȣ1"), strGroupNo1);
	m_pCM->GetControlMgrData(_T("�׷��ȣ2"), strGroupNo2);
	
	strQuery.Format(_T("I.REC_KEY=S.REC_KEY AND I.REC_KEY=B.SPECIES_KEY AND I.SPECIES_CLASS IN ('0', '1')"));
	
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
		strOption.Format(_T(" AND B.ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE REC_KEY IS NOT NULL %s)"), strBookQuery);
		strQuery += strOption;
	}
	else 
	{
		return -1;
	}
	
	m_strQuery = strQuery;
	
	return 0;
}

HBRUSH CIndexRestructSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
