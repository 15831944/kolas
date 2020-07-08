// XroshorSMSSender.cpp: implementation of the CXroshorSMSSender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XroshorSMSSender.h"
#include "SMSSendDlg.h"
#include "SMSMessageConvert.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXroshorSMSSender::CXroshorSMSSender( CESL_Mgr *pParent )
{
	m_initSet = false;
	m_bind = false;
	b_submit_ack = false;
	b_report_ack = false;
	user_type = USER_TYPE_PERSONAL;
	user_access_ip = _T("127.0.0.1");
	m_pParent = pParent;
}

CXroshorSMSSender::~CXroshorSMSSender()
{
	UnbindXShot();	
}

// ���� CXroshorSMSSender �ʱ�ȭ �Ѵ�. �̰� ���� ������ �޼��� ���� �� ����!
INT CXroshorSMSSender::InitSetXShot(
								CString bind_id,
								CString bind_pwd,
								CString bind_domain,
								CString user_phone_id,
								CString call_back,
								CString confFilePath )
{
	this->bind_id = bind_id;
	this->bind_pwd = bind_pwd;
	this->bind_domain = bind_domain;
	this->user_phone_id = user_phone_id;
	this->call_back = call_back;

	//++2009.01.19 ADD BY CJY {{++
	//DEL ũ�μ�API�� ANSI��� �Լ��̴�.
	CString strConvert = confFilePath;
	m_confFilePath = confFilePath;
	
	int len; 
	len = WideCharToMultiByte(CP_ACP, 0, strConvert, -1, NULL, 0, NULL,NULL);
	char* ctmp = new char[len]; 
	// ���� ��ȯ
	WideCharToMultiByte(CP_ACP, 0, strConvert, -1, ctmp, len, NULL, NULL);			
	if(FALSE == x_msggate_conf_init(ctmp))
	{
		return -1;
	}
	delete []ctmp;
//DEL	�����ҽ�
//DEL 	m_confFilePath = confFilePath;
//DEL 	if( !x_msggate_conf_init( m_confFilePath.GetBuffer( 0 ) ) )
//DEL 		return -1;	// x_msggate.conf �� ���� ����
	//--2009.01.19 ADD BY CJY --}}

	m_initSet = true;

	return 0;
}

// Bind �ϱ�.
BOOL CXroshorSMSSender::BindXShot()
{
	UnbindXShot();
	
	//++2009.01.19 ADD BY CJY {{++
	//DEL ũ�μ�API�� ANSI��� �Լ��̴�.
	const int CONVERT_STRING_COUNT = 3;
	CString strConvert[CONVERT_STRING_COUNT] = 
	{
		bind_id, bind_pwd, bind_domain
	};
	char* ctmp[CONVERT_STRING_COUNT] = 
	{
		NULL, NULL, NULL
	};
	
	for(int nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, NULL, 0, NULL,NULL);
		ctmp[nCnt] = new char[len];
		// ���� ��ȯ
		WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, ctmp[nCnt] , len, NULL, NULL);					
	}
	INT ids = fn_x_bind(
		user_type,
		ctmp[0],		// bind_id
		ctmp[1],		// bind_pwd
		ctmp[2],		// bind_domain
		b_submit_ack,
		lpfn_submit,
		b_report_ack,
		lpfn_report,
		&x_ret_code );

	for(nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		delete []ctmp[nCnt];
	}	
//DEL	�����ҽ�
//DEL 	INT ids = fn_x_bind(
//DEL 						user_type,
//DEL 						bind_id.GetBuffer( 0 ),
//DEL 						bind_pwd.GetBuffer( 0 ),
//DEL 						bind_domain.GetBuffer( 0 ),
//DEL 						b_submit_ack,
//DEL 						lpfn_submit,
//DEL 						b_report_ack,
//DEL 						lpfn_report,
//DEL 						&x_ret_code );
	//--2009.01.19 ADD BY CJY --}}

	if( X_RET_FUNC_SUCCESS == ids )
		return m_bind = true;
	return false;
}

// Unbind �ϱ�.
BOOL CXroshorSMSSender::UnbindXShot()
{
	if( !m_bind )
		return false;
	
	//++2009.01.19 ADD BY CJY {{++
	//DEL ũ�μ�API�� ANSI��� �Լ��̴�.
	const int CONVERT_STRING_COUNT = 2;
	CString strConvert[CONVERT_STRING_COUNT] = 
	{
		bind_id, bind_pwd
	};
	char* ctmp[CONVERT_STRING_COUNT] = 
	{
		NULL, NULL
	};
	
	for(int nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, NULL, 0, NULL,NULL);
		ctmp[nCnt] = new char[len];
		// ���� ��ȯ
		WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, ctmp[nCnt] , len, NULL, NULL);					
	}
	int ids = fn_x_unbind(
		ctmp[0],		// bind_id
		ctmp[1]			// bind_pwd
		);
	for(nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		delete []ctmp[nCnt];
	}	
