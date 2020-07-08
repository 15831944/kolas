// CO_CENTER_USER_MANAGER_02.cpp : implementation file
//

#include "stdafx.h"
#include "CO_CENTER_USER_MANAGER_02.h"

#include "CO_CENTER_USER_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_MANAGER_02 dialog
// IndexToolkit
#include "..\..\..\..\����\IndexToolKit\Index.h"


CCO_CENTER_USER_MANAGER_02::CCO_CENTER_USER_MANAGER_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_MANAGER_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CCO_CENTER_USER_MANAGER_02::~CCO_CENTER_USER_MANAGER_02()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CCO_CENTER_USER_MANAGER_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_MANAGER_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_MANAGER_02, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_MANAGER_02)
	ON_BN_CLICKED(IDC_btnFIND, OnbtnFIND)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnSEARCH_CLEAR, OnbtnSEARCHCLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_MANAGER_02 message handlers

BOOL CCO_CENTER_USER_MANAGER_02::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("CO_CENTER_USER_MANAGER_02")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCO_CENTER_USER_MANAGER_02::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
		
		// TODO: Add your specialized code here and/or call the base class
		
		return CDialog::Create(IDD, pParentWnd);
	
	EFS_END
		return FALSE;
	
}

VOID CCO_CENTER_USER_MANAGER_02::OnbtnFIND() 
{
	EFS_BEGIN

	AfxGetApp()->DoWaitCursor(1);

	CString where;
	MakeWhere( where );
	
	CCO_CENTER_USER_MANAGER_01 *pParent = ( CCO_CENTER_USER_MANAGER_01 * )pParentMgr;
	
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );
//	pParent->pRemoteDBMgr->SetFetchCount( 1001, 100 );
///	pParent->pRemoteDBMgr->LOAN_Select( pDM, where, _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST"), 1, 1 );
	
	ConvertCivilNum( pDM, _T("�ֹι�ȣ_VIEW") );
	
	// 2009.10.08 ADD BY KSJ : �����ڹ�ȣ�� �ֹι�ȣ�� ��� ó��
	CString strValue1;
	INT ids = GetManageValue( _T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), strValue1 );
	if( 0 > ids ) ERROR_MSG_RETURN_VOID2( -1010 , _T("OnbtnFIND") );
	strValue1.TrimLeft();strValue1.TrimRight();
	strValue1.MakeUpper();
	
	INT i , nRowCnt = pDM->GetRowCount();
	CString strUserNo;
	for( i=0 ; i<nRowCnt ; i++ )
	{
		if( _T("Y") != strValue1 )
		{
			ids = pDM->GetCellData( _T("�����ڹ�ȣ"), i , strUserNo );
			if( 0 > ids ) ERROR_MSG_RETURN_VOID2( -1020 , _T("OnbtnFIND") );
			// 2010.09.03 ADD BY KSJ : �����ڹ�ȣ ǥ�ù�� ����
//DEL 			CString strBuf = strUserNo;
//DEL 			if( 13 <= strUserNo.GetLength() )
//DEL 			{
//DEL 				strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
//DEL 			}
//DEL 			ids = CLocCommonAPI::CivilNoCheck( strBuf );
//DEL 			if( 0 > ids ) ERROR_MSG_RETURN_VOID2( -1030 , _T("OnbtnFIND") );
//DEL 			if( 0 == ids )
//DEL 			{
//DEL 				// 2009.12.14 ADD BY CJY : �ֹι�ȣ ǥ�ù�� ����
//DEL 				strUserNo.Format(_T("%s%s******"),strBuf.Left(6),strBuf.Mid(7,1));
//DEL 				if( 0 > ids ) ERROR_MSG_RETURN_VOID2( -1040 , _T("OnbtnFIND") );
//DEL 			}
			CLocCommonAPI::ChangeSecuriyUserNo(this,strUserNo,m_sIsUnityLoanService);
			ids = pDM->SetCellData( _T("�����ڹ�ȣ_VIEW"), strUserNo , i );
			if( 0 > ids ) ERROR_MSG_RETURN_VOID2( -1050 , _T("OnbtnFIND") );
		}
		// 2010.04.08 ADD BY PWR : �������� �߰�
		else
		{
			ids = pDM->GetCellData( _T("�����ڹ�ȣ"), i , strUserNo );
			if( 0 > ids ) ERROR_MSG_RETURN_VOID2( -1020 , _T("OnbtnFIND") );
			ids = pDM->SetCellData( _T("�����ڹ�ȣ_VIEW"), strUserNo , i );
			if( 0 > ids ) ERROR_MSG_RETURN_VOID2( -1050 , _T("OnbtnFIND") );
		}
	}

	ControlDisplay( _T("CM_CO_CENTER_USER_MANAGER_01"), _T("�����̿��������׸���") );

	AfxGetApp()->DoWaitCursor(-1);
	
	if( 0 == pDM->GetRowCount() )
	{
		AfxMessageBox(_T("�̿��ڰ� �������� �ʽ��ϴ�."));
	}
	else if( 1001 == pDM->GetRowCount() )
	{
		AfxMessageBox( _T("�˻������ 1000���� �Ѿ����ϴ�.\r\n�˻������� �� �־ �˻��� �ֽʽÿ�.") );
	}

	/*
	AfxGetApp()->DoWaitCursor(1);

	SearchCenterUser();	

	AfxGetApp()->DoWaitCursor(-1);
	*/

	EFS_END
}

