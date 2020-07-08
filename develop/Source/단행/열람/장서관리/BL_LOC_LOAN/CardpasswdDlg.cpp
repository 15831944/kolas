#include "stdafx.h"
#include "CardpasswdDlg.h"

#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCardpasswdDlg::CCardpasswdDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCardpasswdDlg::IDD, pParent)
{
	
	m_strInputData = _T("");
	m_strMsg = _T("[��й�ȣ�� �Է��ϼ���]");
	m_nStatus = 0;
	
	m_bReg = FALSE;
	m_nDiscord = 0;
	m_strCardpasswd = _T("");

	
	m_pParent = pParent;
}

CCardpasswdDlg::~CCardpasswdDlg()
{	
}
VOID CCardpasswdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_MSG_MAP(CCardpasswdDlg)
	DDX_Text(pDX, IDC_EDT_CARDPASSWD, m_strInputData);
	DDX_Text(pDX, IDC_SttMSG_CARDPASSWD, m_strMsg);	
	DDV_MaxChars(pDX, m_strInputData, 20);
	//}}AFX_MSG_MAP
}


BEGIN_MESSAGE_MAP(CCardpasswdDlg, CDialog)
	//{{AFX_MSG_MAP(CCardpasswdDlg)
	ON_BN_CLICKED(IDC_btnREG_CARDPASSWD, OnReg)
	ON_BN_CLICKED(IDOK_CARDPASSWD, OnConfirm)
	ON_BN_CLICKED(IDC_btnIgnore, OnIgnore)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_EDT_CARDPASSWD, OnChangeEdtCardpasswd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

VOID CCardpasswdDlg::OnReg() 
{
	if( m_bReg == TRUE )
	{
		return;
	}
	else
	{
		if( CheckCardpasswd() == FALSE )	return;

		m_bReg = TRUE;

		GetDlgItem(IDC_btnREG_CARDPASSWD)->EnableWindow(FALSE);
		GetDlgItem(IDOK_CARDPASSWD)->EnableWindow(TRUE);

		m_strCardpasswd = m_strInputData;
		
		if(TRUE == m_pParent->m_pInfo->m_bCardPwEncryption && 64 > m_strCardpasswd.GetLength())

		{
			m_strCardpasswd = CLocCommonAPI::StringToSHA256(m_strCardpasswd);
		}

		m_strInputData.Empty();

		m_strMsg = _T("[����� ��й�ȣ�� �ѹ� �� �Է��ϼ���]");
		GetDlgItem(IDC_EDT_CARDPASSWD)->SetFocus();
		UpdateData(FALSE);

		m_nStatus = 1;
	}
}

VOID CCardpasswdDlg::OnConfirm()  
{
	if( CheckCardpasswd() == FALSE )	return;	
	
	if(TRUE == m_pParent->m_pInfo->m_bCardPwEncryption && 64 > m_strInputData.GetLength())
	{
		m_strInputData = CLocCommonAPI::StringToSHA256(m_strInputData);
	}

	if( m_strCardpasswd != m_strInputData )
	{		
		m_nDiscord++;
		m_strMsg.Format(_T("[��й�ȣ�� ��ġ���� �ʽ��ϴ�(%d/3)]"), m_nDiscord);

		m_strInputData.Empty();

		if( m_nDiscord == 3 )
		{
			m_nDiscord = 0;

			if( IDYES == AfxMessageBox(_T("��й�ȣ�� ��ġ���� �ʽ��ϴ�.\n\n���ο� ��й�ȣ�� ����Ͻðڽ��ϱ�?"), MB_ICONQUESTION | MB_YESNO ) )
			{
				GetDlgItem(IDC_btnREG_CARDPASSWD)->EnableWindow(TRUE);
				GetDlgItem(IDOK_CARDPASSWD)->EnableWindow(FALSE);
				
				m_strMsg = _T("[���ο� ��й�ȣ�� �Է��ϼ���]");
				SetDlgItemText(IDC_btnREG_CARDPASSWD, _T("�Է�"));
			}
			else
			{
				m_strMsg = _T("[��й�ȣ�� �Է��ϼ���]");
			}
		}

		UpdateData(FALSE);
	}
	else	
	{
		m_strMsg = _T("[��й�ȣ�� Ȯ���Ͽ����ϴ�]");
		
		
		if(FALSE == m_pParent->m_pInfo->m_bCardPwEncryption)
		{
			UpdateData(FALSE);
		}
		CDialog::OnOK();
	}
}

BOOL CCardpasswdDlg::CheckCardpasswd()
{
	UpdateData(TRUE);
	
	INT		nLength		=	m_strInputData.GetLength();
	if( 4 > nLength || 20 < nLength )
	{
		AfxMessageBox(_T("ȸ���� ��й�ȣ�� 4�ڸ� �̻�, 20�ڸ� ���Ϸ� �Է��Ͽ� �ֽʽÿ�."), MB_ICONWARNING);
		m_strInputData.Empty();
		UpdateData(FALSE);
		
		return FALSE;
	}

	return TRUE;
}

BOOL CCardpasswdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nStatus = 0;
	m_nDiscord = 0;
	m_bReg = FALSE;

	if( m_strCardpasswd.IsEmpty() == TRUE )
	{
		m_strMsg = _T("[���ο� ��й�ȣ�� �Է��ϼ���]");
		SetDlgItemText(IDC_SttMSG_CARDPASSWD, m_strMsg);

		GetDlgItem(IDC_btnREG_CARDPASSWD)->EnableWindow(TRUE);
		GetDlgItem(IDOK_CARDPASSWD)->EnableWindow(FALSE);

		SetDlgItemText(IDC_btnREG_CARDPASSWD, _T("�Է�"));
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	AfxGetMainWnd()->SetForegroundWindow();

	return TRUE;
}

BOOL CCardpasswdDlg::PreTranslateMessage(MSG* pMsg) 
{

	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnCancel();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CCardpasswdDlg::OnIgnore() 
{
	CDialog::OnOK();
}

HBRUSH CCardpasswdDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}

void CCardpasswdDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	
}

void CCardpasswdDlg::OnChangeEdtCardpasswd() 
{
	
}
