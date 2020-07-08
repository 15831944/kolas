// CO_CENTER_USER_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "CO_CENTER_USER_MANAGER_01.h"
#include "CO_CENTER_USER_DETAILVIEWER2.h"


#include "..\..\����\����\�弭����\BL_LOC_USER\BO_LOC_3320.h"

#include "../../../�������/check_dup_api/rs_sql.h"
#include "CO_CENTER_USER_DUP_ID_LIST.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_MANAGER_01 dialog


CCO_CENTER_USER_MANAGER_01::CCO_CENTER_USER_MANAGER_01(CESL_Mgr* pParent,INT nMode)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSearchDlg = NULL;
	m_bStop = TRUE;
	m_bBreak = FALSE;

	m_nMode = nMode;
	
	pRemoteDBMgr = NULL;

	m_pLoanShareManager = NULL;
}

CCO_CENTER_USER_MANAGER_01::~CCO_CENTER_USER_MANAGER_01()
{
	EFS_BEGIN
	
	if( pRemoteDBMgr )
	{
		delete pRemoteDBMgr;
		pRemoteDBMgr = NULL;
	}

	if(NULL != m_pLoanShareManager)
	{
		delete m_pLoanShareManager;
		m_pLoanShareManager = NULL;
	}

	EFS_END
}
VOID CCO_CENTER_USER_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_MANAGER_01)
	DDX_Control(pDX, IDC_prgIMPORT, m_PrgCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_MANAGER_01)
	ON_BN_CLICKED(IDC_btnUSERCENTERSEARCH, OnbtnUSERCENTERSEARCH)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnUSERCENTERIMPORT, OnbtnUSERCENTERIMPORT)
	ON_BN_CLICKED(IDC_btnUSERCENTERDETAILVIEW, OnbtnUSERCENTERDETAILVIEW)
	ON_BN_CLICKED(IDC_bCOPY_USER_INFO, OnbCOPYUSERINFO)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bPRINT, OnbPRINT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_MANAGER_01 message handlers


BOOL CCO_CENTER_USER_MANAGER_01::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
		
		// TODO: Add your specialized code here and/or call the base class
		
		return CDialog::Create(IDD, pParentWnd);
	
	EFS_END
		return FALSE;
	
}

BOOL CCO_CENTER_USER_MANAGER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	if(InitESL_Mgr(_T("CO_CENTER_USER_MANAGER_01")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );

	pRemoteDBMgr = new CRS_SQL( this );	
	InitDefaultDM();

	// 2009.05.11 ADD BY CJY : ���;��ε�� �߰�
	if(NULL == m_pLoanShareManager)
	{
 		m_pLoanShareManager = new CLoanShareManager(this);
	}

	CRect WndPos;
	GetWindowRect(WndPos);
	WndPos.bottom++;

	
	MoveWindow(WndPos);

	// 2010.09.08 ADD BY KSJ : ���յ������� ��뿩��
	// 2011.10.12 UPDATE BY PWR : ���յ������� ������ �������� ��� ����
	if(TRUE == m_pInfo->m_bKLUse)
	{
		m_sIsUnityLoanService = _T("Y");
	}
	else
	{
		m_sIsUnityLoanService = _T("N");
	}
// 	GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), m_sIsUnityLoanService );
// 	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
// 	m_sIsUnityLoanService.MakeUpper();
// 	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");

	// 2010.10.03 ADD BY KSJ : ���յ������� - 
	GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("�ֹι�ȣ���ȼ�����뿩��"), m_strIsKLSecurityCivilNo );
	m_strIsKLSecurityCivilNo.TrimLeft();	m_strIsKLSecurityCivilNo.TrimRight();
	m_strIsKLSecurityCivilNo.MakeUpper();
	if(_T("N")!=m_strIsKLSecurityCivilNo) m_strIsKLSecurityCivilNo = _T("Y");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// �˻�
VOID CCO_CENTER_USER_MANAGER_01::OnbtnUSERCENTERSEARCH() 
{
	EFS_BEGIN
		
	if( m_pSearchDlg == NULL || !::IsWindow(m_pSearchDlg->m_hWnd) )
	{
		m_pSearchDlg = new CCO_CENTER_USER_MANAGER_02(this);
		// 2010.09.08 ADD BY KSJ : ���յ������� ��뿩��
		m_pSearchDlg->m_sIsUnityLoanService = m_sIsUnityLoanService;
		m_pSearchDlg->pMain = this->pMain;
		m_pSearchDlg->pParentMgr = (CESL_Mgr*)this;
		m_pSearchDlg->m_pInfo = this->m_pInfo;
		m_pSearchDlg->Create(this);
	}
	
	if( !m_pSearchDlg->IsWindowVisible() )
	{
		m_pSearchDlg->ShowWindow(SW_SHOW);
		m_pSearchDlg->UpdateWindow();
		m_pSearchDlg->CenterWindow();
	}
	else
	{
		m_pSearchDlg->SetFocus();
	}
	
	EFS_END
	
}

VOID CCO_CENTER_USER_MANAGER_01::OnClose() 
{
	if( !m_bStop )
	{
		m_bBreak = TRUE;
		return ;
	}

	if( m_pSearchDlg != NULL && ::IsWindow(m_pSearchDlg->m_hWnd) )
		delete m_pSearchDlg;
	
	
	CDialog::OnClose();
}


/// �⺻ DM ����
/// �˻� ������ ���������� ���� ������ ������ Get
INT CCO_CENTER_USER_MANAGER_01::InitDefaultDM()
{
	EFS_BEGIN

	
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_LOCAL_LIB_INFO") );
//	pRemoteDBMgr->SetFetchCount( 0, 100 );
//	pRemoteDBMgr->LOAN_Select( pDM, _T(""), _T("DM_CO_LOCAL_LIB_INFO"), 1, 1 );

	return 0;

	EFS_END

	return -1;
}

VOID CCO_CENTER_USER_MANAGER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// �׸��� ��������
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridCENTERUSER);
	
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}
	
	pGrid->MoveWindow( 0 , 35 , cx, cy-55);
	CWnd* pPrg = GetDlgItem(IDC_prgIMPORT);

	CRect WndRect;
	pPrg->GetWindowRect(WndRect);
	pPrg->MoveWindow( 10 , cy-2-WndRect.Height() , WndRect.Width() , WndRect.Height() );
}

