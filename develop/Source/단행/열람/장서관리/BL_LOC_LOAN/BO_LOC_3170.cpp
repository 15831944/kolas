#include "stdafx.h"
#include "BO_LOC_3170.h"
#include "SMSPreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBO_LOC_3170::CBO_LOC_3170(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager, CLonaManageValue *pLona, INT nMailClass )
	: CESL_Mgr(IDD, pParent)
{	
	m_pLona = pLona;
	m_nMailClass = nMailClass;

	m_bEdit = FALSE;

	m_pSMSContentsEditor = NULL;

	m_pLoanShareManager = pLoanShareManager;

	m_nMenuFlag = 0;	
	
	m_ReceiptDlg = NULL;

	m_nReserveInfoMode	=	0;
	
}
CBO_LOC_3170::~CBO_LOC_3170()
{
	if( m_pReservationInfo != NULL )
		delete m_pReservationInfo;
	m_pReservationInfo = NULL;	
	
	if( m_ReceiptDlg )
		delete m_ReceiptDlg;
	m_ReceiptDlg = NULL;

	if( m_pSMSContentsEditor )
		delete m_pSMSContentsEditor;
	m_pSMSContentsEditor = NULL;
}


VOID CBO_LOC_3170::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3170)	
	//}}AFX_DATA_MAP
	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3170, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3170)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnRESERVATIONCLOSE, OnbtnRESERVATIONCLOSE)
	ON_BN_CLICKED(IDC_btnDUEDATE, OnbtnDUEDATE)
	ON_BN_CLICKED(IDC_btnSENDMAIL, OnbtnSENDMAIL)
	ON_BN_CLICKED(IDC_btnLOAN_CANCEL, OnbtnCANCEL)
	ON_BN_CLICKED(IDC_btnSENDSMS, OnbtnSENDSMS)
	ON_BN_CLICKED(IDC_btnPrintTest, OnbtnPrintTest)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBO_LOC_3170::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3170::OnClose() 
{
EFS_BEGIN
		
	CESL_ControlMgr* pCM;

	if( !m_pLona->m_bVolReserve )
	{
		CString strCMAlias[3] = { _T("CM_BO_LOC_3170_RESERVE_INFO"),
								  _T("CM_BO_LOC_3170_BOOK_INFO"),
								  _T("CM_BO_LOC_3170_LOAN_USER_INFO") };
		for( INT i=0;i<3;i++ )
		{
			if( pCM = FindCM( strCMAlias[i] ) )			
			{
				pCM->AllControlClear();
			}
		}		
	}
	else
	{
		CString strCMAlias[2] = { _T("CM_BO_LOC_3170_VR_RESERVE_INFO"),
								  _T("CM_BO_LOC_3170_VR_BOOK_INFO")};								  
		for( INT i=0;i<2;i++ )
		{
			if( pCM = FindCM( strCMAlias[i] ) )			
			{
				pCM->AllControlClear();
			}
		}		
	}

	{
		OnCancel();
	}
	
EFS_END	
}

