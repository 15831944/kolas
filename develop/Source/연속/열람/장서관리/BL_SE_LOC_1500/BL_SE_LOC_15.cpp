// BL_SE_LOC_15.cpp: implementation of the CBL_SE_LOC_15 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BL_SE_LOC_15.h"

#include "..\..\..\..\����\K2UP_Common\MakeSearchData.h"
#include "..\\..\\..\\����\\SE_API\\SeApi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBL_SE_LOC_15::CBL_SE_LOC_15(CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	m_sMainDMAlias  = _T("DM_SE_LOC_1500");
}

CBL_SE_LOC_15::~CBL_SE_LOC_15()
{

}

INT CBL_SE_LOC_15::SPFExcutePath( CString sPathAlias )
{
	INT ids;

	if( sPathAlias.Compare( SEARCH ) == 0 )
	{
		ids = SearchSE_LOC_1510();
	}
	else if( sPathAlias.Compare( RFID_SEARCH ) == 0 )
	{
		ids = SearchSE_LOC_1510FromRFID();
	}
	else if( sPathAlias.Compare( RECORD_SHELF_DATE ) == 0 )
	{
		ids = RecordShelfDate();
	}
	else if( sPathAlias.Compare( RECORD_TMP_REG_NO ) == 0 )
	{
		ids = RecordRegNo();
	}
	else if( sPathAlias.Compare( CANCEL_SHELF ) == 0 )
	{
		ids = CancelShelf();
	}
	else if( sPathAlias.Compare( CANCEL_TMP_REG_NO ) == 0 )
	{
		ids = CancelRegNo();
	}
	else if( sPathAlias.Compare( CHANGE_BOX_NAME ) == 0 )
	{
		ids = ChangeBoxName();
	}
	else if( sPathAlias.Compare( RESTORE_DATA ) == 0 )
	{
		ids = RestoreData();
	}
	else if( sPathAlias.Compare( CANCEL_RESTORE ) == 0 )
	{
		ids = CancelRestore();
	}
	else if( sPathAlias.Compare( REMOVE_DATA ) == 0 )
	{
		ids = RemoveData();
	}
	else if( sPathAlias.Compare( REMOVE_COMPLETE ) == 0 )
	{
		ids = RemoveComplete();
	}
	else if( sPathAlias.Compare( CANCEL_REMOVE ) == 0 )
	{
		ids = CancelRemove();
	}
	else if( sPathAlias.Compare( ONE_BOOK_SHELF_CHANGE ) == 0 )
	{
		ids = OneBookChangeShelf();
	}
	else if( sPathAlias.Compare( ALL_BOOK_SHELF_CHANGE ) == 0 )
	{
		ids = AllBookChangeShelf();
	}
	else if( sPathAlias.Compare( ALL_SPECIES_SHELF_CHANGE ) == 0 )
	{
		ids = AllSpeciesChangeShelf();
	}
	else if( sPathAlias.Compare( ALL_SPECIES_CODE_CHANGE ) == 0 )
	{
		ids = ChangeSpeciesCode();
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( ALL_BOOK_CODE_CHANGE ) == 0 )
	{
		ids = ChangeCode();
		if( ids == 1 ) return -1;
	}
	if( ids < 0 ) 
		AfxMessageBox(_T("����� ���������� �������� ���Ͽ����ϴ�.") );

	return ids;
}

