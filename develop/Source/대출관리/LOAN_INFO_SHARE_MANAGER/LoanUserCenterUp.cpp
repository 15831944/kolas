// LoanUserCenterUp.cpp : implementation file
//

#include "stdafx.h"
#include "LoanUserCenterUp.h"
// 2011.09.07 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanUserCenterUp dialog

/// ��ȣȭ �Ұ�����
#define _ENCRYPT TRUE

//#define _H_DEBUG 

#define _PICTURE_PATH _T("..\\tunning")
#define _PICTURE_FILE_NAME _T("cpicture")

#define MAX_BLOB_BUFFER 20000000

CLoanUserCenterUp::CLoanUserCenterUp(CESL_Mgr* pParent /*=NULL*/)
	: CShareInfoCenterUp(pParent)
{
	//{{AFX_DATA_INIT(CLoanUserCenterUp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	/// ���� ���̺� �����ϱ�
	m_pMasterUserMapping = new CTableFieldMapping( this , MASTER_LOAN_USER );
	m_pMasterUserHistoryMapping = new CTableFieldMapping( this , MASTER_LOAN_USER_HISTORY );
	m_pLibUserMapping = new CTableFieldMapping( this , LIB_LOAN_USER );
}

CLoanUserCenterUp::~CLoanUserCenterUp()
{
	delete m_pMasterUserMapping;
	delete m_pMasterUserHistoryMapping;
	delete m_pLibUserMapping;
}

VOID CLoanUserCenterUp::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanUserCenterUp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CLoanUserCenterUp, CDialog)
	//{{AFX_MSG_MAP(CLoanUserCenterUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanUserCenterUp message handlers

INT CLoanUserCenterUp::SetData(UPLOAD_SHARE_INFO UploadInfo , CString sLibCode ) 
{
	EFS_BEGIN

	m_ShareInfo = UploadInfo.UploadShareLoanInfo;

	CShareInfoCenterUp::SetData( UploadInfo , sLibCode );

	return 0;

	EFS_END
	return -1;

}

INT CLoanUserCenterUp::UploadData()
{
	EFS_BEGIN

	INT ids;

	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UploadData() [START]") );
	
	CString sType = m_ShareInfo.sType;

	// �̹� Temp Table�� �����ϴ��� �˾ƺ���
	ids = IsTempTable( m_ShareInfo.sUpdatedTableRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("UploadData") );
	if( ids == 0 )
	{
		InsertTempTable(m_ShareInfo,_UP_ERROR_CODE_IS_TEMP_TBL);
		return 0;
	}
	

	/// INSERT ����
	if( sType.Compare(_T("I")) == 0 )
	{
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::InsertUser() [START]") );
		ids = InsertUser();
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::InsertUser() [END]") );
	}
	/// UPDATE ����
	else if( sType.Compare(_T("U")) == 0 )
	{
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UpdateUser() [START]") );
		ids = UpdateUser();
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UpdateUser() [END]") );
	}
	/// DELETE ���� �˾ƺ���
	else if( sType.Compare(_T("D")) == 0 )
	{
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::DeleteUser() [START]") );
		ids = DeleteUser();
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::DeleteUser() [END]") );
	}
	else
	{
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UploadData() [ERROR]-10001") );
		return 10001;
	}

	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UploadData() [END]") );
	

	return ids;

	EFS_END
	return -1;

}

INT CLoanUserCenterUp::InsertUser()
{
	EFS_BEGIN

	INT ids;

	// 1. CLINET ���� ���� ���ؿ���
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::MakeLoaclUserInfo() [START]") );
	ids = MakeLoaclUserInfo();
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::MakeLoaclUserInfo() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertUser") );
	if( ids > 0 ) return ids;
	
	// 2. �̿��� INSERT
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::InsertLocalUserToCenter() [START]") );
	ids = InsertLocalUserToCenter();
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::InsertLocalUserToCenter() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertUser") );
	if( ids > 0 ) return ids;
	
	
	return 0;

	EFS_END
	return -1;

}


INT CLoanUserCenterUp::UpdateUser()
{
	EFS_BEGIN

	INT ids;

	// 1. CLINET ���� ���� ���ؿ���
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::MakeLoaclUserInfo() [START]") );
	ids = MakeLoaclUserInfo();
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::MakeLoaclUserInfo() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertUser") );
	if( ids > 0 ) return ids;
	
	// 2. �̿��� UPDATE
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UpdateLocalUserToCenter() [START]") );
	ids = UpdateLocalUserToCenter();
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UpdateLocalUserToCenter() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertUser") );
	if( ids > 0 ) return ids;

	return 0;

	EFS_END
	return -1;

}



