// BO_LOC_38901.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_38901.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3890 dialog


CBO_LOC_3890::CBO_LOC_3890(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3890)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CBO_LOC_3890::~CBO_LOC_3890()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CBO_LOC_3890::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3890)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3890, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3890)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3890 message handlers

BOOL CBO_LOC_3890::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	INT ids = InitESL_Mgr( _T("BO_LOC_3890") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	/// �������� ���� ����
	ids = InitMailClassInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	/// �׸��� �ݺ� �Լ� ����
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3890"),_T("���α׸���"));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("OnInitDialog") );

	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_3890::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return ;

	CWnd* pWnd = GetDlgItem(IDC_grid3890_Main);

	if( pWnd == NULL ) return;

	pWnd->MoveWindow( 0 , 0 , cx , cy );
	
}



BOOL CBO_LOC_3890::Create(CWnd* pParentWnd) 
{


	return CDialog::Create(IDD, pParentWnd);


}



INT CBO_LOC_3890::InitMailClassInfo()
{



	INT ids;

	CESL_DataMgr* pMailClassDM = FindDM(_T("DM_BO_LOC_3830_MAIL_CLASS"));
	if( pMailClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitMailClassInfo") );

	ids = pMailClassDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitMailClassInfo") );

	pMailClassDM->SORT( _T("�ڵ�") );

	return 0;




}


INT CBO_LOC_3890::ConvertMailClassCodeToDesc(CString sCode,CString &sDesc)
{


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


}


INT CBO_LOC_3890::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	INT ids;

	if( strDMFieldAlias.Compare(_T("��������")) == 0 )
	{
		ids = ConvertMailClassCodeToDesc( str , str );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GridDisplayEventProc") );
	}
	else if( strDMFieldAlias.Compare(_T("����")) == 0 )
	{
		if( str.IsEmpty() || str.Compare(_T("N")) == 0 )
			str = _T("�߼۴����");
		else if( str.Compare(_T("OK")) == 0 )
			str = _T("�߼ۿϷ�");
		else if( str.Compare(_T("CANCEL")) == 0 )
			str = _T("��ҵ�");
		else if( str.Compare(_T("S")) == 0 )
			str = _T("�߼�����");
		else
			str = _T("�߼�ó����");
	}


	return 0;


}


/*
@Create
 - 2009.03.25 BY KSJ
@Description
 - �߼۴��(STATUS:N)-> �߼�����(STATUS:S)�� ����
@Parameter

@Return Value
 - 
 - 0, ��� : ����
 - ���� : ����
@Mini Spec
 1.
*/
INT CBO_LOC_3890::StopSendMail()
{

	INT ids;

	// �˻��ߴ��� Ȯ��
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3890") );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("StopSendMail") );
	}

	if( pDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("�˻� ���� ���ֽʽÿ�.") );
		return 1;
	}

	// �����Ȱ��� �����ϴ��� Ȯ��
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3890") , _T("���α׸���") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("StopSendMail") );
	}

	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("������ ������ �������ֽʽÿ�.") );
		return 2;
	}

	// �Ѱ� �Ѱ� ó���ؾ���
	// ������ ���� ���ư��� ������ ����
	// ������ �� �ִ� �������� �˾ƺ���.

	// ���������ϸ� ����

	// �̹� DB�� �������� �ʴ´ٸ� �߼۵����� ����

	pGrid->SelectGetTailPosition();
	INT nSIndex , i;

	INT nSuccesCnt = 0;
	INT nFailCnt = 0;

	CString sTempKey , sStatus;
	CString sSql;

	for( i=0 ; i<nSelectCnt ; i++ )
	{
		nSIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("����") , nSIndex , sStatus );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("StopSendMail") ); 
		}

		ids = pDM->GetCellData( _T("�߼۴����KEY") , nSIndex , sTempKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("StopSendMail") ); 
		}

		if( sStatus.IsEmpty() || ( sStatus.Compare(_T("N")) == 0 ) )
		{
			sSql.Format( _T("SELECT STATUS FROM MN_MAIL_TEMP_TBL WHERE REC_KEY = %s") , sTempKey );
			ids = pDM->GetOneValueQuery( sSql , sStatus );
			if( ids >= 0 && ( sStatus.IsEmpty() || ( sStatus.Compare(_T("N")) == 0 ) ) )
			{
				pDM->StartFrame();

				pDM->InitDBFieldData();

				pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("S") );

				ids = pDM->MakeUpdateFrame( _T("MN_MAIL_TEMP_TBL") , _T("REC_KEY") , _T("NUMERIC") , sTempKey );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("StopSendMail") );
				}


				ids = pDM->SendFrame();
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("StopSendMail") );
				}

				pDM->EndFrame();

				ids = pDM->SetCellData( _T("����") , _T("S") , nSIndex  );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("StopSendMail") );
				}

				nSuccesCnt++;
			}
			else
			{
				if( ids < 0 ) 
				{
					ids = pDM->SetCellData( _T("����") , _T("OK") , nSIndex  );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("StopSendMail") );
					}
				}
				else
				{
					ids = pDM->SetCellData( _T("����") , sStatus , nSIndex  );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("StopSendMail") );
					}
				}

				nFailCnt++;
			}

			pGrid->DisplayLine(nSIndex);
		}
		else
		{
			nFailCnt++;
		}

		pGrid->SelectGetPrev();
	}
	

	CString sMsg;
	if( nSuccesCnt == 0 )
	{
		// 2009.03.25 UPDATE BY KSJ : ���� �޼��� ����
		//DEL sMsg.Format(_T("������ �� �ִ� ������ �������� �ʽ��ϴ�."));
		sMsg.Format(_T("������ �߼۴�� ���� ���ϸ� �����մϴ�."));
	}
	else
	{
		sMsg.Format(_T("����Ǿ����ϴ�.\r\n������ ���� �� : %d �� , ������ �� ���� ���� �� : %d ��") , nSuccesCnt , nFailCnt );
	}

	AfxMessageBox( sMsg );

	return 0;
}

