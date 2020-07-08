// AddressImpExpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AddressImpExpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddressImpExpDlg dialog


CAddressImpExpDlg::CAddressImpExpDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddressImpExpDlg)
	m_nImpExpMode = 0;
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT

	m_bIsThreadRun = FALSE;
	//m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));
}

CAddressImpExpDlg::~CAddressImpExpDlg()
{
	if (m_bIsThreadRun)
	{
		m_bIsThreadRun = FALSE;
	}
/*
	CloseHandle(m_hThread);

	if (m_bIsThreadRun)
	{
		excelCtrl.CloseManager();
		excelMgr.CloseManager();
		m_bIsThreadRun = FALSE;
		MSG msg;
		while (TRUE)
		{
			if (::PeekMessage(&msg,GetSafeHwnd(), 1, 1, PM_NOREMOVE))
				AfxGetApp()->PumpMessage();

			if (WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0)
				break;
		}
	}
	*/
} 

BOOL CAddressImpExpDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}


VOID CAddressImpExpDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddressImpExpDlg)
	DDX_Control(pDX, IDC_PROGRESS_ZIP, m_ProgressBar);
	DDX_Radio(pDX, IDC_rIMPORT, m_nImpExpMode);
	DDX_Text(pDX, IDC_eFILEPATH, m_strFilePath);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CAddressImpExpDlg, CDialog)
	//{{AFX_MSG_MAP(CAddressImpExpDlg)
	ON_BN_CLICKED(IDC_bFILEOPEN, OnbFILEOPEN)
	ON_BN_CLICKED(IDC_bSTART_IMP_EXP, OnbSTARTIMPEXP)
	ON_BN_CLICKED(IDC_bSTART_IMP_EXP2, OnbSTARTIMPEXP2)
	ON_BN_CLICKED(IDC_rIMPORT, OnrIMPORT)
	ON_BN_CLICKED(IDC_rEXPORT, OnrEXPORT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

UINT ThreadFunc_Import(LPVOID pParam)
{
	CAddressImpExpDlg *pDlg = (CAddressImpExpDlg*)pParam;
	// KOL.UDF.022 ��ť���ڵ� ����
	if (&pDlg == NULL)
	//if (pDlg == NULL)
	{
		pDlg->m_ProgressBar.SetPos(0);
		pDlg->m_bIsThreadRun = FALSE;
		SetEvent(pDlg->m_hThread);
		return -1;
	}
	
	pDlg->m_bIsThreadRun = TRUE;

	pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T("�������� �м��� �Դϴ�."));

	C_ExcelManControl	excelCtrl;
	CExcelManager		excelMgr;

	// Excel Manager Init
	CRect rect;
	pDlg->GetWindowRect(rect);
	excelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, pDlg->GetParent(), WM_USER + 10000);
	if (excelCtrl.m_hWnd == NULL) 
	{
		pDlg->MessageBox(_T("Excelmanager.ocx�� ��ġ�Ǿ� ���� �ʽ��ϴ�. ��ġ�Ͽ� �ֽʽÿ�."));
		pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
		pDlg->m_ProgressBar.SetPos(0);
		pDlg->m_bIsThreadRun = FALSE;
		pDlg->SetControlEnable(TRUE);
		return -1;
	}

	excelMgr.SetManager(&excelCtrl);

	if(excelMgr.OpenManager() == FALSE) 
	{
        pDlg->MessageBox(_T("NOT Installed Excelmanager.ocx or No Excel!!!"));
		pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
		pDlg->m_ProgressBar.SetPos(0);
		pDlg->m_bIsThreadRun = FALSE;
		pDlg->SetControlEnable(TRUE);
		return -1;
	}
	
	excelMgr.SetUseTemplate(TRUE, pDlg->m_strFilePath);
	if (excelMgr.OpenManager() == FALSE)
	{
		pDlg->MessageBox(_T("Open Failed"));
		pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
		pDlg->m_ProgressBar.SetPos(0);
		pDlg->m_bIsThreadRun = FALSE;
		pDlg->SetControlEnable(TRUE);
		return -1;
	}

	/// << Get Header >>
	// ZIP_CODE = 0
	// SIDO		= 1
	// GUGUN	= 2
	// DONG		= 3
	// BUNJI	= 4
	// SEQ		= 5
	INT ids = -1;
	INT nColCount = 6;
	INT nRowCount = 1;

	CString strItem;
	CString strArrItems[6];

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDlg->m_pInfo->CONNECTION_INFO);
	dm.StartFrame();

	// ProgressBar �ʱ�ȭ ���� �۾�
	pDlg->m_ProgressBar.SetRange(0, 200);
	INT nCnt = 0;
	//BOOL bAdd = TRUE;
	while( TRUE )
	{
		for( INT i = 0; i < nColCount; i++ )
		{
			strItem.Empty();
			strItem = excelMgr.GetTextMatrix(nRowCount + 1, i + 1);
		}
		if(pDlg->m_bIsThreadRun==FALSE) break;
		//�Ϸù�ȣ�� ���ٸ� ����
		if (strItem.IsEmpty()) break;
		nRowCount++;
		nCnt++;
		if( nCnt > 200 ) nCnt = 0;
	
		pDlg->m_ProgressBar.SetPos(nCnt);
	}
	
	pDlg->m_ProgressBar.SetPos(0);

	INT nLower = 0;
	INT nUpper = nRowCount;
	pDlg->m_ProgressBar.SetRange(nLower, nUpper);
	pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T("�����ȣ ������ �Դϴ�."));

	if(pDlg->m_bIsThreadRun==FALSE) goto CANCEL;

	//++2008.05.13 ADD BY PWR {{++
	// ��ü���� äũ�ڽ� �߰��Ͽ� ��ü�����϶��� ������ �����ȣ ���̺� ���� ��ü������ �����Ѵ�
	if( (((CButton*)pDlg->GetDlgItem(IDC_chkALLIMPORT))->GetCheck()) == 1 )
	{
		CString strQuery;
		strQuery = _T("DELETE FROM CO_ZIP_CODE_TBL;");
		dm.InitDBFieldData();
		dm.AddFrame( strQuery );
		ids = dm.SendFrame( FALSE );
	}
	//--2008.05.13 UPDATE BY PWR --}}

	nRowCount = 1;
	// Fetch Row
	while (TRUE)
	{
		for (INT i = 0; i < nColCount; i++)
		{
			strItem.Empty();
			strItem = excelMgr.GetTextMatrix(nRowCount + 1, i + 1);
			//++2008.05.13 ADD BY PWR {{++
			// ������ ������ �����Ͽ� Trimó�� 
			strItem.TrimLeft(); strItem.TrimRight();
			//--2008.05.13 ADD BY PWR --}}
			//++2008.07.08 ADD BY PWR {{++
			// INSERT DATA�� '�����ϸ� �����߻� -> ''���� ��ü
			strItem.Replace( _T("'"), _T("''") );
			//--2008.07.08 ADD BY PWR --}}
			strArrItems[i] = strItem;
		}
		if(pDlg->m_bIsThreadRun==FALSE) break;
		if (strItem.IsEmpty()) break;
		nRowCount++;
		pDlg->m_ProgressBar.SetPos(nRowCount);
		
		if (pDlg->InsertAddress(&dm, strArrItems) < 0) break;	
	}
	if(pDlg->m_bIsThreadRun==FALSE) goto CANCEL;

	ids = dm.SendFrame();
	if (ids < 0) 
	{
		pDlg->MessageBox(_T("Sendframe Error"));
		goto ERR;
	}
	ids = dm.EndFrame();
	if (ids < 0) goto ERR;
	
	pDlg->MessageBox(_T("���Լ���"));
	excelMgr.CloseManager();
	pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
	pDlg->m_ProgressBar.SetPos(0);
	pDlg->m_bIsThreadRun = FALSE;
	pDlg->SetControlEnable(TRUE);
	return 0;
