// ReceiptNoInfo.cpp: implementation of the CReceiptNoInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReceiptNoInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("������ȣ����"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReceiptNoInfo::CReceiptNoInfo(CESL_Mgr * pParentMgr) : CGroupInfo(pParentMgr)
{

}

CReceiptNoInfo::~CReceiptNoInfo()
{

}

INT CReceiptNoInfo::InitGroupInfo(CString strConnectionInfo, CString strDMAlias)
{
	INT ids = InitDataManager(strConnectionInfo, strDMAlias);
	if(ids < 0) return -1;
	
	m_MapDMAliasMap.RemoveAll();
	m_MapDMAliasMap.SetAt(_T("���Գ⵵"), _T("���Գ⵵"));
	m_MapDMAliasMap.SetAt(_T("å��"), _T("å��"));
	m_MapDMAliasMap.SetAt(_T("REC_KEY"), _T("������ȣKEY"));
	m_MapDMAliasMap.SetAt(_T("�׷��ȣ"), _T("������ȣ"));
	m_MapDMAliasMap.SetAt(_T("�׷������"), _T("������ȣ������"));
	m_MapDMAliasMap.SetAt(_T("�׷������"), _T("������ȣ������"));
	m_MapDMAliasMap.SetAt(_T("å�Ϸù�ȣ"), _T("�������Ϸù�ȣ"));
	return 0;
}

BOOL CReceiptNoInfo::IsEqual(CGroupInfo * pGroup)
{
	CStringArray ArrAlias, ArrMyData, ArrOtherData;
	ArrAlias.RemoveAll();	ArrMyData.RemoveAll();	ArrOtherData.RemoveAll();
	ArrAlias.Add(_T("���Գ⵵"));	ArrAlias.Add(_T("�׷��ȣ"));
	
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

INT CReceiptNoInfo::InitBookSerialNo()
{
	return 0;
}

INT CReceiptNoInfo::UpdateLastBookSerialNo()
{
	CString strRecKey, strQuery, strLastSerialNo;
	INT ids = GetRecKey(strRecKey);
	if(ids < 0) return -1;
	if(strRecKey.IsEmpty()) return -1;

	ids = GetBookLastSerialNo(strLastSerialNo);
	if(ids < 0)	return -1;
	
	strQuery.Format(
		_T("UPDATE %s SET LAST_SERIAL_NO=%s, LAST_WORK='%s' WHERE REC_KEY=%s;"),
		m_pDM->TBL_NAME, strLastSerialNo, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strRecKey);
	m_pDM->StartFrame();
	ids = m_pDM->AddFrame(strQuery);
	if(ids < 0) return -1;
	ids = m_pDM->SendFrame();
	if(ids < 0) return -1;
	m_pDM->EndFrame();
	return 0;
}
