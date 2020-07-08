// RPTUpdaterClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RPTUpdaterClient.h"
#include "RPTUpdaterClientDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual VOID DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

VOID CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRPTUpdaterClientDlg dialog

CRPTUpdaterClientDlg::CRPTUpdaterClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRPTUpdaterClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRPTUpdaterClientDlg)
	m_strID = _T("");
	m_strPass = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	//Thread
	m_pThreadMain = NULL;
	m_bIsThreadRunning = FALSE;
	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));

	// KOL.UDF.022 ��ť���ڵ� ����
	m_strLogMsg = _T("");
	m_nSuccessCnt = 0;
}

VOID CRPTUpdaterClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRPTUpdaterClientDlg)
	DDX_Control(pDX, IDC_prgMain, m_ctrlProgress);
	DDX_Text(pDX, IDC_edtID, m_strID);
	DDX_Text(pDX, IDC_edtPASS, m_strPass);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRPTUpdaterClientDlg, CDialog)
	//{{AFX_MSG_MAP(CRPTUpdaterClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnSTART, OnbtnSTART)
	ON_MESSAGE(THREAD_END,OnThreadEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRPTUpdaterClientDlg message handlers

BOOL CRPTUpdaterClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add _T("About...") menu item to _tsystem menu.

	// IDM_ABOUTBOX must be in the _tsystem command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	GetDlgItem(IDC_btnSTART)->ShowWindow(FALSE);
	GetDlgItem(IDC_stcProgress)->SetWindowText(_T("�� �α����� �ֽʽÿ�!"));
	

	if(m_Api.SetConfig()<0)
	{
		EndDialog(0);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CRPTUpdaterClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CRPTUpdaterClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		INT cxIcon = GetSystemMetrics(SM_CXICON);
		INT cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		INT x = (rect.Width() - cxIcon + 1) / 2;
		INT y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The _tsystem calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRPTUpdaterClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


VOID CRPTUpdaterClientDlg::OnbtnSTART() 
{
	// TODO: Add your control notification handler code here
	
	//���� procedure����(Thread)
	m_nSuccessCnt = 0;
	m_strLogMsg = _T("");

	m_pThreadMain = AfxBeginThread(ThreadFunc_MainProc, this);

}

INT CRPTUpdaterClientDlg::ShowPatchList()
{
	
	INT nSize = m_Api.m_arrayRptAlias.GetSize();
	CString strList;
	CString strAlias;
	CString msg;

	for(INT i=0;i<nSize;i++)
	{
		strAlias = m_Api.m_arrayRptAlias.GetAt(i);
		strList += strAlias;
		strList += _T("\r\n");
	}
	
	CString strLibraryName = m_Api.m_strLibraryName;
	CString strUSer = m_Api.m_strLogInName;

	msg.Format(_T("�ȳ��ϼ��� [%s]�� [%s]��!\n������ ���������� ������ ���� �� [ %d ]���� ��ġ��� ��¹��� �ֽ��ϴ�.\n")
			   _T("��ġ�� �����ϽǷ��� [��ġ����]��ư�� �����ּ���!"),strLibraryName,strUSer,nSize); 
	GetDlgItem(IDC_edtLog)->SetWindowText(strList);
	GetDlgItem(IDC_stcProgress)->SetWindowText(msg);
	GetDlgItem(IDC_btnSTART)->ShowWindow(TRUE);

	return 0;
}

VOID CRPTUpdaterClientDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//�α��� �Լ�
	UpdateData(TRUE);

	if(m_strID.IsEmpty()) {
		AfxMessageBox(_T("���̵� �Է��� �ֽʽÿ�!"));
		return;
	}

	if(m_strPass.IsEmpty()) {
		AfxMessageBox(_T("���̵� �Է��� �ֽʽÿ�!"));
		return;
	}
	
	//������ ��ȣ�� �����Ѵ�
	if(m_Api.SetLibraryCode(m_strID,m_strPass)<0)
	{
		return;
	}


	//�ش� �������� ��ġ ������ �����ش�
	if(m_Api.InitApi()<0)
	{
		EndDialog(0);	
	}
	
	ShowPatchList();
	
	
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	CEdit* pEdit = NULL;
	pEdit = (CEdit*)GetDlgItem(IDC_edtID);
	if(pEdit)
	{
		pEdit->SetReadOnly();
	}
	
	pEdit = (CEdit*)GetDlgItem(IDC_edtPASS);
	if(pEdit)
	{
		pEdit->SetReadOnly();
	}

	return;
	CDialog::OnOK();
}

VOID CRPTUpdaterClientDlg::OnThreadEnd(WPARAM w,LPARAM l)
{
	//Thread���� 
	INT nSize = m_Api.m_arrayRptAlias.GetSize();
	CString msg;

	if(m_nSuccessCnt == nSize)
	{
		msg.Format(_T("�� [%d] ���� ��ġ��� [%d] �� ��� �����Ͽ����ϴ�!")
				   _T("\n����� Ȯ���Ͻð� ���α׷��� �ݾ��ּ���!"),nSize,m_nSuccessCnt);
	}
	else
	{
		msg.Format(_T("�� [%d] ���� ��ġ��� [%d] �Ǹ� �����Ͽ����ϴ�!")
				   _T("\n����� Ȯ���Ͻð� ��õ� �ϽǷ��ŵ�\n [��õ�] ��ư�� �����ּ���!"),nSize,m_nSuccessCnt);

		GetDlgItem(IDC_btnSTART)->SetWindowText(_T("��õ�"));

	}

	GetDlgItem(IDC_stcProgress)->SetWindowText(msg);
	GetDlgItem(IDC_edtLog)->SetWindowText(m_strLogMsg);

}

UINT ThreadFunc_MainProc(LPVOID pParam)
{
	
	CRPTUpdaterClientDlg* pDlg = (CRPTUpdaterClientDlg*)pParam;
	// KOL.UDF.022 ��ť���ڵ� ����
	//if (pDlg == NULL)
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	CString strQuery;
	CString strAlias;
	CString strPK;
	CString strNotify;
	CString strMsg;
	INT nSize;
	INT ids = 0;
	
	nSize = pDlg->m_Api.m_arrayRptAlias.GetSize();
	pDlg->m_ctrlProgress.SetRange(0,nSize);
	pDlg->m_ctrlProgress.SetPos(0);


//GetOneValueQuery
// -4004 : ���ӵ�������.
// -4005 : ��������� �����߻�. 
// -4006 : ����� �ʹ� ����.
// -4007 : �˻� ��� ����
// -4008 : �˻� �ʵ尡 ��� �ʵ庸�� ����
// -4999 : ���� Oracle�� �������� ����


	for(INT i=0;i<nSize;i++)
	{
		//INSERT ���� UPDATE���� ���� �Ǵ�(���ÿ� ���� ���� �˻�)
		strAlias = pDlg->m_Api.m_arrayRptAlias.GetAt(i);
		strNotify.Format(_T("�� [%s] �ڷ� �ٿ�ε���..."),strAlias);
		pDlg->GetDlgItem(IDC_stcProgress)->SetWindowText(strNotify);
		

		//���ϴٿ�ε� 
		if(pDlg->m_Api.DownLoadRptFile(strAlias))
		{
			strMsg.Format(_T("�� [%s] �ڷ�ó���� ���� �ٿ�ε� ����\r\n"),strAlias);
			pDlg->m_strLogMsg += strMsg;
				
			goto END;

		}

		strNotify.Format(_T("�� [%s] �ڷ� DB üũ��..."),strAlias);
		pDlg->GetDlgItem(IDC_stcProgress)->SetWindowText(strNotify);


		strQuery.Format(_T("SELECT PK FROM ESL_SIREPORT_TBL WHERE RPTALIAS='%s' AND FLAG != 'U'"),strAlias);
		ids = pDlg->m_Api.m_DM.GetOneValueQuery(strQuery,strPK);
		
		
		if(ids<0)
		{
			
			if(-4004 == ids)
			{
				strMsg.Format(_T("�� [%s] �ڷ�ó���� DB ���ӽ���\r\n"),strAlias);
				pDlg->m_strLogMsg += strMsg;
				
				goto END;

			}
			else if(-4005 == ids || -4006 == ids)
			{
			
				strMsg.Format(_T("�� [%s] �ڷ�ó���� SELECT�� ����\r\n"),strAlias);	
				pDlg->m_strLogMsg += strMsg;
				
				goto END;
			}
		}
	
		strPK.TrimLeft();
		strPK.TrimRight();

		strNotify.Format(_T("�� [%s] �ڷ� DB �Է���..."),strAlias);
		pDlg->GetDlgItem(IDC_stcProgress)->SetWindowText(strNotify);
		
		if(strPK.IsEmpty()) //INSERT
		{
			if(pDlg->m_Api.InsertProcedure(strAlias)<0) 
			{
				strMsg.Format(_T("�� [%s] �ڷ�ó���� INSERT�� ����\r\n"),strAlias);	
				pDlg->m_strLogMsg += strMsg;

				goto END;
			}
			
		}
		else //UPDATE 
		{
			if(pDlg->m_Api.UpdateProcedure(strAlias,strPK)<0) 
			{
				strMsg.Format(_T("�� [%s] �ڷ�ó���� UPDATE�� ����\r\n"),strAlias);	
				pDlg->m_strLogMsg += strMsg;
				goto END;
	
			}
		}
		
		strNotify.Format(_T("�� [%s] �ڷ� ������ �α׸� �������..."),strAlias);
		pDlg->GetDlgItem(IDC_stcProgress)->SetWindowText(strNotify);


		if(pDlg->m_Api.LogServer(strAlias)<0) 
		{
			strMsg.Format(_T("�� [%s] �ڷ�ó���� ������ �α� ����\r\n"),strAlias);	
			pDlg->m_strLogMsg += strMsg;
			goto END;
	
		}

		strMsg.Format(_T("[%s] �ڷ� ��ġ ����\r\n"),strAlias);	
		pDlg->m_strLogMsg += strMsg;

		pDlg->m_nSuccessCnt++;
		pDlg->m_ctrlProgress.SetPos(i+1);
	}
	

	goto END;
	

END:
	pDlg->m_bIsThreadRunning = FALSE;
	::SendMessage(pDlg->GetSafeHwnd(),THREAD_END,NULL,NULL);
	return 0;
	
}
