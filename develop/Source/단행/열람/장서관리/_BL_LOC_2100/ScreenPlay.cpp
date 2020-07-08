// ScreenPlay.cpp: implementation of the CScreenPlay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScreenPlay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScreenPlay::CScreenPlay(CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	m_pCommonAPI = NULL;
	m_pCommonAPI = new CLocCommonAPI( m_pParentMgr );

	m_sResultCMAlias = _T("CM_BO_LOC_2170");
	m_sResultProgressAlias = _T("�����Ȳ");
	m_sResultGridAlias = _T("�׸���α�");
	m_sResultDMAlias = _T("DM_BO_LOC_2170");

}

CScreenPlay::~CScreenPlay()
{
	delete m_pCommonAPI;
}

// ���� ȭ�� ���÷���
INT CScreenPlay::MainGridDisplay( CString sBookCMAlias , CString sBookDMAlias , CBLMapData_Mem* pMapData_ETC , CBLMapData_Mem* pMapData_NoDB )
{
	EFS_BEGIN

	INT ids;

	// �ڵ尪 -> DESC�� ���� : �谡���� , �ڷ��
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sBookDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MainGridDisplay( CString sBookCMAlias )") );

	CString sBookWorkingStatus;

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	CString sResultCell;
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{		
		// �谡 ����
		ids = pDM->GetCellData( _T("�谡����") , i , sBookWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MainGridDisplay( CString sBookCMAlias )") ); 

		ids = m_pCommonAPI->GetWorkingStatusDescToCode( sBookWorkingStatus , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("MainGridDisplay( CString sBookCMAlias )") ); 

		ids = pDM->SetCellData( _T("�谡����") , sBookWorkingStatus , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("MainGridDisplay( CString sBookCMAlias )") ); 


		// ���� �߰�
		// ���ݿ� õ���� ���б�ȣ �ֱ�
		pDM->SetCellData( _T("����"), ConvertPrice( pDM->GetCellData( _T("����"), i ) ), i );


		// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
		//
		// db �� ���� �� 
		// �׸��� "���˰��" �߰� 
		pDM->GetCellData( _T("���˰��"),  i , sResultCell );
		if( sResultCell == "" )
		{
			pDM->SetCellData( _T("���˰��"), _T("�����") , i );

			if( pMapData_NoDB != NULL )
			{
				_BLMapData_Value* _mapdata_val_No = pMapData_NoDB->Find( pDM->GetCellData( _T("��Ϲ�ȣ") , i) );
				if( _mapdata_val_No != NULL ) 
				{
					pDM->SetCellData( _T("���˰��"), _T("DB����") , i );
				}
			}

			if( pMapData_ETC != NULL )
			{
				_BLMapData_Value* _mapdata_val_Etc = pMapData_ETC->Find( pDM->GetCellData( _T("��Ϲ�ȣ") , i) );
				if( _mapdata_val_Etc != NULL ) 
				{
					pDM->SetCellData( _T("���˰��"), _T("�ٸ��ڷ��") , i );
				}
			}
		}
		//
		// KOL.RED.2018.201 ---------------------------------------------------------------------------
	}

	// �ڷ�� ����
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("�ڷ�Ǳ���") , _T("�ڷ��") );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("MainGridDisplay( CString sBookCMAlias )") ); 

	// û����ȣ ����
	ids = m_pCommonAPI->MakeShelfCode( sBookDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("MainGridDisplay( CString sBookCMAlias )") );  

	// DM�� �ִ� �ڷḦ �׸��忡 �����ֱ�
	ids = m_pParentMgr->AllControlDisplay( sBookCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("MainGridDisplay( CString sBookCMAlias )") );


	return 1;

	// �Ѱ��� �����ش�.
	INT nRowCount = pDM->GetRowCount();
	CString sBookCnt;
	sBookCnt.Format( _T("%d") , nRowCount );

	CString sEtcCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");
	CString sControlAlias = _T("�̵���ȣ2");

	ids = m_pParentMgr->SetControlData( sEtcCMAlias , sControlAlias , sBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("MainGridDisplay( CString sBookCMAlias )") );



	return 0;

	EFS_END
	return -1;

}