BOOL CBO_LOC_3170::OnInitDialog() 
{
	EFS_BEGIN
		
	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids = 0;
	CString sSMAlias = _T("");

	if ( !m_pLona->m_bVolReserve )
	{
		CDialog::OnInitDialog();
		sSMAlias = _T("BO_LOC_3170");
	}
	else
		sSMAlias = _T("BO_LOC_3170_VOL_RESERVE");

	if(InitESL_Mgr( sSMAlias ) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	CString sCMAlis;
	CString sGridName
		
		,sGridName2
		;
	if ( !m_pLona->m_bVolReserve)
	{
		sCMAlis = _T("CM_BO_LOC_3170_RESERVE_INFO");
		sGridName = _T("ReserveGrid");
	}
	else
	{
		sCMAlis = _T("CM_BO_LOC_3170_VR_RESERVE_INFO");
		sGridName = _T("VRReserveGrid");
		
		sGridName2 = _T("VRLoanInfoGrid");
	}
	
	CESL_ControlMgr* pCM = FindCM(sCMAlis);
	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl(sGridName);

	
	if ( m_pLona->m_bVolReserve == TRUE )
	{
		CESL_Grid *pGrid2 = (CESL_Grid*)pCM->FindControl(sGridName2);
		if(!m_pInfo->m_bUserInfoExportYN)
		{
			pGrid2->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
		}
		else
		{
			pGrid2->m_bPrivacyPrintLogRecord = TRUE;
			pGrid2->m_strPrintInfo = _T("������º���");
		}
	}

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("������º���");
	}

	if ( m_pLona->m_sLoanUserAccessMode.IsEmpty() || m_pLona->m_sLoanUserAccessMode == _T("1") )
	{
		if ( m_pLona->m_bVolReserve)
		{
			CESL_Grid *pGrid2 = (CESL_Grid*)pCM->FindControl(sGridName2);
			pGrid2->SetColumnEncryption(_T("�޴�����ȣ"),TRUE);
			pGrid2->SetColumnEncryption(_T("����ȭ��ȣ"),TRUE);
			pGrid2->SetColumnEncryption(_T("�������̸�"),TRUE);
			pGrid2->SetColumnEncryption(_T("�ٹ���������ȣ"),TRUE);
			
			pGrid->SetColumnEncryption(_T("�μ���"),TRUE);
			pGrid->SetColumnEncryption(_T("������ȭ��ȣ"),TRUE);
		}
		else
		{		
			((CEdit*)GetDlgItem(IDC_edtRESERVATION8))->SetPasswordChar(0x2022);				
		}
		pGrid->SetColumnEncryption(_T("EMAIL"),TRUE);
		pGrid->SetColumnEncryption(_T("��ȭ��ȣ"),TRUE);
		pGrid->SetColumnEncryption(_T("�ڵ���"),TRUE);		
		pGrid->SetColumnEncryption(_T("������"),TRUE);
		
	}
	

	m_pReservationInfo = new CBL_LOC_RESERVATOIN_PROC( this, m_pLoanShareManager, m_pLona );
	m_pReservationInfo->SetInfo( m_sSpeciesKey, m_sVolInfoKey, m_sPublishFormCode, m_sBookKey, m_nMenuFlag );
	
	ids = SearchRerserveInfo();
	if( ids > 0 )
	{
		m_bEdit = FALSE;
		OnCancel();
	}
	if( ids < 0 ) return -1;

	ids = ViewScreen();
	if( ids < 0 )return -1;
		
	CString strQuery,strResult;
	strQuery.Format( _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='��Ÿ' ")
				       _T(" AND GROUP_3='����' AND CLASS_ALIAS='SMS��������' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE );	

	ids = m_pLona->m_pAllSettingValueDM->GetOneValueQuery( strQuery, strResult );
	if( 0 > ids && -4001 != ids )
	{
		return -1;
	}
	
	if( strResult.IsEmpty() )
	{
		strResult = _T("N");
	}
	strResult.MakeUpper();

	if( _T("N") == strResult )
	{
		if ( !m_pLona->m_bVolReserve )
		{
			( GetDlgItem(IDC_btnSENDSMS) )->ShowWindow( FALSE );
		}
		else
		{
			( GetDlgItem(IDC_BTN_SEND_SMS_RESERVE_VOL) )->ShowWindow( FALSE );
		}
	}
	else
	{
		CStringList SMSList;
		SMSList.AddTail( _T("SMS01") );
		SMSList.AddTail( _T("SMS02") );
		SMSList.AddTail( _T("SMS03") );

		m_pSMSContentsEditor = new CSMSContentsEditDlg( this, SMSList );
		m_pSMSContentsEditor->Create( this );
		m_pSMSContentsEditor->CenterWindow();
		m_pSMSContentsEditor->ShowWindow( false );
	}
		
	if(TRUE != m_pLona->m_bVolReserve)
	{		
		if( m_pLona->m_sUseReceipt == _T("Y") )
		{				
			CButton* pBtn = (CButton*)GetDlgItem(IDC_btnPrintTest);
			if(NULL != pBtn && NULL != pBtn->GetSafeHwnd())
			{
				pBtn->ShowWindow(SW_SHOW);
			}		
		}
	}

	if(m_pInfo->MODE==10000)
	{
		(CButton*)GetDlgItem(IDC_btnSENDMAIL)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_btnSENDSMS)->EnableWindow(FALSE);
	}

	return TRUE;  
	              
	EFS_END
	return FALSE;
}

INT CBO_LOC_3170::ViewScreen()
{
EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("");
	CString sOrder = _T("");

	
	ids = ViewDescWorkingStatus();
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ViewScreen") );

	
	if ( TRUE != m_pLona->m_bVolReserve )
	{
		sDMAlias = _T("DM_BO_LOC_3170_RESERVE_INFO");
	}
	else
	{
		sDMAlias = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
	}
	
	CESL_DataMgr *pReserveDM = FindDM( sDMAlias );
	if(NULL==pReserveDM) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ViewScreen") );	
	
	ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pReserveDM , _T("������������") , _T("���������޼���") );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ViewScreen") );

	INT		j , nRowCnt;
	nRowCnt	=	pReserveDM->GetRowCount();
	for( j=0 ; j<nRowCnt ; j++ )
	{
		sOrder.Format( _T("%d") ,j+1 );
		ids = pReserveDM->SetCellData( _T("����") , sOrder , j );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("ViewScreen") );
	}

	if(TRUE==m_pLona->m_bVolReserve) return 0;

	
	CESL_DataMgr *pReserveInfoDM = FindDM(_T("DM_BO_LOC_3170_RESERVE_INFO"));
	if(NULL==pReserveInfoDM) ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("ViewScreen") );

	INT i, nReserveInfoRowCnt;
	nReserveInfoRowCnt = pReserveInfoDM->GetRowCount();
	for( i=0 ; i<nReserveInfoRowCnt ; i++ )
	{
		CString sValue;
		ids = pReserveInfoDM->GetCellData(_T("�����ڹ�ȣ"), i, sValue);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("ViewScreen") );

		CLocCommonAPI::ChangeSecuriyUserNo( this , sValue , m_pLona->m_sIsUnityLoanService );
		ids = pReserveInfoDM->SetCellData(_T("�����ڹ�ȣǥ��"), sValue, i);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("ViewScreen") );
	}

	const INT nCMCnt = 3;
	CString sCMAlias[nCMCnt] = {
		_T("CM_BO_LOC_3170_BOOK_INFO") , _T("CM_BO_LOC_3170_LOAN_USER_INFO") , _T("CM_BO_LOC_3170_RESERVE_INFO")
	};

	INT nViewIdx[nCMCnt] = { 0 , 0 , -1 };
	for( i = 0 ; i < nCMCnt; i++ )
	{
		ids = AllControlDisplay( sCMAlias[i] , nViewIdx[i] );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(  -1100 , _T("ViewScreen") );
	}

	
	CString strUserNo;
	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3170_LOAN_USER_INFO") );
	if(NULL==pCM) ERROR_MSG_RETURN_ERROR_CODE2(  -1110 , _T("ViewScreen") );
						
	ids = pCM->GetControlMgrData( _T("�����ڹ�ȣ"), strUserNo );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1120 , _T("ViewScreen") );

	CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_pLona->m_sIsUnityLoanService );
	ids = pCM->SetControlMgrData( _T("�����ڹ�ȣ"), strUserNo );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1150 , _T("ViewScreen") );

	InitControl();
	
	SetSelect(0);

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3170::OnbtnRESERVATIONCLOSE() 
{
EFS_BEGIN

	OnClose();

EFS_END
}


