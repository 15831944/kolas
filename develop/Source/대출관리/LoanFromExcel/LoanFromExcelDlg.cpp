// LoanFromExcelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoanFromExcel.h"
#include "LoanFromExcelDlg.h"
#include <direct.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LOAN_SUCCESS_LOG _T("���⼺��")
#define RETURN_SUCCESS_LOG _T("�ݳ�����")

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanFromExcelDlg dialog

CLoanFromExcelDlg::CLoanFromExcelDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoanFromExcelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CLoanFromExcelDlg::~CLoanFromExcelDlg()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CLoanFromExcelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanFromExcelDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoanFromExcelDlg, CDialog)
	//{{AFX_MSG_MAP(CLoanFromExcelDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnLOAN_FILE_SEARCH, OnbtnLOANFILESEARCH)
	ON_BN_CLICKED(IDC_btnRETURN_FILE_SEARCH, OnbtnRETURNFILESEARCH)
	ON_BN_CLICKED(IDC_btnINIT, OnbtnINIT)
	ON_BN_CLICKED(IDC_btnRUN, OnbtnRUN)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabRESULT, OnSelchangetabRESULT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanFromExcelDlg message handlers

BOOL CLoanFromExcelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
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
	InitLoanReturnDM();
	InitTabCtrl();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CLoanFromExcelDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CLoanFromExcelDlg::OnPaint() 
{
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
HCURSOR CLoanFromExcelDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CLoanFromExcelDlg::OnbtnLOANFILESEARCH() 
{
	SelectFile(0);
}

VOID CLoanFromExcelDlg::OnbtnRETURNFILESEARCH() 
{
	SelectFile(1);
}


/// ���� ����
/// nType = 0 -> ������ ����
/// nType = 1 -> �ݳ��� ����
INT CLoanFromExcelDlg::SelectFile( INT nType )
{
	CString sFileName;
	TCHAR    sCurrentDirectory[300];
	
	//  �� ���丮�� �����Ѵ�.
    _tgetcwd( sCurrentDirectory, 300 ) ;
		
    CFileDialog dlg(TRUE, _T("*.xls"), _T("*.xls"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("��������XLS Files(*.xls)|*.xls||") );
    if ( IDOK != dlg.DoModal() ) return 0;
	
	// ���� ���丮�� ����
	_tchdir( sCurrentDirectory ) ;
	
    // ���ϸ��� ���õ����ʰ� 'Ȯ��'�� click���� ���.
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return -1;

    sFileName = dlg.GetPathName();	

	if( nType == 0 )
	{
		GetDlgItem(IDC_edtLOAN_FILE_NAME)->SetWindowText(sFileName);
	}
	else
	{
		GetDlgItem(IDC_edtRETURN_FILE_NAME)->SetWindowText(sFileName);		
	}

	return 0;
}

VOID CLoanFromExcelDlg::OnbtnINIT() 
{
	InitLoanReturnDM();
	InitCtrl();
	ViewGrid();
}

INT CLoanFromExcelDlg::InitCtrl()
{
	GetDlgItem(IDC_edtLOAN_FILE_NAME)->SetWindowText(_T(""));
	GetDlgItem(IDC_edtRETURN_FILE_NAME)->SetWindowText(_T(""));
	GetDlgItem(IDC_edtPROC_DATE)->SetWindowText(_T(""));

	return 0;
}

VOID CLoanFromExcelDlg::OnbtnRUN() 
{
	AfxGetApp()->DoWaitCursor(1);

	
	INT ids = CheckRun();	
	if( ids != 0 )
		return ;


	ids = RunProc();
	if( ids != 0 )
		return ;

	ViewGrid();

	ExportExcelFile();

	AfxMessageBox(_T("ó���Ǿ����ϴ�."));


	AfxGetApp()->DoWaitCursor(-1);

}


INT CLoanFromExcelDlg::CheckRun()
{
	INT ids;

	/// 1. ���� �̸��� �ִ��� �˻�
	CString sLoanFileName , sReturnFileName;
	GetDlgItem(IDC_edtLOAN_FILE_NAME)->GetWindowText(sLoanFileName);
	GetDlgItem(IDC_edtRETURN_FILE_NAME)->GetWindowText(sReturnFileName);
	
	if( sLoanFileName.IsEmpty() && sReturnFileName.IsEmpty() )
	{
		AfxMessageBox(_T("������ �������ֽʽÿ�"));
		return 1;
	}

	/// 2. ���� ����
	InitLoanReturnDM();
	if( !sLoanFileName.IsEmpty() )
		ids = MakeLoanDMFromExcelFile(sLoanFileName);
	if( !sReturnFileName.IsEmpty() )
		ids = MakeReturnDMFromExcelFile(sReturnFileName);

	if( ids != 0 ) return 3;

	/// 3. ������ �����ϴ��� �˻�
	if( m_LoanDM.GetRowCount() == 0 && m_ReturnDM.GetRowCount() == 0 ) 
	{
		AfxMessageBox( _T("���Ͽ� ó���� �ڷᰡ �������� �ʽ��ϴ�.") );
		return 2;
	}
	

	return 0;
}

INT CLoanFromExcelDlg::RunProc()
{
	CString sProcDate,sLoanPeriod;
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( IDC_edtPROC_DATE );
	CTime tTime;
	DWORD dwResult = pDate->GetTime( tTime );		
	sProcDate.Format( _T("%04d/%02d/%02d"), tTime.GetYear(), tTime.GetMonth(), tTime.GetDay() );
	
	GetDlgItem(IDC_edtLOANPERIOD)->GetWindowText(sLoanPeriod);
	
	CLoanReturnMgr LoanReturnMgr(this , sProcDate , sLoanPeriod );
	INT ids = LoanReturnMgr.InitInfo();
	if( ids != 0 ) return 1;

	CESL_DataMgr OracleDM;
	OracleDM.SetCONNECTION_INFO(_T("kolas2@k2up/kolas2#[]"));

	/// ���� /�ݳ��� ���ÿ� ó���ϱ�
	/// ������� �ݳ�����...

	CProgressCtrl* pPrg = (CProgressCtrl*)GetDlgItem(IDC_prgLOANRETURN);
	pPrg->SetRange(0,m_LoanDM.GetRowCount()+m_ReturnDM.GetRowCount() );

	CString sProcStatus,sRegNo;
	INT nPos = 1;
	for( INT i = 0 ; i < m_LoanDM.GetRowCount() ; i++ )
	{
		sProcStatus.Empty();
		m_LoanDM.GetCellData(_T("ó������"),i,sProcStatus);
		m_LoanDM.GetCellData(_T("��Ϲ�ȣ"),i,sRegNo);

		pPrg->SetPos(nPos);
		nPos++;
		if( !sProcStatus.IsEmpty() ) continue;

		ids = LoanReturnBook( sRegNo , &OracleDM ,  &LoanReturnMgr );
		if( ids == 4007 ) 
			m_LoanDM.SetCellData( _T("ó������") , _T("DB�� �ڷᰡ �������� �ʽ��ϴ�.") , i );
		

	}

	for( i = 0 ; i < m_ReturnDM.GetRowCount() ; i++ )
	{
		sProcStatus.Empty();
		m_ReturnDM.GetCellData(_T("ó������"),i,sProcStatus);
		m_ReturnDM.GetCellData(_T("��Ϲ�ȣ"),i,sRegNo);
		
		pPrg->SetPos(nPos);
		nPos++;
		if( !sProcStatus.IsEmpty() ) continue;
		
		ids = LoanReturnBook( sRegNo , &OracleDM , &LoanReturnMgr );
		if( ids == 4007 ) 
			m_ReturnDM.SetCellData( _T("ó������") , _T("DB�� �ڷᰡ �������� �ʽ��ϴ�.") , i );
		
	}



	return 0;
}

INT CLoanFromExcelDlg::InitLoanReturnDM()
{
	m_LoanDM.FreeData();
	m_ReturnDM.FreeData();

	m_LoanDM.InitDataMgrRef(3);
	m_LoanDM.SetDataMgrRef( 0 , _T("��Ϲ�ȣ") , _T("") , _T("") , _T("") , _T("") );
	m_LoanDM.SetDataMgrRef( 1 , _T("�̿��ڹ�ȣ") , _T("") , _T("") , _T("") , _T("") );
	m_LoanDM.SetDataMgrRef( 2 , _T("ó������") , _T("") , _T("") , _T("") , _T("") );
	
	m_ReturnDM.InitDataMgrRef(3);
	m_ReturnDM.SetDataMgrRef( 0 , _T("��Ϲ�ȣ") , _T("") , _T("") , _T("") , _T("") );
	m_ReturnDM.SetDataMgrRef( 1 , _T("�̿��ڹ�ȣ") , _T("") , _T("") , _T("") , _T("") );
	m_ReturnDM.SetDataMgrRef( 2 , _T("ó������") , _T("") , _T("") , _T("") , _T("") );
	
	return 0;
}

INT CLoanFromExcelDlg::MakeLoanDMFromExcelFile( CString sFileName )
{
	INT ids;
	ids = MakeDMFromExcelFile( sFileName , &m_LoanDM );
	return ids;
}


INT CLoanFromExcelDlg::MakeReturnDMFromExcelFile( CString sFileName )
{
	INT ids;
	
	ids = MakeDMFromExcelFile( sFileName , &m_ReturnDM );
	return ids;
}

INT CLoanFromExcelDlg::MakeDMFromExcelFile(CString sFileName , CESL_DataMgr *pDM)
{
	INT ids;
	// 1. ������ �����Ѵ�.
	C_ExcelManControl ExcelCtrl;
	CRect rect;
	GetWindowRect(rect);
	
	
	CString strLastError;
	ExcelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, this, WM_USER + 10000);
	if (ExcelCtrl.m_hWnd == NULL) {
		strLastError = _T("Excelmanager.ocx�� ��ġ�Ǿ� ���� �ʽ��ϴ�. ��ġ�Ͽ� �ֽʽÿ�.");
		AfxMessageBox( strLastError);
		return 1;
	}
    CExcelManager ExcelMgr;
	ExcelMgr.SetManager(&ExcelCtrl);
	
    if(ExcelMgr.OpenManager() == FALSE) 
	{
        strLastError = _T("NOT Installed Excelmanager.ocx or No Excel!!!");
		AfxMessageBox( strLastError);
		return 2;
	}
	
	ExcelMgr.SetUseTemplate(TRUE, sFileName);
	if (ExcelMgr.OpenManager() == FALSE)
	{
		strLastError = _T("Open Failed");
		AfxMessageBox( strLastError);
		return 3;
	}
	
	// 3. �Ѷ��ξ� �о�� DM�� INSERT�Ѵ�.	
	/// 0��° �÷��� ��Ϲ�ȣ , 1��° �÷��� �̿��ڹ�ȣ�� �����Ѵ�.
	INT nColCount = 2;
	CString strItem;
	BOOL bStop = FALSE;
	BOOL bRowEmpty;	// ����ü�� ��� �ִ��� �˻�
	// ���� ������ ���� �� ��ü�� ��� �ְų� #EOF�� �����ϴ� ���̴�.
	while(TRUE)
	{	
		pDM->InsertRow(-1);
		bRowEmpty = TRUE;
		for( INT i = 0 ; i < nColCount ; i++ )
		{
			strItem = ExcelMgr.GetTextMatrix(pDM->GetRowCount() , i + 1);
			strItem.TrimLeft(); strItem.TrimRight();
			if( strItem.Compare(_T("#_TEOF"))==0)
			{
				bStop = TRUE;
				i = nColCount;
			}
			
			
			if( !bStop )
			{
				if( !strItem.IsEmpty() )
					bRowEmpty = FALSE;

				if( i == 0 && strItem.GetLength() > 3 )
				{
					ConvertRegNo(strItem);
				}
				
				ids = pDM->SetCellData( pDM->GetRowCount()-1 , i  , strItem  );
				if( ids < 0 ) return -1;
			}
		}
		if( bRowEmpty )
			bStop = TRUE;
		if( bStop ) break;
	}
	pDM->DeleteRow( pDM->GetRowCount()-1 );
	
	ExcelMgr.CloseManager();
	
	return 0;
}

INT CLoanFromExcelDlg::ConvertRegNo( CString &strItem )
{
	CString sPreRegNo = strItem;
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	strItem.Format( _T("%s%010d"),sPreRegNo.Mid(0,2) , _ttoi(sPreRegNo.Mid(2)) );

	strItem = m_pInfo->MakeRegNo(sPreRegNo.Left(nRegCodeLength) , _ttoi(sPreRegNo.Mid(nRegCodeLength)) );
	
	//=====================================================
	

	strItem.MakeUpper();

	return 0;
}

INT CLoanFromExcelDlg::LoanReturnBook( CString sRegNo , CESL_DataMgr* pOracleDM , CLoanReturnMgr* pLoanReturnMgr )
{
	INT ids;

	CString sGetOneValueQuery,sWorkingStatus;
	CString sProcStatus;
	/// �ڷ� ���¸� �˾ƺ���
	sGetOneValueQuery.Format(_T("SELECT WORKING_STATUS FROM BO_BOOK_TBL WHERE REG_NO='%s'"),sRegNo );
	
	ids = pOracleDM->GetOneValueQuery(sGetOneValueQuery,sWorkingStatus);
	if( ids < 0 && ids != -4007 ) return 1;

	if( ids == -4007 ) return 4007;

	CString sCRegNo,sUserNo;
	INT nType = CanLoan(sWorkingStatus);
	CString sErrorMsg;
	if(  nType == 0 )
	{
		for( INT i = 0 ; i < m_LoanDM.GetRowCount() ; i++ )
		{
			sProcStatus.Empty();
			m_LoanDM.GetCellData(_T("ó������"),i,sProcStatus);
			m_LoanDM.GetCellData(_T("��Ϲ�ȣ"),i,sCRegNo);
			m_LoanDM.GetCellData(_T("�̿��ڹ�ȣ"),i,sUserNo);
			
			if( !sProcStatus.IsEmpty() ) continue;
			if( sRegNo.Compare(sCRegNo) != 0 ) continue;

			ids = pLoanReturnMgr->LoanBook( sUserNo , sRegNo , sErrorMsg);
			if( ids == 0 )
			{
				m_LoanDM.SetCellData( _T("ó������") , LOAN_SUCCESS_LOG , i );
			}
			else
				m_LoanDM.SetCellData( _T("ó������") , sErrorMsg , i );

			ids = LoanReturnBook( sRegNo , pOracleDM , pLoanReturnMgr);
			if( ids == 4007 ) 
				m_LoanDM.SetCellData( _T("ó������") , _T("DB�� �ڷᰡ �������� �ʽ��ϴ�.") , i );
			break;
		}
	}
	else if( nType == 1 )
	{
		for( INT i = 0 ; i < m_ReturnDM.GetRowCount() ; i++ )
		{
			sProcStatus.Empty();
			m_ReturnDM.GetCellData(_T("ó������"),i,sProcStatus);
			m_ReturnDM.GetCellData(_T("��Ϲ�ȣ"),i,sCRegNo);
			
			if( !sProcStatus.IsEmpty() ) continue;
			if( sRegNo.Compare(sCRegNo) != 0 ) continue;
			
			ids = pLoanReturnMgr->ReturnBook( sRegNo , sErrorMsg );
			if( ids == 0 )
			{
				m_ReturnDM.SetCellData( _T("ó������") , RETURN_SUCCESS_LOG , i );
			}
			else
				m_ReturnDM.SetCellData( _T("ó������") , sErrorMsg , i );
			ids = LoanReturnBook( sRegNo , pOracleDM , pLoanReturnMgr);
			if( ids == 4007 ) 
				m_ReturnDM.SetCellData( _T("ó������") , _T("DB�� �ڷᰡ �������� �ʽ��ϴ�.") , i );
			break;
		}

	}
	else
	{
		for( INT i = 0 ; i < m_LoanDM.GetRowCount() ; i++ )
		{
			sProcStatus.Empty();
			m_LoanDM.GetCellData(_T("ó������"),i,sProcStatus);
			m_LoanDM.GetCellData(_T("��Ϲ�ȣ"),i,sCRegNo);
			m_LoanDM.GetCellData(_T("�̿��ڹ�ȣ"),i,sUserNo);
			if( !sProcStatus.IsEmpty() ) continue;
			if( sRegNo.Compare(sCRegNo) != 0 ) continue;
			m_LoanDM.SetCellData( _T("ó������") , _T("DB�� �������� �ʴ� �ڷ��Դϴ�") , i );
		}
		for( i = 0 ; i < m_ReturnDM.GetRowCount() ; i++ )
		{
			sProcStatus.Empty();
			m_LoanDM.GetCellData(_T("ó������"),i,sProcStatus);
			m_LoanDM.GetCellData(_T("��Ϲ�ȣ"),i,sCRegNo);
			m_LoanDM.GetCellData(_T("�̿��ڹ�ȣ"),i,sUserNo);
			if( !sProcStatus.IsEmpty() ) continue;
			if( sRegNo.Compare(sCRegNo) != 0 ) continue;
			m_ReturnDM.SetCellData( _T("ó������") , _T("DB�� �������� �ʴ� �ڷ��Դϴ�") , i );
		}
	}

	return 0;
}

INT CLoanFromExcelDlg::CanLoan( CString sWorkingStatus )
{
	if( sWorkingStatus.Compare(_T("BOL112N")) == 0 )
		return 0;
	else if( sWorkingStatus.Compare(_T("BOL211O")) == 0 || 
		//=====================================================
		//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
		// 18.09.27 JMJ KOL.RED.2018.008
		sWorkingStatus.Compare(_T("BOL411O")) == 0 || 
		sWorkingStatus.Compare(_T("BOL511O")) == 0 || 
		// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
		sWorkingStatus.Compare(_T("BOL611O")) == 0 || 
		//=====================================================
		sWorkingStatus.Compare(_T("BOL212O")) == 0 || 
		sWorkingStatus.Compare(_T("BOL213O")) == 0 || 
		sWorkingStatus.Compare(_T("BOL214O")) == 0 || 
		sWorkingStatus.Compare(_T("BOL215O")) == 0 
		)
		return 1;

	return 2;
}

INT CLoanFromExcelDlg::InitTabCtrl()
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabRESULT);


	pTab->InsertItem( 0 , _T("����") );
	pTab->InsertItem( 1 , _T("�ݳ�") );
	pTab->InsertItem( 2 , _T("��������α�") );
	pTab->InsertItem( 3 , _T("�ݳ������α�") );
	
	return 0;
}



