// HoldDupSearchQueryMaker.cpp: implementation of the CHoldDupSearchQueryMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CatSearchQueryMaker.h"
#include "../../../����/BO_SPECIES_API/ISBNChecker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCatSearchQueryMaker::CCatSearchQueryMaker(CESL_Mgr * pMgr) : CQueryMaker(pMgr)
{

}

CCatSearchQueryMaker::~CCatSearchQueryMaker()
{

}

INT CCatSearchQueryMaker::GetSearchQuery(CString &strQuery, CESL_DataMgr * pKeyDM, INT nIdx, EDBM_BIND_DATA_TYPE *pBind)
{
	if(!CheckValid()) return -1;
	if(pKeyDM == NULL) return -1;
	if(nIdx < 0) return -1;

	//===================================================
	//2010.11.29 ADD BY PJW : �������� �Ѽ����� �߰��Ѵ�.
//	CString fieldNames[10] = {
//		_T("��ǥ��"), _T("������"), _T("������"), _T("�����"), _T("����ISBN"), _T("��ü����"), 
//		_T("�Ǽ���"), _T("�����"), _T("��ġ��ȣ"), _T("��ϱ���")
//	};
	CString fieldNames[11] = {
		_T("��ǥ��"), _T("�Ѽ���"), _T("������"), _T("������"), _T("�����"), _T("����ISBN"), _T("��ü����"), 
		_T("�Ǽ���"), _T("�����"), _T("��ġ��ȣ"), _T("��ϱ���")
	};
	//===================================================
	

	INT ids = -1;
	m_ArrQuery.RemoveAll();
	
	//===================================================
	//2010.11.29 ADD BY PJW : �������� �Ѽ����� �߰��Ѵ�.
//	for(INT i=0 ; i<10 ; i++)
//	{
//		if(i < 8)
//		{
	for(INT i=0 ; i<11 ; i++)
	{
		if(i < 9)
		{
	//===================================================
			ids = MakeQuery(fieldNames[i], pKeyDM, nIdx, pBind );
			if(ids < 0) return -1;
		}
		else if(m_nDupSearchType == HOLD_DUP_SEARCH)
		{
			ids = MakeQuery(fieldNames[i], pKeyDM, nIdx, pBind );
			if(ids < 0) return -1;
		}
	}
	
	ids = GetQuery(strQuery, pKeyDM);
	if(ids < 0) return -1;
	
	return 0;
}

INT CCatSearchQueryMaker::GetTitleSearchQuery(CString &strQuery, CESL_DataMgr * pKeyDM, INT nIdx, EDBM_BIND_DATA_TYPE *pBind)
{
	m_ArrQuery.RemoveAll();
	
	CString strArticle;
	CString strTitle = pKeyDM->GetCellData(_T("IBS_��ǥ��"), nIdx);
	GetArticleData(strTitle, strArticle);
	
	//Ű���� ����
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTitle);
	//�����ѱ� ��ȯ
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(strTitle);
	if(!strArticle.IsEmpty())
	{
		//Ű���� ����
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strArticle);
		//�����ѱ� ��ȯ
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(strArticle);
	}
	CString strOption;
	
	if(!strArticle.IsEmpty())
		strOption.Format(_T(" AND CATSEARCH(I.IDX_TITLE, '%s* | %s*', NULL) > 0"), strTitle, strArticle+strTitle);
	else
		strOption.Format(_T(" AND CATSEARCH(I.IDX_TITLE, '%s*', NULL) > 0"), strTitle);
	
	m_ArrQuery.Add(strOption);
	
	INT ids = GetQuery(strQuery, pKeyDM, TRUE);
	if(ids < 0) return -1;
	
	return 0;
}

CString CCatSearchQueryMaker::GetTableName()
{
	return m_strTableName;
}