VOID CCO_CENTER_USER_MANAGER_01::OnbtnUSERCENTERIMPORT() 
{
	EFS_BEGIN

	INT ids;

	CArray< INT, INT >checkedRow;
	GetCheckedRow( checkedRow );

	INT rowCnt = checkedRow.GetSize();
	if( !rowCnt )
	{
		AfxMessageBox( _T("������ �ڷḦ �����Ͽ� �ֽʽÿ�") );
		return;
	}

	const INT cnt = 28;// 2009.03.23 UPDATE BY CJY : ���������� ���� ���� 27;

	CString alias[ cnt ];
	CString field[ cnt ];
	CString data[  cnt ];

	GetFieldName( field );
	GetFieldAlias( alias );

	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );
	pDM->StartFrame();
	
	CStringArray saRecKey; // �̿��ڻ��� ������Ʈ��
	CStringArray saRecKey2; // ���;��ε��
	CStringArray saUserPicName;
	INT overlapedCnt = 0;
	CString sRecKey,sUserPictureName;
	CString	strName, strHandphone;
	CString strTemp, strDupCnt;	
	
	CString manageValue;
	GetManageValue( _T("����"), _T("����"), _T("���;��ε��ߺ�Ű����"), _T("�ߺ�Ű"), manageValue );

	// 2006.12.08 ADD BY PDJ
	// ������ID, ��й�ȣ ���Կ��� �ɼ����� ó����	
	CString IdImportYN;
	GetManageValue( _T("��Ÿ"),_T("����"),_T("�����ڰ�������"),_T("ID���͹��Լ���"), IdImportYN );

	// ��� �ʱ�ȭ
	m_strArrayID.RemoveAll();
	m_strArrayName.RemoveAll();
	m_strArrayCivilNo.RemoveAll();

	INT	nSucessImportCnt;
	INT	nUpdateImportCnt;
	INT nDupIDImportCnt;

	nSucessImportCnt = 0;
	nUpdateImportCnt = 0;
	nDupIDImportCnt = 0;

	CESL_DataMgr* pDupDM = FindDM( _T("DM_CENTER_USER_DUP_LIST") );
	if ( pDupDM == NULL ) return;

	BOOL bIdDupFlag;	
	for( INT i = 0; i < rowCnt; i++ )
	{
		bIdDupFlag = FALSE;
		pDM->GetCellData( alias, cnt, data, checkedRow.GetAt( i ) );

		// ������� ���Թ޴� ���� �������� ó���Ѵ�.
		data[19] = _T("SYSDATE");

		// 1. �ֹε�Ϲ�ȣ 
		// 2. �ֹε�Ϲ�ȣ, �ڵ�����ȣ 
		// 3. �ֹε�Ϲ�ȣ, �ڵ�����ȣ, �����ڹ�ȣ 
		// 4. �ֹε�Ϲ�ȣ, �ڵ�����ȣ, �����ڹ�ȣ, �̸�
		// data[ 7, 17, 2, 4 ]
		if( IsUserOverlap( pDM, data, manageValue ) )
		{
			overlapedCnt++;
			continue;
		}

		data[ 7 ] = MakeCivilNum( data[ 7 ] );
		// 2010.10.01 ADD BY KSJ : ���յ������� ����
		CString strCryptoCivilNo = _T("");
		if(_T("Y")==m_sIsUnityLoanService)
		{
			this->ESL_DES_Encryption(data[7], strCryptoCivilNo);
			// 2010.10.01 ADD BY KSJ : �ֹι�ȣ �� ����� �����ϵ��� ����
			if(_T("N")!=m_strIsKLSecurityCivilNo) CLocCommonAPI::ChangeSecuriyCivilNo( this , data[7] , m_sIsUnityLoanService);
		}
		
		// ID �ߺ�����
		if ( IdImportYN == _T("Y") )
		{
			// �ߺ����� �� ���Կ��� ����
			strTemp.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE USER_ID='%s'"), data[2] );
			pDM->GetOneValueQuery(strTemp, strDupCnt, FALSE);
			if ( _ttoi(strDupCnt.GetBuffer(0)) > 0 )
			{				
				bIdDupFlag = TRUE;
				pDupDM->InsertRow(-1);
				pDupDM->SetCellData(_T("ID"), data[2], nDupIDImportCnt);
				pDupDM->SetCellData(_T("�̸�"), data[4], nDupIDImportCnt);
				pDupDM->SetCellData(_T("�����ڹ�ȣ"), data[1], nDupIDImportCnt);
				nDupIDImportCnt ++;
			}
		}		
		
		pDM->MakeRecKey( sRecKey );		
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), sRecKey );
		for( INT j = 1; j < cnt; j++ )
		{
			// ID, PASSWORD�� �ɼǿ� ���� ����			
			 // if ( bIdDupFlag == TRUE ) continue;
			if ( (bIdDupFlag == TRUE || IdImportYN != _T("Y")) && ( j == 2 || j == 3 ) ) 
			{	
				continue;
			}			

			if( !data[ j ].IsEmpty() && !field[ j ].IsEmpty() && !alias[ j ].IsEmpty() )
			{
				if(19 != j)
				{
					pDM->AddDBFieldData( field[ j ], _T("STRING"), data[ j ], true );
				}
				else
				{
					pDM->AddDBFieldData( field[ j ], _T("NUMERIC"), data[ j ], true );
				}
			}
		}
		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("���͵�������������ڹ��Թޱ�"), __WFILE__, __LINE__ ), true );
		// IDX�� �ʵ� ����
		// 2005.04.22 ADD BY PDJ
		pDM->GetCellData(_T("�̸�") , checkedRow.GetAt( i ) , strName );
		pDM->GetCellData(_T("�ڵ���") , checkedRow.GetAt( i ) , strHandphone );
		pDM->AddDBFieldData( _T("IDX_USER_NAME"), _T("STRING"), strName );
		pDM->AddDBFieldData( _T("IDX_HANDPHONE"), _T("STRING"), strHandphone );
		pDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );
		pDM->AddDBFieldData( _T("USER_CLASS"), _T("STRING"), _T("0") );

		// 2010.10.01 ADD BY KSJ : ���յ������� ����
		pDM->AddDBFieldData( _T("CRYPTO_CIVIL_NO"), _T("STRING"), strCryptoCivilNo );
		
		pDM->MakeInsertFrame( _T("CO_LOAN_USER_TBL") );

		nSucessImportCnt ++;

		// 2009.05.11 UPDATE BY CJY : ���;��ε��
		saRecKey2.Add(sRecKey);
		
		pDM->GetCellData( _T("�̿��ڻ����̸�") , checkedRow.GetAt( i ) , sUserPictureName );
		if( sUserPictureName.IsEmpty() ) continue;

		saRecKey.Add(sRecKey);
		saUserPicName.Add( sUserPictureName );		
	}
	/*
	INT iCh = m_MakeSearchData->ChangeHanjaToHangul( sTitle );	//���� �ѱ� ��ȯ
	if ( 0 != iCh ) {			
	AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
	return;
	}
	iCh = m_MakeSearchData->GetKeyWord( sTitle );				//����, Ư������ ����
	if ( 0 != iCh ) {			
	AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
	return;
	}
	*/

	pDM->SendFrame();
	pDM->EndFrame();

	// 2009.05.11 ADD BY CJY : �����ڰ��� �߰�
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pInfo->LIB_CODE;	
	
	for( i = 0 ; i < saRecKey2.GetSize() ;i++ )
	{
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = saRecKey2.GetAt(i);
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	}

	/// �̿��� ���� ����
	CString sLocalFileName , sLocalDirName , sServerFileName , sServerDirName;
	sLocalDirName = _T("..\\tunning");
	sServerDirName = _T("CT_MASTER_LOAN_USER_TBL");

	CRS_SQL RS_Sql(this);
	CString sPicExt;
	CFile file;
	BYTE* pBuffer;
	pBuffer = NULL;
	for( i = 0 ; i < saRecKey.GetSize() ;i++ )
	{
		sRecKey = saRecKey.GetAt(i);
		sLocalFileName = saUserPicName.GetAt(i);
		sServerFileName = sLocalFileName;


//		ids = RS_Sql.LOAN_FileDownLoad( sLocalFileName , sLocalDirName+_T("\\")+sLocalFileName , sServerFileName , sServerDirName );
		if( ids != 0 ) continue;

		ids = file.Open(sLocalDirName+_T("\\")+sLocalFileName, CFile::modeRead | CFile::typeBinary);
		if( ids == 0 )
		{
			return ;
		}
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		INT nFileSize = file.GetLength();
		if( nFileSize == 0 )
		{
			file.Close();
			continue;
		}
		pBuffer = (BYTE*)malloc(nFileSize+ 1024);
		file.ReadHuge(pBuffer, nFileSize);
		file.Close();
		

		/// ������ ���������� �޾����ٸ�..
		ids = pDM->LOBUpdate( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey , _T("USER_PICTURE") , pBuffer , nFileSize );
		if( ids != 0 )
		{
			free( pBuffer );
			continue;
		}

		sPicExt = sLocalFileName.Right(3);

		pDM->StartFrame();
		pDM->InitDBFieldData();
		
		pDM->AddDBFieldData( _T("USER_PICTURE_TYPE") , _T("STRING") , sPicExt );
		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("�����̿��ڹ���"), __WFILE__, __LINE__ ), TRUE );
		pDM->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pDM->SendFrame();
		pDM->EndFrame();
		
		free( pBuffer );
	}


	CString msg;

	if ( IdImportYN == _T("Y") )
	{
		CCO_CENTER_USER_DUP_ID_LIST	dlg(this);			
		dlg.SetData(nSucessImportCnt, overlapedCnt, nDupIDImportCnt);
		dlg.DoModal();			
	}
	else if( overlapedCnt )
	{		
		
		msg.Format( _T("���ͷ� ���� ������ ���������� �������Ͽ����ϴ�.\r\n")
					_T("�ߺ��� ����ڰ� %d ������ �����Ͽ����ϴ�."), overlapedCnt );
		AfxMessageBox( msg );
	}
	else
	{
		msg.Format( _T("���ͷ� ���� ������ ���������� �������Ͽ����ϴ�.") );
		AfxMessageBox( msg );
	}
	
	
	EFS_END

	/*
	if( m_nMode == 0 )
		ImportCenterUser();
	else
		ImportCenterUserToScreen();
	*/
}


