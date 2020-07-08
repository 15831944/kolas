// CO_CENTER_USER_UPLOAD.cpp : implementation file
//

#include "stdafx.h"
#include "CO_CENTER_USER_UPLOAD.h"
// 2011.09.08 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_UPLOAD dialog


CCO_CENTER_USER_UPLOAD::CCO_CENTER_USER_UPLOAD(CESL_Mgr* pParent,CESL_DataMgr* pUserKeyDM,CString sLibKey,CString sLibCode)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_UPLOAD)
	//}}AFX_DATA_INIT

	m_pUserKeyDM = pUserKeyDM;
	m_sLibKey = sLibKey;
	m_sLibCode = sLibCode;
	m_bStop = FALSE;
	m_bBreak = FALSE;
}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CCO_CENTER_USER_UPLOAD::~CCO_CENTER_USER_UPLOAD()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CCO_CENTER_USER_UPLOAD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_UPLOAD)
	DDX_Control(pDX, IDC_prgUPLOAD, m_PrgCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_UPLOAD, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_UPLOAD)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnUPLOAD_CLOSE, OnbtnUPLOADCLOSE)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_UPLOAD message handlers

BOOL CCO_CENTER_USER_UPLOAD::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("CO_CENTER_USER_UPLOAD")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	SetTimer(0,100,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
		
	return FALSE;
}



VOID CCO_CENTER_USER_UPLOAD::OnTimer(UINT nIDEvent) 
{
	EFS_BEGIN

	if( nIDEvent == 0 )
	{
		KillTimer(0);

		if( GetSafeHwnd() != NULL )
		{
			CenterUpload();
			return ;
		}
		SetTimer(0,100,NULL);
	}
	
	CDialog::OnTimer(nIDEvent);

	EFS_END
}

INT CCO_CENTER_USER_UPLOAD::CenterUpload()
{
	EFS_BEGIN

	AfxBeginThread( CenterUploadThread , this );

	return 0;

	EFS_END

	return -1;
}


// ���� ���ε�
// ȭ���۾� + DB�۾�
INT CCO_CENTER_USER_UPLOAD::CenterUploadProc()
{
	EFS_BEGIN
		
	INT ids;

	// ȭ�� �غ�
	ids = DisplayReady();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CenterUploadProc") );
	if( ids > 0 ) return ids;
	
	// DB �۾�
	ids = CenterUploadDBProc();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CenterUploadProc") );
	if( ids > 0 ) return ids;
	
	return 0;
	
	EFS_END

	return -1;
}


VOID CCO_CENTER_USER_UPLOAD::OnbtnUPLOADCLOSE() 
{
	EFS_BEGIN
	
	if( !m_bStop )
	{
		if( !m_bBreak )
		{
			INT ids = MessageBox( _T("�����Ͻðڽ��ϱ�?") , _T("����") , MB_YESNO | MB_ICONSTOP );
			if( ids == IDYES )
			{
				m_bBreak = TRUE;
			}
		}
		else
		{
			AfxMessageBox(_T("�������Դϴ�."));

		}

		return ;
	}

	OnCancel();

	EFS_END
}


// ȭ�� �غ� �۾�
// ó���� �ڷ��
// ���α׷����� �غ�
INT CCO_CENTER_USER_UPLOAD::DisplayReady()
{
	EFS_BEGIN

	INT nUserCnt;

	nUserCnt = m_pUserKeyDM->GetRowCount();

	// ���α׷����� �ʱ�ȭ
	m_PrgCtrl.SetRange32( 0 , nUserCnt );

	// �α� �ʱ�ȭ
	CString sMsg;
	
	if( nUserCnt > 0 )
	{
		sMsg.Format( _T("%d���� �̿��� ���ε� ����") , nUserCnt );
		AddingLog( sMsg );
	}
	else
	{
		sMsg.Format( _T("���ε��� �̿��ڰ� �������� �ʽ��ϴ�.") );
		AddingLog( sMsg );
		return 1;
	}

	return 0;

	EFS_END

	return -1;

}