// �˻�
INT CBL_SE_LOC_15::SearchSE_LOC_1510()
{
	INT ids;

	// Where�� ����
	CString sWhere;
	ids = GetWhereSE_LOC_1510(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchBO_LOC_1410") );
	if( ids > 0 ) return 1;


	// Refresh DataManager
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( m_sMainDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchBO_LOC_1410") );

	DWORD START, FINISH;
	START = GetTickCount();
	

	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchBO_LOC_1410") );

	FINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - �ǽð����� ã�� �˻��� �ɸ� �ð� :  [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
	TRACE(strMsg.GetBuffer(0));
	pDM->QueryLog(strMsg.GetBuffer(0));

	//===================================================
	//2010.01.14 ADD BY PJW : �˻������ ���� �޼��� �����ش�.
	if( pDM->GetRowCount() <= 0)
	{
		AfxMessageBox(_T("�˻��� ����� �����ϴ�."));
	}
	else
	{
		AfxMessageBox(_T("�˻��� �Ϸ� �Ͽ����ϴ�."));
	}
	//===================================================
	
	// ȭ�鿡 �����ֱ�
	ids = ViewMainGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchBO_LOC_1410") );

	return 0;
}


// �˻�
INT CBL_SE_LOC_15::SearchSE_LOC_1510FromRFID()
{
	INT ids;
	
	CString sWhere;
	
	GetTempData( _T("WHERE") , sWhere );
	
	// Refresh DataManager
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( m_sMainDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchBO_LOC_1410") );
	
	

	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchBO_LOC_1410") );

	
	
	// ȭ�鿡 �����ֱ�
	ids = ViewMainGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchBO_LOC_1410") );
	
	return 0;
}

// Where�� ����
// 050329_�˻����ǿ� ���Ա��� �߰� idx => 24    -_-+++
INT CBL_SE_LOC_15::GetWhereSE_LOC_1510( CString &sWhere )
{
	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1510");	// �˻� CM Alias

	// 1. �⺻ Where�� ����
	const INT nCnt = 31;//30
	CString sControlAlias[nCnt] =
	{
		_T("�ڷ�Ǹ�") , _T("�ΰ�����1") , _T("�ΰ�����2") , _T("�������1") , _T("�������2") , 
		_T("�Թ�ȣ1") , _T("�Թ�ȣ2") , _T("������ȣ1") , _T("������ȣ2"), 
		_T("�����") , _T("�з���ȣ����") , _T("�з���ȣ1") , 
		_T("�з���ȣ2") ,  _T("����Ϲ�ȣ1") , _T("����Ϲ�ȣ2") , _T("�����ȣ1") , 
		_T("�����ȣ2") , _T("����1") , _T("����2") , _T("������1") , _T("������2") , 
		_T("������1") , _T("������2") , _T("ISSN") , _T("�Լ�����"), _T("�谡����1") , 
		_T("�谡����2") , _T("��������1") , _T("��������2") ,  _T("�ڷ����") , _T("ISSN_����")
	};

	CString sControlData[nCnt];	// �Է°�

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetWhereBO_LOC_1410") );

	// Control���� �Է°� ��������
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sControlData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetWhereBO_LOC_1410") );

		// ��Ʈ���� �𽺿��̺��̶�� �˻� ���ǿ� ���� �ʴ´�.
		CESL_Control_Element *pCE = ((CESL_Control_Element*)pCM->FindControlElement( sControlAlias[i] ));
		if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetWhereBO_LOC_1410") );
		
		if( !m_pParentMgr->GetDlgItem( pCE->CTRL_ID )->IsWindowEnabled() )
			sControlData[i].Empty();

		if( sControlAlias[i].Compare(_T("�з���ȣ����")) == 0 )
			sControlData[i].Empty();

		sControlData[i].TrimLeft(); 
		sControlData[i].TrimRight();
	}

	// �ʼ� �Է� �׸� �˻�
	// 2003�� 6�� 16�� ������ ���� : �ڷ���� �ʼ� �Է� �׸����� �˻����� �ʴ´�.
	/*
	if( sControlData[0].IsEmpty() )
	{
		AfxMessageBox( _T("�ڷ���� �������ֽʽÿ�.") );
		return 1;
	}
	*/

	TCHAR *sEcoDBFieldName[25] = 
	{
		_T("B.SHELF_LOC_CODE") , _T("B.IMM_LOC_TRANSFER_DATE") , _T("B.IMM_LOC_TRANSFER_DATE") , _T("B.DISUSE_PROC_DATE") , _T("B.DISUSE_PROC_DATE")  ,
		_T("S.BOX_NAME") , _T("S.BOX_NAME") , _T("B.RECEIPT_NO") , _T("B.RECEIPT_NO"), 
		_T("I.PUB_FREQ") , _T("B.CLASS_NO_TYPE") , _T("B.CLASS_NO") , 
		_T("B.CLASS_NO")  , _T("B.REG_NO") , _T("B.REG_NO") , _T("I.CONTROL_NO") ,
		//_T("I.CONTROL_NO") , _T("I.SE_IDX_TITLE") , _T("I.SE_IDX_TITLE") , _T("I.SE_IDX_PUBLISHER") , _T("I.SE_IDX_PUBLISHER") ,
		_T("I.CONTROL_NO") , _T("I.SE_IDX_TITLE") , _T("I.IDX_TITLE") , _T("I.SE_IDX_PUBLISHER") , _T("I.SE_IDX_PUBLISHER") ,
		_T("V.PUBLISH_DATE") , _T("V.PUBLISH_DATE") , _T("I.ST_ISSN") , _T("B.ACQ_CODE")
	};


	TCHAR *sCatDBFieldName[25] = 
	{
		_T("B.SHELF_LOC_CODE") , _T("B.IMM_LOC_TRANSFER_DATE") , _T("B.IMM_LOC_TRANSFER_DATE") , _T("B.DISUSE_PROC_DATE") , _T("B.DISUSE_PROC_DATE")  ,
		_T("S.BOX_NAME") , _T("S.BOX_NAME") , _T("B.RECEIPT_NO") , _T("B.RECEIPT_NO"), 
		_T("I.PUB_FREQ") , _T("B.CLASS_NO_TYPE") , _T("B.CLASS_NO") , 
		_T("B.CLASS_NO")  , _T("B.REG_NO") , _T("B.REG_NO") , _T("I.CONTROL_NO") ,
		//_T("I.CONTROL_NO") , _T("I.SE_IDX_TITLE") , _T("I.IDX_TITLE") , _T("I.IDX_PUBLISHER") , _T("I.SE_IDX_PUBLISHER") ,
		_T("I.CONTROL_NO") , _T("I.IDX_TITLE") , _T("I.IDX_TITLE") , _T("I.IDX_PUBLISHER") , _T("I.SE_IDX_PUBLISHER") ,
		_T("V.PUBLISH_DATE") , _T("V.PUBLISH_DATE") , _T("I.ST_ISSN") , _T("B.ACQ_CODE")
	};

	/// 2004�� 5�� 20�� ������ ����
	/// �Թ�ȣ�� ������ �˻� �����ϵ��� �ش޶�� ��û�� ����
	
	INT nDBFieldType[25] = 
	{
		ONESTRING , TWODATE , ETC , TWODATE , ETC ,	// 5
		ETC , ETC , TWONUMERIC , ETC ,				// 9
		ONESTRING , ONESTRING , TWOSTRING ,			// 12
		ETC  , TWOSTRING , ETC , TWOSTRING ,		// 16
		ETC , ONEINDEXSEARCH , ETC , ONEINDEXSEARCH , ETC , // 21
		TWODATE , ETC , ETC	, ONESTRING					// 24
	};

	TCHAR** sDBFieldName;
	/// ECO Search���
	if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		sDBFieldName = sEcoDBFieldName;
	else
		sDBFieldName = sCatDBFieldName;
	

	// �˻� �������� ��尡 _T("�ڼ��� >>") ����� ��츸 �˻� ���ǿ� �߰�
	CString sModeAlias;
	CESL_Control_Element *pCE = pCM->FindControlElement( _T("��������") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::GetSqlBO_LOC_1110( CString &sWhere )") );
	(m_pParentMgr->GetDlgItem( pCE->CTRL_ID ))->GetWindowText( sModeAlias );
	
	INT nMakeCnt;
	if( sModeAlias.Compare( _T("�󼼰˻�") ) == 0 )
		nMakeCnt = 9;
	else
		nMakeCnt = 25;

	// Where�� ����
	ids = CLocCommonAPI::MakeSql( sControlData , sDBFieldName , nDBFieldType , nMakeCnt , sWhere , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(  m_pParentMgr->m_hWnd ,-2 , _T("GetWhereBO_LOC_1410") );


	/// 2004�� 5�� 20�� ������ ����
	/// �Թ�ȣ�� ������ �˻� �����ϵ��� �ش޶�� ��û�� ����
	if( !sControlData[5].IsEmpty() )
	{
		if( !sWhere.IsEmpty() ) sWhere += _T(" AND ");

		if( !sControlData[6].IsEmpty() )
		{
			sWhere += _T(" S.BOX_NAME BETWEEN '") + sControlData[5] + _T("' AND '") + sControlData[6] +_T("' ");
		}
		else
		{
			sWhere += _T(" S.BOX_NAME LIKE '") + sControlData[5] + _T("%' ");
		}
	}



	/// 17 �ϰ� 19
	/// �׽�Ʈ�� ���� �ӽ� ����
	FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("rb"));
	BOOL bNewEcoSearch = FALSE;
	if( fd == NULL )
		bNewEcoSearch = TRUE;
	else
		fclose(fd);
	

	
	CString sEcoSearchWhere;
	INT nEcoSearchWhere = 0;
	if( nMakeCnt > 9 && bNewEcoSearch && 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
	{
		CMakeSearchData MakeSearchData(m_pParentMgr);	
		
		BOOL bIsEqualSearch ;
		/// ���� �˻� , ������ �˻�
		// ���ô� ������ �˻� ����
		if( !sControlData[17].IsEmpty() )
		{
			if( sControlData[17].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			else
				bIsEqualSearch = FALSE;

			
			MakeSearchData.GetKeyWord(sControlData[17]);
			if( !bIsEqualSearch ) 
				sControlData[17] += _T("*");
			
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_TITLE, '") + sControlData[17] + _T("') ");
			nEcoSearchWhere++;
		}
		
		if( !sControlData[19].IsEmpty() )
		{
			if( sControlData[19].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			else
				bIsEqualSearch = FALSE;
			
			MakeSearchData.GetKeyWord(sControlData[19]);
			if( !bIsEqualSearch ) 
				sControlData[19] += _T("*");
			
			if( !sEcoSearchWhere.IsEmpty() )
				sEcoSearchWhere += _T(" INTERSECT ");
			
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_PUBLISHER, '") + sControlData[19] + _T("') ");
			nEcoSearchWhere++;
		}
		
		if( !sEcoSearchWhere.IsEmpty() )
		{
			if( !sWhere.IsEmpty() )
				sWhere += _T(" AND ");
			if( nEcoSearchWhere == 1 )
				sWhere += _T(" I.REC_KEY IN ") + sEcoSearchWhere ;
			else
				sWhere += _T(" I.REC_KEY IN ( ") + sEcoSearchWhere +_T(")");
		}
		
	}
	

	


	// ISSN
	CString sISSN;
	if( !sControlData[23].IsEmpty() )
	{
		sISSN = sControlData[23];
		if( !sControlData[30].IsEmpty() )
			sISSN += sControlData[30];
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND I.ST_ISSN = '") + sISSN + _T("' ");
	}

	// + �ڷ��� ���� 
	// �������� �ƴϰ� �������� �ƴϰ� �������� �ƴ� ��ġ �ڷ�
	CString sWhere2;
	if( sControlData[29].Compare(_T("�ǽð��ΰ��ڷ�")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS = 'SEL111O'  ");
	}
	else if( sControlData[29].Compare(_T("�ǽð��ڷ�(�谡�ڷ�+�����ڷ�)")) == 0 )
	{
		//=====================================================
		//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 		sWhere2 = _T(" ( ( B.WORKING_STATUS IN ('SEL311O','SEL312O','SEL313O','SEL314O','SEL315O')  AND B.PREV_WORKING_STATUS = 'SEL112N') or B.WORKING_STATUS = 'SEL112N' ) ");
		// 18.09.27 JMJ KOL.RED.2018.008
		sWhere2 = _T(" ( ( B.WORKING_STATUS IN ('SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL411O','SEL511O','SEL611O')  AND B.PREV_WORKING_STATUS = 'SEL112N') or B.WORKING_STATUS = 'SEL112N' ) ");
		//=====================================================
	}
	else if( sControlData[29].Compare(_T("�ǽð��谡�ڷ�")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS = 'SEL112N' ");
	}
	else if( sControlData[29].Compare(_T("�����ڷ�")) == 0 )
	{
		//=====================================================
		//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 		sWhere2 = _T(" B.WORKING_STATUS IN ( 'SEL311O','SEL312O','SEL313O','SEL314O','SEL315O' )  AND B.PREV_WORKING_STATUS = 'SEL112N' ");
	// 18.09.27 JMJ KOL.RED.2018.008
		sWhere2 = _T(" B.WORKING_STATUS IN ( 'SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL411O','SEL511O','SEL611O' )  AND B.PREV_WORKING_STATUS = 'SEL112N' ");
		//=====================================================
	}
	else if( sControlData[29].Compare(_T("�����ڷ�")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS IN ('SEB111N','SER111N') AND B.PREV_WORKING_STATUS = 'SEL112N' ");
	}
	else if( sControlData[29].Compare(_T("����ΰ��ڷ�")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS = 'SER111N' AND B.PREV_WORKING_STATUS = 'SEL112N' ");
	}
	else if( sControlData[29].Compare(_T("�����ΰ��ڷ�")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS = 'SEB111N' AND B.PREV_WORKING_STATUS = 'SEL112N' ");
	}
	else if( sControlData[29].Compare(_T("����ڷ�")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS = 'SEL113O' AND B.PREV_WORKING_STATUS = 'SEL112N'");
	}

	if( !sWhere2.IsEmpty() )
	{
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		sWhere += sWhere2;
	}

	// �谡����
	sWhere2.Empty();
	if( !sControlData[25].IsEmpty() )
	{
		if( sControlData[26].IsEmpty() )
		{
			sWhere2.Format( _T("( (B.SHELF_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) OR ")
							_T("( B.SHELF_CHANGE_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) ) ") ,
							sControlData[25] , sControlData[25] , sControlData[25] , sControlData[25] 
							);
		}
		else
		{
			sWhere2.Format( _T("( (B.SHELF_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) OR ")
							_T("( B.SHELF_CHANGE_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) ) ") ,
							sControlData[25] , sControlData[26] , sControlData[25] , sControlData[26] 
							);
		}

	}
	if( !sWhere2.IsEmpty() )
	{
		sWhere += _T(" AND ");
		sWhere += sWhere2;
	}

	// ��������
	sWhere2.Empty();
	if( !sControlData[27].IsEmpty() )
	{
		if( sControlData[28].IsEmpty() )
		{
			sWhere2.Format( _T("( (B.BINDING_TRANSFER_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) OR ")
							_T("( B.REG_TRANSFER_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) ) ") ,
							sControlData[27] , sControlData[27] , sControlData[27] , sControlData[27] 
							);
		}
		else
		{
			sWhere2.Format( _T("( (B.BINDING_TRANSFER_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) OR ")
							_T("( B.REG_TRANSFER_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) ) ") ,
							sControlData[27] , sControlData[28] , sControlData[27] , sControlData[28] 
							);
		}

	}
	if( !sWhere2.IsEmpty() )
	{
		sWhere += _T(" AND ");
		sWhere += sWhere2;
	}

	return 0;
}


// ȭ�鿡 �����ֱ�
// 1. �ڵ� -> �������� Convert
// 2. �׸��� View
INT CBL_SE_LOC_15::ViewMainGrid()
{
	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1500");
	CString sDMAlias = _T("DM_SE_LOC_1500");

	// 1. �ڵ� -> ����
	// û����ȣ , ���±���
	ids = CLocCommonAPI::MakeShelfCode( m_pParentMgr , sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-1 , _T("ViewMainGrid") );


	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewMainGrid") );

	// �̿����ѱ���
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("�̿����ѱ���") , _T("�̿����ѱ���") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-2 , _T("ViewMainGrid") );
	
	// ��������
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("��������") , _T("��������") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-2 , _T("ViewMainGrid") );
	
	// �ڷ��
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("�ڷ�Ǳ���") , _T("�ڷ��") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-2 , _T("ViewMainGrid") );

	// �����
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("������ڵ�") , _T("�����") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-2 , _T("ViewMainGrid") );

	CString sWorkingStatus;	// �ڷ����
	CString sBackPath;		// ���� ���
	CString sAppendix;      // �ηϿ���
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		// �ڷ� ���� ����
		ids = pDM->GetCellData( _T("�ڷ����") , i , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewMainGrid") );
		
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("ViewMainGrid") );

		ids = pDM->SetCellData( _T("�ڷ����") , sWorkingStatus, i  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewMainGrid") );

		// ���� ��� ǥ��
		ids = pDM->GetCellData( _T("���Ͱ��") , i , sBackPath );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewMainGrid") );

		if( sBackPath.Compare(_T("Y")) == 0 )
			sBackPath=_T("�����ΰ��ڷ�");
		else
			sBackPath=_T("����ΰ��ڷ�");
		
		ids = pDM->SetCellData( _T("���Ͱ��") , sBackPath, i  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewMainGrid") );		

		ids = pDM->GetCellData( _T("�ηϸ�") , i , sAppendix );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewMainGrid") );
		if( sAppendix != _T("") )
			sAppendix = _T("�η�");

		ids = pDM->SetCellData(_T("�ηϿ���"),sAppendix, i);
	}

	CString strMsg;		
	if( pDM->GetRowCount() == 0 )
		strMsg.Format( _T("�˻��� �ڷᰡ �������� �ʽ��ϴ�.") );
	m_pParentMgr->GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));		
	

	// 2. �׸��� View
	ids = m_pParentMgr->AllControlDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-7 , _T("ViewMainGrid") );

	// 3. �� å�� ǥ��
	ids = DisplaySpeciesBookCnt();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-8 , _T("ViewMainGrid") );


	

	return 0;
}

/// ���� ȭ�鿡 ���� å���� �ѷ��ش�.
INT CBL_SE_LOC_15::DisplaySpeciesBookCnt()
{
	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1500");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewMainGrid") );

	// å�� ���ؿ���
	CString sBookCnt;
	sBookCnt.Format( _T("%d") , pDM->GetRowCount() );
	ids = m_pParentMgr->SetControlData( _T("CM_SE_LOC_1500_ETC") , _T("å��") , sBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("ViewMainGrid") );

	// ���� ���ؿ���
	CString sSpeciesCnt;
	ids = CLocCommonAPI::GetDMEqualRemoveCnt( m_pParentMgr , pDM , _T("������KEY") , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("ViewMainGrid") );
	ids = m_pParentMgr->SetControlData( _T("CM_SE_LOC_1500_ETC") , _T("����") , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("ViewMainGrid") );

	return 0;

}

/// �׸��忡 ���õ� �ڷᰡ �ִ��� �˻�
INT CBL_SE_LOC_15::GetSelectMainGridCnt()
{
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetSelectMainGridCnt") );

	if( pDM->GetRowCount() < 1 ) return 0;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetSelectMainGridCnt") );

	pGrid->SelectMakeList();

	if( pGrid->SelectGetCount() < 1 ) return 0;

	return pGrid->SelectGetCount();
}


/// ȭ���۾�
/// ���� �׸��� ������ ����
CESL_Grid* CBL_SE_LOC_15::GetMainGridPtr()
{
	CString sCMAlias = _T("CM_SE_LOC_1500");
	CString sGridAlias = _T("���α׸���");

	// CM ����
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) return NULL;
	
	return pGrid;
}



// �谡�� ���
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. �谡���� ����Ҽ� �ִ� �ڷ����� �˻�
/// 2. å���� UPDATE 
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�
INT CBL_SE_LOC_15::RecordShelfDate()
{
	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// �谡�ϱ������ �Է°� �˻� - �谡����,�谡�����
	CString sShelfDate,sWorker;
	ids = GetInputShelfRecordData( sShelfDate,sWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sShelfDate.IsEmpty() )
	{
		AfxMessageBox( _T("�谡���ڸ� �Է����ֽʽÿ�.") );
		return 2;
	}
	if( sWorker.IsEmpty() )
	{
		AfxMessageBox( _T("�۾��ڸ� �Է����ֽʽÿ�.") );
		return 3;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus; // �ڷ����

	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("�ڷ����") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sWorkingStatus.Compare( _T("�ǽð��ΰ��ڷ�") ) != 0 )
		{
			pGrid->SelectGetNext();
			continue;
		}

		// �谡�� ��� DB�۾�
		ids = RecordShelfDateDBProc( nIndex , &TmpDM , sShelfDate , sWorker );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// ���� ��� DM �۾�
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// ���� ��� ȭ�� �۾�
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n\r\n�谡�� ��� �ڷ� : '%d'") , nSelectCnt , nSuccessCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n\r\n������ �ڷ� : '%d'(�谡�� ��� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.)") , nSelectCnt , nSelectCnt );
	else
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n\r\n�谡�� ��� �ڷ� : '%d'\r\n")
						   _T("������ �ڷ� : '%d'(�ڷ���°� �谡�� ��� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );
	
	AfxMessageBox( sResultMsg );

	return 0;
}

