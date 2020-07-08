// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <afxpriv.h>
#include "KOLAS3_MAIN.h"
#include "KOLAS3_MAINApi.h"
#include "KOLAS3_MAINView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
ON_WM_CREATE()
ON_WM_TIMER()
ON_WM_CLOSE()
ON_WM_SIZE()
ON_WM_MOVE()
	ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
ON_MESSAGE(WM_SET_PROGRESS_POS, OnSetProgressPos)
ON_MESSAGE(WM_SET_PROGRESS_TEXT, OnSetProgressText)
ON_MESSAGE(WM_SET_PROGRESS_SHOW, OnSetProgressShow)
ON_MESSAGE(WM_SET_STATUS_TEXT, OnSetStatusText)
ON_MESSAGE(WM_CALL_BEGINLOG, OnBeginLog)
ON_MESSAGE(WM_CALL_ENDLOG, OnEndLog)
//ON_MESSAGE_RANGE(WM_USER, WM_USER+3000, OnSetStatusText)
END_MESSAGE_MAP()

static UINT indicators[] =
{
		ID_SEPARATOR,           // status line indicator
		ID_STATUS_SLIDER,
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
		ID_INDICATOR_SCRL,
		ID_INDICATOR_SCRL
		//11115,
	//	11116
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_nPaneBase = 5;
	nSearchedRow = 0;
	nStartTime = 0;
	nFinishTime = 0;
}

CMainFrame::~CMainFrame()
{
}

INT CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifdef _KOLASIII
	SetIcon(::AfxGetApp()->LoadIcon(IDR_K3), FALSE);
#endif
#ifdef _KOLASIII_TL
	SetIcon(::AfxGetApp()->LoadIcon(IDI_MOBILE), FALSE);
#endif

	// status bar
	if (!m_wndStatusBar.Create(this, WS_CHILD|WS_VISIBLE|CBRS_BOTTOM) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	// progress bar
	if(!m_wndProgressBar.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, CRect(0,0,20,10), &m_wndStatusBar, ID_STATUS_PROGRESS))
		return -1;
	
	m_wndProgressBar.SetRange(0, 10000);
	m_wndProgressBar.SetPos(0);
	m_wndProgressBar.ShowWindow(SW_HIDE);
	m_wndStatusBar.SetPaneInfo(0, ID_STATUS_SLIDER, SBPS_NOBORDERS, 440); 
	m_wndStatusBar.SetPaneInfo(1, ID_STATUS_SLIDER, SBPS_NOBORDERS, 0); 
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_NOBORDERS,0);
	m_wndStatusBar.SetPaneInfo(3, ID_SEPARATOR, SBPS_NOBORDERS,0);	// �˻��� Row count
	m_wndStatusBar.SetPaneInfo(4, ID_SEPARATOR, SBPS_NOBORDERS,0);	// �˻��� �ɸ� �ð�
 	m_wndStatusBar.SetPaneInfo(5, ID_SEPARATOR, SBPS_NORMAL,165);	// ��¥ & �ð�
	m_wndStatusBar.SetPaneInfo(6, ID_SEPARATOR, SBPS_NORMAL,65);	// ���� ����
	
	m_wndStatusBar.SetPaneText(2, _T(""));
	m_wndStatusBar.SetPaneText(3, _T(""));
	m_wndStatusBar.SetPaneText(4, _T(""));
	m_wndStatusBar.SetPaneText(5, _T(""));
	m_wndStatusBar.SetPaneText(6, _T(""));

#ifdef _KOLASIII
	LOGFONT logFont;
	ZeroMemory(&logFont,sizeof(logFont));
	_tcscpy(logFont.lfFaceName,_T("����ü"));
	logFont.lfHeight = 12;
	logFont.lfCharSet = DEFAULT_CHARSET;
	
	m_fontStatusBar.CreateFontIndirect(&logFont);
	m_wndStatusBar.SetFont(&m_fontStatusBar);
