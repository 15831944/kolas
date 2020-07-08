// BL_BO_LOC_34.cpp: implementation of the CBL_BO_LOC_34 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BL_BO_LOC_34.h"
#include "direct.h"
// SBL
#include "SBL.h"
// 2011.08.26 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBL_BO_LOC_34::CBL_BO_LOC_34( CESL_Mgr *pWnd , CLonaManageValue* pLoanManageValue )
	: CSPF_Work(pWnd)
{
	m_pLoanManageValue = pLoanManageValue;

	m_pParent = pWnd;
}

CBL_BO_LOC_34::~CBL_BO_LOC_34()
{

}

INT CBL_BO_LOC_34::GetSelectMainGridCnt()
{
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetSelectMainGridCnt") );

	if( pDM->GetRowCount() < 1 ) return 0;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetSelectMainGridCnt") );

	pGrid->SelectMakeList();

	if( pGrid->SelectGetCount() < 1 ) return 0;

	return pGrid->SelectGetCount();
}

CESL_Grid* CBL_BO_LOC_34::GetMainGridPtr()
{
	CString sCMAlias = _T("CM_BO_LOC_3400");
	CString sGridAlias = _T("grid1");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) return NULL;
	
	return pGrid;
}


INT CBL_BO_LOC_34::SPFExcutePath( CString sPathAlias )
{
	INT ids = -1;

	if( sPathAlias.Compare( SEARCH ) == 0 )
	{
		ids = SearchBO_LOC_3410();
	}
	if( sPathAlias.Compare( RFID_SEARCH ) == 0 )
	{
		ids = SearchBO_LOC_3410FromRFID();
	}
	else if( sPathAlias.Compare( INSERT_DELIVERY ) == 0 )
	{
		ids = InsertDelivery();
	}
	else if( sPathAlias.Compare( UPDATE_DELIVERY ) == 0 )
	{
		ids = UpdateDelivery();
	}
	else if( sPathAlias.Compare( RECEIPT ) == 0 )
	{
		ids = Receipt();
	}
	else if( sPathAlias.Compare( DELIVERY ) == 0 )
	{
		ids = Delivery();
	}
	else if( sPathAlias.Compare( DELETE_DELIVERY ) == 0 )
	{
		ids = DeleteDelivery();
	}
	if( ids < 0 ) 
		AfxMessageBox(_T("����� ���������� �������� ���Ͽ����ϴ�.") );

	return ids;
}


/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-08 ~ 2005-12-08
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Sort
 	 
   [   �Լ�����   ] 
      ����ڷ���� �˻� ����
   [  PARAMETERS  ]
      NULL 

   [ RETURN VALUE ]
      1    : ���
      0    : ���� 
      ���� : ����

   [   �̴Ͻ���   ]
      1. SQL�� ����
	  2. �˻���� DM�� �����.	  
	  3. ȭ�鿡 �����ֱ� 
--------------------------------------------------------------------------------------------*/
INT CBL_BO_LOC_34::SearchBO_LOC_3410()
{
	BEGIN_SBL();

	INT ids;

	// ----------------------------------------------------------------------------
	// 1. SQL�� ����     
	// ----------------------------------------------------------------------------
	CString sWhere;
	ids = GetWhereBO_LOC_3410( sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BO_LOC_34::SearchBO_LOC_3410()") );
	if( ids > 0 ) return 1;

	// ----------------------------------------------------------------------------
	// 2. �˻���� DM�� �����.    
	// ----------------------------------------------------------------------------
	ids = MakeDMFromDB( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BO_LOC_34::SearchBO_LOC_3410()") );
	
	// ----------------------------------------------------------------------------
	// 3. ȭ�鿡 �����ֱ�     
	// ----------------------------------------------------------------------------
	ids = GridView();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BO_LOC_34::SearchBO_LOC_3410()") );

	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);

	return 0;
}


/// �˻� PATH
/// 1. SQL�� ����
/// 2. ���� �ڷ� get
/// 2.1.���� �ڷ� Sum �⺻ ���� DM
/// 3. ���� �ڷ� get
/// 3.1 ���� �ڷ� Sum �⺻ ���� DM
/// 4. ȭ�鿡 �����ֱ�
INT CBL_BO_LOC_34::SearchBO_LOC_3410FromRFID()
{
	BEGIN_SBL();

	INT ids;
	CString sWhere;

	GetTempData( _T("WHERE") , sWhere );
	
	ids = MakeDMFromDB( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchBO_LOC_3410") );
	
	ids = GridView();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchBO_LOC_3410") );
	
	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);
	
	return 0;
}

/// SQL �� ����
INT CBL_BO_LOC_34::GetWhereBO_LOC_3410( CString &sWhere )
{
	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3410");

	const INT nControlCnt = 3;
	TCHAR* sControlAlias[nControlCnt] =
	{
		_T("����") , _T("����1") , _T("����2")
	};

	//++2008.04.15 UPDATE BY PWR {{+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ��¥��Ʈ�ѷ� ����
	//
	// ���°� �޾ƿ���
	CString sGetData[nControlCnt];
	ids = m_pParentMgr->GetControlData( sCMAlias, sControlAlias[0], sGetData[0], -1, 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetWhereBO_LOC_3410") );
	sGetData[0].TrimLeft(); sGetData[0].TrimRight();
	//
	// (TO)����1�� �޾ƿ���
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM( sCMAlias );
	if( !pCM ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetWhereBO_LOC_3410") );
	CESL_Control_Element *pCETo = (CESL_Control_Element*)pCM->FindControlElement( sControlAlias[1] );
	CDateTimeCtrl *pDateTo = ( CDateTimeCtrl * )m_pParentMgr->GetDlgItem( pCETo->CTRL_ID );
	SYSTEMTIME tTime;
	DWORD dwResult = pDateTo->GetTime( &tTime );
	if (dwResult != GDT_VALID )
	{
		sGetData[1] = _T("");
	}
	else
	{
		sGetData[1].Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	}
	//
	// (FROM)����2�� �޾ƿ���
	CESL_Control_Element *pCEFrom = (CESL_Control_Element*)pCM->FindControlElement( sControlAlias[2] );
	CDateTimeCtrl *pDateFrom = ( CDateTimeCtrl * )m_pParentMgr->GetDlgItem( pCEFrom->CTRL_ID );
	dwResult = pDateFrom->GetTime( &tTime );
	if (dwResult != GDT_VALID )
	{
		sGetData[2] = _T("");
	}
	else
	{
		sGetData[2].Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	}
	//
	/// ���������� 
	/*CString sGetData[nControlCnt];
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , "GetWhereBO_LOC_3410" );
		sGetData[i].TrimLeft(); sGetData[i].TrimRight();
	}*/
	//--2008.04.15 UPDATE BY PWR -----------------------------------------------------------------------}}

	INT nType = -1; // ��ü

	if( sGetData[0].Compare( _T("��û�ڷ�") ) == 0 )
	{
		sWhere += _T(" D.STATUS = '0' ");
		nType = 0;

	}
	else if( sGetData[0].Compare( _T("�����Ϸ�") ) == 0 )
	{
		sWhere += _T(" D.STATUS = '1' ");
		nType = 1;

	}
	else if( sGetData[0].Compare( _T("��޿Ϸ�") ) == 0 )
	{
		sWhere += _T(" D.STATUS = '2' ");
		nType = 2;
	}

	// 2007.08.15 add by pdw
	// ��¥������ �ʼ��� �����Ѵ�.
	if( sGetData[1].IsEmpty() ) 
	{
		if( sGetData[0].Compare(_T("��ü")) == 0 )
		{
			sGetData[0] = _T("��û");
		}

		CString strMsg;
		strMsg.Format( _T("%s���ڸ� �Է��ؾ� �մϴ�."), sGetData[0].Mid(0,4) );
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return 1;
	}

	TCHAR* sFieldName[3] =
	{
		_T("D.REQUEST_DATE"), _T("D.RECEIPT_DATE") , _T("D.DELIVERY_DATE")
	};

	if ( sGetData[1][0] != '\0' || sGetData[2][0] != '\0' )
	{
		if( !sWhere.IsEmpty() ) sWhere += _T(" AND ");
		if ( nType == -1 ) sWhere += _T("( ");
	}
	
	for( INT i = 0 ; i < 3 ; i++ )
	{
		if( ( nType == -1 ) || ( nType == i ) )
		{			
			if( sGetData[2].IsEmpty() )
				sWhere += CString(sFieldName[i]) + CString(_T(" BETWEEN TO_DATE('"))+sGetData[1]+CString(_T(" 00:00:00' , 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('"))+sGetData[1]+CString(_T(" 23:59:59' , 'YYYY/MM/DD HH24:MI:SS')")); 
			else
				sWhere += CString(sFieldName[i]) + CString(_T(" BETWEEN TO_DATE('"))+sGetData[1]+CString(_T(" 00:00:00' , 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('"))+sGetData[2]+CString(_T(" 23:59:59' , 'YYYY/MM/DD HH24:MI:SS')")); 			
		
			if ( i < 2 )
			{
				if ( nType == -1 )
				{
					if( !sWhere.IsEmpty() ) sWhere += _T(" OR ");					
				}			
			}
		}		
	}

	if ( sGetData[1][0] != '\0' || sGetData[2][0] != '\0' )
	{
		if ( nType == -1 ) sWhere += _T(") ");
	}	

	return 0;
}


/// DB���� ���ڵ� ��������
INT CBL_BO_LOC_34::MakeDMFromDB( CString sWhere )
{
	INT ids;

	CESL_DataMgr TmpDM;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeDMFromDB") );

	pDM->FreeData();

	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeDMFromDB") );

	ids = MakeDMFromBODB( sWhere , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeDMFromDB") );

	ids = CLocCommonAPI::AddDM( &TmpDM , pDM , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeDMFromBODB") );

	ids = MakeDMFromSEDB( sWhere , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") );

	ids = CLocCommonAPI::AddDM( &TmpDM , pDM , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeDMFromBODB") );

	//ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("�ڷ�Ǳ���"), _T("�ڷ�Ǽ���"));

	return 0;
}

/// ���� ���̺��� ���ڵ� ��������
INT CBL_BO_LOC_34::MakeDMFromBODB( CString sWhere , CESL_DataMgr *pTmpDM )
{
	INT ids;

	ids = pTmpDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeDMFromBODB") );

	return 0;
}

/// ���� ���̺��� ���ڵ� ��������
INT CBL_BO_LOC_34::MakeDMFromSEDB( CString sWhere , CESL_DataMgr *pTmpDM  )
{
	INT ids;

	CString sTableNames = _T("SE_BOOK_TBL B, CO_DELIVERY_VIEW D, CO_LOAN_USER_TBL U, IDX_SE_TBL I, SE_VOL_TBL V ");
	CString sDefaultWhere = _T("D.BOOK_KEY=B.REC_KEY AND D.USER_KEY=U.REC_KEY AND B.SPECIES_KEY=I.REC_KEY AND B.VOL_KEY=V.REC_KEY ");

	// ���̺� �̸� ����
	pTmpDM->TBL_NAME = sTableNames;
	// �⺻ WHERE�� ����
	pTmpDM->CONDITION = sDefaultWhere;

	// �ʵ� ���� ����
	ids = CLocCommonAPI::ChangeDMFieldName( m_pParentMgr , pTmpDM , _T("������") , _T("I.AUTHOR") , _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeDMFromBODB") );

	ids = pTmpDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeDMFromBODB") );

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  ����ڷ�˻������ �׸��忡 �ݿ��Ѵ�.
//////////////////////////////////////////////////////////////////////
INT CBL_BO_LOC_34::GridView()
{
	INT ids;

	CString  sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( !pDM ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("GridView") );
		
	INT nRowCnt = pDM->GetRowCount();
	if( !nRowCnt )
	{
		AfxMessageBox( _T("�˻� ����� �����ϴ�.") );		
	}	
	else
	{		
		// û����ȣ ����
		ids = CLocCommonAPI::MakeShelfCode( m_pParentMgr , sDMAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridView") ); 

		CString sWorkingStatus;			// �谡����
		CString sDeliveryStatus;		// ��޻��� 

		// 2011.09.05 ADD : 3000Api
		CStringArray asDes1,asDes2,asDes3;

		// 2009.10.09 ADD BY KSJ : �����ڹ�ȣ�� �ֹι�ȣ�� ��� ó��
		CString strValue , strUserNo;
		ids = m_pParentMgr->GetManageValue( _T("����") , _T("����") , _T("�̿��ڰ���") , _T("����ȭ���ֹι�ȣǥ������") , strValue );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("GridView") );
		strValue.TrimLeft(); strValue.TrimRight(); strValue.MakeUpper();
		for(INT i = 0; i < nRowCnt; i++)
		{
			// �谡����
			ids = pDM->GetCellData( _T("�谡����") , i , sWorkingStatus );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("GridView") ); 
			ids = CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1040 , _T("GridView") ); 
			ids = pDM->SetCellData( _T("�谡����") , sWorkingStatus , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1050 , _T("GridView") ); 
			// ��޻���
			ids = pDM->GetCellData( _T("��޻���") , i , sDeliveryStatus );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1060 , _T("GridView") ); 
			if( sDeliveryStatus.Compare( _T("0") ) == 0 )
				sDeliveryStatus = _T("��û�ڷ�");
			else if( sDeliveryStatus.Compare( _T("1") ) == 0 )
				sDeliveryStatus = _T("�����Ϸ�");
			else if( sDeliveryStatus.Compare( _T("2") ) == 0 )
				sDeliveryStatus = _T("��޿Ϸ�");
			ids = pDM->SetCellData( _T("��޻���") , sDeliveryStatus , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1070 , _T("GridView") );
				
			// 2009.10.09 ADD BY KSJ : �����ڹ�ȣ�� �ֹι�ȣ�� ��� ó��
			ids = pDM->GetCellData( _T("�����ڹ�ȣ"), i , strUserNo );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1080 , _T("GridView") );
			if( _T("Y") != strValue )
			{
				CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pLoanManageValue->m_sIsUnityLoanService );
			}
			ids = pDM->SetCellData( _T("�����ڹ�ȣ����") , strUserNo , i );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1110 , _T("GridView") );

			// 2011.09.05 ADD : 3000Api
			if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
			{
				// 1:�ֹε�Ϲ�ȣ, 2:�̸���, 3:�޴���, 4:�������ּ�, 5:������ȭ, 6:�ٹ�����ȭ, 7:ȸ������й�ȣ
				CString strGetData;
				pDM->GetCellData(_T("�ּ�"), i, strGetData);
				asDes1.Add(strGetData);
				pDM->GetCellData(_T("����ó"), i, strGetData);
				asDes2.Add(strGetData);
				pDM->GetCellData(_T("�̸���"), i, strGetData);
				asDes3.Add(strGetData);
			}
		}

		// 2011.09.05 ADD : 3000Api
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && 0 < nRowCnt)
		{
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_DELIVERY_TBL"), _T("ADDRESS"), asDes1);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_DELIVERY_TBL"), _T("PHONE"), asDes2);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes3);
						pApi->CloseSocket();
					}
					for(i = 0; i < nRowCnt; i++)
					{
						pDM->SetCellData(_T("�ּ�"), asDes1.GetAt(i), i);
						pDM->SetCellData(_T("����ó"), asDes2.GetAt(i), i);
						pDM->SetCellData(_T("�̸���"), asDes3.GetAt(i), i);
					}
				}
				delete pApi;
				pApi = NULL;
			}
			asDes1.RemoveAll(); asDes1.FreeExtra();
			asDes2.RemoveAll(); asDes2.FreeExtra();
			asDes3.RemoveAll(); asDes3.FreeExtra();
		}
		
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("����Ҽ�����") , _T("����Ҽ�����") );
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("������������") , _T("������������") );

		// 2010.05.20 ADD BY KSJ : �ڷ�Ǳ��� �ڵ�->�������� ���̵��� ����
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("�ڷ�Ǳ���"), _T("�ڷ�Ǽ���"));	
	}
	
	CString sCMAlias = _T("CM_BO_LOC_3400");
	ids = m_pParentMgr->AllControlDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1120 , _T("GridView") ); 	 
	
	return 0;
}

