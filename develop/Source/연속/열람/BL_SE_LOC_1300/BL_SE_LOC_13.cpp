// BL_SE_LOC_13.cpp: implementation of the CBL_SE_LOC_13 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BL_SE_LOC_13.h"

// SanFile�� �����ϱ� ����
#include "..\..\..\����\����\�弭����\OpenSanFile\\InputSanFileDlg.h"

// �̵� �������� ����
#include "Kolas2up_MobileApi.h"

// �ε��� ��Ƴ��� ����
#include "..\..\..\����\K2UP_Common\MakeSearchData.h"


//#include _T("..\\..\\..\\����\\Common_Data_Mgr\\Data_Mgr.h")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBL_SE_LOC_13::CBL_SE_LOC_13( CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	m_pLocCommonAPI = new CLocCommonAPI(m_pParentMgr);
}

CBL_SE_LOC_13::~CBL_SE_LOC_13()
{
	if( m_pLocCommonAPI != NULL ) delete m_pLocCommonAPI;
	m_pLocCommonAPI = NULL;
}

INT CBL_SE_LOC_13::SPFExcutePath( CString sPathAlias )
{
	EFS_BEGIN

	INT ids;
	if( sPathAlias.Compare( _T("�谡�ڷ����_�˻�") ) == 0 )
	{
		ids = SearchSE_LOC_1310();
		if( ids == 1 ) return -1;
	}
	if( sPathAlias.Compare( _T("RFID�˻�") ) == 0 )
	{
		ids = SearchSE_LOC_1310FromRFID();
		if( ids == 1 ) return -1;
	}	
	else if( sPathAlias.Compare( _T("�谡�ڷ����_���Ϸΰ˻�") ) == 0 )
	{
		ids = SearchFileSE_LOC_1310();
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( _T("�谡�ڷ����_�ڵ��ϰ�����") ) == 0 )
	{
		ids = ChangeCode();
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( _T("�谡�ڷ����_���Ǻ���") ) == 0 )
	{
		ids = ChangeShelf(0);
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( _T("�谡�ڷ����_�ϰ�����") ) == 0 )
	{
		ids = ChangeShelf(1);
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( _T("�谡�ڷ����_�̿����ѱ���������") ) == 0 )
	{
		ids = ChangeSpeciesCode();
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( _T("�谡�ڷ����_�󼼺���") ) == 0 )
	{
		ids = DetailView();
		if( ids == 1 ) return -1;
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


//-----------------------------------------------------------------------------------------
// �̰����ʹ� �谡 ������ ���Ǵ� �Լ���
//-----------------------------------------------------------------------------------------
INT CBL_SE_LOC_13::SearchSE_LOC_1310()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1300");
	CString sCMAlias = _T("CM_SE_LOC_1300");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );

	// ������ ����
	CString sWhere;
	ids = GetSqlSE_LOC_1310( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );
	
//	ids = pDM->RefreshDataManager( sWhere );
	
	DWORD START, FINISH;
	START = GetTickCount();
	
	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );

	FINISH = GetTickCount();
	CString log;
	log.Format( _T("[ END ] - ����, �谡���ڱ�� [ Time = %d.%03d sec ]"), ( FINISH - START ) / 1000, ( FINISH - START ) % 1000 ); 
	pDM->QueryLog( log.GetBuffer( 0 ) );
	
	
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );

	INT nRowCount = pDM->GetRowCount();

	CString sMsg;
	sMsg.Format( _T("å�� : %d") , nRowCount );
	m_pParentMgr->GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)sMsg.GetBuffer(0));

	ids = GridDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );

	return 0;

	EFS_END
	return -1;

}


INT CBL_SE_LOC_13::SearchSE_LOC_1310FromRFID()
{
	EFS_BEGIN
		
	INT ids;
	
	CString sDMAlias = _T("DM_SE_LOC_1300");
	CString sCMAlias = _T("CM_SE_LOC_1300");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );
	
	// ������ ����
	CString sWhere;
	GetTempData( _T("WHERE") , sWhere );
	
	//	ids = pDM->RefreshDataManager( sWhere );
	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );
	
	INT nRowCount = pDM->GetRowCount();
	if( nRowCount == 0 )
		AfxMessageBox( _T("�˻��� �ڷᰡ �������� �ʽ��ϴ�.") );
	CString sMsg;
	sMsg.Format( _T("å�� : %d") , nRowCount );
	m_pParentMgr->GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)sMsg.GetBuffer(0));
	
	ids = GridDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );
	
	return 0;
	
	EFS_END
	return -1;
	
}


/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-14 ~ 2005-12-14
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   �Լ�����   ] 
      [����-����] �弭���� -�˻�ȭ�� �Է� ������ ������ �����

   [  PARAMETERS  ]
      sWhere : [OUT] ������
   [ RETURN VALUE ]
      0    : ���� 
      ���� : ����

   [   �̴Ͻ���   ]
      1. �˻�ȭ�鿡 �Էµ� �� �������� 
	  2. �⺻ ������ �����ϱ� 	                      
	  3. �߰� ������ �����ϱ� 
