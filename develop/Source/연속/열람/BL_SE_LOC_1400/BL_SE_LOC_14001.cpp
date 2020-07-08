// BL_SE_LOC_14001.cpp: implementation of the CBL_SE_LOC_1400 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BL_SE_LOC_14001.h"

#include "..\..\..\..\����\K2UP_Common\MakeSearchData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBL_SE_LOC_1400::CBL_SE_LOC_1400(CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	m_sMainDMAlias = _T("DM_SE_LOC_1400");
}

CBL_SE_LOC_1400::~CBL_SE_LOC_1400()
{

}

INT CBL_SE_LOC_1400::SPFExcutePath( CString sPathAlias )
{

	EFS_BEGIN

	INT ids;

	if( sPathAlias.Compare( SEARCH ) == 0 )
	{
		ids = SearchSE_LOC_1410();
	}
	if( sPathAlias.Compare( RFID_SEARCH ) == 0 )
	{
		ids = SearchSE_LOC_1410FromRFID();
	}
	if( sPathAlias.Compare( REG_REPAIR ) == 0 )
	{
		ids = RegRepair();
	}
	if( sPathAlias.Compare( CANCEL_REPAIR ) == 0 )
	{
		ids = CancelRepair();
	}
	if( sPathAlias.Compare( REQUEST_REPAIR ) == 0 )
	{
		ids = RequestRepair();
	}
	if( sPathAlias.Compare( COMPLETE_REPAIR ) == 0 )
	{
		ids = CompleteRepair();
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


// �˻�
INT CBL_SE_LOC_1400::SearchSE_LOC_1410()
{
	EFS_BEGIN

	INT ids;

	// Where�� ����
	CString sWhere;
	ids = GetWhereSE_LOC_1410(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchSE_LOC_1410") );
	if( ids > 0 ) return 1;

	DWORD START, FINISH;
	START = GetTickCount();
	

	// Refresh DataManager
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( m_sMainDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchSE_LOC_1410") );

	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchSE_LOC_1410") );

	FINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - �������� �ڷ� ã�� �˻��� �ɸ� �ð� :  [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
	TRACE(strMsg.GetBuffer(0));
	pDM->QueryLog(strMsg.GetBuffer(0));

	// ȭ�鿡 �����ֱ�
	ids = ViewMainGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchSE_LOC_1410") );

	return 0;

	EFS_END
	return -1;

}

// RFID�˻�
INT CBL_SE_LOC_1400::SearchSE_LOC_1410FromRFID()
{
	EFS_BEGIN
		
		INT ids;
	
	// Where�� ����
	CString sWhere;
	GetTempData(_T("WHERE") , sWhere);
	
	
	// Refresh DataManager
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( m_sMainDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchSE_LOC_1410") );
	
	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchSE_LOC_1410") );
	
	// ȭ�鿡 �����ֱ�
	ids = ViewMainGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchSE_LOC_1410") );
	
	return 0;
	
	EFS_END
		return -1;
	
}

// Where�� ����
INT CBL_SE_LOC_1400::GetWhereSE_LOC_1410( CString &sWhere )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1410");	// �˻� CM Alias

	// 1. �⺻ Where�� ����
	const INT nCnt = 18;
	CString sControlAlias[nCnt] =
	{
		_T("�ڷ�Ǹ�") , _T("�����⵵") , _T("������ȣ1") , _T("������ȣ2") , _T("�����������1") , 
		_T("�����������2") , _T("�����Ƿ�����1") , _T("�����Ƿ�����2") , _T("�����Ϸ�����1") , _T("�����Ϸ�����2") , 
		_T("��Ϲ�ȣ����") , _T("��Ϲ�ȣ1") , _T("��Ϲ�ȣ2") , _T("����") , _T("������") , 
		_T("������1") , _T("������2") , _T("���±���")
	};

	CString sControlData[nCnt];	// �Է°�

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetWhereSE_LOC_1410") );

	// Control���� �Է°� ��������
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sControlData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetWhereSE_LOC_1410") );

		// ��Ʈ���� �𽺿��̺��̶�� �˻� ���ǿ� ���� �ʴ´�.
		CESL_Control_Element *pCE = ((CESL_Control_Element*)pCM->FindControlElement( sControlAlias[i] ));
		if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetWhereSE_LOC_1410") );
		
		if( !m_pParentMgr->GetDlgItem( pCE->CTRL_ID )->IsWindowEnabled() )
			sControlData[i].Empty();

		sControlData[i].TrimLeft(); 
		sControlData[i].TrimRight();
	}

	// �ʼ� �Է� �׸� �˻�
	// 2003�� 6�� 16�� ������ ���� �ڷ���� �ʼ� �Է��׸����� �������� ����
	/*
	if( sControlData[0].IsEmpty() )
	{
		AfxMessageBox( _T("�ڷ���� �������ֽʽÿ�.") );
		return 1;
	}
	*/

	TCHAR *sEcoDBFieldName[nCnt-1] = 
	{
		_T("B.SHELF_LOC_CODE") , _T("R.SEQ_YEAR") , _T("R.SEQ_NO") , _T("R.SEQ_NO") , _T("R.REG_DATE") ,
		_T("R.REG_DATE") , _T("R.REQUEST_DATE") , _T("R.REQUEST_DATE") , _T("R.COMPLETE_DATE") , _T("R.COMPLETE_DATE") , 
		_T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO") , _T("I.SE_IDX_TITLE") , _T("I.SE_IDX_PUBLISHER") , 
		_T("B.PUBLISH_DATE") , _T("B.PUBLISH_DATE")
	};

	TCHAR *sCatDBFieldName[nCnt-1] = 
	{
		_T("B.SHELF_LOC_CODE") , _T("R.SEQ_YEAR") , _T("R.SEQ_NO") , _T("R.SEQ_NO") , _T("R.REG_DATE") ,
		_T("R.REG_DATE") , _T("R.REQUEST_DATE") , _T("R.REQUEST_DATE") , _T("R.COMPLETE_DATE") , _T("R.COMPLETE_DATE") , 
		_T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO") , _T("I.IDX_TITLE") , _T("I.IDX_PUBLISHER") , 
		_T("B.PUBLISH_DATE") , _T("B.PUBLISH_DATE")
	};

	
	INT nDBFieldType[nCnt-1] = 
	{
		ONESTRING , ONESTRING , TWONUMERIC , ETC , TWODATE , 
		ETC , TWODATE , ETC , TWODATE , ETC , 
		THREESTRING , TWOSTRING , TWOSTRING , ONEINDEXSEARCH , ONEINDEXSEARCH ,
		TWODATE , ETC
	};

	TCHAR** sDBFieldName;
	/// ECO Search���
	if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		sDBFieldName = sEcoDBFieldName;
	else
		sDBFieldName = sCatDBFieldName;
	

	// Where�� ����
	ids = CLocCommonAPI::MakeSql( sControlData , sDBFieldName , nDBFieldType , nCnt-1 , sWhere , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(  m_pParentMgr->m_hWnd ,-2 , _T("GetWhereSE_LOC_1410") );


	
	/// �׽�Ʈ�� ���� �ӽ� ����
	FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("rb"));
		
	BOOL bNewEcoSearch = FALSE;
	if( fd == NULL )
		bNewEcoSearch = TRUE;
	else
		fclose(fd);
	
	
	// Where�� ����
	ids = CLocCommonAPI::MakeSql( sControlData , sDBFieldName , nDBFieldType , nCnt-1 , sWhere , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(  m_pParentMgr->m_hWnd ,-2 , _T("GetWhereBO_LOC_1410") );
	
	
	
	CString sEcoSearchWhere;
	INT nEcoSearchWhere = 0 ;
	if( bNewEcoSearch && 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
	{
		CMakeSearchData MakeSearchData(m_pParentMgr);	
		
		BOOL bIsEqualSearch ;
		/// ���� �˻� , ������ �˻�
		// ���ô� ������ �˻� ����
		if( !sControlData[13].IsEmpty() )
		{
			if( sControlData[13].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sControlData[13]);
			if( !bIsEqualSearch ) 
				sControlData[13] += _T("*");
			
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_TITLE, '") + sControlData[13] + _T("') ");
			nEcoSearchWhere++;
		}
		
		if( !sControlData[14].IsEmpty() )
		{
			if( sControlData[14].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sControlData[14]);
			if( !bIsEqualSearch ) 
				sControlData[14] += _T("*");
			
			if( !sEcoSearchWhere.IsEmpty() )
				sEcoSearchWhere += _T(" INTERSECT ");
			
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_PUBLISHER, '") + sControlData[14] + _T("') ");
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
	

	// + �ڷ��� ���� 
	// �������� �ƴϰ� �������� �ƴϰ� �������� �ƴ� ��ġ �ڷ�
	CString sWhere2;
	if( sControlData[17].Compare(_T("������ϴ���ڷ�")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL214O' ) ");
	}
	else if( sControlData[17].Compare(_T("����ó�����ڷ�")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS IN ( 'SEL213O' ) ");
	}
	else if( sControlData[17].Compare(_T("��������ڷ�")) == 0 )
	{
		sWhere2 = _T(" R.STATUS = '1' ");
	}
	else if( sControlData[17].Compare(_T("�����Ƿ��ڷ�")) == 0 )
	{
		sWhere2 = _T(" R.STATUS = '2' ");
	}
	else if( sControlData[17].Compare(_T("�����Ϸ��ڷ�")) == 0 )
	{
		sWhere2 = _T(" R.STATUS = '3' ");
	}

	if( !sWhere2.IsEmpty() )
	{
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		sWhere += sWhere2;
	}

	return 0;

	EFS_END
	return -1;

}


// ȭ�鿡 �����ֱ�
// 1. �ڵ� -> �������� Convert
// 2. �׸��� View
INT CBL_SE_LOC_1400::ViewMainGrid()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1400");
	CString sDMAlias = _T("DM_SE_LOC_1400");

	// 1. �ڵ� -> ����
	// û����ȣ , ���±���
	ids = CLocCommonAPI::MakeShelfCode( m_pParentMgr , sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-1 , _T("ViewMainGrid") );

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewMainGrid") );

	CString sWorkingStatus;	// �ڷ����
	CString sRepairStatus;	// ��������
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("�ڷ����") , i , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewMainGrid") );
		
		ids = pDM->GetCellData( _T("��������") , i , sRepairStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewMainGrid") );

		if( sRepairStatus.Compare( _T("1") ) == 0 ) 
			sRepairStatus = _T("���");
		else if( sRepairStatus.Compare( _T("2") ) == 0 )
			sRepairStatus = _T("�Ƿ�");
		else if( sRepairStatus.Compare( _T("3") ) == 0 )
			sRepairStatus = _T("�Ϸ�");
		else
		{
			ids = CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("ViewMainGrid") );

			sRepairStatus = sWorkingStatus;
		}
	
		ids = pDM->SetCellData( _T("��������") , sRepairStatus  , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewMainGrid") );

		ids = pDM->SetCellData( _T("�ڷ����") , sWorkingStatus, i  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewMainGrid") );
		
	}


	// 2. �׸��� View
	ids = m_pParentMgr->AllControlDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-7 , _T("ViewMainGrid") );

	// 3. �ڷ�� View
	CString sShelfLocCode;
	ids = m_pParentMgr->GetControlData( _T("CM_SE_LOC_1410") , _T("�ڷ�Ǹ�") , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-8 , _T("ViewMainGrid") );
	
	ids = m_pParentMgr->SetControlData( _T("CM_SE_LOC_1400") , _T("�ڷ��") , sShelfLocCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-9 , _T("ViewMainGrid") );

	return 0;

	EFS_END
	return -1;

}

// ���� ���
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. ��ϴ�� �ڷ����� Ȯ�� 
/// 2. å���� UPDATE , �������� ���� INSERT
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�
INT CBL_SE_LOC_1400::RegRepair()
{
	EFS_BEGIN

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

	// ����������� �Է°� �˻�
	CString sRegDate;
	ids = GetInputRegDate( sRegDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sRegDate.IsEmpty() )
	{
		AfxMessageBox( _T("����������� �Է����ֽʽÿ�.") );
		return 2;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nSuccessCnt = 0;	// ����������Ͽ� ������ �ڷ� �� 
	CString sRepairStatus; // ��������

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

		ids = pDM->GetCellData( _T("��������") , nIndex , sRepairStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sRepairStatus.Compare( _T("���Ӻ�ġ�ڷ�") ) != 0  && 
			( sRepairStatus.Compare(_T("�Ϸ�"))!=0 ) && 
			( sRepairStatus.Compare(_T("���"))!=0 ) && 
			( sRepairStatus.Compare(_T("�ļ��ڷ�"))!=0 ))
		{
			pGrid->SelectGetNext();
			continue;
		}

		// ���� ��� DB �۾�
		ids = RegRepairDBProc( nIndex , &TmpDM , sRegDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// ���� ��� DM �۾�
		ids = RegRepairDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// ���� ��� ȭ�� �۾�
		ids = RegRepairGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷḦ ���� ����Ͽ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ��������Ҽ� ���� �����̱� ������ ��Ͽ� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷ��� '%d'���� �ڷᰡ ���� ����Ͽ����ϴ�.\r\n")
						   _T("'%d'���� �ڷ�� �ڷ���°� ����Ҽ� ���� �����̱� ������ ��Ͽ� �����Ͽ����ϴ�.") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;

	EFS_END
	return -1;

}

/// ������� DB �۾�
/// 1. å���� UPDATE
/// 2. ������������INSERT
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CBL_SE_LOC_1400::RegRepairDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sRegDate )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepairDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�ڷ����") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepairDBProc") ); 

	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL213O") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();
	
	// 1.1. TmpDM�� ���
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("�ڷ����") , _T("���������ڷ�") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����ڷ����") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RegRepairDBProc") ); 

	// ������ ����
	// 2. ������������ INSERT
	CString sRepairRecKey;
	ids = pDM->MakeRecKey( sRepairRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("RegRepairDBProc") ); 

	pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRepairRecKey );
	pDM->AddDBFieldData( _T("BOOK_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("1") );
	pDM->AddDBFieldData( _T("PUBLISH_FORM_CODE") , _T("STRING") , _T("SE") );
	pDM->AddDBFieldData( _T("REG_DATE") , _T("STRING") , sRegDate );
	pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ) );
	pDM->MakeInsertFrame( _T("CO_REPAIR_BINDING_TBL") );
	pDM->InitDBFieldData();

	// 2.1 TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("��������") , _T("���"), 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("������������KEY"), sRepairRecKey , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����"), sRegDate , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�������±���"), _T("����") , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����⵵") , _T("") , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("������ȣ") , _T("") , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�Ƿ���") , _T("") , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�Ϸ���") , _T("") , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 

	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RegRepairDBProc") ); 

	return 0;

	EFS_END
	return -1;

}

/// ���� ��� DM�۾�
/// �ڷ� ���濡 ������ DM�� ������ ���� ���Ǵ� DM�� ����Ѵ�.
INT CBL_SE_LOC_1400::RegRepairDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

	return 0;

	EFS_END
	return -1;

}

/// ������� ȭ�� �׸����۾�
/// 1. INDEX�κ��� �׸����� ���� ����
INT CBL_SE_LOC_1400::RegRepairGridProc( INT nDMIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairGridProc") );

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairGridProc") );

	return 0;

	EFS_END
	return -1;

}


// ���α׷����� ����
CLocProgressBarDlg* CBL_SE_LOC_1400::CreateProgressBarDlg( INT nUpper )
{
	EFS_BEGIN

	CLocProgressBarDlg* pProgressBar = new CLocProgressBarDlg(m_pParentMgr);
	pProgressBar->Create( m_pParentMgr );
	pProgressBar->ShowWindow( SW_SHOW );
	pProgressBar->InitRange(nUpper);

	return pProgressBar;

	EFS_END
	return NULL;

}

// ���α׷����� ����
INT CBL_SE_LOC_1400::DeleteProgressBarDlg( CLocProgressBarDlg *pProgressBarDlg )
{
	EFS_BEGIN

	if( pProgressBarDlg != NULL ) 
		delete pProgressBarDlg;
	pProgressBarDlg = NULL;
	
	return 0;

	EFS_END
	return -1;

}


/// ȭ���۾�
/// ���� �׸��� ������ ����
CESL_Grid* CBL_SE_LOC_1400::GetMainGridPtr()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_SE_LOC_1400");
	CString sGridAlias = _T("GRID");

	// CM ����
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) return NULL;
	
	return pGrid;

	EFS_END
	return NULL;

}

