// MNG_SERVICE_1130.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_SERVICE_1130.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_SERVICE_1130 dialog


CMNG_SERVICE_1130::CMNG_SERVICE_1130(CESL_Mgr* pParent /*=NULL*/)
	: CMNG_SMS_SERVICE_MIDDLE_DIALOG(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_SERVICE_1130)
	m_sCallBack = _T("");
	m_sCallBack2 = _T("");
	//}}AFX_DATA_INIT
}
CMNG_SERVICE_1130::~CMNG_SERVICE_1130()
{
	
}


VOID CMNG_SERVICE_1130::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_SERVICE_1130)
	DDX_Text(pDX, IDC_eCALLBACK, m_sCallBack);
	DDV_MaxChars(pDX, m_sCallBack, 20);
	DDX_Text(pDX, IDC_eCALLBACK2, m_sCallBack2);
	DDV_MaxChars(pDX, m_sCallBack2, 14);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_SERVICE_1130, CDialog)
	//{{AFX_MSG_MAP(CMNG_SERVICE_1130)
	ON_WM_CTLCOLOR()  
	ON_BN_CLICKED(IDC_radLIBNAME1, OnradLIBNAME1)
	ON_BN_CLICKED(IDC_radNUMBER1, OnradNUMBER1)
	ON_BN_CLICKED(IDC_radNUMBER2, OnradNUMBER2)
	ON_BN_CLICKED(IDC_radLIBNAME2, OnradLIBNAME2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_SERVICE_1130 message handlers

BOOL CMNG_SERVICE_1130::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = InitESL_Mgr( _T("MNG_SMS_SERVICE_1130") );
	if( ids < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr(MNG_SMS_SERVICE_1130)") );
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	// 2010.12.22 ADD BY PWR : ȸ�Ź�ȣ, �������� PC������ ����
	CString strCfgFilePath = _T("..\\cfg\\SMS_Callback.cfg");
	m_CfgFileApi.SetFile(strCfgFilePath);
	CFileFind ff;
	if(!ff.FindFile(strCfgFilePath))
	{
		FILE *fp = _tfopen(strCfgFilePath, _T("w+b"));
		fputc(0xFF, fp); 
		fputc(0xFE, fp);
		fclose(fp);
		m_CfgFileApi.SetData(_T("ȸ�Ź�ȣ����������"), _T("0"));
		m_CfgFileApi.SetData(_T("ȸ�Ź�ȣ"), _T(""));
		m_CfgFileApi.SetData(_T("����������������"), _T("0"));
		m_CfgFileApi.SetData(_T("��������"), _T(""));
	}
	CString strCallbackTarget;
	m_CfgFileApi.GetData(_T("ȸ�Ź�ȣ����������"), strCallbackTarget);
	if(strCallbackTarget.IsEmpty()) strCallbackTarget = _T("0");
	CString strLibNameTarget;
	m_CfgFileApi.GetData(_T("����������������"), strLibNameTarget);
	if(strLibNameTarget.IsEmpty()) strLibNameTarget = _T("0");
	SetCallBackControl(strCallbackTarget);
	SetLibNameControl(strLibNameTarget);
	CString strCallBack;
	m_CfgFileApi.GetData(_T("ȸ�Ź�ȣ"), strCallBack);
	if(!strCallBack.IsEmpty())
	{
		GetDlgItem(IDC_eCALLBACK3)->SetWindowText(strCallBack);
	}
	CString strLibName;
	m_CfgFileApi.GetData(_T("��������"), strLibName);
	if(!strLibName.IsEmpty())
	{		
		GetDlgItem(IDC_eCALLBACK4)->SetWindowText(strLibName);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CMNG_SERVICE_1130::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}


INT CMNG_SERVICE_1130::ReView()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_SMS_TOTAL_ENV"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ReView") );

	if( pDM->GetRowCount() < 1 ) return 0;

	const INT nCnt = 7;

	CString sGetAlias[nCnt] = 
	{
		_T("�����������뿩��") , _T("�ݳ�����뺸��˻�����") , _T("�����뺸��˻�����") , _T("ȸ����ȭ��ȣ") , _T("��������") , 
		_T("�ݳ����˰�����") , _T("�ݳ���������Ƚ��")
	};

	CString sGetData[nCnt];

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = pDM->GetCellData( sGetAlias[i] , 0 , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ReView") );
	}


	pDM = FindDM(_T("DM_MNG_SMS_TYPE_ENV"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ReView") );

	// 2009.12.15 UPDATE BY PWR : SMS11�߰�
	const INT nCondCnt = 5;
	CString sGetCondition[nCondCnt] =
	{
		_T("SMS01"), _T("SMS02"), _T("SMS03"), _T("SMS04"), _T("SMS11")
	};

	CString sAutoSendYN[nCondCnt];
	for( i = 0 ; i < nCondCnt ; i++ )
	{
		ids = CLocCommonAPI::GetCellData( this , pDM , _T("Ÿ��") , sGetCondition[i] , _T("�ڵ������⿩��") , sAutoSendYN[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ReView") );
	}


	// �������к� �ڵ��߼� ���� �߰�
	MakeGridManageCode();
	

	// �����ֱ�
	if( sGetData[0].Compare(_T("Y")) == 0 )
		((CButton*)GetDlgItem(IDC_chkDIVIDEYN))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_chkDIVIDEYN))->SetCheck(FALSE);

	((CComboBox*)GetDlgItem(IDC_COMBO_RETURN))->SetCurSel(_ttoi(sGetData[1]));
	((CComboBox*)GetDlgItem(IDC_COMBO_DELAY))->SetCurSel(_ttoi(sGetData[2]));
	((CComboBox*)GetDlgItem(IDC_COMBO_DELAY2))->SetCurSel(_ttoi(sGetData[5]));
	((CComboBox*)GetDlgItem(IDC_COMBO_DELAY3))->SetCurSel(_ttoi(sGetData[6]));
	
	
	// 2007.08.13 add by pdw
	// �������к� ������ ȸ�� ���	

	//++2008.10.15 UPDATE BY PWR {{++
	// ���սý��� ����
	CString query;
	query.Format(	_T("SELECT SMS_CALLBACK_PHONE,SMS_LIB_NAME ")
					_T("FROM MN_LIBINFO2_TBL ")
					_T("WHERE MANAGE_CODE=UDF_MANAGE_CODE") );		
//	CString query, value;
//	query.Format( _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
//		          _T(" WHERE GROUP_2='��Ÿ' AND GROUP_3='����' AND CLASS_ALIAS='�������к�����������'"));
//	ids = pDM->GetOneValueQuery( query, value );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ReView") );
//	value.TrimLeft();value.TrimRight();
//	value.MakeUpper();
//
//	// 2. ���� ����
//	if( _T("Y") == value )
//	{
//		query.Format(_T("SELECT SMS_CALLBACK_PHONE,SMS_LIB_NAME ")
//					  _T(" FROM MN_LIBINFO2_TBL ")
//		             _T(" WHERE MANAGE_CODE=(SELECT MANAGE_CODE FROM MN_USER_TBL WHERE REC_KEY=%s)"),m_pInfo->USER_PK);		
//	}
//	else
//	{
//		query.Format(_T("SELECT MAX(SMS_CALLBACK_PHONE),MAX(SMS_LIB_NAME) ")
//					  _T(" FROM MN_LIBINFO2_TBL ")
//					 _T(" WHERE MANAGE_CODE IS NULL OR MANAGE_CODE='MA'") );
//	}
	//--2008.10.15 UPDATE BY PWR --}}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO( pDM->CONNECTION_INFO );
	ids = TmpDM.RestructDataManager( query );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ReView") );

	//LIB_NAME	CALLBACK_PHONE
	GetDlgItem(IDC_eCALLBACK)->SetWindowText( TmpDM.GetCellData(0,0));
	GetDlgItem(IDC_eCALLBACK2)->SetWindowText(TmpDM.GetCellData(0,1));

	const INT nIDCCnt = 5;

	INT nIDC[nIDCCnt] =
	{
		IDC_cName2, IDC_cDefault2, IDC_cBookCnt2, IDC_cBook2, IDC_cSMS11
	};

	for( i = 0 ; i < nIDCCnt; i++ )
	{
		((CButton*)GetDlgItem( nIDC[i] ))->SetCheck( FALSE );

		if( sAutoSendYN[i].Compare(_T("Y")) == 0 )
			((CButton*)GetDlgItem( nIDC[i] ))->SetCheck( TRUE );
	}
	
	// �켱���⿹���뿩��
	CString sValue;
	GetManageValue(_T("����"), _T("����"), _T("�������"), _T("�켱���⿹���뿩��"), sValue);
	if(sValue.IsEmpty())
	{ // ������� �⺻
		sValue = _T("Y");
	}
	if(_T("Y") != sValue)
	{ // �������̸� �ڵ��߼ۿ����� �켱���⿹���뿩�δ� �������� �ʴ´�.
		GetDlgItem(IDC_cSMS11)->ShowWindow(FALSE);
	}

	return 0;

	EFS_END

	return -1;

}

INT CMNG_SERVICE_1130::MakeGridManageCode()
{
EFS_BEGIN
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=
	// �������к� �ڵ��߼� �߰� (050308_����)

	// 1. �������� �ڵ� ��������
	CString query, result;
	query.Format( _T("SELECT REC_KEY FROM MN_USER_TBL WHERE ID = '%s'"), GetUserID() );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_SMS_MANAGE_CODE") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeGridManageCode") );	

	pDM->GetOneValueQuery( query, result );

	query.Format(_T("U.USER_KEY = %s"), result);

	pDM->RefreshDataManager(query);

	INT nCnt = pDM->GetRowCount();

	// 2. �ڵ��߼� ���ϴ� ����Ʈ ��������
	CString sNotAutoSend_ManageCode;
	GetManageCodeList(sNotAutoSend_ManageCode);

	// 3. ��뿩�� Setting
	CString sTmp;
	INT nIndex;
	for( INT i=0; i<nCnt; i++)
	{
		sTmp = pDM->GetCellData(_T("�ڵ�"), i );
		nIndex = sNotAutoSend_ManageCode.Find(sTmp);

		// ����Ʈ�� ������ ������
		if ( nIndex >= 0 )
			pDM->SetCellData(_T("��뿩��"), _T("������"),i);
		else 
			pDM->SetCellData(_T("��뿩��"), _T("�����"),i);
	}

	// 4. �׸��� Display
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_SMS_SERVICE_1130") , _T("GRID_MANAGECODE") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeGridManageCode") );	
	pGrid->Display();	

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=
	return 0;

