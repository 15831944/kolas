// CreateAutorCode.cpp: implementation of the CCreateAutorCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CreateAutorCode.h"
#include "efs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//=====================================================
//2008.09.23 ADD BY PJW : K3���� ���ں�ȯ �޸𸮿��� �о��
#include "CharConvert.h"
//=====================================================

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCreateAutorCode::CCreateAutorCode()
{
	//=====================================================
	//2008.09.23 REM BY PJW : K3���� ���ں�ȯ �����о���� �κ� ���ŵ� -> �޸𸮿��� �о��
//  	m_CharSetMgr_KorToChi.InitData(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));
	//=====================================================
	m_CharSetMgr_KorToConsonant.InitData(_T("�ѱ�"), _T("����"));
}

CCreateAutorCode::~CCreateAutorCode()
{

}

/*==============================================================================
 2000.10.2. ��õ����(����,�߾�,ȭ����) ������ �䱸�������� �߰�

 Elord���ڱ��ȣ�� ������ �ش�.
 1.���ڱ�ȣ������ ���� ������ 3���ڿ� ������ 1���ڸ� �����ٰ� ����
 2.���ڰ� 2���� �Ǵ� 1������ ���� ������ ������ ���� ','�� ���̰� ������ ù���ڸ� �ο��Ѵ�.
 3.���� ��ȣ�� �ִ� ��� ��ȣ�� Data�� �����ϰ� �� ������ ������ ������ �ۼ�.(��Ī�� ����)
 4.���ڰ� �ټ��̰ų� ��Ȯ���� ��� �Ǵ� �Ѽ����� ���� ������ ù 3���ڿ� ���ǻ��� ù���ڸ� �ο��Ѵ�.
  * �ѱ��� �����ڼ��� �⺻���� �ϸ� ���� �� ����, Ư�� ��ȣ�� �����Ѵ�.

IN :    f_pSubFieldData - ���ڸ�
        f_AuthorAddData - ���ڱ�ȣ �ΰ� ��ȣ
IN :    f_pElordAuthorData - Elord���ڸ��ȣ��   
==============================================================================*/
INT CCreateAutorCode::GetElordAuthorDate( TCHAR         *f_pSubFieldData    , 
                                       TCHAR         *f_AuthorAddData    ,
                                       TCHAR         *f_pElordAuthorData )
{
	EFS_BEGIN

	//if (m_CharSetMgr_AutorCode.InitData(_T("Elord"), _T("Elord_�ڵ�")) < 0) return -1;

    TCHAR  s_AuthorData[1024],s_tmpAuthorData[32];
    TCHAR *s_pStrptr;
    INT   i,j,s_Strlen;

    _tcscpy( s_AuthorData , f_pSubFieldData );

    s_pStrptr = _tcstok(  s_AuthorData , _T(" ") );
    if( s_pStrptr == NULL ) return -1;
        
    s_Strlen = _tcsclen( s_pStrptr );

    j = 0;
    for( i = 0; i < s_Strlen ; i++ ){
        if( (_TUCHAR)s_pStrptr[i] >  0x80 ){
            s_tmpAuthorData[j] = s_pStrptr[i];
            j++;
        }
    }

    s_tmpAuthorData[j] = '\0';

    f_pElordAuthorData[0] = '\0';
    
	// ä��ȣ
	//=====================================================
	//2009.02.16 UPDATE BY PJW : ELOAD�� ���� ù3���ڸ� �̿��Ѵ�.
// 	if( _tcsclen(s_tmpAuthorData) >= 6 ){
//         _tcsnccpy( f_pElordAuthorData , s_tmpAuthorData , 6 );
//         f_pElordAuthorData[6] = '\0';
//     }
	if( _tcsclen(s_tmpAuthorData) >= 3 ){
        _tcsnccpy( f_pElordAuthorData , s_tmpAuthorData , 3 );
        f_pElordAuthorData[3] = '\0';
    }
	//=====================================================
    else{
        _tcsnccpy( f_pElordAuthorData , s_tmpAuthorData , _tcsclen(s_tmpAuthorData) );
        f_pElordAuthorData[_tcsclen(s_tmpAuthorData)] = '\0';
		//=====================================================
		//2009.01.07 REM BY PJW : ELOAD�� ���ڿ� ���� ù�� ���̿� ','�� ���� �ʴ´�.
		//                        ���߰� �ߴ����� �𸣰��� -> �˾ƺ���
		//2009.07.17 UPDATE BY PJW : ELOAD�� ������ 2�� �����̸� ','�� �ִ´�.		
		_tcscpy( &f_pElordAuthorData[_tcsclen(f_pElordAuthorData)] , _T(","));
		//=====================================================
    }

	// ä��ȣ
    if( f_AuthorAddData[0] == '(' ){
        s_pStrptr = _tcstok( f_AuthorAddData , _T(")") );
        if( s_pStrptr == NULL ){
            return -2;
        }
        s_pStrptr = _tcstok( NULL, _T(")") );
        if( s_pStrptr == NULL ){
            return -2;
        }
		str_ltrim( s_pStrptr , s_pStrptr , ' ' );
    } else {
		s_pStrptr = f_AuthorAddData;
	}

    s_Strlen = _tcsclen(f_pElordAuthorData);
    for( i = 0 ; i < (INT)_tcsclen(s_pStrptr); i++ ){
        if( (_TUCHAR)s_pStrptr[i] >  0x80 ){
            _tcsnccpy( &f_pElordAuthorData[s_Strlen] , &s_pStrptr[i] , 2 );
			f_pElordAuthorData[s_Strlen+2] = '\0';
            break;
        }
    }

    return 0;

	EFS_END
	return -1;
  
}



