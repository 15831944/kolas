// MenuBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MenuBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuBarDlg dialog

CMenuBarDlg::CMenuBarDlg(INT nAliasCount, CWnd* pParent /*=NULL*/)
	: CDialog(CMenuBarDlg::IDD, pParent)
{
	// �޴��� ������ ��� 
	ArrayMenu = (MIMenu*)malloc(sizeof(MIMenu) * 1);

	m_nBmpInfoCount = 0;
	m_IsBkPaint = FALSE;	

	// �� �޴����� ���� 
	m_AliasCount = nAliasCount;

	// ��ư ����Ʈ 
	m_pListIDButton= NULL;

	// ���ٷ� ����� �͵��� ID ����
	m_nButtonCount = 0;


	// LeftMenu�� RightMenu�� ��� Index
	m_nBorderLine = 0;

	m_CurrentID = m_BeforeID = 0;

	m_Statichwnd = NULL;
	m_pStaticWnd = NULL;

	m_pParent = pParent;

	m_nAddMenuIndex = 0 ;
	m_XPosion = 0;

	m_AddFlag = FALSE ;

	// Font
	m_Font.CreateFont( 12, 0,0,0, FW_THIN ,0,0,0, DEFAULT_CHARSET,
			    OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE,_T("����ü"));
	// - default
	m_Font.GetLogFont(&font);
	font.lfUnderline = TRUE;
    m_UnderLineFont.CreateFontIndirect(&font);
	// - Bold, UnderLine
	font.lfWeight = FW_BOLD;
	m_CurrentMenuFont.CreateFontIndirect(&font);
	// - Bold
	font.lfUnderline = FALSE;
	m_BoldFont.CreateFontIndirect(&font);

	draw_rect1.UpperLeft  = 0;
	draw_rect1.LowerRight = 1;	
}

CMenuBarDlg::~CMenuBarDlg()
{
	if( m_pListIDButton != NULL )
	{
		free(m_pListIDButton);
		m_pListIDButton = NULL;
	}
	if( ArrayMenu != NULL )
	{
		free(ArrayMenu);
		ArrayMenu = NULL;
	}

	for ( INT i = 0 ; i < 3 ; i ++ )
	{
		m_BkDC[i].DeleteDC();
		m_BkBmp[i].DeleteObject();
	}

	m_Font.DeleteObject();
	m_UnderLineFont.DeleteObject();
	m_BoldFont.DeleteObject();
}

void CMenuBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMenuBarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMenuBarDlg, CDialog)
	//{{AFX_MSG_MAP(CMenuBarDlg)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuBarDlg message handlers

BOOL CMenuBarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// �ڷḦ �־� �д�. ���� ���� �޾Ƽ�..
	//	PutDataInStruct();
	//	Init();
	//	MakeLineMenu(1110);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ���� �޴��� ����� ��
// ���̵�, �����ʿ��ִ� �ú� �޴����� �ƴ���, �½������ �� ������ 
INT CMenuBarDlg::MakeLineMenu(INT n_ID, BOOL nSiblingFlag, BOOL bSubFlag)
{	
	// �½������ �°��� �ƴҶ� �׳ɳѾ�� �ִ�.
	// �½������ �°��̸� ���� ID�� �ٸ��� �׷����� �����Ƿ�
	if( !bSubFlag )
	{
		if( m_CurrentID == n_ID )
			return 0;
	}

	// BMP �׸����� ���� ���� 
	m_nBmpInfoCount = 0;

	m_AddFlag = FALSE;

	m_CurrentID = n_ID;
	TCHAR szTemp[128] = {NULL};

	INT *pnIDlist;

	pnIDlist = NULL;

	INT i, j, nSun, nCount, nTemp = 0, ids = 0;

	for( i = 0; i < 50; i++ )
	{
		m_Static[i].DestroyWindow();
		m_Static[i].m_hWnd = NULL;
	}
	m_pAddMenuStatic[0].DestroyWindow();
	m_pAddMenuStatic[0].m_hWnd = NULL;

	nCount = 0;
	
	for( i = 0; i < m_AliasCount; i++ )
	{		
		nSun = i;
		if ( ArrayMenu[nSun].nID == n_ID)
		{
			nCount = 1;
			pnIDlist = (INT*)malloc( sizeof(INT) * 1 );
			pnIDlist[0] = n_ID;
			
			for( j = 0; j < m_AliasCount; j++ )					
			{
				if(		( ArrayMenu[j].nLevel == ArrayMenu[nSun].nLevel - 1 ) 
					&&	( ArrayMenu[j].nIndex == ArrayMenu[nSun].nParentIndex ) )
				{					
					nSun = j;
					nCount ++;	
					pnIDlist = (INT*)realloc(pnIDlist, sizeof(INT) * nCount);
					pnIDlist[nCount-1] = ArrayMenu[j].nID;
					j=-1;
				}	
			}
		}		
	}
 	//++2008.08.27 ADD BY CJY {{++
	// nID���� ã�� ���� ���� nMenuID�� �ٽ� �� �� �˻��Ѵ�.
	// �̷��� �ϴ� ������ MENU_ID���� 5�ڸ��� ���
	// INT CMenuBarDlg::InputDate���� nID���� ����Ǳ� �����̴�.
	// �̶� ID���� �׻� 5000�̻��̴�.
 	if(1 >= nCount && 5000 <= n_ID)
 	{
 		CString strID = _T("");
 		strID.Format(_T("%d"), n_ID);
 	for( i = 0; i < m_AliasCount; i++ )
 	{		
 		nSun = i;
 		if ( ArrayMenu[nSun].nMenuID == n_ID)
 		{
 			nCount = 1;
 			pnIDlist = (INT*)malloc( sizeof(INT) * 1 );
 			pnIDlist[0] =  n_ID;
 			
 			for( j = 0; j < m_AliasCount; j++ )					
 			{
 				if(		( ArrayMenu[j].nLevel == ArrayMenu[nSun].nLevel - 1 ) 
 					&&	( ArrayMenu[j].nIndex == ArrayMenu[nSun].nParentIndex ) )
 				{					
 					nSun = j;
 					nCount ++;	
 					pnIDlist = (INT*)realloc(pnIDlist, sizeof(INT) * nCount);
 					pnIDlist[nCount-1] = ArrayMenu[j].nID;
 					j=-1;
 				}	
 			}
 		}		
		if ( ArrayMenu[i].nMenuID == n_ID)
		{
			// �޴������ÿ��� nMenuID�� �ƴ� nID�� �������־�� �Ѵ�.
			pnIDlist[0] = ArrayMenu[i].nID;
		}
 	}	
 	}
 	//--2008.08.27 ADD BY CJY --}}

	ids = MakeLeftLineMenu(nCount, pnIDlist, nSiblingFlag);
	if( ids ) return -5;

	if( pnIDlist != NULL )	pnIDlist = NULL;
	if( !nSiblingFlag )	n_ID = m_BeforeID;
	
	// ���� ���� ������ �͵� ��Ÿ����
	nCount = 0;
	
	for( i = 0; i < m_AliasCount; i++ )
	{		
		if( ArrayMenu[i].nID == n_ID )
		{
			if( pnIDlist != NULL ) pnIDlist = NULL;
			pnIDlist = (INT*)malloc( sizeof(INT) * 1 );
			
			for( j = 0; j < m_AliasCount; j++ )
			{
				if(		( ArrayMenu[i].nLevel == ArrayMenu[j].nLevel ) 
					&&	( ArrayMenu[i].nParentIndex == ArrayMenu[j].nParentIndex) )
				{
					nCount++;
					pnIDlist = (INT*)realloc(pnIDlist, sizeof(INT) * nCount );
					pnIDlist[nCount-1] = ArrayMenu[j].nID;
				}				
			}
			break;
		}
	}
	//++2008.08.27 ADD BY CJY {{++
	// nID���� ã�� ���� ���� nMenuID�� �ٽ� �� �� �˻��Ѵ�.
	// �̷��� �ϴ� ������ MENU_ID���� 5�ڸ��� ���
	// INT CMenuBarDlg::InputDate���� nID���� ����Ǳ� �����̴�.
	// �̶� ID���� �׻� 5000�̻��̴�.
	if(0 == nCount && 5000 <= n_ID)
	{
		for( i = 0; i < m_AliasCount; i++ )
		{		
			if( ArrayMenu[i].nMenuID == n_ID )
			{
				if( pnIDlist != NULL ) pnIDlist = NULL;
				pnIDlist = (INT*)malloc( sizeof(INT) * 1 );
				
				for( j = 0; j < m_AliasCount; j++ )
				{
					if(		( ArrayMenu[i].nLevel == ArrayMenu[j].nLevel ) 
						&&	( ArrayMenu[i].nParentIndex == ArrayMenu[j].nParentIndex) )
					{
						nCount++;
						pnIDlist = (INT*)realloc(pnIDlist, sizeof(INT) * nCount );
						pnIDlist[nCount-1] = ArrayMenu[j].nID;
					}				
				}
				break;
			}
		}
	}
	//--2008.08.27 ADD BY CJY --}}
	ids = MakeRightLineMenu(nCount, pnIDlist);
	if( ids ) return -10;

	if( pnIDlist != NULL )
	{
		free(pnIDlist);
		pnIDlist = NULL;
	}

	m_BeforeID = n_ID;

	return 0;
}