/// ���� �̿��� �˻�
INT CCO_CENTER_USER_MANAGER_02::SearchCenterUser()
{
	EFS_BEGIN

	INT ids;

	CString sWhere;

	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchCenterUser") );

	ids = GetWhereSql(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchCenterUser") );
	if( ids > 0 ) return ids;

	if( sWhere.IsEmpty() )
	{
		AfxMessageBox(_T("������ �Է��Ͻð� �˻��Ͻʽÿ�"));
		return 1;
	}

	pDM->m_dbkind = 4;
	ids = pDM->RefreshDataManager(sWhere);	
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchCenterUser") );

	// �������ڵ� -> ����
	ids = LibCodToDesc();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchCenterUser") );

	ids = AllControlDisplay( _T("CM_CO_CENTER_USER_MANAGER_01") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchCenterUser") );

	return 0;

	EFS_END

	return -1;
}

/// WHERE�� �����ϱ�
INT CCO_CENTER_USER_MANAGER_02::GetWhereSql( CString &sWhere )
{
	EFS_BEGIN

	INT ids;

	const INT nControlCnt = 8;

	CString sCMAlias = _T("CM_CO_CENTER_USER_MANAGER_02");

	CString sControlAlias[nControlCnt] =
	{
		_T("�����ڹ�ȣ1") , _T("�����ڹ�ȣ2") , _T("�����ڸ�1") , _T("�����ڸ�2") , _T("�ֹε�Ϲ�ȣ") , 
		_T("�������1") , _T("�������2") , _T("E_MAIL")	
	};

	TCHAR* sSearchFieldName[nControlCnt] =
	{
		_T("CMLU.USER_NO") , _T("CMLU.USER_NO") , _T("CMLU.NAME") , _T("CMLU.NAME") , _T("CMLU.CIVIL_NO") , 
		_T("CLU.REG_DATE") , _T("CLU.REG_DATE") , _T("CMLU.E_MAIL")	
	};

	INT nSearchType[nControlCnt] =
	{
		TWOSTRING , ETC , TWOSTRING , ETC , ONELIKESTRING , 
		TWODATE , ETC , ONELIKESTRING
	};

	CString sGetData[nControlCnt];

	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sControlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetWhereSql") );
	}

	ids = CLocCommonAPI::MakeSql( sGetData , sSearchFieldName , nSearchType , nControlCnt , sWhere , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetWhereSql") );


	CString sLibName , sLibKey;

	ids = GetControlData( sCMAlias , _T("��������") , sLibName , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetWhereSql") );

	if( sLibName.Compare(_T("��ü")) == 0 ) return 0;

	CESL_DataMgr* pLibInfoDM = FindDM( _T("DM_CO_LOCAL_LIB_INFO") );
	if( pLibInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetWhereSql") );

	ids = CLocCommonAPI::GetCellData( this , pLibInfoDM , _T("�������̸�") , sLibName , _T("������KEY") , sLibKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("GetWhereSql") );

	if( !sLibKey.IsEmpty() )
	{	
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");

		sWhere += _T(" CLU.LIB_KEY = ") + sLibKey;
	}

	return 0;


	EFS_END

	return -1;
}


/// ȭ�� ����
/// ��Ʈ�� �ʱ�ȭ
INT CCO_CENTER_USER_MANAGER_02::InitControl()
{
	EFS_BEGIN

	/*
	m_libCombo = new CJComboBox;
	m_libCombo->SubclassDlgItem( IDC_cmbLIBNAME, ( CWnd * )this );
	m_libCombo->FormatList( 2, DT_LEFT, false , EDGE );
	m_libCombo->RequireFlag = FALSE;
	
	m_libCombo->AddItem( _T("-"), 0, 0 );
	m_libCombo->AddItem( _T("��ü"), 1, 0 );
	
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_LOCAL_LIB_INFO") );
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		m_libCombo->AddItem( pDM->GetCellData( _T("������CODE"), i ), 0, i + 1 );
		m_libCombo->AddItem( pDM->GetCellData( _T("�������̸�"), i ), 1, i + 1 );
	}
	m_libCombo->SetCurSel( 0 );
	*/


	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbLIBNAME );
	pCombo->InsertString( 0, _T("��ü") );

	CESL_DataMgr* pDM = FindDM( _T("DM_CO_LOCAL_LIB_INFO") );
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
		pCombo->InsertString( i + 1, pDM->GetCellData( _T("�������̸�"), i ) );
	pCombo->SetCurSel( 0 );
	
	
	/// �������� ���� ����
	/*
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbLIBNAME);

	pCombo->ResetContent();

	pCombo->AddString(_T("��ü"));

	CESL_DataMgr* pLibInfoDM = FindDM( _T("DM_CO_LOCAL_LIB_INFO") );
	if( pLibInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitControl") );

	CString sLibName;
	for( INT i = 0 ; i < pLibInfoDM->GetRowCount() ; i++ )
	{
		ids = pLibInfoDM->GetCellData( _T("�������̸�") , i , sLibName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitControl") );

		pCombo->AddString(sLibName);
	}

	pCombo->SetCurSel(0);
	*/
	
	return 0;

	EFS_END

	return -1;
}

VOID CCO_CENTER_USER_MANAGER_02::OnbtnCLOSE() 
{
	OnCancel();	
}


/// �������� �ڵ� -> ����
INT CCO_CENTER_USER_MANAGER_02::LibCodToDesc()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr* pLibInfoDM = FindDM( _T("DM_CO_LOCAL_LIB_INFO") );
	if( pLibInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("LibCodToDesc") );
	
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("LibCodToDesc") );

	TCHAR* sAlias[2] =
	{
		_T("����������") , _T("��������������")
	};

	CString sLibCode;
	CString sLibName;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		for( INT j = 0 ; j < 2 ; j++ )
		{
			ids = pDM->GetCellData( sAlias[j] , i , sLibCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("LibCodToDesc") );
			
			ids = CLocCommonAPI::GetCellData( this , pLibInfoDM , _T("������CODE") , sLibCode , _T("�������̸�") , sLibName );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("LibCodToDesc") );
			
			ids = pDM->SetCellData( sAlias[j] , sLibName , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("LibCodToDesc") );
		}
	}

	return 0;

	EFS_END

	return -1;
}


