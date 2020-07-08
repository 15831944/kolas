// InternetNavigator.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "InternetNavigator.h"
// ������Ʈ�� �ҽ����Ͽ��� �����ܰ�θ� �о� �����ڵ�� ��ȯ�ϴ� �� ���
#include "CharConvert.h"	
#include "BalloonHelp.h"

// 17/02/06 ������ : KOLAS3.net Search�� ���� �˻��� 404 ���� �߻�
//*/ ADD ---------------------------------------------------------------------------
#include "..\\..\\..\\����\\����\\�弭����\\LOC_CommonAPI\\LocCommonAPI.h"
//*/ END ---------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInternetNavigator dialog


CInternetNavigator::CInternetNavigator(CWnd* pParent /*=NULL*/)
	: CDialog(CInternetNavigator::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInternetNavigator)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pView = NULL;
	m_bLock = FALSE;

	//++2008.07.03 ADD BY CJY {{++
	m_bIsKeyDown = FALSE;
	m_hDefaultIcon = NULL;

//DEL 	m_bDrawIcon = FALSE;
	//--2008.07.03 ADD BY CJY --}}
}


CInternetNavigator::~CInternetNavigator()
{
	m_Dlg_brush.DeleteObject();

	CString strData[MAX_URL_COUNT];
	const INT MAX = m_aURLHistory.GetSize();
	for(INT nCount=0; nCount < MAX; nCount++)
	{
		strData[nCount] = m_aURLHistory.GetAt(nCount);
	}

	Set_URLHistoryToRegstry(strData, MAX_URL_COUNT, 1);
}

void CInternetNavigator::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInternetNavigator)	
	DDX_Control(pDX, IDC_cmbURL, m_cmbURL);
	DDX_Control(pDX, IDC_edtHELP, m_edtHELP);	
	DDX_Control(pDX, IDC_btnNEXT	, m_btnNext);
	DDX_Control(pDX, IDC_btnPREV	, m_btnPrev);
	DDX_Control(pDX, IDC_btnREFLASH	, m_btnReflash);
	DDX_Control(pDX, IDC_btnSTOP	, m_btnStop);	
	DDX_Control(pDX, IDC_btnHELP	, m_btnHelp);
	//}}AFX_DATA_MAP
	
}

