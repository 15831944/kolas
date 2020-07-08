// AppendixDataMgr.cpp: implementation of the CAppendixDataMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppendixDataMgr.h"
#include "../INCLUDE/BO_ACQ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("������API"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppendixDataMgr::CAppendixDataMgr(CESL_Mgr* pParent /*=NULL*/)
{
	m_pParentMgr = pParent;
	m_pAppendixDM = NULL;
	m_bDMCreate = FALSE;
}

CAppendixDataMgr::~CAppendixDataMgr()
{
	if (m_pAppendixDM && m_bDMCreate) 
	{
		delete m_pAppendixDM;
		m_pAppendixDM = NULL;
	}
}

INT CAppendixDataMgr::SetDataMgr(CESL_DataMgr *pDM)
{
	if(m_pParentMgr == NULL) return -1;
	if(pDM == NULL)
	{
		m_pAppendixDM = new CESL_DataMgr;
		if(m_pAppendixDM == NULL) return -1;
		m_pAppendixDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
		m_pAppendixDM->InitDMPKFromFile(_T("DMUP_����_��_��������"));
		m_pAppendixDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
		m_bDMCreate = TRUE;
	}
	else
	{
		m_pAppendixDM = pDM;
	}
	return 0;
}

INT CAppendixDataMgr::MakeBookQueryFrame(CString strSpecies_key, CString strPurchaseinfo_key, CESL_DataMgr *pDM_OUT, INT nSendCount/*=-1*/)
{
	INT ids, i;
	CString strQuery, strDBFLAG, strBook_key;
	INT nRowCount = m_pAppendixDM->GetRowCount();

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	// ���� I/U/D�� ���� Insert/Update/Delete�� �����Ѵ�.
	CString IsREPRESENT_BOOK_YN;
	for (i = 0; i < nRowCount; i++) {
		strDBFLAG = _T("");
		ids = m_pAppendixDM->GetCellData(_T("BB_DB_FLAG"), i, strDBFLAG);
		if (strDBFLAG == _T("I")) {
			pDM_OUT->MakeRecKey(strBook_key); 
			m_pAppendixDM->SetCellData(_T("BB_åKEY"), strBook_key, i);
		}
		IsREPRESENT_BOOK_YN = _T("");
		ids = m_pAppendixDM->GetCellData(_T("BB_��ǥå����"), i, IsREPRESENT_BOOK_YN);
		IsREPRESENT_BOOK_YN.TrimLeft();		IsREPRESENT_BOOK_YN.TrimRight();
		IsREPRESENT_BOOK_YN.MakeUpper();
		if (IsREPRESENT_BOOK_YN != _T("Y") && IsREPRESENT_BOOK_YN != _T("N")) m_pAppendixDM->SetCellData(_T("BB_��ǥå����"), _T("N"), i);
	}

	//����������� ����� ���ؼ� �׸��� ISBN�Է½� Ư����ȣ�� �����ϱ� ���Ͽ�
	CString volIndex, strEAISBN, strBookAppendixFlag;
	//�������
	for (i = 0; i < nRowCount; i++) {
		RemoveAliasList.RemoveAll();
		strDBFLAG = _T("");
		ids = m_pAppendixDM->GetCellData(_T("BB_DB_FLAG"), i, strDBFLAG);
		if (strDBFLAG == _T("I")) {
			RemoveAliasList.Add(_T("BB_������Ϲ�ȣ"));
			m_pAppendixDM->SetCellData(_T("BB_��������KEY"), strPurchaseinfo_key, i);
			m_pAppendixDM->SetCellData(_T("BB_��KEY"), strSpecies_key, i);
			m_pAppendixDM->SetCellData(_T("BB_�������۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			m_pAppendixDM->SetCellData(_T("BB_�����۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			if (m_pAppendixDM->GetCellData(_T("BB_����"), i).GetLength() == 0) m_pAppendixDM->SetCellData(_T("BB_����"), _T("0"), i);
			//����������� ���� �� ����ISBN�� Ư����ȣ ������
			strEAISBN = m_pAppendixDM->GetCellData(_T("BB_����ISBN"), i); 
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strEAISBN);
			m_pAppendixDM->SetCellData(_T("BB_����ISBN"), strEAISBN, i);
			volIndex = m_pAppendixDM->GetCellData(_T("BB_������ηϸ�"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(volIndex);
				m_pAppendixDM->SetCellData(_T("BB_������ηϸ����"), volIndex, i);
			}
			volIndex = m_pAppendixDM->GetCellData(_T("BB_�����"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(volIndex);
				m_pAppendixDM->SetCellData(_T("BB_���������"), volIndex, i);
			}
			strBook_key = m_pAppendixDM->GetCellData(_T("BB_åKEY"), i);
			pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pAppendixDM, _T("BO_BOOK_TBL"), i, pDM_OUT);
		}
		if (strDBFLAG == _T("U")) {
			RemoveAliasList.Add(_T("BB_åKEY"));
			RemoveAliasList.Add(_T("BB_������Ϲ�ȣ"));
			ids = m_pAppendixDM->GetCellData(_T("BB_åKEY"), i, strBook_key);
			m_pAppendixDM->SetCellData(_T("BB_�������۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			//����������� ���� �� ����ISBN�� Ư����ȣ ������
			strEAISBN = m_pAppendixDM->GetCellData(_T("BB_����ISBN"), i); 
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strEAISBN);
			m_pAppendixDM->SetCellData(_T("BB_����ISBN"), strEAISBN, i);			
			volIndex = m_pAppendixDM->GetCellData(_T("BB_������ηϸ�"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				m_pAppendixDM->SetCellData(_T("BB_������ηϸ����"), volIndex, i);
			}
			volIndex = m_pAppendixDM->GetCellData(_T("BB_�����"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				m_pAppendixDM->SetCellData(_T("BB_���������"), volIndex, i);
			}
			pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pAppendixDM, _T("BO_BOOK_TBL"), i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strBook_key);
			strBook_key = m_pAppendixDM->GetCellData(_T("BB_åKEY"), i);
		}
		if (strDBFLAG == _T("D")) {
			ids = m_pAppendixDM->GetCellData(_T("BB_åKEY"), i, strBook_key);
			if(strBook_key.IsEmpty()) strBook_key = _T("-1");
			strQuery.Format(_T("DELETE FROM BO_BOOK_TBL WHERE REC_KEY=%s;"), strBook_key);
			pDM_OUT->AddFrame(strQuery);
			//å�� ���������� �����Ҷ� ��Ϲ�ȣ�� �ο��� �ڷῡ ���Ͽ� �ο��Ǿ��� ��Ϲ�ȣ�� ������Ϲ�ȣ�� �߰��Ѵ�.
			CString strRecNo = m_pAppendixDM->GetCellData(_T("BB_��Ϲ�ȣ"), i);
			if(!strRecNo.IsEmpty()) InsertToDropAccessionNoTbl(strRecNo, pDM_OUT, TRUE);
		}
	}

	return 0;
}

//�ηϿ� Default���� �����Ų��.
INT CAppendixDataMgr::SetDefaultValue(CESL_DataMgr * pDM_ACCU, INT nRowIdx, INT nSpeciesType) 
{
	if(!m_pAppendixDM) return -1;
	/*
	å ���� 
	- ���Ա��� : BB_���Ա���_�ڵ� : ���Ա���(1-����,2-����) �⺻���� 1��
	- �Է��� : BB_�Է��� : �Է���
	*/
	SPECIES_TYPE sType = (SPECIES_TYPE)nSpeciesType;

	CString INPUT_DATE, strTmpData;
	INPUT_DATE = GetTodayDate();

	INT nLastBookSerialNo = -1;
	CString strLastSerialNo;

	INT nRow = m_pAppendixDM->GetRowCount();
	for (INT i = 0; i < nRow; i++) 
	{
		if(m_pAppendixDM->GetCellData(_T("BB_DB_FLAG"), i) == _T("I") )
		{
			m_pAppendixDM->SetCellData(_T("BB_���̺���±���"), _T("1"), i);
			m_pAppendixDM->SetCellData(_T("BB_å�η��÷���"), _T("B"), i);
			m_pAppendixDM->SetCellData(_T("BB_�Է���"), INPUT_DATE, i);
			strTmpData = m_pAppendixDM->GetCellData(_T("BB_�ڷ����"), i);
			if(strTmpData != _T("BOA113O")) m_pAppendixDM->SetCellData(_T("BB_��ǰ����"), _T("Y"), i);
			else m_pAppendixDM->SetCellData(_T("BB_��ǰ����"), _T("N"), i);
			if(sType == BO_REG_PURCHASE || sType == BO_REG_DONATE || sType == BO_DONATE || sType == BO_REG_INVALID || sType == BO_REGOBJ)
				m_pAppendixDM->SetCellData(_T("BB_�˼���"), GetTodayDate(), i);
			if(sType == BO_REG_PURCHASE || sType == BO_REG_DONATE || sType == BO_REG_INVALID || sType == BO_REGOBJ)
				m_pAppendixDM->SetCellData(_T("BB_����ΰ�����"), GetTodayDate(), i);
		}
	}
	return 0;
}

CString CAppendixDataMgr::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CAppendixDataMgr::InsertToDropAccessionNoTbl(CString strRegNo, CESL_DataMgr * pDM_Out, BOOL IsCheckInsert)
{
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
// 	CString strCode = strRegNo.Left(2);
// 	INT nNumber = _ttoi((strRegNo.Right(10)).GetBuffer(0));
	CString strCode = strRegNo.Left(nRegCodeLength);
	INT nNumber = _ttoi((strRegNo.Right(12-nRegCodeLength)).GetBuffer(0));
	//===================================================================
	CString query, strTmpData;
	if(IsCheckInsert)	
	{
		query.Format(_T("SELECT REC_KEY FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE = '%s' AND REG_NO = %d MANAGE_CODE = UDF_MANAGE_CODE"), strCode, nNumber);
//DEL 		query.Format(_T("SELECT REC_KEY FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE = '%s' AND REG_NO = %d"), strCode, nNumber);
		pDM_Out->GetOneValueQuery(query, strTmpData);
		if(strTmpData.GetLength() != 0) return 0;
	}
	CString strRecKey;
	pDM_Out->MakeRecKey(strRecKey);
	query.Format(
		_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, REG_CODE, REG_NO, MANAGE_CODE) VALUES(%s, 'MO', '%s', %d, UDF_MANAGE_CODE);"),
//DEL 		_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, REG_CODE, REG_NO) VALUES(%s, 'MO', '%s', %d);"),
			strRecKey,
			strCode,
			nNumber);
	pDM_Out->AddFrame(query);
	return 0;
}
