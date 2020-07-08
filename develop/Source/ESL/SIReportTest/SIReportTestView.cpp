// SIReportTestView.cpp : implementation of the CSIReportTestView class
//

#include "stdafx.h"
#include "SIReportTest.h"

#include "SIReportTestDoc.h"
#include "SIReportTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestView

IMPLEMENT_DYNCREATE(CSIReportTestView, CFormView)

BEGIN_MESSAGE_MAP(CSIReportTestView, CFormView)
	//{{AFX_MSG_MAP(CSIReportTestView)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_NEW, OnFind)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestView construction/destruction

CSIReportTestView::CSIReportTestView()
	: CFormView(CSIReportTestView::IDD)
{
	//{{AFX_DATA_INIT(CSIReportTestView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_pos = 0;

	// DataMgr �ʱ�ȭ
	m_pDM = new CESL_DataMgr;
	m_pDM->SetCONNECTION_INFO(_T("kolas2@kolas2/kolas2#[]"));
	m_pDM->DB_MGR_ALIAS = _T("DM_�׽�Ʈ");
	m_pDM->TBL_NAME = _T("ESL_TEST");

	m_pDM->InitDataMgrRef(7);
	m_pDM->SetDataMgrRef(0, _T("���̵�"),	_T("ID"),			_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(1, _T("��ȣ"),		_T("PASSWORD"),		_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(2, _T("�̸�"),		_T("NAME"),			_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(3, _T("�ּ�"),		_T("ADDRESS"),		_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(4, _T("��ȭ��ȣ"), _T("TEL"),			_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(5, _T("����"),		_T("BIRTHDAY"),		_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(6, _T("����"),		_T("GRADE"),		_T("str"), _T(""), _T(""));

	m_strRptName = _T("");
}

CSIReportTestView::~CSIReportTestView()
{
	if (m_pDM) {
		delete m_pDM;
		m_pDM = NULL;
	}
}

VOID CSIReportTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIReportTestView)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_MSHFLEXGRID1, m_Grid);
	DDX_Control(pDX, IDC_EXPLORER1, m_Browser);
	//}}AFX_DATA_MAP
}

BOOL CSIReportTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

VOID CSIReportTestView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	InitTabControl();

	m_Grid.Clear();
	m_Grid.SetRows(2);
	m_Grid.SetCols(0, 8);

	SetColumnInfo(0, _T(""), 40, 4);
	SetColumnInfo(1, _T("���̵�"),		 70, 4);
	SetColumnInfo(2, _T("��ȣ"),		 70, 4);
	SetColumnInfo(3, _T("�̸�"),		70, 4);
	SetColumnInfo(4, _T("�ּ�"),		200, 4);
	SetColumnInfo(5, _T("��ȭ��ȣ"),	100, 4);
	SetColumnInfo(6, _T("����"),		 90, 4);
	SetColumnInfo(7, _T("����"),		 50, 4);
	UpdateData(false);
}

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestView printing

BOOL CSIReportTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

VOID CSIReportTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

VOID CSIReportTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

VOID CSIReportTestView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestView diagnostics

#ifdef _DEBUG
VOID CSIReportTestView::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CSIReportTestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSIReportTestDoc* CSIReportTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSIReportTestDoc)));
	return (CSIReportTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestView message handlers

INT CSIReportTestView::InitTabControl(VOID)
{
	TC_ITEM item;

	item.mask = TCIF_IMAGE | TCIF_TEXT;
	item.iImage = 0;

	item.pszText = _T("Data");
	m_tab.InsertItem(0, &item);

	item.pszText = _T("Xml");
	m_tab.InsertItem(1, &item);
	UpdateData(false);

	return 0;
}

VOID CSIReportTestView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	INT pos = 0;

	RECT r;
	GetClientRect(&r);
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_TAB1);
	if (pWnd) {
		pWnd->MoveWindow(&r, true);
		CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
		pWnd->GetClientRect(&r);
	}
	r.top += 25;
	r.left += 5;
	r.right -= 5;
	r.bottom -= 5;

	CWnd *pGrid = GetDlgItem(IDC_MSHFLEXGRID1);
	CWnd *pBrowser = GetDlgItem(IDC_EXPLORER1);
	if (m_pos == 0) {
		if (pGrid) pGrid->MoveWindow(&r, true);
		if (pBrowser) pBrowser->MoveWindow(0, 0, 0, 0, true);
	} else {
		if (pBrowser) pBrowser->MoveWindow(&r, true);
		if (pGrid) pGrid->MoveWindow(0, 0, 0, 0, true);
	}
}

