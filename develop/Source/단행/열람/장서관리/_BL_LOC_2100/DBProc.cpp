// DBProc.cpp: implementation of the CDBProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBProc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBProc::CDBProc( CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	m_sResultTableName = _T("BO_BOOK_CHK_RESULT_TBL");

	// �ٸ� �ڷ�ǿ� �����ϴ� �ڷḦ üũ�ϱ� ���� DM
	m_sTmpEtcInfoDMAlias = _T("DM_BO_LOC_2100_ETC_TMP");

}

CDBProc::~CDBProc()
{

}

// sBookDMAlias DM�� sWhere���� ������ DB���� ���� ���´�.
INT CDBProc::MakeDMFromDB( CString sBookDMAlias , CString sWhere , CBLMapData_Mem* pMapData , CESL_DataMgr*		pDM_Other )
{
	EFS_BEGIN

	INT ids;


	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sBookDMAlias );

	// ����� DM 
	if( pDM_Other != NULL )
	{
		pDM = pDM_Other;
	}

	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , - 1 , _T("MakeDMFromDB( CString sBookDMAlias , CString sWhere )") );

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	// CBLMapData_Mem ������ DM �����ؼ� pDM �� �ִ´�.
	// �̷��� �����ڵ��� pDM �� �ƹ��� ��ȭ�� ����. ( ���� pDM �� Row ������ �����Ϸ���...)
	CESL_DataMgr DM_Src_Copy;
	if( pMapData != NULL )
	{
		CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , &DM_Src_Copy );
		// pDM �� �ִ´�. ���纻�� ����ؼ� ���� ������ Row �޴´�.
		pDM = &DM_Src_Copy;
	}
	// KOL.RED.2018.201 ---------------------------------------------------------------------------


	CESL_DataMgr SEDM;
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , &SEDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , - 2 , _T("MakeDMFromDB( CString sBookDMAlias , CString sWhere )") ); 

	SEDM.TBL_NAME = _T("SE_BOOK_TBL BB , IDX_SE_TBL IB , SE_VOL_TBL V");	
	
	SEDM.CONDITION = _T("BB.VOL_KEY=V.REC_KEY AND BB.SPECIES_KEY=IB.REC_KEY AND BB.BOOK_TYPE IN ('B','C') ");

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	for( INT k = 0 ; k < SEDM.RefList.GetCount() ; k++ )
	{
		sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = SEDM.GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeDMFromDB") );
		
		if( sFieldAlias.Compare(_T("����/���ӱ���")) == 0 )
		{
			ids = SEDM.SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , _T("SE")  , sDataType );			
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") );
		}
		else if( sFieldAlias.Compare(_T("����")) == 0 )
		{
			ids = SEDM.SetDataMgrRef( k , sFieldAlias , _T("V.PRICE") , sFieldType  , sInitValue  , sDataType );			
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") );
		}
		else if( sFieldAlias.Compare(_T("�����")) == 0 )
		{
			ids = SEDM.SetDataMgrRef( k , sFieldAlias , _T("IB.PUB_YEAR") , sFieldType  , sInitValue , sDataType );			
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") );
		}
	}		
	
	ids = SEDM.RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , - 5 , _T("MakeDMFromDB( CString sBookDMAlias , CString sWhere )") ); 
	
	CString sWhere2;
	sWhere2.Format( _T("%s AND BB.MEDIA_CODE <> 'OF' "),sWhere);

	ids = pDM->RefreshDataManager( sWhere2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , - 6 , _T("MakeDMFromDB( CString sBookDMAlias , CString sWhere )") ); 

	ids = CLocCommonAPI::AddDM( &SEDM , pDM , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , - 7 , _T("MakeDMFromDB( CString sBookDMAlias , CString sWhere )") ); 
	
	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	// ���� pDM �� �ִ� Row �����͵��� �޾ƿ���
	if( pMapData != NULL )
	{
		pMapData->Add_SelectQuery( pDM , _T("��Ϲ�ȣ") );
	}
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	return 0;

	EFS_END
	return -1;

}


