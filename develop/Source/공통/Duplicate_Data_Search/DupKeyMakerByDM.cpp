// DupKeyMakerByDM.cpp: implementation of the CDupKeyMakerByDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DupKeyMakerByDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDupKeyMakerByDM::CDupKeyMakerByDM(CESL_Mgr * pMgr) : CDupKeyMaker(pMgr)
{
	m_pSourceDM = NULL;
	m_pSourceGrid = NULL;
}

CDupKeyMakerByDM::~CDupKeyMakerByDM()
{

}

INT CDupKeyMakerByDM::MakeKeyDM()
{
	if(m_pKeyDM == NULL || m_pSourceDM == NULL || m_pSourceGrid == NULL) return -1;
	
	m_pKeyDM->FreeData();
	
	INT cnt = -1;
	INT nPos = -1;
	INT nCurIdx;
	CMarc marc;
	CString strMarc, title, author, publisher, pubyear, isbn, strTmpData, regNo;
	CArray<CString, CString&> arrIsbn;
	
	INT ids = m_pSourceGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = m_pSourceGrid->SelectGetHeadPosition();
	if(i < 0) return -1;

	while(i >= 0)
	{
		m_pSourceDM->GetCellData(_T("BS_MARC"), i, strMarc);
		if(strMarc.GetLength() == 0)
		{
			i = m_pSourceGrid->SelectGetNext();
			continue;
		}
		ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if(ids < 0) 
		{
			i = m_pSourceGrid->SelectGetNext();
			continue;
		}

		nCurIdx = ExtractKeyByMarc(&marc);
		if(nCurIdx < 0)
		{
			i = m_pSourceGrid->SelectGetNext();
			continue;
		}
		m_pKeyDM->SetCellData(_T("IBS_��KEY"), m_pSourceDM->GetCellData(_T("IBS_��KEY"), i), nCurIdx);
		m_pKeyDM->SetCellData(_T("BB_��ü����"), m_pSourceDM->GetCellData(_T("IBS_��ü����"), i), nCurIdx);
		m_pKeyDM->SetCellData(_T("BB_��ġ��ȣ"), m_pSourceDM->GetCellData(_T("IBS_���κ�ġ��ȣ"), i), nCurIdx);
		m_pKeyDM->SetCellData(_T("BB_��ϱ���"), m_pSourceDM->GetCellData(_T("IBS_���ε�ϱ���"), i), nCurIdx);
		m_pKeyDM->SetCellData(_T("BB_�����"), m_pSourceDM->GetCellData(_T("IBS_���������"), i), nCurIdx);
		m_pKeyDM->SetCellData(_T("BB_�Ǽ���"), m_pSourceDM->GetCellData(_T("IBS_���αǼ���"), i), nCurIdx);
		//===================================================
		//2010.11.29 ADD BY PJW : �������� �Ѽ����� �߰��Ѵ�.
//		m_pKeyDM->SetCellData(_T("UDF_�Ѽ���"), m_pSourceDM->GetCellData(_T("UDF_�Ѽ���"), i), nCurIdx);
		//===================================================
		m_pKeyDM->SetCellData(_T("BS_MARC"), m_pSourceDM->GetCellData(_T("BS_MARC"), i), nCurIdx);
		// 17/05/10 ������ : �����˻� ���� ('����˻�', 'ISBN�˻�'���� ����)
//*/ ADD ---------------------------------------------------------------------------
		//m_pKeyDM->SetCellData(_T("IBS_ISBN"), m_pSourceDM->GetCellData(_T("BB_����_ISBN"), i), nCurIdx);//IBS_ISBN
//*/ END ---------------------------------------------------------------------------
		//JOB.2018.0018 EDIT BY KYJ �����ڷ���� ISBN���� �������簡 ���� �ʴ� ���� ���� 
		//m_pKeyDM->SetCellData(_T("BB_����_ISBN"), m_pSourceDM->GetCellData(_T("BB_����_ISBN"), i), nCurIdx);
		//JOB.2019.0036 EDIT BY KYJ
		m_pKeyDM->SetCellData(_T("BB_����_ISBN"), m_pSourceDM->GetCellData(_T("BB_����ISBN"), i), nCurIdx);
		i = m_pSourceGrid->SelectGetNext();
	}
	return 0;
}