INT CBL_SE_LOC_1400::GetInputRegDate( CString &sRegDate )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1420");
	CString sControlAlias = _T("�����������");

	ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias , sRegDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputRegDate") );


	return 0;

	EFS_END
	return -1;

}

/// �׸��忡 ���õ� �ڷᰡ �ִ��� �˻�
INT CBL_SE_LOC_1400::GetSelectMainGridCnt()
{
	EFS_BEGIN

	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetSelectMainGridCnt") );

	if( pDM->GetRowCount() < 1 ) return 0;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetSelectMainGridCnt") );

	pGrid->SelectMakeList();

	if( pGrid->SelectGetCount() < 1 ) return 0;

	return pGrid->SelectGetCount();

	EFS_END
	return -1;

}


/// ���� �Ϸ� PATH
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. �����Ƿ� �ڷ����� Ȯ�� 
/// 2. å���� UPDATE , �������� ���� UPDATE
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�	
INT CBL_SE_LOC_1400::CompleteRepair()
{
	EFS_BEGIN

	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CompleteRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// �����Ϸ����� ����
	CString sCompleteDate;
	ids = GetInputCompleteDate( sCompleteDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CompleteRepair") );

	if( sCompleteDate.IsEmpty() )
	{
		AfxMessageBox( _T("�����Ϸ����� �Է����ֽʽÿ�.") );
		return 2;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CompleteRepair") );

	INT nSuccessCnt = 0;	// ���������Ϸῡ ������ �ڷ� �� 
	CString sRepairStatus;	// ��������

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

		ids = pDM->GetCellData( _T("��������") , nIndex , sRepairStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CompleteRepair") );

		if( sRepairStatus.Compare(_T("�Ƿ�"))!=0 )
		{
			pGrid->SelectGetNext();
			continue;
		}

		// ���� ��� DB �۾�
		ids = CompleteRepairDBProc( nIndex , &TmpDM , sCompleteDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CompleteRepair") );
		if( ids > 0 ) return ids;

		// ���� ��� DM �۾�
		ids = CompleteRepairDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CompleteRepair") );

		// ���� ��� ȭ�� �۾�
		ids = CompleteRepairGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CompleteRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ���� �Ϸ�Ǿ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ���� �Ϸ��Ҽ� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷ��� '%d'���� �ڷᰡ ���� �Ϸ�Ǿ����ϴ�.\r\n")
						   _T("'%d'���� �ڷ�� �ڷ���°� �Ϸ��Ҽ� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;

	EFS_END
	return -1;

}

/// �����Ϸ� DB �۾�
/// 1. å���� UPDATE
/// 2. ������������ UPDATE
/// 3. TmpDM�� ����� ���� ���
INT CBL_SE_LOC_1400::CompleteRepairDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sCompleteDate )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CompleteRepairDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CompleteRepairDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CompleteRepairDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CompleteRepairDBProc") ); 

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL212N") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , _T("SEL213O") );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();
	
	// 1.1. TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("�ڷ����") , _T("���Ӻ�ġ�ڷ�") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CompleteRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����ڷ����") , _T("SEL213O") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CompleteRepairDBProc") ); 

	// 2. ������������ UPDATE
	CString sRepairRecKey;
	ids = pTmpDM->GetCellData( _T("������������KEY") , 0 , sRepairRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CompleteRepairDBProc") ); 

	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("3") );
	pDM->AddDBFieldData( _T("COMPLETE_DATE") , _T("STRING") , sCompleteDate );
	pDM->MakeUpdateFrame( _T("CO_REPAIR_BINDING_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRepairRecKey );
	pDM->InitDBFieldData();

	// 2.1 TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("��������") , _T("�Ϸ�"), 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CompleteRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�Ϸ���"), sCompleteDate , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CompleteRepairDBProc") ); 

	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("CompleteRepairDBProc") ); 

	return 0;

	EFS_END
	return -1;

}

/// �����Ϸ� DM�۾�
/// �ڷ� ���濡 ������ DM�� ������ ���� ���Ǵ� DM�� ����Ѵ�.
INT CBL_SE_LOC_1400::CompleteRepairDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CompleteRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CompleteRepairDMProc") );

	return 0;

	EFS_END
	return -1;

}

/// �����Ϸ� ȭ�� �׸����۾�
/// 1. INDEX�κ��� �׸����� ���� ����
INT CBL_SE_LOC_1400::CompleteRepairGridProc( INT nDMIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CompleteRepairGridProc") );

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CompleteRepairGridProc") );

	return 0;

	EFS_END
	return -1;

}


/// �����Ϸ����� ����
INT CBL_SE_LOC_1400::GetInputCompleteDate( CString &sCompleteDate )
{
	EFS_BEGIN

	sCompleteDate = CLocCommonAPI::GetDBTime(m_pParentMgr).Format(_T("%Y/%m/%d"));
	return 0;

	EFS_END
	return -1;

}



/// ��� ��� PATH
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. ������� �ڷ����� Ȯ�� 
/// 2. å���� UPDATE , �������� ���� UPDATE
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�	
INT CBL_SE_LOC_1400::CancelRepair()
{
	EFS_BEGIN

	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelRepair") );

	INT nSuccessCnt = 0;	// ���������Ϸῡ ������ �ڷ� �� 
	CString sRepairStatus;	// ��������

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

		ids = pDM->GetCellData( _T("��������") , nIndex , sRepairStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CancelRepair") );

		if( sRepairStatus.Compare(_T("���"))!=0 )
		{
			pGrid->SelectGetNext();
			continue;
		}

		// ��� ��� DB �۾�
		ids = CancelRepairDBProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelRepair") );
		if( ids > 0 ) return ids;

		// ��� ��� DM �۾�
		ids = CancelRepairDMProc( nIndex  , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelRepair") );

		// ��� ��� ȭ�� �۾�
		ids = CancelRepairGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CancelRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ��� ��ҵǾ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ��� ����Ҽ� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷ��� '%d'���� �ڷᰡ ��� ��ҵǾ����ϴ�.\r\n")
						   _T("'%d'���� �ڷ�� �ڷ���°� ��� ����Ҽ� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;

	EFS_END
	return -1;

}

/// �����Ϸ� DB �۾�
/// 1. å����UPDATE
/// 2. ������������ UPDATE
/// 3. TmpDM�� ����� ���� ���
INT CBL_SE_LOC_1400::CancelRepairDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelRepairDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelRepairDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CancelRepairDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey,sPreWorkingStatus;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelRepairDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�����ڷ����") , 0 , sPreWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelRepairDBProc") ); 

	CLocCommonAPI::GetWorkingStatusDescToCode( sPreWorkingStatus , 0 );

	// ������ ����
	// 1. å���� UPDATE
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sPreWorkingStatus );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , _T("SEL213O") );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();
	
	// 1.1. TmpDM�� ���
	CLocCommonAPI::GetWorkingStatusDescToCode( sPreWorkingStatus , 1 );
	ids = pTmpDM->SetCellData( _T("�ڷ����") , sPreWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����ڷ����") , _T("SEL213O") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelRepairDBProc") ); 

	// 2. ������������ UPDATE
	CString sRepairRecKey;
	ids = pTmpDM->GetCellData( _T("������������KEY") , 0 , sRepairRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CancelRepairDBProc") ); 

	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("4") );
	pDM->MakeUpdateFrame( _T("CO_REPAIR_BINDING_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRepairRecKey );
	pDM->InitDBFieldData();

	// 2.1 TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("��������") , _T("���"), 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRepairDBProc") ); 

	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("CancelRepairDBProc") ); 

	return 0;

	EFS_END
	return -1;

}

/// �����Ϸ� DM�۾�
/// �ڷ� ���濡 ������ DM�� ������ ���� ���Ǵ� DM�� ����Ѵ�.
INT CBL_SE_LOC_1400::CancelRepairDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelRepairDMProc") );

	return 0;

	EFS_END
	return -1;

}

