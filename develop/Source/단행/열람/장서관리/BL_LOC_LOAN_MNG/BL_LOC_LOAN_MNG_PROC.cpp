// BL_LOC_LOAN_MNG_PROC.cpp: implementation of the CBL_LOC_LOAN_MNG_PROC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BL_LOC_LOAN_MNG_PROC.h"
#include "..\\BL_LOC_USER\\BO_LOC_3320.h" //�󼼺���
#include "..\BL_LOC_LOAN\BO_LOC_3170.h"	//���� ������ �ο�
#include "..\BL_LOC_LOAN\BO_LOC_3170_ReserveInfo.h"	//���ټ��� ����� ���� ������ �ο�
#include "..\BL_LOC_LOAN\SMSPreviewDlg.h"	//SMS �߼�
#include "..\BL_LOC_LOAN\KL_UserSearchDlg.h" // ���յ�������
#include "..\..\..\..\����\K2UP_Common\MakeSearchData.h"
// SBL
#include "SBL.h"
// 2011.08.31 ADD : 3000Api
#include "BO_LOC_3000Api.h"
#include "..\\..\\..\\..\\����\\�˻�\\BOBookDetailViewer\\BOBookDetailViewerDlg.h"
#include "..\\..\\..\\..\\����\\�˻�\\SEBookDetailViewer\\SEBookDetailViewerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBL_LOC_LOAN_MNG_PROC::CBL_LOC_LOAN_MNG_PROC(CESL_Mgr *pWnd,CLonaManageValue *pLona,CLoanShareManager* pLoanShareManager)
	: CSPF_Work(pWnd)
{
	m_pLoan = pLona;
	m_pLoanShareManager = pLoanShareManager;

	m_pReservationInfo = NULL;
	m_pReservationInfo = new CBL_LOC_RESERVATOIN_PROC(m_pParentMgr,m_pLoanShareManager,pLona);

	m_strUserID = pWnd->GetUserID();
	m_strLibCode=_T("");

	// 2006-01-12 ADD BY PDW(Jr) ============================================================
	// ����� �÷���
	m_pLoan->GetManageValue( _T("��Ÿ") , _T("����") , _T("�����") , _T("") , m_sDefenceFlag );	
	m_sDefenceFlag.TrimLeft();m_sDefenceFlag.TrimRight();
	// ======================================================================================

	m_nUnmannedLoanStatus = 0;
}

CBL_LOC_LOAN_MNG_PROC::~CBL_LOC_LOAN_MNG_PROC()
{
	if( m_pReservationInfo != NULL ) delete m_pReservationInfo;
}

INT CBL_LOC_LOAN_MNG_PROC::SPFExcutePath( CString sPathAlias )
{
	EFS_BEGIN

	INT ids;

	AfxGetApp()->DoWaitCursor(1);
	if( sPathAlias.Compare( _T("�����ڷ����_�˻�") ) == 0 )
	{
		ids = SearchLoanData();
	}
	else if( sPathAlias.Compare( _T("RFID�˻�") ) == 0 )
	{
		ids = SearchLoanDataFromRFID();
	}
	else if( sPathAlias.Compare( _T("ȸ������") ) == 0 )
	{
		ids = ViewMemberInfo();
	}
	else if( sPathAlias.Compare( _T("�����ڷ����_����") ) == 0 )
	{
		ids = ModifyLoanInfo();
	}
	else if( sPathAlias.Compare( _T("�����ڷ����_�ϰ�����") ) == 0 )
	{
		ids = ModifyAllLoanInfo();
	}
	else if( sPathAlias.Compare( _T("�����ڷ����_�������") ) == 0 )
	{
		ids = CancelReservation();
	}
	else if( sPathAlias.Compare( _T("�����ڷ����_���ุ���Ϻο�") ) == 0 )
	{
		ids = RecordExpireDate();
	}
	else if( sPathAlias.Compare( _T("�����ڷ����_����ó��_ȸ������") ) == 0 )
	{
		ids = ViewMemberInfo2();
	}
	else if( sPathAlias.Compare( _T("�����ڷ����_������") ) == 0 )
	{
		ids = ViewDetail();
	}
	else if( sPathAlias.Compare( _T("����") ) == 0 )
	{
		ids = OpenMail();
	}
	else if( sPathAlias.Compare( _T("���ιݳ�ó��") ) == 0 )
	{
		ids = ReturnFromStation();
	}

	//++2008.05.27 UPDATE BY PWR {{++
	// Ÿ���ݳ� �߰�, ����Ÿ��->�а�
	// �а��ݳ��ڷ�ó��
	else if( sPathAlias.Compare( _T("�а��ݳ�ó��") ) == 0 )
	{
		ids = ReturnFromOtherManageCode();
	}
	// Ÿ���ݳ��ڷ�ó��
	else if( sPathAlias.Compare( _T("Ÿ���ݳ�ó��") ) == 0 )
	{
		ids = ReturnFromOtherLibCode();
	}
	/*
	else if( sPathAlias.Compare( _T("Ÿ���ݳ�ó��") ) == 0 )
	{
		ids = ReturnFromOtherManageCode();
	}
	*/
	//--2008.05.27 UPDATE BY PWR --}}

	/*
	else if( sPathAlias.Compare( _T("�����ڷ����_�ݳ�Ȯ�θ��Ϻ�����") ) == 0 )
	{
		ids = OpenMail(RESERVE_BOOK_RETURN);
	}
	*/
	else if( sPathAlias.Compare( _T("���������ο�") ) == 0 )
	{
		// ���� ��� ����
		// �̵��������� ���
		if( (m_pParentMgr->m_pInfo->MODE == 10000) || (m_pParentMgr->m_pInfo->MODE == 30000) )
			return 0;

		ids = ReViewOrder();
	}
	if( ids < 0 ) 
	{
		ViewMessage( _T("������ �߻��Ͽ� ����� �������� ���Ͽ����ϴ�.") );
		return -1;
	}
	return 0;
	
	AfxGetApp()->DoWaitCursor(-1);
	
	EFS_END
	return -1;

}


VOID CBL_LOC_LOAN_MNG_PROC::ViewMessage( CString sMsg )
{
	EFS_BEGIN

	MessageBox( m_pParentMgr->m_hWnd , sMsg , _T("�����ڷ����") , MB_ICONINFORMATION | MB_OK );

	EFS_END
}

// ������������_�˻� PATH
INT CBL_LOC_LOAN_MNG_PROC::SearchLoanDataFromRFID()
{
	EFS_BEGIN
	
	BEGIN_SBL();

	INT ids;
	
	CString sDMAlias[3] = { _T("DM_BO_LOC_3200_BO") , _T("DM_BO_LOC_3200_NON_DB") , _T("DM_BO_LOC_3200_SE") };
	CString sCMAlias = _T("CM_BO_LOC_3200");
	
	CESL_DataMgr* pDstDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3200"));
	if( pDstDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("SearchLoanData") );
	
	pDstDM->FreeData();

	CString sSerialNo;
	GetTempData( _T("WHERE") , sSerialNo );
	
	// DB�۾�
	CString sWhere;		// WHERE��
	CESL_DataMgr *pDM;	// DM MANAGER
	INT nDMCnt = 3; 
	for( INT i = 0 ; i < nDMCnt ; i++ )
	{
		// ������ ����
//DEL 		if( i == 0 || i == 2 )
			sWhere.Format( _T("B.WORKNO = '%s' ") , sSerialNo);
//DEL 		else
//DEL 			sWhere.Format( _T("NB.WORKNO = '%s' ") , sSerialNo);
		
		if( sWhere.IsEmpty() ) continue;
		
		pDM = m_pParentMgr->FindDM( sDMAlias[i] );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchLoanData") );
		ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchLoanData") );

		if( pDM->GetRowCount() == 0 ) continue;
		
		ids = CLocCommonAPI::AddDM( pDM , pDstDM , m_pParentMgr );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SearchLoanData") )
	}

	// 2011.09.05 ADD : 3000Api
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
	{
		CString strGetData;
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7;
		for(INT i = 0; i < pDstDM->GetRowCount(); i++)
		{
			pDstDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strGetData);
			asDes1.Add(strGetData);
			pDstDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes2.Add(strGetData);
			pDstDM->GetCellData(_T("��ȭ��ȣ_����"), i, strGetData);
			asDes3.Add(strGetData);
			pDstDM->GetCellData(_T("��ȭ��ȣ_�ٹ���"), i, strGetData);
			asDes4.Add(strGetData);
			pDstDM->GetCellData(_T("�޴���"), i, strGetData);
			asDes5.Add(strGetData);
			pDstDM->GetCellData(_T("�ּ�_����"), i, strGetData);
			asDes6.Add(strGetData);
			pDstDM->GetCellData(_T("�ּ�_�ٹ���"), i, strGetData);
			asDes7.Add(strGetData);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes4);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes6);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes7);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pDstDM->GetRowCount(); i++)
				{
					pDstDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), asDes1.GetAt(i), i);
					pDstDM->SetCellData(_T("E_MAIL"), asDes2.GetAt(i), i);
					pDstDM->SetCellData(_T("��ȭ��ȣ_����"), asDes3.GetAt(i), i);
					pDstDM->SetCellData(_T("��ȭ��ȣ_�ٹ���"), asDes4.GetAt(i), i);
					pDstDM->SetCellData(_T("�޴���"), asDes5.GetAt(i), i);
					pDstDM->SetCellData(_T("�ּ�_����"), asDes6.GetAt(i), i);
					pDstDM->SetCellData(_T("�ּ�_�ٹ���"), asDes7.GetAt(i), i);
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
	}
	
	// ���� ���ϱ�
	 ReViewOrder(_T("DM_BO_LOC_3200"));
	
	// �����ϱ�(M_2004-1006_HAN...)
	const INT SORT_COND = 2;
	INT nSortCond[SORT_COND] = { ESL_DATAMGR_SORT_ASC, ESL_DATAMGR_SORT_DESC };
	CString sSortCond[SORT_COND] = { _T("������"), _T("������") };

	pDstDM->SORT( SORT_COND, sSortCond, nSortCond, ESL_DATAMGR_SORT_ASC );
	//pDstDM->SORT( _T("������") , TRUE );
	
	// ȭ�� �۾�
	ids = GridDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SearchLoanData") );
	
	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3200") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);

	if( pDstDM->GetRowCount() > 0 )	AfxMessageBox( _T("�ڷᰡ �˻��Ǿ����ϴ�.") );
	else							AfxMessageBox( _T("�ڷᰡ �������� �ʽ��ϴ�.") );

	return 0;
	
EFS_END

	return -1;	
}

// ������������_�˻� PATH

/*
@Description
 -
@Parameter
 -
@Return Value
 - 0 : ����
 - ���� : ����
@Mini Spec
//DEL 2009.02.19 COMMENT BY CJY
//DEL �����ڷ���� �˻��ӵ� ������ ���� ��������
//DEL ���� ��� 
//DEL  1. ������ DM���� ���� ����/����/����ڷḦ �˻��Ѵ�.
//DEL  2. �˻��� ������ DM_BO_LOC_3200�� �߰���Ų��.
//DEL ����� ���
//DEL  1. ������ DM���� ���� ���̺��� �˻������� ���´�.
//DEL  2. DM_BO_LOC_3200�� �ش� ���̺��� �˻������� �������� SQL���� ���´�.
//DEL  3. ���� SQL���� UNION ALL�� ���� �� �˻��Ѵ�
//DEL - ���ǻ��� -
//DEL 1. �˻��� DM_BO_LOC_3200�θ� ����Ǳ� ������ DM_BO_LOC_3200_BO � Į���� �߰��ص� �ǹ̰� ����.
//DEL 2. ������ ���̺���̳� ������������ �������� ������ ����/����/���DM�� ����� �ȵȴ�.
//DEL 3. RefreshDataMgr�δ� UNION�� �������� �ʱ� ������ RestructDataMgr�� ����Ͽ���.
//DEL    ����ó�� ALIAS�� ���ؼ� GetCellData�� SetCellData�� �ϱ� ���ؼ��� 
//DEL    SELECT�Ǵ� Į���� ������ DM�� Į���� ������ ��ġ�ؾ� �Ǵ� ��,
//DEL    UDF�� ����ϰ� �Ǹ� Į��������ü�� Ʋ������ ������ ����ؼ� �ȵȴ�.
//DEL    UDF����� ����Ϸ��� FIELD NAME �׸� NULL�� �������ش�.
//DEL 4. ����/����/��� ���̺� ���� �����;� �� �׸��� �ٸ� ��� �Ʒ����� Replace�� ���� ��ü���ָ� �ȴ�.
*/
INT CBL_LOC_LOAN_MNG_PROC::SearchLoanData()
{
	EFS_BEGIN

	BEGIN_SBL();

	INT ids;

	CString sDMAlias[3] = { _T("DM_BO_LOC_3200_BO") , _T("DM_BO_LOC_3200_NON_DB") , _T("DM_BO_LOC_3200_SE") };
	CString sCMAlias = _T("CM_BO_LOC_3200");

	CESL_DataMgr* pDstDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3200"));
	if( pDstDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("SearchLoanData") );

	pDstDM->FreeData();

	// DB�۾�
	CString sWhere;		// WHERE��
	CESL_DataMgr *pDM;	// DM MANAGER
	INT nDMCnt = 3; //�ϴ��� ���� �ڷḸ , ��޴����ڷḸ
	bool bFirstWaringMsg = FALSE;

	CString strFullQuery;
	for( INT i = 0 ; i < nDMCnt ; i++ )
	{
		// ������ ����
		ids = GetSqlBO_LOC_3210( sWhere , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchLoanData") );
		if( ids == 1 ) continue;

		if( sWhere.IsEmpty() && !bFirstWaringMsg )
		{
			if( i == 0 )
			{
				ids = MessageBox( m_pParentMgr->m_hWnd , _T("�˻� ������ �����Ͽ� �ð��� ���� �ɸ��ų� ����� ����� �������� ���Ҽ� �ֽ��ϴ�. \r\n�˻��Ͻðڽ��ϱ�?") , _T("���") , MB_ICONWARNING | MB_YESNO );
				if( ids == IDNO )
					return 0;
				bFirstWaringMsg = TRUE;
			}
		}

		pDM = m_pParentMgr->FindDM( sDMAlias[i] );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchLoanData") );
		

		// 2009.02.11 ADD BY CJY : ����/����/������� ���� ������ �˻��� ��ġ�� ���� UNION���� ����
		CString strExtraCondition = pDM->EXTRA_CONDITION;
		CString strTblName = pDM->TBL_NAME;
		
		pDstDM->TBL_NAME = pDM->TBL_NAME;
		pDstDM->CONDITION = pDM->CONDITION;
		pDstDM->EXTRA_CONDITION = _T("");
		pDstDM->SetCONNECTION_INFO(_T(""));
			

		CString strQuery, strLSQuery, strCLQuery;

		INT aColIndex[256];	
		strQuery = strLSQuery = strCLQuery = pDstDM->MakeSelectQuery(sWhere, aColIndex);
	
		// VIEW�˻��� UNION���� ����� �˻�
		strLSQuery.Replace(_T("LH_STORAGE_V01"), _T("LS_WORK_T01"));
		strCLQuery.Replace(_T("LH_STORAGE_V01"), _T("CO_LOAN_TBL"));

		strQuery = strLSQuery + _T(" UNION ALL ") + strCLQuery;

		//DEL Ư�� Į���� ���� ����/����/��޿� ���� SELECT�Ǵ� �׸��� �ٸ��ٸ� �̰����� �ٲ�ġ�� �Ѵ�.
		if(_T("DM_BO_LOC_3200_BO") == sDMAlias[i])
		{
			strQuery.Replace(_T("<$�ڷᱸ��$>"),		_T("'����'"));
			strQuery.Replace(_T("<$����/���ӱ���$>"),	_T("'MO'"));
			strQuery.Replace(_T("<$�����$>"),			_T("B.PUBLISH_YEAR"));
			strQuery.Replace(_T("<$����ID$>"),			_T("/*SQL TUNING 2005.08 PDJ*/ /*+ RULE */ (SELECT /*+ INDEX_DESC (CLFI CO_LOAN_FAMILY_ID_IDX1) */ CLFI.ID FROM CO_LOAN_FAMILY_ID_TBL CLFI WHERE CLU.FID_KEY=CLFI.REC_KEY AND ROWNUM=1)"));
			strQuery.Replace(_T("<$�ηϿ���$>"),		_T("B.BOOK_APPENDIX_FLAG")); // 2009.05.13 ADD BY PWR : �ηϿ��� �߰�
		}
		else if(_T("DM_BO_LOC_3200_SE") == sDMAlias[i])
		{
			strQuery.Replace(_T("DECODE( B.BOOK_APPENDIX_FLAG, 'A', NULL, 'B', B.VOL_TITLE )"),	_T("NULL"));
			strQuery.Replace(_T("<$�ڷᱸ��$>"),		_T("'����'"));
			strQuery.Replace(_T("<$����/���ӱ���$>"),	_T("'SE'"));
			strQuery.Replace(_T("<$�����$>"),			_T("NULL"));
			strQuery.Replace(_T("<$����ID$>"),			_T("/*SQL TUNING 2005.08 PDJ*/ /*+ RULE */ (SELECT /*+ INDEX_DESC (CLFI CO_LOAN_FAMILY_ID_IDX1) */ CLFI.ID FROM CO_LOAN_FAMILY_ID_TBL CLFI WHERE CLU.FID_KEY=CLFI.REC_KEY AND ROWNUM=1)"));
			strQuery.Replace(_T("<$�ηϿ���$>"),		_T("B.BOOK_TYPE")); // 2009.05.13 ADD BY PWR : �ηϿ��� �߰�
		}
		else if(_T("DM_BO_LOC_3200_NON_DB") == sDMAlias[i])
		{
			strQuery.Replace(_T("B.BOOK_CODE"),			_T("NULL"));
			strQuery.Replace(_T("B.COPY_CODE"),			_T("NULL"));
			strQuery.Replace(_T("B.VOL_CODE"),			_T("NULL"));
			strQuery.Replace(_T("I.AUTHOR_INFO"),		_T("B.AUTHOR"));
			strQuery.Replace(_T("I.IDX_TITLE"),			_T("B.TITLE"));
			strQuery.Replace(_T("I.PUB_INFO"),			_T("B.PUBLISHER"));
			strQuery.Replace(_T("I.REC_KEY"),			_T("NULL"));
			strQuery.Replace(_T("I.TITLE_INFO"),		_T("B.TITLE"));
			strQuery.Replace(_T("DECODE( B.BOOK_APPENDIX_FLAG, 'A', NULL, 'B', B.VOL_TITLE )"),	_T("NULL"));			
			strQuery.Replace(_T("<$�ڷᱸ��$>"),		_T("'���'"));
			strQuery.Replace(_T("<$����/���ӱ���$>"),	_T("'NB'"));
			strQuery.Replace(_T("<$�����$>"),			_T("NULL"));
			strQuery.Replace(_T("<$����ID$>"),			_T("/*SQL TUNING 2005.08 PDJ*/ /*+ RULE */ (SELECT /*+ INDEX_DESC (CLFI CO_LOAN_FAMILY_ID_IDX1) */ CLFI.ID FROM CO_LOAN_FAMILY_ID_TBL CLFI WHERE CLU.FID_KEY=CLFI.REC_KEY AND ROWNUM=1)"));			
			strQuery.Replace(_T("<$�ηϿ���$>"),		_T("NULL")); // 2009.05.13 ADD BY PWR : �ηϿ��� �߰�
		}

		if(TRUE == strFullQuery.IsEmpty()) strFullQuery = strQuery;
		else strFullQuery = strFullQuery + _T(" UNION ALL ") + strQuery;		

		// 2009.02.11 DEL BY CJY : ���� / ���� / ������� ������ �˻��ϴ� ��� �ѹ��� �˻��ϵ��� ����Ǿ� ����
//DEL 		ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );
//DEL 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchLoanData") );

//DEL 		if( pDM->GetRowCount() == 0 ) continue;
//DEL 		ids = CLocCommonAPI::AddDM( pDM , pDstDM , m_pParentMgr );
//DEL 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SearchLoanData") );

	}

	DWORD nSTART, nFINISH;
	nSTART = GetTickCount();

	strFullQuery = strFullQuery + _T(" ORDER BY NAME ASC, LOAN_DATE DESC");

//DEL DM_BO_LOC_3200�� UDF�� �� �׸��� ���ٰ� �����Ѵ�.
//DEL UDF�� �����Ǿ�� �ϴ� �׸��� DATA_TYPE�� FIELD_NAME�� NULL�� ����
	pDstDM->RestructDataManager(strFullQuery);

	nFINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - ���� �ڷ� ���� �ڷ� ã�� �˻��� �ɸ� �ð� :  [%d.%d sec]\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	TRACE(strMsg.GetBuffer(0));
	pDM->QueryLog(strMsg.GetBuffer(0));

	// 2007.08.20 update by pdw
	// �˻����� ���� ������ ���� ���ĵǵ��� ����
/*	// �����ϱ�(M_2004-1006_HAN...)
	const INT SORT_COND = 2;
	INT nSortCond[SORT_COND] = { ESL_DATAMGR_SORT_ASC, ESL_DATAMGR_SORT_DESC };
	CString sSortCond[SORT_COND] = { _T("������"), _T("������") };

	pDstDM->SORT( SORT_COND, sSortCond, nSortCond, ESL_DATAMGR_SORT_ASC );
	//pDstDM->SORT( _T("������") , TRUE );
*/

	// 2009.02.20 DEL BY CJY : �����ͺ��̽� �˻��� �̹� ������ �ؼ� ������ �´�.
// 	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(sCMAlias);
// 	if( !pCM )ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("SearchLoanData") );
	
// 	CString strSORT;
// 	pCM->GetControlMgrData( _T("���ı���") , strSORT , -1 , 0 );
// 	const INT SORT_COND = 2;
// 	INT nSortCond[SORT_COND] = { ESL_DATAMGR_SORT_ASC, ESL_DATAMGR_SORT_DESC };
// 	CString sSortCond[SORT_COND] = { _T("������"), strSORT };

//  	pDstDM->SORT( SORT_COND, sSortCond, nSortCond, ESL_DATAMGR_SORT_ASC );

	// 2011.08.31 ADD : 3000Api
	CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7;

	// 2007.12.22 ADD BY PWR : �������� ���� �������м������� ����
	INT nCnt = pDstDM->GetRowCount();
	CString sData, sDesc;
	for(i = 0; i < nCnt; i++)
	{
		ids = pDstDM->GetCellData(_T("��������"), i, sData);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("AllControlDisplayGrid") );
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("��������"), sData, sDesc );
		ids = pDstDM->SetCellData( _T("�������м���"), sDesc, i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("AllControlDisplayGrid") );		

		// 2011.08.31 ADD : 3000Api
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			// 1:�ֹε�Ϲ�ȣ, 2:E_MAIL, 3:��ȭ��ȣ_����, 4:��ȭ��ȣ_�ٹ���, 5:�޴���, 6:�ּ�_����, 7:�ּ�_�ٹ���
			CString strGetData;
			pDstDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strGetData);
			asDes1.Add(strGetData);
			pDstDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes2.Add(strGetData);
			pDstDM->GetCellData(_T("��ȭ��ȣ_����"), i, strGetData);
			asDes3.Add(strGetData);
			pDstDM->GetCellData(_T("��ȭ��ȣ_�ٹ���"), i, strGetData);
			asDes4.Add(strGetData);
			pDstDM->GetCellData(_T("�޴���"), i, strGetData);
			asDes5.Add(strGetData);
			pDstDM->GetCellData(_T("�ּ�_����"), i, strGetData);
			asDes6.Add(strGetData);
			pDstDM->GetCellData(_T("�ּ�_�ٹ���"), i, strGetData);
			asDes7.Add(strGetData);
		}
	}

	// 2011.08.31 ADD : 3000Api
	if(0 < asDes1.GetSize() && TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
	{
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes4);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes6);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes7);
					pApi->CloseSocket();
				}
				for(i = 0; i < nCnt; i++)
				{
					pDstDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), asDes1.GetAt(i), i);
					pDstDM->SetCellData(_T("E_MAIL"), asDes2.GetAt(i), i);
					pDstDM->SetCellData(_T("��ȭ��ȣ_����"), asDes3.GetAt(i), i);
					pDstDM->SetCellData(_T("��ȭ��ȣ_�ٹ���"), asDes4.GetAt(i), i);
					pDstDM->SetCellData(_T("�޴���"), asDes5.GetAt(i), i);
					pDstDM->SetCellData(_T("�ּ�_����"), asDes6.GetAt(i), i);
					pDstDM->SetCellData(_T("�ּ�_�ٹ���"), asDes7.GetAt(i), i);
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
	}

	// ȭ�� �۾�
	ids = GridDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("SearchLoanData") );

	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3200") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);

	if( pDstDM->GetRowCount() > 0 )	AfxMessageBox( _T("�ڷᰡ �˻��Ǿ����ϴ�.") );
	else							AfxMessageBox( _T("�ڷᰡ �������� �ʽ��ϴ�.") );


	return 0;

	EFS_END
	return -1;

}

