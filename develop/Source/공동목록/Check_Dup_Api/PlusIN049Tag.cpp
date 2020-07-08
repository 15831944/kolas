// PlusIN049Tag.cpp: implementation of the CPlusIN049Tag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\include\�������\PlusIN049Tag.h"
#include "ESL_Mgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
	
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlusIN049Tag::CPlusIN049Tag()
{

}

CPlusIN049Tag::~CPlusIN049Tag()
{

}

//--------------------------------------------------------------------------
// ���ڿ� '+' �� �ִ��� Ȯ���Ѵ�.
//--------------------------------------------------------------------------
BOOL CPlusIN049Tag::CheckPlusExist ( TCHAR * fi_pOneBookInfo )
{
    TCHAR * ptr;

    if ( fi_pOneBookInfo == NULL )
        return FALSE ;

    for ( ptr = fi_pOneBookInfo ; *ptr && *ptr != '+' ; ptr++ )
        ;

    if ( *ptr == '+' )
        return TRUE ;

    return FALSE ;
}

//##########################################################################
//                           ���� Ȯ�� ��    
//##########################################################################

//--------------------------------------------------------------------------
// ��Ϲ�ȣ '+' ���� Ȯ��  ( ���� Ȯ�� �� )
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Check_AccessionPlus ( TCHAR * fi_pPreBookInfo  ,
									     TCHAR * fi_pNextBookInfo ,
									     TCHAR * fo_pErrorMsg     )
{
	TCHAR AccessionNo[2][64] , VolCode[2][64] , CopyCode[2][64];
	TCHAR sTemp[128];
	TCHAR SubFieldCode[8] ;
	TCHAR *ptr ;

	INT  Plus_Type , ExpectAccessionNo_Cnt , i ;
	BOOL BraceHypen_Flag ;

	//--------------------------------------------------------------------------
	// ��Ϲ�ȣ , ��å��ȣ , ������ȣ�� �� ���ۿ� �����Ѵ�.
	//--------------------------------------------------------------------------
	for ( i=0; i<2 ;i++)
	{
		if ( i == 0 ) ptr = fi_pPreBookInfo  ;
		else          ptr = fi_pNextBookInfo ;

		// ��Ϲ�ȣ
	    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'l' );
		CConverTool->Get_SubFieldData ( ptr             ,
		                                SubFieldCode    ,
										AccessionNo[i]  );
		// ��å��ȣ
	    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'v' );
		CConverTool->Get_SubFieldData ( ptr             ,
										SubFieldCode    ,
										VolCode[i]      );

		// ������ȣ
	    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'c' );
		CConverTool->Get_SubFieldData ( ptr             ,
										SubFieldCode    ,
										CopyCode[i]      );
	}

	if ( AccessionNo[0][0] == '\0' || AccessionNo[1][0] == '\0' )
	{
		_stprintf ( sTemp , _T("'+' ��Ϲ�ȣ�� �����ϴ� [%s] [%s] ") , AccessionNo[0] , AccessionNo[1] );
		CConverTool->Save_ErrorMsg ( fo_pErrorMsg , sTemp );
		return -1;
	}

	if ( _tcsncmp ( AccessionNo[0] , AccessionNo[1] , 2 ) )
	{
		_stprintf ( sTemp , _T("'+' ��ϱ����� �ٸ��ϴ�. [%.2s]:[%.2s]") , AccessionNo[0] , AccessionNo[1] );
		CConverTool->Save_ErrorMsg ( fo_pErrorMsg , sTemp );
		return -1;
	}

	if ( CConverTool->IntegerCheck ( AccessionNo[0]+2 ) || CConverTool->IntegerCheck( AccessionNo[1] + 2 ) )
	{
		_stprintf ( sTemp , _T("'+' ��Ϲ�ȣ�� �Ϸù�ȣ ���� [%s] [%s] ") , AccessionNo[0] , AccessionNo[1] );
		CConverTool->Save_ErrorMsg ( fo_pErrorMsg , sTemp );
		return -1 ;
	}

	//----------------------------------------------------------------------------
	//  ������ȣ , ��å��ȣ ��� ����.
	//----------------------------------------------------------------------------
	BraceHypen_Flag = FALSE ;

	Plus_Type = Exist_ValueCheck ( VolCode[0]  , VolCode[1]  ,  
		                           CopyCode[0] , CopyCode[1] ,
								   &BraceHypen_Flag         );
	switch ( Plus_Type )
	{

		//-------------------------------------------------------------------------------
		// ��Ϲ�ȣ�� �ִ�.
		//-------------------------------------------------------------------------------
		case 1 :
				if ( Expect_OnlyAccessionNo ( AccessionNo[0] ,
					                          AccessionNo[1] ,
											  &ExpectAccessionNo_Cnt ) )
					goto EBOT;
				break ;

		//-------------------------------------------------------------------------------
		// ��å��ȣ�� �ִ�.
		//-------------------------------------------------------------------------------
		case 2 :
			    // ����Ǵ� ��Ϲ�ȣ�� ���� ���Ѵ�.
			    if ( Expect_OnlyVolCode_AccessionNo_Cnt ( VolCode[0]             ,
					                                      VolCode[1]             ,
					                                      BraceHypen_Flag        ,
						  								  &ExpectAccessionNo_Cnt ) )
					goto EBOT;

				// ����Ǵ� ��Ϲ�ȣ ���� �� ��Ϲ�ȣ�� Ȯ���Ѵ�.
				if ( Check_AccessionNoCnt ( AccessionNo[0] , 
					                        AccessionNo[1] , 
										    ExpectAccessionNo_Cnt ) )
					goto EBOT;
				break;

		//-------------------------------------------------------------------------------
		// ������ȣ�� �ִ�. �Ǵ�  ��å��ȣ�� ���� ������ȣ�� ������
		//-------------------------------------------------------------------------------
		case 3 :	
			    if ( Check_OnlyCopyCode ( CopyCode[0] , CopyCode[1] ,
					                      &ExpectAccessionNo_Cnt ) )
					goto EBOT;

				if ( Check_AccessionNoCnt ( AccessionNo[0] , 
					                        AccessionNo[1] , 
									  	    ExpectAccessionNo_Cnt ) )
					goto EBOT;
				break;

		//-------------------------------------------------------------------------------
		// �Ѵ� ��å��ȣ�� �ְ� ������ȣ�� �ڿ��� �ִ� ���
		//-------------------------------------------------------------------------------
		case 4 :
			    if ( Check_AddVolCode_CopyCode2thExist( VolCode[0]             ,
					                                    VolCode[1]             ,
												        CopyCode[1]            ,
			                                            BraceHypen_Flag        ,
					                                    &ExpectAccessionNo_Cnt ) )
					goto EBOT;

			    if ( Check_AccessionNoCnt ( AccessionNo[0] ,
					                        AccessionNo[1] , 
									        ExpectAccessionNo_Cnt-1 ) )
					goto EBOT;
				
				break;

		//-------------------------------------------------------------------------------
		// ������ȣ�� ���� ��å��ȣ�� ���� ���� ���
		//-------------------------------------------------------------------------------
		case 5 :
			    if ( Check_SameCopyCodeAddVolCode( VolCode[0]             ,
					                               VolCode[1]             ,
												   CopyCode[0]            ,
			                                       BraceHypen_Flag        ,
					                               &ExpectAccessionNo_Cnt ) )
					/*goto EBOTessionNo_Cnt ) )
					goto EBOTNoCnt ( AccessionNo[0] ,
					                        AccessionNo[1] ,
									        ExpectAccessionNo_Cnt ) )*/
					goto EBOT;

				ExpectAccessionNo_Cnt--;

				break;
		default :
				_stprintf ( sTemp , _T("ó�� �Ұ����� '+' �����Դϴ�. [%s] [%s] ") , fi_pPreBookInfo , fi_pNextBookInfo );
				CConverTool->Save_ErrorMsg ( fo_pErrorMsg , sTemp );
				return -1;
	}

	return 0;

EBOT:
	_stprintf ( sTemp , _T("'+'�� ���Ե� ��Ϲ�ȣ ó���� �����߻� [%s] [%s] ") , fi_pPreBookInfo , fi_pNextBookInfo );
	CConverTool->Save_ErrorMsg ( fo_pErrorMsg , sTemp );
	return -1;

}