// Where���� �������ִ� ��ƾ
INT CDBProc::MakeSql(  CString *sGetData , TCHAR **sDBFieldName , INT *nDBFieldType , INT nCnt , CString &sWhere )
{
	EFS_BEGIN

	INT ids;
	ids = CLocCommonAPI::MakeSql( sGetData , sDBFieldName , nDBFieldType , nCnt , sWhere , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeSql( TCHAR **ControlFiedlAlias , TCHAR **sDBFieldName , TCHAR **sDBFieldType , CString &sWhere )") );

	return 0;

	EFS_END
	return -1;

}

INT CDBProc::IsRegNo( CESL_DataMgr *pDM , CString sRegNo , CString sShelfLocCode , CString &sPublishForm  )
{
	EFS_BEGIN

	INT ids;

	CString sSql;
	CString sAddSql;
	CString sDefaultAddSql;
	sDefaultAddSql.Format( _T("BB.REG_NO='%s' AND BB.WORKING_STATUS IN (")_T(HOLD_SEARCH_CONDITION)_T(",")_T(HOLD_SE_SEARCH_CONDITION)_T(") ")
						, sRegNo );
	
	if( !sShelfLocCode.IsEmpty() ) sAddSql.Format(_T(" AND BB.SHELF_LOC_CODE='%s'"), sShelfLocCode) ;
/*
	sSql.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE ") );

	sSql += sDefaultAddSql + sAddSql;

	CString sResult;
	
	ids = pDM->GetOneValueQuery( sSql , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("IsRegNo( CESL_DataMgr *pDM , CString sRegNo , CString sShelfLocCode )") );

	if( _ttoi( sResult ) > 0 ) return 0;	// �����ڷ�
*/


	CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM( m_sTmpEtcInfoDMAlias );
	if( pTmpDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsRegNo( CESL_DataMgr *pDM , CString sRegNo , CString sShelfLocCode )") );

	// ���� �ڷ� ����
	sPublishForm = _T("MO");
	pTmpDM->TBL_NAME = _T("BO_BOOK_TBL BB , IDX_BO_TBL IB");

	ids = pTmpDM->RefreshDataManager(sDefaultAddSql);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsRegNo( CESL_DataMgr *pDM , CString sRegNo , CString sShelfLocCode )") );

	if( pTmpDM->GetRowCount() < 1 )
	{
		// ���� �ڷ� ����
		pTmpDM->TBL_NAME = _T("SE_BOOK_TBL BB , IDX_SE_TBL IB");
		
		ids = pTmpDM->RefreshDataManager(sDefaultAddSql);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsRegNo( CESL_DataMgr *pDM , CString sRegNo , CString sShelfLocCode )") );

		sPublishForm = _T("SE");
	}

	for( INT i = 0 ; i < pTmpDM->GetRowCount() ; i++ )
	{
		pTmpDM->SetCellData( _T("����/���ӱ���") , sPublishForm , i);
	}

	if( pTmpDM->GetRowCount() < 1 )
		return 2;	// �н� ��� // DB�� �������� �ʴ� �������̴�.

	if( sShelfLocCode.IsEmpty() ) return 0; // ���� �ڷ� - ���� �˻��ϰ� �ִ� �ڷ�ǿ� �����Ѵ�.

	CString sTmpShelfLocCode;
	ids = pTmpDM->GetCellData( _T("�ڷ��") , 0 , sTmpShelfLocCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("IsRegNo( CESL_DataMgr *pDM , CString sRegNo , CString sShelfLocCode )") );
	sTmpShelfLocCode.TrimLeft(); sTmpShelfLocCode.TrimRight();

	if( sShelfLocCode.Compare(sTmpShelfLocCode)==0 )
		return 0; // ���� �ڷ� - ���� �˻��ϰ� �ִ� �ڷ�ǿ� �����Ѵ�.
	else
		return 1; // �����ڷ� - ���� �˻��ϰ� �ִ� �ڷ�ǿ��� �������� �ʴ´�.

	EFS_END
	return -1;

}

// bIsRegNo : TRUE -> ��Ϲ�ȣ��..
//            FALSE -> åKEY��
INT CDBProc::ChangeBookCheckYN( CESL_DataMgr *pDM , CString sCheck , CString sRegNo , CString sPublishForm , bool bIsRegNo )
{
	EFS_BEGIN

	INT ids;

	CString sTableName;
	if( sPublishForm.Compare(_T("MO")) == 0 )
		sTableName = _T("BO_BOOK_TBL");
	else if( sPublishForm.Compare(_T("SE")) == 0 )
		sTableName = _T("SE_BOOK_TBL");

	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("BOOK_CHECK_YN") , _T("STRING") , sCheck );
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );
	if( bIsRegNo )
		pDM->MakeUpdateFrame( sTableName , _T("REG_NO") , _T("STRING") , sRegNo );
	else
		pDM->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRegNo );

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeBookCheckYN") );

	pDM->EndFrame();
	pDM->InitDBFieldData();

	return 0;

	EFS_END
	return -1;

}

