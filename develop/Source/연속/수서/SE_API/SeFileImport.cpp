// SeFileImport.cpp : implementation file
//

#include "stdafx.h"
#include "SeFileImport.h"
#include "SePurchaseNoDlg.h"
#include "SeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeFileImport dialog


CSeFileImport::CSeFileImport(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeFileImport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeFileImport)
	m_nFILE_TYPE = -1;
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT
	
	m_pDM = NULL;
	m_pDM_OK = NULL;
	m_pDM_SUCCESS = NULL;
	m_pDM_ERR = NULL;
	m_pCM = NULL;
	m_pGrid_OK = NULL;
	m_pGrid_SUCCESS = NULL;
	m_pGrid_ERR = NULL;
	m_pThreadDisplay = NULL;
	m_pThreadImport = NULL;

	m_pDM_SEQ_NO = NULL;
	m_pDM_WORK_NO = NULL;

	m_strWORK_NO = _T("");
	m_strSEQ_NO_KEY = _T("");

	// 16/03/29 ������ : �Ѽ� �����ϱ� �����ϱ�
//*/ ADD -------------------------------------------------------------------------------------
	m_bIsExcel = TRUE;
	CLSID clsExcelApp;
	if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
	{
		m_bIsExcel = FALSE;	// 16/04/05 ������ : ��ȣ���� ������ ���� �ӽ��ּ�ó�� -> // 16/04/19 ������ : �ּ�Ǯ��
	}	
//*/ END ------------------------------------------------------------------------------------
}


CSeFileImport::~CSeFileImport()
{
	if(m_pThreadDisplay != NULL)
	{
		DWORD dwRetCode;
		dwRetCode = ::WaitForSingleObject(m_pThreadDisplay->m_hThread , 2000);
		if (dwRetCode == WAIT_TIMEOUT)
		{
			TerminateThread(m_pThreadDisplay->m_hThread , 0);
		}
	}

	if(m_pThreadImport != NULL)
	{
		DWORD dwRetCode;
		dwRetCode = ::WaitForSingleObject(m_pThreadImport->m_hThread , 2000);
		if (dwRetCode == WAIT_TIMEOUT)
		{
			TerminateThread(m_pThreadImport->m_hThread , 0);
		}
	}
}


VOID CSeFileImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeFileImport)
	DDX_Control(pDX, IDC_tabFILE_IMPORT, m_ctrlTAB);
	DDX_Radio(pDX, IDC_radFILE_TYPE1, m_nFILE_TYPE);
	DDX_Text(pDX, IDC_edtFILE_NAME, m_strFilePath);
	// 16/03/29 ������ : �Ѽ� �����ϱ�
//*/ ADD -------------------------------------------------------------------------------------
	if(m_bIsExcel != TRUE)
	{
		DDX_Text(pDX, IDC_radFILE_TYPE1, CString(_T("HCELL")));	// 16/04/05 ������ : ��ȣ���� ������ ���� �ӽ��ּ�ó�� -> // 16/04/19 ������ : �ּ�Ǯ��
	}
//*/ END -------------------------------------------------------------------------------------
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeFileImport, CDialog)
	//{{AFX_MSG_MAP(CSeFileImport)
	ON_BN_CLICKED(IDC_btnFILE_OPEN, OnbtnFILEOPEN)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabFILE_IMPORT, OnSelchangetabFILEIMPORT)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnFILE_ORDER, OnbtnFILEORDER)
	ON_BN_CLICKED(IDC_btnFILE_MARC, OnbtnFILEMARC)
	ON_BN_CLICKED(IDC_btnFILE_EXPORT, OnbtnFILEEXPORT)
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_edtFILE_ORDER_NO, OnKillfocusedtFILEORDERNO)
	ON_MESSAGE(ALLGRIDDISPLAY, OnGridDisplay)
	ON_MESSAGE(COUNT_STATUS_UPDATE, OnCountStatusUpdate)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeFileImport message handlers