INT CLoanFromExcelDlg::ViewGrid()
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabRESULT);

	INT nIndex = pTab->GetCurSel();

	CESL_DataMgr* pDM = NULL;
	if( nIndex == 0 || nIndex == 2 )
		pDM = &m_LoanDM;
	else if( nIndex == 1 || nIndex == 3)
		pDM = &m_ReturnDM;

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);

	INT nCnt = pDM->GetRowCount()+1;
	if( pDM->GetRowCount() == 0 )
		nCnt = 2;
	pGrid->Clear();
	pGrid->SetRows(nCnt);

	CString sValue;
	INT nInsertIndex = 0;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		sValue = pDM->GetCellData( i , 2 );

		if( nIndex > 1 && ( sValue.Compare(LOAN_SUCCESS_LOG)==0 || sValue.Compare(RETURN_SUCCESS_LOG ) == 0 ) ) continue;
		
		for( INT j = 0 ; j < 4 ; j++ )
		{
			if( j == 0 )
				sValue.Format( _T("%d") , nInsertIndex+1 );
			else
				sValue = pDM->GetCellData( i , j-1 );

			pGrid->SetTextMatrix( nInsertIndex+1 , j , sValue );
		}

		nInsertIndex++;
	}

	pGrid->SetTextMatrix( 0 , 1 , _T("��Ϲ�ȣ") );
	pGrid->SetTextMatrix( 0 , 2 , _T("�̿��ڹ�ȣ") );
	pGrid->SetTextMatrix( 0 , 3 , _T("ó������") );

	return 0;
}