INT CLoanUserCenterUp::DeleteUser()
{
	EFS_BEGIN

	INT ids;
	
	/// �̿��ڰ� �����ϴ��� Ȯ��
	CString sLibUserKey;
	ids = IsLibUserInfo( sLibUserKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DeleteUser") );
	if( ids > 1 ) return ids;
	
	/// �������� �ʴ´�.
	if( ids != 1 ) return 0;

	/// ���� �Ѵٸ� ����
	// 1. [1201][1008] �������� ������ ���� ����
	ids = DeleteLibUserInfo(sLibUserKey);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DeleteUser") );
	if( ids > 0 ) return ids;	
	

	return 0;

	EFS_END
	return -1;

}


/// Local �̿��� ���� Get
INT CLoanUserCenterUp::MakeLoaclUserInfo()
{
	EFS_BEGIN

	CESL_DataMgr* pDM = FindDM( _T("DM_CLIENT_USER_INFO") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeLoaclUserInfo") );

	CString sWhere;
	sWhere.Format( _T("REC_KEY = '%s'") , m_ShareInfo.sUpdatedTableRecKey );

	INT ids = pDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeLoaclUserInfo") );

	if( pDM->GetRowCount() == 0 )
		return 10002;
	
	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
	{
		CString strEnc;
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7,asDes8,asDes9;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("H_ADDR1"), i, strEnc);
			asDes1.Add(strEnc);
			pDM->GetCellData(_T("W_ADDR1"), i, strEnc);
			asDes2.Add(strEnc);
			pDM->GetCellData(_T("CIVIL_NO"), i, strEnc);
			asDes3.Add(strEnc);
			pDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes4.Add(strEnc);
			pDM->GetCellData(_T("HANDPHONE"), i, strEnc);
			asDes5.Add(strEnc);
			pDM->GetCellData(_T("PASSWORD"), i, strEnc);
			asDes6.Add(strEnc);
			pDM->GetCellData(_T("H_PHONE"), i, strEnc);
			asDes7.Add(strEnc);
			pDM->GetCellData(_T("W_PHONE"), i, strEnc);
			asDes8.Add(strEnc);
			pDM->GetCellData(_T("IDX_HANDPHONE"), i, strEnc);
			asDes9.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes2);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes3);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes4);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("PASSWORD"), asDes6);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes7);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes8);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("IDX_HANDPHONE"), asDes9);
					pApi->CloseSocket();
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("H_ADDR1"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("W_ADDR1"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("CIVIL_NO"), asDes3.GetAt(i), i);
					pDM->SetCellData(_T("E_MAIL"), asDes4.GetAt(i), i);
					pDM->SetCellData(_T("HANDPHONE"), asDes5.GetAt(i), i);
					pDM->SetCellData(_T("PASSWORD"), asDes6.GetAt(i), i);
					pDM->SetCellData(_T("H_PHONE"), asDes7.GetAt(i), i);
					pDM->SetCellData(_T("W_PHONE"), asDes8.GetAt(i), i);
					pDM->SetCellData(_T("IDX_HANDPHONE"), asDes9.GetAt(i), i);
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
		asDes6.RemoveAll(); asDes6.FreeExtra();
		asDes7.RemoveAll(); asDes7.FreeExtra();
		asDes8.RemoveAll(); asDes8.FreeExtra();
		asDes9.RemoveAll(); asDes9.FreeExtra();
	}

	return 0;

	EFS_END
	return -1;
}