--------------------------------------------------------------------------------------------*/
INT CBL_SE_LOC_13::GetSqlSE_LOC_1310( CString &sWhere )
{	
EFS_BEGIN

	INT ids;

	FILE *fp = _tfopen( _T("..\\CFG\\_newecosearch"), _T("rb") );
	
	CStringList whereList;
	CString sBuf;	

	// ----------------------------------------------------------------------------
	// 1. �˻�ȭ�鿡 �Էµ� �� �������� 
	// ----------------------------------------------------------------------------
	const INT nCtrlCnt = 29;
	CString sCMAlias = _T("CM_SE_LOC_1310");
	CString sCtrlAlias[ nCtrlCnt + 1 ] =
	{
		_T("�谡����")  , _T("�ڷ�Ǳ���")   , _T("�̿����ѱ���")      , _T("�谡�������")      ,_T("��ϱ���") , 
		_T("��Ϲ�ȣ1") , _T("��Ϲ�ȣ2")    , _T("��ġ��ȣ1")         , _T("��ġ��ȣ1")         , _T("�з���ȣ����") , 
		_T("�з���ȣ1") , _T("�з���ȣ2")    , _T("�谡����1")         , _T("�谡����2")         ,_T("�����1") , 
		_T("�����2")   , _T("�ڵ庯�汸��") , _T("�谡���º�������1") , _T("�谡���º�������2") , _T("��ǥ��") , 
		_T("������")    , _T("������")       , _T("��������")              , _T("�̴���Ⱓ1")           , _T("�̴���Ⱓ2") , 
		_T("������ȣ1")     , _T("������ȣ2")        , _T("��������")              , _T("�˻�����")              , _T("�谡������") /*PDW �߰�*/
	};

	CString sGetData[ nCtrlCnt + 1 ];

	for( INT i = 0 ; i < nCtrlCnt + 1; i++ )
	{		
		INT nComboColumnIndex = 0;
		// 2005-12-15 UPD BY PDW(Jr) ==================================================
		// �ڷ�Ǳ��� �ڵ� ������ġ ���� 
		/* ������
		if( i == 1 || i==2 || i==9)
			nComboColumnIndex = 1;
		*/
		if( i==2 )
			nComboColumnIndex = 1;
		// ============================================================================

		ids = m_pParentMgr->GetControlData( sCMAlias , sCtrlAlias[i] , sGetData[i] , -1 , nComboColumnIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );

		sGetData[i].TrimRight();
	}
	
	// ----------------------------------------------------------------------------
	// 2. �⺻ ������ �����ϱ� 
	// ----------------------------------------------------------------------------
	// �谡����
	CString sShelfState;
	if( !sGetData[0].IsEmpty() && (sGetData[0].Compare(_T("�������") )!=0) )
	{
		if( sGetData[0].Compare(_T("�����ڷ�"))==0 )
		{
			GetWorkingStatusDescToCode( sGetData[0] ,0);
			sWhere += _T(" BB.WORKING_STATUS IN (") + sGetData[0] + _T(")");
		}
		else
		{
			GetWorkingStatusDescToCode( sGetData[0] ,0);
			sWhere += _T(" BB.WORKING_STATUS IN ('") + sGetData[0] + _T("')");
		}

	}
	else
	{
		// 2009.09.16 UPDATE BY KSJ : å ���� �������� ���Ͽ� ���� 
		//DEL sWhere += _T(" BB.WORKING_STATUS IN (") + CString(HOLD_SE_SEARCH_CONDITION) + _T(", 'SEL312O') ");
		sWhere += _T(" BB.WORKING_STATUS IN (") + CString(HOLD_SE_SEARCH_CONDITION) + _T(", 'SEL317O') ");
	}

	// å/�η� �÷���
	if( !sGetData[ 28 ].IsEmpty() && sGetData[ 28 ].Compare( _T("��ü") ) != 0 )
	{
		if( !sGetData[ 28 ].Compare( _T("å") ) )
			sWhere += _T(" AND  BB.BOOK_TYPE != 'A' ");
		else if( !sGetData[ 28 ].Compare( _T("�η�") ) )
			sWhere += _T(" AND  BB.BOOK_TYPE = 'A' ");
	}
	
	
	// �谡������
	if( !sGetData[29].IsEmpty() && (sGetData[29].Compare(_T("�������") )!=0) )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T("( BB.SHELF_WORKER = '")+ sGetData[29]+ _T("' )"); 
	}

	// 2005-12-22 REM BY PDW(JR) ===========================================================
	// ���ŵ� 
	/*
	// �⺻���� ���� �谡 ���°� �ǽð� �谡 �ڷ��ΰ��� �˻��� ���� �ʰ� �Ѵ�.
	if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
	sWhere += _T(" BB.PREV_WORKING_STATUS NOT IN ('SEL112N') ");
	*/
	// =====================================================================================

	// �谡��
	sGetData[1].TrimRight();
	if( (!sGetData[1].IsEmpty()) && (sGetData[1].Compare(_T("�������") )!=0))
	//if( (sGetData[1].Compare( _T("0") ) != 0) && (!sGetData[1].IsEmpty()) && (sGetData[1].Compare(_T("�������") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.SHELF_LOC_CODE = '") + sGetData[1] + _T("'");
	}

	//�̿����ѱ���
	sGetData[2].TrimRight();
	if( !sGetData[2].IsEmpty() && (sGetData[2].Compare(_T("�������") )!=0))
	//if( !sGetData[2].IsEmpty() && (sGetData[2].Compare(_T("0"))!=0) && (sGetData[2].Compare(_T("�������") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.USE_LIMIT_CODE = '") + sGetData[2] + _T("'");
	}		
	
	// ��Ϲ�ȣ���� + ��Ϲ�ȣ ~
	sGetData[4].TrimRight();
	if( !sGetData[4].IsEmpty() && (sGetData[4].Compare(_T("�������") )!=0))
	//if( !sGetData[4].IsEmpty() && (sGetData[4].Compare(_T("0"))!=0) && (sGetData[4].Compare(_T("�������") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[5].IsEmpty() )
		{
			CString sRegNo1 = sGetData[4]+sGetData[5];
			CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo1 );
			if( !sGetData[6].IsEmpty() )
			{
				CString sRegNo2 = sGetData[4]+sGetData[6];
				CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo2 );
				sWhere += _T(" BB.REG_NO BETWEEN '")+ sRegNo1 + _T("' AND '")+ sRegNo2 +_T("' ");
			}
			else
			{
				sWhere += _T(" BB.REG_NO = '")+ sRegNo1+_T("' "); 
			}
		}
		else
		{
			sWhere += _T("1=1");
//			sWhere += _T(" BB.REG_CODE = '")+ sGetData[4] +_T("' ");
		}
	}
	


	// ���ʹ谡���� ~
	if( !sGetData[12].IsEmpty() && (sGetData[12].Compare(_T("�������") )!=0) )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[13].IsEmpty() )
		{
			sWhere += _T("( (BB.SHELF_DATE BETWEEN to_date('")+ sGetData[12]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[13]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') )");
			sWhere += _T(" OR ");
			sWhere += _T(" (BB.SHELF_CHANGE_DATE BETWEEN to_date('")+ sGetData[12]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[13]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ) )");
		}
		else
		{
			sWhere += _T("( (BB.SHELF_DATE BETWEEN to_date('")+ sGetData[12]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[12]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') )");
			sWhere += _T(" OR ");
			sWhere += _T(" (BB.SHELF_CHANGE_DATE BETWEEN to_date('")+ sGetData[12]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[12]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ) )");
		}
	}

	// ��������
	// 16/04/12 ������: [�ڶ�_��ɰ���]�谡�ڷ����_��ȸ�� �������������� ����
	// %KOLASIII%\cfg\Reference\DM\DM_BO_LOC_1300.DM���� ������ �����Ͽ� �Ʒ��� ������ �ʿ����� ����
	// IB.MANAGE_CODE=UDF_MANAGE_CODE ->  BB.MANAGE_CODE=UDF_MANAGE_CODE�� ����
/*// BEFORE -----------------------------------------------------------------------------------------
	sGetData[22].TrimRight();
	if( (!sGetData[22].IsEmpty()) && (sGetData[22].Compare(_T("�������") )!=0))
	//if( (sGetData[22].Compare( _T("0") ) != 0) && (!sGetData[22].IsEmpty()) && (sGetData[22].Compare(_T("�������") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.MANAGE_CODE = '") + sGetData[22] + _T("'");
	}
*/// AFTER -----------------------------------------------------------------------------------------
	/*
	sGetData[22].TrimRight();
	if( (!sGetData[22].IsEmpty()) && (sGetData[22].Compare(_T("�������") )!=0))
	//if( (sGetData[22].Compare( _T("0") ) != 0) && (!sGetData[22].IsEmpty()) && (sGetData[22].Compare(_T("�������") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.MANAGE_CODE = '") + sGetData[22] + _T("'");
	}
	*/
//*/ END -------------------------------------------------------------------------------------------

	// ����� ~ 
	if( !sGetData[14].IsEmpty() && (sGetData[14].Compare(_T("�������") )!=0) )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[15].IsEmpty() )
		{
			sWhere += _T(" V.PUBLISH_DATE BETWEEN to_date('")+ sGetData[14]+ _T("/01/01 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[15]+_T("/12/31 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
		else
		{
			sWhere += _T(" V.PUBLISH_DATE BETWEEN to_date('")+ sGetData[14]+ _T("/01/01 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[14]+_T("/12/31 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
	}

	// �ڵ� ���� ����
	sGetData[16].TrimRight();
	if( !sGetData[16].IsEmpty() && (sGetData[16].Compare( _T("�������") ) != 0 ))
	{
		CString sDBColumnName;
		if( sGetData[16].Compare( _T("�谡����") ) == 0 ) sDBColumnName = _T("LAST_SHELF_STATUS_CHANGE_DATE");
		else if( sGetData[16].Compare( _T("�̿����ѱ���") ) == 0 ) sDBColumnName = _T("LAST_USELIMITCODE_CHANGE_DATE");
		else if( sGetData[16].Compare( _T("��������") ) == 0 ) sDBColumnName = _T("LAST_MANAGE_CODE_CHANGE_DATE");
		else if( sGetData[16].Compare( _T("�ڷ��") ) == 0 ) sDBColumnName = _T("SHELF_CHANGE_DATE");

		if( !sGetData[17].IsEmpty() )
		{
			if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
			if( !sGetData[18].IsEmpty() )
			{
				sWhere += _T(" BB.") + sDBColumnName + _T(" BETWEEN  to_date('")+ sGetData[17]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[18]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
			}
			else
			{
				sWhere += _T(" BB.") + sDBColumnName + _T(" BETWEEN  to_date('")+ sGetData[17]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[17]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
			}
		}
	}

	// ----------------------------------------------------------------------------
	// 3. �߰� ������ �����ϱ� 
	//    �˻� �������� ��尡 _T("�����˻�") ����� ��츸 �˻� ���ǿ� �߰�
	// ----------------------------------------------------------------------------	
	CString sModeAlias;
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );

	CESL_Control_Element *pCE = pCM->FindControlElement( _T("��������") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );

	(m_pParentMgr->GetDlgItem( pCE->CTRL_ID ))->GetWindowText( sModeAlias );
	if( sModeAlias.Compare( _T("�󼼰˻�") ) == 0 ) return 0;

	// ��ġ��ȣ
	sGetData[7].TrimRight();
	if( !sGetData[7].IsEmpty() && (sGetData[7].Compare(_T("�������") )!=0))
	//if( !sGetData[7].IsEmpty() && (sGetData[7].Compare(_T("0"))!=0) && (sGetData[7].Compare(_T("�������") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.SEPARATE_SHELF_CODE = '")+ sGetData[7]+_T("' "); 
	}

	// �з���ȣ���� 
	sGetData[9].TrimRight();
	if( !sGetData[9].IsEmpty() && (sGetData[9].Compare(_T("�������") )!=0))
	//if( !sGetData[9].IsEmpty() && (sGetData[9].Compare(_T("0"))!=0) && (sGetData[9].Compare(_T("�������") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.CLASS_NO_TYPE = '") + sGetData[9] + _T("'");
	}
	
	// �谡�������
	if( !sGetData[3].IsEmpty() && (sGetData[3].Compare(_T("�������") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.SHELF_CHANGE_REASON_CODE = '") + sGetData[3] + _T("' ");
	}

	// �з���ȣ ~
	if( !sGetData[10].IsEmpty() && (sGetData[10].Compare(_T("�������") )!=0) )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[11].IsEmpty() )
		{
			sWhere += _T(" BB.CLASS_NO BETWEEN '")+ sGetData[10]+ _T("' AND '")+sGetData[11]+_T("' ");
		}
		else
		{
			sWhere += _T(" BB.CLASS_NO = '")+ sGetData[10]+_T("' "); 
		}
	}

	// ������ȣ
	if( !sGetData[25].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[25].IsEmpty() )
		{
			sWhere += _T(" BB.BOOK_CODE BETWEEN '")+ sGetData[25]+ _T("' AND '")+sGetData[26]+_T("' ");
		}
		else
		{
			sWhere += _T(" BB.BOOK_CODE = '")+ sGetData[25]+_T("' "); 
		}
	}

	// ������ȣ
	if( sGetData[27].Compare( _T("�������") ) != 0 )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( sGetData[27].Compare( _T("����") ) == 0 )
		{
			sWhere += _T(" ( BB.COPY_CODE = '0' OR BB.COPY_CODE IS NULL ) ");
		}
		else
		{
			sWhere += _T(" ( BB.COPY_CODE != '0' AND BB.COPY_CODE IS NOT NULL ) ") ;
		}

	}

	CMakeSearchData MakeSearchData(m_pParentMgr);

	/// 2003�� 9�� 1�� CATSEARCH -> ECOSEARCH ��ȯ , �۾��� : ������
	bool bIsEqualSearch;
	// ��ǥ��
	if( !sGetData[19].IsEmpty() )
	{
		bIsEqualSearch = FALSE;
		// ���ô� ������ �˻� ����
		if( sGetData[19].GetAt(0) == '\"' )
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sGetData[19]);
		if( !bIsEqualSearch ) 
			sGetData[19] += _T("*");
		
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		//sWhere += _T(" IB.TITLE LIKE '") + sGetData[19] + _T("%'");

			
		if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		{
			if( fp )
				sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_TITLE, '") + sGetData[19] + _T("') ");
			else
			{
				sWhere = sWhere.Left( sWhere.GetLength() - 4 );
				sBuf.Format( _T(" &ECOSEARCH(, SE_IDX_TITLE, '%s' ) "), sGetData[ 19 ] );
				whereList.AddTail( sBuf );
			}
		}
		else
			sWhere += _T(" CATSEARCH( IB.IDX_TITLE, '") + sGetData[19] + _T("',NULL) > 0 ");


