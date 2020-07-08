// TocEditor_TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TocEditor_Test.h"
#include "TocEditor_TestDlg.h"

#include "ChorokTestDlg.h"
#include "YonChorokSet.h"

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
// CTocEditor_TestDlg dialog

CTocEditor_TestDlg::CTocEditor_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTocEditor_TestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTocEditor_TestDlg)
	m_nline = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nKindof= 0 ;
//	mKIND = _DAN_SINGLE;
	mKIND = _DAN_MULTI;
//	mKIND = _YON_;
//	mKIND = _KISA_;

	// KOL.UDF.022 ��ť���ڵ� ����
	m_nMokchaLine = 0;
}

VOID CTocEditor_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTocEditor_TestDlg)
	DDX_Text(pDX, IDC_EDIT1, m_nline);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTocEditor_TestDlg, CDialog)
	//{{AFX_MSG_MAP(CTocEditor_TestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_VIEWER_BUTTON, OnViewerButton)
	ON_BN_CLICKED(IDC_DELETE_CHOROK_BUTTON, OnDeleteChorokButton)
	ON_BN_CLICKED(IDC_UPDATE_CHOROK_BUTTON, OnUpdateChorokButton)
	ON_BN_CLICKED(IDC_EDIT_CHOROK_BUTTON, OnEditChorokButton)
	ON_BN_CLICKED(IDC_VERIFY_CHOROK_BUTTON, OnVerifyChorokButton)
	ON_BN_CLICKED(IDC_SOOSIKEDITOR_BUTTON, OnSoosikEditorButton)
	ON_BN_CLICKED(IDC_SETKISANO2ALLKISA_BUTTON, OnSetKisaNo2allkisaButton)
	ON_BN_CLICKED(IDC_NEWMOKCHA_BUTTON, OnNewmokchaButton)
	ON_MESSAGE(MSG_EDIT_DAN_CHOROK, OnEditDanChorok)
	ON_MESSAGE(MSG_EDIT_YON_CHOROK, OnEditYonChorok)
	ON_MESSAGE(MSG_EDIT_KISA_CHOROK, OnEditKisaChorok)
	ON_MESSAGE(MSG_SHOW_VIEWER, OnShowViewer)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTocEditor_TestDlg message handlers

BOOL CTocEditor_TestDlg::OnInitDialog()
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
	TCHAR *sMokcha;
	//CString sMokcha ;
	CFile cf;
	if(!cf.Open(_T("c:\\toctmp.toc"), CFile::modeRead | CFile::typeBinary)) return FALSE;
	TCHAR cUni;
	cf.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		cf.SeekToBegin();
	}
	
	DWORD dlength = cf.GetLength();
	sMokcha = (TCHAR *)malloc(sizeof(TCHAR) * (dlength+1));
	cf.Read(sMokcha, dlength);
	cf.Close();
	sMokcha[dlength] = '\0';



