	// MNG_SMS_SERVICE_1100.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_SMS_SERVICE_1100.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_SMS_SERVICE_1100 dialog


CMNG_SMS_SERVICE_1100::CMNG_SMS_SERVICE_1100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_SMS_SERVICE_1100)
	//}}AFX_DATA_INIT
	// 2013.11.25 ADD BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (��Ƽ�޺��ڽ�)

	m_strCMAlias = _T("CM_SMS_1100_MultiComboBox");


	m_nCDCnt = 3;
	m_pCWindow =new CMNG_SMS_SERVICE_MIDDLE_DIALOG*[m_nCDCnt];

	for( INT i = 0 ; i < m_nCDCnt ; i++ )
		m_pCWindow[i] = NULL;

}

CMNG_SMS_SERVICE_1100::~CMNG_SMS_SERVICE_1100()
{	
	for( INT i = 0 ; i < m_nCDCnt ; i++ )
		if( m_pCWindow[i] != NULL )
			delete m_pCWindow[i];
		
	delete []m_pCWindow;
}

VOID CMNG_SMS_SERVICE_1100::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_SMS_SERVICE_1100)

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_SMS_SERVICE_1100, CDialog)
	//{{AFX_MSG_MAP(CMNG_SMS_SERVICE_1100)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabTITLE, OnSelchangetabTITLE)
	ON_WM_CTLCOLOR()   
	ON_CBN_SELCHANGE(IDC_SMSTypeCombo, OnSelchangeSMSTypeCombo)// 2013.11.14 ADD BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (�����ü ���� �޺��ڽ� �߰�)
	ON_CBN_SELCHANGE(IDC_COMBO_LMSYN, OnSelchangeComboLmsYN) // 2013.11.14 ADD BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (LMS ��뿩�� �޺��ڽ� �߰�)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_SMS_SERVICE_1100 message handlers

BOOL CMNG_SMS_SERVICE_1100::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = InitESL_Mgr( _T("MNG_SMS_SERVICE_1100") );
	if( ids < 0 )
	{
		AfxMessageBox(_T("InitEsl_Mgr Error"));
	}

	MakeEnvInfo();

	CreateCWindow();

	InitScreen();

	ReView();

	// 2013.11.14 ADD BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (�޺��ڽ� ����)
	SetSMSCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



BOOL CMNG_SMS_SERVICE_1100::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

INT CMNG_SMS_SERVICE_1100::InitScreen()
{
	EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTITLE);
	
	pTab->InsertItem( 0 , _T("���� ����") );
	pTab->InsertItem( 1 , _T("���� ����") );

// 2013.11.14 UPDATE BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (�����ü ����)
	if ( m_strSMSSystemInfo == _T("0") || m_strSMSSystemInfo == _T("1") || m_strSMSSystemInfo == _T("2") || 
		 m_strSMSSystemInfo == _T("3") || m_strSMSSystemInfo == _T("4") || m_strSMSSystemInfo == _T("5") ||
		 m_strSMSSystemInfo == _T("6") || m_strSMSSystemInfo == _T("7") || m_strSMSSystemInfo == _T("8") ||
		 m_strSMSSystemInfo == _T("9")
		 || m_strSMSSystemInfo == _T("10") //JOB.2018.0088 LMS ��ü �ٿ��� �߰�
		 )
	// 2007.07.04 add by pdw
	// �Ｚ��Ʈ���� ���� �� �߰�
//	if ( m_strSMSSystemInfo == _T("0") || m_strSMSSystemInfo == _T("2") || 
//		 m_strSMSSystemInfo == _T("3") || m_strSMSSystemInfo == _T("5"))

	{
		pTab->InsertItem( 2 , _T("�����ü ����") );
	}


	ViewFromTab();

	return 0;

	EFS_END

	return -1;
}

VOID CMNG_SMS_SERVICE_1100::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if( GetSafeHwnd() == NULL ) return ;

	CWnd* pWnd = GetDlgItem(IDC_tabTITLE);
	if( pWnd->GetSafeHwnd() == NULL ) return ;

	pWnd->MoveWindow( 0 , 0 , cx , cy );

	if( m_pCWindow[0] == NULL) return;

	if( m_pCWindow[0]->GetSafeHwnd() == NULL ) return;

	m_pCWindow[0]->MoveWindow( 10 , 30 , cx - 20 , cy - 50 );
		
	if( m_pCWindow[1] == NULL) return;

