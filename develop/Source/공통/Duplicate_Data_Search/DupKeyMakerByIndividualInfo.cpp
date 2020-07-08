// DupKeyMakerByIndividualInfo.cpp: implementation of the CDupKeyMakerByIndividualInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DupKeyMakerByIndividualInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDupKeyMakerByIndividualInfo::CDupKeyMakerByIndividualInfo(CESL_Mgr * pMgr) : CDupKeyMaker(pMgr)
{

}

CDupKeyMakerByIndividualInfo::~CDupKeyMakerByIndividualInfo()
{

}

INT CDupKeyMakerByIndividualInfo::SetMarc(CString strMarc)
{
	if(strMarc.IsEmpty()) return -1;
	INT ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &m_cMarc);
	if(ids < 0) return -1;
	
	return 0;
}

INT CDupKeyMakerByIndividualInfo::MakeKeyDM()
{
	if(m_pKeyDM == NULL) return -1;
	INT ids;
	m_pKeyDM->FreeData();

	ids = ExtractKeyByMarc(&m_cMarc);
	if(ids < 0) return ids;

	m_pKeyDM->SetCellData(_T("BB_��ü����"), m_strMediaCodeInfo, 0);
	m_pKeyDM->SetCellData(_T("BB_�����"), m_strVolInfo, 0);
	m_pKeyDM->SetCellData(_T("BB_�Ǽ���"), m_strVolTitleInfo, 0);
	m_pKeyDM->SetCellData(_T("BB_��ġ��ȣ"), m_strShelfCodeInfo, 0);
	m_pKeyDM->SetCellData(_T("BB_��ϱ���"), m_strRegCodeInfo, 0);
	
	return 0;
}
