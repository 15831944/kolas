// SeApi.cpp: implementation of the CSeApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SeApi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "SeDuplicatedVolDlg.h"
#include "SeAppedixExpDlg.h"
#include "SeSpeciesDelete.h"
#include "..\..\..\����\IndexToolKit\IndexTool.h"
#include "..\..\..\����\IndexToolKit\Index.h"
#include "..\..\..\����\����\�弭����\BL_SE_LOC_1500\BL_SE_LOC_15.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeApi::CSeApi(CESL_Mgr *pParentMgr)
{
	m_pMgr = pParentMgr;
}

CSeApi::~CSeApi()
{

}

INT CSeApi::SPECIES_PURCHASE_INSERT(CESL_DataMgr* pDM_SPECIES, CESL_DataMgr* pDM_IDX, CESL_DataMgr* pDM_PURCHASE, CESL_DataMgr* pDM_SUB_HISTORY, CMarc* pMarc, INT nRowIdx)
{
	// ����-��-������ �Է�(������ �ƴ϶�� ����) 
	// ���ο� 1 row insert
	// ���� 1 row insert
	// ���������� 1 row insert
	// �����̷������� 1row insert 
	// ���������� 1 row insert(REC_KEY) 
	
	
	if(!pDM_SPECIES || !pDM_IDX || !pDM_PURCHASE || !pDM_SUB_HISTORY || !pMarc)
	{
		AfxMessageBox(_T("����-��-������ �Է� ���������� ã�� �� �����ϴ�!"));
		return -1;
	}
	
	//�����ӿ��� ���Ǵ� DM free
	pDM_IDX->FreeData();
	pDM_SUB_HISTORY->FreeData();
	
	// ���� DM�� 1row insert
	pDM_IDX->InsertRow(-1);
	
	// ���� �̷� DM�� 1row insert
	pDM_SUB_HISTORY->InsertRow(-1);

	// ���� �̷¿� REC_KEY����� �ش�
	CString strSUB_HISTORY_KEY;
	pDM_SUB_HISTORY->MakeRecKey(strSUB_HISTORY_KEY);
	pDM_SUB_HISTORY->SetCellData(_T("�����̷�KEY"),strSUB_HISTORY_KEY,nRowIdx);
	pDM_PURCHASE->SetCellData(_T("SP_�����̷�����KEY"),strSUB_HISTORY_KEY,nRowIdx);


	CArray <CString, CString> RemoveAliasList;

	INT ids;
	CString strSpecies_key, strPurchase_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_��KEY"),nRowIdx); 
	strPurchase_key = pDM_PURCHASE->GetCellData(_T("SP_���԰���KEY"),nRowIdx); 	
	

	//���������� �����̷������� ����� 
	MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(pDM_PURCHASE, pDM_SUB_HISTORY ,nRowIdx);


	//Default ���� ���� ��Ų��.
	SetInsertDefaultValue_SPECIES(pDM_SPECIES,pMarc,nRowIdx);
	SetInsertDefaultValue_PURCHASE(pDM_PURCHASE, nRowIdx);
	SetInsertDefaultValue_IDX(pDM_IDX, nRowIdx);
	SetInsertDefaultValue_SUB_HISTORY(pDM_SUB_HISTORY, nRowIdx);


	//���� ������ ��Ų�� 
	pDM_SPECIES->SetCellData(_T("SS_���Կ���"),_T("Y"),nRowIdx);
	pDM_SPECIES->SetCellData(_T("SS_��������"),_T("N"),nRowIdx);
	
	

	//���� ��ũ�� ������ ������ �����Ѵ�
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);


	//���� Ȧ��KEY �� �����Ѵ�
	CString strHoldingKey;
	pDM_SPECIES->MakeRecKey(strHoldingKey);
	pDM_SPECIES->SetCellData(_T("SS_Ȧ��KEY"),strHoldingKey,nRowIdx);


	//startframe
	pDM_SPECIES->StartFrame();

	// ���� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES);


	// ���ο� Insert�� Insert���� �����Ѵ�.
	pDM_IDX->SetCellData(_T("ISS_��KEY"), strSpecies_key, nRowIdx);
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES);
	

	// ���԰��������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("SE_PURCHASE_MANAGE_TBL"), nRowIdx , pDM_SPECIES);


	// �����̷������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SUB_HISTORY, _T("SE_SUBSCRIPTION_HISTORY_TBL"), nRowIdx , pDM_SPECIES);


	// ���������� rec_key�� insert�Ѵ� 
 	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strHoldingKey);
	pDM_SPECIES->MakeInsertFrame(_T("SE_HOLDING_TBL"));
	
	
	//IDX_TMP_SE_TBL ����
	SetIdxTmpSeTbl(pDM_SPECIES, strSpecies_key, _T("I"));
	


	// ������ Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�� ���� ����!!!!"));
		return -1000;		// Species Insert Error
	}
	
	return 0;
}


INT CSeApi::SPECIES_PURCHASE_INSERT_DISCONTINUE(CESL_DataMgr* pDM_SPECIES,
												CESL_DataMgr* pDM_IDX, 
												CESL_DataMgr* pDM_PURCHASE, 
												CESL_DataMgr* pDM_SUB_HISTORY, 
												CMarc* pMarc, 
												CMarc* pOldMarc,
												CString strOldSpeciesKey,
												CString strDiscontinueReason,
												INT nRowIdx)
{
	// ����-��-������ �Է� �� ���� ������ UPDATE 
	// ���ο� 1 row insert
	// ���� 1 row insert
	// ���������� 1 row insert
	// �����̷������� 1 row insert 
	// ���������� 1 row insert(REC_KEY) 
	// ���� ���� MARC update
	// ���� ���ο� ���� ���� update('d' (��) ����)
	
	
	if(!pDM_SPECIES || !pDM_IDX || !pDM_PURCHASE || !pDM_SUB_HISTORY || !pMarc || !pOldMarc || strOldSpeciesKey.IsEmpty())
	{
		AfxMessageBox(_T("����-��-������ �Է� - ��ó�� ���������� ã�� �� �����ϴ�!"));
		return -1;
	}
	
	//�����ӿ��� ���Ǵ� DM free
	pDM_IDX->FreeData();
	pDM_SUB_HISTORY->FreeData();
	
	// ���� DM�� 1row insert
	pDM_IDX->InsertRow(-1);
	
	// ���� �̷� DM�� 1row insert
	pDM_SUB_HISTORY->InsertRow(-1);

	// ���� �̷¿� REC_KEY����� �ش�
	CString strSUB_HISTORY_KEY;
	pDM_SUB_HISTORY->MakeRecKey(strSUB_HISTORY_KEY);
	pDM_SUB_HISTORY->SetCellData(_T("�����̷�KEY"),strSUB_HISTORY_KEY,nRowIdx);
	pDM_PURCHASE->SetCellData(_T("SP_�����̷�����KEY"),strSUB_HISTORY_KEY,nRowIdx);


	CArray <CString, CString> RemoveAliasList;

	INT ids;
	CString strSpecies_key, strPurchase_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_��KEY"),nRowIdx); 
	strPurchase_key = pDM_PURCHASE->GetCellData(_T("SP_���԰���KEY"),nRowIdx); 	
	

	//���������� �����̷������� ����� 
	MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(pDM_PURCHASE, pDM_SUB_HISTORY ,nRowIdx);


	//Default ���� ���� ��Ų��.
	SetInsertDefaultValue_SPECIES(pDM_SPECIES,pMarc,nRowIdx);
	SetInsertDefaultValue_PURCHASE(pDM_PURCHASE, nRowIdx);
	SetInsertDefaultValue_IDX(pDM_IDX, nRowIdx);
	SetInsertDefaultValue_SUB_HISTORY(pDM_SUB_HISTORY, nRowIdx);


	//���� ������ ��Ų�� 
	pDM_SPECIES->SetCellData(_T("SS_���Կ���"),_T("Y"),nRowIdx);
	pDM_SPECIES->SetCellData(_T("SS_��������"),_T("N"),nRowIdx);
	
	
	//���� ��ũ�� ������ ������ �����Ѵ�
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);

	//���� Ȧ��KEY �� �����Ѵ�
	CString strHoldingKey;
	pDM_SPECIES->MakeRecKey(strHoldingKey);
	pDM_SPECIES->SetCellData(_T("SS_Ȧ��KEY"),strHoldingKey,nRowIdx);

	//startframe
	pDM_SPECIES->StartFrame();

	// ���� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES);


	// ���ο� Insert�� Insert���� �����Ѵ�.
	pDM_IDX->SetCellData(_T("ISS_��KEY"), strSpecies_key, nRowIdx);
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES);
	

	// ���԰��������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("SE_PURCHASE_MANAGE_TBL"), nRowIdx , pDM_SPECIES);


	// �����̷������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SUB_HISTORY, _T("SE_SUBSCRIPTION_HISTORY_TBL"), nRowIdx , pDM_SPECIES);


	// ���������� rec_key�� insert�Ѵ� 
 	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strHoldingKey);
	pDM_SPECIES->MakeInsertFrame(_T("SE_HOLDING_TBL"));
	
	
	//���� �� ���� UPDATE 
	CString strQuery;
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("UPDATE IDX_SE_TBL SET PUB_STATUS = 'd' WHERE REC_KEY = %s;"),strOldSpeciesKey);
	pDM_SPECIES->AddFrame(strQuery);
	
	
	//���� �� �� ���� UPDATE
	if(!strDiscontinueReason.IsEmpty())
	{
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET DISCONTINUE_REASON = '%s' WHERE REC_KEY = %s;"),strDiscontinueReason,strOldSpeciesKey);
		pDM_SPECIES->AddFrame(strQuery);
	}
	

	//���� �� �ļ� ���� insert
	CString strWorkLogMsg;
	CString strREC_KEY1,strREC_KEY2;

	strWorkLogMsg = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	pDM_SPECIES->MakeRecKey(strREC_KEY1);
	pDM_SPECIES->MakeRecKey(strREC_KEY2);
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO SE_PREV_SPECIES_TBL (REC_KEY,SPECIES_KEY,PREV_SPECIES_KEY,FIRST_WORK,LAST_WORK) VALUES (%s,%s,%s,'%s','%s');"),strREC_KEY1,strSpecies_key,strOldSpeciesKey,strWorkLogMsg,strWorkLogMsg);
	pDM_SPECIES->AddFrame(strQuery);
	
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO SE_NEXT_SPECIES_TBL (REC_KEY,SPECIES_KEY,NEXT_SPECIES_KEY,FIRST_WORK,LAST_WORK) VALUES (%s,%s,%s,'%s','%s');"),strREC_KEY2,strOldSpeciesKey,strSpecies_key,strWorkLogMsg,strWorkLogMsg);
	pDM_SPECIES->AddFrame(strQuery);

	

	//IDX_TMP_SE_TBL ����
	SetIdxTmpSeTbl(pDM_SPECIES, strSpecies_key, _T("I"));
	SetIdxTmpSeTbl(pDM_SPECIES, strOldSpeciesKey, _T("I"));

	
	// ������ Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�� ���� ����!!!!"));
		return -1000;		// Species Insert Error
	}
	
	//���� ��ũ���� update
	ids = UpdateSpeciesMarcbyCMarc(pDM_SPECIES,strOldSpeciesKey,pOldMarc);
	if (ids < 0) {
		AfxMessageBox(_T("��ũ ������Ʈ ����!!!!"));
		return -1200;		// Species Insert Error
	}


	return 0;
}

INT CSeApi::SPECIES_DONATE_INSERT(CESL_DataMgr* pDM_SPECIES, CESL_DataMgr* pDM_IDX, CESL_DataMgr* pDM_DONATE, CMarc* pMarc, INT nRowIdx)
{
	// ����-��-������ �Է�(������ �ƴ϶�� ����)
	// ���ο� 1 row insert
	// ���� 1 row insert
	// ���������� 1 row insert
	// ���������� 1 row insert (REC_KEY��)

	CArray <CString, CString> RemoveAliasList;

	if(!pDM_SPECIES || !pDM_IDX || !pDM_DONATE || !pMarc)
	{
		AfxMessageBox(_T("����-��-������ �Է� ���������� ã�� �� �����ϴ�!"));
		return -1;
	}
	
	//�����ӿ��� ���Ǵ� DM free
	pDM_IDX->FreeData();


	INT ids;
	CString strSpecies_key, strDonate_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_��KEY"),nRowIdx); 
	strDonate_key = pDM_DONATE->GetCellData(_T("SD_��������KEY"),0); 	
	
	// ���� DM�� 1row insert
	pDM_IDX->InsertRow(-1);
	

	//Default ���� ���� ��Ų��.
	SetInsertDefaultValue_SPECIES(pDM_SPECIES,pMarc,nRowIdx);
	SetInsertDefaultValue_DONATE(pDM_DONATE, nRowIdx);
	SetInsertDefaultValue_IDX(pDM_IDX, nRowIdx);
	

	//���������� ��Ų�� 
	pDM_SPECIES->SetCellData(_T("SS_��������"),_T("Y"),nRowIdx);
	pDM_SPECIES->SetCellData(_T("SS_���Կ���"),_T("N"),nRowIdx);
	
	
	//���� ��ũ�� ������ ������ �����Ѵ�
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);
	
	
	//���� Ȧ��KEY �� �����Ѵ�
	CString strHoldingKey;
	pDM_SPECIES->MakeRecKey(strHoldingKey);
	pDM_SPECIES->SetCellData(_T("SS_Ȧ��KEY"),strHoldingKey,nRowIdx);


	//startframe
	pDM_SPECIES->StartFrame();
	
	// ���� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES);


	// ���ο� Insert�� Insert���� �����Ѵ�.
	pDM_IDX->SetCellData(_T("ISS_��KEY"), strSpecies_key, nRowIdx);
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES);
	

	// �������������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_DONATE, _T("SE_DONATE_MANAGE_TBL"), nRowIdx , pDM_SPECIES);


	// ���������� rec_key�� insert�Ѵ� 
 	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strHoldingKey);
	pDM_SPECIES->MakeInsertFrame(_T("SE_HOLDING_TBL"));
	

	//IDX_TMP_SE_TBL ����
	SetIdxTmpSeTbl(pDM_SPECIES, strSpecies_key, _T("I"));


	// ������ Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�� ���� ����!!!!"));
		return -2000;		// Species Insert Error
	}
	

	return 0;
}

INT CSeApi::SPECIES_DONATE_INSERT_DISCONTINUE(CESL_DataMgr* pDM_SPECIES, 
										      CESL_DataMgr* pDM_IDX, 
										      CESL_DataMgr* pDM_DONATE, 
										      CMarc* pMarc,
										      CMarc* pOldMarc,
										      CString strOldSpeciesKey,
											  CString strDiscontinueReason,
										      INT nRowIdx)
{
	// ����-��-������ �Է� �� ���� �� �� ó�� 
	// ���ο� 1 row insert
	// ���� 1 row insert
	// ���������� 1 row insert
	// ���������� 1 row insert (REC_KEY��)
	// ���� ���� MARC update
	// ���� ���ο� ���� ���� update('d' (��) ����)

	CArray <CString, CString> RemoveAliasList;

	if(!pDM_SPECIES || !pDM_IDX || !pDM_DONATE || !pMarc || !pOldMarc || strOldSpeciesKey.IsEmpty())
	{
		AfxMessageBox(_T("����-��-������ �Է� - ��ó�� ���������� ã�� �� �����ϴ�!"));
		return -1;
	}
	
	//�����ӿ��� ���Ǵ� DM free
	pDM_IDX->FreeData();


	INT ids;
	CString strSpecies_key, strDonate_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_��KEY"),nRowIdx); 
	strDonate_key = pDM_DONATE->GetCellData(_T("SD_��������KEY"),0); 	
	
	// ���� DM�� 1row insert
	pDM_IDX->InsertRow(-1);
	

	//Default ���� ���� ��Ų��.
	SetInsertDefaultValue_SPECIES(pDM_SPECIES,pMarc,nRowIdx);
	SetInsertDefaultValue_DONATE(pDM_DONATE, nRowIdx);
	SetInsertDefaultValue_IDX(pDM_IDX, nRowIdx);
	

	//���������� ��Ų�� 
	pDM_SPECIES->SetCellData(_T("SS_��������"),_T("Y"),nRowIdx);
	pDM_SPECIES->SetCellData(_T("SS_���Կ���"),_T("N"),nRowIdx);
	
	
	//���� ��ũ�� ������ ������ �����Ѵ�
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);
	

	//���� Ȧ��KEY �� �����Ѵ�
	CString strHoldingKey;
	pDM_SPECIES->MakeRecKey(strHoldingKey);
	pDM_SPECIES->SetCellData(_T("SS_Ȧ��KEY"),strHoldingKey,nRowIdx);

	
	//startframe
	pDM_SPECIES->StartFrame();
	

	// ���� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES);


	// ���ο� Insert�� Insert���� �����Ѵ�.
	pDM_IDX->SetCellData(_T("ISS_��KEY"), strSpecies_key, nRowIdx);
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES);
	

	// �������������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_DONATE, _T("SE_DONATE_MANAGE_TBL"), nRowIdx , pDM_SPECIES);


	// ���������� rec_key�� insert�Ѵ� 
 	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strHoldingKey);
	pDM_SPECIES->MakeInsertFrame(_T("SE_HOLDING_TBL"));
	
	

	//���� �� ���� UPDATE 
	CString strQuery;
	strQuery.Format(_T("UPDATE IDX_SE_TBL SET PUB_STATUS = 'd' WHERE REC_KEY = %s;"),strOldSpeciesKey);
	pDM_SPECIES->AddFrame(strQuery);
	

	
	//���� �� �� ���� UPDATE
	if(!strDiscontinueReason.IsEmpty())
	{
		strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET DISCONTINUE_REASON = '%s' WHERE REC_KEY = %s;"),strDiscontinueReason,strOldSpeciesKey);
		pDM_SPECIES->AddFrame(strQuery);
	}
	

	//���� �� �ļ� ���� insert
	CString strWorkLogMsg;
	CString strREC_KEY1,strREC_KEY2;

	strWorkLogMsg = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	pDM_SPECIES->MakeRecKey(strREC_KEY1);
	pDM_SPECIES->MakeRecKey(strREC_KEY2);
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO SE_PREV_SPECIES_TBL (REC_KEY,SPECIES_KEY,PREV_SPECIES_KEY,FIRST_WORK,LAST_WORK) VALUES (%s,%s,%s,'%s','%s');"),strREC_KEY1,strSpecies_key,strOldSpeciesKey,strWorkLogMsg,strWorkLogMsg);
	pDM_SPECIES->AddFrame(strQuery);
	
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO SE_NEXT_SPECIES_TBL (REC_KEY,SPECIES_KEY,NEXT_SPECIES_KEY,FIRST_WORK,LAST_WORK) VALUES (%s,%s,%s,'%s','%s');"),strREC_KEY2,strOldSpeciesKey,strSpecies_key,strWorkLogMsg,strWorkLogMsg);
	pDM_SPECIES->AddFrame(strQuery);

	
	//IDX_TMP_SE_TBL ����
	SetIdxTmpSeTbl(pDM_SPECIES, strSpecies_key, _T("I"));
	SetIdxTmpSeTbl(pDM_SPECIES, strOldSpeciesKey, _T("I"));


	// ������ Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�� ���� ����!!!!"));
		return -2000;		// Species Insert Error
	}
	

	//���� ��ũ���� update
	ids = UpdateSpeciesMarcbyCMarc(pDM_SPECIES,strOldSpeciesKey,pOldMarc);
	if (ids < 0) {
		AfxMessageBox(_T("��ũ ������Ʈ ����!!!!"));
		return -2200;		// Species Insert Error
	}
	
	return 0;
}

INT CSeApi::SPECIES_PURCHASE_UPDATE(CESL_DataMgr *pDM_SPECIES, 
									CESL_DataMgr *pDM_IDX, 
									CESL_DataMgr *pDM_PURCHASE,
									CESL_DataMgr* pDM_SUB_HISTORY, 
									CESL_DataMgr* pDM_SPE_HISTORY, 
									CMarc* pMarc, 
									INT nRowIdx)
{
	// ����-��-����-������ ���� 
	// ���� 1 row update
	// ���ο� 1 row update
	// ���������� 1 row update
	
	CArray <CString, CString> RemoveAliasList;
	pDM_SPECIES->StartFrame();

	INT ids;
	CString strSpecies_key, strPurchase_key,strSubHistory_key;

	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_��KEY"),nRowIdx); 
	strPurchase_key = pDM_PURCHASE->GetCellData(_T("SP_���԰���KEY"),nRowIdx); 	
	strSubHistory_key = pDM_SUB_HISTORY->GetCellData(_T("�����̷�KEY"),nRowIdx);

	//���������� �����̷������� ����� 
	MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(pDM_PURCHASE, pDM_SUB_HISTORY ,nRowIdx);


	//Default ���� ���� ��Ų��.
	SetModifyDefaultValue_SPECIES(pDM_SPECIES,nRowIdx);
	SetModifyDefaultValue_PURCHASE(pDM_PURCHASE, nRowIdx);
	SetModifyDefaultValue_IDX(pDM_IDX, nRowIdx);
	SetModifyDefaultValue_SUB_HISTORY(pDM_SUB_HISTORY, nRowIdx);


	//���� ��ũ�� ������ ���������� �����Ѵ�
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);


	// ���� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	RemoveAliasList.Add(_T("SS_��KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES,_T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// ���ο� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("ISS_��KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// ���������� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("SP_���԰���KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("SE_PURCHASE_MANAGE_TBL") , nRowIdx , pDM_SPECIES, _T("REC_KEY") , _T("NUMERIC") , strPurchase_key);
	

	// �����̷������� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("�����̷�KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SUB_HISTORY, _T("SE_SUBSCRIPTION_HISTORY_TBL") , nRowIdx , pDM_SPECIES, _T("REC_KEY") , _T("NUMERIC") , strSubHistory_key);


    // ���̷������� Insert���� �����Ѵ�
	for(INT i=0;i<pDM_SPE_HISTORY->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPE_HISTORY, _T("SE_SPECIES_HISTORY_TBL"), i , pDM_SPECIES);
	}
	

	//IDX_TMP_SE_TBL ����
	SetIdxTmpSeTbl(pDM_SPECIES, strSpecies_key, _T("I"));

	
	// ������ Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�� ��������!!!"));
		return -3000;	
	}

	return 0;
}

INT CSeApi::SPECIES_PURCHASE_UPDATE_ACQ_CHANGE(CESL_DataMgr *pDM_SPECIES, 
											   CESL_DataMgr *pDM_IDX, 
											   CESL_DataMgr *pDM_PURCHASE, 
											   CESL_DataMgr* pDM_SUB_HISTORY, 
											   CESL_DataMgr* pDM_SPE_HISTORY,
											   CMarc* pMarc, INT nRowIdx)
{
	// �� ȭ�鿡�� ���Ա����� ��ȭ�Ȼ�Ȳ  
	// ���� 1 row update
	// ���ο� 1 row update
	// ���������� 1 row insert
	// �����̷������� 1row insert

	//�����ӿ��� ���Ǵ� DM free
	pDM_SUB_HISTORY->FreeData();
	
	// ���� �̷� DM�� 1row insert
	pDM_SUB_HISTORY->InsertRow(-1);

	// ���� �̷¿� REC_KEY����� �ش�
	CString strSUB_HISTORY_KEY;
	pDM_SUB_HISTORY->MakeRecKey(strSUB_HISTORY_KEY);
	pDM_SUB_HISTORY->SetCellData(_T("�����̷�KEY"),strSUB_HISTORY_KEY,nRowIdx);
	
	// ���� �̷�KEY �� ���������� �־��ش� 
	pDM_PURCHASE->SetCellData(_T("SP_�����̷�����KEY"),strSUB_HISTORY_KEY,nRowIdx);

	//���������� �����̷������� ����� 
	MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(pDM_PURCHASE, pDM_SUB_HISTORY ,nRowIdx);

	CArray <CString, CString> RemoveAliasList;
	pDM_SPECIES->StartFrame();

	INT ids;
	CString strSpecies_key, strPurchase_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_��KEY"),nRowIdx); 
	strPurchase_key = pDM_PURCHASE->GetCellData(_T("SP_���԰���KEY"),nRowIdx); 	
	

	//Default ���� ���� ��Ų��.
	SetModifyDefaultValue_SPECIES(pDM_SPECIES,nRowIdx);
	SetInsertDefaultValue_PURCHASE(pDM_PURCHASE, nRowIdx); //������ �Է����� setting�Ѵ�.
	SetModifyDefaultValue_IDX(pDM_IDX, nRowIdx);
	SetInsertDefaultValue_SUB_HISTORY(pDM_SUB_HISTORY, nRowIdx); //�����̷��� �Է����� setting�Ѵ�.


	//���� ��ũ�� ������ ���������� �����Ѵ�
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);


	// ���� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	RemoveAliasList.Add(_T("SS_��KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES,_T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// ���ο� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("ISS_��KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// ���԰��������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("SE_PURCHASE_MANAGE_TBL"), nRowIdx , pDM_SPECIES);

	
	// �����̷������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SUB_HISTORY, _T("SE_SUBSCRIPTION_HISTORY_TBL"), nRowIdx , pDM_SPECIES);
	
	 // ���̷������� Insert���� �����Ѵ�
	for(INT i=0;i<pDM_SPE_HISTORY->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPE_HISTORY, _T("SE_SPECIES_HISTORY_TBL"), i , pDM_SPECIES);
	}

	
	// ������ Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�� ��������!!!"));
		return -3000;	
	}
	
	return 0;

	
}

INT CSeApi::SPECIES_DONATE_UPDATE(CESL_DataMgr *pDM_SPECIES, 
								  CESL_DataMgr *pDM_IDX, 
								  CESL_DataMgr *pDM_DONATE,
								  CESL_DataMgr* pDM_SPE_HISTORY,
								  CMarc* pMarc, 
								  INT nRowIdx)
{
	// ����-��-���� �Ǽ��� 
	// ���� 1 row update
	// ���ο� 1 row update
	// ���������� 1 row update

	CArray <CString, CString> RemoveAliasList;
	pDM_SPECIES->StartFrame();

	INT ids;
	CString strSpecies_key, strDonate_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_��KEY"),nRowIdx); 
	strDonate_key = pDM_DONATE->GetCellData(_T("SD_��������KEY"),0); 	
	
	//Default ���� ���� ��Ų��.
	SetModifyDefaultValue_SPECIES(pDM_SPECIES,nRowIdx);
	SetModifyDefaultValue_DONATE(pDM_DONATE, nRowIdx);
	SetModifyDefaultValue_IDX(pDM_IDX, nRowIdx);

	//���� ��ũ�� ������ ���������� �����Ѵ�
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);

	// ���� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	//RemoveAliasList.Add(_T("MARC"));
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	RemoveAliasList.Add(_T("SS_��KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES,_T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// ���ο� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("ISS_��KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// ���������� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("SD_��������KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_DONATE, _T("SE_DONATE_MANAGE_TBL") , nRowIdx , pDM_SPECIES, _T("REC_KEY") , _T("NUMERIC") , strDonate_key);

    
	// ���̷������� Insert���� �����Ѵ�
	for(INT i=0;i<pDM_SPE_HISTORY->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPE_HISTORY, _T("SE_SPECIES_HISTORY_TBL"), i , pDM_SPECIES);
	}
	
	//IDX_TMP_SE_TBL ����
	SetIdxTmpSeTbl(pDM_SPECIES, strSpecies_key, _T("I"));


	// ������ Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�� ��������!!!"));
		return -4000;	
	}
	
	return 0;

}

INT CSeApi::SPECIES_DONATE_UPDATE_ACQ_CHANGE(CESL_DataMgr *pDM_SPECIES, 
											 CESL_DataMgr *pDM_IDX, 
											 CESL_DataMgr *pDM_DONATE,
											 CESL_DataMgr* pDM_SPE_HISTORY,
											 CMarc* pMarc, 
											 INT nRowIdx)
{
	// ��ȭ�鿡�� ���� ������ ��ȭ�� ���� 
	// ���� 1 row update
	// ���ο� 1 row update
	// ���������� 1 row insert

	CArray <CString, CString> RemoveAliasList;
	pDM_SPECIES->StartFrame();

	INT ids;
	CString strSpecies_key, strDonate_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_��KEY"),nRowIdx); 
	strDonate_key = pDM_DONATE->GetCellData(_T("SD_��������KEY"),0); 	
	
	//Default ���� ���� ��Ų��.
	SetModifyDefaultValue_SPECIES(pDM_SPECIES,nRowIdx);
	SetInsertDefaultValue_DONATE(pDM_DONATE, nRowIdx); //������ insert
	SetModifyDefaultValue_IDX(pDM_IDX, nRowIdx);


	//���� ��ũ�� ������ ���������� �����Ѵ�
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);

	// ���� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	//RemoveAliasList.Add(_T("MARC"));
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	RemoveAliasList.Add(_T("SS_��KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES,_T("REC_KEY"), _T("NUMERIC"), strSpecies_key);

	// ���ο� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("ISS_��KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// �������������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_DONATE, _T("SE_DONATE_MANAGE_TBL"), nRowIdx , pDM_SPECIES);
	
	
	// ���̷������� Insert���� �����Ѵ�
	for(INT i=0;i<pDM_SPE_HISTORY->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPE_HISTORY, _T("SE_SPECIES_HISTORY_TBL"), i , pDM_SPECIES);
	}


	// ������ Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�� ��������!!!"));
		return -4000;	
	}

	return 0;

}


INT CSeApi::SPECIES_DELETE(CString strSPECIES_KEY,
						   CESL_DataMgr* pDM_SPECIES,
						   CESL_DataMgr* pDM_SPECIES_DELETE, 
						   CESL_DataMgr* pDM_VOL,
						   CESL_DataMgr* pDM_VOL_DELETE,
						   CESL_DataMgr* pDM_BOOK,
						   CESL_DataMgr* pDM_DROP_NO,
						   INT nACQ_TYPE)
{
	//�� ���� �Լ� 
	//�ٸ� ���Ա����� Ȯ���� ���� ������ ���� ���� �Ǵ��Ѵ�.
	//�� 1 row delete ������ 1 row insert
	//������ �۾� ���� update
	//�� n row delete ������ n row insert
	//�׿� ���� delete
	
	if(!pDM_SPECIES || !pDM_SPECIES_DELETE || !pDM_VOL || !pDM_VOL_DELETE || strSPECIES_KEY.IsEmpty())
	{
		AfxMessageBox(_T("�� ���� ���� ������ ã�� �� �����ϴ�!"));
		return -1;
	}

	CString strQuery,strACQ_KEY,strWhere;
	INT nDeleteSpeciesOption = 0;

	
	//������ ���� DM�� FREE �Ѵ�
	pDM_SPECIES->FreeData();
	pDM_SPECIES_DELETE->FreeData();
	pDM_VOL->FreeData();
	pDM_VOL_DELETE->FreeData();
	pDM_BOOK->FreeData();
	pDM_DROP_NO->FreeData();
	

  	//�ٸ��� ���� ������ �ľ��Ѵ�. 
	if(1 == nACQ_TYPE)
		strQuery.Format(_T("SELECT REC_KEY FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	else if(2 == nACQ_TYPE) 
		strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	
	CString tmp;
	pDM_SPECIES->GetOneValueQuery(strQuery,tmp);
	//�ٸ� �ٸ� ���� ������ �������� �ʴ� ��� 
	if(tmp.IsEmpty()) nDeleteSpeciesOption = 1;

	
	//������ �ʿ��� �⺻���� ������ �´�
	CString strToday;
	CString strWorkLog;
	strToday = GetTodayDate();
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);


	pDM_SPECIES->StartFrame();

	const INT cnt = 5;
	CString strData[cnt]; //ACQ_KEY, �Լ� ���� TYPE, �Լ���������KEY,  ���� ���� TYPE, �����Ͽ�������KEY

  	if(1 == nACQ_TYPE)
		strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_TYPE,ACQ_EXP_KEY,PUB_EXP_TYPE,PUB_EXP_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	else if(2 == nACQ_TYPE) 
		strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_TYPE,ACQ_EXP_KEY,PUB_EXP_TYPE,PUB_EXP_KEY FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
		
	pDM_SPECIES->GetValuesQuery(strQuery,strData,cnt);
	
	
	//�������� KEY;
	CString strHoldingKey;
	strQuery.Format(_T("SELECT HOLDING_KEY FROM SE_SPECIES_TBL WHERE REC_KEY = %s"),strSPECIES_KEY);
	pDM_SPECIES->GetOneValueQuery(strQuery,strHoldingKey);


	//���� �Ǵ� �������� KEY
	strACQ_KEY = strData[0];


	//�Լ� ���� ����
	INT ntype = -1;
	CString strTBL_NAME;
	if(!strData[1].IsEmpty() && !strData[2].IsEmpty())
	{
		strTBL_NAME = GetExpTable(strData[1]);
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '0';"),strTBL_NAME,strData[2]);
		pDM_SPECIES->AddFrame(strQuery);
	}
	
	//������ ���� ���� 
	ntype = -1;
	if(!strData[3].IsEmpty() && !strData[4].IsEmpty()) 
	{
		strTBL_NAME = GetExpTable(strData[3]);
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '1';"),strTBL_NAME,strData[4]);
		pDM_SPECIES->AddFrame(strQuery);
	}
	
	//�η� ���� ���� 
	if(!strACQ_KEY.IsEmpty()) {
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_APPENDIX_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
	}

	//��ȣ ���� ���� 
	if(!strACQ_KEY.IsEmpty()) {
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_VOL_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
	}
	
	//���� �̷� ���� 
	if(1 == nACQ_TYPE) 
	{
		if(!strACQ_KEY.IsEmpty()) {
			strQuery.Format(_T("DELETE FROM SE_SUBSCRIPTION_HISTORY_TBL WHERE PURCHASE_MANAGE_KEY = %s;"),strACQ_KEY); 
			pDM_SPECIES->AddFrame(strQuery);
		}
	}
	
	
	//Ŭ���� �̷� ������ �����Ѵ�. 
	if(!strACQ_KEY.IsEmpty()) {
		
		strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY IN (SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE ACQ_KEY = %s);"),strACQ_KEY);
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->AddFrame(strQuery);
	}
	
	
	//å���� ���� ���� -- ��Ϲ�ȣ�� �ִ� å�� ���� ��Ϲ�ȣ TBL�� INSERT�Ѵ� (����å ����)
	
	/*
	strWhere.Format(_T("B.BOOK_TYPE != 'B' AND B.ACQ_CODE = '%d' AND B.ACCESSION_REC_KEY = C.REC_KEY (+)"),nACQ_TYPE);
	pDM_BOOK->RefreshDataManager(strWhere);
	*/

	//å �����ɷϹ�ȣ ó�� 
	//MAKE_DROP_ACCESSION_NO_by_BOOK(pDM_SPECIES,pDM_BOOK,pDM_DROP_NO);
	

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//���� ��� ��ȣ ó�� 2004-03-04����
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	pDM_SPECIES->InitDBFieldData();
// 	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
// 					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,2),SUBSTR(REG_NO,3,10),'%s','%s', UDF_MANAGE_CODE")
// 					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
// 					_T("AND REC_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE BOOK_TYPE != 'B' AND ACQ_CODE = '%d' AND SPECIES_KEY = %s));")
// 					,DEFAULT_SEQ_NAME,strWorkLog,strWorkLog,nACQ_TYPE,strSPECIES_KEY);
// 	pDM_SPECIES->AddFrame(strQuery);

	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,%d),SUBSTR(REG_NO,%d,%d),'%s','%s', UDF_MANAGE_CODE")
					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
					_T("AND REC_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE BOOK_TYPE != 'B' AND ACQ_CODE = '%d' AND SPECIES_KEY = %s));")
					,DEFAULT_SEQ_NAME,nRegCodeLength,nRegCodeLength+1, 12-nRegCodeLength,strWorkLog,strWorkLog,nACQ_TYPE,strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);
	//=====================================================
	
	


	//å���� ����
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE BOOK_TYPE != 'B' AND ACQ_CODE = '%d' AND SPECIES_KEY = %s;"),nACQ_TYPE,strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);
	


	//���Ա��к� ��ȣ����
	if(!strACQ_KEY.IsEmpty()) {

		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->AddFrame(strQuery);
	
	}
	
	//���� - ���� ����
	if(1 == nACQ_TYPE) 
		strQuery.Format(_T("DELETE FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
	else if(2 == nACQ_TYPE) 
		strQuery.Format(_T("DELETE FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
	
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddFrame(strQuery);

	
	//�������� �����´�.
	strWhere.Format(_T("V.VOL_TYPE = 'V' AND V.SPECIES_KEY = %s"),strSPECIES_KEY); //���� ��ȣ�� �ƴ� ��ȣ
	pDM_VOL->RefreshDataManager(strWhere);
	

	//���� ������ �������� ���� ���θ� �Ǵ� �� ������ ���� ���������� MOVE
	//�� ������ �����Ѱ����� �Ǵ� 	
	//���� ���̺�(���,������,����,�ʷ�,����������)
	CString strUDF_KEY,strOtherAcqCount,strVOL_KEY;
	INT nOtherAcqCount = 0;
	INT nRowDeleteVol = 0;
	CArray<CString,CString> RemoveAliasList;

	for(INT i=0;i<pDM_VOL->GetRowCount();i++)
	{
		
		strUDF_KEY =_T("");
		strOtherAcqCount = _T("");

		strOtherAcqCount = pDM_VOL->GetCellData(_T("UDF_��Ÿ���Ա���å��"),i);
		nOtherAcqCount = _ttoi(strOtherAcqCount.GetBuffer(0));
	
		if(1 == nACQ_TYPE) { 
			strUDF_KEY = pDM_VOL->GetCellData(_T("UDF_�������Ա��к���KEY"),i);
		} else if(2 == nACQ_TYPE) {
			strUDF_KEY = pDM_VOL->GetCellData(_T("UDF_���Լ��Ա��к���KEY"),i);
		}
		
		if(strUDF_KEY.IsEmpty() && 0 == nOtherAcqCount)
		{
			//�ǻ���
			strVOL_KEY = pDM_VOL->GetCellData(_T("SV_��KEY"),i);
						
			pDM_VOL_DELETE->InsertRow(-1);	
			nRowDeleteVol = pDM_VOL_DELETE->GetRowCount()-1;
			CopyDMToDMByAlias(pDM_VOL,i,pDM_VOL_DELETE,nRowDeleteVol);
			pDM_VOL_DELETE->SetCellData(_T("SV_��������"),strToday,nRowDeleteVol);
			pDM_VOL_DELETE->SetCellData(_T("SV_�����۾�"),strWorkLog,nRowDeleteVol);
			pDM_VOL_DELETE->SetCellData(_T("SV_�������۾�"),strWorkLog,nRowDeleteVol);

			RemoveAliasList.RemoveAll();
			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_VOL_DELETE, _T("SE_DELETE_VOL_TBL"), nRowDeleteVol , pDM_SPECIES);

			strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->AddFrame(strQuery);

			//���� ��ȣ�� setting
			MakeEcoIndexVol(pDM_SPECIES,strVOL_KEY,FALSE);

			//����,����,�ʷϰ��� ����
			GetArticleDeleteQuery(pDM_SPECIES,strVOL_KEY);
		}


		if(0 != nOtherAcqCount && 1 == nDeleteSpeciesOption) //�ٸ� ���Ա����� �����ϴ� ���
		{
			nDeleteSpeciesOption = 0;
		}

	}

		
	if(1 == nDeleteSpeciesOption) //���� ���� �ؾ� �ϴ� ��Ȳ
	{
		//���� ���������� MOVE 
		strWhere.Format(_T("REC_KEY = %s"),strSPECIES_KEY);
		pDM_SPECIES->RefreshDataManager(strWhere);
		pDM_SPECIES_DELETE->InsertRow(-1);
		CopyDMToDMByAlias(pDM_SPECIES,0,pDM_SPECIES_DELETE,0);
		pDM_SPECIES_DELETE->SetCellData(_T("SS_��������"),strToday,0);
		pDM_SPECIES_DELETE->SetCellData(_T("SS_�����۾�"),strWorkLog,0);
		pDM_SPECIES_DELETE->SetCellData(_T("SS_�������۾�"),strWorkLog,0);
		pDM_SPECIES_DELETE->SetCellData(_T("SS_�۾����±���"),_T("3"),0); //�۾� ���� ������ ������ ǥ��

		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES_DELETE, _T("SE_DELETE_SPECIES_TBL"), 0 , pDM_SPECIES);
		
		strQuery.Format(_T("DELETE FROM SE_SPECIES_TBL WHERE REC_KEY = %s;"),strSPECIES_KEY);
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->AddFrame(strQuery);

		
		if(!strHoldingKey.IsEmpty())
		{
			pDM_SPECIES->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_HOLDING_TBL WHERE REC_KEY = %s;"),strHoldingKey);
			pDM_SPECIES->AddFrame(strQuery);
		}

		//���� update
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("UPDATE IDX_SE_TBL SET SPECIES_CLASS = '3' WHERE REC_KEY = %s;"),strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);
		

		//IDX_TMP_SE_TBL ����
		SetIdxTmpSeTbl(pDM_SPECIES, strSPECIES_KEY, _T("D"));
		

		//��������(���� �÷��� å, ����å, ������ȣ,��������) delete 
		//����å 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_BINDING_BOOK_TBL WHERE BOOK_KEY IN ")
						_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s AND BOOK_TYPE = 'B');"),strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);
		
		
		//���� �÷��� å
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_BOOK_TBL WHERE SPECIES_KEY = %s AND BOOK_TYPE = 'B';"),strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);
		

		//������ȣ (���� �� ������ �̵�)
		pDM_VOL->FreeData();
		pDM_VOL_DELETE->FreeData();


		//���� ��ȣ ������ �����´�.
		strWhere.Format(_T("V.VOL_TYPE = 'B' AND V.SPECIES_KEY = %s"),strSPECIES_KEY); 
		pDM_VOL->RefreshDataManager(strWhere);
		
		for(INT i=0;i<pDM_VOL->GetRowCount();i++)
		{
			//������ ����(�̵�)
			strVOL_KEY = pDM_VOL->GetCellData(_T("SV_��KEY"),i);
				
			pDM_VOL_DELETE->InsertRow(-1);	
			nRowDeleteVol = pDM_VOL_DELETE->GetRowCount()-1;
			CopyDMToDMByAlias(pDM_VOL,i,pDM_VOL_DELETE,nRowDeleteVol);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
			pDM_VOL_DELETE->SetCellData(_T("SV_��������"),strToday,nRowDeleteVol);
			pDM_VOL_DELETE->SetCellData(_T("SV_�����۾�"),strWorkLog,nRowDeleteVol);
			pDM_VOL_DELETE->SetCellData(_T("SV_�������۾�"),strWorkLog,nRowDeleteVol);

			RemoveAliasList.RemoveAll();
			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_VOL_DELETE, _T("SE_DELETE_VOL_TBL"), nRowDeleteVol , pDM_SPECIES);

			pDM_SPECIES->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
			pDM_SPECIES->AddFrame(strQuery);
	
		}

		//���� ����
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_BINDING_INFO_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);

		
		//��������
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_PREV_SPECIES_TBL WHERE (REC_KEY = %s OR SPECIES_KEY = %s);"),strSPECIES_KEY,strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);


		//�ļ�����
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_NEXT_SPECIES_TBL WHERE (REC_KEY = %s OR SPECIES_KEY = %s);"),strSPECIES_KEY,strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);


		//�� �̷�����
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_SPECIES_HISTORY_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);

	}

	INT ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if(ids<0) return -1000;

	return 0;
}



