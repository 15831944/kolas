// DlgSEQ.cpp : implementation file
//

#include "stdafx.h"
#include "kalendar.h"
#include "DlgSEQ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSEQ dialog


CDlgSEQ::CDlgSEQ(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSEQ::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSEQ)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nArrayIndex_Of_SelectPopupStatic = -1;
	m_uDayOfTheWeek = WEEKDAY;
	m_bIsInitDataStatic[TITLE_STATIC] = FALSE;
	m_bIsInitDataStatic[POPUP_STATIC] = FALSE;
	m_bIsInitDataStatic[2] = FALSE;
	m_bIsInitDataStatic[3] = FALSE;

	m_pOldFont = NULL;
	m_pNewFont = NULL;

	// KOL.UDF.022
	//DATA_STATIC* m_pPrev_Data_Static = NULL;
	m_pPrev_Data_Static = NULL;

	m_strToolTipText_For_Mouse_In_the_PopupMenu = _T("");

	m_bIsHoliday = FALSE;
	m_bIsToday = FALSE;
	// KOL.UDF.022
	m_pSelect_Data_Static = NULL;
	m_nArrayIndex_Of_SelectPopupStatic = 0;
	m_uDayOfTheWeek = 0;
	m_uYear = 0;
	m_uMonth = 0;
	m_uDay = 0;	
}

CDlgSEQ::~CDlgSEQ()
{
	RemoveAll_DataStatic();
	if(NULL != m_pNewFont) delete m_pNewFont;
	if(NULL != m_pOldFont) delete m_pOldFont;

	m_Brush.DeleteObject();	

}
void CDlgSEQ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSEQ)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSEQ, CDialog)
	//{{AFX_MSG_MAP(CDlgSEQ)
	ON_COMMAND_RANGE(ID_DIVISION_NO1, ID_DIVISION_NO100, OnButtonPopupMenu)
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()	
	ON_WM_TIMER()	
	ON_WM_MENUSELECT()
	ON_WM_MOUSEMOVE()
	ON_WM_ENTERIDLE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSEQ message handlers

BOOL CDlgSEQ::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Brush.CreateSolidBrush(RGB(255,255,255));

	////////////////////////////////////////////////////////////////////////
	// ���⼭���� ���콺�� �÷����ִ� ����ƽ�� ������ ������ �ʱ�ȭ
	CFont* pFont = GetFont();

	LOGFONT LogFont;
	pFont->GetLogFont(&LogFont);

	m_pOldFont = new CFont;
	m_pOldFont->CreateFontIndirect(&LogFont);
	
	LogFont.lfUnderline = TRUE;
	
	m_pNewFont = new CFont;
	m_pNewFont->CreateFontIndirect(&LogFont);

	m_pPrev_Data_Static = NULL;
	m_pSelect_Data_Static = NULL;
	// ������� ���콺�� �÷����ִ� ����ƽ�� ������ ������ �ʱ�ȭ
	////////////////////////////////////////////////////////////////////////
	
	m_ToolTip.Create(this);
	m_ToolTip.SetDelay(300);
	// ���콺�� ��ġ���� ���� ��µ� �������� ��ġ�� ����ġ
	m_ToolTip.SetOffset(20, 20);	// ���콺�� ��ġ���� 10, 10�� �߰��� ������ ���� �����ֱ�
	m_ToolTip.On(TRUE);

	Init_DataStatic();

	InitBG();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSEQ::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//DEL  	INT nArrayIndex = 0, nIndex = 0;
//DEL  	if(0 <= Find_StaticIndex_InMouse(&nArrayIndex, &nIndex))
//DEL  	{
//DEL  		if(POPUP_STATIC == nIndex)
//DEL  		{
//DEL  			DATA_STATIC_EX *pDATA_STATIC = (DATA_STATIC_EX*)m_aStaticArray[nArrayIndex].GetAt(POPUP_STATIC);
//DEL  			OnButtonPopupStatic(pDATA_STATIC, nArrayIndex, nIndex);			
//DEL  		}
//DEL  		else
//DEL  		{		
//DEL  			m_ToolTip.Hide();
//DEL  			m_nArrayIndex_Of_SelectPopupStatic = -1;
//DEL  
//DEL  
//DEL  			// ���õ� ������ ����
//DEL  			CData* pData = new CData;
//DEL  			Get_SelectData(pData);
//DEL  			GetParent()->SendMessage(WM_NOTIFY, IDD, (LPARAM)pData);
//DEL  			delete pData;
//DEL  		}
//DEL  	}

	CDialog::OnLButtonDown(nFlags, point);
}

HBRUSH CDlgSEQ::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//DEL 	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor==CTLCOLOR_DLG)
	{
	//DEL  	pDC->SetBkMode(TRANSPARENT);		
		//(HBRUSH)::CreateSolidBrush(RGB(255, 255, 255));
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	else if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(128, 128, 128));

		if(NULL != m_pSelect_Data_Static)
		{
			if(pWnd->GetSafeHwnd() ==
				m_pSelect_Data_Static->mCtrl_Static.GetSafeHwnd()
				)
			{
				pDC->SelectObject(m_pNewFont);
				pDC->SetTextColor(RGB(200, 50, 50));
			}
		}
	}
	// TODO: Return a different brush if the default is not desired
	return m_Brush;

	// TODO: Return a different brush if the default is not desired
}


// �Լ��̸�: CDlgSEQ::OnSetCursor
// �Լ�����: Ŀ�� ����
// ���콺�� ����ƽ���� �÷��� �ִ��� �Ǵ��� Ŀ������
// ����, ���� ������ ����ƽ ����(����Ʈ ���� �� ����)
// ��ȯ  ��: BOOL 
// ����[CWnd* pWnd] : 
// ����[UINT nHitTest] : 
// ����[UINT message] : 
BOOL CDlgSEQ::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default	
 	KillTimer(ID_TIMER_FOR_MOUSE_WITHOUT_DLGSEQ_CLIENT);
	DATA_STATIC* pDATA_STATIC = NULL;

	// ���콺�� ����ƽ���� �÷������� ���
	if(0 <= Get_StaticInMouse(&pDATA_STATIC))
	{	
		CString strMsg = _T("AUTO_HIDE:FALSE");
		GetParent()->SendMessage(WM_REQUEST_KALENDARDLG, IDD, (LPARAM)&strMsg);

		SetCursor(LoadCursor(NULL, IDC_HAND));	   	
		m_pSelect_Data_Static = pDATA_STATIC;
		pDATA_STATIC->mCtrl_Static.Invalidate();
		
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		if(_T("") != pDATA_STATIC->m_strToolTipText)
		{		 	
			m_ToolTip.Set(point, pDATA_STATIC->m_strToolTipText);
		}
 		SetTimer(ID_TIMER_FOR_MOUSE_WITHOUT_DLGSEQ_CLIENT, 100, NULL);
	}
	else
	{
		CString strMsg = _T("AUTO_HIDE:TRUE");
		GetParent()->SendMessage(WM_REQUEST_KALENDARDLG, IDD, (LPARAM)&strMsg);

		SetCursor(LoadCursor(NULL, IDC_ARROW));
		m_pSelect_Data_Static = NULL;
		m_ToolTip.Hide();
	}
	
 	if(pDATA_STATIC != m_pPrev_Data_Static)
 	{	
 		if(NULL != m_pPrev_Data_Static)	  
 			m_pPrev_Data_Static->mCtrl_Static.Invalidate();
 		m_pPrev_Data_Static = pDATA_STATIC;
 	}
	return TRUE;
}