BEGIN_MESSAGE_MAP(CInternetNavigator, CDialog)
	//{{AFX_MSG_MAP(CInternetNavigator)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnREFLASH, OnbtnREFLASH)
	ON_BN_CLICKED(IDC_btnSTOP, OnbtnSTOP)
	ON_BN_CLICKED(IDC_btnHELP, OnbtnHELP)
	ON_BN_CLICKED(IDC_btnLOCK, OnbtnLOCK)
	ON_CBN_SELENDOK(IDC_cmbURL, OnSelendokcmbURL)
	ON_CBN_EDITCHANGE(IDC_cmbURL, OnEditchangecmbURL)
	ON_WM_MOVE()
	ON_CBN_DROPDOWN(IDC_cmbURL, OnDropdowncmbURL)
	ON_EN_KILLFOCUS(IDC_edtHELP, OnKillfocusedtHELP)
	ON_EN_SETFOCUS(IDC_edtHELP, OnSetfocusedtHELP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInternetNavigator message handlers

BOOL CInternetNavigator::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	m_pParent = pParentWnd;	
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CInternetNavigator::PreTranslateMessage(MSG* pMsg)
{
	// ��ݱ��
	// �޽��� ���	
 	if ( m_bLock == TRUE ) 
 	{
 		if ( pMsg->message != WM_PAINT )
 		{
 			return TRUE;
 		}
 	}

	//++2008.06.30 UPDATE BY CJY {{++

	// ��ư�� ������ ���
	// ESCŰ -> �ڵ��ϼ� ����Ʈ ����
	// ����Ű -> �ּ�ǥ������ ����Ʈ�� �̵�
	// �Ʒ�/�� ȭ��ǥ/��������/�������ٿ� -> �ڵ��ϼ� ����Ʈ�� ���õ� ����Ʈ ����
	// ��Ű -> �ڵ��ϼ� ����Ʈ�� ���õ� �ؽ�Ʈ�� �޺��ڽ� ����Ʈ�� ����
	if(pMsg->message == WM_KEYDOWN )
	{	
		
		switch(pMsg->wParam)
		{
 		case VK_ESCAPE:			
	 		m_List.ShowWindow(SW_HIDE);
 			return TRUE;
 			break;
		case VK_RETURN:
			OnNavi();
			return TRUE;
			break;
			
		case VK_DOWN:			
			if(TRUE == m_cmbURL.GetDroppedState())
			{
				INT nResult = m_cmbURL.GetCurSel();
				INT nMax = m_cmbURL.GetCount();
				if(nResult+1 < nMax) m_cmbURL.SetCurSel(nResult+1);
			}
			else if(TRUE == m_List.IsWindowVisible())
			{
				m_List.Set_NextCurSel();
			}  			
			return TRUE;
			break;
		case VK_UP:		
			if(TRUE == m_cmbURL.GetDroppedState())
			{
				INT nResult = m_cmbURL.GetCurSel();
				if(0 <= nResult-1) m_cmbURL.SetCurSel(nResult-1);
			}
			else if(TRUE == m_List.IsWindowVisible())
			{
				m_List.Set_PrevCurSel();
			} 				
			return TRUE;			
			break;	
 		case VK_NEXT:
			if(TRUE == m_cmbURL.GetDroppedState())
			{	// 9�� �� ȭ�鿡 ǥ�õǴ� �޺��ڽ� ����� ����
				INT nResult = m_cmbURL.GetCurSel();
				INT nMax = m_cmbURL.GetCount();
				if(nResult+9 < nMax) m_cmbURL.SetCurSel(nResult+9);
				else m_cmbURL.SetCurSel(nMax-1);
			}
			else if(TRUE == m_List.IsWindowVisible())
			{
				m_List.Set_TailCurSel();
			} 			
 			return TRUE;
 			break;		
 		case VK_PRIOR:
			if(TRUE == m_cmbURL.GetDroppedState())
			{	// 9�� �� ȭ�鿡 ǥ�õǴ� �޺��ڽ� ����� ����
				INT nResult = m_cmbURL.GetCurSel();
				if(0 <= nResult-9) m_cmbURL.SetCurSel(nResult-9);
				else m_cmbURL.SetCurSel(0);
			}
			else if(TRUE == m_List.IsWindowVisible())
			{
				m_List.Set_HeadCurSel();
			} 	 			
 			return TRUE;
 			break;
		case VK_TAB:
			//	��Ű�� ������ ��� �ڵ��ϼ�����Ʈ�� ������ ����Ʈ���ڿ� �����ֱ�
			{
				CString strText = m_List.Get_CurSelText();				
				if(_T("") != strText)
				{
					m_cmbURL.SetWindowText(strText);
					m_List.ShowWindow(SW_HIDE);
					
					CEdit* pEdit = m_cmbURL.GetEditCtrl();
					pEdit->SetSel(-1);
				}
				return TRUE;
			}			
			break;
		default:
			m_bIsKeyDown = TRUE;
			break;
		}
		
	}
	else if(pMsg->message == WM_KEYUP)
	{
		m_bIsKeyDown = FALSE;		
	}
	
	//DEL �����ҽ�
//DEL 	if ( pMsg->message == WM_KEYDOWN )
//DEL 	{
//DEL 		if ( pMsg->wParam == VK_ESCAPE ) 
//DEL 		{
//DEL 			return TRUE;
//DEL 		}
//DEL 		else if ( pMsg->wParam == VK_RETURN )
//DEL 		{
//DEL 			//m_pView = m_pFrameWnd->GetActiveView();
//DEL 			( (CEdit*) GetDlgItem (IDC_edtURL) )->GetWindowText(m_strURL);
//DEL 			if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1010, NULL, NULL);
//DEL 			return TRUE;
//DEL 		}
//DEL 	}
	//--2008.06.30 UPDATE BY CJY --}}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CInternetNavigator::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnPrev.LoadBitmaps("..\\BMP\\IePrev.bmp"		, RGB(0,0,0));
	m_btnNext.LoadBitmaps("..\\BMP\\IeNext.bmp"		, RGB(0,0,0));
	m_btnReflash.LoadBitmaps("..\\BMP\\Iereflash.bmp"	, RGB(0,0,0));
	m_btnStop.LoadBitmaps("..\\BMP\\Iestop.bmp"		, RGB(0,0,0));
	m_btnHelp.LoadBitmaps("..\\BMP\\IeHelp.bmp"		, RGB(0,0,0));	

	m_Dlg_brush.CreateSolidBrush(RGB(230,230,230));
	
	//++2008.07.01 ADD BY CJY {{++
	// �ش����Ʈ�� ������������ ���� �� �ҷ��� ���� ����
//DEL 	m_hDefaultIcon = (HICON)::LoadImage(
//DEL 		NULL,
//DEL 		(LPCTSTR)ICON_FILE_PATH _T("\\default.ico"),
//DEL 		IMAGE_ICON,
//DEL 		0,
//DEL 		0,
//DEL 		LR_LOADFROMFILE
//DEL 		); 	
	//DEL m_hDefaultIcon = ExtractIcon(NULL, _T("url.dll"), 1);
	ExtractIconEx(_T("url.dll"), 0, NULL, &m_hDefaultIcon, 1);

	// ���丮 ������ �����(���� �������丮�� �����)
	CFileFind file;
	CString strDir = ICON_FILE_PATH _T("\\");	
	BOOL bResult = file.FindFile(strDir + _T("*.*"));
	if(TRUE != bResult)
	{ 
		CreateDirectory(strDir, NULL);
	}
	else	// ���� ������ ����
	{	// ������ �����ܰ� ���� �湮�� ������Ʈ ������ ��ġ�Ѵٴ� ������ ����
		if(TRUE == file.FindFile(strDir + _T("*.ico")))
		{
			while(TRUE == file.FindNextFile())
			{
				::DeleteFile(strDir + file.GetFileName());
			}
			::DeleteFile(strDir + file.GetFileName());
		}
	}
	

	// �����丮 ����
 	CString strURL[MAX_URL_COUNT];
 	Get_URLHistoryFromRegstry(strURL, MAX_URL_COUNT, 1);

	// �޺��ڽ��� ����� �̹��� ����Ʈ�� �ʱ�ȭ	
 	m_aImageList.Create(16, 16, ILC_COLOR32|ILC_MASK, 0, 25);
 
 	for(INT nCount=0; nCount<MAX_URL_COUNT; nCount++)
 	{
 		if(_T("") == strURL[nCount]) break;
 		m_aURLHistory.Add(strURL[nCount]);		
 	}
	
 	m_List.Create(IDD_AUTO_COMPLETE, this);	
	m_List.m_pParent = this;
 	m_List.Add_AutoCompleteData(&m_aURLHistory);

	m_cmbURL.SetImageList(&m_aImageList);
	m_cmbURL.Set_DefaultIcon(m_hDefaultIcon);
 	m_cmbURL.Set_DrawIconToEdit(TRUE);

 	Add_AllItemFromHistroyArray();
 	Add_AllIconToImageList();
	//--2008.07.01 ADD BY CJY  --}}	

	//++2008.07.10 ADD BY CJY {{++
	// �ʱ�ȭ��� about:blank �����ֱ�	
	// �� �������� �����ֱ� ���ؼ� �������� �߰��Ͽ� �����Ͽ��� �Ѵ�.
	// �޺��ڽ��� �������� �߰��ص� ����ڰ� �޺��ڽ��� Dropdown�� ��
	// m_aURLHistory�� �ִ� �������� �������� ���ŵǱ� ������ �������
//DEL 	m_bDrawIcon = TRUE;
 	m_cmbURL.SetWindowText(_T("about:blank"));
	// �⺻ �������� �ҷ��µڿ�
 //DEL 	m_aImageList.Add(m_hDefaultIcon);
//DEL 
//DEL 	// ������ ����
//DEL 	COMBOBOXEXITEM Item;
//DEL 	Item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE;
//DEL 	Item.iItem = 0;	
//DEL 	Item.iImage = 0;
//DEL 	Item.iSelectedImage = 0;
//DEL 	Item.pszText = (TCHAR*)(LPCTSTR)_T("about:blank");
//DEL 	
//DEL 	// �޺��ڽ��� �������� �߰�����
//DEL 	m_cmbURL.InsertItem(&Item);
//DEL 	// ��� �߰��� �������� �����Ѵ�
//DEL  	m_cmbURL.SetCurSel(0);

	// ���� �̹��� ����Ʈ���� �߰��� �̹��� ����
 //DEL 	m_aImageList.Remove(m_aImageList.GetImageCount()-1);
	
	//--2008.07.10 ADD BY CJY  --}}	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CInternetNavigator::GetURL()
{	
	return m_strURL;
}

VOID CInternetNavigator::SetURL(CString strURL, BOOL bReDownload_IconFile)
{
	m_List.ShowWindow(SW_HIDE);
//++2008.07.01 UPDATE BY CJY {{++
	
 	if(_T("") == strURL)
	{	
 		m_cmbURL.SetCurSel(-1);
		m_cmbURL.SetWindowText(_T("about:blank"));	
	}
 	else
	{
		strURL = Make_URL(strURL);

		INT nIndex = Get_IndexFromHistoryArray(strURL);
		
		// 2009.05.11 REM BY PDJ : �ڲ� �ٿ��.
//		Put_Item(strURL, &nIndex, bReDownload_IconFile);
// 		if(0 <= nIndex)
//		{
//			m_cmbURL.SetCurSel(nIndex);
//		}
	}	
	m_strURL = strURL;

//DEL	�����ҽ�
//DEL 	m_strURL = strURL;
//DEL 	( (CEdit*) GetDlgItem (IDC_edtURL) )->SetWindowText(m_strURL);	
//--2008.07.01 UPDATE BY CJY --}}
}
void CInternetNavigator::OnSetfocusedtURL() 
{
	//++2008.06.30 UPDATE BY CJY {{++
	m_cmbURL.GetWindowText(m_strURL);
	CEdit* pEdit = m_cmbURL.GetEditCtrl();
	pEdit->SetSel(0, m_strURL.GetLength());
//DEL	�����ҽ�
//DEL 	CString strURL;	
//DEL 	( (CEdit*) GetDlgItem (IDC_edtURL) )->GetWindowText(m_strURL);		
//DEL 	( (CEdit*) GetDlgItem (IDC_edtURL) )->SetSel(0,m_strURL.GetLength());
	
	//--2008.06.30 UPDATE BY CJY --}}
}

void CInternetNavigator::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
}

void CInternetNavigator::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);	

	//++2008.07.02 UPDATE BY CJY {{++
	// ����Ʈ�ڽ�->�޺��ڽ��� ����
	// ȭ������� ����
	CWnd *pWnd = ( (CWnd*) GetDlgItem (IDC_cmbURL) );
	if ( pWnd->GetSafeHwnd() )
	{
		if ( cx < 450 ) cx=450;
		// 3�� ũ�Ⱑ ������ ��Ʈ���� ����
		HDWP hdwp = ::BeginDeferWindowPos(4);
 		// �޺��ڽ� ũ������
 		CRect rect = CRect(71, 5, cx-256, 27);
 		::DeferWindowPos(
 			hdwp, 
 			pWnd->GetSafeHwnd(),
 			HWND_TOP,                
 			rect.left,
 			rect.top,
 			rect.Width(),
 			rect.Height(),
 			SWP_NOZORDER|SWP_SHOWWINDOW
 			);
 		// �ڵ��ϼ�����Ʈ ũ������
  		rect.top = rect.bottom;//rect.bottom;
		rect.left = rect.left+25;
		rect.right = rect.right-220;
   		rect.bottom = rect.top+100;//rect.top + 400; 		
 		ClientToScreen(rect);
 		m_List.MoveWindow(rect); // m_List�� ���̾�α�
		// ���� ��ư ũ������
		pWnd = ( (CWnd*) GetDlgItem (IDC_btnSTOP) );
		pWnd->GetClientRect(rect);		
		ClientToScreen(rect);
 		rect.top = 5;
 		rect.bottom = 27;
 		rect.left = cx - ( rect.right - rect.left)-204;
 		rect.right = cx - 204;
 		::DeferWindowPos(
 			hdwp, 
 			pWnd->GetSafeHwnd(),
 			HWND_TOP,                
 			rect.left,
 			rect.top,
 			rect.Width(),
 			rect.Height(),
 			SWP_NOZORDER|SWP_SHOWWINDOW
 			);
		// ���ΰ�ħ ��ư ũ������
		pWnd = ( (CWnd*) GetDlgItem (IDC_btnREFLASH) );
		pWnd->GetClientRect(rect);		
		ClientToScreen(rect);
 		rect.top = 5;
 		rect.bottom = 27;
 		rect.left = cx - ( rect.right - rect.left)- 230;
 		rect.right = cx - 230;
		::DeferWindowPos(
			hdwp, 
			pWnd->GetSafeHwnd(),
			HWND_TOP,                
			rect.left,
			rect.top,
			rect.Width(),
			rect.Height(),
			SWP_NOZORDER|SWP_SHOWWINDOW
			);
		// ���� ��ư ũ������
		pWnd = ( (CWnd*) GetDlgItem (IDC_btnHELP) );
		pWnd->GetClientRect(rect);		
		ClientToScreen(rect);
 		rect.top = 5;
 		rect.bottom = 27;
 		rect.left = cx - ( rect.right - rect.left)- 4;
 		rect.right = cx - 4;
		::DeferWindowPos(
			hdwp, 
			pWnd->GetSafeHwnd(),
			HWND_TOP,                
			rect.left,
			rect.top,
			rect.Width(),
			rect.Height(),
			SWP_NOZORDER|SWP_SHOWWINDOW
			);
		// ���� ����Ʈ ũ������
  		//rect.top = rect.bottom;
		rect.left = rect.left-170;
		rect.right = rect.right-27;
   		//rect.bottom = rect.top+100;
 		//ClientToScreen(rect);
 		m_edtHELP.MoveWindow(rect); 		
		::EndDeferWindowPos (hdwp);
		CStatic *m_HelpMessage;		
		m_HelpMessage = (CStatic*)GetDlgItem(IDC_HELP_MESSAGE);
		m_HelpMessage->GetClientRect(rect);		
		ClientToScreen(rect);
		rect.top = 8;
 		rect.bottom = 24;
 		rect.left = cx - ( rect.right - rect.left)- 55;
 		rect.right = cx - 55;
		m_HelpMessage->MoveWindow(rect); 
	}
}