/// �����Ϸ� ȭ�� �׸����۾�
/// 1. INDEX�κ��� �׸����� ���� ����
INT CBL_SE_LOC_1400::CancelRepairGridProc( INT nDMIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelRepairGridProc") );

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelRepairGridProc") );

	return 0;

	EFS_END
	return -1;

}



/// ���� �Ƿ� PATH
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. �������� ���� UPDATE
/// 2. DM ����
/// 3. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�	
INT CBL_SE_LOC_1400::RequestRepair()
{
	EFS_BEGIN

	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RequestRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	CString sInputData[3];	// 0 : �����⵵ , 1 : ������ȣ , 2 : �Ƿ�����
	ids = GetRequestInputData( sInputData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("") );

	// DM ���� ����
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RequestRepair") );

	INT nSuccessCnt = 0;	// �����Ƿڿ� ������ �ڷ� �� 
	CString sRepairStatus;	// ��������

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

		// ���� �Ƿ� DB �۾�
		ids = RequestRepairDBProc( nIndex , &TmpDM , sInputData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RequestRepair") );
		if( ids > 0 ) return ids;

		// ���� �Ƿ� DM �۾�
		ids = RequestRepairDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RequestRepair") );

		// ���� �Ƿ� ȭ�� �۾�
		ids = RequestRepairGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RequestRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	if( nSuccessCnt > 0 )
	{
		ids = UpdateLastNumber();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RequestRepair") )
	}

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ���� �ǷڵǾ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ���� �Ƿ��Ҽ� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷ��� '%d'���� �ڷᰡ ���� �ǷڵǾ����ϴ�.\r\n")
						   _T("'%d'���� �ڷ�� �ڷ���°� ���� �Ƿ��Ҽ� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;

	EFS_END
	return -1;

}


/// �����Ƿ� DB �۾�
/// 1. ������������ UPDATE
/// 2. TmpDM�� ����� ���� ���
INT CBL_SE_LOC_1400::RequestRepairDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString *sInputData)
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RequestRepairDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RequestRepairDBProc") ); 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RequestRepairDBProc") ); 

	pDM->StartFrame();

	// 2. ������������ UPDATE
	CString sRepairRecKey;
	ids = pTmpDM->GetCellData( _T("������������KEY") , 0 , sRepairRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RequestRepairDBProc") ); 

	pDM->AddDBFieldData( _T("SEQ_YEAR") , _T("STRING") , sInputData[0] );
	pDM->AddDBFieldData( _T("SEQ_NO") , _T("NUMERIC") , sInputData[1] );
	pDM->AddDBFieldData( _T("REQUEST_DATE") , _T("STRING") , sInputData[2] );
	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("2") );
	pDM->MakeUpdateFrame( _T("CO_REPAIR_BINDING_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRepairRecKey );
	pDM->InitDBFieldData();

	// 2.1 TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("��������") , _T("�Ƿ�"), 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����⵵") , sInputData[0], 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("������ȣ") , sInputData[1] , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�Ƿ���") , sInputData[2], 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 

	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("RequestRepairDBProc") ); 

	return 0;

	EFS_END
	return -1;

}