VOID CSIReportTestView::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(true);
	m_pos = m_tab.GetCurSel();

	CWnd *pGrid = GetDlgItem(IDC_MSHFLEXGRID1);
	CWnd *pBrowser = GetDlgItem(IDC_EXPLORER1);

	RECT r;
	GetClientRect(&r);
	r.top += 25;
	r.left += 5;
	r.right -= 5;
	r.bottom -= 5;

	if (!pGrid) return;
	if (!pBrowser) return;
	if (m_pos == 0) {
		pGrid->MoveWindow(&r, true);
		pBrowser->MoveWindow(0, 0, 0, 0, true);
	} else {
		pBrowser->MoveWindow(&r, true);
		pGrid->MoveWindow(0, 0, 0, 0, true);
	}
	if (pResult) *pResult = 0;
}

VOID CSIReportTestView::OnFind(VOID)
{
	INT ids = m_pDM->RefreshDataManager(_T("ID IS NOT NULL ORDER BY ID"));
	INT i;
	CString strGrade;
	for (i = 0; i < m_pDM->GetRowCount(); i++) {
		strGrade = ConvertCodeToDesc_Grade(m_pDM->GetCellData(_T("����"), i));
		m_pDM->SetCellData(_T("����"), strGrade, i);
	}


	INT nRow = m_pDM->GetRowCount();
	m_Grid.Clear();
	m_Grid.SetRows(2);
	m_Grid.SetCols(0, 8);

	SetColumnInfo(0, _T(""), 40, 4);
	SetColumnInfo(1, _T("���̵�"),		 70, 4);
	SetColumnInfo(2, _T("��ȣ"),		 70, 4);
	SetColumnInfo(3, _T("�̸�"),		70, 4);
	SetColumnInfo(4, _T("�ּ�"),		200, 4);
	SetColumnInfo(5, _T("��ȭ��ȣ"),	100, 4);
	SetColumnInfo(6, _T("����"),		 90, 4);
	SetColumnInfo(7, _T("����"),		 50, 4);
	UpdateData(false);
	if (nRow == 0) {
		AfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return;
	}
	
	m_Grid.SetRows(nRow+1);
	CString strMsg;
	for (i = 0; i < nRow; i++) {
		strMsg.Format(_T("%d"), i+1);
		m_Grid.SetTextMatrix(i+1, 0, strMsg);
		m_Grid.SetTextMatrix(i+1, 1, m_pDM->GetCellData(_T("���̵�"), i));
		m_Grid.SetTextMatrix(i+1, 2, m_pDM->GetCellData(_T("��ȣ"), i));
		m_Grid.SetTextMatrix(i+1, 3, m_pDM->GetCellData(_T("�̸�"), i));
		m_Grid.SetTextMatrix(i+1, 4, m_pDM->GetCellData(_T("�ּ�"), i));
		m_Grid.SetTextMatrix(i+1, 5, m_pDM->GetCellData(_T("��ȭ��ȣ"), i));
		m_Grid.SetTextMatrix(i+1, 6, m_pDM->GetCellData(_T("����"), i));
		m_Grid.SetTextMatrix(i+1, 7, m_pDM->GetCellData(_T("����"), i));
	}
	UpdateData(false);
}

INT CSIReportTestView::SetColumnInfo(INT col, CString str, INT width, INT alignment)
{
	m_Grid.SetColAlignmentFixed(col, 4);
	m_Grid.SetTextMatrix(0, col, str);
	m_Grid.SetColWidth(col, 0, width*15);
	m_Grid.SetColAlignment(col, alignment);
	
	return 0;
}

VOID CSIReportTestView::OnFileOpen(VOID)
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");

	wmemset(&OFN, 0, sizeof(OPENFILENAME));

	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFilter = _T("RPT����(*.rpt)\0*.rpt\0�������(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("RPT ���� ����...");
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.lpstrDefExt = _T("rpt");
	OFN.Flags = OFN_OVERWRITEPROMPT;

	if (::GetOpenFileName(&OFN) != 0)
	{
		CString strFileName;
		strFileName = OFN.lpstrFile;
		m_strRptName = strFileName;

		m_Browser.Navigate(strFileName, NULL, NULL, NULL, NULL);
		m_tab.SetCurSel(1);
		UpdateData(false);
		OnSelchangeTab1(NULL, NULL);
	}
}


