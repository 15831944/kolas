// MarcConvertApi.cpp: implementation of the CMarcConvertApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcConvertApi.h"

#include "stdio.h"
#include "tchar.h"

#include "locale.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcConvertApi::CMarcConvertApi()
{
	m_cpConvertData = NULL;
}

CMarcConvertApi::~CMarcConvertApi()
{
	if (m_cpConvertData != NULL) 
	{
		m_cpConvertData = NULL;
	}
	
}
/*
INT CMarcConvertApi::SetConvertMode(INT nConvertModeChk)
{
	m_nConvertModeChk = nConvertModeChk;
	return m_nConvertModeChk;
}
*/
/***************************************************
1. Marc����ü�� �����Ѵ�.
***************************************************/
INT CMarcConvertApi::GetMarcStruct(CString strMarc)
{
	CString strMarcTemp;
	INT		nFindLocate;
	INT		nGetLastIdx;
	INT		nCode;
	_TCHAR cCode;

	nCode   = 30;
	cCode = (_TCHAR)nCode;  
	strMarcTemp = strMarc;
	strMarcTemp.Replace(_T("\r"),_T(""));
	strMarcTemp.Replace(_T("\n"),_T(""));
	
	struMarc.strLead = strMarcTemp.Mid(0, 24);
	nFindLocate = strMarcTemp.Find(cCode);
	struMarc.strDirectory = strMarcTemp.Mid(24, nFindLocate-24);
	struMarc.nDirectoryCnt = struMarc.strDirectory.GetLength()/12;

	nGetLastIdx = strMarcTemp.GetLength();
	struMarc.strField =strMarcTemp.Mid( nFindLocate, nGetLastIdx - nFindLocate);

	SetDirectory();
	strMarc.Format( _T("%s%s%s"), struMarc.strLead, struMarc.strDirectory, struMarc.strField );
	m_strUniConvertMarc = strMarc;
	return 0;
}

/***************************************************
2. Directory�� ���� ��忡 �°� �����Ѵ�.
***************************************************/
INT CMarcConvertApi::SetDirectory(VOID)
{
	CString strLead;
	CString strDirectory;
	CString strDirectoryTemp;
	CString strField;
	
	INT     nFieldLen;
	
	INT     nDirectory1;
	INT     nDirectory2;
	INT     nDirectory3;
	
	INT     nFind1;

	INT     nNextFieldStart;
	
	CString Temp;
	
	CString strOneDirectory;
	CString strOneField;
	
	INT     nCode;
	_TCHAR   strCode;
	INT nGetOneDirectoryStartLocation;
	
	nCode					 = 30;
	strCode					 = (_TCHAR)nCode; 
	struMarc.nWholeLen		 = 0;
	nNextFieldStart = 0;
	nGetOneDirectoryStartLocation = 0;
	

	strLead.Format(_T("%s"),struMarc.strLead);
	strDirectory.Format(_T("%s"), struMarc.strDirectory);
	strField.Format(_T("%s"), struMarc.strField); 
	nFieldLen = strField.GetLength();

	for( INT i = 0 ; i < struMarc.nDirectoryCnt ; i++)
	{
		nFind1 = strField.Find( strCode , 1);
		strOneField.Format( _T("%s"), strField.Mid( 0, nFind1 ) );

		strOneDirectory.Format( _T("%s"), strDirectory.Mid( nGetOneDirectoryStartLocation*12, 12));
		nDirectory1   = _ttoi(strOneDirectory.Mid(0,3));

		if( m_nConvertModeChk == _UTF8_FILE)
		{
			nDirectory2   = GetLengthUnicodeToUTF8(strOneField) -1;
		}
		else if( m_nConvertModeChk == _ANSI_FILE)
		{
			nDirectory2   = GetLengthUnicodeToAnsi(strOneField)-1;
		}
		else
		{
			nDirectory2   = strOneField.GetLength();
		}
		nDirectory3   = nNextFieldStart;
		
		nNextFieldStart = nDirectory2 + nDirectory3;
		struMarc.nWholeLen +=  nDirectory2;

		if( strDirectoryTemp == _T(""))
		{
			strDirectoryTemp.Format( _T("%3d%4d%5d"), nDirectory1, nDirectory2,nDirectory3 ) ;
			strDirectoryTemp.Replace(_T(" "), _T("0"));
		}
		else
		{
			Temp.Format(_T("%s%3d%4d%5d"), strDirectoryTemp, nDirectory1, nDirectory2,nDirectory3 ) ; 
			Temp.Replace(_T(" "), _T("0"));
			strDirectoryTemp.Format( _T("%s"),Temp) ;
		}

		nFieldLen = nFieldLen - nFind1;
		Temp.Format( _T("%s"),strField.Mid( nFind1, nFieldLen ));
		strField.Format(_T("%s"), Temp );
		nGetOneDirectoryStartLocation++;
	}
	INT nLeadLen = 0;
	INT nDirectoryLen = 0;
	if( m_nConvertModeChk == _UTF8_FILE)
	{
		nLeadLen   = GetLengthUnicodeToUTF8(struMarc.strLead) -1;
		nDirectoryLen   = GetLengthUnicodeToUTF8(struMarc.strDirectory) -1;

	}
	else if( m_nConvertModeChk == _ANSI_FILE)
	{
		nLeadLen   = GetLengthUnicodeToAnsi(struMarc.strLead)-1;
		nDirectoryLen   = GetLengthUnicodeToAnsi(struMarc.strDirectory)-1;
	}
	else
	{
		nLeadLen   = strOneField.GetLength();
		nDirectoryLen   =struMarc.strDirectory.GetLength();
	}
	struMarc.nWholeLen += 2 + nLeadLen + nDirectoryLen;
	struMarc.strDirectory.Format(_T("%s"), strDirectoryTemp);
	Temp.Format(_T("%5d"), struMarc.nWholeLen);
	Temp.Replace(_T(" "),_T("0"));
	struMarc.strLead.Format(_T("%s%s"), Temp, strLead.Mid(5, 19));
	
	return 0;
}

