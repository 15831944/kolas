// BO_DonateErrorAdjust.cpp : implementation file
//

#include "stdafx.h"
#include "BO_DonateErrorAdjust.h"
#include "../BO_ACQ_API/BO_ACQ_Business_Api.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_DonateErrorAdjust dialog


CBO_DonateErrorAdjust::CBO_DonateErrorAdjust(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_DonateErrorAdjust::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_DonateErrorAdjust)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pPrintDM = NULL;
	m_pGrid = NULL;
}

CBO_DonateErrorAdjust::~CBO_DonateErrorAdjust()
{
	
}


VOID CBO_DonateErrorAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_DonateErrorAdjust)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_DonateErrorAdjust, CDialog)
	//{{AFX_MSG_MAP(CBO_DonateErrorAdjust)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_DonateErrorAdjust message handlers

VOID CBO_DonateErrorAdjust::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CBO_DonateErrorAdjust::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_��������")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_��������"));
	m_pDM = FindDM(_T("DM_��������"));
	m_pPrintDM = FindDM(_T("DM_��������_��"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_DonateErrorAdjust::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

VOID CBO_DonateErrorAdjust::AdjustDonateDataError()
{
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();	mgr.InitTempPtr();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	INT ids = mgr.SPFExecutePath(_T("����������������"));
	if(ids < 0)
	{
		this->ESLAfxMessageBox(_T("���Կ����� ������ �� �����ϴ�."));
		return ;
	}
	MakeDonateErrorData();
}

VOID CBO_DonateErrorAdjust::SynchronizedBookToIndex()
{
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();	mgr.InitTempPtr();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	INT ids = mgr.SPFExecutePath(_T("å������������ȭ"));
	if(ids < 0)
	{
		this->ESLAfxMessageBox(_T("å������������ȭ�� ������ �� �����ϴ�."));
		return ;
	}
	ESLAfxMessageBox(_T("å�� ���������� ����ȭ���׽��ϴ�."));
	return;
}

BOOL CBO_DonateErrorAdjust::Create(CWnd *pWnd)
{
	return CDialog::Create(IDD, pWnd);
}

VOID CBO_DonateErrorAdjust::MakeDonateErrorData()
{
	CString strQuery;
	strQuery.Format(_T("I.REC_KEY=S.REC_KEY AND S.REC_KEY=B.SPECIES_KEY AND B.ACQ_CODE='2' AND B.ACQ_KEY IS NULL AND B.WORKING_STATUS < 'BOC'"));
	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0) return ;
	m_pPrintDM->FreeData();
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)NULL);
	ids = mgr.SPFExecutePath(_T("åTo��"));

	m_pGrid->Display();
}

HBRUSH CBO_DonateErrorAdjust::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
