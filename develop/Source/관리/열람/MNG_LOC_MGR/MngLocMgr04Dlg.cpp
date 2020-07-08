// MngLocMgr04Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr04Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr04Dlg dialog


CMngLocMgr04Dlg::CMngLocMgr04Dlg(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr04Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sDMAlias = _T("DM_MNG_LOC_MGR_04_DLG");
	m_sCMAlias = _T("CM_MNG_LOC_MGR_04_DLG");
	m_sGridAlias = _T("�������޺�ȯ�漳���׸���");
	m_sComboAlias = _T("�̿��󱸺�");

	m_pGridEdt = NULL;

	m_bFirstReturn = TRUE;

	m_pLoanShareManager = pLoanShareManager;

	m_bIsCopied = FALSE;

	// 2005.05.26
	m_strUserID=pParent->GetUserID();
	m_strLibCode=_T("");
}
CMngLocMgr04Dlg::~CMngLocMgr04Dlg()
{
	
}

VOID CMngLocMgr04Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr04Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngLocMgr04Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr04Dlg)
	ON_WM_SIZE()
	ON_CBN_EDITCHANGE(IDC_cmbUSE_OBJ_CODE, OnEditchangecmbUSEOBJCODE)
	ON_CBN_SELCHANGE(IDC_cmbUSE_OBJ_CODE, OnSelchangecmbUSEOBJCODE)
	ON_MESSAGE(UPDATE_GRID_EDT, OnUpdateGridCelText)
	ON_COMMAND_RANGE(4000,4001,CopyAndPasteProc)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr04Dlg message handlers

BOOL CMngLocMgr04Dlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMngLocMgr04Dlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if( GetSafeHwnd() == NULL ) return;
	
	// 2006-03-15 UPDATE BY PDW(JR) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �޼��� ����ƽ�ڽ� �߰�	
	const INT nCnt = 5;
	CWnd* pWnd[nCnt];
	CRect WndPos[nCnt];
	INT nIDC[nCnt] = 
	{
		IDC_staTITLE1 , IDC_gridPOSITION , IDC_staTITLE2 , IDC_staTITLE3 , IDC_gridPOSITION2
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		pWnd[i] = GetDlgItem( nIDC[i] );
		if( pWnd[i]->GetSafeHwnd() == NULL ) return;
		pWnd[i]->GetWindowRect(WndPos[i]);
		ScreenToClient(WndPos[i]);
	}
	
	pWnd[0]->MoveWindow( 10 , 3 , cx-20 , WndPos[0].Height() );
	pWnd[1]->MoveWindow( 10 , WndPos[0].Height()+3 , cx-20 , WndPos[2].top-WndPos[0].Height()-3-5 );
	pWnd[2]->MoveWindow( 10 , WndPos[2].top , cx-20 , WndPos[2].Height() );
	pWnd[3]->MoveWindow( 10 , WndPos[3].top , cx-20 , WndPos[3].Height() );
	pWnd[4]->MoveWindow( 10 , WndPos[3].bottom + 1 , cx-20 , cy - WndPos[3].bottom - 30  );
	// --------------------------------------------------------------------------------------
}


INT CMngLocMgr04Dlg::ViewScreen()
{
EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_MNG_LOC_MGR_04_DLG");
	CString sCMAlias = _T("CM_MNG_LOC_MGR_04_DLG");
	CString sCMAlias2 = _T("CM_MNG_LOC_MGR_04_2_DLG");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("ViewScreen") );

//	ids = pDM->RefreshDataManager(_T(""));
	ids = CLocCommonAPI::RefreshDataManager( this , pDM , _T("") );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2 , _T("ViewScreen") ); 

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(-3 , _T("ViewScreen") ); 
	pCM->AllControlDisplay();
	
	CESL_ControlMgr *pCM2 = FindCM( sCMAlias2 );
	if( pCM2 == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(-3 , _T("ViewScreen") ); 
	pCM2->AllControlDisplay();

	// ���� ���޺� ��ϱ��к� ȯ�漳��
	ClassRegCodeView();

	return 0;

EFS_END
return -1;
}