// �弭 ���� ��ȣ�� ���� ū ���˹�ȣ ������ �Լ�.
INT CDBProc::GetMaxCheckNumber( CESL_DataMgr *pDM , CString &sCheckNumber )
{
	EFS_BEGIN

	INT ids;

	CString sSql;
	CString sCurrentYear;
	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	sCurrentYear.Format( _T("%d") , (CLocCommonAPI::GetCurrentDateTime(m_pParentMgr,strGetData)).GetYear() );
// 	sCurrentYear.Format( _T("%d") , (CLocCommonAPI::GetDBTime(m_pParentMgr)).GetYear() );
	sSql.Format(_T("SELECT MAX(CHK_SEQ_NO) FROM BO_BOOK_CHK_RESULT_TBL WHERE CHK_YEAR='%s' AND MANAGE_CODE = UDF_MANAGE_CODE") , sCurrentYear );

	ids = pDM->GetOneValueQuery( sSql , sCheckNumber );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetMaxCheckNumber") );

	return 0;

	EFS_END
	return -1;

}

INT CDBProc::MakeCheckNoDMToDB()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_2100_BOOK_CHECK_RESULT_LIST");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeCheckNoDMToDB") );

	pDM->FreeData();
	
	ids = pDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeCheckNoDMToDB") ); 

	///@20071105 PDW
	///@{{++
	///@K3 �䱸���� ����
	///@�ڷ���� �ټ� ������ �� �ֵ��� ����Ǿ���
	/*//before	
	m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("�ڷ�Ǳ���") , _T("����ڷ��") );
	*///after
	INT		nRowCount = pDM->GetRowCount();
	CString strData;
	for( INT i=0;i<nRowCount;i++ )
	{
		ids = pDM->GetCellData(_T("����ڷ��"), i, strData);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeCheckNoDMToDB") ); 
		strData.TrimLeft();strData.TrimRight();
		strData.Replace(_T(" "), _T(""));
		
		CStringArray pArray;
		CLocCommonAPI::MakeArrayList( strData, _T(","), pArray);

		CString strDesc;
		CString strShelfLocCodeDesc;
		for( INT j=0;j<pArray.GetSize();j++ )
		{
			m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�ڷ�Ǳ���"), pArray.GetAt(j), strDesc);

			if( !strShelfLocCodeDesc.IsEmpty() )
				strShelfLocCodeDesc += _T(" , ");

			strShelfLocCodeDesc += strDesc;
		}

		ids = pDM->SetCellData(_T("����ڷ��"), strShelfLocCodeDesc, i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeCheckNoDMToDB") ); 
	}
	///@--}}	

	return 0;

	EFS_END
	return -1;

}

// �н� å ���� �����۾�
INT CDBProc::UpdateLostBookWorkingStatus( CESL_DataMgr *pDM , CString sShelfLocCode , CString sChangeWorkingStatus )
{
	EFS_BEGIN

	INT ids;

	CString sUpdateFrame;

	// �ʱ� ȭ���� ����� �ڷ��� ó���� ���� ó���� �Ѵ�.
	if( sChangeWorkingStatus.Compare(_T("�������"))==0 ) return 0;

	ids = CLocCommonAPI::GetWorkingStatusDescToCode( sChangeWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("UpdateLostBookWorkingStatus") ); 

	// ���� -> �н�
	CString sDefaultSql = _T("UPDATE BO_BOOK_TBL ")
		_T("SET WORKING_STATUS='") + sChangeWorkingStatus + _T("' ")
		_T(", LAST_SHELF_STATUS_CHANGE_DATE = SYSDATE ")
		_T("WHERE BOOK_CHECK_YN='N' ")
		_T("AND BOOK_APPENDIX_FLAG='B' AND MANAGE_CODE = UDF_MANAGE_CODE ")
		_T("AND WORKING_STATUS IN (")+CString(HOLD_SEARCH_CONDITION)+_T(")");

	sUpdateFrame = sDefaultSql + _T(";");
	
	if( !sShelfLocCode.IsEmpty() )
	{
		sUpdateFrame.Format( _T("%s AND SHELF_LOC_CODE IN (%s) ;") , sDefaultSql , sShelfLocCode );
	}

	pDM->StartFrame();

	pDM->AddFrame( sUpdateFrame );

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("UpdateLostBookWorkingStatus") ); 

	pDM->EndFrame();

	// �н� -> ����
	// �̰� �� �ʿ䰡 ����.
	/*
	sDefaultSql = _T("UPDATE BO_BOOK_TBL ")
		_T("SET WORKING_STATUS='BOL112N' ")
		_T(", LAST_SHELF_STATUS_CHANGE_DATE = sysdate ")
		_T("WHERE BOOK_CHECK_YN='Y' ")
		_T("AND BOOK_APPENDIX_FLAG='B' ")
		_T("AND WORKING_STATUS IN ('BOL252N') ");

	sUpdateFrame = sDefaultSql + _T(";");
	
	pDM->AddFrame( sUpdateFrame );

	if( !sShelfLocCode.IsEmpty() )
	{
		sUpdateFrame.Format( _T("%s AND SHELF_LOC_CODE='%s' ;") , sDefaultSql , sShelfLocCode );
	}
	*/



	return 0;

	EFS_END
	return -1;

}