/// ���� �̿��� ����
INT CCO_CENTER_USER_MANAGER_01::ImportCenterUser()
{
	EFS_BEGIN


	AfxBeginThread( ImportCenterUserThread , this );
	
	return 0;

	EFS_END

	return -1;
}


/// ���� �̿��� ���� �κ�
/// �׸��忡 ���õ� �ڷḦ ����
/// Local�� �����ϴ� �̿������� �˻��Ͽ� �����ϴ� �̿��ڶ�� UPDATE ,
/// Local�� �������� �ʴ� �̿��ڶ�� INSERT ���ش�.
INT CCO_CENTER_USER_MANAGER_01::ImportCenterUserProc()
{
	EFS_BEGIN

	INT ids;

	CESL_Grid* pGrid = GetGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ImportCenterUserProc") );

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("���� ��ų �̿��ڸ� �������ֽʽÿ�.") );
		return 1;
	}

	pGrid->SelectGetHeadPosition();

	m_PrgCtrl.SetRange32( 0 , nSelectCnt );

	// ���м�
	INT nFailCnt;

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		ids = ImportDBProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ImportCenterUserProc") );
		if( ids > 0 ) nFailCnt++;

		pGrid->SelectGetNext();

		m_PrgCtrl.SetPos(i+1);
	}

	return 0;

	EFS_END
	
	return -1;
}