INT CMenuBarDlg::MakeLeftLineMenu(INT ButtonCount, INT *pIDButtonList, BOOL nSiblingFlag)
{
	INT ids, i, n_IdButton;
	CString strName, strID ;
	CRect MainRect, TempRect;

	GetParent()->GetClientRect(&TempRect);
	MainRect = TempRect;

	// ���ݼ��� 
	// ������������ ���� 
	MainRect.top = TempRect.top + 5 ;// + 11;
	// ���ʿ��������� ������
	MainRect.left = TempRect.left + 1 ;
	// ��ü���� bottom - top 
	MainRect.bottom = 25 ;//TempRect.bottom - 5 ;//MainRect.top + 11 ;

	if ( NULL != m_pListIDButton )
	{
		m_pListIDButton = NULL;
	}

	m_pListIDButton = (INT*)malloc(sizeof(INT)*ButtonCount);
	m_nBorderLine = m_nButtonCount = ButtonCount;

	for( i = 0 ; i < ButtonCount * 2 - 1 ; i++ )
	{			
		if( 0 ==  i % 2 )
		{
			strID.Format(_T("2%04d"),pIDButtonList[ButtonCount - i/2 -1]);
			
			n_IdButton = _ttoi(strID.GetBuffer(0));

			m_pListIDButton[i/2] = n_IdButton;

			INT nMaxLength = WideCharToMultiByte(CP_ACP, 0, GetNameBYID(pIDButtonList[ButtonCount - i/2-1]).GetBuffer(0), -1, NULL, 0, NULL,NULL);	
			// ���ݼ���
			// _GAPLEGTH �� ������ ���� �Ͱ��� �Ÿ� 
			MainRect.right = MainRect.left + 7 * (nMaxLength-1) + _LEFTGAPLEGTH ;
			
			if( NULL != m_Static[i].m_hWnd )
			{
				m_Static[i].DestroyWindow();
			}

			ids = m_Static[i].Create(GetNameBYID(pIDButtonList[ButtonCount - i/2-1]).GetBuffer(0), NORMAL_STYLE , MainRect , this, n_IdButton);
			if( 0 == ids ) return -10;
			m_Static[i].SetWindowText(GetNameBYID(pIDButtonList[ButtonCount - i/2-1]).GetBuffer(0));
		}
		else
		{
			// ���ݼ���
			// left�� ������ ���� �Ͱ��� �Ÿ� 
			//MainRect.left += 3 ;
			// right�� ���� ���� ũ�� ( �׸����� ��ü ���� �׸��� ũ��� ������� )
			MainRect.right = MainRect.left + 10;//+12;

			if( NULL != m_Static[i].m_hWnd )
			{
				m_Static[i].DestroyWindow();
			}

			m_nBmpInfoCount++;

			BmpInfoList[m_nBmpInfoCount-1].rcPosition = MainRect;
			BmpInfoList[m_nBmpInfoCount-1].rcPosition.left += 2;
			BmpInfoList[m_nBmpInfoCount-1].nKind = 0;
			BmpInfoList[m_nBmpInfoCount-1].nID = 0;

			ids = m_Static[i].Create(_T(""), NORMAL_STYLE , MainRect , this, IDB_bTEST1 /*+ ((i+1)/2)-1*/);
			if( 0 == ids ) return -10;
		}
		m_Static[i].GetClientRect(&TempRect);
		// ���θ������ �����ʳ��� ������ �� ���� �ǿ������� ���� 
		MainRect.left += TempRect.right - TempRect.left ;
	}

	// ����� �ִ� �� �׷� �ش�. ���ʳ��� �Ͱ��� �Ÿ��� + �� ����  
	m_nBmpInfoCount++;
	BmpInfoList[m_nBmpInfoCount-1].rcPosition = MainRect ;
	BmpInfoList[m_nBmpInfoCount-1].rcPosition.left = BmpInfoList[m_nBmpInfoCount-1].rcPosition.right + 7 ;
	BmpInfoList[m_nBmpInfoCount-1].nKind = 1;

	if( !nSiblingFlag )
	{
	}	
	else
	{
		m_LastMenuRect = MainRect;
	}

	return 0;
}

