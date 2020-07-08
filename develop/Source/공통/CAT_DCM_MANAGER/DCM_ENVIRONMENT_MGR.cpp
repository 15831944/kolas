// DCM_ENVIRONMENT_MGR.cpp : implementation file
//

#include "stdafx.h"
#include "DCM_ENVIRONMENT_MGR.h"
#include "DCM_INFO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCM_ENVIRONMENT_MGR dialog


CDCM_ENVIRONMENT_MGR::CDCM_ENVIRONMENT_MGR(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDCM_ENVIRONMENT_MGR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDCM_ENVIRONMENT_MGR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_bActiveChange = FALSE;
	m_strDbFlag = _T("M");
	m_strParentFileType = _T("PDF");
}

CDCM_ENVIRONMENT_MGR::~CDCM_ENVIRONMENT_MGR()
{

}


void CDCM_ENVIRONMENT_MGR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDCM_ENVIRONMENT_MGR)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDCM_ENVIRONMENT_MGR, CDialog)
	//{{AFX_MSG_MAP(CDCM_ENVIRONMENT_MGR)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnInput, OnbtnInput)
	ON_BN_CLICKED(IDC_btnModify, OnbtnModify)
	ON_BN_CLICKED(IDC_btnDelete, OnbtnDelete)
	ON_BN_CLICKED(IDC_btnActiveServer, OnbtnActiveServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCM_ENVIRONMENT_MGR message handlers

HBRUSH CDCM_ENVIRONMENT_MGR::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CDCM_ENVIRONMENT_MGR::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	if( _T("M") == m_strDbFlag )
	{
		if (InitESL_Mgr(_T("SM_BO_DCMȯ�漳��")) < 0)
		{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}	
		
		m_pCM = FindCM(_T("CM_BO_DCMȯ�漳��"));
		m_pDM = FindDM(_T("DM_BO_DCMȯ�漳��"));
	}
	else if( _T("Y") == m_strDbFlag )
	{
		if (InitESL_Mgr(_T("SM_SE_DCMȯ�漳��")) < 0)
		{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}
		
		m_pCM = FindCM(_T("CM_SE_DCMȯ�漳��"));
		m_pDM = FindDM(_T("DM_SE_DCMȯ�漳��"));
	}
	else if( _T("A") == m_strDbFlag )
	{
		if (InitESL_Mgr(_T("SM_SE_ATICLE_DCMȯ�漳��")) < 0)
		{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}
		
		m_pCM = FindCM(_T("CM_SE_ATICLE_DCMȯ�漳��"));
		m_pDM = FindDM(_T("DM_SE_ATICLE_DCMȯ�漳��"));
	}
	else
	{
		AfxMessageBox(_T("DB FLAG�� �������ּ���."));
		return FALSE;
	}
	
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("DcmEnvGrid"));

	if(!Init())
	{
		ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDCM_ENVIRONMENT_MGR::Init()
{
	CString strOption;

// 	INT ids = m_pDM->RefreshDataManager(_T(""));
	strOption.Format( _T("TYPE = '%s'"), m_strParentFileType );
	INT ids = m_pDM->RefreshDataManager( strOption );
	if(ids < 0) return FALSE;
// 	nDmRowCnt = m_pDM->GetRowCount();
	
	m_pDM->SORT(_T("����ID"));	
	m_pGrid->Display();
	m_pGrid->SetReverse(0);

	return TRUE;
}

void CDCM_ENVIRONMENT_MGR::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CButton *pBtn = (CButton*)GetDlgItem(IDC_btnInput);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->MoveWindow( 7, cy - 29, 87, 21);

	CButton *pBtn1 = (CButton*)GetDlgItem(IDC_btnModify);
	if(pBtn1->GetSafeHwnd() == NULL) return;
	pBtn1->MoveWindow(102, cy - 29, 87, 21);

	CButton *pBtn2 = (CButton*)GetDlgItem(IDC_btnDelete);
	if(pBtn2->GetSafeHwnd() == NULL) return;
	pBtn2->MoveWindow(197, cy - 29, 87, 21);

	CButton *pBtn3 = (CButton*)GetDlgItem(IDC_btnActiveServer);
	if(pBtn3->GetSafeHwnd() == NULL) return;
	pBtn3->MoveWindow(cx - 187, cy - 29, 87, 21);

	CButton *pBtn4 = (CButton*)GetDlgItem(IDCANCEL);
	if(pBtn4->GetSafeHwnd() == NULL) return;
	pBtn4->MoveWindow(cx - 92, cy - 29, 87, 21);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridDCM_ENVIRONMENT_MGR);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy - 40);
}