// �˻� ȭ�鿡�� �弭 ���� ������ ������
INT CScreenPlay::GetBookCheckInitYN( INT nIndex , CString &sBookCheckYN )
{
	EFS_BEGIN

	const INT nCnt = 2;
	TCHAR *sControlAlias[nCnt][2] = 
	{
		{ _T("�ʱ�ȭ����1") , _T("N") } , 
		{ _T("�ʱ�ȭ����2") , _T("Y") } , 
	};

	CString sCMAlias = _T("CM_BO_LOC_2120");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetBookCheckInitYN( CString &sBookCheckYN )") );

	CESL_Control_Element *ebtnMode;
	CButton *btnMode;

	ebtnMode = pCM->FindControlElement( sControlAlias[nIndex][0] );
	if( ebtnMode == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetBookCheckInitYN( CString &sBookCheckYN )") );

	btnMode = (CButton*)(m_pParentMgr->GetDlgItem( ebtnMode->CTRL_ID ));

	if( btnMode->GetCheck() == 1 )
	{
		sBookCheckYN = sControlAlias[nIndex][1];
	}

	return 0;

	EFS_END
	return -1;

}

// �弭 üũ ȭ�鿡�� ���α׷����� �����ϱ�
INT CScreenPlay::InitProgressBar( INT nMax )
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_2140");
	CString sProgressAlias = _T("�����Ȳ");

	CProgressCtrl *pProgressCtrl;

	pProgressCtrl = GetProgressPtr( sCMAlias , sProgressAlias );
	if( pProgressCtrl == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("InitProgressBar()") );

	pProgressCtrl->SetRange32( 0 , nMax );

	return 0;

	EFS_END
	return -1;

}


// ���α׷����� ������ ������
CProgressCtrl* CScreenPlay::GetProgressPtr( CString sCMAlias , CString sProgressAlias )
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Control_Element *pProgressElemnet;
	CProgressCtrl *pProgressCtrl;
	
	pProgressElemnet = pCM->FindControlElement( sProgressAlias );
	if( pProgressElemnet == NULL ) return NULL;

	pProgressCtrl = (CProgressCtrl*)(m_pParentMgr->GetDlgItem( pProgressElemnet->CTRL_ID ));
	if( pProgressCtrl == NULL ) return NULL;

	return pProgressCtrl;	

	EFS_END
	return NULL;

}

// �弭üũ�� ���α׷������� ���� ���¸� ��Ÿ����.
INT CScreenPlay::ProgressSetPos( INT nPos )
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_2140");
	CString sProgressAlias = _T("�����Ȳ");

	CProgressCtrl *pProgressCtrl;

	pProgressCtrl = GetProgressPtr( sCMAlias , sProgressAlias );
	if( pProgressCtrl == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("InitProgressBar()") );

	pProgressCtrl->SetPos( nPos );

	return 0;

	EFS_END
	return -1;

}