EFS_END

	return -1;
}

INT CMNG_SERVICE_1130::GetManageCodeList(CString& sResult)
{
EFS_BEGIN
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_SMS_TOTAL_ENV") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageCodeList") );	

	sResult = pDM->GetCellData(_T("�ڵ��߼۰����ڵ�"), 0);

	return 0;
EFS_END
	return -1;
}


// TempDM�� ����
INT CMNG_SERVICE_1130::Save( CESL_DataMgr* pTypeEnvDM , CESL_DataMgr* pTotalEnvDM )
{
	EFS_BEGIN
		
	const INT nCnt = 11;
	INT nIDC[nCnt] =
	{
		IDC_chkDIVIDEYN , IDC_COMBO_RETURN , IDC_COMBO_DELAY , IDC_eCALLBACK , IDC_eCALLBACK2 , 
		IDC_cName2 , IDC_cDefault2 , IDC_cBookCnt2 , IDC_cBook2	, IDC_cSMS11, IDC_COMBO_DELAY2,

	};



	// 1. ������ ������ ��� ����
	CString sDivideYN;
	if( ((CButton*)GetDlgItem( nIDC[0] ))->GetCheck() == 1 )
		sDivideYN = _T("Y");
	else
		sDivideYN = _T("N");

	pTotalEnvDM->SetCellData(  _T("�����������뿩��") , sDivideYN , 0  );

	// 2. �ݳ� ����ڿ� �߼۽ñ� , ��ü�ڿ� �߼۽ñ�
	CString sSendtimeReturn , sSendtimeDelay , sRetryPeriodDay , sRetryLimitCnt;

	CComboBox* pCombo = (CComboBox*)GetDlgItem( nIDC[1] );
	pCombo->GetLBText( pCombo->GetCurSel() , sSendtimeReturn);
	pCombo = (CComboBox*)GetDlgItem( nIDC[2] );
	pCombo->GetLBText( pCombo->GetCurSel() , sSendtimeDelay );
	pCombo = (CComboBox*)GetDlgItem( nIDC[10] );
	pCombo->GetLBText( pCombo->GetCurSel() , sRetryPeriodDay );
	pCombo = (CComboBox*)GetDlgItem( IDC_COMBO_DELAY3 );
	pCombo->GetLBText( pCombo->GetCurSel() , sRetryLimitCnt );
	
	pTotalEnvDM->SetCellData(  _T("�ݳ�����뺸��˻�����") , sSendtimeReturn , 0  );
	pTotalEnvDM->SetCellData(  _T("�����뺸��˻�����") , sSendtimeDelay , 0  );
	pTotalEnvDM->SetCellData(  _T("�ݳ����˰�����") , sRetryPeriodDay , 0  );
	pTotalEnvDM->SetCellData(  _T("�ݳ���������Ƚ��") , sRetryLimitCnt , 0  );
	
	// 3. ȸ�Ź�ȣ , �������� 
 	CString sCallBackNo , sLibName;
 	GetDlgItem( nIDC[3] )->GetWindowText( sCallBackNo );
 	GetDlgItem( nIDC[4] )->GetWindowText( sLibName );
 	pTotalEnvDM->SetCellData(  _T("ȸ����ȭ��ȣ") , sCallBackNo , 0  );
 	pTotalEnvDM->SetCellData(  _T("��������") , sLibName , 0  );

	// 2010.12.22 ADD BY PWR : ȸ�Ź�ȣ, �������� PC������ ����
	CString strCfgFilePath = _T("..\\cfg\\SMS_Callback.cfg");
	m_CfgFileApi.SetFile(strCfgFilePath);
	if(TRUE == ((CButton*)GetDlgItem(IDC_radNUMBER1))->GetCheck())
	{
		m_CfgFileApi.SetData(_T("ȸ�Ź�ȣ����������"), _T("0"));
	}
	else
	{
		CString strCallBack;
		GetDlgItem(IDC_eCALLBACK3)->GetWindowText(strCallBack);
		if(strCallBack.IsEmpty()) 
		{
			MessageBox(_T("ȸ�Ź�ȣ�� �Է��� �ֽʽÿ�."), _T("��������"), MB_ICONINFORMATION);
			GetDlgItem(IDC_eCALLBACK3)->SetFocus();
			return 1;
		}
		CString sChars = _T("0123456789-");
		for(INT i = 0; i < strCallBack.GetLength(); i++)
		{
			BOOL bExistence = FALSE;
			for(INT j = 0; j < sChars.GetLength(); j++) 
			{
			   if(strCallBack.GetAt(i) == sChars.GetAt(j))
			   {
				   bExistence = TRUE;
			   }
			}
			if(FALSE == bExistence)
			{
				MessageBox(_T("ȸ�Ź�ȣ�� ���ڿ� ������(-)���θ� �Է��Ͻʽÿ�."), _T("��������"), MB_ICONINFORMATION);
				GetDlgItem(IDC_eCALLBACK3)->SetFocus();
				return 1;
			}
		}
		m_CfgFileApi.SetData(_T("ȸ�Ź�ȣ����������"), _T("1"));
		m_CfgFileApi.SetData(_T("ȸ�Ź�ȣ"), strCallBack);
	}
	if(TRUE == ((CButton*)GetDlgItem(IDC_radLIBNAME1))->GetCheck())
	{
		m_CfgFileApi.SetData(_T("����������������"), _T("0"));
	}
	else
	{
		CString strLibName;
		GetDlgItem(IDC_eCALLBACK4)->GetWindowText(strLibName);
		if(strLibName.IsEmpty()) 
		{
			MessageBox(_T("���������� �Է��� �ֽʽÿ�."), _T("��������"), MB_ICONINFORMATION);
			GetDlgItem(IDC_eCALLBACK4)->SetFocus();
			return 1;
		}
		m_CfgFileApi.SetData(_T("����������������"), _T("1"));
		m_CfgFileApi.SetData(_T("��������"), strLibName);
	}

	// 4. �ڵ� ���� ������ ��� ����
	CString sType[5] =
	{
		_T("SMS01"), _T("SMS02"), _T("SMS03"), _T("SMS04"), _T("SMS11")
	};
	CString sCheck;
	for( INT i = 0 ; i < 5 ; i++ )
	{
		if( ((CButton*)GetDlgItem( nIDC[i+5] ))->GetCheck() == 1 )
			sCheck = _T("Y");
		else
			sCheck = _T("N");

		CLocCommonAPI::SetCellData( this , pTypeEnvDM , _T("Ÿ��") , sType[i] , _T("�ڵ������⿩��") , sCheck );
	}

	// 5. �������к� �ڵ� ���� ������ ��� ����
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_SMS_MANAGE_CODE") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("Save") );
	
	CString sUseYN, sManageCodeList = _T("");
	INT nRowCnt = pDM->GetRowCount();
	for( i = 0; i< nRowCnt; i++ )
	{
		sUseYN = pDM->GetCellData(_T("��뿩��"), i);
		if ( _T("������") == sUseYN )
		{
			CString sCode = pDM->GetCellData(_T("�ڵ�"), i);
			sManageCodeList += sCode + _T(" ");
		}
	}	


	sManageCodeList.TrimRight();
	INT ids = pTotalEnvDM->SetCellData(_T("�ڵ��߼۰����ڵ�"), sManageCodeList, 0);
	
	return 0;
	
	EFS_END
		
	return -1;
	
}