INT CSeApi::SPECIES_CAT_DELETE(CString strSPECIES_KEY,
							   CESL_DataMgr* pDM_SPECIES,
						       CESL_DataMgr* pDM_SPECIES_DELETE, 
						       CESL_DataMgr* pDM_VOL)
{
	//�������� �� ���� �Լ� --> �������� ����
	//��� ���Ա����� �ڷḦ �����Ѵ�
	//�� 1 row delete ������ 1 row insert
	//������ �۾� ���� update
	//�� n row delete ������ n row insert
	//�׿� ���� delete
	
	if(!pDM_SPECIES || !pDM_SPECIES_DELETE || !pDM_VOL || strSPECIES_KEY.IsEmpty())
	{
		AfxMessageBox(_T("�� ���� ���� ������ ã�� �� �����ϴ�!"));
		return -1;
	}

	CString strQuery,strACQ_KEY,strWhere,strTBL_NAME;


	/////////////////////////////////////////////////
	//�� ������ ������ �� �ѹ��� ���� �ǻ縦 ���´�//
	/////////////////////////////////////////////////
	CSeSpeciesDelete dlg(m_pMgr);
	dlg.m_strSpeciesKey = strSPECIES_KEY;
	if(dlg.DoModal() != IDOK) return -1001; //procedure cancel


	///////////////////////////////////////////////////////////
	//�ش� ������ å ����, ��������, �Ϲ����� ���¸� Ȯ���Ѵ�//
	///////////////////////////////////////////////////////////

	//����
	strQuery.Format(_T("SELECT COUNT(*) FROM LH_STORAGE_V01 WHERE STATUS IN ('0','2','3','5') ")
					_T("AND BOOK_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s)")
					,strSPECIES_KEY);

	INT nCount = 0;
	CString strCount;

	pDM_SPECIES->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			AfxMessageBox(_T("�ش� �� ���� ���� Ȥ�� �������� å�� �ֽ��ϴ�!\nȮ���Ͻð� �ٽ� ������ �ֽʽÿ�!"));
			return -1002; //verifying fail
		}

	}

	//��������
	strQuery.Format(_T("SELECT COUNT(*) FROM CO_REPAIR_BINDING_TBL WHERE STATUS IN ('1','2') ")
					_T("AND BOOK_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s)")
					,strSPECIES_KEY);

	pDM_SPECIES->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			AfxMessageBox(_T("�ش� �� ���� ���������� ��� �Ǵ� �Ƿ��� å�� �ֽ��ϴ�!\nȮ���Ͻð� �ٽ� ������ �ֽʽÿ�!"));
			return -1002; //verifying fail
		}

	}

	//�Ϲ� ����
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE WORKING_STATUS IN ('SEB112N','SEB113N') AND SPECIES_KEY = %s")
					,strSPECIES_KEY);

	pDM_SPECIES->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			AfxMessageBox(_T("�ش� �� ���� ���� ������ ���� Ȥ�� ���� ������ å�� �ֽ��ϴ�!\nȮ���Ͻð� �ٽ� ������ �ֽʽÿ�!"));
			return -1002; //verifying fail
		}

	}

	
	//������ ���� DM�� FREE �Ѵ�
	pDM_SPECIES->FreeData();
	pDM_SPECIES_DELETE->FreeData();
	pDM_VOL->FreeData();
	
	
	//������ �ʿ��� �⺻ ���� �����´� 
	CString strToday;
	CString strWorkLog;
	strToday = GetTodayDate();
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);


	////////////////////////////////////////////////////////////////
	//����, ���� ���� ���̺��� ������ �ʿ��� KEY���� ������ �´�//
	////////////////////////////////////////////////////////////////

	const INT cnt = 5;
	CString strPurData[cnt]; //ACQ_KEY, �Լ� ���� TYPE, �Լ���������KEY,  ���� ���� TYPE, �����Ͽ�������KEY
	CString strDonData[cnt]; 
	//���԰��� ������ 
	strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_TYPE,ACQ_EXP_KEY,PUB_EXP_TYPE,PUB_EXP_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	pDM_SPECIES->GetValuesQuery(strQuery,strPurData,cnt);
	//�������� ������ 
	strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_TYPE,ACQ_EXP_KEY,PUB_EXP_TYPE,PUB_EXP_KEY FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	pDM_SPECIES->GetValuesQuery(strQuery,strDonData,cnt);
	
	
	pDM_SPECIES->StartFrame();


	//////////////////////////////////////////////////////////////////////////////////
	//���� ������ �����  ���̺� ���� (����,�����̷�,Ŭ����,���Ա��к� ��,���԰���) //
	//////////////////////////////////////////////////////////////////////////////////

	if(!strPurData[0].IsEmpty())
	{
		
		strACQ_KEY = strPurData[0];
		
		//�Լ��� ���� ����
		if(!strPurData[1].IsEmpty() && !strPurData[2].IsEmpty()) 
		{
			strTBL_NAME = GetExpTable(strPurData[1]);
			pDM_SPECIES->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '0';"),strTBL_NAME,strPurData[2]);
			pDM_SPECIES->AddFrame(strQuery);
		}
	
		//������ ���� ���� 
		if(!strPurData[3].IsEmpty() && !strPurData[4].IsEmpty()) 
		{
			strTBL_NAME = GetExpTable(strPurData[3]);
			pDM_SPECIES->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '1';"),strTBL_NAME,strPurData[4]);
			pDM_SPECIES->AddFrame(strQuery);
		}


		//�η� ���� ���� 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_APPENDIX_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
		

		//��ȣ ���� ���� 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_VOL_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
		
		
		//���� �̷� ���� 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_SUBSCRIPTION_HISTORY_TBL WHERE PURCHASE_MANAGE_KEY = %s;"),strACQ_KEY); 
		pDM_SPECIES->AddFrame(strQuery);
	
		
		//Ŭ���� �̷� ���� 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY IN (SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE ACQ_KEY = %s);"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
		

		//���Ա��к� �� ���� 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
	
		//���԰��� ���� 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_PURCHASE_MANAGE_TBL WHERE REC_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);	

	}


	///////////////////////////////////////////////////////////////
	//���� ������ �����  ���̺� ���� (����,Ŭ����,���Ա��к� ��)//
	///////////////////////////////////////////////////////////////

	if(!strDonData[0].IsEmpty())
	{
		
		strACQ_KEY = strDonData[0];
		
		//�Լ��� ���� ����
		if(!strDonData[1].IsEmpty() && !strDonData[2].IsEmpty()) 
		{
			strTBL_NAME = GetExpTable(strDonData[1]);
			pDM_SPECIES->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '0';"),strTBL_NAME,strDonData[2]);
			pDM_SPECIES->AddFrame(strQuery);
		}
	
		//������ ���� ���� 
		if(!strDonData[3].IsEmpty() && !strDonData[4].IsEmpty()) 
		{
			strTBL_NAME = GetExpTable(strDonData[3]);
			pDM_SPECIES->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '1';"),strTBL_NAME,strDonData[4]);
			pDM_SPECIES->AddFrame(strQuery);
		}


		//�η� ���� ���� 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_APPENDIX_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);


		//��ȣ ���� ���� 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_VOL_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);

		
		//Ŭ���� �̷� ����
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY IN (SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE ACQ_KEY = %s);"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);


		//���Ա��к� �� ���� 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
	

		//�������� ���� 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_DONATE_MANAGE_TBL WHERE REC_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);		

	}


	
	//////////////////////////////////////////////////////////////
	//å�������� ����(å,�������,������Ϲ�ȣ ó��,�η�,����å)//
	//////////////////////////////////////////////////////////////
	

	//���� ���� ����
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_LOAN_MAIL_SEND_TBL WHERE LOAN_KEY IN ")
					_T("(SELECT REC_KEY FROM LH_STORAGE_V01 WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s));"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);

   
	//���� ���� 
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE LS_WORK_T01 WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s);"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);
	

	//��� �ڷ� ���� 
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_DELIVERY_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s);"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);

		
	//���� ���� ���� 
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_REPAIR_BINDING_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s);"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);


	//�η����� 
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_APPENDIX_BOOK_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s);"),strSPECIES_KEY);	
	pDM_SPECIES->AddFrame(strQuery);
	

	//����å 
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_BINDING_BOOK_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s);"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);

	

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//���� ��� ��ȣ ó�� 
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
//	pDM_SPECIES->InitDBFieldData();
// 	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
// 					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,2),SUBSTR(REG_NO,3,10),'%s','%s',UDF_MANAGE_CODE ")
// 					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
// 					_T("AND REC_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s));")
// 					,DEFAULT_SEQ_NAME,strWorkLog,strWorkLog,strSPECIES_KEY);
// 	pDM_SPECIES->AddFrame(strQuery);

		pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,%d),SUBSTR(REG_NO,%d,%d),'%s','%s',UDF_MANAGE_CODE ")
					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
					_T("AND REC_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s));")
					,DEFAULT_SEQ_NAME,nRegCodeLength,nRegCodeLength+1, 12-nRegCodeLength,strWorkLog,strWorkLog,strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);
	//===================================================== 

	//å���� ����
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_BOOK_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);	
	pDM_SPECIES->AddFrame(strQuery);



	///////////////////////////////////////////////////////////////////
	//��ȣ ���� ����(��ȣ,�������Է�,���,������,�����ʷ�,��ȣ����)//
	///////////////////////////////////////////////////////////////////
	
	//��ȣ ������ �����´�.
	strWhere.Format(_T("V.SPECIES_KEY = %s"),strSPECIES_KEY); 
	pDM_VOL->RefreshDataManager(strWhere);
	
	CString strVOL_KEY;
	INT nRowDeleteVol = 0;

	CArray<CString,CString> RemoveAliasList;
	
	//������ȣ�� �Է�
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO SE_DELETE_VOL_TBL ")
					_T("(SELECT REC_KEY,SPECIES_KEY,VOL_TYPE,TITLE,FORM_CODE,MEDIA_CODE,VOL_SORT_NO,")
					_T("VOL_TITLE,OTHER_VOL_TITLE,ALTERNATIVE_VOL_TITLE,IDX_VOL_TITLE,PUBLISH_DATE,")
					_T("CURRENCY_CODE,PRICE,PRICE_CHARACTER,PAGE,PHYSICAL_PROPERTY,BOOK_SIZE,ACCOMP_MAT,")
					_T("REMARK,VOLUME_CLASS,CONTENTS_YN,CONTENTS_KEY,'%s',FIRST_WORK,'%s', UDF_MANAGE_CODE ")
					_T("FROM SE_VOL_TBL WHERE SPECIES_KEY = %s);"),strToday,strWorkLog,strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);


	for(INT i=0;i<pDM_VOL->GetRowCount();i++)
	{
		
		strVOL_KEY = pDM_VOL->GetCellData(_T("SV_��KEY"),i);
		
		if(strVOL_KEY.IsEmpty()) continue;

		//���� ��ȣ�� setting
		MakeEcoIndexVol(pDM_SPECIES,strVOL_KEY,FALSE);

	}


	//���,����,�ʷϰ��� ����
	GetArticleDeleteQuerybySpecies(pDM_SPECIES,strSPECIES_KEY);
	

	//��ȣ ����
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);

	
	///////////////////////////////////////////////////////////////////////////////////////
	//�� ���� ����(��������, ��, ��������, �ļ�����,�� �̷�����, ��������, ���� ������Ʈ)//
	///////////////////////////////////////////////////////////////////////////////////////

	
	//��������
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_HOLDING_TBL WHERE REC_KEY IN (SELECT HOLDING_KEY FROM SE_SPECIES_TBL ")
					_T("WHERE REC_KEY = %s);"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);



	//���� ����
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_BINDING_INFO_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);


	//��������
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_PREV_SPECIES_TBL WHERE (REC_KEY = %s OR SPECIES_KEY = %s);"),strSPECIES_KEY,strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);


	//�ļ�����
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_NEXT_SPECIES_TBL WHERE (REC_KEY = %s OR SPECIES_KEY = %s);"),strSPECIES_KEY,strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);


	//�� �̷�����
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_SPECIES_HISTORY_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);


	//���� ���������� MOVE 
	strWhere.Format(_T("REC_KEY = %s"),strSPECIES_KEY);
	pDM_SPECIES->RefreshDataManager(strWhere);
	pDM_SPECIES_DELETE->InsertRow(-1);
	CopyDMToDMByAlias(pDM_SPECIES,0,pDM_SPECIES_DELETE,0);
	pDM_SPECIES_DELETE->SetCellData(_T("SS_��������"),strToday,0);
	pDM_SPECIES_DELETE->SetCellData(_T("SS_�����۾�"),strWorkLog,0);
	pDM_SPECIES_DELETE->SetCellData(_T("SS_�������۾�"),strWorkLog,0);
	pDM_SPECIES_DELETE->SetCellData(_T("SS_�۾����±���"),_T("3"),0); //�۾� ���� ������ ������ ǥ��

	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES_DELETE, _T("SE_DELETE_SPECIES_TBL"), 0 , pDM_SPECIES);
	

	//������
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_SPECIES_TBL WHERE REC_KEY = %s;"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);

		
	//���� update
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("UPDATE IDX_SE_TBL SET SPECIES_CLASS = '3' WHERE REC_KEY = %s;"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);
		

	//IDX_TMP_SE_TBL ����
	SetIdxTmpSeTbl(pDM_SPECIES, strSPECIES_KEY, _T("D"));
		

	INT ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if(ids<0) return -1003; //db oepration error

	return 0;
}




INT CSeApi::GetArticleDeleteQuery(CESL_DataMgr* pDM_OUT, CString strVOL_KEY)
{
	//��ȣŰ�� ���� ����������� �����ϴ� ������ ����� �Լ� 
	CString strQuery;

	//2003/03/22 �߰�
	//IDX_TMP_SE_TOC_TBL�� ������ SE_ARTICLE_TBL�� INSERT�Ѵ�
	strQuery.Format(_T("INSERT INTO IDX_TMP_SE_TOC_TBL ")
					_T("(SELECT %s.NEXTVAL,REC_KEY,'D','%s' FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s);")
					,DEFAULT_SEQ_NAME,GetTodayDate(),strVOL_KEY);

	pDM_OUT->AddFrame(strQuery);


	//SE_ABSTRACTS_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ABSTRACTS_TBL WHERE ARTICLE_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_OUT->AddFrame(strQuery);


	//SE_ARTICLE_TOC_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ARTICLE_TOC_TBL WHERE ARTICLE_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_OUT->AddFrame(strQuery);
	

	//SE_ARTICLE_TBL 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);
	pDM_OUT->AddFrame(strQuery);


	//IDX_SE_TOC_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM IDX_SE_TOC_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);
	pDM_OUT->AddFrame(strQuery);


	//SE_CONTENTS_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_CONTENTS_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);
	pDM_OUT->AddFrame(strQuery);

	return 0;
}

INT CSeApi::GetArticleDeleteQuerybySpecies(CESL_DataMgr* pDM_OUT, CString strSPECIES_KEY)
{
	
	//��Ű�� ���� ����������� �����ϴ� ������ ����� �Լ� (������)
	CString strQuery;


	//2003/03/22 �߰�
	//IDX_TMP_SE_TOC_TBL�� ������ SE_ARTICLE_TBL�� INSERT�Ѵ�
	strQuery.Format(_T("INSERT INTO IDX_TMP_SE_TOC_TBL ")
					_T("(SELECT %s.NEXTVAL,REC_KEY,'D','%s' FROM SE_ARTICLE_TBL WHERE VOL_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_VOL_TBL WHERE SPECIES_KEY = %s));")
					,DEFAULT_SEQ_NAME,GetTodayDate(),strSPECIES_KEY);

	pDM_OUT->AddFrame(strQuery);
	


	//SE_ABSTRACTS_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ABSTRACTS_TBL WHERE ARTICLE_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_ARTICLE_TBL WHERE VOL_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE SPECIES_KEY= %s));"),strSPECIES_KEY);
	pDM_OUT->AddFrame(strQuery);



	//SE_ARTICLE_TOC_TBL		
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ARTICLE_TOC_TBL WHERE ARTICLE_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_ARTICLE_TBL WHERE VOL_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE SPECIES_KEY= %s));"),strSPECIES_KEY);
	pDM_OUT->AddFrame(strQuery);

	

	//SE_ARTICLE_TBL 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ARTICLE_TBL WHERE VOL_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE SPECIES_KEY= %s);"),strSPECIES_KEY);
	pDM_OUT->AddFrame(strQuery);



	//IDX_SE_TOC_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM IDX_SE_TOC_TBL WHERE VOL_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE SPECIES_KEY= %s);"),strSPECIES_KEY);
	pDM_OUT->AddFrame(strQuery);



	//SE_CONTENTS_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_CONTENTS_TBL WHERE VOL_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE SPECIES_KEY= %s);"),strSPECIES_KEY);
	pDM_OUT->AddFrame(strQuery);


	return 0;
}

INT CSeApi::SPECIES_PUR_OBJ_INSERT(CESL_DataMgr* pDM, 
								   CMarc* pMarc, 
								   CESL_DataMgr* pDM_SEQ_NO, 
								   CESL_DataMgr* pDM_WORK_NO, 
								   BOOL IsNewWorkNo, 
								   INT nRowIdx,
								   BOOL bIsDepartMngUse)
{
	
	// ����-���Դ���ڷ� �Է� 
	if(!pDM || !pMarc || !pDM_SEQ_NO || !pDM_WORK_NO)
	{
		AfxMessageBox(_T("���Դ���ڷ� �Է� ���� ������ ã�� �� �����ϴ�!"));
		return -1;
	}


	if(0 == pDM_SEQ_NO->GetRowCount())
	{
		AfxMessageBox(_T("���� ������ ã�� �� �����ϴ�!"));
		return -2;
	}

	//������ ���� free
	pDM_WORK_NO->FreeData();


	CString strREC_KEY;
	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	pDM->MakeRecKey(strREC_KEY);

	pDM->SetCellData(_T("REC_KEY"),strREC_KEY,nRowIdx);
	pDM->SetCellData(_T("�Է�����"),GetTodayDate(),nRowIdx);
	pDM->SetCellData(_T("�����۾�"),strWorkLog,nRowIdx);		
	pDM->SetCellData(_T("�������۾�"),strWorkLog,nRowIdx);	


	//�Է� ���� ó�� 
	CString strINPUT_TYPE;
	strINPUT_TYPE = pDM->GetCellData(_T("�Է±���"),nRowIdx);	
	if(strINPUT_TYPE.IsEmpty())
	{
		pDM->SetCellData(_T("�Է±���"),_T("0"),nRowIdx);	 //ȭ�� �Է�
	}
	
	CString strSEQ_NO_KEY = _T("");
	strSEQ_NO_KEY = pDM_SEQ_NO->GetCellData(_T("REC_KEY"),0);

	//������ȣ, �۾���ȣ ó��
	BOOL bIsInsertWorkNo = FALSE;
	CString strLastWorkNo = _T("");

	if(IsNewWorkNo) //�� �۾� ��ȣ 
	{
		strLastWorkNo = GetLastWorkNo(pDM_SEQ_NO,strSEQ_NO_KEY);

		pDM->SetCellData(_T("�������۾���ȣ"),strLastWorkNo,nRowIdx);
		pDM->SetCellData(_T("�����Ϸù�ȣ"),_T("1"),nRowIdx);

		pDM_WORK_NO->InsertRow(-1);
		CString strWorkKey;
		pDM_WORK_NO->MakeRecKey(strWorkKey);
		
		pDM_WORK_NO->SetCellData(_T("REC_KEY"),strWorkKey,0);
		pDM_WORK_NO->SetCellData(_T("��������KEY"),strSEQ_NO_KEY,0);
		pDM_WORK_NO->SetCellData(_T("�۾���ȣ"),strLastWorkNo,0);
		pDM_WORK_NO->SetCellData(_T("�������Ϸù�ȣ"),_T("1"),0);		
		pDM_WORK_NO->SetCellData(_T("������"),GetTodayDate(),0);
		pDM_WORK_NO->SetCellData(_T("������"),m_pMgr->m_pInfo->USER_ID,0);
		pDM_WORK_NO->SetCellData(_T("�����۾�"),strWorkLog,0);		
		pDM_WORK_NO->SetCellData(_T("�������۾�"),strWorkLog,0);	

		bIsInsertWorkNo = TRUE; //insert

	}
	else //���� �۾���ȣ 
	{
		CString strWorkNo = _T("");
		strWorkNo = pDM->GetCellData(_T("�������۾���ȣ"),nRowIdx);
		
		CString strSerialWorkNo = _T("");
		strSerialWorkNo = GetSerialWorkNo(pDM_WORK_NO,strSEQ_NO_KEY,strWorkNo);
		pDM->SetCellData(_T("�����Ϸù�ȣ"),strSerialWorkNo,nRowIdx);
	}	


	CArray<CString, CString> RemoveAliasList;
	pDM->StartFrame();
	
	// ���� ���� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("SE_PURCHASE_SPECIES_TBL"), nRowIdx , pDM);
	

	//�� �۾� ��ȣ 
	if(bIsInsertWorkNo) 
	{
		RemoveAliasList.RemoveAll();
		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_WORK_NO, _T("SE_PURCHASE_WORK_NO_TBL"), 0 , pDM);
	}


	// ���� ������ Send Frame
	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		return -3000;	
	}

	//�μ� �����ڷ� ���� ��ũ���� ����� �����ڸ� �̾� ������ �ʵ忡 �־��ش�
	if(bIsDepartMngUse)
	{
		CString strTitle;
		CString strPublisher;
		CString strQuery;

		m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("245$a"),strTitle);
		m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("260$b"),strPublisher);
		strQuery.Format(_T("UPDATE SE_PURCHASE_SPECIES_TBL SET TITLE = '%s', PUBLISHER = '%s' ")
			            _T("WHERE REC_KEY = %s;"),strTitle,strPublisher,strREC_KEY);
		
		ids = pDM->ExecuteQuery(strQuery,1);

		if (ids < 0) {
			return -3001;	
		}

	}

	return 0;
}

INT CSeApi::SPECIES_PUR_OBJ_UPDATE(CESL_DataMgr* pDM, 
								   CMarc* pMarc, 
								   CESL_DataMgr* pDM_SEQ_NO, 
								   CESL_DataMgr* pDM_WORK_NO, 
								   BOOL IsNewWorkNo, 
								   BOOL IsWorkNoChanged,
								   INT nRowIdx,
								   BOOL bIsDepartMngUse)
{
	// ����-���Դ���ڷ� ���� 
	if(!pDM || !pMarc || !pDM_SEQ_NO || !pDM_WORK_NO)
	{
		AfxMessageBox(_T("���Դ���ڷ� ���� ���� ������ ã�� �� �����ϴ�!"));
		return -1;
	}


	if(0 == pDM_SEQ_NO->GetRowCount())
	{
		AfxMessageBox(_T("���� ������ ã�� �� �����ϴ�!"));
		return -2;
	}
	
	
	CString strREC_KEY;
	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	strREC_KEY = pDM->GetCellData(_T("REC_KEY"),nRowIdx);

	pDM->SetCellData(_T("�������۾�"),strWorkLog,nRowIdx);	

	CArray<CString, CString> RemoveAliasList;
	pDM->StartFrame();
	

	CString strSEQ_NO_KEY = _T("");
	strSEQ_NO_KEY = pDM_SEQ_NO->GetCellData(_T("REC_KEY"),0);


	//������ȣ, �۾���ȣ ó��
	BOOL bIsInsertWorkNo = FALSE;
	CString strLastWorkNo = _T("");

	if(IsNewWorkNo) //�� �۾� ��ȣ 
	{
		strLastWorkNo = GetLastWorkNo(pDM_SEQ_NO,strSEQ_NO_KEY);

		pDM->SetCellData(_T("�������۾���ȣ"),strLastWorkNo,nRowIdx);
		pDM->SetCellData(_T("�����Ϸù�ȣ"),_T("1"),nRowIdx);

		pDM_WORK_NO->InsertRow(-1);
		CString strWorkKey;
		pDM_WORK_NO->MakeRecKey(strWorkKey);
		
		pDM_WORK_NO->SetCellData(_T("REC_KEY"),strWorkKey,0);
		pDM_WORK_NO->SetCellData(_T("��������KEY"),strSEQ_NO_KEY,0);
		pDM_WORK_NO->SetCellData(_T("�۾���ȣ"),strLastWorkNo,0);
		pDM_WORK_NO->SetCellData(_T("�������Ϸù�ȣ"),_T("1"),0);		
		pDM_WORK_NO->SetCellData(_T("������"),GetTodayDate(),0);
		pDM_WORK_NO->SetCellData(_T("������"),m_pMgr->m_pInfo->USER_ID,0);
		pDM_WORK_NO->SetCellData(_T("�����۾�"),strWorkLog,0);		
		pDM_WORK_NO->SetCellData(_T("�������۾�"),strWorkLog,0);	

		bIsInsertWorkNo = TRUE; //insert

	}
	else //���� �۾���ȣ (�۾���ȣ�� ���� �Ǿ��� ���� �Ǵ��Ѵ� )
	{
		
		if(IsWorkNoChanged)
		{

			CString strWorkNo = _T("");
			strWorkNo = pDM->GetCellData(_T("�������۾���ȣ"),nRowIdx);
			
			CString strSerialWorkNo = _T("");
			strSerialWorkNo = GetSerialWorkNo(pDM_WORK_NO,strSEQ_NO_KEY,strWorkNo);
			pDM->SetCellData(_T("�����Ϸù�ȣ"),strSerialWorkNo,nRowIdx);

		}

	}	


	// ���� ���� update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("REC_KEY"));
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("SE_PURCHASE_SPECIES_TBL"), nRowIdx , pDM, _T("REC_KEY") , _T("NUMERIC") , strREC_KEY);
	
	
	//�� �۾� ��ȣ 
	if(bIsInsertWorkNo) 
	{
		RemoveAliasList.RemoveAll();
		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_WORK_NO, _T("SE_PURCHASE_WORK_NO_TBL"), 0 , pDM);
	}

	// ���� ������ Send Frame
	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("���� �� ���� ����!!!!"));
		return -4000;	
	}

	//�μ� �����ڷ� ���� ��ũ���� ����� �����ڸ� �̾� ������ �ʵ忡 �־��ش�
	if(bIsDepartMngUse)
	{
		CString strTitle;
		CString strPublisher;
		CString strQuery;

		m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("245$a"),strTitle);
		m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("260$b"),strPublisher);
		strQuery.Format(_T("UPDATE SE_PURCHASE_SPECIES_TBL SET TITLE = '%s', PUBLISHER = '%s' ")
			            _T("WHERE REC_KEY = %s;"),strTitle,strPublisher,strREC_KEY);
		
		ids = pDM->ExecuteQuery(strQuery,1);

		if (ids < 0) {
			return -4001;	
		}

	}
	
	return 0;
}

INT CSeApi::SPECIES_PUR_OBJ_DELETE(CESL_DataMgr* pDM, INT nRowIdx)
{
	
	CString strREC_KEY;
	strREC_KEY = pDM->GetCellData(_T("REC_KEY"),nRowIdx);
	if(strREC_KEY.IsEmpty()) return -1;

	CString strQuery;
	strQuery.Format(_T("DELETE FROM SE_PURCHASE_SPECIES_TBL WHERE REC_KEY = %s;"),strREC_KEY);
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	if(ids<0) return -7000;


	return 0;
}

INT CSeApi::SPECIES_PUR_OBJ_FILE_IMPORT(CESL_DataMgr* pDM,
										CMarc* pMarc, 
										INT nRowIdx)
{

	// ����-���Դ���ڷ� �Է�  (���� ����)
	CString strREC_KEY;
	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	pDM->MakeRecKey(strREC_KEY);

	pDM->SetCellData(_T("REC_KEY"),strREC_KEY,nRowIdx);
	pDM->SetCellData(_T("�Է�����"),GetTodayDate(),nRowIdx);
	pDM->SetCellData(_T("�����۾�"),strWorkLog,nRowIdx);		
	pDM->SetCellData(_T("�������۾�"),strWorkLog,nRowIdx);	

	//�Է� ���� ó�� 
	CString strINPUT_TYPE;
	strINPUT_TYPE = pDM->GetCellData(_T("�Է±���"),nRowIdx);	
	if(strINPUT_TYPE.IsEmpty())
	{
		pDM->SetCellData(_T("�Է±���"),_T("0"),nRowIdx);	 //ȭ�� �Է�
	}

	CArray<CString, CString> RemoveAliasList;
	pDM->StartFrame();
	
	// ���� ���� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("SE_PURCHASE_SPECIES_TBL"), nRowIdx , pDM);
	

	// ���� ������ Send Frame
	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		return -3000;	
	}
	
	return 0;
}