#endif
	EnableDocking(CBRS_ALIGN_ANY);
	
#ifdef _KOLASIII
	if(::IsWindow(m_wndDialogBar))
	{
		ShowControlBar(&m_wndDialogBar, TRUE, FALSE);		
		return FALSE;
	}
	m_Navigator.SetFrameWnd(this);	
	if (!m_wndDialogBar.Create(this, &m_Navigator, CString("Explorer Bar"), IDD_INTERNET_NAVI) )
	{
		TRACE0("Failed to create dialogbar\n");
		return FALSE;      // fail to create
	}
	m_MenuBar.SetFrameWnd(this);	
	if ( !m_wndDialogBar2.Create(this, &m_MenuBar, CString("Menu Bar"), IDD_MENU_BAR) )
	{
		TRACE0("Failed to create dialogbar\n");
		return FALSE;      // fail to create
	}
	m_wndDialogBar2.SetBarStyle(m_wndDialogBar2.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC|CBRS_FLOAT_MULTI);	
#endif		
	m_nCurrentMode = _INET_FORM;
	SetTimer(0, 500, NULL);
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.hMenu = NULL;
	cs.style &= ~FWS_ADDTOTITLE;
	
	if (!LoadWindowPosition(cs))
	{
		CWnd *pWnd = this->GetDesktopWindow();
		CRect RectDesktop;
		pWnd->GetWindowRect(&RectDesktop);
		if (RectDesktop.Height() > 600) {
			cs.cx = 1020;
			cs.cy = 700;
		} else {
		}
	}	
	cs.lpszName = _T("KOLAS III");	

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// 2010.04.08 ADD BY CJY : �ٸ� ���α׷����� k3�� �ڵ��� ������� Ŭ�����̸��� ������Ų��.
	WNDCLASS wc;
	::GetClassInfo( AfxGetInstanceHandle(), cs.lpszClass, &wc );
#ifdef _KOLASIII
	wc.lpszClassName = cs.lpszClass = _T("Afx:400000:b:10003:6:8dd1399");
#endif
#ifdef _KOLASIII_TL
    wc.lpszClassName = cs.lpszClass = _T("Afx:400000:b:10003:6:1d91dab");	    
#endif
	::RegisterClass( &wc );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
VOID CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

VOID CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CView *pView = GetActiveView();	
	if ( pView->GetSafeHwnd() != NULL ) pView->SendMessage(WM_COMMAND, wParam, lParam);	
	
	return CFrameWnd::OnCommand(wParam, lParam);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_HANGUL ) 
	{
		SetHanEngStatusItems();
	}	
	
	if (pMsg->message == WM_CHILDACTIVATE)
	{
		INT a = 1;
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}


