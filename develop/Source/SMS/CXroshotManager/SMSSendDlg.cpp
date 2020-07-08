// SMSSendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMSSendDlg.h"
#include "..\\CO_SMS\\CfgFileApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSSendDlg dialog

#define SMS_MAXBYTE			2000

CSMSSendDlg::CSMSSendDlg(CESL_Mgr* pParent, 
						 SMS_CONNECTION_INFO &SMS_ConnectionInfo, 
						 CString SMSType, 
						 CString callbackNum, 
						 CStringList &userKey,
						 CStringList &LoanKey,
						 CStringList &name, 
						 CStringList &phoneNum, 
						 CStringList &content,
						 CStringList &SmsUseYN,
						 INT nType)
	: CESL_Mgr( IDD, pParent)
{
	if ( nType == 0 )
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD );
		m_nIDHelp = IDD;
	}
	else
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD_MINI );
		m_nIDHelp = IDD_MINI;
	}

	m_sSMSType = SMSType;
	m_nSMSType = _ttoi( SMSType.Right( 2 ) ) - 1;
	// 17/09/04 ������ : '����>����/�ݳ�>SMS>������������ȳ�' �޴��� �Լ����� �ʴ� ����ڷ� �����͸� ��ȸ��� �߰�
//*/ ADD ---------------------------------------------------------------------------
	if( 8 < m_nSMSType)
	{
		// ������������ȳ����� �߰��� �޽��� SMS18/SMS19�� ���� ���� ó���߰�
		if( m_nSMSType == 17 || m_nSMSType == 18)
		{
			m_nSMSType = 3;
		}
	}
//*/ END ---------------------------------------------------------------------------
	m_callbackNum = callbackNum;
	m_pParent	  = pParent;

	m_name.AddTail( &name );
	m_phoneNum.AddTail( &phoneNum );
	m_content.AddTail( &content );
	m_userKey.AddTail( &userKey );
	m_sLoanKey.AddTail( &LoanKey );
	m_sSmsUseYN.AddTail( &SmsUseYN );
	
	m_SMSType[ 0 ] = _T("�ݳ��뺸��");
	m_SMSType[ 1 ] = _T("�����뺸��");
	m_SMSType[ 2 ] = _T("�����ڷ� ���Ⱑ�� �뺸��");
	// 17/09/04 ������ : '����>����/�ݳ�>SMS>������������ȳ�' �޴��� �Լ����� �ʴ� ����ڷ� �����͸� ��ȸ��� �߰�
/*//BEFORE-----------------------------------------------------------------------------
	m_SMSType[ 3 ] = _T("������������뺸��");
*///AFTER------------------------------------------------------------------------------
	m_SMSType[ 3 ] = _T("��������뺸��");
/*//END-------------------------------------------------------------------------------*/	
	m_SMSType[ 4 ] = _T("��������");
	m_SMSType[ 5 ] = _T("���ȳ�");
	m_SMSType[ 6 ] = _T("�Ű��ȳ�");
	m_SMSType[ 7 ] = _T("��õ�����ȳ�");
	m_SMSType[ 8 ] = _T("�Ϲݹ���");

	m_SMS_ConnectionInfo.sBindId			= SMS_ConnectionInfo.sBindId;
	m_SMS_ConnectionInfo.sBindPwd			= SMS_ConnectionInfo.sBindPwd;
	m_SMS_ConnectionInfo.sUserDomain		= SMS_ConnectionInfo.sUserDomain;
	m_SMS_ConnectionInfo.sUserPhoneId		= SMS_ConnectionInfo.sUserPhoneId;
	m_SMS_ConnectionInfo.sUserAccessIp		= SMS_ConnectionInfo.sUserAccessIp;
	m_SMS_ConnectionInfo.sCfgConFilePath	= SMS_ConnectionInfo.sCfgConFilePath;

	m_pSMSSender = NULL;
	m_pSMSList = NULL;
	m_SMSSending = false;
	m_bSMSStop = false;
		
	// 2006.01.02 ADD BY PDW(Jr)
	m_pGrid    = NULL;
	m_pGridEdt = NULL;
	m_sURL     = _T("");
	m_bUseURL  = FALSE;

	// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	m_nKatokPlus = 0;
	m_pSMS_MSG_INFO = NULL;
	// KOL.RED.2018.224 ---------------------------------------------------------------------------
}

CSMSSendDlg::CSMSSendDlg(CESL_Mgr* pParent): CESL_Mgr( IDD, pParent)
{
	m_pParent = pParent;
}

BOOL CSMSSendDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CSMSSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMSSendDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSSendDlg, CDialog)
	//{{AFX_MSG_MAP(CSMSSendDlg)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE( SMS_SEND_STATUS, ShowSMSSendStatus )
	ON_MESSAGE( UPDATE_GRIDTEXT, OnUpdateGridtext )
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSSendDlg message handlers

BOOL CSMSSendDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("SMSSendDlg") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SMSSendDlg") );
		return false;
	}
	
	// 2006.01.02 ADD BY PDW(Jr) : �߼۸�� �׸���
	m_pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	if ( !m_pGrid ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CSMSSendDlg::OnInitDialog()") );

	//JOB.2019.0017 ADD BY KYJ
	//�������� ���� ����,�α� ������ ����
	//-------------------------------------------------------------------------
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		((CESL_Grid*)m_pGrid)->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		((CESL_Grid*)m_pGrid)->m_bPrivacyPrintLogRecord = TRUE;
		((CESL_Grid*)m_pGrid)->m_strPrintInfo = _T("SMS �߼�");
	}
	//-------------------------------------------------------------------------

	SMSInit();

	// 2006.01.04 ADD BY PDW(Jr) : URL ����Ʈ ǥ���� ���� �߰� ����
	SetMSG_GB();

	SetEcoMailUseYN();	
	ViewCommonInfo();
	SetSMSList();
	ViewSMSList();

	m_pSMSList = new SMS_LIST;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;
}

VOID CSMSSendDlg::ViewCommonInfo()
{
	CString msg;
	msg.Format( _T("%s SMS �߼�"), m_SMSType[ m_nSMSType ] );
	this->SetWindowText( msg );
}