VOID CLoanFromExcelDlg::OnSelchangetabRESULT(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ViewGrid();
	
	*pResult = 0;
}


INT CLoanFromExcelDlg::ExportExcelFile()
{
	CString sLoanFailFileName;
	CString sReturnFailFileName;

	sLoanFailFileName.Format( _T("[%s]�������.xls") , CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S")) );
	sReturnFailFileName.Format( _T("[%s]�ݳ�����.xls") , CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S")) );
	
	MakeFile( 0 , sLoanFailFileName );
	MakeFile( 1 , sReturnFailFileName );
	
	return 0;
}

INT CLoanFromExcelDlg::MakeFile( INT nType , CString sFileName )
{
	
	TCHAR TmpDir[500];
	GetCurrentDirectory(500,TmpDir);
	CRect rect;
	GetClientRect(&rect);
	
	// ���� ����
    C_ExcelManControl ExcelCtrl;
	ExcelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, this, 8805);
	if (ExcelCtrl.m_hWnd == NULL) {
		MessageBox(_T("Excelmanager.ocx�� ��ġ�Ǿ� ���� �ʽ��ϴ�. ��ġ�Ͽ� �ֽʽÿ�."), _T("Warnning!"), MB_ICONEXCLAMATION);
		return 1;
	}
    CExcelManager ExcelManager(&ExcelCtrl);
    if(ExcelManager.OpenManager() == FALSE) {
        MessageBox(_T("NOT Installed Excelmanager.ocx or No Excel!!!"), _T("Warnning!"), MB_ICONEXCLAMATION);
		return 2;
	}
	
	// ���� �Է�
	CESL_DataMgr* pDM = NULL;
	if( nType == 0 )
		pDM = &m_LoanDM;
	else if( nType == 1)
		pDM = &m_ReturnDM;
		
	CString sValue;
	INT nInsertIndex = 0;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		sValue = pDM->GetCellData( i , 2 );
		
		if( sValue.Compare(LOAN_SUCCESS_LOG)==0 || sValue.Compare(RETURN_SUCCESS_LOG ) == 0 ) continue;
		
		for( INT j = 0 ; j < 3 ; j++ )
		{
			sValue = pDM->GetCellData( i , j );
			
			ExcelManager.SetTextMatrix( nInsertIndex+1 , j+1 , sValue );		
		}
		
		nInsertIndex++;
	}

	CString sFullPath;
	sFullPath.Format(_T("%s\\%s"),TmpDir,sFileName);
	
	if( nInsertIndex > 0 )
		ExcelManager.SaveAs(sFullPath);
	
	// ����� ���� ��ü�� ��ȯ�Ѵ�.
    ExcelManager.CloseManager();
	
	SetCurrentDirectory(TmpDir);

	if( nInsertIndex != 0 )
	{
		CString sMsg;
		if( nType == 0 )
			sMsg.Format(_T("����� ������ �αװ� '%s'���Ͽ� ����Ǿ����ϴ�.") , sFileName );
		else
			sMsg.Format(_T("�ݳ��� ������ �αװ� '%s'���Ͽ� ����Ǿ����ϴ�.") , sFileName );
		
		AfxMessageBox(sMsg);

	}

	
	return 0;
}

HBRUSH CLoanFromExcelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// ������� �߰�

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
