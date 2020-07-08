// KalendarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Kalendar.h"
#include "KalendarDlg.h"
#include "DlgListView.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
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
// CKalendarDlg dialog

CKalendarDlg::CKalendarDlg(CWnd* pParent /*=NULL*/)
	: CTabDialog(CKalendarDlg::IDD, pParent)
{	

	//{{AFX_DATA_INIT(CKalendarDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

//	m_hTitleIcon = m_hIcon;
//	m_sTitle  = _T("     Kalendar");
	m_bIsConnectDB = FALSE;
	m_bAutoTabSize = FALSE;
	m_nBorderPos = TAB_BORDER_TOP;
	m_nXpos = 100;

	m_pDlgAlarm		= NULL;
	m_pDlgList		= NULL;
	m_pDlgMemo		= NULL;
	m_pDlgSearchList	= NULL;
	for(INT nCount=0; nCount < MAXDAY; nCount++)
	{
		m_Days[nCount] = NULL;
		m_SEQDays[nCount] = NULL;
	}


	m_bCloseBox = TRUE;
	m_strAuth = _T("");
	m_strManageCode = _T("");
	m_bIsInit = FALSE;

	m_ctlSlider = NULL;

	m_nMode = -1;

	m_pWndKolasIII = new CWnd;

	m_phFileMap = NULL;


	m_bIsSocketMode = TRUE;

	m_uYear = 0;
	m_uMonth = 0;	
	m_aSend_DivisionNo_To_KolasIII.RemoveAll();


	m_lpMapView = NULL;
	for(int i=0;i<6;i++)
		m_recInitCtrlRect[i] = NULL;
	
	m_UserInfo.strUser_ID =_T("");
	m_UserInfo.strUser_Name = _T("");
}

CKalendarDlg::~CKalendarDlg()
{	
  	for(INT nCount=0; nCount < MAXDAY; nCount++)
  	{
  		if(NULL != m_Days[nCount]) 
  		{
  		 	delete m_Days[nCount];
  		}
  		if(NULL != m_SEQDays[nCount])
  		{
   			delete m_SEQDays[nCount];
  		}
  	}	
  
//DEL   �����߻� ���� ��������
   	if(NULL != m_pWndKolasIII)
	{
		m_pWndKolasIII->Detach();
		delete m_pWndKolasIII;
		m_pWndKolasIII = NULL;
	}
  	if(NULL != m_pDlgAlarm)		delete m_pDlgAlarm;
  	if(NULL != m_pDlgList)		delete m_pDlgList;
  	if(NULL != m_pDlgMemo)		delete m_pDlgMemo;
  	if(NULL != m_pDlgSearchList)	delete m_pDlgSearchList;
  	
	if(NULL != m_ctlSlider) delete m_ctlSlider;

 	Close_FileMapping();
}

void CKalendarDlg::DoDataExchange(CDataExchange* pDX)
{
	// �� ���̾�α� �߰�
//	CDialog::DoDataExchange(pDX);
	CTabDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKalendarDlg)
	DDX_Control(pDX, IDC_BUTTON_TODAY, m_btnToday);
	DDX_Control(pDX, IDC_BUTTON_YEAR, m_btnYear);
	DDX_Control(pDX, IDC_BUTTON_MONTH, m_btnMonth);
	DDX_Control(pDX, IDC_BUTTON_MONTH_ENG, m_btnMonthEng);
 	DDX_Control(pDX, IDC_BUTTON_SEQ_MODE, m_btnDivisionNoView);
 	DDX_Control(pDX, IDC_BUTTON_MEMO_MODE, m_btnMemoView);
	DDX_Control(pDX, IDC_BUTTON_PREV_MONTH, m_btnPrevMonth);
	DDX_Control(pDX, IDC_BUTTON_NEXT_MONTH, m_btnNextMonth);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_btnListView);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BUTTON_DROPDOWN_T, m_btnDropDown_T);
	DDX_Control(pDX, IDC_BUTTON_DROPDOWN_B, m_btnDropDown_B);
	DDX_Control(pDX, IDC_BUTTON_DROPDOWN_L, m_btnDropDown_L);
	DDX_Control(pDX, IDC_BUTTON_DROPDOWN_R, m_btnDropDown_R);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKalendarDlg, CDialog)
	//{{AFX_MSG_MAP(CKalendarDlg)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_YEAR, OnButtonYear)
	ON_BN_CLICKED(IDC_BUTTON_MONTH, OnButtonMonth)
	ON_BN_CLICKED(IDC_BUTTON_MONTH_ENG, OnButtonMonthEng)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_PREV_MONTH, OnButtonPrevMonth)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_MONTH, OnButtonNextMonth)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)	
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON_SEQ_MODE, OnButtonSeqMode)
	ON_BN_CLICKED(IDC_BUTTON_MEMO_MODE, OnButtonMemoMode)
	ON_BN_CLICKED(IDC_BUTTON_TODAY, OnButtonToday)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_YEAR, ID_YEAR_NEXT_5, OnYear)
 	ON_COMMAND_RANGE(ID_MONTH, ID_MONTH_12, OnMonth)
	ON_MESSAGE(WM_REQUEST_KALENDARDLG, OnProcessRequest)
	ON_MESSAGE(WM_USER + 862, OnMyMethod)
	ON_MESSAGE(1113, Send_NextDivisonNo)
	ON_MESSAGE(WM_USER+2, OnToggleKalendar)
	ON_COMMAND(ID_MENU_REFRESH, OnbtnRefreshSEQ)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_DROPDOWN_T, IDC_BUTTON_DROPDOWN_R, ShowTabWindow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKalendarDlg message handlers

// �޷��� ����
BOOL CKalendarDlg::OnInitDialog()
{		

	DELETELOG();
	LOG(_T("In : CKalendarDlg::OnInitDialog"));

//////////////////////////////////////////////////////////////
// ���⼭���� ����������� �˻�
	// �ߺ������
	if(TRUE == ChkRun())
	{		
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
 		CDialog::OnCancel();
		LOG(_T("Out: (FALSE) CKalendarDlg::OnInitDialog �ߺ�����"));
		return FALSE;
	}

// 	CString strDir = _T("");
// 	CCalFileMgr::Get_FILEDIR(strDir);
// 
//  	CFileFind filefind;
//  	if(TRUE == filefind.FindFile(strDir + _T("DO_NOT_EXCUTE")))
// 	{
// 		AfxGetMainWnd()->SendMessage(WM_CLOSE);
// 		CDialog::OnCancel();
// 		return FALSE;
// 	}
// ������� ����������� �˻�
//////////////////////////////////////////////////////////////

	// �������� �б�
	ReadConfigFile();
	TRACE(_T("MODE %d\r\n"), GetMode());

	CTabDialog::OnInitDialog();
	
	m_btnDropDown_L.MoveWindow(0, 0, 0, 0);
	m_btnDropDown_T.MoveWindow(0, 0, 0, 0);
	m_btnDropDown_R.MoveWindow(0, 0, 0, 0);
	m_btnDropDown_B.MoveWindow(0, 0, 0, 0);


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon	
	// TODO: Add extra initialization here	
	
	
	SetWindowText(_T("Kalendar"));
//////////////////////////////////////////////////////////////
// ���⼭���� ���� ��Ʈ�� �ʱ�ȭ
	InitBG();
	InitTime();	
	m_AlarmTimeMgr.ReadAlarmFile();		
	InitDlgMemo();
	InitDlgList();			
 	InitDlgDay();
	InitDlgSEQ();	
	InitDM();
	LoadBitmapForButton();

	// ��Ʈ�� �̵�
	//����ÿ��� ���� ����鼭 ����� ��Ʈ���� ��ġ�� �޶����� ����
	MoveAllCtrl();
// ������� ���� ��Ʈ�� �ʱ�ȭ
//////////////////////////////////////////////////////////////
	
	// ��� ����
	SetMode(GetMode());
	
 	m_bIsConnectDB = ConnectDB();

	CString strResult[2];
	CStdioFile file;

	// 2009.03.11 UPDATE BY CJY : �������� ������������ �α��� �� ��츦 ���� PID�� �����Ѵ�.
	DWORD dwSID = ::GetCurrentProcessId();
	CString strCHMfileName;
	strCHMfileName.Format(_T("..\\CFG\\EFS\\user_info(%u).log"), dwSID);
		
	if (file.Open(strCHMfileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		CString strLine;
		strLine.Format(_T("%s\r\n"), m_strManageCode);
		file.WriteString(strLine);
		file.Close();
	}

	Refresh_SEQ_DM();

	// �����/�Ͽ��� ���ڻ��� ����
	SetWeekend();

	// ����޷� ����
	SetKalendar();	
	
	// �˶�üũ�� ���� Ÿ�̸�
	SetTimer(ID_TIMER_ALARMTIME, 5000, NULL);
	// ���� �����, �����ٶ��� ��ũ�� ȿ������
	SetScroll(6, 10);	// �� 6���� ���� �����ְ�, ��ũ�ѵ����̴� 10
	m_bIsInit = TRUE;

	m_ctlSlider = new CSlider;
	m_ctlSlider->Create(this, 740, 715, _T("Kalendar"));

	LOG(_T("Out: CKalendarDlg::OnInitDialog"));

	Open_FileMapping();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKalendarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CTabDialog::OnSysCommand(nID, lParam);
//		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKalendarDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{			
		CTabDialog::OnPaint();

		static int tmp = 0;
		if(25<=tmp)
			tmp = 0;
		else
			tmp++;
		CDC* pDC = GetDC();
		CRect rect; 
		GetClientRect( &rect );	
//		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_BGDC, tmp, 0, SRCCOPY);	
		ReleaseDC(pDC);
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKalendarDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// �˻��ϱ� ��ư Ŭ����
void CKalendarDlg::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	m_pDlgSearchList->ResetList();		
	CRect recList, recButton;
	m_pDlgSearchList->GetWindowRect(recList);
	m_btnSearch.GetWindowRect(recButton);	

	int nWidth = recList.right - recList.left;
	int nHeight = recList.bottom - recList.top;
	
	recList.right = recButton.right;
	recList.left = recList.right - nWidth;
	recList.top = recButton.bottom + 5;
	recList.bottom = recList.top + nHeight;

	//++2008.08.13 UPDATE BY CJY {{++
	// ȭ��ۿ��� ���涧�� �ܻ�����
	if(TRUE == m_pDlgSearchList->IsWindowVisible())
		m_pDlgSearchList->ShowWindow(SW_HIDE);
	else
	{
		m_pDlgSearchList->MoveWindow(recList, FALSE);
		m_pDlgSearchList->ShowWindow(SW_SHOW);
	}

	// �����ҽ�
	// m_pDlgSearchList->MoveWindow(recList, FALSE);
	// m_pDlgSearchList->ShowWindow(!m_pDlgSearchList->IsWindowVisible());
	//--2008.08.13 UPDATE BY CJY --}}
}

// �� ��ư Ŭ�����
// �� �̵� �˾��޴� ����
void CKalendarDlg::OnButtonMonth() 
{
	// TODO: Add your control notification handler code here
	CMenu  *sMenu,*pMenu;
	pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU_MONTH);	
	sMenu = pMenu->GetSubMenu(0);
	
	// �޴��� ��ư���ʾƷ��� ����
	CRect rect;
	GetDlgItem(IDC_BUTTON_MONTH)->GetWindowRect(&rect);
	sMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN, rect.left+20, rect.bottom + 5, this);
	delete pMenu;
}

