// IndexDataMgr.cpp: implementation of the CIndexDataMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndexDataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("������API"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndexDataMgr::CIndexDataMgr(CESL_Mgr* pParent /*=NULL*/): CESL_Mgr(IDD, pParent)
{
	m_pParentMgr = pParent;
	m_pIndexDM = NULL;
	m_bDMCreate = FALSE;


	nIndexSpeciesRelation = 0;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_���ı���_�ڵ�");		nIndexSpeciesRelation++;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_��������_�ڵ�");		nIndexSpeciesRelation++;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_������ϱ���_�ڵ�");	nIndexSpeciesRelation++;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_�����ڷᱸ��_�ڵ�");	nIndexSpeciesRelation++;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_�з�ǥ����");			nIndexSpeciesRelation++;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_�ڷᱸ��_�ڵ�");		nIndexSpeciesRelation++;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_���");					nIndexSpeciesRelation++;
	
}

CIndexDataMgr::~CIndexDataMgr()
{
	if (m_pIndexDM && m_bDMCreate) 
	{
		delete m_pIndexDM;
		m_pIndexDM = NULL;
	}
}

INT CIndexDataMgr::ExtractIndexInMarc(CString strMarc, CStringArray &strAliasArray)
{
	INT nRowIdx, ids, i, nIndexToolKit;
	CString strData, strIdxTitle, strIdxKeyword, strIdxAuthor, strIdxPublisher, strIdxAllItem =_T("");
	CString IndexToolKit[16], IndexToolKitCommand[16];
	CStringArray TargetArray;
	CMarc Marc;

	strAliasArray.RemoveAll();
	m_MarcDataArray.RemoveAll();
	pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &Marc);		
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->Init(m_pParentMgr->m_pInfo->pMarcMgr, &Marc);
	
	nIndexToolKit = 0;	
	IndexToolKit[nIndexToolKit] = _T("IBS_�˻��뺻ǥ��");			IndexToolKitCommand[nIndexToolKit] = _T("��ǥ��");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�˻���Ű����");			IndexToolKitCommand[nIndexToolKit] = _T("Ű��������");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�˻���������");			IndexToolKitCommand[nIndexToolKit] = _T("������");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�˻��������");			IndexToolKitCommand[nIndexToolKit] = _T("������");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_ǥ������");				IndexToolKitCommand[nIndexToolKit] = _T("ǥ������");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�����ڻ���");				IndexToolKitCommand[nIndexToolKit] = _T("�����ڻ���");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�����ڻ���");				IndexToolKitCommand[nIndexToolKit] = _T("�����ڻ���");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_����⵵����");			IndexToolKitCommand[nIndexToolKit] = _T("����⵵����");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�˻������Ŀ뺻ǥ��");		IndexToolKitCommand[nIndexToolKit] = _T("�����˻��뺻ǥ��");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�˻�������˻�������");	IndexToolKitCommand[nIndexToolKit] = _T("�����˻��������");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�����˻������⵵");		IndexToolKitCommand[nIndexToolKit] = _T("�����˻������⵵");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_��������");				IndexToolKitCommand[nIndexToolKit] = _T("��������");			nIndexToolKit++;	
	
	m_pIndexDM->FreeData();

	ids = InsertRow(-1);
	if( ids < 0 )	return ids;
	
	nRowIdx = m_pIndexDM->GetRowCount()-1;
	for( i=0; i<nIndexToolKit; i++ ) 
	{
		strData.Empty();
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(IndexToolKitCommand[i], strData);
		m_pIndexDM->SetCellData(IndexToolKit[i], strData, nRowIdx);
		strAliasArray.Add(IndexToolKit[i]);
		m_MarcDataArray.Add(strData);
	}	


	m_pIndexDM->GetCellData(_T("IBS_�˻��뺻ǥ��"), nRowIdx, strIdxTitle);
	m_pIndexDM->GetCellData(_T("IBS_�˻���������"), nRowIdx, strIdxAuthor);
	m_pIndexDM->GetCellData(_T("IBS_�˻��������"), nRowIdx, strIdxPublisher);
	m_pIndexDM->GetCellData(_T("IBS_�˻���Ű����"), nRowIdx, strIdxKeyword);

	strData.Empty();
	m_pIndexDM->GetCellData(_T("IBS_������"), nRowIdx, strData);
	if( strData.IsEmpty() == TRUE ) 
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&Marc, _T("245$d"), strData);
		DeleteUnlessWord(strData);
		m_pIndexDM->SetCellData(_T("IBS_������"), strData, nRowIdx);		
		strAliasArray.Add(_T("IBS_������"));
		m_MarcDataArray.Add(strData);
	}

	strData.Empty();
	m_pIndexDM->GetCellData(_T("IBS_��ǥ��"), nRowIdx, strData);
	if( strData.IsEmpty() == TRUE )
	{
		//===================================================
		//2010.06.23 UPDATE BY PJW : IDX��ǥ���� �ƴ� ǥ�������� �߰��Ѵ�.
//		m_pIndexDM->SetCellData(_T("IBS_��ǥ��"), strIdxTitle, nRowIdx);
//		m_pIndexDM->GetCellData(_T("IBS_ǥ������"), nRowIdx, strData);
		CString str245$x = _T("");
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&Marc, _T("245$a"), strData);
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&Marc, _T("245$x"), str245$x);
		if( !str245$x.IsEmpty() )
		{
			strData = strData + _T("=") + str245$x;
		}
		
		m_pIndexDM->SetCellData(_T("IBS_��ǥ��"), strData, nRowIdx);
		//===================================================		
		strAliasArray.Add(_T("IBS_��ǥ��"));
		m_MarcDataArray.Add(strIdxTitle);
	}

	strData.Empty();
	m_pIndexDM->GetCellData(_T("IBS_������"), nRowIdx, strData);
	if( strData.IsEmpty() == TRUE )
	{
		m_pIndexDM->SetCellData(_T("IBS_������"), strIdxPublisher, nRowIdx);
		strAliasArray.Add(_T("IBS_������"));
		m_MarcDataArray.Add(strIdxPublisher);
	}

	strData.Empty();
	m_pIndexDM->GetCellData(_T("IBS_�����"), nRowIdx, strData);
	if( strData.IsEmpty() == TRUE )
	{
		m_pIndexDM->GetCellData(_T("IBS_����⵵����"), nRowIdx, strData);
		m_pIndexDM->SetCellData(_T("IBS_�����"), strData, nRowIdx);
		strAliasArray.Add(_T("IBS_�����"));
		m_MarcDataArray.Add(strData);
	}

	
	// IDX_ALL_ITEM�ʵ� ���� - �˻��뺻ǥ��, �˻�������, �˻��������, �˻���Ű����
	strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublisher + _T(" ") + strIdxKeyword;
	TargetArray.RemoveAll();
	ParsingString(_T(" "), strIdxAllItem, &TargetArray);
	strIdxAllItem.Empty();
	for( i=0 ; i<TargetArray.GetSize(); i++ )
	{
		if( strIdxAllItem.IsEmpty() )	strIdxAllItem = TargetArray.GetAt(i);
		else strIdxAllItem += _T(" ") + TargetArray.GetAt(i);
	}
	m_pIndexDM->SetCellData(_T("IBS_��ü�˻�����ξ�"), strIdxAllItem, nRowIdx);
	strAliasArray.Add(_T("IBS_��ü�˻�����ξ�"));
	m_MarcDataArray.Add(strIdxAllItem);
	
	//DB ���� �� ���� �Լ� �߰�.
	//RestructIndex(&m_AliasArray);
	return 0;
}

