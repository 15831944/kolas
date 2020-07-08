// RequestErrorAdjust.cpp : implementation file
//

#include "stdafx.h"
#include "RequestErrorAdjust.h"
// 2011.09.08 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequestErrorAdjust dialog


CRequestErrorAdjust::CRequestErrorAdjust(CESL_Mgr* pParent /*=NULL*/)
	: CErrorAdjustDlg(CRequestErrorAdjust::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequestErrorAdjust)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CRequestErrorAdjust::~CRequestErrorAdjust()
{
	if(m_pErrorAdjustApi)
	{
		delete m_pErrorAdjustApi;
		m_pErrorAdjustApi = NULL;
	}
}

VOID CRequestErrorAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequestErrorAdjust)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequestErrorAdjust, CDialog)
	//{{AFX_MSG_MAP(CRequestErrorAdjust)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequestErrorAdjust message handlers

BOOL CRequestErrorAdjust::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_��ġ��������籸��")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	m_pCM = FindCM(_T("CM_��ġ��������籸��"));
	m_pDM = FindDM(_T("DM_��ġ��������籸��"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));	

	m_pErrorAdjustApi = new CErrorAdjustApi(this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRequestErrorAdjust::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridREQUEST_ERROR_ADJUST);
	if(pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rect;
	GetClientRect(rect);
	//rect.top += 5;
	//rect.bottom -= 5;
	//rect.left += 5;
	//rect.right -= 5;
	
	pGrid->MoveWindow(rect);		
}

BOOL CRequestErrorAdjust::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CRequestErrorAdjust::CMD_SEARCH()
{
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("��ġ����ڷḦ �˻��ϰ� �ֽ��ϴ�."));
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	INT ids = m_pDM->RefreshDataManager(_T(""), m_pDataProcessingDlg->GetProgressCtrl());
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�˻��� ������ �� �����ϴ�."));
	}
	else if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻������ �����ϴ�."));
	}
	DeleteDataProcessingDlg();

	// 2011.09.08 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5;
		CString strGetData;
		for(INT i = 0; i < m_pDM->GetRowCount(); i++)
		{
			m_pDM->GetCellData(_T("�ּ�"), i, strGetData);
			asDes1.Add(strGetData);
			m_pDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strGetData);
			asDes2.Add(strGetData);
			m_pDM->GetCellData(_T("EMAIL"), i, strGetData);
			asDes3.Add(strGetData);
			m_pDM->GetCellData(_T("�ڵ���"), i, strGetData);
			asDes4.Add(strGetData);
			m_pDM->GetCellData(_T("��ȭ��ȣ"), i, strGetData);
			asDes5.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("ADDRESS"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("CIVIL_NO"), asDes2);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes3);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("HANDPHONE"), asDes4);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("TEL"), asDes5);
					pApi->CloseSocket();
				}
				for(i = 0; i < m_pDM->GetRowCount(); i++)
				{
					m_pDM->SetCellData(_T("�ּ�"), asDes1.GetAt(i), i);
					m_pDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), asDes2.GetAt(i), i);
					m_pDM->SetCellData(_T("EMAIL"), asDes3.GetAt(i), i);
					m_pDM->SetCellData(_T("�ڵ���"), asDes4.GetAt(i), i);
					m_pDM->SetCellData(_T("��ȭ��ȣ"), asDes5.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
	}

	m_pGrid->Display();
	return 0;
}

INT CRequestErrorAdjust::CMD_ERRORADJUST()
{
	if(m_pErrorAdjustApi == NULL)
	{
		ESLAfxMessageBox(_T("��ġ����ڷ� ���������� �籸���� �� �����ϴ�."));
		return -1;
	}
	INT ids = CheckSelectDataValid(m_pDM);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return -1;
	}
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("������ �ڷῡ ���� ���ο� ���������� �����մϴ�."));
	
	m_pErrorAdjustApi->InitTempData();	m_pErrorAdjustApi->InitTempPtr();
	m_pErrorAdjustApi->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pErrorAdjustApi->SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	m_pErrorAdjustApi->SetTempPtr(_T("ProgressCTRL"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	ids = m_pErrorAdjustApi->SPFExecutePath(_T("��ġ��������籸��"));
	DeleteDataProcessingDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("��ġ����ڷ� ���������� �籸���� �� �����ϴ�."));
		return ids;
	}
	
	m_pGrid->Display();
	return 0;
}
