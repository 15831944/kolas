// ECO_SearchMgrApi.cpp : implementation file
//

#include "stdafx.h"
#include "ECO_SearchMgrApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECO_SearchMgrApi
#define ECO_SEARCH_FETCH_COUNT 1000
#define OPENBASE_IDX_ITEM_CNT    55
tINDEX_MAP_STRU INDEX_MAP_STRU[OPENBASE_IDX_ITEM_CNT] =
{
	{ _T("BO_REG_CODE"),			_T("REG_CD"), 			_T("MONO")	},
	{ _T("BO_USE_LIMIT_CODE"), 		_T("USE_LIMIT_CD"), 	_T("MONO")	},
	{ _T("BO_MANAGE_CODE"), 		_T("MANAGE_CD"), 		_T("MONO")	},
	{ _T("BO_IDX_TITLE"), 			_T("TITLE"), 			_T("MONO")	},
	{ _T("BO_IDX_KEYWORD"), 		_T("KEYWORD"), 			_T("MONO")	},
	{ _T("BO_IDX_AUTHOR"), 			_T("AUTHOR"), 			_T("MONO")	},
	{ _T("BO_IDX_PUBLISHER"), 		_T("PUBLISHER"),		_T("MONO")	},
	{ _T("BO_IDX_ALLITEM"), 		_T("ALL_ITEM"),			_T("MONO")	},
	{ _T("BO_IDX_VOL"), 			_T("VOL"),				_T("MONO")	},
	{ _T("BO_IDX_VOL_TITLE"), 		_T("VOL_TITLE"),		_T("MONO")	},
	{ _T("BO_IDX_PLACE"), 			_T("PLACE"), 			_T("MONO")	},
	{ _T("BO_IDX_HOLD_PLACE"), 		_T("HOLD_PLACE"),		_T("MONO")	},
	{ _T("BO_IDX_SE_SHELF_CODE"), 	_T("SE_SHELF_CD"), 		_T("MONO")	},
	{ _T("BO_ST_CODE"), 			_T("ST_CD"),			_T("MONO")	},
	{ _T("BO_ST_ISSN"), 			_T("ST_ISSN"),			_T("MONO")	},
	{ _T("BO_ST_STRN"), 			_T("ST_STRN"),			_T("MONO")	},
	{ _T("BO_ST_SNSTRN"), 			_T("ST_RNSTRN"),		_T("MONO")	},
	{ _T("BO_ST_CBN"), 				_T("ST_CBN"),			_T("MONO")	},
	{ _T("BO_ST_CAN"), 				_T("ST_CAN"),			_T("MONO")	},
	{ _T("BO_KDC_CLASS"), 			_T("KDC_CS"),			_T("MONO")	},
	{ _T("BO_DDC_CLASS"), 			_T("DDC_CS"),			_T("MONO")	},
	{ _T("BO_LC_CLASS"), 			_T("LC_CS"),			_T("MONO")	},
	{ _T("BO_BOOK_VOL"), 			_T("VOL_CD"),			_T("MONO")	},
	{ _T("SE_REG_CODE"), 			_T("REG_CD"),			_T("SERIAL")},
	{ _T("SE_USE_LIMIT_CODE"), 		_T("USE_LIMIT_CD"), 	_T("SERIAL")},
	{ _T("SE_MANAGE_CODE"), 		_T("MANAGE_CD"),		_T("SERIAL")},
	{ _T("SE_IDX_TITLE"), 			_T("TITLE"), 			_T("SERIAL")},
	{ _T("SE_IDX_KEYWORD"), 		_T("KEYWORD"),			_T("SERIAL")},
	{ _T("SE_IDX_AUTHOR"), 			_T("AUTHOR"), 			_T("SERIAL")},
	{ _T("SE_IDX_PUBLISHER"), 		_T("PUBLISHER"),		_T("SERIAL")},
	{ _T("SE_IDX_ALLITEM"), 		_T("ALL_ITEM"),			_T("SERIAL")},
	{ _T("SE_IDX_PLACE"), 			_T("PLACE"),			_T("SERIAL")},
	{ _T("SE_IDX_HOLD_PLACE"), 		_T("HOLD_PLACE"),		_T("SERIAL")},
	{ _T("SE_IDX_SE_SHELF_CODE"), 	_T("SE_SHELF_CD"),		_T("SERIAL")},
	{ _T("SE_ST_CODE"), 			_T("ST_CD"),			_T("SERIAL")},
	{ _T("SE_ST_ISSN"), 			_T("ST_ISSN"),			_T("SERIAL")},
	{ _T("SE_ST_STRN"), 			_T("ST_STRN"), 			_T("SERIAL")},
	{ _T("SE_ST_SNSTRN"), 			_T("ST_RNSTRN"),		_T("SERIAL")},
	{ _T("SE_ST_CBN"), 				_T("ST_CBN"),			_T("SERIAL")},
	{ _T("SE_ST_CAN"), 				_T("ST_CAN"),			_T("SERIAL")},
	{ _T("SE_ST_CODEN"), 			_T("ST_CODEN"),			_T("SERIAL")},
	{ _T("SE_KDC_CLASS"), 			_T("KDC_CS"),			_T("SERIAL")},
	{ _T("SE_DDC_CLASS"), 			_T("DDC_CS"), 			_T("SERIAL")},
	{ _T("SE_LC_CLASS"), 			_T("LC_CS"),			_T("SERIAL")},
	{ _T("BOTOC_IDX_TOC_KEYWORD"),	_T("TOC_KEYWORD"),		_T("BO_TOC")},
	{ _T("BOTOC_IDX_ABS_KEYWORD"), 	_T("ABS_KEYWORD"),		_T("BO_TOC")},
	{ _T("SETOC_IDX_ARTICLE"), 		_T("ARTICLE"),			_T("SE_TOC")},
	{ _T("SETOC_IDX_ARTICLE_AUTHOR"), 	_T("AUTHOR"),		_T("SE_TOC")},
	{ _T("SETOC_IDX_ARTICLE_KEYWORD"), 	_T("KEYWORD"),		_T("SE_TOC")},
	{ _T("SETOC_IDX_ARTICLE_TOC_KEYWORD"),_T("TOC_KEYWORD"),_T("SE_TOC")},
	{ _T("SETOC_IDX_ABS_KEYWORD"), 	_T("ABS_KEYWORD"),	 	_T("SE_TOC")},
	{ _T("SETOC_IDX_ALL_ITEM"), 	_T("ALL_ITEM"),			_T("SE_TOC")},
	{  _T("ECO$V_SETOCIDXTOCKEYWORD$I")	, _T("TOC_KEYWORD"),_T("SE_TOC")}, // ���� ������ ����
	{  _T("BOTOC_IDX_TOC_KEYWORD")		, _T("TOC_KEYWORD"),_T("BO_TOC")}, // ���� ��������
	{  _T("BOTOC_IDX_ABS_KEYWORD")		, _T("ABS_KEYWORD"),_T("BO_TOC")} // ���� �ʷ� ����

	/*
	{  _T("BO_IDX_TITLE")					, _T("TITLE")		,_T("MONO")		}, // ���� ���� ����
	{  _T("BO_IDX_AUTHOR")					, _T("AUTHOR")		,_T("MONO")		}, // ���� ���� ����
	{  _T("BO_IDX_PUBLISHER")				, _T("PUBLISHER")	,_T("MONO")		}, // ���� ������ ����
	{  _T("BO_IDX_KEYWORD")					, _T("KEYWORD")		,_T("MONO")		}, // ���� Ű���� ����
	{  _T("BO_REG_CODE")					, _T("ST_CD")		,_T("MONO")		}, // ���� Ű���� ����
	{  _T("BO_ST_CODE")						, _T("ST_CD")		,_T("MONO")		}, // ���� Ű���� ����
	{  _T("BO_IDX_ALLITEM")					, _T("ALL_ITEM")	,_T("MONO")		}, // ���� Ű���� ����
	{  _T("SE_IDX_TITLE")					, _T("TITLE")		,_T("SERIAL")	}, // ���� ���� ����
	{  _T("SE_IDX_AUTHOR")					, _T("AUTHOR")		,_T("SERIAL")	}, // ���� ���� ����
	{  _T("SE_IDX_PUBLISHER")				, _T("PUBLISHER")	,_T("SERIAL")	}, // ���� ������ ����
	{  _T("SE_IDX_ALLITEM")					, _T("ALL_ITEM")	,_T("SERIAL")	}, // ���� Ű���� ����
	{  _T("SETOC_IDX_ARTICLE_KEYWORD")		, _T("KEYWORD")		,_T("SE_TOC")	}, // ���� ������
	{  _T("ECO$V_SETOCIDXTOCKEYWORD$I")		, _T("TOC_KEYWORD")	,_T("SE_TOC")	}, // ���� ������ ����
	{  _T("BOTOC_IDX_TOC_KEYWORD")			, _T("TOC_KEYWORD")	,_T("BO_TOC")	}, // ���� ��������
	{  _T("BOTOC_IDX_ABS_KEYWORD")			, _T("ABS_KEYWORD")	,_T("BO_TOC")	} // ���� �ʷ� ����
	*/
};