/*
@Create
 - 2009.03.25 BY KSJ
@Description
 - �߼۴��(STATUS:N),�߼�����(STATUS:S) -> MN_MAIL_TEMP_TBL���� ����
@Parameter

@Return Value
 - 
 - 0, ��� : ����
 - ���� : ����
@Mini Spec
 1. 
*/
INT CBO_LOC_3890::CancelSendMail()
{
	INT ids;

	// �˻��ߴ��� Ȯ��
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3890") );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("CancelSendMail") );
	}

	if( pDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("�˻� ���� ���ֽʽÿ�.") );
		return 1;
	}

	// �����Ȱ��� �����ϴ��� Ȯ��
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3890") , _T("���α׸���") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("CancelSendMail") );
	}

	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("������ ������ �������ֽʽÿ�.") );
		return 2;
	}

	// �Ѱ� �Ѱ� ó���ؾ���
	// ������ ���� ���ư��� ������ ����
	// ����� �� �ִ� �������� �˾ƺ���.

	// ���������ϸ� ����

	// �̹� DB�� �������� �ʴ´ٸ� �߼۵����� ����

	pGrid->SelectGetTailPosition();
	INT		nSIndex , i;
	INT		nSuccesCnt = 0;
	INT		nFailCnt = 0;

	CString sTempKey , sStatus;
	CString sSql;

	for( i=0 ; i<nSelectCnt ; i++ )
	{
		nSIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("����") , nSIndex , sStatus );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("CancelSendMail") ); 
		}

		ids = pDM->GetCellData( _T("�߼۴����KEY") , nSIndex , sTempKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("CancelSendMail") ); 
		}

		if( (sStatus.Compare(_T("N"))==0)  || (sStatus.Compare(_T("S"))==0) || sStatus.IsEmpty() )
		{
			sSql.Format( _T("SELECT STATUS FROM MN_MAIL_TEMP_TBL WHERE REC_KEY = %s") , sTempKey );
			ids = pDM->GetOneValueQuery( sSql , sStatus );
			if( ids >= 0 && ((sStatus.Compare(_T("N"))==0)  || (sStatus.Compare(_T("S"))==0) || sStatus.IsEmpty() ) )
			{
				pDM->StartFrame();

				ids = pDM->MakeDeleteFrame( _T("MN_MAIL_TEMP_TBL") , _T("REC_KEY") , _T("NUMERIC") , sTempKey );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("CancelSendMail") );
				}


				ids = pDM->SendFrame();
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("CancelSendMail") );
				}

				pDM->EndFrame();

				ids = pDM->SetCellData( _T("����") , _T("CANCEL") , nSIndex  );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("CancelSendMail") );
				}

				nSuccesCnt++;
			}
			else
			{
				if( ids < 0 ) 
				{
					ids = pDM->SetCellData( _T("����") , _T("OK") , nSIndex  );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("CancelSendMail") );
					}
				}
				else
				{
					ids = pDM->SetCellData( _T("����") , sStatus , nSIndex  );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("CancelSendMail") );
					}
				}

				nFailCnt++;
			}

			pGrid->DisplayLine(nSIndex);
		}
		else
		{
			nFailCnt++;
		}

		pGrid->SelectGetPrev();
	}
	
	CString sMsg;
	if( nSuccesCnt == 0 )
	{
		// 2009.03.25 UPDATE BY KSJ : ���� �޼��� ����
		//DEL sMsg.Format(_T("������ �� �ִ� ������ �������� �ʽ��ϴ�."));
		sMsg.Format(_T("��Ҵ� �߼۴��,�߼����� ���� ���ϸ� �����մϴ�."));
	}
	else
	{
		sMsg.Format(_T("����Ǿ����ϴ�.\r\n������ ���� �� : %d �� , ������ �� ���� ���� �� : %d ��") , nSuccesCnt , nFailCnt );
	}

	AfxMessageBox( sMsg );

	return 0;
}

