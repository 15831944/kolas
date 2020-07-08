// AutoCompleteList.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "AutoCompleteList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteList dialog


CAutoCompleteList::CAutoCompleteList(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoCompleteList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoCompleteList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIsVisible = TRUE;
}


void CAutoCompleteList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoCompleteList)
	DDX_Control(pDX, IDC_LIST_AUTO_COMPLETE, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoCompleteList, CDialog)
	//{{AFX_MSG_MAP(CAutoCompleteList)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_LIST_AUTO_COMPLETE, OnSelchangeListAutoComplete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteList message handlers

void CAutoCompleteList::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	CRect rect;
	GetWindowRect(rect);
	if(NULL != m_List.m_hWnd)
		m_List.MoveWindow(0,0,cx,cy);	
	// TODO: Add your message handler code here
	
}

INT CAutoCompleteList::Add_AutoCompleteData(CString strAdd)
{
	m_strAutoList.Add(strAdd);

	return 0;
}

INT CAutoCompleteList::SetAt_AutoCompleteData(INT nIndex, CString strModify)
{
	if(TRUE == Is_BadIndex(nIndex)) return -1;
	m_strAutoList.SetAt(nIndex, strModify);

	return 0;
}

INT CAutoCompleteList::RemoveAt_AutoCompleteData(INT nIndex, CString strDelete)
{
	if(TRUE == Is_BadIndex(nIndex)) return -1;
	m_strAutoList.RemoveAt(nIndex);

	return 0;
}

INT CAutoCompleteList::RemoveAll_AutoCompleteData()
{
	m_strAutoList.RemoveAll();
	return 0;
}

INT CAutoCompleteList::Add_AutoCompleteData(CStringArray *aAddArray)
{
	const INT MAX = aAddArray->GetSize();
	for(INT nCount=0; nCount<MAX; nCount++)
	{
		Add_AutoCompleteData(aAddArray->GetAt(nCount));
	}

	return 0;
}

INT CAutoCompleteList::ShowAutoCompleteList(CString strCompare)
{	
	
	strCompare.MakeLower();

	// ȭ�� ���� ����(AddString�� ȭ���½�� ���󰳼�)
 	m_List.SetRedraw(FALSE);
	// ���� ����Ʈ�� ��� ���� ����
 	m_List.ResetContent();
	
	if(_T("") == strCompare)
	{
		if(TRUE == m_List.IsWindowVisible()) ShowWindow(SW_HIDE);
		return 1;
	}

	CString strGet = _T("");
	CString strTmp = _T("");
	const INT MAX = GetCount_AutoCompleteData();

	INT nAddListCount = 1;
	for(INT nCount=0; nCount<MAX; nCount++)
	{
		GetAt_AutoCompleteData(nCount, &strGet);
		strTmp = strGet;
		strTmp.MakeLower();

		if(0 <= strTmp.Find(strCompare))
		{
			if(AUTO_LIST_MAX >= nAddListCount)
			{
				m_List.AddString(strGet);
				nAddListCount++;
			}
			else break;
		}
	}
	

	if(0 < m_List.GetCount() && FALSE == m_List.IsWindowVisible() && TRUE == m_bIsVisible)
	{
		HCURSOR hCursor = LoadCursor(NULL ,IDC_ARROW);
		SetCursor(hCursor);

 	 	SetCapture();	
 		ShowWindow(SW_SHOWNA);
	
	}
 	else if(0 >= m_List.GetCount() && TRUE == m_List.IsWindowVisible())
 		ShowWindow(SW_HIDE);
	
	//++2008.07.23 ADD BY CJY {{++
	// �ڵ��ϼ� �������� ������ �°� ũ�⸦ �����Ѵ�
	CRect rDlg;
	GetWindowRect(rDlg);

	// �����ϳ��� ���̸� ���Ѵ���
	CDC* pDC = m_List.GetDC();
	CFont* pOldFont = (CFont*)pDC->SelectStockObject(DEFAULT_GUI_FONT);
	CSize size = pDC->GetTextExtent(_T(" "));
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	// ���� �ؽ�Ʈ�� ��µǴ� ClientRect ������ WindowRect����
	// ���̰� 2�ȼ� ����.
	// ���̾�α��� 2�ȼ��� ����Ʈ�ڽ��� 2�ȼ����ؼ� 4�ȼ��� ���ؾ�
	// ���� �ؽ�Ʈ�� ��µ� �� �߸��� �ʴ´�
	const INT NEW_HEIGHT = ((nAddListCount-1)*size.cy) + 4;
	rDlg.bottom = rDlg.top + NEW_HEIGHT;
	MoveWindow(rDlg);

	//--2008.07.23 ADD BY CJY --}}

	//++2008.08.05 ADD BY CJY {{++
	if(0 < m_List.GetCount())
	{
		m_List.SetCurSel(0);
	}
	//--2008.08.05 ADD BY CJY --}}

	
	// ȭ�鰻�� ���
 	m_List.SetRedraw(TRUE);
	return 0;
}

