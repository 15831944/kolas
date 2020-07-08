// SMS_LifeLong_Sender.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_LifeLong_Sender.h"
#include "..\CXroshotManager\EmmaSMSManager.h"
#include "SMS_LifeLong_Contents_Editor.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong_Sender dialog


CSMS_LifeLong_Sender::CSMS_LifeLong_Sender(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_LifeLong_Sender)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCSMS_LifeLong = (CSMS_LifeLong*)pParent;
	m_pDMSend = NULL;
	m_receiverName.RemoveAll();
	m_receiverNum.RemoveAll();
	m_msg.RemoveAll();
	m_sTitle = _T("");
	m_sDefaultMsg = _T("");
	m_sMESSAGE = _T("");
	m_sLibName = _T("");

	// 2006-01-09 ADD BY PDW(Jr) ==============================================
	// ����н��� SMS �α� ���� ���� �߰�
	m_lstEmmaKey.RemoveAll();;
	m_lstDCnt.RemoveAll();;
	// ========================================================================
}


VOID CSMS_LifeLong_Sender::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_LifeLong_Sender)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_LifeLong_Sender, CDialog)
	//{{AFX_MSG_MAP(CSMS_LifeLong_Sender)
	ON_WM_DESTROY()
	ON_MESSAGE( SMS_SEND_STATUS, ShowSMSSendStatus )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong_Sender message handlers

BOOL CSMS_LifeLong_Sender::Create(CWnd* pParentWnd)
{
EFS_BEGIN
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::Create(IDD, pParentWnd);
EFS_END
	return FALSE;
}