/*
@Create
 - 2009.03.25 BY KSJ
@Description
 - �߼�����(STATUS:S) -> �߼۴��(STATUS:N)���� ����
@Parameter

@Return Value
 - 
 - 0, ��� : ����
 - ���� : ����
@Mini Spec
 1. 
*/
INT CBO_LOC_3890::SendMail()
{
	INT ids;

	// �˻��ߴ��� Ȯ��
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3890") );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("CancelSendMail") );
	}

	INT		nRowCnt	=	pDM->GetRowCount();
	if( 0 == nRowCnt )
	{
		AfxMessageBox( _T("�˻� ���� ���ֽʽÿ�.") );
		return 1;
	}

	// �����Ȱ��� �����ϴ��� Ȯ��
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3890") , _T("���α׸���") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("CancelSendMail") );
	}

	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("������ ������ �������ֽʽÿ�.") );
		return 2;
	}

	// �Ѱ� �Ѱ� ó���ؾ���
	// ������ ���� ���ư��� ������ ����
	// �߼��� �� �ִ� �������� �˾ƺ���.

	// ���������ϸ� ����

	// �̹� DB�� �������� �ʴ´ٸ� �߼۵����� ����

	pGrid->SelectGetTailPosition();

	INT nSuccesCnt = 0;
	INT nFailCnt = 0;

	CString sTempKey , sStatus;
	CString sSql;

	INT		i , nSIndex;
	for( i=0 ; i<nSelectCnt ; i++ )
	{
		nSIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("����") , nSIndex , sStatus );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SendMail") ); 
		}

		ids = pDM->GetCellData( _T("�߼۴����KEY") , nSIndex , sTempKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SendMail") ); 
		}

		if( (sStatus.Compare(_T("S"))==0) || sStatus.IsEmpty() )
		{
			sSql.Format( _T("SELECT STATUS FROM MN_MAIL_TEMP_TBL WHERE REC_KEY = %s") , sTempKey );
			ids = pDM->GetOneValueQuery( sSql , sStatus );
			if( ids >= 0 && ((sStatus.Compare(_T("S"))==0) || sStatus.IsEmpty() ) )
			{
				pDM->StartFrame();

				pDM->InitDBFieldData();

				pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("N") );

				ids = pDM->MakeUpdateFrame( _T("MN_MAIL_TEMP_TBL") , _T("REC_KEY") , _T("NUMERIC") , sTempKey );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("SendMail") );
				}

				ids = pDM->SendFrame();
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("SendMail") );
				}

				pDM->EndFrame();

				ids = pDM->SetCellData( _T("����") , _T("N") , nSIndex  );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("SendMail") );
				}


				nSuccesCnt++;
			}
			else
			{
				if( ids < 0 ) 
				{
					ids = pDM->SetCellData( _T("����") , _T("OK") , nSIndex  );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("SendMail") );
					}
				}
				else
				{
					ids = pDM->SetCellData( _T("����") , sStatus , nSIndex  );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("SendMail") );
					}
				}

				nFailCnt++;
			}

			pGrid->DisplayLine(nSIndex);
		}
		else
		{
			nFailCnt++;
		}

		pGrid->SelectGetPrev();
	}
	

	CString sMsg;
	if( nSuccesCnt == 0 )
	{
		// 2009.03.25 UPDATE BY KSJ : ���� �޼��� ����
		//DEL sMsg.Format(_T("������ �� �ִ� ������ �������� �ʽ��ϴ�."));
		sMsg.Format(_T("�߼��� �߼����� ���� ���ϸ� �����մϴ�."));
	}
	else
	{
		sMsg.Format(_T("����Ǿ����ϴ�.\r\n������ ���� �� : %d �� , ������ �� ���� ���� �� : %d ��") , nSuccesCnt , nFailCnt );
	}

	AfxMessageBox( sMsg );

	return 0;
}

HBRUSH CBO_LOC_3890::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