//	===================================================
//	������ TocEditor ���̾�α׸� ����ϴ� �����Դϴ�.
//	�Լ� ȣ���ϴ� ������ �����Ͽ� ����Ͽ� �ֽʽÿ�.
//	===================================================
// ���ົ , ������ ������� ���� - ���� ���� ���� ���⸦ ����
	m_TocEditorDlg.SetMoveRecordFunc( (INT (CWnd::*)(BOOL, BOOL, CString&, BOOL*, BOOL*))MoveRecord ) ;

	if(mKIND == _DAN_SINGLE) {

		// 1. ���ົ�� ���
			m_nKindof= _DAN_SINGLE;
			// ����-����, ����-�ٱ�, ���Ӱ��๰ ����	_DAN_SINGLE, _DAN_MULTI, _YON_
			// �ݵ�� �������ֵ��� �Ѵ�. �������� ��쿡�� ����Ű�� �ݵ�� ������ �ش�.
			if ( m_TocEditorDlg.MyCreate(this, CRect(10,10,500,500), m_nKindof ) )
				AfxMessageBox( _T("���� �����⸦ ����� ���߽��ϴ�!") ) ;
			
		
		//	sMokcha= _T("<id>KMO199700001\r\n<call>01$a021.7$b��26e$c4\r\n<count>\r\n<date>1997\r\n<pub>�����߾ӵ�����\r\n<sertitle>\r\n<title>����������ȭ\r\n<body>����\r\nǥ���� = 0\r\n�߰��� = i\r\n���� = ii\r\n��1�� ���� = 1\r\n ��1�� ��Ȳ = 2\r\n ��2�� ������ = 5\r\n��2�� ���� = 20\r\n ��1�� �ذ��� = 20\r\n ��2�� ���İ�ȹ = 30\r\n��3�� ��� = 55\r\n������ = 155\r\n</body>\r\n<table>ǥ����\r\n<ǥ1>�ѱ��缭���� ��ġ��Ȳ = 15\r\n<ǥ2>�Ϻ��� �б������� �缭���� ��ġ���� = 22\r\n</table>\r\n<figure>�׸�����\r\n[�׸�1]�缭������ ���ɺ� �з� = 8\r\n[����1]�б� ������ = 12\r\n</figure>\r\n<body>Contents\r\ntitle page = 0\r\nReward = i\r\ncontents = ii\r\nchapter 1 preface = 1\r\n section 1 introduction = 2\r\n section 2 problem = 5\r\n</body>") ;
//			sMokcha= _T("<id>KMO2222\r\n<call>01  $$A001$$  B13$$C1-2$$\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<title>\r\n<vol>1\r\n<count>\r\n<body>����\r\n1 �ٰ����� �̱��� = 11\r\n2 ��ȭ�� ��� = 34\r\n</body>\r\n</vol>\r\n<vol>2\r\n<count>\r\n<body>����\r\n8 �ڽ��ϱ� ������ = 261\r\n9 ��ȭ�� ���� = 291\r\n10 �� ������ ���� = 358\r\n</body>\r\n</vol>");
			m_TocEditorDlg.SetTocFromString(sMokcha) ;

	}
	else if(mKIND == _YON_) {
			// 2. �������� ���
				m_nKindof= _YON_ ;
				if ( m_TocEditorDlg.MyCreate(this, CRect(0,0,500,400), m_nKindof ) ) {
					return FALSE ;	// ����!
				}

				// Primary Ű�� DB�κ��� �������� �Լ� ���� - CS �� �����־�� �Ѵ�.
				m_TocEditorDlg.SetYonKeyFunc( (INT (CWnd::*)(TCHAR*))MakePrimaryKey ) ;

			

//				sMokcha= _T("<id>KSE000011632\r\n<vol>2ȣ(II)\r\n<body>����\r\n������ / ����� = 2[!--10000000--]\r\n���翵 ���Ŀ���$$wow~~good~~$$ \r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\<body> Wow!!\r\n</body>\r\n<body>����\r\n������ / ����� = 2[!--10000000--]\r\n������ ���Ŀ��� $$\\sum { aa }$$** !$$!!$$!��٥��\r\n!$$%%%$$= 10[!--20000000--]\r\n��������������� : �����̰���ٽ������� / ������ = 14[!--30000000--]\r\n</body>\r\n") ;
			//	sMokcha= _T("<id>KSE000011632\r\n<key>17638754\r\n<vol>â��ȣ\r\n<call>\r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\n<body>����\r\n������ / ����� = 2[!--17806227--]\r\n������ / ��٥�� = 4[!--17806228--]\r\n��������Ѻ������Ҵ������ǳ������ / ˩�� = 10[!--17806229--]\r\n��������������� : �����̰���ٽ������� / ������ = 14[!--17806230--]\r\n�����϶� / ����� = 19[!--17806231--]\r\n���������濡���Ͼ� / ���� = 22[!--17806232--]\r\n���������� / ����� = 24[!--17806233--]\r\n���������������� / ���� = 29[!--17806234--]\r\n������������������θ����ϳ�� / ������ = 34[!--17806235--]\r\n����϶��������� / ������ = 39[!--17806236--]\r\n����������� / ��� = 42[!--17806237--]\r\n���� = 46[!--17806238--]\r\n����������? / ��٥�� = 47[!--17806239--]\r\n���������� / ������ = 55[!--17806240--]\r\n��˧ߣ��� / ��� = 57[!--17806241--]\r\n������������ / ���� = 64[!--17806242--]\r\n�����������[!--17806243--]\r\n��������ġ��[!--17806244--]\r\n</body>\r\n</vol>\r\n");

				// ��Ʈ������ ������ �־��ش�.
				m_TocEditorDlg.SetTocFromString(sMokcha) ;

				// �ʷ� ����.
				m_TocEditorDlg.SetExistYonChorok(_T("<ID=KDM000000001>\r\nù��° �ʷ�") , _T("10000000")) ;
				m_TocEditorDlg.SetExistYonChorok(_T("<ID=KDM000000002>\r\n�ι�° �ʷ�") , _T("20000000")) ;
				m_TocEditorDlg.SetExistYonChorok(_T("<ID=KDM000000003>\r\n����° �ʷ�") , _T("30000000")) ;

				// ����߿��� ���� ����� ���� �������� ���ϵ��� ���� ����� ��������ȣ�� �������ش�.
				// added by jungjoo 2002.4.12.
				if( m_TocEditorDlg.SetWonmunExistKisaNo(_T("30000000")) < 0 )
				{
					EndDialog(FALSE) ;
				}
				if( m_TocEditorDlg.SetWonmunExistKisaNo(_T("20000000")) < 0 )
				{
					EndDialog(FALSE) ;
				}

			/*
				sMokcha= _T("<id>KSE199501211\n<key>1021005640428\n<vol>���4\n<call>\n<count>\n<date>\n<pub>\n<sertitle>\n<issue>\n<body>����\n�д� ���� : ������ ���� / �ں��� = 8[!--10191568--]\n��[!--10191569--]\n �����⵵������ / �躸�� = 12[!--10191570--]\n �޲ɡ��Ͼ�׸��� / ��â�� = 14[!--10191571--]\n ������ �帣�鼭������ / ����ȭ = 17[!--10191572--]\n ����� ��������⵵ 2 / �̱��� = 19[!--10191573--]\n �޲ٴ� ������ �Ǿ��å / �̻� = 21[!--10191574--]\n ������ �뷡�����갭 Ǯ�� / �ֱ�â = 24[!--10191575--]\nƯ�� : �ѱ��⵶�������� ����[!--10191576--]\n �⵶�� ������ ���ɼ� / ������ = 28[!--10191577--]\n ������ ������ �� / ���̵� = 40[!--10191578--]\n �ѱ� �Ҽ��� �⵶���� ���� / �̰Ǽ� = 45[!--10191579--]\n �⵶�� �Ƶ������� ���ð� ���� / ��ö�� = 51[!--10191580--]\n����[!--10191581--]\n �߰��� �ֿϰ� �׸��� ������ / ����� = 56[!--10191582--]\n ���� � ���� / �۱�� = 61[!--10191583--]\n��ȭ[!--10191584--]\n �������� ���� / ��ö�� = 66[!--10191585--]\n ���� ���� ��� ���� ���� ��� / ���¼� = 72[!--10191586--]\n����� : ��������(3) / �ڰ��� = 81[!--10191587--]\n�ٽú� : ��ݰ� �㼼�� �ɸ� / ������ = 90[!--10191588--]\n�������� : ������̶�� ��ȥ ���ϳ��� / �۸��� = 93[!--10191589--]\n</body>\n</vol>\n");
				m_TocEditorDlg.SetTocFromString(sMokcha) ;
				m_TocEditorDlg.SetExistYonChorok(_T("<ID=KDM000000001>\r\nù��° �ʷ�") , _T("10191568")) ;
				m_TocEditorDlg.SetExistYonChorok(_T("<ID=KDM000000002>\r\n�ι�° �ʷ�") , _T("10191570")) ;
			*/
	}
	else if(mKIND == _KISA_) {

			// 3. ��繰 �϶�..

			m_nKindof= _KISA_ ;
			if ( m_TocEditorDlg.MyCreate(this, CRect(0,0,500,400), m_nKindof ) ) {
				return FALSE ;	// ����!
			}

		
//			sMokcha= _T("<id>KMO2222\n<call>�����01$$A001$$B13$$C1-2$$\n<date>\n<pub>\n<sertitle>\n<title>\n<vol>1\n<count>\n<body>����\n1 �ٰ����� �̱��� = 11\n2 ��ȭ�� ��� = 34\n</body>\n</vol>\n<vol>2\n<count>\n<body>����\n8 �ڽ��ϱ� ������ = 261\n9 ��ȭ�� ���� = 291\n10 �� ������ ���� = 358\n</body>\n</vol>");
//			sMokcha= _T("0123456789$$<?xml version='1.0' encoding='euc-kr'?>\r\n<m:math xmlns:m='http://www.w3.org/1998/Math/MathML'>\r\n <m:mrow>r\n  <m:mfrac>\r\n   <m:mrow>\r\n    <m:mi>&#x0031;</m:mi>\r\n    <m:mi>&#x0032;</m:mi>\r\n    <m:mi>&#x0033;</m:mi>\r\n   </m:mrow>\r\n   <m:mrow>\r\n    <m:mi>&#x0031;</m:mi>\r\n    <m:mi>&#x0032;</m:mi>\r\n   </m:mrow>\r\n  </m:mfrac>\r\n  <m:mo>&#x002b;</m:mo>\r\n  <m:msub>\r\n   <m:mrow>\r\n    <m:mo>&#x222b;</m:mo>\r\n   </m:mrow>\r\n</m:msub>\r\n  <m:mrow>\r\n   <m:mi>&#x0066;</m:mi>\r\n   <m:mi>&#x0077;</m:mi>\r\n   <m:mi>&#x0065;</m:mi>\r\n   <m:mi>&#x0077;</m:mi>\r\n  </m:mrow>\r\n </m:mrow>\r\n</m:math >\r\n \r\n$$0123456789012$$<?xml version='1.0' encoding='euc-kr'?>\r\n<m:math xmlns:m='http://www.w3.org/1998/Math/MathML'>\r\n <m:mrow>\r\n  <m:munder>\r\n   <m:mrow>\r\n    <m:mo>&#x2211;</m:mo>\r\n    <m:mo>&#x0028;</m:mo>\r\n     <m:mrow>\r\n      <m:mi>&#x006e;</m:mi>\r\n      <m:mo>&#x002b;</m:mo>\r\n      <m:mi>&#x0031;</m:mi>\r\n     </m:mrow>\r\n    <m:mo>&#x0029;</m:mo>\r\n   </m:mrow>\r\n   <m:mrow>\r\n    <m:mi>&#x006b;</m:mi>\r\n    <m:mo>&#x003d;</m:mo>\r\n    <m:mi>&#x0031;</m:mi>\r\n   </m:mrow>\r\n</m:munder>\r\n </m:mrow>\r\n</m:math >\r\n$$3456789012345678901234567890") ;

			// ��Ʈ������ ������ �־��ش�.
			m_TocEditorDlg.SetTocFromString(sMokcha) ;
	}