// �弭 ���� ���̺��� ���ڵ带 �μ�Ʈ�Ͽ� REC_KEY�� ���´�.
INT CDBProc::MakeBookCheckResultRecord( CESL_DataMgr *pDM , CString sCheckNo , CString sCheckYear , CString &sRecKey )
{
	EFS_BEGIN

	INT ids;

	pDM->StartFrame();

	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("CHK_SEQ_NO") , _T("NUMERIC") , sCheckNo );
	pDM->AddDBFieldData( _T("CHK_YEAR") , _T("STRING") , sCheckYear );
	pDM->AddDBFieldData( _T("START_DATE") , _T("NUMERIC") , _T("SYSDATE") );

	ids = pDM->MakeRecKey( sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResultRecord") ); 

	pDM->AddDBFieldData( _T("REC_KEY") , _T("STRING") , sRecKey );

	pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );
	
	pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") );

	ids = pDM->MakeInsertFrame( m_sResultTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeBookCheckResultRecord") ); 

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeBookCheckResultRecord") ); 

	pDM->InitDBFieldData();
	pDM->EndFrame();

	return 0;

	EFS_END
	return -1;

}

INT CDBProc::GetDBForFile( CESL_DataMgr *pDM , INT nMode )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr SEDM;

	CString sShelfLocCode;

	// 2009.02.25 UPDATE BY PDJ : �ڷ���� ������ �Է¹��� �� �ֵ��� ����	
//	CString sCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");
//	CString sControlAlias = _T("�ڷ�Ǹ�");
//
//	ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias , sShelfLocCode , -1 , 0 );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetDBForFile()") ); 
//	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	sShelfLocCode = GetShelfLocCode();
	

	CString sWhere;
	sWhere.Format( _T(" WORKING_STATUS IN (")_T(HOLD_SEARCH_CONDITION)_T(",")_T(HOLD_SE_SEARCH_CONDITION)_T(") "));
	if( !sShelfLocCode.IsEmpty() ) 
		sWhere += _T(" AND SHELF_LOC_CODE IN (") + sShelfLocCode + _T(")");

	if( nMode == 0 )
		sWhere += _T(" AND BOOK_CHECK_YN = 'Y'");
	else
		sWhere += _T(" AND BOOK_CHECK_YN = 'N'");

	// 2009.02.25 ADD BY PDJ : MANAGE_CODEó�� �߰�
	sWhere += _T("AND MANAGE_CODE=UDF_MANAGE_CODE");

	pDM->FreeData();

	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , &SEDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetDBForFile()") ); 

	SEDM.TBL_NAME = _T("SE_BOOK_TBL");

	/// 2004 �� 05�� 12�� ������ ���� ��ü ������ OF �� �ڷ�� �弭���� ��� �ڷῡ�� ���ܽ�Ų��.
	CString sWhere2;
	sWhere2.Format( _T("%s AND MEDIA_CODE <> 'OF'") , sWhere );
	ids = pDM->RefreshDataManager( sWhere2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetDBForFile()") ); 

	// ���ӵ� ���ش�.
	SEDM.CONDITION =  _T("BOOK_TYPE IN ( 'B' , 'C' )");
	ids = SEDM.RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetDBForFile()") ); 

	ids = CLocCommonAPI::AddDM( &SEDM , pDM , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("GetDBForFile()") ); 

	return 0;


	EFS_END
	return -1;

}

// ������ DB�� ���ε�
INT CDBProc::UploadResultFile( CESL_DataMgr *pDM , INT nMode , CString sFileName , CString sCheckNoKey )
{
	EFS_BEGIN

	INT ids;

	TCHAR *sDBFieldName[4] = 
	{
		_T("EXIST_LIST_FILE") , _T("LOST_LIST_FILE") , _T("ETC_LIST_FILE") , _T("ETC_LIST_FILE2")
	};
	
	// Open bitmap file and read file to pBuffer
	
	CFile file;
	ids = file.Open(sFileName, CFile::modeRead | CFile::typeBinary);
	if( ids == 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("UploadResultFile") );
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
	
	INT nFileSize = file.GetLength()-2;
	BYTE* pBuffer = (BYTE*)malloc(nFileSize+ 1024);
	file.ReadHuge(pBuffer, nFileSize);
	file.Close();

	pBuffer[nFileSize] = '\0';

	ids = pDM->LOBUpdate( m_sResultTableName , (_T("REC_KEY")), (_T("NUMERIC")), sCheckNoKey, sDBFieldName[nMode] , pBuffer, nFileSize);
	if (ids < 0) {
		AfxMessageBox(_T("Insert Fail!"));
		free (pBuffer);
		return -2;
	}
	free(pBuffer);

	return 0;

	EFS_END
	return -1;

}