INT CDlgSEQ::AddData(CString strStatus, CString strData, CString strCaption, CString strToolTipText)
{
	INT nMode = -1;
	if(_T("����") == strStatus)
	{
		nMode = ACQ;
	}
	else if(_T("�˼�") == strStatus)
	{
		nMode = CHECKIN;
	}
	else if(_T("�̳�") == strStatus)
	{
		nMode = MISSING;
	}
	else if(_T("���") == strStatus)
	{
		nMode = REG;
	}
	else return -1;

	if(FALSE == m_bIsInitDataStatic[nMode]) return -1;
	
	if(_T("") == strCaption) strCaption = strData;	



	CArray<DATA_STATIC*, DATA_STATIC*>* pArray = &m_aStaticArray[nMode];

	// Ÿ��Ʋ ����ƽ�� ������ �߰�
	DATA_STATIC* pDATA_STATIC = (DATA_STATIC*)pArray->GetAt(TITLE_STATIC);
	pDATA_STATIC->m_strData += _T(", ") + strData;
	pDATA_STATIC->m_strData.TrimLeft(_T(", "));
	if(_T("") != strToolTipText)
		pDATA_STATIC->m_strToolTipText = 
			pDATA_STATIC->m_strToolTipText +
			_T("\n") +
			strToolTipText;
	pDATA_STATIC->m_strToolTipText.TrimLeft(_T("\n"));
	pDATA_STATIC->mCtrl_Static.ShowWindow(SW_SHOWNA);


	// �˾�����ƽ�� ������̸�(�Ϲ� ����ƽ�� ���̻� �����͸� �߰� ����Ű�� ���)
  //	DATA_STATIC* pPopup_Static = (DATA_STATIC*)pArray->GetAt(POPUP_STATIC);
	DATA_STATIC* pNo_3_Static = (DATA_STATIC*)pArray->GetAt(NO_3_STATIC);
 	if(/*TRUE == pPopup_Static->m_bUse || */TRUE == pNo_3_Static->m_bUse)
	{	

/////////////////////////////////////////////////////////////////////////////////
{// ���⼭���� 	�˾� ����ƽ�� �� ó�� �����Ͱ� ������ ����� ó��		
		// ������ �������ִ� �˾� ����ƽ�� �����־�� �ϴ� ��,
		// ������ ������ȣ ����ƽ�� �˾� ����ƽ�� ��ĥ��츦 ���� ó��
		// �� ��� ������ ����ƽ�� �����, ������ ����ƽ�� �ִ�
		// �����͸� �˾� ����ƽ�� �߰���Ų��.

		DATA_STATIC_EX* pPopup_Static = 
			(DATA_STATIC_EX*)(DATA_STATIC*)	pArray->GetAt(POPUP_STATIC);
		CRect rPopup = pPopup_Static->m_rStaticRect;
		
		for(INT nIndex = NO_1_STATIC; nIndex <= NO_3_STATIC; nIndex++)
		{
			DATA_STATIC* pNo_Static = pArray->GetAt(nIndex);
			CRect rNo = pNo_Static->m_rStaticRect;
			// ���� ������ȣ ����ƽ�� ��ġ�� �˾� ����Ƽ��� ��ĥ ��
			if(rNo.right >= rPopup.left - 3)	// -3�� ����ƽ���� ����
			{
				// ��ȣ ����ƽ�� ���ܹ�����
				pNo_Static->mCtrl_Static.MoveWindow(CRect(0, 0, 0, 0));
				pNo_Static->m_rStaticRect = CRect(0, 0, 0, 0);
				
				// ���� ����ƽ�� �����͸� �˾�����ƽ�� �߰���Ų��			
				//DEL 	pPopup_Static->m_strData += _T(", ") + pNo_3_Static->m_strData;
				//DEL 	pPopup_Static->m_strData.TrimLeft(_T(", "));
				pPopup_Static->m_aData.Add(pNo_Static->m_strData);
				pPopup_Static->m_aCaption.Add(pNo_Static->m_strCaption);
				pPopup_Static->m_aToolTipText.Add(pNo_Static->m_strToolTipText);

				pPopup_Static->m_strToolTipText = 
					pPopup_Static->m_strToolTipText + 
					_T("\n") + 
					pNo_Static->m_strToolTipText;
				pPopup_Static->m_strToolTipText.TrimLeft(_T("\n"));
				
				pNo_Static->m_strData = _T("");
				pNo_Static->m_strCaption = _T("");
				pNo_Static->m_strToolTipText = _T("");
				
			}
		}
		// �˾�����ƽ�� �����ش�
		pPopup_Static->mCtrl_Static.ShowWindow(SW_SHOW);
		pPopup_Static->m_bUse = TRUE;

}//  ������� �˾� ����ƽ�� �� ó�� �����Ͱ� ������ ����� ó��
/////////////////////////////////////////////////////////////////////////////////
		DATA_STATIC_EX* pPopup_Static = (DATA_STATIC_EX*)pArray->GetAt(POPUP_STATIC);
		// �˾�����ƽ�� ������ �߰�
//DEL 		pPopup_Static->m_strData += _T(", ") + strData;
//DEL 		pPopup_Static->m_strData.TrimLeft(_T(", "));
		pPopup_Static->m_aData.Add(strData);
		pPopup_Static->m_aCaption.Add(strCaption);
		pPopup_Static->m_aToolTipText.Add(strToolTipText);
		if(_T("") != strToolTipText)
			pPopup_Static->m_strToolTipText = 
				pPopup_Static->m_strToolTipText + 
				_T("\n") + 
				strToolTipText;
		pPopup_Static->m_strToolTipText.TrimLeft(_T("\n"));
	}
	// ����ƽ�� �����ͼ��� ���ɽ�
	else
	{
		// ����ƽ�� ��ġ�� �����ϱ� ���� ���� ����ƽ�� ��ġ�� ��´�.		
		for(INT nIndex = NO_1_STATIC; nIndex <= NO_3_STATIC; nIndex++)
		{
			DATA_STATIC* pDATA_STATIC = (DATA_STATIC*)pArray->GetAt(nIndex);
			if(FALSE == pDATA_STATIC->m_bUse) break;
		}
		if(NO_3_STATIC < nIndex) return -1;

		DATA_STATIC* pDATA_STATIC = (DATA_STATIC*)pArray->GetAt(nIndex-1);
		INT nLeft = pDATA_STATIC->m_rStaticRect.right + 5;// 5�� ����ƽ���� ����
		INT nTop = pDATA_STATIC->m_rStaticRect.top;		
		INT nBottom = pDATA_STATIC->m_rStaticRect.bottom;
		
		// ������ �ؽ�Ʈ�� ũ�⸦ ��´�(ȭ�� ��¿�)
		CRect rStatic = CalculateTextSize(strCaption, CRect(nLeft, nTop, nLeft+10, nBottom));

//DEL 		DATA_STATIC_EX* pPopup_Static = 
//DEL 			(DATA_STATIC_EX*)pArray->GetAt(POPUP_STATIC);
//DEL 		// ����° ������ȣ ����ƽ�� ��ġ�� �˾� ����Ƽ��� ��ĥ ��
//DEL 		if(rStatic.right >= pPopup_Static->rStaticRect.left - 3)	//
		// ���̸� ������� üũ
		if(rStatic.right >= RIGHT)
		{
			// ��� ��� �˾�����ƽ�� Ȱ��ȭ
			DATA_STATIC_EX* pPopup_Static = 
				(DATA_STATIC_EX*)pArray->GetAt(POPUP_STATIC);
			pPopup_Static->m_aCaption.Add(strCaption);
			pPopup_Static->m_aData.Add(strData);
			pPopup_Static->m_aToolTipText.Add(strToolTipText);
			pPopup_Static->mCtrl_Static.ShowWindow(SW_SHOWNA);
 			pPopup_Static->m_bUse = TRUE;			
			if(_T("") != strToolTipText)
				pPopup_Static->m_strToolTipText = 
					pPopup_Static->m_strToolTipText + 
					_T("\n") + 
					strToolTipText;
			pPopup_Static->m_strToolTipText.TrimLeft(_T("\n"));
//DEL 			// �� ���ĺ��ʹ� �˾�����ƽ�� �߰��� �ϱ����� 
//DEL 			// ���� ����ƽ�� �������� no_3_static�� ��������� ó���Ѵ�
//DEL 			pDATA_STATIC = (DATA_STATIC*)pArray->GetAt(NO_3_STATIC);
//DEL 			pDATA_STATIC->m_bUse = TRUE;
		}

		// ����� ������ �����͸� �����Ѵ�.
		else
		{
			pDATA_STATIC = (DATA_STATIC*)pArray->GetAt(nIndex);
			pDATA_STATIC->mCtrl_Static.SetWindowText(strCaption);
			pDATA_STATIC->mCtrl_Static.MoveWindow(rStatic);
			pDATA_STATIC->m_rStaticRect = rStatic;
			pDATA_STATIC->m_strCaption = strCaption;
			pDATA_STATIC->m_strData = strData;
			pDATA_STATIC->m_strToolTipText = strToolTipText;
			pDATA_STATIC->m_bUse = TRUE;
			pDATA_STATIC->m_nDataSize = CSize(rStatic.Width(), rStatic.Height());

		}
		DATA_STATIC_EX* pPopup_Static = (DATA_STATIC_EX*)pArray->GetAt(POPUP_STATIC);
 		// ����° ������ȣ ����ƽ�� ��ġ�� �˾� ����Ƽ��� ��ĥ ��
 		if(rStatic.right >= pPopup_Static->m_rStaticRect.left - 3)
		{
			// �� ���ĺ��ʹ� �˾�����ƽ�� �߰��� �ϱ����� 
			// ���� ����ƽ�� �������� no_3_static�� ��������� ó���Ѵ�
			pDATA_STATIC = (DATA_STATIC*)pArray->GetAt(NO_3_STATIC);
			pDATA_STATIC->m_bUse = TRUE;
		}
	}
	
	return 0;
}

