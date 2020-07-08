// SeAppendixExpApi.cpp: implementation of the CSeAppendixExpApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SeAppendixExpApi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeAppendixExpApi::CSeAppendixExpApi(CESL_Mgr *pParentMgr)
{
	m_pMgr = pParentMgr;
	m_pDM_EXP = NULL; 
	m_pDM = NULL; 
}

CSeAppendixExpApi::~CSeAppendixExpApi()
{

}


INT CSeAppendixExpApi::InitAppendixExpMgr(CString strACQ_KEY, INT nACQ_TYPE)
{
	if(strACQ_KEY.IsEmpty())
	{
		AfxMessageBox(_T("[�ηϿ���] ���Ա���KEY�� ã�� �� �����ϴ�"));
		return -1;
	}

	m_pDM = m_pMgr->FindDM(DM_ALIAS);
	m_pDM_EXP = m_pMgr->FindDM(DM_EXP_ALIAS);
	
	if(!m_pDM || !m_pDM_EXP)
	{
		AfxMessageBox(_T("[�ηϿ���] �ηϰ���DM�� ã�� �� �����ϴ�"));
		return -2;
	}
	m_pDM->FreeData();
	CString strWhere;
	strWhere.Format(_T("ACQ_CODE = '%d' AND ACQ_KEY = %s"),nACQ_TYPE,strACQ_KEY);
	m_pDM_EXP->RefreshDataManager(strWhere);

	INT nRow = m_pDM_EXP->GetRowCount();
	
	if(nRow>0)
	{
		
		const INT cnt = 4;
		CString alias[cnt];
		CString data[cnt];
		INT nIdx;

		alias[0] = _T("�ηϸ�");
		alias[1] = _T("���ı���");
		alias[2] = _T("��ü����");
		alias[3] = _T("��⿩��");
				
		for(INT i=0;i<nRow;i++)
		{
			m_pDM->InsertRow(-1);
			nIdx = m_pDM->GetRowCount()-1;
			m_pDM_EXP->GetCellData(alias,4,data,i);
			m_pDM->SetCellData(alias,4,data,nIdx);

		}
	}

	return 0;
}