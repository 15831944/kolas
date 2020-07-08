// OrderCancelDivnoSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderCancelDivnoSelectDlg.h"
#include "../../../����/BO_ACQ_API/BO_ACQ_Business_Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderCancelDivnoSelectDlg dialog


COrderCancelDivnoSelectDlg::COrderCancelDivnoSelectDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(COrderCancelDivnoSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderCancelDivnoSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGroupCollection = NULL;
	m_pOrderCancelGroupCollection = NULL;
	m_pGroup = NULL;
	m_pComboMgr = NULL;
}

COrderCancelDivnoSelectDlg::~COrderCancelDivnoSelectDlg()
{
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}

}

VOID COrderCancelDivnoSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderCancelDivnoSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderCancelDivnoSelectDlg, CDialog)
	//{{AFX_MSG_MAP(COrderCancelDivnoSelectDlg)
	ON_CBN_SELCHANGE(IDC_cmbDIVNO_INFO, OnSelchangecmbDIVNOINFO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderCancelDivnoSelectDlg message handlers

BOOL COrderCancelDivnoSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("K2UPGRADE_�����������")) < 0 ) 
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_�����������"));
	m_pDM = FindDM(_T("DM_�ֹ��ڷ����"));
	m_pComboMgr = new CMulitComboBoxManager(this);
	m_pOrderCancelGroupCollection = new CGroupCollection(this);

	if( m_pCM == NULL || m_pDM == NULL || m_pComboMgr == NULL || m_pOrderCancelGroupCollection == NULL || 
		Init() == FALSE )
	{
		ESLAfxMessageBox(_T("�ֹ���� ���� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

VOID COrderCancelDivnoSelectDlg::OnOK() 
{
	if( m_pGroup == NULL )
	{
		ESLAfxMessageBox(_T("���� ���������� �ùٸ��� �ʽ��ϴ�."));
		return ;
	}

	CString strMsg;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Group"), (DWORD*)m_pGroup);
	INT ids = api.SPFExecutePath(_T("�����ڷ��ֹ����"));
	if( ids < 0 )
	{
		strMsg.Format(_T("%s�⵵ %s �����ڷḦ �ֹ���� �� �� �����ϴ�."), m_pGroup->GetAcqYear(), m_pGroup->GetGroupInfo());
		ESLAfxMessageBox(strMsg);
		return ;
	}
	else
	{
		CGroupInfo Group;
		m_pGroup->CopyData(&Group);
		m_pOrderCancelGroupCollection->AddGroupInfo(&Group);
		strMsg.Format(_T("������ȣ [%s]���� �ֹ�����Ͽ����ϴ�."), m_pGroup->GetGroupInfo());
		m_pGroupCollection->DeleteThisGroupInfo(m_pGroup);
		ESLAfxMessageBox(strMsg);
	}

	if( Init() == FALSE )
	{
		EndDialog(IDOK);
	}
}

BOOL COrderCancelDivnoSelectDlg::Init()
{
	INT nGroupCnt = m_pGroupCollection->GetGroupCount();
	if( nGroupCnt == 0 )	return FALSE;

	CStringArray arrGroupInfo;
	CString strTmpData, strWorkCode;

	strWorkCode = m_pGroupCollection->GetWorkCode();
	m_pCM->SetControlMgrData(_T("��������"), strWorkCode);
	m_pGroupCollection->GetGroupInfo(&arrGroupInfo, strTmpData);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();

	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &arrGroupInfo, _T("��������"));

	return TRUE;
}

VOID COrderCancelDivnoSelectDlg::OnSelchangecmbDIVNOINFO() 
{
	CESL_ControlComboBox* pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("��������"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroupCollection->SetCurSel(nCurSel);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
}

HBRUSH COrderCancelDivnoSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}