// �����ڷ���� WHERE�� ����
// nMode = 0 -> ����
//         1 -> ��޴���
//         2 -> ����
/*
INT CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )
{
	EFS_BEGIN

	INT ids;

	sWhere.Empty();
	
	CString sCMAlias = _T("CM_BO_LOC_3210");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );

	// �ڷ� ������ ���� ���� SQL���� ����
	CString sCMode[3] = { _T("����") , _T("���") , _T("����") };

	CString sMode;
	ids = pCM->GetControlMgrData( _T("�ڷᱸ��") , sMode , -1 , 0 );

	if( sMode.Compare(_T("��ü"))!=0 )
	{
		if( sMode.Compare( sCMode[nMode] ) != 0 )
			return 1;
	}

	// �˻� �������� ��尡 _T("�󼼰˻�") ����� ��츸 �˻� ���ǿ� �߰�
	CString sModeAlias;
	CESL_Control_Element *pCE = pCM->FindControlElement( _T("��������") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
	(m_pParentMgr->GetDlgItem( pCE->CTRL_ID ))->GetWindowText( sModeAlias );

	// 2005.11.11 UPD BY PDW(jr) /////////////////////////////////////////////////////
	// ����� �˻� ���� �߰� 
	// ���Ա��а� �谡���ڸ� �˻����ǿ��� �߰�, ������ ���� �Է¹��� �ʴ´�. 
	// ����� ���� �߰�
	const INT nCtrlCnt = 45;
	CString sCtrlAlias[ nCtrlCnt ] =
	{
		_T("�����ڷ����_��ü")		, _T("�����ڷ����_����")		, _T("�����ڷ����_�ݳ�")		, _T("�����ڷ����_��ü")	, _T("�����ڷ����_�ݳ�����")	,
		_T("�����ڷ����_����")		, _T("�����ڷ����_�������")	, _T("�����ڷ����_���ιݳ���")	, _T("��������")			, _T("��������")				, 
		_T("�ڷ�Ǹ�")				, _T("������1")					, _T("������2")					, _T("�ݳ�������1")			, _T("�ݳ�������2")				,
		_T("�ݳ���1")				, _T("�ݳ���2")					, _T("������1")					, _T("������2")				, _T("���ุ����1")				,
		_T("���ุ����2")			, _T("����Ҽ�����")			, _T("������������")			, _T("�����ڹ�ȣ1")			, _T("�����ڹ�ȣ2")				,
		_T("������1")				, _T("������2")					, _T("��ǥ��")					, _T("������")				, _T("��ġ��ȣ1")				,
		_T("��ġ��ȣ2")				, _T("�з���ȣ1")				, _T("�з���ȣ2")				, _T("��Ϲ�ȣ����")		, _T("��Ϲ�ȣ1")				,
		_T("��Ϲ�ȣ2")				, _T("�ڵ���")					, _T("�����ڷ����_����Ϸ�")   , _T("���Ա���")            , _T("�谡��1")                 ,
		_T("�谡��2")               , _T("������1")                 , _T("������2")                 , _T("����1")				, _T("����2")
	};
	INT nBoFieldType[ nCtrlCnt ] = 
	{
		ETC			, ETC		, ETC			, ETC			, ETC		, 
		ETC			, ETC		, ETC			, ONESTRING		, ONESTRING	, 
		ONESTRING	, TWODATE	, ADDINFO		, TWODATE		, ADDINFO	,
		TWODATE		, ADDINFO	, TWODATE		, ADDINFO		, TWODATE	,
		ADDINFO		, ONESTRING	, ONESTRING		, TWOSTRING		, ADDINFO	,
		TWOSTRING	, ADDINFO	, ONEINDEXSEARCH, ONEINDEXSEARCH, TWOSTRING	,
		ADDINFO		, TWOSTRING	, ADDINFO		, THREESTRING	, ADDINFO	,
		ADDINFO		, ADDINFO	, ETC           , ONESTRING     , TWODATE   ,
		ADDINFO     , TWODATE   , ADDINFO       , ADDINFO		, ADDINFO		
	};
	INT nNonDBFieldType[ nCtrlCnt ] = 
	{
		ETC			, ETC		, ETC		, ETC			, ETC		, 
		ETC			, ETC		, ETC		, ONESTRING		, ONESTRING	, 
		ONESTRING	, TWODATE	, ADDINFO	, TWODATE		, ADDINFO	,
		TWODATE		, ADDINFO	, TWODATE	, ADDINFO		, TWODATE	,
		ADDINFO		, ONESTRING	, ONESTRING	, TWOSTRING		, ADDINFO	,
		TWOSTRING	, ADDINFO	, ONESTRING	, ONESTRING		, TWOSTRING	,
		ADDINFO		, ETC		, ETC		, THREESTRING	, ADDINFO	,
		ADDINFO		, ADDINFO	, ETC       , ETC           , ETC       ,
		ETC         , ETC       , ETC       , ADDINFO		, ADDINFO		
	};

	TCHAR *sBo_ECOSERARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("CL.MANAGE_CODE") , 
		_T("CL.SHELF_LOC_CODE") , _T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") ,
		_T("CL.RETURN_DATE") , _T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") ,
		_T("CL.RESERVATION_EXPIRE_DATE") , _T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") ,
		_T("CLU.NAME") , _T("CLU.NAME") , _T("I.BO_IDX_TITLE") , _T("I.BO_IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") ,
		_T("B.SEPARATE_SHELF_CODE") , _T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") ,
		_T("B.REG_NO"), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc")		
	};

	TCHAR *sBo_CATSEARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("CL.MANAGE_CODE") , 
		_T("CL.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.NAME") , 
		_T("CLU.NAME") , _T("I.IDX_TITLE") , _T("I.IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , 
		_T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc")
	};

	TCHAR *sSe_ECOSEARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("CL.MANAGE_CODE") , 
		_T("CL.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.NAME") , 
		_T("CLU.NAME") , _T("I.SE_IDX_TITLE") , _T("I.SE_IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , 
		_T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc")
	};

	TCHAR *sSe_CATSEARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("CL.MANAGE_CODE") , 
		_T("CL.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.NAME") , 
		_T("CLU.NAME") , _T("I.IDX_TITLE") , _T("I.IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , 
		_T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc")
	};

	
	TCHAR *sNonDBDBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("CL.MANAGE_CODE") , 
		_T("CL.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.NAME") , 
		_T("CLU.NAME") , _T("NB.TITLE") , _T("NB.AUTHOR") , _T("NB.SEPARATE_SHELF_CODE") , _T("NB.SEPARATE_SHELF_CODE") , 
		_T("etc") , _T("etc") , _T("NB.REG_NO") , _T("NB.REG_NO") , _T("NB.REG_NO"), _T(""), _T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,_T("CLU.CLASS_NO") , _T("CLU.CLASS_NO")
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////

	TCHAR **pDBFieldName;
	pDBFieldName = NULL;
	INT *pnDBFieldType;
	pnDBFieldType = NULL;

	if( nMode == 0 )
	{
		if( m_pParentMgr->m_pInfo->GetSearchIndexType() == 1 )
			pDBFieldName = sBo_ECOSERARCH_DBFieldName;
		else
			pDBFieldName = sBo_CATSEARCH_DBFieldName;
		pnDBFieldType = nBoFieldType;
	}
	else if( nMode == 1 )
	{
		pDBFieldName = sNonDBDBFieldName;
		pnDBFieldType = nNonDBFieldType;
	}
	else if( nMode == 2 )
	{
		if( m_pParentMgr->m_pInfo->GetSearchIndexType() == 1 )
			pDBFieldName = sSe_ECOSEARCH_DBFieldName;
		else
			pDBFieldName = sSe_CATSEARCH_DBFieldName;
		pnDBFieldType = nBoFieldType;
	}


	CString sGetData[ nCtrlCnt ];

	for( INT i = 0 ; i < nCtrlCnt ; i++ )
	{
		if( pnDBFieldType[i] == ETC ) continue;
		ids = pCM->GetControlMgrData( sCtrlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
	}

	// ���� ����
	if( sGetData[8].Compare(_T("�������")) == 0 )
		sGetData[8] = _T("");
	else if( sGetData[8].Compare(_T("�Ϲ�")) == 0 )
		sGetData[8] = _T("0");
	else if( sGetData[8].Compare(_T("Ư��")) == 0 )
		sGetData[8] = _T("1");
	else if( sGetData[8].Compare(_T("����")) == 0 )
		sGetData[8] = _T("2");
	else if( sGetData[8].Compare(_T("����")) == 0 )
		sGetData[8] = _T("3");
	// 2005.09.27 ADD BY PDW(Jr) ++++++++++++++++++++
	// ��� ���� �߰�
	else if( sGetData[8].Compare(_T("���")) == 0 )
		sGetData[8] = _T("4");
	// ______________________________________________

	INT nCnt;
	if( sModeAlias.Compare( _T("�󼼰˻�") ) == 0 ) 
		nCnt = 27;
	else 
		nCnt = nCtrlCnt;
	ids = MakeWhereSql( nCnt , pnDBFieldType , pDBFieldName , sGetData , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );


	// New Ecosearch ����
	// �׽�Ʈ�� ���� �ӽ� ����
	// 27 , 28  
	FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("r"));
	BOOL bNewEcoSearch = FALSE;
	if( fd == NULL )
		bNewEcoSearch = TRUE;
	else
		fclose(fd);
	
	CString sEcoSearchWhere;
	INT nEcoSearchCnt=0;
	if( nCnt > 27 && ( nMode == 0 || nMode == 2 ) && bNewEcoSearch && 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
	{
		if( m_pParentMgr->m_pInfo->MODE == 10000 || ( m_pParentMgr->m_pInfo->MODE == 30000 ) )
		{
		}
		else
		{
			CMakeSearchData MakeSearchData(m_pParentMgr);	
			
			// 2005.02.19 ADD BY PDJ 
			// �ʱ�ȭ �ʿ�
			// ���� ������ TRUE�� �Ǿ���.
			BOOL bIsEqualSearch = FALSE;

			/// ���� �˻� , ������ �˻�
			// ���ô� ������ �˻� ����
			if( !sGetData[27].IsEmpty() )
			{
				if( sGetData[27].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;
				
				MakeSearchData.GetKeyWord(sGetData[27]);
				if( !bIsEqualSearch ) 
					sGetData[27] += _T("*");
				
				sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_TITLE, '") + sGetData[27] + _T("') ");
				nEcoSearchCnt++;
			}
			
			if( !sGetData[28].IsEmpty() )
			{
				if( sGetData[28].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;
				
				MakeSearchData.GetKeyWord(sGetData[28]);
				if( !bIsEqualSearch ) 
					sGetData[28] += _T("*");
				
				if( !sEcoSearchWhere.IsEmpty() )
					sEcoSearchWhere += _T(" INTERSECT ");
				
				sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_AUTHOR, '") + sGetData[28] + _T("') ");
				nEcoSearchCnt++;
			}
			
			if( !sEcoSearchWhere.IsEmpty() )
			{
				if( !sWhere.IsEmpty() )
					sWhere += _T(" AND ");
				if( nEcoSearchCnt == 1 )
					sWhere += _T("I.REC_KEY IN ") + sEcoSearchWhere ;
				else
					sWhere += _T("I.REC_KEY IN ( ") + sEcoSearchWhere +_T(")");
			}
		}
	}
	
	
	// ���� �߰�
	// �ڵ��� ��ȣ �˻��߰�
	// IDX_HANDPHONE

	sGetData[ 36 ].TrimLeft();
	sGetData[ 36 ].TrimRight();
	sGetData[ 36 ].Replace( _T(" "), _T("") );
	sGetData[ 36 ].Replace( _T("-"), _T("") );
	if( !sGetData[ 36 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		sWhere += ( _T("CLU.IDX_HANDPHONE like '%") + sGetData[ 36 ] + _T("%'") );
	}

	/// 20040223 ������ ����
	/// �� �����ȣ , �ּ� �߰�
	CString sZipCode , sAddress;
	ids = pCM->GetControlMgrData( _T("�����ȣ") , sZipCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -201 , _T("GetSqlBO_LOC_3210") );
	sZipCode.TrimLeft(); sZipCode.TrimRight();

	ids = pCM->GetControlMgrData( _T("�ּ�") , sAddress , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -201 , _T("GetSqlBO_LOC_3210") );
	sAddress.TrimLeft(); sAddress.TrimRight();

	if( !sZipCode.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		sWhere += _T("CLU.H_ZIPCODE LIKE ('")+sZipCode+_T("%')");
	}

	if( !sAddress.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");
		
		sWhere += _T("CLU.H_ADDR1 LIKE ('%")+sAddress+_T("%')");
	}
	
	// 2005.11.11 ADD BY PDW(Jr) /////////////////////////////////////////////
	// ���� �߰�
	sGetData[ 43 ].TrimLeft();sGetData[ 43 ].TrimRight();
	if( !sGetData[ 43 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		CString sClass = _T("");
		sGetData[ 44 ].TrimLeft();sGetData[ 44 ].TrimRight();
		if( !sGetData[ 44 ].IsEmpty() ) 
			sClass = _T("CLU.CLASS_NO BETWEEN '") + sGetData[ 43 ] + _T("' AND '") + sGetData[ 44 ] + _T("'");
		else
			sClass = _T("CLU.CLASS_NO = '") + sGetData[ 43 ] + _T("'");
		
		sWhere += sClass;
	}

	// üũ �ڽ� ����
	INT nCheck;
	CESL_Control_Element *pCheckCE;

	CString sWhere2;

	// ���� �ݳ��� üũ
	CString sStation,sDoor;
//	pCM->GetControlMgrData( _T("���ιݳ���") , sStation , -1 , 0 );
//	pCM->GetControlMgrData( _T("���ιݳ���") , sDoor , -1 , 1 );
	
	CComboBox* pCombo = (CComboBox*)(pCM->FindControl(_T("���ιݳ���")));
	INT nSSIdx = pCombo->GetCurSel();

	if( nSSIdx != 0 )
	{
		CESL_DataMgr* pStationList = m_pParentMgr->FindDM(_T("DM_���ιݳ�����"));
		// �˻��� PC_NAME���� �ؾ��Ѵ�.
		pStationList->GetCellData( _T("�����̼�") , nSSIdx-1 , sStation );
		pStationList->GetCellData( _T("�����̸�") , nSSIdx-1 , sDoor );
	}

	sStation.TrimLeft(); sStation.TrimRight();
	sDoor.TrimLeft(); sDoor.TrimRight();
	
	if( !sStation.IsEmpty() )
	{
		if( !sWhere2.IsEmpty() ) sWhere2 += _T(" AND ");
		if( !sDoor.IsEmpty() )
			sWhere2 += _T(" CL.SELF_RETURN_STATION = '")+sStation+_T("' AND  CL.SELF_RETURN_DOOR = '")+sDoor+_T("' ");
		else
			sWhere2 += _T(" CL.SELF_RETURN_STATION = '")+sStation+_T("' ");
	}

	if( !sWhere2.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += sWhere2;
	}
	
	sWhere2.Empty();

	for( i = 0 ; i < 9 ; i++ )
	{
		if ( (m_pLoan->m_bVolReserve) && (9==(i+1)) )		i = 37;
		else if ( !(m_pLoan->m_bVolReserve) && (9==(i+1)) )	continue;

		pCheckCE= pCM->FindControlElement( sCtrlAlias[i] );
		if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck != 1 ) continue;
		
		// ��ü üũ
		if( i == 0 )	return 0;
		// ���� üũ
		else if( i == 1 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '0')");
		}
		// �ݳ� üũ
		else if( i == 2 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '1')");
		}
		// ��ü üũ
		else if( i == 3 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");

			if( m_pParentMgr->m_pInfo->MODE == 10000 || (m_pParentMgr->m_pInfo->MODE==30000) )	
				sWhere2 += _T(" ( CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < to_char(now(),'YYYY/MM/DD') ) ");
			else
				sWhere2 += _T(" ( CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < to_char(SYSDATE,'YYYY/MM/DD') ) ");
		}
		// �ݳ� ���� üũ
		else if( i == 4 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '2')");
		}
		// ���� üũ or ���� üũ ������
		else if( i == 5 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			if ( m_pLoan->m_bVolReserve )
				sWhere2 += _T(" (CL.STATUS = '3' AND CL.REG_NO IS NULL)");
			else
				sWhere2 += (_T(" (CL.STATUS = '3')"));
		}
		// ���� ��� üũ
		else if( i == 6 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '4')");
		}	
		// ���ιݳ��� üũ
		else if( i == 7 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '5')");
		}
		// ���� üũ �Ϸ� üũ
		else if ( 37==i )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '3' AND CL.REG_NO IS NOT NULL)");
		}
	}
		
	if( sWhere2.GetLength() > 1 ) sWhere2 = _T("( ") + sWhere2 + _T(") ");

	
	if( !sWhere2.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += sWhere2;
	}

	//if ( m_pLoan->m_bVolReserve )
	//{
	//	if( !sWhere.IsEmpty() && !m_sTimeCondition.IsEmpty() )
	//		sWhere = sWhere + _T(" AND ") + m_sTimeCondition;
	//}

	return 0;

	EFS_END
	return -1;

}
*/
INT CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )
{
	EFS_BEGIN

	INT ids;

	sWhere.Empty();
	
	CString sCMAlias = _T("CM_BO_LOC_3210");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetSqlBO_LOC_3210") );

	// �ڷ� ������ ���� ���� SQL���� ����
	CString sCMode[3] = { _T("����") , _T("���") , _T("����") };

	CString sMode;
	ids = pCM->GetControlMgrData( _T("�ڷᱸ��") , sMode , -1 , 0 );

	if( sMode.Compare(_T("��ü"))!=0 )
	{
		if( sMode.Compare( sCMode[nMode] ) != 0 )
			return 1;
	}

	// �˻� �������� ��尡 _T("�󼼰˻�") ����� ��츸 �˻� ���ǿ� �߰�
	CString sModeAlias;
	CESL_Control_Element *pCE = pCM->FindControlElement( _T("��������") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetSqlBO_LOC_3210") );
	(m_pParentMgr->GetDlgItem( pCE->CTRL_ID ))->GetWindowText( sModeAlias );

	// 2010.11.29 ADD BY KSJ : ��ü�� �߰�
	// 2007.05.25 UPDATE BY PDW(JR)
	// ��������� �߰�
	const INT nCtrlCnt = 50;
	CString sCtrlAlias[ nCtrlCnt ] =
	{
		_T("�����ڷ����_��ü")		, _T("�����ڷ����_����")		, _T("�����ڷ����_�ݳ�")		, _T("�����ڷ����_��ü")	, _T("�����ڷ����_�ݳ�����")	, // 4
		_T("�����ڷ����_����")		, _T("�����ڷ����_�������")	, _T("�����ڷ����_���ιݳ���")	, _T("��������")			, _T("��������")				, // 9
		_T("�ڷ�Ǹ�")				, _T("������1")					, _T("������2")					, _T("�ݳ�������1")			, _T("�ݳ�������2")				, // 14
		_T("�ݳ���1")				, _T("�ݳ���2")					, _T("������1")					, _T("������2")				, _T("���ุ����1")				, // 19
		_T("���ุ����2")			, _T("����Ҽ�����")			, _T("������������")			, _T("�����ڹ�ȣ1")			, _T("�����ڹ�ȣ2")				, // 24
		_T("������1")				, _T("������2")					, _T("��ǥ��")					, _T("������")				, _T("��ġ��ȣ1")				, // 29
		_T("��ġ��ȣ2")				, _T("�з���ȣ1")				, _T("�з���ȣ2")				, _T("��Ϲ�ȣ����")		, _T("��Ϲ�ȣ1")				, // 34
		_T("��Ϲ�ȣ2")				, _T("�ڵ���")					, _T("�����ڷ����_����Ϸ�")   , _T("���Ա���")            , _T("�谡��1")                 , // 39
		_T("�谡��2")               , _T("������1")                 , _T("������2")                 , _T("����1")				, _T("����2")                   , // 44
		_T("����")                  , _T("���������1")             , _T("���������2")				, _T("��ü��1")				, _T("��ü��2")					// 49
	};
	INT nBoFieldType[ nCtrlCnt ] = 
	{
		ETC			, ETC		, ETC			, ETC			, ETC		, 
		ETC			, ETC		, ETC			, ONESTRING		, ONESTRING	, 
		ONESTRING	, TWODATE	, ADDINFO		, TWODATE		, ADDINFO	,
		TWODATE		, ADDINFO	, TWODATE		, ADDINFO		, TWODATE	,
		ADDINFO		, ONESTRING	, ONESTRING		, TWOSTRING		, ADDINFO	,
		// 17/01/18 ������ : ����>����/�ݳ�>�����ڷ������ ��ġ��ȣ ���� �˻��� ����
/*//BEFORE-----------------------------------------------------------------------------
		TWOSTRING	, ADDINFO	, ONEINDEXSEARCH, ONEINDEXSEARCH, TWOSTRING	,
*///AFTER------------------------------------------------------------------------------
		TWOSTRING	, ADDINFO	, ONEINDEXSEARCH, ONEINDEXSEARCH, ONESTRING	, 
/*//END-------------------------------------------------------------------------------*/
		ADDINFO		, TWOSTRING	, ADDINFO		, ADDINFO	    , ADDINFO   ,
		ADDINFO		, ADDINFO	, ETC           , ONESTRING     , TWODATE   ,
		ADDINFO     , TWODATE   , ADDINFO       , ADDINFO		, ADDINFO	,
		ADDINFO     , ADDINFO   , ADDINFO		, ADDINFO		, ADDINFO		
	};
	INT nNonDBFieldType[ nCtrlCnt ] = 
	{
		ETC			, ETC		, ETC		, ETC			, ETC		, 
		ETC			, ETC		, ETC		, ONESTRING		, ONESTRING	, 
		ONESTRING	, TWODATE	, ADDINFO	, TWODATE		, ADDINFO	,
		TWODATE		, ADDINFO	, TWODATE	, ADDINFO		, TWODATE	,
		ADDINFO		, ONESTRING	, ONESTRING	, TWOSTRING		, ADDINFO	,
		// 17/01/18 ������ : ����>����/�ݳ�>�����ڷ������ ��ġ��ȣ ���� �˻��� ����
/*//BEFORE-----------------------------------------------------------------------------
		TWOSTRING	, ADDINFO	, ONESTRING	, ONESTRING		, TWOSTRING	,
*///AFTER------------------------------------------------------------------------------
		TWOSTRING	, ADDINFO	, ONESTRING	, ONESTRING		, ONESTRING	, 
/*//END-------------------------------------------------------------------------------*/
		ADDINFO		, ETC		, ETC		, ADDINFO       , ADDINFO  	,
		ADDINFO		, ADDINFO	, ETC       , ETC           , ETC       ,
		ETC         , ETC       , ETC       , ADDINFO		, ADDINFO	,
		ADDINFO     , ADDINFO   , ADDINFO	, ADDINFO		, ADDINFO
	};

	// 2006-03-02 UPDATE BY PDW(Jr) =============================================================
	// �����ڸ� �˻� ���� NAME >> IDX_USER_NAME ����
	// �ڷ�� CL.SHELF_LOC_CODE >> B.SHELF_LOC_CODE ����

	TCHAR *sBo_ECOSERARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("B.MANAGE_CODE") , 
		_T("B.SHELF_LOC_CODE") , _T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") ,
		_T("CL.RETURN_DATE") , _T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") ,
		_T("CL.RESERVATION_EXPIRE_DATE") , _T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") ,
		_T("CLU.IDX_USER_NAME") , _T("CLU.IDX_USER_NAME") , _T("I.BO_IDX_TITLE") , _T("I.BO_IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") ,
		_T("B.SEPARATE_SHELF_CODE") , _T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") ,
		_T("B.REG_NO"),_T("") , _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc") ,
		_T("CLU.USER_DEFINE_CODE1"), _T(""), _T(""), _T(""), _T("")
	};

	TCHAR *sBo_CATSEARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("B.MANAGE_CODE") , 
		_T("B.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.IDX_USER_NAME") , 
		_T("CLU.IDX_USER_NAME") , _T("I.IDX_TITLE") , _T("I.IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , 
		_T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc") ,_T("CLU.USER_DEFINE_CODE1"), _T(""), _T(""), _T(""), _T("")
	};

	TCHAR *sSe_ECOSEARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("B.MANAGE_CODE") , 
		_T("B.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.IDX_USER_NAME") , 
		_T("CLU.IDX_USER_NAME") , _T("I.SE_IDX_TITLE") , _T("I.SE_IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , 
		_T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc") ,_T("CLU.USER_DEFINE_CODE1"), _T(""), _T(""), _T(""), _T("")
	};

	TCHAR *sSe_CATSEARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("B.MANAGE_CODE") , 
		_T("B.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.IDX_USER_NAME") , 
		_T("CLU.IDX_USER_NAME") , _T("I.IDX_TITLE") , _T("I.IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , 
		_T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc") , _T("CLU.USER_DEFINE_CODE1"), _T(""), _T(""), _T(""), _T("")
	};

	
	TCHAR *sNonDBDBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("B.MANAGE_CODE") , 
		_T("CL.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.IDX_USER_NAME") , 
		_T("CLU.IDX_USER_NAME") , _T("B.TITLE") , _T("B.AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("etc") , _T("etc") , _T("B.REG_NO") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,_T("CLU.CLASS_NO") , _T("CLU.CLASS_NO") ,
		_T("CLU.USER_DEFINE_CODE1"), _T(""), _T(""), _T(""), _T("")
	};

	TCHAR **pDBFieldName;
	pDBFieldName = NULL;
	INT *pnDBFieldType;
	pnDBFieldType = NULL;

	if( nMode == 0 )
	{
		if( m_pParentMgr->m_pInfo->GetSearchIndexType() == 1 )
			pDBFieldName = sBo_ECOSERARCH_DBFieldName;
		else
			pDBFieldName = sBo_CATSEARCH_DBFieldName;
		pnDBFieldType = nBoFieldType;
	}
	else if( nMode == 1 )
	{
		pDBFieldName = sNonDBDBFieldName;
		pnDBFieldType = nNonDBFieldType;
	}
	else if( nMode == 2 )
	{
		if( m_pParentMgr->m_pInfo->GetSearchIndexType() == 1 )
			pDBFieldName = sSe_ECOSEARCH_DBFieldName;
		else
			pDBFieldName = sSe_CATSEARCH_DBFieldName;
		pnDBFieldType = nBoFieldType;
	}


	CString sGetData[ nCtrlCnt ];

	for( INT i = 0 ; i < nCtrlCnt ; i++ )
	{
		if( pnDBFieldType[i] == ETC ) continue;

		// 2006-01-12 ADD BY PDW(Jr) ======================================================
		// ����� ���� ����
		if ( m_sDefenceFlag != _T("Y") && ( i == 38 || i == 45 ) ) continue;
		// ================================================================================

		ids = pCM->GetControlMgrData( sCtrlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetSqlBO_LOC_3210") );
	}

	// ���� ����
	if( sGetData[8].Compare(_T("�������")) == 0 )
		sGetData[8] = _T("");
	else if( sGetData[8].Compare(_T("�Ϲ�")) == 0 )
		sGetData[8] = _T("0");
	else if( sGetData[8].Compare(_T("Ư��")) == 0 )
		sGetData[8] = _T("1");
	else if( sGetData[8].Compare(_T("����")) == 0 )
		sGetData[8] = _T("2");
	else if( sGetData[8].Compare(_T("����")) == 0 )
		sGetData[8] = _T("3");
	// 2005.09.27 ADD BY PDW(Jr) ++++++++++++++++++++
	// ��� ���� �߰�
	else if( sGetData[8].Compare(_T("���")) == 0 )
		sGetData[8] = _T("4");
	// ______________________________________________

	INT nCnt;
	if( sModeAlias.Compare( _T("�󼼰˻�") ) == 0 ) 
	{
		nCnt = 27;
	}
	else 
	{
		nCnt = nCtrlCnt;
	}
	ids = MakeWhereSql( nCnt , pnDBFieldType , pDBFieldName , sGetData , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetSqlBO_LOC_3210") );

	///{{++
	/// 2008.03.03 add by pdw
	/// ��Ϲ�ȣ ����� �����Ͽ���
	if( 0 != sModeAlias.Compare(_T("�󼼰˻�")) )
	{
		/// ��ϱ��� ������ �ʼ�
		if( !sGetData[33].IsEmpty() )
		{
			CString strRegNoAlias;

//DEL 			if( 0 == nMode )
//DEL 			{
//DEL 				strRegNoAlias = _T("B.REG_NO");
//DEL 			}
//DEL 			else if( 1 == nMode )
//DEL 			{
//DEL 				strRegNoAlias = _T("NB.REG_NO");
//DEL 			}
//DEL 			else if( 2 == nMode )
//DEL 			{
//DEL 				strRegNoAlias = _T("B.REG_NO");
//DEL 			}
			strRegNoAlias = _T("B.REG_NO");

			CESL_Control_Element* pElement = pCM->FindControlElement(_T("��Ϲ�ȣ���"));
			if( NULL == pElement ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("GetSqlBO_LOC_3210") );
			CButton* pCheck = (CButton*)m_pParentMgr->GetDlgItem(pElement->CTRL_ID);
			if( NULL == pCheck ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GetSqlBO_LOC_3210") );
			INT nCheck = pCheck->GetCheck();
			/// ���� �˻��̶��
			if( 0 == nCheck )
			{					
				if( !sGetData[34].IsEmpty() )
				{
					if( sGetData[35].IsEmpty() )
					{
						sGetData[35] = sGetData[34];
					}

					CString strRegNo1, strRegNo2;

					strRegNo1 = sGetData[33] + sGetData[34];
					strRegNo2 = sGetData[33] + sGetData[35];
						
					CLocCommonAPI::MakeRegNoSpaceToZero( strRegNo1 );
					CLocCommonAPI::MakeRegNoSpaceToZero( strRegNo2 );

					if( sWhere.GetLength() > 1 )
						sWhere += _T(" AND ");

					sWhere += ( strRegNoAlias + _T(" BETWEEN '") + strRegNo1 + _T("' AND '") + strRegNo2 + _T("'") );
				}
			}
			/// ���� �˻��̶��
			/// �Է°��� ","�� ���еǾ� �ִ�
			else if( 1 == nCheck )
			{
				CString strData;
				ids = pCM->GetControlMgrData( _T("��Ϲ�ȣȮ��"), strData, -1, 0 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("GetSqlBO_LOC_3210") );
				
				if( !strData.IsEmpty() )
				{
					CStringArray pArray;
					CLocCommonAPI::MakeArrayList(strData, _T(","), pArray);

					CString strRegNo;
					CString strRegNoWhere;
					INT     nArrayCnt = pArray.GetSize();					

					for( i=0; i<nArrayCnt; i++ )
					{
						strRegNo = sGetData[33] + pArray.GetAt(i);
						CLocCommonAPI::MakeRegNoSpaceToZero( strRegNo );

						if( !strRegNoWhere.IsEmpty() )
						{
							strRegNoWhere += _T(",");
						}

						strRegNoWhere += ( _T("'") + strRegNo + _T("'") );
					}
					
					if( !strRegNoWhere.IsEmpty() )
					{
						if( sWhere.GetLength() > 1 )
							sWhere += _T(" AND ");

						sWhere += ( strRegNoAlias + _T(" IN ( ") + strRegNoWhere + _T(" )") );
					}
				}
			}	
		}
	}
	///}}--

	// 2007.05.25 add by pdw(jr) {{++++++++++++++++++++++++++++++++++++++++++++++++
	// ��������� �߰�
	sGetData[46].TrimLeft();
	sGetData[46].TrimRight();
	if( !sGetData[46].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		sGetData[47].TrimLeft();
		sGetData[47].TrimRight();
		if( !sGetData[47].IsEmpty() )
		{
			//CString(_T(" BETWEEN to_date('")) + sGetData[i] + CString(_T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")) + sGetData[i+1] +_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS')");
			sWhere += ( _T("CL.RESERVE_CANCEL_DATE BETWEEN TO_DATE('") + sGetData[46] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[47] + _T("23:59:59','YYYY/MM/DD HH24:MI:SS')") );
		}
		else
		{
			sWhere += ( _T("CL.RESERVE_CANCEL_DATE BETWEEN TO_DATE('") + sGetData[46] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[46] + _T("23:59:59','YYYY/MM/DD HH24:MI:SS')") );
		}
	}
	// --------------------------------------------------------------------------}}

	// 2010.11.29 ADD BY KSJ : ��ü�� �߰�
	sGetData[48].TrimLeft();	sGetData[48].TrimRight();
	sGetData[49].TrimLeft();	sGetData[49].TrimRight();
	if(sGetData[48].IsEmpty() && !sGetData[49].IsEmpty())
	{
		sGetData[48] = _T("1");
	}
	if(!sGetData[48].IsEmpty())
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		CString strTemp;
		// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
/*//BEFORE-----------------------------------------------------------------------------		
		strTemp.Format(_T(" %s <= (TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD')-TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD'))"),sGetData[48]);
		sWhere += strTemp;

		if(!sGetData[49].IsEmpty())
		{
			strTemp.Format(_T(" AND %s >= (TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD')-TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD'))"),sGetData[49]);
			sWhere += strTemp;
		}
*///AFTER------------------------------------------------------------------------------
		CString strValue = _T("");
		m_pLoan->GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("��ü�Ⱓ�ް�����������"), strValue);
		if( !strValue.IsEmpty() && _T("Y") != strValue)		
		{
			strTemp.Format(_T(" %s <= (TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD')-TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD')-GET_HOLIDAYCOUNT(TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD'), TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD'), '%s'))"),sGetData[48], m_pParentMgr->m_pInfo->MANAGE_CODE);
			sWhere += strTemp;

			if(!sGetData[49].IsEmpty())
			{
				strTemp.Format(_T(" AND %s >= (TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD')-TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD')-GET_HOLIDAYCOUNT(TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD'), TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD'), '%s'))"),sGetData[49],m_pParentMgr->m_pInfo->MANAGE_CODE);
				sWhere += strTemp;
			}		
		}
		else
		{
			strTemp.Format(_T(" %s <= (TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD')-TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD'))"),sGetData[48]);
			sWhere += strTemp;

			if(!sGetData[49].IsEmpty())
			{
				strTemp.Format(_T(" AND %s >= (TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD')-TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD'))"),sGetData[49]);
				sWhere += strTemp;
			}
		}
/*//END-------------------------------------------------------------------------------*/
		
	}

	// New Ecosearch ����
	// �׽�Ʈ�� ���� �ӽ� ����
	// 27 , 28  
	FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("rb"));
	BOOL bNewEcoSearch = FALSE;
	if( fd == NULL )
		bNewEcoSearch = TRUE;
	else
		fclose(fd);
	
	CString sEcoSearchWhere;
	INT nEcoSearchCnt=0;
	if( nCnt > 27 && ( nMode == 0 || nMode == 2 ) && bNewEcoSearch && 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
	{
		//if( m_pParentMgr->m_pInfo->MODE == 10000 || ( m_pParentMgr->m_pInfo->MODE == 30000 ) )
		//{
		//}
		//else
		//{
			CMakeSearchData MakeSearchData(m_pParentMgr);	
			
			// 2005.02.19 ADD BY PDJ 
			// �ʱ�ȭ �ʿ�
			// ���� ������ TRUE�� �Ǿ���.
			BOOL bIsEqualSearch = FALSE;

			/// ���� �˻� , ������ �˻�
			// ���ô� ������ �˻� ����
			if( !sGetData[27].IsEmpty() )
			{
				if( sGetData[27].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;
				
				MakeSearchData.GetKeyWord(sGetData[27]);
				if( !bIsEqualSearch ) 
					sGetData[27] += _T("*");
				
				// 2007.07.12 update pdw
				// ������ ������ �ٸ���.
				if( nMode == 2 )
				{
					sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_TITLE, '") + sGetData[27] + _T("') ");
				}
				else
				{
					sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_TITLE, '") + sGetData[27] + _T("') ");
				}
				nEcoSearchCnt++;
			}
			
			if( !sGetData[28].IsEmpty() )
			{
				if( sGetData[28].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;
				
				MakeSearchData.GetKeyWord(sGetData[28]);
				if( !bIsEqualSearch ) 
					sGetData[28] += _T("*");
				
				if( !sEcoSearchWhere.IsEmpty() )
					sEcoSearchWhere += _T(" INTERSECT ");
				
				// 2007.07.12 update pdw
				// ������ ������ �ٸ���.
				if( nMode == 2 )
				{
					sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_AUTHOR, '") + sGetData[28] + _T("') ");
				}
				else
				{
					sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_AUTHOR, '") + sGetData[28] + _T("') ");
				}
				nEcoSearchCnt++;
			}
			
			if( !sEcoSearchWhere.IsEmpty() )
			{
				if( !sWhere.IsEmpty() )
					sWhere += _T(" AND ");
				if( nEcoSearchCnt == 1 )
					sWhere += _T("I.REC_KEY IN ") + sEcoSearchWhere ;
				else
					sWhere += _T("I.REC_KEY IN ( ") + sEcoSearchWhere +_T(")");
			}
		//}
	}
	
	
	// ���� �߰�
	// �ڵ��� ��ȣ �˻��߰�
	// IDX_HANDPHONE

	sGetData[ 36 ].TrimLeft();
	sGetData[ 36 ].TrimRight();
	sGetData[ 36 ].Replace( _T(" "), _T("") );
	sGetData[ 36 ].Replace( _T("-"), _T("") );
	if( !sGetData[ 36 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		sWhere += ( _T("CLU.IDX_HANDPHONE like '%") + sGetData[ 36 ] + _T("%'") );
	}

	/// 20040223 ������ ����
	/// �� �����ȣ , �ּ� �߰�
	CString sZipCode , sAddress;
	ids = pCM->GetControlMgrData( _T("�����ȣ") , sZipCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -201 , _T("GetSqlBO_LOC_3210") );
	sZipCode.TrimLeft(); sZipCode.TrimRight();

	ids = pCM->GetControlMgrData( _T("�ּ�") , sAddress , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -201 , _T("GetSqlBO_LOC_3210") );
	sAddress.TrimLeft(); sAddress.TrimRight();

	if( !sZipCode.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		sWhere += _T("CLU.H_ZIPCODE LIKE ('")+sZipCode+_T("%')");
	}

	if( !sAddress.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");
		
		sWhere += _T("CLU.H_ADDR1 LIKE ('%")+sAddress+_T("%')");
	}
	
	// 2005.11.11 ADD BY PDW(Jr) /////////////////////////////////////////////
	// ���� �߰�
	sGetData[ 43 ].TrimLeft();sGetData[ 43 ].TrimRight();
	if( !sGetData[ 43 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		CString sClass = _T("");
		sGetData[ 44 ].TrimLeft();sGetData[ 44 ].TrimRight();
		if( !sGetData[ 44 ].IsEmpty() ) 
			sClass = _T("CLU.CLASS_NO BETWEEN '") + sGetData[ 43 ] + _T("' AND '") + sGetData[ 44 ] + _T("'");
		else
			sClass = _T("CLU.CLASS_NO = '") + sGetData[ 43 ] + _T("'");
		
		sWhere += sClass;
	}
	// ���� �߰� ////////////////////////////////////////////////
	sGetData[ 45 ].TrimLeft();sGetData[ 45 ].TrimRight();
	if( !sGetData[ 45 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		CString sClass = _T("");
		sClass = _T("CLU.USER_DEFINE_CODE1 = '") + sGetData[ 45 ] + _T("'");
		
		sWhere += sClass;
	}

	// üũ �ڽ� ����
	INT nCheck;
	CESL_Control_Element *pCheckCE;

	CString sWhere2;

	// ���� �ݳ��� üũ
	CString sStation,sDoor;
//	pCM->GetControlMgrData( _T("���ιݳ���") , sStation , -1 , 0 );
//	pCM->GetControlMgrData( _T("���ιݳ���") , sDoor , -1 , 1 );
	
	CComboBox* pCombo = (CComboBox*)(pCM->FindControl(_T("���ιݳ���")));
	INT nSSIdx = pCombo->GetCurSel();

	if( nSSIdx != 0 )
	{
		CESL_DataMgr* pStationList = m_pParentMgr->FindDM(_T("DM_���ιݳ�����"));
		// �˻��� PC_NAME���� �ؾ��Ѵ�.
		pStationList->GetCellData( _T("�����̼�") , nSSIdx-1 , sStation );
		pStationList->GetCellData( _T("�����̸�") , nSSIdx-1 , sDoor );
	}

	sStation.TrimLeft(); sStation.TrimRight();
	sDoor.TrimLeft(); sDoor.TrimRight();
	
	if( !sStation.IsEmpty() )
	{
		if( !sWhere2.IsEmpty() ) sWhere2 += _T(" AND ");
		if( !sDoor.IsEmpty() )
			sWhere2 += _T(" CL.SELF_RETURN_STATION = '")+sStation+_T("' AND  CL.SELF_RETURN_DOOR = '")+sDoor+_T("' ");
		else
			sWhere2 += _T(" CL.SELF_RETURN_STATION = '")+sStation+_T("' ");
	}

	if( !sWhere2.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += sWhere2;
	}
	
	sWhere2.Empty();

	for( i = 0 ; i < 9 ; i++ )
	{
		if ( (m_pLoan->m_bVolReserve) && (9==(i+1)) )		i = 37;
		else if ( !(m_pLoan->m_bVolReserve) && (9==(i+1)) )	continue;

		pCheckCE= pCM->FindControlElement( sCtrlAlias[i] );
		if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetSqlBO_LOC_3210") );
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck != 1 ) continue;
		
		// ��ü üũ
		if( i == 0 )	return 0;
		// ���� üũ
		else if( i == 1 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS IN ('0', 'L'))");
//DEL 			sWhere2 += _T(" (CL.STATUS = '0')");
		}
		// �ݳ� üũ
		else if( i == 2 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS IN ('1', 'R'))");
//DEL 			sWhere2 += _T(" (CL.STATUS = '1')");
		}
		// ��ü üũ
		else if( i == 3 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");

			//++2008.09.02 DEL BY LKS {{++
//REMOVE�� 			if( m_pParentMgr->m_pInfo->MODE == 10000 || (m_pParentMgr->m_pInfo->MODE==30000) )	
//REMOVE�� 				sWhere2 += _T(" ( CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < to_char(now(),'YYYY/MM/DD') ) ");
			//--2008.09.02 DEL BY LKS --}}
//			else

			// 18.09.27 JMJ KOL.RED.2018.008
			//��ü���� �ڷ� �˻� �� [å�η� �ݳ�����] ���� �߰�
			sWhere2 += _T(" ( CL.STATUS IN ('0','2', 'L','D') AND CL.RETURN_PLAN_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') ) ");
			//sWhere2 += _T(" ( CL.STATUS IN ('0','2', 'L','D') AND CL.RETURN_PLAN_DATE < TO_DATE(SYSDATE,'YYYY/MM/DD') ) ");
//			// 2009.12.15 UPDATE BY PWR : TO_CHAR -> TO_DATE �ӵ�����
//			sWhere2 += _T(" ( CL.STATUS IN ('0','2', 'L') AND CL.RETURN_PLAN_DATE < TO_DATE(SYSDATE,'YYYY/MM/DD') ) ");
// 			sWhere2 += _T(" ( CL.STATUS IN ('0','2', 'L') AND CL.RETURN_PLAN_DATE < to_char(SYSDATE,'YYYY/MM/DD') ) ");
//DEL 				sWhere2 += _T(" ( CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < to_char(SYSDATE,'YYYY/MM/DD') ) ");
		}
		// �ݳ� ���� üũ
		else if( i == 4 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			// 18.09.27 JMJ KOL.RED.2018.008
			sWhere2 += _T(" (CL.STATUS IN ('2', 'D'))");
			//sWhere2 += _T(" (CL.STATUS = '2')");
		}
		// ���� üũ or ���� üũ ������
		else if( i == 5 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			//++2008.06.04 UPDATE BY PWR {{++
			// ���ο������ý��� ���� ���°� ���еǵ��� �˻������� ����
			if( m_pLoan->m_sUnmannedReserveSys == _T("Y") )
			{
				if ( m_pLoan->m_bVolReserve )
					sWhere2 += _T(" (CL.STATUS='3' AND CL.REG_NO IS NULL AND CL.UNMANNED_RESERVATION_LOAN IS NULL)");
				else
					sWhere2 += _T(" (CL.STATUS='3' AND CL.UNMANNED_RESERVATION_LOAN IS NULL)");
			}
			else
			{
				if ( m_pLoan->m_bVolReserve )
					sWhere2 += _T(" (CL.STATUS='3' AND CL.REG_NO IS NULL)");
				else
					sWhere2 += _T(" (CL.STATUS='3')");
			}
			/*
			if ( m_pLoan->m_bVolReserve )
				sWhere2 += _T(" (CL.STATUS = '3' AND CL.REG_NO IS NULL)");
			else
				sWhere2 += (_T(" (CL.STATUS = '3')"));
			*/
			//--2008.06.04 UPDATE BY PWR --}}
		}
		// ���� ��� üũ
		else if( i == 6 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '4')");
		}	
		// ���ιݳ��� üũ
		else if( i == 7 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '5')");
		}
		// ���� üũ �Ϸ� üũ
		else if ( 37==i )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '3' AND CL.REG_NO IS NOT NULL)");
		}
	}

	//++2008.06.04 UPDATE BY PWR {{++
	// �а��ݳ��ڷ�	
	pCheckCE = pCM->FindControlElement( _T("�����ڷ����_�а��ݳ���") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );

	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS='6')");
		}
	}
	// �߰����⿹��
	pCheckCE = pCM->FindControlElement( _T("�����ڷ����_�߰����⿹��") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );

	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" AND ");
			sWhere2 += _T(" CL.NIGHT_RESERVATION_LOAN IN ('Y','O')");
		}
	}	
	// Ÿ���ݳ��ڷ�
	pCheckCE = pCM->FindControlElement( _T("�����ڷ����_Ÿ���ݳ���") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS='7')");
		}
	}
	// ���δ��⿹��
	pCheckCE = pCM->FindControlElement( _T("�����ڷ����_���δ��⿹��") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");		
			sWhere2 += _T(" (CL.UNMANNED_RESERVATION_LOAN IN ('Y','O') AND CL.STATUS='3')");
			
			if( sWhere2.GetLength() > 1 ) sWhere2 = _T("( ") + sWhere2 + _T(") ");
	
			if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
			sWhere += sWhere2;

			return 0;
		}
	}
	// �߰����⿹���û
	pCheckCE = pCM->FindControlElement( _T("�߰��������_��û") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" AND ");
			sWhere2 = sWhere2 + _T(" CL.NIGHT_RESERVATION_LOAN = 'Y'");		

			if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
			sWhere += _T(" CL.STATUS='3'");
		}
	}
	// �߰����⿹������
	pCheckCE = pCM->FindControlElement( _T("�߰��������_����") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" AND ");
			sWhere2 = sWhere2 + _T(" CL.NIGHT_RESERVATION_LOAN = 'O'");		

			if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
			sWhere += _T(" CL.STATUS='3'");
		}
	}
	// ���δ��⿹���û
	pCheckCE = pCM->FindControlElement( _T("���δ������_��û") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 = sWhere2 + _T(" (CL.UNMANNED_RESERVATION_LOAN = 'Y' AND CL.STATUS='3')");
		}
	}
	// ���δ����� ( ���°��� 'O'�̸鼭 ���ุ������ ������� ���� �ڷ� )
	pCheckCE = pCM->FindControlElement( _T("���δ������_���") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			// 2009.12.15 UPDATE BY PWR : TO_CHAR -> TO_DATE �ӵ�����
			sWhere2 += _T(" (CL.UNMANNED_RESERVATION_LOAN='O' AND CL.RESERVATION_EXPIRE_DATE >= TO_DATE(SYSDATE,'YYYY/MM/DD') AND CL.STATUS='3')");
// 			sWhere2 += _T(" (CL.UNMANNED_RESERVATION_LOAN='O' AND CL.RESERVATION_EXPIRE_DATE >= TO_CHAR(SYSDATE,'YYYY/MM/DD') AND CL.STATUS='3')");
		}
	}
	// ���δ������� ( ���°��� 'O'�̸鼭 ���ุ������ ����� �ڷ� )
	pCheckCE = pCM->FindControlElement( _T("���δ������_����") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			// 2009.12.15 UPDATE BY PWR : TO_CHAR -> TO_DATE �ӵ�����
			sWhere2 = sWhere2 + _T(" (CL.UNMANNED_RESERVATION_LOAN='O' AND CL.RESERVATION_EXPIRE_DATE < TO_DATE(SYSDATE,'YYYY/MM/DD') AND CL.STATUS='3')");	
// 			sWhere2 = sWhere2 + _T(" (CL.UNMANNED_RESERVATION_LOAN='O' AND CL.RESERVATION_EXPIRE_DATE < TO_CHAR(SYSDATE,'YYYY/MM/DD') AND CL.STATUS='3')");	
		}
	}
	/*
	pCheckCE= pCM->FindControlElement( _T("�����ڷ����_�߰����⿹��") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
	nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
	if( nCheck )
	{
		if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" AND ");		
			sWhere2 += _T(" CL.NIGHT_RESERVATION_LOAN IN ('Y','O') ");
	}

	pCheckCE= pCM->FindControlElement( _T("�����ڷ����_Ÿ���ݳ���") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
	nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
	if( nCheck )
	{
		if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '6')");
	}

	BOOL bCheck2;
	CString sWhere3;
	pCheckCE= pCM->FindControlElement( _T("�߰��������_��û") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
	nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();

	pCheckCE= pCM->FindControlElement( _T("�߰��������_����") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
	bCheck2 = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();

	if ( nCheck )
	{
		if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" AND ");		
		sWhere2 = sWhere2 + _T(" CL.NIGHT_RESERVATION_LOAN = 'Y' ");		

		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" CL.STATUS='3' ");

	}
    else if ( bCheck2 )
	{
		if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" AND ");
		sWhere2 = sWhere2 + _T(" CL.NIGHT_RESERVATION_LOAN = 'O' ");		

		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" CL.STATUS='3' ");
	}	
	*/
	//--2008.06.04 UPDATE BY PWR --}}
	
	if( sWhere2.GetLength() > 1 ) sWhere2 = _T("( ") + sWhere2 + _T(") ");

	
	if( !sWhere2.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += sWhere2;
	}

	//if ( m_pLoan->m_bVolReserve )
	//{
	//	if( !sWhere.IsEmpty() && !m_sTimeCondition.IsEmpty() )
	//		sWhere = sWhere + _T(" AND ") + m_sTimeCondition;
	//}

	return 0;

	EFS_END
	return -1;

}
// TOOL 
// WHERE�� ����
// [out]sWhere
// [in] nCnt -> ó���� ����
// [in] nFieldType -> TYPE
// [in] sDBFieldAlias -> DB Field Name 
// [in] sGetData -> ����
INT CBL_LOC_LOAN_MNG_PROC::MakeWhereSql( INT nCnt , INT nFieldType[] , TCHAR **sDBFieldName , CString sGetData[] , CString &sWhere )
{
	EFS_BEGIN

	CLocCommonAPI::MakeSql( sGetData , sDBFieldName , nFieldType , nCnt , sWhere ,m_pParentMgr);
/*
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		sGetData[i].TrimLeft(); sGetData[i].TrimRight();

		switch( nFieldType[i] )
		{
		case ONENUMERIC :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				sWhere += sDBFieldName[i] + CString(_T(" = ")) + sGetData[i] + CString(_T(" "));
			}
			break;
		case ONESTRING :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				sWhere += sDBFieldName[i] + CString(_T(" = '")) + sGetData[i] + CString(_T("' "));
			}
			break;
		case TWOSTRING :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				if( sGetData[i+1].IsEmpty() )
				{
					sWhere += sDBFieldName[i] + CString(_T(" = '")) + sGetData[i] + CString(_T("' "));
				}
				else
				{
					sWhere += CString(_T("( ")) + sDBFieldName[i] + CString(_T(" BETWEEN '")) + sGetData[i] + CString(_T("' AND '")) + sGetData[i+1] + CString(_T("' ) "));
				}
				i++;
			}
			break;
		case ONEDATE:
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				sWhere += CString(_T(" to_char(")) + sDBFieldName[i]+ CString(_T(",'YYYY/MM/DD')")) + CString(_T(" = '")) + sGetData[i] + CString(_T("' "));
			}
			break;
		case TWODATE:
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				if( sGetData[i+1].IsEmpty() )
				{
					sWhere += CString(_T(" to_char(")) +sDBFieldName[i] + CString(_T(",'YYYY/MM/DD') = '")) + sGetData[i] + CString(_T("' "));
				}
				else
				{
					sWhere += CString(_T("( to_char(")) + sDBFieldName[i] + CString(_T(",'YYYY/MM/DD') BETWEEN '")) + sGetData[i] + CString(_T("' AND '")) + sGetData[i+1] + CString(_T("' ) "));
				}
				i++;
			}
			break;
		case THREESTRING :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				if( !sGetData[i+1].IsEmpty() )
				{
					// ��� ��ȣ�� ��쿡�� Ư���� ���� �Ѵ�.
					CString sData1 = sGetData[i]+sGetData[i+1];
					CString sData2 = sGetData[i]+sGetData[i+2];
					if( CString(_T("B.REG_CODE")).Compare(sDBFieldName[i])==0 || (CString(_T("NB.REG_NO")).Compare(sDBFieldName[i])==0))
					{
						CLocCommonAPI::MakeRegNoSpaceToZero( sData1);
						if( !sGetData[i+2].IsEmpty() )
							CLocCommonAPI::MakeRegNoSpaceToZero( sData2);
					}
					if( sGetData[i+2].IsEmpty() )
					{
						sWhere += sDBFieldName[i+1] + CString(_T(" = '")) + sData1 + CString(_T("' "));
					}
					else
					{
						sWhere += CString(_T("( ")) + sDBFieldName[i+1] + CString(_T(" BETWEEN '")) + sData1 +CString(_T("' AND '")) + sData2 + _T("' ) ");
					}
				}
				else
				{
					if( CString(_T("B.REG_CODE")).Compare(sDBFieldName[i])==0 )
						sWhere += sDBFieldName[i] + CString(_T(" = '")) + sGetData[i] + CString(_T("' "));
					else if( CString(_T("NB.REG_NO")).Compare(sDBFieldName[i])==0 )
						sWhere += sDBFieldName[i] + CString(_T(" LIKE ('")) + sGetData[i] + CString(_T("%') "));
					else
						sWhere += sDBFieldName[i] + CString(_T(" LIKE ('")) + sGetData[i] + CString(_T("%') "));

					
				}
				i = i+2;
			}
			break;
		case ONEINDEXSEARCH :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				sWhere += CString(_T(" ( CATSEARCH("))+sDBFieldName[i] +CString(_T(",'")) + sGetData[i] + CString(_T("',NULL) > 0 ) "));
			}
			break;
		default:
			break;
		}
	}
*/
	return 0;

	EFS_END
	return -1;

}