INT CSeApi::SPECIES_ORDER(CESL_DataMgr* pDM_PUR_SPECIES, 
						  CESL_DataMgr* pDM_SPECIES, 
						  CESL_DataMgr* pDM_IDX,
						  CESL_DataMgr* pDM_PURCHASE,
						  CESL_DataMgr* pDM_SUB_HISTORY,
						  CMarc* pMarc,
						  INT nRowIdx)
{

	// �ֹ�ó�� �Լ� 
	// ���� ���� ���� ���θ� �Ǵ��� ������ ����� �ְ� ������ ���԰��� ������ �����Ѵ�.

	if(!pDM_PUR_SPECIES || !pDM_SPECIES || !pDM_IDX || !pDM_PURCHASE || !pDM_SUB_HISTORY)
	{
		AfxMessageBox(_T("�ֹ����� ������ ã�� �� �����ϴ�!"));
		return -1;
	}
	
	//������ ���� DM free
	pDM_SPECIES->FreeData();
	pDM_IDX->FreeData();
	pDM_PURCHASE->FreeData();
	pDM_SUB_HISTORY->FreeData();


	CString strSPECIES_KEY = _T("");
	CString strWhere;
	CArray<CString,CString> RemoveAliasList;
	strSPECIES_KEY = pDM_PUR_SPECIES->GetCellData(_T("SS_��KEY"),nRowIdx);
	CString strQuery;

	CString strSEQ_NO = _T("");
	strSEQ_NO =	pDM_PUR_SPECIES->GetCellData(_T("������ȣ"),nRowIdx);


	INT ids = 0;

	BOOL bDepartmentOrderUse = IsDepartMngUse(pDM_SPECIES);

	if(!strSPECIES_KEY.IsEmpty()) //������ ���� �����ϴ� ���� (���� ���� �ڷ�� �����̷¸� ó���� �ش�);
	{
		
		strWhere.Format(_T("SPECIES_KEY = %s"),strSPECIES_KEY);
		pDM_PURCHASE->RefreshDataManager(strWhere);
		if(0 == pDM_PURCHASE->GetRowCount()) 
		{
			AfxMessageBox(_T("�ش� ���� ���������� ã�� �� �����ϴ�!"));
			return -2;
		}
		
		CopyDMToDMByAlias(pDM_PUR_SPECIES,nRowIdx,pDM_PURCHASE,0);
		pDM_SUB_HISTORY->InsertRow(-1);
		
		
		//���������� �����̷������� ����� 
		MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(pDM_PURCHASE, pDM_SUB_HISTORY ,0);
		
		CString strPREV_HISTORY_KEY,strHISTORY_KEY,strPURCHASE_KEY,strPUR_SPECIES_KEY;
		strPUR_SPECIES_KEY = pDM_PUR_SPECIES->GetCellData(_T("REC_KEY"),nRowIdx);
		strPURCHASE_KEY = pDM_PURCHASE->GetCellData(_T("SP_���԰���KEY"),0);
		strPREV_HISTORY_KEY = pDM_PURCHASE->GetCellData(_T("�����̷�KEY"),0);
		pDM_SPECIES->MakeRecKey(strHISTORY_KEY);
		

		pDM_SUB_HISTORY->SetCellData(_T("�����̷�KEY"),strHISTORY_KEY,0);
		pDM_PUR_SPECIES->SetCellData(_T("���籸���̷�KEY"),strHISTORY_KEY,nRowIdx);
		pDM_PUR_SPECIES->SetCellData(_T("���������̷�KEY"),strPREV_HISTORY_KEY,nRowIdx);
		pDM_PUR_SPECIES->SetCellData(_T("�ֹ�����"),GetTodayDate(),nRowIdx);
		pDM_PURCHASE->SetCellData(_T("SP_�����̷�����KEY"),strHISTORY_KEY,0);
		
		SetInsertDefaultValue_SUB_HISTORY(pDM_SUB_HISTORY,0);
		
		//������
		pDM_SPECIES->StartFrame();
		

		//�������� update ���� �����Ѵ�.
		RemoveAliasList.RemoveAll();
		RemoveAliasList.Add(_T("REC_KEY"));
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PUR_SPECIES, _T("SE_PURCHASE_SPECIES_TBL") , nRowIdx , pDM_SPECIES, _T("REC_KEY") , _T("NUMERIC") , strPUR_SPECIES_KEY);

		
		// �����̷������� insert ���� �����Ѵ�.
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SUB_HISTORY, _T("SE_SUBSCRIPTION_HISTORY_TBL") , 0 , pDM_SPECIES);
		
		
		//���������� update���� �����Ѵ�.
		RemoveAliasList.RemoveAll();
		RemoveAliasList.Add(_T("SP_���԰���KEY"));
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("SE_PURCHASE_MANAGE_TBL") , 0 , pDM_SPECIES, _T("REC_KEY") , _T("NUMERIC") , strPURCHASE_KEY);

	
		//�ش� ������ update���� �����Ѵ� (�۾� ���¸� �ֹ����� �����.) 
		if(!strSEQ_NO.IsEmpty())
		{
			//++2008.10.17 UPDATE BY PWR {{++
			// ���սý��� ����
			strQuery.Format( _T("UPDATE SE_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS='1' ")
							 _T("WHERE SEQ_NO=%s AND MANAGE_CODE=UDF_MANAGE_CODE;"), strSEQ_NO );
			// strQuery.Format(_T("UPDATE SE_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS = '1' WHERE SEQ_NO = %s;"),strSEQ_NO);
			//--2008.10.17 UPDATE BY PWR --}}
			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->AddFrame(strQuery);

		}
		

		//���� �μ��� �ֹ� ������ DELETE �ϰ� ���ο� �μ��� �ֹ� ������ ��ü�Ѵ�. 
		if(bDepartmentOrderUse)
		{
			strQuery.Format(_T("DELETE FROM SE_DEPARTMENT_ORDER_TBL WHERE PURCHASE_MANAGE_KEY = %s'"),strPURCHASE_KEY);
			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->AddFrame(strQuery);


			strQuery.Format(_T("UPDATE SE_DEPARTMENT_ORDER_TBL SET PURCHASE_MANAGE_KEY = %s ")
							_T("WHERE PURCHASE_SPECIES_KEY = %s;"),strPURCHASE_KEY,strPUR_SPECIES_KEY);

			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->AddFrame(strQuery);

		}


		ids = pDM_SPECIES->SendFrame();
		pDM_SPECIES->EndFrame();
		if(ids<0) return -5000;

	}
	else //���� ���� �Է��ؾ� �ϴ� ���� 
	{

		pDM_SPECIES->InsertRow(-1);
		pDM_IDX->InsertRow(-1);
		pDM_PURCHASE->InsertRow(-1);
		pDM_SUB_HISTORY->InsertRow(-1);

		//���� ���� ������ ������ ������ copy	
		CopyDMToDMByAlias(pDM_PUR_SPECIES,nRowIdx,pDM_SPECIES,0);
		CopyDMToDMByAlias(pDM_PUR_SPECIES,nRowIdx,pDM_PURCHASE,0);
	

		//KEY������ ���� �� �ش�.
		CString strPUR_SPECIES_KEY;
		strPUR_SPECIES_KEY = pDM_PUR_SPECIES->GetCellData(_T("REC_KEY"),nRowIdx);

		CString strSPECIES_KEY,strPURCHASE_KEY,strHISTORY_KEY,strACQ_EXP_KEY,strPUB_EXP_KEY,strHOLDING_KEY;
		pDM_SPECIES->MakeRecKey(strSPECIES_KEY);
		pDM_SPECIES->MakeRecKey(strPURCHASE_KEY);
		pDM_SPECIES->MakeRecKey(strHISTORY_KEY);
		pDM_SPECIES->MakeRecKey(strACQ_EXP_KEY);
		pDM_SPECIES->MakeRecKey(strPUB_EXP_KEY);
		pDM_SPECIES->MakeRecKey(strHOLDING_KEY);
	

		pDM_SPECIES->SetCellData(_T("SS_��KEY"),strSPECIES_KEY,0);
		pDM_SPECIES->SetCellData(_T("SS_Ȧ��KEY"),strHOLDING_KEY,0);
		pDM_PURCHASE->SetCellData(_T("SP_���԰���KEY"),strPURCHASE_KEY,0);
		pDM_PURCHASE->SetCellData(_T("SP_�Լ���������KEY"),strACQ_EXP_KEY,0);
		pDM_PURCHASE->SetCellData(_T("SP_���࿹������KEY"),strPUB_EXP_KEY,0);
		pDM_PURCHASE->SetCellData(_T("SP_��KEY"),strSPECIES_KEY,0);
		pDM_PURCHASE->SetCellData(_T("SP_�����̷�����KEY"),strHISTORY_KEY,0);
		pDM_SUB_HISTORY->SetCellData(_T("�����̷�KEY"),strHISTORY_KEY,0);
		pDM_PUR_SPECIES->SetCellData(_T("SS_��KEY"),strSPECIES_KEY,nRowIdx);
		pDM_PUR_SPECIES->SetCellData(_T("���籸���̷�KEY"),strHISTORY_KEY,nRowIdx);
		pDM_PUR_SPECIES->SetCellData(_T("���������̷�KEY"),strHISTORY_KEY,nRowIdx);

		
		//���������� �����̷������� ����� 
		MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(pDM_PURCHASE, pDM_SUB_HISTORY ,0);


		//Default ���� ���� ��Ų��.
		SetInsertDefaultValue_SPECIES(pDM_SPECIES,pMarc,0);
		SetInsertDefaultValue_PURCHASE(pDM_PURCHASE,0);
		SetInsertDefaultValue_IDX(pDM_IDX,0);
		SetInsertDefaultValue_SUB_HISTORY(pDM_SUB_HISTORY,0);
		

		//���� ������ ��Ų�� 
		pDM_SPECIES->SetCellData(_T("SS_���Կ���"),_T("Y"),0);
		pDM_SPECIES->SetCellData(_T("SS_��������"),_T("N"),0);
		
	
		//���� ��ũ�� ������ ������ �����Ѵ�
		MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,0);
		

		//�ֹ����� ���� 
		pDM_PUR_SPECIES->SetCellData(_T("�ֹ�����"),GetTodayDate(),nRowIdx);


		//������
		pDM_SPECIES->StartFrame();
 

		// ���� Insert�� Insert���� �����Ѵ�.
		RemoveAliasList.RemoveAll();
		CString strStreamMarc;
		m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
		pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,0);
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), 0 , pDM_SPECIES);


		// ���ο� Insert�� Insert���� �����Ѵ�.
		pDM_IDX->SetCellData(_T("ISS_��KEY"), strSPECIES_KEY, 0);
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), 0, pDM_SPECIES);
		

		// ���԰��������� Insert�� Insert���� �����Ѵ�.
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("SE_PURCHASE_MANAGE_TBL"), 0 , pDM_SPECIES);


		// �����̷������� Insert�� Insert���� �����Ѵ�.
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SUB_HISTORY, _T("SE_SUBSCRIPTION_HISTORY_TBL"), 0 , pDM_SPECIES);


		// ���������� rec_key�� insert�Ѵ� 
 		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strHOLDING_KEY);
		pDM_SPECIES->MakeInsertFrame(_T("SE_HOLDING_TBL"));
		
		
		//�������� update ���� �����Ѵ�.
		RemoveAliasList.RemoveAll();
		RemoveAliasList.Add(_T("REC_KEY"));
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PUR_SPECIES, _T("SE_PURCHASE_SPECIES_TBL") , nRowIdx , pDM_SPECIES, _T("REC_KEY") , _T("NUMERIC") , strPUR_SPECIES_KEY);
		
	
		//�ش� ������ update���� �����Ѵ� (�۾� ���¸� �ֹ����� �����.) 
		if(!strSEQ_NO.IsEmpty())
		{
			//++2008.10.17 UPDATE BY PWR {{++
			// ���սý��� ����
			strQuery.Format( _T("UPDATE SE_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS='1' ")
							 _T("WHERE SEQ_NO=%s AND MANAGE_CODE=UDF_MANAGE_CODE;"), strSEQ_NO );
			// strQuery.Format(_T("UPDATE SE_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS = '1' WHERE SEQ_NO = %s;"),strSEQ_NO);
			//--2008.10.17 UPDATE BY PWR --}}
			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->AddFrame(strQuery);

		}

		//IDX_TMP_SE_TBL ����
		SetIdxTmpSeTbl(pDM_SPECIES, strSPECIES_KEY, _T("I"));


		//�μ��� �ֹ� ������ PURCHASE_MANAGE_KEY�� �����Ѵ�.
		if(bDepartmentOrderUse)
		{
			strQuery.Format(_T("UPDATE SE_DEPARTMENT_ORDER_TBL SET PURCHASE_MANAGE_KEY = %s ")
							_T("WHERE PURCHASE_SPECIES_KEY = %s;"),strPURCHASE_KEY,strPUR_SPECIES_KEY);

			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->AddFrame(strQuery);


		}

		// ������ Send Frame
		ids = pDM_SPECIES->SendFrame();
		pDM_SPECIES->EndFrame();
		if (ids < 0) return -6000;	

	}

	return 0;
}

INT CSeApi::SPECIES_ORDER_CANCEL(CESL_DataMgr* pDM_PUR_SPECIES,
								 CESL_DataMgr* pDM_PURCHASE,
								 CESL_DataMgr* pDM_SUB_HISTORY,
								 INT nRowIdx)
{
	//�ֹ� ��� �Լ�
	//�ֹ� ��ҿ��� ���� �����Ǿ��� ���� �׷��� �ʾƾ� �� ��찡 �����Ѵ�.
	
	/*
	1)���� ���� �� ���: 
		-�ش� ��KEY�� å�� �������� �ʰ�, 
		-�ش� ��KEY�� �������� ���̺��� �������� �ʰ�,
		-���� �����̷� KEY�� ���� �����̷� KEY�� ���� �� 
	
		->�� ��쿡�� ��, ���԰���, ���� ����, �����̷��� ����. 
		->������ �������δ� ���� 

	2)���� ���� ���� ���� ���:
		-1)�� ��Ȳ�� �踮 �� ���.
		
		->�� ��쿡�� ���� �����̷� KEY ������ ���� �� ��� ���� �����̷� ������ 
		���԰��� ������ roll back ��Ų �� ���� �����̷� ������ �����. 
	*/
	
	
	






	return 0;
}

INT CSeApi::CHECK_IN(CESL_DataMgr* pDM_IDX,
					 INT nIdx_IDX, 
					 CESL_DataMgr* pDM_WORK,
					 INT nIdx_WORK, 
					 CESL_DataMgr* pDM_ACQ,
					 INT nACQ_TYPE,
					 CESL_DataMgr* pDM_VOL,
					 CESL_DataMgr* pDM_ACQ_VOL,
					 CESL_DataMgr* pDM_BOOK,
					 CArray<CString,CString>& arrayVolExpKeyList,
					 CArray<CString,CString>& arrayOtherVolExpKeyList,
					 CESL_DataMgr* pDM_BOOK_INSERT,
					 CESL_DataMgr* pDM_APPENDIX,
					 CString strACQ_YEAR,
					 CString strRECEIPT_NO,
					 BOOL bIsDepartMngUse,
					 CESL_DataMgr* pDM_DEPART) 
{

	//üũ�� �Լ� 
	//�Լ����� �ڷḦ ������� 
	//��(1 or 0 row), ���Ա��к���(1 row) , å(n row)�� �Է��Ѵ� 
	
	if(!pDM_IDX || !pDM_WORK || !pDM_ACQ || !pDM_VOL || !pDM_ACQ_VOL || !pDM_BOOK)
	{
		AfxMessageBox(_T("üũ�� ���� ������ ã�� �� �����ϴ�"));
		return -1;
	}

	//�����ӿ��� ���Ǵ� DM�� FREE�Ѵ� 
	pDM_VOL->FreeData();
	pDM_ACQ_VOL->FreeData();
	pDM_BOOK->FreeData();
	pDM_BOOK_INSERT->FreeData();


	//���� ��ȣ �������縦 �Ѵ� 
	BOOL bIsNewVol = TRUE;
	if(InspectDuplicateVol(pDM_WORK,nIdx_WORK,pDM_VOL)<0)
	{
		return -1;
	}
	
	if(pDM_VOL->GetRowCount()>0) bIsNewVol = FALSE;
	
	
	//��ȣ ������ �ľ��� �̹���, Ȥ�� �պ�ȣ�� ���� ó���� �Ѵ�.
	//1: �Ϲ� 2: �պ�ȣ 3: �̹���
	BOOL bIsNonPublish = FALSE;
	CString strVOLUME_CLASS = _T("");
	strVOLUME_CLASS = pDM_WORK->GetCellData(_T("SV_��ȣ����"),nIdx_WORK);
	if(_T("3") == strVOLUME_CLASS) bIsNonPublish = TRUE; //�̹���

	
	INT nTotolBook = 0;
	CString strCHECKIN_STATUS = _T("");
	INT nACQ_BOOK_CNT = 0;
	INT nMISS_BOOK_CNT = 0;


	if(bIsNonPublish) //�̹��� 
	{
		strCHECKIN_STATUS = _T("1");
	}
	else 
	{
		
		//�ηϿ����� �����ϴ����� �ľ��Ѵ�
		CheckAppendix(pDM_WORK,nIdx_WORK,nACQ_TYPE,pDM_APPENDIX,pDM_BOOK_INSERT);


		//�۾��������� å���� ������ ���� ���� �ľ��Ѵ�.
		CString strACQ_BOOK_CNT = _T("");
		CString strMISS_BOOK_CNT = _T("");
		strACQ_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_�Լ�å��"),nIdx_WORK);
		strMISS_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_��ȣå��"),nIdx_WORK);
		nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
		nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));

		if(0 == nACQ_BOOK_CNT && 0 == nMISS_BOOK_CNT)
		{
			AfxMessageBox(_T("ERROR: �Լ� ��ȣ å���� ��� 0 �Դϴ�!"));
			return -1;
		}

		
		if(0 == nMISS_BOOK_CNT) 
			strCHECKIN_STATUS = _T("1"); //�Լ�
		else if(0 == nACQ_BOOK_CNT) 
			strCHECKIN_STATUS = _T("2"); //��ȣ
		else 
			strCHECKIN_STATUS = _T("3"); //�̴�	

		nTotolBook = nACQ_BOOK_CNT + nMISS_BOOK_CNT;

	}

	//��ũ ����ü�� ����� 
	CString strMarcStream;
	strMarcStream = pDM_IDX->GetCellData(_T("MARC"),nIdx_IDX);
	CMarc Marc;
	m_pMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pMgr->m_pInfo->pMarcMgr->Decoding(strMarcStream, &Marc);


	//���ΰ� �۾���������  ��ȣ������ �����Ѵ�. (�������翡 �ɸ��� �ʾ��� ���) 
    if(bIsNewVol) 
	{
		MAKE_VOL_by_IDX_WORK(pDM_IDX,
							 nIdx_IDX,
							 pDM_WORK,
							 nIdx_WORK,
							 pDM_VOL,
							 &Marc,
							 strVOLUME_CLASS);
	}

	//���ΰ� ���Ա�������, �۾���������  ���Ա��к� ��ȣ������ �����Ѵ�
	MAKE_ACQ_VOL_by_IDX_ACQ_WORK(pDM_ACQ,
								 nACQ_TYPE,
								 pDM_WORK,
								 nIdx_WORK,
								 pDM_ACQ_VOL,
								 strCHECKIN_STATUS);

	
	//å�Է��� �����Ѵ� 
	if(!bIsNonPublish)
	{
		MAKE_BOOK_by_ACQ_WORK(pDM_ACQ,
							  nACQ_TYPE,
							  pDM_WORK,
							  nIdx_WORK,
							  pDM_BOOK,
							  nACQ_BOOK_CNT,
							  nMISS_BOOK_CNT,
							  strACQ_YEAR,
							  strRECEIPT_NO,
							  bIsDepartMngUse,
							  pDM_DEPART);
	}
	
	//�� ������ ����� foreign KEY�� �����Ѵ� 
	CString strVOL_KEY = _T("");
	strVOL_KEY = pDM_VOL->GetCellData(_T("SV_��KEY"),0);
	CString strACQ_VOL_KEY = _T("");
	strACQ_VOL_KEY = pDM_ACQ_VOL->GetCellData(_T("SA_���Ա��к���KEY"),0);
	
	//��KEY ����
	pDM_ACQ_VOL->SetCellData(_T("SA_��KEY"),strVOL_KEY,0);
	
		
	CArray<CString,CString> RemoveAliasList;
	pDM_VOL->StartFrame();

	if(bIsNewVol)
	{
		//�� query
		RemoveAliasList.RemoveAll();
		pDM_VOL->InitDBFieldData();
		pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_VOL, _T("SE_VOL_TBL"), 0, pDM_VOL);
	
		//���� ��ȣ�� setting
		MakeEcoIndexVol(pDM_VOL,strVOL_KEY);
	}

	//���Ա��к��� query
	RemoveAliasList.RemoveAll();
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_ACQ_VOL, _T("SE_ACQ_TYPE_VOL_TBL"), 0, pDM_VOL);


	CString sCopyCode = _T("");
	//å query
	for(INT i=0;i<nTotolBook;i++)
	{
		pDM_BOOK->SetCellData(_T("SB_��KEY"),strVOL_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_���Ա��к���KEY"),strACQ_VOL_KEY,i);

		if( i > 0)
		{
			sCopyCode.Format(_T("%d"),i+1);
			pDM_BOOK->SetCellData(_T("SB_������ȣ"),sCopyCode,i);
		}
		
		RemoveAliasList.RemoveAll();
		pDM_VOL->InitDBFieldData();
		pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_BOOK, _T("SE_BOOK_TBL"), i , pDM_VOL);
	}
	

	//�η� query
	INT nTotalAppendixBook = pDM_BOOK_INSERT->GetRowCount();
	if(nTotalAppendixBook>0)
	{
		
		INT nTotalAppendix = pDM_APPENDIX->GetRowCount();
		CString strUDF;

		for(i=0;i<nTotalAppendix;i++)
		{
			strUDF = _T("");
			strUDF = pDM_APPENDIX->GetCellData(_T("UDF_����"),i);
			if(_T("I") != strUDF) continue;
			
			
			pDM_VOL->SetCellData(_T("��KEY"),strVOL_KEY,i);
			RemoveAliasList.RemoveAll();
			pDM_VOL->InitDBFieldData();
			pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_APPENDIX, _T("SE_APPENDIX_BOOK_TBL"), i , pDM_VOL);
			pDM_APPENDIX->SetCellData(_T("UDF_����"),_T(""),i); //�ٽ� ��ĭ���� 
		}
		
		//2018�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���		
		//KOL.RED.019-002 JY ���Ӱ��๰ ��� ����ȭ
		//ADD===============================================================
		CString strSBWorkingStatus = _T("");
		pDM_BOOK->GetCellData(_T("SB_�ڷ����"),0,strSBWorkingStatus);
		//==================================================================
		
		for(i=0;i<nTotalAppendixBook;i++)
		{
			pDM_BOOK_INSERT->SetCellData(_T("SB_��KEY"),strVOL_KEY,i);
			pDM_BOOK_INSERT->SetCellData(_T("SB_���Ա��к���KEY"),strACQ_VOL_KEY,i);
			//2018�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
			//KOL.RED.019-002 JY ���Ӱ��๰ ��� ����ȭ
			//ADD===============================================================
			pDM_BOOK_INSERT->SetCellData(_T("SB_�ڷ����"),strSBWorkingStatus,i);
			//==================================================================
			
			RemoveAliasList.RemoveAll();
			pDM_VOL->InitDBFieldData();
			pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_BOOK_INSERT, _T("SE_BOOK_TBL"), i , pDM_VOL);
		}
	}

					

	//�����ڷῡ ���� update ó��
	UpdateExpectData(pDM_VOL,
		             pDM_ACQ,
					 nACQ_TYPE,
					 pDM_WORK,
					 nIdx_WORK,
					 arrayVolExpKeyList,
					 arrayOtherVolExpKeyList);
				


	CString strQuery;

	//������ȣ�Ϸù�ȣ 
	CString strLAST_VOL_SORT_NO,strSPECIES_KEY;
	strLAST_VOL_SORT_NO = pDM_IDX->GetCellData(_T("SS_������ȣ�Ϸù�ȣ"),nIdx_IDX);
	strSPECIES_KEY = pDM_IDX->GetCellData(_T("ISS_��KEY"),nIdx_IDX);
	strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET LAST_VOL_SORT_NO = %s WHERE REC_KEY = %s;"),strLAST_VOL_SORT_NO,strSPECIES_KEY);
	pDM_VOL->InitDBFieldData();
	pDM_VOL->AddFrame(strQuery);
	

	//���� UPDATE
	APPLY_IndexItem_Book(pDM_VOL, strSPECIES_KEY);


	// Send Frame
	INT ids = pDM_VOL->SendFrame();
	pDM_VOL->EndFrame();
	if (ids < 0) {
		return -1000;	
	}
	

	
	//�Լ��� refresh
	if(_T("2") != strCHECKIN_STATUS && !bIsNonPublish) //��ȣ�� �ƴϰ�,  �̹����� �ƴ� �ڷῡ ���ؼ�
	{
		CString strACQ_DATE;
		strACQ_DATE = pDM_WORK->GetCellData(_T("UDF_�Լ���"),nIdx_WORK);
		
		//������ �Լ����� ���� ���� ���� ��¥�� �־��ش�.
		if(strACQ_DATE.IsEmpty())
		{
			pDM_WORK->SetCellData(_T("UDF_�Լ���"),GetTodayDate(),nIdx_WORK);
		}

	}

	//�ηϼ� refresh
	if(!bIsNonPublish)
	{
		CString strAppendixCnt;
		strAppendixCnt.Format(_T("%d"),nTotalAppendixBook);
		pDM_WORK->SetCellData(_T("UDF_�ηϼ�"),strAppendixCnt,nIdx_WORK);
	}

	//���� refresh
	if(!bIsNonPublish)
	{
		pDM_WORK->SetCellData(_T("UDF_����"),_T("0"),nIdx_WORK);
	}
	
	//������ �ʿ����ڷ�(��, ���Ա��к���)�� pDM_WORK�� �־��ش�
	CopyDMToDMByAlias(pDM_VOL,0,pDM_WORK,nIdx_WORK);
	CopyDMToDMByAlias(pDM_ACQ_VOL,0,pDM_WORK,nIdx_WORK);


	return 0;
}

INT CSeApi::ACQ_DELAY(CESL_DataMgr* pDM_IDX,
					 INT nIdx_IDX, 
					 CESL_DataMgr* pDM_WORK,
					 INT nIdx_WORK, 
					 CESL_DataMgr* pDM_ACQ,
					 INT nACQ_TYPE,
					 CESL_DataMgr* pDM_VOL,
					 CESL_DataMgr* pDM_ACQ_VOL,
					 CESL_DataMgr* pDM_BOOK,
					 CArray<CString,CString>& arrayVolExpKeyList,
					 CArray<CString,CString>& arrayOtherVolExpKeyList)
					 
					 
{

	//�Լ����� �Լ� 
	//�Լ����� �ڷḦ ������� 
	//��(1 or 0 row), ���Ա��к���(1 row) , ��ȣå(n row)�� �Է��Ѵ� 
	
	if(!pDM_IDX || !pDM_WORK || !pDM_ACQ || !pDM_VOL || !pDM_ACQ_VOL || !pDM_BOOK)
	{
		AfxMessageBox(_T("���� DM �� ã�� �� �����ϴ�"));
		return -1;
	}

	//�����ӿ��� ���Ǵ� DM�� FREE�Ѵ� 
	pDM_VOL->FreeData();
	pDM_ACQ_VOL->FreeData();
	pDM_BOOK->FreeData();


	//�۾��������� å���� ������ ���� ���� �ľ��Ѵ�.
	CString strMISS_BOOK_CNT = _T("");
	INT nMISS_BOOK_CNT;
	strMISS_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_��ȣå��"),nIdx_WORK);
	nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));

	if(0 == nMISS_BOOK_CNT)
	{
		AfxMessageBox(_T("ERROR: ��ȣ å���� 0 �Դϴ�!"));
		return -1;
	}

	CString strCHECKIN_STATUS;
	strCHECKIN_STATUS = _T("2"); //��ȣ
	INT nTotolBook = nMISS_BOOK_CNT;
	

	//��ũ ����ü�� ����� 
	CString strMarcStream;
	strMarcStream = pDM_IDX->GetCellData(_T("MARC"),nIdx_IDX);
	CMarc Marc;
	m_pMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pMgr->m_pInfo->pMarcMgr->Decoding(strMarcStream, &Marc);


	//���ΰ� �۾���������  ��ȣ������ �����Ѵ�.
    MAKE_VOL_by_IDX_WORK(pDM_IDX,nIdx_IDX,pDM_WORK,nIdx_WORK,pDM_VOL,&Marc);


	//���ΰ� ���Ա�������, �۾���������  ���Ա��к� ��ȣ������ �����Ѵ�
	MAKE_ACQ_VOL_by_IDX_ACQ_WORK(pDM_ACQ,nACQ_TYPE,pDM_WORK,nIdx_WORK,pDM_ACQ_VOL,strCHECKIN_STATUS);

	
	//å�Է��� �����Ѵ� 
	MAKE_BOOK_by_ACQ_WORK(pDM_ACQ,nACQ_TYPE,pDM_WORK,nIdx_WORK,pDM_BOOK,0,nMISS_BOOK_CNT,_T(""),_T(""));

	
	//�� ������ ����� foreign KEY�� �����Ѵ� 
	CString strVOL_KEY = _T("");
	strVOL_KEY = pDM_VOL->GetCellData(_T("SV_��KEY"),0);
	CString strACQ_VOL_KEY = _T("");
	strACQ_VOL_KEY = pDM_ACQ_VOL->GetCellData(_T("SA_���Ա��к���KEY"),0);
	

	//��KEY ����
	pDM_ACQ_VOL->SetCellData(_T("SA_��KEY"),strVOL_KEY,0);
	CArray<CString,CString> RemoveAliasList;
	pDM_VOL->StartFrame();
	

	//�� query
	RemoveAliasList.RemoveAll();
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_VOL, _T("SE_VOL_TBL"), 0, pDM_VOL);
	
	
	//���� ��ȣ�� setting
	MakeEcoIndexVol(pDM_VOL,strVOL_KEY);


	//���Ա��к��� query
	RemoveAliasList.RemoveAll();
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_ACQ_VOL, _T("SE_ACQ_TYPE_VOL_TBL"), 0, pDM_VOL);


	//å query
	for(INT i=0;i<nTotolBook;i++)
	{
		pDM_BOOK->SetCellData(_T("SB_��KEY"),strVOL_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_���Ա��к���KEY"),strACQ_VOL_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_å����"),_T("L"),i);


		RemoveAliasList.RemoveAll();
		pDM_VOL->InitDBFieldData();
		pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_BOOK, _T("SE_BOOK_TBL"), i , pDM_VOL);
	}

	//�����ڷῡ ���� update ó��
	UpdateExpectData(pDM_VOL,pDM_ACQ,nACQ_TYPE,pDM_WORK,nIdx_WORK,arrayVolExpKeyList,arrayOtherVolExpKeyList);

	//������ȣ�Ϸù�ȣ 
	CString strLAST_VOL_SORT_NO,strSPECIES_KEY;
	strLAST_VOL_SORT_NO = pDM_IDX->GetCellData(_T("SS_������ȣ�Ϸù�ȣ"),nIdx_IDX);
	strSPECIES_KEY = pDM_IDX->GetCellData(_T("ISS_��KEY"),nIdx_IDX);
	CString strQuery;
	strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET LAST_VOL_SORT_NO = %s WHERE REC_KEY = %s;"),strLAST_VOL_SORT_NO,strSPECIES_KEY);
	pDM_VOL->InitDBFieldData();
	pDM_VOL->AddFrame(strQuery);

	// Send Frame
	INT ids = pDM_VOL->SendFrame();
	pDM_VOL->EndFrame();
	if (ids < 0) {
		return -1000;	
	}
	
	return 0;
}

INT CSeApi::VOL_UPDATE(CESL_DataMgr* pDM_IDX,
					   INT nIdx_IDX, 
					   CESL_DataMgr* pDM_WORK,
					   INT nIdx_WORK, 
					   CESL_DataMgr* pDM_ACQ,
					   INT nACQ_TYPE,
					   CESL_DataMgr* pDM_VOL,
					   CESL_DataMgr* pDM_ACQ_VOL,
					   CESL_DataMgr* pDM_BOOK,
					   CESL_DataMgr* pDM_BOOK_INSERT,
					   CString strACQ_YEAR,
					   CString strRECEIPT_NO,
					   BOOL bIsLightVersion,
					   BOOL bIsDepartMngUse,
					   CESL_DataMgr* pDM_DEPART) 
{
	
	//�� ����  �Լ� 
	//�۾� DM �ڷḦ ������� 
	//��(1 row), ���Ա��к���(1 row) update 
	//å(n row) insert or delete or update - å�����ÿ��� �ڷ���¸� Ȯ���Ͽ� ����� ���� ��츦 ����Ѵ� 
	if(!pDM_IDX || !pDM_WORK || !pDM_ACQ || !pDM_VOL || !pDM_ACQ_VOL || !pDM_BOOK)
	{
		AfxMessageBox(_T("���� DM �� ã�� �� �����ϴ�"));
		return -1;
	}

	//�����ӿ��� ���Ǵ� DM�� FREE�Ѵ� 
	pDM_VOL->FreeData();
	pDM_ACQ_VOL->FreeData();
	pDM_BOOK->FreeData();
	pDM_BOOK_INSERT->FreeData();


	//�۾��������� ��, ���Ա��� ������ �����´�.
	pDM_VOL->InsertRow(-1);
	CopyDMToDMByAlias(pDM_WORK, nIdx_WORK, pDM_VOL,0);
	pDM_ACQ_VOL->InsertRow(-1);
	CopyDMToDMByAlias(pDM_WORK, nIdx_WORK, pDM_ACQ_VOL,0);
	
	
	//SV_��KEY
	CString strVOL_KEY;
	strVOL_KEY = pDM_WORK->GetCellData(_T("SV_��KEY"),nIdx_WORK);

	//SA_���Ա��к���KEY
	CString strACQ_VOL_KEY;
	strACQ_VOL_KEY = pDM_WORK->GetCellData(_T("SA_���Ա��к���KEY"),nIdx_WORK);

	
	//SV_��ȣ��
	CString strVOL_TITLE;
	strVOL_TITLE = pDM_WORK->GetCellData(_T("SV_��ȣ��"),nIdx_WORK);
	
	
	//SV_��������ȣ�� 
	CString strOTHER_VOL_TITLE;
	strOTHER_VOL_TITLE = pDM_WORK->GetCellData(_T("SV_��������ȣ��"),nIdx_WORK);
	

	//SV_���α�ȣ��
	CString strIDX_VOL_TITLE;
	CArray<CString,CString> arrayList;
	arrayList.Add(strVOL_TITLE);
	arrayList.Add(strOTHER_VOL_TITLE);
	strIDX_VOL_TITLE = MakeKeyWord(arrayList);
	pDM_VOL->SetCellData(_T("SV_���α�ȣ��"),strIDX_VOL_TITLE,0);


	//���������� �ľ��Ѵ�.
	//BOOK_TYPE  C:�Ϲ�  M:��ȣ  L:���� 
	CString strWhere;
	strWhere.Format(_T("ACQ_TYPE_VOL_KEY = %s AND (BOOK_TYPE = 'C' OR BOOK_TYPE = 'M' OR BOOK_TYPE = 'L')"),strACQ_VOL_KEY); 
	pDM_BOOK->RefreshDataManager(strWhere);
	INT nDB_ACQ_BOOK_CNT = 0;
	INT nDB_MISS_BOOK_CNT = 0;
	INT nDeletePossible = 0;
	CString strBOOK_TYPE;
	CString strWORKING_STATUS;
	CString strSHELF_LOC_CODE;
	for(INT i=0;i<pDM_BOOK->GetRowCount();i++)
	{
		strSHELF_LOC_CODE = _T("");
		strSHELF_LOC_CODE = pDM_WORK->GetCellData(_T("SS_�谡��ġ_�ڵ�"),i);

		strBOOK_TYPE = _T("");
		strBOOK_TYPE = pDM_BOOK->GetCellData(_T("SB_å����"),i);
		CString strResultCode;
		if(_T("C") == strBOOK_TYPE) {
			
			strWORKING_STATUS = _T("");
			strWORKING_STATUS = pDM_BOOK->GetCellData(_T("SB_�ڷ����"),i);
			//�ڷ���°� �ǽð�, ����, ��� ����� ���� ������ �����ϴ�  
			if(_T("SEL111O") == strWORKING_STATUS || _T("SEB111N") == strWORKING_STATUS || _T("SER111N") == strWORKING_STATUS) nDeletePossible++;
			nDB_ACQ_BOOK_CNT++;
			
			//�μ��� �����ڷ� �߰�.. HS
			INT ids = GetNextPriorityShelfLocCode(strResultCode,
											  strSHELF_LOC_CODE,
											  pDM_DEPART,
											  pDM_BOOK);
			if(ids<0) continue;

			pDM_BOOK->SetCellData(_T("SB_�谡��ġ_�ڵ�"),strResultCode,i);
		}	

		if(_T("M") == strBOOK_TYPE || _T("L") == strBOOK_TYPE) nDB_MISS_BOOK_CNT++;	
	}
	INT nDB_TOTAL_BOOK_CNT = nDB_ACQ_BOOK_CNT + nDB_MISS_BOOK_CNT;

	
	//��ȣ����
	//1: �Ϲ� 2: �պ�ȣ 3: �̹���
	BOOL bIsNonPublish = FALSE;
	CString strVOLUME_CLASS = _T("");
	strVOLUME_CLASS = pDM_WORK->GetCellData(_T("SV_��ȣ����"),nIdx_WORK);
	if(_T("3") == strVOLUME_CLASS) bIsNonPublish = TRUE; //�̹���
	

	if(nDB_TOTAL_BOOK_CNT != 0 && bIsNonPublish) //�űԷ� �̹��� ó���� ��� 
	{
		return VOL_UPDATE_MAKE_NON_PUBLISH(pDM_VOL,pDM_ACQ_VOL,pDM_WORK,nIdx_WORK);
	}



	//�۾��������� å���� ������ ���� ���� �ľ��Ѵ�.
	CString strACQ_BOOK_CNT = _T("");
	CString strMISS_BOOK_CNT = _T("");
	INT nACQ_BOOK_CNT;
	INT nMISS_BOOK_CNT;
	strACQ_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_�Լ�å��"),nIdx_WORK);
	strMISS_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_��ȣå��"),nIdx_WORK);
	nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
	nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));

	CString strCHECKIN_STATUS;
	if(0 == nMISS_BOOK_CNT) 
		strCHECKIN_STATUS = _T("1"); //�Լ�
	else if(0 == nACQ_BOOK_CNT) 
		strCHECKIN_STATUS = _T("2"); //��ȣ
	else 
		strCHECKIN_STATUS = _T("3"); //�̴�	
	
	//�Լ����� ó�� 
	pDM_ACQ_VOL->SetCellData(_T("SA_�Լ�����"),strCHECKIN_STATUS,0);
		

	//��ȭ�� å���� ���� å���� ���Ѵ�.
	INT nWORK_TOTAL_BOOK_CNT = nACQ_BOOK_CNT + nMISS_BOOK_CNT;
	INT nTotalBookGap = nWORK_TOTAL_BOOK_CNT - nDB_TOTAL_BOOK_CNT;
	INT nAcqBookGap = nACQ_BOOK_CNT - nDB_ACQ_BOOK_CNT;
	INT nMissBookGap = nMISS_BOOK_CNT - nDB_MISS_BOOK_CNT;
	

	CString msg;
	if(nAcqBookGap<0) //�Լ�å ���� 
	{
		if(nDeletePossible + nAcqBookGap <0) //���ҵǾ��� �Լ�å�� ������ �Լ�å���� ū ��� 
		{
			
			if(bIsLightVersion)
			{
				msg.Format(_T("[ %s ] ���� ó���� ���� �� å �߿� �̹� �ΰ�� å�� �־� \n") 
							_T("�۾��� ���� �� �� �����ϴ�!\n")
							_T("�̷��� å���� å���� ȭ�鿡�� Ȯ���Ͻ� �� �����Ͻ� �� �ֽ��ϴ�"),strVOL_TITLE);   
				
			}
			else
			{
				msg.Format(_T("[ %s ] ���� ó���� ���� �� å �߿� [�ǽð� ���],[���� ���], [��� ���]\n") 
							_T("�̿��� å�� �־� �۾��� ���� �� �� �����ϴ�!\n")
							_T("�̷��� å���� å���� ȭ�鿡�� Ȯ���Ͻ� �� �����Ͻ� �� �ֽ��ϴ�"),strVOL_TITLE);   
				
			}
			AfxMessageBox(msg);   
			return -1;
		}
	}
	

	if(nTotalBookGap>0) //�� å���� ���� ��� 
	{
		for(i=0;i<nTotalBookGap;i++)
		{
			pDM_BOOK_INSERT->InsertRow(-1);
			pDM_BOOK_INSERT->SetCellData(_T("UDF_����"),_T("I"),i); //insert
		}

		//å�⺻���� ���� 
		VOL_UPDATE_NEW_BOOK(pDM_ACQ,nACQ_TYPE,
			                pDM_WORK,nIdx_WORK,
							pDM_BOOK_INSERT,
							bIsDepartMngUse);
	}
		
	if(0 != nAcqBookGap || 0 != nMissBookGap) //å�� ��ȭ�� ���� ��� 
	{
		//å�� ���¸� ������ pDM_BOOK_INSERT �� copy�Ѵ�. 
		VOL_UPDATE_SET_BOOK_STATUS(pDM_WORK,
								   nIdx_WORK,
								   pDM_BOOK,
								   pDM_BOOK_INSERT,
								   nACQ_TYPE,
								   strACQ_YEAR,
								   strRECEIPT_NO,
								   bIsDepartMngUse,
								   pDM_DEPART);
	}
	
	
	//������ �۾� setting
	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	

	//������ ���� 
	CArray<CString,CString> RemoveAliasList;
	pDM_VOL->StartFrame();
	

	// �������� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("SV_��KEY"));
	pDM_VOL->SetCellData(_T("SV_�������۾�"),strWorkLog,0);
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_VOL, _T("SE_VOL_TBL") , 0 , pDM_VOL, _T("REC_KEY") , _T("NUMERIC") , strVOL_KEY);
	
	
	//���� ��ȣ�� setting
	MakeEcoIndexVol(pDM_VOL,strVOL_KEY);

	
	// ���Ա��к��������� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("SA_���Ա��к���KEY"));
	pDM_ACQ_VOL->SetCellData(_T("SA_�������۾�"),strWorkLog,0);
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_ACQ_VOL, _T("SE_ACQ_TYPE_VOL_TBL") , 0 , pDM_VOL, _T("REC_KEY") , _T("NUMERIC") , strACQ_VOL_KEY);
	
	
	
	if(0 != nAcqBookGap || 0 != nMissBookGap) //å�� ��ȭ�� ���� ��� 
	{
		CString strUDF;
		CString strQuery;
		CString strBOOK_KEY;

		//å���� ������ �����Ѵ� 
		for(INT i=0;i<pDM_BOOK_INSERT->GetRowCount();i++)
		{
			strUDF = _T("");
			strUDF = pDM_BOOK_INSERT->GetCellData(_T("UDF_����"),i);
			
			if(_T("I") == strUDF) //insert
			{

				RemoveAliasList.RemoveAll();
				pDM_VOL->InitDBFieldData();
				pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_BOOK_INSERT, _T("SE_BOOK_TBL"), i , pDM_VOL);

			}
			else if(_T("U") == strUDF) //update
			{

				strBOOK_KEY = _T("");
				RemoveAliasList.RemoveAll();
				RemoveAliasList.Add(_T("SB_åKEY"));
				strBOOK_KEY = pDM_BOOK_INSERT->GetCellData(_T("SB_åKEY"),i);
				pDM_VOL->InitDBFieldData();
				pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_BOOK_INSERT, _T("SE_BOOK_TBL") , i , pDM_VOL, _T("REC_KEY") , _T("NUMERIC") , strBOOK_KEY);

			}
			else if(_T("D") == strUDF) //delete 
			{
				strBOOK_KEY = _T("");
				strBOOK_KEY = pDM_BOOK_INSERT->GetCellData(_T("SB_åKEY"),i);
				strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE REC_KEY = %s;"),strBOOK_KEY);
				pDM_VOL->InitDBFieldData();
				pDM_VOL->AddFrame(strQuery);
			}
		}
	

		//ISS_��KEY
		CString strSPECIES_KEY;
		strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_��KEY"),nIdx_WORK);

		//���� UPDATE
		APPLY_IndexItem_Book(pDM_VOL, strSPECIES_KEY);
	}
	


	//�����ڷ� refresh
	//�Լ� ���°� �̴� Ȥ�� ��ȣ�� ���� ���� �ڷḦ ��ȣ�ڷ�� �����
	CString strUDF;
	strUDF = pDM_WORK->GetCellData(_T("UDF_�Լ�����"),nIdx_WORK);

	if(_T("����") != strUDF)
	{
		if(_T("��ȣ") == strUDF || _T("�̴�") == strUDF)
		{
			CString strQuery;
			strQuery.Format(_T("UPDATE SE_BOOK_TBL SET BOOK_TYPE = 'M' WHERE BOOK_TYPE = 'L' AND ACQ_TYPE_VOL_KEY = %s;"),strACQ_VOL_KEY);
			pDM_VOL->InitDBFieldData();
			pDM_VOL->AddFrame(strQuery);
			
			//WORK DM���� UDF_�������� 0���� �����
			pDM_WORK->SetCellData(_T("UDF_������"),_T("0"),nIdx_WORK);
			
		}
		
	}
	

	//�Լ��� refresh
	if(_T("2") != strCHECKIN_STATUS && !bIsNonPublish) //��ȣ�� �ƴϰ�,  �̹����� �ƴ� �ڷῡ ���ؼ�
	{
		CString strACQ_DATE;
		strACQ_DATE = pDM_WORK->GetCellData(_T("UDF_�Լ���"),nIdx_WORK);
		
		//������ �Լ����� ���� ���� ���� ��¥�� �־��ش�.
		if(strACQ_DATE.IsEmpty())
		{
			pDM_WORK->SetCellData(_T("UDF_�Լ���"),GetTodayDate(),nIdx_WORK);
		}

	}
	
	// Send Frame
	INT ids = pDM_VOL->SendFrame();
	pDM_VOL->EndFrame();
	if (ids < 0) {
		return -2000;	
	}
	

	//������ �ʿ����ڷ�(��, ���Ա��к���)�� pDM_WORK�� �־��ش�
	if(_T("����") != strUDF)
	{
		if(_T("��ȣ") == strUDF || _T("�̴�") == strUDF)
		{
			pDM_WORK->SetCellData(_T("UDF_������"),_T("0"),nIdx_WORK);
		}
		
	}
	
	CopyDMToDMByAlias(pDM_VOL,0,pDM_WORK,nIdx_WORK);
	CopyDMToDMByAlias(pDM_ACQ_VOL,0,pDM_WORK,nIdx_WORK);

	return 0;
}