/// ���� DB �۾�
/// Local�� �����ϴ� �̿������� �˻��Ͽ� �����ϴ� �̿��ڶ�� UPDATE ,
/// Local�� �������� �ʴ� �̿��ڶ�� INSERT ���ش�.
/// �ߺ� �˻�� �ֹε�Ϲ�ȣ�� �Ѵ�.
INT CCO_CENTER_USER_MANAGER_01::ImportDBProc( INT nIndex )
{
	EFS_BEGIN

	INT ids;

	CString sTableName = _T("CO_LOAN_USER_TBL");

	// ������ ������ ���ϰ� �ִ� DM
	CESL_DataMgr* pSrcDM = FindDM(_T("DM_CO_CENTER_USER_MANAGER_01"));
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ImportDBProc") );

	// 2006-03-29 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ������ID, ��й�ȣ ���� ��Ŵ	

	const INT nCnt = 25;
	TCHAR* sDMColumn[nCnt] =
	{
		_T("����_�ּ�") ,     _T("�ٹ���_�ּ�") ,   _T("����") ,            _T("�ֹε�Ϲ�ȣ") ,               _T("��") ,
		_T("�ٹ���_���޸�") , _T("�ٹ���_�μ���") , _T("E_MAIL") ,          _T("�г�") ,                       _T("�ڵ���") , 
		_T("����߼�����") ,  _T("�̸�") ,          _T("����_��ȭ��ȣ") ,   _T("�ٹ���_��ȭ��ȣ") ,            _T("�б�") , 
		_T("�����ڹ�ȣ") ,    _T("����_�����ȣ") , _T("�ٹ���_�����ȣ") , _T("����_TYPE")	,                  _T("�����") , 
		_T("ȸ������") ,      _T("����Ҽ�����") ,  _T("������������") ,    _T("��������������������������") , _T("��߱�Ƚ��")
	};

	TCHAR* sFieldType[nCnt] =
	{
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,	_T("STRING") , 
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("NUMERIC") 
	};

	CString sDBFieldName[nCnt] =
	{
		_T("H_ADDR1") ,   _T("W_ADDR1") ,            _T("BIRTHDAY") ,             _T("CIVIL_NO") ,         _T("CLASS") ,
		_T("CLASS_NAME") ,     _T("DEPARTMENT_NAME") ,         _T("E_MAIL") ,               _T("GRADE") ,            _T("HANDPHONE") ,
		_T("MAIL_SEND_AREA") , _T("NAME") ,                    _T("H_PHONE") ,           _T("W_PHONE") ,       _T("SCHOOL") ,
		_T("USER_NO") ,        _T("H_ZIPCODE") ,           _T("W_ZIPCODE") ,        _T("BIRTHDAY_TYPE") ,    _T("REG_DATE") , 
		_T("MEMBER_CLASS") ,   _T("USER_POSITION_CODE") , _T("USER_CLASS_CODE") , _T("LAST_MODIFY_DATE") , _T("REISSUE_CNT")
	};
	// ---------------------------------------------------------------------------------------------------------	
	
	/*
	INT nImportCnt;
	if ( IdImportYN == _T("Y") ) nImportCnt = 28; 
	else nImportCnt = 26; 
	*/

	CString sImportData[28];

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = pSrcDM->GetCellData( sDMColumn[i] , nIndex , sImportData[i] );
		if( ids < 0 )
		{
			CString sEMsg;
			sEMsg.Format( _T("sDMColumn : %s") , sDMColumn[i] );
			AfxMessageBox( sEMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ImportDBProc") );
	}

	// 1. �ߺ� �˻�
	// 1.1 �ֹε�Ϲ�ȣ �ߺ��˻�
	CESL_DataMgr* pLocalDM = FindDM( _T("DM_CO_LOCAL"));
	if( pLocalDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ImportDBProc") );

	CString sCivilNo = sImportData[3];
	CString sSql;
	CString sValue;
	CString sValue2;
	sSql.Format( _T("SELECT COUNT(*) FROM CO_LOAN_USER_TBL WHERE CIVIL_NO = '%s'") , sCivilNo );

	ids = pLocalDM->GetOneValueQuery( sSql , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ImportDBProc") );

	/*
	// 1.2 ID �ߺ��˻�
	sSql.Format( _T("SELECT COUNT(*) FROM CO_LOAN_USER_TBL WHERE USER_ID = '%s'") , sImportData[27] );
	ids = pLocalDM->GetOneValueQuery( sSql , sValue2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ImportDBProc") );	
	if ( _ttoi(sValue2) > 0 ) 		
	{
		m_strArrayID.Add(sImportData[27]);
		m_strArrayName.Add(sImportData[11]);		
		m_strArrayCivilNo.Add(sImportData[3]);

		nImportCnt = 26;
	}
	*/

	// 2. ����
	pLocalDM->StartFrame();
	pLocalDM->InitDBFieldData();
	
	for( i = 0 ; i < nCnt ; i++ )
	{
		pLocalDM->AddDBFieldData( sDBFieldName[i] , sFieldType[i] , sImportData[i] , TRUE );
	}

	pLocalDM->AddDBFieldData( _T("CENTER_UPLOAD_CHECK") , _T("STRING") , _T("N") , TRUE );

	CString sRecKey;
	if( _ttoi(sValue) == 0 )
	{
		pLocalDM->MakeRecKey( sRecKey );
		pLocalDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pLocalDM->AddDBFieldData( _T("USER_CLASS") , _T("STRING") , _T("0") );
		pLocalDM->MakeInsertFrame( sTableName );
	}
	else
	{
		sSql.Format( _T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE CIVIL_NO = '%s'") , sCivilNo );
		ids = pLocalDM->GetOneValueQuery( sSql , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ImportDBProc") );

		pLocalDM->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	}

	pLocalDM->SendFrame();
	pLocalDM->EndFrame();

	return 0;

	EFS_END

	return -1;

}


// �׸��� ������ Get
CESL_Grid* CCO_CENTER_USER_MANAGER_01::GetGridPtr()
{
	EFS_BEGIN

	CESL_Grid* pGrid;

	CString sCMAlias = _T("CM_CO_CENTER_USER_MANAGER_01");
	CString sGridAlias = _T("�����̿��������׸���");

	CESL_ControlMgr* pCM = FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;
	
	pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);

	return pGrid;

	EFS_END

	return NULL;
}

/// �̿��� ���� : ȭ�鿡 �ݿ�
/// ���� ���밡��
INT CCO_CENTER_USER_MANAGER_01::ImportCenterUserToScreen()
{
	EFS_BEGIN

	INT ids;

	CESL_Grid* pGrid = GetGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ImportCenterUserProc") );
	
	pGrid->SelectMakeList();
	
	INT nSelectCnt = pGrid->SelectGetCount();
	
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("���� ��ų �̿��ڸ� �������ֽʽÿ�.") );
		return 1;
	}
	
	if( nSelectCnt > 1 )
	{
		AfxMessageBox( _T("�Ѹ� �����Ͻʽÿ�") );
		return 2;		
	}
	
	INT nIndex = pGrid->SelectGetIdx();

	// DM���� �� ��������
	// ������ ������ ���ϰ� �ִ� DM
	CESL_DataMgr* pSrcDM = FindDM(_T("DM_CO_CENTER_USER_MANAGER_01"));
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ImportCenterUserToScreen") );
	
	// ������ �ʵ� Get
	const INT nCnt = 27;
	TCHAR* sDMColumn[nCnt] =
	{
		_T("�����ڹ�ȣ") , _T("����_�ּ�") , _T("�ٹ���_�ּ�") , _T("�ֹε�Ϲ�ȣ") , _T("����")  ,		//4
		_T("��") ,_T("�ٹ���_���޸�") , _T("�ٹ���_�μ���") , _T("E_MAIL") , _T("�г�") ,				//9
		_T("�ڵ���") , _T("����߼�����") , _T("�̸�") , _T("����_��ȭ��ȣ") , _T("�ٹ���_��ȭ��ȣ") ,	//14
		_T("�б�") , _T("����_�����ȣ") , _T("�ٹ���_�����ȣ") ,  _T("����_TYPE")	,_T("������ID") ,	//19
		_T("��й�ȣ") , _T("�����") , _T("ȸ������") , _T("����Ҽ�����") , _T("������������") ,		//24
		_T("��������������������������") , _T("��߱�Ƚ��")						//26
	};

	CString sImportData[nCnt];
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = pSrcDM->GetCellData( sDMColumn[i] , nIndex , sImportData[i] );
		if( ids < 0 )
		{
			CString sEMsg;
			sEMsg.Format( _T("sDMColumn : %s") , sDMColumn[i] );
			AfxMessageBox( sEMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ImportCenterUserToScreen") );
	}
	
	// 2006-03-29 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �����ID, ��й�ȣ ����
	CString sControlAlias[nCnt] = 
	{
		_T("�����ڹ�ȣ") , _T("�����ּ�") , _T("�ٹ����ּ�")  , _T("�ֹε�Ϲ�ȣ") , _T("����"), 
		_T("��") , _T("���޸�") , _T("�μ���") , _T("") , _T("�г�") , 
		_T("") , _T("") , _T("�̸�") , _T("������ȭ") , _T("�ٹ�����ȭ") ,
		_T("�б���") ,  _T("���ÿ����ȣ") , _T("�ٹ��������ȣ") ,  _T("")	,_T("") , 
		_T("") , _T("�������") , _T("") , _T("�Ҽ�") , _T("����") , 
		_T("��������") , _T("�߱�Ƚ��")
	};
	// ---------------------------------------------------------------------------------------------------------
	CESL_ControlMgr* pClinetCM = FindCM(_T("CM_BO_LOC_3320"));
	if( pClinetCM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( - 3 , _T("ImportCenterUserToScreen") );

	for( i = 0 ; i < nCnt ; i++ )
	{
		if( sControlAlias[i].IsEmpty() ) continue;

		ids = pClinetCM->SetControlMgrData( sControlAlias[i] , sImportData[i] );
		if( ids < 0 )
		{
			CString sEMsg;
			sEMsg.Format( _T("sControlAlias : %s") , sControlAlias[i] );
			AfxMessageBox( sEMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ImportCenterUserToScreen") );
	}


	// SetControlMgrData�� �ȸԴ� ��Ʈ��
	// ��й�ȣȮ�� , ����TYPE ,����߼����� , ȸ������ , �޴��� , E_MAIL 

	// ��й�ȣ Ȯ��
	ids = pClinetCM->SetControlMgrData( _T("��й�ȣȮ��") , sImportData[20] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ImportCenterUserToScreen") );


	// ���� TYPE
	CESL_Control_Element* pCE;
	
	pCE = (CESL_Control_Element*)pClinetCM->FindControlElement(_T("����TYPE"));
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ImportCenterUserToScreen") );
	CComboBox* pCombo = (CComboBox*)(pParentMgr->GetDlgItem(pCE->CTRL_ID));
	CString sStr;
	for( i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sStr);
		if( sStr.Compare(sImportData[18]) == 0 )
			pCombo->SetCurSel(i);
	}
	
	// ����߼�����
	pCE = (CESL_Control_Element*)pClinetCM->FindControlElement(_T("����߼�����"));
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ImportCenterUserToScreen") );
	
	pCombo = (CComboBox*)(pParentMgr->GetDlgItem(pCE->CTRL_ID));
	if( sImportData[11].Compare(_T("H"))==0 ) sImportData[11] = _T("����");
	else sImportData[11] = _T("�ٹ���");

	for( i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sStr);
		if( sStr.Compare(sImportData[11]) == 0 )
			pCombo->SetCurSel(i);
	}
	
	// ȸ������

	pCE = (CESL_Control_Element*)pClinetCM->FindControlElement(_T("ȸ������"));
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ImportCenterUserToScreen") );
	
	pCombo = (CComboBox*)(pParentMgr->GetDlgItem(pCE->CTRL_ID));
	if( sImportData[22].Compare(_T("0"))==0 ) sImportData[22] = _T("��ȸ��");
	else sImportData[22] = _T("��ȸ��");
	
	for( i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sStr);
		if( sStr.Compare(sImportData[22]) == 0 )
			pCombo->SetCurSel(i);
	}
	
	// �޴���
	CString sHandPhone = sImportData[10];
	CStringArray saHandPhone;

	CLocCommonAPI::MakeArrayList( sHandPhone , _T("-") , saHandPhone );

	if( saHandPhone.GetSize() == 3 )
	{
		ids = pClinetCM->SetControlMgrData( _T("�޴���2") , saHandPhone.GetAt(1) );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ImportCenterUserToScreen") );
		ids = pClinetCM->SetControlMgrData( _T("�޴���3") , saHandPhone.GetAt(2) );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ImportCenterUserToScreen") );
		
		pCE = (CESL_Control_Element*)pClinetCM->FindControlElement(_T("�޴���1"));
		if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ImportCenterUserToScreen") );
		pCombo = (CComboBox*)(pParentMgr->GetDlgItem(pCE->CTRL_ID));
		for( INT i = 0 ; i < pCombo->GetCount() ; i++ )
		{
			pCombo->GetLBText(i,sStr);
			if( sStr.Compare(saHandPhone.GetAt(0)) == 0 )
				pCombo->SetCurSel(i);
		}
	}

	// e_mail
	CString sEmail = sImportData[8];
	CStringArray saEmail;

	CLocCommonAPI::MakeArrayList( sEmail , _T("@") , saEmail );
	
	if( saEmail.GetSize() == 2 )
	{
		ids = pClinetCM->SetControlMgrData( _T("EMAIL1") , saEmail.GetAt(0) );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ImportCenterUserToScreen") );
		
		pCE = (CESL_Control_Element*)pClinetCM->FindControlElement(_T("EMAIL2"));
		if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ImportCenterUserToScreen") );
		pCombo = (CComboBox*)(pParentMgr->GetDlgItem(pCE->CTRL_ID));
		bool bSearch=FALSE;
		for( INT i = 0 ; i < pCombo->GetCount() ; i++ )
		{
			pCombo->GetLBText(i,sStr);
			if( sStr.Compare(saEmail.GetAt(1)) == 0 )
			{
				pCombo->SetCurSel(i);
				bSearch = TRUE;
			}
			if( !bSearch )
			{
				pCombo->AddString( saEmail.GetAt(1) );
				pCombo->SetWindowText( saEmail.GetAt(1) );
			}
		}
	}
	

	// ���������� ����Ǹ� ȭ�鿡 �ݴ´�.
	OnCancel();

	return 0;

	EFS_END

	return -1;
}

// ������ �Լ�
UINT ImportCenterUserThread(LPVOID lParam)
{
	EFS_BEGIN
		
		INT ids;
	CString sErrorMsg;
	
	CCO_CENTER_USER_MANAGER_01 *pObj = (CCO_CENTER_USER_MANAGER_01 *)lParam;
	if ( pObj == NULL )
		return -1;
	
	pObj->m_bStop = FALSE;
	pObj->m_bBreak = FALSE;
	
	ids = pObj->ImportCenterUserProc();
	if (ids == 100 )
	{
		pObj->m_bStopState = 1;
		sErrorMsg = _T("�����Ǿ����ϴ�.");
	}
	else if (ids < 0)
	{
		pObj->m_bStopState = 1;
		sErrorMsg.Format(_T("���� �߻�-%d"),ids );
	}
	else if (ids == 0)
	{
		pObj->m_bStopState = 0;
		sErrorMsg = _T("�Ϸ�Ǿ����ϴ�.");
	}
	
	pObj->m_bStop = TRUE;

	AfxMessageBox( sErrorMsg );
	
	return 0;
	
	EFS_END
		
		return -1;
}


// �󼼺���
VOID CCO_CENTER_USER_MANAGER_01::OnbtnUSERCENTERDETAILVIEW() 
{
	EFS_BEGIN

/*
	CArray< INT, INT >checkedRow;
	GetCheckedRow( checkedRow );

	INT cnt = checkedRow.GetSize();
	if( !cnt )
	{
		AfxMessageBox( _T("�󼼺����� �ڷḦ �����Ͽ� �ֽʽÿ�") );
		return;
	}

	CCO_CENTER_USER_DetailViewer dlg( this );
	dlg.SetRowIndex( checkedRow );
	dlg.DoModal();
*/	
	/*
	CCO_CENTER_USER_MANAGE_03 DetailView(this);
	DetailView.DoModal();
	*/	

	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );
	if( pDM->GetRowCount() < 1 ) return ;


	CString sCMAlias = _T("CM_CO_CENTER_USER_MANAGER_01");
	CString sGridAlias = _T("�����̿��������׸���");


	CCO_CENTER_USER_DETAILVIEWER2 DetailView( this , sCMAlias , sGridAlias);
	// 2010.09.08 ADD BY KSJ : ���յ������� ��뿩��
	DetailView.m_sIsUnityLoanService = m_sIsUnityLoanService;
	DetailView.DoModal();


	EFS_END

}

VOID CCO_CENTER_USER_MANAGER_01::GetCheckedRow( CArray< INT, INT >&checkedRow )
{
	EFS_BEGIN

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridCENTERUSER );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
		if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
			checkedRow.Add( i - 1 );

	EFS_END
}



/*
[ DM_CO_CENTER_USER_MANAGER_01_USER_LIST alias ]
  
���ڵ�KEY
����������
������
��������������
����������
�����ڹ�ȣ
������ID
��й�ȣ
�̸�
�̸�����
�������
������
�ֹε�Ϲ�ȣ
�ּ�_��
�ּ�_�ٹ���
�����ȣ_��
�����ȣ_�ٹ���
��ȭ��ȣ_��
��ȭ��ȣ_�ٹ���
������ȣ
�ٹ����μ���
�ٹ������޸�
�ڵ���
�ڵ�������
E-MAIL
�����
������
�б�
�г�
��
����
����߼�����
���
�ڷ���ڵ�
�̿��ڻ����̸�
ȸ�����нǿ���
�����۾�
�������۾�
*/

VOID CCO_CENTER_USER_MANAGER_01::GetFieldName( CString field[] )
{
	EFS_BEGIN
	
	// 2006-03-29 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �����ID, ��й�ȣ ����
	field[  0 ] = _T("REC_KEY");
	field[  1 ] = _T("USER_NO");
	field[  2 ] = _T("USER_ID");
	field[  3 ] = _T("PASSWORD");
	field[  4 ] = _T("NAME");
	field[  5 ] = _T("BIRTHDAY");
	field[  6 ] = _T("BIRTHDAY_TYPE");
	field[  7 ] = _T("CIVIL_NO");
	field[  8 ] = _T("H_ADDR1");
	field[  9 ] = _T("W_ADDR1");
	field[ 10 ] = _T("H_ZIPCODE");
	field[ 11 ] = _T("W_ZIPCODE");
	field[ 12 ] = _T("H_PHONE");
	field[ 13 ] = _T("W_PHONE");
	field[ 14 ] = _T("EXCHANGE_NO");
	field[ 15 ] = _T("DEPARTMENT_NAME");
	field[ 16 ] = _T("CLASS_NAME");
	field[ 17 ] = _T("HANDPHONE");
	field[ 18 ] = _T("E_MAIL");
	field[ 19 ] = _T("REG_DATE");
	field[ 20 ] = _T("REMOVE_DATE");
	field[ 21 ] = _T("SCHOOL");
	field[ 22 ] = _T("GRADE");
	field[ 23 ] = _T("CLASS");
	field[ 24 ] = _T("CLASS_NO");
	field[ 25 ] = _T("MAIL_SEND_AREA");
//	field[ 26 ] = _T("REMARK");
	field[ 26 ] = _T("CARD_PASSWORD");
	// 2009.03.23 ADD BY CJY : ����߰�
	field[ 27 ] = _T("REMARK");
	//field[ 27 ] = _T("SHELF_LOC_CODE");

	EFS_END
}


VOID CCO_CENTER_USER_MANAGER_01::GetFieldAlias( CString alias[] )
{
	EFS_BEGIN

	// 2006-03-29 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �����ID, ��й�ȣ ����
	alias[  0 ] = _T("���ڵ�KEY");
	alias[  1 ] = _T("�����ڹ�ȣ");
	alias[  2 ] = _T("������ID");
	alias[  3 ] = _T("��й�ȣ");
	alias[  4 ] = _T("�̸�");
	alias[  5 ] = _T("�������");
	alias[  6 ] = _T("������");
	alias[  7 ] = _T("�ֹε�Ϲ�ȣ");
	alias[  8 ] = _T("�ּ�_��");
	alias[  9 ] = _T("�ּ�_�ٹ���");
	alias[ 10 ] = _T("�����ȣ_��");
	alias[ 11 ] = _T("�����ȣ_�ٹ���");
	alias[ 12 ] = _T("��ȭ��ȣ_��");
	alias[ 13 ] = _T("��ȭ��ȣ_�ٹ���");
	alias[ 14 ] = _T("������ȣ");
	alias[ 15 ] = _T("�ٹ����μ���");
	alias[ 16 ] = _T("�ٹ������޸�");
	alias[ 17 ] = _T("�ڵ���");
	alias[ 18 ] = _T("E-MAIL");
	alias[ 19 ] = _T("�����");
	alias[ 20 ] = _T("������");
	alias[ 21 ] = _T("�б�");
	alias[ 22 ] = _T("�г�");
	alias[ 23 ] = _T("��");
	alias[ 24 ] = _T("����");
	alias[ 25 ] = _T("����߼�����");
//	alias[ 26 ] = _T("���");
	alias[ 26 ] = _T("ȸ������й�ȣ");
	//alias[ 27 ] = _T("�ڷ���ڵ�");
	// 2009.03.23 ADD BY CJY : ����߰�
	alias[ 27 ] = _T("���");

	EFS_END
}


INT CCO_CENTER_USER_MANAGER_01::IsUserOverlap( CESL_DataMgr *pDM, CString data[], CString manageValue )
{
	EFS_BEGIN

	// 1. �ֹε�Ϲ�ȣ 
	// 2. �ֹε�Ϲ�ȣ, �ڵ�����ȣ 
	// 3. �ֹε�Ϲ�ȣ, �ڵ�����ȣ, �����ڹ�ȣ 
	// 4. �ֹε�Ϲ�ȣ, �ڵ�����ȣ, �����ڹ�ȣ, �̸�
	// data[ 7, 17, 2, 4 ]
	
	const INT fieldCnt = 28;
	CString field[ fieldCnt ];
	GetFieldName( field );

	const INT cnt = 4;
	// 2006-03-29 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �����ڹ�ȣ �ε����� Ʋ�Ǵ�
	INT fieldNum[ cnt ] = {
		7, 17, 1, 4 };
	
	bool continueFlag[ cnt ];
	for( INT i = 0; i < cnt; i++ )
		if( _T("T") == manageValue.Mid( i, 1 ) )
			continueFlag[ i ] = false;
		else
			continueFlag[ i ] = true;
		
	CString tmp, where;
	for( i = 0; i < cnt; i++ )
		if( data[ fieldNum[ i ] ].IsEmpty() || continueFlag[ i ] )
			continue;
		else if( 7 == fieldNum[ i ] )
		{
			// 2011.10.12 UPDATE BY PWR : ���յ������� ����
			if(_T("Y")==m_sIsUnityLoanService)
			{
				CString strCryptoCivilNo;
				this->ESL_DES_Encryption(data[7], strCryptoCivilNo);
				tmp.Format( _T(" CRYPTO_CIVIL_NO='%s' AND"), strCryptoCivilNo);
			}
			else
			{
				data[ 7 ].Replace( _T("-"), _T("") );
				tmp.Format( _T(" %s IN( '%s', '%s-%s' ) AND"), 
														field[ 7 ], 
														data[ 7 ], 
														data[ 7 ].Left( 6 ),
														data[ 7 ].Right( 7 ) );
			}
			where += tmp;
		}
		else
		{
			tmp.Format(  _T(" %s = '%s' AND"), field[ fieldNum[ i ] ], data[ fieldNum[ i ] ] );
			where += tmp;
		}
	if( _T("AND") == where.Right( 3 ) )
	{
		tmp = where.Left( where.GetLength() - 3 );
		where = tmp;
	}

	CString query, result;
	query.Format( _T("SELECT COUNT( * ) FROM CO_LOAN_USER_TBL WHERE %s"), where );
	pDM->GetOneValueQuery( query, result );

	return _ttoi( result );	

	EFS_END

	return -1;
}

VOID CCO_CENTER_USER_MANAGER_01::SetModeProperty()
{
	EFS_BEGIN

	const INT cnt = 2;
	UINT id[ cnt ] = {
		IDC_btnUSERCENTERIMPORT, IDC_bCOPY_USER_INFO };
	bool flag[ cnt ] = {
		true, true };	

	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridCENTERUSER);
	switch( m_mode )
	{
		case 1 :	// ���Ϳ��� ���Թ�����
			flag[ 1 ] = false;
			break;
		case 2 :	// ����� �Է½� ���Ϳ� �ִ°� ��������
			//pGrid->SetColWidth( 1, 0, 0 );
			flag[ 0 ] = false;
			break;
	}

	CButton *pBtn = NULL;
	for( INT i = 0; i < cnt; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( id[ i ] );
		pBtn->ShowWindow( flag[ i ] );		
	}

	EFS_END
		
}


// �θ����� ���ֱ�. 	
VOID CCO_CENTER_USER_MANAGER_01::OnbCOPYUSERINFO() 
{
	EFS_BEGIN
	// TODO: Add your control notification handler code here
	

	SetUserInfoToParent();

	EFS_END
}

VOID CCO_CENTER_USER_MANAGER_01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	SetModeProperty();

	if( bShow )
		SearchUserByCivilNum();

	EFS_END
}