INT CBO_LOC_3170::SearchRerserveInfo()
{
EFS_BEGIN

	INT ids;

	ids = m_pReservationInfo->SPFExcutePath( _T("���������˻�") );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(  -1010 , _T("SearchRerserveInfo") );
	}

	if( 0 > ids ) 
	{
		return ids;
	}

	return 0;

EFS_END
return -1;
}

BEGIN_EVENTSINK_MAP(CBO_LOC_3170, CDialog)
	//{{AFX_EVENTSINK_MAP(CBO_LOC_3170)
	ON_EVENT(CBO_LOC_3170, IDC_gridRESERVATION, -600 /* Click */, OnClickgridRESERVATION, VTS_NONE)
	ON_EVENT(CBO_LOC_3170, IDC_gridRESERVATION, -601 /* DblClick */, OnDblClickgridRESERVATION, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_3170::OnClickgridRESERVATION() 
{
	EFS_BEGIN

	SetSelect( 2 );

	EFS_END
}


INT CBO_LOC_3170::SetSelect( INT n )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3170_RESERVE_INFO");
	CString sGridAlias = _T("ReserveGrid");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("SetSelect") );
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("SetSelect") );
	
	if( n == 0 ) 	ids = pGrid->SetReverse(0);
	else ids = pGrid->SetReverse(pGrid->GetIdx());

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("SetSelect") );
	return 0;

	EFS_END
	return -1;

}


VOID CBO_LOC_3170::OnbtnDUEDATE() 
{
	EFS_BEGIN

	if( TRUE == m_pLona->m_bVolReserve )
	{
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->SetWindowText( _T("") );	
	}
	else
	{
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170 )->SetWindowText( _T("") );	
	}

	EndowReserveExpireDate();
	

	EFS_END
}


VOID CBO_LOC_3170::OnbtnSENDMAIL() 
{
	EFS_BEGIN

	if ( m_pLona->m_sLoanUserAccessMode.IsEmpty() || m_pLona->m_sLoanUserAccessMode == _T("1") )
	{		
		AfxMessageBox(_T("E-MAIL�߼� �Ұ����� �����Դϴ�.") );
		return;
	}

	if( TRUE == m_pLona->m_bVolReserve )
	{
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->SetWindowText( _T("") );	
	}
	else
	{
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170 )->SetWindowText( _T("") );	
	}	
	
	INT ids;
	ids = SendEmail();
	if( 0 == ids )
	{
		
		
	}
	else if( 1010 == ids )
	{
	}
	else if( 1020 == ids )
	{
	}
	else if( 1030 == ids )
	{
		AfxMessageBox( _T("������ �����ڴ� ���������� �Ǿ� ���� �ʽ��ϴ�.") , MB_ICONINFORMATION );
	}
	else if( 1040 == ids )
	{
		AfxMessageBox( _T("�ݳ��� �ڷᰡ �ƴϹǷ� ������ ������ �� �����ϴ�.") , MB_ICONINFORMATION );
	}
	else if( 1050 == ids )
	{
		
		AfxMessageBox( _T("�̿��ڰ� E-Mail ������ ��û���� �ʾҽ��ϴ�.") , MB_ICONINFORMATION );
	}
	else if( 1060 == ids )
	{
		AfxMessageBox( _T("�̿����� E-Mail�� �Էµ��� �ʾ� E-Mail �߼��� ���� �ʾҽ��ϴ�.") , MB_ICONINFORMATION );
	}
	
	else if( 1070 == ids )
	{
		AfxMessageBox(_T("���ο��� �ڷ��Դϴ�."), MB_ICONINFORMATION);
	}
	
	m_bEdit = TRUE;

	EFS_END
}


VOID CBO_LOC_3170::OnbtnCANCEL() 
{
	EFS_BEGIN

	
	if( TRUE == m_pLona->m_bVolReserve )
	{
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->SetWindowText( _T("") );	
	}
	else
	{
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170 )->SetWindowText( _T("") );	
	}

	INT ids;
	ids = m_pReservationInfo->SPFExcutePath( _T("�������") );
	if( 0 != ids ) return;

	SuccessCancelReservation();

	m_bEdit = TRUE;

	EFS_END
}

