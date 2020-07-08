// SIReportTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIReportTester.h"
#include "SIReportTesterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Kolas2up_CardList.h"
#include "Kolas2up_BookCardList.h"
#include "Kolas2up_SerialCardList.h"
#include "ESL_Mgr.h"

/////////////////////////////////////////////////////////////////////////////
// CSIReportTesterDlg dialog

CSIReportTesterDlg::CSIReportTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSIReportTesterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSIReportTesterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CONNECTION_INFO = _T("kolas2@kolas2/kolas2#[]");
}

VOID CSIReportTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIReportTesterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSIReportTesterDlg, CDialog)
	//{{AFX_MSG_MAP(CSIReportTesterDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bPRINT001, OnbPRINT001)
	ON_BN_CLICKED(IDC_bPRINT002, OnbPRINT002)
	ON_BN_CLICKED(IDC_bPRINT003, OnbPRINT003)
	ON_BN_CLICKED(IDC_bPRINT004, OnbPRINT004)
	ON_BN_CLICKED(IDC_bTEST001, OnbTEST001)
	ON_BN_CLICKED(IDC_bPRINT005, OnbPRINT005)
	ON_BN_CLICKED(IDC_bPRINT006, OnbPRINT006)
	ON_BN_CLICKED(IDC_bPRINT007, OnbPRINT007)
	ON_BN_CLICKED(IDC_bPRINT008, OnbPRINT008)
	ON_BN_CLICKED(IDC_bTEST002, OnbTEST002)
	ON_BN_CLICKED(IDC_bPRINT009, OnbPRINT009)
	ON_BN_CLICKED(IDC_bPRINT010, OnbPRINT010)
	ON_BN_CLICKED(IDC_bTEST003, OnbTEST003)
	ON_BN_CLICKED(IDC_bPRINT011, OnbPRINT011)
	ON_BN_CLICKED(IDC_bPRINT012, OnbPRINT012)
	ON_BN_CLICKED(IDC_bPRINT100, OnbPRINT100)
	ON_BN_CLICKED(IDC_bPRINT4, OnbPRINT4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIReportTesterDlg message handlers

BOOL CSIReportTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CSIReportTesterDlg::OnPaint() 
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
HCURSOR CSIReportTesterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

INT CSIReportTesterDlg::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}

VOID CSIReportTesterDlg::OnbPRINT001() 
{
	// �Ϲ����� �ݺ��� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("LIGHT_�ֹ��ڷ���"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}
	CESL_DataMgr *pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(CONNECTION_INFO);
	CString strDMPK = FindDMPK(_T("DM_�ֹ����ϰ���"));
	ids = pDM->InitDMPKFromFile(strDMPK);
	ids = pDM->RefreshDataManager(_T("rownum < 50"));
	INT nRow = pDM->GetRowCount();
	pSIReportManager->SetDataMgr(0, pDM, 0);
	pDM->SetCellData(_T("IBS_��ǥ��"), _T("\r\n\r\n\r\nA\r\nB\r\nC\r\n\r\n\r\nD"), 1);

	pSIReportManager->m_nUserPaperSize = -9999;
	pSIReportManager->m_nOrientation = DMORIENT_PORTRAIT;
	pSIReportManager->m_nResolution = 1728;

	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}	
}

CString CSIReportTesterDlg::FindDMPK(CString strDMAlias)
{
	CString strFileName;
	strFileName.Format(_T("..\\CFG\\Reference\\DM\\DMPK.CFG"));
	CStdioFile fd;
	if (fd.Open(strFileName, CStdioFile::modeRead | CStdioFile::shareDenyWrite | CFile::typeBinary) == 0) return -1;	// ���������� ã�� �� ����
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	TCHAR szBuf[16000];
	CString strLine;
	INT pos;
	

	INT nStart = 0;
	CString PK, ALIAS;
	CString result = _T("");
	while(TRUE) {
		if (fd.ReadString(szBuf, 16000) == NULL) break;
		if (szBuf[0] == '#') continue;
		strLine.Format(_T("%s"), szBuf);
		strLine.TrimLeft();			strLine.TrimRight();
		
		nStart = 0;

		pos = strLine.Find('|', nStart);
		PK = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		ALIAS = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		PK.TrimLeft();		PK.TrimRight();
		ALIAS.TrimLeft();	ALIAS.TrimRight();

		if (ALIAS == strDMAlias) {
			result = PK;
			break;
		}
	}
	fd.Close();
	return result;
}

