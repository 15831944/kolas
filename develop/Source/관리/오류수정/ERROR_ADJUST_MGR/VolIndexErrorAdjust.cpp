// VolIndexErrorAdjust.cpp : implementation file
//

#include "stdafx.h"
#include "VolIndexErrorAdjust.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolIndexErrorAdjust dialog


CVolIndexErrorAdjust::CVolIndexErrorAdjust(CESL_Mgr* pParent /*=NULL*/)
	: CErrorAdjustDlg(CVolIndexErrorAdjust::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVolIndexErrorAdjust)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPrintDM = NULL;
}

CVolIndexErrorAdjust::~CVolIndexErrorAdjust()
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

VOID CVolIndexErrorAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolIndexErrorAdjust)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolIndexErrorAdjust, CDialog)
	//{{AFX_MSG_MAP(CVolIndexErrorAdjust)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolIndexErrorAdjust message handlers

BOOL CVolIndexErrorAdjust::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CVolIndexErrorAdjust::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if (InitESL_Mgr(_T("K2UPGRADE_å���ε���ȭ")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_å���ε���ȭ"));
	m_pDM = FindDM(_T("DM_å���ε���ȭ"));
	m_pPrintDM = FindDM(_T("DM_å���ε���ȭ_��"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));

	m_pSearchMgr = new CErrorAdjustSearchMgr(0, this);
	m_pSearchMgr->SetErrorAdjustType(VOLINDEX_ERROR_ADJUST);

	m_pErrorAdjustApi = new CErrorAdjustApi(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CVolIndexErrorAdjust::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridVOLINDEX_ERROR_ADJUST);
	if(pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rect;
	GetClientRect(rect);
	//rect.top += 5;
	//rect.bottom -= 5;
	//rect.left += 5;
	//rect.right -= 5;
	
	pGrid->MoveWindow(rect);	
}

INT CVolIndexErrorAdjust::CMD_SEARCH()
{
	CString strQuery;
	strQuery = m_pSearchMgr->GetQuery();
	if(strQuery.IsEmpty())
	{
		ESLAfxMessageBox(_T("�˻������� ������ �� �����ϴ�."));
		return -1;
	}
	
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("å-���� ����ȭ�ڷḦ �˻��� �Դϴ�."));
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	INT ids = m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�˻��� ������ �� �����ϴ�."));
	}
	else if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
	}
	else 
	{
		m_pDataProcessingDlg->SetMessage(_T("å-���� ����ȭ�ڷḦ �������� �� �������Դϴ�."));

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
}

INT CVolIndexErrorAdjust::CMD_REFRESH()
{
	return CMD_SEARCH();
}

INT CVolIndexErrorAdjust::CMD_ERRORADJUST()
{
	if(m_pErrorAdjustApi == NULL)
	{
		ESLAfxMessageBox(_T("å-�������� ����ȭ �۾��� ������ �� �����ϴ�."));
		return -1;
	}
	INT ids = CheckSelectDataValid(m_pPrintDM);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return -1;
	}	
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("������ �ڷ��� ��/�������� ����ȭ �۾��� �����մϴ�."));
	
	m_pErrorAdjustApi->InitTempData();	m_pErrorAdjustApi->InitTempPtr();
	m_pErrorAdjustApi->SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	m_pErrorAdjustApi->SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	m_pErrorAdjustApi->SetTempPtr(_T("ProgressCTRL"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	ids = m_pErrorAdjustApi->SPFExecutePath(_T("å���ε���ȭ"));
	
	DeleteDataProcessingDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("å-�������� ����ȭ �۾��� �����ϴ� ���� ������ �߻��Ͽ����ϴ�."));
		return ids;
	}
	
	m_pGrid->Display();
	return 0;
}

INT CVolIndexErrorAdjust::CMD_ERRORADJUST1()
{
	if(m_pErrorAdjustApi == NULL)
	{
		ESLAfxMessageBox(_T("å-�������� ����ȭ �۾��� ������ �� �����ϴ�."));
		return -1;
	}
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("å/�η� �÷��װ� NULL�� �ڷḦ 'B'�� �����մϴ�."));
	
	m_pErrorAdjustApi->InitTempData();	m_pErrorAdjustApi->InitTempPtr();
	m_pErrorAdjustApi->SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	m_pErrorAdjustApi->SetTempPtr(_T("ProgressCTRL"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	INT ids = m_pErrorAdjustApi->SPFExecutePath(_T("å�η��÷��׼���"));
	DeleteDataProcessingDlg();
	if(ids == 1000)
	{
		ESLAfxMessageBox(_T("å/�η� �÷��׸� ������ �ڷᰡ �����ϴ�."));
		return 0;
	}
	else if(ids < 0)
	{
		ESLAfxMessageBox(_T("å/�η� �÷��׸� ������ �� �����ϴ�."));
		return 0;
	}

	ESLAfxMessageBox(_T("å/�η� �÷��װ� NULL�� �ڷḦ 'B'�� �����Ͽ����ϴ�."));
	return 0;
}