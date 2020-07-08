// SelectMailServer.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "SelectMailServer.h"
#include "RegKey.h"
#include "SMTPLOGDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectMailServer dialog

CSelectMailServer::CSelectMailServer(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectMailServer::IDD, pParent)
{
	m_nListCtrlIndex = 0;
	m_nMailCount     = 0;
	m_pMail          = NULL;
	
	//{{AFX_DATA_INIT(CSelectMailServer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSelectMailServer::~CSelectMailServer()
{
	delete[] m_pMail;
	m_Dlg_brush.DeleteObject();
}

VOID CSelectMailServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectMailServer)
	DDX_Control(pDX, IDCANCEL, m_ctrlClose);
	DDX_Control(pDX, IDOK, m_ctrlOK);
	DDX_Control(pDX, IDC_MAIL_ACCOUNT_LIST, m_ctrlMailAccount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectMailServer, CDialog)
	//{{AFX_MSG_MAP(CSelectMailServer)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MAIL_ACCOUNT_LIST, OnItemchangedMailAccountList)	
	ON_BN_CLICKED(IDC_bSmrtLogView, OnbSmrtLogView)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectMailServer message handlers

BOOL CSelectMailServer::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// ListCtrl �ʱ�ȭ
	m_ctrlMailAccount.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
	
	//LoadImageList();
	InsertColumn ();

	// Button Bitmap Load
	m_ctrlOK.LoadBitmaps(IDB_OK_N, IDB_OK_S, IDB_OK_N, IDB_OK_D);
	m_ctrlClose.LoadBitmaps(IDB_CLOSE_N, IDB_CLOSE_S, IDB_CLOSE_N, IDB_CLOSE_D);

	// ������Ʈ���� �ִ� ���������� �߰��Ѵ�
	//AddMailServerInfoFromRegistry();
	GetMailServerInfo();
	
	if (m_nMailCount == 0)
	{
		MessageBox(_T("���ϼ��� ������ �����ϴ�.\n\n���ϼ��� ������ ����Ŭ���̾�Ʈ ���α׷����� �����Ͻ� �� �ֽ��ϴ�."), _T("���"), MB_ICONEXCLAMATION);
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// Display
	Display();

	m_ctrlMailAccount.SetItemState(m_nListCtrlIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239)); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSelectMailServer::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

VOID CSelectMailServer::GetMailServerInfo()
{
	CRegKeyMgr RegKey;

	CString sInternetAccountManagerPath = _T(".DEFAULT\\SOFTWARE\\Microsoft\\Internet Account Manager");
	
	CString sDefalutMailAccount;

	CString sSMTPServer;
	CString sSMTPEmailAddress;
	CString sSMTPDisplayName;

	HKEY    hKeyRoot = HKEY_USERS;

	if (ERROR_SUCCESS != RegKey.Open(hKeyRoot, sInternetAccountManagerPath))		return;
	if (ERROR_SUCCESS != RegKey.Read(_T("Default Mail Account"), sDefalutMailAccount))	return;
	
	RegKey.Close();

	CString sAccount;
	CString sAccountPath;
	CString sEmailAddress;

	for (INT i=1 ; i<10 ; i++)
	{
		if (10>i)			sAccount.Format(_T("0000000%d"), i);
		else if (100 > i)	sAccount.Format(_T("000000%d"),  i);
		else return;

		sAccountPath = sInternetAccountManagerPath + _T("\\Accounts\\") + sAccount;
		if (ERROR_SUCCESS != RegKey.Open(hKeyRoot,				sAccountPath))		continue;

		if (ERROR_SUCCESS != RegKey.Read(_T("SMTP Server"),			sSMTPServer))		return;
		if (ERROR_SUCCESS != RegKey.Read(_T("SMTP Email Address"),	sSMTPEmailAddress)) return;
		if (ERROR_SUCCESS != RegKey.Read(_T("SMTP Display Name"),	sSMTPDisplayName))	return;
		
		sEmailAddress.Format(_T("\"%s\" <%s>"), sSMTPDisplayName, sSMTPEmailAddress);
		AddMailServerRecord(sSMTPServer.GetBuffer(0), sEmailAddress.GetBuffer(0));
	}

}

/*
VOID CSelectMailServer::LoadImageList()
{
	//m_ImageList.Create(IDB_MAIL_SERVER, 20, 1, RGB(0, 128, 128));
	//m_ctrlMailAccount.SetImageList(&m_ImageList, LVSIL_SMALL);
}
*/


VOID CSelectMailServer::InsertColumn()
{
	CRect rect;

	m_ctrlMailAccount.GetClientRect(&rect);

	// Į���� ���� ������ �����ϴ� ����ü
	LV_COLUMN lvcolumn;

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	//lvcolumn.mask = LVCF_FMT | LVCF_TEXT ;

	// Į�� ����� �߾ӿ� ����ϵ��� ��
	lvcolumn.fmt = LVCFMT_LEFT;

	for (INT i = 0; i < 2; i++)
	{
		lvcolumn.iSubItem = i;
		
		switch (i)
		{
		case 0 :
			lvcolumn.cx = 120;
			lvcolumn.pszText = _T("���ϼ���");
			break;
		case 1 :
			lvcolumn.cx = 220;
			lvcolumn.pszText = _T("�����ּ�");
			break;
		}
		m_ctrlMailAccount.InsertColumn(i, &lvcolumn);
	}

}