INT CSeApi::VOL_UPDATE_MAKE_NON_PUBLISH(CESL_DataMgr* pDM_VOL,
										CESL_DataMgr* pDM_ACQ_VOL,
										CESL_DataMgr* pDM_WORK,
										INT nIdx_WORK)
{
	//�űԷ� DB ������ �̹��� ó���� ��쿡 ����ϴ� �Լ�
	//SV_��KEY
	CString strVOL_KEY;
	strVOL_KEY = pDM_WORK->GetCellData(_T("SV_��KEY"),nIdx_WORK);

	//SA_���Ա��к���KEY
	CString strACQ_VOL_KEY;
	strACQ_VOL_KEY = pDM_WORK->GetCellData(_T("SA_���Ա��к���KEY"),nIdx_WORK);
	
	//�Լ����´� �Լ� 
	pDM_ACQ_VOL->SetCellData(_T("SA_�Լ�����"),_T("1"),0);
	
	//������ ���� 
	CArray<CString,CString> RemoveAliasList;
	pDM_VOL->StartFrame();
	

	// �������� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("SV_��KEY"));
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_VOL, _T("SE_VOL_TBL") , 0 , pDM_VOL, _T("REC_KEY") , _T("NUMERIC") , strVOL_KEY);
	

	//���� ��ȣ�� setting
	MakeEcoIndexVol(pDM_VOL,strVOL_KEY);


	// ���Ա��к��������� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("SA_���Ա��к���KEY"));
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_ACQ_VOL, _T("SE_ACQ_TYPE_VOL_TBL") , 0 , pDM_VOL, _T("REC_KEY") , _T("NUMERIC") , strACQ_VOL_KEY);
	

	//å������ ��� �����Ѵ�
	CString strQuery;
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE ACQ_TYPE_VOL_KEY = %s;"),strACQ_VOL_KEY);
	pDM_VOL->AddFrame(strQuery);

	
	// Send Frame
	INT ids = pDM_VOL->SendFrame();
	pDM_VOL->EndFrame();
	if (ids < 0) {
		return -2000;	
	}
	

	//refresh
	pDM_WORK->SetCellData(_T("UDF_������"),_T("0"),nIdx_WORK);
	pDM_WORK->SetCellData(_T("UDF_�Լ���"),_T(""),nIdx_WORK);
	CopyDMToDMByAlias(pDM_VOL,0,pDM_WORK,nIdx_WORK);
	CopyDMToDMByAlias(pDM_ACQ_VOL,0,pDM_WORK,nIdx_WORK);


	return 0;
}

INT CSeApi::VOL_DELETE_VERIFY(CESL_DataMgr* pDM_WORK,INT nIdx_WORK)
{
	//üũ�ο��� ���Ǵ� �� ���� FLOW �߿��� �ش��ȣ�� 
	//���� �Ҽ� ���� å�� �ִ����� ���� �˻�.

	if(!pDM_WORK) return -1;
	CString strVOL_KEY,strACQ_VOL_KEY;
	strVOL_KEY = pDM_WORK->GetCellData(_T("SV_��KEY"),nIdx_WORK);
	strACQ_VOL_KEY = pDM_WORK->GetCellData(_T("SA_���Ա��к���KEY"),nIdx_WORK);
	if(strVOL_KEY.IsEmpty() || strACQ_VOL_KEY.IsEmpty()) return -2;
	

	CString strQuery;
	CString strResult = _T("");

	//�ش� ���Ա��к���KEY�� ���� ������ �� ���� å�� �ִ� ���� �����Ѵ�
	//++2008.10.17 UPDATE BY PWR {{++
	// ���սý��� ����
	strQuery.Format( _T("SELECT COUNT(*) FROM SE_BOOK_TBL ")
					 _T("WHERE ACQ_TYPE_VOL_KEY=%s ")
					 _T("AND WORKING_STATUS NOT IN ('SEB111N','SEL111O','SER111N','SEA112N','SEA212N') ")
					 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strACQ_VOL_KEY );
	// strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE ACQ_TYPE_VOL_KEY = %s AND WORKING_STATUS NOT IN ('SEB111N','SEL111O','SER111N','SEA112N','SEA212N')"),strACQ_VOL_KEY);
	//--2008.10.17 UPDATE BY PWR --}}
	pDM_WORK->GetOneValueQuery(strQuery,strResult);
	INT nIllegalCnt = _ttoi(strResult.GetBuffer(0));

	if(nIllegalCnt>0) 
	{
		return -1001; //������ �� ���� å ����
	}
	
	/* ��� �η��� ��ȣ�� �Բ� ���� ��Ų��
	CString strArticleCnt;
	INT nArticleCnt = 0;
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s"),strVOL_KEY);
	pDM_WORK->GetOneValueQuery(strQuery,strArticleCnt);
	nArticleCnt = _ttoi(strArticleCnt.GetBuffer(0));
					
	if(nArticleCnt>0)
	{
		return -1002; //��� ���� ����  
	}
				
	CString strAppendixCnt;
	INT nAppendixCnt = 0;
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND BOOK_TYPE = 'A'"),strVOL_KEY);
	pDM_WORK->GetOneValueQuery(strQuery,strAppendixCnt);
	nAppendixCnt = _ttoi(strAppendixCnt.GetBuffer(0));
	
	if(nAppendixCnt>0)
	{
		return -1003; //�η� ���� ���� 
	}
	*/

	CString strArticleCnt;
	INT nArticleCnt = 0;
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s"),strVOL_KEY);
	pDM_WORK->GetOneValueQuery(strQuery,strArticleCnt);
	nArticleCnt = _ttoi(strArticleCnt.GetBuffer(0));
				
	CString strAppendixCnt;
	INT nAppendixCnt = 0;
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND BOOK_TYPE = 'A'"),strVOL_KEY);
	pDM_WORK->GetOneValueQuery(strQuery,strAppendixCnt);
	nAppendixCnt = _ttoi(strAppendixCnt.GetBuffer(0));

	INT nReturnOption = 0;

	if(nArticleCnt>0) nReturnOption = 1;
	
	if(nAppendixCnt>0) {
		if(0 == nReturnOption) nReturnOption = 2;
		else nReturnOption = 3;
	}
	
	if(1 == nReturnOption) return -1002; //��� ����
	if(2 == nReturnOption) return -1003; //�η� ���� 
	if(3 == nReturnOption) return -1004; //��� �� �η� ���� 

	return 0;
}

INT CSeApi::VOL_DELETE(CESL_DataMgr* pDM_WORK,
					   INT nIdx_WORK, 
					   INT nACQ_TYPE)
{
	
	// üũ�� ȭ�鿡�� ���Ǵ� �� ���� �Լ� 
	// �ٸ� ���Ա��� ������ �ľ��ؼ� �Ǳ��� ������ �������� �Ǵ� 
	// �ش��ȣ�� å ���¸� �ľ��� ����� ���� ���� return
	// ��(1 row or 0 row delete)
	// ���Ա��к��� (1row delete)
	// å , �η� , Ŭ���� �̷� (n row delete) 
	
	if(!pDM_WORK) return -1;
	CString strVOL_KEY,strACQ_VOL_KEY;
	strVOL_KEY = pDM_WORK->GetCellData(_T("SV_��KEY"),nIdx_WORK);
	strACQ_VOL_KEY = pDM_WORK->GetCellData(_T("SA_���Ա��к���KEY"),nIdx_WORK);
	if(strVOL_KEY.IsEmpty() || strACQ_VOL_KEY.IsEmpty()) return -2;
	
	CString strQuery;
	CString strResult = _T("");


	//�ش� ��KEY�� ������ ��� ����, ������ ��� ������ ���Ա��к� �� ������ Ȯ���Ѵ�.
	if(1 == nACQ_TYPE)
	{
		strQuery.Format(_T("SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s AND ACQ_CODE = '2'"),strVOL_KEY);
	}
	else if(2 == nACQ_TYPE)
	{
		strQuery.Format(_T("SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s AND ACQ_CODE = '1'"),strVOL_KEY);
	}

	strResult = _T("");
	pDM_WORK->GetOneValueQuery(strQuery,strResult);
	
	strResult.TrimLeft();
	strResult.TrimRight();

	pDM_WORK->StartFrame();

	if(strResult.IsEmpty()) { //��ȣ ���� ����
		

		CString strToday = GetTodayDate();
		CString strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

		//Ŭ���� �̷� 
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY IN (SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
		pDM_WORK->AddFrame(strQuery);

		//å (�η� ����)
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);
		pDM_WORK->AddFrame(strQuery);
		

		//�η�å 
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_APPENDIX_BOOK_TBL WHERE BOOK_KEY IN ")
					    _T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
		pDM_WORK->AddFrame(strQuery);


		//���Ա��к��� 
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);
		pDM_WORK->AddFrame(strQuery);
	

		//������ȣ�� �Է�
		//===================================================
		//2009.07.06 UPDATE BY PJW : ������ UDF_MANAGE_CODE�߰���
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("INSERT INTO SE_DELETE_VOL_TBL ")
					_T("(SELECT REC_KEY,SPECIES_KEY,VOL_TYPE,TITLE,FORM_CODE,MEDIA_CODE,VOL_SORT_NO,")
					_T("VOL_TITLE,OTHER_VOL_TITLE,ALTERNATIVE_VOL_TITLE,IDX_VOL_TITLE,PUBLISH_DATE,")
					_T("CURRENCY_CODE,PRICE,PRICE_CHARACTER,PAGE,PHYSICAL_PROPERTY,BOOK_SIZE,ACCOMP_MAT,")
					_T("REMARK,VOLUME_CLASS,CONTENTS_YN,CONTENTS_KEY,'%s',FIRST_WORK,'%s',UDF_MANAGE_CODE ")
					_T("FROM SE_VOL_TBL WHERE REC_KEY = %s);"),strToday,strWorkLog,strVOL_KEY);
		pDM_WORK->AddFrame(strQuery);
		//===================================================


		//�� 
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
		pDM_WORK->AddFrame(strQuery);
		

		//���,����,�ʷϰ��� ����
		GetArticleDeleteQuery(pDM_WORK,strVOL_KEY);


		//���α�ȣ��
		MakeEcoIndexVol(pDM_WORK,strVOL_KEY,FALSE);

	} 
	else //���Ա��к��� ������ ���� 
	{
		
		//Ŭ���� �̷� 
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY = %s;"),strACQ_VOL_KEY);
		pDM_WORK->AddFrame(strQuery);
	
		//å (�η��� �ƴ� å�� ����)
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE ACQ_TYPE_VOL_KEY = %s AND BOOK_TYPE != 'A';"),strACQ_VOL_KEY);
		pDM_WORK->AddFrame(strQuery);


		//���Ա��к��� 
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s;"),strACQ_VOL_KEY);
		pDM_WORK->AddFrame(strQuery);

	}
	
	
	//ISS_��KEY
	CString strSPECIES_KEY;
	strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_��KEY"),nIdx_WORK);

	//���� UPDATE
	APPLY_IndexItem_Book(pDM_WORK, strSPECIES_KEY);

	INT ids = pDM_WORK->SendFrame();
	pDM_WORK->EndFrame();
	
	if(ids<0) return -1003; //���� ���� 

	return 0;
}


INT CSeApi::VOL_CAT_DELETE(CESL_DataMgr* pDM_OUT,
						   CString strSPECIES_KEY,
						   CString strVOL_KEY,
						   CString strVOL_TYPE,
						   CString strVolTitle,
						   CStringArray& arrayChildBookKeys,
						   BOOL bOnlyAddFrame,
						   BOOL bOnlyVerify)
{

	//�������� ����ϴ� �ǻ��� �Լ� 
	//�� Ÿ���� �Ǵ��� ������ ���� �Լ��� �̿��Ѵ�
	
	if(!pDM_OUT || strSPECIES_KEY.IsEmpty() || strVOL_KEY.IsEmpty() || strVOL_TYPE.IsEmpty() || strVolTitle.IsEmpty())
	{
		return -1;
	}
	
	INT ids = -1;

	if(_T("B") == strVOL_TYPE && 0 == arrayChildBookKeys.GetSize())
	{
		AfxMessageBox(_T("������ȣ�� ���� åŰ ������ �Էµ��� �ʾҽ��ϴ�!")); 
		return -1002; //verifying fail
	}


	if(!bOnlyAddFrame) pDM_OUT->StartFrame();

	if(_T("V") == strVOL_TYPE) //�Ϲ�
	{


		ids = VOL_CAT_DELETE_GENERAL(pDM_OUT,
									 strSPECIES_KEY,
									 strVOL_KEY,
									 strVolTitle,
									 bOnlyVerify);
								
	}
	else if(_T("B") == strVOL_TYPE) //���� 
	{


		ids = VOL_CAT_DELETE_BINDING(pDM_OUT,
									 strSPECIES_KEY,
									 strVOL_KEY,
									 strVolTitle,
									 arrayChildBookKeys,
									 bOnlyVerify);

	}

	
	if(ids<0) {
		if(!bOnlyAddFrame) pDM_OUT->EndFrame();
		return -1002; //verifying fail
	}

	if(bOnlyAddFrame)
	{
		return 0;
	}

	ids = pDM_OUT->SendFrame();
	pDM_OUT->EndFrame();

	if(ids<0) return -1003; //Transaction error 

	return 0;

}


INT CSeApi::VOL_CAT_DELETE_GENERAL(CESL_DataMgr* pDM_VOL,
								   CString strSPECIES_KEY,
								   CString strVOL_KEY,
						           CString strVolTitle,
							       BOOL bOnlyVerify)
{
	//�������� ����� �� ���� �Լ��� ���� �Լ�
	//�Ϲ� ��ȣ�� ������ �� ���
	//���� ���п� ���� ���� �ش� ��ȣ�� ������ ��� ������ �����Ѵ�
	//�ش� ��ȣ�� ���� å���� ���¸� ���� �����Ѵ�

	if(!pDM_VOL) return -1;

		
	CString strQuery;
	CString msg;
	INT nCount = 0;
	CString strCount;

	///////////////////////////////////////////////////////////////
	//�ش� �ǳ��� å�߿� ������ �Ǿ� �ִ� å�� �ִ� ���� Ȯ���Ѵ�//
	///////////////////////////////////////////////////////////////

	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND BINDED_BOOK_YN = 'Y'")
					,strVOL_KEY);

	pDM_VOL->GetOneValueQuery(strQuery,strCount);

	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			msg.Format(_T("[ %s ] ��ȣ ���� �����Ǿ� �ִ� å�� �ֽ��ϴ�. \n �ش� ����å�� ���� ������ �ֽ� ��\n")
					   _T("Ȯ���Ͻð� �ٽ� ������ �ֽʽÿ�!"),strVolTitle);
		
			AfxMessageBox(msg);
			return -1002; //verifying fail
		}

	}

	///////////////////////////////////////////////////////////
	//�ش� �ǳ��� å ����, ��������, �Ϲ����� ���¸� Ȯ���Ѵ�//
	///////////////////////////////////////////////////////////

	//����
	strQuery.Format(_T("SELECT COUNT(*) FROM LH_STORAGE_V01 WHERE STATUS IN ('0','2','3','5') ")
					_T("AND BOOK_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s)")
					,strVOL_KEY);

	pDM_VOL->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			msg.Format(_T("[ %s ] ��ȣ ���� ���� Ȥ�� �������� å�� �ֽ��ϴ�!\n")
					   _T("Ȯ���Ͻð� �ٽ� ������ �ֽʽÿ�!"),strVolTitle);
		
			AfxMessageBox(msg);
			return -1002; //verifying fail
		}

	}

	//��������
	strQuery.Format(_T("SELECT COUNT(*) FROM CO_REPAIR_BINDING_TBL WHERE STATUS IN ('1','2') ")
					_T("AND BOOK_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s)")
					,strVOL_KEY);

	pDM_VOL->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			msg.Format(_T("[ %s ] ��ȣ ���� ���������� ��� �Ǵ� �Ƿ��� å�� �ֽ��ϴ�!\n")
					   _T("Ȯ���Ͻð� �ٽ� ������ �ֽʽÿ�!"),strVolTitle);
			AfxMessageBox(msg);
			return -1002; //verifying fail
		}

	}

	//�Ϲ� ����
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE WORKING_STATUS IN ")
					_T("('SEB112N','SEB113N') AND VOL_KEY = %s"),strVOL_KEY);
				
	pDM_VOL->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			msg.Format(_T("[ %s ] ��ȣ ���� ���� ������ ���� Ȥ�� ���� ������ å�� �ֽ��ϴ�!\n")
					   _T("Ȯ���Ͻð� �ٽ� ������ �ֽʽÿ�!"),strVolTitle);
			AfxMessageBox(msg);
			return -1002; //verifying fail
		}

	}

	//////////////////////////////////////////////////////////
	//Ʈ������� �ƴ� ��쿡�� ���� ���ɿ� ���� ������ �Ѵ�.//
	//////////////////////////////////////////////////////////
	if(bOnlyVerify) return 0;



	CString strWorkLog,strToday;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	strToday = GetTodayDate();
	


	//////////////////////////////////////////////////////////////
	//å�������� ����(å,�������,������Ϲ�ȣ ó��,�η�,����å)//
	//////////////////////////////////////////////////////////////
	

	//���� ���� ����
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_LOAN_MAIL_SEND_TBL WHERE LOAN_KEY IN ")
					_T("(SELECT REC_KEY FROM LH_STORAGE_V01 WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s));"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);

   
	//���� ���� 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE LS_WORK_T01 WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);
	

	//��� �ڷ� ���� 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_DELIVERY_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);

		
	//���� ���� ���� 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_REPAIR_BINDING_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);


	//�η����� 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_APPENDIX_BOOK_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);	
	pDM_VOL->AddFrame(strQuery);
	

	//����å 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_BINDING_BOOK_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);

	

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//���� ��� ��ȣ ó�� 
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	pDM_VOL->InitDBFieldData();
// 	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
// 					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,2),SUBSTR(REG_NO,3,10),'%s','%s',UDF_MANAGE_CODE")
// 					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
// 					_T("AND REC_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s));")
// 					,DEFAULT_SEQ_NAME,strWorkLog,strWorkLog,strVOL_KEY);
// 	pDM_VOL->AddFrame(strQuery);

	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,%d),SUBSTR(REG_NO,%d,%d),'%s','%s',UDF_MANAGE_CODE")
					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
					_T("AND REC_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s));")
					,DEFAULT_SEQ_NAME,nRegCodeLength,nRegCodeLength+1, 12-nRegCodeLength,strWorkLog,strWorkLog,strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);
	//=====================================================
	
					
	

	//å���� ����
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_BOOK_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);	
	pDM_VOL->AddFrame(strQuery);



	///////////////////////////////////////////////////
	//���� ���к� ��ȣ ����(Ŭ�����̷�,���Ա��к� ��)//
	///////////////////////////////////////////////////

	//Ŭ���� �̷� ���� 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);
		

	//���Ա��к� �� ���� 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);

	///////////////////////////////////////////////////////////////////
	//��ȣ ���� ����(��ȣ,�������Է�,���,������,�����ʷ�,��ȣ����)//
	///////////////////////////////////////////////////////////////////
	

	//������ȣ�� �Է�
	//===================================================
	//2009.07.06 UPDATE BY PJW : ������ UDF_MANAGE_CODE�߰���
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO SE_DELETE_VOL_TBL ")
					_T("(SELECT REC_KEY,SPECIES_KEY,VOL_TYPE,TITLE,FORM_CODE,MEDIA_CODE,VOL_SORT_NO,")
					_T("VOL_TITLE,OTHER_VOL_TITLE,ALTERNATIVE_VOL_TITLE,IDX_VOL_TITLE,PUBLISH_DATE,")
					_T("CURRENCY_CODE,PRICE,PRICE_CHARACTER,PAGE,PHYSICAL_PROPERTY,BOOK_SIZE,ACCOMP_MAT,")
					_T("REMARK,VOLUME_CLASS,CONTENTS_YN,CONTENTS_KEY,'%s',FIRST_WORK,'%s',UDF_MANAGE_CODE ")
					_T("FROM SE_VOL_TBL WHERE REC_KEY = %s);"),strToday,strWorkLog,strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);
	//===================================================
	

	//���� ��ȣ�� setting
	MakeEcoIndexVol(pDM_VOL,strVOL_KEY,FALSE);


	//���,����,�ʷϰ��� ����
	GetArticleDeleteQuery(pDM_VOL,strVOL_KEY);
	

	//��ȣ ����
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);

	
	//���� UPDATE
	APPLY_IndexItem_Book(pDM_VOL, strSPECIES_KEY);
	

	return 0;
}

INT CSeApi::VOL_CAT_DELETE_BINDING(CESL_DataMgr* pDM_VOL,
								   CString strSPECIES_KEY,
								   CString strVOL_KEY,
						           CString strVolTitle,
								   CStringArray& arrayChildBookKeys,
							       BOOL bOnlyVerify)
{

	//�������� ����� �� ���� �Լ��� ���� �Լ�
	//���� ��ȣ�� ������ �� ���
	//�ش� ��ȣ�� ���� å���� ���¸� ���� �����Ѵ�

	if(!pDM_VOL) return -1;

		
	CString strQuery;
	CString msg;
	INT nCount = 0;
	CString strCount;

	///////////////////////////////////////////////////////////
	//�ش� �ǳ��� å ����, ��������, �Ϲ����� ���¸� Ȯ���Ѵ�//
	///////////////////////////////////////////////////////////

	//����
	strQuery.Format(_T("SELECT COUNT(*) FROM LH_STORAGE_V01 WHERE STATUS IN ('0','2','3','5') ")
					_T("AND BOOK_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s)")
					,strVOL_KEY);

	pDM_VOL->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			msg.Format(_T("[ %s ] ��ȣ ���� ���� Ȥ�� �������� å�� �ֽ��ϴ�!\n")
					   _T("Ȯ���Ͻð� �ٽ� ������ �ֽʽÿ�!"),strVolTitle);
		
			AfxMessageBox(msg);
			return -1002; //verifying fail
		}

	}

	//��������
	strQuery.Format(_T("SELECT COUNT(*) FROM CO_REPAIR_BINDING_TBL WHERE STATUS IN ('1','2') ")
					_T("AND BOOK_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s)")
					,strVOL_KEY);

	pDM_VOL->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			msg.Format(_T("[ %s ] ��ȣ ���� ���������� ��� �Ǵ� �Ƿ��� å�� �ֽ��ϴ�!\n")
					   _T("Ȯ���Ͻð� �ٽ� ������ �ֽʽÿ�!"),strVolTitle);
			AfxMessageBox(msg);
			return -1002; //verifying fail
		}

	}

	
	
	CString strWorkLog,strToday;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	strToday = GetTodayDate();

	INT ids = -1;


	/////////////////////////////////////////////////////////
	//���� å �������� ����(����å�� �����ִ� å �� ����å)//
	/////////////////////////////////////////////////////////
	
	CString strBOOK_KEY;
	INT nSize = arrayChildBookKeys.GetSize();

	for(INT i=0;i<nSize;i++)
	{
		strBOOK_KEY = arrayChildBookKeys.GetAt(i);
		if(strBOOK_KEY.IsEmpty()) continue;

		ids = BOOK_CAT_DELETE_BINDING(pDM_VOL,
									  strBOOK_KEY,
								      strVOL_KEY,
								      strSPECIES_KEY,
									  strVolTitle,
									  _T("V"),
								      FALSE,
									  bOnlyVerify);
		
		if(ids<0) 
		{
			return -1002; //verifying error
		}


	}


	//////////////////////////////////////////////////////////
	//Ʈ������� �ƴ� ��쿡�� ���� ���ɿ� ���� ������ �Ѵ�.//
	//////////////////////////////////////////////////////////
	if(bOnlyVerify) return 0;



	///////////////////////////////////////////////////////////////////
	//��ȣ ���� ����(��ȣ,�������Է�,���,������,�����ʷ�,��ȣ����)//
	///////////////////////////////////////////////////////////////////
	

	//������ȣ�� �Է�
	pDM_VOL->InitDBFieldData();
	//===================================================
	//2009.07.06 UPDATE BY PJW : ������ UDF_MANAGE_CODE�߰���
	strQuery.Format(_T("INSERT INTO SE_DELETE_VOL_TBL ")
					_T("(SELECT REC_KEY,SPECIES_KEY,VOL_TYPE,TITLE,FORM_CODE,MEDIA_CODE,VOL_SORT_NO,")
					_T("VOL_TITLE,OTHER_VOL_TITLE,ALTERNATIVE_VOL_TITLE,IDX_VOL_TITLE,PUBLISH_DATE,")
					_T("CURRENCY_CODE,PRICE,PRICE_CHARACTER,PAGE,PHYSICAL_PROPERTY,BOOK_SIZE,ACCOMP_MAT,")
					_T("REMARK,VOLUME_CLASS,CONTENTS_YN,CONTENTS_KEY,'%s',FIRST_WORK,'%s',UDF_MANAGE_CODE ")
					_T("FROM SE_VOL_TBL WHERE REC_KEY = %s);"),strToday,strWorkLog,strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);
	//===================================================
	

	//���� ��ȣ�� setting
	MakeEcoIndexVol(pDM_VOL,strVOL_KEY,FALSE);


	//��ȣ ����
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);

	
	//���� UPDATE
	APPLY_IndexItem_Book(pDM_VOL, strSPECIES_KEY);
	

	return 0;
}

INT CSeApi::VOL_DELETE_EXP(CESL_DataMgr* pDM_WORK,
						   INT nIdx_WORK,
					       CESL_DataMgr* pDM_ACQ,
					       INT nACQ_TYPE,
					       CArray<CString,CString>& arrayVolExpKeyList,
					       CArray<CString,CString>& arrayOtherVolExpKeyList)
{
	//���� ��ȣ�� ���� �����Լ��� �������������� update���Ѵ�. 
	
	if(!pDM_WORK || !pDM_ACQ) {
		AfxMessageBox(_T("���� ���� ���� ���� DM�� ã�� �� �����ϴ�!"));
		return -1;
	}

	pDM_WORK->StartFrame();
	
	INT ids;
	ids = UpdateExpectData(pDM_WORK,
						   pDM_ACQ,
					       nACQ_TYPE,
					       pDM_WORK,
					       nIdx_WORK, 
					       arrayVolExpKeyList,
					       arrayOtherVolExpKeyList);

	if(ids<0) return -1001;
	
	ids = pDM_WORK->SendFrame();
	pDM_WORK->EndFrame();
	if(ids<0) return -1002;
	


	return 0;
}

INT CSeApi::VOL_UPDATE_NEW_BOOK(CESL_DataMgr* pDM_ACQ,
								INT nACQ_TYPE, 
								CESL_DataMgr* pDM_WORK,
								INT nIdx_WORK, 
								CESL_DataMgr* pDM_BOOK,
								BOOL bIsDepartMngUse) 
{
	
	//��ȣ ���� �Լ����� ����ϴ� �Լ��� ���ο� å�� �ԷµǾ��� ��� 
	//�⺻ ������ setting�Ѵ�
	
	
	//��KEY
	CString strSPECIES_KEY = _T("");
	strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_��KEY"),nIdx_WORK);
	

	//��KEY
	CString strVOL_KEY = _T("");	
	strVOL_KEY = pDM_WORK->GetCellData(_T("SV_��KEY"),nIdx_WORK);


	//���Ա��к���KEY
	CString strACQ_VOL_KEY = _T("");	
	strACQ_VOL_KEY = pDM_WORK->GetCellData(_T("SA_���Ա��к���KEY"),nIdx_WORK);	
	

	//��ϱ���
	CString strREG_CODE = _T("");
	strREG_CODE = pDM_WORK->GetCellData(_T("SS_��ϱ���_�ڵ�"),nIdx_WORK);

	//�̿����ѱ���
	CString strUSE_LIMIT_CODE = _T("");
	strUSE_LIMIT_CODE = pDM_WORK->GetCellData(_T("SS_�̿����ѱ���_�ڵ�"),nIdx_WORK);
	
	//��������
	CString strMANAGE_CODE = _T("");
	strMANAGE_CODE = pDM_WORK->GetCellData(_T("SS_��������_�ڵ�"),nIdx_WORK);
	
	//�谡��ġ�ڵ�
	CString strSHELF_LOC_CODE = _T("");
	strSHELF_LOC_CODE = pDM_WORK->GetCellData(_T("SS_�谡��ġ_�ڵ�"),nIdx_WORK);
	
	//�з���ȣ����
	CString strCLASS_NO_TYPE = _T("");
	strCLASS_NO_TYPE = pDM_WORK->GetCellData(_T("SS_�з���ȣ����_�ڵ�"),nIdx_WORK);
	
	//���Ա��� 
	CString strACQ_TYPE;
	strACQ_TYPE.Format(_T("%d"),nACQ_TYPE);
	
	//�Է��� 
	CString strINPUT_DATE;
	strINPUT_DATE = GetTodayDate();

	//�����۾�, ������ �۾�
	CString strWorkLog = _T("");
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	CString strREC_KEY;
	
	//�ΰ� ���� ����
	CString strQuery;
	CString strResult;
	CString strTRANSFER_YN;
	//++2008.10.17 UPDATE BY PWR {{++
	// ���սý��� ����
	strQuery =	_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
				_T("WHERE GROUP_1='L' AND GROUP_2='����' AND CLASS_ALIAS='�ǽð�����ΰ��뿩��' ")
				_T("AND MANAGE_CODE=UDF_MANAGE_CODE");
	// strQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '����' AND CLASS_ALIAS='�ǽð�����ΰ��뿩��'");
	//--2008.10.17 UPDATE BY PWR --}}
	pDM_BOOK->GetOneValueQuery(strQuery,strResult);
	
	if(_T("Y") == strResult)
		strTRANSFER_YN = _T("N");
	else
		strTRANSFER_YN = _T("Y");


	for(INT i=0;i<pDM_BOOK->GetRowCount();i++)
	{
		pDM_BOOK->MakeRecKey(strREC_KEY);		
		pDM_BOOK->SetCellData(_T("SB_åKEY"),strREC_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_��KEY"),strSPECIES_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_��KEY"),strVOL_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_���Ա��к���KEY"),strACQ_VOL_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_���Ա���"),strACQ_TYPE,i);
		pDM_BOOK->SetCellData(_T("SB_��ϱ���_�ڵ�"),strREG_CODE,i);
		pDM_BOOK->SetCellData(_T("SB_�̿����ѱ���_�ڵ�"),strUSE_LIMIT_CODE,i);
		pDM_BOOK->SetCellData(_T("SB_��������"),strMANAGE_CODE,i);
		
		if(!bIsDepartMngUse)
			pDM_BOOK->SetCellData(_T("SB_�谡��ġ_�ڵ�"),strSHELF_LOC_CODE,i);
		
		pDM_BOOK->SetCellData(_T("SB_�з���ȣ����_�ڵ�"),strCLASS_NO_TYPE,i);
		pDM_BOOK->SetCellData(_T("SB_�Է���"),strINPUT_DATE,i);
		pDM_BOOK->SetCellData(_T("SB_�����۾�"),strWorkLog,i);
		pDM_BOOK->SetCellData(_T("SB_�������۾�"),strWorkLog,i);
		pDM_BOOK->SetCellData(_T("SB_����å����"),_T("N"),i); //default
		pDM_BOOK->SetCellData(_T("SB_���̺���±���"),_T("1"),i); //default
		pDM_BOOK->SetCellData(_T("SB_�弭����"),_T("N"),i); //default
		pDM_BOOK->SetCellData(_T("SB_�ΰ迩��"),strTRANSFER_YN,i); //manage value
	}

	return 0;
}

