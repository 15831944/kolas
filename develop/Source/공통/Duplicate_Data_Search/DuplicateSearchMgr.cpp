// DuplicateSearchMgr.cpp: implementation of the CDuplicateSearchMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DuplicateSearch_Interface.h"
#include "DuplicateSearchMgr.h"
#include "ECOSearchQueryMaker.h"
#include "CatSearchQueryMaker.h"
#include "../BO_ACQ_API/SortCondition.h"
#include "DupKeyMakeMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("�ߺ���������API"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDuplicateSearchMgr::CDuplicateSearchMgr(CWnd* pWnd, CESL_Mgr* pMgr):CSPF_Work(pMgr)
{
	m_pParentWnd = pWnd;
	m_pSourceDM = NULL;
	m_pQueryMaker = NULL;

	// openbase flag	
	CFileFind ff;
	if ( ff.FindFile(_T("..\\cfg\\_OPENBASE")) )
	{
		m_bOpenbase = TRUE;
	}
	else
	{
		m_bOpenbase = FALSE;
	}
}

CDuplicateSearchMgr::CDuplicateSearchMgr(CESL_Mgr* pMgr):CSPF_Work(pMgr)
{
	m_pParentWnd = NULL;
	m_pSourceDM = NULL;
	m_pQueryMaker = NULL;

	// openbase flag	
	CFileFind ff;
	if ( ff.FindFile(_T("..\\cfg\\_OPENBASE")) )
	{
		m_bOpenbase = TRUE;
	}
	else
	{
		m_bOpenbase = FALSE;
	}
}

CDuplicateSearchMgr::~CDuplicateSearchMgr()
{
	if(m_pSourceDM)
	{
		delete m_pSourceDM;
		m_pSourceDM = NULL;
	}
	DeleteQueryMaker();
}

INT CDuplicateSearchMgr::SPFExecutePath(CString strPath)
{
	if(strPath.GetLength() == 0) return -1;

	if(strPath == _T("�ߺ�����"))
		return SearchDuplicateWorkData();
	if(strPath == _T("�����ߺ�����"))
		return SearchDuplicateWorkDataByDivno();
	else if(strPath == _T("��������"))
		return SearchDuplicateHoldingData();
	else if(strPath == _T("�����ڷẹ������"))
		return SearchDuplicateHoldingBasicData();
	else if(strPath == _T("�����ڷẹ������"))
		return SearchDuplicateHoldingImportData();
	else if(strPath == _T("��ġ����ڷẹ������"))
		return SearchDuplicateHoldingRequestData();
	else if(strPath == _T("������������"))
		return SearchDuplicateHoldingDataByDivno();
	else if(strPath == _T("����������"))
		return SearchDuplicateInputData();
	else if(strPath == _T("�����ڵ���������"))
		return SearchDuplicateAutoInputData();
	else if(strPath == _T("��������������"))
		return SearchDuplicateCatInputData();
	else if(strPath == _T("������������"))
		return SearchDuplicateCatData();
	else if(strPath == _T("��ġ�����������"))
		return SearchDuplicateRequestData();
	else if(strPath == _T("�ߺ����������������"))
		return ShowBookInfo();
	else if(strPath == _T("�׸��弱��ó��"))
		return WriteSelectSymbol();
	else if(strPath == _T("�����߱�"))
		return DuplicateDataApply();
	else if(strPath == _T("��ġ����Է�ȭ�麹������"))
		return SearchDuplicateMarcData();
	else if(strPath == _T("���Ӻ����ڷ�����"))
		return SerialDuplicateDataApply();	
	else if(strPath == _T("���Ӽ���������"))
		return SearchSerialDuplicateInputData();
	else if(strPath == _T("�ڷ�����"))
		return SortDuplicateData();
	else if(strPath == _T("�Ǻ���������"))
		return SearchDuplicateIndividualData();
	return 0;
}

INT CDuplicateSearchMgr::SearchDuplicateWorkData()
{
	//species_class�� ���� �Ŀ� ����Ǿ�� ��
	// 0:�۾���, 1:������, 2:��Ȱ����, 3:������
	CGroupCollection * pGroups = (CGroupCollection*)GetTempPtr(_T("GroupCollection"));	
	if(pGroups == NULL) return -1;
	CString strGroupInfo, strWorkCode;
	pGroups->GetGroupInfo(strGroupInfo);
	strWorkCode = pGroups->GetWorkCode();

	CString headQuery;
	if(strWorkCode.IsEmpty())
		headQuery.Format( 
			_T("I.SPECIES_CLASS IN ('0', '1') AND I.WORKING_STATUS < 'BOT' ")
			);
	else
		headQuery.Format( 
			_T("I.SPECIES_CLASS IN ('0', '1') AND I.WORKING_STATUS < 'BOT' AND I.WORK_CODE = '%s' "),
			strWorkCode);

	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(pGroups, NULL, TRUE);
	if(ids < 0) return ids;

	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(WORK_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);

	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� 
	ids = SearchDuplicateData(headQuery, TRUE, WORK_DUPLICATE_SEARCH);
	DeleteQueryMaker();
	return ids;
}

INT CDuplicateSearchMgr::SearchDuplicateHoldingBasicData()
{
	//species_class�� ���� �Ŀ� ����Ǿ�� ��
	// 0:�۾���, 1:������, 2:��Ȱ����, 3:������
	CString headQuery  = 
		_T("(I.SPECIES_CLASS = '0' OR I.SPECIES_CLASS = '1') ");	
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� 
	ids = SearchDuplicateData(headQuery, FALSE, HOLDING_DUPLICATE_SEARCH_BASIC_DATA);
	DeleteQueryMaker();
	return ids;
}

INT CDuplicateSearchMgr::SearchDuplicateHoldingImportData()
{
	//species_class�� ���� �Ŀ� ����Ǿ�� ��
	// 0:�۾���, 1:������, 2:��Ȱ����, 3:������
	CString headQuery  = 
		_T("(I.SPECIES_CLASS = '0' OR I.SPECIES_CLASS = '1') ");
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� 
	ids = SearchDuplicateData(headQuery, FALSE, HOLDING_DUPLICATE_SEARCH_IMPORT_DATA);
	DeleteQueryMaker();
	return ids;
}

INT CDuplicateSearchMgr::SearchDuplicateHoldingRequestData()
{
	//species_class�� ���� �Ŀ� ����Ǿ�� ��
	// 0:�۾���, 1:������, 2:��Ȱ����, 3:������
	CString headQuery  = 
		_T("(I.SPECIES_CLASS = '0' OR I.SPECIES_CLASS = '1') AND I.WORKING_STATUS < 'BOT' ");
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� 
	ids = SearchDuplicateData(headQuery, FALSE, HOLDING_DUPLICATE_SEARCH_REQUEST_DATA);	
	DeleteQueryMaker();
	return ids;
}

INT CDuplicateSearchMgr::SearchDuplicateHoldingData()
{
	//species_class�� ���� �Ŀ� ����Ǿ�� ��
	// 0:�۾���, 1:������, 2:��Ȱ����, 3:������
	CString headQuery;
	
	headQuery = _T("(I.SPECIES_CLASS = '0' OR I.SPECIES_CLASS = '1') AND I.WORKING_STATUS < 'BOT' ");	
		
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� 
	ids = SearchDuplicateData(headQuery, FALSE, HOLDING_DUPLICATE_SEARCH);
	DeleteQueryMaker();
	return ids;	
}