// ���ϼ����ڱ�ȣ ����
INT CCreateAutorCode::GetJangIlSeaAuthorData(
                                         TCHAR         *f_pSubFieldData        ,  // ���ڸ�
                                         TCHAR         *f_AuthorAddData        ,  // �ΰ���ȣ
                                         TCHAR         *f_pJangIlSeaAuthorData )  // ���ϼ����ڱ�ȣ
{
	EFS_BEGIN

	if (m_CharSetMgr_AutorCode.InitData(_T("���ϼ�"), _T("���ϼ�_�ڵ�")) < 0) return -1;

    TCHAR  s_AuthorData[1024],s_tmpAuthorData[32],s_Data[32],s_TmpData[128];
    INT   ids;

    str_ltrim( f_pSubFieldData , f_pSubFieldData , ' ' );
    str_rtrim( f_pSubFieldData , f_pSubFieldData , ' ' );

    str_ltrim( f_AuthorAddData , f_AuthorAddData , ' ' );
    str_rtrim( f_AuthorAddData , f_AuthorAddData , ' ' );


    _tcscpy( s_TmpData, f_pSubFieldData );

    //=====================================================
	//2009.02.04 UPDATE BY PJW : UNICODE�� ���鼭 LENGTH�����
// 	_tcsnccpy( s_AuthorData , f_pSubFieldData , 2 );
//     s_AuthorData[2] = '\0';
	_tcsnccpy( s_AuthorData , f_pSubFieldData , 1 );
    s_AuthorData[1] = '\0';
	//=====================================================
    s_Data[0] ='\0';
    f_pJangIlSeaAuthorData[0] = '\0';


    // 1.�ѱ��� ��쿡�� ������ ���δ�.
    if( (_TUCHAR)s_AuthorData[0] >= 0x80 ){
		ids = m_CharSetMgr_KorToConsonant.Find(s_AuthorData, s_Data);
        if( ids < 0 ){            
            return -1;
        }
        if( s_Data[0] != '\0' ){
            _tcscpy( &f_pJangIlSeaAuthorData[_tcsclen(f_pJangIlSeaAuthorData)] , s_Data );
        }
    }    

    s_tmpAuthorData[0] = '\0';
    /// 2.���ڰ� �ִ� ��� ���ڸ� �ѱ۷� ��ȯ.
	//=====================================================
	//2008.09.23 UPDATE BY PJW : K3���� ���ں�ȯ �����о���� �κ� ���ŵ� -> �޸𸮿��� �о��  
//    if( (_TUCHAR)f_pSubFieldData[0] <= 0xFE && (_TUCHAR)f_pSubFieldData[0] >= 0xCA ){
//		m_CharSetMgr_KorToChi.Find(s_TmpData, s_tmpAuthorData);
//        if( ids < 0 ) {
//            return -2;
//        }
//        if( s_tmpAuthorData[0] != '\0'){
//            _tcscpy( s_TmpData , s_tmpAuthorData );
//        }
//    }
	if( (_TUCHAR)f_pSubFieldData[0] <= 0xFE && (_TUCHAR)f_pSubFieldData[0] >= 0xCA ){
		CString strTemp ;
		strTemp.Format (_T("%s"),s_AuthorData );
		ConvertHanjaToHangul(strTemp);
		_tcscpy(s_TmpData,strTemp.GetBuffer (0));
	}
	//=====================================================
    


    /// 3.���ϼ� ���ڱ�ȣǥ���� ���ڱ�ȣ�� �����´�.
	for (INT i = 0; i < m_CharSetMgr_AutorCode.nData; i++) {
		ids = _tcscmp(s_TmpData, m_CharSetMgr_AutorCode.pData[i].szFrom);
		if (ids == 0)
		{
			_stprintf(s_Data, m_CharSetMgr_AutorCode.pData[i].szTo);
			break;
		}
		else if (ids < 0 && i != 0)
		{
			_stprintf(s_Data, m_CharSetMgr_AutorCode.pData[i - 1].szTo);
			break;
		}
	}

	//ids = m_CharSetMgr_AutorCode.Find(s_TmpData, s_Data);
    /*ids = m_pUserManager->GetCodeData( m_AuthorCodeValue  ,
                                       _T("�ڵ�")        ,
                                       s_TmpData     ,
                                       _T("��ȣ")        ,
                                       s_Data        );
									   */
    //if( ids < 0 ) return 100;

    if( s_Data[0] == 0 ){
        AfxMessageBox(_T("���ڱ�ȣ�� �߸��Ǿ����ϴ�."));
        return -3;
    }
    _tcscpy( &f_pJangIlSeaAuthorData[_tcsclen(f_pJangIlSeaAuthorData)] , s_Data );



    // 4.������ ù���� ������ ���δ�.
    _tcsnccpy( s_AuthorData , f_AuthorAddData , 2 );
    s_AuthorData[2] = '\0';
    
    
    if( (_TUCHAR)s_AuthorData[0] >= 0x80 ){
		ids = m_CharSetMgr_KorToConsonant.Find(s_AuthorData, s_Data);
        if( ids < 0 ){            
            return -1;
        }
        if( s_Data[0] != '\0' ){
            _tcscpy( &f_pJangIlSeaAuthorData[_tcsclen(f_pJangIlSeaAuthorData)] , s_Data );
        }
    }    

    return 0;
	
	EFS_END
	return -1;
  

    
}