// 2013.11.14 UPDATE BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (�����ü ���� ��ġ����)

	if( m_pCWindow[1]->GetSafeHwnd() == NULL ) return;
	m_pCWindow[1]->MoveWindow( 10 , 100 , cx - 20 , cy - 50 );

//	if( m_pCWindow[1]->GetSafeHwnd() == NULL ) return;
//	m_pCWindow[1]->MoveWindow( 10 , 30 , cx - 20 , cy - 50 ); 

	if( m_pCWindow[2]->GetSafeHwnd() == NULL ) return;
	m_pCWindow[2]->MoveWindow( 10 , 30 , cx - 20 , cy - 50 );

}

INT CMNG_SMS_SERVICE_1100::CreateCWindow()
{
	EFS_BEGIN

	m_pCWindow[0] = new CMNG_SERVICE_1110(this);
	m_pCWindow[0]->pMain = this->pMain;
	m_pCWindow[0]->pParentMgr = (CESL_Mgr*)this;
	m_pCWindow[0]->m_pInfo = this->m_pInfo;
	m_pCWindow[0]->Create(this);
	m_pCWindow[0]->UpdateWindow();
	m_pCWindow[0]->CenterWindow();
	
	m_pCWindow[1] = new CMNG_SERVICE_1120(this, _ttoi(m_strSMSSystemInfo) );
	m_pCWindow[1]->pMain = this->pMain;
	m_pCWindow[1]->pParentMgr = (CESL_Mgr*)this;
	m_pCWindow[1]->m_pInfo = this->m_pInfo;
	m_pCWindow[1]->Create(this);
	m_pCWindow[1]->UpdateWindow();
	m_pCWindow[1]->CenterWindow();

	m_pCWindow[2] = new CMNG_SERVICE_1130(this);
	m_pCWindow[2]->pMain = this->pMain;
	m_pCWindow[2]->pParentMgr = (CESL_Mgr*)this;
	m_pCWindow[2]->m_pInfo = this->m_pInfo;
	m_pCWindow[2]->Create(this);
	m_pCWindow[2]->UpdateWindow();
	m_pCWindow[2]->CenterWindow();


	return 0;

	EFS_END

	return -1;
}

INT CMNG_SMS_SERVICE_1100::ViewFromTab()
{
	EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTITLE);

	INT nIndex = pTab->GetCurSel();

	m_pCWindow[0]->ShowWindow(SW_HIDE);
	m_pCWindow[1]->ShowWindow(SW_HIDE);
	m_pCWindow[2]->ShowWindow(SW_HIDE);

	// 2013.11.14 UPDATE BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (�����ü�� ���� ���� ���� �����ֱ�)
	// ������������ ��ư Ȱ��ȭ/��Ȱ��ȭ
	if( nIndex == 0 )
	{ 
		m_pCWindow[0]->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SMSTypeCombo)->ShowWindow(SW_HIDE); 
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_LMSYN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LMS_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LMS_HINT)->ShowWindow(SW_HIDE);
	}
	else if( nIndex == 1 )
	{ 
		m_pCWindow[2]->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SMSTypeCombo)->ShowWindow(SW_HIDE); 
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_LMSYN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LMS_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LMS_HINT)->ShowWindow(SW_HIDE);
	}
	else if( nIndex == 2 )
	{ 
		m_pCWindow[1]->ShowWindow(SW_SHOW);	
		GetDlgItem(IDC_SMSTypeCombo)->ShowWindow(SW_SHOW); 
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
		CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_SMSTypeCombo);
		CString strGetType;
		pCMB->GetLBText(pCMB->GetCurSel(), strGetType, 0);		
		// KOL.RED.016 LMS ���� �� ������� ����. ũ�μ��� LMS �����Ѵ�.
		//JOB.2018.0088 LMS ��ü �ٿ��� �߰�
		//if(_T("0") == strGetType )
		if(_T("0") == strGetType || _T("10") == strGetType)
		//if(_T("7") == strGetType || _T("8") == strGetType || _T("9") == strGetType)
		{
			GetDlgItem(IDC_LMS_STATIC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_LMSYN)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_LMS_HINT)->ShowWindow(SW_SHOW);			
		}
	}
	else 
	{
		m_pCWindow[0]->ShowWindow(SW_HIDE);	
		m_pCWindow[1]->ShowWindow(SW_HIDE);	
		m_pCWindow[2]->ShowWindow(SW_HIDE);	
	}