INT CCO_CENTER_USER_UPLOAD::AddingLog( CString sMsg )
{
	EFS_BEGIN


	CString sTime;
	sTime = (CLocCommonAPI::GetDBTime(this)).Format(_T("%H�� %M�� %S��"));

	// �α� DM�� ���
	CESL_DataMgr* pLogDM = FindDM( _T("DM_UPLOAD_LOG"));
	if( pLogDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddingLog") );

	pLogDM->InsertRow(-1);
	INT nInsertIndex = pLogDM->GetRowCount()-1;

	pLogDM->SetCellData( _T("�ð�") , sTime , nInsertIndex );
	pLogDM->SetCellData( _T("�α�") , sMsg , nInsertIndex);


	// �׸��忡 �ѷ��ֱ�
	CLocCommonAPI::InsertGridRow(this,_T("CM_UPLOAD_LOG") , _T("�αױ׸���") , pLogDM->GetRowCount()-1 , pLogDM->GetRowCount()-1 );

	return 0;

	EFS_END

	return -1;
}

// DB�۾�
INT CCO_CENTER_USER_UPLOAD::CenterUploadDBProc()
{
	EFS_BEGIN

	INT ids;

	INT nUploadUserCnt = m_pUserKeyDM->GetRowCount();

	CString sUploadUserKey;
	for( INT i = 0 ; i < nUploadUserCnt ; i++ )
	{
		SetPos(i+1);

		ids = m_pUserKeyDM->GetCellData( _T("�̿���KEY") , i , sUploadUserKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CenterUploadDBProc") );

		// LOCAL DB���� �̿��� ������ Get
		ids = GetUserInfoFromLocalDB(sUploadUserKey);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CenterUploadDBProc") );
		if( ids > 0 )
		{
			if( ids == 1000 )
			{
				CString sEMsg;
				sEMsg.Format(_T("�̿���KEY�� '%s'�� �ڷᰡ DB�� �������� �ʽ��ϴ�.") , sUploadUserKey );
				AddingLog( sEMsg );
			}
			continue;
		}


		// Center�������� UPLOAD
		ids = UploadUserInfoToCenterDB();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CenterUploadDBProc") );
		if( ids > 0 )
		{
			if( ids == 1000 )
			{
				CString sEMsg;
				sEMsg.Format(_T("�̿���KEY�� '%s'�� �ڷ� ���ε�� �˼� ���� ������ ���� �۾��� �������� ���߽��ϴ�.") , sUploadUserKey );
				AddingLog( sEMsg );
			}

			ids = RecordSFStatus( sUploadUserKey ,i , FALSE );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CenterUploadDBProc") );

			continue;
		}

		ids = RecordSFStatus( sUploadUserKey , i , TRUE );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CenterUploadDBProc") );

		if( m_bBreak )
		{
			AddingLog( _T("����ڿ� ���� �����Ǿ����ϴ�.") );
			return 100;
		}
		
	}

	AddingLog( _T("�Ϸ�Ǿ����ϴ�."));

	return 0;

	EFS_END

	return -1;
}

/// LOCAL DB���� �̿��� ������ ���´�.
INT CCO_CENTER_USER_UPLOAD::GetUserInfoFromLocalDB( CString sUserKey )
{
	EFS_BEGIN

	INT ids;

	CString sUserInfoDMAlias = _T("DM_UPLOAD_USER_INFO");

	CESL_DataMgr* pUserInfoDM = FindDM( sUserInfoDMAlias );
	if( pUserInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetUserInfoFromLocalDB") );

	CString sWhere;
	sWhere.Format( _T(" REC_KEY = %s ") , sUserKey );

	pUserInfoDM->FreeData();

	ids = pUserInfoDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetUserInfoFromLocalDB") );

	INT nRowCount = pUserInfoDM->GetRowCount();

	if( nRowCount == 0 )
		return 1000;

	// 2011.09.08 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pUserInfoDM->GetRowCount())
	{
		CString strGetData;
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7,asDes8;
		for(INT i = 0; i < pUserInfoDM->GetRowCount(); i++)
		{
			pUserInfoDM->GetCellData(_T("�����ּ�"), i, strGetData);
			asDes1.Add(strGetData);
			pUserInfoDM->GetCellData(_T("�ٹ����ּ�"), i, strGetData);
			asDes2.Add(strGetData);
			pUserInfoDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strGetData);
			asDes3.Add(strGetData);
			pUserInfoDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes4.Add(strGetData);
			pUserInfoDM->GetCellData(_T("�ڵ���"), i, strGetData);
			asDes5.Add(strGetData);
			pUserInfoDM->GetCellData(_T("��й�ȣ"), i, strGetData);
			asDes6.Add(strGetData);
			pUserInfoDM->GetCellData(_T("������ȭ��ȣ"), i, strGetData);
			asDes7.Add(strGetData);
			pUserInfoDM->GetCellData(_T("�ٹ�����ȭ��ȣ"), i, strGetData);
			asDes8.Add(strGetData);
		}
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes1);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes2);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes3);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes4);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("PASSWORD"), asDes6);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes7);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes8);
					p3000Api->CloseSocket();	
				}
				for(i = 0; i < pUserInfoDM->GetRowCount(); i++)
				{
					pUserInfoDM->SetCellData(_T(""), asDes1.GetAt(i), i);
					pUserInfoDM->SetCellData(_T(""), asDes2.GetAt(i), i);
					pUserInfoDM->SetCellData(_T(""), asDes3.GetAt(i), i);
					pUserInfoDM->SetCellData(_T(""), asDes4.GetAt(i), i);
					pUserInfoDM->SetCellData(_T(""), asDes5.GetAt(i), i);
					pUserInfoDM->SetCellData(_T(""), asDes6.GetAt(i), i);
					pUserInfoDM->SetCellData(_T(""), asDes7.GetAt(i), i);
					pUserInfoDM->SetCellData(_T(""), asDes8.GetAt(i), i);
				}
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
		asDes6.RemoveAll(); asDes6.FreeExtra();
		asDes7.RemoveAll(); asDes7.FreeExtra();
		asDes8.RemoveAll(); asDes8.FreeExtra();
	}

	return 0;

	EFS_END

	return -1;
}