CECO_SearchMgrApi::CECO_SearchMgrApi(CESL_DataMgr *pDM)
{
	m_pDM = pDM;
}

CECO_SearchMgrApi::~CECO_SearchMgrApi()
{

}

BEGIN_MESSAGE_MAP(CECO_SearchMgrApi, CWnd)
	//{{AFX_MSG_MAP(ECO_SearchMgrApi)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ECO_SearchMgrApi message handlers
BOOL CECO_SearchMgrApi::ECO_GetSearchKey(CString strCondition, CStringArray &strarraySearchCondition, CString strTables, CStringArray &strarrayTables, INT &nTotCnt, INT &nRemainCnt)
{	
	CString strConvertItem;
	CString strTotalCondition;
	CString strIndexKeyList;
	CString strSearchCondition;
	CString strJoinColumn;
	CString strBridge;	
	CString strPage;	
	BOOL bSucess;
	INT nPos = -1;
	INT nPos1, nPos2, nPos3;
	INT nOffset;
	INT	nIdxCnt;
	INT nTotalCount;
	INT nCurPage;
	INT nLastPage;
	INT	i;
	INT nTblCnt;

	BOOL bECOSearch;
	BOOL bMulticondition;	


	nTotCnt = 0;
	nRemainCnt = 0;
	bECOSearch = FALSE;
	bMulticondition = FALSE;

	CString strArg1, strArg2, strArg3;
	tQUERY_MAP_STRU QueryMapStru[50];
	tTABLES_MAP_STRU TablesMapStru[50];
	
	strTotalCondition= _T("");	
	// ECO Search -> Open Base�� ��ȯ
	nOffset = 0;
	nIdxCnt = -1;
	nTotalCount = 0;
	nCurPage = 0;
	nTblCnt = 0;
	strarraySearchCondition.RemoveAll();
	strarraySearchCondition.RemoveAll();

	//m_pDM->QueryLog(strCondition.GetBuffer(0));

	if ( m_pDM->MANAGE_CODE == _T("") )
	{
		m_pDM->CheckManageCode();
	}

	m_pDM->QueryLog(strCondition.GetBuffer(0), 0);
	
	// condition ��ȯ
	while ( TRUE )
	{
		nPos = strCondition.Find(_T("&ECOSEARCH("), nOffset);
		if (nPos == -1) break;
		
		bECOSearch = TRUE;
		
		nPos1 = strCondition.Find(',', nPos+11);
		if (nPos1 == -1) break;
		
		nPos2 = strCondition.Find(',', nPos1+1);
		if (nPos2 == -1) break;
		
		nPos3 = strCondition.Find(')', nPos2+1);
		if (nPos3 == -1) break;
		
		strArg1 = strCondition.Mid(nPos+11, nPos1-nPos-11);
		strArg1.TrimLeft();			strArg1.TrimRight();
		strArg2 = strCondition.Mid(nPos1+1, nPos2-nPos1-1);
		strArg2.TrimLeft();			strArg2.TrimRight();
		strArg3 = strCondition.Mid(nPos2+1, nPos3-nPos2-1);
		strArg3.TrimLeft();			strArg3.TrimRight();
		strArg3 = strArg3.Mid(1, strArg3.GetLength() - 2);
		strArg3.TrimLeft();			strArg3.TrimLeft();						
		
		
		bSucess = ECO_MakeItem(strArg1, strArg2, strArg3, strConvertItem);
		nOffset = nPos3;		

		if ( nIdxCnt > -1 ) 
		{
			strBridge.Format(_T("%s"),strCondition.Mid(QueryMapStru[nIdxCnt].nEndPos, nPos-QueryMapStru[nIdxCnt].nEndPos));
			strBridge.MakeUpper();
		}
		else
		{
			strBridge = _T("");
		}
		
		if ( strBridge.Find(_T("INTERSECT"),0) >= 0 )
		{			
			bMulticondition = TRUE;
		}
		else
		{
			nIdxCnt ++;
			QueryMapStru[nIdxCnt].nStartPos=0;
			QueryMapStru[nIdxCnt].nEndPos = 0;
			QueryMapStru[nIdxCnt].nPublishFormCode=_T("");		
			QueryMapStru[nIdxCnt].strJoinColumn=_T("");
			QueryMapStru[nIdxCnt].strQuery=_T("");
			QueryMapStru[nIdxCnt].strSearchKeyList.RemoveAll();
			strTotalCondition =  _T("");
			QueryMapStru[nIdxCnt].nStartPos = nPos;			
		}

		if ( bSucess ) 
		{			
			if ( !strTotalCondition.IsEmpty() ) strTotalCondition += _T("&");
			strTotalCondition += strConvertItem;
		}			
		QueryMapStru[nIdxCnt].nEndPos = nPos3+1;		
		QueryMapStru[nIdxCnt].strQuery = strTotalCondition;
		QueryMapStru[nIdxCnt].strJoinColumn = strArg1;
		QueryMapStru[nIdxCnt].nPublishFormCode = GetCollection(strArg2);
	}	

	// Binding variable
	CString strBindTemp;
	EDBM_BIND_DATA_TYPE* pBind = NULL;
	INT ids;
	
	CString strPublishFormCode;	
	if ( bECOSearch )
	{	
		BOOL bOption;
		bOption = FALSE;
		// ��ü �Ǽ��� ��ġ�ؼ� ������.
		for ( i=0 ; i<=nIdxCnt ; i++ ) // �Ѱ����� ������. // ���� + ������ �������� �ʰ� �Ѱ������� �ϴ°����� 
		{
			nCurPage = 1;
			nLastPage = 1;
			while ( nCurPage <= nLastPage  )
			{	
				// 2009.11.27 ADD BY PDJ : MANAGE_CODE������ �����Ѵ�.
				CString strManageCD;						
				INT nOffset;
				nOffset = strCondition.Find(_T("MANAGE_CODE='"),0);
				if ( nOffset < 0 ) nOffset = strCondition.Find(_T("MANAGE_CODE = '"),0);
				
				if ( nOffset >=0 ) 
				{					
					INT nEnd;
					nEnd=strCondition.Find(_T("'"), nOffset );
					if ( nEnd >= 0 ) 
					{
						nEnd=strCondition.Find(_T("'"), nEnd+1 );
						if ( nEnd >= 0 ) 
						{
							strManageCD = strCondition.Mid(nOffset, nEnd+1-nOffset);
							strManageCD.Replace(_T("MANAGE_CODE"), _T("MANAGE_CD"));
							strManageCD.Replace(_T("'"), _T(""));							
							strManageCD.Replace(_T(" "), _T(""));
						}
						else
						{
							strManageCD = _T("");
						}
					}
					else 
					{
						strManageCD = _T("");
					}
				}

				// ���� ����
				InitEDBMBindDataType( &pBind, 0, 3);	
				if ( QueryMapStru[i].nPublishFormCode == _T("MONO") )
				{				
					if ( strManageCD.IsEmpty() )
					{
						strSearchCondition.Format(_T("SELECT K3ADD.SEARCH.get_result(:IDX_TBL,:IDX_PAGE,'%d', UTL_RAW.CAST_TO_RAW(:IDX_KWD) ) FROM DUAL"), 
							ECO_SEARCH_FETCH_COUNT);

						strPage.Format(_T("%d"), nCurPage);
						AddEDBMBindDataByString(pBind, STRING, _T(":IDX_TBL"), _T("IDX_BO_TBL") );
						AddEDBMBindDataByString(pBind, STRING, _T(":IDX_PAGE"), strPage.GetBuffer(0));
						AddEDBMBindDataByString(pBind, STRING, _T(":IDX_KWD"), QueryMapStru[i].strQuery.GetBuffer(0) );
					}
					else
					{
						strSearchCondition.Format(_T("SELECT K3ADD.SEARCH.get_result(:IDX_TBL,:IDX_PAGE,'%d', UTL_RAW.CAST_TO_RAW(:IDX_KWD) ) FROM DUAL"), 
							ECO_SEARCH_FETCH_COUNT);

						strPage.Format(_T("%d"), nCurPage);
						strBindTemp.Format(_T("%s&%s"), QueryMapStru[i].strQuery, strManageCD);
						AddEDBMBindDataByString(pBind, STRING, _T(":IDX_TBL"), _T("IDX_BO_TBL") );
						AddEDBMBindDataByString(pBind, STRING, _T(":IDX_PAGE"), strPage.GetBuffer(0));
						AddEDBMBindDataByString(pBind, STRING, _T(":IDX_KWD"), strBindTemp.GetBuffer(0) );
					}
				}
				// ���� ����
				else if ( QueryMapStru[i].nPublishFormCode == _T("SERIAL") )
				{
					if ( strManageCD.IsEmpty() )
					{
						strSearchCondition.Format(_T("SELECT K3ADD.SEARCH.get_result(:IDX_TBL,:IDX_PAGE,'%d', UTL_RAW.CAST_TO_RAW(:IDX_KWD) ) FROM DUAL"), 
						ECO_SEARCH_FETCH_COUNT);

						strPage.Format(_T("%d"), nCurPage);
						AddEDBMBindDataByString(pBind, STRING, _T(":IDX_TBL"), _T("IDX_SE_TBL") );
						AddEDBMBindDataByString(pBind, STRING, _T(":IDX_PAGE"), strPage.GetBuffer(0));
						AddEDBMBindDataByString(pBind, STRING, _T(":IDX_KWD"), QueryMapStru[i].strQuery.GetBuffer(0) );
					}
					else
					{
						strSearchCondition.Format(_T("SELECT K3ADD.SEARCH.get_result(:IDX_TBL,:IDX_PAGE,'%d', UTL_RAW.CAST_TO_RAW(:IDX_KWD) ) FROM DUAL"), 
						ECO_SEARCH_FETCH_COUNT);

						strPage.Format(_T("%d"), nCurPage);
						strBindTemp.Format(_T("%s&%s"), QueryMapStru[i].strQuery, strManageCD);
						AddEDBMBindDataByString(pBind, STRING, _T(":IDX_TBL"), _T("IDX_SE_TBL") );
						AddEDBMBindDataByString(pBind, STRING, _T(":IDX_PAGE"), strPage.GetBuffer(0));
						AddEDBMBindDataByString(pBind, STRING, _T(":IDX_KWD"), strBindTemp.GetBuffer(0) );
					}					
					
				}
				// ���� ����
				else if ( QueryMapStru[i].nPublishFormCode == _T("BO_TOC") )
				{				
					strSearchCondition.Format(_T("SELECT K3ADD.SEARCH.get_result(:IDX_TBL,:IDX_PAGE,'%d', UTL_RAW.CAST_TO_RAW(:IDX_KWD) ) FROM DUAL"), 
						ECO_SEARCH_FETCH_COUNT);

					strPage.Format(_T("%d"), nCurPage);
					AddEDBMBindDataByString(pBind, STRING, _T(":IDX_TBL"), _T("IDX_BO_TOC_TBL") );
					AddEDBMBindDataByString(pBind, STRING, _T(":IDX_PAGE"), strPage.GetBuffer(0));
					AddEDBMBindDataByString(pBind, STRING, _T(":IDX_KWD"), QueryMapStru[i].strQuery.GetBuffer(0) );
				}
				// ���� ������
				else if ( QueryMapStru[i].nPublishFormCode == _T("SE_TOC") )
				{
					strSearchCondition.Format(_T("SELECT K3ADD.SEARCH.get_result(:IDX_TBL,:IDX_PAGE,'%d', UTL_RAW.CAST_TO_RAW(:IDX_KWD) ) FROM DUAL"), 
						ECO_SEARCH_FETCH_COUNT);

					strPage.Format(_T("%d"), nCurPage);
					AddEDBMBindDataByString(pBind, STRING, _T(":IDX_TBL"), _T("IDX_SE_TOC_TBL") );
					AddEDBMBindDataByString(pBind, STRING, _T(":IDX_PAGE"), strPage.GetBuffer(0));
					AddEDBMBindDataByString(pBind, STRING, _T(":IDX_KWD"), QueryMapStru[i].strQuery.GetBuffer(0) );
				}

				
				ids = m_pDM->GetOneValueQuery(strSearchCondition, strIndexKeyList, TRUE, pBind);	
				FreeEDBMBindData(&pBind, 2);
				if ( ids < 0 ) return FALSE;
				
				if ( strIndexKeyList == _T("0,1") ) 
				{
					strIndexKeyList= _T("-1");					
				}
				else
				{
					// ��ü �Ǽ�
					nPos1 = strIndexKeyList.Find(_T(","));
					nTotCnt = _ttoi(strIndexKeyList.Left(nPos1));
					// ��� ������
					nPos2 = strIndexKeyList.Find(_T(","),nPos1+1);
					nCurPage = _ttoi(strIndexKeyList.Mid(nPos1+1, nPos2-nPos1));
					// ���� key list
					strIndexKeyList = strIndexKeyList.Mid(nPos2+1);

					// ������ ������
					nLastPage = nTotCnt/ECO_SEARCH_FETCH_COUNT+1;			
				}

				QueryMapStru[i].strSearchKeyList.Add(strIndexKeyList);
				nCurPage ++;			
			}
		}
		// ���� ����
		
		CString strTmpQuery;
		CString strTmpTables;
		nCurPage = 0;
		// [Caution] QueryMapStru[0] : ��� ������ �����ϴٴ� ����
		for ( nCurPage = 0 ; nCurPage < QueryMapStru[0].strSearchKeyList.GetSize() ; nCurPage ++ )
		{
			strTmpQuery = strCondition;
			for ( i=0 ; i<=nIdxCnt ; i++ )
			{
				if ( QueryMapStru[nIdxCnt-i].strJoinColumn.IsEmpty() )
				{
					if ( bMulticondition )
					{
						strSearchCondition.Format(_T("%s %s %s"),
							strTmpQuery.Left(QueryMapStru[nIdxCnt-i].nStartPos),
							QueryMapStru[0].strSearchKeyList.GetAt(nCurPage),
							strTmpQuery.Mid(QueryMapStru[nIdxCnt-i].nEndPos));
					}
					else
					{
						strSearchCondition.Format(_T("%s ( %s ) %s"),
							strTmpQuery.Left(QueryMapStru[nIdxCnt-i].nStartPos),
							QueryMapStru[0].strSearchKeyList.GetAt(nCurPage),
							strTmpQuery.Mid(QueryMapStru[nIdxCnt-i].nEndPos));
					}
					
					strTmpQuery = strSearchCondition;
				}
				else
				{
					
					strSearchCondition.Format(_T("%s %s IN ( %s ) %s"),
						strTmpQuery.Left(QueryMapStru[nIdxCnt-i].nStartPos),
						QueryMapStru[nIdxCnt-i].strJoinColumn,
						QueryMapStru[0].strSearchKeyList.GetAt(nCurPage),
						strTmpQuery.Mid(QueryMapStru[nIdxCnt-i].nEndPos));
					strTmpQuery = strSearchCondition;
					
				}
			}

			// ��ȯ�� �� ������ Intersect�� and�� ��ü
			strTmpQuery.Replace(_T("INTERSECT"), _T("AND"));
			strarraySearchCondition.Add(strTmpQuery);	

			// ���������δ� ���յ� sql�� ����� �����ϹǷ�
			// �����ϱ� �� ġȯ�� key����Ʈ�� �����ϴ� ��ġ�� �̰��̹Ƿ�
			// ���⼭ �ڵ���.
			//
			// from���� �ٽ� ������ �ʰ� where������ ���°����� ��ü�ϴ� ���
			// �����ϴٰ� �Ǵ��Ѵ�.
			strTmpTables = strTables;
			nOffset = 0;
			BOOL bMultiTbl;			
			while ( TRUE )
			{
				nPos = strTmpTables.Find(_T("&ECOSEARCH("), nOffset);
				if (nPos == -1) break;

				bMultiTbl = FALSE;

				// tables���� ġȯ�� ���� ���ϱ�.
				// ecosearch �� ecosearch ���̿� intersect�� ���� ���
				// �ϳ��� ����.

				// �ι�° ���ʹ� ����Ȱ����� �������� �Ǵ��Ѵ�.
				if ( nTblCnt > 1 )
				{
					strBridge.Format(_T("%s"), strTmpTables.Mid(TablesMapStru[nTblCnt-1].nEndPos, nPos-TablesMapStru[nTblCnt-1].nEndPos));
					if ( strBridge.Find(_T("INTERSECT"),0) >= 0 )					
					bMultiTbl = TRUE;
				}

				if ( bMultiTbl )
				{					
					TablesMapStru[nTblCnt-1].nEndPos = strTmpTables.Find(')', nPos+1);
					if (TablesMapStru[nTblCnt-1].nEndPos == -1) break;
				}
				else
				{
					nTblCnt++;
					TablesMapStru[nTblCnt-1].nStartPos = nPos;
					TablesMapStru[nTblCnt-1].nEndPos = strTmpTables.Find(_T(")"), nPos+1);
					if (TablesMapStru[nTblCnt-1].nEndPos == -1) break;
					nOffset = TablesMapStru[nTblCnt-1].nEndPos + 1;
				}				
			}
			// replace QueryMapStru[0].strSearchKeyList.GetAt(nCurPage)
			// ECOSearch�� key list�� ġȯ
				// �ڿ��� ����.
			if ( nTblCnt > 0 )
			{
				for ( INT i=0 ; i<nTblCnt ; i++ )
				{
					CString strBegin;
					CString strEnd;
					CString strMid;
					
					strBegin = strTmpTables.Left(TablesMapStru[nTblCnt-i-1].nStartPos);
					strEnd = strTmpTables.Mid(TablesMapStru[nTblCnt-i-1].nEndPos);
					strMid = QueryMapStru[0].strSearchKeyList.GetAt(nCurPage);
					
					strTmpTables = strBegin + strMid + strEnd;	
				}
				
				strarrayTables.Add(strTmpTables);
			}
			//===================================================
			//2010.05.14 ADD BY PJW : �ѹ� ġȯ�� ������ �ʱ�ȭ �Ѵ�.
			nTblCnt = 0;
			//===================================================			
		}
	}
	else
	{
		strarraySearchCondition.Add(strCondition);		
		nTotCnt=1;
		nRemainCnt=1;
	}

	return bSucess;
}