// �⵵ ��ư�� Ŭ����
// �⵵�̵� �˾��޴�����
void CKalendarDlg::OnButtonYear() 
{
	// TODO: Add your control notification handler code here
	CMenu  *sMenu,*pMenu;
	pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU_YEAR);	
	sMenu = pMenu->GetSubMenu(0);
	
	CString st;
	for(int i=0;i<11;i++)
	{		
		st.Format(_T("%d"), m_DateCalculator.GetYear()+i-5);
		sMenu->ModifyMenu(
			ID_YEAR_PREV_5 + i, MF_BYCOMMAND | MF_STRING, ID_YEAR_PREV_5 + i,  st);
	}	
	sMenu->ModifyMenu(ID_YEAR, MF_BYCOMMAND | MF_STRING, ID_YEAR,  _T("����⵵"));

	// �޴��� ��ư���ʾƷ��� ����
	CRect rect;
	GetDlgItem(IDC_BUTTON_YEAR)->GetWindowRect(&rect);
	sMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN, rect.left -3, rect.bottom + 3, this);
	delete pMenu;
}

// ����Ʈ�� �����ư Ŭ����
void CKalendarDlg::OnButtonView() 
{
	// TODO: Add your control notification handler code here	
	m_pDlgList->ResetList();	
	
	CRect recList, recButton;
	m_pDlgList->GetWindowRect(recList);
	m_btnListView.GetWindowRect(recButton);	

	int nWidth = recList.right - recList.left;
	int nHeight = recList.bottom - recList.top;
	
	recList.right = recButton.right;
	recList.left = recList.right - nWidth;
	recList.top = recButton.bottom + 5;
	recList.bottom = recList.top + nHeight;
	
	//++2008.08.13 UPDATE BY CJY {{++
	// ȭ��ۿ��� ���涧�� �ܻ�����
	SetList();
	if(TRUE == m_pDlgList->IsWindowVisible())
		m_pDlgList->ShowWindow(SW_HIDE);
	else
	{
		m_pDlgList->MoveWindow(recList, FALSE);
		m_pDlgList->ShowWindow(SW_SHOW);
	}

	// �����ҽ�
	// m_pDlgList->MoveWindow(recList, FALSE);
	// SetList();
	// m_pDlgSearchList->ShowWindow(!m_pDlgSearchList->IsWindowVisible());
	//--2008.08.13 UPDATE BY CJY --}}

}

// ���� ���� ������ ���
void CKalendarDlg::OnButtonPrevMonth() 
{
	LOG(_T("   : CKalendarDlg::OnButtonPrevMonth"));
	// TODO: Add your control notification handler code here	
	if(1==m_uMonth)
	{
		m_uMonth = 12;
		m_uYear -= 1;	
		Refresh_SEQ_DM();
	}
	else
	{
		m_uMonth -= 1;
	}
	SetKalendar();
}

// ���� ���� ������ ���
void CKalendarDlg::OnButtonNextMonth() 
{
	LOG(_T("   : CKalendarDlg::OnButtonNextMonth"));
	// TODO: Add your control notification handler code here
	if(12==m_uMonth)
	{
		m_uMonth = 1;
		m_uYear += 1;
		Refresh_SEQ_DM();
	}
	else
	{
		m_uMonth += 1;
	}
	SetKalendar();
}

// ���� �޷¼���
// �޷¿� ��¥ǥ��(1���� ������ ���� �� ���� ��Ʈ�Ѻ��� ��¥ǥ�����)
//��	    ��		ȭ	    ��		��	    ��		��
//IDC_1	IDC_2	IDC_3	IDC_4	IDC_5	IDC_6	IDC_7
// ���� 1���� �Ͽ����ΰ�쿣 IDC_EDIT1�� 1��, IDC_EDIT2�� 2��������..
// ���� 1���� ȭ�����ΰ�쿣 IDC_EDIT3�� 1��, IDC_EDIT4�� 2��....
void CKalendarDlg::SetKalendar()
{
	CString strTmp;
	strTmp.Format(_T("   : CKalendarDlg::SetKalendar [%d/%d]"), m_uYear, m_uMonth);
	LOG(strTmp);
	AfxGetApp()->DoWaitCursor(1);

	EnableWindow(FALSE);
	ResetKalendar();

	CString st;
	st.Format(_T("%d"), m_uYear);
	GetDlgItem(IDC_BUTTON_YEAR)->SetWindowText(st);
	st.Format(_T("%d"), m_uMonth);
	GetDlgItem(IDC_BUTTON_MONTH)->SetWindowText(st);	
	if(1841>m_uYear || m_uYear>2042)
	{
		CString strTmp;
		strTmp.Format(_T("ERROR : �������� �ʴ� �⵵(%d)"), m_uYear);		
		LOG(strTmp);
		AfxMessageBox(_T("�������� �ʴ� �⵵�Դϴ�."));
		return;
	}
	m_btnYear.SetYear(m_uYear);
	m_btnYear.DrawItem(NULL);
	m_btnMonth.SetMonth(m_uMonth);
	m_btnMonth.DrawItem(NULL);
	m_btnMonthEng.SetMonth(m_uMonth);
	m_btnMonthEng.DrawItem(NULL);

	//++2008.07.16 ADD BY CJY {{++
	Add_SEQData();
	//--2008.07.16 ADD BY CJY --}}

	// ��¥ ����
	// ��¥ǥ�ø� ������ ��Ʈ�ѳѹ�	/ GetDayofWeek�� ���ϰ�: �Ͽ����� 1, ȭ������ 2...
	int StartIdNum = m_DateCalculator.GetDayOfWeek(m_uYear, m_uMonth, 1) - 1;	
	// 1�Ϻ��� �ش� ���� ��¥����ŭ�� ǥ��(���� 4���� 30�ϱ����� ǥ��)
	for(int i=0;i<m_DateCalculator.GetDaysOfMonth(m_uYear, m_uMonth);i++)
	{		
		m_Days[StartIdNum+i]->SetDay(m_uYear, m_uMonth, i+1);
		m_SEQDays[StartIdNum+i]->SetDay(m_uYear, m_uMonth, i+1);
		//++2008.07.14 ADD BY CJY {{++
		if(DIVISION_NO_VIEW_MODE == GetMode())
			m_SEQDays[StartIdNum+i]->ShowWindow(SW_SHOWNA);
		//--2008.07.14 ADD BY CJY --}}
	}
	//++2008.07.14 ADD BY CJY {{++
	// ��Ÿ ��¥�� �����
	for(; StartIdNum+i < MAXDAY; i++)
	{
		m_SEQDays[StartIdNum+i]->ShowWindow(SW_HIDE);
	}
	for(i=0; i < StartIdNum; i++)
	{
		m_SEQDays[i]->ShowWindow(SW_HIDE);
	}
	//--2008.07.14 ADD BY CJY --}}
	CTime t= CTime::GetCurrentTime();
	if(t.GetYear()==(int)m_uYear&&t.GetMonth()==(int)m_uMonth)
	{
		m_Days[GetDlgDayIndex(m_uYear, m_uMonth, t.GetDay())]
			->SetToday();
		m_SEQDays[GetDlgDayIndex(m_uYear, m_uMonth, t.GetDay())]
			->SetToday();
	}


	SetHoliday();
	SetList();	
	SetTimer(ID_TIMER_WAIT_SET_KALENDAR, 100, NULL);		
}

// ȭ�鿡 ǥ�õ� ��¥ ���� �����
void CKalendarDlg::ResetKalendar()
{	
//DEL 	Reset_KalendarForDlgDays();
	for(int i=0;i<MAXDAY;i++)
	{			
		m_Days[i]->ResetDay();	
		m_SEQDays[i]->ResetDay();
	}	
	m_pDlgList->ResetList();
}

// ȭ�鿡 ǥ�õ� ��¥ ���� �����
//DEL void CKalendarDlg::Reset_KalendarForDlgDays()
//DEL {	
//DEL 	for(int i=0;i<MAXDAY;i++)
//DEL 	{			
//DEL 		m_Days[i]->ResetDay();		
//DEL 	}	
//DEL 	m_pDlgList->ResetList();
//DEL }

void CKalendarDlg::OnMonth(UINT nID) 
{
	// TODO: Add your command handler code here
 	switch(nID)
 	{
	case ID_MONTH:		m_uMonth = (CTime::GetCurrentTime()).GetMonth(); break;
 	case ID_MONTH_1:	m_uMonth = 1;	break;
 	case ID_MONTH_2: 	m_uMonth = 2;	break;
 	case ID_MONTH_3:	m_uMonth = 3;	break;
 	case ID_MONTH_4:	m_uMonth = 4;	break;
 	case ID_MONTH_5:	m_uMonth = 5;	break;
 	case ID_MONTH_6:	m_uMonth = 6;	break;
 	case ID_MONTH_7:	m_uMonth = 7;	break;
 	case ID_MONTH_8:	m_uMonth = 8;	break;
 	case ID_MONTH_9:	m_uMonth = 9;	break;
 	case ID_MONTH_10:	m_uMonth = 10;	break;
 	case ID_MONTH_11:	m_uMonth = 11;	break;
 	case ID_MONTH_12:	m_uMonth = 12;	break;
	default:
		break;
 	}

	SetKalendar();
}
void CKalendarDlg::OnYear(UINT nID)
{
	INT nYear = (CTime::GetCurrentTime()).GetYear();

	switch(nID)
	{
	case ID_YEAR:		m_uYear = nYear;	break;
	case ID_YEAR_PREV_5:	m_uYear = nYear-5;	break;
	case ID_YEAR_PREV_4:	m_uYear = nYear-4;	break;
	case ID_YEAR_PREV_3:	m_uYear = nYear-3;	break;
	case ID_YEAR_PREV_2:	m_uYear = nYear-2;	break;
	case ID_YEAR_PREV_1:	m_uYear = nYear-1;	break;	
	case ID_YEAR_:		m_uYear = nYear;	break;
	case ID_YEAR_NEXT_1:	m_uYear = nYear+1;	break;
	case ID_YEAR_NEXT_2:	m_uYear = nYear+2;	break;
	case ID_YEAR_NEXT_3:	m_uYear = nYear+3;	break;
	case ID_YEAR_NEXT_4:	m_uYear = nYear+4;	break;
	case ID_YEAR_NEXT_5:	m_uYear = nYear+5;	break;
	default:
		break;		
	}
	Refresh_SEQ_DM();
	SetKalendar();	
}