VOID CMainFrame::OnTimer(UINT nIDEvent) 
{
	// Status Bar refresh
	if (nIDEvent == 0) 		
	{
		ShowCurrentStatus();		
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}


VOID CMainFrame::SetHanEngStatusItems()
{

	//++2008.07.01 UPDATE BY LKS {{++
	
	HKEY hKey = NULL; 
	// Key�� Open�ϰ� key���� �޾Ƴ���. 
	
	CString m_strPath;
	TCHAR m_szLanguage[256] = {0,};
	GetKeyboardLayoutName(m_szLanguage);
	
	// ������ �⺻���� E0200412�� ���� "�ѱ�"�� �ƴ� "Microsoft IME 2003" �̱� ������ ���������� ó��
	// ���� �ѱ� ������ ��� �ѱ�/���� ���ζ��� ������ ó���ؾ���

	if(_tcscmp(m_szLanguage,_T("E0200412"))==0)
	{
		//++2008.07.01 ���� �ҽ� {{++
		HWND hWnd	= GetSafeHwnd();
		HIMC hIMC	= ImmGetContext(hWnd);
		BOOL bOpen	= ImmGetOpenStatus(hIMC);
		
		if (bOpen)
			m_wndStatusBar.SetPaneText(6, _T("\t�ѱ�") );
		else 
			m_wndStatusBar.SetPaneText(6, _T("\t����") );
		
		ImmReleaseContext(hWnd,hIMC);
		//--2008.07.01 ���� �ҽ� --}}
	}
	
	else
	{
		//������Ʈ��
		//HKEY_LOCAL_MACHINE->System->CurrentControlSet->Control->Keyboard Layouts�� ���Ǻ� ������ ����ִ�.

		m_strPath.Format(_T("%s%s"),_T("System\\CurrentControlSet\\Control\\Keyboard Layouts\\"),  	m_szLanguage);
		
		LONG lResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, 
			m_strPath, 0, /*KEY_ALL_ACCESS*/KEY_QUERY_VALUE, &hKey); 
		
		// Open�� ������ �ƴ� ���
		if(ERROR_SUCCESS != lResult) return;
		
		CString strValueName = _T("Layout Text");// ���� ���� �̸�	
		DWORD dwType = REG_SZ;  // ã�°��� TYPE 
		DWORD dwCount =0;//_MAX_PATH;  // ��ȯ�� ���� ���� 	
		
		// dwCount�� �����͸� �޾ƿ��� ��, �ʿ��� ũ�Ⱑ ���´�.
		TCHAR* pszValue = '\0';
		lResult = RegQueryValueEx(
			hKey,
			strValueName,
			NULL,
			&dwType,
			(LPBYTE)pszValue,
			&dwCount
			); 
		
		// �ʿ��� ũ�⸸ŭ �����Ҵ� ������
		pszValue = new TCHAR[dwCount];
		// �ٽ� �����͸� �޾ƿ´�.
		lResult = RegQueryValueEx(
			hKey,
			strValueName,
			NULL,
			&dwType,
			(LPBYTE)pszValue,
			&dwCount
			); 
		
		//++2008.07.25 UPDATE BY CJY {{++
		// �޸� �������� 
		// ���� �����ϱ����� �޸𸮸� ���������ش�
		if(ERROR_SUCCESS != lResult)
		{
			RegCloseKey(hKey);
			delete[] pszValue;
			return ;
		}
		// �����ҽ�
		//DEL if(ERROR_SUCCESS != lResult) return;
		//--2008.07.25 UPDATE BY CJY --}}
		
		//�ʿ��� ������ �߶󳽴�.(�� ó�� ���� �ձ���) ex) �Ϻ��� �Է� �ý��� -> �Ϻ���
		CString strLangValue;
		strLangValue.Format(_T("\t%s"),pszValue);
		
		int nIndex = strLangValue.Find(' ',0);
		
		CString strTemp;
		
		if (nIndex!=-1)//������ ������ �߶󳽴� ex) ���� (�̱���) -> ����
		{
			strTemp = strLangValue.Left(nIndex);
			
		}
		
		else//������ ������ �״�� ���� ex) ������� -> �������
		{
			strTemp = strLangValue;
		}
		
		
		m_wndStatusBar.SetPaneText(6, strTemp );
	
		//++2008.07.25 ADD BY CJY {{++
		// �޸� ���� ����
		// delete �߰�
		RegCloseKey(hKey);
		delete[] pszValue;		

		//--2008.07.25 ADD BY CJY --}}
	}

	//--2008.07.01 UPDATE BY LKS --}}
		
}

VOID CMainFrame::ShowCurrentStatus()
{
	CTime time = CTime::GetCurrentTime();
	HWND hWnd = GetSafeHwnd();
	
	CString strMsg;
	
	INT nHour = time.GetHour();
	if (nHour < 12) {
		strMsg.Format(	_T("\t%04d�� %02d�� %02d�� ���� %02d:%02d"), 
			time.GetYear(), time.GetMonth(), time.GetDay(),
			nHour, time.GetMinute()
			);
	} else {
		if (nHour >= 13) nHour -= 12;
		strMsg.Format(	_T("\t%04d�� %02d�� %02d�� ���� %02d:%02d"), 
			time.GetYear(), time.GetMonth(), time.GetDay(),
			nHour, time.GetMinute()
			);
	}
	m_wndStatusBar.SetPaneText(m_nPaneBase+0, strMsg );
	
	SetHanEngStatusItems();
}