//----------------------------------------------------------------------------
//  '+' �� ������ ���Ѵ�.
//----------------------------------------------------------------------------
INT CPlusIN049Tag::Exist_ValueCheck ( TCHAR * VolCode1  , TCHAR * VolCode2  , 
								      TCHAR * CopyCode1 , TCHAR * CopyCode2 ,
								      INT  * BraceHypen_Flag              )
{
	INT  i ,j ;
	TCHAR *ptr ;
	INT  BraceHypen1_Flag , BraceHypen2_Flag ;

	INT  Accession_Plus_Type ;

	//------------------------------------------------------------------------
	// ��Ϲ�ȣ�� �ִ�.  [ ���� ] 
	//------------------------------------------------------------------------
	if ( VolCode1[0] == '\0'  && VolCode2[0]  == '\0' &&
		 CopyCode1[0] == '\0' && CopyCode2[0] == '\0' )
		 return 1;

	// �� �߿� �ϳ��� �ִ� ��å ��ȣ => ����
	if ( ( VolCode1[0] == '\0' && VolCode2[0] != '\0' ) ||
		 ( VolCode1[0] != '\0' && VolCode2[0] == '\0' ) )
		 return -1;

	// �տ��� ������ȣ�� �ְ� �ڿ��� ����. => ����
	if ( CopyCode1[0] != '\0' && CopyCode2[0] == '\0' )
		return -1;

	/* ��å��ȣ ���� ������ȣ�� 1 ���� ���� ��쿡�� �̷� ��찡 �߻�
	// ��å��ȣ ���� ���¿��� ������ȣ�� �ڿ��� �ִ� ���
	if ( VolCode1[0]  == '\0' && VolCode2[0] == '\0' && 
		 CopyCode1[0] == '\0' && CopyCode2[0] != '\0' )
		 return -1;
	 */

	// ������ȣ �� Ȯ�� ( �����̿��� �� => ���� )
	if ( CopyCode1[0] != '\0' || CopyCode2[0] != '\0' )
	{
		for ( i=0 ; i < 2 ; i++ )
		{
			if ( i== 0 ) ptr = CopyCode1 ;
			else         ptr = CopyCode2 ;

			if ( *ptr == '\0' ) continue ;

			if ( *ptr == '0' && *(ptr+1) == '\0' )
				return -1;

			for ( ; *ptr ; ptr++ )
			{
				if ( ! (*ptr >= '0' && *ptr <= '9' ) )
					return -1;
			}
		}
	}

	//=================================================================

	Accession_Plus_Type = 0 ;
	//------------------------------------------------------------------------
	// ��å��ȣ�� �ִ�.									[ ���� ]
	// [ ��å��ȣ�� Ȯ���ؾ� �Ѵ�. ]
	//------------------------------------------------------------------------
	if ( CopyCode1[0] == '\0' && CopyCode2[0] == '\0' &&
		 VolCode1[0]  != '\0' && VolCode2[0]  != '\0' )
		 Accession_Plus_Type = 2;

	//------------------------------------------------------------------------
	// ������ȣ�� �ִ�.									[ ���� ]
	//------------------------------------------------------------------------
	if ( VolCode1[0]  == '\0' && VolCode2[0]  == '\0' &&
		 ( ( CopyCode1[0] != '\0' && CopyCode2[0] != '\0') ||
		   ( CopyCode1[0] == '\0' && CopyCode2[0] != '\0') ) )
		 return 3;

	//------------------------------------------------------------------------
	// ��å��ȣ�� ���� ������ȣ�� �ٸ���.				[ ���� ]
	//------------------------------------------------------------------------
	if ( !_tcscmp ( VolCode1  , VolCode2  ) &&
		  _tcscmp ( CopyCode1 , CopyCode2 )  )
         return 3;

	//------------------------------------------------------------------------
	// ��å��ȣ�� �����ǰ� ������ȣ�� �ڿ��� �ִ� ���	[ ���� ]
	// [ ��å��ȣ�� Ȯ���ؾ� �Ѵ�. ]
	//------------------------------------------------------------------------
	if ( _tcscmp ( VolCode1   , VolCode2  ) &&
		 CopyCode1[0] == '\0' && CopyCode2[0] != '\0' )
		 Accession_Plus_Type = 4;

	//------------------------------------------------------------------------
	// ������ȣ�� ���� ��å��ȣ�� ������				[ ���� ]
	// [ ��å��ȣ�� Ȯ���ؾ� �Ѵ�. ]
	//------------------------------------------------------------------------
	if (  _tcscmp ( VolCode1  , VolCode2  ) &&
		  CopyCode1[0] != '\0' &&
		  CopyCode2[0] != '\0' &&
		 !_tcscmp ( CopyCode1 , CopyCode2 )  )
         Accession_Plus_Type = 5;

	//------------------------------------------------------------------------
	// ó�������� �������� ���.
	//------------------------------------------------------------------------
	if ( Accession_Plus_Type == 0 )
		return 0;

	//------------------------------------------------------------------------
	// ��å��ȣ�� Ȯ���Ѵ�.
	//------------------------------------------------------------------------

	// ��å��ȣ �� Ȯ�� ( ���� ) , ���� ( ) -
	BraceHypen1_Flag = BraceHypen2_Flag = 0 ;
	*BraceHypen_Flag = 0 ;
	if ( VolCode1[0] != '\0' )
	{
		for ( i=0 ; i < 2 ; i++ )
		{
			if ( i== 0 ) ptr = VolCode1 ;
			else         ptr = VolCode2 ;

			for (j=0 ; ptr[j] ; j++ )
			{
				if ( ! (  ( ptr[j] >= '0' && ptr[j] <= '9' ) ||
					      ( ptr[j] >= 'a' && ptr[j] <= 'z' ) ||
						  ( ptr[j] >= 'A' && ptr[j] <= 'Z' ) ||
						  ( ptr[j] == '(' || ptr[j] == ')' ) ||
						  ( ptr[j] == '-'                )  ) )
					return -1;

				if ( ptr[j] == '(' || ptr[j] == ')' )
				{
					if ( i==0 ) BraceHypen1_Flag = 1 ;
					else        BraceHypen2_Flag = 1 ;
				}
				if ( ptr[j] == '-' )
				{
					if ( i==0 ) BraceHypen1_Flag = 2 ;
					else        BraceHypen2_Flag = 2 ;
				}
			}
		}

		if ( BraceHypen1_Flag != BraceHypen2_Flag )
			return -1 ;

		if ( BraceHypen1_Flag != 0 &&
			 BraceHypen1_Flag == BraceHypen2_Flag  )
    		*BraceHypen_Flag = BraceHypen1_Flag ;

		if ( *BraceHypen_Flag == 0 )
		{
   			// ������ �� ����Ʈ��� �Ѵ�.
			if ( ! ( VolCode1[0] >= '0' && VolCode1[0] <= '9' ) )
			{
				if ( ( VolCode1[0] >= 'a' && VolCode1[0] <= 'z' || 
					   VolCode1[0] >= 'A' && VolCode1[0] <= 'Z' ) && VolCode1[1] == '\0' &&
					 ( VolCode2[0] >= 'a' && VolCode2[0] <= 'z' ||
					   VolCode2[0] >= 'A' && VolCode2[0] <= 'Z' ) && VolCode2[1] == '\0'
				   )	 
				   ;
				else
					return -1;
			}
		}
	}

	return Accession_Plus_Type ;
}