// ��¥ �ʱ�ȭ
void CKalendarDlg::InitDlgDay()
{
	LOG(_T("   : CKalendarDlg::InitDlgDay"));
	CRect rec;	
	rec.left = LEFTSPACE;
	rec.right = rec.left + DAYWIDTH;
	rec.top = TITLESPACE;
	rec.bottom = rec.top + DAYHEIGHT;

	for(int i=0;i<MAXDAY;i++)
	{
		m_Days[i] = new CDlgDay;	
		m_Days[i]->Create(IDD_DIALOG_DAY, this);
//MACRODEL		m_Days[i]->m_pDlgMemo = m_pDlgMemo;
		MoveDlgDay(i);
//DEL 		m_Days[i]->ShowWindow(SW_SHOW);
	}
//MACRODEL	// �Ʒ�CDlgDay�� ȭ��󿡴� ������ ������ 
//MACRODEL	// ���� ȭ�鿡 �������� ��¥�� ����� ���� ��¥�� ����ų �� ���. 
//MACRODEL	// ��/ȭ�鳯¥�� 2008��2�� ������ 3���� Ư������ ������ �ʿ��Ѱ��
//MACRODEL	m_Days[MAXDAY-1]->ShowWindow(false);
}

HBRUSH CKalendarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CTabDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject( NULL_BRUSH );
	}	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CKalendarDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default		
	CRect rect; 
	GetClientRect( &rect );	
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_BGDC, 0, 0, SRCCOPY);				
	return TRUE;
}

// �� ��¥ ���̾�α��� �̵�
void CKalendarDlg::MoveDlgDay(int Index)
{	
	int X = m_nTabHeight, Y = m_nTabHeight;
	
	// ������ġ
	CRect rec;
	rec.left = LEFTSPACE + X;
	rec.right = rec.left + DAYWIDTH;
	rec.top = TITLESPACE + Y;
	rec.bottom = rec.top + DAYHEIGHT;

	// Index�� ������������
	int DayOfWeek = Index % 7;
	// Index�� ����������
	int Week = Index / 7;

	rec.left = rec.left + (DayOfWeek * (DAYWIDTH + WIDTHSPACE));
	rec.right = rec.left + DAYWIDTH;
	rec.top = rec.top + (Week * (DAYHEIGHT + HEIGHTSPACE));
	rec.bottom = rec.top + DAYHEIGHT;
	
	m_Days[Index]->MoveWindow(&rec);
}

// �� ��¥ ���̾�α��� �̵�
void CKalendarDlg::MoveSEQDay(int Index)
{	
	int X = m_nTabHeight, Y = m_nTabHeight;
	
	// ������ġ
	CRect rec;
	rec.left = LEFTSPACE + X;
	rec.right = rec.left + DAYWIDTH;
	rec.top = TITLESPACE + Y;
	rec.bottom = rec.top + DAYHEIGHT;

	// Index�� ������������
	int DayOfWeek = Index % 7;
	// Index�� ����������
	int Week = Index / 7;

	rec.left = rec.left + (DayOfWeek * (DAYWIDTH + WIDTHSPACE));
	rec.right = rec.left + DAYWIDTH;
	rec.top = rec.top + (Week * (DAYHEIGHT + HEIGHTSPACE));
	rec.bottom = rec.top + DAYHEIGHT;
	
	m_SEQDays[Index]->MoveWindow(&rec);
}

int CKalendarDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CKalendarDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CTabDialog::Create(IDD, pParentWnd);
}

void CKalendarDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CTabDialog::OnMouseMove(nFlags, point);
	
	MoveDropDownButton();
	m_btnClose.MoveWindow(m_closeRect);
}

void CKalendarDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	LOG(_T("   : CKalendarDlg::OnLButtonDown"));

	// TODO: Add your message handler code here and/or call default		
//MACRODEL	m_pDlgList->ShowWindow(false);	
//MACRODEL	m_pDlgMemo->SaveMemo();	
	SaveMemo();
	m_pDlgMemo->ShowWindow(SW_HIDE);
	CTabDialog::OnLButtonDown(nFlags, point);	
}

void CKalendarDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CTabDialog::OnLButtonUp(nFlags, point);
}

//Ÿ�̸�
// �˶�üũ
void CKalendarDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default	
	if(!m_bIsInit)
		return;
	if(ID_TIMER_WAIT_SET_KALENDAR == nIDEvent)
	{
		KillTimer(ID_TIMER_WAIT_SET_KALENDAR);
		EnableWindow(TRUE);
		AfxGetApp()->DoWaitCursor(-1);
	}
	else if(ID_TIMER_ALARMTIME == nIDEvent)
	{
		OnTimerForAlarmTime();
	} 

	CTabDialog::OnTimer(nIDEvent);
}

void CKalendarDlg::InitDlgMemo()
{		
	LOG(_T("   : CKalendarDlg::InitDlgMemo"));

	m_pDlgMemo = new CDlgMemo;
	if(FALSE==m_pDlgMemo->Create(CDlgMemo::IDD, this))
		LOG(_T("ERROR : [CKalendarDlg::InitDlgMemo] m_pDlgMemo->Create"));		

	m_pDlgAlarm = new CDlgAlarm;
	if(FALSE==m_pDlgAlarm->Create(CDlgAlarm::IDD, this))
		LOG(_T("ERROR : [CKalendarDlg::InitDlgMemo] m_pDlgAlarm->Create"));
}

// �󼼺��� �� �˻��ϱ� ���̾���� �ʱ�ȭ
void CKalendarDlg::InitDlgList()
{
	LOG(_T("   : CKalendarDlg::InitDlgList"));
	m_pDlgList = new CDlgListView;
	if(FALSE==m_pDlgList->Create(CDlgListView::IDD, this))
		LOG(_T("ERROR : [CKalendarDlg::InitDlgList] m_pDlgList->Create"));		
//MACRODEL	m_pDlgList->m_pDlgMemo = m_pDlgMemo;		

	m_pDlgSearchList = new CDlgListView;
	if(FALSE==m_pDlgSearchList->Create(CDlgListView::IDD, this))
		LOG(_T("ERROR : [CKalendarDlg::InitDlgList] m_pDlgSearchList->Create"));		
//MACRODEL	m_pDlgSearchList->m_pDlgMemo = m_pDlgMemo;	

	m_pDlgSearchList->m_StaticYear.ShowWindow(true);
	m_pDlgSearchList->m_btnSearch.ShowWindow(true);
	m_pDlgSearchList->m_EditSearchText.ShowWindow(true);
	m_pDlgSearchList->m_EditSearchYear.ShowWindow(true);
	m_pDlgSearchList->m_StaticDate.ShowWindow(false);
}

LRESULT CKalendarDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CTabDialog::WindowProc(message, wParam, lParam);
}

void CKalendarDlg::SetList()
{	
	CString stDay = _T(""), stText = _T("");		
	m_pDlgList->ResetList();
	m_pDlgList->SetDate(m_uYear, m_uMonth);
	for(int i=0;i<MAXDAY;i++)
	{
		if(m_Days[i]->IsDay()&&!m_Days[i]->IsDataEmpty())
		{
			UINT uYear, uMonth, uDay;
			m_Days[i]->GetDate(&uYear, &uMonth, &uDay);
			stText = m_Days[i]->GetText();
			m_pDlgList->AddList(uYear, uMonth, uDay, stText);
		}
	}
	m_pDlgList->InvalidateHTMLList(FALSE);
}

// �Լ��̸�: CKalendarDlg::OnNotify
// �Լ�����: �� �ڽ� ���̾�α׿��� ������ �޽��� ó��
// ��ȯ  ��: BOOL 
// ����[WPARAM wParam] : �޽����� ���� Ŭ������ ���̵�
// ����[LPARAM lParam] : ���� ������
// ����[LRESULT* pResult] : 
BOOL CKalendarDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	LOG(_T("   : CKalendarDlg::OnNotify"));
	CData* pData = (CData*)lParam;
	switch(wParam)
	{		
	// ��¥�� Ŭ������ ��
	// �޸��� �����͸� ��¥�� �����ͷ� ����
	case CDlgDay::IDD:
		OnNotifyForDlgDay(pData);		
		break;
	// �޸𿡼� Ȯ�ι�ư�� ������ ��
	// �޸��� ������ ��¥�� ����(�ش� ��¥�� ������� ���Ϸ� ����)
	case CDlgMemo::IDD:
		OnNotifyForDlgMemo(pData);		
		break;
	// �󼼺���/�˻�â���� ����Ʈ�� �������� ��
	// �޸��� �����͸� ����Ʈ�� �ش��ϴ� �����ͷ� ����
	case CDlgListView::IDD:
		OnNotifyForDlgListView(pData);		
		break;
	// �˶�â�� ����� ��	
	case CDlgAlarm::IDD:
		OnNotifyForDlgAlarm(pData);
		break;
	// ����ڰ� ������ȣ�� Ŭ���Ͽ��� ��
	// KOLASIII�� �����ϱ�
	case CDlgSEQ::IDD:
		OnNotifyForDlgSEQ(pData);
		break;
	default:
		break;
	}
	return TRUE;
}
/********************************************************
�Ʒ� 6�� ��Ʈ���� ��ġ����
IDC_BUTTON_VIEW			�󼼺��� ��ư
IDC_BUTTON_SEARCH		�˻��ϱ� ��ư
IDC_BUTTON_YEAR			�⵵���� ��ư		
IDC_BUTTON_MONTH		�� ���� ��ư
IDC_BUTTON_PREV_MONTH	���� �� ��ư
IDC_BUTTON_NEXT_MONTH	���� �� ��ư
*********************************************************/

