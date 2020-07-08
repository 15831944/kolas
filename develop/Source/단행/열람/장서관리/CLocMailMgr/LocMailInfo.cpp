// LocMailInfo.cpp : implementation file
//

#include "stdafx.h"
#include "LocMailInfo.h"
#include "MailAccountMgr.h"
// 2011.09.07 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocMailInfo dialog


CLocMailInfo::CLocMailInfo(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	// KOL.UDF.022
	m_sSMAlias = _T("");
	m_sLibName = _T("");
	m_sEcoMailUseYN = _T("");
	m_strTO_EMP_CODE = _T("");
	m_sTitle = _T("");
	m_sBody = _T("");
	m_sEMail = _T("");
	m_sRecipientName = _T("");
	m_sCCName = _T("");
	m_sCCAddress = _T("");
	m_sLoanDMAlias = _T("");
	m_sUserDMAlias = _T("");
	m_sMailModeDMAlias = _T("");

	m_pLoanMailDM = NULL;
	m_pTmpLoanMailDM = NULL;
	m_pUserMailDM = NULL;
	m_pMailModeInfoDM = NULL;
	m_pLoanKeyDM = NULL;
	m_pMailAliasDM = NULL;	
	m_pSendMailMgr = NULL;	
	m_pMailRecord = NULL;

}

CLocMailInfo::~CLocMailInfo()
{
	if( m_pMailRecord != NULL ) delete m_pMailRecord;
	m_pMailRecord = NULL;
	
	if( m_pSendMailMgr != NULL ) delete m_pSendMailMgr;
	m_pSendMailMgr = NULL;
}


VOID CLocMailInfo::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocMailInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CLocMailInfo, CDialog)
	//{{AFX_MSG_MAP(CLocMailInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocMailInfo message handlers


