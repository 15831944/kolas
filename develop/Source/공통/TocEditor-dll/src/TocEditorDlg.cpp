// TocEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TocEditorDlg.h"
#include "ErrorQueue.h"

#include "TocSyntaxChecker.h"
#include "ChorokDlg.h"
#include "TocViewerDlg.h"

#include "ChorokEditorDlg.h"

#include "BibilographyDlg.h"
#include "BodyDlg.h"
#include "YonSujiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LINE_HEIGHT			12			// ���� ���� ( pixel )
#define BODY_START_TAG		_T("<body>")
#define BODY_START_LENGTH	6
#define BODY_END_TAG		_T("</body>")

CStringArray g_arrAlreadyExistKisaNo;	// �ӽú��� - ���߿� ����� �ű��

/////////////////////////////////////////////////////////////////////////////
// CTocEditorDlg dialog


CTocEditorDlg::CTocEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTocEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTocEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nStartChar = m_nEndChar = 0 ;
	m_icurLine = 1 ;
	m_nIsKindOf = _DAN_SINGLE ;	// ���ົ(���Ϻ�,�ٱǺ�), ���Ӱ��๰ default�� ���ϴ��ົ
	m_bUpDown = FALSE ;		// PAGE_UP or PAGE_DOWN key
	m_bTagEnd = FALSE ;		// '>' �ڿ� ��Ʈ(��) ���� ����

	m_pParentWnd= NULL ;
	m_pYonKeyFunc= NULL ;
	m_pMoveRecordFunc= NULL ;

	Clear() ;

	m_aChorokStru.SetSize(0,1) ;
	m_aBackupChorokStru.SetSize(0,1) ;
	m_aRedoChorokStru.SetSize(0,1) ;
	m_arrWonmunKisaNo.SetSize(0,1) ;

	bIsValidKey= FALSE ;	// tmp
	bIsCut= FALSE ;			// tmp
//	csId.Empty(); csCall.Empty(); csCount.Empty(); csDate.Empty();
//	csPub.Empty(); csSertitle.Empty(); csTitle.Empty(); csIssue.Empty(); csKey.Empty();
	for(INT i=0;i<8;i++) csstrTag[i].Empty();
}

CTocEditorDlg::~CTocEditorDlg()
{
	m_aChorokStru.RemoveAll() ;
	m_aChorokStru.FreeExtra() ;
	m_aBackupChorokStru.RemoveAll() ;
	m_aBackupChorokStru.FreeExtra() ;
	m_aRedoChorokStru.RemoveAll() ;
	m_aRedoChorokStru.FreeExtra() ;

	m_arrWonmunKisaNo.RemoveAll() ;
	m_arrWonmunKisaNo.FreeExtra() ;

	m_pYonKeyFunc= NULL ;
	m_pMoveRecordFunc= NULL ;
}

VOID CTocEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTocEditorDlg)
	DDX_Control(pDX, IDC_TOCEDITOR, m_TocEdit);
	DDX_Control(pDX, IDC_LINE_VIEW, m_LineView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTocEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CTocEditorDlg)
	ON_COMMAND(ID_TOC_UNDO, OnUndo)
	ON_COMMAND(ID_TOC_REDO, OnRedo)
	ON_COMMAND(ID_TOC_CUT, OnTocCut)
	ON_COMMAND(ID_TOC_COPY, OnTocCopy)
	ON_COMMAND(ID_TOC_PASTE, OnTocPaste)
	ON_COMMAND(ID_CHECK_SYNTAX, OnCheckSyntax)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(EN_SELCHANGE, IDC_TOCEDITOR, OnSelchangeEditRichedit)
	ON_EN_CHANGE(IDC_TOCEDITOR, OnChangeToceditor)
	ON_WM_TIMER()
	ON_MESSAGE(MSG_EDIT_YON_CHOROK, OnEditYonChorok)
	ON_MESSAGE(MSG_EDIT_DAN_CHOROK, OnEditDanChorok)
	ON_MESSAGE(WM_READ_ECOMATH, OnReadEcoMath)
	ON_MESSAGE(MSG_SHOW_VIEWER, OnShowViewer)
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTocEditorDlg message handlers

BOOL CTocEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_TocEdit.SetEventMask( m_TocEdit.GetEventMask() | ENM_CHANGE | ENM_SELCHANGE ) ;
	
//	m_TocEdit.SetOptions(ECOOP_SET, ~ECO_NOHIDESEL);
	m_LineView.SetBackgroundColor(FALSE, RGB(238,238,238));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CTocEditorDlg::OnUndo() 
{
//	return ;		// ������� �ʵ��� ����.(�ѱ۹���)

	if ( m_sBackupTocEdit.IsEmpty() ) {
		TRACE( _T("Undo Empty!\n")) ;
		return ;
	}

	m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;
	CString tmp ;
	m_TocEdit.GetWindowText( tmp ) ;
	m_sRedoTocEdit= tmp ;
	m_aRedoChorokStru.Copy(m_aChorokStru) ;
	m_TocEdit.SetWindowText( m_sBackupTocEdit ) ;
	m_aChorokStru.Copy(m_aBackupChorokStru) ;
	m_sBackupTocEdit.Empty() ;
	m_TocEdit.SetSel( m_nStartChar, m_nEndChar ) ;
	
	DisplayCurrentLineNumber() ;
}

VOID CTocEditorDlg::OnRedo() 
{
//	return ;		// ������� �ʵ��� ����.(�ѱ۹���)

	if ( m_sRedoTocEdit.IsEmpty() ) {
		TRACE(_T("Redo Empty!\n") ) ;
		return ;
	}

	m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;

	TocEditorBackup() ;

	m_TocEdit.SetWindowText( m_sRedoTocEdit ) ;
	m_aChorokStru.Copy(m_aRedoChorokStru) ;

	m_sRedoTocEdit.Empty() ;
	m_aRedoChorokStru.RemoveAll() ;

	m_TocEdit.SetSel( m_nStartChar, m_nEndChar ) ;

	DisplayCurrentLineNumber() ;
}

VOID CTocEditorDlg::OnTocCut() 
{
	// ��� ���ش�.
	m_TocEdit.GetSel( m_nStartChar , m_nEndChar ) ;
	if ( m_nStartChar==m_nEndChar ) return ;

	TocEditorBackup() ;

	m_TocEdit.Cut() ;

	DisplayCurrentLineNumber() ;
}

VOID CTocEditorDlg::OnTocCopy() 
{
	m_TocEdit.Copy() ;
}

VOID CTocEditorDlg::OnTocPaste() 
{
//	if (! ::IsClipboardFormatAvailable(CF_TEXT))	return ;
	if (! m_TocEdit.CanPaste() )	return ;

	HGLOBAL hglb=NULL;
	CString sText ;
	OpenClipboard();

	//++2008.09.04 UPDATE BY CJY {{++
	// �����ڵ��������� ����
	hglb= GetClipboardData(CF_UNICODETEXT);
	// �����ҽ�
	// hglb= GetClipboardData(CF_TEXT);
	//--2008.09.04 UPDATE BY CJY --}}
	
	if (hglb != NULL)
	{
		sText= (LPCTSTR)(LPCTSTR)GlobalLock(hglb);
		GlobalUnlock(hglb);
	}
	CloseClipboard();

	if ( sText.IsEmpty() )	return ;

	INT nStartFind= sText.Find(_START_CHOROK_KEY_) ;
	if (nStartFind>=0)
	{
		if ( sText.Find(_END_CHOROK_KEY_ , nStartFind)>0 )
		{
			MessageBox(_T("��������ȣ �±װ� �ִ� �ؽ�Ʈ�� �ٿ��ֱ� �� �� �����ϴ�."), _T("���"), MB_ICONWARNING) ;
			return ;
		}
	}

	LONG nStartChar, nEndChar ;
	m_TocEdit.GetSel( nStartChar, nEndChar ) ;
	m_TocEdit.SetSel( nStartChar-1, nEndChar-1 ) ;

	if( m_TocEdit.GetSelText()==_T(">") ) m_bTagEnd = TRUE ;
	m_TocEdit.SetSel( nStartChar, nEndChar ) ;

	// ��� ���ش�.
	TocEditorBackup() ;

	m_TocEdit.Paste() ;

	long nIdx= nStartChar + sText.GetLength() ;

	// �±׿� ���� �ٽ� ĥ���ֱ� ����
	m_TocEdit.GetWindowText( m_strBak ) ;
	m_TocEdit.SetWindowText( m_strBak ) ;

	m_TocEdit.SetSel(nIdx, nIdx) ;
//	m_TocEdit.DisplayTagColor(TRUE);

	DisplayCurrentLineNumber() ;
}

INT CTocEditorDlg::CheckSyntax()
{
	long nStartChar, nEndChar ;
	nStartChar= nEndChar= 0 ;

	m_TocEdit.HideSelection(TRUE, TRUE);

	m_TocEdit.GetSel( nStartChar, nEndChar ) ;

	CErrorQueue ErrorQueue;
	ErrorQueue.ResetContent();

//	cfg ���� ��μ���.
	TCHAR m_exeDir[256];
	GetCurrentDirectory( 256 , m_exeDir ) ;
	CString sWorkDir = m_exeDir ;
	sWorkDir.TrimRight(_T("\\"));
	INT idx= sWorkDir.ReverseFind( _T('\\') ) ;
	if( idx<=0 ) {
		AfxMessageBox( _T("cfg ������ ��θ� �������� �����ϴ�.\r\n����üũ�� ������ �� �����ϴ�!") , MB_ICONERROR | MB_OK ) ;
		return -2 ;
	}
	sWorkDir= sWorkDir.Mid(0,idx) ;
	sWorkDir += PATH_CFG ;

	CTocSyntaxChecker TocSyntaxChecker(& ErrorQueue, sWorkDir, m_nIsKindOf);

	// �±׿� ���� �ٽ� ĥ���ֱ� ����
	m_TocEdit.GetWindowTextNoMath( m_strBak ) ;
//	m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;
//	m_TocEdit.SetWindowText( m_strBak );
	m_TocEdit.DisplayTagColor(FALSE);
//	m_TocEdit.SetSel(0, -1);
//	m_TocEdit.SetSelectionCharFormat(m_TocEdit.cfBase);
//	m_TocEdit.SetSel( m_nStartChar, m_nEndChar ) ;

	m_TocEdit.DisplayTagColor(TRUE);

	// ���� �ӽ� ������ �������� �װ��� �Ѱ��ش�.	���߿� �����.
	TCHAR szPath[MAX_PATH];
	TCHAR szTempFileName[MAX_PATH];
	GetTempPath(MAX_PATH, szPath);
	GetTempFileName(szPath, _T("toc"), 0, szTempFileName);
	SaveFile(szTempFileName);

	TocSyntaxChecker.CheckSyntax(szTempFileName);
	DeleteFile(szTempFileName);

	m_SyntaxCheckDlg.DeleteAllItems() ;

	INT iMaxCount = ErrorQueue.GetErrorCount();
	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		m_SyntaxCheckDlg.SetErrorMsg(_T(" ") + ErrorQueue.GetLine(i), ErrorQueue.GetErrorMessage(i), ErrorQueue.GetSolution(i), i) ;
	}

	m_TocEdit.SetSel( nStartChar, nEndChar ) ;

	if( iMaxCount > 0 ) {	// ���� ����
		m_SyntaxCheckDlg.ShowWindow(SW_SHOW) ;
		return -1 ;
	}
	else {					// ���� ����
		m_SyntaxCheckDlg.ShowWindow(SW_HIDE) ;
		return 0 ;
	}

	m_TocEdit.HideSelection(FALSE, TRUE);

}

VOID CTocEditorDlg::OnCheckSyntax()
{
	INT retval= CheckSyntax() ;
	if ( retval==0 )
	{
		MessageBox( _T("������ �����ϴ�."), _T("���� ����üũ") ) ;
	}
}

VOID CTocEditorDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_SyntaxCheckDlg.Create(IDD_SYNTAXCHECK_DIALOG, this) ;	// Modaless �� ����ش�.
	m_SyntaxCheckDlg.SetParentWnd(this) ;

	m_TocEdit.SetKindOf(m_nIsKindOf, this) ;

	// ���� ǥ�� - 1
	m_LineView.SetWindowText(_T("1"));

	m_TocEdit.OnInitialUpdate();
	
}

VOID CTocEditorDlg::SetCurrentErrorLine(INT iLine)
{
	if ( iLine<=0 )	return ;

	m_TocEdit.SetSelectionLine(iLine-1);

	while( TRUE ) {
		INT nVisibleTopLine = m_TocEdit.GetFirstVisibleLine() + 1;
		INT nVisibleBottomLine = GetVisibleBottomLine() ;

		// Error line�� ȭ�鿡 �����ش�.
		if( iLine < nVisibleTopLine )
			m_TocEdit.SendMessage( WM_VSCROLL , SB_PAGEUP , NULL ) ;
		else if( iLine > nVisibleBottomLine )
			m_TocEdit.SendMessage( WM_VSCROLL , SB_PAGEDOWN , NULL ) ;
		else break ;
	}

}

