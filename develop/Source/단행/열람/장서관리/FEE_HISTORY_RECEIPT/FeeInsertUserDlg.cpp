// FeeInsertUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FeeInsertUserDlg.h"
#include "FeeInsertUserSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeInsertUserDlg dialog


CFeeInsertUserDlg::CFeeInsertUserDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFeeInsertUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeInsertUserDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFeeInsertUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeInsertUserDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeInsertUserDlg, CDialog)
	//{{AFX_MSG_MAP(CFeeInsertUserDlg)
	ON_BN_CLICKED(IDC_btnInsertUserSearch, OnbtnInsertUserSearch)
	ON_BN_CLICKED(IDC_btnInsertUserOK, OnbtnInsertUserOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFeeInsertUserDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CFeeInsertUserDlg)
	ON_EVENT(CFeeInsertUserDlg, IDC_girdInsertUser, -601 /* DblClick */, OnDblClickgirdInsertUser, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeInsertUserDlg message handlers

BOOL CFeeInsertUserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// XP THEME
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	if( InitESL_Mgr( _T("FEE_INSERT_USER") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : FEE_INSERT_USER") );
		return FALSE;
	}

	//JOB.2019.0017 ADD BY KYJ
	//�������� ���� ����,�α� ������ ����
	//-------------------------------------------------------------------------
	CESL_ControlMgr* pCM =FindCM(_T("CM_FEE_INSERT_USER"));
	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("������ �Է� - ������ �˻�");
	}
	//-------------------------------------------------------------------------

	return TRUE;
}

void CFeeInsertUserDlg::OnbtnInsertUserSearch() 
{
	CFeeInsertUserSearchDlg SearchDlg(this);
	// 2010.09.08 ADD BY KSJ : ���յ������� ��뿩��
	SearchDlg.m_sIsUnityLoanService = m_sIsUnityLoanService;
	SearchDlg.DoModal();
}

void CFeeInsertUserDlg::OnbtnInsertUserOK() 
{
	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_INSERT_USER") );
	if( pDM == NULL ) 
	{
		ERROR_MSG_RETURN_VOID2( -1000, _T("OnbtnInsertUserOK") );
	}
	if( 1 > pDM->GetRowCount())
	{
		return;
	}

	CESL_ControlMgr *pCM = FindCM( _T("CM_FEE_INSERT_USER") );
	if( pCM == NULL )
	{
		ERROR_MSG_RETURN_VOID2( -1010, _T("OnbtnInsertUserOK") );
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if( pGrid == NULL )
	{
		ERROR_MSG_RETURN_VOID2( -1020, _T("OnbtnInsertUserOK") );
	}
	
	pGrid->SelectMakeList();
	INT nSelGridCnt = pGrid->SelectGetCount();
	if ( nSelGridCnt > 1 ) 
	{
		AfxMessageBox( _T("������ ������ �ݵ�� �Ѱ��� ���� �����Ǿ�� �մϴ�."));
		return;
	}
	else if( nSelGridCnt < 1 )
	{
		return;
	}

	INT nIndex = pGrid->SelectGetIdx();

	ids = SetUser(nIndex);
	if(0>ids)
	{
		OnCancel();
		return;
	}

	OnOK();
}

void CFeeInsertUserDlg::OnDblClickgirdInsertUser() 
{
	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_INSERT_USER") );
	if( pDM == NULL ) 
	{
		ERROR_MSG_RETURN_VOID2( -1000, _T("OnDblClickgirdInsertUser") );
	}
	if( 1 > pDM->GetRowCount())
	{
		return;
	}

	CESL_ControlMgr *pCM = FindCM( _T("CM_FEE_INSERT_USER") );
	if( pCM == NULL )
	{
		ERROR_MSG_RETURN_VOID2( -1010, _T("OnDblClickgirdInsertUser") );
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if( pGrid == NULL )
	{
		ERROR_MSG_RETURN_VOID2( -1020, _T("OnDblClickgirdInsertUser") );
	}

	if( pGrid->GetMouseRow() < 1 ) 
	{
		return;
	}
	
	INT nIndex = pGrid->GetIdx();

	ids = SetUser(nIndex);
	if(0>ids)
	{
		OnCancel();
		return;
	}
	
	OnOK();
}

INT CFeeInsertUserDlg::SetUser(INT nIndex)
{
	INT ids;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_INSERT_USER") );
	if( pDM == NULL ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020, _T("SetUser") );
	}
	ids = pDM->GetCellData(_T("REC_KEY"), nIndex, m_strUserKey );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030, _T("SetUser") );
	}
	ids = pDM->GetCellData(_T("�������̸�"), nIndex, m_strUserName );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040, _T("SetUser") );
	}
	ids = pDM->GetCellData(_T("�����ڹ�ȣ"), nIndex, m_strUserNo );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050, _T("SetUser") );
	}
	ids = pDM->GetCellData(_T("�����ڹ�ȣ����"), nIndex, m_strUserNoDesc );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050, _T("SetUser") );
	}

	return 0;
}

BOOL CFeeInsertUserDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( VK_RETURN == pMsg->wParam )
	{
		if( WM_KEYDOWN == pMsg->message || WM_CHAR == pMsg->message )
		{
			// �˻�
			if( GetDlgItem(IDC_btnInsertUserSearch)->GetSafeHwnd() == pMsg->hwnd)
			{
				// �˻�����
				OnbtnInsertUserSearch();
			}
			// Ȯ��
			else if( GetDlgItem(IDC_btnInsertUserOK)->GetSafeHwnd() == pMsg->hwnd)
			{
				// Ȯ������
				OnbtnInsertUserOK();
			}
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