VOID CSIReportTestView::OnFileSave(VOID)
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");

	wmemset(&OFN, 0, sizeof(OPENFILENAME));

	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFilter = _T("RPT����(*.rpt)\0*.rpt\0�������(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("RPT ���� DB�� ����");
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.lpstrDefExt = _T("rpt");
	OFN.Flags = OFN_OVERWRITEPROMPT;

	if (::GetOpenFileName(&OFN) == 0) return;
	CString strFileName = OFN.lpstrFile;

	if (AfxMessageBox(_T("������ DB�� Upload �ϴ� �۾��� �ణ �����ɸ��� �ֽ��ϴ�. ��� �Ͻðڽ��ϱ�?"), MB_YESNO) != IDYES) return;
	CTime t = CTime::GetCurrentTime();
	CString cur_date;
	cur_date.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	CFile fd;
	fd.Open(strFileName, CFile::modeRead | CFile::typeBinary);
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString query;
	CString PK;
	m_pDM->MakeRecKey(PK, _T("esl_seq"));
	query.Format(_T("INSERT INTO ESL_SIREPORT_TBL (PK, FLAG, RPTALIAS, ENGINEER, DESCRIP, RPTSIZE, FIRST_DATE, LAST_DATE) VALUES ")
		         _T("(%s, 'X', 'SI REPORT TEST', 'wjchoi', 'wjchoi@eco.co.kr', %d, '%s', '%s');"),
				 PK, fd.GetLength(), cur_date, cur_date);

	m_pDM->ExecuteQuery(query, true);
	
	BYTE *pBuffer = (BYTE *)malloc(sizeof(BYTE) * fd.GetLength() + 64);
	if (pBuffer == NULL) {
		AfxMessageBox(_T("�޸� �Ҵ� ����"));
		return;
	}
	INT BufferSize = sizeof(BYTE) * fd.GetLength() + 64;
	fd.ReadHuge(pBuffer, fd.GetLength());

	INT ids = m_pDM->LOBUpdate(_T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), PK, _T("RPTDATA"), pBuffer, fd.GetLength());
	if (ids < 0) {
		AfxMessageBox(_T("������ DB�� �ø����� ������ �߻��Ͽ����ϴ�."));
	} else {
		AfxMessageBox(_T("������ DB�� ���������� Upload �Ͽ����ϴ�."));
	}
	if (pBuffer) {
		free(pBuffer);
		pBuffer = NULL;
	}
}

VOID CSIReportTestView::OnFilePrint(VOID)
{
	if (m_strRptName.GetLength() == 0) m_strRptName = _T("sireporttest2.rpt");

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("SI REPORT TEST"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	pSIReportManager->SetDataMgr(m_pDM);
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
}

INT CSIReportTestView::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	CString msg;
	msg.Format(_T("- %d/%d -"), EventInfo->m_nPage, EventInfo->m_nTotalPage);


	//INT horz_align;							// ����(����:ALIGN_LEFT, �߾�:ALIGN_HCENTER, ������:ALIGN_RIGHT)
	//INT vert_align;							// ����(��:ALIGN_TOP, ���:ALIGN_VCENTER, �Ʒ�:ALIGN_BOTTOM)
	//pSIReportManager->SetTableCellText(_T("��������ȣ"), 0, 0, msg, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	pSIReportManager->SetTableCellText(_T("��������ȣ"), 0, 0, msg, ALIGN_VCENTER, ALIGN_HCENTER);
	if (EventInfo->m_nPage == 1) pSIReportManager->SetBarcodeCaption(_T("���ڵ�"), _T("123"));
	if (EventInfo->m_nPage == 2) pSIReportManager->SetBarcodeCaption(_T("���ڵ�"), _T("123456"));
	if (EventInfo->m_nPage == 3) pSIReportManager->SetBarcodeCaption(_T("���ڵ�"), _T("123456789"));
	return 0;
}

CString CSIReportTestView::ConvertCodeToDesc_Grade(CString code)
{
	code.TrimLeft();		code.TrimRight();
	if (code == _T("1")) return _T("����");
	if (code == _T("2")) return _T("����");
	if (code == _T("3")) return _T("����");
	if (code == _T("4")) return _T("����");
	if (code == _T("5")) return _T("����");
	return _T("�������");
}

