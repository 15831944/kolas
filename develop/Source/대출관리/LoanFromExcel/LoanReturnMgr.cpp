// LoanReturnMgr.cpp : implementation file
//

#include "stdafx.h"
#include "LoanReturnMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanReturnMgr dialog


CLoanReturnMgr::CLoanReturnMgr(CESL_Mgr* pParent , CString sProcDate , CString sLoanPeriod )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoanReturnMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_sProcDate = sProcDate;

	CTimeSpan tsLoanPeriod(_ttoi(sLoanPeriod) , 0 , 0 , 0 );
	CTime tLoanDate( _ttoi(sProcDate.Mid(0,4)) , _ttoi(sProcDate.Mid(5,2)) , _ttoi(sProcDate.Mid(8,2)) , 0 , 0 , 0  );
	CTime tReturnPlanDate = tLoanDate + tsLoanPeriod;
	m_sReturnPlanDate = tReturnPlanDate.Format(_T("%Y/%m/%d"));
}

CLoanReturnMgr::~CLoanReturnMgr()
{
	delete m_pVirtualInfo;
}

VOID CLoanReturnMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanReturnMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoanReturnMgr, CDialog)
	//{{AFX_MSG_MAP(CLoanReturnMgr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanReturnMgr message handlers

INT CLoanReturnMgr::LoanBook( CString sUserNo , CString sBookRegNo , CString &sErrorMsg )
{
	INT ids;

	/// 1. �̿��� ���� �ε�
	ids = LoadUserInfo( sUserNo );	
	if( ids != 0 )
	{
		sErrorMsg = _T("�̿��� ������ �������� �ʽ��ϴ�.");
		return ids;
	}


	/// 2. å���� �ε�
	ids = LoadBookInfo( sBookRegNo );	
	if( ids != 0 )
	{
		sErrorMsg = _T("å������ �������� �ʽ��ϴ�.");
		return ids;
	}
	
	/// 3. ����
	ids = LoanDBProc();
	if( ids != 0 )
	{
		sErrorMsg = _T("�˼� ���� ������ DB�۾��� �����Ͽ����ϴ�.");
		return ids;
	}
	

/*
	CString sMsg;
	sMsg.Format( _T("���� : %s , %s ") , sUserNo , sBookRegNo );
	AfxMessageBox(sMsg);
*/
	return 0;
}

INT CLoanReturnMgr::ReturnBook(CString sBookRegNo , CString &sErrorMsg )
{
	INT ids;

	/// 1. ����ݳ����� �ε�
	ids = LoadLoanInfo( sBookRegNo );	
	if( ids != 0 )
	{
		sErrorMsg = _T("����/�ݳ� ������ �������� �ʽ��ϴ�.");
		return ids;
	}
	
		
	/// 3. �ݳ�
	ids = ReturnDBProc();
	if( ids != 0 )
	{
		sErrorMsg = _T("�˼� ���� ������ DB�۾��� �����Ͽ����ϴ�.");
		return ids;
	}
	
/*
	CString sMsg;
	sMsg.Format( _T("�ݳ� : %s  ") , sBookRegNo );
	AfxMessageBox(sMsg);
*/
	return 0;
}

INT CLoanReturnMgr::LoadUserInfo( CString sUserNo )
{
	INT ids;
	
	CESL_DataMgr* pUserDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if( pUserDM == NULL ) return 1;

	CString sWhere;
	sWhere.Format( _T("CLU.USER_NO = '%s' ") , sUserNo );

	ids = pUserDM->RefreshDataManager(sWhere);
	if( ids < 0 ) return 2;

	if( pUserDM->GetRowCount() < 1 ) 
		return 3;

	return 0;
}


INT CLoanReturnMgr::LoadBookInfo( CString sBookRegNo )
{
	INT ids;

	CESL_DataMgr* pBookDM = FindDM(_T("DM_EXP_LOAN_BO_BOOK_INFO"));
	if( pBookDM == NULL ) return 1;
	
	CString sWhere;
	sWhere.Format( _T("BB.REG_NO = '%s' ") , sBookRegNo );
	
	ids = pBookDM->RefreshDataManager(sWhere);
	if( ids < 0 ) return 2;
	
	if( pBookDM->GetRowCount() < 1 ) 
		return 3;
	
	return 0;
}


