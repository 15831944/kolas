// MailMgrTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MailMgrTest.h"
#include "MailMgrTestDlg.h"
#include "Windows.h"
//#include <imm.h>        

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
// CMailMgrTestDlg dialog

CMailMgrTestDlg::CMailMgrTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMailMgrTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailMgrTestDlg)
	m_ctvRecipientName = _T("");
	m_ctvRecipientEmail = _T("");
	m_ctvBackground = FALSE;
	m_ctvCleareLog = FALSE;
	m_ctvConfigFilepath = CFG_FILE_PATH;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->m_bLogOutput = FALSE;
}

VOID CMailMgrTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailMgrTestDlg)
	DDX_Control(pDX, IDC_chkDELETE_LOG_FILE, m_ctrlDeleteLogFile);
	DDX_Control(pDX, IDC_chkCLEAR, m_ctrlClear);
	DDX_Control(pDX, IDC_chkBACKGROUND, m_ctrlBackground);
	DDX_Control(pDX, IDC_eRECIPIENT_EMAIL, m_ctrlRecipientEmail);
	DDX_Control(pDX, IDC_eLOG, m_ctrlLog);
	DDX_Control(pDX, IDC_eRECIPIENT_NAME, m_ctrlRecipientName);
	DDX_Control(pDX, IDC_rSEND, m_ctrlRadioSend);
	DDX_Control(pDX, IDC_rRESULT, m_ctrlRadioResult);
	DDX_Text(pDX, IDC_eRECIPIENT_NAME, m_ctvRecipientName);
	DDX_Text(pDX, IDC_eRECIPIENT_EMAIL, m_ctvRecipientEmail);
	DDX_Check(pDX, IDC_chkBACKGROUND, m_ctvBackground);
	DDX_Check(pDX, IDC_chkCLEAR, m_ctvCleareLog);
	DDX_Text(pDX, IDC_eCFG_FILE_PATH, m_ctvConfigFilepath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMailMgrTestDlg, CDialog)
	//{{AFX_MSG_MAP(CMailMgrTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_rSEND, OnrSEND)
	ON_BN_CLICKED(IDC_rRESULT, OnrRESULT)
	ON_EN_CHANGE(IDC_eRECIPIENT_NAME, OnChangeeRECIPIENTNAME)
	ON_EN_CHANGE(IDC_eRECIPIENT_EMAIL, OnChangeeRECIPIENTEMAIL)
	ON_BN_CLICKED(IDC_chkBACKGROUND, OnchkBACKGROUND)
	ON_BN_CLICKED(IDC_btnMAIL_SEND, OnbtnMAILSEND)
	ON_EN_CHANGE(IDC_eLOG, OnChangeeLOG)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_chkCLEAR, OnchkCLEAR)
	ON_BN_CLICKED(IDC_btnMAIL_SERVER, OnbtnMAILSERVER)
	ON_BN_CLICKED(ID_btnClose, OnbtnClose)
	ON_BN_CLICKED(IDC_chkDELETE_LOG_FILE, OnchkDELETELOGFILE)
	ON_EN_KILLFOCUS(IDC_eRECIPIENT_NAME, OnKillfocuseRecipientNameOrEmail)
	ON_EN_SETFOCUS(IDC_eRECIPIENT_EMAIL, OnSetfocuseRECIPIENTEMAIL)
	ON_BN_CLICKED(IDC_btnMAIL_SERVER_VIEW, OnbtnMAILSERVERVIEW)
	ON_EN_KILLFOCUS(IDC_eRECIPIENT_EMAIL, OnKillfocuseRecipientNameOrEmail)
	ON_BN_CLICKED(IDC_chkVIEW_LOG_FILE, OnchkVIEWLOGFILE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailMgrTestDlg message handlers

BOOL CMailMgrTestDlg::OnInitDialog()
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

	this->m_ctrlClear.SetCheck(1);
	OnrSEND();
	m_ctrlRadioSend.SetFocus();
	m_ctrlRecipientName.SetFocus();
	
    long	ltime;
    tm		*t;
    time(&ltime);
    t = localtime (&ltime);
	m_sLogFilepath.Format(_T("..\\tunning\\(%04d%02d%02d)SmtpLog.txt"), 1900+t->tm_year, t->tm_mon+1, t->tm_mday);
	
	((CWnd*)(GetDlgItem(IDC_btnMAIL_SEND)))->EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CMailMgrTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CMailMgrTestDlg::OnPaint() 
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
HCURSOR CMailMgrTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CMailMgrTestDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	OnrSEND();
	m_ctrlRadioSend.SetFocus();
	m_ctrlRecipientName.SetFocus();
}