// ���ʸ����ڱ�ȣ ����
INT CCreateAutorCode::GetJungPilMoAuthorData(
                                         TCHAR         *f_pSubFieldData        ,  // ���ڸ�
                                         TCHAR         *f_AuthorAddData        ,  // �ΰ���ȣ
                                         TCHAR         *f_pJungPilMoAuthorData )  // ���ʸ����ڱ�ȣ
{
EFS_BEGIN
/*
	if (m_CharSetMgr_AutorCode.InitData(_T("���ʸ�"), _T("���ʸ�_�ڵ�")) < 0) return -1;
    
	TCHAR  s_AuthorData[1024],s_tmpAuthorData[32],s_Data[32],s_TmpData[128];
    INT   ids;

    str_ltrim( f_pSubFieldData , f_pSubFieldData , ' ' );
    str_rtrim( f_pSubFieldData , f_pSubFieldData , ' ' );

    str_ltrim( f_AuthorAddData , f_AuthorAddData , ' ' );
    str_rtrim( f_AuthorAddData , f_AuthorAddData , ' ' );


    _tcscpy( s_TmpData, f_pSubFieldData );

    _tcsnccpy( s_AuthorData , f_pSubFieldData , 2 );
    s_AuthorData[2] = '\0';
    s_Data[0] ='\0';
    f_pJungPilMoAuthorData[0] = '\0';



    s_tmpAuthorData[0] = '\0';
    /// 2.���ڰ� �ִ� ��� ���ڸ� �ѱ۷� ��ȯ.
    if( (_TUCHAR)f_pSubFieldData[0] <= 0xFE && (_TUCHAR)f_pSubFieldData[0] >= 0xCA ){
		ids = m_CharSetMgr_KorToChi.Find(s_TmpData, s_tmpAuthorData);
        if( ids < 0 ) {
            return -2;
        }
        if( s_tmpAuthorData[0] != '\0'){
            _tcscpy( s_TmpData , s_tmpAuthorData );
        }
    }


    /// 3.���ʸ� ���ڱ�ȣǥ���� ���ڱ�ȣ�� �����´�.

	ids = m_CharSetMgr_AutorCode.Find(s_TmpData, s_Data);
    /*ids = m_pUserManager->GetCodeData( m_AuthorCodeValue  ,
                                       _T("�ڵ�")        ,
                                       s_TmpData     ,
                                       _T("��ȣ")        ,
                                       s_Data        );

    if( ids < 0 ) return 100;

    if( s_Data[0] == 0 ){
        AfxMessageBox(_T("���ڱ�ȣ�� �߸��Ǿ����ϴ�."));
        return -3;
    }
    _tcscpy( f_pJungPilMoAuthorData , s_Data );

    return 0;
*/

	CString sData = f_pSubFieldData;
	CString sAuthorMid, sAuthorNo;
	
	sAuthorNo = _T("");

		//���ϰ��
		CString strFilePath = _T("..\\CFG\\���ʸ�.txt");
		
		FILE *fp;
		fp = _tfopen( strFilePath , _T("rb") );
		
		if ( fp == NULL ) {
			AfxMessageBox( _T("���ڱ�ȣ���� ������ �����ϴ�.") );
			return -1;
		}
		
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode�϶� 2byte�ǳʶ��
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------
		
		INT iLenAuthor = sData.GetLength();
		while ( sAuthorNo == _T("") ) 
		{	
			TCHAR cLine[256];
			while( _fgetts( cLine, 256, fp ) ) 
			{
				
				CString sLine;
				sLine.Format( _T("%s"), cLine );
				CString tnp = sLine.Mid(0,8);
				tnp.Replace( _T(" "), NULL );
				if ( sData >= tnp ) 
				{
					INT iPos = sLine.Find(_T("|"));
					sAuthorNo = sLine.Mid( iPos+1, 5 );
					sAuthorNo.Replace( _T(" "), NULL );
					if ( sAuthorMid.Left(1) == _T(",") ) 
					{
						sAuthorNo = _T(",") + sAuthorNo;						
					}
				}else
					break;
			}			
			INT result = fseek( fp, 256, SEEK_SET);
	}
	
	if ( sAuthorNo.IsEmpty())
	{
		AfxMessageBox( _T("���ڱ�ȣ�� ã�� �� �����ϴ�. ���ڱ�ȣ���� ������ Ȯ���ϼ���.") );
	}
	
	_tcscpy( f_pJungPilMoAuthorData , sAuthorNo );
	return 0;
	
	EFS_END
	return -1;
  
}