/***************************************************
�����ͺ�ȯ
***************************************************/

/***************************************************
3. UNICODE -> ANSI�� �ʿ� ����Ʈ�� ���´�.
***************************************************/
INT CMarcConvertApi::GetLengthUnicodeToAnsi(CString strData )
{
	INT nNeedByte;

	nNeedByte = WideCharToMultiByte( CP_ACP	, 
								0				,
								strData.GetBuffer(0), 
								-1				,
								NULL			, 
								0				,
								NULL			, 
								NULL      );

	return nNeedByte;
}
/***************************************************
4. UNICODE -> UTF8�� �ʿ� ����Ʈ�� ���´�.
***************************************************/
INT CMarcConvertApi::GetLengthUnicodeToUTF8(CString strData )
{
	INT nNeedByte;

	nNeedByte = WideCharToMultiByte( CP_UTF8	, 
								0				,
								strData.GetBuffer(0), 
								-1				,
								NULL			, 
								0				,
								NULL			, 
								NULL      );

	return nNeedByte;
}
/***************************************************
5. ANSI-> UNICODE�� �ʿ� ����Ʈ�� ���´�.
***************************************************/
INT CMarcConvertApi::GetLengthAnsiToUniCode(CHAR* cpData )
{
	INT nNeedByte;
	nNeedByte = MultiByteToWideChar( CP_ACP	, 
								0				,
								cpData,
								-1				,
								NULL			, 
								NULL      );
								
	return nNeedByte;
}


/***************************************************
6. UTF8-> UNICODE�� �ʿ� ����Ʈ�� ���´�.
***************************************************/
INT CMarcConvertApi::GetLengthUtf8ToUniCode(CHAR* cpData )
{
	INT nNeedByte=0;
	nNeedByte = MultiByteToWideChar( CP_UTF8	, 
								0				,
								cpData,
								-1				,
								NULL			, 
								NULL      );
	return nNeedByte;
}

/***************************************************
7. UNICODE -> ANSI�� ��Ʈ���� ��ȯ�Ѵ�
***************************************************/
INT CMarcConvertApi::GetStrUniCodeToAnsi( CString strData )
{
	INT ids = 0;
	INT nNeedByte = 0;
	TCHAR *tcpData = NULL;
	if (m_cpConvertData != NULL) 
	{
		m_cpConvertData = NULL;
	}
	nNeedByte = strData.GetLength();
	//tcpData = (TCHAR *)calloc( nNeedByte+1, sizeof( TCHAR ) );
	tcpData = (TCHAR *)calloc( nNeedByte, sizeof( TCHAR ) );
	wcscpy(tcpData, strData);
	nNeedByte = GetLengthUnicodeToAnsi( strData );
	m_cpConvertData = (CHAR *)calloc( nNeedByte, sizeof( CHAR ) );
	m_CharConvert->UnicodeToMultiByte( tcpData, &m_cpConvertData, &nNeedByte);
	if (tcpData != NULL) 
	{
//		free(tcpData);
		tcpData = NULL;
	}

	return ids;
}