INT CIndexDataMgr::SetDataMgr(CESL_DataMgr *pDM)
{
	if(m_pParentMgr == NULL) return -1;
	if(pDM == NULL)
	{
		m_pIndexDM = new CESL_DataMgr;
		if(m_pIndexDM == NULL) return -1;
		m_pIndexDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
		m_pIndexDM->InitDMPKFromFile(_T("DMUP_����_��_��������"));
		m_pIndexDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
		m_bDMCreate = TRUE;
	}
	else
	{
		m_pIndexDM = pDM;
	}
	return 0;
}

INT CIndexDataMgr::SetRecKey(CString strRecKey, INT nRowIdx)
{
	INT ids = -1;
	ids = m_pIndexDM->SetCellData(_T("IBS_REC_KEY"), strRecKey, nRowIdx);
	if(ids < 0) return -1;

	return 0;
}
INT CIndexDataMgr::INDEX_to_SPECIES(CStringArray &strAlias, INT nRowIdx)
{
	INT ids;
	CString strData;

	for (INT i = 0; i < nIndexSpeciesRelation; i++) {
		strData = IndexSpeciesRelation_INDEX[i];
		ids = m_pIndexDM->GetCellData(strData, nRowIdx, strAlias[i]);
		if(ids < 0) return -1;
	}
	return 0;
}