/// �谡�� ��� DB �۾�
/// 1. å���� UPDATE
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CBL_SE_LOC_15::RecordShelfDateDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sShelfDate,CString sWorker )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordShelfDateDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordShelfDateDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordShelfDateDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordShelfDateDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�ڷ����") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RecordShelfDateDBProc") ); 

	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL112N") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , sShelfDate );
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , sWorker );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM�� ���
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("�ڷ����") , _T("�ǽð��谡�ڷ�") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordShelfDateDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����ڷ����") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RecordShelfDateDBProc") ); 
	ids = pTmpDM->SetCellData( _T("���ʹ谡����") , sShelfDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordShelfDateDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡�����") , sWorker , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RecordShelfDateDBProc") ); 


	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RecordShelfDateDBProc") ); 

	// 4. �������� SPECIES_CLASS ����
	CString sSpeciesKey, sVolKey;
	ids = pTmpDM->GetCellData( _T("������KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = pTmpDM->GetCellData( _T("��ȣKEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 

	ids = SettingSpeciesField( sSpeciesKey , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("RecordShelfDateDBProc") ); 

	return 0;
}

/// �ڷ� ���濡 ������ DM�� ������ ���� ���Ǵ� DM�� ����Ѵ�.
INT CBL_SE_LOC_15::DBInsertUpdateDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

	// ���� �߰�
	ids = pDM->SetCellData( _T("��Ϲ�ȣ"), pTmpDM->GetCellData( _T("����Ϲ�ȣ"), 0 ), nDMIndex );
	if( ids < 0 ) 
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNoDBProc") ); 
	
	return 0;
}

/// 1. INDEX�κ��� �׸����� ���� ����
INT CBL_SE_LOC_15::DBInsertUpdateGridProc( INT nDMIndex )
{
	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairGridProc") );

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairGridProc") );

	return 0;
}


/// �谡�� ��Ͽ� �ʿ��� �Է°��� �����´�.
INT CBL_SE_LOC_15::GetInputShelfRecordData( CString &sShelfDate,CString &sWorker )
{
	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1530");
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�谡����") , sShelfDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sShelfDate.TrimLeft(); sShelfDate.TrimRight();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�谡�����") , sWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetInputShelfRecordData") );
	sWorker.TrimLeft(); sWorker.TrimRight();

	return 0;

}


// ���α׷����� ����
CLocProgressBarDlg* CBL_SE_LOC_15::CreateProgressBarDlg( INT nUpper )
{
	CLocProgressBarDlg* pProgressBar = new CLocProgressBarDlg(m_pParentMgr);
	pProgressBar->Create( m_pParentMgr );
	pProgressBar->ShowWindow( SW_SHOW );
	pProgressBar->InitRange(nUpper);

	return pProgressBar;
}

// ���α׷����� ����
INT CBL_SE_LOC_15::DeleteProgressBarDlg( CLocProgressBarDlg *pProgressBarDlg )
{
	if( pProgressBarDlg != NULL ) 
		delete pProgressBarDlg;
	pProgressBarDlg = NULL;
	
	return 0;
}




// ����Ϲ�ȣ �ο�
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. ����Ϲ�ȣ�� ����Ҽ� �ִ� �ڷ����� �˻�
/// 2. å���� UPDATE 
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�
INT CBL_SE_LOC_15::RecordRegNo()
{
	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordRegNo") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordRegNo") );

	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus; // �ڷ����

	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("�ڷ����") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordRegNo") );

		if( sWorkingStatus.Compare( _T("�ǽð��ΰ��ڷ�") ) != 0 && ( sWorkingStatus.Compare( _T("�ǽð��谡�ڷ�") ) != 0 ) )
		{
			pGrid->SelectGetNext();
			continue;
		}

		// �谡�� ��� DB�۾�
		ids = RecordRegNoDBProc( nIndex , &TmpDM );
		if( ids < 0 ) 
		{
			DeleteProgressBarDlg( pProgressBar );
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordRegNo") );
		}
		if( ids > 0 ) return ids;

		// DM �۾�
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RecordRegNo") );

		// ȭ�� �۾�
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNo") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷῡ ��Ϲ�ȣ�� �ο��Ͽ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ��Ϲ�ȣ�� �ο� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n���� �ڷ� : '%d'\r\n")
						   _T("���� �ڷ� : '%d'(�ڷ���°� ��Ϲ�ȣ�� �ο� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;
}

/// ����Ϲ�ȣ �ο� DB �۾�
/// 1. å���� UPDATE
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CBL_SE_LOC_15::RecordRegNoDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;

	CString sRegNo;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordRegNoDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordRegNoDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordRegNoDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey ;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordRegNoDBProc") ); 

	// 5163���� select
	CString strIs5163;
	m_pParentMgr->GetManageValue( _T("��Ÿ") , _T("����") , _T("5163") , _T("5163") , strIs5163 );

	// 050116_�����߰� ++++++++++++++++++++++++++++++++++++++++++++++++
	// 5163�ϰ�� ����Ϲ�ȣ ���ϱ�( S0/HS + 2005 + 6�ڸ� �Ϸù�ȣ)
	// ���� ������ ( 2005 + 8�ڸ� �Ϸù�ȣ)
	if ( _T("Y") == strIs5163 )
		ids = MakeTmpRegNo_5163(sRegNo);
	else 
		ids = MakeTmpRegNo(sRegNo);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RecordRegNoDBProc") );

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , sRegNo );

	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("����Ϲ�ȣ") , sRegNo , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNoDBProc") ); 

	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RecordRegNoDBProc") ); 

	return 0;
}


/// ����Ϲ�ȣ ���� & ���� DB�۾�
INT CBL_SE_LOC_15::MakeTmpRegNo( CString &sRegNo )
{
	INT ids;

	CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM( _T("DM_TMP") );
	if( pTmpDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeTmpRegNo") ); 

	CString sYear;
	CString sSql;
	CString sTailRegNo;

	sYear.Format( _T("%d") , CLocCommonAPI::GetDBTime(m_pParentMgr).GetYear() );

	// ������ ��Ϲ�ȣ ����
	sSql.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),sYear);
	ids = pTmpDM->GetOneValueQuery( sSql , sTailRegNo );
	// 2008.06.06 REM BY PDJ : ���� ���� ��� - �������Ƿ� �����ϸ� �ȵ�.
	//if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeTmpRegNo") );

	// ������ ��Ϲ�ȣ�� �������� ���� ��� ����
	if( sTailRegNo.IsEmpty() )
	{
		sSql.Format( _T("INSERT INTO CO_LAST_NUMBER_TBL(REC_KEY,KIND_CODE,PUBLISH_FORM_CODE,MAKE_YEAR,LAST_NUMBER,CREATE_DATE,MANAGE_CODE) ")
											_T("VALUES(esl_seq.NextVal,'TRG_NO','CO','%s',1,SYSDATE,UDF_MANAGE_CODE);") , sYear );
		ids = pTmpDM->ExecuteQuery( sSql , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeTmpRegNo") ); 

		sTailRegNo = _T("1");
	}

	sRegNo.Format( _T("%s%08s"),sYear,sTailRegNo);
	//2018�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
	//KOL.RED.019-001 JY ���Ӱ��๰ ��� ����ȭ
	//ADD=================================================================================================
	sSql.Empty();
	sSql.Format( 
		_T("SELECT (SELECT MAX(REG_NO) FROM SE_BOOK_TBL WHERE REG_NO LIKE '%s________'  AND MANAGE_CODE = UDF_MANAGE_CODE) AS REG_NO ")
		_T("FROM DUAL WHERE ")
		_T("EXISTS (SELECT REG_NO FROM SE_BOOK_TBL WHERE REG_NO = '%s'  AND MANAGE_CODE = UDF_MANAGE_CODE) ")
	,sYear,sRegNo);

	CString strValue = _T("");
	pTmpDM->GetOneValueQuery( sSql , strValue );

	if ( !strValue.IsEmpty() ) {
		int tmpValue = _ttoi(strValue.Right(4));
		sTailRegNo.Format( _T("%d"), tmpValue );
		sRegNo.Format( _T("%s%08d"),sYear,tmpValue +1 );
	}
	
	sSql.Empty();
	//====================================================================================================

	// ������ ��Ϲ�ȣ ++
	sSql.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
					_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND KIND_CODE='TRG_NO' AND MAKE_YEAR='%s';")
					, _ttoi(sTailRegNo)+1 , sYear 
					);

	ids = pTmpDM->ExecuteQuery( sSql , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeTmpRegNo") ); 

	return 0;
}


/// ����Ϲ�ȣ ���� & ���� DB�۾�(050116_�����߰�)
INT CBL_SE_LOC_15::MakeTmpRegNo_5163( CString &sRegNo )
{
	INT ids;

	CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM( _T("DM_TMP") );
	if( pTmpDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeTmpRegNo") ); 

	CString sYear;
	CString sSql;
	CString sTailRegNo;

	sYear.Format( _T("%d") , CLocCommonAPI::GetDBTime(m_pParentMgr).GetYear() );

	// ������ ��Ϲ�ȣ ����
	sSql.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),sYear);
	pTmpDM->GetOneValueQuery( sSql , sTailRegNo );

	// ������ ��Ϲ�ȣ�� �������� ���� ��� ����
	if( sTailRegNo.IsEmpty() )
	{
		sSql.Format( _T("INSERT INTO CO_LAST_NUMBER_TBL(REC_KEY,KIND_CODE,PUBLISH_FORM_CODE,MAKE_YEAR,LAST_NUMBER,CREATE_DATE,MANAGE_CODE) ")
											_T("VALUES(esl_seq.NextVal,'TRG_NO','CO','%s',1,SYSDATE,UDF_MANAGE_CODE);") , sYear );
		ids = pTmpDM->ExecuteQuery( sSql , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeTmpRegNo") ); 

		sTailRegNo = _T("1");
	}

	CString strRegCode = _T("");
	CESL_DataMgr *pTmpRegNoDM = m_pParentMgr->FindDM( _T("DM_SE_LOC_1500_TMP_REGNO") );
	if( pTmpRegNoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeTmpRegNo") ); 
	strRegCode = pTmpRegNoDM->GetCellData(_T("����ϱ���"), 0);
	//ids = m_pParentMgr->GetDataMgrData(_T("DM_SE_LOC_1500_TMP_REGNO"), _T("����ϱ���"), strRegCode);

	sRegNo.Format( _T("%s%s%06s"),strRegCode, sYear,sTailRegNo);

	// ������ ��Ϲ�ȣ ++
	sSql.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
					_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND KIND_CODE='TRG_NO' AND MAKE_YEAR='%s';")
					, _ttoi(sTailRegNo)+1 , sYear 
					);

	ids = pTmpDM->ExecuteQuery( sSql , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeTmpRegNo") ); 

	return 0;
}



/// �谡 ���
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. �谡����Ҽ� �ִ� �ڷ����� �˻�
/// 2. å���� UPDATE 
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�
INT CBL_SE_LOC_15::CancelShelf()
{
	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}


	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus; // �ڷ����

	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("�ڷ����") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sWorkingStatus.Compare( _T("�ǽð��谡�ڷ�") ) != 0 )
		{
			pGrid->SelectGetNext();
			continue;
		}

		// �谡 ��� DB�۾�
		ids = CancelShelfDBProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// DM �۾�
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// ȭ�� �۾�
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷḦ �谡����Ͽ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ �谡��� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n\r\n���� �ڷ� : '%d'\r\n")
						   _T("���� �ڷ� : '%d'(�ڷ���°� �谡��� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;
}

