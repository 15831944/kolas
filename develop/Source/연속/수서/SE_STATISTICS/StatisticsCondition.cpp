// StatisticsCondition.cpp : implementation file
//

#include "stdafx.h"
#include "StatisticsCondition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsCondition dialog


CStatisticsCondition::CStatisticsCondition(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CStatisticsCondition::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatisticsCondition)
	m_bIsDirectPrint = FALSE;
	//}}AFX_DATA_INIT
	
	m_pCM			= NULL;
	m_strCondition	= _T("");
	m_strDate1		= _T("");
	m_strDate2		= _T("");
	m_strInputDate1 = _T("");
	m_strInputDate2 = _T("");

}

CStatisticsCondition::~CStatisticsCondition()
{
}

VOID CStatisticsCondition::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsCondition)
	DDX_Control(pDX, IDC_cmbSTC_CONDITION, m_ctrlcmbCondition);
	DDX_Check(pDX, IDC_chkSTC, m_bIsDirectPrint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsCondition, CDialog)
	//{{AFX_MSG_MAP(CStatisticsCondition)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsCondition message handlers

BOOL CStatisticsCondition::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("����_����_���_����") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_���_����") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_����_����_���_����"));
	if(!m_pCM) 	{
		AfxMessageBox(_T("CM Error : CM_����_����_���_����"));
		return FALSE;
	}		
	
	
	m_ctrlcmbCondition.InsertString(0,_T("�����"));
	m_ctrlcmbCondition.InsertString(1,_T("�ڷᱸ��"));
	m_ctrlcmbCondition.InsertString(2,_T("��ϱ���"));
	m_ctrlcmbCondition.InsertString(3,_T("�ڷ�Ǳ���"));
	m_ctrlcmbCondition.InsertString(4,_T("������ϱ���"));

	UserDefineCodeYN();
	
	m_ctrlcmbCondition.SetCurSel(0);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CStatisticsCondition::UserDefineCodeYN()
{
	CString strYN, strAlias;

	CESL_DataMgr TmpDM;		
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	
	strYN.Empty();
	TmpDM.GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 103"), strYN);
	if(strYN == _T("Y"))
	{
		//=====================================================
		//2009.01.29 UPDATE BY PJW : strAlias�� ��������� �߰����� �ʴ´�.
// 		m_ctrlcmbCondition.AddString(strAlias);	
		if(!strAlias.IsEmpty())
		{
			m_ctrlcmbCondition.AddString(strAlias);	
		}
		//=====================================================
	}

	strYN.Empty();
	TmpDM.GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 101"), strYN);
	if(strYN == _T("Y"))
	{
		TmpDM.GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 101"), strAlias);
		//=====================================================
		//2009.01.29 UPDATE BY PJW : strAlias�� ��������� �߰����� �ʴ´�.
// 		m_ctrlcmbCondition.AddString(strAlias);	
		if(!strAlias.IsEmpty())
		{
			m_ctrlcmbCondition.AddString(strAlias);	
		}
		//=====================================================
		
	}

	strYN.Empty();
	TmpDM.GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 102"), strYN);
	if(strYN == _T("Y"))
	{
		TmpDM.GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 102"), strAlias);
		//=====================================================
		//2009.01.29 UPDATE BY PJW : strAlias�� ��������� �߰����� �ʴ´�.
// 		m_ctrlcmbCondition.AddString(strAlias);	
		if(!strAlias.IsEmpty())
		{
			m_ctrlcmbCondition.AddString(strAlias);	
		}
		//=====================================================
		
	}
}

VOID CStatisticsCondition::OnOK() 
{
	// TODO: Add extra validation here

	m_ctrlcmbCondition.GetWindowText(m_strCondition);
	m_pCM->GetControlMgrData(_T("�Լ���1"),m_strDate1);
	m_pCM->GetControlMgrData(_T("�Լ���2"),m_strDate2);
	m_pCM->GetControlMgrData(_T("�Է���1"),m_strInputDate1);
	m_pCM->GetControlMgrData(_T("�Է���2"),m_strInputDate2);
	UpdateData(TRUE);

	
	CDialog::OnOK();
}

BOOL CStatisticsCondition::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			OnOK(); 
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CStatisticsCondition::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
