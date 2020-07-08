// Slider.cpp : implementation file
//

#include "stdafx.h"
#include "Slider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSlider

CSlider::CSlider()
{
	//�����̴� �� ���� �ö� ��ư�� ��ġ ����
	m_nPosPercent = 0;
	nButtonPosX = 1;
	nButtonPosY = 1;
	// KOL.UDF.022 ��ť���ڵ� ����
	m_pParent = NULL;
	m_pButton = NULL;
	m_nLeftBound = 0;
	m_nRightBound = 0;
	m_rectWndPos = NULL;
	bgRect = NULL;
	mainDlgRect = NULL;
	INT buttonWidth;
	
	CString m_strAlias;

}

CSlider::~CSlider()
{
	Save();
	if( m_pButton != NULL )
	{
		delete m_pButton;
		m_pButton = NULL;
	}
}

BEGIN_MESSAGE_MAP(CSlider, CButton)
	//{{AFX_MSG_MAP(CSlider)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(UM_SLIDERBUTTONMOVED,OnButtonMoved)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlider message handlers

void CSlider::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	
}

BOOL CSlider::Create(CWnd *pParent,INT nWndPosX,INT nWndPosY,CString alias) 
{
	// �θ� ������ ������ ��´�
	m_pParent		= pParent;
	
	// ����� �ε��Ѵ�.
	m_picBg.Load(_T("..\\BMP\\slider_bg.bmp"));
	m_rectWndPos.left	= nWndPosX;
	m_rectWndPos.top	= nWndPosY;
	m_rectWndPos.right	= nWndPosX + m_picBg.m_Width;
	m_rectWndPos.bottom	= nWndPosY + m_picBg.m_Height;
	
	
	// �ۼ�Ʈ ��� �ϱ� ���� �ٿ������ ���
	// picTemp �� ��ư�� ���� �󸶳� �Ǵ��� �˱����� �ӽ÷� ��� (���� ����?)
	
	CPicture	picTemp;
	picTemp.Load(_T("..\\BMP\\slider_buttonon.bmp"));
	buttonWidth = picTemp.m_Width;
	
	//���ʰ� ������ ��� ����
	m_nLeftBound	= nButtonPosX;
	m_nRightBound	= m_nLeftBound+(m_picBg.m_Width-(m_nLeftBound*2));
	
	// �켱 ����� �Ǵ� �κ��� ������ ����
	CButton::Create(_T("slider"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,m_rectWndPos,m_pParent,20001);
	
	// �����̴� ��ư ����
	m_pButton = new CSliderButton;
	m_pButton->Create(this,nButtonPosX,nButtonPosY,m_nRightBound);
	
	////I/O//////////////////////////////////////////////////////////////////////////////
	
	if ( alias == _T("") )
	{
		m_strAlias = _T("common");
	}
	else
	{
		m_strAlias = alias;
	}
	
	
	CString filename;		
	filename.Format(_T("..\\cfg\\Transparency\\%s.cfg"), m_strAlias);				
	
	FILE *fr;
	fr = _tfopen(filename.GetBuffer(0), _T("rb"));
	if(fr != NULL)
	{
		// KOL.UDF.022 ��ť���ڵ� ����
		//if (!fr) //�ش� ������ ���ٸ�
		if (&fr == NULL) //�ش� ������ ���ٸ�
		{
			FILE *fw;
			fw = _tfopen(filename.GetBuffer(0), _T("wb"));
			if (fw != NULL) 
			{
				fclose(fw);
			}				
		}
		else //�ش������� �ִٸ�
		{
			CString strContents,temp;
			int nPos;
			TCHAR szBuf[128];
			_fgetts(szBuf, 127, fr);
			
			strContents.Format(_T("%s"), szBuf);
			nPos=strContents.Find(':',0);
			temp=strContents.Mid(nPos+1,3);
			CString getStr = temp.GetAt(2);
			
			if(getStr == ']')// 100�� �ƴѰ�
			{
				// 100�� �ƴѰ�
				temp = strContents.Mid(nPos+1,2);
				SetPos(_ttoi(temp));
				
			}
			else if(getStr == '0')// 100�̸�
			{
				// 100
				temp = strContents.Mid(nPos+1,3);
				SetPos(_ttoi(temp));
			}
			
		}
		fclose(fr);
	}
	else
	{
		SetMaxPos();
	}
	
	//��ư �������� �ּҰ��� �� OnbuttonMoved �Լ��� �Ҹ��� �ʾ� 
	//�޽����� �θ� ���̾�α׷� ���� �� �����Ƿ� �߰�
	m_pParent->SendMessage(UM_SLIDERMOVED,(WPARAM)0,(LPARAM)m_nPosPercent+20);
	
	return TRUE;
}

BOOL CSlider::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	// ��� �׷��ְ�.
	m_picBg.Show(pDC,CRect(0,0,m_picBg.m_Width,m_picBg.m_Height));

	return TRUE;
}