//-------------------------------------------------------------------------------
// ��Ϲ�ȣ�� �ִ� ���
//-------------------------------------------------------------------------------
INT CPlusIN049Tag::Expect_OnlyAccessionNo ( TCHAR * fi_pAccessionNo1          ,
							                TCHAR * fi_pAccessionNo2          ,
										    INT  * fo_pExpectAccessionNo_Cnt )
{

	UINT AccessionNo_SerialNo1 , AccessionNo_SerialNo2 ;

	AccessionNo_SerialNo1 = _ttol ( fi_pAccessionNo1 + 2 );
	AccessionNo_SerialNo2 = _ttol ( fi_pAccessionNo2 + 2 );

	* fo_pExpectAccessionNo_Cnt = (INT)( AccessionNo_SerialNo2 - AccessionNo_SerialNo1 ) ;
	if ( * fo_pExpectAccessionNo_Cnt <= 0 || * fo_pExpectAccessionNo_Cnt > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	return 0;
}

//-------------------------------------------------------------------------------
// ��å��ȣ�� ���� ��� ����Ǵ� ��Ϲ�ȣ ���� ���Ѵ�.
//-------------------------------------------------------------------------------
INT CPlusIN049Tag::Expect_OnlyVolCode_AccessionNo_Cnt ( TCHAR * fi_pVolCode1              ,
													    TCHAR * fi_pVolCode2              ,
										                BOOL   fi_BraceHypen_Flag        ,
	   												    INT  * fo_pExpectAccessionNo_Cnt ) 
{
	TCHAR VolCode1[2][64] , VolCode2[2][64] ;
	TCHAR *ptr;
	INT  j , rtn ;

	* fo_pExpectAccessionNo_Cnt = -1;

	if ( fi_BraceHypen_Flag == FALSE )
	{
		rtn = Check_Two_VolCode ( fi_pVolCode1              ,
			 		              fi_pVolCode2              ,
						          fo_pExpectAccessionNo_Cnt );
		if ( rtn < 0 || *fo_pExpectAccessionNo_Cnt <= 0 || *fo_pExpectAccessionNo_Cnt > MAX_PLUS_ACCESSION_NO_CNT )
			return -1;

		return 0;
	}

	//TCHAR VolCode1[2][64] , VolCode2[2][64] ;
	// Brace , '-' �� �ִ� ����

	for ( ptr = fi_pVolCode1 , j= 0; *ptr && *ptr != '(' ; ptr++ , j++ )
		VolCode1[0][j] = *ptr ;
	VolCode1[0][j] = '\0';

	if ( *ptr == '(' )
	{
		for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
			VolCode1[1][j] = *ptr;
		if ( *(ptr-1) != ')' )
			return -1;
		VolCode1[1][j-1] = '\0';

		for ( ptr = fi_pVolCode2 , j= 0; *ptr && *ptr != '(' ; ptr++ , j++ )
			VolCode2[0][j] = *ptr ;
		VolCode2[0][j] = '\0';
		if ( *ptr == '\0' )
			return -1;

		for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
			VolCode2[1][j] = *ptr;
		if ( *(ptr-1) != ')' )
			return -1;
		VolCode2[1][j-1] = '\0';
	}
	else
	{
		//-------------------------------------------------------------------------------
		// ���� ����
		//-------------------------------------------------------------------------------

		// '-' �̴�.
		//-------------------------------
		for ( ptr = fi_pVolCode1 , j=0 ; *ptr && *ptr != '-' ; 
		      ptr++ , j++ )
			VolCode1[0][j] = *ptr ;
		if ( *ptr == '\0' )	return -1;
		VolCode1[0][j] = '\0';

		for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
			VolCode1[1][j] = *ptr ;
		VolCode1[1][j] = '\0';

		//-------------------------------
		for ( ptr = fi_pVolCode2, j=0 ; *ptr && *ptr != '-' ; ptr++ , j++ )
			VolCode2[0][j] = *ptr ;
		if ( *ptr == '\0' )	return -1;
		VolCode2[0][j] = '\0';

		for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
			VolCode2[1][j] = *ptr ;
		VolCode2[1][j] = '\0';
	}

	//-------------------------------------------------------------------------------
	// �� ���� �ϳ��� ���� ���ƾ� �Ѵ�.
	//-------------------------------------------------------------------------------
    if ( !_tcscmp ( VolCode1[0] , VolCode2[0] ) &&
          _tcscmp ( VolCode1[1] , VolCode2[1] ) )
	{
		rtn = Check_Two_VolCode ( VolCode1[1]               ,
			                      VolCode2[1]               ,
								  fo_pExpectAccessionNo_Cnt );
	}	
	else if (  _tcscmp ( VolCode1[0] , VolCode2[0] ) && 
              !_tcscmp ( VolCode1[1] , VolCode2[1] ) )
	{
		rtn = Check_Two_VolCode ( VolCode1[0]               , 
			                      VolCode2[0]               ,
								  fo_pExpectAccessionNo_Cnt );
	}
	else
		return -1;

	if ( rtn < 0 || *fo_pExpectAccessionNo_Cnt <= 0 || *fo_pExpectAccessionNo_Cnt > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	return 0 ;
}

//--------------------------------------------------------------------------
// ��Ϲ�ȣ ���̿� ����Ǵ� ��Ϲ�ȣ�� ������ ���Ѵ�.
//--------------------------------------------------------------------------
INT  CPlusIN049Tag::Check_AccessionNoCnt ( TCHAR * AccessionNo1          ,
								           TCHAR * AccessionNo2          ,
										   INT    ExpectAccessionNo_Cnt )
{
	UINT AccessionNo_SerialNo1 , AccessionNo_SerialNo2 ;

	// ����Ǵ� ��Ϲ�ȣ�� �� Ȯ��
	if ( ExpectAccessionNo_Cnt <= 0 )
		return -1;

	AccessionNo_SerialNo1 = _ttol ( AccessionNo1 + 2 );
	AccessionNo_SerialNo2 = _ttol ( AccessionNo2 + 2 );

	// �ϳ��� '+' �� ����� �� �ִ� ��Ϲ�ȣ�� �ʰ�
	if ( ExpectAccessionNo_Cnt                          > MAX_PLUS_ACCESSION_NO_CNT ||
		 AccessionNo_SerialNo2 - AccessionNo_SerialNo1  > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	if ( ExpectAccessionNo_Cnt == (INT)( AccessionNo_SerialNo2 - AccessionNo_SerialNo1 ) )
		return 0;

	return -1;
}

//--------------------------------------------------------------------------
//  ��å��ȣ ���� ���� �� Ȯ���� �Ѵ�.
//  - ���� ���� �� ���°� �����̰ų� ���� �̿��� �Ѵ�.
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Check_Two_VolCode ( TCHAR * fi_pVolCode1 ,
				   			           TCHAR * fi_pVolCode2 ,
									   INT  * Subtract     )
{
	INT rtn1, rtn2;

	rtn1 = Check_VolCode_Eng_Integer ( fi_pVolCode1 );
    rtn2 = Check_VolCode_Eng_Integer ( fi_pVolCode2 );

	if ( rtn1 < 0 || rtn2 < 0 )
		return -1;

	if ( rtn1 == rtn2 )
	{
		if ( rtn1 == 1 ) // ����
			*Subtract = _ttoi(fi_pVolCode2) - _ttoi(fi_pVolCode1) ;
		else if ( rtn1 == 2 ) // ����
			*Subtract = fi_pVolCode2[0] - fi_pVolCode1[0] ;

		if ( *Subtract <= 0 )
			return -1;

		return rtn1;
	}

	return -1;
}

//--------------------------------------------------------------------------
// ��å��ȣ�� �� Ȯ�� 
// �����̰ų� ���� �� Byte
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Check_VolCode_Eng_Integer ( TCHAR *fi_pStr )
{
	INT i;

	// ���� 
	for ( i=0 ; fi_pStr[i] ; i++ )
	{
		if ( fi_pStr[i] >='0' && fi_pStr[i] <= '9' )
			;
		else
			break;
	}

	if ( fi_pStr[i] == '\0' )
		return 1 ;

	// ���� 1 byte

	if ( ( ( fi_pStr[0] >= 'a' && fi_pStr[0] <= 'z') ||
		   ( fi_pStr[0] >= 'A' && fi_pStr[0] <= 'Z') )&&
		 fi_pStr[i+1] == '\0' )
		 return 2;

	return -1;
}

//--------------------------------------------------------------------------
// ������ȣ�� ���� ��쿡 ���� ��Ϲ�ȣ ������ ���Ѵ�.
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Check_OnlyCopyCode ( TCHAR * fi_pCopyCode1            ,
									    TCHAR * fi_pCopyCode2            ,
									    INT  * fo_pExpectAccessionNo_Cnt ) 
{
	INT First_CopyCode ;

	First_CopyCode = _ttoi( fi_pCopyCode1 );
	if ( First_CopyCode == 0 ) 
		First_CopyCode = 1;
	
	* fo_pExpectAccessionNo_Cnt = _ttoi(fi_pCopyCode2) - First_CopyCode ;
	if ( * fo_pExpectAccessionNo_Cnt <= 0 || * fo_pExpectAccessionNo_Cnt > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	return 0;
}

//--------------------------------------------------------------------------
//   ������ȣ�� ��å��ȣ�� ���� ���
// - ������ȣ�� ���� ��å��ȣ�� ����
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Check_SameCopyCodeAddVolCode( TCHAR * fi_pVolCode1               ,
							                     TCHAR * fi_pVolCode2               ,
											     TCHAR * fi_pCopyCode               ,
						                         BOOL   fi_BraceHypen_Flag         ,
								                 INT  * fo_pExpectAccessionNo_Cnt  )
{
	INT rtn;
	INT Expect_VolAccessionNoCnt ;

	//------------------------------------------------------------------------
	// ��å��ȣ ���̿� ���� ��Ϲ�ȣ ���� �� 
	//------------------------------------------------------------------------
	rtn = Expect_OnlyVolCode_AccessionNo_Cnt ( fi_pVolCode1              ,
		                                       fi_pVolCode2              ,
											   fi_BraceHypen_Flag        ,
										       &Expect_VolAccessionNoCnt ) ;
	if (rtn)
		return -1;

	//------------------------------------------------------------------------
	// ���� ��Ϲ�ȣ ��
	//------------------------------------------------------------------------
	* fo_pExpectAccessionNo_Cnt = Expect_VolAccessionNoCnt * _ttoi(fi_pCopyCode);

	if ( * fo_pExpectAccessionNo_Cnt <= 0 || * fo_pExpectAccessionNo_Cnt > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	return 0;
}

//--------------------------------------------------------------------------
//   ������ȣ�� ��å��ȣ�� ���� ���
// - ��å��ȣ�� �����ǰ� ������ȣ�� �ڿ��� �ִ� ���
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Check_AddVolCode_CopyCode2thExist(TCHAR * fi_pVolCode1               ,
							                         TCHAR * fi_pVolCode2               ,
											         TCHAR * fi_pCopyCode               ,
											   	     BOOL   fi_BraceHypen_Flag         ,
												     INT  * fo_pExpectAccessionNo_Cnt  ) 
{
	INT rtn;
	INT Expect_VolAccessionNoCnt ;

	//------------------------------------------------------------------------
	// ��å��ȣ ���̿� ���� ��Ϲ�ȣ ���� �� 
	//------------------------------------------------------------------------
	rtn = Expect_OnlyVolCode_AccessionNo_Cnt ( fi_pVolCode1              ,
		                                       fi_pVolCode2              ,
											   fi_BraceHypen_Flag        ,
										       &Expect_VolAccessionNoCnt ) ;
	if (rtn)
		return -1;

	//------------------------------------------------------------------------
	// ���� ��Ϲ�ȣ ��
	//------------------------------------------------------------------------
	* fo_pExpectAccessionNo_Cnt = ( Expect_VolAccessionNoCnt + 1 )* _ttoi(fi_pCopyCode);

	if ( * fo_pExpectAccessionNo_Cnt <= 0 || * fo_pExpectAccessionNo_Cnt > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	return 0;
}

//##########################################################################
//               '+' ���� ��Ϲ�ȣ , ��å��ȣ , ���� ��ȣ ���� 
//##########################################################################

//===============================================================================
//  '+' ��ȣ�� �ִ� ��Ϲ�ȣ ó�� 
//===============================================================================
INT CPlusIN049Tag::Save_Plus_BookInfo  (  TCHAR * fi_pPreBookInfo  ,
									      TCHAR * fi_pNextBookInfo ,
 										  TCHAR * fo_pAccessionNo  , 
										  TCHAR * fo_pVolCode      , 
										  TCHAR * fo_pCopyCode     , 
                                          TCHAR * fo_pShelfCode    )
{
	TCHAR AccessionNo[2][64] , VolCode[2][64] , CopyCode[2][64];
	TCHAR ShelfCode[2][16];
	TCHAR SubFieldCode[8] ;
	TCHAR *ptr ;

	INT  Plus_Type , i ;
	INT  BraceHypen_Flag ;

	TCHAR SameVolCode[256] , AddVolStart[256] , AddVolEnd[256];
	INT  AddLocate ;

	//--------------------------------------------------------------------------
	// ��Ϲ�ȣ , ��å��ȣ , ������ȣ�� �� ���ۿ� �����Ѵ�.
	//--------------------------------------------------------------------------
	for ( i=0; i<2 ;i++)
	{
		if ( i == 0 ) ptr = fi_pPreBookInfo  ;
		else          ptr = fi_pNextBookInfo ;

		// ��Ϲ�ȣ
	    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'l' );
		CConverTool->Get_SubFieldData ( ptr             ,
		                                SubFieldCode    ,
										AccessionNo[i]    );
		// ��å��ȣ
	    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'v' );
		CConverTool->Get_SubFieldData ( ptr             ,
										SubFieldCode    ,
										VolCode[i]      );

		// ������ȣ
	    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'c' );
		CConverTool->Get_SubFieldData ( ptr             ,
										SubFieldCode    ,
										CopyCode[i]     );

		_stprintf ( SubFieldCode , _T("%c%c") , GT , 'f' );
		CConverTool->Get_SubFieldData ( ptr             ,
										SubFieldCode    ,
									    ShelfCode[i]   );
	}

	//----------------------------------------------------------------------------
	//  ������ȣ , ��å��ȣ ��� ����.
	//  BraceHypen_Flag == 1 : ( )   == 2 : - 
	//----------------------------------------------------------------------------
	BraceHypen_Flag = FALSE ;

	Plus_Type = Exist_ValueCheck ( VolCode[0]  , VolCode[1]  ,
		                           CopyCode[0] , CopyCode[1] ,
								   &BraceHypen_Flag         );
	switch ( Plus_Type )
	{
		//-------------------------------------------------------------------------------
		// ��Ϲ�ȣ�� �ִ�.
		//-------------------------------------------------------------------------------
		case 1 :
				if ( Save_OnlyAccessionNo ( AccessionNo[0]     ,
					                        AccessionNo[1]     ,
											fo_pAccessionNo    ,
											fo_pVolCode        ,
											fo_pCopyCode       ) )
					return -1;

				break;

		//-------------------------------------------------------------------------------
		// ��å��ȣ�� �ִ�.
		//-------------------------------------------------------------------------------
		case 2 :
				//-----------------------------------------------------------------------
				// �и���ȣ�� ���� ���
				//-----------------------------------------------------------------------
			    if ( BraceHypen_Flag == 0 )
				{
					if ( Save_OnlyVolCode_NoBraceHypen ( VolCode[0]       ,
						                                 VolCode[1]       ,
														 AccessionNo[0]   ,
														 AccessionNo[1]   ,
 								                         fo_pAccessionNo  , 
		                                                 fo_pVolCode      , 
                                                         fo_pCopyCode     ) )
						return -1;
					break ;
				}

				//-----------------------------------------------------------------------
				// �и���ȣ�� ���� ���
				//-----------------------------------------------------------------------

				//-----------------------------------------------------------------------
				// ��å��ȣ�� �м��Ͽ� ������ ���� ��ġ , �� , ���� ���� ���Ѵ�.
				//-----------------------------------------------------------------------
				if ( ExistBraceHypen_Separation_VolCode ( VolCode[0]      ,
										                  VolCode[1]      ,
										                  BraceHypen_Flag ,
										                  SameVolCode     ,
										                  &AddLocate      ,
										                  AddVolStart     ,
										                  AddVolEnd       ) )
					return -1;

				//-----------------------------------------------------------------------
				// ��Ϲ�ȣ , ��å��ȣ , ������ȣ�� ���Ѵ�.
				//-----------------------------------------------------------------------
				if ( Save_OnlyVolCode_ExistBraceHypen ( AccessionNo[0]  ,
					                                    AccessionNo[1]  ,
					                                    BraceHypen_Flag , 
														SameVolCode     ,
														AddLocate       ,
														AddVolStart     ,
														AddVolEnd       ,
 								                        fo_pAccessionNo , 
		                                                fo_pVolCode     , 
                                                        fo_pCopyCode    ) )
					return -1;
				break;

		//-------------------------------------------------------------------------------
		// ������ȣ�� �ִ�. �Ǵ�  ��å��ȣ�� ���� ������ȣ�� ������
		//-------------------------------------------------------------------------------
		case 3 :	
			    if ( Save_OnlyCopyCode ( AccessionNo[0]   ,
					                     AccessionNo[1]   ,
					                     VolCode[0]       ,
					                     CopyCode[0]      ,
					                     CopyCode[1]      ,
		 	                             fo_pAccessionNo  ,
                                         fo_pVolCode      , 
                                         fo_pCopyCode     ) )
					return -1;

				break;

		//-------------------------------------------------------------------------------
		// ��å��ȣ�� �����ǰ� ������ȣ�� �ι�°���� �ִ�.
		//-------------------------------------------------------------------------------
		case 4 :
			    if ( BraceHypen_Flag == 0 )
				{
					if ( Save_AddVolCode2thCopyCodeExist_NoBraceHypen ( VolCode[0]       ,
						                                                VolCode[1]       ,
															      		CopyCode[1]      ,
														                AccessionNo[0]   ,
														                AccessionNo[1]   ,
 								                                        fo_pAccessionNo  , 
		                                                                fo_pVolCode      , 
                                                                        fo_pCopyCode     ) )
						return -1;
					break ;
				}

				//-----------------------------------------------------------------------
				// ��å��ȣ�� �м��Ͽ� ������ ���� ��ġ , �� , ���� ���� ���Ѵ�.
				//-----------------------------------------------------------------------
				if ( ExistBraceHypen_Separation_VolCode ( VolCode[0]      ,
										                  VolCode[1]      ,
										                  BraceHypen_Flag ,
										                  SameVolCode     ,
										                  &AddLocate      ,
										                  AddVolStart     ,
										                  AddVolEnd       ) )
					return -1;

				//-----------------------------------------------------------------------
				// ��Ϲ�ȣ , ��å��ȣ , ������ȣ�� ���Ѵ�.
				//-----------------------------------------------------------------------
				if ( Save_AddVolCode2thCopyCodeExist_ExistBraceHypen ( AccessionNo[0]  ,
					                                                   AccessionNo[1]  ,
					                                                   BraceHypen_Flag , 
												          		       SameVolCode     ,
														               AddLocate       ,
														               AddVolStart     ,
														               AddVolEnd       ,
																       CopyCode[1]     ,
 								                                       fo_pAccessionNo , 
		                                                               fo_pVolCode     , 
                                                                       fo_pCopyCode    ) )
					return -1;
				break;

		//-------------------------------------------------------------------------------
		// ������ȣ�� ���� ��å��ȣ�� ���� ���� ���
		//-------------------------------------------------------------------------------
		case 5 :
			    if ( BraceHypen_Flag == 0 )
				{
					if ( Save_SameCopyCodeAddVolCode_NoBraceHypen ( VolCode[0]       ,
						                                            VolCode[1]       ,
																	CopyCode[0]      ,
														            AccessionNo[0]   ,
														            AccessionNo[1]   ,
 								                                    fo_pAccessionNo  , 
		                                                            fo_pVolCode      , 
                                                                    fo_pCopyCode     ) )
						return -1;
					break ;
				}

				//-----------------------------------------------------------------------
				// ��å��ȣ�� �м��Ͽ� ������ ���� ��ġ , �� , ���� ���� ���Ѵ�.
				//-----------------------------------------------------------------------
				if ( ExistBraceHypen_Separation_VolCode ( VolCode[0]      ,
										                  VolCode[1]      ,
										                  BraceHypen_Flag ,
										                  SameVolCode     ,
										                  &AddLocate      ,
										                  AddVolStart     ,
										                  AddVolEnd       ) )
					return -1;

				//-----------------------------------------------------------------------
				// ��Ϲ�ȣ , ��å��ȣ , ������ȣ�� ���Ѵ�.
				//-----------------------------------------------------------------------
				_stprintf( fo_pAccessionNo+_tcsclen(fo_pAccessionNo) , _T("%.2s%010u"),
						 AccessionNo[0]  , _ttol( AccessionNo[0] + 2 ) ) ;
				_stprintf( fo_pVolCode+_tcsclen(fo_pVolCode)   , _T("%s#") , VolCode[0] );
				_stprintf( fo_pCopyCode+_tcsclen(fo_pCopyCode) , _T("%s#") , CopyCode[0] );

				if ( Save_SameCopyCodeAddVolCode_ExistBraceHypen ( AccessionNo[0]  ,
					                                               AccessionNo[1]  ,
					                                               BraceHypen_Flag , 
												          		   SameVolCode     ,
														           AddLocate       ,
														           AddVolStart     ,
														           AddVolEnd       ,
																   CopyCode[0]     ,
 								                                   fo_pAccessionNo , 
		                                                           fo_pVolCode     , 
                                                                   fo_pCopyCode    ) )
					return -1;						
				break;
		default :
				return -1;
	}

	//----------------------------------------------------------------------------
	// �� �ǿ� ��ġ��ȣ ����
	//----------------------------------------------------------------------------
	Save_ShelfCode ( AccessionNo[0] , AccessionNo[1] , 
		             ShelfCode[0]   , ShelfCode[1]   , 
					 fo_pShelfCode );

	return 0;
}


//----------------------------------------------------------------------------
// �� �ǿ� ��ġ��ȣ ����
// ��Ģ : ù��° ��ġ��ȣ : �� ������ - 1 ��° ��Ϲ�ȣ�� ����
//        �ι�° ��ġ��ȣ : �� ��������° ��Ϲ�ȣ�� ����
//----------------------------------------------------------------------------
VOID CPlusIN049Tag:: Save_ShelfCode ( TCHAR * fi_pAccessionNo1 ,
                                     TCHAR * fi_pAccessionNo2 ,
					                 TCHAR * fi_pShelfCode1   ,
                                     TCHAR * fi_pShelfCode2   ,
					                 TCHAR * fo_pShelfCode    )
{
	INT s_nLastIndex , i ;
	INT s_nOffset = 0 ;
	TCHAR temp[16];

	s_nLastIndex = _ttoi ( fi_pAccessionNo2 + 2 ) - _ttoi ( fi_pAccessionNo1 + 2 ) ;
	s_nOffset = _tcsclen ( fo_pShelfCode );

	// 1. �� ������ - 1 �� ������  ' ' �� ä���
	for ( i = 0 ; i < s_nLastIndex - 1 ; i ++ )
	{
		s_nOffset += _stprintf ( fo_pShelfCode + s_nOffset , _T(" #") );
	}

	// 2. ù��° ��ġ��ȣ�� ���� ���� ��� �������� �ι�°�� ����
	CConverTool->str_rtrim ( fi_pShelfCode1 , temp , ' ' );
	s_nOffset += _stprintf ( fo_pShelfCode + s_nOffset , _T("%s#") ,
		                   (temp[0] != '\0') ? temp : _T(" ") );

	// 3. ������ ��ġ��ȣ�� ���� ���� ��� �������� �ι�°�� ����
	CConverTool->str_rtrim ( fi_pShelfCode2 , temp , ' ' );
	s_nOffset += _stprintf ( fo_pShelfCode + s_nOffset , _T("%s#") ,
		                   (temp[0] != '\0') ? temp : _T(" ") );
}



//----------------------------------------------------------------------------
//  OneBookInfo : $lEM9584736$v123$c2
//----------------------------------------------------------------------------
VOID CPlusIN049Tag::Save_BookInfo  (  TCHAR * fi_pOneBookInfo , 
						              TCHAR * fo_pAccessionNo , 
									  TCHAR * fo_pVolCode     , 
									  TCHAR * fo_pCopyCode    ,
                                      TCHAR * fo_pShelfCode   )
{
    TCHAR Temp[256] , SubFieldCode[8];

	//----------------------------------------------------------------------------
	//  ��Ϲ�ȣ
	//----------------------------------------------------------------------------
    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'l' );
    CConverTool->Get_SubFieldData ( fi_pOneBookInfo  ,
									SubFieldCode     ,
									Temp             );
    
    _stprintf( fo_pAccessionNo + _tcsclen( fo_pAccessionNo ) , _T("%.2s") , Temp );
    _stprintf( fo_pAccessionNo + _tcsclen( fo_pAccessionNo ) , _T("%010u") , _ttol( Temp+2 ) );

	//----------------------------------------------------------------------------
	//  ��å��ȣ
	//----------------------------------------------------------------------------
    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'v' );
    CConverTool->Get_SubFieldData ( fi_pOneBookInfo  ,
								    SubFieldCode     ,
									Temp             );

    _stprintf ( fo_pVolCode + _tcsclen( fo_pVolCode ), _T("%s#") , (Temp[0] == '\0') ? _T(" ") : Temp );

	//----------------------------------------------------------------------------
	//  ������ȣ
	//----------------------------------------------------------------------------

    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'c' );
    CConverTool->Get_SubFieldData ( fi_pOneBookInfo  ,
									SubFieldCode     ,
									Temp             );

    _stprintf ( fo_pCopyCode + _tcsclen( fo_pCopyCode ), _T("%s#") , (Temp[0] == '\0') ? _T(" ") : Temp );

	//----------------------------------------------------------------------------
	//  ��ġ��ȣ
	//----------------------------------------------------------------------------
    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'f' );
    CConverTool->Get_SubFieldData ( fi_pOneBookInfo  ,
									SubFieldCode     ,
									Temp             );
	CConverTool->str_rtrim ( Temp , Temp , ' ' );
    _stprintf ( fo_pShelfCode + _tcsclen( fo_pShelfCode ), _T("%s#") , (Temp[0] == '\0') ? _T(" ") : Temp );

    return ;
}