VOID CSIReportTesterDlg::OnOK() 
{
	CDialog::OnOK();
}

VOID CSIReportTesterDlg::OnbPRINT002() 
{
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����-���ڵ����"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}

	CESL_DataMgr *pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(CONNECTION_INFO);
	pDM->InitDataMgrRef(1);
	pDM->SetDataMgrRef(0, _T("BB_��Ϲ�ȣ"), _T("REG_NO"), _T("str"), _T(""), _T(""));
	pDM->TBL_NAME = _T("BO_BOOK_TBL");
	ids = pDM->RefreshDataManager(_T("rownum <= 50"));
	CString str;
	pDM->GetCellData(_T("BB_��Ϲ�ȣ"), 0, str);
	INT nRowCount = pDM->GetRowCount();

	pSIReportManager->SetTempVariable(_T("%BARCODE���ڵ�����%"), _T("����(�̾���)������"));
	
	pSIReportManager->SetDataMgr(0, pDM);
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}
}

VOID CSIReportTesterDlg::OnbPRINT003() 
{
	// ��ũ�� �ݺ� & ��� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->m_nDefaultPaperType = PAPER_A4_LANDSCAPE;

	CESL_DataMgr *pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(CONNECTION_INFO);
	CString strDMPK = FindDMPK(_T("DM_�ֹ����ϰ���"));
	ids = pDM->InitDMPKFromFile(strDMPK);
	ids = pDM->RefreshDataManager(_T("rownum < 50"));
	INT nRow = pDM->GetRowCount();
	pDM->SetCellData(_T("IBS_��ǥ��"), _T("\r\n\r\n\r\nA\r\nB\r\nC\r\n\r\n\r\nD"), 1);

	ids = pSIReportManager->DeleteTempPageFiles();

	pSIReportManager->SetCONNECTION_INFO(CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("LIGHT_�ֹ��ڷ���"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}

	for (INT i = 0; i < 2; i++) {
		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// ��ũ�� Manual �ݺ� ���
	}

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}	
}

VOID CSIReportTesterDlg::OnbPRINT004() 
{
	// �Ұ�, �Ѱ� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("LIGHT_����_�Լ���Ȳ"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}
	CESL_DataMgr *pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(CONNECTION_INFO);
	CString strDMPK = FindDMPK(_T("DM_�Լ���Ȳ"));
	ids = pDM->InitDMPKFromFile(strDMPK);
	ids = pDM->RefreshDataManager(_T("rownum < 50"));
	INT nRow = pDM->GetRowCount();
	pSIReportManager->SetDataMgr(0, pDM, 0);

	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}	
}

VOID CSIReportTesterDlg::OnbTEST001() 
{
	INT nPos;
	CString str1 = _T("�����ʰ��̵漼�����ǽ�ȿ���м������ѿ���");
	CString str2 = _T("�ѱ���Ȱ��ڹ��� �����Ȱ�� ������Ȱ�� ������Ȱ�� ��������Ȱ�� �Ŷ��Ȱ��");
	
	nPos = 26;
	BOOL flag1 = IsCanCut(str1, nPos-1);
	if (!flag1) nPos++;
	CString strTmp1 = str1.Left(nPos);
	CString strTmp2 = str1.Mid(nPos);

	nPos = 26;
	BOOL flag2 = IsCanCut(str2, nPos-1);
	if (!flag2) nPos++;
	CString strTmp3 = str2.Left(nPos);
	CString strTmp4 = str2.Mid(nPos);
}

BOOL CSIReportTesterDlg::IsCanCut(CString sStr, INT nPos)
{
	INT i, nLastPos = -1;
	//���ڼ� �ʰ��� FALSE
	if(nPos > sStr.GetLength())
		return FALSE;
	TCHAR ch = sStr.GetAt(nPos);
	//������ ���� || �ѱ��� �ƴϸ� TRUE
	if(nPos + 1 == sStr.GetLength() || (ch & (0x80)) != 128)
		return TRUE;
	//ù���ڰ� 1byte���ڸ� TRUE �ƴϸ� FALSE
	if(nPos == 0)
		if((ch & (0x80)) != 128)
			return TRUE;
		else
			return FALSE;

	for(i = 0; i < nPos; i++) {
		ch = sStr.GetAt(i);
		//�ڸ��� �κп��� ���� �����(��������)������ ��ġ
		if((ch & (0x80)) != 128)
			nLastPos = i;
	}
	if (nLastPos == -1 && nPos % 2 == 1) return TRUE;
	if (nLastPos == -1 && nPos % 2 == 0) return FALSE;
	//¦���� ���̳��� TRUE
	BOOL result = !((nPos - nLastPos) % 2);
	return result;
}

