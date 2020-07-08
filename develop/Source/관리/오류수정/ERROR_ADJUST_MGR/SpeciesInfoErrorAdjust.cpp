// SpeciesInfoErrorAdjust.cpp : implementation file
//

#include "stdafx.h"
#include "SpeciesInfoErrorAdjust.h"
#include "SpeciesErrorDataSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpeciesInfoErrorAdjust dialog


CSpeciesInfoErrorAdjust::CSpeciesInfoErrorAdjust(CESL_Mgr* pParent /*=NULL*/)
	: CErrorAdjustDlg(CSpeciesInfoErrorAdjust::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpeciesInfoErrorAdjust)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nSpeciesType = -1;
}

CSpeciesInfoErrorAdjust::~CSpeciesInfoErrorAdjust()
{
	if(m_pErrorAdjustApi)
	{
		delete m_pErrorAdjustApi;
		m_pErrorAdjustApi = NULL;
	}
	if(m_pSearchMgr)
	{
		delete m_pSearchMgr;
		m_pSearchMgr = NULL;
	}
}

VOID CSpeciesInfoErrorAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpeciesInfoErrorAdjust)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpeciesInfoErrorAdjust, CDialog)
	//{{AFX_MSG_MAP(CSpeciesInfoErrorAdjust)
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpeciesInfoErrorAdjust message handlers

VOID CSpeciesInfoErrorAdjust::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridSPECIESINFO_ERROR_ADJUST);
	if(pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rect;
	GetClientRect(rect);
	//rect.top += 5;
	//rect.bottom -= 5;
	//rect.left += 5;
	//rect.right -= 5;
	
	pGrid->MoveWindow(rect);		
}

BOOL CSpeciesInfoErrorAdjust::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_��������������")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	m_pCM = FindCM(_T("CM_��������������"));
	m_pDM = FindDM(_T("DM_��������������"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));	
	
	m_pErrorAdjustApi = new CErrorAdjustApi(this);	
	m_pSearchMgr = new CSpeciesErrorDataSearchDlg(this);
	m_pSearchMgr->SetErrorAdjustType(INDEX_ERROR_ADJUST);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSpeciesInfoErrorAdjust::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CSpeciesInfoErrorAdjust::CMD_SEARCH()
{
	INT ids = ShowSearchDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�˻�ȭ���� ������ �� �����ϴ�."));
		return -1;
	}
	return 0;
}

INT CSpeciesInfoErrorAdjust::CMD_ERRORADJUST()
{
	if(m_pErrorAdjustApi == NULL)
	{
		ESLAfxMessageBox(_T("������ �������� �۾��� ������ �� �����ϴ�."));
		return -1;
	}
	INT ids = CheckSelectDataValid(m_pDM);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return -1;
	}
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("������ �ڷῡ ���Ͽ� ������ ������ �����մϴ�."));
	
	m_pErrorAdjustApi->InitTempData();	m_pErrorAdjustApi->InitTempPtr();
	m_pErrorAdjustApi->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pErrorAdjustApi->SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	m_pErrorAdjustApi->SetTempPtr(_T("ProgressCTRL"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	m_pErrorAdjustApi->SetTempData(_T("SPECIES_TYPE"), m_nSpeciesType);
	ids = m_pErrorAdjustApi->SPFExecutePath(_T("��������������"));
	DeleteDataProcessingDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �������� �۾��� �����ϴ� ���� ������ �߻��Ͽ����ϴ�."));
		return ids;
	}
	
	m_pGrid->Display();
	return 0;
}

INT CSpeciesInfoErrorAdjust::ShowSearchDlg()
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

UINT CSpeciesInfoErrorAdjust::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	strQuery = m_pSearchMgr->GetQuery();
	if(strQuery.IsEmpty())
	{
		ESLAfxMessageBox(_T("�˻� ���Ǹ� ������ �� �����ϴ�."));
		return -1;
	}
	
	m_nSpeciesType = lParam;
	if(m_nSpeciesType == 0)	m_pDM->TBL_NAME = _T("IDX_BO_TBL I, BO_SPECIES_TBL S, BO_BOOK_TBL B");
	else if(m_nSpeciesType == 1) m_pDM->TBL_NAME = _T("IDX_BO_TBL I, BO_DELETE_SPECIES_TBL S, BO_DELETE_BOOK_TBL B");
	else m_pDM->TBL_NAME = _T("IDX_BO_TBL I, BO_SPECIES_TBL S, BO_BOOK_TBL B");
	
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("������ �����ڷḦ �˻����Դϴ�."));
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
	DeleteDataProcessingDlg();
	m_pGrid->Display();
	
	return 0;
}