/// Local �̿��� ���� => Center Up BLOB ����
/// DM�� �ִ� ������ ��� �ϸ� �ȵǰ� ������ �о� mapping�� �ʵ带 �̾Ƴ��� �����Ѵ�.
INT CLoanUserCenterUp::InsertLocalUserToCenter()
{
	EFS_BEGIN

	/// ���� ������ ���� KEY
	CString sMasterUserKey;

	// 1. [1004] ������ ���� ������ ������ ó���Ϸ��� �̿��ڰ� �����ϴ°�?
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::IsCenterMasterUser() [START]") );
	INT ids = IsCenterMasterUser(sMasterUserKey);
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::IsCenterMasterUser() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertLocalUserToCenter") );
	if( ids > 1 ) return ids;

	if( ids == 1 )
	{
		// 2. [1005] ���� ������ ������ INSERT
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UploadMasterUserInfo() [START]") );
		ids = UploadMasterUserInfo(sMasterUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UploadMasterUserInfo() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertLocalUserToCenter") );
		if( ids > 0 ) return ids;

		// 3. ���� �̹��� UPLOAD
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::InsertLocalUserImageCenterUp() [START]") );
		ids = InsertLocalUserImageCenterUp(1,sMasterUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::InsertLocalUserImageCenterUp() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertUser") );
		if( ids > 0 ) return ids;
		

		// 4. [1016] ���� ������ ���� �̷¿� INSERT
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::RecordMasterUserHistory() [START]") );
		ids = RecordMasterUserHistory(sMasterUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::RecordMasterUserHistory() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertLocalUserToCenter") );
		if( ids > 0 ) return ids;

		// ���� ������ ������ INSERT�� �Ǿ��ٸ� ������ �� ������ �ʿ�� ����.
		m_ShareInfo.sMasterUserInfoChangeYN = _T("N");
	}


	/// �������� ������ ����KEY
	CString sLibUserKey;
	
	// 3. [1007] �������� ������ ������ ó���Ϸ��� �̿��ڰ� �����ϴ°�?
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::IsLibUserInfo() [START]") );
	ids = IsLibUserInfo(sLibUserKey);
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::IsLibUserInfo() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertLocalUserToCenter") );
	if( ids > 1 ) return ids;

	if( ids == 1 )
	{
		// 4. [1008] �������� ������ ���� ����
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::DeleteLibUserInfo() [START]") );
		ids = DeleteLibUserInfo(sLibUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::DeleteLibUserInfo() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertLocalUserToCenter") );
		if( ids > 0 ) return ids;	
	}

	// 5. [1009] �������� ������ ���� INSERT
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UploadLibUserInfo() [START]") );
	ids = UploadLibUserInfo(sMasterUserKey);
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UploadLibUserInfo() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("InsertLocalUserToCenter") );
	if( ids > 0 ) return ids;

	
	// 6. [1010] ���� ������ ���� ���ΰ� 'Y'�ΰ�?
	if( m_ShareInfo.sMasterUserInfoChangeYN.Compare(_T("Y")) == 0 )
	{
		// 7. [1011] ���� ������ ������ UPDATE
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UpdateMasterUserInfo() [START]") );
		ids = UpdateMasterUserInfo(sMasterUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UpdateMasterUserInfo() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("InsertLocalUserToCenter") );
		if( ids > 0 ) return ids;

		// 8. [1012] ���� ������ ���� �̷¿� INSERT
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::RecordMasterUserHistory() [START]") );
		ids = RecordMasterUserHistory(sMasterUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::RecordMasterUserHistory() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("InsertLocalUserToCenter") );
		if( ids > 0 ) return ids;
	}
	
		
	return 0;
	
	EFS_END
	return -1;
	
}

/// BLOB�� �̿��� ���� => Center Up
/// nMode : 0 -> �������� ������ �̹��� , 1 -> ���� ������ �̹���
INT CLoanUserCenterUp::InsertLocalUserImageCenterUp( INT nMode , CString sRecKey )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_CLIENT_USER_INFO") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertLocalUserImageCenterUp") );
	
	CString sPictureExt;
	ids = pDM->GetCellData( _T("USER_PICTURE_TYPE") , 0 , sPictureExt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertLocalUserImageCenterUp") );

	sPictureExt.TrimLeft(); sPictureExt.TrimRight();

	if( sPictureExt.IsEmpty() ) return 0;

	CString sSrvTableName[2] =
	{
		_T("CT_LIBRARY_LOAN_USER_TBL") , _T("CT_MASTER_LOAN_USER_TBL")
	};

	CString sFileName,sFileFullPath,sSrvFileName,sSrvDirName;
	sFileFullPath.Format( _T("%s\\%s.%s") , _PICTURE_PATH , _PICTURE_FILE_NAME , sPictureExt );
	sFileName.Format( _T("%s.%s") , _PICTURE_FILE_NAME ,  sPictureExt );

	sSrvFileName.Format( _T("%s.%s") , sRecKey , sPictureExt );
	sSrvDirName.Format( _T("%s") , sSrvTableName[nMode]);

	BYTE *pBuffer = (BYTE *)malloc(MAX_BLOB_BUFFER);
	
	INT nSize;
	ids = pDM->LOBGet( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , m_ShareInfo.sUpdatedTableRecKey ,  _T("USER_PICTURE") , pBuffer , &nSize );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertLocalUserImageCenterUp") );

	if( nSize > 0 )
	{
		CFile fd;
		if (fd.Open( sFileFullPath , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) == NULL) {
			AfxMessageBox(_T("�̿��ڻ��� Tmp File Open Error~"));
			free( pBuffer );
			return 1;
		}
		if( 0 == fd.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			fd.Write( &cUni, sizeof(TCHAR));
		}
		
		fd.WriteHuge(pBuffer, nSize);
		free( pBuffer );
		
		fd.Close();
		
		// ���ͷ� ���� ���ε� 
//		ids = m_pCS_SQL->LOAN_FileUpload( sFileName , sFileFullPath , sSrvFileName , sSrvDirName );
		if( ids < 0 )
		{
			CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
			return 10006;
		}

		// ���� ���̺� ���� �̸� UPDATE
		CString sUpdateSql;
		sUpdateSql.Format( _T("UPDATE %s SET USER_PICTURE_NAME = '%s' WHERE REC_KEY=%s;") , sSrvDirName , sSrvFileName , sRecKey);
		
//		ids = m_pCS_SQL->LOAN_UnSelect( sUpdateSql , FALSE );
		if( ids != 0 )
		{
			CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
			return 10006;
		}
		
	}

	
		
	return 0;
	
	EFS_END
	return -1;
	
}

/// [1004] ������ ���� �����ڸ� ó���Ϸ��� �̿��ڰ� �����ϴ°�?
/// sMasterUserKey : �˻��� ���� ������ KEY
INT CLoanUserCenterUp::IsCenterMasterUser( CString &sMasterUserKey )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_CLIENT_USER_INFO") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsCenterMasterUser") );

	CString sSql,sCivilNo;
	ids = pDM->GetCellData( _T("CIVIL_NO") , 0 , sCivilNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("IsCenterMasterUser") );

	if( sCivilNo.IsEmpty() )
		return 10004;

	sSql.Format(	_T("SELECT ")
					_T("REC_KEY ")
					_T("FROM ")
					_T("CT_MASTER_LOAN_USER_TBL ")
					_T("WHERE ")
					_T("CIVIL_NO = '%s' ")
					,
					sCivilNo
				);
	
	
	/// -> Center�� ���� ������ 
	CString sValue;