INT CBO_LOC_3170::InitControl()
{
	EFS_BEGIN

	CString sReserveDMAlias = _T("DM_BO_LOC_3170_RESERVE_INFO");
	CESL_DataMgr *pDM = FindDM( sReserveDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(  -1010 , _T("InitControl") );
	}

	INT		nRowCnt	=	pDM->GetRowCount();
	if( 1 > nRowCnt )
	{
		EnableDisableButton(false);
	}
	else
	{
		EnableDisableButton(true);
	}
	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3170::EnableDisableButton( bool bFlag )
{
	EFS_BEGIN

	
	if( 10000 == m_pInfo->MODE )
	{
		GetDlgItem( IDC_btnSENDMAIL )->EnableWindow( FALSE );	
		GetDlgItem( IDC_btnSENDSMS )->EnableWindow( FALSE );	
	}
	else
	{
		GetDlgItem(IDC_btnSENDMAIL)->EnableWindow(bFlag);	
		GetDlgItem(IDC_btnSENDSMS)->EnableWindow(bFlag);	
	}
	
	GetDlgItem( IDC_btnDUEDATE )->EnableWindow(bFlag);		
	GetDlgItem( IDC_btnLOAN_CANCEL )->EnableWindow(bFlag);		
	GetDlgItem( IDC_btnPrintTest )->EnableWindow(bFlag);	

	EFS_END
}


INT CBO_LOC_3170::SuccessCancelReservation()
{
EFS_BEGIN

	INT ids , nGridSelIdx;
	CString sReserveDMAlias = _T("");
	CString sCMAlias = _T("");
	CString sGridAlias = _T("");

	if ( TRUE != m_pLona->m_bVolReserve )
	{
		sReserveDMAlias = _T("DM_BO_LOC_3170_RESERVE_INFO");
		sCMAlias = _T("CM_BO_LOC_3170_RESERVE_INFO");
		sGridAlias = _T("ReserveGrid");
	}
	else
	{
		sReserveDMAlias = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
		sCMAlias = _T("CM_BO_LOC_3170_VR_RESERVE_INFO");
		sGridAlias = _T("VRReserveGrid");
	}
	
	CESL_DataMgr *pDM = FindDM( sReserveDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SuccessCancelReservation") );
	}

	CESL_Grid *pGrid = (CESL_Grid*) FindControl( sCMAlias, sGridAlias );
	if( NULL == pGrid )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SuccessCancelReservation") );
	}

	nGridSelIdx = pGrid->GetIdx();

	if( 0 == m_nMenuFlag )
	{		
		CString	strBookKey;
		CString	strMainBookKey;
		CString	strStatus;
		CESL_DataMgr *pMainDM = FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );
		
		if( NULL == pMainDM )
		{
			
			CESL_Mgr* pMgr = pParentMgr->FindSM(_T("SM_BO_LOC_RESERVE"));
			if(NULL != pMgr)
			{
				pMgr->SendMessage(WM_USER+1000, 0, 0);
			}
		}
		else
		{
			CString		strSearchUserKey , strReserveCancelUserKey;
			
			
			ids	= pDM->GetCellData( _T("������KEY") , nGridSelIdx , strReserveCancelUserKey );
			if ( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SuccessCancelReservation") );
			}

			
			ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO") , _T("������KEY") , strSearchUserKey , 0 );
			if ( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("SuccessCancelReservation") );
			}

			
			if( strSearchUserKey == strReserveCancelUserKey )
			{
				CESL_Mgr*	pParent = pParentMgr->FindSM( _T("BO_LOC_3100") );
				if( NULL != pParent )
				{
					pParent->SendMessage( WM_USER + 150 , 0 , 0 );
				}
			}
		}
	}

	ids = pDM->DeleteRow( nGridSelIdx );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("SuccessCancelReservation") ); 
	}

 	ViewScreen();	
	return 0;

EFS_END
return -1;
}