/*
#ifdef _ECOSEARCH_
		sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_TITLE, '") + sGetData[19] + _T("') ");
#endif
#ifndef _ECOSEARCH_
		sWhere += _T(" CATSEARCH( IB.IDX_TITLE, '") + sGetData[19] + _T("',NULL) > 0 ");
#endif	
		*/
	}
	// ������
	if( !sGetData[20].IsEmpty() )
	{
		bIsEqualSearch = FALSE;
		// ���ô� ������ �˻� ����
		if( sGetData[20].GetAt(0) == '\"' )
			bIsEqualSearch = TRUE;
		MakeSearchData.GetKeyWord(sGetData[20]);
		
		if( !bIsEqualSearch ) 
			sGetData[20] += _T("*");
		
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		{
			if( fp )
				sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_AUTHOR, '") + sGetData[20] + _T("') ");
			else
			{
				sWhere = sWhere.Left( sWhere.GetLength() - 4 );
				sBuf.Format( _T(" &ECOSEARCH( , SE_IDX_AUTHOR, '%s' ) "), sGetData[ 20 ] );
				whereList.AddTail( sBuf );
			}
		}
		else
			sWhere += _T(" CATSEARCH( IB.IDX_AUTHOR, '") + sGetData[20] + _T("',NULL) > 0 ");

/*
#ifdef _ECOSEARCH_
		sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_AUTHOR, '") + sGetData[20] + _T("') ");
#endif
		//sWhere += _T(" IB.AUHOTR LIKE '") + sGetData[20] + _T("%'");
#ifndef _ECOSEARCH_
		sWhere += _T(" CATSEARCH( IB.IDX_AUTHOR, '") + sGetData[20] + _T("',NULL) > 0 ");
#endif
		*/

	}
	// ������
	if( !sGetData[21].IsEmpty() )
	{
		bIsEqualSearch = FALSE;
		// ���ô� ������ �˻� ����
		if( sGetData[21].GetAt(0) == '\"' )
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sGetData[21]);
		if( !bIsEqualSearch ) 
			sGetData[21] += _T("*");
		
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		{
			if( fp )
				sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_PUBLISHER , '") + sGetData[21] + _T("') ");
			else
			{
				sWhere = sWhere.Left( sWhere.GetLength() - 4 );
				sBuf.Format( _T(" &ECOSEARCH( , SE_IDX_PUBLISHER, '%s' ) "), sGetData[ 21 ] );
				whereList.AddTail( sBuf );
			}
		}
		else
			sWhere += _T(" CATSEARCH( IB.IDX_PUBLISHER , '") + sGetData[21] + _T("',NULL) > 0 ");


/*
#ifdef _ECOSEARCH_
		sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_PUBLISHER , '") + sGetData[21] + _T("') ");
#endif

		//sWhere += _T(" IB.PUBLISHER LIKE '") + sGetData[21] + _T("%'");
#ifndef _ECOSEARCH_
		sWhere += _T(" CATSEARCH( IB.IDX_PUBLISHER , '") + sGetData[21] + _T("',NULL) > 0 ");
#endif
		*/

	}
	
	// �̴��� �ڷ� �˻�
	if( !sGetData[23].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		sWhere += _T("( ( SELECT COUNT(*) FROM LH_STORAGE_V01 CL WHERE CL.BOOK_KEY = BB.REC_KEY ");
		
		if( !sGetData[23].IsEmpty() )
		{
			sWhere += _T(" AND CL.LOAN_DATE BETWEEN to_date('")+ sGetData[23]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[24]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
		else
		{
			sWhere += _T(" AND CL.LOAN_DATE BETWEEN to_date('")+ sGetData[23]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[23]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}

		sWhere += _T(" ) = 0 ) ");
	}

	// 2005-12-14 UPD BY PDW(Jr) ==================================================
	// ���ι�ȣ �˻� 
	CString sAccessionRegNoFrom, sAccessionRegNoTo;
	m_pParentMgr->GetControlData( sCMAlias , _T("���ι�ȣ����"), sAccessionRegNoFrom );
	m_pParentMgr->GetControlData( sCMAlias , _T("���ι�ȣ����"), sAccessionRegNoTo	 );

	if( !sAccessionRegNoFrom.IsEmpty() && !sAccessionRegNoTo.IsEmpty() )
	{
		if( sWhere.GetLength() ) 
			sWhere += _T(" AND ");

		sBuf.Format( _T("ACCESSION_REC_KEY IN ( SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO BETWEEN '%s' AND '%s' )"), sAccessionRegNoFrom, sAccessionRegNoTo );
		sWhere += sBuf;
	}
	else if( !sAccessionRegNoFrom.IsEmpty() )
	{
		if( sWhere.GetLength() ) 
			sWhere += _T(" AND ");
		sBuf.Format( _T("ACCESSION_REC_KEY IN ( SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO = '%s' )"), sAccessionRegNoFrom );
		sWhere += sBuf;
	}
		
	// �����ۼ��⵵ �˻� 
	CString sYear;
	m_pParentMgr->GetControlData( sCMAlias , _T("�����ۼ��⵵"), sYear );
	if( !sYear.IsEmpty() )
	{
		if( sWhere.GetLength() )
			sWhere += _T(" AND ");

		sBuf.Format( _T(" ACCESSION_REC_KEY IN ( SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '%s' )"), sYear );
		sWhere += sBuf;
	}
	// ============================================================================

	INT cnt = whereList.GetCount();
	if( !fp && cnt )
	{
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		if( 1 == cnt )
			sBuf.Format( _T(" IB.REC_KEY IN %s "), whereList.GetAt( whereList.FindIndex( 0 ) ) );
		else
		{
			sBuf = _T(" IB.REC_KEY IN ( ") +  whereList.GetAt( whereList.FindIndex( 0 ) );
			for( INT i = 1; i < cnt; i++ )
				sBuf = sBuf + _T("  INTERSECT ") +  whereList.GetAt( whereList.FindIndex( i ) );	
					
			sBuf += _T(" ) ");
		}
		sWhere += sBuf;
	}
	//===================================================
	//2009.05.18 UPDATE BY PJW : �����ִ� ��츸 �ݴ´�.
// 	fclose(fp);
	else if( fp )
	{
		fclose(fp);
	}	
	//===================================================
	
	
	return 0;

EFS_END
	return -1;
}