/// �谡 ��� DB �۾�
/// 1. å���� UPDATE
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CBL_SE_LOC_15::CancelShelfDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelShelfDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelShelfDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CancelShelfDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�ڷ����") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelShelfDBProc") ); 

	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL111O") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , _T("") );
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_DATE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_BEFORE_LOC") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_REASON_CODE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_ETC_REASON") , _T("STRING") , _T(""));
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM�� ���
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("�ڷ����") , _T("�ǽð��ΰ��ڷ�") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����ڷ����") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("���ʹ谡����") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡�����") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡��������") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡��������ڵ�") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡�����������") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelShelfDBProc") ); 


	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CancelShelfDBProc") ); 

	// 4. �������� SPECIES_CLASS ����
	// 4. �������� SPECIES_CLASS ����
	CString sSpeciesKey, sVolKey;
	ids = pTmpDM->GetCellData( _T("������KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = pTmpDM->GetCellData( _T("��ȣKEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = SettingSpeciesField( sSpeciesKey , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("RecordShelfDateDBProc") ); 
	
	return 0;

}


/// ����Ϲ�ȣ ���
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. ����Ϲ�ȣ ����Ҽ� �ִ� �ڷ����� �˻�
/// 2. å���� UPDATE 
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�
INT CBL_SE_LOC_15::CancelRegNo()
{
	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordRegNo") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordRegNo") );

	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus; // �ڷ����

	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("�ڷ����") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordRegNo") );

		if( sWorkingStatus.Compare( _T("�ǽð��ΰ��ڷ�") ) != 0 && ( sWorkingStatus.Compare( _T("�ǽð��谡�ڷ�") ) != 0 ) )
		{
			pGrid->SelectGetNext();
			continue;
		}

		// ����Ϲ�ȣ ��� DB�۾�
		ids = CancelRegNoDBProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordRegNo") );
		if( ids > 0 ) return ids;

		// DM �۾�
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RecordRegNo") );

		// ȭ�� �۾�
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNo") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷῡ ��Ϲ�ȣ�� �����Ͽ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ��Ϲ�ȣ�� ���� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n\r\n���� �ڷ� : '%d'\r\n")
						   _T("���� �ڷ� : '%d'(�ڷ���°� ��Ϲ�ȣ�� ���� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;
}
/// ����Ϲ�ȣ ���� DB �۾�
/// 1. å���� UPDATE
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CBL_SE_LOC_15::CancelRegNoDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;

	CString sRegNo;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordRegNoDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordRegNoDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordRegNoDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey ;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordRegNoDBProc") ); 

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , _T("") );

	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("����Ϲ�ȣ") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNoDBProc") ); 


	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RecordRegNoDBProc") ); 

	return 0;

}


/// �Թ�ȣ ����
/// 1. �Թ�ȣ �����Ҽ� �ִ� �ڷ����� �˻�
/// 2. ������ UPDATE 
/// 3. DM ����
/// 4. �׸��� ����
INT CBL_SE_LOC_15::ChangeBoxName()
{
	INT ids;

	INT nCurrentIndex;
	CString sBoxName;
	GetTempData( _T("�����ε���") , nCurrentIndex );
	GetTempData( _T("�Թ�ȣ") , sBoxName );

	// �׸��� ������ ����
	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordRegNo") );

	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus; // �ڷ����


	CESL_DataMgr TmpDM;

	// �Թ�ȣ ���� DB�۾�
	ids = ChnageBoxNameDBProc( nCurrentIndex , &TmpDM , sBoxName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordRegNo") );
	if( ids > 0 ) return ids;

	// DM �۾�
	ids = DBChangeBoxNameDMProc( nCurrentIndex , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RecordRegNo") );

	// ȭ�� �۾�
	ids = DBChangeBoxNameGridProc(nCurrentIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNo") );


	CString sResultMsg;
	sResultMsg.Format(_T("�Թ�ȣ�� ����Ǿ����ϴ�."));
	AfxMessageBox( sResultMsg );

	return 0;
}

/// �Թ�ȣ ���� DB �۾�
/// 1. ������ UPDATE
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CBL_SE_LOC_15::ChnageBoxNameDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sBoxName)
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordRegNoDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordRegNoDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordRegNoDBProc") ); 

	pDM->StartFrame();

	CString sSpeciesRecKey ;
	ids = pTmpDM->GetCellData( _T("������KEY") , 0 , sSpeciesRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordRegNoDBProc") ); 

	// ������ ����
	// 1. ������ UPDATE
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("BOX_NAME") , _T("STRING") , sBoxName );

	pDM->MakeUpdateFrame( _T("SE_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("�Թ�ȣ") , sBoxName , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNoDBProc") ); 


	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RecordRegNoDBProc") ); 

	return 0;
}


/// �ڷ� ���濡 ������ DM�� ������ ���� ���Ǵ� DM�� ����Ѵ�.
INT CBL_SE_LOC_15::DBChangeBoxNameDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDMProc") );

	CString sSpeciesKey;
	CString sTmpSpeciesKey;
	ids = pDM->GetCellData( _T("������KEY") , nDMIndex , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

	CString sBoxName;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("������KEY") , i , sTmpSpeciesKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

		if( sTmpSpeciesKey.Compare(sSpeciesKey) != 0 ) continue;

		ids = pTmpDM->GetCellData( _T("�Թ�ȣ") , 0 , sBoxName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

		ids = pDM->SetCellData( _T("�Թ�ȣ") , sBoxName , i );

	}

	return 0;
}

/// 1. INDEX�κ��� �׸����� ���� ����
INT CBL_SE_LOC_15::DBChangeBoxNameGridProc( INT nDMIndex )
{
	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairGridProc") );

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDMProc") );

	CString sSpeciesKey;
	CString sTmpSpeciesKey;
	ids = pDM->GetCellData( _T("������KEY") , nDMIndex , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("������KEY") , i , sTmpSpeciesKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

		if( sTmpSpeciesKey.Compare(sSpeciesKey) != 0 ) continue;

		ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , i , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairGridProc") );
	}


	return 0;
}



/// ������ ����
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. ������ �����Ҽ� �ִ� �ڷ����� �˻�
/// 2. å���� UPDATE 
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�
INT CBL_SE_LOC_15::RestoreData()
{
	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// ������ ���� ���� �Է°� �˻� - ���� ����
	CString sRestoreDate;
	ids = GetInputRestoreDate( sRestoreDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sRestoreDate.IsEmpty() )
	{
		AfxMessageBox( _T("�������ڸ� �Է����ֽʽÿ�.") );
		return 2;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nCopySuccessCnt = 0;	// �����ΰ迡 �۾��� ������ �ڷ� �� 
	INT nRegSuccessCnt = 0;		// ����ΰ迡 �۾��� ������ �ڷ� �� 
	CString sWorkingStatus;			// �ڷ����
	CString sRestoreWorkingStatus;	// ���Ͱ��

	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("�ڷ����") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sWorkingStatus.Compare( _T("�ǽð��谡�ڷ�") ) != 0  )
		{
			pGrid->SelectGetNext();
			continue;
		}

		ids = pDM->GetCellData( _T("���Ͱ��") , nIndex , sRestoreWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		//  DB�۾�
		ids = RestoreDataDBProc( nIndex , &TmpDM , sRestoreDate , sRestoreWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// DM �۾�
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// ȭ�� �۾�
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		if( sRestoreWorkingStatus.Compare(_T("����ΰ��ڷ�")) == 0 )
			nRegSuccessCnt++;
		else if( sRestoreWorkingStatus.Compare(_T("�����ΰ��ڷ�")) == 0 )
			nCopySuccessCnt++;

		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nCopySuccessCnt>0 || (nRegSuccessCnt > 0)  )
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n�����ΰ��ڷ� : '%d'\r\n����ΰ��ڷ� : '%d' ") 
						, nSelectCnt , nCopySuccessCnt , nRegSuccessCnt  );
	else
		sResultMsg.Format( _T("�����Ͻ� �ڷ��� ���� ������ �ڷᰡ �������� �ʽ��ϴ�."));

	AfxMessageBox( sResultMsg );

	return 0;
}

/// ������ ���� DB �۾�
/// 1. å���� UPDATE
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CBL_SE_LOC_15::RestoreDataDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sRestoreDate , CString sRestoreWorkingStatus )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RestoreDataDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RestoreDataDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RestoreDataDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�ڷ����") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RestoreDataDBProc") ); 

	CString sRestoreFieldName;
	if( sRestoreWorkingStatus.Compare(_T("�����ΰ��ڷ�"))==0 )
		sRestoreFieldName = _T("BINDING_TRANSFER_DATE");
	else
		sRestoreFieldName = _T("REG_TRANSFER_DATE");


	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
	CLocCommonAPI::GetWorkingStatusDescToCode( sRestoreWorkingStatus , 0 );

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sRestoreWorkingStatus );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->AddDBFieldData( sRestoreFieldName , _T("STRING") , sRestoreDate );
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_DATE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_BEFORE_LOC") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_REASON_CODE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_ETC_REASON") , _T("STRING") , _T(""));

	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM�� ���
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
	CLocCommonAPI::GetWorkingStatusDescToCode( sRestoreWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("�ڷ����") , sRestoreWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����ڷ����") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("���ʹ谡����") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡�����") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("����Ϲ�ȣ") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡��������") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡��������ڵ�") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡�����������") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 


	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RestoreDataDBProc") ); 

	// 4. �������� SPECIES_CLASS ����
	// 4. �������� SPECIES_CLASS ����
	CString sSpeciesKey, sVolKey;
	ids = pTmpDM->GetCellData( _T("������KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = pTmpDM->GetCellData( _T("��ȣKEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = SettingSpeciesField( sSpeciesKey , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("RecordShelfDateDBProc") ); 
	
	return 0;
}


/// ������ ���� ���� ����
INT CBL_SE_LOC_15::GetInputRestoreDate( CString &sRestoreDate )
{
	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1520");
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("��������") , sRestoreDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sRestoreDate.TrimLeft(); sRestoreDate.TrimRight();

	return 0;
}


/// ���� ���
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. ��������Ҽ� �ִ� �ڷ����� �˻�
/// 2. å���� UPDATE 
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�
INT CBL_SE_LOC_15::CancelRestore()
{
	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// ����������� �Է°� �˻� - �谡����,�谡�����
	CString sShelfDate,sWorker;
	ids = CancelRestoreData( sShelfDate,sWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sShelfDate.IsEmpty() )
	{
		AfxMessageBox( _T("�谡���ڸ� �Է����ֽʽÿ�.") );
		return 2;
	}
	if( sWorker.IsEmpty() )
	{
		AfxMessageBox( _T("�۾��ڸ� �Է����ֽʽÿ�.") );
		return 3;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus; // �ڷ����

	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("�ڷ����") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sWorkingStatus.Compare( _T("����ΰ��ڷ�") ) != 0 && (sWorkingStatus.Compare( _T("�����ΰ��ڷ�") ) != 0))
		{
			pGrid->SelectGetNext();
			continue;
		}

		// ���� ��� DB�۾�
		ids = CancelRestoreDBProc( nIndex , &TmpDM ,sShelfDate , sWorker );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// DM �۾�
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// ȭ�� �۾�
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷḦ ��������Ͽ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ������� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n���� �ڷ� : '%d'\r\n")
						   _T("���� �ڷ� : '%d'(�ڷ���°� ������� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;
}


/// ���� ��� DB �۾�
/// 1. å���� UPDATE
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CBL_SE_LOC_15::CancelRestoreDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sShelfDate , CString sWorker )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelRestoreDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelRestoreDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CancelRestoreDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelRestoreDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�ڷ����") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelRestoreDBProc") ); 

	CString sRestoreFieldName;
	if( sWorkingStatus.Compare(_T("�����ΰ��ڷ�"))==0 )
		sRestoreFieldName = _T("BINDING_TRANSFER_DATE");
	else
		sRestoreFieldName = _T("REG_TRANSFER_DATE");


	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL112N") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->AddDBFieldData( sRestoreFieldName , _T("STRING") , _T("") );
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , sShelfDate );
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , sWorker );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM�� ���
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("�ڷ����") , _T("�ǽð��谡�ڷ�") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CancelRestoreDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����ڷ����") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRestoreDBProc") ); 
	ids = pTmpDM->SetCellData( _T("���ʹ谡����") , sShelfDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRestoreDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡�����") , sWorker , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRestoreDBProc") ); 


	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CancelRestoreDBProc") ); 

	// 4. �������� SPECIES_CLASS ����
	// 4. �������� SPECIES_CLASS ����
	CString sSpeciesKey, sVolKey;
	ids = pTmpDM->GetCellData( _T("������KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = pTmpDM->GetCellData( _T("��ȣKEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = SettingSpeciesField( sSpeciesKey , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("RecordShelfDateDBProc") ); 
	
	return 0;
}


/// ���� ��ҿ� �ʿ��� �Է°��� �����´�.
INT CBL_SE_LOC_15::CancelRestoreData( CString &sShelfDate,CString &sWorker )
{
	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1520");
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�谡����") , sShelfDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sShelfDate.TrimLeft(); sShelfDate.TrimRight();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�谡�����") , sWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sWorker.TrimLeft(); sWorker.TrimRight();

	return 0;
}



