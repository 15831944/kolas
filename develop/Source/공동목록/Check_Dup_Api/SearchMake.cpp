// SearchMake.cpp: implementation of the SearchMake class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include _T("check_dup_api.h")
#include "SearchMake.h"
#include "..\..\esl\marc\Marcedit\MarcEditor.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//2004-03-03 [������ ����]
CSearchMake::CSearchMake(CESL_Mgr *pParentMgr /*= NULL*/)
{
    m_tSEARCH97_RecordStru      = NULL;
    m_nSEARCH97_RecordCnt       = 0;
    m_tSEARCH97_DelRecordStru   = NULL;
    m_nSEARCH97_DelRecordCnt    = 0;
    
	//indexTool = CIndexTool*)pParentMgr->m_pInfo->pIndexTool;
	
    m_pIndexTool = (CIndexTool*)pParentMgr->m_pInfo->pIndexTool ;
	
}

CSearchMake::~CSearchMake()
{
   
}


VOID CSearchMake::Init_S97_SpeciesTable( )
{
   INT i, j;

    for ( i = 0; i < m_nSEARCH97_RecordCnt; i++ ) {
        for ( j = 0; j < m_tSEARCH97_RecordStru[i].TagCnt; j++ ) {
            if ( m_tSEARCH97_RecordStru[i].pTag[j].pData != NULL ) 
                free( m_tSEARCH97_RecordStru[i].pTag[j].pData );
        }
        if ( m_tSEARCH97_RecordStru[i].pTag != NULL ) free( m_tSEARCH97_RecordStru[i].pTag );
    }

    for ( i = 0; i < m_nSEARCH97_DelRecordCnt; i++ ) {
        for ( j = 0; j < m_tSEARCH97_DelRecordStru[i].TagCnt; j++ ) {
            if ( m_tSEARCH97_DelRecordStru[i].pTag[j].pData != NULL ) 
                free( m_tSEARCH97_DelRecordStru[i].pTag[j].pData );
        }
        if ( m_tSEARCH97_DelRecordStru[i].pTag != NULL ) free( m_tSEARCH97_DelRecordStru[i].pTag );
    }
    
    pDataMgr->InitDBFieldData () ;

    m_tSEARCH97_RecordStru      = NULL;
    m_nSEARCH97_RecordCnt		= 0;
    m_tSEARCH97_DelRecordStru   = NULL;
    m_nSEARCH97_DelRecordCnt    = 0;

    m_nUpdateCollectionType		= -1;
}



INT  CSearchMake::Make_S97_SpeciesTable( INT       f_nDMLType   , // _UPDATE , _INSERT, _DELETE
                                        CString    f_icDispAlias,
                                        TCHAR      *f_icRegCode  ,
                                        BOOL       f_ibCommit  ,    // TRUE : DEFAULT
									    TCHAR	 **cFieldName ,
										TCHAR     **cFieldData ,
										INT       *nFieldType ,
										INT        nFieldCnt
                                         )