VOID CSelectMailServer::Display()
{
	// �ʱ�ȭ
	m_ctrlMailAccount.DeleteAllItems();

	LV_ITEM lvitem;

	lvitem.mask      = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvitem.mask      = LVIF_TEXT | LVIF_STATE;
	lvitem.iSubItem  = 0;
	lvitem.iImage    = 0;
	lvitem.stateMask = LVIS_STATEIMAGEMASK;
	lvitem.state     = INDEXTOSTATEIMAGEMASK(1);

	for (INT i = 0; i < m_nMailCount; i++)
	{
		lvitem.iItem   = m_ctrlMailAccount.GetItemCount();
		lvitem.pszText = (LPTSTR)(LPCTSTR) m_pMail[i].sMailServer;

		// ���ο� �� ���
		m_ctrlMailAccount.InsertItem(&lvitem);

		// �� Į���� �ؽ�Ʈ ����
		m_ctrlMailAccount.SetItemText(lvitem.iItem, 1, m_pMail[i].sMailAddress);
	}
}

VOID CSelectMailServer::GetMailAccount(CString& sMailServer, CString& sMailAddress)
{
	sMailServer  = m_pMail[m_nListCtrlIndex].sMailServer;
	sMailAddress = m_pMail[m_nListCtrlIndex].sMailAddress;
}

VOID CSelectMailServer::OnItemchangedMailAccountList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_nListCtrlIndex = pNMListView->iItem;

	*pResult = 0;
}

INT  CSelectMailServer::AddMailServerRecord(CString  sMailServer, CString  sMailAddress)
{
	// �ߺ� Ȯ��
	for (INT i = 0; i < m_nMailCount; i++)
	{
		if (m_pMail[i].sMailServer.Compare(sMailServer) == 0 && m_pMail[i].sMailAddress.Compare(sMailAddress) == 0)
		{
			return 100;
		}
	}

	Mail* pTemp = new Mail[m_nMailCount + 1];
	if (pTemp == NULL)
	{
		return -1;
	}

	for (i = 0; i < m_nMailCount; i++)
	{
		pTemp[i].sMailServer  = m_pMail[i].sMailServer;
		pTemp[i].sMailAddress = m_pMail[i].sMailAddress;
	}

	delete [] m_pMail;
	m_pMail = NULL;

	m_pMail = pTemp;

	m_pMail[m_nMailCount].sMailServer.Format(_T("%s"), sMailServer);
	m_pMail[m_nMailCount].sMailAddress.Format(_T("%s"), sMailAddress);
	
	m_nMailCount++;

	return 0;
}

VOID CSelectMailServer::AddMailServerInfoFromRegistry()
{
	// ������Ʈ���� �ִ� ���ϼ��� ������ ����Ʈ ��Ʈ�ѿ� �Է��Ѵ�
	CRegKeyMgr RegKey;
	
	CString sInternetAccountManagerPath = _T(".DEFAULT\\SOFTWARE\\Microsoft\\Internet Account Manager");
	CString sDefalutMailAccount;
	CString sDefaultMailAccountPath;
	CString sSMTPServer;
	CString sSMTPEmailAddress;
	CString sSMTPDisplayName;
	
	HKEY    hKeyRoot = HKEY_USERS;
	
	if (RegKey.Open(hKeyRoot, sInternetAccountManagerPath) == ERROR_SUCCESS) 
	{
		if (RegKey.Read(_T("Default Mail Account"), sDefalutMailAccount) == ERROR_SUCCESS) 
		{
			RegKey.Close();
			
			sDefaultMailAccountPath = sInternetAccountManagerPath;
			sDefaultMailAccountPath += _T("\\Accounts\\");
			sDefaultMailAccountPath += sDefalutMailAccount;
			
			if (RegKey.Open(hKeyRoot, sDefaultMailAccountPath) == ERROR_SUCCESS)
			{
				if (RegKey.Read(_T("SMTP Server"), sSMTPServer) == ERROR_SUCCESS)
				{
					if (RegKey.Read(_T("SMTP Email Address"), sSMTPEmailAddress) == ERROR_SUCCESS)
					{
						if (RegKey.Read(_T("SMTP Display Name"), sSMTPDisplayName) == ERROR_SUCCESS)
						{
							CString sEmailAddress;
							
							sEmailAddress.Format(_T("\"%s\" <%s>"), sSMTPDisplayName, sSMTPEmailAddress);
							
							AddMailServerRecord(sSMTPServer.GetBuffer(0), sEmailAddress.GetBuffer(0));
						}
					}
				}
			}
		}
	}
}

VOID CSelectMailServer::OnbSmrtLogView() 
{
	CSMTPLOGDLG	dlg;
	dlg.DoModal();
}

HBRUSH CSelectMailServer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = NULL;
	if(pWnd->m_hWnd == this->m_hWnd) hbr = HBRUSH(m_Dlg_brush);
	else hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(RGB(239, 239, 239));

		return m_Dlg_brush;
	}

	return hbr;
}