INT CCreateAutorCode::GetLeeJaeChulAuthorData(				 
							 TCHAR          f_VolCode              ,  // ��ǥ��
                             TCHAR         *f_pSubFieldData        ,  // ���ڸ�                             
                             TCHAR         *f_pJangIlSeaAuthorData ) // ����ö���ڱ�ȣ
{
	EFS_BEGIN

	if (m_CharSetMgr_AutorCode.InitData(_T("����ö2ǥ"), _T("����ö2ǥ_�ڵ�")) < 0) return -1;
	
	TCHAR  s_AuthorData[1024],s_tmpAuthorData[32],s_Data[32],s_TmpData[128];
    TCHAR  s_FirstChar[3],s_AuthorCode[32];
    TCHAR *s_pStrptr;
    INT   ids;

	str_ltrim( f_pSubFieldData , f_pSubFieldData , ' ' );
    str_rtrim( f_pSubFieldData , f_pSubFieldData , ' ' );

    _tcscpy( s_TmpData, f_pSubFieldData );

    
    s_Data[0] ='\0';
    f_pJangIlSeaAuthorData[0] = '\0';
    
    s_tmpAuthorData[0] = '\0';
    /// 1.���ڰ� �ִ� ��� ���ڸ� �ѱ۷� ��ȯ.
	//=====================================================
	//2008.09.23 REM BY PJW : K3���� ���ں�ȯ �����о���� �κ� ���ŵ� -> �޸𸮿��� �о��  
//	if( (_TUCHAR)f_pSubFieldData[0] <= 0xFE && (_TUCHAR)f_pSubFieldData[0] >= 0xCA ){
//        ids = m_CharSetMgr_KorToChi.Find(s_TmpData, s_tmpAuthorData);
//        if( ids < 0 ) {
//            return -2;
//        }
//        if( s_tmpAuthorData[0] != '\0'){
//            _tcscpy( s_TmpData , s_tmpAuthorData );
//        }
//    }
	if( (_TUCHAR)f_pSubFieldData[0] <= 0xFE && (_TUCHAR)f_pSubFieldData[0] >= 0xCA ){
		CString strTemp ;
		strTemp.Format (_T("%s"),s_AuthorData );
		ConvertHanjaToHangul(strTemp);
		_tcscpy(s_TmpData,strTemp.GetBuffer (0));
	}
	//=====================================================
    
    _tcsnccpy( s_FirstChar , s_TmpData ,2 );
    s_FirstChar[2] = '\0';

    // 2. 3 ��° ����Ʈ�� �ѱ��� �ƴ� ���
	s_pStrptr = _tcschr( f_pSubFieldData , ',');
    if( s_pStrptr != NULL && f_pSubFieldData[2] == ',' ){
        
        str_trimall( s_pStrptr , s_pStrptr , ' ' );
        //s_FileIO.str_rtrim( s_pStrptr , s_pStrptr , ' ' );

		//_tcscpy( s_AuthorData , _T(",") );
        _tcsnccpy( s_AuthorData , s_pStrptr , 3 );
        s_AuthorData[3] = '\0';

        /// 2.2. ����ö ���ڱ�ȣǥ���� ���ڰ�ȣ�� �����´�.
		ids = m_CharSetMgr_AutorCode.Find(s_AuthorData, s_Data);
        if( ids < 0 ) return 100;

        if( s_Data[0] == 0 ){
            AfxMessageBox(_T("���ڱ�ȣ�� �߸��Ǿ����ϴ�."));
            return -3;
        }
        _stprintf( s_AuthorCode , _T(",%s"), s_Data );
    }
    else{
        _tcsnccpy( s_AuthorData , &f_pSubFieldData[2] , 2 );
        s_AuthorData[2] = '\0';
        /// 2.2. ����ö ���ڱ�ȣǥ���� ���ڰ�ȣ�� �����´�.
		ids = m_CharSetMgr_AutorCode.Find(s_AuthorData, s_Data);
        if( ids < 0 ) return 100;

        if( s_Data[0] == 0 ){
            AfxMessageBox(_T("���ڱ�ȣ�� �߸��Ǿ����ϴ�."));
            return -3;
        }
        _tcscpy( s_AuthorCode , s_Data );
    }
    
    _stprintf( f_pJangIlSeaAuthorData , _T("%s%s"), s_FirstChar, s_AuthorCode );

	return 0;
	
	EFS_END
	return -1;
  



}