/// �����Ƿ� DM�۾�
/// �ڷ� ���濡 ������ DM�� ������ ���� ���Ǵ� DM�� ����Ѵ�.
INT CBL_SE_LOC_1400::RequestRepairDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RequestRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RequestRepairDMProc") );

	return 0;

	EFS_END
	return -1;

}


/// �������� ȭ�� �׸����۾�
/// 1. INDEX�κ��� �׸����� ���� ����
INT CBL_SE_LOC_1400::RequestRepairGridProc( INT nDMIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RequestRepairGridProc") );

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RequestRepairGridProc") );

	return 0;

	EFS_END
	return -1;

}


/// �����Ƿ� �Է� ���� ����
/// 0:�����⵵ , 1:������ȣ , 2:�����Ƿ�����
INT CBL_SE_LOC_1400::GetRequestInputData( CString *sInputData )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1430");
	// �����⵵ , ������ȣ , �Ƿ����ڰ� �� �ִ��� �˻�
	const INT nCnt = 3;
	CString sControlAlias[nCnt] = 
	{
		_T("�����⵵") , _T("������ȣ") , _T("�����Ƿ�����") 
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sInputData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetRequestInputData") );
		sInputData[i].TrimLeft(); sInputData[i].TrimRight();
	}

	return 0;

	EFS_END
	return -1;

}

