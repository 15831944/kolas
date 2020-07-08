// BO_LOC_3841.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3841.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3841 dialog

#define ECO_MAIL_RESULT_FILE _T("..\\cfg\\���ϸ��ý���\\�����.cfg")

CBO_LOC_3841::CBO_LOC_3841(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3841)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CBO_LOC_3841::~CBO_LOC_3841()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CBO_LOC_3841::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3841)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3841, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3841)
	ON_BN_CLICKED(IDC_btn3841_Search, Onbtn3841Search)
	ON_BN_CLICKED(IDC_btn3841_Search2, Onbtn3841Search2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3841 message handlers

BOOL CBO_LOC_3841::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("BO_LOC_3841") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3841") );

	ids = InitMailClass();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3841") );

	ids = InitResultCfg();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3841") );

	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		GetDlgItem(IDC_sta3841_MANAGE_CODE)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmb3841_MANAGE_CODE)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END 

	return FALSE;
}

VOID CBO_LOC_3841::Onbtn3841Search() 
{
	EFS_BEGIN

	AfxGetApp()->DoWaitCursor(1);

	INT ids = SearchResult();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("Onbtn3841Search") );

	AfxGetApp()->DoWaitCursor(-1);

	EFS_END
	
}

VOID CBO_LOC_3841::Onbtn3841Search2() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}


BOOL CBO_LOC_3841::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END

	return FALSE;
}




INT CBO_LOC_3841::InitMailClassInfo()
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


INT CBO_LOC_3841::ConvertMailClassCodeToDesc(CString sCode,CString &sDesc)
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



INT CBO_LOC_3841::InitMailClassControl()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3830_MAIL_CLASS"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitMailClassControl") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3841"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitMailClassControl") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("��������")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitMailClassControl") );

	CString sInsertCode[2];
	
	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("  ");
	sInsertCode[1] = _T("�������");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitMailClassControl") );

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("�ڵ�") , i , sInsertCode[0] );
		pDM->GetCellData( _T("����") , i , sInsertCode[1] );
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitMailClassControl") );
	}
	
	
	pCMCB->SetCurSel(0);

	return 0;

EFS_END
return -1;

}



INT CBO_LOC_3841::InitMailClass()
{
EFS_BEGIN

	INT ids;

	ids = InitMailClassInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitMailClass") );

	ids = InitMailClassControl();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitMailClass") );


	return 0;

EFS_END
return -1;

}


INT CBO_LOC_3841::InitResultCfg()
{
EFS_BEGIN
	
	INT ids;

	/// ù���� : ��
	/// �ι�° ���� : ��
	/// ����° ���� : �����
	CStdioFile fCfgFile;
	ids = fCfgFile.Open( ECO_MAIL_RESULT_FILE , CFile::modeRead | CFile::typeBinary );
	if( ids == 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitResultCfg") );
		TCHAR cUni;
		fCfgFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fCfgFile.SeekToBegin();
		}
	
	CString sColAlias;
	CString sRowAlias;
	CString sResultAlias;

	CString sLine;
	INT nIndex = 0;
	while( fCfgFile.ReadString( sLine ) )
	{
		sLine.TrimRight();
		sLine.TrimLeft();
		if( nIndex == 0 )
			sColAlias = sLine;
		else if( nIndex == 1 )
			sRowAlias = sLine;
		else
		{
			if( nIndex != 2 ) sResultAlias += _T("\r\n");
			sResultAlias += sLine;
		}
		
		nIndex++;
	}

	fCfgFile.Close();

	/// 1. �� ����
	CString sTmp;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmd3841_COMOB7);
	for( INT i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sTmp);
		if( sColAlias.Compare( sTmp ) == 0 )
		{
			pCombo->SetCurSel(i);
		}
	}

	/// 2. �� ����
	pCombo = (CComboBox*)GetDlgItem(IDC_cmd3841_COMOB6);
	for( i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sTmp);
		if( sRowAlias.Compare( sTmp ) == 0 )
		{
			pCombo->SetCurSel(i);
		}
	}

	/// 3. ��� ����
	GetDlgItem(IDC_edt3841_EDIT8)->SetWindowText(sResultAlias);
	GetDlgItem(IDC_edt3841_EDIT9)->SetWindowText(_T("(���� : �ѹ߼۰Ǽ�(�߼ۼ����Ǽ�(���ŰǼ�/�̼��ŰǼ�)/�߼۽��аǼ�))"));
	

	return 0;



EFS_END

return -1;
}