// ���� ���� ���� ����
// 1. ���� ������ ���� ���� ����
INT CLocMailInfo::MakeInitInfo()
{
	EFS_BEGIN

	INT ids;

	ids = m_pMailModeInfoDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeInitInfo") );

	INT itest = m_pMailModeInfoDM->GetRowCount();
	m_pMailModeInfoDM->SORT( _T("��������") );

	/// ������ �̸� ��������
	CString sLibInfo[1] =
	{
		_T("LIB_NAME")
	};
	CLocCommonAPI::GetLibInfo( sLibInfo , sLibInfo , 1 , this);
	m_sLibName = sLibInfo[0];
	
	/// ���ϸ� �ý��� ��� ���������� �˾ƺ���.
	ids = GetManageValue( _T("��Ÿ")  , _T("����") , _T("���ϸ��ý���") , _T("��뿩��") , m_sEcoMailUseYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeInitInfo") );

	if( m_sEcoMailUseYN.IsEmpty() ) m_sEcoMailUseYN = _T("N");

	return 0;

	EFS_END
	return -1;

}

// SQL WHERE���� �̿��� ���� ���ڵ� ����
// DM�� DB���� �����Ͽ� ������ ������ �յ��� ������ �����Ѵ�.
INT CLocMailInfo::MakeMailInfo( INT nMailClass , CString sWhere )
{
	EFS_BEGIN

	INT ids;

	// ���� ���� DM ����
	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

	ids = MakeSql( nMailClass , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailInfo") );
	if( ids > 0 ) return ids;

	ids = pLocDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeMailInfo") );

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pLocDM->GetRowCount() && _T("DM_LOC_MAIL_INFO") == pLocDM->GETDMAlias())
	{
		CString strEnc;
		CStringArray asDes1;
		for(INT i = 0; i < pLocDM->GetRowCount(); i++)
		{
			pLocDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes1.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pLocDM->GetRowCount(); i++)
				{
					pLocDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	// û����ȣ ����
	// 2007.04.04 update by pdw {{+++++++++++++++++++++++++++++++++++++++
	// Ż��ȸ���߰�
	if( ( nMailClass != 8 ) && ( nMailClass != 20 ) )
		CLocCommonAPI::MakeShelfCode( this , pLocDM->DB_MGR_ALIAS );
	// ----------------------------------------------------------------}}

	// nMailClass�� 0���� 7�� ��� ��� ���� �ڷᵵ �����־�� �Ѵ�. + ����
	// 8�� ��쿣 ���� �ʿ䰡 ����.
	ids = AddNonDBBookDM( nMailClass , sWhere , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeMailInfo") );

	ids = MakeMailRecord( nMailClass , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeMailInfo") );

	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::MakeMailInfo( INT nMailClass , CStringArray &saWhere )
{
	EFS_BEGIN
		
	INT ids;
	
	// ���� ���� DM ����
	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

	pLocDM->FreeData();

	CString sWhere;

	INT nWhereCnt = saWhere.GetSize();

	CESL_DataMgr TmpDM;
	ids = CLocCommonAPI::MakeCopyDM( this , pLocDM , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailInfo") );

	for( INT i = 0 ; i < nWhereCnt ; i++ )
	{
		sWhere = saWhere.GetAt(i);
		ids = MakeSql( nMailClass , sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeMailInfo") );

		TmpDM.FreeData();
		ids = TmpDM.RefreshDataManager(sWhere);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeMailInfo") );

		// 2011.09.07 ADD : 3000Api
		if(TRUE == m_pInfo->m_bLibSecuUse && 0 < TmpDM.GetRowCount() && _T("DM_LOC_MAIL_INFO") == pLocDM->GETDMAlias())
		{
			CString strEnc;
			CStringArray asDes1;
			for(INT i = 0; i < TmpDM.GetRowCount(); i++)
			{
				TmpDM.GetCellData(_T("E_MAIL"), i, strEnc);
				asDes1.Add(strEnc);
			}
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
						pApi->CloseSocket();	
					}
					for(i = 0; i < TmpDM.GetRowCount(); i++)
					{
						TmpDM.SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
					}
				}
				delete pApi;
				pApi = NULL;
			}
			asDes1.RemoveAll(); asDes1.FreeExtra();
		}

		ids = CLocCommonAPI::AddDM( &TmpDM , pLocDM , this );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeMailInfo") );		
	}
	
	
	// û����ȣ ����
	if( nMailClass != 8 )
		CLocCommonAPI::MakeShelfCode( this , pLocDM->DB_MGR_ALIAS );
	
	
	// nMailClass�� 0���� 7�� ��� ��� ���� �ڷᵵ �����־�� �Ѵ�. + ����
	ids = AddNonDBBookDM( nMailClass , sWhere , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeMailInfo") );
	
	ids = MakeMailRecord( nMailClass , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeMailInfo") );
	
	return 0;
	
	EFS_END
		return -1;
	
}

// nMailClass�� 0���� 7�� ��� ��� ���� �ڷᵵ �����־�� �Ѵ�. + ���� �ڷ�
INT CLocMailInfo::AddNonDBBookDM( INT nMailClass , CString sWhere , CESL_DataMgr *pLocDM )
{
	EFS_BEGIN

	INT ids;

	// 2005.04.08 UPDATE BY PDJ
	if( !( (nMailClass==0) || (nMailClass==1) || (nMailClass==2) || 
		   (nMailClass==3) || (nMailClass==4) || (nMailClass==5) || 
		   (nMailClass==6) || (nMailClass==7) || (nMailClass==9) || (nMailClass==10)) )
	{		
		return 0;
	}

	// 2005.04.08 ADD BY PDJ
	// 5163�� ��츸 9�� �ݳ������̴�.
	// ������ 9�� ������. 10�� �ݳ������̴�.
	if(m_pSendMailMgr->Check_5163MailSystem() == TRUE)
	{
		if ( nMailClass==10 ) return 0;
	}
	else
	{
		if ( nMailClass==9 ) return 0;
	}
		

	CString sNonDBBookDMAlias = _T("DM_LOC_MAIL_INFO_NON_DB");

	ids = MakeSql( nMailClass , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeLocDMAddTmpDM") );

	// ��޴����� ��� book_tbl�� Alias�� BB�� �ƴ϶� BL�̴�.
	// DB�� �⺻ Condition���� �� �־ �ε��� ������.
	// �ƿ� �ҽ������� �����߸� �����ϹǷ� ��ȯ�� ������.
	sWhere.Replace(_T("BB."), _T("NB."));	

	CESL_DataMgr *pNonDM = FindDM( sNonDBBookDMAlias );
	if( pNonDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddNonDBBookDM") );

	// 2009.02.25 ADD BY PWR : ��Ȳ���� �����ؾ��ϴ°��� LS_WORK_T01�� ���̺� ġȯ
	if( nMailClass == 0 || nMailClass == 1 || nMailClass == 2 || 
		nMailClass == 3 || nMailClass == 7 || nMailClass == 10 )
	{
		pNonDM->TBL_NAME.Replace(_T("LH_STORAGE_V01"), _T("LS_WORK_T01"));
	}
	else
	{
		pNonDM->TBL_NAME.Replace(_T("LS_WORK_T01"), _T("LH_STORAGE_V01"));
	}

	ids = pNonDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("AddNonDBBookDM") );

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pNonDM->GetRowCount())
	{
		CString strEnc;
		CStringArray asDes1;
		for(INT i = 0; i < pNonDM->GetRowCount(); i++)
		{
			pNonDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes1.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pNonDM->GetRowCount(); i++)
				{
					pNonDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	ids = CLocCommonAPI::AddDM( pNonDM , pLocDM , this ); 
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("AddNonDBBookDM") );

	CString sSEDBBookDMAlias = _T("DM_LOC_MAIL_INFO_SE_DB");

	CESL_DataMgr *pSEDM = FindDM( sSEDBBookDMAlias );
	if( pSEDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddNonDBBookDM") );

	// 2009.02.25 ADD BY PWR : ��Ȳ���� �����ؾ��ϴ°��� LS_WORK_T01�� ���̺� ġȯ
	if( nMailClass == 0 || nMailClass == 1 || nMailClass == 2 || 
		nMailClass == 3 || nMailClass == 7 || nMailClass == 10 )
	{
		pSEDM->TBL_NAME.Replace(_T("LH_STORAGE_V01"), _T("LS_WORK_T01"));
	}
	else
	{
		pSEDM->TBL_NAME.Replace(_T("LS_WORK_T01"), _T("LH_STORAGE_V01"));
	}

	sWhere.Replace(_T("NB."), _T("BB."));

	ids = pSEDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("AddNonDBBookDM") );

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pSEDM->GetRowCount())
	{
		CString strEnc;
		CStringArray asDes1;
		for(INT i = 0; i < pSEDM->GetRowCount(); i++)
		{
			pSEDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes1.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pSEDM->GetRowCount(); i++)
				{
					pSEDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	ids = CLocCommonAPI::AddDM( pSEDM , pLocDM , this ); 
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("AddNonDBBookDM") );

	// û����ȣ ����
	if( nMailClass != 8 )		
		CLocCommonAPI::MakeShelfCode( this , pLocDM->DB_MGR_ALIAS );

	return 0;

	EFS_END
	return -1;

}

// DM���� ���� ���ڵ带 �����Ѵ�.
INT CLocMailInfo::MakeMailRecord( INT nMailClass , CESL_DataMgr *pLocDM )
{
	EFS_BEGIN

	INT ids;

	// 1. ������ ���� �����´�.
	CArray < CString , CString > saOrder;

	ids = GetOrderArray( nMailClass , saOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailRecord") );


	// 2. ������ �Ѵ�.
	ids = CLocCommonAPI::SortDM( this , pLocDM , saOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailRecord") );

	// 3. ���� ���ڵ��� ���� �����Ѵ�.
	// 4. ���� ���ڵ忡 �߰��Ѵ�.
	ids = MakeAndAddMailRecord( nMailClass , pLocDM , saOrder );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailRecord") );
	}

	return 0;

	EFS_END
	return -1;

}

// DM���� ���� ���ڵ� �����Ͽ� �߰�
INT CLocMailInfo::MakeAndAddMailRecord( INT nMailClass , CESL_DataMgr *pLocDM , CArray < CString , CString > &saOrder )
{
	EFS_BEGIN

	INT ids;

	CString	strCheckMailClass;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowCount = m_pMailModeInfoDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				break;
			}
		}
	}	

	// 2006-11-21 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++
	// ������ ��ã�Ҵٸ� �ƿ�~
	if ( i == nRowCount ) return 1;
	// ---------------------------------------------------------------

	if( m_sEcoMailUseYN.Compare(_T("Y")) == 0 )
	{
		INT nMailSendCnt = 0;
		CString sMailClass;
		sMailClass.Format( _T("%d") , nMailClass );

		CString sMailInfoKey;
		CString sMailName;

		ids = m_pMailModeInfoDM->GetCellData( _T("��������KEY") , nRowIndex , sMailInfoKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("MakeAndAddMailRecord") );

		ids = m_pMailModeInfoDM->GetCellData( _T("�����̸�") , nRowIndex , sMailName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("MakeAndAddMailRecord") );

		CString sTmpResult;
		CString sResult;

		CString sLoanKeyList;
		CString sLoanKey;
		CString sUserKey;
		BOOL bAddRecord = FALSE;
		
		INT nMailCnt = pLocDM->GetRowCount();	

		if( nMailCnt == 0 )
		{
			AfxMessageBox( _T("������ ������ �� �����ϴ�. "));
			return 1010;
		}

		pLocDM->StartFrame();
		for( INT i = 0 ; i < nMailCnt ; i++ )
		{

			if( i != pLocDM->GetRowCount()-1 )
			{
				for( INT k = 0 ; k < saOrder.GetSize() ; k++ )
				{
					ids = pLocDM->GetCellData( saOrder.GetAt(k) , i , sTmpResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeAndAddMailRecord") );

					ids = pLocDM->GetCellData( saOrder.GetAt(k) , i+1 , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeAndAddMailRecord") );

					if( sTmpResult.Compare( sResult ) != 0 )
						bAddRecord = TRUE;
				}
			}
			else
			{
				bAddRecord = TRUE;
			}

			if( sMailClass.Compare(_T("8")) != 0 && sMailClass.Compare(_T("20")) != 0 )
			{
				ids = pLocDM->GetCellData( _T("����/�ݳ�����KEY") , i , sLoanKey );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeAndAddMailRecord") );				

				if( !sLoanKeyList.IsEmpty() ) sLoanKeyList += _T(",");
				sLoanKeyList += sLoanKey;
			}

			if( saOrder.GetSize() == 0 ) bAddRecord = TRUE;


			if( bAddRecord )
			{
				if( sMailClass.Compare(_T("8")) != 0 && sMailClass.Compare(_T("20")) != 0 )
				{
					ids = pLocDM->GetCellData( _T("�̿���KEY") , i , sUserKey );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeAndAddMailRecord") );
				}
				else
				{
					ids = pLocDM->GetCellData( _T("�����KEY") , i , sUserKey );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("MakeAndAddMailRecord") );
				}

				// 2009.02.26 ADD BY CJY : �������� �׸� �߰�
				CString strManageCode;
				if(NULL != m_pInfo && _T("") != m_pInfo->MANAGE_CODE)
				{
					strManageCode = m_pInfo->MANAGE_CODE;
				}
				else
				{
					strManageCode = _T("UDF_MANAGE_CODE");
				}
				/// Temp�� INSERT�ϱ�
				pLocDM->InitDBFieldData();
				pLocDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , _T("esl_mail_seq.NextVal") );
				pLocDM->AddDBFieldData( _T("MAIL_KEY") , _T("NUMERIC") , sMailInfoKey );
				pLocDM->AddDBFieldData( _T("LOAN_KEY_LIST") , _T("STRING") , sLoanKeyList );
				pLocDM->AddDBFieldData( _T("USER_KEY") , _T("NUMERIC") , sUserKey );
				pLocDM->AddDBFieldData( _T("LIB_NAME") , _T("STRING") , m_sLibName );
				pLocDM->AddDBFieldData( _T("MAIL_CLASS") , _T("STRING") , sMailClass );

				// 2009.02.26 ADD BY CJY : �������� �׸� �߰�
				pLocDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , strManageCode);
				pLocDM->MakeInsertFrame(_T("MN_MAIL_TEMP_TBL"));

				sUserKey.Empty();
				sLoanKeyList.Empty();
				bAddRecord = FALSE;

				nMailSendCnt++;
			}

		}

		ids = pLocDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("MakeAndAddMailRecord") );

		pLocDM->EndFrame();

		CString sMsg;
		sMsg.Format( _T("[%s] %d���� ���� ������ ������� �ڷ�� �����Ǿ����ϴ�.") , sMailName , nMailSendCnt );
		AfxMessageBox( sMsg );

	}
	else
	{
		CString strMailClass;
		strMailClass.Format( _T("%d") , nMailClass );

		m_pSendMailMgr->DeleteAllMailRecord();	

		// ���� �ٵ� �����Ѵ�.
		CString sTmpResult;
		CString sResult;

		CString sBody;
		CString sTmpBody;
		BOOL bAddRecord = FALSE;
		
		
		// ���� �߰�
		// ���� �߼۰Ǽ��� 100������ ����
		// �ʰ��� 100������ ��� ����
		INT mailCnt = pLocDM->GetRowCount();

		/*
		if( mailCnt > POSSIBLE_MAIL_CNT )
		{
			CString msg;
			msg.Format( 
				_T("1ȸ ���� �߼۰Ǽ��� %d ������ ���ѵǾ��ֽ��ϴ�.\n")
				_T("���� ������ ����� %d ����� �����Ͽ����ϴ�."),
				POSSIBLE_MAIL_CNT, POSSIBLE_MAIL_CNT );
			MessageBox( msg );
			mailCnt = POSSIBLE_MAIL_CNT;
		}
		*/
		CString strMailInfoKey;
		CString strMailName;

		ids = m_pMailModeInfoDM->GetCellData( _T("��������KEY") , nRowIndex , strMailInfoKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1000 , _T("MakeAndAddMailRecord") );

		ids = m_pMailModeInfoDM->GetCellData( _T("�����̸�") , nRowIndex , strMailName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1010 , _T("MakeAndAddMailRecord") );

		CString strLoanKeyList;
		CString strLoanKey;
		CString strUserKey;

		for( INT i = 0 ; i < mailCnt ; i++ )
		{

			if( i != pLocDM->GetRowCount()-1 )
			{
				for( INT k = 0 ; k < saOrder.GetSize() ; k++ )
				{
					ids = pLocDM->GetCellData( saOrder.GetAt(k) , i , sTmpResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeAndAddMailRecord") );

					ids = pLocDM->GetCellData( saOrder.GetAt(k) , i+1 , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeAndAddMailRecord") );

					if( sTmpResult.Compare( sResult ) != 0 )
						bAddRecord = TRUE;
				}
			}
			else
			{
				bAddRecord = TRUE;
			}

			if( strMailClass.Compare(_T("8")) != 0 )
			{
				ids = pLocDM->GetCellData( _T("����/�ݳ�����KEY") , i , strLoanKey );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("MakeAndAddMailRecord") );				

				if( !strLoanKeyList.IsEmpty() ) strLoanKeyList += _T(",");
				strLoanKeyList += strLoanKey;
			}

			ids = m_pMailModeInfoDM->GetCellData( _T("�߰���") , nRowIndex , sTmpBody );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("MakeAndAddMailRecord") );

			ids = MakeAliasToRealValue(sTmpBody,pLocDM,i);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeAndAddMailRecord") );

			//2004-12-30 leehakjoong
			//sBody += _T("<br><br>") + sTmpBody;
			sBody += _T("<br>") + sTmpBody;

			if( saOrder.GetSize() == 0 ) bAddRecord = TRUE;

			if( bAddRecord )
			{
				ids = SettingMailInfo( nMailClass , pLocDM , i , sBody );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeAndAddMailRecord") );

				m_pMailRecord->m_sBody = m_sBody;
				m_pMailRecord->m_sRecipientAddress = m_sEMail;	//���ڿ���
				m_pMailRecord->m_sRecipientName	 = m_sRecipientName;	//��û���̸�
				m_pMailRecord->m_sSubject = m_sTitle;
				
				// 2009.03.03 ADD BY CJY : �������ۿ� �߰�
				if( strMailClass.Compare(_T("8")) != 0 )
				{
					ids = pLocDM->GetCellData( _T("�̿���KEY") , i , strUserKey );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeAndAddMailRecord") );
				}
				else
				{
					ids = pLocDM->GetCellData( _T("�����KEY") , i , strUserKey );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("MakeAndAddMailRecord") );
				}

				m_pMailRecord->m_sMailInfoKey = strMailInfoKey;
				m_pMailRecord->m_sLoanKeyList = strLoanKeyList;
				m_pMailRecord->m_sUserKey = strUserKey;
				m_pMailRecord->m_sLibName = m_sLibName;
				m_pMailRecord->m_sMailClass = strMailClass;
			
				CString strManageCode;
				if(NULL != m_pInfo && _T("") != m_pInfo->MANAGE_CODE)
				{
					m_pMailRecord->m_sManageCode = m_pInfo->MANAGE_CODE;
				}
				else
				{
					m_pMailRecord->m_sManageCode = _T("UDF_MANAGE_CODE");
				}

				//2004-12-15 
				m_pMailRecord->TO_EMP_CODE = m_strTO_EMP_CODE ;
				m_pSendMailMgr->AddMailRecord(m_pMailRecord);
// 				ids = AddMailRecord();
// 				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeAndAddMailRecord") );

				sBody.Empty();

				bAddRecord = FALSE;
			}

		}
	}


	return 0;

	EFS_END
	return -1;

}

// ���ڵ忡 �߰�
INT CLocMailInfo::SettingMailInfo( INT nMailClass , CESL_DataMgr *pLocDM , INT nIndex , CString sBody )
{
	EFS_BEGIN

	INT ids;

	CString	strCheckMailClass;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowCount = m_pMailModeInfoDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				break;
			}
		}
	}
	

	// �޴� ��� �̸�
	ids = pLocDM->GetCellData( _T("�̿��ڸ�") , nIndex , m_sRecipientName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SettingMailInfo") );

	// �޴� ��� EMAIL
	ids = pLocDM->GetCellData( _T("E_MAIL") , nIndex , m_sEMail );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SettingMailInfo") );



	//2004-12-15 
	m_strTO_EMP_CODE = _T("") ;
	ids = pLocDM->GetCellData( _T("�����ڹ�ȣ") , nIndex , m_strTO_EMP_CODE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SettingMailInfo") );



/*
	// ������ ��� �̸�
	ids = m_pMailModeInfoDM->GetCellData( _T("�����»���̸�") , nMailClass , m_sCCName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SettingMailInfo") );

	// ������ ��� EMAIL
	ids = m_pMailModeInfoDM->GetCellData( _T("�����»��E_MAIL") , nMailClass , m_sCCAddress );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SettingMailInfo") );
*/	
	// ���� ����
	ids = m_pMailModeInfoDM->GetCellData( _T("����") , nRowIndex , m_sTitle );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("SettingMailInfo") );

	ids = MakeAliasToRealValue( m_sTitle , pLocDM , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SettingMailInfo") );

	// ���� ���� ����
	CString sHeader,sTail;
	ids = m_pMailModeInfoDM->GetCellData( _T("�Ӹ���") , nRowIndex , sHeader );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SettingMailInfo") );

	ids = m_pMailModeInfoDM->GetCellData( _T("������") , nRowIndex , sTail );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SettingMailInfo") );

	m_sBody = sHeader + _T("\r\n") + sBody + _T("\r\n") + sTail;

	ids = MakeAliasToRealValue( m_sBody , pLocDM , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SettingMailInfo") );

	m_sBody.Replace( _T("\r\n") , _T("<br>") );
	m_sBody.Replace( _T("\n") , _T("<br>") );

	return 0;

	EFS_END
	return -1;

}

// ���� ���� �߰�
INT CLocMailInfo::AddMailRecord()
{
	EFS_BEGIN

// 2009.03.03 DEL BY CJY : �̻�� �Լ�
// 	///Mail Record ��ü ����
// 	m_pMailRecord->m_sBody = m_sBody;
// 	m_pMailRecord->m_sRecipientAddress = m_sEMail;	//���ڿ���
// 	m_pMailRecord->m_sRecipientName	 = m_sRecipientName;	//��û���̸�
// 	m_pMailRecord->m_sSubject = m_sTitle;
// 
// 
// 	//2004-12-15 
// 	m_pMailRecord->TO_EMP_CODE = m_strTO_EMP_CODE ;
// 
// 
// /*
// 	m_pMailRecord->m_sCCName    = m_sCCName;	// ������ ���
// 	m_pMailRecord->m_sCCAddress = m_sCCAddress;			// ������ ��� �ּ�	
// */		
// 	// 2005.03.04 UPDATE BY PDJ
// 	// 5163�� ��� TO_EMP_CODE(�����ڹ�ȣ)�� 00000~99999 �� ������ �ƴ϶�� 
// 	// ������ �߼����� �ʴ´�.
// 	/*
// 	if(m_pSendMailMgr->Check_5163MailSystem() == TRUE)
// 	{
// 		INT nEmpCode;
// 		nEmpCode = _ttoi(m_pMailRecord->TO_EMP_CODE);
// 		if ( !(nEmpCode >= 0 && nEmpCode <= 99999) ) 
// 		{
// 			return 0;
// 		}
// 	}
// 	*/
// 	///Mail Record ��ü �߰�
// 	m_pSendMailMgr->AddMailRecord(m_pMailRecord);

	return 0;

	EFS_END
	return -1;

}
// �˸��ƽ��� VALUE�� �����ϴ� �۾�
INT CLocMailInfo::MakeAliasToRealValue( CString &sString , CESL_DataMgr *pLocDM , INT nIndex )
{
	EFS_BEGIN

	INT ids;



	CString sAlias;
	CString sValue;
	for( INT i = 0 ; i < m_pMailAliasDM->GetRowCount() ; i++ )
	{
		sValue.Empty();

		ids = m_pMailAliasDM->GetCellData( _T("�˸��ƽ�") , i , sAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeAndAddMailRecord") );

		ids = pLocDM->GetCellData( sAlias , nIndex , sValue );

		if( sAlias.Compare(_T("��������")) == 0 )
//			sValue = sLibInfo[0];
			sValue = m_sLibName;

		sAlias = _T("%") + sAlias + _T("%");
		sString.Replace( sAlias , sValue );
	}

	sString.Replace( _T("''") , _T("'") );

	return 0;

	EFS_END
	return -1;

}


// ���� ������ ���� ���� ������ �����´�.
INT CLocMailInfo::GetOrderArray( INT nMailClass , CArray < CString , CString > &saOrder )
{
	EFS_BEGIN

	INT ids;

	CString sMailClass;
	CString sOrder;

	for( INT i = 0 ; i < m_pMailModeInfoDM->GetRowCount(); i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , sMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetOrderArray") );

		if( _ttoi(sMailClass)==nMailClass ) 
		{
			ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , sOrder ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetOrderArray") );
			sOrder.TrimLeft(); sOrder.TrimRight();

			if( sOrder.IsEmpty() ) return 0;

			GetOrderArray( saOrder , sOrder );

			return 0;
		}
	}

	return 0;

	EFS_END
	return -1;

}

// sOrder�� ','�α����Ͽ� saOrder�� ���� �ִ´�.
INT CLocMailInfo::GetOrderArray(CArray < CString , CString > &saOrder,CString sOrder)
{
	EFS_BEGIN

	INT nCommaIndex;

	saOrder.RemoveAll();
	
	bool bIsEnd = FALSE;
	while(TRUE)
	{
		nCommaIndex=sOrder.Find(_T(","),0);
		if( nCommaIndex < 0 )
			bIsEnd = TRUE;

		if( nCommaIndex > -1 )
		{
			saOrder.Add( sOrder.Mid(0,nCommaIndex) );
			sOrder = sOrder.Mid( nCommaIndex+1 , sOrder.GetLength()-nCommaIndex);
		}
		else
			saOrder.Add( sOrder );


		if( bIsEnd ) return 0;
	}
	return 0;

	EFS_END
	return -1;

}

// ���� ������ ���� ���Ǵ� DM�� �ٸ���.
CESL_DataMgr* CLocMailInfo::FindUseLocDM( INT nMailClass )
{
EFS_BEGIN

	// 2009.02.25 ADD BY PWR : ��Ȳ���� �����ؾ��ϴ°��� LS_WORK_T01�� ���̺� ġȯ
	if( nMailClass == 0 || nMailClass == 1 || nMailClass == 2 || 
		nMailClass == 3 || nMailClass == 7 || nMailClass == 10 )
	{
		m_pLoanMailDM->TBL_NAME.Replace(_T("LH_STORAGE_V01"), _T("LS_WORK_T01"));
	}
	else
	{
		m_pLoanMailDM->TBL_NAME.Replace(_T("LS_WORK_T01"), _T("LH_STORAGE_V01"));
	}

	if( (nMailClass >= 0) && (nMailClass <= 7) || (nMailClass == 9) || (nMailClass == 10))
	{
		
		return m_pLoanMailDM;
	}
	// 2007.04.04 UPDATE BY PDW : Ż��ȳ����� �߰�
	else if( (nMailClass == 8) || (nMailClass == 20) )
	{
		return m_pUserMailDM;
	}

	return NULL;

	EFS_END
	return NULL;

}

CESL_DataMgr* CLocMailInfo::FindTempUseLocDM( INT nMailClass )
{
	EFS_BEGIN

	// 2009.02.25 ADD BY PWR : ��Ȳ���� �����ؾ��ϴ°��� LS_WORK_T01�� ���̺� ġȯ
	if( nMailClass == 0 || nMailClass == 1 || nMailClass == 2 || 
		nMailClass == 3 || nMailClass == 7 || nMailClass == 10 )
	{
		m_pTmpLoanMailDM->TBL_NAME.Replace(_T("LH_STORAGE_V01"), _T("LS_WORK_T01"));
	}
	else
	{
		m_pTmpLoanMailDM->TBL_NAME.Replace(_T("LS_WORK_T01"), _T("LH_STORAGE_V01"));
	}

	// 2005.03.08 UPDATE BY PDJ
	// 9���� �ݳ������ȳ��� 9���� ���ƾ� ��.
	if( (nMailClass >= 0) && (nMailClass <= 10) )
	//if( (nMailClass >= 0) && (nMailClass <= 8) )
	{
		return m_pTmpLoanMailDM;
	}
	else if( nMailClass == 8 )
	{
		return NULL;
	}

	return NULL;	

	EFS_END
	return NULL;

}

INT CLocMailInfo::MakeChangeAliasDM()
{
	EFS_BEGIN

	INT ids;

	CString sFilePath = _T("..\\cfg\\MailAlias\\MailAlias.txt");

	CStdioFile fFile;
	ids = fFile.Open( sFilePath , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( ids == 0 )
	{
		AfxMessageBox(_T("�˸��ƽ� ������ ���µ� �����߽��ϴ�."));
		return 1;
	}
		TCHAR cUni;
		fFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fFile.SeekToBegin();
		}
	
	m_pMailAliasDM->FreeData();

	CString sString;
	CString sAlias;
	CString sDesc;
	INT nIndex;

	while( fFile.ReadString( sString ) )
	{
		nIndex = -1;
		sAlias.Empty(); sDesc.Empty();
		sString.TrimLeft();  sString.TrimRight();
		if( sString.IsEmpty() ) continue;
		
		nIndex = sString.Find(_T("|"));

		if( nIndex < 0 ) 
		{
			sAlias = sString;
		}
		else
		{
			sAlias = sString.Mid( 0 , nIndex );
			sDesc = sString.Mid( nIndex+1 , sString.GetLength() - nIndex );
		}
		sAlias.TrimLeft(); sAlias.TrimRight();
		sDesc.TrimLeft(); sDesc.TrimRight();
		
		ids = m_pMailAliasDM->SetCellData( _T("�˸��ƽ�") , sAlias , -1  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeChangeAliasDM") );
	}

	fFile.Close();

	return 0;

	EFS_END
	return -1;

}


INT CLocMailInfo::SendMail( INT nMailClass , bool bShowWindow )
{
	EFS_BEGIN

	INT ids;

	if( IsUseMailClass( nMailClass ) != 0 )
		return 1;

	/// ECO Mailing System�̶�� ������ ������ �ʴ´�.
	if( m_sEcoMailUseYN.Compare(_T("Y")) == 0 ) 
		return 2;

	// 2006.12.21 ADD BY PDJ
	// ������ ����ڸ����� �����Ѵ�.
	// �ش������ ���� ��� ������ ���� ������ ����
	CString strMailType;
	CString strMailServer;
	CString strAccountYN;
	CString strAccount;
	CString strID;
	CString strPassword;
	CString strSenderName;
	CString strMailAddress;

	strMailType = GetMailType(nMailClass);
	if ( strMailType != _T("") )
	{		
		CMailAccountMgr MailAccountMgr;
		MailAccountMgr.GetMailAccount(strMailType, strAccount, strID, strPassword, strSenderName, strMailAddress);
		MailAccountMgr.GetMailServer(strMailServer, strAccountYN);		
	}

	// 2007.09.04 update by pdw +++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	// bShowWindow ���� ���� POPUP���� ���� 
	/*
	if ( strMailAddress != _T("") )
	{
		m_pSendMailMgr->DoModal(MAIL_SERVER_USER, strMailServer, strMailAddress, strAccountYN, strID, strPassword);
	}
	else if( bShowWindow )
	{
		m_pSendMailMgr->DoModal(MAIL_SERVER_CFGFILE);
	}
	else
	{
		m_pSendMailMgr->SetMailServerInfoFromCfgFile(TRUE);
		m_pSendMailMgr->SendMail(FALSE,TRUE);
	}
	*/
	if( bShowWindow )
	{
		if( strMailAddress.IsEmpty() )
		{
			m_pSendMailMgr->DoModal(MAIL_SERVER_CFGFILE);
		}
		else
		{
			m_pSendMailMgr->DoModal(MAIL_SERVER_USER, strMailServer, strMailAddress, strAccountYN, strID, strPassword);
		}
	}
	else
	{
		if( strMailAddress.IsEmpty() )
		{
			m_pSendMailMgr->SetMailServerInfoFromCfgFile(TRUE);
			m_pSendMailMgr->SendMail(FALSE,TRUE);
		}
		else
		{
			m_pSendMailMgr->SetDefaultMailServer( strMailServer, strMailAddress, strAccountYN, strID, strPassword );
			m_pSendMailMgr->SendMail(FALSE,TRUE);
		}
	}
	// --------------------------------------------------------------------------------

	ids = MakeSuccessYNList( nMailClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SendMail") );

	ids = InsertDBSuccessYN( nMailClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SendMail") );

	return 0;

	EFS_END
	return -1;

}

// ������ �߼� ������ �ڷḦ INSERT�Ѵ�.
INT CLocMailInfo::InsertDBSuccessYN( INT nMailClass )
{
	EFS_BEGIN

	INT ids;

	CString sAlias;
	CString sLoanKey;
	CString sMailClass;
	CString sYN;
	CString sTableName = _T("CO_LOAN_MAIL_SEND_TBL");
	sMailClass.Format( _T("%d") , nMailClass );

	if( nMailClass == 8 ) 
	{
		sAlias = _T("�����KEY");
	}
	else if( nMailClass > -1 && (nMailClass < 10))
	{
		sAlias = _T("����/�ݳ�����KEY");
	}
	else
		return 0;

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertDBSuccessYN") );

	CString sRecKey;
	for( INT i = 0 ; i < pLocDM->GetRowCount() ; i++ )
	{
		sRecKey.Empty();
		ids = pLocDM->GetCellData( _T("��������") , i , sYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertDBSuccessYN") );

		if( sYN.Compare(_T("Y")) != 0 ) continue;

		// DB�� INSERT
		// REC_KEY ��������
		pLocDM->StartFrame();
		pLocDM->InitDBFieldData();

		ids = pLocDM->MakeRecKey(sRecKey);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertDBSuccessYN") );

		ids = pLocDM->GetCellData( sAlias , i , sLoanKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertDBSuccessYN") );

		pLocDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pLocDM->AddDBFieldData( _T("LOAN_KEY") , _T("NUMERIC") , sLoanKey );
		pLocDM->AddDBFieldData( _T("MAIL_CLASS") , _T("STRING") , sMailClass );
		pLocDM->AddDBFieldData( _T("MAIL_SEND_DATE") , _T("NUMERIC") , _T("SYSDATE") );

		pLocDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") );

		pLocDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("���Ϻ�����"), __WFILE__, __LINE__ ), TRUE );
		pLocDM->MakeInsertFrame( sTableName );
		
		ids = pLocDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertDBSuccessYN") );
		
		// 2006-03-27 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ������ �����ʴ´�.
		ids = pLocDM->EndFrame();
		// -----------------------------------------------------------------------------------
	}

	return 0;

	EFS_END
	return -1;

}

// ������ �߼� ���� ���θ� DM�� ����Ѵ�.
INT CLocMailInfo::MakeSuccessYNList( INT nMailClass )
{
	EFS_BEGIN


	INT nMailIndex = 0;
	INT ids;
	bool bNextMailRecord = FALSE;
	INT nFailCode;
	INT	i;

	CString sResult;
	CString sTmpResult;
	CString sYN;
	CString sEMail;
	CString sFailReason;
	CString	strLoanNum;

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSuccessYNList") );

	CArray < CString , CString > saOrder;
	ids = GetOrderArray( nMailClass , saOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSuccessYNList") );

	/*
	// 2005.03.08 ADD BY PDJ
	// 5163�� ��� �����ڹ�ȣ�� 00000~99999�ΰ�� 
	// ���� �߼� ��󿡼� ���ܵǹǷ� üũ���� �ʴ´�.
	INT	nRowCount;
	nRowCount = pLocDM->GetRowCount();
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = pLocDM->GetCellData( _T("�����ڹ�ȣ") , nRowCount-1- i , strLoanNum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeAndAddMailRecord") );
		if ( !(_ttoi(strLoanNum) >= 0 && _ttoi(strLoanNum) <= 99999) )
		{
			ids = pLocDM->DeleteRow(nRowCount-1- i);
		}
	}
	*/


	for( i = 0 ; i < pLocDM->GetRowCount() ; i++ )
	{

		if( i != pLocDM->GetRowCount()-1 )
		{
			for( INT k = 0 ; k < saOrder.GetSize() ; k++ )
			{
				ids = pLocDM->GetCellData( saOrder.GetAt(k) , i , sTmpResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeAndAddMailRecord") );

				ids = pLocDM->GetCellData( saOrder.GetAt(k) , i+1 , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeAndAddMailRecord") );

				if( sTmpResult.Compare( sResult ) != 0 )
					bNextMailRecord = TRUE;
			}
		}

		nFailCode = m_pSendMailMgr->GetMailRecordSendState( nMailIndex );
		if( nFailCode==0 ) sYN = _T("Y");
		else
		{
			if( nFailCode == -3 ) sFailReason = _T("���� ������ ������ �����Ͽ����ϴ�.");
			if( nFailCode == -2 ) sFailReason = _T("E_MAIL �ּҰ� ��ϵǾ� ���� ���� �̿����Դϴ�.");
			if( nFailCode == 1 ) sFailReason = _T("������ �߼����� �ʾҽ��ϴ�.");
			if( nFailCode == -1 ) sFailReason = _T("�˼� ���� �����Դϴ�.���� ���� �ּҸ� Ȯ�����ֽʽÿ�.");

			sYN = _T("N");
		}

		if( saOrder.GetSize() == 0 ) bNextMailRecord = TRUE;

		ids = pLocDM->SetCellData( _T("��������") ,sYN ,i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeSuccessYNList") );
		ids = pLocDM->SetCellData( _T("���л���") ,sFailReason ,i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeSuccessYNList") );


		if( bNextMailRecord )
		{
			nMailIndex++;
			bNextMailRecord = FALSE;
		}
	}
	
	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::SetInitValue()
{
	EFS_BEGIN

	m_sSMAlias = _T("SM_LOC_MAIL_INFO");
	m_sLoanDMAlias = _T("DM_LOC_MAIL_INFO");
	m_sUserDMAlias = _T("DM_USER_MAIL_INFO");
	m_sMailModeDMAlias = _T("DM_LOC_MAIL_MODE_INFO");

	INT ids;
	ids = InitESL_Mgr(m_sSMAlias);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CLocMailInfo") );

	m_pLoanMailDM = FindDM( m_sLoanDMAlias );
	if( m_pLoanMailDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CLocMailInfo") );

	m_pUserMailDM = FindDM( m_sUserDMAlias );
	if( m_pUserMailDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CLocMailInfo") );

	m_pMailModeInfoDM = FindDM( m_sMailModeDMAlias );
	if( m_pMailModeInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CLocMailInfo") );

	m_pMailAliasDM = FindDM( _T("DM_CHANGE_ALIAS") );
	if( m_pMailAliasDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CLocMailInfo") );

	m_pTmpLoanMailDM = FindDM( _T("DM_TEMP_LOC_MAIL_INFO") );
	if( m_pTmpLoanMailDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CLocMailInfo") );

	ids = MakeInitInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CLocMailInfo") );

	ids = MakeChangeAliasDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CLocMailInfo") );

	// ���� ����
	m_pMailRecord = new SendMailRecord;

	m_pMailRecord->m_nFileAttachCnt = 0;
	m_pMailRecord->m_pFileAttach = _T("");
	m_pMailRecord->m_sEncoding = _T("text/html");

	m_pSendMailMgr = new CSendMailMgr(this);


	return 0;

	EFS_END
	return -1;

}


INT CLocMailInfo::MakeSql( INT nMailClass , CString &sWhere )
{
	EFS_BEGIN

	INT ids;

	CString sDefaultWhere;

	CString	strCheckMailClass;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowIndex = 0;
	nRowCount = m_pMailModeInfoDM->GetRowCount();

	// 2007.09.03 update by pdw ++++++++++++++++++++++++++++++++++++++++++
	// ������ ������ ���� ��뿩�ΰ� �ȵǾ� ������ �ִ�.
	// ������ ã�Ҵ��� Ȯ���Ѵ�.
	BOOL bFind = FALSE;

	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				bFind  = TRUE;
				break;
			}
		}
	}	

	if( !bFind )
		return 100;
	// ------------------------------------------------------------------

	ids = m_pMailModeInfoDM->GetCellData( _T("�⺻SQL") , nRowIndex , sDefaultWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSql") );
	sDefaultWhere.TrimLeft(); sDefaultWhere.TrimRight();

	// 2005.01.25 ADD BY SSM
	// �������� Ÿ�� ����
	sDefaultWhere.Replace(_T("CL.RESERVATION_DATE"), _T("to_char(CL.RESERVATION_DATE, 'YYYY/MM/DD')"));
	sDefaultWhere.Replace(_T("CL_TEMP.RESERVATION_DATE"), _T("to_char(CL_TEMP.RESERVATION_DATE, 'YYYY/MM/DD')"));
	// SSM C++

	sDefaultWhere.Replace( _T("''") , _T("'") );
	sDefaultWhere.Replace( _T("''") , _T("'") );

	if( sDefaultWhere.IsEmpty() ) return 0;

	if( sWhere.GetLength() > 1 )
		sWhere += _T(" AND ");

	sWhere += sDefaultWhere;

	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::GetMailCnt()
{
	EFS_BEGIN

	return m_pSendMailMgr->GetCountMailRecord();

	EFS_END
	return -1;

}

// �ϰ� ������ ����� ����� �� ������ �˻��Ѵ�.
// �ϳ��� ����� �� �ִٸ� ��밡���̴�.
// return 0 : ��밡��
// return 1 : ���Ұ�
INT CLocMailInfo::IsUseBatchSendMail()
{
	EFS_BEGIN

	CString	strCheckMailClass;
	INT ids;

	CString sData;
	for( INT i = 0 ; i < m_pMailModeInfoDM->GetRowCount() ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , strCheckMailClass );
		ids = IsUseBatchSendMail(_ttoi(strCheckMailClass));
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseBatchSendMail") );
		if( ids == 0 ) return 0;
	}

	return 1;

	EFS_END
	return -1;

}

// �ϰ� ������ ����� ����� �� �մ��� �˻��Ѵ�.
// return 0 : ��� ����
// return 1 : ��� �Ұ�
INT CLocMailInfo::IsUseBatchSendMail( INT nMailClass )
{
	EFS_BEGIN

	CString sData;
	CString	strCheckMailClass;
	INT ids;	
	INT nCheckMailClass;
	INT	i;
	INT	nRowCount;
	
	nRowCount = m_pMailModeInfoDM->GetRowCount();

	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseBatchSendMail") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				ids = m_pMailModeInfoDM->GetCellData( _T("�ϰ��ڵ��������ɻ�뿩��") , i , sData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseBatchSendMail") );
				sData.TrimRight(); sData.TrimLeft();
				if( sData.Compare(_T("Y")) == 0 ) 
				{
					return 0;
				}
			}
		}
	}	

	return 1;

	EFS_END
	return -1;

}

// �ϰ� ������ ����� ���� ���� ����
INT CLocMailInfo::BatchMakeMailInfo( INT nMailClass )
{
	EFS_BEGIN

	INT ids;

	CString sWhere;

	ids = MakeBatchWhereSql( nMailClass , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("BatchMakeMailInfo") );

	// ���� ���� DM ����
	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

	// ���� �ڷ�
	ids = pLocDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeMailInfo") );

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pLocDM->GetRowCount() && _T("DM_LOC_MAIL_INFO") == pLocDM->GETDMAlias())
	{
		CString strEnc;
		CStringArray asDes1;
		for(INT i = 0; i < pLocDM->GetRowCount(); i++)
		{
			pLocDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes1.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pLocDM->GetRowCount(); i++)
				{
					pLocDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	// û����ȣ ����
	if( nMailClass != 8 )		
		CLocCommonAPI::MakeShelfCode( this , pLocDM->DB_MGR_ALIAS );	

	// ��� ���� �ڷ� + ���� �ڷ���ϱ�
	ids = AddNonDBBookDM( nMailClass , sWhere , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeMailInfo") );


	ids = MakeMailRecord( nMailClass , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeMailInfo") );

	return 0;

	EFS_END
	return -1;

}

// �ϰ� ������ ��� ���
INT CLocMailInfo::MakeBatchWhereSql( INT nMailClass , CString &sWhere )
{
	EFS_BEGIN

	INT ids;


	CString sBatchValue;
	CString sBatchWhere;
	CString sReSendYN;

	// �⺻ WHERE SQL�� ��������
	ids = MakeSql( nMailClass , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeBatchWhereSql") );

	CString	strCheckMailClass;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowCount = m_pMailModeInfoDM->GetRowCount();

	nRowIndex = 0;
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				break;
			}
		}
	}	
	
	ids = m_pMailModeInfoDM->GetCellData( _T("���ǰ�") , nRowIndex , sBatchValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeBatchWhereSql") );

	ids = m_pMailModeInfoDM->GetCellData( _T("��߼ۿ���") , nRowIndex , sReSendYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeBatchWhereSql") );	

	CString sLibInfo[1] =
	{
		_T("MANAGE_CODE")
	};

	CString	m_sLibName;
	CLocCommonAPI::GetLibInfo( sLibInfo , sLibInfo , 1 , this);
	m_sLibName = sLibInfo[0];

	switch( nMailClass )
	{
	
	case 0:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.RESERVATION_DATE)>%s ") , sBatchValue );
		sBatchWhere.Format( _T(" CL.RESERVATION_DATE < TO_CHAR( SYSDATE - (%d+1) , 'YYYYMMDD' ) ") , _ttoi(sBatchValue) );
		break;

	case 1:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.RESERVATION_DATE)>%s ") , sBatchValue );
		sBatchWhere.Format( _T(" CL.RESERVATION_DATE < TO_CHAR( SYSDATE - (%d+1) , 'YYYYMMDD' ) ") , _ttoi(sBatchValue) );
		break;

	case 2:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.RETURN_PLAN_DATE)>%s ") , sBatchValue );
		// 2005.02.28 UPDATE BY PDJ
		// ���� �߸���
		sBatchWhere.Format( _T(" CL.RETURN_PLAN_DATE < TO_CHAR( SYSDATE - (%d-1) , 'YYYYMMDD' ) ") , _ttoi(sBatchValue) );
		//sBatchWhere.Format( _T(" CL.RETURN_PLAN_DATE > TO_CHAR( SYSDATE + %d , 'YYYYMMDD' ) ") , _ttoi(sBatchValue) );
		break;

	case 3:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.LOAN_DATE)<%s ") , sBatchValue );
		sBatchWhere.Format( _T(" CL.LOAN_DATE BETWEEN TO_CHAR( SYSDATE - %d, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE , 'YYYYMMDD') ") , _ttoi(sBatchValue) );
		break;

	case 4:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.RETURN_DATE)<%s ") , sBatchValue );
		//sBatchWhere.Format( _T(" CL.RETURN_DATE BETWEEN TO_CHAR( SYSDATE, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE + %d, 'YYYYMMDD') ") , _ttoi(sBatchValue)-1 );
		sBatchWhere.Format( _T(" CL.RETURN_DATE BETWEEN TO_CHAR( SYSDATE - %d, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE, 'YYYYMMDD') ") , _ttoi(sBatchValue) );
		break;

	case 5:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.RETURN_DATE)<%s ") , sBatchValue );
		//sBatchWhere.Format( _T(" CL.RETURN_DATE BETWEEN TO_CHAR( SYSDATE, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE + %d, 'YYYYMMDD') ") , _ttoi(sBatchValue)-1 );
		sBatchWhere.Format( _T(" CL.RETURN_DATE BETWEEN TO_CHAR( SYSDATE - %d, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE, 'YYYYMMDD') ") , _ttoi(sBatchValue) );
		break;

	case 6:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.RESERVE_CANCEL_DATE)<%s ") , sBatchValue );
		//sBatchWhere.Format( _T(" CL.RESERVE_CANCEL_DATE BETWEEN TO_CHAR( SYSDATE, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE + %d, 'YYYYMMDD') ") , _ttoi(sBatchValue)-1 );
		sBatchWhere.Format( _T(" CL.RESERVE_CANCEL_DATE BETWEEN TO_CHAR( SYSDATE-%d, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE, 'YYYYMMDD') ") , _ttoi(sBatchValue) );
		break;

	case 7:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.DELAY_RETURN_DATE)<%s ") , sBatchValue );
		//sBatchWhere.Format( _T(" CL.DELAY_RETURN_DATE BETWEEN TO_CHAR( SYSDATE, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE + %d, 'YYYYMMDD') ") , _ttoi(sBatchValue)-1 );
		sBatchWhere.Format( _T(" CL.DELAY_RETURN_DATE BETWEEN TO_CHAR( SYSDATE-%d, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE, 'YYYYMMDD') ") , _ttoi(sBatchValue) );
		break;

	case 9:
		{
			if ( m_sLibName.IsEmpty() )
			{
				sBatchWhere.Format( _T(" CL.RETURN_PLAN_DATE BETWEEN TO_CHAR( SYSDATE, 'YYYYMMDD' ) AND ECO$F_G_ISHOLIDAY$I(TO_CHAR (SYSDATE + %d, 'YYYYMMDD')) ") , _ttoi(sBatchValue) );
			}
			else
			{
				//�ݳ����� �߰�
				//sBatchWhere.Format( _T(" (CL.RETURN_PLAN_DATE-SYSDATE)<=%s ") , sBatchValue );		
				sBatchWhere.Format( _T(" CL.RETURN_PLAN_DATE BETWEEN TO_CHAR( SYSDATE, 'YYYYMMDD' ) AND ECO$F_G_ISHOLIDAY$I(TO_CHAR (SYSDATE + %d, 'YYYYMMDD'),'%s') ") , _ttoi(sBatchValue), m_sLibName );
			}		
		}	
		break;
	default:
		break;

	}

	if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

	sWhere += _T("(") + sBatchWhere + _T(")") ;

	if( sReSendYN.Compare(_T("N")) == 0 )
	{
		CString sBatchWhere2;
		sBatchWhere2.Format(_T(" AND ((SELECT COUNT(*) FROM CO_LOAN_MAIL_SEND_TBL WHERE MAIL_CLASS = '%d' AND LOAN_KEY = CL.REC_KEY )=0)") 
			, nMailClass);
		sWhere += sBatchWhere2;
	}

	// 2005.03.22 ADD BY PDJ
	// 5163�� ��� �ϰ�������� ������ ��ȣ�� 00000~99999�� �ڷḸ �������� �Ѵ�.
	if(m_pSendMailMgr->Check_5163MailSystem() == TRUE)
	{
		sWhere += _T(" AND CLU.USER_NO BETWEEN '00000' AND '99999' AND LENGTH(CLU.USER_NO)=5 ");
	}	
	// 2007.08.08 add by pdw
	// �Ϲݵ������� ��� e-mail�� �ִ°��� �߼��Ѵ�.
	else
	{
		sWhere += _T(" AND CLU.E_MAIL IS NOT NULL ");
	}
	
	// ���� ���´� �ڷ�� ������ �ʴ´�.
	// 2007.08.09 update by pdw
	// ��߼ۿ��ΰ� Y�ϰ�츸 �����߰�
	if( sReSendYN.Compare(_T("Y")) == 0 )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		CString sBatchWhere3;
		sBatchWhere3.Format( _T(" ((SELECT COUNT(*) FROM CO_LOAN_MAIL_SEND_TBL ")
								_T("WHERE MAIL_CLASS='%d' AND LOAN_KEY=CL.REC_KEY AND to_char(MAIL_SEND_DATE,'YYYY/MM/DD')=to_char(SYSDATE,'YYYY/MM/DD'))=0) ") , nMailClass );

		sWhere += sBatchWhere3;
	}

	return 0;
	

	EFS_END
	return -1;

}

// ���� ������ �̸� ��������
CString CLocMailInfo::GetMailName( INT nMailClass )
{
	EFS_BEGIN

	CString sMailName;	
	CString	strCheckMailClass;
	INT ids;	
	INT nCheckMailClass;
	INT	i;
	INT	nRowCount;

	sMailName = _T("");
	nRowCount = m_pMailModeInfoDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				ids = m_pMailModeInfoDM->GetCellData( _T("�����̸�") , i , sMailName );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );
				break;
			}
		}
	}	

	return sMailName;

	EFS_END
	return _T("");

}

// ������ ������ ����
// ������ DM���� ���ڵ��� ������ ���ִ� �Լ�
INT CLocMailInfo::GetLogDMCnt( INT nMailClass , INT &nDMRCnt , INT &nSuccessDMRCnt , INT &nFailDMRCnt)
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetLogDMCnt") );

	nDMRCnt = pLocDM->GetRowCount();

	nSuccessDMRCnt = 0;
	nFailDMRCnt = 0;

	CString sYN;
	
	// 2005.03.08 ADD BY PDJ 
	// 5163�̸� ����ī��Ʈ�� ���� Groupping�� �Ǽ��� üũ
	CString	strLoanNo;
	CString	strPrevLoanNo;
	BOOL	b5163Flag;

	b5163Flag = m_pSendMailMgr->Check_5163MailSystem();	
	strLoanNo.Empty();
	strPrevLoanNo.Empty();

	for( INT i = 0 ; i < nDMRCnt ; i++ )
	{
		// 2005.03.08 ADD BY PDJ
		ids = pLocDM->GetCellData( _T("�����ڹ�ȣ") , i , strLoanNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetLogDMCnt") );
		if ( b5163Flag) 
		{
			if ( strLoanNo == strPrevLoanNo ) continue;
		}

		ids = pLocDM->GetCellData( _T("��������") , i , sYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetLogDMCnt") );
		sYN.TrimLeft(); sYN.TrimRight();

		if( sYN.Compare(_T("Y"))==0 )
			nSuccessDMRCnt++;
		else
			nFailDMRCnt++;

		// 2005.03.08 ADD BY PDJ
		strPrevLoanNo = strLoanNo;
	}

	// 2005.03.08 ADD BY PDJ
	// ��ü ī��Ʈ�� ����+����
	if ( b5163Flag) 
	{
		nDMRCnt = nSuccessDMRCnt+nFailDMRCnt;
	}

	return 0;

	EFS_END
	return -1;

}

// ���� �α׸� pDstDM�� ���
INT CLocMailInfo::AddFailLog( INT nMailClass , CESL_DataMgr *pDstDM )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetLogDMCnt") );

	const INT nAliasCnt = 4;
	TCHAR *sDMFieldAlias[nAliasCnt] =
	{
		_T("�̿��ڸ�") , _T("����") , _T("��Ϲ�ȣ") , _T("���л���") 
	};

	CString sYN;
	CString sData;
	for( INT i = 0 ; i < pLocDM->GetRowCount() ; i++ )
	{
		ids = pLocDM->GetCellData( _T("��������") , i , sYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("AddFailLog") );
		sYN.TrimLeft(); sYN.TrimRight();

		if( sYN.Compare(_T("Y"))==0 ) continue;

		pDstDM->InsertRow(-1);

		for( INT j = 0 ; j < nAliasCnt ; j++ )
		{
			ids = pLocDM->GetCellData( sDMFieldAlias[j] , i , sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("AddFailLog") );
			sData.TrimLeft(); sData.TrimRight();

			ids = pDstDM->SetCellData( sDMFieldAlias[j] , sData , pDstDM->GetRowCount()-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("AddFailLog") );
		}

		ids = pDstDM->SetCellData( _T("��������") , GetMailName(nMailClass) , pDstDM->GetRowCount()-1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("AddFailLog") ); 
	}

	return 0;

	EFS_END
	return -1;

}



INT CLocMailInfo::GetMailClassFromName( CString sMailName )
{
	EFS_BEGIN

	INT ids;

	CString sTmpMailName;
	CString strMailClass;

	for( INT i = 0 ; i < m_pMailModeInfoDM->GetRowCount() ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("�����̸�") , i , sTmpMailName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailClassFromName") );

		if( sMailName.Compare( sTmpMailName ) == 0 )
		{
			ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , strMailClass );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailClassFromName") );
			return _ttoi(strMailClass);
		}
	}

	return 100;

	EFS_END
	return -1;

}