/// ��� ���°� �ƴ� �ڷ��� 'V'üũ ����
INT CBL_SE_LOC_1400::SelectCheckRegStatus()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1400");

	CESL_Grid* pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SelectCheckRegStatus") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();

	CString sRepairStatus;
	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("��������") , sRepairStatus , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SelectCheckRegStatus") );
		
		if( sRepairStatus.Compare( _T("���") ) == 0 )
		{
			pGrid->SelectGetNext();
			continue;
		}

		pGrid->SetAt( nIndex , 0 , _T("") );
	
		pGrid->SelectGetNext();
	}

	return 0;

	EFS_END
	return -1;

}


/// ���� ��ȣ ����
/// ������ ��ȣ�������� �����´�.
INT CBL_SE_LOC_1400::MakeSeqNo( CString sYear , CString &sSeqNo )
{
	EFS_BEGIN

	INT ids;

	CString sTableName = _T("CO_LAST_NUMBER_TBL");
	CString sPublishFormCode = _T("SE");
	CString sKindCode = _T("BND_NO");
	CString sSql;
	
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_SE_LOC_1400") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetMaxCheckNumber") );

	INT nCurrentYear = (CLocCommonAPI::GetDBTime(m_pParentMgr)).GetYear();

	// �ش� ���� ������ ��Ϲ�ȣ�� �����ϴ��� �˻�
	sSql.Format( _T("SELECT COUNT(*) FROM %s ")
		_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND MAKE_YEAR=%d AND PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' ") , 
		sTableName , nCurrentYear , sPublishFormCode , sKindCode );


	CString sCount;
	ids = pDM->GetOneValueQuery( sSql , sCount );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-2 , _T("AutoSetUserNo") );

	// ���� ���
	if( _ttoi( sCount ) < 1 )
	{
		CString sYear;
		sYear.Format( _T("%d") , nCurrentYear );

		CString sRecKey;
		pDM->MakeRecKey( sRecKey );

		pDM->StartFrame();

		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("PUBLISH_FORM_CODE") , _T("STRING") , sPublishFormCode );
		pDM->AddDBFieldData( _T("KIND_CODE") , _T("STRING") , sKindCode );
		pDM->AddDBFieldData( _T("MAKE_YEAR") , _T("STRING") , sYear );
		pDM->AddDBFieldData( _T("LAST_NUMBER") , _T("STRING") , _T("0") );
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") );


		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ) );
		pDM->MakeInsertFrame( sTableName );
		pDM->SendFrame();
		pDM->EndFrame();
	}	

	// �ش� ���� ������ ��ȣ ��������
	sSql.Format( _T("SELECT LAST_NUMBER FROM %s ")
		_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND MAKE_YEAR=%d AND PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' ") , 
		sTableName , nCurrentYear , sPublishFormCode , sKindCode );

	ids = pDM->GetOneValueQuery( sSql , sSeqNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("AutoSetUserNo") );

	return 0;

	EFS_END
	return -1;

}