INT CMngLocMgr04Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	
	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_gridPOSITION);

	CString sValue;
	CString sRecKey;

	//�̵���������
	CString strTL_SetData;
	CString strTL_Condition;
	CString strTempData;

	// 2009.02.25 ADD BY PWR : �̵����Ⱑ��å�� �߰�
	const INT nCnt = 12;

	TCHAR *sDBColumnName[nCnt] =
	{
		_T("USEOBJCODE") ,
		_T("LOAN_PERIOD") , 
		_T("POSTPONEMENT_PERIOD") ,
		_T("UNITY_LOAN_BOOK_CNT") ,
		_T("�ڰ�����å��") , 
		_T("LOAN_BOOK_CNT") , 
		_T("OTHER_LOAN_BOOK_CNT") , 
		_T("TL_LOAN_BOOK_CNT") , 
		_T("RESERVATION_EXPIRE_DAY") , 
		_T("PREENGAGE_BOOK_CNT") ,
		_T("PREENGAGE_CANCEL_LIMIT_PERIOD") ,
		_T("PREENGAGE_CANCEL_LIMIT_COUNT")
	};

	TCHAR *sColumnType[nCnt] =
	{
		_T("STRING") ,
		_T("NUMERIC") , 
		_T("NUMERIC") , 
		_T("NUMERIC") , 
		_T("NUMERIC") , 
		_T("NUMERIC") , 
		_T("NUMERIC") , 
		_T("NUMERIC") , 
		_T("NUMERIC") ,
		_T("NUMERIC") ,
		_T("NUMERIC") , 
		_T("NUMERIC")
	};

	TCHAR *sAliasName[nCnt] =
	{
		_T("�̿��󱸺�") ,
		_T("�������") , 
		_T("���⿬��Ⱓ") , 
		_T("���մ��Ⱑ��å��") , 
		_T("�ڰ����Ⱑ��å��") , 
		_T("�������Ⱑ��å��") , 
		_T("Ÿ�����Ⱑ��å��") , 
		_T("�̵����Ⱑ��å��") , 
		_T("���ุ���ϼ�") ,
		_T("���డ��å��") ,
		_T("���������������Ⱓ") , 
		_T("����������Ѱ���")
	};

	// 2005.05.26 ADD BY PDJ
	if ( m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE�� ����
// 		CString	strWhere;
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE ")
// 			            _T("MANAGE_CODE IN ( SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
// 						_T("WHERE ID = '%s')"), m_strUserID );
// 		m_pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
		m_strLibCode = m_pInfo->LIB_CODE;
	}

	// ���޺� �ݳ����� ���� ����� ���� �������� �˸��� 
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	// Mobile API ����
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , m_pDM );
	pMobileApi->StartFrame();

	
	CString strUseObjectCode;				
	CString strLoanPeriod;
	CString strPostponementPeriod;
	CString strLoanBookCnt;
	CString strPreengageBookCnt; // 5	
	CString strReservationExpireDay;	
	CString strPreenageCancelLimitCount;
	CString strPreenageCancelLimitPeriod;
	CString strOtherLoanBookCnt; // 10
	CString strUnityLoanBookCnt;
	CString strTLLoanBookCnt;
	CString strValue;
	CString strCode;

	for( INT i = 0 ; i < m_pDM->GetRowCount() ; i++ )
	{
		pMobileApi->InitDBFieldData();

		strTL_SetData = _T("");
		m_pDM->GetCellData( _T("REC_KEY") , i , sRecKey );

		BOOL bEdit = FALSE;
		for( INT j = 1 ; j < nCnt+1 ; j++ )
		{
			if(j==5) continue; //�ڰ�����å���� ���̺��� ���� �ֱ⶧���� pass~
			//�̵��������� ���
			if( m_pInfo->MODE == 10000 )
			{
				if((j!=2)&&(j!=3)&&(j!=8)) continue;
			}
			sValue.Empty();
			m_pGrid->GetAt( i , j , sValue );
			sValue.TrimRight(); sValue.TrimLeft();

			if( (CString(_T("NUMERIC")).Compare(sColumnType[j-1])==0) && sValue.IsEmpty() )
			{
				sValue = _T("0");
			}

			// 2009.07.01 ADD BY PWR : �����Ѱ͸� UPDATE�ϵ����Ѵ�.
			CString sValue2;
			m_pDM->GetCellData(sAliasName[j-1], i, sValue2);
			sValue2.TrimRight(); sValue2.TrimLeft();
			if(sValue2.IsEmpty())
			{
				sValue2 = _T("0");
			}
			if(sValue.IsEmpty())
			{
				sValue = _T("0");
			}
			if(sValue == sValue2)
			{
				continue;
			}
			bEdit = TRUE;			

			ids = pMobileApi->AddDBFieldData( sDBColumnName[j-1] , sColumnType[j-1] , sValue );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );
			
			//�̵��������� ���
			if( m_pInfo->MODE == 10000 )
			{
				if(!strTL_SetData.IsEmpty())
				{
					strTL_SetData+=_T(",");
				}
				if(!sValue.IsEmpty())
				{
					strTempData.Format(_T("%s=%s"),sDBColumnName[j-1],sValue);
					strTL_SetData +=strTempData;
				}
			}
		}
		if(bEdit == TRUE)
		{
			ids = pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("����ȯ�漳��"), __WFILE__, __LINE__ ), TRUE );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );
			ids = pMobileApi->MakeUpdateFrame( _T("CD_CODE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );

			// ���� �ݳ����� ���� ����� ���� �������� �˸��� 	
			UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
			UploadShareInfo.UploadShareLoanInfo.sCommand = _T("61");
			UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CD_CODE_TBL");
			UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("");
			UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
			UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
			m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		}
		//JOB.2019.0073
		/*
		//�̵��������� ���
		if( m_pInfo->MODE == 10000 )
		{
			if(ids>=0)
			{
				if(!strTL_SetData.IsEmpty())
				{
					strTL_Condition.Format(_T("REC_KEY=%s"),sRecKey);
					m_syncApi.RefreshSyncData(_T("CD_CODE_TBL"),strTL_SetData,strTL_Condition);
				}
			}
		}
		*/
		//=============================================
		//2009.02.20 REM BY LKS : �̵��������� ��������
		//CLocCommonAPI::MobileApiEndLog( pMobileApi );
		//=============================================
	}

	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveScreen") );
	pMobileApi->EndFrame();

	strTL_SetData.Empty();
	strTL_Condition.Empty();

	//++2008.12.09 ADD BY PWR {{++
	// �ڰ�����å�� UPDATE : ���������� ���� ���̺��� �����ϹǷ� 
	INT nTmp = m_pDM->GetRowCount();
	CString strTmp;
	// Mobile API ����
	//CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this, m_pDM );
	pMobileApi->StartFrame();
	for( i = 0 ; i < m_pDM->GetRowCount() ; i++ )
	{
		strTmp.Format(_T("%d"),i);
		if(i==27)
		{
			int a=0;
		}
		TRACE(strTmp);
		m_pDM->GetCellData( _T("�ڰ�KEY"), i, sRecKey );
		if(sRecKey.IsEmpty()) continue;
		
		pMobileApi->InitDBFieldData();
		
		sValue.Empty();
		m_pGrid->GetAt( i , 5, sValue );
		sValue.TrimRight(); sValue.TrimLeft();
		if( sValue.IsEmpty() ) sValue = _T("0");

		// 2009.07.01 ADD BY PWR : �����Ѱ͸� UPDATE�ϵ����Ѵ�.
		CString sValue2;
		m_pDM->GetCellData(_T("�ڰ����Ⱑ��å��"), i, sValue2);
		sValue2.TrimRight(); sValue2.TrimLeft();
		if( sValue2.IsEmpty() ) sValue2 = _T("0");

		if(sValue == sValue2)
		{
			continue;
		}

		ids = pMobileApi->AddDBFieldData( _T("LOAN_BOOK_CNT") , _T("NUMERIC") , sValue );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );
		ids = pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("����ȯ�漳��"), __WFILE__, __LINE__ ), TRUE );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );
		ids = pMobileApi->MakeUpdateFrame( _T("CD_INDIVIDUAL_CLASS_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );

		//JOB.2019.0073
		/*
		//�̵��������� ���
		if( m_pInfo->MODE == 10000 )
		{
			if(ids>=0)
			{
				if(sValue.IsEmpty()) sValue = _T("0");
				strTL_SetData.Format(_T("LOAN_BOOK_CNT=%s"),sValue);
				strTL_Condition.Format(_T("REC_KEY=%s AND MANAGE_CODE='UDF_MANAGE_CODE'"),sRecKey);
				m_syncApi.RefreshSyncData(_T("CD_INDIVIDUAL_CLASS_TBL"),strTL_SetData,strTL_Condition);
			}
		}
		*/
		// Mobile API delete
		//=============================================
		//2009.02.20 REM BY LKS : �̵��������� ��������
		//CLocCommonAPI::MobileApiEndLog( pMobileApi );
		//=============================================
	}
	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveScreen") );
	m_pDM->EndFrame();

	// ��ϱ��к� ���޺� ȯ�� ����
	ids = SaveRegClassValue();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SaveScreen") );

	LockWindowUpdate();
	ViewScreen();
	UnlockWindowUpdate();

	return 0;

EFS_END
return -1;
}

BOOL CMngLocMgr04Dlg::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	INT ids;
	ids = InitESL_Mgr(_T("SM_MNG_LOC_MGR_04_DLG"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1000, _T("OnInitDialog") );

	m_pDM = FindDM( m_sDMAlias );
	if( m_pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(-1010,_T("OnInitDialog"));

	m_pCM = FindCM( m_sCMAlias );
	if( m_pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(-1020,_T("OnInitDialog"));

	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_gridPOSITION);
	// 2009.02.25 UPDATE BY PWR : �̵����Ⱑ��å�� �߰�( �̵��� �̵��� �����ְ��Ѵ�. )
	if( m_pInfo->MODE == 10000 )
	{
		// �ٸ����Ⱑ��å���� HIDE
		m_pGrid->SetColWidth(5, 0, 0);
		m_pGrid->SetColWidth(6, 0, 0);
		m_pGrid->SetColWidth(7, 0, 0);
		m_pGrid->SetColWidth(8, 0, 0);
	}
	else
	{
		// 2008.12.09 ADD BY PWR : ���մ���Ǽ���뿩�ο� ���� ó��
		CString sValue;
		if( GetManageValue( _T("����"), _T("����"), _T("���մ�����"), _T("���մ���Ǽ���뿩��"), sValue ) < 0 )
			ERROR_MSG_RETURN_ERROR_CODE2(-1030,_T("OnInitDialog"));
		sValue.TrimLeft( ); sValue.TrimRight( );
		if ( sValue.IsEmpty() ) sValue = _T("N");
		if( sValue == _T("Y") )
		{ // �����
			// ���մ��Ⱑ��å��, �ڰ����Ⱑ��å��(��������) SHOW
			// �ڰ����Ⱑ��å��, Ÿ�����Ⱑ��å�� HIDE
			m_pGrid->SetColWidth(7, 0, 0);
			m_pGrid->SetColWidth(8, 0, 0);
		}
		else
		{ // ������
			// �ڰ����Ⱑ��å��, Ÿ�����Ⱑ��å�� SHOW
			// ���մ��Ⱑ��å��, �ڰ����Ⱑ��å��(��������) HIDE
			m_pGrid->SetColWidth(5, 0, 0);
			m_pGrid->SetColWidth(6, 0, 0);
		}
		//�̵����Ⱑ��å�� HIDE
		m_pGrid->SetColWidth(9, 0, 0);
	}
	
	//JOB.2019.0073
	//// �̵������� ������ ����ȭ Class DB����
	//m_syncApi.SetSyncDBInfo_TL(m_pInfo->CONNECTION_INFO,1);

	// 18.09.27 JMJ KOL.RED.2018.008
	// å�η����� ������ �߰�
	CString strValue;
	ids = GetManageValue(_T("����"), _T("����"), _T("å�η��ý���"), _T("å�η���뿩��"), strValue);
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("OnInitDialog"));
	}
	strValue.TrimLeft(); strValue.TrimRight();
	if(_T("Y") == strValue) 
	{
		ids = GetManageValue(_T("����"), _T("����"), _T("å�η��ý���"), _T("å�η�����"), strValue);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("OnInitDialog"));
		}
		strValue.TrimLeft(); strValue.TrimRight();
		if(strValue.IsEmpty())
		{
			m_sBookCooperativeMode = _T("I");
		}
		else
		{
			m_sBookCooperativeMode = strValue;
		}
	}
	else
	{
		m_sBookCooperativeMode = _T("I");
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

BEGIN_EVENTSINK_MAP(CMngLocMgr04Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr04Dlg)
	ON_EVENT(CMngLocMgr04Dlg, IDC_gridPOSITION, -600 /* Click */, OnClickgridPOSITION, VTS_NONE)
	ON_EVENT(CMngLocMgr04Dlg, IDC_gridPOSITION, -601 /* DblClick */, OnDblClickgridPOSITION, VTS_NONE)
	ON_EVENT(CMngLocMgr04Dlg, IDC_gridPOSITION, -604 /* KeyUp */, OnKeyUpgridPOSITION, VTS_PI2 VTS_I2)
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	ON_EVENT(CMngLocMgr04Dlg, IDC_gridPOSITION2, -600 /* Click */, OnClickgridCLASSREG, VTS_NONE)
	ON_EVENT(CMngLocMgr04Dlg, IDC_gridPOSITION2, -601 /* DblClick */, OnDblClickgridCLASSREG, VTS_NONE)	
	ON_EVENT(CMngLocMgr04Dlg, IDC_gridPOSITION2, -604 /* KeyUp */, OnKeyUpgridCLASSREG, VTS_PI2 VTS_I2)
	// -----------------------------------------------------------------------------------------------
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMngLocMgr04Dlg::OnClickgridPOSITION() 
{
EFS_BEGIN

//	m_pGrid->SetReverse( m_pGrid->GetIdx() );
//	FreeGridEdt();
	m_bFirstReturn = FALSE;
	// 2006-03-14 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++
	// �׸��� �����Ͱ� ���������� ����ȴ�.
	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_gridPOSITION);
	// ---------------------------------------------------------------

EFS_END
}