/// Center DB�� �̿��� ������ ������Ʈ�Ѵ�.
INT CCO_CENTER_USER_UPLOAD::UploadUserInfoToCenterDB()
{
	EFS_BEGIN

	INT ids;

	CString sUserInfoDMAlias = _T("DM_UPLOAD_USER_INFO");
	
	CESL_DataMgr* pUserInfoDM = FindDM( sUserInfoDMAlias );
	if( pUserInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("UploadUserInfoToCenterDB") );

	/// �ߺ� �˻�� �ֹε�Ϲ�ȣ�� �������� �Ѵ�.
	CString sCivilNo;	

	ids = pUserInfoDM->GetCellData( _T("�ֹε�Ϲ�ȣ") , 0 , sCivilNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("UploadUserInfoToCenterDB") );


	CESL_DataMgr* pCenterUploadDM = FindCenterDM();
	if( pCenterUploadDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("UploadUserInfoToCenterDB") );

	// ���� ���� ���� ���̺� ( CO_MASTER_LOAN_USER_TBL )�� �̿��ڰ� �����ϴ��� �˻�
	// �����ϸ� UPDATE���� �������� ������ INSERT ����

	CString sValue;
	CString sCenterUserKey;
	CString sSql;
	sSql.Format(_T("SELECT COUNT(*) FROM CO_MASTER_LOAN_USER_TBL WHERE CIVIL_NO='%s'"),sCivilNo);
	ids = pCenterUploadDM->GetOneValueQuery(sSql,sValue);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("UploadUserInfoToCenterDB") );

	pCenterUploadDM->StartFrame();

	ids = MakeMasterLoanUserTblFrame( sValue , sCenterUserKey , pCenterUploadDM , sCivilNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("UploadUserInfoToCenterDB") );
	

	// �������� ������ ���� ���̺� ( CO_LIBRARY_USER_TBL )�� �̿��ڰ� �����ϴ��� �˻�
	// �����ϸ� UPDATE���� �������� ������ INSERT ����
	sSql.Format(_T("SELECT COUNT(*) FROM CO_LIBRARY_USER_TBL WHERE LIB_KEY='%s' AND USER_KEY='%s'"),m_sLibKey,sCenterUserKey);
	ids = pCenterUploadDM->GetOneValueQuery(sSql,sValue);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("UploadUserInfoToCenterDB") );

	ids = MakeLibraryUserTblFrame( sValue , sCenterUserKey , pCenterUploadDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("UploadUserInfoToCenterDB") );
	
	// ������ ����
	ids = pCenterUploadDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("UploadUserInfoToCenterDB") );
	
	pCenterUploadDM->EndFrame();


	return 0;

	EFS_END

	return -1;
}


