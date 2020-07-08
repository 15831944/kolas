// CopyCodeMgr.cpp: implementation of the CCopyCodeMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CopyCodeMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCopyCodeMgr::CCopyCodeMgr()
{
	m_bRegCodeYN = FALSE;
	m_bVolYN = FALSE;
	m_bSeShelfCodeYN = FALSE;
}

CCopyCodeMgr::~CCopyCodeMgr()
{
}

INT CCopyCodeMgr::Init(CString strConnectionInfo)
{
	if(strConnectionInfo.IsEmpty()) return -1;
	CString strQuery, strTmpData;
	strQuery.Format(
		_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_ALIAS='������ȣ�ο����'"));
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(strConnectionInfo);
	tmpDM.RestructDataManager(strQuery);
	INT nRowCnt = tmpDM.GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = tmpDM.GetCellData(i, 0);
		if		(strTmpData == _T("��ϱ��к�"))	m_bRegCodeYN = TRUE;
		else if	(strTmpData == _T("������"))		m_bVolYN = TRUE;
		else if	(strTmpData == _T("��ġ��ȣ��"))	m_bSeShelfCodeYN = TRUE;
	}
	return 0;
}

VOID CCopyCodeMgr::SetSrcInfo(CString strVol, CString strRegCode, CString strSeShelfCode)
{
	m_strVol		 = strVol;
	m_strRegCode	 = strRegCode;
	m_strSeShelfCode = strSeShelfCode;
}

BOOL CCopyCodeMgr::IsCopy(CESL_DataMgr *pDM_BOOK, INT idx)
{
	if( pDM_BOOK == NULL )	return FALSE;
	CString strAlias[3] = {
		_T("BB_�����"), _T("BB_��ϱ���_�ڵ�"), _T("BB_��ġ��ȣ_�ڵ�")
	};
	CString strData[3];

	pDM_BOOK->GetCellData(strAlias, 3, strData, idx);
	if( m_bVolYN && m_strVol != strData[0] )					return FALSE;
	if( m_bRegCodeYN && m_strRegCode != strData[1] )			return FALSE;
	if( m_bSeShelfCodeYN && m_strSeShelfCode != strData[2] )	return FALSE;
	return TRUE;
}