INT CDuplicateSearchMgr::SearchDuplicateWorkDataByDivno()
{
	CGroupInfo * pGroup = (CGroupInfo*)GetTempPtr(_T("GroupInfo"));
	if(pGroup == NULL) return -1;
	CString headQuery, strWorkCode;
	strWorkCode = pGroup->GetWorkCode();
	if(strWorkCode.IsEmpty())
		headQuery.Format(
			_T("I.SPECIES_CLASS IN ('0', '1') AND I.WORKING_STATUS < 'BOT' ")
			);
	else
		headQuery.Format(
			_T("I.SPECIES_CLASS IN ('0', '1') AND I.WORKING_STATUS < 'BOT' AND I.WORK_CODE = '%s' "),
			strWorkCode);
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, pGroup, TRUE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(WORK_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� 
	ids = SearchDuplicateData(headQuery, TRUE, WORK_DUPLICATE_SEARCH_DIVNO);
	DeleteQueryMaker();
	return ids;	
}

INT CDuplicateSearchMgr::SearchDuplicateHoldingDataByDivno()
{
	CString headQuery  = 
		_T("(I.SPECIES_CLASS = '0' OR I.SPECIES_CLASS = '1') AND I.WORKING_STATUS < 'BOT' ");
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� 
	ids = SearchDuplicateData(headQuery, FALSE, HOLDING_DUPLICATE_SEARCH_DIVNO);	
	DeleteQueryMaker();
	return ids;	
}

INT CDuplicateSearchMgr::SearchDuplicateInputData()
{
	CString skey, tmpstr;
	skey = tmpstr = _T("");
	GetTempData(_T("SpeciesKey"), skey);
	if(skey.GetLength() != 0)
		tmpstr.Format(_T("AND I.REC_KEY != %s"), skey);
	CString headQuery  = 
		_T("(I.SPECIES_CLASS = '1' OR I.SPECIES_CLASS = '0') AND I.WORKING_STATUS < 'BOT' ") + tmpstr;
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� 
	ids = SearchDuplicateData(headQuery, FALSE, TITLE_DUPLICATE_SEARCH);		
	DeleteQueryMaker();
	return ids;	
}

INT CDuplicateSearchMgr::SearchSerialDuplicateInputData()
{
	CString skey, tmpstr;
	skey = tmpstr = _T("");
	GetTempData(_T("SpeciesKey"), skey);
	if(skey.GetLength() != 0)
		tmpstr.Format(_T("AND I.REC_KEY != %s"), skey);
	CString headQuery  = 
		_T("I.REC_KEY IS NOT NULL ") + tmpstr;
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� , 7:���Ӽ���������
	ids = SearchDuplicateData(headQuery, FALSE, SERIAL_TITLE_DUPLICATE_SEARCH);		
	DeleteQueryMaker();
	return ids;	
}

INT CDuplicateSearchMgr::SearchDuplicateCatData()
{
	CString headQuery  = 
		//===================================================
		//2009.04.24 UPDATE BY PJW : ��� ���������� ���� ������ �����ڷ���� ���� ���·� �����Ѵ�. 
// 		_T("(I.SPECIES_CLASS = '0' OR I.SPECIES_CLASS = '1') AND I.WORKING_STATUS > 'BOC' AND I.WORKING_STATUS < 'BOR' ");
		_T("(I.SPECIES_CLASS = '0' OR I.SPECIES_CLASS = '1') AND I.WORKING_STATUS < 'BOT' ");
		//===================================================
		
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� 
	ids = SearchDuplicateData(headQuery, FALSE, CAT_DUPLICATE_SEARCH);		
	DeleteQueryMaker();
	return ids;	
}

INT CDuplicateSearchMgr::SearchDuplicateCatInputData()
{
	
	CString skey, tmpstr;
	skey = tmpstr = _T("");
	GetTempData(_T("SpeciesKey"), skey);
	if(skey.GetLength() != 0)
		tmpstr.Format(_T("AND I.REC_KEY != %s"), skey);
	CString headQuery  = 
		//===================================================
		//2009.04.24 UPDATE BY PJW : ��� ���������� ���� ������ �����ڷ���� ���� ���·� �����Ѵ� 
// 		_T("(I.SPECIES_CLASS = '1' OR I.SPECIES_CLASS = '0') AND I.WORKING_STATUS > 'BOC' AND I.WORKING_STATUS < 'BOR' ") + tmpstr;
		_T("(I.SPECIES_CLASS = '1' OR I.SPECIES_CLASS = '0') AND I.WORKING_STATUS < 'BOT' ") + tmpstr;
		//===================================================
		
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� 
	ids = SearchDuplicateData(headQuery, FALSE, CAT_TITLE_DUPLICATE_SEARCH);
	DeleteQueryMaker();
	return ids;	
}

INT CDuplicateSearchMgr::SearchDuplicateAutoInputData()
{
	CString skey, tmpstr;
	skey = tmpstr = _T("");
	GetTempData(_T("SpeciesKey"), skey);
	if(skey.GetLength() != 0)
		tmpstr.Format(_T("AND I.REC_KEY != %s"), skey);
	CString headQuery  =
		_T("(I.SPECIES_CLASS = '0' OR I.SPECIES_CLASS = '1') AND I.WORKING_STATUS < 'BOT' ") + tmpstr ; 
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� 
	ids = SearchDuplicateData(headQuery, FALSE, TITLE_AUTO_DUPLICATE_SEARCH);	
	DeleteQueryMaker();
	return ids;	
}

INT CDuplicateSearchMgr::SearchDuplicateRequestData()
{
	
	CString skey, tmpstr;
	skey = tmpstr = _T("");
	GetTempData(_T("SpeciesKey"), skey);
	if(!skey.IsEmpty())
		tmpstr.Format(_T("AND I.REC_KEY != %s"), skey);
	CString headQuery  = 
		_T("(I.SPECIES_CLASS = '0' OR I.SPECIES_CLASS = '1') AND I.WORKING_STATUS < 'BOT' ") + tmpstr;
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ����������� 
	ids = SearchDuplicateData(headQuery, FALSE, FURNISH_DATA_DUPLICATE_SEARCH);		
	DeleteQueryMaker();
	return ids;	
}

INT CDuplicateSearchMgr::SearchDuplicateMarcData()
{
	CString headQuery  = 
		_T("I.SPECIES_CLASS = '1' ");
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ�������������, 7:������ϸ�ũ��������
	ids = SearchDuplicateData(headQuery, FALSE, MARCINFO_DUPLICATE_SEARCH);		
	DeleteQueryMaker();
	return ids;	
}

INT CDuplicateSearchMgr::GetRecKeyInquery(
										  CString CMAlias, 
										  CString DMAlias, 
										  CString GridAlias, 
										  CStringArray * pArrinquery
										  )
{
/*
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1001;
	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if(ids < 0) return -1002;
	INT i = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
	if(i < 0) return -1002;
	INT count = 0;
	CString key, inquery;
	key = inquery = _T("");
	while( i >= 0)
	{
		if(count == 600)
		{
			inquery = _T("(") + inquery + _T(")");
			pArrinquery->Add(inquery);
			inquery.Empty();
			count = 0;
		}
		pDM->GetCellData(_T("IBS_��KEY"), i, key);
		if(key.IsEmpty())
			pDM->GetCellData(_T("BS_��KEY"), i, key);
		if(inquery.IsEmpty()) inquery = key;
		else inquery += _T(", ") + key;
		
		i = m_pParentMgr->SelectGetNext(CMAlias, GridAlias);
		count++;
		key = _T("");
	}
	inquery = _T("(") + inquery + _T(")");
	pArrinquery->Add(inquery);
*/
	return 0;
}

INT CDuplicateSearchMgr::InsertSearchResultAndSourceDataToTargetDM(CESL_DataMgr * pTargetDM, 
																   CString MainQuery, 
																   CString prev_key, 
																   INT *dupCount, 
																   CString CopyDMAlias, 
																   INT nSearchType,
																   INT *pSourceDataInsertIdx, 
																   EDBM_BIND_DATA_TYPE *pBind )
{
	if(!pTargetDM) return -1000;
	CESL_DataMgr * pCopyDM = m_pParentMgr->FindDM(CopyDMAlias);
	if(!pCopyDM) return -1000;
	
	CString tmpstr, tmpstr1, strOption;

	pCopyDM->TBL_NAME = m_pQueryMaker->GetTableName();

	if(nSearchType == WORK_DUPLICATE_SEARCH || nSearchType == WORK_DUPLICATE_SEARCH_DIVNO)
		strOption = _T("i.rec_key = p.species_key AND (") + MainQuery + _T(") ");//ORDER BY I.WORKING_STATUS_T(";
	else
		strOption = _T("i.rec_key = p.species_key(+) and i.rec_key = q.species_key(+) AND (") + MainQuery + _T(") ");//ORDER BY I.WORKING_STATUS_T(";
	
	INT ids = pCopyDM->RefreshDataManager(strOption, NULL, pBind );
	if(ids < 0) return -1001;
	
	if(!prev_key.IsEmpty())
	{
		ids = DeleteBOTDataExceptSourceData(pCopyDM, prev_key);		
		if( ids != 0) 
		{
			return 1000;
		}
	}
	
	INT rowCnt = pCopyDM->GetRowCount();
	if(rowCnt < 1 ) return 0;
	else if(rowCnt > 200  && 
		nSearchType != TITLE_AUTO_DUPLICATE_SEARCH && 
		nSearchType != CAT_TITLE_DUPLICATE_SEARCH && 
		nSearchType != TITLE_DUPLICATE_SEARCH &&
		nSearchType != HOLDING_DUPLICATE_SEARCH_INDIVIDUAL_DATA) 
	{
		//�� ��ġ�� ���ؾ� �Ѵ�.
		return 2000;
	}
	
	tmpstr.Format(_T("%d"), rowCnt);
	for(INT i=0 ; i<rowCnt ; i++)
	{
		if(m_pParentWnd != NULL && !((CDuplicateSearch_Interface*)m_pParentWnd)->GetRunning()) return 0;
		ids = pCopyDM->SetCellData(_T("�ĺ���ȣ"), _T("**"), i);
		if(ids < 0) return -1003;
		ids = pCopyDM->SetCellData(_T("�����ڷ�"), tmpstr, i);
		if(ids < 0) return -1003;
		tmpstr1.Format(_T("%d"), i+1);
		ids = pCopyDM->SetCellData(_T("�Ϸù�ȣ"), tmpstr1, i);
		if(ids < 0) return -1003;
	}
	*pSourceDataInsertIdx = AddToTargetDM(pTargetDM, pCopyDM, -1, prev_key);
	if(*pSourceDataInsertIdx < 0) return -1002;

	// ���� �ڷ� targer DM�� �߰�
	if(!prev_key.IsEmpty())
	{
		(*dupCount)++;
		tmpstr.Format(_T("%d"), *dupCount);
		ids = m_pSourceDM->SetCellData(_T("�ĺ���ȣ"), tmpstr, 0);
		if(ids < 0) return -1003;
		ids = AddToTargetDM(pTargetDM, m_pSourceDM, *pSourceDataInsertIdx);
		if(ids < 0) return -1002;
	}
	return rowCnt;
}

INT CDuplicateSearchMgr::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray, BOOL bUnique)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos > 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+1);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		if(bUnique && !CheckIsHave(pTargetArray, strTmpData))
			pTargetArray->Add(strTmpData);
		else if(!bUnique)
			pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(bUnique && !CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	else if(!bUnique)
		pTargetArray->Add(strSource);
	return 0;
}

BOOL CDuplicateSearchMgr::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;
}