// �弭 üũ�� ����� ȭ�鿡 �����ִ� �Լ�
INT CScreenPlay::ViewBookCheckResult( INT nBookCheckY , INT nBookCheckN , INT nBookCheckX , INT nEtc , INT nTotalRegNoCnt , INT nTotalBookCheckCnt )
{
	EFS_BEGIN

	INT ids;

	CString sTmp;
	CString sCMAlias = _T("CM_BO_LOC_2140");
	CString sControlAlias;


	// ���� �ڷ��
	sTmp.Format( _T("%d") , nBookCheckY );

	sControlAlias = _T("�����ڷ��");

	ids = m_pParentMgr->SetControlData( sCMAlias , sControlAlias , sTmp );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewBookCheckResult()") );

	sTmp.Format( _T("%d") , nBookCheckN );

	sControlAlias = _T("�н��ڷ��");

	ids = m_pParentMgr->SetControlData( sCMAlias , sControlAlias , sTmp );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewBookCheckResult()") );

	sTmp.Format( _T("%d") , nBookCheckX );

	sControlAlias = _T("�������ڷ��");

	ids = m_pParentMgr->SetControlData( sCMAlias , sControlAlias , sTmp );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewBookCheckResult()") );

	sTmp.Format( _T("%d") , nEtc );

	sControlAlias = _T("��Ÿ�ڷ��");

	ids = m_pParentMgr->SetControlData( sCMAlias , sControlAlias , sTmp );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewBookCheckResult()") );

	sTmp.Format( _T("%d") , nTotalRegNoCnt );

	sControlAlias = _T("��Ϲ�ȣ�ڷ��");

	ids = m_pParentMgr->SetControlData( sCMAlias , sControlAlias , sTmp );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewBookCheckResult()") );

	sTmp.Format( _T("%d") , nTotalBookCheckCnt );

	sControlAlias = _T("���˴���ڷ��");

	ids = m_pParentMgr->SetControlData( sCMAlias , sControlAlias , sTmp );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("ViewBookCheckResult()") );

	return 0;

	EFS_END
	return -1;

}

// �弭 üũ - ���� �Ϸ� ��ư Ȱ��ȭ
INT CScreenPlay::SetBookCheckButton()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_2140");
	CString sButtonAlias = _T("���˿Ϸ��ư");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SetBookCheckButton()") );

	CESL_Control_Element *pButtonElemnet;
	CButton *pButtonCtrl;
	
	pButtonElemnet = pCM->FindControlElement( sButtonAlias );
	if( pButtonElemnet == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SetBookCheckButton()") );

	pButtonCtrl = (CButton*)(m_pParentMgr->GetDlgItem( pButtonElemnet->CTRL_ID ));
	if( pButtonCtrl == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SetBookCheckButton()") );

	pButtonCtrl->EnableWindow( TRUE );

	return 0;

	EFS_END
	return -1;

}

// �׸����� Ư�� nIndex���� �ʱ�ȭ���а��� sCheck�� ������Ʈ
INT CScreenPlay::GridCheckYN( INT nIndex , CString sCheck )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GridCheckYN( INT nIndex , CString sCheck )") );

	TCHAR *sDMColumnAlias[1] = { _T("�ʱ�ȭ����") };

	INT nColIndex[1] = { -1 };

	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sDMColumnAlias , 1 , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GridCheckYN") );

	INT nCol = nColIndex[0];

	if( nCol == -1 ) return 0;

	ids = pGrid->SetAt( nIndex , nCol , sCheck );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GridCheckYN( INT nIndex , CString sCheck )") ); 

	return 0;

	EFS_END
	return -1;

}

// �θ��� �׸��� ������ ����
CESL_Grid* CScreenPlay::GetMainGridPtr()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_2100");
	CString sGridAlias = _T("gridMAIN");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) return NULL;

	return pGrid;

	EFS_END
	return NULL;

}

CESL_Grid* CScreenPlay::GetInDBGridPtr()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_2100_IN_DB_ETC_GRID");
	CString sGridAlias = _T("InDBETCList");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) return NULL;

	return pGrid;

	EFS_END
	return NULL;

}