LRESULT CMainFrame::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{
	UINT nIDLast = m_nIDLastMessage;
	m_nFlags &= ~WF_NOPOPMSG;
	
	CWnd* pMessageBar = GetMessageBar();
	if (pMessageBar != NULL)
	{
		CString strMsg;
		CString strMessage;
		
		// set the message bar text
		if (lParam != 0)
		{
			ASSERT(wParam == 0);    // can't have both an ID and a string
			m_strStatusBar = (LPCTSTR)lParam;
			strMsg = m_strStatusBar;
		}
		else if (wParam != 0)
		{
			// map SC_CLOSE to PREVIEW_CLOSE when in print preview mode
			if (wParam == AFX_IDS_SCCLOSE && m_lpfnCloseProc != NULL)
				wParam = AFX_IDS_PREVIEW_CLOSE;
			
			// get message associated with the ID indicated by wParam
            if (wParam == AFX_IDS_IDLEMESSAGE)
				strMsg = m_strStatusBar;
			else
			{
				GetMessageString(wParam, strMessage);
				strMsg = strMessage;
			}
		}
		
		pMessageBar->SetWindowText(strMsg);
		
		// update owner of the bar in terms of last message selected
		CFrameWnd* pFrameWnd = pMessageBar->GetParentFrame();
		if (pFrameWnd != NULL)
		{
			m_nIDLastMessage = (UINT)wParam;
			m_nIDTracking = (UINT)wParam;
		}
	}
	
	m_nIDLastMessage = (UINT)wParam;    	// new ID (or 0)
	m_nIDTracking = (UINT)wParam;       	// so F1 on toolbar buttons work
	
	return nIDLast;
}

BOOL CMainFrame::LoadWindowPosition(CREATESTRUCT& cs)
{
	CWinApp *pApp = AfxGetApp();
	
	INT ids = -1;
	CRect rect;
	
	if ((rect.left = pApp->GetProfileInt(_T("MAIN_FRAME"), _T("left"), -1)) != -1 &&
		(rect.top = pApp->GetProfileInt(_T("MAIN_FRAME"), _T("top"), -1)) != -1 &&
		(rect.right = pApp->GetProfileInt(_T("MAIN_FRAME"), _T("right"), -1)) != -1 &&
		(rect.bottom = pApp->GetProfileInt(_T("MAIN_FRAME"), _T("bottom"), -1)) != -1
		)
	{
		// width 
		cs.cx = rect.right - rect.left;
		// height
		cs.cy = rect.bottom - rect.top;
		
		// window�� ������ ��������...
		INT x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
		INT y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
		
		
		cs.x = min(rect.left, x);
		cs.y = min(rect.top, y);
	}
	else
		return FALSE;
	
	return TRUE;
}

VOID CMainFrame::OnClose() 
{
	// �������� ��ġ�� �����Ѵ�.
	CWinApp *pApp = AfxGetApp();
	
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	
	pApp->WriteProfileInt(_T("MAIN_FRAME"), _T("top"), wp.rcNormalPosition.top);
	pApp->WriteProfileInt(_T("MAIN_FRAME"), _T("left"), wp.rcNormalPosition.left);
	pApp->WriteProfileInt(_T("MAIN_FRAME"), _T("right"), wp.rcNormalPosition.right);
	pApp->WriteProfileInt(_T("MAIN_FRAME"), _T("bottom"), wp.rcNormalPosition.bottom);
	
	CESL_DataMgr dm1;
	dm1.ORACLE_DisconnectDB();
	
	CFrameWnd::OnClose();	
}