/*
	// 3. ���� ���� �������� �׽�Ʈ
	CString sResult;
	CString sMokcha1= _T("<id>KSE000011632\r\n<key>17638754\r\n<vol>â��ȣ\r\n<call>\r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\n<body>����\r\n������ / ����� = 2[!--17806227--]\r\n������ / ��٥�� = 4[!--17806228--]\r\n��������Ѻ������Ҵ������ǳ������ / ˩�� = 10[!--17806229--]\r\n��������������� : �����̰���ٽ������� / ������ = 14[!--17806230--]\r\n�����϶� / ����� = 19[!--17806231--]\r\n���������濡���Ͼ� / ���� = 22[!--17806232--]\r\n���������� / ����� = 24[!--17806233--]\r\n���������������� / ���� = 29[!--17806234--]\r\n������������������θ����ϳ�� / ������ = 34[!--17806235--]\r\n����϶��������� / ������ = 39[!--17806236--]\r\n����������� / ��� = 42[!--17806237--]\r\n���� = 46[!--17806238--]\r\n����������? / ��٥�� = 47[!--17806239--]\r\n���������� / ������ = 55[!--17806240--]\r\n��˧ߣ��� / ��� = 57[!--17806241--]\r\n������������ / ���� = 64[!--17806242--]\r\n�����������[!--17806243--]\r\n��������ġ��[!--17806244--]\r\n</body>\r\n</vol>\r\n");
	m_TocEditorDlg.GetBanchoolFormat(sMokcha1, sResult );
	AfxMessageBox(sResult);
*/





	if( m_TocEditorDlg.m_hWnd!= NULL)
		m_TocEditorDlg.SetFocus() ;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CTocEditor_TestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CTocEditor_TestDlg::OnPaint() 
{
//	m_nline = m_TocEditorDlg.m_TocEdit.GetFirstVisibleLine();;
//	UpdateData(FALSE);
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
HCURSOR CTocEditor_TestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CTocEditor_TestDlg::OnViewerButton() 
{
	// TODO: Add your control notification handler code here
	OnShowViewer() ;
}

VOID CTocEditor_TestDlg::OnDeleteChorokButton() 
{
	// TODO: Add your control notification handler code here
	// ���� Ŀ���� ��ġ�� ���� ���� �̿��ؼ� ��������ȣ�� ã�´�.
	m_nMokchaLine= m_TocEditorDlg.GetCurrentLine() ;
	CString sChorokText, sKisaNo ;
	sKisaNo= m_TocEditorDlg.GetYonKisaNo(m_nMokchaLine, false) ;

	if ( sKisaNo.IsEmpty()			// ���ο� �ʷ��� ���� ���� �ʴ� ��츦 �ɷ�����.
		|| sKisaNo==_CHOROK_ERROR_STRING_ )
		return ;

	CString sMsg ;
	sMsg= _T("��������ȣ ") + sKisaNo + _T(" �� ���� �ʷ��� ���� �ϰڽ��ϱ�?");
	if ( IDYES == AfxMessageBox( sMsg , MB_YESNO ) ) 
	{
		// �ʷ� ����ü���� ���� ���ش�.(���� �÷���)
//		if ( m_TocEditorDlg.DeleteYonKisaNo(sKisaNo) < 0 ) {
//			;	// �ʷ� ���� ����
//		}
		if ( m_TocEditorDlg.ReflectYonChorok2Stru(_T(""),sKisaNo) < 0 ) {
			// ����
		}
	}

}

VOID CTocEditor_TestDlg::OnUpdateChorokButton() 
{
	// TODO: Add your control notification handler code here

	CYonChorokSet rs ;
	m_TocEditorDlg.GetResultYonChorokSet(rs) ;
	while ( rs.Next() ) {
		CString sKey= rs.GetKisaNo() ;
		CString sText= rs.GetChorokText() ;
		CString sKisaStatus= rs.GetKisaStatus() ;		
		CString sChorokStatus= rs.GetChorokStatus() ;
		CString sKisa= rs.GetKisa() ;		// ����߰�!

		CString sMsg ;
		if ( sChorokStatus==_CHOROK_STATUS_NEW_ )		// �ʷ��� ���� �ۼ��� ���
		{
			sMsg = _T("�ʷ� �����ۼ�!\r\n") ;
			sMsg += _T("Ű : ") ;
			sMsg += sKey ;
			sMsg += _T("\r\n") ;
			sMsg += _T("���� : ") ;
			sMsg += sText ;
			sMsg += _T("\r\n") ;
			sMsg += _T("��� : ") ;
			sMsg += sKisa ;
			AfxMessageBox(sMsg) ;
			// DB �� ������Ʈ �����ش�.
		}
		else if ( sChorokStatus==_CHOROK_STATUS_MODIFY_ )	// �ʷ��� ����� ���
		{
			sMsg = _T("�ʷ� ����!\r\n") ;
			sMsg += _T("Ű : ") ;
			sMsg += sKey ;
			sMsg += _T("\r\n") ;
			sMsg += _T("���� : ") ;
			sMsg += sText ;
			sMsg += _T("\r\n") ;
			sMsg += _T("��� : ") ;
			sMsg += sKisa ;
			AfxMessageBox(sMsg) ;
			// DB �� ������Ʈ �����ش�.
		}
		else if ( sChorokStatus==_CHOROK_STATUS_DELETE_ )	// �ʷ��� ������ ���
		{
			sMsg = _T("�ʷ� ����!\r\n") ;
			sMsg += _T("Ű : ") ;
			sMsg += sKey ;
			sMsg += _T("\r\n") ;
			sMsg += _T("���� : ") ;
			sMsg += sText ;
			sMsg += _T("\r\n") ;
			sMsg += _T("��� : ") ;
			sMsg += sKisa ;
			AfxMessageBox(sMsg) ;
			// DB �� ������Ʈ �����ش�.
		}

		if ( sKisaStatus==_CHOROK_STATUS_NEW_ )		// ��������ȣ�� ���� �ۼ��� ���
		{
			sMsg = _T("��������ȣ �����ۼ�!\r\n") ;
			sMsg += _T("Ű : ") ;
			sMsg += sKey ;
			sMsg += _T("\r\n") ;
			sMsg += _T("���� : ") ;
			sMsg += sText ;
			sMsg += _T("\r\n") ;
			sMsg += _T("��� : ") ;
			sMsg += sKisa ;
			AfxMessageBox(sMsg) ;
		}
		else if ( sKisaStatus==_CHOROK_STATUS_DELETE_ )		// ��������ȣ�� ������ ���
		{
			sMsg = _T("��������ȣ ����!\r\n") ;
			sMsg += _T("Ű : ") ;
			sMsg += sKey ;
			sMsg += _T("\r\n") ;
			sMsg += _T("���� : ") ;
			sMsg += sText ;
			sMsg += _T("\r\n") ;
			sMsg += _T("��� : ") ;
			sMsg += sKisa ;
			AfxMessageBox(sMsg) ;
		}
	}
	rs.Free();

}

VOID CTocEditor_TestDlg::OnEditChorokButton() 
{
	// TODO: Add your control notification handler code here
	if (m_nKindof==_YON_)// �������� ���
	{
		OnEditYonChorok();
	}
	else if (m_nKindof == _KISA_)
	{
		OnEditKisaChorok();
	}		
	else	// ���ົ�� ���
	{
		OnEditDanChorok();
	}
}

// ������ ������Ʈ �ϱ� ����
// ������ ���� ��������ȣ�� ��Ȯ�� ��ġ�� ������ Ȯ���� �� �� �ִ� �Լ�
// �������� ��������ȣ�� ����� ���ԵǾ����� �˻�������.
VOID CTocEditor_TestDlg::OnVerifyChorokButton() 
{
	// TODO: Add your control notification handler code here
	CString sMokcha ;
	sMokcha= m_TocEditorDlg.GetFinalToc() ;

	INT nCnt= m_TocEditorDlg.CheckYonKisaNo(sMokcha) ;
	if ( nCnt ) {	// nCnt �� �������� ���� �Ǽ��� ��Ÿ���ش�.
		CString sError ;
		sError.Format(_T("�������� ��������ȣ ���� �Ǽ��� %d �� �˻��Ǿ����ϴ�."), nCnt) ;
		AfxMessageBox(sError) ;
	}
	else {
		AfxMessageBox(_T("�̻����!")) ;
	}
}

VOID CTocEditor_TestDlg::OnSoosikEditorButton() 
{
	// TODO: Add your control notification handler code here
	m_TocEditorDlg.ExecuteSoosikEditor() ;
}

VOID CTocEditor_TestDlg::OnSetKisaNo2allkisaButton() 
{
	// TODO: Add your control notification handler code here
	m_TocEditorDlg.SetKisaNo2AllKisa() ;
}

VOID CTocEditor_TestDlg::OnNewmokchaButton() 
{
	// TODO: Add your control notification handler code here
	m_TocEditorDlg.TocNewEdit() ;
}

// ������ ��ư �˾��޴��� �ʷ� ����(���ົ)
VOID CTocEditor_TestDlg::OnEditDanChorok()
{
//	AfxMessageBox( _T("�̰����� ���ົ �ʷ� ���� �����Ͻʽÿ�!") ) ;
	ASSERT(m_TocEditorDlg) ;
	CChorokTestDlg dlg(_T("<ID=KDM000000000>\r\n�׽�Ʈ �����ʷ�"), &m_TocEditorDlg) ;
	dlg.DoModal();
}

// ������ ��ư �˾��޴��� �ʷ� ����(������)
VOID CTocEditor_TestDlg::OnEditYonChorok()
{
//	AfxMessageBox( _T("�̰����� ������ �ʷ� ���� �����Ͻʽÿ�!") ) ;
	ASSERT(m_TocEditorDlg);
	CChorokTestDlg dlg(_T("<id=KDM000000000>\r\n<kabs>�׽�Ʈ �����ʷ�\r\n\r\n</kabs>\r\n<fabs>abstract\r\n\r\n</fabs>"), &m_TocEditorDlg) ;
	dlg.DoModal();
}

VOID CTocEditor_TestDlg::OnEditKisaChorok()
{
	ASSERT(m_TocEditorDlg);
	CChorokTestDlg dlg(_T("<ID=KDM000000000>\r\n�׽�Ʈ ����ʷ�"), &m_TocEditorDlg) ;
	dlg.DoModal();
}

// ������ ��ư �˾��޴��� ���� ��� ����()
VOID CTocEditor_TestDlg::OnShowViewer()
{
	m_TocEditorDlg.ShowViewer(TRUE, FALSE) ;
}

// �ӽ� MakePrimaryKey �Լ�
INT CTocEditor_TestDlg::MakePrimaryKey(TCHAR* pKey)
{
	CString strKey;
	CStdioFile file;
	if ( file.Open(_T("key.txt"),CFile::modeRead | CFile::typeBinary)==NULL )
		return -1 ;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
		
	file.ReadString(strKey);
	INT nID=_ttoi(strKey);
	file.Close();
	nID++ ;

	TCHAR szID[64] ;
	wmemset( szID, '\0', sizeof(szID) ) ;
	_itot(nID, szID, 10) ;
	_tcscpy( pKey, szID );
	
	if (file.Open(_T("key.txt"),CFile::modeCreate|CFile::modeWrite | CFile::typeBinary)==NULL)
		return -2 ;
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	strKey.Format(_T("%d"),nID);
	file.WriteString(strKey);
	file.Close();
	return 0 ;
}

// �ӽ� MoveRecord �Լ� - ���� ���� ���� ������ �� �ʿ��ϴ�.
INT CTocEditor_TestDlg::MoveRecord( BOOL bMokcha , BOOL bNext , CString& strNewString , BOOL *pbPrevEnable , BOOL *pbNextEnable )
{
	*pbPrevEnable = FALSE ;
	*pbNextEnable = TRUE ;

	if ( bMokcha )
		strNewString = _T("<id>MoveRecord�Լ� �׽�Ʈ\r\n<key>\r\n<call>test\r\n<count>123\r\n<date>2001/10\r\n<pub>eco\r\n<sertitle>\r\n<issue>\r\n<body>����\r\nPrev, Next Test!!!! = 1[!--10000000--]\r\n</body>\r\n<table>\r\n</table>\r\n<figure>\r\n</figure>\r\n<body>\r\nù��° ��� - �� �κ��� Prev, Next Test $$ \\sum { aa } $$ ���ϴ�. = 2[!--20000000--]\r\n</body>\r\n<body>\r\nPrev, Next Test = 3[!--30000000--]\r\n</body>") ;
	else
		strNewString = _T("<ID=KDM999999999>\r\n�ٲ� �ʷ�!") ;

	return 0 ;
}

VOID CTocEditor_TestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CStringArray arrString ;
	m_TocEditorDlg.GetYonAllKisaNo(arrString) ;

	for(INT i=0 ; i<arrString.GetSize() ; i++ )
	{
		AfxMessageBox(arrString.GetAt(i) );
	}
}