VOID CMailMgrTestDlg::OnrSEND() 
{
	// TODO: Add your control notification handler code here
	m_ctrlRadioSend.SetCheck(1);
	m_ctrlRadioResult.SetCheck(0);
	m_ctrlRecipientName.SetFocus();
}

VOID CMailMgrTestDlg::OnrRESULT() 
{
	// TODO: Add your control notification handler code here
	m_ctrlRadioSend.SetCheck(0);
	m_ctrlRadioResult.SetCheck(1);
	//m_ctrlLog.SetWindowText(_T(""));
	m_ctrlLog.SetFocus();
}

VOID CMailMgrTestDlg::OnChangeeRECIPIENTNAME() 
{
	OnrSEND();	
	m_ctrlRecipientName.SetFocus();
	OnKillfocuseRecipientNameOrEmail();
}

VOID CMailMgrTestDlg::OnChangeeRECIPIENTEMAIL() 
{
	OnrSEND();	
	//OnSetfocuseRECIPIENTEMAIL();
	OnKillfocuseRecipientNameOrEmail();
	m_ctrlRecipientEmail.SetFocus();
}

VOID CMailMgrTestDlg::OnchkBACKGROUND() 
{
	OnrSEND();	
	m_ctrlBackground.SetFocus();
}

VOID CMailMgrTestDlg::OnchkCLEAR() 
{
	OnrRESULT();
	this->m_ctrlLog.SetFocus();
}

VOID CMailMgrTestDlg::OnChangeeLOG() 
{
	OnrRESULT();
	this->m_ctrlLog.SetFocus();
}


