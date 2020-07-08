// SeDuplicatedVolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeDuplicatedVolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeDuplicatedVolDlg dialog


CSeDuplicatedVolDlg::CSeDuplicatedVolDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeDuplicatedVolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeDuplicatedVolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pCM = NULL;
	m_nCurrentIdx = -1;
	m_strVOL_TITLE = _T("");

}

CSeDuplicatedVolDlg::~CSeDuplicatedVolDlg()
{
}

VOID CSeDuplicatedVolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeDuplicatedVolDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeDuplicatedVolDlg, CDialog)
	//{{AFX_MSG_MAP(CSeDuplicatedVolDlg)
	ON_BN_CLICKED(IDC_btnDUP_NEWVOL, OnbtnDUPNEWVOL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeDuplicatedVolDlg message handlers

BOOL CSeDuplicatedVolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("����_����_üũ��_��ȣ��������"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_üũ��_��ȣ��������") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_����_��ȣ��������"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���"));
	m_pDM = FindDM(_T("DM_üũ��_��"));
	

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : ����_����_üũ��_��ȣ��������") );
		return false;
	}	
	
	
	m_pCM->SetControlMgrData(_T("��ȣ��"),m_strVOL_TITLE);

	INT nRowCnt = m_pDM->GetRowCount();
	CString msg;
	msg.Format(_T("�� [ %d ] ���� ��ȣ ������������ �ֽ��ϴ�."),nRowCnt);
	GetDlgItem(IDC_stcDUP_INSPECT_RESULT)->SetWindowText(msg);
	m_pGrid->Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CSeDuplicatedVolDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeDuplicatedVolDlg)
	ON_EVENT(CSeDuplicatedVolDlg, IDC_gridDUP, -600 /* Click */, OnClickgridDUP, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeDuplicatedVolDlg::OnClickgridDUP() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		m_nCurrentIdx = row-1;
	
	}
}

VOID CSeDuplicatedVolDlg::OnOK() 
{
	// TODO: Add extra validation here
	// ���� ��ȣ ���� 	
	if(m_nCurrentIdx<0) {
		AfxMessageBox(_T("���õ� ��ȣ�� �����ϴ�!"));
		return;
	}
	

	CDialog::OnOK();
}

VOID CSeDuplicatedVolDlg::OnbtnDUPNEWVOL() 
{
	// TODO: Add your control notification handler code here
	// ����ȣ�� �Է� 
	EndDialog(11);

}

VOID CSeDuplicatedVolDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	// �۾����� 
	CDialog::OnCancel();
}

HBRUSH CSeDuplicatedVolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