VOID CSMSSendDlg::SetSMSList()
{
	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMSSendDlg") );
	if( pDM == NULL ) 
	{
		AfxMessageBox( _T("SetInfoEx Error[-1]") );
		return ;
	}

	// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//// 2006.01.04 UPDATE BY PDW(Jr) : URL ����Ʈ ǥ���� ���� �߰� 
	//const INT cnt = 7;
	//CString alias[ cnt ] = {
	//	_T("�̸�"), _T("�ڵ���"), _T("����"), _T("USER_KEY"), _T("BYTE"), _T("LOAN_KEY"), _T("SMS_USE_YN") };
	const INT cnt = 8;
	CString alias[ cnt ] = {
		_T("�̸�"), _T("�ڵ���"), _T("����"), _T("USER_KEY"), _T("BYTE"), _T("LOAN_KEY"), _T("SMS_USE_YN") , _T("����_ī��") };
	// KOL.RED.2018.224 ---------------------------------------------------------------------------


	CString data[ cnt ];

	POSITION pos = m_name.GetHeadPosition();
	for( INT i = 0; i < m_name.GetCount(); i++ )
	{
		data[ 0 ] = m_name.GetAt( m_name.FindIndex( i ) );
		data[ 1 ] = m_phoneNum.GetAt( m_phoneNum.FindIndex( i ) );
		data[ 2 ] = m_content.GetAt( m_content.FindIndex( i ) );
		data[ 3 ] = m_userKey.GetAt( m_userKey.FindIndex( i ) );	
		data[ 4 ] = GetSMSByte( data[ 2 ] );
		data[ 5 ] = m_sLoanKey.GetAt( m_sLoanKey.FindIndex( i ) );	
		data[ 6 ] = m_sSmsUseYN.GetAt( m_sSmsUseYN.FindIndex( i ) );	
		// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
		if( m_asContent_Katok.GetSize() > i )
		{
			data[ 7 ] = m_asContent_Katok.GetAt(i);
		}
		// KOL.RED.2018.224 ---------------------------------------------------------------------------
		pDM->SetCellData( alias, cnt, data, -1 );
	}
	
	if( m_sEcoMailUseYN.Compare(_T("Y")) != 0 ) return;
	/// ��ġ��� ���� �߼��̶�� ���� �ʴ´�.
	if( m_nSMSType == 3 ) return;

	INT nSize = m_asPositionCode.GetSize();

	const INT nFieldCnt = 6;
	CString sDMFieldAlias[nFieldCnt] =
	{
		_T("�Ҽ�") , _T("����") , _T("ȸ������") , _T("�̿���ID") , _T("����") , _T("�ֹε�Ϲ�ȣ") 
	};

	CString sPositionCode;
	CString sClassCode;
	for( i = 0 ; i < nSize ; i++ )
	{
		sPositionCode = m_asPositionCode.GetAt(i);
		m_pInfo->pCodeMgr->ConvertDescToCode( _T("����Ҽ�����") , sPositionCode , sPositionCode );
		ids = pDM->SetCellData( sDMFieldAlias[0] , sPositionCode ,i );
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("SetInfoEx Error[-2]") );
			return ;
		}

		sClassCode =  m_asClassCode.GetAt(i);
		m_pInfo->pCodeMgr->ConvertDescToCode( _T("������������") , sClassCode , sClassCode );
		ids = pDM->SetCellData( sDMFieldAlias[1] ,sClassCode , i );
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("SetInfoEx Error[-2]") );
			return;
		}

		ids = pDM->SetCellData( sDMFieldAlias[2] ,  m_asInferiorClass.GetAt(i), i );
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("SetInfoEx Error[-2]") );
			return ;
		}

		ids = pDM->SetCellData( sDMFieldAlias[3]  , m_asUserID.GetAt(i) , i);
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("SetInfoEx Error[-2]") );
			return ;
		}

		ids = pDM->SetCellData( sDMFieldAlias[4] , m_asBirthDay.GetAt(i) , i);
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("SetInfoEx Error[-2]") );
			return ;
		}

		ids = pDM->SetCellData( sDMFieldAlias[5] ,  m_asCivilNo.GetAt(i), i );
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("SetInfoEx Error[-2]") );
			return ;
		}
	}
}

VOID CSMSSendDlg::ViewSMSList()
{
	ControlDisplay( _T("CM_SMSSendDlg"), _T("grid") );

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	pGrid->SetRowHeight( 0, 500 );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetTextMatrix( i, 1, _T("V") );
		pGrid->SetRowHeight( i, 700 );
	}
}

VOID CSMSSendDlg::OnOK()
{
	if( m_SMSSending )
		m_bSMSStop = true;
	else
		SendSMSMsg();
	
	SetCloseBtnEnable();
}

INT CSMSSendDlg::MakeSMSList( CStringList &receiverName, CStringList &receiverNum, CStringList &msg, CStringList &userKey, CStringList &loanKey, CStringList &smsUseYN
							// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
							, CStringArray *pmsg_katok
							// KOL.RED.2018.224 ---------------------------------------------------------------------------
							 )
{
	receiverName.RemoveAll();
	receiverNum.RemoveAll();
	msg.RemoveAll();

	CESL_DataMgr *pDM = FindDM( _T("DM_SMSSendDlg") );
	m_gridIdx.RemoveAll();

	// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//// 2009.03.02 UPDATE BY PWR : USER_KEY, LOAN_KEY�߰�
	//const INT cnt = 6;
	//CString alias[ cnt ] = {
	//	_T("�̸�"), _T("�ڵ���"), _T("����"), _T("USER_KEY"), _T("LOAN_KEY"), _T("SMS_USE_YN") };
	const INT cnt = 7;
	CString alias[ cnt ] = {
		_T("�̸�"), _T("�ڵ���"), _T("����"), _T("USER_KEY"), _T("LOAN_KEY"), _T("SMS_USE_YN") , _T("����_ī��") };
	// KOL.RED.2018.224 ---------------------------------------------------------------------------

	CString data[ cnt ];

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	INT i , nRowCnt , SMSCnt = 0;
	nRowCnt		=	pGrid->GetRows();
	for( i=1; i<nRowCnt ; i++ )
	{
		if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
		{
			pDM->GetCellData( alias, cnt, data, i - 1 );

			receiverName.AddTail( data[ 0 ] );
			receiverNum.AddTail( data[ 1 ] );
			msg.AddTail( data[ 2 ] );
			if( TRUE == data[ 3 ].IsEmpty() )
			{
				data[ 3 ] = _T("0");
			}

			userKey.AddTail( data[ 3 ] );
			if( TRUE == data[ 4 ].IsEmpty() )
			{
				data[ 4 ] = _T("0");
			}

			loanKey.AddTail( data[ 4 ] );
			smsUseYN.AddTail( data[ 5 ] );

			// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if( pmsg_katok != NULL )
			{
				pmsg_katok->Add( data[6] );
			}	
			// KOL.RED.2018.224 ---------------------------------------------------------------------------

			SMSCnt++;
			m_gridIdx.Add( i );
		}
	}

	return SMSCnt;
}

CString CSMSSendDlg::GetSMSErrorMsg( INT ids )
{
	CString errorMsg;
	switch( ids )
	{
		case 0 :
			errorMsg = _T("����");
			break;
		case 1 :
			errorMsg = _T("���� �Ϸ�");
			break;
		case 2 :
			// 2009.04.27 UPDATE BY KSJ : Error Msg ����
			//DEL errorMsg = _T("�ߴ�");
			errorMsg = _T("SMS ������ �ߴܵǾ����ϴ�.");
			break;
		case -1 :
			errorMsg = _T("SMS Sender �� �ʱ�ȭ ���� �ʾҽ��ϴ�.");
			break;
		case -2 :
			errorMsg = _T("Binding�� �����Ͽ����ϴ�.");
			break;
		case -3 :
			errorMsg = _T("money Status �� �������� ���߽��ϴ�.");
			break;
		case -4 :
			errorMsg = _T("������ ��ȭ��ȣ�� �ùٸ��� �ʽ��ϴ�.");
			break;
		case -5 :
			errorMsg = _T("SMS ���ۿ� �����Ͽ����ϴ�.");
			break;
		case -6 :
			errorMsg = _T("Unbinding�� �����Ͽ����ϴ�.");
			break;
		case -7 :
			errorMsg = _T("���� �������� ���� �̿��� �Դϴ�.");
			break;
		default :
			break;
	}

	return errorMsg;
}