/// �Է� PATH
/// 1. CONTROL,DM���� �Է°� ������ - �Է°� �˻�
/// 2. ������� DB INSERT
INT CBL_BO_LOC_34::InsertDelivery()
{
	BEGIN_SBL();

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3420");
	CString sUserDMAlias = _T("DM_BO_LOC_3420_USER");
	CString sBookDMAlias = _T("DM_BO_LOC_3420_BOOK");
	
	CString sMainDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pMainDM = m_pParentMgr->FindDM( sMainDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("") );

	CESL_DataMgr *pUserDM = m_pParentMgr->FindDM( sUserDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("") );
	
	CESL_DataMgr *pBookDM = m_pParentMgr->FindDM( sBookDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("") );

	if( pUserDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("�̿��ڸ� �������ֽʽÿ�.") );
		return 1;
	}
	if( pBookDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("å������ �������ֽʽÿ�.") );
		return 2;
	}

	// �̹� ��û�� ��û , ������ �ڷ����� ����
	CString sSql , sUserKey , sBookKey , sValue;
	ids = pUserDM->GetCellData( _T("�̿���KEY") , 0 , sUserKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	ids = pBookDM->GetCellData( _T("å����KEY") , 0 , sBookKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	//++2008.10.22 UPDATE BY PWR {{++
	// ���սý��� ����
	sSql.Format( _T("SELECT COUNT(*) FROM CO_DELIVERY_TBL ")
				 _T("WHERE USER_KEY=%s AND BOOK_KEY=%s AND STATUS IN('0','1') ")
				 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), sUserKey , sBookKey );
// 	sSql.Format( _T("SELECT COUNT(*) FROM CO_DELIVERY_TBL WHERE USER_KEY=%s AND BOOK_KEY=%s AND STATUS IN('0','1')") , sUserKey , sBookKey );
	//--2008.10.22 UPDATE BY PWR --}}
	ids = pMainDM->GetOneValueQuery( sSql , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );

	if( _ttoi(sValue) > 0 )
	{
		AfxMessageBox( _T("��� ��û�� �Ҽ� �����ϴ�.\r\n���� �̿��ڰ� ��� ��û�̳� �������� �ڷ��Դϴ�.") );
		return 1;
	}

	const INT nCnt = 8 + 1;
	CString sInputValue[nCnt];

	pMainDM->MakeRecKey( sInputValue[0] );

	ids = pBookDM->GetCellData( _T("�������±���") , 0 , sInputValue[1] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );

	ids = pBookDM->GetCellData( _T("å����KEY") , 0 , sInputValue[2] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	
	ids = pUserDM->GetCellData( _T("�̿���KEY") , 0 , sInputValue[3] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("��û����") , sInputValue[4] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	if( sInputValue[4].IsEmpty() )
	{
		AfxMessageBox( _T("��û���ڸ� �Է��Ͻʽÿ�") );
		return 3;
	}

	sInputValue[5] = _T("0") ;
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�ּ�") , sInputValue[6] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	if( sInputValue[6].IsEmpty() )
	{
		AfxMessageBox( _T("�ּҸ� �Է��Ͻʽÿ�") );
		return 3;
	}

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�����ȣ") , sInputValue[7] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	if( sInputValue[7].IsEmpty() )
	{
		AfxMessageBox( _T("�����ȣ�� �Է��Ͻʽÿ�") );
		return 3;
	}

	// ����ó�� �������� 
	// �׷��� ���� �߰���
	// 031031
	ids = m_pParentMgr->GetControlData( sCMAlias, _T("����ó"), sInputValue[ 8 ] );
	if( ids < 0 ) 
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	if( sInputValue[ 8 ].IsEmpty() )
	{
		AfxMessageBox( _T("����ó�� �Է��Ͻÿ�.") );
		return 3;
	}

	// 2007.03.14 ADD BY PDJ
	// ������ �ּҸ� DM���� ������Ʈ �Ѵ�.
	ids = pUserDM->SetCellData( _T("�ּ�") , sInputValue[6], 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );

	ids = pUserDM->SetCellData( _T("�����ȣ") ,sInputValue[7], 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	
	ids = pUserDM->SetCellData( _T("����ó") ,sInputValue[8], 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	
	// DB field Name
	CString sDBFieldName[nCnt] =
	{
		_T("REC_KEY") , _T("PUBLISH_FORM_CODE") , _T("BOOK_KEY") , _T("USER_KEY") , _T("REQUEST_DATE") , 
		_T("STATUS") , _T("ADDRESS") , _T("ZIP_CODE"), _T("PHONE")
	};
	CString sDBFieldType[nCnt] =
	{
		_T("NUMERIC") , _T("STRING") , _T("NUMERIC") , _T("NUMERIC") , _T("STRING") , 
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") 
	};

	// 2011.09.06 ADD : 3000Api
	BOOL bSBUse = FALSE;
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
	{	
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				pMainDM->StartFrame();
				pMainDM->InitDBFieldData();
				CStringArray DataArray;
				for(INT i = 0; i < nCnt; i++)
				{
					if(_T("ADDRESS") == sDBFieldName[i] || _T("PHONE") == sDBFieldName[i])
					{
						if(TRUE == p3000Api->ConnectSocket())
						{
							DataArray.Add(sInputValue[i]);
							if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_DELIVERY_TBL"), sDBFieldName[i], DataArray))
							{
								sInputValue[i] = DataArray.GetAt(0);
							}
							p3000Api->CloseSocket();
							DataArray.RemoveAll(); DataArray.FreeExtra();
						}
					}
					pMainDM->AddDBFieldData(sDBFieldName[i], sDBFieldType[i], sInputValue[i]);
				}
				bSBUse = TRUE;
			}
			delete p3000Api;
			p3000Api = NULL;
		}
	}
	if(FALSE == bSBUse)
	{
		pMainDM->StartFrame();
		pMainDM->InitDBFieldData();
		for( INT i = 0 ; i < nCnt ; i++ )
		{
			pMainDM->AddDBFieldData(sDBFieldName[i], sDBFieldType[i], sInputValue[i]);
		}
	}

	pMainDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );

	//++2008.10.22 ADD BY PWR {{++
	// ���սý��� ����
	pMainDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );
	//--2008.10.22 ADD BY PWR --}}

	pMainDM->MakeInsertFrame( _T("CO_DELIVERY_TBL") );

	ids = pMainDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertDelivery") );

	pMainDM->EndFrame();

	// (M_2004-1109_HAN)
	// �ѹ��� �о�� �Է��ϱ⿡�� �ʹ� ���� �����Ͷ�
	// DB���� ��Ϲ�ȣ�� �̿��� �о�� ������.
	////////////////////////////
	// ���� DM�� �����Ѵ�.
	CESL_DataMgr TmpDM;
	CString sRegNo = _T("");
	CString sWhere = _T("");
	INT nIndex = 0;

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pMainDM , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DeliveryDBProc") );

	if ( !sBookKey.IsEmpty( ) )
		sWhere.Format( _T("B.REC_KEY=%s AND D.USER_KEY=%s "), sBookKey, sUserKey );

	// 2007.09.03 add by pdw
	// �����ڷ� �Է½� ���̺�� ����ǵ��� ����
	if( _T("SE") == sInputValue[1] )
	{
		TmpDM.TBL_NAME.Replace( _T("BO_BOOK_TBL") , _T("SE_BOOK_TBL") );
		TmpDM.TBL_NAME.Replace( _T("IDX_BO_TBL") , _T("IDX_SE_TBL") );
	}

	ids = TmpDM.RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );

	pMainDM->InsertRow(-1);
	nIndex = pMainDM->GetRowCount() - 1 ;
	// 2. SELECT �� ���� ���� DM���� �̵�
	ids = CLocCommonAPI::AddDM( &TmpDM, pMainDM, 0, nIndex, m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DeliveryDBProc") ); 

	GridView( );

	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);	
	
	AfxMessageBox( _T("��û�Ǿ����ϴ�.") );

	return 0;