CRect CDlgSEQ::CalculateTextSize(const CString strText, const CRect rTextRect, CFont* pFont)
{	
	pFont = (NULL == pFont)? GetFont() : pFont;
	// ��Ʈ���� 			
 	CDC* pDC = GetDC(); 	// DC����	
	CFont* OldFont = pDC->SelectObject(pFont);//DEL (CFont*)pDC->SelectStockObject(DEFAULT_GUI_FONT);
		
	// ���
	CRect rText = rTextRect;
	pDC->DrawText(strText, &rText, DT_EDITCONTROL | DT_CALCRECT);
	
	pDC->SelectObject(OldFont);
	ReleaseDC(pDC);
	
	return rText;
}

void CDlgSEQ::SetWeekDay()
{
	m_uDayOfTheWeek = WEEKDAY;
}

void CDlgSEQ::SetSunday()
{
	m_uDayOfTheWeek = SUNDAY;
}

void CDlgSEQ::SetSaturday()
{
	m_uDayOfTheWeek = SATURDAY;
}

// void CDlgSEQ::OnPaint() 
// {
//  	CPaintDC dc(this); // device context for painting
//  	
//  	// TODO: Add your message handler code here
//  	CBitmap MemBitmap, Bitmap[2];
//  	CDC DC[2];
//  	
//  	CDC MemDC;
//  	MemDC.CreateCompatibleDC(&dc);
//  	MemBitmap.CreateCompatibleBitmap(&dc, 20, 15);
//  	MemDC.SelectObject(&MemBitmap);
//  
//  	UINT uNum[2] = {0,0};
//  	UINT uTmp = m_uDay;
//  	UINT BitmapDay = IDB_BITMAP_DAY_BLACK_0;
//  	if(SATURDAY == m_uDayOfTheWeek)
//  		BitmapDay = IDB_BITMAP_DAY_BLUE_0;
//  	if(SUNDAY == m_uDayOfTheWeek)
//  		BitmapDay = IDB_BITMAP_DAY_RED_0;
//  
//  	for(UINT i=0;i<2;i++)
//  	{
//  		uNum[1-i] = uTmp % 10;
//  		uTmp = uTmp / 10;
//  	}		
//  
//  	if(0==uNum[0] && 0==uNum[1])
//  	{
//  		for(i=0;i<2;i++)
//  		{
//  			DC[i].CreateCompatibleDC(&dc);	
//  			Bitmap[i].LoadBitmap(IDB_BITMAP_DAY_NULL);		
//  			DC[i].SelectObject(&Bitmap[i]);
//  		}
//  	}
//  	else
//  	{
//  		for(i=0;i<2;i++)
//  		{
//  			DC[i].CreateCompatibleDC(&dc);				
//  			Bitmap[i].LoadBitmap(BitmapDay + uNum[i]);		
//  			DC[i].SelectObject(&Bitmap[i]);
//  		}
//  	}
//  
//  	for(i=0;i<2;i++)
//  	{
//  		MemDC.BitBlt(i*10,0,10, 17, &DC[i],0,0,SRCCOPY);
//  	}	
//  	
//   	CDC* pDC = GetDC();
//  	pDC->BitBlt(0,0,20,15,&MemDC,0,0,SRCCOPY);
//   	ReleaseDC(pDC);
// 	
// 	// Do not call CDialog::OnPaint() for painting messages
// }

// ��¥����
// ���� ��¥�� �����ϰ�, �� ��¥�� ����
void CDlgSEQ::SetDay(UINT uYear, UINT uMonth, UINT uDay)
{	

	if(FALSE==ChkDay(uYear, uMonth, uDay))
	{
		CString strTemp;
		strTemp.Format(_T("ERROR : [CDlgSEQ::SetDay] �ùٸ��� ���� ��¥(%d/%d/%d)"), uYear, uMonth, uDay);
		LOG(strTemp);
		return;
	}
		// ��¥�� ����
		m_uYear = uYear;
		m_uMonth = uMonth;
		m_uDay = uDay;	
	
	//  ��������
/*	
	INT nMax = rand()%5;
	for(INT nCount=0; nCount < nMax; nCount++)
	{		
		INT nNum = rand()% ( ( ((rand()%2)*100) + ((rand()%10)*10) ) + 1);
		CString strTemp;
		strTemp.Format(_T("%d"), nNum);
		AddData(_T("����"), strTemp);
	}
	nMax = rand()%5;
	for(nCount=0; nCount < nMax; nCount++)
	{
		INT nNum = rand()% ( ( ((rand()%2)*100) + ((rand()%10)*10) ) + 1);
		CString strTemp;
		strTemp.Format(_T("%d"), nNum);
		AddData(_T("�˼�"), strTemp);
	}
	nMax = rand()%5;
	for(nCount=0; nCount < nMax; nCount++)
	{
		INT nNum = rand()% ( ( ((rand()%2)*100) + ((rand()%10)*10) ) + 1);
		CString strTemp;
		strTemp.Format(_T("%d"), nNum);
		AddData(_T("�̳�"), strTemp);
	}
	nMax = rand()%5;
	for(nCount=0; nCount < nMax; nCount++)
	{
		INT nNum = rand()% ( ( ((rand()%2)*100) + ((rand()%10)*10) ) + 1);
		CString strTemp;
		strTemp.Format(_T("%d"), nNum);
		AddData(_T("���"), strTemp);
	}
*/	//
// 	OnPaint();
	Invalidate();
}