VOID CMailMgrTestDlg::OnbtnMAILSEND() 
{
	OnrSEND();	

	UpdateData(TRUE);
	
	INT rc;
	if (m_sTestCfgFile.IsEmpty())
	{
		rc = MakeTestMailAccountCfgFile(m_ctvConfigFilepath, FALSE);
		if (0>rc) 
		{
			AfxMessageBox(_T("Mail ���� �׽�Ʈ�� ���� ȯ�� ������ �������� ���߽��ϴ�."));
			return;
		}
		ASSERT(!m_sTestCfgFile.IsEmpty());
	}
	
	if (this->m_ctvCleareLog) OnchkDELETELOGFILE();
	this->m_ctrlLog.SetReadOnly();

	CString sTemp;

	
	//--------------------------------------------------------------------------------------
	///Mail Manager ��ü ����
	//--------------------------------------------------------------------------------------
	CESL_Mgr *pESLMgr;

	pESLMgr = new CESL_Mgr(0,NULL);
	CSendMailMgr MailMgr(pESLMgr);

	

	//--------------------------------------------------------------------------------------
	///������ ��� ���ϼ���/���� ������ Config file���� �������� ��� config file ��� ����
	///MailMgr default ������ = _T("..\\cfg\\MailAccount.cfg")
	///���� default �������� �ٸ� ���� �ݵ�� �����ؾ� �Ѵ�.
	//--------------------------------------------------------------------------------------

	//MailMgr.SetCfgFilepath(_T("..\\cfg\\MailAccount.cfg"));
	//MailMgr.SetCfgFilepath(m_ctvConfigFilepath);
	MailMgr.SetCfgFilepath(this->m_sTestCfgFile);

	
	//--------------------------------------------------------------------------------------
	///���� ���� �α� ���� ��� ����
	///MailMgr default ������ = _T("..\\tunning\\SmtpLog.txt")
	///���� default �������� �ٸ� ���� �ݵ�� �����ؾ� �Ѵ�.
	///�α����ϸ��� ������ ���ϸ�(SmtpLog.txt) �տ� ��¥�� �ٿ�(YYYYMMDD) �����Ѵ�.
	///��, ..\\tunning\\(20030227)SmtpLog.txt �̷� ������
	//--------------------------------------------------------------------------------------

	//MailMgr.SetLogFilepath(_T("..\\tunning\\SmtpLog.txt"));


	

	//--------------------------------------------------------------------------------------
	///Mail Record �߰� -> MailMgr.AddMailRecord() �� ��� -> AddTwoMailRecord �Լ� ����
	//--------------------------------------------------------------------------------------
	rc = AddTwoMailRecord(&MailMgr);
	if (0>rc) 
	{
		sTemp.Format(_T("AddTwoMailRecord(MailMgr) Error [%d] !!!"), rc);
		AfxMessageBox(sTemp);
		return;
	}


	//--------------------------------------------------------------------------------------
	///MailMgr Dialog�� ����� ���� ������ �ϴ� ���
	//--------------------------------------------------------------------------------------
	if (!m_ctvBackground)
	{
		//-------------------------------------------
		///���� ����/���� ���� �����ϰ� Dialog�� ���
		//-------------------------------------------

		///���� ���ϼ����� ������ ����� �����ϴ� ���
		//MailMgr.DoModal(MAIL_SERVER_USER, _T("mail.eco.co.kr"), _T("\"����������\"<foo@bar.baz>"));
		
		///Registry Mail Account ������ ����ϴ� ���
		//MailMgr.DoModal(MAIL_SERVER_REGISTRY);

		///Config file���� ���ϼ����� ������ ����� �����ϴ� ���
		MailMgr.DoModal(MAIL_SERVER_CFGFILE);
	}

	
	//--------------------------------------------------------------------------------------
	///MailMgr Dialog�� ����� �ʰ� Background�� ���� ������ �ϴ� ���
	//--------------------------------------------------------------------------------------
	else
	{
		//-------------------------------------------
		///���� ����/���� ���� ����
		//-------------------------------------------
		
		///���� ���ϼ����� ������ ����� �����ϴ� ���
		//MailMgr.SetDefaultMailServer(_T("mail.eco.co.kr"), _T("\"����������\"<foo@bar.baz>"));
		
		///Registry Mail Account ������ ����ϴ� ���
		//MailMgr.SetMailServerInfoFromRegistry(TRUE);
		
		///Config file���� ���ϼ����� ������ ����� �����ϴ� ���
		rc = MailMgr.SetMailServerInfoFromCfgFile(TRUE);	// <<< �ݵ�� TRUE�� �Ű������� �Ѱܾ� �Ѵ�.
		if (0>rc)
		{
			AfxMessageBox(_T("���ϼ����� ������ �� �����ϴ�."));
			return;
		}
		
		//-------------------------------------------------------------------
		///Background ���� ���� ���� ����
		//-------------------------------------------------------------------
		rc = MailMgr.SendMail(FALSE, TRUE);	// <<< �ݵ�� �ι�° �Ű�����(Background����)�� TRUE�� �Ѱܾ� �Ѵ�.
		if (0>rc)
		{
			AfxMessageBox(_T("�������� �� ���ο����� �߻��Ͽ����ϴ�."));
			return;
		}
	}

	///���� ������ log file path ��������
	this->m_sLogFilepath = MailMgr.GetLogFilepath();
	ShowMailLog(this->m_sLogFilepath);

	//-------------------------------------------------------------------
	///��� Ȯ�� - ���ۼ���/������ ���� ���� Ȯ��
	//-------------------------------------------------------------------
	if (m_ctvBackground)
	{
		sTemp.Format(_T("���������� �Ϸ�Ǿ����ϴ�. [����:%d / ����:%d]"), 
						MailMgr.GetCountMailSended(),		///���ۼ����� ���� ����
						MailMgr.GetCountMailFailed()		///���۽����� ���� ����
			);
		MessageBox(sTemp, _T("Ȯ��"), MB_ICONINFORMATION);
	}
	

	//--------------------------------------------------------------------------------------
	///��� Ȯ�� - �� ���� ���ڵ庰�� Ȯ��
	///  - Return	-9 : ���� ���ڵ� �ε��� ���� (zero based)
	///				-1 : ���۽���
	///				 0 : ���ۼ���
	///				 1 : ���۾���
	//--------------------------------------------------------------------------------------
	ShowMailRecordState( MailMgr.GetMailRecordSendState(0),	///ù��° ���� ���ڵ� ���۰�� Ȯ��
	  				     MailMgr.GetMailRecordSendState(1)	///�ι�° ���� ���ڵ� ���۰�� Ȯ��
						 );

}