INT CLoanReturnMgr::LoanDBProc()
{
	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_EXP_LOAN_BO_BOOK_INFO") );
	if( pDM == NULL ) return 1;


	pDM->InitDBFieldData();
	pDM->StartFrame();

	// 1. å���� DM���� ����
	CString sSrcDMAlias = _T("DM_EXP_LOAN_BO_BOOK_INFO");
	const INT nSrcFieldCnt = 14;
	TCHAR *sSrcFieldAlias[nSrcFieldCnt] = 
	{
		_T("åKEY") , _T("�ڷ�Ǳ���") , _T("����/���ӱ���") , _T("�з���ȣ����") , _T("�з���ȣ") , 
		_T("��������") , _T("��ϱ���") , _T("������Ű") , _T("��Ϲ�ȣ") , _T("��ġ��ȣ") , 
		_T("��ǥ��") , _T("������") , _T("������") , _T("û����ȣ")
	};
	TCHAR *sDstFieldAlias[nSrcFieldCnt] = 
	{
		_T("BOOK_KEY") , _T("SHELF_LOC_CODE") , _T("PUBLISH_FORM_CODE") , _T("CLASS_NO_TYPE") , _T("CLASS_NO") , 
		_T("MANAGE_CODE") , _T("REG_CODE") , _T("SPECIES_KEY")  , _T("REG_NO") , _T("SEPARATE_SHELF_CODE") , 
		_T("TITLE") , _T("AUTHOR") , _T("PUBLISHER") , _T("CALL_NO")
	};
	TCHAR *sDstFieldTypeAlias[nSrcFieldCnt] = 
	{
		_T("NUMERIC") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , 
		_T("STRING") , _T("STRING") , _T("NUMERIC") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING")  , _T("STRING") , _T("STRING")
	};

	CString sGetData;
	for( INT i= 0 ; i < nSrcFieldCnt ;i++ )
	{
		ids = GetDataMgrData( sSrcDMAlias , sSrcFieldAlias[i] , sGetData , 0 );
		if( ids < 0 ) return 26;

		if( CString(_T("NUMERIC")).Compare(sDstFieldTypeAlias[i])==0 )
			if( sGetData.IsEmpty() ) sGetData = _T("NULL");

		if( i == 2 ) sGetData = _T("MO");

		ids = pDM->AddDBFieldData( sDstFieldAlias[i] , sDstFieldTypeAlias[i] , sGetData , TRUE );
		if( ids < 0 ) return 25;

	}

	// 2. USER�������� ����
	CString sSrcUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	const INT nSrcUserFieldCnt = 4;
	TCHAR *sSrcUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("������KEY") , _T("�����ڼҼ�") , _T("����������") , _T("ȸ������") 
	};
	TCHAR *sDstUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("USER_KEY") , _T("USER_POSITION_CODE") , _T("USER_CLASS_CODE") , _T("MEMBER_CLASS") 
	};
	TCHAR *sDstUserFieldTypeAlias[nSrcUserFieldCnt] = 
	{
		_T("NUMERIC") , _T("STRING") , _T("STRING") , _T("STRING") 
	};


	for( i= 0 ; i < nSrcUserFieldCnt ;i++ )
	{
		ids = GetDataMgrData( sSrcUserDMAlias , sSrcUserFieldAlias[i] , sGetData , 0 );
		if( ids < 0 ) return 22;

		ids = pDM->AddDBFieldData( sDstUserFieldAlias[i] , sDstUserFieldTypeAlias[i] , sGetData , TRUE );
		if( ids < 0 ) return 2;

	}

	// 3. ��Ʈ�ѿ��� ����
	// ���� ���� �ڵ� : 0->�Ϲ� , 1->Ư�� , 2->����
	CString sLoanType = _T("0");
	ids = pDM->AddDBFieldData( _T("LOAN_TYPE_CODE") , _T("STRING") , sLoanType , TRUE );
	if( ids < 0 ) return 3;

	// 4. Make RecKey
	CString sRecKey;
	ids = pDM->MakeRecKey( sRecKey );
	if( ids < 0 ) return 4;


	// �ݳ� ������ 
	ids = pDM->AddDBFieldData( _T("RETURN_PLAN_DATE") , _T("STRING") , m_sReturnPlanDate , TRUE);
	if( ids < 0 ) return 5;


	// 6. �������� �ԷµǴ� �ʵ�
	// ���� ���� : 0 -> ���� 
	// ������ : ������
	CString sLoanStatus = _T("0");
	ids = pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , sLoanStatus , TRUE );
	if( ids < 0 ) return 6;

	ids = pDM->AddDBFieldData( _T("LOAN_DATE") , _T("STRING") , m_sProcDate , TRUE);
	if( ids < 0 ) return 7;
	
	// 6.2 �η� �ڷ��� ��� �η��� å���� �����Խ� LOAN_KEY �� 1�� �Է�
	CString sIsAppendix;
	ids = GetDataMgrData( _T("DM_EXP_LOAN_BO_BOOK_INFO") , _T("�ηϿ���") , sIsAppendix , 0 );
	if( ids < 0 ) return 8;



	if( sIsAppendix.Compare( _T("A") ) == 0 )
	{
		if( IsAppendixPlus() )
			pDM->AddDBFieldData( _T("LOAN_KEY") , _T("NUMERIC") , _T("1") );
	}
	
	// 7.0 �α� �����
	CString sLog;
	sLog.Format( _T("%s") , GetWorkLogMsg( _T("Excel�δ���"), __WFILE__, __LINE__ ) );
	pDM->AddDBFieldData( _T("FIRST_WORK") , _T("STRING") , sLog , TRUE );
	
	// 7. SQL�� ����
	
	pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	ids = pDM->MakeInsertFrame( _T("LS_WORK_T01") );
	if( ids < 0 ) return 9;

	pDM->InitDBFieldData();

	
	pDM->InitDBFieldData();
	CString sBookRecKey;
			
	// 2. REC_KEY ����	
	ids = pDM->GetCellData( _T("åKEY") , 0 , sBookRecKey );
	if( ids < 0 ) return 11;
	// 3. å���¸� ����
	ids = pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("BOL211O") , TRUE);
	if( ids < 0 ) return 12;
	
	ids = pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , _T("BOL112N") , TRUE);
	if( ids < 0 ) return 13;

	// �α� �����
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("Excel�δ���"), __WFILE__, __LINE__ ), TRUE );
	
	// 4. UPDATE SQL ����
	ids = pDM->MakeUpdateFrame( _T("BO_BOOK_TBL"), _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	if( ids < 0 ) return 14;
	
	pDM->InitDBFieldData();

	ids = pDM->SendFrame();
	if( ids < 0 ) return 15;

	pDM->EndFrame();
	

	return 0;
}

