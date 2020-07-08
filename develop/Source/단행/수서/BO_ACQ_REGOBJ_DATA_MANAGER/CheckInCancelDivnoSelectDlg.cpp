// CheckInCancelDivnoSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckInCancelDivnoSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckInCancelDivnoSelectDlg dialog


CCheckInCancelDivnoSelectDlg::CCheckInCancelDivnoSelectDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckInCancelDivnoSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckInCancelDivnoSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCheckInCancelDivnoSelectDlg::~CCheckInCancelDivnoSelectDlg()
{
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}

	m_Dlg_brush.DeleteObject();
}


VOID CCheckInCancelDivnoSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckInCancelDivnoSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckInCancelDivnoSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CCheckInCancelDivnoSelectDlg)
	ON_CBN_SELCHANGE(IDC_cmbORDER_CANCEL_WORK_CODE, OnSelchangecmbORDERCANCELWORKCODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckInCancelDivnoSelectDlg message handlers

BOOL CCheckInCancelDivnoSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_�����������")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_�����������"));
	m_pDM = FindDM(_T("DM_���Ե�ϴ���ڷ����"));

	m_pComboMgr = new CMulitComboBoxManager(this);
	m_pOrderCancelGroupCollection = new CGroupCollection(this);

	if(!Init())
	{
		ESLAfxMessageBox(_T("�ֹ���� ���� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCheckInCancelDivnoSelectDlg::Init()
{
	INT nGroupCnt = m_pGroupCollection->GetGroupCount();
	if(nGroupCnt == 0) return FALSE;
	CStringArray arrGroupInfo;
	CString strTmpData, strWorkCode;
	strWorkCode = m_pGroupCollection->GetWorkCode();
	m_pCM->SetControlMgrData(_T("��������"), strWorkCode);
	m_pGroupCollection->GetGroupInfo(&arrGroupInfo, strTmpData);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();

	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &arrGroupInfo, _T("��������"));
	return TRUE;
}

VOID CCheckInCancelDivnoSelectDlg::OnSelchangecmbORDERCANCELWORKCODE() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("��������"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroupCollection->SetCurSel(nCurSel);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();	
}

VOID CCheckInCancelDivnoSelectDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_pGroup == NULL) 
	{
		ESLAfxMessageBox(_T("���� ���������� �ùٸ��� �ʽ��ϴ�."));
		return ;
	}
	CString strMsg;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Group"), (DWORD*)m_pGroup);
	INT ids = api.SPFExecutePath(_T("�����ڷ�˼����"));
	if(ids < 0)
	{
		strMsg.Format(_T("%s�⵵ %s �����ڷḦ �˼���� �� �� �����ϴ�."), m_pGroup->GetAcqYear(), m_pGroup->GetGroupInfo());
		ESLAfxMessageBox(strMsg);
		return ;
	}
	else if(ids > 0)
	{
		strMsg.Format(_T("%d���� å������ ������� �ΰ�Ǿ����ϴ�. \n%s-%s-%s������ �˼���� �ɼ� �����ϴ�."),
			ids, m_pGroup->GetAcqYear(), m_pGroup->GetWorkCode(), m_pGroup->GetGroupInfo());
		ESLAfxMessageBox(strMsg);
	}
	else
	{
		CGroupInfo Group;
		m_pGroup->CopyData(&Group);
		m_pOrderCancelGroupCollection->AddGroupInfo(&Group);
		m_pGroupCollection->DeleteThisGroupInfo(m_pGroup);
	}
	if(!Init())
	{
		EndDialog(IDOK);
	}	
}

HBRUSH CCheckInCancelDivnoSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=NULL;
	if(pWnd->m_hWnd==this->m_hWnd) hbr= HBRUSH(m_Dlg_brush);	
	else hbr =CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG) 
	{
		pDC->SetTextColor(RGB(0,0,0));		
		pDC->SetBkMode(OPAQUE);		
		pDC->SetBkColor(RGB(239,239,239));		
		
		return m_Dlg_brush;		
	}

	return hbr;
}