// #########################################################################################
// [ �Լ����� ]
//	 ���Ѽ��� �׸����� ���� �÷��� ���� �κ�Ʈ ����
// [ �̴Ͻ��� ]
//
// #########################################################################################
VOID CMngLocMgr04Dlg::InputDataToGrid()
{
EFS_BEGIN

	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();
	CString sValue = m_pGrid->GetText();
	
	// 2006-03-14 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++
	// ȯ�漳�� ��ġ�� ���� Ʋ����.
	// ���޺� ���������� �����ڷ� ���⿩�� ������ �����Ѵ�.
	/*
	if( !nRow )
		return;

	switch( nCol )
	{
		case 2 :		// �̿��󱸺�-�����ڷ���⿩��
			SetYNCheck( sValue, nRow, nCol );
			break;
		case 3 :		// ����Ⱓ
		case 4 :		// �ݳ������ϼ�
		case 5 :		// ���Ⱑ��å��
		case 6 :		// ������ȿ�ϼ�
		case 7 :		// ���డ��å��
			GetGridCellNewValue( sValue, nRow, nCol );
			break;
		default :
			break;
	}
	*/
	if ( m_pGrid == (CESL_Grid*)GetDlgItem(IDC_gridPOSITION) )
	{
		// ����� �����ϰų� ù �÷��� �����ϸ� �����ؼ��� �ȵȴ�.
		if ( !nRow || nCol == 1 )	return;

		// 2009.02.25 UPDATE BY PWR : �̵������� ó��
		if( nCol == 2 ) 
		{ // �����ڷ���⿩�� ������ �Ѵ�.
			SetYNCheck( sValue, nRow, nCol );
		}
		if( m_pInfo->MODE == 10000 )
		{ // �̵��������϶�
			if( (nCol >= 3 && nCol <= 5) || nCol == 9 )
			{ // ���� �Է��� �� �ִ� �Է�â�� �����.
				 GetGridCellNewValue( sValue, nRow, nCol );
			}
		}
		else
		{
			if( nCol >= 3 && nCol <= 13 && nCol != 9 )
			{ // ���� �Է��� �� �ִ� �Է�â�� �����.
				 GetGridCellNewValue( sValue, nRow, nCol );
			}
		}
		//// �����ڷ���⿩�� ������ �Ѵ�.
		//if ( nCol == 2 ) SetYNCheck( sValue, nRow, nCol );
		//// ���� �Է��� �� �ִ� �Է�â�� �����.
		//else if ( nCol >= 3 && nCol <= 13 ) GetGridCellNewValue( sValue, nRow, nCol );
	}
	// ���޺���ϱ��к� ����
	else if ( m_pGrid == (CESL_Grid*)GetDlgItem(IDC_gridPOSITION2) )
	{
		// ����� �����̴�.
		if ( nRow < 2 )			return;

		GetGridCellNewValue( sValue, nRow, nCol );				
	}
	// -------------------------------------------------------------------------
EFS_END
}

// #########################################################################################
// [ �Լ����� ]
//	 ���õ� ���� ���ο� ���� �Է��� �� �ִ� �Է�â�� �����Ѵ�.
// [ �̴Ͻ��� ]
//
// #########################################################################################
VOID CMngLocMgr04Dlg::GetGridCellNewValue(CString value, INT row, INT col)
{
EFS_BEGIN
		
	// ===================================================================================
	// 1. �ʱ�ȭ
	// ===================================================================================
	//2004-12-20
	//modified by loveisverb
	FreeGridEdt();

	if( m_bFirstReturn ) 
		m_bFirstReturn = FALSE;
	else
	{
		m_bFirstReturn = TRUE;
		return;
	}

	// ===================================================================================
	// 2. �Է�â ����
	// ===================================================================================

	// 2006-03-14 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ���޺� ������ ���޺���ϱ��к� ������ ���� ����ϰ� ���� �Ǿ���.
	/*
	CWnd *pWnd = ( CWnd * )GetDlgItem( IDC_gridPOSITION );
	*/
	CWnd* pWnd = ( CWnd* )m_pGrid;
	// -----------------------------------------------------------------------------------
	m_pGridEdt = new CGridEdt;	
	m_pGridEdt->row = row;
	m_pGridEdt->col = col;
	m_pGridEdt->parent = ( CWnd * )this;
	m_pGridEdt->Create( ES_LEFT|ES_MULTILINE|ES_UPPERCASE|ES_AUTOVSCROLL|WS_CHILD, CRect( 0 , 0, 10, 20 ), pWnd->GetParent(), 645 );

	// ===================================================================================
	// 3. ������ �Է�â�� ��ġ �� ũ�� ����
	// ===================================================================================
	CRect rect,gridRect;
	RECT parentRect;
	pWnd->GetWindowRect( &gridRect );
	pWnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( pWnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );

	gridRect.top  -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left   = gridRect.left + ( long )( ( float )m_pGrid->GetCellLeft()   / 15.0f );
	rect.top    = gridRect.top  + ( long )( ( float )m_pGrid->GetCellTop()    / 15.0f );
	rect.right  = rect.left     + ( long )( ( float )m_pGrid->GetCellWidth()  / 15.0f );
	rect.bottom = rect.top      + ( long )( ( float )m_pGrid->GetCellHeight() / 15.0f );
	m_pGridEdt->MoveWindow( rect.left - 1, rect.top - 1, rect.Width(), rect.Height() );

	// ===================================================================================
	// 4. �Է�â���� ��Ŀ�� ����
	// ===================================================================================
	m_pGridEdt->ShowWindow( true );
	m_pGridEdt->SetFocus();

	m_pGridEdt->SetWindowText( value );
    m_pGridEdt->SetSel( 0, -1 );
	
	// 2006-03-14 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	// ���޺���ϱ��к������� 'N' ������ �����ϵ��� ����
	if ( m_pGrid == (CESL_Grid*)GetDlgItem(IDC_gridPOSITION2) && ( col%2 ) )
		m_pGridEdt->m_bUseNotMode = TRUE;	
	// -----------------------------------------------------------------------------------

EFS_END
}

VOID CMngLocMgr04Dlg::FreeGridEdt()
{
	EFS_BEGIN

	if( m_pGridEdt != NULL )
	{
		delete m_pGridEdt;
		m_pGridEdt = NULL;
	}

	EFS_END
}