/*
CString sUpdateData[16];
ids = pBookDM->GetCellData( _T("������"), 0 , sUpdateData[0] );
ids = pBookDM->GetCellData( _T("��Ϲ�ȣ"), 0 , sUpdateData[1] );
ids = pBookDM->GetCellData( _T("�谡����"), 0 , sUpdateData[2] );
ids = pBookDM->GetCellData( _T("������"), 0 , sUpdateData[3] );
ids = pBookDM->GetCellData( _T("����"), 0 , sUpdateData[4] );
ids = pUserDM->GetCellData( _T("�����ڸ�"), 0 , sUpdateData[5] );
ids = pUserDM->GetCellData( _T("�����ڹ�ȣ"), 0 , sUpdateData[6] );
pMainDM->InsertRow(-1);
INT nIndex = pMainDM->GetRowCount() -1 ;
pMainDM->SetCellData( _T("������") , sUpdateData[0] , nIndex );
pMainDM->SetCellData( _T("å����KEY") , sInputValue[2] , nIndex );
pMainDM->SetCellData( _T("��Ϲ�ȣ") , sUpdateData[1] , nIndex );
pMainDM->SetCellData( _T("�谡����") , sUpdateData[2] , nIndex );
pMainDM->SetCellData( _T("�ּ�") , sInputValue[6] , nIndex );
pMainDM->SetCellData( _T("����ó") , sInputValue[ 8 ] , nIndex );
pMainDM->SetCellData( _T("��û����") , sInputValue[4] , nIndex );
pMainDM->SetCellData( _T("��޻���") , _T("��û�ڷ�") , nIndex );
pMainDM->SetCellData( _T("�����ȣ") , sInputValue[7] , nIndex );
pMainDM->SetCellData( _T("������") , sUpdateData[3] , nIndex );
pMainDM->SetCellData( _T("����") , sUpdateData[4] , nIndex );
pMainDM->SetCellData( _T("�����ڸ�") , sUpdateData[5] , nIndex );
pMainDM->SetCellData( _T("�̿���KEY") , sInputValue[3] , nIndex );
pMainDM->SetCellData( _T("�����ڹ�ȣ") , sUpdateData[6] , nIndex );
pMainDM->SetCellData( _T("�������KEY") , sInputValue[0] , nIndex );
pMainDM->SetCellData( _T("�������±���") , sInputValue[1] , nIndex );
// ���� �߰�	2004-02-13
// û����ȣ ���� ����
pMainDM->SetCellData( _T("û����ȣ"), m_pParentMgr->FindDM( _T("DM_BO_LOC_3420_BOOK") )->GetCellData( _T("û����ȣ"), 0 ), nIndex );
//CLocCommonAPI::InsertGridRow( m_pParentMgr , _T("CM_BO_LOC_3400") , _T("grid1") , nIndex , nIndex );
*/
}

/// ���� PATH
/// 1. CONTROL���� �Է°� ������ - �Է°� �˻�
/// 2. ������� DB UPDATE
INT CBL_BO_LOC_34::UpdateDelivery()
{
	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3420_1");
	
	CString sMainDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pMainDM = m_pParentMgr->FindDM( sMainDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("") );

	INT nIndex;
	GetTempData( _T("�����ε���") , nIndex );



	const INT nCnt = 10;
	CString sInputValue[nCnt];

	pMainDM->GetCellData( _T("�������KEY") , nIndex , sInputValue[0] );

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("��û����") , sInputValue[4] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("��������") , sInputValue[7] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�������") , sInputValue[8] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("����ó") , sInputValue[9] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�ּ�") , sInputValue[5] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	if( sInputValue[5].IsEmpty() )
	{
		AfxMessageBox( _T("�ּҸ� �Է��Ͻʽÿ�") );
		return 3;
	}

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�����ȣ") , sInputValue[6] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	if( sInputValue[6].IsEmpty() )
	{
		AfxMessageBox( _T("�����ȣ�� �Է��Ͻʽÿ�") );
		return 3;
	}

	// DB field Name
	CString sDBFieldName[nCnt] =
	{
		_T("REC_KEY") , _T("PUBLISH_FORM_CODE") , _T("BOOK_KEY") , _T("USER_KEY") , _T("REQUEST_DATE") , 
		_T("ADDRESS") , _T("ZIP_CODE") , _T("RECEIPT_DATE") , _T("DELIVERY_DATE") , _T("PHONE")
	};
	CString sDBFieldType[nCnt] =
	{
		_T("NUMERIC") , _T("STRING") , _T("NUMERIC") , _T("NUMERIC") , _T("STRING") , 
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING")
	};

	pMainDM->StartFrame();
	pMainDM->InitDBFieldData();

	// 2011.09.06 ADD : 3000Api
	BOOL bSBUse = FALSE;
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
	{
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				CString strGetData;
				CStringArray DataArray;
				for(INT i = 4; i < nCnt; i++)
				{
					strGetData = sInputValue[i];
					if(_T("ADDRESS") == sDBFieldName[i] || _T("PHONE") == sDBFieldName[i])
					{
						if(TRUE == p3000Api->ConnectSocket())
						{
							DataArray.Add(strGetData);
							if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_DELIVERY_TBL"), sDBFieldName[i], DataArray))
							{
								strGetData = DataArray.GetAt(0);
							}
							DataArray.RemoveAll(); DataArray.FreeExtra();
							p3000Api->CloseSocket();
						}
					}
					pMainDM->AddDBFieldData(sDBFieldName[i], sDBFieldType[i], strGetData);
				}
				bSBUse = TRUE;
			}
			delete p3000Api;
			p3000Api = NULL;
		}
	}
	if(FALSE == bSBUse)
	{
		for(INT i = 4; i < nCnt; i++)
		{
			pMainDM->AddDBFieldData(sDBFieldName[i], sDBFieldType[i], sInputValue[i]);
		}
	}

	pMainDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );
	pMainDM->MakeUpdateFrame( _T("CO_DELIVERY_TBL") , _T("REC_KEY") , _T("NUMERIC") , sInputValue[0] );

	ids = pMainDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertDelivery") );

	pMainDM->EndFrame();

	pMainDM->SetCellData( _T("�ּ�") , sInputValue[5] , nIndex );
	pMainDM->SetCellData( _T("��û����") , sInputValue[4] , nIndex );
	pMainDM->SetCellData( _T("��������") , sInputValue[7] , nIndex );
	pMainDM->SetCellData( _T("�������") , sInputValue[8] , nIndex );
	pMainDM->SetCellData( _T("�����ȣ") , sInputValue[6] , nIndex );
	pMainDM->SetCellData( _T("����ó") , sInputValue[9] , nIndex );
	CLocCommonAPI::InsertGridRow( m_pParentMgr , _T("CM_BO_LOC_3400") , _T("grid1") , nIndex , nIndex );
	AfxMessageBox( _T("�����Ͽ����ϴ�.") );

	return 0;
}




/// ����ó�� PATH
/// �ϰ� ����
/// 1. ������ �ڷῡ ����
/// 2. ������� DB UPDATE
/// 3. å���� DB UPDATE
INT CBL_BO_LOC_34::Receipt()
{
	INT ids;
	CString strFuncName = _T("CBL_BO_LOC_34::Receipt()");

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , strFuncName);

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// �Է°� �˻�
	CString sInputDate;
	ids = GetReceiptInputDate( sInputDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , strFuncName);

	if( sInputDate.IsEmpty() )
	{
		AfxMessageBox( _T("�������ڸ� �Է����ֽʽÿ�.") );
		return 2;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , strFuncName);

	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus; // �ڷ����
	CString sStatus ; // ��� ����
	
	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nReservationFail = 0;	// �����̷� ����
	INT nUserStatusFail  = 0;   // ȸ������ ����

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));

	INT nIndex;
	CString sGet[4];
	CString sSql , sResult ;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CString sBookKey , sPublishForm , strDeliveryKey;
		ids = pDM->GetCellData( _T("å����KEY") , nIndex , sBookKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , strFuncName);

		// 2010.02.09 ADD BY KSJ : �˻� ���� �������� ���� �߰�
		ids = pDM->GetCellData( _T("�������KEY") , nIndex , strDeliveryKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , strFuncName);

		sSql.Format(
			_T("SELECT D.STATUS, B.WORKING_STATUS FROM BO_BOOK_TBL B, CO_DELIVERY_VIEW D WHERE D.BOOK_KEY=B.REC_KEY AND B.REC_KEY = %s AND D.REC_KEY = %s")
			,sBookKey,strDeliveryKey);
		TmpDM.GetValuesQuery(sSql, sGet, 2);

		if(sGet[0].Compare(_T("0")) != 0)
		{
			pGrid->SelectGetNext();
			continue;
		}
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( sGet[1] , 1 );
		if( sGet[1].Compare( _T("��ġ�ڷ�") ) != 0 )
		{
			pGrid->SelectGetNext();
			continue;
		}
// 		ids = pDM->GetCellData( _T("��޻���") , nIndex , sStatus );
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , strFuncName);
// 
// 		if( sStatus.Compare( _T("��û�ڷ�") ) != 0 )
// 		{
// 			pGrid->SelectGetNext();
// 			continue;
// 		}
// 		
// 		// 2006-04-12 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		// ��ġ�ڷḸ ����ó���� �����ϴ�.
// 		ids = pDM->GetCellData( _T("�谡����") , nIndex , sWorkingStatus );
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , strFuncName);
// 
// 		if( sWorkingStatus.Compare( _T("��ġ�ڷ�") ) != 0 )
// 		{
// 			pGrid->SelectGetNext();
// 			continue;
// 		}
// 		// ------------------------------------------------------------------------------------------------------
// 		
		// 2007.08.08 add by pdw
		// �����ڷ�� ����ó���� �Ұ����ϴ�.

		// 2009.05.19 UPDATE BY CJY : DM�� å���°� �ƴ� DB�� �ִ� �ֽŻ��·� �Ǻ��Ѵ�. (����Ӹ� �ƴ϶� �������� �ڷ������� �Ǻ�)
// 		sSql.Format( _T("SELECT COUNT(*) FROM LS_WORK_T01 WHERE BOOK_KEY=%s AND STATUS = '3'") , sBookKey );
// 		ids = pDM->GetOneValueQuery( sSql , sResult );
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , strFuncName);
// 		if( _ttoi(sResult) > 0 )
		sSql.Format( _T("SELECT 'L' FROM LS_WORK_T01 WHERE BOOK_KEY=%s AND STATUS IN ('0', '2', 'L', 'D', '5', '6', '7') UNION SELECT 'R' FROM LS_WORK_T01 WHERE BOOK_KEY=%s AND STATUS = '3' UNION SELECT NULL FROM DUAL") , sBookKey, sBookKey );
		ids = pDM->GetOneValueQuery( sSql , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , strFuncName);
		if(_T("") != sResult)
		{
			if(_T("R") == sResult)
			{
				nReservationFail++;
			}
			pGrid->SelectGetNext();
			continue;
		}

		BOOL bIsReceiptUserStatus = TRUE;
		
		CString sUserKey;
 		ids = pDM->GetCellData( _T("�̿���KEY") , nIndex , sUserKey );
 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );		
 
		sSql.Format( _T("SELECT U.USER_CLASS, U.LOAN_STOP_DATE, U.MEMBER_CLASS, U.LOST_USER_CARD FROM CO_LOAN_USER_TBL U WHERE U.REC_KEY=%s") , sUserKey );
 		ids = TmpDM.GetValuesQuery(sSql , sGet, 4);
 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
 
		if(_T("1") == sGet[0])
		{
 			// ��������ȸ���̶� ������������ �����Ϻ��� �����̸� ��������ȸ������ ���� �ʴ´�.
 			CString strLoanStopDate;
 			strLoanStopDate = sGet[1];
 			
 			COleDateTime oleLoanStopDate;
 			int nYear = 1, nMonth = 1, nDay = 1;
 			_stscanf(strLoanStopDate.GetBuffer(0), _T("%d/%d/%d"), &nYear, &nMonth, &nDay);
 			
 			oleLoanStopDate.SetDate(nYear, nMonth, nDay);
 			
 			COleDateTime oleToday = COleDateTime::GetCurrentTime();
 			oleToday.SetDate(oleToday.GetYear(), oleToday.GetMonth(), oleToday.GetDay());
 			
 			if(oleToday > oleLoanStopDate)
 			{
 			}
 			else
 			{
 				bIsReceiptUserStatus = FALSE;
 			}
 		}
 		if(_T("2") == sGet[0] || _T("3") == sGet[0])
 		{
 			bIsReceiptUserStatus = FALSE;
 		}
 		// ȸ������ 
 		if(_T("1") == sGet[2]) 
 		{ // ��ȸ��
 			bIsReceiptUserStatus = FALSE; 
 		}
 		// 2008.05.28 ADD BY PWR : ��ȸ���� ȸ�������Ŀ� ������ �����ϹǷ� ����
 		else if(_T("2") == sGet[2])
 		{ // ��ȸ��
 			bIsReceiptUserStatus = FALSE; 
 		}
 		// ȸ�����н�ȸ���̸�
 		if(_T("Y") == sGet[3]) { bIsReceiptUserStatus = FALSE; }
 
 		if(FALSE == bIsReceiptUserStatus)
 		{
  			nUserStatusFail++;
  			pGrid->SelectGetNext();
  			continue;
 		}

		//++2009.01.09 UPDATE BY CJY {{++
		//DEL	DM�� �����ϵ��� ����
// 		BOOL bIsReceiptUserStatus = TRUE;
// 
// 		CString strUserClass;
// 		ids = pDM->GetCellData( _T("�ҷ�ȸ������") , nIndex, strUserClass);
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , strFuncName); 
// 		if(_T("1") == strUserClass) 
// 		{ 
// 			// ��������ȸ���̶� ������������ �����Ϻ��� �����̸� ��������ȸ������ ���� �ʴ´�.
// 			CString strLoanStopDate;
// 			ids = pDM->GetCellData(_T("����������"), nIndex, strLoanStopDate);
// 			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , strFuncName);
// 			
// 			COleDateTime oleLoanStopDate;
// 			int nYear = 1, nMonth = 1, nDay = 1;
// 			_stscanf(strLoanStopDate.GetBuffer(0), _T("%d/%d/%d"), &nYear, &nMonth, &nDay);
// 			
// 			oleLoanStopDate.SetDate(nYear, nMonth, nDay);
// 			
// 			COleDateTime oleToday = COleDateTime::GetCurrentTime();
// 			oleToday.SetDate(oleToday.GetYear(), oleToday.GetMonth(), oleToday.GetDay());
// 			
// 			if(oleToday > oleLoanStopDate)
// 			{
// 			}
// 			else
// 			{
// 				bIsReceiptUserStatus = FALSE;
// 			}
// 		}
// 		if(_T("2") == strUserClass || _T("3") == strUserClass)
// 		{
// 			bIsReceiptUserStatus = FALSE;
// 		}
// 
// 		// ȸ������ 
// 		CString strMemberClass;
// 		ids = pDM->GetCellData( _T("ȸ������") , nIndex, strMemberClass);
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , strFuncName); 
// 		if(_T("1") == strMemberClass) 
// 		{ // ��ȸ��
// 			bIsReceiptUserStatus = FALSE; 
// 		}
// 		// 2008.05.28 ADD BY PWR : ��ȸ���� ȸ�������Ŀ� ������ �����ϹǷ� ����
// 		else if(_T("2") == strMemberClass)
// 		{ // ��ȸ��
// 			bIsReceiptUserStatus = FALSE; 
// 		}
// 		
// 		// ȸ�����н�ȸ���̸�
// 		CString strLostUserCard;
// 		ids = pDM->GetCellData( _T("ȸ�����нǿ���") , nIndex, strLostUserCard);
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , strFuncName); 
// 		if(_T("Y") == strLostUserCard) { bIsReceiptUserStatus = FALSE; }
// 
// 		if(FALSE == bIsReceiptUserStatus)
// 		{
//  			nUserStatusFail++;
//  			pGrid->SelectGetNext();
//  			continue;
// 		}
//DEL	�����ҽ�
//DEL 		// 2007.08.28 add by pdw
//DEL 		// ȸ�����¿� ���� ����ó���� �Ұ����ϴ�.
//DEL 		CString sUserKey;
//DEL 		ids = pDM->GetCellData( _T("�̿���KEY") , nIndex , sUserKey );
//DEL 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );		
//DEL 
//DEL 		sSql.Format( _T("SELECT USER_CLASS FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s") , sUserKey );
//DEL 		ids = pDM->GetOneValueQuery( sSql , sResult );
//DEL 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
//DEL 
//DEL 		if( _T("2") == sResult || _T("3") == sResult )
//DEL 		{
//DEL 			nUserStatusFail++;
//DEL 			pGrid->SelectGetNext();
//DEL 			continue;
//DEL 		}
//DEL 		else if( _T("1") == sResult )
//DEL 		{
//DEL 			sSql.Format(_T("SELECT TO_CHAR(LOAN_STOP_DATE, 'YYYY/MM/DD') FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s") , sUserKey);
//DEL 			ids = pDM->GetOneValueQuery( sSql , sResult );
//DEL 			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
//DEL 
//DEL 			if( !sResult.IsEmpty() )
//DEL 			{
//DEL 				//++2008.05.13 UPDATE BY PWR {{++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//DEL 				// CTime -> COleDateTime ���� ���� ( ������������ Ŀ�� �����߻� )
//DEL 				COleDateTime tTmpCurrent = COleDateTime::GetCurrentTime();
//DEL 				//CTime tTmpCurrent = CLocCommonAPI::GetDBTime(m_pParentMgr);
//DEL 				COleDateTime tCurrent = COleDateTime( tTmpCurrent.GetYear() , tTmpCurrent.GetMonth() , tTmpCurrent.GetDay() , 0 , 0 , 0 );
//DEL 				COleDateTime tLoanStopDate = COleDateTime( _ttoi(sResult.Mid(0,4)) , _ttoi(sResult.Mid(5,2)) , _ttoi(sResult.Mid(8,2)) , 0 , 0 , 0 );
//DEL 				//--2008.05.13 UPDATE BY PWR ------------------------------------------------------------}}
//DEL 				if( tCurrent <= tLoanStopDate )
//DEL 				{
//DEL 					nUserStatusFail++;
//DEL 					pGrid->SelectGetNext();
//DEL 					continue;
//DEL 				}
//DEL 			}
//DEL 			else
//DEL 			{
//DEL 				nUserStatusFail++;
//DEL 				pGrid->SelectGetNext();
//DEL 				continue;
//DEL 			}
//DEL 
//DEL 		}
//DEL 		//++2008.07.15 ADD BY PWR {{++
//DEL 		// �������н�ȸ�� ����ó�� �Ұ���
//DEL 		sResult = m_pLoanManageValue->m_sLoanIsLostUserCard;
//DEL 		if(sResult == _T("Y") )
//DEL 		{
//DEL 			sSql.Format( _T("SELECT LOST_USER_CARD FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), sUserKey );
//DEL 			ids = pDM->GetOneValueQuery( sSql , sResult );
//DEL 			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("IsLoan()") );
//DEL 			if( _T("Y") == sResult ) 
//DEL 			{
//DEL 				nUserStatusFail++;
//DEL 				pGrid->SelectGetNext();
//DEL 				continue;
//DEL 			}
//DEL 		}
//DEL 		//--2008.07.15 ADD BY PWR --}}
		//--2009.01.09 UPDATE BY CJY --}}


		//  DB�۾�
		ids = ReceiptDBProc( nIndex , &TmpDM , sInputDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , strFuncName);
		if( ids > 0 ) return ids;

		//  DM �۾�
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , strFuncName);

		//  ȭ�� �۾�
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , strFuncName);

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
	{
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷḦ ���� ó�� �Ͽ����ϴ�.") , nSelectCnt );
	}
	else 
	{
		sResultMsg.Format( _T("[������ ��ü �ڷ� �Ǽ�]           : %d \r\n")
			               _T("[����ó�� ���� �Ǽ�]               : %d \r\n")
			               _T("[����ó�� ���� �Ǽ�(�ڷ����)] : %d \r\n")
						   _T("[����ó�� ���� �Ǽ�(�����ڷ�)] : %d \r\n")
						   _T("[����ó�� ���� �Ǽ�(ȸ������)] : %d "), nSelectCnt, nSuccessCnt, 
						   nSelectCnt-nSuccessCnt-nReservationFail-nUserStatusFail, nReservationFail, nUserStatusFail );
	}
	AfxMessageBox( sResultMsg );

	return 0;
}

/// ���� ������ DB �۾�
/// 2. ������� UPDATE
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CBL_BO_LOC_34::ReceiptDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sInputDate )
{
	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDBProc") ); 

	// 2010.01.27 ADD BY PWR : TEMP�ʱ�ȭ
	pTmpDM->FreeData();

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepairDBProc") ); 

	pDM->StartFrame();	

	// ������ ����
	pDM->InitDBFieldData();
	// 2. ��� ���� UPDATE
	CString sDeliveryRecKey;
	ids = pTmpDM->GetCellData( _T("�������KEY") , 0 , sDeliveryRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepairDBProc") ); 

	CString sReceiptDate;	
	CTime tReceiptDate = CTime::GetCurrentTime();
	sReceiptDate.Format(
		_T("TO_DATE('%s %0d:%d:%d', 'YYYY/MM/DD HH24:MI:SS')"), 
		sInputDate, tReceiptDate.GetHour(), tReceiptDate.GetMinute(), tReceiptDate.GetSecond());

	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("1") );
// 	pDM->AddDBFieldData( _T("RECEIPT_DATE") , _T("STRING") , sInputDate );
	pDM->AddDBFieldData( _T("RECEIPT_DATE") , _T("NUMERIC") , sReceiptDate );
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeUpdateFrame( _T("CO_DELIVERY_TBL") , _T("REC_KEY") , _T("NUMERIC") , sDeliveryRecKey );
	pDM->InitDBFieldData();

	// 2. 1 TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("��޻���") , _T("�����Ϸ�"), 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("��������") , sInputDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RegRepairDBProc") ); 


	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();

	// 2005.03.14 ADD BY PDJ 
	// å���� ������Ʈ
	// ������ �̿��ڰ� ������ �� ������ 
	// å�� ���¸� �����Ϸ�� �����Ѵ�.
	CString sBookRecKey , sWorkingStatus , sPreWorkingStatus , sPublishForm , sNewWorkingStatus;
	CString sUserRecKey;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�谡����") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�������±���") , 0 , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�̿���KEY") , 0 , sUserRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 

	CString sTableName;	
	CString sIDXTblName;	
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	CString sTmpIDXTblName;	
	//=====================================================
	
	CString sProcName;	

	if( sPublishForm.Compare( _T("MO") ) == 0 )
	{
		sTableName = _T("BO_BOOK_TBL");
		sNewWorkingStatus = _T("BOL215O");
		sIDXTblName = _T("IDX_BO_TBL");
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		sTmpIDXTblName = _T("IDX_TMP_BO_TBL");
		//=====================================================		
		sProcName = _T("TESTWORKSTATUS");
	}
	else
	{
		sTableName = _T("SE_BOOK_TBL");
		sNewWorkingStatus = _T("SEL315O");
		sIDXTblName = _T("IDX_SE_TBL");
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		sTmpIDXTblName = _T("IDX_TMP_SE_TBL");
		//=====================================================		
		sProcName = _T("TESTSEWORKSTATUS");
	}


	// ������ ����
	// 1. å���� UPDATE
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sNewWorkingStatus );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	ids = CLocCommonAPI::GetWorkingStatusDescToCode( sNewWorkingStatus , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") ); 

	ids = pTmpDM->SetCellData( _T("�谡����") , sNewWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("DeliveryDBProc") ); 

	// ���� �߰�
	CString speciesKey = pDM->GetCellData( _T("������KEY"), nDMIndex );
	CString query;
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	query.Format( _T("UPDATE %s SET WORKING_STATUS=%s( %s ) WHERE REC_KEY=%s;\n")
// 				  _T("INSERT INTO %s( SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE, REC_KEY) VALUES( %s, 'I', SYSDATE, ESL_SEQ.NEXTVAL );"),
// 				sTableName, sProcName, speciesKey, speciesKey, sTmpIDXTblName, speciesKey );
// 	if( !speciesKey.IsEmpty() )
// 		pDM->AddFrame( query );	
	query.Format( _T("UPDATE %s SET WORKING_STATUS=%s( %s ) WHERE REC_KEY=%s;"),sTableName, sProcName, speciesKey, speciesKey);
	if( !speciesKey.IsEmpty() )
 		pDM->AddFrame( query );	
	//=====================================================
	

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("DeliveryDBProc") ); 

	ids = pDM->EndFrame();

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RegRepairDBProc") ); 

	return 0;
}

/// �ڷ� ���濡 ������ DM�� ������ ���� ���Ǵ� DM�� ����Ѵ�.
INT CBL_BO_LOC_34::DBInsertUpdateDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

	return 0;
}

/// 1. INDEX�κ��� �׸����� ���� ����
INT CBL_BO_LOC_34::DBInsertUpdateGridProc( INT nDMIndex )
{
	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairGridProc") );

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairGridProc") );

	return 0;
}


INT CBL_BO_LOC_34::GetReceiptInputDate( CString &sInputDate )
{
	CString sCMAlias = _T("CM_BO_LOC_3430");

	INT ids;

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("��������") , sInputDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetReceiptInputDate") );
	sInputDate.TrimRight(); sInputDate.TrimLeft();

	return 0;
}

// ���α׷����� ����
CLocProgressBarDlg* CBL_BO_LOC_34::CreateProgressBarDlg( INT nUpper )
{
	CLocProgressBarDlg* pProgressBar = new CLocProgressBarDlg(m_pParentMgr);
	pProgressBar->Create( m_pParentMgr );
	pProgressBar->ShowWindow( SW_SHOW );
	pProgressBar->InitRange(nUpper);

	return pProgressBar;
}

// ���α׷����� ����
INT CBL_BO_LOC_34::DeleteProgressBarDlg( CLocProgressBarDlg *pProgressBarDlg )
{
	if( pProgressBarDlg != NULL ) 
		delete pProgressBarDlg;
	pProgressBarDlg = NULL;
	
	return 0;
}