INT CDuplicateSearchMgr::AddToTargetDM(CESL_DataMgr *pTargetDM, CESL_DataMgr *pSourceDM, INT idx, CString strKey/*=_T("")*/)
{
	if (!pTargetDM || !pSourceDM) return -1000;
	
	INT row, col;
	INT nCol, nCols, FirstRow;
	FirstRow = -1;

	// ����� �ʵ� ������ �����Ѵ�.
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	INT nRow = pSourceDM->GetRowCount();
	INT nCurRow;
	CString tmpstr, workingstatus, strMarc, title, author, publisher, pubyear, isbn;
	CMarc marc;
	if (nRow == 0) return 0;		// �ҽ��� ����Ÿ�� ����
	for (row = 0; row < nRow; row++)
	{
		tmpstr = pSourceDM->GetCellData(_T("IBS_��KEY"), row);
		if(!strKey.IsEmpty() && tmpstr == strKey) continue;

		pTargetDM->InsertRow(idx);
		if(idx >= 0)
			nCurRow = idx;
		else
			nCurRow = pTargetDM->GetRowCount() - 1;
		if(FirstRow < 0) FirstRow = nCurRow;
		nCols = pSourceDM->m_nCols;
		if(nCols < 0) nCols = pSourceDM->GetRefColumnCount();
		for (col = 0; col < nCols; col++) {
			if (pSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
			nCol = pTargetDM->FindColumn(FIELD_ALIAS);
			pTargetDM->SetCellData(nCurRow, nCol, pSourceDM->GetCellData(row, col));
			if(FIELD_ALIAS == _T("IBS_�ڷ����"))
				workingstatus = pSourceDM->GetCellData(row, col);
			if(FIELD_ALIAS == _T("BS_MARC"))
				strMarc = pSourceDM->GetCellData(row, col);
		}
		tmpstr = GetSpeciesWorkingStatusDesc(workingstatus);
		pTargetDM->SetCellData(_T("UDF_�ڷ���¼���"), tmpstr, nCurRow);
		pTargetDM->SetCellData(_T("BP_��������KEY"), _T("ALL"), nCurRow);
		if(pTargetDM->GetCellData(_T(""), nCurRow).IsEmpty())
			pTargetDM->SetCellData(_T(""), _T("�������"), nCurRow);
	}
	return FirstRow;
}

// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������
INT CDuplicateSearchMgr::SearchDuplicateData(CString strHeadQuery, BOOL isWorkData, INT SearchType)
{
	CString TargetDMAlias, FileName, CopyDMAlias, KeyDMAlias;
	CTextProgressCtrl * pPrgCtrl = NULL;
	this->GetTempData(_T("TargetDMAlias"), TargetDMAlias);
	this->GetTempData(_T("CopyDMAlias"), CopyDMAlias);
	this->GetTempData(_T("KeyDMAlias"), KeyDMAlias);
	pPrgCtrl = (CTextProgressCtrl*)GetTempPtr(_T("DataProcessingProgressCtrl"));

	if(TargetDMAlias.GetLength() == 0)
		return -1000;

	// �ߺ��˻�/�����˻縦 ���� KEY�ʵ带 ���� DM�� �����ϰ� �� DM�� Refresh�Ѵ�.
	INT ids = SetDuplicateSearchKeyDM(SearchType);
	SetDuplicateSortKeyDM();
	if(ids < 0)	return ids;	
	if(pPrgCtrl && !((CDuplicateSearch_Interface*)m_pParentWnd)->GetRunning()) return 0;

	CESL_DataMgr * pKeyDM = m_pParentMgr->FindDM(KeyDMAlias);
	CESL_DataMgr * pTargetDM = m_pParentMgr->FindDM(TargetDMAlias);
	if(!pKeyDM || !pTargetDM) return -1005;
	//�����ڷḦ ���� DM�� ������
	m_pSourceDM = new CESL_DataMgr;
	m_pSourceDM->SetCONNECTION_INFO(pTargetDM->CONNECTION_INFO);
	m_pSourceDM->InitDMPKFromFile(CopyDMAlias);
	m_pSourceDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	pTargetDM->FreeData();
	
	// �籸���� �ߺ��˻�/�����˻縦 ���� KEY�ʵ带 ���� DM�� �� �ο츦 �о�
	// �˻� ������ ����� ���� �ڷ�� �˻� ��� �ڷḦ TargetDM�� �߰��Ѵ�.
	INT rowCnt = pKeyDM->GetRowCount();
	if(rowCnt == 0) return -1007;
	CString DupSearchQuery, key;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pKeyDM->CONNECTION_INFO);
	INT SearchCount, DupCount;
	SearchCount = DupCount = 0;

	CString prgMsg = _T("");
	if(SearchType == WORK_DUPLICATE_SEARCH || SearchType == HOLDING_DUPLICATE_SEARCH 
		|| SearchType == CAT_DUPLICATE_SEARCH || SearchType == HOLDING_DUPLICATE_SEARCH_IMPORT_DATA
		|| SearchType == WORK_DUPLICATE_SEARCH_DIVNO || SearchType == HOLDING_DUPLICATE_SEARCH_DIVNO
		|| SearchType == HOLDING_DUPLICATE_SEARCH_REQUEST_DATA)
	{
		if(pPrgCtrl) pPrgCtrl->SetRange(0, rowCnt);
	}
	//�����ڷ� ������ġ
	INT nSourceDataInsertIdx = 0;
	//CString posMsg;
	INT posCnt = 1;
	// ���� ������ ���� ��˻��Ҷ��� ��˻� ī��Ʈ
	INT nResearchCnt = 0;
	CString strPercent;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		// �籸���� �ߺ��˻�/�����˻縦 ���� KEY�ʵ带 ���� DM�� �ο츦 �о�
		// �˻� ������ �����.
		if(pPrgCtrl)
		{
			if(!((CDuplicateSearch_Interface*)m_pParentWnd)->GetRunning()) return 0;
			pPrgCtrl->SetPos(posCnt++);
		}
		DupSearchQuery.Empty();
		pKeyDM->GetCellData(_T("IBS_��KEY"), i, key);
		if(SearchType == HOLDING_DUPLICATE_SEARCH_REQUEST_DATA) key.Empty();

		// Bind ó��..  +++++++
		EDBM_BIND_DATA_TYPE *pBind;
		pBind = NULL;
		InitEDBMBindDataType( &pBind, 0, 1);


		if(SearchType == TITLE_DUPLICATE_SEARCH || SearchType == TITLE_AUTO_DUPLICATE_SEARCH
			|| SearchType == CAT_TITLE_DUPLICATE_SEARCH || SearchType == FURNISH_DATA_DUPLICATE_SEARCH)
			ids = m_pQueryMaker->GetTitleSearchQuery(DupSearchQuery, pKeyDM, i, pBind );
		else
			ids = m_pQueryMaker->GetSearchQuery(DupSearchQuery, pKeyDM, i, pBind );
		if(ids < 0) continue;
		if(DupSearchQuery.IsEmpty())
		{
			ShowErrorMsg(pKeyDM, i, QUERY_INVALID);
			continue;
		}

		// 2007.06.01 ADD BY PDJ
		// �������� �߰� ������ ���δ�.
		//===================================================
		//2009.05.27 UPDATE BY PJW : �����������ϰ��� �����ڷ� ������ �����Ѵ�.
		if( SearchType != 2 )
		{
			AddAppendCondition(DupSearchQuery);
		}
		//===================================================

		nSourceDataInsertIdx = 0;
		ids = InsertSearchResultAndSourceDataToTargetDM(pTargetDM, 
														strHeadQuery+DupSearchQuery, 
														key, 
														&DupCount, 
														CopyDMAlias, 
														SearchType, 
														&nSourceDataInsertIdx,
														pBind);

		FreeEDBMBindData(&pBind, 1);
		// Bind ó��..  -------
		
		if(ids < 0) return -99;
		else if(ids == 1000)
		{			
			ShowErrorMsg(pKeyDM, i, SOURCE_INVALID);
			nResearchCnt++;
			if(nResearchCnt > 1)
			{
				nResearchCnt = 0;
				continue;
			}
			else
			{
				i--;
				continue;
			}
		}
		else if(ids == 2000)
		{
			ShowErrorMsg(pKeyDM, i, SEARCH_OVERFLOW);
			continue;
		}
		else if(ids > 0 && SearchType == HOLDING_DUPLICATE_SEARCH_REQUEST_DATA)
		{
			InsertSourceDataByKeyDM(pTargetDM, pKeyDM, &DupCount, CopyDMAlias, i, nSourceDataInsertIdx);
		}
		SearchCount += ids;
		nResearchCnt = 0;
	}
	UpdateTargetDMDuplicateSearchSelectCount(pTargetDM, DupCount);

	this->SetTempData(_T("SearchCount"), SearchCount);

	return 0; 
}

INT CDuplicateSearchMgr::UpdateTargetDMDuplicateSearchSelectCount(CESL_DataMgr *pTargetDM, INT DupCount)
{
	if(!pTargetDM) return -1000;
	if(DupCount == 0) return 0;

	INT ids = -1;
	CString tmpstr, tmpvalue;
	tmpstr = tmpvalue = _T("");
	INT rowCnt = pTargetDM->GetRowCount();
	for(INT i=0 ; i<rowCnt ; i++)
	{
		ids = pTargetDM->GetCellData(_T("�ĺ���ȣ"), i, tmpstr);
		if(ids < 0) return -1001;
		if(tmpstr == _T("**") || tmpstr.GetLength() == 0) continue;
		else
		{
			tmpvalue.Format(_T("%d"), DupCount);
			ids = pTargetDM->SetCellData(_T("�����ڷ�"), tmpvalue, i);
			if(ids < 0) return -1002;
		}
	}
	return 0;
}

INT CDuplicateSearchMgr::GetNextUserSelectDataIndex(CString CMAlias, CString GridAlias, INT idx)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(_T("DM_�ߺ�������"));
	if(CMAlias.GetLength() == 0 || GridAlias.GetLength() == 0 || !pDM)
		return -1000;
	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if(ids < 0) return -1001;
	INT i = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
	if(i < 0) return -1001;
	
	while(i >= 0)
	{
		if(i <= idx )
			i = m_pParentMgr->SelectGetNext(CMAlias, GridAlias);
		else
			break;
	}

	CString tmpstr;
	while(i >= 0)
	{
		ids = pDM->GetCellData(_T("�ĺ���ȣ"), i, tmpstr);
		if(tmpstr != _T("**")) return i;
		else i--;
	}
	
	return -1002;
}

INT CDuplicateSearchMgr::GetPrevUserSelectDataIndex(CString CMAlias, CString GridAlias, INT idx)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(_T("DM_�ߺ�������"));
	if(CMAlias.GetLength() == 0 || GridAlias.GetLength() == 0 || !pDM)
		return -1000;
	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if(ids < 0) return -1001;
	INT i = m_pParentMgr->SelectGetTailPosition(CMAlias, GridAlias);
	if(i < 0) return -1001;
	if(idx < 0) idx = 0;
	while(i >= 0)
	{
		if(i >= idx )
			i = m_pParentMgr->SelectGetPrev(CMAlias, GridAlias);
		else
			break;
	}
	CString tmpstr;
	while(i >= 0)
	{
		ids = pDM->GetCellData(_T("�ĺ���ȣ"), i, tmpstr);
		if(tmpstr != _T("**")) return i;
		else i--;
	}
	
	return -1002;
}

INT CDuplicateSearchMgr::GetNextDuplicateDataIndex(CString CMAlias, CString GridAlias, INT idx, INT startIdx)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(_T("DM_�ߺ�������"));
	if(CMAlias.GetLength() == 0 || GridAlias.GetLength() == 0 || !pDM)
		return -1000;
	INT rowCnt = pDM->GetRowCount();
	if(startIdx < 0)
	{
		startIdx = 1;
		startIdx += idx;
	}
	INT ids ;
	if(idx < 0)
	{
		ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
		if(ids < 0) return -1;
		startIdx = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
		if(startIdx < 0) return -1;
	}
	CString tmpstr;
	if(rowCnt-1 < startIdx) return -1;
	ids = pDM->GetCellData(_T("�ĺ���ȣ"), startIdx, tmpstr);
	if(ids < 0) return -1001;
	if(tmpstr != _T("**")) return -1;
	return startIdx;
}

INT CDuplicateSearchMgr::GetPrevDuplicateDataIndex(CString CMAlias, CString GridAlias, INT idx, INT startIdx)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(_T("DM_�ߺ�������"));
	if(CMAlias.GetLength() == 0 || GridAlias.GetLength() == 0 || !pDM)
		return -1000;
	if(startIdx > -1 && startIdx == idx) return -1;
	//�̺κ��� ������ �ȵɰ��̴�. �ֳ��ϸ� idx�� 0���� ���� �� ���� �����̴�.
	if(idx < 0)
	{
		INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
		if(ids < 0) return -1;
		startIdx = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
		if(startIdx < 0) return -1;
	}
	return startIdx;
}

INT CDuplicateSearchMgr::ShowBookInfo()
{
	INT idx;
	CString SpeciesDMAlias;
	this->GetTempData(_T("SpeciesDMIdx"), idx);
	this->GetTempData(_T("SpeciesDMAlias"), SpeciesDMAlias);

	CESL_DataMgr * pSpeciesDM = m_pParentMgr->FindDM(SpeciesDMAlias);
	if(idx < 0 || !pSpeciesDM) return -1000;

	CString species_key, pkey, strOption;
	INT ids = pSpeciesDM->GetCellData(_T("IBS_��KEY"), idx, species_key);
	if(ids < 0) return -1001;
	
	//���⼭ ��ȭ���� ��������
	this->SetTempData(_T("SpeciesKey"), species_key.GetBuffer(0));
	return 0;
}