HBRUSH CInternetNavigator::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=NULL;
	if(pWnd->m_hWnd==this->m_hWnd) hbr= HBRUSH(m_Dlg_brush);	
	else hbr =CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CInternetNavigator::OnbtnPREV() 
{	
	if ( m_pView == NULL ) return;
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1100, NULL, NULL);		
}

void CInternetNavigator::OnbtnNEXT() 
{	
	if ( m_pView == NULL ) return;
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1101, NULL, NULL);			
}

void CInternetNavigator::OnbtnREFLASH() 
{	
	if ( m_pView == NULL ) return;
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1102, NULL, NULL);			
	
}

void CInternetNavigator::OnbtnSTOP() 
{	
	if ( m_pView == NULL ) return;
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1103, NULL, NULL);					
}

void CInternetNavigator::OnbtnHELP() 
{	
	if ( m_pView == NULL ) return;
	if ( m_pView->GetSafeHwnd() != NULL ) OnNavi();
}

void CInternetNavigator::OnbtnLOCK() 
{	
	if ( m_pView == NULL ) return;
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+99, NULL, NULL);	
}

VOID CInternetNavigator::SetFrameWnd(CFrameWnd* pFrameWnd)
{
	m_pFrameWnd = pFrameWnd;	
}

VOID CInternetNavigator::SetParentView(CView *pView)
{
	m_pView = pView;
}


//++2008.06.30 ADD BY CJY {{++
// �߰��� ����
// 1. �ּ�ǥ���� ����Ʈ�ڽ����� Ȯ���޺��ڽ��� ��ȯ
// 2. ������Ʈ������ �湮�� ������Ʈ ����� �޾ƿͼ� �޺��ڽ��� �߰�
// 3. �޺��ڽ� ������ ���ý� �ش� ����Ʈ�� �̵�
// 4. ������ �߰�
// 5. ����ڰ� ����Ű�� ����Ʈ �湮�� ������Ʈ���� �湮�� ������Ʈ ���
// 6. �޺��ڽ��� �ִ� ���� �ڵ��ϼ� ���

// �Լ��̸�: CInternetNavigator::GetURLHistoryFromRegstry
// �Լ�����: ������Ʈ���κ��� url����� �о�´�.
// ��ȯ  ��: INT 
// ����[CString *strResult[]] : �޾ƿ� ���� �迭
// ����[INT nGetURLCount] : �޾ƿ� ���� ����
// ����[INT nStart] : ������Ʈ���� �ִ� url1���� url25�������� �� �� �� ��°������ �޾ƿ� ������ ����
INT CInternetNavigator::Get_URLHistoryFromRegstry(CString* strResult, INT nGetURLCount, INT nStart)
{
	// ������ �߸��� ��� ����
	if(NULL == strResult) return -1;
	if(1 > nGetURLCount || nGetURLCount > MAX_URL_COUNT) return -2;
	if(0 > nStart || nStart > MAX_URL_COUNT) return -3;
	if(MAX_URL_COUNT+1 < nGetURLCount + nStart) return -4;

	HKEY hKey = NULL; 
	// Key�� Open�ϰ� key���� �޾Ƴ���. 
	LONG lResult = RegOpenKeyExW(
		HKEY_CURRENT_USER, 
		_T("Software\\Microsoft\\Internet Explorer\\TypedURLs"),
		0,
		KEY_QUERY_VALUE,
		&hKey
		); 

	// Open�� ������ �ƴ� ���
	if(ERROR_SUCCESS != lResult)
	{
		RegCloseKey(hKey);
		return -5;
	}

	CString strValueName = _T("");// ���� ���� �̸�	
	DWORD dwType = REG_SZ;  // ã�°��� TYPE 
	DWORD dwCount =0;//_MAX_PATH;  // ��ȯ�� ���� ���� 	
	for(INT nCount=0; nCount<nGetURLCount; nCount++)
	{ 
		strValueName.Format(_T("url%d"), nStart+nCount);
				
		// dwCount�� �����͸� �޾ƿ��� ��, �ʿ��� ũ�Ⱑ ���´�.
		TCHAR* pszValue = NULL;
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
		
		if(ERROR_SUCCESS != lResult)
		{
			RegCloseKey(hKey);
			delete[] pszValue;
			return -6;
		}
		else
		{			
			strResult[nCount] = pszValue;
			delete[] pszValue;
		}
	}

	RegCloseKey(hKey);

	return 0;
}

//--2008.06.30 ADD BY CJY --}}
//++2008.07.01 ADD BY CJY {{++

// CInternetFile/CInternetSession�� ���
#include <afxinet.h>

// �Լ��̸�: GetIconPath
// �Լ�����: �ش� ������Ʈ�� �ҽ��� ���� �������� ��ġ��θ� ��� ��ȯ�Ѵ�.
// ��ȯ  ��: INT 
// ����[CString strURL] : 
// ����[CString* strIconPath] : 
INT CInternetNavigator::Get_IconPathFromURLSourceFile(CString strURL, CString* strIconPath, CString* strIconFileName)
{	
	if(NULL == strIconPath || NULL == strIconFileName) return -1;

	strURL.TrimLeft();
	if(0 != strURL.Find(_T("http://"))) return -2;

	// 2009.02.25 ADD BY PDJ : �ӽ÷� ó����. �������� �󼼺���� ������ ����.
	CString strMainUrl;
	INT		nOffset;
	nOffset = strURL.Find(_T("/"), 7);
	strMainUrl = strURL.Left(nOffset);
	strMainUrl.MakeUpper();
	//if ( ( strMainUrl.Find(_T("KYOBOBOOK")) >= 0 || strMainUrl.Find(_T("INTERPARK")) >= 0) && strURL.GetLength() > 40 ) return 0;	
	//

	
	TCHAR* szUTmp = NULL;	
	char szData[20000];
	// 2009.05.11 REM BY PDJ : ���� ���ѷ��� �� �޸� ��
	
	// URL�� �ҽ������� �о�ͼ� �� �� ������������ ���´�.
//  	try
//	{
//		CInternetSession session;
//		CInternetFile *file = (CInternetFile*)session.OpenURL(strURL);
//		
//		file->SetReadBufferSize(20000);		
//		file->ReadHuge(szData, 20000);		
//	}
//	catch(...)
//	{
//		return GetLastError();
//	}
	
	
	try
	{
		CInternetSession session;
		CInternetFile *file = (CInternetFile*)session.OpenURL(strURL);

		DWORD dwSize=file->GetLength();// ������ ũ��
		
		file->SetReadBufferSize(dwSize);		
		file->Read(szData, dwSize);		
	}
	catch(...)
	{
		return GetLastError();
	}

	CharConvert cv;	
	CString strData;
	INT len;
	while(TRUE)
	{
	// ansi�� �о���� ������ �����ڵ�� ��ȯ�Ѵ�.
		if(0 == cv.MultiByteToUnicode(szData,&szUTmp,&len))
		{			
			strData = szUTmp;				
			delete [] szUTmp;
		}
		
		INT nIndex = strData.Find(_T("shortcut icon"));
		if(0 > nIndex) break;
		
		nIndex = strData.Find(_T("href"), nIndex+1);
		if(0 > nIndex) break;
		
		// icon�� �ּҰ� ���۵Ǵ� ����
		nIndex = strData.Find(_T("\""), nIndex+1);
		if(0 > nIndex) break;
		INT nFirst = nIndex+1;

		// icon�� �ּҰ� ������ ����
		nIndex = strData.Find(_T("\""), nIndex+1);
		if(0 > nIndex) break;
		INT nEnd = nIndex;

		// icon�� �ּ�
		strData = strData.Mid(nFirst, nEnd-nFirst);		
		
		nIndex = strData.ReverseFind('/');
		if(0 > nIndex) break;

		*strIconPath = strData.Left(nIndex);
		*strIconFileName = strData.Mid(nIndex+1);

		return 0;
	}

	INT nIndex = strURL.Find(_T("//"));
	nIndex = strURL.Find(_T("/"), nIndex+2);

	if(0 <= nIndex)
	{
		*strIconPath = strURL.Left(nIndex);
		*strIconFileName = _T("favicon.ico");
	}
	else return -1;
	return 0;
}


// �Լ��̸�: CInternetNavigator::OnSelendokcmbURL
// �Լ�����: ����ڰ� �޺��ڽ���Ͽ��� �������� Ŭ���Ͽ��� ���
// ��ȯ  ��: void 
void CInternetNavigator::OnSelendokcmbURL()
{
	// TODO: Add your control notification handler code here	
 	INT nIndex = m_cmbURL.GetCurSel();
 	if(0>nIndex) return;
	
 	CString strText = _T("");
 	m_cmbURL.GetLBText(nIndex, strText);
  	m_cmbURL.SetCurSel(nIndex);

 	SetURL(strText);

//DEL 	CString strFilePath = ICON_FILE_PATH _T("//") + Get_IconFileName(nIndex);
//DEL 	CFileFind file;
//DEL 	if(FALSE == file.FindFile(strFilePath))
	if(FALSE == Is_IconFile(nIndex))
	{
		// 2009.05.11 REM BY PDJ : �ȵǴ� ����Ʈ ����
		//Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(nIndex));
		//Modify_ImageToImageList(nIndex);
	}

  	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1010, NULL, NULL);
}