BOOL CCO_CENTER_USER_MANAGER_02::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		OnbtnFIND();
		return TRUE;
	}
		
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

VOID CCO_CENTER_USER_MANAGER_02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	
}

VOID CCO_CENTER_USER_MANAGER_02::MakeWhere( CString &where )
{
/*
	alias[ 0 ] = _T("�����ڸ�1");
	alias[ 1 ] = _T("�����ڸ�2");
	alias[ 2 ] = _T("�����ڹ�ȣ1");
	alias[ 3 ] = _T("�����ڹ�ȣ2");
	alias[ 4 ] = _T("�������1");
	alias[ 5 ] = _T("�������2");
	alias[ 6 ] = _T("�ֹε�Ϲ�ȣ");
	alias[ 7 ] = _T("E_MAIL");
*/	
	const INT cnt = 10;
	CString ctrlAlias[ cnt ];
	GetCtrlAlias( ctrlAlias );
	CString ctrlData[ cnt ];
	GetCtrlData( ctrlData, ctrlAlias );

	CString tmp;
	CString field[] = {
		_T("IDX_NAME"),		//	0
		_T("USER_NO"),		//	1
		_T("REG_DATE"),		//	2
		_T("CIVIL_NO"),		//	3
		_T("E_MAIL") };		//	4
	
	CIndex index;
	index.MakeIndex( ctrlData[ 0 ] );
	index.MakeIndex( ctrlData[ 1 ] );
		
	INT idx = 0;
	// ���� ����
	for( INT i = 0; i < 3; i ++ )
	{
		idx = i * 2;
		
		if( ctrlData[ idx ].IsEmpty() )
			continue;
		else if( !ctrlData[ idx  + 1 ].IsEmpty() )
			tmp.Format( _T(" %s BETWEEN '%s' AND '%s' AND"), field[ i ], ctrlData[ idx ], ctrlData[ idx + 1 ] );
		else
			switch( i )
			{
				case 0 :
					tmp.Format( _T(" %s LIKE '%s%%' AND"), field[ i ], ctrlData[ idx ] );
					break;
				case 1 :
				case 2 :
					tmp.Format( _T(" %s = '%s' AND"), field[ i ], ctrlData[ idx ] );
					break;
				default :
					break;
			}

		where += tmp;
	}

	CString sBuf;
	if( !ctrlData[ 6 ].IsEmpty() )
	{
		TCHAR cTmp;
		for( INT i = 0; i < ctrlData[ 6 ].GetLength(); i++ )
		{
			cTmp = ctrlData[ 6 ].GetAt( i );

			if( cTmp > 47 && cTmp < 58 )
				sBuf += cTmp;
		}

		if( 13 == sBuf.GetLength() )
			tmp.Format( _T(" %s IN( '%s', '%s-%s' ) AND"), field[ 3 ], sBuf, sBuf.Left( 6 ), sBuf.Right( 7 ) );
		else
			tmp.Format( _T(" %s LIKE '%%%s%%' ) AND"), field[ 3 ], ctrlData[ 6 ] );
		
		where += tmp;
	}

	if( !ctrlData[ 7 ].IsEmpty() )
	{
		// 2009.03.26 UPDATE BY CJY : �����ܰ˻����� ����
// 		tmp.Format( _T(" %s = '%s' AND"), field[ 4 ], ctrlData[ 7 ] );
		tmp.Format( _T(" %s LIKE '%s%%' AND"), field[ 4 ], ctrlData[ 7 ] );
		where += tmp;
	}

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbLIBNAME );
	INT curSel = pCombo->GetCurSel();
	if( curSel )
	{
		CESL_DataMgr* pDM = FindDM( _T("DM_CO_LOCAL_LIB_INFO") );
		tmp.Format( _T(" MAKE_LIB_CODE = '%s' AND"), pDM->GetCellData( _T("������CODE"), curSel - 1 ) );
		where += tmp;
	}

	/// ���� ������
	if( !ctrlData[8].IsEmpty() )
	{
		if( !ctrlData[9].IsEmpty() )
			tmp.Format( _T(" LAST_UPDATE_DATE BETWEEN '%s' AND '%s' AND") , ctrlData[8] , ctrlData[9] );
		else
			tmp.Format( _T(" LAST_UPDATE_DATE = '%s' AND") , ctrlData[8] );
		where += tmp;
	}
	
	if( _T("AND") == where.Right( 3 ) )
	{
		tmp = where.Left( where.GetLength() - 3 );
		where = tmp;
	}
}