/************************************************************************/
/* Mail record �߰�														*/
/************************************************************************/
INT CMailMgrTestDlg::AddTwoMailRecord(CSendMailMgr* pMailMgr)
{
	INT rc;



	//--------------------------------------------------------------------------------------
	///Mail Record ��ü ����
	//--------------------------------------------------------------------------------------
	SendMailRecord MailRecord;
	

	
	//--------------------------------------------------------------------------------------
	///Mail Record ��ü ����
	//--------------------------------------------------------------------------------------
	
	///÷������ ���� ����, ������ 0
	MailRecord.m_nFileAttachCnt = 2;
	///÷������ path ����. �������� ��� ','(�޸�)�� �и��Ѵ�.
	//MailRecord.m_pFileAttach = _T("Loader.exe,Mailȯ�漳�����׽�Ʈ.exe");
	MailRecord.m_pFileAttach = _T("..\\Cfg\\MailAccount.cfg,..\\Cfg\\MailAccount.4test.cfg");
	///���� Body - Encoding�� 'text/html' �� ��� HTML ��������
	///          - Encoding�� 'text/html' �� �ƴ� ��� TEXT �������� 
	MailRecord.m_sBody = _T("<head><title>ù��°</title></head><body>ù��° ����</body>");
	///���� Encoding ����
	MailRecord.m_sEncoding = _T("text/html");
	///�޴� ��� �̸� - '�޴»�� �̸�'
	MailRecord.m_sRecipientName	   = this->m_ctvRecipientName;	
	///�޴� ��� ���ڿ��� - 'foo@bar.baz'
	MailRecord.m_sRecipientAddress = this->m_ctvRecipientEmail;	
	///���� ����(Subject)
	MailRecord.m_sSubject = _T("�������� - ù��°");
	///���������� �̸� - '���������� �̸�'
	MailRecord.m_sCCName    = _T("");
	///���������� ���ڿ��� - 'foooo@baaar.baaaz'
	MailRecord.m_sCCAddress = _T("");

	//--------------------------------------------------------------------------------------
	///Mail Record ��ü �߰� - SendMailRecord ��ü�� �����Ͽ� �߰�
	//--------------------------------------------------------------------------------------
	rc = pMailMgr->AddMailRecord(&MailRecord);
	if (0>rc) return -1;
	

	//--------------------------------------------------------------------------------------
	///Mail Record �߰� - ���Ϸ��ڵ� ������ ���� �����Ͽ� �߰�
	//--------------------------------------------------------------------------------------
 	rc = pMailMgr->AddMailRecord(_T("�������� - �ι�°"),
 								 this->m_ctvRecipientName,
 								 this->m_ctvRecipientEmail,
 								 _T("�ι�° ����"),
 								 _T("text"),
 								 _T("..\\Cfg\\MailAccount.4test.cfg"),	//_T("Mailȯ�漳�����׽�Ʈ.exe"),
 								 1,
 								 _T(""),
 								 _T(""),
								 _T(""),
								 _T(""),
								 _T(""),
								 _T(""),
								 _T(""),
								 _T(""),
								 _T(""));
 	if (0>rc) return -2;

	return 0;
}