INT CLocMailInfo::MakeMailInfo( INT nMailClass , CESL_DataMgr *pSrcDM , CESL_Grid *pGrid )
{
	EFS_BEGIN

	INT ids;

	// ���� ���� DM ����
	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

	pLocDM->FreeData();

	ids = MakeLocDM( nMailClass , pSrcDM , pGrid );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );
	if( ids > 0 ) return ids;

	ids = MakeMailRecord( nMailClass , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeMailInfo") );

	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::MakeLocDM( INT nMailClass , CESL_DataMgr *pSrcDM , CESL_Grid *pGrid )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeMailInfo") );

	pGrid->SelectMakeList();

	INT nIndex;
	INT nSelectGetCount = pGrid->SelectGetCount();
	pGrid->SelectGetHeadPosition();

	CString sRecKeyList;
	CString sRecKey;
	CString sWhere;
	CString sMailingUseYN;
	for( INT i = 0 ; i < nSelectGetCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		ids = pSrcDM->GetCellData( _T("���ϼ��ſ���") , nIndex , sMailingUseYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

		if ( sMailingUseYN == _T("N") )
		{
			pGrid->SelectGetNext();
			continue;
		}
		
		ids = pSrcDM->GetCellData( _T("����/�ݳ�����KEY") , nIndex , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

		if( i == 0 || ( i > 300 && ((i)%300)==1 ) ) 
			sRecKeyList = sRecKey ;
		else 
		{
			if( sRecKeyList.IsEmpty() )
			{
				sRecKeyList = sRecKey ;
			}
			else
			{
				sRecKeyList += _T(",") + sRecKey;
			}
		}

		// 2007.04.11 update by pdw {{+++++++++++++++++++++++++++++++++++++++++++++
		// �̿����� �������� ���ϸ� �������̶�� �̰� ó������ �ʱ� ������ ����		
		/*
		if( ((i!=0)&&(i%300==0)) || (i==nSelectGetCount-1) )
		{
			sWhere.Format( _T(" CL.REC_KEY IN ( %s ) ") , sRecKeyList );

			// TMP DM�� ������ �� LCO DM�� ADD�Ѵ�.
			ids = MakeLocDMAddTmpDM(nMailClass , sWhere);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailInfo") );

			ids = AddNonDBBookDM( nMailClass , sWhere , pLocDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeMailInfo") );			
		}
		*/
		if( (i!=0) && (i%300==0) )
		{
			sWhere.Format( _T(" CL.REC_KEY IN ( %s ) ") , sRecKeyList );

			// TMP DM�� ������ �� LCO DM�� ADD�Ѵ�.
			ids = MakeLocDMAddTmpDM(nMailClass , sWhere);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailInfo") );

			ids = AddNonDBBookDM( nMailClass , sWhere , pLocDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeMailInfo") );	
			
			sRecKeyList.Empty();
		}		
		// ----------------------------------------------------------------------}}

		pGrid->SelectGetNext();
	}

	if( !sRecKeyList.IsEmpty() )
	{
		sWhere.Format( _T(" CL.REC_KEY IN ( %s ) ") , sRecKeyList );

		// TMP DM�� ������ �� LCO DM�� ADD�Ѵ�.
		ids = MakeLocDMAddTmpDM(nMailClass , sWhere);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailInfo") );

		ids = AddNonDBBookDM( nMailClass , sWhere , pLocDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeMailInfo") );			
	}
	
	if( pLocDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("�����Ͻ� �ڷ��߿� ������ ������ �ִ� �ڷᰡ �������� �ʽ��ϴ�.") );
		return 1;
	}

	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::MakeLocDMAddTmpDM( INT nMailClass , CString sWhere )
{
	EFS_BEGIN

	INT ids;

	ids = MakeSql( nMailClass , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeLocDMAddTmpDM") );

	CESL_DataMgr *pTmpLocDM = FindTempUseLocDM( nMailClass );
	if( pTmpLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeLocDMAddTmpDM") );

	ids = pTmpLocDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeLocDMAddTmpDM") );

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pTmpLocDM->GetRowCount())
	{
		CString strEnc;
		CStringArray asDes1;
		for(INT i = 0; i < pTmpLocDM->GetRowCount(); i++)
		{
			pTmpLocDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes1.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pTmpLocDM->GetRowCount(); i++)
				{
					pTmpLocDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

	ids = CLocCommonAPI::AddDM( pTmpLocDM , pLocDM , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeLocDMAddTmpDM") );

	// û����ȣ ����
	if( nMailClass != 8 )		
		CLocCommonAPI::MakeShelfCode( this , pLocDM->DB_MGR_ALIAS );

	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::IsUseMailClass( INT nMailClass )
{
	EFS_BEGIN

	INT ids;

	CString	strCheckMailClass;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowIndex = -1;
	nRowCount = m_pMailModeInfoDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				break;
			}
		}
	}

	if ( nRowIndex == -1 )
	{
		return 1;
	}
	else
	{
		CString sYN;
		ids = m_pMailModeInfoDM->GetCellData( _T("��ɻ�뿩��") , nRowIndex , sYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseMailClass") );

		if( sYN.Compare( _T("N") )== 0 )
			return 1;
	}

	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::DMAndGridSetting( INT nMailClass , CESL_DataMgr *pDM , CESL_Grid *pGrid )
{
	EFS_BEGIN

	INT ids;

	if( nMailClass > 1 ) return 0;

	CString sLoanKey;

	INT nSelectCnt = pGrid->SelectGetCount();

	pGrid->SelectGetHeadPosition();

	CString sToday;
	
	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	sToday = (CLocCommonAPI::GetCurrentDateTime(this,strGetData)).Format( _T("%Y/%m/%d") );
// 	sToday = (CLocCommonAPI::GetDBTime(this)).Format( _T("%Y/%m/%d") );

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("����/�ݳ�����KEY") , nIndex , sLoanKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DMAndGridSetting") );

		ids = GetSuccessYN( nMailClass , sLoanKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DMAndGridSetting") );

		if( ids > 0 ) continue;



		// ����
		// DM UPDATE
		ids = pDM->SetCellData( GetMailDMAlias(nMailClass), sToday , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DMAndGridSetting") );

		// �׸��� ������Ʈ
		ids = CLocCommonAPI::InsertGridRow( this , pGrid , nIndex , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DMAndGridSetting") );
	}

	return 0;

	EFS_END
	return -1;

}

CString CLocMailInfo::GetMailDMAlias( INT nMailClass )
{
	EFS_BEGIN

	const INT nCnt = 8;
	CString sMailDMFieldAlias[nCnt] =
	{
		_T("����ȳ�(�ݳ���)����_�߼���") , _T("����ȳ�(��ġ�ڷΌ��)����_�߼���") , _T("�ݳ����˸���_�߼���") , 
		_T("����ȳ�����_�߼���") , _T("�ݳ��ȳ�(��ü�ȵ��ڷ�)����_�߼���") , _T("�ݳ�Ȯ��(��ü���ڷ�)����_�߼���") , 
		_T("������Ҹ���_�߼���") , _T("�ݳ��������_�߼���")
	};

	return sMailDMFieldAlias[nMailClass];

	EFS_END
	return _T("");

}

INT CLocMailInfo::GetSuccessYN( INT nMailClass , CString sLoanKey )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

	CString sTmpLoanKey;
	CString sYN;
	for( INT i = 0 ; i < pLocDM->GetRowCount() ; i++ )
	{
		ids = pLocDM->GetCellData( _T("����/�ݳ�����KEY") , i , sTmpLoanKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSuccessYN") );

		if( sLoanKey.Compare(sTmpLoanKey)!=0 ) continue;

		ids = pLocDM->GetCellData( _T("��������") , i , sYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetSuccessYN") );

		if( sYN.Compare(_T("Y"))==0 ) return 0;
	}

	return 1;

	EFS_END
	return -1;

}

/*
EFS_BEGIN

	INT ids;

	CString	strCheckMailClass;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowIndex = -1;
	nRowCount = m_pMailModeInfoDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				break;
			}
		}
	}

	if ( nRowIndex == -1 )
	{
		return 1;
	}
	else
	{
		CString sYN;
		ids = m_pMailModeInfoDM->GetCellData( _T("��ɻ�뿩��") , nRowIndex , sYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseMailClass") );

		if( sYN.Compare( _T("N") )== 0 )
			return 1;
	}

	return 0;

	EFS_END
	return -1;
*/

CString CLocMailInfo::GetMailType(INT nMailClass)
{
	EFS_BEGIN

	INT ids;

	CString	strCheckMailClass;
	CString strMailType;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowIndex = -1;
	nRowCount = m_pMailModeInfoDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("��������") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				break;
			}
		}
	}

	if ( nRowIndex == -1 )
	{
		return _T("");
	}
	else
	{		
		ids = m_pMailModeInfoDM->GetCellData( _T("����Ÿ��") , nRowIndex , strMailType );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseMailClass") );
		return strMailType;
	}

	return _T("");

	EFS_END
	return _T("");
}