BOOL CSeFileImport::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("����_����_���Ϲ���"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_���Ϲ���") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_����_����_���Ϲ���"));
	
	m_pGrid_OK = (CESL_Grid*)m_pCM->FindControl(_T("�׸���_����"));
	m_pGrid_SUCCESS = (CESL_Grid*)m_pCM->FindControl(_T("�׸���_����"));
	m_pGrid_ERR = (CESL_Grid*)m_pCM->FindControl(_T("�׸���_����"));
	
	m_pDM = FindDM(_T("DM_����_����_���Ϲ���"));
	m_pDM_OK = FindDM(_T("DM_����_����_���Ϲ���_����"));
	m_pDM_SUCCESS = FindDM(_T("DM_����_����_���Ϲ���_����"));
	m_pDM_ERR = FindDM(_T("DM_����_����_���Ϲ���_����"));
	m_pDM_SEQ_NO = FindDM(_T("DM_����_����_���Ϲ���_������ȣ"));
	m_pDM_WORK_NO = FindDM(_T("DM_����_����_���Ϲ���_�۾���ȣ"));

	if(!m_pCM || !m_pGrid_OK ||!m_pGrid_SUCCESS || !m_pGrid_ERR || !m_pDM || !m_pDM_OK || !m_pDM_SUCCESS || !m_pDM_ERR || !m_pDM_SEQ_NO || !m_pDM_WORK_NO) {
		AfxMessageBox( _T("DM,CM,grid Error : ����_����_���Ϲ���") );
		return FALSE;
	}	

	GetWindowRect(m_rcInitial);


	m_nFILE_TYPE = 0;

	m_ctrlTAB.InsertItem(0, _T("����"));
	m_ctrlTAB.InsertItem(1, _T("����"));
	m_ctrlTAB.InsertItem(2, _T("����"));

	
	m_pGrid_OK->BringWindowToTop();
	m_pGrid_ERR->ShowWindow(SW_HIDE);
	m_pGrid_SUCCESS->ShowWindow(SW_HIDE);


	// Status Bar
	m_wndStatusBarCtrl.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP , CRect(0, 0, 0, 0), this, WM_USER + 1000);
	m_wndStatusBarCtrl.SetMinHeight(22);
	
	if (!m_wndProgressBar.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, CRect(0,0,100,100), &m_wndStatusBarCtrl, 17298))
		return -1;

	m_wndProgressBar.SetFont(GetFont());

	const INT nParts = 5;
	CRect rect;
	m_wndStatusBarCtrl.GetClientRect(&rect);
	INT widths[nParts] = { 100, 200, 300, 400, -1 };
	m_wndStatusBarCtrl.SetParts(nParts, widths);

	PostMessage(WM_SIZE);

	UpdateData(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSeFileImport::OnbtnFILEOPEN() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;


	if (m_nFILE_TYPE == 0)
	{
		// 16/03/29 ������ : �Ѽ� �����ϱ�
//*/ ADD -------------------------------------------------------------------------------------
		// 16/04/05 ������ : ��ȣ���� ������ ���� �ӽ��ּ�ó�� -> // 16/04/19 ������ : �ּ�Ǯ��
		if (m_bIsExcel != TRUE) 
		{
			OFN.lpstrFilter = _T("�Ѽ�����(*.cell)\0*.cell\0�������(*.*)\0*.*\0");
			OFN.lpstrTitle = _T("�Ѽ� ���� ����");
		}	
		else
//*/ END -------------------------------------------------------------------------------------
		{
			OFN.lpstrFilter = _T("XLS����(*.xls)\0*.xls\0�������(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("EXCEL ���� ����");
		}
	}
	else if(m_nFILE_TYPE == 1)
	{
		OFN.lpstrFilter = _T("MARC����(*.txt)\0*.txt\0�������(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("MARC ���� ����");
	}


	BOOL bSuccess = GetOpenFileName(&OFN);
	SetCurrentDirectory(szBuffer);
	if (bSuccess)
	{
		m_strFilePath = OFN.lpstrFile;
		UpdateData(FALSE);
		
		
		m_wndProgressBar.SetWindowText(_T("���� ��ġ��..."));
		m_wndProgressBar.UpdateWindow();
		
		
		//DM free
		m_pDM->FreeData();
		m_pDM_OK->FreeData();
		m_pDM_SUCCESS->FreeData();
		m_pDM_ERR->FreeData();


		// ���� ���� ����
		ImportMarcFromFile();
		
	}
}


INT CSeFileImport::ImportMarcFromFile()
{
	
	BeginWaitCursor();
	INT ids;

	if (m_strFilePath.IsEmpty()) return -1;

	// MarcMgr, FileType, FilePath ����
	// File Type : XLS -- EXCEL ����, TXT -- TEXT ����
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

	//����� ��ȯ�� ���� �ڵ�Ŵ���
	m_marcImpExpMgr.m_pCodeMgr = m_pInfo->pCodeMgr;

	m_marcImpExpMgr.Init(this, m_pInfo->pMarcMgr, m_nFILE_TYPE , m_strFilePath);


	// Call Back Func ����
	m_marcImpExpMgr.SetCallBackMarcCheck((INT (CWnd::*)(CMarc *pMarc, CString &str))CallBackCheckMarc);


	// DM ����
	m_marcImpExpMgr.InitDMList();
	m_marcImpExpMgr.AddDMPtr(m_pDM);

	// ���� üũ ���� DM Alias ����
	m_marcImpExpMgr.SetErrChkAlias(_T("UDF_���Ϲ��Կ���"));

	// Stream Marc ���� DM Alias ����
	m_marcImpExpMgr.SetMarcStreamAlias(_T("MARC"));

	// Import ����
	ids = m_marcImpExpMgr.ImportFile();
	
	// Display Thread Start
	if (ids >= 0)
		m_pThreadDisplay = AfxBeginThread(ThreadFunc_Display, this);
	

	EndWaitCursor();

	return ids;
}

INT CSeFileImport::DeleteUnnecessaryMarcItem(CMarc* pMarc)
{

	const INT cnt = 5;
	CString strfiled[cnt] = {
		_T("001"),	//0
		_T("049"),	//1	
		_T("090"),	//2
		_T("980"),	//3	
		_T("052")	//4	
	};	
	
	for(INT i=0;i<cnt;i++)
	{
		m_pInfo->pMarcMgr->DeleteField(pMarc,strfiled[i]);
	}

	return 0;
}

INT CSeFileImport::SetCodeInfo(CESL_DataMgr* pDM,INT nRowIdx)
{
	//ȭ���� ��Ʈ�Ѱ� ���� ����, �⺻���� �־��ش�
	const INT cnt = 14;
	CString strAlias[cnt] = {
		_T("����ó_�ڵ�"),
		_T("��������_�ڵ�"),
		_T("��ϱ���_�ڵ�"),
		_T("��ü����_�ڵ�"),
		_T("������ϱ���_�ڵ�"),
		_T("�з���ȣ����_�ڵ�"),
		_T("�̿��󱸺�_�ڵ�"),
		_T("�̿����ѱ���_�ڵ�"),
		_T("�ڷᱸ��_�ڵ�"),
		_T("�����ڷᱸ��_�ڵ�"),
		_T("���ı���_�ڵ�"),
		_T("�����⵵"),
		_T("������ȣ"),
		_T("�Լ�å��")
	};
	
	CString strData;

	for(INT i=0;i<cnt;i++)
	{
		strData = _T("");
		m_pCM->GetControlMgrData(strAlias[i],strData);
		pDM->SetCellData(strAlias[i],strData,nRowIdx);
	}

	pDM->SetCellData(_T("�Է±���"),_T("2"),nRowIdx); //���� ����

	//�۾���ȣ �� �۾��Ϸù�ȣ ó�� 
	CSeApi api(this);
	CString strSerialNo = _T("");
	strSerialNo = api.GetSerialWorkNo(pDM,m_strSEQ_NO_KEY,m_strWORK_NO);

	pDM->SetCellData(_T("�������۾���ȣ"),m_strWORK_NO,nRowIdx); 
	pDM->SetCellData(_T("�����Ϸù�ȣ"),strSerialNo,nRowIdx); 	

	return 0;
}


VOID CSeFileImport::OnGridDisplay(WPARAM w,LPARAM l)
{
	m_pGrid_SUCCESS->Display();
	m_pGrid_ERR->Display();
	m_pGrid_OK->Display();
}

VOID CSeFileImport::OnCountStatusUpdate(WPARAM w,LPARAM l)
{
	CString strCount;

	//ERR COUNT
	strCount.Format(_T("���� : %d"), m_pDM_ERR->GetRowCount());
	m_wndStatusBarCtrl.SetText(strCount, 2, 0);
	

	//OK COUNT
	strCount.Format(_T("���� : %d"), m_pDM_OK->GetRowCount());
	m_wndStatusBarCtrl.SetText(strCount, 1, 0);
	

	//SUCCESS COUNT
	strCount.Format(_T("���� : %d"), m_pDM_SUCCESS->GetRowCount());
	m_wndStatusBarCtrl.SetText(strCount, 3, 0);

}

INT CSeFileImport::CallBackCheckMarc(CMarc *pMarc, CString &strCheckErrMsg)
{
	
	//008 ����
	//�Է��� ����
	COleDateTime t = COleDateTime::GetCurrentTime();
	CString tmp,inputdate;
	tmp.Format(_T("%04d%02d%02d"),t.GetYear(),t.GetMonth(),t.GetDay());
	inputdate = tmp.Mid(2);
	m_pInfo->pMarcMgr->DeleteItem(pMarc,_T("008@0-5"));
	m_pInfo->pMarcMgr->SetItem(pMarc,_T("008@0-5"),inputdate);
	

	//���� ���� (40�ڸ�)
	CString strtmp;
	m_pInfo->pMarcMgr->GetItem(pMarc,_T("008*"),strtmp);

	if(strtmp.GetLength()<40)
	{
		m_pInfo->pMarcMgr->SetItem(pMarc,_T("008@39"),_T(" "));
	}

	//������°� ���� ��� ������(c)�� ����
	strtmp = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc,_T("008@6"),strtmp);
	strtmp.TrimLeft();
	strtmp.TrimRight();

	if(strtmp.IsEmpty())
	{
		m_pInfo->pMarcMgr->SetItem(pMarc,_T("008@6"),_T("c"));
	}

	return 0;
}

INT CSeFileImport::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

UINT ThreadFunc_Display(LPVOID pParam)
{
	CSeFileImport *pDlg = (CSeFileImport*)pParam;
	// KOL.UDF.022
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	pDlg->m_bIsThreadRunning = TRUE;
	
	CString strCount;
	INT nTotalCount = -1;
	INT nErrCount = 0;
	INT nOKCount = 0;
	
	//Total Count
	nTotalCount = pDlg->m_pDM->GetRowCount();
	strCount.Format(_T("��ü : %d"), nTotalCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 0, 0);
	pDlg->m_wndProgressBar.SetRange(0, nTotalCount);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));

	CMarc Marc;

	//Error Count
	CString strErr;
	INT nRowIdx;
	for (INT i=0;i<nTotalCount;i++)
	{
		
		strErr = _T("");
		strErr = pDlg->m_pDM->GetCellData(_T("UDF_���Ϲ��Կ���"), i);
		
		if(!strErr.IsEmpty())
		{
			pDlg->m_pDM_ERR->InsertRow(-1);
			nRowIdx = pDlg->m_pDM_ERR->GetRowCount()-1;
			
			pDlg->CopyDMToDMByAlias(pDlg->m_pDM,i,pDlg->m_pDM_ERR,nRowIdx);
			nErrCount++;	

		}
		else
		{
			pDlg->m_pDM_OK->InsertRow(-1);
			nRowIdx = pDlg->m_pDM_OK->GetRowCount()-1;
			
			pDlg->CopyDMToDMByAlias(pDlg->m_pDM,i,pDlg->m_pDM_OK,nRowIdx);
		}
		
		pDlg->m_wndProgressBar.SetPos(i+1);
	}

	strCount.Format(_T("���� : %d"), nErrCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);
	

	//OK COUNT
	nOKCount = nTotalCount - nErrCount;
	strCount.Format(_T("���� : %d"), nOKCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);
	

	//SUCCESS COUNT
	pDlg->m_wndStatusBarCtrl.SetText(_T("���� : 0"), 3, 0);

	//Grid Display(post message)
	::PostMessage(pDlg->GetSafeHwnd(),ALLGRIDDISPLAY,0,0);

	pDlg->m_bIsThreadRunning = FALSE;
	

	return 0;
}