BOOL CTocEditorDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == ID_TOC_NEW)
		AfxMessageBox(_T("new"));

	bIsValidKey= FALSE ;
	bIsCut= FALSE ;		//	Ctrl + 'X'	- Cut
	long nStartChar, nEndChar ;
	m_TocEdit.GetSel( nStartChar, nEndChar ) ;

	switch ( pMsg -> message ) {
	case WM_KEYDOWN :

		if( pMsg -> wParam == VK_UP || pMsg -> wParam == VK_DOWN ) m_bUpDown = TRUE ;

		if( pMsg->wParam >= 0x30 && pMsg->wParam <= 0x39 )		bIsValidKey = TRUE ;	// '0' ~ '9'
		else if( pMsg->wParam >= 0x41 && pMsg->wParam <= 0x5A )	bIsValidKey = TRUE ;	// 'A' ~ 'Z'
		else if( pMsg->wParam >= 0x60 && pMsg->wParam <= 0x6f )	bIsValidKey = TRUE ;	// numpad ; 96<pMsg->wParam<111
		else if( pMsg->wParam >= 0xba && pMsg->wParam <= 0xc0 )	bIsValidKey = TRUE ;	// 186<pMsg->wParam<192
		else if( pMsg->wParam >= 0xdb && pMsg->wParam <= 0xde )	bIsValidKey = TRUE ;	// 219<pMsg->wParam<222
		else if( pMsg->wParam == 0xe5 )	{	// �ѱ� �Է½�
//			m_TocEdit.GetWindowText( m_strBak ) ;
			bIsValidKey= TRUE ;
		}
		else {
			switch( pMsg->wParam ) {
			case VK_DELETE :
			case VK_BACK :
//				DisplayCurrentLineNumber() ;
			case VK_SPACE :
			case VK_RETURN :
				bIsValidKey = TRUE ;
				break ;
			}
		}

		if ( GetAsyncKeyState(VK_CONTROL) )
		{
			switch( pMsg->wParam ) {
			case 86 :	//	Ctrl + 'V'	- Paste
			case 88 :	//	Ctrl + 'X'	- Cut
				bIsValidKey= TRUE ;
				bIsCut= TRUE ;
				break ;
			default :
				bIsValidKey= FALSE ;
				break ;
			}
		}
		

		// ���������� ��������ȣ�� ó���ϱ� ���ؼ�...
		if ( m_nIsKindOf==_YON_ && bIsValidKey )
		{
			CString sSelText ;
			sSelText= m_TocEdit.GetSelText() ;
			m_TocEdit.GetWindowText(m_strBak) ;

			if ( nStartChar!=nEndChar )
			{
				// ��������ȣ �±� ��ȣ ��ƾ!
				// ��������ȣ�±״� ������ �� ������ ���ش�.
				if ( m_strBak.GetLength()>nEndChar && m_strBak.Mid( nEndChar, 2 )==_T("\r\n") )	// �Ѷ��ο��� ����Ű ���ﶧ nEndChar�� �� ���� �����������
				{
					if ( IsInKisaTag(m_strBak, m_icurLine, nEndChar+1 ) )	// nEndChar ����
					{
						return TRUE ;
					}
				}
				else
				{
					if ( IsInKisaTag(m_strBak, m_icurLine, nEndChar ) )	// nEndChar ����
					{
						return TRUE ;
					}
				}
				// nStartChar ����
				{
//					if ( sSelText.Find(_T("\r\n"))>=0 )
//					{
						INT curLine= 1 ;
						for( INT idx=0 ; idx<nStartChar ; idx++ ) {
							if( m_strBak.GetAt(idx)=='\n' ) curLine++ ;
						}

						if ( IsInKisaTag(m_strBak, curLine, nStartChar ) )
						{
							return TRUE ;
						}
//					}
				}

				// ����ڰ� ������ ������ ��������ȣ�� �ִٸ� �����޽��� ��, �����ش�.
				CString sStartTag= _START_CHOROK_KEY_ ;
				CString sEndTag= _END_CHOROK_KEY_ ;
				if ( nEndChar-nStartChar>sStartTag.GetLength()+sEndTag.GetLength() )
				{
					INT nStart= 0 ;
					INT nFindStart= -1 ;
					INT nFindEnd= -1 ;
					CStringArray arrDeletedKisaNo ;
					arrDeletedKisaNo.SetSize(0,1) ;

					do {
						nFindStart= sSelText.Find( sStartTag, nStart ) ;
						if ( nFindStart>=0 )
						{
							nStart= nFindStart + sStartTag.GetLength() ;
							nFindEnd= sSelText.Find( sEndTag, nStart ) ;
							if ( nFindEnd>=0 )
							{
								CString sKisaNo ;
								sKisaNo= sSelText.Mid( nStart, nFindEnd-nStart) ;
								if ( IsYonKisaNoRight(sKisaNo) )
								{
									INT nSize= m_arrWonmunKisaNo.GetSize() ;
									for ( INT idx=0; idx<nSize ; idx++)
									{
										if ( sKisaNo==m_arrWonmunKisaNo.GetAt(idx) )
										{
											MessageBox(_T("������ ����� ���� ���� �� �����ϴ�."), _T("���"), MB_ICONWARNING|MB_OK ) ;
											return TRUE ;
										}
									}
									arrDeletedKisaNo.Add(sKisaNo) ;
								}

								nStart= nFindEnd + sEndTag.GetLength() ;
							}
						}

					} while ( nFindStart>=0 && nFindEnd>=0 ) ;

					if ( arrDeletedKisaNo.GetSize()>0 )
					{
						INT retval= MessageBox(_T("���õ� ��������ȣ�� �׿� ���� �ʷϵ� ��� �������ϴ�. ���� ����ڽ��ϱ�?"), _T("���"), MB_YESNO|MB_ICONWARNING ) ;
						if ( retval!=IDYES )
						{
							return TRUE ;
						}
						else	// ��������ȣ �����
						{
							if ( bIsCut )
								OnTocCut() ;
							else
								TocEditorBackup() ;
							for ( INT i=0 ; i<arrDeletedKisaNo.GetSize() ; i++ )
							{
								if ( DeleteYonKisaNo(arrDeletedKisaNo.GetAt(i)) ) {
									m_sBackupTocEdit.Empty() ;
									m_aBackupChorokStru.RemoveAll() ;
									return TRUE ;			// �������� ���!
								}
							}
							if ( bIsCut )	return TRUE ;
						}
					}

					arrDeletedKisaNo.RemoveAll() ;
					arrDeletedKisaNo.FreeExtra() ;
				}
			}
			else		// ��������ȣ�� �ִ� �ٿ����� ���� �ܿ��� �ٹٲ��� �Ͼ�� ���ϵ��� ����.
			{
//				if ( IsInBody() ) {
				m_strBak= GetKisaNofromMokchaLine(m_icurLine) ;
				if ( IsYonKisaNoRight(m_strBak) )
				{
					m_TocEdit.GetWindowText(m_strBak) ;

					if ( pMsg->wParam==VK_RETURN ) {
						if ( m_strBak.Mid( nStartChar, 2 )!=_T("\r\n") ) {
							MessageBox(_T("��������ȣ�� �ִ� ���ο����� ���� �ٲ� �� �����ϴ�.!"), _T("���"), MB_ICONWARNING) ;
							return TRUE ;
						}
						else	// �� �ڿ� ������ ���������.
							goto CONTINUE ;
					}

					// ��������ȣŰ �±��� �տ��� del Ű�� �����°� �����ϱ� ����
					if ( pMsg->wParam==VK_DELETE ) {
//						if ( m_strBak.Mid( nStartChar, 2 )!=_T("\r\n") ) {
							if ( IsInKisaTag(m_strBak, m_icurLine, nEndChar+1 ) )	// nEndChar ����
							{
								return TRUE ;
							}
//						}
					}
					// ��������ȣ �±� ��ȣ ��ƾ!
					// ��������ȣ�±״� ������ �� ������ ���ش�.
					if ( IsInKisaTag(m_strBak, m_icurLine, nEndChar ) )	// nEndChar ����
					{
						return TRUE ;
					}

				}
//				}


			}

		}

  
CONTINUE:
		// ����Ű
		if( GetAsyncKeyState(VK_CONTROL) ) {
			bIsValidKey = FALSE ;
			switch( pMsg->wParam ) {
			case 78 :	//	Ctrl + 'N'	- New Toc
				break ;
			case 79 :	//	Ctrl + 'O'	- Open
				break ;
			case 83 :	//	Ctrl + 'S'	- Save
				break ;
			case 80 :	//	Ctrl + 'P'	- Print
				break ;
			case 67 :	//	Ctrl + 'C'	- Copy
				OnTocCopy() ;
				return TRUE ;
				break ;
			case 86 :	//	Ctrl + 'V'	- Paste
				OnTocPaste() ;
				return TRUE ;
			case 88 :	//	Ctrl + 'X'	- Cut
				OnTocCut() ;
				return TRUE ;
			case 89 :	//	Ctrl + 'Y'	- Redo
				OnRedo() ;
				return TRUE ;
			case 90 :	//	Ctrl + 'Z'	- Undo
				OnUndo() ;
				return TRUE ;
			case VK_HOME :	//	Ctrl + 'Home'
				m_TocEdit.SendMessage( WM_VSCROLL , SB_TOP , NULL ) ;
				break ;
			case VK_END :	//	Ctrl + 'End'
				m_TocEdit.SendMessage( WM_VSCROLL , SB_BOTTOM , NULL ) ;
				break ;
			case 65 :	//	Ctrl + 'A'	- ��ü����
				break ;
			default :
				return TRUE ;
			}
		}

		if ( pMsg->wParam == VK_F7 ) {	// F7 - ���� üũ !
			OnCheckSyntax();
			return TRUE;
		}
		if ( pMsg->wParam == VK_ESCAPE ) {	// ESC Ű�� ���´�.
			return TRUE;
		}


		if(	bIsValidKey ) {
			// ���� ���ڰ� '>' ���� üũ���ش�.
/*			m_TocEdit.SetSel( nStartChar-1, nStartChar ) ;
			if ( m_TocEdit.GetSelText() == _T(">") )	m_bTagEnd = TRUE ;
			m_TocEdit.SetSel( nStartChar, nEndChar ) ;

			// ������ش�.
			// ���⼭�� ����ü ����� ���ָ� �ȵȴ�.
			m_TocEdit.GetWindowText( m_strBak ) ;
			m_sBackupTocEdit= m_strBak ;				// ���
*/		}

		break ;

	case WM_KEYUP :
		switch( pMsg->wParam ) {
		case VK_DELETE :
		case VK_BACK :
			DisplayCurrentLineNumber() ;	// ���� ǥ��.
			break ;
		case 190 :	// '>' - tag �� ���� �����ֱ� ����
			if( GetAsyncKeyState(VK_SHIFT) ) {
/*				m_TocEdit.GetWindowText( m_strBak ) ;
				m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;
				m_TocEdit.SetWindowText( m_strBak ) ;
				m_TocEdit.SetSel( m_nStartChar, m_nEndChar ) ;
				m_bTagEnd = TRUE ;
*/
			}
			break ;
		case VK_RETURN :	// test
			if ( m_nIsKindOf==_YON_ ) {
				if ( IsInBody() ) {
					TocEditorBackup() ;

//					SetKisaNo2AllKisa();
				}
			}
			break ;
		}

		break ;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CTocEditorDlg::DisplayCurrentLineNumber()
{
	CString sMessage;
	CString sBuffer;
	INT nFirstVisible = m_TocEdit.GetFirstVisibleLine();
	INT cntLine = m_TocEdit.GetLineCount() ;
	for(INT i = nFirstVisible + 1 ; i <= cntLine ; i++)
	{
		sBuffer.Format(_T("%d "),i);
		sMessage += sBuffer + _T("\r\n");
	} 
	m_LineView.SetWindowText(sMessage);

}

VOID CTocEditorDlg::OnSelchangeEditRichedit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SELCHANGE *pSelChange = reinterpret_cast<SELCHANGE *>(pNMHDR);
	// TODO: The control will not send this notification unless you override the
	// CDialog::OnInitDialog() function to send the EM_SETEVENTMASK message
	// to the control with the ENM_SELCHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here

	LONG max = pSelChange->chrg.cpMax ;

	CString strEditText ;
	m_TocEdit.GetWindowText( strEditText ) ;

	if( strEditText.GetLength() < max )	max = strEditText.GetLength() ;

	INT curLine= 1 ;
	for( INT idx=0 ; idx < max ; idx++ )
		if( strEditText.GetAt(idx)=='\n' ) curLine++ ;

	m_icurLine= curLine ;
	INT nVisibleTopLine= m_TocEdit.GetFirstVisibleLine() + 1 ;
	INT nVisibleBottomLine= GetVisibleBottomLine() ;

	INT curPos= m_TocEdit.GetScrollPos(SB_VERT) ;
	if( m_bUpDown ) {
		if( nVisibleTopLine >= curLine ) {
			DisplayCurrentLineNumber() ;
		}
		if( nVisibleTopLine > curLine ) {
			DisplayCurrentLineNumber() ;
			m_bUpDown = FALSE ;
		}
	}
	else if( nVisibleBottomLine < curLine && m_bUpDown ) {
		DisplayCurrentLineNumber() ;
		m_bUpDown = FALSE ;
	}

	// line number ǥ��
	if( m_TocEdit.m_preScrollPos != curPos ) {
		DisplayCurrentLineNumber() ;
		m_TocEdit.m_preScrollPos = curPos ;
	}

	*pResult = 0;
}

