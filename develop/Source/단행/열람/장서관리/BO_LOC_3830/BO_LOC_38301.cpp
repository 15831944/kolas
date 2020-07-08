// BO_LOC_38301.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_38301.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3830 dialog


CBO_LOC_3830::CBO_LOC_3830(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3830)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CBO_LOC_3830::~CBO_LOC_3830()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}
VOID CBO_LOC_3830::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3830)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3830, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3830)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3830 message handlers

BOOL CBO_LOC_3830::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr(_T("BO_LOC_3830"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	/// �������� ���� ����
	ids = InitMailClassInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	/// �׸��� �ݺ� �Լ� ����
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3830"),_T("���α׸���"));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	

	

	// 2006-01-23 UPDATE BY PDW(Jr) ======================================================
	// �ӽ� ������ �����ͷδ� ����� �����ų�� ����. 
	// �������� ���ϱ�
	/// ������ �̸� ��������
	CString sLibInfo[1] =
	{
		_T("LIB_NAME")
	};
	/*
	CLocCommonAPI::GetLibInfo( sLibInfo , sLibInfo , 1 , this);
	m_sLibName = sLibInfo[0];
	*/
	CLocCommonAPI::GetLibInfo( sLibInfo , &m_sLibName , 1 , this);
	// ===================================================================================
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}



BOOL CBO_LOC_3830::Create(CWnd* pParentWnd) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_LOC_3830::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	

	if( GetSafeHwnd() == NULL ) return ;

	CWnd* pWnd = GetDlgItem(IDC_grid3830_Main);

	if( pWnd == NULL ) return;

	pWnd->MoveWindow( 0 , 0 , cx , cy );

EFS_END
}


INT CBO_LOC_3830::InitMailClassInfo()
{
EFS_BEGIN


	INT ids;

	CESL_DataMgr* pMailClassDM = FindDM(_T("DM_BO_LOC_3830_MAIL_CLASS"));
	if( pMailClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitMailClassInfo") );

	ids = pMailClassDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitMailClassInfo") );

	pMailClassDM->SORT( _T("�ڵ�") );

	return 0;



EFS_END
return -1;

}


INT CBO_LOC_3830::ConvertMailClassCodeToDesc(CString sCode,CString &sDesc)
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pMailClassDM = FindDM(_T("DM_BO_LOC_3830_MAIL_CLASS"));
	if( pMailClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ConvertMailClassCodeToDesc") );

	CString sTmpCode;
	for( INT i = 0 ; i < pMailClassDM->GetRowCount() ; i++ )
	{
		ids = pMailClassDM->GetCellData( _T("�ڵ�") , i , sTmpCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ConvertMailClassCodeToDesc") );

		if( sCode.Compare(sTmpCode) == 0 )
		{
			ids = pMailClassDM->GetCellData( _T("����") , i , sDesc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ConvertMailClassCodeToDesc") );
			
			return 0;
		}
	}


	return 1;

EFS_END
return -1;

}


INT CBO_LOC_3830::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN

	INT ids;

	if( strDMFieldAlias.Compare(_T("��������")) == 0 )
	{
		ids = ConvertMailClassCodeToDesc( str , str );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GridDisplayEventProc") );
	}
	else if( strDMFieldAlias.Compare(_T("�Ҽ�")) == 0 )
	{
		ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("����Ҽ�����") ,  str , str );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GridDisplayEventProc") );
	}
	else if( strDMFieldAlias.Compare(_T("����")) == 0 )
	{
		ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("������������") ,  str , str );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GridDisplayEventProc") );
	}
	else if( strDMFieldAlias.Compare(_T("�Ҽ��ڷ��")) == 0 )
	{
		ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�ڷ�Ǳ���") ,  str , str );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GridDisplayEventProc") );
	}
	else if( strDMFieldAlias.Compare(_T("���л���")) == 0 ) 
	{
		ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("���Ϲ߼۽����ڵ�") ,  str , str );
		if ( ids == -2004 ) return 0;
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("GridDisplayEventProc") );

		if( str.Compare(_T("�������")) == 0 ) str = _T("");
	}
	else if( strDMFieldAlias.Compare(_T("���ſ���")) == 0 ) 
	{
		if( str.Compare(_T("Y")) == 0 ) str = _T("Y");
		// 2009.10.21 UPDATE BY KSJ : ���ſ��� ���п��� �׸��忡 ǥ�� �ϵ��� ����
		//DEL else str = _T("");
		else str = _T("N");
	}
	else if( strDMFieldAlias.Compare(_T("�߼ۿ���")) == 0 ) 
	{
		if( str.Compare(_T("Y")) == 0 ) str = _T("Y");
		// 2009.10.21 UPDATE BY KSJ : �߼ۿ��� ���п��� �׸��忡 ǥ�� �ϵ��� ����
		//DEL else str = _T("");
		else str = _T("N");
	}
	else if( strDMFieldAlias.Compare(_T("����ó������")) == 0 ) 
	{
		if( _ttoi(str) == 0 )
			str = _T("");
		else
			str.Format( _T("��߼�Ƚ��(%d)") , _ttoi(str) );
	}

	return 0;