//	if( nIndex == 0 )
//	{
//		m_pCWindow[0]->ShowWindow(SW_SHOW);
//	}
//	else if( nIndex == 2 )
//	{
//		m_pCWindow[1]->ShowWindow(SW_SHOW);	
//	}
//	else if( nIndex == 1 )
//	{
//		m_pCWindow[2]->ShowWindow(SW_SHOW);
//	}

	return 0;

	EFS_END
	
	return -1;
}


VOID CMNG_SMS_SERVICE_1100::OnSelchangetabTITLE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ViewFromTab();
	
	*pResult = 0;
}


INT CMNG_SMS_SERVICE_1100::MakeEnvInfo()
{
	EFS_BEGIN	


	AfxGetApp()->DoWaitCursor(1);

	// 1. ���� ������ ���� ���� ��������

	/// ���� ������ ���� �޶�����.
	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_SMS_TYPE_ENV"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeEnvInfo") );

	CString sVersion , sWhere;
	//++2008.10.22 UPDATE BY PWR {{++
	// ���սý��� ����
	pDM->GetOneValueQuery(  _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
							_T("WHERE CLASS_ALIAS='SMS��������' AND MANAGE_CODE=UDF_MANAGE_CODE"), sVersion );
// 	pDM->GetOneValueQuery( _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = 'SMS��������'") , sVersion );
	//--2008.10.22 UPDATE BY PWR --}}

	if( sVersion.Compare(_T("S")) == 0 )
	{
		sWhere.Format( _T("TYPE BETWEEN 'SMS01' AND 'SMS04' ") );
	}

	// 2007.08.13 add by pdw
	// �߰����⿹�� 
	CString strValue;
	GetManageValue(_T("����"),_T("����"),_T("�����ڷ����"),_T("�߰����⿹��������"),strValue);
	strValue.TrimLeft();strValue.TrimRight();
	strValue.MakeUpper();
	if( _T("Y") == strValue )
	{
	}
	else
	{
		if( !sWhere.IsEmpty() ) sWhere += _T(" AND ");
		sWhere += _T(" TYPE <> 'NRL03' ");
	}
	
	INT ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeEnvInfo") );
	
	// 2. ��ü ���� ���� ��������		
	pDM = FindDM(_T("DM_MNG_SMS_TOTAL_ENV"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeEnvInfo") );

	//++2008.10.22 UPDATE BY PWR {{++
	// ���սý��� ����
	sWhere=	_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
			_T("WHERE CLASS_ALIAS='SMS�ý�������' AND MANAGE_CODE=UDF_MANAGE_CODE");
// 	sWhere=_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='SMS�ý�������'");
	//--2008.10.22 UPDATE BY PWR --}}
	pDM->GetOneValueQuery(sWhere, m_strSMSSystemInfo);

	if ( m_strSMSSystemInfo.IsEmpty() )
	{
		sWhere = _T("");
	}
	else
	{
		sWhere.Format(_T("SMS_TYPE='%s'"), m_strSMSSystemInfo );
	}
	
	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeEnvInfo") );

	AfxGetApp()->DoWaitCursor(-1);

	// 2013.11.20 ADD BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (SMS�����ü�� ���� ��ü����) 
	CESL_DataMgr* pAllDM = FindDM(_T("DM_MNG_SMS_TOTAL_ENV_ALL"));
	if( pAllDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeEnvInfo") );
	ids = pAllDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeEnvInfo") );

	return 0;

	EFS_END

	return -1;
}


// ���� ���� ���� - �ٽ� �б�
INT CMNG_SMS_SERVICE_1100::ReView()
{
	EFS_BEGIN

	AfxGetApp()->DoWaitCursor(1);

	for( INT i = 0 ; i < m_nCDCnt ; i++ )
	{	
		if( m_pCWindow[i] == NULL ) return 0;

		if( m_pCWindow[i]->GetSafeHwnd() == NULL ) return 0;

		m_pCWindow[i]->ReView();
	
	}

	AfxGetApp()->DoWaitCursor(-1);

	return 0;

	EFS_END

	return -1;
}

// ���泻�� �����ϱ�
INT CMNG_SMS_SERVICE_1100::Save()
{
	EFS_BEGIN
	
	AfxGetApp()->DoWaitCursor(1);

	INT ids;

	CESL_DataMgr TypeEnvDM;
	CESL_DataMgr TotalEnvDM;
	
	CLocCommonAPI::MakeCopyDM( this , GetTypeEnvDM() , &TypeEnvDM );
	CLocCommonAPI::MakeCopyDM( this , GetTotalEnvDM() , &TotalEnvDM );
	
	CLocCommonAPI::AddDM( GetTypeEnvDM() , &TypeEnvDM , this );
	CLocCommonAPI::AddDM( GetTotalEnvDM() , &TotalEnvDM , this );
	
	// Temp DM�� �����ϱ�
	
	for( INT i = 0 ; i < m_nCDCnt ; i++ )
	{	
		if( m_pCWindow[i] == NULL ) return 0;
		
		if( m_pCWindow[i]->GetSafeHwnd() == NULL ) return 0;
		
		ids = m_pCWindow[i]->Save( &TypeEnvDM , &TotalEnvDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("Save") );
		if( ids > 0 ) return 0;
	}

	
	AfxGetApp()->DoWaitCursor(-1);

	/// DB�� �����ϱ�
	ids = SaveEnvDM( &TypeEnvDM , &TotalEnvDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("Save") );

	/// DM�� �����ϱ�
	GetTypeEnvDM()->FreeData();
	GetTotalEnvDM()->FreeData();
	CLocCommonAPI::AddDM( &TypeEnvDM , GetTypeEnvDM() , this );
	CLocCommonAPI::AddDM( &TotalEnvDM , GetTotalEnvDM() , this );

	/// �ٽú����ֱ�
	ReView();
	
	AfxMessageBox( _T("����Ǿ����ϴ�.") );

	return 0;

	EFS_END

	return -1;

}




INT CMNG_SMS_SERVICE_1100::SaveEnvDM( CESL_DataMgr* pTypeEnvDM , CESL_DataMgr* pTotalEnvDM )
{
	EFS_BEGIN

	INT ids;
	
	ids = SaveTypeEnvDM( pTypeEnvDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveEnvDM") );

	ids = SaveTotalEnvDM(  pTotalEnvDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveEnvDM") );
	
	return 0;

	EFS_END

	return -1;
}

/// Ÿ�Ժ� ���� ���� �����ϱ�
INT CMNG_SMS_SERVICE_1100::SaveTypeEnvDM( CESL_DataMgr* pTypeEnvDM )
{
	EFS_BEGIN
		
	INT ids;

	pTypeEnvDM->StartFrame();

	CString sTableName = _T("MN_SMS_TYPE_ENV_TBL");

	const INT nFieldCnt = 5;
	CString sDMFieldAlias[nFieldCnt] =
	{
		_T("�ڵ������⿩��") , _T("�⺻����") , _T("����") , _T("Ÿ��") , _T("��������KEY")
	};
	CString sDBFieldName[nFieldCnt] =
	{
		_T("AUTO_SEND_YN") , _T("DEFAULT_MESSAGE") , _T("NAME") , _T("TYPE") , _T("REC_KEY")
	};
	CString sDBFieldType[nFieldCnt] =
	{
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("NUMERIC")
	};
	
	CString sData;

	for( INT i = 0 ; i < pTypeEnvDM->GetRowCount() ; i++ )
	{
		pTypeEnvDM->InitDBFieldData();
		for( INT j = 0 ; j < nFieldCnt -1 ; j++ )
		{
			ids = pTypeEnvDM->GetCellData( sDMFieldAlias[j] , i , sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveTypeEnvDM") );

			pTypeEnvDM->AddDBFieldData( sDBFieldName[j] , sDBFieldType[j] , sData );
		}

		sData = GetWorkLogMsg( _T("SMS ����"), __WFILE__, __LINE__ );
		CLocCommonAPI::GetWorkLogMsg( this , sData , __WFILE__ , __LINE__ );
		pTypeEnvDM->AddDBFieldData( _T("LAST_WORK") , _T("STRING") , sData );

		ids = pTypeEnvDM->GetCellData( sDMFieldAlias[j] , i , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveTypeEnvDM") );

		pTypeEnvDM->MakeUpdateFrame( sTableName , sDBFieldName[j] , sDBFieldType[j] , sData );
	}


	ids = pTypeEnvDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("SaveTypeEnvDM") );

	pTypeEnvDM->EndFrame();
	
	return 0;
	
	EFS_END
		
	return -1;
}


// 2007.08.13 update by pdw
// ������ ȸ�������� �������к��� ���� �ɼ� �ֵ��� �Ѵ�.
INT CMNG_SMS_SERVICE_1100::SaveTotalEnvDM( CESL_DataMgr* pTotalEnvDM )
{
	EFS_BEGIN
		
	INT ids;
	
	pTotalEnvDM->StartFrame();
	
	CString sTableName = _T("MN_SMS_TOTAL_ENV_TBL");
	
	// �ڵ��߼۰����ڵ� �߰� _050308
	// 2009.04.01 ADD BY KSJ : ���� ũ�� �߰�
	//DEL const INT nFieldCnt = 17;
	const INT nFieldCnt = 18;
	CString sDMFieldAlias[nFieldCnt] =
	{
		_T("���ε���̵�") ,		 _T("���ε��н�����") ,		_T("ȸ����ȭ��ȣ") , 
		_T("��������") ,			 _T("�����뺸��˻�����") , _T("�ݳ�����뺸��˻�����") , 
		_T("�����������뿩��") , _T("���ڱ�������") ,		_T("�̿�������IP") , 
		_T("�̿��ڵ�����") ,		 _T("�̿��ھ��̵�") ,		_T("�̿��ڹ�ȣ") ,
		_T("�ݳ����˰�����") ,		 _T("�ݳ���������Ƚ��") ,	_T("�ڵ��߼۰����ڵ�"),
		_T("��Ʈ") ,				 _T("TITLE_SIZE") ,			_T("����ȯ��KEY")
	};
	CString sDBFieldName[nFieldCnt] =
	{
		_T("BIND_ID") ,				_T("BIND_PWD") ,			_T("CALLBACK_PHONE") , 
		_T("LIB_NAME") ,			_T("SENDTIME_DELAY") ,		_T("SENDTIME_RETURN") , 
		_T("SEND_DIVIDE_YN") ,		_T("SMS_MAKE_COMPONENT") ,	_T("USER_ACCESS_IP") , 
		_T("USER_DOMAIN") ,			_T("USER_ID") ,				_T("USER_PHONE_ID") , 
		_T("RETRY_PERIOD_DAY") ,	_T("RETRY_LIMIT_COUNT") ,	_T("NOT_AUTOSEND_MANAGECODE"),
		_T("PORT") ,				_T("TITLE_SIZE") ,			_T("REC_KEY")
	};
	CString sDBFieldType[nFieldCnt] =
	{
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("NUMERIC") ,_T("NUMERIC") , _T("STRING"),
		_T("STRING") , _T("NUMERIC"),  _T("NUMERIC")
	};
	CString sData;
	CString strCallBack, strLibName;
	pTotalEnvDM->InitDBFieldData();
	INT		j;
	for( j=0 ; j<nFieldCnt-1 ; j++ )
	{
		ids = pTotalEnvDM->GetCellData( sDMFieldAlias[j] , 0 , sData );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SaveTotalEnvDM") );
		}
		
		// ȸ�Ź�ȣ�� ���������� ������ �����Ѵ�.
		if( j == 2)
		{
			strCallBack = sData;
			continue;
		}
		else if( j == 3 )
		{
			strLibName = sData;
			continue;
		}

		pTotalEnvDM->AddDBFieldData( sDBFieldName[j] , sDBFieldType[j] , sData );
	}		
	
	sData = GetWorkLogMsg( _T("SMS ����"), __WFILE__, __LINE__ );
	CLocCommonAPI::GetWorkLogMsg( this , sData , __WFILE__ , __LINE__ );
	pTotalEnvDM->AddDBFieldData( _T("LAST_WORK") , _T("STRING") , sData );

	INT		nRowCnt		=	pTotalEnvDM->GetRowCount();
	if ( 0 < nRowCnt )
	{
		ids = pTotalEnvDM->GetCellData( sDMFieldAlias[j] , 0 , sData );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SaveTotalEnvDM") );
		}

		pTotalEnvDM->MakeUpdateFrame( sTableName , sDBFieldName[j] , sDBFieldType[j] , sData );
	}
	else
	{
		CString strRecKey;
		pTotalEnvDM->MakeRecKey(strRecKey);
		pTotalEnvDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC") , strRecKey );
		pTotalEnvDM->MakeInsertFrame( sTableName );
	}

	// 2007.08.13 add by pdw
	// �������к� ȸ������ ����	

	//++2008.10.15 UPDATE BY PWR {{++
	// ���սý��� ����
	CString query;
	query.Format(_T("UPDATE MN_LIBINFO2_TBL SET SMS_CALLBACK_PHONE='%s', SMS_LIB_NAME='%s' ")					  
		         _T("WHERE MANAGE_CODE=UDF_MANAGE_CODE;")
				 , strCallBack, strLibName );
//	CString value,query;
//	query.Format( _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
//		          _T(" WHERE GROUP_2='��Ÿ' AND GROUP_3='����' AND CLASS_ALIAS='�������к�����������'"));
//	ids = pTotalEnvDM->GetOneValueQuery( query, value );	
//	value.TrimLeft();value.TrimRight();
//	value.MakeUpper();
//	// 2. ���� ����
//	if( _T("Y") == value )
//	{		
//		query.Format(_T("UPDATE MN_LIBINFO2_TBL SET SMS_CALLBACK_PHONE='%s', SMS_LIB_NAME='%s' ")					  
//		             _T(" WHERE MANAGE_CODE=(SELECT MANAGE_CODE FROM MN_USER_TBL WHERE REC_KEY=%s);"),
//					 strCallBack,strLibName,m_pInfo->USER_PK);		
//	}
//	else
//	{
//		query.Format(_T("UPDATE MN_LIBINFO2_TBL SET SMS_CALLBACK_PHONE='%s', SMS_LIB_NAME='%s' ")					  
//		             _T(" WHERE MANAGE_CODE IS NULL OR MANAGE_CODE='MA';"),strCallBack,strLibName);		
//	}
	//--2008.10.15 UPDATE BY PWR --}}
		
	pTotalEnvDM->AddFrame( query );

	// 2013.11.20 ADD BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (SMS�����ü����, LMS��뿩�� CM TO DM)
	// SMS�����ü
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_SMSTypeCombo);

	CString strGetType;
	pCMB->GetLBText(pCMB->GetCurSel(), strGetType, 0);
	pTotalEnvDM->SetCellData(_T("SMS_TYPE"), strGetType, 0);
	// SMS�� ��ü �������� ����� �ǹǷ� MANAGE_CODE������ �����Ѵ�.
	query.Format(_T("UPDATE ESL_MANAGE_TBL SET VALUE_NAME='%s', LAST_WORK='%s' WHERE CLASS_ALIAS='SMS�ý�������';"), strGetType, GetWorkLogMsg( _T("�߼۸޽�������"),__WFILE__,__LINE__ ));
	pTotalEnvDM->AddFrame( query );
	m_strSMSSystemInfo = strGetType;
	// KOL.RED.016 LMS ���� �� ������� ����. ũ�μ��� LMS �����Ѵ�.
	//if(_T("0")==strGetType )
	if(_T("0")==strGetType || _T("10")==strGetType ) //JOB.2018.0088
	// LMS��뿩�δ� �߰��� �������׿� ���ؼ��� ���� (7:SK��ε���, 8:KT&DS, 9:LGU+)
	//if(_T("7")==strGetType || _T("8")==strGetType || _T("9")==strGetType) 
	{
		CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LMSYN);
		CString strLMSUseYN;
		pCombo->GetLBText(pCombo->GetCurSel(), strLMSUseYN);
		pTotalEnvDM->SetCellData(_T("LMS��뿩��"), strLMSUseYN, 0);
		CESL_DataMgr *pDM = FindDM(_T("DM_MNG_SMS_TOTAL_ENV_ALL"));
		if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SaveTotalEnvDM") );
		CString strGetData, strRecKey;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("SMS_TYPE"), i, strGetData);
			if(strGetData == strGetType) 
			{
				pDM->SetCellData(_T("LMS��뿩��"), strLMSUseYN, i);
				pDM->GetCellData(_T("����ȯ��KEY"), i, strRecKey);
				break;
			}
		}
		if(!strRecKey.IsEmpty())
		{
			query.Format(_T("UPDATE MN_SMS_TOTAL_ENV_TBL SET LMS_USE_YN='%s' WHERE REC_KEY=%s;"), strLMSUseYN, strRecKey);
			pTotalEnvDM->AddFrame( query );
		}
	}

	ids = pTotalEnvDM->SendFrame();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SaveTotalEnvDM") );
	}
	
	pTotalEnvDM->EndFrame();
	
	return 0;
	
	EFS_END
		
	return -1;
}