VOID CCO_CENTER_USER_MANAGER_01::SetUserInfoToParent()
{
	EFS_BEGIN

	if( 2 != m_mode )
		return;
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridCENTERUSER );
	INT row = pGrid->GetRow() - 1;
	if( row < 0 )
		return;
	
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );
	if( !pDM->GetRowCount() )
		return;

	const INT cnt = 28;// 2009.03.23 UPDATE BY CJY : ���������� ���� ���� 27;
	CString alias[ cnt ];
	GetFieldAlias( alias );
	CString data[ cnt  ];
	pDM->GetCellData( alias, cnt, data, row );

	SetCenterUserInfo( data );

/*
	CBO_LOC_3320 *pParent = ( CBO_LOC_3320 * )pParentMgr;
	pParent->SetCenterUserInfo( data );	
*/
	// 2009.03.24 ADD BY CJY : �̿��ڹ��� ���� �� â �ݵ��� ����
	PostMessage(WM_CLOSE, NULL, NULL);
// 	OnClose();

	EFS_END

}

BEGIN_EVENTSINK_MAP(CCO_CENTER_USER_MANAGER_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CCO_CENTER_USER_MANAGER_01)
	ON_EVENT(CCO_CENTER_USER_MANAGER_01, IDC_gridCENTERUSER, -601 /* DblClick */, OnDblClickgridCENTERUSER, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CCO_CENTER_USER_MANAGER_01::OnDblClickgridCENTERUSER() 
{
	EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	// 2009.03.19 UPDATE BY CJY : �׸��� ����Ŭ���� ����� �ƴ� ��쿡�� �����ϵ��� ����
// 	if( 2 == m_mode )
// 		SetUserInfoToParent();
 	if( 2 == m_mode )
	{
		CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridCENTERUSER);
		if(NULL != pGrid)
		{
			int nRow = (int)pGrid->GetMouseRow();
			if(0 < nRow)
			{
				SetUserInfoToParent();
			}
		} 		
	}

	EFS_END
}