VOID CCO_CENTER_USER_MANAGER_02::GetCtrlAlias( CString alias[] )
{
	
	alias[ 0 ] = _T("�����ڸ�1");
	alias[ 1 ] = _T("�����ڸ�2");
	alias[ 2 ] = _T("�����ڹ�ȣ1");
	alias[ 3 ] = _T("�����ڹ�ȣ2");
	alias[ 4 ] = _T("�������1");
	alias[ 5 ] = _T("�������2");
	alias[ 6 ] = _T("�ֹε�Ϲ�ȣ");
	alias[ 7 ] = _T("E_MAIL");
	alias[ 8 ] = _T("����������1");
	alias[ 9 ] = _T("����������2");

}
CString CCO_CENTER_USER_MANAGER_02::TakeControlData( CString alias, INT option )
{
	// option == 0 : ��Ƽ�޺��� �ڵ尪�� �����´�. 
	// option == 1 : ��Ƽ�޺��� �ڵ弳���� �����´�. 
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_CO_CENTER_USER_MANAGER_02"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_CO_CENTER_USER_MANAGER_02"), _T("K2Up") );
		AfxMessageBox( alias );
		return _T("");
	}
	
	// �ڵ尪�� _T("0") �̰ų� ������ _T("�������")�϶� _T("")�� ��ȯ�Ѵ�. 
	switch( option )
	{
		case 0 :
			if( _T("0") == result )
				return _T("");
			break;
		case 1 :
			if( _T("�������") == result )
				return _T("");
			break;
		default :
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_CO_CENTER_USER_MANAGER_02"), _T("K2Up") );
			return _T("");
			break;
	}

	result.TrimLeft(); result.TrimRight();
	return result;
}