//-------------------------------------------------------------------------------
// ��Ϲ�ȣ�� �ִ�.
//-------------------------------------------------------------------------------
INT  CPlusIN049Tag::Save_OnlyAccessionNo ( TCHAR * fi_pAccessionNo1 , 
									       TCHAR * fi_pAccessionNo2 ,
									 	   TCHAR * fo_pAccessionNo  ,
										   TCHAR * fo_pVolCode      ,
										   TCHAR * fo_pCopyCode     )
{
	INT i;
	UINT AccessionNo ;
	UINT AccessionNo1 , AccessionNo2 ;

	AccessionNo1 = _ttol ( fi_pAccessionNo1 + 2 );
	AccessionNo2 = _ttol ( fi_pAccessionNo2 + 2 );

	if ( AccessionNo2 - AccessionNo1 <= 0  ||
		 AccessionNo2 - AccessionNo1 + 1 > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	for ( AccessionNo = AccessionNo1 , i = 0 ; AccessionNo <= AccessionNo2 ; AccessionNo++ , i++ )
	{
		// ��Ϲ�ȣ
		_stprintf ( fo_pAccessionNo + _tcsclen( fo_pAccessionNo ) , _T("%.2s%010u") , fi_pAccessionNo1 ,
				                                                         _ttol(fi_pAccessionNo1 + 2) + i );

		// ��å��ȣ
		_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode )   , _T("%c#") , ' ' );

		// ������ȣ
		_stprintf ( fo_pCopyCode + _tcsclen ( fo_pCopyCode ) , _T("%c#") , ' ' );
	}

	return 0;
}

