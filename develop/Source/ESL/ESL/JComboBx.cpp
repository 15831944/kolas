// JComboBox.cpp : implementation file
//
#include "stdafx.h"
#include "except.h"
#include "JComboBx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CTitleTip

CJTitleTip::CJTitleTip()
{
EFS_BEGIN
	// Register the window class if it has not already been registered.
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
    if(!(::GetClassInfo(hInst, CJTITLETIP_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style = CS_SAVEBITS ;
        wndcls.lpfnWndProc = ::DefWindowProc;
        wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
        wndcls.hInstance = hInst;
        wndcls.hIcon = NULL;
        wndcls.hCursor = LoadCursor( hInst, IDC_ARROW );
        wndcls.hbrBackground = (HBRUSH)(COLOR_INFOBK + 1); 	//COLOR_HIGHLIGHT;//
        wndcls.lpszMenuName = NULL;
        wndcls.lpszClassName = CJTITLETIP_CLASSNAME;
        if (!AfxRegisterClass(&wndcls))
                AfxThrowResourceException();
    }
	TITLETIP_SHOW = TRUE;
	bFocusRect = FALSE;
EFS_END
}

CJTitleTip::~CJTitleTip()
{
}

BOOL CJTitleTip::Create(CWnd * pParentWnd)
{
EFS_BEGIN
	ASSERT_VALID(pParentWnd);
		//ASSERT( !::IsWindow( m_hWnd ) );
        DWORD dwStyle = WS_BORDER | WS_POPUP; 
        DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
        m_pParentWnd = pParentWnd;
		//if (m_hWnd == NULL)
			return CreateEx( dwExStyle, 
							 CJTITLETIP_CLASSNAME, 
							 NULL, 
							 dwStyle, 
							 0, 
							 0, 
							 0, 
							 0, 
							 NULL, 
							 NULL, 
							 NULL );
		
		//return FALSE;
EFS_END
	return FALSE;
}
// Show                 - Show the titletip if needed
// rectTitle            - The rectangle within which the original 
//                        title is constrained - in client coordinates
// lpszTitleText        - The text to be displayed
// xoffset              - Number of pixel that the text is offset from
//                        left border of the cell

VOID CJTitleTip::Show(CRect rectTitle, LPCTSTR lpszTitleText, INT xoffset /*=0*/)
{
EFS_BEGIN
	ASSERT( ::IsWindow( m_hWnd ) );
    ASSERT( !rectTitle.IsRectEmpty() );

    // Do not display the titletip is app does not have focus
    if(( GetFocus() == NULL )||(!TITLETIP_SHOW))
	{
		if( IsWindowVisible() ) 
			ShowWindow(FALSE);	
		return;
	}

    // Determine the width of the text
//     m_pParentWnd->ClientToScreen( rectTitle );
	CWnd* pWnd = FindWindow(_T("ComboLBox"), NULL);
	if(pWnd)
	{
		pWnd->ClientToScreen(rectTitle);
	}
	else
	{
		m_pParentWnd->ClientToScreen( rectTitle );
	}


    CClientDC dctmp(this);
	dc = CDC::FromHandle(dctmp.m_hDC);

    CString strTitle(lpszTitleText);
	strTitle += _T("  ");
    CFont *pFont = m_pParentWnd->GetFont(); // use same font as ctrl
	dc->SelectObject( pFont );

    CRect rectDisplay = rectTitle;
    CSize size = dc->GetTextExtent( strTitle );

// 	rectDisplay.top   += 24;
// 	rectDisplay.bottom  += 25;
	rectDisplay.bottom  += 1;
    rectDisplay.left += xoffset-4;
    rectDisplay.right = rectDisplay.left + size.cx +1;

    // Do not display if the text fits within available space 
    if( rectDisplay.right < rectTitle.right-xoffset-5 ){
		if( IsWindowVisible() ) 
			ShowWindow(FALSE);
		return;
	}

	
	//;
	if ( DCB_RESET == dc->GetBoundsRect(&rectTitle,DCB_RESET))
		 dc->SetBoundsRect(NULL,DCB_ENABLE);

	dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	dc->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
 
   // Show the titletip
   // If titletip is already displayed, don't do anything.

    if( IsWindowVisible() ) 
	{
            MoveWindow(rectDisplay.left, rectDisplay.top, 
            rectDisplay.Width(), rectDisplay.Height());
			UpdateWindow();
			
	   		dc->FillSolidRect(&rectTitle,::GetSysColor(COLOR_HIGHLIGHT));
			//if rectDisplay is not being used, let me use it for Text area
			rectDisplay = rectTitle;
			rectDisplay.left += 2;
			dc->DrawText(strTitle,-1,rectDisplay, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
			dc->DrawFocusRect(&rectTitle); 
	}
	else
	{
		SetWindowPos( &wndTop, rectDisplay.left, rectDisplay.top, 
                    rectDisplay.Width(), rectDisplay.Height(), 
                    SWP_SHOWWINDOW|SWP_NOACTIVATE );
		dc->SetBkMode( OPAQUE );
		dc->FillSolidRect(&rectTitle,::GetSysColor(COLOR_HIGHLIGHT));
		dc->DrawText(strTitle,-1,rectDisplay, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
		dc->DrawFocusRect(&rectTitle);
	}

EFS_END
}


/////////////////////////////////////////////////////////////////////////////
// CJComboBox

CJComboBox::CJComboBox()
{
EFS_BEGIN
	TotalCol = 1;
	CHECKBOX = FALSE;
	m_pJStruct = NULL;
	////++우종 추가...
	m_nMaxRowCnt=0;
	for (INT i=0;i<5;i++) m_bCal[i]=FALSE;
/*	pFirst = NULL;
	pCurrent = NULL;
	pObjDump = NULL;
*/
EFS_END
}
						
CJComboBox::~CJComboBox()
{
EFS_BEGIN
	////////////////////////////////////////////////////////////////
	// Yes, I could use MFC Collection class
	// But, it looks crappy anyway.
	// It doesn't give me a good practice approach.
	// Maybe one of these days I will use it (-; (when I became lazy)
	/////////////////////////////////////////////////////////////////
	
	POSITION pos = m_jcbList.GetHeadPosition();
	while( NULL != pos )
		delete m_jcbList.GetNext( pos );
	m_jcbList.RemoveAll();	

	BOOL bSuccess = m_titletip.DestroyWindow();
EFS_END
}

INT CJComboBox::FreeComboData(VOID)
{
EFS_BEGIN
	POSITION pos = m_jcbList.GetHeadPosition();
	while( NULL != pos )
		delete m_jcbList.GetNext( pos );
	m_jcbList.RemoveAll();	

EFS_END
	return 0;
}

BEGIN_MESSAGE_MAP(CJComboBox, CComboBox)
	//{{AFX_MSG_MAP(CJComboBox)
    ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// CJComboBox message handlers

 
// if your sorting scheme is different than a standard collating sequence, 
// such as one where the comparisons need to be case-insensitive.)
INT CJComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	return -1;
}


VOID CJComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
EFS_BEGIN
	BOOL bFocus=(lpDrawItemStruct->itemAction & ODA_FOCUS);
	BOOL bDrawEntire=(lpDrawItemStruct->itemAction & ODA_FOCUS);
	BOOL bSelected = ((lpDrawItemStruct->itemState & ODS_SELECTED));

	//Hide the titletip first until dropdown is active
	//but titletip window has been initialised.... (-:
	m_titletip.TITLETIP_SHOW = GetDroppedState();

	if (bFocus||bSelected)
	{
		if (EDGE_STYLE == EDGE) 
			DrawDropList(lpDrawItemStruct, EDGE_HIGHLIGHT);
		else
			DrawDropList(lpDrawItemStruct,HIGH_LIGHT);
	}
	else
	{
		if (EDGE_STYLE == EDGE) 
			DrawDropList(lpDrawItemStruct, EDGE_NORMAL);
		else
			DrawDropList(lpDrawItemStruct,NORMAL);
	}
	
 	if (CHECKBOX)
		if( (lpDrawItemStruct->itemID == CurSel))	//(bSelected)||
			DrawDropList(lpDrawItemStruct,CHECKED);
EFS_END
}


// You can actually specify variable heights as long as you set the 
// CBS_OWNERDRAWVARIABLE style.
VOID CJComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
EFS_BEGIN
	// all items are of fixed size
	// must use CBS_OWNERDRAWVARIABLE for this to work
	if (EDGE_STYLE == EDGE)
		lpMeasureItemStruct->itemHeight = ITEM_HEIGHT;

EFS_END
}

INT CJComboBox::AddString(LPCTSTR lpszString)
{  	
EFS_BEGIN
	return CJComboBox::AddItem(lpszString,0,CComboBox::GetCount()); 
EFS_END
	return 0;
}

INT CJComboBox::AddItem(CString strItem,INT nCol,INT nRow,INT nMask,INT nFmt)
{
EFS_BEGIN
	INT CB_MSG;
	if (nColShow == nCol) //nRow == PrevRow && 
		CB_MSG = CComboBox::AddString(strItem);

 	cItem[nCol][nRow] = strItem;
		AddItemPtr(nCol,nRow);

	PrevRow = nRow;

	////++ 우종추가... 이래야 그 Col의 최대 스트링을 다시 계산한다...
	m_bCal[nCol]=FALSE;

	return CB_MSG; 

EFS_END
	return 0;
}
////////////////////////////////////////
//For index key lookup operations//////
/////////////////////////////////////////

VOID CJComboBox::AddItemPtr(INT nCol, INT nRow)
{	
EFS_BEGIN
//////////////////////////////////////////////////////////////////
//	}
//////////////////////////////////////////////////////////////////////
// This portion of the code is only applicable to MFC pointer Lists
// I keep it for my own reference in the future.
//////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////
	//Here we could use 
	//MFC Collection class method, too easy.
			
	if (!nCol) 
	{
		// set Head position here
		m_pJStruct = new CJStruct();
		m_jcbList.AddTail(m_pJStruct);
		m_pJStruct->m_strKey[nCol] = cItem[nCol][nRow];
	}
	else
	if (nCol > 0) {
		POSITION pos;
		pos = m_jcbList.FindIndex(nRow);
		m_pJStruct = m_jcbList.GetAt(pos);
	   	m_pJStruct->m_strKey[nCol] = cItem[nCol][nRow];
	}
	if (m_pJStruct != NULL)
	{
		m_pJStruct->m_nKey = nRow;
		CComboBox::SetItemDataPtr(nRow,m_pJStruct);
	}
			
		
EFS_END			
}

VOID CJComboBox::DrawDropList(LPDRAWITEMSTRUCT lpDrawItemStruct,UINT STATE)
{
EFS_BEGIN
	
	if (!m_titletip.GetSafeHwnd()) m_titletip.Create(this);
	
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	CRect rcText;
    CRect rectParent , rectThis , rect;
	
	COLORREF RGB_COLOR_WINDOW = GetSysColor(COLOR_WINDOW);
	if (RequireFlag) {
		RGB_COLOR_WINDOW = RGB(m_bkR, m_bkG, m_bkB);
	}
    switch(STATE)
	{
		case EDGE_HIGHLIGHT:
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
			pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));			
			rcItem.SetRect(rcItem.left+1,rcItem.top,rcItem.right-2,rcItem.bottom);
			pDC->FillRect(&rcItem,&CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
			if(lpDrawItemStruct->itemID>=0)
				rcText = OnDrawThisText(pDC,lpDrawItemStruct->rcItem,lpDrawItemStruct->itemID);			
			if (CHECKBOX) 
			{
				rcItem.SetRect(rcItem.right-14,rcItem.top,rcItem.right+3,rcItem.bottom);
				pDC->DrawFrameControl(&rcItem, DFC_BUTTON, DFCS_CHECKED );
			}			
			
			// You can set your own position for the Tooltip here
			rcText.SetRect( rcText.left+5,rcText.top,rcText.right+5,rcText.bottom );
            m_titletip.Show( rcText , cItem[TotalCol-1][lpDrawItemStruct->itemID], 0 );          

		break;

		case EDGE_NORMAL:
			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
			pDC->SetBkColor(RGB_COLOR_WINDOW);			
			pDC->FillRect(&lpDrawItemStruct->rcItem,&CBrush(RGB_COLOR_WINDOW));
			
			if(lpDrawItemStruct->itemID>=0)
				OnDrawThisText(pDC,lpDrawItemStruct->rcItem,lpDrawItemStruct->itemID);
			if (CHECKBOX) {
				rcItem.SetRect(rcItem.right-17,rcItem.top,rcItem.right+1,rcItem.bottom);
					pDC->DrawFrameControl(&rcItem, DFC_BUTTON, DFCS_BUTTONCHECK);
			}
	    break;

		case HIGH_LIGHT:
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
			pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
			pDC->FillRect(&rcItem,&CBrush(::GetSysColor(COLOR_HIGHLIGHT)));

			if (CHECKBOX) 
            {     
				rcItem.SetRect(rcItem.right-17,rcItem.top,rcItem.right+1,rcItem.bottom);
				pDC->DrawFrameControl(&rcItem, DFC_BUTTON, DFCS_CHECKED );
			}
			
			rcItem = lpDrawItemStruct->rcItem;
			rcItem.SetRect(rcItem.left-3,rcItem.top,rcItem.right,rcItem.bottom);
			if(lpDrawItemStruct->itemID>=0)
				OnDrawThisText(pDC,rcItem,lpDrawItemStruct->itemID);
			rcItem.SetRect(rcItem.left-3,rcItem.top,rcItem.right+20,rcItem.bottom); 
			pDC->DrawFocusRect(&rcItem);

			m_titletip.Show(rcItem, cItem[nColShow][lpDrawItemStruct->itemID], 10 );

		break;

		case NORMAL:
			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
			pDC->SetBkColor(RGB_COLOR_WINDOW);
			pDC->FillRect(&lpDrawItemStruct->rcItem,&CBrush(::GetSysColor(COLOR_WINDOW)));
			
			if (CHECKBOX) {
				rcItem.SetRect(rcItem.right-17,rcItem.top,rcItem.right+1,rcItem.bottom);
				pDC->DrawFrameControl(&rcItem, DFC_BUTTON, DFCS_BUTTONCHECK);				
				pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
			}
			rcItem = lpDrawItemStruct->rcItem;	
			rcItem.SetRect(rcItem.left-3,rcItem.top,rcItem.right,rcItem.bottom);
			if(lpDrawItemStruct->itemID>=0)
				OnDrawThisText(pDC,rcItem,lpDrawItemStruct->itemID);

	   break;

		case CHECKED:
			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
			pDC->SetBkColor(RGB_COLOR_WINDOW);
			pDC->FillRect(&lpDrawItemStruct->rcItem,&CBrush(::GetSysColor(COLOR_WINDOW)));
	
			if(lpDrawItemStruct->itemID>=0)
				OnDrawThisText(pDC,lpDrawItemStruct->rcItem,lpDrawItemStruct->itemID);
			rcItem.SetRect(rcItem.right-17,rcItem.top,rcItem.right+1,rcItem.bottom);
			pDC->DrawFrameControl(&rcItem, DFC_BUTTON, DFCS_CHECKED);
	   break;

	}


	
EFS_END
}




