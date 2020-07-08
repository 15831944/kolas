// BO_BookData.cpp: implementation of the CBO_BookData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_BookData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_BookData::CBO_BookData(CESL_Mgr * pParentMgr) : CBO_BusinessData(pParentMgr)
{

}

CBO_BookData::~CBO_BookData()
{

}

INT CBO_BookData::ApplyDM(CESL_DataMgr * pDM, INT nIdx)
{
	if(m_pDM == NULL || pDM == NULL) return -1;
	if(nIdx >= pDM->GetRowCount()) return -1;
	
	if(nIdx < 0)
	{
		pDM->InsertRow(-1);
		nIdx = pDM->GetRowCount() - 1;
	}
	
	INT ids ;
	
	INT nColCnt = pDM->GetRefColumnCount();
	INT nPos;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strTmpData;
	for(INT i=0 ; i<nColCnt ; i++)
	{
		pDM->GetColumnRefInfo(i, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
		strTmpData = FIELD_ALIAS.Left(2);
		nPos = FIELD_NAME.Find(_T("."), 0);
		if(nPos >= 0) FIELD_NAME = FIELD_NAME.Mid(nPos+1);
		if(strTmpData == _T("BB")) 
		{
			ids = ApplyDMOneCell(pDM, nIdx, i, FIELD_NAME);
			if(ids < 0) return -1;
		}
	}
	return 0;		
}

INT CBO_BookData::ApplyMarcToData(CMarc * pMarc)
{
	if(pMarc == NULL || m_pParentMgr == NULL || m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	if(m_ArrDataAlias.GetSize() == 0) return 0;

	CString strData1, strData2, strData3;
	INT ids;
	// �Ϲ����� ����Ÿ ó��
	for (INT i = 0; i < m_ArrDataAlias.GetSize(); i++) 
	{
		strData1 = _T("");
		ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, m_ArrMarcAlias.GetAt(i), strData1);
		if(ids < 0) return -1;
		m_pDM->SetCellData(m_ArrDataAlias.GetAt(i), strData1, 0);
	}

	//260$c�� �����Ͱ� ������ 008 7-10�� �����Ͱ� ������ �ش� �����͸� ���� ����⿡ �����Ų��.
	strData1 = m_pDM->GetCellData(_T("BB_�����"), 0);
	if(strData1.IsEmpty())
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@7-10"), strData1);
		m_pDM->SetCellData(_T("BB_�����"), strData1, 0);
	}
	
	// �����(��ũ->�� ����ô� ���� 1���� ��츸 ��ǥå�� �������ش�.)
/*
	if (nVolumes == 1) 
	{
		strData1 = _T("");
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$n"), strData1);
		m_pDM->SetCellData(_T("BB_�����"), strData1, 0);
	}
	// ����(�Ǽ���)
	if (nVolumes == 1) 
	{
		strData1 = _T("");
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$p"), strData1);
		m_pDM->SetCellData(_T("BB_������ηϸ�"), strData1, 0);
	}


	// ���� ����
	CString strPrice;
	strData1 = _T("");
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strData1);
	if (strData1.GetLength() > 0)
	{
		ids = GetDigitData(strData1, strPrice);
		if(ids < 0) return -1;

		if(nVolumes == 1)
			m_pDM->SetCellData(_T("BB_����"), strData2, 0);
	}	
	strData1.Empty();
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$a"), strData1);
	if(strData1.GetLength() > 0)
	{
		if(nVolumes == 1)
			m_pDM->SetCellData(_T("BB_���ݼ���"), strData1, 0);
	}
	strData1.Empty();
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$c"), strData1);
	if(strData1.GetLength() > 0)
	{
		if(nVolumes == 1)
			m_pDM->SetCellData(_T("BB_����������"), strData1, 0);
	}

	// ������
	strData1 = _T("");
	ids = Get_AUTHOR(pMarc, strData1);
	if(ids < 0) return -1;
	m_pDM->SetCellData(_T("BB_������"), strData1, 0);

	// Ư���� ����Ÿ ó�� - 020(����)		// 1���� ���� IDX�� ��ǥBOOK�� 1�� �̻��ϰ��� ����
	CString strMarcData1;
	INT nPos;
	if (nVolumes == 1) 
	{
		FindItemByIndicator(pMarc, _T("020$a"), _T("  "), strMarcData1);
		//isbn�� 893726387382(�����)�̷��� ����. �׷��� '()'���� ���ڴ� �������Ѵ�.
		nPos = strMarcData1.Find(_T("("), 0);
		if(nPos >= 0) strMarcData1 = strMarcData1.Mid(0, nPos);
		m_pDM->SetCellData(_T("BB_����ISBN"), strMarcData1, 0);

		FindItemByIndicator(pMarc, _T("020$g"), _T("  "), strMarcData1);
		//���Ϲ��Խ� �ΰ���ȣ �������� - �ΰ���ȣ�� TCHAR(5)�̴� ���� �� �̻��� ��� Error�޽����� ����ؾ��Ѵ�.
		if(strMarcData1.GetLength() > 5) 
		{
			strData1.Format(_T("����ISBN �ΰ���ȣ[%s]�� ������ �ֽ��ϴ�. �ش� ������ ������� �ʽ��ϴ�."), strMarcData1);
			m_pParentMgr->ESLAfxMessageBox(strData1);
			strMarcData1.Empty();
		}
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strMarcData1);
		m_pDM->SetCellData(_T("BB_����ISBN�ΰ���ȣ"), strMarcData1, 0);

		FindItemByIndicator(pMarc, _T("020$z"), _T("  "), strMarcData1);
		m_pDM->SetCellData(_T("BB_���ǿ���ISBN"), strMarcData1, 0);
	}
*/
	return 0;
}