INT CCO_CENTER_USER_MANAGER_01::SetCenterUserInfo( CString data[] )
{
EFS_BEGIN

	CESL_ControlMgr* pClinetCM = FindCM(_T("CM_BO_LOC_3320"));
	if( pClinetCM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( - 3 , _T("ImportCenterUserToScreen") );
	
	const INT cnt = 28;// 2009.03.23 UPDATE BY CJY : ���������� ���� ���� 27;
	// 2006-03-29 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �����ID, ��й�ȣ ����
	CString ctrlAlias[ cnt ] = {
		_T(""),
		_T("�����ڹ�ȣ"), 
		_T(""),
		_T(""),
		_T("�̸�"),
		_T("����"),
		_T(""),
		_T("�ֹε�Ϲ�ȣ"),
		_T("�����ּ�"),
		_T("�ٹ����ּ�"),
		_T("���ÿ����ȣ"),
		_T("�ٹ��������ȣ"),
		_T("������ȭ"),
		_T("�ٹ�����ȭ"),
		_T("�ٹ���������ȣ"),
		_T("�μ���"),
		_T("���޸�"),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T("�б���"),
		_T("�г�"),
		_T("��"),
		_T("����/����/����"),
		_T("����߼�����"),		
//		_T("�ڷ��") ,
		_T("ȸ������й�ȣ"),
		_T("���")
	};
	// ---------------------------------------------------------------------------------------------------------	

	// 2010.04.08 UPDATE BY PWR : ���� ���Խ� �����ڹ�ȣ ��Ŀ���̵��� �ߺ�üũ���� �����߻� ����
	CBO_LOC_3320 *pParent = ( CBO_LOC_3320 * )pParentMgr;
	BOOL bCheck = pParent->m_bCivilNoAutoCheck;
	pParent->m_bCivilNoAutoCheck = FALSE;
// 	for( INT i = 0; i < cnt; i++ )
// 		if( !ctrlAlias[ i ].IsEmpty() )
// 			SetControlData( _T("CM_BO_LOC_3320") , ctrlAlias[ i ] , data[ i ] );
	for( INT i = 0; i < cnt; i++ )
	{
		if( FALSE == ctrlAlias[ i ].IsEmpty())
		{
			SetControlData( _T("CM_BO_LOC_3320") , ctrlAlias[ i ] , data[ i ] );
		}
	}
	pParent->m_bCivilNoAutoCheck = bCheck;

	// 2006-03-29 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �����ID, ��й�ȣ ����
	// 2007.02.13 UPDATE BY PDJ
	// �ɼ�ó���� ����
	CString IdImportYN;
	GetManageValue( _T("��Ÿ"),_T("����"),_T("�����ڰ�������"),_T("ID���͹��Լ���"), IdImportYN );
	if ( IdImportYN == _T("Y") )
	{
		// 2010.10.20 ADD BY KSJ :  �����ID ���� ��� �����ID ���� �� �ϵ��� ����
		//DEL SetControlData( _T("CM_BO_LOC_3320"), _T("�����ID"), data[ 2 ] );
		if(!data[ 2 ].IsEmpty())
		{
			SetControlData( _T("CM_BO_LOC_3320"), _T("�����ID"), data[ 2 ] );
		}

		SetControlData( _T("CM_BO_LOC_3320"), _T("��й�ȣ"), data[ 3 ] );
		SetControlData( _T("CM_BO_LOC_3320"), _T("��й�ȣȮ��"), data[ 3 ] );
	}
	

	/*			
	CComboBox *pCombo = NULL;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbUSER8 );
	*/
	CESL_Control_Element* pCE;
	
	pCE = (CESL_Control_Element*)pClinetCM->FindControlElement(_T("����TYPE"));
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ImportCenterUserToScreen") );
	CComboBox* pCombo = (CComboBox*)(pParentMgr->GetDlgItem(pCE->CTRL_ID));
	
	if( _T("+") == data[ 6 ] )
		pCombo->SetCurSel( 0 );
	else
		pCombo->SetCurSel( 1 );
/*	
	CString phoneN_1, phoneN_2, phoneN_3;

	data[ 17 ].Replace( _T("-"), _T("") );
	INT phoneNumLength = data[ 17 ].GetLength();
	if( 10 == phoneNumLength || 11 == phoneNumLength )
	{
		phoneN_1 = data[ 17 ].Left( 3 );
		phoneN_3 = data[ 17 ].Right( 4 );
		
		if( 10 == phoneNumLength )
			phoneN_2 = data[ 17 ].Mid( 3, 3 );
		else
			phoneN_2 = data[ 17 ].Mid( 3, 4 );
	}
*/
	/*			
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbUSER5 );
	*/

	CStringArray asPhone;
	CString phoneN_1, phoneN_2, phoneN_3;
	CLocCommonAPI::MakeArrayList( data[17] , _T("-") , asPhone );

	if( asPhone.GetSize() > 0 )
		phoneN_1 = asPhone.GetAt(0);

	if( asPhone.GetSize() > 1)
		phoneN_2 = asPhone.GetAt(1);

	if( asPhone.GetSize() > 2)
		for( INT i = 2 ; i < asPhone.GetSize() ; i++ )
			phoneN_3 += asPhone.GetAt(i);

	pCE = (CESL_Control_Element*)pClinetCM->FindControlElement(_T("�޴���1"));
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ImportCenterUserToScreen") );
	pCombo = (CComboBox*)(pParentMgr->GetDlgItem(pCE->CTRL_ID));
/*	
	CString phoneCorpNum[] = {
		_T("�������"), _T("011"), _T("016"), _T("017"), _T("018"), _T("019") , _T("010") , _T("0505") };
	pCombo->SetCurSel( 0 );
	for( i = 0; i < 8; i++ )
		if( phoneCorpNum[ i ] == phoneN_1 )
			pCombo->SetCurSel( i );	
*/

	CString sPhoneHead;

	for( i = 0; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText( i , sPhoneHead );

		if( sPhoneHead.Compare(phoneN_1) == 0 )
			pCombo->SetCurSel(i);
	}

	SetControlData( _T("CM_BO_LOC_3320"), _T("�޴���2"), phoneN_2 );
	SetControlData( _T("CM_BO_LOC_3320"), _T("�޴���3"), phoneN_3 );

	//++2008.12.19 ADD BY PWR {{++
	// �ֹι�ȣ �и��� ����
	pParent->SetCivilNum();
	//--2008.12.19 ADD BY PWR --}}

	CString mailHost, host;
	INT		nHostCnt;
	INT locate = data[ 18 ].Find( _T("@") );	
	pCE = ( CESL_Control_Element * )pClinetCM->FindControlElement( _T("EMAIL2") );
	pCombo = ( CComboBox * )( pParentMgr->GetDlgItem( pCE->CTRL_ID ) );

	nHostCnt = pCombo->GetCount();
	if( -1 != locate  )
	{
		SetControlData( _T("CM_BO_LOC_3320"), _T("EMAIL1"), data[ 18 ].Left( locate ) );

		mailHost = data[ 18 ].Right( data[ 18 ].GetLength() - locate - 1 );
		for( INT i = 0; i <nHostCnt ; i++ )
		{
			pCombo->SetCurSel( i );
			pCombo->GetWindowText( host );

			if( host == mailHost )
			{
				pCombo->SetCurSel( i );
				i = -1;
				break;
			}
		}
		if( -1 != i )
		{
			// 2009.05.20 ADD BY PDJ : ���� HOST�� ��� �߰���
			//pCombo->SetCurSel( 0 );			
			pCombo->AddString(mailHost);			
			pCombo->SetCurSel(nHostCnt);			
		}
	}	
	
	// ���� �����ޱ�
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridCENTERUSER );
	INT row = pGrid->GetRow() - 1;
	
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );

	CString sMasterUserRecKey;
	CString sUserPictureName;
	pDM->GetCellData(_T("�̿��ڻ����̸�") , row , sUserPictureName );

	sUserPictureName.TrimLeft(); sUserPictureName.TrimRight();

	//++2008.12.19 ADD BY PWR {{++
	// ���͹������� ������ ������̸� ���� �ߺ��˻� ���ϵ��� FLAG�߰�
	pParent->m_bImport = TRUE;
	//++2008.12.19 ADD BY PWR --}}

	CString sSrvFullPath,sClientFullPath;
	if( sUserPictureName.IsEmpty() )
	{
		pParentMgr->SendMessage( WM_USER_PICTURE_IMPORT , ( ( WPARAM )( &sClientFullPath ) ) );
		return 0;
	}

	sSrvFullPath.Format( _T("CT_MASTER_LOAN_USER_TBL") );
	sClientFullPath.Format( _T("..\\tunning\\%s") , sUserPictureName );
	
	CRS_SQL RS_Sql( this );