INT CCatSearchQueryMaker::MakeQuery(CString strFieldName, CESL_DataMgr *pKeyDM, INT nIdx, EDBM_BIND_DATA_TYPE *pBind)
{
	INT searchType, searchQueryType;
	searchType = searchQueryType = -1;
	BOOL articleSearchYN, useYN ;
	articleSearchYN = useYN = FALSE;
	CString articleData, tmpcondition;
	articleData = tmpcondition = _T("");

	CString searchPlace, searchField, tmpsrcstr, DMAlias, CMAlias;
	searchPlace = searchField = _T("");
	INT ids = m_SearchKey.GetFieldKeySettingInfo(strFieldName, &useYN, &searchType, &articleSearchYN);
	if(ids < 0) return -1000;
	if(!useYN) return 0;
	ids = m_SearchKey.GetDMCMAlias(strFieldName, DMAlias, CMAlias);
	if(ids < 0) return -1000;
	ids = pKeyDM->GetCellData(DMAlias, nIdx, tmpsrcstr);
	if(tmpsrcstr.GetLength() == 0) return 0;
	ids = m_SearchKey.GetSearchFieldKeySettingInfo(strFieldName, searchPlace, searchField);
	if(ids < 0) return -1000;

	if(searchPlace == _T("IDX_BO_TBL")) searchPlace = _T("I");
	else searchPlace = _T("B");
	searchQueryType = GetSearchType(searchField);
	
	if(articleSearchYN && searchType != 2)
		ids = GetArticleData(tmpsrcstr, articleData);
	
	if(searchType == 0)
	{
		if(searchQueryType == 0 || searchQueryType == 1)
		{
			//Ű���� ����
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(tmpsrcstr);
			//�����ѱ� ��ȯ
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(tmpsrcstr);
			if(!articleData.IsEmpty())
			{
				//Ű���� ����
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(articleData);
				//�����ѱ� ��ȯ
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(articleData);
			}
			if(tmpsrcstr.IsEmpty()) return 0;
		}
		
		if(searchQueryType == 0)
		{
			if(articleSearchYN && articleData.GetLength() != 0)
				tmpcondition.Format(_T(" AND CATSEARCH(%s, '%s* | %s*', NULL)>0"), searchPlace+_T(".")+searchField, tmpsrcstr, articleData+tmpsrcstr);
			else
			{
				//tmpcondition.Format(_T(" AND CATSEARCH(%s, '%s*', NULL)>0"), searchPlace+_T(".")+searchField, tmpsrcstr);
				tmpcondition.Format(_T(" AND CATSEARCH(%s, :%s, NULL)>0"), searchPlace+_T(".")+searchField, searchField);
				
				CString strBindVar;
				strBindVar.Format(_T(":%s"), searchField );
				tmpsrcstr+=_T("*");
				AddEDBMBindDataByString(pBind, STRING , strBindVar.GetBuffer(0) , tmpsrcstr.GetBuffer(0) );

			}
		}
		else if(searchQueryType == 1)
			tmpcondition.Format(_T(" AND %s='%s'"), searchPlace+_T(".")+searchField, tmpsrcstr);
		else
		{
			CString strTmpQuery;	strTmpQuery.Empty();
			CStringArray arrData;
			CString strTmpData;
			ParsingString(_T(" "), tmpsrcstr, &arrData, TRUE);
			INT cnt = arrData.GetSize();
			tmpsrcstr.Empty();
			for(INT i=0 ; i<cnt ; i++)
			{
				if(tmpsrcstr.IsEmpty()) tmpsrcstr = arrData.GetAt(i) + _T("*");
				else tmpsrcstr += _T("|") + arrData.GetAt(i) + _T("*");
				if(i==4) break;
			}
			//tmpcondition.Format(_T(" AND CATSEARCH(%s, '%s', NULL)>0"), searchPlace+_T(".")+searchField, tmpsrcstr);

			tmpcondition.Format(_T(" AND CATSEARCH(%s, :%s, NULL)>0"), searchPlace+_T(".")+searchField, searchField);
				
			CString strBindVar;
			strBindVar.Format(_T(":%s"), searchField );
			AddEDBMBindDataByString(pBind, STRING , strBindVar.GetBuffer(0) , tmpsrcstr.GetBuffer(0) );

		}
		m_ArrQuery.Add(tmpcondition);
	}
	else if(searchType == 1)
	{
		if(searchQueryType == 0 || searchQueryType == 1)
		{
			//Ű���� ����
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(tmpsrcstr);
			//�����ѱ� ��ȯ
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(tmpsrcstr);
			if(!articleData.IsEmpty())
			{
				//Ű���� ����
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(articleData);
				//�����ѱ� ��ȯ
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(articleData);
			}
			if(tmpsrcstr.IsEmpty()) return 0;
		}
		
		if(searchQueryType == 0)
		{
			if(articleSearchYN && articleData.GetLength() != 0)
				tmpcondition.Format(_T(" AND CATSEARCH(%s, '\"%s\" | \"%s\"', NULL)>0"), searchPlace+_T(".")+searchField, tmpsrcstr, articleData+tmpsrcstr);
			else
				tmpcondition.Format(_T(" AND CATSEARCH(%s, '\"%s\"', NULL)>0"), searchPlace+_T(".")+searchField, tmpsrcstr);
		}
		else if(searchQueryType == 1)
			tmpcondition.Format(_T(" AND %s='%s'"), searchPlace+_T(".")+searchField, tmpsrcstr);
		else
		{
			CString strTmpQuery;	strTmpQuery.Empty();
			CStringArray arrData;
			CString strTmpData;
			ParsingString(_T(" "), tmpsrcstr, &arrData, TRUE);
			INT cnt = arrData.GetSize();
			tmpsrcstr.Empty();

			for(INT i=0 ; i<cnt ; i++)
			{
				if( searchField == _T("ST_CODE") )
				{

					CISBNChecker isbnChecker;
					CString sPair = _T("");
					CString strISBNFrom, strOutISBN, strCheckDigit, strISBNTo, strTemp;
					CString sISBN = arrData.GetAt(i);
					INT nLength = sISBN.GetLength();
					if( nLength == 10 )
					{
						sISBN = _T("978") + sISBN;
						sISBN = sISBN.Left(12);
					}

					else if( nLength == 13 )
					{
						sISBN = sISBN.Mid(3);
						sISBN = sISBN.Left(9);
					}
					
					nLength = sISBN.GetLength();
			
					switch( nLength )
					{
					
					case 9:
					{
						strTemp = sISBN.Left(2);
						isbnChecker.ReturnISBNCheckDigit( sISBN.GetBuffer(0), strOutISBN.GetBuffer(MAX_PATH), strCheckDigit.GetBuffer(10), FALSE );
						strOutISBN.ReleaseBuffer();
						strCheckDigit.ReleaseBuffer();			
						strOutISBN.Replace(_T("-"), _T(""));
						strISBNTo.Format(_T("%s%s%s"), strTemp, strOutISBN, strCheckDigit);
						
						//strMsg.Format(_T("(%s)�� �����Ǿ����ϴ�."), strISBNTo);
						//MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN üũ"), MB_OK | MB_ICONINFORMATION);
					}
					break;
					case 12:
					{			
						isbnChecker.ReturnISBNCheckDigit( sISBN.GetBuffer(0), strOutISBN.GetBuffer(MAX_PATH), strCheckDigit.GetBuffer(10), FALSE );
						strOutISBN.ReleaseBuffer();
						strOutISBN.Replace(_T("-"),_T(""));
						strCheckDigit.ReleaseBuffer();
						strTemp = sISBN.Left(3);
						CString strTemp2 = sISBN.Mid(3,2);
						strISBNTo.Format(_T("%s%s%s%s"), strTemp, strTemp2, strOutISBN, strCheckDigit);
						
						//strMsg.Format(_T("(%s)�� �����Ǿ����ϴ�."), strISBNTo);
						//MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN üũ"), MB_OK | MB_ICONINFORMATION);
					}
					break;

					default:
						{
						//	strMsg = _T("�߸��� �ڸ����� �ԷµǾ����ϴ�");
						//	MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN üũ"), MB_OK | MB_ICONWARNING);
							;
						}
						break;
					}

					sPair = strISBNTo;

					if(tmpsrcstr.IsEmpty())
					{
						tmpsrcstr = arrData.GetAt(i);
						if( sPair.GetLength() > 0 )
							tmpsrcstr += _T("|") + sPair;
	
					}		
					else
					{
						tmpsrcstr += _T("|") + arrData.GetAt(i);
							if( sPair.GetLength() > 0 )
							tmpsrcstr += _T("|") + sPair;
					}
				}
				else
				{
					if(tmpsrcstr.IsEmpty())
						tmpsrcstr = arrData.GetAt(i);
					else tmpsrcstr += _T("|") + arrData.GetAt(i);
				}
				
			}
			tmpcondition.Format(_T(" AND CATSEARCH(%s, '%s', NULL)>0"), searchPlace+_T(".")+searchField, tmpsrcstr);
		}
		m_ArrQuery.Add(tmpcondition);
	}
	else if(searchType == 2)
	{
		//����ǥ���� ���
		this->DelArticleFromStr(tmpsrcstr, tmpsrcstr);
		this->WordHeaderCuttingData(tmpsrcstr, tmpsrcstr);
		tmpcondition.Format(_T(" AND CATSEARCH(%s, '\"%s\"', NULL)>0"), searchPlace+_T(".")+searchField, tmpsrcstr);
		m_ArrQuery.Add(tmpcondition);
	}
	return 0;
}

INT CCatSearchQueryMaker::GetQuery(CString &strQuery, CESL_DataMgr *pKeyDM, BOOL bTitleSearch/*=FALSE*/)
{
	INT nCnt = m_ArrQuery.GetSize();
	if(nCnt == 0) return -1;
	if(pKeyDM == NULL) return -1;
	
	CString strTmpQuery;	strTmpQuery.Empty();
	for(INT i=0 ; i<nCnt ; i++)
	{
		strTmpQuery += m_ArrQuery.GetAt(i);
	}
	
	strQuery = strTmpQuery;
	return 0;
}