/// ��� ó�� PATH
/// �ϰ� ����
/// 1. ������ �ڷῡ ����
/// 2. ������� DB UPDATE
/// 3. å���� DB UPDATE
/// 4. �̿������� DB UPDATE
/// 5. ����/�ݳ� ���� DB UPDATE
INT CBL_BO_LOC_34::Delivery()
{
	INT ids;

	// �׸��� ������ ����
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("Delivery") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// �Է°� �˻�
	CString sInputDate,sLoanPeriod;
	ids = GetDeliveryInputDate( sInputDate ,sLoanPeriod );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("Delivery") );

	if( sInputDate.IsEmpty() )
	{
		AfxMessageBox( _T("�������ڸ� �Է����ֽʽÿ�.") );
		return 2;
	}
	if( sLoanPeriod.IsEmpty() )
	{
		AfxMessageBox( _T("��������� �Է����ֽʽÿ�.") );
		return 3;
	}

	// DM ���� ����
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("Delivery") );

	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus; // �ڷ����
	CString sStatus ; // ��� ����

	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	InitLog(_T("-- ��޿Ϸ� ó�� ���� ���� --\r\n"));

	INT nIndex;
	CString sRegNo , sUserNo;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("��޻���") , nIndex , sStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("Delivery") );

		ids = pDM->GetCellData( _T("��Ϲ�ȣ") , nIndex , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("Delivery") );
		
		ids = pDM->GetCellData( _T("�����ڹ�ȣ") , nIndex , sUserNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("Delivery") );

		if( sStatus.Compare( _T("�����Ϸ�") ) != 0 )
		{
			AddingLog( sRegNo , sUserNo , nIndex , -1 );
			pGrid->SelectGetNext();
			continue;
		}

		ids = pDM->GetCellData( _T("�谡����") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("Delivery") );

		// 2005.03.23 UPDATE BY PDJ
		// �����Ϸ�� ����� �Ϸ��� �ߴµ� ������ �����ϹǷ� ������ ����ڷ�� ����.
		// �����Ϸ�� ����������� �߼��ҷ��� å�� ������ �����̱� ������.
		//if( ( status.Compare(_T("��ġ�ڷ�")) !=0 ) && (sWorkingStatus.Compare( _T("���Ӻ�ġ�ڷ�") ) != 0) && (sWorkingStatus.Compare( _T("�ǽð��谡�ڷ�") ) != 0))
		if( !(sStatus.Compare(_T("�����Ϸ�"))==0 && sWorkingStatus.Compare( _T("����ڷ�") )==0)  && 
			!(sStatus.Compare(_T("�����Ϸ�"))==0 && sWorkingStatus.Compare( _T("���ӹ���ڷ�") )==0) )
		{
			AddingLog( sRegNo , sUserNo , nIndex , -2 );
			pGrid->SelectGetNext();
			continue;
		}

		// �̿��ڰ� �����Ҽ� �ִ� �������� �˻�
		ids = IsLoan(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("Delivery") );
		if( ids > 0 )
		{
			AddingLog( sRegNo , sUserNo , nIndex , ids );
			pGrid->SelectGetNext();
			continue;
		}

		// �ڷ� ���´� �ǽð����� �˾ƺ���.
		// �ڷᰡ ������ �Ǿ� �ִ��� �˾ƺ���.
		// ����� �ڷ��� ����� �Ұ����ϴ�.
		ids = IsReservation( nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("Delivery") );
		if( ids > 0 )
		{
			AddingLog( sRegNo , sUserNo , nIndex , ids );			
			pGrid->SelectGetNext();
			continue;
		}

		// ��� ó�� DB �۾�
		ids = DeliveryDBProc( nIndex , &TmpDM , sInputDate , sLoanPeriod);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("Delivery") );
		if( ids > 0 ) return ids;

		// ��� ó�� DM �۾�
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("Delivery") );

		// ��� ó�� ȭ�� �۾�
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("Delivery") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷḦ ��� �Ϸ� ó�� �Ͽ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ��� �Ϸ� ó�� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.")
							_T("\r\n���� ������ '%s'���Ͽ� ����Ͽ����ϴ�.") , nSelectCnt , m_sLogFile );
	else
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷ��� '%d'���� �ڷḦ ��� �Ϸ� ó���Ͽ����ϴ�.\r\n")
						   _T("'%d'���� �ڷ�� �ڷ���°� ��� �Ϸ� ó�� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") 
						   _T("\r\n���� ������ '%s'���Ͽ� ����Ͽ����ϴ�."), 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt , m_sLogFile );

	AfxMessageBox( sResultMsg );

	return 0;
}

/// ��� �Ϸ� DB �۾�
/// 1. å���� UPDATE
/// 2. ������� UPDATE
/// 3. �̿������� DB UPDATE
/// 4. ����/�ݳ� ���� DB UPDATE
/// 5. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CBL_BO_LOC_34::DeliveryDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sInputDate , CString sLoanPeriod )
{
	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DeliveryDBProc") );

	// 1. Column���� pTmpDM�� ����
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DeliveryDBProc") ); 

	// 2010.01.27 ADD BY PWR : TEMP�ʱ�ȭ
	pTmpDM->FreeData();

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DeliveryDBProc") ); 

	CString sLoanRecKey;
	pDM->MakeRecKey( sLoanRecKey );

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus , sPreWorkingStatus , sPublishForm , sNewWorkingStatus;
	CString sUserRecKey;
	ids = pTmpDM->GetCellData( _T("å����KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�谡����") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�������±���") , 0 , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->GetCellData( _T("�̿���KEY") , 0 , sUserRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 

	CString sTableName;
	
	CString sIDXTblName;
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	CString sTmpIDXTblName;
	//=====================================================	
	CString sProcName;
	if( sPublishForm.Compare( _T("MO") ) == 0 )
	{
		sTableName = _T("BO_BOOK_TBL");
		sNewWorkingStatus = _T("BOL215O");
		sIDXTblName = _T("IDX_BO_TBL");
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		sTmpIDXTblName = _T("IDX_TMP_BO_TBL");
		//=====================================================		
		sProcName = _T("TESTWORKSTATUS");
	}
	else
	{
		sTableName = _T("SE_BOOK_TBL");
		sNewWorkingStatus = _T("SEL315O");
		sIDXTblName = _T("IDX_SE_TBL");
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		sTmpIDXTblName = _T("IDX_TMP_SE_TBL");
		//=====================================================
		sProcName = _T("TESTSEWORKSTATUS");
	}


	/*
	// ������ ����
	// 1. å���� UPDATE
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sNewWorkingStatus );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// ���� �߰�
	CString speciesKey = pDM->GetCellData( _T("������KEY"), nDMIndex );
	CString query;
	query.Format( 
				_T("UPDATE %s SET WORKING_STATUS = %s( %s ) WHERE REC_KEY = %s;\n")
				_T("INSERT INTO %s( SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE, REC_KEY ) VALUES( %s, 'I', SYSDATE, ESL_SEQ.NEXTVAL );"),
				sTableName, sProcName, speciesKey, speciesKey, sTmpIDXTblName, speciesKey );
	if( !speciesKey.IsEmpty() )
		pDM->AddFrame( query );
	*/

	
	// 1.1. TmpDM�� ���
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
	CLocCommonAPI::GetWorkingStatusDescToCode( sNewWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("�谡����") , sNewWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�����ڷ����") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("DeliveryDBProc") ); 

	// 2. ��� ���� UPDATE
	CString sDeliveryRecKey;
	ids = pTmpDM->GetCellData( _T("�������KEY") , 0 , sDeliveryRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 

	CString sDeliveryDate;	
	CTime tsDeliveryDate = CTime::GetCurrentTime();
	sDeliveryDate.Format(
		_T("TO_DATE('%s %0d:%d:%d', 'YYYY/MM/DD HH24:MI:SS')"), 
		sInputDate, tsDeliveryDate.GetHour(), tsDeliveryDate.GetMinute(), tsDeliveryDate.GetSecond());


	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("2") );
// 	pDM->AddDBFieldData( _T("DELIVERY_DATE") , _T("STRING") , sInputDate );
	pDM->AddDBFieldData( _T("DELIVERY_DATE") , _T("NUMERIC") , sDeliveryDate );
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeUpdateFrame( _T("CO_DELIVERY_TBL") , _T("REC_KEY") , _T("NUMERIC") , sDeliveryRecKey );
	pDM->InitDBFieldData();

	// 2. 1 TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("��޻���") , _T("��޿Ϸ�"), 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�������") , sInputDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("DeliveryDBProc") ); 

	// 3. ����/�ݳ� ���� ����
	CTime tLoanDate( _ttoi(sInputDate.Mid(0,4)) , _ttoi(sInputDate.Mid(5,2)) , _ttoi(sInputDate.Mid(8,2)) , 0 , 0 , 0 );
	// ���������� �˻�
	INT nLoanPeriod = _ttoi(sLoanPeriod);
	CCheckLoanBook CheckValue( m_pParentMgr , m_pLoanManageValue );
	CheckValue.GetAddHolyDay(nLoanPeriod);

	CTimeSpan tPeriod( nLoanPeriod , 0 , 0, 0);

	tLoanDate += tPeriod;

	CString sReturnPlanDate = tLoanDate.Format( _T("%Y/%m/%d") );

	pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sLoanRecKey );
	pDM->AddDBFieldData( _T("BOOK_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->AddDBFieldData( _T("USER_KEY") , _T("NUMERIC") , sUserRecKey );
	pDM->AddDBFieldData( _T("PUBLISH_FORM_CODE") , _T("STRING") , sPublishForm );
	pDM->AddDBFieldData( _T("LOAN_TYPE_CODE") , _T("STRING") , _T("0") );
	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("0") );
	pDM->AddDBFieldData( _T("RETURN_PLAN_DATE") , _T("STRING") , sReturnPlanDate );
// 	pDM->AddDBFieldData( _T("LOAN_DATE") , _T("STRING") , sInputDate );
	pDM->AddDBFieldData( _T("LOAN_DATE") , _T("NUMERIC") , sDeliveryDate );
	// 2009.01.08 ADD BY CJY : ���ó���ÿ��� �����Ѱ��� KOLASIII�� ����
	pDM->AddDBFieldData( _T("L_DEVICE"), _T("STRING"), _T("KOLASIII"));
	// 2009.04.02 ADD BY PWR : �ݳ�ó���� �߰�
	pDM->AddDBFieldData( _T("L_WORKER"), _T("STRING"), m_pParentMgr->m_pInfo->USER_ID);

	// �Է��Ұ� �� ����.. �Ѥ�;; ��� ������ �����ؼ�..
	// (M_2004-1109_HAN)����� �ԷµǴ� ��� ������ �Է��� �־���Ѵ�.
	const INT nEtcDMColumnCnt = 15;
	CString sEtcDBFieldName[nEtcDMColumnCnt] =
	{
		_T("SHELF_LOC_CODE")		, _T("CLASS_NO_TYPE")	, _T("CLASS_NO")			, _T("MANAGE_CODE")				, _T("REG_CODE")	, 
		_T("REG_NO")				, _T("SPECIES_KEY")		, _T("USER_CLASS_CODE"), _T("USER_POSITION_CODE") , _T("MEMBER_CLASS"),
		_T("SEPARATE_SHELF_CODE")	, _T("TITLE")			, _T("AUTHOR")				, _T("PUBLISHER")				, _T("CALL_NO")
	};
	CString sEtcDMColumnName[nEtcDMColumnCnt] = 
	{
		_T("�ڷ�Ǳ���")			, _T("�з���ȣ����"), _T("�з���ȣ")	, _T("��������")	, _T("��ϱ���"),
		_T("��Ϲ�ȣ")				, _T("������KEY")	, _T("������������"), _T("����Ҽ�����"), _T("ȸ������"),
		_T("IBS_û��_��ġ��ȣ_�ڵ�"), _T("����")		, _T("������")		, _T("������")		, _T("û����ȣ")
	};
	CString sEtcValue[nEtcDMColumnCnt];
	pTmpDM->GetCellData( sEtcDMColumnName , nEtcDMColumnCnt , sEtcValue , 0);
	// 2006-04-08 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Desc�� Code�� �����Ѵ�
	// ��������
	m_pParentMgr->m_pInfo->pCodeMgr->ConvertDescToCode( _T("������������"), sEtcValue[7], sEtcValue[7]);
	// �Ҽ�����	
	m_pParentMgr->m_pInfo->pCodeMgr->ConvertDescToCode( _T("����Ҽ�����"), sEtcValue[8], sEtcValue[8]);
    // ----------------------------------------------------------------------------------------------------------
	pDM->AddDBFieldData(_T("DELAY_CNT") , _T("NUMERIC") , _T("0") )	;//�ݳ�����Ƚ��
	for( INT i = 0 ; i < nEtcDMColumnCnt ; i++ )
		pDM->AddDBFieldData(sEtcDBFieldName[i] , _T("STRING") , sEtcValue[i] )	;

	pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeInsertFrame( _T("LS_WORK_T01") );
	pDM->InitDBFieldData();

	// 3. 2 TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("����/�ݳ�����KEY") ,sLoanRecKey, 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->SetCellData( _T("�������±���") ,sPublishForm, 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("DeliveryDBProc") ); 

	// 4. �̿��� ���� ����
	// 2010.10.11 ADD BY KSJ : LAST_LOAN_DATE UPDATE
	pDM->AddDBFieldData( _T("LAST_LOAN_DATE") , _T("NUMERIC") , sDeliveryDate );
	pDM->AddDBFieldData( _T("LOAN_COUNT") , _T("NUMERIC") , _T("LOAN_COUNT+1") );
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sUserRecKey );
	pDM->InitDBFieldData();

	// 5. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("DeliveryDBProc") ); 

	ids = pDM->EndFrame();

	// 2010.10.11 ADD BY KSJ : ���յ������� ���� ó��
	CString strKLMemberYN;
	pTmpDM->GetCellData(_T("����ȸ������"), 0, strKLMemberYN);
	if(_T("Y")==m_pLoanManageValue->m_sIsUnityLoanService && _T("Y")==strKLMemberYN )
	{
		CString strFormatLoanDate, strFormatReturnPlanDate;
		strFormatLoanDate.Format(_T("%s %0d:%d:%d"), sInputDate, tsDeliveryDate.GetHour(), tsDeliveryDate.GetMinute(), tsDeliveryDate.GetSecond());
		strFormatReturnPlanDate.Format(_T("%s 23:59:59"), sReturnPlanDate);

		// ���� ������
		CFrameWriter Writer(m_pParentMgr->m_pInfo);
		Writer.setCommand(_T("IL_K10_LOC_B01_SERVICE"));
		Writer.addRecord();
		CString strGetData;
		ids = pTmpDM->GetCellData(_T("�����ڹ�ȣ����"), 0, strGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, _T("DeliveryDBProc"));
		Writer.addElement(_T("���մ����ڹ�ȣ"), strGetData);
		Writer.addElement(_T("���⵵����"), m_pParentMgr->m_pInfo->LIB_CODE);
		Writer.addElement(_T("������"), strFormatLoanDate);
		Writer.addElement(_T("�ݳ�������"), strFormatReturnPlanDate);
		Writer.addElement(_T("����"), _T("0"));
		Writer.addElement(_T("��Ϲ�ȣ"), sEtcValue[5]);
		Writer.addElement(_T("����"), sEtcValue[11]);
 	 	Writer.addElement(_T("����"), sEtcValue[12]);
 	 	Writer.addElement(_T("������"), sEtcValue[13]);
		Writer.addElement(_T("û����ȣ"), sEtcValue[14]);
		Writer.addElement(_T("����������"), strFormatLoanDate);
		ids = pTmpDM->GetCellData(_T("IBS_û��_��å��ȣ"), 0, strGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, _T("DeliveryDBProc"));
		Writer.addElement(_T("�����"), strGetData);
		Writer.addElement(_T("����KEY"), sLoanRecKey);

		// ������ �߼�
 	 	CFrameSender Sender(m_pParentMgr->m_pInfo);
 	 	Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("����ڷ����-���ó��"), __WFILE__, __LINE__));
	}

	// 2009.05.29 UPDATE BY CJY : �����ڰ��� �ּ�ó�� ����
	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// 6. ���� ���� ����/�ݳ� ���� , ������ ����� ���� �������� �˸��� 
	// (A_2004-0913_HAN...)
	// --------------------------------------------------------------------------------------
	// 6.1  ����/�ݳ� ���� ���� ���
	SetTempData( SHARE_CO_LOAN_TBL_INSERT_REC_KEY , CComVariant(sLoanRecKey) );

	// 6.2  ������ ���� ���� ���
	SetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY , CComVariant(sUserRecKey) );
	SetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS , CComVariant(_T("LOAN_COUNT")) );

	return 0;
}