// �Լ��̸�: CDlgSEQ::ResetDay
// �Լ�����: ������ �ʱ�ȭ
// ��ȯ  ��: void 
void CDlgSEQ::ResetDay()
{
// 	GetDlgItem(IDC_STATIC_TODAY)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STATIC_HOLIDAY)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STATIC_TODAY_HOLIDAY)->ShowWindow(SW_HIDE);

	m_bIsToday = FALSE;
	m_bIsHoliday = FALSE;

	m_uDay = 0;
	m_uMonth = 0;
	m_uYear = 0;
	m_nArrayIndex_Of_SelectPopupStatic = -1;

	DATA_STATIC* pDATA_STATIC = NULL;
	DATA_STATIC_EX* pDATA_STATIC_EX = NULL;
	for(INT nArrayIndex=0; nArrayIndex < ARRAY_COUNT; nArrayIndex++)
	{
		pDATA_STATIC = (DATA_STATIC*)m_aStaticArray[nArrayIndex].GetAt(TITLE_STATIC);
		pDATA_STATIC->m_strData = _T("");
		pDATA_STATIC->m_strToolTipText = _T("");
		pDATA_STATIC->m_nDataSize = CSize(0, 0);
		pDATA_STATIC->mCtrl_Static.SetFont(m_pOldFont);
		pDATA_STATIC->mCtrl_Static.ShowWindow(SW_HIDE);

		pDATA_STATIC_EX = 
			(DATA_STATIC_EX*)m_aStaticArray[nArrayIndex].GetAt(POPUP_STATIC);
		pDATA_STATIC_EX->m_strData = _T("");
		pDATA_STATIC_EX->m_nDataSize = CSize(0, 0);
		pDATA_STATIC_EX->m_strToolTipText = _T("");		
		pDATA_STATIC_EX->mCtrl_Static.ShowWindow(SW_HIDE);
		pDATA_STATIC_EX->mCtrl_Static.SetFont(m_pOldFont);
		pDATA_STATIC_EX->m_bUse = FALSE;

		pDATA_STATIC_EX->m_aCaption.RemoveAll();
		pDATA_STATIC_EX->m_aData.RemoveAll();
		pDATA_STATIC_EX->m_aToolTipText.RemoveAll();

		for(INT nIndex=NO_1_STATIC; nIndex <= NO_3_STATIC; nIndex++)
		{
			pDATA_STATIC = (DATA_STATIC*)m_aStaticArray[nArrayIndex].GetAt(nIndex);
			pDATA_STATIC->m_bUse = FALSE;
			pDATA_STATIC->m_nDataSize = CSize(0, 0);			
			pDATA_STATIC->m_rStaticRect = CRect(0, 0, 0, 0);
			pDATA_STATIC->m_strCaption = _T("");
			pDATA_STATIC->m_strData = _T("");
			pDATA_STATIC->m_strToolTipText = _T("");
			
			pDATA_STATIC->mCtrl_Static.MoveWindow(CRect(0, 0, 0, 0));
			pDATA_STATIC->mCtrl_Static.SetWindowText(_T(""));
			pDATA_STATIC->mCtrl_Static.SetFont(m_pOldFont);
		}
	}
	m_pSelect_Data_Static = NULL;
	m_pPrev_Data_Static = NULL;

	Invalidate();
}

void CDlgSEQ::SetToday()
{
// 	GetDlgItem(IDC_STATIC_TODAY)->ShowWindow(SW_SHOW);	
	m_bIsToday = TRUE;
}


void CDlgSEQ::SetHoliday()
{
// 	if(!IsToday())
// 	{
// 		GetDlgItem(IDC_STATIC_HOLIDAY)->ShowWindow(SW_SHOW);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_TODAY)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_TODAY_HOLIDAY)->ShowWindow(SW_SHOW);
// 	}
	m_bIsHoliday = TRUE;		
}

BOOL CDlgSEQ::IsToday() const
{
	CTime t = CTime::GetCurrentTime();
	if((UINT)t.GetYear() != m_uYear)
		return FALSE;
	if((UINT)t.GetMonth() != m_uMonth)
		return FALSE;
	if((UINT)t.GetDay() != m_uDay)
		return FALSE;
	return TRUE;	
}

INT CDlgSEQ::Get_StaticInMouse(DATA_STATIC** pGet_Data)
{
	if(NULL == pGet_Data) return -1;
	DATA_STATIC* pDATA_STATIC = Get_StaticInMouse();
	if(NULL == pDATA_STATIC) return -2;
	*pGet_Data = pDATA_STATIC;

	return 0;
}

CDlgSEQ::DATA_STATIC* CDlgSEQ::Get_StaticInMouse()
{
	INT nArrayIndex = 0, nIndex = 0;
	INT nResult = Find_StaticIndex_InMouse(&nArrayIndex, &nIndex);
	if(0 > nResult) return NULL;

	DATA_STATIC* pDATA_STATIC =
		(DATA_STATIC*)m_aStaticArray[nArrayIndex].GetAt(nIndex);
	
	return pDATA_STATIC;
}