LONG CSlider::OnButtonMoved(WPARAM wParam, LPARAM lParam)
{
	INT nButtonPos = (INT)wParam;
	
	// ��ư�� ������ �Ÿ� / �����̴� ���� ���� * 80
	// ���̾�αװ� ������ ������°��� ���� ���� 80�����κ�ȯ
	float a,b,c;
	a = (float)(nButtonPos-m_nLeftBound);
	b = (float)(m_nRightBound-m_nLeftBound-buttonWidth);
	c = ( a / b * 80);
	m_nPosPercent = (INT)c;
	//	TRACE(_T("%d\n"),m_nPosPercent);
	
	if(m_nPosPercent<0)
	{
		m_nPosPercent = 0;
	}
	else if(m_nPosPercent>80)
	{
		m_nPosPercent = 80;
	}
	
	Invalidate();
	UpdateWindow();
		
	m_pParent->SendMessage(UM_SLIDERMOVED,(WPARAM)0,(LPARAM)m_nPosPercent+20);
	
	
	return 0;
}

void CSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//m_pButton
	
	CPoint n;
	GetCursorPos(&n);
	ScreenToClient(&n);
	
	// �����̴� �� ��谪 ���̿� Ŭ���� �ش� ���콺 �����͸� �߽����� ��ư �̵�
	if(n.x>=m_nLeftBound+(buttonWidth/2) && n.x<=m_nRightBound-(buttonWidth/2))
	{
		m_pButton->m_rectButtonPos.left = n.x-(buttonWidth/2);
		m_pButton->m_rectButtonPos.right = n.x + (buttonWidth/2);
		m_pButton->MoveWindow(&(m_pButton->m_rectButtonPos));
		
		Invalidate();
		UpdateWindow();
	}
	
	// �׷��� ������( ��谪�� ������ ) ���� �� ��ġ�� ��ư �̵�
	else
	{
		if(n.x<m_nLeftBound+(buttonWidth/2))
		{
			m_pButton->m_rectButtonPos.left = m_nLeftBound;
			m_pButton->m_rectButtonPos.right=m_nLeftBound+buttonWidth;
			m_pButton->MoveWindow(&(m_pButton->m_rectButtonPos));
		}
		else if(n.x>m_nRightBound-(buttonWidth/2))
		{
			m_pButton->m_rectButtonPos.left=m_nRightBound-buttonWidth;
			m_pButton->m_rectButtonPos.right=m_nRightBound;
			m_pButton->MoveWindow(&(m_pButton->m_rectButtonPos));
		}
	}
	
	CButton::OnLButtonDown(nFlags, point);
}

void CSlider::SetMaxPos()
{
	m_pButton->m_rectButtonPos.left=m_nRightBound-buttonWidth;
	m_pButton->m_rectButtonPos.right=m_nRightBound;
	m_pButton->MoveWindow(&(m_pButton->m_rectButtonPos));	
}

void CSlider::SetPos(int nPercent)
{
	//percentage���� ��ư ������ġ�� ������ ��ȯ
	int ButtonLeft = 0;
	int temp = m_nLeftBound;
	
	ButtonLeft = (((float)(nPercent)*float(m_nRightBound-m_nLeftBound-buttonWidth))/80)-17;
	
	m_pButton->m_rectButtonPos.left = ButtonLeft;
	m_pButton->m_rectButtonPos.right= m_pButton->m_rectButtonPos.left+buttonWidth;
	m_pButton->MoveWindow(&(m_pButton->m_rectButtonPos));
}

