// CheckinVolResouceMgr.cpp: implementation of the CCheckinVolResouceMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckinVolResouceMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckinVolResouceMgr::CCheckinVolResouceMgr(CESL_Mgr *pParentESLMgr /*= NULL */)
	:m_dlgSpeciesInfo(pParentESLMgr)
{
	m_pFrameWnd = NULL;
	m_pParentESLMgr = pParentESLMgr;

	m_nResourceID = 31001;

	// default bitmap
	m_strBitmapNormalPath	= _T("..\\bmp\\normal.bmp");
	m_strBitmapHotPath		= _T("..\\bmp\\hot.bmp");
	m_strBitmapDisabledPath	= _T("..\\bmp\\disabled.bmp");

}

CCheckinVolResouceMgr::~CCheckinVolResouceMgr()
{
	FreeResourceList();
}

INT CCheckinVolResouceMgr::CreateResource(CFrameWnd *pFrameWnd)
{
	m_pFrameWnd = pFrameWnd;
	
	InitMenu();
	InitToolBar();
		
	CreateMenu();
	CreateToolBar();
	CreateDialogBar();

	m_pFrameWnd->RecalcLayout();

	return 0;
}

INT CCheckinVolResouceMgr::InitMenu()
{
	INT nIdx = 0;

	CreateMenuItem(nIdx, _T("����(&V)"), _T("VIEW_IDD_CHECKIN_VOL_SPECIES"), _T("������ ����"));
	
	return 0;
}