INT CBL_BO_LOC_34::GetDeliveryInputDate( CString &sInputDate , CString &sLoanPeriod)
{
	CString sCMAlias = _T("CM_BO_LOC_3440");

	INT ids;

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("��޿Ϸ�����") , sInputDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetReceiptInputDate") );
	sInputDate.TrimRight(); sInputDate.TrimLeft();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("�������") , sLoanPeriod );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetReceiptInputDate") );
	sLoanPeriod.TrimRight(); sLoanPeriod.TrimLeft();

	return 0;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-08 ~ 2005-12-08
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   �Լ�����   ] 
      ����ڷ���� ���� ����
   [  PARAMETERS  ]
      NULL 

   [ RETURN VALUE ]
      1    : ���
      0    : ���� 
      ���� : ����

   [   �̴Ͻ���   ]
      1. �׸��忡 ���õ� �ڷᰡ �ִ��� Ȯ��
	  2. ���� ���� ����
	  3. ������ �۾�
--------------------------------------------------------------------------------------------*/
INT CBL_BO_LOC_34::DeleteDelivery()
{
	BEGIN_SBL();

	INT ids;
	
	// ----------------------------------------------------------------------------
	// 1. �׸��忡 ���õ� �ڷᰡ �ִ��� Ȯ��     
	// ----------------------------------------------------------------------------
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BO_LOC_34::DeleteDelivery()") );

	pGrid->SelectMakeList();
	pGrid->SelectGetTailPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return 1;
	}

	// 2005-12-08 ADD BY PDW(Jr) ==================================================
	if ( AfxMessageBox( _T("������ �ڷḦ ���� �Ͻðڽ��ϱ� ?"), MB_ICONQUESTION | MB_YESNO ) == IDNO )
		return 1;
	// ============================================================================

	// 2005-12-08 REM BY PDW(Jr) ==================================================
	// ������� �ʴ´� 
	// DM ���� ����
	/*
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BO_LOC_34::DeleteDelivery()") );
	*/
	// ============================================================================

	// ----------------------------------------------------------------------------
	// 2. ���� ���� ����   
	// ----------------------------------------------------------------------------
	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);
	if ( pProgressBar == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BO_LOC_34::DeleteDelivery()") );

	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus; // �ڷ����
	CString sStatus ;       // ��� ����
	INT nIndex;

	// DM ���� ����
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	for( INT i = 0;i < nSelectCnt;i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("��޻���") , nIndex , sStatus );
		if ( ids ) return ids;

		if ( sStatus == _T("��޿Ϸ�") ) 
		{
			pGrid->SelectGetPrev();
			continue;
		}

		//  DB�۾�
		ids = DeleteDBProc( nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_BO_LOC_34::DeleteDelivery()") );
		if( ids > 0 ) return ids;

		// 2010.02.18 ADD BY PWR : �����ڷ� �������� ����
		pGrid->SelectGetPrev();
		nSuccessCnt++;
	}
	
	// ----------------------------------------------------------------------------
	// 3. ������ �۾�
	// ----------------------------------------------------------------------------
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar( i );
	DeleteProgressBarDlg( pProgressBar );

	m_pParentMgr->AllControlDisplay( _T("CM_BO_LOC_3400") );

	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);

	// ��� �޽���
	CString sResultMsg;	
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷḦ ���� ó�� �Ͽ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ���� ó�� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷ��� '%d'���� �ڷḦ ���� ó���Ͽ����ϴ�.\r\n")
						   _T("'%d'���� �ڷ�� �ڷ���°� ���� ó�� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;
}

INT CBL_BO_LOC_34::DeleteDBProc( INT nDMIndex )
{
	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDBProc") );

	pDM->StartFrame();	

	// ������ ����
	pDM->InitDBFieldData();
	// 2. ��� ���� UPDATE
	CString sDeliveryRecKey;
	CString sBookKey;
	CString sPublishFormCode;
	ids = pDM->GetCellData( _T("�������KEY") , nDMIndex , sDeliveryRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepairDBProc") ); 

	ids = pDM->GetCellData( _T("å����KEY") , nDMIndex , sBookKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepairDBProc") ); 

	ids = pDM->GetCellData( _T("�������±���") , nDMIndex , sPublishFormCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepairDBProc") ); 	

	pDM->MakeDeleteFrame( _T("CO_DELIVERY_TBL") , _T("REC_KEY") , _T("NUMERIC") , sDeliveryRecKey );
	
	// 2009.03.18 UPDATE BY PWR : WORKING_STATUS�� ����ڷ�(BOL215O)�϶��� ��ġ�� ����
	// DM���� �����ü����ִµ� ������ ��� �׽����� ���°��� ��� ������ ���� ��񿡼� �ٷΰ����´�.
	CString sWorkingStatus;
	CString sQuery;
	if ( sPublishFormCode == _T("MO") )
	{
		sQuery.Format( _T("SELECT WORKING_STATUS FROM BO_BOOK_TBL WHERE REC_KEY=%s"), sBookKey );
 		ids = pDM->GetOneValueQuery( sQuery , sWorkingStatus );
		sWorkingStatus.TrimLeft(); sWorkingStatus.TrimRight();
		if(_T("BOL215O") == sWorkingStatus)
		{
			pDM->InitDBFieldData();
			pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("BOL112N") );		
			pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );
			pDM->MakeUpdateFrame( _T("BO_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookKey );
		}
	}
	else
	{
		sQuery.Format( _T("SELECT WORKING_STATUS FROM SE_BOOK_TBL WHERE REC_KEY=%s"), sBookKey );
 		ids = pDM->GetOneValueQuery( sQuery , sWorkingStatus );
		sWorkingStatus.TrimLeft(); sWorkingStatus.TrimRight();
		if(_T("SEL315O") == sWorkingStatus)
		{
			pDM->InitDBFieldData();
			pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL212N") );		
			pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );
			pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookKey );
		}
	}
// 	if ( sPublishFormCode == _T("MO") )
// 	{
// 		pDM->InitDBFieldData();
// 		pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("BOL112N") );		
// 		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );
// 		pDM->MakeUpdateFrame( _T("BO_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookKey );
// 	}
// 	else
// 	{
// 		pDM->InitDBFieldData();
// 		pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL212N") );		
// 		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ), TRUE );
// 		pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookKey );
// 	}

	// 3. ������ DB�� Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RegRepairDBProc") ); 

	pDM->DeleteRow( nDMIndex );

	return 0;
}

/// ������ ������ �������� �˾ƺ���.
// 1. ������������ �������� �˻�
// 2. �Ѵ���å���� �ʰ��ߴ��� �˻�
// 3. ȸ������ �˻� (����, ȸ�����н�ȸ�� ��������)		// 2008.03.26 ADD BY PWR
// 4. ����� ��� ���� �˻�								// 2008.06.17 ADD BY PWR
INT CBL_BO_LOC_34::IsLoan( INT nIndex )
{
	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsLoan") );

	// �̿���KEY�� ���´�.
	CString sUserKey;
	ids = pDM->GetCellData( _T("�̿���KEY") , nIndex , sUserKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsLoan") );

	// �̿��� ���� , ���� ���� å�� , ���� �������� ���´�.
	CString sWhere;
	sWhere.Format( _T("REC_KEY = %s") , sUserKey );

	CESL_DataMgr *pUserDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400_USER_INFO") );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("IsLoan") );
	
	ids = pUserDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsLoan") ); 


	CString sLoanClassCode , sLoanBookCnt , sLoanStopDate;
	ids = pUserDM->GetCellData( _T("������������") , 0 , sLoanClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("IsLoan") ); 
	ids = pUserDM->GetCellData( _T("�Ѵ���å��") , 0 , sLoanBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsLoan") ); 
	ids = pUserDM->GetCellData( _T("����������") , 0 , sLoanStopDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("IsLoan") ); 
	
	// 1. ������������ �������� �˻�
	if( !sLoanStopDate.IsEmpty() )
	{
		//++2008.05.13 UPDATE BY PWR {{++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// CTime -> COleDateTime ���� ���� ( ������������ Ŀ�� �����߻� )
		COleDateTime tTmpCurrent = COleDateTime::GetCurrentTime();
		//CTime tTmpCurrent = CLocCommonAPI::GetDBTime(m_pParentMgr);
		COleDateTime tCurrent = COleDateTime( tTmpCurrent.GetYear() , tTmpCurrent.GetMonth() , tTmpCurrent.GetDay() , 0 , 0 , 0 );
		COleDateTime tLoanStopDate = COleDateTime( _ttoi(sLoanStopDate.Mid(0,4)) , _ttoi(sLoanStopDate.Mid(5,2)) , _ttoi(sLoanStopDate.Mid(8,2)) , 0 , 0 , 0 );
		//--2008.05.13 UPDATE BY PWR ------------------------------------------------------------}}

		if( tCurrent <= tLoanStopDate )
			return 1;
	}

	// 2. �Ѵ���å���� �ʰ��ߴ��� �˻�
	// 17/08/07 ������ : ����ڷ� ����Ǽ��� �η��ڷᰡ ���Ե��� �ʵ��� ����
//*/ ADD ---------------------------------------------------------------------------
	CString sExAppedixLoanCnt = _T("");
	// �η��ڷ� ����å�� ���Կ���Ȯ��
	if( m_pLoanManageValue->m_sIsLoanAppendix.Compare( _T("Y") ) == 0  && m_pLoanManageValue->m_sIsIncludeLoanCnt.Compare( _T("N") ) == 0 )
	{ // �η��ڷ� ���Ծ��Ҷ�
		CString sBookKey  = _T("");
		ids = pDM->GetCellData( _T("å����KEY") , nIndex , sBookKey );
		if(!sBookKey.IsEmpty())
		{
			CString sSql, sValue;				
			sSql.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, BO_BOOK_TBL B WHERE CL.STATUS IN ('0','2') AND CL.USER_KEY=%s AND CL.BOOK_KEY = B.REC_KEY AND B.BOOK_APPENDIX_FLAG <> 'A' AND CL.MANAGE_CODE=UDF_MANAGE_CODE"), sUserKey);							
			ids = pUserDM->GetOneValueQuery( sSql , sValue );
			sExAppedixLoanCnt = sValue;

			sSql.Format( _T("SELECT BOOK_APPENDIX_FLAG FROM BO_BOOK_TBL WHERE REC_KEY = %s "), sBookKey);							
			ids = pUserDM->GetOneValueQuery( sSql , sValue );
			INT nExAppedixLoanCnt = _ttoi(sExAppedixLoanCnt);
			if(sValue == "A" &&  nExAppedixLoanCnt > 0) //�η��ڷ��̸�
			{
				sExAppedixLoanCnt.Format(_T("%d"),nExAppedixLoanCnt-1);
			}
		}
	}
//*/ END ---------------------------------------------------------------------------
	CString sResult;
	// 2009.03.24 UPDATE BY PWR : ���մ��Ⱑ�ɱǼ� ��������� ���� ó��
	if(m_pLoanManageValue->m_sUnityLoanCntYN == _T("Y"))
	{ // �����
		// �������� �ڰ����Ⱑ�ɱǼ�
		ids = m_pLoanManageValue->GetIndividualUserPosition( sLoanClassCode, _T("�Ѵ��Ⱑ�ɼ�"), sResult );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-8 , _T("IsLoan()") ); 
		}
		if( sResult.IsEmpty() )
		{
			sResult = _T("0");
		}
		INT nMaxLoanBookCnt = _ttoi( sResult );
		
		if( nMaxLoanBookCnt < (_ttoi(sLoanBookCnt)+1) ) 
		{
			// 17/08/07 ������ : ����ڷ� ����Ǽ��� �η��ڷᰡ ���Ե��� �ʵ��� ����
//*/ ADD ---------------------------------------------------------------------------
			if( sExAppedixLoanCnt.IsEmpty() || nMaxLoanBookCnt < (_ttoi(sExAppedixLoanCnt)+1) )
//*/ END ---------------------------------------------------------------------------	
				return 2;
		}
	}
	else
	{ // ������
		// �ڰ����Ⱑ�ɱǼ�
		ids = m_pLoanManageValue->GetUserPosition( sLoanClassCode, _T("�Ѵ��Ⱑ�ɼ�"), sResult );
 		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-8 , _T("IsLoan()") ); 
		}
		if( sResult.IsEmpty() )
		{
			sResult = _T("0");
		}
		
		INT nMaxLoanBookCnt = _ttoi( sResult );

		if( nMaxLoanBookCnt < (_ttoi(sLoanBookCnt)+1) ) 
		{
			// 17/08/07 ������ : ����ڷ� ����Ǽ��� �η��ڷᰡ ���Ե��� �ʵ��� ����
//*/ ADD ---------------------------------------------------------------------------
			if( sExAppedixLoanCnt.IsEmpty() || nMaxLoanBookCnt < (_ttoi(sExAppedixLoanCnt)+1) )
//*/ END ---------------------------------------------------------------------------	
				return 2;
		}
	}