INT CTocEditorDlg::GetTotalLine(CString &m_strFile)
{
	INT find = m_strFile.ReverseFind( _T('\n') ) ;
	if ( find==-1 ) return 1 ;
	
	if ( m_strFile.GetAt( find-1 )==_T('\r') )
		m_strFile.TrimRight( _T("\r\n") ) ;
	else 
		m_strFile.TrimRight( _T("\n") ) ;

	INT retval = 1 ;
	for ( INT idx=0 ; idx<m_strFile.GetLength() ; idx++ )
		if( m_strFile.GetAt(idx)=='\n' ) retval++ ;

	return retval ;
}

INT CTocEditorDlg::GetVisibleBottomLine()
{
	CRect rect ;
	m_TocEdit.GetClientRect( &rect ) ;	// m_TocEdit�� ����. 2001.10.9
	INT curPos = m_TocEdit.GetScrollPos(SB_VERT) ;

	return ( rect.bottom + curPos ) / LINE_HEIGHT ;
}

VOID CTocEditorDlg::OnChangeToceditor() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	if( m_bTagEnd ) {		// '>' �ٷ� ���� ��Ʈ�� ������ �ֱ� ����
		m_TocEdit.GetWindowText( m_strBak ) ;
		m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;
		m_TocEdit.SetWindowText( m_strBak ) ;
		m_TocEdit.SetSel( m_nStartChar, m_nEndChar ) ;
		m_bTagEnd = FALSE ;
//		m_TocEdit.DisplayTagColor(TRUE);
	}

	DisplayCurrentLineNumber() ;	// ������ �����ִ� ��ɵ� �����Ѵ�.
}

//DEL INT CTocEditorDlg::SetIsKindOf(INT nKindOf, CString sControlNo/*=_T("")*/)
//DEL {
//DEL 	m_nIsKindOf= nKindOf;
//DEL 
//DEL /*
//DEL 	if ( m_nIsKindOf==_YON_ ) {
//DEL 		if( m_sControlNo.IsEmpty() ) {
//DEL 			AfxMessageBox(_T("�������� ��쿡�� �����ȣ�� ������ �־�� �մϴ�!"), MB_ICONERROR|MB_OK) ;
//DEL 			return -1 ;
//DEL 		}
//DEL 	}
//DEL */
//DEL 	if ( m_TocEdit.m_hWnd!=NULL )
//DEL 		m_TocEdit.SetKindOf(m_nIsKindOf, this) ;
//DEL 
//DEL 	return 0 ;
//DEL }

VOID CTocEditorDlg::SetTocFromString(CString sTocContents)
{
	m_TocEdit.HideSelection(TRUE, TRUE);

	m_sFirstMokcha= sTocContents;
	m_sFirstVisible= sTocContents;

	if( m_TocEdit.m_hWnd!=NULL )
		m_TocEdit.SetWindowText(m_sFirstVisible) ;
	

	// added 2002.11.1
	if ( m_nIsKindOf==_YON_ )
	{
		GetYonAllKisaNo(g_arrAlreadyExistKisaNo);
	}
	m_TocEdit.DisplayTagColor(TRUE);
	m_TocEdit.HideSelection(FALSE, TRUE);
}

INT CTocEditorDlg::SaveFile(CString filename)
{
	CStdioFile file;
	CFileStatus status ;
	CString strError ;
	if( !file.GetStatus( filename , status ) ) {
		strError = _T("\"") + filename + _T("\" �� �������� �ʽ��ϴ�!");
		MessageBox( strError, _T("Error"), MB_OK | MB_ICONERROR );
		return -1;
	}

	if( status.m_attribute == 0x01 ) {
		strError = _T("\"") + filename + _T("\" �� �б� �����̹Ƿ� Write �� �� �����ϴ�!") ;
		MessageBox( strError, _T("Error"), MB_OK | MB_ICONERROR ) ;
		return -2 ;
	}

	CFileException ex;
	if( !file.Open(filename, CFile::modeCreate| CFile::modeWrite | CFile::typeBinary, &ex) ) {

		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		strError += _T("Couldn't open source file:\r\n\r\n");
		strError += szError;
		AfxMessageBox(strError) ;
		return -3;
	}
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	CString strTocText;
	m_TocEdit.GetWindowTextNoMath(strTocText);
//	m_TocEdit.GetWindowText(strTocText);
//	if( strTocText.IsEmpty() ) return -1 ;

	INT idx=0;
	CString linebuf, buffer;
	while ((idx=strTocText.Find(_T("\r\n")))!=-1)
	{
		linebuf= strTocText.Mid(0, idx+1);
		linebuf.TrimRight(_T("\r\n"));
		buffer.Format(_T("%s\r\n"), linebuf);
		file.WriteString(buffer);
		strTocText= strTocText.Mid(idx+1);
	}
	
	if (strTocText!=_T(""))
	{
		linebuf= strTocText;
		linebuf.TrimRight(_T("\r\n"));
		buffer.Format(_T("%s\r\n"), linebuf);
		file.WriteString(buffer);
	}
	file.Close();
	return 0 ;
}

VOID CTocEditorDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == 1 ) {
		KillTimer(1) ;
		CheckSyntax() ;
	}

	CDialog::OnTimer(nIDEvent);
}

//DEL VOID CTocEditorDlg::OnButtonViewer() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	CString sText ;
//DEL 	m_TocEdit.GetWindowText( sText ) ;
//DEL 
//DEL 	CTocViewerDlg dlg ;
//DEL 	dlg.SetTocViewer( sText ) ;
//DEL 	dlg.DoModal() ;
//DEL }

//DEL BOOL CTocEditorDlg::CheckbeforeExit()
//DEL {
//DEL 	CString sCurTocText ;
//DEL 	m_TocEdit.GetWindowText(sCurTocText) ;
//DEL 	if ( m_sFirstVisible!=sCurTocText ) {
//DEL 		INT retval = AfxMessageBox( _T("���� �������� ������ ������� �ݽ��ϱ�?") , MB_YESNO ) ;
//DEL 		switch( retval ) {
//DEL 		case IDNO :
//DEL 			return FALSE ;
//DEL 		case IDYES :
//DEL 			return TRUE ;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	return TRUE ;
//DEL }

INT CTocEditorDlg::MyCreate( CWnd *pParentWnd, CRect rect/*=CRect(0,0,0,0)*/, INT IsKind/*=_DAN_SINGLE*/ )
{
	if (pParentWnd==NULL) {
		AfxMessageBox(_T("���� ���� ȭ���� ���µ� �����߽��ϴ�!")) ;
		return -1;
	}

	m_pParentWnd= pParentWnd ;

	if ( Create(IDD_TOCEDITOR_DIALOG,pParentWnd) != 0 ) {
		if ( ! rect.IsRectEmpty() ) {

			CRect rcLine(0,0,28,0) ;
			rcLine.bottom= rect.bottom- rect.top;		// 13 ��ŭ ������ �ִ�.			

			CRect rcEdit(27,0,0,0) ;
			rcEdit.bottom= rect.bottom - rect.top ;		// 7 ��ŭ ������ �ִ�.
			rcEdit.right= rect.right - rect.left ;			// 7 ��ŭ ������ �ִ�.

			m_nIsKindOf= IsKind ;
//			if ( m_nIsKindOf != _YON_ ) {		// ���๰�϶�
				GetDlgItem(IDC_CHOROK_KEY)->ShowWindow(SW_HIDE) ;
//			}
/*
			else {						// �������϶�

				CRect rcKisaNo ;
				rcKisaNo.top= 7 ;
				rcKisaNo.right= rect.right - rect.left - 7 ;			// 7 ��ŭ ������ �ִ�.
				rcKisaNo.left= rcKisaNo.right - 70 ;				// width �� 40 ���� ����.
				rcKisaNo.bottom= rect.bottom - rect.top - 7 ;		// 7 ��ŭ ������ �ִ�.

				rcEdit.right= rcKisaNo.left - 1;		// ������ 1 �� ��������.

				GetDlgItem(IDC_CHOROK_KEY)->MoveWindow(rcKisaNo) ;
			}
*/
			GetDlgItem(IDC_LINE_VIEW)->MoveWindow(rcLine) ;
			GetDlgItem(IDC_TOCEDITOR)->MoveWindow(rcEdit) ;
			MoveWindow(rect);
		}

		ShowWindow(SW_SHOW);
	}
	else {
		AfxMessageBox(_T("���� ���� ȭ���� ���µ� �����߽��ϴ�!")) ;
		return -2 ;
	}

	return 0 ;
}

// �̹� DB�� �ۼ��Ǿ� �����ϴ� ������ �ʷ��� �������ش�.
INT CTocEditorDlg::SetExistYonChorok(CString sChorokText, CString sKisaNo)
{
	// �������� ��쿡�� ����� �� �ֵ��� �Ѵ�.
	if ( m_nIsKindOf!=_YON_ && m_nIsKindOf!=_KISA_ ) {
		AfxMessageBox(_T("�ʷ� ������ �������� ���� �� �� �ֽ��ϴ�!")) ;
		return -1 ;
	}

	if ( sKisaNo.IsEmpty() ) {
		AfxMessageBox( _T("������ �ʷ� �������� ������ �߻��Ͽ����ϴ�!\r\n��������ȣ�� �����ϴ�.") ) ;
		return -2 ;
	}
/*
	if ( ! IsYonKisaNoFormal(sKisaNo) )
	{
		CString sError ;
		sError= _T("���� ��������ȣ�� ���Ŀ� ���� �ʽ��ϴ�.\r\n") ;
		sError+= sKisaNo ;
		AfxMessageBox(sError) ;
		return -3 ;
	}
*/
	bool bFindinMokcha= TRUE ;
	if ( ! IsThereYonKisaNoinMokcha(m_sFirstMokcha, sKisaNo) )
	{
		bFindinMokcha= FALSE ;
		CString sError ;
		sError= _T("���� ��������ȣ�� ������ �������� �ʽ��ϴ�.\r\n") ;
		sError+= sKisaNo ;
		sError+= _T("\r\n�����Ȱ����� ó���մϴ�.") ;
		AfxMessageBox(sError) ;
//		return -4 ;
	}

	if( ! IsYonKisaNoUnique(sKisaNo) )
	{
		CString sError ;
		sError= _T("������ �ʷ� ���� ����!") ;
		AfxMessageBox(sError) ;
		return -5 ;
	}

	tCHOROK tOneChorok ;
	tOneChorok.sKey= sKisaNo;
	if ( !bFindinMokcha ) {		// ������ ��������ȣ�� ���� ��� ���⼭�� �����Ȱ����� �����صд�.
		tOneChorok.sKisaStatus= _CHOROK_STATUS_DELETE_ ;
		tOneChorok.sChorokText= _T("") ;
		if ( !sChorokText.IsEmpty() )
			tOneChorok.sChorokStatus= _CHOROK_STATUS_DELETE_ ;
		else
			tOneChorok.sChorokStatus= _T("") ;
	} else {
		tOneChorok.sKisaStatus= _T("") ;
		tOneChorok.sChorokText= sChorokText;
		tOneChorok.sChorokStatus= _T("") ;
	}

	m_aChorokStru.Add(tOneChorok);	// �߰����ش�.
	m_aBackupChorokStru.Copy(m_aChorokStru) ;	// ��� �ʱ�ȭ

	if( ! bFindinMokcha )	return -6 ;

	return 0 ;
}