// ��Ʈ���� �̵�
// ���̾�α׿� ���� �׷����� �ϱ� ������ 
// ���� ũ�⸸ŭ ��Ʈ�ѵ��� �̵��Ǿ�� �Ѵ�.
void CKalendarDlg::MoveAllCtrl()
{
	LOG(_T("   : CKalendarDlg::MoveAllCtrl"));
//	�̵��Ǵ� ��Ʈ���� �Ʒ��� 6��
//	IDC_BUTTON_VIEW            �󼼺��� ��ư  
//	IDC_BUTTON_SEARCH           �˻��ϱ� ��ư
//	IDC_BUTTON_YEAR               �� �̵� ��ư
//	IDC_BUTTON_MONTH            �� �̵� ��ư
//	IDC_BUTTON_PREV_MONTH	���� �� �̵���ư
//	IDC_BUTTON_NEXT_MONTH   ���� �� �̵���ư
//	IDC_BUTTON_SEQ_MODE	�������
//	IDC_BUTTON_MEMO_MODE	�޸���
//	IDC_BUTTON_TODAY	������
	CRect rec;
	const UINT uStart = IDC_BUTTON_VIEW;
	const UINT uEnd = IDC_BUTTON_TODAY;
	for(UINT i=uStart; i<=uEnd; i++)
	{
		GetDlgItem(i)->GetWindowRect(&rec);
		ScreenToClient(&rec);		
		rec.left += m_nTabHeight;
		rec.right += m_nTabHeight;
		rec.top += m_nTabHeight;
		rec.bottom += m_nTabHeight;
		GetDlgItem(i)->MoveWindow(rec);
	}
	m_btnClose.MoveWindow(m_closeRect);
//DEL 	m_btnDropDown_T.MoveWindow(m_rDropdown_T);
//DEL 	m_btnDropDown_B.MoveWindow(m_rDropdown_B);
//DEL 	m_btnDropDown_L.MoveWindow(m_rDropdown_L);
//DEL 	m_btnDropDown_R.MoveWindow(m_rDropdown_R);
}

// ��¥�� �ε��� ��ȯ
// ã�����ϴ� ��¥�� ��/��/���� �޾Ƽ� �� ��¥�� �迭�� ���° �ε������� ��ȯ
// �ش� �����ÿ��� -1 ��ȯ
int CKalendarDlg::GetDlgDayIndex(UINT uYear, UINT uMonth, UINT uDay)
{
	// ã�����ϴ� ��¥���� �ùٸ��� ���� ���
	if( (m_uYear != uYear) || (m_uMonth != uMonth) || 0==uDay || 31<uDay)
	{
		return -1;
	}		
	if(m_uYear==uYear&&m_uMonth==uMonth)
	{
		int StartIdNum = m_DateCalculator.GetDayOfWeek(m_uYear, m_uMonth, 1) - 1;	
		return StartIdNum + uDay -1;	
	}
	// ���� �޷��� ���� ��¥�� ã�� ���
	else
		return -1;
//MACRODEL	{
//MACRODEL		// MAXDAY-1��° �迭���� ȭ��� ǥ�õǴ� �ް� ������ ��¥�� �ʿ�ÿ� ���		
//MACRODEL		if(m_Days[MAXDAY-1]->GetDay() == uDay)
//MACRODEL			return MAXDAY-1;
//MACRODEL	}
		
}

// ��Ʈ�ʹ�ư �ҷ�����
void CKalendarDlg::LoadBitmapForButton()
{
	LOG(_T("   : CKalendarDlg::LoadBitmapForButton"));

 	m_btnNextMonth.LoadBitmaps(IDB_BITMAP_BUTTON_NEXTMONTH);
 	m_btnPrevMonth.LoadBitmaps(IDB_BITMAP_BUTTON_PREVMONTH);
 	m_btnSearch.LoadBitmaps(IDB_BITMAP_BUTTON_SEARCH);
 	m_btnListView.LoadBitmaps(IDB_BITMAP_BUTTON_LISTVIEW);
 	m_btnClose.LoadBitmaps(IDB_BITMAP_CLOSE_KALENDAR);
   
 	m_btnDivisionNoView.LoadBitmaps(IDB_BITMAP_DIVISION_NO);
 	m_btnMemoView.LoadBitmaps(IDB_BITMAP_BUTTON_MEMO);
 	m_btnToday.LoadBitmaps(IDB_BITMAP_BUTTON_TODAY);

	m_btnDropDown_T.LoadBitmaps(IDB_BITMAP_BUTTON_DROPDOWN_T);
	m_btnDropDown_B.LoadBitmaps(IDB_BITMAP_BUTTON_DROPDOWN_B);
	m_btnDropDown_L.LoadBitmaps(IDB_BITMAP_BUTTON_DROPDOWN_L);
	m_btnDropDown_R.LoadBitmaps(IDB_BITMAP_BUTTON_DROPDOWN_R);
}

// �ƱԸ�Ʈ ����
VOID CKalendarDlg::SetAuth(CString strCmdLine)
{
	m_strAuth = strCmdLine;
}

// ������ ����
// �����ͺ��̽����� ������������ �޾ƿ� �ش糯¥�� �������� �����Ͽ� �ش�.
void CKalendarDlg::SetHoliday()
{	
	LOG(_T("   : CKalendarDlg::SetHoliday"));
	if(FALSE==m_bIsConnectDB)
		return;

	// ���� �޷��� ���� ������ ���� �޾ƿ´�.
	CString strValue = ReadHolidayFromDB();
	CString st, stTemp;
	int nOffset;
	UINT uNum;

	// �޾ƿ� ������ ���� ���ڿ��� ��ǥ������ ���еǾ� �ִ�.
	// �� ���� ��ǥ������ �߶� �ش糯¥�� �����Ϸ� �����Ͽ� �ش�.
	// ��> "1, 9, 13, 23" -> 1�� 9�� 13�� 23�� ������ ����
	while(true)
	{
		nOffset = strValue.Find(_T(", "));
		if(0>=nOffset)
			stTemp = strValue;
		else
		{
			stTemp = strValue.Left(nOffset);
			strValue = strValue.Mid(nOffset +2);
		}		
		uNum = CAlarmTime::GetConvertStringToUINT(stTemp, 2);
		int nIndex = GetDlgDayIndex(m_uYear, m_uMonth, uNum);
		if(-1!=nIndex)
		{
			m_Days[nIndex]->SetHoliday();
			m_SEQDays[nIndex]->SetHoliday();
		}
		if(0>=nOffset)
			break;
	}
}

// �ð� �ʱ�ȭ
// OnInitDialog�Լ����� ȣ��Ǿ� �޷��� �ʱ� ���� �̹����� �ǵ��� �Ѵ�.
void CKalendarDlg::InitTime()
{
	LOG(_T("   : CKalendarDlg::InitTime"));
	CTime t = CTime::GetCurrentTime();
	m_uYear = t.GetYear();
	m_uMonth = t.GetMonth();	
	m_DateCalculator.SetDateAsCurrent();
}

void CKalendarDlg::InitDM()
{
	m_SEQ_DM.InitDMPKFromFile(_T("DM_KALENDAR"));

	m_SEQ_DM.SetDB(_T("ORACLE"));
	m_DataMgr.SetDB(_T("ORACLE"));
	m_SEQ_DM.SetCONNECTION_INFO(_T(""));
	m_DataMgr.SetCONNECTION_INFO(_T(""));

}

// �����ͺ��̽� ���ἳ��
BOOL CKalendarDlg::ConnectDB()
{
	LOG(_T("   : CKalendarDlg::ConnectDB"));
		
 	// arg ��������
 	CString strUserID;
 	CString strConnectionInfo;	
 	INT nOffset;
 
 	nOffset = m_strAuth.Find(_T(" "));
 	strUserID = m_strAuth.Left(nOffset);
//  	strConnectionInfo = m_strAuth.Mid(nOffset+1);
 
 	if(_T("") == strUserID)
 		return FALSE;
//  	if(_T("") == strConnectionInfo)
//  		return FALSE;
 	
 	CString strManageCode = _T("");
 	CString strUserName = _T("");
 	
 	
 	CString strQuery;	
//   	m_DataMgr.SetCONNECTION_INFO(strConnectionInfo);				
 	strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE ID='%s'"), strUserID);
 
	INT ids = 0;


	ids = m_DataMgr.GetOneValueQuery(strQuery, strManageCode);


 	if(0 > ids)
 	{
 		CString strTemp;
 		strTemp.Format(_T("ERROR : [CKalendarDlg::ConnectDB] %s/[%s]"), strQuery, strConnectionInfo);
 		LOG(strTemp);
 		return FALSE;
 	} 
 	strQuery.Format(_T("SELECT NAME FROM MN_USER_TBL WHERE ID = '%s'"), strUserID);

	ids = m_DataMgr.GetOneValueQuery(strQuery, strUserName);

	if(0 > ids)
 	{
 		CString strTemp;
 		strTemp.Format(_T("ERROR : [CKalendarDlg::ConnectDB] %s/[%s]"), strQuery, strConnectionInfo);
 		LOG(strTemp);
 		return FALSE;
 	}
 
  	m_UserInfo.strUser_ID = strUserID;
  	m_strManageCode = strManageCode;
  	m_UserInfo.strUser_Name = strUserName;
//  	m_DBInfo.strConnectionInfo = strConnectionInfo;
 	
	return TRUE;
}

// DB�κ��� �������� �о���δ�.
// -4004 : ���ӵ�������.
// -4005 : ��������� �����߻�. 
// -4006 : ����� �ʹ� ����.
// -4007 : �˻� ��� ����
// -4008 : �˻� �ʵ尡 ��� �ʵ庸�� ����
// -4999 : ���� Oracle�� �������� ����
// 0   : ����
CString CKalendarDlg::ReadHolidayFromDB()
{
	LOG(_T("   : CKalendarDlg::ReadHolidayFromDB"));
	if(FALSE==m_bIsConnectDB)
		return _T("");
	CString strQuery, strValue;
	if ( m_strManageCode.IsEmpty() )
	{
//DEL 		strQuery.Format(_T(
//DEL 			"SELECT DAYS FROM MN_LIB_HOLIDAY_TBL WHERE MANAGE_CODE IS NULL AND YEAR=%d AND MONTH=%d"), m_uYear, m_uMonth);		
	}
	else
	{
		strQuery.Format(_T(
			"SELECT DAYS FROM MN_LIB_HOLIDAY_TBL WHERE MANAGE_CODE='%s' AND YEAR=%d AND MONTH=%d"), m_strManageCode, m_uYear, m_uMonth);
	}	

	INT nResult = 0;
	try
	{

		nResult = m_DataMgr.GetOneValueQuery(strQuery, strValue);
		
	}
	catch(...)
	{		
		CString strTemp;
		strTemp.Format(_T("%d"), nResult);
		strTemp = _T("ERROR(") + strTemp + _T(") : ");
		strTemp = strTemp + strQuery;
		LOG(strTemp);
		return _T("");
	
	}
	
	return strValue;
}