/***************************************************
8. UNICODE -> UTF8�� ��Ʈ���� ��ȯ�Ѵ�
***************************************************/
INT CMarcConvertApi::GetStrUniCodeToUtf8(CString strData )
{
	INT ids=0;
	INT nNeedByte=0;CString strDataTemp ;
	TCHAR *tcpData=NULL;
	if (m_cpConvertData != NULL) 
	{
		m_cpConvertData = NULL;
	}
	nNeedByte = strData.GetLength(); 
	tcpData = (TCHAR *)calloc( nNeedByte, sizeof( TCHAR ) );
	wcscpy(tcpData, strData.GetBuffer(0));
	nNeedByte = GetLengthUnicodeToUTF8(strData );
	//m_cpConvertData = (CHAR *)malloc( sizeof( CHAR )*nNeedByte+sizeof( CHAR ) );
	m_cpConvertData = (CHAR *)malloc( sizeof( CHAR )* ( nNeedByte+1) );
	m_CharConvert->UnicodeToUTF8(tcpData, &m_cpConvertData, &nNeedByte);
	if (tcpData != NULL) 
	{
		tcpData = NULL;
	}

	return ids;
}


/***************************************************
9. ANSI -> UNICODE  �� ��Ʈ���� ��ȯ�Ѵ�
***************************************************/
INT CMarcConvertApi::GetStrAnsiToUniCode(CHAR* cpData)
{
	INT ids=0;
	INT nNeedByte=0;
	TCHAR *tcpData=NULL;
	nNeedByte = GetLengthAnsiToUniCode( cpData );
	tcpData = (TCHAR *)malloc( sizeof( TCHAR )*nNeedByte+sizeof( TCHAR ) );
	m_CharConvert->MultiByteToUnicode(cpData, &tcpData, &nNeedByte);
	m_strConvertData.Format(tcpData);
	if (tcpData != NULL) 
	{
		free(tcpData);
	}
	
	return ids;
}

/***************************************************
10. UTF8-> UNICODE  �� ��Ʈ���� ��ȯ�Ѵ�
***************************************************/
INT CMarcConvertApi::GetStrUtf8ToUniCode(CHAR* cpData)
{
	INT ids=0;
	INT nNeedByte=0;
	TCHAR *tcpData=NULL;
	nNeedByte = GetLengthUtf8ToUniCode( cpData )+1;
	tcpData = (TCHAR *)calloc( nNeedByte, sizeof( TCHAR ) );
	m_CharConvert->UTF8ToUnicode(cpData, &tcpData, &nNeedByte);
	m_strConvertData.Format(tcpData);
	if (tcpData != NULL) 
	{
		tcpData = NULL;
	}
	return ids;
}
/***********************************************************
//wcscpy(tcpData, strData.GetBuffer(0));
	//nNeedByte = GetLengthUnicodeToUTF8(strData );
	//cpData = (CHAR *)calloc( nNeedByte, sizeof( CHAR ) );
	//strData.Format(tcpData);
	//wcscpy(strData, tcpData);
	//wcscpy(tcpData, strData.GetBuffer(0));
	//nNeedByte = GetLengthUnicodeToUTF8(strData );
	//cpData = (CHAR *)calloc( nNeedByte, sizeof( CHAR ) );

************************************************************/