//-------------------------------------------------------------------------------
// ��å��ȣ�� �ְ� () , - �� ���� ���¿��� ��Ϲ�ȣ, ��å��ȣ, ������ȣ
//-------------------------------------------------------------------------------
INT CPlusIN049Tag::Save_OnlyVolCode_NoBraceHypen ( TCHAR * fi_pVolCode1    ,
	 											   TCHAR * fi_pVolCode2    ,
												   TCHAR * fi_pAccessionNo1,
												   TCHAR * fi_pAccessionNo2,
 												   TCHAR * fo_pAccessionNo , 
												   TCHAR * fo_pVolCode     , 
												   TCHAR * fo_pCopyCode    )
{
	// KOL.UDF.022 ��ť���ڵ� ����
	INT rtn1 = 0, rtn2 = 0 , i = 0;
	INT Start_Vol = 0 , End_Vol = 0 , VolCode = 0;

	rtn1 = Check_VolCode_Eng_Integer ( fi_pVolCode1 );
	rtn2 = Check_VolCode_Eng_Integer ( fi_pVolCode2 );

	if ( rtn1 < 0 || rtn2 < 0 || rtn1 != rtn2 )
		return -1;

	if ( rtn1 == 1 )
	{
		Start_Vol = _ttoi ( fi_pVolCode1 );   End_Vol = _ttoi ( fi_pVolCode2 );
	}else if ( rtn1 == 2 )
	{
		Start_Vol = fi_pVolCode1[0] ;   End_Vol = fi_pVolCode2[0] ;
	}

	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
			                    End_Vol - Start_Vol ) )
		return -1;

	for ( VolCode = Start_Vol , i = 0 ; VolCode <= End_Vol ; VolCode++ , i++ )
	{
		// ��Ϲ�ȣ
		_stprintf ( fo_pAccessionNo + _tcsclen( fo_pAccessionNo ) , _T("%.2s%010u") , fi_pAccessionNo1 , 
				                                                         _ttol(fi_pAccessionNo1 + 2) + i );

		// ��å��ȣ
		if ( rtn1 == 1 ) // ����
			_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%d#") , VolCode );
		else if ( rtn1 == 2 ) // ����
			_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%c#") , VolCode );

		// ������ȣ
		_stprintf ( fo_pCopyCode + _tcsclen ( fo_pCopyCode ) , _T("%c#") , ' ' );
	}

	return 0;
}