// ����� �� �޹�ưŬ����
// �� �̵��޴� ����
void CKalendarDlg::OnButtonMonthEng() 
{
	// TODO: Add your control notification handler code here
	CMenu  *sMenu,*pMenu;
	pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU_MONTH);	
	sMenu = pMenu->GetSubMenu(0);
	
	// �޴��� ��ư���ʾƷ��� ����
	CRect rect;
	m_btnMonthEng.GetWindowRect(&rect);
	sMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN, rect.left, rect.bottom, this);
	delete pMenu;
}

// ���� ���α׷��� �ߺ�����ǰ� �ִ��� üũ
BOOL CKalendarDlg::ChkRun()
{
	//++2008.08.07 UPDATE BY CJY {{++
	// ���ϸ����� ��������ִٴ� �̾߱�� �̹� �������̶�� �̾߱�
//DEL 	HANDLE phFileMap = OpenFileMapping(
//DEL 		FILE_MAP_READ,
//DEL 		FALSE,
//DEL 		(LPCTSTR)_T("KALENDAR_SELECT_DIVISION_NO")
//DEL 		);
//DEL 	if(INVALID_HANDLE_VALUE == phFileMap)	return TRUE;
//DEL 	else					return FALSE;
//DEL   �����ҽ�
 	INT nExcuteCount = GetExcuteProgramCount(_T("Kalendar.exe"));
 	if(1 < nExcuteCount) return TRUE;
 	else return FALSE;
	//--2008.08.07 UPDATE BY CJY --}}
}

void CKalendarDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	
}

void CKalendarDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
//	HideTabWindow();
	CDialog::OnCancel();
} 


BOOL CKalendarDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{			
			HideTabWindow();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CKalendarDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	WriteConfigFile();
	OnOK();
}

void CKalendarDlg::OnOK()
{
	CTabDialog::OnOK();
}

void CKalendarDlg::InitBG()
{
	LOG(_T("   : CKalendarDlg::InitBG"));
	CDC* pDC = GetDC();
	m_BGDC.CreateCompatibleDC(pDC);		
	m_BGBitmap.LoadBitmap(IDB_BITMAP_BG_MAIN);
	m_BGDC.SelectObject(&m_BGBitmap);
	ReleaseDC(pDC);
}

void CKalendarDlg::ReadConfigFile()
{
	LOG(_T("   : CKalendarDlg::ReadConfigFile"));

	STRUCT_CFGDATA* pCFGDATA = new STRUCT_CFGDATA;
	if(FALSE == CCalFileMgr::ReadConfigFile(pCFGDATA))
	{
		pCFGDATA->nMode = MEMO_VIEW_MODE;
		pCFGDATA->nBorderPos = -1;
		pCFGDATA->nX = -1;
		pCFGDATA->nY = -1;
	}
	
	TRACE(_T("bExcute=%d nBorderPos=%d nMode=%d nX=%d nY=%d\r\n"),
		pCFGDATA->bExcute,
		pCFGDATA->nBorderPos,
		pCFGDATA->nMode,
		pCFGDATA->nX,
		pCFGDATA->nY
		);

	////////////////////////////////////////////////////////////////////
	// ������ġ�� �����¿� ����͵� �ƴϸ� �⺻�� ����
	switch(pCFGDATA->nBorderPos)
	{
	case TAB_BORDER_LEFT:		
	case TAB_BORDER_RIGHT:		
	case TAB_BORDER_TOP:		
	case TAB_BORDER_BOTTOM:
		break;
	default:
		pCFGDATA->nBorderPos = TAB_BORDER_TOP;
		pCFGDATA->nX = 0;
		pCFGDATA->nY = 0;
		break;	
	}
	//
	////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	// ������ ��ǥ ����üũ
	CRect rDlg;
	GetWindowRect(rDlg);
	LONG cxScreen = GetSystemMetrics(SM_CXSCREEN);
	LONG cyScreen = GetSystemMetrics(SM_CYSCREEN);
	
	if(-m_nTabHeight > pCFGDATA->nX ||
		-m_nTabHeight > pCFGDATA->nY ||
		cxScreen + m_nTabHeight < pCFGDATA->nX + rDlg.Width() ||
		cyScreen + m_nTabHeight < pCFGDATA->nY + rDlg.Height()
		)
	{
 		pCFGDATA->nBorderPos = TAB_BORDER_TOP;
		pCFGDATA->nX = 0;
		pCFGDATA->nY = 0;
	}

	//
	////////////////////////////////////////////////////////////////////
	
	m_nBorderPos = pCFGDATA->nBorderPos;
	m_nXpos = pCFGDATA->nX;
	m_nYpos = pCFGDATA->nY;		

	if(DIVISION_NO_VIEW_MODE != pCFGDATA->nMode &&
		MEMO_VIEW_MODE != pCFGDATA->nMode)
	{
		pCFGDATA->nMode = MEMO_VIEW_MODE;
	}

	SetMode(pCFGDATA->nMode, FALSE);

	delete pCFGDATA;
}

void CKalendarDlg::WriteConfigFile()
{
	LOG(_T("   : CKalendarDlg::WriteConfigFile"));
	CRect rec;
	GetWindowRect(rec);

	LONG cxScreen = GetSystemMetrics(SM_CXSCREEN);
	LONG cyScreen = GetSystemMetrics(SM_CYSCREEN);
	
	if(-m_nTabHeight>rec.left)
		rec.left = -m_nTabHeight;
	if(cxScreen<rec.left)
		rec.left = cxScreen;
	if(-m_nTabHeight>rec.top)
		rec.top = -m_nTabHeight;
	if(cyScreen<rec.top)
		rec.top = cyScreen;

	STRUCT_CFGDATA CFGDATA;	
	CFGDATA.nX = rec.left;
	CFGDATA.nY = rec.top;
	CFGDATA.nBorderPos = m_nBorderPos;
	CFGDATA.nMode = GetMode();

	CCalFileMgr::WriteConfigFile(CFGDATA);
}

void CKalendarDlg::SetKalendarToday()
{
	LOG(_T("   : CKalendarDlg::SetKalendarToday"));
	CTime Today = CTime::GetCurrentTime();
	m_uYear = Today.GetYear();
	m_uMonth = Today.GetMonth();
	SetKalendar();
}

void CKalendarDlg::OnNotifyForDlgMemo(CData *pData)
{
	UINT uYear = 0;
	UINT uMonth = 0;
	UINT uDay = 0;
	pData->GetDate(&uYear, &uMonth, &uDay);	
	SaveMemo();

	if(m_pDlgList->IsSameDate(uYear, uMonth))
	{
		m_pDlgList->ModifyItem(
			uYear, uMonth, uDay, pData->GetText());
	}	
	m_pDlgSearchList->ModifyItem(
		uYear, uMonth, uDay, pData->GetText());

}

void CKalendarDlg::OnNotifyForDlgDay(CData *pData)
{
	SaveMemo();

	UINT uYear, uMonth, uDay;
	pData->GetDate(&uYear, &uMonth, &uDay);
	BOOL bIsDay = ChkDay(uYear, uMonth, uDay);
	if(FALSE==bIsDay)
	{
		m_pDlgMemo->ShowWindow(SW_HIDE);
		return;
	}
	m_pDlgMemo->ResetDay();	
	m_pDlgMemo->SetMemo(pData);	
	INT nIndex = m_AlarmTimeMgr.GetIndex(uYear, uMonth, uDay);
	if(-1!=nIndex)
	{
		CTime AlarmTime = m_AlarmTimeMgr.GetTimeAt(nIndex);
		m_pDlgMemo->SetAlarmTime(AlarmTime);
	}
	else
		m_pDlgMemo->ResetAlarmTime();

	m_pDlgMemo->MoveMemo();		
	m_pDlgMemo->ShowWindow(SW_SHOW);	
}


// �Լ��̸�: CKalendarDlg::OnNotifyForDlgListView
// �Լ�����: 
// �޸��� �����͸� ����ڰ� �󼼺��⳪ �˻�â���� ������ ��¥�� �����ͷ� ��ü
// CDlgListView������ �����Ϳ� �ؽ�Ʈ�� �Ѿ���� �ʱ� ������
// �޸� ���� �ؽ�Ʈ�� �����ϱ����� ���� �޷��̳� ���Ϸκ���
// �ؽ�Ʈ�� �޾ƾƾ� �Ѵ�.
// ��ȯ  ��: void 
// ����[CData *pData] : CDlgListView���� �Ѿ�� ������
void CKalendarDlg::OnNotifyForDlgListView(CData *pData)
{
	m_pDlgMemo->ShowWindow(SW_HIDE);
	SaveMemo();

	//////////////////////////////////////////////////////
	// ������� ������ ����
	UINT uYear = 0;
	UINT uMonth = 0;
	UINT uDay = 0;
	pData->GetDate(&uYear, &uMonth, &uDay);	
	int nIndex = GetDlgDayIndex(uYear, uMonth, uDay);
	if(-1!=nIndex)
		m_Days[nIndex]->GetData(pData);
	else
		CCalFileMgr::ReadMemoFile(pData);	
	// ������� ������ ����
	//////////////////////////////////////////////////////
	m_pDlgMemo->ResetDay();	
	m_pDlgMemo->SetData(pData);
	m_pDlgMemo->MoveMemo();
	m_pDlgMemo->SetFocus();
	m_pDlgMemo->ShowWindow(SW_SHOW);
}


// �Լ��̸�: CKalendarDlg::SaveMemo
// �Լ�����: �޸��� ���
// ��ȯ  ��: BOOL 
BOOL CKalendarDlg::SaveMemo()
{	
	CData* pData = new CData;
	m_pDlgMemo->GetData(pData);

	//////////////////////////////////////////////////////
	// ������� �޸�����
	UINT uYear = 0, uMonth = 0, uDay = 0;
	pData->GetDate(&uYear, &uMonth, &uDay);
	INT nIndex = GetDlgDayIndex(uYear, uMonth, uDay);
	if(-1!=nIndex)	// ȭ�鿡 ǥ�õǴ� �޷¿� �ش� ��¥�� ������
		m_Days[nIndex]->SetText(pData->GetText());	
	else
		CCalFileMgr::WriteMemoFile(pData);
	// ������� �޸�����
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	// ������� �˶�����
	UINT uAlarmHour = 0, uAlarmMinute = 0;
	m_pDlgMemo->GetAlarmTime(&uAlarmHour,&uAlarmMinute);
	BOOL bUseAlarm = pData->GetUseAlarm();
	if(TRUE==bUseAlarm)
		m_AlarmTimeMgr.SetAlarm(uYear, uMonth, uDay, uAlarmHour, uAlarmMinute);
	else
	{
		INT nIndex = m_AlarmTimeMgr.GetIndex(uYear, uMonth, uDay);
		if(-1!=nIndex)
			m_AlarmTimeMgr.DeleteAlarmAt(nIndex);
	}
	// ������� �˶�����
	//////////////////////////////////////////////////////
	delete pData;
	return TRUE;
}


