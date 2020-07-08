// MakeSearchData.cpp: implementation of the CMakeSearchData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeSearchData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeSearchData::CMakeSearchData(CESL_Mgr * pParentMgr)
{
	pMgr = pParentMgr;
}

CMakeSearchData::~CMakeSearchData()
{

}

INT CMakeSearchData::GetKeyWord(CString &targetStr)
{
	((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(targetStr);
	//=====================================================
	//2008.12.11 REM BY PJW : IDX_TITLE�� ���߱� ���ؼ� ���� 
	//targetStr.Replace(_T("."), _T(""));
	//=====================================================
	return 0;

}

INT CMakeSearchData::GetKeyWord(TCHAR* szpData, TCHAR* szpKeyword)
{
	((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(szpData, szpKeyword);	
	return 0;
}

INT CMakeSearchData::ChangeHanjaToHangul(CString &targetStr)
{
	if(targetStr.GetLength() == 0) return 0;
	INT ids = ((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(targetStr);
	if(ids < 0) return -1000;
	return 0;
}

INT CMakeSearchData::IsValidYearData(CString strYearData)
{
	if(strYearData.GetLength() == 0) return 0;

	if(strYearData.GetLength() != 4)
		return -9000;
	if(!CheckDigit(strYearData))
		return -9001;
	return 0;
}

BOOL CMakeSearchData::CheckDigit(CString str)
{
	if(str.IsEmpty()) return FALSE;

	TCHAR * tmp = str.GetBuffer(0);
	INT size = str.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;
}
/*
INT  CMakeSearchData::RemoveSpecialChar(CString &strSrc)
{
	CString strReturn;

	INT nCount = strSrc.GetLength();

	TCHAR *pBuffer = strSrc.GetBuffer(0);
	
	_TUCHAR ch1;
	_TUCHAR ch2;
	for (INT i = 0; i < nCount; i++)
	{
		ch1 = pBuffer[i];
		ch2 = pBuffer[i+1];

		// 2Byte
		if (ch1 & 0x80)
		{
			if (ch1 >= 0xa1 && ch1 <= 0xa3 && ch2 >= 0xa1)
			{
				if (ch1 != 0xa3 || !(ch2 >= 0xb0 && ch2 <= 0xb9)) // ���Ŵ��..
					;
				else // 2Byte ���� ==> 1Byte ���ڷ�
					strReturn += '0' + (ch2 - 0xb0);

				i++;
			}
			else
			{
				strReturn += ch1;
				strReturn += ch2;
				i++;
			}
		}
		else if ((ch1 >= 'A' && ch1 <= 'Z') ||      // �빮�� ���� 
			     (ch1 >= 'a' && ch1 <= 'z') ||      // �ҹ��� ����
			     (ch1 >= '0' && ch1 <= '9')


		{
			strReturn += ch1;
		}

	}

	strSrc = strReturn;

	return 0;
}
*/


//--------------------------------------------------------------------------------------
// üũ����Ʈ�� ���� 9�ڸ� ISBN�� �Է�ȭ�� '89'�� üũ����Ʈ�� ���� �߰� ISBN��
// üũ����Ʈ�� �������ִ� �Լ�
// �Է� �� : 
//		.ptInputISBN : 9�ڸ� ISBN ( '89'�� ���ԵǾ� �־�� �Ѵ�. )
//		.ptReturnISBN : �̰��� �߰� ISBN�� ����. �� ���ϰ�. XXX-XXXX �̷���.
//		.ptReturnValue : üũ����Ʈ�� �� ������ ���ϵȴ�.
// ���� �� : 
//		. 0 : �������� ����
//		.-1 : �Է��� ISBN�� 9�ڸ��� �ƴ� ��
//--------------------------------------------------------------------------------------
INT CMakeSearchData::ECO_ReturnISBNCheckDigit( TCHAR *ptInputISBN, TCHAR *ptReturnISBN, TCHAR *ptReturnValue, TCHAR* pszHeader )
{
TCHAR tmpStr[3];
	TCHAR caInputISBN[256];

	INT nCheckSum, nCheckRest, nISBNData, nDash, nDestLength;
	
	_tcscpy( caInputISBN, ptInputISBN );
	
	GetIsbnNumberData( caInputISBN, caInputISBN );

	if ( _tcsncmp( caInputISBN, _T("89"), 2 ) == 0 ) nDestLength = _SHORT_ISBN_LNG;
	else if ( _tcsncmp( caInputISBN, _T("978"), 3 ) == 0 || _tcsncmp( caInputISBN, _T("979"), 3 ) == 0 ) nDestLength = _LONG_ISBN_LNG;
	else
	{
		ptReturnValue[0] = 0;
        return 100;
	}

	caInputISBN[nDestLength-1] = 0;	
	if ( _tcsclen( caInputISBN ) != nDestLength-1 ) return -1;    

	// Create ISBN CheckDigit
	nCheckSum = 0;
	tmpStr[1] = 0;
	if ( nDestLength == _SHORT_ISBN_LNG )
	{
		for ( INT i = 0, j= 10; i < 9; i++, j-- ) {
			tmpStr[0] = caInputISBN[i];
			nCheckSum += ( _ttoi( tmpStr ) * j );
		}	

		nCheckRest = nCheckSum % 11;

		switch( nCheckRest )
		{
			case 0 :
				_stprintf( ptReturnValue, _T("%d"), nCheckRest );
				break;
			case 1 :
				_stprintf( ptReturnValue, _T("X") );
				break;
			default :
				_stprintf( ptReturnValue, _T("%d"), 11 - nCheckRest );
				break;
		}

		memcpy( tmpStr, &caInputISBN[2], sizeof(TCHAR)*2 );
		tmpStr[2] = 0;
		nISBNData = _ttoi( tmpStr );
		
		if ( 0  <= nISBNData && nISBNData <= 24 ) nDash = 2;
		if ( 25 <= nISBNData && nISBNData <= 54 ) nDash = 3;
		if ( 55 <= nISBNData && nISBNData <= 84 ) nDash = 4;
		if ( 85 <= nISBNData && nISBNData <= 94 ) nDash = 5;
		if ( 95 <= nISBNData && nISBNData <= 99 ) nDash = 6;

		_tcscpy(pszHeader, caInputISBN); pszHeader[2] = 0;		
		memcpy( ptReturnISBN, caInputISBN + 2, sizeof(TCHAR)*nDash );
		memcpy( ptReturnISBN + nDash + 1, caInputISBN + nDash + 2 , sizeof(TCHAR)*(9 - nDash) );

		ptReturnISBN[nDash] = '-';
		ptReturnISBN[8] = 0;
	}
	else
	{
		for ( INT i = 0; i<12; i++ ) 
		{
			tmpStr[0] = caInputISBN[i];
			if ( i%2 == 0 ) nCheckSum += ( _ttoi(tmpStr) * 1 );
			else nCheckSum += ( _ttoi(tmpStr) * 3 );			
		}

		INT nCheckDigit;
		if ( nCheckSum%10 == 0 ) nCheckDigit = 0;
		else nCheckDigit = 10 - nCheckSum%10;

		_stprintf( ptReturnValue, _T("%d"), nCheckDigit );

		memcpy( tmpStr, &caInputISBN[5], sizeof(TCHAR)*2 );
		tmpStr[2] = 0;
		nISBNData = _ttoi( tmpStr );
		
		if ( 0  <= nISBNData && nISBNData <= 24 ) nDash = 2;
		if ( 25 <= nISBNData && nISBNData <= 54 ) nDash = 3;
		if ( 55 <= nISBNData && nISBNData <= 84 ) nDash = 4;
		if ( 85 <= nISBNData && nISBNData <= 94 ) nDash = 5;
		if ( 95 <= nISBNData && nISBNData <= 99 ) nDash = 6;

		memcpy( pszHeader, caInputISBN, sizeof(TCHAR)*3 );
		memcpy( pszHeader + 3 + 1, &caInputISBN[3], sizeof(TCHAR)*2 );
		pszHeader[3] = '-';
		pszHeader[6] = 0;					

		memcpy( ptReturnISBN, caInputISBN + 5, sizeof(TCHAR)*nDash );
		memcpy( ptReturnISBN + nDash + 1, caInputISBN + nDash + 5 , sizeof(TCHAR)*(12 - nDash) );
		ptReturnISBN[nDash] = '-';
		ptReturnISBN[8] = 0;		
	}

	return 0;
}

// nType default 0
//	0 üũ����Ʈ ���� def
//	1 üũ����Ʈ ������
//  2 KOLISNET �˻������� ������� ===> <word> %s <in> est_code
//  return 100 : ���� �̻�
//         200 : ������ �̻�
INT CMakeSearchData::S97ValueMakeIsbn(TCHAR* pszIn, CString &out, INT nType)
{
	INT i, ids, nDestLng;
	TCHAR szTmp[128], pszUnFormatISBN[64];
	TCHAR szPubNo[16], szCheck[16], szHeader[16];
	TCHAR szFirstIsbn[64], szSecondIsbn[64];
	CString strValue;

	// (��Ʈ) ó��
	_tcscpy( szTmp, pszIn ); 
    strValue = _tcstok( szTmp, _T("(") );
    if ( strValue.IsEmpty() ) {
		return 100;        
    }

	_tcscpy(pszUnFormatISBN, strValue.GetBuffer(0));

	// Ư����������
    GetIsbnNumberData( pszUnFormatISBN , pszUnFormatISBN );

	// ���� �Ǵ� X���� üũ
	for(i=0; i< _tcslen(pszUnFormatISBN); i++)
	{
		if ( !(pszUnFormatISBN[i] >= '0' || pszUnFormatISBN[i] <= '9' || pszUnFormatISBN[i] == 'X') )
		{
			return 200;
		}
	}

	if ( _tcsncmp(pszUnFormatISBN, _T("89"), 2) == 0 ) nDestLng = _SHORT_ISBN_LNG;
	else if ( _tcsncmp(pszUnFormatISBN, _T("97889"), 5) == 0 || _tcsncmp(pszUnFormatISBN, _T("97989"), 5) == 0) nDestLng = _LONG_ISBN_LNG;
	else 
	{
		return 200;
	}
	
	if ( nDestLng == _SHORT_ISBN_LNG && _tcslen(pszUnFormatISBN) != _SHORT_ISBN_LNG )
	{
		return 100;		
	}
	if ( nDestLng == _LONG_ISBN_LNG && _tcslen(pszUnFormatISBN) != _LONG_ISBN_LNG )
	{
		return 100;
	}

	/*
	// �Էµ� ���� üũ����Ʈ �����Ͽ� ù��° ISBN �����.
	ids = ECO_ReturnISBNCheckDigit(pszUnFormatISBN, szPubNo, szCheck, szHeader);
	if ( ids == 100 ) return 200;
	else if (ids == -1 ) return 100;*/

	if ( nType == 1 ) {		
		_tcscpy(szFirstIsbn, pszUnFormatISBN);
		szFirstIsbn[nDestLng-1] ='\0';
	}
	else {	
		_tcscpy(szFirstIsbn, pszUnFormatISBN);
	}	

	GetIsbnNumberData( szFirstIsbn , szFirstIsbn );
	if ( _tcsncmp(szFirstIsbn, _T("979"), 3) == 0 ) {
		if ( nType == 2 ) {
			out.Format(_T(" ( <WORD> ( %s ) <IN> EST_CODE ) "), szFirstIsbn);
		}
		else {
			out.Format(_T(" ( %s <IN> ZONE_ST_CODE ) "), szFirstIsbn);
		}
		
		return 0;
	}
	
	if ( _tcsncmp(szFirstIsbn, _T("978"), 3) == 0 ) {  // 10�ڸ� ISBN�� �����.
		_tcscpy(szTmp, &pszUnFormatISBN[3]);
	}
	else if ( _tcsncmp(szFirstIsbn, _T("89"), 2) == 0 ) { // 13�ڸ��� �����.		
		_stprintf(szTmp, _T("978%s"), pszUnFormatISBN);
	}
	else return 200;

	ids = ECO_ReturnISBNCheckDigit(szTmp, szPubNo, szCheck, szHeader);
	if ( ids == 100 ) return 200;
	else if (ids == -1 ) return 100;

	if ( nType == 1 ) {
		_stprintf(szSecondIsbn, _T("%s%s"), szHeader, szPubNo);
	}
	else {
		_stprintf(szSecondIsbn, _T("%s%s%s"), szHeader, szPubNo, szCheck);
	}
	GetIsbnNumberData( szSecondIsbn , szSecondIsbn );

	// KOLIS-NET <word> %s <in> est_code
	if ( nType == 2 ) {
		//out.Format(_T(" ( <WORD> ( %s ) <IN> EST_CODE OR <WORD> ( %s ) <IN> EST_CODE ) "), szFirstIsbn, szSecondIsbn);
		out.Format(_T(" ( ( %s ) <IN> ZST_CODE <OR> ( %s ) <IN> ZST_CODE ) "), szFirstIsbn, szSecondIsbn);		
	}
	// KOLIS
	else {
		out.Format(_T(" ( ( %s ) <IN> ZONE_ST_CODE <OR> ( %s ) <IN> ZONE_ST_CODE ) "), szFirstIsbn, szSecondIsbn);
	}
	
	return 0;	
}

void CMakeSearchData::GetIsbnNumberData( TCHAR * pInPutData, TCHAR * pOutPutData )
{
	INT i, nPutIdx=0;
	TCHAR szValue[256];
	for(i=0; i<_tcslen(pInPutData); i++)
	{
		if ( (pInPutData[i] >= '0' && pInPutData[i] <= '9') || pInPutData[i] == 'X' )
		{
			szValue[nPutIdx] = pInPutData[i];
			nPutIdx++;
		}
	}
	szValue[nPutIdx] = '\0';
	_tcscpy(pOutPutData, szValue);
	return;
}



// nType default 0
//	0 ���ǰ˻�
//	1 ������ �˻�
//  2 ��ġ �˻�
INT CMakeSearchData::S97ValueMake(LPCTSTR in, CString &out, INT nType)
{
	INT		nPos, i, nNeedByte;
	CString str;
	CHAR	*pTemp = NULL;
	TCHAR	*pUniTemp = NULL;
	BOOL	bMix = FALSE;

	INT		nLen = _tcslen(in);
	pUniTemp = new TCHAR[nLen+sizeof(TCHAR)];
	pUniTemp[0] = '\0';
	_tcscpy ( pUniTemp, in );
	
	/////////////////////////////////////////////////////////
	// 1. *�� �ٿ��� �ϴ��� Ȯ���Ѵ�.
	/////////////////////////////////////////////////////////
	if ( nType == 1 ) {
		for( i = 0 ; i < nLen ; i++ ){
			if( (_T('a') <= in[i] && in[i] <= _T('z') ) || 
				(_T('A') <= in[i] && in[i] <= _T('Z') ) || 
				(_T('0') <= in[i] && in[i] <= _T('9') ) ) {
				bMix = TRUE;
				break;
			}
		}
	}

	/////////////////////////////////////////////////////////
	// 2. Ư�����ڸ� �������� �ٲ۴�.
	/////////////////////////////////////////////////////////
	/*
	nNeedByte = WideCharToMultiByte(
									CP_ACP		, // code page
									0			, // performance and mapping flags
									in			, // wide-character string
									-1			, // number of chars in string
									NULL		, // buffer for new string
									0			, // size of buffer
									"ڱ"		, // default for unmappable chars
									NULL		  // set when default char used
									);
	if ( nNeedByte == 0 ) {
		delete [] pUniTemp;
		return GetLastError();
	}

	pTemp = new CHAR[nNeedByte];
	pTemp[0] = '\0';
	nNeedByte = WideCharToMultiByte( CP_ACP		, 
									 0			,
									 in			, 
									 -1			,
									 pTemp		, 
									 nNeedByte	,
									 "ڱ"		, 
									 NULL
									 );
	if ( nNeedByte == 0 )
		return GetLastError();

	nPos = 0;
	for ( i = 0 ; i < (INT)strlen(pTemp) ; i++ ) {
		if ( IsDBCSLeadByte(pTemp[i]) ) {
			i++;
			nPos ++;
			continue;
		}
		if ( pTemp[i] < 48 ) {
			pUniTemp[nPos] = ' ';
		} else if ( pTemp[i] >= 58 && pTemp[i] <= 64 ) {
			pUniTemp[nPos] = ' ';
		} else if ( pTemp[i] >= 91 && pTemp[i] <= 96 ) {
			pUniTemp[nPos] = ' ';
		} else if ( pTemp[i] > 122 ) {
			pUniTemp[nPos] = ' ';
		}
		nPos ++;
	}
	*/

	((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->ChangeBlankSpecialChar( pUniTemp, pUniTemp );

	/////////////////////////////////////////////////////////
	// 3. ���ڸ� �ѱ۷� �ٲ۴�.
	/////////////////////////////////////////////////////////
	((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(pUniTemp, pUniTemp);
	str.Format(_T("%s"), pUniTemp);
	delete []pUniTemp;
	pUniTemp = NULL;

	/////////////////////////////////////////////////////////
	// 4. ���� �ҹ��ڸ� �빮�ڷ� �ٲ۴�.
	/////////////////////////////////////////////////////////
	str.TrimLeft();
	str.TrimRight();
	str.MakeUpper();

	nPos = str.Find(_T("  "));
	while( nPos >= 0 ) {
		str.Replace( _T("  "), _T(" ") );
		nPos = str.Find(_T("  "));
	}

	/////////////////////////////////////////////////////////
	// 5. ���� ��ȣ�Ѵ�.
	/////////////////////////////////////////////////////////
	str = _T(" ") + str + _T(" ");
	str.Replace(_T(" AND "), _T(" ��NNN�� "));
	str.Replace(_T(" OR "), _T(" ��RRR�� "));
	str.Replace(_T(" NOT "), _T(" ��TTT�� "));
	str.TrimLeft();
	str.TrimRight();

	if ( 2 == nType ) {
		// ��ġ�˻��� ��� ������ �����Ѵ�.
		str.Replace(_T(" "), _T(""));
	}
	/////////////////////////////////////////////////////////
	// 6. ���ǰ˻��� ��� ������ <AND>�� �ٲ۴�.
	/////////////////////////////////////////////////////////
	if ( nType == 0 )
		str.Replace( _T(" "), _T("��SSS��") );
	pUniTemp = new TCHAR[str.GetLength()+1];
	pUniTemp[0] = '\0';
	_tcscpy ( pUniTemp, str.GetBuffer(0) );
	//Get_Keyword(pUniTemp, pUniTemp);
	str.Format(_T("%s"), pUniTemp);
	delete []pUniTemp;
	pUniTemp = NULL;

	str.Replace(_T("��NNN��"), _T("\"AND\""));
	str.Replace(_T("��RRR��"), _T("\"OR\""));
	str.Replace(_T("��TTT��"), _T("\"NOT\""));
	str.Replace(_T("��SSS��"), _T("<AND>"));

	INT		nAllocSize = str.GetLength()+1;
	pUniTemp = (TCHAR*)malloc(sizeof(TCHAR)*nAllocSize);
	if ( NULL == pUniTemp ) {
		return -10;
	}
	pUniTemp[0] = '\0';

	/////////////////////////////////////////////////////////
	// 7. �ѹ���Ʈ ���ڿ� �ι���Ʈ ���� ���̿� ������ �Է��Ѵ�.
	/////////////////////////////////////////////////////////
	BOOL	bUni = FALSE;
	TCHAR	sTemp[2] = {NULL};

	for ( i = 0 ; i < str.GetLength() ; i++ ) {
		sTemp[0] = str.GetAt(i);
		sTemp[1] = '\0';
		nNeedByte = WideCharToMultiByte( CP_ACP, 0, sTemp, -1, NULL, 0, NULL, NULL );
		if ( i == 0 && nNeedByte <= 2 ) {
			//ù��°�� �����̶�� �տ� ������ �Է��Ѵ�.
			nAllocSize++;
			pUniTemp = (TCHAR*)realloc(pUniTemp, sizeof(TCHAR) * nAllocSize);
			_tcscat( pUniTemp, _T(" ") );
		}
		if ( sTemp[0] == ' ' ) {
			_tcscat( pUniTemp, sTemp );
			continue;
		}
		if ( bUni ) {
			//2����Ʈ ���ڿ��ٸ�
			if ( nNeedByte <= 2 ) {
				// 1����Ʈ ���ڶ��
				nAllocSize++;
				pUniTemp = (TCHAR*)realloc(pUniTemp, sizeof(TCHAR) * nAllocSize);
				_tcscat( pUniTemp, _T(" ") );
				bUni = FALSE;
			}
		} else {
			// 1����Ʈ ���ڿ��ٸ�
			if ( nNeedByte > 2 ) {
				// 2����Ʈ ���ڶ��
				nAllocSize++;
				pUniTemp = (TCHAR*)realloc(pUniTemp, sizeof(TCHAR) * nAllocSize);
				_tcscat( pUniTemp, _T(" ") );
				bUni = TRUE;
			}
		}
		_tcscat( pUniTemp, sTemp );
	}
	str.Format(_T("%s"), pUniTemp);
	free(pUniTemp);
	pUniTemp = NULL;
	str.TrimLeft();
	str.TrimRight();
	if ( str.IsEmpty() ) {
		out = str;
		return 0;
	}

	/////////////////////////////////////////////////////////
	// 8. *�� �ٿ��� �Ұ�� ���̰� �˻��� �糡�� ������ �߰��Ѵ�.
	/////////////////////////////////////////////////////////
	if ( bMix ) {
		// �������̸鼭 �����Ǵ� ���ڰ� �ִٸ� ���� *�� ���δ�.
		//str += _T("*");
	}
	str = _T(" ") + str + _T(" ");

	if ( nType == 0 ) {
		str = _T("(") + str + _T(")");
	}
	
	out = str;
	return 0;
}

// nType default 0
//	0 ���ǰ˻�
//	1 ������ �˻�
//  2 ��ġ �˻�
INT CMakeSearchData::S97ValueMake_Kolisnet(LPCTSTR in, CString &out, INT nType)
{
	INT		nPos, i, nNeedByte;
	CString str;
	CHAR	*pTemp = NULL;
	TCHAR	*pUniTemp = NULL;
	BOOL	bMix = FALSE;

	INT		nLen = _tcslen(in);
	pUniTemp = new TCHAR[nLen+1];
	pUniTemp[0] = '\0';
	_tcscpy ( pUniTemp, in );

	/////////////////////////////////////////////////////////
	// 1. *�� �ٿ��� �ϴ��� Ȯ���Ѵ�.
	/////////////////////////////////////////////////////////
	if ( nType == 1 ) {
		for( i = 0 ; i < nLen ; i++ ){
			if( (_T('a') <= in[i] && in[i] <= _T('z') ) || 
				(_T('A') <= in[i] && in[i] <= _T('Z') ) || 
				(_T('0') <= in[i] && in[i] <= _T('9') ) ) {
				bMix = TRUE;
				break;
			}
		}
	}
	
	/////////////////////////////////////////////////////////
	// 2. Ư�����ڸ� �������� �ٲ۴�.
	/////////////////////////////////////////////////////////
	/*
	nNeedByte = WideCharToMultiByte(
									CP_ACP		, // code page
									0			, // performance and mapping flags
									in			, // wide-character string
									-1			, // number of chars in string
									NULL		, // buffer for new string
									0			, // size of buffer
									"ڱ"		, // default for unmappable chars
									NULL		  // set when default char used
									);
	if ( nNeedByte == 0 ) {
		delete [] pUniTemp;
		return GetLastError();
	}

	pTemp = new CHAR[nNeedByte];
	pTemp[0] = '\0';
	nNeedByte = WideCharToMultiByte( CP_ACP		, 
									 0			,
									 in			, 
									 -1			,
									 pTemp		, 
									 nNeedByte	,
									 "ڱ"		, 
									 NULL
									 );
	if ( nNeedByte == 0 )
		return GetLastError();

	nPos = 0;
	for ( i = 0 ; i < (INT)strlen(pTemp) ; i++ ) {
		if ( IsDBCSLeadByte(pTemp[i]) ) {
			i++;
			nPos ++;
			continue;
		}
		if ( pTemp[i] < 48 ) {
			pUniTemp[nPos] = ' ';
		} else if ( pTemp[i] >= 58 && pTemp[i] <= 64 ) {
			pUniTemp[nPos] = ' ';
		} else if ( pTemp[i] >= 91 && pTemp[i] <= 96 ) {
			pUniTemp[nPos] = ' ';
		} else if ( pTemp[i] > 122 ) {
			pUniTemp[nPos] = ' ';
		}
		nPos ++;
	}
	*/	
	
	((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->ChangeBlankSpecialChar( pUniTemp, pUniTemp );	
	/////////////////////////////////////////////////////////
	// 3. ���ڸ� �ѱ۷� �ٲ۴�.
	/////////////////////////////////////////////////////////	
	((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(pUniTemp, pUniTemp);		
	str.Format(_T("%s"), pUniTemp);		
	//delete pUniTemp;		
	//pUniTemp = NULL;	
	/////////////////////////////////////////////////////////
	// 4. ���� �ҹ��ڸ� �빮�ڷ� �ٲ۴�.
	/////////////////////////////////////////////////////////
	str.TrimLeft();
	str.TrimRight();
	str.MakeUpper();

	nPos = str.Find(_T("  "));
	while( nPos >= 0 ) {
		str.Replace( _T("  "), _T(" ") );
		nPos = str.Find(_T("  "));
	}

	/////////////////////////////////////////////////////////
	// 5. ���� ��ȣ�Ѵ�.
	/////////////////////////////////////////////////////////
	
	str = _T(" ") + str + _T(" ");	
	str.Replace(_T(" AND "), _T(" ��NNN�� "));	
	str.Replace(_T(" OR "), _T(" ��RRR�� "));	
	str.Replace(_T(" NOT "), _T(" ��TTT�� "));	
	str.TrimLeft();	
	str.TrimRight();	

	if ( nType ) {
		// ��ġ ������ �˻��� ��� ������ �����Ѵ�.
		str.Replace(_T(" "), _T(""));
	}	
	/////////////////////////////////////////////////////////
	// 6. ���ǰ˻��� ��� ������ <AND>�� �ٲ۴�.
	/////////////////////////////////////////////////////////
	if ( nType == 0 )
		str.Replace( _T(" "), _T("��SSS��") );
//	pUniTemp = new TCHAR[str.GetLength()+1];
//	pUniTemp[0] = '\0';
//	_tcscpy ( pUniTemp, str );
//	//Get_Keyword(pUniTemp, pUniTemp);
//	str.Format(_T("%s"), pUniTemp);
//	delete pUniTemp;
//	pUniTemp = NULL;

	str.Replace(_T("��NNN��"), _T("\"AND\""));
	str.Replace(_T("��RRR��"), _T("\"OR\""));
	str.Replace(_T("��TTT��"), _T("\"NOT\""));
	str.Replace(_T("��SSS��"), _T("<AND>"));

	INT		nAllocSize = str.GetLength()+1;
	pUniTemp = (TCHAR*)malloc(sizeof(TCHAR)*nAllocSize);
	if ( NULL == pUniTemp ) {
		return -10;
	}
	pUniTemp[0] = '\0';

	/////////////////////////////////////////////////////////
	// 7. �ѹ���Ʈ ���ڿ� �ι���Ʈ ���� ���̿� ������ �Է��Ѵ�.
	/////////////////////////////////////////////////////////
	BOOL	bUni = FALSE;
	TCHAR	sTemp[2] = {NULL};

	if ( nType == 0 ) {
		// ���� �˻��� ��쿡�� 2����Ʈ ���ڿ� �ѹ���Ʈ ���ڻ翡�� ������ �߰��Ѵ�.
		for ( i = 0 ; i < str.GetLength() ; i++ ) {
			sTemp[0] = str.GetAt(i);
			sTemp[1] = '\0';
			nNeedByte = WideCharToMultiByte( CP_ACP, 0, sTemp, -1, NULL, 0, NULL, NULL );
			if ( i == 0 && nNeedByte <= 2 ) {
				//ù��°�� �����̶�� �տ� ������ �Է��Ѵ�.
				nAllocSize++;
				pUniTemp = (TCHAR*)realloc(pUniTemp, sizeof(TCHAR) * nAllocSize);
				_tcscat( pUniTemp, _T(" ") );
			}
			if ( sTemp[0] == ' ' ) {
				_tcscat( pUniTemp, sTemp );
				continue;
			}
			if ( bUni ) {
				//2����Ʈ ���ڿ��ٸ�
				if ( nNeedByte <= 2 ) {
					// 1����Ʈ ���ڶ��
					nAllocSize++;
					pUniTemp = (TCHAR*)realloc(pUniTemp, sizeof(TCHAR) * nAllocSize);
					_tcscat( pUniTemp, _T(" ") );
					bUni = FALSE;
				}
			} else {
				// 1����Ʈ ���ڿ��ٸ�
				if ( nNeedByte > 2 ) {
					// 2����Ʈ ���ڶ��
					nAllocSize++;
					pUniTemp = (TCHAR*)realloc(pUniTemp, sizeof(TCHAR) * nAllocSize);
					_tcscat( pUniTemp, _T(" ") );
					bUni = TRUE;
				}
			}
			_tcscat( pUniTemp, sTemp );
		}
		str.Format(_T("%s"), pUniTemp);
		free(pUniTemp);
		pUniTemp = NULL;
	} else {
		BOOL bOneByte = FALSE;
		for ( i = 0 ; i < str.GetLength() ; i++ ) {
			sTemp[0] = str.GetAt(i);
			sTemp[1] = '\0';
			nNeedByte = WideCharToMultiByte( CP_ACP, 0, sTemp, -1, NULL, 0, NULL, NULL );
			if ( i == 0 && nNeedByte <= 2 ) {
				//ù��°�� �����̶�� �տ� ������ �Է��Ѵ�.
				nAllocSize++;
				pUniTemp = (TCHAR*)realloc(pUniTemp, sizeof(TCHAR) * nAllocSize);
				_tcscat( pUniTemp, _T(" ") );
			}
			if ( sTemp[0] == ' ' ) {
				_tcscat( pUniTemp, sTemp );
				continue;
			}
			if ( bUni ) {
				//2����Ʈ ���ڿ��ٸ�
				if ( nNeedByte <= 2 ) {
					// 1����Ʈ ���ڶ��
					nAllocSize++;
					if ( bOneByte ) {
						nAllocSize++;
					}
					pUniTemp = (TCHAR*)realloc(pUniTemp, sizeof(TCHAR) * nAllocSize );
					if ( bOneByte ) {
						//_tcscat( pUniTemp, _T("*") );
						_tcscat( pUniTemp, _T(" ") );
					}
					_tcscat( pUniTemp, _T(" ") );
					bUni = FALSE;
				}
			} else {
				if ( i != 0 ) {
					bOneByte = TRUE;
				}
				// 1����Ʈ ���ڿ��ٸ�
				if ( nNeedByte > 2 ) {
					// 2����Ʈ ���ڶ��
					nAllocSize++;
					pUniTemp = (TCHAR*)realloc(pUniTemp, sizeof(TCHAR) * nAllocSize);
					_tcscat( pUniTemp, _T(" ") );
					bUni = TRUE;
				}
			}
			_tcscat( pUniTemp, sTemp );
		}
		str.Format(_T("%s"), pUniTemp);
		free(pUniTemp);
		pUniTemp = NULL;
	}
	str.TrimLeft();
	str.TrimRight();
	if ( str.IsEmpty() ) {
		out = str;
		return 0;
	}

	/////////////////////////////////////////////////////////
	// 8. *�� �ٿ��� �Ұ�� ���̰� �˻��� �糡�� ������ �߰��Ѵ�.
	/////////////////////////////////////////////////////////
	if ( bMix ) {
		// �������̸鼭 �����Ǵ� ���ڰ� �ִٸ� ���� *�� ���δ�.
		/*
		INT pPos = str.ReverseFind(' ');
		if ( pPos == str.GetLength() - 2 ) {
			sTemp[0] = str.GetAt(str.GetLength()-1);
			sTemp[1] = '\0';
			nNeedByte = WideCharToMultiByte( CP_ACP, 0, sTemp, -1, NULL, 0, NULL, NULL );
			if ( nNeedByte > 2 ) {
				str += _T("*");
			}
		} else {
			str += _T("*");
		}
		*/
	}
	str = _T(" ") + str + _T(" ");

	if ( nType == 0 ) {
		str = _T("(") + str + _T(")");
	}

	out = str;
	return 0;
}


//===================================================
//2010.05.02 ADD BY PJW : ��Ʈ���� ��ȯ �߰�
INT CMakeSearchData::UnicodeSpecialConvert(CString &targetStr)
{	
	if(targetStr.GetLength() == 0) return 0;
	((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->UnicodeSpecialConvert(targetStr);
	return 0;
}
//===================================================