VOID CMngLocMgr04Dlg::SetGridText(INT row, INT col, CString text)
{
	EFS_BEGIN

	INT curRow = m_pGrid->GetRow();
	INT curCol = m_pGrid->GetCol();

	m_pGrid->SetRow( row );
	m_pGrid->SetCol( col );
	m_pGrid->SetText( text );

	m_pGrid->SetRow( curRow );
	m_pGrid->SetCol( curCol );

	EFS_END
}

VOID CMngLocMgr04Dlg::OnUpdateGridCelText(WPARAM w, LPARAM l)
{
	EFS_BEGIN

	if( w )
	{
		CString newValue;
		m_pGridEdt->GetWindowText( newValue );
		// 2006-03-14 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// N�� �ٸ� Ű�Է��� �޴´ٸ� N���� ���� ��Ų��.
		if ( newValue.Replace( _T("N"), _T("") ) > 0 )
			newValue = _T("N");
		// ---------------------------------------------------------------------------------
		SetGridText( m_pGridEdt->row, m_pGridEdt->col, newValue );
	}
	FreeGridEdt();
	m_pGrid->SetFocus();


	EFS_END
}


INT CMngLocMgr04Dlg::SetYNCheck( CString sValue , INT nRow , INT nCol )
{
	EFS_BEGIN

	if( sValue.Compare(_T("Y"))==0 )
		sValue = _T("N");
	else 
		sValue = _T("Y");

	m_pGrid->SetAt( nRow-1 , nCol-1 , sValue );

	return 0;

	EFS_END
	return -1;

}

VOID CMngLocMgr04Dlg::ViewMultiCombo(CString sValue, INT nRow, INT nCol )
{
	EFS_BEGIN

	CString sCode;
	if( sValue.GetLength() )
		sCode = GetCodeFromDM( nRow - 1 );	
	
	CRect rect = GetGridCellRect();

	m_pGrid->EnableWindow( false );
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbUSE_OBJ_CODE );
	INT rowCount = pCombo->GetItemHeight( 0 );
	pCombo->MoveWindow( rect.left - 2, rect.top - 3, rect.Width() + 2, rect.Height()  );

	pCombo->ShowWindow( true );
	pCombo->SetFocus();

	if( sCode.GetLength() )
		SetControlData( m_sCMAlias, _T("�̿��󱸺�"), sCode );

	EFS_END
}

CString CMngLocMgr04Dlg::GetCodeFromDM(INT dmRow)
{
	EFS_BEGIN

	CString sCode;
	m_pDM->GetCellData( _T("�̿��󱸺�"), dmRow, sCode );

	return sCode;

	EFS_END
	return _T("");

}

CRect CMngLocMgr04Dlg::GetGridCellRect()
{
	CWnd *wnd = ( CWnd * )GetDlgItem( IDC_gridPOSITION );

	CRect rect, gridRect;
	RECT parentRect;
	wnd->GetWindowRect( &gridRect );
	wnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( wnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );
	
	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left	= gridRect.left +	( long )( ( float )m_pGrid->GetCellLeft()		/ 15.0f );
	rect.top	= gridRect.top +	( long )( ( float )m_pGrid->GetCellTop()		/ 15.0f );
	rect.right	= rect.left +		( long )( ( float )m_pGrid->GetCellWidth()	/ 15.0f );
	rect.bottom = rect.top +		( long )( ( float )m_pGrid->GetCellHeight()	/ 15.0f );

	return rect;
}

VOID CMngLocMgr04Dlg::OnEditchangecmbUSEOBJCODE() 
{
EFS_BEGIN
EFS_END
}


CString CMngLocMgr04Dlg::TakeControlData(CString sAlias, INT nOption)
{
	EFS_BEGIN

	// option == 0 : ��Ƽ�޺��� �ڵ尪�� �����´�. 
	// option == 1 : ��Ƽ�޺��� �ڵ弳���� �����´�. 
	
	CString sResult;

	INT ids = 0;
	ids = GetControlData( m_sCMAlias, sAlias , sResult, -1, nOption );
	if(0 > ids)
	{
		MessageBox( _T("Error : GetControlData() of CM_�ηϿ���"), _T("K2Up") );
		return _T("");
	}
	
	// �ڵ尪�� _T("0") �̰ų� ������ _T("�������")�϶� _T("")�� ��ȯ�Ѵ�. 
	switch( nOption )
	{
		case 0 :
			if( _T("0") == sResult )
				return _T("");
			break;
		case 1 :
			if( _T("�������") == sResult )
				return _T("");
			break;
		default :
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_�ηϿ���"), _T("K2Up") );
			return _T("");
			break;
	}

	return sResult;

	EFS_END
	return _T("");

}

VOID CMngLocMgr04Dlg::OnSelchangecmbUSEOBJCODE() 
{
	EFS_BEGIN

	CString sValue = TakeControlData( _T("�̿��󱸺�") , 0);
	
	INT nRow = m_pGrid->GetRow();
	
	m_pDM->SetCellData( _T("�̿��󱸺��ڵ�") , sValue , nRow-1 );

	sValue = sValue + _T(" : ") + TakeControlData( _T("�̿��󱸺�"), 1 );
	m_pGrid->SetText( sValue );

	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbUSE_OBJ_CODE );
	pCombo->ShowWindow( false );	
	
	m_pGrid->EnableWindow( true );
	m_pGrid->SetFocus();				

	EFS_END
}

VOID CMngLocMgr04Dlg::OnDblClickgridPOSITION() 
{
EFS_BEGIN
	
	// 2006-03-14 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++
	// �׸��� �����Ͱ� ���������� ���Ѵ�.
	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_gridPOSITION);
	// --------------------------------------------------------------------
	INT nCol = m_pGrid->GetCol();
	
	m_bFirstReturn = TRUE;
	InputDataToGrid();
	
	m_pGrid->SetCol( nCol );

EFS_END
}

VOID CMngLocMgr04Dlg::OnKeyUpgridPOSITION(short FAR* KeyCode, short Shift) 
{
EFS_BEGIN

	/*
	if( *KeyCode == 13 )
	{
		INT nCol = m_pGrid->GetCol();

		InputDataToGrid();
		
		m_pGrid->SetCol( nCol );
		
	}
	*/

EFS_END
}

BOOL CMngLocMgr04Dlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN


	if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_TAB)
	{
		//2004.12.16
		// ����Ű�� ������� InputDataToGrid�� �� ȣ������ �ʰ� WM_LBUTTONDBLCLK �޽����� ������.
		//InputDataToGrid();
		PostMessage(WM_LBUTTONDBLCLK,NULL,NULL);
		return TRUE;
	}


	// 2004.12.16
	// modified by loveisverb
	// �׸��忡�� ���콺 ������ ��ư�� �̿��� copy & paste ���� 
	if( pMsg->message == WM_RBUTTONUP )
	{
		HWND MsgH = pMsg->hwnd;
		
		if( GetDlgItem(IDC_gridPOSITION)->m_hWnd==MsgH )
		{
			ShowCopyAndPasteMenu(pMsg->pt);
			return TRUE;
		}

		
	}

	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}

INT CMngLocMgr04Dlg::ClassRegCodeView()
{
	EFS_BEGIN

	INT ids;

	ids = ClassRegCodeGridInit();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ClassRegCodeView") );

	ids = ClassRegCodeShowValue();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ClassRegCodeView") );
	

	return 0;


	EFS_END

	return -1;
}