INT CIndexDataMgr::InsertRow(INT nIdx)
{
	if(m_pIndexDM == NULL) return -1;
	return m_pIndexDM->InsertRow(nIdx);
}

INT CIndexDataMgr::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray, BOOL bIndexing/*=TRUE*/)
{
	INT pos;
	CString strTmpData;

	if( strSource.GetLength() == 0 )	return 0;

	strSource.TrimLeft();	strSource.TrimRight();
	pos = strSource.Find(DelimeterStr, 0);	
	while(pos >= 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+DelimeterStr.GetLength());
		strSource.TrimLeft();	strSource.TrimRight();

		if( bIndexing )		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);

		if( !bIndexing || !CheckIsHave(pTargetArray, strTmpData) )		pTargetArray->Add(strTmpData);

		pos = strSource.Find(DelimeterStr, 0);
	}

	if( bIndexing )		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if( !bIndexing || !CheckIsHave(pTargetArray, strSource) )		pTargetArray->Add(strSource);

	return 0;
}

BOOL CIndexDataMgr::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;
}

INT CIndexDataMgr::RestructIndexInMarc(CStringArray *pMarcAliasArray)
{
	INT nIds, i, nColIndex;	
	CString strColName, strAlias, strData;
	CString strFieldAlias, strFieldName, strType, strDataType;

/*	nIds = m_pIndexDM->StartFrame();
	if(nIds < 0) return -1;

	m_pIndexDM->InitDBFieldData();*/

	for( i=0; i<pMarcAliasArray->GetSize(); i++ )
	{
		strAlias = pMarcAliasArray->GetAt(i);
		nColIndex = m_pIndexDM->FindColumn(strAlias);
		if( nColIndex == -1 )	return -1;

		nIds = m_pIndexDM->FindColumnFieldName(nColIndex, strFieldAlias, strFieldName, strType, strDataType, 0, strData);
		if( nIds < 0 )			continue;
		//============================================
		//20080611 UPDATE BY PJW : �ҹ��ڴ빮�ڵ��� �����Ƿ� �ҹ��ڷ� �ٲٵ��� �����Ѵ�.
		//strType.MakeUpper();	strDataType.MakeUpper();
		strType.MakeLower(); strDataType.MakeLower();
		//============================================
		
		if( strDataType == _T("udf") )		continue;

		if		( strType == _T("str") )	
		{
			m_pIndexDM->AddDBFieldData(strFieldName, _T("STRING"), strData);
		}
		else if	( strType == _T("int") ) 
		{
			if( strData.IsEmpty() == TRUE )		m_pIndexDM->AddDBFieldData(strFieldName, _T("NUMERIC"), _T("NULL"));
			else								m_pIndexDM->AddDBFieldData(strFieldName, _T("NUMERIC"), strData);
		}
		
	}
	return 0;
}