INT CMarcConvertApi::TEST_TEMP()
{
	CString strDataTemp ;
	CString strDataTemp2 ;
	strDataTemp.Format(_T("������"));
	INT nNeedByte = 0;
	
	TCHAR *tcDataTemp=NULL;
	CHAR *cDataTemp=NULL;
	nNeedByte = strDataTemp.GetLength(); 
	tcDataTemp = (TCHAR *)calloc( nNeedByte, sizeof( TCHAR ) );
	wcscpy(tcDataTemp, strDataTemp.GetBuffer(0));
	nNeedByte = GetLengthUnicodeToAnsi(strDataTemp );
	cDataTemp = (CHAR *)calloc( nNeedByte, sizeof( CHAR ) );
	
	
	//MultiByteToWideChar(CP_ACP, 0, cDirectory,nFind1         , NULL, NULL);
	//MultiByteToWideChar(CP_ACP, 0, sTime     , lstrlen(sTime), NULL, NULL);
	//MultiByteToWideChar(CP_ACP, 0, szURL     ,-1             , NULL, 0   );
	/*
	CString strDataTemp ;
	CString strDataTemp2 ;
	strDataTemp.Format(_T("������"));
	INT nDataTemp = 0;
	TCHAR *tcDataTemp=NULL;
	CHAR *cDataTemp=NULL;
	nDataTemp = strData.GetLength(); 
	tcDataTemp = (TCHAR *)calloc( nDataTemp, sizeof( TCHAR ) );
	wcscpy(tcDataTemp, strData.GetBuffer(0));
	nNeedByte = GetLengthUnicodeToUTF8(strData );
	cDataTemp = (CHAR *)calloc( nNeedByte, sizeof( CHAR ) );
	*/
	/*
	LPCTSTR = (LPTSTR)(LPCTSTR)CString
CHAR*   = (LPSTR)(LPCTSTR)TCHAR*
	*/
	
	/*
	INT nNeedByte;
	CHAR* cDataTemp;
	TCHAR* tcDataTemp=NULL;
	
	CString temp;
	temp.Format(_T("1111��������1AA"));
	nNeedByte = GetLengthUnicodeToUTF8(temp);
	cDataTemp = (CHAR *)calloc( nNeedByte, sizeof( CHAR ) );

	strcpy(cDataTemp,(LPSTR)(LPTSTR)(LPCTSTR)temp );

	nNeedByte = MultiByteToWideChar( CP_UTF8	, 
								0				,
								cDataTemp,//.GetBuffer(0), 
								-1				,
								NULL			, 
								NULL      );
	*/
								
	

	return nNeedByte;
}
/***************************************************
TEST
***************************************************/
INT CMarcConvertApi::TEST_UNICODE_TO_MULTIBYTE()
{
	INT nNeedByte;
  
	/*
	LPCTSTR = (LPTSTR)(LPCTSTR)CString
CHAR*   = (LPSTR)(LPCTSTR)TCHAR*
	*/
	/***********************************************
	UNICODE -> UTF8
	************************************************/
//		_tsetlocale(LC_ALL, _T("kor")); 
	
	CString strDataTemp ;
	CString strDataTemp2 ;
	strDataTemp.Format(_T("����������111AAA"));
	
	INT nDataTemp = 0;
	TCHAR *getTemp=NULL;
	CHAR *getTemp1=NULL;
	//TCHAR �޸� �Ҵ� CSTRING ����
	nDataTemp = strDataTemp.GetLength(); 
	//getTemp = (TCHAR *)calloc( nDataTemp, sizeof( TCHAR ) );
	getTemp = (TCHAR*)malloc(sizeof(TCHAR)*(nDataTemp+1));
	wcscpy(getTemp, strDataTemp.GetBuffer(0));
	//UTF8�� ��ȯ
	nNeedByte = GetLengthUnicodeToUTF8( strDataTemp )+1;
	//getTemp1 = (CHAR *)calloc( nNeedByte, sizeof( CHAR ) );
	getTemp1 = (CHAR*)malloc(sizeof(CHAR)*(nNeedByte));
	//IsDBCSLeadByte(), IsDBCSLeadByteEx(), IsTextUnicode()
	//_tsetlocale(LC_ALL, _T("kor")); 
	//_wsetlocale(LC_ALL, _T("kor")); 
	//CharEncodeMgr.CharConvert ���
	m_CharConvert->UnicodeToUTF8( getTemp, &getTemp1, &nNeedByte);
	
	/***********************************************
	UNICODE -> UTF8
	************************************************/
	/***********************************************
	UNICODE -> ANSI
	************************************************/
	CString strDataTemp_ ;
	CString strDataTemp2_ ;
	strDataTemp_.Format(_T("����������111AAA"));

	INT nDataTemp_ = 0;
	TCHAR *getTemp_=NULL;
	CHAR *getTemp1_=NULL;
	nDataTemp_ = strDataTemp_.GetLength(); 
	//getTemp_ = (TCHAR *)calloc( nDataTemp_, sizeof( TCHAR ) );
	getTemp_ = (TCHAR*)malloc(sizeof(TCHAR)*nDataTemp_);
	wcscpy(getTemp_, strDataTemp_.GetBuffer(0));
	nNeedByte = GetLengthUnicodeToAnsi( strDataTemp_ )-1;
	//getTemp1_ = (CHAR *)calloc( nNeedByte, sizeof( CHAR ) );
	getTemp1_ = (CHAR*)malloc(sizeof(CHAR)*(nNeedByte+1));
	m_CharConvert->UnicodeToMultiByte( getTemp_, &getTemp1_, &nNeedByte);
	/***********************************************
	UNICODE -> ANSI
	************************************************/
	/***********************************************
	�޸� ����
	************************************************/
//	free(getTemp);
//	free(getTemp1);
//	free(getTemp_);
//	free(getTemp1_);
	/***********************************************
	�޸� ����
	************************************************/

	

//	nNeedByte = MultiByteToWideChar(CP_UTF8,0,strData.GetBuffer(0),-1,NULL,0);
	return nNeedByte;
}