INT CSeApi::VOL_UPDATE_SET_BOOK_STATUS(CESL_DataMgr* pDM_WORK,
									   INT nIdx_WORK,
									   CESL_DataMgr* pDM_BOOK,
									   CESL_DataMgr* pDM_BOOK_INSERT,
									   INT nACQ_TYPE,
									   CString strACQ_YEAR,
									   CString strRECEIPT_NO,
									   BOOL bIsDepartMngUse,
					                   CESL_DataMgr* pDM_DEPART)
{
	
	//�Ǽ����ÿ� å�� ���°��� �������ִ� �Լ��̴�. 
	//BOOK_TYPE  C:�Ϲ�  M:��ȣ  L:����
	INT nDB_ACQ_BOOK_CNT = 0;
	INT nDB_MISS_BOOK_CNT = 0;
	CString strBOOK_TYPE;
	
	//DBå�� 
	INT nBookRowCnt = pDM_BOOK->GetRowCount();
	for(INT i=0;i<nBookRowCnt;i++)
	{
		strBOOK_TYPE = _T("");
		strBOOK_TYPE = pDM_BOOK->GetCellData(_T("SB_å����"),i);
		if(_T("C") == strBOOK_TYPE) nDB_ACQ_BOOK_CNT++;
		if(_T("M") == strBOOK_TYPE || _T("L") == strBOOK_TYPE ) nDB_MISS_BOOK_CNT++;	
		pDM_BOOK->SetCellData(_T("UDF_�����켱����"),_T("Z"),i); //�ϴ� ���� �켱 ������ Z �� �ο�(�����Ұ�) 
	}
	
	//�۾�å��.
	CString strACQ_BOOK_CNT = _T("");
	CString strMISS_BOOK_CNT = _T("");
	INT nACQ_BOOK_CNT;
	INT nMISS_BOOK_CNT;
	strACQ_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_�Լ�å��"),nIdx_WORK);
	strMISS_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_��ȣå��"),nIdx_WORK);
	nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
	nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));	
	
	//å�� ���� 
	INT nAcqBookGap = nACQ_BOOK_CNT - nDB_ACQ_BOOK_CNT;
	INT nMissBookGap = nMISS_BOOK_CNT - nDB_MISS_BOOK_CNT;
	

	//å���� �ڷ���¿� ������, ���������� ���� �� �߰��ؾ��� �켱 ������ 
	//���� �켱 ����(DM_üũ��_å.UDF_�����켱����)�� ���س�� 
	CString strREAD_FIRST_YN;
	CString strWORKING_STATUS;
	strREAD_FIRST_YN = pDM_WORK->GetCellData(_T("UDF_�����켱����"),nIdx_WORK);

	for(i=0;i<nBookRowCnt;i++)
	{
		strWORKING_STATUS = pDM_BOOK->GetCellData(_T("SB_�ڷ����"),i);
			
		if(_T("SEL111O") == strWORKING_STATUS) //�ǽð� ��� 
			pDM_BOOK->SetCellData(_T("UDF_�����켱����"),_T("A"),i); //�켱 ������ A �� �ο� 
		else if(_T("SEB111N") == strWORKING_STATUS) //���� ��� 
			pDM_BOOK->SetCellData(_T("UDF_�����켱����"),_T("B"),i); //�켱 ������ B �� �ο� 
		else if(_T("SER111N") == strWORKING_STATUS) //��� ��� 
			pDM_BOOK->SetCellData(_T("UDF_�����켱����"),_T("C"),i); //�켱 ������ C �� �ο� 
	}

	INT nRow;
	
	
	if(nAcqBookGap<0) //////////////////////////////// 1) �Լ�å���� ���� ��� 
	{
		if(nMissBookGap>0) //��ȣå�� �þ����� 
		{
			//��ȣ å�� ���� ��ŭ ���� �Լ�å�� ��ȣå���� �����. 
			for(i = 0;i<nMissBookGap;i++) 
			{
				nRow = GetDeleteHighestPriority(pDM_BOOK);
				MoveBookToMissBook(pDM_BOOK,nACQ_TYPE,nRow); //(�Լ�,�߰�->��ȣ) 
				pDM_BOOK->SetCellData(_T("UDF_����"),_T("U"),nRow); // update 
				nAcqBookGap++; 
				nMissBookGap--;
				if(0 == nAcqBookGap || 0==nMissBookGap) break; 
			}

			if(nAcqBookGap<0) //��ȣå���� �ű�� ���� å���� �������� ���� ��ŭ ����  
			{
				while(1)
				{
					nRow = GetDeleteHighestPriority(pDM_BOOK);
					pDM_BOOK->SetCellData(_T("UDF_����"),_T("D"),nRow); // delete 
					nAcqBookGap++;
					if(0 == nAcqBookGap) break;
				}

			}

			if(nMissBookGap>0) //���� ��ȣå�� �� ������ �Էµ� å�� ��ȣå���� �����
			{
				for(i = 0;i<pDM_BOOK_INSERT->GetRowCount();i++) 
				{
					MoveBookToMissBook(pDM_BOOK_INSERT,nACQ_TYPE,i); //(�Լ�,�߰�->��ȣ)
				}
			}
				
			
		}
		else if(nMissBookGap<0) //��ȣå�� �پ����� 
		{
			while(1) //�Լ�å���� 
			{
				CString strDeleteKey;
				nRow = GetDeleteHighestPriority(pDM_BOOK);
				pDM_BOOK->SetCellData(_T("UDF_����"),_T("D"),nRow); // delete 
				nAcqBookGap++;
				if(0 == nAcqBookGap) break;
			}
			

			for(INT i=0;i<nBookRowCnt;i++) //��ȣå ���� 
			{
				strBOOK_TYPE = _T("");
				strBOOK_TYPE = pDM_BOOK->GetCellData(_T("SB_å����"),i);
				if(_T("M") == strBOOK_TYPE || _T("L") == strBOOK_TYPE) {
					pDM_BOOK->SetCellData(_T("UDF_����"),_T("D"),nRow); // delete 
					nMissBookGap++;
				}
				if(0 == nMissBookGap) break;
			}

		}
		else //��ȣå�� ������ �ʾ����� 
		{
			while(1) //�Լ�å���� 
			{
				nRow = GetDeleteHighestPriority(pDM_BOOK);
				pDM_BOOK->SetCellData(_T("UDF_����"),_T("D"),nRow); // delete 
				nAcqBookGap++;
				if(0 == nAcqBookGap) break;
			}
		}
	}
	else if(nAcqBookGap>0) //////////////////////////////// 2) �Լ�å���� �þ ��� 
	{
		
		if(nMissBookGap>0) //��ȣå�� �þ����� 
		{
			
			for(i=0;i<pDM_BOOK_INSERT->GetRowCount();i++)
			{
				
				if(nAcqBookGap>0)
				{
					//�Էµ� å���� �Լ� å���� �þ ��ŭ �Լ�å���� ������ش� 
					MoveBookToAcqBook(pDM_WORK,
									  nIdx_WORK,
									  pDM_BOOK,
									  pDM_BOOK_INSERT,
									  pDM_BOOK_INSERT,
									  nACQ_TYPE,
									  strACQ_YEAR,
									  strRECEIPT_NO,
									  i,
									  bIsDepartMngUse,
									  pDM_DEPART); //(��ȣ,�߰� -> �Լ�)
					nAcqBookGap--;
				}
				else
				{
					//�Էµ� å���� ��ȣ å���� �þ ��ŭ ��ȣå���� ������ش� 
					MoveBookToMissBook(pDM_BOOK_INSERT,nACQ_TYPE,i); //(�Լ�,�߰� -> ��ȣ)
				}
			}
				
			
		}
		else if(nMissBookGap<0) //��ȣå�� �پ����� 
		{
			
			//��ȣå�� ���� ��ŭ ��ȣå�� �Լ�å���� �ű��. 
			for(i=0;i<nBookRowCnt;i++)
			{
				strBOOK_TYPE = _T("");
				strBOOK_TYPE = pDM_BOOK->GetCellData(_T("SB_å����"),i);
				if(_T("M") == strBOOK_TYPE || _T("L") == strBOOK_TYPE)
				{
					MoveBookToAcqBook(pDM_WORK,
									  nIdx_WORK,
									  pDM_BOOK,
									  pDM_BOOK_INSERT,
									  pDM_BOOK,
									  nACQ_TYPE,
									  strACQ_YEAR,
									  strRECEIPT_NO,
									  i,
									  bIsDepartMngUse,
									  pDM_DEPART); //(��ȣ,�߰� -> �Լ�)

					pDM_BOOK->SetCellData(_T("UDF_����"),_T("U"),i); // update 
					nMissBookGap++;
					nAcqBookGap--;
					if(0 == nMissBookGap || 0 == nAcqBookGap) break;					
				}
			}
			
			//��ȣ å�� ��� �ٿ����� �ʾ����� ���� ��ŭ ����  
			if(nMissBookGap<0)
			{

				for(i=0;i<nBookRowCnt;i++)
				{
					strBOOK_TYPE = _T("");
					strBOOK_TYPE = pDM_BOOK->GetCellData(_T("SB_å����"),i);
					if(_T("M") == strBOOK_TYPE || _T("L") == strBOOK_TYPE) {
						pDM_BOOK->SetCellData(_T("UDF_����"),_T("D"),i); // delete 
						nMissBookGap++;
					}
					if(0 == nMissBookGap) break;
				}
			}

			//�߰��� �Է� �� å�� ��� �Լ� å���� ����� 
			for(i=0;i<pDM_BOOK_INSERT->GetRowCount();i++)
			{
				
					MoveBookToAcqBook(pDM_WORK,
									  nIdx_WORK,
									  pDM_BOOK,
									  pDM_BOOK_INSERT,
									  pDM_BOOK_INSERT,
									  nACQ_TYPE,
									  strACQ_YEAR,
									  strRECEIPT_NO,
									  i,
									  bIsDepartMngUse,
									  pDM_DEPART); //(��ȣ,�߰� -> �Լ�)
			}
			
		}
		else //��ȣå�� ������ �ʾ����� 
		{
			
			//�߰��� �Է� �� å�� ��� �Լ� å���� ����� 
			for(i=0;i<pDM_BOOK_INSERT->GetRowCount();i++)
			{
				
					MoveBookToAcqBook(pDM_WORK,
									  nIdx_WORK,
									  pDM_BOOK,
									  pDM_BOOK_INSERT,
									  pDM_BOOK_INSERT,
									  nACQ_TYPE,
									  strACQ_YEAR,
									  strRECEIPT_NO,
									  i,
									  bIsDepartMngUse,
									  pDM_DEPART); //(��ȣ,�߰� -> �Լ�)
			}
		}

	} 
	else if(0 == nAcqBookGap) //////////////////////////////// 3) �Լ�å���� ���� ���� ���  
	{
	
		if(nMissBookGap>0) //��ȣå�� �þ����� 
		{
			//�߰��� �Է� �� å�� ��� ��ȣ å���� ����� 
			for(i=0;i<pDM_BOOK_INSERT->GetRowCount();i++)
			{
				MoveBookToMissBook(pDM_BOOK_INSERT,nACQ_TYPE,i); //(�Լ�,�߰�->��ȣ)
			}
			
		}
		else if(nMissBookGap<0) //��ȣå�� �پ����� 
		{
			
			//��ȣå�� �پ�� ��ŭ  ��ŭ ����  
			for(i=0;i<nBookRowCnt;i++)
			{
				strBOOK_TYPE = _T("");
				strBOOK_TYPE = pDM_BOOK->GetCellData(_T("SB_å����"),i);
				if(_T("M") == strBOOK_TYPE || _T("L") == strBOOK_TYPE) {
					pDM_BOOK->SetCellData(_T("UDF_����"),_T("D"),i); // delete 
					nMissBookGap++;
				}
				if(0 == nMissBookGap) break;
			}
		}

	}

	
	//å DM �� �����Ѵ� 
	INT nRowIdx;
	for(i=0;i<nBookRowCnt;i++)
	{
		pDM_BOOK_INSERT->InsertRow(-1);
		nRowIdx = pDM_BOOK_INSERT->GetRowCount()-1;
		CopyDMToDMByAlias(pDM_BOOK,i,pDM_BOOK_INSERT,nRowIdx);
	}


	return 0;
}


INT CSeApi::BOOK_DELETE_VERIFY(CESL_DataMgr* pDM_OUT,
							   CString strBOOK_KEY,
							   CString strVOL_KEY,
							   CString strBOOK_TYPE,
							   CString strBooktitle)
{



	//����
	CString strQuery;
	CString msg;
	INT nCount = 0;
	CString strCount;


	strQuery.Format(_T("SELECT COUNT(*) FROM LH_STORAGE_V01 WHERE STATUS IN ('0','2','3','5') ")
					_T("AND BOOK_KEY = %s"),strBOOK_KEY);

	pDM_OUT->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			if(_T("A") == strBOOK_TYPE)
			{
				msg.Format(_T("[ %s ]  �η��� ���� Ȥ�� �������Դϴ�!\n")
						   _T("Ȯ���Ͻð� �ٽ� ������ �ֽʽÿ�!"),strBooktitle);
			}
			else
			{
				msg.Format(_T("[ %s ]  å�� ���� Ȥ�� �������Դϴ�!\n")
						   _T("Ȯ���Ͻð� �ٽ� ������ �ֽʽÿ�!"),strBooktitle);
			}
		
			AfxMessageBox(msg);
			return -1;
		}

	}

	//��������
	strQuery.Format(_T("SELECT COUNT(*) FROM CO_REPAIR_BINDING_TBL WHERE STATUS IN ('1','2') ")
					_T("AND BOOK_KEY = %s"),strBOOK_KEY);

	pDM_OUT->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			
			if(_T("A") == strBOOK_TYPE)
			{
				msg.Format(_T("[ %s ] �η��� ���������� ��� �Ǵ� �Ƿ� �߿� �ֽ��ϴ�!\n")
						   _T("Ȯ���Ͻð� �ٽ� ������ �ֽʽÿ�!"),strBooktitle);
			}
			else
			{
				msg.Format(_T("[ %s ] å�� ���������� ��� �Ǵ� �Ƿ� �߿� �ֽ��ϴ�!\n")
						   _T("Ȯ���Ͻð� �ٽ� ������ �ֽʽÿ�!"),strBooktitle);
			}

			AfxMessageBox(msg);
			return -1;
		}

	}

	//�Ϲ� ����
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE WORKING_STATUS IN ")
					_T("('SEB112N','SEB113N') AND REC_KEY = %s"),strBOOK_KEY);
				
	pDM_OUT->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			if(_T("A") == strBOOK_TYPE)
			{
				msg.Format(_T("[ %s ] �η��� ���� ������ ���� Ȥ�� ���� ���¿� �ֽ��ϴ�!\n")
						   _T("Ȯ���Ͻð� �ٽ� ������ �ֽʽÿ�!"),strBooktitle);
			}
			else
			{
				msg.Format(_T("[ %s ] å�� ���� ������ ���� Ȥ�� ���� ���¿� �ֽ��ϴ�!\n")
						   _T("Ȯ���Ͻð� �ٽ� ������ �ֽʽÿ�!"),strBooktitle);

			}

			AfxMessageBox(msg);
			return -1;
		}

	}


	return 0;
}


INT CSeApi::BOOK_CAT_DELETE(CESL_DataMgr* pDM_OUT,
							CString strBOOK_KEY,
							CString strACQ_TYPE_VOL_KEY,
							CString strVOL_KEY,
							CString strSPECIES_KEY,
							CString strBOOK_TYPE,
							CString strBooktitle,
							BOOL bOnlyAddFrame,
							BOOL bOnlyVerify)
{
	//�������� ����ϴ� å���� �Լ� 
	//å Ÿ���� �Ǵ��� ������ ���� �Լ��� �̿��Ѵ�
	
	if(!pDM_OUT || strBOOK_KEY.IsEmpty() || strVOL_KEY.IsEmpty() || strSPECIES_KEY.IsEmpty() || strBOOK_TYPE.IsEmpty())
	{
		return -1;
	}
	
	INT ids = -1;

	ids = BOOK_DELETE_VERIFY(pDM_OUT,
							 strBOOK_KEY,
							 strVOL_KEY,
							 strBOOK_TYPE,
							 strBooktitle);

	if(ids<0)
	{
		return -1002; //verifying fail
	}


	if(!bOnlyAddFrame) pDM_OUT->StartFrame();


	if(_T("C") == strBOOK_TYPE) //�Ϲ�
	{


		ids = BOOK_CAT_DELETE_GENERAL(pDM_OUT,
									  strBOOK_KEY,
								      strACQ_TYPE_VOL_KEY,
								      strVOL_KEY,
								      strSPECIES_KEY,
									  strBooktitle,
									  _T("C"),
									  bOnlyVerify);
		

	}
	else if(_T("B") == strBOOK_TYPE) //���� 
	{


		ids = BOOK_CAT_DELETE_BINDING(pDM_OUT,
									  strBOOK_KEY,
								      strVOL_KEY,
								      strSPECIES_KEY,
									  strBooktitle,
									  _T("B"),
									  TRUE,
									  bOnlyVerify);
		

	}
	else if(_T("A") == strBOOK_TYPE) //�η�
	{

		ids = BOOK_CAT_DELETE_APPENDIX(pDM_OUT,
									   strBOOK_KEY,
								       strSPECIES_KEY,
									   bOnlyVerify);
	}
	
	if(ids<0) 
	{
		if(!bOnlyAddFrame) 
		{
			pDM_OUT->EndFrame();
		}

		//===================================================
		//2009.05.11 UPDATE BY PJW : �η��� �������� ���� ���� �������Ѵ�.
// 		return -1002; //verifying fail
		if(ids == -1004) 
		{
			return ids; //verifying fail
		}
		else
		{
			return -1002; //verifying fail
		}
		//===================================================
	}

	if(bOnlyAddFrame)
	{
		return 0;
	}


	ids = pDM_OUT->SendFrame();
	pDM_OUT->EndFrame();

	if(ids<0) return -1003; //Transaction error 

	return 0;
}

INT CSeApi::BOOK_CAT_DELETE_BINDING(CESL_DataMgr* pDM_OUT,
								    CString strBOOK_KEY,
								    CString strVOL_KEY,
								    CString strSPECIES_KEY,
									CString strtitle,
									CString strDeleteOption,
									BOOL bVolDelete,
									BOOL bOnlyVerify)
{
	//��,å���� �Լ��� �����Լ� 
	//����å�� �׿� ����� �Ϲ�å�� �����ϴ� ������ Ʈ����� �ȿ� �ִ´�
	

	//����å ���̺��� �����ִ� �ٸ� å���� KEY�� ������ �ͼ� ���� å���� ������Ų��
	CString strQuery;
	CString strBookKeys;

	strQuery.Format(_T("SELECT BOOK_KEYS FROM SE_BINDING_BOOK_TBL ")
					_T("WHERE BOOK_KEY = %s"),strBOOK_KEY);

	
	pDM_OUT->GetOneValueQuery(strQuery,strBookKeys);

	CStringArray arrayBookKeys;
	ParsingKeyString(_T(","),strBookKeys, &arrayBookKeys);

	CString strBindedBookKey;
	CString strBindedBookAcqTypeVolKey;
	CString strBindedBookVolKey;
	CString strResult[2];
	INT ids = -1;

	INT nSize = arrayBookKeys.GetSize();
	CString msg;
	
	if(0 == nSize)
	{
		
		if(_T("V") == strDeleteOption) //��ȣ ���� 
		{
			msg.Format(_T("[ %s ] ��ȣ�� ���� �ִ� ���� å �߿� \n")
						_T("���� �ִ� �Ϲ� å ������ ã�� �� ���� �ڷᰡ �־�\n")
						_T("������ �� �����ϴ�!"),strtitle);
		}
		else if(_T("B") == strDeleteOption) //å ����
		{
			msg.Format(_T("[ %s ] ���� å�� ���� �ִ� �Ϲ� å������ ã�� �� ����\n")
						_T("������ �� �����ϴ�!"),strtitle);
		}

		AfxMessageBox(msg);
		return -1;

	}

	for(INT i=0;i<nSize;i++)
	{
		
		strBindedBookKey = arrayBookKeys.GetAt(i);
		strBindedBookKey.TrimLeft();
		strBindedBookKey.TrimRight();
		if(strBindedBookKey.IsEmpty()) continue;

		strQuery.Format(_T("SELECT ACQ_TYPE_VOL_KEY, VOL_KEY FROM SE_BOOK_TBL ")
						_T("WHERE REC_KEY = %s"),strBindedBookKey);

		pDM_OUT->GetValuesQuery(strQuery,strResult,2);


		strBindedBookAcqTypeVolKey = strResult[0];
		strBindedBookVolKey = strResult[1];
		
		if(!strBindedBookVolKey.IsEmpty())
		{

			ids = BOOK_CAT_DELETE_GENERAL(pDM_OUT,
										  strBindedBookKey,
										  strBindedBookAcqTypeVolKey,
										  strBindedBookVolKey,
										  strSPECIES_KEY,
										  strtitle,
										  strDeleteOption,
										  bOnlyVerify);

			if(ids<0) return -1;
		}
	}
	
	//������ �ϴ� ���� return
	if(bOnlyVerify) return 0;
	

	//�ٸ� ����å�� ������ �ڽ��� ���� ��ȣ ���� �����Ѵ�
	if(bVolDelete)
	{
		strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL ")
						_T("WHERE VOL_KEY = %s"),strVOL_KEY);

		CString strAnotherBookCnt;
		INT nAnotherBookCnt;

		pDM_OUT->GetOneValueQuery(strQuery,strAnotherBookCnt);
		nAnotherBookCnt = _ttoi(strAnotherBookCnt.GetBuffer(0));

		if(0 == nAnotherBookCnt)
		{
			//���� ��ȣ ����
			pDM_OUT->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
			pDM_OUT->AddFrame(strQuery);
		

			//���� ��ȣ�� setting
			MakeEcoIndexVol(pDM_OUT,strVOL_KEY,FALSE);
		}
	}


	//////////////////////////////////////////////////////////////
	//å�������� ����(å,�������,������Ϲ�ȣ ó��,�η�,����å)//
	//////////////////////////////////////////////////////////////
	
	
	//���� ���� ����
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_LOAN_MAIL_SEND_TBL WHERE LOAN_KEY IN ")
					_T("(SELECT REC_KEY FROM LH_STORAGE_V01 WHERE BOOK_KEY = %s);")
					,strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

   
	//���� ���� 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE LS_WORK_T01 WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);
	

	//��� �ڷ� ���� 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_DELIVERY_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

		
	//���� ���� ���� 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_REPAIR_BINDING_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

	//���� å
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_BINDING_BOOK_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

	
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//���� ��� ��ȣ ó�� 
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	CString strWorkLog;
// 	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
// 
// 	pDM_OUT->InitDBFieldData();
// 	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
// 					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,2),SUBSTR(REG_NO,3,10),'%s','%s',UDF_MANAGE_CODE ")
// 					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
// 					_T("AND REC_KEY = %s);")
// 					,DEFAULT_SEQ_NAME,strWorkLog,strWorkLog,strBOOK_KEY);
// 	pDM_OUT->AddFrame(strQuery);

	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,%d),SUBSTR(REG_NO,%d,%d),'%s','%s',UDF_MANAGE_CODE ")
					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
					_T("AND REC_KEY = %s);")
					,DEFAULT_SEQ_NAME,nRegCodeLength,nRegCodeLength+1, 12-nRegCodeLength,strWorkLog,strWorkLog,strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);
	//=====================================================

	//å
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE REC_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

	//���� UPDATE
	APPLY_IndexItem_Book(pDM_OUT, strSPECIES_KEY);

	return 0;
}



INT CSeApi::BOOK_CAT_DELETE_GENERAL(CESL_DataMgr* pDM_OUT,
								    CString strBOOK_KEY,
								    CString strACQ_TYPE_VOL_KEY,
								    CString strVOL_KEY,
								    CString strSPECIES_KEY,
								    CString strtitle,
									CString strDeleteOption,
									BOOL bOnlyVerify)
{

	//��,å���� �Լ��� �����Լ� 
	//�Ϲ�å���� �����ϴ� ������ Ʈ����� �ȿ� �ִ´�
	//�������� �Է��Ͽ� ���Ա��к���KEY(strACQ_TYPE_VOL_KEY)�� ���� ���� ��ȣ�� �����Ѵ�. 
	INT nACQ_BOOK_CNT = 0;
	INT nMISS_BOOK_CNT = 0;

	CString strWorkLog,strToday;
	strToday = GetTodayDate();
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	CString strQuery;

	if(!strACQ_TYPE_VOL_KEY.IsEmpty())
	{
		CString strResult[3];
		strQuery.Format(_T("SELECT ACQ_BOOK_CNT,MISS_BOOK_CNT,CHECKIN_STATUS FROM SE_ACQ_TYPE_VOL_TBL ")
						_T("WHERE REC_KEY = %s"),strACQ_TYPE_VOL_KEY);

		pDM_OUT->GetValuesQuery(strQuery,strResult,3);
		
		nACQ_BOOK_CNT = _ttoi(strResult[0].GetBuffer(0));
		nMISS_BOOK_CNT = _ttoi(strResult[1].GetBuffer(0));
		
		nACQ_BOOK_CNT--;
	}


	if(0 == nACQ_BOOK_CNT && 0 == nMISS_BOOK_CNT) //���Ա��к� �� ������ ���� �ؾ��ϴ� ��Ȳ 
	{
		
		if(!strACQ_TYPE_VOL_KEY.IsEmpty())
		{

			//Ŭ���� �̷� ����
			pDM_OUT->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY = %s;"),strACQ_TYPE_VOL_KEY);
			if(!bOnlyVerify) pDM_OUT->AddFrame(strQuery);
		

			//���Ա��к� ��ȣ ����
			pDM_OUT->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s;"),strACQ_TYPE_VOL_KEY);
			if(!bOnlyVerify) pDM_OUT->AddFrame(strQuery);

		}

		//�� �������� �ؾ� �ϴ� ���� �Ǵ��Ѵ�(�ش� �ǿ� ���� �ִ� �ٸ� å�� �ִ���)
		CString strAnotherBookCnt = _T("");
		INT nAnotherBook = 0;
		strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL ")
						_T("WHERE VOL_KEY = %s AND REC_KEY != %s AND BOOK_TYPE != 'A'")
						,strVOL_KEY,strBOOK_KEY);


		pDM_OUT->GetOneValueQuery(strQuery,strAnotherBookCnt);
		nAnotherBook = _ttoi(strAnotherBookCnt.GetBuffer(0));

		CString strDeleteAppendixYN;
		strDeleteAppendixYN = _T("N");
		if(0 == nAnotherBook) //�� ���� ���� 
		{
			
			//�η��� �ִ��� �Ǵ��� ����ó�� 
			CString strAppendixCnt;
			INT nAppendixCnt;
			strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND BOOK_TYPE = 'A'")
							,strVOL_KEY);

			pDM_OUT->GetOneValueQuery(strQuery,strAppendixCnt);
			nAppendixCnt = _ttoi(strAppendixCnt.GetBuffer(0));

			if(nAppendixCnt>0) //���� �����ϴ� ��Ȳ�ε� �η��� �ǿ� �پ� �ִ� ��� 
			{
				
				CString msg;

				
				if(_T("C") == strDeleteOption) //�Ϲ� å����
				{
					//===================================================
					//2009.05.11 UPDATE BY PJW : �η��� �ִ��ڷ�� ����� ó���ϵ��� �Ѵ�.
//					msg.Format(_T("[ %s ] å�� �ش� ��ȣ�� �η��ڷᰡ �־� ���� �� �� �����ϴ�!\n")
//								_T("�η��ڷḦ ���� �����Ͽ� �ֽʽÿ�."),strtitle);
//					AfxMessageBox(msg);
//					return -1; 
					msg.Format(_T("[ %s ] å�� �ش� ��ȣ�� �ηϵ� �Բ� ���� �˴ϴ�.\n")
								_T("���� �Ͻðڽ��ϱ�?"),strtitle);
					if ( IDYES == AfxMessageBox(msg, MB_YESNO) )
					{
						strDeleteAppendixYN = _T("Y");
					}
					else
					{
						return -1004; 
					}
					//===================================================
					
				}
				else if(_T("V") == strDeleteOption) //���� �� ������ ���� ����
				{
					msg.Format(_T("[ %s ] ������ȣ�� ���� �η��ڷᰡ �ֽ��ϴ�.\n")
								_T("üũ�ο��� �η��ڷḦ ���� �����Ͻ� �� �� �ڷḦ �����Ͻʽÿ�"),strtitle);
					AfxMessageBox(msg);
					return -1; 
				}
				else if(_T("B") == strDeleteOption) //���� å ������ ���� ����
				{
					msg.Format(_T("[ %s ] ����å�� ���� �η��ڷᰡ �ֽ��ϴ�.\n")
								_T("üũ�ο��� �η��ڷḦ ���� �����Ͻ� �� �� �ڷḦ �����Ͻʽÿ�"),strtitle);
					AfxMessageBox(msg);
					return -1; 
				}

				
			}

			///////////////////////////////////////////////////////////////////
			//��ȣ ���� ����(��ȣ,�������Է�,���,������,�����ʷ�,��ȣ����)//
			///////////////////////////////////////////////////////////////////
			

			//������ȣ�� �Է�
			//===================================================
			//2009.07.06 UPDATE BY PJW : ������ UDF_MANAGE_CODE�߰���
			pDM_OUT->InitDBFieldData();
			strQuery.Format(_T("INSERT INTO SE_DELETE_VOL_TBL ")
							_T("(SELECT REC_KEY,SPECIES_KEY,VOL_TYPE,TITLE,FORM_CODE,MEDIA_CODE,VOL_SORT_NO,")
							_T("VOL_TITLE,OTHER_VOL_TITLE,ALTERNATIVE_VOL_TITLE,IDX_VOL_TITLE,PUBLISH_DATE,")
							_T("CURRENCY_CODE,PRICE,PRICE_CHARACTER,PAGE,PHYSICAL_PROPERTY,BOOK_SIZE,ACCOMP_MAT,")
							_T("REMARK,VOLUME_CLASS,CONTENTS_YN,CONTENTS_KEY,'%s',FIRST_WORK,'%s',UDF_MANAGE_CODE ")
							_T("FROM SE_VOL_TBL WHERE REC_KEY = %s);"),strToday,strWorkLog,strVOL_KEY);
			if(!bOnlyVerify) pDM_OUT->AddFrame(strQuery);
			//===================================================
			

			//���� ��ȣ�� setting
			if(!bOnlyVerify) MakeEcoIndexVol(pDM_OUT,strVOL_KEY,FALSE);


			//���,����,�ʷϰ��� ����
			if(!bOnlyVerify) GetArticleDeleteQuery(pDM_OUT,strVOL_KEY);
			

			//��ȣ ����
			pDM_OUT->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
			if(!bOnlyVerify) 
			{
				pDM_OUT->AddFrame(strQuery);
				//===================================================
				//2009.05.11 ADD BY PJW : ���� �����Ұ�� �ηϵ� �����Ѵ�.
				if ( strDeleteAppendixYN == _T("Y") )
				{
					strQuery.Format(_T("DELETE FROM SE_APPENDIX_BOOK_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);						
					pDM_OUT->AddFrame(strQuery);
				}
				//===================================================
			}
			

		}	

	}
	else
	{
		
		//å�� �� �Լ� ���� update
		CString strCHECKIN_STATUS;
		if(0 == nMISS_BOOK_CNT) 
			strCHECKIN_STATUS = _T("1"); //�Լ�
		else if(0 == nACQ_BOOK_CNT) 
			strCHECKIN_STATUS = _T("2"); //��ȣ
		else 
			strCHECKIN_STATUS = _T("3"); //�̴�
		
		
		pDM_OUT->InitDBFieldData();
		strQuery.Format(_T("UPDATE SE_ACQ_TYPE_VOL_TBL SET ACQ_BOOK_CNT = %d, CHECKIN_STATUS = '%s',LAST_WORK = '%s' ")
						_T("WHERE REC_KEY = %s;"),nACQ_BOOK_CNT,strCHECKIN_STATUS,strWorkLog,strACQ_TYPE_VOL_KEY);
		if(!bOnlyVerify) pDM_OUT->AddFrame(strQuery);
		
	}

	
	//������ �ϴ� ���� return
	if(bOnlyVerify) return 0;


	//////////////////////////////////////////////////////////////
	//å�������� ����(å,�������,������Ϲ�ȣ ó��,�η�,����å)//
	//////////////////////////////////////////////////////////////
	

	//���� ���� ����
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_LOAN_MAIL_SEND_TBL WHERE LOAN_KEY IN ")
					_T("(SELECT REC_KEY FROM LH_STORAGE_V01 WHERE BOOK_KEY = %s);")
					,strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

   
	//���� ���� 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE LS_WORK_T01 WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);
	

	//��� �ڷ� ���� 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_DELIVERY_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

		
	//���� ���� ���� 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_REPAIR_BINDING_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

	
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//���� ��� ��ȣ ó�� 
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	pDM_OUT->InitDBFieldData();
// 	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
// 					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,2),SUBSTR(REG_NO,3,10),'%s','%s',UDF_MANAGE_CODE ")
// 					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
// 					_T("AND REC_KEY = %s);")
// 					,DEFAULT_SEQ_NAME,strWorkLog,strWorkLog,strBOOK_KEY);
// 	pDM_OUT->AddFrame(strQuery);

	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,%d),SUBSTR(REG_NO,%d,%d),'%s','%s',UDF_MANAGE_CODE ")
					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
					_T("AND REC_KEY = %s);")
					,DEFAULT_SEQ_NAME,nRegCodeLength,nRegCodeLength+1, 12-nRegCodeLength,strWorkLog,strWorkLog,strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);
	//===================================================== 
	


	//å DELETE
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE REC_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);


	//���� UPDATE
	APPLY_IndexItem_Book(pDM_OUT, strSPECIES_KEY);


	return 0;
}


INT CSeApi::BOOK_CAT_DELETE_APPENDIX(CESL_DataMgr* pDM_OUT,
									 CString strBOOK_KEY,
									 CString strSPECIES_KEY,
									 BOOL bOnlyVerify)
{

	//��,å���� �Լ��� �����Լ� 
	//�η�å���� �����ϴ� ������ Ʈ����� �ȿ� �ִ´�

	
	//������ �ϴ� ���� return
	if(bOnlyVerify) return 0;


	CString strQuery;
	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	//���� ���� ����
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_LOAN_MAIL_SEND_TBL WHERE LOAN_KEY IN ")
					_T("(SELECT REC_KEY FROM LH_STORAGE_V01 WHERE BOOK_KEY = %s);")
					,strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

   
	//���� ���� 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE LS_WORK_T01 WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);
	

	//��� �ڷ� ���� 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_DELIVERY_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

		
	//���� ���� ���� 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_REPAIR_BINDING_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);


	//�η�å DELETE
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE REC_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);


	//���� UPDATE
	APPLY_IndexItem_Book(pDM_OUT, strSPECIES_KEY);


	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//���� ��� ��ȣ ó�� 
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	pDM_OUT->InitDBFieldData();
// 	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
// 					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,2),SUBSTR(REG_NO,3,10),'%s','%s',UDF_MANAGE_CODE ")
// 					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
// 					_T("AND REC_KEY = %s);")
// 					,DEFAULT_SEQ_NAME,strWorkLog,strWorkLog,strBOOK_KEY);
// 	pDM_OUT->AddFrame(strQuery);

	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,%d),SUBSTR(REG_NO,%d,%d),'%s','%s',UDF_MANAGE_CODE ")
					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
					_T("AND REC_KEY = %s);")
					,DEFAULT_SEQ_NAME,nRegCodeLength,nRegCodeLength+1, 12-nRegCodeLength,strWorkLog,strWorkLog,strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);
	//=====================================================  
	


	return 0;
}

INT CSeApi::BOOK_DELETE(CESL_DataMgr* pDM_BOOK,
						CESL_DataMgr* pDM_VOL,
					    CESL_DataMgr* pDM_VOL_DELETE,
						INT nIdx_BOOK,
						BOOL bIsVerified)
{

	
	//å���� �Լ�
	//���Ա��к� �� ����, �� ������ �ľ��� ��������� ������ �ش� 
	
	if(!pDM_BOOK || !pDM_VOL || !pDM_VOL_DELETE) return -1;
	


	//�����ӿ����� ��� �Ǵ� ������ ��ü
	pDM_VOL->FreeData();
	pDM_VOL_DELETE->FreeData();
	
	if(!bIsVerified)
	{
		if(VerifyBookDeleteProc(pDM_BOOK,nIdx_BOOK)<0) return -1004; //verify ���� 
	}

	CString strBookKey,strVolKey,strAcqTypeVolKey,strAcqCode,strRegNo;
	CString strAccessionKey,strAccessionNo,strAccessionYear;
	
	strBookKey = pDM_BOOK->GetCellData(_T("SB_åKEY"),nIdx_BOOK);
	strVolKey = pDM_BOOK->GetCellData(_T("SB_��KEY"),nIdx_BOOK);
	strAcqTypeVolKey = pDM_BOOK->GetCellData(_T("SB_���Ա��к���KEY"),nIdx_BOOK);
	strAcqCode = pDM_BOOK->GetCellData(_T("SB_���Ա���"),nIdx_BOOK);
	strRegNo = pDM_BOOK->GetCellData(_T("SB_��Ϲ�ȣ"),nIdx_BOOK);
	strAccessionKey = pDM_BOOK->GetCellData(_T("SB_��Ͽ���KEY"),nIdx_BOOK);
	
	CString strQuery;

	if(!strAccessionKey.IsEmpty())
	{
		CString strAccessData[2];
		
		strQuery.Format(_T("SELECT ACCESSION_REC_NO,ACCESSION_REC_MAKE_YEAR FROM CO_ACCESSION_REC_TBL WHERE REC_KEY = %s"),strAccessionKey);
		pDM_BOOK->GetValuesQuery(strQuery,strAccessData,2);
		strAccessionNo = strAccessData[0];
		strAccessionYear = strAccessData[1];
	}

	CString strResult[3];
	
	strQuery.Format(_T("SELECT ACQ_BOOK_CNT,MISS_BOOK_CNT,CHECKIN_STATUS FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s"),strAcqTypeVolKey);
	pDM_BOOK->GetValuesQuery(strQuery,strResult,3);
	
	INT nACQ_BOOK_CNT;
	INT nMISS_BOOK_CNT;
	nACQ_BOOK_CNT = _ttoi(strResult[0].GetBuffer(0));
	nMISS_BOOK_CNT = _ttoi(strResult[1].GetBuffer(0));
	
	nACQ_BOOK_CNT--;

	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	pDM_BOOK->StartFrame();

	if(0 == nACQ_BOOK_CNT && 0 == nMISS_BOOK_CNT) //���Ա��к� �� ������ ���� �ؾ��ϴ� ��Ȳ 
	{

		//Ŭ���� �̷� ���� delete 
		pDM_BOOK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY = %s;"),strAcqTypeVolKey);
		pDM_BOOK->AddFrame(strQuery);
	

		//���Ա��к� ��ȣ ����
		pDM_BOOK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s;"),strAcqTypeVolKey);
		pDM_BOOK->AddFrame(strQuery);
	

		//�� �������� �ؾ� �ϴ� ���� �Ǵ��Ѵ�
		CString strAnotherBookCnt = _T("");
		INT nAnotherBook = 0;
		//++2008.10.17 UPDATE BY PWR {{++
		// ���սý��� ����
		strQuery.Format( _T("SELECT COUNT(*) FROM SE_BOOK_TBL ")
						 _T("WHERE VOL_KEY=%s AND ACQ_CODE!='%s' AND BOOK_TYPE NOT IN ('B','A') ")
						 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strVolKey, strAcqCode );
		// strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND ACQ_CODE != '%s' AND BOOK_TYPE NOT IN ('B','A')"),strVolKey,strAcqCode);
		//--2008.10.17 UPDATE BY PWR --}}
		pDM_BOOK->GetOneValueQuery(strQuery,strAnotherBookCnt);
		nAnotherBook = _ttoi(strAnotherBookCnt.GetBuffer(0));


		if(0 == nAnotherBook) //�� ���� ���� 
		{
			
			//���� ��ȣ�� �̵�
			//�������� �����´�.
			CString strWhere;
			strWhere.Format(_T("REC_KEY = %s"),strVolKey); 
			pDM_VOL->RefreshDataManager(strWhere);
			
			INT nRowCnt = pDM_VOL->GetRowCount();
	
			if(nRowCnt>0)
			{
				CString strWorkLog;
				INT nRowDeleteVol;
				CArray<CString,CString> RemoveAliasList;
				strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
				
				pDM_VOL_DELETE->InsertRow(-1);	
				nRowDeleteVol = pDM_VOL_DELETE->GetRowCount()-1;
				CopyDMToDMByAlias(pDM_VOL,0,pDM_VOL_DELETE,nRowDeleteVol);
				pDM_VOL_DELETE->SetCellData(_T("SV_��������"),GetTodayDate(),nRowDeleteVol);
				pDM_VOL_DELETE->SetCellData(_T("SV_�����۾�"),strWorkLog,nRowDeleteVol);
				pDM_VOL_DELETE->SetCellData(_T("SV_�������۾�"),strWorkLog,nRowDeleteVol);
					
				//������ insert
				RemoveAliasList.RemoveAll();
				pDM_BOOK->InitDBFieldData();
				pDM_BOOK->MakeQueryByDM(RemoveAliasList, pDM_VOL_DELETE, _T("SE_DELETE_VOL_TBL"), nRowDeleteVol , pDM_BOOK);
				
				//�� delete
				pDM_BOOK->InitDBFieldData();
				strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVolKey);
				pDM_BOOK->AddFrame(strQuery);
				
				//���� ��ȣ�� setting
				MakeEcoIndexVol(pDM_BOOK,strVolKey,FALSE);

			}

		}	


	}
	else
	{
		
		//å�� �� �Լ� ���� update
		CString strCHECKIN_STATUS;
		if(0 == nMISS_BOOK_CNT) 
			strCHECKIN_STATUS = _T("1"); //�Լ�
		else if(0 == nACQ_BOOK_CNT) 
			strCHECKIN_STATUS = _T("2"); //��ȣ
		else 
			strCHECKIN_STATUS = _T("3"); //�̴�
		
		
		pDM_BOOK->InitDBFieldData();
		strQuery.Format(_T("UPDATE SE_ACQ_TYPE_VOL_TBL SET ACQ_BOOK_CNT = %d, CHECKIN_STATUS = '%s',LAST_WORK = '%s' ")
						_T("WHERE REC_KEY = %s;"),nACQ_BOOK_CNT,strCHECKIN_STATUS,strWorkLog,strAcqTypeVolKey);
		pDM_BOOK->AddFrame(strQuery);
		
	}
	
	
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//������Ϲ�ȣ ó��
	if(!strRegNo.IsEmpty())
	{
		CString strRegOnlyCode;
		CString strRegOnlyNo;
		INT nRegOnlyNo;


		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		strRegOnlyCode = strRegNo.Left(2);
// 		nRegOnlyNo = _ttoi(strRegNo.Mid(2).GetBuffer(0));
// 		strRegOnlyNo.Format(_T("%d"),nRegOnlyNo);
		
		strRegOnlyCode = strRegNo.Left(nRegCodeLength);
		nRegOnlyNo = _ttoi(strRegNo.Mid(nRegCodeLength).GetBuffer(0));
		strRegOnlyNo.Format(_T("%d"),nRegOnlyNo);
		//=====================================================
		
			
		//��� ��ȣ�� ����� ��ȣ������ üũ�Ѵ�.(��ϱ����� �����̸� ����� ��ȣ)
		if(!IsDigitString(strRegOnlyCode))
		{

			pDM_BOOK->InitDBFieldData();
			//++2008.10.17 UPDATE BY PWR {{++
			// ���սý��� ����
			strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
							_T("(REC_KEY,PUBLISH_FORM_CODE,ACCESSION_REC_MAKE_YEAR,ACCESSION_REC_NO,REG_CODE,REG_NO,FIRST_WORK,LAST_WORK, MANAGE_CODE) ")
							_T("VALUES (%s.NEXTVAL,'SE','%s','%s','%s','%s','%s','%s',UDF_MANAGE_CODE);"),
							DEFAULT_SEQ_NAME,strAccessionYear,strAccessionNo,strRegOnlyCode,strRegOnlyNo,strWorkLog,strWorkLog);
//			strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
//							_T("(REC_KEY,PUBLISH_FORM_CODE,ACCESSION_REC_MAKE_YEAR,ACCESSION_REC_NO,REG_CODE,REG_NO,FIRST_WORK,LAST_WORK) ")
//							_T("VALUES (%s.NEXTVAL,'SE','%s','%s','%s','%s','%s','%s');"),
//							DEFAULT_SEQ_NAME,strAccessionYear,strAccessionNo,strRegOnlyCode,strRegOnlyNo,strWorkLog,strWorkLog);
			//--2008.10.17 UPDATE BY PWR --}}
			
			pDM_BOOK->AddFrame(strQuery);
		}
	}


	pDM_BOOK->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE REC_KEY = %s;"),strBookKey);
	pDM_BOOK->AddFrame(strQuery);
	
	
	//��KEY
	CString strSPECIES_KEY;
	strSPECIES_KEY = pDM_BOOK->GetCellData(_T("SB_��KEY"),nIdx_BOOK);

	//���� UPDATE
	APPLY_IndexItem_Book(pDM_BOOK, strSPECIES_KEY);
	

	INT ids = pDM_BOOK->SendFrame();
	pDM_BOOK->EndFrame();	
	
	if(ids<0) return -1005; //���� ����
	
	return 0;
}