ERR:
	pDlg->MessageBox(_T("���Խ���"));
	excelMgr.CloseManager();
	pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
	pDlg->m_ProgressBar.SetPos(0);
	pDlg->m_bIsThreadRun = FALSE;
	pDlg->SetControlEnable(TRUE);
	return -1;
CANCEL:
	pDlg->MessageBox(_T("��������"));
	excelMgr.CloseManager();
	pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
	pDlg->m_ProgressBar.SetPos(0);
	pDlg->m_bIsThreadRun = FALSE;
	pDlg->SetControlEnable(TRUE);
	return -1;
}

UINT ThreadFunc_Export(LPVOID pParam)
{
	CAddressImpExpDlg *pDlg = (CAddressImpExpDlg*)pParam;
	// KOL.UDF.022 ��ť���ڵ� ����
	if (&pDlg == NULL)
	//if (pDlg == NULL)
	{
		pDlg->m_ProgressBar.SetPos(0);
		pDlg->m_bIsThreadRun = FALSE;
		SetEvent(pDlg->m_hThread);
		return -1;
	}
	
	pDlg->m_bIsThreadRun = TRUE;

	pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T("�����ȣ ���� ������ �Դϴ�."));
	pDlg->GetDlgItem(IDC_bSTART_IMP_EXP2)->EnableWindow(FALSE);
	pDlg->m_ProgressBar.SetRange(0, 10);

	C_ExcelManControl	excelCtrl;
	CExcelManager		excelMgr;

	// Excel Manager Init
	CRect rect;
	pDlg->GetWindowRect(rect);
	excelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, pDlg->GetParent(), WM_USER + 10000);
	if (excelCtrl.m_hWnd == NULL) 
	{
		pDlg->MessageBox(_T("Excelmanager.ocx�� ��ġ�Ǿ� ���� �ʽ��ϴ�. ��ġ�Ͽ� �ֽʽÿ�."));
		pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
		pDlg->m_ProgressBar.SetPos(0);
		pDlg->m_bIsThreadRun = FALSE;
		pDlg->SetControlEnable(TRUE);
		return -1;
	}

	excelMgr.SetManager(&excelCtrl);
	pDlg->m_ProgressBar.SetPos(2);
	if(excelMgr.OpenManager() == FALSE) 
	{
        pDlg->MessageBox(_T("NOT Installed Excelmanager.ocx or No Excel!!!"));
		pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
		pDlg->m_ProgressBar.SetPos(0);
		pDlg->m_bIsThreadRun = FALSE;
		pDlg->SetControlEnable(TRUE);
		return -1;
	}
	pDlg->m_ProgressBar.SetPos(4);
	INT nColCount = 6;
	
	CString strQuery = _T("SELECT ZIP_CODE, SIDO, GUGUN, DONG, BUNJI, SEQ FROM CO_ZIP_CODE_TBL ORDER BY SEQ");
	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDlg->m_pInfo->CONNECTION_INFO);
	INT ids = dm.RestructDataManager(strQuery);
	if (ids < 0) 
	{
		pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
		pDlg->m_ProgressBar.SetPos(0);
		pDlg->m_bIsThreadRun = FALSE;
		pDlg->SetControlEnable(TRUE);
		return ids;
	}
	pDlg->m_ProgressBar.SetPos(8);
	CString strItem;
	CString strArrItems[6];

	excelMgr.SetTextMatrix(1, 1, _T("�����ȣ"));
	excelMgr.SetTextMatrix(1, 2, _T("�õ�"));
	excelMgr.SetTextMatrix(1, 3, _T("��"));
	excelMgr.SetTextMatrix(1, 4, _T("��"));
	excelMgr.SetTextMatrix(1, 5, _T("����"));
	excelMgr.SetTextMatrix(1, 6, _T("��ȣ"));

	pDlg->m_ProgressBar.SetPos(10);
	INT nRows = dm.GetRowCount();
	pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T("�����ȣ ������ �Դϴ�."));
	INT nLower = 0;
	INT nUpper = nRows;
	pDlg->m_ProgressBar.SetRange(nLower, nUpper);
	pDlg->m_ProgressBar.SetPos(0);
	pDlg->GetDlgItem(IDC_bSTART_IMP_EXP2)->EnableWindow(TRUE);
	for (INT row = 0; row < nRows; row++)
	{
		for (INT col = 0; col < nColCount; col++)
		{
			excelMgr.SetTextMatrix(row + 2, col + 1, dm.GetCellData(row, col));
		}
		pDlg->m_ProgressBar.SetPos(row);
		if( pDlg->m_bIsThreadRun == FALSE ) break;
	}
	if( pDlg->m_bIsThreadRun == FALSE ) goto CANCEL;
	excelMgr.SaveAs(pDlg->m_strFilePath);
	excelMgr.CloseManager();
	pDlg->MessageBox(_T("���⼺��"));
	pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
	pDlg->m_ProgressBar.SetPos(0);
	pDlg->m_bIsThreadRun = FALSE;
	pDlg->SetControlEnable(TRUE);
	return 0;