// ���� ��ȣ�� ȭ�� �����ش�.
INT CScreenPlay::ViewCheckNumber()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_2100_BOOK_CHECK_RESULT");
	CString sDMFieldAlias = _T("���˹�ȣ");
	CString sDMFieldAlias2 = _T("�弭���˳⵵");
	CString sDMFieldAlias3 = _T("����ڷ��");
	CString sCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");
	CString sControlAlias = _T("���˹�ȣ");
	CString sControlAlias2 = _T("���˳⵵");
	CString sControlAlias3 = _T("�ڷ�Ǹ�");
	
	CString sCheckNo , sCheckYear , sShelfLocCode;

	ids = m_pParentMgr->GetDataMgrData( sDMAlias , sDMFieldAlias , sCheckNo , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewCheckNumber()") );

	ids = m_pParentMgr->GetDataMgrData( sDMAlias , sDMFieldAlias2 , sCheckYear , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewCheckNumber()") );

	ids = m_pParentMgr->GetDataMgrData( sDMAlias , sDMFieldAlias3 , sShelfLocCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewCheckNumber()") );

	ids = m_pParentMgr->SetControlData( sCMAlias , sControlAlias , sCheckNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewCheckNumber()") );

	ids = m_pParentMgr->SetControlData( sCMAlias , sControlAlias2 , sCheckYear );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewCheckNumber()") );

	ids = m_pParentMgr->SetControlData( sCMAlias , sControlAlias3 , sShelfLocCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewCheckNumber()") );

	return 0;

	EFS_END
	return -1;

}

// ���� ��ȣ ���� ȭ�� �����ִ� �Լ�
INT CScreenPlay::ViewCheckNoList()
{
	EFS_BEGIN

	INT ids;
	
	CString sCMAlias = _T("CM_BO_LOC_2160");

	ids = m_pParentMgr->AllControlDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewCheckNumber()") );

	return 0;

	EFS_END
	return -1;

}

// ���� ��ȣ ���� �׸��忡 ���� ����Ű�� ���� ����
INT CScreenPlay::GetBookCheckSelectIndex()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_2160");
	CString sGridAlias = _T("gridCHECKNOLIST");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetBookCheckSelectIndex()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetBookCheckSelectIndex()") );

	return pGrid->GetIdx();


	EFS_END
	return -1;

}

// �弭 ���� �Ϸ� ȭ�� �ʱ�ȭ
INT CScreenPlay::InitResultScreen()
{
	EFS_BEGIN

	INT ids;

	// ���α׷����� �ʱ�ȭ 0~100����
	ids = InitResultProgressBar();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitResultScreen()") );

	// �α� �ʱ�ȭ
	ids = InitResultLog();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("InitResultScreen()") );

	return 0;

	EFS_END
	return -1;

}

// �弭 ���� �Ϸ� ���α׷����� �ʱ�ȭ
INT CScreenPlay::InitResultProgressBar()
{
	EFS_BEGIN

	CProgressCtrl *pProgressCtrl;

	pProgressCtrl = GetProgressPtr( m_sResultCMAlias , m_sResultProgressAlias );
	if( pProgressCtrl == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitResultProgressBar()") );

	pProgressCtrl->SetRange(0,100);

	return 0;

	EFS_END
	return -1;

}

// �弭 ���� �Ϸ� �α� ��Ȳ �ʱ�ȭ
INT CScreenPlay::InitResultLog()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( m_sResultDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitResultLog()") );
	
	pDM->FreeData();

	ids = ResultLogAdd( _T("�۾�����") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("InitResultLog()") );

	return 0;

	EFS_END
	return -1;

}


// �弭 ���� �Ϸ� ���α׷��� , �α� ����
INT CScreenPlay::SetResultScreen( INT nPos , CString sLog )
{
	EFS_BEGIN

	INT ids;

	ids = ResultPorgressSetPos( nPos );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SetResultScreen()") ); 

	ids = ResultLogAdd( sLog );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SetResultScreen()") ); 

	return 0;

	EFS_END
	return -1;

}

// �弭 ���� �Ϸ� ���α׷��� ����
INT CScreenPlay::ResultPorgressSetPos( INT nPos )
{
	EFS_BEGIN

	CProgressCtrl *pProgressCtrl;

	pProgressCtrl = GetProgressPtr( m_sResultCMAlias , m_sResultProgressAlias );
	if( pProgressCtrl == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ResultPorgressSetPos()") );

	pProgressCtrl->SetPos(nPos);

	return 0;

	EFS_END
	return -1;

}

// �弭 ���� �Ϸ� �α� ����
INT CScreenPlay::ResultLogAdd( CString sLog )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( m_sResultDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ResultLogAdd()") );

	pDM->InsertRow(-1);

	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr,strGetData);
// 	CTime tCurrentTime = CLocCommonAPI::GetDBTime(m_pParentMgr);

	CString sCurrentTime = tCurrentTime.Format( _T("%Y/%m/%d %H:%M:%S") );

	ids = pDM->SetCellData( _T("�ð�") , sCurrentTime , pDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ResultLogAdd()") );

	ids = pDM->SetCellData( _T("����") , sLog , pDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ResultLogAdd()") );

	// �μ�Ʈ
	
	INT nIndex = pDM->GetRowCount()-1;

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , m_sResultCMAlias , m_sResultGridAlias , nIndex , nIndex  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ResultLogAdd()") ); 

	return 0;

	EFS_END
	return -1;

}

// ������ ���
INT CScreenPlay::GetBookChangeMode( INT &nMode )
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_2180");
	
	const INT nCnt = 3;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("����") , _T("�н�") , _T("����")
	};

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetBookChangeMode()") );

	CESL_Control_Element *pButtonElemnet;
	CButton *pButtonCtrl;

	for( INT i = 0 ; i < nCnt ; i++ )
	{	
		pButtonElemnet = pCM->FindControlElement( sControlAlias[i] );
		if( pButtonElemnet == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetBookChangeMode()") );

		pButtonCtrl = (CButton*)(m_pParentMgr->GetDlgItem( pButtonElemnet->CTRL_ID ));
		if( pButtonCtrl == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetBookChangeMode()") );

		if( pButtonCtrl->GetCheck() == 1 ) nMode = i;
	}
	
	return 0;

	EFS_END
	return -1;

}


INT CScreenPlay::ChangeBookCheckPrgressBarInit( INT nMax )
{
	EFS_BEGIN

	CProgressCtrl *pProgressCtrl = GetChangeBookCheckProgressBar();
	if( pProgressCtrl == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeBookCheckPrgressBarInit()") );
	pProgressCtrl->ShowWindow( SW_SHOW );
	pProgressCtrl->SetRange( 0 , nMax );

	return 0;

	EFS_END
	return -1;

}
INT CScreenPlay::ChangeBookCheckPrgressBarSetPos( INT nPos )
{
	EFS_BEGIN

	CProgressCtrl *pProgressCtrl = GetChangeBookCheckProgressBar();
	if( pProgressCtrl == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeBookCheckPrgressBarSetPos()") );
	pProgressCtrl->SetPos( nPos );

	return 0;

	EFS_END
	return -1;

}
INT CScreenPlay::ChangeBookCheckPrgressBarEnd()
{
	EFS_BEGIN

	CProgressCtrl *pProgressCtrl = GetChangeBookCheckProgressBar();
	if( pProgressCtrl == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeBookCheckPrgressBarEnd()") );
	pProgressCtrl->ShowWindow( SW_SHOW );

	return 0;

	EFS_END
	return -1;

}

CProgressCtrl* CScreenPlay::GetChangeBookCheckProgressBar()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_2180");
	CString sProgressAlias = _T("�����Ȳ");

	CProgressCtrl *pProgressCtrl = GetProgressPtr( sCMAlias , sProgressAlias );
	if( pProgressCtrl == NULL ) return NULL;

	return pProgressCtrl;

	EFS_END
	return NULL;

}

// �ϰ� ����
// ����,���� �� ���� ������� 
INT CScreenPlay::GetCheckMode( CString &sCheck )
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_2130");
	CString sControlAlias = _T("����");

	sCheck = _T("");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetCheckMode()") );

	CESL_Control_Element *ebtnMode;
	CButton *btnMode;

	ebtnMode = pCM->FindControlElement( sControlAlias );
	if( ebtnMode == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3, _T("GetCheckMode()") );
	btnMode = (CButton*)(m_pParentMgr->GetDlgItem( ebtnMode->CTRL_ID ));
	if( btnMode->GetCheck() == 1 )
		sCheck = _T("V");

	return 0;

	EFS_END
	return -1;

}

// ���ǿ� �´� �ڷ����� �˻�
// nMode - 0:�ڷ�Ǹ� , 1:�谡���� , 2:������ , 3:�ݳ���
INT CScreenPlay::IsCanCheck( INT nMode , CString sValue )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_2130");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsCanCheck()") );

	CString sCondition;
	if( nMode == 0 )
	{
		CString sControlAlias = _T("�ڷ�Ǹ�");
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias , sCondition , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsCanCheck()") );
		sCondition.TrimLeft(); sCondition.TrimRight();

		if( sCondition.IsEmpty() ) return 0;

		if( sValue.Compare( sCondition ) != 0 ) return 1;
	}
	else if( nMode == 1 )
	{
		INT nIndex;
		const INT nCnt = 7;
		CString sCheckButtonAlias[nCnt] = 
		{
			_T("����"),_T("�谡"),_T("����"),_T("�н�"),_T("����/����"),_T("��޽�û"),_T("�ļ�")
		};

		CESL_Control_Element *ebtnMode;
		CButton *btnMode;
		BOOL bCheck = FALSE;
		for( INT i = 0 ; i < nCnt ; i++ )
		{
			ebtnMode = pCM->FindControlElement( sCheckButtonAlias[i] );
			if( ebtnMode == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3, _T("IsCanCheck()") );
			btnMode = (CButton*)(m_pParentMgr->GetDlgItem( ebtnMode->CTRL_ID ));
			if( btnMode->GetCheck() == 1 )
				bCheck = TRUE;
		}

		if( !bCheck ) return 0;

		nIndex = 0;
		// ������
		if( sValue.Compare( _T("BOL211O") ) == 0 )
			nIndex = 0;
		else if( sValue.Compare( _T("BOL212O") ) == 0) 
			nIndex = 0;
		//=====================================================
		//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
		// 18.09.27 JMJ KOL.RED.2018.008
		else if( sValue.Compare( _T("BOL411O") ) == 0) 
			nIndex = 0;
		else if( sValue.Compare( _T("BOL511O") ) == 0) 
			nIndex = 0;
		// 2013.12.01 ADD BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
		else if( sValue.Compare( _T("BOL611O") ) == 0) 
			nIndex = 0;		
		//=====================================================
		else if( sValue.Compare( _T("BOL213O") ) == 0) 
			nIndex = 0;
		else if( sValue.Compare( _T("BOL214O") ) == 0)
			nIndex = 0;
		else if( sValue.Compare( _T("BOL215O") ) == 0 )
			nIndex = 0;
		else if( sValue.Compare( _T("SEL311O") ) == 0 )
			nIndex = 0;
		else if( sValue.Compare( _T("SEL312O") ) == 0 )
			nIndex = 0;
		//=====================================================
		//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
		// 18.09.27 JMJ KOL.RED.2018.008
		else if( sValue.Compare( _T("SEL411O") ) == 0 )
			nIndex = 0;
		else if( sValue.Compare( _T("SEL511O") ) == 0 )
			nIndex = 0;
		//=====================================================
		else if( sValue.Compare( _T("SEL313O") ) == 0 )
			nIndex = 0;
		else if( sValue.Compare( _T("SEL314O") ) == 0 )
			nIndex = 0;
		else if( sValue.Compare( _T("SEL315O") ) == 0 )
			nIndex = 0;
		// �谡��
		else if( sValue.Compare( _T("BOL112N") ) == 0 )
			nIndex = 1;
		else if( sValue.Compare( _T("SEL212N") ) == 0 )
			nIndex = 1;
		// ����
		else if( sValue.Compare( _T("BOL115O") ) == 0 )
			nIndex = 2;
		else if( sValue.Compare( _T("SEL215O") ) == 0 )
			nIndex = 2;
		// �н�
		else if( sValue.Compare( _T("BOL252N") ) == 0 )
			nIndex = 3;
		else if( sValue.Compare( _T("SEL216O") ) == 0 )
			nIndex = 3;
		// ��������
		else if( sValue.Compare( _T("BOL113O") ) == 0 )
			nIndex = 4;
		else if( sValue.Compare( _T("SEL213O") ) == 0 )
			nIndex = 4;
		// �ļ�
		else if( sValue.Compare( _T("BOL114O") ) == 0 )
			nIndex = 6;
		else if( sValue.Compare( _T("SEL214O") ) == 0 )
			nIndex = 6;
		
		ebtnMode = pCM->FindControlElement( sCheckButtonAlias[nIndex] );
		if( ebtnMode == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("IsCanCheck()") );
		btnMode = (CButton*)(m_pParentMgr->GetDlgItem( ebtnMode->CTRL_ID ));
		if( btnMode->GetCheck() != 1 )
			return 1;

	}
	// ������
	else if( nMode == 2 || nMode == 3 )
	{
		CString sControlAlias[2][2] = { { _T("������1") , _T("������2") } , { _T("�ݳ���1") , _T("�ݳ���2") } };
		CString sCondition[2];

		for( INT i=0 ; i<2 ; i++ )
		{
			ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[nMode-2][i] , sCondition[i] );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsCanCheck()") );
			sCondition[i].TrimLeft(); sCondition[i].TrimRight();
		}

		if( sCondition[0].IsEmpty() ) return 0;

		if( sCondition[1].IsEmpty() )
		{
			if( sValue.IsEmpty() ) return 1;
			if( sValue.Compare( sCondition[0] ) != 0 ) return 1;
		}
		else
		{
			if( sValue.IsEmpty() ) return 1;
			if( (sValue < sCondition[0]) || (sValue > sCondition[1]) ) return 1;
		}
	}

	return 0;

	EFS_END
	return -1;

}


// �̵�
INT CScreenPlay::MoveGrid( INT nIndex )
{
	EFS_BEGIN

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MoveGrid()") );

	
	if( nIndex < 1 || nIndex > pGrid->GetRows() - 1 )
	{
		AfxMessageBox( _T("�̵��� �� ���� �� ��ȣ�Դϴ�.") );
		return 0;	
	}
	
	pGrid->SetRow(nIndex-1);
	pGrid->SetCol(1);
	pGrid->SetTopRow(nIndex);

	return 0;

	EFS_END
	return -1;

}

INT CScreenPlay::AllControlDisplayGrid()
{
	EFS_BEGIN

	INT ids;

	const INT nCnt = 3;

	CString sCMDMAlias[nCnt][2] =
	{
		{ _T("CM_BO_LOC_2100") , _T("DM_BO_LOC_2100") } , 
		{ _T("CM_BO_LOC_2100_IN_DB_ETC_GRID") , _T("DM_BO_LOC_2100_ETC") } , 
		{ _T("CM_BO_LOC_2100_NOT_IN_DB_ETC_GRID") , _T("DM_BO_LOC_2100_NOT_IN_DB") }
	};

	ids = MainGridDisplay( sCMDMAlias[0][0] , sCMDMAlias[0][1] , NULL );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AllControlDisplayGrid") );
	
	CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM( sCMDMAlias[1][1] );
	if( pTmpDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AllControlDisplayGrid") );

	m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pTmpDM , _T("�ڷ�Ǳ���") , _T("�ڷ��") );
	
	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	// ����� �Ѱ��� ���̱� 
	for( INT i = 1 ; i < 1 ; i++ )
	//for( INT i = 1 ; i < 3 ; i++ )
	// KOL.RED.2018.201 ---------------------------------------------------------------------------
	{
		ids = m_pParentMgr->AllControlDisplay( sCMDMAlias[i][0] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AllControlDisplayGrid") );
	}

	return 0;

	EFS_END
	return -1;

}

CString CScreenPlay::ConvertPrice( CString price )
{
	price.Replace( _T(","), _T("") );
	INT len = price.GetLength();
	if( len < 4 )
		return price;

	INT cnt = 0;
	CString tmp;
	for( INT i = len -1; i > -1; i-- )
	{
		cnt++;
		tmp = price.Mid( i, 1 ) + tmp;

		if( 3 == cnt )
		{
			tmp = _T(",") + tmp;
			cnt = 0;
		}		
	}
	
	if( ',' == tmp.GetAt( 0 ) )
		price = tmp.Mid( 1, tmp.GetLength() - 1 );
	else
		price = tmp;
	
	return price;
}