/// ������ ���
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. ����Ҽ� �ִ� �ڷ����� �˻�
/// 2. å���� UPDATE 
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�
INT CBL_SE_LOC_15::RemoveData()
{
	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// ������ ���� ���� �Է°� �˻� - ���� ����
	CString sRemoveDate;
	ids = GetInputRemoveDate( sRemoveDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sRemoveDate.IsEmpty() )
	{
		AfxMessageBox( _T("������ڸ� �Է����ֽʽÿ�.") );
		return 2;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus;			// �ڷ����
	CString sRestoreWorkingStatus;	// ���Ͱ��

	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("�ڷ����") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sWorkingStatus.Compare( _T("�ǽð��谡�ڷ�") ) != 0  )
		{
			pGrid->SelectGetNext();
			continue;
		}

		//  DB�۾�
		ids = RemoveDataDBProc( nIndex , &TmpDM , sRemoveDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// DM �۾�
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// ȭ�� �۾�
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		nSuccessCnt++;

		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSuccessCnt > 0   )
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n\r\n���� �ڷ� : '%d' ") 
						, nSelectCnt , nSuccessCnt);
	else
		sResultMsg.Format( _T("�����Ͻ� �ڷ��� ��� ������ �ڷᰡ �������� �ʽ��ϴ�."));

	AfxMessageBox( sResultMsg );

	return 0;
}

/// ������ ���� DB �۾�
/// 1. å���� UPDATE
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CBL_SE_LOC_15::RemoveDataDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sRemoveDate )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RemoveDataDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RemoveDataDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RemoveDataDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�ڷ����") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RemoveDataDBProc") ); 

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL113O") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , _T("SEL112N") );
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("DISUSE_PROC_DATE") , _T("STRING") , sRemoveDate );
	pDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_DATE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_BEFORE_LOC") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_REASON_CODE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_ETC_REASON") , _T("STRING") , _T(""));

	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM�� ���

	ids = pTmpDM->SetCellData( _T("�ڷ����") , _T("�ǽð�����ڷ�") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����ڷ����") , _T("�ǽð��谡�ڷ�") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("���ʹ谡����") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡�����") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("����Ϲ�ȣ") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡��������") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡��������ڵ�") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡�����������") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 


	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RemoveDataDBProc") ); 

	
	// 4. �������� SPECIES_CLASS ����
	// 4. �������� SPECIES_CLASS ����
	CString sSpeciesKey, sVolKey;
	ids = pTmpDM->GetCellData( _T("������KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = pTmpDM->GetCellData( _T("��ȣKEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = SettingSpeciesField( sSpeciesKey , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("RecordShelfDateDBProc") ); 
	
	return 0;
}

/// ������ ���� ����(DB����)
/// ��� ���¿� ���ؼ� ���� ����(�� ���� ������ �ڷῡ ���ؼ��� ������ ���� �ʴ´�.)
/// (������ �ڷῡ ���� �ϰ� ó��)
/// 1. ������ �� �ִ� �ڷ����� �˻�
///   1.1 ���� ������ ���ϴ��� ���� �� �ǽ�
/// 2. å���� UPDATE
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�
INT CBL_SE_LOC_15::RemoveComplete()
{
	EFS_BEGIN

	INT ids = 0;
	INT nSelectCnt = 0;
	INT nIndex = 0;
	INT nSuccessCnt = 0;
	CString sMsg = _T("");
	CString sWorkingStatus = _T("");
	CString sBookKey = _T("");
	CString sAcqTypeVolKey = _T("");
	CString sVolKey = _T("");
	CString sSpeciesKey = _T("");
	CString sBookType = _T("");
	CString sVolTitle = _T("");
	CString sCMAlias = _T("CM_SE_LOC_1500");
	// 18.09.27 JMJ KOL.RED.2018.008
	CString sLoanStatus = _T("���Ӱ��ܴ����ڷ�, ���Ӱ��������ڷ�, ����Ư�������ڷ�, ���ӱ�޴����ڷ�, ���ӹ���ڷ�, ����å�η����������ڷ�");

	CSeApi* pRemove = new CSeApi( m_pParentMgr );
	if ( NULL==pRemove ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1, _T("RemoveComplete") ); 

	// CM ����
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1, _T("RemoveComplete") ); 

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -2, _T("RemoveComplete") );

	pGrid->SelectMakeList();
	nIndex = pGrid->SelectGetTailPosition();
	nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}
	else if ( nSelectCnt > 0 ) 
	{
		// 1. ���� ������ ���ϴ��� ����
		sMsg.Format(_T("������ DB���� �����Ͱ� �����Ǿ� ������ �Ұ����մϴ�.\r\n")
					_T("%d���� �ڷḦ ������ �����Ͻðڽ��ϱ�?"), nSelectCnt );
		ids = AfxMessageBox( sMsg, MB_YESNO );
		if ( IDNO==ids )	return 2;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -3, _T("RemoveComplete") );

	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	// å���� Frame�� ���� DB�� update�Ѵ�.
	for ( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		// �ڷ���°� ������ �ƴϸ� ������ �����Ѵ�.
		ids = pDM->GetCellData( _T("�ڷ����"), nIndex, sWorkingStatus );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("RemoveComplete") );
		if ( sLoanStatus.Find( sWorkingStatus ) > -1 )	continue;

		ids = pDM->GetCellData( _T("å����KEY"), nIndex, sBookKey );
		ids = pDM->GetCellData( _T("���Ա��к���KEY"), nIndex, sAcqTypeVolKey );
		ids = pDM->GetCellData( _T("��ȣKEY"), nIndex, sVolKey );
		ids = pDM->GetCellData( _T("������KEY"), nIndex, sSpeciesKey );
		ids = pDM->GetCellData( _T("å����"), nIndex, sBookType );
		ids = pDM->GetCellData( _T("��ȣ��"), nIndex, sVolTitle );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("RemoveComplete") );

		// 2. å���� UPDATE
		// ������ å ������ ���� ���ൿ���� ��ũ�� �ɾ ������ �� �ְ��Ѵ�.
		ids = pRemove->BOOK_CAT_DELETE( pDM			, sBookKey		, sAcqTypeVolKey,
										sVolKey		, sSpeciesKey	, sBookType		,
										sVolTitle	, FALSE			, FALSE );
		if ( ids < 0 ) 
		{
			//===================================================
			//2009.05.11 ADD BY PJW : �Ϲ��ڷ��ϰ��� ��������Ѵ�.
			if(ids == -1004) 
			{
				if ( (i+1)!=nSelectCnt )
				{
					nIndex = pGrid->SelectGetPrev( );
				}
				continue;
			}
			//===================================================
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -5, _T("RemoveComplete") );
		}


		// 2.1 ��/ȣ�� �谡���� �� ����.
		ids = SettingSpeciesField( sSpeciesKey , sVolKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RemoveComplete") ); 

		// 3. DM ����
		ids = pDM->DeleteRow( nIndex );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -7, _T("RemoveComplete") );

		nSuccessCnt++;

		if ( (i+1)!=nSelectCnt )
			nIndex = pGrid->SelectGetPrev( );
	}

	// 4. �׸��� ����
	ids = pCM->AllControlDisplay( );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -8, _T("RemoveComplete") );

	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );
	// ���� ��ü ����
	delete pRemove;
	pRemove = NULL;

	if( nSuccessCnt > 0   )
		sMsg.Format( _T("���� �ڷ� : '%d'\r\n\r\n���� �ڷ� : '%d' ") 
						, nSelectCnt , nSuccessCnt);
	else
		sMsg.Format( _T("�����Ͻ� �ڷ��� ���� ������ �ڷᰡ �������� �ʽ��ϴ�."));

	AfxMessageBox( sMsg );

	return 0;

	EFS_END
	return -1;
}

/// ��� ���
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. �������Ҽ� �ִ� �ڷ����� �˻�
/// 2. å���� UPDATE 
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�
INT CBL_SE_LOC_15::CancelRemove()
{
	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// ����������� �Է°� �˻� - �谡����,�谡�����
	CString sShelfDate,sWorker;
	ids = CancelRemoveData( sShelfDate,sWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sShelfDate.IsEmpty() )
	{
		AfxMessageBox( _T("�谡���ڸ� �Է����ֽʽÿ�.") );
		return 2;
	}
	if( sWorker.IsEmpty() )
	{
		AfxMessageBox( _T("�۾��ڸ� �Է����ֽʽÿ�.") );
		return 3;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus; // �ڷ����

	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("�ڷ����") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sWorkingStatus.Compare( _T("�ǽð�����ڷ�") ) != 0)
		{
			pGrid->SelectGetNext();
			continue;
		}

		// ���� ��� DB�۾�
		ids = CancelRemoveDBProc( nIndex , &TmpDM ,sShelfDate , sWorker );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// DM �۾�
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// ȭ�� �۾�
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷḦ �������Ͽ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ������ �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n\r\n���� �ڷ� : '%d'\r\n")
						   _T("���� �ڷ� : '%d'(�ڷ���°� ������ �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;

}

/// ���� ��� DB �۾�
/// 1. å���� UPDATE
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CBL_SE_LOC_15::CancelRemoveDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sShelfDate , CString sWorker )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelRemoveDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelRemoveDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CancelRemoveDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelRemoveDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�ڷ����") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelRemoveDBProc") ); 


	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL112N") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , sShelfDate );
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , sWorker );
	pDM->AddDBFieldData( _T("DISUSE_PROC_DATE") , _T("STRING") , _T(""));
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM�� ���
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("�ڷ����") , _T("�ǽð��谡�ڷ�") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CancelRemoveDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����ڷ����") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRemoveDBProc") ); 
	ids = pTmpDM->SetCellData( _T("���ʹ谡����") , sShelfDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRemoveDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡�����") , sWorker , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRemoveDBProc") ); 


	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CancelRemoveDBProc") ); 

	
	// 4. �������� SPECIES_CLASS ����
	// 4. �������� SPECIES_CLASS ����
	CString sSpeciesKey, sVolKey;
	ids = pTmpDM->GetCellData( _T("������KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = pTmpDM->GetCellData( _T("��ȣKEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = SettingSpeciesField( sSpeciesKey , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("RecordShelfDateDBProc") ); 
	
	return 0;

}

/// ��� ��ҿ� �ʿ��� �Է°��� �����´�.
INT CBL_SE_LOC_15::CancelRemoveData( CString &sShelfDate,CString &sWorker )
{
	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1580");
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�谡����") , sShelfDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sShelfDate.TrimLeft(); sShelfDate.TrimRight();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�谡�����") , sWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sWorker.TrimLeft(); sWorker.TrimRight();

	return 0;

}

/// ��� ���� ����
INT CBL_SE_LOC_15::GetInputRemoveDate( CString &sRemoveDate )
{
	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1580");
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�������") , sRemoveDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sRemoveDate.TrimLeft(); sRemoveDate.TrimRight();

	return 0;

}