BOOL CSMS_LifeLong_Sender::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids;
	//////////////////////////////////////////////////////////////////////////////
	// 1. SM �ʱ�ȭ
	//////////////////////////////////////////////////////////////////////////////
	ids = InitESL_Mgr(_T("SMS_DLG_LifeLong_Sender"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::OnInitDialog()") );
	
	//////////////////////////////////////////////////////////////////////////////
	// 2. ���α׷��� �ʱ�ȭ
	//////////////////////////////////////////////////////////////////////////////
	ids = InitProgress();
	if( ids < 0 ) return FALSE;

	//////////////////////////////////////////////////////////////////////////////
	// 3. SMS ���� 
	//////////////////////////////////////////////////////////////////////////////
	ids = InitSMSEdit();
	if( ids < 0 ) return FALSE;

	// 2006-01-05 ADD BY PDW(Jr) =================================================
	// �������� ���� ����
	if ( SetLibName() < 0 )
	{
		return FALSE;
	}
	// ===========================================================================

	//////////////////////////////////////////////////////////////////////////////
	// 4. �̸����� �ʱ�ȭ
	//    �̸������ SMS�� ������ ���õǾ� �־���Ѵ�.
	//////////////////////////////////////////////////////////////////////////////
	if ( !m_pCSMS_LifeLong->m_sSMSType.IsEmpty() )
	{
		ids = InitPreview();
		if ( ids < 0 ) return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �߼ۿ� DM�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::SetInfo(CESL_DataMgr *pDM)
{
EFS_BEGIN

	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::SetInfo") );
	m_pDMSend = pDM;

	return 0;
EFS_END
	return -1;
}

VOID CSMS_LifeLong_Sender::OnOK() 
{
	// TODO: Add extra validation here
	SendSMS();
//	CDialog::OnOK();
}
//////////////////////////////////////////////////////////////////////////////
// [�Լ�����] 
// �߼�
//////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::SendSMS()
{
EFS_BEGIN
	
	INT ids;
	//////////////////////////////////////////////////////////////////////////////
	// 1. �Ʒ����� ������ ����Ʈ�� �����.
	//    �޴»���̸�
	//    �޴»����ȣ
	//    Ÿ��Ʋ
	//    �޼���
	//////////////////////////////////////////////////////////////////////////////
	ids = MakeSMSData();
	if (ids < 0) return ids;

	//////////////////////////////////////////////////////////////////////////////
	// 2. �Ʒ��� Ŭ������ �ִ´�.
	//////////////////////////////////////////////////////////////////////////////
	CEmmaSMSManager Emma;

	// 2006-01-09 ADD BY PDW(Jr) =================================================
	// ����н��� �÷��� �߰� 
	Emma.m_bLifeLong = TRUE;
	// ===========================================================================

	ids = Emma.SendSMSMsg(m_receiverName, m_receiverNum, _T(""), m_msg, _T(""), this);
	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::SendSMS") );

	//////////////////////////////////////////////////////////////////////////////
	// 3. ������
	//////////////////////////////////////////////////////////////////////////////
	return 0;
EFS_END
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �Ʒ��� �߼ۿ� ����Ʈ�� �����.
//////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::MakeSMSData()
{
EFS_BEGIN
	INT ids;
	INT nCnt;
	
	//////////////////////////////////////////////////////////////////////////////
	// 1. DM���� �⺻�����͸� �����.
	//////////////////////////////////////////////////////////////////////////////
	
	// 2006-01-05 ADD BY PDW(Jr) =================================================
	// �ڵ��� ��ȣ�� NULL�� ȸ���� ���� ó���� �߰���
	INT nNullHPCnt = 0;

	nCnt = m_pDMSend->GetRowCount();
	if (nCnt < 1) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::MakeSMSData") );

	CString HP      = _T("");
	CString name    = _T("");
	CString program = _T("");
	CString msg     = _T("");

	for (INT i = 0;i < nCnt;i++)
	{		
		m_pDMSend->GetCellData(_T("�ڵ���"), i, HP);
		
		if ( HP.IsEmpty() )	
		{
			nNullHPCnt++;
			continue;
		}

		m_pDMSend->GetCellData(_T("ȸ����"), i, name);
		if (m_pCSMS_LifeLong->m_nDisplayMode == 0)
			m_pDMSend->GetCellData(_T("���¸�"), i, program);

		//////////////////////////////////////////////////////////////////////////
		// 2. �޼����� �����.
		//////////////////////////////////////////////////////////////////////////
		ids = MakeMSG(&msg, name, program);
		if (ids < 0) continue;
				
		//////////////////////////////////////////////////////////////////////////
		// 3. �޼����������� �����ߴٸ�
		//    ����Ʈ�� �߰��ȴ�.
		//////////////////////////////////////////////////////////////////////////
		m_receiverNum.AddTail(HP);
		m_receiverName.AddTail(name);
		m_msg.AddTail(msg);
	}

	if ( nNullHPCnt )
	{
		CString sResultMsg;
		sResultMsg.Format( _T("�ڵ��� ��ȣ�� ���� ȸ�������� �ֽ��ϴ�. ( %d : �Ǽ� )"), nNullHPCnt );
		AfxMessageBox( sResultMsg );
	}

	// ===========================================================================
	return 0;
EFS_END
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �޼����� �����.
//////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::MakeMSG(CString* psGetMsg, CString sName, CString sProgram)
{
EFS_BEGIN

	INT ids;

	//////////////////////////////////////////////////////////////////////////
	// 1. MN_SMS_TYPE_ENV_TBL �� DEFAULT_MAESSAGE�� �����´�
	//////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::MakeMSG") );

	CString sQuery  = _T("");
    CString sGetMsg = _T("");

/*	sQuery.Format( _T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE = '%s'") , 
				   m_pCSMS_LifeLong->m_sSMSType );

	ids = pDM->GetOneValueQuery( sQuery, sGetMsg );
	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Sender::MakeMSG") );
*/
	// �޼��� ��� ����
	sGetMsg = m_sMESSAGE;

	//////////////////////////////////////////////////////////////////////////
	// 2. �˸��ƽ��� �����ͷ� ��ȯ�Ѵ�
	//    [ALIAS]
	//    %������% -> �����ڸ�
	//    %���¸�% -> ���¸�
	//    %������% -> ���°����� 
	//  ** �ҽ����� �˸��ƽ� ó���� ���ش�. 
	//     ��������� ������ü�� Ư�� ���� ����̱� ������ ���۷����� ������ �ʴ´�.
	//////////////////////////////////////////////////////////////////////////
	sGetMsg.Replace(_T("%������%"), sName);
	sGetMsg.Replace(_T("%���¸�%"), sProgram);

	// 2.1) ���α׷����� �ִٸ� �������� ���Ҽ� �ִ�.
	CString sOpenDay = _T("");
	if ( !sProgram.IsEmpty() )
	{	
		sQuery.Format(_T("SELECT GAEGANGDAY FROM PG_PROGRAM@EMT WHERE PG_NAME = '%s'"), sProgram);
		ids = pDM->GetOneValueQuery( sQuery, sOpenDay );
		if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong_Sender::MakeMSG") );			
	}

	sGetMsg.Replace(_T("%������%"), sOpenDay);
	*psGetMsg = sGetMsg;

	return 0;
EFS_END
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ���α׷����ٸ� ���� ��Ų��.
//////////////////////////////////////////////////////////////////////////////
VOID CSMS_LifeLong_Sender::ShowSMSSendStatus(WPARAM wParam, LPARAM lParam)
{
	CString statusMsg = ( ( CString * )wParam )->GetBuffer( 0 );

	// cnt = ��ü ����, ids = ���� ��ġ, nCnt = ��������, error = �÷���
	INT cnt, idx, dCnt, error;
	_stscanf( statusMsg, _T("%d/%d/%d/%d"), &cnt, &idx, &dCnt, &error );
	CProgressCtrl *pProgress = ( CProgressCtrl * )GetDlgItem( IDC_pProgress );
		
	// 2006-01-09 UPDATE BY PDW(Jr) ==========================================
	// �α� ���úκ� �߰� ��
	switch( error )
	{
		case 1 :
			pProgress->SetPos( m_pDMSend->GetRowCount() );
			InsertSMSLog();
			AfxMessageBox( _T(" SMS ������ �Ϸ��Ͽ����ϴ�. ") );
			break;
		case 2 :
			InsertSMSLog();
			AfxMessageBox( _T(" SMS ������ �ߴܵǾ����ϴ�. ") );
			break;
		default :
			pProgress->SetPos( idx );
			AddSMSLog( dCnt );
			break;
	}
	// ======================================================================
}

//////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ���α׷����ٸ� �ʱ�ȭ �Ѵ�.
//////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::InitProgress()
{
	CProgressCtrl *pProgress = ( CProgressCtrl * )GetDlgItem( IDC_pProgress );
	if (pProgress == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CSMS_LifeLong_Sender::InitProgress") );
	
	INT nCnt = m_pDMSend->GetRowCount();
	pProgress->SetRange( 0, nCnt );
	pProgress->SetPos( 0 );
	return 0;
}

VOID CSMS_LifeLong_Sender::OnDestroy() 
{
	CDialog::OnDestroy();
		
}

//////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// SMS ���� ǥ��
//////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::InitSMSEdit()
{
	//////////////////////////////////////////////////////////////////////////
	// 1. DM���� �ش� �̸��� ǥ�� ���ش�.
	//////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Sender::OnInitDialog()") );
	pDM->RefreshDataManager( _T("") );

	INT nCnt = pDM->GetRowCount();
	CString sValue = _T("");

	for (INT i = 0;i < nCnt;i++)
	{
		pDM->GetCellData( _T("Ÿ��"), i, sValue );

		if ( _tcscmp( m_pCSMS_LifeLong->m_sSMSType, sValue ) == 0 )
		{
			pDM->GetCellData( _T("SMS��"), i, sValue );
			SetDlgItemText(IDC_eMAILNAME, sValue );
			// ADD WORK �⺻�޼��� ����
			pDM->GetCellData( _T("�⺻�޼���"), i, m_sDefaultMsg );
			break;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �̸����� ȭ�� �ʱ�ȭ
//////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::InitPreview()
{
	INT ids;

	CSMS_LifeLong_Contents_Editor Contents(this);
	Contents.m_sSMSType = m_pCSMS_LifeLong->m_sSMSType;
	
	//////////////////////////////////////////////////////////////////////////
	// 1. ���������� ���� �ҷ��´�
	//////////////////////////////////////////////////////////////////////////
	CString sContents = _T("");
	Contents.GetCFGContents( &sContents );
	
	//   ������ �̸�,      �����ڸ�,        ���¸�,           ������,        �⺻�޼���
	BOOL bLibName = FALSE, bSender = FALSE, bProgram = FALSE, bOpen = FALSE, bDefaultMsg = FALSE;
	
	bLibName    = _ttoi( sContents.Mid(0,1) );
	bSender     = _ttoi( sContents.Mid(1,1) );
	bProgram    = _ttoi( sContents.Mid(2,1) );
	bOpen       = _ttoi( sContents.Mid(3,1) );
	bDefaultMsg = _ttoi( sContents.Mid(4,1) );

	//////////////////////////////////////////////////////////////////////////
	// 2. �������� ���� �޼����� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////
	CString sMsg = _T("");
	
	// 2005-12-22 ADD BY PDW(Jr) =============================================
	// URL�̶�� URL�� �����ش�.
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::InitPreview()") );

	CString sData;
	ids = pDM->GetOneValueQuery( _T("SELECT MIN(MSG_GB) FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE = '3' AND MANAGE_CODE=UDF_MANAGE_CODE"), sData );
	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Sender::InitPreview()") );
	if ( sData == _T("5") )
	{
		ids = pDM->GetOneValueQuery( _T("SELECT MIN(URL) FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE = '3' AND MANAGE_CODE=UDF_MANAGE_CODE"), sData );
		if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong_Sender::InitPreview()") );

		if ( !sData.IsEmpty() )
			sMsg += sData + _T(" ");
	}
	// =======================================================================	

	if ( bLibName && m_sLibName.GetLength() > 0 )
	{		
		sMsg += _T("[") + m_sLibName + _T("]");
	}

	if ( bSender )
	{
		sMsg += _T("%������% ");
	}

	if ( bDefaultMsg )
	{	
		CString sDefault = _T("");
		Contents.GetDefaultMsg( &sDefault );
		sMsg += sDefault;
	}

	// ���� ���� ������ �ʼ� ������ �ִ�.
	// 1. PM�� ���� ������ �־���Ѵ�. 
	// 2. SMS ������ '�����ȳ�', '��������Ȯ��'�� ��츸 ����
	if ( bProgram && m_pCSMS_LifeLong->m_nDisplayMode == 0 && 
			( m_pCSMS_LifeLong->m_sSMSType == _T("LFE01") || m_pCSMS_LifeLong->m_sSMSType == _T("LFE02") ) )
		sMsg += _T(" %���¸�% ");
	if ( bOpen && m_pCSMS_LifeLong->m_nDisplayMode == 0 &&
			( m_pCSMS_LifeLong->m_sSMSType == _T("LFE01") || m_pCSMS_LifeLong->m_sSMSType == _T("LFE02") ) )
		sMsg += _T(" ������:%������%");

	//////////////////////////////////////////////////////////////////////////
	// 3. �̸����� ȭ�鿡 ����Ѵ�.
	//    �˸��ƽ��� �̸����� ������� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////
	m_sMESSAGE = sMsg;
	sMsg.Replace( _T("%������%"), _T("'������'") );
	sMsg.Replace( _T("%���¸�%"), _T("'���¸�'") );	
	sMsg.Replace( _T("%������%"), _T("'������'") );		
	
	INT nByte = sMsg.GetLength();
	CString sTitle = _T("");
	sTitle.Format( _T("�̸����� ( %d Byte )"), nByte );

	// 2005-12-12 UPD BY PDW(Jr) ==================================================
	// ���ڱ��̸� 16����Ʈ ������ �ڸ���.	
	INT nPos = 0;
	INT nLength = sMsg.GetLength();
	
	while ( TRUE )
	{
		// �Ϲ������� 8���ڴ� ����. KTF����
		nPos += 16;
		if ( nPos > nLength ) break;
		
		if ( !IsCanCut( sMsg, nPos - 1 ) )	nPos--;	
		
	    sMsg = sMsg.Mid( 0, nPos ) + _T("\n     ") + sMsg.Mid( nPos );
		nPos += 6;
		nLength += 6;
	}

	sMsg = _T("     ") + sMsg;
	//SetDlgItemText( IDC_reSMS, sMsg );
	CRichEditCtrl* edtPreView = (CRichEditCtrl*)GetDlgItem( IDC_reSMS );
	edtPreView->SetWindowText( sMsg );
	edtPreView->SetReadOnly();
	// CRichEditCtrl
	// ============================================================================

	SetDlgItemText( IDC_stPreView, sTitle );
	

	return 0;
}



VOID CSMS_LifeLong_Sender::OnCancel() 
{
	// TODO: Add extra cleanup here
//	this->DestroyWindow();	
	CDialog::OnCancel();
}

BOOL CSMS_LifeLong_Sender::IsCanCut(CString str, INT pos)
{
	INT i, nLastPos = -1;
	//���ڼ� �ʰ��� FALSE
	if( str.GetLength() <= pos )
		return TRUE;
	
	TCHAR ch = str.GetAt( pos );
	
	//������ ���� || �ѱ��� �ƴϸ� TRUE
	if( pos + 1 == str.GetLength() || ( ch & ( 0x80 ) ) != 128 )
		return true;
	
	//ù���ڰ� 1byte���ڸ� TRUE �ƴϸ� FALSE
	if( !pos )
		if( ( ch & ( 0x80 ) ) != 128 )
			return true;
		else
			return false;

	for( i = 0; i < pos; i++ )
	{
		ch = str.GetAt( i );
		
		//�ڸ��� �κп��� ���� �����(��������)������ ��ġ
		if( ( ch & ( 0x80 ) ) != 128 )
			nLastPos = i;
	}
	
	if( nLastPos == -1 && pos % 2 == 1 )
		return true;
	
	if( nLastPos == -1 && pos % 2 == 0 )
		return false;
	
	//¦���� ���̳��� TRUE
	bool result = !( ( pos - nLastPos ) % 2 );
	return result;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2006-01-05 ~ 2006-01-05
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
 	 
   [   �Լ�����   ] 
	  ���������� �����Ѵ�.

   [ RETURN VALUE ]
      0    : ���� 
      ���� : ���� 
--------------------------------------------------------------------------------------------*/
INT CSMS_LifeLong_Sender::SetLibName()
{
	INT ids;
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSInfo") );
	if ( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::SetLibName()") );

	// ���� ������ �����´� 
	CString sQuery = _T("");
	sQuery.Format( _T("SMS_TYPE='3'") );
	ids = pDM->RefreshDataManager( sQuery );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Sender::SetLibName()") );

	// ��������
	pDM->GetCellData( _T("��������"), 0, m_sLibName );	
	return 0;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2006-01-09 ~ 2006-01-09
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
 	 
   [   �Լ�����   ] 
	  �α׿� ���õ� ��������( ���� �߶󺸳� Ƚ���� ���� )

   [ RETURN VALUE ]
      0    : ���� 
      ���� : ���� 
--------------------------------------------------------------------------------------------*/
INT CSMS_LifeLong_Sender::AddSMSLog(INT nDCnt)
{
	CString strTemp;
	strTemp.Format( _T("%d"), nDCnt );
	m_lstDCnt.AddTail( strTemp );
	return 0;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2006-01-09 ~ 2006-01-09
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
 	 
   [   �Լ�����   ] 
	  �α����̺� ����Ѵ�.

   [ RETURN VALUE ]
      0    : ���� 
      ���� : ���� 
--------------------------------------------------------------------------------------------*/
INT CSMS_LifeLong_Sender::InsertSMSLog()
{
	INT ids = 0;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMSSendDlg") );
	
	pDM->StartFrame();
	pDM->InitDBFieldData();

	// 2006-01-09 UPDATE BY PDW(Jr) ======================================================
	// ���� �̷°����� ���� �ʵ� �߰� �� ( EMMA_KEY )
	const INT cnt = 14;
	CString fieldName[ cnt ] = {
		_T("REC_KEY"), 
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
		_T("EMMA_KEY") };
	CString fieldType[ cnt ] = {
		_T("NUMERIC"), 
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
		_T("NUMERIC") };
	
	CString data[ cnt ];	
	CTime time = pDM->GetDBTime();
	data[  0 ] = _T("ESL_SMS_SEQ.NEXTVAL");
	data[  5 ] = m_pCSMS_LifeLong->m_sSMSType;
	// 2006-01-11 UPDATE BY PDW(Jr) ===========================================================
	// ���۽ð� SYSDATE�� ����
	/*
	data[  7 ].Format( _T("%d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	*/
	data[  7 ] = _T("SYSDATE");
	// ========================================================================================
	data[  8 ] = GetUserID();
	data[  9 ] = _T("Y");
	data[ 11 ] = GetWorkLogMsg( _T("SMS �߼�"), __WFILE__, __LINE__ );
	
	INT rowCnt = m_receiverName.GetCount();

	for( INT i = 0; i < rowCnt; i++ )
	{
		data[ 1 ]  = _T("");														// ������ KEY
		data[ 2 ]  = m_receiverName.GetAt( m_receiverName.FindIndex( i ) );	    // ������ ��
		data[ 3 ]  = m_receiverNum.GetAt( m_receiverNum.FindIndex( i ) );	    // ������ HP
		data[ 4 ]  = m_msg.GetAt( m_msg.FindIndex( i ) );	                    // �߼� �޼��� 
		data[ 6 ]  = m_lstDCnt.GetAt( m_lstDCnt.FindIndex( i ) );				// �߼� Ƚ��
		data[ 13 ] = m_lstEmmaKey.GetAt( m_lstEmmaKey.FindIndex( i ) );			// EMMA KEY

		for( INT j = 0; j < cnt; j++ )
		{
			if( !data[ j ].IsEmpty() )
				pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ], true );
		}

		pDM->MakeInsertFrame( _T("MN_SMS_LOG_TBL") );
	}

	if ( rowCnt > 0 ) 
	{
		pDM->SendFrame();
	}

	pDM->EndFrame();	
	
	m_lstDCnt.RemoveAll();
	m_lstEmmaKey.RemoveAll();

	return 0;
}