//-------------------------------------------------------------------------------
// ��å��ȣ�� �и���ȣ ( '()' '-' ) �� ���� ��� �� ���� �м��Ͽ�
// ��å��ȣ�� ������ �� �ֵ��� �Ѵ�.
// ����
//  fi_pVolCode1     : ù ��å��ȣ
//	fi_pVolCode2     : �� ��å��ȣ
//	BraceHypen_Flag  : �и���ȣ�� ����    ( 1 : ()   2 : - )
//	fo_pSameVolCode  : ������ �ʴ� �� 
//	fo_AddLocate     : �����Ǵ� ���� ��ġ ( 1 : ��   2 ; �� )
//	fo_pAddVolStart  : �����Ǵ� ���� ���� ��
//	fo_pAddVolEnd    : �����Ǵ� ���� ��  ��
//-------------------------------------------------------------------------------
INT CPlusIN049Tag::ExistBraceHypen_Separation_VolCode ( TCHAR * fi_pVolCode1    ,
														TCHAR * fi_pVolCode2    ,
														INT	BraceHypen_Flag    ,
														TCHAR * fo_pSameVolCode ,
														INT  * fo_pAddLocate   ,
														TCHAR * fo_pAddVolStart ,
														TCHAR * fo_pAddVolEnd   )
{
	TCHAR VolCode1[2][64] , VolCode2[2][64] ;
	TCHAR *ptr;
	INT  j ;

	//------------------------------------------------------------------------
	//  �� ���� ����
	//------------------------------------------------------------------------

	switch ( BraceHypen_Flag )
	{
		case 1 :	//	'( )'

				// Prev
				for ( ptr = fi_pVolCode1 , j= 0; *ptr && *ptr != '(' ; ptr++ , j++ )
					VolCode1[0][j] = *ptr ;
				VolCode1[0][j] = '\0';
				if ( *ptr != '(' ) 
					return -1;

				for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
					VolCode1[1][j] = *ptr;
				if ( *(ptr-1) != ')' )
					return -1;
				VolCode1[1][j-1] = '\0';
		
				// Next
				for ( ptr = fi_pVolCode2 , j= 0; *ptr && *ptr != '(' ; ptr++ , j++ )
					VolCode2[0][j] = *ptr ;
				VolCode2[0][j] = '\0';
				if ( *ptr == '\0' )
					return -1;

				for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
					VolCode2[1][j] = *ptr;
				if ( *(ptr-1) != ')' )
					return -1;
				VolCode2[1][j-1] = '\0';

			break;
		case 2 :	//  ' - '

				for ( ptr = fi_pVolCode1 , j=0 ; *ptr && *ptr != '-' ; 
					  ptr++ , j++ )
					VolCode1[0][j] = *ptr ;
				if ( *ptr == '\0' )	return -1;
				VolCode1[0][j] = '\0';

				for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
					VolCode1[1][j] = *ptr ;
				VolCode1[1][j] = '\0';

				//-------------------------------
				for ( ptr = fi_pVolCode2, j=0 ; *ptr && *ptr != '-' ; ptr++ , j++ )
					VolCode2[0][j] = *ptr ;
				if ( *ptr == '\0' )	return -1;
				VolCode2[0][j] = '\0';

				for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
					VolCode2[1][j] = *ptr ;
				VolCode2[1][j] = '\0';

			break;
		default :
			return -1;
	}

	//-------------------------------------------------------------------------------
	// �� ���� �ϳ��� ���� ���ƾ� �Ѵ�.
	//-------------------------------------------------------------------------------

	// �� ���� ���� ���
    if ( !_tcscmp ( VolCode1[0] , VolCode2[0] ) &&
          _tcscmp ( VolCode1[1] , VolCode2[1] ) )
	{
		*fo_pAddLocate = 2 ; 
		_stprintf ( fo_pSameVolCode , _T("%s") , VolCode1[0] );
		_stprintf ( fo_pAddVolStart , _T("%s") , VolCode1[1] );
		_stprintf ( fo_pAddVolEnd   , _T("%s") , VolCode2[1] );

	}	
	// �� ���� ���� ���
	else if (  _tcscmp ( VolCode1[0] , VolCode2[0] ) && 
              !_tcscmp ( VolCode1[1] , VolCode2[1] ) )
	{
		*fo_pAddLocate = 1 ; 
		_stprintf ( fo_pSameVolCode , _T("%s") , VolCode1[1] );
		_stprintf ( fo_pAddVolStart , _T("%s") , VolCode1[0] );
		_stprintf ( fo_pAddVolEnd   , _T("%s") , VolCode2[0] );
	}
	else
		return -1;

	return 0;
}