/// ���� �谡����
INT CBL_SE_LOC_15::OneBookChangeShelf()
{
	INT ids;
	INT nIndex;
	GetTempData( _T("�����ε���") , nIndex );

	ids = OneBookChangeShelf( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OneBookChangeShelf") );
	if( ids == 1000 )
	{
		AfxMessageBox( _T("�ڷ� ���°� �谡 ������ �� ���� �ڷ�����Դϴ�. ")) ;
		return ids;
	}
	if( ids > 0 ) return ids;



	return 0;
}


/// DM�� ���ڵ尡 nIndex�� �ڷḦ �����Ѵ�.
INT CBL_SE_LOC_15::OneBookChangeShelf( INT nIndex )
{
	INT ids;
	// �Է°��� �ùٸ��� �ԷµǾ� �ִ��� �˻�

	CString sChangeReasonCode, sChangeReason , sShelfLocCode , sShelfWorker , sShelfChangeDate;
	
	ids = GetBookChangeShelfInputData( sChangeReasonCode, sChangeReason , sShelfLocCode , sShelfWorker , sShelfChangeDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OneBookChangeShelf") );

	if( sShelfLocCode.IsEmpty() )
	{
		AfxMessageBox( _T("����� �谡 ��ġ�� �������ֽʽÿ�") );
		return 1;
	}
	if( sShelfChangeDate.IsEmpty() )
	{
		AfxMessageBox( _T("�谡 ���� ���ڸ� �Է����ֽʽÿ�.") );
		return 2;
	}
	if( sShelfWorker.IsEmpty() )
	{
		AfxMessageBox( _T("�谡 �����ڸ� �Է����ֽʽÿ�.") );
		return 3;
	}

	CString sDMAlias = _T("DM_SE_LOC_1500");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("OneBookChangeShelf") );


	CESL_DataMgr TmpDM;
	CString sWorkingStatus;

	ids = pDM->GetCellData( _T("�ڷ����") , nIndex , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("OneBookChangeShelf") );

	if( sWorkingStatus.Compare( _T("�ǽð��谡�ڷ�") ) != 0)
		return 1000;


	ids = OneBookChangeShelfDBProc( nIndex ,&TmpDM ,sChangeReasonCode, sChangeReason , sShelfLocCode , sShelfWorker , sShelfChangeDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OneBookChangeShelf") );
	
	// DM �۾�
	ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("OneBookChangeShelf") );

	// ȭ�� �۾�
	ids = DBInsertUpdateGridProc(nIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("OneBookChangeShelf") );
	
	return 0;
}


/// å�谡����ȭ�鿡�� �Է°��� �����´�.
INT CBL_SE_LOC_15::GetBookChangeShelfInputData( CString &sChangeReasonCode, CString &sChangeReason , CString &sShelfLocCode , CString &sShelfWorker , CString &sShelfChangeDate )
{
	CString sCMAlias = _T("CM_SE_LOC_1540");

	INT ids;

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("��������ڵ�_�Է�") , sChangeReasonCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetBookChangeShelfInputData") );
	sChangeReasonCode.TrimLeft(); sChangeReasonCode.TrimRight();
	if( sChangeReasonCode.Compare(_T("�������")) == 0 ) sChangeReasonCode.Empty();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�����������_�Է�") , sChangeReason );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetBookChangeShelfInputData") );
	sChangeReason.TrimLeft(); sChangeReason.TrimRight();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("������ġ_�Է�") , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetBookChangeShelfInputData") );
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("����������_�Է�") , sShelfWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetBookChangeShelfInputData") );
	sShelfWorker.TrimLeft(); sShelfWorker.TrimRight();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("���泯¥_�Է�") , sShelfChangeDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("GetBookChangeShelfInputData") );
	sShelfChangeDate.TrimLeft(); sShelfChangeDate.TrimRight();

	return 0;
}


/// DM�� ���ڵ尡 nIndex�� �ڷḦ �谡 �����Ѵ�.
INT CBL_SE_LOC_15::OneBookChangeShelfDBProc( INT nIndex , CESL_DataMgr* pTmpDM , CString sChangeReasonCode, CString sChangeReason , CString sShelfLocCode , CString sShelfWorker , CString sShelfChangeDate )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OneBookChangeShelfDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("OneBookChangeShelfDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("OneBookChangeShelfDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sShelfBeroreLoc;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("OneBookChangeShelfDBProc") ); 

	ids = pTmpDM->GetCellData( _T("�ڷ��") , 0 , sShelfBeroreLoc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("OneBookChangeShelfDBProc") ); 

	m_pParentMgr->m_pInfo->pCodeMgr->ConvertDescToCode( _T("�ڷ�Ǳ���") , sShelfBeroreLoc , sShelfBeroreLoc );

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , sShelfWorker );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_REASON_CODE") , _T("STRING") , sChangeReasonCode  );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_DATE") , _T("STRING") , sShelfChangeDate );
	pDM->AddDBFieldData( _T("SHELF_LOC_CODE") , _T("STRING") , sShelfLocCode );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_ETC_REASON") , _T("STRING") , sChangeReason );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_BEFORE_LOC") , _T("STRING") , sShelfBeroreLoc );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM�� ���
	m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�ڷ�Ǳ���") , sShelfLocCode , sShelfLocCode );

	ids = pTmpDM->SetCellData( _T("�谡�����") , sShelfWorker , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OneBookChangeShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡��������ڵ�") , sChangeReasonCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OneBookChangeShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡��������") , sShelfChangeDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OneBookChangeShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�ڷ��") , sShelfLocCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OneBookChangeShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�谡�����������") , sChangeReason , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OneBookChangeShelfDBProc") ); 


	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("OneBookChangeShelfDBProc") ); 

	// ������ ����Ѵ�.
	CString sSpeciesKey;
	ids = pTmpDM->GetCellData( _T("������KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("OneBookChangeShelfDBProc") ); 

	CString sSpceciesFunc;
	sSpceciesFunc.Format( _T("GET_DISP_SE_BOOK_INFO(%s,'SHELF_LOC_CODE')") , sSpeciesKey);
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("idx_hold_place") , _T("NUMERIC") , sSpceciesFunc );
	pDM->MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );
	
	sSpceciesFunc.Format( _T("GET_SE_MAIN_SHELF_CODE(%s)") , sSpeciesKey);
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("main_place_info") , _T("NUMERIC") , sSpceciesFunc );
	pDM->MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );
	
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("place_info") , _T("NUMERIC") , sSpceciesFunc );
	pDM->MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	CLocCommonAPI::MakeIdxTmpTblInsertFrame( pDM , sSpeciesKey , 1 , m_pParentMgr );

	// ���� ���̺� �Բ� �����Ѵ�.
	CString sQuery;
	sQuery.Format( _T("UPDATE IDX_SE_TBL SET IDX_PLACE = GET_SE_BOOK_INFO( '%s', 'SHELF_LOC_CODE' ) WHERE REC_KEY = %s;"),
					sSpeciesKey, sSpeciesKey );
	pDM->AddFrame( sQuery );
	
	pDM->SendFrame();
	
	return 0;
}


/// �ϰ� �谡����
INT CBL_SE_LOC_15::AllBookChangeShelf()
{
	INT ids;
	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AllBookChangeShelf") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nSuccessCnt = 0;

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		ids = OneBookChangeShelf(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AllBookChangeShelf") );
		if( ids == 1000 )
		{
			pGrid->SelectGetNext();
			continue;
		}
		else if( ids > 0 ) 
		{
			DeleteProgressBarDlg( pProgressBar );
			return 1;
		}

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}

	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷḦ �谡�����Ͽ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ �谡���� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n\r\n���� �ڷ� : '%d'\r\n")
						   _T("���� �ڷ� : '%d'(�ڷ���°� �谡���� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;
}


/// ���谡����
INT CBL_SE_LOC_15::AllSpeciesChangeShelf()
{
	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// �谡�� ����
	CString sShelfLocCode;
	ids = GetSpeciesChangeShelfInputData( sShelfLocCode);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sShelfLocCode.IsEmpty() )
	{
		AfxMessageBox( _T("�谡���� �Է����ֽʽÿ�.") );
		return 2;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		// ���谡���� ��� DB�۾�
		ids = AllSpeciesChangeShelfDBProc( nIndex  , sShelfLocCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	return 0;
}

/// ���谡���� DB�۾�
INT CBL_SE_LOC_15::AllSpeciesChangeShelfDBProc( INT nIndex , CString sShelfLocCode )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OneBookChangeShelfDBProc") );

	pDM->StartFrame();

	CString sSpeciesRecKey ;
	ids = pDM->GetCellData( _T("������KEY") , nIndex , sSpeciesRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("OneBookChangeShelfDBProc") ); 

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("SHELF_LOC_CODE") , _T("STRING") , sShelfLocCode );
	pDM->MakeUpdateFrame( _T("SE_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesRecKey );
	pDM->InitDBFieldData();

	// ���谡�� ����� ���� ���̺��� �谡�ǵ� �Բ� �����Ѵ�. 
	CString sQuery;
	sQuery.Format( _T("UPDATE IDX_SE_TBL SET PLACE_INFO = '%s', MAIN_PLACE_INFO = '%s' WHERE REC_KEY = %s;"), 
					sShelfLocCode, sShelfLocCode, sSpeciesRecKey );
	pDM->AddFrame( sQuery );
	
	
	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("OneBookChangeShelfDBProc") ); 

	return 0;

}

/// å�谡����ȭ�鿡�� �Է°��� �����´�.
INT CBL_SE_LOC_15::GetSpeciesChangeShelfInputData( CString &sShelfLocCode )
{
	CString sCMAlias = _T("CM_SE_LOC_1590");

	INT ids;

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�谡��") , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetBookChangeShelfInputData") );
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	if( sShelfLocCode.Compare(_T("�������")) == 0 ) sShelfLocCode.Empty();

	return 0;
}



// �ڵ� �ϰ� ����
INT CBL_SE_LOC_15::ChangeCode()
{
	EFS_BEGIN

	INT ids;
	INT nGetRowCount = 0;
	CString sParentCMAlias = _T("CM_SE_LOC_1500");
	CString sParentDMAlias = _T("DM_SE_LOC_1500");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeCode()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("���α׸���") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::ChangeCode()") );

	INT nIndex;

	CString sCodeNameCode;	// ����� �ڵ��� ���� �ڵ� ( �̿����ѱ��� : 0 , �������� : 1 )
	CString sCodeName;			// ����� �ڵ��� �ʵ��( USE_LIMIT_CODE , MANAGE_CODE )
	CString sCodeValue;			 // ��Ʈ�ѿ��� ����
	CString sDMColumnAlias;
	ids = GetInputControlData( sCodeNameCode , sCodeName , sCodeValue , sDMColumnAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::ChangeCode()") );
	if( ids > 0 ) return 1;

	// �׸����� �������� ����������
	pGrid->SelectGetTailPosition();
	
	INT nMaxUpdateRecordCnt = 50;
	CString sRecKeyList;		 // ����� ���ڵ��� REC_KEY ����Ʈ
	
	nGetRowCount= pGrid->SelectGetCount();

	
	// ���α׷����ٸ� �����ϱ� ����--------------------
	CLocProgressBarDlg *ProgressBar;
	ProgressBar = new CLocProgressBarDlg( m_pParentMgr );
	ProgressBar->Create( m_pParentMgr );
	ProgressBar->CenterWindow( m_pParentMgr );
	ProgressBar->ShowWindow( SW_SHOW );
	INT nUpper = nGetRowCount-1;
	if( nGetRowCount == 1 ) nUpper = 1;
	ProgressBar->InitRange(nUpper);
	//-----------------------------------------------

	// ������ �ڷ�
	CString sRepresentBookYN;
	CString sWorkingStatus;

	INT nNoChangeCnt=0;
	INT nChangeCnt=0;
	for( INT i = 0 ; i < nGetRowCount ; i++ )
	{
		// ���α׷����� ����------------------
		ProgressBar->ChangeProgressBar(i);
		//-----------------------------------
		nIndex = pGrid->SelectGetIdx();

		// ���� ���� �ڷ�� �����Ҽ� ����.
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("�ڷ����") , sWorkingStatus , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
		
		//=====================================================
		//2009.02.17 UPDATE BY PJW : WORKING_STATUS���� �߸���
// 		if( sWorkingStatus.Compare(_T("SEL321O"))==0 || (sWorkingStatus.Compare(_T("SEL322O"))==0) || (sWorkingStatus.Compare(_T("SEL323O"))==0) ||
// 		(sWorkingStatus.Compare(_T("SEL324O"))==0) || (sWorkingStatus.Compare(_T("SEL325O"))==0) )
		if( (sWorkingStatus.Compare(_T("SEL311O"))==0) || (sWorkingStatus.Compare(_T("SEL312O"))==0) || (sWorkingStatus.Compare(_T("SEL313O"))==0) ||
			// 18.09.27 JMJ KOL.RED.2018.008
			(sWorkingStatus.Compare(_T("SEL314O"))==0) || (sWorkingStatus.Compare(_T("SEL315O"))==0) || (sWorkingStatus.Compare(_T("SEL411O"))==0) || (sWorkingStatus.Compare(_T("SEL511O"))==0) || (sWorkingStatus.Compare(_T("SEL611O"))==0))
		//=====================================================
		{
			pGrid->SelectGetPrev();
			nNoChangeCnt++;
			continue;
		}


		// UPDATE�� å���̺��� REC_KEY ����
		CString sBookRecKey;
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("å����KEY") , sBookRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::ChangeCode()") );

		CString sSpeciesRecKey;
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("������KEY") , sSpeciesRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::ChangeCode()") );

		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("��ǥå��") , sRepresentBookYN , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::ChangeCode()") );



		sRecKeyList = sBookRecKey;

		ids = ChangeCodeDBUpdate( sCodeNameCode , sCodeName , sCodeValue  , sRecKeyList , sSpeciesRecKey , sRepresentBookYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_SE_LOC_13::ChangeCode()") );
		sRecKeyList.Empty();
		
		// ������ �޴������� UPDATE
		// �ڵ带 ������ �޴����� �Է�
		ids = DMCodeChangeUpdate( sDMColumnAlias , sCodeValue , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_SE_LOC_13::ChangeCode()") );

		// �׸��忡 �����ִ� �۾�
		ids = CMGridCodeChangeUpdate( nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::ChangeCode()") );


		if( i !=  nGetRowCount-1 ) pGrid->SelectGetPrev();
		nChangeCnt++;
	}
	// ���α׷����� ��Ʈ����------------------
	ProgressBar->DestroyWindow();
	if( ProgressBar != NULL ) delete ProgressBar;
	//----------------------------------

	CString sMsg;
	if( (nChangeCnt > 0) && (nNoChangeCnt > 0) )
		sMsg.Format( _T("���� �ڷ� : '%d'\r\n���� �ڷ� : '%d'\r\n���� �ڷ� : '%d'(�������� �ڷ��̱� ������ ó������ ���߽��ϴ�.)"),
		nChangeCnt+nNoChangeCnt,nChangeCnt,nNoChangeCnt );
	else if( (nChangeCnt > 0) && (nNoChangeCnt < 1) )
		sMsg.Format( _T("��� '%d'���� ó���Ǿ����ϴ�.") , nChangeCnt );
	else if( (nChangeCnt < 1) && (nNoChangeCnt > 0) )
		sMsg.Format( _T("��� '%d'���� �������� �ڷ��̱� ������ ó������ ���߽��ϴ�.") , nNoChangeCnt );
	AfxMessageBox(sMsg);


	return 0;

	EFS_END
	return -1;

}

INT CBL_SE_LOC_15::GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1595");

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�ڵ庯������") , sCodeNameCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )") );

	CString sCodeComboAlias;
	if( sCodeNameCode.Compare( _T("0") ) == 0 )
	{
		sCodeName = _T("USE_LIMIT_CODE");
		sCodeComboAlias = _T("�̿����ѱ����ڵ�");
		sDMColumnAlias = _T("�̿����ѱ���");
	}
	else if( sCodeNameCode.Compare( _T("1") ) == 0 )
	{
		sCodeName = _T("MANAGE_CODE");
		sCodeComboAlias = _T("���������ڵ�");
		sDMColumnAlias = _T("��������");
	}
	else
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )") );
	}

	ids = m_pParentMgr->GetControlData( sCMAlias , sCodeComboAlias , sCodeValue , -1 , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )") );

	// �谡 ���� �ڵ�� �ٸ��� �Ѵ�.
	if( sCodeNameCode.Compare( _T("0") ) == 0 )
	{
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( sCodeValue , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )") );
	}

	if( sCodeValue.IsEmpty() || ( sCodeValue.Compare( _T("0") )== 0 ) )
	{
		AfxMessageBox( _T("����� ���а��� �������ֽʽÿ�") );
		return 1;
	}

	return 0;

	EFS_END
	return -1;

}