EFS_END
return -1;

}


	/// ���� ���
INT CBO_LOC_3830::DeleteHis()
{
EFS_BEGIN
	
	INT ids;
	
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3830") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DeleteHis") );

	if( pDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("�ڷḦ ���� �˻����ֽʽÿ�") );
		return 1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3830") , _T("���α׸���") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DeleteHis") );



	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("������ �ڷḦ �������ֽʽÿ�.") );
		return 2;
	}

	ids = MessageBox( _T("�����Ͻðڽ��ϱ�?") , _T("�̷� ����") , MB_YESNO | MB_ICONQUESTION );
	if( ids == IDNO ) return 3;

	pDM->StartFrame();

	CString sHisKey;
	CArray < INT , INT > aIIndexs;
	INT nSIndex;
	pGrid->SelectGetTailPosition();
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nSIndex = pGrid->SelectGetIdx();
	
		ids = pDM->GetCellData( _T("���Ϸα�KEY") , nSIndex , sHisKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DeleteHis") );
		
		aIIndexs.Add(nSIndex);

		/// DB �����
		ids = pDM->MakeDeleteFrame( _T("MN_MAIL_LOG_TBL") , _T("REC_KEY") , _T("NUMERIC") , sHisKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DeleteHis") );

		pGrid->SelectGetPrev();
	}

	ids = pDM->SendFrame();
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("DeleteHis") );

	pDM->EndFrame();

	/// DM�� ȭ�� ����
	for( i = 0 ; i < aIIndexs.GetSize() ; i++ )
	{
		ids = pDM->DeleteRow(aIIndexs.GetAt(i));
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("DeleteHis") );

		if( pDM->GetRowCount() == 0 )
			AllControlDisplay( _T("CM_BO_LOC_3830") );
		else
			pGrid->RemoveItem(aIIndexs.GetAt(i)+1);
	}

	ids = pGrid->DisplayNumber();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("DeleteHis") ); 

	return 0;

EFS_END
return -1;
}

	/// ��� ���
INT CBO_LOC_3830::PrintList()
{
EFS_BEGIN

	INT ids;


	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3830") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	if( pDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("�ڷḦ ���� �˻����ֽʽÿ�") );
		return 1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3830") , _T("���α׸���") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("PrintList") );



	INT nMode = 0;
	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt == 0 )
	{
		/// ��ü ���
		nMode = 1;
	}

	CESL_DataMgr PrintDM;
	ids = CLocCommonAPI::MakeCopyDM( this , pDM , &PrintDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("PrintList") ); 

	CString sNumber;
	if( nMode == 0 )
	{
		INT nSIndex;
		pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < nSelectCnt ; i++ )
		{
			nSIndex = pGrid->SelectGetIdx();

			PrintDM.InsertRow(-1);

			ids = CLocCommonAPI::AddDM( pDM , &PrintDM , nSIndex , i , this );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("PrintList") ); 
					
			pGrid->SelectGetNext();
		}

	}
	else if( nMode == 1 )
	{
		ids = CLocCommonAPI::AddDM( pDM , &PrintDM , this );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("PrintList") ); 
	}

	CString sMailClass;
	for( INT i = 0 ; i < PrintDM.GetRowCount() ; i++ )
	{
		sNumber.Format( _T("%d") , i+1 );
		ids = PrintDM.SetCellData( _T("��ȣ") , sNumber , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("PrintList") ); 

		ids = PrintDM.GetCellData( _T("��������") , i , sMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("PrintList") ); 

		ids = ConvertMailClassCodeToDesc( sMailClass , sMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("PrintList") );

		ids = PrintDM.SetCellData( _T("��������") , sMailClass , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("PrintList") ); 

	}

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( &PrintDM , _T("����Ҽ�����") , _T("�Ҽ�") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( &PrintDM , _T("������������") , _T("����") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( &PrintDM , _T("�ڷ�Ǳ���") , _T("�Ҽ��ڷ��") );

	// �Ϲ����� �ݺ��� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = NULL;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(PrintDM.CONNECTION_INFO);


	ids = pSIReportManager->GetSIReportFileFromDB(_T("���ϸ��ý���_�����̷�"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 1;
	}

	pSIReportManager->SetDataMgr(0, &PrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	

	return 0;

EFS_END
return -1;
}




INT CBO_LOC_3830::ReSendMail()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3830") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ReSendMail") );

	if( pDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("�ڷḦ ���� �˻����ֽʽÿ�") );
		return 1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3830") , _T("���α׸���") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ReSendMail") );



	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("��߼��� �ڷḦ �������ֽʽÿ�.") );
		return 2;
	}

	ids = MessageBox( _T("��߼��Ͻðڽ��ϱ�?") , _T("��߼�") , MB_YESNO | MB_ICONQUESTION );
	if( ids == IDNO ) return 3;

	pDM->StartFrame();

	CString sHisKey;
	CArray < INT , INT > aIIndexs;
	INT nSIndex;
	pGrid->SelectGetTailPosition();

	CString sMailInfoKey;
	CString sLoanKeyList;
	CString sUserKey;
	// 2006-01-23 REMOVE BY PDW(Jr) =========================================
	// �ʿ���� ���� 
	// CString m_sLibName;
	// ======================================================================
	CString sMailClass;
	CString sBatchWorkName;
	CString sBookKeyList;
	CString sFurnishKey;
	CString sReSendCnt;

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nSIndex = pGrid->SelectGetIdx();
	
		ids = pDM->GetCellData( _T("���Ϸα�KEY") , nSIndex , sHisKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ReSendMail") );

		ids = pDM->GetCellData( _T("����ȴ����̷�") , nSIndex , sLoanKeyList );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("ReSendMail") );
		
		ids = pDM->GetCellData( _T("�̿���KEY") , nSIndex , sUserKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("ReSendMail") );

		ids = pDM->GetCellData( _T("��������") , nSIndex , sMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("ReSendMail") );
		
		ids = pDM->GetCellData( _T("��ġ�۾���") , nSIndex , sBatchWorkName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("ReSendMail") );

		ids = pDM->GetCellData( _T("�����å����") , nSIndex , sBookKeyList );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("ReSendMail") );

		ids = pDM->GetCellData( _T("����ó������") , nSIndex , sReSendCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -26 , _T("ReSendMail") );

		aIIndexs.Add(nSIndex);

		if( sUserKey.IsEmpty() ) sUserKey = _T("NULL");
		if( sFurnishKey.IsEmpty() ) sFurnishKey = _T("NULL");

		ids = GetMailInfoKey( sMailClass , sMailInfoKey );
		if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -27 , _T("ReSendMail") ); 

		/// Temp�� INSERT�ϱ�
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , _T("ESL_SEQ.NextVal") );
		pDM->AddDBFieldData( _T("MAIL_KEY") , _T("NUMERIC") , sMailInfoKey );
		pDM->AddDBFieldData( _T("LOAN_KEY_LIST") , _T("STRING") , sLoanKeyList );
		pDM->AddDBFieldData( _T("BOOK_KEY_LIST") , _T("STRING") , sBookKeyList );
		pDM->AddDBFieldData( _T("USER_KEY") , _T("NUMERIC") , sUserKey );
		pDM->AddDBFieldData( _T("LIB_NAME") , _T("STRING") , m_sLibName );
		pDM->AddDBFieldData( _T("MAIL_CLASS") , _T("STRING") , sMailClass );
		pDM->AddDBFieldData( _T("BATCH_WORK_NAME") , _T("STRING") , sBatchWorkName );
		pDM->AddDBFieldData( _T("FURNISH_KEY") , _T("NUMERIC") , sFurnishKey );

		ids = pDM->MakeInsertFrame(_T("MN_MAIL_TEMP_TBL"));
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("ReSendMail") );

		pDM->InitDBFieldData();
		sReSendCnt.Format( _T("%d") , _ttoi(sReSendCnt)+1 );
		pDM->AddDBFieldData( _T("RESEND_CNT") , _T("NUMERIC") , sReSendCnt );
		ids = pDM->MakeUpdateFrame( _T("MN_MAIL_LOG_TBL") , _T("REC_KEY") , _T("NUMERIC") , sHisKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -32 , _T("ReSendMail") );

		pGrid->SelectGetPrev();
	}

	ids = pDM->SendFrame();
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ReSendMail") );

	pDM->EndFrame();

	/// DM�� ȭ�� ����
	for( i = 0 ; i < aIIndexs.GetSize() ; i++ )
	{
		ids = pDM->GetCellData( _T("����ó������") , aIIndexs.GetAt(i) , sReSendCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ReSendMail") );

		sReSendCnt.Format( _T("%d") , _ttoi(sReSendCnt)+1);

		ids = pDM->SetCellData( _T("����ó������") , sReSendCnt , aIIndexs.GetAt(i) );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ReSendMail") );

		pGrid->DisplayLine(aIIndexs.GetAt(i));
	}

	AfxMessageBox(_T("�߼� ��� ����� �����Ͽ����ϴ�."));

	return 0;

EFS_END

return -1;

}


INT CBO_LOC_3830::GetMailInfoKey( CString sMailClass , CString &sMailInfoKey )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pMailClassDM = FindDM(_T("DM_BO_LOC_3830_MAIL_CLASS"));
	if( pMailClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ConvertMailClassCodeToDesc") );

	CString sTmpCode;
	for( INT i = 0 ; i < pMailClassDM->GetRowCount() ; i++ )
	{
		ids = pMailClassDM->GetCellData( _T("�ڵ�") , i , sTmpCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ConvertMailClassCodeToDesc") );

		if( sMailClass.Compare(sTmpCode) == 0 )
		{
			ids = pMailClassDM->GetCellData( _T("��������KEY") , i , sMailInfoKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ConvertMailClassCodeToDesc") );
			
			return 0;
		}
	}


	return 1;

EFS_END
return -1;
}

HBRUSH CBO_LOC_3830::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