VOID CMailMgrTestDlg::ShowMailLog(const CString sFielpath)
{
	if (this->m_ctvCleareLog) this->m_ctrlLog.SetWindowText(_T(""));

	CStdioFile file;
	if (!file.Open(sFielpath, CFile::modeRead | CFile::typeBinary))
	{
		//AfxMessageBox(_T("Log ������ �����ϴ�."));
		return;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CString sLine;

	while (file.ReadString(sLine))
	{
		this->AppendText(sLine + _T("\r\n"));
	}
	file.Close();

	this->m_ctrlLog.SetReadOnly();
	this->m_bLogOutput = TRUE;
}

VOID CMailMgrTestDlg::ShowMailRecordState(INT nRecord1State, INT nRecord2State)
{
	AppendText(_T("\r\n\r\n"));
	AppendText(_T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n"));
	this->AppendText(_T("+ ù��° ���� : ") + GetMailRecordSendState(nRecord1State) + _T("\r\n"));
	this->AppendText(_T("+ �ι�° ���� : ") + GetMailRecordSendState(nRecord2State) + _T("\r\n"));
	AppendText(_T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n"));
}

CString CMailMgrTestDlg::GetMailRecordSendState(INT nState)
{
/** �� ���Ϸ��ڵ� ���ۿ���Ȯ��.
@param nRecordIdx ���� ���ڵ� �ε���(zero-based)
@return -9 : �߸��� ���Ϸ��ڵ� �ε���(zero-based)
@return -3 : MAIL_SEND_NOTCONNECT
             (���ϼ��� ���ӽ��� - ���� ���� ���ڵ�� ���̻� �������� ����)
@return -2 : MAIL_SEND_NONRECIP(���ϼ����� ���� - �������� ����)
@return -1 : MAIL_SEND_FAIL(���Ϸ��ڵ� ���۽��� - SmtpLog.txt������ Ȯ���Ͽ��� ��)
@return 0  : MAIL_SEND_SUCCESS(���Ϸ��ڵ� ���ۼ���)
@return 1  : MAIL_SEND_NOT(���Ϸ��ڵ� �������� ���� 
             - ���� ���ӽ��з� ���� ���� �߰ų�, ����ڰ� ���� ���ڵ� ���� üũ�� ������) 
*/

	CString sState;
	switch (nState) {
	case -9 :
		sState = _T("÷�ڿ���");
		break;
	case -3 :
		sState = _T("���ϼ������ӽ���");
		break;
	case -2:
		sState = _T("���� ������ ����");
		break;
	case -1:
		sState = _T("���۽���");
		break;
	case 0:
		sState = _T("���ۼ���");
		break;
	case 1:
		sState = _T("���۾���");
		break;
	default:
		sState.Format(_T("%d:���Ǿȵ�"), nState);
	}
	return sState;
}

VOID CMailMgrTestDlg::AppendText(LPCTSTR sLog)
{
	INT Length = this->m_ctrlLog.GetWindowTextLength();
	
	this->m_ctrlLog.SetSel(Length, Length);
	this->m_ctrlLog.ReplaceSel(sLog);
	this->m_ctrlLog.LineScroll( this->m_ctrlLog.GetLineCount() );
}

VOID CMailMgrTestDlg::OnbtnMAILSERVERVIEW() 
{
	UpdateData(TRUE);
	
	UpdateCtrlState(FALSE);
	this->m_ctrlRadioResult.SetWindowText(_T("���ϼ������� ����"));
	SetDlgItemText(IDC_btnMAIL_SERVER, _T("�����׽�Ʈ�� ����"));

	this->m_ctrlLog.GetWindowText(m_sLogBackup);
	this->m_ctrlLog.SetWindowText(_T(""));
	
	INT nLineCnt=0;
	BOOL bExistServerInfo=FALSE;
	BOOL bExistAccountInfo=FALSE;
	BOOL bExistSenderInfo=FALSE;
	CString sTemp;

	CStdioFile file;
	
	BOOL bOpen = file.Open(m_ctvConfigFilepath, CFile::modeRead | CFile::typeBinary);	//_T("..\\Cfg\\MailAccount.cfg")
	if (!bOpen) 
	{
		AfxMessageBox(_T("[���� ���� ��������] �� ������ ������ ã�� �� �����ϴ�."));
		return;
	}
	else 
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		CString sLine;
		
		while (file.ReadString(sLine))
		{
			this->AppendText(sLine + _T("\r\n"));
			nLineCnt++;
			sLine.TrimLeft();
			if (sLine.IsEmpty()) continue;
			if (_T(".") != sLine.Left(1))
			{
				INT idx;
				idx = sLine.Find(_T("="));
				if (0 <= idx)
				{
					sTemp = sLine.Left(idx);
					sTemp.TrimLeft();
					sTemp.TrimRight();
					if (sTemp == _T("���ϼ���")) 
					{
						sTemp = sLine.Mid(idx+1);
						sTemp.TrimLeft();
						sTemp.TrimRight();
						if (!sTemp.IsEmpty()) bExistServerInfo = TRUE;
					} 
					else if (sTemp == _T("���ϰ���")) 
					{
						sTemp = sLine.Mid(idx+1);
						sTemp.TrimLeft();
						sTemp.TrimRight();
						if (!sTemp.IsEmpty()) bExistAccountInfo = TRUE;
					}
					else if (sTemp == _T("�߼��ڸ�")) 
					{
						sTemp = sLine.Mid(idx+1);
						sTemp.TrimLeft();
						sTemp.TrimRight();
						if (!sTemp.IsEmpty()) bExistSenderInfo = TRUE;
					}
				}
				
			}
		}
		file.Close();
		this->m_ctrlLog.SetReadOnly();
	}
	
	if (0==nLineCnt)  
	{
		AfxMessageBox(_T("MailAccount.cfg ���Ͽ� ������ �����ϴ�."));
		return;
	}
	
	this->m_bLogOutput = FALSE;

	if (bOpen)
	{
		if (!bExistServerInfo || !bExistAccountInfo || !bExistSenderInfo)
		{
			sTemp = _T("[ ");
			if (!bExistServerInfo)  sTemp  += _T("���ϼ���");
			if (!bExistAccountInfo) 
				(sTemp == _T("[ ")) ? 
					sTemp += _T("���ϰ���")  :
					sTemp += _T("/���ϰ���") ;
			if (!bExistSenderInfo)  
				(sTemp == _T("[ ")) ? 
					sTemp += _T("�߼��ڸ�")  :
					sTemp += _T("/�߼��ڸ�") ;
			
			AfxMessageBox(_T("MailAccount.cfg ���Ͽ� ") + sTemp + _T(" ] ������ �����ϴ�."));
			return;
		}
	}

	//�ӽ� - �׽�Ʈ
	//MakeTestMailAccountCfgFile(m_ctvConfigFilepath);
}


VOID CMailMgrTestDlg::OnbtnMAILSERVER() 
{
	UpdateData(TRUE);

	this->m_ctrlLog.SetReadOnly(FALSE);
	
	CStdioFile file;

	INT rc = MakeTestMailAccountCfgFile(m_ctvConfigFilepath, FALSE);
	if (0>rc) 
	{
		AfxMessageBox(_T("Mail ���� �׽�Ʈ�� ���� ȯ�� ������ �������� ���߽��ϴ�."));
		return;
	}
	ASSERT(!m_sTestCfgFile.IsEmpty());
	
	CString sBtnName;
	GetDlgItemText(IDC_btnMAIL_SERVER, sBtnName);
	if (sBtnName == _T("�����׽�Ʈ�� ����")) 
	{
		UpdateCtrlState(FALSE);
		SetDlgItemText(IDC_btnMAIL_SERVER, _T("�׽�Ʈ�� ���� ����"));
		this->m_ctrlLog.GetWindowText(m_sLogBackup);
		this->m_ctrlLog.SetWindowText(_T(""));

		INT nLineCnt=0;
		BOOL bExistServerInfo=FALSE;
		BOOL bExistAccountInfo=FALSE;
		BOOL bExistSenderInfo=FALSE;
		CString sTemp;

		//BOOL bOpen = file.Open(m_ctvConfigFilepath, CFile::modeRead);	//_T("..\\Cfg\\MailAccount.cfg")
		BOOL bOpen = file.Open(this->m_sTestCfgFile, CFile::modeRead | CFile::typeBinary);	//_T("..\\Cfg\\MailAccount.cfg")
		if (bOpen)
		{
			TCHAR cUni;
			file.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				file.SeekToBegin();
			}
			
			CString sLine;
			
			while (file.ReadString(sLine))
			{
				this->AppendText(sLine + _T("\r\n"));
				nLineCnt++;
				sLine.TrimLeft();
				if (sLine.IsEmpty()) continue;
				if (_T(".") != sLine.Left(1))
				{
					//sTemp = sLine.Left(8);
					//if (sTemp == _T("���ϼ���")) bExistServerInfo = TRUE;
					//if (sTemp == _T("���ϰ���")) bExistAccountInfo = TRUE;
					//if (sTemp == _T("�߼��ڸ�")) bExistSenderInfo = TRUE;

					INT idx;
					idx = sLine.Find(_T("="));
					if (0 <= idx)
					{
						sTemp = sLine.Left(idx);
						sTemp.TrimLeft();
						sTemp.TrimRight();
						if (sTemp == _T("���ϼ���")) 
						{
							sTemp = sLine.Mid(idx+1);
							sTemp.TrimLeft();
							sTemp.TrimRight();
							if (!sTemp.IsEmpty()) bExistServerInfo = TRUE;
						} 
						else if (sTemp == _T("���ϰ���")) 
						{
							sTemp = sLine.Mid(idx+1);
							sTemp.TrimLeft();
							sTemp.TrimRight();
							if (!sTemp.IsEmpty()) bExistAccountInfo = TRUE;
						}
						else if (sTemp == _T("�߼��ڸ�")) 
						{
							sTemp = sLine.Mid(idx+1);
							sTemp.TrimLeft();
							sTemp.TrimRight();
							if (!sTemp.IsEmpty()) bExistSenderInfo = TRUE;
						}
					}
					
				}
			}
			file.Close();
		}

		if (!bOpen || 0==nLineCnt)
		{
			if (!bOpen)
			{
				AfxMessageBox(_T("MailAccount.cfg ������ �����ϴ�.\n\n")
							  _T("���Ϻ����� �׽�Ʈ�� ���� �������� ������ �Է��� ��\n\n")
							  _T("[�׽�Ʈ�� ���� ����] ��ư�� ���� �����Ͽ� �ֽʽÿ�."));
			}
			this->AppendText(_T(".\r\n"));
			this->AppendText(_T(".��������---------------------------\r\n"));
			this->AppendText(_T("....................................\r\n"));
			this->AppendText(_T(". ���ϼ��� = ���ϼ����ּ�\r\n"));
			this->AppendText(_T(". ���ϰ��� = foo@bar.baz\r\n"));
			this->AppendText(_T(". �߼��ڸ� = ���\r\n"));
			this->AppendText(_T("....................................\r\n"));
			this->AppendText(_T(".\r\n"));
			this->AppendText(_T("���ϼ��� = \r\n"));
			this->AppendText(_T("���ϰ��� = \r\n"));
			this->AppendText(_T("�߼��ڸ� = \r\n"));
		}
		if (bOpen)
		{
			this->m_ctrlLog.GetWindowText(sTemp);
			if (!bExistServerInfo || !bExistAccountInfo || !bExistSenderInfo)
				this->AppendText(_T("\r\n.======================================\r\n"));
			if (!bExistServerInfo)  
				this->AppendText(_T(".���ϼ��� ������ �����ϴ�.\r\n"));
			if (!bExistAccountInfo) 
				this->AppendText(_T(".���ϰ��� ������ �����ϴ�.\r\n"));
			if (!bExistSenderInfo)  
				this->AppendText(_T(".�߼��ڸ� ������ �����ϴ�.\r\n"));
		}
	}
	else if (sBtnName == _T("�׽�Ʈ�� ���� ����")) 
	{
		
		//if (!file.Open(m_ctvConfigFilepath, CFile::modeCreate | CFile::modeWrite))
		if (!file.Open(this->m_sTestCfgFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			AfxMessageBox(_T("���Ϻ����� �׽�Ʈ�� ���� Config ������ ������ �� �����ϴ�."));
			return;
		}
		
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		
		CString sText;
		this->m_ctrlLog.GetWindowText(sText);
		sText.Replace(_T("\r"), NULL);
		file.WriteString(sText);
		//AfxMessageBox(sText);
		file.Close();

		SetDlgItemText(IDC_btnMAIL_SERVER, _T("�����׽�Ʈ�� ����"));
		if (this->m_bLogOutput) this->m_ctrlLog.SetWindowText(this->m_sLogBackup);
		this->m_ctrlLog.SetWindowText(_T(""));
		UpdateCtrlState(TRUE);
		OnKillfocuseRecipientNameOrEmail();
	}
}


VOID CMailMgrTestDlg::OnbtnClose() 
{
	EndDialog(IDCANCEL);
}

VOID CMailMgrTestDlg::UpdateCtrlState(BOOL bEnable/*=TRUE*/)
{
	if (bEnable)
	{
		this->m_ctrlRadioResult.SetWindowText(_T("���� ���� ���"));
		OnrSEND();
	}
	else
	{
		this->m_ctrlRadioResult.SetWindowText(_T("���� ���� ����"));
		//((CWnd*)(GetDlgItem(IDC_btnMAIL_SEND)))->EnableWindow(FALSE);
		this->OnKillfocuseRecipientNameOrEmail();
		OnrRESULT();
	}
}

INT CMailMgrTestDlg::MakeTestMailAccountCfgFile(CString sMailAccountFile, BOOL bReCreate/*=TRUE*/) 
{
	ASSERT(!sMailAccountFile.IsEmpty());

	CString sTestCfgFile;
	sTestCfgFile = sMailAccountFile;	// + _T(".4test.cfg");
	sTestCfgFile.Replace(_T(".cfg"), _T(".4test.cfg"));

	CFileStatus status;
	if ( !CFile::GetStatus( sTestCfgFile, status ) || bReCreate )
	{
		BOOL bTestFileCreated = FALSE;
		bTestFileCreated = CopyFile(sMailAccountFile, sTestCfgFile, FALSE);
		if (!bTestFileCreated)
		{
			this->m_sTestCfgFile.Empty();
			return -1;
		}
		//this->m_sTestCfgFile = sTestCfgFile;
	}
	this->m_sTestCfgFile = sTestCfgFile;
		
	return 0;
}


VOID CMailMgrTestDlg::OnchkDELETELOGFILE() 
{
	if (m_sLogFilepath.IsEmpty()) return;
	CFileStatus status;
	if ( CFile::GetStatus( m_sLogFilepath, status ) )
	{
		CFile::Remove(this->m_sLogFilepath);	
	}

	this->m_ctrlDeleteLogFile.SetCheck(0);
}

VOID CMailMgrTestDlg::OnKillfocuseRecipientNameOrEmail() 
{
	UpdateData();
	m_ctvRecipientName.TrimLeft(); 
	m_ctvRecipientName.TrimRight();
	m_ctvRecipientEmail.TrimLeft();
	m_ctvRecipientEmail.TrimRight();

	CString sBtnName;
	this->GetDlgItemText(IDC_btnMAIL_SERVER, sBtnName);

	BOOL bEnable = ( !m_ctvRecipientName.IsEmpty()			&& 
		             !m_ctvRecipientEmail.IsEmpty()			&& 
					 0<m_ctvRecipientEmail.Find(_T("@"))		&& 
					 m_ctvRecipientEmail.Right(1) != _T("@")	&&
					 sBtnName.Compare(_T("�׽�Ʈ�� ���� ����"))		);
	((CWnd*)(GetDlgItem(IDC_btnMAIL_SEND)))->EnableWindow(bEnable);
}

VOID CMailMgrTestDlg::OnSetfocuseRECIPIENTEMAIL() 
{
/*
	HIMC hIME;
	DWORD dwConversion, dwSentence;
	HWND hWnd = ((CEdit*)(GetDlgItem(IDC_eRECIPIENT_EMAIL)))->GetSafeHwnd();	
	hIME = ::ImmGetContext(hWnd);
	if(hIME) 
	{
		ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_ALPHANUMERIC; 
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
		ImmReleaseContext(hWnd, hIME); 
	}
*/
}


VOID CMailMgrTestDlg::OnchkVIEWLOGFILE() 
{
	SetDlgItemText(IDC_btnMAIL_SERVER, _T("�����׽�Ʈ�� ����"));
	ShowMailLog(this->m_sLogFilepath);
}