{	
    TCHAR           cRegCode[4];
	INT             ids, i;
    
    
	if ( f_ibCommit ) {
		m_tSEARCH97_RecordStru      = NULL;
		m_nSEARCH97_RecordCnt		= 0;
	}

    cRegCode[0] = 0;

    //===========================================================
    // 1. SEARCH97�� ���� STRUCTURE ����.
	//===========================================================
    m_tSEARCH97_RecordStru = ( tRECORD_STRU * )realloc( m_tSEARCH97_RecordStru, sizeof( tRECORD_STRU ) * (m_nSEARCH97_RecordCnt+1) );	
    if ( m_tSEARCH97_RecordStru == NULL ) {
        Init_S97_SpeciesTable( );
        return -6;
    }
    
    

    m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].TagCnt = 0;
    m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].pTag   = NULL;
    
    //===========================================================
    // 2. ������ �ڷḦ ��� �����´�.
	//===========================================================       


    //===========================================================
    // 2. �۾������ �Ǵ� Collection�� ã�´�.
	//===========================================================
	m_nUpdateCollectionType = -1;
	
	// KOLIS NET
	
	// �˻��������� �������±����� �����Ǿ���.
	for ( i = 0; i < nFieldCnt; i++ ) {
		if ( _tcscmp(cFieldName[i], _T("PUB_FORM_CODE") ) != 0 ) {
			ids = Add_S97_OneField( 1, &cFieldName[i], &cFieldData[i] );
			if ( ids ) return ids;
		}
	}
	
	// 2.2 1���� : �������±���.
	for( i = 0; i < nFieldCnt; i++ ) {
		if ( _tcscmp(cFieldName[i], _T("PUB_FORM_CODE") ) == 0 ) {
			if ( cFieldData[i] != NULL && _tcscmp( cFieldData[i], _T("SE") ) == 0 ) m_nUpdateCollectionType = _KN_YON_;
		}
	}
	
	// 2.2 2���� : �̿����ѱ���.
	if ( m_nUpdateCollectionType == -1 ) {
		for( i = 0; i < nFieldCnt; i++ ) {
			if ( _tcscmp(cFieldName[i], _T("USE_LIMIT_CODE") ) == 0 ) {
				if ( cFieldData[i] != NULL && _tcscmp( cFieldData[i], _T("CL") ) == 0 ) m_nUpdateCollectionType = _KN_DAN_LIMIT_;
				break;
			}
		} 
	}
	
	TCHAR			*szpCollType;
	szpCollType = (TCHAR*) malloc ( sizeof(TCHAR) * 10 );
	// 2.3 3���� : �ڷᱸ��.
	if ( m_nUpdateCollectionType == -1 ) {
		for( i = 0; i < nFieldCnt; i++ ) {
			if ( _tcscmp(cFieldName[i], _T("MAT_CODE") ) == 0 &&
				m_nUpdateCollectionType == -1           ) {
				if ( cFieldData[i] != NULL ) {
					if ( ( _tcscmp( cFieldData[i], _T("DM") ) == 0 || _tcscmp( cFieldData[i], _T("DP") ) == 0 ) )  
					{   // ������
						m_nUpdateCollectionType = _KN_DAN_DMDP_;
						_tcscpy(szpCollType, _T("dan-dmdp"));
						
					} else if ( _tcscmp( cFieldData[i], _T("GM") ) == 0 ) {
						// �Ϲ��ڷ�
						m_nUpdateCollectionType = _KN_DAN_GM_;
						_tcscpy(szpCollType, _T("dan-gm"));
					} else if ( _tcscmp( cFieldData[i], _T("RB") ) == 0 ) {
						// ��
						m_nUpdateCollectionType = _KN_DAN_RB_;
						_tcscpy(szpCollType, _T("dan-rb"));
					} else if ( _tcscmp( cFieldData[i], _T("CA") ) == 0 ) {
						// ��ȭ
						m_nUpdateCollectionType = _KN_DAN_CA_;
						_tcscpy(szpCollType, _T("dan-ca"));
					} else if ( _tcscmp( cFieldData[i], _T("HA") ) == 0 ) {
						// �н���
						m_nUpdateCollectionType = _KN_DAN_HA_;
						_tcscpy(szpCollType, _T("dan-ha"));
					} else if ( _tcscmp( cFieldData[i], _T("HR") ) == 0 ) {
						// 
						m_nUpdateCollectionType = _KN_DAN_HR_;
						_tcscpy(szpCollType, _T("dan-hr"));
					} else {
						// ��Ÿ
						m_nUpdateCollectionType = _KN_DAN_ETC_;
						_tcscpy(szpCollType, _T("dan-etc"));
					} 
				}
			}
		}
	} 
	
	// 2.3.4���� : ������.
	if ( m_nUpdateCollectionType == -1 ) 
	{		
		m_nUpdateCollectionType = _KN_DAN_GM_;		
		_tcscpy(szpCollType, _T("dan-gm"));
	}
	
	// 2008.07.10 ADD BY PDJ : Collection Type�߰�.
	TCHAR    *pszFieldName[1] = { _T("COLL_TYPE") };
	ids = Add_S97_OneField( 1, &pszFieldName[0], &szpCollType );
	if ( ids ) return ids;
	
    //===========================================================
    // 3. CreateIndex Class�κ��� �������� �׸�. (�Ϲ��׸�)
    //    �з���ȣ���� �ʵ带 �����Ѵ�.
	//===========================================================   	
	ids = Make_S97_CreateIndex(f_icDispAlias , cRegCode   );	
    if ( ids ) {
        Init_S97_SpeciesTable( );
        return ids;
    }
    //===========================================================
	// 2. ���������� ���Ѵ�.
    //    ( ����ǥ������, ���������ڻ���, ���ι����ڻ���, ���ι���⵵���� )
    //    049���� �ʵ带 �����Ѵ�.
	//===========================================================	
	ids = Make_S97_SimpleInfo( f_icDispAlias, cRegCode );	
    if ( ids ) {
        Init_S97_SpeciesTable( );
        return ids;
    }

    //===========================================================
    // 5. ��Ÿ�� �Է��Ѵ�.( �谡��, ������, sort��ǥ�� )
	//===========================================================	
    ids = Make_S97_EtcInfo(  f_icDispAlias );
    if ( ids ) {
        Init_S97_SpeciesTable( );
        return ids;
    }

    m_nSEARCH97_RecordCnt++;

    if ( f_ibCommit ) {
    }

    return 0;

}



