// ECReports_LoanHistoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECReports_LoanHistory.h"
#include "ECReports_LoanHistoryDlg.h"

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
// CECReports_LoanHistoryDlg dialog

CECReports_LoanHistoryDlg::CECReports_LoanHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CECReports_LoanHistoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CECReports_LoanHistoryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	CoInitialize(NULL);
}

VOID CECReports_LoanHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CECReports_LoanHistoryDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CECReports_LoanHistoryDlg, CDialog)
	//{{AFX_MSG_MAP(CECReports_LoanHistoryDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECReports_LoanHistoryDlg message handlers

BOOL CECReports_LoanHistoryDlg::OnInitDialog()
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

	/// Init
	SetRunMode();

	DM.SetCONNECTION_INFO( GetCONNECTION_INFO() );

	
	RunProc();
	EndDialog(0);
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CECReports_LoanHistoryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CECReports_LoanHistoryDlg::OnPaint() 
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
HCURSOR CECReports_LoanHistoryDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CECReports_LoanHistoryDlg::OnButton1() 
{
//	RunProc();
//	AfxMessageBox( GetLibCode(_T("MA")) );
//	EndDialog(0);
}

CString CECReports_LoanHistoryDlg::GetCONNECTION_INFO()
{
	CStdioFile ConnFile;
	CString    strLine;
	
	if( ConnFile.Open( _T("..\\cfg\\ConnectionInfo.txt"), CFile::modeRead | CFile::typeBinary ) )
	{
		TCHAR cUni;
		ConnFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			ConnFile.SeekToBegin();
		}
		
		while(true)
		{
			ConnFile.ReadString(strLine);
			
			if(strLine.GetLength() == 0) continue;
			if(strLine.GetAt(0) == '#')  continue;
			if(strLine.GetAt(0) == '.')  continue;

			ConnFile.Close();

			if( strLine.GetLength() )
			{	
				return strLine;
			}
			else
			{
				break;
			}
		}
	}

	return _T("kolas2@k2up/kolas2#[]");
}

//////////////////////////////////////////////////////////////////////
/// ���������� ����Ͽ� ������ �ڵ带 �����´�.
/// 
CString CECReports_LoanHistoryDlg::GetLibCode(CString strManageCode)
{
	INT		ids;
	CString strQuery;
	CString strResult;
	CESL_DataMgr TmpDM;

	TmpDM.SetCONNECTION_INFO( DM.CONNECTION_INFO );

	strQuery.Format( _T("SELECT MAX(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE='%s'"), strManageCode );

	ids = TmpDM.GetOneValueQuery( strQuery, strResult );
	if( 0 > ids )
	{
		///// ���� �α�
		CString strLogFile,strFile,strMsg;
		CTime t = CTime::GetCurrentTime();
		strLogFile.Format(_T("%04d%02d%02d"),  t.GetYear(), t.GetMonth(), t.GetDay());
		strFile.Format(_T("..\\log\\FAIL_%s.TXT"),strLogFile);
		strMsg.Format( _T("[DB CONNECTION ERROR !!!!]\r\n"));
		WriteLog(strFile,strMsg);
		return _T("");
	}
	
	strResult.TrimLeft();strResult.TrimRight();
	
	if( !strResult.GetLength() )
	{
		///// ���� �α�
		CString strLogFile,strFile,strMsg;
		CTime t = CTime::GetCurrentTime();
		strLogFile.Format(_T("%04d%02d%02d"),  t.GetYear(), t.GetMonth(), t.GetDay());
		strFile.Format(_T("..\\log\\FAIL_%s.TXT"),strLogFile);
		strMsg.Format( _T("[CAN NOT FIND LIB_CODE ERROR !!!!] %s\r\n"), strManageCode);
		WriteLog(strFile,strMsg);
		return _T("");
	}

	return strResult;
}

/////////////////////////////////////////////////////////////////////////
/// ������ �������� ������ ���Ĵ�
/// ��� ���������� �޶����� �Ѵ�.
/// ��� ����ð��� ���ϴ� �����̵ȴ�.
VOID CECReports_LoanHistoryDlg::SetRunMode()
{
	CStdioFile file;	
	
	if( file.Open( _T("..\\cfg\\BMode.txt"), CFile::modeRead | CFile::typeBinary ) )
	{
		file.Close();
		MODE = BEFORE_MODE;
	}
	else
	{
		MODE = AFTER_MODE;
	}
}