UINT ThreadFunc_Import(LPVOID pParam)
{
	
	CSeFileImport *pDlg = (CSeFileImport*)pParam;
	// KOL.UDF.022
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	CSeApi api(pDlg);
	pDlg->m_bIsThreadRunning = TRUE;
	
	pDlg->m_pGrid_OK->SelectMakeList();
	INT nCount = pDlg->m_pGrid_OK->SelectGetCount();
	INT idx = pDlg->m_pGrid_OK->SelectGetTailPosition();
	pDlg->m_wndProgressBar.SetRange(0, nCount);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));
	INT nProcess = 0;
	INT nFailCount = 0;
	CMarc Marc;
	CString strStreamMarc;
	INT ids;
	INT nSuccessIdx;
	CString strTitle;
	CString msg;

	while(idx>=0)
	{
		
		//���ʿ��� ��ũ�� �������� 
		strStreamMarc = _T("");		
		strStreamMarc = pDlg->m_pDM_OK->GetCellData(_T("MARC"),idx);
		pDlg->m_pInfo->pMarcMgr->Decoding(strStreamMarc,&Marc);
		pDlg->DeleteUnnecessaryMarcItem(&Marc);
		strTitle = _T("");
		pDlg->m_pInfo->pMarcMgr->GetItem(&Marc,_T("245$a"),strTitle);
		msg.Format(_T("[ %s ] ������..."),strTitle);
		pDlg->m_wndProgressBar.SetWindowText(msg);


		//�ڵ�,����, �⺻�� ���� �������ְ� 
		pDlg->SetCodeInfo(pDlg->m_pDM_OK,idx);
		
		
		//���� ��ũ Leader ����
		api.SetSerialMarcReader(&Marc);
		

		//API �Լ��� �θ���
		ids = api.SPECIES_PUR_OBJ_FILE_IMPORT(pDlg->m_pDM_OK,
											  &Marc,
	         								  idx);
		
		if(ids<0) {
			nFailCount++;
		}
		else 
		{
			pDlg->m_pDM_SUCCESS->InsertRow(-1);
			nSuccessIdx = pDlg->m_pDM_SUCCESS->GetRowCount()-1;
			pDlg->CopyDMToDMByAlias(pDlg->m_pDM_OK,idx,pDlg->m_pDM_SUCCESS,nSuccessIdx);
			pDlg->m_pDM_OK->DeleteRow(idx);
		}

		nProcess++;
		pDlg->m_wndProgressBar.SetPos(nProcess);
		idx = pDlg->m_pGrid_OK->SelectGetPrev();
	}
	
	pDlg->m_wndProgressBar.SetWindowText(_T("���ԿϷ�"));


	if(0 == nFailCount)
	{
		msg.Format(_T(" �� [ %d ] ���� �ڷḦ ���� ���� �Ͽ����ϴ�!"),nProcess);
	}
	else
	{
		msg.Format(_T(" �� [ %d ] ���� �ڷ��� [ %d ] ���� �ڷ�  ���Կ� ���� �Ͽ����ϴ�!"),nProcess,nFailCount);
	}

	AfxMessageBox(msg);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));	

	//Grid Display(post message)
	::PostMessage(pDlg->GetSafeHwnd(),ALLGRIDDISPLAY,0,0);

	//Count Status Update(post message)
	::PostMessage(pDlg->GetSafeHwnd(),COUNT_STATUS_UPDATE,0,0);

	pDlg->m_bIsThreadRunning = FALSE;
	

	return 0;
}