CANCEL:
	excelMgr.CloseManager();
	pDlg->MessageBox(_T("��������"));
	pDlg->GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
	pDlg->m_ProgressBar.SetPos(0);
	pDlg->m_bIsThreadRun = FALSE;
	pDlg->SetControlEnable(TRUE);
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CAddressImpExpDlg message handlers

BOOL CAddressImpExpDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (m_pInfo == NULL) 
	{
		MessageBox(_T("�θ� �����찡 �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//++2008.05.13 ADD BY PWR {{++++++++++++++++++++++++++++++++++++++++++++
	// ��ü���� äũ�ڽ� �߰��Ͽ� �⺻�� äũ
	// (��ü����:�����ǿ����ȣ���̺� ���� ��ü ������ ��ü����)
	((CButton*)GetDlgItem(IDC_chkALLIMPORT))->SetCheck(1);
	//--2008.05.13 ADD BY PWR --------------------------------------------}}

	Display();

	GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CAddressImpExpDlg::OnbFILEOPEN() 
{
	EFS_BEGIN

	if (m_bIsThreadRun == TRUE) return;

	UpdateData();

	// 2006-01-18 ADD BY PDW(Jr) ==================================================
	// ���丮�� �����·� �ǵ������Ѵ�. 
	TCHAR szCurDir[1024];
	GetCurrentDirectory( 1024, szCurDir );
	// ============================================================================

	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFilter = _T("XLS ����(*.xls)\0*.xls\0�������(*.*)\0*.*\0");

	//++2008.05.13 UPDATE BY PWR {{++++++++++++++++++++++++++++++++++++++++++++++++
	if (m_nImpExpMode == 0)
		//OFN.lpstrTitle = _T("XLS ���� ����...");
		OFN.lpstrTitle = _T("XLS ���� ����...");
	else if (m_nImpExpMode == 1)
		//OFN.lpstrTitle = _T("XLS ���� ����...");
		OFN.lpstrTitle = _T("XLS ���� ����...");
	//--2008.05.13 UPDATE BY PWR ------------------------------------------------}}

	OFN.lpstrFile = lpstrFile;
	OFN.lpstrDefExt = _T("xls");
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;


	BOOL bSuccess = FALSE;
	if (m_nImpExpMode == 0)
		 bSuccess = GetOpenFileName(&OFN);
	else if (m_nImpExpMode == 1)
		bSuccess = GetSaveFileName(&OFN);
	else 
		return;

	if (bSuccess)
	{
		m_strFilePath = OFN.lpstrFile;
	}
	else 
		return;


	UpdateData(FALSE);
	Display();

	// 2006-01-18 ADD BY PDW(Jr) ==================================================
	// ���丮�� �����·� �ǵ������Ѵ�. 
	SetCurrentDirectory( szCurDir );
	// ============================================================================

	EFS_END
}

VOID CAddressImpExpDlg::OnbSTARTIMPEXP() 
{
	EFS_BEGIN

	if (m_bIsThreadRun == TRUE) return;

	UpdateData();

	if (m_strFilePath.IsEmpty()) 
	{
		MessageBox(_T("���� ��θ� �����ϼ���"));
		return;
	}

	CString sMode;
	GetDlgItem(IDC_bSTART_IMP_EXP2)->GetWindowText(sMode);
	if( sMode == _T("�ݱ�") )
	{
		SetControlEnable(FALSE);		
	}

	if (m_nImpExpMode == 0)
	{
		ImpAddress();
	}
	else if (m_nImpExpMode == 1)
	{
		ExpAddress();
	}

	EFS_END
}


INT CAddressImpExpDlg::Display()
{
	EFS_BEGIN

	UpdateData();

	if (m_strFilePath.IsEmpty())
		GetDlgItem(IDC_bSTART_IMP_EXP)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bSTART_IMP_EXP)->EnableWindow(TRUE);

	return 0;

	EFS_END
	return -1;

}
INT CAddressImpExpDlg::SetControlEnable(BOOL Enable)
{
	if( Enable == TRUE )
	{
		GetDlgItem(IDC_rIMPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_rEXPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_chkALLIMPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_bFILEOPEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_bSTART_IMP_EXP)->EnableWindow(TRUE);
		GetDlgItem(IDC_bSTART_IMP_EXP2)->SetWindowText(_T("�ݱ�"));
	}
	else
	{
		GetDlgItem(IDC_rIMPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_rEXPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_chkALLIMPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_bFILEOPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_bSTART_IMP_EXP)->EnableWindow(FALSE);
		GetDlgItem(IDC_bSTART_IMP_EXP2)->SetWindowText(_T("����"));
	}

	return 0;
}

INT CAddressImpExpDlg::ImpAddress()
{
EFS_BEGIN

	if (m_bIsThreadRun == TRUE) return -1;
	UpdateData();
	// ������� Import ���μ����� �����Ѵ�.
	AfxBeginThread(ThreadFunc_Import, this);
/*
	UpdateData();
	GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T("�������� �м��� �Դϴ�. ��ø� ��޷� �ֽʽÿ�"));

	C_ExcelManControl	excelCtrl;
	CExcelManager		excelMgr;

	// Excel Manager Init
	CRect rect;
	GetWindowRect(rect);
	excelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, this, WM_USER + 10000);
	if (excelCtrl.m_hWnd == NULL) 
	{
		MessageBox(_T("Excelmanager.ocx�� ��ġ�Ǿ� ���� �ʽ��ϴ�. ��ġ�Ͽ� �ֽʽÿ�."));
		GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
		return -1;
	}

	excelMgr.SetManager(&excelCtrl);

	if(excelMgr.OpenManager() == FALSE) 
	{
        MessageBox(_T("NOT Installed Excelmanager.ocx or No Excel!!!"));
		GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
		return -1;
	}
	
	excelMgr.SetUseTemplate(TRUE, m_strFilePath);
	if (excelMgr.OpenManager() == FALSE)
	{
		MessageBox(_T("Open Failed"));
		GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
		return -1;
	}

	/// << Get Header >>
	// ZIP_CODE = 0
	// SIDO		= 1
	// GUGUN	= 2
	// DONG		= 3
	// BUNJI	= 4
	// SEQ		= 5
	INT ids = -1;
	INT nColCount = 6;
	INT nRowCount = 1;

	CString strItem;
	CString strArrItems[6];

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dm.StartFrame();

	// ProgressBar �ʱ�ȭ ���� �۾�
	INT nMaxPos = 200;
	m_ProgressBar.SetRange(0, nMaxPos);
	
	INT nCnt = 0;
	while( TRUE )
	{
		for( INT i = 0; i < nColCount; i++ )
		{
			strItem.Empty();
			strItem = excelMgr.GetTextMatrix(nRowCount + 1, i + 1);
		}
		//�Ϸù�ȣ�� ���ٸ� ����
		if (strItem.IsEmpty()) break;
		nRowCount++;
		nCnt++;
		if( nCnt > nMaxPos ) nCnt = 0;
		m_ProgressBar.SetPos(nCnt);
	}
	m_ProgressBar.SetPos(0);

	nMaxPos = nRowCount;
	m_ProgressBar.SetRange(0, nMaxPos);
	GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T("�����ȣ ������ �Դϴ�."));

	//++2008.05.13 ADD BY PWR {{++
	// ��ü���� äũ�ڽ� �߰��Ͽ� ��ü�����϶��� ������ �����ȣ ���̺� ���� ��ü������ �����Ѵ�
	if( (((CButton*)GetDlgItem(IDC_chkALLIMPORT))->GetCheck()) == 1 )
	{
		CString strQuery;
		strQuery = _T("DELETE FROM CO_ZIP_CODE_TBL;");
		dm.InitDBFieldData();
		dm.AddFrame( strQuery );
		ids = dm.SendFrame( FALSE );
	}
	//--2008.05.13 UPDATE BY PWR --}}

	nRowCount = 1;
	// Fetch Row
	while (TRUE)
	{
		for (INT i = 0; i < nColCount; i++)
		{
			strItem.Empty();
			strItem = excelMgr.GetTextMatrix(nRowCount + 1, i + 1);
			//++2008.05.13 ADD BY PWR {{++
			// ������ ������ �����Ͽ� Trimó�� 
			strItem.TrimLeft(); strItem.TrimRight();
			//--2008.05.13 ADD BY PWR --}}
			//++2008.07.08 ADD BY PWR {{++
			// INSERT DATA�� '�����ϸ� �����߻� -> ''���� ��ü
			strItem.Replace( _T("'"), _T("''") );
			//--2008.07.08 ADD BY PWR --}}
			strArrItems[i] = strItem;
		}

		if (strItem.IsEmpty()) break;
		nRowCount++;
		m_ProgressBar.SetPos(nRowCount);

		if (InsertAddress(&dm, strArrItems) < 0) break;	
	}
	
	ids = dm.SendFrame();
	if (ids < 0) 
	{
		MessageBox(_T("Sendframe Error"));
		goto ERR;
	}
	ids = dm.EndFrame();
	if (ids < 0) goto ERR;
	
	MessageBox(_T("���Լ���"));
	excelMgr.CloseManager();
	GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
	m_ProgressBar.SetPos(0);
	return 0;
ERR:
	MessageBox(_T("���Խ���"));
	excelMgr.CloseManager();
	GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
	m_ProgressBar.SetPos(0);
	*/
	return 0;

EFS_END
GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
m_ProgressBar.SetPos(0);
return -1;
}

// ZIP_CODE = 0
// SIDO = 1
// GUGUN = 2
// DONG = 3
// BUNJI = 4
// SEQ = 5
INT CAddressImpExpDlg::InsertAddress(CESL_DataMgr *pDM, CString *pStrArr)
{
	EFS_BEGIN

	if (pDM == NULL || pStrArr == NULL) return -1;
	
	CString strQuery;
	CString strResult;

	// Check Dup
	if( (((CButton*)GetDlgItem(IDC_chkALLIMPORT))->GetCheck()) == 0 )
	{
		//++2008.05.13 ADD BY PWR {{++
		// ������ ������ BUNJI=''�� �ƴ϶� BUNJI IS NULL
		CString sBunji;
		if( pStrArr[4].IsEmpty() )
		{
			sBunji = _T("BUNJI IS NULL");
		}
		else
		{
			sBunji.Format( _T("BUNJI='%s'"), pStrArr[4] );
		}
		// �����ȣ(ZIP_CODE)�� �ߺ��� ���� �ϹǷ� ����(BUNJI), ��(DONG)���� �߰�
		//strQuery.Format("SELECT REC_KEY FROM CO_ZIP_CODE_TBL WHERE ZIP_CODE = '%s'", pStrArr[0]);
		strQuery.Format( _T("SELECT REC_KEY FROM CO_ZIP_CODE_TBL WHERE ZIP_CODE='%s' AND DONG='%s' AND %s")
			, pStrArr[0], pStrArr[3], sBunji );
		//--2008.05.13 UPDATE BY PWR --}}
		
		pDM->GetOneValueQuery(strQuery, strResult);
	}
	
	// 2006-01-18 UPDATE BY PDW(Jr) ==============================================
	// SEQ �ʵ� Ÿ�� ���� ��Ŵ 
	// STRING >> NUMERIC
	if (strResult.IsEmpty())
	{
		// Insert
		CString strSeq;
		strSeq.Format(_T("%s.NEXTVAL"), DEFAULT_SEQ_NAME);
		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strSeq);
		pDM->AddDBFieldData(_T("ZIP_CODE"), _T("STRING"), pStrArr[0]);
		pDM->AddDBFieldData(_T("SIDO"), _T("STRING"), pStrArr[1]);
		pDM->AddDBFieldData(_T("GUGUN"), _T("STRING"), pStrArr[2]);
		pDM->AddDBFieldData(_T("DONG"), _T("STRING"), pStrArr[3]);
		pDM->AddDBFieldData(_T("BUNJI"), _T("STRING"), pStrArr[4]);
		pDM->AddDBFieldData(_T("SEQ"), _T("NUMERIC"), pStrArr[5]);
		pDM->MakeInsertFrame(_T("CO_ZIP_CODE_TBL"));
	}
	else
	{
		// Update
		pDM->InitDBFieldData();
		//dm.AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strSeq);
		pDM->AddDBFieldData(_T("ZIP_CODE"), _T("STRING"), pStrArr[0]);
		pDM->AddDBFieldData(_T("SIDO"), _T("STRING"), pStrArr[1]);
		pDM->AddDBFieldData(_T("GUGUN"), _T("STRING"), pStrArr[2]);
		pDM->AddDBFieldData(_T("DONG"), _T("STRING"), pStrArr[3]);
		pDM->AddDBFieldData(_T("BUNJI"), _T("STRING"), pStrArr[4]);
		pDM->AddDBFieldData(_T("SEQ"), _T("NUMERIC"), pStrArr[5]);
		pDM->MakeUpdateFrame(_T("CO_ZIP_CODE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strResult);
	}
	
	return 0;

	EFS_END
	return -1;
}

INT CAddressImpExpDlg::ExpAddress()
{
	EFS_BEGIN

	UpdateData();
	// ������� Export ���μ����� �����Ѵ�.
	AfxBeginThread(ThreadFunc_Export, this);

	/*
	GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T("�����ȣ ���� ������ �Դϴ�."));

	C_ExcelManControl	excelCtrl;
	CExcelManager		excelMgr;

	// Excel Manager Init
	CRect rect;
	GetWindowRect(rect);
	excelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, this, WM_USER + 10000);
	if (excelCtrl.m_hWnd == NULL) 
	{
		MessageBox(_T("Excelmanager.ocx�� ��ġ�Ǿ� ���� �ʽ��ϴ�. ��ġ�Ͽ� �ֽʽÿ�."));
		GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
		return -1;
	}

	excelMgr.SetManager(&excelCtrl);

	if(excelMgr.OpenManager() == FALSE) 
	{
        MessageBox(_T("NOT Installed Excelmanager.ocx or No Excel!!!"));
		GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
		return -1;
	}

	INT nColCount = 6;
	
	CString strQuery = _T("SELECT ZIP_CODE, SIDO, GUGUN, DONG, BUNJI, SEQ FROM CO_ZIP_CODE_TBL ORDER BY SEQ");
	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	INT ids = dm.RestructDataManager(strQuery);
	if (ids < 0) 
	{
		GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
		return ids;
	}

	CString strItem;
	CString strArrItems[6];

	
	excelMgr.SetTextMatrix(1, 1, _T("�����ȣ"));
	excelMgr.SetTextMatrix(1, 2, _T("�õ�"));
	excelMgr.SetTextMatrix(1, 3, _T("��"));
	excelMgr.SetTextMatrix(1, 4, _T("��"));
	excelMgr.SetTextMatrix(1, 5, _T("����"));
	excelMgr.SetTextMatrix(1, 6, _T("��ȣ"));

	INT nRows = dm.GetRowCount();
	GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T("�����ȣ ������ �Դϴ�."));
	INT nLower = 0;
	INT nUpper = nRows;
	m_ProgressBar.SetRange(nLower, nUpper);

	for (INT row = 0; row < nRows; row++)
	{
		for (INT col = 0; col < nColCount; col++)
		{
			excelMgr.SetTextMatrix(row + 2, col + 1, dm.GetCellData(row, col));
		}
		m_ProgressBar.SetPos(nRows);
	}

	excelMgr.SaveAs(m_strFilePath);
	excelMgr.CloseManager();

	MessageBox(_T("���⼺��"));
	GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
	m_ProgressBar.SetPos(0);
	*/
	return 0;

EFS_END
GetDlgItem(IDC_STATIC_ZIP_STATUS)->SetWindowText(_T(""));
m_ProgressBar.SetPos(0);
return -1;
}

INT CAddressImpExpDlg::SendFrame(CESL_DataMgr *pDM_OUT, INT nSendCount)
{
	EFS_BEGIN

	if (!pDM_OUT) return -1;
	if (nSendCount <= 0) return -2;

	INT ids;
	while(TRUE) {
		if (pDM_OUT->frame.IsEmpty() == TRUE) break;
		ids = pDM_OUT->ORACLE_SendFrame2(nSendCount);
		if (ids < 0) return ids;
	}
	return 0;

	EFS_END
	return -1;
}

VOID CAddressImpExpDlg::OnbSTARTIMPEXP2() 
{
	EFS_BEGIN

	//if (m_bIsThreadRun == TRUE) return;

	CString sMode;
	GetDlgItem(IDC_bSTART_IMP_EXP2)->GetWindowText(sMode);
	if( sMode == _T("�ݱ�") )
	{
		OnCancel();	
	}
	else
	{
		//SetControlEnable(TRUE);
		m_bIsThreadRun = FALSE;
	}

	EFS_END
}

// 2008.05.13 ADD BY PWR 
// ** ���Լ���
void CAddressImpExpDlg::OnrIMPORT() 
{
	((CButton*)GetDlgItem(IDC_chkALLIMPORT))->EnableWindow(1);
}

// 2008.05.13 ADD BY PWR 
// ** ���⼱��
void CAddressImpExpDlg::OnrEXPORT() 
{
	((CButton*)GetDlgItem(IDC_chkALLIMPORT))->EnableWindow(0);
}

HBRUSH CAddressImpExpDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CAddressImpExpDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