VOID CSIReportTesterDlg::OnbPRINT005() 
{
	// �Ϲ����� �ݺ��� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����ũ��"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}
	CESL_DataMgr *pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(CONNECTION_INFO);
	CString strDMPK = FindDMPK(_T("DM_�ֹ����ϰ���"));
	ids = pDM->InitDMPKFromFile(strDMPK);
	pSIReportManager->SetDataMgr(0, pDM, 0);

	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}			
}

VOID CSIReportTesterDlg::OnbPRINT006() 
{
	// �Ϲ����� �ݺ��� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����ũ��2"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}
	CESL_DataMgr *pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(CONNECTION_INFO);
	CString strDMPK = FindDMPK(_T("DM_�ֹ����ϰ���"));
	ids = pDM->InitDMPKFromFile(strDMPK);
	pSIReportManager->SetDataMgr(0, pDM, 0);

	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}			
}

VOID CSIReportTesterDlg::OnbPRINT007() 
{
	// �Ϲ����� �ݺ��� ���(B4 ���� �׽�Ʈ)
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("LIGHT_�ֹ��ڷ���-B4"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}
	CESL_DataMgr *pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(CONNECTION_INFO);
	CString strDMPK = FindDMPK(_T("DM_�ֹ����ϰ���"));
	ids = pDM->InitDMPKFromFile(strDMPK);
	ids = pDM->RefreshDataManager(_T("rownum < 50"));
	INT nRow = pDM->GetRowCount();
	pSIReportManager->SetDataMgr(0, pDM, 0);
	pDM->SetCellData(_T("IBS_��ǥ��"), _T("\r\n\r\n\r\nA\r\nB\r\nC\r\n\r\n\r\nD"), 1);

	pSIReportManager->m_nUserPaperSize = -9999;
	pSIReportManager->m_nOrientation = DMORIENT_PORTRAIT;
	pSIReportManager->m_nResolution = 1728;
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}		
}

VOID CSIReportTesterDlg::OnbPRINT008() 
{
	// �Ϲ����� �ݺ��� ���(B4 ���� �׽�Ʈ)
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("LIGHT_�ֹ��ڷ���-B4-����"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}
	CESL_DataMgr *pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(CONNECTION_INFO);
	CString strDMPK = FindDMPK(_T("DM_�ֹ����ϰ���"));
	ids = pDM->InitDMPKFromFile(strDMPK);
	ids = pDM->RefreshDataManager(_T("rownum < 50"));
	INT nRow = pDM->GetRowCount();
	pSIReportManager->SetDataMgr(0, pDM, 0);
	pDM->SetCellData(_T("IBS_��ǥ��"), _T("\r\n\r\n\r\nA\r\nB\r\nC\r\n\r\n\r\nD"), 1);

	pSIReportManager->m_nUserPaperSize = DMPAPER_FANFOLD_US;
	pSIReportManager->m_nOrientation = DMORIENT_LANDSCAPE;
	pSIReportManager->m_nResolution = 1728;
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}			
}