BOOL CJComboBox::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
EFS_END
	return FALSE;
}




VOID CJComboBox::ResetContent(INT tCol,INT nJustify, BOOL bCheckBox, UINT nEdge,INT ColDisplay)
{
EFS_BEGIN
	FormatList(tCol,nJustify, bCheckBox, nEdge,ColDisplay);
	CComboBox::ResetContent();
EFS_END
}

/////////////////////////////////////////////////////////////////
///// The older version of this CJComboBox will still be using FormatList()   ///////
/////  I override the ResetContent to be used the same way///
/////////////////////////////////////////////////
//  ColDisplay > -1 means you can HIDE the column that 
//     is not ColDisplay as an index key for  database purposes...
//     BUT tCol must be 0 to hide your selected column
VOID CJComboBox::FormatList(INT tCol,INT nJustify, BOOL bCheckBox, UINT nEdge,INT ColDisplay)
{
EFS_BEGIN
	//Everything _getts initialised here
	TotalCol = tCol;
	JUSTIFY = nJustify;
	CHECKBOX = bCheckBox;
	EDGE_STYLE = nEdge;
	PrevRow = -1;
	PrevCol = -1;
//	CurSel = -1; 
	CurSel = 0; 
	nColShow = ColDisplay;
	EnableToolTips(TRUE);
	m_pJStruct = NULL;
EFS_END
}