VOID    CCreateAutorCode::str_trimall(
								TCHAR   *strln   ,
		                        TCHAR   *strline1,
		                        TCHAR    var
		                    )
{
	EFS_BEGIN

	INT     i = 0, j = 0;

    for ( i = 0; strln[i] != '\0' && strln[i] != '\n'; i++ ) {
        if ( strln[i] == var );
        else { strline1[j] = strln[i]; j++; }
    }
    strline1[j] ='\0';
	
	EFS_END
  
}

VOID    CCreateAutorCode::str_ltrim	(
								TCHAR   *strln   ,
		                        TCHAR   *strline1,
		                        TCHAR    var
							)
{
	EFS_BEGIN

	INT i = 0;

    for ( i = 0; strln[i] != '\0' && strln[i] != '\n'; i++ ){
        if(strln[i] != var) {
            _tcscpy( strline1, &strln[i] );
            return;
        }
    }
    strline1[0] = '\0';
	
	
	EFS_END

  
}

VOID    CCreateAutorCode::str_rtrim	(
								TCHAR   *strln   ,
		                        TCHAR   *strline1,
		                        TCHAR    var
							)
{
	EFS_BEGIN

	INT i = 0;

    _tcscpy( strline1, strln );
    for ( i = _tcsclen(strln)-1; i >= 0; i-- ){
        if(strln[i] != var) {
            strline1[i+1] = '\0';
            return;
        }
    }
    strline1[0] = '\0';
	EFS_END

  
}
// ���缭 ���ڱ�ȣ ����
// ������� 
// 1. ���� Tag���� ���� ��� ���ڱ�ȣ ���̺��� ���ں��� �۰ų� ���� ����� �ڷκ��� 
//    �Ѱ��� ���ڸ� ���� �����Ͽ� �߰ߵ� �ڷ��� ���ڱ�ȣ�� ���� ū ���� ���Ѵ�
CString CCreateAutorCode::GetOrientBookData(
											TCHAR *f_pSubFieldData,  //���ڸ�
											TCHAR *f_pAuthorAddData, //���ڱ�ȣ �ΰ���ȣ
											TCHAR *f_pOrientBookData //���缭 �ΰ���ȣ
											)
{
	
	EFS_BEGIN

	CString sData = f_pSubFieldData;
	CString sAuthorMid, sAuthorNo;
	
	sAuthorNo = _T("");

		
		//���ϰ��
		CString strFilePath = _T("..\\CFG\\���缭���ڱ�ȣ.txt");
		
		FILE *fp;
		fp = _tfopen( strFilePath , _T("rb") );
		
		if ( fp == NULL ) {
			AfxMessageBox( _T("���ڱ�ȣ���� ������ �����ϴ�.") );
			return -1;
		}
		
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode�϶� 2byte�ǳʶ��
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------
		
		INT iLenAuthor = sData.GetLength();
		while ( sAuthorNo == _T("") ) 
		{	
			TCHAR cLine[256];
			while( _fgetts( cLine, 256, fp ) ) 
			{
				
				CString sLine;
				sLine.Format( _T("%s"), cLine );
				CString tnp = sLine.Mid(0,14);
				tnp.Replace( _T(" "), NULL );
				if ( sData >= tnp ) 
				{
					INT iPos = sLine.Find(_T("|"));
					sAuthorNo = sLine.Mid( iPos+1, 5 );
					sAuthorNo.Replace( _T(" "), NULL );
					if ( sAuthorMid.Left(1) == _T(",") ) 
					{
						sAuthorNo = _T(",") + sAuthorNo;						
					}
				}else
					break;
			}			
			INT result = fseek( fp, 256, SEEK_SET);
	}
	
	if ( sAuthorNo.IsEmpty())
	{
		AfxMessageBox( _T("���ڱ�ȣ�� ã�� �� �����ϴ�. ���ڱ�ȣ���� ������ Ȯ���ϼ���.") );
	}
	
	CString strItemData = sData.Left(1) + sAuthorNo;
	return strItemData;

EFS_END
	return _T("");
}

