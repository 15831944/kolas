// DivisionNoInfo.cpp: implementation of the CDivisionNoInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DivisionNoInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDivisionNoInfo::CDivisionNoInfo(CESL_Mgr * pParentMgr) : CGroupInfo(pParentMgr)
{
	m_pWorkNoInfo = NULL;
}

CDivisionNoInfo::~CDivisionNoInfo()
{
	DeleteWorkNoInfo();
}

INT CDivisionNoInfo::DeleteWorkNoInfo()
{
	if(m_pWorkNoInfo)
	{
		delete m_pWorkNoInfo;
		m_pWorkNoInfo = NULL;
	}
	return 0;
}

INT CDivisionNoInfo::InitGroupInfo(CString strConnectionInfo, CString strDMAlias)
{
	INT ids = InitDataManager(strConnectionInfo, strDMAlias);
	if(ids < 0) return -1;

	DeleteWorkNoInfo();
	m_pWorkNoInfo = new CWorkNoInfo(m_pParentMgr);
	ids = m_pWorkNoInfo->InitDataManager(strConnectionInfo);
	if(ids < 0) return -1;

	m_MapDMAliasMap.RemoveAll();
	m_MapDMAliasMap.SetAt(_T("���Գ⵵"), _T("���Գ⵵"));
	m_MapDMAliasMap.SetAt(_T("�˼��Ϸ�����"), _T("�˼��Ϸ�����"));
	m_MapDMAliasMap.SetAt(_T("�ߺ����翩��"), _T("�ߺ����翩��"));
	m_MapDMAliasMap.SetAt(_T("�������翩��"), _T("�������翩��"));
	m_MapDMAliasMap.SetAt(_T("å��"), _T("å��"));
	m_MapDMAliasMap.SetAt(_T("�������۾���ȣ"), _T("�������۾���ȣ"));
	m_MapDMAliasMap.SetAt(_T("�ֹ�����"), _T("�ֹ�����"));
	m_MapDMAliasMap.SetAt(_T("REC_KEY"), _T("��������KEY"));
	m_MapDMAliasMap.SetAt(_T("���ǿϷῩ��"), _T("���ǿϷῩ��"));
	m_MapDMAliasMap.SetAt(_T("�׷��ȣ"), _T("������ȣ"));
	m_MapDMAliasMap.SetAt(_T("�׷������"), _T("����������"));
	m_MapDMAliasMap.SetAt(_T("�׷������"), _T("����������"));
	m_MapDMAliasMap.SetAt(_T("�۾�����"), _T("�۾�����"));
	m_MapDMAliasMap.SetAt(_T("��������"), _T("��������"));
	m_MapDMAliasMap.SetAt(_T("�۾���ȣ��뿩��"), _T("�۾���ȣ��뿩��"));
	m_MapDMAliasMap.SetAt(_T("å�Ϸù�ȣ"), _T("å�Ϸù�ȣ"));
	return 0;
}

BOOL CDivisionNoInfo::IsEqual(CGroupInfo * pGroup)
{
	CStringArray ArrAlias, ArrMyData, ArrOtherData;
	ArrAlias.RemoveAll();	ArrMyData.RemoveAll();	ArrOtherData.RemoveAll();
	ArrAlias.Add(_T("���Գ⵵"));	ArrAlias.Add(_T("�׷��ȣ"));	ArrAlias.Add(_T("��������"));	ArrAlias.Add(_T("�۾�����"));
	
	INT ids = ReplaceAlias(&ArrAlias);
	if(ids < 0) return FALSE;
	ids = GetCellData(&ArrAlias, &ArrMyData);
	if(ids < 0) return FALSE;
	ids = GetCellData(&ArrAlias, &ArrOtherData);
	if(ids < 0) return FALSE;

	INT nCnt = ArrMyData.GetSize();
	for(INT i=0 ; i<nCnt ; i++)
	{
		if(ArrMyData.GetAt(i) != ArrOtherData.GetAt(i))
			return FALSE;
	}
	return TRUE;
}

INT CDivisionNoInfo::InitBookSerialNo()
{
	return 0;
}

INT CDivisionNoInfo::UpdateLastBookSerialNo()
{
	return 0;
}

INT CDivisionNoInfo::InitOrderSerialNo()
{
	if(m_pDM == NULL) return -1;
	CString strQuery, strValue, strAcqYear, strGroupInfo, strWorkCode;
	INT ids = GetAcqYear(strAcqYear);
	if(ids < 0) return -1;
	ids = GetGroupInfo(strGroupInfo);
	if(ids < 0) return -1;
	ids = GetOtherInfo(_T("��������"), strWorkCode);
	if(ids < 0) return -1;

	strQuery.Format(
		_T("SELECT MAX(P.SERIAL_NO) FROM IDX_BO_TBL I, BO_PURCHASEINFO_TBL P ")
		_T("WHERE I.REC_KEY=P.SPECIES_KEY AND ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s ")
		_T("AND I.WORK_CODE='%s' AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
//DEL 		_T("AND I.WORK_CODE='%s'"),
		strAcqYear, strGroupInfo, strWorkCode);
	ids = m_pDM->GetOneValueQuery(strQuery, strValue);
	if(ids < 0) return -1;
	m_strOrderSerialNo = strValue;
	return 0;
}

INT CDivisionNoInfo::InitBookSerialNoInDivNo()
{
	if(m_pDM == NULL) return -1;
	CString strQuery, strValue, strAcqYear, strGroupInfo, strWorkCode;
	INT ids = GetAcqYear(strAcqYear);
	if(ids < 0) return -1;
	ids = GetGroupInfo(strGroupInfo);
	if(ids < 0) return -1;
	ids = GetOtherInfo(_T("��������"), strWorkCode);
	if(ids < 0) return -1;
	
	strQuery.Format(
		_T("SELECT MAX(B.SERIAL_NO) FROM IDX_BO_TBL I, BO_BOOK_TBL B ")
		_T("WHERE I.REC_KEY=B.SPECIES_KEY AND ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s ")
		_T("AND I.WORK_CODE='%s' AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
//DEL 		_T("AND I.WORK_CODE='%s'"),
		strAcqYear, strGroupInfo, strWorkCode);
	ids = m_pDM->GetOneValueQuery(strQuery, strValue);
	if(ids < 0) return -1;
	m_strBookSerialNoInDivNo = strValue;
	return 0;
}

VOID CDivisionNoInfo::SetOrderSerialNo(INT nOrderSerialNo)
{
	m_strOrderSerialNo.Format(_T("%d"), nOrderSerialNo);
}

VOID CDivisionNoInfo::SetBookSerialNoInDivNo(INT nBookSerialNo)
{
	m_strBookSerialNoInDivNo.Format(_T("%d"), nBookSerialNo);
}

INT CDivisionNoInfo::GetOrderSerialNoToInt()
{
	return _ttoi(m_strOrderSerialNo.GetBuffer(0));
}

INT CDivisionNoInfo::GetBookSerialNoInDivNoToInt()
{
	return _ttoi(m_strBookSerialNoInDivNo.GetBuffer(0));
}