// DB�� ���� ����� ��������ȣ�� �������ش�.
INT CTocEditorDlg::SetWonmunExistKisaNo(CString sKisaNo)
{
	// �������� ��쿡�� ����� �� �ֵ��� �Ѵ�.
	if ( m_nIsKindOf!=_YON_ ) {
		AfxMessageBox(_T("�������� ���� ������ ������������ �� �� �ֽ��ϴ�!")) ;
		return -1 ;
	}

	if ( sKisaNo.IsEmpty() ) {
		AfxMessageBox( _T("���� ����� ��������ȣ �������� ������ �߻��Ͽ����ϴ�!\r\n��������ȣ�� �����ϴ�.") ) ;
		return -2 ;
	}

	bool bFindinMokcha= TRUE ;
	if ( ! IsThereYonKisaNoinMokcha(m_sFirstMokcha, sKisaNo) )
	{
		bFindinMokcha= FALSE ;
		CString sError ;
		sError= _T("���� ��������ȣ�� ������ �������� �ʽ��ϴ�.\r\n") ;
		sError+= sKisaNo ;
		AfxMessageBox(sError) ;
		return -3 ;
	}

	// Array �� �־��ش�.
	m_arrWonmunKisaNo.Add(sKisaNo) ;

	return 0 ;
}

// ���� ������ ���� �� ���� string�� �ް� ���� ��
CString CTocEditorDlg::GetFinalToc()
{
	CString sToc ;
	m_TocEdit.GetWindowText(sToc) ;

	return sToc ;
}