// �Լ��̸�: CInternetNavigator::OnEditchangecmbURL
// �Լ�����: �޺��ڽ��� ����Ʈ�ڽ��� ���ڰ� ����Ǿ��� ���
// ��ȯ  ��: void 
void CInternetNavigator::OnEditchangecmbURL() 
{
	// TODO: Add your control notification handler code here

//DEL  	����Ʈ�ڽ��� ����� ���� ����ڰ� �ּ�ǥ���ٿ� ���ڸ� �Է��ϰų�
//DEL  	�޺��ڽ��� �������� �����Ͽ��� ���� ��,
//DEL  	����ڰ� ���ڸ� �Է��Ͽ������� �ڵ��ϼ������ �����ְ� �Ѵ�.
 	if(TRUE == m_bIsKeyDown) // m_bIsKeyDown�� Pretranslate���� �����Ѵ�.
 	{		
 		CString strEditText = _T("");
 		m_cmbURL.GetWindowText(strEditText);
 		m_List.ShowAutoCompleteList(strEditText); 
 	}
}

//--2008.07.01 ADD BY CJY --}}

//++2008.07.07 ADD BY CJY {{++


// �Լ��̸�: CInternetNavigator::Download_IconFile
// �Լ�����: url��ο��� ���������� ��θ� ã�� �ٿ�޴´�.
// ��ȯ  ��: INT 
// ����[CString strURL] : �������� �޾ƿ� url���
// ����[CString strFilePath] : ������������ ������ ���ϰ��
// ����[CString strFileName] : ���������� �̸�
INT CInternetNavigator::Download_IconFile(CString strURL, CString strFilePath, CString strFileName)
{
	::DeleteFile(strFilePath + _T("\\") + strFileName);
	
	CString strGetIconPath = _T("");
	CString strGetIconName = _T("favicon.ico");
	Get_IconPathFromURLSourceFile(strURL, &strGetIconPath, &strGetIconName);
	
	if(_T("") == strGetIconPath) strGetIconPath = strURL;
	
	// ��ΰ� ����̺��ΰ��'c:\', 'd:\', 
	if(0 <= strGetIconPath.Find(_T(":\\"))) return -1;

	// ���� ��� ����
	if(_T("") == strFilePath)
	{
		TCHAR tmp[256];
		ZeroMemory(tmp, 256);
		::GetCurrentDirectory(256, tmp);
		strFilePath = tmp;
	}    

	strURL = strGetIconPath + _T("/") + strGetIconName;
	CString strFile = strFilePath + _T("/") + strFileName;
	
	try
	{
		HRESULT hr = URLDownloadToFile(
			NULL,      // ptr to ActiveX container
			strURL,      // URL to get
			strFile,     // file to store data in
			0,         // reserved
			NULL //&callback  // ptr to IBindStatusCallback
			);
		if(!SUCCEEDED(hr)) return -1;

		return 0;
	}
	catch(...)
	{
		return GetLastError();
	}	

	return -1;
}


// �Լ��̸�: CInternetNavigator::Add_ItemFromHistroyArray
// �Լ�����: �����丮 ���ڿ����� ���ڿ��� ������ �޺��ڽ��� �߰��Ѵ�.
// ��ȯ  ��: INT 
// ����[INT nIndex] : 
INT CInternetNavigator::Add_ItemFromHistroyArray(INT nIndex)
{
	if(0 > nIndex || nIndex >= m_aURLHistory.GetSize()) return -1;
	if(_T("") == m_aURLHistory.GetAt(nIndex)) return -2;

	COMBOBOXEXITEM Item;
	Item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE;
	Item.iItem = nIndex;
	Item.iImage = nIndex;
	Item.iSelectedImage = nIndex;
	Item.pszText = (TCHAR*)(LPCTSTR)m_aURLHistory.GetAt(nIndex);
	
	m_cmbURL.InsertItem(&Item);	
	
	return 0;
}


// �Լ��̸�: CInternetNavigator::Add_AllItemFromHistroyArray
// �Լ�����: ���� �湮�ѻ���Ʈ �迭�� �ִ� ��� ������ �޺��ڽ��� ���������� �߰��Ѵ�.
// ��ȯ  ��: INT 
INT CInternetNavigator::Add_AllItemFromHistroyArray()
{
	for(INT nCount=0; nCount < MAX_URL_COUNT; nCount++)
	{
		Add_ItemFromHistroyArray(nCount);
	}

	return 0;
}


// �Լ��̸�: CInternetNavigator::Reset_ItemList
// �Լ�����: �޺��ڽ��� �����۰� �̹�������Ʈ�� �ʱ�ȭ�Ѵ�
// ��ȯ  ��: INT 
INT CInternetNavigator::Reset_ItemList()
{
	m_cmbURL.ResetContent();
	const INT MAX = m_aImageList.GetImageCount();
	for(INT nCount=0; nCount < MAX; nCount++)
		m_aImageList.Remove(0);

	return 0;
}


// �Լ��̸�: CInternetNavigator::Set_URLHistoryToRegstry
// �Լ�����: 
// ��ȯ  ��: INT 
// ����[CString *strPutData] : ���ڹ迭
// ����[INT nSetURLCount] : ������ ����
// ����[INT nStart] : ���۹�ȣ 0������ ����
INT CInternetNavigator::Set_URLHistoryToRegstry(CString *strPutData, INT nSetURLCount, INT nStart)
{
	if(NULL == strPutData) return -1;
	if(1 > nSetURLCount || nSetURLCount > MAX_URL_COUNT) return -2;
	if(0 > nStart || nStart > MAX_URL_COUNT) return -3;
	if(MAX_URL_COUNT+1 < nSetURLCount + nStart) return -4;
		
	HKEY hKey = NULL; 
	// Key�� Open�ϰ� key���� �޾Ƴ���. 
	LONG lResult = RegOpenKeyExW(HKEY_CURRENT_USER, 
		_T("Software\\Microsoft\\Internet Explorer\\TypedURLs"), 0, KEY_ALL_ACCESS, &hKey); 
	// Open�� ������ �ƴ� ���
	if(ERROR_SUCCESS != lResult) return -5;

	CString strValueName = _T("");// ���� ���� �̸�		
	DWORD dwCount =0;// ���� ���� 	
	for(INT nCount=0; nCount<nSetURLCount; nCount++)
	{ 
		strValueName.Format(_T("url%d"), nStart+nCount);
		if(_T("") == strPutData[nCount]) break;

		// ����� ũ��
		DWORD dwCount = ( sizeof(TCHAR)*(strPutData[nCount].GetLength()+1) );
		TCHAR* pszValue = strPutData[nCount].GetBuffer(strPutData[nCount].GetLength());

		lResult = RegSetValueEx(
			hKey,
			strValueName,	// �� �̸�
			NULL,
			REG_SZ,		// ���� ����
			(LPBYTE)pszValue,	// ����� ��
			dwCount			// ����� ���� ����
			);
 
		if(ERROR_SUCCESS != lResult) return -6;
	}
	RegCloseKey(hKey);
	return 0;
}