// ���� �ڵ� �ϰ� ����
INT CBL_SE_LOC_15::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList , CString sSpeciesKey , CString sRepresentBookYN)
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );

	// LOG DB �÷� ����
	CString sLogColumnName;
	// �谡 ����
	// �̿����� ���� 
	if( sCodeNameCode.Compare(_T("0")) == 0 ) 
		sLogColumnName = _T("LAST_USELIMITCODE_CHANGE_DATE");
	// �������� 
	else if( sCodeNameCode.Compare(_T("1")) == 0 )
		sLogColumnName = _T("LAST_MANAGE_CODE_CHANGE_DATE");


	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi.StartLog();
	MobileApi.SetDM( pDM );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( sCodeName , _T("STRING") , sCodeValue );
	pDM->AddDBFieldData( sLogColumnName , _T("NUMERIC") , _T("sysdate") );

	MobileApi.MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKeyList );

	ids = MobileApi.SendFrame();
	if( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );
	pDM->EndFrame();

	MobileApi.EndLog();

	CString sSpeciesCodeValue;
	if( sCodeNameCode.Compare(_T("1")) == 0 ) 
	{
		return 0;
	}

	
	// �̿����� �����̸� ������ �����Ѵ�.
	if( sCodeNameCode.Compare(_T("0")) == 0 )
	{
		ids = ChangeUseLimitCode( sSpeciesKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ChangeCodeDBUpdate") );

		return 0;
	}

	CKolas2up_MobileApi MobileApi2;
	MobileApi2.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi2.StartLog();
	MobileApi2.SetDM( pDM );
	
	pDM->StartFrame();

	CString sSpeciesFunc;
	sSpeciesFunc.Format( _T("GET_DISP_SE_BOOK_INFO(%s,'MANAGE_CODE')") , sSpeciesKey );
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("NUMERIC") , sSpeciesFunc );
	MobileApi2.MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	CLocCommonAPI::MakeIdxTmpTblInsertFrame( pDM , sSpeciesKey , 1 , m_pParentMgr , &MobileApi2 );
	
	// QUERY SEND
	ids = MobileApi2.SendFrame();
	if( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );

	pDM->EndFrame();

	MobileApi.EndLog();


	return 0;	

	EFS_END
	return -1;

}




/// ���� �ڵ� �ϰ� ������
/// ���� ���� -> ���� ����
INT CBL_SE_LOC_15::ChangeSpeciesCode()
{
	EFS_BEGIN


	INT ids;
	INT nGetRowCount = 0;
	CString sParentCMAlias = _T("CM_SE_LOC_1500");
	CString sParentDMAlias = _T("DM_SE_LOC_1500");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("���α׸���") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

	INT nIndex;

	CString sCodeNameCode;		// ����� �ڵ��� ���� �ڵ� ( �谡���� : 0 , �̿����ѱ��� : 1 , �������� : 2 )
	CString sCodeName;			// ����� �ڵ��� �ʵ��( WORKING_STATUS , USE_LIMIT_CODE , MANAGE_CODE )
	CString sCodeValue;			// ��Ʈ�ѿ��� ����
	CString sDMColumnAlias;
	ids = GetInputControlData( sCodeNameCode , sCodeName , sCodeValue , sDMColumnAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );
	if( ids > 0 ) return 1;

	// �׸����� �������� ����������
	pGrid->SelectGetTailPosition();
	
	INT nMaxUpdateRecordCnt = 50;
	CString sRecKeyList;		 // ����� ���ڵ��� REC_KEY ����Ʈ
	
	nGetRowCount= pGrid->SelectGetCount();

	
	// ���α׷����ٸ� �����ϱ� ����--------------------
	CLocProgressBarDlg *ProgressBar;
	ProgressBar = new CLocProgressBarDlg( m_pParentMgr );
	ProgressBar->Create( m_pParentMgr );
	ProgressBar->CenterWindow( m_pParentMgr );
	ProgressBar->ShowWindow( SW_SHOW );
	INT nUpper = nGetRowCount-1;
	if( nGetRowCount == 1 ) nUpper = 1;
	ProgressBar->InitRange(nUpper);
	//-----------------------------------------------

	// ������ �ڷ�
	CString sRepresentBookYN;
	CString sWorkingStatus;

	INT nNoChangeCnt=0;
	INT nChangeCnt=0;
	for( INT i = 0 ; i < nGetRowCount ; i++ )
	{
		// ���α׷����� ����------------------
		ProgressBar->ChangeProgressBar(i);
		//-----------------------------------
		nIndex = pGrid->SelectGetIdx();


		// UPDATE�� å���̺��� REC_KEY ����
		CString sSpeciesRecKey;
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("������KEY") , sSpeciesRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

		pParentDM->StartFrame();

		pParentDM->InitDBFieldData();
		pParentDM->AddDBFieldData(_T("USE_LIMIT_CODE"),_T("STRING"),sCodeValue);
		pParentDM->MakeUpdateFrame(_T("SE_SPECIES_TBL"),_T("REC_KEY"),_T("NUMERIC"),sSpeciesRecKey);

		ids = pParentDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("") )

		pParentDM->EndFrame();

		if( i !=  nGetRowCount-1 ) pGrid->SelectGetPrev();
		nChangeCnt++;
	}
	// ���α׷����� ��Ʈ����------------------
	ProgressBar->DestroyWindow();
	if( ProgressBar != NULL ) delete ProgressBar;
	//----------------------------------

	CString sMsg;
	if( (nChangeCnt > 0) && (nNoChangeCnt > 0) )
		sMsg.Format( _T("���� �ڷ� : '%d'\r\n\r\n���� �ڷ� : '%d'\r\n���� �ڷ� : '%d'(�������� �ڷ��̱� ������ ó������ ���߽��ϴ�.)"),
		nChangeCnt+nNoChangeCnt,nChangeCnt,nNoChangeCnt );
	else if( (nChangeCnt > 0) && (nNoChangeCnt < 1) )
		sMsg.Format( _T("��� '%d'���� ó���Ǿ����ϴ�.") , nChangeCnt );
	else if( (nChangeCnt < 1) && (nNoChangeCnt > 0) )
		sMsg.Format( _T("��� '%d'���� �������� �ڷ��̱� ������ ó������ ���߽��ϴ�.") , nNoChangeCnt );
	AfxMessageBox(sMsg);


	return 0;

	EFS_END

	return -1;
}


