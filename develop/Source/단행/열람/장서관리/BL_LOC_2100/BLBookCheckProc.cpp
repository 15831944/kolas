// BLBookCheckProc.cpp: implementation of the CBLBookCheckProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BLBookCheckProc.h"
#include <direct.h>
#include "..\\..\\..\\..\\����\\�˻�\\BOBookDetailViewer\\BOBookDetailViewerDlg.h"
#include "..\\..\\..\\..\\����\\�˻�\\SEBookDetailViewer\\SEBookDetailViewerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define MAX_BLOB_BUFFER 20000000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBLBookCheckProc::CBLBookCheckProc( CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	
	// ȭ�� ���� �۾� CLASS ����
	m_pDBProc = new CDBProc( m_pParentMgr );
	if( m_pDBProc == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -1 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );
	
	// DB���� �۾� CLASS ����
	m_pScreenPlay = new CScreenPlay( m_pParentMgr );
	if( m_pScreenPlay == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -2 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );
	
	m_pLocCommonAPI = new CLocCommonAPI( m_pParentMgr );
	if( m_pLocCommonAPI == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -3 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );


	// �弭 ���� ��� �ڷ� DM , CM ����
	m_sBookDMAlias = _T("DM_BO_LOC_2100");
	m_sBookCMAlias = _T("CM_BO_LOC_2100");

	m_pBookDM = m_pParentMgr->FindDM( m_sBookDMAlias );
	if( m_pBookDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -4 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );
	
	m_pBookCM = m_pParentMgr->FindCM( m_sBookCMAlias );
	if( m_pBookDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -5 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );

	// �弭 ���� ��� �ڷ� �˻� CM����
	m_sSearchBookCMAlias = _T("CM_BO_LOC_2120");

	// ��Ϲ�ȣ�� ���õ� DM
	m_sParentRegNoDMAlias = _T("DM_BO_LOC_2100_REG_NO");
	m_sChildRegNoDMAlias = _T("DM_BO_LOC_2110_TMP_REG_NO");

	m_pRegNoDM = m_pParentMgr->FindDM( m_sParentRegNoDMAlias );
	if( m_pRegNoDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -6 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );

	// ��Ÿ ���� DM
	m_sInDBEtcDMAlias = _T("DM_BO_LOC_2100_ETC");
	m_pInDBEtcDM = m_pParentMgr->FindDM( m_sInDBEtcDMAlias );
	if( m_pInDBEtcDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -7 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );

	m_sNotInDBEtcDMAlias = _T("DM_BO_LOC_2100_ETC_NOT_IN_DB");
	m_pNotInDBEtcDM = m_pParentMgr->FindDM( m_sNotInDBEtcDMAlias );
	if( m_pNotInDBEtcDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -7 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );

	// �弭 ���� ��ȣ DM
	m_sCheckNumberDMAlias = _T("DM_BO_LOC_2100_BOOK_CHECK_RESULT");
	m_pCheckNumberDM = m_pParentMgr->FindDM( m_sCheckNumberDMAlias );
	if( m_pCheckNumberDM == NULL )ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -8 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );

	m_sForFileDM = _T("DM_BO_LOC_2100_FOR_MAKE_FILE");
	m_pForFileDM = m_pParentMgr->FindDM( m_sForFileDM );
	if( m_pForFileDM == NULL )ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -9 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );

	// ùȭ���� ��Ÿ ���� CM
	m_sEtcInfoCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");

	// �ٸ� �ڷ�ǿ� �����ϴ� �ڷḦ üũ�ϱ� ���� DM
	m_sTmpEtcInfoDMAlias = _T("DM_BO_LOC_2100_ETC_TMP");

}

CBLBookCheckProc::~CBLBookCheckProc()
{
	if( m_pDBProc != NULL ) delete m_pDBProc;
	if( m_pScreenPlay != NULL ) delete m_pScreenPlay;
	if( m_pLocCommonAPI != NULL ) delete m_pLocCommonAPI;
	m_pDBProc = NULL;
	m_pScreenPlay = NULL;
	m_pLocCommonAPI = NULL;
}

INT CBLBookCheckProc::SPFExcutePath( CString sPathAlias )
{
	EFS_BEGIN

	INT ids;

	if( sPathAlias.Compare( BOOK_SEARCH ) == 0 )
	{
		ids = SearchBook();
	}
	else if( sPathAlias.Compare( OPEN_REGNO_EDIT ) == 0 )
	{
		ids = OpenRegNoEdit();
	}
	else if( sPathAlias.Compare( SAVE_REGNO_EDIT ) == 0 )
	{
		ids = SaveRegNoEdit();
	}
	else if( sPathAlias.Compare( BOOK_CHECK ) == 0 )
	{
		ids = BookCheck();
	}
	else if( sPathAlias.Compare( INITIAL_BOOK_CHECK ) == 0 )
	{
		ids = InitBookCheckYN();
	}
	else if( sPathAlias.Compare( INIT_BOOK_CHECK_NUMBER ) == 0 )
	{
		ids = InitBookCheckNumber();
	}
	else if( sPathAlias.Compare( OPEN_BOOK_CHECK_NO_LIST ) == 0 )
	{
		ids = OpenBookCheckNoList();
	}
	else if( sPathAlias.Compare( CHANGE_CHECK_NO ) == 0 )
	{
		ids = ChangeCheckNo();
	}
	else if( sPathAlias.Compare( BOOK_CHECK_END ) == 0 )
	{
		ids = BookCheckEnd();
	}
	else if( sPathAlias.Compare( CHANGE_BOOK_CHECK_YN ) == 0 )
	{
		ids = ChangeBookCheckYN();
	}
	else if( sPathAlias.Compare( OPTION_SELECT ) == 0 )
	{
		ids = OptionSelect();
	}
	else if( sPathAlias.Compare( MOVE_GRID ) == 0 )
	{
		ids = MoveGrid();
	}
	else if( sPathAlias.Compare( SEARCH_BOOK_CHECK_N ) == 0 )
	{
		ids = SearchBookCheckN();
	}
	else if( sPathAlias.Compare( DETAIL_VIEW ) == 0 )
	{
		ids = DetailView();
	}
	else if( sPathAlias.Compare( DETAIL_VIEW2 ) == 0 )
	{
		ids = DetailView2();
	}
	else 
	{
		return -1;
	}

	if( ids < 0 ) 
		AfxMessageBox(_T("����� ���������� �������� ���Ͽ����ϴ�.") );

	return 0;

	EFS_END
	return -1;

}