void CDCM_ENVIRONMENT_MGR::OnbtnInput() 
{
	OnOpenDcmInfo(0) ;
}

void CDCM_ENVIRONMENT_MGR::OnbtnModify() 
{
	OnOpenDcmInfo(1) ;
}

void CDCM_ENVIRONMENT_MGR::OnbtnDelete() 
{
	CString strQuery;
	CString strData;
	CString strSeverId;
	CString strContentsType;
	INT nSelectCnt;
// 	INT nDmRowCnt;
	INT nIdx;
	INT nIds;
	
	m_pGrid->SelectMakeList();
	nSelectCnt = m_pGrid->SelectGetCount();
	nIdx = m_pGrid->SelectGetHeadPosition();

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		strData = m_pDM->GetCellData(_T("����ID"),nIdx);
		if( strSeverId.IsEmpty() )
		{
			strSeverId = strData;
		}
		else
		{
			strSeverId += _T(", ") + strData;
		}

		nIdx = m_pGrid->SelectGetNext();		
	}

	if( strSeverId.IsEmpty() )
	{
		AfxMessageBox(_T("������ �����͸� ������ �ּ���"));
		return;
	}
	else
	{
		strData.Format(_T("%s ������ ���� �Ͻðڽ��ϱ�?"), strSeverId);
		nIds = pParentMgr->ESLAfxMessageBox(strData,MB_YESNO);
		if( nIds != IDYES )
		{
			return;
		}
	}
	nIds = m_pDM->StartFrame();

	strQuery.Format(_T("DELETE FROM DL_CONTENTS_SERVER_TBL WHERE SERVER_ID IN( %s );"), strSeverId );
	m_pDM->AddFrame(strQuery);
	
	nIds = m_pDM->SendFrame();
  	if ( nIds < 0 )
  	{
  		AfxMessageBox(_T("DB���� �۾��� �����߽��ϴ�."));
  		return ;
  	}
 	
  	nIds = m_pDM->EndFrame();
  	if ( nIds < 0 )
  	{
  		AfxMessageBox(_T("DB���� �۾��� �����߽��ϴ�."));
  		return ;
  	}
	strData.Format(_T("%s ������ ���� �Ͽ����ϴ�."), strSeverId);
	AfxMessageBox( strData );

	nIdx = m_pGrid->SelectGetTailPosition();
	for( i = 0 ; i < nSelectCnt ; i++ )
	{
		m_pDM->DeleteRow(nIdx);
		nIdx = m_pGrid->SelectGetPrev();		
	}	

	m_pGrid->Display();

	m_bActiveChange = TRUE;
}

VOID CDCM_ENVIRONMENT_MGR::OnOpenDcmInfo(INT nOpenMode) 
{
	CString strOption;	
	INT nIdx;
// 	INT nIds;
	
	CDCM_INFO dlg(this);
	dlg.SetParentDM(m_pDM);
	dlg.SetParentGRID(m_pGrid);
	dlg.SetOpenMode(nOpenMode);
	nIdx = m_pGrid->GetIdx()-1;
	dlg.SetIndex( nIdx );
	dlg.SetDbFlag(m_strDbFlag);

 	dlg.SetParentFileType(m_strParentFileType);

	dlg.DoModal();
	m_bActiveChange = dlg.GetActiveChange();
}



