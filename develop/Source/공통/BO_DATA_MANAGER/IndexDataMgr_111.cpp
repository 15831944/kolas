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

/*INT CIndexDataMgr::ExtractIndex(CString strMarc, CString strSpeciesKey, INT nSelectIndex)
{
	m_strIndexKey = strSpeciesKey;
	m_nSelectIndex = nSelectIndex;
//	
//	m_strIndexKey->Format(_T("%s"), strSpeciesKey);
	if(ExtractIndexInMarc(strMarc) == -1) return -1;

	return 0;
}
*/
INT CIndexDataMgr::ExtractIndexInMarc(CString strMarc, CStringArray &strAliasArray)
{
	CMarc Marc;
	CString strIdxTitle, strIdxKeyword, strIdxAuthor, strIdxPublisher, strData;
	INT nRowIdx;

	m_AliasArray.RemoveAll();
	pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &Marc);		
	
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->Init(m_pParentMgr->m_pInfo->pMarcMgr, &Marc);
	
	m_pIndexDM->FreeData();

	INT ids = InsertRow(-1);
	if(ids < 0) return ids;
	
	nRowIdx = m_pIndexDM->GetRowCount()-1;

	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("��ǥ��"), strIdxTitle);
	m_pIndexDM->SetCellData(_T("IBS_�˻��뺻ǥ��"), strIdxTitle, nRowIdx);
	m_AliasArray.Add(_T("IBS_�˻��뺻ǥ��"));
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("Ű����"), strIdxKeyword);
	m_pIndexDM->SetCellData(_T("IBS_�˻���Ű����"), strIdxKeyword, nRowIdx);
	m_AliasArray.Add(_T("IBS_�˻���Ű����"));
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("������"), strIdxAuthor);
	m_pIndexDM->SetCellData(_T("IBS_�˻���������"), strIdxAuthor, nRowIdx);
	m_AliasArray.Add(_T("IBS_�˻���������"));
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("������"), strIdxPublisher);
	m_pIndexDM->SetCellData(_T("IBS_�˻��������"), strIdxPublisher, nRowIdx);
	m_AliasArray.Add(_T("IBS_�˻��������"));
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("ǥ������"), strData);
	m_pIndexDM->SetCellData(_T("IBS_ǥ������"), strData, nRowIdx);
	m_AliasArray.Add(_T("IBS_ǥ������"));
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("�����ڻ���"), strData);
	m_pIndexDM->SetCellData(_T("IBS_�����ڻ���"), strData, nRowIdx);
	m_AliasArray.Add(_T("IBS_�����ڻ���"));
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("�����ڻ���"), strData);
	m_pIndexDM->SetCellData(_T("IBS_�����ڻ���"), strData, nRowIdx);
	m_AliasArray.Add(_T("IBS_�����ڻ���"));
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("����⵵����"), strData);
	m_pIndexDM->SetCellData(_T("IBS_����⵵����"), strData, nRowIdx);
	m_AliasArray.Add(_T("IBS_����⵵����"));
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("�����˻��뺻ǥ��"), strData);
	m_pIndexDM->SetCellData(_T("IBS_�˻������Ŀ뺻ǥ��"), strData, nRowIdx);
	m_AliasArray.Add(_T("IBS_�˻������Ŀ뺻ǥ��"));
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("�����˻��������"), strData);
	m_pIndexDM->SetCellData(_T("IBS_�˻�������˻�������"), strData, nRowIdx);
	m_AliasArray.Add(_T("IBS_�˻�������˻�������"));
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("�����˻������⵵"), strData);
	m_pIndexDM->SetCellData(_T("IBS_�����˻������⵵"), strData, nRowIdx);
	m_AliasArray.Add(_T("IBS_�����˻������⵵"));
	
	// IDX_ALL_ITEM�ʵ� ���� - �˻��뺻ǥ��, �˻�������, �˻��������, �˻���Ű����
	CString strIdxAllItem =_T("");
	strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublisher + _T(" ") + strIdxKeyword;
	CStringArray TargetArray;
	TargetArray.RemoveAll();
	ParsingString(_T(" "), strIdxAllItem, &TargetArray);
	INT cnt = TargetArray.GetSize();
	strIdxAllItem.Empty();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strIdxAllItem.IsEmpty()) strIdxAllItem = TargetArray.GetAt(i);
		else strIdxAllItem += _T(" ") + TargetArray.GetAt(i);
	}
	m_pIndexDM->SetCellData(_T("IBS_��ü�˻�����ξ�"), strIdxAllItem, nRowIdx);
	m_AliasArray.Add(_T("IBS_��ü�˻�����ξ�"));
	
	//DB ���� �� ���� �Լ� �߰�.
	RestructIndex(&m_AliasArray);
	
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

INT CIndexDataMgr::InsertRow(INT nIdx)
{
	if(m_pIndexDM == NULL) return -1;
	return m_pIndexDM->InsertRow(nIdx);
}