/// ���� ���θ� DB�� ��� + �̿���KEY DM���� ���
INT CCO_CENTER_USER_UPLOAD::RecordSFStatus( CString sUserKey , INT nIndex , bool bSuccess )
{
	EFS_BEGIN

	INT ids;

	CString sValue = _T("Y");

	if( !bSuccess )
		sValue = _T("F");

	CESL_DataMgr* pLocalUpdateDM = FindLocalUpdateDM();
	if( pLocalUpdateDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RecordSFStatus") );

	pLocalUpdateDM->StartFrame();

	pLocalUpdateDM->InitDBFieldData();
	pLocalUpdateDM->AddDBFieldData( _T("CENTER_UPLOAD_CHECK") , _T("STRING") , sValue , TRUE );
	pLocalUpdateDM->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sUserKey );

	ids = pLocalUpdateDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RecordSFStatus") );

	pLocalUpdateDM->EndFrame();

	ids = m_pUserKeyDM->SetCellData( _T("��������") , sValue , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RecordSFStatus") );
	
	return 0;
	
	EFS_END

	return -1;
}

/// ���� DB�� ������ �� �ִ� DM�� �ε��Ѵ�.
CESL_DataMgr* CCO_CENTER_USER_UPLOAD::FindLocalUpdateDM()
{
	EFS_BEGIN

	CESL_DataMgr* pDM = FindDM( _T("DM_LOCAL_UPLOAD") );
	return pDM;

	EFS_END

	return NULL;
}

/// ���� DB�� ������ �� �ִ� DM�� �ε��Ѵ�.
CESL_DataMgr* CCO_CENTER_USER_UPLOAD::FindCenterDM()
{
	EFS_BEGIN

	CESL_DataMgr* pDM = FindDM( _T("DM_CENTER_UPLOAD") );
	return pDM;

	EFS_END

	return NULL;
}


/// ���� ���� ���� ���̺� CO_MASTER_LOAN_USER_TBL�� UPDATE�� INSERT�Ѵ�.
INT CCO_CENTER_USER_UPLOAD::MakeMasterLoanUserTblFrame( CString sValue , CString &sCenterUserKey , CESL_DataMgr* pDM , CString sCivilNo )
{
	EFS_BEGIN

	INT ids;

	CString sTableName = _T("CO_MASTER_LOAN_USER_TBL");

	CString sUserInfoDMAlias = _T("DM_UPLOAD_USER_INFO");
	
	CESL_DataMgr* pUserInfoDM = FindDM( sUserInfoDMAlias );
	if( pUserInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("UploadUserInfoToCenterDB") );

	const INT nUpdateCnt = 19;

	CString sDMColumn[nUpdateCnt] =
	{
		_T("�����ּ�") , _T("�ٹ����ּ�") , _T("����") , _T("�ֹε�Ϲ�ȣ") , _T("��") ,
		_T("�ٹ������޸�") , _T("�ٹ����μ���") , _T("E_MAIL") , _T("�г�") , _T("�ڵ���") , 
		_T("����߼�����") , _T("�������̸�") , _T("������ȭ��ȣ") , _T("�ٹ�����ȭ��ȣ") ,_T("�б�") , 
		_T("�����ڹ�ȣ") , _T("���ÿ����ȣ") , _T("�ٹ��������ȣ") ,  _T("����TYPE")
	};

	CString sFieldType[nUpdateCnt] =
	{
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") 
	};

	CString sDBFieldName[nUpdateCnt] =
	{
		_T("H_ADDR1") , _T("W_ADDR1") , _T("BIRTHDAY") , _T("CIVIL_NO") , _T("CLASS") ,
		_T("CLASS_NAME") , _T("DEPARTMENT_NAME") , _T("E_MAIL") , _T("GRADE") , _T("HANDPHONE") ,
		_T("MAIL_SEND_AREA") , _T("NAME") , _T("H_PHONE") , _T("W_PHONE") , _T("SCHOOL") ,
		_T("USER_NO") , _T("H_ZIPCODE") , _T("W_ZIPCODE") , _T("BIRTHDAY_TYPE")
	};

	pDM->InitDBFieldData();

	CString sData;
	for( INT i = 0 ; i < nUpdateCnt ; i++ )
	{
		ids = pUserInfoDM->GetCellData( sDMColumn[i] , 0 , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMasterLoanUserTblFrame") );

		pDM->AddDBFieldData(sDBFieldName[i] , sFieldType[i] , sData , TRUE );
	}

	/// REC_KEY ����
	if( _ttoi(sValue) == 0 )
	{
		pDM->MakeRecKey( sCenterUserKey );
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sCenterUserKey , TRUE);
		pDM->AddDBFieldData( _T("MAKE_DATE") , _T("NUMERIC") , _T("SYSDATE") , TRUE );
		pDM->AddDBFieldData( _T("MAKE_LIB_CODE") , _T("STRING") , m_sLibCode , TRUE );
		pDM->MakeInsertFrame( sTableName );
	}
	else
	{
		CString sSql;
		sSql.Format( _T("SELECT REC_KEY FROM %s WHERE CIVIL_NO='%s' ") , sTableName , sCivilNo );
		ids = pDM->GetOneValueQuery( sSql , sCenterUserKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMasterLoanUserTblFrame") );

		pDM->AddDBFieldData( _T("UPDATE_DATE") , _T("NUMERIC") , _T("SYSDATE") );
		pDM->AddDBFieldData( _T("LAST_UPDATE_LIB_CODE") , _T("STRING") , m_sLibCode  , TRUE );
		
		pDM->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sCenterUserKey );
	}
	
	return 0;

	EFS_END

	return -1;
}



/// ���� ���� ���� ���̺� CO_MASTER_LOAN_USER_TBL�� UPDATE�� INSERT�Ѵ�.
INT CCO_CENTER_USER_UPLOAD::MakeLibraryUserTblFrame( CString sValue , CString sCenterUserKey , CESL_DataMgr* pDM )
{
	EFS_BEGIN
		
	INT ids;
	
	CString sTableName = _T("CO_LIBRARY_USER_TBL");
	
	CString sUserInfoDMAlias = _T("DM_UPLOAD_USER_INFO");
	
	CESL_DataMgr* pUserInfoDM = FindDM( sUserInfoDMAlias );
	if( pUserInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeLibraryUserTblFrame") );
	
	const INT nUpdateCnt = 9;
	
	CString sDMColumn[nUpdateCnt] =
	{
		_T("������ID") , _T("��й�ȣ") , _T("�����") , _T("ȸ������") , _T("����Ҽ�����") , 
		_T("������������") , _T("��������������") , _T("���") , _T("��߱�Ƚ��")
	};
	
	CString sFieldType[nUpdateCnt] =
	{
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("STRING") , _T("NUMERIC") 
	};
	
	CString sDBFieldName[nUpdateCnt] =
	{
		_T("USER_ID") , _T("PASSWORD") , _T("REG_DATE") , _T("MEMBER_CLASS") , _T("USER_POSITION_CODE") , 
		_T("USER_CLASS_CODE") , _T("LAST_MODIFY_DATE") , _T("REMARK") , _T("REISSUE_CNT")
	};
	
	pDM->InitDBFieldData();
	
	CString sData;
	for( INT i = 0 ; i < nUpdateCnt ; i++ )
	{
		ids = pUserInfoDM->GetCellData( sDMColumn[i] , 0 , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeLibraryUserTblFrame") );
		
		pDM->AddDBFieldData(sDBFieldName[i] , sFieldType[i] , sData , TRUE );
	}
	
	/// REC_KEY ����
	CString sRecKey;
	if( _ttoi(sValue) == 0 )
	{
		pDM->MakeRecKey( sRecKey );
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pDM->AddDBFieldData( _T("MAKE_DATE") , _T("NUMERIC") , _T("SYSDATE") );
		pDM->AddDBFieldData( _T("USER_KEY") , _T("NUMERIC") , sCenterUserKey );
		pDM->AddDBFieldData( _T("LIB_KEY") , _T("NUMERIC") , m_sLibKey );
		pDM->MakeInsertFrame( sTableName );
	}
	else
	{
		
		CString sSql;
		sSql.Format(_T("SELECT REC_KEY FROM CO_LIBRARY_USER_TBL WHERE LIB_KEY='%s' AND USER_KEY='%s'"),m_sLibKey,sCenterUserKey);
		ids = pDM->GetOneValueQuery( sSql , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeLibraryUserTblFrame") );
		
		pDM->AddDBFieldData( _T("UPDATE_DATE") , _T("NUMERIC") , _T("SYSDATE") );
		
		pDM->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	}
	
	return 0;
	
	EFS_END
		
	return -1;
}

INT CCO_CENTER_USER_UPLOAD::SetPos( INT nPos )
{
	m_PrgCtrl.SetPos( nPos );

	return 0;
}



// ������ �Լ�
UINT CenterUploadThread(LPVOID lParam)
{
	EFS_BEGIN
		
		INT ids;
	CString sErrorMsg;
	
	CCO_CENTER_USER_UPLOAD *pObj = (CCO_CENTER_USER_UPLOAD *)lParam;
	if ( pObj == NULL )
		return -1;
	
	pObj->m_bStop = FALSE;
	pObj->m_bBreak = FALSE;
	
	ids = pObj->CenterUploadProc();
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
	pObj->m_bBreak = TRUE;
	
	return 0;
	
	EFS_END
		
	return -1;
}

VOID CCO_CENTER_USER_UPLOAD::OnClose() 
{
	if( !m_bStop )
	{
		return ;
	}
	
	CDialog::OnClose();
}

HBRUSH CCO_CENTER_USER_UPLOAD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// ������� �߰�

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