//	ids = m_pCS_SQL->LOAN_GetOneValue( sSql , sValue , _ENCRYPT );
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;	
	}


	if( sValue.IsEmpty() )
		return 1;

	sMasterUserKey = sValue;
		
	return 0;
	
	EFS_END

	CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
	return 10006;
	
}

/// [1005] ���� ������ ������ INSERT
INT CLoanUserCenterUp::UploadMasterUserInfo( CString &sMasterUserKey )
{
	EFS_BEGIN

	INT ids;


	CString sTableName = _T("CT_MASTER_LOAN_USER_TBL");

	CESL_DataMgr* pLocalUserDM = FindDM( _T("DM_CLIENT_USER_INFO") );
	if( pLocalUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("UploadMasterUserInfo") );


	/// ���� �ʵ� ����
	CESL_DataMgr* pMappingDM = m_pMasterUserMapping->GetMappingInfo();
	if( pMappingDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("UploadMasterUserInfo") );

	pMappingDM->StartFrame();
	pMappingDM->InitDBFieldData();

	CString sSrcField , sDstField , sType , sData;
	for( INT i = 0 ; i < pMappingDM->GetRowCount() ; i++ )
	{
		pMappingDM->GetCellData( _T("�ҽ��ʵ�") , i , sSrcField );
		pMappingDM->GetCellData( _T("�����ʵ�") , i , sDstField );
		pMappingDM->GetCellData( _T("Ÿ��")		, i , sType );
		
		pLocalUserDM->GetCellData( sSrcField , 0 , sData );

		if( sType.Compare(_T("NUMERIC")) == 0 && sData.IsEmpty() )
			sData = _T("NULL");
		

		pMappingDM->AddDBFieldData( sDstField , sType , sData , TRUE );
	}

	/// RecKey ���� --------------------------------
	ids = CShareInfoCenterUp::MakeCenterRecKey(sMasterUserKey);
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	pMappingDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sMasterUserKey );
	/// --------------------------------------------

	/// ������ �ʵ� ����
	pMappingDM->AddDBFieldData( _T("MAKE_LIB_CODE") , _T("STRING") , m_sLibCode );
	pMappingDM->AddDBFieldData( _T("MAKE_DATE")		, _T("NUMERIC") , _T("SYSDATE") );
	pMappingDM->AddDBFieldData( _T("LAST_UPDATE_LIB_CODE")		, _T("STRING") , m_sLibCode );
	pMappingDM->AddDBFieldData( _T("LAST_UPDATE_DATE ")		, _T("NUMERIC") , _T("SYSDATE") );
	
	CString sFirstWork;
	sFirstWork = CESL_Mgr::GetWorkLogMsg( _T("[1005]") , __WFILE__ , __LINE__ );
	pMappingDM->AddDBFieldData( _T("FIRST_WORK")		, _T("STRING") , sFirstWork );


	CString sInsertQuery;
	/// Insert���� �����
	pMappingDM->MakeInsertFrameQuery( sTableName , sInsertQuery);

	/// Insert ����

	/*
	POSITION pos = pMappingDM->frame.FindIndex(0);
	sInsertQuery = *((CString*)(pMappingDM->frame.GetAt(pos)));
	*/

	/// ���� ���� ���� ������ --------------------
//	ids = m_pCS_SQL->LOAN_UnSelect( sInsertQuery , _ENCRYPT );	
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	/// --------------------------------------------

	pMappingDM->EndFrame();
	
		
	return 0;
	
	EFS_END
	return -1;
	
}