//////////////////////////////////////////////////////////////////////////
/// �۾���� ����� ��� DM�� �����Ѵ�.
INT CECReports_LoanHistoryDlg::MakeList	()
{
	INT		ids;
	CString strQuery;
	CString strTime;
	CString strCurTime = GetNow();

	if( MODE == BEFORE_MODE )
	{
		strTime.Format( _T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s') AND TO_DATE('%s')+1"), strCurTime, strCurTime );
	}
	else if( MODE == AFTER_MODE )
	{
		strTime.Format( _T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s')-1 AND TO_DATE('%s')"), strCurTime, strCurTime );
	}

	strQuery.Format( _T("SELECT BB.EPC, BB.MANAGE_CODE, TO_CHAR(CL.LOAN_DATE, 'YYYY-MM-DD-HH24-MI-SS') ")
					  _T(" FROM BO_BOOK_TBL BB, ")
						   _T(" LH_STORAGE_V01 CL ")
					 _T(" WHERE BB.REC_KEY = CL.BOOK_KEY ")
                     _T(" AND BB.EPC IS NOT NULL ")
                     _T(" AND CL.PUBLISH_FORM_CODE='MO' %s ORDER BY BB.MANAGE_CODE"), strTime );
	ids = DM.RestructDataManager( strQuery );
	if( 0 > ids )
	{
		///// ���� �α�
		CString strLogFile,strFile,strMsg;
		CTime t = CTime::GetCurrentTime();
		strLogFile.Format(_T("%04d%02d%02d"),  t.GetYear(), t.GetMonth(), t.GetDay());
		strFile.Format(_T("..\\log\\FAIL_%s.TXT"),strLogFile);
		strMsg.Format( _T("[DB CONNECTION ERROR !!!!]\r\n") );
		WriteLog(strFile,strMsg);
		return ids;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/// SYSDATE�� �Ұ�� �۾��ð��� ������� ������ �Ѿ�� 
/// ������ �ɼ� �ֱ� ������
/// �˻� ���ǿ� ���� ��¥�� �̸� ���� �д�.
CString CECReports_LoanHistoryDlg::GetNow()
{
	INT		ids;
	CString strQuery;
	CString strResult;
	CESL_DataMgr TmpDM;

	TmpDM.SetCONNECTION_INFO( DM.CONNECTION_INFO );

	strQuery.Format( _T("SELECT TO_CHAR( SYSDATE, 'YYYY/MM/DD' ) FROM DUAL") );

	ids = TmpDM.GetOneValueQuery( strQuery, strResult );
	if( 0 > ids )
	{
		///// ���� �α�
		CString strLogFile,strFile,strMsg;
		CTime t = CTime::GetCurrentTime();
		strLogFile.Format(_T("%04d%02d%02d"),  t.GetYear(), t.GetMonth(), t.GetDay());
		strFile.Format(_T("..\\log\\FAIL_%s.TXT"),strLogFile);
		strMsg.Format( _T("[DB CONNECTION ERROR !!!!]\r\n"));
		WriteLog(strFile,strMsg);
		return _T("");
	}

	strResult.TrimLeft();strResult.TrimRight();

	return strResult;
}

///////////////////////////////////////////////////////////////////////
/// ���� �Լ�
/// 1. ��� ����
/// 2. �������ڵ� ����
/// 3. xml ó��	--> API ó��
/// 4. send tcp	--> API ó��
/// 5. result xml �м� --> API ó��
/// 6. �Ϸ�~~~~
INT CECReports_LoanHistoryDlg::RunProc()
{
	if( 0 > MakeList() )
	{
		return -1;
	}

	INT		ids;
	CString strBeforeManageCode;
	CString strData;
	CString strEPC;
	CString strLibCode;
	CString strLendingData;
	CBOOK_TRADE_EPC_API API( BCS1 );
	CString strLogFile;

	CTime t = CTime::GetCurrentTime();
	strLogFile.Format(_T("%04d%02d%02d"),  t.GetYear(), t.GetMonth(), t.GetDay());
	
	/// ������ ��� ���� ��ŭ ���� ó��
	/// DM  COL 0 : EPC
	///         1 : B.MANAGE_CODE
	///         2 : LOAN_DATE ( YYYY-MM-DD-HH-MI-SS )
	
	for( INT i=0;i<DM.GetRowCount();i++ )
	{
		/// EPC
		/// test
//		strEPC = _T("urn:epc:tag:sgtin-96:1.9788988621.042.1");
		strEPC = DM.GetCellData(i, 0);
	
		/// ��������
		/// ������������ ������ �ڵ带 ó���Ѵ�.
		strData = DM.GetCellData(i, 1);

		/// ������ �ڵ尡 ����.
		if( !(strLibCode.GetLength()) || (strBeforeManageCode != strData) )
		{
			strLibCode = GetLibCode( strData );
			strBeforeManageCode = strData;			
		}
		else
		{
			//// ������ �ڵ尡 �����ϴ� 
		}
	
		/// ������ �Ǿ��µ��� ����� ���ٸ� �����̴�.
		if( !strLibCode.GetLength() )
		{
			/// �ٸ� ���������ڷᰡ ������ �ִ�.
			/// �ٽ� �õ� �Ѵ�.
			
			continue;
		}

		/// ������
		/// ���� �����ؾ��Ѵ�.
		strData = DM.GetCellData(i, 2);
		/// YYYY-MM-DD-HH-MI-SS >> YYYY-MM-DDTHH:MI:SS.0Z
		CString strYear, strMon, strDay, strHour, strMin, strSec;
		INT	nSPos=0;
		INT	nEPos=0;

		/// ��
		nEPos = strData.Find(_T("-"));
		strYear = strData.Mid( nSPos, nEPos );

		// ��
		nSPos = nEPos+1;
		nEPos = strData.Find(_T("-"),nSPos);
		strMon = strData.Mid( nSPos, nEPos-nSPos );

		// ��
		nSPos = nEPos+1;
		nEPos = strData.Find(_T("-"),nSPos);
		strDay = strData.Mid( nSPos, nEPos-nSPos );

		// �ð� 
		nSPos = nEPos+1;
		nEPos = strData.Find(_T("-"),nSPos);
		strHour = strData.Mid( nSPos, nEPos-nSPos );

		// �� 
		nSPos = nEPos+1;
		nEPos = strData.Find(_T("-"),nSPos);
		strMin = strData.Mid( nSPos, nEPos-nSPos );

		// ��
		nSPos = nEPos+1;	
		strSec = strData.Mid( nSPos );
		
		strLendingData.Format( _T("%s-%s-%sT%s:%s:%s.0Z"), strYear,strMon,strDay,strHour,strMin,strSec);

		ids = API.SendLoanHistory( strEPC, strLibCode, strLendingData );
		CString strFile;
		CString strMsg;
		if( -500 == ids )
		{
			///// ���� �α�
			strFile.Format(_T("..\\log\\FAIL_%s.TXT"),strLogFile);
			strMsg.Format( _T("[ECReports RESULT -1] %s : %s\r\n"),strLibCode,strEPC );
			WriteLog(strFile,strMsg);
			continue;
		}
		//// ���� �α�
		strFile.Format(_T("..\\log\\SUCCESS_%s.TXT"),strLogFile);
		strMsg.Format( _T("[ECReports SUCCESS] %s : %s\r\n"),strLibCode,strEPC );
		WriteLog(strFile,strMsg);
	}
	
	return 0;
}

INT CECReports_LoanHistoryDlg::WriteLog(CString strFile, CString strLog)
{	
	FILE* fp;
	
	fp = _tfopen( strFile.GetBuffer(0), _T("a+b") );
	
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE �����۾�
		// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			/*UNCHANGE*/fputc(0xFF, fp);
			/*UNCHANGE*/fputc(0xFE, fp);
		}
		// ------------------------------------------------------------------------------
	// KOL.UDF.022
	if( &fp != NULL )
	{		
		_ftprintf( fp, _T("%s"), strLog.GetBuffer(0) );
	}	

	fclose(fp);
	return 0;
}