VOID CSeFileImport::OnSelchangetabFILEIMPORT(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	INT nCurSel = m_ctrlTAB.GetCurSel();
	
	if(0 == nCurSel)
	{
		m_pGrid_OK->BringWindowToTop();
		m_pGrid_OK->ShowWindow(SW_SHOW);
		m_pGrid_ERR->ShowWindow(SW_HIDE);
		m_pGrid_SUCCESS->ShowWindow(SW_HIDE);
	}
	else if(1 == nCurSel)
	{
		m_pGrid_ERR->BringWindowToTop();
		m_pGrid_OK->ShowWindow(SW_HIDE);
		m_pGrid_ERR->ShowWindow(SW_SHOW);
		m_pGrid_SUCCESS->ShowWindow(SW_HIDE);

	}
	else if(2 == nCurSel)
	{

		m_pGrid_SUCCESS->BringWindowToTop();
		m_pGrid_OK->ShowWindow(SW_HIDE);
		m_pGrid_ERR->ShowWindow(SW_HIDE);
		m_pGrid_SUCCESS->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}

VOID CSeFileImport::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (GetSafeHwnd() == NULL) return;

	CRect rcWnd;
	GetWindowRect(rcWnd);

	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	

		MoveWindow(rcWnd);
		return;
	}

	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			

		MoveWindow(rcWnd);
		return;
	}

	INT i = 0;
	CRect rcClient;
	CWnd *pWnd = NULL;

	CRect rcDlg;
	GetClientRect(rcDlg);

	// Tab Ctrl
	pWnd = GetDlgItem(IDC_tabFILE_IMPORT);
	if (pWnd)
	{
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);

		rcClient.right = rcDlg.right - 10;
		rcClient.bottom = rcDlg.bottom - 24;

		pWnd->MoveWindow(rcClient);
	}
	
	// Grids
	rcClient.top += 20;
	
	const INT nResizeControlCnt = 3;
	INT nArrayResizeControl[nResizeControlCnt] = {
		IDC_gridFILE_OK, IDC_gridFILE_ERR, 
		IDC_gridFILE_SUCCESS, 
	};	
	
	for (i = 0; i < nResizeControlCnt; i++)
	{
		pWnd = GetDlgItem(nArrayResizeControl[i]);
		if (pWnd != NULL)
		{
			pWnd->MoveWindow(rcClient);
		}
	}

	// StatusBar Ctrl
	if (m_wndStatusBarCtrl.GetSafeHwnd() != NULL)
		m_wndStatusBarCtrl.SendMessage(WM_SIZE, 0, 0);

	// ProgressBar
	if(!::IsWindow(m_wndProgressBar.m_hWnd))
        return;

	CRect   rc;
    m_wndStatusBarCtrl.GetRect(4,rc);
	rc.top -=1;
	rc.bottom +=5;
    
    m_wndProgressBar.SetWindowPos(NULL,rc.left,
                                 rc.top-1,
                                 rc.Width(),
                                 rc.Height()+2,SWP_NOZORDER | SWP_NOACTIVATE);

	m_wndProgressBar.GetClientRect(rc);
	HRGN hrgn = CreateRectRgn(rc.left+2, rc.top+4, rc.right-2, rc.bottom-6);
	m_wndProgressBar.SetWindowRgn(hrgn, TRUE);
	DeleteObject(hrgn);
}