VOID CSMSSendDlg::SetSMSSendInfo( INT SMSCnt, INT currentCnt, INT divideCnt, INT error )
{
	CProgressCtrl *pProgress = ( CProgressCtrl * )GetDlgItem( IDC_SMS_PROGRESS );
	CESL_Grid *pGrid = ( CESL_Grid * )( FindCM( _T("CM_SMSSendDlg") )->FindControl( _T("grid") ) );
	INT row = m_gridIdx.GetAt( currentCnt );

	if( !currentCnt )
	{
		pProgress->ShowWindow( true );
		pProgress->SetRange( 0, 100 );
		pProgress->SetPos( 0 );
	}
	
	switch( error )
	{
		case 1 :
			pProgress->SetPos( 100 );
			// 2009.03.02 DEL BY PWR : ���ν��� ������ LOG������
			//InsertSMSLog();
			AfxMessageBox( _T(" SMS ������ �Ϸ��Ͽ����ϴ�. ") );
			pProgress->ShowWindow( false );
			SetSMSSendStatus( false );
			SetCloseBtnEnable();
			break;
			//2009.04.27 DELETE BY KSJ : �����޼��� ����� �޼���â-> �׸����� �߼۰�� �÷����� ���
//DEL 		case 2 :
//DEL 			// 2009.03.02 DEL BY PWR : ���ν��� ������ LOG������
//DEL 			//InsertSMSLog();
//DEL 			AfxMessageBox( _T(" SMS ������ �ߴܵǾ����ϴ�. ") );
//DEL 			pProgress->ShowWindow( false );
//DEL 			SetSMSSendStatus( false );	
//DEL 			SetCloseBtnEnable();
//DEL 			break;
		default :
			pProgress->SetPos( ( currentCnt ? currentCnt : 1 ) * 100 / SMSCnt );
			// 2006.01.04 UPDATE BY PDW(Jr) : ����Ʈ ǥ�� �߰��� ���� �÷� �ε��� ����
			pGrid->SetTextMatrix( row, 6, itos( divideCnt ) );
			pGrid->SetTextMatrix( row, 7, GetSMSErrorMsg( error ) );
			pGrid->SetTopRow( row );
			pGrid->SetReverse( row - 1 );
			if( !error )
			{
				pGrid->SetTextMatrix( row, 1, _T("") );
				AddSMSLog( m_gridIdx.GetAt( currentCnt ) - 1, divideCnt );
			}
			break;
	}
}

CString CSMSSendDlg::itos( INT num )
{
	CString s;
	s.Format( _T("%d"), num );
	return s;
}

VOID CSMSSendDlg::RemoveLog()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		// 2006.01.04 UPDATE BY PDW(Jr) : ����Ʈ ǥ�� �߰��� ���� �÷� �ε��� ����
		pGrid->SetTextMatrix( i, 6, _T("") );		
		pGrid->SetTextMatrix( i, 7, _T("") );
	}
}

VOID CSMSSendDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	ResizeCtrls( cx, cy );
}

VOID CSMSSendDlg::ResizeCtrls( INT cx, INT cy )
{
	return;
	
	const INT term = 4;
	CWnd *pWnd = NULL;

	// pWnd->MoveWindow( x, y, width, height );
	pWnd = ( CWnd * )GetDlgItem( IDC_sPROGRESS );
	if( pWnd->GetSafeHwnd() )
		pWnd->MoveWindow( term, cy - 39 - term, cx - ( term * 2 ), 39 );

	pWnd = ( CWnd * )GetDlgItem( IDC_sSMS_LIST );
	if( pWnd->GetSafeHwnd() )
		pWnd->MoveWindow( term, term + 4 , cx - ( term * 2 ), cy - ( term + 3 + 39  ) );

	pWnd = ( CWnd * )GetDlgItem( IDC_gMAIN_GRID );
	if( pWnd->GetSafeHwnd() )
		pWnd->MoveWindow( term + 5, ( term + 4 ) + 22, cx - ( term * 2 + 10 ), cy - ( term + 3 + 39 + 32 ) );
	
	pWnd = ( CWnd * )GetDlgItem( IDCANCEL );
	if( pWnd->GetSafeHwnd() )
		pWnd->MoveWindow( cx - ( term + 10 + 75 ), cy - ( term + 21 + 5 ), 75, 21 );

	pWnd = ( CWnd * )GetDlgItem( IDOK );
	if( pWnd->GetSafeHwnd() )
		pWnd->MoveWindow( cx - ( term + 15 + ( 75 * 2 ) ), cy - ( term + 21 + 5 ), 75, 21 );
}

VOID CSMSSendDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

VOID CSMSSendDlg::AddSMSLog( INT idx, INT divideCnt )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSSendDlg") );
	
	m_logUserKey.AddTail( pDM->GetCellData( _T("USER_KEY"), idx ) );
	m_logName.AddTail( pDM->GetCellData( _T("�̸�"), idx ) );
	m_logNum.AddTail( pDM->GetCellData( _T("�ڵ���"), idx ) );
	m_logMsg.AddTail( pDM->GetCellData( _T("����"), idx ) );
	m_logDCnt.AddTail( itos( divideCnt ) );	
}

VOID CSMSSendDlg::InsertSMSLog()
{
	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMSSendDlg") );
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	
	// 2006.01.09 ADD BY PDW(Jr) : EMMA �̷°��� ���� �߰�
	const INT cnt = 14;
	CString fieldName[ cnt ] =	{	_T("REC_KEY"), 
									_T("USER_KEY"), 
									_T("NAME"), 
									_T("SEND_PHONE_NUMBER"), 
									_T("SEND_MESSAGE"), 
									_T("SMS_TYPE"), 
									_T("SEND_DIVIDE_CNT"), 
									_T("SEND_TIME"), 
									_T("SEND_WORKER"), 
									_T("SEND_SUCCESS_YN"), 
									_T("SEND_FAIL_REASON"), 
									_T("FIRST_WORK"), 
									_T("LAST_WORK"),
									_T("EMMA_KEY") 
								};
	CString fieldType[ cnt ] =	{	_T("NUMERIC"), 
									_T("NUMERIC"), 
									_T("STRING"), 
									_T("STRING"), 
									_T("STRING"), 
									_T("STRING"), 
									_T("NUMERIC"), 
									_T("NUMERIC"), 
									_T("STRING"), 
									_T("STRING"), 
									_T("STRING"), 
									_T("STRING"), 
									_T("STRING"),
									_T("NUMERIC") 
								};

	CString data[ cnt ];	
	CTime time = pDM->GetDBTime();
	data[  0 ] = _T("ESL_SMS_SEQ.NEXTVAL");
	data[  5 ] = m_sSMSType;
	data[  7 ] = _T("SYSDATE");
	data[  8 ] = GetUserID();
	data[  9 ] = _T("Y");
	data[ 11 ] = GetWorkLogMsg( _T("SMS �߼�"), __WFILE__, __LINE__ );
	
	INT rowCnt = m_logUserKey.GetCount();
	for( INT i = 0; i < rowCnt; i++ )
	{
		GetFieldData( i, data );
		for( INT j = 0; j < cnt; j++ )
		{
//			if ( j == 4 ) continue;
			if( !data[ j ].IsEmpty() )
				pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ], true );
		}
		pDM->MakeInsertFrame( _T("MN_SMS_LOG_TBL") );

		/// ���ϸ� �ý����� Y�� ��츸 DL�� ���ΰ������� ���̺� �μ�Ʈ�Ѵ�.
		if( m_sEcoMailUseYN.Compare(_T("Y")) != 0 ) continue;
		if( m_nSMSType == 3 ) continue;

		CString sPositionCode , sClassCode , sInferiorClass , sUserID , sBirthDay , sCivilNo;

		ids = pDM->GetCellData( _T("�Ҽ�") , i , sPositionCode );
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("ERROR-1") );
			return ;
		}
		ids = pDM->GetCellData( _T("����") , i , sClassCode );
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("ERROR-2") );
			return ;
		}
		ids = pDM->GetCellData( _T("ȸ������") , i , sInferiorClass );
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("ERROR-3") );
			return ;
		}
		ids = pDM->GetCellData( _T("�̿���ID") , i , sUserID );
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("ERROR-4") );
			return ;
		}
		ids = pDM->GetCellData( _T("����") , i , sBirthDay );
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("ERROR-5") );
			return ;
		}
		ids = pDM->GetCellData( _T("�ֹε�Ϲ�ȣ") , i , sCivilNo );
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("ERROR-6") );
			return ;
		}

		CString sDLTitle;
		CString sSmsClass = m_SMSType[m_nSMSType];
		sDLTitle.Format( _T("SMS�߼� %s:%s") , sSmsClass , m_sLibName );
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("NO"),				_T("NUMERIC"),	_T("DL_SEQ.NextVal") );
		pDM->AddDBFieldData( _T("TITLE"),			_T("STRING"),	sDLTitle );
		pDM->AddDBFieldData( _T("WRITE_DATE"),		_T("NUMERIC"),	_T("SYSDATE") );
		pDM->AddDBFieldData( _T("CONTENTS"),		_T("STRING"),	data[4] );
		pDM->AddDBFieldData( _T("POSITION_CODE"),	_T("STRING"),	sPositionCode );
		pDM->AddDBFieldData( _T("CLASS_CODE"),		_T("STRING"),	sClassCode );
		pDM->AddDBFieldData( _T("INFERIOR_CLASS"),	_T("STRING"),	sInferiorClass );
		pDM->AddDBFieldData( _T("USER_ID"),			_T("STRING"),	sUserID );
		pDM->AddDBFieldData( _T("USER_BIRTHDAY"),	_T("STRING"),	sBirthDay );
		sCivilNo.Replace(_T("-"),_T(""));
		if( sCivilNo.GetLength() == 13 )
		{
			if( sCivilNo.GetAt(6) == '1' || sCivilNo.GetAt(6) == '3' || sCivilNo.GetAt(6) == '5' )
			{
				pDM->AddDBFieldData( _T("USER_GENDER") , _T("STRING") , _T("M") );
			}
			else
			{
				pDM->AddDBFieldData( _T("USER_GENDER") , _T("STRING") , _T("F") );
			}
		}

		ids = pDM->MakeInsertFrame( _T("TBL_PRIVATE_NOTICE") );
		if( ids < 0 ) 
		{
			AfxMessageBox( _T("DL Table Insert ERROR") );
			return ;
		}
	}

	if ( rowCnt > 0 ) 
	{
		pDM->SendFrame();
	}
	pDM->EndFrame();	
		
	m_logUserKey.RemoveAll();
	m_logName.RemoveAll();
	m_logNum.RemoveAll();
	m_logMsg.RemoveAll();
	m_logDCnt.RemoveAll();

	// 2006.01.09 ADD BY PDW(Jr) : EMMA �̷°��� ���� �߰�
	m_logEmmaKey.RemoveAll();
}