// #########################################################################################
// [ �Լ����� ]
//   ���޺���ϱ��к� �׸��� �ʱ�ȭ
// [ �̴Ͻ��� ]
// 
// #########################################################################################
INT CMngLocMgr04Dlg::ClassRegCodeGridInit()
{
EFS_BEGIN

	INT ids;
	
	// ==================================================================================
	// 1. DM �ʱ�ȭ
	// ==================================================================================
	// ��ϱ����� �����´�.
	CESL_DataMgr* pRegCodeDM = FindDM(_T("DM_MNG_LOC_MGR_04_REG_CODE"));
	if( pRegCodeDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ClassRegCodeGridInit") );
	ids = CLocCommonAPI::RefreshDataManager( this , pRegCodeDM , _T("") );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ClassRegCodeGridInit") );
	
	// ���ޱ����� �����´�.
	CESL_DataMgr* pClassDM = FindDM(_T("DM_MNG_LOC_MGR_04_CLASS_CODE"));
	if( pClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ClassRegCodeGridInit") );
	ids = CLocCommonAPI::RefreshDataManager( this , pClassDM , _T("") );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ClassRegCodeGridInit") );

	// ȯ�氪�� �����´�.
	CESL_DataMgr* pEnvDM = FindDM(_T("DM_MNG_LOC_MGR_04_2_DLG"));
	if( pEnvDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ClassRegCodeGridInit") );
	ids = CLocCommonAPI::RefreshDataManager( this , pEnvDM , _T("") );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ClassRegCodeGridInit") );	
	
	// ==================================================================================
	// 2. �׸��� ����
	// ==================================================================================
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_LOC_MGR_04_2_DLG") , _T("����ȯ�漳��"));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ClassRegCodeGridInit") );
	
	// 2006-03-14 REMOVE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �⺻ �Է�â���� �������� �ʴ´�.
	/*
	pGrid->m_bGridEdit = TRUE;
	*/
	// ----------------------------------------------------------------------------------

	INT nCols = pRegCodeDM->GetRowCount()*2 + 1;
	pGrid->SetCols(  0 , nCols );
	pGrid->SetFixedCols(1);
	pGrid->SetMergeCells(1);
//	pGrid->SetMergeCol(1,TRUE);
	pGrid->SetMergeRow(0,TRUE);
	
	// ��� ����( ��ϱ��� )
	CString sColumnName;
	for( INT i = 0 ; i < pRegCodeDM->GetRowCount() ; i++ )
	{
		ids = pRegCodeDM->GetCellData( _T("��ϱ���") , i , sColumnName );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ClassRegCodeGridInit") );
		
		pGrid->SetTextMatrix( 0 , i*2+1 , sColumnName );
		pGrid->SetTextMatrix( 0 , i*2+2 , sColumnName );
		pGrid->SetTextMatrix( 1 , i*2+1 , _T("�������") );
		pGrid->SetTextMatrix( 1 , i*2+2 , _T("�ݳ������ϼ�") );
		pGrid->SetColWidth( i*2+1 , 0 , 900);
		pGrid->SetColWidth( i*2+2 , 0 , 1200);
		pGrid->SetCol(i*2+1);
		pGrid->SetRow(0);
		pGrid->SetCellAlignment(4);
		pGrid->SetCellFontBold(TRUE);			
	}
	
	// ���� �� �� ���� 
	INT nRows = pClassDM->GetRowCount() + 2;
	pGrid->SetRows(nRows);
	pGrid->SetFixedRows(2);
	pGrid->SetColWidth( 0 , 0 , 900);
	for( i = 0 ; i < pClassDM->GetRowCount() ; i++ )
	{
		ids = pClassDM->GetCellData( _T("����") , i , sColumnName );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ClassRegCodeGridInit") );
		
		pGrid->SetTextMatrix( i+2 , 0 , sColumnName );
		pGrid->SetCol(0);
		pGrid->SetRow(i+2);
		pGrid->SetCellFontBold(TRUE);
	}
	return 0;

EFS_END
	return -1;		
}


INT CMngLocMgr04Dlg::ClassRegCodeShowValue()
{
	EFS_BEGIN


	INT ids;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_LOC_MGR_04_2_DLG") , _T("����ȯ�漳��"));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ClassRegCodeShowValue") );
	
	CESL_DataMgr* pEnvDM = FindDM(_T("DM_MNG_LOC_MGR_04_2_DLG"));
	if( pEnvDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("ClassRegCodeShowValue") );
	CESL_DataMgr* pRegCodeDM = FindDM(_T("DM_MNG_LOC_MGR_04_REG_CODE"));
	if( pRegCodeDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ClassRegCodeGridInit") );
	CESL_DataMgr* pClassDM = FindDM(_T("DM_MNG_LOC_MGR_04_CLASS_CODE"));
	if( pClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ClassRegCodeGridInit") );
	

	CString sLoanPeriod , sDelayDay;
	CString sRegKey , sClassKey;
	CString sColumn;
	INT nColIndex = -1;
	INT nRowIndex = -1;

	INT nTmp = pEnvDM->GetRowCount();
	for( INT i = 0 ; i < pEnvDM->GetRowCount() ; i++, nColIndex = -1, nRowIndex = -1 )
	{
		ids = pEnvDM->GetCellData( _T("�������") , i , sLoanPeriod );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ClassRegCodeShowValue") );
		ids = pEnvDM->GetCellData( _T("�ݳ������ϼ�") , i , sDelayDay );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ClassRegCodeShowValue") );
		ids = pEnvDM->GetCellData( _T("��ϱ���KEY") , i , sRegKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ClassRegCodeShowValue") );
		ids = pEnvDM->GetCellData( _T("����KEY") , i , sClassKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ClassRegCodeShowValue") );

		nTmp = pRegCodeDM->GetRowCount();
		for( INT j = 0 ; j < pRegCodeDM->GetRowCount() ; j++ )
		{
			ids = pRegCodeDM->GetCellData( _T("��ϱ���KEY") , j , sColumn );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ClassRegCodeShowValue") );

			if( sColumn.Compare(sRegKey) == 0 )
				nColIndex = (j*2)+1;
		}

		nTmp = pClassDM->GetRowCount();
		for( j = 0 ; j < pClassDM->GetRowCount() ; j++ )
		{
			ids = pClassDM->GetCellData( _T("����KEY") , j , sColumn );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ClassRegCodeShowValue") );
			
			if( sColumn.Compare(sClassKey) == 0 )
				nRowIndex = j+2;
		}

		if(0 < nRowIndex && 0 < nColIndex)
		{
			pGrid->SetTextMatrix( nRowIndex , nColIndex , sLoanPeriod );
			pGrid->SetTextMatrix( nRowIndex , nColIndex+1 , sDelayDay );
		}
	}



	return 0;

	EFS_END
	
	return -1;

}

// #########################################################################################
// [ �Լ����� ]
//   ���޺���ϱ��к� ������ DB �ݿ�
// [ �̴Ͻ��� ]
// 
// #########################################################################################
INT CMngLocMgr04Dlg::SaveRegClassValue()
{
EFS_BEGIN

	INT ids;

	// 1. �ʱ�ȭ
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_LOC_MGR_04_2_DLG"), _T("����ȯ�漳��"));
	if(NULL == pGrid)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("SaveRegClassValue"));
	}
	CESL_DataMgr* pEnvDM = FindDM(_T("DM_MNG_LOC_MGR_04_2_DLG"));
	if(NULL == pEnvDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("SaveRegClassValue"));
	}
	CESL_DataMgr* pRegCodeDM = FindDM(_T("DM_MNG_LOC_MGR_04_REG_CODE"));
	if(NULL == pRegCodeDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("SaveRegClassValue"));
	}
	CESL_DataMgr* pClassDM = FindDM(_T("DM_MNG_LOC_MGR_04_CLASS_CODE"));
	if(NULL == pClassDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1030, _T("SaveRegClassValue"));
	}
	if(m_strLibCode.IsEmpty())
	{
		m_strLibCode = m_pInfo->LIB_CODE;
	}

	INT nCols = pGrid->GetCols(0);
	INT nRows = pGrid->GetRows();

	CString sRecKey, sRegKey, sClassKey, strLoanPeriod_CM, strDelayDay_CM, sValue;

	// �̵��������� ���� ����
	CString strTL_SetData;
	CString strTL_Condition;

	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi(this, pEnvDM);
	pMobileApi = CLocCommonAPI::InitMobileApi(this, pEnvDM);
	pMobileApi->StartFrame();

	// �׸��忡 �Էµ� �� �����ͼ� ��
	for(INT i = 2 ; i < nRows; i++) // ����
	{
		ids = pClassDM->GetCellData(_T("����KEY"), i-2, sClassKey);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("SaveRegClassValue"));
		}

		for(INT j = 1; j < nCols; j++) //��ϱ���
		{
			strTL_SetData.Empty();
			strTL_Condition.Empty();

			ids = pRegCodeDM->GetCellData( _T("��ϱ���KEY"), (j-1)/2, sRegKey);
			if(0 > ids)
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("SaveRegClassValue"));
			}
	
			// ����Ⱓ
			sValue = pGrid->GetTextMatrix(i, j);
			sValue.TrimLeft(); sValue.TrimRight();
			if(!sValue.IsEmpty())
			{
				strLoanPeriod_CM.Format( _T("%d"), _ttoi(sValue));
			}
			else
			{
				strLoanPeriod_CM = _T("");
			}
			// �ݳ������ϼ�
			sValue = pGrid->GetTextMatrix(i, j+1);
			sValue.TrimLeft() ; sValue.TrimRight();
			if(!sValue.IsEmpty())
			{
				strDelayDay_CM.Format(_T("%d"), _ttoi(sValue));
			}
			else 			
			{
				strDelayDay_CM = _T("");
			}

			// �׸��忡 ��ϱ��к� ������ �׸��忡�� �ִ� �������� DB���� �ִ� �������� Ȯ���Ѵ�.
			BOOL bIsFindDM = FALSE; // DB(MN_LOAN_ENV_POSREG_TBL)�� �ִ��� ���� 
			sRecKey.Empty();
			CString strEnvRecKey, strEnvClassKey, strEnvRegKey;
			INT nEnvCnt = 0;
			const INT ENV_CNT = pEnvDM->GetRowCount(); // ���� MN_LOAN_ENV_POSREG_TBL����
			for(nEnvCnt = 0; nEnvCnt < ENV_CNT; nEnvCnt++)
			{
				ids = pEnvDM->GetCellData(_T("REC_KEY"), nEnvCnt, strEnvRecKey);
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1060, _T("SaveRegClassValue"));
				}
				else if(strEnvRecKey.IsEmpty())
				{
					continue;
				}

				ids = pEnvDM->GetCellData(_T("����KEY"), nEnvCnt, strEnvClassKey);
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1070, _T("SaveRegClassValue"));
				}
				else if(strEnvClassKey != sClassKey)
				{
					continue;
				}

				ids = pEnvDM->GetCellData(_T("��ϱ���KEY"), nEnvCnt, strEnvRegKey);
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1080, _T("SaveRegClassValue"));
				}
				else if(strEnvRegKey != sRegKey)
				{
					continue;
				}

				ids = pEnvDM->SetCellData(_T("62_CMD_TYPE"), _T("U"), nEnvCnt);
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1090, _T("SaveRegClassValue"));
				}
				ids = pEnvDM->SetCellData(_T("83_CMD_TYPE"), _T("U"), nEnvCnt);
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1100, _T("SaveRegClassValue"));
				}

				sRecKey = strEnvRecKey;
				bIsFindDM = TRUE;
				break;
			} // ���� DB�� �� ��
			
			pMobileApi->InitDBFieldData();

			// CM���� ���� ���µ� DM���� �ִٸ� �����Ѱ� -> DELETE
			if(_T("") == strDelayDay_CM) 
			{ // 62 COMMAND (���Ϳ� ��ϱ����� ���޺� �ݳ������ϼ� ����)
				if(TRUE == bIsFindDM)
				{	
					// ���;˸�
					ids = pEnvDM->SetCellData( _T("62_CMD_TYPE"), _T("D"), nEnvCnt);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1100, _T("SaveRegClassValue"));
					}
				}
			}
			if(_T("") == strLoanPeriod_CM && _T("") == strDelayDay_CM)
			{ // 83 COMMAND (Ÿ���� ��ϱ����� ���޺� ����Ⱓ,�ݳ������ϼ� ����ȭ) 
				if(TRUE == bIsFindDM)
				{
					// �ڰ� ���� ����
					pMobileApi->MakeDeleteFrame(_T("MN_LOAN_ENV_POSREG_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
					// Ÿ�� ����ȭ
					ids = pEnvDM->SetCellData( _T("83_CMD_TYPE"), _T("D"), nEnvCnt);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1110, _T("SaveRegClassValue"));
					}
				}
			}
			else
			{
				// CM���� ���� �ְ� DM���� �ִٸ� ������ ���� -> UPDATE
				if(TRUE == bIsFindDM)
				{ 
					CString strLoanPeriod_DM, strDelayDay_DM;
					pEnvDM->GetCellData(_T("�������"), nEnvCnt, strLoanPeriod_DM);
					pEnvDM->GetCellData(_T("�ݳ������ϼ�"), nEnvCnt, strDelayDay_DM);
					// ��������� ������ ������Ʈ ����.
					// 62 COMMAND
					if(strDelayDay_DM == strDelayDay_CM)
					{
						ids = pEnvDM->SetCellData( _T("62_CMD_TYPE"), _T(""), nEnvCnt);					
						if(0 > ids)
						{
							ERROR_MSG_RETURN_ERROR_CODE2(-1120, _T("SaveRegClassValue"));
						}
					}
					else
					{
						if(_T("") == strDelayDay_CM)
						{ // ���ٸ� DELETE�Ѵ�.
							ids = pEnvDM->SetCellData( _T("62_CMD_TYPE"), _T("D"), nEnvCnt);
							if(0 > ids)
							{
								ERROR_MSG_RETURN_ERROR_CODE2(-1130, _T("SaveRegClassValue"));
							}
						}
						else
						{
							ids = pEnvDM->SetCellData( _T("62_CMD_TYPE"), _T("U"), nEnvCnt);
							if(0 > ids)
							{
								ERROR_MSG_RETURN_ERROR_CODE2(-1130, _T("SaveRegClassValue"));
							}
						}
					}
					// 83 COMMAND
					if(strLoanPeriod_DM == strLoanPeriod_CM && strDelayDay_DM == strDelayDay_CM)
					{
						ids = pEnvDM->SetCellData( _T("83_CMD_TYPE"), _T(""), nEnvCnt);
						if(0 > ids)
						{
							ERROR_MSG_RETURN_ERROR_CODE2(-1140, _T("SaveRegClassValue"));
						}
					}
					else
					{
						if(_T("") == strLoanPeriod_CM && _T("") == strDelayDay_CM)
						{ // ���ٸ� DELETE�Ѵ�.
							pMobileApi->MakeDeleteFrame(_T("MN_LOAN_ENV_POSREG_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
							ids = pEnvDM->SetCellData( _T("83_CMD_TYPE"), _T("D"), nEnvCnt);
							if(0 > ids)
							{
								ERROR_MSG_RETURN_ERROR_CODE2(-1150, _T("SaveRegClassValue"));
							}
						}
						else
						{
							pMobileApi->AddDBFieldData(_T("REG_CODE_KEY"), _T("NUMERIC"), sRegKey);
							pMobileApi->AddDBFieldData(_T("LOAN_CLASS_CODE_KEY"), _T("NUMERIC"), sClassKey);
							pMobileApi->AddDBFieldData(_T("LOAN_PERIOD"), _T("NUMERIC"), strLoanPeriod_CM);
							pMobileApi->AddDBFieldData(_T("POSTPONEMENT_PERIOD"), _T("NUMERIC"), strDelayDay_CM);			
							pMobileApi->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("����ȯ�漳��"), __WFILE__, __LINE__ ), TRUE);
							pMobileApi->MakeUpdateFrame(_T("MN_LOAN_ENV_POSREG_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);

							ids = pEnvDM->SetCellData( _T("83_CMD_TYPE"), _T("U"), nEnvCnt);
							if(0 > ids)
							{
								ERROR_MSG_RETURN_ERROR_CODE2(-1150, _T("SaveRegClassValue"));
							}
						}
					}
				}
				// CM���� ���� �ְ� DM���°��� ���ٸ� �߰������� ���� -> INSERT
				else
				{ 
					pMobileApi->MakeRecKey( sRecKey );
					pMobileApi->AddDBFieldData(_T("REG_CODE_KEY"), _T("NUMERIC"), sRegKey);
					pMobileApi->AddDBFieldData(_T("LOAN_CLASS_CODE_KEY"), _T("NUMERIC"), sClassKey);

					pEnvDM->InsertRow(-1);					
					INT nInsertIndex = pEnvDM->GetRowCount() - 1;

					ids = pEnvDM->SetCellData(_T("REC_KEY"), sRecKey, nInsertIndex);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1160, _T("SaveRegClassValue"));
					}
					ids = pEnvDM->SetCellData(_T("����KEY"), sClassKey, nInsertIndex);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1170, _T("SaveRegClassValue"));
					}
					ids = pEnvDM->SetCellData(_T("��ϱ���KEY"), sRegKey, nInsertIndex);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1180, _T("SaveRegClassValue"));
					}
					ids = pEnvDM->SetCellData(_T("�������"), strLoanPeriod_CM, nInsertIndex);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1190, _T("SaveRegClassValue"));
					}
					ids = pEnvDM->SetCellData(_T("�ݳ������ϼ�"), strDelayDay_CM, nInsertIndex);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1200, _T("SaveRegClassValue"));
					}
					ids = pEnvDM->SetCellData(_T("83_CMD_TYPE"), _T("I"), nInsertIndex);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1220, _T("SaveRegClassValue"));
					}

					if(_T("") != strLoanPeriod_CM)
					{
						pMobileApi->AddDBFieldData(_T("LOAN_PERIOD"), _T("NUMERIC"), strLoanPeriod_CM);
					}
					if(_T("") != strDelayDay_CM)
					{ // CMD 62
						pMobileApi->AddDBFieldData(_T("POSTPONEMENT_PERIOD"), _T("NUMERIC"), strDelayDay_CM);			
						ids = pEnvDM->SetCellData(_T("62_CMD_TYPE"), _T("I"), nInsertIndex);
						if(0 > ids)
						{
							ERROR_MSG_RETURN_ERROR_CODE2(-1220, _T("SaveRegClassValue"));
						}
					}
					pMobileApi->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("����ȯ�漳��"), __WFILE__, __LINE__ ), TRUE);
					pMobileApi->MakeInsertFrame(_T("MN_LOAN_ENV_POSREG_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
				} // DB�� �������� ���� ����� ��

			} // �׸��忡 �������� �ִ� ����� ��

			//JOB.2019.0073
			/*
			//�̵��������� ���
			if( m_pInfo->MODE == 10000 )
			{
				if(!strLoanPeriod_CM.IsEmpty() && !strLoanPeriod_CM.IsEmpty())
				{
					if(strLoanPeriod_CM.IsEmpty()) strLoanPeriod_CM = _T("0");
					if(strDelayDay_CM.IsEmpty()) strDelayDay_CM = _T("0");
					strTL_SetData.Format(_T("REG_CODE_KEY=%s,LOAN_CLASS_CODE_KEY=%s,LOAN_PERIOD=%s,POSTPONEMENT_PERIOD=%s")
						,sRegKey,sClassKey,strLoanPeriod_CM,strDelayDay_CM);
					strTL_Condition.Format(_T("REG_CODE_KEY=%s AND LOAN_CLASS_CODE_KEY=%s"),sRegKey,sClassKey);
					
					m_syncApi.RefreshSyncData(_T("MN_LOAN_ENV_POSREG_TBL"),strTL_SetData,strTL_Condition);
				}
			}
			*/
			j++;
		}
		
	}

 	ids = pMobileApi->SendFrame();
	pEnvDM->EndFrame();

	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveRegClassValue") );
	}
		
	// ���� �ݳ����� ���� ����� ���� �������� �˸��� 	
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("MN_LOAN_ENV_POSREG_TBL");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;

	CString strEnvRecKey, strEnvType, strEnvClassKey, strEnvRegKey;
	CString strValue, strRegCode, strClassCode, strLoanPeriod, strPostponePeriod;
	for( i = 0 ; i < pEnvDM->GetRowCount() ; i++ )
	{
		pEnvDM->GetCellData(_T("REC_KEY"), i, strEnvRecKey);
		if(strEnvRecKey.IsEmpty())
		{
			continue;
		}
		pEnvDM->GetCellData(_T("62_CMD_TYPE"), i, strEnvType);
		if(strEnvType.IsEmpty())
		{
			continue;
		}
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields.Empty();
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("62");
		UploadShareInfo.UploadShareLoanInfo.sType = strEnvType;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = strEnvRecKey;
		if(_T("U") == strEnvType)
		{
			UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("POSTPONEMENT_PERIOD");
		}
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	} // �����̷°��� ���� ������ ��
	
	return 0;