INT CSeApi::VerifyBookDeleteProc(CESL_DataMgr* pDM_BOOK,INT nIdx_BOOK)
{
	
	//��� å�� ���� �Ǿ��� ��� -- ���, �η� ���� �Ǻ�
	CString strAcqVolKey;
	CString strVolKey;
	CString msg;
	CString strQuery;
	strAcqVolKey = pDM_BOOK->GetCellData(_T("SB_���Ա��к���KEY"),nIdx_BOOK);
	strVolKey = pDM_BOOK->GetCellData(_T("SB_��KEY"),nIdx_BOOK);


    //�Լ�å�� ��ȣå�� ������ �´�
	INT nMissBookCnt = 0;
  	INT nAcqBookCnt = 0;
	CString strResult[2];

	strQuery.Format(_T("SELECT ACQ_BOOK_CNT, MISS_BOOK_CNT FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s"),strAcqVolKey);
	pDM_BOOK->GetValuesQuery(strQuery,strResult,2);

	if(!strResult[0].IsEmpty()) nAcqBookCnt = _ttoi(strResult[0].GetBuffer(0));
	if(!strResult[1].IsEmpty()) nMissBookCnt = _ttoi(strResult[1].GetBuffer(0));		
		
	if(1 == nAcqBookCnt && 0 == nMissBookCnt) //�ش� ���Ա��к� ��ȣ ���� 
	{
	
		//�ٸ� ���� ������ �ִ����� ����
		CString strResult;
		INT nAcqVolCnt = 0;
		strQuery.Format(_T("SELECT COUNT(*) FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s"),strVolKey);
		pDM_BOOK->GetOneValueQuery(strQuery,strResult);			
		if(!strResult.IsEmpty()) nAcqVolCnt = _ttoi(strResult.GetBuffer(0));
			
		if(nAcqVolCnt>1) //�ٸ� ���Ա����� �ִ� ��� (�ش� ��ȣ ���� �޽���)
		{
			msg.Format(_T("�ش�å ������ ���� å�� ���� �Ǿ�\n") 
						_T("��ȣ ������ ������ϴ�\n")
						_T("�׷��� ���� �Ͻðڽ��ϱ�?"));
				
			if(AfxMessageBox(msg,MB_YESNO) != IDYES) return -1001;

		}
		else  //�ٸ� ���Ա����� ���� ��� (�ش� ��ȣ�� ���� �η��� Ȯ�� �� ���� �޽���)
		{
				
			CString strArticleCnt;
			INT nArticleCnt;
			strQuery.Format(_T("SELECT COUNT(*) FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s"),strVolKey);
			pDM_BOOK->GetOneValueQuery(strQuery,strArticleCnt);
			nArticleCnt = _ttoi(strArticleCnt.GetBuffer(0));
					
			if(nArticleCnt>0)
			{
				AfxMessageBox(_T("�ش� ��ȣ��  ��������� �־� �ش� å�� ���� �Ҽ� �����ϴ�!\n")
							   _T("���� ��������� �����Ͻ� �� ���� �Ͻ� �� �ֽ��ϴ�."));					  
				return -1002;
			}
				
			CString strAppendixCnt;
			INT nAppendixCnt;
			strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND BOOK_TYPE = 'A'"),strVolKey);
			pDM_BOOK->GetOneValueQuery(strQuery,strAppendixCnt);
			nAppendixCnt = _ttoi(strAppendixCnt.GetBuffer(0));
	
			if(nAppendixCnt>0)
			{
				AfxMessageBox(_T("�ش� ��ȣ�� �η� ������ �־� �ش� å�� ���� �Ҽ� �����ϴ�!\n")
									_T("���� �η� ������ �����Ͻ� �� ���� �Ͻ� �� �ֽ��ϴ�."));	
				return -1003;
			}

			msg.Format(_T("�ش�å ������ ���� å�� ���� �Ǿ�\n") 
			_T("��ȣ ������ ������ϴ�\n")
			_T("�׷��� ���� �Ͻðڽ��ϱ�?"));
				
			if(AfxMessageBox(msg,MB_YESNO) != IDYES) return -1001;
		}

	}


	return 0;
}

				
/*
INT CSeApi::BOOK_DELETE(CESL_DataMgr* pDM, 
						CString strBookKey, 
						CString strVolKey, 
						CString strAcqTypeVolKey,
						CString strAcqCode,
						CString strRegNo,
						CString strAccessionNo,
						CString strAccessionYear)
{
	//å���� �Լ� - ���� ���� �ܰ迡�� ����Ѵ�. 
	//���Ա��к� �� ����, �� ������ �ľ��� ��������� ������ �ش� 
	if(!pDM || strBookKey.IsEmpty() || strVolKey.IsEmpty() || strAcqTypeVolKey.IsEmpty() || strAcqCode.IsEmpty()) return -1;

	CString strQuery;
	CString strResult[3];
	
	strQuery.Format(_T("SELECT ACQ_BOOK_CNT,MISS_BOOK_CNT,CHECKIN_STATUS FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s"),strAcqTypeVolKey);
	pDM->GetValuesQuery(strQuery,strResult,3);
	
	INT nACQ_BOOK_CNT;
	INT nMISS_BOOK_CNT;
	nACQ_BOOK_CNT = _ttoi(strResult[0].GetBuffer(0));
	nMISS_BOOK_CNT = _ttoi(strResult[1].GetBuffer(0));
	
	nACQ_BOOK_CNT--;

	pDM->StartFrame();

	if(0 == nACQ_BOOK_CNT && 0 == nMISS_BOOK_CNT) //���Ա��к� �� ������ ���� �ؾ��ϴ� ��Ȳ 
	{
		
		pDM->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s"),strAcqTypeVolKey);
		pDM->AddFrame(strQuery);
		

		//�� �������� �ؾ� �ϴ� ���� �Ǵ��Ѵ�
		CString strAnotherBookCnt = _T("");
		INT nAnotherBook = 0;
		strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND ACQ_CODE != '%s' AND BOOK_TYPE NOT IN ('B','A')"),strVolKey,strAcqCode);
		pDM->GetOneValueQuery(strQuery,strAnotherBookCnt);
		nAnotherBook = _ttoi(strAnotherBookCnt.GetBuffer(0));

		if(0 == nAnotherBook) //�� ���� ���� (�ش�ǿ� ���� �η��� ����Ǿ� �ִ� ���� �˻��Ѵ�)
		{
			
			CString strArticleCnt;
			INT nArticleCnt;
			strQuery.Format(_T("SELECT COUNT(*) FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s"),strVolKey);
			pDM->GetOneValueQuery(strQuery,strArticleCnt);
			nArticleCnt = _ttoi(strArticleCnt.GetBuffer(0));
			
			if(nArticleCnt>0)
			{
				AfxMessageBox(_T("�ش� ��ȣ�� ��� ������ �־� ���� �Ҽ� �����ϴ�")); 
				pDM->EndFrame();
				return -1000;
			}

			CString strAppendixCnt;
			INT nAppendixCnt;
			strQuery.Format(_T("SELECT COUNT(*) FROM SE_APPENDIX_BOOK_TBL WHERE VOL_KEY = %s"),strVolKey);
			pDM->GetOneValueQuery(strQuery,strAppendixCnt);
			nAppendixCnt = _ttoi(strAppendixCnt.GetBuffer(0));

			if(nAppendixCnt>0)
			{
				AfxMessageBox(_T("�ش� ��ȣ�� �η��� �־� ���� �Ҽ� �����ϴ�")); 
				pDM->EndFrame();
				return -1001;
			}

			pDM->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s"),strVolKey);
			pDM->AddFrame(strQuery);
			
			//���� ��ȣ �� �̵�
			

		}	


	}
	else
	{
		
		//å�� �� �Լ� ���� update
		CString strCHECKIN_STATUS;
		if(0 == nMISS_BOOK_CNT) 
			strCHECKIN_STATUS = _T("1"); //�Լ�
		else if(0 == nACQ_BOOK_CNT) 
			strCHECKIN_STATUS = _T("2"); //��ȣ
		else 
			strCHECKIN_STATUS = _T("3"); //�̴�
		
		
		pDM->InitDBFieldData();
		strQuery.Format(_T("UPDATE SE_ACQ_TYPE_VOL_TBL SET ACQ_BOOK_CNT = %d, CHECKIN_STATUS = '%s' WHERE REC_KEY = %s"),nACQ_BOOK_CNT,strCHECKIN_STATUS,strAcqTypeVolKey);
		pDM->AddFrame(strQuery);
		
	}
	

	//������Ϲ�ȣ ó��
	if(!strRegNo.IsEmpty())
	{
		CString strRegOnlyCode;
		CString strRegOnlyNo;
		INT nRegOnlyNo;
		CString strWorkLog;
		strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

		strRegOnlyCode = strRegNo.Left(2);
		nRegOnlyNo = _ttoi(strRegNo.Mid(2).GetBuffer(0));
		strRegOnlyNo.Format(_T("%d"),nRegOnlyNo);
			
		//��� ��ȣ�� ����� ��ȣ������ üũ�Ѵ�.(��ϱ����� �����̸� ����� ��ȣ)
		if(!IsDigitString(strRegOnlyCode))
		{

			pDM->InitDBFieldData();
			strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
							_T("(REC_KEY,PUBLISH_FORM_CODE,ACCESSION_REC_MAKE_YEAR,ACCESSION_REC_NO,REG_CODE,REG_NO,FIRST_WORK,LAST_WORK) ")
							_T("VALUES (%s.NEXTVAL,'SE','%s','%s','%s','%s','%s',%s');"),
							DEFAULT_SEQ_NAME,strAccessionYear,strAccessionNo,strRegOnlyCode,strRegOnlyNo,strWorkLog,strWorkLog);
			
			pDM->AddFrame(strQuery);
		}
	}


	pDM->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE REC_KEY = %s"),strBookKey);
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();	

	return ids;
}
*/  

INT CSeApi::GetDeleteHighestPriority(CESL_DataMgr* pDM_BOOK)
{
	//åDM �� ������ �� �ִ� �켱 ������ ���´� 
	
	CString strCompare; // �� �켱 ���� 
	CString strPriority; //���� �ְ� �켱 ���� 
	INT nHighestPriorityIdx = -1; //�ְ� �켱 ���� rowidx

	for(INT i=0;i<pDM_BOOK->GetRowCount();i++)
	{
		strCompare = _T("");
		strCompare = pDM_BOOK->GetCellData(_T("UDF_�����켱����"),i);
		
		if(0 == i) 
		{
			if(_T("Z") != strCompare)
			{
				strPriority = strCompare;
				nHighestPriorityIdx = 0;
			}
		}
		else
		{
			if(nHighestPriorityIdx<0)
			{
				if(_T("Z") != strCompare)
				{
					strPriority = strCompare;
					nHighestPriorityIdx = i;
				}
			}
			else
			{
				if(strPriority>strCompare) {
					strPriority = strCompare;
					nHighestPriorityIdx = i;
				}
					
			}
		}
	}
	
	pDM_BOOK->SetCellData(_T("UDF_�����켱����"),_T("Z"),nHighestPriorityIdx);

	return nHighestPriorityIdx;
}

INT CSeApi::MoveBookToMissBook(CESL_DataMgr* pDM_BOOK,INT nACQ_TYPE,INT nRowIdx)
{
	//�ش� row�� å�� ��ȣå���� setting �Ѵ�.
	
	if(1 == nACQ_TYPE) pDM_BOOK->SetCellData(_T("SB_�ڷ����"),_T("SEA112N"),nRowIdx);  //���� ��ȣ   	
	else pDM_BOOK->SetCellData(_T("SB_�ڷ����"),_T("SEA212N"),nRowIdx);	 //���� ��ȣ 
	
	pDM_BOOK->SetCellData(_T("SB_å����"),_T("M"),nRowIdx); //��ȣå 
	pDM_BOOK->SetCellData(_T("SB_�Լ���"),_T(""),nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_�ǽð��ΰ���"),_T(""),nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_�����ΰ���"),_T(""),nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_����ΰ���"),_T(""),nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_���Գ⵵"),_T(""),nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_������ȣ"),_T(""),nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_�����Ϸù�ȣ"),_T(""),nRowIdx); 

	return 0;
}

INT CSeApi::MoveBookToAcqBook(CESL_DataMgr* pDM_WORK,
							  INT nIdx_WORK,
							  CESL_DataMgr* pDM_BOOK,
							  CESL_DataMgr* pDM_BOOK_INSERT,
							  CESL_DataMgr* pDM_BOOK_OUT,
							  INT nACQ_TYPE,
							  CString strACQ_YEAR,
							  CString strRECEIPT_NO,
							  INT nRowIdx,
							  BOOL bIsDepartMngUse,
					          CESL_DataMgr* pDM_DEPART)
{
	//�ش� row�� å�� �Լ�å���� setting �Ѵ�.
	//�����켱����, ��������, ��� å���� Ȯ���� WORKING_STATUS�� ���Ѵ�. 

	
	//����
	CString strLAST_SERIAL_NO;
	CString strToday = GetTodayDate();
	pDM_BOOK_OUT->SetCellData(_T("SB_å����"),_T("C"),nRowIdx); //�Ϲ�å 
	pDM_BOOK_OUT->SetCellData(_T("SB_�Լ���"),strToday,nRowIdx); 
	pDM_BOOK_OUT->SetCellData(_T("SB_���Գ⵵"),strACQ_YEAR,nRowIdx); 
	pDM_BOOK_OUT->SetCellData(_T("SB_������ȣ"),strRECEIPT_NO,nRowIdx); 
	strLAST_SERIAL_NO = GetReceiptSerialNo(pDM_BOOK, nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
	pDM_BOOK_OUT->SetCellData(_T("SB_�����Ϸù�ȣ"),strLAST_SERIAL_NO,nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_�ǽð��ΰ���"),strToday,nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_�����ΰ���"),strToday,nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_����ΰ���"),strToday,nRowIdx); 

	//�ڷ� ���� 
	//���ڷ��� ���� ���� 
	CString strBINDING_YN;
	strBINDING_YN = pDM_WORK->GetCellData(_T("SS_��������"),nIdx_WORK);

	//���Ա����� �����켱����,���å��  
	CString strREAD_FIRST_YN,strREG_BOOK_CNT;
	strREAD_FIRST_YN = pDM_WORK->GetCellData(_T("UDF_�����켱����"),nIdx_WORK);
	strREG_BOOK_CNT = pDM_WORK->GetCellData(_T("UDF_���å��"),nIdx_WORK);
	INT nREG_BOOK_CNT = 0;
	if(!strREG_BOOK_CNT.IsEmpty()) nREG_BOOK_CNT = _ttoi(strREG_BOOK_CNT.GetBuffer(0));
	

	if(_T("Y") == strREAD_FIRST_YN) //�ǽð� ó�� �Ǿ� �ִ� ���� ������ �ǽð� ó���Ѵ�  
	{
		pDM_BOOK_OUT->SetCellData(_T("SB_�ڷ����"),_T("SEL111O"),nRowIdx);    	
	}
	else //�ǽð� ó���Ǿ� ���� ���� ���  
	{
		
		//���� ���� ������ ��� å���� ���� å�� ���� �� ��� å���� ���Ѵ� 
		if(nREG_BOOK_CNT>0) 
		{
			
			CString strWORKING_STATUS;
			INT nCurrentBindBook = 0;
			INT nCurrentRegBook = 0;

			//���� �����Ϳ��� ���� �� ��� ����� å�� �ľ��Ѵ� 
			for(INT i=0;i<pDM_BOOK->GetRowCount();i++)
			{
				strWORKING_STATUS = pDM_BOOK->GetCellData(_T("SB_�ڷ����"),i);
				if(_T("SEB111N") == strWORKING_STATUS) nCurrentBindBook++;
				if(_T("SER111N") == strWORKING_STATUS) nCurrentRegBook++;
			}
			
			for(i=0;i<pDM_BOOK_INSERT->GetRowCount();i++)
			{
				strWORKING_STATUS = pDM_BOOK_INSERT->GetCellData(_T("SB_�ڷ����"),i);
				if(_T("SEB111N") == strWORKING_STATUS) nCurrentBindBook++;
				if(_T("SER111N") == strWORKING_STATUS) nCurrentRegBook++;
			}
			
			nREG_BOOK_CNT -= nCurrentBindBook;
			nREG_BOOK_CNT -= nCurrentRegBook;
			
			if(nREG_BOOK_CNT>0) //�� ��� Ȥ�� �����ؾ��� å�� ���� ������ 
			{
				//���� ���θ� �Ǵ��Ѵ�.
				if(_T("Y") == strBINDING_YN) pDM_BOOK_OUT->SetCellData(_T("SB_�ڷ����"),_T("SEB111N"),nRowIdx); 
				else pDM_BOOK_OUT->SetCellData(_T("SB_�ڷ����"),_T("SER111N"),nRowIdx); 	
			}
			else //�ƴϸ� �ǽð� 
			{
				pDM_BOOK_OUT->SetCellData(_T("SB_�ڷ����"),_T("SEL111O"),nRowIdx);  
			}

		}
		else //�ǽð� ó�� 
		{
			pDM_BOOK_OUT->SetCellData(_T("SB_�ڷ����"),_T("SEL111O"),nRowIdx);    	
		}
	}

	if(bIsDepartMngUse)
	{
		//�⺻ �谡��ġ�ڵ�
		CString strSHELF_LOC_CODE = _T("");
	    strSHELF_LOC_CODE = pDM_WORK->GetCellData(_T("SS_�谡��ġ_�ڵ�"),nIdx_WORK);

		CString strResultCode;
		INT ids = -1;

		ids = GetNextPriorityShelfLocCode(strResultCode,
										  strSHELF_LOC_CODE,
										  pDM_DEPART,
										  pDM_BOOK,
										  pDM_BOOK_INSERT,
										  2);

		if(ids>=0) pDM_BOOK_OUT->SetCellData(_T("SB_�谡��ġ_�ڵ�"),strResultCode,nRowIdx);
	}


	return 0;
}

INT CSeApi::InspectDuplicateVol(CESL_DataMgr* pDM_WORK,INT nIdx_WORK,CESL_DataMgr* pDM_VOL)
{
	//��ȣ ���� ���� �Լ� 
	//SV_��ȣ���� IDX_VOL_TITLE�� catsearch�˻��� �� ����� ������ ����� �����ش� 
	CString strVOL_TITLE = _T("");
	strVOL_TITLE = pDM_WORK->GetCellData(_T("SV_��ȣ��"),nIdx_WORK);
	CString strSPECIES_KEY = _T("");
	strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_��KEY"),nIdx_WORK);
	
	if(strVOL_TITLE.IsEmpty()) return 0;

	CString strQuery;

/*
if(ORA_CATSEARCH == m_pMgr->m_pInfo->GetSearchIndexType())
	strQuery.Format(_T("SPECIES_KEY = %s AND CATSEARCH(IDX_VOL_TITLE, '%s*', NULL)>0"),strSPECIES_KEY,strVOL_TITLE);
else
	strQuery.Format(_T("SPECIES_KEY = %s AND &ECOSEARCH(REC_KEY,SE_VOL_TITLE, '%s*)"),strSPECIES_KEY,strVOL_TITLE);
*/
	
	//like�˻����� ��ȯ 
	strQuery.Format(_T("SPECIES_KEY = %s AND IDX_VOL_TITLE LIKE '%%%s%%'"),strSPECIES_KEY,strVOL_TITLE);

	pDM_VOL->RefreshDataManager(strQuery);

	
	if(pDM_VOL->GetRowCount()>0) //������ ��ȣ�� ������ Ȯ�� â�� ���� 
	{
		CSeDuplicatedVolDlg dlg(m_pMgr);
		dlg.m_strVOL_TITLE = strVOL_TITLE;
		INT nCheck = dlg.DoModal();

		if(nCheck == IDOK) //���� ��ȣ �Է� 
		{
			INT nIdx = dlg.m_nCurrentIdx;
			CString strVOL_KEY = pDM_VOL->GetCellData(_T("SV_��KEY"),nIdx);
			strQuery.Format(_T("REC_KEY = %s"),strVOL_KEY);
			pDM_VOL->RefreshDataManager(strQuery); //���� ��ȣ�� ������ �´� 

		}
		else if(nCheck == IDCANCEL) //�۾� ���� 
		{
			return -1;
		}
		else //�� ��ȣ�� �Է� 
		{
			pDM_VOL->FreeData();
		}

	}

	return 0; 
}

INT CSeApi::MAKE_INDEX_by_SPECIES_MARC(CESL_DataMgr* pDM_SPECIES,CMarc* pMarc,CESL_DataMgr* pDM_IDX,INT nRowIdx)
{
	
	//pDM_SPECIES �˸��ƽ��� ���� pDM_IDX�� ����
	CString field_alias, field_name, field_type, data_type, strData;
	INT npos = 0;
	CString strtmpalias;

	INT nColumn = pDM_SPECIES->RefList.GetCount();
	for(INT col = 0; col < nColumn; col++) {   
		strData = _T("");
		pDM_SPECIES->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, nRowIdx, strData);
		
		npos = field_alias.Find(_T("_"));
		if(npos<0) continue;
		
		strtmpalias.Format(_T("ISS_%s"),field_alias.Mid(npos+1));
	
		pDM_IDX->SetCellData(strtmpalias, strData, nRowIdx);
	}


	//IndexToolKit�� ����� MARC���� ���������� �����Ѵ�
	APPLY_IndexItem(pDM_IDX,pMarc,nRowIdx); 


	//2004.02.24 ����(���� SHELF_LOC_CODE -> ������ PLACE_INFO, MAIN_PLACE_INFO)
	//(SS_�谡��ġ_�ڵ�-> ISS_�ڷ��ִ°�INFO_�ڵ�, ISS_���ڷ��ִ°�INFO_�ڵ�)
	strData = pDM_SPECIES->GetCellData(_T("SS_�谡��ġ_�ڵ�"),nRowIdx);
	pDM_IDX->SetCellData(_T("ISS_�ڷ��ִ°�INFO_�ڵ�"), strData, nRowIdx);
	pDM_IDX->SetCellData(_T("ISS_���ڷ��ִ°�INFO_�ڵ�"), strData, nRowIdx);	



	return 0;
}

INT CSeApi::MAKE_EXP_WORK_by_IDX_ACQ(CESL_DataMgr* pDM_IDX,INT nIdx_IDX,CESL_DataMgr* pDM_WORK,INT nIdx_WORK,CESL_DataMgr* pDM_ACQ, INT nACQ_TYPE)
{
	//üũ�� ���� ����(����, ���Ա���)�� ���� ���������� ���� 
	//�ǰ� ���Ա��к� �� ������ ����� ����. 
	

///////////////////////////////////������ 
	
	//���� ����
	CString strFORM_CODE_IDX = _T("");
	strFORM_CODE_IDX = pDM_IDX->GetCellData(_T("ISS_���ı���_�ڵ�"),nIdx_IDX);
	pDM_WORK->SetCellData(_T("SV_���ı���_�ڵ�"),strFORM_CODE_IDX,nIdx_WORK);


	//��ü����
	CString strMEDIA_CODE_IDX = _T("");
	strMEDIA_CODE_IDX = pDM_IDX->GetCellData(_T("ISS_��ü����_�ڵ�"),nIdx_IDX);
	pDM_WORK->SetCellData(_T("SV_��ü����_�ڵ�"),strMEDIA_CODE_IDX,nIdx_WORK);


	//ȭ�� ����	
	CString strCURRENCY_CODE_IDX = _T("");
	strCURRENCY_CODE_IDX = pDM_IDX->GetCellData(_T("SS_ȭ�󱸺�_�ڵ�"),nIdx_IDX);
	pDM_WORK->SetCellData(_T("SV_ȭ�󱸺�"),strCURRENCY_CODE_IDX,nIdx_WORK);


	//���� 
	CString strPRICE_IDX = _T("");
	strPRICE_IDX = pDM_IDX->GetCellData(_T("SS_����"),nIdx_IDX);
	pDM_WORK->SetCellData(_T("SV_����"),strPRICE_IDX,nIdx_WORK);


	//���ݼ��� 
	CString strPRICE_CHARACTER_IDX = _T("");
	strPRICE_CHARACTER_IDX = pDM_IDX->GetCellData(_T("SS_���ݼ���"),nIdx_IDX);
	pDM_WORK->SetCellData(_T("SV_���ݼ���"),strPRICE_CHARACTER_IDX,nIdx_WORK);


	//������ ��ũ ������ ������ �´�.
	CString strMarcStream;
	strMarcStream = pDM_IDX->GetCellData(_T("MARC"),nIdx_IDX);
	CMarc Marc;
	m_pMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pMgr->m_pInfo->pMarcMgr->Decoding(strMarcStream, &Marc);

	
	//��ȣǥ��(245$a)
	CString strTITLE_IDX = _T("");
	m_pMgr->m_pInfo->pMarcMgr->GetItem(&Marc,_T("245$a"),strTITLE_IDX);
	pDM_WORK->SetCellData(_T("SV_��ȣǥ��"),strTITLE_IDX,nIdx_WORK);

	if(strTITLE_IDX.IsEmpty())
	{
		CString strTITLE_IDX = _T("");
		strTITLE_IDX = pDM_IDX->GetCellData(_T("ISS_��ǥ��"),nIdx_IDX);
		pDM_WORK->SetCellData(_T("SV_��ȣǥ��"),strTITLE_IDX,nIdx_WORK);
	}


	//����� 
	CString strPAGE_IDX = _T("");
	m_pMgr->m_pInfo->pMarcMgr->GetItem(&Marc,_T("300$a"),strPAGE_IDX);
	pDM_WORK->SetCellData(_T("SV_�����"),strPAGE_IDX,nIdx_WORK);


	//������Ư�� 
	CString strPHYSICAL_IDX = _T("");
	m_pMgr->m_pInfo->pMarcMgr->GetItem(&Marc,_T("300$b"),strPHYSICAL_IDX);
	pDM_WORK->SetCellData(_T("SV_������Ư��"),strPHYSICAL_IDX,nIdx_WORK);


	//åũ�� 
	CString strSIZE_IDX = _T("");
	m_pMgr->m_pInfo->pMarcMgr->GetItem(&Marc,_T("300$c"),strSIZE_IDX);
	pDM_WORK->SetCellData(_T("SV_åũ��"),strSIZE_IDX,nIdx_WORK);


	//�����ڷ� 
	CString strACCOMP_IDX = _T("");
	m_pMgr->m_pInfo->pMarcMgr->GetItem(&Marc,_T("300$d"),strACCOMP_IDX);
	pDM_WORK->SetCellData(_T("SV_�����ڷ�"),strACCOMP_IDX,nIdx_WORK);



//////////////////���Ա��к� �� ���� 
	//���Ա���
	CString strACQ_TYPE;
	strACQ_TYPE.Format(_T("%d"),nACQ_TYPE);
	pDM_WORK->SetCellData(_T("SA_���Ա���"),strACQ_TYPE,nIdx_WORK);


	//���Ա���KEY , �Լ�ó ,�Լ�����å��  
	CString strACQ_KEY = _T("");
	CString strPROVIDER = _T("");
	CString strACQ_EXP_BOOK_CNT = _T("");
	if(1 == nACQ_TYPE)
	{
		strACQ_KEY = pDM_ACQ->GetCellData(_T("SP_���԰���KEY"),0);
		strPROVIDER = pDM_ACQ->GetCellData(_T("SP_����ó_�ڵ�"),0);
		strACQ_EXP_BOOK_CNT = pDM_ACQ->GetCellData(_T("SP_�Լ�å��"),0);
	} 
	else 
	{
		strACQ_KEY = pDM_ACQ->GetCellData(_T("SD_��������KEY"),0);
		strPROVIDER = pDM_ACQ->GetCellData(_T("SD_�����ڸ�"),0);
		strACQ_EXP_BOOK_CNT = pDM_ACQ->GetCellData(_T("SD_�Լ�å��"),0);
	}

	pDM_WORK->SetCellData(_T("SA_���Ա���KEY"),strACQ_KEY,nIdx_WORK);
	pDM_WORK->SetCellData(_T("SA_�Լ�ó"),strPROVIDER,nIdx_WORK);
	pDM_WORK->SetCellData(_T("SA_�Լ�����å��"),strACQ_EXP_BOOK_CNT,nIdx_WORK);
	pDM_WORK->SetCellData(_T("SA_�Լ�å��"),_T("0"),nIdx_WORK);
	pDM_WORK->SetCellData(_T("SA_��ȣå��"),_T("0"),nIdx_WORK);
	

////////////////////��Ÿ����
	
	//���� - �� ���� 	
	const INT cnt = 11;
	CString alias[cnt] = {
		_T("ISS_��KEY"),
		_T("ISS_�����ȣ"),
		_T("SS_��ϱ���_�ڵ�"),
		_T("SS_�̿����ѱ���_�ڵ�"),
		_T("SS_��������_�ڵ�"),
		_T("SS_�谡��ġ_�ڵ�"),
		_T("SS_�з���ȣ����_�ڵ�"),
		_T("SS_��������"),
		_T("ISS_���ı���_�ڵ�"),
		_T("ISS_��ü����_�ڵ�"),
		_T("SS_ȭ�󱸺�_�ڵ�")};
	
	CString data[cnt];
	pDM_IDX->GetCellData(alias,cnt,data,nIdx_IDX);
	pDM_WORK->SetCellData(alias,cnt,data,nIdx_WORK);
		

	//�����켱����,���å�� 
	CString strREAD_FIRST_YN;
	CString strREG_BOOK_CNT;
	
	if(1 == nACQ_TYPE)
	{
		strREAD_FIRST_YN = pDM_ACQ->GetCellData(_T("SP_�����켱����"),0);
		strREG_BOOK_CNT = pDM_ACQ->GetCellData(_T("SP_���å��"),0);
	} 
	else 
	{
		strREAD_FIRST_YN = pDM_ACQ->GetCellData(_T("SD_�����켱����"),0);
		strREG_BOOK_CNT = pDM_ACQ->GetCellData(_T("SD_���å��"),0);
	}

	pDM_WORK->SetCellData(_T("UDF_�����켱����"),strREAD_FIRST_YN,nIdx_WORK);
	pDM_WORK->SetCellData(_T("UDF_���å��"),strREG_BOOK_CNT,nIdx_WORK);
	

	return 0;
}

INT CSeApi::MAKE_DB_WORK_by_IDX_ACQ(CESL_DataMgr* pDM_IDX,INT nIdx_IDX,CESL_DataMgr* pDM_WORK,INT nIdx_WORK,CESL_DataMgr* pDM_ACQ, INT nACQ_TYPE)
{
	
	//���� - �� ���� 	
	const INT cnt = 11;
	CString alias[cnt] = {
		_T("ISS_��KEY"),
		_T("ISS_�����ȣ"),
		_T("SS_��ϱ���_�ڵ�"),
		_T("SS_�̿����ѱ���_�ڵ�"),
		_T("SS_��������_�ڵ�"),
		_T("SS_�谡��ġ_�ڵ�"),
		_T("SS_�з���ȣ����_�ڵ�"),
		_T("SS_��������"),
		_T("ISS_���ı���_�ڵ�"),
		_T("ISS_��ü����_�ڵ�"),
		_T("SS_ȭ�󱸺�_�ڵ�")};
	
	CString data[cnt];
	pDM_IDX->GetCellData(alias,cnt,data,nIdx_IDX);
	pDM_WORK->SetCellData(alias,cnt,data,nIdx_WORK);
		

	//�����켱����,���å�� 
	CString strREAD_FIRST_YN;
	CString strREG_BOOK_CNT;
	
	if(1 == nACQ_TYPE)
	{
		strREAD_FIRST_YN = pDM_ACQ->GetCellData(_T("SP_�����켱����"),0);
		strREG_BOOK_CNT = pDM_ACQ->GetCellData(_T("SP_���å��"),0);
	} 
	else 
	{
		strREAD_FIRST_YN = pDM_ACQ->GetCellData(_T("SD_�����켱����"),0);
		strREG_BOOK_CNT = pDM_ACQ->GetCellData(_T("SD_���å��"),0);
	}

	pDM_WORK->SetCellData(_T("UDF_�����켱����"),strREAD_FIRST_YN,nIdx_WORK);
	pDM_WORK->SetCellData(_T("UDF_���å��"),strREG_BOOK_CNT,nIdx_WORK);


	return 0;
}