// line number �� �Ѱ��ָ� �� ������ ��������ȣ�� ã�� �Ѱ��ִ� �Լ�
CString CTocEditorDlg::GetKisaNofromMokchaLine(INT iLine)
{
	if( iLine<= 0 ) {
		CString sError ;
		sError.Format( _T("�������� Line ��(%d)�Դϴ�.") , iLine) ;
		AfxMessageBox( sError ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	CString sOneLine ;
	TCHAR szLine[MAX_LINE] ;
	INT iLineLength= m_TocEdit.GetLine( iLine-1, szLine ) ;
	szLine[iLineLength]= '\0' ;
	sOneLine= szLine ;

	INT nFindStart, nFindEnd ;
	nFindStart= nFindEnd= -1 ;
	CString sStartTag ;
	sStartTag= _START_CHOROK_KEY_ ;
	nFindStart= sOneLine.Find(sStartTag) ;
	if ( nFindStart<0 )
	{
		return _T("") ;
	}

	nFindEnd=  sOneLine.Find(_END_CHOROK_KEY_, nFindStart ) ;
	if ( nFindEnd<0 )
	{
		return _T("") ;
	}

	INT nStart= nFindStart+sStartTag.GetLength() ;
	CString sKey ;
	sKey= sOneLine.Mid(nStart, nFindEnd-nStart) ;

	return sKey ;
}

INT CTocEditorDlg::GetCurrentLine()
{
	return m_icurLine ;
}

VOID CTocEditorDlg::ShowViewer(BOOL bPrev/*=FALSE*/, BOOL bNext/*=FALSE*/)
{
	CString sText ;
	m_TocEdit.GetWindowText( sText ) ;

	CTocViewerDlg dlg(this) ;
	if ( m_nIsKindOf==_YON_ )
	{
		dlg.SetTocViewer( sText, TRUE, FALSE ) ;

		// ������ ��ȣ�� �Ѱ��ֱ� ����...
		// View ���̾�αװ� �� �ִ� ������ ������ �����Ǹ� �ȵȴ�.
		INT nSize= m_aChorokStru.GetSize() ;
		CString sKisaTag ;
		CString sFront ;
		for ( INT i=0 ; i<nSize ; i++ )
		{
			tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
			if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_DELETE_ ||		// �ʷ��� �ִ� ����� ���θ� �Ѱ��ֵ��� ����.
				tOneChorok.sChorokStatus==_CHOROK_STATUS_DELETE_ ||
				(tOneChorok.sChorokText).IsEmpty() )
				continue ;

			sKisaTag= _START_CHOROK_KEY_ ;
			sKisaTag+= tOneChorok.sKey ;
			sKisaTag+= _END_CHOROK_KEY_ ;

			INT nFind= sText.Find(sKisaTag) ;
			if ( nFind<0 ) {	//�����߻�
				AfxMessageBox( _T("����±� ") + sKisaTag + _T(" �� ������ �������� �ʽ��ϴ�.")) ;
				continue ;
			}

			sFront= sText.Mid(0, nFind) ;
			INT nLine= sFront.Replace(_T("\r\n"), _T("\r\n"));
			dlg.SetChorokLine(nLine+1) ;
		}
	}
	else		// ���ົ�� ���
		dlg.SetTocViewer( sText ) ;

	dlg.EnablePrevNext(bPrev, bNext) ;
	dlg.DoModal() ;
}

// ���������� �ʷ��� ���Ӱ� �ۼ��Ҷ��� ���ο� ��������ȣ�� ���������־�� �Ѵ�.
CString CTocEditorDlg::GetNewKisaNo()
{
	return _T("");
	TCHAR szRecKey[64] ;
	wmemset( szRecKey, '\0', sizeof(szRecKey) ) ;
	if ( m_pYonKeyFunc==NULL ) {
		AfxMessageBox( _T("Ű ���� �޾ƿ��� �Լ� ������ ���� �ʾҽ��ϴ�."), MB_ICONERROR ) ;
		return _CHOROK_ERROR_STRING_ ;
	}
	else if ( (m_pParentWnd->*m_pYonKeyFunc)(szRecKey)<0 ) {
		AfxMessageBox( _T("Ű ���� �ο��޴µ� �����߽��ϴ�!\r\nMakePrimaryKey �Լ� ����!"), MB_ICONERROR ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	CString sKisaNo ;
	sKisaNo= szRecKey ;

	return sKisaNo ;

}

CString CTocEditorDlg::GetYonChorok(INT iLine)
{
	if ( m_nIsKindOf!=_YON_ ) {	// ���๰�϶��� ������ �����Ѵ�.
		AfxMessageBox( _T("���� ������ ���๰�� �����Ǿ��ֽ��ϴ�.\r\n������ Ȯ���� �ֽʽÿ�.") ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	CString sKisaNo= GetKisaNofromMokchaLine(iLine) ;

	if ( sKisaNo==_CHOROK_ERROR_STRING_ ) {
		AfxMessageBox( _T("�ʷ� �������� ����!"), MB_ICONERROR ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	if ( sKisaNo.IsEmpty() )
	{

		return _T("") ;		// �ʷ��� ���� ������ Empty�� �������ش�.

	}
	else	// ���� �ʷ��� �����ϴ� ���
	{
		for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
			tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
			if( sKisaNo==tOneChorok.sKey ) {
				return tOneChorok.sChorokText ;
			}
		}

//		CString sError ;
//		sError=	_T("��������ȣ ") + sKisaNo + _T(" �� ����� �ʷ��� �����ϴ�.") ;
//		AfxMessageBox( _T("�ʷ� �������� ����!"), MB_ICONERROR ) ;
//		return _CHOROK_ERROR_STRING_ ;
		return _T("") ;		// ��ó�� ������ �ҷ������� ��������ȣ�� �ִµ� �ʷ��� ���� ����̴�.
	}
}

CString CTocEditorDlg::GetYonKisaNo(INT iLine, bool bNew/*=TRUE*/)
{
	if ( m_nIsKindOf!=_YON_ ) {	// ���๰�϶��� ������ �����Ѵ�.
		AfxMessageBox( _T("���� ������ ���๰�� �����Ǿ��ֽ��ϴ�.\r\n������ Ȯ���� �ֽʽÿ�.") ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	CString sKisaNo= GetKisaNofromMokchaLine(iLine) ;

	if ( sKisaNo==_CHOROK_ERROR_STRING_ ) {	// ����!
		AfxMessageBox( _T("��������ȣ �������� ����!"), MB_ICONERROR ) ;
		return _CHOROK_ERROR_STRING_ ;
	}
	else if ( sKisaNo.IsEmpty() ) {		// ��������ȣ�� ���� ����
		if ( bNew ) {
			CString sNewKey= GetNewKisaNo() ;
			if( sNewKey==_CHOROK_ERROR_STRING_ ) {
				AfxMessageBox(_T("��������ȣ �������� ����!"), MB_ICONERROR );
				return _CHOROK_ERROR_STRING_;
			}
			else	return sNewKey ;	// ���ο� ��������ȣ�� ���ͼ� �Ѱ��ش�.
		}
		else {
			return sKisaNo ;		// Empty ���� �Ѱ�����.
		}
	}
	else	return sKisaNo ;		// ��������ȣ�� ã�Ҵ�!

}

INT CTocEditorDlg::ReflectYonChorok2Stru(CString sChorokText, CString sKisaNo)
{
/*
	if ( ! IsYonKisaNoFormal(sKisaNo) ) {
		CString sError ;
		sError= _T("���� ��������ȣ�� ������ �ʷϿ� �ݿ���Ű�� ���߽��ϴ�!\r\n") ;
		sError+= sKisaNo ;
		AfxMessageBox( sError ) ;
		return -1 ;
	}
*/
	BOOL bFindKey= FALSE ;
	for ( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;

		if ( sKisaNo==tOneChorok.sKey )	{
			bFindKey= TRUE ;

			tCHOROK tNewChorok ;
			tNewChorok.sKey= sKisaNo ;
			tNewChorok.sKisaStatus= tOneChorok.sKisaStatus ;
			tNewChorok.sChorokText= sChorokText ;

			if ( sChorokText.IsEmpty() ) {
				if ( ! (tOneChorok.sChorokText).IsEmpty() && tOneChorok.sChorokStatus!=_CHOROK_STATUS_NEW_ )
					tNewChorok.sChorokStatus=_CHOROK_STATUS_DELETE_ ;
			} else {
				if ( tOneChorok.sChorokText==sChorokText )
					tNewChorok.sChorokStatus=_T("") ;
				else {
					if ( tOneChorok.sChorokStatus==_T("") ) {
						if ( (tOneChorok.sChorokText).IsEmpty() )
							tNewChorok.sChorokStatus=_CHOROK_STATUS_NEW_ ;
						else
							tNewChorok.sChorokStatus=_CHOROK_STATUS_MODIFY_ ;
					}
					else if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_NEW_ )
						tNewChorok.sChorokStatus=_CHOROK_STATUS_NEW_ ;
					else if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_MODIFY_ )
						tNewChorok.sChorokStatus=_CHOROK_STATUS_MODIFY_ ;
					else if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_DELETE_ )
						tNewChorok.sChorokStatus=_CHOROK_STATUS_MODIFY_ ;
				}
			}

			m_aChorokStru.RemoveAt(i) ;		// �켱 �����
			m_aChorokStru.InsertAt(i, tNewChorok) ;		// �� ��ġ�� ���Ӱ� �־��ش�.
			break ;
		}

	}

	if( ! bFindKey ) {	// ��������ȣ�� �����ۼ���
		if( ! IsYonKisaNoUnique(sKisaNo) ) {
			CString sError ;
			sError= _T("���� ��������ȣ�� ������ �ʷϿ� �ݿ���Ű�� ���߽��ϴ�.\r\n") ;
			sError+= sKisaNo ;
			AfxMessageBox( sError ) ;
			return -2 ;
		}

		tCHOROK tOneChorok ;
		tOneChorok.sKey= sKisaNo;
		tOneChorok.sKisaStatus= _CHOROK_STATUS_NEW_ ;		// ����Ʈ - �����ۼ� �÷���
		// added 2002.11.1 - ��翡
		INT nSize= g_arrAlreadyExistKisaNo.GetSize();
		for( INT idx=0 ; idx<nSize ; idx++ )
		{
			if ( sKisaNo==g_arrAlreadyExistKisaNo.GetAt(idx) )
			{
				(tOneChorok.sKisaStatus).Empty();		// ó���� �����ϴ� ���� �����ۼ��� ���� �ƴ�
				break;
			}
		}
		tOneChorok.sChorokText= sChorokText;

		if ( sChorokText.IsEmpty() )
			tOneChorok.sChorokStatus= _T("") ;
		else
			tOneChorok.sChorokStatus= _CHOROK_STATUS_NEW_ ;

		m_aChorokStru.Add(tOneChorok);	// �߰����ش�.
	}

	return 0 ;
}

CString CTocEditorDlg::WriteYonKisaNo2Mokcha(INT nLine, CString sKisaNo, CString sMokcha)
{
	INT nTotalLine= sMokcha.Replace( _T("\r\n"), _T("\r\n") ) ;
	nTotalLine++ ;

	if ( nTotalLine<nLine ) {
		AfxMessageBox( _T("Line ������ �߸��Ǿ����ϴ�.\r\n������ ������ ���ο� ��������ȣ�� �־����� ���߽��ϴ�.") ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	CString sKisaNoinMokcha ;
	sKisaNoinMokcha= _START_CHOROK_KEY_ + sKisaNo + _END_CHOROK_KEY_ ;	// ������ �� ��������ȣ �±� ����

	INT nStart= 0 ;
	INT nLineCnt= 0 ;
	BOOL bSuccess= FALSE ;
	for (INT i=0 ; i<nTotalLine ; i++) {
		nStart= sMokcha.Find(_T("\r\n"), nStart) ;
		nLineCnt++ ;
		if( nLineCnt == nLine ) {
			bSuccess=TRUE ;
			if ( nStart<0 )	sMokcha+= sKisaNoinMokcha ;
			else			sMokcha.Insert(nStart, sKisaNoinMokcha) ;
			break ;
		}
		nStart= nStart+2 ;
	}

	if( !bSuccess ) { 
		CString sError ;
		sError.Format( _T("������ ������ %d ���ο� ��������ȣ %s �� �־����� ���߽��ϴ�!") , nLine, sKisaNo) ;
		AfxMessageBox( sError ) ;
		return _CHOROK_ERROR_STRING_ ;
	}
	else 
		return sMokcha ;
}

INT CTocEditorDlg::WriteYonNewKisaNo2Mokcha(INT nLine, CString sKisaNo)
{
	CString sMokcha ;
	m_TocEdit.GetWindowText(sMokcha) ;
	INT nTotalLine= sMokcha.Replace( _T("\r\n"), _T("\r\n") ) ;
	nTotalLine++ ;

	if ( nTotalLine<nLine ) {
		AfxMessageBox( _T("Line ������ �߸��Ǿ����ϴ�.\r\n������ ���ο� ��������ȣ�� �־����� ���߽��ϴ�.") ) ;
		return -1 ;
	}

	sKisaNo.TrimRight() ;
	sKisaNo.TrimLeft() ;

	CString sKey ;
	sKey= _START_CHOROK_KEY_ + sKisaNo + _END_CHOROK_KEY_ ;

	INT nStart= 0 ;
	INT nPrev= 0 ;
	INT nLineCnt= 0 ;
	BOOL bSuccess= FALSE ;
	for (INT i=0 ; i<nTotalLine ; i++) {
		nPrev= nStart ;
		nStart= sMokcha.Find(_T("\r\n"), nStart) ;
		nLineCnt++ ;
		if( nLineCnt == nLine ) {
			bSuccess= TRUE ;
			if ( nStart<0 ) sMokcha+= sKey ;		// ������ ����
			else			sMokcha.Insert(nStart, sKey) ;
			break ;
		}
		nStart= nStart+2 ;
	}

	if( !bSuccess ) { 
		CString sError ;
		sError.Format( _T("������ ������ %d ���ο� ��������ȣ %s �� �־����� ���߽��ϴ�!") , nLine, sKisaNo) ;
		AfxMessageBox( sError ) ;
		return -3 ;
	}

	m_TocEdit.GetSel(m_nStartChar, m_nEndChar);
	m_TocEdit.SetWindowText(sMokcha) ;
	m_TocEdit.SetSel(m_nStartChar, m_nEndChar);

	return 0 ;
}

INT CTocEditorDlg::DeleteYonKisaNo(CString sKisaNo)
{
	if ( m_nIsKindOf!=_YON_ ) {	// ���๰�϶��� ������ �����Ѵ�.
		AfxMessageBox( _T("���� ������ ���๰�� �����Ǿ��ֽ��ϴ�.\r\n������ Ȯ���� �ֽʽÿ�.") ) ;
		return -1 ;
	}

	INT retval = 0 ;
	// �켱 ����ü���� �ʷ������� ��������.
	BOOL bFindKey= FALSE ;
	for ( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if( sKisaNo==tOneChorok.sKey )	{
			bFindKey= TRUE ;

			if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_NEW_ ) {	// ���� �ۼ��ϰ� ����� ����϶��� �׳� ����⸸ ���ش�.
				m_aChorokStru.RemoveAt(i) ;		// ����⸸ �ϰ� insert �� ���� �ʴ´�.
			} else if ( tOneChorok.sKisaStatus==_T("") ) {
				tCHOROK tNewChorok ;
				tNewChorok.sKey= sKisaNo ;
				tNewChorok.sChorokText= _T("") ;
				tNewChorok.sKisaStatus= _CHOROK_STATUS_DELETE_ ;
				if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_DELETE_ ) {
					tNewChorok.sChorokStatus=_CHOROK_STATUS_DELETE_ ;
				} else {
					if ( ! (tOneChorok.sChorokText).IsEmpty() )
					{
						if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_NEW_ )
							(tNewChorok.sChorokStatus).Empty() ;
						else
							tNewChorok.sChorokStatus=_CHOROK_STATUS_DELETE_ ;
					}
				}

				m_aChorokStru.RemoveAt(i) ;		// �켱 �����
				m_aChorokStru.InsertAt(i, tNewChorok) ;

			} else if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_DELETE_ ) {	// �̹� �����Ǿ� �ִ� ���
				CString sError ;
				sError= _T("��������ȣ ") + sKisaNo + _T(" �� �̹� �����Ǿ� �ֽ��ϴ�!") ;
				AfxMessageBox(sError) ;
			}

			break ;
		}
	}

	if ( ! bFindKey ) {
		// ��������ȣ�� ����ü�� ����.
		CString sError ;
		sError= _T("��������ȣ ") + sKisaNo + _T(" �� �������� �ʽ��ϴ�.") ;
		AfxMessageBox(sError) ;
		retval= -1 ;
	}

	return retval ;
}

VOID CTocEditorDlg::Clear()
{
	m_sBackupTocEdit.Empty() ;
	m_sRedoTocEdit.Empty() ;

	m_aChorokStru.RemoveAll() ;
	m_aBackupChorokStru.RemoveAll() ;
	m_aRedoChorokStru.RemoveAll() ;

	if ( m_TocEdit.m_hWnd != NULL )
	{
		m_icurLine = 1 ;
		m_LineView.SetWindowText(_T("1"));

		// m_TocEdit �� ��� ������ �����ش�.
		m_TocEdit.SetSel(0,-1);
		m_TocEdit.Clear() ;
	}

}

// Ű�� ���Ŀ� �´��� üũ!		-> �����ȣ_ID
//DEL BOOL CTocEditorDlg::IsYonKisaNoFormal(CString sKisaNo)
//DEL {
//DEL /*
//DEL 	if ( m_sControlNo.IsEmpty() ) {
//DEL 		AfxMessageBox(_T("�����ȣ�� �����Ǿ� ���� �ʽ��ϴ�!"), MB_ICONERROR|MB_OK) ;
//DEL 		return FALSE ;
//DEL 	}
//DEL */
//DEL 	TCHAR szKisaNo[64] ;
//DEL 	_tcscpy( szKisaNo, sKisaNo ) ;
//DEL 	TCHAR seps[] = _T("_");
//DEL 	TCHAR *token= NULL;
//DEL 	INT nCnt=0 ;
//DEL 
//DEL 	token = _tcstok( szKisaNo, seps );
//DEL 	while( token != NULL )
//DEL 	{
//DEL 		nCnt++ ;
//DEL 
//DEL 		/* While there are tokens in _T("string") */
//DEL 		if ( (nCnt>2) || ( nCnt==1 && m_sControlNo.Compare(token))) {
//DEL 			CString sError ;
//DEL 			sError= _T("���Ŀ� ���� �ʴ� Ű�� ���Ǿ����ϴ�.\r\n") ;
//DEL 			sError+= sKisaNo ;
//DEL 			AfxMessageBox( sError ) ;
//DEL 			return FALSE ;
//DEL 		}
//DEL 
//DEL 		/* Get next token: */
//DEL 		token = _tcstok( NULL, seps );
//DEL 	}
//DEL 
//DEL 	return TRUE ;
//DEL }

// Ű�� �������� �����ϴ��� ���� üũ
BOOL CTocEditorDlg::IsThereYonKisaNoinMokcha(CString sMokcha, CString sKisaNo)
{
	CString sKey ;
	sKey= _START_CHOROK_KEY_ + sKisaNo + _END_CHOROK_KEY_ ;

	INT nCnt= sMokcha.Replace( sKey, sKey ) ;

	if ( nCnt==0 ) return FALSE ;

	if ( nCnt > 1 ) {		// �� ���̻� ����� ���
		CString sMsg ;
		sMsg= _T("��������ȣ ") + sKisaNo + _T(" �� ������ �ΰ� �̻� �����մϴ�.") ;
		sMsg+= _T("\r\n���� ��������ȣ�� ��ȿ�մϴ�.") ;
		AfxMessageBox( sMsg ) ;
	}

	return TRUE ;
}

// Ű�� ����ũ ���� üũ!
BOOL CTocEditorDlg::IsYonKisaNoUnique(CString sKisaNo)
{
	return TRUE;
	if( sKisaNo.IsEmpty() )
		return FALSE ;

	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if( sKisaNo==tOneChorok.sKey ) {
			CString sError ;
			sError= _T("���� ��������ȣ�� Unique���� �ʽ��ϴ�.\r\n") ;
			sError+= sKisaNo ;
			AfxMessageBox(sError) ;
			return FALSE ;
		}
	}

	return TRUE ;
}

// Ű�� �����ۼ��� Ű���� Ȯ�����ش�.
BOOL CTocEditorDlg::IsYonKisaNoRight(CString sKisaNo)
{
	if( sKisaNo.IsEmpty() )
		return FALSE ;

	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if( sKisaNo==tOneChorok.sKey && tOneChorok.sKisaStatus!=_CHOROK_STATUS_DELETE_) {
			return TRUE ;		// ����ü�� �����ϴ� Ű
		}
	}

	return FALSE ;
}

CString CTocEditorDlg::GetOneLineString(CString sMokcha, INT nLine)
{
	CString sOneLine ;

	TCHAR string[MAX_LINE] ;
	_tcscpy( string, sMokcha ) ;
	TCHAR seps[] = _T("\r\n");
	INT nCnt= 1 ;
	TCHAR *token= NULL;

	token = _tcstok( string, seps );
	while( token != NULL )
	{
		if( nCnt == nLine ) {
			sOneLine= token ;
			return sOneLine ;
		}

		/* Get next token: */
		token = _tcstok( NULL, seps );
		nCnt++ ;
	}

	return _T("") ;
}

INT CTocEditorDlg::GetOneLineString(INT nLine, CString& sOneLine)
{
	sOneLine= _T("") ;

	CString sMokcha ;
	m_TocEdit.GetWindowText(sMokcha) ;

	INT nTotalLine=	sMokcha.Replace( _T("\r\n") , _T("\r\n") ) ;
	nTotalLine++ ;

	if ( nTotalLine < nLine ) {
		AfxMessageBox( _T("���� ����!"), MB_ICONERROR ) ;
		return -1 ;
	}

	INT nStart=0 ;
	INT nFind=-1 ;
	for ( INT idx=1 ; idx<nLine ; idx++ )
	{
		nFind= sMokcha.Find( _T("\r\n") , nStart ) ;
		if ( nFind<0 ) {
			AfxMessageBox( _T("���� ����!"), MB_ICONERROR ) ;
			return -2 ;
		}
		nStart= nFind + 2 ;
	}

	if ( nTotalLine==nLine )		// ������ ����
	{
		sOneLine= sMokcha.Mid(nStart) ;
		return 0 ;
	}

	nFind= sMokcha.Find( _T("\r\n") , nStart ) ;
	if ( nFind<0 ) {
		AfxMessageBox( _T("���� ����!"), MB_ICONERROR ) ;
		return -3 ;
	}

	sOneLine= sMokcha.Mid(nStart, nFind-nStart) ;
	return 0 ;
}

// ������ ���� ��������ȣ���� �� �پ� �ִ��� ����ü�� �˻��� �ش�.
// �˻� ������ ����ü�̴�.
// ��, �������϶��� ����Ѵ�.
INT CTocEditorDlg::CheckYonKisaNo(CString sFinalMokcha)
{
	INT nCnt = 0 ;		// �������� ��������ȣ�� ���� ����
	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		CString sKisaNo ;
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		sKisaNo= _START_CHOROK_KEY_ ;
		sKisaNo+= tOneChorok.sKey ;
		sKisaNo+=_END_CHOROK_KEY_ ;		// ��������ȣ ����!

		if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_DELETE_ ) {
			if ( sFinalMokcha.Find( sKisaNo )>=0 ) {
				nCnt++ ;
				CString sError ;
				sError = _T("�������� ��������ȣ ") + sKisaNo + _T(" �� �̹� ������ ��������ȣ�Դϴ�!") ;
				AfxMessageBox( sError, MB_ICONERROR ) ;
			}
			else
				continue ;
		}

		if ( sFinalMokcha.Find( sKisaNo )<0 ) {
			nCnt++ ;
			CString sError ;
			sError = _T("�������� ��������ȣ�� ���� ���� ") + sKisaNo + _T(" �� �����ϴ�.\r\nȮ���Ͽ� �ֽʽÿ�!") ;
			AfxMessageBox( sError, MB_ICONERROR ) ;
		}
	}

	return nCnt ;
}

INT CTocEditorDlg::GetKindofMokcha()
{
	return m_nIsKindOf ;
}