INT CMarcConvertApi::TEST_MULTIBYTE_TO_UNICODE()
{
	INT nNeedByte;

	CString strDataTemp ;
	strDataTemp.Format(_T("������"));
	INT nDataTemp = 0;
	CHAR *getTemp=NULL;
	nDataTemp = strDataTemp.GetLength(); 
	getTemp = (CHAR*) malloc(sizeof(CHAR) * nDataTemp + sizeof(CHAR));
//	wcscpy(getTemp, (LPTSTR)(LPCTSTR)strDataTemp.GetBuffer(nDataTemp));
	nNeedByte = MultiByteToWideChar(CP_UTF8,0,getTemp,-1,NULL,0);


//	MultiByteToWideChar(CP_UTF8, 0, sTime, lstrlen(sTime), NULL, NULL);
	

	

//	nNeedByte = MultiByteToWideChar(CP_UTF8,0,strData.GetBuffer(0),-1,NULL,0);
	return nNeedByte;
}



////==========================�߰��Ǿ�����ڵ�
//==============================================================
/*
/*
ANSI ���ڿ����� �����ڵ���� ��ȯ ��� 
	// sTime�̶� ANSI ���ڿ��� bstr�̶� �̸��� �����ڵ�(BSTR Ÿ��) ������ ��ȯ
	char sTime[] = "�����ڵ� ��ȯ ����";
	BSTR bstr;
	// sTime�� �����ڵ�� ��ȯ�ϱ⿡ �ռ� ���� �� ���̸� �˾ƾ� �Ѵ�.
	int nLen = MultiByteToWideChar(CP_ACP, 0, sTime, lstrlen(sTime), NULL, NULL);
	// �� ���̸�ŭ �޸𸮸� �Ҵ��Ѵ�.
	bstr = SysAllocStringLen(NULL, nLen);
	// ���� ��ȯ�� �����Ѵ�.
	MultiByteToWideChar(CP_ACP, 0, sTime, lstrlen(sTime), bstr, nLen);
         // �ʿ�������� �����Ѵ�.
         SysFreeString(bstr);
*/
/*
LPCTSTR = (LPTSTR)(LPCTSTR)CString
CHAR*   = (LPSTR)(LPCTSTR)TCHAR*


-- �߰� ������ --
#ifdef _UNICODE
	#ifndef __WFILE__
	#define WIDEN2(x)   L ## x		 
	#define WIDEN(x)   WIDEN2(x) 
	#define __WFILE__ WIDEN(__FILE__)
	#endif
#else
	#ifndef __WFILE__
	#define __WFILE__ __FILE__
	#endif
#endif


atof    = _tcstod(x, TCHAR* s);


// wchar_t * �� ���ڿ��� char * ��ȯ�� �ʿ��� ���� ���
    int len; 
    len = WideCharToMultiByte(CP_ACP, 0, tmp, -1, NULL, 0, NULL,NULL);
    char* ctmp = new char[len]; 
    // ���� ��ȯ
    WideCharToMultiByte(CP_ACP, 0, tmp, -1, ctmp, len, NULL, NULL);
    printf("%s", ctmp);
    delete []ctmp;

// char * �� ���ڿ��� wchar_t * ��ȯ�� �ʿ��� ���� ���
    int len;
    len  =  MultiByteToWideChar(CP_ACP,0,szURL,-1,NULL,0);
    WCHAR *wszURL = new WCHAR[len];
    MultiByteToWideChar(CP_ACP,0,szURL,-1,wszURL,wlen);


_UNICODE,UNICODE
*/