//  	ids = m_pLoanManageValue->GetUserPosition( sLoanClassCode , _T("�Ѵ��Ⱑ�ɼ�") , sResult );
//  	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-8 , _T("IsLoan()") ); 
// 	
// 	if( sResult.IsEmpty() ) sResult = _T("0");
// 	
// 	INT nMaxLoanBookCnt = _ttoi( sResult );
// 
// 	if( nMaxLoanBookCnt < (_ttoi(sLoanBookCnt)+1) ) 
// 		return 2;
	
	//++2009.01.09 ADD BY CJY {{++
	// ��ȸ���� ���� ���ó������
	// ����ȸ���� �ƴϸ�
	CString strUserClass;
	ids = pDM->GetCellData( _T("�ҷ�ȸ������") , nIndex, strUserClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("IsLoan") ); 
 	if(_T("1") == strUserClass) 
	{ 
		// ��������ȸ���̶� ������������ �����Ϻ��� �����̸� ��������ȸ������ ���� �ʴ´�.
		CString strLoanStopDate;
		ids = pDM->GetCellData(_T("����������"), nIndex, strLoanStopDate);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("SettingDelayDay") );
		
		COleDateTime oleLoanStopDate;
		int nYear = 1, nMonth = 1, nDay = 1;
		_stscanf(strLoanStopDate.GetBuffer(0), _T("%d/%d/%d"), &nYear, &nMonth, &nDay);
		
		oleLoanStopDate.SetDate(nYear, nMonth, nDay);
		
		COleDateTime oleToday = COleDateTime::GetCurrentTime();
		oleToday.SetDate(oleToday.GetYear(), oleToday.GetMonth(), oleToday.GetDay());
		
		if(oleToday > oleLoanStopDate)
		{
		}
		else return 15;
	}
	if(_T("2") == strUserClass) { return 10; }
	if(_T("3") == strUserClass) { return 11; }

	// ȸ������
	CString strMemberClass;
	ids = pDM->GetCellData( _T("ȸ������") , nIndex, strMemberClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("IsLoan") ); 
	if(_T("1") == strMemberClass) 
	{ // ��ȸ��
		return 16; 
	}
	// 2008.05.28 ADD BY PWR : ��ȸ���� ȸ�������Ŀ� ������ �����ϹǷ� ����
	else if(_T("2") == strMemberClass)
	{ // ��ȸ��
		return 17; 
	}

	// ȸ�����н�ȸ���̸�
	CString strLostUserCard;
	ids = pDM->GetCellData( _T("ȸ�����нǿ���") , nIndex, strLostUserCard);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("IsLoan") ); 
	if(_T("Y") == strLostUserCard) { return 14; }
	
	
//DEL	�����ҽ�
//DEL 	//++2008.04.15 ADD BY PWR {{++
//DEL 	// 3. ȸ������ �˻� (����, Ż��ȸ�� ��������)
//DEL 	CString sSql;
//DEL 	sSql.Format( _T("SELECT USER_CLASS FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), sUserKey );
//DEL 	ids = pDM->GetOneValueQuery( sSql , sResult );
//DEL 	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("IsLoan()") );
//DEL 	// ����ȸ��
//DEL 	if( _T("2") == sResult )
//DEL 	{
//DEL 		return 10;
//DEL 	}
//DEL 	// Ż��ȸ��
//DEL 	else if( _T("3") == sResult )
//DEL 	{
//DEL 		return 11;
//DEL 	}
//DEL 	//--2008.04.15 ADD BY PWR --}}
//DEL 	//++2008.07.15 ADD BY PWR {{++
//DEL 	// �������н�ȸ�� ��������
//DEL 	sResult = m_pLoanManageValue->m_sLoanIsLostUserCard;
//DEL 	if(sResult == _T("Y") )
//DEL 	{
//DEL 		sSql.Format( _T("SELECT LOST_USER_CARD FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), sUserKey );
//DEL 		ids = pDM->GetOneValueQuery( sSql , sResult );
//DEL 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("IsLoan()") );
//DEL 		if( _T("Y") == sResult ) return 14;
//DEL 	}
//DEL 	//--2008.07.15 ADD BY PWR --}}
	//--2009.01.09 ADD BY CJY --}}


	//++2008.06.17 ADD BY PWR {{++
	// 4. ����� ��� ���� �˻�
	ids = IsReLoanBook( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -12, _T("IsLoan()") );
	if( ids > 0 )
	{
		// ������ߴ� �ڷ��ε� �����Ѵٸ� ���Ⱑ�ɻ��°� �ƴϴ�.
		if( ids == 12 || ids == 13 )
			return ids;
	}
	//--2008.06.17 ADD BY PWR --}}
	
	return 0;
}