//-----------------------------------------------------------------------
// ��å��ȣ�� �ִ� ���¿��� 
// ��Ϲ�ȣ , ��å��ȣ , ������ȣ�� ���Ѵ�.
//-----------------------------------------------------------------------
INT CPlusIN049Tag::Save_OnlyVolCode_ExistBraceHypen ( TCHAR * fi_pAccessionNo1   ,
													  TCHAR * fi_pAccessionNo2   ,
													  INT    fi_BraceHypen_Flag ,
													  TCHAR * fi_pSameVolCode    ,
													  INT    fi_AddLocate       ,
													  TCHAR * fi_pAddVolStart    ,
													  TCHAR * fi_pAddVolEnd      ,
 													  TCHAR * fo_pAccessionNo    ,
													  TCHAR * fo_pVolCode        ,
												  	  TCHAR * fo_pCopyCode       )
{
	INT rtn1 , rtn2         ;
	INT VolCode , Start_Vol , End_Vol ;
	INT i;

	rtn1 = Check_VolCode_Eng_Integer ( fi_pAddVolStart );
    rtn2 = Check_VolCode_Eng_Integer ( fi_pAddVolEnd   );

	if ( ( rtn1 < 0 || rtn2 < 0 ) || ( rtn1 != rtn2 ) )
		return -1;

	switch ( rtn1 )
	{
		case 1 :		// ����
				Start_Vol = _ttoi( fi_pAddVolStart ) ;
				End_Vol   = _ttoi( fi_pAddVolEnd )   ;
				break;

		case 2 :		// ����
				Start_Vol = fi_pAddVolStart[0];
				End_Vol   = fi_pAddVolEnd[0]  ;
				break;

		default :
			return -1;
	}

	// ���� Ȯ��
	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
		                        End_Vol - Start_Vol ) )
		return -1;								

	//-------------------------------------------------------------------
	// ��Ϲ�ȣ, ������ȣ, ��å��ȣ ����
	//-------------------------------------------------------------------
	for ( VolCode = Start_Vol , i = 0  ; VolCode <= End_Vol ; VolCode ++ , i++ )
	{
		// ��Ϲ�ȣ
		_stprintf ( fo_pAccessionNo + _tcsclen ( fo_pAccessionNo ) , _T("%.2s%010u") , 
			                                                     fi_pAccessionNo1 ,
																 _ttol(fi_pAccessionNo1 + 2 ) + i );
		// ��å��ȣ
		if ( rtn1 == 1 )  // ����
		{
			if ( fi_AddLocate == 1 ) // ���� ����
			{
				_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%d%c%s%s"),
																 VolCode   ,
																 ( fi_BraceHypen_Flag == 1 ) ? '(' : '-' ,
																 fi_pSameVolCode ,
																 ( fi_BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
			}
			if ( fi_AddLocate == 2 ) // �ڰ� ����
			{
				_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%s%c%d%s"),
																 fi_pSameVolCode                      ,
																 ( fi_BraceHypen_Flag == 1 ) ? '(' : '-' ,
																 VolCode                              ,
				                                                 ( fi_BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
			}
		}
		else if ( rtn1 == 2 )
		{
			if ( fi_AddLocate == 1 ) // ���� ����
			{
				_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%c%c%s%s"),
																 VolCode   ,
																 ( fi_BraceHypen_Flag == 1 ) ? '(' : '-' ,
																 fi_pSameVolCode ,
																 ( fi_BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
			}
			if ( fi_AddLocate == 2 ) // �ڰ� ����
			{
				_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%s%c%c%s"),
																 fi_pSameVolCode                      ,
																 ( fi_BraceHypen_Flag == 1 ) ? '(' : '-' ,
																 VolCode                              ,
				                                                 ( fi_BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
			}
		}

		// ������ȣ
		_stprintf ( fo_pCopyCode + _tcsclen ( fo_pCopyCode ) , _T("%c#") , ' ' );
	}

	return 0;
}

//-------------------------------------------------------------------------------
// ������ȣ�� �ִ�. �Ǵ�  ��å��ȣ�� ���� ������ȣ�� ������
//-------------------------------------------------------------------------------
INT CPlusIN049Tag::Save_OnlyCopyCode ( TCHAR * fi_pAccessionNo1 ,
									   TCHAR * fi_pAccessionNo2 ,
									   TCHAR * fi_pVolCode      ,
									   TCHAR * fi_pCopyCode1    ,
									   TCHAR * fi_pCopyCode2    ,
		 							   TCHAR * fo_pAccessionNo  ,
									   TCHAR * fo_pVolCode      , 
									   TCHAR * fo_pCopyCode     )
{ 
	INT CopyCode , StartCopy , EndCopy ;
	INT i ;

	StartCopy = _ttoi ( fi_pCopyCode1 );
	EndCopy   = _ttoi ( fi_pCopyCode2 );

	if ( StartCopy == 0 ) 
		StartCopy = 1;

	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
		                        EndCopy - StartCopy ) )
		return -1;								

	for ( CopyCode = StartCopy , i = 0 ; CopyCode <= EndCopy ; CopyCode++ , i++ )
	{
		// ��Ϲ�ȣ
		_stprintf ( fo_pAccessionNo + _tcsclen (fo_pAccessionNo) , _T("%.2s%010u") , fi_pAccessionNo1  ,
	              _ttol(fi_pAccessionNo1+2) + i );
		// ��å��ȣ
		_stprintf ( fo_pVolCode + _tcsclen(fo_pVolCode) , _T("%s#") ,
	              ( fi_pVolCode[0] != '\0' ) ? fi_pVolCode : _T(" ") );
		// ������ȣ
		if ( CopyCode > 1 )
    		_stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%d#") , CopyCode );
		else
    		_stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%c#") , ' ' );
	}

	return 0;
}

//--------------------------------------------------------------------------
// �и���ȣ�� ���� ���¿��� ������ȣ�� ���� ��å��ȣ�� ������ ����
//--------------------------------------------------------------------------
INT  CPlusIN049Tag::Save_SameCopyCodeAddVolCode_NoBraceHypen ( TCHAR * fi_pVolCode1     ,
						                                       TCHAR * fi_pVolCode2     ,
												               TCHAR * fi_pCopyCode     ,
												               TCHAR * fi_pAccessionNo1 ,
												               TCHAR * fi_pAccessionNo2 ,
 															   TCHAR * fo_pAccessionNo  , 
															   TCHAR * fo_pVolCode      , 
															   TCHAR * fo_pCopyCode     )
{
	INT rtn1 , rtn2;
	INT Start_Vol , End_Vol , AddVolCode , i ;
	INT SameCopyCode , CopyCode;

	// ��å��ȣ , ������ȣ 

	rtn1 = Check_VolCode_Eng_Integer ( fi_pVolCode1 );
    rtn2 = Check_VolCode_Eng_Integer ( fi_pVolCode2 );

	if ( ( rtn1 < 0 || rtn2 < 0 ) || ( rtn1 != rtn2 ) )
		return -1;

	if ( rtn1 == 1 )
	{
	   Start_Vol = _ttoi ( fi_pVolCode1 );
	   End_Vol   = _ttoi ( fi_pVolCode2 );
	}
	else 
	{
	   Start_Vol = fi_pVolCode1[0] ;
	   End_Vol   = fi_pVolCode2[0] ;
	}

	SameCopyCode = _ttoi ( fi_pCopyCode );

	// ���� Ȯ��
	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
		                        ( End_Vol - Start_Vol) * SameCopyCode ) )
		return -1;								

	// ù å���� ����

	_stprintf( fo_pAccessionNo+_tcsclen(fo_pAccessionNo) , _T("%.2s%010u"),
			 fi_pAccessionNo1  , _ttol( fi_pAccessionNo1 + 2 ) ) ;
	_stprintf( fo_pVolCode+_tcsclen(fo_pVolCode)   , _T("%s#") , fi_pVolCode1 );
	_stprintf( fo_pCopyCode+_tcsclen(fo_pCopyCode) , _T("%s#") , fi_pCopyCode );

	//-----------------------------------------------------------------------------------
	// ��Ϲ�ȣ , ������ȣ , ��å��ȣ
	//-----------------------------------------------------------------------------------
	for ( AddVolCode = Start_Vol + 1 , i=1 ; AddVolCode <= End_Vol ; AddVolCode ++  )
	{
		for ( CopyCode = 1 ; CopyCode <= SameCopyCode ; CopyCode++ , i++ )
		{
			// ��Ϲ�ȣ
			_stprintf ( fo_pAccessionNo + _tcsclen(fo_pAccessionNo) , _T("%.2s%010u") ,
				      fi_pAccessionNo1 ,
					  _ttol( fi_pAccessionNo1 + 2) + i );

			// ��å��ȣ
			if ( rtn1 == 1 ) // ����
				_stprintf ( fo_pVolCode + _tcsclen( fo_pVolCode ) , _T("%d#") , AddVolCode );
			else
				_stprintf ( fo_pVolCode + _tcsclen( fo_pVolCode ) , _T("%c#") , AddVolCode );

			// ������ȣ
			if ( CopyCode == 1 )
			    _stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%s#"), _T(" "));
			else
			    _stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%d#"), CopyCode);
		}
	}

	return 0;
}

//--------------------------------------------------------------------------
// �и���ȣ�� �ִ� ���¿��� ������ȣ�� ���� ��å��ȣ�� 
//--------------------------------------------------------------------------
INT  CPlusIN049Tag::Save_SameCopyCodeAddVolCode_ExistBraceHypen ( TCHAR * fi_pAccessionNo1,
												   TCHAR * fi_pAccessionNo2,
												   INT    BraceHypen_Flag , 
											       TCHAR * fi_pSameVolCode ,
												   INT    fi_AddLocate    ,
												   TCHAR * fi_pAddVolStart ,
												   TCHAR * fi_pAddVolEnd   ,
												   TCHAR * fi_pCopyCode    ,
 								                   TCHAR * fo_pAccessionNo , 
		                                           TCHAR * fo_pVolCode     , 
                                                   TCHAR * fo_pCopyCode    )
{
	INT rtn1 , rtn2         ;
	INT AddVolCode , Start_Vol , End_Vol ;
	INT CopyCode , SameCopyCode ;
	INT i;

	rtn1 = Check_VolCode_Eng_Integer ( fi_pAddVolStart );
    rtn2 = Check_VolCode_Eng_Integer ( fi_pAddVolEnd   );

	if ( ( rtn1 < 0 || rtn2 < 0 ) || ( rtn1 != rtn2 ) )
		return -1;

	switch ( rtn1 )
	{
		case 1 :		// ����
				Start_Vol = _ttoi( fi_pAddVolStart ) ;
				End_Vol   = _ttoi( fi_pAddVolEnd )   ;
				break;

		case 2 :		// ����
				Start_Vol = fi_pAddVolStart[0] ;
				End_Vol   = fi_pAddVolEnd[0]   ;
				break;

		default :
			return -1;
	}

	SameCopyCode = _ttoi( fi_pCopyCode );

	// ���� Ȯ��
	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
		                        ( End_Vol - Start_Vol) * SameCopyCode ) )
		return -1;								

	for ( AddVolCode = Start_Vol + 1 , i = 1; AddVolCode <= End_Vol ; AddVolCode ++ )
	{
		for ( CopyCode = 1 ; CopyCode <= SameCopyCode ; CopyCode ++ , i++ )
		{
			// ��Ϲ�ȣ
			_stprintf ( fo_pAccessionNo+_tcsclen(fo_pAccessionNo) , _T("%.2s%010u") , 
					  fi_pAccessionNo1 , 
					  _ttol(fi_pAccessionNo1+2) + i );

			// ��å��ȣ
			if ( rtn1 == 1 )  // ����
			{
				if ( fi_AddLocate == 1 ) // ���� ����
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%d%c%s%s"),
																	 AddVolCode   ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 fi_pSameVolCode ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
				if ( fi_AddLocate == 2 ) // �ڰ� ����
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%s%c%d%s"),
																	 fi_pSameVolCode                      ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 AddVolCode                           ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
			}
			else if ( rtn1 == 2 )
			{
				if ( fi_AddLocate == 1 ) // ���� ����
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%c%c%s%s"),
																	 AddVolCode   ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 fi_pSameVolCode ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
				if ( fi_AddLocate == 2 ) // �ڰ� ����
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%s%c%c%s"),
																	 fi_pSameVolCode                      ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 AddVolCode                           ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
			}

			// ������ȣ
			if ( CopyCode == 1 )
				_stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%s#"), _T(" ") );
			else
				_stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%d#"),CopyCode );

		}
	}

	return 0;
}