VOID CMainFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	if(!::IsWindow(m_wndProgressBar.m_hWnd))
        return;
	
	CRect   rc;
    m_wndStatusBar.GetItemRect(1,rc);
	rc.top -=1;
	rc.bottom +=5;
    
    m_wndProgressBar.SetWindowPos(NULL,rc.left,
		rc.top-1,
		rc.Width(),
		rc.Height()+2,SWP_NOZORDER | SWP_NOACTIVATE);
	
	m_wndProgressBar.GetClientRect(rc);
	HRGN hrgn = CreateRectRgn(rc.left+2, rc.top+4, rc.right-2, rc.bottom-6);
	m_wndProgressBar.SetWindowRgn(hrgn, TRUE);
	DeleteObject(hrgn);

	//++2008.07.07 UPDATE BY LKS {{++
	//ũ�� ������ ����OR ���ͳ��� �� ���� MENU FORM�� �����
	//���¹� �� ũ�� �� �Ӽ��� �ٸ��� �����Ѵ�.
	/*�����ҽ�
	INT nPaneSize_0 = cx-750;
	if (nPaneSize_0 < 50) {
		m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS , 50); 
	}else {
		m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS , nPaneSize_0); 
	}
	*/
	if(m_nCurrentMode == _INET_FORM)
	{
		SetStatusBarMode_InternetForm();
	}
	else
	{
		SetStatusBarMode_MenuForm();
	}
	//--2008.07.07 UPDATE BY LKS --}}
}

LRESULT CMainFrame::OnSetProgressPos(WPARAM wParam, LPARAM lParam)
{
	m_wndProgressBar.SetRange(0, (short)lParam);
	m_wndProgressBar.SetPos((short)wParam);
	
	return 0;
}

LRESULT CMainFrame::OnSetProgressText(WPARAM wParam, LPARAM lParam)
{
	//m_wndProgressBar.SetRange(0, (short)lParam);
	CString strText;
	strText.Format(_T("%s"), ((CString*)wParam)->GetBuffer(0));
	m_wndProgressBar.SetWindowText(strText);
	m_wndProgressBar.SetPos((short)lParam);
	
	return 0;
}

LRESULT CMainFrame::OnSetProgressShow(WPARAM wParam, LPARAM lParam)
{
	m_wndProgressBar.ShowWindow((BOOL)wParam);
	return 0;
}

LRESULT CMainFrame::OnSetStatusText(WPARAM wParam, LPARAM lParam)
{

/*
TCHAR *pBuf = (TCHAR*)lParam;
CString str;
if (pBuf) str.Format(_T("%s"), pBuf);
else str = _T("");

  m_wndStatusBar.SetPaneText(2, str );
	*/
	return 0;
}

void CMainFrame::DoDataExchange(CDataExchange* pDX) 
{	
	CFrameWnd::DoDataExchange(pDX);
}

void CMainFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
	
	CView *pView = GetActiveView();
	if ( pView != NULL )
	{
		
		DWORD dwParam;
		WORD  wHigh, wLow;
		
		wHigh = x;
		wLow = y;
		
		dwParam = 0;
		dwParam = (DWORD) (wHigh << 16);
		dwParam |= (DWORD) (wLow);   		
		
		if ( pView->GetSafeHwnd() != NULL ) pView->SendMessage(WM_MOVE, dwParam, dwParam);
	}

#ifdef _KOLASIII
	if(NULL != m_Navigator.GetSafeHwnd()) m_Navigator.SendMessage(WM_MOVE, x, y);
#endif
}

//++2008.07.02 ADD BY LKS {{++
LRESULT CMainFrame::OnBeginLog(WPARAM wParam, LPARAM lParam)
{
	//���� �ð��� ����Ѵ�.
	nStartTime = GetTickCount();

	return TRUE;
}