/// Ÿ�Ժ� �������� DM
CESL_DataMgr* CMNG_SMS_SERVICE_1100::GetTypeEnvDM()
{
	EFS_BEGIN
		
		
	return FindDM(_T("DM_MNG_SMS_TYPE_ENV"));
	
	EFS_END
		
	return NULL;
}

/// ��ü �������� DM
CESL_DataMgr* CMNG_SMS_SERVICE_1100::GetTotalEnvDM()
{
	EFS_BEGIN
		
	return FindDM(_T("DM_MNG_SMS_TOTAL_ENV"));
	
	EFS_END
		
	return NULL;
}

HBRUSH CMNG_SMS_SERVICE_1100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


// 2013.11.14 ADD BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (�����ü ���� �� �� �ֵ��� ��)
INT CMNG_SMS_SERVICE_1100::SetSMSCombo()
{
	int ids =-1;

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_SMSTypeCombo);
	if(!pCMB)
	{
		return 0;
	}

	const int COL_COUNT = 2;
	pCMB->ResetContent();
	pCMB->FreeComboData();
	pCMB->InitMultiComboBox(COL_COUNT);

	//JOB.2018.0088 LMS ��ü �ٿ��� �߰�
	//const int STATE_COUNT =10;
	const int STATE_COUNT =11;
	CString strCmbData[STATE_COUNT][COL_COUNT] =
	{
		{_T("0"),_T("KTũ�μ�")},
		{_T("1"),_T("ARREO")},
		{_T("2"),_T("I-HEART")},
		{_T("3"),_T("EMMA")},
		{_T("4"),_T("IMO")},
		{_T("5"),_T("SS��Ʈ����")},
		{_T("6"),_T("LG")},
		{_T("7"),_T("SK��ε���")},
		{_T("8"),_T("KT&DS")},
		{_T("9"),_T("LGU+")}
		//JOB.2018.0088 LMS ��ü �ٿ��� �߰�
		,{_T("10"),_T("�ٿ���")}
	};

	INT nIndex = 0;
	for(int nRow=0; nRow <STATE_COUNT; nRow++)
	{
		for(int nCol=0; nCol<COL_COUNT; nCol++)
		{
			pCMB->AddItem(strCmbData[nRow][nCol],nCol,nRow);
			if(0 == nCol)
			{
				if(strCmbData[nRow][nCol]== m_strSMSSystemInfo)
				{
					nIndex = nRow;
				}
			}
		}	
	}
	// ����Ǿ��ִ� ��ü�� �޺��ڽ� ����
	pCMB->SetCurSel(nIndex);
	//LMS ��뿩��
	//JOB.2018.0088 LMS ��ü �ٿ��� �߰�
	//if(7 == nIndex || 8 == nIndex || 9 == nIndex)
	if(7 == nIndex || 8 == nIndex || 9 == nIndex || 10 == nIndex)
	{
		
		m_pCWindow[2]->GetDlgItem(IDC_chkDIVIDEYN)->SetWindowText(_T("���ڰ� 90byte�ʰ��� ������ ������ ���"));
	}
	else
	{
		GetDlgItem(IDC_COMBO_LMSYN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LMS_STATIC)->ShowWindow(SW_HIDE);
		m_pCWindow[2]->GetDlgItem(IDC_chkDIVIDEYN)->SetWindowText(_T("���ڰ� 80byte�ʰ��� ������ ������ ���"));
	}

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LMSYN);
	pCombo->ResetContent();
	pCombo->InsertString( 0, _T("Y") );
	pCombo->InsertString( 1, _T("N") );
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_SMS_TOTAL_ENV"));
	CString strGetData, strLMSUseYN;
	pDM->GetCellData(_T("LMS��뿩��"), 0, strLMSUseYN);
	if(_T("Y") == strLMSUseYN)
	{
		pCombo->SetCurSel(0);
		m_pCWindow[2]->GetDlgItem(IDC_chkDIVIDEYN)->ShowWindow(SW_HIDE);
	}
	else
	{
		pCombo->SetCurSel(1);
		m_pCWindow[2]->GetDlgItem(IDC_chkDIVIDEYN)->ShowWindow(SW_SHOW);
	}

	return 0;
}