INT CBO_LOC_3170::ViewDescWorkingStatus()
{
	EFS_BEGIN

	INT ids = 0;
	CString sDMAlias = _T("");
	CString sCodeValue = _T("");

	if ( !m_pLona->m_bVolReserve )
		sDMAlias = _T("DM_BO_LOC_3170_BOOK_INFO");
	else
		sDMAlias = _T("DM_BO_LOC_3170_VR_BOOK_INFO");

	ids = GetDataMgrData( sDMAlias, _T("�谡����") , sCodeValue , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("ViewDescWorkingStatus") ); 

	ids = m_pReservationInfo->m_pLoanProc->GetWorkingStatusDescToCode( sCodeValue , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("ViewDescWorkingStatus") ); 

	ids = SetDataMgrData( sDMAlias, _T("�谡����") , sCodeValue , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("ViewDescWorkingStatus") ); 

	
	ids = CLocCommonAPI::MakeShelfCode( this , sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("ViewDescWorkingStatus") ); 

	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_3170::ViewExpireDate()
{
EFS_BEGIN

	INT		nIndex		= 0;
	CString	sDMAlias	= _T("");
	CString	sCMAlias	= _T("");
	CString	sGridAlias	= _T("");

	if ( TRUE != m_pLona->m_bVolReserve )
	{
		sDMAlias	= _T("DM_BO_LOC_3170_RESERVE_INFO");
	}
	else
	{
		sDMAlias	= _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
		sCMAlias	= _T("CM_BO_LOC_3170_VR_RESERVE_INFO");
		sGridAlias	= _T("VRReserveGrid");
	}

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ViewExpireDate") ); 
	}

	
	if ( TRUE == m_pLona->m_bVolReserve )
	{
		CESL_ControlMgr *pCM = FindCM( sCMAlias );
		if( NULL == pCM ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ViewExpireDate") ); 
		}

		CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( sGridAlias );
		if( NULL == pGrid ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ViewExpireDate") ); 
		}

		nIndex = pGrid->GetIdx();
	}

	INT		ids = pDM->SetCellData( _T("���ุ����") , m_pReservationInfo->m_sExpireDate , nIndex );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("ViewExpireDate") ); 
	}

	ViewScreen();

	
	CESL_Mgr* pEslMgr = pParentMgr->FindSM(_T("SM_BO_LOC_RESERVE"));
	if(NULL != pEslMgr)
	{
		CString strLoanKey;
		ids = pDM->GetCellData(_T("����/�ݳ�����KEY"), nIndex, strLoanKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ViewExpireDate"));
		CESL_DataMgr* pReserveMainDM = FindDM(_T("DM_BO_LOC_RESERVE"));
		if(NULL == pReserveMainDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ViewExpireDate"));
		for(INT i = 0; i < pReserveMainDM->GetRowCount(); i++)
		{
			CString strValue;
			pReserveMainDM->GetCellData(_T("LOAN_KEY"), i, strValue);
			if(strValue == strLoanKey)
			{
				pReserveMainDM->SetCellData(_T("���ุ����"), m_pReservationInfo->m_sExpireDate, i);
				CESL_Grid* pReserveGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_RESERVE"), _T("MainGrid"));
				if(NULL == pReserveGrid) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ViewExpireDate"));
				pReserveGrid->DisplayLine(i);
				break;
			}
		}
	}
		
	if( TRUE != m_pLona->m_bVolReserve )
	{
		CESL_Mgr*	pMgr	=	pParentMgr->FindSM( _T("BO_LOC_3100") );
		if( NULL != pMgr )
		{
			CString		strReserveMarkUserKey , strSearchUserKey;

			
			CESL_DataMgr*	pDMMemberInfo		= FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
			if( NULL == pDMMemberInfo ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1160 , _T("ReserveComplete") );
			}
			
			
			ids	=	pDMMemberInfo->GetCellData( _T("������KEY") , 0 , strSearchUserKey );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1170 , _T("ReserveComplete") );
			}

			
			ids	=	pDM->GetCellData( _T("������KEY") , 0 , strReserveMarkUserKey );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1180 , _T("ReserveComplete") );
			}

			if( 0 == strSearchUserKey.Compare(strReserveMarkUserKey) )
			{
				CString		strLoanInfoLoanKey , strReservemarkLoanInfo;
				ids = pDM->GetCellData( _T("����/�ݳ�����KEY") , 0 , strReservemarkLoanInfo );
				if( 0 > ids )
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1190 , _T("ReserveComplete") );
				}

				
				CESL_DataMgr*	pDMLoanInfo		= FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );
				if( NULL == pDMLoanInfo ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1200 , _T("ReserveComplete") );
				}

				INT		i , nRowCnt;
				nRowCnt	=	pDMLoanInfo->GetRowCount();
				for( i=0 ; i<nRowCnt ; i++ )
				{
					ids = pDMLoanInfo->GetCellData( _T("����/�ݳ�����KEY") , i , strLoanInfoLoanKey );
					if( 0 > ids )
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1210 , _T("ReserveComplete") );
					}

					if( 0 == strReservemarkLoanInfo.Compare( strLoanInfoLoanKey ) )
					{
						ids = pDMLoanInfo->SetCellData( _T("���ุ����") , m_pReservationInfo->m_sExpireDate , i );
						if( 0 > ids ) 
						{
							ERROR_MSG_RETURN_ERROR_CODE2( -1230 , _T("ReserveComplete") );
						}

						
						CESL_Grid*		pGridLoanList	= (CESL_Grid*)FindControl( _T("CM_BO_LOC_3100_LOAN_INFO") , _T("MainGrid") );
						if( NULL == pGridLoanList ) 
						{
							ERROR_MSG_RETURN_ERROR_CODE2( -1220 , _T("ReserveComplete") );
						}

						
						pGridLoanList->DisplayLine(i);
					}
				}
			}
		}
	}
	

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3170::OnbtnSENDSMS() 
{
	if ( m_pLona->m_sLoanUserAccessMode.IsEmpty() || m_pLona->m_sLoanUserAccessMode == _T("1") )
	{
		AfxMessageBox(_T("SMS �߼� �Ұ����� �����Դϴ�.") );
		return;
	}

	if( TRUE == m_pLona->m_bVolReserve )
	{
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->SetWindowText( _T("") );	
	}
	else
	{
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170 )->SetWindowText( _T("") );	
	}
	
	SMS_CONNECTION_INFO SMS_ConnectionInfo;
	SMS_MSG_INFO SMS_MsgInfo;	

	m_SMSType = _T("SMS03");

	CSMS_MakeMsg MakeSMS( this );
	m_pSMSContentsEditor->GetSMSInfo( m_SMSType, SMS_MsgInfo , SMS_ConnectionInfo, 1 );

	CSMS_Env_Info SMSDMSender( this );

	SMSDMSender.InitSMSFieldData();

	CString sUserDMAlias , sBookDMAlias;
	INT		nSelectIndex		=	0;		

	CESL_DataMgr *pReserveUserDM;	
	CESL_DataMgr *pReserveBookDM;	
	CString name, phoneNum, bookTitle, msg, date, bookCnt, successYN, user_key, loan_key , strSMSReceiptYN;	
	CStringArray asPositionCode,asClassCode,asInferiorClass,asUserID,asBirthDay,asCivilNo;
	INT		ids = 0;
	
	
	if( !m_pLona->m_bVolReserve )
	{
		
		pReserveUserDM = FindDM( _T("DM_BO_LOC_3170_RESERVE_INFO") );
		if( NULL == pReserveUserDM ) 
		{
			return;
		}
		
		
		pReserveBookDM = FindDM( _T("DM_BO_LOC_3170_BOOK_INFO") );
		if( NULL == pReserveBookDM ) 
		{
			return;
		}
		
		CString strCellPhone	=	_T("");
		
		ids		=	GetDataMgrData( _T("DM_BO_LOC_3170_RESERVE_INFO") , _T("�ڵ���") , strCellPhone , nSelectIndex );
		if( ids < 0 ) 
		{
			return;
		}

		if( strCellPhone.IsEmpty() )
		{
			AfxMessageBox( _T("�̿����� �ڵ��� ��ȣ�� �Էµ��� �ʾ� SMS �߼��� ���� �ʾҽ��ϴ�.") , MB_ICONINFORMATION );
			return;
		}
		
		CString		strForm , strSpeciesKey , strRegNo , strQuery , strWorkingStatus;
		ids = pReserveUserDM->GetCellData( _T("����/���ӱ���") , nSelectIndex , strForm );
		if( 0 > ids )
		{
			return;
		}

		ids = pReserveUserDM->GetCellData( _T("��KEY") , nSelectIndex , strSpeciesKey );
		if( 0 > ids )
		{
			return;
		}

		ids = pReserveUserDM->GetCellData( _T("��Ϲ�ȣ") , nSelectIndex , strRegNo );
		if( 0 > ids )
		{
			return;
		}

		if(_T("MO") == strForm)
		{
			strQuery.Format(_T("SELECT WORKING_STATUS FROM BO_BOOK_TBL WHERE SPECIES_KEY='%s' AND REG_NO='%s'")
				,strSpeciesKey, strRegNo);
		}
		else if(_T("SE") == strForm)
		{
			strQuery.Format(_T("SELECT WORKING_STATUS FROM SE_BOOK_TBL WHERE SPECIES_KEY='%s' AND REG_NO='%s'")
				,strSpeciesKey, strRegNo);
		}
		
		ids = pReserveUserDM->GetOneValueQuery( strQuery , strWorkingStatus );
		if( 0 > ids )
		{
			return;
		}

		if(		0 != strWorkingStatus.Compare(_T("��ġ�ڷ�"))
			&&	0 != strWorkingStatus.Compare(_T("�ǽð��谡�ڷ�"))
			&&	0 != strWorkingStatus.Compare(_T("���Ӻ�ġ�ڷ�"))
			&&	0 != strWorkingStatus.Compare(_T("BOL112N"))
			&&	0 != strWorkingStatus.Compare(_T("SEL212N"))
			&&	0 != strWorkingStatus.Compare(_T("SEL112N")) )
		{
			AfxMessageBox( _T("�ݳ��� �ڷᰡ �ƴϹǷ� SMS�� ������ �� �����ϴ�.") , MB_ICONINFORMATION );
			return;
		}
		

		bookTitle	=	pReserveBookDM->GetCellData( _T("��ǥ��"),		nSelectIndex );
		name		=	pReserveUserDM->GetCellData( _T("������"),		nSelectIndex );
		phoneNum	=	pReserveUserDM->GetCellData( _T("�ڵ���"),		nSelectIndex );
		date		=	pReserveUserDM->GetCellData( _T("���ุ����"),	nSelectIndex );
		user_key	=	pReserveUserDM->GetCellData( _T("������KEY"),	nSelectIndex );
		loan_key	=	pReserveUserDM->GetCellData( _T("����/�ݳ�����KEY"),	nSelectIndex );

		strSMSReceiptYN	=	pReserveUserDM->GetCellData( _T("SMS���ſ���"),	nSelectIndex );
	}
	else
	{
		
		
		pReserveUserDM = FindDM( _T("DM_BO_LOC_3170_VR_RESERVE_INFO") );
		if( NULL == pReserveUserDM ) 
		{
			return;
		}
		
		pReserveBookDM = FindDM( _T("DM_BO_LOC_3170_VR_BOOK_INFO") );
		if( NULL == pReserveBookDM ) 
		{
			return;
		}
		
		CESL_Grid*	pGrid	=	(CESL_Grid*)FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRReserveGrid") );
		if( NULL == pGrid )
		{
			return;
		}

		nSelectIndex	=	pGrid->GetIdx();

		
		CString strUnmanned;
		ids = pReserveUserDM->GetCellData(_T("���ο������"), nSelectIndex, strUnmanned);
		if(0 > ids){return;}
		if(!strUnmanned.IsEmpty())
		{
			AfxMessageBox(_T("���ο��� �ڷ��Դϴ�."), MB_ICONINFORMATION);
			return;
		}
		
		CString sReserveMark	=	_T("");
		
		ids = GetDataMgrData( _T("DM_BO_LOC_3170_VR_RESERVE_INFO") , _T("��������") , sReserveMark , nSelectIndex );
		if( 0 > ids ) 
		{
			return;
		}

		if( _T("O") != sReserveMark )
		{
			AfxMessageBox(_T("������ �����ڴ� ���������� �Ǿ� ���� �ʽ��ϴ�.") , MB_ICONINFORMATION );
			return;
		}

		CString strCellPhone	=	_T("");
		
		ids		=	GetDataMgrData( _T("DM_BO_LOC_3170_VR_RESERVE_INFO") , _T("�ڵ���") , strCellPhone , nSelectIndex );
		if( 0 > ids )
		{
			return;
		}

		if( strCellPhone.IsEmpty() )
		{
			AfxMessageBox( _T("�̿����� �ڵ��� ��ȣ�� �Էµ��� �ʾ� SMS �߼��� ���� �ʾҽ��ϴ�.") , MB_ICONINFORMATION );
			return;
		}
		
		bookTitle	=	pReserveBookDM->GetCellData( _T("����"),		nSelectIndex );
		name		=	pReserveUserDM->GetCellData( _T("������"),		nSelectIndex );
		phoneNum	=	pReserveUserDM->GetCellData( _T("�ڵ���"),		nSelectIndex );
		date		=	pReserveUserDM->GetCellData( _T("���ุ����"),	nSelectIndex );
		user_key	=	pReserveUserDM->GetCellData( _T("������KEY"),	nSelectIndex );
		loan_key	=	pReserveUserDM->GetCellData( _T("����/�ݳ�����KEY"),	nSelectIndex );	
		
		strSMSReceiptYN	=	pReserveUserDM->GetCellData( _T("SMS���ſ���"),	nSelectIndex );
		
	}

	if( _T("Y") != strSMSReceiptYN )
 	{
 		AfxMessageBox( _T("�̿��ڰ� SMS Service ������ ��û���� �ʾҽ��ϴ�.") , MB_ICONINFORMATION );
 		return;
 	}

	CStringList slUserKey, slLoanKey, slName, slPhoneNum, slContent , slSMSUseYN;
	MakeSMS.MakeSMSMsg( m_SMSType, &SMS_MsgInfo, name, date, bookTitle, 1, msg );
	slUserKey.AddTail(user_key);
	slLoanKey.AddTail(loan_key);
	slName.AddTail(name);
	phoneNum.Replace( _T("-"), _T("") );
	slPhoneNum.AddTail(phoneNum);
	slContent.AddTail(msg);
	slSMSUseYN.AddTail(strSMSReceiptYN);

	CSMSSendDlg dlg( this, SMS_ConnectionInfo, m_SMSType, SMS_MsgInfo.sCallBackNumber, 
					 slUserKey, slLoanKey, slName, slPhoneNum, slContent, slSMSUseYN , 1 );
	dlg.SetInfoEx( asPositionCode, asClassCode, asInferiorClass, asUserID, asBirthDay, asCivilNo );
	dlg.DoModal();

}