/// [1006][1012]  ���� ������ ���� �̷¿� INSERT
INT CLoanUserCenterUp::RecordMasterUserHistory( CString sMasterUserKey )
{
	EFS_BEGIN

	INT ids;

	CString sTableName = _T("CT_MASTER_USER_HISTORY_TBL");
	
	CESL_DataMgr* pMasterLoanUserDM = FindDM( _T("DM_MASTER_LOAN_USER_INFO") );
	if( pMasterLoanUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RecordMasterUserHistory") );

	/// ���ͷ� ������ -> ���ľ� �� [ ������ ]
	CString sWhere;
	sWhere.Format( _T("REC_KEY='%s'") , sMasterUserKey );
	
//	m_pCS_SQL->SetFetchCount( 0, 100 );
//	ids = m_pCS_SQL->LOAN_Select( pMasterLoanUserDM , sWhere , pMasterLoanUserDM->DB_MGR_ALIAS , 10 , 10 , _ENCRYPT );
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}

	if( pMasterLoanUserDM->GetRowCount() == 0 ) return 10005;
	
	
	/// ���� �ʵ� ����
	CESL_DataMgr* pMappingDM = m_pMasterUserHistoryMapping->GetMappingInfo();
	if( pMappingDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RecordMasterUserHistory") );
	
	pMappingDM->StartFrame();
	pMappingDM->InitDBFieldData();
	
	CString sSrcField , sDstField , sType , sData;
	for( INT i = 0 ; i < pMappingDM->GetRowCount() ; i++ )
	{
		pMappingDM->GetCellData( _T("�ҽ��ʵ�") , i , sSrcField );
		pMappingDM->GetCellData( _T("�����ʵ�") , i , sDstField );
		pMappingDM->GetCellData( _T("Ÿ��")		, i , sType );
		
		pMasterLoanUserDM->GetCellData( sSrcField , 0 , sData );

		if( sType.Compare(_T("NUMERIC")) == 0 && sData.IsEmpty() )
			sData = _T("NULL");
		
		
		pMappingDM->AddDBFieldData( sDstField , sType , sData , TRUE );
	}
	
	/// RecKey ���� --------------------------------
	CString sRecKey;
	ids = CShareInfoCenterUp::MakeCenterRecKey( sRecKey );
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}

	pMappingDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	/// --------------------------------------------
	
	/// ������ �ʵ� ����
	pMappingDM->AddDBFieldData( _T("CHANGE_DATE ")		, _T("NUMERIC") , _T("SYSDATE") );

	CString sFirstWork;
	sFirstWork = CESL_Mgr::GetWorkLogMsg( _T("[1006]") , __WFILE__ , __LINE__ );
	pMappingDM->AddDBFieldData( _T("FIRST_WORK")		, _T("STRING") , sFirstWork );
	
	
	/// Insert���� �����
#ifdef _H_DEBUG
	OnShowDMViewer(0);
#endif


	CString sInsertQuery;
	
	pMappingDM->MakeInsertFrameQuery( sTableName , sInsertQuery );
	
	/*
	/// Insert ����
	POSITION pos = pMappingDM->frame.FindIndex(0);
	sInsertQuery = *((CString*)(pMappingDM->frame.GetAt(pos)));
	*/
	
	/// ���� ���� ���� ������ --------------------
//	ids = m_pCS_SQL->LOAN_UnSelect( sInsertQuery , _ENCRYPT );
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	/// --------------------------------------------
	
	pMappingDM->EndFrame();
	
	return 0;
	
	EFS_END
	return -1;
	
}

/// [1007] �������� ������ ������ ó���Ϸ��� �̿��ڰ� �����ϴ°�?
INT CLoanUserCenterUp::IsLibUserInfo( CString &sLibUserKey )
{
	EFS_BEGIN

	INT ids;

	CString sClientUserKey;

	sClientUserKey = m_ShareInfo.sUpdatedTableRecKey;
	
	CString sSql;
	sSql.Format(	_T("SELECT REC_KEY FROM ")
					_T("CT_LIBRARY_LOAN_USER_TBL ")
					_T("WHERE ")
					_T("CLIENT_REC_KEY = %s ")
					_T("AND ")
					_T("LIB_CODE = '%s' ")
					, sClientUserKey 
					, m_sLibCode
					);

	/// ���Ϳ� ���� ������
//	ids = m_pCS_SQL->LOAN_GetOneValue( sSql , sLibUserKey , _ENCRYPT );
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}

	if( !sLibUserKey.IsEmpty() ) return 1;
		
	return 0;
	
	EFS_END
	return -1;
	
}

/// [1008] �������� ������ ���� ����
INT CLoanUserCenterUp::DeleteLibUserInfo( CString sLibUserKey )
{
	EFS_BEGIN

	INT ids;

	CString sSql;
	sSql.Format( 
		_T("DELETE FROM ")
		_T("CT_LIBRARY_LOAN_USER_TBL ")
		_T("WHERE ")
		_T("REC_KEY = %s ;")
		,
		sLibUserKey
		);
		
	/// ���Ϳ� ���� ������
//	ids = m_pCS_SQL->LOAN_UnSelect( sSql , _ENCRYPT );
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}

	/// ���õ� ����/�ݳ� ���� ����
	sSql.Format( 
		_T("DELETE FROM ")
		_T("CT_CO_LOAN_INFO_TBL ")
		_T("WHERE ")
		_T("USER_KEY = %s AND LIB_CODE='%s' ;")
		,
		m_ShareInfo.sUpdatedTableRecKey
		, m_sLibCode
		);