INT CSeApi::MAKE_VOL_by_IDX_WORK(CESL_DataMgr* pDM_IDX,
								 INT nIdx_IDX,
								 CESL_DataMgr* pDM_WORK,
								 INT nIdx_WORK,
								 CESL_DataMgr* pDM_VOL,
								 CMarc* pMarc,
								 CString strVOLUME_CLASS)
{
	pDM_VOL->InsertRow(-1);
	CopyDMToDMByAlias(pDM_WORK,nIdx_WORK,pDM_VOL,0);
	
	//REC_KEY
	CString strVOL_KEY = _T("");
	pDM_VOL->MakeRecKey(strVOL_KEY);
	pDM_VOL->SetCellData(_T("SV_��KEY"),strVOL_KEY,0);

	//��KEY
	CString strSPECIES_KEY = _T("");
	strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_��KEY"),nIdx_WORK);
	pDM_VOL->SetCellData(_T("SV_��KEY"),strSPECIES_KEY,0);
	
	
	//���α�ȣ��
	CString strVOL_TITLE = _T("");
	CString strOTHER_VOL_TITLE = _T("");
	CString strIDX_VOL_TITLE = _T("");
	strVOL_TITLE = pDM_WORK->GetCellData(_T("SV_��ȣ��"),nIdx_WORK);
	strOTHER_VOL_TITLE = pDM_WORK->GetCellData(_T("SV_��������ȣ��"),nIdx_WORK);
	CArray<CString,CString> arrayList;
	arrayList.Add(strVOL_TITLE);
	arrayList.Add(strOTHER_VOL_TITLE);
	strIDX_VOL_TITLE = MakeKeyWord(arrayList);
	pDM_VOL->SetCellData(_T("SV_���α�ȣ��"),strIDX_VOL_TITLE,0);


	//������ȣ�Ϸ� ��ȣ(üũ�� ���� �� ����) 
	CString strLAST_VOL_SORT_NO = _T("");
	INT nLAST_VOL_SORT_NO;
	strLAST_VOL_SORT_NO = pDM_IDX->GetCellData(_T("SS_������ȣ�Ϸù�ȣ"),nIdx_IDX);
	nLAST_VOL_SORT_NO = _ttoi(strLAST_VOL_SORT_NO.GetBuffer(0));
	nLAST_VOL_SORT_NO += 10;
	strLAST_VOL_SORT_NO.Format(_T("%d"),nLAST_VOL_SORT_NO);
	pDM_VOL->SetCellData(_T("SV_��ȣ�Ϸù�ȣ"),strLAST_VOL_SORT_NO,0);
	pDM_IDX->SetCellData(_T("SS_������ȣ�Ϸù�ȣ"),strLAST_VOL_SORT_NO,nIdx_IDX); 

	
	//��ȣ����
	pDM_VOL->SetCellData(_T("SV_��ȣ����"),_T("V"),0);


	//��ȣ����
	pDM_VOL->SetCellData(_T("SV_��ȣ����"),strVOLUME_CLASS,0);
	
	
	//�����۾�, ������ �۾�
	pDM_VOL->SetCellData(_T("SV_�����۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),0);
	pDM_VOL->SetCellData(_T("SV_�������۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),0);


	return 0;
}

INT CSeApi::MAKE_ACQ_VOL_by_IDX_ACQ_WORK(CESL_DataMgr* pDM_ACQ,INT nACQ_TYPE, CESL_DataMgr* pDM_WORK, INT nIdx_WORK, CESL_DataMgr* pDM_ACQ_VOL,CString strCHECKIN_STATUS)
{
	pDM_ACQ_VOL->InsertRow(-1);
	CopyDMToDMByAlias(pDM_WORK,nIdx_WORK,pDM_ACQ_VOL,0);
	
	//REC_KEY
	CString strACQ_VOL_KEY = _T("");
	pDM_ACQ->MakeRecKey(strACQ_VOL_KEY);
	pDM_ACQ_VOL->SetCellData(_T("SA_���Ա��к���KEY"),strACQ_VOL_KEY,0);
	
	//�Լ����� 
	pDM_ACQ_VOL->SetCellData(_T("SA_�Լ�����"),strCHECKIN_STATUS,0);


	//�����۾�, ������ �۾�
	pDM_ACQ_VOL->SetCellData(_T("SA_�����۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),0);
	pDM_ACQ_VOL->SetCellData(_T("SA_�������۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),0);


	return 0;
}

INT CSeApi::MAKE_BOOK_by_ACQ_WORK(CESL_DataMgr* pDM_ACQ,
								  INT nACQ_TYPE, 
								  CESL_DataMgr* pDM_WORK,
								  INT nIdx_WORK, 
								  CESL_DataMgr* pDM_BOOK, 
								  INT nACQ_BOOK_CNT,
								  INT nMISS_BOOK_CNT,
								  CString strACQ_YEAR,
					              CString strRECEIPT_NO,
								  BOOL bIsDepartMngUse,
								  CESL_DataMgr* pDM_DEPART) 
{
	
	//��KEY
	CString strSPECIES_KEY = _T("");
	strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_��KEY"),nIdx_WORK);
	
	//��ϱ���
	CString strREG_CODE = _T("");
	strREG_CODE = pDM_WORK->GetCellData(_T("SS_��ϱ���_�ڵ�"),nIdx_WORK);

	//�̿����ѱ���
	CString strUSE_LIMIT_CODE = _T("");
	strUSE_LIMIT_CODE = pDM_WORK->GetCellData(_T("SS_�̿����ѱ���_�ڵ�"),nIdx_WORK);
	
	//��������
	CString strMANAGE_CODE = _T("");
	strMANAGE_CODE = pDM_WORK->GetCellData(_T("SS_��������_�ڵ�"),nIdx_WORK);
	
	//�谡��ġ�ڵ�
	CString strSHELF_LOC_CODE = _T("");
	strSHELF_LOC_CODE = pDM_WORK->GetCellData(_T("SS_�谡��ġ_�ڵ�"),nIdx_WORK);
	
	//�з���ȣ����
	CString strCLASS_NO_TYPE = _T("");
	strCLASS_NO_TYPE = pDM_WORK->GetCellData(_T("SS_�з���ȣ����_�ڵ�"),nIdx_WORK);
	
	//���Ա��� 
	CString strACQ_TYPE;
	strACQ_TYPE.Format(_T("%d"),nACQ_TYPE);
	
	//�Է��� 
	CString strINPUT_DATE;
	strINPUT_DATE = GetTodayDate();
//	COleDateTime t = COleDateTime::GetCurrentTime();
	//Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());	
	//strINPUT_DATE = GetTodayDate();
//	strINPUT_DATE.Format(_T("TO_DATE(%04d/%02d/%02d %02d:%02d:%02d),'YYYY/MM/DD HH24:MI:SS')"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());	

	//�Լ��� 
	CString strACQ_DATE;
	strACQ_DATE = pDM_WORK->GetCellData(_T("UDF_�Լ���"),nIdx_WORK);
	if(strACQ_DATE.IsEmpty()) strACQ_DATE = GetTodayDate();
	
	
	//�ΰ� ���� ����
	CString strQuery;
	CString strResult;
	CString strTRANSFER_YN;
	//++2008.10.17 UPDATE BY PWR {{++
	// ���սý��� ����
	strQuery =	_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
				_T("WHERE GROUP_1='L' AND GROUP_2='����' AND CLASS_ALIAS='�ǽð�����ΰ��뿩��' ")
				_T("AND MANAGE_CODE=UDF_MANAGE_CODE");
	// strQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '����' AND CLASS_ALIAS='�ǽð�����ΰ��뿩��'");
	//--2008.10.17 UPDATE BY PWR --}}
	pDM_BOOK->GetOneValueQuery(strQuery,strResult);
	
	if(_T("Y") == strResult)
		strTRANSFER_YN = _T("N");
	else
		strTRANSFER_YN = _T("Y");

	//å�� 
	INT nTotolBook = nACQ_BOOK_CNT + nMISS_BOOK_CNT;
	
	//�����۾�, ������ �۾�
	CString strWorkLog = _T("");
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	
	CString strREC_KEY;
	//å�� ����
	for(INT i=0;i<nTotolBook;i++)
	{
		pDM_BOOK->InsertRow(-1);
		pDM_BOOK->MakeRecKey(strREC_KEY);		

		//����
		pDM_BOOK->SetCellData(_T("SB_åKEY"),strREC_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_��KEY"),strSPECIES_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_���Ա���"),strACQ_TYPE,i);
		pDM_BOOK->SetCellData(_T("SB_��ϱ���_�ڵ�"),strREG_CODE,i);
		pDM_BOOK->SetCellData(_T("SB_�̿����ѱ���_�ڵ�"),strUSE_LIMIT_CODE,i);
		pDM_BOOK->SetCellData(_T("SB_��������"),strMANAGE_CODE,i);
		
		if(!bIsDepartMngUse)
			pDM_BOOK->SetCellData(_T("SB_�谡��ġ_�ڵ�"),strSHELF_LOC_CODE,i);
		
		pDM_BOOK->SetCellData(_T("SB_�з���ȣ����_�ڵ�"),strCLASS_NO_TYPE,i);
		pDM_BOOK->SetCellData(_T("SB_�Է���"),strINPUT_DATE,i);
		pDM_BOOK->SetCellData(_T("SB_�����۾�"),strWorkLog,i);
		pDM_BOOK->SetCellData(_T("SB_�������۾�"),strWorkLog,i);
		pDM_BOOK->SetCellData(_T("SB_����å����"),_T("N"),i); //default
		pDM_BOOK->SetCellData(_T("SB_���̺���±���"),_T("1"),i); //default
		pDM_BOOK->SetCellData(_T("SB_�弭����"),_T("N"),i); //default
		pDM_BOOK->SetCellData(_T("SB_�ΰ迩��"),strTRANSFER_YN,i); //manage value

	}
	
	
	//�ڷ� ����(WORKING_STATUS) ó��-->���� ��ȣ ó�� �߰�   
	
	//���ڷ��� ���� ���� 
	CString strBINDING_YN;
	strBINDING_YN = pDM_WORK->GetCellData(_T("SS_��������"),nIdx_WORK);
	

	//���Ա����� �����켱����,���å��  
	CString strREAD_FIRST_YN,strREG_BOOK_CNT;
	strREAD_FIRST_YN = pDM_WORK->GetCellData(_T("UDF_�����켱����"),nIdx_WORK);
	strREG_BOOK_CNT = pDM_WORK->GetCellData(_T("UDF_���å��"),nIdx_WORK);

	
	INT nREG_BOOK_CNT = 0;
	if(!strREG_BOOK_CNT.IsEmpty()) nREG_BOOK_CNT = _ttoi(strREG_BOOK_CNT.GetBuffer(0));
	
	CString strLAST_SERIAL_NO = _T("");
	
	if(0 == nACQ_BOOK_CNT) //1) ��ȣ�ڷ� 
	{
		for(INT i=0;i<nTotolBook;i++)
		{
			if(1 == nACQ_TYPE) pDM_BOOK->SetCellData(_T("SB_�ڷ����"),_T("SEA112N"),i);  //���� ��ȣ   	
			else pDM_BOOK->SetCellData(_T("SB_�ڷ����"),_T("SEA212N"),i);	 //���� ��ȣ 
			pDM_BOOK->SetCellData(_T("SB_å����"),_T("M"),i); //��ȣå 
		}

	}
	else // 2)�Լ��� �ڷᰡ ���� ���  
	{
		if(_T("Y") == strREAD_FIRST_YN) //���� �켱 ���� 
		{
			BOOL IsAssigned = FALSE;

			for(INT i=0;i<nTotolBook;i++)
			{
				if(!IsAssigned) {
					pDM_BOOK->SetCellData(_T("SB_�ڷ����"),_T("SEL111O"),i);   //�ǽð�   	
					pDM_BOOK->SetCellData(_T("SB_å����"),_T("C"),i); //�Ϲ�å 
					pDM_BOOK->SetCellData(_T("SB_�Լ���"),strACQ_DATE,i); 
					pDM_BOOK->SetCellData(_T("SB_���Գ⵵"),strACQ_YEAR,i); 
					pDM_BOOK->SetCellData(_T("SB_������ȣ"),strRECEIPT_NO,i); 
					pDM_BOOK->SetCellData(_T("SB_�ǽð��ΰ���"),strACQ_DATE,i); 
					pDM_BOOK->SetCellData(_T("SB_�����ΰ���"),strACQ_DATE,i); 
					pDM_BOOK->SetCellData(_T("SB_����ΰ���"),strACQ_DATE,i); 
					strLAST_SERIAL_NO = GetReceiptSerialNo(pDM_BOOK, nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
					pDM_BOOK->SetCellData(_T("SB_�����Ϸù�ȣ"),strLAST_SERIAL_NO,i); 
				}

				if(IsAssigned)	{
				
					if(1 == nACQ_TYPE) pDM_BOOK->SetCellData(_T("SB_�ڷ����"),_T("SEA112N"),i);  //���� ��ȣ     	
					else pDM_BOOK->SetCellData(_T("SB_�ڷ����"),_T("SEA212N"),i);	//���� ��ȣ 
					pDM_BOOK->SetCellData(_T("SB_å����"),_T("M"),i); //��ȣå 	
				}

				if(nACQ_BOOK_CNT == i+1) IsAssigned = TRUE;
			}
		
		}
		else //���� �켱 ���θ� ������ �ʾ��� ��� 
		{
			BOOL IsAssigned = FALSE;

			for(INT i=0;i<nTotolBook;i++)
			{
				
				if(!IsAssigned) {
				
					if(nREG_BOOK_CNT>0) //��� å�� ��ŭ 
					{
						if(_T("Y") == strBINDING_YN) //�������� �ľ�
						{
							pDM_BOOK->SetCellData(_T("SB_�ڷ����"),_T("SEB111N"),i);   //����å   	
							pDM_BOOK->SetCellData(_T("SB_å����"),_T("C"),i); //�Ϲ�å 
						}
						else
						{
							pDM_BOOK->SetCellData(_T("SB_�ڷ����"),_T("SER111N"),i);   //���å 	
							pDM_BOOK->SetCellData(_T("SB_å����"),_T("C"),i); //�Ϲ�å 
						}
						nREG_BOOK_CNT--;
					}
					else //�ǽð� 
					{
						pDM_BOOK->SetCellData(_T("SB_�ڷ����"),_T("SEL111O"),i);   //�ǽð�   	
						pDM_BOOK->SetCellData(_T("SB_å����"),_T("C"),i); //�Ϲ�å 
					}

					pDM_BOOK->SetCellData(_T("SB_�Լ���"),strACQ_DATE,i); 
					pDM_BOOK->SetCellData(_T("SB_���Գ⵵"),strACQ_YEAR,i); 
					pDM_BOOK->SetCellData(_T("SB_������ȣ"),strRECEIPT_NO,i); 
					strLAST_SERIAL_NO = GetReceiptSerialNo(pDM_BOOK, nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
					pDM_BOOK->SetCellData(_T("SB_�����Ϸù�ȣ"),strLAST_SERIAL_NO,i); 
					pDM_BOOK->SetCellData(_T("SB_�ǽð��ΰ���"),strACQ_DATE,i); 
					pDM_BOOK->SetCellData(_T("SB_�����ΰ���"),strACQ_DATE,i); 
					pDM_BOOK->SetCellData(_T("SB_����ΰ���"),strACQ_DATE,i); 

				}
						
				if(IsAssigned)	{
						if(1 == nACQ_TYPE) pDM_BOOK->SetCellData(_T("SB_�ڷ����"),_T("SEA112N"),i);  //���� ��ȣ     	
						else pDM_BOOK->SetCellData(_T("SB_�ڷ����"),_T("SEA212N"),i);	//���� ��ȣ 
						pDM_BOOK->SetCellData(_T("SB_å����"),_T("M"),i); //��ȣå 	
					}

				if(nACQ_BOOK_CNT == i+1) IsAssigned = TRUE;
				
			}
		}
	}


	//�μ��� �������
	if(bIsDepartMngUse)
	{
		CString strResultCode;
		CString strBookType;
		INT ids = -1;
		
		for(i=0;i<nTotolBook;i++)
		{

			strBookType = pDM_BOOK->GetCellData(_T("SB_å����"),i); 
			if(_T("C") != strBookType) continue;

			ids = GetNextPriorityShelfLocCode(strResultCode,
											  strSHELF_LOC_CODE,
											  pDM_DEPART,
											  pDM_BOOK);

			if(ids<0) continue;

			pDM_BOOK->SetCellData(_T("SB_�谡��ġ_�ڵ�"),strResultCode,i);

		}


	}

	

	return 0;
}

INT CSeApi::MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(CESL_DataMgr* pDM_PURCHASE, CESL_DataMgr* pDM_SUB_HISTORY, INT nRowIdx)
{
	
	//���԰���KEY
	CString strtmp = _T("");
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_���԰���KEY"),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("���԰���KEY"),strtmp,nRowIdx);
	strtmp = _T("");

	//����ó_�ڵ� 
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_����ó_�ڵ�"),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("����ó_�ڵ�"),strtmp,nRowIdx);
	strtmp = _T("");


	//�������� 
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_��������"),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("��������"),strtmp,nRowIdx);
	strtmp = _T("");


	//���������� 
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_����������"),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("����������"),strtmp,nRowIdx);
	strtmp = _T("");


	//���������� 
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_����������"),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("����������"),strtmp,nRowIdx);
	strtmp = _T("");

	
	//����μ� 
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_����μ�"),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("����μ�"),strtmp,nRowIdx);
	strtmp = _T("");


	return 0;
}

INT CSeApi::MAKE_DROP_ACCESSION_NO_by_BOOK(CESL_DataMgr* pDM_OUT,CESL_DataMgr* pDM_BOOK,CESL_DataMgr* pDM_DROP_NO)
{
	
	CString strREG_CODE,strREG_NO,strALL_REG_NO,strACCESSIOM_NO,strACCESSION_YEAR,strREC_KEY;
	CArray<CString,CString> RemoveAliasList;
	INT nREG_NO = 0;
	INT nRowIdx = 0;
	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	for(INT i=0;i<pDM_BOOK->GetRowCount();i++)
	{
		strREG_CODE = _T("");
		strACCESSIOM_NO = _T("");
		strACCESSION_YEAR = _T("");

		strALL_REG_NO = pDM_BOOK->GetCellData(_T("��Ϲ�ȣ"),i);
		if(strALL_REG_NO.IsEmpty()) continue;
		
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		strREG_CODE = strALL_REG_NO.Left(2);
// 		nREG_NO = _ttoi(strALL_REG_NO.Mid(2).GetBuffer(0));
// 		strREG_NO.Format(_T("%d"),nREG_NO);

		strREG_CODE = strALL_REG_NO.Left(nRegCodeLength);
		nREG_NO = _ttoi(strALL_REG_NO.Mid(nRegCodeLength).GetBuffer(0));
		strREG_NO.Format(_T("%d"),nREG_NO);
		//=====================================================
		

		//��� ��ȣ�� ����� ��ȣ������ üũ�Ѵ�.(��ϱ����� �����̸� ����� ��ȣ)
		if(IsDigitString(strREG_CODE)) continue;

		strACCESSIOM_NO = pDM_BOOK->GetCellData(_T("���ι�ȣ"),i);
		strACCESSION_YEAR = pDM_BOOK->GetCellData(_T("�����ۼ��⵵"),i);
		
		pDM_DROP_NO->InsertRow(-1);
		nRowIdx = pDM_DROP_NO->GetRowCount()-1;
		pDM_DROP_NO->MakeRecKey(strREC_KEY); 
		pDM_DROP_NO->SetCellData(_T("REC_KEY"),strREC_KEY,nRowIdx);
		pDM_DROP_NO->SetCellData(_T("�������±���"),_T("SE"),nRowIdx);
		pDM_DROP_NO->SetCellData(_T("�����ۼ��⵵"),strACCESSION_YEAR,nRowIdx);
		pDM_DROP_NO->SetCellData(_T("���ι�ȣ"),strACCESSIOM_NO,nRowIdx);
		
		pDM_DROP_NO->SetCellData(_T("��ϱ���"),strREG_CODE,nRowIdx);
		pDM_DROP_NO->SetCellData(_T("��Ϲ�ȣ"),strREG_NO,nRowIdx);
		pDM_DROP_NO->SetCellData(_T("�����۾�"),strWorkLog,nRowIdx);
		pDM_DROP_NO->SetCellData(_T("�������۾�"),strWorkLog,nRowIdx);

		pDM_OUT->InitDBFieldData();
		RemoveAliasList.RemoveAll();
		pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_DROP_NO, _T("CO_DROP_ACCESSION_NO_TBL"), nRowIdx , pDM_OUT);
	}


	return 0;
}

INT CSeApi::CheckAppendix(CESL_DataMgr* pDM_WORK,
					  INT nIdx_WORK,
					  INT nACQ_TYPE,
					  CESL_DataMgr* pDM_APPENDIX, 
					  CESL_DataMgr* pDM_BOOK)
{
	
	//�ηϿ����� �Ǿ� �ִ����� Ȯ���� ������ �������� �Է��� �������� �����Ѵ�.
	INT nRowCnt = pDM_APPENDIX->GetRowCount();
	if(0 == nRowCnt) return 0;
	
	CString strVOL_TITLE = _T("");
	strVOL_TITLE = pDM_WORK->GetCellData(_T("SV_��ȣ��"),nIdx_WORK);

	CSeAppedixExpDlg dlg(m_pMgr);
	dlg.m_strVOL_TITLE = strVOL_TITLE;
	dlg.DoModal();
	
	CString strUDF;
	for(INT i=0;i<nRowCnt;i++)
	{
		strUDF = _T("");
		strUDF = pDM_APPENDIX->GetCellData(_T("UDF_����"),i);
		if(_T("I") != strUDF) continue;
		
		MAKE_APPENDIX_BOOK_by_WORK(pDM_WORK,nIdx_WORK,nACQ_TYPE,pDM_APPENDIX,i,pDM_BOOK);
	}

	return 0;
}

INT CSeApi::MAKE_APPENDIX_BOOK_by_WORK(CESL_DataMgr* pDM_WORK,
									   INT nIdx_WORK,
									   INT nACQ_TYPE,
									   CESL_DataMgr* pDM_APPENDIX,
									   INT nIdx_APPEN,
									   CESL_DataMgr* pDM_BOOK,
									   BOOL bOption)
{
	//�۾� DM ���� �η�å��, �η� ������ �����Ѵ�. 
	
	////////////////////////////////////�η� å 
	//��KEY
	CString strSPECIES_KEY = _T("");
	strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_��KEY"),nIdx_WORK);
	
	//��KEY
	CString strVOL_KEY = _T("");
	strVOL_KEY = pDM_WORK->GetCellData(_T("SV_��KEY"),nIdx_WORK);

	//��ϱ���
	CString strREG_CODE = _T("");
	strREG_CODE = pDM_WORK->GetCellData(_T("SS_��ϱ���_�ڵ�"),nIdx_WORK);

	//�̿����ѱ���
	CString strUSE_LIMIT_CODE = _T("");
	strUSE_LIMIT_CODE = pDM_WORK->GetCellData(_T("SS_�̿����ѱ���_�ڵ�"),nIdx_WORK);
	
	//��������
	CString strMANAGE_CODE = _T("");
	strMANAGE_CODE = pDM_WORK->GetCellData(_T("SS_��������_�ڵ�"),nIdx_WORK);
	
	//�谡��ġ�ڵ�
	CString strSHELF_LOC_CODE = _T("");
	strSHELF_LOC_CODE = pDM_WORK->GetCellData(_T("SS_�谡��ġ_�ڵ�"),nIdx_WORK);
	
	//�з���ȣ����
	CString strCLASS_NO_TYPE = _T("");
	strCLASS_NO_TYPE = pDM_WORK->GetCellData(_T("SS_�з���ȣ����_�ڵ�"),nIdx_WORK);
	
	//���Ա��� 
	CString strACQ_TYPE;
	strACQ_TYPE.Format(_T("%d"),nACQ_TYPE);
	
	//���Գ⵵ 
	CString strACQ_YEAR;
	strACQ_YEAR = GetCurrentYear();


	//�Է��� 
	CString strINPUT_DATE;
	strINPUT_DATE = GetTodayDate();

	//�����۾�, ������ �۾�
	CString strWorkLog = _T("");
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	CString strBOOK_KEY;
	INT idx_book;
	
	//�۾���
	CString strWorker = m_pMgr->m_pInfo->USER_ID;

	pDM_BOOK->InsertRow(-1);
	idx_book = pDM_BOOK->GetRowCount()-1;

	pDM_BOOK->MakeRecKey(strBOOK_KEY);		
	pDM_BOOK->SetCellData(_T("SB_åKEY"),strBOOK_KEY,idx_book);
	pDM_BOOK->SetCellData(_T("SB_��KEY"),strSPECIES_KEY,idx_book);
	pDM_BOOK->SetCellData(_T("SB_��KEY"),strVOL_KEY,idx_book);
	pDM_BOOK->SetCellData(_T("SB_���Ա���"),strACQ_TYPE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_���Գ⵵"),strACQ_YEAR,idx_book);
	pDM_BOOK->SetCellData(_T("SB_��ϱ���_�ڵ�"),strREG_CODE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_�̿����ѱ���_�ڵ�"),strUSE_LIMIT_CODE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_��������"),strMANAGE_CODE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_�谡��ġ_�ڵ�"),strSHELF_LOC_CODE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_�з���ȣ����_�ڵ�"),strCLASS_NO_TYPE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_�Է���"),strINPUT_DATE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_�����۾�"),strWorkLog,idx_book);
	pDM_BOOK->SetCellData(_T("SB_�������۾�"),strWorkLog,idx_book);
	pDM_BOOK->SetCellData(_T("SB_����å����"),_T("N"),idx_book); //default
	pDM_BOOK->SetCellData(_T("SB_���̺���±���"),_T("1"),idx_book); //default
	pDM_BOOK->SetCellData(_T("SB_�弭����"),_T("N"),idx_book); //default
	pDM_BOOK->SetCellData(_T("SB_å����"),_T("A"),idx_book); //�η�å 
	pDM_BOOK->SetCellData(_T("SB_�ΰ迩��"),_T("Y"),idx_book); //default 
	pDM_BOOK->SetCellData(_T("SB_�۾���"),strWorker,idx_book); 


	////////////////////////////////////�η� 
	CString strAPPENDIX_KEY;
	pDM_APPENDIX->MakeRecKey(strAPPENDIX_KEY);	
	pDM_APPENDIX->SetCellData(_T("�η�KEY"),strAPPENDIX_KEY,nIdx_APPEN);
	pDM_APPENDIX->SetCellData(_T("åKEY"),strBOOK_KEY,nIdx_APPEN);
	pDM_APPENDIX->SetCellData(_T("��KEY"),strVOL_KEY,nIdx_APPEN);
	pDM_APPENDIX->SetCellData(_T("�����۾�"),strWorkLog,nIdx_APPEN);
	pDM_APPENDIX->SetCellData(_T("�������۾�"),strWorkLog,nIdx_APPEN);
	
	if(!bOption)
	{
		pDM_APPENDIX->SetCellData(_T("���ົ��Ͽ���"),_T("N"),nIdx_APPEN); //default
	}

	return 0;
}

INT CSeApi::UpdateExpectData(CESL_DataMgr* pDM_OUT,
							 CESL_DataMgr* pDM_ACQ,
							 INT nACQ_TYPE,
							 CESL_DataMgr* pDM_WORK,
							 INT nIdx_WORK, 
							 CArray<CString,CString>& arrayVolExpKeyList,
							 CArray<CString,CString>& arrayOtherVolExpKeyList)
{

	CString strPUB_EXP_KEY,strACQ_EXP_KEY,strPUB_EXP_TYPE,strACQ_EXP_TYPE,strTBL_NAME,strQuery;

	if(1 == nACQ_TYPE)
	{
		strPUB_EXP_KEY = pDM_ACQ->GetCellData(_T("SP_���࿹������KEY"),0); 
		strACQ_EXP_KEY = pDM_ACQ->GetCellData(_T("SP_�Լ���������KEY"),0);
		strPUB_EXP_TYPE = pDM_ACQ->GetCellData(_T("SP_���࿹������"),0);
		strACQ_EXP_TYPE = pDM_ACQ->GetCellData(_T("SP_�Լ���������"),0);
	}
	else
	{
		strPUB_EXP_KEY = pDM_ACQ->GetCellData(_T("SD_���࿹������KEY"),0); 
		strACQ_EXP_KEY = pDM_ACQ->GetCellData(_T("SD_�Լ���������KEY"),0);
		strPUB_EXP_TYPE = pDM_ACQ->GetCellData(_T("SD_���࿹������"),0);
		strACQ_EXP_TYPE = pDM_ACQ->GetCellData(_T("SD_�Լ���������"),0);
	}
	
	CString strNEW_PUB_EXP_DATE,strNEW_ACQ_EXP_DATE,strACQ_LAST_COUNT,strPUB_LAST_COUNT;
	strNEW_PUB_EXP_DATE = pDM_WORK->GetCellData(_T("UDF_�������࿹����"),nIdx_WORK);
	strNEW_ACQ_EXP_DATE = pDM_WORK->GetCellData(_T("UDF_�����Լ�������"),nIdx_WORK);
	strACQ_LAST_COUNT = pDM_WORK->GetCellData(_T("UDF_�Լ������Ȱ���"),nIdx_WORK);
	strPUB_LAST_COUNT = pDM_WORK->GetCellData(_T("UDF_���࿹���Ȱ���"),nIdx_WORK);
	if(strACQ_LAST_COUNT.IsEmpty()) strACQ_LAST_COUNT = _T("0");
	if(strPUB_LAST_COUNT.IsEmpty()) strPUB_LAST_COUNT = _T("0");


	//������ 
	if(!strNEW_PUB_EXP_DATE.IsEmpty())
	{
		strTBL_NAME = GetExpTable(strPUB_EXP_TYPE);
		strQuery.Format(_T("UPDATE %s SET EXP_START_DATE = '%s',LAST_ACQ_EXP_COUNT = %s WHERE REC_KEY = %s;"),strTBL_NAME,strNEW_PUB_EXP_DATE,strPUB_LAST_COUNT,strPUB_EXP_KEY);
		pDM_OUT->InitDBFieldData();
		pDM_OUT->AddFrame(strQuery);
	}
	
	//�Լ��� 
	if(!strNEW_ACQ_EXP_DATE.IsEmpty())
	{
		strTBL_NAME = GetExpTable(strACQ_EXP_TYPE);
		strQuery.Format(_T("UPDATE %s SET EXP_START_DATE = '%s',LAST_ACQ_EXP_COUNT = %s WHERE REC_KEY = %s;"),strTBL_NAME,strNEW_ACQ_EXP_DATE,strACQ_LAST_COUNT,strACQ_EXP_KEY);
		pDM_OUT->InitDBFieldData();
		pDM_OUT->AddFrame(strQuery);
	}
	
	CString strACQ_EXP_NO;
	INT ids = 0; 

	//��ȣ
	strACQ_EXP_NO = pDM_WORK->GetCellData(_T("UDF_��ȣ�������ۼ�"),nIdx_WORK);
	if(!strACQ_EXP_NO.IsEmpty()) {
		ids = UpdateVolExpectData(pDM_OUT,strACQ_EXP_NO,_T("1"),arrayVolExpKeyList);
		if(ids<0) return -1;
	}

		
	//������ ��ȣ 
	strACQ_EXP_NO = pDM_WORK->GetCellData(_T("UDF_��������ȣ�������ۼ�"),nIdx_WORK);
	if(!strACQ_EXP_NO.IsEmpty()) {
		ids = UpdateVolExpectData(pDM_OUT,strACQ_EXP_NO,_T("2"),arrayOtherVolExpKeyList);		
		if(ids<0) return -1;
	}

	return 0;
}

INT CSeApi::UpdateVolExpectData(CESL_DataMgr* pDM_OUT, CString strACQ_EXP_NO, CString strFORMAT_TYPE, CArray<CString,CString>& arrayVolExpKeyList)
{
	
	INT nPos = strACQ_EXP_NO.Find(_T(","));
	CString strREC_KEY;
	CString strQuery;
	

	if(nPos<0) //LEVEL �� �ϳ� �ִ� ��� 
	{
		INT nSize = arrayVolExpKeyList.GetSize();
	
		if(nSize == 0) 
		{
			AfxMessageBox(_T("��ȣ ���� KEY�� ã�� �� �����ϴ�"));
			return -1;
		}
		
		strREC_KEY = arrayVolExpKeyList.GetAt(0);
		
		strACQ_EXP_NO.TrimLeft();
		strACQ_EXP_NO.TrimRight();
		strREC_KEY.TrimLeft();
		strREC_KEY.TrimRight();
		
		strQuery.Format(_T("UPDATE SE_VOL_EXP_TBL SET ACQ_EXP_NO = '%s' WHERE REC_KEY = %s AND FORMAT_TYPE = %s;"),strACQ_EXP_NO,strREC_KEY,strFORMAT_TYPE);
		pDM_OUT->InitDBFieldData();
		pDM_OUT->AddFrame(strQuery);
	
	}
	else //LEVEL�� ������ �ִ� ��� 
	{
		CString tmp;
		
		for(INT i=0;i<arrayVolExpKeyList.GetSize(); i++)
		{
			INT nLevel, nLimit;

			if(nPos<0) 
			{
				tmp = strACQ_EXP_NO;
			}
			else
			{
				tmp = strACQ_EXP_NO.Left(nPos);
				strACQ_EXP_NO = strACQ_EXP_NO.Mid(nPos+1);
			}
			
			tmp.TrimLeft();
			tmp.TrimRight();

			strREC_KEY = arrayVolExpKeyList.GetAt(i);
		
			strREC_KEY.TrimLeft();
			strREC_KEY.TrimRight();
			
			CString sQuery, sLevel, sLimit;
			sQuery.Format(_T("SELECT INC_UNIT_LEVEL FROM SE_VOL_EXP_TBL WHERE REC_KEY = %s AND FORMAT_TYPE = %s"),strREC_KEY,strFORMAT_TYPE);
			pDM_OUT->GetOneValueQuery(sQuery,sLevel);
			
			sQuery.Format(_T("SELECT INC_UNIT_LIMIT FROM SE_VOL_EXP_TBL WHERE REC_KEY = %s AND FORMAT_TYPE = %s"),strREC_KEY,strFORMAT_TYPE);
			pDM_OUT->GetOneValueQuery(sQuery,sLimit);

			if(!sLevel.IsEmpty())
			{
				nLevel = _ttoi(sLevel);
				nLimit = _ttoi(sLimit);
				if( nLimit > nLevel)
					nLevel++;
				else if( nLimit == nLevel )
					nLevel = 1;

				sLevel.Format(_T("%d"),nLevel);

				strQuery.Format(_T("UPDATE SE_VOL_EXP_TBL SET ACQ_EXP_NO = '%s', INC_UNIT_LEVEL = '%s' WHERE REC_KEY = %s AND FORMAT_TYPE = %s;"),tmp,sLevel,strREC_KEY,strFORMAT_TYPE);
			}
			else
				strQuery.Format(_T("UPDATE SE_VOL_EXP_TBL SET ACQ_EXP_NO = '%s' WHERE REC_KEY = %s AND FORMAT_TYPE = %s;"),tmp,strREC_KEY,strFORMAT_TYPE);
			pDM_OUT->InitDBFieldData();
			pDM_OUT->AddFrame(strQuery);

			nPos = strACQ_EXP_NO.Find(_T(","));
		}
	}


	return 0;
}

INT CSeApi::SetInsertDefaultValue_SPECIES(CESL_DataMgr* pDM_SPECIES, CMarc* pMarc, INT nRowIdx)
{
	
	//default value
	pDM_SPECIES->SetCellData(_T("SS_�Է±���"),_T("0"),nRowIdx); //ȭ���Է�
	pDM_SPECIES->SetCellData(_T("SS_�Է���"),GetTodayDate(),nRowIdx); //�Է���
	pDM_SPECIES->SetCellData(_T("SS_�����۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	pDM_SPECIES->SetCellData(_T("SS_�������۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);


	
	//�����ȣ setting
	CString strControlCode,strControlNo;
	strControlCode = pDM_SPECIES->GetCellData(_T("SS_�����ڷᱸ��_�ڵ�"),nRowIdx);
	strControlNo = GetControlNo(pDM_SPECIES, strControlCode);
	pDM_SPECIES->SetCellData(_T("SS_�����ȣ"),strControlNo,nRowIdx);
	


	//��ũ �����ȣ 
	//���Ӽ���
	m_pMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pMgr->m_pInfo->pMarcMgr->SetItem(pMarc,_T("001*"),strControlNo);

	

	//���Գ⵵ 
	pDM_SPECIES->SetCellData(_T("SS_���Գ⵵"),GetCurrentYear(),nRowIdx);
	

	
	//�۾����±���
	pDM_SPECIES->SetCellData(_T("SS_�۾����±���"),_T("1"),nRowIdx); //�۾� ������ �Է�
	

	return 0;
}

INT CSeApi::SetInsertDefaultValue_IDX(CESL_DataMgr* pDM_IDX, INT nRowIdx)
{
	//�����а�(�۾����±���)
	pDM_IDX->SetCellData(_T("ISS_�����а�"),_T("0"),nRowIdx); //�۾� ������ �Է�
		
	return 0;
}

INT CSeApi::SetInsertDefaultValue_PURCHASE(CESL_DataMgr* pDM_PURCHASE, INT nRowIdx)
{
	CString strtmp = _T("");
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_�Լ�å��"),nRowIdx);
	if(strtmp.IsEmpty()) pDM_PURCHASE->SetCellData(_T("SP_�Լ�å��"),_T("1"), nRowIdx); 

	strtmp = _T("");
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_���å��"),nRowIdx);
	if(strtmp.IsEmpty()) pDM_PURCHASE->SetCellData(_T("SP_���å��"),_T("0"), nRowIdx);

	strtmp = _T("");
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_��������"),nRowIdx);
	if(strtmp.IsEmpty()) pDM_PURCHASE->SetCellData(_T("SP_��������"),_T("0"), nRowIdx);

	pDM_PURCHASE->SetCellData(_T("SP_�����۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	pDM_PURCHASE->SetCellData(_T("SP_�������۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);

	return 0;
}

INT CSeApi::SetInsertDefaultValue_DONATE(CESL_DataMgr *pDM_DONATE, INT nRowIdx)
{
	CString strtmp = _T("");
    
	strtmp = _T("");
	strtmp = pDM_DONATE->GetCellData(_T("SD_�Լ�å��"),nRowIdx);
	if(strtmp.IsEmpty()) pDM_DONATE->SetCellData(_T("SD_�Լ�å��"),_T("1"), nRowIdx); 

	strtmp = _T("");
	strtmp = pDM_DONATE->GetCellData(_T("SD_���å��"),nRowIdx);
	if(strtmp.IsEmpty()) pDM_DONATE->SetCellData(_T("SD_���å��"),_T("0"), nRowIdx);

	pDM_DONATE->SetCellData(_T("SD_�����۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	pDM_DONATE->SetCellData(_T("SD_�������۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);

	return 0;
}

INT CSeApi::SetInsertDefaultValue_SUB_HISTORY(CESL_DataMgr* pDM_SUB_HISTORY, INT nRowIdx)
{
	
	pDM_SUB_HISTORY->SetCellData(_T("�����۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("�������۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);

	return 0;
}


INT CSeApi::SetModifyDefaultValue_SPECIES(CESL_DataMgr* pDM_SPECIES, INT nRowIdx)
{
	pDM_SPECIES->SetCellData(_T("SS_�������۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	return 0;
}

INT CSeApi::SetModifyDefaultValue_IDX(CESL_DataMgr* pDM_IDX, INT nRowIdx)
{
	
	return 0;
}

INT CSeApi::SetModifyDefaultValue_PURCHASE(CESL_DataMgr* pDM_PURCHASE,INT nRowIdx)
{
	pDM_PURCHASE->SetCellData(_T("SP_�������۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	return 0;
}

INT CSeApi::SetModifyDefaultValue_SUB_HISTORY(CESL_DataMgr* pDM_SUB_HISTORY,INT nRowIdx)
{
	pDM_SUB_HISTORY->SetCellData(_T("�������۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);

	return 0;
}

INT CSeApi::SetModifyDefaultValue_DONATE(CESL_DataMgr *pDM_DONATE, INT nRowIdx)
{
	pDM_DONATE->SetCellData(_T("SD_�������۾�"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	return 0;
}

INT CSeApi::FileLog(CString str)
{
	FILE *fDebug = _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));

	FILE * f = _tfopen(_T("_SE_API.log"), _T("a+b"));
	if (!f) return -1;
	
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE �����۾�
		// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
		fseek(f, 0, SEEK_END);		
		if( 0 == ftell(f) )
		{
			/*UNCHANGE*/fputc(0xFF, f);
			/*UNCHANGE*/fputc(0xFE, f);
		}
		// ------------------------------------------------------------------------------
		
	_ftprintf(f, _T("%s\r\n"), str);
	fclose(f);
	return 0;
}

CString CSeApi::GetTodayDate()
{
	CString result;
	COleDateTime t = COleDateTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

CString CSeApi::GetCurrentYear()
{
	CString result;
	COleDateTime t = COleDateTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;

}

INT CSeApi::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray)
{
	INT pos;
	CString strTmpData;	
	CIndexTool* pIndexTool	= NULL;
	CIndex* pIndex			= NULL;
	
	pIndexTool = (CIndexTool*)m_pMgr->m_pInfo->pIndexTool;
	if( !pIndexTool )	return -1;

	pIndex = pIndexTool->GetIndex();
	if(!pIndex) return -1;

	if( strSource.GetLength() == 0 )	return 0;
	strSource.TrimLeft();	strSource.TrimRight();
	pos = strSource.Find(_T(" "), 0);	
	while( pos > 0 )
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+1);
		strSource.TrimLeft();	strSource.TrimRight();

		pIndex->MakeIndex(strTmpData);
		if( CheckIsHave(pTargetArray, strTmpData) == FALSE )		pTargetArray->Add(strTmpData);
		
		pos = strSource.Find(_T(" "), 0);
	}

	pIndex->MakeIndex(strSource);
	if( CheckIsHave(pTargetArray, strSource) == FALSE )		pTargetArray->Add(strSource);

	return 0;
}

BOOL CSeApi::CheckIsHave(CStringArray *codeValue, CString code)
{

	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;

}

INT CSeApi::ParsingKeyString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray)
{
	
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos > 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+1);
		
		strTmpData.TrimLeft();
		strTmpData.TrimRight();

		if(!strTmpData.IsEmpty()) pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}

	strSource.TrimLeft();
	strSource.TrimRight();
	if(!strSource.IsEmpty()) pTargetArray->Add(strSource);
	
	return 0;
}

CString CSeApi::GetExpTable(CString strType)
{

	CString strTBL_NAME = _T("");

	switch(strType.GetAt(0))
	{
	case 'M':
		strTBL_NAME = _T("SE_MONTH_EXP_TBL");
		break;
	case 'W':
		strTBL_NAME = _T("SE_WEEK_EXP_TBL");
		break;
	case 'D':
		strTBL_NAME = _T("SE_DAY_EXP_TBL");
		break;
	case 'E':
		strTBL_NAME = _T("SE_ETC_EXP_TBL");
		break;
	default:
		break;
	}

	return strTBL_NAME;
}

CString CSeApi::GetControlNo(CESL_DataMgr* pDM, CString strControlCode)
{
	//���� ��ȣ�� ��� �Լ� 
	pDM->StartFrame();
	
	//�ش��
	CString strYear;
	strYear = GetCurrentYear();

	CString strLastNumber,strQuery,strControlNo;

	
	strQuery.Format(_T("LOCK TABLE CO_LAST_NUMBER_TBL IN EXCLUSIVE MODE;"));
	pDM->ExecuteQuery(strQuery,0,FALSE);
	

	//�ش�⵵�� �ش� �����ȣ�� SELECT�ϰ� ���� ���� INSERT�Ѵ� 
	//++2008.10.17 UPDATE BY PWR {{++
	// ���սý��� ����
	strQuery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					 _T("WHERE KIND_CODE='CON_NO' AND PUBLISH_FORM_CODE='CO' AND DIVISION_VALUE='%s' ")
					 _T("AND MAKE_YEAR='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), strControlCode, strYear );
	// strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'CON_NO' AND PUBLISH_FORM_CODE = 'CO' AND DIVISION_VALUE = '%s' AND MAKE_YEAR = '%s'"),strControlCode,strYear);
	//--2008.10.17 UPDATE BY PWR --}}
	INT ids =  pDM->GetOneValueQuery(strQuery,strLastNumber,FALSE);

	INT nControlNo = 0;
	if(!strLastNumber.IsEmpty())
	{
		nControlNo = _ttoi(strLastNumber.GetBuffer(0));
	}

	nControlNo++;

	if( -4007 == ids)	//�˻������ ������ INSERT 
	{	
		
		if(!strControlCode.IsEmpty())
		{
			strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR,DIVISION_VALUE,LAST_NUMBER,CREATE_DATE,CREATE_WORKER,FIRST_WORK,MANAGE_CODE)")
							_T("VALUES	(%s.NEXTVAL,'CON_NO','CO','%s','%s',1,'%s','%s','%s',UDF_MANAGE_CODE);"), DEFAULT_SEQ_NAME, strYear,strControlCode,GetTodayDate(),m_pMgr->m_pInfo->USER_ID,m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI));
			pDM->ExecuteQuery(strQuery,1);
		}
		
	}
	else	//������ UPDATE
	{
		//++2008.10.17 UPDATE BY PWR {{++
		// ���սý��� ����
		strQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d, LAST_WORK='%s' ")
						 _T("WHERE MAKE_YEAR='%s' AND KIND_CODE='CON_NO' AND DIVISION_VALUE='%s' ")
						 _T("AND MANAGE_CODE=UDF_MANAGE_CODE;")
						 , nControlNo, m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI), strYear, strControlCode );
		// strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %d, LAST_WORK = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = 'CON_NO' AND DIVISION_VALUE = '%s';"),nControlNo,m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),strYear,strControlCode);
		//--2008.10.17 UPDATE BY PWR --}}
		pDM->ExecuteQuery(strQuery,1);

	}

	strControlNo.Format(_T("%s%s%05d"),strControlCode,strYear,nControlNo);

	pDM->EndFrame();

	return strControlNo;
}


CString CSeApi::GetLastReceiptNo(CESL_DataMgr* pDM, INT nACQ_TYPE, CString strACQ_YEAR)
{
	//������ ������ȣ�� ��� �Լ� 
	
	// 2005-04-11 �ּ�ó��
	// pDM->StartFrame();
	CString strYear;
	strYear = strACQ_YEAR;

	CString strLastNumber,strQuery,strReceiptNo,strKindCode;

	if(1 == nACQ_TYPE)
		strKindCode = _T("CK1_NO"); //���� ������ȣ 
	else if(2 == nACQ_TYPE)
		strKindCode = _T("CK2_NO"); //���� ������ȣ 
	
	// 2005-04-11 �ּ�ó��
	// strQuery.Format(_T("LOCK TABLE CO_LAST_NUMBER_TBL IN EXCLUSIVE MODE;"));
	// pDM->ExecuteQuery(strQuery,0,FALSE);
	

	//�ش�⵵�� ������ ������ȣ�� SELECT�ϰ� ���� ���� INSERT�Ѵ� 
	//++2008.10.17 UPDATE BY PWR {{++
	// ���սý��� ����
	strQuery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					 _T("WHERE KIND_CODE='%s' AND PUBLISH_FORM_CODE='SE' AND MAKE_YEAR='%s' ")
					 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strKindCode, strYear );
	// strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = '%s' AND PUBLISH_FORM_CODE = 'SE' AND MAKE_YEAR = '%s'"),strKindCode,strYear);
	//--2008.10.17 UPDATE BY PWR --}}
	INT ids =  pDM->GetOneValueQuery(strQuery,strLastNumber,FALSE);
	
	INT nReceiptNo = 0;
	if(!strLastNumber.IsEmpty())
	{
		nReceiptNo = _ttoi(strLastNumber.GetBuffer(0));
		nReceiptNo++;
	}
	else
	{
		nReceiptNo = 1;
	}

	// 2005-04-11 �ּ�ó��
	//if( -4007 == ids)	//�˻������ ������ INSERT 
	//{
	//	strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR,LAST_NUMBER,CREATE_DATE,CREATE_WORKER,FIRST_WORK)")
	//					_T("VALUES	(%s.NEXTVAL,'%s','SE','%s',1,'%s','%s','%s');"), DEFAULT_SEQ_NAME,strKindCode, strYear,GetTodayDate(),m_pMgr->m_pInfo->USER_ID,m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI));
	//	pDM->ExecuteQuery(strQuery,1);
	//}
	//else	//������ UPDATE
	//{
	//	strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %d, LAST_WORK = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = '%s';"),nReceiptNo,m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),strYear,strKindCode);
	//	pDM->ExecuteQuery(strQuery,1);
	//}
	//
	//pDM->EndFrame();
	
	strReceiptNo.Format(_T("%d"),nReceiptNo);

	return strReceiptNo;
}

  
CString CSeApi::GetReceiptSerialNo(CESL_DataMgr* pDM, INT nACQ_TYPE, CString strACQ_YEAR, CString strRECEIPT_NO)
{
	/*
	//���� �Ϸ� ��ȣ�� ��� �Լ� 
	CString strLAST_SERIAL_NO = _T("");
	
	pDM->StartFrame();
	
	CString strQuery;
	
	strQuery.Format(_T("LOCK TABLE SE_RECEIPT_TBL IN EXCLUSIVE MODE;"));
	pDM->ExecuteQuery(strQuery,0,FALSE);
	

	CString strDBLastNo = _T("");

	//�ش� ������ȣ�� ���� ��ȣ�� SELECT�Ѵ�. 
	strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM SE_RECEIPT_TBL WHERE ACQ_CODE = '%d' AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s"),nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
	INT ids =  pDM->GetOneValueQuery(strQuery,strDBLastNo,FALSE);
	INT nLastNo = _ttoi(strDBLastNo.GetBuffer(0));
	nLastNo++;


	strQuery.Format(_T("UPDATE SE_RECEIPT_TBL SET LAST_SERIAL_NO = %d WHERE ACQ_CODE = '%d' AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s;"),nLastNo,nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
	pDM->ExecuteQuery(strQuery,1);
	pDM->EndFrame();
	
	strLAST_SERIAL_NO.Format(_T("%d"),nLastNo);
	return strLAST_SERIAL_NO;
	*/

	//modified by loveisverb 2004/09/08
	//���� ����


	//�ش� ������ȣ�� ���� ��ȣ�� SELECT�Ѵ�. 
	CString strQuery;
	CString strDBLastNo,strLAST_SERIAL_NO;
	INT nLastNo = 0;
	//++2008.10.17 UPDATE BY PWR {{++
	// ���սý��� ����
	strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM SE_RECEIPT_TBL ")
					_T("WHERE ACQ_CODE='%d' AND ACQ_YEAR='%s' AND RECEIPT_NO=%s AND MANAGE_CODE=UDF_MANAGE_CODE")
					, nACQ_TYPE, strACQ_YEAR, strRECEIPT_NO );
	// strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM SE_RECEIPT_TBL WHERE ACQ_CODE = '%d' AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s"),nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
	//--2008.10.17 UPDATE BY PWR --}}
	INT ids =  pDM->GetOneValueQuery(strQuery,strDBLastNo);
	if(!strDBLastNo.IsEmpty()) nLastNo = _ttoi(strDBLastNo.GetBuffer(0));
	nLastNo++;


	pDM->StartFrame();
	//++2008.10.17 UPDATE BY PWR {{++
	// ���սý��� ����
	strQuery.Format(_T("UPDATE SE_RECEIPT_TBL SET LAST_SERIAL_NO=%d ")
					_T("WHERE ACQ_CODE='%d' AND ACQ_YEAR='%s' AND RECEIPT_NO=%s AND MANAGE_CODE=UDF_MANAGE_CODE;")
					,nLastNo, nACQ_TYPE, strACQ_YEAR, strRECEIPT_NO );
	// strQuery.Format(_T("UPDATE SE_RECEIPT_TBL SET LAST_SERIAL_NO = %d WHERE ACQ_CODE = '%d' AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s;"),nLastNo,nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
	//--2008.10.17 UPDATE BY PWR --}}
	pDM->AddFrame(strQuery);
	pDM->SendFrame();
	pDM->EndFrame();

	strLAST_SERIAL_NO.Format(_T("%d"),nLastNo);
	return strLAST_SERIAL_NO;
}

CString CSeApi::GetSerialWorkNo(CESL_DataMgr* pDM, CString strSEQ_NO_KEY, CString strWorkNo)
{
	// �۾� �Ϸ� ��ȣ�� ��� �Լ� 
	CString strSerialWorkNo = _T("");
	INT nSerialNo = 0;
	CString strQuery;

	pDM->StartFrame();

	
	strQuery.Format(_T("LOCK TABLE SE_PURCHASE_WORK_NO_TBL IN EXCLUSIVE MODE;"));
	pDM->ExecuteQuery(strQuery,0,FALSE);
	

	strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM SE_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY = %s AND WORK_NO = %s"),strSEQ_NO_KEY,strWorkNo);
	INT ids =  pDM->GetOneValueQuery(strQuery,strSerialWorkNo,FALSE);

	if(ids<0)
	{
		pDM->EndFrame();
		return _T("");
	}
	
	nSerialNo = _ttoi(strSerialWorkNo.GetBuffer(0));
	nSerialNo++;
	
	strQuery.Format(_T("UPDATE SE_PURCHASE_WORK_NO_TBL SET LAST_SERIAL_NO = %d WHERE PURCHASE_SEQ_NO_KEY = %s AND WORK_NO = %s;"),nSerialNo,strSEQ_NO_KEY,strWorkNo);
	pDM->ExecuteQuery(strQuery,1);

	pDM->EndFrame();
	strSerialWorkNo.Format(_T("%d"),nSerialNo);


	return strSerialWorkNo;
}

CString CSeApi::GetLastWorkNo(CESL_DataMgr* pDM, CString strSEQ_NO_KEY)
{
	//������ȣ�� ������ �۾� ��ȣ ��� �Լ� 
	CString strLastWorkNo = _T("");
	INT nLastWorkNo = 0;
	CString strQuery;
	
	pDM->StartFrame();
	
	
	strQuery.Format(_T("LOCK TABLE SE_PURCHASE_SEQ_NO_TBL IN EXCLUSIVE MODE;"));
	pDM->ExecuteQuery(strQuery,0,FALSE);
	

	strQuery.Format(_T("SELECT LAST_WORK_NO FROM SE_PURCHASE_SEQ_NO_TBL WHERE REC_KEY = %s"),strSEQ_NO_KEY);
	INT ids =  pDM->GetOneValueQuery(strQuery,strLastWorkNo,FALSE);

	if(ids<0)
	{
		pDM->EndFrame();
		return _T("");
	}
	
	nLastWorkNo = _ttoi(strLastWorkNo.GetBuffer(0));
	nLastWorkNo++;
	
	strQuery.Format(_T("UPDATE SE_PURCHASE_SEQ_NO_TBL SET LAST_WORK_NO = %d WHERE REC_KEY = %s;"),nLastWorkNo,strSEQ_NO_KEY);
	pDM->ExecuteQuery(strQuery,1);
	
	pDM->EndFrame();
	strLastWorkNo.Format(_T("%d"),nLastWorkNo);

	return strLastWorkNo;
}

INT CSeApi::UpdateSpeciesMarcbyCMarc(CESL_DataMgr *pDM_SPECIES, CString strSpecies_key, CMarc *pMarc)
{

	if (!pDM_SPECIES) return-1;
	if (!pMarc) return -2;

	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->StartFrame();
	
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddDBFieldData(_T("MARC"), _T("STRING"), strStreamMarc, -1);
	
	pDM_SPECIES->MakeUpdateFrame(pDM_SPECIES->TBL_NAME,_T("REC_KEY"),_T("NUMERIC"),strSpecies_key);

	INT ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	return ids;

}

INT CSeApi::APPLY_IndexItem(CESL_DataMgr *pDM_IDX, CMarc *pMarc, INT nRowIdx) 
{
	//���Ӽ���
	m_pMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));


	//INDEX TOOL KIT �̿� �⺻ ���ξ� ���� 
	INT nIndexToolKit = 0;
	CString IndexToolKit[MAX_TEMP_STRING_ARRAY_SIZE];
	CString IndexToolKitCommand[MAX_TEMP_STRING_ARRAY_SIZE];
		
	
	//�⺻ ����  (ISS_��ü�˻�����ξ� ����)
	IndexToolKit[nIndexToolKit] = _T("ISS_�˻��뺻ǥ��");			IndexToolKitCommand[nIndexToolKit] = _T("��������");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_Ű����");					IndexToolKitCommand[nIndexToolKit] = _T("Ű��������");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�˻���������");			IndexToolKitCommand[nIndexToolKit] = _T("������");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�˻��������");			IndexToolKitCommand[nIndexToolKit] = _T("������");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�����˻��뺻ǥ��");		IndexToolKitCommand[nIndexToolKit] = _T("�����˻��뺻ǥ��");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�����˻��������");		IndexToolKitCommand[nIndexToolKit] = _T("�����˻��������");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�����˻������⵵");		IndexToolKitCommand[nIndexToolKit] = _T("�����˻������⵵");	nIndexToolKit++;
	
	//2004.9.2 loveisverb����
	//IndexToolKit[nIndexToolKit] = _T("ISS_ǥ������");				IndexToolKitCommand[nIndexToolKit] = _T("ǥ������INFO");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_ǥ������");				IndexToolKitCommand[nIndexToolKit] = _T("ǥ������");			nIndexToolKit++;

	IndexToolKit[nIndexToolKit] = _T("ISS_�����ڻ���");				IndexToolKitCommand[nIndexToolKit] = _T("�����ڻ���");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�����ڻ���");				IndexToolKitCommand[nIndexToolKit] = _T("�����ڻ���");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_����⵵����");			IndexToolKitCommand[nIndexToolKit] = _T("����⵵����");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_��ǥ��");					IndexToolKitCommand[nIndexToolKit] = _T("��ǥ��");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_������");					IndexToolKitCommand[nIndexToolKit] = _T("������");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_������");					IndexToolKitCommand[nIndexToolKit] = _T("������");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_����⵵2");				IndexToolKitCommand[nIndexToolKit] = _T("�����");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_������");					IndexToolKitCommand[nIndexToolKit] = _T("������");				nIndexToolKit++;

	
	//�̿��� ��ȣ �� 
	IndexToolKit[nIndexToolKit] = _T("ISS_ISBN");					IndexToolKitCommand[nIndexToolKit] = _T("ǥ�غ�ȣ_ISBN");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_STRN");					IndexToolKitCommand[nIndexToolKit] = _T("ǥ�غ�ȣ_STRN");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_RNSTRN");					IndexToolKitCommand[nIndexToolKit] = _T("ǥ�غ�ȣ_RNSTRN");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_����������ȣ");			IndexToolKitCommand[nIndexToolKit] = _T("ǥ�غ�ȣ_����������ȣ");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_���۱ǵ�Ϲ�ȣ");			IndexToolKitCommand[nIndexToolKit] = _T("ǥ�غ�ȣ_���۱ǵ�Ϲ�ȣ");				nIndexToolKit++;	
	IndexToolKit[nIndexToolKit] = _T("ISS_�з���ȣKDCP");			IndexToolKitCommand[nIndexToolKit] = _T("KDCP");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�з���ȣKDC");			IndexToolKitCommand[nIndexToolKit] = _T("KDC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�з���ȣLC");				IndexToolKitCommand[nIndexToolKit] = _T("LC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�з���ȣNDC");			IndexToolKitCommand[nIndexToolKit] = _T("NDC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�з���ȣCEC");			IndexToolKitCommand[nIndexToolKit] = _T("CEC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�з���ȣCOC");			IndexToolKitCommand[nIndexToolKit] = _T("COC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�з���ȣCWC");			IndexToolKitCommand[nIndexToolKit] = _T("CWC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�з���ȣDDC");			IndexToolKitCommand[nIndexToolKit] = _T("DDC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�з���ȣUDC");			IndexToolKitCommand[nIndexToolKit] = _T("UDC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�ѱ����к�ȣ");			IndexToolKitCommand[nIndexToolKit] = _T("�ѱ����к�ȣ");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�ѱ����α����ȣ");		IndexToolKitCommand[nIndexToolKit] = _T("���α����ȣ");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_�������");				IndexToolKitCommand[nIndexToolKit] = _T("�������");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_��๮���");				IndexToolKitCommand[nIndexToolKit] = _T("��๮���");			nIndexToolKit++;

	CString strData;	

	CIndexTool *pIndexTool = NULL;
	pIndexTool = (CIndexTool*)m_pMgr->m_pInfo->pIndexTool;
	if(!pIndexTool) return -1;

	pIndexTool->Init(m_pMgr->m_pInfo->pMarcMgr,pMarc);

	for(INT i=0;i<nIndexToolKit;i++) {
		strData = _T("");
		pIndexTool->GetIndex(IndexToolKitCommand[i], strData);
		pDM_IDX->SetCellData(IndexToolKit[i], strData, nRowIdx);
	}

	//008 TAG ���� �Է�
	//�������
	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("008@6"),strData);
	pDM_IDX->SetCellData(_T("ISS_�������"), strData, nRowIdx);
	strData = _T("");
	
	//â����,�󰣳�  
	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("008@7-10"),strData);
	pDM_IDX->SetCellData(_T("ISS_â����"), strData, nRowIdx);
	strData = _T("");

	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("008@11-14"),strData);
	pDM_IDX->SetCellData(_T("ISS_�󰣳�"), strData, nRowIdx);
	strData = _T("");
	

	//�����(�����Ⱓ(��ĭ)�� ��� _T("_") ó���� ���ش� 
	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("008@18"),strData);
	
	strData.TrimLeft();
	strData.TrimRight();

	if(strData.IsEmpty())
	{
		pDM_IDX->SetCellData(_T("ISS_�����"), _T("_"), nRowIdx);
	}
	else
	{
		pDM_IDX->SetCellData(_T("ISS_�����"), strData, nRowIdx);
	}
	
	strData = _T("");
	

	//���Լ�
	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("008@19"),strData);
	pDM_IDX->SetCellData(_T("ISS_���Լ�"), strData, nRowIdx);
	strData = _T("");
	
	//�̿��� ���� 
	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("022$a"),strData);
	pDM_IDX->SetCellData(_T("ISS_ISSN"), strData, nRowIdx);
	strData = _T("");
	
	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("030$a"),strData);
	pDM_IDX->SetCellData(_T("ISS_CODEN"), strData, nRowIdx);
	strData = _T("");


	//��ü�˻�����ξ�
	//IDX_ALL_ITEM�ʵ带 ä��� ���ؼ� �˻��뺻ǥ��, ������, ������, Ű���� �� �����Ѵ�.
	CString strIdxTitle,strIdxAuthor,strIdxPublihser,strIdxKeyword;
	pDM_IDX->GetCellData(_T("ISS_�˻��뺻ǥ��"), nRowIdx, strIdxTitle);
	pDM_IDX->GetCellData(_T("ISS_�˻���������"), nRowIdx, strIdxAuthor);
	pDM_IDX->GetCellData(_T("ISS_�˻��������"), nRowIdx, strIdxPublihser);
	pDM_IDX->GetCellData(_T("ISS_Ű����"), nRowIdx, strIdxKeyword);

	// IDX_ALL_ITEM�ʵ� ���� - �˻��뺻ǥ��, �˻�������, �˻��� ������, �˻�����ξ�
	CString strIdxAllItem ;
	strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublihser + _T(" ") + strIdxKeyword;
	CStringArray TargetArray;
	TargetArray.RemoveAll();
	ParsingString(_T(" "), strIdxAllItem, &TargetArray);
	INT cnt = TargetArray.GetSize();
	strIdxAllItem.Empty();
	for(i=0 ; i<cnt ; i++)
	{
		if( strIdxAllItem.IsEmpty() == TRUE )	
		{
			strIdxAllItem = TargetArray.GetAt(i);
		}
		else 
		{
			strIdxAllItem += _T(" ") + TargetArray.GetAt(i);
		}
	}

	pDM_IDX->SetCellData(_T("ISS_��ü�˻�����ξ�"), strIdxAllItem, nRowIdx);


	return 0;
}

