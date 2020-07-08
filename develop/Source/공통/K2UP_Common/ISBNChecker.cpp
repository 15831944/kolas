// ISBNChecker.cpp: implementation of the CISBNChecker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ISBNChecker.h"

#include "Keyword.h"
			   
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CISBNChecker::CISBNChecker()
{

}

CISBNChecker::~CISBNChecker()
{

}



/** ISBN�� ��Ģ�� �ǰ��Ͽ� ��ȯ�Ѵ�.
ISBN�� '()'�� ������(Set ISBN�� ���) '()'�� ���� ISBN üũ�� �ϸ�,
'-' ��ġ�� ��Ȯ�ϰ� �����Ѵ�.
'89'�� �����ϴ� ���� ISBN�� ���ؼ��� ó���Ѵ�.

  ECO_ConvertISBN(in, out, &nreturn );
  if ( nreturn ) {
	  if (nreturn == -1) 
		  MessageBox( _T("ISBN �ڸ����� Ȯ���Ͽ� �ֽʽÿ�."), _T("ISBN CHECK"), MB_ICONEXCLAMATION);
	  if (nreturn == -2) 
		  MessageBox( _T("ISBN üũ����Ʈ�� Ȯ���Ͽ� �ֽʽÿ�."), _T("ISBN CHECK"), MB_ICONEXCLAMATION);
	  ...
  }
  
	@memo	ISBN �˻� �� 89-PPP-SSSS-C ���·� ����
	@param	szUnFormattedISBN		�Է�ISBN
	@param	szCorrectFormattedISBN	ISBN�� �°� �����Ǿ��� ��°�( 89-XXX-XXXX-X )
	@param	nCorrect				�Է�ISBN�� �ڸ����� �ȸ����� -1, üũ����Ʈ�� Ʋ���� -2 ������ 0
	@return 0	üũ����Ʈ�� Ʋ�Ȱų� ������ 0�� ����, �� ���ο����� ���� ���
	@return n<0 ���ο���
	@author �ڱ���
	@since	2002-11-21 
*/
INT CISBNChecker::ConvertISBN (TCHAR* szUnFormattedISBN, 
							   TCHAR* szCorrectFormattedISBN, 
							   INT * nCorrect)
{
    INT     ISBNData;
	TCHAR    *szExtraStrPointer;		// szUnFormattedISBN���� '('������ ���ڿ�
    TCHAR    szTemp[10000];			// �ӽ� ���ڿ�
	TCHAR    szTempISBN[10000];		// �ӽ� ISBN
    INT     i, j, nCheckSum, nCheckRest, nFirstDash, nSecondDash, nThirdDash;
	
	*nCorrect = 0;

    // '('������ ���ڿ��� ���� �����Ѵ�.
    szExtraStrPointer = _tcschr( szUnFormattedISBN, '(' );

    // ��ƮISBN�� ��� '(' �� �����ؾ��Ѵ�.
	_tcscpy(szTemp, szUnFormattedISBN); 

	_tcscpy(szTempISBN, szUnFormattedISBN);

    //szTempISBN = _tcstok( szTemp, _T("(") );
    //if ( szTempISBN == NULL ) {
        //_tcscpy( szCorrectFormattedISBN, szUnFormattedISBN );
      //  /return 0;
    //}/

	CKeyword::getKeyword(szTempISBN , szTempISBN, FALSE);

	switch( _tcsclen( szTempISBN ) )
	{
	case 10:
		{
			// ISBN CheckDigit Check.
			nCheckSum = 0;
			szTemp[1] = 0;
    
			for( i = 0, j = 10 ; i < 9 ; i++, j-- ) 
			{
				szTemp[0] = szTempISBN[i];
				nCheckSum += ( _ttoi(szTemp) * j );
			}

			nCheckRest = nCheckSum % 11;

			switch (nCheckRest) 
			{
				case 0:
					{
						if( szTempISBN[9] != '0' )
						{
							szTempISBN[9] = '0';
							*nCorrect = -2;
						}
					}
					break;
				case 1:
					{
						if( szTempISBN[9] != 'x' && szTempISBN[9] != 'X' )
						{
							szTempISBN[9] = 'X';
							*nCorrect = -2;
						}
					}
					break;
				default:
					{
						_stprintf( szTemp, _T("%d"), 11 - nCheckRest );
						if( szTemp[0] != szTempISBN[9] )
						{
							szTempISBN[9] = szTemp[0];
							*nCorrect = -2;
						}
					}
					break;
			}

			// ISBN Form Change.
			nFirstDash = 2;
			nThirdDash = 11;

			wmemcpy( szTemp , &szTempISBN[2] , 2 );
			szTemp[2] = 0;
			ISBNData = _ttoi( szTemp );

			if ( 0  <= ISBNData && ISBNData <= 24 ) nSecondDash = 5;
			if ( 25 <= ISBNData && ISBNData <= 54 ) nSecondDash = 6;
			if ( 55 <= ISBNData && ISBNData <= 84 ) nSecondDash = 7;
			if ( 85 <= ISBNData && ISBNData <= 94 ) nSecondDash = 8;
			if ( 95 <= ISBNData && ISBNData <= 99 ) nSecondDash = 9;

			wmemcpy( szCorrectFormattedISBN                  , szTempISBN                 , 2 );
			wmemcpy( szCorrectFormattedISBN +(nFirstDash +1) , szTempISBN +2              , nSecondDash-nFirstDash-1 );
			wmemcpy( szCorrectFormattedISBN +(nSecondDash+1) , szTempISBN+(nSecondDash-1) , nThirdDash-nSecondDash-1 );
			wmemcpy( szCorrectFormattedISBN +(nThirdDash +1) , szTempISBN +9              , 1 );

			szCorrectFormattedISBN[nFirstDash]	= '-';
			szCorrectFormattedISBN[nSecondDash] = '-';
			szCorrectFormattedISBN[nThirdDash]	= '-';
			szCorrectFormattedISBN[13]			= 0	 ;

			if ( szExtraStrPointer != NULL ) 
				_tcscpy( &szCorrectFormattedISBN[13], szExtraStrPointer );

		}
		break;

	case 13:
		{
			// ISBN CheckDigit Check.
			nCheckSum = 0;
			szTemp[1] = 0;

			for( i=0; i<12; i++ )
			{
				szTemp[0] = szTempISBN[i];
				if( i%2 )	nCheckSum += _ttoi(szTemp) * 3;
				else		nCheckSum += _ttoi(szTemp) * 1;
			}

			nCheckRest = nCheckSum%10;
			if( nCheckRest != 0 )
			{
				_stprintf( szTemp, _T("%d"), (10-nCheckRest) );
			}
			else
			{
				_stprintf( szTemp, _T("%d"), nCheckRest );
			}

			if( szTemp[0] != szTempISBN[12] )
			{
				szTempISBN[12] = szTemp[0];
				*nCorrect = -2;
			}

			// ISBN Form Change.
			INT nAddDash = 3;
			nFirstDash = 2 + nAddDash + 1;
			nThirdDash = 11 + nAddDash + 1;

			wmemcpy( szTemp , &szTempISBN[2+nAddDash] , 2 );
			szTemp[2] = 0;
			ISBNData = _ttoi( szTemp );

			if ( 0  <= ISBNData && ISBNData <= 24 ) nSecondDash = 5 + nAddDash +1;
			if ( 25 <= ISBNData && ISBNData <= 54 ) nSecondDash = 6 + nAddDash +1;
			if ( 55 <= ISBNData && ISBNData <= 84 ) nSecondDash = 7 + nAddDash +1;
			if ( 85 <= ISBNData && ISBNData <= 94 ) nSecondDash = 8 + nAddDash +1;
			if ( 95 <= ISBNData && ISBNData <= 99 ) nSecondDash = 9 + nAddDash +1;

			wmemcpy( szCorrectFormattedISBN                  , szTempISBN                , nAddDash );
			wmemcpy( szCorrectFormattedISBN +(nAddDash +1)   , szTempISBN+nAddDash       , 2 );
			wmemcpy( szCorrectFormattedISBN +(nFirstDash+1)  , szTempISBN+nAddDash+2     , nSecondDash-nFirstDash-1 );
			wmemcpy( szCorrectFormattedISBN +(nSecondDash+1) , szTempISBN+(nSecondDash-2), nThirdDash-nSecondDash-1 );
			wmemcpy( szCorrectFormattedISBN +(nThirdDash +1) , szTempISBN+nAddDash+9     , 1 );

			szCorrectFormattedISBN[nAddDash]	= '-';
			szCorrectFormattedISBN[nFirstDash]	= '-';
			szCorrectFormattedISBN[nSecondDash] = '-';
			szCorrectFormattedISBN[nThirdDash]	= '-';
			szCorrectFormattedISBN[17]			= 0	 ;

			if( szExtraStrPointer != NULL ) 
			{
				_tcscpy( szCorrectFormattedISBN+17, szExtraStrPointer );
			}

		}
		break;

	default:
		{
			//if( _tcsclen( szTempISBN ) != 10 )
			{
				_tcscpy( szCorrectFormattedISBN, szUnFormattedISBN );
				*nCorrect = -1;
				return -1;
			}
		}
		break;
	}
  
    return 0;
}