// �׸����� ȭ���� �����ش�.
INT CBL_LOC_LOAN_MNG_PROC::GridDisplay( CString sCMAlias )
{
	EFS_BEGIN

	INT ids;

	// ȸ������ , ���� ���� , ���� �߼ۿ��� , û����ȣ�� �����ش�.
	// �ϴ��� ���ุ �Ѵ�. , ��޴��⸸ �Ѵ�.

/*
	const INT nDMCnt = 3;

	CString sDMAlias[nDMCnt] =
	{
		_T("DM_BO_LOC_3200_BO") , _T("DM_BO_LOC_3200_NON_DB") , _T("DM_BO_LOC_3200_SE") 
	};

*/
	DWORD nSTART,nFINISH;
	nSTART = GetTickCount();

	const INT nDMCnt = 1;
	
	const INT nRCnt = 8;
	CString sReserveAlias[nRCnt] =
	{
		_T("IBS_û��_������ȣ"),		_T("IBS_û��_�з���ȣ"), _T("IBS_û��_������ȣ"), 
		_T("IBS_û��_��ġ��ȣ_�ڵ�"),	_T("IBS_û��_��å��ȣ"), _T("�谡����"), 
		_T("�����ڷ����"),				_T("��Ϲ�ȣ")			
	};

	CString sDMAlias[nDMCnt] =
	{
		_T("DM_BO_LOC_3200")
	};

	CString sMemberClass;
	CString sLoanStatus;
	CString sMailSendDate;
	CString sWorkingStatus;
	CString sRegNo;
	CString strReservationExpireDate;
	CString strReserveCancelDate;
	// 2008.06.04 ADD BY PWR : ���δ��⿹����°�
	// ("Y":���δ����û, "O, ���ุ���ϰ���ȵ�":���δ�����, "O, ���ุ���ϰ��":���δ�������)
	CString sUnmannedReserveStatus;

	CESL_DataMgr *pDM;

	// 2009.10.30 ADD BY KSJ : �����ڹ�ȣ�� �ֹι�ȣ�� ��� ó��
	CString strValue1;
	ids = m_pLoan->GetManageValue( _T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), strValue1 );
	strValue1.TrimLeft();		strValue1.TrimRight();		strValue1.MakeUpper();

	// 2010.08.27 ADD BY PWR : �ӵ�����
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
	CTime tCurrent(tTime.GetYear(), tTime.GetMonth(), tTime.GetDay(), tTime.GetHour(), 0, 0);

	INT nCnt = 1;
	INT nRowCount;
	for( INT j = 0 ; j < nCnt ; j++ )
	{
		pDM = m_pParentMgr->FindDM( sDMAlias[j] );
		nRowCount = pDM->GetRowCount();
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("GridDisplay") );

		for( INT i = 0 ; i < nRowCount ; i++ )
		{
			// 2009.10.30 ADD BY KSJ : �����ڹ�ȣ�� �ֹι�ȣ�� ��� ó��
			CString strUserNo , strBuf , strCivilNo;
			if( _T("Y") != strValue1 )
			{
				ids = pDM->GetCellData( _T("�����ڹ�ȣ") , i , strUserNo );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridDisplay") );

				// 2010.09.08 ADD BY KSJ : ���յ������� ��� �� ��� ǥ�ù�� ����
//DEL 				strBuf = strUserNo;
//DEL 				if( 13 <= strUserNo.GetLength() )
//DEL 				{
//DEL 					strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
//DEL 				}
//DEL 				
//DEL 				ids = CLocCommonAPI::CivilNoCheck( strBuf );
//DEL 				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("GridDisplay") );
//DEL 				if( 0 == ids )
//DEL 				{
//DEL 					//ids = CLocCommonAPI::ChangeSecuriyCivilNo( NULL, sUserNo );
//DEL 					strUserNo.Format( _T("%s%s******") , strBuf.Left(6) , strBuf.Mid(7,1) );
//DEL 					if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1040 , _T("GridDisplay") );
//DEL 
//DEL 					ids = pDM->SetCellData( _T("�����ڹ�ȣ") , strUserNo , i );
//DEL 					if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridDisplay") );
//DEL 				}
				CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pLoan->m_sIsUnityLoanService );
				ids = pDM->SetCellData( _T("�����ڹ�ȣ") , strUserNo , i );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridDisplay") );
			}

			// 2010.01.12 ADD BY PWR : �ֹε�Ϲ�ȣ *ǥ��
			ids = pDM->GetCellData( _T("�ֹε�Ϲ�ȣ") , i , strCivilNo );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridDisplay") );
			// 2010.09.08 ADD BY KSJ : ���յ������� ��� �� ��� ǥ�ù�� ����
//DEL 			strUserNo.Replace(_T("-"), _T(""));
//DEL 			strBuf = strUserNo;
//DEL 			if( 13 <= strUserNo.GetLength() )
//DEL 			{
//DEL 				strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
//DEL 			}
//DEL 			ids = CLocCommonAPI::CivilNoCheck( strBuf );
//DEL 			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("GridDisplay") );
//DEL 			if( 0 == ids )
//DEL 			{
//DEL 				strUserNo.Format( _T("%s-%s******") , strBuf.Left(6) , strBuf.Mid(7,1) );
//DEL 				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1040 , _T("GridDisplay") );
//DEL 				ids = pDM->SetCellData( _T("�ֹε�Ϲ�ȣ") , strUserNo , i );
//DEL 				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridDisplay") );
//DEL 			}
			ids = CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strCivilNo , m_pLoan->m_sIsUnityLoanService );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("GridDisplay") );
			ids = pDM->SetCellData( _T("�ֹε�Ϲ�ȣ") , strCivilNo , i );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridDisplay") );

			// ȸ������
			ids = pDM->GetCellData( _T("����") , i , sMemberClass );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1050 , _T("GridDisplay") );
			
			ids = ConvertMemberClassCodeToDesc( 0 , sMemberClass );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1060 , _T("GridDisplay") ); 

			ids = pDM->SetCellData( _T("����") , sMemberClass , i);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1070 , _T("GridDisplay") ); 

			// �������
			ids = pDM->GetCellData( _T("�������") , i , sLoanStatus );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1080 , _T("GridDisplay") );
			
			// 2009.02.16 ADD BY PWR : ���ټ��� ������¿� ��Ϲ�ȣ�� å�� ��Ϲ�ȣ�� �ǹ̾��� �������η� ����
			CString sReserveMark;
			ids = pDM->GetCellData( _T("��������"), i, sReserveMark );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1090 , _T("GridDisplay") );
			// ���ټ��� ���� �������̸� �ʿ���³��� �����
			if( (m_pLoan->m_bVolReserve) && (0==sLoanStatus.Compare(_T("3"))) )
			{				
				// 2009.02.16 UPDATE BY PWR : ���ټ��� ������¿� ��Ϲ�ȣ�� å�� ��Ϲ�ȣ�� �ǹ̾��� �������η� ����
				//if( sRegNo.IsEmpty() )
				if(sReserveMark == _T("X"))
				{
					for( INT k = 0 ; k < nRCnt ; k++ )
					{
						ids = pDM->SetCellData( sReserveAlias[k] , _T("") , i);
						if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("GridDisplay") ); 
					}
				}
			}
			
			ids = pDM->GetCellData( _T("��Ϲ�ȣ") , i , sRegNo );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GridDisplay") );

			ids = pDM->GetCellData( _T("���ุ����") , i , strReservationExpireDate );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GridDisplay") );

			ids = pDM->GetCellData( _T("���������") , i , strReserveCancelDate );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GridDisplay") );
			
			// 2008.06.04 ADD BY PWR : ���δ��⿹�� ������°� �����ؼ� �����ֱ�
			CString sWhere, sValue, sRecKey;
			if( m_pLoan->m_sUnmannedReserveSys == _T("Y") )
			{
				m_nUnmannedLoanStatus = 0;
				ids = pDM->GetCellData( _T("����/�ݳ�����KEY"), i, sRecKey );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GridDisplay") );

				ids = pDM->GetCellData( _T("���δ��⿹��"), i, sUnmannedReserveStatus );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GridDisplay") );

				if( sUnmannedReserveStatus == _T("Y") )
				{
					m_nUnmannedLoanStatus = 1;//���δ��⿹���û
				}
				else if( sUnmannedReserveStatus == _T("O") )
				{
					//���ุ������ ��� �ϸ� ��������
					// 2011.09.19 UPDATE BY PWR : ���ο����ڷ��ε� ���ุ���� ���� �ڷ��϶� �������� ���� ����
					if(13 <= strReservationExpireDate.GetLength())
					{
						CTime tExpireDate( _ttoi(strReservationExpireDate.Mid(0,4)), _ttoi(strReservationExpireDate.Mid(5,2)), _ttoi(strReservationExpireDate.Mid(8,2)),_ttoi(strReservationExpireDate.Mid(11,2)),0,0 ); 
						// 2010.08.27 DEL BY PWR : �ӵ�����
	// 					CTime tCurrent = CLocCommonAPI::GetDBTime(m_pParentMgr);
	// 					tCurrent = CTime(tCurrent.GetYear(), tCurrent.GetMonth(), tCurrent.GetDay(), tCurrent.GetHour(), 0, 0 );
						if( tExpireDate < tCurrent )
							m_nUnmannedLoanStatus = 3;//���δ�������
						else
							m_nUnmannedLoanStatus = 2;//���δ�����
					}
				}
			}
			
			// 2008.04.16 UPDATE BY PDW : �������Ÿ�� �ʵ尡 �߰��Ǹ鼭 ���°��� �ο��ִ� ���� ����Ǿ���
			if( _T("4") == sLoanStatus || 0 < sLoanStatus.Find(_T("�������")) )
			{
				CString strReturnCancelType;
				ids = pDM->GetCellData( _T("�������Ÿ��"), i, strReturnCancelType );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , _T("GridDisplay") );

				if( strReturnCancelType.IsEmpty() )
				{
					ids = ConvertLoanStatusCodeToDesc(strReservationExpireDate, strReserveCancelDate, sLoanStatus );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7, _T("GridDisplay") ); 
				}
				else
				{
					/*
					0	�������(����)
					1	�������(�缭)
					2	�������(�̿���)
					3	�������(����)
					*/
					if(strReturnCancelType == _T("0"))
						sLoanStatus = _T("�������(����)");
					else if(strReturnCancelType == _T("1"))
						sLoanStatus = _T("�������(�缭)");
					else if(strReturnCancelType == _T("2"))
						sLoanStatus = _T("�������(�̿���)");
					else if(strReturnCancelType == _T("3"))
						sLoanStatus = _T("�������(����)");
					else
						sLoanStatus = _T("�������");
					/*
					const int nTypeCnt = 5;
					CString strstrReturnCancelTypeDesc[nTypeCnt][2] = {
						{_T("0"),_T("�������(����)")},   {_T("1"),_T("�������(�缭)")},
						{_T("2"),_T("�������(�̿���)")}, {_T("3"),_T("�������(����)")},
						{_T(""),_T("�������")}
					};

					for( int k=0;k<nTypeCnt;k++ )
					{
						if( 0 == strstrReturnCancelTypeDesc[k][0].Compare(strReturnCancelType) )
						{
							sLoanStatus = strstrReturnCancelTypeDesc[k][1];
							break;
						}
					}
					*/
				}
			}
			else
			{
				ids = ConvertLoanStatusCodeToDesc( 0, sRegNo, sLoanStatus );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7, _T("GridDisplay") ); 
			}

			ids = pDM->SetCellData( _T("�������") , sLoanStatus , i);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("GridDisplay") ); 

			// û����ȣ ����
			ids = MakeShelfCode( sDMAlias[j] , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("GridDisplay") ); 

			// �谡 ����
			if( j == 0 )
			{
				ids = pDM->GetCellData( _T("�谡����") , i , sWorkingStatus );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("GridDisplay") ); 

				CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

				ids = pDM->SetCellData( _T("�谡����") , sWorkingStatus , i );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("GridDisplay") ); 

			}

			/*
			///@20071105 PDW
			///@{{++
			///@K3 �䱸���� �����۾�
			///@�����ڷ���� �˻������ �Ѽ����� �߰��Ѵ�.
			///@�Ѽ����� ��ũ�����Ϳ��� �����ؾ� �Ѵ�.
			CString strMarc;

			ids = pDM->GetCellData(_T("MARC"), i , strMarc);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CBL_LOC_LOAN_MNG_PROC::GridDisplay( sCMAlias )") ); 

			if( 0 < strMarc.GetLength() )
			{
				CString strTotalTitle;
				ids = pDM->GetMarcTagData( strMarc, _T("440$a"), strTotalTitle );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("CBL_LOC_LOAN_MNG_PROC::GridDisplay( sCMAlias )") ); 

				ids = pDM->SetCellData( _T("�Ѽ���"), strTotalTitle, i );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("CBL_LOC_LOAN_MNG_PROC::GridDisplay( sCMAlias )") ); 
			}
			///@--}}
			*/
		}

	}
	ids = m_pParentMgr->AllControlDisplay( sCMAlias);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GridDisplay") );


	nFINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - ���� �ڷ� ���� �ڷ� ã��(�׸��� ���÷���) �ɸ� �ð� :  [%d.%d sec]\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	pDM->QueryLog(strMsg.GetBuffer(0));
	
	return 0;

	EFS_END
	return -1;

}