VOID CSMSSendDlg::GetFieldData( INT idx, CString data[] )
{
	data[  1 ] = m_logUserKey.GetAt( m_logUserKey.FindIndex( idx ) );
	data[  2 ] = m_logName.GetAt( m_logName.FindIndex( idx ) );
	data[  3 ] = m_logNum.GetAt( m_logNum.FindIndex( idx ) );
	data[  4 ] = m_logMsg.GetAt( m_logMsg.FindIndex( idx ) );
	data[  6 ] = m_logDCnt.GetAt( m_logDCnt.FindIndex( idx ) );

	// 2006.01.09 ADD BY PDW(Jr) : EMMA �̷°��� ���� �߰�
	if ( m_strSMSSystemInfo == _T("3") )
	{
		data[ 13 ] = m_logEmmaKey.GetAt( m_logEmmaKey.FindIndex( idx ) );
	}
	else
	{
		data[ 13 ] = _T("");
	}
}

UINT SendSMSMsgThread( LPVOID pParam )
{	
	struct SMS_LIST *pSMSList = ( struct SMS_LIST * )pParam;
	
	CXroshorSMSSender *pSMSSender = ( CXroshorSMSSender * )pSMSList->pSMSSender;
	INT ids = pSMSSender->SendSMSMsg(
				pSMSList->receiverName,
				pSMSList->receiverNum,
				pSMSList->title,
				pSMSList->msg,
				_T(""),
				( VOID * )pSMSList->pParent );
	
//	if( 0 != ids )
//		((CSMSSendDlg*)pSMSList->pParent)->SetSMSSendInfo(0,0,0,2);

	return 0;
}

UINT SendSMSMsgThread2( LPVOID pParam )
{	
	struct SMS_LIST *pSMSList = ( struct SMS_LIST * )pParam;
	
	CArreoSMSManager *pSMSSender = new CArreoSMSManager;
	
	INT ids = pSMSSender->SendSMSMsg(
				pSMSList->receiverName,
				pSMSList->receiverNum,
				pSMSList->title,
				pSMSList->msg,
				_T(""),
				( VOID * )pSMSList->pParent );

	if( 0 != ids )
		((CSMSSendDlg*)pSMSList->pParent)->SetSMSSendInfo(0,0,0,2);

	delete pSMSSender;
		
	return ids;
}

UINT SendSMSMsgThread3( LPVOID pParam )
{	
	struct SMS_LIST *pSMSList = ( struct SMS_LIST * )pParam;
	
	CPajuSMSManager *pSMSSender = new CPajuSMSManager;	
	INT ids = pSMSSender->SendSMSMsg(
				pSMSList->receiverName,
				pSMSList->receiverNum,
				pSMSList->title,
				pSMSList->msg,
				_T(""),
				( VOID * )pSMSList->pParent );	

	if( 0 != ids )
		((CSMSSendDlg*)pSMSList->pParent)->SetSMSSendInfo(0,0,0,2);
		
	delete pSMSSender;
		
	return ids;
}

UINT SendSMSMsgThread4( LPVOID pParam )
{	
	struct SMS_LIST *pSMSList = ( struct SMS_LIST * )pParam;
	
	CEmmaSMSManager *pSMSSender = new CEmmaSMSManager;	
	INT ids = pSMSSender->SendSMSMsg(
				pSMSList->receiverName,
				pSMSList->receiverNum,
				pSMSList->title,
				pSMSList->msg,
				_T(""),
				( VOID * )pSMSList->pParent );	

	if( 0 != ids )
		((CSMSSendDlg*)pSMSList->pParent)->SetSMSSendInfo(0,0,0,2);
		
	delete pSMSSender;
		
	return ids;
}

UINT SendSMSMsgThread5( LPVOID pParam )
{	
	struct SMS_LIST *pSMSList = ( struct SMS_LIST * )pParam;
	
	CImoSMSManager *pSMSSender = new CImoSMSManager;	
	INT ids = pSMSSender->SendSMSMsg(
				pSMSList->receiverName,
				pSMSList->receiverNum,
				pSMSList->title,
				pSMSList->msg,
				_T(""),
				( VOID * )pSMSList->pParent );	

	if( 0 != ids )
		((CSMSSendDlg*)pSMSList->pParent)->SetSMSSendInfo(0,0,0,2);
	
	delete pSMSSender;
		
	return ids;
}
UINT SendSMSMsgThread6( LPVOID pParam )
{	
	struct SMS_LIST *pSMSList = ( struct SMS_LIST * )pParam;
	
	CSamsungNetworks *pSMSSender = new CSamsungNetworks;	
	INT ids = pSMSSender->SendSMSMsg(
				pSMSList->receiverName,
				pSMSList->receiverNum,
				pSMSList->title,
				pSMSList->msg,		        
				_T(""),
				( VOID * )pSMSList->pParent );	

	if( 0 != ids )
		((CSMSSendDlg*)pSMSList->pParent)->SetSMSSendInfo(0,0,0,2);
		
	delete pSMSSender;
		
	return ids;
}

CSMSSendDlg::~CSMSSendDlg()
{
	if( m_pSMSSender )
	{
		delete m_pSMSSender;
		m_pSMSSender = NULL;
	}

	if( m_pSMSList )
	{
		delete m_pSMSList;
		m_pSMSList = NULL;
	}

}

