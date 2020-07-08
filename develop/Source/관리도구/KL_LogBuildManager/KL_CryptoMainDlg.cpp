// KL_CryptoMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KL_CryptoManager.h"
#include "KL_CryptoMainDlg.h"
#include "KL_CryptoInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CKL_CryptoInfo* theInfo;

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoMainDlg dialog


CKL_CryptoMainDlg::CKL_CryptoMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKL_CryptoMainDlg::IDD, pParent)
{
#ifdef CRYPTO_DEV
	m_nStepPage = (UINT)STEP_PAGE_CONN;
#else
	m_nStepPage = (UINT)STEP_PAGE_DESC;
#endif
}

void CKL_CryptoMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_CryptoMainDlg)
	DDX_Control(pDX, IDC_BTN_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKL_CryptoMainDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_CryptoMainDlg)
	ON_BN_CLICKED(IDC_BTN_PREV, OnBtnPrev)
	ON_BN_CLICKED(IDC_BTN_NEXT, OnBtnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoMainDlg message handlers

/*=============================================================
	@ Function    :	OnInitDialog
	@ Description :
=============================================================*/
BOOL CKL_CryptoMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitStepPage();
	
	return TRUE;
}

/*=============================================================
	@ Function    :	PreTranslateMessage
	@ Description :
=============================================================*/
BOOL CKL_CryptoMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_Step4Exec.IsRunThread() && pMsg->message == WM_KEYDOWN)
	{
		// ������ �߿��� �Է��� ���� �ʴ´�
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

/*=============================================================
	@ Function    :	OnBtnPrev
	@ Description :
=============================================================*/
void CKL_CryptoMainDlg::OnBtnPrev() 
{
	m_nStepPage--;
	ShowStepPage((STEP_PAGE_TYPE)m_nStepPage);
}

/*=============================================================
	@ Function    :	OnBtnNext
	@ Description :
=============================================================*/
void CKL_CryptoMainDlg::OnBtnNext() 
{
	// ������ �̵� ���� �ʼ� �׸���� �Է��ߴ��� Ȯ���غ���.
	switch (m_nStepPage)
	{
	case STEP_PAGE_DESC:
		// Ȯ���� ������ ����.
		break;
	case STEP_PAGE_CONN:
		// ���� ���� ������ Ȯ���Ѵ�
		{
			if (theInfo->GetConnectInfo().IsEmpty() || theInfo->GetKolasUserInfo().IsEmpty())
			{
				MessageBox(_T("�����׽�Ʈ�� ������ ��쿡�� ���� �ܰ�� ������ �� �ֽ��ϴ�"), _T("���ӷα� �ϰ����� ���α׷�"), MB_ICONEXCLAMATION);
				return;
			}
		}
		break;
	case STEP_PAGE_EXEC:
		// Ȯ���� ������ ����.
		break;
	}

	m_nStepPage++;
	ShowStepPage((STEP_PAGE_TYPE)m_nStepPage);
}

/*=============================================================
	@ Function    :	InitStepPage
	@ Description :
=============================================================*/
void CKL_CryptoMainDlg::InitStepPage()
{
	CRect rc;
	GetDlgItem(IDC_STATIC_PROPERTY_PAGE)->GetWindowRect(rc);
	ScreenToClient(rc);

	// ������ ����
	m_Step1Desc.Create(CKL_CryptoDescPage::IDD, this);
	m_Step2Conn.Create(CKL_CryptoConnPage::IDD, this);	
	m_Step4Exec.Create(CKL_CryptoExecPage::IDD, this);

	// ��ġ ����
	m_Step1Desc.MoveWindow(rc);
	m_Step2Conn.MoveWindow(rc);	
	m_Step4Exec.MoveWindow(rc);

	ShowStepPage((STEP_PAGE_TYPE)m_nStepPage);
}

/*=============================================================
	@ Function    :	ShowStepPage
	@ Description :
=============================================================*/
void CKL_CryptoMainDlg::ShowStepPage(STEP_PAGE_TYPE enStepPage)
{
	m_Step1Desc.ShowWindow(FALSE);
	m_Step2Conn.ShowWindow(FALSE);
	m_Step4Exec.ShowWindow(FALSE);

	m_btnPrev.EnableWindow(TRUE);
	m_btnNext.EnableWindow(TRUE);

	switch (enStepPage)
	{
	case STEP_PAGE_DESC:
		m_Step1Desc.ShowWindow(TRUE);
		m_btnPrev.EnableWindow(FALSE);	// �� �� ��������� ���� ��ư ���Ұ�
		break;
	case STEP_PAGE_CONN:
		m_Step2Conn.ShowWindow(TRUE);
		break;
	case STEP_PAGE_EXEC:
		m_Step4Exec.ShowWindow(TRUE);
		m_btnNext.EnableWindow(FALSE);	// �� �� ��������� ���� ��ư ���Ұ�
		break;
	}
}
