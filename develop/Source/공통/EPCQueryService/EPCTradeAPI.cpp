// EPCTradeAPI.cpp: implementation of the CEPCTradeAPI class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "EPCTradeAPI.h"
#include "mbstring.h"
#include "eco_file_api.h"
#include "..\\..\\ESL\\ESL\\COM_File_Mgr.h"
#include "URLEncode.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CEPCTradeAPI::CEPCTradeAPI(INT SIGNATURE)
CEPCTradeAPI::CEPCTradeAPI()
{
	CoInitialize(NULL);
	m_nMode = -1;	

}

CEPCTradeAPI::~CEPCTradeAPI()
{
	CoUninitialize();
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  10�ڸ� ISBN�� 13�ڸ��� �����Ѵ�.
//  Header�� '978' || '979' ����
// [Mini Spec]
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::ConvertISBN13(CString strISBN, CString strConvertHeader, CString &strISBN13)
{	
	INT		nCheckSum  = 0;
	INT		nCheckRest = 0;
	
	if( 10 != strISBN.GetLength() )		
	{
		// ISBN �ڸ��� ����!
		return -100;
	}

	if( "89" != strISBN.Mid(0,2) )		
	{
		// ���� �ڵ� ����!
		return -100;
	}

	if( "978" == strConvertHeader )
	{
		strISBN = strConvertHeader + strISBN;

		for( INT i=0;i<12;i++ )
		{
			if( i%2 )
			{
				nCheckSum += (strISBN.GetAt(i) - '0')*3;
			}
			else
			{
				nCheckSum += (strISBN.GetAt(i) - '0')*1;
			}
		}

		nCheckRest = nCheckSum%10;

		strISBN13.Empty();

		if( nCheckRest )
		{
			strISBN13.Format( _T("%s%d"), strISBN.Mid(0,12), (10-nCheckRest) );			
		}
		else
		{
			strISBN13.Format( _T("%s%d"), strISBN.Mid(0,12), nCheckRest );
		}
	}
	else if( _T("979") == strConvertHeader )
	{
		// ���� �������� �ʴ´�.
	}
	else
	{
		// ���� ��� ����!
		return -200;
	}
	 
	return 0;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  13�ڸ� ISBN�� SGTIN96 �ڵ�� �����Ѵ�.
// [Mini Spec]
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::ConvertSGTIN96(CString strISBN13, CString &strSGTIN96)
{
	//++2008.08.22 ADD BY CJY {{++
	if( 13 != strISBN13.GetLength() )
	{
		return -100;
	}

	/// 13�ڸ� �����ڵ� 89�� ��ȯ�����ϴ�
	if( "97889" != strISBN13.Mid(0,5) )
	{
		return -200;
	}
	//--2008.08.22 ADD BY CJY --}}

	INT		ids;
	CString strRegistrantElement;
	CString strPublicationElement;
	CString strHeadder            = "urn:epc:tag:sgtin-96";
	CString strFilter             =  "1";
	CString strEnterpriseElement;
	CString strProductElement;
	INT		nProductElementLength = 0;
	
	ids = GetRegistrantElement( strISBN13, strRegistrantElement );
	if( 0 > ids ) return ids;

	ids = GetPublicationElement( strISBN13, strPublicationElement );
	if( 0 > ids ) return ids;
	
	// ��ü �ڵ�
	strEnterpriseElement = strISBN13.Mid(0,5) + strRegistrantElement;

	// ��ǰ �ڵ�
	nProductElementLength = 13 - strEnterpriseElement.GetLength();
	
	for( INT i=0;i<nProductElementLength-strPublicationElement.GetLength();i++ )
	{
		strProductElement = "0" + strProductElement;
	}

	strProductElement = strProductElement + strPublicationElement;
	
	// ����
	strSGTIN96 = strHeadder + ":" + strFilter + "." + strEnterpriseElement + "." + strProductElement;

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  13�ڸ� ISBN���� �����ڹ�ȣ�� �����´�.
// [Mini Spec]
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::GetRegistrantElement(CString strISBN13, CString &strRegistrantElement)
{
	INT		nCheckNo = 0;
	INT		nLength  = 0;

	if( 13 != strISBN13.GetLength() )
	{
		// ISBN �ڸ��� ����!
		return -100;
	}

	// ���κο� ������ȣ�� 5�ڸ��� �����̴�.	
	// �����ڹ�ȣ�� 2~6�ڸ� ��ȣ�� ���� ���ڸ��� ������ �ڸ����� ���������� �ִ�.
	nCheckNo = _ttoi( strISBN13.Mid(5,2) );
	
	strRegistrantElement.Empty();

	if( 0<= nCheckNo && 24 >= nCheckNo )		// 2
	{
		nLength = 2;
	}
	else if( 25<= nCheckNo && 54 >= nCheckNo )	// 3
	{
		nLength = 3;
	}
	else if( 55<= nCheckNo && 84 >= nCheckNo )	// 4
	{
		nLength = 4;
	}
	else if( 85<= nCheckNo && 94 >= nCheckNo )	// 5
	{
		nLength = 5;
	}
	else if( 95<= nCheckNo && 99 >= nCheckNo )	// 6 Length
	{
		nLength = 6;
	}
	
	if( nLength )
	{
		strRegistrantElement = strISBN13.Mid(5,nLength);
	}

	return nLength;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  13�ڸ� ISBN���� ����ĺ���ȣ�� �����´�.
// [Mini Spec]
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::GetPublicationElement(CString strISBN13, CString &strPublicationElement)
{
	INT		ids;
	CString strRegistrantElement;
	INT		nRegistrantElementLength  = 0;
	INT     nPublicationElementLength = 0;

	if( 13 != strISBN13.GetLength() )
	{
		// ISBN �ڸ��� ����!
		return -100;
	}

	// ���κο� ������ȣ�� 5�ڸ��� �����̴�.
	// �����ڹ�ȣ �ڸ����� �����̴�.
	ids = GetRegistrantElement( strISBN13, strRegistrantElement );
	if( 0 > ids ) return ids;

	nRegistrantElementLength = strRegistrantElement.GetLength();
	
	// �����ڹ�ȣ + ����ĺ���ȣ �ڸ��� ���� 7�ڸ��̴�.
	// ����ĺ���ȣ �ڸ��� = 7 - �����ڹ�ȣ �ڸ���
	nPublicationElementLength = 7 - nRegistrantElementLength;

	if( nRegistrantElementLength )
	{
		strPublicationElement.Format( _T("%s"), strISBN13.Mid( 5 + nRegistrantElementLength, nPublicationElementLength ) );
	}

	return nPublicationElementLength;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  MTB �ڵ彦��ũ�� CFG�� ���� �����´�.
// [Mini Spec]
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::GetHandShake(CString &strHandShake)
{
	if( m_nMode == QWS1 )
	{
		strHandShake = "QWS110X1X1NA0000END1";
	}
	else if( m_nMode == BCS1 )
	{
		// 2009.03.31 DEL BY CJY :  ISSUE_ID�� ���� �ٸ���.
		// strHandShake = "QWS110X1X1NA0000END1";
		strHandShake = "BCS110X1X1NA0000END1";
	}

	return strHandShake.GetLength();
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  ������ ���� �����͸� �����´�.
// [Mini Spec]
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::GetDataToServer(CString strParam, CString &strGetData, INT nTransMode/*=TCP_IP*/)
{		
	if( 0 > strParam.GetLength() )
	{
		// �Է°� ����
		return -100;
	}

	switch( nTransMode )
	{
		case TCP_IP:
			{
				return GetDataToTCP_IP( strParam, strGetData );
			}
		case WEBSERVICE:
			{
				return GetDataToWB( strParam, strGetData );
			}
		default:
			{
				// NOT DEFIND
				return -300;
			}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  tcp/ip ����� ���� �����͸� �����´�.
// [Mini Spec]
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::GetDataToTCP_IP(CString strParam, CString &strGetData)
{
	CString strHandShake;	

	if( 0 > strParam.GetLength() )
	{
		// �Է°� ����
		return -100;
	}

	if( 0 >= GetHandShake( strHandShake ) )
	{
		// �ڵ彦��ũ ����
		return -200;
	}

	/// API Calling~
	INT nMode = 1;
	if( m_nMode == QWS1 )
	{
		nMode = 1;
	}
	else if( m_nMode == BCS1 )
	{
		nMode = 2;
	}		
	
	return EPCAPI.DataToSearch97( strHandShake, strParam, strGetData );	
}

INT CEPCTradeAPI::SetConServer(INT nFuncType)
{
	INT ids;

	ids = EPCAPI.InitEnvVar(nFuncType);
	if (ids) return -5;

	if ( nFuncType == ISSUE_ID           ) m_nMode = QWS1;
	else if ( nFuncType == EPC_CHECK_IN  ) m_nMode = BCS1;
	else if ( nFuncType == LOAN_HISTORY  ) m_nMode = BCS1;
	else if ( nFuncType == MAKE_NEW_INFO ) m_nMode = BCS1;
	else if ( nFuncType == DEL_BOOK_INFO ) m_nMode = BCS1;
	else -10;

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  �����񽺸� ����Ͽ� �����͸� �����´�.
//  ##########�̱���##########
// [Mini Spec]
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::GetDataToWB(CString strParam, CString &strGetData)
{
	if( 0 > strParam.GetLength() )
	{
		// �Է°� ����
		return -100;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  �ش��±��� Value�� �����´�.
//  ------------- ������ -------------
// [Mini Spec]
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::GetFirstTagValue(IXMLDOMNodePtr pDom, CString strTagName, CString &strGetValue)
{
	INT			ids;
	CComBSTR	bstrTagName;
	CComBSTR	bstrTagValue;

	if( 0 > pDom->get_baseName(&bstrTagName) )
	{
			return -100;
	}

	if( CString(bstrTagName) == strTagName )
	{
		if( 0 > pDom->get_text(&bstrTagValue) )
		{
			return -200;
		}

		strGetValue = CString(bstrTagValue);
	}
	else
	{
		IXMLDOMNodeListPtr pNodeList = pDom->childNodes;
		
		for( LONG i=0;i<pNodeList->length;i++ )
		{
			IXMLDOMNodePtr pObjectNode = pNodeList->item[i];
			ids = GetFirstTagValue( pObjectNode, strTagName, strGetValue );
			if( 0 > ids )
			{
				pObjectNode.Release();
				return ids;
			}

			pObjectNode.Release();
		}

		pNodeList.Release();
	}

	return strGetValue.GetLength();
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  �ش��±��� ������ �����´�.
//  ------------- ������ -------------
// [Mini Spec]
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::GetTagCount(IXMLDOMNodePtr pDom, CString strTagName, INT &nCount)
{
	INT			ids;
	CComBSTR	bstrTagName;

	if( 0 > pDom->get_baseName(&bstrTagName) )
	{
		return -100;
	}

	if( CString(bstrTagName) == strTagName )
	{
		nCount++;
		return 0;
	}
	else
	{
		IXMLDOMNodeListPtr pNodeList = pDom->childNodes;
		
		for( LONG i=0;i<pNodeList->length;i++ )
		{
			IXMLDOMNodePtr pObjectNode = pNodeList->item[i];
			ids = GetTagCount( pObjectNode, strTagName, nCount );
			if( 0 > ids )
			{
				pObjectNode.Release();
				return ids;
			}

			pObjectNode.Release();
		}

		pNodeList.Release();
	}

	return 0;
}
//  ------------- ������ -------------
INT CEPCTradeAPI::GetTagCount(IXMLDOMNodePtr pDom, CString strTagName)
{
	INT		ids;
	INT		nCount = 0;

	ids = GetTagCount( pDom, strTagName, nCount );
	if( 0 > ids ) return ids;

	return nCount;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  �� Ŀ�ǵ庰�� cfg������ default xml�� load
// [Mini Spec]
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::MakeDefaultXML(INT nType, MSXML2::IXMLDOMDocumentPtr& pDom)
{
	CString strFileName;

	switch( nType )
	{
		case ISSUE_ID:
			{
				strFileName = "..\\CFG\\BOOKTRADE\\XML\\ISSUE_ID.XML";				
			}
			break;
		case EPC_CHECK_IN:
			{
				strFileName = "..\\CFG\\BOOKTRADE\\XML\\CHECKIN.XML";				
			}
			break;
		case LOAN_HISTORY:
			{
				strFileName = "..\\CFG\\BOOKTRADE\\XML\\LOANHISTORY.XML";				
			}
			break;
		case MAKE_NEW_INFO:
			{
				strFileName = "..\\CFG\\BOOKTRADE\\XML\\MAKENEWBOOK.XML";				
			}
			break;
		case DEL_BOOK_INFO:
			{
				strFileName = "..\\CFG\\BOOKTRADE\\XML\\DELBOOKINFO.XML";				
			}
			break;
		default:
			return -100;
	}

	if( !pDom->load( strFileName.GetBuffer(0) ) )
	{
		// XML LOAD ERROR!!
		return -200;
	}

	return 0;
}

INT CEPCTradeAPI::LoadAllXML()
{

	FILE *   fp;
	TCHAR*   buffer;
	TCHAR*   ptr   ;
	CString strFileName, strFileStream;

	CCOM_File_Mgr FileMgr;

	// ISSUE_ID
	strFileName = "..\\CFG\\BOOKTRADE\\XML\\ISSUE_ID.TXT";
	fp = FileMgr.ReadFileToUTF16FromAllFile(strFileName.GetBuffer(0), TRUE);
	if ( fp == NULL ) return -200;
	
	buffer = (TCHAR*)malloc( sizeof(TCHAR)*50000 );
	
	while ( (ptr = FdkFgets( buffer , 50000 , fp )) != NULL )
	{
		strFileStream += buffer;			
	}
	free( buffer );	
	m_strXMLQuery.Add(strFileStream);
	FileMgr.ColseFileUTF16(fp);
	strFileStream=_T("");

	// EPC_CHECK_IN
	strFileName = "..\\CFG\\BOOKTRADE\\XML\\CHECKIN.TXT";
	fp = FileMgr.ReadFileToUTF16FromAllFile(strFileName.GetBuffer(0), TRUE);
	if ( fp == NULL ) return -200;
	
	buffer = (TCHAR*)malloc( sizeof(TCHAR)*50000 );
	
	while ( (ptr = FdkFgets( buffer , 50000 , fp )) != NULL )
	{
		strFileStream += buffer;			
	}
	free( buffer );	
	m_strXMLQuery.Add(strFileStream);
	FileMgr.ColseFileUTF16(fp);	
	strFileStream=_T("");

	// LOAN_HISTORY
	strFileName = "..\\CFG\\BOOKTRADE\\XML\\LOANHISTORY.TXT";
	fp = FileMgr.ReadFileToUTF16FromAllFile(strFileName.GetBuffer(0), TRUE);
	if ( fp == NULL ) return -200;
	
	buffer = (TCHAR*)malloc( sizeof(TCHAR)*50000 );
	
	while ( (ptr = FdkFgets( buffer , 50000 , fp )) != NULL )
	{
		strFileStream += buffer;			
	}
	free( buffer );	
	m_strXMLQuery.Add(strFileStream);
	FileMgr.ColseFileUTF16(fp);	
	strFileStream=_T("");

	// MAKE_NEW_INFO
	strFileName = "..\\CFG\\BOOKTRADE\\XML\\MAKENEWBOOK.TXT";
	fp = FileMgr.ReadFileToUTF16FromAllFile(strFileName.GetBuffer(0), TRUE);
	if ( fp == NULL ) return -200;
	
	buffer = (TCHAR*)malloc( sizeof(TCHAR)*50000 );
	
	while ( (ptr = FdkFgets( buffer , 50000 , fp )) != NULL )
	{
		strFileStream += buffer;			
	}
	free( buffer );	
	m_strXMLQuery.Add(strFileStream);
	FileMgr.ColseFileUTF16(fp);	
	strFileStream=_T("");

	// DEL_BOOK_INFO
	strFileName = "..\\CFG\\BOOKTRADE\\XML\\DELBOOKINFO.TXT";
	fp = FileMgr.ReadFileToUTF16FromAllFile(strFileName.GetBuffer(0), TRUE);
	if ( fp == NULL ) return -200;
	
	buffer = (TCHAR*)malloc( sizeof(TCHAR)*50000 );
	
	while ( (ptr = FdkFgets( buffer , 50000 , fp )) != NULL )
	{
		strFileStream += buffer;			
	}
	free( buffer );	
	m_strXMLQuery.Add(strFileStream);
	FileMgr.ColseFileUTF16(fp);	
	strFileStream=_T("");

	return 0;
}


INT CEPCTradeAPI::LoadXMLQuery(INT nQueryType, CString &strXML)
{	
	CString strFileName, strFileStream;

	//++2008.09.25 ADD BY CJY {{++
	// �� �Լ��� �ҷ����� ���� LoadAllXML()�Լ��� ���� ȣ��Ǿ�� �Ѵ�.
	// ������ �������� ����ó��	
	const int nCnt = m_strXMLQuery.GetSize();
	if(5 > nCnt)
	{
		TCHAR szPath[200] = {0}; 
		GetCurrentDirectory(MAX_PATH, szPath);
		CString strPath = szPath;
		strPath.TrimRight(_T("\\bin"));
		CString strErrorMsg;
		strErrorMsg.Format(
			_T("[%s\\CFG\\BOOKTRADE\\XML\\]��ο� ������ ������ �ֽ��ϴ�.\r\n")
			_T("���ǹ��������� �ۼ��� �� �����ϴ�.\r\n"),
			strPath
			);
		AfxMessageBox(strErrorMsg);
		return -1;
	}
	//--2008.09.25 ADD BY CJY --}}

	switch( nQueryType )
	{
		case ISSUE_ID:
			{
				strXML.Format(_T("%s"), m_strXMLQuery.GetAt(0));
			}
			break;
		case EPC_CHECK_IN:
			{
				strXML.Format(_T("%s"), m_strXMLQuery.GetAt(1));
			}
			break;
		case LOAN_HISTORY:
			{
				strXML.Format(_T("%s"), m_strXMLQuery.GetAt(2));
			}
			break;
		case MAKE_NEW_INFO:
			{
				strXML.Format(_T("%s"), m_strXMLQuery.GetAt(3));
			}
			break;
		case DEL_BOOK_INFO:
			{
				strXML.Format(_T("%s"), m_strXMLQuery.GetAt(4));
			}
			break;
		default:
			return -100;
	}	

	return 0;

}

//////////////////////////////////////////////////////////////////////
///	<Field>
///		<name>name</name>
///		<value>value</value>
/// </Field>
/// pDom�� �±׸��� �ݵ�� Field���� �Ѵ�.
/// Child Tag�� name, value�� �ִٰ� �����Ѵ�. (�������� skip)
/// name, value�� ���ϵ尡 ���ٰ� �����Ѵ�.
/// ���� name, value Tag�� text�� �������ش�.
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::GetFieldInfo(IXMLDOMNodePtr pDom, CString &strFieldName, CString &strFieldValue)
{
	//++2008.08.22 ADD BY CJY {{++
	if(NULL == pDom) return -1;
	//--2008.08.22 ADD BY CJY --}}

	CComBSTR	nameString;
	CComBSTR	textString;

	pDom->get_baseName(&nameString);

	if( CString(nameString) == "field" )
	{
		IXMLDOMNodeListPtr pNodeList = pDom->childNodes;

		for( LONG i=0;i<pNodeList->length;i++ )
		{
			IXMLDOMNodePtr pObjectNode = pNodeList->item[i];

			nameString.Empty();
			textString.Empty();

			pObjectNode->get_baseName(&nameString);
			pObjectNode->get_text(&textString);

			CString strName( nameString );
			CString strText( textString );

			if( "name" == strName )
			{
				strFieldName = strText;
			}
			else if( "value" == strName )
			{
				strFieldValue = strText;
			}
		}

		return 1;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
/// �±׿� �ش��ϴ� ��ġ�� ��ȯ�Ѵ�.
//////////////////////////////////////////////////////////////////////
IXMLDOMNodePtr CEPCTradeAPI::GetNodePtr(IXMLDOMNodePtr pDom, CString strTagName)
{
	CComBSTR	nameString;

	pDom->get_baseName(&nameString);

	if( CString(nameString) == strTagName )
	{		
		return pDom;
	}
	else
	{
		IXMLDOMNodeListPtr pNodeList = pDom->childNodes;
		
		for( LONG i=0;i<pNodeList->length;i++ )
		{
			IXMLDOMNodePtr pObjectNode = pNodeList->item[i];
		
			IXMLDOMNodePtr pResultNode = GetNodePtr(pObjectNode, strTagName);
			if( pResultNode )
			{				
				return pResultNode;
			}
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
/// <Fields>
////	<Field>
////	<Field>
////	......
/// </Fields>
/// �ʵ�� �ش��ϴ� ��ġ�� ��ȯ�Ѵ�.
//////////////////////////////////////////////////////////////////////
IXMLDOMNodePtr CEPCTradeAPI::GetFieldPtr(IXMLDOMNodePtr pDom, CString strFieldName)
{
	CComBSTR	nameString;
	CComBSTR	textString;
		
	/// XML root �±׸��� Fields���� �Ѵ�.
	pDom->get_baseName(&nameString);

	if( CString(nameString) == "fields" )
	{
		/// Fields �� Field�� ���ϵ�� ������.
		IXMLDOMNodeListPtr pNodeList = pDom->childNodes;

		for( LONG i=0;i<pNodeList->length;i++ )
		{
			IXMLDOMNodePtr pObjectNode = pNodeList->item[i];

			/// Field�� ������ ������ ã������ name�� ���Ѵ�.
			CString strName;
			CString strValue;

			if( GetFieldInfo( pObjectNode, strName, strValue ) )
			{
				if( strFieldName == strName )
				{
					return pObjectNode;
				}
			}
		}
	}

	return NULL;
}

INT CEPCTradeAPI::SetFieldValue(const CString& pszField, const CString& szValue, CString &strXML)
{
	// 2009.02.27 ADD BY CJY : Ư������ ó�� �߰�
	CString strSrc, strDst;
	strSrc.Format(_T("<value>%s</value>"), pszField);
	strDst.Format(_T("%s"), szValue);
	strDst.Replace(_T("\""), _T("&quot;"));
	strDst.Replace(_T("'"), _T("&apos;"));
	strDst.Replace(_T("<"), _T("&lt;"));
	strDst.Replace(_T(">"), _T("&gt;"));
	strDst.Replace(_T("&"), _T("&amp;"));
	strDst = _T("<value>") + strDst + _T("</value>");

	strXML.Replace(strSrc, strDst);

	return 0;	
}

INT CEPCTradeAPI::SetFieldValue(TCHAR* pszField, TCHAR* szValue, CString &strXML)
{
// 	INT ids=-1;
// 	TCHAR szSrcValue[5000];
// 	TCHAR szDestValue[5000];
// 	CString strConVal ;
// 	
// 	//CURLEncode url_encode;
// 	//strConVal = url_encode.URLEncode(CString(szValue));
// 
// 	// URL���ڵ��� �ƴ� CDATA�� ���μ� ������.
// 	_stprintf(szSrcValue, _T("<value>%s</value>"), pszField);
// 
// 	//++2008.09.25 UPDATE BY CJY {{++
// 	// �ٲﳻ������
// 	_stprintf(szDestValue, _T("<value>%s</value>"), szValue);
// 	// �����ҽ�
// 	//DEL _stprintf(szDestValue, _T("<value><![CDATA[%s]]></value>"), szValue);
// 	//--2008.09.25 UPDATE BY CJY --}}	
// 
// 	strXML.Replace(szSrcValue, szDestValue);	

	// 2009.02.27 ADD BY CJY : Ư������ ó�� �߰�
	CString strSrc, strDst;
	strSrc.Format(_T("<value>%s</value>"), pszField);
	strDst.Format(_T("%s"), szValue);
	strDst.Replace(_T("\""), _T("&quot;"));
	strDst.Replace(_T("'"), _T("&apos;"));
	strDst.Replace(_T("<"), _T("&lt;"));
	strDst.Replace(_T(">"), _T("&gt;"));
	strDst.Replace(_T("&"), _T("&amp;"));
	strDst = _T("<value>") + strDst + _T("</value>");

	strXML.Replace(strSrc, strDst);

	return 0;	
}

//////////////////////////////////////////////////////////////////////
/// <field>
///		<name>
///		<value>
/// </field>
/// value �±��� text�� �����Ѵ�.
/// pDom�� field �±׸� �����ϴ�.
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::SetFieldValue(IXMLDOMNodePtr pFieldDom, CString strFieldValue)
{
	CComBSTR	nameString;
	
	INT nNeedByte=0;
	
	TCHAR szWValue[50000];
	CHAR* szDTmp=NULL;
	USHORT* szValue=NULL;
	
	pFieldDom->get_baseName(&nameString);

	if( CString(nameString) == "field" )
	{
		IXMLDOMNodeListPtr pNodeList = pFieldDom->childNodes;

		for( LONG i=0;i<pNodeList->length;i++ )
		{
			IXMLDOMNodePtr pObjectNode = pNodeList->item[i];

			nameString.Empty();			

			pObjectNode->get_baseName(&nameString);
			
			CString strName( nameString );			

			if( _T("value") == strName )
			{
				strFieldValue.Replace( _T("<"), _T("") );
				strFieldValue.Replace( _T(">"), _T("") );

				_tcscpy(szWValue, strFieldValue.GetBuffer(0));

				//UniCodeToUtf8String(szWValue, &szDTmp, &nNeedByte);				
				cvt.UnicodeToUTF8(szWValue, &szDTmp, &nNeedByte);				
				pObjectNode->put_text( szWValue );				
				return 1;
			}
		}
	}

	return 0;
}

CString CEPCTradeAPI::Encoding2(LPCTSTR str)
{
	USES_CONVERSION;

    // str�� Unicode���� Ansi ���� ���� �ʿ���� T2CW�� ��ȯ  
    const WCHAR* wStr = T2CW(str);    

    // ���̴� -1�� �־� ��NULL ���ڵ� ��ȯ�ǵ���
    //WCHAR -> UTF-8

    int nUTF8codeSize = WideCharToMultiByte(CP_UTF8, 0, wStr, -1, NULL, 0, NULL, NULL); //wStr�� ũ�⸦ ����
    char *utf8Str = new char[nUTF8codeSize];        

    WideCharToMultiByte(CP_UTF8, 0, wStr, -1, utf8Str, nUTF8codeSize, 0, 0);    

	CString strResult;
	strResult.Format( _T("%s"),  utf8Str );
	
	// KOL.UDF.022 ��ť���ڵ� ����
	delete[] utf8Str;
	//delete utf8Str;

	return strResult;
}

CHAR* CEPCTradeAPI::UnicodeEncoding(LPCTSTR str)
{
	USES_CONVERSION;

    // str�� Unicode���� Ansi ���� ���� �ʿ���� T2CW�� ��ȯ    
    const WCHAR* wStr = T2CW(str);    

    // ���̴� -1�� �־� ��NULL ���ڵ� ��ȯ�ǵ���
    //WCHAR -> UTF-8

    int nUTF8codeSize = WideCharToMultiByte(CP_UTF8, 0, wStr, -1, NULL, 0, NULL, NULL); //wStr�� ũ�⸦ ����
    char *utf8Str = new char[nUTF8codeSize];        

    WideCharToMultiByte(CP_UTF8, 0, wStr, -1, utf8Str, nUTF8codeSize, 0, 0);    
    return utf8Str;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  EPC�� �ο��޴´�.
// [Mini Spec]
//  1. ISBN ����üũ
//  2. EAN - 13
//  3. SGTIN - 96
//  4. Get Default XML
//  5. Make Param XML
//  6. result xml analyze
//  7. Make result
//////////////////////////////////////////////////////////////////////
INT CEPCTradeAPI::GetEPCToServer(CString strISBN, INT nCount, CStringArray &arrEPC)
{
	INT ids;
	CString strSGTIN96, strXML, strValue, strCount, strResult;

	// 2009.03.31 ADD BY CJY : �������� �߰�(�������� ��Ʈ�� �ٸ���.)
	SetConServer(ISSUE_ID);

	CString strLogMsg;
	strLogMsg.Format(_T("\r\n/------EPCToServer : %s------/"), strISBN);
	MakeLog(strLogMsg);

	// 1. ISBN ���� üũ
	if( !( 10 == strISBN.GetLength() || 13 == strISBN.GetLength() ) )
	{		
		/// ISBN ����
		return -1;
	}	
	
	// 2. EAN-13 ��ȯ
	if( 10 == strISBN.GetLength() )
	{
		/// ISBN ����
		if( 0 > ConvertISBN13( strISBN, "978", strISBN ) )
		{
			return -2;
		}
	}

	// 3. SGTIN-96 ��ȯ
	if( 0 > ConvertSGTIN96( strISBN, strSGTIN96 ) )
	{
		/// SGTIN-96 ��ȯ ����
		return -3;
	}

	// 4. Get Default XML	
	ids = LoadXMLQuery(ISSUE_ID, strXML);
	if (ids)
	{
		return -4;
	}

	/// count
	strCount.Format(_T("%d"), nCount);

	/*
	IXMLDOMDocumentPtr pDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);	
	
	if( 0 > MakeDefaultXML( ISSUE_ID, pDom ) )
	{
		/// XML ���� ����
		return -3;
	}
	
	/// fields�� ��ġ�� �����´�.
	IXMLDOMNodePtr pFields = GetNodePtr( pDom, "fields" );
	IXMLDOMNodePtr pIdClassNode = GetFieldPtr( pFields, "idClass" );
	SetFieldValue( pIdClassNode, strSGTIN96 );
	IXMLDOMNodePtr pFilterNode = GetFieldPtr( pFields, "filter" );
	SetFieldValue( pFilterNode, _T("0") );	
	IXMLDOMNodePtr pQuantityNode = GetFieldPtr( pFields, "quantity" );			
	SetFieldValue( pQuantityNode, strCount );	*/

	SetFieldValue(_T("idClass"), strSGTIN96.GetBuffer(0), strXML);
	SetFieldValue(_T("filter"), _T("0"), strXML);
	SetFieldValue(_T("quantity"), strCount.GetBuffer(0), strXML);
	
	//++2008.08.22 ADD BY CJY {{++
	{	
		if(_T("") == m_strLiCode)
		{
			GetLibCode(m_pInfomation, m_strLiCode);
		}		
		SetFieldValue(_T("LIBRARY_ID"),m_strLiCode.GetBuffer(0), strXML);
		
	}	
	//--2008.08.22 ADD BY CJY --}}

	//pDom->save( "C:\\r.xml" );	
	/*FILE* fp;
	fp = FdkFopen(_T("C:\\IssueID.txt"), _T("w"));
	FdkFprintf(fp, _T("%s"), strXML.GetBuffer(0));
	FdkFclose(fp);	*/
	
// ����
//DEL 	IXMLDOMDocumentPtr pQueryDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
//DEL 	if( !pQueryDom->loadXML( strXML.GetBuffer(0) ) )
//DEL 	{
//DEL 		return -4;	
//DEL 	}
//DEL 	pQueryDom->save( "C:\\query.xml" );
	
	MakeLog(strXML);

	/// trans server
	ids = GetDataToServer( strXML, strResult );

	MakeLog(strResult);

	if( 0 > ids)
	{
		return -5;
	}

	

	// 6. result xml analyze
	IXMLDOMDocumentPtr pResultDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		
	if( !pResultDom->loadXML( strResult.GetBuffer(0) ) )
	{
		return -6;	
	}

	CString strSerialFrom;
	CString strSerialCnt;
	CString strMessage;

	/// ��������� fields�� �������� �����Ѵ�
	/// �׷����� ��� ���۰� ���� �����ϱ⶧���� 
	/// ù��° ��������� �����ϵ��� �������.

	/// fields�� ��ġ�� �����´�.
	IXMLDOMNodePtr pResultFields = GetNodePtr( pResultDom, "fields" );
	IXMLDOMNodePtr pResultFromNode = GetFieldPtr( pResultFields, "serialFrom" );
	IXMLDOMNodePtr pResultQuanNode = GetFieldPtr( pResultFields, "quantity" );
	IXMLDOMNodePtr pResultMessageNode = GetFieldPtr( pResultFields, "Message" );
	CString strName;

	GetFieldInfo( pResultFromNode, strName, strSerialFrom );
	GetFieldInfo( pResultQuanNode, strName, strSerialCnt );
	GetFieldInfo( pResultMessageNode, strName, strMessage );

	if( _ttoi(strSerialCnt) != nCount )
	{
		if(_T("") != strMessage)
		{
			arrEPC.Add(strMessage);
		}
		return -7;
	}
	
	// 7. Make result
	CString strEPC;

	for( INT i=0;i<_ttoi(strSerialCnt);i++ )
	{
		strEPC.Format( _T("%s.%d"), strSGTIN96, _ttoi(strSerialFrom)+i );

		arrEPC.Add( strEPC );
	}

	// ��ϱ��� �۾��� ���� ���ϰ� ����
	CString strNew;
	IXMLDOMNodePtr pNewNode = GetFieldPtr( pResultFields, "isNew" );

	GetFieldInfo( pNewNode, strName, strNew );

	strNew.MakeUpper();

	//// test
	if( strNew == "TRUE" )
	{
		return 1001;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////
/// �԰� ������ �����Ѵ�.
INT CEPCTradeAPI::SendCheckInEPC(CString strLibCode, CString strEPC)
{
	//++2008.08.22 ADD BY CJY {{++
	//// ��� ��ȯ
	INT nBeforeMode = m_nMode;
	m_nMode = BCS1;
	//--2008.08.22 ADD BY CJY --}}

	// 2009.03.31 ADD BY CJY : �������� �߰�(�������� ��Ʈ�� �ٸ���.)
	SetConServer(EPC_CHECK_IN);

	CString strLogMsg;
	strLogMsg.Format(_T("\r\n/------SendCheckInEPC : %s------/"), strEPC);
	MakeLog(strLogMsg);

	INT ids;
	CString strXML, strTime, strResult;	

	if(0 == strLibCode.GetLength()) return -1;
	if(0 == strEPC.GetLength()) return -2;

//DEL 	if( !strLibCode.GetLength() || ! strEPC.GetLength() )
//DEL 	{
//DEL 		/// �Է°� ����
//DEL 		return -1;
//DEL 	}

	// 1. Get Default XML
	ids = LoadXMLQuery(EPC_CHECK_IN, strXML);
	if (ids) return -4;

	/// time
	CTime t = CTime::GetCurrentTime();
	strTime.Format(_T("%d-%02d-%02dT%02d:%02d:%02d.0Z"),t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());

	/*
	IXMLDOMDocumentPtr pDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);	

	if( 0 > MakeDefaultXML( EPC_CHECK_IN, pDom ) )
	{
		/// XML ���� ����
		return -3;
	}	

	/// fields�� ��ġ�� �����´�.
	IXMLDOMNodePtr pFields = GetNodePtr( pDom, "fields" );

	IXMLDOMNodePtr pTagNode = GetFieldPtr( pFields, "tag" );
	SetFieldValue( pTagNode, strEPC );
	IXMLDOMNodePtr pLibCodeNode = GetFieldPtr( pFields, "LIBRARY_ID" );
	SetFieldValue( pLibCodeNode, strLibCode );	
	IXMLDOMNodePtr pTimeNode = GetFieldPtr( pFields, "readTime" );
	SetFieldValue( pTimeNode, strTime );	*/

	SetFieldValue(_T("tag"), strEPC.GetBuffer(0), strXML);
	SetFieldValue(_T("LIBRARY_ID"), strLibCode.GetBuffer(0), strXML);	
	SetFieldValue(_T("readTime"), strTime.GetBuffer(0), strXML);

	//++2008.08.22 ADD BY CJY {{++
	SetFieldValue(_T("READING_BOOK"), _T("N"), strXML);
	//--2008.08.22 ADD BY CJY --}}

	/*FILE* fp;
	fp = FdkFopen(_T("C:\\CheckIn.txt"), _T("w"));
	if ( fp == NULL ) return -3;
	FdkFprintf(fp, _T("%s"), strXML.GetBuffer(0));
	FdkFclose(fp);	*/

	/// trans server	
	if( 0 > GetDataToServer( strXML, strResult ) )
	{
		return -5;
	}

	// 2. result xml analyze
	IXMLDOMDocumentPtr pResultDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	if( !pResultDom->loadXML( strResult.GetBuffer(0) ) )
	{
		return -6;	
	}

	IXMLDOMNodePtr pResultFields = GetNodePtr( pResultDom, "fields" );
	IXMLDOMNodePtr pSQLNode = GetFieldPtr( pResultFields, "SQLCode" );
	IXMLDOMNodePtr pResultMessageNode = GetFieldPtr( pResultFields, "Message" );

	CString strFieldName,strFieldValue;

	if( !GetFieldInfo( pSQLNode, strFieldName, strFieldValue ) )
	{
		//����� ã���� ���� ��??
		return -7;
	}

	if( _ttoi( strFieldValue ) )
	{
		/// ���� �۾��� ������� �ʾҴ� ���� ~~~
		return -8;
	}

//	pResultDom->save(_T("d:\\CheckIn_Result.xml"));

//DEL ???
//DEL 	CoUninitialize(); 

	return 0;
}

////////////////////////////////////////////////////////////////////////
/// �԰� ������ �����Ѵ�.
INT CEPCTradeAPI::SendCheckInEPC(CString strLibCode, CString strEPC, CString &strMessage)
{
	//++2008.08.22 ADD BY CJY {{++
	//// ��� ��ȯ
	INT nBeforeMode = m_nMode;
	m_nMode = BCS1;
	//--2008.08.22 ADD BY CJY --}}

	// 2009.03.31 ADD BY CJY : �������� �߰�(�������� ��Ʈ�� �ٸ���.)
	SetConServer(EPC_CHECK_IN);

	CString strLogMsg;
	strLogMsg.Format(_T("\r\n/------SendCheckInEPC : %s------/"), strEPC);
	MakeLog(strLogMsg);

	INT ids;
	CString strXML, strTime, strResult;	

	if(0 == strLibCode.GetLength()) return -1;
	if(0 == strEPC.GetLength()) return -2;

	// 1. Get Default XML
	ids = LoadXMLQuery(EPC_CHECK_IN, strXML);
	if (ids) return -4;

	/// time
	CTime t = CTime::GetCurrentTime();
	strTime.Format(_T("%d-%02d-%02dT%02d:%02d:%02d.0Z"),t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());

	SetFieldValue(_T("tag"), strEPC.GetBuffer(0), strXML);
	SetFieldValue(_T("LIBRARY_ID"), strLibCode.GetBuffer(0), strXML);	
	SetFieldValue(_T("readTime"), strTime.GetBuffer(0), strXML);

	//++2008.08.22 ADD BY CJY {{++
	SetFieldValue(_T("READING_BOOK"), _T("N"), strXML);
	//--2008.08.22 ADD BY CJY --}}

	/// trans server	
	MakeLog(strXML);
	ids = GetDataToServer( strXML, strResult );
	MakeLog(strResult);
	if( 0 > ids)
	{
		return -5;
	}

	// 2. result xml analyze
	IXMLDOMDocumentPtr pResultDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	if( !pResultDom->loadXML( strResult.GetBuffer(0) ) )
	{
		return -6;	
	}

	IXMLDOMNodePtr pResultFields = GetNodePtr( pResultDom, "fields" );
	IXMLDOMNodePtr pSQLNode = GetFieldPtr( pResultFields, "SQLCode" );
	IXMLDOMNodePtr pResultMessageNode = GetFieldPtr( pResultFields, "Message" );

	CString strFieldName,strFieldValue;
	GetFieldInfo( pResultMessageNode, strFieldName, strMessage );

	if( !GetFieldInfo( pSQLNode, strFieldName, strFieldValue ) )
	{
		

		//����� ã���� ���� ��??
		return -7;
	}

	if( _ttoi( strFieldValue ) )
	{
		/// ���� �۾��� ������� �ʾҴ� ���� ~~~
		return -8;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////
/// ��� ����
/// ������ ����
///     MAT_CODE = BO
///		ISBN
///     EAN13
///     TITLE
///     ���ǻ��
///     �����
///     ����
///     ����
///     

//INT CEPCTradeAPI::SendNewISBN(CString strISBN, CString strBookKey)
INT CEPCTradeAPI::SendNewISBN(CString strISBN, CString strTitle, CString strPublisher, CString strPubYear, CString strAuthor, TCHAR* szLibCode, CString& strMessage)
{
	INT		ids;
	INT nNeedByte=0;
	INT		nBeforeMode = m_nMode;
	CHAR*   szdTmp=NULL;
	CString strXML, strLibCode, strTime, strResult, strConVal;
	
	// 2009.03.31 ADD BY CJY : �������� �߰�(�������� ��Ʈ�� �ٸ���.)
	SetConServer(MAKE_NEW_INFO);

	CString strLogMsg;
	strLogMsg.Format(_T("\r\n/------SendNewISBN : %s------/"), strISBN);
	MakeLog(strLogMsg);

	/// XML ����
	ids = LoadXMLQuery(MAKE_NEW_INFO, strXML);
	if (ids) return -4;

	strLibCode.Format(_T("%s"), szLibCode);
	CTime t = CTime::GetCurrentTime();
	strTime.Format(_T("%d-%02d-%02dT%02d:%02d:%02d.0Z"),t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());

	/*
	IXMLDOMDocumentPtr pDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);	

	if( 0 > MakeDefaultXML( MAKE_NEW_INFO, pDom ) )
	{
		/// XML ���� ����
		return -1;
	}
	
	/// fields�� ��ġ�� �����´�.
	IXMLDOMNodePtr pFields = GetNodePtr( pDom, "fields" );
	IXMLDOMNodePtr pTitleNodePtr = GetFieldPtr( pFields, "TITLE" );
	SetFieldValue( pTitleNodePtr, strTitle );		
	IXMLDOMNodePtr pNameNodePtr = GetFieldPtr( pFields, "NAME" );
	SetFieldValue( pNameNodePtr, strTitle );
	IXMLDOMNodePtr pPublisherNodePtr = GetFieldPtr( pFields, "PUBLISHER" );
	SetFieldValue( pPublisherNodePtr, strPublisher );
	IXMLDOMNodePtr pManuNodePtr = GetFieldPtr( pFields, "MANUFACTUREID" );
	SetFieldValue( pManuNodePtr, strPublisher );
	IXMLDOMNodePtr pPubYearNodePtr = GetFieldPtr( pFields, "PUB_YEAR" );
	SetFieldValue( pPubYearNodePtr, strPubYear );
	IXMLDOMNodePtr pAuthorNodePtr = GetFieldPtr( pFields, "AUTHOR" );
	SetFieldValue( pAuthorNodePtr, strAuthor );
	IXMLDOMNodePtr pIsbnNodePtr = GetFieldPtr( pFields, "ISBN" );
	SetFieldValue( pIsbnNodePtr, strISBN );
	IXMLDOMNodePtr pEan13NodePtr = GetFieldPtr( pFields, "EAN_13_NO" );
	SetFieldValue( pEan13NodePtr, strISBN );
	IXMLDOMNodePtr pClassIDNodePtr = GetFieldPtr( pFields, "CLASSID" );
	SetFieldValue( pClassIDNodePtr, strISBN );
	ConvertSGTIN96( strISBN, strISBN );
	IXMLDOMNodePtr pSGTIN96NodePtr = GetFieldPtr( pFields, "EPCCLASS" );
	SetFieldValue( pSGTIN96NodePtr, strISBN );
	IXMLDOMNodePtr pLibNodePtr = GetFieldPtr( pFields, "LIBRARY_ID" );
	SetFieldValue( pLibNodePtr, strLibCode );
	IXMLDOMNodePtr pTimeNode = GetFieldPtr( pFields, "readTime" );
	SetFieldValue( pTimeNode, strTime );*/	

	SetFieldValue( _T("TITLE"), strTitle.GetBuffer(0), strXML );	
	SetFieldValue( _T("NAME"), strTitle.GetBuffer(0), strXML );	
	SetFieldValue( _T("PUBLISHER"), strPublisher.GetBuffer(0), strXML );		
	SetFieldValue( _T("MANUFACTUREID"), strPublisher.GetBuffer(0), strXML );	
	SetFieldValue( _T("PUB_YEAR"), strPubYear.GetBuffer(0), strXML );	
	SetFieldValue( _T("AUTHOR"), strAuthor.GetBuffer(0), strXML );
	
	//++2008.08.22 ADD BY CJY {{++
	// �ҽ��������� �ּ�ȭ�ϱ� ���� ����� �������� ����
	// �����ҽ����� classNoType�� Category�׸��� ���ڷ� ���� ���ϱ� ������
	// dm�� ���ؼ� ���´�.
	// ���� �ٸ� ������ �ʿ��Ҷ��� ����� dm���� ��Ű�� �����޴´�.
	// �� �Լ��� ȣ��Ǳ� ���� ȣ���� �ʿ��� m_DM�� ��Ű�� ������ �־�� �Ѵ�.
	CString strBookKey = m_DM.GetCellData(0, 0);
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(CONNECTION_INFO);
	CString strQuery = _T("");	
	strQuery.Format(
		_T("SELECT ")
//DEL 		_T("IB.TITLE, ")
//DEL 		_T("IB.PUBLISHER, ")
//DEL 		_T("IB.PUB_YEAR, ")
//DEL 		_T("IB.AUTHOR, ")
		_T("BB.CLASS_NO, ") // 0
		_T("BB.CLASS_NO_TYPE ")	// 1	
		_T(" FROM BO_BOOK_TBL BB,IDX_BO_TBL IB ")
		_T(" WHERE BB.SPECIES_KEY=IB.REC_KEY AND BB.REC_KEY=%s"), strBookKey);
	ids = DM.RestructDataManager( strQuery );
	
	enum COLUMN_INDEX
	{
		CLASS_NO = 0,
		CLASS_NO_TYPE = 1
	};
	CString strCateGory = DM.GetCellData(0, CLASS_NO);
	CString strClassNoType = DM.GetCellData(0, CLASS_NO_TYPE);
	/// 3�ڸ��� �߶� ������
	if( _T("1") == strClassNoType )
	{
		if( !strCateGory.IsEmpty() )
		{
			if( -1 < strCateGory.Find(_T(".")) )
			{
				strCateGory = strCateGory.Mid(0, strCateGory.Find(_T(".")));
			}
			if( 3 > strCateGory.GetLength() )
			{
				strCateGory = _T("000");
			}
			else
			{
				strCateGory = strCateGory.Mid(0,1) + _T("00");
			}
		}
	}
	else
	{
		strCateGory = _T("");
	}	
	SetFieldValue(_T("CATEGORY"), strCateGory.GetBuffer(0), strXML);
	
	//--2008.08.22 ADD BY CJY --}}

	SetFieldValue( _T("ISBN"), strISBN.GetBuffer(0), strXML );	
	SetFieldValue( _T("EAN_13_NO"), strISBN.GetBuffer(0), strXML );	
	SetFieldValue( _T("CLASSID"), strISBN.GetBuffer(0), strXML );
	if ( _tcslen(strISBN) == 10 )
	{
		ConvertISBN13( strISBN, "978", strISBN );
	}
	ConvertSGTIN96( strISBN, strISBN );
	SetFieldValue( _T("EPCCLASS"), strISBN.GetBuffer(0), strXML );	
	SetFieldValue( _T("LIBRARY_ID"), strLibCode.GetBuffer(0), strXML );	
	SetFieldValue( _T("readTime"), strTime.GetBuffer(0), strXML );	
		
	//pDom->save( "c:\\newlist.xml");	
	/*FILE* fp;
	fp = FdkFopen(_T("C:\\SendNewISBN.txt"), _T("w"));
	FdkFprintf(fp, _T("%s"), strXML.GetBuffer(0));
	FdkFclose(fp);	*/
	
	MakeLog(strXML);	
	
	/// trans server
	ids = GetDataToServer( strXML, strResult );

	MakeLog(strResult);

	if( 0 > ids )
	{
		return -5;
	}

	

	// 2. result xml analyze
	IXMLDOMDocumentPtr pResultDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	if( !pResultDom->loadXML( strResult.GetBuffer(0) ) )
	{
		return -6;	
	}

	IXMLDOMNodePtr pResultNode = GetNodePtr( pResultDom, "fields" );
	IXMLDOMNodePtr pSQLNode = GetFieldPtr( pResultNode, "SQLCode" );
	IXMLDOMNodePtr pResultMessageNode = GetFieldPtr( pResultNode, "Message" );


	CString strFieldName,strFieldValue;
	GetFieldInfo( pResultMessageNode, strFieldName, strMessage );

	if( !GetFieldInfo( pSQLNode, strFieldName, strFieldValue ) )
	{		
		//����� ã���� ���� ��??
		return -7;
	}

	if( _ttoi( strFieldValue ) )
	{
		/// ���� �۾��� ������� �ʾҴ� ���� ~~~
		return -8;
	}

	return 0;
}

INT CEPCTradeAPI::SendNewISBN(STRU_EPC_SEND_NEW_ISBN struNewISBN, CString& strMessage)
{
	INT		ids;
	INT nNeedByte=0;
	CString strXML, strTime, strConVal;
	
	// 2009.03.31 ADD BY CJY : �������� �߰�(�������� ��Ʈ�� �ٸ���.)
	SetConServer(MAKE_NEW_INFO);

	CString strLogMsg;
	strLogMsg.Format(_T("\r\n/------SendNewISBN : %s------/"), struNewISBN.strISBN);
	MakeLog(strLogMsg);

	/// XML ����
	ids = LoadXMLQuery(MAKE_NEW_INFO, strXML);
	if (ids) return -4;

	CTime t = CTime::GetCurrentTime();
	strTime.Format(
		_T("%d-%02d-%02dT%02d:%02d:%02d.0Z"), 
		t.GetYear(), t.GetMonth(), t.GetDay(), 
		t.GetHour(), t.GetMinute(), t.GetSecond()
		);

	CString strCateGory = struNewISBN.strClassNo;
	CString strClassNoType = struNewISBN.strClassNoType;

	/// 3�ڸ��� �߶� ������
	if( _T("1") == strClassNoType )
	{
		if( !strCateGory.IsEmpty() )
		{
			if( -1 < strCateGory.Find(_T(".")) )
			{
				strCateGory = strCateGory.Mid(0, strCateGory.Find(_T(".")));
			}
			if( 3 > strCateGory.GetLength() )
			{
				strCateGory = _T("000");
			}
			else
			{
				strCateGory = strCateGory.Mid(0,1) + _T("00");
			}
		}
	}
	else
	{
		strCateGory = _T("");
	}	
	SetFieldValue(_T("CATEGORY"), strCateGory, strXML);
		

	SetFieldValue( _T("TITLE"),			struNewISBN.strTitle,		strXML );	
	SetFieldValue( _T("NAME"),			struNewISBN.strTitle,		strXML );	
	SetFieldValue( _T("PUBLISHER"),		struNewISBN.strPublisher,	strXML );		
	SetFieldValue( _T("MANUFACTUREID"),	struNewISBN.strPublisher,	strXML );	
	SetFieldValue( _T("PUB_YEAR"),		struNewISBN.strPubYear,		strXML );	
	SetFieldValue( _T("AUTHOR"),		struNewISBN.strAuthor,		strXML );
	SetFieldValue( _T("LIBRARY_ID"),	struNewISBN.strLibCode,		strXML );	

	CString strISBN = struNewISBN.strISBN;
	SetFieldValue( _T("ISBN"),			strISBN,					strXML );	
	SetFieldValue( _T("EAN_13_NO"),		strISBN,					strXML );	
	SetFieldValue( _T("CLASSID"),		strISBN,					strXML );

	if ( _tcslen(strISBN) == 10 )
	{
		ConvertISBN13( strISBN, "978", strISBN );
	}
	ConvertSGTIN96( strISBN, strISBN );

	SetFieldValue( _T("EPCCLASS"),		strISBN,					strXML );		
	SetFieldValue( _T("readTime"),		strTime,					strXML );	
		
	

	MakeLog(strXML);	
	
	/// trans server
	CString strResult;
	ids = GetDataToServer( strXML, strResult );

	MakeLog(strResult);
	
	if( 0 > ids)
	{
		return -5;
	}

	// 2. result xml analyze
	IXMLDOMDocumentPtr pResultDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	if( !pResultDom->loadXML( strResult.GetBuffer(0) ) )
	{
		return -6;	
	}

	IXMLDOMNodePtr pResultNode = GetNodePtr( pResultDom, "fields" );
	IXMLDOMNodePtr pSQLNode = GetFieldPtr( pResultNode, "SQLCode" );
	IXMLDOMNodePtr pResultMessageNode = GetFieldPtr( pResultNode, "Message" );

	CString strFieldName,strFieldValue;
	GetFieldInfo( pResultMessageNode, strFieldName, strMessage );

	if( !GetFieldInfo( pSQLNode, strFieldName, strFieldValue ) )
	{			

		//����� ã���� ���� ��??
		return -7;
	}

	if( _ttoi( strFieldValue ) )
	{
		/// ���� �۾��� ������� �ʾҴ� ���� ~~~
		return -8;
	}

	return 0;
}

INT CEPCTradeAPI::DeleteMatInfoFromServer(TCHAR* pszOldEPC, TCHAR* pszLibCode)
{
	INT		ids;
	INT nNeedByte=0;
	INT		nBeforeMode = m_nMode;
	CHAR*   szdTmp=NULL;
	CString strXML, strResult,strTime;
	
	/// XML ����
	ids = LoadXMLQuery(DEL_BOOK_INFO, strXML);
	if (ids) return -3;

	CTime t = CTime::GetCurrentTime();
	strTime.Format(_T("%d-%02d-%02dT%02d:%02d:%02d.0Z"),t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());

	/*
	IXMLDOMDocumentPtr pDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);	

	if( 0 > MakeDefaultXML( DEL_BOOK_INFO, pDom ) )
	{
		/// XML ���� ����
		return -5;
	}
	
	/// fields�� ��ġ�� �����´�.
	IXMLDOMNodePtr pFields = GetNodePtr( pDom, "fields" );
	IXMLDOMNodePtr ptagNodePtr = GetFieldPtr( pFields, "tag" );
	SetFieldValue( ptagNodePtr, szdTmp );
	IXMLDOMNodePtr pLibNodePtr = GetFieldPtr( pFields, "LIBRARY_ID" );
	SetFieldValue( pLibNodePtr, szdTmp );
	IXMLDOMNodePtr pTimeNode = GetFieldPtr( pFields, "readTime" );
	SetFieldValue( pTimeNode, szdTmp );*/

	SetFieldValue(_T("tag"), pszOldEPC, strXML);
	SetFieldValue(_T("LIBRARY_ID"), pszLibCode, strXML);
	SetFieldValue(_T("readTime"), strTime.GetBuffer(0), strXML);
	
	/*FILE* fp;
	fp = FdkFopen(_T("C:\\DeleteMatInfoFromServer.txt"), _T("w"));
	FdkFprintf(fp, _T("%s"), strXML.GetBuffer(0));
	FdkFclose(fp);	*/

	// ������.	
	if( 0 > GetDataToServer( strXML, strResult ) )
	{
		return -40;
	}

	// 2. result xml analyze
	IXMLDOMDocumentPtr pResultDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	if( !pResultDom->loadXML( strResult.GetBuffer(0) ) )
	{
		return -45;	
	}

	IXMLDOMNodePtr pResultNode = GetNodePtr( pResultDom, "fields" );
	IXMLDOMNodePtr pSQLNode = GetFieldPtr( pResultNode, "SQLCode" );

	CString strFieldName,strFieldValue;

	if( !GetFieldInfo( pSQLNode, strFieldName, strFieldValue ) )
	{
		//����� ã���� ���� ��??
		return -50;
	}

	if( _ttoi( strFieldValue ) )
	{
		/// ���� �۾��� ������� �ʾҴ� ���� ~~~
		return -500;
	}
	
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////
/// �����̷��� �����Ѵ�.
INT CEPCTradeAPI::SendLoanHistory(CString strEPC, CString strLibCode, CString strLoanDate)
{
	INT ids;
	CString strXML, strResult, strTime ;

	/// XML ����
	ids = LoadXMLQuery(LOAN_HISTORY, strXML);
	if (ids) return -3;

	CTime t = CTime::GetCurrentTime();
	strTime.Format(_T("%d-%02d-%02dT%02d:%02d:%02d.0Z"),t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());

	/*
	IXMLDOMDocumentPtr pDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);

	if( 0 > MakeDefaultXML( LOAN_HISTORY, pDom ) )
	{
		/// XML ���� ����
		return -1;
	}
	
	/// fields�� ��ġ�� �����´�.
	IXMLDOMNodePtr pFields = GetNodePtr( pDom, "fields" );
	IXMLDOMNodePtr pTagNodePtr = GetFieldPtr( pFields, "tag" );
	SetFieldValue( pTagNodePtr, strEPC );
	IXMLDOMNodePtr pLibIDNodePtr = GetFieldPtr( pFields, "LIBRARY_ID" );
	SetFieldValue( pLibIDNodePtr, strLibCode );
	IXMLDOMNodePtr pLendingNodePtr = GetFieldPtr( pFields, "LENDING_DATE" );	
	SetFieldValue( pLendingNodePtr, strLoanDate );
	IXMLDOMNodePtr pTimeNode = GetFieldPtr( pFields, "readTime" );
	SetFieldValue( pTimeNode, strTime );*/

	SetFieldValue(_T("tag"), strEPC.GetBuffer(0), strXML);
	SetFieldValue(_T("LIBRARY_ID"), strLibCode.GetBuffer(0), strXML);
	SetFieldValue(_T("LENDING_DATE"), strLoanDate.GetBuffer(0), strXML);
	SetFieldValue(_T("readTime"), strTime.GetBuffer(0), strXML);	

//	pDom->save( "C:\\loanhistory.xml" );

	AfxMessageBox(strXML);

	/// trans server	
	if( 0 > GetDataToServer( strXML, strResult ) )	
	{
		return -4;
	}

	// 2. result xml analyze
	IXMLDOMDocumentPtr pResultDom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	AfxMessageBox(strResult);

	if( !pResultDom->loadXML( strResult.GetBuffer(0) ) )
	{
		return -4;	
	}

	IXMLDOMNodePtr pResultNode = GetNodePtr( pResultDom, "fields" );
	IXMLDOMNodePtr pSQLNode = GetFieldPtr( pResultNode, "SQLCode" );

	CString strFieldName,strFieldValue;

	if( !GetFieldInfo( pSQLNode, strFieldName, strFieldValue ) )
	{
		//����� ã���� ���� ��??
		return -4;
	}

	if( _ttoi( strFieldValue ) )
	{
		/// ���� �۾��� ������� �ʾҴ� ���� ~~~
		return -500;
	}

	return 0;
}


/*
bool CEPCTradeAPI::isUnsafe(char compareChar)
{
  bool bcharfound = false;
  char tmpsafeChar;
  int m_strLen = 0;

  m_strLen = csUnsafeString.GetLength();
  for(int ichar_pos = 0; ichar_pos < m_strLen ;ichar_pos++)
  {
    tmpsafeChar = csUnsafeString.GetAt(ichar_pos);
    if(tmpsafeChar == compareChar)
    {
      bcharfound = true;
      break;
    }
  }
  int char_ascii_value = 0;
  //char_ascii_value = __toascii(compareChar);
  char_ascii_value = (int) compareChar;

  if(bcharfound == false &&  char_ascii_value > 32 &&
                             char_ascii_value < 123)
  {
    return false;
  }
  // found no unsafe chars, return false
  else
  {
    return true;
  }

  return true;
}

CString CEPCTradeAPI::decToHex(char num, int radix)
{
  int temp=0;
  CString csTmp;
  int num_char;

num_char = (int) num;
  if (num_char < 0)
    num_char = 256 + num_char;

  while (num_char >= radix)
    {
    temp = num_char % radix;
    num_char = (int)floor(num_char / radix);
    csTmp = hexVals[temp];
    }

  csTmp += hexVals[num_char];

  if(csTmp.GetLength() < 2)
  {
    csTmp += '0';
  }

  CString strdecToHex(csTmp);
  // Reverse the String
  strdecToHex.MakeReverse();

  return strdecToHex;
}

CString CEPCTradeAPI::convert(char val)
{
  CString csRet;
  csRet += "%";
  csRet += decToHex(val, 16);
  return  csRet;
}*/


VOID CEPCTradeAPI::SetCONNECTION_INFO(CString strCONNECTION_INFO)
{	
	CONNECTION_INFO = strCONNECTION_INFO;

	m_DM.SetCONNECTION_INFO( CONNECTION_INFO );
}

INT CEPCTradeAPI::GetLibCode(CESL_Information* pInfomation, CString& strGetLibCode)
{	
	if ( !m_strLiCode.IsEmpty() )
	{
		strGetLibCode = m_strLiCode;
		return 0;
	}

	if ( !pInfomation )
	{
		return -1;
	}

	m_pInfomation = pInfomation;

	if ( m_strLiCode.IsEmpty() )
	{
		//++2008.10.15 UPDATE BY PWR {{++
		// ���սý��� ����
		CString	strWhere;
		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE") );
// 		CString	strWhere, strManageCode, strManageValue;		
//		
//		strWhere.Format(_T("SELECT MAX(VALUE_NAME) FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND ")
//						_T(" GROUP_2='��Ÿ' AND ")
//	                    _T(" GROUP_3='����' AND ")
//                        _T(" CLASS_ALIAS='�������к�����������'") );
//		m_DM.GetOneValueQuery( strWhere, strManageValue, FALSE );
//		// -----------------------------------------------------------------------		
//
//		strWhere.Format(_T("SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
//						_T("WHERE ID = '%s'"), pInfomation->USER_ID );
//		m_DM.GetOneValueQuery( strWhere, strManageCode, FALSE );
//
//		if ( strManageCode == _T("") || strManageValue == _T("N") )
//		{
//			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"));
//		}
//		else
//		{
//			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE ='%s'"), strManageCode);		
//		}
		//--2008.10.15 UPDATE BY PWR --}}

		m_DM.GetOneValueQuery( strWhere, m_strLiCode, FALSE );
	}
	
	strGetLibCode =m_strLiCode;
	return 0;
}

VOID CEPCTradeAPI::MakeLog(CString &query)
{
	if (TRUE == query.IsEmpty()) return;
	CTime t = CTime::GetCurrentTime();
	CString filename, msg;
	filename.Format(_T("..\\tunning\\EPCTradeAPI_%04d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	msg.Format(_T("%s"), query);
	
	//++2008.16 UPDATE BY CJY {{++
	// �ؽ�Ʈ ���Ŀ��� �������� �������� ����
	// _DEBUG ������ �����ô� ������Ĵ�� ����

	CFileFind filefind;
	BOOL bFind = filefind.FindFile(_T("..\\cfg\\_log"));

	// 2009.04.07 ADD BY CJY : �α� ���߰� �ɼ� �߰�
	BOOL bNoLogFind = filefind.FindFile(_T("..\\cfg\\_nolog"));

	if(FALSE == bFind && FALSE == bNoLogFind)
	{
		filename = filename + _T(".tunning");

		// ���Ͽ��� �÷��� ����
		// ������ ������ ���������� ����, ������ ���� �����.
		CFileFind filefind;
		UINT nOpenFlags = CFile::modeWrite|CFile::modeCreate|CFile::typeBinary;
		if(TRUE == filefind.FindFile(filename))
		{
			nOpenFlags ^= CFile::modeCreate;
		}
		
		CFile file;
		BOOL bIsOpen = file.Open(filename, nOpenFlags);
		if(TRUE != bIsOpen) return;
		
		file.SeekToEnd();
		
		// �ؽ�Ʈ�� ����Ѵ�.
		{
			const INT nLen = msg.GetLength();
			
			TCHAR *pChar = new TCHAR[nLen+1];
			memset(pChar, NULL, nLen);
			pChar[nLen] = '\0';
			_tcscpy(pChar, (TCHAR*)msg.GetBuffer(0));
			
			for(INT nCnt=0; nCnt < nLen; nCnt++)
			{
				pChar[nCnt] ^= 0xAA; // ������ 1010 1010
			}
			
			file.WriteHuge(pChar, sizeof(TCHAR)*nLen);
			delete []pChar;
		}
		
		// ���๮�ڸ� ����Ѵ�.
		{
			CString strLine = _T("\r\n");	
			const INT LEN = strLine.GetLength();
			TCHAR *szLine = new TCHAR[LEN+1];
			memset(szLine, NULL, LEN);
			szLine[LEN] = '\0';
			_tcscpy(szLine, (TCHAR*)strLine.GetBuffer(0));
			
			for(INT nCnt=0; nCnt < LEN; nCnt++)
			{
				szLine[nCnt] ^= 0xAA; // ������ 1010 1010
			}
			file.Write(szLine, sizeof(TCHAR)*LEN);
			delete []szLine;
		}
		
		file.Close();
	}
	else if(FALSE == bNoLogFind)
	{
		filename = filename + _T(".txt");

		// �����ҽ�	
		FILE* fp;
		
		fp = _tfopen (filename, _T("a+b"));
		if (fp == NULL)
		{
			return ;
		}
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE �����۾�
		// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			/*UNCHANGE*/fputc(0xFF, fp);
			/*UNCHANGE*/fputc(0xFE, fp);
		}
		// ------------------------------------------------------------------------------
		
		_ftprintf (fp, _T("%s\r\n"), msg);
		
		fclose(fp);
	}
//--2008.10.16 UPDATE BY CJY --}}

}