//	RS_Sql.LOAN_FileDownLoad( sUserPictureName , sClientFullPath , sUserPictureName , sSrvFullPath );
	pParentMgr->SendMessage( WM_USER_PICTURE_IMPORT , ( ( WPARAM )( &sClientFullPath ) ) );

	return 0;	

EFS_END
return -1;
}

VOID CCO_CENTER_USER_MANAGER_01::SearchUserByCivilNum()
{
	EFS_BEGIN

	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );
	
	m_initCivilNum.Replace( _T("-"), _T("") );
	m_initCivilNum.Replace( _T(" "), _T("") );
	m_initUserName.Replace( _T(" "), _T("") );
	
	CString where_1, where_2, where;

	if( !m_initCivilNum.IsEmpty() )
		where_1.Format( _T("CIVIL_NO IN( '%s', '%s-%s' )"), m_initCivilNum, m_initCivilNum.Left( 6 ), m_initCivilNum.Right( 7 ) );

	if( !m_initUserName.IsEmpty() )
		where_2.Format( _T("NAME LIKE '%%%s%%'"), m_initUserName );
	
	if( where_1.IsEmpty() && where_2.IsEmpty() )
		return;
	else if( !where_1.IsEmpty() && !where_2.IsEmpty() )
		where.Format( _T("%s OR %s"), where_1, where_2 );
	else
		where = where_1 + where_2;
	