VOID CBO_LOC_3170::OnbtnPrintTest() 
{
	
	if( TRUE == m_pLona->m_bVolReserve )
	{
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->SetWindowText( _T("") );	
	}
	else
	{
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170 )->SetWindowText( _T("") );	
	}

	if ( m_ReceiptDlg->GetSafeHwnd() == NULL )
	{
		m_ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG( 3, this , m_pLona );
		m_ReceiptDlg->pMain = this->pMain;
		m_ReceiptDlg->pParentMgr = (CESL_Mgr*)this;
		m_ReceiptDlg->m_pInfo = this->m_pInfo;				
		if (m_ReceiptDlg->Create(this))
		{
			m_ReceiptDlg->CenterWindow();
			m_ReceiptDlg->UpdateWindow();				
		}
		else
		{
			delete m_ReceiptDlg;
		}
	}
	
	
	if( m_pLona->m_bVolReserve )
	{
		
		
		CESL_DataMgr*  pLoanInfo = FindDM( _T("DM_BO_LOC_3170_VR_RESERVE_INFO") );
		if( !pLoanInfo )	return;

		CESL_DataMgr* pBookInfo = FindDM( _T("DM_BO_LOC_3170_VR_BOOK_INFO"));
		if( !pBookInfo )	return;
		
		CESL_Grid*	pVRReserveGrid	=	(CESL_Grid*)FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRReserveGrid") );
		if( pVRReserveGrid == NULL ) return;

		INT	nSelectIndex	=	pVRReserveGrid->GetIdx();
		
		CString sReserveMark   = _T("");
		pLoanInfo->GetCellData( _T("��������")	, nSelectIndex , sReserveMark );

		if( _T("O")	!=	sReserveMark )
		{
			AfxMessageBox(_T("������ �����ڴ� ���������� �Ǿ� ���� �ʽ��ϴ�.") , MB_ICONINFORMATION );
			return;
		}
	
	}
	

	m_ReceiptDlg->PrintReservationInfo();
}