VOID CSMSSendDlg::RemoveSMSList()
{
	m_pSMSList->msg.RemoveAll();
	m_pSMSList->userKey.RemoveAll();
	m_pSMSList->loanKey.RemoveAll();
	m_pSMSList->receiverName.RemoveAll();
	m_pSMSList->receiverNum.RemoveAll();
	m_pSMSList->smsUseYN.RemoveAll();
	m_pSMSList->pSMSSender = NULL;
	m_pSMSList->title.Empty();
	
	// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_pSMSList->msg_katok.RemoveAll();
	// KOL.RED.2018.224 ---------------------------------------------------------------------------
}

VOID CSMSSendDlg::ShowSMSSendStatus( WPARAM wParam, LPARAM lParam )
{
	CString statusMsg = ( ( CString * )wParam )->GetBuffer( 0 );
	
	INT cnt, idx, dCnt, error;
	_stscanf( statusMsg, _T("%d/%d/%d/%d"), &cnt, &idx, &dCnt, &error );
	SetSMSSendInfo( cnt, idx, dCnt, error );
}

VOID CSMSSendDlg::SendSMSMsg()
{
	RemoveLog();
	RemoveSMSList();

	// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//CKatokPlusSend katok_plus;
	//katok_plus.Init( m_pInfo->CONNECTION_INFO , m_pInfo->MANAGE_CODE );
	// KOL.RED.2018.224 ---------------------------------------------------------------------------

	// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//if( !MakeSMSList( m_pSMSList->receiverName, m_pSMSList->receiverNum, m_pSMSList->msg, m_pSMSList->userKey, m_pSMSList->loanKey, m_pSMSList->smsUseYN ) )
	if( !MakeSMSList( m_pSMSList->receiverName, m_pSMSList->receiverNum, m_pSMSList->msg, m_pSMSList->userKey, m_pSMSList->loanKey, m_pSMSList->smsUseYN , &m_pSMSList->msg_katok ) )
	// KOL.RED.2018.224 ---------------------------------------------------------------------------
	{
		AfxMessageBox( _T("SMS ���� ����� �������� �ʽ��ϴ�.") );
		return;
	}
	else
	{
		m_pSMSList->title = m_SMSType[ m_nSMSType ];
		m_pSMSList->pParent = this;

		// 2009.02.20 UPDATE BY PWR : ���ν����� ��ü
		INT ids;
		// 1. DM SETTING 
		CESL_DataMgr TmpDM;
 		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 		TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

		// FIRST_WORK
		CString sFirst_Work;
		sFirst_Work.Format( _T("%s"), GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ) );
		// SEND PHONE
		CString sQuery, sDividCnt, sSendMsg, sSendNum, sRecevNum, sUserKey, sLoanKey, sUserName;
		sSendNum = m_callbackNum;
		// 2010.12.22 ADD BY PWR : ȸ�Ź�ȣ, �������� PC������ ����
		CCfgFileApi cfgFileApi;
		CString strCfgFilePath = _T("..\\cfg\\SMS_Callback.cfg");
		cfgFileApi.SetFile(strCfgFilePath);
		CFileFind ff;
		if(ff.FindFile(strCfgFilePath))
		{
			CString strCallbackTarget;
			cfgFileApi.GetData(_T("ȸ�Ź�ȣ����������"), strCallbackTarget);
			CString strValue;
			if(_T("1") == strCallbackTarget)
			{
				cfgFileApi.GetData(_T("ȸ�Ź�ȣ"), strValue);
				if(!strValue.IsEmpty()) sSendNum = strValue;
			}
		}
		sSendNum.Replace(_T("-"), _T(""));

		for(INT i = 0; i < m_pSMSList->receiverNum.GetCount(); i++)
		{
			// �ڵ�����ȣȮ��
			if(m_pSMSList->receiverNum.GetAt(m_pSMSList->receiverNum.FindIndex(i)) == _T("")) 
			{
				SetSMSSendInfo( m_pSMSList->receiverNum.GetCount(), i, 0, -4 );
				continue;
			}
			// ���ſ���
			if(m_pSMSList->smsUseYN.GetAt(m_pSMSList->smsUseYN.FindIndex(i)) != _T("Y")) 
			{
				SetSMSSendInfo( m_pSMSList->receiverNum.GetCount(), i, 0, -7 );
				continue;
			}

			sSendMsg = m_pSMSList->msg.GetAt(m_pSMSList->msg.FindIndex(i));
			sSendMsg.Replace(_T("'"), _T("''"));
			sRecevNum = m_pSMSList->receiverNum.GetAt(m_pSMSList->receiverNum.FindIndex(i));

			sUserKey = m_pSMSList->userKey.GetAt(m_pSMSList->userKey.FindIndex(i));
			sLoanKey = m_pSMSList->loanKey.GetAt(m_pSMSList->loanKey.FindIndex(i));
			// 2009.05.07 ADD BY PWR : ��ġ������ڿ� ��û�� ������ USER_NAME�߰�
			sUserName = m_pSMSList->receiverName.GetAt(m_pSMSList->receiverName.FindIndex(i));
			// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			/*
			// (1:USER_KEY,2:LOAN_KEY,3:SEND_TEL,4:RECV_TEL,5:MSG,6:MSG_GB,7:MANAGE_CODE,8:SMS_TYPE,9:SEND_WORKER,10:FIRST_WORK,11:USER_NAME)			
			if(m_sSMSType == _T("SMS04"))
			{
				sQuery.Format( _T("SMS_TOT_SEND_PROC(%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s');")
							, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg 	// 1,2,3,4,5
							, _T(""), m_pInfo->MANAGE_CODE, m_sSMSType				// 6,7,8
							, m_pInfo->USER_NAME, sFirst_Work, sUserName );			// 9,10,11
			}
			else
			{
				sQuery.Format( _T("SMS_TOT_SEND_PROC(%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s');")
							, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg 	// 1,2,3,4,5
							, _T(""), m_pInfo->MANAGE_CODE, m_sSMSType				// 6,7,8
							, m_pInfo->USER_NAME, sFirst_Work );					// 9,10
			}
			ids = TmpDM.ExecuteQuery( sQuery, TRUE );
			*/
			// (1:USER_KEY,2:LOAN_KEY,3:SEND_TEL,4:RECV_TEL,5:MSG,6:MSG_GB,7:MANAGE_CODE,8:SMS_TYPE,9:SEND_WORKER,10:FIRST_WORK,11:USER_NAME)		

			if( m_pSMS_MSG_INFO != NULL && m_pSMS_MSG_INFO->CheckUseKatok() > 0 )
			{
				CString  sSendMsg_katok = m_pSMSList->msg_katok.GetAt(i);
				sSendMsg_katok.Replace(_T("'"), _T("''"));
				// ī�� �÷��� ������.
				sQuery.Format( _T("KAKAOPLUS_TOT_SEND_PROC(%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s' , '%s' , '%s');")
								, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg 	
								, _T(""), m_pInfo->MANAGE_CODE, m_sSMSType, m_pInfo->USER_NAME, sFirst_Work, sUserName , 
								m_pSMSList->title , sSendMsg_katok );		
			}
			else
			{
				// sms ������.�����ҽ�.
				
				if(m_sSMSType == _T("SMS04"))
				{
					sQuery.Format( _T("SMS_TOT_SEND_PROC(%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s');")
								, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg 	// 1,2,3,4,5
								, _T(""), m_pInfo->MANAGE_CODE, m_sSMSType				// 6,7,8
								, m_pInfo->USER_NAME, sFirst_Work, sUserName );			// 9,10,11
				}
				else
				{
					sQuery.Format( _T("SMS_TOT_SEND_PROC(%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s');")
								, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg 	// 1,2,3,4,5
								, _T(""), m_pInfo->MANAGE_CODE, m_sSMSType				// 6,7,8
								, m_pInfo->USER_NAME, sFirst_Work );					// 9,10
				}
				
			}

			OutputDebugString( sQuery + _T("\n") );

			ids = TmpDM.ExecuteQuery( sQuery, TRUE );
			// KOL.RED.2018.224 ---------------------------------------------------------------------------


			// �׸��忡 ���ǥ�� �� ���α׷�����
			if(0>ids)
			{ // ����
				SetSMSSendInfo( m_pSMSList->receiverNum.GetCount(), i, 0, 2 );
				continue;
			}

			// 2013.11.19 UPDATE BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (�߼� �Ǽ� ����Ʈ��� �űԾ�ü �� LMS �߰�)
		
			else
			{ // ����
				
				INT nDividCnt = 0;
				// KOL.RED.016 LMS ���� �� ������� ����. ũ�μ��� LMS �����Ѵ�.
				if(	m_strSMSSystemInfo == _T("1") || m_strSMSSystemInfo == _T("2") || m_strSMSSystemInfo == _T("3") || 
					m_strSMSSystemInfo == _T("4") || m_strSMSSystemInfo == _T("5") || m_strSMSSystemInfo == _T("6") ||
					m_strSMSSystemInfo == _T("7") || m_strSMSSystemInfo == _T("8") || m_strSMSSystemInfo == _T("9") )
				/*
				if(	m_strSMSSystemInfo == _T("0") || m_strSMSSystemInfo == _T("1") || m_strSMSSystemInfo == _T("2") || 
					m_strSMSSystemInfo == _T("3") || m_strSMSSystemInfo == _T("4") || m_strSMSSystemInfo == _T("5") ||
					m_strSMSSystemInfo == _T("6"))*/
					{
					if(m_sDividCnt==_T("Y"))
					{
						CESL_DataMgr *pDM = FindDM( _T("DM_SMSSendDlg") );
						if ( !pDM ) 
						{
							ERROR_MSG_RETURN_VOID2( -1, _T("SendSMSMsg") );
						}
						CString sValue;
						ids = pDM->GetCellData(_T("����"), i, sValue);
						if(0>ids) 
						{
							ERROR_MSG_RETURN_VOID2( -2, _T("SendSMSMsg") );
						}
						INT nByte = GetSMSLength(sValue.GetBuffer(0));
					
						if(80 != nByte)
						{
							nDividCnt = (nByte / 80) + 1;
						}
						else
						{
							nDividCnt = nByte / 80;
						}
							
					}
					else
					{ 
						nDividCnt = 1;
					}
					SetSMSSendInfo( m_pSMSList->receiverNum.GetCount(), i, nDividCnt, 0 );
				}
				
				else
				{
					if(m_sLmsYN ==_T("Y"))
					{	
						CESL_DataMgr *pDM = FindDM( _T("DM_SMSSendDlg") );
						if ( !pDM ) 
						{
							ERROR_MSG_RETURN_VOID2( -1, _T("SendSMSMsg") );
						}
						CString sValue;
						ids = pDM->GetCellData(_T("����"), i, sValue);
						if(0>ids)
						{
							ERROR_MSG_RETURN_VOID2( -2, _T("SendSMSMsg") );
						}
						/*
						INT nByte = GetSMSLength(sValue.GetBuffer(0));
						if(2000 != nByte)
						{
							nDividCnt = (nByte / 2000) + 1;
						}
						else
						{
							nDividCnt = nByte / 2000;
						}
						*/
						nDividCnt = 1;
						SetSMSSendInfo( m_pSMSList->receiverNum.GetCount(), i, nDividCnt, 0 );
					}
				
					else
					{
						if(m_sDividCnt==_T("Y"))
						{ 
							CESL_DataMgr *pDM = FindDM( _T("DM_SMSSendDlg") );
							if ( !pDM ) 
							{
								ERROR_MSG_RETURN_VOID2( -1, _T("SendSMSMsg") );
							}
							CString sValue;
							ids = pDM->GetCellData(_T("����"), i, sValue);
							if(0>ids) 
							{
								ERROR_MSG_RETURN_VOID2( -2, _T("SendSMSMsg") );
							}
							INT nByte = GetSMSLength(sValue.GetBuffer(0));
					
							if(90 != nByte)
							{
								nDividCnt = (nByte / 90) + 1;
							}
							else
							{
								nDividCnt = nByte / 90;
							}
						}
						else
						{ 
							nDividCnt = 1;
						}
					SetSMSSendInfo( m_pSMSList->receiverNum.GetCount(), i, nDividCnt, 0 );
					}
			
				}
			} 
		} 
		SetSMSSendInfo( 0, 0, 0, 1 );	
		
		/*
			else
			{ // ����
				// ������ ���� ����
				INT nDividCnt = 0;
				if(m_sDividCnt==_T("Y"))
				{ // ���������
					CESL_DataMgr *pDM = FindDM( _T("DM_SMSSendDlg") );
					if ( !pDM ) 
					{
						ERROR_MSG_RETURN_VOID2( -1, _T("SendSMSMsg") );
					}
					CString sValue;
					ids = pDM->GetCellData(_T("����"), i, sValue);
					if(0>ids) 
					{
						ERROR_MSG_RETURN_VOID2( -2, _T("SendSMSMsg") );
					}
					INT nByte = GetSMSLength(sValue.GetBuffer(0));
					//nDividCnt = (nByte/80)+1;
					if(90 != nByte)
					{
						nDividCnt = (nByte / 90) + 1;
					}
					else
					{
						nDividCnt = nByte / 90;
					}
				}
				else
				{ // 80��������
					nDividCnt = 1;
				}
				
				// �׸���, ���α׷�����
				SetSMSSendInfo( m_pSMSList->receiverNum.GetCount(), i, nDividCnt, 0 );
			} // END ELSE
		} // END FOR
		// �����޽���
		SetSMSSendInfo( 0, 0, 0, 1 );	
		



		/*
		// SK ũ�μ��� �̿��� �߼�		
		if ( m_strSMSSystemInfo == _T("0") || m_strSMSSystemInfo.IsEmpty() )
		{			
			m_pSMSList->pSMSSender = ( CObject * )m_pSMSSender;		
			
			SetSMSSendStatus( true );
			AfxBeginThread( SendSMSMsgThread, ( LPVOID )m_pSMSList );	
		}
		// �Ʒ����� �̿��� �߼�
		else if ( m_strSMSSystemInfo == _T("1") )
		{			
			SetSMSSendStatus( true );
			m_pSMSList->pSMSSender = NULL;
			AfxBeginThread( SendSMSMsgThread2, ( LPVOID )m_pSMSList );	
		}
		// ������Ʈ SMS �ý���
		// 2009.02.20 UPDATE BY PWR : �ӽ÷� ARREO�� I-HEART�� ����
		else if ( m_strSMSSystemInfo == _T("2") || m_strSMSSystemInfo == _T("7") )
		{
			SetSMSSendStatus( true );
			AfxBeginThread( SendSMSMsgThread2, ( LPVOID )m_pSMSList );	
		}
		//else if ( m_strSMSSystemInfo == _T("2") )
		//{
		//	SetSMSSendStatus( true );
		//	AfxBeginThread( SendSMSMsgThread3, ( LPVOID )m_pSMSList );	
		//}		
		// ���� SMS �ý���
		else if ( m_strSMSSystemInfo == _T("3") )
		{
			SetSMSSendStatus( true );
			AfxBeginThread( SendSMSMsgThread4, ( LPVOID )m_pSMSList );	
		}	
		// ���̿��� SMS �ý���
		else if ( m_strSMSSystemInfo == _T("4") )
		{
			SetSMSSendStatus( true );
			AfxBeginThread( SendSMSMsgThread5, ( LPVOID )m_pSMSList );	
		}		
		// �Ｚ ��Ʈ�p��
		else if ( m_strSMSSystemInfo == _T("5") )
		{
			SetSMSSendStatus( true );
			AfxBeginThread( SendSMSMsgThread6, ( LPVOID )m_pSMSList );	
		}
		*/
	} // END ELSE


	// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//katok_plus.Release();
	// KOL.RED.2018.224 ---------------------------------------------------------------------------

}