/// �ڷ� ���¸� Ȯ���Ѵ�.
INT CBL_BO_LOC_34::IsReservation( INT nIndex )
{
	INT ids;
	
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsReservation") );

	CString sBookKey , sPublishForm;
	ids = pDM->GetCellData( _T("å����KEY") , nIndex , sBookKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsReservation") );

	ids = pDM->GetCellData( _T("�������±���") , nIndex , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsReservation") );
	
	CESL_DataMgr *pBookDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400_BOOK_INFO") );
	if( pBookDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("IsReservation") );

	CString sWhere;
	sWhere.Format( _T("B.REC_KEY = %s") , sBookKey );

	CString sTableName;
	CString sUseObjCodeFieldName;
	if( sPublishForm.Compare(_T("MO")) == 0 )
	{
		sTableName = _T("BO_BOOK_TBL B");
		sUseObjCodeFieldName = _T("B.USE_OBJECT_CODE");
	}
	else if( sPublishForm.Compare(_T("SE")) == 0 )
	{
		sTableName = _T("SE_BOOK_TBL B , IDX_SE_TBL I");
		sUseObjCodeFieldName = _T("I.USE_OBJ_CODE");
		sWhere += _T(" AND B.SPECIES_KEY = I.REC_KEY ");
	}

	pBookDM->TBL_NAME = sTableName;

	CString sColumnAlias , sFieldName , sFieldType , sInitValue , sDataType;
	for( INT k = 0 ; k < pBookDM->RefList.GetCount() ; k++ )
	{
		sColumnAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = pBookDM->GetColumnRefInfo( k , sColumnAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsReservation") );
		
		if( sColumnAlias.Compare(_T("�̿��󱸺�")) == 0 )
			sFieldName = sUseObjCodeFieldName;

		ids = pBookDM->SetDataMgrRef( k , sColumnAlias , sFieldName , sFieldType  , sInitValue  , sDataType );			
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsReservation") );
	}		
	
	ids = pBookDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsReservation") );

	// 1. �ڷ� ���� Ȯ��
	CString sWorkingStatus;
	ids = pBookDM->GetCellData( _T("å����") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("IsReservation") );

	// 2005.03.23 UPDATE BY PDJ
	//if( sWorkingStatus.Compare(_T("BOL112N")) !=0 && (sWorkingStatus.Compare(_T("SEL112N")) !=0) && (sWorkingStatus.Compare(_T("SEL212N")) !=0) )
	if( sWorkingStatus.Compare(_T("BOL215O")) !=0 && (sWorkingStatus.Compare(_T("SEL315O")) !=0) && (sWorkingStatus.Compare(_T("SEL212N")) !=0) )
		return 3;
	
	//++2008.06.08 DEL BY PWR {{++
	// ����ó���� ���ຸ�� ���� �̷�������Ƿ� ����ó���� �����ؾ��Ѵ� 
	// ����ó������ �̹� �����ڷ� Ȯ�������Ƿ� ����ó������ Ȯ�ξ��ϵ��� �Ѵ�.
	/*
	// 2. ����� �ڷᰡ �����ϴ��� Ȯ��
	CString sSql , sResult ;
	sSql.Format( _T("SELECT COUNT(*) FROM LH_STORAGE_V01 WHERE BOOK_KEY=%s AND STATUS = '3'") , sBookKey );
	ids = pBookDM->GetOneValueQuery( sSql , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );

	if( _ttoi(sResult) > 0 )
		return 4;
	*/
	//--2008.06.08 DEL BY PWR --}}

	// 1. ���� ���α׷��� ����ϰ� �ִ� �������� �������а� å�� �������а��� �ٸ� ��� 
	// ������ ���� ������ �������� �о�� �� ����
	if( m_pLoanManageValue->m_sIsLoanManageCode.Compare(_T("N"))==0 ) 
	{
		CString sExpManageCode;		
		ids = pBookDM->GetCellData( _T("��������") , 0 , sExpManageCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
		
		CString sManageCode;
		ids = m_pParentMgr->m_pInfo->pCodeMgr->GetDefaultCode( _T("��������") , sManageCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
		
		if( sExpManageCode.Compare(sManageCode) != 0 )
			return 5;
	}
	
	
	// 2. �����ڿ� ���þ��� å������ ���� ������ �������� �˻�
	//    - ��ϱ��� , ��ġ��ȣ , �̿����ѱ���
	const INT nCnt = 3;
	TCHAR *sFieldAlias[nCnt] = {
		_T("��ϱ���") , _T("��ġ��ȣ") , _T("�̿����ѱ���")
	};
	CString sCode[nCnt];
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = pBookDM->GetCellData( sFieldAlias[i] , 0 , sCode[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
	}
	for( i = 0 ; i < nCnt ; i++ )
	{
		CString sValue;
		ids = m_pLoanManageValue->IsLoanCodeYN( sFieldAlias[i], sCode[i] , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
		
		if( sValue.Compare(_T("N")) == 0 )
			return 6+i;
	}

	// 3. ������ ���޺� ���������� ������ �������� �Ұ������� �˻�
	//    - �̿����ڷᱸ��
	CString sMemberDMAlias = _T("DM_BO_LOC_3400_USER_INFO");
	CString sUserClassCode;
	ids = m_pParentMgr->GetDataMgrData( sMemberDMAlias , _T("������������") , sUserClassCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );

	// �̿� ��� �ڷᱸ��
	CString sExpUseObjCode;
	ids = pBookDM->GetCellData( _T("�̿��󱸺�") , 0 , sExpUseObjCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
	
	if( sExpUseObjCode.Compare(_T("AD"))==0 )
	{
		CString sUseObjCode;	
		ids = m_pLoanManageValue->GetUserPosition( sUserClassCode , _T("�̿��󱸺�") , sUseObjCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );

		if( sUseObjCode.Compare(_T("N"))==0 ) 
			return 9;
		
	}
	
	
	return 0;
}

// �α� ���Ͽ� ���
INT CBL_BO_LOC_34::AddingLog( CString sRegNo , CString sUserNo , INT nGridIndex , INT nErrorCode )
{
	_tmkdir(DELIVERY_LOG_DIR_PATH);
	
	CStdioFile fLogFile;
	INT ids = fLogFile.Open( m_sLogFile , CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite | CFile::typeBinary);
	if( ids == 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AddingLog") );

	if( 0 == fLogFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fLogFile.Write( &cUni, sizeof(TCHAR));
	}
		
	CString sEMsg;
	switch(nErrorCode)
	{
		case -1 :
			sEMsg.Format( _T("���� �Ϸ� ���°� �ƴϹǷ� ��� ó���� �����߽��ϴ�.") );
			break;
		case -2 :
			sEMsg.Format( _T("��ġ ���°� �ƴϹǷ� ��� ó���� �����߽��ϴ�.") );
			break;
		case 1 :
			sEMsg.Format( _T("������������ ������ ���� �̿����̱� ������ ��� ó���� �����߽��ϴ�.") );
			break;
		case 2 :
			sEMsg.Format( _T("�̿밡���� �Ѵ���å���� �ʰ��߱� ������ ��� ó���� �����߽��ϴ�.") );
			break;
		case 3 :
			sEMsg.Format( _T("��ġ ���°� �ƴϹǷ� ��� ó���� �����߽��ϴ�.") );
			break;
		case 4 :
			sEMsg.Format( _T("����� �ڷ��̱� ������ ��� ó���� �����߽��ϴ�.") );
			break;
		case 5 :
			sEMsg.Format( _T("���������� �ٸ� �ڷ��̱� ������ ��� ó���� �����߽��ϴ�.") );
			break;
		case 6 :
			sEMsg.Format( _T("��ϱ����� �����Ҽ� ���� �ڷ��Դϴ�.") );
			break;
		case 7 :
			sEMsg.Format( _T("��ġ��ȣ�� �����Ҽ� ���� �ڷ��Դϴ�.") );
			break;
		case 8 :
			sEMsg.Format( _T("�̿����ѱ����� �����Ҽ� ���� �ڷ��Դϴ�.") );
			break;
		case 9 :
			sEMsg.Format( _T("���� �ڷḦ ������ �� ���� �̿����Դϴ�.") );
			break;
		// 2008.04.15 ADD BY PWR
		case 10 :
			sEMsg.Format( _T("ȸ�����°� ����ȸ���̱� ������ ��� ó���� �����߽��ϴ�.") );
			break;
		case 11 :
			sEMsg.Format( _T("ȸ�����°� Ż��ȸ���̱� ������ ��� ó���� �����߽��ϴ�.") );
			break;
		// 2008.06.17 ADD BY PWR
		case 12 :
			sEMsg.Format( _T("������ ���� �Ǿ��� å�̱� ������ ��� ó���� �����߽��ϴ�.") );
			break;
		case 13 :
			sEMsg.Format( _T("������ �Ѹ��� ������ �����߾��� å�̱� ������ ��� ó���� �����߽��ϴ�.") );
			break;
		// 2008.07.15 ADD BY PWR
		case 14 :
			sEMsg.Format( _T("������ �н�ȸ���̱� ������ ��� ó���� �����߽��ϴ�.") );
			break;
		case 15 :
			sEMsg.Format( _T("��������ȸ���̱� ������ ��� ó���� �����߽��ϴ�.") );
			break;
		case 16 :
			sEMsg.Format( _T("��ȸ���̱� ������ ��� ó���� �����߽��ϴ�.") );
			break;
		// 2008.05.28 ADD BY PWR : ��ȸ���� ȸ�������Ŀ� ������ �����ϹǷ� ����
		case 17 :
			sEMsg.Format( _T("��ȸ���̱� ������ ��� ó���� �����߽��ϴ�.") );
			break;
		default:
			sEMsg.Format( _T("�˼� ���� ����.") );
			break;
	}
	
	CString sMsg;
	sMsg.Format(_T("�׸��� ��ȣ [%d] , ��Ϲ�ȣ[%s] , �̿��ڹ�ȣ[%s] : %s \r\n") , nGridIndex+1 , sRegNo , sUserNo , sEMsg);
	
	fLogFile.SeekToEnd();
	fLogFile.WriteString( sMsg );
	
	fLogFile.Close();
	
	return 0;
}

// �α� ���� �ʱ�ȭ
INT CBL_BO_LOC_34::InitLog( CString sTitle )
{
	_tmkdir(DELIVERY_LOG_DIR_PATH);

	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	m_sLogFile.Format( _T("%s[%s].txt") , DELIVERY_LOG_FILE_PATH , (CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData)).Format(_T("%Y%m%d%H%M%S")) );
// 	m_sLogFile.Format( _T("%s[%s].txt") , DELIVERY_LOG_FILE_PATH , (CLocCommonAPI::GetDBTime(m_pParentMgr)).Format(_T("%Y%m%d%H%M%S")) );

	CStdioFile fLogFile;
	INT ids = fLogFile.Open( m_sLogFile , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary);
	if( ids == 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitLog") );
	
	if( 0 == fLogFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fLogFile.Write( &cUni, sizeof(TCHAR));
	}
	
	fLogFile.WriteString( sTitle );

	fLogFile.Close();

	return 0;
}

// 2008.06.17 ADD BY PWR
// ** ����� ���� ����
INT CBL_BO_LOC_34::IsReLoanBook( INT nIndex )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1, _T("IsReLoanBook()") );

	// ���������
	CString sValue;
	sValue = m_pLoanManageValue->m_sIsOneManReLoan;

	// �����������
	CString sReLoanLimitDay;
	sReLoanLimitDay = m_pLoanManageValue->m_sReLoanLimitDay;

	// ���� �����
	// ���� ����� N + ���� ����� Y -> ���� ����⸸ �˻�
	// ���� ����� N + ���� ����� N -> ����, ����
	INT nReLoanMode		= 0;
	INT nReLoanYN		= 0;
	INT nFamilyReLoanYN = 0;

	if( sValue.Compare(_T("Y")) != 0 )
	{// ���� ����� ������
		nReLoanYN = 1;
		sValue = m_pLoanManageValue->m_sIsFamilyReLoan;
		if( sValue.Compare(_T("Y")) != 0 )
		{//���� ����� ������
			nFamilyReLoanYN = 1;
		}
	}
	else
	{// ���� ����� ���
		nReLoanYN = 0;	
	}

	CString sSql;
	CString sBookRecKey;
	CString sUserRecKey;
	CString sFamilyRecKey;

	ids = pDM->GetCellData( _T("å����KEY"), nIndex, sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -3, _T("IsReLoanBook()") );
	ids = pDM->GetCellData( _T("�̿���KEY"), nIndex, sUserRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("IsReLoanBook()") );
	ids = pDM->GetCellData( _T("����KEY"), nIndex, sFamilyRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -5, _T("IsReLoanBook()") );

	INT nCount=0;
	INT nOneManCnt=0;
	INT nFamilyCnt=0;
	
	CString strLimitWhere;
	if( _ttoi(sReLoanLimitDay) != 0 )
	{
		//++2008.09.02 DEL BY LKS {{++
//REMOVE�� 		if( m_pParentMgr->m_pInfo->MODE == 10000 || m_pParentMgr->m_pInfo->MODE == 30000 )
//REMOVE�� 			strLimitWhere = _T("AND to_date(now(),'YYYY/MM/DD') - to_date(CL.RETURN_DATE,'YYYY/MM/DD') < ") + sReLoanLimitDay ;
//REMOVE�� 		else
		//--2008.09.02 DEL BY LKS --}}
			strLimitWhere = _T("AND TO_DATE(SYSDATE,'YYYY/MM/DD') - TO_DATE(CL.RETURN_DATE,'YYYY/MM/DD') < ") + sReLoanLimitDay ;
	}

	sSql.Format( _T(" SELECT SUM(CNT) FROM ( ")
		         _T(" SELECT COUNT(1) AS CNT FROM CO_LOAN_USER_TBL CLU, LS_WORK_T01 CL ")
				 _T(" WHERE CL.STATUS IN ('0','2') ")
				 _T(" AND CL.USER_KEY = CLU.REC_KEY ")
				 _T(" AND CL.BOOK_KEY = %s ")
				 _T(" AND CL.USER_KEY = %s ")
				 _T(" UNION ")
				 _T(" SELECT COUNT(1) AS CNT FROM CO_LOAN_USER_TBL CLU, LH_STORAGE_V01 CL ")
				 _T(" WHERE CL.STATUS IN ('1','5','6') ")
				 _T(" AND CL.USER_KEY = CLU.REC_KEY ")
				 _T(" AND CL.BOOK_KEY = %s ")
				 _T(" AND CL.USER_KEY = %s ")
				 _T(" %s )"), sBookRecKey , sUserRecKey, sBookRecKey , sUserRecKey , strLimitWhere );

	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr , pDM , sSql , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -6, _T("IsReLoanBook()") );

	nOneManCnt = _ttoi(sValue);

	if( !sFamilyRecKey.IsEmpty() )
	{
		sSql.Format( _T(" SELECT SUM(CNT) FROM ( ")
					 _T(" SELECT COUNT(1) AS CNT FROM CO_LOAN_USER_TBL CLU, LS_WORK_T01 CL ")
					 _T(" WHERE CL.STATUS IN ('0','2') ")
					 _T(" AND CL.USER_KEY = CLU.REC_KEY ")
					 _T(" AND CL.BOOK_KEY = %s ")
					 _T(" AND ( CLU.FID_KEY = %s ) ")
				     _T(" UNION ")
				     _T(" SELECT COUNT(1) AS CNT FROM CO_LOAN_USER_TBL CLU, LH_STORAGE_V01 CL ")
					 _T(" WHERE CL.STATUS IN ('1','5','6') ")
					 _T(" AND CL.USER_KEY = CLU.REC_KEY ")
					 _T(" AND CL.BOOK_KEY = %s ")
					 _T(" AND ( CLU.FID_KEY = %s ) ")
					 _T(" %s )"), sBookRecKey , sFamilyRecKey, sBookRecKey , sFamilyRecKey , strLimitWhere );

		ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr , pDM , sSql , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -7, _T("IsReLoanBook()") );

		nFamilyCnt = _ttoi(sValue);
	}

	if( nOneManCnt > 0 )
	{
		if( nReLoanYN == 1 ) 
		{// ��������� ������
			return 12;
		}
	}
	else if( nFamilyCnt > 0 )
	{
		if( nFamilyReLoanYN == 1 )
		{// ��������� ������
			return 13;
		}
	}

	return 0;

EFS_END
return -1;
}