//	ids = m_pCS_SQL->LOAN_UnSelect( sSql , _ENCRYPT );
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	
	return 0;
	
	EFS_END
	return -1;
	
}

/// [1009] �������� ������ ���� INSERT
INT CLoanUserCenterUp::UploadLibUserInfo( CString sMasterUserKey )
{
	EFS_BEGIN

	INT ids;
	
	CString sTableName = _T("CT_LIBRARY_LOAN_USER_TBL");
	
	CESL_DataMgr* pLocalUserDM = FindDM( _T("DM_CLIENT_USER_INFO") );
	if( pLocalUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("UploadMasterUserInfo") );

	
	/// ���� �ʵ� ����
	CESL_DataMgr* pMappingDM = m_pLibUserMapping->GetMappingInfo();
	if( pMappingDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("UploadMasterUserInfo") );
	
	pMappingDM->StartFrame();
	pMappingDM->InitDBFieldData();
	
	CString sSrcField , sDstField , sType , sData;
	for( INT i = 0 ; i < pMappingDM->GetRowCount() ; i++ )
	{
		pMappingDM->GetCellData( _T("�ҽ��ʵ�") , i , sSrcField );
		pMappingDM->GetCellData( _T("�����ʵ�") , i , sDstField );
		pMappingDM->GetCellData( _T("Ÿ��")		, i , sType );
		
		pLocalUserDM->GetCellData( sSrcField , 0 , sData );

		if( sType.Compare(_T("NUMERIC")) == 0 && sData.IsEmpty() )
			sData = _T("NULL");
		
		
		pMappingDM->AddDBFieldData( sDstField , sType , sData , TRUE );

		m_asUpdatedFields.Add( sSrcField );
	}
	
	/// RecKey ���� --------------------------------
	CString sRecKey;
	ids = CShareInfoCenterUp::MakeCenterRecKey(sRecKey);
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	pMappingDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	/// --------------------------------------------
	
	/// ������ �ʵ� ����
	pMappingDM->AddDBFieldData( _T("LIB_CODE")		,_T("STRING")	, m_sLibCode );
	pMappingDM->AddDBFieldData( _T("MASTER_USER_KEY")		, _T("NUMERIC") , sMasterUserKey );
	
	CString sFirstWork;
	sFirstWork = CESL_Mgr::GetWorkLogMsg( _T("[1009]") , __WFILE__ , __LINE__ );
	pMappingDM->AddDBFieldData( _T("FIRST_WORK")	, _T("STRING") , sFirstWork );
	

	CString sInsertQuery;
	/// Insert���� �����
	pMappingDM->MakeInsertFrameQuery( sTableName , sInsertQuery );
	
	/*
	/// Insert ����
	POSITION pos = pMappingDM->frame.FindIndex(0);
	sInsertQuery = *((CString*)(pMappingDM->frame.GetAt(pos)));
	*/
	
	/// ���� ���� ���� ������ --------------------
//	ids = m_pCS_SQL->LOAN_UnSelect( sInsertQuery , _ENCRYPT );	
	if( ids != 0 ) return 10006;
	/// --------------------------------------------
	
	pMappingDM->EndFrame();
	
	// [1009(2)] ���� �̹��� UPLOAD
	ids = InsertLocalUserImageCenterUp(0,sRecKey);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertUser") );
	if( ids > 0 ) return ids;
	
	m_asUpdatedFields.Add( _T("USER_PICTURE_TYPE") );
		
	return 0;
	
	EFS_END
	return -1;
	
}


