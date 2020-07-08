// DropRegNoApplyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DropRegNoApplyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDropRegNoApplyDlg dialog


CDropRegNoApplyDlg::CDropRegNoApplyDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDropRegNoApplyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDropRegNoApplyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_bCreate = FALSE;
}
CDropRegNoApplyDlg::~CDropRegNoApplyDlg()
{
	
}

VOID CDropRegNoApplyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDropRegNoApplyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDropRegNoApplyDlg, CDialog)
	//{{AFX_MSG_MAP(CDropRegNoApplyDlg)
	ON_BN_CLICKED(IDC_btnDROP_REG_NO_SEARCH, OnbtnDROPREGNOSEARCH)
	ON_WM_CTLCOLOR()   
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropRegNoApplyDlg message handlers

BOOL CDropRegNoApplyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_������Ϲ�ȣ����")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_������Ϲ�ȣ����"));	
	m_pDM = FindDM(_T("DM_������Ϲ�ȣ����"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("������Ϲ�ȣ ���� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}


	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDropRegNoApplyDlg::Create(CWnd *pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

VOID CDropRegNoApplyDlg::OnbtnDROPREGNOSEARCH() 
{
	// TODO: Add your control notification handler code here
	m_pCM->GetControlMgrData(_T("�ڷᱸ��"), m_strDataClass);
	m_pCM->GetControlMgrData(_T("��ϱ���"), m_strRegCode);
	m_pCM->GetControlMgrData(_T("��Ϲ�ȣ1"), m_strRegNo1);
	m_pCM->GetControlMgrData(_T("��Ϲ�ȣ2"), m_strRegNo2);

	if(m_strDataClass.IsEmpty())
	{
		ESLAfxMessageBox(_T("����/���� ���а� ������ �ùٸ��� �ʽ��ϴ�."));
		return;
	}
	if(m_strDataClass == _T("����")) m_strDataClass = _T("MO");
	else if(m_strDataClass == _T("����")) m_strDataClass = _T("SE");

	if(m_strRegCode.IsEmpty())
	{
		ESLAfxMessageBox(_T("��ϱ��а��� �ùٸ��� �ʽ��ϴ�."));
		return;
	}

	CString strQuery, strOption;
	strQuery.Format(_T("PUBLISH_FORM_CODE='%s' AND REG_CODE='%s'"), m_strDataClass, m_strRegCode);

	INT nRegNo1, nRegNo2, nTmpData;
	nRegNo1 = _ttoi(m_strRegNo1.GetBuffer(0));
	nRegNo2 = _ttoi(m_strRegNo2.GetBuffer(0));

	if(nRegNo1 > nRegNo2)
	{
		nTmpData = nRegNo1;
		nRegNo1 = nRegNo2;
		nRegNo2 = nTmpData;
	}

	if(nRegNo1 > 0 && nRegNo2 > 0)
	{
		strOption.Format(_T(" AND REG_NO BETWEEN %d AND %d"), nRegNo1, nRegNo2);
		strQuery += strOption;
	}
	else if(nRegNo1 > 0 && nRegNo2 <= 0)
	{
		strOption.Format(_T(" AND REG_NO >= %d"), nRegNo1);
		strQuery += strOption;
	}
	else if(nRegNo1 <= 0 && nRegNo2 > 0)
	{
		strOption.Format(_T(" AND REG_NO <= %d"), nRegNo2);
		strQuery += strOption;
	}

	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������Ϲ�ȣ�� �˻��� �� �����ϴ�."));
		return;
	}

	m_pGrid->Display();
}

VOID CDropRegNoApplyDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(!CheckSelectValid())
	{
		ESLAfxMessageBox(_T("���������� �ùٸ��� �ʽ��ϴ�."));
		return;
	}

	INT ids = m_pGrid->SelectMakeList();
	ids = m_pGrid->SelectGetHeadPosition();
	INT nTmpData;
	CString strTmpData;
	while(ids >= 0)
	{
		strTmpData = m_pDM->GetCellData(_T("��Ϲ�ȣ"), ids);
		nTmpData = _ttoi(strTmpData.GetBuffer(0));
		m_ArrSelRegNo.Add(nTmpData);
		ids = m_pGrid->SelectGetNext();
	}
	CDialog::OnOK();
}

BOOL CDropRegNoApplyDlg::CheckSelectValid()
{
	if(m_pDM->GetRowCount() == 0) return FALSE;

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return FALSE;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return FALSE;

	return TRUE;
}

BOOL CDropRegNoApplyDlg::Init()
{
	if(m_strDataClass.IsEmpty() || m_strRegCode.IsEmpty())
		return FALSE;

	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�ڷᱸ��"));
	if(pCMB == NULL) return FALSE;
	pCMB->ResetContent();
	pCMB->AddString(_T("����"));
	pCMB->AddString(_T("����"));

	if(m_strDataClass == _T("MO")) pCMB->SetCurSel(0);
	else pCMB->SetCurSel(1);
	m_pCM->SetControlMgrData(_T("��ϱ���"), m_strRegCode);
	
	return TRUE;
}




HBRUSH CDropRegNoApplyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