VOID CSIReportTesterDlg::OnbTEST002() 
{
	INT ids;

	// XML �� WorkForm��ü�� �����Ѵ�.
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	
	if (pXMLIOMgr == NULL) {
		AfxMessageBox(_T("XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}
	if (pPageWorkForm == NULL) {
		AfxMessageBox(_T("�߰� XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}

	// ���� ������ ������ ��ȣ�� �����´�.
	INT nMaxTmpPageNum = pXMLIOMgr->GetMaxTmpPageNum();
	if (nMaxTmpPageNum <= 0) {
		AfxMessageBox(_T("������ ������ ã�� �� �����ϴ�."));
		return;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// ���� ������ ������ ������ �ҷ��´�.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) {
		AfxMessageBox(_T("�������� �ε� ���� ���Ͽ����ϴ�."));
		return;
	}
	// �ҷ��� WorkForm�߿��� First Sheet�� �����´�.(�߰� ������ ���Ͽ��� Sheet�� �Ѱ����̴�.)
	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) {
		AfxMessageBox(_T("�������� ã�� �� �����ϴ�."));
		return;
	}
	// ������ ������ ó���� �̹� �Ǿ� �ִ��� ����. �̹� ó���Ǿ� �ִٸ� �ι� �� �ʿ� ����.
	CSIReportObject * pObject = pSheet->FindObject(_T("�߰��ȸ�����������"));
	if (pObject) return;
	// �ܺ����̺� �� �ݺ� ���̺��� �����´�.(���̺� �̸��� ��Ȳ�� ���� ������ ��)
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_17"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_43"));
	if (pIn == NULL) return;
	if (pOut == NULL) return;

	CRect InLeft, InRight, OutLeft, OutRight, NewRect;

	CSIReportObject_Table_Cell *pCell;

	// �ݺ����̺��� ������ ������ ���� ó���� ������ ���������� Rect�� ���ϰ�
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, 0);
	if (pCell) InLeft = pCell->GetRect();
	
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, pIn->col_count-1);
	if (pCell) InRight = pCell->GetRect();

	// �ܺ����̺��� ������ ������ ���� ó�� ���� Rect�� ���Ѵ�.
	pCell = NULL;
	pCell = pOut->GetCell(pOut->row_count-1, 0);
	if (pCell) OutLeft = pCell->GetRect();

	// �ܺ����̺��� �ݺ����̺��� ������ ������ �������� �߰����ش�.
	if (InLeft.bottom < OutLeft.bottom) {
		// ������ �������� Rect�� ����Ѵ�.
		NewRect.top = InLeft.bottom;
		NewRect.bottom = InLeft.bottom;
		NewRect.left = InLeft.left;
		NewRect.right = InRight.right;

		// ������ �������� �߰� ������ ���Ͽ� �߰��Ѵ�.
		CSIReportObject_Line *pLine = (CSIReportObject_Line *)pSheet->AddNewObject(OBJECT_LINE);
		if (pLine) {
			pLine->SetName(_T("�߰��ȸ�����������"));
			pLine->SetRect(NewRect);
		}

		// ����� �߰������� ������ �����Ѵ�.
		ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	}

	// ������ Object�� ����
	if (pXMLIOMgr) {
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}
	if (pPageWorkForm) {
		delete pPageWorkForm;
		pPageWorkForm = NULL;
	}

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
}

VOID CSIReportTesterDlg::OnbPRINT011() 
{
	INT nMAX_BOOKCARD_LINE = 13;		// �Ѱ��� ��ī��� ���� ���μ�
	CKolas2up_BookCardList list;
	list.m_nCols = 3;
	list.m_nRows = 4;
	INT i, j;
	CString str;
	for (i = 0; i < 18; i++) {
		CKolas2up_BookCard *pCard = list.AddCard(nMAX_BOOKCARD_LINE);
		if (pCard == NULL) continue;

		for (j = 0; j < nMAX_BOOKCARD_LINE; j++) {
			str.Format(_T("%d : LINE %02d"), i, j);
			pCard->SetAt(j, str);
		}
	}

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->m_nDefaultPaperType = PAPER_A4_LANDSCAPE;
	ids = pSIReportManager->DeleteTempPageFiles();

	// ��ī�� ��¹� ����
	ids = list.Print(pSIReportManager);

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

}

VOID CSIReportTesterDlg::OnbPRINT010() 
{
	// ��ũ�� �ݺ� & ��� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->m_nDefaultPaperType = PAPER_A4_LANDSCAPE;

	ids = pSIReportManager->DeleteTempPageFiles();

	pSIReportManager->SetCONNECTION_INFO(CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("���1"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}

	CString str;
	for (INT i = 0; i < 2; i++) {
		str.Format(_T("\r\n\r\nAAAA\r\n             EM"));
		pSIReportManager->SetTempVariable(_T("���ڿ�01"), str);
		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// ��ũ�� Manual �ݺ� ���
	}

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
}

VOID CSIReportTesterDlg::OnbTEST003() 
{
	CESL_DataMgr *pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(CONNECTION_INFO);

	pDM->RestructDataManager(_T("SELECT * FROM CO_ZIP_CODE_TBL"));
	
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}
	AfxMessageBox(_T("Finish"));
}

VOID CSIReportTesterDlg::OnbPRINT009() 
{
	// ��ī���� ���
	
	CKolas2up_CardList list;
	INT i, j;
	CString str;
	for (i = 0; i < 10; i++) {
		CKolas2up_Card *pCard = list.AddCard();
		if (pCard == NULL) continue;

		for (j = 0; j < 16; j++) {
			str.Format(_T("%d : LINE %02d"), i, j);
			pCard->SetAt(j, str);
		}
	}

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	ids = pSIReportManager->DeleteTempPageFiles();

	ids = list.Print(pSIReportManager);

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
}

VOID CSIReportTesterDlg::OnbPRINT012() 
{
	INT ids;

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}

	pSIReportManager->m_nResolution = 1728;

	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	ids = pPageWorkForm->AddNewSheet();

	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	pPageWorkForm->paper = PAPER_FANFOLD_US_PORTRAIT;
	//pPageWorkForm->paper = PAPER_A4_PORTRAIT;

	CSIReportObject_Table *pTable1 = (CSIReportObject_Table *)pSheet->AddNewObject(OBJECT_TABLE);
	pTable1->horz_line = TRUE;
	pTable1->vert_line = TRUE;
	pTable1->InitCell(1, 1);
	CSIReportObject_Table_Cell *pCell1 = pTable1->GetCell(0, 0);
	CRect rect1;
	rect1.top = 100;
	rect1.bottom = 300;
	rect1.left = 100;
	rect1.right = 300;
	pCell1->SetRect(rect1);
	pCell1->text.text = _T("�׽�ƮTEST�����");

	ids = pSIReportManager->DeleteTempPageFiles();

	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	if (pXMLIOMgr == NULL) return;

	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page0001.xml"));
	pCell1->text.text = _T("�׽�ƮTEST����� 001");
	ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	strFileName.Format(_T("rpt_tmp\\page0002.xml"));
	pCell1->text.text = _T("�׽�ƮTEST����� 002");
	ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	strFileName.Format(_T("rpt_tmp\\page0003.xml"));
	pCell1->text.text = _T("�׽�ƮTEST����� 003");
	ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	if (pXMLIOMgr) {
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}

	ids = pSIReportManager->Print2();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	if (pPageWorkForm) {
		delete pPageWorkForm;
		pPageWorkForm = NULL;
	}
	return;
}