INT CDlgSEQ::Init_DataStatic()
{
	{
	CRect rStaticTitle = CRect(3, ACQ_TOP, 10, ACQ_BOTTOM);	
	rStaticTitle = CalculateTextSize(_T("����:"), rStaticTitle);
		
	CRect rStaticPopup = CRect(3, ACQ_TOP, 10, ACQ_BOTTOM);
	rStaticPopup = CalculateTextSize(_T("��"), rStaticPopup);
	const INT WIDTH = rStaticPopup.Width();
	rStaticPopup.right = RIGHT;
	rStaticPopup.left = RIGHT - WIDTH;
	
	INT nResult = Init_DataStatic(ACQ, _T("����:"), rStaticTitle, rStaticPopup);
	if(0 <= nResult) { m_bIsInitDataStatic[ACQ] = TRUE; }
	}

	{
	CRect rStaticTitle = CRect(3, CHECKIN_TOP, 10, CHECKIN_BOTTOM);	
	rStaticTitle = CalculateTextSize(_T("�˼�:"), rStaticTitle);
		
	CRect rStaticPopup = CRect(3, CHECKIN_TOP, 10, CHECKIN_BOTTOM);
	rStaticPopup = CalculateTextSize(_T("��"), rStaticPopup);
	const INT WIDTH = rStaticPopup.Width();
	rStaticPopup.right = RIGHT;
	rStaticPopup.left = RIGHT - WIDTH;
	
	INT nResult = Init_DataStatic(CHECKIN, _T("�˼�:"), rStaticTitle, rStaticPopup);
	if(0 <= nResult) { m_bIsInitDataStatic[CHECKIN] = TRUE; }
	}

	{
	CRect rStaticTitle = CRect(3, MISSING_TOP, 10, MISSING_TOP);	
	rStaticTitle = CalculateTextSize(_T("�̳�:"), rStaticTitle);
		
	CRect rStaticPopup = CRect(3, MISSING_TOP, 10, MISSING_TOP);
	rStaticPopup = CalculateTextSize(_T("��"), rStaticPopup);
	const INT WIDTH = rStaticPopup.Width();
	rStaticPopup.right = RIGHT;
	rStaticPopup.left = RIGHT - WIDTH;
	
	INT nResult = Init_DataStatic(MISSING, _T("�̳�:"), rStaticTitle, rStaticPopup);
	if(0 <= nResult) { m_bIsInitDataStatic[MISSING] = TRUE; }
	}

	
	{
	CRect rStaticTitle = CRect(3, REG_TOP, 10, REG_BOTTOM);	
	rStaticTitle = CalculateTextSize(_T("���:"), rStaticTitle);
		
	CRect rStaticPopup = CRect(3, REG_TOP, 10, REG_BOTTOM);
	rStaticPopup = CalculateTextSize(_T("��"), rStaticPopup);
	const INT WIDTH = rStaticPopup.Width();
	rStaticPopup.right = RIGHT;
	rStaticPopup.left = RIGHT - WIDTH;
	
	INT nResult = Init_DataStatic(REG, _T("���:"), rStaticTitle, rStaticPopup);
	if(0 <= nResult) { m_bIsInitDataStatic[REG] = TRUE; }
	}

	return 0;
}



// �Լ��̸�: CDlgSEQ::Init_DataStatic
// �Լ�����: ����ƽ�� �������� �����Ѵ�
// ��>
//		[Ÿ��Ʋ][����1][����2][����3][�˾�]
//		[����: ][ 12  ][ 15  ][ 20   ][��  ]
// ��ȯ  ��: INT 
// ����[INT nArrayIndex] : �ε����� ����(����ACQ)
// ����[CString strCaption] : ȭ�鿡 ������ �ؽ�Ʈ
// ����[CRect rStaticTitle] : Ÿ��Ʋ����ƽ�� ũ��
// ����[CRect rStaticPopup] : �˾�����ƽ�� ũ��
INT CDlgSEQ::Init_DataStatic(INT nArrayIndex, CString strCaption, CRect rStaticTitle, CRect rStaticPopup)
{
	if(ACQ != nArrayIndex &&
		CHECKIN != nArrayIndex &&
		MISSING != nArrayIndex &&
		REG !=	nArrayIndex
		) return -1;
	
	// �迭�� ���� ũ��� 5��
	m_aStaticArray[nArrayIndex].SetSize(5);

	// Ÿ��Ʋ ����ƽ
	DATA_STATIC* pDATA_STATIC = New_DataStatic(strCaption, _T(""), rStaticTitle);
	pDATA_STATIC->m_nArrayIndex = nArrayIndex;	
	pDATA_STATIC->m_nDataIndex = TITLE_STATIC;
	m_aStaticArray[nArrayIndex].SetAt(TITLE_STATIC, pDATA_STATIC);
	
	// �˾� ����ƽ
//DEL 	pDATA_STATIC = New_DataStatic_Ex(_T(">>"), _T(""), rStaticPopup);
	pDATA_STATIC = New_DataStatic_Ex(_T("��"), _T(""), rStaticPopup);
	pDATA_STATIC->m_nArrayIndex = nArrayIndex;
	pDATA_STATIC->m_nDataIndex = POPUP_STATIC;
	pDATA_STATIC->mCtrl_Static.ShowWindow(SW_HIDE);
	m_aStaticArray[nArrayIndex].SetAt(POPUP_STATIC, pDATA_STATIC);
	

	// 3�� ��������ƽ�� ����
	for(INT nIndex=NO_1_STATIC; nIndex <= NO_3_STATIC; nIndex++)
	{
		pDATA_STATIC = New_DataStatic(_T(""), _T(""), CRect(0,0,0,0));
		pDATA_STATIC->m_bUse = FALSE;
		pDATA_STATIC->m_nArrayIndex = nArrayIndex;
		pDATA_STATIC->m_nDataIndex = nIndex;
		m_aStaticArray[nArrayIndex].SetAt(nIndex, pDATA_STATIC);		
	}
	return 0;
}


// �Լ��̸�: CDlgSEQ::New_DataStatic
// �Լ�����: ������ ����ƽ ���� ����
// ��ȯ  ��: CDlgSEQ::DATA_STATIC* 
// ����[CString strCaption] : 
// ����[CString strData] : 
// ����[CRect rStatic] : 
// ����[CFont *pFont] : 
CDlgSEQ::DATA_STATIC* CDlgSEQ::New_DataStatic(CString strCaption, CString strData, CRect rStatic, CFont *pFont)
{
	DATA_STATIC* pDATA_STATIC = new DATA_STATIC;
	
	pFont = (NULL == pFont)? GetFont() : pFont;

	pDATA_STATIC->m_strData = strData;
	pDATA_STATIC->mCtrl_Static.Create(strCaption, WS_VISIBLE, rStatic, this);
	pDATA_STATIC->mCtrl_Static.SetFont(pFont);
	pDATA_STATIC->m_nDataSize = CSize(rStatic.Width(), rStatic.Height());
	pDATA_STATIC->m_rStaticRect = rStatic;
	pDATA_STATIC->m_bUse = TRUE;

	return pDATA_STATIC;
}

CDlgSEQ::DATA_STATIC_EX* CDlgSEQ::New_DataStatic_Ex(CString strCaption, CString strData, CRect rStatic, CFont *pFont)
{
	DATA_STATIC_EX* pDATA_STATIC = new DATA_STATIC_EX;
	
	pFont = (NULL == pFont)? GetFont() : pFont;

	pDATA_STATIC->m_strData = strData;
	pDATA_STATIC->mCtrl_Static.Create(strCaption, WS_VISIBLE, rStatic, this);
	pDATA_STATIC->mCtrl_Static.SetFont(pFont);
	pDATA_STATIC->m_nDataSize = CSize(rStatic.Width(), rStatic.Height());
	pDATA_STATIC->m_rStaticRect = rStatic;
	pDATA_STATIC->m_bUse = TRUE;

	return pDATA_STATIC;	
}