BOOL CLoanReturnMgr::IsAppendixPlus()
{
	return m_bAppendixPlus;
}


INT CLoanReturnMgr::ReturnDBProc()
{
	
	INT ids;
	
	CESL_DataMgr *pUpdateDM = FindDM( _T("DM_BO_LOC_3100_BO_LOAN_INFO") );
	if( pUpdateDM == NULL ) return 1;
	
	pUpdateDM->StartFrame();
	pUpdateDM->InitDBFieldData();
	
	// ���� ����( 1: �ݳ� ) , �ݳ�����( 0: �Ϲ� , 1: �ϰ� , 2:�ұ� ), ���� , ��ü�� , �ݳ���(������)	
	// �������
	ids = pUpdateDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("1") , TRUE );
	if( ids < 0 ) return 2;
	
	// �ݳ�����
	ids = pUpdateDM->AddDBFieldData( _T("RETURN_TYPE_CODE") , _T("STRING") , _T("0") , TRUE );
	if( ids < 0 ) return 3;
	
	ids = pUpdateDM->AddDBFieldData( _T("RETURN_DATE") , _T("STRING") , m_sProcDate , TRUE );
	if( ids < 0 ) return 4;
	
	// REC_KEY 
	CString sRecKey;
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_BO_LOAN_INFO") , _T("CL_����/�ݳ�����KEY") , sRecKey , 0 );
	if( ids < 0 ) return 5;
	
	// �α� �����
	pUpdateDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("Excel�� �ݳ�"), __WFILE__, __LINE__ ), TRUE );
	
	// UPDATE SQL����
	
	ids = pUpdateDM->MakeUpdateFrame( _T("LS_WORK_T01") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) return 6;
	
	pUpdateDM->InitDBFieldData();

	
	pUpdateDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , _T("BOL211O") , TRUE );

	ids = pUpdateDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("BOL112N") , TRUE );
	if( ids < 0 ) return 7;

	// REC_KEY
	// �α� �����
	pUpdateDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("Excel�ιݳ�"), __WFILE__, __LINE__ ), TRUE );

	CString sBookRecKey;
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_BO_LOAN_INFO") , _T("BB_åKEY") , sBookRecKey , 0 );
	ids = pUpdateDM->MakeUpdateFrame( _T("BO_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	
	pUpdateDM->InitDBFieldData();

	ids = pUpdateDM->SendFrame();
	if( ids < 0 ) return 15;

	pUpdateDM->EndFrame();

	return 0;
	
}

INT CLoanReturnMgr::LoadLoanInfo( CString sBookRegNo )
{
	INT ids;

	CESL_DataMgr *pLoanInfoDM = FindDM( _T("DM_BO_LOC_3100_BO_LOAN_INFO") );
	if( pLoanInfoDM == NULL ) return 1;

	CString sWhere;
	sWhere.Format( _T("BB.REG_NO='%s' AND CL.STATUS = '0' ") , sBookRegNo );

	ids = pLoanInfoDM->RefreshDataManager( sWhere );
	if( ids < 0 ) return 2;

	if( pLoanInfoDM->GetRowCount() < 1 ) return 3;

	return 0;
	
}

INT CLoanReturnMgr::InitInfo()
{
	m_pVirtualInfo = new CESL_Information();
	m_pVirtualInfo->pMarcMgr = NULL;

	m_pInfo = m_pVirtualInfo;

	INT ids = InitESL_Mgr(_T("SM_LOAN_FROM_EXCEL"));
	if( ids < 0 )
	{
		AfxMessageBox(_T("SM Init Eror[SM_LOAN_FROM_EXCEL]"));

		return -1;
	}
	
	m_bAppendixPlus = FALSE;
	
	/// �η��� +�ϴ��� �˾ƺ���
	
	CString sAPlus;
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
	pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE VALUE_NAME='�ηϴ���Ǽ����Կ���' ") , sAPlus );
	if( sAPlus.Compare(_T("Y")) == 0 ) m_bAppendixPlus = TRUE;
	else  m_bAppendixPlus = FALSE;
	
	return 0;
}