VOID CTocEditorDlg::GetResultYonChorokSet(CYonChorokSet& rs)
{
	rs.Free() ;
	CString sKisa, sKisaStatus ;

	// ��縦 ��� �־��ش�. �ƹ��� �÷��� ����. added 2002.10.31
	// ���� ������ �����ϴ� ��������ȣ ������ŭ ����� �־��ְԵȴ�.
	CStringArray sArrKisaNo;
	CString sChorok, sKisaNo;	sKisaNo.Empty();
	BOOL bSkip= FALSE;	BOOL bExist= FALSE;
	if ( GetYonAllKisaNo(sArrKisaNo) )
	{
		return ;
	}
	INT nSize= g_arrAlreadyExistKisaNo.GetSize();
	INT nCurrentKisaCnt= sArrKisaNo.GetSize();
	for ( INT idx=0 ; idx<nSize ; idx++ )
	{
		sKisaNo= g_arrAlreadyExistKisaNo.GetAt(idx);

		bSkip= FALSE;	sChorok.Empty();	sKisaStatus.Empty();
		for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
			tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
			if( sKisaNo==tOneChorok.sKey ) {
				// �����÷��� ���� empty �� ��츸 �־��ش�.
				// ��, ���� ��������ȣ�� �ι� ���� �ʱ� �����̴�.
				// �۾��� �̷���� �͵��� ���� for ������ �־��ֱ� �����̴�.
				if ( ! ( (tOneChorok.sChorokStatus).IsEmpty() && (tOneChorok.sKisaStatus).IsEmpty() ) )
				{
					bSkip= TRUE;
				}
				sChorok= tOneChorok.sChorokText ;
				break;
			}
		}
		bExist= FALSE;	sKisaStatus.Empty();	sKisa.Empty();
		for ( INT j=0 ; j<nCurrentKisaCnt ; j++ )	// ��� delete �߰�
		{
			if( sKisaNo== sArrKisaNo.GetAt(j) )
			{
				bExist= TRUE;
			}
		}
		if ( !bExist )	sKisaStatus= _CHOROK_STATUS_DELETE_;
		else			sKisa= GetKisafromKisaNo( sKisaNo ) ;

		if ( !bSkip )	// ���°��� �ִ°͵��� �Ѿ��.
			rs.SetChorok(sKisaNo , sChorok, _T(""), sKisaStatus, sKisa ) ;

	}
	sArrKisaNo.RemoveAll();	sArrKisaNo.FreeExtra();



	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		// ������ �־��ذ͵�(flag �� ���°�)�� ���⼭ �־��ָ� �ȵȴ�.
		// added 2002.11.1
		if ( (tOneChorok.sChorokStatus).IsEmpty() && (tOneChorok.sKisaStatus).IsEmpty() )	continue;
		if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_DELETE_ )	{	// DELETE �� KEY
			rs.SetChorok( tOneChorok.sKey, tOneChorok.sChorokText, tOneChorok.sChorokStatus, tOneChorok.sKisaStatus, _T("") ) ;
		}
		else {
			sKisa= GetKisafromKisaNo( tOneChorok.sKey ) ;
			rs.SetChorok( tOneChorok.sKey, tOneChorok.sChorokText, tOneChorok.sChorokStatus, tOneChorok.sKisaStatus, sKisa ) ;
		}
	}

}

CString CTocEditorDlg::GetKisafromKisaNo(CString sKisaNo)
{
	CString sMokcha ;
	m_TocEdit.GetWindowText(sMokcha) ;

	INT nFindKey= sMokcha.Find(sKisaNo) ;
	if ( nFindKey<0 )
	{
		CString sError ;
		sError= _T("��������ȣ ") + sKisaNo + _T(" �� ������ �������� �ʽ��ϴ�!") ;
		MessageBox( sError, _T("��� ��������"), MB_ICONERROR | MB_OK ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	INT curLine = 1;
	for ( INT idx=0 ; idx<nFindKey ; idx++ )
	{
		if( sMokcha.GetAt(idx)=='\n' ) curLine++ ;
	}

	CString sOneLine ;
	TCHAR szLine[MAX_LINE] ;
	INT iLineLength = m_TocEdit.GetLine( curLine-1, szLine ) ;
	sOneLine= szLine ;

	if ( sOneLine.Find(sKisaNo)<0 )
	{
		CString sError ;
		sError= _T("��������ȣ ") + sKisaNo + _T(" �� ������ �������� �ʽ��ϴ�!") ;
		MessageBox( sError, _T("��� ��������"), MB_ICONERROR | MB_OK ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	INT nFindStart= sOneLine.Find(_START_CHOROK_KEY_) ;
	if ( nFindStart<0 )
	{
		CString sError ;
		sError= _T("��������ȣ ") + sKisaNo + _T(" �� ������ �������� �ʽ��ϴ�!") ;
		MessageBox( sError, _T("��� ��������"), MB_ICONERROR | MB_OK ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	CString sKisa ;
	sKisa= sOneLine.Mid(0,nFindStart) ;

	return sKisa ;
}

// ��ġ����Ʈ���� ������ ��ư �˾��޴��� �ʷ� ����(���ົ)
VOID CTocEditorDlg::OnEditDanChorok()
{
	if ( m_pParentWnd==NULL ) return ;
	m_pParentWnd->SendMessage(MSG_EDIT_DAN_CHOROK) ;
}

// ��ġ����Ʈ���� ������ ��ư �˾��޴��� �ʷ� ����(������)
VOID CTocEditorDlg::OnEditYonChorok()
{
//	if ( m_pParentWnd==NULL ) return ;

	INT nLine= GetCurrentLine() ;

	CString sOldChorokText, sNewChorokText, sKisaNo ;
	sKisaNo= GetYonKisaNo(nLine, false) ;
	if ( sKisaNo.IsEmpty() || sKisaNo==_CHOROK_ERROR_STRING_ )
		return ;

	sOldChorokText= GetYonChorok(nLine) ;
	if ( sOldChorokText==_CHOROK_ERROR_STRING_ )
		return ;
		
	CChorokEditorDlg dlg(sOldChorokText, sKisaNo, GetKisafromKisaNo(sKisaNo), nLine, this) ;
	if ( IDCANCEL==dlg.DoModal() )
		return ;

	// ����, ������ �������� ��� ��������ȣ�� ��������־�� ��.
	// modified 2002.11.1
	sKisaNo= GetYonKisaNo(m_icurLine, false) ;
	if ( sKisaNo.IsEmpty() || sKisaNo==_CHOROK_ERROR_STRING_ )
		return ;

	// �ʷ� ������ ������ �� �ʷ� ������ �޾ƿ´�.
	sNewChorokText= dlg.GetChorokText()  ;

	if ( sNewChorokText==sOldChorokText ) return ;

	// �ʷ��� �ݿ������ش�. ����
	TocEditorBackup() ;

	if ( ReflectYonChorok2Stru(sNewChorokText, sKisaNo) ) {
		AfxMessageBox( _T("���� �߻�!") );
		return ;
	}
}
/*
INT CTocEditorDlg::ShowYonMokcha(CString sYonMokcha)
{
	if ( m_nIsKindOf!=_YON_ || m_TocEdit.m_hWnd==NULL ) return -1 ;

	m_TocEdit.SetWindowText(sYonMokcha) ;

	return 0 ;
}
*/
INT CTocEditorDlg::KisaNoDeleteMessage(CString sID)
{
	if ( sID.IsEmpty() )	return IDYES ;

	CString sKisaNo ;
	sKisaNo= sID ;

	CString sMsg ;
	sMsg= _T("��������ȣ ") + sKisaNo + _T(" �� ����ڽ��ϱ�?\r\n���� ����ٸ� �ʷ��� �ٽ� �����Ҽ� �����ϴ�.") ;
	INT retval= AfxMessageBox( sMsg, MB_YESNO, MB_ICONWARNING );

	if ( retval==IDYES )
	{	// ���⼭ ����ü�� ����Ǿ� �ִ� Ű ���������� 'D' �� setting �� ����.
		TocEditorBackup() ;

		INT ret= DeleteYonKisaNo(sKisaNo) ;
		if ( ret<0 ) {
			sMsg= _T("��������ȣ ") + sKisaNo + _T(" �� ������ ���߽��ϴ�!") ;
			AfxMessageBox(sMsg) ;
			return IDNO ;
		}
	}
	else
	{
		return IDNO ;
	}

	return IDYES ;
}

VOID CTocEditorDlg::TocNewEdit()
{
//	if(m_nIsKindOf==_YON_ )
//		TocNewEdit(csId, csKey, csSertitle, csTitle, csIssue, csDate, csCall, csCount);
//	else
//		TocNewEdit(csId, csCall, csCount, csDate, csPub, csSertitle, csTitle, _T(""));
	TocNewEdit(csstrTag);
}

VOID CTocEditorDlg::TocNewEdit(CString *csstrTag)
{

	CString sMokcha ;
	m_TocEdit.GetWindowText( sMokcha ) ;

	if ( ! sMokcha.IsEmpty() ) {
		INT retval = -1 ;
		if( retval = AfxMessageBox( _T("���� �������� ������ ������� �ݰڽ��ϴ�.") , MB_YESNO ) ) {
			switch( retval ) {
			case IDYES :
				break ;
			case IDNO :
				return ;
			}
		}
	}

	// ���� �ʱ�ȭ.
	m_sBackupTocEdit.Empty() ;
	m_sRedoTocEdit.Empty() ;
	m_aBackupChorokStru.RemoveAll() ;
	m_aRedoChorokStru.RemoveAll() ;
	if ( m_TocEdit.m_hWnd != NULL )
	{
		m_icurLine = 1 ;
		m_LineView.SetWindowText(_T("1"));

		// m_TocEdit �� ��� ������ �����ش�.
		m_TocEdit.SetSel(0,-1);
		m_TocEdit.Clear() ;
	}
	// ��� ��縦 delete �÷��׷� ������.
	// DeleteYonKisaNo �Լ��� ��쿡 ���� �ʷ� �÷��׵� �ڵ������� ������Ʈ�����ش�.
	for ( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if ( tOneChorok.sKisaStatus!=_CHOROK_STATUS_DELETE_ )
			DeleteYonKisaNo(tOneChorok.sKey) ;
	}


	m_LineView.SetWindowText(_T("1"));	// ���� ǥ�� - 1
	m_SyntaxCheckDlg.DeleteAllItems() ;
	m_SyntaxCheckDlg.ShowWindow(SW_HIDE) ;	// �������� ���̾�α� �����

	m_sBackupTocEdit.Empty() ;
	m_sRedoTocEdit.Empty() ;
	m_aRedoChorokStru.RemoveAll() ;

	CString sTocEditText ;
	CBibilographyDlg dlg ;
	dlg.m_nIsKind = m_nIsKindOf;

	dlg.m_str1 = csstrTag[0];
	dlg.m_str2 = csstrTag[1];
	dlg.m_str3 = csstrTag[2];
	dlg.m_str4 = csstrTag[3];
	dlg.m_str5 = csstrTag[4];
	dlg.m_str6 = csstrTag[5];
	dlg.m_str7 = csstrTag[6];
	dlg.m_str8 = csstrTag[7];

	if( dlg.DoModal() != IDOK ) return ;
	sTocEditText = dlg.m_strBibilography ;

/*
	if ( m_nIsKindOf==_YON_ )
	{
		CYonSujiDlg dlg ;
		if( dlg.DoModal() != IDOK ) return ;
		sTocEditText = dlg.m_strBibilography ;
	}
	else 
	{
		CBibilographyDlg dlg ;
		if( dlg.DoModal() != IDOK ) return ;
		sTocEditText = dlg.m_strBibilography ;
	}
*/
	// ��� ���ش�.
	TocEditorBackup() ;

	m_TocEdit.SetWindowText(sTocEditText) ;

	DisplayCurrentLineNumber() ;

	// ���� ���̾�α�
	CBodyDlg Bodydlg ;
	if( Bodydlg.DoModal() != IDOK ) return ;

	sTocEditText += Bodydlg.m_strResult ;
	sTocEditText.TrimRight( _T("\r\n") ) ;
	m_TocEdit.SetWindowText(sTocEditText);

	INT nLineIdx= 0 ;
/*	if ( m_nIsKindOf==_YON_ )
	{
		nLineIdx= m_TocEdit.LineIndex(9);

		SetKisaNo2AllKisa() ;		// body �� ������� ���������ȣ�� �־�����.
	}
	else
	{
		nLineIdx= m_TocEdit.LineIndex(8);
	}
*/
	m_TocEdit.SetSel(nLineIdx,nLineIdx) ;	// body ����κп� Ŀ���� �����ֱ� ����
	m_TocEdit.SetFocus() ;

	DisplayCurrentLineNumber() ;
}

INT CTocEditorDlg::ExecuteSoosikEditor()
{
	m_cSoosikEditor.RemoveSharedMemoryPool() ;

	INT retval= m_cSoosikEditor.ExecuteSoosikEditor(m_hWnd) ;	// �ڵ��� �Ѱ�����.

	return retval ;
}

INT CTocEditorDlg::ReadFromSoosikEditor()
{
	CString sRead ;
	sRead= m_cSoosikEditor.ReadFromSoosikEditor() ;

	if ( sRead.IsEmpty() )
		return -1 ;

	m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;
	CString sMokcha ;
	m_TocEdit.GetWindowText(sMokcha) ;
	TocEditorBackup() ;

	sMokcha.Insert( m_nStartChar, sRead ) ;

	m_TocEdit.SetWindowText(sMokcha) ;

	INT nLength= sRead.GetLength() ;
	m_TocEdit.SetSel( m_nStartChar+nLength , m_nStartChar+nLength ) ;

	return 0 ;
}

VOID CTocEditorDlg::OnReadEcoMath()
{
	ReadFromSoosikEditor() ;
}

INT CTocEditorDlg::SetKisaNo2AllKisa(BOOL bCheckSyntax/*=FALSE*/)
{
	if ( bCheckSyntax ) {
		if ( CheckSyntax()<0 )
		{
			AfxMessageBox( _T("������ ������ �ֽ��ϴ�.\r\n���� �������� ������ �ٷ�����ֽʽÿ�."), MB_ICONWARNING) ;
			return 0 ;
		}
	}

	CString sMokcha ;
	m_TocEdit.GetWindowText(sMokcha) ;
	CString sKisaNo ;
	CString sOneKisa ;

	INT nStart= 0 ;
	INT nFindStart= -1 ;
	INT nFindEnd= -1 ;
	INT nStartLine= 1 ;

	do {
		nFindStart= sMokcha.Find( BODY_START_TAG , nStart ) ;
		if ( nFindStart>=0 )
		{
			nStart= nFindStart + BODY_START_LENGTH ;
			nFindEnd= sMokcha.Find( BODY_END_TAG , nStart ) ;
			if ( nFindEnd>=0 )		// <body>�� </body>�� ã�Ҵ�.
			{
				INT nFindReturn= sMokcha.Find( _T("\r\n") , nFindStart ) ;
				if ( nFindReturn>=0 )
				{
					CString sTmp ;
					sTmp= sMokcha.Mid(0, nFindReturn+2 ) ;
					nStartLine= sTmp.Replace( _T("\r\n") , _T("\r\n") ) ;
					nStartLine++ ;	// body contents �� ���۵Ǵ� ����

					sTmp= sMokcha.Mid(nFindReturn+2, nFindEnd-nFindReturn-2 ) ;
					INT nReturnCnt= sTmp.Replace( _T("\r\n") , _T("\r\n") ) ;
					for ( INT i=0 ; i<nReturnCnt ; i++ )
					{
						sKisaNo= GetYonKisaNo(nStartLine, false) ;
						if ( sKisaNo==_CHOROK_ERROR_STRING_ )	continue ;
						if ( sKisaNo.IsEmpty() )
						{
							sKisaNo= GetNewKisaNo() ;
							if ( sKisaNo==_CHOROK_ERROR_STRING_ )	return -1 ;
//							if ( GetOneLineString(nStartLine, sOneKisa)<0 )	continue ;
//							if ( ! sOneKisa.IsEmpty() )	{	// ��簡 �������� ��������ȣ�� �߰������� ������ ����!
								WriteYonNewKisaNo2Mokcha( nStartLine, sKisaNo ) ;
								ReflectYonChorok2Stru(_T(""), sKisaNo) ;
//							}
						}
						nStartLine++ ;
					}

					nStart= nFindEnd + BODY_START_LENGTH + 1 ;
				}
			}
		}

	} while ( nFindStart>=0 && nFindEnd>=0 ) ;


	return 0 ;
}

INT CTocEditorDlg::CheckKisaNofromAllKisa(CString sMokcha)
{
	CString sKisaNo ;
	CString sOneKisa ;

	INT nCnt= 0 ;
	INT nStart= 0 ;
	INT nFindStart= -1 ;
	INT nFindEnd= -1 ;
	INT nStartLine= 1 ;

	do {
		nFindStart= sMokcha.Find( BODY_START_TAG , nStart ) ;
		if ( nFindStart>=0 )
		{
			nStart= nFindStart + BODY_START_LENGTH ;
			nFindEnd= sMokcha.Find( BODY_END_TAG , nStart ) ;
			if ( nFindEnd>=0 )		// <body>�� </body>�� ã�Ҵ�.
			{
				INT nFindReturn= sMokcha.Find( _T("\r\n") , nFindStart ) ;
				if ( nFindReturn>=0 )
				{
					CString sTmp ;
					sTmp= sMokcha.Mid(0, nFindReturn+2 ) ;
					nStartLine= sTmp.Replace( _T("\r\n") , _T("\r\n") ) ;
					nStartLine++ ;	// body contents �� ���۵Ǵ� ����

					sTmp= sMokcha.Mid(nFindReturn+2, nFindEnd-nFindReturn-2 ) ;
					INT nReturnCnt= sTmp.Replace( _T("\r\n") , _T("\r\n") ) ;
					for ( INT i=0 ; i<nReturnCnt ; i++ )
					{
						sKisaNo= GetYonKisaNo(nStartLine, false) ;
						if ( sKisaNo==_CHOROK_ERROR_STRING_ )	continue ;
						if ( sKisaNo.IsEmpty() )
						{
							nCnt++ ;
						}
						nStartLine++ ;
					}

					nStart= nFindEnd + BODY_START_LENGTH + 1 ;
				}
			}
		}

	} while ( nFindStart>=0 && nFindEnd>=0 ) ;


	return nCnt ;		// body ���� ��� �߿� ��������ȣ�� ���� ����
}

BOOL CTocEditorDlg::IsInBody()
{
	CString sMokcha ;
	m_TocEdit.GetWindowText(sMokcha) ;

	m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;

	INT nFindStart= 0 ;
	INT nFindEnd= 0 ;
	INT nFindIdx= -1 ;
	CString sTmp ;
	sTmp= sMokcha.Mid(0, m_nStartChar) ;	// ����

	do {
		nFindIdx= sTmp.Find(BODY_START_TAG, nFindStart) ;
		if ( nFindIdx!=-1 )	nFindStart= nFindIdx+BODY_START_LENGTH ;
	} while ( nFindIdx!=-1 ) ;

	do {
		nFindIdx= sTmp.Find(BODY_END_TAG, nFindEnd) ;
		if ( nFindIdx!=-1 )	nFindEnd= nFindIdx+BODY_START_LENGTH+1 ;
	} while ( nFindIdx!=-1 ) ;

	if ( nFindStart<=nFindEnd )	return FALSE ;

	sTmp= sMokcha.Mid(m_nStartChar) ;		// ����
	nFindStart= sTmp.Find(BODY_START_TAG) ;
	nFindEnd= sTmp.Find(BODY_END_TAG) ;

	if ( nFindStart==-1 && nFindEnd!=-1 )	return TRUE ;
	if ( nFindStart<=nFindEnd )				return FALSE ;

	return TRUE ;
}

BOOL CTocEditorDlg::ChorokExist(CString sKisaNo)
{
	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if ( tOneChorok.sKey==sKisaNo ) {
			if ( (tOneChorok.sChorokText).IsEmpty() )
				return FALSE ;
			else
				return TRUE ;
		}
	}

	return FALSE ;
}

VOID CTocEditorDlg::InitYonStatus()
{
	InitYonKisaStatus() ;
	InitYonChorokStatus() ;
}

VOID CTocEditorDlg::InitYonKisaStatus()
{
	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if ( (tOneChorok.sKisaStatus).IsEmpty() ) continue ;

		if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_DELETE_ ) {
			m_aChorokStru.RemoveAt(i) ;		// ��簡 delete �Ǹ� ���ڵ带 �����.
			i-- ;
			continue ;
		}

		if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_NEW_ || 
			tOneChorok.sKisaStatus==_CHOROK_STATUS_MODIFY_ )
		{	(tOneChorok.sKisaStatus).Empty() ;	}
		else
		{	AfxMessageBox( _T("��� ���������� ���������� ���� ����ֽ��ϴ�!") , MB_ICONERROR) ;	}
	}
}

VOID CTocEditorDlg::InitYonChorokStatus()
{
	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if ( (tOneChorok.sChorokStatus).IsEmpty() )	continue ;

		if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_DELETE_ ) {
			m_aChorokStru.RemoveAt(i) ;		// �����.
			i-- ;
			continue ;
		}

		if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_NEW_ || 
			tOneChorok.sChorokStatus==_CHOROK_STATUS_MODIFY_ || 
			tOneChorok.sChorokStatus==_CHOROK_STATUS_DELETE_ )
			(tOneChorok.sChorokStatus).Empty() ;
		else
		{	AfxMessageBox( _T("�ʷ� ���������� ���������� ���� ����ֽ��ϴ�!") , MB_ICONERROR) ;	}
	}
}