BOOL CBO_LOC_3170::PreTranslateMessage(MSG* pMsg) 
{

	return CESL_Mgr::PreTranslateMessage(pMsg);
}

HBRUSH CBO_LOC_3170::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

VOID CBO_LOC_3170::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( TRUE == bShow )
	{

		if( 0 == m_nMenuFlag )
		{
			CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
			CString sGridAlias = _T("MainGrid");

			CESL_ControlMgr *pCM = pParentMgr->FindCM( sCMAlias );
			if( NULL == pCM ) 
			{
				return;
			}

			CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( sGridAlias );
			if( NULL == pGrid ) 
			{
				return;
			}

			
			INT nRowIndex = pGrid->SelectGetIdx();
			if( 0 > nRowIndex ) 
			{
				return;
			}

			CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
			TCHAR *sFieldAlias[5] = 
			{
				_T("����/���ӱ���"), _T("åKEY"), _T("�����̿��ڼ�"), _T("��KEY"), _T("��ȣ����KEY")
			};

			CString sGetData[5];
			INT		i;
			for( i=0 ; i<5 ; i++ )
			{
				INT ids = pParentMgr->GetDataMgrData( sDMAlias , sFieldAlias[i] , sGetData[i] , nRowIndex );
				if( 0 > ids ) 
				{
					return;
				}
			}

			SetInfo( sGetData[3], sGetData[4], sGetData[0], sGetData[1] );	
			m_pReservationInfo->SetInfo( m_sSpeciesKey, m_sVolInfoKey, m_sPublishFormCode, m_sBookKey, m_nMenuFlag );
		}
		

		INT ids = SearchRerserveInfo();
		if( 0 < ids )
		{
			m_bEdit = FALSE;
			OnCancel();
		}
		if( 0 > ids ) 
		{
			return;
		}

		ids = ViewScreen();
		if( 0 > ids )
		{
			return;
		}
	}
}

void CBO_LOC_3170::OnTimer(UINT nIDEvent) 
{
	
	CDialog::OnTimer(nIDEvent);
}

INT CBO_LOC_3170::SendEmail()
{
EFS_BEGIN
	
	INT ids;
	m_pReservationInfo->SetMailClass( m_nMailClass );
	ids = m_pReservationInfo->SPFExcutePath( _T("EMAIL������") );
	
	return ids;
EFS_END	
return -1;
}