INT CMenuBarDlg::MakeRightLineMenu(INT ButtonCount, INT *pIDButtonList)
{
	INT ids, i, n_IdButton;
	CString strName, strID ;
	CRect MainRect, TempRect;

	MainRect = m_LastMenuRect;

	// ���� ���̸޴��� ������ Sibling ���� �Ÿ� 
	MainRect.left += 25;

	m_nButtonCount += ButtonCount;

	m_pListIDButton = (INT*)realloc( m_pListIDButton, sizeof(INT*)* m_nButtonCount);	

	GetClientRect(&m_ParentRect);

	for( i = 0 ; i < ButtonCount * 2 - 1 ; i++ )
	{		
		if( 0 ==  i % 2 )
		{
			// Real Size ���ϱ�	
			INT nMaxLength = WideCharToMultiByte(CP_ACP, 0, GetNameBYID(pIDButtonList[i/2]).GetBuffer(0), -1, NULL, 0, NULL,NULL);	
			//MainRect.right = MainRect.left + 14 * GetNameBYID(pIDButtonList[i/2]).GetLength();
			MainRect.right = MainRect.left + 6 * ( nMaxLength -1) + _RIGHTGAPLEGTH ;

			strID.Format(_T("3%04d"),pIDButtonList[i/2]);
			
			n_IdButton = _ttoi(strID.GetBuffer(0));
			
			m_pListIDButton[i/2 + (m_nButtonCount- ButtonCount)] = n_IdButton;

			// �� ���ʿ��� ������ �����Ÿ��� ������ ������ ����
			// ������ �����κ��� 24������ ���� ���� �������� ���� ������ �ɽ� ADDMENU����ƽ�� �����Ѵ�.
			if( ( m_AddFlag == FALSE ) && ( MainRect.left > m_ParentRect.left + 300 ) && ( m_ParentRect.right < MainRect.right + 24 /*MainRect.right > m_ParentRect.right - (MainRect.right - MainRect.left) - 60*/ ) )
			{
				TempRect = MainRect;
				// ���ݼ���
				// ADD�޴��� ����ƽ ũ�� ���� 
				TempRect.right = TempRect.left + 12;
				
				if( NULL != m_pAddMenuStatic[0].m_hWnd )
				{
					m_pAddMenuStatic[0].DestroyWindow();
				}
				
				m_pAddMenuStatic[0].Create(_T("��"), NORMAL_STYLE , TempRect , this, IDB_ADDMENU);
				
				m_nAddMenuIndex = i/2 + (m_nButtonCount- ButtonCount);

				// ǥ�ÿ� ��ġ�� ���� ǥ�� ����ƽ ����
				m_Static[i-1+ ((m_nButtonCount - ButtonCount) *2-1) ].DestroyWindow();	

				// ADDMENU�� �����Ǿ��ٴ� ���� ��Ÿ���� �÷��� 
				m_AddFlag = TRUE;
			}
			else
			{
				// ����޴��� ����� �׸��� ǥ������ �ʴ´�. 
				if ( m_AddFlag != TRUE )
				{
					m_nBmpInfoCount += 2;
					
					BmpInfoList[m_nBmpInfoCount-2].rcPosition = BmpInfoList[m_nBmpInfoCount-1].rcPosition = MainRect;
					BmpInfoList[m_nBmpInfoCount-2].rcPosition.left -= 5;
					BmpInfoList[m_nBmpInfoCount-1].rcPosition.left = MainRect.right ;
					
					BmpInfoList[m_nBmpInfoCount-2].nID = BmpInfoList[m_nBmpInfoCount-1].nID = n_IdButton;				
					
					if( m_CurrentID == (n_IdButton %10000) )
					{
						BmpInfoList[m_nBmpInfoCount-1].nKind = 5;
						BmpInfoList[m_nBmpInfoCount-2].nKind = 4;
					}
					else
					{
						BmpInfoList[m_nBmpInfoCount-1].nKind = 3;
						BmpInfoList[m_nBmpInfoCount-2].nKind = 2;
					}
				}
			}
			
			if( m_AddFlag != TRUE )
			{			
				if( NULL != m_Static[i].m_hWnd )
				{
					m_Static[i+((m_nButtonCount-ButtonCount)*2-1)].DestroyWindow();
				}
				ids = m_Static[i+((m_nButtonCount-ButtonCount)*2-1)].Create(GetNameBYID(pIDButtonList[i/2]), NORMAL_STYLE , MainRect , this, n_IdButton);
				if( 0 == ids ) return -10;
				m_Static[i+ ((m_nButtonCount - ButtonCount) *2-1) ].SetWindowText(GetNameBYID(pIDButtonList[i/2]));				
			}
		}
		else
		{
			if( m_AddFlag != TRUE )
			{
				//MainRect.left += 3 ;
				// ���ݼ���
				// �����ʿ� �ִ� �޴����� ƴ���� �Ÿ� 
				MainRect.right = MainRect.left+12+8;
				
				if( NULL != m_Static[i].m_hWnd )
				{
					m_Static[i+ ((m_nButtonCount - ButtonCount) *2-1)].DestroyWindow();
				}

				ids = m_Static[i+ ((m_nButtonCount - ButtonCount) *2-1) ].Create(_T(""), NORMAL_STYLE , MainRect , this, IDB_bTEST2 /*+ ((i+1)/2)-1*/);
				if( 0 == ids ) return -10;
				//m_Static[i+ ((m_nButtonCount - ButtonCount) *2-1) ].SetWindowText(_T("-"));			
			}
		}	
		
		m_Static[ i+ ((m_nButtonCount - ButtonCount) *2-1) ].GetClientRect(&TempRect);
		
		MainRect.left += TempRect.right - TempRect.left ;
	}

	return 0;
}

CString CMenuBarDlg::GetNameBYID(INT n_ID)
{
	INT i;
	for (i = 0 ; i < m_AliasCount ; i ++ )
	{		
		if ( ArrayMenu[i].nID == n_ID)
		{
			return ArrayMenu[i].strAlias;
		}
			
	}
	//++2008.08.27 ADD BY CJY {{++
	// nID���� ã�� ���� ���� nMenuID�� �ٽ� �� �� �˻��Ѵ�.
	// �̷��� �ϴ� ������ MENU_ID���� 5�ڸ��� ���
	// INT CMenuBarDlg::InputDate���� nID���� ����Ǳ� �����̴�.
	// �̶� ID���� �׻� 5000�̻��̴�.
	if(5000 <= n_ID)
	{
		for (i = 0 ; i < m_AliasCount ; i ++ )
		{		
			if ( ArrayMenu[i].nMenuID == n_ID)
			{
				return ArrayMenu[i].strAlias;
			}
			
		}
	}
	//--2008.08.27 ADD BY CJY --}}

	return _T("����");
}

// Ŭ������ �� �޽��� �����°� 
BOOL CMenuBarDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	WORD wControlID = LOWORD(wParam);

	if ( HIWORD(wParam) == STN_CLICKED  )
	{
		INT ids;
		if( IDB_ADDMENU == wControlID )
		{
			ids = MakeAddMenu();
			if( ids ) return FALSE;
			return TRUE;
		}


		if ( (wControlID > 20000) && (wControlID < 44000) ) 
		{			
			for( INT i = 0 ; i < m_nButtonCount ; i ++ )
			{
				if ( m_pListIDButton[i] == wControlID  )
				{	
					HWND		hWnd;
					hWnd = GetDlgItem(wControlID)->GetSafeHwnd();

					m_Statichwnd = hWnd;

					ids = MakeMenuByID( wControlID , FALSE);
					if(ids) return FALSE;

					return TRUE;
				}
			}						
		}
		if ( (wControlID > 1000) && (wControlID < 6000) ) 
		{
			ids = SendIDToParent(wControlID);
			if( ids ) return FALSE;

			ids = MakeMenuByID( wControlID , TRUE);	
			if( ids ) return FALSE;
			
			return TRUE;
		}
    }
	return CDialog::OnCommand(wParam, lParam);
}