// �Լ��̸�: CDlgSEQ::RemoveAll_DataStatic
// �Լ�����: �����ͽ���ƽ �޸� ����
// ��ȯ  ��: INT 
INT CDlgSEQ::RemoveAll_DataStatic()
{
	DATA_STATIC* pDATA_STATIC = NULL;	

	for(INT nArrayCount = 0; nArrayCount < ARRAY_COUNT; nArrayCount++)
	{
		for(INT nRemainCount = m_aStaticArray[nArrayCount].GetSize()-1; 0 <= nRemainCount ; nRemainCount--)
		{
			pDATA_STATIC = (DATA_STATIC*)m_aStaticArray[nArrayCount].GetAt(nRemainCount);
			if(NULL != pDATA_STATIC)
			{
				// �˾�����ƽ�� Ȯ�� ������ ����ƽ�����̴�
				if(POPUP_STATIC == nRemainCount)
				{
					DATA_STATIC_EX* pDATA_STATIC_EX = 
						(DATA_STATIC_EX*)m_aStaticArray[nArrayCount].GetAt(nRemainCount);
					pDATA_STATIC_EX->m_aCaption.RemoveAll();
					pDATA_STATIC_EX->m_aData.RemoveAll();
					pDATA_STATIC_EX->m_aToolTipText.RemoveAll();

					delete pDATA_STATIC_EX;
				}
				else
				{
					delete pDATA_STATIC;
				}
			}		
		}
		m_aStaticArray[nArrayCount].RemoveAll();
	}

	return 0;
}

// �Լ��̸�: CDlgSEQ::Find_StaticIndex_InMouse
// �Լ�����: ���� ���콺�� ����Ű�� ����ƽ�������� �ε����� ���´�.
// ��ȯ  ��: INT 
// ����[INT *nArrayIndex] : 
// ����[INT *nIndex] : 
INT CDlgSEQ::Find_StaticIndex_InMouse(INT *nArrayIndex, INT *nIndex)
{	
	if(NULL == nArrayIndex)	return -1;
	if(NULL == nIndex)	return -2;

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	
	// ���� ���콺�� ��ġ��
	// ����, �˼�, �̳�, ��� ����ƽ������ ��� �迭�� �����ִ��� �Ǵ�
	CRect r[ARRAY_COUNT];
	r[ACQ] =	CRect(LEFT, ACQ_TOP, RIGHT, ACQ_BOTTOM);
	r[CHECKIN] =	CRect(LEFT, CHECKIN_TOP, RIGHT, CHECKIN_BOTTOM);
	r[MISSING] =	CRect(LEFT, MISSING_TOP, RIGHT, MISSING_BOTTOM);
	r[REG] =	CRect(LEFT, REG_TOP, RIGHT, REG_BOTTOM);
	
	for(INT nCount = 0; nCount < ARRAY_COUNT; nCount++)
	{
		if(TRUE == r[nCount].PtInRect(point)) break;
	}
	if(ARRAY_COUNT <= nCount) return -1;
		
	INT nArray = nCount;

	DATA_STATIC* pDATA_STATIC = NULL;
	CRect rCompare;	
	
	INT nMAX = m_aStaticArray[nArray].GetSize();
	for(INT nDataCount=0; nDataCount < nMAX; nDataCount++)
	{
		pDATA_STATIC = (DATA_STATIC*)m_aStaticArray[nArray].GetAt(nDataCount);
		rCompare = pDATA_STATIC->m_rStaticRect;
		if(TRUE == rCompare.PtInRect(point))
		{	
			// �˾�����ƽ/Ÿ��Ʋ ����ƽ�� ��� �⺻������ ���� ������ ��,
			// �̶��� ���� ������ �����Ѵ�
			if(TITLE_STATIC == nDataCount)
			{
				return -1;
			}
			else if(POPUP_STATIC == nDataCount)
			{
				if(SW_HIDE == pDATA_STATIC->mCtrl_Static.IsWindowVisible())
				{
					continue;
				}				
			}			
			*nArrayIndex	= nArray;
			*nIndex		= nDataCount;				
			return 0;
		}
	}

	*nArrayIndex	= -1;
	*nIndex		= -1;

	return -2;
}

void CDlgSEQ::OnButtonPopupStatic(DATA_STATIC_EX *pPopup_static, INT nArrayIndex, INT nDataIndex)
{
//DEL 	CStringArray aData;

	if(NULL == pPopup_static) return ;
	if(POPUP_STATIC != pPopup_static->m_nDataIndex) return ;
	m_nArrayIndex_Of_SelectPopupStatic = pPopup_static->m_nArrayIndex;

	const nMAX = pPopup_static->m_aCaption.GetSize();
	if(0 >= nMAX) return ;

	CString strMsg = _T("AUTO_HIDE:FALSE");
	GetParent()->SendMessage(WM_REQUEST_KALENDARDLG, IDD, (LPARAM)&strMsg);
	/////////////////////////////////////////////////////////////////////////
	{// ���⼭���� �˾��޴� ����
	CMenu  *sMenu,*pMenu;
	pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU_DIVISION_NO);		
  	sMenu = pMenu->GetSubMenu(0);	
 	sMenu->ModifyMenu(0, MF_BYPOSITION, ID_DIVISION_NO1, pPopup_static->m_aCaption.GetAt(0));
	for(INT nCount = 1, nID = ID_DIVISION_NO2; nCount < nMAX; nCount++, nID++)
	{		
		sMenu->InsertMenu(nCount, MF_BYPOSITION, nID, pPopup_static->m_aCaption.GetAt(nCount));
	}
	// �޴��� ����
	CRect rect = pPopup_static->m_rStaticRect;
	ClientToScreen(rect);

	// �˾��޴��� �����ʳ��� ȭ���� ����� ��츦 �����ϱ� ����
	// Į���ٰ� �����ʳ��� �������� �˾��޴��� ��ġ�� �ٸ��� �Ѵ�.
	CRect rDlg;
	GetWindowRect(rDlg);
	const LONG cxScreen = GetSystemMetrics(SM_CXSCREEN);
	
	if(cxScreen > rDlg.right +100)	// 100�� �ӽ� ����
	{				// �˾�ũ�⸦ ���� ����� �𸣰���
		sMenu->TrackPopupMenu(
			TPM_RIGHTBUTTON|TPM_LEFTALIGN,
			rect.right,
			rect.top,
			this
			);
	}
	else
	{	// Į���ٰ� �����ʳ��� ���� ���
		sMenu->TrackPopupMenu(
			TPM_RIGHTBUTTON|TPM_RIGHTALIGN,
			rect.left,
			rect.top,
			this
			);
	}
	delete pMenu;	
	}// ������� �˾��޴� ����
	/////////////////////////////////////////////////////////////////////////

}

INT CDlgSEQ::Reset_SelectStatic()
{
	if(NULL != m_pSelect_Data_Static)
		m_pSelect_Data_Static->mCtrl_Static.SetFont(m_pOldFont);

	return 0;
}

INT CDlgSEQ::Get_SelectData(CData *pData) const
{
//DEL 	if(NULL == m_pSelect_Data_Static) return -1;
//DEL 
//DEL 	CString strDataType = _T("");
//DEL 	switch(m_pSelect_Data_Static->m_nArrayIndex)
//DEL 	{
//DEL 	case ACQ:
//DEL 		strDataType = _T("����:");
//DEL 		break;
//DEL 	case CHECKIN:
//DEL 		strDataType = _T("�˼�:");
//DEL 		break;
//DEL 	case MISSING:
//DEL 		strDataType = _T("�̳�:");
//DEL 		break;
//DEL 	case REG:
//DEL 		strDataType = _T("���:");
//DEL 		break;	
//DEL 	}
//DEL 	pData->SetData(
//DEL 		m_uYear,
//DEL 		m_uMonth,
//DEL 		m_uDay,
//DEL 		strDataType + m_pSelect_Data_Static->m_strData
//DEL 		);
	return Get_Data(m_pSelect_Data_Static, pData);	
}