int CSlider::GetPosition()
{
	return this->m_nPosPercent;
}

void CSlider::SetPosition(int percent)
{
	//0~100 % ������ 0~80% ������ ��ȯ
	int calcPercent = (80*percent / 100)+20;

	int ButtonLeft = 0;
	ButtonLeft = (((float)(calcPercent)*float(m_nRightBound-m_nLeftBound-buttonWidth))/80)-17;
	
	m_pButton->m_rectButtonPos.left = ButtonLeft;
	m_pButton->m_rectButtonPos.right= m_pButton->m_rectButtonPos.left+buttonWidth;
	m_pButton->MoveWindow(&(m_pButton->m_rectButtonPos));
}


///////////////////////////////////////////////////////////////////////////////////////////
// CSliderButton
///////////////////////////////////////////////////////////////////////////////////////////
CSliderButton::CSliderButton()
{
	// KOL.UDF.022 ��ť���ڵ� ����
	m_pParent = NULL;
	m_ptOld = NULL;
	m_ptNew = NULL;
	m_bLButtonDown = FALSE;
	m_bCursorOnWindow = FALSE;
	m_nLeftBound = 0;
	m_nRightBound = 0;
	m_rectButtonPos = NULL;
	m_rectButtonImage = NULL;
	minX = 0;
	maxX = 0;
	buttonWidth = 0;
	
}

CSliderButton::~CSliderButton()
{
}


BEGIN_MESSAGE_MAP(CSliderButton, CButton)
//{{AFX_MSG_MAP(CSliderButton)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_PAINT()
ON_WM_WINDOWPOSCHANGED()
ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSliderButton message handlers

// ��ư�� OWNERDRAW �Ӽ��� ���ؼ� �־�� �ϴ� �Լ�
void CSliderButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	
}

BOOL CSliderButton::Create(CWnd *pParent,INT x,INT y,INT nMoveWidth) 
{
	m_pParent			= pParent;
	m_bCursorOnWindow	= FALSE;
	m_nLeftBound		= x;
	m_nRightBound		= nMoveWidth;
	
	// �����̴� ��ư�� �ö� �̹��� �ε�
	m_picButton1.Load("..\\BMP\\slider_buttonon.bmp");
	m_picButton2.Load("..\\BMP\\slider_button.bmp");
	m_picButtonBuffer1.Load("..\\BMP\\slider_buttonon.bmp");
	m_picButtonBuffer2.Load("..\\BMP\\slider_button.bmp");
	
	buttonWidth = m_picButton1.m_Width;
	
	// �����̴� ��ư�� ��ġ
	m_rectButtonPos.left	= x;
	m_rectButtonPos.top		= y;
	m_rectButtonPos.right	= x + m_picButton1.m_Width;
	m_rectButtonPos.bottom	= y + m_picButton1.m_Height;
	
	// �����̴� ��ư ���� �ö󰡴� �̹��� ũ��
	m_rectButtonImage.left		= 0;
	m_rectButtonImage.top		= 0;
	m_rectButtonImage.right		= m_picButton1.m_Width;
	m_rectButtonImage.bottom	= m_picButton1.m_Height;
	
	CButton::Create(_T("sliderbutton"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,m_rectButtonPos,m_pParent,20000);
	
	return TRUE;
}


void CSliderButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRect bgWnd;
	GetParent()->GetWindowRect(bgWnd);
	
	// ���콺�� �̵��� ��谪 ����
	minX = bgWnd.left + m_nLeftBound + (buttonWidth/2);
	maxX = bgWnd.left + m_nRightBound - (buttonWidth/2)-1;
	
	
	SetCapture();
	m_bLButtonDown = TRUE;
	
	if( m_bCursorOnWindow == FALSE )
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bCursorOnWindow = _TrackMouseEvent(&tme);
	}
	
	// ~��~ ���� ��ġ�� �ٲ� �� Ŀ���� ���� ��ġ�� �ٽ� ����(m_ptNew�� m_ptOld�� �����ϰ���)
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	
	m_ptNew.x = pt.x;
	m_ptNew.y = pt.y;
	
	m_ptOld = m_ptNew;	
	
	CButton::OnLButtonDown(nFlags, point);
}

void CSliderButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_bLButtonDown = FALSE;
	m_bCursorOnWindow=FALSE;
	ReleaseCapture();
	
	Invalidate();
	UpdateWindow();
	
	CButton::OnLButtonUp(nFlags, point);
}

void CSliderButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPoint mousePoint;
	GetCursorPos(&mousePoint);
	
	//TRACE(_T("%d\n"),mousePoint.x);
	
	CPoint ptDiffer;
	m_ptNew = mousePoint;
	ptDiffer = m_ptNew - m_ptOld;
	//	m_ptOld = m_ptNew;
	
	// ���� ��ư�� ���� ���� ��
	if( m_bLButtonDown == TRUE )//&& m_bCursorOnWindow == TRUE )
	{
		// ���콺 �̵� ��谪 ���̶�� �̵��Ÿ��� ����Ͽ� ��ư �̵�
		if(mousePoint.x>=minX && mousePoint.x<=maxX )
		{
			
			ScreenToClient(&ptDiffer);
			
			m_rectButtonPos.left		= m_rectButtonPos.left + ptDiffer.x ;
			m_rectButtonPos.right		= m_rectButtonPos.right + ptDiffer.x ;
			MoveWindow(&m_rectButtonPos);
		}
		
		// ���콺 �̵��� ��谪�� ����ٸ� ��ư�� �� ��ġ�� �̵�(����� ó�� ����)
		else
		{
			if(mousePoint.x<minX)
			{
				m_rectButtonPos.left=m_nLeftBound;
				m_rectButtonPos.right=m_nLeftBound+buttonWidth;
				MoveWindow(&m_rectButtonPos);
			}
			else if(mousePoint.x>=maxX)
			{
				m_rectButtonPos.left=m_nRightBound-buttonWidth;
				m_rectButtonPos.right=m_nRightBound;
				MoveWindow(&m_rectButtonPos);
			}
		}
		
	}	
	
	CButton::OnMouseMove(nFlags, point);
}

void CSliderButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// �����̴� ��ư �׸���.
	if( m_bCursorOnWindow == TRUE )
	{
		m_picButton2.Show(&dc,m_rectButtonImage);
	}	
	else if( m_bCursorOnWindow == FALSE )
	{
		m_picButton1.Show(&dc,m_rectButtonImage);
	}
}

LONG CSliderButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{	
	// ���콺�� ��ư ���� ������ ó���� OnMouseMove��
	// OnMouseMove��TrackMouseEvent�� �����WM_MOUSEHOVER
	Invalidate();
	UpdateWindow();
	
	return 0;
}

LONG CSliderButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	// Ŀ���� �����츦 ��� �� : FALSE
	m_bCursorOnWindow = FALSE;	
	// ��ư �׸� Change
	Invalidate();
	UpdateWindow();
	
	return 0;
}

void CSliderButton::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CButton::OnWindowPosChanged(lpwndpos);
	
	// Button moving -> SendMessage
	m_pParent->SendMessage(UM_SLIDERBUTTONMOVED,(WPARAM)m_rectButtonPos.left,0);	
}
///////////////////////////////////////////////////////////////////////////////////////////

void CSlider::Save()
{
//���Ͽ� �ۼ�Ƽ���� ����
	CString filename;
	filename.Format(_T("..\\cfg\\Transparency\\%s.cfg"), m_strAlias);
	FILE *fd;
	fd = _tfopen(filename.GetBuffer(0), _T("wb"));
	if(fd != NULL)
	{
		CString str;
		str.Format(_T("[POSITION:%d]"),m_nPosPercent+20);
		_ftprintf(fd, _T("%s\0"), str);
		
		fclose(fd);
	}
}
