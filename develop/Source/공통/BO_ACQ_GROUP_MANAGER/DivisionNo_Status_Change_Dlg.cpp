// DivisionNo_Status_Change_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DivisionNo_Status_Change_Dlg.h"
#include "GroupMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Status_Change_Dlg dialog


CDivisionNo_Status_Change_Dlg::CDivisionNo_Status_Change_Dlg(CESL_Mgr * pParent /*=NULL*/)
	: CESL_Mgr(CDivisionNo_Status_Change_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDivisionNo_Status_Change_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pArrSelIdx = NULL;
}

CDivisionNo_Status_Change_Dlg::~CDivisionNo_Status_Change_Dlg()
{
}

VOID CDivisionNo_Status_Change_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDivisionNo_Status_Change_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDivisionNo_Status_Change_Dlg, CDialog)
	//{{AFX_MSG_MAP(CDivisionNo_Status_Change_Dlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Status_Change_Dlg message handlers

BOOL CDivisionNo_Status_Change_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_�������º���")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_�������º���"));	

	if(!Init())
	{
		ESLAfxMessageBox(_T("�������º��� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDivisionNo_Status_Change_Dlg::Init()
{
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������"), m_strWorkCode, m_strWorkCode);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_strAcqYear);
	m_pCM->SetControlMgrData(_T("��������"), m_strWorkCode);
	m_pCM->SetControlMgrData(_T("�����ڷ����"), m_strWorkStatus);
	m_pCM->SetControlMgrData(_T("����"), m_strDivNo);
	CMulitComboBoxManager mgr(this);
	CStringArray arrData;	arrData.RemoveAll();
	if(m_strWorkStatus == _T("���Դ��"))
	{
		arrData.Add(_T("�ֹ�"));
		arrData.Add(_T("�˼��Ϸ�"));
	}
	else if(m_strWorkStatus == _T("�ֹ�"))
	{
		arrData.Add(_T("���Դ��"));
		arrData.Add(_T("�˼��Ϸ�"));
	}
	else if(m_strWorkStatus == _T("�˼��Ϸ�"))
	{
		arrData.Add(_T("���Դ��"));
		arrData.Add(_T("�ֹ�"));
	}
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, arrData.GetAt(0), &arrData, _T("�����ڷ����"));

	return TRUE;
}

VOID CDivisionNo_Status_Change_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strChangeStatus, strWorkCode;
	m_pCM->GetControlMgrData(_T("�����ڷ����"), strChangeStatus);
	if(strChangeStatus == _T("���Դ��")) strChangeStatus = _T("0");
	else if(strChangeStatus == _T("�ֹ�")) strChangeStatus = _T("1");
	else if(strChangeStatus == _T("�˼��Ϸ�")) strChangeStatus = _T("2");
	m_pInfo->pCodeMgr->ConvertDescToCode(_T("��������"), m_strWorkCode, strWorkCode);
	CGroupMgr mgr(this);
	mgr.InitTempData();	
	mgr.InitTempPtr();

	mgr.SetTempData(_T("���Գ⵵"), m_strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("��������"), strWorkCode.GetBuffer(0));
	mgr.SetTempData(_T("��������"), m_strDivNo.GetBuffer(0));
	mgr.SetTempData(_T("�������"), strChangeStatus.GetBuffer(0));
	mgr.SetTempPtr(_T("SELECT_INDEX"), (DWORD*)m_pArrSelIdx);
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids = mgr.SPFExecutePath(_T("�������º���"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ ������ ���¸� ������ �� �����ϴ�."));
		return;
	}
	else if(ids == 1000)
	{
		ESLAfxMessageBox(_T("������ ������ ���Ե� �ڷᰡ �����մϴ�. �� �˻��Ͽ� Ȯ���� �ֽʽÿ�."));
	}
	CDialog::OnOK();
}

HBRUSH CDivisionNo_Status_Change_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