// �弭���� ��� �ڷ� �˻�
INT CBLBookCheckProc::SearchBook()
{
	EFS_BEGIN

	INT ids;

	CString sWhere;		//WHERE��
	// 1. SQL�� ����
	ids = GetSqlFromControl( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchBook()") );
	if( ids > 0 ) return ids;

	// 2. DB���� SELECT�ؿ´�.
	ids = m_pDBProc->MakeDMFromDB( m_sBookDMAlias , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchBook()") );
	if( ids > 0 ) return ids;

	// 3. ����� ȭ�鿡 �����ش�.
	ids = m_pScreenPlay->MainGridDisplay( m_sBookCMAlias , m_sBookDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchBook()") );
	if( ids > 0 ) return ids;

	return 0;

	EFS_END
	return -1;

}



// �弭���� ��� �ڷ� �˻� ȭ�� -> WHERE�� ����
INT CBLBookCheckProc::GetSqlFromControl( CString &sWhere )
{
	EFS_BEGIN

	INT nMode;	// 0: ��Ϲ�ȣ�� �˻� , 1:û����ȣ�� �˻�

	// 0. ��� ����
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( m_sSearchBookCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetSqlFromControl( CString &sWhere )") );

	CESL_Control_Element *ebtnMode = pCM->FindControlElement( _T("��Ϲ�ȣüũ") );
	if( ebtnMode == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetSqlFromControl( CString &sWhere )") );

	CButton *btnMode = (CButton*)(m_pParentMgr->GetDlgItem( ebtnMode->CTRL_ID ));

	if( btnMode->GetCheck() == 1 ) 
		nMode = 0;
	else
		nMode = 1;

	INT nControlCnt;			// �̿�� ��Ʈ�� ��
	TCHAR **sControlFieldAlias;	// ��Ʈ�� �˸��ƽ�
	sControlFieldAlias = NULL;
	TCHAR **sDBFieldName;		// DB �ʵ� �̸�
	sDBFieldName = NULL;
	INT *nDBFieldType;		// DB �ʵ� Ÿ��
	nDBFieldType = NULL;
	CString *sGetData;			// ��Ʈ���� ��
	sGetData = NULL;

	// 1. ��Ʈ���� ���� ���´�.
	// 2. DB Field���� �����ش�.	
	// 3. DB Field Type�� �����ش�.
	if( nMode == 0 )
		nControlCnt = 4;
	else
		nControlCnt = 8;

	sControlFieldAlias = new TCHAR*[nControlCnt];
	sDBFieldName = new TCHAR*[nControlCnt];
	nDBFieldType = new INT[nControlCnt];
	sGetData = new CString[nControlCnt];

	INT ids = GetSearchConitionFromControl( nMode , sControlFieldAlias , sDBFieldName , nDBFieldType );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetSqlFromControl( CString &sWhere )") );

	// Control�� �����Ͱ��� ���´�.
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( m_sSearchBookCMAlias ,  sControlFieldAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetSqlFromControl( CString &sWhere )") );
	}

	
	// 4. Where���� �����Ѵ�.
	ids = m_pDBProc->MakeSql( sGetData , sDBFieldName , nDBFieldType , nControlCnt , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("GetSqlFromControl( CString &sWhere )") );

	// �弭 ���˰��� WHERE���� ������ ��Ų��.
	CString sBookCheckYN;
	CString sSpecialWhere;
	for( INT j = 0 ; j < 2 ; j++ )
	{
		ids = m_pScreenPlay->GetBookCheckInitYN( j , sBookCheckYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GetSqlFromControl( CString &sWhere )") );

		if( sBookCheckYN.IsEmpty() ) continue;

		if( sSpecialWhere.GetLength() > 1 ) sSpecialWhere += _T(" OR ");
		sSpecialWhere += _T(" BB.BOOK_CHECK_YN = '") + sBookCheckYN + _T("'");

		sBookCheckYN.Empty();
	}

	if( !sSpecialWhere.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T("( ") + sSpecialWhere + _T(" )");
	}

	if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
	sWhere += _T(" BB.WORKING_STATUS IN (") + CString(HOLD_SEARCH_CONDITION)+ _T(") ");

	// �⺻���� ���� �谡 ���°� �ǽð� �谡 �ڷ��ΰ��� �˻��� ���� �ʰ� �Ѵ�.
	if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
	sWhere += _T(" ( BB.PREV_WORKING_STATUS NOT IN ('SEL112N') OR BB.PREV_WORKING_STATUS IS NULL ) ");
	
	///@20071105 PDW
	///@{{++
	///@K3 �䱸���� ����
	///@�ڷ���� �ټ� �Է� ���� �� �ֵ��� ����	
	CString strShelfCode = GetShelfLocCode();
	if( !strShelfCode.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += " BB.SHELF_LOC_CODE IN (" + strShelfCode + ")";
	}
	///@--}}

	// 2009.02.25 ADD BY PDJ : MANAGE_CODEó�� �߰�
	sWhere += _T("AND BB.MANAGE_CODE=UDF_MANAGE_CODE");

	// ���� �Ҵ��� �޸𸮸� DELETE��Ų��. 
	
	for( i = 0 ; i < nControlCnt ; i++ )
	{
		delete sControlFieldAlias[i];
		delete sDBFieldName[i];
	}
	
	delete []sControlFieldAlias;
	delete []sDBFieldName;
	delete []nDBFieldType;
	delete []sGetData;
	
	return 0;

	EFS_END
	return -1;

}

// nMode�� ���� 0:��Ϲ�ȣ �˻� 1:û����ȣ�� �˻�
// �˻��ϴ� ��Ʈ�� �ʵ� �˸��ƽ��� DBField Name�� Field Type�� ���ϴ� �Լ�
INT CBLBookCheckProc::GetSearchConitionFromControl( INT nMode , TCHAR **sControlFieldAlias , TCHAR **sDBFieldName , INT *nDBFieldType )
{
	EFS_BEGIN

	///@20071205 PDW
	///@{{++
	///@K3 �䱸���� ����
	///@�ڷ�� ������ ������ �Է��Ѵ�.
	if( nMode == 0 )
	{
		const INT nTmpControlCnt = 3;
		TCHAR *sTmpControlFieldAlias[nTmpControlCnt] = 
		{
			/*_T("�ڷ�Ǹ�") , */_T("��ϱ���") , _T("��Ϲ�ȣ1") , _T("��Ϲ�ȣ2") 
		};
		TCHAR *sTmpDBFieldName[nTmpControlCnt] = 
		{
			/*_T("BB.SHELF_LOC_CODE") , */_T("BB.REG_CODE") , _T("BB.REG_NO") , _T("BB.REG_NO") 
		};
		INT nTmpDBFieldType[nTmpControlCnt] = 
		{
			/*ONESTRING , */THREESTRING , ETC , ETC
		};
		for( INT i = 0 ; i < nTmpControlCnt ; i++ )
		{
			sControlFieldAlias[i] = new TCHAR[_tcsclen(sTmpControlFieldAlias[i])+1];
			_tcscpy( sControlFieldAlias[i] , sTmpControlFieldAlias[i] );
			sDBFieldName[i] = new TCHAR[_tcsclen(sTmpDBFieldName[i])+1];
			_tcscpy( sDBFieldName[i] , sTmpDBFieldName[i] );
			nDBFieldType[i] = nTmpDBFieldType[i];
		}

	}
	else
	{
		const INT nTmpControlCnt = 4;
		TCHAR *sTmpControlFieldAlias[nTmpControlCnt] = 
		{
			/*_T("�ڷ�Ǹ�") , */_T("��ġ��ȣ1") , _T("��ġ��ȣ2") ,  _T("�з���ȣ1") , _T("�з���ȣ2") 
		};
		TCHAR *sTmpDBFieldName[nTmpControlCnt] = 
		{
			/*_T("BB.SHELF_LOC_CODE") , */_T("IB.SE_SHELF_CODE") , _T("IB.SE_SHELF_CODE")  , _T("IB.CLASS_NO") , _T("IB.CLASS_NO") 
		};
		INT nTmpDBFieldType[nTmpControlCnt] = 
		{
			/*ONESTRING , */TWOSTRING , ETC , TWOSTRING , ETC 
		};
		for( INT i = 0 ; i < nTmpControlCnt ; i++ )
		{
			sControlFieldAlias[i] = new TCHAR[_tcsclen(sTmpControlFieldAlias[i])+1];
			_tcscpy( sControlFieldAlias[i] , sTmpControlFieldAlias[i] );
			sDBFieldName[i] = new TCHAR[_tcsclen(sTmpDBFieldName[i])+1];
			_tcscpy( sDBFieldName[i] , sTmpDBFieldName[i] );
			nDBFieldType[i] = nTmpDBFieldType[i];
		}
	}
	///@--}}
	return 0;

	EFS_END
	return -1;

}


// �θ� ��Ϲ�ȣ DM���� �ڽ� ��Ϲ�ȣ DM���� ī���ϴ� �Լ�
INT CBLBookCheckProc::OpenRegNoEdit()
{
	EFS_BEGIN

	//ids = m_pLocCommonAPI->CopyDM( m_sParentRegNoDMAlias , m_sChildRegNoDMAlias );
	//if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OpenRegNoEdit()") );

	return 0;

	EFS_END
	return -1;

}

// �ڽ� ��Ϲ�ȣ DM���� �θ� ��Ϲ�ȣ DM���� ī���ϴ� �Լ�
INT CBLBookCheckProc::SaveRegNoEdit()
{
	EFS_BEGIN

	INT ids;

	//ids = m_pLocCommonAPI->CopyDM( m_sChildRegNoDMAlias , m_sParentRegNoDMAlias );
	//if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SaveRegNoEdit()") );


	// ȭ�鿡 ��Ϲ�ȣ���� �����Ѵ�.
	CESL_DataMgr *pRegNoDM = m_pParentMgr->FindDM( m_sParentRegNoDMAlias );
	if( pRegNoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SaveRegNoEdit()") );

	CString sRegNoCnt;
	sRegNoCnt.Format( _T("%d") , pRegNoDM->GetRowCount() );

	ids = m_pParentMgr->SetControlData( m_sEtcInfoCMAlias , _T("��Ϲ�ȣ��") , sRegNoCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SaveRegNoEdit()") ); 


	return 0;

	EFS_END
	return -1;

}

HANDLE				g_hEvent_Thread_BookCheck_1;
// �弭 üũ PATH
INT CBLBookCheckProc::BookCheck()
{
	EFS_BEGIN

	// ������� �����ϴ� �Լ� ����
	CWinThread* pwin_hThread =	AfxBeginThread( BookCheckThread, this  );

	return 0;

	EFS_END
	return -1;

}

// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//
// ���� �׸��� ��� ���̱�
//
// KOL.RED.2018.201 ---------------------------------------------------------------------------
void CBLBookCheckProc::Display_BookCheckProc()
{
	m_pScreenPlay->MainGridDisplay( m_sBookCMAlias , m_sBookDMAlias , &m_cMapData_ETC ,&m_cMapData_NoDB );
}

// �弭 üũ
INT CBLBookCheckProc::BookCheckProc()
{
	EFS_BEGIN

	INT ids;

	// ȭ�� �۾� : ���α׷����� �ʱ�ȭ
	// DM �۾� : ��Ÿ ���� DM �ʱ�ȭ
	ids = InitBookCheck();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("BookCheckProc()") );
	if( ids > 0 ) return ids;

	// DM �۾� : ��Ϲ�ȣ�� ���� �弭���� ����ڷ��� �ʱⰪ�� 'Y'�� �����ϰ� 
	//           �������� �ʴ� ��Ϲ�ȣ�� ��Ÿ ����DM�� �Է��Ѵ�.
	// ȭ�� �۾� : �׸����� ���� ���� �����Ѵ�.
	ids = ChangeRegNoCheckY();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("BookCheckProc()") );
	if( ids > 0 ) return ids;

	// �ڷ�� ����
	CString sShelfLocCode;
	///@20071205 PDW
	///@{{++
	///@K3 �䱸���� ����
	///@�ڷ���� ������ ������ �� �ֵ��� ����
	/*//before	
	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_2100_ETC_INFO") , _T("�ڷ�Ǹ�") , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("BookCheckProc()") );
	*///after
	sShelfLocCode = GetShelfLocCode();
	///@--}}
	
	ids = m_pParentMgr->SetDataMgrData( _T("DM_BO_LOC_2100_BOOK_CHECK_RESULT") , _T("����ڷ��") , sShelfLocCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("BookCheckProc()") );
	
	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	// ��� ������ ���� DM �� �ֱ� , ������ ���� �޼��� ������
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------
	ViewGridBookCheck();
	SendMessage( m_pParentMgr->m_hWnd , WM_USER + 2011 , 0 , 0 );

	/*
	// ����� ����
	// ���� �ڷ� , ��Ÿ �ڷ� , �н� �ڷ� , ������ �ڷ���� ȭ�鿡 ������
	ids = MakeResultBookCheck();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("BookCheckProc()") );
	if( ids > 0 ) return ids;
	*/
	return 0;

	EFS_END
	return -1;

}


// ���α׷����� �ʱ�ȭ
// ��Ÿ ���� DM �ʱ�ȭ
INT CBLBookCheckProc::InitBookCheck()
{
	EFS_BEGIN

	INT ids;

	
	INT nMaxCnt;
	if( m_bIsRegNoInDM )
		nMaxCnt = m_pRegNoDM->GetRowCount();
	else
		nMaxCnt = m_regNo.GetSize();
	
	if( nMaxCnt < 1 )
	{
		AfxMessageBox( _T("���� ��Ϲ�ȣ�� �������� �ʽ��ϴ�.\r\n��Ϲ�ȣ ������ ���� �Ͻʽÿ�.") );
		return 1;
	}

	// ���α׷����� �ʱ�ȭ
	ids = m_pScreenPlay->InitProgressBar( nMaxCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitBookCheck()") );


	// ��Ÿ ���� DM �ʱ�ȭ
	m_pInDBEtcDM->FreeData();
	m_pNotInDBEtcDM->FreeData();

	return 0;

	EFS_END
	return -1;

}

// ��Ϲ�ȣ��ϰ� �弭���� ����ڷ� ����� ��
INT CBLBookCheckProc::ChangeRegNoCheckY()
{
	EFS_BEGIN

	INT ids;

	CString sRegNoAlias = _T("��Ϲ�ȣ") ;
	// ��Ϲ�ȣ DM
	INT nRegNoDMRowCount;
	if( m_bIsRegNoInDM )
		nRegNoDMRowCount = m_pRegNoDM->GetRowCount();
	else
		nRegNoDMRowCount = m_regNo.GetSize();

	if( !nRegNoDMRowCount )
		return 0;
	// ������ڷ� DM
	INT nBookDMRowCount = m_pBookDM->GetRowCount();

	CString sRegNo;
	CString sBookCheckRegNo;

	bool bEqual = FALSE;	//���� �ڷᰡ �����ϴ°�?

	CString sPublishForm;



	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	// ���� m_pBookDM DM (��ó�� �׸����� ��ĪDM) ī�Ǻ����� ����Ʈ.

	/*
	// ��Ϲ�ȣ�� �������� SELECT�� DM
	CESL_DataMgr *pSelectDM = m_pParentMgr->FindDM( _T("DM_BLBookCheckProc_SELECT") );
	if( !pSelectDM )
		return -1;
	*/
	m_pBookDM->FreeData();
	CESL_DataMgr* pSelectDM = new CESL_DataMgr();
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , m_pBookDM , pSelectDM );	
	// �Է¼������ �׸��忡 �Ѹ��� ���� �ӽ� ����� �ʱ�ȭ
	m_cMapData_ETC.Clear();
	m_cMapData_NoDB.Clear();
	m_cMapData_ALL.Clear();
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	// �ִ� �ڷḦ UPDATE�� DM
	CESL_DataMgr *pUpdateDM = m_pParentMgr->FindDM( _T("DM_BLBookCheckProc_UPDATE") );
	if( !pUpdateDM )
		return -1;

	// �ð����
	CTime tStart = CTime::GetCurrentTime();
	
	INT startIndex, lastIndex;
	INT loopCnt = ( nRegNoDMRowCount % DEVIDED_CHECK_CNT ) ? ( nRegNoDMRowCount / DEVIDED_CHECK_CNT ) + 1 : ( nRegNoDMRowCount / DEVIDED_CHECK_CNT );
	CString query;
	CString shelfLocCode = GetShelfLocCode();
	for( INT i = 0; i < loopCnt; i++ )
	{
		// ©�󺸳� ���� index
		startIndex = i * DEVIDED_CHECK_CNT;
		// ©�󺸳�   �� index
		lastIndex  = startIndex + DEVIDED_CHECK_CNT - 1;
		lastIndex  = ( lastIndex > nRegNoDMRowCount - 1 ) ? nRegNoDMRowCount - 1 : lastIndex;

		// ©�� �˻��� ������ �����.
		query = MakeSelectQuery( m_pRegNoDM, startIndex, lastIndex );

		// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
		//
		// �ٲ� DM ���� ����Ʈ
		//ids = pSelectDM->RefreshDataManager( query );
		ids = m_pDBProc->MakeDMFromDB( _T("") , query , NULL , pSelectDM );
		//
		// KOL.RED.2018.201 ---------------------------------------------------------------------------
		
		// ��Ϲ�ȣ�� �˻��Ѵ�. 
		CheckRegNo( pSelectDM, pUpdateDM, startIndex, lastIndex, shelfLocCode );

		if( m_bBreak )
			return 100;
	}
	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	delete	pSelectDM;
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	CTime tEnd = CTime::GetCurrentTime();
	CTimeSpan ts = tEnd - tStart;
	m_sBookCheckSuccessMsg.Format( _T("\n\n\n")
				_T("���۽ð�	: %04d/%02d/%02d - %02d:%02d:%02d\n")
				_T("����ð�	: %04d/%02d/%02d - %02d:%02d:%02d\n")
				_T("�ҿ�ð�	: %4d�� %02d��"),
				tStart.GetYear(), tStart.GetMonth(), tStart.GetDay(), tStart.GetHour(), tStart.GetMinute(), tStart.GetSecond(),
				tEnd.GetYear(), tEnd.GetMonth(), tEnd.GetDay(), tEnd.GetHour(), tEnd.GetMinute(), tEnd.GetSecond(),
				ts.GetTotalMinutes(), ts.GetSeconds() );

	
	
	/*
	for( INT i = 0 ; i < nRegNoDMRowCount ; i++ )
	{
		//--------------------------------------------------------------
		// ���α׷��� �� ����
		m_pScreenPlay->ProgressSetPos(i+1);
		//--------------------------------------------------------------
		// ���� ������ ����--------------------------------------------------
		if( m_bBreak ) return 100;
		//--------------------------------------------------------------------

		bEqual = FALSE;

		ids = m_pRegNoDM->GetCellData( sRegNoAlias , i , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeRegNoCheckY()") );
		
		ids = IsBoBookTbl( sRegNo , sPublishForm);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeRegNoCheckY()") );
		if( ids == 0 )
		{
			// ���� �ڷ� ����
			ids = SettingInLib( sRegNo , sPublishForm );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ChangeRegNoCheckY()") );
			bEqual = TRUE;
		}

		// ��Ÿ ���� DM INSERT
		if( bEqual ) continue;
		
		if( ids == 1)
		{
			// TMP ETC DM -ADD-> ETC DM
			ids = m_pLocCommonAPI->AddDM( m_sTmpEtcInfoDMAlias , m_sInDBEtcDMAlias , m_pParentMgr );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ChangeRegNoCheckY()") ); 
		}else if( ids == 2 )
		{
			m_pNotInDBEtcDM->InsertRow(-1);

			ids = m_pNotInDBEtcDM->SetCellData( sRegNoAlias , sRegNo , m_pNotInDBEtcDM->GetRowCount()-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ChangeRegNoCheckY()") ); 
		}
	}
	*/

	return 0;

	EFS_END
	return -1;

}

// ��Ϲ�ȣ�� sRegNo�� �ڷᰡ DB�� �����ϴ� �� ����
// return 0 : �����ڷ��̴�.
//        ���� : ����
//        1 : ��Ÿ �ڷ� ( �ٸ� �ڷ�ǿ� ���� )
//        2 : �������� �ʴ� �ڷ�
INT CBLBookCheckProc::IsBoBookTbl( CString sRegNo , CString &sPublishForm )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");
	CString sControlAlias = _T("�ڷ�Ǹ�");

	CString sShelfLocCode;
	ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsBoBookTbl()") ); 
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();

	ids = m_pDBProc->IsRegNo( m_pBookDM , sRegNo , sShelfLocCode , sPublishForm );
	return ids;

	EFS_END
	return -1;

}

// ����� ����
// ���� �ڷ� , ��Ÿ �ڷ� , �н� �ڷ� , ������ �ڷ���� ȭ�鿡 ������
INT CBLBookCheckProc::MakeResultBookCheck()
{
	EFS_BEGIN

	INT ids;

	INT nBookCheckY = 0;	// �����ڷ�
	INT nBookCheckN = 0;	// �н��ڷ�
	INT nBookCheckX = 0;	// �������ڷ�
	INT nEtc = 0;			// ��Ÿ �ڷ�
	INT nTotalRegNoCnt = 0;		// ��Ϲ�ȣ �ڷ��
	INT nTotalBookCheckCnt =0;	// �弭���� ��� �ڷ��

	if( m_bIsRegNoInDM )
		nTotalRegNoCnt = m_pRegNoDM->GetRowCount();
	else
		nTotalRegNoCnt = m_regNo.GetSize();

	nTotalBookCheckCnt = m_pBookDM->GetRowCount();
	
	ids = GetBookCheckCnt( nBookCheckY , nBookCheckN , nBookCheckX );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeResultBookCheck()") ); 

//	nEtc = m_pEtcDM->GetRowCount();

	ids = m_pScreenPlay->ViewBookCheckResult( nBookCheckY , nBookCheckN , nBookCheckX , nEtc , nTotalRegNoCnt , nTotalBookCheckCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeResultBookCheck()") ); 

	return 0;

	EFS_END
	return -1;

}

// ���� �ڷ� , �н� �ڷ� , ������ �ڷḦ ������ �Լ�
INT CBLBookCheckProc::GetBookCheckCnt( INT &nBookCheckY , INT &nBookCheckN , INT &nBookCheckX )
{
	EFS_BEGIN

	INT ids;

	INT nRowCount = m_pBookDM->GetRowCount();

	CString sBookCheck;

	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = m_pBookDM->GetCellData( _T("�ʱ�ȭ����") , i , sBookCheck );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetBookCheckCnt( INT &nBookCheckY , INT &nBookCheckN , INT &nBookCheckX )") ); 

		if( sBookCheck.Compare( _T("Y") )==0 ) 
			nBookCheckY++;
		else if( sBookCheck.Compare( _T("N") )==0 ) 
			nBookCheckN++;
		else if( sBookCheck.Compare( _T("X") )==0 ) 
			nBookCheckX++;
	}

	return 0;

	EFS_END
	return -1;

}

bool CBLBookCheckProc::GetThreadState()
{
	EFS_BEGIN

	return m_bStop;

	EFS_END
	return FALSE;

}

INT CBLBookCheckProc::SetStopThread()
{
	EFS_BEGIN

	m_bBreak = TRUE;
	return 0;	

	EFS_END
	return -1;

}

INT CBLBookCheckProc::SetBookCheckButton()
{
	EFS_BEGIN

	m_pScreenPlay->SetBookCheckButton();

	return 0;

	EFS_END
	return -1;

}

// ���� �ڷ��� ��� �ʱ�ȭ ���а��� Y�� �����Ѵ�.
// DB�۾�
INT CBLBookCheckProc::SettingInLib( CString sRegNo , CString sPublishForm )
{
	EFS_BEGIN

	INT ids;

	ids = m_pDBProc->ChangeBookCheckYN( m_pBookDM , _T("Y") , sRegNo , sPublishForm , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitBookCheck") );

	return 0;

	EFS_END
	return -1;

}

// ������ �Լ�
UINT BookCheckThread(LPVOID lParam)
{
	INT ids;
	CString sErrorMsg;

	CBLBookCheckProc *pObj = (CBLBookCheckProc *)lParam;
	if ( pObj == NULL )
		return -1;

	pObj->m_bStop = FALSE;
	pObj->m_bBreak = FALSE;

	ids = pObj->BookCheckProc();
	if (ids == 100 ) sErrorMsg = _T("�弭 üũ�� �����Ǿ����ϴ�.");
	else if (ids < 0) sErrorMsg.Format(_T("�弭 üũ ���� �߻�-%d"),ids );
	else if (ids == 0) sErrorMsg = _T("�弭 üũ�� �Ϸ�Ǿ����ϴ�.") + pObj->m_sBookCheckSuccessMsg;
	if( !sErrorMsg.IsEmpty() ) AfxMessageBox(sErrorMsg);


	pObj->m_bStop = TRUE;


	SetEvent( g_hEvent_Thread_BookCheck_1 );
	return ids;
}


// �ʱ�ȭ
INT CBLBookCheckProc::InitBookCheckYN()
{
	EFS_BEGIN

	INT ids;
	// 1. SQL�� ���� Frame ���� , DB�� ������.
	ids = MakeInitBookCheckFrame( m_pBookDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitBookCheck") );
	if( ids > 0 ) return ids;

	// 2. �׸��� �ʱ�ȭ
	ids = InitGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("InitBookCheck") );
	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::InitGrid()
{
	EFS_BEGIN

	INT ids;

	m_pBookDM->FreeData();
	ids = m_pScreenPlay->MainGridDisplay( m_sBookCMAlias , m_sBookDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitGrid") );

	return 0;

	EFS_END
	return -1;

}

// �ʱ�ȭ UPDATE FRAME ����
INT CBLBookCheckProc::MakeInitBookCheckFrame( CESL_DataMgr *pDM )
{
	EFS_BEGIN

	INT ids;

	// ��Ʈ�ѿ��� ���� �����´�.
	///@20071205 PDW
	///@{{++
	///@K3 �䱸���� ����
	///@�ڷ���� �ټ� ������ �� �ֵ��� ����
	/*//before	
	CString sCMAlias = _T("CM_BO_LOC_2150");
	CString sParentCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");

	const INT nCnt = 1;
	TCHAR *sControlAlias[nCnt] = 
	{
		_T("�ڷ�Ǹ�") 
	};

	CString sGetData[nCnt];
	for( INT i=0 ; i<nCnt ;i++ )
	{
		ids = m_pParentMgr->GetControlData( sParentCMAlias , sControlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeInitBookCheckFrame( CESL_DataMgr *pDM )") );
		sGetData[i].TrimLeft(); sGetData[i].TrimRight();
	}
	*///after
	CString sCMAlias = _T("CM_BO_LOC_2150");
	CString sParentCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");
	CString sGetData[1];
	sGetData[0] = GetShelfLocCode();
	///@--}}
	

	CString sUpdateFrame;
	CString sUpdate;
	CString sWhere;

	//2020.03.27 JOB.2020.0021 add by KYJ : �弭���� �η� ���� �ɼ� �߰�
	CESL_ControlMgr* pMainCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_2100"));
	if( pMainCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , - 4 , _T("MakeInitBookCheckFrame") ); 
	
	CESL_Control_Element* pIncludeAppendix = pMainCM->FindControlElement( _T("�η�����üũ") );
	if( pIncludeAppendix == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , - 5 , _T("MakeInitBookCheckFrame") ); 
	
	CButton* btn1 = (CButton*)(m_pParentMgr->GetDlgItem( pIncludeAppendix->CTRL_ID ));
	if( btn1 == NULL ) {
		btn1 = (CButton*)(m_pParentMgr->pParentMgr->GetDlgItem( pIncludeAppendix->CTRL_ID ));
		if( btn1 == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , - 6 , _T("MakeInitBookCheckFrame") ); 
	}


	TCHAR *sWorkingStatus[8] = 
	{
		_T(HOLD_SEARCH_CONDITION),
		// 17/07/20 ������ : å���������ڷ� ���� �߰� 
/*//BEFORE-----------------------------------------------------------------------------
		//=====================================================
		//2009.03.26 UPDATE BY PJW :  ���� ��ȣ���� �ڷ���� �߰�
// 		_T("'BOL211O','BOL212O','BOL213O','BOL214O','BOL215O'"),
		// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
		_T("'BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O','BOL511O','BOL511O'"), 
		//_T("'BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O','BOL511O'"), 
		//=====================================================
*///AFTER------------------------------------------------------------------------------
		// 18.09.27 JMJ KOL.RED.2018.008
		_T("'BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O','BOL219O','BOL411O','BOL511O','BOL611O'"), 
/*//END-------------------------------------------------------------------------------*/		
		_T("'BOL112N'") ,
		_T("'BOL115O'") , 
		_T("'BOL252N'") , 
		_T("'BOL113O'") , 
		_T("'BOL114O'") ,
		_T("'BOL116O'")
	};
	
	pDM->StartFrame();

	// 1. ��ü
	// ���� �ڷḸ...
	if( sGetData[0].IsEmpty() )
		//2020.03.27 JOB.2020.0021 add by KYJ : �弭���� �η� ���� �ɼ� �߰�
		if ( btn1->GetCheck() == TRUE )
			sUpdate.Format( _T("UPDATE BO_BOOK_TBL SET BOOK_CHECK_YN = 'N' WHERE WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sWorkingStatus[0] );
		else
		sUpdate.Format( _T("UPDATE BO_BOOK_TBL SET BOOK_CHECK_YN = 'N' WHERE BOOK_APPENDIX_FLAG='B' AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sWorkingStatus[0] );
	else 
	{
		sUpdate.Format( _T("UPDATE BO_BOOK_TBL SET BOOK_CHECK_YN = 'N' ") );
		//2020.03.27 JOB.2020.0021 add by KYJ : �弭���� �η� ���� �ɼ� �߰�
		if ( btn1->GetCheck() == TRUE )
			sWhere.Format( _T("WHERE SHELF_LOC_CODE IN (%s) AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sGetData[0] , sWorkingStatus[0]);
		else		
		sWhere.Format( _T("WHERE BOOK_APPENDIX_FLAG='B' AND SHELF_LOC_CODE IN (%s) AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sGetData[0] , sWorkingStatus[0]);
	}

	sUpdateFrame = sUpdate + sWhere;
	pDM->AddFrame( sUpdateFrame );

	// ���� ���� �ڷḸ...
	TCHAR *sSEWorkingStatus[8] = 
	{
		_T(HOLD_SE_SEARCH_CONDITION) , 
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 			_T("'SEL311O','SEL312O','SEL313O','SEL314O','SEL315O'") ,
			// 18.09.27 JMJ KOL.RED.2018.008
			_T("'SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL316O','SEL317O','SEL411O','SEL511O'") ,
			//=====================================================
			_T("'SEL212N'") ,
			_T("'SEL215O'") , 
			_T("'SEL216O'") , 
			_T("'SEL213O'") , 
			_T("'SEL214O'") ,
			_T("'BOL217O'")
	};
	if( sGetData[0].IsEmpty() )
		//2020.03.27 JOB.2020.0021 add by KYJ : �弭���� �η� ���� �ɼ� �߰�
		if ( btn1->GetCheck() == TRUE )
			sUpdate.Format( _T("UPDATE SE_BOOK_TBL SET BOOK_CHECK_YN = 'N' WHERE BOOK_TYPE IN ('B','C','A') AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sSEWorkingStatus[0] );
		else
		sUpdate.Format( _T("UPDATE SE_BOOK_TBL SET BOOK_CHECK_YN = 'N' WHERE BOOK_TYPE IN ('B','C') AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sSEWorkingStatus[0] );
	else 
	{
		sUpdate.Format( _T("UPDATE SE_BOOK_TBL SET BOOK_CHECK_YN = 'N' ") );
		//2020.03.27 JOB.2020.0021 add by KYJ : �弭���� �η� ���� �ɼ� �߰�
		if ( btn1->GetCheck() == TRUE )
			sWhere.Format( _T("WHERE BOOK_TYPE IN ('B','C','A')  AND SHELF_LOC_CODE IN (%s) AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sGetData[0] , sSEWorkingStatus[0]);
		else
		sWhere.Format( _T("WHERE BOOK_TYPE IN ('B','C')  AND SHELF_LOC_CODE IN (%s) AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sGetData[0] , sSEWorkingStatus[0]);
	}
	
	sUpdateFrame = sUpdate + sWhere;
	pDM->AddFrame( sUpdateFrame );

	const INT nButtonCnt = 7;
	CString sCheckButtonAlias[nButtonCnt] = 
	{
		_T("����"),_T("�谡"),_T("����"),_T("�н�"),_T("����/����"),_T("�ļ�"),_T("������")
	};

	CESL_Control_Element *ebtnMode;
	CButton *btnMode;

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeInitBookCheckFrame( CESL_DataMgr *pDM )") );
	for( INT i = 1 ; i < nButtonCnt+1 ; i++ )
	{

		ebtnMode = pCM->FindControlElement( sCheckButtonAlias[i-1] );
		if( ebtnMode == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3, _T("IsCanCheck()") );
		btnMode = (CButton*)(m_pParentMgr->GetDlgItem( ebtnMode->CTRL_ID ));

		if( btnMode->GetCheck() != 1 ) continue;

		// ����
		sUpdate.Format( _T("UPDATE BO_BOOK_TBL SET BOOK_CHECK_YN = 'Y' ") );
		//2020.03.27 JOB.2020.0021 add by KYJ : �弭���� �η� ���� �ɼ� �߰�
		if ( btn1->GetCheck() == TRUE )
			sWhere.Format( _T("WHERE WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE") , sWorkingStatus[i] );
		else
		sWhere.Format( _T("WHERE BOOK_APPENDIX_FLAG='B' AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE") , sWorkingStatus[i] );

		if( !sGetData[0].IsEmpty() )
			sWhere +=  _T("AND SHELF_LOC_CODE IN (")+sGetData[0]+_T(")");
		
		sUpdateFrame = sUpdate + sWhere + _T(";");

		pDM->AddFrame( sUpdateFrame );

		// ����
		sUpdate.Format( _T("UPDATE SE_BOOK_TBL SET BOOK_CHECK_YN = 'Y' ") );
		//2020.03.27 JOB.2020.0021 add by KYJ : �弭���� �η� ���� �ɼ� �߰�
		if ( btn1->GetCheck() == TRUE )
			sWhere.Format( _T("WHERE BOOK_TYPE IN ('B','C','A') AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE") , sSEWorkingStatus[i] );
		else
		sWhere.Format( _T("WHERE BOOK_TYPE IN ('B','C') AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE") , sSEWorkingStatus[i] );

		if( !sGetData[0].IsEmpty() )
			sWhere +=  _T("AND SHELF_LOC_CODE IN (")+sGetData[0]+_T(")");
		
		sUpdateFrame = sUpdate + sWhere + _T(";");
		
		pDM->AddFrame( sUpdateFrame );
		
	}
	
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeInitBookCheckFrame( CESL_DataMgr *pDM )") );

	ids = pDM->EndFrame();

	return 0;

	EFS_END
	return -1;

}

// ���� ��ȣ �ʱ�ȭ
INT CBLBookCheckProc::InitBookCheckNumber()
{
	EFS_BEGIN

	INT ids;

	CString sCheckNumber;

	// 1. DB���� ���� �ο��� ���˹�ȣ�� ��������
	ids = m_pDBProc->GetMaxCheckNumber( m_pBookDM , sCheckNumber );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitBookCheckNumber()") );
	
	// DM �� �μ�Ʈ
	sCheckNumber.Format( _T("%d") , _ttoi( sCheckNumber ) + 1 );

	CString sShelfLocCode;
	if( m_pCheckNumberDM->GetRowCount() > 0 )
	{
		ids = m_pCheckNumberDM->GetCellData( _T("����ڷ��") , 0 , sShelfLocCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("InitBookCheckNumber") );
	}

	m_pCheckNumberDM->FreeData();
	m_pCheckNumberDM->InsertRow(-1);
	ids = m_pCheckNumberDM->SetCellData( _T("���˹�ȣ") , sCheckNumber , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("InitBookCheckNumber()") );

	// ���˳⵵��..
	CString sCheckYear;
	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	sCheckYear.Format(_T("%d") ,(CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData)).GetYear());
// 	sCheckYear.Format(_T("%d") ,(CLocCommonAPI::GetDBTime(m_pParentMgr)).GetYear());	
	
	ids = m_pCheckNumberDM->SetCellData( _T("�弭���˳⵵") , sCheckYear , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("InitBookCheckNumber()") );
	
	ids = m_pCheckNumberDM->SetCellData( _T("����ڷ��") , sShelfLocCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("InitBookCheckNumber()") );

	// 2. ȭ�鿡 �����ֱ�
	ids = m_pScreenPlay->ViewCheckNumber();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("InitBookCheckNumber()") );

	return 0;

	EFS_END
	return -1;

}


// ���� ��ȣ ������ ȭ���� ������ �Ͼ�� �÷ο�
INT CBLBookCheckProc::OpenBookCheckNoList()
{
	EFS_BEGIN

	INT ids;
	
	// 1. DM �����ϱ�
	ids = m_pDBProc->MakeCheckNoDMToDB();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OpenBookCheckNoList()") );

	// 2. ȭ�鿡 �����ֱ�
	ids = m_pScreenPlay->ViewCheckNoList();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("OpenBookCheckNoList()") );

	return 0;

	EFS_END
	return -1;

}

// ���� ��ȣ ����
INT CBLBookCheckProc::ChangeCheckNo()
{
	EFS_BEGIN

	INT ids;

	// DM �� �����ϴ� �� �˻�
	CString sDMAlias = _T("DM_BO_LOC_2100_BOOK_CHECK_RESULT_LIST");
	CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM( sDMAlias );
	if( pTmpDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeCheckNo()") );

	if( pTmpDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ���� ��ȣ�� �������� �ʽ��ϴ�.") );
		return 1;
	}

	// ���õȰ��� ���˹�ȣ DM���� �̵�
	INT nIndex = m_pScreenPlay->GetBookCheckSelectIndex();
	const INT nCopyCnt = 4;
	TCHAR *sFieldAlias[nCopyCnt] = 
	{
		_T("�弭���˳⵵") , _T("���˹�ȣ") , _T("�弭����KEY") , _T("����ڷ��")
	};

	CString sData;

	for( INT i = 0 ; i < nCopyCnt ; i++ )
	{
		ids = pTmpDM->GetCellData( sFieldAlias[i] , nIndex , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeCheckNo()") );

		if( i == 3 )
			m_pParentMgr->m_pInfo->pCodeMgr->ConvertDescToCode( _T("�ڷ�Ǳ���") , sData , sData );

		ids = m_pCheckNumberDM->SetCellData( sFieldAlias[i] , sData , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ChangeCheckNo()") );
	}

	// ���� ��ȣ�� ������.
	ids = m_pScreenPlay->ViewCheckNumber();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ChangeCheckNo()") );

	// �ʱ�ȭ���� ���濡�� ��� 
	// �̹� ���ſ� �ѹ� �����ߴٸ� �� ������ �ҷ��´�.
	// ������ �κп����� �� ���� ���� �ʴ´�.
	if( m_nLoadSPF == 0 )
	{
		ids = LoadPreBookCheckInfo();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ChangeCheckNo()") );
	}

	return 0;

	EFS_END
	return -1;

}


// �弭 ���� �Ϸ�
INT CBLBookCheckProc::BookCheckEndProc()
{
	EFS_BEGIN

	INT ids;

	CString sRecKey;

	// ȭ�� ProgressBar , �α� ��� ----------------------------------------------------------------
	ids = m_pScreenPlay->InitResultScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("BookCheckEndProc()") );


	// 1. ���� ��ȣ ����
	ids = MakeBookCheckNo( sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("BookCheckEndProc()") );


	// 2. �н� �ڷ� ó��
	ids = LostBookProc();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("BookCheckEndProc()") ); 
	

	// 3. ��� ���� ����
	ids = MakeBookCheckResulFile();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("BookCheckEndProc()") ); 


	// 4. ���� ���̺� ������Ʈ
	//    ��Ÿ ���̺� �μ�Ʈ	
	ids = BookCheckResultDBUpdate();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("BookCheckEndProc()") ); 
	

	// ȭ�� ProgressBar , �α� ��� ----------------------------------------------------------------
	ids = m_pScreenPlay->SetResultScreen( 100 , _T("�۾� �Ϸ�") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("BookCheckEndProc()") );

	/*
	// ���˹�ȣ �����ϱ�
	ids = InitBookCheckNumber();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("BookCheckEndProc()") );
	*/
	
	return 0;

	EFS_END
	return -1;

}

// ���� ��ȣ ����
INT CBLBookCheckProc::MakeBookCheckNo( CString &sRecKey )
{
	EFS_BEGIN

	INT ids;

	ids = m_pScreenPlay->SetResultScreen( 1 , _T("���� ��ȣ ������..") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("BookCheckEndProc()") );

	// ���� ��ȣ�� �����ϴ��� �˻�
	ids = m_pCheckNumberDM->GetCellData( _T("�弭����KEY") , 0 , sRecKey );
	if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("BookCheckEndProc()") );


	if( sRecKey.IsEmpty() )
	{
		CString sCheckNo;
		ids = m_pCheckNumberDM->GetCellData( _T("���˹�ȣ") , 0 , sCheckNo );
		if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("BookCheckEndProc()") );
		CString sCheckYear;
		ids = m_pCheckNumberDM->GetCellData( _T("�弭���˳⵵") , 0 , sCheckYear );
		if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("BookCheckEndProc()") );
		
		// ���� ��ȣ�� DB�� �������� �ʴ´ٸ� ����
		ids = m_pDBProc->MakeBookCheckResultRecord( m_pBookDM , sCheckNo , sCheckYear , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("BookCheckEndProc()") );

		// DM�� �Է�
		ids = m_pCheckNumberDM->SetCellData( _T("�弭����KEY") , sRecKey , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("BookCheckEndProc()") );
	}

	ids = m_pScreenPlay->SetResultScreen( 10 , _T("���� ��ȣ ���� �Ϸ�") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("BookCheckEndProc()") );

	return 0;

	EFS_END
	return -1;

}

// ��� ���� ����
INT CBLBookCheckProc::MakeBookCheckResulFile()
{
	EFS_BEGIN

	INT ids;

	ids = m_pScreenPlay->SetResultScreen( 21 , _T("�弭 ���� ��� ���� ������..") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , _T("MakeBookCheckResulFile()") );
/*
	// 1. ���� ��� ���� ����
	ids = MakeResultFile(0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResulFile()") );
*/
	// 2. �н� ��� ���� ����
	ids = MakeResultFile(1);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeBookCheckResulFile()") );

	// 3. ��Ÿ ��� ���� ����
	ids = MakeEtcResultFile();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeBookCheckResulFile()") );

	ids = m_pScreenPlay->SetResultScreen( 80 , _T("�弭 ���� ��� ���� ���� �Ϸ�") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -17 , _T("MakeBookCheckResulFile()") );

	return 0;

	EFS_END
	return -1;

}

// ���� ���̺� ������Ʈ 
INT CBLBookCheckProc::BookCheckResultDBUpdate()
{
	EFS_BEGIN

	INT ids;

	ids = m_pScreenPlay->SetResultScreen( 81 , _T("��� ���� ���ε���..") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -18 , _T("BookCheckResultDBUpdate()") );

	// ��� ���� UPLOAD
	INT nCnt = 4;
	CString sFileName;
	CString sCheckNoKey;
	ids = m_pCheckNumberDM->GetCellData( _T("�弭����KEY") , 0 , sCheckNoKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("BookCheckResultDBUpdate()") );
	
	CString sShelfLocCode;
	ids = m_pParentMgr->GetDataMgrData( _T("DM_BO_LOC_2100_BOOK_CHECK_RESULT") , _T("����ڷ��") , sShelfLocCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("BookCheckResultDBUpdate()") ); 
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();

	for( INT i = 1 ; i < nCnt ; i++ )
	{
		ids = GetFileName( i , sFileName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("BookCheckResultDBUpdate()") );
		ids = m_pDBProc->UploadResultFile( m_pBookDM , i , sFileName , sCheckNoKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("BookCheckResultDBUpdate()") );
	}

	ids = m_pDBProc->EndBookCheckDBProc( m_pInDBEtcDM , m_pNotInDBEtcDM , sCheckNoKey , sShelfLocCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("BookCheckResultDBUpdate()") );

	ids = m_pScreenPlay->SetResultScreen( 100 , _T("��� ���� ���ε� �Ϸ�") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -19, _T("BookCheckResultDBUpdate()") );

	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::BookCheckEnd()
{
	EFS_BEGIN

	// ������� �����ϴ� �Լ� ����
	AfxBeginThread( BookCheckEndThread, this );

	return 0;

	EFS_END
	return -1;

}

// �н� �ڷ� ó��
INT CBLBookCheckProc::LostBookProc()
{
	EFS_BEGIN

	INT ids;

	ids = m_pScreenPlay->SetResultScreen( 11 , _T("�н� �ڷ� ó����...") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("LostBookProc()") );

	///@20071105 PDW
	///@{{++
	///@K3 �䱸���� ����
	///@�ڷ���� �ټ� ������ �� �ְ� ����
	/*//before	
	CString sShelfLocCode;
	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_2100_ETC_INFO") , _T("�ڷ�Ǹ�") , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("LostBookProc()") ); 

	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	*///after
	CString sShelfLocCode = GetShelfLocCode();
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	///@--}}	

	CString sWorkingStatus;

	ids = m_pParentMgr->GetControlData( m_sEtcInfoCMAlias , _T("�н��ڷ�ó��") , sWorkingStatus , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("LostBookProc()") ); 

	ids = m_pDBProc->UpdateLostBookWorkingStatus( m_pBookDM , sShelfLocCode , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("LostBookProc()") ); 
	
	ids = m_pScreenPlay->SetResultScreen( 20 , _T("�н� �ڷ� ó�� �Ϸ�") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("LostBookProc()") );

	return 0;

	EFS_END
	return -1;

}

// ��� ���� ����
INT CBLBookCheckProc::MakeResultFile(INT nMode)
{
	EFS_BEGIN

	INT ids;

	ids = m_pDBProc->GetDBForFile( m_pForFileDM , nMode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeResultFile()") ); 

	CString sFileName;
	ids = GetFileName( nMode , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeResultFile()") ); 

	ids = SanFileAPI::PortableScannerFileSave( m_pForFileDM , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeResultFile()") ); 

	CString sMsg;
	INT nProgress;
	if( nMode == 0 )
	{
		nProgress = 40;
		sMsg.Format( _T("���� ��� ������ '") );
	}
	else if( nMode == 1 )
	{
		nProgress = 60;
		sMsg.Format( _T("�н� �ڷ� ��� ������ '") );
	}
	sMsg = sMsg + sFileName + _T("' �� �����Ǿ����ϴ�.");
	ids = m_pScreenPlay->SetResultScreen( nProgress , sMsg );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , _T("MakeBookCheckResulFile()") );


	return 0;

	EFS_END
	return -1;

}


// ������ �Լ�
UINT BookCheckEndThread(LPVOID lParam)
{
	INT ids;
	CString sErrorMsg;

	CBLBookCheckProc *pObj = (CBLBookCheckProc *)lParam;
	if ( pObj == NULL )
		return -1;

	pObj->m_bStop = FALSE;
	pObj->m_bBreak = FALSE;

	ids = pObj->BookCheckEndProc();
	if (ids == 100 ) sErrorMsg = _T("�弭 ���� �Ϸ� ����");
	else if (ids < 0) sErrorMsg.Format(_T("�弭 ���� �Ϸ� ���� �߻�-%d"),ids );
	else if (ids == 0) sErrorMsg = _T("�弼 ������ �Ϸ�Ǿ����ϴ�.");

	pObj->m_bStop = TRUE;

	if (ids < 0  && ids != 100){
		AfxMessageBox(sErrorMsg);
		return 0;
	}
	if( ids > 0 ) 
		return 0;

	return 0;
}


// ������� ���� �̸��� ������ �Լ�
// nMode 0: ������ ���� , 1: �нǸ�� ���� , 2: ��Ÿ��� ���� , 3: ��Ÿ��� ����2
INT CBLBookCheckProc::GetFileName( INT nMode , CString &sFileName )
{
	EFS_BEGIN

	INT ids;

	CString sPath = _T("..\\BookCheck");
	_tmkdir( sPath );

	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	CTime tCurrent = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr,strGetData);
// 	CTime tCurrent = CLocCommonAPI::GetDBTime(m_pParentMgr);

	// ���˹�ȣ
	CString sCheckNo;
	
	ids = m_pCheckNumberDM->GetCellData( _T("���˹�ȣ") , 0 , sCheckNo );
	if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetFileName()") );

	sPath += _T("\\") + sCheckNo;
	_tmkdir( sPath );

	// ������
	sPath += _T("\\") + tCurrent.Format(_T("%Y%m%d"));
	_tmkdir( sPath );

	if( nMode == 0 ) 
		sFileName = sPath + _T("\\������.txt");
	else if( nMode == 1 )
		sFileName = sPath + _T("\\�нǸ��.txt");
	else if( nMode == 2 )
		sFileName = sPath + _T("\\��Ÿ���1.txt");
	else if( nMode == 3 )
		sFileName = sPath + _T("\\��Ÿ���2.txt");

	return 0;

	EFS_END
	return -1;

}

// ��Ÿ ���� ���� 
INT CBLBookCheckProc::MakeEtcResultFile()
{
	EFS_BEGIN

	INT ids;
	
	CString sFileName;
	ids = GetFileName( 2 , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeEtcResultFile()") ); 

	ids = SanFileAPI::PortableScannerFileSave( m_pInDBEtcDM , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeEtcResultFile()") ); 

	CString sMsg;
	sMsg.Format( _T("��Ÿ ����1�� '%s'�� �����Ǿ����ϴ�.") , sFileName );

	ids = m_pScreenPlay->SetResultScreen( 70 , sMsg  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , _T("MakeBookCheckResulFile()") );


	ids = GetFileName( 3 , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeEtcResultFile()") ); 

	ids = SanFileAPI::PortableScannerFileSave( m_pNotInDBEtcDM , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeEtcResultFile()") ); 

	sMsg.Format( _T("��Ÿ ����2�� '%s'�� �����Ǿ����ϴ�.") , sFileName );

	ids = m_pScreenPlay->SetResultScreen( 80 , sMsg  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , _T("MakeBookCheckResulFile()") );

	return 0;

	EFS_END
	return -1;

}

// ������
// �׸��忡 üũ�� �Ǿ� �ִ� �κ��� ����
INT CBLBookCheckProc::ChangeBookCheckYN()
{
	EFS_BEGIN

	INT ids;


	CESL_Grid *pGrid = m_pScreenPlay->GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeBookCheckYN()") ); 

	INT nRowCount = pGrid->GetCount();

	if( nRowCount < 1 ) return 0;

	INT nMode;
	ids = m_pScreenPlay->GetBookChangeMode( nMode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeBookCheckYN()") ); 

	ids = m_pScreenPlay->ChangeBookCheckPrgressBarInit( nRowCount );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ChangeBookCheckYN()") ); 

	CString sCheck;
	CString sBookCheckYN;
	CString sRegNo;
	CString sBookKey;

	TCHAR *sDMColumnAlias[1] = { _T("����") };

	INT nColIndex[1] = { -1 };

	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sDMColumnAlias , 1 , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeBookCheckYN") );

	INT nCheckColIndex = nColIndex[0];

	CString sPublishForm;

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	CString sCheckResult_NoDB;
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = m_pScreenPlay->ChangeBookCheckPrgressBarSetPos(i+1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ChangeBookCheckYN()") ); 

		ids = pGrid->GetAt( i , nCheckColIndex , sCheck );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ChangeBookCheckYN()") ); 

		if( sCheck.Compare(_T("V"))!=0 )
		{
			m_pBookDM->SetCellData( _T("����") , _T("") , i );
		}
		if( sCheck.Compare(_T("V"))!=0 ) continue;
		
		// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
		//
		// "���˰��" ��  "DB����" �̸� ����.
		m_pBookDM->GetCellData( _T("���˰��") , i , sCheckResult_NoDB );
		if( sCheckResult_NoDB == _T("DB����") ) 
		{
			continue;
		}
		//
		// KOL.RED.2018.201 ---------------------------------------------------------------------------
		
		ids = m_pBookDM->SetCellData( _T("����") , _T("V") , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("ChangeBookCheckYN()") ); 

		ids = m_pBookDM->GetCellData( _T("�ʱ�ȭ����") , i , sBookCheckYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("ChangeBookCheckYN()") ); 

		if( nMode == 0 ) sBookCheckYN = _T("Y");
		else if( nMode == 1 ) sBookCheckYN = _T("N");
		else if( nMode == 2 )
		{
			if( sBookCheckYN.Compare(_T("Y"))==0)
				sBookCheckYN = _T("N");
			else if( sBookCheckYN.Compare(_T("N"))==0)
				sBookCheckYN = _T("Y");
		}
		ids = m_pBookDM->SetCellData( _T("�ʱ�ȭ����") , sBookCheckYN , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("ChangeBookCheckYN()") ); 

		ids = m_pBookDM->GetCellData( _T("��Ϲ�ȣ") , i , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9, _T("ChangeBookCheckYN()") ); 

		ids = m_pBookDM->GetCellData( _T("åKEY") , i , sBookKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9, _T("ChangeBookCheckYN()") ); 

		ids = m_pBookDM->GetCellData( _T("����/���ӱ���") , i , sPublishForm );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9, _T("ChangeBookCheckYN()") ); 
		

		ids = m_pDBProc->ChangeBookCheckYN( m_pBookDM , sBookCheckYN , sBookKey , sPublishForm , FALSE );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("ChangeBookCheckYN()") ); 
	}

	ids = m_pScreenPlay->MainGridDisplay( m_sBookCMAlias , m_sBookDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("ChangeBookCheckYN()") );  

	ids = m_pScreenPlay->ChangeBookCheckPrgressBarEnd();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("ChangeBookCheckYN()") ); 

	return 0;

	EFS_END
	return -1;

}

// �ϰ� ����
INT CBLBookCheckProc::OptionSelect()
{
	EFS_BEGIN

	INT ids;

	// �˻� �ɼ�..
	// DM�� �о���� �˻� ���ǿ� �ɸ��� ���� üũ�Ѵ�.
	INT nRowCount = m_pBookDM->GetRowCount();

	CESL_Grid *pGrid = m_pScreenPlay->GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeBookCheckYN()") ); 

	CString sCheck;
	ids = m_pScreenPlay->GetCheckMode( sCheck );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OptionSelect") ); 

	TCHAR *sDMColumnAlias[1] = { _T("����") };
	INT nColIndex[1] = { -1 };

	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = IsCanCheck(i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OptionSelect") );
		
		if( ids > 0 ) continue;

		nColIndex[0] = -1;

		ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sDMColumnAlias , 1 , nColIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GridCheckYN") );

		if( nColIndex[0] == -1 ) continue;

		pGrid->SetAt( i , nColIndex[0]  , sCheck );
	}

	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::IsCanCheck( INT nIndex )
{
	EFS_BEGIN

	INT ids;

	CString sShelfLocCode;
	CString sWorkingStatus;
	CString sLoanDate;
	CString sReturnDate;


	// 1. �ڷ�� ��
	ids = m_pBookDM->GetCellData( _T("�ڷ��") , nIndex , sShelfLocCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsCanCheck()") );
	sShelfLocCode.TrimRight(); sShelfLocCode.TrimLeft();

	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertDescToCode( _T("�ڷ�Ǳ���") , sShelfLocCode , sShelfLocCode );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsCanCheck()") );

	ids = m_pScreenPlay->IsCanCheck( 0 , sShelfLocCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("IsCanCheck()") );
	if( ids > 0 ) return 1;


	// 2. �谡 ���� ��
	ids = m_pBookDM->GetCellData( _T("�谡����") , nIndex , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsCanCheck()") );

	ids = m_pLocCommonAPI->GetWorkingStatusDescToCode( sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("IsCanCheck()") );

	ids = m_pScreenPlay->IsCanCheck( 1 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsCanCheck()") );
	if( ids > 0 ) return 1;

	// 3. ���� �ڷ� ��
	ids = m_pBookDM->GetCellData( _T("������") , nIndex , sLoanDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("IsCanCheck()") );
	ids = m_pBookDM->GetCellData( _T("�ݳ���") , nIndex , sReturnDate  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("IsCanCheck()") );
	//    ������ ��
	ids = m_pScreenPlay->IsCanCheck( 2 , sLoanDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("IsCanCheck()") );
	if( ids > 0 ) return 1;
	//    �ݳ��� ��
	ids = m_pScreenPlay->IsCanCheck( 3 , sReturnDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("IsCanCheck()") );
	if( ids > 0 ) return 1;

	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::MoveGrid()
{
	EFS_BEGIN

	CString sMove1 , sMove2;

	INT ids;

	ids = m_pParentMgr->GetControlData( m_sEtcInfoCMAlias , _T("�̵���ȣ1") , sMove1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MoveGrid()") );
	ids = m_pParentMgr->GetControlData( m_sEtcInfoCMAlias , _T("�̵���ȣ2") , sMove2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MoveGrid()") );
	
	if( sMove2.Compare(_T("0"))==0 ) return 0;

	if( _ttoi(sMove1) < 0 ) return 0;

	ids = m_pScreenPlay->MoveGrid( _ttoi(sMove1) );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MoveGrid()") );

	return 0;

	EFS_END
	return -1;

}


// ����� �ڷ�� ���õ� �ڷ� ã��
INT CBLBookCheckProc::SearchBookCheckN()
{
	EFS_BEGIN

	INT ids;

	// 1. SQL�� ����
	CString sCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");
	///@20071105 PDW
	///@{{++
	///@K3 �䱸���� ����
	///@�ڷ���� �ټ� ������ �� �ֵ��� ����
	/*//before	
	CString sShelfLocCode;
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�ڷ�Ǹ�") , sShelfLocCode , -1 , 0 );	
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchBookCheckN()") );
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	*///after
	CString sShelfLocCode = GetShelfLocCode();	
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	///@--}}
	//CString sWhere = _T(" ( BB.BOOK_CHECK_YN = 'N' OR BB.BOOK_CHECK_YN IS NULL ) AND ( BB.PREV_WORKING_STATUS NOT IN ('SEL112N') OR BB.PREV_WORKING_STATUS IS NULL ) AND BB.WORKING_STATUS IN (")HOLD_SEARCH_CONDITION_T(",")HOLD_SE_SEARCH_CONDITION_T(") ");
	CString sWhere = _T("( BB.BOOK_CHECK_YN = 'N' OR BB.BOOK_CHECK_YN IS NULL ) AND ( BB.PREV_WORKING_STATUS NOT IN ('SEL112N') OR BB.PREV_WORKING_STATUS IS NULL ) AND BB.WORKING_STATUS IN (")_T(HOLD_SEARCH_CONDITION)_T(",")_T(HOLD_SE_SEARCH_CONDITION) _T(") ");
	if( !sShelfLocCode.IsEmpty() ) sWhere += _T(" AND BB.SHELF_LOC_CODE IN (" + sShelfLocCode +_T(")"));

	// 2009.02.25 ADD BY PDJ : MANAGE_CODEó�� �߰�
	sWhere += _T("AND BB.MANAGE_CODE=UDF_MANAGE_CODE");

	// 2. DB���� SELECT�ؿ´�.


	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	// ���� ���� DM �� �����ϰ� , �߰��Ѵ�.
	// m_sBookDMAlias ����ؼ� Row ������ ���� MapData_Mem
	//ids = m_pDBProc->MakeDMFromDB( m_sBookDMAlias , sWhere );
	CBLMapData_Mem	cMapData_NotHave; // �����
	ids = m_pDBProc->MakeDMFromDB( m_sBookDMAlias , sWhere , &cMapData_NotHave );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchBookCheckN()") );
	if( ids > 0 ) return ids;
	// ���� ���� DM �� ��� �޾ƿ� Row �߰��Ѵ�.
	cMapData_NotHave.CopyToDM( m_pBookDM );
	cMapData_NotHave.Clear();
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------



	// 3. ����� ȭ�鿡 �����ش�.
	ids = m_pScreenPlay->MainGridDisplay( m_sBookCMAlias , m_sBookDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchBookCheckN()") );
	if( ids > 0 ) return ids;

	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::DetailView()
{
	EFS_BEGIN

	if( m_pBookDM->GetRowCount() < 0 ) return 0;

	CESL_Grid *pGrid = m_pScreenPlay->GetMainGridPtr();
	if( pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::DetailView()") );

	if( pGrid->GetMouseRow() < 1 ) return 0;

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;

	INT nIndex = -1;
	nIndex = pGrid->GetIdx();
	m_pBookDM->GetCellData( _T("��KEY") , nIndex , sSpeciesKey );
	sSpeciesKeyList.AddTail( sSpeciesKey );
	asGridRow.Add( nIndex );
	
	// ���� ���� ����
	CString sPublishForm;
	m_pBookDM->GetCellData( _T("����/���ӱ���") , nIndex , sPublishForm );
	if( sPublishForm.Compare(_T("MO"))==0 )
	{
		CBOBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow );
		Dlg.DoModal();
	}
	else if( sPublishForm.Compare(_T("SE"))==0 )
	{
		CSEBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow );
		Dlg.DoModal();
	}

	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::DetailView2()
{
	EFS_BEGIN

	if( m_pInDBEtcDM->GetRowCount() < 0 ) return 0;

	CESL_Grid *pGrid = m_pScreenPlay->GetInDBGridPtr();
	if( pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::DetailView()") );

	if( pGrid->GetMouseRow() < 1 ) return 0;

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;

	INT nIndex = -1;
	nIndex = pGrid->GetIdx();
	m_pInDBEtcDM->GetCellData( _T("��KEY") , nIndex , sSpeciesKey );
	sSpeciesKeyList.AddTail( sSpeciesKey );
	asGridRow.Add( nIndex );

	// ���� ���� ����
	CString sPublishForm;
	m_pInDBEtcDM->GetCellData( _T("����/���ӱ���") , nIndex , sPublishForm );
	if( sPublishForm.Compare(_T("MO"))==0 )
	{
		CBOBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow );
		Dlg.DoModal();
	}
	else if( sPublishForm.Compare(_T("SE"))==0 )
	{
		CSEBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow );
		Dlg.DoModal();
	}

	return 0;

	EFS_END
	return -1;

}

// ���� �弭 ���� ���� �ε��ϱ�

INT CBLBookCheckProc::LoadPreBookCheckInfo()
{
	EFS_BEGIN

	INT ids;

	// ����� �ڷ� ã��
	// ��Ÿ�ڷ�1 ã��
	// ��Ÿ�ڷ�2 ã��
	ids = MakeBookCheckResultDMFromPreBookCheckInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("LoadPreBookCheckInfo") );
	if( ids > 0 ) return 1;


	SendMessage( m_pParentMgr->m_hWnd, WM_USER + 807, 1000, NULL );
	SendMessage( m_pParentMgr->m_hWnd, WM_USER + 808, 999, NULL );
	AfxMessageBox( _T("�弭���� ��ȣ ������ �Ϸ��Ͽ����ϴ�.") );
	
	// �׸��忡 �����ֱ�
	ids = AllControlDisplayGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("LoadPreBookCheckInfo") );

	
	
	return 0;

	EFS_END
	return -1;

}

// ���� �弭���� �̷� DB���� ���� DM�����ϱ�
INT CBLBookCheckProc::MakeBookCheckResultDMFromPreBookCheckInfo()
{
	EFS_BEGIN

	INT ids;

	const INT nCnt = 3;
	CString sInfo[nCnt][2] =
	{
		{ _T("LOST_LIST_FILE") , _T("DM_BO_LOC_2100") } , 
		{ _T("ETC_LIST_FILE") , _T("DM_BO_LOC_2100_ETC") }, 
		{ _T("ETC_LIST_FILE2") , _T("DM_BO_LOC_2100_ETC_NOT_IN_DB") }
	};
	
	CESL_DataMgr *pDstDM;		// ��� DM
	CESL_DataMgr *pRegNoDM;		// ��Ϲ�ȣ DM
	

	pRegNoDM = MakeTempRegNoDM();
	if( pRegNoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResultDMFromPreBookCheckInfo") );

	
	INT nFindLostList = AfxMessageBox( _T("������ڷḦ �Բ� �˻��Ͻðڽ��ϱ�? \n�˻��� ���� ������ڷ��̱⶧���� ���������� �ٸ� �� �ֽ��ϴ�.") , MB_YESNO );
	CString currentJob[ 3 ] = {
		_T("[ 1 / 3 : ������ڷḦ �˻���             ]"), 
		_T("[ 2 / 3 : �ٸ��ڷ�ǿ� �����ϴ� �ڷ� �˻� ]"), 
		_T("[ 3 / 3 : DB�� �������� �ʴ� �ڷ� �˻�    ]") };
	//SendMessage( m_pParentMgr->m_hWnd, WM_USER + 807, nCnt, NULL );

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		//SendMessage( m_pParentMgr->m_hWnd, WM_USER + 808, i, NULL );
		if( !i && nFindLostList != IDYES )
			continue;
		SendMessage( m_pParentMgr->m_hWnd, WM_USER + 809, 2, ( LPARAM )&currentJob[ i ] );
		
		pRegNoDM->FreeData();

		pDstDM = m_pParentMgr->FindDM( sInfo[i][1] );
		if( pDstDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResultDMFromPreBookCheckInfo") );
		pDstDM->FreeData();

		if( i == 2 )
		{
			ids = MakeRegNoFromBLOB( sInfo[i][0] , pDstDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResultDMFromPreBookCheckInfo") );			
		}
		else
		{
			ids = MakeRegNoFromBLOB( sInfo[i][0] , pRegNoDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResultDMFromPreBookCheckInfo") );
	
			ids = MakeDMFromRegNoSql( pDstDM , pRegNoDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResultDMFromPreBookCheckInfo") );
			if( ids > 0 ) return 1;
		}
	}

	// �޸� ����
	if( pRegNoDM != NULL ) 
		delete pRegNoDM;

	pRegNoDM = NULL;

	return 0;

	EFS_END
	return -1;

}

// ���� ��Ϲ�ȣ DM �ʱ�ȭ�ϱ�
CESL_DataMgr* CBLBookCheckProc::MakeTempRegNoDM()
{
	EFS_BEGIN

	INT ids;

	// DM �޸� ����
	CESL_DataMgr *pTmpDM = new CESL_DataMgr;
	

	// DM ���۷��� ����
	pTmpDM->InitDataMgrRef(1);		
	pTmpDM->m_nCols = 1;
	ids = pTmpDM->SetDataMgrRef( 0 , _T("��Ϲ�ȣ") , _T("") , _T("")  , _T("")  , _T("") );			
	if( ids < 0 ) return NULL;

	return pTmpDM;

	EFS_END
	return NULL;

}



// DB BO_BOOK_CHK_RESULT_TBL�� sFieldName�� ������ pRegNoDM�� ��Ϲ�ȣ�� �߰��Ͽ� ����
INT CBLBookCheckProc::MakeRegNoFromBLOB(CString sFieldName , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;

	CString sFileName = _T("BookCheckTmp.txt");	// ���� ���� �̸�

	// 1. �弭 ���� REC_KEY ��������
	//    REC_KEY�� ���ٸ� SKIP
	
	
	CString currentJob = _T("�弭���� ������ �������� ���Դϴ�.");
	SendMessage( m_pParentMgr->m_hWnd, WM_USER + 809, 1, ( LPARAM )&currentJob );
	
	ids = MakeRegNoFileFromDBLOBField( sFileName , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeRegNoFromBLOB") );
	if( ids > 0 ) return 0;
	

	// 2. ���Ͽ� �ִ� ������ pRegNoDM�� ����
	ids = SanFileAPI::RegNoFileLoad( pRegNoDM , m_pParentMgr , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeRegNoFromBLOB") );

	return 0;

	EFS_END
	return -1;

}

// ���� ���õ� �弭 üũ REC_KEY�� ���´�.
INT CBLBookCheckProc::GetBookCheckRecKey(CString &sRecKey)
{
	EFS_BEGIN

	INT ids;

	CString sFieldAlias = _T("�弭����KEY");

	ids = m_pCheckNumberDM->GetCellData( sFieldAlias , 0 , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetBookCheckRecKey()") );
	sRecKey.TrimRight(); sRecKey.TrimLeft();

	return 0;

	EFS_END
	return -1;

}

// �弭���� ��� ���̺��� sFieldName�� ������ sFileName�� ����Ѵ�.
INT CBLBookCheckProc::MakeRegNoFileFromDBLOBField( CString sFileName , CString sFieldName )
{
	EFS_BEGIN

	INT ids;

	CString sTableName = _T("BO_BOOK_CHK_RESULT_TBL");	// �弭 ���� ��� ���̺�

	CString sRecKey;	// �弭���� ���̺� REC_KEY

	// ���� ������ ����.
	CFile fd;
	if (fd.Open( sFileName , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) == NULL) {
		AfxMessageBox(_T("�弭 ���� Tmp File Open Error~"));		
		return 1;
	}
/*
	if( 0 == fd.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fd.Write( &cUni, sizeof(TCHAR));
	}
*/	
	fd.Write(_T(""), 0);
	fd.Close();
	//_tremove(sFileName);

	ids = GetBookCheckRecKey( sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeRegNoFileFromDBLOBField()") );
	if( sRecKey.IsEmpty() ) return 1;
	
	// 2. �弭 ���� ��� ������ DB���� ��������
	BYTE *pBuffer = (BYTE *)malloc(MAX_BLOB_BUFFER);

	INT nSize;
	ids = m_pCheckNumberDM->LOBGet( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey , sFieldName , pBuffer , &nSize );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeRegNoFileFromDBLOBField") );
	if ( nSize == 0 ) 
	{
		free( pBuffer );
		return 0;	
	}

	
	if (fd.Open( sFileName , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) == NULL) {
		AfxMessageBox(_T("�弭 ���� Tmp File Open Error~"));
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

	return 0;

	EFS_END
	return -1;

}

// pRegNoDM���� ��Ϲ�ȣ�� �������� �ϴ� SQL�� �����Ͽ� pDstDM�� ����
INT CBLBookCheckProc::MakeDMFromRegNoSql( CESL_DataMgr *pDstDM , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;

	INT nRegNoRowCount = pRegNoDM->GetRowCount();

	CESL_DataMgr TmpDstDM;
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDstDM , &TmpDstDM );	
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeDMFromRegNoSql") );

	INT nLimitCnt = 50;
	bool IsSend = FALSE;

	CString sRecKeyList;
	CString sWhere;
	CString sRecKey;

	CString currentJob[ 2 ] = {
		_T("���� �ڷ� ������ �˻����Դϴ�."),
		_T("���� �ڷ� ������ �˻����Դϴ�.") };
	// ���� �ڷ� , ���� �ڷῡ�� �˻��Ѵ�.
	for( INT j = 0 ; j < 2 ; j++ )
	{
		if( j == 1)
		{
			TmpDstDM.TBL_NAME = _T("SE_BOOK_TBL BB , IDX_SE_TBL IB , SE_VOL_TBL V");
			TmpDstDM.CONDITION = _T("BB.VOL_KEY=V.REC_KEY AND BB.SPECIES_KEY=IB.REC_KEY AND BB.BOOK_TYPE IN ('B','C','A') ");
			
			CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
			for( INT k = 0 ; k < TmpDstDM.RefList.GetCount() ; k++ )
			{
				sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
				ids = TmpDstDM.GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeDMFromDB") );
				
				if( sFieldAlias.Compare(_T("����/���ӱ���")) == 0 )
				{
					ids = TmpDstDM.SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , _T("SE")  , sDataType );			
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") );
				}
				else if( sFieldAlias.Compare(_T("����")) == 0 )
				{
					ids = TmpDstDM.SetDataMgrRef( k , sFieldAlias , _T("V.PRICE") , sFieldType  , sInitValue  , sDataType );			
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") );
				}
				else if( sFieldAlias.Compare(_T("�����")) == 0 )
				{
					ids = TmpDstDM.SetDataMgrRef( k , sFieldAlias , _T("IB.PUB_YEAR") , sFieldType  , sInitValue , sDataType );			
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") );
				}
			}
		}
		
		SendMessage( m_pParentMgr->m_hWnd, WM_USER + 809, 1, ( LPARAM )&currentJob[ j ] );
		SendMessage( m_pParentMgr->m_hWnd, WM_USER + 807, nRegNoRowCount, NULL );
		for( INT i = 0 ; i < nRegNoRowCount ; i++ )
		{
			SendMessage( m_pParentMgr->m_hWnd, WM_USER + 808, i, NULL );
			
			IsSend=FALSE;

			if( i == nRegNoRowCount-1 ) IsSend = TRUE;
			if( (i!=0) && (i%nLimitCnt==0) ) IsSend = TRUE;


			ids = pRegNoDM->GetCellData( _T("��Ϲ�ȣ") , i , sRecKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeDMFromRegNoSql") );

			if( !sRecKey.IsEmpty() )
			{
				if( !sRecKeyList.IsEmpty() ) sRecKeyList += _T(",");
				sRecKeyList += _T("'") + sRecKey + _T("'");
			}
			
			if( !IsSend ) continue;

			if( sRecKeyList.IsEmpty() ) continue;

			sWhere.Format(_T(" BB.WORKING_STATUS IN (%s) AND BB.REG_NO IN ( %s ) "), _T(HOLD_SEARCH_CONDITION)_T(",")_T(HOLD_SE_SEARCH_CONDITION) , sRecKeyList );
			ids = TmpDstDM.RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeDMFromRegNoSql") );

			ids = CLocCommonAPI::AddDM( &TmpDstDM , pDstDM , m_pParentMgr );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromRegNoSql") );

			sRecKeyList.Empty();
		}
	}

	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::AllControlDisplayGrid()
{
	EFS_BEGIN

	INT ids;

	ids = m_pScreenPlay->AllControlDisplayGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,  -1 , _T("AllControlDisplayGrid") );

	return 0;

	EFS_END
	return -1;

}

CString CBLBookCheckProc::MakeSelectQuery( CESL_DataMgr *pDM, INT startIndex, INT lastIndex )
{
	CString query, tmp, buf;

	tmp = _T(" REG_NO IN(");
	for( INT i = startIndex; i < lastIndex + 1; i++ )
	{
		if( m_bIsRegNoInDM )
			buf.Format( _T(" '%s',"), pDM->GetCellData( _T("��Ϲ�ȣ"), i ) );
		else
			buf.Format( _T(" '%s',"), m_regNo.GetAt( i ) );

		tmp += buf;
	}
	buf = tmp.Left( tmp.GetLength() - 1 ) + _T(" )");
	query = buf;
	

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	// ���� �׸����� DM ���� ����Ʈ�� �Ҳ��� 
	// �Ʒ� ������ ���Ѵ�.
	/*
	query.Format( 
		_T("SELECT 'M', B.REG_NO, I.TITLE_INFO, B.SHELF_LOC_CODE, ECO$F_MAKE_CALLNO$I(B.SEPARATE_SHELF_CODE, B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE) AS CALL_NO  FROM BO_BOOK_TBL B, IDX_BO_TBL I WHERE B.SPECIES_KEY = I.REC_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND %s ")
		_T("UNION ")
		_T("SELECT 'S', B.REG_NO, I.TITLE_INFO, B.SHELF_LOC_CODE, ECO$F_MAKE_CALLNO$I(B.SEPARATE_SHELF_CODE, B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE) AS CALL_NO  fROM SE_BOOK_TBL B, IDX_SE_TBL I WHERE B.SPECIES_KEY = I.REC_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND %s "),
		buf, buf );
	*/

	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	return query;
}


INT CBLBookCheckProc::CheckRegNo( CESL_DataMgr *pSelectDM, CESL_DataMgr *pUpdateDM, INT startIndex, INT lastIndex, CString shelfLocCode )
{
	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	// �ؽ� �����..
	CBLMapData_Mem	cMapData_pDMSelect;
	cMapData_pDMSelect.Add_SelectQuery( pSelectDM , _T("��Ϲ�ȣ") );
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	CString query, regNo, compareRegNo;
	
	INT type;	// 1 : �����ڷ��϶�
				// 2 : DB�� ����
				// 3 : �ڷ�� �ٸ�
	
	pUpdateDM->StartFrame();
	pUpdateDM->InitDBFieldData();

	CString strTargetLoc;
	
	INT idx = 0;
	// ©�� üũ�� ī��Ʈ ��ŭ loop
	for( INT j = startIndex; j < lastIndex + 1; j++ )
	{
		m_pScreenPlay->ProgressSetPos( j + 1 );
		
		// �˻��񱳵� ��Ϲ�ȣ
		if( m_bIsRegNoInDM )
			regNo = m_pRegNoDM->GetCellData( _T("��Ϲ�ȣ"), j );
		else
			regNo = m_regNo.GetAt( j );
			
		type = 2;
			
		// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
		//
		// �ؽ��� ������� �ʾƼ� �ʹ� ����
		// SQL In �� 200 -> 1000�� ������ �ø��� (���� 900)
		// �ؽ� �����ϰ� ã�� ������� ��ü
		_BLMapData_Value* pFind = cMapData_pDMSelect.Find( regNo );
		idx = -1;
		if( pFind != NULL )
		{
			idx = pFind->row_idx;
			if( shelfLocCode.IsEmpty() )
			{
				type = 1;
			}else{
				// dm �ڷ���δ콺 : 11
				strTargetLoc = pFind->GetArrData(11);
					if( -1 == shelfLocCode.Find(strTargetLoc) )
						type = 3;
					// �ڷ�ǰ� ��Ϲ�ȣ�� ��ġ�Ҷ�					
					else						
						type = 1;
			}
		}
			
		//// �˻��� ���ڵ� ������ŭ loop
		//for( INT k = 0; k < pSelectDM->GetRowCount(); k++ )
		//{
		//	compareRegNo = pSelectDM->GetCellData( k, 1 );

		//	// �ڷ���� �������� �ʰ� ��Ϲ�ȣ�� ��ġ�Ҷ�
		//	if( shelfLocCode.IsEmpty() )
		//	{
		//		if( regNo == compareRegNo )
		//		{
		//			type = 1;
		//			idx = k;
		//		}
		//	}
		//	else
		//	{
		//		if( regNo == compareRegNo )
		//		{
		//			///@20071205 PDW
		//			///@{{++
		//			///@K3 �䱸���� ����
		//			///@�ڷ���� �ټ� ������ �� �ֵ��� ����
		//			/*//before	
		//			if( shelfLocCode == pSelectDM->GetCellData( k, 3 ) )
		//			// �ڷ�ǰ� ��Ϲ�ȣ�� ��ġ�Ҷ�					
		//				type = 1;
		//			// �ڷ���� Ʋ���� ��Ϲ�ȣ�� ��ġ�Ҷ�
		//			else
		//				type = 3;
		//			*///after					
		//			// �ڷ���� Ʋ���� ��Ϲ�ȣ�� ��ġ�Ҷ�
		//			strTargetLoc = pSelectDM->GetCellData( k, 3 );
		//			if( -1 == shelfLocCode.Find(strTargetLoc) )
		//				type = 3;
		//			// �ڷ�ǰ� ��Ϲ�ȣ�� ��ġ�Ҷ�					
		//			else						
		//				type = 1;
		//			///@--}}
		//				
		//			idx = k;
		//		}
		//	}
		//	
		//	// ������ �����ϱ�
		//	if( m_bBreak )
		//		return 100;			
		//}

		//
		// KOL.RED.2018.201 ---------------------------------------------------------------------------

		UpdateRegNoInfo( pUpdateDM, type, regNo, pSelectDM, idx );		
	}

	pUpdateDM->SendFrame();
	pUpdateDM->EndFrame();
	
	cMapData_pDMSelect.Clear();

	// �ڷ�Ǹ�
	return 0;
}

///@20071205 PDW
///@{{++
///@K3 �䱸���� ����
///@�ڷ���� ���� ���ð����ϵ��� ����
/*//before	
CString CBLBookCheckProc::GetShelfLocCode()
{
	CString sShelfLocCode;
	m_pParentMgr->GetControlData( _T("CM_BO_LOC_2100_ETC_INFO"), _T("�ڷ�Ǹ�") ,sShelfLocCode ,-1 ,0 );
	return sShelfLocCode;
}
*///after
CString CBLBookCheckProc::GetShelfLocCode()
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
///@--}}


VOID CBLBookCheckProc::UpdateRegNoInfo( CESL_DataMgr *pUpdateDM, INT type, CString regNo, CESL_DataMgr *pSelectDM, INT idx )
{
	// �ڷ���� �ٸ� ��Ϲ�ȣ�϶��� ���� ����
	const int cnt = 4;
	CString alias[ cnt ] = {
		_T("��Ϲ�ȣ"), _T("����"), _T("�ڷ��"), _T("û����ȣ") };
	CString data[ cnt ];
	
	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	int  alias_count = pSelectDM->GetColCount();
	CString *sel_alias_str = new CString[alias_count];
	CString *sel_Data_str = new CString[alias_count];
	pSelectDM->GetAllAlias(sel_alias_str, alias_count);
	int i=0;
	for( i = 0 ; i < alias_count ; i ++ )
	{
		CString str = sel_alias_str[i];
		str += _T("\n");
		OutputDebugString( str );
	}

	//���� �׸��� DM �� "����/���ӱ���" -> 22 �÷�
	TCHAR tch_MO_SE;
	if( idx > -1 )
	{
		CString sMO_SE = pSelectDM->GetCellData( idx, 22 );
		if( sMO_SE.GetLength() > 0 )
		{
			tch_MO_SE = sMO_SE.GetAt( 0 );
		}
	}
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------
	
	CESL_DataMgr *pDM = NULL;
	CString query;
	switch( type )
	{
		case 1 :	// �����ڷ��϶�

			// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			//
			//switch( pSelectDM->GetCellData( idx, 0 ).GetAt( 0 ) )
			switch( tch_MO_SE )
			//
			// KOL.RED.2018.201 ---------------------------------------------------------------------------
			{
				case 'M' :	// �����϶�
					query.Format( _T("UPDATE BO_BOOK_TBL SET BOOK_CHECK_YN = 'Y' WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;"), regNo );
					break;
				case 'S' :	// �����϶�
					query.Format( _T("UPDATE SE_BOOK_TBL SET BOOK_CHECK_YN = 'Y' WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;"), regNo );
					break;
				default :
					break;
			}
			pUpdateDM->AddFrame( query );
			break;
		case 2 :	// DB�� ����	- DM_BO_LOC_2100_ETC_NOT_IN_DB
			pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_2100_ETC_NOT_IN_DB") );
			pDM->SetCellData( _T("��Ϲ�ȣ"), regNo, -1 );

			// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			//
			m_cMapData_NoDB.Add( regNo, sel_Data_str , 0 );	// ���µ�Ϲ�ȣ  �ӽ�����
			m_cMapData_ALL.Add( regNo, sel_Data_str , 0 );
			//
			// KOL.RED.2018.201 ---------------------------------------------------------------------------
			break;
		case 3 :	// �ڷ�� �ٸ�	- DM_BO_LOC_2100_ETC

			// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			// ���� DM �Ⱦ�
			/*
			data[ 0 ] = pSelectDM->GetCellData( idx, 1 );
			data[ 1 ] = pSelectDM->GetCellData( idx, 2 );
			data[ 2 ] = pSelectDM->GetCellData( idx, 3 );
			data[ 3 ] = pSelectDM->GetCellData( idx, 4 );
			pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_2100_ETC") );
			pDM->SetCellData( alias, 4, data, -1 );
			*/

			pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_2100_ETC") );
			pDM->SetCellData( _T("��Ϲ�ȣ"), regNo, -1 );

			pSelectDM->GetCellData( sel_alias_str , alias_count , sel_Data_str , idx );
			m_cMapData_ETC.Add( regNo, sel_Data_str , alias_count ); // �ٸ� �ڷ�� �ӽ�����
			m_cMapData_ALL.Add( regNo, sel_Data_str , alias_count );
			//
			// KOL.RED.2018.201 ---------------------------------------------------------------------------


			break;
		default :
			break;
	}
	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	// 
	delete [] sel_alias_str;
	delete [] sel_Data_str;
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------
}

// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//
// ���Լ����� �з��ؼ� ������ �����͸� �Էµ� ������ ���� �׸��忡 ���̱�.
void	CBLBookCheckProc::ViewGridBookCheck()
{
	// �ѱ׸��忡 ���϶� �ٸ� �ڷ�� Row ������ ���Ŀ� 
	// ���� Row ������ ���̱�.
	//ViewGridBookCheck_MapData( &m_cMapData_ETC );
	//ViewGridBookCheck_MapData( &m_cMapData_NoDB );

	// ������ ������κ��� ���� �Ʒ� �ڵ�
	ViewGridBookCheck_MapData( &m_cMapData_ALL );
}
//
// KOL.RED.2018.201 ---------------------------------------------------------------------------


// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
// �ؽ� ����ҿ� �ִ°Ÿ� ( �з��Ȱ͵� ) 
// m_pBookDM ���� �׸��� DM �� �ִ´�.
// 
void	CBLBookCheckProc::ViewGridBookCheck_MapData( CBLMapData_Mem* pMapData )
{
	int  alias_count = m_pBookDM->GetColCount();
	CString *sel_alias_str = new CString[alias_count];
	CString *sel_Data_str  = new CString[alias_count];
	m_pBookDM->GetAllAlias(sel_alias_str, alias_count);

	INT nRegNoDMRowCount;
	if( m_bIsRegNoInDM )	nRegNoDMRowCount = m_pRegNoDM->GetRowCount();
	else					nRegNoDMRowCount = m_regNo.GetSize();

	CString			regNo;
	CArray<CString,CString&>	*arr_Data;
	for( int i = 0 ; i < nRegNoDMRowCount ; i++ )
	{
		if( m_bIsRegNoInDM )	regNo = m_pRegNoDM->GetCellData( _T("��Ϲ�ȣ"), i );
		else					regNo = m_regNo.GetAt( i );

		arr_Data = pMapData->Find_ArrData( regNo );

		if( arr_Data == NULL )
		{
			// ERR 
			continue;
		}
	
		if( arr_Data->GetSize() > 0 )
		{
			for( int o = 0 ; o < alias_count ; o++ )
			{
				sel_Data_str[o] = arr_Data->GetAt(o);
			}
			m_pBookDM->SetCellData( sel_alias_str, alias_count, sel_Data_str ,-1 );
		}else{
			// �迭 �����Ͱ� ���°� DB�� ���°�
			// ���Լ����� ���ڿ� �迭�� �ƹ��͵� �ȳ־���.
			// �߰� ó�� �Ҳ��� ���⼭..
			m_pBookDM->SetCellData( _T("��Ϲ�ȣ"), regNo, -1 );
		}
	}

	delete [] sel_alias_str;
	delete [] sel_Data_str;

}
//
// KOL.RED.2018.201 ---------------------------------------------------------------------------


CString CBLBookCheckProc::ConvertPrice( CString price )
{
	return m_pScreenPlay->ConvertPrice( price );
}