VOID CTocEditorDlg::SetReadOnly(BOOL bReadOnly)
{
	/*
	if ( bReadOnly )
		m_TocEdit.SetBackgroundColor(FALSE, RGB(191,191,191)) ;
	else
		m_TocEdit.SetBackgroundColor(FALSE, RGB(255,255,255)) ;

	m_TocEdit.SetReadOnly(bReadOnly) ;
	*/
}

// ��������ȣ �±� ��ȣ ��ƾ!
// ��������ȣ�±״� ������ �� ������ �� �ֱ� ���� ����Ѵ�.
BOOL CTocEditorDlg::IsInKisaTag(CString sMokcha, INT nLine, INT nCharIdx)
{
	INT nFindStart= -1 ;
	INT nFindEnd= -1 ;
	INT nLineIdx= 0 ;

//	GetOneLineString(nLine, sMokcha) ;	// m_TocEdit.GetLine() �Լ� ��� �Լ�
	TCHAR szLine[MAX_LINE] ;
	INT nLineLength= m_TocEdit.GetLine(nLine-1, szLine) ;
	szLine[nLineLength]= _T('\0') ;
	sMokcha= szLine ;

	nFindStart= sMokcha.Find(_START_CHOROK_KEY_) ;
	if ( nFindStart>=0 ) {
		CString sEndTag ;
		sEndTag= _END_CHOROK_KEY_ ;
		nFindEnd= sMokcha.Find(_END_CHOROK_KEY_, nFindStart) ;
		if ( nFindEnd>0 ) {
			nFindEnd+= sEndTag.GetLength() ;
			nLineIdx= m_TocEdit.LineIndex(nLine-1) ;
			if ( nFindStart+nLineIdx<nCharIdx && nFindEnd+nLineIdx>=nCharIdx ) {
				MessageBox(_T("��������ȣ �±״� ������ �� �����ϴ�!"), _T("���"), MB_ICONWARNING) ;
				return TRUE ;
			}
		}
	}

	return FALSE ;
}

VOID CTocEditorDlg::TocEditorBackup()
{
	m_TocEdit.GetWindowText( m_strBak ) ;
	m_sBackupTocEdit= m_strBak ;				// ���
	if ( m_nIsKindOf==_YON_ )
		m_aBackupChorokStru.Copy(m_aChorokStru) ;	// ���
}

INT CTocEditorDlg::GetChorokFromLine(UINT nLine, CString &sChorok)
{
	sChorok.Empty() ;

	CString sTmp ;
	sTmp= GetYonKisaNo(nLine, false) ;
	if ( sTmp.IsEmpty() || sTmp==_CHOROK_ERROR_STRING_ )
		return -1 ;

	sTmp= GetYonChorok(nLine) ;
	if ( sTmp==_CHOROK_ERROR_STRING_ )
		return -2 ;

	sChorok= sTmp ;

	return 0 ;
}

BOOL CTocEditorDlg::KisaExistPrevnNext(INT nLine, BOOL bPrev, BOOL bChorokEmptyCheck/*=FALSE*/)
{
	CString sKisaNo ;
	CString strChorok ;

	if ( bPrev )
	{
		for ( INT i=nLine-1 ; i>=1 ; i-- )
		{
			sKisaNo= GetYonKisaNo(i, false) ;
			if ( ! (sKisaNo.IsEmpty() || sKisaNo==_CHOROK_ERROR_STRING_ ) )
			{
				if ( bChorokEmptyCheck )	// 2002.5.3 added by jungjoo
				{
					strChorok= GetYonChorok(i) ;
					if ( strChorok==_CHOROK_ERROR_STRING_ || strChorok.IsEmpty() ) 
						continue ;
				}

				return TRUE ;
			}
		}
	}
	else
	{
		INT nTotalLine= m_TocEdit.GetLineCount() ;
		for ( INT i=nLine+1 ; i<=nTotalLine ; i++ )
		{
			sKisaNo= GetYonKisaNo(i, false) ;
			if ( ! (sKisaNo.IsEmpty() || sKisaNo==_CHOROK_ERROR_STRING_ ) )
			{
				if ( bChorokEmptyCheck )	// 2002.5.3 added by jungjoo
				{
					strChorok= GetYonChorok(i) ;
					if ( strChorok==_CHOROK_ERROR_STRING_ || strChorok.IsEmpty() ) 
						continue ;
				}

				return TRUE ;
			}
		}
	}

	return FALSE ;
}