EFS_END
return -1;
}
// INT CMngLocMgr04Dlg::SaveRegClassValue()
// {
// EFS_BEGIN
// 
// 	INT ids;
// 
// 	CString sTableName = _T("MN_LOAN_ENV_POSREG_TBL");
// 	
// 	// ====================================================================================
// 	// 1. �ʱ�ȭ
// 	// ====================================================================================
// 	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_LOC_MGR_04_2_DLG") , _T("����ȯ�漳��"));
// 	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveRegClassValue") );
// 	
// 	CESL_DataMgr* pEnvDM = FindDM(_T("DM_MNG_LOC_MGR_04_2_DLG"));
// 	if( pEnvDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("SaveRegClassValue") );
// 	CESL_DataMgr* pRegCodeDM = FindDM(_T("DM_MNG_LOC_MGR_04_REG_CODE"));
// 	if( pRegCodeDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveRegClassValue") );
// 	CESL_DataMgr* pClassDM = FindDM(_T("DM_MNG_LOC_MGR_04_CLASS_CODE"));
// 	if( pClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveRegClassValue") );
// 
// 	// 2005.05.26 ADD BY PDJ +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	if ( m_strLibCode.IsEmpty() )
// 	{
// 		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE�� ����
// // 		CString	strWhere;
// // 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE ")
// // 			            _T("MANAGE_CODE IN ( SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
// // 						_T("WHERE ID = '%s')"), m_strUserID );
// // 		pEnvDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
// 		m_strLibCode = m_pInfo->LIB_CODE;
// 	}
// 	// -----------------------------------------------------------------------------------
// 
// 	// �����Ѵ�.
// 	INT nCols = pGrid->GetCols(0);
// 	INT nRows = pGrid->GetRows();
// 
// 	CString sRegKey , sClassKey , sLoanPeriod , sDelayDay , sValue;
// 	CString sRecKey;
// 
// 	// ######################################################################################
// 	// --------------------------------------------------------------------------------------
// 	// ���޺� ��ϱ��к� �ݳ����� ���� ����� ���� �������� �˸��� 
// 	// 2003�� 11�� 8�� �۾�	
// 	UPLOAD_SHARE_INFO UploadShareInfo;
// 	UploadShareInfo.nType = 0;
// 	
// 	UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
// 	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("62");
// 	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("MN_LOAN_ENV_POSREG_TBL");
// 	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
// 	// --------------------------------------------------------------------------------------
// 	// ######################################################################################	
// 
// 	// Mobile API ����
// 	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , pEnvDM );
// 	pMobileApi->StartFrame();
// 
// 	for( INT i = 0 ; i < pEnvDM->GetRowCount() ; i++ )
// 	{
// 		ids = pEnvDM->GetCellData( _T("REC_KEY") , i , sRecKey );
// 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );
// 
// 		pEnvDM->InitDBFieldData();
// 		pMobileApi->MakeDeleteFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
// 
// 		// ######################################################################################
// 		// --------------------------------------------------------------------------------------
// 		// ���� �ݳ����� ���� ����� ���� �������� �˸��� 
// 		// 2003�� 11�� 8�� �۾�		
// 		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
// 		
// 		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );		
// 		// --------------------------------------------------------------------------------------
// 		// ######################################################################################
// 	}
// 
// 	ids = pMobileApi->SendFrame();
// 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveRegClassValue") );
// 	
// 	pMobileApi->EndFrame();
// 	
// 	//=============================================
// 	//2009.02.20 REM BY LKS : �̵��������� ��������
// 	//CLocCommonAPI::MobileApiEndLog( pMobileApi );
// 	//=============================================
// 
// 	pEnvDM->FreeData();
// 
// 	// ######################################################################################
// 	// --------------------------------------------------------------------------------------
// 	// ���޺� ��ϱ��к� �ݳ����� ���� ����� ���� �������� �˸��� 
// 	// 2003�� 11�� 8�� �۾�	
// 	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
// 
// 	CStringArray asRecKey;
// 	// --------------------------------------------------------------------------------------
// 	// ######################################################################################
// 	
// 	//�̵��������� ���� ����
// 	CString strTL_SetData;
// 	CString strTL_Condition;
// 
// 	pMobileApi = CLocCommonAPI::InitMobileApi( this , pEnvDM );
// 	pMobileApi->StartFrame();
// 	for( i = 2 ; i < nRows ; i++ )
// 	{
// 		ids = pClassDM->GetCellData( _T("����KEY") , i-2 , sClassKey );
// 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveRegClassValue") )
// 
// 		for( INT j = 1 ; j < nCols ; j++ )
// 		{
// 			strTL_SetData.Empty();
// 			strTL_Condition.Empty();
// 
// 			ids = pRegCodeDM->GetCellData( _T("��ϱ���KEY") , (j-1)/2 , sRegKey );
// 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveRegClassValue") )
// 	
// 			sValue = pGrid->GetTextMatrix( i , j );
// 			sValue.TrimLeft() ; sValue.TrimRight();
// 			if( !sValue.IsEmpty() )
// 			{
// 				// 2006-03-14 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++
// 				// 'N'�� ���� DB Column Type�� �´� ������ INT�� ����
// 				if ( sValue == _T("N") )	sValue = _T("-1");
// 				// ---------------------------------------------------------------------
// 				sLoanPeriod.Format( _T("%d") , _ttoi(sValue) );
// 			}
// 			else
// 				sLoanPeriod = _T("NULL");
// 
// 			sValue = pGrid->GetTextMatrix( i , j+1 );
// 			sValue.TrimLeft() ; sValue.TrimRight();
// 			if( !sValue.IsEmpty() )
// 			{
// 				// 2006-03-14 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++
// 				// 'N'�� ���� DB Column Type�� �´� ������ INT�� ����
// 				if ( sValue == _T("N") )	sValue = _T("-1");
// 				// ---------------------------------------------------------------------
// 				sDelayDay.Format( _T("%d") , _ttoi(sValue) );
// 			}
// 			else 			
// 				sDelayDay = _T("NULL");
// 
// 			/// �ΰ��� ���� ��� NULL�̶�� �Է����� �ʴ´�.
// 			if( (sLoanPeriod == _T("NULL")) && (sDelayDay == _T("NULL")) )
// 			{
// 				j++;
// 				continue;
// 			}
// 			
// 			pMobileApi->InitDBFieldData();
// 			pMobileApi->MakeRecKey( sRecKey );
// 
// 			pMobileApi->AddDBFieldData( _T("REG_CODE_KEY") , _T("NUMERIC") , sRegKey );
// 			pMobileApi->AddDBFieldData( _T("LOAN_CLASS_CODE_KEY") , _T("NUMERIC") , sClassKey );
// 			pMobileApi->AddDBFieldData( _T("LOAN_PERIOD") , _T("NUMERIC") , sLoanPeriod );
// 			pMobileApi->AddDBFieldData( _T("POSTPONEMENT_PERIOD") , _T("NUMERIC") , sDelayDay );
// 			pMobileApi->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("����ȯ�漳��"), __WFILE__, __LINE__ ), TRUE );
// 			
// 			pMobileApi->MakeInsertFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
// 
// 			//�̵��������� ���
// 			if( m_pInfo->MODE == 10000 )
// 			{
// 				strTL_SetData.Format(_T("REG_CODE_KEY=%s,LOAN_CLASS_CODE_KEY=%s,LOAN_PERIOD=%s,POSTPONEMENT_PERIOD=%s")
// 					,sRegKey,sClassKey,sLoanPeriod,sDelayDay);
// 				strTL_Condition.Format(_T("REG_CODE_KEY=%s AND LOAN_CLASS_CODE_KEY=%s"),sRegKey,sClassKey);
// 				
// 				m_syncApi.RefreshSyncData(_T("MN_LOAN_ENV_POSREG_TBL"),strTL_SetData,strTL_Condition);
// 			}
// 
// 			// ######################################################################################
// 			// --------------------------------------------------------------------------------------
// 			// ���� �ݳ����� ���� ����� ���� �������� �˸��� 
// 			// 2003�� 11�� 8�� �۾�			
// 			asRecKey.Add( sRecKey );
// 			// --------------------------------------------------------------------------------------
// 			// ######################################################################################
// 
// 			pEnvDM->InsertRow(-1);
// 		
// 			INT nInsertIndex = pEnvDM->GetRowCount()-1;
// 			pEnvDM->SetCellData( _T("REC_KEY") , sRecKey , nInsertIndex);
// 			pEnvDM->SetCellData( _T("����KEY") , sClassKey , nInsertIndex);
// 			pEnvDM->SetCellData( _T("��ϱ���KEY") , sRegKey , nInsertIndex);
// 			pEnvDM->SetCellData( _T("�������") , sLoanPeriod , nInsertIndex);
// 			pEnvDM->SetCellData( _T("�ݳ������ϼ�") , sDelayDay , nInsertIndex);
// 			
// 			j++;
// 		}
// 	}
// 
// 	ids = pMobileApi->SendFrame();
// 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveRegClassValue") );
// 	
// 	pEnvDM->EndFrame();
// 
// 	// ######################################################################################
// 	// --------------------------------------------------------------------------------------
// 	// ���� �ݳ����� ���� ����� ���� �������� �˸��� 
// 	// 2003�� 11�� 8�� �۾�		
// 	for( i = 0 ; i < asRecKey.GetSize() ; i++ )
// 	{
// 		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = asRecKey.GetAt(i);
// 		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
// 	}
// 	// --------------------------------------------------------------------------------------
// 	// ######################################################################################
// 	
// 	//=============================================
// 	//2009.02.20 REM BY LKS : �̵��������� ��������
// 	//CLocCommonAPI::MobileApiEndLog( pMobileApi );
// 	//=============================================
// 
// 	return 0;
// 
// EFS_END
// 	return -1;
// }