/*==============================================================================
 ī�ͼ��� ���ڱ�ȣ ����

 1.���ڱ�ȣ������ ���� ������ ������ 1���� ���Ͽ��� �˻��� ���ڸ� �����Ͽ� �����Ѵ�.

IN :    f_pSubFieldData - ���ڸ�
        f_AuthorAddData - ���ڱ�ȣ �ΰ� ��ȣ
IN :    f_pElordAuthorData - Elord���ڸ��ȣ��   
==============================================================================*/
CString CCreateAutorCode::GetCutterSanbornAuthorData(
													 TCHAR *f_pSubFieldData, 
													 TCHAR *f_pAuthorAddData, 
													 TCHAR *f_pOrientBookData
													 )
{
EFS_BEGIN

	CString sData = f_pSubFieldData;
	CString sAuthorMid, sAuthorNo;

	sAuthorNo = _T("");
	
	//���ϰ��
	CString strFilePath = _T("..\\CFG\\ī�ͼ���.TXT");
	
	FILE *fp;
	fp = _tfopen( strFilePath , _T("rb") );
	
	if ( fp == NULL ) {
		AfxMessageBox( _T("���ڱ�ȣ���� ������ �����ϴ�.") );
		return -1;
	}
	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode�϶� 2byte�ǳʶ��
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------
	
/*
	TCHAR cLine[256];
	while( _fgetts( cLine, 256, fp ) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLine );
		CString tnp = sLine.Mid(0,sData.GetLength());
		tnp.Replace( _T(" "), NULL );
		if ( sData <= tnp ) {
			break;
		} else {
			INT iPos = sLine.Find(_T("|"));
			sAuthorNo = sLine.Mid( iPos+1, 3 );
			sAuthorNo.Replace( _T(" "), NULL );
		}
		if ( sData.Left(1) == _T(",") ) {
			sAuthorNo = _T(",") + sAuthorNo;						
		}
*/
		INT iLenAuthor = sData.GetLength();
		while ( sAuthorNo == _T("") ) 
		{	
			TCHAR cLine[256];
			while( _fgetts( cLine, 256, fp ) ) 
			{
				
				CString sLine;
				sLine.Format( _T("%s"), cLine );
				CString tnp = sLine.Mid(0,14);
				
				tnp.TrimLeft();
				tnp.TrimRight();

				//===================================================
				//2009.06.08 ADD BY PJW : ī�ͼ��� ���� �񱳽� " "�� ���� �ϰ� ���Ѵ�.
				// 2009.11.11 UPDATE BY PDJ : ������ �����ϸ� �������� �񱳸� �� �� ����.
				// ex) 'ROSEM'�� 'ROSE M'�� ���� �ٸ� ���̴�.
				//2009.12.14 UPDATE BY PJW : ���������� ����ȭ�� �䱸�������� �������� ����(2009/12/14���ڷ� ���� ����)
				//2010.05.25 UPDATE BY PJW : ���������� ������ ����� �䱸����(2010.05.25 : 5�� ���� ��ġ)���� SPACE �������� �ʵ��� ��
//				tnp.Replace(_T(" "),_T(""));
				//===================================================

				if ( sData >= tnp ) 
				{
					INT iPos = sLine.Find(_T("|"));
					sAuthorNo = sLine.Mid( iPos+1, 5 );
					sAuthorNo.TrimLeft();
					sAuthorNo.TrimRight();
					if ( sAuthorMid.Left(1) == _T(",") ) 
					{
						sAuthorNo = _T(",") + sAuthorNo;						
					}
				}else
					break;
			}			
			INT result = fseek( fp, 256, SEEK_SET);
	}				
	
	if ( sAuthorNo.IsEmpty())
	{
		AfxMessageBox( _T("���ڱ�ȣ�� ã�� �� �����ϴ�. ���ڱ�ȣ���� ������ Ȯ���ϼ���.") );
	}

	CString strItemData = sData.Left(1) + sAuthorNo;
	return strItemData;

EFS_END
	return _T("");
}