INT CTocEditorDlg::GetChoroknKisa(CString &sChorok, CString &sKisaNo, CString &sKisa, INT nLine, BOOL bPrev, BOOL bChorokEmptyCheck/*=FALSE*/)
{
	CString strKisaNo ;
	CString strChorok ;
	CString strKisa ;

	if ( bPrev )
	{
		for ( INT i=nLine-1 ; i>=1 ; i-- )
		{
			strKisaNo= GetKisaNofromMokchaLine(i) ;
			if ( strKisaNo==_CHOROK_ERROR_STRING_ || strKisaNo.IsEmpty() ) {
				continue ;
			} else {
				strChorok= GetYonChorok(i) ;
				if ( strChorok==_CHOROK_ERROR_STRING_ ) 
					return -1 ;
				strKisa= GetKisafromKisaNo(strKisaNo) ;
				if ( strKisa==_CHOROK_ERROR_STRING_ )
					return -2 ;

				if ( bChorokEmptyCheck && strChorok.IsEmpty() )	continue ;	// 2002.5.3 added by jungjoo
				sChorok= strChorok ;
				sKisaNo= strKisaNo ;
				sKisa= strKisa ;
				m_icurLine= i ;	// added 2002.11.1 ������ �ʷϿ��� ���� ���� ����� ����
				return i ;		// ��簡 �����ϴ� ������ �������ش�.
			}

		}

	}
	else
	{
		INT nTotalLine= m_TocEdit.GetLineCount() ;
		for ( INT i=nLine+1 ; i<=nTotalLine ; i++ )
		{
			strKisaNo= GetKisaNofromMokchaLine(i) ;
			if ( strKisaNo==_CHOROK_ERROR_STRING_ || strKisaNo.IsEmpty() ) {
				continue ;
			} else {
				strChorok= GetYonChorok(i) ;
				if ( strChorok==_CHOROK_ERROR_STRING_ ) 
					return -1 ;
				strKisa= GetKisafromKisaNo(strKisaNo) ;
				if ( strKisa==_CHOROK_ERROR_STRING_ )
					return -2 ;

				if ( bChorokEmptyCheck && strChorok.IsEmpty() )	continue ;	// 2002.5.3 added by jungjoo
				sChorok= strChorok ;
				sKisaNo= strKisaNo ;
				sKisa= strKisa ;
				m_icurLine= i ;	// added 2002.11.1 ������ �ʷϿ��� ���� ���� ����� ����
				return i ;		// ��簡 �����ϴ� ������ �������ش�.
			}

		}
	}

	return -10 ;
}

BOOL CTocEditorDlg::ExistUndonRedo(BOOL bIsUndo)
{
	if ( bIsUndo )
		return ! m_sBackupTocEdit.IsEmpty() ;
	else
		return ! m_sRedoTocEdit.IsEmpty() ;
}

INT CTocEditorDlg::GetMoveRecord(BOOL bMokcha, BOOL bNext, CString& strNewString, BOOL *pbPrevEnable, BOOL *pbNextEnable)
{
	if ( m_pMoveRecordFunc==NULL ) {
		AfxMessageBox( _T("m_pMoveRecordFunc is NULL"), MB_ICONERROR ) ;
		return -20 ;
	}
	else 
	{
		INT ids= (m_pParentWnd->*m_pMoveRecordFunc)(bMokcha, bNext, strNewString, pbPrevEnable, pbNextEnable) ;
		if ( ids<0 )
		{
//			AfxMessageBox( _T("���� �ο��޴µ� �����߽��ϴ�!\r\nMoveRecord �Լ� ����!"), MB_ICONERROR ) ;
			strNewString.Empty() ;	// �������� ��� �׳� �����ش�.
			return ids ;
		}
	}

	return 0 ;
}

VOID CTocEditorDlg::OnShowViewer()
{
	if ( m_pParentWnd==NULL ) return ;
	m_pParentWnd->SendMessage(MSG_SHOW_VIEWER) ;
}

VOID CTocEditorDlg::GetChorokLine(CUIntArray *parrChorokLine)
{
	parrChorokLine->RemoveAll() ;

	CString sText ;
	m_TocEdit.GetWindowText( sText ) ;

	// ������ ��ȣ�� �Ѱ��ֱ� ����...
	// View ���̾�αװ� �� �ִ� ������ ������ �����Ǹ� �ȵȴ�.
	INT nSize= m_aChorokStru.GetSize() ;
	CString sKisaTag ;
	CString sFront ;
	for ( INT i=0 ; i<nSize ; i++ )
	{
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_DELETE_ ||		// �ʷ��� �ִ� ����� ���θ� �Ѱ��ֵ��� ����.
			tOneChorok.sChorokStatus==_CHOROK_STATUS_DELETE_ ||
			(tOneChorok.sChorokText).IsEmpty() )
			continue ;

		sKisaTag= _START_CHOROK_KEY_ ;
		sKisaTag+= tOneChorok.sKey ;
		sKisaTag+= _END_CHOROK_KEY_ ;

		INT nFind= sText.Find(sKisaTag) ;
		if ( nFind<0 ) {	//�����߻�
			AfxMessageBox( _T("����±� ") + sKisaTag + _T(" �� ������ �������� �ʽ��ϴ�.")) ;
			continue ;
		}

		sFront= sText.Mid(0, nFind) ;
		INT nLine= sFront.Replace(_T("\r\n"), _T("\r\n"));
		parrChorokLine->Add(nLine+1) ;
	}
}

INT CTocEditorDlg::GetYonAllKisaNo(CStringArray& arrKisaNo)
{
	INT nTotalLine= m_TocEdit.GetLineCount() ;
	CString sKisaNo ;
	for ( INT i=1 ; i<=nTotalLine ; i++ ) {
		sKisaNo= GetKisaNofromMokchaLine(i) ;
		if ( sKisaNo==_CHOROK_ERROR_STRING_ ) {
			AfxMessageBox( _T("��������ȣ �������� ����!"), MB_ICONERROR ) ;
			return -1 ;
		}
		else if ( ! sKisaNo.IsEmpty() )
		{
			arrKisaNo.Add(sKisaNo) ;
		}
	}

	return 0 ;
}

INT CTocEditorDlg::GetBanchoolFormat(CString sMokcha, CString& sResult)
{
	// ��������ȣ^���Ӱ��๰�����ȣ^��ȣ�����ȣ^�������^���ڸ�^����������(26)
	// ��������ȣ :		_START_CHOROK_KEY_ ~ _END_CHOROK_KEY_
	// ���Ӱ��๰�����ȣ :	<id>
	// ��ȣ�����ȣ :		<key>
	// �������
	// ���ڸ�
	// ����������

	// �ʱ�ȭ
	sMokcha.Replace(_T("\r\n"), _T("\n"));	// Ȥ�� �������� �𸣴� ���๮�� ġȯ
	sResult.Empty();
	TCHAR szDelimiter= 26;
	TCHAR sep1[] = _T("/");		// �������� ���ڸ� ������
	TCHAR sep2[] = _T("=");		// ���� ���������� ������


	INT returnidx, tagidx, nYonControlTagLength, nKwonControlTagLength;
	CString sYonControlTag, sYonControlNo;
	CString sKwonControlTag, sKwonControlNo;
	sYonControlTag= _T("<id>");		nYonControlTagLength= sYonControlTag.GetLength();
	sKwonControlTag= _T("<key>");	nKwonControlTagLength= sKwonControlTag.GetLength();
	CString sKey, sKisaTitle, sAuthor, sStartPage;

	// ���Ӱ��๰�����ȣ
	tagidx= sMokcha.Find(sYonControlTag);
	if ( tagidx<0 )	return -1;
	returnidx= sMokcha.Find(_T("\n"), tagidx);	// ���๮�� �˻�
	if ( returnidx< 0 )	return -2;
	sYonControlNo= sMokcha.Mid(tagidx+nYonControlTagLength , returnidx-tagidx-nYonControlTagLength);

	// ��ȣ�����ȣ
	tagidx= sMokcha.Find(sKwonControlTag);
	if ( tagidx<0 )	return -3;
	returnidx= sMokcha.Find(_T('\n'), tagidx);	// ���๮�� �˻�
	if ( returnidx< 0 )	return -4;
	sKwonControlNo= sMokcha.Mid(tagidx+nKwonControlTagLength , returnidx-tagidx-nKwonControlTagLength);

	INT nFindStart, nFindEnd, nStart, nReturn, ntmp;
	nFindStart= nFindEnd= 0 ;	// �ʱ�ȭ
	CString sStartTag, sEndTag, sCutting, sTmp ;
	sStartTag= _START_CHOROK_KEY_ ;
	sEndTag= _END_CHOROK_KEY_ ;


	while ( TRUE )
	{
		nFindStart= sMokcha.Find(sStartTag, nFindEnd) ;
		if ( nFindStart<0 )
		{
			break ;
		}

		nFindEnd=  sMokcha.Find(_END_CHOROK_KEY_, nFindStart ) ;
		if ( nFindEnd<0 )
		{
			return -10 ;
		}

		nStart= nFindStart+sStartTag.GetLength() ;
		sKey= sMokcha.Mid(nStart, nFindEnd-nStart) ;	// KEY ����
		sCutting= sMokcha;

		sCutting= sCutting.Mid(0,nFindStart);
		nReturn= sCutting.ReverseFind('\n');
		if ( nReturn<0 ) return -11 ;
		sCutting= sCutting.Mid( nReturn+1 ) ;	// sCutting- �� ���ο��� ��������ȣ �ձ����� ����


		// �������, ���ڸ�, ���������� �����´�.
		sKisaTitle= sCutting;
		ntmp= sCutting.Find(sep1);
		if( ntmp<0 )	// '/' �� ���� ���
		{
			sAuthor= _T("");	// ���ڸ��� ���� ������ �����Ѵ�.
			ntmp= sCutting.Find(sep2);
			if( ntmp<0 )	// �����ڰ� ��� ���� ���
			{
				sStartPage= _T("");
			}
			else	// '=' �� �ִ� ���
			{
				sKisaTitle= sCutting.Left(ntmp);
				sStartPage= sCutting.Mid(ntmp+1);
			}
		}
		else		// '/' �� �ִ� ���
		{
			sKisaTitle= sCutting.Left(ntmp);
			sCutting= sCutting.Mid(ntmp+1);
			ntmp= sCutting.Find(sep2);
			if( ntmp<0 )	// '=' �� ���� ���
			{
				sAuthor= sCutting;
				sStartPage= _T("");	// ������������ ���� ������ �����Ѵ�.
			}
			else	// �����ڰ� ��� �ִ� ���
			{
				sAuthor= sCutting.Left(ntmp);
				sStartPage= sCutting.Mid(ntmp+1);
			}
		}

		sKisaTitle.TrimLeft();	sKisaTitle.TrimRight();
		sAuthor.TrimLeft();		sAuthor.TrimRight();
		sStartPage.TrimLeft();	sStartPage.TrimRight();

		sResult+= szDelimiter + sKey + _T("^");	// ��������ȣ
		sResult+= sYonControlNo + _T("^");		// ���Ӱ��๰�����ȣ
		sResult+= sKwonControlNo + _T("^");		// ��ȣ�����ȣ
		sResult+= sKisaTitle + _T("^");			// �������
		sResult+= sAuthor + _T("^");			// ���ڸ�
		sResult+= sStartPage;					// ����������
	}

	sResult.TrimLeft(szDelimiter);

	return 0 ;	// ����
}
/*
VOID CTocEditorDlg::SetTocDefault(CString csstr0, CString csstr1, CString csstr2,
		CString csstr3, CString csstr4, CString csstr5, CString csstr6);
{
	csId = csstr0;
	csCall = csstr1;
	csCount = csstr2;
	csDate = csstr3;
	csPub = csstr4;
	csSertitle = csstr5;
	csTitle = csstr6;	
}
*/
//VOID CTocEditorDlg::SetTocDefault(CString csstr0, CString csstr1, CString csstr2,
//		CString csstr3, CString csstr4, CString csstr5, CString csstr6, CString csstr7);
VOID CTocEditorDlg::SetTocDefault(CString *csstr)
{
	for(INT i=0;i<8;i++) {
		if(&csstr[i] == NULL) return;
		csstrTag[i] = csstr[i];
	}
}



