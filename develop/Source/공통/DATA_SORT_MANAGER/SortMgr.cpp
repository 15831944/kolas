// SortMgr.cpp: implementation of the CSortMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SortMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSortMgr::CSortMgr(CESL_Mgr * pParentMgr) : CSPF_Work(pParentMgr)
{

}

CSortMgr::~CSortMgr()
{

}

INT CSortMgr::SPFExecutePath(CString strPath)
{
	if(strPath.IsEmpty()) return -1;
	else if(strPath == _T("�����׸�DM����"))
		return MakeSortElementDM();
	return -1;
}

INT CSortMgr::MakeSortElementDM()
{
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CSortItemSet * pItemSet = NULL;
	pDM = (CESL_DataMgr *)GetTempPtr(_T("DM"));
	pItemSet = (CSortItemSet *)GetTempPtr(_T("ITEMSET"));
	pGrid = pItemSet->GetGrid();
	
	if(pItemSet == NULL || pGrid == NULL || pDM == NULL) return -1;
	pDM->FreeData();
	
	INT nColCnt, nFirst, nCurIdx;
	
	CString strInitValue, strLinkValue;
	if(pGrid->UseUserDefine())
	{
		strInitValue = pGrid->USER_INIT_VALUE;
		strLinkValue = pGrid->USER_LINK_DATA;
		nColCnt = 1;
		if (strInitValue.GetLength() == 0) return -1;
		for (INT i = 0; i < strInitValue.GetLength(); i++)
		{
			if (strInitValue.GetAt(i) == ',') nColCnt++;
		}
	}
	else
	{
		strInitValue = pGrid->ELEMENT->INIT_VALUE;
		strLinkValue = pGrid->ELEMENT->LINK_DATA;
		nColCnt = pGrid->CountLinkColumns();
	}
	strInitValue.TrimLeft();	strInitValue.TrimRight();
	strLinkValue.TrimLeft();	strLinkValue.TrimRight();
	if (strInitValue.IsEmpty() || strLinkValue.IsEmpty()) return -1;
	
	CString strLinkElement, strInitElement, strDMAlias, strColumn, strDMFieldAlias, strTmpData;
	for(INT i=0 ; i<nColCnt ; i++) 
	{
		strLinkElement.Empty();
		
		strLinkElement = strLinkValue.SpanExcluding(_T(","));
		strLinkElement.TrimLeft();		strLinkElement.TrimRight();
		if (!strLinkElement.IsEmpty()) 
		{
			if(pGrid->ParseMgrAndColumn(strLinkElement, strDMAlias, strDMFieldAlias) < 0) 
				return -1;
		}
		else strDMFieldAlias.Empty();
		
		if(strLinkValue.GetLength() > strLinkElement.GetLength())
			strTmpData = strLinkValue.Mid(strLinkElement.GetLength()+1);
		else
			strTmpData.Empty();
		strLinkValue = strTmpData;
		
		strInitElement.Empty();
		strInitElement = strInitValue.SpanExcluding(_T(","));
		if(!strInitElement.IsEmpty())
		{
			strColumn.Empty();
			nFirst = 0;
			nFirst = strInitElement.Find(':', 0);
			strColumn = strInitElement.Mid(1, nFirst-1);
		}
		else strColumn.Empty();
		
		if(strInitValue.GetLength() > strInitElement.GetLength())
			strTmpData = strInitValue.Mid(strInitElement.GetLength()+1);
		else strTmpData.Empty();
		strInitValue = strTmpData;
		
		pDM->InsertRow(-1);
		nCurIdx = pDM->GetRowCount()-1;
		
		pDM->SetCellData(_T("�̸�"), strColumn, nCurIdx);
		pDM->SetCellData(_T("�˸��ƽ�"), strDMFieldAlias, nCurIdx);
	}
	
	return SetSortInfo(pDM, pItemSet);
}

INT CSortMgr::SetSortInfo(CESL_DataMgr * pDM, CSortItemSet *pSortItemSet)
{
	if(pDM == NULL || pSortItemSet == NULL) return -1;
	INT nSortItemCnt, nRowCnt;
	nSortItemCnt = pSortItemSet->GetDataCount();
	nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0 || nSortItemCnt == 0) return 0;

	INT ids;
	CSortItem tmpItem(m_pParentMgr);
	tmpItem.InitDataManager(pDM->CONNECTION_INFO, pDM->GETDMAlias());
	
	for(INT i=0 ; i<nSortItemCnt ; i++)
	{
		pSortItemSet->SetCurrentSelectData(i);
		pSortItemSet->GetCurrentSelectData(&tmpItem);
		ids = SetSortInfo(pDM, &tmpItem);
		if(ids < 0) return ids;
	}
	return 0;
}

INT CSortMgr::SetSortInfo(CESL_DataMgr *pDM, CSortItem *pItem)
{
	if(pDM == NULL || pItem == NULL) return -1;

	CString strTitle, strDMAlias, strTmpData;
	INT nRowCnt = pDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		pItem->GetCellData(_T("�̸�"), strTitle);
		pItem->GetCellData(_T("�˸��ƽ�"), strDMAlias);
		if(pDM->GetCellData(_T("�̸�"), i) == strTitle &&
			pDM->GetCellData(_T("�˸��ƽ�"), i) == strDMAlias)
		{
			pItem->GetCellData(_T("�켱����"), strTmpData);
			pDM->SetCellData(_T("�켱����"), strTmpData, i);
			pItem->GetCellData(_T("���Ĺ��"), strTmpData);
			pDM->SetCellData(_T("���Ĺ��"), strTmpData, i);
		}
	}

	return 0;
}