/*==============================================================================
 ī�ͼ���2 ���ڱ�ȣ ����

 1.���ڱ�ȣ������ ���� ������ ������ 1���� ���Ͽ��� �˻��� ���ڸ� �����Ͽ� �����Ѵ�.

IN :    f_pSubFieldData - ���ڸ�
        f_AuthorAddData - ���ڱ�ȣ �ΰ� ��ȣ
IN :    f_pElordAuthorData - Elord���ڸ��ȣ��   
==============================================================================*/
CString CCreateAutorCode::GetCutterSanborn2AuthorData(
													 TCHAR *f_pSubFieldData, 
													 TCHAR *f_pAuthorAddData, 
													 TCHAR *f_pOrientBookData
													 )
{
EFS_BEGIN

	CString sData = f_pSubFieldData;
	CString sAuthorMid, sAuthorNo;

	sAuthorNo = _T("");
	
	//���ϰ��
	CString strFilePath = _T("..\\CFG\\ī�ͼ���2.TXT");
	
	FILE *fp;
	fp = _tfopen( strFilePath , _T("rb") );
	
	if ( fp == NULL ) {
		AfxMessageBox( _T("���ڱ�ȣ���� ������ �����ϴ�.") );
		return -1;
	}
	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode�϶� 2byte�ǳʶ��
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------
	
/*
	TCHAR cLine[256];
	while( _fgetts( cLine, 256, fp ) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLine );
		CString tnp = sLine.Mid(0,sData.GetLength());
		tnp.Replace( _T(" "), NULL );
		if ( sData <= tnp ) {
			break;
		} else {
			INT iPos = sLine.Find(_T("|"));
			sAuthorNo = sLine.Mid( iPos+1, 3 );
			sAuthorNo.Replace( _T(" "), NULL );
		}
		if ( sData.Left(1) == _T(",") ) {
			sAuthorNo = _T(",") + sAuthorNo;						
		}
*/
		INT iLenAuthor = sData.GetLength();
		CString strAuthorCode;
		CString strKeywordAuthorCode;	
		CString tnp;
		CString sLine;

		while ( sAuthorNo == _T("") ) 
		{	
			TCHAR cLine[256];
			while( _fgetts( cLine, 256, fp ) ) 
			{				
				
				sLine.Format( _T("%s"), cLine );
				tnp = sLine.Mid(0,14);
				tnp.TrimLeft();
				tnp.TrimRight();
				strKeywordAuthorCode = tnp;
				strKeywordAuthorCode.MakeUpper();
				if ( sData >= strKeywordAuthorCode ) 
				{
					INT iPos = sLine.Find(_T("|"));
					sAuthorNo = sLine.Mid( iPos+1, 5 );
					sAuthorNo.TrimLeft();
					sAuthorNo.TrimRight();
					if ( sAuthorMid.Left(1) == _T(",") ) 
					{
						sAuthorNo = _T(",") + sAuthorNo;						
					}
					strAuthorCode = tnp;					
				}else
					break;
			}			
			INT result = fseek( fp, 256, SEEK_SET);
	}				
	
	if ( sAuthorNo.IsEmpty())
	{
		AfxMessageBox( _T("���ڱ�ȣ�� ã�� �� �����ϴ�. ���ڱ�ȣ���� ������ Ȯ���ϼ���.") );
	}

	// 2005.08.02 ADD BY PDJ
	// ī�ͼ���2�ΰ�� ���۱����� ������ ���� �ڵ��� �ڸ����� ����
	INT nCodeLen;
	nCodeLen = 1;
	if ( sData.Left(1) == _T("A")  || sData.Left(1) == _T("E")  || 
		 sData.Left(1) == _T("I")  || sData.Left(1) == _T("O")  || 
		 sData.Left(1) == _T("U")  || sData.Left(1) == _T("S")  )
	{
		nCodeLen=2;
	}
	if ( sData.Left(2) == _T("SC") )
	{
		nCodeLen=3;
	}

	if ( sData.GetLength() < nCodeLen )
	{
		nCodeLen = sData.GetLength();
	}	

	CString strItemData = strAuthorCode.Left(nCodeLen) + sAuthorNo;
	return strItemData;

EFS_END
	return _T("");
}

//=====================================================
//2008.09.23 REM BY PJW : K3���� ���ں�ȯ �޸𸮿��� �о�� 
INT CCreateAutorCode::ConvertHanjaToHangul(CString &strSrc)	
{
	EFS_BEGIN	

	if ( strSrc == "" ) return 0;

	CharConvert	pCharConvert;
	
	CHAR		*UTF8String;
	_TCHAR		*OutUTF16;
	INT			ids;
	INT			nLen;
	
	CharConvert dlg;	
	
	ids = pCharConvert.UnicodeToUTF8(strSrc.GetBuffer(0) , &UTF8String, &nLen);
	ids = pCharConvert.Hanja2Hangul(UTF8String);
	ids = pCharConvert.UTF8ToUnicode(UTF8String, &OutUTF16, &nLen);
	
	strSrc.Format(_T("%s"), OutUTF16 );
	
	free(UTF8String);
	free(OutUTF16);

	
	return 0;

	EFS_END
	return -1;

}
//=====================================================