CRect CJComboBox::OnDrawThisText(CDC* pDC,CRect rcItem,INT itemID)
{
EFS_BEGIN

	CRect rcText = rcItem;
	CRect rcBorder = rcItem;	
	rcText.SetRect(rcText.left,rcText.top,rcText.left+3,rcText.bottom);
	rcBorder.SetRect(rcBorder.left,rcBorder.top,rcText.left+1,rcBorder.bottom);
	INT wid = 0;
	for(INT j=1;j<=TotalCol;j++){

		if (j!=TotalCol)
		{	
			rcText.SetRect(rcBorder.right+2,rcText.top,rcText.right+ wid,rcText.bottom);
			wid = DrawThisText(pDC,rcText,j-1,itemID);
	
			rcBorder.SetRect(rcBorder.right,rcBorder.top,rcBorder.right+wid,rcBorder.bottom);
			if ((EDGE_STYLE == EDGE)&&(itemID >= -1))
			{
				pDC->DrawEdge(&rcBorder,EDGE_ETCHED,BF_RECT);
			}
		}
		else
		if (j == TotalCol)
		{
			rcText.SetRect(rcBorder.right+4,rcText.top,rcBorder.right+ OFFSET_END,rcText.bottom);
			wid = DrawThisText(pDC,rcText,j-1,itemID);
			//rcBorder.SetRect(rcBorder.right,rcBorder.top,rcItem.right-1,rcBorder.bottom);

			//// 이하 우종 수정...(5/19)
			//// 이유 : 멀티콤보의 폭을 좁게하면 다운되는 현상 해결하기 위해...
			if (rcBorder.right<rcItem.right-1) {
				rcBorder.SetRect(rcBorder.right,rcBorder.top,rcItem.right-1,rcBorder.bottom);
			}
			else {
				rcBorder.SetRect(rcBorder.right,rcBorder.top,rcBorder.right+1,rcBorder.bottom);
			}
			//// 이상 우종 수정...(5/19)

			if ((EDGE_STYLE == EDGE)&&(itemID >= -1))
			{
				pDC->DrawEdge(&rcBorder,EDGE_ETCHED,BF_RECT);
			}
		}
		
	}
	

	return rcBorder;// for One column 
EFS_END
	return 0;
}