// �ڵ� �ϰ� ������
// ����Ǿ��� ��� ȭ�鿡 �����ִ� �۾�
INT CBL_SE_LOC_15::CMGridCodeChangeUpdate(INT nIndex)
{
	EFS_BEGIN
		
		INT ids;
	
	CString sDMAlias = _T("DM_SE_LOC_1500");
	
	CString sCMAlias = _T("CM_SE_LOC_1500");
	CString sGridAlias = _T("���α׸���");
	
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	
	const INT nCnt = 2;
	
	TCHAR *sDMFieldAlias[nCnt] =
	{
		_T("��������") , _T("�̿����ѱ���")
	};
	
	INT nColIndex[nCnt] =
	{
		-1,-1
	};
	
	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sDMFieldAlias , nCnt , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") ); 
	
	CString sData;
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetDataMgrData( sDMAlias , sDMFieldAlias[i] , sData , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") ); 
		
		if( nColIndex[i] == -1 ) continue;
		
		ids = pGrid->SetAt( nIndex , nColIndex[i] , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	}
	
	return 0;
	
	EFS_END
	return -1;
	
}


// �ڵ� �ϰ� ����� DM UPDATE
INT CBL_SE_LOC_15::DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )
{
	EFS_BEGIN
		
		INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1500");
	
	if( sDMColumnAlias.Compare(_T("��������")) == 0 )
	{
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(  _T("��������") , sCodeValue , sCodeValue   );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
		
	}
	else if( sDMColumnAlias.Compare(_T("�ڷ����")) == 0 )
	{
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( sCodeValue , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
	}
	else if( sDMColumnAlias.Compare(_T("�̿����ѱ���")) == 0 )
	{
		// �̿����� ����
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(  _T("�̿����ѱ���") , sCodeValue , sCodeValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
	}
	
	ids = m_pParentMgr->SetDataMgrData( sDMAlias , sDMColumnAlias , sCodeValue , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
	
	return 0;
	
	EFS_END
		return -1;
	
}



// �� �̿����� ���� UPDATE
INT CBL_SE_LOC_15::ChangeUseLimitCode( CString sSpeciesKey )
{
	EFS_BEGIN

	INT ids;
	CString sPreUseLimitCode , sNewUseLimitCode;

	CESL_DataMgr *pUseLimitDM = m_pParentMgr->FindDM(_T("DM_SE_LOC_1500_USE_LIMIT_SPECIES"));
	if( pUseLimitDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeUseLimitCode") );

	// ����_�̿����ѱ��� ��������
	CString sQuery;
	sQuery.Format( _T("SELECT USE_LIMIT_CODE FROM IDX_SE_TBL WHERE REC_KEY = %s") , sSpeciesKey );
	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr , pUseLimitDM , sQuery , sPreUseLimitCode );
//	ids = pUseLimitDM->GetOneValueQuery( sQuery , sPreUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeUseLimitCode") );

	// ���ο�_�̿����ѱ��� ��������
	sQuery.Format( _T(" SPECIES_KEY = %s ") , sSpeciesKey );
	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pUseLimitDM , sQuery );
//	ids = pUseLimitDM->RefreshDataManager( sQuery );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ChangeUseLimitCode") );


	CString sUseLimitCode;
	for( INT i = 0 ; i < pUseLimitDM->GetRowCount() ; i++ )
	{
		ids = pUseLimitDM->GetCellData( _T("�̿����ѱ���") , i , sUseLimitCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ChangeUseLimitCode") );

		if( sNewUseLimitCode.GetLength() > 1 )
			sNewUseLimitCode += _T(" ");

		sNewUseLimitCode += sUseLimitCode;
	}

	CArray < CString , CString > saPreUseLimitCode;
	CArray < CString , CString > saNewUseLimitCode;

	ids = MakeArrayUseLimitCode( sPreUseLimitCode , saPreUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd , -5 , _T("ChangeUseLimitCode") );

	ids = MakeArrayUseLimitCode( sNewUseLimitCode , saNewUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd , -6 , _T("ChangeUseLimitCode") );

	BOOL IsEqual = TRUE;

	// ������ Ʋ���Ĵ� ������� ������ �Ұ����ϴ�.
	IsEqual = CompareUseLimtCodes( saPreUseLimitCode , saNewUseLimitCode );

	if( IsEqual ) return 0;

	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi.StartLog();
	MobileApi.SetDM( pUseLimitDM );

	// �� ���̺� ���ο� �̿����ѱ����� �Է��Ѵ�.
	pUseLimitDM->StartFrame();
	pUseLimitDM->InitDBFieldData();
	pUseLimitDM->AddDBFieldData( _T("USE_LIMIT_CODE") , _T("STRING") , sNewUseLimitCode );
	// ���� ���� ���̺� - ��� �̿����� ���� ����Ʈ�� ����.
	MobileApi.MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );	
//	pUseLimitDM->MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	// ���� �� ���̺� - ���ο� ��ǥ�� ����.
	CString sBossPreUseLimitCode , sBossNewUseLimitCode;
	ids = GetBossUseLimitCode( sBossPreUseLimitCode , saPreUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("ChangeUseLimitCode") );
	ids = GetBossUseLimitCode( sBossNewUseLimitCode , saNewUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("ChangeUseLimitCode") );

	pUseLimitDM->InitDBFieldData();
	pUseLimitDM->AddDBFieldData( _T("USE_LIMIT_CODE") , _T("STRING") , sBossNewUseLimitCode );
//	pUseLimitDM->MakeUpdateFrame( _T("SE_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );
	MobileApi.MakeUpdateFrame( _T("SE_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	CLocCommonAPI::MakeIdxTmpTblInsertFrame( pUseLimitDM , sSpeciesKey , 1 , m_pParentMgr , &MobileApi );
	

//	ids = pUseLimitDM->SendFrame();
	ids = MobileApi.SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("ChangeUseLimitCode") );
	ids = pUseLimitDM->EndFrame();

	MobileApi.EndLog();

	// �̷������� ���� ���ΰ��� �˾ƺ���.
	if( sBossPreUseLimitCode.Compare( sBossNewUseLimitCode ) == 0 ) return 0;

	// �̷� ������ �����.
	CString sRecKey;

	CKolas2up_MobileApi MobileApi2;
	MobileApi2.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi2.SetDM( pUseLimitDM );
	MobileApi2.StartLog();

	pUseLimitDM->StartFrame();
	pUseLimitDM->InitDBFieldData();
	pUseLimitDM->AddDBFieldData( _T("SPECIES_KEY") , _T("NUMERIC") , sSpeciesKey );
	pUseLimitDM->AddDBFieldData( _T("CREATE_DATE") , _T("NUMERIC") , _T("SYSDATE") );
	pUseLimitDM->AddDBFieldData( _T("WORKER") , _T("STRING") , m_pParentMgr->GetUserID() );
	pUseLimitDM->AddDBFieldData( _T("UPDATE_YN") , _T("STRING") , _T("N") );
	pUseLimitDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") );

//	pUseLimitDM->MakeRecKey( sRecKey );
	MobileApi2.MakeRecKey( sRecKey );
//	pUseLimitDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );

//	pUseLimitDM->MakeInsertFrame( _T("BO_USE_LIMIT_UPDATE_TBL") );
	MobileApi2.AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ) );
	MobileApi2.MakeInsertFrame( _T("SE_USE_LIMIT_UPDATE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	
//	ids = pUseLimitDM->SendFrame();
	ids = MobileApi2.SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("ChangeUseLimitCode") );

	pUseLimitDM->EndFrame();

	// ��~~~


	return 0;

	EFS_END
	return -1;

}



// �켱 ������ �������� �˾Ƴ��ش�.
INT CBL_SE_LOC_15::GetBossUseLimitCode( CString &sUseLimitCode , CArray <CString,CString> &saUseLimitCode )
{
	EFS_BEGIN
		
		const INT nCodeCnt = 5;
	CString sOrder[nCodeCnt][2] = 
	{
		{ _T("CL") , _T("0") } , 
		{ _T("CD") , _T("1") } , 
		{ _T("CA") , _T("2") } , 
		{ _T("IZ") , _T("3") } , 
		{ _T("GM") , _T("4") }
	};
	
	INT nIndex;
	CString sTmpCode;
	INT nUseLimitCode=-1;
	for( INT i = 0 ; i < saUseLimitCode.GetSize() ; i++ )
	{
		sTmpCode = saUseLimitCode.GetAt(i);
		
		nIndex = -1;
		for( INT j = 0 ; j < nCodeCnt ; j++ )
		{
			if( sTmpCode.Compare( sOrder[j][0] ) == 0 )
			{
				nIndex = j;
			}
		}
		
		if( nIndex == -1 ) continue;
		
		if( i == 0 )
		{
			nUseLimitCode = _ttoi(sOrder[nIndex][1]);
		}
		else
		{
			if( _ttoi( sOrder[nIndex][1] ) > nUseLimitCode  ) 
				nUseLimitCode = _ttoi( sOrder[nIndex][1] );
		}
	}
	
	if( nUseLimitCode != -1 )
	{
		sUseLimitCode = sOrder[nUseLimitCode][0];
	}
	
	return 0;
	
	EFS_END
	return -1;
	
}



bool CBL_SE_LOC_15::CompareUseLimtCodes( CArray <CString,CString> &saPreUseLimitCode , CArray <CString,CString> &saNewUseLimitCode )
{
	EFS_BEGIN
		
		CString sPreUseLimitCode;
	CString sNewUseLimitCode;
	
	bool IsEqual;
	for( INT i = 0 ; i < saPreUseLimitCode.GetSize() ; i++ )
	{
		IsEqual = FALSE;
		
		sPreUseLimitCode = saPreUseLimitCode.GetAt(i);
		
		for( INT j = 0 ; j < saNewUseLimitCode.GetSize() ; j++ )
		{
			sNewUseLimitCode = saNewUseLimitCode.GetAt(j);
			
			if( sPreUseLimitCode.Compare(sNewUseLimitCode)==0 )
				IsEqual = TRUE;
		}
		if( !IsEqual ) return FALSE;
	}
	
	for( i = 0 ; i < saNewUseLimitCode.GetSize() ; i++ )
	{
		IsEqual = FALSE;
		
		sNewUseLimitCode = saNewUseLimitCode.GetAt(i);
		
		for( INT j = 0 ; j < saPreUseLimitCode.GetSize() ; j++ )
		{
			sPreUseLimitCode = saPreUseLimitCode.GetAt(j);
			
			if( sNewUseLimitCode.Compare(sPreUseLimitCode)==0 )
				IsEqual = TRUE;
		}
		if( !IsEqual ) return FALSE;
	}
	
	return TRUE;
	
	EFS_END
		return FALSE;
	
}




INT CBL_SE_LOC_15::MakeArrayUseLimitCode( CString sUseLimitCode , CArray <CString,CString> &saUseLimitCode )
{
	EFS_BEGIN
		
		sUseLimitCode.TrimLeft();
	sUseLimitCode.TrimRight();
	
	if( sUseLimitCode.IsEmpty() ) return 0;
	
	INT nIndex = -1;
	
	bool IsEnd = FALSE;
	CString sCode;
	while(TRUE)
	{
		nIndex = sUseLimitCode.Find( _T(" ") );
		
		if( nIndex < 0 )
		{
			saUseLimitCode.Add( sUseLimitCode );
			IsEnd = TRUE;
		}
		else
		{
			sCode = sUseLimitCode.Mid( 0 , nIndex );
			sUseLimitCode = sUseLimitCode.Mid( nIndex+1 , sUseLimitCode.GetLength()-nIndex );
			
			saUseLimitCode.Add( sCode );
		}
		
		if( IsEnd ) break;
	}
	
	return 0;
	
	EFS_END
		return -1;
	
}


/// �ڷᰡ �۾������� ���������� �˾Ƴ��� �����̺� �����Ѵ�.
/// ��ȣ�� �ִ�. �ε��� ���̺� �ִ�.
INT CBL_SE_LOC_15::SettingSpeciesField( CString sSpeciesKey , CString sVolKey )
{
	EFS_BEGIN

	INT ids;

	CString sSql;
	sSql.Format( _T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s AND WORKING_STATUS IN (%s) ") , 
				sSpeciesKey ,
				_T(HOLD_SE_SEARCH_CONDITION)_T(",'SEL112N','SEL2160'")
				);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection());
	TmpDM.ParseCONNECTION_INFO();
	
	CString sValue;
	TmpDM.GetOneValueQuery( sSql , sValue );

	TmpDM.StartFrame();
	TmpDM.InitDBFieldData();
	if( _ttoi(sValue) > 0 )
	{
		TmpDM.AddDBFieldData( _T("SPECIES_CLASS") , _T("STRING") , _T("1") );
	}
	else
	{
		TmpDM.AddDBFieldData( _T("SPECIES_CLASS") , _T("STRING") , _T("0") );
	}

	TmpDM.MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	TmpDM.InitDBFieldData();
	if( _ttoi(sValue) > 0 )
	{
		TmpDM.AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("0") );
	}
	else
	{
		TmpDM.AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("1") );
	}
	
	TmpDM.MakeUpdateFrame( _T("SE_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );


	// ��ȣ ���� ����
	// ��ȣKEY�� ���� å���� �������̶�� �������� �����Ѵ�.
	sSql.Format( _T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND WORKING_STATUS IN (%s) ") , 
		sVolKey ,
		_T(HOLD_SE_SEARCH_CONDITION)_T(",'SEL112N','SEL2160'")
		);
		
	TmpDM.GetOneValueQuery( sSql , sValue );

	TmpDM.InitDBFieldData();
	if( _ttoi(sValue) > 0 )
	{
		TmpDM.AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("0") );
	}
	else
	{
		TmpDM.AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("1") );
	}
	
	TmpDM.MakeUpdateFrame( _T("SE_CONTENTS_TBL") , _T("VOL_KEY") , _T("NUMERIC") , sVolKey );

	if( _ttoi(sValue) > 0 )
	{
		TmpDM.AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("0") );
	}
	else
	{
		TmpDM.AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("1") );
	}
	TmpDM.MakeUpdateFrame( _T("IDX_SE_TOC_TBL") , _T("VOL_KEY") , _T("NUMERIC") , sVolKey );

	CESL_DataMgr TmpDM2;
	TmpDM2.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	CString sGetArticleKey;
	sGetArticleKey.Format( _T("SELECT REC_KEY FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s ") , sVolKey );
	ids = TmpDM2.RestructDataManager(  sGetArticleKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,  -31 , _T("SettingSpeciesField") );
  
	if( TmpDM2.GetRowCount() > 0 )
	{
		CString sArticleKey;
		for( INT i = 0 ; i < TmpDM2.GetRowCount() ; i++ )
		{
			ids = TmpDM2.GetCellData( i , 0 , sArticleKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,  -32 , _T("SettingSpeciesField") );

			TmpDM.InitDBFieldData();
			TmpDM.AddDBFieldData( _T("ARTICLE_KEY") , _T("NUMERIC") , sArticleKey );
			TmpDM.AddDBFieldData( _T("IDX_WORK_FLAG") , _T("STRING") , _T("I") );
			TmpDM.AddDBFieldData( _T("INPUT_DATE") , _T("NUMERIC") , _T("SYSDATE") );	
			TmpDM.AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , _T("ESL_SEQ.NEXTVAL") );
			TmpDM.MakeInsertFrame( _T("IDX_TMP_SE_TOC_TBL") );
		}
		
		TmpDM.InitDBFieldData();
	}

	/// ��� ���� Ȩ������ ����ȭ


	CLocCommonAPI::MakeIdxWokingStatusFrame( &TmpDM , sSpeciesKey , 1 , m_pParentMgr );
//	CLocCommonAPI::MakeIdxTmpTblInsertFrame( &TmpDM , sSpeciesKey , 1 );
	
	ids = TmpDM.SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,  -1 , _T("SettingSpeciesField") );
	
	TmpDM.EndFrame();

	return 0;

	EFS_END

	return -1;

}