/** üũ����Ʈ�� ���� 9�ڸ� ISBN�� �Է��ϸ� '89'�� üũ����Ʈ�� ���� �߰� ISBN��
üũ����Ʈ�� �������ִ� �Լ�.
	@param szInputISBN		9�ڸ� ISBN ( '89'�� ���ԵǾ� �־�� �Ѵ�. )
	@param szOutputISBN		�̰��� �߰� ISBN�� ����. �� ���ϰ�. XXX-XXXX �̷���.
	@param szReturnValue	üũ����Ʈ�� �� ������ ���ϵȴ�.
	@return 0 �������� ����
	@return -1 �Է��� ISBN�� 9�ڸ��� �ƴ� ��
	@author �ڱ���
	@since 2002-11-21 
*/
INT CISBNChecker::ReturnISBNCheckDigit( TCHAR *szInputISBN, TCHAR *szOutputISBN, TCHAR *szReturnValue)
{
	INT i, j, nCheckSum, nCheckRest, nISBNData, nDash;
	TCHAR szTemp[3], szISBN[256];

	nCheckSum = 0;
	ZeroMemory(szISBN, sizeof(TCHAR)*256);
	ZeroMemory(szTemp, sizeof(TCHAR)*3);
	szTemp[1] = 0;
	
	_tcscpy( szISBN, szInputISBN );
	
	CKeyword::getKeyword( szISBN, szISBN, TRUE );
	
	// szISBN[9] = 0;
	// if( _tcsclen( szISBN ) != 9 )		return -1;

	switch( _tcsclen(szISBN) )
	{
	case 9:
		{
			// Create ISBN CheckDigit
			for( i=0, j=10; i<9; i++, j-- )
			{
				szTemp[0] = szISBN[i];
				nCheckSum += ( _ttoi( szTemp ) * j );
			}

			nCheckRest = nCheckSum % 11;

			switch( nCheckRest )
			{
				case 0 :
					_stprintf( szReturnValue, _T("%d"), nCheckRest );
					break;
				case 1 :
					_stprintf( szReturnValue, _T("X") );
					break;
				default :
					_stprintf( szReturnValue, _T("%d"), 11 - nCheckRest );
					break;
			}

			wmemcpy( szTemp, &szISBN[2], 2 );
			szTemp[2] = 0;
			nISBNData = _ttoi( szTemp );
			
			if ( 0  <= nISBNData && nISBNData <= 24 ) nDash = 2;
			if ( 25 <= nISBNData && nISBNData <= 54 ) nDash = 3;
			if ( 55 <= nISBNData && nISBNData <= 84 ) nDash = 4;
			if ( 85 <= nISBNData && nISBNData <= 94 ) nDash = 5;
			if ( 95 <= nISBNData && nISBNData <= 99 ) nDash = 6;

			wmemcpy( szOutputISBN, szISBN + 2, nDash );
			wmemcpy( szOutputISBN + nDash + 1, szISBN + nDash + 2 , 9 - nDash );

			szOutputISBN[nDash] = '-';
			szOutputISBN[8] = 0;
		}
		break;
	//++ 2006.11.27 - ISBN ������ (13�ڸ��� ����)
	case 12:
		{
			// Create ISBN CheckDigit
			nCheckSum = 0;
			for( i=0; i<12; i++ )
			{
				szTemp[0] = szISBN[i];
				if( i%2 )	nCheckSum += _ttoi(szTemp) * 3;
				else		nCheckSum += _ttoi(szTemp) * 1;
			}

			nCheckRest = nCheckSum%10;
			if( nCheckRest != 0 )
			{
				_stprintf( szReturnValue, _T("%d"), (10-nCheckRest) );				
			}
			else
			{
				_stprintf( szReturnValue, _T("%d"), nCheckRest );
			}

			_tcsnccpy( szTemp, &szISBN[5], 2 );
			szTemp[2] = 0;
			nISBNData = _ttoi( szTemp );
			
			if ( 0  <= nISBNData && nISBNData <= 24 ) nDash = 2;
			if ( 25 <= nISBNData && nISBNData <= 54 ) nDash = 3;
			if ( 55 <= nISBNData && nISBNData <= 84 ) nDash = 4;
			if ( 85 <= nISBNData && nISBNData <= 94 ) nDash = 5;
			if ( 95 <= nISBNData && nISBNData <= 99 ) nDash = 6;

			wmemcpy( szOutputISBN, szISBN + 5, nDash );
			wmemcpy( szOutputISBN + nDash + 1, szISBN + 5 + nDash , 12 - nDash );

			szOutputISBN[nDash] = '-';
			szOutputISBN[8] = 0;
		}
		break;
	//-- 2006.11.27 - ISBN ������ (13�ڸ��� ����)

	default:
		return -1;
	}

	return 0;
}

/** ISBN���� Ư������ ('-')�� �����Ѵ�.
	@param szSource �Է�ISBN
	@param szDest	���ISBN
*/
VOID CISBNChecker::RemoveDashFromISBN(TCHAR* szSource, TCHAR* szDest )
{
    BOOL b = FALSE;
	
	for ( ; *szSource; szSource++ ) {
        if ( *szSource == '(' ) b = TRUE;
		
        if ( b ||  // ��ȣ�����̰ų�
			( *szSource >= '0' && *szSource <= '9' ) ||
			( *szSource >= 'A' && *szSource <= 'Z' ) ||
			( *szSource >= 'a' && *szSource <= 'z' )   ) {
            *szDest++ = *szSource;
        }
	}
    *szDest = 0;
}

