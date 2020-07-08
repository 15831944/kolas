// BO_LOC_3891.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3891.h"
// 2011.09.08 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3891 dialog


CBO_LOC_3891::CBO_LOC_3891(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3891)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CBO_LOC_3891::~CBO_LOC_3891()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
	m_Dlg_brush.DeleteObject();
}

VOID CBO_LOC_3891::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3891)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3891, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3891)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btn3891_CLOSE, Onbtn3891CLOSE)
	ON_BN_CLICKED(IDC_btn3891_SEARCH, Onbtn3891SEARCH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3891 message handlers

BOOL CBO_LOC_3891::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239)); 
	
	INT ids = InitESL_Mgr( _T("BO_LOC_3891") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( - 1, _T("OnInitDialog") );

	InitMailClass();
	
	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		GetDlgItem(IDC_sta3891_MANAGE_CODE)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmb3891_MANAGE_CODE)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




BOOL CBO_LOC_3891::Create(CWnd* pParentWnd) 
{

	return CDialog::Create(IDD, pParentWnd);

}


INT CBO_LOC_3891::InitMailClass()
{

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3830_MAIL_CLASS"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitMailClass") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3891"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitMailClass") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("��������")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitMailClass") );

	CString sInsertCode[2];
	
	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("  ");
	sInsertCode[1] = _T("�������");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitMailClass") );

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("�ڵ�") , i , sInsertCode[0] );
		pDM->GetCellData( _T("����") , i , sInsertCode[1] );
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitMailClass") );
	}
	
	
	pCMCB->SetCurSel(0);

	return 0;

}

VOID CBO_LOC_3891::Onbtn3891CLOSE() 
{
	OnCancel();	
}

VOID CBO_LOC_3891::Onbtn3891SEARCH() 
{
	AfxGetApp()->DoWaitCursor(1);	

	SearchMailTemp();

	AfxGetApp()->DoWaitCursor(-1);
}