INT CBO_LOC_3841::SearchResult()
{
EFS_BEGIN

	INT ids;
	CString sWhere;
	CESL_DataMgr SendDM,ResultDM;
	CString sColAlias,sRowAlias,sResultAlias;

	/// 1. ������� ������ �Ǿ��ִ��� �˻��ϱ�
	ids = CheckResultValue(sColAlias,sRowAlias,sResultAlias);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchResult") );
	if( ids > 0 ) return ids;

	/// 2. WHERE�� �����ϱ�
	ids = GetWhere( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchResult") );
	if( ids > 0 ) return ids;

	/// 3. ���� ������
	ids = SendQuery(sWhere,sColAlias,sRowAlias,&SendDM);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchResult") );
	if( ids > 0 ) return ids;

	/// 4. ����� ��� DM�����ϱ�
	ids = ReStructDM(&SendDM);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchResult") );
	if( ids > 0 ) return ids;

	/// 5. �׸��忡 �����ֱ�
	ids = ShowResult(sColAlias,sRowAlias,sResultAlias);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchResult") );
	if( ids > 0 ) return ids;


	return 0;

EFS_END

return -1;
}


INT CBO_LOC_3841::GetWhere( CString &sWhere )
{
EFS_BEGIN

	INT ids;

	const INT nControlCnt = 10;
	CString sControlAlias[nControlCnt] =
	{
		_T("��������") , _T("���Ϲ߼���1") , _T("���Ϲ߼���2") , _T("��ġ�۾���") , _T("�����ּ�") , 
		_T("�������̸�") , _T("�Ҽ�") ,_T("����") , _T("����1") , _T("����2") 
	};

	CString sGetData[nControlCnt];

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3841"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetWhere") );

	for( INT i = 0 ; i < nControlCnt ; i++ )
	{

		ids = pCM->GetControlMgrData( sControlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetWhere") );

		sGetData[i].TrimLeft(); sGetData[i].TrimRight();

	}

	// �⺻����
	sWhere = _T("MAIL_CLASS NOT IN ( '9' )");

	// ��������
	if( !sGetData[0].IsEmpty() )
	{
		sWhere += _T(" AND MAIL_CLASS = '")+sGetData[0]+_T("' ");
	}

	// ���Ϲ߼���
	if( !sGetData[1].IsEmpty() )
	{
		if( sGetData[2].IsEmpty() )
		{
			sWhere += _T(" AND SEND_DATE BETWEEN TO_DATE('")+sGetData[1]+_T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND ")
					  _T("TO_DATE('")+sGetData[1]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
		else
		{
			sWhere += _T(" AND SEND_DATE BETWEEN TO_DATE('")+sGetData[1]+_T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND ")
					  _T("TO_DATE('")+sGetData[2]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
	}

	// ��ġ �۾���
	if( !sGetData[3].IsEmpty() )
	{
		sWhere += _T(" AND BATCH_WORK_NAME BETWEEN '")+sGetData[3]+_T("' AND '")+sGetData[3]+_T("��' ");
	}

	// �����ּ�
	if( !sGetData[4].IsEmpty() )
	{
		sWhere += _T(" AND E_MAIL BETWEEN '")+sGetData[4]+_T("' AND '")+sGetData[4]+_T("��' ");
	}

	// �������̸�
	if( !sGetData[5].IsEmpty() )
	{
		sWhere += _T(" AND NAME BETWEEN '")+sGetData[5]+_T("' AND '")+sGetData[5]+_T("��' ");
	}

	// �Ҽ�
	if( !sGetData[6].IsEmpty() )
	{
		sWhere += _T(" AND USER_POSITION_CODE BETWEEN '")+sGetData[6]+_T("' AND '")+sGetData[6]+_T("��' ");
	}

	// ����
	if( !sGetData[7].IsEmpty() )
	{
		sWhere += _T(" AND USER_CLASS_CODE BETWEEN '")+sGetData[7]+_T("' AND '")+sGetData[7]+_T("��' ");
	}

	// ����
	if( !sGetData[8].IsEmpty() )
	{
		if( sGetData[9].IsEmpty() )
		{
			sWhere += _T(" AND AGE = ") + sGetData[8];
		}
		else
		{
			sWhere += _T(" AND AGE BETWEEN ")+sGetData[8]+_T(" AND ")+sGetData[9]+_T(" ");
		}
	}

	// �߼ۿ���
	CString sSendYN;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmd3841_COMOB2);

	pCombo->GetLBText( pCombo->GetCurSel() , sSendYN );

	if( sSendYN.Compare(_T("��ü")) != 0 )
	{
		sWhere += _T(" AND MAIL_SEND_YN = '") + sSendYN+_T("'");
	}

	

	// ���ſ���
	CString sReceiptYN;
	pCombo = (CComboBox*)GetDlgItem(IDC_cmd3841_COMOB3);

	pCombo->GetLBText( pCombo->GetCurSel() , sReceiptYN );

	if( sReceiptYN.Compare(_T("��ü")) != 0 )
	{
		sWhere += _T(" AND MAIL_CHECK_YN = '") + sReceiptYN+_T("'");
	}

	CString strManageCode;
	CESL_ControlMultiComboBox* pCmb = (CESL_ControlMultiComboBox*)FindControl(_T("CM_BO_LOC_3841"), _T("��������"));
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
			sWhere += _T(" AND MANAGE_CODE = '") + strManageCode +_T("'");
		}
	}

	return 0;