//���˿Ϸᰡ �Ϸ�� ���� DB�۾� 
INT CDBProc::EndBookCheckDBProc( CESL_DataMgr *pDM , CESL_DataMgr *pDM2 , CString sCheckNoKey , CString sShelfLocCode )
{
	EFS_BEGIN

	INT ids;

	// ��� ���̺� �۾�
	// �۾��� , ���� ������ , �н� �ݾ��� UPDATE
	
	CString sUserID = m_pParentMgr->GetUserID();
	CString sWhere;
	sWhere.Format( _T("SELECT SUM(PRICE) FROM BO_BOOK_TBL WHERE BOOK_CHECK_YN='N' AND MANAGE_CODE = UDF_MANAGE_CODE ") );

	sShelfLocCode.Replace(_T("'"),_T(""));

	if( !sShelfLocCode.IsEmpty() )
	{
		//===================================================
		//2010.05.17 UPDATE BY PJW : �ڷ���� CHAR��
//		sWhere += _T(" AND SHELF_LOC_CODE IN (")+sShelfLocCode+_T(") ");
		sWhere += _T(" AND SHELF_LOC_CODE IN ( '")+sShelfLocCode+_T("' ) ");
		//===================================================	
	}

	CString sLostPrice;
	ids = pDM->GetOneValueQuery( sWhere , sLostPrice );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("EndBookCheckDBProc") );

	// ���� �ڷᵵ ó��
	sWhere.Format( _T("SELECT SUM(V.PRICE) FROM SE_BOOK_TBL B , SE_VOL_TBL V WHERE V.REC_KEY=B.VOL_KEY AND B.BOOK_CHECK_YN='N' AND B.MANAGE_CODE = UDF_MANAGE_CODE ") );
	if( !sShelfLocCode.IsEmpty() )
	{
		//===================================================
		//2010.05.17 UPDATE BY PJW : �ڷ���� CHAR��
//		sWhere += _T(" AND B.SHELF_LOC_CODE IN (")+sShelfLocCode+_T(") AND BOOK_TYPE IN ('A','B','C')");
		sWhere += _T(" AND B.SHELF_LOC_CODE IN ( '")+sShelfLocCode+_T("' ) AND BOOK_TYPE IN ('A','B','C')");
		//===================================================
		
	}
	
	CString sSELostPrice;
	ids = pDM->GetOneValueQuery( sWhere , sSELostPrice );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("EndBookCheckDBProc") );
	
	sLostPrice.Format(_T("%d") , _ttoi(sLostPrice) + _ttoi(sSELostPrice) );

	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("CHK_USER_ID") , _T("STRING") , sUserID );
	pDM->AddDBFieldData( _T("LOST_PRICE") , _T("STRING") , sLostPrice );
	pDM->AddDBFieldData( _T("LAST_MODIFY_DATE") , _T("NUMERIC") , _T("SYSDATE") );
	pDM->AddDBFieldData( _T("SHELF_LOC_CODE") , _T("STRING") , sShelfLocCode );
	
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeUpdateFrame( m_sResultTableName , _T("REC_KEY") , _T("NUMERIC") , sCheckNoKey );

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("EndBookCheckDBProc()") );

	pDM->EndFrame();

	return 0;

	EFS_END
	return -1;

}

CString CDBProc::GetShelfLocCode()
{
	CString sShelfLocCode;
	
	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)m_pParentMgr->FindControl(_T("CM_BO_LOC_2100_ETC_INFO"), _T("�ڷ������"));
	if( NULL == pCMCB )	return "";

	INT nCount = pCMCB->GetCount();

	CString strData;

	for( INT i=0;i<nCount;i++ )
	{
		if( !sShelfLocCode.IsEmpty() )
			sShelfLocCode += " , ";
				
		if( 0 > pCMCB->GetData( strData , i , 0 ) ) return "";

		strData.TrimLeft();strData.TrimRight();
		if( strData.IsEmpty() )	continue;

		sShelfLocCode += "'" + strData + "'";
	}

	return sShelfLocCode;
}