//DEL	�����ҽ�
//DEL 	INT ids = fn_x_unbind(
//DEL 		bind_id.GetBuffer( 0 ),
//DEL 		bind_pwd.GetBuffer( 0 ) );	
	//--2009.01.19 ADD BY CJY --}}

	if( X_RET_FUNC_SUCCESS == ids )
	{
		m_bind = false;
		return true;
	}
	return false;
}

// Money Status ���.
BOOL CXroshorSMSSender::GetMoneyStatus()
{
	INT pro_status;
	INT bill_main_code;
	INT	bill_sub_code;
	INT	avail_money;
	INT	reserved_money;
	INT	sms_price;
	INT	fax_price;
	INT	voice_cel_unit;
	INT	voice_cel_price;
	INT	voice_tel_unit;
	INT	voice_tel_price;

	//++2009.01.19 ADD BY CJY {{++
	//DEL ũ�μ�API�� ANSI��� �Լ��̴�.
	const int CONVERT_STRING_COUNT = 4;
	CString strConvert[CONVERT_STRING_COUNT] = 
	{
		bind_id, bind_domain, user_phone_id, user_access_ip
	};
	char* ctmp[CONVERT_STRING_COUNT] = 
	{
		NULL, NULL, NULL, NULL
	};
	
	for(int nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, NULL, 0, NULL,NULL);
		ctmp[nCnt] = new char[len];
		// ���� ��ȯ
		WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, ctmp[nCnt] , len, NULL, NULL);					
	}
	INT ids = fn_x_money_status(
								ctmp[0],		// bind_id
								ctmp[1],		// bind_domain
								ctmp[2],		// user_phone_id
								ctmp[3],		// user_access_ip
								&bill_main_code,
								&bill_sub_code,
								&pro_status,
								&avail_money,
								&reserved_money,
								&sms_price,
								&fax_price,
								&voice_cel_unit,
								&voice_cel_price,
								&voice_tel_unit,
								&voice_tel_price,
								&x_ret_code );

	for(nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		delete []ctmp[nCnt];
	}
//DEL	�����ҽ�	
//DEL 	INT ids = fn_x_money_status(
//DEL 								bind_id.GetBuffer( 0 ),
//DEL 								bind_domain.GetBuffer( 0 ),
//DEL 								user_phone_id.GetBuffer( 0 ),
//DEL 								user_access_ip.GetBuffer( 0 ),
//DEL 								&bill_main_code,
//DEL 								&bill_sub_code,
//DEL 								&pro_status,
//DEL 								&avail_money,
//DEL 								&reserved_money,
//DEL 								&sms_price,
//DEL 								&fax_price,
//DEL 								&voice_cel_unit,
//DEL 								&voice_cel_price,
//DEL 								&voice_tel_unit,
//DEL 								&voice_tel_price,
//DEL 								&x_ret_code );
	//--2009.01.19 ADD BY CJY --}}

	if( X_RET_FUNC_SUCCESS == ids )
		return true;
	return false;
}

// ���糯¥�ð� ���.
CString CXroshorSMSSender::GetNowTime()
{
	CTime time = CTime::GetCurrentTime();
	CString now;
	now.Format( _T("%04d%02d%02d%02d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
	return now;
}

// �̹� bind �Ǿ� �ִ� ���¿��� ���� ���� ������ �Լ�
INT CXroshorSMSSender::SendSMSMsg(
								CString receiverName,
								CString receiverNum,
								CString title,
								CString msg,
								CString callbackNum )
{
	if( callbackNum.IsEmpty() )
		callbackNum = call_back;
	
	CString receiver;
	receiver.Format( _T("%s:%s;"), receiverName, receiverNum );

	//++2009.01.19 ADD BY CJY {{++
	//DEL ũ�μ�API�� ANSI��� �Լ��̴�.
	const int CONVERT_STRING_COUNT = 9;
	CString strConvert[CONVERT_STRING_COUNT] = 
	{
		bind_id, bind_domain, user_phone_id, user_access_ip, title, GetNowTime(), callbackNum, receiver, msg
	};
	char* ctmp[CONVERT_STRING_COUNT] = 
	{
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
	};
	
	for(int nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, NULL, 0, NULL,NULL);
		ctmp[nCnt] = new char[len];
		// ���� ��ȯ
		WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, ctmp[nCnt] , len, NULL, NULL);					
	}

	INT ids = fn_x_submit_2(
							X_METHOD_SMS,
							X_SUBTYPE_TTS,
							1,
							ctmp[0],		// bind_id
							ctmp[1],		// bind_domain
							ctmp[2],		// user_phone_id
							ctmp[3],		// user_access_ip
							WM_USER + 1010,
							ctmp[4],		// title
							ctmp[5],		// GetNowTime()
							ctmp[6],		// callbackNum
							0,
							1,
							ctmp[7],		// receiver
							ctmp[8],		// msg
							"",
							0,
							"");

	for(nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		delete []ctmp[nCnt];
	}


//DEL 	INT ids = fn_x_submit_2(
//DEL 							X_METHOD_SMS,
//DEL 							X_SUBTYPE_TTS,
//DEL 							1,
//DEL 							bind_id.GetBuffer( 0 ),
//DEL 							bind_domain.GetBuffer( 0 ),
//DEL 							user_phone_id.GetBuffer( 0 ),
//DEL 							user_access_ip.GetBuffer( 0 ),
//DEL 							WM_USER + 1010,
//DEL 							title.GetBuffer( 0 ),
//DEL 							GetNowTime().GetBuffer( 0 ),
//DEL 							callbackNum.GetBuffer( 0 ),
//DEL 							0,
//DEL 							1,
//DEL 							receiver.GetBuffer( 0 ),
//DEL 							msg.GetBuffer( 0 ),
//DEL 							_T(""),
//DEL 							0,
//DEL 							_T("") );
	
	if( 1 == ids )
		return 0;
	else
		return ids;
}