// �Լ��̸�: CKalendarDlg::OnTimerForAlarmTime
// �Լ�����: 
// �˶��Ŵ����� �ִ� �ð��� ����ð� ���ų� �����̸� �˶��� ���
// �˶��� ���� �ش�˶����� ����
// ��ȯ  ��: void 
void CKalendarDlg::OnTimerForAlarmTime()
{
	BOOL bAlarmChange = FALSE;
	INT nMax = m_AlarmTimeMgr.GetSize();		
	CTime AlarmTime;
	CTime t = CTime::GetCurrentTime();
	for(int nCount=0;nCount<nMax;nCount++)
	{
		// �ð�üũ
		AlarmTime = m_AlarmTimeMgr.GetTimeAt(nCount);
		int uYear = AlarmTime.GetYear();
		int uMonth = AlarmTime.GetMonth();
		int uDay = AlarmTime.GetDay();
		int uHour = AlarmTime.GetHour();
		int uMinute = AlarmTime.GetMinute();	
		
		BOOL bAlarm = TRUE;
		if(uYear>t.GetYear())
			bAlarm = FALSE;
		if(uMonth>t.GetMonth())
			bAlarm = FALSE;
		if(uDay>t.GetDay())
			bAlarm = FALSE;
		if(uHour>t.GetHour())
			bAlarm = FALSE;
		if(uMinute>t.GetMinute())
			bAlarm = FALSE;
		
		if(TRUE==bAlarm)
		{								
			CString stText = _T("");
			int nIndex = GetDlgDayIndex(uYear, uMonth, uDay);
			if(-1!=nIndex)
				stText = m_Days[nIndex]->GetText();
			else
				CCalFileMgr::ReadMemoFile(uYear, uMonth, uDay, &stText);
			
			m_pDlgAlarm->SetData(AlarmTime, stText);
			m_pDlgAlarm->Invalidate(TRUE);
			m_pDlgAlarm->ShowWindow(SW_SHOW);
			m_AlarmTimeMgr.DeleteAlarmAt(nCount);			
			bAlarmChange = TRUE;
			break;			
		}		
	}
	if(TRUE==bAlarmChange)
		m_AlarmTimeMgr.WriteAlarmFile();
}

void CKalendarDlg::SetTransparency(int percent)
{
	SLWA pSetLayeredWindowAttributes = NULL;  // �Լ������� ����, �ʱ�ȭ.
	HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL")); // �ν��Ͻ� ����.
	pSetLayeredWindowAttributes=(SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
    //�Լ������� ����.
	HWND hwnd = this->m_hWnd; //���̾�α��� �ڵ� ����.
	SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (255 * percent) / 100, LWA_ALPHA);
}

LONG CKalendarDlg::OnMyMethod(WPARAM wParam,LPARAM lParam)
{
	int nPercent = (int)lParam;
	SetTransparency(nPercent);

	return 0;
}

// �Լ��̸�: CKalendarDlg::OnNotifyForDlgAlarm
// �Լ�����: 
// �޸��� ��¥�� �˶��� ��¥�� ���ٸ�
// �˶��� �︱ ��, �޸��� �˶������� �ʱ�ȭ
// ��ȯ  ��: void 
// ����[CData *pData] : �˶����̾�α׿��� ������ ������
void CKalendarDlg::OnNotifyForDlgAlarm(CData *pData)
{
	UINT uAlarmYear, uAlarmMonth, uAlarmDay;
	pData->GetDate(&uAlarmYear, &uAlarmMonth, &uAlarmDay);

	UINT uMemoYear, uMemoMonth, uMemoDay;
	m_pDlgMemo->GetDate(&uMemoYear, &uMemoMonth, &uMemoDay);
	
	BOOL bSameDate = TRUE;
	if(uAlarmYear!=uMemoYear)
		bSameDate = FALSE;
	if(uAlarmMonth!=uMemoMonth)
		bSameDate = FALSE;
	if(uAlarmDay!=uMemoDay)
		bSameDate = FALSE;

	if(TRUE==bSameDate)
	{
		m_pDlgMemo->ResetAlarmTime();
	}
}	

void CKalendarDlg::SetWeekend()
{
	LOG(_T("{In : CKalendarDlg::SetWeekend"));
	////////////////////////////////////////////////////////////////////////
	{// ���⼭���� �����/�Ͽ��� ����
		
		INT nSaturdayIndex = -1;
		INT nSundayIndex = -1;

		for(INT nCount=0; nCount < MAXDAY; nCount+=7)
		{
			nSaturdayIndex = nCount + 6;
			nSundayIndex = nCount;

			if(MAXDAY > nSaturdayIndex) m_Days[nSaturdayIndex]->SetSaturday();
			if(MAXDAY > nSaturdayIndex) m_SEQDays[nSaturdayIndex]->SetSaturday();
			if(MAXDAY > nSundayIndex) m_Days[nSundayIndex]->SetSunday();
			if(MAXDAY > nSundayIndex) m_SEQDays[nSundayIndex]->SetSunday();
		}


	}// ������� �����/�Ͽ��� ����
	////////////////////////////////////////////////////////////////////////
	LOG(_T("}Out: CKalendarDlg::SetWeekend"));
}
//++2008.07.08 DEL BY CJY {{++
//DEL 
//DEL // ����޷� �̵�
//DEL void CKalendarDlg::OnMonth() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	CTime t = CTime::GetCurrentTime();
//DEL 	m_uMonth = t.GetMonth();
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnMonth1() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	m_uMonth = 1;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnMonth2() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	m_uMonth = 2;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnMonth3() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	m_uMonth = 3;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnMonth4() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	m_uMonth = 4;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnMonth5() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	m_uMonth = 5;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnMonth6() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	m_uMonth = 6;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnMonth7() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	m_uMonth = 7;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnMonth8() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	m_uMonth = 8;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnMonth9() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	m_uMonth = 9;
//DEL 	SetKalendar();
//DEL }
//DEL void CKalendarDlg::OnMonth10() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	m_uMonth = 10;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnMonth11() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	m_uMonth = 11;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnMonth12() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	m_uMonth = 12;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL 
//DEL 
//DEL void CKalendarDlg::OnYearPrev5() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	CTime t = CTime::GetCurrentTime();
//DEL 	m_uYear = t.GetYear() -  5;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnYearPrev4() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	CTime t = CTime::GetCurrentTime();
//DEL 	m_uYear = t.GetYear() -  4;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnYearPrev3() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	CTime t = CTime::GetCurrentTime();
//DEL 	m_uYear = t.GetYear() -  3;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnYearPrev2() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	CTime t = CTime::GetCurrentTime();
//DEL 	m_uYear = t.GetYear() -  2;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnYearPrev1() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	CTime t = CTime::GetCurrentTime();
//DEL 	m_uYear = t.GetYear() -  1;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnYear() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	CTime t = CTime::GetCurrentTime();
//DEL 	m_uYear = t.GetYear();
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnYearNext1() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	CTime t = CTime::GetCurrentTime();
//DEL 	m_uYear = t.GetYear() + 1;
//DEL 	SetKalendar();
//DEL }
//DEL void CKalendarDlg::OnYearNext2() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	CTime t = CTime::GetCurrentTime();
//DEL 	m_uYear = t.GetYear() + 2;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnYearNext3() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	CTime t = CTime::GetCurrentTime();
//DEL 	m_uYear = t.GetYear() + 3;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnYearNext4() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	CTime t = CTime::GetCurrentTime();
//DEL 	m_uYear = t.GetYear() + 4;
//DEL 	SetKalendar();
//DEL }
//DEL 
//DEL void CKalendarDlg::OnYearNext5() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	CTime t = CTime::GetCurrentTime();
//DEL 	m_uYear = t.GetYear() + 5;
//DEL 	SetKalendar();
//DEL }
//--2008.07.08 DEL BY CJY --}}


// �Լ��̸�: CKalendarDlg::SetMode
// �Լ�����: �޸𺸱� ���� �������� ��带 ��ȯ�Ѵ�
// ��ȯ  ��: INT 
// ����[INT nNewMode] : 
INT CKalendarDlg::SetMode(INT nNewMode, BOOL bReDraw)
{	
//DEL 	if(GetMode() == nNewMode) return -1;
	
	if(DIVISION_NO_VIEW_MODE != nNewMode &&
		MEMO_VIEW_MODE != nNewMode)
		return -1;

	m_nMode = nNewMode;
	if(FALSE == bReDraw) return 0;
	
	////////////////////////////////////////////////////////////////////
	// ���⼭���� ȭ�� ��ȯ


	// �÷��� ����
	UINT uFlag_SEQDay = SWP_NOZORDER|SWP_NOSIZE|SWP_NOMOVE;
	UINT uFlag_DlgDay = SWP_NOZORDER|SWP_NOSIZE|SWP_NOMOVE;
	
	if(DIVISION_NO_VIEW_MODE == nNewMode)
	{	
		uFlag_DlgDay = uFlag_SEQDay|SWP_HIDEWINDOW;
		uFlag_SEQDay = uFlag_SEQDay|SWP_SHOWWINDOW;
		
		m_btnMemoView.EnableWindow(TRUE);
		m_btnDivisionNoView.EnableWindow(FALSE);	
	}
	else if(MEMO_VIEW_MODE == nNewMode)
	{
		uFlag_DlgDay = uFlag_SEQDay|SWP_SHOWWINDOW;
		uFlag_SEQDay = uFlag_SEQDay|SWP_HIDEWINDOW;

		m_btnMemoView.EnableWindow(FALSE);
		m_btnDivisionNoView.EnableWindow(TRUE);	
	}
	else -2;	

	// 
	HDWP hdwp = ::BeginDeferWindowPos(MAXDAY*2);
	
	CRect rect;
	for(INT nCount=0; nCount < MAXDAY; nCount++)
	{	
		if(TRUE == m_SEQDays[nCount]->IsDay())
		{
			m_SEQDays[nCount]->GetWindowRect(rect);
			ScreenToClient(rect);
			
			::DeferWindowPos(
				hdwp, 
				m_SEQDays[nCount]->GetSafeHwnd(),
				HWND_TOP,                
				rect.left,
				rect.top,
				rect.Width(),
				rect.Height(),
				uFlag_SEQDay
				);
		}
	
		::DeferWindowPos(
			hdwp, 
			m_Days[nCount]->GetSafeHwnd(),
			HWND_TOP,                
			rect.left,
			rect.top,
			rect.Width(),
			rect.Height(),
			uFlag_DlgDay
			);			
	}

	EndDeferWindowPos(hdwp);	

	// ������� ȭ�� ��ȯ
	////////////////////////////////////////////////////////////////////
	
	return 0;
}