// 2006.12.06 ADD BY PDJ
/// ������¸� ����� ��ҿ� ���� �ڵ���ҷ� �����ϱ� ���Ѱ�
INT CBL_LOC_LOAN_MNG_PROC::ConvertLoanStatusCodeToDesc(CString strReservationExpireDate, CString strReserveCancelDate, CString &sLoanStatus )
{
	// ���� ������ ���� ������� �� ũ�� �ڵ���ҵȰ���
	// CASE 1. [������] ����ݳ� ȭ�鿡�� ��ҵ� ���
	// CASE 2. Cron�� ����ؼ� ��ġ�� ������� 
	//         �� 12�� ������ ������ ��� ����.?? 

	if ( strReserveCancelDate == _T("") ) 
	{
		// 2009.01.07 ADD BY PWR (����Ͼ��� ����̷� 4�� ���̴����󶧹���)
		sLoanStatus = _T("�������(����Ͼ���)");
		return 0;
	}

	// 2007.04.11 add by pdw {{+++++++++++++++++++++++++++++++++++++++++++++++++	
	if( strReservationExpireDate.IsEmpty() )
	{
		sLoanStatus = _T("�������(�̿���)");
	}
	else
	{
		if ( strReserveCancelDate <= strReservationExpireDate )
		{
			sLoanStatus = _T("�������(�̿���)");
		}
		else
		{
			sLoanStatus = _T("�������(����)");
		}
	}
	// --------------------------------------------------------------------------}}
	
	
	return 0;
}

// ��������� �ڵ带 �������� �Ǵ� ������ �ڵ�� �����ϴ� �Լ�
// nMode 0 : �ڵ�->����
// nMode 1 : ����->�ڵ�
INT CBL_LOC_LOAN_MNG_PROC::ConvertLoanStatusCodeToDesc( INT nMode, CString &sRegNo, CString &sCode )
{
	EFS_BEGIN
	
	// 2007.02.22 UPDATE BY PDW
	// �а��ݳ��ڷ� �߰�
	// 2008.06.04 UPDATE BY PWR
	// Ÿ���ݳ�, ���δ��⿹�� �߰�
	// 2008.12.15 UPDATE BY CJY
	// å�η�����, å�η��ݳ�, å�η��ݳ����� �߰�
	const INT nCnt = 17;
	TCHAR *sLoanStatus[nCnt][2] = 
	{
		{_T("0"),_T("����")}				,{_T("1"),_T("�ݳ�")}				,{_T("2"),_T("�ݳ�����")}, 
		{_T("3"),_T("����(������)")}		,{_T("3"),_T("����(�����Ϸ�)")}		,{_T("3"),_T("����(���δ����û)")},
		{_T("3"),_T("����(���δ�����)")}	,{_T("3"),_T("����(���δ�������)")}	,{_T("4"),_T("�������")}, 
		{_T("4"),_T("�������(�̿���)")}	,{_T("4"),_T("�������(����)")}		,{_T("5"),_T("�ݳ�(���ιݳ���)")},
		{_T("6"),_T("�ݳ�(�а��ݳ���)")}	,{_T("7"),_T("�ݳ�(Ÿ���ݳ���)")}	,{_T("L"),_T("å�η�����")}	,
		{_T("R"),_T("å�η��ݳ�")}			,{_T("D"),_T("å�η��ݳ�����")}
		/*
		{_T("0"),_T("����")}, {_T("1"),_T("�ݳ�")}, {_T("2"),_T("�ݳ�����")}, 
		{_T("3"),_T("����(������)")}, {_T("3"), _T("����(�����Ϸ�)")}, {_T("4"),_T("�������")}, 
		{_T("4"),_T("�������(�̿���)")}, {_T("4"),_T("�������(����)")}, {_T("5"),_T("�ݳ�(���ιݳ���)")},
		{_T("6"),_T("�ݳ�(�а����ݳ���)")}
		*/
	};

	INT nCompareIndex = 0;
	INT nReturnIndex = 1;
	if( nMode == 1 )
	{
		nCompareIndex = 1;
		nReturnIndex = 0;
	}

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sCode.Compare( sLoanStatus[i][nCompareIndex] ) != 0 ) continue;

		if(i == 3)//����
		{
			if(m_nUnmannedLoanStatus==1)
			{
				if( nReturnIndex == 0 )
					sCode = "3";
				else
					sCode = "����(���δ����û)";

				m_nUnmannedLoanStatus = 0;
			}
			else if(m_nUnmannedLoanStatus==2)
			{
				if( nReturnIndex == 0 )
					sCode = "3";
				else
					sCode = "����(���δ�����)";

				m_nUnmannedLoanStatus = 0;
			}
			else if(m_nUnmannedLoanStatus==3)
			{
				if( nReturnIndex == 0 )
					sCode = "3";
				else
					sCode = "����(���δ�������)";

				m_nUnmannedLoanStatus = 0;
			}
			else
			{
				if(m_pLoan->m_bVolReserve)
				{ // ���ټ��⿹��
					if( sRegNo.IsEmpty() )
					{
						if( nReturnIndex == 0 )
							sCode = "3";
						else
							sCode = "����(������)";
					}
					else
					{
						if( nReturnIndex == 0 )
							sCode = "3";
						else
							sCode = "����(�����Ϸ�)";
					}
				}
				else
				{ // �Ϲݿ���
					if( nReturnIndex == 0 )
						sCode = "3";
					else
						sCode = "����";
				}
			}
		}
		else
		{
			sCode = sLoanStatus[i][nReturnIndex];
		}
	}

	return 0;

	EFS_END
	return -1;

}

// ȸ�������� �ڵ带 �������� �Ǵ� ������ �ڵ�� �����ϴ� �Լ�
// nMode 0 : �ڵ�->����
// nMode 1 : ����->�ڵ�
INT CBL_LOC_LOAN_MNG_PROC::ConvertMemberClassCodeToDesc( INT nMode , CString &sMemberClass )
{
	EFS_BEGIN

	const INT nCnt = 2;
	TCHAR *sMemberClassCode[nCnt][2] = 
	{
		{_T("0"),_T("��ȸ��")} , {_T("1"),_T("��ȸ��")} 
	};

	INT nCompareIndex = 0;
	INT nReturnIndex = 1;
	if( nMode == 1 )
	{
		nCompareIndex = 1;
		nReturnIndex = 0;
	}

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sMemberClass.Compare( sMemberClassCode[i][nCompareIndex] ) != 0 ) continue;

		sMemberClass = sMemberClassCode[i][nReturnIndex];
	}

	return 0;

	EFS_END
	return -1;

}

// ȸ�� ������ �����ش�.
INT CBL_LOC_LOAN_MNG_PROC::ViewMemberInfo()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias;
	ids = GetParentDM(sDMAlias);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewMemberInfo") );
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewMemberInfo") );

	if( pDM->GetRowCount() < 1 ) return 0;

	INT nIndex = GetCurrentGridPos();
	if( nIndex < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewMemberInfo") );

	CString sRecKey;
	ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("������KEY") , sRecKey , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewMemberInfo") );
	
	// 2007.04.02 add by pdw {{++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Ż��ȸ���� ������ �� �� ����.
	CString strUserNo;
	ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("�����ڹ�ȣ") , strUserNo , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewMemberInfo") );
	strUserNo.TrimLeft();strUserNo.TrimRight();

	if( strUserNo.IsEmpty() )
	{
		CString strQuery, strResult;
		strQuery.Format( _T("SELECT USER_CLASS FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), sRecKey );
		ids = pDM->GetOneValueQuery( strQuery, strResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("ViewMemberInfo") );

		// 0:����, 1:��������, 2:����, 3:Ż��
		if( strResult ==_T("3") )
		{
			AfxMessageBox( _T("Ż��ó���� �Ϸ�Ǿ� ��ȸ�� ������ �����ϴ�."), MB_ICONSTOP );
			return 0;
		}
	}
	//---------------------------------------------------------------------------------------}}

	CString sGridAlias;
	ids = GetParentGridAlias( sGridAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewMemberInfo") );
	
	CBO_LOC_3320 ViewMember(m_pParentMgr, m_pLoanShareManager, m_pLoan);
	ViewMember.SetMode(4,TRUE);
	ViewMember.SetRecKey( sRecKey );
	ViewMember.SetGridAlias( sGridAlias );
	ViewMember.m_nAction = ACTION_READ_F_LOANIFOMGR;
	ViewMember.DoModal();
	
	return 0;

	EFS_END
	return -1;

}

// ���� ����Ű�� �ִ� �׸����� ���� ����
INT CBL_LOC_LOAN_MNG_PROC::GetCurrentGridPos()
{
	EFS_BEGIN

	CString sCMAlias;
	CString sGridAlias;

	INT ids;

	ids = GetParentCMAlias( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetCurrentGridPos") );

	ids = GetParentGridAlias( sGridAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetCurrentGridPos") );

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetCurrentGridPos") );
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( sGridAlias );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetCurrentGridPos") );

	INT nIndex = pGrid->GetIdx();
	if( nIndex < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetCurrentGridPos") );

	return nIndex;

	EFS_END
	return -1;

}

// û����ȣ �����ϴ� �Լ�
INT CBL_LOC_LOAN_MNG_PROC::MakeShelfCode( CString sDMAlias , INT nRowIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeShelfCode") );
	
	INT nRowCount = pDM->GetRowCount();
	
	CString sShelfCode;

	ids = CLocCommonAPI::MakeShelfCode( m_pParentMgr , pDM , nRowIndex , sShelfCode );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeShelfCode") );
	

	return 0;

	EFS_END
	return -1;

}

// �����ڷ� ���� ����
INT CBL_LOC_LOAN_MNG_PROC::ModifyLoanInfo()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias;
	ids = GetParentDM( sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ModifyLoanInfoDBProc"));

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ModifyLoanInfoDBProc"));

	// Mobile API ����
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );

	// 1. SQL�� ����
	ids = MakeModifySQL( pMobileApi );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ModifyLoanInfo") );

	// 2. DB ����
	ids = ModifyLoanInfoDBProc( pMobileApi );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ModifyLoanInfo") );

	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	// 3. DM ����
	ids = CMToDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ModifyLoanInfo") );

	// 4. �׸��� ����
	ids = DMToGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ModifyLoanInfo") );

	return 0;

	EFS_END
	return -1;

}

// �����ڷ����_���� SQL������
INT CBL_LOC_LOAN_MNG_PROC::MakeModifySQL(CKolas2up_MobileApi *pMobileApi)
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias;
	ids = GetCMAlias3320( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeModifySQL"));

	const INT nCnt = 7;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("������") , _T("�ݳ�������") , _T("�ݳ���") , _T("������") , _T("���ุ����") ,
		_T("��ü��") , _T("����") 
	};
	TCHAR *sDBFieldName[nCnt] = 
	{
		_T("LOAN_DATE") , _T("RETURN_PLAN_DATE") , _T("RETURN_DATE") , _T("RESERVATION_DATE") , _T("RESERVATION_EXPIRE_DATE") , 
		_T("ARREAR") , _T("REPARATION")
	};
	TCHAR *sType[nCnt] = 
	{
		_T("NUMERIC") , _T("NUMERIC") , _T("NUMERIC") , _T("NUMERIC") , _T("NUMERIC") , 
		_T("NUMERIC") , _T("NUMERIC")
	};

	// 2007.06.18 add by pdw
	CString strTime;
	CESL_DataMgr* pTimeDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3220"));
	if( !pTimeDM ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeModifySQL"));

	CString sGetData[nCnt];
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeModifySQL"));

		if( _T("��ü��") == sControlAlias[i] ||
			_T("����") == sControlAlias[i]) continue;
			
		if( !sGetData[i].IsEmpty() )
		{
			if( _T("������") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("����ð�"),0,strTime);
			}
			else if( _T("�ݳ�������") == sControlAlias[i] )
			{
				// 2009.08.05 UPDATE BY PWR
				strTime = _T("23:59:59");
				//pTimeDM->GetCellData(_T("�ݳ������ð�"),0,strTime);
			}
			else if( _T("�ݳ���") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("�ݳ��ð�"),0,strTime);
			}
			else if( _T("������") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("����ð�"),0,strTime);
			}
			else if( _T("���ุ����") == sControlAlias[i] )
			{
				// 2009.08.05 UPDATE BY PWR
				strTime = _T("23:59:59");
				//pTimeDM->GetCellData(_T("���ุ��ð�"),0,strTime);
			}

			if( strTime.IsEmpty() )
				strTime = _T("00:00:00");

			CString strTmp;
			strTmp.Format(_T("TO_DATE('%s %s','YYYY/MM/DD HH24:MI:SS')"),sGetData[i], strTime);

			sGetData[i] = strTmp;
		}
	}


	CString sDMAlias;
	ids = GetParentDM( sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeModifySQL")); 

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeModifySQL"));

	pDM->StartFrame();
	pDM->InitDBFieldData();

	CString sUpdateFields;
	for( INT j = 0 ; j < nCnt ; j++ )
	{
		if( sGetData[j].IsEmpty() )
			if( CString(_T("NUMERIC")).Compare(sType[j])==0)
				sGetData[j] = _T("NULL");
			else
			{
				sType[j] = _T("NUMERIC");
				sGetData[j] = _T("NULL");
			}
		ids = pDM->AddDBFieldData( sDBFieldName[j] , sType[j] , sGetData[j] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeModifySQL")); 

		// 2009.08.05 ADD BY PWR : �������
		if(!sUpdateFields.IsEmpty())
		{
			sUpdateFields += _T(",");
		}
		sUpdateFields += sDBFieldName[j];
	}
	// 2009.08.05 ADD BY PWR : LAST_WORK
	pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�����ڷ����_����"), __WFILE__, __LINE__ ), TRUE );

	INT nIndex = GetCurrentGridPos();
	if( nIndex < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeModifySQL") );

	CString sRecKey;
	ids = pDM->GetCellData( _T("����/�ݳ�����KEY") , nIndex , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("MakeModifySQL"));

	// 2009.06.09 UPDATE BY PWR : CO_LOAN_TBL�� �̵��� �ڷ��ϼ� �����Ƿ� ���� UPDATE�Ѵ�.
	CString strQuery, strCnt;
	strQuery.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE REC_KEY=%s"), sRecKey);
	pDM->GetOneValueQuery(strQuery, strCnt, FALSE);
	if(0 < _ttoi(strCnt))
	{
		ids = pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	}
	else
	{
		ids = pMobileApi->MakeUpdateFrame( _T("CO_LOAN_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	}
	//ids = pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("MakeModifySQL"));

// 	// 2009.08.05 ADD BY PWR : �������
// 	UPLOAD_SHARE_INFO UploadShareInfo;
// 	UploadShareInfo.nType = 0;
// 	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
// 	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
// 	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
// 	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
// 	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;
// 	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
// 	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

	return 0;

	EFS_END
	return -1;

}

// �����ڷ� ���� ���� :: DB ����
INT CBL_LOC_LOAN_MNG_PROC::ModifyLoanInfoDBProc(CKolas2up_MobileApi *pMobileApi)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ModifyLoanInfoDBProc");

	CString sDMAlias;
	ids = GetParentDM( sDMAlias );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	// 2005.5. ADD BY PDJ
	if ( m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE�� ����
// 		//++2008.10.15 UPDATE BY PWR {{++
// 		// ���սý��� ����
// 		CString	strWhere;
// 		strWhere = _T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");		
// //		//++2006.07.31 ADD BY PDW(Jr) {{++
// //		// ���������� ����ϴ��� üũ
// //		CString	strWhere, strManageCode, strManageValue;		
// //		
// //		strWhere.Format(_T("SELECT MAX(VALUE_NAME) FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND ")
// //						_T(" GROUP_2='��Ÿ' AND ")
// //	                    _T(" GROUP_3='����' AND ")
// //                        _T(" CLASS_ALIAS='�������к�����������'") );
// //		pDM->GetOneValueQuery( strWhere, strManageValue, FALSE );
// //		//--2006.07.31 ADD BY PDW(Jr) --}}
// //
// //		strWhere.Format(_T("SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
// //						_T("WHERE ID = '%s'"), m_strUserID );
// //		pDM->GetOneValueQuery( strWhere, strManageCode, FALSE );
// //
// //		if ( strManageCode == _T("") || strManageValue == _T("N") )
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"));		
// //		}
// //		else
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE ='%s'"), strManageCode);		
// //		}
// 		//--2008.10.15 UPDATE BY PWR --}}
// 
// 		pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
		m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	}


	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = pMobileApi->EndFrame();

	INT nIndex = GetCurrentGridPos();
	if(0 > nIndex) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	// 2010.08.05 ADD BY PWR : ���յ������� �����ڷ���� ����
	if(_T("Y") == m_pLoan->m_sIsUnityLoanService)
	{
		CString strGetData;
		ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3220"), _T("����ȸ������"), strGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(_T("Y") == strGetData)
		{
			CString strStatus;
			pDM->GetCellData(_T("�������"), nIndex, strStatus);
			CString strRegNo;
			pDM->GetCellData(_T("��Ϲ�ȣ"), nIndex, strRegNo);
			ConvertLoanStatusCodeToDesc(1, strRegNo, strStatus);
			if(_T("3") != strStatus || _T("4") != strStatus)
			{
				pDM->GetCellData(_T("��������"), nIndex, strGetData);
				if(strGetData == m_pParentMgr->m_pInfo->MANAGE_CODE)
				{
					CESL_DataMgr* pTimeDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3220"));
					if(NULL == pTimeDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					CFrameWriter Writer(m_pParentMgr->m_pInfo);
					Writer.setCommand(_T("IL_K23_LOC_B01_SERVICE"));
					Writer.addRecord();
					CString strRecKey;
					ids = pDM->GetCellData(_T("����/�ݳ�����KEY"), nIndex, strRecKey);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					Writer.addElement(_T("����KEY"), strRecKey);
					Writer.addElement(_T("���⵵����"), m_pParentMgr->m_pInfo->LIB_CODE);
					pDM->GetCellData(_T("�����ڹ�ȣ"), nIndex, strGetData);
					Writer.addElement(_T("���մ����ڹ�ȣ"), strGetData);
					CString strLoanDate, strTime;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3220"), _T("������"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						pTimeDM->GetCellData(_T("����ð�"),0,strTime);
						if(strTime.IsEmpty()) strTime = _T("00:00:00");
						strLoanDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("������"), strLoanDate);
					}
					CString strReturnDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3220"), _T("�ݳ���"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						pTimeDM->GetCellData(_T("�ݳ��ð�"),0,strTime);
						if(strTime.IsEmpty()) strTime = _T("00:00:00");
						strReturnDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("�ݳ���"), strReturnDate);
					}
					CString strReturnPlanDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3220"), _T("�ݳ�������"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						strTime = _T("23:59:59");
						strReturnPlanDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("�ݳ�������"), strReturnPlanDate);
					}
					CString strReservationDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3220"), _T("������"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						pTimeDM->GetCellData(_T("����ð�"),0,strTime);
						if(strTime.IsEmpty()) strTime = _T("00:00:00");
						strReservationDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("������"), strReservationDate);
					}
					CString strReservationExpireDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3220"), _T("���ุ����"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						strTime = _T("23:59:59");
						strReservationExpireDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("���ุ����"), strReservationExpireDate);
					}
					CFrameSender Sender(m_pParentMgr->m_pInfo);
					Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("�����ڷ����_����"), __WFILE__, __LINE__ ));
				}
			}
		}
	}

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// ���� ���� ����/�ݳ� ���� , ������ ����� ���� �������� �˸��� 
	// 2003�� 11�� 8�� �۾�
	CString sRecKey;
	ids = pDM->GetCellData( _T("����/�ݳ�����KEY") , nIndex , sRecKey );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("LOAN_DATE,RETURN_PLAN_DATE,RETURN_DATE,RESERVATION_DATE,RESERVATION_EXPIRE_DATE,ARREAR,REPARATION");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	
	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------

	return 0;

EFS_END
return -1;
}

// �����ڷ� ���� ���� :: CM�� �ִ� �ڷḦ DM�� ����
INT CBL_LOC_LOAN_MNG_PROC::CMToDM()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias;
	CString sDMAlias;

	ids = GetCMAlias3320( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CMToDM"));

	ids = GetParentDM( sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CMToDM"));

	// 2007.06.18 add by pdw
	CString strTime;
	CESL_DataMgr* pTimeDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3220"));
	if( !pTimeDM ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeModifySQL"));

	const INT nCnt = 7;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("������") , _T("�ݳ�������") , _T("�ݳ���") , _T("������") , _T("���ุ����") ,
		_T("��ü��") , _T("����") 
	};

	CString sGetData[nCnt];

	INT nIndex = GetCurrentGridPos();
	if( nIndex < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CMToDM") );

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CMToDM"));

		// 2007.06.18 add by pdw
		if( _T("��ü��") == sControlAlias[i] ||
			_T("����") == sControlAlias[i]) continue;
			
		if( !sGetData[i].IsEmpty() )
		{
			if( _T("������") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("����ð�"),0,strTime);
			}
			else if( _T("�ݳ�������") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("�ݳ������ð�"),0,strTime);
			}
			else if( _T("�ݳ���") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("�ݳ��ð�"),0,strTime);
			}
			else if( _T("������") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("����ð�"),0,strTime);
			}
			else if( _T("���ุ����") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("���ุ��ð�"),0,strTime);
			}

			// 2009.10.27 ADD BY PWR : �ݳ�������, ���ุ���� �ð� 23:59:59�Է�
			if(_T("�ݳ�������") == sControlAlias[i] || _T("���ุ����") == sControlAlias[i])
			{
				strTime = _T("23:59:59");
			}
			else
			{
				if( strTime.IsEmpty() && 
					!( _T("���ุ����") == sControlAlias[i] || _T("�ݳ�������") == sControlAlias[i] ) )
					strTime = _T("00:00:00");
			}

			if( 8 < strTime.GetLength() )
				strTime = strTime.Mid(0,8);


			CString strTmp;
			strTmp.Format(_T("%s %s"),sGetData[i], strTime);
			strTmp.TrimLeft();strTmp.TrimRight();
			sGetData[i] = strTmp;
		}

		ids = m_pParentMgr->SetDataMgrData( sDMAlias , sControlAlias[i] , sGetData[i] , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CMToDM"));
	}

	return 0;

	EFS_END
	return -1;

}

// �����ڷ� ���� ���� :: DM�� �ִ� �ڷḦ �׸��忡 ����
INT CBL_LOC_LOAN_MNG_PROC::DMToGrid()
{
EFS_BEGIN

	INT ids;

	CString sCMAlias;
	CString sGridAlias;
	CString sDMAlias;

	ids = GetParentCMAlias( sCMAlias );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("DMToGrid"));
	}

	ids = GetParentDM( sDMAlias );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("DMToGrid"));
	}
	
	ids = GetParentGridAlias( sGridAlias );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("DMToGrid"));
	}

	INT nIndex = GetCurrentGridPos();
	if( 0 > nIndex ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1040 , _T("DMToGrid") );
	}

	const INT nCnt = 5;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("������") , _T("�ݳ�������") , _T("�ݳ���") , _T("������") , _T("���ุ����") ,
	};

	INT nColIndex[nCnt] = 
	{
		-1 , -1 , -1 , -1 , -1 
	};


	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( NULL == pCM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1050 , _T("DMToGrid"));
	}

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1060 , _T("DMToGrid")); 
	}

	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sControlAlias , nCnt , nColIndex );

	CString sPublishForm;

	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("����/���ӱ���") , sPublishForm );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1070 , _T("DMToGrid") );
	}

	CString sGetData[nCnt];
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetDataMgrData( sDMAlias , sControlAlias[i] , sGetData[i] , nIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1080 , _T("DMToGrid"));
		}
		
		if( 0 == sPublishForm.Compare(_T("N")) && (( 3 == i ) || ( 4 == i )) ) 
		{
			continue;
		}

		if( -1 == nColIndex[i] )
		{
			continue;
		}

		ids = pGrid->SetAt( nIndex , nColIndex[i] , sGetData[i] );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1090 , _T("DMToGrid"));
		}
	}

	return 0;

EFS_END
return -1;
}


// ����ó�� :: �������
INT CBL_LOC_LOAN_MNG_PROC::CancelReservation()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("CancelReservation");

	// 1. ���õ� �ڷ� ������� �ϱ�
	CString sDMAlias = _T("DM_BO_LOC_3240_BO");
	CString sParentDMAlias = _T("DM_BO_LOC_3200");
	CString sCMAlias = _T("CM_BO_LOC_3240");
	CString sTableName = _T("LS_WORK_T01");

	// ���� ���ǰ� �ִ� �׸��� ������
	CESL_Grid *pGrid = GetClientGridPtr();
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = pGrid->SelectMakeList();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	// �θ� �׸��� ������
	CESL_Grid *pParentGrid = GetParentGridPtr();
	if(NULL == pParentGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT nRowCount = pGrid->SelectGetCount();

	if(1 > nRowCount)
	{
 		ViewMessage(_T("�ڷḦ �������ֽʽÿ�"));
		return 0;
	}
	ids = MessageBox( m_pParentMgr->m_hWnd , _T("���õ� �ڷḦ ����Ͻðڽ��ϱ�?") , _T("���� ���") , MB_YESNO | MB_ICONQUESTION );
	if( IDNO == ids ) 
	{
		return 0;
	}

	// ���� ���Ǵ� DM ������
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	// �θ� DM ������
	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if(NULL == pParentDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if ( TRUE == m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE�� ����
// 		CString	strWhere, strManageCode;		
// //		strWhere.Format(_T("SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
// //						_T("WHERE ID = '%s'"), m_strUserID );
// //		pDM->GetOneValueQuery( strWhere, strManageCode, FALSE );
// //
// //		if ( strManageCode == _T("") )
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"));		
// //		}
// //		else
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE ='%s'"), strManageCode);		
// //		}
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE"));
// 		pDM->GetOneValueQuery( strWhere , m_strLibCode , FALSE );
		m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	}

	pGrid->SelectGetTailPosition();
	INT nIndex;
	CString sRecKey;
	CString sStatus;
	CString sReservationBookCnt;
	CString sUserRecKey;
	CString sMailSql;

	INT nStatusColIndex = -1;
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pParentGrid , _T("�������") , nStatusColIndex );

	CLocMailMgrAPI* pMailMgr;
	pMailMgr = NULL;

	if( m_pParentMgr->m_pInfo->MODE == 10000 || (m_pParentMgr->m_pInfo->MODE == 30000) ) ;
	else
	{
		if( m_pReservationInfo->m_pLoanProc->m_pCheckLoan->IsSendCancelMail() )
		{
			pMailMgr = new CLocMailMgrAPI( m_pParentMgr );
			pMailMgr->CreateMailInfo();
		}
	}

	INT		i , nCount = 0;
	for( i=0 ; i<nRowCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		if(0 > nIndex) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pDM->GetCellData( _T("����/�ݳ�����KEY") , nIndex , sRecKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pDM->GetCellData( _T("�������") , nIndex , sStatus );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		// 2005.02.21 UPDATE BY PDJ
		// ���� �������δ� �ڷᰡ �˻����� �ʴ´�.
		//if( (0!=sStatus.Compare(_T("����") )) || (0!=sStatus.Find(_T("����(") ))  )
		if(	TRUE !=	( 0 == sStatus.Compare(_T("����")) || 0 == sStatus.Find(_T("����(")) ) )
		{
			pGrid->SelectGetPrev();
			continue;
		}

		// 2010.05.20 ADD BY PWR : ���ο������ ���ο����û�ڷ�� �޽���, ���������ڷ�� ��ҺҰ�ó��
		if(_T("Y") == m_pLoan->m_sUnmannedReserveSys)
		{
			CString strQuery, strValue, strMsg;;
			strQuery.Format(_T("SELECT UNMANNED_RESERVATION_LOAN FROM LS_WORK_T01 WHERE REC_KEY=%s"), sRecKey);
			pDM->GetOneValueQuery(strQuery, strValue);
			if(_T("Y") == strValue)
			{ // ���ο����û
				strMsg.Format(_T("���ο����û �ڷ��Դϴ�.\r\n������� �Ͻðڽ��ϱ�?"));
				if(IDYES != MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("���"), MB_ICONWARNING | MB_YESNO))
				{
					pGrid->SelectGetPrev();
					continue;
				}
			}
			else if(_T("O") == strValue)
			{ // ���ο�����, ����
				strMsg = _T("���ο����� �ڷ�� ������� �� �� �����ϴ�.");
				MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("���"), MB_ICONWARNING);
				pGrid->SelectGetPrev();
				continue;
			}
		}

		// Mobile API ����
		CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );

		// DB UPDATE
		// LH_STORAGE_V01
		pDM->StartFrame();
		pDM->InitDBFieldData();
		ids = pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("4") );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		//++2008.09.02 DEL BY LKS {{++