INT CBL_SE_LOC_13::SearchFileSE_LOC_1310()
{
	EFS_BEGIN

	INT ids;
	CInputSanFileDlg SanFileDlg( m_pParentMgr );
	
	ids = SanFileDlg.DoModal();
	if( ids == IDCANCEL )
	{
		AfxMessageBox(_T("��ҵǾ����ϴ�."));
		return 0;
	}
	
	CESL_DataMgr pDM;

	ids = SanFileDlg.GetSanFileDM( &pDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );
	
	CESL_DataMgr TmpParentDM;
	CESL_DataMgr *pTmpParentDM;
	pTmpParentDM = &TmpParentDM;
	
	// = m_pParentMgr->FindDM( _T("DM_TMP_SE_LOC_1300") );
	//if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );
	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( _T("DM_SE_LOC_1300") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );
	pParentDM->FreeData();

	CLocCommonAPI::MakeCopyDM( m_pParentMgr , pParentDM , pTmpParentDM );

	INT nMaxUpdateRecordCnt = 50;
	CString sRegNoList;
	INT nRowCount = pDM.GetRowCount();
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		// SELECT�� ��Ϲ�ȣ ����
		CString sRegNo;
		ids = pDM.GetCellData(  i , 0 , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );

		// RECKEY����Ʈ ����
		if( (i!=0) && ((i!=1)&&(i%nMaxUpdateRecordCnt!=1))|| (i==1)) sRegNoList += _T(",");
		sRegNoList += _T("'") +sRegNo + _T("'");

		// ���ڵ尡 100���� �Ǿ��ų� �������� ��� SQL���� ������
		if( ( i !=0 && (i%nMaxUpdateRecordCnt == 0)) || (i == (pDM.GetRowCount()-1)) ) 
		{
			//===================================================
			//2009.05.21 UPDATE BY PJW : å�� �ڷ���¸� ���ӿ����� ������
// 			CString sWhere = _T(" BB.REG_NO IN (") + sRegNoList + _T(")") + _T(" AND BB.WORKING_STATUS IN (")+CString(HOLD_SEARCH_CONDITION) +_T(")") ;
			CString sWhere = _T(" BB.REG_NO IN (") + sRegNoList + _T(")") + _T(" AND BB.WORKING_STATUS IN (")+CString(HOLD_SE_SEARCH_CONDITION) +_T(")") ;
			//===================================================
			
			ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pTmpParentDM , sWhere );
//			ids = pTmpParentDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );
			
			ids = AddDM( pParentDM , pTmpParentDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );

			sRegNoList.Empty();
		}
	}
	
	CString sCMAlias = _T("CM_SE_LOC_1300");
	ids = GridDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );

	return 0;

	EFS_END
	return -1;

}

// �ڵ� �ϰ� ����
INT CBL_SE_LOC_13::ChangeCode()
{
	EFS_BEGIN

	INT ids;
	INT nGetRowCount = 0;
	CString sParentCMAlias = _T("CM_SE_LOC_1300");
	CString sParentDMAlias = _T("DM_SE_LOC_1300");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeCode()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::ChangeCode()") );

	INT nIndex;

	CString sCodeNameCode;	// ����� �ڵ��� ���� �ڵ� ( �谡���� : 0 , �̿����ѱ��� : 1 , �������� : 2 )
	CString sCodeName;			// ����� �ڵ��� �ʵ��( WORKING_STATUS , USE_LIMIT_CODE , MANAGE_CODE )
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
	CBlLocProgressBarDlg *ProgressBar;
	ProgressBar = new CBlLocProgressBarDlg( m_pParentMgr );
	ProgressBar->Create( m_pParentMgr );
	ProgressBar->CenterWindow( m_pParentMgr );
	ProgressBar->ShowWindow( SW_SHOW );
	INT nUpper = nGetRowCount-1;
	if( nGetRowCount == 1 ) nUpper = 1;
	ProgressBar->InitRange(nUpper);
	//-----------------------------------------------

	bool bIsChangeDeleteBook;
	CString sChangeDeleteBook;
	GetTempData( _T("�����ڷắ�����") , sChangeDeleteBook );
	
	if( sChangeDeleteBook.Compare(_T("Y")) == 0 )
		bIsChangeDeleteBook = TRUE;
	else
		bIsChangeDeleteBook = FALSE;
	
	// ������ �ڷ�
	CString sRepresentBookYN;
	CString sWorkingStatus;

	// �������� ���� ����
	INT repairBindingCnt = 0;

	INT nNoChangeCnt=0;
	INT nNoDeleteBookChangeCnt = 0;
	INT nChangeCnt=0;
	for( INT i = 0 ; i < nGetRowCount ; i++ )
	{
		// ���α׷����� ����------------------
		ProgressBar->ChangeProgressBar(i);
		//-----------------------------------
		nIndex = pGrid->SelectGetIdx();

		// ���� ���� �ڷ�� �����Ҽ� ����.
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BB_�谡����") , sWorkingStatus , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

		//=====================================================
		//2009.02.17 UPDATE BY PJW : WORKING_STATUS���� �߸���
// 		if( sWorkingStatus.Compare(_T("SEL321O"))==0 || (sWorkingStatus.Compare(_T("SEL322O"))==0) || (sWorkingStatus.Compare(_T("SEL323O"))==0) ||
// 		(sWorkingStatus.Compare(_T("SEL324O"))==0) || (sWorkingStatus.Compare(_T("SEL325O"))==0) )
		if( (sWorkingStatus.Compare(_T("SEL311O"))==0) || (sWorkingStatus.Compare(_T("SEL312O"))==0) || (sWorkingStatus.Compare(_T("SEL313O"))==0) ||
			// 18.09.27 JMJ KOL.RED.2018.008
			(sWorkingStatus.Compare(_T("SEL314O"))==0) || (sWorkingStatus.Compare(_T("SEL315O"))==0) || (sWorkingStatus.Compare(_T("SEL411O"))==0) || (sWorkingStatus.Compare(_T("SEL511O"))==0)  || (sWorkingStatus.Compare(_T("SEL611O"))==0) )
		//=====================================================
		{
			pGrid->SelectGetPrev();
			nNoChangeCnt++;
			continue;
		}

		// �������� �ϰ��
		if( _ttoi( pParentDM->GetCellData( _T("��������"), nIndex ) ) )
		{
			pGrid->SelectGetPrev();
			repairBindingCnt++;
			continue;
		}

		// ���� �ڷ��� ��� 
		// ���� �ɼǿ� ���� �����ϰ� �������� �ʴ´�.
		// �̿����� ������ ��츸
		if( _ttoi(sCodeNameCode) == 1 )
		{
			if( sWorkingStatus.Compare(_T("SEL215O")) == 0 )
			{
				if( !bIsChangeDeleteBook )
				{
					/// �缭�������� ����Ǵ� �Ŷ�� �����Ѵ�.
					if( sCodeValue.Compare(_T("CL")) != 0 )
					{
						pGrid->SelectGetPrev();
						nNoDeleteBookChangeCnt++;
						continue;			
					}
				}
			}
		}
		
		// UPDATE�� å���̺��� REC_KEY ����
		CString sBookRecKey;
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BB_åKEY") , sBookRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::ChangeCode()") );

		CString sSpeciesRecKey;
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BS_��KEY") , sSpeciesRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::ChangeCode()") );

		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BB_��ǥå��") , sRepresentBookYN , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::ChangeCode()") );



		// RECKEY����Ʈ ����
//		if( (i!=0) && ((i!=1)&&(i%nMaxUpdateRecordCnt!=1))|| (i==1)) sRecKeyList += _T(",");
//		sRecKeyList += sBookRecKey;

		sRecKeyList = sBookRecKey;

		// ���ڵ尡 MAX���� �Ǿ��ų� �������� ��� SQL���� ������
//		if( ( i !=0 && (i%nMaxUpdateRecordCnt == 0)) || (i == (nGetRowCount-1)) ) 
//		{
			ids = ChangeCodeDBUpdate( sCodeNameCode , sCodeName , sCodeValue  , sRecKeyList , sSpeciesRecKey , sRepresentBookYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_SE_LOC_13::ChangeCode()") );
			sRecKeyList.Empty();
//		}
		
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
		sMsg.Format( _T("�� '%d'���� �ڷ��� ó���� �ڷᰡ '%d'���̸� \r\n")
		_T("ó������ ���� �ڷᰡ '%d'���Դϴ�."),
		nChangeCnt+nNoChangeCnt+repairBindingCnt+nNoDeleteBookChangeCnt , nChangeCnt , nNoChangeCnt+repairBindingCnt+nNoDeleteBookChangeCnt);
	else if( (nChangeCnt > 0) && (nNoChangeCnt < 1) && ( repairBindingCnt < 1 ) && ( nNoDeleteBookChangeCnt < 1 ) )
		sMsg.Format( _T("��� '%d'���� ó���Ǿ����ϴ�.") , nChangeCnt );
	
	if( !sMsg.IsEmpty() )
		AfxMessageBox(sMsg);
	
	if( (nNoChangeCnt > 0) )
	{
		sMsg.Format( _T("�������� �ڷ��̹Ƿ� '%d'����ó������ ���߽��ϴ�.") , nNoChangeCnt );
		AfxMessageBox(sMsg);
	}		
	
	if( repairBindingCnt > 0 )
	{
		sMsg.Format( _T("�������� �����̹Ƿ� '%d' ���� ó������ ���߽��ϴ�."), repairBindingCnt );
		AfxMessageBox( sMsg );
	}
	
	if( nNoDeleteBookChangeCnt > 0 )
	{
		sMsg.Format( _T("���� �����̹Ƿ� '%d' ���� ó������ ���߽��ϴ�."), nNoDeleteBookChangeCnt );
		AfxMessageBox( sMsg );
	}
	
	return 0;

	EFS_END
	return -1;

}

INT CBL_SE_LOC_13::GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1340");

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�ڵ庯������") , sCodeNameCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )") );

	CString sCodeComboAlias;
	if( sCodeNameCode.Compare( _T("0") ) == 0 )
	{
		sCodeName = _T("WORKING_STATUS");
		sCodeComboAlias = _T("�谡�����ڵ�");
		sDMColumnAlias = _T("BB_�谡����");
	}
	else if( sCodeNameCode.Compare( _T("1") ) == 0 )
	{
		sCodeName = _T("USE_LIMIT_CODE");
		sCodeComboAlias = _T("�̿����ѱ����ڵ�");
		sDMColumnAlias = _T("BB_�̿����ѱ���");
	}
	else if( sCodeNameCode.Compare( _T("2") ) == 0 )
	{
		sCodeName = _T("MANAGE_CODE");
		sCodeComboAlias = _T("���������ڵ�");
		sDMColumnAlias = _T("BB_��������");
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
		ids = GetWorkingStatusDescToCode( sCodeValue , 0 );
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
INT CBL_SE_LOC_13::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList , CString sSpeciesKey , CString sRepresentBookYN)
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1300");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );

	// LOG DB �÷� ����
	CString sLogColumnName;
	// �谡 ����
	if( sCodeNameCode.Compare(_T("0")) == 0 ) 
		sLogColumnName = _T("LAST_SHELF_STATUS_CHANGE_DATE");
	// �̿����� ���� 
	else if( sCodeNameCode.Compare(_T("1")) == 0 ) 
		sLogColumnName = _T("LAST_USELIMITCODE_CHANGE_DATE");
	// �������� 
	else if( sCodeNameCode.Compare(_T("2")) == 0 )
		sLogColumnName = _T("LAST_MANAGE_CODE_CHANGE_DATE");


	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi.StartLog();
	MobileApi.SetDM( pDM );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( sCodeName , _T("STRING") , sCodeValue );
	pDM->AddDBFieldData( sLogColumnName , _T("NUMERIC") , _T("sysdate") );

//	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKeyList );
	MobileApi.MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKeyList );

/*
	CString sUpdateQuery;
	// UPDATE QUERY ����
	sUpdateQuery.Format( _T("UPDATE SE_BOOK_TBL SET"
											_T(" %s = '%s' , %s = sysdate ")
											_T("WHERE REC_KEY IN ( %s) ;")) , 
										sCodeName , sCodeValue ,  sLogColumnName , sRecKeyList );
	pDM->AddFrame( sUpdateQuery );
*/
//	ids = pDM->SendFrame();
	ids = MobileApi.SendFrame();
	if( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );
	pDM->EndFrame();

	MobileApi.EndLog();

	/// �谡 ���� ������ ���
	CString sSpeciesCodeValue;
	if( sCodeNameCode.Compare(_T("0")) == 0 ) 
	{
		pDM->StartFrame();

		CLocCommonAPI::MakeIdxWokingStatusFrame( pDM , sSpeciesKey  , 1 , m_pParentMgr );

		pDM->SendFrame();
		pDM->EndFrame();

		return 0;
	}

	
	// �̿����� �����̸� ������ �����Ѵ�.
	if( sCodeNameCode.Compare(_T("1")) == 0 )
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