//	pRemoteDBMgr->SetFetchCount( 1001, 100 );
//	pRemoteDBMgr->LOAN_Select( pDM, where, _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST"), 1, 1 );

//DEL 	for( INT i = 0; i < pDM->GetRowCount(); i++ )
//DEL 		pDM->SetCellData( _T("�ֹι�ȣ_VIEW"), ConvertCivilNum( pDM->GetCellData( _T("�ֹι�ȣ_VIEW"), i ) ), i );
	INT ids = CLocCommonAPI::ChangeSecuriyCivilNo( this , pDM , -1, _T("�ֹι�ȣ_VIEW") , m_sIsUnityLoanService );
	if(0>ids) return;

	// 2010.10.18 ADD BY KSJ : �����ڹ�ȣ -> �����ڹ�ȣ_VIEW
	CString strUserNo;
	INT nRowCnt = pDM->GetRowCount();
	for(INT i=0; i<nRowCnt; i++)
	{
		strUserNo = pDM->GetCellData(_T("�����ڹ�ȣ"), i);
		ids = CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo, m_sIsUnityLoanService );
		pDM->SetCellData(_T("�����ڹ�ȣ_VIEW"), strUserNo, i);
	}

	ControlDisplay( _T("CM_CO_CENTER_USER_MANAGER_01"), _T("�����̿��������׸���") );

	EFS_END
}

CString CCO_CENTER_USER_MANAGER_01::ConvertCivilNum( CString civilNum )
{
	EFS_BEGIN

	civilNum.Replace( _T("-"), _T("") );
	if( 13 != civilNum.GetLength() )
		return civilNum;

	CString frontNum = civilNum.Left( 6 );
	civilNum.Format( _T("%s-*******"), frontNum );	
	return civilNum;

	EFS_END

	return _T("");
}

CString CCO_CENTER_USER_MANAGER_01::MakeCivilNum( CString civilNum )
{
	EFS_BEGIN

	CString returnValue;
	
	civilNum.Replace( _T(" "), _T("") );
	civilNum.Replace( _T("-"), _T("") );
	if( 13 != civilNum.GetLength() )
		return civilNum;

	returnValue.Format( _T("%s-%s"), civilNum.Left( 6 ), civilNum.Right( 7 ) );
	return returnValue;

	EFS_END

	return _T("");
}

VOID CCO_CENTER_USER_MANAGER_01::OnbPRINT() 
{
	EFS_BEGIN
	
	INT ids, i;	

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );
	if( pSrcDM == NULL ) return;
	CESL_DataMgr *pPrintDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST_PRINT") );
	if( pPrintDM == NULL ) return;
	
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return;
	}
// pSrcDM -> pPrintDM���� ����
	const INT nCopyCnt=8;
	pPrintDM->FreeData();
	
	TCHAR *sField[nCopyCnt][2] =
	{
		{_T("���������������̸�"), _T("���������������̸�")},
		{_T("�����������̸�"), _T("�����������̸�")},
		{_T("�ּ�_��"), _T("�ּ�_��")},
		{_T("�ֹι�ȣ_VIEW"),_T("�ֹε�Ϲ�ȣ")},
		{_T("����������"),_T("����������")},
		{_T("������"),_T("������")},
		{_T("�̸�"),_T("�̸�")},				
		{_T("�����ڹ�ȣ_VIEW"),_T("�����ڹ�ȣ")}		
	};
	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_CO_CENTER_USER_MANAGER_01") , _T("�����̿��������׸���") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	
	INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();
	
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
	CString sData;
	INT nIndex;
	for( i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();
		

		pPrintDM->InsertRow(-1);
		
		sData.Format( _T("%d ") , i+1 );
		ids = pPrintDM->SetCellData( _T("��ȣ") , sData , i);
		if( ids < 0 ) return;
		
		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) return;
			
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) return;
		}
		if( nMode == 1)
			pGrid->SelectGetNext();
		
	}
	// �Ϲ����� �ݺ��� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����������_��������_���"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}
	
	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return;
	
	EFS_END
	return;		
}

/// ����Ʈ �ݹ� �Լ� - �ʿ����
INT CCO_CENTER_USER_MANAGER_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	
	return 0;
	
	EFS_END
	return -1;	
}

/// 2008.06.04 ADD BY PWR 
/// �����̿��� �ߺ��˻�� �Լ� �߰�
/// CBO_LOC_3320���� ���ȴ� 
INT CCO_CENTER_USER_MANAGER_01::IsDuplicateCenterUser()
{
EFS_BEGIN

	INT		ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST"));
	if( NULL == pDM ) 
	{
		AfxMessageBox(_T("CCO_CENTER_USER_MANAGER_01::IsDuplicateCenterUser() ERROR -1"));
		return -1;
	}
	
	m_initCivilNum.Replace( _T("-"), _T("") );
	m_initCivilNum.Replace( _T(" "), _T("") );
	m_initUserName.Replace( _T(" "), _T("") );

	CString strWhere;

	/// �ֹι�ȣ�� �ݵ�� �־�� �Ѵ�!!
	if( m_initCivilNum.IsEmpty() )
		return 0;

	strWhere.Format( _T("CIVIL_NO IN( '%s', '%s-%s' )"), m_initCivilNum, m_initCivilNum.Left( 6 ), m_initCivilNum.Right( 7 ) );

	if( !m_initUserName.IsEmpty() )
	{
		strWhere += (_T(" AND NAME LIKE '%%") + m_initUserName + _T("%%'"));
	}
	
//	ids = pRemoteDBMgr->SetFetchCount( 1001, 100 );
	if( 0 > ids )
	{
		AfxMessageBox(_T("CCO_CENTER_USER_MANAGER_01::IsDuplicateCenterUser() ERROR -2"));
		return -2;
	}
//	ids = pRemoteDBMgr->LOAN_Select( pDM, strWhere, _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST"), 1, 1 );
	if( 0 > ids )
	{
		// �޽��� ó���� ������ �� �� �ֵ��� ����
		//AfxMessageBox(_T("CCO_CENTER_USER_MANAGER_01::IsDuplicateCenterUser() ERROR -3"));
		return -3005;
	}

	return pDM->GetRowCount();

EFS_END
return -1;
}

HBRUSH CCO_CENTER_USER_MANAGER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