//--------------------------------------------------------------------------
// ��å��ȣ�� �����ǰ� ������ȣ�� �ڿ��� ���� ��� && �и���ȣ ���� 
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Save_AddVolCode2thCopyCodeExist_NoBraceHypen ( TCHAR * fi_pVolCode1      ,
																  TCHAR * fi_pVolCode2      ,
																  TCHAR * fi_pCopyCode      ,
																  TCHAR * fi_pAccessionNo1  ,
																  TCHAR * fi_pAccessionNo2  ,
 																  TCHAR * fo_pAccessionNo   , 
																  TCHAR * fo_pVolCode       , 
																  TCHAR * fo_pCopyCode      )
{
	INT rtn1 , rtn2;
	INT Start_Vol , End_Vol , AddVolCode , i ;
	INT SameCopyCode , CopyCode;

	// ��å��ȣ , ������ȣ 

	rtn1 = Check_VolCode_Eng_Integer ( fi_pVolCode1 );
    rtn2 = Check_VolCode_Eng_Integer ( fi_pVolCode2 );

	if ( ( rtn1 < 0 || rtn2 < 0 ) || ( rtn1 != rtn2 ) )
		return -1;

	if ( rtn1 == 1 )
	{
	   Start_Vol = _ttoi ( fi_pVolCode1 );
	   End_Vol   = _ttoi ( fi_pVolCode2 );
	}
	else 
	{
	   Start_Vol = fi_pVolCode1[0] ;
	   End_Vol   = fi_pVolCode2[0] ;
	}

	SameCopyCode = _ttoi ( fi_pCopyCode );

	// ���� Ȯ��
	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
		                        ( ( End_Vol - Start_Vol) + 1 ) * SameCopyCode - 1 ) )
		return -1;								

	//-----------------------------------------------------------------------------------
	// ��Ϲ�ȣ , ������ȣ , ��å��ȣ
	//-----------------------------------------------------------------------------------
	for ( AddVolCode = Start_Vol , i=0 ; AddVolCode <= End_Vol ; AddVolCode ++  )
	{
		for ( CopyCode = 1 ; CopyCode <= SameCopyCode ; CopyCode++ , i++ )
		{
			// ��Ϲ�ȣ
			_stprintf ( fo_pAccessionNo + _tcsclen(fo_pAccessionNo) , _T("%.2s%010u") ,
				      fi_pAccessionNo1 ,
					  _ttol( fi_pAccessionNo1 + 2) + i );

			// ��å��ȣ
			if ( rtn1 == 1 ) // ����
				_stprintf ( fo_pVolCode + _tcsclen( fo_pVolCode ) , _T("%d#") , AddVolCode );
			else
				_stprintf ( fo_pVolCode + _tcsclen( fo_pVolCode ) , _T("%c#") , AddVolCode );

			// ������ȣ
			if ( CopyCode == 1 )
			    _stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%s#"), _T(" "));
			else
			    _stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%d#"), CopyCode);
		}
	}

	return 0;
}

//--------------------------------------------------------------------------
// �и���ȣ�� �ִ� ���¿��� ��å��ȣ�� �����ǰ� ������ȣ�� �ڿ��� ����
//--------------------------------------------------------------------------
INT  CPlusIN049Tag::Save_AddVolCode2thCopyCodeExist_ExistBraceHypen ( TCHAR * fi_pAccessionNo1,
													      TCHAR * fi_pAccessionNo2,
												          INT    BraceHypen_Flag , 
											              TCHAR * fi_pSameVolCode ,
												          INT    fi_AddLocate    ,
												          TCHAR * fi_pAddVolStart ,
												          TCHAR * fi_pAddVolEnd   ,
												          TCHAR * fi_pCopyCode    ,
 								                          TCHAR * fo_pAccessionNo , 
		                                                  TCHAR * fo_pVolCode     , 
                                                          TCHAR * fo_pCopyCode    )
{
	INT rtn1 , rtn2         ;
	INT AddVolCode , Start_Vol , End_Vol ;
	INT CopyCode , SameCopyCode ;
	INT i;

	rtn1 = Check_VolCode_Eng_Integer ( fi_pAddVolStart );
    rtn2 = Check_VolCode_Eng_Integer ( fi_pAddVolEnd   );

	if ( ( rtn1 < 0 || rtn2 < 0 ) || ( rtn1 != rtn2 ) )
		return -1;

	switch ( rtn1 )
	{
		case 1 :		// ����
				Start_Vol = _ttoi( fi_pAddVolStart ) ;
				End_Vol   = _ttoi( fi_pAddVolEnd )   ;
				break;

		case 2 :		// ����
				Start_Vol = fi_pAddVolStart[0] ;
				End_Vol   = fi_pAddVolEnd[0]   ;
				break;

		default :
			return -1;
	}

	SameCopyCode = _ttoi( fi_pCopyCode );

	// ���� Ȯ��
	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
		                        (( End_Vol - Start_Vol) + 1 ) * SameCopyCode - 1 ) )
		return -1;								

	for ( AddVolCode = Start_Vol , i = 0; AddVolCode <= End_Vol ; AddVolCode ++ )
	{
		for ( CopyCode = 1 ; CopyCode <= SameCopyCode ; CopyCode ++ , i++ )
		{
			// ��Ϲ�ȣ
			_stprintf ( fo_pAccessionNo+_tcsclen(fo_pAccessionNo) , _T("%.2s%010u") ,
					  fi_pAccessionNo1 ,
					  _ttol(fi_pAccessionNo1+2) + i );

			// ��å��ȣ
			if ( rtn1 == 1 )  // ����
			{
				if ( fi_AddLocate == 1 ) // ���� ����
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%d%c%s%s"),
																	 AddVolCode   ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 fi_pSameVolCode ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
				if ( fi_AddLocate == 2 ) // �ڰ� ����
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%s%c%d%s"),
																	 fi_pSameVolCode                      ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 AddVolCode                           ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
			}
			else if ( rtn1 == 2 )
			{
				if ( fi_AddLocate == 1 ) // ���� ����
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%c%c%s%s"),
																	 AddVolCode   ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 fi_pSameVolCode ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
				if ( fi_AddLocate == 2 ) // �ڰ� ����
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%s%c%c%s"),
																	 fi_pSameVolCode                      ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 AddVolCode                           ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
			}

			// ������ȣ
			if ( CopyCode == 1 )
				_stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%s#"), _T(" ") );
			else
				_stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%d#"),CopyCode );
		}
	}

	return 0;
}

//===============================================================================
// ��Ϲ�ȣ�� �ߺ��� Ȯ���Ѵ�.
// - fo_psDupAccno_List �� ' '�� �������� ����ȴ�.
//===============================================================================
INT CPlusIN049Tag::Dup_AccessionNo ( TCHAR * fi_pAccessionNo    ,
								     TCHAR * fo_psDupAccno_List /*=NULL*/)
{
	INT  nAccessionCnt           ;
	INT  nSameAccessionCnt = 0   ;
	BOOL DupAccession_Exist = FALSE ;

	if ( fo_psDupAccno_List ) fo_psDupAccno_List[0] = '\0' ;

	nAccessionCnt = _tcsclen ( fi_pAccessionNo ) / 12 ;
	//-----------------------------------------------------------------
	//  ����Ʈ ���
	//-----------------------------------------------------------------
	for ( INT i=0 , j ; i < nAccessionCnt - 1 ; i++ )
	{
		for ( j = i+1 , nSameAccessionCnt=1 ; j < nAccessionCnt ; j++ )
		{
			if ( !_tcsncmp ( fi_pAccessionNo + (i*12) ,
				            fi_pAccessionNo + (j*12) ,
							12 ) )
			{
				nSameAccessionCnt++ ;
			}
		}

		if ( nSameAccessionCnt > 1 )
		{
			DupAccession_Exist = TRUE ;
			if ( fo_psDupAccno_List )
			{
				if ( fo_psDupAccno_List[0] != '\0' )
					_tcscat ( fo_psDupAccno_List , _T(" ") ) ;
				_stprintf ( fo_psDupAccno_List+_tcsclen(fo_psDupAccno_List), _T("%-12.12s") , fi_pAccessionNo+(i*12) );
			}
		}
	}

	if ( DupAccession_Exist )
		return -1;

	return 0;
}

/*
	INT nAccessionCnt           ;
	INT nSameAccessionCnt = 0   ;

	fo_pDup_AccnoList[0] = '\0' ;

	nAccessionCnt = _tcsclen ( fi_pAccessionNoList ) / 12 ;
	//-----------------------------------------------------------------
	//  ����Ʈ ���
	//-----------------------------------------------------------------
	for ( INT i=0 , j ; i < nAccessionCnt - 1 ; i++ )
	{
		for ( j = i+1 , nSameAccessionCnt=1 ; j < nAccessionCnt ; j++ )
		{
			if ( !_tcsncmp ( fi_pAccessionNoList + (i*12) ,
				            fi_pAccessionNoList + (j*12) ,
							12 ) )
			{
				nSameAccessionCnt++ ;
			}
		}

		if ( nSameAccessionCnt > 1 )
		{
			_ftprintf ( put_fp , _T("%s")         , fi_pControlNo              );
			_ftprintf ( put_fp , _T("  %-12.12s") , fi_pAccessionNoList+(i*12) );
			_ftprintf ( put_fp , _T("  %d")       , nSameAccessionCnt          );
			_ftprintf ( put_fp , _T("  %s\n")     , fi_pColNumber              );

			if ( fo_pDup_AccnoList[0] != '\0' )
				_tcscat ( fo_pDup_AccnoList , _T(" ") ) ;
			_stprintf ( fo_pDup_AccnoList+_tcsclen(fo_pDup_AccnoList), _T("%-12.12s") , fi_pAccessionNoList+(i*12) );
		}
	}
	_fputts ( _T("\n") , put_fp );

*/