INT CBL_SE_LOC_13::ChangeShelf( INT nMode )
{
	EFS_BEGIN

	INT ids;

	INT nPreIndex;

	CString sShelfDate;
	CString sShelfWorker;

	CString sDMAlias = _T("DM_SE_LOC_1300");
	CString sCMAlias = _T("CM_SE_LOC_1300");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

	CESL_Grid *pGrid = ((CESL_Grid*)pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

	nPreIndex = pGrid->GetIdx();

	// �Է� ������ ����
	CString sGetInputData[6];	// ������ġ , ���泯¥ , ���������� , ��������ڵ� , ������� , �谡 ��������ġ 
	ids = GetInputDataSE_LOC_1330( sGetInputData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
	if( ids > 0 ) return ids;


	INT nIndex;
	INT nMaxUpdateRecordCnt = 50;
	CString sRecKeyList;

	INT nNoChangeCnt=0;
	INT nChangeCnt=0;
	CString sWorkingStatus;
	CString sSpeciesKey;

	// ���� ����
	if( nMode == 0 )
	{
		nIndex = pGrid->GetIdx();

		// �ڷ� ���°� ���� ������ �˻��Ͽ� �������� �ڷ�� �������� ���Ѵ�.
		ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_�谡����") , sWorkingStatus , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
		//=====================================================
		//2009.02.17 UPDATE BY PJW : WORKING_STATUS���� �߸���
// 		if( sWorkingStatus.Compare(_T("SEL321O"))==0 || (sWorkingStatus.Compare(_T("SEL322O"))==0) || (sWorkingStatus.Compare(_T("SEL323O"))==0) ||
// 		(sWorkingStatus.Compare(_T("SEL324O"))==0) || (sWorkingStatus.Compare(_T("SEL325O"))==0) )

		// 2012.11.07 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2��
		// ���ܴ����ڷ�� �谡���� ����
		if( (sWorkingStatus.Compare(_T("SEL312O"))==0) || (sWorkingStatus.Compare(_T("SEL313O"))==0) || (sWorkingStatus.Compare(_T("SEL314O"))==0) || 
			// 18.09.27 JMJ KOL.RED.2018.008
			(sWorkingStatus.Compare(_T("SEL315O"))==0) || (sWorkingStatus.Compare(_T("SEL511O"))==0) || (sWorkingStatus.Compare(_T("SEL411O"))==0) || (sWorkingStatus.Compare(_T("SEL611O"))==0) )
//		if( (sWorkingStatus.Compare(_T("SEL311O"))==0) || (sWorkingStatus.Compare(_T("SEL312O"))==0) || (sWorkingStatus.Compare(_T("SEL313O"))==0) ||
//			(sWorkingStatus.Compare(_T("SEL314O"))==0) || (sWorkingStatus.Compare(_T("SEL315O"))==0) || (sWorkingStatus.Compare(_T("SEL511O"))==0))
		//=====================================================	
		
		{
			nNoChangeCnt++;
		}
		else
		{
			// UPDATE�� å���̺��� REC_KEY ����
			CString sBookRecKey;
			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_åKEY") , sBookRecKey , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BS_��KEY") , sSpeciesKey , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
			
			sRecKeyList = sBookRecKey;
			ids = RecordChangeShelfProc( sGetInputData , sRecKeyList , sSpeciesKey);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
			sRecKeyList.Empty();

			ids = DMUpdate( sGetInputData , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

			ids = CMGridUpdate( sGetInputData , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

			nChangeCnt++;
		}
	}
	// �ϰ� ����
	else
	{
		INT nGetRowCount = pGrid->SelectGetCount();
		// ���α׷����ٸ� �����ϱ� ����--------------------
		CBlLocProgressBarDlg *ProgressBar;
		ProgressBar = new CBlLocProgressBarDlg( m_pParentMgr );
		ProgressBar->Create( m_pParentMgr );
		ProgressBar->CenterWindow( m_pParentMgr );
		ProgressBar->ShowWindow( SW_SHOW );
		INT nUpper = nGetRowCount-1;
		if( nGetRowCount == 1 ) nUpper = 1;
		ProgressBar->InitRange(nUpper);
		//-----------------------------------------------

		// �׸����� �������� ����������
		pGrid->SelectGetTailPosition();

		for( INT i = 0 ; i < nGetRowCount ; i++ )
		{
			// ���α׷����� ����------------------
			ProgressBar->ChangeProgressBar(i);
			//-----------------------------------
			
			nIndex = pGrid->SelectGetIdx();

			// �ڷ� ���°� ���� ������ �˻��Ͽ� �������� �ڷ�� �������� ���Ѵ�.
			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_�谡����") , sWorkingStatus , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
			CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

			//=====================================================
			//2009.02.17 UPDATE BY PJW : WORKING_STATUS���� �߸���
// 			if( sWorkingStatus.Compare(_T("SEL321O"))==0 || (sWorkingStatus.Compare(_T("SEL322O"))==0) || (sWorkingStatus.Compare(_T("SEL323O"))==0) ||
// 			(sWorkingStatus.Compare(_T("SEL324O"))==0) || (sWorkingStatus.Compare(_T("SEL325O"))==0) )

			// 2012.11.07 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2��
			// ���ܴ����ڷ�� �谡���� ����
			if( (sWorkingStatus.Compare(_T("SEL312O"))==0) || (sWorkingStatus.Compare(_T("SEL313O"))==0) || (sWorkingStatus.Compare(_T("SEL314O"))==0) || 
				// 18.09.27 JMJ KOL.RED.2018.008
				(sWorkingStatus.Compare(_T("SEL315O"))==0) || (sWorkingStatus.Compare(_T("SEL511O"))==0) || (sWorkingStatus.Compare(_T("SEL411O"))==0) || (sWorkingStatus.Compare(_T("SEL611O"))==0) )
//			if( (sWorkingStatus.Compare(_T("SEL311O"))==0) || (sWorkingStatus.Compare(_T("SEL312O"))==0) || (sWorkingStatus.Compare(_T("SEL313O"))==0) ||
//				(sWorkingStatus.Compare(_T("SEL314O"))==0) || (sWorkingStatus.Compare(_T("SEL315O"))==0) || (sWorkingStatus.Compare(_T("SEL511O"))==0))
			//=====================================================
			
			{
				nNoChangeCnt++;
				pGrid->SelectGetPrev();
				continue;
			}


			// UPDATE�� å���̺��� REC_KEY ����
			CString sBookRecKey;
			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_åKEY") , sBookRecKey , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BS_��KEY") , sSpeciesKey , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

			// RECKEY����Ʈ ����
//			if( (i!=0) && ((i!=1)&&(i%nMaxUpdateRecordCnt!=1))|| (i==1)) sRecKeyList += _T(",");
			sRecKeyList += sBookRecKey;

			// ���ڵ尡 100���� �Ǿ��ų� �������� ��� SQL���� ������
//			if( ( i !=0 && (i%nMaxUpdateRecordCnt == 0)) || (i == (nGetRowCount-1)) ) 
//			{
				ids = RecordChangeShelfProc( sGetInputData , sRecKeyList , sSpeciesKey );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
				sRecKeyList.Empty();
//			}

			ids = DMUpdate( sGetInputData , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
			
			ids = CMGridUpdate( sGetInputData , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

			if( i !=  nGetRowCount-1 ) pGrid->SelectGetPrev();
			nChangeCnt++;
		}
		// ���α׷����� ��Ʈ����------------------
		ProgressBar->DestroyWindow();
		if( ProgressBar != NULL ) delete ProgressBar;
		//----------------------------------
	}

	m_pParentMgr->AllControlDisplay( _T("CM_SE_LOC_1330") , nPreIndex );

	CString sMsg;
	if( nMode == 1 )
	{
		if( (nChangeCnt > 0) && (nNoChangeCnt > 0) )
			sMsg.Format( _T("�� '%d'���� �ڷ��� ó���� �ڷᰡ '%d'���̸� �������� �ڷ��̱� ������ ó������ ���� �ڷᰡ '%d'���Դϴ�."),
			nChangeCnt+nNoChangeCnt,nChangeCnt,nNoChangeCnt );
		else if( (nChangeCnt > 0) && (nNoChangeCnt < 1) )
			sMsg.Format( _T("��� '%d'���� ó���Ǿ����ϴ�.") , nChangeCnt );
		else if( (nChangeCnt < 1) && (nNoChangeCnt > 0) )
			sMsg.Format( _T("��� '%d'���� �������� �ڷ��̱� ������ ó������ ���߽��ϴ�.") , nNoChangeCnt );
	}
	else
	{
		if( (nChangeCnt > 0) && (nNoChangeCnt < 1) )
			sMsg.Format( _T("ó���Ǿ����ϴ�.") );
		else if( (nChangeCnt < 1) && (nNoChangeCnt > 0) )
			sMsg.Format( _T("�������� �ڷ��̱� ������ ó������ ���߽��ϴ�.") );
	}
	AfxMessageBox(sMsg);

	return 0;


	EFS_END
	return -1;

}

// ������ġ , ���泯¥ , ���������� , ��������ڵ� , ������� , �谡 ��������ġ 
INT CBL_SE_LOC_13::GetInputDataSE_LOC_1330( CString *sGetInputData )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1330");

	TCHAR *sControlAlias[6] =
	{
		_T("������ġ_�Է�") , _T("���泯¥_�Է�") , _T("����������_�Է�") , _T("��������ڵ�_�Է�") , _T("�����������_�Է�") , _T("�谡��ġ")
	};

	INT nIndex = 1;
	for( INT i = 0 ; i < 6 ; i++ )
	{
		if( i == 3 ) nIndex = 0;
		else nIndex = 0;
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sGetInputData[i] , -1 , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::GetInputDataSE_LOC_1330( CString *sGetInputData )") );

		sGetInputData[i].TrimLeft(); sGetInputData[i].TrimRight();

		if( sGetInputData[i].Compare(_T("�������")) == 0 ) sGetInputData[i] = _T("");

		if( sGetInputData[i].IsEmpty() ) 
		{
			if( i == 0 || i == 1 || i==2 )
			{
				CString sMsg;
				if( i == 0 ) sMsg = _T("������ġ�� �������ֽʽÿ�");
				if( i == 1 ) sMsg = _T("�谡���� �Է��Ͻʽÿ�");
				if( i == 2 ) sMsg = _T("���������ڸ� �Է��Ͻʽÿ�");
				AfxMessageBox( sMsg );
				return 1;
			}
		}
	}
	return 0;

	EFS_END
	return -1;

}


INT CBL_SE_LOC_13::RecordChangeShelfProc( CString *sGetInputData , CString &sRecKeyList , CString sSpeciesKey)
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1300");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::RecordChangeShelfProc( CString *sGetInputData , CString &sRecKeyList )") );


	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi.StartLog();
	MobileApi.SetDM( pDM );

	pDM->StartFrame();
	
	CString sUpdateQuery;

	pDM->AddDBFieldData( _T("SHELF_LOC_CODE") , _T("STRING") , sGetInputData[0] );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_DATE") , _T("STRING") , sGetInputData[1] );
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , sGetInputData[2] );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_REASON_CODE") , _T("STRING") , sGetInputData[3] );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_ETC_REASON") , _T("STRING") , sGetInputData[4] );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_BEFORE_LOC") , _T("STRING") , sGetInputData[5] );

	MobileApi.MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKeyList );

	// ������ ����Ѵ�.
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

	CLocCommonAPI::MakeIdxTmpTblInsertFrame( pDM , sSpeciesKey , 1 , m_pParentMgr ,  &MobileApi );
	
	// QUERY SEND
//	ids = pDM->SendFrame();
	ids = MobileApi.SendFrame();
	if( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::RecordChangeShelfProc( CString *sGetInputData , CString &sRecKeyList )") );

	pDM->EndFrame();

	MobileApi.EndLog();

	return 0;

	EFS_END
	return -1;

}

