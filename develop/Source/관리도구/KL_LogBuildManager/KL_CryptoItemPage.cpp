// KL_CryptoItemPage.cpp : implementation file
//

#include "stdafx.h"
#include "KL_CryptoManager.h"
#include "KL_CryptoItemPage.h"
#include "KL_CryptoInfo.h"

#include "ESL_Mgr.h"

#define TIMER_ANALYZE_LIBRARYINFO	100

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CKL_CryptoInfo* theInfo;

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoItemPage dialog
IMPLEMENT_DYNAMIC(CKL_CryptoItemPage, CPropertyPage)

CKL_CryptoItemPage::CKL_CryptoItemPage()
	: CPropertyPage(CKL_CryptoItemPage::IDD)
{
	m_bWarnning = FALSE;
}


void CKL_CryptoItemPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_CryptoItemPage)
	DDX_Control(pDX, IDC_EDT_KOLAS_USER_RESULT, m_edtManagerResult);
	DDX_Control(pDX, IDC_EDT_LOAN_USER_RESULT, m_edtUserResult);
	DDX_Control(pDX, IDC_CHK_MANAGER_PASSWD, m_btnManagerPasswd);
	DDX_Control(pDX, IDC_CHK_USER_PASSWD, m_btnUserPasswd);
	DDX_Control(pDX, IDC_CHK_CARD_PASSWD, m_btnCardPasswd);	
	//}}AFX_DATA_MAP
}

/*=============================================================
	@ Function    :	OnInitDialog
	@ Description :
=============================================================*/
BOOL CKL_CryptoItemPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

#ifdef CRYPTO_DEV
	// ���� �������� üũ���·� �����.
	m_btnManagerPasswd.SetCheck(TRUE);
	m_btnUserPasswd.SetCheck(TRUE);
	m_btnCardPasswd.SetCheck(TRUE);
	// üũ���� �ݿ��ϱ� ���ؼ� ȣ��
	OnChkOption();
#endif
	return TRUE;
}

BEGIN_MESSAGE_MAP(CKL_CryptoItemPage, CPropertyPage)
	//{{AFX_MSG_MAP(CKL_CryptoItemPage)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_MANAGER_PASSWD, OnChkOption)
	ON_BN_CLICKED(IDC_CHK_USER_PASSWD, OnChkOption)
	ON_BN_CLICKED(IDC_CHK_CARD_PASSWD, OnChkOption)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoItemPage message handlers

/*=============================================================
	@ Function    :	OnShowWindow
	@ Description :
=============================================================*/
void CKL_CryptoItemPage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	if (bShow)
	{
		SetTimer(TIMER_ANALYZE_LIBRARYINFO, 100, NULL);
	}
}

/*=============================================================
	@ Function    :	OnTimer
	@ Description :
=============================================================*/
void CKL_CryptoItemPage::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TIMER_ANALYZE_LIBRARYINFO:
		KillTimer(TIMER_ANALYZE_LIBRARYINFO);
		
		if (AnalyzeLibraryInfo())
		{
			m_bWarnning = FALSE;
			GetDlgItem(IDC_STATIC_INFO_STATUS)->SetWindowText(_T("������ DB���� �м��� �Ϸ�Ǿ����ϴ�"));			
		}
		else
		{
			m_bWarnning = TRUE;
			GetDlgItem(IDC_STATIC_INFO_STATUS)->SetWindowText(_T("������ DB������ �м��ϴ� �߿� ������ �߻��߽��ϴ�!"));			
		}

		if (theInfo->GetCryptoDisableOption() == CRYPTO_OPT_ALL)
		{
			GetDlgItem(IDC_STATIC_ITEM)->SetWindowText(_T("��� �׸���� ��ȣȭ �۾��� �Ϸ�� �����Դϴ�"));			
		}
		else
		{
			GetDlgItem(IDC_STATIC_ITEM)->SetWindowText(_T("��ȣȭ �׸��� ������ �ֽʽÿ�"));
		}
		break;
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}