VOID CCO_CENTER_USER_MANAGER_02::GetCtrlData( CString data[], CString alias[] )
{
	const INT cnt = 10;
	for( INT i = 0; i < cnt; i++ )
		data[ i ] = TakeControlData( alias[ i ] );
}

VOID CCO_CENTER_USER_MANAGER_02::ConvertCivilNum( CESL_DataMgr *pDM, CString alias )
{
	// 2009.10.08 UPDATE BY KSJ : �ֹι�ȣ ǥ�� ���� ����
//DEL 	CString civilNum;
//DEL 	INT cnt = pDM->GetRowCount();
//DEL 	for( INT i = 0; i < cnt; i++ )
//DEL 		pDM->SetCellData( alias, ConvertCivilNum( pDM->GetCellData( alias, i ) ), i );
	// 2010.09.03 ADD BY KSJ : ���յ������� ��뿩�� �߰� �۾�
	//DEL INT ids = CLocCommonAPI::ChangeSecuriyCivilNo( this , pDM , -1, alias );
	INT ids = CLocCommonAPI::ChangeSecuriyCivilNo( this , pDM , -1, alias , m_sIsUnityLoanService );
	if( 0 > ids ) return;
}

CString CCO_CENTER_USER_MANAGER_02::ConvertCivilNum( CString civilNum )
{
	// 2010.09.03 ADD BY KSJ : ���յ������� ��뿩�� �߰� �۾�
//DEL 	civilNum.Replace( _T("-"), _T("") );
//DEL 	if( 13 != civilNum.GetLength() )
//DEL 		return civilNum;
//DEL 
//DEL 	CString frontNum = civilNum.Left( 6 );
//DEL 	civilNum.Format( _T("%s-*******"), frontNum );
	INT ids = CLocCommonAPI::ChangeSecuriyCivilNo( this , civilNum , m_sIsUnityLoanService );
	return civilNum;	
}

VOID CCO_CENTER_USER_MANAGER_02::OnbtnSEARCHCLEAR() 
{

	CESL_ControlMgr* pCM = (CESL_ControlMgr*)FindCM(_T("CM_CO_CENTER_USER_MANAGER_02"));
	if( pCM == NULL )
	{

		AfxMessageBox(_T("�ָ�ã��"));
		return;
	}

	INT ids = pCM->AllControlClear();	
	if( ids < 0 ) 
	{
		AfxMessageBox(_T("�� �����°� �ȵ�") );
	}
}

HBRUSH CCO_CENTER_USER_MANAGER_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// ������� �߰�

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