INT CBL_SE_LOC_1400::UpdateLastNumber()
{
	EFS_BEGIN
	
	INT ids;
	
	CString sTableName = _T("CO_LAST_NUMBER_TBL");
	CString sPublishFormCode = _T("SE");
	CString sKindCode = _T("BND_NO");
	CString sSql;
	
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_SE_LOC_1400") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetMaxCheckNumber") );
	
	INT nCurrentYear = (CLocCommonAPI::GetDBTime(m_pParentMgr)).GetYear();
	
	// �ش� ���� ������ ��Ϲ�ȣ�� �����ϴ��� �˻�
	sSql.Format( _T("SELECT COUNT(*) FROM %s ")
		_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND MAKE_YEAR=%d AND PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' ") , 
		sTableName , nCurrentYear , sPublishFormCode , sKindCode );
	
	
	CString sCount;
	ids = pDM->GetOneValueQuery( sSql , sCount );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-2 , _T("AutoSetUserNo") );
	
	// ���� ���
	CString sRecKey;
	if( _ttoi( sCount ) < 1 )
	{
		CString sYear;
		sYear.Format( _T("%d") , nCurrentYear );
		
		pDM->MakeRecKey( sRecKey );
		
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("PUBLISH_FORM_CODE") , _T("STRING") , sPublishFormCode );
		pDM->AddDBFieldData( _T("KIND_CODE") , _T("STRING") , sKindCode );
		pDM->AddDBFieldData( _T("MAKE_YEAR") , _T("STRING") , sYear );
		pDM->AddDBFieldData( _T("LAST_NUMBER") , _T("STRING") , _T("1") );
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") );
		
		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ) );
		pDM->MakeInsertFrame( sTableName );
		pDM->SendFrame();
		pDM->EndFrame();
	}	
	
	// �ش� ���� ������ ��ȣ ��������
	sSql.Format( _T("SELECT REC_KEY FROM %s ")
		_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND MAKE_YEAR=%d AND PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' ") , 
		sTableName , nCurrentYear , sPublishFormCode , sKindCode );
	
	ids = pDM->GetOneValueQuery( sSql , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("AutoSetUserNo") );

	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("LAST_NUMBER") , _T("NUMERIC") , _T("LAST_NUMBER+1") );
	pDM->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("AutoSetUserNo") );
	pDM->EndFrame();

	return 0;

	EFS_END
	return -1;
}