INT CBO_BookData::ApplyDataToMarc(CMarc * pMarc)
{
	if(pMarc == NULL || m_pParentMgr == NULL || m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	if(m_ArrDataAlias.GetSize() == 0) return 0;
	
	CString strData1, strData2, strData3;

	// �Ϲ����� ����Ÿ ó��
	for (INT i = 0; i < m_ArrDataAlias.GetSize(); i++) {
		strData1.Empty();
		m_pDM->GetCellData(m_ArrDataAlias.GetAt(i), 0, strData1);
		if (strData1.GetLength() > 0) 
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, m_ArrMarcAlias.GetAt(i), strData1, _T(""), MODIFY_ITEM);
		else 
			m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, m_ArrMarcAlias.GetAt(i));
	}
	
/*
	// ����(�Ǽ���)
	if (pDM_VOLUME->GetRowCount() == 1) 
	{
		strData1 = _T("");
		m_pDM->GetCellData(_T("BB_������ηϸ�"), 0, strData1);
		if (strData1.GetLength() > 0) 
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$p"), strData1);
		else 
			m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$p"));
	}
	
	SetISBN_to_Marc_ByVolume(pMarc, pDM_VOLUME, _T("020"), _T("  "));		// IBS_����_ISBN is 020 blank blank
	*/
	
	return 0;
}

INT CBO_BookData::InitBusinessData(CString strConnectionInfo, CString strDMAlias/*=_T("")*/)
{
	if(strConnectionInfo.IsEmpty()) strConnectionInfo = m_pParentMgr->CONNECTION_INFO;
	if(!strDMAlias.IsEmpty()) m_strDMAlias = strDMAlias;
	
	if(strConnectionInfo.IsEmpty() || m_strDMAlias.IsEmpty()) return -1;
	
	INT ids = InitDataManager(strConnectionInfo, m_strDMAlias);
	if(ids < 0) return -1;
	
	m_ArrDataAlias.RemoveAll();	m_ArrMarcAlias.RemoveAll();
	m_ArrDataAlias.Add(_T("BB_�����"));		m_ArrDataAlias.Add(_T("IBS_�����ڷ�"));
	m_ArrDataAlias.Add(_T("BB_�����"));		m_ArrDataAlias.Add(_T("BB_������Ư��"));	
	m_ArrDataAlias.Add(_T("BB_ũ��"));			
	m_ArrMarcAlias.Add(_T("260$c"));			m_ArrMarcAlias.Add(_T("300$e"));
	m_ArrMarcAlias.Add(_T("300$a"));			m_ArrMarcAlias.Add(_T("300$b"));
	m_ArrMarcAlias.Add(_T("300$c"));			
	
	return 0;
}