// 2013.11.14 ADD BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (�����ü ���� �޺��ڽ� �߰�)
void CMNG_SMS_SERVICE_1100::OnSelchangeSMSTypeCombo() 
{
	// ��ü ���ý� ���ð� �ҷ�����
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_SMSTypeCombo);
	INT nIndex = pCMB->GetCurSel();

	// �ش��ü�� ȭ������ ��
	//  - ȭ��ݾҴٰ�
	delete m_pCWindow[1];
	m_pCWindow[1] = NULL;
	//  - �ٽ� ����
	m_pCWindow[1] = new CMNG_SERVICE_1120(this, nIndex);
	m_pCWindow[1]->pMain = this->pMain;
	m_pCWindow[1]->pParentMgr = (CESL_Mgr*)this;
	m_pCWindow[1]->m_pInfo = this->m_pInfo;
	m_pCWindow[1]->Create(this);
	m_pCWindow[1]->UpdateWindow();
	m_pCWindow[1]->CenterWindow();
	//  - ȭ�� ��
	m_pCWindow[1]->ShowWindow(SW_SHOW);	
	CRect rc;
	this->GetClientRect(rc);
	this->SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
	
	//LMS ��뿩��
	// KOL.RED.016 LMS ���� �� ������� ����. ũ�μ��� LMS �����Ѵ�.
	//JOB.2018.0088 LMS ��ü �ٿ��� �߰�
	//if( 0 == nIndex )
	if( 0 == nIndex || 7 == nIndex || 10 == nIndex)
	//if(7 == nIndex || 8 == nIndex || 9 == nIndex)
	{
		GetDlgItem(IDC_LMS_HINT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_LMSYN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LMS_STATIC)->ShowWindow(SW_SHOW);
		m_pCWindow[2]->GetDlgItem(IDC_chkDIVIDEYN)->SetWindowText(_T("���ڰ� 90byte�ʰ��� ������ ������ ���"));
	}
	else
	{
		GetDlgItem(IDC_LMS_HINT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_LMSYN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LMS_STATIC)->ShowWindow(SW_HIDE);
		m_pCWindow[2]->GetDlgItem(IDC_chkDIVIDEYN)->SetWindowText(_T("���ڰ� 80byte�ʰ��� ������ ������ ���"));
	}

	CString strQuery, strGetData, strLMSUseYN;
	CString strGetType;
	pCMB->GetLBText(pCMB->GetCurSel(), strGetType, 0);
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_SMS_TOTAL_ENV_ALL"));
	for (INT i = 0; i < pDM->GetRowCount(); i++) 
	{
		pDM->GetCellData(_T("SMS_TYPE"), i, strGetData);
		if(strGetData == strGetType)
		{
			pDM->GetCellData(_T("LMS��뿩��"), i, strLMSUseYN);
			break;
		}
	}
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LMSYN);
	if(_T("Y") == strLMSUseYN)
	{
		pCombo->SetCurSel(0);
		m_pCWindow[2]->GetDlgItem(IDC_chkDIVIDEYN)->ShowWindow(SW_HIDE);
	}
	else
	{
		pCombo->SetCurSel(1);
		m_pCWindow[2]->GetDlgItem(IDC_chkDIVIDEYN)->ShowWindow(SW_SHOW);
	}

	pCMB->SetFocus();
}

void CMNG_SMS_SERVICE_1100::OnSelchangeComboLmsYN() 
{
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LMSYN);

	if(pCombo->GetCurSel() == 0)
	{
		m_pCWindow[2]->GetDlgItem(IDC_chkDIVIDEYN)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pCWindow[2]->GetDlgItem(IDC_chkDIVIDEYN)->ShowWindow(SW_SHOW);
	}


}