INT CSearchMake::Add_S97_OneField(
                                                INT      f_inFieldCnt ,
                                                TCHAR   **f_icFieldName,
                                                TCHAR   **f_icFieldData,
                                                INT      f_nDMLType
                                      )
{

    INT i;

    if ( f_nDMLType == _DELETE ) {
        for ( i = 0; i < f_inFieldCnt; i++ ) {
            if ( f_icFieldData[i] != NULL && _tcsclen( f_icFieldData[i] ) > 0 ) {
                m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].pTag = ( tTAG_STRU * )realloc( m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].pTag, sizeof(tTAG_STRU) * ( m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].TagCnt + 1 ) );
                if ( m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].pTag == NULL ) {
                    Init_S97_SpeciesTable( );
                    return -8;
                }

                m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].pTag[m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].TagCnt].DataLen = _tcsclen( f_icFieldName[i] ) + _tcsclen( f_icFieldData[i] ) + 2;
                m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].pTag[m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].TagCnt].pData   = ( TCHAR * )malloc( (m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].pTag[m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].TagCnt].DataLen+1) * sizeof(TCHAR) );
                if ( m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].pTag[m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].TagCnt].pData == NULL ) {
                    Init_S97_SpeciesTable( );
                    return -9;
                }

                _stprintf( m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].pTag[m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].TagCnt].pData, _T("%s: %s"), f_icFieldName[i], f_icFieldData[i] );
    //   ����� ++
    //            debug( m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].pTag[m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].TagCnt].pData );
    // --
                m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].TagCnt++;
            }
        }
    } else {
        for ( i = 0; i < f_inFieldCnt; i++ ) {
            if ( f_icFieldData[i] != NULL && _tcsclen( f_icFieldData[i] ) > 0 ) {
                m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].pTag = ( tTAG_STRU * )realloc( m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].pTag, sizeof(tTAG_STRU) * ( m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].TagCnt + 1 ) );
                if ( m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].pTag == NULL ) {
                    Init_S97_SpeciesTable( );
                    return -8;
                }

                m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].pTag[m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].TagCnt].DataLen = _tcsclen( f_icFieldName[i] ) + _tcsclen( f_icFieldData[i] ) + 2;
                m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].pTag[m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].TagCnt].pData   = ( TCHAR * )malloc( (m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].pTag[m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].TagCnt].DataLen+1) * sizeof(TCHAR) );
                if ( m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].pTag[m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].TagCnt].pData == NULL ) {
                    Init_S97_SpeciesTable( );
                    return -9;
                }