EFS_END
return -1;

}


INT CBO_LOC_3841::CheckResultValue( CString &sColAlias,CString &sRowAlias,CString &sResultAlias)
{
EFS_BEGIN

	INT ids;

	/// �� ��������
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmd3841_COMOB7);
	pCombo->GetLBText( pCombo->GetCurSel() , sColAlias );

	/// �� ��������
	pCombo = (CComboBox*)GetDlgItem(IDC_cmd3841_COMOB6);
	pCombo->GetLBText( pCombo->GetCurSel() , sRowAlias );


	/// ��� ��������
	GetDlgItem(IDC_edt3841_EDIT8)->GetWindowText(sResultAlias);

	if( sResultAlias.IsEmpty() )
	{
		AfxMessageBox( _T("��� ������ �Է����ֽʽÿ�.") );
		return 1;
	}

	/// �������� ���Ͽ� ����

	CStdioFile fCfgFile;
	ids = fCfgFile.Open( ECO_MAIL_RESULT_FILE , CFile::modeCreate | CFile::modeWrite | CFile::typeBinary );
	if( ids == 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitResultCfg") );

	if( 0 == fCfgFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fCfgFile.Write( &cUni, sizeof(TCHAR));
	}	
	
	fCfgFile.WriteString( sColAlias );
	fCfgFile.WriteString( _T("\r\n") );
	fCfgFile.WriteString( sRowAlias );
	fCfgFile.WriteString( _T("\r\n") );
	fCfgFile.WriteString( sResultAlias );
	fCfgFile.WriteString( _T("\r\n") );

	fCfgFile.Close();

	return 0;

EFS_END

return -1;
}


INT CBO_LOC_3841::SendQuery( CString sWhere,CString sColAlias,CString sRowAlias,CESL_DataMgr* pSendDM)
{
EFS_BEGIN

	INT ids;

	/// �� , �� �ʵ�� ��ȯ�ϱ�
	CString sColFieldName , sRowFieldName;

	ids = ConvertAliasToFieldName( sColAlias , sColFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SendQuery") );
	if( ids > 0 ) 
	{
		AfxMessageBox( _T("���� DB Field�̸����� ������ �� �����ϴ�.") );
		return ids;
	}

	ids = ConvertAliasToFieldName( sRowAlias , sRowFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SendQuery") );
	if( ids > 0 ) 
	{
		AfxMessageBox( _T("���� DB Field�̸����� ������ �� �����ϴ�.") );
		return ids;
	}

	/// ���� �����
	CString sQuery;
	sQuery.Format( _T("SELECT %s,%s,MAIL_SEND_YN,MAIL_CHECK_YN,COUNT(*) FROM MN_MAIL_LOG_TBL WHERE %s ")
				   _T(" GROUP BY %s,%s,MAIL_SEND_YN,MAIL_CHECK_YN ") ,
				   sColFieldName , sRowFieldName , sWhere , sColFieldName , sRowFieldName );

	pSendDM->SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	ids = pSendDM->RestructDataManager( sQuery );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SendQuery") );

	if( pSendDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("�ڷᰡ �������� �ʽ��ϴ�.") );
		return 1;
	}

	return 0;

EFS_END

return -1;
}


INT CBO_LOC_3841::ConvertAliasToFieldName( CString sAlias , CString &sFieldName )
{
EFS_BEGIN

	const INT nCnt = 8;

	CString sFieldInfo[nCnt][2] =
	{
		{ _T("��������") , _T("MAIL_CLASS") } , 
		{ _T("�Ҽ��ڷ��") , _T("SHELF_LOC_CODE") } , 
		{ _T("����") , _T("USER_CLASS_CODE") } , 
		{ _T("�Ҽ�") , _T("USER_POSITION_CODE") } , 
		{ _T("�߼���") , _T("TO_CHAR(SEND_DATE,'YYYY/MM/DD')") } , 
		{ _T("�߼ۿ�") , _T("TO_CHAR(SEND_DATE,'YYYY/MM')") } , 
		{ _T("�߼۳�") , _T("TO_CHAR(SEND_DATE,'YYYY')") } , 
		{ _T("����") , _T("FLOOR(AGE/10)*10") } 
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sFieldInfo[i][0].Compare( sAlias ) == 0 )
		{
			sFieldName = sFieldInfo[i][1];
			return 0;
		}
	}


return 1;

EFS_END

return -1;
}