// ���̵� �޾Ƽ� �װſ� ���� ó�����ִ°� ( Ŭ�� ���� �� ) Ŭ���� ��ĸ� 
INT CMenuBarDlg::MakeMenuByID(INT n_ID, BOOL fListflag)
{
	INT ids, nSunIndex;;
	INT nILevel, nJLevel, nKLevel;

	// ID �� IJK���� �����´�. 
	// ID�������� 5�� �����Ѵ�. 
	ids = TakeIJK(n_ID % 10000, &nILevel, &nJLevel, &nKLevel );
	if( ids ) return -15;

	// �����ʿ� �ִ� sibling�̸�
	if( n_ID / 10000 == 3 )
	{
		// �ڽ����ִ� ��� 
		if( GetCheckSun( nILevel, nKLevel, &nSunIndex ) )
		{
			nILevel += 1;
			nJLevel = nKLevel ;
			nKLevel = nSunIndex ;			

			// �ڽĺ��� �����ϴ� �޴��� �����. 
			// ������� ù��° ���� �Ⱦ� 
			ids = AutoMakeMenu( n_ID , nILevel, nJLevel, nKLevel, !fListflag);
			if( ids ) return -20;
		}
		else
		{
			// �׿��� ��� 
			ids = SendIDToParent(n_ID);
			if( ids ) return -5;
			
			ids = MakeLineMenu(n_ID % 10000, FALSE);
			if( ids ) return -10;
		}
		
		return 0;
	}
		
	if( fListflag )
	{
		// �޴��ȿ��� ��ID �޽��� �� �� 
		ids = MakeLineMenu(n_ID % 10000);
		if( ids ) return -10;
	}	

	// �ڽź��� �����ϴ� �޴��� �����. 
	// ������� ù��° ���� �Ⱦ� 
	ids = AutoMakeMenu(n_ID , nILevel, nJLevel, nKLevel, !fListflag);
	if( ids ) return -20;

	return 0;
}

INT CMenuBarDlg::SendIDToParent(INT n_ID)
{
	CString strMenuId;
	strMenuId.Format(_T("%d"), n_ID % 10000);
	if ( NULL != m_pParent )
	{
		m_pParent->SendMessage(WM_MANUBARRESULT , NULL, (LPARAM)(LPCTSTR)strMenuId);
	}
	return 0;
}

// ID ���ָ� I,J,K�� �����Ѵ�.
INT CMenuBarDlg::TakeIJK(INT n_ID, INT *nILevel, INT *nJLevel, INT *nKLevel)
{
	INT ids = 0, i ;

	for (i = 0 ; i < m_AliasCount ; i ++ )
	{		
		if ( ArrayMenu[i].nID == n_ID  )
		{
			*nILevel = ArrayMenu[i].nLevel;
			*nJLevel = ArrayMenu[i].nParentIndex;
			*nKLevel = ArrayMenu[i].nIndex;
			return 0;
		}
	}
	//++2008.08.27 ADD BY CJY {{++
	// nID���� ã�� ���� ���� nMenuID�� �ٽ� �� �� �˻��Ѵ�.
	// �̷��� �ϴ� ������ MENU_ID���� 5�ڸ��� ���
	// INT CMenuBarDlg::InputDate���� nID���� ����Ǳ� �����̴�.
	// �̶� ID���� �׻� 5000�̻��̴�.
	if(5000 <= n_ID)
	{
		for (i = 0 ; i < m_AliasCount ; i ++ )
		{		
			if ( ArrayMenu[i].nMenuID == n_ID  )
			{
				*nILevel = ArrayMenu[i].nLevel;
				*nJLevel = ArrayMenu[i].nParentIndex;
				*nKLevel = ArrayMenu[i].nIndex;
				return 0;
			}
		}
	}
	//--2008.08.27 ADD BY CJY --}}
	return 5;
}

// CMenu �޴��� ����� �� 
INT CMenuBarDlg::AutoMakeMenu(INT n_ID, INT niLevel, INT njLevel, INT nkLevel, BOOL bShowFlag, BOOL bAddMenu )
{
	INT ids, i, j, k;
	INT ILevel, JLevel, KLevel;
	CPoint point;
	CString strName;

	BOOL flag;

	GetCursorPos(&point);

	CMenu mainmenu;

	// ���θ޴� ����
	ids = mainmenu.CreatePopupMenu();
	if( ids == 0 ) return -5;

	INT nItemList[16];
	INT nCount;

	// ���� �׷��� ���̵���� ��������
	ids = GetSameGroupID( niLevel, njLevel, &nCount, nItemList);
	if( ids ) return -5;

	Init();
	
	// ���� �޴����� �����ϴ� �κ� 
	for( i = 0 ; i < nCount ; i ++ )
	{
		flag = TRUE;

		if( bAddMenu )
		{
			for( j = 0; j < m_nAddMenuIndex ; j ++ )
			{
				if( nItemList[i] == (m_pListIDButton[j]%10000) )
				{
					flag = FALSE;
					break;
				}
			}
		}

		if( flag == TRUE )
		{
			
			ids = GetIJK( nItemList[i], &ILevel, &JLevel, &KLevel, &strName);	
			if( ids ) return -10;
			
			if( nMaxDepth == 1 || FALSE == GetCheckSun( ILevel, KLevel) )
			{
				//INT nTemp = GetIDByIJK( ILevel, JLevel, KLevel );
				ids = mainmenu.AppendMenu(MF_STRING , nItemList[i] , strName);
				if( 0 == ids ) return -15;
			}
			else
			{
				ids = mainmenu.AppendMenu(MF_STRING | MF_POPUP, (UINT)hmenuPopup[ILevel]/*[JLevel]*/[KLevel], strName);
				if( 0 == ids ) return -15;
			}
		}
	}


	// �׿��� �޴����� ���� 
	for( i = niLevel ; i < nMaxDepth ; i ++ )
	{/*
		for( j = 0 ; j <= nMaxItem ; j ++ )
		{*/
			for( k = 0 ; k <= nMaxItem ; k ++ )
			{				
				//strName = GetMenuName(i,j,k);
				strName = GetMenuName2(i,k);
				if( strName.GetLength() != 0 )
				{
					if( i != niLevel )
					{
						ids = GetParentIJK(i, 0, k, &ILevel, &JLevel, &KLevel);
						if( ids )return -20;
						
						if( GetCheckSun( i, k ) )
						{
							ids = AppendMenu(hmenuPopup[ILevel]/*[JLevel]*/[KLevel], MF_STRING| MF_POPUP, (UINT)hmenuPopup[i]/*[j]*/[k], strName);
							if( 0 == ids )	
							{
								DWORD temp ;
								temp = GetLastError();
								return temp;
							}
						}
						else
						{
							INT nTemp = GetIDByIJK2( i, k );
							ids = AppendMenu(hmenuPopup[ILevel]/*[JLevel]*/[KLevel], MF_STRING , nTemp, strName);
							if( 0 == ids ) 	
							{
								DWORD temp ;
								temp = GetLastError();
								return temp;
							}
						}
					}
				}
			}
		//}
	}

	// ȭ�鿡 ������ �޴����� �����ִ� �κ� 
	if( bShowFlag )
	{
		CRect rect;
		if( GetDlgItem(n_ID) !=NULL )
		{
			GetDlgItem(n_ID)->GetWindowRect(rect);
			
			ids = mainmenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN  , rect.left, rect.bottom+1 , this);		
			if ( 0 == ids ) 
			{
				DWORD temp ;
				temp = GetLastError();
				return temp;
			}
		}
	}

	Free();
	
	ids = mainmenu.DestroyMenu();
	if ( 0 == ids ) 
	{
		DWORD temp ;
		temp = GetLastError();
		return temp;
	}
	

	Invalidate(FALSE);
	
	return 0;
}