VOID CSIReportTesterDlg::OnbPRINT100() 
{
	CKolas2up_SerialCardList SCL;		// SCL is _T("Serial Card List")

	SCL.SetRows(3);
	SCL.SetCols(2);

	INT nSerialCardMaxLine = 16;
	INT i, j, ids;

	CKolas2up_SerialCard *pCard;
	CString str;
	for (i = 0; i < 1; i++) {
		pCard = SCL.AddCard(nSerialCardMaxLine);
		for (j = 0; j < nSerialCardMaxLine; j++) {
			str.Format(_T("CARD%02d - %04d"), i, j);
			pCard->SetAt(j, str);
		}
	}

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}

	ids = pSIReportManager->DeleteTempPageFiles();
	ids = SCL.Print(pSIReportManager);
	ids = pSIReportManager->Print2();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	return;
}

VOID CSIReportTesterDlg::OnbPRINT4() 
{
	INT j, k;
	CString str;
	CSIReportObject_Table_Cell * pCell;
	CSIReportObject_Text_Box *pTextBox;
	//CSIReportObject_Text_Box *pData;
	CSIReportObject_Text_Box *pData;
	for (INT i = 0; i <50; i++) {
		CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
		pPageWorkForm->AddNewSheet();

		CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
		CSIReportObject_Table * ptmpObject = (CSIReportObject_Table *)pSheet->AddNewObject(OBJECT_TABLE);
		ptmpObject->SetName(_T("TEST"));
		ptmpObject->horz_line = TRUE;
		ptmpObject->vert_line = TRUE;
		ptmpObject->row_count = 0;
		ptmpObject->col_count = 9;
		
		for (j = 0; j < 20; j++) {
			ptmpObject->AddRow();
			str.Format(_T("%d : %d"), i, j);
			SetWindowText(str);
		}
		//ptmpObject->FreeCell();
		delete pPageWorkForm;
	}
}