void CDlgSEQ::OnTimer(UINT nIDEvent) 
{	
	// TODO: Add your message handler code here and/or call default
	// ���콺�� ���� ���̾�α׿����� ��� ��츦 ���
	if(nIDEvent = ID_TIMER_FOR_MOUSE_WITHOUT_DLGSEQ_CLIENT)
	{
		if(NULL == Get_StaticInMouse())
		{
 			KillTimer(ID_TIMER_FOR_MOUSE_WITHOUT_DLGSEQ_CLIENT);

			SetCursor(LoadCursor(NULL, IDC_ARROW));
			if(NULL != m_pSelect_Data_Static)
			{
				m_pSelect_Data_Static->mCtrl_Static.SetFont(m_pOldFont);
				m_pSelect_Data_Static = NULL;
			}	
			if(NULL != m_pPrev_Data_Static)
			{	
				m_pPrev_Data_Static->mCtrl_Static.SetFont(m_pOldFont);
				m_pPrev_Data_Static = NULL;
			}
		}		
	}
	CDialog::OnTimer(nIDEvent);
}


BOOL CDlgSEQ::IsDay() const
{	
	if(0>=m_uDay) 
		return FALSE;	
	return TRUE;
}


// �Լ��̸�: CDlgSEQ::OnButtonPopupMenu
// �Լ�����: ����ڰ� �˾�����ƽ�� �޴��� ������ ��� 
// ����ڰ� ������ ID�� �ش��ϴ� ������ȣ�� �˾�����ƽ���κ��� �˾Ƴ�����
// �ش� ������ȣ�� ���õǾ����� CKalendarDlg�� �����Ѵ�
// ��ȯ  ��: void 
// ����[UINT nID] : 
void CDlgSEQ::OnButtonPopupMenu(UINT nID)
{
	m_ToolTip.Hide();

	// 1. �˾�����ƽ�� ��� �������¿� �ش��ϴ��� �˾Ƴ�����
	INT nArrayIndex = -1;
	switch(m_nArrayIndex_Of_SelectPopupStatic)
	{	
	case ACQ:
	case CHECKIN:
	case MISSING:
	case REG:
		nArrayIndex = m_nArrayIndex_Of_SelectPopupStatic;
		break;
	default:
		return;
		break;
	}

	// 2. �ش� �˾�����ƽ�� �����͸� ���´�.
//DEL 	DATA_STATIC* pDATA_STATIC = m_aStaticArray[nArrayIndex].GetAt(POPUP_STATIC);

	// 3. ���� ����ڰ� ������ ���̵� �ش��ϴ� ������ȣ�� ��´�.	
//DEL 	CString strData = pDATA_STATIC->m_strData;
//DEL 	strData.TrimRight(_T(", "));
//DEL 	strData += _T(", ");
//DEL 
//DEL 	INT nIndex = 0;
//DEL 	INT nStart = 0;
//DEL 	CString strDivisionNo = _T("");

//DEL 	for(UINT nFindID = ID_DIVISION_NO1; nFindIndex <= nID; nFindIndex++)
//DEL 	{
//DEL  		nIndex = strData.Find(_T(", "), nStart);
//DEL  		if(nFindIndex == nID)
//DEL  			strDivisionNo = strData.Mid(nStart, nIndex - nStart);
//DEL  		nStart = nIndex+2;
//DEL  	}
	

//DEL 	// 4. KalendarDlg�� �����Ѵ�	
//DEL 	CString strBackup = pDATA_STATIC->m_strData;
//DEL 	pDATA_STATIC->m_strData = strDivisionNo;
	
	CData* pData = new CData;
	INT nIndex = nID - ID_DIVISION_NO1;
	DATA_STATIC_EX* pPopup_Static = 
		(DATA_STATIC_EX*)m_aStaticArray[nArrayIndex].GetAt(POPUP_STATIC);

	if(0 <= Get_Data_For_PopupStatic(pPopup_Static, pData, nIndex))
	{	
		GetParent()->SendMessage(WM_NOTIFY, IDD, (LPARAM)pData);
	}	
//DEL 	pDATA_STATIC->m_strData = strBackup;
	delete pData;
}

INT CDlgSEQ::Get_Data(const DATA_STATIC *pPDATA_STATIC, CData *pGetData) const
{	
	if(NULL == pPDATA_STATIC) return -1;
	
	CString strDataType = _T("");
	switch(pPDATA_STATIC->m_nArrayIndex)
	{
	case ACQ:
		strDataType = _T("����:");
		break;
	case CHECKIN:
		strDataType = _T("�˼�:");
		break;
	case MISSING:
		strDataType = _T("�̳�:");
		break;
	case REG:
		strDataType = _T("���:");
		break;	
	}
	if(_T("") == pPDATA_STATIC->m_strData) return -1;

	pGetData->SetData(
		m_uYear,
		m_uMonth,
		m_uDay,
		strDataType + pPDATA_STATIC->m_strData
		);

	return 0;
}

INT CDlgSEQ::Get_Data_For_PopupStatic(const DATA_STATIC_EX* pDATA_STATIC_EX, CData* pGetData, INT nIndex) const
{
	if(NULL == pDATA_STATIC_EX) return -1;
	
	CString strDataType = _T("");
	switch(pDATA_STATIC_EX->m_nArrayIndex)
	{
	case ACQ:
		strDataType = _T("����:");
		break;
	case CHECKIN:
		strDataType = _T("�˼�:");
		break;
	case MISSING:
		strDataType = _T("�̳�:");
		break;
	case REG:
		strDataType = _T("���:");
		break;	
	}

	pGetData->SetData(
		m_uYear,
		m_uMonth,
		m_uDay,
		strDataType + pDATA_STATIC_EX->m_aData.GetAt(nIndex)
		);

	return 0;
}

BOOL CDlgSEQ::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DestroyWindow();
}

void CDlgSEQ::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu) 
{
 	CDialog::OnMenuSelect(nItemID, nFlags, hSysMenu);
 
 	INT nArrayIndex = m_nArrayIndex_Of_SelectPopupStatic;
 	if(0 > nArrayIndex) return;
 
 	DATA_STATIC_EX* pPopup_static = 
 		(DATA_STATIC_EX*)m_aStaticArray[nArrayIndex].GetAt(POPUP_STATIC);
 
	// ITEM ID�� ID_DIVISION_NO1���� ���۵ȴ�
 	INT nIndex = nItemID - ID_DIVISION_NO1;
	if(0 > nIndex) return;

 	m_strToolTipText_For_Mouse_In_the_PopupMenu = pPopup_static->m_aToolTipText.GetAt(nIndex);

	// TODO: Add your message handler code here
	
}

void CDlgSEQ::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnMouseMove(nFlags, point);
}