// �Լ��̸�: CInternetNavigator::Is_AlreadyInsertHistoryArray
// �Լ�����: �ش繮�ڿ��� �����丮 �迭�� ����Ǿ� �ִ��� ���� �Ǵ�
// ��ȯ  ��: BOOL 
// ����[CString strData] : 
BOOL CInternetNavigator::Is_AlreadyInsertHistoryArray(CString strData)
{
	if(0<= Get_IndexFromHistoryArray(strData)) return TRUE;
	return FALSE;
}


// �Լ��̸�: CInternetNavigator::OnDropdowncmbURL
// �Լ�����: ����ڰ� �޺��ڽ��� ����� ������ ���
// ��ȯ  ��: void 
void CInternetNavigator::OnDropdowncmbURL() 
{
	// TODO: Add your control notification handler code here
	// �ڵ��ϼ� ����Ʈ�ڽ��� �����
	m_List.ShowWindow(SW_HIDE);
	// �޺��ڽ��� ��� ������ �ʱ�ȭ
   	Reset_ItemList();
	// �̹�������Ʈ�� �������� ����
 	Add_AllIconToImageList();
	// �������� �߰�
	Add_AllItemFromHistroyArray();

	m_cmbURL.Set_DrawIconToEdit(TRUE);
}


// �Լ��̸�: CInternetNavigator::Get_IconFileName
// �Լ�����: Ư�� �ε����� �����������̸��� ��ȯ�Ѵ�
// ��ȯ  ��: CString 
// ����[INT nIndex] : 
CString CInternetNavigator::Get_IconFileName(INT nIndex)
{
	CString strIndex = _T("");
	strIndex.Format(_T("url%d.ico"), nIndex+1);
	return strIndex;
}


// �Լ��̸�: CInternetNavigator::Add_IconToImageList
// �Լ�����: ������ ���� ��ο��� �ش������������ �о�� ����Ʈ�� �߰��Ѵ�.
// ��ȯ  ��: INT 
// ����[CString strIconName] : 
INT CInternetNavigator::Add_IconToImageList(CString strIconName)
{
	CString strFile = ICON_FILE_PATH;
	strFile += _T("\\") + strIconName;
	
	HICON hIcon = (HICON)::LoadImage(
		NULL,
		(LPCTSTR)strFile,
		IMAGE_ICON,
		0,
		0,
		LR_LOADFROMFILE
		);
	
	INT nCount = m_aImageList.GetImageCount();
	if(0>m_aImageList.Add(hIcon))
	{			 
		m_aImageList.Add(m_hDefaultIcon);
	}
	nCount = m_aImageList.GetImageCount();
	return 0;
	
}


// �Լ��̸�: CInternetNavigator::Add_AllIconToImageList
// �Լ�����: ���������ϰ�ο��� ��� ������������ �ҷ��� ����Ʈ�� �߰��Ѵ�.
// ��ȯ  ��: INT 
INT CInternetNavigator::Add_AllIconToImageList()
{
	CString strIconName = _T("");
	for(INT nCount=0; nCount < m_aURLHistory.GetSize(); nCount++)
	{
		strIconName = Get_IconFileName(nCount);
		Add_IconToImageList(strIconName);
	}
	return 0;
}


// �Լ��̸�: CInternetNavigator::Modify_ImageToImageList
// �Լ�����: �̹�������Ʈ�� Ư�� �ε����� �̹����� �ٽ� �ҷ��´�.
// ��ȯ  ��: INT 
// ����[INT nIndex] : 
INT CInternetNavigator::Modify_ImageToImageList(INT nIndex)
{
	CString strIconName = Get_IconFileName(nIndex);
	
	CString strFile = ICON_FILE_PATH;
	strFile += _T("\\") + strIconName;
	
	HICON hIcon = (HICON)::LoadImage(
		NULL,
		(LPCTSTR)strFile,
		IMAGE_ICON,
		0,
		0,
		LR_LOADFROMFILE
		);
	if(NULL != hIcon)
	{
		HICON hPrevIcon = m_aImageList.ExtractIcon(nIndex);
		HICON hNewIcon = hIcon;
		CString strURL = m_aURLHistory.GetAt(nIndex);

		TRACE(_T("REPLACE ICON : %d -> %d (Default = %d) %s\r\n"),
			hPrevIcon, hNewIcon, m_hDefaultIcon, strURL
			);

		m_aImageList.Replace(nIndex, hIcon);
	}
	return 0;
}


// �Լ��̸�: CInternetNavigator::Get_IndexFromHistoryArray
// �Լ�����: ���� �湮��������Ʈ �迭���� Ư�� ���ڿ��� ���Ե� �ε����� ��ȯ�Ѵ�
// ��ȯ  ��: INT 
// ����[CString strData] : 
INT CInternetNavigator::Get_IndexFromHistoryArray(CString strData)
{
	const INT MAX = m_aURLHistory.GetSize();
	for(INT nCount=0; nCount<MAX; nCount++)
	{
		if(strData == m_aURLHistory.GetAt(nCount)) return nCount;
	}

	return -1;
}



// �Լ��̸�: CInternetNavigator::OnMove
// �Լ�����: 
// ������ �̵��� �ڵ��ϼ�����Ʈ�� ��ġ�� ���� �̵�
// ������������ OnMove�Լ����� �� ������ SendMessage�� ������.
// ��ȯ  ��: void 
// ����[int x] : 
// ����[int y] : 
void CInternetNavigator::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	
	// �޺��ڽ� ũ������
	if(NULL != m_cmbURL.GetSafeHwnd())
	{
		CRect rect;
		m_cmbURL.GetWindowRect(&rect);
		// �ڵ��ϼ�����Ʈ ũ������
		rect.top = rect.bottom;//rect.bottom;	
		rect.left = rect.left+25;
		rect.right = rect.right-20;
		rect.bottom = rect.top+100;//rect.top + 400;
		m_List.MoveWindow(rect); // m_List�� ���̾�α�		
	}
}



// �Լ��̸�: CInternetNavigator::OnNotify
// �Լ�����: �ڵ��ϼ� ����Ʈ���� �������� ���õǾ��� ��� SendMessage�� ����� ��������.
// ��ȯ  ��: BOOL 
// ����[WPARAM wParam] : ���� ���� id��
// ����[LPARAM lParam] : 
// ����[LRESULT* pResult] : 
BOOL CInternetNavigator::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class

	// �ڵ��ϼ� ����Ʈ���� ����ڰ� �������� �����Ͽ��� ���
	if(CAutoCompleteList::IDD == wParam)
	{
		NMHDR* pHdr = (NMHDR*)lParam;
		if(LBN_SELCHANGE == pHdr->code)
		{			
			CString strData = m_List.Get_CurSelText();
			m_cmbURL.SetWindowText(strData);
			INT nLength = strData.GetLength();

			CEdit* pEdit = m_cmbURL.GetEditCtrl();
			pEdit->SetSel(-1,-1);
		}
		else if(VK_RETURN == pHdr->code)
		{
			PostMessage(WM_KEYDOWN, VK_RETURN);
		}
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}