//REMOVE�� 		if( m_pParentMgr->m_pInfo->MODE == 10000 || (m_pParentMgr->m_pInfo->MODE==20000) )	
//REMOVE�� 			ids = pDM->AddDBFieldData( _T("RESERVE_CANCEL_DATE") , _T("NUMERIC") , _T("now()") );
		//--2008.09.02 DEL BY LKS --}}
//		else

// 		// 2010.07.28 UPDATE BY PWR : ���յ��������϶��� ������� ����Ͽ� ����ǵ���
// 		CString strDate;
// 		CString sReserveCancelDate;
// 		if(_T("Y") == m_pLoan->m_sIsUnityLoanService)
// 		{
// 			strDate = (CLocCommonAPI::GetDBTime(m_pParentMgr)).Format(_T("%Y/%m/%d %H:%M:%S"));
// 			sReserveCancelDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strDate);
// 		}
// 		else
// 		{
// 			sReserveCancelDate = _T("SYSDATE");
// 		}
// 		ids = pDM->AddDBFieldData(_T("RESERVE_CANCEL_DATE"), _T("NUMERIC"), sReserveCancelDate);
// 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
 		ids = pDM->AddDBFieldData( _T("RESERVE_CANCEL_DATE") , _T("NUMERIC") , _T("SYSDATE") );
 		if( 0 > ids ) 
 		{
 			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1100 , _T("CancelReservation") );
 		}

		//++2008.04.16 ADD BY PDW {{++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ������� Ÿ�� �ʵ尡 �߰��Ǿ���
		ids = pDM->AddDBFieldData( _T("RESERVATION_CANCEL_TYPE"), _T("STRING"), _T("1") );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		//----------------------------------------------------------------------------------------}}

		//++2008.11.20 ADD BY PWR {{++
		// ������ҽ� R_DEVICE�� KOLASIII���� (LH_STORAGE_V01���� SELECT�ɼ� �ֵ���)
		ids = pDM->AddDBFieldData( _T("R_DEVICE"), _T("STRING"), _T("KOLASIII"), TRUE );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		//--2008.11.20 ADD BY PWR --}}

		// 2009.08.05 ADD BY PWR : LAST_WORK
		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�����ڷ����_�������"), __WFILE__, __LINE__ ), TRUE );

//		ids = pDM->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		ids = pMobileApi->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		// CO_LOAN_USER_TBL
		// ������ ����
		ids = pDM->GetCellData( _T("������KEY") , nIndex , sUserRecKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = GetTotalReservationBookCnt( sUserRecKey , sReservationBookCnt );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		INT nReservationBookCnt = _ttoi(sReservationBookCnt);
		nReservationBookCnt = ( 0 > nReservationBookCnt-1 ) ? 0 : nReservationBookCnt-1;
		sReservationBookCnt.Format( _T("%d") , nReservationBookCnt);
	
		pDM->InitDBFieldData();
		ids = pDM->AddDBFieldData( _T("RESERVATION_COUNT") , _T("NUMERIC") , sReservationBookCnt );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		// 2009.08.05 ADD BY PWR : LAST_WORK
		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�����ڷ����_�������"), __WFILE__, __LINE__ ), TRUE );

//		ids = pDM->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sUserRecKey );
		ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sUserRecKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pMobileApi->SendFrame();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		pDM->EndFrame();

// 		// 2010.07.28 ADD BY PWR : ���յ������� �����ڷ���� �������
// 		if(_T("Y") == m_pLoan->m_sIsUnityLoanService)
// 		{
// 			CString sParentIndex;
// 			ids = pDM->GetCellData(_T("�θ�INDEX"), nIndex, sParentIndex);
// 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 			CString strGetData;
// 			ids = pParentDM->GetCellData(_T("��������"), _ttoi(sParentIndex), strGetData);
// 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 			if(strGetData == m_pParentMgr->m_pInfo->MANAGE_CODE)
// 			{
// 				ids = pParentDM->GetCellData(_T("����ȸ������"), _ttoi(sParentIndex), strGetData);
// 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 				if(_T("Y") == strGetData)
// 				{
// 					CFrameWriter Writer(m_pParentMgr->m_pInfo);
// 					Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
// 					Writer.addRecord();
// 					ids = pParentDM->GetCellData(_T("�����ڹ�ȣ"), _ttoi(sParentIndex), strGetData);
// 					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 					Writer.addElement(_T("���մ����ڹ�ȣ"), strGetData);
// 					Writer.addElement(_T("���⵵������ȣ"), m_pParentMgr->m_pInfo->LIB_CODE);
// 					Writer.addElement(_T("�ݳ���������ȣ"), m_pParentMgr->m_pInfo->LIB_CODE);
// 					Writer.addElement(_T("���������"), sReserveCancelDate);
// 					ids = pParentDM->GetCellData(_T("��Ϲ�ȣ"), _ttoi(sParentIndex), strGetData);
// 					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 					Writer.addElement(_T("��Ϲ�ȣ"), strGetData);
// 					Writer.addElement(_T("����"), _T("4"));
// 					CFrameSender Sender(m_pParentMgr->m_pInfo);
// 					Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("�����ڷ����-�������"), __WFILE__, __LINE__));
// 				}
// 			}
// 		}

		// Mobile API delete
		CLocCommonAPI::MobileApiEndLog( pMobileApi );

		// --------------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------------
		// ���� ���� ����/�ݳ� ���� , ������ ����� ���� �������� �˸��� 
		// 2003�� 11�� 8�� �۾�
		
		
		UPLOAD_SHARE_INFO UploadShareInfo;
		UploadShareInfo.nType = 0;
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("STATUS,RESERVE_CANCEL_DATE,RESERVATION_CANCEL_TYPE,R_DEVICE");
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;		
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		
		// 2. ������ ���� Update
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
		UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sUserRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("RESERVATION_COUNT");
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		
		// --------------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------------
		
		// ȭ�� UPDATE
		// GRID ������Ʈ
		pGrid->SelectGetPrev();

		CString sParentIndex;
		ids = pDM->GetCellData( _T("�θ�INDEX") , nIndex , sParentIndex );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pParentDM->SetCellData( _T("�������") , _T("�������") , _ttoi(sParentIndex) );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if ( TRUE == m_pLoan->m_bVolReserve )
		{
				/// ���� ���� �����ϱ�
				/// ���� ������ ����Ǿ�� �Ѵ�
				CString sStatus , sRSpeciesKey , sRVolInfoKey;
				ids = pDM->GetCellData( _T("��KEY") , nIndex , sRSpeciesKey);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

				ids = pDM->GetCellData( _T("��ȣ����KEY") , nIndex , sRVolInfoKey );
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

				CESL_DataMgr* pParentDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3200") );
				if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

				CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentMgr->FindControl( _T("CM_BO_LOC_3200") , _T("MainGrid") );
				if(NULL == pParentGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

				CString sSpeciesKey , sVolInfoKey;
				INT		j , nParentRowCnt;
				nParentRowCnt		=	pParentDM->GetRowCount();
				for( j=0 ; j<nParentRowCnt ; j++ )
				{
					ids = pParentDM->GetCellData( _T("�������") , j , sStatus );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					ids = pParentDM->GetCellData( _T("��KEY") , j , sSpeciesKey );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					ids = pParentDM->GetCellData( _T("��ȣ����KEY") , j , sVolInfoKey );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					// 2009.05.18 UPDATE BY KSJ : ������ҽ� �׸��� ���� �� �Ǵ� ���� ����
					//if( ( 0 == sStatus.Find(_T("����(")) ) && ( 0 == sSpeciesKey.Compare(sRSpeciesKey) ) && ( 0 == sVolInfoKey.Compare(sRVolInfoKey) ) )
					if( ( 0 == sStatus.Find(_T("����")) ) && ( 0 == sSpeciesKey.Compare(sRSpeciesKey) ) && ( 0 == sVolInfoKey.Compare(sRVolInfoKey) ) )
					{
						ReViewOrder( pParentDM , j );
						pParentGrid->DisplayLine(j);
					}
				}

				INT		nRowCnt;
				nRowCnt		=	pDM->GetRowCount();
				for( j=0 ; j<nRowCnt ; j++ )
				{
					ids = pDM->GetCellData( _T("��KEY") , j , sSpeciesKey );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					ids = pDM->GetCellData( _T("��ȣ����KEY") , j , sVolInfoKey );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					if( ( 0 == sSpeciesKey.Compare(sRSpeciesKey) ) && ( 0 == sVolInfoKey.Compare(sRVolInfoKey) ) )
					{
						ReViewOrder( pDM , j );
						pGrid->DisplayLine(j);
					}
				}
		}
		else
		{
				if( -1 != nStatusColIndex )
				{
					ids = pParentGrid->SetAt( _ttoi(sParentIndex) , nStatusColIndex , _T("�������") );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				}
		}

		// 2006-11-21 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++
		// �ʵ�� ����
		//sMailSql.Format( _T(" CL.LOAN_KEY = %s ") , sRecKey );
		sMailSql.Format( _T(" CL.REC_KEY = %s ") , sRecKey );
		// -------------------------------------------------------------------

		if( m_pReservationInfo->m_pLoanProc->m_pCheckLoan->IsSendCancelMail() )
		{
			if( NULL != pMailMgr )
			{
				pMailMgr->MakeRecord( 6 , sMailSql , FALSE );
			}
		}

		pDM->DeleteRow( nIndex );
		nCount++;
	}

	if( NULL != pMailMgr )
	{
		delete pMailMgr;
	}

	pMailMgr = NULL;

	CString sMsg;
	if( 0 < nCount )
	{
		// ���� �� �ο�
		ReViewOrder();

		sMsg.Format( _T("�� %d���� ���� ��ҵǾ����ϴ�.") , nCount );
		ViewMessage( sMsg );
		m_pParentMgr->AllControlDisplay(sCMAlias);
	}
// 	else
// 	{
// 		sMsg.Format( _T("�ڷḦ �������ֽʽÿ�") , nCount );
// 		ViewMessage( sMsg );
// 	}

	return 0;

EFS_END
return -1;
}

// �� ���� å�� GET
INT CBL_LOC_LOAN_MNG_PROC::GetTotalReservationBookCnt( CString sUserRecKey , CString &sReservationBookCnt )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3240_BO");
	// ���� ���Ǵ� DM ������
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1, _T("GetTotalReservationBookCnt") );

	CString sSQL;
	sSQL.Format( _T("SELECT RESERVATION_COUNT FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s ") , sUserRecKey );
//	ids = pDM->GetOneValueQuery( sSQL , sReservationBookCnt );
	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr, pDM , sSQL , sUserRecKey );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetTotalReservationBookCnt") );

	return 0;

	EFS_END
	return -1;

}

// ���� ������ �ο�
INT CBL_LOC_LOAN_MNG_PROC::RecordExpireDate()
{
EFS_BEGIN

	INT ids;

	// 1. ���õ� �ڷ� ������� �ϱ�
	CString sDMAlias		= _T("DM_BO_LOC_3240_BO");
	CString sParentDMAlias	= _T("DM_BO_LOC_3200");

	// ���� ���ǰ� �ִ� �׸��� ������
	CESL_Grid *pGrid = GetClientGridPtr();
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("RecordExpireDate") );
	}

	ids = pGrid->SelectMakeList();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("RecordExpireDate") );
	}

	// �θ� �׸��� ������
	CESL_Grid *pParentGrid = GetParentGridPtr();
	if( NULL == pParentGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("RecordExpireDate") );
	}

	INT nRowCount = pGrid->SelectGetCount();

	// ���� ���Ǵ� DM ������
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1040 , _T("RecordExpireDate") );
	}

	// �θ� DM ������
	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( NULL == pParentDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1050 , _T("RecordExpireDate") );
	}

	INT nExpireColIndex = -1;
	INT nParentExpireColIndex = -1;

	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , _T("���ุ����") , nExpireColIndex );
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pParentGrid , _T("���ุ����") , nParentExpireColIndex );

	// 2009.04.23 ADD BY KSJ : SMS �߼۰�� �߰�
	INT		nSMSResultIdx = -1;
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , _T("SMS�߼۰��") , nSMSResultIdx );


	pGrid->SelectGetTailPosition();
	
	INT nIndex;
	CString sRecKey;
	CString sBookKey;
	CString sRegNo;

	INT		nCount				= 0;
	INT		nFailReturn			= 0;
	INT		nFailFirstReserve	= 0;
	INT		nFailRC				= 0;	// ���������� �ο��� �� �Ǿ� �־ ������ �ڷ� Cnt
	INT		nFailReserveExpireDay	= 0;	// ���ุ������ �̹� �ο� �Ǽ� ������ �ڷ� Cnt
	// 2009.10.21 ADD BY KSJ : ���������� �ٸ� �ڷ� ���ุ���� �ο� �� �ǵ��� ����
	INT		nFailOtherManageCode = 0;	// ���������� �ٸ� �ڷ�� ������ �ڷ� Cnt
	
	// 2009.04.22 ADD BY KSJ : SMS �߼۰�� �߰�
	INT		nSendSMSCnt		=	0;		// SMS �߼� ���� Cnt
	INT		nFailSMSUseYN	=	0;		// �̿��ڰ� SMS ���� ���� �� �ؼ� SMS�߼� ������ ��� Cnt
	INT		nFailPhone		=	0;		// �̿����� ����ȣ�� ��� SMS�߼� ������ ��� Cnt
	INT		nFailMakeMsg	=	0;		// �޼��� ������ �����Ͽ� SMS�߼� ������ ��� Cnt
	INT		nFailUnSelectProc	=	0;	// Procedure���� ������ ��� Cnt
	
	// 2009.05.18 ADD BY KSJ : SMS �߼� ��� �޽��� ��� Flag �߰�
	BOOL	bShowSMSSendResult = FALSE;

	// 2010.06.17 ADD BY PWR : ���ο������ �߰�
	INT nUnmennedCnt = 0;
	INT nFailUnmenned = 0;

	INT		i;
	for( i=0 ; i<nRowCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		if( 0 > nIndex ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1060 , _T("RecordExpireDate") );
		}

		ids = pDM->GetCellData( _T("����/�ݳ�����KEY") , nIndex , sRecKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1070 , _T("RecordExpireDate") );
		}

		ids = pDM->GetCellData( _T("����åKEY") , nIndex , sBookKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1080 , _T("RecordExpireDate") );
		}

		//���ټ��� ���� �� ���
		/*
		if ( m_pLoan->m_bVolReserve )
		{
				ids = pDM->GetCellData( _T("��Ϲ�ȣ") , nIndex , sRegNo );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8, _T("RecordExpireDate") );

				// ���� �Ϸ�� �ڷ����� �˾ƺ���.
				if( sRegNo.IsEmpty() )
				{
					pGrid->SelectGetPrev();
					nFailRC++;
				}
		}
		*/

		// 2009.10.21 ADD BY KSJ : ���������� �ٸ� �ڷ� ���ุ���� �ο� �� �ǵ��� ����
		CString		strManageCode;
		pDM->GetCellData( _T("��������") , nIndex , strManageCode );
		if( m_pParentMgr->m_pInfo->MANAGE_CODE != strManageCode )
		{
			pGrid->SelectGetPrev();
			nFailOtherManageCode++;
			continue;
		}

		// 2010.06.17 ADD BY PWR : ���δ��⿹������߰�
		BOOL bUnmanned = FALSE;
		CString	strUnmanned;
		ids = pDM->GetCellData(_T("���δ��⿹��"), nIndex, strUnmanned);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1100, _T("RecordExpireDate"));

		//�Ϲݿ��� �� ���
		if ( !(m_pLoan->m_bVolReserve) )
		{
			// 1. ���� ������ �ο��Ϸ��� å�� �ݳ��Ǿ����� ����
			ids = m_pReservationInfo->IsReturn( pDM , sBookKey );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1090 , _T("RecordExpireDate") );
			}

			if( 0 < ids ) 
			{
				pGrid->SelectGetPrev();
				nFailReturn++;
				continue;
			}

			// 2. ���� ������ �ο��Ϸ��� ����� ����� ����� ���� ó������ ����
			ids = m_pReservationInfo->IsFirstReservation( pDM , sBookKey , sRecKey );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1100 , _T("RecordExpireDate") );
			}

			if( 0 < ids ) 
			{
				pGrid->SelectGetPrev();
				nFailFirstReserve++;
				continue;
			}

			// 2010.06.17 ADD BY PWR : �Ϲݿ��࿡���� ���ο����ϰ�� �����Ϻο�����
			if(_T("Y") == m_pLoan->m_sUnmannedReserveSys)
			{
				if(!strUnmanned.IsEmpty())
				{
					pGrid->SelectGetPrev();
					nFailUnmenned++;
					continue;
				}
			}
		}
		// 2010.06.17 ADD BY PWR : ���ټ����϶� ���ο����ϰ�� ó��
		else
		{
			if(_T("Y") == m_pLoan->m_sUnmannedReserveSys)
			{
				if(!strUnmanned.IsEmpty())
				{
					bUnmanned = TRUE;
				}
			}
		}

		//++2008.12.22 ADD BY KSJ{{++
		//�ڷ� ������ �� �� �ڷ��, ���ุ���� �ο� �� �� ����
		CString		strRegNo;
		pDM->GetCellData( _T("��Ϲ�ȣ") , nIndex , strRegNo );
		if( _T("")	==	strRegNo )
		{
			pGrid->SelectGetPrev();
			nFailRC++;
			continue;
		}

		//���ุ������ �̹� �ο� �� �ڷῡ, ���ุ���� �ο� �� �� ����		
		CString		strExpireDate;
		pDM->GetCellData( _T("���ุ����") , nIndex , strExpireDate );
		if( _T("")	!=	strExpireDate )
		{
			pGrid->SelectGetPrev();
			nFailReserveExpireDay++;
			continue;
		}
		//--2008.12.22 ADD BY KSJ--}}

		// 3. ���� ������ ������
		INT nExpireDay = 0;

		CString sPositionCode;
		pDM->GetCellData( _T("����") , nIndex , sPositionCode );

		ids = m_pReservationInfo->GetReservationExpireDay( nExpireDay , sPositionCode );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1110 , _T("RecordExpireDate") );
		}

		CString sExpireDate;
		ids = m_pReservationInfo->MakeReservationExpireDate( nExpireDay , sExpireDate );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1120 , _T("RecordExpireDate") );
		}
		
		// 4. DB ����
		ids = m_pReservationInfo->ExpireDBProc( pDM , sRecKey , sExpireDate );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1130 , _T("RecordExpireDate") );
		}

		// ȭ�� UPDATE
		// GRID ������Ʈ
		pGrid->SelectGetPrev();

		CString sParentIndex;
		ids = pDM->GetCellData( _T("�θ�INDEX") , nIndex , sParentIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1140 , _T("RecordExpireDate") );
		}

		INT		nParentIndex	=	_ttoi(sParentIndex);
		//++2009.01.29 ADD BY KSJ ++{{
		//�������� �� ���ุ���� ��ο� �Ǵ� ������ ���Ͽ�, DM_BO_LOC_3240_BO DM�� ���ุ���� ����
		ids = pDM->SetCellData( _T("���ุ����") , sExpireDate , nIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1150 , _T("RecordExpireDate") );
		}
		//--2009.01.29 ADD BY KSJ --}}

		ids = pParentDM->SetCellData( _T("���ุ����") , sExpireDate , nParentIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1160 , _T("RecordExpireDate") );
		}

		ids = pParentGrid->SetAt( nParentIndex , nParentExpireColIndex  , sExpireDate );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1170 , _T("RecordExpireDate") );
		}
		
		ids = pGrid->SetAt( nIndex , nExpireColIndex , sExpireDate );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1180 , _T("RecordExpireDate") );
		}

		nCount++;

		// 2010.06.17 ADD BY PWR : ���ο����ϰ�� ó��
		if(TRUE == bUnmanned)
		{
			nUnmennedCnt++;
		}

		//===========================================================================
		// 2009.04.21 ADD BY KSJ : ���ุ������ �ο� �ƴٸ� �������� ���� SMS �߼�
		/// SMS System ��뿩��
		CString strSMSSystemUseMode;
		ids = m_pParentMgr->m_pInfo->GetSMSUseMode();
		if( 1 == ids )
		{ //// SMS System ���
			strSMSSystemUseMode	=	_T("Y");
		}
		else
		{ //// SMS System ��� �� ��
			strSMSSystemUseMode	=	_T("N");
		}

		// 2009.05.18 UPDATE BY KSJ : SMS �߼� ��� �޽��� ��� Flag �߰�
		if(		_T("Y") == strSMSSystemUseMode				// SMS System ��뿩��
			&&	_T("Y") == m_pLoan->m_sReserveExpireSMS	)	// ���ุ���� �ο� �� ��� SMS �߼� ����)
		{
			bShowSMSSendResult = TRUE;
		}

		if(		TRUE	==	bShowSMSSendResult
			&&	10000	!=	m_pParentMgr->m_pInfo->MODE		// 2009.05.14 ADD BY KSJ : �̵��������� ��� SMS �߼� �� ��
			&&	30000	!=	m_pParentMgr->m_pInfo->MODE	)	
		{
			// 2010.06.17 ADD BY PWR : ���ο����ڷ�� SMS�߼۾���
			if(!strUnmanned.IsEmpty())
			{
				ids = pDM->SetCellData( _T("SMS�߼۰��") , _T("���ο����ڷ�� ��� ������ �߼۵˴ϴ�.") , nIndex );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1300 , _T("RecordExpireDate") );
				ids = pGrid->SetAt( nIndex , nSMSResultIdx , _T("���ο����ڷ�� ��� ������ �߼۵˴ϴ�.") );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1260 , _T("RecordExpireDate") );
				continue;
			}

			/// �̿��� SMS ���ſ���
			CString		strSMSUseYN;
			ids = pParentDM->GetCellData( _T("SMS���ſ���") , nParentIndex , strSMSUseYN );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1250 , _T("RecordExpireDate") );
			}

			if( _T("Y") != strSMSUseYN )
			{/// �̿��ڰ� SMS ���� ��û�� ���� �ʾƼ� SMS �߼� ����
				ids = pDM->SetCellData( _T("SMS�߼۰��") , _T("���� �������� ���� �̿��� �Դϴ�.") , nIndex );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1300 , _T("RecordExpireDate") );
				}

				ids = pGrid->SetAt( nIndex , nSMSResultIdx , _T("���� �������� ���� �̿��� �Դϴ�.") );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1260 , _T("RecordExpireDate") );
				}

				nFailSMSUseYN++;
				continue;
			}

			CESL_DataMgr TmpDM;
 			TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
 			TmpDM.SetManageCode(m_pParentMgr->m_pInfo->MANAGE_CODE);

			CString		strUserName, strUserKey, strLoanKey, strTitle, strReceiveCallNo, strSMSType;
			strSMSType	=	_T("SMS03");
			ids = pDM->GetCellData( _T("������"),			nIndex, strUserName);		// ȸ���̸�
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1200 , _T("RecordExpireDate") );
			}

			ids = pDM->GetCellData( _T("������KEY"),		nIndex, strUserKey);		// USER_KEY
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1210 , _T("RecordExpireDate") );
			}

			ids = pDM->GetCellData( _T("����/�ݳ�����KEY"),	nIndex, strLoanKey);		// LOAN_KEY
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1220 , _T("RecordExpireDate") );
			}

			ids = pDM->GetCellData( _T("��ǥ��"),			nIndex, strTitle);			// TITLE
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1230 , _T("RecordExpireDate") );
			}

			ids = pParentDM->GetCellData( _T("�޴���"), nParentIndex, strReceiveCallNo);// �޴»�� ��ȣ
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1240 , _T("RecordExpireDate") );
			}
			
			CSMSPreviewDlg	SMSDlg(m_pParentMgr);
			SMSDlg.SetInfo( strUserKey, strLoanKey, strUserName, strTitle, sExpireDate, strReceiveCallNo, strSMSType);
			ids = SMSDlg.MakeMsg();
			if( 1030 == ids )
			{ /// 1030 : �ڵ��� ��ȣ ��� SMS �߼� ����
				ids = pDM->SetCellData( _T("SMS�߼۰��") , _T("������ ��ȭ��ȣ�� �ǹٸ��� �ʽ��ϴ�.") , nIndex );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1310 , _T("RecordExpireDate") );
				}

				ids = pGrid->SetAt( nIndex , nSMSResultIdx , _T("������ ��ȭ��ȣ�� �ǹٸ��� �ʽ��ϴ�.") );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1270 , _T("RecordExpireDate") );
				}

				nFailPhone++;
				continue;
			}
			else if( 0 > ids )
			{ /// �޼��� ���� ���з� ���� SMS �߼� ����
				ids = pDM->SetCellData( _T("SMS�߼۰��") , _T("�޼��� ������ �����Ͽ����ϴ�.") , nIndex );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1320 , _T("RecordExpireDate") );
				}

				ids = pGrid->SetAt( nIndex , nSMSResultIdx , _T("�޼��� ������ �����Ͽ����ϴ�.") );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1280 , _T("RecordExpireDate") );
				}

				nFailMakeMsg++;
				continue;
			}


			ids = SMSDlg.SendMsg();
			if( 0 == ids )
			{ /// SMS �߼� ����
				ids = pDM->SetCellData( _T("SMS�߼۰��") , _T("����") , nIndex );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1330 , _T("RecordExpireDate") );
				}

				ids = pGrid->SetAt( nIndex , nSMSResultIdx , _T("����") );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1290 , _T("RecordExpireDate") );
				}

				nSendSMSCnt++;
			}
			else 
			{ /// Procedure���� �߼� ����
				ids = pDM->SetCellData( _T("SMS�߼۰��") , _T("UnSelectProc Error") , nIndex );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1340 , _T("RecordExpireDate") );
				}

				ids = pGrid->SetAt( nIndex , nSMSResultIdx , _T("UnSelectProc Error") );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1290 , _T("RecordExpireDate") );
				}

				nFailUnSelectProc++;
			}
		}
		//===========================================================================
	}

	CString sMsg;
	if( 0 == nRowCount ) 
	{
		sMsg.Format( _T("�ڷḦ ���� �������ֽʽÿ�") );
	}
	else
	{
		sMsg.Format( _T("�� ó�� �Ǽ� : %d ") , nRowCount );
		CString sTmp;
		// 2009.10.21 ADD BY KSJ : ���������� �ٸ� �ڷ� ���ุ���� �ο� �� �ǵ��� ����
		if( 0 < nFailOtherManageCode )
		{
			sTmp.Format(_T("\r\nŸ�� �����ڷ��̱� ������ ���� ������ �ο��� ������ �Ǽ� : %d ") , nFailOtherManageCode );
		}

		sMsg += sTmp;
		sTmp.Empty();
		if( 0 < nFailRC ) 
		{
			sTmp.Format(_T("\r\n�����Ϸ� ���� ���� �ڷ�  : %d ") , nFailRC );
		}

		sMsg += sTmp;
		sTmp.Empty();
		if( 0 < nFailReturn ) 
		{
			sTmp.Format(_T("\r\n�ݳ��� ���� �ʾ� ���� ������ �ο��� ������ �Ǽ� : %d ") , nFailReturn );
		}

		sMsg += sTmp;
		sTmp.Empty();
		if( 0 < nFailFirstReserve ) 
		{
			sTmp.Format(_T("\r\n�տ� ����� ����� �ֱ� ������ ���� ������ �ο��� ������ �Ǽ� : %d ") , nFailFirstReserve  );
		}

		sMsg += sTmp;
		sTmp.Empty();

		//++2009.01.09 ADD BY KSJ {{++
		//���ุ������ �̹� �ο� �� �ڷῡ ���� Message �߰�
		if( 0 < nFailReserveExpireDay )  
		{
			sTmp.Format(_T("\r\n���� �������� �̹� �ο� �Ǿ��� ������ ���� ������ �ο��� ������ �Ǽ� : %d ") , nFailReserveExpireDay  );
		}

		sMsg += sTmp;
		sTmp.Empty();
		//--2009.01.09 ADD BY KSJ --}}

		// 2010.06.17 ADD BY PWR : ���ο����ϰ�� ó��
		if(0 < nFailUnmenned)
		{
			sTmp.Format(_T("\r\n���ο����ڷ�� ������ �Ǽ� : %d ") , nFailReserveExpireDay  );
		}
		sMsg += sTmp;
		sTmp.Empty();

		if( 0 < nCount ) 
		{
			sTmp.Format(_T("\r\n������ �Ǽ� : %d ") , nCount );
		}

		sMsg += sTmp;
		sTmp.Empty();

		// 2009.05.14 ADD BY KSJ : �̵��������� ��� SMS �߼� �� �� ( 10000 : �̵������� , 30000 : ���� ��� �� �ϴ� MODE �� )
		if( 10000 == m_pParentMgr->m_pInfo->MODE || 30000 == m_pParentMgr->m_pInfo->MODE )
		{/// �̵��������� ���
		}
		else
		{/// �̵������� �ƴ� ���
			// 2009.05.18 ADD BY KSJ : SMS �߼� ����� SMS SYSTEM ����ϰ� ���ุ���� �ο� �� ��� SMS �߼� ��� �� ��쿡�� ������
			if( TRUE == bShowSMSSendResult )
			{
				//==============================================================================
				// 2009.04.22 ADD BY KSJ : SMS �߼� ��� ���
				// 2010.06.17 UPDATE BY PWR : �����ڷ�� SMS�Ⱥ����Ƿ�
				if( 0 < (nCount - nUnmennedCnt) ) 
// 				if( 0 < nCount ) 
				{
					// 2010.06.17 UPDATE BY PWR : �����ڷ�� SMS�Ⱥ����Ƿ�
					sTmp.Format( _T("\n\r\n\rSMS�߼� �� ó�� �Ǽ� : %d ") , nCount - nUnmennedCnt);
// 					sTmp.Format( _T("\n\r\n\rSMS�߼� �� ó�� �Ǽ� : %d ") , nCount );
					sMsg += sTmp;
					sTmp.Empty();
				}

				if( 0 < nFailSMSUseYN ) 
				{
					sTmp.Format( _T("\r\nSMS���� ��û���� �ʾ� �߼� ������ �Ǽ� : %d ") , nFailSMSUseYN );
					sMsg += sTmp;
					sTmp.Empty();
				}
				
				if( 0 < nFailPhone ) 
				{
					sTmp.Format( _T("\r\n�������� ����ȣ�� ���� �߼� ������ �Ǽ� : %d ") , nFailPhone );
					sMsg += sTmp;
					sTmp.Empty();
				}

				if( 0 < nFailMakeMsg ) 
				{
					sTmp.Format( _T("\r\n�޼��� ������ �����Ͽ� �߼� ������ �Ǽ� : %d ") , nFailMakeMsg );
					sMsg += sTmp;
					sTmp.Empty();
				}

				if( 0 < nFailUnSelectProc ) 
				{
					sTmp.Format( _T("\r\nUnSelectProc Error �Ǽ� : %d ") , nFailUnSelectProc );
					sMsg += sTmp;
					sTmp.Empty();
				}

				if( 0 < nSendSMSCnt ) 
				{
					sTmp.Format( _T("\r\nSMS�߼� ������ �Ǽ� : %d ") , nSendSMSCnt );
					sMsg += sTmp;
					sTmp.Empty();
				}
				//==============================================================================	
			}
		}
	}
	ViewMessage( sMsg );

	// 2010.06.17 ADD BY PWR : ���ο�������ڷ� �޽��� ó��
	if(0 < nUnmennedCnt)
	{
		sMsg.Format( _T("[ %d ]���� ���ο��� �ڷᰡ ó�� �Ǿ����ϴ�."), nUnmennedCnt);
		ViewMessage(sMsg);
	}
	
	return 0;