INT CKalendarDlg::GetMode() const
{
	return m_nMode;
}

void CKalendarDlg::InitDlgSEQ()
{
	LOG(_T("   : CKalendarDlg::InitDlgDay"));
	CRect rec;	
	rec.left = LEFTSPACE;
	rec.right = rec.left + DAYWIDTH;
	rec.top = TITLESPACE;
	rec.bottom = rec.top + DAYHEIGHT;

	for(int i=0;i<MAXDAY;i++)
	{
		m_SEQDays[i] = new CDlgSEQ;	
		m_SEQDays[i]->Create(CDlgSEQ::IDD, this);
//MACRODEL		m_Days[i]->m_pDlgMemo = m_pDlgMemo;
		MoveSEQDay(i);
//DEL 		m_SEQDays[i]->ShowWindow(SW_SHOW);
	}
}

//DEL INT CKalendarDlg::Set_KalendarForDlgDays()
//DEL {
//DEL 	// ��¥ǥ�ø� ������ ��Ʈ�ѳѹ�	/ GetDayofWeek�� ���ϰ�: �Ͽ����� 1, ȭ������ 2...
//DEL 	int StartIdNum = m_DateCalculator.GetDayOfWeek(m_uYear, m_uMonth, 1) - 1;	
//DEL 	// 1�Ϻ��� �ش� ���� ��¥����ŭ�� ǥ��(���� 4���� 30�ϱ����� ǥ��)
//DEL 	for(int i=0;i<m_DateCalculator.GetDaysOfMonth(m_uYear, m_uMonth);i++)
//DEL 	{		
//DEL 		m_Days[StartIdNum+i]->SetDay(m_uYear, m_uMonth, i+1);		
//DEL 	}	
//DEL 	CTime t= CTime::GetCurrentTime();
//DEL 	if(t.GetYear()==(int)m_uYear&&t.GetMonth()==(int)m_uMonth)
//DEL 	{
//DEL 		m_Days[GetDlgDayIndex(m_uYear, m_uMonth, t.GetDay())]
//DEL 			->SetToday();
//DEL 	}
//DEL 	
//DEL 	return 0;
//DEL }

void CKalendarDlg::OnButtonSeqMode() 
{
	// TODO: Add your control notification handler code here
	SetMode(DIVISION_NO_VIEW_MODE);
}

void CKalendarDlg::OnButtonMemoMode() 
{
	// TODO: Add your control notification handler code here
	SetMode(MEMO_VIEW_MODE);	
}

HWND CKalendarDlg::GetKolasHandle()
{		
	HWND hWnd;	
	CCalFileMgr::ReadKolasHandleFile(_T(""), &hWnd);

	return hWnd;
}


// �Լ��̸�: CKalendarDlg::GetExcuteProgramCount
// �Լ�����: 
// ��ȯ  ��: BOOL 
// ����[CString strProgramName] : 
// ����[INT nCheckCount] : 
INT CKalendarDlg::GetExcuteProgramCount(CString strProgramName)
{
	LOG(_T("   : CKalendarDlg::IsExcuteProgram"));
	HANDLE         hProcessSnap = NULL; 
	BOOL           bRet      = FALSE; 
	PROCESSENTRY32 pe32      = {0}; 
	// KOL.UDF.022
	//INT nCount;
	INT nCount = 0;
//DEL 	BOOL		bFind	= FALSE;

	
	//  Take a snapshot of all processes in the system. 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	
	if (hProcessSnap != INVALID_HANDLE_VALUE) 
	{		
		//  Fill in the size of the structure before using it. 
		pe32.dwSize = sizeof(PROCESSENTRY32); 
		
		//  Walk the snapshot of the processes, and for each process, 
		//  display information.     
		
//DEL 		INT nCount;
		nCount = 0;
		if (Process32First(hProcessSnap, &pe32)) 
		{ 
			do 
			{ 					
				if(_tcsicmp(pe32.szExeFile, strProgramName)==0)
				{					
					nCount++;
//DEL 					if ( nCount >= nCheckCount )
//DEL 					{
//DEL 						bFind = TRUE;
//DEL 						HANDLE hProcess = NULL;
//DEL 						break;
//DEL 					}
				} 				
			} 
			while (Process32Next(hProcessSnap, &pe32)); 
		}
		
		// Do not forget to clean up the snapshot object. 
		CloseHandle (hProcessSnap); 
	}

	return nCount;
//DEL 	return bFind;	
}

void CKalendarDlg::OnButtonToday() 
{
	// TODO: Add your control notification handler code here
	SetToday();
}

void CKalendarDlg::SetToday()
{
	CTime Today = CTime::GetCurrentTime();
	m_uYear = Today.GetYear();
	m_uMonth = Today.GetMonth();
	Refresh_SEQ_DM();

	SetKalendar();
}


// �Լ��̸�: CKalendarDlg::OnNotifyForDlgSEQ
// �Լ�����: 
// ����ڰ� ������¥ ���̾�α׿��� ������ Ŭ���� �̰����� �̺�Ʈ�� �Ѿ�´�.
// ���� KOLASIII�� ���������� �Ǵ��ϰ�, �ڵ������� �о� KOLASIII�� �޽����� ������.
// ��ȯ  ��: INT 
// ����[CData *pData] : 
INT CKalendarDlg::OnNotifyForDlgSEQ(CData *pData)
{	
	HideTabWindow();

	// �Ѿ�� ������ ���� Ȯ��
	CString strText = pData->GetText();
	if(_T("") == strText) return -1;

	INT nSplitIndex = strText.Find(_T(":"));
	if(0 > nSplitIndex) return -2;

	DWORD dwStart = GetTickCount();

	CString strSendText = _T("");
	strSendText.Format(_T("%d_"), m_uYear);
	strSendText = strSendText + strText;
	const INT MAX_SIZE = 10;
	if(MAX_SIZE < m_aSend_DivisionNo_To_KolasIII.GetSize()) return -1;
	m_aSend_DivisionNo_To_KolasIII.Add(strSendText);
	
	// ���� ����϶����� ����
	// ����� �������� ��� KOLASIII���� Ȯ�� �޽����� ���� ���޳�
	// ���� �����͸� �����Ѵ�
	if(1 == m_aSend_DivisionNo_To_KolasIII.GetSize()) 
	{		
		Send_FirstDivisonNo();
	}

	return 0;
}

INT CKalendarDlg::Refresh_SEQ_DM()
{
	if(FALSE == m_bIsConnectDB)
	{
		return -1;
	}
 	
// 	m_SEQ_DM.InitDMPKFromFile(_T("DM_KALENDAR"));
// 	m_SEQ_DM.SetCONNECTION_INFO(m_DBInfo.strConnectionInfo);

	CString strRefreshOption = _T("");
	//  AND WORK_CODE = '1'
	strRefreshOption.Format(
		_T("MANAGE_CODE = '%s' AND ACQ_YEAR = '%d' AND SEQ_NO_CREATE_WORKER = '%s'"),
		m_strManageCode,
		m_uYear,
		m_UserInfo.strUser_Name
		);

	m_SEQ_DM.RefreshDataManager(strRefreshOption);	

	return 0;
}

INT CKalendarDlg::Add_SEQData()
{
	
	CString strGetAlias[3] =
	{ 		
		_T("���Դܰ�����"),
		_T("�˼��ܰ�����"),
		_T("��ϴܰ�����")
	};
	CString strAddSEQStatus[3] =
	{
		_T("����"),
		_T("�˼�"),
		_T("���")
	};
	CString strSEQCaptionAlias[3] = 
	{
		_T("����å��"),
		_T("�˼�å��"),
		_T("���å��")
	};

	INT nMAX = m_SEQ_DM.GetRowCount();
	for(INT nRow=0; nRow < nMAX; nRow++)
	{
		CString strSEQNo = m_SEQ_DM.GetCellData(_T("������ȣ"), nRow);
		CString strWork = m_SEQ_DM.GetCellData(_T("��������"), nRow);
	 	CString strWorkCode = m_SEQ_DM.GetCellData(_T("��������_�ڵ�"), nRow);
		
		// ���
 		CString strRemark = m_SEQ_DM.GetCellData(_T("���"), nRow);
		// ����
		CString	strTooltip = 
			strWork +
			_T(" / ") +
			strSEQNo;
		if(_T("") != strRemark) 
		{
			strTooltip = 
			strTooltip +
			_T(" / ") + 
			strRemark;
		}

		for(INT nAliasIndex=0; nAliasIndex < 3; nAliasIndex++)
		{
			CString strCaption = strSEQNo;
			CString strTemp = m_SEQ_DM.GetCellData(strGetAlias[nAliasIndex], nRow);
			CString strData = strWorkCode + _T("/") + strSEQNo;

			strTemp += _T("/");
			INT nPos = 0;
			INT nStart = 0;
						
			UINT nDate[3];
			enum { YEAR=0, MONTH=1, DAY=2 };
 			for(UINT nFindIndex = 0; nFindIndex < 3; nFindIndex++)
 			{
 				nPos = strTemp.Find(_T("/"), nStart);
  				nDate[nFindIndex] = _ttoi(strTemp.Mid(nStart, nPos - nStart));
 				nStart = nPos+1;
 			}
 			if(m_uYear == nDate[YEAR] && m_uMonth == nDate[MONTH])
 			{
 				// �ش糯¥�� �������̾�α� �ε����� ����
 				INT nDayIndex = GetDlgDayIndex(m_uYear, m_uMonth, nDate[DAY]);
 
 				CString strBookNo = m_SEQ_DM.GetCellData(strSEQCaptionAlias[nAliasIndex], nRow);
 			 	strCaption = strCaption + _T("(") + strBookNo + _T(")");
 				
 				m_SEQDays[nDayIndex]->AddData(
 					strAddSEQStatus[nAliasIndex],	// �������� ����(����, �˼�, �̳�, ���)
 					strData,			// ������(Ŭ���� �ڶ󽺿� ���� ����)
 					strCaption,			// ĸ��(ȭ�鿡 ������ ����)
 					strTooltip			// ��������
 					);
 
 				if(_T("�˼��ܰ�����") == strGetAlias[nAliasIndex])
 				{
 					//++2008.07.23 ADD BY CJY {{++
 					INT nIndex = strCaption.Find(_T("("));
 					strCaption = strCaption.Left(nIndex);
 					CString strBookNo = m_SEQ_DM.GetCellData(_T("�̳�å��"), nRow);
 					strCaption = strCaption + _T("(") + strBookNo + _T(")");
 					m_SEQDays[nDayIndex]->AddData(_T("�̳�"), strData, strCaption, strTooltip);
 					//--2008.07.23 ADD BY CJY --}}
 				}
 				
 			}
		}
	}

	return 0;
}