// �Լ��̸�: CInternetNavigator::Is_IconFile
// �Լ�����: �ϵ��ũ�󿡼� Ư�� �ε����� ������������ ���翩�� �Ǵ�
// ��ȯ  ��: BOOL 
// ����[INT nIndex] : 
BOOL CInternetNavigator::Is_IconFile(INT nIndex)
{
	CString strFilePath = ICON_FILE_PATH _T("//") + Get_IconFileName(nIndex);
	CFileFind file;
	return file.FindFile(strFilePath);
}

//--2008.07.07 ADD BY CJY --}}

//++2008.07.08 ADD BY CJY {{++
// �Լ��̸�: CInternetNavigator::Put_Item
// �Լ�����: ���ڷ� ���� �ؽ�Ʈ�� �̹� �޺��ڽ� ����Ʈ�� ������ �ش� ����Ʈ�� �ε����� ��ȯ�ϰ�,
//		������ �޺��ڽ��� �߰���Ų��. 
// ��ȯ  ��: INT 
// ����[CString strText] : 
// ����[INT *pGetIndex] : 
INT CInternetNavigator::Put_Item(CString strText, INT *pGetIndex, BOOL bReDownload_IconFile)
{
	// ��Ͽ� ���� �����̸� �߰��Ѵ�.
	INT nTextIndex = Get_IndexFromHistoryArray(strText);
	if(0 > nTextIndex)
	{
		
		const INT nSize = m_aURLHistory.GetSize();
		if(MAX_URL_COUNT == nSize)
		{ 
			//DEL 		// ����� ������ ��á���� url 2���� url max��������
			//DEL 		// url1���� url max-1������ �ű� ��
			//DEL 		// �� ������ url max�� ����
			//DEL 		CString strData[MAX_URL_COUNT];
			//DEL 		Get_URLHistoryFromRegstry(strData, MAX_URL_COUNT-1, 2);
			//DEL 		strData[MAX_URL_COUNT-1] = strText;
			//DEL 		Set_URLHistoryToRegstry(strData, MAX_URL_COUNT, 1);
			
			// url����� ����
			m_aURLHistory.RemoveAt(0);
			m_aURLHistory.Add(strText);
			
 			// icon���� ���� �� ���� ������ ����Ʈ ������ �ٿ�ε�
 			CString strFile = ICON_FILE_PATH;
 			strFile += _T("\\");
 			strFile += Get_IconFileName(0);
			
 			::DeleteFile(strFile);
			
 			for(INT nCount=1; nCount <= MAX_URL_COUNT; nCount++)
 			{ 	
 				CString strFilePath = ICON_FILE_PATH _T("\\");
 				CString strOldFilePath = strFilePath + Get_IconFileName(nCount);
 				CString strNewFilePath = strFilePath + Get_IconFileName(nCount-1);
 				CFileFind filefind;
 				CFile file;
 				if(TRUE == filefind.FindFile(strOldFilePath))
 				{
 					file.Rename(strOldFilePath, strNewFilePath);
 				}			
 				
 			}
			// ���� ������ ����Ʈ�� �������� �ٿ�޾�
			// �̹��� ����Ʈ�� �߰��ѵ�
			// �޺��ڽ� ����Ʈ���� �߰��Ѵ�.
			Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(MAX_URL_COUNT-1));
			m_aImageList.Remove(0);
			Add_IconToImageList(Get_IconFileName(MAX_URL_COUNT-1));
			
			m_cmbURL.DeleteItem(0);
			Add_ItemFromHistroyArray(MAX_URL_COUNT-1);
			
			if(NULL != pGetIndex) *pGetIndex = MAX_URL_COUNT-1;
		}				
		// ����� ������ ����������
		// url nCount+1���� ����
		else if(MAX_URL_COUNT > nSize)
		{
			//DEL 		Set_URLHistoryToRegstry(&strText, 1, nSize+1);
			m_aURLHistory.Add(strText);
			Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(nSize));
			Add_IconToImageList(Get_IconFileName(nSize));
			Add_ItemFromHistroyArray(nSize);
			
			if(NULL != pGetIndex) *pGetIndex = nSize;
		}
		else return -1;

		m_List.Add_AutoCompleteData(strText);
	}
	else // �̹� ��Ͽ� �ִ� ����Ʈ�� ���
	{		
		// ������������ ���� ��� ���� �ٿ�޴´�.				
		if(FALSE == Is_IconFile(nTextIndex))
		{
			if(TRUE == bReDownload_IconFile)
			{
				Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(nTextIndex));
				Modify_ImageToImageList(nTextIndex);
			}

			if(NULL != pGetIndex) *pGetIndex = nTextIndex;
		}
	}	
	
	return 0;
}


// �Լ��̸�: CInternetNavigator::Make_URL
// �Լ�����: 
// ��ȯ  ��: CString 
// ����[CString strURL] : 
CString CInternetNavigator::Make_URL(CString strURL) const
{
	strURL.TrimLeft();
	if(-1 == strURL.Find(_T("://")) && _T("") != strURL)
	{
		strURL = _T("http://") + strURL;
	}
	// 17/02/06 ������ : KOLAS3.net Search�� ���� �˻��� 404 ���� �߻�
/*//REM--------------------------------------------------------------------------
	if(strURL.GetLength()-1 != strURL.ReverseFind('/') && _T("") != strURL)
	{
		strURL += _T("/");
	}
*///END--------------------------------------------------------------------------	

	return strURL;
}
//--2008.07.08 ADD BY CJY --}}
//DEL LRESULT CInternetNavigator::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL 	if(TRUE == m_bDrawIcon && NULL != m_cmbURL.GetSafeHwnd())
//DEL 	{		
//DEL 		CDC* pDC = m_cmbURL.GetDC();
//DEL 		DrawIconEx(
//DEL 			pDC->m_hDC,
//DEL 			3,
//DEL 			3,
//DEL 			m_hDefaultIcon,
//DEL 			16,
//DEL 			16,
//DEL 			NULL,
//DEL 			(HBRUSH)RGB(255, 255, 255),
//DEL 			DI_NORMAL
//DEL 			);
//DEL 		ReleaseDC(pDC);
//DEL 	}	
//DEL 	return CDialog::WindowProc(message, wParam, lParam);
//DEL }

// 2008.11.10 ADD BY PWR
// ���̾�α� XP�׸�����
VOID CInternetNavigator::EnableThemeDialogTexture(HWND hwndDlg)
{
	HINSTANCE hDll=LoadLibrary(_T("UxTheme.dll"));	// 'UxTheme.dll'
	if(hDll==NULL)
		return;	// the DLL won't be available on anything except Windows XP
	ULONG (PASCAL *lpfnEnableTheme)(HWND, DWORD);
	(FARPROC&)lpfnEnableTheme=GetProcAddress(hDll,"EnableThemeDialogTexture");	// 'EnableThemeDialogTexture'
	if(lpfnEnableTheme)
		lpfnEnableTheme(hwndDlg, 6);	// ETDT_ENABLETAB = (ETDT_ENABLE | ETDT_USETABTEXTURE) = (2|4) = 6
	else ASSERT(FALSE);
	FreeLibrary(hDll);
}