// sGetInputData : ������ġ , ���泯¥ , ���������� , ��������ڵ� , ������� , �谡 ��������ġ 
INT CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1300");

	TCHAR *sDMColumnAlias[5] =
	{
		_T("BB_�谡��ȣ") , _T("BB_�谡��������") , _T("BB_�谡����������") , _T("BB_�谡�������") , _T("BB_�谡�����������") 
	};

	for( INT i = 0 ; i < 5 ; i++ )
	{
		ids = m_pParentMgr->SetDataMgrData( sDMAlias , sDMColumnAlias[i] , sGetInputData[i] , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	}
	
	CString sCodeDesc;
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�ڷ�Ǳ���") , sGetInputData[0] , sCodeDesc  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );

	ids = m_pParentMgr->SetDataMgrData( sDMAlias , _T("BB_�ڷ�Ǹ�") , sCodeDesc , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	return 0;


	EFS_END
	return -1;

}

// �׸��� ������Ʈ ������ġ , ���泯¥ , ���������� , ��������ڵ� , ������� , �谡 ��������ġ 
INT CBL_SE_LOC_13::CMGridUpdate( CString *sGetInputData , INT nIndex )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1300");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );

	TCHAR *sDMColumnAlias[3] =
	{
		_T("BB_�谡��ȣ") , _T("BB_�ڷ�Ǹ�") , _T("BB_�谡�����������") 
	};

	INT nColIndex[3] = 
	{
		-1 , -1 , -1 
	};

	CString sData[3];

	sData[0] = sGetInputData[0];
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�ڷ�Ǳ���") , sGetInputData[0] , sData[1]  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	sData[2] = sGetInputData[4];

	// �÷� ã�� : ��ũ�� ������ �޴����� �˸��ƽ��� ã�´�.
	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , sCMAlias , sGridAlias , sDMColumnAlias , 3 , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );

	for( INT i = 0 ; i < 3 ; i++ )
	{
		if( nColIndex[i] == -1 ) continue;
		ids = pGrid->SetAt( nIndex , nColIndex[i] , sData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	}

	return 0;


	EFS_END
	return -1;

}

//------------------------------------------------------------------------------------
// �������� ���̴� ���
//------------------------------------------------------------------------------------

INT CBL_SE_LOC_13::AddDM( CESL_DataMgr *pDestDM , CESL_DataMgr *pSourceDM )
{
	EFS_BEGIN

	INT ids;
	CString sGetData;

	const INT nColumnCnt = pSourceDM->GetColCount();
	CString *sColumnAlias;
	sColumnAlias = NULL;
	sColumnAlias = new CString[nColumnCnt];

	ids = pSourceDM->GetAllAlias( sColumnAlias , nColumnCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::AddDM( CESL_DataMgr *pDestDM , CESL_DataMgr *pSourceDM )") );

	INT nRowCount = pSourceDM->GetRowCount();
	for( INT i = 0 ; i < nRowCount ;i++ )
	{
		for( INT j = 0 ; j < pDestDM->GetColCount() ; j++ )
		{
			if( j == 0 ) pDestDM->InsertRow( -1 );
			ids = pSourceDM->GetCellData( i , j , sGetData );
			if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::AddDM( CESL_DataMgr *pDestDM , CESL_DataMgr *pSourceDM )") );

			ids = pDestDM->SetCellData( sColumnAlias[j] , sGetData , pDestDM->GetRowCount()-1);			
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::AddDM( CESL_DataMgr *pDestDM , CESL_DataMgr *pSourceDM )") );
			sGetData.Empty();
		}
	}

	return 0;

	EFS_END
	return -1;

}


// nMode 
// 0-> WORKING_STATUS �� DESCRIPTION�� �ڵ�� �������ش�.
// 1-> WORKING_STATUS �� �ڵ带 DESCRIPTION���� �������ش�.
INT CBL_SE_LOC_13::GetWorkingStatusDescToCode( CString &sCodeValue , INT nMode )
{
	EFS_BEGIN

	INT ids;

	ids = m_pLocCommonAPI->GetWorkingStatusDescToCode( sCodeValue , nMode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetWorkingStatusDescToCode") );
	return 0;

	EFS_END
	return -1;

}


// �谡 �ڷ� ������ �׸��带 ȭ�鿡 ����� �ڵ尪�� �������� �����Ű�� �۾��� �Ѵ�.
INT CBL_SE_LOC_13::GridDisplay( CString sCMAlias )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias;
	CString sFieldAlias;

	if( sCMAlias.Compare(_T("CM_SE_LOC_1300"))==0)
	{
		sDMAlias = _T("DM_SE_LOC_1300");
		sFieldAlias = _T("BB_�ڷ�Ǹ�");
	}
	else if( sCMAlias.Compare(_T("CM_BO_LOC_1100"))==0)
	{
		sDMAlias = _T("DM_BO_LOC_1100");
		sFieldAlias = _T("BB_�谡��ġ��ȣ");
	}

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

	// �谡 ��ġ CODE -> ����
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("�ڷ�Ǳ���") , sFieldAlias  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

	// �谡 ���� ����
	if( sCMAlias.Compare(_T("CM_SE_LOC_1300"))==0) 
	{
		CString sCodeValue;
		for( INT i = 0 ; i < pDM->GetRowCount() ;i++ )
		{
			ids = pDM->GetCellData( _T("BB_�谡����") , i , sCodeValue );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );
			ids = GetWorkingStatusDescToCode( sCodeValue , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );
			ids = pDM->SetCellData( _T("BB_�谡����") , sCodeValue , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

		}
		
		// ��������
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("��������") , _T("BB_��������")  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );
		// �̿����� ����
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("�̿����ѱ���") , _T("BB_�̿����ѱ���")  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );
		// ���� ����
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("���ı���") , _T("���»���")  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

	}

	if( sCMAlias.Compare(_T("CM_SE_LOC_1300"))==0)
	{
		// û����ȣ ����
		ids = MakeShelfCode( sDMAlias );
		if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

		// ���������� ǥ��
		ids = DisplayCopyBook( sDMAlias );
		if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

		// ���� �߰�
		// �׸��忡 �����ٶ� ���»��� �����ֱ�
		if( DisplayFormType( pDM ) )
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::DisplayFormType( CESL_DataMgr *pDM )") );


	}

	ids = m_pParentMgr->AllControlDisplay( sCMAlias , -1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

	return 0;

	EFS_END
	return -1;

}

// û����ȣ �����ϴ� �Լ�
INT CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )
{
	EFS_BEGIN

	INT ids;

	ids = CLocCommonAPI::MakeShelfCode( m_pParentMgr , sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )") );
/*
	const INT nCnt = 5;
	TCHAR *sFieldAlias[nCnt] =
	{
		_T("IBS_û��_��ġ��ȣ_�ڵ�") ,
		_T("IBS_û��_�з���ȣ") ,
		_T("IBS_û��_������ȣ") ,
		_T("IBS_û��_��å��ȣ") ,
		_T("IBS_û��_������ȣ") ,
	};


	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )") );

	INT nRowCount = pDM->GetRowCount();

	CString sShelfCode;
	CString sTmpCode;
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		sShelfCode.Empty();
		// û����ȣ ����
		for( INT j = 0 ; j < nCnt ; j++ )
		{
			ids = pDM->GetCellData( sFieldAlias[j] , i , sTmpCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )") );
			
			if( !sTmpCode.IsEmpty() ) 
			{
				if( sShelfCode.GetLength() > 1 ) sShelfCode += _T("-");
				sShelfCode += sTmpCode;
			}
		}

		// û����ȣ ����
		ids = pDM->SetCellData( _T("û����ȣ") , sShelfCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )") );
	}
*/
	return 0;

	EFS_END
	return -1;

}

// ��/���� ���� ǥ��
INT CBL_SE_LOC_13::DisplayCopyBook( CString sDMAlias )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DisplayCopyBook( CString sDMAlias )") );

	INT nRowCount = pDM->GetRowCount();

	CString sCopyCode;
	CString sTmpCode;
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		sCopyCode.Empty();
		// ������ ����
		ids = pDM->GetCellData( _T("BB_��/��������") , i , sTmpCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )") );
		
		if( !sTmpCode.IsEmpty() && (sTmpCode.Compare(_T("0"))!=0)) 
			sCopyCode = _T("����");
		else
			sCopyCode = _T("����");

		// ��/�������� ����
		ids = pDM->SetCellData( _T("BB_��/��������") , sCopyCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )") );
	}

	return 0;


	EFS_END
	return -1;

}