EFS_END
return -1;
}


CESL_Grid* CBL_LOC_LOAN_MNG_PROC::GetClientGridPtr()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3240");
	CString sGridAlias = _T("ReservationProcessGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));

	return pGrid;

	EFS_END
	return NULL;

}

CESL_Grid* CBL_LOC_LOAN_MNG_PROC::GetParentGridPtr()
{
	EFS_BEGIN

	INT ids;

	CString sParentCMAlias;
	CString sParentGridAlias;

	ids = GetParentCMAlias( sParentCMAlias );
	if( ids < 0 ) return NULL;

	ids = GetParentGridAlias( sParentGridAlias );
	if( ids < 0 ) return NULL;

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) return NULL;
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sParentGridAlias ));

	return pGrid;

	EFS_END
	return NULL;

}

// �����ڷ����_����ó��_�����ڻ󼼺���
INT CBL_LOC_LOAN_MNG_PROC::ViewMemberInfo2()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3240_BO");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewMemberInfo2") );

	if( pDM->GetRowCount() < 1 ) return 0;

	CESL_Grid *pGrid = GetClientGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewMemberInfo2") );

	INT nIndex = pGrid->GetIdx();
	if( nIndex < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewMemberInfo2") );

	CString sRecKey;
	ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("������KEY") , sRecKey , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewMemberInfo2") );

	
	CBO_LOC_3320 ViewMember(m_pParentMgr, m_pLoanShareManager, m_pLoan);
	ViewMember.SetMode(5,TRUE);
	ViewMember.SetRecKey( sRecKey );
	ViewMember.m_nAction = ACTION_READ_F_LOANIFOMGR;
	ViewMember.DoModal();
	
	return 0;

	EFS_END
	return -1;

}

// ����
INT CBL_LOC_LOAN_MNG_PROC::OpenMail()
{
	EFS_BEGIN

	
	INT ids;
	CString sDMAlias;
	GetParentDM(sDMAlias);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1, _T("OpenMail") );

	CESL_Grid *pGrid = GetParentGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("OpenMail") );

	if( pGrid->GetCount() < 1 ) return 0;

	pGrid->SelectMakeList();
	INT nCount = pGrid->SelectGetCount();

	if( nCount < 1 ) 
	{
		AfxMessageBox( _T("���� ���� �ڷḦ ������ �ֽʽÿ�") );
		return 0;
	}

	/*
	if( nCount > 100 )
	{
		AfxMessageBox( _T("������ �ڷᰡ 100���� �Ѿ� ���� �߼��� �Ҽ� �����ϴ�. 100�� ���Ϸ� �������ֽʽÿ�.") );
		return 0;
	}
	*/

	// ���� ���� �����ϱ�	
	CLocMailMgrAPI MailMgr(m_pParentMgr);

	ids = MailMgr.MakeRecord( -1 , pDM , pGrid , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OpenMail") );


	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::SetCheckParentMailSendYN( CString sLoanKey , CString sYN )
{
	EFS_BEGIN

	INT ids;

	CString sParentDMAlias = _T("DM_BO_LOC_3200");

	// ���� ���ǰ� �ִ� �׸��� ������

	// �θ� �׸��� ������
	CESL_Grid *pParentGrid = GetParentGridPtr();
	if(pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SetCheckMailSendYN") );

	// �θ� DM ������
	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("OSetCheckMailSendYN"));

	INT nParentColIndex = -1;

	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pParentGrid , _T("���Ϲ߼�") , nParentColIndex );

	CString sTmpLoanKey;
	for( INT i = 0 ; i < pParentDM->GetRowCount() ; i++ )
	{
		ids = pParentDM->GetCellData( _T("����/�ݳ�����KEY") , i , sTmpLoanKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OSetCheckMailSendYN"));

		if( sTmpLoanKey.Compare(sLoanKey)!=0 ) continue;

		ids = pParentDM->SetCellData( _T("���Ϲ߼�") , sYN , i );
		if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("OSetCheckMailSendYN"));

		if( nParentColIndex != -1 )
			pParentGrid->SetAt( i , nParentColIndex , sYN );

		return 0;
	
	}

	return 0;

	EFS_END
	return -1;

}


// �޼��� �����ֱ�
INT CBL_LOC_LOAN_MNG_PROC::ShowMailResultMessage( INT nMailMode , INT nMailSendList , INT nDenyReasonStatus , INT nDenyReasonOrder , INT nDenyReasonWorkingStatus )
{

	return 0;

}

// nReason -> 0:������°� ���ණ�� Ȥ�� ������°� �ƴ϶� , 1:������ 1�� �ƴϱ� ���� , 2:��ġ�ڷᰡ �ƴϱ� ���� 
INT CBL_LOC_LOAN_MNG_PROC::MailAddRecord( INT nMailMode , CLocMailMgrAPI *MailMgr , CESL_DataMgr *pDM , INT nIndex ,  INT &nReason )
{

	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::AddRecordMailReturnPressing( INT nMailMode , CLocMailMgrAPI *MailMgr , CESL_DataMgr *pDM , INT nIndex ,  INT &nReason )
{
	EFS_BEGIN


	return 0;

	EFS_END
	return -1;

}

// ���� �ڷ� �ݳ� Ȯ�� �����̴�.
INT CBL_LOC_LOAN_MNG_PROC::AddRecordMailReserveBookReturn( INT nMailMode , CLocMailMgrAPI *MailMgr , CESL_DataMgr *pDM , INT nIndex ,  INT &nReason )
{
	EFS_BEGIN

	return 0;

	EFS_END
	return -1;

}

// �ݳ� Ȯ�� ���� ������
INT CBL_LOC_LOAN_MNG_PROC::OpenMail(INT nMailMode )
{
	EFS_BEGIN

	return 0;

	EFS_END
	return -1;

}

// �θ� �׸���� ���� ó�� �׸��忡 ���� �߼� ���ο� 'Y'�� üũ�Ѵ�.
INT CBL_LOC_LOAN_MNG_PROC::SetCheckMailSendYN( CString sLoanKey , CString sYN )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3240_BO");
	CString sParentDMAlias = _T("DM_BO_LOC_3200");
	CString sCMAlias = _T("CM_BO_LOC_3240");

	// ���� ���ǰ� �ִ� �׸��� ������
	CESL_Grid *pGrid = GetClientGridPtr();
	if(pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SetCheckMailSendYN") );
	ids = pGrid->SelectMakeList();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SetCheckMailSendYN") );
	// �θ� �׸��� ������
	CESL_Grid *pParentGrid = GetParentGridPtr();
	if(pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SetCheckMailSendYN") );
	INT nRowCount = pGrid->SelectGetCount();
	if( nRowCount == 0 ) return 0;

	// ���� ���Ǵ� DM ������
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5, _T("SetCheckMailSendYN") );
	// �θ� DM ������
	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("OSetCheckMailSendYN"));

	INT nClientColIndex = -1;
	INT nParentColIndex = -1;
	CString  sParentRowIndex;

	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , _T("���Ϲ߼�") , nClientColIndex );
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pParentGrid , _T("���Ϲ߼�") , nParentColIndex );

	CString sTmpLoanKey;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("����/�ݳ�����KEY") , i , sTmpLoanKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OSetCheckMailSendYN"));

		if( sTmpLoanKey.Compare(sLoanKey)!=0 ) continue;

		ids = pDM->GetCellData( _T("�θ�INDEX") , i , sParentRowIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OSetCheckMailSendYN"));

		ids = pDM->SetCellData( _T("���Ϲ߼�") , sYN , i );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("OSetCheckMailSendYN"));

		ids = pParentDM->SetCellData( _T("���Ϲ߼�") , sYN , _ttoi(sParentRowIndex) );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("OSetCheckMailSendYN"));

		if( nClientColIndex != -1 )
			pGrid->SetAt( i , nClientColIndex , sYN );
		if( nParentColIndex != -1 )
			pParentGrid->SetAt( _ttoi(sParentRowIndex) , nParentColIndex , sYN );
	
		return 0;
	}

	return 0;

	EFS_END
	return -1;

}

// ���� �ڷ� ���� �ϰ� ����
INT CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfo()
{
	EFS_BEGIN

	INT ids;

	// �׸��忡 ���õ� ���� ã�Ƴ��� �ϳ��� DB�� UPDATE
	// DM���� UPDATE
	// �׸��忡�� UPDATE
	CESL_Grid *pGrid = GetParentGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ModifyAllLoanInfo") );

	pGrid->SelectMakeList();

	INT nSelectCount = pGrid->SelectGetCount();

	if( nSelectCount < 1 ) 
	{
		ViewMessage( _T("������ �ڷḦ ���� �������ֽʽÿ�") );
		return 0;
	}

	INT nIndex;
	pGrid->SelectGetHeadPosition();
	for( INT i = 0 ; i < nSelectCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		// DB�� ������ , DM�� ������ , �׸��� ������
		ids = ModifyAllLoanInfoProc( nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ModifyAllLoanInfo") );
		if( ids > 0 ) return 1;

		pGrid->SelectGetNext();
	}

	ViewMessage( _T("��� �����Ǿ����ϴ�.") );

	return 0;

	EFS_END
	return -1;

}


// ���� �ڷ� ���� �ϰ� ����
INT CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc( INT &nIndex )
{
	EFS_BEGIN

	INT ids;

	CString sRecKey;
	CString sParentDMAlias;

	ids = GetParentDM( sParentDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc()") );

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc()") );

	CArray<CString,CString&> sChangeDMColumns;
	CArray<CString,CString&> sChangeDBColumns;
	CArray<CString,CString&> sChangeValues;
	CArray<CString,CString&> sValueTypes;
	CArray<CString,CString&> sColIndexs;

	INT nCnt = 0;
	ids = GetChangeColumns( &sChangeDMColumns , &sChangeDBColumns , &sChangeValues , &sValueTypes , &sColIndexs , nCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc()") );

	if( nCnt == 0 ) 
	{
		ViewMessage( _T("������ �� �ִ� �׸��� �������� �ʽ��ϴ�. ") );
		return 1;
	}

	// �����׸� üũ
	INT	i;
	BOOL bChange;
	bChange = FALSE;
	for ( i=0 ; i<nCnt ; i++ )
	{
		if ( !sChangeValues.GetAt(i).IsEmpty() ) 
		{
			bChange = TRUE;
			break;
		}
	}	

	if ( bChange == FALSE )
	{
		ViewMessage( _T("������ �׸��� 1�� �̻� �Է��Ͽ��� �մϴ�.") );
		return 100;
	}

	// DB �۾�
	ids = ModifyAllLoanInfoDBProc( pDM , nIndex , &sChangeDBColumns , &sChangeValues , &sValueTypes , nCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc()") );

	// DM �۾�
	ids = ModifyAllLoanInfoDMProc( pDM , nIndex , &sChangeDMColumns , &sChangeValues , nCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc()") );

	// �׸��� �۾�
	ids = ModifyAllLoanInfoGridProc( nIndex , &sColIndexs , &sChangeValues , nCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc()") );

	return 0;

	EFS_END
	return -1;

}

// ���� �ڷ� ���� �ϰ� ������ UPDATE�� DM,DB �ʵ��� ����� ������ ������ �Լ�
INT CBL_LOC_LOAN_MNG_PROC::GetChangeColumns( CArray<CString,CString&> *sChangeDMColumns , CArray<CString,CString&> *sChangeDBColumns , CArray<CString,CString&> *sChangeValues , CArray<CString,CString&> *sValueTypes , CArray<CString,CString&> *sColIndexs , INT &nCnt )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3260");
	CString sCMAlias = _T("CM_BO_LOC_3260_EDIT_INFO");

	const INT nColumnCnt = 7;
	TCHAR *sColumns[nColumnCnt][5] = 
	{
		{ _T("������") , _T("LOAN_DATE") , _T("������") , _T("NUMERIC") , _T("-1")} , 
		{ _T("�ݳ�������") , _T("RETURN_PLAN_DATE") , _T("�ݳ�������") , _T("NUMERIC") , _T("-1") } , 
		{ _T("�ݳ���") , _T("RETURN_DATE") , _T("�ݳ���") , _T("NUMERIC") , _T("-1") } , 
		{ _T("������") , _T("RESERVATION_DATE") , _T("������") , _T("NUMERIC") , _T("-1")} , 
		{ _T("���ุ����") , _T("RESERVATION_EXPIRE_DATE") , _T("���ุ����") , _T("NUMERIC") , _T("-1")} , 
		{ _T("��ü��") , _T("ARREAR") , _T("��ü��") , _T("NUMERIC") , _T("-1")} , 
		{ _T("����") , _T("REPARATION") , _T("����") , _T("NUMERIC") , _T("-1") } , 
	};

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetChangeColumns") );

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetChangeColumns") );

	CESL_Control_Element *pControlElement;
	BOOL bIsEnable;
	CString sGetData;

	CString sParentCMAlias;
	CString sParentGridAlias;
	GetParentCMAlias( sParentCMAlias );
	GetParentGridAlias( sParentGridAlias );

	CString sCol;
	CString strTime;
	for( INT i = 0 ; i < nColumnCnt ; i++ )
	{
		pControlElement = pCM->FindControlElement( sColumns[i][2] );
		if( pControlElement == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetChangeColumns") );

		bIsEnable = (m_pParentMgr->GetDlgItem( pControlElement->CTRL_ID ))->IsWindowEnabled();

		if( !bIsEnable ) continue;

		ids = pCM->GetControlMgrData( sColumns[i][2] , sGetData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetChangeColumns") );
		sGetData.TrimLeft(); sGetData.TrimRight();

		sChangeDBColumns->Add( CString(sColumns[i][1]) );
		sChangeDMColumns->Add( CString(sColumns[i][0]) );
		sValueTypes->Add( CString(sColumns[i][3]) );

		strTime.Empty();

		// 2007.06.18 add by pdw {{++++++++++++++++++++++++++++++++++++++++++++++++
		// �ð� �߰� ��.
		if(_T("������") == CString(sColumns[i][0]))
		{			
			ids = pDM->GetCellData(_T("����ð�"), 0, strTime );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("GetChangeColumns") );
		}
		else if(_T("�ݳ�������") == CString(sColumns[i][0]))
		{
//			ids = pDM->GetCellData(_T("�ݳ������ð�"), 0, strTime );
//			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GetChangeColumns") );
			strTime.Empty();
		}
		else if(_T("�ݳ���") == CString(sColumns[i][0]))
		{
			ids = pDM->GetCellData(_T("�ݳ��ð�"), 0, strTime );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GetChangeColumns") );
		}
		else if(_T("������") == CString(sColumns[i][0]))
		{
			ids = pDM->GetCellData(_T("����ð�"), 0, strTime );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GetChangeColumns") );
		}
		else if(_T("���ุ����") == CString(sColumns[i][0]))
		{
//			ids = pDM->GetCellData(_T("���ุ��ð�"), 0, strTime );
//			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GetChangeColumns") );
			strTime.Empty();
		}

		// 2009.09.11 ADD BY PWR : �ݳ�������, ���ุ���� �ð� 23:59:59�Է�
		if(_T("�ݳ�������") == CString(sColumns[i][0]) || _T("���ุ����") == CString(sColumns[i][0]))
		{
			if(!sGetData.IsEmpty())
			{
				sGetData = sGetData + _T(" 23:59:59");
			}
		}
		else
		{
			if( strTime.IsEmpty() &&
				!( _T("�ݳ�������") == CString(sColumns[i][0]) || _T("���ุ����") == CString(sColumns[i][0]) ||
				   _T("��ü��") == CString(sColumns[i][0])     || _T("����") == CString(sColumns[i][0]) ) )
				strTime = "00:00:00";
			if( !sGetData.IsEmpty() )
			{
				if( strTime.IsEmpty() )
				{
					sGetData = sGetData;
				}
				else
				{
					sGetData = sGetData + " " + strTime;
				}
			}
		}
		// ----------------------------------------------------------------------}}

		sChangeValues->Add( sGetData );

		INT nColIndex = -1;
		CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , sParentCMAlias  , sParentGridAlias , sColumns[i][0] , nColIndex );
		sCol.Format(_T("%d") , nColIndex );
		sColIndexs->Add( sCol );

		nCnt++;
	}
	

	return 0;

	EFS_END
	return -1;

}

// �ϰ�����
// DB�۾�
INT CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoDBProc( CESL_DataMgr *pDM , INT nIndex , CArray<CString,CString&> *sChangeDBColumns , CArray<CString,CString&> *sChangeValues , CArray<CString,CString&> *sValueTypes , INT nCnt )
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ModifyAllLoanInfoDBProc");

	// Mobile API ����
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );

	pDM->StartFrame();

	pDM->InitDBFieldData();

	if ( m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE�� ����
// 		CString	strWhere, strManageCode;		
// 
// //		strWhere.Format(_T("SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
// //						_T("WHERE ID = '%s'"), m_strUserID );
// //		pDM->GetOneValueQuery( strWhere, strManageCode, FALSE );
// //
// //		if ( strManageCode == _T("") )
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"));		
// //		}
// //		else
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE ='%s'"), strManageCode);		
// //		}
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE"));
// 
// 		pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
		m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	}

	// ---------------------------------
	// ���� ����
	CString sUpdateFields;
	// ---------------------------------

	// 2007.06.18 add by pdw ++++++++++++++++++++++++++++++++++++++++++++
	CString strChangeDate;

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sChangeValues->GetAt(i).IsEmpty() )
			continue;
			
		/*
		"��ü��" , ARREAR
		"����" , REPARATION
		*/
		if( _T("ARREAR") != sChangeDBColumns->GetAt(i) && _T("REPARATION") != sChangeDBColumns->GetAt(i) )
		{
			strChangeDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"),sChangeValues->GetAt(i));
		}
		else
		{
			strChangeDate.Format(_T("%s"),sChangeValues->GetAt(i));
		}

		pDM->AddDBFieldData( sChangeDBColumns->GetAt(i) , sValueTypes->GetAt(i) , strChangeDate );

		// ---------------------------------
		// ���� ����
		if( !sUpdateFields.IsEmpty() ) sUpdateFields += _T(",");
		sUpdateFields += sChangeDBColumns->GetAt(i);
		// ---------------------------------
	}

	// 2009.08.05 ADD BY PWR : LAST_WORK
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�����ڷ����_�ϰ�����"), __WFILE__, __LINE__ ), TRUE );

	CString sRecKey;
	pDM->GetCellData( _T("����/�ݳ�����KEY") , nIndex , sRecKey );

	// 2009.06.09 UPDATE BY PWR : CO_LOAN_TBL�� �̵��� �ڷ��ϼ� �����Ƿ� ���� UPDATE�Ѵ�.
	CString strQuery, strCnt;
	strQuery.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE REC_KEY=%s"), sRecKey);
	pDM->GetOneValueQuery(strQuery, strCnt, FALSE);
	if(0 < _ttoi(strCnt))
	{
		pMobileApi->MakeUpdateFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	}
	else
	{
		pMobileApi->MakeUpdateFrame(_T("CO_LOAN_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	}
	//pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );

	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	pDM->EndFrame();

	// 2010.08.05 ADD BY PWR : ���յ������� �����ڷ���� �ϰ�����
	if(_T("Y") == m_pLoan->m_sIsUnityLoanService)
	{
		CString strGetData;
		pDM->GetCellData(_T("����ȸ������"), nIndex, strGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(_T("Y") == strGetData)
		{
			CString strStatus;
			pDM->GetCellData(_T("�������"), nIndex, strStatus);
			CString strRegNo;
			pDM->GetCellData(_T("��Ϲ�ȣ"), nIndex, strRegNo);
			ConvertLoanStatusCodeToDesc(1, strRegNo, strStatus);
			if(_T("3") != strStatus && _T("4") != strStatus)
			{
				pDM->GetCellData(_T("��������"), nIndex, strGetData);
				if(strGetData == m_pParentMgr->m_pInfo->MANAGE_CODE)
				{
					CESL_DataMgr* pTimeDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3260"));
					if(NULL == pTimeDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					CFrameWriter Writer(m_pParentMgr->m_pInfo);
					Writer.setCommand(_T("IL_K23_LOC_B01_SERVICE"));
					Writer.addRecord();
					CString strRecKey;
					ids = pDM->GetCellData(_T("����/�ݳ�����KEY"), nIndex, strRecKey);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					Writer.addElement(_T("����KEY"), strRecKey);
					Writer.addElement(_T("���⵵����"), m_pParentMgr->m_pInfo->LIB_CODE);
					pDM->GetCellData(_T("�����ڹ�ȣ"), nIndex, strGetData);
					Writer.addElement(_T("���մ����ڹ�ȣ"), strGetData);
					CString strLoanDate, strTime;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3260_EDIT_INFO"), _T("������"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						pTimeDM->GetCellData(_T("����ð�"),0,strTime);
						if(strTime.IsEmpty()) strTime = _T("00:00:00");
						strLoanDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("������"), strLoanDate);
					}
					CString strReturnDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3260_EDIT_INFO"), _T("�ݳ���"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						pTimeDM->GetCellData(_T("�ݳ��ð�"),0,strTime);
						if(strTime.IsEmpty()) strTime = _T("00:00:00");
						strReturnDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("�ݳ���"), strReturnDate);
					}
					CString strReturnPlanDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3260_EDIT_INFO"), _T("�ݳ�������"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						strTime = _T("23:59:59");
						strReturnPlanDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("�ݳ�������"), strReturnPlanDate);
					}
					CString strReservationDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3260_EDIT_INFO"), _T("������"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						pTimeDM->GetCellData(_T("����ð�"),0,strTime);
						if(strTime.IsEmpty()) strTime = _T("00:00:00");
						strReservationDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("������"), strReservationDate);
					}
					CString strReservationExpireDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3260_EDIT_INFO"), _T("���ุ����"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						strTime = _T("23:59:59");
						strReservationExpireDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("���ุ����"), strReservationExpireDate);
					}
					CFrameSender Sender(m_pParentMgr->m_pInfo);
					Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("�����ڷ����-�ϰ�����"), __WFILE__, __LINE__ ));
				}
			}
		}
	}

	// Mobile API delete
	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// ���� ���� ����/�ݳ� ���� , ������ ����� ���� �������� �˸��� 
	// 2003�� 11�� 8�� �۾�
	
	
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;	
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	
	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	
	return 0;

EFS_END
return -1;
}

// �ϰ� ����
// DM �۾�
INT CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoDMProc( CESL_DataMgr *pDM , INT nIndex , CArray<CString,CString&> *sChangeDMColumns , CArray<CString,CString&> *sChangeValues , INT nCnt )
{
	EFS_BEGIN

	INT ids;

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sChangeValues->GetAt(i).IsEmpty() )
			continue;

		ids = pDM->SetCellData( sChangeDMColumns->GetAt(i) ,  sChangeValues->GetAt(i) , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ModifyAllLoanInfoDMProc") );
	}	

	return 0;

	EFS_END
	return -1;

}