VOID CSMSSendDlg::SetSMSSendStatus( bool flag )
{
	CButton *pBtn = ( CButton * )GetDlgItem( IDOK );
	
	if( flag )	// ������
	{
		m_SMSSending = true;
		pBtn->SetWindowText( _T("�� ��") );
	}
	else		// ������
	{
		m_SMSSending = false;
		m_bSMSStop = false;
		pBtn->SetWindowText( _T("�� ��") );
	}
}

VOID CSMSSendDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	if( m_SMSSending )
	{
		AfxMessageBox( _T("SMS �������Դϴ�.\r\n���� SMS ������ �ߴ��Ͽ� �ֽʽÿ�") );
		return;
	}
	else
		CDialog::OnCancel();
}

VOID CSMSSendDlg::SetCloseBtnEnable()
{
	CButton *pBtn = ( CButton * )GetDlgItem( IDCANCEL );
	if( m_SMSSending )
		pBtn->EnableWindow( false );
	else
		pBtn->EnableWindow( true );
}


INT CSMSSendDlg::SetInfoEx(  CStringArray &asPositionCode , CStringArray &asClassCode 
		, CStringArray& asInferiorClass , CStringArray& asUserID , CStringArray& asBirthDay , 
		CStringArray& asCivilNo )
{
	
	m_asPositionCode.Append(asPositionCode);
	m_asClassCode.Append(asClassCode);
	m_asInferiorClass.Append(asInferiorClass);
	m_asUserID.Append(asUserID);
	m_asBirthDay.Append(asBirthDay);
	m_asCivilNo.Append(asCivilNo);

	return 0;
}