VOID CSeFileImport::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

VOID CSeFileImport::OnOK() 
{
	// TODO: Add extra validation here
	
	//���� �ǿ� ���ؼ��� ����(�����ڷῡ ����), ���� �ڷ�� ���� ������ ������ Thread ��� 
	//���� ������ �ڵ� ������ �־� �ְ� ���ʿ��� �±״� ���� �Ѵ�.
	
	// Display Thread Start
	INT nCurSel = m_ctrlTAB.GetCurSel();
	if(nCurSel != 0) 
	{
		AfxMessageBox(_T("������ �����ڷῡ ���ؼ��� �� �� �ֽ��ϴ�!"));
		return;
	}
	
	CString strSEQ_NO;
	m_pCM->GetControlMgrData(_T("������ȣ"),strSEQ_NO);

	if(	strSEQ_NO.IsEmpty())
	{	
		AfxMessageBox(_T("������ȣ�� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}

	CString strACQ_BOOK_CNT;
	m_pCM->GetControlMgrData(_T("�Լ�å��"),strACQ_BOOK_CNT);
	if(strACQ_BOOK_CNT.IsEmpty())
	{
		AfxMessageBox(_T("�Լ�å���� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}


	m_pGrid_OK->SelectMakeList();
	INT idx = m_pGrid_OK->SelectGetHeadPosition();
	if(idx<0 || 0 == m_pDM_OK->GetRowCount())
	{
		AfxMessageBox(_T("������ �׸��� �����ϴ�!"));
		return;
	}
	

	//�۾� ��ȣ�� Ȯ���� ����ȣ�� ��� ���� �ش� ������ �� �۾���ȣ�� �߰��ϰ� ��� ������ �����Ѵ�. 
	if(0 == m_pDM_SEQ_NO->GetRowCount()) 
	{
		AfxMessageBox(_T("���� ������ ã�� �� �����ϴ�!"));
		return;
	}
	
	m_strSEQ_NO_KEY = m_pDM_SEQ_NO->GetCellData(_T("REC_KEY"),0);
	if(m_strSEQ_NO_KEY.IsEmpty())
	{
		AfxMessageBox(_T("����KEY�� ã�� �� �����ϴ�!"));
		return;

	}
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbFILE_WORK_NO);
	if(!pCombo) return;
	CString	tmp	= _T("");
	pCombo->GetWindowText(tmp);	
	
	if(_T("����ȣ") == tmp)
	{
		
		CSeApi api(this);

		CString strREC_KEY;
		CString strWorkLog;
		strWorkLog = GetWorkLogMsg(WORK_LOG_SEFILEIMPORT);
		m_pDM_WORK_NO->MakeRecKey(strREC_KEY);
		
		m_strWORK_NO = api.GetLastWorkNo(m_pDM_SEQ_NO,m_strSEQ_NO_KEY);
	
		m_pDM_WORK_NO->FreeData();
		m_pDM_WORK_NO->InsertRow(-1);
		m_pDM_WORK_NO->SetCellData(_T("REC_KEY"),strREC_KEY,0);
		m_pDM_WORK_NO->SetCellData(_T("��������KEY"),m_strSEQ_NO_KEY,0);
		m_pDM_WORK_NO->SetCellData(_T("�۾���ȣ"),m_strWORK_NO,0);
		m_pDM_WORK_NO->SetCellData(_T("�������Ϸù�ȣ"),_T("0"),0);		
		m_pDM_WORK_NO->SetCellData(_T("������"),api.GetTodayDate(),0);
		m_pDM_WORK_NO->SetCellData(_T("������"),m_pInfo->USER_ID,0);
		m_pDM_WORK_NO->SetCellData(_T("�����۾�"),strWorkLog,0);		
		m_pDM_WORK_NO->SetCellData(_T("�������۾�"),strWorkLog,0);	

		
		CArray<CString,CString> RemoveAliasList;

		m_pDM_WORK_NO->StartFrame();
		RemoveAliasList.RemoveAll();
		m_pDM_WORK_NO->InitDBFieldData();
		m_pDM_WORK_NO->MakeQueryByDM(RemoveAliasList, m_pDM_WORK_NO, _T("SE_PURCHASE_WORK_NO_TBL"), 0 , m_pDM_WORK_NO);
		INT ids = m_pDM_WORK_NO->SendFrame();
		m_pDM_WORK_NO->EndFrame();


		if(ids<0) 
		{
			AfxMessageBox(_T("�� �۾���ȣ �Է¿� ���� �߽��ϴ�!"));
			return;
		}


	}
	else if(_T("������") == tmp)
	{
		m_strWORK_NO = _T("0");
	}
	else
	{
		m_strWORK_NO = tmp;
	}
	

	//���Ϲ��� Thread
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pThreadImport = AfxBeginThread(ThreadFunc_Import, this);


	//CDialog::OnOK();
}

VOID CSeFileImport::OnbtnFILEORDER() 
{
	// TODO: Add your control notification handler code here
	CSePurchaseNoDlg dlg(this);
	if(dlg.DoModal() != IDOK) return;
		
	m_pCM->SetControlMgrData(_T("�����⵵"),dlg.m_strYear);
	m_pCM->SetControlMgrData(_T("������ȣ"),dlg.m_strPurchaseNo);

	SetPurchaseNo();

}

VOID CSeFileImport::OnbtnFILEMARC() 
{
	// TODO: Add your control notification handler code here
	INT nCurSel = m_ctrlTAB.GetCurSel();
	INT nType;

	if(0 == nCurSel) 
	{
		nType = 1;
	}
	else if(1 == nCurSel) 
	{

		nType = 2;
	}
	else
	{
		AfxMessageBox(_T("���� ������ �ڷ�� ������ �� �����ϴ�!"));
		return;
	}

	CSeFileImportMarcEdit dlg(this);
	dlg.m_nType = nType;
	dlg.m_pDM_ERR = m_pDM_ERR;
	dlg.m_pDM_OK = m_pDM_OK;
	dlg.m_pGrid_ERR = m_pGrid_ERR;
	dlg.m_pGrid_OK = m_pGrid_OK;
	

	dlg.DoModal();

}

VOID CSeFileImport::OnbtnFILEEXPORT() 
{
	// TODO: Add your control notification handler code here
	
	
	INT nCurSel = m_ctrlTAB.GetCurSel();
	// KOL.UDF.022 ��ť���ڵ� ����
	INT nRowCnt = 0;
	//INT nRowCnt;
	
	if (nCurSel == 0) 
		nRowCnt = m_pDM_OK->GetRowCount();
	else if (nCurSel == 1) 
		nRowCnt = m_pDM_ERR->GetRowCount();
	else if (nCurSel == 2)
		nRowCnt = m_pDM_SUCCESS->GetRowCount();


	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	
	CString msg;
	msg.Format(_T("[ %d ] ���� �ڷḦ ���� �Ͻðڽ��ϱ�?"),nRowCnt);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;


	// File Select Dialog
	CString strFilePath;
	

	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;
	OFN.lpstrDefExt = _T("TXT");

	OFN.lpstrFilter = _T("TXT����(*.txt)\0*.txt\0�������(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("TEXT ���� ����");

	if (GetSaveFileName(&OFN) != 0)
	{
		strFilePath= OFN.lpstrFile;
		ExportToTxtFile(strFilePath);
	}

	SetCurrentDirectory(szBuffer);

}

INT CSeFileImport::ExportToTxtFile(CString strFilePath)
{
	
	BeginWaitCursor();

	// Determine What Grid is Selected
	CESL_DataMgr* pDM = NULL;
	INT nCurSel = m_ctrlTAB.GetCurSel();

	if (nCurSel == 0) 
		pDM = m_pDM_OK;
	else if (nCurSel == 1) 
		pDM = m_pDM_ERR;
	else if (nCurSel == 2)
		pDM = m_pDM_SUCCESS;


	// File
	CStdioFile file;
	if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	CString strMarc;

	for (INT i=0;i<pDM->GetRowCount();i++)
	{
		strMarc = _T("");
		strMarc = pDM->GetCellData(_T("MARC"),i);
		if (strMarc.IsEmpty()) continue;

		strMarc += _T("\r\n");
		file.WriteString(strMarc);
	}

	file.Close();
	EndWaitCursor();
	AfxMessageBox(_T("�����Ͽ����ϴ�!"));

	return 0;
}

VOID CSeFileImport::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	

	CDialog::OnClose();
}

VOID CSeFileImport::OnKillfocusedtFILEORDERNO() 
{
	// TODO: Add your control notification handler code here
	
	// ���� ��ȣ kill focus
	SetPurchaseNo();

}

VOID CSeFileImport::SetPurchaseNo()
{

	CString strSEQ_NO = _T("");
	m_pCM->GetControlMgrData(_T("������ȣ"),strSEQ_NO);
	if(strSEQ_NO.IsEmpty()) return;

	CString strYear;
	m_pCM->GetControlMgrData(_T("�����⵵"),strYear);
	if(strYear.IsEmpty()) return;

	CString strQuery;
	strQuery.Format(_T("ACQ_YEAR = '%s' AND SEQ_NO = %s"),strYear,strSEQ_NO);
	m_pDM_SEQ_NO->RefreshDataManager(strQuery);
	
	INT nRowCnt = m_pDM_SEQ_NO->GetRowCount();
	if(0 == nRowCnt) {
		AfxMessageBox(_T("�ش� �����⵵�� �������� �ʴ� ������ȣ�Դϴ�!"));
		GetDlgItem(IDC_edtFILE_ORDER_NO)->SetFocus();
		return;
	}
	
	//���Դ�� �ش��ϴ� ������ �Է� �� �� �ְ� �Ѵ�
	CString strWorking_status;
	strWorking_status = m_pDM_SEQ_NO->GetCellData(_T("�۾�����"),0);
	if(_T("0") != strWorking_status) //���Դ���� �ƴϸ� 
	{
		AfxMessageBox(_T("�ش� ������ �ֹ����Դϴ�. ���Դ�� ������ ������ �ּ���!"));
		GetDlgItem(IDC_edtFILE_ORDER_NO)->SetFocus();
		return;
	}

	CString strWorkNo = _T("");
	strWorkNo = m_pDM_SEQ_NO->GetCellData(_T("�������۾���ȣ"),0);
	
	CString strREC_KEY = _T("");
	strREC_KEY = m_pDM_SEQ_NO->GetCellData(_T("REC_KEY"),0);


	if(_T("0") == strWorkNo) //�۾���ȣ�� ������� �ʴ� ��� 
	{
		SetWorkNo(_T(""),_T(""),FALSE);
	}
	else //����ϴ� ���
	{
		SetWorkNo(strREC_KEY,strWorkNo); //������ ��ȣ�� setting 
	}

}


VOID CSeFileImport::SetWorkNo(CString strSEQ_NO_KEY, CString strCurrentWorkNo , BOOL bUseWorkNo)
{
	//�۾���ȣ ���� �Լ� 
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbFILE_WORK_NO);
	if(!pCombo) return;
	
	if(!bUseWorkNo) //������
	{
		pCombo->ResetContent();
		pCombo->InsertString( 0, _T("������") );
		pCombo->SetCurSel( 0 );
		return;
	}

	if(strSEQ_NO_KEY.IsEmpty()) return;
	
	CString strQuery;
	strQuery.Format(_T("PURCHASE_SEQ_NO_KEY = %s"),strSEQ_NO_KEY);
	m_pDM_WORK_NO->RefreshDataManager(strQuery);
	
	pCombo->ResetContent();
	INT nRowCnt = m_pDM_WORK_NO->GetRowCount();
	CString strWorkNo;

	for(INT i=0;i<nRowCnt;i++)
	{
		strWorkNo = _T("");
		strWorkNo = m_pDM_WORK_NO->GetCellData(_T("�۾���ȣ"),i);
		pCombo->InsertString( i, strWorkNo);
	}
	
	pCombo->InsertString( nRowCnt , _T("����ȣ") );
		
	
	if(strCurrentWorkNo.IsEmpty())
	{
		pCombo->SetCurSel( 0 );
	}
	else
	{
		INT nIndex = pCombo->FindStringExact(0,strCurrentWorkNo);
	
		if(nIndex>=0) pCombo->SetCurSel( nIndex );
		else pCombo->SetCurSel( 0 );

	}

}


HBRUSH CSeFileImport::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
