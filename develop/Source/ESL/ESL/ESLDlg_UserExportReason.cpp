// ESLDlg_UserExportReason.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ESLDlg_UserExportReason.h"
//#include "afxdialogex.h"
#include "imm.h"

// CESLDlg_UserExportReason ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CESLDlg_UserExportReason, CDialog)

CESLDlg_UserExportReason::CESLDlg_UserExportReason(CWnd* pParent /*=NULL*/)
	: CDialog(CESLDlg_UserExportReason::IDD, pParent)
{

}

CESLDlg_UserExportReason::~CESLDlg_UserExportReason()
{
}

void CESLDlg_UserExportReason::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CESLDlg_UserExportReason, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_EDT_USER_EXPORT_REASON, OnSetfocusEdtUserExportReason)
END_MESSAGE_MAP()


// CESLDlg_UserExportReason �޽��� ó�����Դϴ�.


void CESLDlg_UserExportReason::OnBnClickedOk()
{
	GetDlgItem(IDC_EDT_USER_EXPORT_REASON)->GetWindowText(m_strReason);
	if(m_strReason.IsEmpty() == TRUE)
	{
		CString strMsg;
		strMsg.Format(_T("����� �Ͻ÷��� ������ �����ּž� �մϴ�."));
		AfxMessageBox(strMsg);
		{
			return;
		}
	}
	else if (m_strReason.GetLength() < 3)
	{
		CString strMsg;
		strMsg.Format(_T("3���� �̻� �Է����ֽñ� �ٶ��ϴ�."));
		AfxMessageBox(strMsg);
		return;
		
	}

	wchar_t c;
	for (INT i = 0; i < m_strReason.GetLength() ; i++)
	{
		c = m_strReason.GetAt(i);
		if( c > 32 && c < 127 
			&& !((c >= 'a' && c <= 'z') || (c >= 'A'&& c <= 'Z'))
			&& !( c >= '0' && c <= '9') ) 
		{
			CString strMsg;
			strMsg.Format(_T("Ư�����ڴ� �Է��� �� �����ϴ�."));
			AfxMessageBox(strMsg);
			return;
		}
	}
	
	CDialog::OnOK();
}


void CESLDlg_UserExportReason::OnSetfocusEdtUserExportReason()
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;
	
    hIME = ::ImmGetContext(GetDlgItem(IDC_EDT_USER_EXPORT_REASON)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;
		
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
		
        ImmReleaseContext(GetDlgItem(IDC_EDT_USER_EXPORT_REASON)->m_hWnd,hIME); 
    }
}