/*=============================================================
	@ Function    :	AnalyzeLibraryInfo
	@ Description :
=============================================================*/
BOOL CKL_CryptoItemPage::AnalyzeLibraryInfo()
{	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( theInfo->GetConnectInfo() );

	// �ڶ�3 ����� �� Ȯ��
	CString strResult;
	int ids = dm.GetOneValueQuery(_T("SELECT COUNT(1) FROM MN_USER_TBL"), strResult);
	if (ids < 0)
	{
		// ���� ������ �ٽ� �ؾ��Ѵ�.
		return FALSE;
	}
	// ����� ����Ʈ��Ʈ�ѿ� �ݿ�
	m_edtManagerResult.SetWindowText(strResult);
	// �����ȹ�� ����� ���ؼ� ���� ����.
	theInfo->SetUserCount((UINT)_ttoi(strResult));

	// ����/�ݳ� �̿��� �� Ȯ��
	ids = dm.GetOneValueQuery(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL"), strResult);
	if (ids < 0)
	{
		// ������??
		return FALSE;
	}
	// ����� ����Ʈ��Ʈ�ѿ� �ݿ�
	m_edtUserResult.SetWindowText(strResult);
	// �����ȹ�� ����� ���ؼ� ���� ����.
	theInfo->SetLoanUserCount((UINT)_ttoi(strResult));

	// ��ȣȭ�� ������ �׸��� �ִ��� Ȯ��
	{
		UINT nDisableOpt = 0;

		// ����� ��ȣȭ ����
		strResult.Empty();
#ifdef CRYPTO_DEV
		ids = dm.GetOneValueQuery(_T("SELECT USE_YN_TEST FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE=\'USERPWENCRYPTION\'"), strResult);
#else
		ids = dm.GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE=\'USERPWENCRYPTION\'"), strResult);
#endif
		if (ids < 0 && ids != -4007)
		{
			// ������??
			return FALSE;
		}
		if (strResult.CompareNoCase(_T("Y")) == 0)
		{
			m_btnManagerPasswd.SetCheck(FALSE);
			m_btnManagerPasswd.EnableWindow(FALSE);

			nDisableOpt += (UINT)CRYPTO_OPT_USER_PASSWD;
		}

		// �̿��� ��ȣȭ ����
		strResult.Empty();
#ifdef CRYPTO_DEV
		ids = dm.GetOneValueQuery(_T("SELECT USE_YN_TEST FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE=\'VISITORPWENCRYPTION\'"), strResult);
#else
		ids = dm.GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE=\'VISITORPWENCRYPTION\'"), strResult);
#endif
		if (ids < 0 && ids != -4007)
		{
			// ������??
			return FALSE;
		}
		if (strResult.CompareNoCase(_T("Y")) == 0)
		{
			m_btnUserPasswd.SetCheck(FALSE);
			m_btnUserPasswd.EnableWindow(FALSE);

			nDisableOpt += (UINT)CRYPTO_OPT_LOAN_USER_PASSWD;
		}
		
		// ȸ���� ��ȣȭ ����
		strResult.Empty();
#ifdef CRYPTO_DEV
		ids = dm.GetOneValueQuery(_T("SELECT USE_YN_TEST FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE=\'CARDPWCRYPTION\'"), strResult);
#else
		ids = dm.GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE=\'CARDPWCRYPTION\'"), strResult);
#endif
		if (ids < 0 && ids != -4007)
		{
			// ������??
			return FALSE;
		}
		if (strResult.CompareNoCase(_T("Y")) == 0)
		{
			m_btnCardPasswd.SetCheck(FALSE);
			m_btnCardPasswd.EnableWindow(FALSE);

			nDisableOpt += (UINT)CRYPTO_OPT_LOAN_USER_CARD;
		}

		theInfo->SetCryptoDisableOption(nDisableOpt);

		// ����� ������ �ִٸ� ����
		OnChkOption();
	}
	
	
	return TRUE;
}

/*=============================================================
	@ Function    :	OnChkOption
	@ Description :
=============================================================*/
void CKL_CryptoItemPage::OnChkOption() 
{
	UINT nOpt = 0;
	
	if (m_btnManagerPasswd.GetCheck())
	{
		nOpt += (UINT)CRYPTO_OPT_USER_PASSWD;
	}
	if (m_btnUserPasswd.GetCheck())
	{
		nOpt += (UINT)CRYPTO_OPT_LOAN_USER_PASSWD;
	}
	if (m_btnCardPasswd.GetCheck())
	{
		nOpt += (UINT)CRYPTO_OPT_LOAN_USER_CARD;
	}

	theInfo->SetCryptoOption(nOpt);
}

/*=============================================================
	@ Function    :	OnCtlColor
	@ Description :
=============================================================*/
HBRUSH CKL_CryptoItemPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_INFO_STATUS:
		if (m_bWarnning)
		{
			pDC->SetTextColor(RGB(0xFF, 0x00, 0x00));
		}
		else
		{
			pDC->SetTextColor(RGB(0x00, 0x00, 0x00));
		}
		break;
	case IDC_STATIC_ITEM:
		if (theInfo->GetCryptoDisableOption() == CRYPTO_OPT_ALL)
		{
			pDC->SetTextColor(RGB(0x00, 0x00, 0xFF));
		}
		else
		{
			pDC->SetTextColor(RGB(0x00, 0x00, 0x00));
		}
		break;
	}

	return hbr;
}
