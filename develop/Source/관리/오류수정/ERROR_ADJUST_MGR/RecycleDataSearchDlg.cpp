// RecycleDataSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RecycleDataSearchDlg.h"
#include "../../../����/IndexToolKit/IndexTool.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecycleDataSearchDlg dialog


CRecycleDataSearchDlg::CRecycleDataSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CErrorAdjustSearchMgr(CRecycleDataSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecycleDataSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
}
CRecycleDataSearchDlg::~CRecycleDataSearchDlg()
{
	
}


VOID CRecycleDataSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecycleDataSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecycleDataSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CRecycleDataSearchDlg)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecycleDataSearchDlg message handlers

BOOL CRecycleDataSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_��Ȱ�뺹���˻�")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_��Ȱ�뺹���˻�"));	
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("��Ȱ���ڷ� �˻� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRecycleDataSearchDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CRecycleDataSearchDlg::Init()
{
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�����۾�����"));
	if(pCMB == NULL) return FALSE;
	pCMB->ResetContent();
	pCMB->AddString(_T("��ü�ڷ�"));
	pCMB->AddString(_T("�����ڷ�"));
	pCMB->AddString(_T("����ڷ�"));
	pCMB->AddString(_T("�����ڷ�"));
	pCMB->AddString(_T("�����ڷ�"));
	
	pCMB->SetCurSel(0);

	return TRUE;
}

VOID CRecycleDataSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strWorkStatus, strDelDate1, strDelDate2, strTitle;
	m_pCM->GetControlMgrData(_T("�����۾�����"), strWorkStatus);
	m_pCM->GetControlMgrData(_T("��ǥ��"), strTitle);
	m_pCM->GetControlMgrData(_T("��¥1"), strDelDate1);
	m_pCM->GetControlMgrData(_T("��¥2"), strDelDate2);

	if(strDelDate1.IsEmpty() && !strDelDate2.IsEmpty())
	{
		ESLAfxMessageBox(_T("�˻� �������� �Է��� �ּ���."));
		return ;
	}

	CString strQuery, strOption;
	strQuery = _T("(I.SPECIES_CLASS = '2' OR I.SPECIES_CLASS = '3') AND I.REC_KEY = S.REC_KEY AND I.REC_KEY = B.SPECIES_KEY AND I.REC_KEY = P.SPECIES_KEY(+) ");
	//����ȭ
	if(!strTitle.IsEmpty())
	{
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTitle);
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(strTitle);
		if(m_pInfo->GetSearchIndexType() == ORA_CATSEARCH)
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_TITLE, '%s*', NULL)>0"), strTitle);
			strQuery += strOption;
		}
		else if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*') "), strTitle);
			strQuery += strOption;
		}
	}
	strOption = MakeDateCondition_Stmt(_T("I.DELETE_DATE"), strDelDate1, strDelDate2);
	if(strOption == _T("-1")) return ;
	strQuery += strOption;
	
	strOption.Empty();

	if(strWorkStatus == _T("�����ڷ�"))
		strOption.Format(_T(" AND I.WORKING_STATUS > 'BOA' AND I.WORKING_STATUS < 'BOC'"));
	else if(strWorkStatus == _T("����ڷ�"))
		strOption.Format(_T(" AND I.WORKING_STATUS > 'BOR' AND I.WORKING_STATUS < 'BOT'"));
	else if(strWorkStatus == _T("�����ڷ�"))
		strOption.Format(_T(" AND I.WORKING_STATUS > 'BOC' AND I.WORKING_STATUS < 'BOL'"));
	else if(strWorkStatus == _T("�����ڷ�"))
		strOption.Format(_T(" AND I.WORKING_STATUS > 'BOL' AND I.WORKING_STATUS < 'BOR'"));

	strQuery += strOption;
	m_strQuery = strQuery;
	SendQueryCreateMsg();
}



HBRUSH CRecycleDataSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}