// �ڵ� �ϰ� ������ 
// å���̺��� ��ǥ�� �Ǵ� �ڵ带 ������ �Լ�
// �̿����� ���� : 
//                 		_T("CL") , _T("CD") , _T("CA") , _T("IZ") , _T("GM")
INT CBL_SE_LOC_13::GetUseLimitBossCode( CESL_DataMgr *pSpeciesDM , CString &sSpeciesCodeValue )
{
	EFS_BEGIN

	INT ids;

	const INT nUseLimitCodeCnt = 5;
	INT nUseLimitCodeIndex;
	TCHAR *sUseLimitCode[nUseLimitCodeCnt] = 
	{
		_T("CL") , _T("CD") , _T("CA") , _T("IZ") , _T("GM")
	};



	CString sCode;
	nUseLimitCodeIndex = 0;
	for( INT i = 0 ; i < pSpeciesDM->GetRowCount() ; i++ )
	{
		ids = pSpeciesDM->GetCellData( _T("�̿����ѱ���") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::GetUseLimitBossCode( CESL_DataMgr *pSpeciesDM , CString &sSpeciesCodeValue )") );
		
		if( sCode.IsEmpty() ) sCode = sUseLimitCode[0];
		
		for( INT j = 0 ; j < nUseLimitCodeCnt ; j++ )
		{
			if( sCode.Compare( sUseLimitCode[j]) == 0 ) break;
		}
		if( j == nUseLimitCodeCnt ) continue;

		if( nUseLimitCodeIndex < j ) nUseLimitCodeIndex = j;
	}
	sSpeciesCodeValue = sUseLimitCode[nUseLimitCodeIndex];

	return 0;

	EFS_END
	return -1;

}

// �ڵ� �ϰ� ������
// ����Ǿ��� ��� ȭ�鿡 �����ִ� �۾�
INT CBL_SE_LOC_13::CMGridCodeChangeUpdate(INT nIndex)
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1300");

	CString sCMAlias = _T("CM_SE_LOC_1300");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );

	const INT nCnt = 3;

	TCHAR *sDMFieldAlias[nCnt] =
	{
		_T("BB_��������") , _T("BB_�谡����") , _T("BB_�̿����ѱ���")
	};

	INT nColIndex[nCnt] =
	{
		-1,-1,-1
	};

	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sDMFieldAlias , nCnt , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") ); 

	CString sData;
	
	for( INT i = 0 ; i < 3 ; i++ )
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
INT CBL_SE_LOC_13::DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1300");

	if( sDMColumnAlias.Compare(_T("BB_��������")) == 0 )
	{
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(  _T("��������") , sCodeValue , sCodeValue   );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
		
	}
	else if( sDMColumnAlias.Compare(_T("BB_�谡����")) == 0 )
	{
		ids = GetWorkingStatusDescToCode( sCodeValue , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
	}
	else if( sDMColumnAlias.Compare(_T("BB_�̿����ѱ���")) == 0 )
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

INT CBL_SE_LOC_13::DetailView()
{
	EFS_BEGIN

	CString sParentCMAlias = _T("CM_SE_LOC_1300");
	CString sParentDMAlias = _T("DM_SE_LOC_1300");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeCode()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::ChangeCode()") );

	// �˻�
	if( pParentDM->GetRowCount() < 1 ) 
	{
		AfxMessageBox( _T("�ڷḦ �˻��� �ֽʽÿ�") );
		return 0;
	}

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;
	
	INT nMode = 0;
	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	
	INT nStartPos = 0;
	INT nTmpStartPos = pGrid->GetIdx();

	if( nSelectCnt > 1 )
	{
		nMode = 1;
		pGrid->SelectGetHeadPosition();
	}
	else
	{
		nSelectCnt = pParentDM->GetRowCount();
		nStartPos = pGrid->GetIdx();
	}
	
	INT nIndex;
	
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 1 )
			nIndex = pGrid->SelectGetIdx();
		else
			nIndex = i;
		
		pParentDM->GetCellData( _T("BS_��KEY") , nIndex, sSpeciesKey );
		sSpeciesKeyList.AddTail( sSpeciesKey );
		asGridRow.Add( nIndex );

		if( nIndex == nTmpStartPos )
			nStartPos = i;
		
		if( nMode == 1)
			pGrid->SelectGetNext();
		
	}
	
	
	CSEBookDetailViewerDlg Dlg(m_pParentMgr);
	
	Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , nStartPos );
	Dlg.DoModal();

	return 0;

	EFS_END
	return -1;

}

// �� �̿����� ���� UPDATE
INT CBL_SE_LOC_13::ChangeUseLimitCode( CString sSpeciesKey )
{
	EFS_BEGIN

	INT ids;
	CString sPreUseLimitCode , sNewUseLimitCode;

	CESL_DataMgr *pUseLimitDM = m_pParentMgr->FindDM(_T("DM_SE_LOC_1300_USE_LIMIT_SPECIES"));
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

INT CBL_SE_LOC_13::MakeArrayUseLimitCode( CString sUseLimitCode , CArray <CString,CString> &saUseLimitCode )
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

// �켱 ������ �������� �˾Ƴ��ش�.
INT CBL_SE_LOC_13::GetBossUseLimitCode( CString &sUseLimitCode , CArray <CString,CString> &saUseLimitCode )
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

bool CBL_SE_LOC_13::CompareUseLimtCodes( CArray <CString,CString> &saPreUseLimitCode , CArray <CString,CString> &saNewUseLimitCode )
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


/// ���� �ڵ� �ϰ� ������
/// ���� ���� -> ���� ����
INT CBL_SE_LOC_13::ChangeSpeciesCode()
{
	EFS_BEGIN


	INT ids;
	INT nGetRowCount = 0;
	CString sParentCMAlias = _T("CM_SE_LOC_1300");
	CString sParentDMAlias = _T("DM_SE_LOC_1300");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
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
	CBlLocProgressBarDlg *ProgressBar;
	ProgressBar = new CBlLocProgressBarDlg( m_pParentMgr );
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
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BS_��KEY") , sSpeciesRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

		pParentDM->StartFrame();

		pParentDM->InitDBFieldData();
		pParentDM->AddDBFieldData(_T("USE_LIMIT_CODE"),_T("STRING"),sCodeValue);
		pParentDM->MakeUpdateFrame(_T("SE_SPECIES_TBL"),_T("REC_KEY"),_T("NUMERIC"),sSpeciesRecKey);

		// 2006-04-03 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ���� ���� �κ��� �߰�
		pParentDM->InitDBFieldData();
		pParentDM->AddDBFieldData(_T("USE_LIMIT_CODE"),_T("STRING"),sCodeValue);
		pParentDM->MakeUpdateFrame(_T("IDX_SE_TBL"),_T("REC_KEY"),_T("NUMERIC"),sSpeciesRecKey);

		CLocCommonAPI::MakeIdxTmpTblInsertFrame( pParentDM, sSpeciesRecKey, 1 , m_pParentMgr);
		// ------------------------------------------------------------------------------------------------------

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
		sMsg.Format( _T("�� '%d'���� �ڷ��� ó���� �ڷᰡ '%d'���̸� �������� �ڷ��̱� ������ ó������ ���� �ڷᰡ '%d'���Դϴ�."),
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

INT CBL_SE_LOC_13::DisplayFormType( CESL_DataMgr *pDM )
{
	CString data[ 4 ];
	CString output;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		output.Empty();
		
		data[ 0 ] = pDM->GetCellData( _T("PAGE"), i );
		data[ 1 ] = pDM->GetCellData( _T("PHYSICAL_PROPERTY"), i );
		data[ 2 ] = pDM->GetCellData( _T("BOOK_SIZE") , i );
		data[ 3 ] = pDM->GetCellData( _T("ACCOMP_MAT"), i );
		
		if( !data[ 0 ].IsEmpty() )
			output += data[ 0 ];
		if( !data[ 1 ].IsEmpty() )
			output = output + _T(":") + data[ 1 ];
		if( !data[ 2 ].IsEmpty() )
			output = output + _T(";") + data[ 2 ];
		if( !data[ 3 ].IsEmpty() )
			output = output + _T("+") + data[ 3 ];

		pDM->SetCellData( _T("���»���"), output, i );

		//pDM->SetCellData( _T("���»���"), MakeFormType( pDM->GetCellData( _T("��ũ"), i ) ), i );
	}
	return 0;
}

CString CBL_SE_LOC_13::MakeFormType( CString marc )
{
	CMarcMgr* pMarcMgr = m_pParentMgr->m_pInfo->pMarcMgr;
	CMarc Marc;
	pMarcMgr->Decoding( marc , &Marc );

	CString formType;
	Marc.GetField( _T("300"), formType );
	formType.Replace( CString( 31 ) + _T("a") , _T("") );
	formType.Replace( CString( 31 ) + _T("b") , _T("") );
	formType.Replace( CString( 31 ) + _T("c") , _T("") );
	formType.Replace( CString( 31 ) + _T("d") , _T("") );
	formType.Replace( CString( 31 ) + _T("e") , _T("") );
	formType.Replace( CString( 30 ) , _T("") );
	formType.TrimLeft();
	formType.TrimRight();	

	return formType;
}