INT CMngLocMgr04Dlg::ShowCopyAndPasteMenu(POINT& pt)
{

	EFS_BEGIN

	INT nRow = m_pGrid->GetRow();
	if(0 == nRow) return 0;
	m_pGrid->SetReverse(nRow-1);


	CMenu menuCopyAndPaste;
	menuCopyAndPaste.CreatePopupMenu();

	if(m_bIsCopied)
	{
		menuCopyAndPaste.AppendMenu(MF_STRING,4001,_T("�ٿ��ֱ�"));
		menuCopyAndPaste.AppendMenu(MF_SEPARATOR);
		menuCopyAndPaste.AppendMenu(MF_STRING,4000,_T("����"));
		menuCopyAndPaste.TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
	}
	else
	{
		menuCopyAndPaste.AppendMenu(MF_STRING,4000,_T("����"));
		menuCopyAndPaste.TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
	}




	return 0;

	EFS_END

	return -1;

}
VOID CMngLocMgr04Dlg::CopyAndPasteProc(UINT nIDC)
{
	EFS_BEGIN
	
	INT nRow = m_pGrid->GetRow();
	if(0 == nRow) return;

	if(4000 == nIDC)
	{
		for(INT i=0;i<6;i++)
		{
			m_pGrid->GetAt( nRow-1 , i+1 , m_strCopiedData[i] );
		}
	
		m_bIsCopied = TRUE;

	}
	else if(4001 == nIDC)
	{

		for(INT i=0;i<6;i++)
		{
			m_pGrid->SetAt( nRow-1 , i+1 , m_strCopiedData[i] );
		}

	}


	EFS_END
}

// 2006-03-14 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++
// ���޺���ϱ��к� ������� ���� �׸��� �߰� �̺�Ʈ
VOID CMngLocMgr04Dlg::OnClickgridCLASSREG() 
{
EFS_BEGIN

//	m_pGrid->SetReverse( m_pGrid->GetIdx() );
//	FreeGridEdt();
	m_bFirstReturn = FALSE;
	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_gridPOSITION2);

EFS_END
}

// 2006-03-14 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++
// ���޺���ϱ��к� ������� ���� �׸��� �߰� �̺�Ʈ
VOID CMngLocMgr04Dlg::OnDblClickgridCLASSREG() 
{
EFS_BEGIN

	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_gridPOSITION2);
	INT nCol = m_pGrid->GetCol();
	
	m_bFirstReturn = TRUE;
	InputDataToGrid();
	
	m_pGrid->SetCol( nCol );

EFS_END
}

// 2006-03-14 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++
// ���޺���ϱ��к� ������� ���� �׸��� �߰� �̺�Ʈ
VOID CMngLocMgr04Dlg::OnKeyUpgridCLASSREG(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	EFS_END
}

HBRUSH CMngLocMgr04Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}