INT CIndexDataMgr::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray, BOOL bIndexing/*=TRUE*/)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos >= 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+DelimeterStr.GetLength());
		if(bIndexing) ((CIndexTool*)pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		if(!bIndexing || !CheckIsHave(pTargetArray, strTmpData))
			pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	if(bIndexing) ((CIndexTool*)pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(!bIndexing || !CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
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

INT CIndexDataMgr::RestructIndex(CStringArray *pMarcAliasArray)
{
	INT nIds;
	INT nColCount = pMarcAliasArray->GetSize();
	INT nColIndex = -1;
	CString strColName;
	CString strAlias;
	CString strData;
	CString strFieldAlias, strFieldName, strType, strDataType;

	nIds = m_pIndexDM->StartFrame();
	if(nIds < 0) return -1;

	m_pIndexDM->InitDBFieldData();

	for(INT i=0; i < nColCount; i++)
	{
		strAlias = pMarcAliasArray->GetAt(i);
		nColIndex = m_pIndexDM->FindColumn(strAlias);
		if( nColIndex == -1)
			return -1;
		nIds = m_pIndexDM->FindColumnFieldName(nColIndex, strFieldAlias, strFieldName, strType, strDataType, 0, strData);
		if(nIds < 0) continue;
		
		//============================================
		//20080611 UPDATE BY PJW : �ҹ��ڴ빮�ڵ��� �����Ƿ� �ҹ��ڷ� �ٲٵ��� �����Ѵ�.
		//strType.MakeUpper();	strDataType.MakeUpper();
		strType.MakeLower(); strDataType.MakeLower();
		//============================================
		if(strDataType == _T("udf")) continue;
		if(strType == _T("str")) m_pIndexDM->AddDBFieldData(strFieldName, _T("STRING"), strData);
		else if(strType == _T("int")) 
		{
			if(strData.IsEmpty())
				m_pIndexDM->AddDBFieldData(strFieldName, _T("NUMERIC"), _T("NULL"));
			else
				m_pIndexDM->AddDBFieldData(strFieldName, _T("NUMERIC"), strData);
		}
		
	}
	//ExtractIndexInBook
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'REG_CODE')"), m_strIndexKey);
	m_pIndexDM->AddDBFieldData(_T("REG_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE')"), m_strIndexKey);
	m_pIndexDM->AddDBFieldData(_T("USE_LIMIT_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'MANAGE_CODE')"), m_strIndexKey);
	m_pIndexDM->AddDBFieldData(_T("MANAGE_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'VOL_INDEX')"), m_strIndexKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_VOL"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX')"), m_strIndexKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_VOL_TITLE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE')"), m_strIndexKey);
	m_pIndexDM->AddDBFieldData(_T("PLACE_INFO"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE')"), m_strIndexKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_SE_SHELF_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'EA_ISBN')"), m_strIndexKey);
	m_pIndexDM->AddDBFieldData(_T("ST_CODE"), _T("NUMERIC"), strData);
	//IDX_HOLD_PLACE, MAIN_PLACE_INFO �߰�
	strData.Format(_T("GET_DISP_BO_BOOK_INFO(%s, 'SHELF_LOC_CODE')"), m_strIndexKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_HOLD_PLACE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_MAIN_SHELF_CODE(%s)"), m_strIndexKey);
	m_pIndexDM->AddDBFieldData(_T("MAIN_PLACE_INFO"), _T("NUMERIC"), strData);

	nIds = m_pIndexDM->MakeUpdateFrame(m_pIndexDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), m_strIndexKey, 0);
	if(nIds < 0) return nIds;

	//�˻��������� ����ȭ
	if(SyncIndexWithSearchEngine(m_strIndexKey) < 0 ) return -1;
	//ECO_SEARCH���� ����ȭ
	if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		if(SyncIndexWithECO_SEARCH(m_strIndexKey) < 0 ) return -1;
	
	nIds = m_pIndexDM->SendFrame();
	if(nIds < 0) return nIds;
	nIds = m_pIndexDM->EndFrame();
	if(nIds < 0) return nIds;
	
	return 0;
}
INT CIndexDataMgr::SyncIndexWithSearchEngine(CString strSpeciesKey)
{
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	CString strQuery;
// 	strQuery.Format(
// 		_T("INSERT INTO IDX_TMP_BO_TBL ")
// 		_T("(SELECT esl_seq.NEXTVAL, rec_key, 'I', SYSDATE ")
// 		_T("FROM IDX_BO_TBL ")
// 		_T("WHERE REC_KEY=%s);"),
// 		strSpeciesKey);
// 	
// 	INT ids = m_pIndexDM->AddFrame(strQuery);
// 	if(ids < 0)
// 	{
// 		ESLAfxMessageBox(_T("�˻��������� ����ȭ�� ���� ���� ���� ����"));
// 		return -1;
// 	}
	//=====================================================
	
	
	return 0;
}

INT CIndexDataMgr::SyncIndexWithECO_SEARCH(CString strSpeciesKey)
{
	CString strQuery;
	strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
	
	INT ids = m_pIndexDM->AddFrame(strQuery);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("Oracle8i�� ECO_SEARCH���� ����ȭ�� ���� ���� ���� ����"));
		return -1;
	}
	
	return 0;
}

INT CIndexDataMgr::UpdateParentDM(CESL_DataMgr * pParentDM, INT nParentDMIndex, CString strParentParam)
{
	CString strAlias;
	CString strData, strParentData;
	CString strFieldAlias, strFieldName, strType, strDataType;
	CString strParentAlias, strParentName, strParentType, strParentDataType;
	INT nColIndex = -1;
	INT nColCount = m_AliasArray.GetSize();
	INT nFindParentFeild = -1;
	
	for(INT i=0; i < nColCount; i++)
	{
		strAlias = m_AliasArray.GetAt(i);
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

INT CIndexDataMgr::BO_INDEX_TEMP_INSERT(CESL_DataMgr * pDM_OUT, CString strTblName, CString strSpeciesKey, CString strWorkFlag)
{
	return 0;
}