CString CECO_SearchMgrApi::ECO_MakeQuery(CString strCondition, CString strResultKeyList)
{
	CString strSearchQuery;

	return strSearchQuery;
}

BOOL CECO_SearchMgrApi::ECO_MakeItem(CString strType, CString strItem, CString strValue, CString &strRetVal)
{	
	BOOL bFind;
	INT i;

	bFind = FALSE;
	strItem.MakeUpper();
	for ( i=0 ; i<OPENBASE_IDX_ITEM_CNT ; i++ )
	{
		if ( INDEX_MAP_STRU[i].SearchItem == strItem )
		{
			strItem = INDEX_MAP_STRU[i].OpenBaseItem;
			bFind = TRUE;
			break;
		}
	}

	if ( bFind )
	{		
		// ������
		strValue.Replace(_T("*"), _T(""));
		// or �˻���			
		strValue.Replace(_T("|"), _T(" | "));
		strRetVal.Format(_T("%s=%s"), strItem, strValue );
	}
	else
	{
		strRetVal = _T("");
	}

	return bFind;
}

CString CECO_SearchMgrApi::GetCollection(CString strIdxItem)
{
	CString strCollection;
	INT i;

	strIdxItem.MakeUpper();

	strCollection = _T("");
	for ( i=0 ; i<OPENBASE_IDX_ITEM_CNT ; i++ )
	{
		if ( INDEX_MAP_STRU[i].SearchItem == strIdxItem )
		{
			strCollection = INDEX_MAP_STRU[i].Collection;
			break;			
		}
	}

	return strCollection;
}