INT CJComboBox::DrawThisText(CDC * pDC,CRect rcText,
									 INT nColumn,INT nItem)
{
EFS_BEGIN
	CString str;

	if (EDGE_STYLE == EDGE) 
		rcText.SetRect(rcText.left,rcText.top+1,rcText.right,rcText.bottom);

	if ((nColumn >= 0)&&(nItem >= 0))
		if (nColShow >= TotalCol) //TRY WITH  (nColShow >= 0) I added this for version 2.0.99999 
			str = cItem[nColShow][nItem]; // so, you can hide the column you want
		else
			str = cItem[nColumn][nItem];



	if (EDGE_STYLE == EDGE)
		pDC->DrawText(str,-1,rcText, JUSTIFY | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
	else
	{	///////This operation is for showing hint when the box is droping down.////
		//rcText.SetRect(rcText.left,rcText.top,rcText.right+30,rcText.bottom);
		pDC->TabbedTextOut(rcText.left,rcText.top,str,0,NULL,rcText.left);

	}


	////////////////////////////////////////////////////////////////
	//ALGORITHM (2)
	//This algorithm is slow but discretely proven to be accurate.
	//////////////////////////////////////////////////
////++
/*++
	for (INT i=0;i<=nItem;i++)
	{
		if (pDC->GetTextExtent(cItem[nColumn][i]).cx >= pDC->GetTextExtent(cItem[nColumn][0]).cx)
			ColWidth[nColumn] = pDC->GetTextExtent(cItem[nColumn][0]).cx +12; 
	}
++*/
	////++ 우종 변경.. 현 Col의 스트링 중 최대 긴 스트링의 길이를 기준삼아 사각형을 그린다.
	////++
	if (m_bCal[nColumn]==FALSE) {
		ColWidth[nColumn]=pDC->GetTextExtent(cItem[nColumn][0]).cx;
		for (INT i=0;i<m_nMaxRowCnt;i++) {
			if (pDC->GetTextExtent(cItem[nColumn][i]).cx > ColWidth[nColumn]) {
				ColWidth[nColumn]=pDC->GetTextExtent(cItem[nColumn][i]).cx;
			}
		}
		if (ColWidth[nColumn]!=0) ColWidth[nColumn]+=12;
		m_bCal[nColumn]=TRUE;
	}	//이건 그 Col에서 한 번만 수행되게 한다. 어차피 ColWidth가 멤버 함수니까...
        
	return ColWidth[nColumn];  

EFS_END
	return 0;
}


VOID CJComboBox::PreSubclassWindow() 
{
EFS_BEGIN
	ModifyStyleEx(WS_EX_TOPMOST,NULL,0);
	CComboBox::PreSubclassWindow();
	//m_titletip.Create(this);
EFS_END
}

VOID CJComboBox::OnMouseMove(UINT nFlags, CPoint point) 
{
EFS_BEGIN
	m_titletip.TITLETIP_SHOW = TRUE;
	CComboBox::OnMouseMove(nFlags, point);
EFS_END
}

VOID CJComboBox::OnCloseup() 
{
EFS_BEGIN
	m_titletip.TITLETIP_SHOW = FALSE;
	CJComboBox::SetCurSel(CJComboBox::GetCurSel());
EFS_END
}

INT CJComboBox::GetLBText(INT nIndex, CString & rString,INT col)
{
EFS_BEGIN
    if ( nIndex >= 0 ) {
	    rString = cItem[col][nIndex];
	    //CComboBox::GetLBText(nIndex,rString);
	    return rString.GetLength();
    } else {
	    rString = _T("");
	    return 0;
    }

EFS_END
	return 0;
}

INT CJComboBox::SetCurSel(INT nSelect)
{
EFS_BEGIN
	CurSel = nSelect;
	return CComboBox::SetCurSel(nSelect);
EFS_END
	return 0;
}

// nStartAfter = -1 to search for entire combobox
// nColSelect = which column you want to get data from
//  lpszString = returning the value you are requesting
//////////////////////////////////////////////////////
INT CJComboBox::SelectString(LPCTSTR lpszString,INT nStartAfter, INT nColSelect)
{
EFS_BEGIN
// MFC Collection class search
	CJStruct * pCurrent;
	for (INT nCol=-1;TotalCol>nCol;nCol++)
	{

		if (nStartAfter == -1) //search for entire combobox
		{
			POSITION pos = m_jcbList.GetHeadPosition();
			while (pos != NULL)
			{
				if (lpszString == (pCurrent = m_jcbList.GetNext(pos))->m_strKey[nCol+1])
					return CComboBox::SetCurSel(pCurrent->m_nKey);				
			}
	
		}
		else
		{	//if you don't want to search the entire Combobox
			POSITION pos = m_jcbList.GetHeadPosition();
			while (pos != NULL)
			{
				
				if (nStartAfter == (pCurrent = m_jcbList.GetNext(pos))->m_nKey)
				{
					if (lpszString == pCurrent->m_strKey[nCol+1])
						return CComboBox::SetCurSel(pCurrent->m_nKey);
					break;
				}
		
			}
		} //END ELSE

		} //end for
	return CB_ERR;
	///////////////////////////////////////////////////////////////////////
EFS_END
	return 0;
}

CString CJComboBox::GetItemDataString(INT nIndex, INT nColumn)
{
EFS_BEGIN
	if (nIndex >= 0)
	{
		CJStruct* m_pJStruct = (CJStruct*)CComboBox::GetItemDataPtr(nIndex);
		if (m_pJStruct != NULL) {
			return m_pJStruct->m_strKey[nColumn];
		}
	}
	return _T("");
EFS_END
	return _T("");
}


VOID CJComboBox::Finish()
{
}