//	int slen = MultiByteToWideChar(CP_ACP, 0, filename, strlen(filename), NULL, NULL);


	//MultiByteToWideChar(CP_ACP,0,cDirectory,nFind1,NULL,NULL);
	//MultiByteToWideChar(CP_ACP, 0, szAnsi, -1, NULL, NULL);
	/*
	CString strTemp = _T("test"); 
     char *getTemp=NULL; 

     getTemp = malloc(strTemp.GetLength()+1); 
     strcpy(getTemp, strTemp.GetBuffer(strTemp.GetLength()); 
     printf("���:%sn", getTemp); 

     free(getTemp); 



  char *getTemp = (LPSTR)(LPCSTR)strData;
	*/
	//MultiByteToWideChar(CP_ACP, 0, filename, strlen(filename), NULL, NULL);
	
/*	nNeedByte = MultiByteToWideChar( CP_UTF8	, 
								0				,
								strData.GetBuffer(0), 
								-1				,
								NULL			, 
								0			
								);
*/
	/*
	char *getTemp=NULL; 

     getTemp = malloc(strTemp.GetLength()+1); 
     strcpy(getTemp, strTemp.GetBuffer(strTemp.GetLength()); 
	*/
/*
	_TCHAR	*pszDestinationData;
	INT		nWCharNeeded;
	INT		nSouceDataByte;
	INT nUNILENG;
	CHAR* TEMP = NULL;
	//pszDestinationData = (CHAR*) malloc(sizeof(CHAR) * nBytesNeeded + sizeof(CHAR));
	nUNILENG = strData.GetLength();
	pszDestinationData = (_TCHAR*) malloc(sizeof(_TCHAR) * nUNILENG + sizeof(_TCHAR));
	//strcpy(TEMP, strData.GetBuffer(strData.GetLength())); 
	strcpy(pszDestinationData, strData); 
*/

/**********************************************

������ ��ġ


*************************************************/
/*
INT CMarcConvertApi::GetMarcStruct(CString* strMarc)
{
	CString strMarcTemp;
//	CString strMarcRecKey;
//	CString strLead;
//	CString strDirectory;
//	CString strField;
	
	INT		nFindLocate;
	INT		nGetLastIdx;
	INT		nCode;
	_TCHAR strCode;
	nCode   = 30;
	strCode = (_TCHAR)nCode;  


//	strMarcRecKey.Empty();
	strLead.Empty();
//	strDirectory.Empty();	
//	strField.Empty();

	strMarcTemp.Replace(_T("\r"),_T(""));
	strMarcTemp.Replace(_T("\n"),_T(""));
	
	//20071114 UPDATE PJW : �ڵ尣��ȭ
	//strLead = strMarcTemp.Mid(0, 24);
	//struMarc.strLead = strLead;
	struMarc.strLead = strMarcTemp.Mid(0, 24);
	nFindLocate = strMarcTemp.Find(strCode);
	//20071114 UPDATE PJW : �ڵ尣��ȭ
	//strDirectory = strMarcTemp.Mid(24, nFindLocate-24);
	//struMarc.strDirectory = strDirectory;
	struMarc.strDirectory = strMarcTemp.Mid(24, nFindLocate-24);
	struMarc.nDirectoryCnt = struMarc.strDirectory.GetLength()/12;

	nGetLastIdx = strMarcTemp.GetLength();
	//20071114 UPDATE PJW : �ڵ尣��ȭ
	//strField =strMarcTemp.Mid( nFindLocate, nGetLastIdx - nFindLocate);
	//struMarc.strField = strField;
	struMarc.strField =strMarcTemp.Mid( nFindLocate, nGetLastIdx - nFindLocate);

	SetDirectory();

//	nGetLastIdx = struMarc.strField.GetLength();	
	return 0;
}
*/

