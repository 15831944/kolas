// RecycleDataRestoreDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RecycleDataRestoreDlg.h"
#include "RecycleDataSearchDlg.h"
#include "RecycleDataDetailInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecycleDataRestoreDlg dialog


CRecycleDataRestoreDlg::CRecycleDataRestoreDlg(CESL_Mgr* pParent /*=NULL*/)
	: CErrorAdjustDlg(CRecycleDataRestoreDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecycleDataRestoreDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPrintDM = NULL;
}


CRecycleDataRestoreDlg::~CRecycleDataRestoreDlg()
{
	if(m_pSearchMgr)
	{
		delete m_pSearchMgr;
		m_pSearchMgr = NULL;
	}
	if(m_pErrorAdjustApi)
	{
		delete m_pErrorAdjustApi;
		m_pErrorAdjustApi = NULL;
	}
}

VOID CRecycleDataRestoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecycleDataRestoreDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecycleDataRestoreDlg, CDialog)
	//{{AFX_MSG_MAP(CRecycleDataRestoreDlg)
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_WM_CTLCOLOR()                                                                  

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecycleDataRestoreDlg message handlers

BOOL CRecycleDataRestoreDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if (InitESL_Mgr(_T("K2UPGRADE_��Ȱ���ڷẹ��")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_��Ȱ���ڷẹ��"));
	m_pDM = FindDM(_T("DM_��Ȱ���ڷẹ��"));
	m_pPrintDM = FindDM(_T("DM_��Ȱ���ڷẹ��_��"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));	

	m_pSearchMgr = new CRecycleDataSearchDlg(this);
	m_pSearchMgr->SetErrorAdjustType(RECYCLE_DATA_RESTORE);
	
	m_pErrorAdjustApi = new CErrorAdjustApi(this);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRecycleDataRestoreDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridRECYCLE_DATA_RESTORE);
	if(pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rect;
	GetClientRect(rect);
	//rect.top += 5;
	//rect.bottom -= 5;
	//rect.left += 5;
	//rect.right -= 5;
	
	pGrid->MoveWindow(rect);	
}

BOOL CRecycleDataRestoreDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CRecycleDataRestoreDlg::CMD_ERRORADJUST()
{
	CRecycleDataDetailInfo dlg(this);
	dlg.SetOpenMode(1000);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.SetErrorAdjustApi(m_pErrorAdjustApi);
	dlg.DoModal();
	return 0;
}

INT CRecycleDataRestoreDlg::CMD_SEARCH()
{
	INT ids = ShowSearchDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�˻�ȭ���� ������ �� �����ϴ�."));
		return -1;
	}
	return 0;
}

INT CRecycleDataRestoreDlg::CMD_VIEWERRORINFO()
{
	return 0;
}

UINT CRecycleDataRestoreDlg::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	strQuery = m_pSearchMgr->GetQuery();
	if(strQuery.IsEmpty())
	{
		ESLAfxMessageBox(_T("�˻� ���Ǹ� ������ �� �����ϴ�."));
		return -1;
	}
	
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("��Ȱ�� ���� �ڷḦ �˻����Դϴ�."));
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	INT ids = m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�˻� ���Ǹ� ������ �� �����ϴ�."));
	}
	else if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
	}
	else 
	{
		m_pDataProcessingDlg->SetMessage(_T("��Ȱ�� ���� �ڷḦ �������� �� �������Դϴ�."));
		
		m_pErrorAdjustApi->InitTempData();	m_pErrorAdjustApi->InitTempPtr();
		m_pErrorAdjustApi->SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
		m_pErrorAdjustApi->SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
		m_pErrorAdjustApi->SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
		ids = m_pErrorAdjustApi->SPFExecutePath(_T("åTo��"));
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("�� �ڷḦ ������ �� �����ϴ�."));
		}
	}
	DeleteDataProcessingDlg();
	m_pGrid->Display();
	
	return 0;
	return 0;
}

INT CRecycleDataRestoreDlg::ShowSearchDlg()
{
	if(m_pSearchMgr == NULL) return -1;
	
	if(!m_pSearchMgr->GetCreate())
	{
		if(!m_pSearchMgr->CreateSearchDlg(this))
			return -1;
	}
	
	m_pSearchMgr->ShowWindow(SW_SHOW);
	m_pSearchMgr->UpdateWindow();
	m_pSearchMgr->CenterWindow(this);
	
	return 0;
}


HBRUSH CRecycleDataRestoreDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