INT CIndexDataMgr::RestructIndexInSpecies(CString strRecKey, BOOL bDelete/*=FALSE*/)
{
	INT ids, i;
	CString strTbl, strQuery = _T("");
	CString strCodes[7] = {	_T("WORK_CODE"),
							_T("MAT_CODE"),
							_T("FORM_CODE"),
							_T("MEDIA_CODE"),
							_T("USE_OBJ_CODE"),
							_T("SUB_REG_CODE"),
							_T("CLASS_NO_TYPE")	};
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pIndexDM->CONNECTION_INFO);

	if( strRecKey.IsEmpty() == TRUE )	return -1;

	if( bDelete == TRUE )	
	{
		strTbl = _T("BO_DELETE_SPECIES_TBL");
		
		// �������� �߰��Ѵ�.
		CString strAddQuery = _T("");
		CString strResult	= _T("");
		strAddQuery.Format(_T("SELECT S.DELETE_DATE ")
						   _T("FROM BO_DELETE_SPECIES_TBL S ")
						   _T("WHERE S.REC_KEY = %s"), strRecKey);		
		m_pIndexDM->GetOneValueQuery(strAddQuery, strResult);
		m_pIndexDM->AddDBFieldData(_T("DELETE_DATE"), _T("STRING"), strResult);
		
	}
	else
	{
		strTbl = _T("BO_SPECIES_TBL");	
	}
	
	
	strQuery.Format(_T("SELECT WORK_CODE, MAT_CODE, FORM_CODE, MEDIA_CODE, USE_OBJ_CODE, SUB_REG_CODE, CLASS_NO_TYPE ")
					_T("FROM %s ")
					_T("WHERE REC_KEY = %s"), strTbl, strRecKey);
	ids = tmpDM.RestructDataManager(strQuery);
	if( ids < 0 )	return ids;

	for( i=0; i<7 ; i++ )
	{
		CString strData = _T("");
		tmpDM.GetCellData(0,i,strData);
		m_pIndexDM->AddDBFieldData(strCodes[i], _T("STRING"), strData);
	}

	return 0;	
}


INT CIndexDataMgr::RestructIndexInBook(CString strSpeciesKey)
{
	CString strData;

	//RestructIndexInBook
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'REG_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("REG_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("USE_LIMIT_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'MANAGE_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("MANAGE_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'VOL_INDEX')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_VOL"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_VOL_TITLE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("PLACE_INFO"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_SE_SHELF_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'EA_ISBN')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("ST_CODE"), _T("NUMERIC"), strData);
	//IDX_HOLD_PLACE, MAIN_PLACE_INFO �߰�
	strData.Format(_T("GET_DISP_BO_BOOK_INFO(%s, 'SHELF_LOC_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_HOLD_PLACE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_MAIN_SHELF_CODE(%s)"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("MAIN_PLACE_INFO"), _T("NUMERIC"), strData);

	return 0;
}

INT CIndexDataMgr::RestructIndexInDeleteBook(CString strSpeciesKey)
{
	CString strData;

	//RestructIndexInBook
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'REG_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("REG_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'USE_LIMIT_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("USE_LIMIT_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'MANAGE_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("MANAGE_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'VOL_INDEX')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_VOL"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'VOL_TITLE_INDEX')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_VOL_TITLE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'SHELF_LOC_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("PLACE_INFO"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'SEPARATE_SHELF_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_SE_SHELF_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'EA_ISBN')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("ST_CODE"), _T("NUMERIC"), strData);
	//IDX_HOLD_PLACE, MAIN_PLACE_INFO �߰�
	strData.Format(_T("GET_DISP_BO_DELETE_BOOK_INFO(%s, 'SHELF_LOC_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_HOLD_PLACE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_MAIN_SHELF_CODE(%s)"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("MAIN_PLACE_INFO"), _T("NUMERIC"), strData);

	return 0;
}

INT CIndexDataMgr::MakeInsert(CString strSpeciesKey)
{
	m_pIndexDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);

	INT nIds = m_pIndexDM->MakeInsertFrame(m_pIndexDM->TBL_NAME);
	if(nIds < 0) return nIds;

	return 0;
}


INT CIndexDataMgr::MakeUpdate(CString strSpeciesKey)
{
	INT nIds = m_pIndexDM->MakeUpdateFrame(m_pIndexDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey, 0);
	if(nIds < 0) return nIds;

	return 0;
}

INT CIndexDataMgr::SyncIndexWithSearchEngine(CString strSpeciesKey, CESL_DataMgr * pDM_OUT)
{
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	CString strQuery;
// 	strQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL ")
// 		_T("(SELECT esl_seq.NEXTVAL, rec_key, 'I', SYSDATE ")
// 		_T("FROM IDX_BO_TBL ")
// 					_T("WHERE REC_KEY=%s);"), strSpeciesKey);
// 	
// 	INT ids = pDM_OUT->AddFrame(strQuery);
// 	if(ids < 0)
// 	{
// 		ESLAfxMessageBox(_T("�˻��������� ����ȭ�� ���� ���� ���� ����"));
// 		return -1;
// 	}
	//=====================================================

	return 0;
}

INT CIndexDataMgr::SyncIndexWithECO_SEARCH(CString strSpeciesKey, CESL_DataMgr * pDM_OUT)
{
	CString strQuery;
	strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
	
	INT ids = pDM_OUT->AddFrame(strQuery);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("Oracle8i�� ECO_SEARCH���� ����ȭ�� ���� ���� ���� ����"));
		return -1;
	}
	
	return 0;
}