BOOL CKalendarDlg::DestroyWindow() 
{

	if ( m_ctlSlider != NULL )
	{
		delete m_ctlSlider;
		m_ctlSlider = NULL;
	}
	m_bIsConnectDB = FALSE;

	if(NULL != m_pDlgAlarm && NULL != m_pDlgAlarm->GetSafeHwnd())		m_pDlgAlarm->DestroyWindow();
	if(NULL != m_pDlgList && NULL != m_pDlgList->GetSafeHwnd())		m_pDlgList->DestroyWindow();
	if(NULL != m_pDlgMemo && NULL != m_pDlgMemo->GetSafeHwnd())		m_pDlgMemo->DestroyWindow();
	if(NULL != m_pDlgSearchList && NULL != m_pDlgSearchList->GetSafeHwnd())	m_pDlgSearchList->DestroyWindow();

	 if(NULL != m_pWndKolasIII)
	{
		m_pWndKolasIII->Detach();
		delete m_pWndKolasIII;
		m_pWndKolasIII = NULL;
	}

	return CTabDialog::DestroyWindow();
}

INT CKalendarDlg::Open_FileMapping()
{
	////////////////////////////////////////////////////////////////////////////
	// ���⼭���� �����޸� ����
	// WPARAM�̳� LPARAM���� �����͸� �ѱ��� �ʴ� ������
	// KALENDAR�� KOLASIII�� ���� �ٸ� ���μ����̱� ������
	// ���ڿ��� �������ּҴ� �ǹ̰� ���⶧��
	//++2008.07.15 ADD BY CJY {{++
	m_phFileMap = CreateFileMapping(
		(HANDLE)0xFFFFFFFF,
		NULL,
		PAGE_READWRITE,
		0,
		/* sizeof(DWORD)*/
		sizeof(TCHAR),
		/*pszShareName*/
		(LPCTSTR)_T("KALENDAR_SELECT_DIVISION_NO")
		);
	if(INVALID_HANDLE_VALUE == m_phFileMap) return -1;

	// Map view
	m_lpMapView = MapViewOfFile(
		m_phFileMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0
		);
	if(NULL == m_lpMapView)	return -2;
	//--2008.07.15 ADD BY CJY --}}
	// ������� �����޸� ����
	////////////////////////////////////////////////////////////////////////////

	return 0;
}

INT CKalendarDlg::Close_FileMapping()
{	
	if(NULL != m_phFileMap && INVALID_HANDLE_VALUE != m_phFileMap)
	{
		// ������ �����޸� ����
		UnmapViewOfFile(m_lpMapView);
		CloseHandle(m_phFileMap);
	}

	return 0;
}

long CKalendarDlg::OnProcessRequest(WPARAM wParam, LPARAM lParam)
{
	CString* strMsg = (CString*)lParam;
	if(_T("AUTO_HIDE:FALSE") == *strMsg)
	{
		SetAutoHide(FALSE);
	}
	else if(_T("AUTO_HIDE:TRUE") == *strMsg)
	{
		SetAutoHide(TRUE);
	}
	return 0;
}

long CKalendarDlg::Send_NextDivisonNo(WPARAM wParam, LPARAM lParam)
{	
	INT nSize = m_aSend_DivisionNo_To_KolasIII.GetSize();
	if(1 > nSize)
	{		
		return -1;
	}
	else if(1 == nSize)
	{
 		TRACE(_T("Send All Data\r\n"));
		m_aSend_DivisionNo_To_KolasIII.RemoveAt(0);	
		return -1;
	}
	
	m_aSend_DivisionNo_To_KolasIII.RemoveAt(0);

	INT ids;
#ifdef _ED
	ids = GetExcuteProgramCount(_T("ExpressDesk.exe"));
#else
	ids = GetExcuteProgramCount(_T("KOLASIII.exe"));
#endif
	if(1 < ids)
	{		
		HWND hWnd = GetKolasHandle();		
		
		if(hWnd != m_pWndKolasIII->m_hWnd)
		{
			m_pWndKolasIII->Detach();
			m_pWndKolasIII->Attach(hWnd);
		}
	}
	else return -2;
	
	CString strSendDivNo = m_aSend_DivisionNo_To_KolasIII.GetAt(0);
	wcscpy((TCHAR*)m_lpMapView, strSendDivNo);
	

	// KOLASIII�� ������ ���õǾ����� �˶�
	m_pWndKolasIII->PostMessage(1113, 0, 0);
	TRACE(_T("Send Data %s\r\n"), strSendDivNo);
	return 0;
}

long CKalendarDlg::Send_FirstDivisonNo()
{
	if(0 >= m_aSend_DivisionNo_To_KolasIII.GetSize()) return -1;
	
	INT ids;
#ifdef _ED
	ids = GetExcuteProgramCount(_T("ExpressDesk.exe"));
#else
	ids = GetExcuteProgramCount(_T("KOLASIII.exe"));
#endif
	if(0 < ids)
	{
		HWND hWnd = GetKolasHandle();		
		
		if(hWnd != m_pWndKolasIII->m_hWnd)
		{
			m_pWndKolasIII->Detach();
			m_pWndKolasIII->Attach(hWnd);
		}
	}
	else return -2;
	
	CString strSendDivNo = m_aSend_DivisionNo_To_KolasIII.GetAt(0);
	wcscpy((TCHAR*)m_lpMapView, strSendDivNo);
	
	m_pWndKolasIII->PostMessage(1113, 0, 0);
 	TRACE(_T("Send First Data %s\r\n"), strSendDivNo);
	return 0;
}


CString CKalendarDlg::GetAuth()
{
	return m_strAuth;
}

LRESULT CKalendarDlg::ShowTabWindow(WPARAM wParam, LPARAM lParam)
{
	CTabDialog::ShowTabWindow();

	return NULL;
}

//++2008.08.07 ADD BY CJY {{++
// ���α׷� ����ó��
// ���α׷� ������ �߻��ϸ� ������ �ٽ� �����Ѵ�
long __stdcall ExceptionCatch(PEXCEPTION_POINTERS pExceptionInfo)
{
	CKalendarDlg* pKalendar = (CKalendarDlg*)AfxGetApp()->GetMainWnd();
	pKalendar->ShowWindow(SW_HIDE);
	pKalendar->SendMessage(WM_CLOSE, 0, 0);		

//DEL 	CString strCmd;
//DEL 	strCmd.Format(_T("Kalendar.exe %s"), pKalendar->GetAuth());
//DEL 	
//DEL 	int len; 
//DEL 	len = WideCharToMultiByte(CP_ACP, 0, strCmd, -1, NULL, 0, NULL,NULL);
//DEL 	char* ctmp = new char[len]; 
//DEL 	// ���� ��ȯ
//DEL 	WideCharToMultiByte(CP_ACP, 0, strCmd, -1, ctmp, len, NULL, NULL);			
//DEL 	::WinExec(ctmp, SW_SHOW);
//DEL 	delete []ctmp;				

	return TRUE;
}
//--2008.08.07 ADD BY CJY --}}

void CKalendarDlg::MoveDropDownButton()
{
	const int DROPDOWN_BTN_HEIGHT = 12;
	const int DROPDOWN_BTN_WIDTH = 23;

	CRect rDropdown;
	if(TAB_BORDER_TOP == m_nBorderPos || TAB_BORDER_BOTTOM == m_nBorderPos)
	{	
		rDropdown.left	= 27;
		rDropdown.right	= rDropdown.left + DROPDOWN_BTN_WIDTH;
		rDropdown.top		= m_nYDC + (m_nTabHeight - DROPDOWN_BTN_HEIGHT)  / 2 - m_nTabRound;
		rDropdown.bottom	= rDropdown.top + DROPDOWN_BTN_HEIGHT;

		if(TAB_BORDER_TOP == m_nBorderPos) m_btnDropDown_T.MoveWindow(rDropdown);
		else m_btnDropDown_B.MoveWindow(rDropdown);
	}
	else if(TAB_BORDER_LEFT == m_nBorderPos || TAB_BORDER_RIGHT == m_nBorderPos)
	{


		if(1000 < m_nXDC) AfxMessageBox(_T("1000"));
 		rDropdown.left	= m_nXDC + (m_nTabHeight - 13)  / 2 - m_nTabRound+1;
 		rDropdown.right	= rDropdown.left + DROPDOWN_BTN_WIDTH;
 		rDropdown.bottom	= 753/*rRect.Height()*/ - m_nTabHeight - 94;
 		rDropdown.top 	= rDropdown.bottom - DROPDOWN_BTN_WIDTH;

 		if(TAB_BORDER_LEFT == m_nBorderPos) m_btnDropDown_L.MoveWindow(rDropdown);
		else m_btnDropDown_R.MoveWindow(rDropdown);		
	}
}

void CKalendarDlg::OnbtnRefreshSEQ(WPARAM wParam, LPARAM lParam)
{
	Refresh_SEQ_DM();
	SetKalendar();
}

void CKalendarDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		// TODO: Add your message handler code here and/or call default
	CDialog::OnRButtonDown(nFlags, point);

	if(DIVISION_NO_VIEW_MODE == GetMode())
	{
		CMenu  *sMenu,*pMenu;
		pMenu = new CMenu;
		pMenu->LoadMenu(IDM_RMOUSE_CLICK);	
		sMenu = pMenu->GetSubMenu(0);
		
		CPoint pos;
		GetCursorPos(&pos);

		sMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN, pos.x, pos.y, this);
		delete pMenu;
	}
}

LRESULT CKalendarDlg::OnToggleKalendar(WPARAM wParam, LPARAM lParam)
{
	if(m_bHiddenStatus)
	{
		CTabDialog::ShowTabWindow();
	}
	else
	{
		CTabDialog::HideTabWindow();
	}

	return 0;
}