INT CLoanUserCenterUp::UpdateLocalUserToCenter()
{
	EFS_BEGIN

	INT ids;

	
	/// ���� ������ ���� KEY
	CString sMasterUserKey;
	
	// 1. [1101][1004] ������ ���� ������ ������ ó���Ϸ��� �̿��ڰ� �����ϴ°�?
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::IsCenterMasterUser() [START]") );
	ids = IsCenterMasterUser(sMasterUserKey);
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::IsCenterMasterUser() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertLocalUserToCenter") );
	if( ids > 1 ) return ids;
	
	if( ids == 1 )
	{
		// 2. [1102][1005] ���� ������ ������ INSERT
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UploadMasterUserInfo() [START]") );
		ids = UploadMasterUserInfo(sMasterUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UploadMasterUserInfo() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertLocalUserToCenter") );
		if( ids > 0 ) return ids;
		
		// 3. ���� �̹��� UPLOAD
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::InsertLocalUserImageCenterUp() [START]") );
		ids = InsertLocalUserImageCenterUp(1,sMasterUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::InsertLocalUserImageCenterUp() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertUser") );
		if( ids > 0 ) return ids;
		
		
		// 4. [1104][1016] ���� ������ ���� �̷¿� INSERT
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::RecordMasterUserHistory() [START]") );
		ids = RecordMasterUserHistory(sMasterUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::RecordMasterUserHistory() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertLocalUserToCenter") );
		if( ids > 0 ) return ids;
		
		// ���� ������ ������ INSERT�� �Ǿ��ٸ� ������ �� ������ �ʿ�� ����.
		m_ShareInfo.sMasterUserInfoChangeYN = _T("N");
	}
	
	
	/// �������� ������ ����KEY
	CString sLibUserKey;
	
	// 3. [1103][1007] �������� ������ ������ ó���Ϸ��� �̿��ڰ� �����ϴ°�?
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::IsLibUserInfo() [START]") );
	ids = IsLibUserInfo(sLibUserKey);
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::IsLibUserInfo() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertLocalUserToCenter") );
	if( ids > 1 ) return ids;
	
	// ���ٸ�
	if( ids == 0 )
	{
		// 5. [1105][1009] �������� ������ ���� INSERT
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UploadLibUserInfo() [START]") );
		ids = UploadLibUserInfo(sMasterUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UploadLibUserInfo() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("InsertLocalUserToCenter") );
		if( ids > 0 ) return ids;
	}
	else
	{
		// 6. [1106] �������� ������ ���� UPDATE
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UpdateLibUserInfo() [START]") );
		ids = UpdateLibUserInfo(sLibUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UpdateLibUserInfo() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("InsertLocalUserToCenter") );
		if( ids > 0 ) return ids;
	}
	
	// 7. [1010] ���� ������ ���� ���ΰ� 'Y'�ΰ�?
	if( m_ShareInfo.sMasterUserInfoChangeYN.Compare(_T("Y")) == 0 )
	{
		// 8. [1011] ���� ������ ������ UPDATE
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UpdateMasterUserInfo() [START]") );
		ids = UpdateMasterUserInfo(sMasterUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::UpdateMasterUserInfo() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("InsertLocalUserToCenter") );
		if( ids > 0 ) return ids;
		
		// 9. [1012] ���� ������ ���� �̷¿� INSERT
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::RecordMasterUserHistory() [START]") );
		ids = RecordMasterUserHistory(sMasterUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::RecordMasterUserHistory() [END]") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("InsertLocalUserToCenter") );
		if( ids > 0 ) return ids;
	}
	

	return 0;

	EFS_END

	return -1;

}


INT CLoanUserCenterUp::UpdateLibUserInfo( CString sLibUserKey )
{
	EFS_BEGIN

	
	INT ids;
	
	CString sTableName = _T("CT_LIBRARY_LOAN_USER_TBL");
	
	CESL_DataMgr* pLocalUserDM = FindDM( _T("DM_CLIENT_USER_INFO") );
	if( pLocalUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("UploadMasterUserInfo") );
	
	
	/// ���� �ʵ� ����
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::m_pLibUserMapping->GetMappingInfo [START]") );
	CESL_DataMgr* pMappingDM = m_pLibUserMapping->GetMappingInfo();
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::m_pLibUserMapping->GetMappingInfo [END]") );
	if( pMappingDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("UploadMasterUserInfo") );

	// Update�� �ʵ�
	// �̹��� ������ ������Ʈ �Ѱ����� Ȯ�� 
//	for( INT j = 0 ; j < m_asUpdatedFields.GetSize() ; j++ )
//	{
//		if( m_asUpdatedFields.GetAt(j).Compare(_T("USER_PICTURE_TYPE")) == 0 )
//		{
			// [1009(2)] ���� �̹��� UPLOAD
	

//			return 0;
//		}
//	}
	
	pMappingDM->StartFrame();
	pMappingDM->InitDBFieldData();
	
	CString sSrcField , sDstField , sType , sData;
	for( INT i = 0 ; i < pMappingDM->GetRowCount() ; i++ )
	{
		pMappingDM->GetCellData( _T("�ҽ��ʵ�") , i , sSrcField );
		pMappingDM->GetCellData( _T("�����ʵ�") , i , sDstField );
		pMappingDM->GetCellData( _T("Ÿ��")		, i , sType );

		if( !IsInUpdatedFields(m_asUpdatedFields,sSrcField) ) continue;
		
		pLocalUserDM->GetCellData( sSrcField , 0 , sData );

		if( sType.Compare(_T("NUMERIC")) == 0 && sData.IsEmpty() )
			sData = _T("NULL");		
		
		pMappingDM->AddDBFieldData( sDstField , sType , sData , TRUE );
	}
	
	/// ������ �ʵ� ����
	if( IsInUpdatedFields(m_asUpdatedFields,_T("LIB_CODE")) )
		pMappingDM->AddDBFieldData( _T("LIB_CODE")		,_T("STRING")	, m_sLibCode );
	
	CString sFirstWork;
	sFirstWork = CESL_Mgr::GetWorkLogMsg( _T("[1009]") , __WFILE__ , __LINE__ );
	pMappingDM->AddDBFieldData( _T("LAST_WORK")	, _T("STRING") , sFirstWork );
	

	CString sInsertQuery;

	/// Insert���� �����
	pMappingDM->MakeUpdateFrameQuery( sTableName , _T("REC_KEY") , _T("NUMERIC") , sLibUserKey , sInsertQuery);
	
/*
	/// Insert ����
	POSITION pos = pMappingDM->frame.FindIndex(0);
	sInsertQuery = *((CString*)(pMappingDM->frame.GetAt(pos)));
*/	
	/// ���� ���� ���� ������ --------------------
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::LOAN_UnSelect() [START]") );
//	ids = m_pCS_SQL->LOAN_UnSelect( sInsertQuery , _ENCRYPT );	
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::LOAN_UnSelect() [END]") );
	if( ids != 0 ) return 10006;
	/// --------------------------------------------
	
	pMappingDM->EndFrame();

	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::InsertLocalUserImageCenterUp() [START]") );
	ids = InsertLocalUserImageCenterUp(0,sLibUserKey);
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::InsertLocalUserImageCenterUp() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertUser") );
	if( ids > 0 ) return ids;
	
	return 0;
	

	EFS_END

	return -1;
}

// [1108]
INT CLoanUserCenterUp::UpdateMasterUserInfo( CString sMasterUserKey )
{
	EFS_BEGIN

		
	INT ids;
	
	CString sTableName = _T("CT_MASTER_LOAN_USER_TBL");
	
	CESL_DataMgr* pLocalUserDM = FindDM( _T("DM_CLIENT_USER_INFO") );
	if( pLocalUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("UploadMasterUserInfo") );
	
	
	/// ���� �ʵ� ����
	CESL_DataMgr* pMappingDM = m_pMasterUserMapping->GetMappingInfo();
	if( pMappingDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("UploadMasterUserInfo") );
	
	// Update�� �ʵ�
	// �̹��� ������ ������Ʈ �Ѱ����� Ȯ�� 
//	for( INT j = 0 ; j < m_asUpdatedFields.GetSize() ; j++ )
//	{
//		if( m_asUpdatedFields.GetAt(j).Compare(_T("USER_PICTURE_TYPE")) == 0 )
//		{
			
//			return 0;
//		}
//	}
	
	pMappingDM->StartFrame();
	pMappingDM->InitDBFieldData();
	
	CString sSrcField , sDstField , sType , sData;
	for( INT i = 0 ; i < pMappingDM->GetRowCount() ; i++ )
	{
		pMappingDM->GetCellData( _T("�ҽ��ʵ�") , i , sSrcField );
		pMappingDM->GetCellData( _T("�����ʵ�") , i , sDstField );
		pMappingDM->GetCellData( _T("Ÿ��")		, i , sType );
		
		if( !IsInUpdatedFields(m_asUpdatedFields,sSrcField) ) continue;

		pLocalUserDM->GetCellData( sSrcField , 0 , sData );

		if( sType.Compare(_T("NUMERIC")) == 0 && sData.IsEmpty() )
			sData = _T("NULL");
		
		pMappingDM->AddDBFieldData( sDstField , sType , sData , TRUE );
	}
	
	/// ������ �ʵ� ����
	pMappingDM->AddDBFieldData( _T("LAST_UPDATE_LIB_CODE")		, _T("STRING") , m_sLibCode );
	pMappingDM->AddDBFieldData( _T("LAST_UPDATE_DATE ")		, _T("NUMERIC") , _T("SYSDATE") );
	
	CString sFirstWork;
	sFirstWork = CESL_Mgr::GetWorkLogMsg( _T("[1009]") , __WFILE__ , __LINE__ );
	pMappingDM->AddDBFieldData( _T("LAST_WORK")	, _T("STRING") , sFirstWork );
	
	CString sInsertQuery;

	/// Insert���� �����
	pMappingDM->MakeUpdateFrameQuery( sTableName , _T("REC_KEY") , _T("NUMERIC") , sMasterUserKey , sInsertQuery);

/*	
	/// Insert ����
	POSITION pos = pMappingDM->frame.FindIndex(0);
	sInsertQuery = *((CString*)(pMappingDM->frame.GetAt(pos)));
*/	
	/// ���� ���� ���� ������ --------------------
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::LOAN_UnSelect() [START]") );
//	ids = m_pCS_SQL->LOAN_UnSelect( sInsertQuery , _ENCRYPT );	
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::LOAN_UnSelect() [END]") );
	if( ids != 0 ) return 10006;
	/// --------------------------------------------
	
	pMappingDM->EndFrame();
	
	// [1009(2)] ���� �̹��� UPLOAD
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::InsertLocalUserImageCenterUp() [START]") );
	ids = InsertLocalUserImageCenterUp(1,sMasterUserKey);
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanUserCenterUp::InsertLocalUserImageCenterUp() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertUser") );
	if( ids > 0 ) return ids;
	
	return 0;

	EFS_END

	return -1;
	
}