INT CCheckinVolResouceMgr::InitToolBar()
{
	INT nIdx = 0;

	CreateToolBarItem(nIdx, _T("�Լ�ó��"), _T("�Լ�ó��"), _T("����, ��ȣ������ ���ؼ� �Լ�ó���� �մϴ�."),_T(""),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("��ȣó��"), _T("��ȣó��"), _T("���������� ���ؼ� ��ȣó���� �մϴ�."),_T(""),_T("..\\bmp\\349_on.bmp"),_T("..\\bmp\\349_on.bmp"),_T("..\\bmp\\349_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("��ȣ�Է�"), _T("���Է�"), _T("�������� ���� ��ȣ������ �Է��մϴ�."),_T(""),_T("..\\bmp\\350_on.bmp"),_T("..\\bmp\\350_on.bmp"),_T("..\\bmp\\350_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("��ȣ����"), _T("�Ǽ���"), _T("���õ� ��ȣ������ �����մϴ�."),_T(""),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("��ȣ����"), _T("�ǻ���"), _T("���õ� ��ȣ������ �����մϴ�."),_T(""),_T("..\\bmp\\352_on.bmp"),_T("..\\bmp\\352_on.bmp"),_T("..\\bmp\\352_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("�պ�ȣ"), _T("�պ�ȣ"), _T("���õ� �ΰ��� ��ȣ������ �պ�ȣ ó���մϴ�."),_T(""),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("�̹���"), _T("�̹���"), _T("���õ� ��ȣ������ �̹��� ó���մϴ�."),_T(""),_T("..\\bmp\\352_on.bmp"),_T("..\\bmp\\352_on.bmp"),_T("..\\bmp\\352_dimd.bmp"));	
	CreateToolBarItem(nIdx, _T("��������"), _T("��������"), _T("������������â�� ���ϴ�."),_T(""),_T("..\\bmp\\353_on.bmp"),_T("..\\bmp\\353_on.bmp"),_T("..\\bmp\\353_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("��������"), _T("��������"), _T("��������â�� ���ϴ�."),_T(""),_T("..\\bmp\\353_on.bmp"),_T("..\\bmp\\353_on.bmp"),_T("..\\bmp\\353_dimd.bmp"));
//  2005-10-11 Ȳ���� : ���� ���� ����
//	CreateToolBarItem(nIdx, _T("����"), _T("����"), _T("�۾��� �������� �ϰ� �����մϴ�."),_T(""),_T("..\\bmp\\343_on.bmp"),_T("..\\bmp\\343_on.bmp"),_T("..\\bmp\\343_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("���"), _T("���"), _T("�ش� ����Ʈ�� ����մϴ�."),_T(""),_T("..\\bmp\\155_on.bmp"),_T("..\\bmp\\155_on.bmp"),_T("..\\bmp\\155_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("�Լ���Ȳ"), _T("�Լ���Ȳ"), _T("���� �Լ���Ȳ�� ����մϴ�."),_T(""),_T("..\\bmp\\014_on.bmp"),_T("..\\bmp\\014_on.bmp"),_T("..\\bmp\\014_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("���ΰ�ħ"), _T("���ΰ�ħ"), _T("��� üũ�� ������ ���α����մϴ�."),_T(""),_T("..\\bmp\\354_on.bmp"),_T("..\\bmp\\354_on.bmp"),_T("..\\bmp\\354_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("�߰�����"), _T("�߰�����"), _T("�������� ������ ��ȣ�� �����մϴ�."),_T(""),_T("..\\bmp\\354_on.bmp"),_T("..\\bmp\\354_on.bmp"),_T("..\\bmp\\354_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("����"), _T("����"), _T("�ڵ� �߰��������ο� �߰����� ����, ���º� �׸��� ������ �����մϴ�."),_T(""),_T("..\\bmp\\354_on.bmp"),_T("..\\bmp\\354_on.bmp"),_T("..\\bmp\\354_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("������ȣ"), _T("������ȣ"), _T("������ȣ�� �����մϴ�."),_T(""),_T("..\\bmp\\298_on.bmp"),_T("..\\bmp\\298_on.bmp"),_T("..\\bmp\\298_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("ǥ������"), _T("ǥ������"), _T("ǥ�������� �Է��մϴ�."),_T(""),_T("..\\bmp\\ebook_on.bmp"),_T("..\\bmp\\ebook_on.bmp"),_T("..\\bmp\\ebook_off.bmp"));
	CreateToolBarItem(nIdx, _T("����"), _T("����"), _T("���� ���õ� �������� üũ�� ������ �����մϴ�."),_T(""),_T("..\\bmp\\345_on.bmp"),_T("..\\bmp\\345_on.bmp"),_T("..\\bmp\\345_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("����"), _T("����"), _T("���� ���õ� �������� üũ�� ������ �����մϴ�."),_T(""),_T("..\\bmp\\346_on.bmp"),_T("..\\bmp\\346_on.bmp"),_T("..\\bmp\\346_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("�ݱ�"), _T("�ݱ�"), _T("�Ǻ�üũ�� â�� �ݽ��ϴ�."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));

	

	return 0;
}

INT CCheckinVolResouceMgr::CreateToolBarItem(INT &nIdx, 
											 LPCTSTR lpszAlias, 
											 LPCTSTR lpszCaption, 
											 LPCTSTR lpszToolTip,
											 LPCTSTR lpszStyle, 
											 LPCTSTR lpszBitmapNormal,
											 LPCTSTR lpszBitmapHot,
											 LPCTSTR lpszBitmapDisabled
											 )
{
	
	CToolBarItem *pToolBarItem = new CToolBarItem;
	m_listToolBarItem.AddTail(pToolBarItem);
	pToolBarItem->m_nID = ++m_nResourceID;
	pToolBarItem->m_nIndex = nIdx++;
	pToolBarItem->m_strAlias = lpszAlias;
	pToolBarItem->m_strCaption = lpszCaption;
	pToolBarItem->m_ToolTip = lpszToolTip;
	pToolBarItem->m_strStyle = lpszStyle;
	

	pToolBarItem->m_strBitmapNormal = lpszBitmapNormal;
	pToolBarItem->m_strBitmapHot = lpszBitmapHot;
	pToolBarItem->m_strBitmapDisabled = lpszBitmapDisabled;
	

	return 0;
}

INT CCheckinVolResouceMgr::CreateToolBar()
{
	CToolBar *pToolBar = new CToolBar;
	m_listToolBar.AddTail(pToolBar);

	if (!pToolBar->CreateEx(m_pFrameWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
		CBRS_TOOLTIPS | CBRS_FLYBY))
	{
		delete pToolBar;
		return NULL;
	}

	pToolBar->SetWindowText(_T("��ȣ�� üũ�� ��������"));

	pToolBar->EnableDocking(CBRS_ALIGN_TOP);


	// Bitmap
	CBitmap bmpNormal;
	CBitmap bmpHot;
	CBitmap bmpDisabled;

	// Create ToolTip
	CToolTipCtrl *pToolTipCtrl = new CToolTipCtrl;
	pToolTipCtrl->Create(m_pFrameWnd, TTS_ALWAYSTIP);
	m_listToolTip.AddTail((CObject*)pToolTipCtrl);

	// Create ImageList
	CImageList * pImgListNormal = new CImageList;
	pImgListNormal->Create(BMP_SIZE, BMP_SIZE, ILC_COLOR24, 0, 4);
	
	CImageList * pImgListHot = new CImageList;
	pImgListHot->Create(BMP_SIZE, BMP_SIZE, ILC_COLOR24, 0, 4);
		
	CImageList * pImgListDisabled = new CImageList;
	pImgListDisabled->Create(BMP_SIZE, BMP_SIZE, ILC_COLOR24, 0, 4);

	m_listImageList.AddTail(pImgListNormal);
	m_listImageList.AddTail(pImgListHot);
	m_listImageList.AddTail(pImgListDisabled);

	// Init ToolBar
	CToolBarCtrl &rpToolBarCtrl = pToolBar->GetToolBarCtrl();
	if (!::IsWindow(rpToolBarCtrl.GetSafeHwnd())) return -1;
	while (rpToolBarCtrl.GetButtonCount() > 0)
		rpToolBarCtrl.DeleteButton(0);

	// Button Size
	INT nButtonWidth = GetButtonWidth();	
	nButtonWidth = nButtonWidth + USER_BSIZE_WIDTH;
	rpToolBarCtrl.SetButtonSize(CSize(nButtonWidth, USER_BSIZE_HIGHT));
		
	rpToolBarCtrl.SetMaxTextRows(2);
	rpToolBarCtrl.SetDrawTextFlags(DT_RIGHT, DT_WORDBREAK | DT_EDITCONTROL | DT_RIGHT);
	
	// Set ToolBar
	TCHAR szBuf[128];
	CToolBarItem *pToolBarItem = NULL;
	POSITION pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem.GetNext(pos));
		if (pToolBarItem == NULL) continue;
		
		TBBUTTON TBButton;
		
		TBButton.iBitmap	= pToolBarItem->m_nIndex;
		TBButton.idCommand	= pToolBarItem->m_nID;
		TBButton.fsState	= TBSTATE_ENABLED;
		TBButton.fsStyle	= TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
		_stprintf(szBuf, _T("%s\0"), pToolBarItem->m_strCaption);
		TBButton.iString	= rpToolBarCtrl.AddStrings(szBuf);
		TBButton.dwData		= 0;
		
		if (pToolBarItem->m_strStyle == _T("S"))
			TBButton.fsStyle = TBSTYLE_SEP;
		else if (pToolBarItem->m_strType == _T("R"))
			TBButton.fsState = TBSTATE_ENABLED;
		else if (pToolBarItem->m_strType == _T("I"))
			TBButton.fsState = TBSTATE_HIDDEN;
		else if (pToolBarItem->m_strType == _T("D"))
			TBButton.fsState = TBSTATE_INDETERMINATE;
		
		// Normal bitmap file check
		if (!IsFileExist(pToolBarItem->m_strBitmapNormal))
		{
			// Default Check
			if (!IsFileExist(m_strBitmapNormalPath))
			{
				AfxMessageBox(_T("Default Bitmap�� �����ϴ�."));
				return -1;
			}
			else
				pToolBarItem->m_strBitmapNormal = m_strBitmapNormalPath;
		}
		
		// Hot bitmap file check
		if (!IsFileExist(pToolBarItem->m_strBitmapHot))
		{
			// Default Check
			if (!IsFileExist(m_strBitmapHotPath))
			{
				AfxMessageBox(_T("Default Bitmap�� �����ϴ�."));
				return -1;
			}
			else
				pToolBarItem->m_strBitmapHot = m_strBitmapHotPath;
		}
		
		// Disabled bitmap file check
		if (!IsFileExist(pToolBarItem->m_strBitmapDisabled))
		{
			// Default Check
			if (!IsFileExist(m_strBitmapDisabledPath))
			{
				AfxMessageBox(_T("Default Bitmap�� �����ϴ�."));
				return -1;
			}
			else
				pToolBarItem->m_strBitmapDisabled = m_strBitmapDisabledPath;
		}
		
		
		// attach bitmap
		bmpNormal.Attach(::LoadImage(NULL, pToolBarItem->m_strBitmapNormal, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		bmpHot.Attach(::LoadImage(NULL, pToolBarItem->m_strBitmapHot, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		bmpDisabled.Attach(::LoadImage(NULL, pToolBarItem->m_strBitmapDisabled, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		
		ReplaceBackgroundColor(bmpNormal);
		ReplaceBackgroundColor(bmpHot);
		ReplaceBackgroundColor(bmpDisabled);
		
		pImgListNormal->Add(&bmpNormal, RGB(0, 0, 0));
		pImgListHot->Add(&bmpHot, RGB(0, 0, 0));
		pImgListDisabled->Add(&bmpDisabled, RGB(0, 0, 0));
		
		bmpNormal.Detach();
		bmpHot.Detach();
		bmpDisabled.Detach();
		
		rpToolBarCtrl.InsertButton(pToolBarItem->m_nIndex, &TBButton);
		
		pToolBar->SetButtonText(pToolBarItem->m_nIndex, pToolBarItem->m_strCaption);
		
		// tooltip..
		CRect rect;
		rpToolBarCtrl.GetItemRect(pToolBarItem->m_nIndex, rect);
		pToolTipCtrl->AddTool(pToolBar, pToolBarItem->m_ToolTip, rect, pToolBarItem->m_nID);

	}

	rpToolBarCtrl.SetImageList(pImgListNormal);
	rpToolBarCtrl.SetHotImageList(pImgListHot);
	rpToolBarCtrl.SetDisabledImageList(pImgListDisabled);

	rpToolBarCtrl.SetToolTips(pToolTipCtrl);

	rpToolBarCtrl.AutoSize();
	
	return 0;
}

INT CCheckinVolResouceMgr::CreateMenu()
{
	CMenu *pSystemMenu = new CMenu;
	pSystemMenu->CreateMenu();

	m_listMenu.AddTail(pSystemMenu);

	CString strKey;
	INT nValue;
	
	CMenuItem *pMenuItem = NULL;
	POSITION posMenuItem = NULL;
	POSITION posMenuGroup = m_mapMenuGroup.GetStartPosition();
	while (posMenuGroup)
	{
		m_mapMenuGroup.GetNextAssoc(posMenuGroup, strKey, nValue);
		if (strKey.IsEmpty()) continue;

		// Crete SubMenu
		CMenu *pMenu = new CMenu;
		pMenu->CreateMenu();
		m_listMenu.AddTail(pMenu);

		posMenuItem = m_listMenuItem.GetHeadPosition();
		while (posMenuItem)
		{
			pMenuItem = static_cast<CMenuItem*>(m_listMenuItem.GetNext(posMenuItem));
			if (pMenuItem == NULL) continue;
			if (strKey != pMenuItem->m_strGroupAlias) continue;

			
			pMenu->AppendMenu(MF_STRING | MF_ENABLED, pMenuItem->m_nID, pMenuItem->m_strCaption);
		}

		// Append SubMenu
		pSystemMenu->AppendMenu(MF_POPUP, (UINT)pMenu->m_hMenu, strKey);
		pMenu->Detach();
	}

	m_pFrameWnd->SetMenu(pSystemMenu);

	return 0;
}

INT CCheckinVolResouceMgr::CreateMenuItem(INT &nIdx, LPCTSTR lpszGroupAlias, LPCTSTR lpszAlias, LPCTSTR lpszCaption)
{
	CMenuItem *pMenuItem = new CMenuItem;
	m_listMenuItem.AddTail(pMenuItem);

	m_mapMenuGroup.SetAt(lpszGroupAlias, 1);

	pMenuItem->m_nID = ++m_nResourceID;
	pMenuItem->m_nIndex = nIdx++;
	pMenuItem->m_strGroupAlias = lpszGroupAlias;
	pMenuItem->m_strAlias = lpszAlias;
	pMenuItem->m_strCaption = lpszCaption;
	
	return 0;
}

INT CCheckinVolResouceMgr::CreateDialogBar()
{
	/*
	m_dlgSpeciesInfo.pMain = NULL;
	if (!m_wndSpeciesInfo.Create(m_pFrameWnd, &m_dlgSpeciesInfo, CString(_T("������"))), 
		IDD_CHECKIN_VOL_SPECIES, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY))
	{
		TRACE0("Failed to create m_wndCodeBar\n");
        return -1;      // fail to create
	}
	
	m_wndCodeBar.EnableDocking(CBRS_ALIGN_TOP);
	m_pFrameWnd->DockControlBar(&m_wndCodeBar);
	
	*/
	

	m_dlgSpeciesInfo.pMain = NULL;
	if (!m_wndSpeciesInfo.Create(m_pFrameWnd, &m_dlgSpeciesInfo, CString(_T("������")), 
		IDD_CHECKIN_VOL_SPECIES, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("Failed to create m_wndSpeciesInfo\n");
        return -1;      // fail to create
	}

	m_wndSpeciesInfo.EnableDocking(CBRS_ALIGN_BOTTOM);
	m_pFrameWnd->DockControlBar(&m_wndSpeciesInfo);


	return 0;
}

INT CCheckinVolResouceMgr::GetButtonWidth()
{
	CString strTmp;
	INT nCount = 0;
	INT nCountSize = 0;
	INT nButtonSize = 0;
	INT nTmpSize = 0;
	

	CToolBarItem *pToolBarItem = NULL;
	POSITION pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem.GetNext(pos));
		if (pToolBarItem == NULL) continue;
		
		strTmp = pToolBarItem->m_strCaption;
		
		strTmp.TrimLeft();
		strTmp.TrimRight();
		if(strTmp.GetLength() > 10)
		{
			nTmpSize = strTmp.GetLength() - 10;
			nCountSize = (nTmpSize * 5);
			if (nCountSize > nButtonSize)
			{
				nButtonSize = (nTmpSize * 5) + 2;
			}
		}
	}

	return nButtonSize;
}

BOOL CCheckinVolResouceMgr::IsFileExist(LPCTSTR lpszFileName)
{
	CFile file;
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary))
		return FALSE;
	else
		file.Close();
	return TRUE;
}

VOID CCheckinVolResouceMgr::ReplaceBackgroundColor(CBitmap& ioBM)
{
	// figure out how many pixels there are in the bitmap
	BITMAP		bmInfo;

	VERIFY (ioBM.GetBitmap (&bmInfo));
	
	 // add support for additional bit depths here if you choose
	if (bmInfo.bmBitsPixel != 24) return;
	VERIFY (bmInfo.bmBitsPixel == 24);
	VERIFY (bmInfo.bmWidthBytes == (bmInfo.bmWidth * 3));

	const UINT		numPixels (bmInfo.bmHeight * bmInfo.bmWidth);

	 // get a pointer to the pixels
    DIBSECTION  ds;
	VERIFY (ioBM.GetObject (sizeof (DIBSECTION), &ds) == sizeof (DIBSECTION));

	RGBTRIPLE*		pixels = reinterpret_cast<RGBTRIPLE*>(ds.dsBm.bmBits);
	VERIFY (pixels != NULL);

	 // get the user's preferred button color from the _tsystem
	const COLORREF		buttonColor (::GetSysColor (COLOR_BTNFACE));
	const RGBTRIPLE		userBackgroundColor = {
		GetBValue (buttonColor), GetGValue (buttonColor), GetRValue (buttonColor)};

	RGBTRIPLE	kBackgroundColor = {192, 192, 192};
	//RGBTRIPLE	kBackgroundColor = {200, 208, 212};
	 // search through the pixels, substituting the user's button
	 // color for any pixel that has the magic background color
	for (UINT i = 0; i < numPixels; ++i)
	{
		if (pixels [i].rgbtBlue == kBackgroundColor.rgbtBlue &&
			pixels [i].rgbtGreen == kBackgroundColor.rgbtGreen &&
			pixels [i].rgbtRed == kBackgroundColor.rgbtRed)
		{
			pixels [i] = userBackgroundColor;
		}
	}
}

VOID CCheckinVolResouceMgr::FreeResourceList()
{
	VOID *pClass = NULL;

	// ToolBar
	FreePtrList(&m_listToolBar, static_cast<CToolBar*>(pClass));

	// ToolBarItemList
	FreePtrList(&m_listToolBarItem, static_cast<CToolBarItem*>(pClass));

	// ImageList
	FreePtrList(&m_listImageList, static_cast<CImageList*>(pClass));

	// ToolTipList
	FreePtrList(&m_listToolTip, static_cast<CToolTipCtrl*>(pClass));

	// Menu
	//VOID *pClass = NULL;
	FreePtrList(&m_listMenu, static_cast<CMenu*>(pClass));
	
	// MenuItemList
	FreePtrList(&m_listMenuItem, static_cast<CMenuItem*>(pClass));
	
}

template <class T> VOID CCheckinVolResouceMgr::FreePtrList(CPtrList *pPtrList, T *pClass)
{
	if (pPtrList == NULL) return;

	pClass = NULL;
	POSITION pos = pPtrList->GetHeadPosition();
	while (pos)
	{
		pClass = static_cast<T*>(pPtrList->GetNext(pos));
		if (pClass)
		{
			delete pClass;
			pClass = NULL;
		}
	}

	pPtrList->RemoveAll();
}

BOOL CCheckinVolResouceMgr::OnCmdMsg(UINT nID)
{
	POSITION pos = NULL;
		
	// Menu
	CMenuItem *pMenuItem = NULL;
	pos = m_listMenuItem.GetHeadPosition();
	while (pos)
	{
		pMenuItem = static_cast<CMenuItem*>(m_listMenuItem.GetNext(pos));
		if (pMenuItem && pMenuItem->m_nID == nID)
			return TRUE;
	}
	

	// ToolBar
	CToolBarItem *pToolBarItem = NULL;
	pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem.GetNext(pos));
		if(pToolBarItem && pToolBarItem->m_nID == nID)
			return TRUE;
	}

	return FALSE;
}

BOOL CCheckinVolResouceMgr::OnCommand(UINT nID, CString &strCommandAlias)
{
	POSITION pos = NULL;

	
	// Menu
	CMenuItem *pMenuItem = NULL;
	pos = m_listMenuItem.GetHeadPosition();
	while (pos)
	{
		pMenuItem = static_cast<CMenuItem*>(m_listMenuItem.GetNext(pos));
		if (pMenuItem && pMenuItem->m_nID == nID)
		{
			strCommandAlias = pMenuItem->m_strAlias;

			return TRUE;
		}
	}
	

	// ToolBar
	CToolBarItem *pToolBarItem = NULL;
	pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem.GetNext(pos));
		if(pToolBarItem && pToolBarItem->m_nID == nID)
		{
			strCommandAlias = pToolBarItem->m_strAlias;

			return TRUE;
		}
	}

	return FALSE;
}


BOOL CCheckinVolResouceMgr::OnCommand(CString strCommandAlias)
{
	
	if (strCommandAlias == _T("VIEW_IDD_CHECKIN_VOL_SPECIES"))
	{
		if (m_wndSpeciesInfo.IsWindowVisible())
			m_wndSpeciesInfo.ShowWindow(SW_HIDE);
		else
			m_wndSpeciesInfo.ShowWindow(SW_SHOW);

		m_pFrameWnd->DockControlBar(&m_wndSpeciesInfo, AFX_IDW_DOCKBAR_BOTTOM);
	}
	else return FALSE;

	m_pFrameWnd->RecalcLayout();

	return TRUE;
}

INT CCheckinVolResouceMgr::HideToolBarButton(CString strToolBarButtonAlias, BOOL bHide /*= TRUE*/)
{
	INT nID = -1;

	CStringArray StringArray;

	CToolBarItem *pToolBarItem = NULL;

	INT nCnt = m_listToolBarItem.GetCount();
	if(0 == nCnt) return 0;

	POSITION pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem.GetNext(pos));
		if (pToolBarItem == NULL) continue;
		if (pToolBarItem->m_strAlias == strToolBarButtonAlias)
		{
			nID = pToolBarItem->m_nID;
			break;
		}
	}
	
	if (nID == -1) return -1;

	UINT nState = 0;//TBSTATE_WRAP;

	if (bHide)
		nState |= TBSTATE_HIDDEN ;
	else
		nState |= TBSTATE_ENABLED;

	CToolBar *pToolBar = NULL;
	pos = m_listToolBar.GetHeadPosition();
	while (pos)
	{
		pToolBar = static_cast<CToolBar*>(m_listToolBar.GetNext(pos));
		if (pToolBar == NULL) continue;

		CToolBarCtrl &pToolBarCtrl = pToolBar->GetToolBarCtrl();

		pToolBarCtrl.SetState(nID, TBSTATE_WRAP);
		pToolBarCtrl.SetState(nID, nState);

		m_pFrameWnd->RecalcLayout();
	}

	return 0;
}