INT CDuplicateSearchMgr::WriteSelectSymbol()
{
	CString CMAlias, DMAlias, GridAlias, SelSymbol;
	INT idx = -1000;
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("GridAlias"), GridAlias);
	this->GetTempData(_T("��������"), SelSymbol);
	this->GetTempData(_T("Index"), idx);

	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(CMAlias);
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pCM || !pDM || idx < -1) return -1000;

	CESL_Grid *pgridDUPLICATE_WORKDATA = (CESL_Grid *)pCM->FindControl(GridAlias);
	if (!pgridDUPLICATE_WORKDATA) return -1001;
	
	INT nRow = -1;
	INT nCol = -1;
	nRow = pgridDUPLICATE_WORKDATA->GetRow();
	nCol = pgridDUPLICATE_WORKDATA->GetCol();
	CString tmpstr;
	if (1 == nCol)
	{
		pgridDUPLICATE_WORKDATA->GetAt(nRow-1, 0, tmpstr);
		tmpstr.TrimLeft();	tmpstr.TrimRight();
		if(SelSymbol == _T("MainGrid"))
		{
			if(CheckIsSourceData(pDM, nRow-1))
			{
				if(tmpstr == _T("V"))
					pgridDUPLICATE_WORKDATA->SetAt(nRow-1, 0, _T(""));
				else
					pgridDUPLICATE_WORKDATA->SetAt(nRow-1, 0, _T("V"));
			}
			else
			{
				if(tmpstr == _T("V"))
					pgridDUPLICATE_WORKDATA->SetAt(nRow-1, 0, _T(""));
			}
		}
		else if(SelSymbol == _T("All"))
		{
			INT gridRowCnt = (INT)pgridDUPLICATE_WORKDATA->GetCount();
	
			for(INT i=1 ; i<gridRowCnt ; i++)
			{
				if(CheckIsSourceData(pDM, i-1))
					pgridDUPLICATE_WORKDATA->SetAt(i-1, 0, _T("V"));
			}
		}
		else if(SelSymbol == _T("OneSelect"))
		{
			INT gridRowCnt = (INT)pgridDUPLICATE_WORKDATA->GetCount();
	
			for(INT i=1 ; i<=gridRowCnt ; i++)
			{
				pgridDUPLICATE_WORKDATA->SetAt(i-1, 0, _T(""));
			}
			pgridDUPLICATE_WORKDATA->SetAt(nRow-1, 0, _T("V"));
		}
		else if(SelSymbol == _T("NextOneSelect"))
		{
			INT gridRowCnt = (INT)pgridDUPLICATE_WORKDATA->GetCount();
			if(idx >= gridRowCnt) return -1002;
			if(idx < 0) return -1003;
			if(idx-1 >= 0)
				pgridDUPLICATE_WORKDATA->SetAt(idx-1, 0, _T(""));
			pgridDUPLICATE_WORKDATA->SetAt(idx, 0, _T("V"));

		}
		else if(SelSymbol == _T("PrevOneSelect"))
		{
			INT gridRowCnt = (INT)pgridDUPLICATE_WORKDATA->GetCount();
			if(idx >= gridRowCnt) return -1002;
			if(idx < 0) return -1003;
			if(idx+1 < gridRowCnt)
				pgridDUPLICATE_WORKDATA->SetAt(idx+1, 0, _T(""));
			pgridDUPLICATE_WORKDATA->SetAt(idx, 0, _T("V"));
		}
		else
		{
			pgridDUPLICATE_WORKDATA->SetAt(idx, 0, SelSymbol);
		}
	}
	return 0;
}

BOOL CDuplicateSearchMgr::CheckIsSourceData(CESL_DataMgr *pDM, INT rowIdx)
{
	if(!pDM || rowIdx < 0) return FALSE;
	CString tmpstr;
	INT ids = pDM->GetCellData(_T("�ĺ���ȣ"), rowIdx, tmpstr);
	if(ids < 0) return FALSE;
	if(tmpstr == _T("**"))
		return FALSE;
	return TRUE;
}

INT CDuplicateSearchMgr::DuplicateDataApply()
{
	CString CMAlias, DMAlias, GridAlias;
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("GridAlias"), GridAlias);
	
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);

	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if(ids < 0) return -1000;
	INT i = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
	if(i < 0) return -1000;
	CString key, status;
	INT cnt = m_pParentMgr->SelectGetCount(CMAlias, GridAlias);
	
	(m_pParentMgr->FindDM(DMAlias))->GetCellData(_T("IBS_��KEY"), i, key);
	
	this->InitTempData();
	this->SetTempData(_T("SpeciesKey"), key.GetBuffer(0));
	return 0;
}

BOOL CDuplicateSearchMgr::CheckIsPurchaseData(CESL_DataMgr *pDM, INT rowIdx)
{
	if(!pDM || rowIdx < 0) return FALSE;
	CString tmpstr;
	INT ids = pDM->GetCellData(_T("UDF_�ڷ���¼���"), rowIdx, tmpstr);
	if(ids < 0) return FALSE;
	if(tmpstr == _T("�����ڷ�"))
		return TRUE;
	return FALSE;
}

INT CDuplicateSearchMgr::SetDuplicateSearchKeyDM(CString SourceCMAlias, CString SourceDMAlias, CString SourceGridAlias, CESL_DataMgr *pKeyDM)
{
/*
	pKeyDM->FreeData();
	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(SourceCMAlias);
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(SourceDMAlias);
	CESL_Grid * pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = pGrid->SelectGetHeadPosition();
	INT nRow = -1;
	
	while(i >= 0)
	{
		pKeyDM->InsertRow(-1);
		nRow = pKeyDM->GetRowCount()-1;
		pKeyDM->SetCellData(_T("IBS_��ǥ��"), pDM->GetCellData(_T("��ǥ��"), i), nRow);
		pKeyDM->SetCellData(_T("IBS_������"), pDM->GetCellData(_T("������"), i), nRow);
		pKeyDM->SetCellData(_T("IBS_������"), pDM->GetCellData(_T("������"), i), nRow);
		pKeyDM->SetCellData(_T("IBS_�����"), pDM->GetCellData(_T("�����"), i), nRow);
		pKeyDM->SetCellData(_T("IBS_��KEY"), pDM->GetCellData(_T("��ġ���KEY"), i), nRow);
		i = pGrid->SelectGetNext();
	}
*/
	return 0;
}

INT CDuplicateSearchMgr::SetDuplicateSearchKeyDM(CString query, CESL_DataMgr * pKeyDM)
{
/*
	pKeyDM->RefreshDataManager(query);
	INT rowCnt = pKeyDM->GetRowCount();
	INT cnt = -1;
	INT nPos = -1;
	CMarc marc;
	CString strMarc, title, author, publisher, pubyear, isbn, strTmpData, regNo;
	CArray<CString, CString&> arrIsbn;
	
	CESL_CharSetMgr CharSetMgr_UnlessWord;
	CharSetMgr_UnlessWord.InitData(_T("���Ҿ�"), _T("���Ҿ�"));
	
	for(INT i=0 ; i<rowCnt ; i++)
	{
		pKeyDM->GetCellData(_T("BS_MARC"), i, strMarc);
		if(strMarc.GetLength() == 0) continue;
		m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("245$a"), title);
		//���⼭ 245$d�� �̾Ƽ� ���Ҿ �����Ѵ�.
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("245$d"), author);
		author.TrimLeft();	author.TrimRight();
		INT nFind = author.ReverseFind(' ');
		if (nFind > 0)
		{
			CString str1 = author.Mid(nFind);
			CString str2 = str1;
			str1.TrimLeft();
			str1.TrimRight();
			
			if (CharSetMgr_UnlessWord.Find(str1, str2) == 0)
			{
				author.Delete(nFind, author.GetLength() - nFind);
			}
			author.TrimLeft();	author.TrimRight();
		}

		if(author.IsEmpty()) m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("100$a"), author);
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("260$b"), publisher);
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("260$c"), pubyear);
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("020$a"), isbn, &arrIsbn);
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), regNo);
		pKeyDM->SetCellData(_T("IBS_��ǥ��"), title, i);
		pKeyDM->SetCellData(_T("IBS_������"), author, i);
		pKeyDM->SetCellData(_T("IBS_������"), publisher, i);
		pKeyDM->SetCellData(_T("IBS_�����"), pubyear, i);
		pKeyDM->SetCellData(_T("BB_��Ϲ�ȣ"), regNo, i);
		
		cnt = arrIsbn.GetSize();
		if(cnt == 0) pKeyDM->SetCellData(_T("BB_����_ISBN"), _T(""), i);
		else if(cnt == 1)
		{
			nPos = isbn.Find(_T("("), 0);
			if(nPos >= 0) isbn = isbn.Mid(0, nPos);
			pKeyDM->SetCellData(_T("BB_����_ISBN"), isbn, i);
		}
		else
		{
			for(INT j=0 ; j<cnt ; j++)
			{
				strTmpData = arrIsbn.GetAt(j);
				nPos = strTmpData.Find(_T("("), 0);
				if(nPos >= 0) strTmpData = strTmpData.Mid(0, nPos);
				if(isbn.IsEmpty()) isbn = strTmpData;
				else isbn += _T(" ") + strTmpData;
			}
			pKeyDM->SetCellData(_T("BB_����_ISBN"), isbn, i);
		}
		title = author = publisher = pubyear = isbn = _T("");
	}
*/
	return 0;
}

INT CDuplicateSearchMgr::SetDuplicateSearchKeyDM(CString strMarc, 
												 CString strMediaCodeInfo,
												 CString strVolInfo, 
												 CString strVolTitleInfo,
												 CString strShelfCodeInfo,
												 CString strRegCodeInfo,
												 CESL_DataMgr *pKeyDM, 
												 INT SearchType)
{
/*
	if(!pKeyDM) return -1000;

	CMarc pMarc;
	INT ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &pMarc);
	if(ids < 0) return -1000;
	
	pKeyDM->FreeData();
	pKeyDM->InsertRow(-1);
	CString strTmpData;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&pMarc, _T("245$a"), strTmpData);
	if(ids >=0) pKeyDM->SetCellData(_T("IBS_��ǥ��"), strTmpData, 0);
	strTmpData.Empty();
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&pMarc, _T("245$d"), strTmpData);
	if(ids >=0) pKeyDM->SetCellData(_T("IBS_������"), strTmpData, 0);
	strTmpData.Empty();
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&pMarc, _T("260$b"), strTmpData);
	if(ids >=0) pKeyDM->SetCellData(_T("IBS_������"), strTmpData, 0);
	strTmpData.Empty();
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&pMarc, _T("260$c"), strTmpData);
	if(ids >=0) pKeyDM->SetCellData(_T("IBS_�����"), strTmpData, 0);
	strTmpData.Empty();
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&pMarc, _T("260$c"), strTmpData);
	if(ids >=0) pKeyDM->SetCellData(_T("IBS_�����"), strTmpData, 0);
	strTmpData.Empty();
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&pMarc, _T("020$a"), strTmpData);
	if(ids >=0) pKeyDM->SetCellData(_T("BB_����_ISBN"), strTmpData, 0);
	pKeyDM->SetCellData(_T("BB_��ü����"), strMediaCodeInfo, 0);
	pKeyDM->SetCellData(_T("BB_�����"), strVolInfo, 0);
	pKeyDM->SetCellData(_T("BB_�Ǽ���"), strVolTitleInfo, 0);
	pKeyDM->SetCellData(_T("BB_��ġ��ȣ"), strShelfCodeInfo, 0);
	pKeyDM->SetCellData(_T("BB_��ϱ���"), strRegCodeInfo, 0);
	
	strTmpData.Empty();

*/
	return 0;
}

// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������
INT CDuplicateSearchMgr::SetDuplicateSearchKeyDM(INT SearchType)
{
	CDupKeyMakeMgr DupKeyMaker(m_pParentMgr, this);
	return DupKeyMaker.SPFExecutePath(SearchType);
}
//
//	CString SourceCMAlias, SourceDMAlias, SourceGridAlias, TargetDMAlias, KeyDMAlias;
//	CString strMarc, strVolInfo, strVolTitleInfo, strShelfCodeInfo, strRegCodeInfo, strMediaCodeInfo, title;
//	CGroupInfo * pGroup  = NULL;
//	this->GetTempData(_T("KeyDMAlias"), KeyDMAlias);
//	if(SearchType == WORK_DUPLICATE_SEARCH ||
//		SearchType == HOLDING_DUPLICATE_SEARCH || 
//		SearchType == HOLDING_DUPLICATE_SEARCH_BASIC_DATA || 
//		SearchType == HOLDING_DUPLICATE_SEARCH_IMPORT_DATA ||
//		SearchType == HOLDING_DUPLICATE_SEARCH_REQUEST_DATA ||
//		SearchType == CAT_DUPLICATE_SEARCH )
//	{
//		this->GetTempData(_T("CMAlias"), SourceCMAlias);
//		this->GetTempData(_T("SourceDMAlias"), SourceDMAlias);
//		this->GetTempData(_T("GridAlias"), SourceGridAlias);
//	}	
//	else if(SearchType == CAT_TITLE_DUPLICATE_SEARCH || SearchType == TITLE_DUPLICATE_SEARCH || 
//			SearchType == TITLE_AUTO_DUPLICATE_SEARCH || SearchType == FURNISH_DATA_DUPLICATE_SEARCH || 
//			SearchType == SERIAL_TITLE_DUPLICATE_SEARCH)
//	{
//		this->GetTempData(_T("Title"), title);
//	}
//	else if(SearchType == HOLDING_DUPLICATE_SEARCH_INDIVIDUAL_DATA)
//	{
//		GetTempData(_T("Marc"), strMarc);
//		GetTempData(_T("MediaCodeInfo"), strMediaCodeInfo);
//		GetTempData(_T("VolInfo"), strVolInfo);
//		GetTempData(_T("VolTitleInfo"), strVolTitleInfo);
//		GetTempData(_T("ShelfCodeInfo"), strShelfCodeInfo);
//		GetTempData(_T("RegCodeInfo"), strRegCodeInfo);
//	}
//	else if(SearchType == WORK_DUPLICATE_SEARCH_DIVNO || SearchType == HOLDING_DUPLICATE_SEARCH_DIVNO)
//	{
//		pGroup = (CGroupInfo*)GetTempPtr(_T("GroupInfo"));
//	}
//
//	CESL_DataMgr * pKeyDM = m_pParentMgr->FindDM(KeyDMAlias);
//	if(pKeyDM == NULL)
//	{
//		return -1;
//	}
//
//	INT ids = -1;
//	if(SearchType == WORK_DUPLICATE_SEARCH || 
//		SearchType == HOLDING_DUPLICATE_SEARCH || 
//		SearchType == HOLDING_DUPLICATE_SEARCH_BASIC_DATA || 
//		SearchType == HOLDING_DUPLICATE_SEARCH_IMPORT_DATA ||
//		SearchType == CAT_DUPLICATE_SEARCH)
//	{
//		CStringArray arrInquery;
//		CString inquery, query;
//		ids = GetRecKeyInquery(SourceCMAlias, SourceDMAlias, SourceGridAlias, &arrInquery);
//		if(ids < 0) return ids;
//		ids = SetDuplicateSearchKeyDM(&arrInquery, pKeyDM);
//		if(ids < 0) return ids;
//	}
//	else if(SearchType == HOLDING_DUPLICATE_SEARCH_INDIVIDUAL_DATA)
//	{
//		ids = SetDuplicateSearchKeyDM(strMarc, strMediaCodeInfo, strVolInfo, strVolTitleInfo, strShelfCodeInfo, strRegCodeInfo, pKeyDM, SearchType);
//	}
//	else if(SearchType	==	TITLE_DUPLICATE_SEARCH
//			|| SearchType == CAT_TITLE_DUPLICATE_SEARCH
//			|| SearchType == TITLE_AUTO_DUPLICATE_SEARCH
//			|| SearchType == FURNISH_DATA_DUPLICATE_SEARCH
//			|| SearchType == SERIAL_TITLE_DUPLICATE_SEARCH)
//	{
//		pKeyDM->FreeData();
//		pKeyDM->InsertRow(-1);
//		pKeyDM->SetCellData(_T("IBS_��ǥ��"), title, 0);
//	}
//	else if(SearchType == WORK_DUPLICATE_SEARCH_DIVNO || SearchType == HOLDING_DUPLICATE_SEARCH_DIVNO)
//	{
//		CString strQuery, strWorkCode;
//		strWorkCode = pGroup->GetWorkCode();
//		if(strWorkCode.IsEmpty())
//			strQuery.Format(
//				_T("I.REC_KEY=S.REC_KEY AND I.REC_KEY IN (SELECT SPECIES_KEY FROM BO_PURCHASEINFO_TBL WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO='%s')"),
//				pGroup->GetAcqYear(), pGroup->GetGroupInfo());
//		else 
//			strQuery.Format(
//				_T("I.REC_KEY=S.REC_KEY AND I.REC_KEY IN (SELECT I.REC_KEY FROM IDX_BO_TBL I, BO_PURCHASEINFO_TBL P WHERE I.REC_KEY=P.SPECIES_KEY AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO='%s' AND I.WORK_CODE='%s')"),
//				pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strWorkCode);
//		ids = SetDuplicateSearchKeyDM(strQuery, pKeyDM);
//		if(ids < 0) return ids;
//	}
//	else if(SearchType == HOLDING_DUPLICATE_SEARCH_REQUEST_DATA)
//	{
//		ids = SetDuplicateSearchKeyDM(SourceCMAlias, SourceDMAlias, SourceGridAlias, pKeyDM);
//	}
//	return 0;
//}


CString CDuplicateSearchMgr::GetSpeciesWorkingStatusDesc(CString workingstatus)
{
	if(workingstatus.IsEmpty()) return _T("�����ڷ�");
	else
	{
		//CString tmpstr = workingstatus.Left(3);
		CString tmpstr = workingstatus;
		tmpstr.MakeUpper();
		/*
		if(tmpstr == _T("BOA")) return _T("�����ڷ�");
		else if(tmpstr == _T("BOR")) return _T("����ڷ�");
		else if( tmpstr == _T("BOC")) return _T("�����ڷ�");
		else if( tmpstr == _T("BOL")) return _T("�����ڷ�");
		else if( tmpstr == _T("BOT")) return _T("��������ڷ�");
		*/
		 if(tmpstr == _T("BOT111O")) return _T("�����ڷ�");
		 else if(tmpstr == _T("BOT112O")) return _T("�����ڷ�");
		 else if(tmpstr == _T("BOT211O")) return _T("���������߱��ڷ�");
		 else if(tmpstr == _T("BOT212O")) return _T("���������߱��ڷ�");
		 else if(tmpstr == _T("BOA111N")) return _T("���Դ���ڷ�");
		 else if(tmpstr == _T("BOA112N")) return _T("�˼�����ڷ�(�ֹ��ڷ�)");
		 else if(tmpstr == _T("BOA113O")) return _T("�̳�ǰ�ڷ�");
		 else if(tmpstr == _T("BOA114O")) return _T("(����)�ΰ����ڷ�");
		 else if(tmpstr == _T("BOA211O")) return _T("���������ڷ�(�ΰ����ڷ�)");
		 else if(tmpstr == _T("BOA212O")) return _T("�����δ��ڷ�");
		 else if(tmpstr == _T("BOR111N")) return _T("����ΰ��ڷ�");
		 else if(tmpstr == _T("BOR112N")) return _T("����ڷ�(�����ΰ����ڷ�)");
		 else if(tmpstr == _T("BOR113O")) return _T("�����ι�ȣ�ο��ڷ�");
		 else if(tmpstr == _T("BOC111O")) return _T("�����������ڷ�");
		 else if(tmpstr == _T("BOC112O")) return _T("�з�����ڷ�");
		 else if(tmpstr == _T("BOC113N")) return _T("��������ڷ�");
		 else if(tmpstr == _T("BOC114O")) return _T("�����ΰ����ڷ�(�����Ϸ��ڷ�)");
		 else if(tmpstr == _T("BOC211O")) return _T("�����������ڷ�");
		 else if(tmpstr == _T("BOL111O")) return _T("�����ΰ��ڷ�(�谡����ڷ�)");
		 else if(tmpstr == _T("BOL112N")) return _T("�谡�ڷ�(�����ڷ�)");
		 else if(tmpstr == _T("BOL113O")) return _T("���������ڷ�");
		 else if(tmpstr == _T("BOL114O")) return _T("�ļ��ڷ�");
		 else if(tmpstr == _T("BOL115O")) return _T("�����ڷ�");
		 else if(tmpstr == _T("BOL116O")) return _T("�������ڷ�");
		 else if(tmpstr == _T("BOL252N")) return _T("�н��ڷ�");
		 else if(tmpstr == _T("BOL211O")) return _T("���ܴ����ڷ�");
		 else if(tmpstr == _T("BOL212O")) return _T("���������ڷ�");
		 //===================================================
		 //2009.03.09 UPDATE BY PJW :  ���� ��ȣ���� �ڷ���� �߰�
		 else if(tmpstr == _T("BOL218O")) return _T("��ȣ���������ڷ�");
		 // 17/07/20 ������ : å���������ڷ� ���� �߰�
//*/ ADD ---------------------------------------------------------------------------
		else if(tmpstr == _T("BOL219O")) return _T("å���������ڷ�");
//*/ END ---------------------------------------------------------------------------	
		// 18.09.27 JMJ KOL.RED.2018.008
		 else if(tmpstr == _T("BOL411O")) return _T("����å�η����������ڷ�");
		 else if(tmpstr == _T("BOL511O")) return _T("���յ���Ÿ���ݳ�");
		 // 2013.12.01 ADD BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
		 else if(tmpstr == _T("BOL611O")) return _T("���յ���Ÿ������");
		 //=====================================================
		 else if(tmpstr == _T("BOL213O")) return _T("Ư�������ڷ�");
		 else if(tmpstr == _T("BOL214O")) return _T("��޴����ڷ�");
		 else if(tmpstr == _T("BOL215O")) return _T("����ڷ�");


	}
	return workingstatus;
}

