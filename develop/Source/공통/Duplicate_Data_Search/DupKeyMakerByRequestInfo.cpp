// DupKeyMakerByRequestInfo.cpp: implementation of the CDupKeyMakerByRequestInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DupKeyMakerByRequestInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDupKeyMakerByRequestInfo::CDupKeyMakerByRequestInfo(CESL_Mgr * pMgr) : CDupKeyMaker(pMgr)
{
	m_pSourceDM = NULL;
	m_pSourceGrid = NULL;
}

CDupKeyMakerByRequestInfo::~CDupKeyMakerByRequestInfo()
{

}

INT CDupKeyMakerByRequestInfo::MakeKeyDM()
{
	if(m_pKeyDM == NULL || m_pSourceGrid == NULL || m_pSourceDM == NULL) return -1;

	m_pKeyDM->FreeData();
	
	CString strTitle, strAuthor, strPublisher;
	INT ids = m_pSourceGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = m_pSourceGrid->SelectGetHeadPosition();
	INT nRow = -1;	
	// 17/05/10 ������ : �����˻� ���� ('����˻�', 'ISBN�˻�'���� ����)
//*/ ADD ---------------------------------------------------------------------------
	CString strISBN = _T("");
//*/ END ---------------------------------------------------------------------------

	while(i >= 0)
	{
		strTitle = m_pSourceDM->GetCellData(_T("��ǥ��"), i);
		strAuthor = m_pSourceDM->GetCellData(_T("������"), i);
		strPublisher = m_pSourceDM->GetCellData(_T("������"), i);
		// 17/05/10 ������ : �����˻� ���� ('����˻�', 'ISBN�˻�'���� ����)
/*//BEFORE----------------------------------------------------------------------------------------
		if(strTitle.IsEmpty())
*///AFTER-----------------------------------------------------------------------------------------
		strISBN = m_pSourceDM->GetCellData(_T("ISBN"), i); 
		if(strTitle.IsEmpty() && strISBN.IsEmpty())
/*//END-------------------------------------------------------------------------------------------*/		
		{
			if(strAuthor.IsEmpty() && strPublisher.IsEmpty())
			{
				i = m_pSourceGrid->SelectGetNext();
				continue;
			}
		}
		m_pKeyDM->InsertRow(-1);
		nRow = m_pKeyDM->GetRowCount()-1;
		m_pKeyDM->SetCellData(_T("IBS_��ǥ��"), strTitle, nRow);
		// 17/05/10 ������ : �����˻� ���� ('����˻�', 'ISBN�˻�'���� ����)
//*/ ADD ---------------------------------------------------------------------------
		m_pKeyDM->SetCellData(_T("BB_����_ISBN"), strISBN, nRow);
//*/ END ---------------------------------------------------------------------------
		//=====================================================
		//2009.01.05 ADD PJW : ���ڿ��Ҿ� ���� 
		strAuthor.TrimLeft();	strAuthor.TrimRight();
		RemoveRoleWord(strAuthor);
		//=====================================================

		m_pKeyDM->SetCellData(_T("IBS_������"), strAuthor, nRow);
		m_pKeyDM->SetCellData(_T("IBS_������"), strPublisher, nRow);
		m_pKeyDM->SetCellData(_T("IBS_�����"), m_pSourceDM->GetCellData(_T("�����"), i), nRow);
		m_pKeyDM->SetCellData(_T("IBS_��KEY"), m_pSourceDM->GetCellData(_T("��ġ���KEY"), i), nRow);

		i = m_pSourceGrid->SelectGetNext();
	}
	
	return 0;
}