BEGIN_EVENTSINK_MAP(CMNG_SERVICE_1130, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_SERVICE_1130)
	ON_EVENT(CMNG_SERVICE_1130, IDC_gMANAGE_CODE, -601 /* DblClick */, OnDblClickgMANAGECODE, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_SERVICE_1130::OnDblClickgMANAGECODE() 
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_SMS_SERVICE_1130") , _T("GRID_MANAGECODE") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnDblClickgMANAGECODE") );

	INT nRow = pGrid->GetMouseRow();
	INT nCol = pGrid->GetMouseCol();

	if( nRow < 1 ) return ;
	if( nCol != 3 ) return ;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_SMS_MANAGE_CODE") );
	if( pDM == NULL ) return;


	CString sUseYN = pDM->GetCellData(_T("��뿩��"), nRow-1);

	if ( _T("������") == sUseYN )
		pDM->SetCellData(_T("��뿩��"), _T("�����"), nRow-1 );
	else if ( _T("�����") == sUseYN )
		pDM->SetCellData(_T("��뿩��"), _T("������"), nRow-1 );

	pGrid->Display();

	
}

HBRUSH CMNG_SERVICE_1130::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

INT CMNG_SERVICE_1130::SetCallBackControl(CString strCallbackTarget)
{
	INT nMode = _ttoi(strCallbackTarget);
	if(0 == nMode)
	{
		OnradNUMBER1();
	}
	else if(1 == nMode)
	{
		OnradNUMBER2();
	}
	else
	{
		OnradNUMBER1();
	}

	return 0;
}