INT CMenuBarDlg::GetMaxDepth(INT niLevel, INT nkLevel)
{
	INT nCount, j;

	nCount = 0;

	for( j = 0 ; j < m_AliasCount ; j ++ )
	{
		if( ArrayMenu[j].nLevel > nCount )
		{
			nCount = ArrayMenu[j].nLevel;
		}
	} 

	return nCount + 1;
}

INT CMenuBarDlg::GetMaxItem(INT niLevel, INT nkLevel)
{
	INT nCount, j;

	nCount = 0;

	for( j = 0 ; j < m_AliasCount ; j ++ )
	{
		if( ArrayMenu[j].nIndex > nCount )
		{
			nCount = ArrayMenu[j].nIndex;
		}
	} 

	return nCount ;
}

INT CMenuBarDlg::GetSameGroupID(INT niLevel, INT njLevel, INT *Count, INT *nItemList)
{
	INT nCount, i;

	nCount = 0;

	for( i = 0 ; i < m_AliasCount ; i ++ )
	{		
		if( (ArrayMenu[i].nLevel == niLevel) && (ArrayMenu[i].nParentIndex == njLevel) )
		{
			nCount++;
			nItemList[nCount-1] = ArrayMenu[i].nID;
		}
	}
	*Count = nCount;

	return 0;
}

INT CMenuBarDlg::GetIJK(INT n_ID, INT *ILevel, INT *JLevel, INT *KLevel, CString *strName)
{
	INT nCount, i;

	nCount = 0;

	for( i = 0 ; i < m_AliasCount ; i ++ )
	{
		if( ArrayMenu[i].nID == n_ID )
		{
			*ILevel = ArrayMenu[i].nLevel;
			*JLevel = ArrayMenu[i].nParentIndex;
			*KLevel = ArrayMenu[i].nIndex;
			strName->Format(_T("%s"), ArrayMenu[i].strAlias );
		}
	}

	return 0;
}