INT CBO_LOC_3891::SearchMailTemp()
{
	INT ids;
	CString sWhere;
	CString sUnionSql;

	/// WHERE�� ����
	ids = GetWhere( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchMailTemp") );

	/// UNNION ���� ����
	// ���� ���� , ������ �̸� , E_MAIL , �Է��� , �߼۴����KEY
	// 2011.09.08 ADD : 3000Api
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CString strQuery1,strQuery2;
		strQuery1.Format( 
			_T("SELECT  ")
			_T(" MMTT.MAIL_CLASS , CLU.NAME , CLU.E_MAIL , TO_CHAR(MMTT.SEND_DATE,'YYYY/MM/DD HH24:MI:SS') , MMTT.REC_KEY ,")
			_T(" MMTT.STATUS ")
			_T(" FROM MN_MAIL_TEMP_TBL MMTT , CO_LOAN_USER_TBL CLU ")
			_T(" WHERE MMTT.USER_KEY = CLU.REC_KEY AND MMTT.MAIL_CLASS <> '13' ")
			_T(" %s "),
			sWhere
		);
		strQuery2.Format( 
			_T("SELECT  ")
			_T(" MMTT.MAIL_CLASS , NULL , CLU.EMAIL , TO_CHAR(MMTT.SEND_DATE,'YYYY/MM/DD HH24:MI:SS') , MMTT.REC_KEY , ")
			_T(" MMTT.STATUS ")
			_T(" FROM MN_MAIL_TEMP_TBL MMTT , BO_FURNISH_TBL CLU ")
			_T(" WHERE MMTT.FURNISH_KEY = CLU.REC_KEY AND MMTT.MAIL_CLASS = '13' ")
			_T(" %s "),
			sWhere
		);

		/// ���� ������
		CESL_DataMgr TmpDM1;
		TmpDM1.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
		CESL_DataMgr TmpDM2;
		TmpDM2.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
		ids = TmpDM1.RestructDataManager( strQuery1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchMailTemp") );
		ids = TmpDM2.RestructDataManager( strQuery2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchMailTemp") );

		CStringArray asDes1,asDes2;
		CString strGetData;
		for(INT i = 0; i < TmpDM1.GetRowCount(); i++)
		{
			TmpDM1.GetCellData(i, 2, strGetData);
			asDes1.Add(strGetData);
		}
		for(i = 0; i < TmpDM2.GetRowCount(); i++)
		{
			TmpDM2.GetCellData(i, 2, strGetData);
			asDes2.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes2);
					pApi->CloseSocket();
				}
				for(i = 0; i < TmpDM1.GetRowCount(); i++)
				{
					TmpDM1.SetCellData(i, 2, asDes1.GetAt(i));
				}
				for(i = 0; i < TmpDM2.GetRowCount(); i++)
				{
					TmpDM2.SetCellData(i, 2, asDes2.GetAt(i));
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		TmpDM.AddDM(&TmpDM1);
		TmpDM.AddDM(&TmpDM2);
	}
	else
	{
		sUnionSql.Format( 
			_T("SELECT  ")
			_T(" MMTT.MAIL_CLASS , CLU.NAME , CLU.E_MAIL , TO_CHAR(MMTT.SEND_DATE,'YYYY/MM/DD HH24:MI:SS') , MMTT.REC_KEY ,")
			_T(" MMTT.STATUS ")
			_T(" FROM MN_MAIL_TEMP_TBL MMTT , CO_LOAN_USER_TBL CLU ")
			_T(" WHERE MMTT.USER_KEY = CLU.REC_KEY AND MMTT.MAIL_CLASS <> '13' ")
			_T(" %s ")
			_T(" UNION ")
			_T("SELECT  ")
			_T(" MMTT.MAIL_CLASS , NULL , CLU.EMAIL , TO_CHAR(MMTT.SEND_DATE,'YYYY/MM/DD HH24:MI:SS') , MMTT.REC_KEY , ")
			_T(" MMTT.STATUS ")
			_T(" FROM MN_MAIL_TEMP_TBL MMTT , BO_FURNISH_TBL CLU ")
			_T(" WHERE MMTT.FURNISH_KEY = CLU.REC_KEY AND MMTT.MAIL_CLASS = '13' ")
			_T(" %s ") ,
			sWhere , sWhere
		);

		ids = TmpDM.RestructDataManager( sUnionSql );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchMailTemp") );
	}

	/// DM�� ����� �μ�Ʈ
	CESL_DataMgr* pParentDM = FindDM( _T("DM_BO_LOC_3890") );
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchMailTemp") );

	pParentDM->FreeData();

	const INT nFieldCnt = 6;
	CString sDMFieldName[nFieldCnt] =
	{
		_T("��������") , _T("�������̸�") , _T("E_MAIL") , _T("�Է���") , _T("�߼۴����KEY") , _T("����")
	};

	CString sData;
	for( INT i = 0 ; i < TmpDM.GetRowCount() ; i++ )
	{
		pParentDM->InsertRow(-1);

		for( INT j = 0 ; j < nFieldCnt ; j++ )
		{
			ids = TmpDM.GetCellData( i , j , sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SearchMailTemp") );

			ids = pParentDM->SetCellData( sDMFieldName[j] , sData , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SearchMailTemp") );
		}
	}
	


	/// �Է��� ASC �� ����
	pParentDM->SORT( _T("�Է���") ,FALSE );

	/// ȭ�鿡 �����ֱ�
	ids = AllControlDisplay( _T("CM_BO_LOC_3890") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("SearchMailTemp") ); 

	AfxMessageBox( _T("�˻��Ͽ����ϴ�.") );

	return 0;

}

INT CBO_LOC_3891::GetWhere( CString &sWhere )
{
	INT ids;

	CString sMailClass;
	CString sInputDate1 , sInputDate2;

	ids = GetControlData( _T("CM_BO_LOC_3891") , _T("��������") , sMailClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("GetWhere") ); 

	ids = GetControlData( _T("CM_BO_LOC_3891") , _T("�Է���1") , sInputDate1  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("GetWhere") ); 

	ids = GetControlData( _T("CM_BO_LOC_3891") , _T("�Է���2") , sInputDate2  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("GetWhere") ); 

	if( !sMailClass.IsEmpty() )
	{
		sWhere += _T(" AND MAIL_CLASS='")+sMailClass+_T("' ");
	}

	if( !sInputDate1.IsEmpty() )
	{
		if( sInputDate2.IsEmpty() )
			sWhere += _T(" AND SEND_DATE BETWEEN TO_DATE('")+sInputDate1+_T(" 00:00:00' ,'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('")+sInputDate1+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		else
			sWhere += _T(" AND SEND_DATE BETWEEN TO_DATE('")+sInputDate1+_T(" 00:00:00' ,'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('")+sInputDate2+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");

	}

	CString strManageCode;
	CESL_ControlMultiComboBox* pCmb = (CESL_ControlMultiComboBox*)FindControl(_T("CM_BO_LOC_3891"), _T("��������"));
	if(NULL != pCmb)
	{	
		const int nIdx = pCmb->GetCurSel();
		if(0 > nIdx)
		{
			return -1000;
		}

		pCmb->GetData(strManageCode, nIdx, 0);
		if(FALSE == strManageCode.IsEmpty())
		{
			sWhere += _T(" AND MMTT.MANAGE_CODE = '") + strManageCode +_T("'");
		}
	}

	return 0;
}
HBRUSH CBO_LOC_3891::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=NULL;
	if(pWnd->m_hWnd==this->m_hWnd) hbr= HBRUSH(m_Dlg_brush);	
	else hbr =CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG) 
	{
		pDC->SetTextColor(RGB(0, 0, 0));		
		pDC->SetBkMode(OPAQUE);		
		pDC->SetBkColor(RGB(239, 239, 239));		
		
		return m_Dlg_brush;		
	}
	// ������� �߰�
	return hbr;
}