INT CMNG_SERVICE_1130::SetLibNameControl(CString strCallbackTarget)
{
	INT nMode = _ttoi(strCallbackTarget);
	if(0 == nMode)
	{
		OnradLIBNAME1();
	}
	else if(1 == nMode)
	{
		OnradLIBNAME2();
	}
	else
	{
		OnradLIBNAME1();
	}

	return 0;
}

void CMNG_SERVICE_1130::OnradNUMBER1() 
{
	((CButton*)GetDlgItem(IDC_radNUMBER1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radNUMBER2))->SetCheck(FALSE);
	GetDlgItem(IDC_eCALLBACK)->EnableWindow(TRUE);
	GetDlgItem(IDC_eCALLBACK3)->EnableWindow(FALSE);
}

void CMNG_SERVICE_1130::OnradNUMBER2() 
{
	((CButton*)GetDlgItem(IDC_radNUMBER2))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radNUMBER1))->SetCheck(FALSE);
	GetDlgItem(IDC_eCALLBACK3)->EnableWindow(TRUE);
	GetDlgItem(IDC_eCALLBACK)->EnableWindow(FALSE);
}

void CMNG_SERVICE_1130::OnradLIBNAME1() 
{
	((CButton*)GetDlgItem(IDC_radLIBNAME1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radLIBNAME2))->SetCheck(FALSE);
	GetDlgItem(IDC_eCALLBACK2)->EnableWindow(TRUE);
	GetDlgItem(IDC_eCALLBACK4)->EnableWindow(FALSE);
}

void CMNG_SERVICE_1130::OnradLIBNAME2() 
{
	((CButton*)GetDlgItem(IDC_radLIBNAME2))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radLIBNAME1))->SetCheck(FALSE);
	GetDlgItem(IDC_eCALLBACK4)->EnableWindow(TRUE);
	GetDlgItem(IDC_eCALLBACK2)->EnableWindow(FALSE);
}