// �ϰ� ����
// �׸��� �۾�
INT CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoGridProc( INT nIndex , CArray<CString,CString&> *nColIndexs , CArray<CString,CString&> *sChangeValues , INT nCnt )
{
	EFS_BEGIN

	CESL_Grid *pGrid = GetParentGridPtr();

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( _ttoi(nColIndexs->GetAt(i)) < 0 ) continue;

		if( sChangeValues->GetAt(i).IsEmpty() )
			continue;
		pGrid->SetAt( nIndex , _ttoi(nColIndexs->GetAt(i)) , sChangeValues->GetAt(i) );
	}	

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::GetParentDM( CString &sDMAlias )
{
	EFS_BEGIN

/*	
	CString sPublishForm;

	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("����/���ӱ���") , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetPublishForm") );

	if( sPublishForm.Compare(_T("M")) == 0 )
	{
		sDMAlias = _T("DM_BO_LOC_3200_BO");
	}
	else if( sPublishForm.Compare(_T("N")) == 0 )
	{
		sDMAlias = _T("DM_BO_LOC_3200_NON_DB");
	}
	else if( sPublishForm.Compare(_T("S")) == 0 )
	{
		sDMAlias = _T("DM_BO_LOC_3200_SE");
	}
*/
	sDMAlias = _T("DM_BO_LOC_3200");

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::GetParentCMAlias( CString &sCMAlias )
{
	EFS_BEGIN

/*	
	CString sPublishForm;

	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("����/���ӱ���") , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetPublishForm") );

	if( sPublishForm.Compare(_T("M")) == 0 )
	{
		sCMAlias = _T("CM_BO_LOC_3200");
	}
	else if( sPublishForm.Compare(_T("N")) == 0 )
	{
		sCMAlias = _T("CM_BO_LOC_3200");
	}
	else if( sPublishForm.Compare(_T("S")) == 0 )
	{
		sCMAlias = _T("CM_BO_LOC_3200");
	}
*/
	sCMAlias = _T("CM_BO_LOC_3200");
	return 0;


	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::GetParentGridAlias( CString &sGridAlias )
{
	EFS_BEGIN

/*	
	CString sPublishForm;

	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("����/���ӱ���") , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetPublishForm") );

	if( sPublishForm.Compare(_T("M")) == 0 )
	{
		sGridAlias = _T("MainBOGrid");
	}
	else if( sPublishForm.Compare(_T("N")) == 0 )
	{
		sGridAlias = _T("MainNonDBGrid");
	}
	else if( sPublishForm.Compare(_T("S")) == 0 )
	{
		sGridAlias = _T("MainSEGrid");
	}
*/
	sGridAlias = _T("MainGrid");
	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::GetCMAlias3320( CString &sCMAlias )
{
	EFS_BEGIN

/*
	CString sPublishForm;

	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("����/���ӱ���") , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetPublishForm") );

	if( sPublishForm.Compare(_T("M")) == 0 )
	{
		sCMAlias = _T("CM_BO_LOC_3220");
	}
	else if( sPublishForm.Compare(_T("N")) == 0 )
	{
		sCMAlias = _T("CM_BO_LOC_3220_NON_DB");
	}
	else if( sPublishForm.Compare(_T("S")) == 0 )
	{
		sCMAlias = _T("CM_BO_LOC_3220");
	}
*/

	sCMAlias = _T("CM_BO_LOC_3220");

	return 0;


	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::ViewDetail()
{
	EFS_BEGIN

	CString sParentCMAlias = _T("CM_BO_LOC_3200");
	CString sParentDMAlias = _T("DM_BO_LOC_3200");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::ChangeCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::ChangeCode()") );
	if( pParentDM->GetRowCount() < 0 ) return 0;
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::ChangeCode()") );

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;

	INT nIndex = pGrid->GetIdx();
	pParentDM->GetCellData( _T("��KEY") , nIndex , sSpeciesKey );
	sSpeciesKeyList.AddTail( sSpeciesKey );
	asGridRow.Add( nIndex );

	CString sPublishForm;
	pParentDM->GetCellData( _T("�ڷᱸ��") , nIndex , sPublishForm );

	

	if( sPublishForm.Compare(_T("����")) == 0 )
	{
		CBOBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , 0 );
		Dlg.DoModal();
	}
	else if( sPublishForm.Compare(_T("����")) == 0 )
	{
		CSEBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , 0 );
		Dlg.DoModal();		
	}


	sSpeciesKeyList.RemoveAll();
	asGridRow.RemoveAll();

	return 0;

	EFS_END
	return -1;

}

// ���� �ڷ� ���� ��ο�
INT CBL_LOC_LOAN_MNG_PROC::ReViewOrder()
{
	EFS_BEGIN

	

	CString sDMAlias;

	GetParentDM( sDMAlias );

	ReViewOrder(sDMAlias);

	return 0;

	EFS_END
	return -1;

}

// ���� �ڷ� ���� ��ο�
INT CBL_LOC_LOAN_MNG_PROC::ReViewOrder( CString sDMAlias )
{
	EFS_BEGIN

	// ���� ����
	// �����ڷ� ���� �������� ������ query �ѹ� ������ ���������� ����
	/*
	const INT reservateInfoCnt = 2;
	CString reservateInfoAlias[ reservateInfoCnt ] = {
		_T("����/�ݳ�����KEY"), _T("����åKEY") };
	CString reservateInfoData[ reservateInfoCnt ];
	
	RESERVATE_ORDER_LIST ReservationOrderList;
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	INT DMRowCnt = pDM->GetRowCount();
	for( INT i = 0; i < DMRowCnt; i++ )
		if( 3 == _ttoi( pDM->GetCellData( _T("�������"), i ) ) )	// �˻��� ����/�ݳ� �ڷῡ�� ���°� ������ �ڷḸ 
		{
			pDM->GetCellData( reservateInfoAlias, reservateInfoCnt, reservateInfoData, i );
			// RESERVATE_ORDER_LIST �� �� ���ڵ带 �߰��Ѵ�
			ReservationOrderList.AddReservateInfo( reservateInfoData[ 0 ], reservateInfoData[ 1 ], i );
		}

	// �����ڷᰡ �ϳ��� ���ٸ� �׳� ���
	if( !ReservationOrderList.GetInfoCount() )
		return 0;

	// �����ڷᰡ �ִٸ� DM : DM_BO_LOC_3200 �� �����ڷ� ������ �ִ´�
	if( SetReservationRank( pDM, ReservationOrderList ) )
		return -1;
	*/
	
	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ReViewOrder") );

	CString sStatus;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("�������") , i , sStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ReViewOrder") );

		if( !( (sStatus.Compare(_T("3"))==0) || (sStatus.Compare(_T("����"))==0) || (0 == sStatus.Find(_T("����("))) ) )
			continue;

		ids = ReViewOrder( pDM , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ReViewOrder") );
	}
	
	
	
	return 0;
	
	EFS_END
	return -1;
	
}

// ���� �ڷ� ���� ��ο�
INT CBL_LOC_LOAN_MNG_PROC::ReViewOrder( CESL_DataMgr *pDM , INT nIndex )
{
	EFS_BEGIN

	INT ids = 0;

	CString sBookKey = _T("");
	CString sLoanKey = _T("");
	CString sSpeciesKey = _T("");
	CString sVolInfoKey = _T("");
	CString sOrder = _T("");
	CString sSql = _T("");

	// ���� ���� �����ֱ�
	// ���� : åKEY , ����/�ݳ�����KEY
	ids = pDM->GetCellData( _T("����åKEY") , nIndex , sBookKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 
	ids = pDM->GetCellData( _T("����/�ݳ�����KEY") , nIndex , sLoanKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 
	ids = pDM->GetCellData( _T("��KEY") , nIndex , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 
	ids = pDM->GetCellData( _T("��ȣ����KEY") , nIndex , sVolInfoKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") );

	CString sOrderDMAlias = _T("DM_BO_LOC_3200_RESERVATION_ORDER");

	CESL_DataMgr* pOrderDM = m_pParentMgr->FindDM( sOrderDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ReViewOrder") );
	pOrderDM->FreeData();

	if(sVolInfoKey == _T(""))
	{
		ids = pDM->SetCellData( _T("����") , _T("Not VolInfo") , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-4 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 

		return 0;
	}

	if ( m_pLoan->m_bVolReserve )
	{
		// GET_RESERVATE_RANK 
		// input = ��ȲŰ, SpeciesKey, VolInfoKey
		// output = ?/?/? ( rank/������å����/����ɸ�å�� )
		sSql.Format( _T("SELECT GET_RESERVATE_RANK( %s , %s , %s ) FROM DUAL") , sLoanKey , sSpeciesKey , sVolInfoKey );
		ids = pOrderDM->GetOneValueQuery( sSql , sOrder );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-3 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 
	}
	else
	{
		sSql.Format( _T(" STATUS = '3' AND BOOK_KEY = %s") , sBookKey );
		ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pOrderDM ,sSql );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-3 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 

		CString sOrderLoanKey;
		for( INT i = 0 ; i < pOrderDM->GetRowCount() ; i++ )
		{
			ids = pOrderDM->GetCellData( _T("����/�ݳ�����KEY") , i , sOrderLoanKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ReViewOrder") );

			if( sLoanKey.Compare( sOrderLoanKey ) == 0 )
				sOrder.Format( _T("%d") , i+1 );
		}
	}

	ids = pDM->SetCellData( _T("����") , sOrder , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-4 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 

	return 0;

	EFS_END
	return -1;

}

/// ���� �ݳ����� �ڷ� ó��
/// �׸����� ������ �ڷ��� ���°� ���� �ݳ����� �ڷḸ ����
INT CBL_LOC_LOAN_MNG_PROC::ReturnFromStation()
{
	EFS_BEGIN

	INT ids;
	
	// �׸��忡 ���õ� ���� ã�Ƴ��� �ϳ��� DB�� UPDATE
	// DM���� UPDATE
	// �׸��忡�� UPDATE
	CESL_Grid *pGrid = GetParentGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfo()") );
	
	pGrid->SelectMakeList();
	
	INT nSelectCount = pGrid->SelectGetCount();
	
	if( nSelectCount < 1 ) 
	{
		ViewMessage( _T("������ �ڷḦ ���� �������ֽʽÿ�") );
		return 0;
	}
	
	INT nIndex;
	pGrid->SelectGetHeadPosition();

	INT nReturnCnt = 0;
	for( INT i = 0 ; i < nSelectCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		// DB�� ������ , DM�� ������ , �׸��� ������
		ids = StationReturnProc( nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfo()") );
		
		if( ids == 0 ) nReturnCnt++;
		pGrid->SelectGetNext();
	}
	
	if( nReturnCnt == 0 )
	{
		ViewMessage( _T("ó���� �ڷᰡ �������� �ʽ��ϴ�.") );
		return 0;
	}
	
	if( nReturnCnt == nSelectCount )
	{
		// 2008.06.04 UPDATE BY PWR : ����� �ڷᰡ �ݳ�ó�� �Ǿ������� �� �� �ֵ���
		CString strMsg;
		strMsg.Format( _T("[ %d ]���� �ڷᰡ �ݳ�ó�� �Ǿ����ϴ�."), nReturnCnt );
		ViewMessage( strMsg );
	}
	else
	{
		CString sMsg;
		sMsg.Format( _T("ó���� �ڷ� : %d \r\nó������ ���� �ڷ�(������°� ���ιݳ����� �ƴ��ڷ�) : %d \r\nó���� �ڷ� : %d ") 
			, nSelectCount , nSelectCount-nReturnCnt , nReturnCnt );
		ViewMessage( sMsg );
	}
	
	return 0;
	
	EFS_END
	
	return -1;
}

// 18.09.27 JMJ KOL.RED.2018.008
// å�η� �ڷᰡ ���ιݳ���� �ݳ��� ��� LS_WORK_T01.STATUS�� 1�� �ٲ�����
// ��迡�� �Ϲ��ڷ�ݳ����� ������. ���ϵ��������� å�η��ڷ�� �������� ��û��.
// å�η����� �� å�η� �ݳ��Ǵ� �ڷ�� ILL_TRANS_TBL.APL_LIBCODE �� ���� �ƴϱ� ������
// �̸� Ȯ���Ͽ� R�� ���� 1������ �����Ѵ�.
INT CBL_LOC_LOAN_MNG_PROC::DecideLoanStatus( CString strLoanKey, CString& strLoanStatus )
{
	INT ids;
	CString strGetQuery, strTransNo;
	CESL_DataMgr TmpDM;
	
	strGetQuery.Empty();
	strTransNo.Empty();

	TmpDM.SetCONNECTION_INFO(_T(""));

	// å�η��� ������� �ʴ´ٸ� 1
	if ( m_pLoan->m_sBookCooperativeSys == _T("N") ) strLoanStatus = _T("1");
	// å�η��� ����Ѵٸ� ���� å�η��ڷ����� Ȯ��
	else {
		strGetQuery.Format( _T("SELECT TRANS_NO FROM ILL_TRANS_TBL WHERE LOAN_KEY=%s AND APL_LIBCODE IS NOT NULL"), strLoanKey);
		ids = TmpDM.GetOneValueQuery(strGetQuery, strTransNo);
		if ( ids == -4007 ) strTransNo=_T("");
		else if(0 > ids ) {
			return -100;
		}

		if ( strTransNo.GetLength() > 0 ) strLoanStatus = _T("R");
		else strLoanStatus = _T("1");		
	}	

	return 0;
}

INT CBL_LOC_LOAN_MNG_PROC::StationReturnProc( INT nIndex )
{
EFS_BEGIN

	INT ids;

	CString sDMAlias;
	ids = GetParentDM( sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1, _T("StationReturnProc"));
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -2, _T("StationReturnProc"));

	if ( m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE�� ����
// 		CString	strWhere;		
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE"));
// 		pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
		m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	}

	CString sStatus;
	pDM->GetCellData( _T("�������"), nIndex, sStatus );
	CString sRegNo;
	pDM->GetCellData( _T("��Ϲ�ȣ"), nIndex, sRegNo );
	CString strReservationExpireDate;
	pDM->GetCellData( _T("���ุ����"), nIndex, strReservationExpireDate );
	CString strReserveCancelDate;
	pDM->GetCellData( _T("���������"), nIndex, strReserveCancelDate );
	ConvertLoanStatusCodeToDesc( 1, sRegNo, sStatus );

	if( sStatus != _T("5") )	return 1;
		
	// 1. DB�۾�	
	CString sLoanKey;
	pDM->GetCellData( _T("����/�ݳ�����KEY"), nIndex, sLoanKey );

	// Mobile API ����
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr, pDM );
	pMobileApi->StartFrame();
	
	// 18.09.27 JMJ KOL.RED.2018.008
	ids = DecideLoanStatus(sLoanKey, sStatus);
	if ( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));

	//sStatus = _T("1");
	pMobileApi->InitDBFieldData();
	pMobileApi->AddDBFieldData( _T("STATUS"), _T("STRING"), sStatus );
	pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sLoanKey );

	CString sPublishForm;
	CString sBookKey;
	CString sWorkingStatus;
	CString sPreWorkingStatus;
	CString sBookTableName;
	pDM->GetCellData( _T("�����ڷ����"), nIndex, sPreWorkingStatus );
	pDM->GetCellData( _T("�谡����"), nIndex, sWorkingStatus );
	pDM->GetCellData( _T("����åKEY"), nIndex, sBookKey );
	pDM->GetCellData( _T("�ڷᱸ��"), nIndex, sPublishForm );

	// 2010.08.31 UPDATE BY PWR : ���ιݳ�ó���� ���°� ���� ����(WORKING_STATUS:����, PREV_WORKING_STATUS:���ιݳ���)
	if(_T("����") == sPublishForm)
	{
		sBookTableName = _T("BO_BOOK_TBL");
		sWorkingStatus = _T("BOL112N");
		//KOL.RED.014 JMC ���ιݳ����·� �־��ٰ� ����ݳ�-���κ�����ȭ�鿡�� �����ϸ� ������ ������.
		sPreWorkingStatus = _T("BOL211O");
		//sPreWorkingStatus = _T("BOL216O");
	}
	else if(_T("����") == sPublishForm)
	{
		sBookTableName = _T("SE_BOOK_TBL");
		CLocCommonAPI::GetWorkingStatusDescToCode(sPreWorkingStatus, 0);
		CLocCommonAPI::GetWorkingStatusDescToCode(sWorkingStatus, 0);
		if(_T("SEL112N") == sPreWorkingStatus || _T("SEL112N") == sWorkingStatus )
		{
			sWorkingStatus = _T("SEL112N");
		}
		else
		{
			sWorkingStatus = _T("SEL212N");
		}
		//KOL.RED.014 JMC ���ιݳ����·� �־��ٰ� ����ݳ�-���κ�����ȭ�鿡�� �����ϸ� ������ ������.
		sPreWorkingStatus = _T("SEL311O");
		//sPreWorkingStatus = _T("SEL316O");
	}
	else if(_T("���") == sPublishForm)
	{
		sBookTableName = _T("CO_NON_DB_BOOK_TBL");
		sWorkingStatus = _T("BOL112N");
		//KOL.RED.014 JMC ���ιݳ����·� �־��ٰ� ����ݳ�-���κ�����ȭ�鿡�� �����ϸ� ������ ������.
		sPreWorkingStatus = _T("BOL211O");
		//sPreWorkingStatus = _T("BOL216O");
	}
	else
	{
		sBookTableName = _T("BO_BOOK_TBL");
		sWorkingStatus = _T("BOL112N");
		//KOL.RED.014 JMC ���ιݳ����·� �־��ٰ� ����ݳ�-���κ�����ȭ�鿡�� �����ϸ� ������ ������.
		sPreWorkingStatus = _T("BOL211O");
		//sPreWorkingStatus = _T("BOL216O");
	}	
	pMobileApi->InitDBFieldData();
	// 2010.09.28 ADD BY KSJ : WORKING_STATUS , PREV_WORKING_STATUS �ݴ�� ���� ���� ����
//DEL 	pMobileApi->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), sPreWorkingStatus);
//DEL 	pMobileApi->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), sWorkingStatus);
	pMobileApi->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), sWorkingStatus);
	pMobileApi->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), sPreWorkingStatus);

// 	if( sPublishForm.Compare(_T("����")) == 0 )
// 		sBookTableName = _T("BO_BOOK_TBL");
// 	else if( sPublishForm.Compare(_T("����")) == 0 )
// 		sBookTableName = _T("SE_BOOK_TBL");
// 	else if( sPublishForm.Compare(_T("���")) == 0 )
// 		sBookTableName = _T("CO_NON_DB_BOOK_TBL");
// 	CLocCommonAPI::GetWorkingStatusDescToCode(sPreWorkingStatus ,  0);
// 	CLocCommonAPI::GetWorkingStatusDescToCode(sWorkingStatus ,  0);
// 	pMobileApi->InitDBFieldData();
// 	pMobileApi->AddDBFieldData( _T("WORKING_STATUS"), _T("STRING"), sPreWorkingStatus );
// 	pMobileApi->AddDBFieldData( _T("PREV_WORKING_STATUS"), _T("STRING"), sWorkingStatus );
	pMobileApi->MakeUpdateFrame( sBookTableName, _T("REC_KEY"), _T("NUMERIC"), sBookKey );

	// 2009.03.11 ADD BY CJY : WORKING_STATUS ������ ���� WORKING_STATUS ������Ʈ (����� ��û����)
	CString strSpeciesKey;
	pDM->GetCellData( _T("��KEY"), nIndex, strSpeciesKey );	
	AddIdxQueryFrame(pMobileApi, sPublishForm, strSpeciesKey );

	ids = pMobileApi->SendFrame();
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));
	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	// 2010.10.12 ADD BY KSJ : ����ȸ���ý��� ����
	CString strKLMemberYN;
	pDM->GetCellData( _T("����ȸ������"), nIndex, strKLMemberYN );
	strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
	strKLMemberYN.MakeUpper();
	if(_T("Y")==m_pLoan->m_sIsUnityLoanService && _T("Y")==strKLMemberYN)
	{
		CString strGetData;
		CFrameWriter Writer(m_pParentMgr->m_pInfo);
		Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
		Writer.addRecord();
		ids = pDM->GetCellData(_T("�����ڹ�ȣ"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));
		Writer.addElement(_T("���մ����ڹ�ȣ"), strGetData);
		Writer.addElement(_T("���⵵������ȣ"), m_pParentMgr->m_pInfo->LIB_CODE);
		Writer.addElement(_T("�ݳ���������ȣ"), m_pParentMgr->m_pInfo->LIB_CODE);
		//DEL Writer.addElement(_T("��Ϲ�ȣ"), sRegNo);
		Writer.addElement(_T("����KEY"), sLoanKey);
		ids = pDM->GetCellData(_T("�ݳ���"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));
		Writer.addElement(_T("�ݳ���"), strGetData);
		Writer.addElement(_T("����"), _T("1"));
		ids = pDM->GetCellData(_T("ȸ������"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));
		Writer.addElement(_T("ȸ������"), strGetData);
		ids = pDM->GetCellData(_T("����������"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));
		Writer.addElement(_T("����������"), strGetData);
		ids = pDM->GetCellData(_T("���������ο���"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));
		Writer.addElement(_T("���������ο���"), strGetData);
		CFrameSender Sender(m_pParentMgr->m_pInfo);
		Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("�����ڷ����-���ιݳ�ó��"), __WFILE__, __LINE__));		
	}

	// ���� ���� ����/�ݳ� ���� , ������ ����� ���� �������� �˸��� 
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sLoanKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("STATUS");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;	
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	
	// DM�۾�
	CLocCommonAPI::GetWorkingStatusDescToCode(sPreWorkingStatus,  1);
	CLocCommonAPI::GetWorkingStatusDescToCode(sWorkingStatus,  1);
	
	//ids = ConvertLoanStatusCodeToDesc(strReservationExpireDate, strReserveCancelDate, sStatus );
	//if(ids<0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -7, _T("StationReturnProc") ); 

	ConvertLoanStatusCodeToDesc( 0, sRegNo, sStatus );
	
	pDM->SetCellData(_T("�������"), sStatus, nIndex );
	// KOL.RED.014 ���¿��� ����.
	pDM->SetCellData(_T("�谡����"), sWorkingStatus, nIndex );
	pDM->SetCellData(_T("�����ڷ����"), sPreWorkingStatus, nIndex );
	/*
	pDM->SetCellData(_T("�谡����"), sPreWorkingStatus, nIndex );
	pDM->SetCellData(_T("�����ڷ����"), sWorkingStatus, nIndex );*/

	// �׸��� �۾�	
	const INT nCnt = 3;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("�������") , _T("�谡����") , _T("�����ڷ����")
	};
	
	INT nColIndex[nCnt] = 
	{
		-1 , -1 , -1
	};
	
	CESL_Grid *pGrid = GetParentGridPtr();
	if(pGrid==NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc")); 
	
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr, pGrid, sControlAlias, nCnt, nColIndex );
		
	CString sGetData[nCnt];
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetDataMgrData( sDMAlias , sControlAlias[i] , sGetData[i] , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("StationReturnProc"));
				
		if( nColIndex[i] == -1 ) continue;
		
		ids = pGrid->SetAt( nIndex , nColIndex[i] , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("StationReturnProc"));
	}

	// 2007.05.07 ADD BY PDW : �����ڷ� ó��
	// 1. �����̳� ���Ӹ� �����ϴ�.
	CString strSpeciesFlag;
	CString strQuery;
	CString strTableName;
	CString strBookKey;
	CString strVolInfo;
	CString strRegNo;

	pDM->GetCellData( _T("����/���ӱ���"), nIndex, strSpeciesFlag );
	pDM->GetCellData( _T("����åKEY"), nIndex, strBookKey );
	pDM->GetCellData( _T("��ȣ����KEY"), nIndex, strVolInfo );
	pDM->GetCellData( _T("��Ϲ�ȣ"), nIndex, strRegNo );
	// 2009.05.22 ADD BY PWR : �η��ڷ��̸� ����ó�� ���ϵ��� ����
	CString strAppendixFlag;
	pDM->GetCellData(_T("�ηϿ���"), nIndex, strAppendixFlag);
	if(strAppendixFlag != _T("B"))
	{
		return 0;
	}

	// 2. �����ڷᰡ �ִ��� Ȯ���Ѵ�.
	if( strSpeciesFlag == _T("MO") )
	{
		strTableName = _T("BO_BOOK_TBL");
	}
	else if( strSpeciesFlag == _T("SE") )
	{
		strTableName = _T("SE_BOOK_TBL");
	}
	
	if( !strTableName.GetLength() ) return 0;

	if( !m_pLoan->m_bVolReserve )
	{
		strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, %s BB ")
				         _T("WHERE CL.BOOK_KEY=BB.REC_KEY ")
						 _T("AND BB.REC_KEY=%s AND CL.STATUS='3'"), strTableName, strBookKey );
	}
	else
	{
		strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, %s BB ")
				         _T("WHERE CL.SPECIES_KEY=BB.SPECIES_KEY ")
						 _T("AND BB.SPECIES_KEY=%s ")
						 _T("AND ((CL.VOL_INFO=BB.VOL_SORT_NO) OR (CL.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL)) ")
						 _T("AND CL.STATUS='3'"), strTableName, strSpeciesKey );
	}

	CString strResult;
	ids = pDM->GetOneValueQuery( strQuery, strResult );
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -6, _T("StationReturnProc"));

	if( 0 < _ttoi(strResult) )
	{
		CString strMsg;
		strMsg.Format( _T("[ %s ]�� ����� �ڷ��Դϴ�. ���� ó���� �Ͻðڽ��ϱ�?"), strRegNo );
		if( IDYES == MessageBox( m_pParentMgr->m_hWnd, strMsg, _T("���� ó��"), MB_YESNO|MB_ICONQUESTION ) )
		{
			if( m_pLoan->m_bVolReserve )
			{ // ���ټ��⿹��
				CBO_LOC_3170_ReserveInfo Dlg( m_pParentMgr, m_pLoanShareManager, m_pLoan, 0 );
				Dlg.pMain = m_pParentMgr->pMain;
				Dlg.pParentMgr = (CESL_Mgr*)m_pParentMgr;
				Dlg.m_pInfo = m_pParentMgr->m_pInfo;
				Dlg.SetInfo(strSpeciesKey, strVolInfo, strSpeciesFlag, strBookKey, 1 );
				
				// 2009.02.03 ADD BY KSJ : ���ټ��⿹�� ó����� ����
				Dlg.m_strReturnBookKey = strBookKey;
				// �ݳ� �� å�� REG_NO
				Dlg.m_strReturnBookRegNo = sRegNo;
				// 0:�������, 1:����ó��
				Dlg.m_nReserveInfoMode = 1;
				Dlg.DoModal();
			}
			else
			{ // �Ϲݿ���
				CBO_LOC_3170 Dlg( m_pParentMgr, m_pLoanShareManager, m_pLoan, 0 );
				Dlg.pMain = m_pParentMgr->pMain;
				Dlg.pParentMgr = (CESL_Mgr*)m_pParentMgr;
				Dlg.m_pInfo = m_pParentMgr->m_pInfo;
				Dlg.SetInfo(strSpeciesKey, strVolInfo, strSpeciesFlag, strBookKey, 1 );
				Dlg.DoModal();				
			}
		}
	}

	return 0;

EFS_END
return -1;
}

// ���� �߰�
// �����ڷ� �켱���� ���� �ѹ����� ������!!!
INT CBL_LOC_LOAN_MNG_PROC::SetReservationRank( CESL_DataMgr *pLoanDM, RESERVATE_ORDER_LIST &ReservationOrderList )
{
	CArray< CString, CString >bookKeyList;
	ReservationOrderList.GetBookKeyList( bookKeyList );

	// LH_STORAGE_V01 ���� �˻��� �����ڷ��� BOOK_KEY�� �����ϴ� ��� ���ڵ带 �������� ���� QUERY �� �����.
	// ORDER BY BOOK_KEY, RESERVATION_DATE �̱� ������ ���� BOOK_KEY �� ���� BOOK_KEY �� üũ�����ν� �켱���� �ʱ�ȭ�� �Ѵ�. 
	// ���� ���� ���� �켱������ ���������̴�. 
	CString query = _T("SELECT REC_KEY, BOOK_KEY FROM LS_WORK_T01 ");
	CString where = _T("WHERE STATUS = '3' AND BOOK_KEY IN( ");
	for( INT i = 0; i < bookKeyList.GetSize(); i++ )
		where += ( bookKeyList.GetAt( i ) + _T(", ") );
	if( _T(", ") == where.Right( 2 ) )
		where = where.Left( where.GetLength() - 2 );
	query += ( where + _T(" ) ORDER BY BOOK_KEY, RESERVATION_DATE") );
	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( pLoanDM->CONNECTION_INFO );
	
	dm.RestructDataManager( query );
	INT cnt = dm.GetRowCount();
	INT reservationInfoCnt = ReservationOrderList.GetInfoCount();
	CString loanKey, bookKey, pastBookKey;
	INT rank = 0;
	for( i = 0; i < cnt; i++ )
	{
		loanKey = dm.GetCellData( i, 0 );
		bookKey = dm.GetCellData( i, 1 );
		
		// ���� BOOK_KEY �� ���� BOOK_KEY �� ���� �ʴٸ� �켱������ 1 �� �ʱ�ȭ �ȴ�.
		if( pastBookKey != bookKey )
			rank = 1;
		else
			rank++;	// ��� ���� å�̶�� �켱������ �����Ѵ�. 

		// ������ �˻��� ��� �����ڷ��� ������ŭ ���鼭 ����Ű�� ������ �˻��Ѵ�. 
		for( INT j = 0; j < reservationInfoCnt; j++ )
			// ����Ű�� ���ٸ� ���� �켱������ ����ȴ�. 
			if( loanKey == ReservationOrderList.GetLoanKey( j ) )
			{
				CString sRank;
				sRank.Format( _T("%d"), rank );
				pLoanDM->SetCellData( _T("����"), sRank, ReservationOrderList.GetDMRow( j ) );
			}

		pastBookKey = bookKey;
	}

	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  Ÿ���ݳ��ڷ� ó��
/////////////////////////////////////////////////////////////////////////////
INT CBL_LOC_LOAN_MNG_PROC::ReturnFromOtherManageCode()
{
	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;
	INT		nGridSelectCount;
	INT		nSelectIndex;
	INT		nSuccessCount = 0;

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	CESL_Grid* pMainGrid = GetParentGridPtr();
	if( !pMainGrid ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_LOC_LOAN_MNG_PROC::ReturnFromOtherManageCode()") );

	pMainGrid->SelectMakeList();

	nGridSelectCount = pMainGrid->SelectGetCount();

	if( 1 > nGridSelectCount )
	{
		ViewMessage( _T("������ �ڷḦ ���� �������ֽʽÿ�") );
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	pMainGrid->SelectGetHeadPosition();

	for( INT i=0;i<nGridSelectCount;i++ )
	{
		nSelectIndex = pMainGrid->SelectGetIdx();

		ids = OtherManageCodeReturnProc( nSelectIndex );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_LOC_LOAN_MNG_PROC::ReturnFromOtherManageCode()") );

		if( 0 == ids )
		{
			nSuccessCount++;
		}

		pMainGrid->SelectGetNext();
	}

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	if( 0 == nSuccessCount )
	{
		ViewMessage( _T("ó���� �ڷᰡ �������� �ʽ��ϴ�.") );
		return 0;
	}

	if( nSuccessCount == nGridSelectCount )
	{
		//ViewMessage( _T("��� �����Ǿ����ϴ�.") );

		//++2008.03.16 ADD BY PWR {{+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// �а��ݳ�ó���� "��� �����Ǿ����ϴ�." -> "?���� �ڷᰡ �ݳ�ó�� �Ǿ����ϴ�." �� ����
		CString strMsg;
		strMsg.Format( _T("[ %d ]���� �ڷᰡ �ݳ�ó�� �Ǿ����ϴ�."), nSuccessCount );
		ViewMessage( strMsg );
		//--2008.03.16 ADD BY PWR -----------------------------------------------------------}}
	}
	else
	{
		CString strMsg;
		strMsg.Format( _T("ó���� �ڷ� : %d \r\n")
			           _T("ó������ ���� �ڷ�(������°� �а��ݳ����� �ƴ��ڷ�) : %d \r\n")
					   _T("ó���� �ڷ� : %d "), 
					     nGridSelectCount , nGridSelectCount-nSuccessCount , nSuccessCount );
		ViewMessage( strMsg );
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  Ÿ���ݳ��ڷ� �Ǻ�ó��
//////////////////////////////////////////////////////////////////////
INT CBL_LOC_LOAN_MNG_PROC::OtherManageCodeReturnProc(INT nIndex)
{
	//////////////////////////////////////////////////////////////////
	// Declare Variable
	//////////////////////////////////////////////////////////////////
	INT		ids;
	CString strDMAlias;
	CString strLoanStatus;
	CString strRegNo;
	CString strReservationExpireDate;
	CString strReserveCancelDate;
	CString strLoanKey;

	CString strBookKey;
	CString strPublishForm;	
	CString strWorkingStatus;
	CString strPreWorkingStatus;
	CString strBookTableName;

	CString strFuncName = _T("OtherManageCodeReturnProc");

	//////////////////////////////////////////////////////////////////
	// 1. ������ ����
	//////////////////////////////////////////////////////////////////
	ids = GetParentDM( strDMAlias );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , strFuncName);

	CESL_DataMgr* pDM = m_pParentMgr->FindDM( strDMAlias );
	if( !pDM ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , strFuncName);

	ids = pDM->GetCellData( _T("�������") , nIndex , strLoanStatus );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , strFuncName);

	ids = pDM->GetCellData( _T("��Ϲ�ȣ") , nIndex , strRegNo );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , strFuncName);
	
	ConvertLoanStatusCodeToDesc( 1, strRegNo, strLoanStatus );

	if( _T("6") != strLoanStatus )
	{
		// Ÿ��������°� �ƴ� �ڷ�� ���
		return 1;
	}

	// 2009.10.26 ADD BY PWR : Ÿ�� �̷� �а��ݳ�ó�� �ȵǵ���
	CString strManageCode;
	ids = pDM->GetCellData(_T("��������"), nIndex, strManageCode);
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, strFuncName);
	}
	if(strManageCode != m_pParentMgr->m_pInfo->MANAGE_CODE)
	{
		return 1;
	}

	ids = pDM->GetCellData( _T("���ุ����") , nIndex , strReservationExpireDate );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , strFuncName);			
	
	ids = pDM->GetCellData( _T("���������") , nIndex , strReserveCancelDate );	
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , strFuncName);

	ids = pDM->GetCellData( _T("����/�ݳ�����KEY") , nIndex , strLoanKey );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , strFuncName);

	ids = pDM->GetCellData( _T("�����ڷ����") , nIndex , strPreWorkingStatus );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , strFuncName);
	
	ids = pDM->GetCellData( _T("�谡����") , nIndex , strWorkingStatus );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , strFuncName);

	ids = pDM->GetCellData( _T("����åKEY") , nIndex , strBookKey );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , strFuncName);

	ids = pDM->GetCellData( _T("�ڷᱸ��") , nIndex , strPublishForm );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , strFuncName);

	if( strPublishForm.Compare(_T("����")) == 0 )
	{
		strBookTableName = _T("BO_BOOK_TBL");
	}
	else if( strPublishForm.Compare(_T("����")) == 0 )
	{
		strBookTableName = _T("SE_BOOK_TBL");
	}
	else if( strPublishForm.Compare(_T("���")) == 0 )
	{
		strBookTableName = _T("CO_NON_DB_BOOK_TBL");
	}

	CLocCommonAPI::GetWorkingStatusDescToCode(strPreWorkingStatus ,  0);
	CLocCommonAPI::GetWorkingStatusDescToCode(strWorkingStatus ,  0);

	//////////////////////////////////////////////////////////////////
	// 2. DB Update
	//////////////////////////////////////////////////////////////////
	CKolas2up_MobileApi* pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );
	
	pMobileApi->StartFrame();

	// �������
	strLoanStatus = _T("1");
	pMobileApi->InitDBFieldData();
	pMobileApi->AddDBFieldData( _T("STATUS") , _T("STRING") , strLoanStatus );
	pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey );

	// å����
	pMobileApi->InitDBFieldData();
	pMobileApi->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , strPreWorkingStatus );
	pMobileApi->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , strWorkingStatus );
	pMobileApi->MakeUpdateFrame( strBookTableName , _T("REC_KEY") , _T("NUMERIC") , strBookKey );

	// 2009.03.11 ADD BY CJY : WORKING_STATUS ������ ���� WORKING_STATUS ������Ʈ (����� ��û����)
	CString strSpeciesKey;
	pDM->GetCellData( _T("��KEY"), nIndex, strSpeciesKey );	
	AddIdxQueryFrame(pMobileApi, strPublishForm, strSpeciesKey );

	ids = pMobileApi->SendFrame();
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , strFuncName);

	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	// 2013.02.05 DEL BY PWR : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 14ȸ (���յ������񽺿����� �а��ݳ���ɻ��)