INT CDuplicateSearchMgr::SerialDuplicateDataApply()
{
	CString CMAlias, DMAlias, GridAlias;
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("GridAlias"), GridAlias);
	
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);

	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if(ids < 0) return -1000;
	INT i = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
	if(i < 0) return -1000;
	CString key;
	
	(m_pParentMgr->FindDM(DMAlias))->GetCellData(_T("ISS_��KEY"), i, key);
	
	this->InitTempData();
	this->SetTempData(_T("SpeciesKey"), key.GetBuffer(0));
	return 0;
}

INT CDuplicateSearchMgr::SetDuplicateSearchKeyDM(CStringArray *pArrInquery, CESL_DataMgr *pKeyDM)
{
/*
		CBO_SPECIES api(m_pParentMgr);
		INT cnt = pArrInquery->GetSize();
		if(cnt == 0) return -1;
		if(!pKeyDM) return -1;
		CESL_DataMgr * pDM = NULL;
		CArray<CESL_DataMgr*, CESL_DataMgr*> ArrDataMgr;
		CString strDMPK, strQuery;
		for(INT i=0 ; i<cnt ; i++)
		{
			pDM = new CESL_DataMgr;
			pDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
			pDM->InitDMPKFromFile(pKeyDM->DB_MGR_ALIAS);
			pDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
			strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN %s"), pArrInquery->GetAt(i));
			SetDuplicateSearchKeyDM(strQuery, pDM);
			ArrDataMgr.Add(pDM);
		}
	
		cnt = ArrDataMgr.GetSize();
		pKeyDM->FreeData();
		for(i=0 ; i<cnt ; i++)
		{
			AddToTargetDM(pKeyDM, ArrDataMgr.GetAt(i), -1);
		}
		cnt = ArrDataMgr.GetSize();
		for(i=0 ; i<cnt ; i++)
		{
			delete ArrDataMgr.GetAt(i);
		}
		ArrDataMgr.RemoveAll();
*/
	
	return 0;
}

INT CDuplicateSearchMgr::SetDupSearchDMTableName(CGroupCollection * pGroups, CGroupInfo * pGroupInfo, BOOL bWorkDupSearch)
{
	CString strOption, strAcqYear, strGroupInfo;
	INT ids;

	//===================================================
//	2008.02.28 ADD BY PJW : �ϳ��� ��Ű�� 2���� ����,��������Ű�� ������ ��� �ϳ��� �̵��� ����
//	2008.04.16 DELETE BY PJW : �ӵ������� ������� �ǵ���
//	2010.11.17 UPDATE BY PJW : ����/���� ������ ȭ�󱸺��� �߰��Ѵ�.currency_code currency_code
//	CString strDonInfoTbl;
//	CString strPurchaseInfoTbl;
//	strDonInfoTbl.Format(_T("( SELECT * FROM BO_DONINFO_TBL Q1 WHERE Q1.REC_KEY = ( SELECT MAX(Q2.REC_KEY) FROM BO_DONINFO_TBL Q2 WHERE Q2.SPECIES_KEY =Q1.SPECIES_KEY))"));
//	strPurchaseInfoTbl.Format(_T("( SELECT * FROM BO_PURCHASEINFO_TBL P1 WHERE P1.REC_KEY = ( SELECT MAX(P2.REC_KEY) FROM BO_PURCHASEINFO_TBL P2 WHERE P2.SPECIES_KEY = P1.SPECIES_KEY))"));

	if(!bWorkDupSearch)
	{
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			m_strTblName = 
				_T("idx_bo_tbl i, ")
				_T("(SELECT rec_key rec_key, species_key species_key, (acq_year || '-' || receipt_no) group_info, '2' acq_code, '����' acq_type, input_type input_type, price price, total_vol_cnt total_vol_cnt, subject_code subject_code, acq_year acq_year, receipt_no group_no, null order_date, checkin_date checkin_date, currency_code currency_code ")
				_T(" FROM bo_doninfo_tbl %WHERE%) q, ")
				_T("(SELECT rec_key rec_key, species_key species_key, (order_year || '-' || order_seq_no || '-' || order_work_no || '-' || order_serial_no) group_info, '1' acq_code, '����' acq_type, input_type input_type, price price, total_vol_cnt total_vol_cnt, subject_code subject_code, order_year acq_year, order_seq_no group_no, order_date order_date, checkin_date checkin_date, currency_code currency_code ")
				_T(" FROM bo_purchaseinfo_tbl %WHERE%) p");
				//===================================================
//				20080228 ADD BY PJW : �ϳ��� ��Ű�� 2���� ����,��������Ű�� ������ ��� �ϳ��� �̵��� ����
//				20080416 UPDATE BY PJW : �ӵ������� ������� �ǵ���
//				" FROM " + strDonInfoTbl + "%WHERE%) q, "
//				"(SELECT rec_key rec_key, species_key species_key, (order_year || '-' || order_seq_no || '-' || order_work_no || '-' || order_serial_no) group_info, '1' acq_code, '����' acq_type, input_type input_type, price price, total_vol_cnt total_vol_cnt, subject_code subject_code, order_year acq_year, order_seq_no group_no, order_date order_date, checkin_date checkin_date "
//				" FROM " + strPurchaseInfoTbl + "%WHERE%) p";
				//===================================================
		}
		else if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ORA_CATSEARCH)
		{
			m_strTblName = 
				_T("idx_bo_tbl i, ")
				_T("(SELECT rec_key rec_key, species_key species_key, (acq_year || '-' || receipt_no) group_info, '2' acq_code, '����' acq_type, input_type input_type, price price, total_vol_cnt total_vol_cnt, subject_code subject_code, acq_year acq_year, receipt_no group_no, null order_date, checkin_date checkin_date, currency_code currency_code ")
				_T(" FROM bo_doninfo_tbl) q, ")
				_T("(SELECT rec_key rec_key, species_key species_key, (order_year || '-' || order_seq_no || '-' || order_work_no || '-' || order_serial_no) group_info, '1' acq_code, '����' acq_type, input_type input_type, price price, total_vol_cnt total_vol_cnt, subject_code subject_code, order_year acq_year, order_seq_no group_no, order_date order_date, checkin_date checkin_date, currency_code currency_code ")
				_T(" FROM bo_purchaseinfo_tbl) p");		
				//===================================================
//				20080228 ADD BY PJW : �ϳ��� ��Ű�� 2���� ����,��������Ű�� ������ ��� �ϳ��� �̵��� ����
//				20080416 UPDATE BY PJW : �ӵ������� ������� �ǵ���
//				" FROM " + strDonInfoTbl +") q, "
//				"(SELECT rec_key rec_key, species_key species_key, (order_year || '-' || order_seq_no || '-' || order_work_no || '-' || order_serial_no) group_info, '1' acq_code, '����' acq_type, input_type input_type, price price, total_vol_cnt total_vol_cnt, subject_code subject_code, order_year acq_year, order_seq_no group_no, order_date order_date, checkin_date checkin_date "
//				" FROM " + strPurchaseInfoTbl + ") p";		
				//===================================================
		}
		
		ids = 0;
		ids = SetUDFDonateRelativeDMInfo(FALSE);
	}
	else if(pGroupInfo)
	{
		m_strTblName =
			_T("idx_bo_tbl i, ")
		   _T("(SELECT rec_key rec_key, species_key species_key, (order_year || '-' || order_seq_no || '-' || order_work_no || '-' || order_serial_no) group_info, '1' acq_code, '����' acq_type, input_type input_type, price price, total_vol_cnt total_vol_cnt, subject_code subject_code, order_year acq_year, order_seq_no group_no, order_date order_date, checkin_date checkin_date, currency_code currency_code ")
		   _T(" FROM bo_purchaseinfo_tbl %WHERE%) p ");
		strAcqYear = pGroupInfo->GetAcqYear();
		strGroupInfo = pGroupInfo->GetGroupInfo();
		if(!strAcqYear.IsEmpty() && !strGroupInfo.IsEmpty())
		{
			strOption.Format(_T("WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO IN (%s)"), strAcqYear, strGroupInfo);
			if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
				strOption += _T(" %AND%");
			m_strTblName.Replace(_T("%WHERE%"), strOption);
		}
		else
		{
			if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ORA_CATSEARCH)
				m_strTblName.Replace(_T("%WHERE%"), _T(""));
		}
		ids = SetUDFDonateRelativeDMInfo(TRUE);
	}
	else if(pGroups)
	{
		m_strTblName =
			_T("idx_bo_tbl i, ")
		   _T("(SELECT rec_key rec_key, species_key species_key, (order_year || '-' || order_seq_no || '-' || order_work_no || '-' || order_serial_no) group_info, '1' acq_code, '����' acq_type, input_type input_type, price price, total_vol_cnt total_vol_cnt, subject_code subject_code, order_year acq_year, order_seq_no group_no, order_date order_date, checkin_date checkin_date, currency_code currency_code ")
		   _T(" FROM bo_purchaseinfo_tbl %WHERE%) p ");
		pGroups->GetGroupInfo(strGroupInfo);
		strAcqYear = pGroups->GetAcqYear();
		if(!strAcqYear.IsEmpty() && !strGroupInfo.IsEmpty() && strGroupInfo != _T("��ü�ڷ�"))
		{
			strOption.Format(_T("WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO in (%s)"), strAcqYear, strGroupInfo);
			if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
				strOption += _T(" %AND%");	
			
			m_strTblName.Replace(_T("%WHERE%"), strOption);
		}
		else
		{
			if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ORA_CATSEARCH)
				m_strTblName.Replace(_T("%WHERE%"), _T(""));
		}
		ids = SetUDFDonateRelativeDMInfo(TRUE);
	}
	//===================================================
	return ids;
}