VOID CBO_LOC_3170::EndowReserveExpireDate()
{
EFS_BEGIN

	
	if( TRUE == m_pLona->m_bVolReserve )
	{
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->SetWindowText( _T("") );	
	}
	else
	{
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170 )->SetWindowText( _T("") );	
	}
	
	CString sDMAlias = _T("");
	CString sPositionCode = _T("");
	
	if ( TRUE != m_pLona->m_bVolReserve )
	{
		sDMAlias = _T("DM_BO_LOC_3170_RESERVE_INFO");
	}
	else
	{
		sDMAlias = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
	}

	CESL_DataMgr *pReserveDM = FindDM( sDMAlias );
	if( NULL == pReserveDM ) 
	{
		ERROR_MSG_RETURN_VOID2(  -1010 , _T("CBL_LOC_3170::OnbtnDUEDATE()") );
	}

	
	INT ids;
	CString strUnmanned;
	ids = pReserveDM->GetCellData(_T("���ο������"), 0, strUnmanned);
	if(0 > ids)ERROR_MSG_RETURN_VOID2(-1000, _T("EndowReserveExpireDate"));
	if(_T("O") == strUnmanned)
	{
 		AfxMessageBox(_T("���ο����� �ڷ��Դϴ�."), MB_ICONINFORMATION);
 		return;
	}


	ids = pReserveDM->GetCellData( _T("����������") , 0 , sPositionCode );
	if( 0 > ids ) 
	{
		return;
	}

	m_pReservationInfo->SetPositionCode( sPositionCode );

	ids = m_pReservationInfo->SPFExcutePath( _T("���ุ���Ϻο�") );
	if( 0 > ids ) 
	{
		return;
	}
	BOOL bUnmanned = FALSE;
	if( TRUE != m_pLona->m_bVolReserve )
	{

		CString strFaileMsg, strTmp;
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170 )->GetWindowText( strFaileMsg );
		if( 0 == ids )
		{
			strTmp = _T("");
		}
		else if( 1010 == ids )
		{
			strTmp = _T("SMS �޽����� �����Ͽ����ϴ�.");
		}
		else if( 1030 == ids )
		{
			strTmp = _T("�̿����� �ڵ��� ��ȣ�� �Էµ��� �ʾ� SMS ������ ���� �ʾҽ��ϴ�.");
		}
		
		else if( 1040 == ids )
		{
			strTmp = _T("Procedure���� SMS ������ �ߴܵǾ����ϴ�.");
		}
		else if( 0 < ids ) 
		{
			return;
		}
		else
		{
		}

		if( TRUE != strFaileMsg.IsEmpty() )
		{
			strFaileMsg += _T("     ");
		}

		strFaileMsg += strTmp;
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170 )->SetWindowText( strFaileMsg );

		AfxMessageBox( _T("���ุ������ �ο��Ǿ����ϴ�.") );
		
	}
	else
	{
		
		CString strFaileMsg, strTmp;
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->GetWindowText( strFaileMsg );
		if( 0 == ids )
		{
			strTmp = _T("");
		}
		else if( 1010 == ids )
		{
			strTmp = _T("SMS �޽����� �����Ͽ����ϴ�.");
		}
		else if( 1030 == ids )
		{
			strTmp = _T("�̿����� �ڵ��� ��ȣ�� �Էµ��� �ʾ� SMS ������ ���� �ʾҽ��ϴ�.");
		}
		else if( 1040 == ids )
		{
			strTmp = _T("Procedure���� SMS ������ �ߴܵǾ����ϴ�.");
		}
		
		else if( 1050 == ids )
		{
			bUnmanned = TRUE;
			strTmp = _T("���ο��� ��û �ڷ��Դϴ�.");
		}
		else if( 0 < ids ) 
		{
			return;
		}
		else
		{
		}

		if( TRUE != strFaileMsg.IsEmpty() )
		{
			strFaileMsg.Format( _T("     ") );
		}

		strFaileMsg += strTmp;
		GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->SetWindowText( strFaileMsg );
		


		CESL_ControlMgr *pCM = pParentMgr->FindCM( _T("CM_BO_LOC_3100_LOAN_INFO") );
		if( NULL == pCM ) 
		{
			return;
		}

		CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( _T("MainGrid") );
		if( NULL == pGrid ) 
		{
			return;
		}
		
		INT nRowIndex = pGrid->GetIdx();
		if( 0 > nRowIndex ) 
		{
			return;
		}
		
		TCHAR *sFieldAlias[5] = 
		{
			_T("����/���ӱ���"), _T("åKEY"), _T("�����̿��ڼ�"), _T("��KEY"), _T("��ȣ����KEY")
		};

		CString sGetData[5];
		INT		i;
		for( i=0 ; i<5 ; i++ )
		{
			INT ids = pParentMgr->GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO") , sFieldAlias[i] , sGetData[i] , nRowIndex );
			if( 0 > ids ) 
			{
				return;
			}
		}
		SetInfo(sGetData[3], sGetData[4], sGetData[0], _T("") );	
	}

	
	ViewExpireDate();
	
	if(TRUE == bUnmanned)
	{
		AfxMessageBox( _T("���ο����û �ڷῡ ���Ͽ� ���ุ������ �ο��Ͽ����ϴ�.") );
	}

	m_bEdit = TRUE;

EFS_END	
	return;
}

void CBO_LOC_3170::OnDblClickgridRESERVATION() 
{
	
	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3170_RESERVE_INFO"), _T("ReserveGrid"));
	if(NULL == pGrid){return;}
	if(1 > pGrid->GetMouseRow())
	{
		return;
	}

	INT nIdx = pGrid->GetIdx();
	if(0 > nIdx)
	{
		return;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3170_RESERVE_INFO"));
	m_strSearchUserKey = pDM->GetCellData(_T("������KEY"), nIdx);	


	CESL_Mgr* pMgr = pParentMgr->FindSM(_T("BO_LOC_3100"));
	if(pMgr)
	{
		pMgr->SendMessage(WM_SEARCH_USER_FROM_KEY, 1, (WPARAM)&m_strSearchUserKey);
	}
}