// ���� �Ѱ� �ִ� ���� ��ŭ ���� ���� �ڸ���
// msg : ���� ���� -> �ڸ��� ������ ����
CString CXroshorSMSSender::GetDivideMsg( CString &msg )
{
	CSMSMessageConvert ConvertDlg;
	CString sendMsg;
	INT length;	

	CHAR		*szMsg;		
	CHAR		szCutMsg[81];
	INT			nLen;

	ConvertDlg.UnicodeToMultiByte(msg.GetBuffer(0), &szMsg, &nLen );	

	if( IsCanCut( szMsg, 80 ) )
		length = 80;
	else
		length = 80 - 1;

	strncpy(szCutMsg , szMsg, length);
	szCutMsg[length] = '\0';


	TCHAR *szUnicodeMessage;
	INT		nUnicodeLen;

	ConvertDlg.MultiByteToUnicode(szCutMsg, &szUnicodeMessage, &nUnicodeLen );

	sendMsg.Format(_T("%s"), szUnicodeMessage );
	
	msg = msg.Mid( sendMsg.GetLength() );

	return sendMsg;
}

// ���ڰ� �ѱ��� ��� ��� �߶�� ���� ��ġ ���ϱ�
bool CXroshorSMSSender::IsCanCut( CString str, INT pos )
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
		return true;
	
	//¦���� ���̳��� TRUE
	bool result = !( ( pos - nLastPos ) % 2 );
	return result;
}

// ������ ��ȭ��ȣ�� �ǹٸ� �������� �˻�
INT CXroshorSMSSender::CheckPhoneNum( CString phoneNum )
{
	phoneNum.Replace( _T("-"), _T("") );
	
	CString tmp;
	TCHAR num;
	for( INT i = 0; i < phoneNum.GetLength(); i++ )
	{
		num = phoneNum.GetAt( i );

		if( num > 57 || num < 48 )
			continue;

		tmp += phoneNum.Mid( i, 1 );
	}
	phoneNum = tmp;

	INT length = phoneNum.GetLength();
	if( 10 != length && 11 != length )
		return -1;

	CStringList hpNumList;
	if( CLocCommonAPI::GetHPCorpNumList( hpNumList ) )
		CLocCommonAPI::MakeDefaultHPNumList( hpNumList );
	
	bool flag = false;
	for( i = 0; i < hpNumList.GetCount(); i++ )
	{
		CString pNum = hpNumList.GetAt( hpNumList.FindIndex( i ) );
		if( phoneNum.Left( pNum.GetLength() ) == pNum )
		{
			flag = true;
			break;
		}
	}
	if( !flag )
		return -2;
	
	return 0;
}

// SMS �� ������ �غ� �Ǿ��ִ��� �˻��ϰ� bind ���� money status ��������
INT CXroshorSMSSender::ReadySMS()
{
	if( m_bind )
		return 0;
	
	if( !m_initSet )
		return -1;		// �ʱ�ȭ�� �Ǿ����� ���� ����

 	if( !BindXShot() )
 		return -2;		// Binding�� ����

	if( !GetMoneyStatus() )
		return -3;		// money Status �� �������� ����.

	return 0;
}