//wParam�� �˻��� ���� ������ �Ѿ����, 
//lParam���� �α׸� ȣ���� ���� �˻��̸� 0 , �����̸� 1�� �Ѿ�´�.
LRESULT CMainFrame::OnEndLog(WPARAM wParam, LPARAM lParam)
{
	CKOLAS3_MAINView* pView = ((CKOLAS3_MAINView*)GetActiveView());
	CString strRowCount,strWorkingTime;
	strRowCount.Format(_T("\t%s%d%s"),_T(""),wParam,_T(" rows"));

	nFinishTime = GetTickCount();

	strWorkingTime.Format(_T("\t%d.%02d sec"),
							(nFinishTime-nStartTime)/1000,
							(nFinishTime-nStartTime)%1000);

	if(lParam == 0)
	{
		m_wndStatusBar.SetPaneText(3, strRowCount );
		m_wndStatusBar.SetPaneText(4, strWorkingTime );

		//���� ���õ� �Ǹ���Ʈ �׸� �˻��ð��� Row ������ ����Ѵ�.
		pView->UpdateTabInfo(strRowCount,strWorkingTime);
	}
	else//save���� ȣ���� ���
	{
		//���� ���
		//m_wndStatusBar.SetPaneText(4, strWorkingTime );
	}
	
	nStartTime = 0;

	return TRUE;
}

//KOLAS3_MAINView���� ȣ���ϴ� �Լ�
//StatusBar�� WorkingTime�� �˻��� Row������ ����
VOID CMainFrame::ClearStatusBarLog()
{
	m_wndStatusBar.SetPaneText(0, _T(""));
	m_wndStatusBar.SetPaneText(1, _T(""));
	m_wndStatusBar.SetPaneText(2, _T(""));
	m_wndStatusBar.SetPaneText(3, _T(""));
	m_wndStatusBar.SetPaneText(4, _T(""));
}

VOID CMainFrame::SetStatusBarMode_InternetForm()
{
	CRect rect;
	GetClientRect(&rect);
	INT cx = rect.Width();
	INT nPaneSize_0 = cx-750;

	m_wndStatusBar.SetPaneInfo(1, ID_STATUS_SLIDER, SBPS_NOBORDERS, 0); 
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_NOBORDERS,0);
	m_wndStatusBar.SetPaneInfo(3, ID_SEPARATOR, SBPS_NOBORDERS,0);	// �˻��� Row count
	m_wndStatusBar.SetPaneInfo(4, ID_SEPARATOR, SBPS_NOBORDERS,0);	// �˻��� �ɸ� �ð�

	if (nPaneSize_0 < 50) {
		m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS , 440); 
	} else {
		m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS , nPaneSize_0+440);
	}

	m_nCurrentMode = _INET_FORM;	//OnSize���� ���� Mode�� �˾ƾ� �Ѵ�.
}

VOID CMainFrame::SetStatusBarMode_MenuForm()
{
	CRect rect;
	GetClientRect(&rect);
	INT cx = rect.Width();
	INT nPaneSize_0 = cx-750;
	m_wndStatusBar.SetPaneInfo(1, ID_STATUS_SLIDER, SBPS_NOBORDERS, 195); 
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_NORMAL,95);
	m_wndStatusBar.SetPaneInfo(3, ID_SEPARATOR, SBPS_NORMAL,70);	// �˻��� Row count
	m_wndStatusBar.SetPaneInfo(4, ID_SEPARATOR, SBPS_NORMAL,80);	// �˻��� �ɸ� �ð�

	if (nPaneSize_0 < 50) {
		m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS , 50); 
	}else {
		m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS , nPaneSize_0+0); 
	}
	
	m_nCurrentMode = _MENU_FORM;	//OnSize���� ���� Mode�� �˾ƾ� �Ѵ�.
}
//--2008.07.02 ADD BY LKS --}}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

BOOL CMainFrame::OnNcActivate(BOOL bActive) 
{
	// TODO: Add your message handler code here and/or call default
	if(TRUE == bActive) 
	{
		CView *pView = GetActiveView();
		if(NULL != pView) pView->SendMessage(WM_NCACTIVATE, (BOOL)bActive, NULL);
	}
	return CFrameWnd::OnNcActivate(bActive);
}