// 	// 2010.10.12 ADD BY KSJ : ����ȸ���ý��� ����
// 	CString strKLMemberYN;
// 	pDM->GetCellData( _T("����ȸ������"), nIndex, strKLMemberYN );
// 	strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
// 	strKLMemberYN.MakeUpper();
// 	if(_T("Y")==m_pLoan->m_sIsUnityLoanService && _T("Y")==strKLMemberYN)
// 	{
// 		CString strGetData;
// 		CFrameWriter Writer(m_pParentMgr->m_pInfo);
// 		Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
// 		Writer.addRecord();
// 		ids = pDM->GetCellData(_T("�����ڹ�ȣ"), nIndex, strGetData);
// 		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
// 		Writer.addElement(_T("���մ����ڹ�ȣ"), strGetData);
// 		Writer.addElement(_T("���⵵������ȣ"), m_pParentMgr->m_pInfo->LIB_CODE);
// 		Writer.addElement(_T("�ݳ���������ȣ"), m_pParentMgr->m_pInfo->LIB_CODE);
// 		//DEL Writer.addElement(_T("��Ϲ�ȣ"), sRegNo);
// 		Writer.addElement(_T("����KEY"), strLoanKey);
// 		ids = pDM->GetCellData(_T("�ݳ���"), nIndex, strGetData);
// 		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
// 		Writer.addElement(_T("�ݳ���"), strGetData);
// 		Writer.addElement(_T("����"), _T("1"));
// 		ids = pDM->GetCellData(_T("ȸ������"), nIndex, strGetData);
// 		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
// 		Writer.addElement(_T("ȸ������"), strGetData);
// 		ids = pDM->GetCellData(_T("����������"), nIndex, strGetData);
// 		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
// 		Writer.addElement(_T("����������"), strGetData);
// 		ids = pDM->GetCellData(_T("���������ο���"), nIndex, strGetData);
// 		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
// 		Writer.addElement(_T("���������ο���"), strGetData);
// 		CFrameSender Sender(m_pParentMgr->m_pInfo);
// 		Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("�����ڷ����-�а��ݳ�ó��"), __WFILE__, __LINE__));		
// 	}

	// ���;��ε�
	if( m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE�� ����
// 		CString	strWhere, strManageCode;		
// 
// //		strWhere.Format(_T("SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL WHERE ID = '%s'"), m_strUserID );
// //
// //		ids = pDM->GetOneValueQuery( strWhere, strManageCode, FALSE );
// //		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CBL_LOC_LOAN_MNG_PROC::OtherManageCodeReturnProc()"));
// //
// //		if ( strManageCode == _T("") )
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"));		
// //		}
// //		else
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE ='%s'"), strManageCode);		
// //		}
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE"));
// 
// 		ids = pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
// 		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("CBL_LOC_LOAN_MNG_PROC::OtherManageCodeReturnProc()"));
		m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	}

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = strLoanKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("STATUS");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;	
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

	//////////////////////////////////////////////////////////////////
	// 3. Display
	//////////////////////////////////////////////////////////////////
	CLocCommonAPI::GetWorkingStatusDescToCode(strPreWorkingStatus ,  1);
	CLocCommonAPI::GetWorkingStatusDescToCode(strWorkingStatus ,  1);

	// 2009.02.24 UPDATE BY PWR : ��������϶��� 
	if(strLoanStatus==_T("4"))
	{
		ids = ConvertLoanStatusCodeToDesc(strReservationExpireDate, strReserveCancelDate, strLoanStatus );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , strFuncName );
	}
	//ids = ConvertLoanStatusCodeToDesc(strReservationExpireDate, strReserveCancelDate, strLoanStatus );
	//if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("CBL_LOC_LOAN_MNG_PROC::OtherManageCodeReturnProc()"));

	ConvertLoanStatusCodeToDesc( 0, strRegNo, strLoanStatus );

	ids = pDM->SetCellData( _T("�������") , strLoanStatus , nIndex );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , strFuncName);
	
	ids = pDM->SetCellData( _T("�谡����") , strPreWorkingStatus , nIndex );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -17 , strFuncName);
	
	ids = pDM->SetCellData( _T("�����ڷ����") , strWorkingStatus , nIndex );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -18 , strFuncName);

	// �׸��� �۾�
	const INT nCnt = 3;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("�������") , _T("�谡����") , _T("�����ڷ����")
	};
	
	INT nColIndex[nCnt] = 
	{
		-1 , -1 , -1
	};
	
	CESL_Grid *pGrid = GetParentGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , strFuncName); 
	
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sControlAlias , nCnt , nColIndex );
		
	CString sGetData[nCnt];
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetDataMgrData( strDMAlias , sControlAlias[i] , sGetData[i] , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , strFuncName);
				
		if( nColIndex[i] == -1 ) continue;
		
		ids = pGrid->SetAt( nIndex , nColIndex[i] , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , strFuncName);
	}
	
	// 2007.05.07 add by pdw {{+++++++++++++++++++++++++++++++++++++++++++++++	
	/*
	�����ڷ� ó��
	*/
	// 1. �����̳� ���Ӹ� �����ϴ�.
	CString strSpeciesFlag;
	CString strQuery;
	CString strTableName;
	CString strBookKey2;
	CString strVolInfo;
	CString strRegNo2;

	pDM->GetCellData( _T("����/���ӱ���"), nIndex, strSpeciesFlag );
	pDM->GetCellData( _T("����åKEY"), nIndex, strBookKey2 );
	pDM->GetCellData( _T("��ȣ����KEY"), nIndex, strVolInfo );
	pDM->GetCellData( _T("��Ϲ�ȣ"), nIndex, strRegNo2 );
	// 2009.05.13 ADD BY PWR : �η��ڷ��̸� ����ó�� ���ϵ��� ����
	CString strAppendixFlag;
	pDM->GetCellData(_T("�ηϿ���"), nIndex, strAppendixFlag);
	if(strAppendixFlag != _T("B"))
	{
		return 0;
	}

	// 2. �����ڷᰡ �ִ��� Ȯ���Ѵ�.
	//    ���� Ȯ�� ���ټ���� �Ϲݿ����� ������ Ʋ����.
	if( strSpeciesFlag == _T("MO") )
	{
		strTableName = _T("BO_BOOK_TBL");
	}
	else if( strSpeciesFlag == _T("SE") )
	{
		strTableName = _T("SE_BOOK_TBL");
	}
	
	
	if( !strTableName.GetLength() ) return 0;

//	{
		if( !m_pLoan->m_bVolReserve )
		{
			strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, %s BB ")
				             _T(" WHERE CL.BOOK_KEY=BB.REC_KEY ")
							   _T(" AND BB.REC_KEY = %s AND CL.STATUS = '3'"), strTableName, strBookKey2 );
		}
		else
		{
			/*
			SELECT COUNT(1) FROM LH_STORAGE_V01 WHERE SPECIES_KEY = SPECIES_KEY 
  AND ( (CL2.VOL_INFO = BB.VOL_SORT_NO) OR (CL2.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL) ) 
  AND STATUS = '3';
			*/
			strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, %s BB ")
				             _T(" WHERE CL.SPECIES_KEY = BB.SPECIES_KEY ")
							   _T(" AND BB.SPECIES_KEY = %s ")
							   _T(" AND ( (CL.VOL_INFO = BB.VOL_SORT_NO) OR (CL.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL) ) ")
							   _T(" AND CL.STATUS ='3'"), strTableName, strSpeciesKey );
		}
//	}

	CString strResult;

	ids = pDM->GetOneValueQuery( strQuery, strResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , strFuncName);

	if( 0 < _ttoi(strResult) )
	{
		CString strMsg;
		strMsg.Format( _T("[ %s ]�� ����� �ڷ��Դϴ�. ���� ó���� �Ͻðڽ��ϱ�?"), strRegNo2 );
		if( IDYES == MessageBox( m_pParentMgr->m_hWnd , strMsg , _T("���� ó��") , MB_YESNO | MB_ICONQUESTION ) )
		{
			if( !m_pLoan->m_bVolReserve )
			{				
				CBO_LOC_3170 Dlg( m_pParentMgr , m_pLoanShareManager , m_pLoan , 0 );
				Dlg.pMain = m_pParentMgr->pMain;
				Dlg.pParentMgr = (CESL_Mgr*)m_pParentMgr;
				Dlg.m_pInfo = m_pParentMgr->m_pInfo;
				Dlg.SetInfo(strSpeciesKey, strVolInfo, strSpeciesFlag, strBookKey2, 1 );
				Dlg.DoModal();
				
			}
			else if( m_pLoan->m_bVolReserve )
			{
				CBO_LOC_3170_ReserveInfo Dlg( m_pParentMgr , m_pLoanShareManager , m_pLoan , 0 );
				Dlg.pMain = m_pParentMgr->pMain;
				Dlg.pParentMgr = (CESL_Mgr*)m_pParentMgr;
				Dlg.m_pInfo = m_pParentMgr->m_pInfo;
				Dlg.SetInfo(strSpeciesKey, strVolInfo, strSpeciesFlag, strBookKey2, 1 );
				Dlg.DoModal();
			}
		}
	}
	// ---------------------------------------------------------------------}}

	return 0;
}

// 2008.06.02 ADD BY PWR
/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  Ÿ���ݳ��ڷ� ó��
/////////////////////////////////////////////////////////////////////////////
INT CBL_LOC_LOAN_MNG_PROC::ReturnFromOtherLibCode()
{
	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;
	INT		nGridSelectCount;
	INT		nSelectIndex;
	INT		nSuccessCount = 0;

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	CESL_Grid* pMainGrid = GetParentGridPtr();
	if( !pMainGrid ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1, _T("CBL_LOC_LOAN_MNG_PROC::ReturnFromOtherManageCode()") );

	pMainGrid->SelectMakeList();

	nGridSelectCount = pMainGrid->SelectGetCount();

	if( 1 > nGridSelectCount )
	{
		ViewMessage( _T("������ �ڷḦ ���� �������ֽʽÿ�") );
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	pMainGrid->SelectGetHeadPosition();

	for( INT i = 0; i < nGridSelectCount; i++ )
	{
		nSelectIndex = pMainGrid->SelectGetIdx();

		ids = OtherLibCodeReturnProc( nSelectIndex );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -2, _T("CBL_LOC_LOAN_MNG_PROC::ReturnFromOtherManageCode()") );

		if( 0 == ids )
		{
			nSuccessCount++;
		}

		pMainGrid->SelectGetNext();
	}

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	if( 0 == nSuccessCount )
	{
		ViewMessage( _T("ó���� �ڷᰡ �������� �ʽ��ϴ�.") );
		return 0;
	}

	if( nSuccessCount == nGridSelectCount )
	{
		CString strMsg;
		strMsg.Format( _T("[ %d ]���� �ڷᰡ �ݳ�ó�� �Ǿ����ϴ�."), nSuccessCount );
		ViewMessage( strMsg );
	}
	else
	{
		CString strMsg;
		strMsg.Format( _T("ó���� �ڷ� : %d \r\n")
			           _T("ó������ ���� �ڷ�(������°� Ÿ���ݳ����� �ƴ��ڷ�) : %d \r\n")
					   _T("ó���� �ڷ� : %d "), 
					     nGridSelectCount , nGridSelectCount-nSuccessCount , nSuccessCount );
		ViewMessage( strMsg );
	}

	return 0;
}

// 2008.06.02 ADD BY PWR
//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  Ÿ���ݳ��ڷ� �Ǻ�ó��
//////////////////////////////////////////////////////////////////////
INT CBL_LOC_LOAN_MNG_PROC::OtherLibCodeReturnProc(INT nIndex)
{
	//////////////////////////////////////////////////////////////////
	// Declare Variable
	//////////////////////////////////////////////////////////////////
	INT		ids;
	CString strDMAlias;
	CString strLoanStatus;
	CString strRegNo;
	CString strReservationExpireDate;
	CString strReserveCancelDate;
	CString strLoanKey;

	CString strBookKey;
	CString strPublishForm;	
	CString strWorkingStatus;
	CString strPreWorkingStatus;
	CString strBookTableName;

	CString strFuncName = _T("OtherLibCodeReturnProc");

	//////////////////////////////////////////////////////////////////
	// 1. ������ ����
	//////////////////////////////////////////////////////////////////
	ids = GetParentDM( strDMAlias );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1010 , strFuncName);

	CESL_DataMgr* pDM = m_pParentMgr->FindDM( strDMAlias );
	if(NULL==pDM) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1020, strFuncName);

	ids = pDM->GetCellData( _T("�������"), nIndex, strLoanStatus );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1030 , strFuncName);

	ids = pDM->GetCellData( _T("��Ϲ�ȣ"), nIndex, strRegNo );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1040 , strFuncName);
	
	ConvertLoanStatusCodeToDesc( 1, strRegNo, strLoanStatus );

	if( _T("7") != strLoanStatus )
	{
		// Ÿ��������°� �ƴ� �ڷ�� ���
		return 1;
	}

	// 2009.10.26 ADD BY PWR : Ÿ�� �̷� Ÿ���ݳ�ó�� �ȵǵ���
	CString strManageCode;
	ids = pDM->GetCellData(_T("��������"), nIndex, strManageCode);
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, strFuncName);
	if(strManageCode != m_pParentMgr->m_pInfo->MANAGE_CODE) return 1;

	ids = pDM->GetCellData( _T("���ุ����"), nIndex, strReservationExpireDate );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1050 , strFuncName);
	
	ids = pDM->GetCellData( _T("���������"), nIndex, strReserveCancelDate );	
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1060 , strFuncName);

	ids = pDM->GetCellData( _T("����/�ݳ�����KEY"), nIndex, strLoanKey );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1070 , strFuncName);

	ids = pDM->GetCellData( _T("�����ڷ����"), nIndex, strPreWorkingStatus );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1080 , strFuncName);
	
	ids = pDM->GetCellData( _T("�谡����"), nIndex, strWorkingStatus );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1090 , strFuncName);

	ids = pDM->GetCellData( _T("����åKEY"), nIndex , strBookKey );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1100, strFuncName);

	ids = pDM->GetCellData( _T("�ڷᱸ��"), nIndex, strPublishForm );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1110 , strFuncName);

	if( strPublishForm.Compare( _T("����") ) == 0 )
	{
		strBookTableName = _T("BO_BOOK_TBL");
	}
	else if( strPublishForm.Compare( _T("����") ) == 0 )
	{
		strBookTableName = _T("SE_BOOK_TBL");
	}
	else if( strPublishForm.Compare( _T("���") ) == 0 )
	{
		strBookTableName = _T("CO_NON_DB_BOOK_TBL");
	}

	CLocCommonAPI::GetWorkingStatusDescToCode(strPreWorkingStatus ,  0);
	CLocCommonAPI::GetWorkingStatusDescToCode(strWorkingStatus ,  0);

	//////////////////////////////////////////////////////////////////
	// 2. DB Update
	//////////////////////////////////////////////////////////////////
	CKolas2up_MobileApi* pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );
	
	pMobileApi->StartFrame();

	// �������
	strLoanStatus = _T("1");
	pMobileApi->InitDBFieldData();
	pMobileApi->AddDBFieldData( _T("STATUS"), _T("STRING"), strLoanStatus );
	pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey );

	// å����
	pMobileApi->InitDBFieldData();
	pMobileApi->AddDBFieldData( _T("WORKING_STATUS"), _T("STRING"), strPreWorkingStatus );
	pMobileApi->AddDBFieldData( _T("PREV_WORKING_STATUS"), _T("STRING"), strWorkingStatus );
	pMobileApi->MakeUpdateFrame( strBookTableName, _T("REC_KEY"), _T("NUMERIC"), strBookKey );

	// 2009.03.11 ADD BY CJY : WORKING_STATUS ������ ���� WORKING_STATUS ������Ʈ (����� ��û����)
	CString strSpeciesKey;
	pDM->GetCellData( _T("��KEY"), nIndex, strSpeciesKey );	
	AddIdxQueryFrame(pMobileApi, strPublishForm, strSpeciesKey );

	ids = pMobileApi->SendFrame();
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1120 , strFuncName);

	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	// 2010.10.12 ADD BY KSJ : ����ȸ���ý��� ����
	CString strKLMemberYN;
	pDM->GetCellData( _T("����ȸ������"), nIndex, strKLMemberYN );
	strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
	strKLMemberYN.MakeUpper();
	if(_T("Y")==m_pLoan->m_sIsUnityLoanService && _T("Y")==strKLMemberYN)
	{
		CString strGetData;
		CFrameWriter Writer(m_pParentMgr->m_pInfo);
		Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
		Writer.addRecord();
		ids = pDM->GetCellData(_T("�����ڹ�ȣ"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
		Writer.addElement(_T("���մ����ڹ�ȣ"), strGetData);
		Writer.addElement(_T("���⵵������ȣ"), m_pParentMgr->m_pInfo->LIB_CODE);
		Writer.addElement(_T("�ݳ���������ȣ"), m_pParentMgr->m_pInfo->LIB_CODE);
		//DEL Writer.addElement(_T("��Ϲ�ȣ"), sRegNo);
		Writer.addElement(_T("����KEY"), strLoanKey);
		ids = pDM->GetCellData(_T("�ݳ���"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
		Writer.addElement(_T("�ݳ���"), strGetData);
		Writer.addElement(_T("����"), _T("1"));
		ids = pDM->GetCellData(_T("ȸ������"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
		Writer.addElement(_T("ȸ������"), strGetData);
		ids = pDM->GetCellData(_T("����������"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
		Writer.addElement(_T("����������"), strGetData);
		ids = pDM->GetCellData(_T("���������ο���"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
		Writer.addElement(_T("���������ο���"), strGetData);
		CFrameSender Sender(m_pParentMgr->m_pInfo);
		Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("�����ڷ����-Ÿ���ݳ�ó��"), __WFILE__, __LINE__));		
	}

	// ���;��ε�
	if( TRUE == m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE�� ����
// 		CString	strWhere, strManageCode;		
// 
// //		strWhere.Format( _T("SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL WHERE ID='%s'"), m_strUserID );
// //
// //		ids = pDM->GetOneValueQuery( strWhere, strManageCode, FALSE );
// //		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -13, _T("CBL_LOC_LOAN_MNG_PROC::OtherManageCodeReturnProc()") );
// //
// //		if ( strManageCode == _T("") )
// //		{
// //			strWhere.Format( _T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL") );		
// //		}
// //		else
// //		{
// //			strWhere.Format( _T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE ='%s'"), strManageCode);		
// //		}
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE"));
// 		ids = pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
// 		if( 0 > ids ) 
// 		{
// 			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1130 , _T("OtherLibCodeReturnProc") );
// 		}
		m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	}

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = strLoanKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("STATUS");				
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;	
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

	//////////////////////////////////////////////////////////////////
	// 3. Display
	//////////////////////////////////////////////////////////////////
	CLocCommonAPI::GetWorkingStatusDescToCode(strPreWorkingStatus ,  1);
	CLocCommonAPI::GetWorkingStatusDescToCode(strWorkingStatus ,  1);

	// 2009.02.24 UPDATE BY PWR : ��������϶��� 
	if(strLoanStatus==_T("4"))
	{
		ids = ConvertLoanStatusCodeToDesc(strReservationExpireDate, strReserveCancelDate, strLoanStatus );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1140 , strFuncName);
	}
	//ids = ConvertLoanStatusCodeToDesc(strReservationExpireDate, strReserveCancelDate, strLoanStatus );
	//if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("CBL_LOC_LOAN_MNG_PROC::OtherManageCodeReturnProc()") );

	ConvertLoanStatusCodeToDesc( 0, strRegNo, strLoanStatus );

	ids = pDM->SetCellData( _T("�������"), strLoanStatus , nIndex );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1150 , strFuncName);
	
	ids = pDM->SetCellData( _T("�谡����") , strPreWorkingStatus , nIndex );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1160 , strFuncName);
	
	ids = pDM->SetCellData( _T("�����ڷ����") , strWorkingStatus , nIndex );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1170 , strFuncName);

	// �׸��� �۾�
	const INT nCnt = 3;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("�������"), _T("�谡����"), _T("�����ڷ����")
	};
	
	INT nColIndex[nCnt] = 
	{
		-1 , -1 , -1
	};
	
	CESL_Grid *pGrid = GetParentGridPtr();
	if(NULL==pGrid) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1180 , strFuncName);
	
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sControlAlias , nCnt , nColIndex );
		
	CString sGetData[nCnt];
	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		ids = m_pParentMgr->GetDataMgrData( strDMAlias , sControlAlias[i] , sGetData[i] , nIndex );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1190 , strFuncName);
				
		if( nColIndex[i] == -1 ) continue;
		
		ids = pGrid->SetAt( nIndex , nColIndex[i] , sGetData[i] );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1200 , strFuncName);
	}
	
	// 2007.05.07 add by pdw {{+++++++++++++++++++++++++++++++++++++++++++++++	
	/*
	�����ڷ� ó��
	*/
	// 1. �����̳� ���Ӹ� �����ϴ�.
	CString strSpeciesFlag;
	CString strQuery;
	CString strTableName;
	CString strBookKey2;
	CString strVolInfo;
	CString strRegNo2;

	pDM->GetCellData( _T("����/���ӱ���"), nIndex, strSpeciesFlag );
	pDM->GetCellData( _T("����åKEY"), nIndex, strBookKey2 );
	pDM->GetCellData( _T("��ȣ����KEY"), nIndex, strVolInfo );
	pDM->GetCellData( _T("��Ϲ�ȣ"), nIndex, strRegNo2 );

	// 2. �����ڷᰡ �ִ��� Ȯ���Ѵ�.
	//    ���� Ȯ�� ���ټ���� �Ϲݿ����� ������ Ʋ����.
	if( _T("MO") == strSpeciesFlag )
	{
		strTableName = _T("BO_BOOK_TBL");
	}
	else if( _T("SE") == strSpeciesFlag )
	{
		strTableName = _T("SE_BOOK_TBL");
	}
	
	if( TRUE != strTableName.GetLength() ) 
	{
		return 0;
	}

//	{
		if( TRUE != m_pLoan->m_bVolReserve )
		{
			strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, %s BB ")
				             _T(" WHERE CL.BOOK_KEY=BB.REC_KEY ")
							 _T(" AND BB.REC_KEY = %s AND CL.STATUS = '3'"), strTableName, strBookKey2 );
		}
		else
		{
			/*
			SELECT COUNT(1) FROM LH_STORAGE_V01 WHERE SPECIES_KEY = SPECIES_KEY 
  AND ( (CL2.VOL_INFO = BB.VOL_SORT_NO) OR (CL2.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL) ) 
  AND STATUS = '3';
			*/
			strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, %s BB ")
				             _T(" WHERE CL.SPECIES_KEY = BB.SPECIES_KEY ")
							 _T(" AND BB.SPECIES_KEY = %s ")
							 _T(" AND ( (CL.VOL_INFO = BB.VOL_SORT_NO) OR (CL.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL) ) ")
							 _T(" AND CL.STATUS ='3'"), strTableName, strSpeciesKey );
		}
//	}

	CString strResult;

	ids = pDM->GetOneValueQuery( strQuery, strResult );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1210 , strFuncName);

	if( 0 < _ttoi(strResult) )
	{
		CString strMsg;
		strMsg.Format( _T("[ %s ]�� ����� �ڷ��Դϴ�. ���� ó���� �Ͻðڽ��ϱ�?"), strRegNo2 );
		if( IDYES == MessageBox( m_pParentMgr->m_hWnd , strMsg , _T("���� ó��") , MB_YESNO | MB_ICONQUESTION ) )
		{
			if( TRUE != m_pLoan->m_bVolReserve )
			{				
				CBO_LOC_3170 Dlg( m_pParentMgr , m_pLoanShareManager , m_pLoan , 0 );
				Dlg.pMain = m_pParentMgr->pMain;
				Dlg.pParentMgr = (CESL_Mgr*)m_pParentMgr;
				Dlg.m_pInfo = m_pParentMgr->m_pInfo;
				Dlg.SetInfo(strSpeciesKey, strVolInfo, strSpeciesFlag, strBookKey2, 1 );
				Dlg.DoModal();
				
			}
			else if( TRUE == m_pLoan->m_bVolReserve )
			{
				CBO_LOC_3170_ReserveInfo Dlg( m_pParentMgr , m_pLoanShareManager , m_pLoan , 0 );
				Dlg.pMain = m_pParentMgr->pMain;
				Dlg.pParentMgr = (CESL_Mgr*)m_pParentMgr;
				Dlg.m_pInfo = m_pParentMgr->m_pInfo;
				Dlg.SetInfo(strSpeciesKey, strVolInfo, strSpeciesFlag, strBookKey2, 1 );
				Dlg.DoModal();
			}
		}
	}
	// ---------------------------------------------------------------------}}

	return 0;
}

/// ������� ���º���
/// nMode : 0  >> ��û���·� ���� 'Y'
///       : 1  >> �������·� ���� 'O'
INT CBL_LOC_LOAN_MNG_PROC::ChangeNightLoanStatus(INT nMode)
{
EFS_BEGIN

	INT ids;
	
	CESL_Grid *pGrid = GetParentGridPtr();
	if( NULL == pGrid )
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("ChangeNightLoanStatus") );
	}

	pGrid->SelectMakeList();
	
	INT nSelectCount = pGrid->SelectGetCount();

	if( nSelectCount < 1 ) 
	{
		ViewMessage( _T("������ �ڷḦ ���� �������ֽʽÿ�") );
		return 0;
	}

	INT nIndex;
	pGrid->SelectGetHeadPosition();

	INT		i , nReturnCnt = 0;
	for( i=0 ; i<nSelectCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		// DB�� ������ , DM�� ������ 
		ids = ChangeNightLoanStatusProc( nMode, nIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("ChangeNightLoanStatus") );
		}
		
		if( 0 == ids ) 
		{
			nReturnCnt++;
		}

		pGrid->SelectGetNext();
	}
	
	if( 0 == nReturnCnt )
	{
		ViewMessage( _T("ó���� �ڷᰡ �������� �ʽ��ϴ�.") );
		return 0;
	}
	
	if( nReturnCnt == nSelectCount )
	{
		ViewMessage( _T("��� �����Ǿ����ϴ�.") );
	}
	else
	{
		CString sMsg;
		sMsg.Format( _T("ó���� �ڷ� : %d \r\nó������ ���� �ڷ�(�߰����⿹���� �ƴ� �ڷ�) : %d \r\nó���� �ڷ� : %d ") 
			, nSelectCount , nSelectCount-nReturnCnt , nReturnCnt );
		ViewMessage( sMsg );
	}
	
	return 0;
	
EFS_END
return -1;
}

INT CBL_LOC_LOAN_MNG_PROC::ChangeNightLoanStatusProc(INT nMode, INT nIndex)
{
EFS_BEGIN

	INT ids;

	CString sDMAlias;
	ids = GetParentDM( sDMAlias );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("ChangeNightLoanStatusProc"));
	}
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("ChangeNightLoanStatusProc"));
	}

	CString sStatus;
	pDM->GetCellData(_T("�߰����⿹��") , nIndex , sStatus );
	if( _T("N") == sStatus )
	{
		return 1;	
	}

	// 1. DB�۾�
	
	CString sLoanKey;

	pDM->GetCellData( _T("����/�ݳ�����KEY") , nIndex , sLoanKey );

	// Mobile API ����
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );
	
	pMobileApi->StartFrame();

	if( nMode == 0 )
	{
		sStatus = _T("Y");
	}
	else
	{
		sStatus = _T("O");
	}

	pMobileApi->InitDBFieldData();
	pMobileApi->AddDBFieldData( _T("NIGHT_RESERVATION_LOAN") , _T("STRING") , sStatus );
	pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01") , _T("REC_KEY") , _T("NUMERIC") , sLoanKey );
	ids = pMobileApi->SendFrame();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE(  m_pParentMgr->m_hWnd , -1030 , _T("ChangeNightLoanStatusProc"));
	}
	
	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	pDM->SetCellData(_T("�߰����⿹��") , sStatus , nIndex );
	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_MNG_PROC::AddIdxQueryFrame( CKolas2up_MobileApi *pMobileApi, CString strPublishFormCode, CString sSpeciesKey )
{
EFS_BEGIN

	/// �̵��������� ���� ���� �ʴ´�.
	if( ( 10000 == m_pParentMgr->m_pInfo->MODE ) || ( 30000 == m_pParentMgr->m_pInfo->MODE ) )
	{
		return 0;
	}

	CString sIDXTblName, sProcName;

	if( _T("����") == strPublishFormCode || _T("MO") == strPublishFormCode)
	{
		sIDXTblName = _T("IDX_BO_TBL");			
		sProcName = _T("TESTWORKSTATUS");
	}
	else if( _T("����") == strPublishFormCode || _T("SE") == strPublishFormCode)
	{
		sIDXTblName = _T("IDX_SE_TBL");			
		sProcName = _T("TESTSEWORKSTATUS");
	}
	else
	{
		return -1;
	}

	CString sQuery;
	sQuery.Format( _T("UPDATE %s SET WORKING_STATUS = %s(%s) WHERE REC_KEY = %s;") ,sIDXTblName, sProcName , sSpeciesKey , sSpeciesKey);	
	pMobileApi->AddFrame( sQuery );

	return 0;

EFS_END
return -1;
}