// ���� ������ - 1ȸ ���� �ִ� ���̸� ���� ��� �߶� ��� ������.
INT CXroshorSMSSender::SendMessage( CString receiverName, CString receiverNum, CString title, CString msg, CString callbackNum, INT &divideCnt )
{
	divideCnt = 1;
	while( true )
	{
		if( SendSMSMsg( receiverName, receiverNum, title, GetDivideMsg( msg ), callbackNum ) )
			return -5;

		if( msg.IsEmpty() )
			break;
	
		divideCnt++;
	}

	return 0;

}

// ���� �Ѱ��� ������ - ���� ���̰� 80bytes�� ������ ©�� ������. 
INT CXroshorSMSSender::SendSMSMsg(
								CString receiverName,
								CString receiverNum,
								CString title,
								CString msg,
								INT &divideCnt,
								CString callbackNum )
{
	INT ids;
	ids = ReadySMS();
	if( ids )
		return ids;
	
	if( CheckPhoneNum( receiverNum ) )
		return -4;
	
	if( SendMessage( receiverName, receiverNum, title, msg, callbackNum, divideCnt ) )
		return -5;

	if( !UnbindXShot() )
		return -6;		// Unbinding�� ����


	return 0;
}

// SMS ���� ����Ʈ�� ������� ������ ���� ������.
INT CXroshorSMSSender::SendSMSMsgThread( LPVOID pParam )
{
	struct SMS_LIST *pSMSList = ( SMS_LIST * )pParam;
	return SendSMSMsg(
					pSMSList->receiverName,
					pSMSList->receiverNum,
					pSMSList->title,
					pSMSList->msg );
}

// �������� ���ڸ� ���� ���� ������
INT CXroshorSMSSender::SendSMSMsg(
								CStringList &receiverName,
								CStringList &receiverNum,
								CString title,
								CStringList &msg,
								CString callbackNum,
								VOID *pParent )
{
	INT dCnt = 0;
	INT SMSCnt = msg.GetCount();
	
	CString receiverPNum;
	INT length = 0, ids;
	
	INT cnt = receiverName.GetCount();
	for( INT idx = 0; idx < cnt; idx++ )
	{
		// SMS ���� �������� Ȯ��
		if( GetStopStatus( pParent ) )
		{
			SendErrorMsg( 0, 0, 0, 2 );
			UnbindXShot();
			return 0;
		}
		
		// ������ �ڵ��� ��ȣ�� �ǹٸ��� �˻�
		receiverPNum = receiverNum.GetAt( receiverNum.FindIndex( idx ) );
		if( CheckPhoneNum( receiverPNum ) )
		{
			SendErrorMsg( cnt, idx, 0, -4 );
			continue;
		}
		
		// ���� bind �� �Ѵ�. 
		if( !idx )
		{
			ids = ReadySMS();
			if( ids )
			{
				SendErrorMsg( cnt, idx, 0, ids );
				continue;
			}
		}

		// 1ȸ bind �� ���� �� �ִ� ���ڿ� ���̸� �ʰ��ϸ� unbind �� �ٽ� bind �Ѵ� 
		length += msg.GetAt( msg.FindIndex( idx ) ).GetLength();
		if( length > SMS_TOTAL_LENGTH )
		{
			length = 0;
			UnbindXShot();

			ids = ReadySMS();
			if( ids )
			{
				SendErrorMsg( cnt, idx, 0, ids );
				continue;
			}		
		}
		
		// SMS ����
		ids = this->SendMessage( 
						receiverName.GetAt( receiverName.FindIndex( idx ) ),
						receiverPNum,
						title,
						msg.GetAt( msg.FindIndex( idx ) ),
						callbackNum,
						dCnt );
		if( ids )
		{
			SendErrorMsg( cnt, idx, 0, -5 );
			continue;
		}
		
		SendErrorMsg( cnt, idx, dCnt, 0 );
	}
	
	SendErrorMsg( 0, 0, 0, 1 );
	
	ids = UnbindXShot();
	if( ids )
		return -6;
	
	return 0;
}

// �θ𿡰� ���۰���� ������.
VOID CXroshorSMSSender::SendErrorMsg( INT cnt, INT idx, INT dCnt, INT error )
{
	if( !m_pParent )
		return;
	
	CString statusMsg;
	statusMsg.Format( _T("%d/%d/%d/%d"), cnt, idx, dCnt, error );

	::SendMessage( m_pParent->m_hWnd, WM_USER + 1005, ( WPARAM )&statusMsg, true );
}

// �θ�� ���� �������� ���� �̺�Ʈ�� �Դ��� �˻�
bool CXroshorSMSSender::GetStopStatus( VOID *pParent )
{
	if( !pParent )
		return false;

	CSMSSendDlg *parentDlg;
	parentDlg = NULL;
	parentDlg = ( CSMSSendDlg * )pParent;
	return parentDlg->m_bSMSStop;
}