INT CDuplicateSearchMgr::SetUDFDonateRelativeDMInfo(BOOL bUDFSet)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(_T("DM_�ߺ�������_���纻"));
	if(pDM == NULL) return -1;

	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	INT col, nCols;
	nCols = pDM->m_nCols;
	if(nCols < 0) nCols = pDM->GetRefColumnCount();
	for (col = 0; col < nCols; col++) {
		if (pDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		if(FIELD_NAME.Left(1) == _T("S"))
		{
			pDM->SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, _T("udf"));
		}
		if(FIELD_NAME.Left(1) == _T("Q"))// || FIELD_NAME.Left(1) == _T("S"))
		{
			if(bUDFSet)
				pDM->SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, _T("udf"));
			else
				pDM->SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, _T(""));
		}
		if(FIELD_ALIAS == _T("IBS_������"))
		{
			if(bUDFSet)
			{				
				pDM->SetDataMgrRef(col, FIELD_ALIAS, _T("I.AUTHOR_INFO"), FIELD_TYPE, INIT_VALUE, _T(""));				
			}
			else
			{
				// 2006-01-05 ������ ���� - ��������� �������ڷḦ �ߺ����� ������ ORDERED -> RULE�� HINT ����
				pDM->SetDataMgrRef(col, FIELD_ALIAS, _T("/*+ RULE */ I.AUTHOR_INFO"), FIELD_TYPE, INIT_VALUE, _T(""));				
			}
		}
		//===================================================
		//2010.12.03 ADD BY PJW : �������� ���ǿ� ���� ���� �޸� �Ѵ�.
		if( FIELD_ALIAS == _T("BP_����") )
		{
			INT nPurPos = m_strTblName.Find(_T("bo_purchaseinfo_tbl"));
			INT nDonPos = m_strTblName.Find(_T("bo_doninfo_tbl"));

			if( 0 < nPurPos && 0 < nDonPos )
			{				
				pDM->SetDataMgrRef(col, FIELD_ALIAS, _T("DECODE( P.PRICE, '', Q.PRICE, NULL, Q.PRICE, P.PRICE)"), FIELD_TYPE, INIT_VALUE, _T(""));
			}
			else if( 0 < nPurPos )
			{
				pDM->SetDataMgrRef(col, FIELD_ALIAS, _T("P.PRICE"), FIELD_TYPE, INIT_VALUE, _T(""));
			}
			else if( 0 < nPurPos )
			{
				pDM->SetDataMgrRef(col, FIELD_ALIAS, _T("Q.PRICE"), FIELD_TYPE, INIT_VALUE, _T(""));
			}
			else
			{
				pDM->SetDataMgrRef(col, FIELD_ALIAS, _T("DECODE( P.PRICE, '', Q.PRICE, NULL, Q.PRICE, P.PRICE)"), FIELD_TYPE, INIT_VALUE, _T(""));
			}
		}
		if( FIELD_ALIAS == _T("BP_ȭ�󱸺�") )
		{
			INT nPurPos = m_strTblName.Find(_T("bo_purchaseinfo_tbl"));
			INT nDonPos = m_strTblName.Find(_T("bo_doninfo_tbl"));

			if( 0 < nPurPos && 0 < nDonPos )
			{				
				pDM->SetDataMgrRef(col, FIELD_ALIAS, _T("DECODE( P.CURRENCY_CODE, '', Q.CURRENCY_CODE, NULL, Q.CURRENCY_CODE, P.CURRENCY_CODE)"), FIELD_TYPE, INIT_VALUE, _T(""));
			}
			else if( 0 < nPurPos )
			{
				// 2006-01-05 ������ ���� - ��������� �������ڷḦ �ߺ����� ������ ORDERED -> RULE�� HINT ����
				pDM->SetDataMgrRef(col, FIELD_ALIAS, _T("P.CURRENCY_CODE"), FIELD_TYPE, INIT_VALUE, _T(""));
			}
			else if( 0 < nPurPos )
			{
				// 2006-01-05 ������ ���� - ��������� �������ڷḦ �ߺ����� ������ ORDERED -> RULE�� HINT ����
				pDM->SetDataMgrRef(col, FIELD_ALIAS, _T("Q.CURRENCY_CODE"), FIELD_TYPE, INIT_VALUE, _T(""));
			}
			else
			{
			}
		}
		//===================================================		
	}
	
	return 0;
}

INT CDuplicateSearchMgr::SortDuplicateData()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	INT nCol = -1;
	INT nSortDirection = 0;
	BOOL bSortDirection = FALSE;
	CString strSortAlias;
	GetTempData(_T("SORT_FIELD_ALIAS"), strSortAlias);
	GetTempData(_T("SORT_DIRECTION"), nSortDirection);
	if(pDM == NULL) return -1;
	if(strSortAlias.IsEmpty()) return -1;
	if(nSortDirection == 0) return -1;
	if(nSortDirection == ESL_DATAMGR_SORT_DESC) bSortDirection = TRUE;
	else bSortDirection = FALSE;
	CString strTmpData;
	INT nRowCnt, nStartPos, nEndPos;
	nStartPos = nEndPos = -1;
	nRowCnt = pDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("�ĺ���ȣ"), i);
		if(strTmpData != _T("**") && nStartPos < 0)
		{
			nStartPos = i;
			continue;
		}
		if(strTmpData != _T("**") && nStartPos >= 0 && nEndPos < 0)
		{
			nEndPos = i;
			pDM->SORT(strSortAlias, nStartPos+1, nEndPos, bSortDirection);
			nStartPos = i;
			nEndPos = -1;
		}
	}
	if(nStartPos >= 0)
	{
		nEndPos = i;
		pDM->SORT(strSortAlias, nStartPos+1, nEndPos, bSortDirection);
	}
	else if(nStartPos < 0 && nEndPos < 0)
	{
		pDM->SORT(strSortAlias, bSortDirection);
	}
	return 0;
}

INT CDuplicateSearchMgr::InsertSourceDataByKeyDM(CESL_DataMgr *pTargetDM, CESL_DataMgr *pKeyDM, INT *DupCount, CString CopyDMAlias, INT nIdx, INT nInsertRow)
{
	if(!pTargetDM) return -1000;
	if(!pKeyDM) return -1000;
	CESL_DataMgr * pCopyDM = m_pParentMgr->FindDM(CopyDMAlias);
	if(!pCopyDM) return -1000;
	pCopyDM->FreeData();
	CopyAndAddDM(pCopyDM, pKeyDM, nIdx);
	(*DupCount)++;
	CString strTmpData;
	strTmpData.Format(_T("%d"), *DupCount);
	pCopyDM->SetCellData(_T("�ĺ���ȣ"), strTmpData, 0);
	pCopyDM->SetCellData(_T("UDF_�ڷ���¼���"), _T("��ġ����ڷ�"), 0);
	pCopyDM->SetCellData(_T("IBS_�ڷ����"), _T("��ġ����ڷ�"), 0);
	AddToTargetDM(pTargetDM, pCopyDM, nInsertRow);
	
	return 0;
}

INT CDuplicateSearchMgr::CopyAndAddDM(CESL_DataMgr *pTargetDM, CESL_DataMgr *pSourceDM, INT nSourceIdx)
{
	if (!pTargetDM || !pSourceDM) return -1000;
	
	INT row, col;
	INT nCol, nCols;
	// ����� �ʵ� ������ �����Ѵ�.
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	pTargetDM->InsertRow(-1);
	row = pTargetDM->GetRowCount()-1;
	nCols = pSourceDM->m_nCols;
	if(nCols < 0) nCols = pSourceDM->GetRefColumnCount();
	for (col = 0; col < nCols; col++) {
		if (pSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0)
			continue;
		nCol = pTargetDM->FindColumn(FIELD_ALIAS);
		if(nCol < 0) 
			continue;
		pTargetDM->SetCellData(row, nCol, pSourceDM->GetCellData(nSourceIdx, col));
	}
	return 0;
}

INT CDuplicateSearchMgr::DeleteCopyData(CESL_DataMgr *pCopyDM, CString strKey)
{
	CString strTmpData;
	INT nRowCnt = pCopyDM->GetRowCount();
	for(INT i=nRowCnt-1 ; i>=0 ; i--)
	{
		strTmpData = pCopyDM->GetCellData(_T("IBS_��KEY"), i);
		if(strKey != strTmpData) pCopyDM->DeleteRow(i);
	}
	return 0;
}

INT CDuplicateSearchMgr::InsertToTargetDM(CESL_DataMgr *pTargetDM, INT nTargetIdx, CESL_DataMgr *pSourceDM, INT nSourceIdx, BOOL bCheckDup/*=FALSE*/, CString strCheckAlias/*=_T("")*/)
{
	if (!pTargetDM || !pSourceDM) return -1000;
	
	INT nCol, nCols, nRow;

	// ����� �ʵ� ������ �����Ѵ�.
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strKey, strTmpData;
	
	if(bCheckDup && !strCheckAlias.IsEmpty())
	{
		strKey = pSourceDM->GetCellData(strCheckAlias, nSourceIdx);
		nRow = pTargetDM->GetRowCount();
		for(INT i=0 ; i<nRow ; i++)
		{
			strTmpData = pTargetDM->GetCellData(strCheckAlias, i);
			if(strTmpData == strKey)
				return 0;
		}
	}
	INT nCurRow;
	if(nTargetIdx < 0)
	{
		pTargetDM->InsertRow(-1);
		nCurRow = pTargetDM->GetRowCount()-1;
	}
	else nCurRow = nTargetIdx;
	
	nCols = pSourceDM->m_nCols;
	if(nCols < 0) nCols = pSourceDM->GetRefColumnCount();
	for (INT i = 0; i < nCols; i++) 
	{
		if (pSourceDM->GetColumnRefInfo(i, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0)
			continue;
		nCol = pTargetDM->FindColumn(FIELD_ALIAS);
		pTargetDM->SetCellData(nCurRow, nCol, pSourceDM->GetCellData(nSourceIdx, i));
	}
	return 0;
}

INT CDuplicateSearchMgr::DeleteBOTDataExceptSourceData(CESL_DataMgr *pCopyDM, CString strKey)
{
	m_pSourceDM->FreeData();
	BOOL bSourceDataCopyYN = FALSE;
	INT nRowCnt = pCopyDM->GetRowCount();
	CString strWorkingStatus, strTmpKey;
	for(INT i=nRowCnt-1 ; i>=0 ; i--)
	{
		strWorkingStatus = pCopyDM->GetCellData(_T("IBS_�ڷ����"), i);
		strTmpKey = pCopyDM->GetCellData(_T("IBS_��KEY"), i);
		if(strWorkingStatus.Left(3) == _T("BOT") && strTmpKey != strKey)
			pCopyDM->DeleteRow(i);
		else if(strTmpKey == strKey)
		{
			if(m_pSourceDM->GetRowCount() == 0)
			{
				InsertToTargetDM(m_pSourceDM, -1, pCopyDM, i);
				bSourceDataCopyYN = TRUE;
			}
			pCopyDM->DeleteRow(i);
		}
	}
	if(bSourceDataCopyYN) return 0;
	return 1;
}

VOID CDuplicateSearchMgr::ShowErrorMsg(CESL_DataMgr *pKeyDM, INT nIdx, DUP_ERROR_TYPE nErrorType)
{
	CString strMsg, strTailMsg;
	if(nErrorType == SEARCH_OVERFLOW) 
	{
		strTailMsg = _T("�������� �˻��� �ڷᰡ 200���� �ѽ��ϴ�. �ڷ�� �˻����Ǹ� Ȯ���� �ֽʽÿ�.");
		//===================================================
		//2009.10.05 ADD BY PJW : ���ܻ�Ȳ �޼��� ������
		strMsg.Format(
			_T("����   : [ %s ]\n")
			_T("����   : [ %s ]\n")
			_T("������ : [ %s ]\n")
			_T("����� : [ %s ]\n")
			_T("%s"),
			pKeyDM->GetCellData(_T("IBS_��ǥ��"), nIdx), 
			pKeyDM->GetCellData(_T("IBS_������"), nIdx), 
			pKeyDM->GetCellData(_T("IBS_������"), nIdx), 
			pKeyDM->GetCellData(_T("IBS_�����"), nIdx),
			strTailMsg);
		m_pParentMgr->ESLAfxMessageBox(strMsg);
		//===================================================
	}
	else if(nErrorType == SOURCE_INVALID) 
	{
		strTailMsg = _T("�˻���� �����ڷᰡ ���Ե��� �ʾҽ��ϴ�. �������� �ڷ���¸� Ȯ���� �ֽʽÿ�.");
	}
	else if(nErrorType == QUERY_INVALID) 
	{
		strTailMsg = _T("�� �ڷῡ ���Ͽ� ���� �����׸����� ������ �������� ���߽��ϴ�.");
		//===================================================
		//2009.10.05 ADD BY PJW : ���ܻ�Ȳ �޼��� ������
		strMsg.Format(
			_T("����   : [ %s ]\n")
			_T("����   : [ %s ]\n")
			_T("������ : [ %s ]\n")
			_T("����� : [ %s ]\n")
			_T("%s"),
			pKeyDM->GetCellData(_T("IBS_��ǥ��"), nIdx), 
			pKeyDM->GetCellData(_T("IBS_������"), nIdx), 
			pKeyDM->GetCellData(_T("IBS_������"), nIdx), 
			pKeyDM->GetCellData(_T("IBS_�����"), nIdx),
			strTailMsg);
		m_pParentMgr->ESLAfxMessageBox(strMsg);
		//===================================================
	}
	
	//===================================================
	// 2008.03.15 REM BY PDJ
	// �޽����� ������ �ʿ� ����.
	// �׳� �����ϴ°� ����.
//	strMsg.Format(
//		_T("����   : [ %s ]\n")
//		_T("����   : [ %s ]\n")
//		_T("������ : [ %s ]\n")
//		_T("����� : [ %s ]\n")
//		_T("%s"),
//		pKeyDM->GetCellData(_T("IBS_��ǥ��"), nIdx), 
//		pKeyDM->GetCellData(_T("IBS_������"), nIdx), 
//		pKeyDM->GetCellData(_T("IBS_������"), nIdx), 
//		pKeyDM->GetCellData(_T("IBS_�����"), nIdx),
//		strTailMsg);
//	m_pParentMgr->ESLAfxMessageBox(strMsg);
	//===================================================
	if(nErrorType == SOURCE_INVALID)
	{
		CString strKey = pKeyDM->GetCellData(_T("IBS_��KEY"), nIdx);
		CString strMarc = pKeyDM->GetCellData(_T("BS_MARC"), nIdx);
		CString strTitle, strAuthor, strPublisher, strAllItem, strWorkingStatus;
		CESL_DataMgr tmpDM;
		tmpDM.SetCONNECTION_INFO(pKeyDM->CONNECTION_INFO);
		CMarc marc;
		m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);

		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->Init(m_pParentMgr->m_pInfo->pMarcMgr, &marc);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("��ǥ��"), strTitle);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("������"), strAuthor);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("������"), strPublisher);
		//===================================================
		//2010.11.29 UPDATE BY PJW : ALL_ITEM�� 653$a�� �߰��Ѵ�..