/*
                if ( _tcsclen( f_icFieldData[i] ) > 1950 ) {
                    TCHAR Msg[256];
                    _stprintf( Msg, _T("%s�� �ʹ� ��� ������ �� �����ϴ�.\n �ش� Tag�� 1950����Ʈ ���Ϸ� ���� �Ͻ� ���� �ٽ� �����Ͻʽÿ�"), f_icFieldName[i] );
                    AfxMessageBox( Msg );
                    return 1000;
                }
*/
                _stprintf( m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].pTag[m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].TagCnt].pData, _T("%s: %s"), f_icFieldName[i], f_icFieldData[i] );
    //  ����� ++
    //            debug( m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].pTag[m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].TagCnt].pData );
    // --
                m_tSEARCH97_RecordStru[m_nSEARCH97_RecordCnt].TagCnt++;
            }
        }
    }

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////
//
// SEARCH97�� ���� STRUCTURE ����.
//
//////////////////////////////////////////////////////////////////////////////////////

#define     nAllAppendCount    26

INT CSearchMake::Make_S97_CreateIndex(
                                                CString   f_icDispAlias ,
                                                TCHAR     *szCollType   
                                              )
{  

    TCHAR    *cpCreateIndexFieldName[nAllAppendCount] = {  
                _T("TITLE"), _T("KEYWORD"), _T("AUTHOR"), _T("PUBLISHER"),
                _T("PUB_YEAR"), _T("DEG_CODE"), _T("TEXT_LANG"), _T("SUM_LANG"), 
				_T("UNI_CODE"), _T("GOV_CODE"), _T("SUBJECT"),
                _T("KDC_CLASS"), _T("DDC_CLASS"), _T("KDCP_CLASS"), _T("CEC_CLASS"), _T("CWC_CLASS"), _T("COC_CLASS"),
				_T("UDC_CLASS"), _T("NDC_CLASS"), _T("LC_CLASS"),
                _T("ST_CODE"), _T("ST_ISSN"), _T("ST_STRN"), _T("ST_RNSTRN"), _T("ST_CBN"), _T("ST_CAN") 
    };

    
    CString temp_Create[nAllAppendCount] = {
		         _T("��ǥ��"),_T("Ű����"),_T("������"),_T("������"),
				 _T("�����"),_T("�������а�"),_T("�������"),_T("��๮���"),
				 _T("�ѱ����к�ȣ"),_T("���α����ȣ"),_T("����������"),
                 _T("KDC"),_T("DDC"),_T("KDCP"),_T("CEC"),_T("CWC"),_T("COC"),_T("UDC"),_T("NDC"),_T("LC"),
				 _T("ǥ�غ�ȣ_ISBN"),_T("ǥ�غ�ȣ_ISSN"),_T("ǥ�غ�ȣ_STRN"),_T("ǥ�غ�ȣ_RNSTRN"),
				 _T("ǥ�غ�ȣ_����������ȣ"),_T("ǥ�غ�ȣ_���۱ǵ�Ϲ�ȣ")

	}; //29

    TCHAR           *cIndexData ;
	INT             i, ids;

    //===========================================================
    // 1. �Լ� �ʱ�ȭ.
	//===========================================================

  //  CIndexTool indexTool;   

	ids = pMarcMgr->Decoding (tmp_Stream,&pMarc) ;
	m_pIndexTool->Init(pMarcMgr, &pMarc);

 
    //===========================================================
    // 2. CreateIndex Class�κ��� �������� �׸�. (�Ϲ��׸�)
	//===========================================================
  
	
	for( i = 0; i < nAllAppendCount; i++ ) 
	{	
        CString strIndexItem;
      
		ids = m_pIndexTool->GetIndex(temp_Create[i] , strIndexItem );
        if(ids <0) {
			continue ;
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if(temp_Create[i] == _T("�����"))
		{   
			if(strIndexItem == _T(""))
			{
				pMarcMgr->GetItem(&pMarc, _T("008@7-10"), strIndexItem);
            }
		}

		// 2008.07.09 ADD BY PDJ : ���� ���� ����
		if ( cpCreateIndexFieldName[i] == _T("TITLE") )
		{
			strIndexItem.Replace(_T(" "), _T(" �� "));
			strIndexItem = _T("�� ") + strIndexItem + _T(" ��");
		}
		else if ( cpCreateIndexFieldName[i] == _T("KEYWORD")	||
				  cpCreateIndexFieldName[i] == _T("AUTHOR")		||
				  cpCreateIndexFieldName[i] == _T("PUBLISHER")	||
				  cpCreateIndexFieldName[i] == _T("SUBJECT")	)
		{
			strIndexItem.Replace(_T(" "), _T(" �� "));
			strIndexItem = _T("�� ") + strIndexItem + _T(" ��");
		}

       
        cIndexData = strIndexItem.GetBuffer (0);
        ids = Add_S97_OneField( 1, &cpCreateIndexFieldName[i], &cIndexData );
        if ( ids ) return ids;
	}

   
    return 0;

}




//===========================================================
// 2. ���������� ���Ѵ�.
//    ( ����ǥ������, ���������ڻ���, ���ι����ڻ���, ���ι���⵵���� )
//===========================================================
 
#define     nSimpleInfoField    5 
INT CSearchMake::Make_S97_SimpleInfo(
                                                CString f_icDispAlias,
                                                TCHAR     *szCollType  
                                              )
{

    TCHAR    *cpCreateIndexFieldName[5] = 
	{  
       _T("TITLE_INFO"), _T("PUB_INFO"), _T("AUTHOR_INFO"), _T("PUB_YEAR_INFO"),_T("IPUB_YEAR")
    };
	
    TCHAR           *cData;
	INT             i,  ids;
    
    CString tmp_Create[5] = { _T("ǥ������"),_T("�����ڻ���"),_T("�����ڻ���"),_T("����⵵����"),_T("�����˻������⵵")};
   
	ids = pMarcMgr->Decoding (tmp_Stream,&pMarc) ;
	m_pIndexTool->Init(pMarcMgr, &pMarc);

    //===========================================================
    //  ����,����,������,�����,�����˻������⵵ ������ ���Ѵ�.
    //===========================================================
	CString strIndexItem;
    for ( i = 0; i < nSimpleInfoField; i++ ) 
	{           
		ids = m_pIndexTool->GetIndex(tmp_Create[i] , strIndexItem );
		if(ids <0) 
		{
			continue ;
		}	
		
		cData = strIndexItem.GetBuffer(0);
        ids = Add_S97_OneField( 1, &cpCreateIndexFieldName[i], &cData );
        if ( ids ) return ids; 		
    }

    return 0;
}



//===========================================================
// 2. ��Ÿ ������ ���Ѵ�.
//    ( �����ȣ, �����˻��� ��ǥ��, �����˻��� ������, �����,CIP�����ȣ )
//===========================================================

INT CSearchMake::Make_S97_EtcInfo( CString   f_icDispAlias)
{
   
   
	TCHAR    *cpRSCreateIndexFieldName[nAllAppendCount] = {  
			//2003-02-25 [������]================================================
		    //_T("SORT_TITLE"), _T("CONTROL_NO"), _T("TEMP"), _T("IPUBLISHER"), _T("CIP_CONTROL_NO")
		    _T("SORT_TITLE"), _T("CONTROL_NO"), _T("PUB_FREQ"), _T("IPUBLISHER"), _T("CIP_CONTROL_NO")
	};


	INT             i, ids;
	TCHAR           *cData;

    //2003-02-25 [������]========================================================
    CString tmp_Create[5] = { _T("�����˻��뺻ǥ��"), _T("��Ÿ��ȣ_�����ȣ"),_T("�����"),
		  _T("�����˻��������"),_T("CIP�����ȣ")	          
	};


  //  CIndexTool indexTool;   
	ids = pMarcMgr->Decoding (tmp_Stream,&pMarc) ;
	m_pIndexTool->Init(pMarcMgr, &pMarc);
     
 
    for(i =0 ; i < 5 ; i++)
	{
    
    CString strIndexItem;  
	strIndexItem.Empty () ;
	if( i == 5 )
	{
	     pMarcMgr->GetItem (&pMarc,_T("023$a"),strIndexItem);
	}
	else 
	{
	ids = m_pIndexTool->GetIndex(tmp_Create[i] , strIndexItem );
	if(ids <0) {
	   continue ;
	   }
	}    
    
	cData = strIndexItem.GetBuffer (0);	
	ids = Add_S97_OneField( 1, &cpRSCreateIndexFieldName[i], &cData );
	if ( ids ) return ids; 
	//strIndexItem.ReleaseBuffer();

	}
    return 0;

}


//===========================================================
// 3. 049,052 �ʵ� ���� 
//   
//===========================================================

#define     n049_052InfoField   5 
INT CSearchMake::Make_S97_049_052Info( CString f_icDispAlias)
{

    TCHAR    *cpCreateIndexFieldName[nAllAppendCount] = {  
                _T("SE_SHELF_CODE"), _T("VOL_CODE"), _T("CLASS_NO"),
                _T("BOOK_CODE"), _T("VOL_CODE_DISP"),
    };
   
	INT       i, ids;
    TCHAR      *cData ;     

    CString tmp_Create[n049_052InfoField] = {_T("��Ÿ��ȣ_û����ȣ_��ġ��ȣ"),_T("��Ÿ��ȣ_û����ȣ_��å��ȣ"),
                _T("��Ÿ��ȣ_û����ȣ_�η���ȣ"),_T("��Ÿ��ȣ_û����ȣ_������ȣ"),
				_T("��Ÿ��ȣ_û����ȣ_��å��ȣ")
				};

    
//	CIndexTool indexTool;   
	ids = pMarcMgr->Decoding (tmp_Stream,&pMarc) ;
	m_pIndexTool->Init(pMarcMgr, &pMarc);
     
 
    for(i =0 ; i < 5 ; i++)
	{
    
		CString strIndexItem;  
		strIndexItem.Empty () ;

		ids = m_pIndexTool->GetIndex(tmp_Create[i] , strIndexItem );
		if(ids <0) {
		return -222 ;}
		

		cData = strIndexItem.GetBuffer (0);
		ids = Add_S97_OneField( 1, &cpCreateIndexFieldName[i], &cData );
		if ( ids ) return ids; 

		//strIndexItem.ReleaseBuffer();

	}
    
    return 0;

}


INT CSearchMake::Delete_S97_SpeciesTable(
                                                CString      f_icClassType  ,
                                                CString      f_icSpeciesKey ,
                                                bool       f_ibCommit
                                              )
{


    TCHAR           *cFieldName[1] = { _T("VDKVGWKEY") };
//    INT             nClassType;

    if ( f_icSpeciesKey.IsEmpty () || _ttoi(f_icSpeciesKey.GetBuffer (0)) <= 0 ) {
        Init_S97_SpeciesTable( );
        return -1;
    }

   
		if      ( _T("MO")   == f_icClassType ) {
			m_nUpdateCollectionType = _KN_DAN_GM_;
		}
		else if ( _T("SE") == f_icClassType ) {
			m_nUpdateCollectionType = _KN_YON_;
		}
		else {
			Init_S97_SpeciesTable( );
			return -2;
		}

    if ( f_ibCommit ) {
		m_tSEARCH97_DelRecordStru   = NULL;
		m_nSEARCH97_DelRecordCnt       = 0;
	}


    //===========================================================
    // 7. SEARCH97�� ���� STRUCTURE ����.
	//===========================================================
    m_tSEARCH97_DelRecordStru = ( tRECORD_STRU * )realloc( m_tSEARCH97_DelRecordStru, sizeof( tRECORD_STRU ) * (m_nSEARCH97_DelRecordCnt+1) );
    if ( m_tSEARCH97_DelRecordStru == NULL ) {
        Init_S97_SpeciesTable( );
        return -6;
    }

    m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].TagCnt = 0;
    m_tSEARCH97_DelRecordStru[m_nSEARCH97_DelRecordCnt].pTag   = NULL;
    

	TCHAR *Temp_f_icSpeciesKey;

	Temp_f_icSpeciesKey = new TCHAR[f_icSpeciesKey.GetLength()+1];
	_tcscpy(Temp_f_icSpeciesKey,f_icSpeciesKey.GetBuffer (0)) ;
    Add_S97_OneField( 1, cFieldName, &Temp_f_icSpeciesKey , _DELETE );
	//f_icSpeciesKey.ReleaseBuffer();

    m_nSEARCH97_DelRecordCnt++;

    if ( f_ibCommit ) {

    }

	// KOL.UDF.022 ��ť���ڵ� ����
	//delete Temp_f_icSpeciesKey;
	delete[] Temp_f_icSpeciesKey;

    return 0;

}