INT CIndexDataMgr::UpdateParentDM(CStringArray &strAliasArray, CESL_DataMgr * pParentDM, INT nParentDMIndex, CString strParentParam)
{
	CString strAlias;
	CString strData, strParentData;
	CString strFieldAlias, strFieldName, strType, strDataType;
	CString strParentAlias, strParentName, strParentType, strParentDataType;
	INT nColIndex = -1;
	INT nColCount = strAliasArray.GetSize();
	INT nFindParentFeild = -1;
	
	for(INT i=0; i < nColCount; i++)
	{
		strAlias = strAliasArray.GetAt(i);
		nColIndex = m_pIndexDM->FindColumn(strAlias);
		if( nColIndex < 0 )
			return -1;
		INT nIds = m_pIndexDM->FindColumnFieldName(nColIndex, strFieldAlias, strFieldName, strType, strDataType, 0, strData);
		if(nIds < 0) continue;
		
		INT nParentColCount = pParentDM->GetColCount();
		for(INT j=0; j < nParentColCount; j++)
		{
			INT nParentIds = pParentDM->FindColumnFieldName(j, strParentAlias, strParentName, strParentType, strParentDataType, 0, strParentData);
			if(nParentIds < 0) continue;
			INT IsIndexParam = strParentName.Find(strParentParam+_T("."));
			if(IsIndexParam < 0) continue;
			CString strIndexName = strParentName.Replace(strParentParam+_T("."), _T(""));
			if(strFieldName == strParentName)
			{
				pParentDM->SetCellData(strParentAlias, strData, nParentDMIndex);
				break;
			}
			//���� �̸��� field_Name�� ã�� ���� ���� HOW??
		}
		
	}
	
	return 0;

}

INT CIndexDataMgr::SetDefaultValue(INT nRowIdx, INT nOpenMode)
{
	if(!m_pIndexDM) return -1;

	CString worker = m_pParentMgr->m_pInfo->USER_ID;
	m_pIndexDM->SetCellData(_T("IBS_�۾���"), worker, nRowIdx);
	if(nOpenMode > 0)
	{
		m_pIndexDM->SetCellData(_T("IBS_�����۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_û��_������ȣ"), _T(""), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_û��_�з���ȣ"), _T(""), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_�����ȣ"), _T(""), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_������ȣINFO"), _T(""), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_�ڷ��ִ°�INFO_�ڵ�"), _T(""), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_�����а�"), _T("0"), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_û��_��å��ȣ"), _T(""), nRowIdx);
	}
	m_pIndexDM->SetCellData(_T("IBS_�������۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	return 0;
}

INT CIndexDataMgr::MakeInsertQuery(INT nRowIdx, CESL_DataMgr * pDM_OUT)
{
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	m_pIndexDM->InitDBFieldData();
	m_pIndexDM->MakeQueryByDM(RemoveAliasList, m_pIndexDM, m_pIndexDM->TBL_NAME, nRowIdx, pDM_OUT);
	return 0;
}

INT CIndexDataMgr::DeleteUnlessWord(CString &strData)
{
	CESL_CharSetMgr CharSetMgr_UnlessWord;
	CharSetMgr_UnlessWord.InitData(_T("���Ҿ�"), _T("���Ҿ�"));
	INT nFind = strData.ReverseFind(' ');
	if( nFind > 0 )
	{
		CString str1 = strData.Mid(nFind);
		CString str2 = str1;
		str1.TrimLeft();
		str1.TrimRight();
		
		if( CharSetMgr_UnlessWord.Find(str1, str2) == 0 )
		{
			strData.Delete(nFind, strData.GetLength() - nFind);
		}
	}

	return 0;
}