INT CSeApi::APPLY_IndexItem_Book(CESL_DataMgr* pDM_OUT, CString strSpeciesKey)
{

	//������ å�������� ���� ����
	CString strQuery;

	//��� ����(REG_CODE)
	CString strRegCode;
	strRegCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'REG_CODE') FROM DUAL)"),strSpeciesKey);
	

	//�̿����ѱ���(USE_LIMIT_CODE)
	CString strUseLimitCode;
	strUseLimitCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'USE_LIMIT_CODE') FROM DUAL)"),strSpeciesKey);
	

	//��������(MANAGE_CODE)
	CString strManageCode;
	strManageCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'MANAGE_CODE') FROM DUAL)"),strSpeciesKey);


	//�ڷ��(IDX_PLACE)
	CString strIdxPlace;
	strIdxPlace.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'SHELF_LOC_CODE') FROM DUAL)"),strSpeciesKey);

	
	//��ġ��ȣ(IDX_SE_SHELF_CODE)
	CString strSeparateCode;
	strSeparateCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'SEPARATE_SHELF_CODE') FROM DUAL)"),strSpeciesKey);
	

	strQuery.Format(_T("UPDATE IDX_SE_TBL SET REG_CODE = %s, USE_LIMIT_CODE = %s,")
					_T(" MANAGE_CODE = %s, IDX_PLACE = %s, IDX_SE_SHELF_CODE = %s")
					_T(" WHERE REC_KEY = %s;"),
					strRegCode,strUseLimitCode,strManageCode,strIdxPlace,strSeparateCode,strSpeciesKey);
	

//	pDM_OUT->InitDBFieldData();
	pDM_OUT->AddFrame(strQuery);

	//���� ��ȭ ����
	SetIdxTmpSeTbl(pDM_OUT, strSpeciesKey, _T("I"));

	strQuery.Format(_T("UPDATE IDX_SE_TBL SET MANAGE_CODE = UDF_MANAGE_CODE ")
					_T("WHERE REC_KEY = %s;"),
					strSpeciesKey);	

//	pDM_OUT->InitDBFieldData();
	pDM_OUT->AddFrame(strQuery);

	return 0;
}

INT CSeApi::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}


CString CSeApi::MakeKeyWord(CArray <CString, CString> &IndexList)
{
	//�ش縮��Ʈ�� ���ڵ��� ��ĭ�� �����ϰ� ���ڸ� �ѱ۷� ����� ���� ��Ų��.
	INT nSize;
	CString strKEYWORD = _T("");
	nSize = IndexList.GetSize();
	if(0 == nSize) return  strKEYWORD;
	
	INT nCount = 0;
	CString strData;
	
	CIndexTool *pIndexTool = NULL;
	pIndexTool = (CIndexTool*)m_pMgr->m_pInfo->pIndexTool;
	if(!pIndexTool) return strKEYWORD;

	CIndex* pIndex = NULL;
	pIndex = pIndexTool->GetIndex();
	if(!pIndex) return strKEYWORD;


	for(INT i=0;i<nSize;i++) 
	{
		strData = _T("");
		strData = IndexList.GetAt(i);
		if(!strData.IsEmpty()) 
		{
			if(nCount) strKEYWORD += _T(" ");
			pIndex->TrimAll(strData);
			pIndex->RemoveSpecialChar(strData);
			pIndex->ConvertHanjaToHangul(strData);
			strKEYWORD += strData;
			nCount++;
		}
	}

	return strKEYWORD;
}

VOID CSeApi::MakeIndexWord(CString &strData)
{
	//�ش� �������� ������ �����, Ʈ���ϰ�, Ư����ȣ�� ������,  ���ڸ� �ѱ۷� �����.
	if(strData.IsEmpty()) return;

	strData.Replace(_T(" "),_T(""));

	CIndexTool *pIndexTool = NULL;
	pIndexTool = (CIndexTool*)m_pMgr->m_pInfo->pIndexTool;
	if(!pIndexTool) return;

	CIndex* pIndex = NULL;
	pIndex = pIndexTool->GetIndex();
	if(!pIndex) return;


	strData.MakeUpper();
	pIndex->TrimAll(strData);
	pIndex->RemoveSpecialChar(strData);
	pIndex->ConvertHanjaToHangul(strData);
}


VOID CSeApi::SetSerialMarcReader(CMarc* pMarc)
{
	//���� ��ũ MarcReader ����( 05 : n , 06 : a ,07 : s)
	
	CString strLeader;
	pMarc->GetLeader(strLeader);
	strLeader.SetAt(5,'n');
	strLeader.SetAt(6,'a');
	strLeader.SetAt(7,'s');
	pMarc->SetLeader(strLeader.GetBuffer(0));
	
}


INT CSeApi::BOOK_TRANSFER_TO_REAL_TIME(CESL_DataMgr* pDM, CString strBookKey)
{
	//�ǽð� �ΰ� ó�� 
	if(!pDM || strBookKey.IsEmpty())
	{
		AfxMessageBox(_T("�ǽð� �ΰ� ó��  ���� ������ ã�� �� �����ϴ�!"));
		return -1;
	}

	CString strQuery = _T("");
	CString strWorkLogMsg;
	strWorkLogMsg = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	strQuery.Format(_T("UPDATE SE_BOOK_TBL SET TRANSFER_YN = 'Y',IMM_LOC_TRANSFER_DATE = '%s',LAST_WORK = '%s' WHERE REC_KEY = %s;"),GetTodayDate(),strWorkLogMsg,strBookKey);
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();

	return ids;
}

INT CSeApi::BOOK_TRANSFER_TO_REG(CESL_DataMgr* pDM, CString strBookKey)
{
	//��ϴ��(SER111N) ó�� 
	if(!pDM || strBookKey.IsEmpty())
	{
		AfxMessageBox(_T("��ϴ�� ó�� ���� ������ ã�� �� �����ϴ�!"));
		return -1;
	}

	CString strQuery = _T("");
	CString strWorkLogMsg;
	strWorkLogMsg = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SER111N',TRANSFER_YN = 'Y',REG_TRANSFER_DATE = '%s', LAST_WORK = '%s' WHERE REC_KEY = %s;"),GetTodayDate(),strWorkLogMsg,strBookKey);
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();	

	return ids;
}

INT CSeApi::BOOK_TRANSFER_TO_BINDING(CESL_DataMgr* pDM, CString strBookKey)
{
	//�������(SEB111N) ó��
	if(!pDM || strBookKey.IsEmpty())
	{
		AfxMessageBox(_T("������� ó�� ���� ������ ã�� �� �����ϴ�!"));
		return -1;
	}


	CString strQuery = _T("");
	CString strWorkLogMsg;
	strWorkLogMsg = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SEB111N',TRANSFER_YN = 'Y',BINDING_TRANSFER_DATE = '%s',LAST_WORK = '%s' WHERE REC_KEY = %s;"),GetTodayDate(),strWorkLogMsg,strBookKey);
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();

	return ids;
}

INT CSeApi::BOOK_TRANSFER_TO_SHELF(CESL_DataMgr* pDM, 
								   CString strPrevWorkingStatus, 
								   CString strSpeciesKey, 
								   CString strVolKey, 
								   CString strBookKey)
{
	//�ǽð� �谡 ó��
	//�������� �����ϴ� api�� ����� ���� �� ��縦 �۾� -> �������� ����� ���´�
	if(!pDM || strPrevWorkingStatus.IsEmpty() || strSpeciesKey.IsEmpty() || strVolKey.IsEmpty() || strBookKey.IsEmpty())
	{
		AfxMessageBox(_T("�ǽð� �谡 ���� �ڷḦ ã�� �� �����ϴ�!"));
		return -1;
	}


	CString strQuery = _T("");
	CString strWorkLogMsg;
	strWorkLogMsg = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SEL112N',")
					_T("TRANSFER_YN = 'Y',")
					_T("PREV_WORKING_STATUS = '%s',")
					_T("SHELF_DATE = '%s',")
					_T("SHELF_WORKER = '%s',")
					_T("LAST_WORK = '%s' ")
					_T("WHERE REC_KEY = %s;"),strPrevWorkingStatus,GetTodayDate(),m_pMgr->m_pInfo->USER_ID,strWorkLogMsg,strBookKey);
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	

	CBL_SE_LOC_15 BL(m_pMgr);
	BL.SettingSpeciesField( strSpeciesKey , strVolKey );


	return ids;	
}

VOID CSeApi::SetIdxTmpSeTbl(CESL_DataMgr* pDM, CString strSpeciesKey, CString strMode)
{
	//���ο� ��ȭ�� ������ �� IDX_TMP_SE_TBL �� �������ش�
	if(!pDM || strSpeciesKey.IsEmpty()) return;
	
	CString strQuery;
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	strQuery.Format(_T("INSERT INTO IDX_TMP_SE_TBL (REC_KEY,SPECIES_KEY,IDX_WORK_FLAG,INPUT_DATE) ")
// 					_T("VALUES (%s.NEXTVAL,%s,'%s','%s');"),DEFAULT_SEQ_NAME,strSpeciesKey,strMode,GetTodayDate());
// 
// 	//pDM->InitDBFieldData();
// 	pDM->AddFrame(strQuery);
	//=====================================================
	
	

	if(ORA_CATSEARCH == m_pMgr->m_pInfo->GetSearchIndexType())
	{

	}
	else
	{
		//ecosearch�� ���� procedure
		strQuery.Format(_T("ECO$P_MAKE_SE$I(%s);"),strSpeciesKey);
		pDM->AddFrame(strQuery);
	}

}

VOID CSeApi::MakeEcoIndexVol(CESL_DataMgr* pDM, CString strVolKey, BOOL bIsInsert)
{

	if(ORA_CATSEARCH == m_pMgr->m_pInfo->GetSearchIndexType())
	{
			return;
	}
	else
	{
		//ecosearch�� ���� procedure

		CString strQuery;	
		pDM->InitDBFieldData();
		if(bIsInsert)
		{
			strQuery.Format(_T("ECO$P_MAKE_SEVOL$I(%s);"),strVolKey);
		}
		else 
		{
			strQuery.Format(_T("ECO$P_DELETE_SEVOL$I(%s);"),strVolKey);
		}

		pDM->AddFrame(strQuery);
	}
}

BOOL CSeApi::IsLightVersion(CESL_DataMgr* pDM)
{
	TCHAR szValue;
	
	szValue = m_pMgr->m_pInfo->GetKIIUPClientVersion();

	if('L' == szValue) return TRUE;

	return FALSE;
}

BOOL CSeApi::IsDepartMngUse(CESL_DataMgr* pDM)
{
	CString strQuery;
	CString strResult;

	strQuery.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'SEDEPARTMNG'"));

	pDM->GetOneValueQuery(strQuery,strResult);

	if(strResult.IsEmpty() || _T("N") == strResult) return FALSE;

	return TRUE;
}

BOOL CSeApi::IsCoverImageUse(CESL_DataMgr* pDM)
{
	CString strQuery;
	CString strResult;

	strQuery.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USECOVERIMG'"));

	pDM->GetOneValueQuery(strQuery,strResult);

	if(strResult.IsEmpty() || _T("N") == strResult) return FALSE;

	return TRUE;
}


BOOL CSeApi::IsDigitString(CString str)
{
	if(str.IsEmpty()) return TRUE;

	TCHAR * tmp = str.GetBuffer(0);
	INT size = str.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;

}

CString CSeApi::GetWorkingStatusDesc(CString strCode)
{
	CString code[64];
	CString desc[64];
	INT cnt = 0;

	code[cnt] = _T("SEA111N");	desc[cnt] = _T("�����Լ��ڷ�"); cnt++;
	code[cnt] = _T("SEA112N");	desc[cnt] = _T("���԰�ȣ�ڷ�"); cnt++;
	code[cnt] = _T("SEA211N");	desc[cnt] = _T("�����Լ��ڷ�"); cnt++;
	code[cnt] = _T("SEA212N");	desc[cnt] = _T("������ȣ�ڷ�"); cnt++;
	code[cnt] = _T("SEB111N");	desc[cnt] = _T("�����ΰ��ڷ�"); cnt++;
	code[cnt] = _T("SEB112N");	desc[cnt] = _T("���������ڷ�"); cnt++;
	code[cnt] = _T("SEB113N");	desc[cnt] = _T("���������ڷ�"); cnt++;	
	code[cnt] = _T("SEB114N");	desc[cnt] = _T("�����˼��ڷ�"); cnt++;	
	code[cnt] = _T("SEL111O");	desc[cnt] = _T("�ǽð��ΰ��ڷ�"); cnt++;	
	code[cnt] = _T("SEL112N");	desc[cnt] = _T("�ǽð��谡�ڷ�"); cnt++;	
	code[cnt] = _T("SEL113O");	desc[cnt] = _T("�ǽð�����ڷ�"); cnt++;	
	code[cnt] = _T("SER111N");	desc[cnt] = _T("����ΰ��ڷ�"); cnt++;
	code[cnt] = _T("SER112N");	desc[cnt] = _T("����ڷ�"); cnt++;	
	code[cnt] = _T("SER113O");	desc[cnt] = _T("�����ι�ȣ�ο��ڷ�"); cnt++;
	code[cnt] = _T("SEC111N");	desc[cnt] = _T("��������ڷ�"); cnt++;
	code[cnt] = _T("SEC112O");	desc[cnt] = _T("�����ΰ����ڷ�"); cnt++;
	code[cnt] = _T("SEL211O");	desc[cnt] = _T("�����ΰ��ڷ�"); cnt++;
	code[cnt] = _T("SEL212N");	desc[cnt] = _T("�谡�ڷ�"); cnt++;
	code[cnt] = _T("SEL213O");	desc[cnt] = _T("���������ڷ�"); cnt++;
	code[cnt] = _T("SEL214O");	desc[cnt] = _T("�ļ��ڷ�"); cnt++;
	code[cnt] = _T("SEL215O");	desc[cnt] = _T("�����ڷ�"); cnt++;
	code[cnt] = _T("SEL216O");	desc[cnt] = _T("�н��ڷ�"); cnt++;
	code[cnt] = _T("SEL217O");	desc[cnt] = _T("�������ڷ�"); cnt++;
	code[cnt] = _T("SEL311O");	desc[cnt] = _T("���ܴ����ڷ�"); cnt++;
	code[cnt] = _T("SEL312O");	desc[cnt] = _T("���������ڷ�"); cnt++;
	//=====================================================
	//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
	// 18.09.27 JMJ KOL.RED.2018.008
	code[cnt] = _T("SEL411O");	desc[cnt] = _T("����å�η����������ڷ�"); cnt++;
	code[cnt] = _T("SEL511O");	desc[cnt] = _T("���յ���Ÿ���ݳ�"); cnt++;
	//=====================================================
	code[cnt] = _T("SEL313O");	desc[cnt] = _T("Ư�������ڷ�"); cnt++;
	code[cnt] = _T("SEL314O");	desc[cnt] = _T("��޴����ڷ�"); cnt++;
    code[cnt] = _T("SEL315O");  desc[cnt] = _T("����ڷ�"); cnt++; 


	CString strDesc = _T("");
	for(INT i=0;i<cnt;i++)
	{
		if(code[i] == strCode)
		{
			strDesc = desc[i];
			break;
		}
	
	}

	return strDesc;
}

INT CSeApi::GetNextPriorityShelfLocCode(CString& strResult,
									    CString strDefaultCode,
									    CESL_DataMgr* pDM_DEPART,
									    CESL_DataMgr* pDM_BOOK,
									    CESL_DataMgr* pDM_BOOK_INSERT,
									    INT nMode)
{

	if(1 == nMode)
	{
		if(!pDM_DEPART || !pDM_BOOK) return -1;
	}
	else if(2 == nMode)
	{
		if(!pDM_DEPART || !pDM_BOOK || !pDM_BOOK_INSERT) return -1;
	}

	if(1 == nMode) //üũ��
	{
		INT nRowBook = pDM_BOOK->GetRowCount();
		INT nRowDepart = pDM_DEPART->GetRowCount();

		if(0 == nRowDepart || 0 == nRowBook) //�μ��� ������ �Ǿ����� ����
		{
			strResult = strDefaultCode;
			return 0;
		}

		strResult.Empty();
		CString strCode;
		CString strExpBookCnt;
		CString strCurCode;
		CString strBookType;
		INT nExpBookCnt = 0;
		INT nCurAssignCount = 0;

		for(INT i=0;i<nRowDepart;i++)
		{
			nExpBookCnt = 0;
			nCurAssignCount = 0;
			strCode = pDM_DEPART->GetCellData(_T("�μ��ڵ�"),i);
			strCode.TrimLeft();
			strCode.TrimRight();
			if(strCode.IsEmpty()) strCode = strDefaultCode;
			strExpBookCnt = pDM_DEPART->GetCellData(_T("�Լ�����å��"),i);
			if(!strExpBookCnt.IsEmpty()) nExpBookCnt = _ttoi(strExpBookCnt.GetBuffer(0));
			
			
			for(INT j=0;j<nRowBook;j++)
			{
				strBookType = pDM_BOOK->GetCellData(_T("SB_å����"),j);
				if(_T("C") != strBookType) continue;

				strCurCode = pDM_BOOK->GetCellData(_T("SB_�谡��ġ_�ڵ�"),j);
				if(strCode == strCurCode) nCurAssignCount++;
			}

			if(nCurAssignCount >= nExpBookCnt) 
			{
				continue;
			}
			else
			{
				strResult = strCode;
				return 0;
			}

		}	
	}
	else if(2 == nMode) //�Ǽ���
	{

		INT nRowBook = pDM_BOOK->GetRowCount();
		INT nRowBookInsert = pDM_BOOK_INSERT->GetRowCount();
		INT nRowDepart = pDM_DEPART->GetRowCount();
		INT nTotalBook = nRowBook + nRowBookInsert;

		if(0 == nRowDepart || 0 == nTotalBook) //�μ��� ������ �Ǿ����� ����
		{
			strResult = strDefaultCode;
			return 0;
		}

		strResult.Empty();
		CString strCode;
		CString strExpBookCnt;
		CString strCurCode;
		CString strBookType;
		INT nExpBookCnt = 0;
		INT nCurAssignCount = 0;

		for(INT i=0;i<nRowDepart;i++)
		{
			nExpBookCnt = 0;
			nCurAssignCount = 0;
			strCode = pDM_DEPART->GetCellData(_T("�μ��ڵ�"),i);
			strCode.TrimLeft();
			strCode.TrimRight();
			if(strCode.IsEmpty()) strCode = strDefaultCode;
			strExpBookCnt = pDM_DEPART->GetCellData(_T("�Լ�����å��"),i);
			if(!strExpBookCnt.IsEmpty()) nExpBookCnt = _ttoi(strExpBookCnt.GetBuffer(0));
			
			
			for(INT j=0;j<nRowBook;j++)
			{
				strBookType = pDM_BOOK->GetCellData(_T("SB_å����"),j);
				if(_T("C") != strBookType) continue;

				strCurCode = pDM_BOOK->GetCellData(_T("SB_�谡��ġ_�ڵ�"),j);
				if(strCode == strCurCode) nCurAssignCount++;
			}

			for(j=0;j<nRowBookInsert;j++)
			{
				strBookType = pDM_BOOK_INSERT->GetCellData(_T("SB_å����"),j);
				if(_T("C") != strBookType) continue;

				strCurCode = pDM_BOOK_INSERT->GetCellData(_T("SB_�谡��ġ_�ڵ�"),j);
				if(strCode == strCurCode) nCurAssignCount++;
			}

			if(nCurAssignCount >= nExpBookCnt) 
			{
				continue;
			}
			else
			{
				strResult = strCode;
				return 0;
			}

		}	

	}
	
	return -1;

}