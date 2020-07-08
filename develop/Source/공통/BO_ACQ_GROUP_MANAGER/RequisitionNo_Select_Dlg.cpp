// RequisitionNo_Select_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "RequisitionNo_Select_Dlg.h"
#include "RequisitionNo_Insert_Dlg.h"
#include "RequisitionNo_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequisitionNo_Select_Dlg dialog


CRequisitionNo_Select_Dlg::CRequisitionNo_Select_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequisitionNo_Select_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequisitionNo_Select_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGroupCollection = NULL;
	m_pGroup = NULL;
	m_strAcqYear = _T("");
}

CRequisitionNo_Select_Dlg::~CRequisitionNo_Select_Dlg()
{
	if(m_pGroupCollection)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = NULL;
	}
}

VOID CRequisitionNo_Select_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequisitionNo_Select_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequisitionNo_Select_Dlg, CDialog)
	//{{AFX_MSG_MAP(CRequisitionNo_Select_Dlg)
	ON_EN_KILLFOCUS(IDC_edtACQ_YEAR, OnKillfocusedtACQYEAR)
	ON_EN_KILLFOCUS(IDC_edtDIVNO, OnKillfocusedtDIVNO)
	ON_BN_CLICKED(IDC_btnVIEW_DIVNO, OnbtnVIEWDIVNO)
	ON_BN_CLICKED(IDC_btnREQNO_CREATE, OnbtnREQNOCREATE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequisitionNo_Select_Dlg message handlers

BOOL CRequisitionNo_Select_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_������ȣ_����")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_������ȣ_����"));
	m_pDM = FindDM(_T("DM_������ȣ����"));
	this->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	if(m_strAcqYear.IsEmpty()) m_strAcqYear = GetTodayYear();
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_strAcqYear);

	m_pGroupCollection = new CGroupCollection(this);
	m_pGroupCollection->SetAcqType(DONATE);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRequisitionNo_Select_Dlg::OnKillfocusedtACQYEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->GetControlMgrData(_T("���Գ⵵"), m_strAcqYear);
	OnKillfocusedtDIVNO();	
}

VOID CRequisitionNo_Select_Dlg::OnKillfocusedtDIVNO() 
{
	// TODO: Add your control notification handler code here
	CString strGroupNo;
	m_pCM->GetControlMgrData(_T("������ȣ"), strGroupNo);
	if(strGroupNo.IsEmpty()) return;

	INT pos = strGroupNo.Find(_T(","), 0);
	if(pos >= 0)
	{
		strGroupNo = strGroupNo.Mid(0, pos);
		strGroupNo.TrimLeft();	strGroupNo.TrimRight();
	}

	if(m_pGroup != NULL && m_pGroup->GetGroupInfo() == strGroupNo && m_pGroup->GetAcqYear() == m_strAcqYear)
		return ;

	INT ids = m_pGroupCollection->CheckValidate(strGroupNo, m_strAcqYear);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�Է��Ͻ� ������ȣ�� �ùٸ��� �ʽ��ϴ�."));
		m_pCM->SetControlMgrData(_T("������ȣ"), _T(""));
		return;
	}
	
	m_pGroupCollection->RemoveAll();
	m_pGroupCollection->AddGroupInfo(m_strAcqYear, strGroupNo);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
}

VOID CRequisitionNo_Select_Dlg::OnbtnVIEWDIVNO() 
{
	// TODO: Add your control notification handler code here
	CString strGroupNo, strAcqYear;
	strAcqYear = m_pGroupCollection->GetAcqYear();
	if(strAcqYear.IsEmpty())
		m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	CRequisitionNo_List_Dlg dlg(this);

	dlg.SetAcqYear(strAcqYear);
	dlg.SetGroupCollection(m_pGroupCollection);
	if(IDOK == dlg.DoModal())
	{
		strGroupNo = dlg.GetSelInfo();
		strAcqYear = m_pGroupCollection->GetAcqYear();
	}
	else
		return ;

	if(strGroupNo.IsEmpty()) return;
	
	INT pos = strGroupNo.Find(_T(","), 0);
	if(pos >= 0)
	{
		strGroupNo = strGroupNo.Mid(0, pos);
		strGroupNo.TrimLeft();	strGroupNo.TrimRight();
	}

	m_pCM->SetControlMgrData(_T("������ȣ"), strGroupNo);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_strAcqYear = strAcqYear;
	m_pGroupCollection->AddGroupInfo(m_strAcqYear, strGroupNo);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
	OnKillfocusedtDIVNO();	
}

VOID CRequisitionNo_Select_Dlg::OnOK() 
{
	// TODO: Add extra validation here	
	if(!m_pGroup)
	{
		ESLAfxMessageBox(_T("�Է��� ������ȣ ������ �ùٸ��� �ʽ��ϴ�."));
		return ;
	}

	CDialog::OnOK();
}

BOOL CRequisitionNo_Select_Dlg::GetDonatorInfo()
{
	return FALSE;
}

CString CRequisitionNo_Select_Dlg::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

VOID CRequisitionNo_Select_Dlg::OnbtnREQNOCREATE() 
{
	// TODO: Add your control notification handler code here
	CRequisitionNo_Insert_Dlg dlg(this);
	dlg.SetOpenMode(DINSERT);
	dlg.SetAcqYear(m_strAcqYear);
	if(IDOK == dlg.DoModal())
	{
		CString strAddGroupNo;
		dlg.GetInsertInfo(strAddGroupNo);
		m_pGroupCollection->AddGroupInfo(m_strAcqYear, strAddGroupNo);
		m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
		m_pCM->SetControlMgrData(_T("������ȣ"), m_pGroup->GetGroupInfo());
	}	
}

CGroupInfo * CRequisitionNo_Select_Dlg::GetSelGroup()
{
	return m_pGroup;
}

BOOL CRequisitionNo_Select_Dlg::PreTranslateMessage(MSG *pMsg)
{
	if (pMsg->message == WM_CHAR)
    {
		INT nVirtKey = (INT) pMsg->wParam;
		if ((nVirtKey < '0' || nVirtKey > '9') && nVirtKey != 8)
		{
			ESLAfxMessageBox(_T("���ڸ� �Է��Ͻ� �� �ֽ��ϴ�.")); 
            return TRUE;
		}
    }
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CRequisitionNo_Select_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