BOOL CMenuBarDlg::GetCheckSun(INT niLevel, INT nkLevel, INT *nSunIndex)
{
	INT nCount, j;

	nCount = 0;

	for( j = 0 ; j < m_AliasCount ; j ++ )
	{		
		if( ArrayMenu[j].nLevel == niLevel+1 )
		{
			if( ArrayMenu[j].nParentIndex == nkLevel )
			{
				if( nSunIndex != NULL )
				{
					*nSunIndex = ArrayMenu[j].nIndex;
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

INT CMenuBarDlg::GetIDByIJK(INT i, INT j, INT k)
{
	INT l;

	for ( l = 0 ; l < m_AliasCount ; l ++ )
	{
		if ( (ArrayMenu[l].nLevel == i) && (ArrayMenu[l].nParentIndex == j) && (ArrayMenu[l].nIndex == k))
		{
			return ArrayMenu[l].nID;
		}
	}
	return -5;
}

INT CMenuBarDlg::GetIDByIJK2(INT i, INT k)
{
	INT l;

	for (l = 0 ; l < m_AliasCount ; l ++ )
	{
		if ( (ArrayMenu[l].nLevel == i) && (ArrayMenu[l].nIndex == k))
		{
			return ArrayMenu[l].nID;
		}
	}
	return -5;
}

CString CMenuBarDlg::GetMenuName(INT niLevel, INT njLevel, INT nkLevel)
{
	INT i; 

	for (i = 0 ; i < m_AliasCount ; i ++ )
	{		
		if ( ArrayMenu[i].nLevel == niLevel )
		{
			if( ArrayMenu[i].nParentIndex == njLevel )
			{
				if( ArrayMenu[i].nIndex == nkLevel )
				{
					return (CString) ArrayMenu[i].strAlias;
				}
			}
		}
	}
	return _T("");
}

CString CMenuBarDlg::GetMenuName2(INT niLevel, INT nkLevel)
{
	INT i; 

	for (i = 0 ; i < m_AliasCount ; i ++ )
	{		
		if ( ArrayMenu[i].nLevel == niLevel )
		{
			if( ArrayMenu[i].nIndex == nkLevel )
			{
				return (CString) ArrayMenu[i].strAlias;
			}
		}
	}
	return _T("");
}

INT CMenuBarDlg::GetParentIJK(INT i, INT j, INT k, INT *ILevel, INT *JLevel, INT *KLevel)
{
	INT l;
	j = GetParentIndex( i, k );
	for (l = 0 ; l < m_AliasCount ; l ++ )
	{		
		if ( (ArrayMenu[l].nLevel == i-1) && (ArrayMenu[l].nIndex == j) )
		{
			*ILevel = ArrayMenu[l].nLevel ;
			*JLevel = ArrayMenu[l].nParentIndex ;
			*KLevel = ArrayMenu[l].nIndex ;
		}
	}

	return 0;
}

INT CMenuBarDlg::GetParentIndex(INT i, INT k)
{
	INT l;
	for (l = 0 ; l < m_AliasCount ; l ++ )
	{		
		if ( (ArrayMenu[l].nLevel == i) && (ArrayMenu[l].nIndex == k) )
		{
			return ArrayMenu[l].nParentIndex;
		}
	}

	return 0;
}


INT CMenuBarDlg::InputDate(CObList *listMenuItem)
{
	CMenuItem *pMenuItem;

	INT i;
	INT nPos;
	INT nIndex = 0;
	CString strTempName;

	INT nRows = listMenuItem->GetCount();

	POSITION pos;
	pos = listMenuItem->GetHeadPosition();

	m_AliasCount = nRows ;

	nIndex = 3;

	// �޴��� ������ ��� 
	ArrayMenu = (MIMenu *)realloc(ArrayMenu, sizeof(MIMenu) * nIndex);

	// ���� ���� ������ �⺻���� ���� 
	_tcscpy(ArrayMenu[0].strAlias, _T("����"));
	_tcscpy(ArrayMenu[1].strAlias,  _T("����"));
	_tcscpy(ArrayMenu[2].strAlias,  _T("����"));
	ArrayMenu[0].nParentIndex = 0;	
	ArrayMenu[1].nParentIndex = 0;
	ArrayMenu[2].nParentIndex = 0;
	ArrayMenu[0].nIndex = 0;	
	ArrayMenu[1].nIndex = 1;
	ArrayMenu[2].nIndex = 2;
	ArrayMenu[0].nID = 1000;	
	ArrayMenu[1].nID = 2000;
	ArrayMenu[2].nID = 3000;
	ArrayMenu[0].nLevel = 0;
	ArrayMenu[1].nLevel = 0;
	ArrayMenu[2].nLevel = 0;



	// �ߵ��鼭 ������� ��� �Ѵ�..

	// ��Ÿ���� s�ΰ��� ������ 1���� �� �����Ͽ� 
	// �θ�� ����. �ε����� 1�϶���, �׸��� �ڽ��� �װ��� ���̵� �װ� ����ε��� ���� 

	for( i = 0 ; i < m_AliasCount ; i ++ )
	{		
		pMenuItem = NULL;
		pMenuItem = (CMenuItem *)listMenuItem->GetNext(pos);

		if( ( pMenuItem->m_nILevel == NULL ) && ( pMenuItem->m_nJLevel == NULL ) ) 
		{
			continue;
		}

		// �ڽ��� �ƴ� �ڽ��� �θ� ���� �����ϴ� ���� 
		// ��Ÿ���� ��Ƽ �϶�, �ε����� 1�϶� �ѹ���, 3�����϶� 
		if( ( pMenuItem->m_strStyle == _T("S") ) && ( pMenuItem->m_nIndex == 1 ) && ( pMenuItem->m_nILevel == 3 ) )
		{	
			ArrayMenu = (MIMenu *)realloc(ArrayMenu, sizeof(MIMenu) * (nIndex + 1));
			
			ArrayMenu[nIndex].nLevel = pMenuItem->m_nILevel-1;

			strTempName = pMenuItem->m_strGroupAlias;
			nPos = pMenuItem->m_strGroupAlias.Find(_T("_"));

			strTempName.Delete(0, nPos+1);
			
			// �̸��� ����ٱ��� �߶󳽴� ex) CO6_����ϰ������� => ����ϰ�������
			_tcscpy(ArrayMenu[nIndex].strAlias,  strTempName );

			// ���̵� 4õ�� ������ 
			if( _ttoi(pMenuItem->m_strMenuID) > 4000 )
			{
				// �θ� �ε����� 21�� �����Ѵ�.
				ArrayMenu[nIndex].nParentIndex = 21;
			}
			else
			{
				// �ٸ� ���� �Ǿյ��ڸ��� 00�� ���δ� ex) 3910 -> 3900
				for( INT j = 0 ; j < nIndex ; j ++ )
				{
					if( (_ttoi(pMenuItem->m_strMenuID) / 100 * 100 ) == ArrayMenu[j].nID )
						ArrayMenu[nIndex].nParentIndex = ArrayMenu[j].nIndex;
				}
			}
			// �ε����� �װ��� �θ��� ������ �ε����� �Ѵ� ( DB���� )
			ArrayMenu[nIndex].nIndex = pMenuItem->m_nJLevel;
			// ID�� 5�� �����ϴ� ������ ���ڷ� �Ѵ� ( ���޴��� ������ 999 �����ϋ��� ���� ) 
			ArrayMenu[nIndex].nID = 5000 + nIndex;
			nIndex++;
		}

		// �ڽ��� �����ϴ� ���� �̸��� ijk�� �����Ѵ�.
		// ���� �ڽ��� �����Ѵ�.
		// �޴��� ������ ��� 
		ArrayMenu = (MIMenu *)realloc(ArrayMenu, sizeof(MIMenu) * (nIndex + 1));		


		// 1������ �͵� ó�� 
		// i������ 1�̰� �ڽ��� �ε����� 1�϶� 
		if( ( pMenuItem->m_nILevel == 1 ) && ( pMenuItem->m_nIndex == 1 ) )
		{
			strTempName = pMenuItem->m_strGroupAlias;
			nPos = pMenuItem->m_strGroupAlias.Find(_T("."));

			strTempName.Delete(0, nPos+1);

			// �� ���ĸ� �̸����� ���� ( ����.���� => ���� )
			_tcscpy(ArrayMenu[nIndex].strAlias,  strTempName );
			// ���̵�� 151 => 1500 ������ �����Ѵ�.
			ArrayMenu[nIndex].nID = _ttoi(pMenuItem->m_strMenuID) / 10 * 100;
		}
		else
		{
			// �׿��� ���� �̸��� ĸ���� �̸��� ����. ( �����ڰ��� ) 
			_tcscpy(ArrayMenu[nIndex].strAlias,  pMenuItem->m_strCaption );

			if( _ttoi(pMenuItem->m_strMenuID) > 9999 )
			{
				//���̵� 9999���� ũ�� ��, 5�ڸ����̵��̸� ( 4���� �͵� ) 5000+�� ������ ���� ����� �ش�.
				ArrayMenu[nIndex].nID = 5000 + nIndex;
				//++2008.08.27 ADD BY CJY {{++
				// �ڶ�3���� ID�� �������� ���� nID�� �ƴ�
				// ���� MENU_ID���� �Ѿ���� ������ �� ���� ���ϱ� ����
				// ���� MENU_ID���� ������ �����Ų��
				ArrayMenu[nIndex].nMenuID = _ttoi(pMenuItem->m_strMenuID);
				pMenuItem->m_nID = 5000 + nIndex;				
				//--2008.08.27 ADD BY CJY --}}
			}
			else
			{
				// �׿��� ���� �׳� ���̵� ����. 
				ArrayMenu[nIndex].nID = _ttoi(pMenuItem->m_strMenuID);
			}
		}

		// IJK ���� �ִ� �״�� �����Ѵ�. 
		ArrayMenu[nIndex].nLevel = pMenuItem->m_nILevel;
		ArrayMenu[nIndex].nParentIndex = pMenuItem->m_nJLevel;
		ArrayMenu[nIndex].nIndex = pMenuItem->m_nKLevel;

		nIndex++;
	}

	m_AliasCount = nIndex ;

	ModifyStyle(0, /*WS_CLIPCHILDREN |*/ WS_CLIPSIBLINGS);

	return 0;
}


INT CMenuBarDlg::PutDataInStruct()
{
	// ��ǲ�� �������� �޴´�.

// ���� ��ǲ 

	_tcscpy(ArrayMenu[0].strAlias, _T("����"));
	_tcscpy(ArrayMenu[1].strAlias,  _T("����"));

	_tcscpy(ArrayMenu[2].strAlias,  _T("�������"));
	_tcscpy(ArrayMenu[3].strAlias,  _T("����ڷ����"));
	_tcscpy(ArrayMenu[4].strAlias,  _T("�����ڷ����"));

	_tcscpy(ArrayMenu[5].strAlias,  _T("���"));
	_tcscpy(ArrayMenu[6].strAlias,  _T("��ϰ���"));
	_tcscpy(ArrayMenu[7].strAlias,  _T("����ڷ����"));
	_tcscpy(ArrayMenu[8].strAlias,  _T("EPC�ϰ��ο�"));

	ArrayMenu[0].nParentIndex = 0;	
	ArrayMenu[1].nParentIndex = 0;

	ArrayMenu[2].nParentIndex = 0;
	ArrayMenu[3].nParentIndex = 0;
	ArrayMenu[4].nParentIndex = 0;

	ArrayMenu[5].nParentIndex = 0;
	ArrayMenu[6].nParentIndex = 1;
	ArrayMenu[7].nParentIndex = 2;
	ArrayMenu[8].nParentIndex = 2;

	ArrayMenu[0].nIndex = 0;	
	ArrayMenu[1].nIndex = 1;

	ArrayMenu[2].nIndex = 0;
	ArrayMenu[3].nIndex = 0;
	ArrayMenu[4].nIndex = 1;

	ArrayMenu[5].nIndex = 1;
	ArrayMenu[6].nIndex = 2;
	ArrayMenu[7].nIndex = 0;
	ArrayMenu[8].nIndex = 1;


	ArrayMenu[0].nID = 1000;	
	ArrayMenu[1].nID = 4000;

	ArrayMenu[2].nID = 1100;
	ArrayMenu[3].nID = 1110;
	ArrayMenu[4].nID = 1120;

	ArrayMenu[5].nID = 1300;
	ArrayMenu[6].nID = 1310;
	ArrayMenu[7].nID = 1311;
	ArrayMenu[8].nID = 1312;
	
	ArrayMenu[0].nLevel = 0;	
	ArrayMenu[1].nLevel = 0;

	ArrayMenu[2].nLevel = 1;
	ArrayMenu[3].nLevel = 2;
	ArrayMenu[4].nLevel = 2;

	ArrayMenu[5].nLevel = 1;
	ArrayMenu[6].nLevel = 2;
	ArrayMenu[7].nLevel = 3;
	ArrayMenu[8].nLevel = 3;

	_tcscpy(ArrayMenu[9].strAlias,  _T("����"));
	ArrayMenu[9].nParentIndex = 0;
	ArrayMenu[9].nIndex = 2;
	ArrayMenu[9].nID = 3000;
	ArrayMenu[9].nLevel = 0;


	return 0;
}

BOOL CMenuBarDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CMenuBarDlg::Init()
{
	INT i, k ;
	CString strName;

	nMaxDepth = GetMaxDepth(0, 0)+1;
	nMaxItem  = GetMaxItem(0, 0)+1;

	// �ִ� ���� ��� ( ������ ���� )
	hmenuPopup = (HMENU**)malloc(sizeof(HMENU**) * nMaxDepth);
	for (i = 0 ; i < nMaxDepth ; i ++ )
	{
		hmenuPopup[i] = (HMENU*)malloc(sizeof(HMENU*) * nMaxItem);
		for ( k = 0 ; k <= nMaxItem ; k ++ )
		{
			strName = GetMenuName2(i,k);
 			if( strName.GetLength() != 0 )
			{
				hmenuPopup[i][k] = ::CreatePopupMenu();				
			}				
		}	
	}

	return 0;
}

INT CMenuBarDlg::Free()
{
	INT i, k ;
	CString strName;

	nMaxDepth = GetMaxDepth(0, 0)+1;
	nMaxItem  = GetMaxItem(0, 0)+1;

	// �ִ� ���� ��� ( ������ ���� )
	for (i = 0 ; i < nMaxDepth ; i ++ )
	{
		for ( k = 0 ; k < nMaxItem ; k ++ )
		{
			strName = GetMenuName2(i,k);
			if( strName.GetLength() != 0 )
			{
				INT ids = DestroyMenu(hmenuPopup[i][k]);
				if( 0 == ids )	
				{
					DWORD temp ;
					temp = GetLastError();
					//return temp;
				}
			}				
		}
		free(hmenuPopup[i]);
	}
	free(hmenuPopup);

	return 0;
}

HBRUSH CMenuBarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if( nCtlColor == CTLCOLOR_STATIC )
    {	
		// ������� 
		pDC->SetBkMode(TRANSPARENT);
		// �۾���
		pDC->SetTextColor(TEXTCOLOR);
		// Add Menu Color (��)
		if( pWnd->GetDlgCtrlID() == IDB_ADDMENU  )
		{
			pDC->SetTextColor(ADDMENUCOLOR);
			OldFont = pDC->SelectObject(&m_Font);
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		}
		// Menu Color
		else
		{	
			for( INT i = 0; i < m_nButtonCount; i++ )
			{
				if(	( pWnd->GetDlgCtrlID() == m_pListIDButton[i] ) )
				{
					// ���콺�� ��ġ�� Static
					if( m_Statichwnd == pWnd->GetSafeHwnd() )
					{
						// �۾��� �� ����� �����Ѵ�.
						pDC->SetTextColor(CHOOSE_TXTCOLOR);	
						OldFont = pDC->SelectObject(&m_UnderLineFont);
						
						// Left Menu ������ Index�� ����
						if( m_nBorderLine-1 == i)
						{
							pDC->SetTextColor(CHOOSE_TXTCOLOR);
							OldFont = pDC->SelectObject(&m_CurrentMenuFont);
						}
					}
					// ���콺�� ��ġ���� ���� Static
					else
					{
						CRect rect;
						GetDlgItem(m_pListIDButton[i])->GetClientRect(rect);
						ReDrawBG(pDC, rect);
						OldFont = pDC->SelectObject(&m_Font);	
						
						// Left Menu ������ Index�� ����
						if( m_nBorderLine-1 == i)
						{
							pDC->SetTextColor(TEXTCOLOR);
							OldFont = pDC->SelectObject(&m_BoldFont);
						}
					}
					return (HBRUSH)GetStockObject(NULL_BRUSH);
				}
			}
			OldFont = pDC->SelectObject(&m_Font);
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		}
	}
	return hbr;
}


BOOL CMenuBarDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HWND temp,temp2;
	temp = pWnd->GetSafeHwnd();	

	// ���� Ŀ���� �ִ� ���̶� ������ Ŀ���� �ִ��� �ٸ� �� 
	if( m_Statichwnd != temp )
	{
		CRect rect;
		::GetClientRect(m_Statichwnd,&rect);

		temp2 = this->GetSafeHwnd();

		if( m_Statichwnd != temp2)
		 	InvalidateRect(rect,TRUE);		
		else
		 	Invalidate(FALSE);

		m_Statichwnd = temp;
	}
	else
	{
		return TRUE;
	}

	CPoint	pt;			// ���콺 Ŀ�� ��ġ�� ������ ��ü
	CRect	rc;			// ����ƽ ��Ʈ���� ��ġ�� ������ ��ü.
	GetCursorPos(&pt);	// ���콺 Ŀ���� ��ġ�� ã�ƿ´�.
	
	if( GetDlgItem(IDB_ADDMENU) != NULL )
	{
		GetDlgItem(IDB_ADDMENU)->GetWindowRect(rc);
		
		// ���� ���콺�� ����ƽ ��Ʈ�� ���� ��������..
		if(rc.PtInRect(pt))
		{
			::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
			//GetDlgItem(m_pListIDButton[i])->Invalidate(FALSE);
			return TRUE;
		}
	}
	for( INT i = 0; i < m_nButtonCount; i++ )
	{		
		// ����ƽ ��Ʈ���� ��ġ�� ã�ƿ´�.
		if( GetDlgItem(m_pListIDButton[i]) != NULL )
		{
			GetDlgItem(m_pListIDButton[i])->GetWindowRect(rc);
			
			// ���� ���콺�� ����ƽ ��Ʈ�� ���� ��������..
			if(rc.PtInRect(pt))
			{
				::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
				//GetDlgItem(m_pListIDButton[i])->Invalidate(FALSE);
				return TRUE;
			}
		}
	}

	/*
	// ] �� ����� �͵��� �������� [] �ѽ־� �� 
	for( i = m_nBmpInfoCount-1 ; i >= 0 ; i-=2 )
	{
		// 2���� Ŭ�� �� ������ �Ǿ����� �ȵǾ������� [] ���� ��쿡
		if( BmpInfoList[i].nKind >= 2 )
		{
			// ���콺 Ŀ���� ���̾ȿ� ��ġ�ϴ��� Ȯ��
			GetDlgItem(BmpInfoList[i-1].nID)->GetWindowRect(rc);
			if(rc.PtInRect(pt))
			{
				// �ȿ� ��ġ�ϰ� ������ ������ �͵��� �������� �����ϰ� 
				BmpInfoList[i-1].nKind = 4;
				BmpInfoList[i].nKind = 5;

				for( INT j = 0 ; j < m_nBmpInfoCount ; j ++)
				{
					// �ڽ��̰ų� �ڽ������� �϶��� �����·� ������ ���� ( �ȼ��õ� ���·� )
					//                    I
					// EX)   [    ]  [    ]  [    ]
					//               J    J      �̷� ��쿡�� ��ŵ    
					if( (i != j) && (i != j+1) )
					{
						if( BmpInfoList[j].nKind == 5 )
							BmpInfoList[j].nKind = 3;
						if( BmpInfoList[j].nKind == 4 )
							BmpInfoList[j].nKind = 2;
					}
				}
				Invalidate();
			}			
		}
	}
	*/

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CMenuBarDlg::OnEraseBkgnd(CDC* pDC) 
{

	if(!m_IsBkPaint)
	{
		InitBMP(pDC);
		m_IsBkPaint = true;
	}

	// ���� �ٽ� ĥ���ֱ� 
	FillBackground(pDC);


	// �׸� �־� �ֱ� 
	for( INT i =0 ; i < m_nBmpInfoCount ; i ++ )
	{
		// ������ �Ʒ��� ���� ���� 
		pDC->BitBlt( BmpInfoList[i].rcPosition.left , 9 ,info[BmpInfoList[i].nKind].bmWidth,
			info[BmpInfoList[i].nKind].bmHeight,&m_BkDC[BmpInfoList[i].nKind], 0, 0, SRCCOPY);		
	}

	// ������ ����ƽ �׷��ֱ� 
	Invalidate(FALSE);

	return FALSE;
}

VOID CMenuBarDlg::FillBackground(CDC *pDC)
{
	if( pDC->GetSafeHdc() == NULL )
		return;

	CRect rect;
	GetClientRect(&rect);
	
	TRIVERTEX data1[2];

	data1[0].x = rect.left;	
	data1[0].y = rect.top+1;		
	data1[0].Red   = 252 * 255;
	data1[0].Green = 253 * 255;
	data1[0].Blue  = 253 * 255;
	data1[0].Alpha = 0x0000;

	data1[1].Red   = 231 * 255;
	data1[1].Green = 231 * 255;
	data1[1].Blue  = 231 * 255;
	data1[1].Alpha = 0x0000;
	data1[1].x  = rect.right;
	data1[1].y  = rect.bottom;
	
	::GradientFill(pDC->m_hDC, data1, 2, &draw_rect1, 1, GRADIENT_FILL_RECT_V);

}

INT CMenuBarDlg::MakeAddMenu()
{
//	CMenu mainmenu;

	INT ilevel, JLevel, KLevel;

	CString strName;

	GetIJK(m_pListIDButton[m_nAddMenuIndex]%10000, &ilevel, &JLevel, &KLevel, &strName);


	AutoMakeMenu( IDB_ADDMENU, ilevel, JLevel, KLevel, TRUE, TRUE);

	Invalidate(FALSE);

	return 0;

}

void CMenuBarDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(  abs(m_XPosion - cx) > 5 )
	{
		KillTimer(1);
		SetTimer(1, 200, NULL);
		m_XPosion = cx;
	}
}

void CMenuBarDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if( nIDEvent == 1 )
	{
		MakeLineMenu(m_CurrentID % 10000, TRUE, TRUE);
		KillTimer(1);
	}

	CDialog::OnTimer(nIDEvent);
}


void CMenuBarDlg::ReDrawBG(CDC *pDC, CRect rect)
{	
	if( pDC->GetSafeHdc() == NULL )
		return;
		
	TRIVERTEX data1[2];
	
	data1[0].x = rect.left;	
	data1[0].y = rect.top;		
	data1[0].Red   = 250 * 255;
	data1[0].Green = 251 * 255;
	data1[0].Blue  = 251 * 255;
	data1[0].Alpha = 0x0000;
	
	data1[1].Red   = 233 * 255;
	data1[1].Green = 233 * 255;
	data1[1].Blue  = 233 * 255;
	data1[1].Alpha = 0x0000;
	data1[1].x  = rect.right;
	data1[1].y  = rect.bottom;
	
	::GradientFill(pDC->m_hDC, data1, 2, &draw_rect1, 1, GRADIENT_FILL_RECT_V);
}

void CMenuBarDlg::InitBMP(CDC *pDC)
{
	for( INT i = 0 ; i < 6 ; i ++ )
	{
		// prevent continuous loading.
		INT ids =  m_BkBmp[i].LoadBitmap(IDB_LEFTBMP + i) ;
		if( ids == 0)
			return ;
		m_BkBmp[i].GetObject(sizeof(BITMAP),&info[i]);
			
		m_BkDC[i].CreateCompatibleDC(pDC);
		m_BkDC[i].SelectObject(&m_BkBmp[i]);
	}
	return;	
}