INT CSMSSendDlg::SetEcoMailUseYN()
{
	INT ids;

	/// ������ �̸� ��������
	CString sLibInfo[1] =
	{
		_T("LIB_NAME")
	};
	CLocCommonAPI::GetLibInfo( sLibInfo , sLibInfo , 1 , this);
	m_sLibName = sLibInfo[0];
	
	/// ���ϸ� �ý��� ��� ���������� �˾ƺ���.
	ids = GetManageValue( _T("��Ÿ")  , _T("����") , _T("���ϸ��ý���") , _T("��뿩��") , m_sEcoMailUseYN );
	if( ids < 0 )
	{
		AfxMessageBox(_T("Error2"));
		return -2;
	}

	if( m_sEcoMailUseYN.IsEmpty() ) m_sEcoMailUseYN = _T("N");
	
	
	return 0;
}


VOID CSMSSendDlg::OneSendSMSMsg(SMS_LIST* pSMS_LIST)
{	
	pSMS_LIST->title = _T("�Ϲݸ޽���");
	pSMS_LIST->pParent = NULL;
	
	// SK ũ�μ��� �̿��� �߼�		
	if ( m_strSMSSystemInfo == _T("0") || m_strSMSSystemInfo.IsEmpty() )
	{			
		pSMS_LIST->pSMSSender = ( CObject * )m_pSMSSender;				
		m_SMSSending = true;
		
		CXroshorSMSSender	pSMS(this);
		pSMS.InitSetXShot( m_SMS_ConnectionInfo.sBindId,
							m_SMS_ConnectionInfo.sBindPwd,
							m_SMS_ConnectionInfo.sUserDomain,
							m_SMS_ConnectionInfo.sUserPhoneId,
							m_callbackNum,
							m_SMS_ConnectionInfo.sCfgConFilePath );
		pSMS.BindXShot( );	

		pSMS.SendSMSMsg( _T("ECO"),
						 pSMS_LIST->receiverNum.GetAt(pSMS_LIST->receiverNum.FindIndex(0)),
						 _T("�Ϲݸ޽���"),
						 pSMS_LIST->msg.GetAt(pSMS_LIST->msg.FindIndex(0)),
						 m_callbackNum );		
		pSMS.UnbindXShot();		
	}
	// �Ʒ����� �̿��� �߼�
	else if ( m_strSMSSystemInfo == _T("1") )
	{			
		m_SMSSending = true;		
		pSMS_LIST->pSMSSender = this;
		pSMS_LIST->pParent = this;
		AfxBeginThread( SendSMSMsgThread2, ( LPVOID )pSMS_LIST );	
	}
	// ���� SMS �ý���
	else if ( m_strSMSSystemInfo == _T("2") )
	{
		m_SMSSending = true;		
		pSMS_LIST->pSMSSender = this;
		pSMS_LIST->pParent = this;
		AfxBeginThread( SendSMSMsgThread3, ( LPVOID )pSMS_LIST );	
	}	
	// EMMA SMS �ý���
	else if ( m_strSMSSystemInfo == _T("3") )
	{
		m_SMSSending = true;		
		pSMS_LIST->pSMSSender = this;
		pSMS_LIST->pParent = this;
		AfxBeginThread( SendSMSMsgThread4, ( LPVOID )pSMS_LIST );	
	}
	// IMO SMS �ý���
	else if ( m_strSMSSystemInfo == _T("4") )
	{
		m_SMSSending = true;		
		pSMS_LIST->pSMSSender = this;
		pSMS_LIST->pParent = this;
		AfxBeginThread( SendSMSMsgThread5, ( LPVOID )pSMS_LIST );	
	}	
	
	
}