//		strAllItem = strTitle + _T(" ") + strAuthor + _T(" ") + strPublisher;
		CArray<CString, CString&> astr653Tag;
		CString str653Tag = _T("");
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("653$a"), str653Tag, &astr653Tag);
		str653Tag = _T("");
		for( INT i = 0 ; i < astr653Tag.GetSize() ; i++ )
		{
			if( str653Tag.IsEmpty() )
			{
				str653Tag += astr653Tag.GetAt(i);
			}
			else
			{
				str653Tag += _T(" ") + astr653Tag.GetAt(i);
			}
		}
		strAllItem = strTitle + _T(" ") + strAuthor + _T(" ") + strPublisher + _T(" ") + str653Tag;
		//===================================================
		
		
		pKeyDM->StartFrame();	
		strMsg.Format(
			_T("UPDATE IDX_BO_TBL SET	IDX_TITLE='%s', ")
									_T("IDX_AUTHOR='%s', ")
									_T("IDX_PUBLISHER='%s', ")
									_T("IDX_ALL_ITEM='%s', ")
									//_T("WORKING_STATUS=TESTWORKINGSTATUS(%s), ")
									_T("WORKING_STATUS=TESTWORKSTATUS(%s), ")
									_T("USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
									_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
									_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
									_T("IDX_VOL_TITLE=GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX'), ")
									_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
									_T("IDX_VOL=GET_BO_BOOK_INFO(%s,'VOL_INDEX'), ")
									_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE'), ")
									_T("ST_CODE=GET_BO_BOOK_INFO(%s, 'EA_ISBN'), ")
									_T("LAST_WORK='%s' ")
									_T("WHERE REC_KEY=%s;"), 
			strTitle, strAuthor, strPublisher, strAllItem, strKey, strKey, strKey, strKey, strKey, strKey, strKey, strKey, strKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strKey);
		pKeyDM->AddFrame(strMsg);
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		strMsg.Format(
// 			_T("INSERT INTO IDX_TMP_BO_TBL ")
// 			_T("(SELECT esl_seq.NEXTVAL, rec_key, 'I', SYSDATE ")
// 			_T("FROM IDX_BO_TBL ")
// 			_T("WHERE REC_KEY=%s);"),
// 			strKey);
// 		pKeyDM->AddFrame(strMsg);
		//=====================================================
		
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strMsg.Format(_T("ECO$P_MAKE_BO$I(%s);"), strKey);
			pKeyDM->AddFrame(strMsg);
		}
		INT ids = pKeyDM->SendFrame();
		if(ids < 0) return ;
		ids = pKeyDM->EndFrame();
		if(ids < 0) return ;
	}
}

VOID CDuplicateSearchMgr::SetDuplicateSortKeyDM()
{
/*
	CBO_ACQ_DataSortDlg dlg;
	CSortCondition * pCondition = dlg.GetSortCondition(_T(""));

	CString KeyDMAlias;
	this->GetTempData(_T("KeyDMAlias"), KeyDMAlias);
	CESL_DataMgr * pKeyDM = m_pParentMgr->FindDM(KeyDMAlias);
	if(pKeyDM->GetRowCount() == 0)
	{
		return;
	}

	// SORT
	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];

	strAlias[nCondColumns] = _T("BB_��Ϲ�ȣ");					
	nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		
	nCondColumns++;

	if(pCondition->m_nFirstType != 0)
	{
		strAlias[nCondColumns] = pCondition->m_strFirstAlias;	
		nCond[nCondColumns] = pCondition->m_nFirstType;		
		nCondColumns++;
	}
	if(pCondition->m_nSecondType != 0)
	{
		strAlias[nCondColumns] = pCondition->m_strSecondAlias;	
		nCond[nCondColumns] = pCondition->m_nSecondType;	
		nCondColumns++;
	}
	if(pCondition->m_nThirdType != 0)
	{
		strAlias[nCondColumns] = pCondition->m_strThirdAlias;	
		nCond[nCondColumns] = pCondition->m_nThirdType;		
		nCondColumns++;
	}
	if(pCondition->m_nFourthType != 0)
	{
		strAlias[nCondColumns] = pCondition->m_strFourthAlias;	
		nCond[nCondColumns] = pCondition->m_nFourthType;	
		nCondColumns++;
	}

	pKeyDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
*/
}

INT CDuplicateSearchMgr::CreateQueryMaker()
{
	DeleteQueryMaker();

	INT nSearchType = m_pParentMgr->m_pInfo->GetSearchIndexType();
	if(nSearchType == ECO_SEARCH)
	{
		m_pQueryMaker = new CECOSearchQueryMaker(m_pParentMgr);		
		m_pQueryMaker->m_bOpenbase = m_bOpenbase;
	}
	else if(nSearchType == ORA_CATSEARCH)
	{
		m_pQueryMaker = new CCatSearchQueryMaker(m_pParentMgr);
	}
	m_pQueryMaker->SetSearchType(nSearchType);
	m_pQueryMaker->SetSearchKey(m_strKeyFileName);
	return 0;
}	

INT CDuplicateSearchMgr::DeleteQueryMaker()
{
	if(m_pQueryMaker)
	{
		delete m_pQueryMaker;
		m_pQueryMaker = NULL;
	}
	return 0;
}

INT CDuplicateSearchMgr::SearchDuplicateIndividualData()
{
	CString skey, tmpstr;
	skey = tmpstr = _T("");
	GetTempData(_T("SpeciesKey"), skey);
	if(skey.GetLength() != 0)
		tmpstr.Format(_T("AND I.REC_KEY != %s"), skey);
	CString headQuery  = 
		_T("(I.SPECIES_CLASS = '1' OR I.SPECIES_CLASS = '0') AND I.WORKING_STATUS < 'BOT' ") + tmpstr;
	//���̺� �� ����
	INT ids = SetDupSearchDMTableName(NULL, NULL, FALSE);
	if(ids < 0) return ids;
	// ��������Ŀ ����
	ids = CreateQueryMaker();
	m_pQueryMaker->SetDupSearchType(HOLD_DUP_SEARCH);
	m_pQueryMaker->SetTableName(m_strTblName);
	// SearchType 0:�ߺ�����, 1:��������, 2:����������, 3:������������, 4:��������������, 5:�����ڵ���������, 6:��ġ������������� 
	ids = SearchDuplicateData(headQuery, FALSE, HOLDING_DUPLICATE_SEARCH_INDIVIDUAL_DATA);		
	DeleteQueryMaker();
	return ids;	
}

BOOL CDuplicateSearchMgr::GetBookTradeInfo()
{
	CString		sFileName;
	CStdioFile	file;

	sFileName = _T("..\\CFG\\��������.CFG");

	if ( file.Open(sFileName,::CFile::modeRead | CFile::typeBinary)==NULL ) 
	{
		file.Close();
		return FALSE;
	}

	file.Close();
	
	return TRUE;
}

VOID CDuplicateSearchMgr::AddAppendCondition(CString &Query)
{	
	CStdioFile file;		
	CString strLine;	
	CString filename = _T("duplicate_search_key2.txt");	

	if ( !file.Open(filename, CFile::modeRead | CFile::typeBinary )) 
	{
		AfxMessageBox(_T("�������� ����Ű�� �������� �ʽ��ϴ�."));
		return;
	}	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	while( file.ReadString(strLine) )
	{
		if ( strLine == _T("[�����ڷ����Կ���]") )
		{
			file.ReadString(strLine);
			if ( strLine == _T("USE_YN=1") ) 
			{
				Query += _T("AND I.WORKING_STATUS != 'BOL115O'");
			}
		}
	}

	file.Close();
}