INT CSIReportTestView::GetSIReportFileFromDB(CString RptName, CString tmpFilename, CString &PK)
{
	CESL_DataMgr *pDM = new CESL_DataMgr;
	if (!pDM) return -1000;
	pDM->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	INT i, ids;
	CString szBuf, sQuery;
	CString strPK = _T(""), strRPTSize;

	sQuery.Format(_T("SELECT PK, RPTSIZE, FLAG, BITMAP_ALIAS FROM ESL_SIREPORT_TBL WHERE RPTALIAS='%s'"), RptName);
	pDM->RestructDataManager(sQuery);
	INT nRow = pDM->GetRowCount();

	// ����� ���� ������ �ִ��� ã�ƺ���. ������ �̰��� ���� ���ĺ��� �켱�Ѵ�.
	for (i = 0; i < nRow; i++) {
		pDM->GetCellData(i, 2, szBuf);
		szBuf.TrimLeft();			szBuf.TrimRight();
		szBuf.MakeUpper();
		if (szBuf == _T("U")) {
			pDM->GetCellData(i, 0, strPK);
			strPK.TrimLeft();		strPK.TrimRight();
			pDM->GetCellData(i, 1, strRPTSize);
			strRPTSize.TrimLeft();	strRPTSize.TrimRight();
			if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
			break;
		}
	}
	// ����� ���� ������ �������� ���� ������ ã�´�.
	if (strPK.GetLength() == 0) {
		for (i = 0; i < nRow; i++) {
			pDM->GetCellData(i, 2, szBuf);
			szBuf.TrimLeft();			szBuf.TrimRight();
			szBuf.MakeUpper();
			if (szBuf == _T("X")) {
				pDM->GetCellData(i, 0, strPK);
				strPK.TrimLeft();		strPK.TrimRight();
				pDM->GetCellData(i, 1, strRPTSize);
				strRPTSize.TrimLeft();	strRPTSize.TrimRight();
				if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
				break;
			}
		}
	}
	if (strPK.GetLength() == 0) return -1022;		// �ش� Report �� �����ϴ�.
	PK = strPK;

	// Report Data File�� �����´�.
	ids = GetLOBToFile(pDM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), _ttoi(strRPTSize), tmpFilename);
	if (ids < 0) return -1025;		// Lob�� File�� �ٿ� �޴µ� ����

	CString strTmp, strALIAS;
	for (i = 0; i < nRow; i++) {
		pDM->GetCellData(i, 2, szBuf);
		szBuf.TrimLeft();			szBuf.TrimRight();
		szBuf.MakeUpper();
		if (szBuf == _T("B")) {
			pDM->GetCellData(i, 0, strPK);
			strPK.TrimLeft();		strPK.TrimRight();
			pDM->GetCellData(i, 1, strRPTSize);
			strRPTSize.TrimLeft();	strRPTSize.TrimRight();
			if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
			pDM->GetCellData(i, 3, strALIAS);
			strALIAS.TrimLeft();	strALIAS.TrimRight();
			if (strALIAS.GetLength() == 0) continue;
			strTmp.Format(_T("rpt_tmp\\%s.bmp"), strALIAS);
			ids = GetLOBToFile(pDM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), _ttoi(strRPTSize), strTmp);
		}
	}
	
	return 0;
}

INT CSIReportTestView::GetLOBToFile(CESL_DataMgr *pDM, CString table_name, CString PKFieldname, CString PKFieldtype, CString PKData, CString FieldName, INT Size, CString FileName)
{
	BYTE *pBuffer;
	INT nBufferSize = Size + 1024;
	INT ids;

	pBuffer = (BYTE*)malloc(sizeof(BYTE) * nBufferSize);
	if (!pBuffer) {
		AfxMessageBox(_T("Malloc Memory Fail!"));
		return -1024;
	}
	
	ids = pDM->LOBGet(table_name, PKFieldname, PKFieldtype, PKData, FieldName, pBuffer, &nBufferSize);
	if (ids < 0) {
		AfxMessageBox(_T("Get Lob Data Fail!"));
		free(pBuffer);
		return -1024;
	}
	if (nBufferSize <= 0) {
		AfxMessageBox(_T("Empty Lob Data!"));
		return -1024;
	}
	CFile f;
    if( !f.Open(FileName, CFile::modeWrite|CFile::modeCreate | CFile::typeBinary) ) {
		free(pBuffer);
		AfxMessageBox(_T("Create Lob File Fail"));
		return - 1025;
	}
	
	if( 0 == f.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		f.Write( &cUni, sizeof(TCHAR));
	}
		
	f.WriteHuge(pBuffer, nBufferSize);
	f.Close();
	free (pBuffer);

	return 0;
}