INT CSMSSendDlg::SMSInit(CESL_Mgr* pParent)
{
	// 2005.10.05 ADD BY PDJ : SMS �ý��������� �о�´�.
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSSendDlg") );
	
	CString strQuery;
	// 2008.10.22 UPDATE BY PWR : ���սý��� ����
	strQuery=_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
			 _T("WHERE CLASS_ALIAS='SMS�ý�������' AND MANAGE_CODE=UDF_MANAGE_CODE");
	//strQuery=_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='SMS�ý�������'");
	pDM->GetOneValueQuery(strQuery, m_strSMSSystemInfo);
	
	if ( m_strSMSSystemInfo == _T("0") || m_strSMSSystemInfo.IsEmpty() )
	{
		if ( pParent == NULL ) m_pSMSSender = new CXroshorSMSSender( this );
		else m_pSMSSender = new CXroshorSMSSender( pParent );
		m_pSMSSender->InitSetXShot( 
								m_SMS_ConnectionInfo.sBindId,
								m_SMS_ConnectionInfo.sBindPwd,
								m_SMS_ConnectionInfo.sUserDomain,
								m_SMS_ConnectionInfo.sUserPhoneId,
								m_callbackNum,
								m_SMS_ConnectionInfo.sCfgConFilePath );		
	}	

	// 2009.02.20 ADD BY PWR : ���������� ����
	strQuery.Format( _T("SELECT MIN(SEND_DIVIDE_YN) ")
					 _T("FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), m_strSMSSystemInfo );
	pDM->GetOneValueQuery(strQuery, m_sDividCnt);

	strQuery.Format( _T("SELECT LMS_USE_YN ")
					 _T("FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), m_strSMSSystemInfo );
	pDM->GetOneValueQuery(strQuery, m_sLmsYN);
	return 0;
}



BEGIN_EVENTSINK_MAP(CSMSSendDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSMSSendDlg)
	ON_EVENT(CSMSSendDlg, IDC_gMAIN_GRID, -601 /* DblClick */, OnDblClickgMAINGRID, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/*--------------------------------------------------------------------------------------------
   @Work Day : 2006-01-02 ~ 2006-01-02
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
 	 
   [   �Լ�����   ] 
      �߼� ����Ʈ���� ���� ������ �� �ְ� �̺�Ʈ ó��
--------------------------------------------------------------------------------------------*/
VOID CSMSSendDlg::OnDblClickgMAINGRID() 
{
	INT nCol = m_pGrid->GetCol();
	
	InputDataToGrid();
	
	m_pGrid->SetCol( nCol );
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2006-01-02 ~ 2006-01-02
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
 	 
   [   �Լ�����   ] 
      ������ ������ �� �ִ� �Է�â�� �����Ѵ�.
--------------------------------------------------------------------------------------------*/
INT CSMSSendDlg::InputDataToGrid()
{
	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();
	CString sValue = m_pGrid->GetText();

	if( !nRow || nCol != 4)
		return 0;
	
	FreeGridEdt();	

	m_pGridEdt = new CGridUpdateEdt;

	m_pGridEdt->SetParentWnd( (CWnd*)this );
	
	m_pGridEdt->m_nRow = nRow;
	m_pGridEdt->m_nCol = nCol;

	CWnd *pWnd = ( CWnd * )GetDlgItem( IDC_gMAIN_GRID );
	
	m_pGridEdt->Create( ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD, CRect( 0 , 0, 10, 20 ), pWnd->GetParent(), 645 );

	CRect rect,gridRect;
	RECT parentRect;
	pWnd->GetWindowRect( &gridRect );
	pWnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( pWnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );

	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left   = gridRect.left + ( long )( ( float )m_pGrid->GetCellLeft()   / 15.0f );
	rect.top    = gridRect.top  + ( long )( ( float )m_pGrid->GetCellTop()    / 15.0f );
	rect.right  = rect.left     + ( long )( ( float )m_pGrid->GetCellWidth()  / 15.0f );
	rect.bottom = rect.top      + ( long )( ( float )m_pGrid->GetCellHeight() / 15.0f );
	m_pGridEdt->MoveWindow( rect.left - 1, rect.top - 1, rect.Width(), rect.Height() );

	m_pGridEdt->ShowWindow( true );
	m_pGridEdt->SetFocus();

	m_pGridEdt->SetWindowText( sValue );

 	return 0;
}

INT CSMSSendDlg::FreeGridEdt()
{
	if ( m_pGridEdt )
	{
		delete m_pGridEdt;
		m_pGridEdt = NULL;
	}

	return 0;
}

VOID CSMSSendDlg::OnUpdateGridtext(WPARAM wParam, LPARAM lParam)
{
	if( wParam )
	{
		CString newValue;
		m_pGridEdt->GetWindowText( newValue );		

		INT curRow = m_pGrid->GetRow();
		INT curCol = m_pGrid->GetCol();

		m_pGrid->SetRow( m_pGridEdt->m_nRow );
		m_pGrid->SetCol( m_pGridEdt->m_nCol );
		m_pGrid->SetText( newValue );

		// BYTE ����
		m_pGrid->SetCol( m_pGridEdt->m_nCol + 1 );
		m_pGrid->SetText( GetSMSByte( newValue ) );

		m_pGrid->SetRow( curRow );
		m_pGrid->SetCol( curCol );

		// DM�� �ݿ����� 
		CESL_DataMgr *pDM = FindDM( _T("DM_SMSSendDlg") );
		if ( !pDM ) ERROR_MSG_RETURN_VOID2( -1, _T("CSMSSendDlg::OnUpdateGridtext()") );

		pDM->SetCellData( _T("����") , newValue , m_pGridEdt->m_nRow - 1 );
		pDM->SetCellData( _T("BYTE") , GetSMSByte( newValue ) , m_pGridEdt->m_nRow - 1 );		
	}

	FreeGridEdt();
	m_pGrid->SetFocus();
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2006-01-03 ~ 2006-01-03
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
 	 
   [   �Լ�����   ] 
	  URL�� ��뿩�θ� �����Ѵ�.

   [ RETURN VALUE ]
      0    : ����
      ���� : ����

   [   �̴Ͻ���   ]
      1. URL�� �����Ѵ�.
	  2. URL��� ������ �����Ѵ�.
--------------------------------------------------------------------------------------------*/
INT CSMSSendDlg::SetMSG_GB()
{
	INT ids;
	// URL ������ �ȵǴ� ���񽺴� ���ϵȴ�. 	
	if ( !( m_strSMSSystemInfo == _T("1") || m_strSMSSystemInfo == _T("3") || m_strSMSSystemInfo == _T("4") ) )
	{		
		return 0;
	}

	CString sDMAlias = _T("DM_SMSSendDlg");
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if ( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T(" CSMSSendDlg::SetMSG_GB()") );

	// --------------------------------------------------------------------------------
	// 1. URL�� �����Ѵ�. 
	// --------------------------------------------------------------------------------
	CString sQuery;
	sQuery.Format( _T("SELECT MIN(URL) FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE = '%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), m_strSMSSystemInfo );
	ids = pDM->GetOneValueQuery( sQuery, m_sURL );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T(" CSMSSendDlg::SetMSG_GB()") );

	// --------------------------------------------------------------------------------
	// 2. URL��� ������ �����Ѵ�.
	// --------------------------------------------------------------------------------
	CString sMSG_GB;
	sQuery.Format( _T("SELECT MIN(MSG_GB) FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE = '%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), m_strSMSSystemInfo );
	ids = pDM->GetOneValueQuery( sQuery, sMSG_GB );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T(" CSMSSendDlg::SetMSG_GB()") );
	
	if ( m_strSMSSystemInfo == _T("1") && ( sMSG_GB == _T("B") || sMSG_GB == _T("D") ) )	m_bUseURL = TRUE;	// �Ʒ���
	else if ( m_strSMSSystemInfo == _T("3") &&  sMSG_GB == _T("5") )	m_bUseURL = TRUE;					// ����
	else if ( m_strSMSSystemInfo == _T("4") &&  sMSG_GB == _T("1") )	m_bUseURL = TRUE;					// ���̿���

	return 0;	
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2006-01-04 ~ 2006-01-04
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
 	 
   [   �Լ�����   ] 
	  ���ڳ����� BYTE �����Ѵ�.

   [  PARAMETERS  ]
      sContents : [I N] ���ڳ���
      
   [ RETURN VALUE ]
      ����Ʈ ǥ�� ���� 
--------------------------------------------------------------------------------------------*/
CString CSMSSendDlg::GetSMSByte(CString sContents)
{
	CString sBYTE;

	if ( m_bUseURL )
	{
		sBYTE.Format( _T("%d / %d"), GetSMSLength(sContents.GetBuffer(0)), SMS_MAXBYTE - ( GetSMSLength(m_sURL.GetBuffer(0)) + 1 ) );
	}
	else
	{
		sBYTE.Format( _T("%d"), GetSMSLength(sContents.GetBuffer(0)) );
	}

	return sBYTE;
}

HBRUSH CSMSSendDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


INT	CSMSSendDlg::GetSMSLength(_TCHAR *fi_pSourceData)
{
	// 16.11.29 JMJ KOL.RED.016
	int nBytesNeeded = WideCharToMultiByte(CP_ACP,0,fi_pSourceData,lstrlenW(fi_pSourceData),NULL,0,NULL,NULL);

	return nBytesNeeded;
}