BEGIN_EVENTSINK_MAP(CDCM_ENVIRONMENT_MGR, CDialog)
    //{{AFX_EVENTSINK_MAP(CDCM_ENVIRONMENT_MGR)
	ON_EVENT(CDCM_ENVIRONMENT_MGR, IDC_gridDCM_ENVIRONMENT_MGR, -601 /* DblClick */, OnDblClickgridDCMENVIRONMENTMGR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDCM_ENVIRONMENT_MGR::OnDblClickgridDCMENVIRONMENTMGR() 
{
	INT nSelectCnt;
	INT nIdx;
	m_pGrid->SelectMakeList();
	nSelectCnt = m_pGrid->SelectGetCount();
	if( 0 <nSelectCnt )
	{
		nIdx = m_pGrid->GetIdx();
		m_pGrid->SetTextMatrix(nIdx+1, 1,_T("V"));
		m_pGrid->SelectMakeList();
	}

	OnOpenDcmInfo( 2 ); 		
}

void CDCM_ENVIRONMENT_MGR::OnbtnActiveServer() 
{
	CString strQuery;
	CString strData;
	CString strSeverId;
	CString strContentsType;
	INT nSelectCnt;
	INT nDmRowCnt;
	INT nIdx;
	INT nIds;
	m_pGrid->SelectMakeList();
	m_pGrid->SetProtectSelectColumn(TRUE);
	nSelectCnt = m_pGrid->SelectGetCount();
	if( 1 == nSelectCnt )
	{
		nIdx = m_pGrid->SelectGetHeadPosition();
		strSeverId = m_pDM->GetCellData(_T("����ID"),nIdx);
		strContentsType = m_pDM->GetCellData(_T("CONTENTS_TYPE"),nIdx);

		nIds = m_pDM->StartFrame();

		strQuery.Format(_T("UPDATE DL_CONTENTS_SERVER_TBL SET ACTIVE_FLAG = 'N' WHERE TYPE = '%s' AND ACTIVE_FLAG = 'Y';"), strContentsType );
		m_pDM->AddFrame(strQuery);
		
		strQuery.Format(_T("UPDATE DL_CONTENTS_SERVER_TBL SET ACTIVE_FLAG = 'Y' WHERE SERVER_ID = %s;"), strSeverId );
		m_pDM->AddFrame(strQuery);
		
		nIds = m_pDM->SendFrame();
  		if ( nIds < 0 )
  		{
			m_pGrid->SetProtectSelectColumn(FALSE);
  			AfxMessageBox(_T("DB���� �۾��� �����߽��ϴ�."));
  			return ;
  		}
 		
  		nIds = m_pDM->EndFrame();
  		if ( nIds < 0 )
  		{
			m_pGrid->SetProtectSelectColumn(FALSE);
  			AfxMessageBox(_T("DB���� �۾��� �����߽��ϴ�."));
  			return ;
  		}
		strData.Format(_T("%s�� ������ Ȱ��ȭ �Ͽ����ϴ�."), strSeverId);
		AfxMessageBox( strData );

		
		nDmRowCnt = m_pDM->GetRowCount();
		
		for( INT i = 0 ; i < nDmRowCnt ; i++ )
		{
			strData = m_pDM->GetCellData(_T("CONTENTS_TYPE"),i);			
			if( strContentsType == strData )
			{
				m_pDM->SetCellData(_T("Ȱ��ȭ����"),_T("N"),i);
			}
		}

		m_pDM->SetCellData(_T("Ȱ��ȭ����"),_T("Y"),nIdx);
		m_pGrid->Display();
		m_pGrid->SetReverse(nIdx);
	}
	else if( 1 < nSelectCnt )
	{
		AfxMessageBox( _T("�ϳ��� �����͸� ������ �ּ���.")	);
	}
	else 
	{
		AfxMessageBox( _T("�����͸� ������ �ּ���.")	);
	}

	m_pGrid->SetProtectSelectColumn(FALSE);

	m_bActiveChange = TRUE;
}