INT CBO_LOC_3841::ReStructDM( CESL_DataMgr* pSendDM)
{
EFS_BEGIN

	INT ids;
	CString sColFieldName , sRowFieldName , sSendYN , sCheckYN , sResult;

	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("ReStructDM") );

	pResultDM->FreeData();


	INT nRowCnt = pSendDM->GetRowCount();

	for( INT i = 0 ; i < nRowCnt ; i++ )
	{
		ids = pSendDM->GetCellData( i , 0 , sColFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ReStructDM") );
		ids = pSendDM->GetCellData( i , 1 , sRowFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ReStructDM") );
		ids = pSendDM->GetCellData( i , 2 , sSendYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ReStructDM") );
		ids = pSendDM->GetCellData( i , 3 , sCheckYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ReStructDM") );
		ids = pSendDM->GetCellData( i , 4 , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ReStructDM") );


		ids = InsertValueToResultDM(sColFieldName , sRowFieldName , sSendYN , sCheckYN , sResult);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ReStructDM") );
	}

	/// �ջ� ���
	ids = SumCal();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ReStructDM") ); 


	return 0;


EFS_END

return -1;

}


INT CBO_LOC_3841::ShowResult(CString sColAlias,CString sRowAlias,CString sResultAlias)
{
EFS_BEGIN

	INT ids;

	/// �׸��� ����
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3840") , _T("���α׸���") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowResult") ); 

	pGrid->SetRedraw(FALSE);

	// ���� �� �����ϱ�
	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ShowResult") );

	CESL_DataMgr* pColDM = FindDM( _T("DM_BO_LOC_3840_��") );
	if( pColDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ShowResult") );

	CESL_DataMgr* pRowDM = FindDM( _T("DM_BO_LOC_3840_��") );
	if( pRowDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ShowResult") );

	pColDM->FreeData();
	pRowDM->FreeData();
	

	CString sColField , sRowField;
	for( INT i = 0 ; i < pResultDM->GetRowCount() ; i++ )
	{
		ids = pResultDM->GetCellData( _T("��") , i , sColField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ShowResult") );

		ids = pResultDM->GetCellData( _T("��") , i , sRowField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ShowResult") );

		ids = AddRemoveDuplicate( pColDM , sColField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ShowResult") );

		ids = AddRemoveDuplicate( pRowDM , sRowField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ShowResult") );
	}

	// �����ϱ� , �ڵ� �������� �ٲٱ�
	ids = ConvertCodeToDescSort( pColDM , sColAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ShowResult") );

	ids = ConvertCodeToDescSort( pRowDM , sRowAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("ShowResult") );

	// �׸��忡 ����
	INT nColCnt = pColDM->GetRowCount();
	INT nRowCnt = pRowDM->GetRowCount();
	pGrid->SetCols(0,2);
	pGrid->SetRows(2);
	pGrid->SetCols( 0 , pColDM->GetRowCount()+2 );
	pGrid->SetRows( pRowDM->GetRowCount()+2 );

	CString sCheckYCnt , sCheckNCnt , sSendNCnt;
	CString sSendCnt,sSendYCnt;
	CString sInputResult;
	INT nIndex;
	for( i = 0 ; i < pColDM->GetRowCount() ; i++ )
	{
		ids = pColDM->GetCellData( _T("�ڵ�") , i , sColField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("ShowResult") ); 
		for( INT j = 0 ; j < pRowDM->GetRowCount() ; j++ )
		{
			ids = pRowDM->GetCellData( _T("�ڵ�") , j , sRowField );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("ShowResult") ); 
			
			nIndex = GetResultIdx( sColField , sRowField );

			sSendCnt.Empty();
			sSendYCnt.Empty();
			sCheckYCnt.Empty();
			sCheckNCnt.Empty();
			sSendNCnt.Empty();

			if( nIndex >= 0 )
			{
				ids = pResultDM->GetCellData( _T("�ѹ߼۰Ǽ�") , nIndex , sSendCnt );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("ShowResult") ); 
				ids = pResultDM->GetCellData( _T("�߼ۼ۰��Ǽ�") , nIndex , sSendYCnt);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("ShowResult") ); 
				ids = pResultDM->GetCellData( _T("���ŰǼ�") , nIndex , sCheckYCnt );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("ShowResult") ); 
				ids = pResultDM->GetCellData( _T("�̼��ŰǼ�") , nIndex , sCheckNCnt);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("ShowResult") ); 
				ids = pResultDM->GetCellData( _T("�߼۽��аǼ�") , nIndex ,sSendNCnt  );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("ShowResult") ); 
			}

			sInputResult = sResultAlias;

			if( sSendCnt.IsEmpty() ) sSendCnt = _T("0");
			if( sSendYCnt.IsEmpty() ) sSendYCnt = _T("0");
			if( sCheckYCnt.IsEmpty() ) sCheckYCnt = _T("0");
			if( sCheckNCnt.IsEmpty() ) sCheckNCnt = _T("0");
			if( sSendNCnt.IsEmpty() ) sSendNCnt = _T("0");

			sInputResult.Replace( _T("%�ѹ߼۰Ǽ�%") , sSendCnt );
			sInputResult.Replace( _T("%�߼ۼ����Ǽ�%") , sSendYCnt );
			sInputResult.Replace( _T("%���ŰǼ�%") , sCheckYCnt );
			sInputResult.Replace( _T("%�̼��ŰǼ�%") , sCheckNCnt );
			sInputResult.Replace( _T("%�߼۽��аǼ�%") , sSendNCnt );

			pGrid->SetTextMatrix( j+1 , i+1 , sInputResult );
		}
	}

	// �հ� �����ϱ�
	for( i = 0 ; i < pColDM->GetRowCount() ; i++ )
	{
		ids = pColDM->GetCellData( _T("�ڵ�") , i , sColField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("ShowResult") ); 

		ids = GetSumInfo( 0 , sColField , sResultAlias , sInputResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("ShowResult") ); 

		pGrid->SetTextMatrix( pRowDM->GetRowCount()+1 , i+1 , sInputResult );

	}

	for( i = 0 ; i < pRowDM->GetRowCount() ; i++ )
	{
		ids = pRowDM->GetCellData( _T("�ڵ�") , i , sRowField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("ShowResult") ); 

		ids = GetSumInfo( 1 , sRowField , sResultAlias , sInputResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("ShowResult") ); 

		pGrid->SetTextMatrix( i+1 ,  pColDM->GetRowCount()+1 , sInputResult );

	}

	// ��ü �հ� �����ϱ�
	ids = GetTotalSumInfo( sResultAlias, sInputResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("ShowResult") ); 

	pGrid->SetTextMatrix( pRowDM->GetRowCount()+1 , pColDM->GetRowCount()+1 , sInputResult );


	// ��� ���� �Է��ϱ�
	pGrid->SetColAlignment( 0 , 1 );
	pGrid->SetColAlignment( pColDM->GetRowCount()+1 , 1 );
	pGrid->SetColWidth( 0 , 0 , 2000 );
	pGrid->SetTextMatrix( pRowDM->GetRowCount()+1 , 0 , _T("�հ�") );
	pGrid->SetCol( pColDM->GetRowCount() + 1 );
	pGrid->SetRow( 0 );
	pGrid->SetCellFontBold(TRUE);
	pGrid->SetCol( 0 );
	pGrid->SetRow( pRowDM->GetRowCount() + 1 );
	pGrid->SetCellFontBold(TRUE);
	pGrid->SetCol( pColDM->GetRowCount() + 1 );
	pGrid->SetRow( pRowDM->GetRowCount() + 1 );
	pGrid->SetCellFontBold(TRUE);


	for( i = 0 ; i < pColDM->GetRowCount() ; i++ )
	{
		pGrid->SetColAlignment( i+1 , 1 );
		ids = pColDM->GetCellData( _T("����") , i , sColField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("ShowResult") ); 

		pGrid->SetTextMatrix( 0 , i+1 , sColField );
		pGrid->SetCol(i+1);
		pGrid->SetRow(0);
		pGrid->SetCellFontBold(TRUE);
		pGrid->SetColWidth( i+1 , 0 , 2000 );
	}

	pGrid->SetColWidth( i+1 , 0 , 2000 );
	
	// ��� ���� �Է��ϱ�
	pGrid->SetTextMatrix( 0 , pColDM->GetRowCount()+1 , _T("�հ�") );
	pGrid->SetRowHeight(0,600);
	for( i = 0 ; i < pRowDM->GetRowCount() ; i++ )
	{
		ids = pRowDM->GetCellData( _T("����") , i , sColField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("ShowResult") ); 

		pGrid->SetTextMatrix( i+1 , 0 , sColField );
		pGrid->SetCol(0);
		pGrid->SetRow(i+1);
		pGrid->SetCellFontBold(TRUE);
		pGrid->SetRowHeight(i+1,600);
	}
	pGrid->SetRowHeight(i+1,600);

	pGrid->SetRedraw(TRUE);

	AfxMessageBox( _T("�Ϸ�Ǿ����ϴ�.") );


	return 0;


EFS_END

return -1;

}


INT CBO_LOC_3841::InsertValueToResultDM( CString sColFieldName , CString sRowFieldName , CString sSendYN , CString sCheckYN , CString sResult)
{
EFS_BEGIN

	INT ids;

	INT nIndex = -1;
	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertValueToResultDM") );

	CString sTmpColName , sTmpRowName;
	BOOL bStop = FALSE;
	for( INT i = 0 ; i < pResultDM->GetRowCount() ; i++ )
	{
		ids = pResultDM->GetCellData( _T("��") , i , sTmpColName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertValueToResultDM") );

		ids = pResultDM->GetCellData( _T("��") , i , sTmpRowName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertValueToResultDM") );

		if( (sTmpColName.Compare(sColFieldName) == 0) && (sTmpRowName.Compare( sRowFieldName ) == 0) )
		{
			nIndex = i;
			bStop = TRUE;
		}
		if( bStop ) break;
	}

	if( nIndex == -1 )
	{
		pResultDM->InsertRow(-1);
		nIndex = pResultDM->GetRowCount() - 1;

		ids = pResultDM->SetCellData( _T("��")  , sColFieldName , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("InsertValueToResultDM") );
		ids = pResultDM->SetCellData( _T("��")  , sRowFieldName , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("InsertValueToResultDM") );

	}

	CString sInsertColAlias;
	if( sSendYN.Compare(_T("Y")) == 0 )
	{
		if( sCheckYN.Compare(_T("Y")) == 0 )
			sInsertColAlias = _T("���ŰǼ�");
		else
			sInsertColAlias = _T("�̼��ŰǼ�");
	}
	else
	{
		sInsertColAlias = _T("�߼۽��аǼ�");
	}


	CString sPreCnt;
	ids = pResultDM->GetCellData( sInsertColAlias , nIndex , sPreCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertValueToResultDM") );

	sResult.Format( _T("%d") , _ttoi(sResult) + _ttoi(sPreCnt) );

	ids = pResultDM->SetCellData( sInsertColAlias  , sResult , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertValueToResultDM") );


	return 0;

EFS_END

return -1;
}

INT CBO_LOC_3841::SumCal()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SumCal") );
	
	INT nRowCnt = pResultDM->GetRowCount();

	CString sCheckYCnt , sCheckNCnt , sSendNCnt;
	CString sSendCnt,sSendYCnt;


	for( INT i = 0 ; i < nRowCnt ; i++ )
	{
		ids = pResultDM->GetCellData( _T("���ŰǼ�") , i , sCheckYCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SumCal") ); 

		ids = pResultDM->GetCellData( _T("�̼��ŰǼ�") , i , sCheckNCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SumCal") ); 

		ids = pResultDM->GetCellData( _T("�߼۽��аǼ�") , i , sSendNCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SumCal") ); 
		
		sSendCnt.Format( _T("%d") , _ttoi(sCheckYCnt) + _ttoi(sCheckNCnt) + _ttoi(sSendNCnt) );

		ids = pResultDM->SetCellData( _T("�ѹ߼۰Ǽ�") , sSendCnt , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SumCal") ); 

		sSendYCnt.Format( _T("%d") , _ttoi(sCheckYCnt) + _ttoi(sCheckNCnt) ) ;

		ids = pResultDM->SetCellData( _T("�߼ۼ۰��Ǽ�") , sSendYCnt , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("SumCal") ); 
	}

	


	return 0;

EFS_END

return -1;
}


INT CBO_LOC_3841::AddRemoveDuplicate( CESL_DataMgr* pDM , CString sField )
{
EFS_BEGIN

	INT ids;
	INT nIndex=-1;
	
	CString sCode;
	BOOL bStop = FALSE;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("�ڵ�") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddRemoveDuplicate") );

		if( sCode.Compare(sField) == 0 )
		{
			nIndex = i;
			bStop = TRUE;
		}

		if( bStop ) break;

	}

	if( nIndex != -1 ) return 0;

	pDM->InsertRow(-1);

	ids = pDM->SetCellData( _T("�ڵ�") ,  sField , pDM->GetRowCount()-1);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("AddRemoveDuplicate") );


	return 0;

EFS_END

return -1;
}


INT CBO_LOC_3841::ConvertCodeToDescSort( CESL_DataMgr* pDM , CString sAlias )
{
EFS_BEGIN

	INT ids;

	CString sSortAlias;
	CString sCode;
	CString sDesc;
	if( sAlias.Compare( _T("��������") ) == 0 )
	{
		sSortAlias = _T("�ڵ�");

		for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			ids = pDM->GetCellData( _T("�ڵ�") , i , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ConvertCodeToDescSort") );

			ids = ConvertMailClassCodeToDesc( sCode , sDesc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ConvertCodeToDescSort") );

			if( sDesc.IsEmpty() ) sDesc = sCode;

			ids = pDM->SetCellData( _T("����") , sDesc , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ConvertCodeToDescSort") );

		}
	}
	else if( sAlias.Compare( _T("�Ҽ��ڷ��") ) == 0 )
	{
		sSortAlias = _T("�ڵ�");

		for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			ids = pDM->GetCellData( _T("�ڵ�") , i , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ConvertCodeToDescSort") );

			ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�ڷ�Ǳ���") ,  sCode , sDesc );
//			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ConvertCodeToDescSort") );

			if( sDesc.IsEmpty() ) sDesc = sCode;
			sDesc += _T("(")+sCode+_T(")");

			ids = pDM->SetCellData( _T("����") , sDesc , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ConvertCodeToDescSort") );

		}

	}
	else if( sAlias.Compare( _T("����") ) == 0 )
	{
		sSortAlias = _T("�ڵ�");

		for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			ids = pDM->GetCellData( _T("�ڵ�") , i , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ConvertCodeToDescSort") );

			ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("������������") ,  sCode , sDesc );
//			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ConvertCodeToDescSort") );

			if( sDesc.IsEmpty() ) sDesc = sCode;
			sDesc += _T("(")+sCode+_T(")");

			ids = pDM->SetCellData( _T("����") , sDesc , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ConvertCodeToDescSort") );

		}

	}
	else if( sAlias.Compare( _T("�Ҽ�") ) == 0 )
	{
		sSortAlias = _T("�ڵ�");

		for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			ids = pDM->GetCellData( _T("�ڵ�") , i , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("ConvertCodeToDescSort") );

			ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("����Ҽ�����") ,  sCode , sDesc );
//			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("ConvertCodeToDescSort") );

			if( sDesc.IsEmpty() ) sDesc = sCode;
			sDesc += _T("(")+sCode+_T(")");

			ids = pDM->SetCellData( _T("����") , sDesc , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("ConvertCodeToDescSort") );

		}

	}
	else if( sAlias.Compare( _T("�߼���") ) == 0 || sAlias.Compare( _T("�߼ۿ�") ) == 0 || sAlias.Compare( _T("�߼۳�") ) == 0 
		|| sAlias.Compare( _T("����") ) == 0 )
	{
		sSortAlias = _T("����");

		for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			ids = pDM->GetCellData( _T("�ڵ�") , i , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("ConvertCodeToDescSort") );

			sDesc = sCode;

			ids = pDM->SetCellData( _T("����") , sDesc , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("ConvertCodeToDescSort") );

		}

	}

	ids = pDM->SORT( sSortAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("ConvertCodeToDescSort") ); 

	return 0;

EFS_END

return -1;

}

INT CBO_LOC_3841::GetResultIdx( CString sColField , CString sRowField )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SumCal") );

	CString sTmpCol , sTmpRow;
	for( INT i = 0 ; i < pResultDM->GetRowCount() ; i++ )
	{
		ids = pResultDM->GetCellData( _T("��") , i , sTmpCol );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetResultIdx") );

		ids = pResultDM->GetCellData( _T("��") , i , sTmpRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetResultIdx") );

		if( (sTmpCol.Compare( sColField ) == 0) && (sTmpRow.Compare(sRowField) == 0) ) 
		{
			return i;
		}

	}


return -1;

EFS_END

return -1;
}


INT CBO_LOC_3841::GetSumInfo( INT nMode , CString sField , CString sResultAlias , CString &sInputResult )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSumInfo") );


	CString sColAlias;

	if( nMode == 0 ) 
		sColAlias = _T("��");
	else
		sColAlias = _T("��");

	CString sTmpField;
	CString sCheckYCnt , sCheckNCnt , sSendNCnt;
	CString sSendCnt,sSendYCnt;
	CString sTmpCheckYCnt , sTmpCheckNCnt , sTmpSendNCnt;
	CString sTmpSendCnt,sTmpSendYCnt;
	for( INT i = 0 ; i < pResultDM->GetRowCount() ; i++ )
	{
		ids = pResultDM->GetCellData( sColAlias , i , sTmpField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetSumInfo") );

		if( sTmpField.Compare(sField) == 0 )
		{
			ids = pResultDM->GetCellData( _T("�ѹ߼۰Ǽ�") , i , sTmpSendCnt );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("GetSumInfo") ); 
			ids = pResultDM->GetCellData( _T("�߼ۼ۰��Ǽ�") , i , sTmpSendYCnt);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("GetSumInfo") ); 
			ids = pResultDM->GetCellData( _T("���ŰǼ�") , i , sTmpCheckYCnt );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("GetSumInfo") ); 
			ids = pResultDM->GetCellData( _T("�̼��ŰǼ�") , i , sTmpCheckNCnt);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("GetSumInfo") ); 
			ids = pResultDM->GetCellData( _T("�߼۽��аǼ�") , i ,sTmpSendNCnt  );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("GetSumInfo") ); 

			sSendCnt.Format( _T("%d") , _ttoi(sSendCnt)+_ttoi(sTmpSendCnt) );
			sSendYCnt.Format( _T("%d") , _ttoi(sSendYCnt)+_ttoi(sTmpSendYCnt) );
			sCheckYCnt.Format( _T("%d") , _ttoi(sCheckYCnt)+_ttoi(sTmpCheckYCnt) );
			sCheckNCnt.Format( _T("%d") , _ttoi(sCheckNCnt)+_ttoi(sTmpCheckNCnt) );
			sSendNCnt.Format( _T("%d") , _ttoi(sSendNCnt)+_ttoi(sTmpSendNCnt) );

		}
	}
	sInputResult = sResultAlias;

	if( sSendCnt.IsEmpty() ) sSendCnt = _T("0");
	if( sSendYCnt.IsEmpty() ) sSendYCnt = _T("0");
	if( sCheckYCnt.IsEmpty() ) sCheckYCnt = _T("0");
	if( sCheckNCnt.IsEmpty() ) sCheckNCnt = _T("0");
	if( sSendNCnt.IsEmpty() ) sSendNCnt = _T("0");

	sInputResult.Replace( _T("%�ѹ߼۰Ǽ�%") , sSendCnt );
	sInputResult.Replace( _T("%�߼ۼ����Ǽ�%") , sSendYCnt );
	sInputResult.Replace( _T("%���ŰǼ�%") , sCheckYCnt );
	sInputResult.Replace( _T("%�̼��ŰǼ�%") , sCheckNCnt );
	sInputResult.Replace( _T("%�߼۽��аǼ�%") , sSendNCnt );

	return 0;

EFS_END

return -1;

}


INT CBO_LOC_3841::GetTotalSumInfo( CString sResultAlias, CString &sInputResult )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSumInfo") );


	CString sCheckYCnt , sCheckNCnt , sSendNCnt;
	CString sSendCnt,sSendYCnt;
	CString sTmpCheckYCnt , sTmpCheckNCnt , sTmpSendNCnt;
	CString sTmpSendCnt,sTmpSendYCnt;
	for( INT i = 0 ; i < pResultDM->GetRowCount() ; i++ )
	{
		ids = pResultDM->GetCellData( _T("�ѹ߼۰Ǽ�") , i , sTmpSendCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("GetTotalSumInfo") ); 
		ids = pResultDM->GetCellData( _T("�߼ۼ۰��Ǽ�") , i , sTmpSendYCnt);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("GetTotalSumInfo") ); 
		ids = pResultDM->GetCellData( _T("���ŰǼ�") , i , sTmpCheckYCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("GetTotalSumInfo") ); 
		ids = pResultDM->GetCellData( _T("�̼��ŰǼ�") , i , sTmpCheckNCnt);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("GetTotalSumInfo") ); 
		ids = pResultDM->GetCellData( _T("�߼۽��аǼ�") , i ,sTmpSendNCnt  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("GetTotalSumInfo") ); 

		sSendCnt.Format( _T("%d") , _ttoi(sSendCnt)+_ttoi(sTmpSendCnt) );
		sSendYCnt.Format( _T("%d") , _ttoi(sSendYCnt)+_ttoi(sTmpSendYCnt) );
		sCheckYCnt.Format( _T("%d") , _ttoi(sCheckYCnt)+_ttoi(sTmpCheckYCnt) );
		sCheckNCnt.Format( _T("%d") , _ttoi(sCheckNCnt)+_ttoi(sTmpCheckNCnt) );
		sSendNCnt.Format( _T("%d") , _ttoi(sSendNCnt)+_ttoi(sTmpSendNCnt) );

	}
	sInputResult = sResultAlias;

	if( sSendCnt.IsEmpty() ) sSendCnt = _T("0");
	if( sSendYCnt.IsEmpty() ) sSendYCnt = _T("0");
	if( sCheckYCnt.IsEmpty() ) sCheckYCnt = _T("0");
	if( sCheckNCnt.IsEmpty() ) sCheckNCnt = _T("0");
	if( sSendNCnt.IsEmpty() ) sSendNCnt = _T("0");

	sInputResult.Replace( _T("%�ѹ߼۰Ǽ�%") , sSendCnt );
	sInputResult.Replace( _T("%�߼ۼ����Ǽ�%") , sSendYCnt );
	sInputResult.Replace( _T("%���ŰǼ�%") , sCheckYCnt );
	sInputResult.Replace( _T("%�̼��ŰǼ�%") , sCheckNCnt );
	sInputResult.Replace( _T("%�߼۽��аǼ�%") , sSendNCnt );



	return 0;


EFS_END

return -1;

}

HBRUSH CBO_LOC_3841::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