// �Լ��̸�: CDlgSEQ::OnEnterIdle
// �Լ�����: ��� ���̾�α׳� �޴��� �˾��� �����Ͻ� ������ �޽���
// ��ȯ  ��: void 
// ����[UINT nWhy] : MSGF_DIALOGBOX ���, MSGF_MENU �޴�
// ����[CWnd* pWho] : �ش� ������
void CDlgSEQ::OnEnterIdle(UINT nWhy, CWnd* pWho) 
{
	CDialog::OnEnterIdle(nWhy, pWho);
		
	// TODO: Add your message handler code here
	if(MSGF_MENU == nWhy)
	{		
		CRect r;
		pWho->GetWindowRect(r);
	
		CPoint point;
		GetCursorPos(&point);	

		if(TRUE == r.PtInRect(point))
		{		
 			if(_T("") != m_strToolTipText_For_Mouse_In_the_PopupMenu)
 			{				
 				m_ToolTip.Display(
 					point, 
 					CPoint(r.right+4, point.y), 
 					m_strToolTipText_For_Mouse_In_the_PopupMenu
 					); 				
//DEL 				pWho->SetWindowPos(
//DEL  					&m_ToolTip,
//DEL  					0,
//DEL  					0,
//DEL  					0,
//DEL  					0,
//DEL  					SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW
//DEL  					);
  			}
		}		
		else m_ToolTip.Hide();
	}
	
}

LRESULT CDlgSEQ::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_EXITMENULOOP == message)
	{
		CString strMsg = _T("AUTO_HIDE:TRUE");
		GetParent()->SendMessage(WM_REQUEST_KALENDARDLG, IDD, (LPARAM)&strMsg);		
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgSEQ::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
 	INT nArrayIndex = 0, nIndex = 0;
 	if(0 <= Find_StaticIndex_InMouse(&nArrayIndex, &nIndex))
 	{
 		if(POPUP_STATIC == nIndex)
 		{
 			DATA_STATIC_EX *pDATA_STATIC = (DATA_STATIC_EX*)m_aStaticArray[nArrayIndex].GetAt(POPUP_STATIC);
 			OnButtonPopupStatic(pDATA_STATIC, nArrayIndex, nIndex);			
 		}
 		else
 		{		
 			m_ToolTip.Hide();
 			m_nArrayIndex_Of_SelectPopupStatic = -1;
 
 
 			// ���õ� ������ ����
 			CData* pData = new CData;
 			Get_SelectData(pData);
 			GetParent()->SendMessage(WM_NOTIFY, IDD, (LPARAM)pData);
 			delete pData;
 		}
 	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CDlgSEQ::OnEraseBkgnd(CDC* pDC) 
{
// 	CDialog::OnEraseBkgnd(pDC);

	
// 	CRect rRect;
// 	GetClientRect(&rRect);
// 
// 	CBitmap MemBitmap, Bitmap[2];
// 	CDC DC[2];
// 	
// 	CDC MemDC;
// 	MemDC.CreateCompatibleDC(pDC);
// 	MemBitmap.CreateCompatibleBitmap(pDC, rRect.Width(), rRect.Height());
// 	MemDC.SelectObject(&MemBitmap);
// 
// 	UINT uNum[2] = {0,0};
// 	UINT uTmp = m_uDay;
// 	UINT BitmapDay = IDB_BITMAP_DAY_BLACK_0;
// 	if(SATURDAY == m_uDayOfTheWeek)
// 		BitmapDay = IDB_BITMAP_DAY_BLUE_0;
// 	if(SUNDAY == m_uDayOfTheWeek)
// 		BitmapDay = IDB_BITMAP_DAY_RED_0;
// 
// 	for(UINT i=0;i<2;i++)
// 	{
// 		uNum[1-i] = uTmp % 10;
// 		uTmp = uTmp / 10;
// 	}		
// 
// 	if(0==uNum[0] && 0==uNum[1])
// 	{
// 		for(i=0;i<2;i++)
// 		{
// 			DC[i].CreateCompatibleDC(pDC);	
// 			Bitmap[i].LoadBitmap(IDB_BITMAP_DAY_NULL);		
// 			DC[i].SelectObject(&Bitmap[i]);
// 		}
// 	}
// 	else
// 	{
// 		for(i=0;i<2;i++)
// 		{
// 			DC[i].CreateCompatibleDC(pDC);				
// 			Bitmap[i].LoadBitmap(BitmapDay + uNum[i]);		
// 			DC[i].SelectObject(&Bitmap[i]);
// 		}
// 	}
// 
// 	MemDC.FillSolidRect(0, 0, rRect.Width(), rRect.Height(), RGB(255,255,255));
// 	for(i=0;i<2;i++)
// 	{
// 		MemDC.BitBlt(i*10,0,10, 17, &DC[i],0,0,SRCCOPY);
// 	}	
//  	pDC->BitBlt(0, 0, 20, 15, &MemDC, rRect.Width(), rRect.Height(), SRCCOPY);

	UINT uNum[2] = {0,0};
	UINT uTmp = m_uDay;
	INT nY = 0;
	switch(m_uDayOfTheWeek)
	{
	case SATURDAY:
		nY = 15;
		break;
	case SUNDAY:
		nY = 30;
		break;
	default:
		nY = 0;
		break;
	}

	for(UINT i=0;i<2;i++)
	{
		uNum[1-i] = uTmp % 10;
		uTmp = uTmp / 10;
	}		

	m_MemDC.FillSolidRect(0, 0, DAYWIDTH, DAYHEIGHT, RGB(255,255,255));

	if(0 != uNum[0] || 0 != uNum[1])
	{
		m_MemDC.BitBlt(0, 0, 10, 15, &m_DC[0], uNum[0] * 10, nY, SRCCOPY);	
		m_MemDC.BitBlt(10, 0, 10, 15, &m_DC[0], uNum[1] * 10, nY, SRCCOPY);	
	}
	
	if(m_bIsHoliday && m_bIsToday)
	{
		m_MemDC.BitBlt(20, 0, 83, 15, &m_DC[3], 0, 0, SRCCOPY);	
	}
	else if(m_bIsHoliday)
	{
		m_MemDC.BitBlt(20, 0, 49, 15, &m_DC[1], 0, 0, SRCCOPY);	
	}
	else if(m_bIsToday)
	{
		m_MemDC.BitBlt(20, 0, 36, 14, &m_DC[2], 0, 0, SRCCOPY);	
	}

	pDC->BitBlt(0, 0, DAYWIDTH, DAYHEIGHT, &m_MemDC, 0, 0, SRCCOPY);

 	return TRUE;
}

INT CDlgSEQ::InitBG()
{
	CDC* pDC = GetDC();

	m_MemDC.CreateCompatibleDC(pDC);
	m_MemBitmap.CreateCompatibleBitmap(pDC, DAYWIDTH, DAYHEIGHT);
	m_MemDC.SelectObject(&m_MemBitmap);

	m_DC[0].CreateCompatibleDC(pDC);
	m_Bitmap[0].LoadBitmap(IDB_DAY);
	m_DC[0].SelectObject(&m_Bitmap[0]);

	m_DC[1].CreateCompatibleDC(pDC);
	m_Bitmap[1].LoadBitmap(IDB_BITMAP_HOLIDAY);
	m_DC[1].SelectObject(&m_Bitmap[1]);

	m_DC[2].CreateCompatibleDC(pDC);
	m_Bitmap[2].LoadBitmap(IDB_BITMAP_TODAY);
	m_DC[2].SelectObject(&m_Bitmap[2]);

	m_DC[3].CreateCompatibleDC(pDC);
	m_Bitmap[3].LoadBitmap(IDB_BITMAP_TODAY_HOLIDAY);
	m_DC[3].SelectObject(&m_Bitmap[3]);

	ReleaseDC(pDC);

	return 0;
}