INT CAutoCompleteList::GetAt_AutoCompleteData(INT nIndex, CString *strGet)
{
	if(TRUE == Is_BadIndex(nIndex)) return -1;
	if(NULL == strGet) return -2;

	*strGet = m_strAutoList.GetAt(nIndex);

	return 0;
}

BOOL CAutoCompleteList::Is_BadIndex(INT nIndex)
{
	if(0 > nIndex || nIndex >= GetCount_AutoCompleteData()) return TRUE;
	
	return FALSE;
}

INT CAutoCompleteList::GetCount_AutoCompleteData()
{
	return m_strAutoList.GetSize();
}

void CAutoCompleteList::Set_Visible(BOOL bIsVisible)
{
	m_bIsVisible = bIsVisible;
}

INT CAutoCompleteList::Set_NextCurSel()
{
	const INT nIndex = m_List.GetCurSel();
	const INT nCount = m_List.GetCount();
	
	if(nCount <= nIndex+1) return -1;
	else m_List.SetCurSel(nIndex+1);
	SelChange();

	return 0;
}

INT CAutoCompleteList::Set_PrevCurSel()
{
	const INT nIndex = m_List.GetCurSel();
	const INT nCount = m_List.GetCount();
	
	if(0 > nIndex-1) return -1;
	else m_List.SetCurSel(nIndex-1);
	SelChange();

	return 0;

}

INT CAutoCompleteList::Set_HeadCurSel()
{
	const INT nCount = m_List.GetCount();
	if(0 == nCount) return -1;
	else m_List.SetCurSel(0);
	SelChange();

	return 0;
}

INT CAutoCompleteList::Set_TailCurSel()
{
	const INT nCount = m_List.GetCount();
	if(0 == nCount) return -1;
	else m_List.SetCurSel(nCount-1);
	SelChange();

	return 0;
}

void CAutoCompleteList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rRect;
	m_List.GetClientRect(rRect);

	ReleaseCapture();
	ShowWindow(SW_HIDE);

	// ���콺�� ��ġ�� �ڵ��ϼ�������� ������
	// ���� ���콺��ġ�� �ִ� ����Ʈ�� ���ý�Ų��.
	if(TRUE == rRect.PtInRect(point))
	{	
		
		point.y -= rRect.top;
		INT nItemSize = rRect.Height() / m_List.GetCount();
		INT nIndex = point.y / nItemSize;

		m_List.SetCurSel(nIndex);
		OnSelchangeListAutoComplete();
	}
	else
	{
		// �ٸ� ���� Ŭ���Ͽ��� ��� �� ������ ���콺 Ŭ�� �޽����� ������
		CPoint Send_Point = point;
		ClientToScreen(&Send_Point);
		CWnd* pTopWnd = GetTopLevelParent();
		CWnd* pWnd = pTopWnd->WindowFromPoint(Send_Point);	
	 	pWnd->PostMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(Send_Point.x, Send_Point.y));
	}
	ShowWindow(SW_HIDE);
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CAutoCompleteList::OnShowWindow(BOOL bShow, UINT nStatus) 
{
 	if(SW_HIDE == bShow) ReleaseCapture();
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
 	
}

void CAutoCompleteList::OnSelchangeListAutoComplete() 
{
	// TODO: Add your control notification handler code here		
	SelChange();
	ShowWindow(SW_HIDE);

	NMHDR hdr;
	hdr.hwndFrom = m_hWnd;
	hdr.idFrom = IDD;
	hdr.code = VK_RETURN;
	m_pParent->SendMessage(WM_NOTIFY, IDD, (LPARAM)&hdr);
}

CString CAutoCompleteList::Get_CurSelText()
{
	CString strData = _T("");
	const INT nSelect = m_List.GetCurSel();

	if(0 > nSelect) return _T("");
	else m_List.GetText(m_List.GetCurSel(), strData);

	return strData;
}

void CAutoCompleteList::SelChange()
{
	if(NULL != m_pParent && NULL != m_pParent->GetSafeHwnd())
	{
		NMHDR hdr;
		hdr.hwndFrom = m_hWnd;
		hdr.idFrom = IDD;
		hdr.code = LBN_SELCHANGE;
		m_pParent->SendMessage(WM_NOTIFY, IDD, (LPARAM)&hdr);
	}	
}