VOID CInternetNavigator::OnNavi()
{
	CWnd* CHelpEdit = (CEdit*) GetDlgItem(IDC_edtHELP);
	CWnd* pHelpBtn = (CButton*) GetDlgItem(IDC_btnHELP);
	CWnd* pWnd = GetFocus();
	
	if ( pWnd == CHelpEdit || pWnd == pHelpBtn)
	{
		CString strHelpURL;
		CString strHelpText;		
		((CEdit*) GetDlgItem (IDC_edtHELP))->GetWindowText(strHelpText);
		strHelpText.Replace(_T(" "), _T("+"));		
		// 17/02/06 ������ : KOLAS3.net Search�� ���� �˻��� 404 ���� �߻�
/*//BEFORE-----------------------------------------------------------------------------
		strHelpURL.Format( _T("http://www.kolas3.net/bbs/board.php?bo_table=faq&sfl=wr_subject%%7C%%7Cwr_content&stx=%s&sop=and"), strHelpText);		
*///AFTER------------------------------------------------------------------------------
		CString strTmp;	//URL ���ڵ�
		strTmp = CLocCommonAPI::UrlEncode((LPCTSTR)strHelpText);
		strHelpURL.Format( _T("http://kolas.nl.go.kr/nltech/plib/faq/faqList.do?bbCategory=1&searchCondition=bb_content&searchKeyword=%s"), strTmp);
/*//END-------------------------------------------------------------------------------*/
		SetURL(strHelpURL);
		m_cmbURL.SetWindowText(strHelpURL);					
	}
	
	m_List.ShowWindow(SW_HIDE);				
	CString strText = _T("");
	m_cmbURL.GetWindowText(strText);
	
	SetURL(strText, TRUE);
	//DEL	
	//DEL 				// ��Ͽ� ���� �����̸� ������Ʈ���� �߰��Ѵ�.
	//DEL 				INT nTextIndex = Get_IndexFromHistoryArray(strText);
	//DEL 				if(0 > nTextIndex)
	//DEL 				{		
	//DEL 					const INT nSize = m_aURLHistory.GetSize();
	//DEL 					if(MAX_URL_COUNT == nSize)
	//DEL 					{ 
	//DEL 						// ����� ������ ��á���� url 2���� url max��������
	//DEL 						// url1���� url max-1������ �ű� ��
	//DEL 						// �� ������ url max�� ����
	//DEL  						CString strData[MAX_URL_COUNT];
	//DEL 						Get_URLHistoryFromRegstry(strData, MAX_URL_COUNT-1, 2);
	//DEL 						strData[MAX_URL_COUNT-1] = strText;
	//DEL 						Set_URLHistoryToRegstry(strData, MAX_URL_COUNT, 1);
	//DEL 						
	//DEL 						// url����� ����
	//DEL 						m_aURLHistory.RemoveAt(0);
	//DEL 						m_aURLHistory.Add(strText);
	//DEL 						
	//DEL 						// icon���� ���� �� ���� ������ ����Ʈ ������ �ٿ�ε�
	//DEL 						CString strFile = ICON_FILE_PATH;
	//DEL 						strFile += _T("\\");
	//DEL 						strFile += Get_IconFileName(0);
	//DEL 						
	//DEL 						::DeleteFile(strFile);
	//DEL 						
	//DEL 						for(INT nCount=1; nCount <= MAX_URL_COUNT; nCount++)
	//DEL 						{ 	
	//DEL 							CString strFilePath = ICON_FILE_PATH _T("\\");
	//DEL 							CString strOldFilePath = strFilePath + Get_IconFileName(nCount);
	//DEL 							CString strNewFilePath = strFilePath + Get_IconFileName(nCount-1);
	//DEL 							CFileFind filefind;
	//DEL 							CFile file;
	//DEL 							if(TRUE == filefind.FindFile(strOldFilePath))
	//DEL 							{
	//DEL 								file.Rename(strOldFilePath, strNewFilePath);
	//DEL 							}			
	//DEL 							
	//DEL 						}
	//DEL 						// ���� ������ ����Ʈ�� �������� �ٿ�޾�
	//DEL 						// �̹��� ����Ʈ�� �߰��ѵ�
	//DEL 						// �޺��ڽ� ����Ʈ���� �߰��Ѵ�.
	//DEL 						Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(MAX_URL_COUNT-1)); 
	//DEL 						m_aImageList.Remove(0);
	//DEL 						Add_IconToImageList(Get_IconFileName(MAX_URL_COUNT-1));
	//DEL 						
	//DEL 						m_cmbURL.DeleteItem(0);
	//DEL 						Add_ItemFromHistroyArray(MAX_URL_COUNT-1);//MAX_URL_COUNT-1);
	//DEL 
	//DEL 
	//DEL 					//DEL 	m_cmbURL.SetCurSel(MAX_URL_COUNT-1);
	//DEL 					}				
	//DEL 					// ����� ������ ����������
	//DEL 					// url nCount+1���� ����
	//DEL 					else if(MAX_URL_COUNT > nSize)
	//DEL 					{
	//DEL 						Set_URLHistoryToRegstry(&strText, 1, nSize+1);
	//DEL 						m_aURLHistory.Add(strText);
	//DEL 						Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(nSize));
	//DEL 						Add_IconToImageList(Get_IconFileName(nSize));
	//DEL 						Add_ItemFromHistroyArray(nSize);
	//DEL 
	//DEL 					//DEL 	m_cmbURL.SetCurSel(nSize);
	//DEL 					}
	//DEL 					else return TRUE;
	//DEL 				}
	//DEL 				else // �̹� ��Ͽ� �ִ� ����Ʈ�� ���
	//DEL 				{					
	//DEL 					
	//DEL 					// ������������ ���� ��� ���� �ٿ�޴´�.				
	//DEL 					if(FALSE == Is_IconFile(nTextIndex))
	//DEL 					{
	//DEL 						Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(nTextIndex));
	//DEL 					 	Modify_ImageToImageList(nTextIndex);
	//DEL 					}
	//DEL 				}
	m_List.Set_Visible(TRUE); 							
	
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1010, NULL, NULL);				
	
 			
}
void CInternetNavigator::OnKillfocusedtHELP() 
{

	CString strText;
	CStatic *m_HelpMessage;		
	m_HelpMessage = (CStatic*)GetDlgItem(IDC_HELP_MESSAGE);

	( (CEdit*) GetDlgItem(IDC_edtHELP) )->GetWindowText(strText);

	if ( strText.IsEmpty() )
	{
		m_HelpMessage->ShowWindow(SW_SHOW);
	}
	else
	{
		m_HelpMessage->ShowWindow(SW_HIDE);
	}		
}

void CInternetNavigator::OnSetfocusedtHELP() 
{	
	CStatic *m_HelpMessage;		
	m_HelpMessage = (CStatic*)GetDlgItem(IDC_HELP_MESSAGE);
	m_HelpMessage->ShowWindow(SW_HIDE);
}
