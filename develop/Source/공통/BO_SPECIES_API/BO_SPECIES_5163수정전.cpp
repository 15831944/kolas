// BO_SPECIES.cpp: implementation of the CBO_SPECIES class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_SPECIES.h"
#include "MarcDefaultValueMgr.h"
#include "../COMMON_EBOOK_API/EBookUpNDownloadMgr.h"
#include "../COMMON_CONVERSION_MGR/PriceInfoConversionMgr.h"
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

CBO_SPECIES::CBO_SPECIES(CESL_Mgr *pParentMgr)
{
	m_nSpeciesType = 0;
	pMgr = pParentMgr;
	m_nOpenMode = 1100; 

	m_pDM_SPECIES = NULL;
	m_pDM_BOOK = NULL;
	m_pDM_PURCHASEINFO = NULL;
	m_pDM_IDX_BO = NULL;
	m_pDM_DELETE_BOOK = NULL;
	m_pDM_DELETE_SPECIES = NULL;
	m_pDM_DELETE_VOLUME = NULL;
	m_pDM_ACCU = NULL;
	m_pDM_EBook = NULL;

	nIndexMarcRelation = 0;
	IndexMarcRelation_INDEX[nIndexMarcRelation] = _T("IBS_�ǻ���");			IndexMarcRelation_MARC[nIndexMarcRelation] = _T("����");		nIndexMarcRelation++;
	IndexMarcRelation_INDEX[nIndexMarcRelation] = _T("IBS_�Ǽ���");			IndexMarcRelation_MARC[nIndexMarcRelation] = _T("����");		nIndexMarcRelation++;
	IndexMarcRelation_INDEX[nIndexMarcRelation] = _T("IBS_������");			IndexMarcRelation_MARC[nIndexMarcRelation] = _T("������");		nIndexMarcRelation++;
	IndexMarcRelation_INDEX[nIndexMarcRelation] = _T("IBS_������");			IndexMarcRelation_MARC[nIndexMarcRelation] = _T("������");		nIndexMarcRelation++;
	IndexMarcRelation_INDEX[nIndexMarcRelation] = _T("IBS_�����");			IndexMarcRelation_MARC[nIndexMarcRelation] = _T("�����");		nIndexMarcRelation++;
	IndexMarcRelation_INDEX[nIndexMarcRelation] = _T("IBS_�Ѽ���");			IndexMarcRelation_MARC[nIndexMarcRelation] = _T("�Ѽ�ǥ��");	nIndexMarcRelation++;
	IndexMarcRelation_INDEX[nIndexMarcRelation] = _T("IBS_�Ѽ�����");		IndexMarcRelation_MARC[nIndexMarcRelation] = _T("�Ѽ�����");	nIndexMarcRelation++;
	IndexMarcRelation_INDEX[nIndexMarcRelation] = _T("IBS_ũ��");			IndexMarcRelation_MARC[nIndexMarcRelation] = _T("ũ��");		nIndexMarcRelation++;
	IndexMarcRelation_INDEX[nIndexMarcRelation] = _T("IBS_�����");			IndexMarcRelation_MARC[nIndexMarcRelation] = _T("�����");		nIndexMarcRelation++;
	IndexMarcRelation_INDEX[nIndexMarcRelation] = _T("IBS_������Ư��");		IndexMarcRelation_MARC[nIndexMarcRelation] = _T("������Ư��");	nIndexMarcRelation++;
	IndexMarcRelation_INDEX[nIndexMarcRelation] = _T("IBS_�����ڷ�");		IndexMarcRelation_MARC[nIndexMarcRelation] = _T("�����ڷ�");	nIndexMarcRelation++;

	nIndexSpeciesRelation = 0;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_���ı���_�ڵ�");		IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_���ı���_�ڵ�");		nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_��������");		IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_��������_�ڵ�");		nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_������ϱ���_�ڵ�");	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_������ϱ���_�ڵ�");	nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_�����ڷᱸ��");	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_�����ڷᱸ��_�ڵ�");	nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_�з�ǥ����");			IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_�з�ǥ����");			nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_�ڷᱸ��_�ڵ�");		IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_�ڷᱸ��_�ڵ�");		nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_���");				IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_���");					nIndexSpeciesRelation++;
	
	nIndexBookRelation = 0;
	IndexBookRelation_Book[nIndexBookRelation] = _T("BB_�з�ǥ����");			IndexBookRelation_INDEX[nIndexBookRelation] = _T("IBS_�з�ǥ����");			nIndexBookRelation++;
	IndexBookRelation_Book[nIndexBookRelation] = _T("BB_��������_�ڵ�");		IndexBookRelation_INDEX[nIndexBookRelation] = _T("IBS_��������_�ڵ�");			nIndexBookRelation++;
	IndexBookRelation_Book[nIndexBookRelation] = _T("BB_��ü����_�ڵ�");		IndexBookRelation_INDEX[nIndexBookRelation] = _T("IBS_��ü����_�ڵ�");			nIndexBookRelation++;
	IndexBookRelation_Book[nIndexBookRelation] = _T("BB_��ϱ���_�ڵ�");		IndexBookRelation_INDEX[nIndexBookRelation] = _T("IBS_��ϱ���_�ڵ�");			nIndexBookRelation++;
	IndexBookRelation_Book[nIndexBookRelation] = _T("BB_��ġ��ȣ_�ڵ�");		IndexBookRelation_INDEX[nIndexBookRelation] = _T("IBS_��ġ��ȣ_�ڵ�");		nIndexBookRelation++;
	IndexBookRelation_Book[nIndexBookRelation] = _T("BB_�̿����ѱ���_�ڵ�");	IndexBookRelation_INDEX[nIndexBookRelation] = _T("IBS_�̿����ѱ���_�ڵ�");		nIndexBookRelation++;
	IndexBookRelation_Book[nIndexBookRelation] = _T("BB_�̿��󱸺�_�ڵ�");	IndexBookRelation_INDEX[nIndexBookRelation] = _T("IBS_�̿��󱸺�_�ڵ�");		nIndexBookRelation++;
	
	nSpeciesMarcRelation = 0;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_�����");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("260$c");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("BB_�����");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$a");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("BB_������Ư��");	SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$b");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("BB_ũ��");			SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$c");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("BB_�����ڷ�");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$e");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_�����");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$a");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_������Ư��");	SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$b");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_ũ��");			SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$c");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_�����ڷ�");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$e");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_�ǻ���");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("250$a");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_������");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("260$a");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_������");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("260$b");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("BB_�����");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("260$c");			nSpeciesMarcRelation++;
}

CBO_SPECIES::~CBO_SPECIES()
{
	if (m_pDM_SPECIES) {
		delete m_pDM_SPECIES;
		m_pDM_SPECIES = NULL;
	}
	if (m_pDM_BOOK) {
		delete m_pDM_BOOK;
		m_pDM_BOOK = NULL;
	}
	if (m_pDM_PURCHASEINFO) {
		delete m_pDM_PURCHASEINFO;
		m_pDM_PURCHASEINFO = NULL;
	}
	if (m_pDM_IDX_BO) {
		delete m_pDM_IDX_BO;
		m_pDM_IDX_BO = NULL;
	}
	if (m_pDM_DELETE_BOOK) {
		delete m_pDM_DELETE_BOOK;
		m_pDM_DELETE_BOOK = NULL;
	}
	if (m_pDM_DELETE_SPECIES) {
		delete m_pDM_DELETE_SPECIES;
		m_pDM_DELETE_SPECIES = NULL;
	}
	if (m_pDM_DELETE_VOLUME) {
		delete m_pDM_DELETE_VOLUME;
		m_pDM_DELETE_VOLUME = NULL;
	}
	if(m_pDM_ACCU)
	{
		delete m_pDM_ACCU;
		m_pDM_ACCU = NULL;
	}
	if(m_pDM_EBook)
	{
		delete m_pDM_EBook;
		m_pDM_EBook = NULL;
	}
}

INT CBO_SPECIES::SetOpenMode(INT nOpenMode)
{
	m_nOpenMode = nOpenMode;
	return 0;
}

INT CBO_SPECIES::GetOpenMode()
{
	return m_nOpenMode;
}

CString CBO_SPECIES::FindDMPK(CString strDMAlias)
{
	CString strFileName;
	strFileName.Format(_T("..\\CFG\\Reference\\DM\\DMPK.CFG"));
	CStdioFile fd;
	if (fd.Open(strFileName, CStdioFile::modeRead | CStdioFile::shareDenyWrite | CFile::typeBinary) == 0) return -1;	// ���������� ã�� �� ����
	TCHAR szBuf[16000];
	CString strLine;
	INT pos;
	
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}

	INT nStart = 0;
	CString PK, ALIAS;
	CString result = _T("");
	while(TRUE) {
		if (fd.ReadString(szBuf, 16000) == NULL) break;
		if (szBuf[0] == '#') continue;
		strLine.Format(_T("%s"), szBuf);
		strLine.TrimLeft();			strLine.TrimRight();
		
		nStart = 0;

		pos = strLine.Find('|', nStart);
		PK = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		ALIAS = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		PK.TrimLeft();		PK.TrimRight();
		ALIAS.TrimLeft();	ALIAS.TrimRight();

		if (ALIAS == strDMAlias) {
			result = PK;
			break;
		}
	}
	fd.Close();
	return result;
}

INT CBO_SPECIES::InitDataMgr(VOID)
{
	CString strDMPK;

	if (GetOpenMode() != 4000) {
		// �� DM�� �����Ѵ�.
//���ο���==============================================
		m_pDM_SPECIES = new CESL_DataMgr;
		m_pDM_SPECIES->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		m_pDM_SPECIES->InitDMPKFromFile(_T("DMUP_����_��_������"));
//���ο���==============================================
		m_pDM_SPECIES->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	}

	// å DM�� �����Ѵ�.
//���ο���==============================================
	m_pDM_BOOK = new CESL_DataMgr;
	m_pDM_BOOK->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
	m_pDM_BOOK->InitDMPKFromFile(_T("DMUP_����_��_å����"));
//���ο���==============================================
	m_pDM_BOOK->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	
//#ifdef _EBOOK_SUPPORT_
	m_pDM_EBook = new CESL_DataMgr;
	m_pDM_EBook->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
	m_pDM_EBook->InitDMPKFromFile(_T("DMUP_����_��_EBOOK"));
	m_pDM_EBook->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
//#endif
	
	if (m_nOpenMode == 4000 ) {
		// ���� DM�� �����Ѵ�.
//���ο���==============================================
		m_pDM_PURCHASE = new CESL_DataMgr;
		m_pDM_PURCHASE->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		m_pDM_PURCHASE->InitDMPKFromFile(_T("DMUP_����_��_��������"));
//���ο���==============================================
		m_pDM_PURCHASE->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	}
	if (m_nOpenMode == 5000 ) {
		// ���� DM�� �����Ѵ�.
		m_pDM_IDX_BO = new CESL_DataMgr;
		m_pDM_IDX_BO->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		m_pDM_IDX_BO->InitDMPKFromFile(_T("DMUP_����_��_��������"));
		m_pDM_IDX_BO->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);

		// ���� DM�� �����Ѵ�.
		m_pDM_ACCU = new CESL_DataMgr;
		m_pDM_ACCU->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		m_pDM_ACCU->InitDMPKFromFile(_T("DMUP_����_��_��������"));
		m_pDM_ACCU->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	}
	if(m_nOpenMode == 5100) 
	{
		m_pDM_IDX_BO = new CESL_DataMgr;
		m_pDM_IDX_BO->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		m_pDM_IDX_BO->InitDMPKFromFile(_T("DMUP_����_��_��������"));
		m_pDM_IDX_BO->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);

		// ���� DM�� �����Ѵ�.
		m_pDM_ACCU = new CESL_DataMgr;
		m_pDM_ACCU->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		m_pDM_ACCU->InitDMPKFromFile(_T("DMUP_����_��_��������"));
		m_pDM_ACCU->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	}
	if (m_nOpenMode == -6000) {
		// ���� DM�� �����Ѵ�.
		m_pDM_IDX_BO = new CESL_DataMgr;
		m_pDM_IDX_BO->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		m_pDM_IDX_BO->InitDMPKFromFile(_T("DMUP_����_��_��������"));
		m_pDM_IDX_BO->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);

		// ������ ��Ȱ���� ���� ����DM�� �����Ѵ�. - ����������
		m_pDM_DELETE_SPECIES = new CESL_DataMgr;
		m_pDM_DELETE_SPECIES->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		m_pDM_DELETE_SPECIES->InitDMPKFromFile(_T("DMUP_����_��API_����������"));
		m_pDM_DELETE_SPECIES->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);

		// ������ ��Ȱ���� ���� ����DM�� �����Ѵ�. - ����å����
		m_pDM_DELETE_BOOK = new CESL_DataMgr;
		m_pDM_DELETE_BOOK->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		m_pDM_DELETE_BOOK->InitDMPKFromFile(_T("DMUP_����_��API_����å����"));
		m_pDM_DELETE_BOOK->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	}
	
	if(GetOpenMode() == -7000)
	{
		// ���� DM�� �����Ѵ�.
		m_pDM_IDX_BO = new CESL_DataMgr;
		m_pDM_IDX_BO->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		m_pDM_IDX_BO->InitDMPKFromFile(_T("DMUP_����_��_��������"));
		m_pDM_IDX_BO->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	}

	if(GetOpenMode() == -10000)
	{
		// ���� DM�� �����Ѵ�.
		m_pDM_IDX_BO = new CESL_DataMgr;
		m_pDM_IDX_BO->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		m_pDM_IDX_BO->InitDMPKFromFile(_T("DMUP_����_��_��������"));
		m_pDM_IDX_BO->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
		SetAccuDM(TRUE);
	}
	if(GetOpenMode() == -11000)
	{
		// ���� DM�� �����Ѵ�.
		m_pDM_IDX_BO = new CESL_DataMgr;
		m_pDM_IDX_BO->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		m_pDM_IDX_BO->InitDMPKFromFile(_T("DMUP_����_��_��������"));
		m_pDM_IDX_BO->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
		SetAccuDM(FALSE);
	}
	return 0;
}	

// Error Code : -33000 ~
INT CBO_SPECIES::BO_SPECIES_PURCHASE_INSERT(CESL_DataMgr *pDM_IDX_BO, CESL_DataMgr *pDM_SPECIES, CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_ACCU, INT nRowIdx/*=0*/, CESL_DataMgr *pDM_TARGET/*=NULL*/)
{
	// ����-��-������ ����
	// ���� 1 row insert
	// Idx �� 1row insert
	// ���������� 1 row insert
	// (å)�������� n rows insert

	CArray <CString, CString> RemoveAliasList;

	pDM_BOOK->StartFrame();
	pDM_SPECIES->StartFrame();

	INT ids;
	CString strSpecies_key, strPurchaseinfo_key, strBook_key, strData = _T("");
	pDM_SPECIES->MakeRecKey(strSpecies_key); 
	m_strSPECIES_KEY = strSpecies_key;
	pDM_SPECIES->MakeRecKey(strPurchaseinfo_key); 

	// Default ���� ���� ��Ų��.
	SetDefaultValue(nRowIdx, pDM_IDX_BO, pDM_SPECIES, pDM_ACCU, pDM_BOOK);

	// ���ο� �η� ������ �����Ѵ�.
	APPLY_IDX_APPENDIX_YN(pDM_IDX_BO, pDM_BOOK, nRowIdx);

	// ���� Insert�� Insert���� �����Ѵ�.
	INDEX_to_SPECIES(pDM_IDX_BO, nRowIdx, pDM_SPECIES, nRowIdx);		// ������ ������ ���� ������� �ش�.
	//���� ������ �̿����� ���� �ڵ� ����
	CString strUSELIMIT_CODE;
	Get_Priority_USELIMIT_CODE(pDM_BOOK, strUSELIMIT_CODE);
	pDM_SPECIES->SetCellData(_T("BS_�̿����ѱ���_�ڵ�"), strUSELIMIT_CODE, nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_��KEY"), strSpecies_key, nRowIdx);
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BS_MARC"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("BO_SPECIES_TBL"), nRowIdx, pDM_SPECIES);
	FileLog(pDM_SPECIES->m_lastmsg);

	CMarc *pMarc = new CMarc;
	CString strStreamMarc;
	pDM_SPECIES->GetCellData(_T("BS_MARC"), nRowIdx, strStreamMarc);
	pMgr->m_pInfo->pMarcMgr->Decoding(strStreamMarc, pMarc);		

	// �˻� �ʵ带 ���ο� �ϼ��Ѵ�.
	ids = APPLY_IndexItem(pDM_IDX_BO, pMarc, nRowIdx);
	if (pMarc) {
		delete pMarc;
		pMarc = NULL;
	}

	// ���ο� Insert�� Insert���� �����Ѵ�.
	pDM_IDX_BO->SetCellData(_T("IBS_REC_KEY"), strSpecies_key, nRowIdx);
	CString strSpecies_class;
	if (m_nOpenMode == 1100 || m_nOpenMode == 1200) strSpecies_class = _T("0");
	if (m_nOpenMode == 3100 || m_nOpenMode == 3200) strSpecies_class = _T("1");
	if (m_nOpenMode == 5000) strSpecies_class = _T("0");		// ��ġ����ô� 0

	pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), strSpecies_class, nRowIdx);
	//ST_CODE�� �����Ѵ�.
	Set_Index_ST_CODE(_T("IBS_ISBN"), pDM_IDX_BO, pDM_BOOK, -1);
	
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX_BO, _T("IDX_BO_TBL"), nRowIdx, pDM_SPECIES);
	FileLog(pDM_SPECIES->m_lastmsg);

	//�̿����ѱ���/��������/�ڷ��/��ϱ��� ���а��� ���а� concatenation
	CStringArray Index_classfycodes;
	Index_classfycodes.Add(_T("IBS_�̿����ѱ���_�ڵ�"));
	Index_classfycodes.Add(_T("IBS_��������_�ڵ�"));
	Index_classfycodes.Add(_T("IBS_�ڷ��ִ°�INFO_�ڵ�"));
	Index_classfycodes.Add(_T("IBS_��ϱ���_�ڵ�"));
	CStringArray Book_classfycodes;
	Book_classfycodes.Add(_T("BB_�̿����ѱ���_�ڵ�"));
	Book_classfycodes.Add(_T("BB_��������_�ڵ�"));
	Book_classfycodes.Add(_T("BB_�谡��ġ��ȣ"));
	Book_classfycodes.Add(_T("BB_��ϱ���_�ڵ�"));
	CString Index_Update_sql;
	MakeIndexClassifyCodeConcatenationSQL(pDM_IDX_BO, pDM_BOOK, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strSpecies_key);
	pDM_SPECIES->AddFrame(Index_Update_sql);
	
	// ���������� Insert�� Insert���� �����Ѵ�.
	CString strWORKING_STATUS = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_�۾�����INFO"), nRowIdx, strWORKING_STATUS);
	//�˼����/�˼��Ϸ��ڷ� �Է½� ��ǰå��, ����å��, ������, �˼��� ���� ����
	if (strWORKING_STATUS == _T("BOA113N")) pDM_ACCU->SetCellData(_T("BP_��������"), GetTodayDate(), nRowIdx);
	if (strWORKING_STATUS == _T("BOA141N"))
	{
		pDM_ACCU->SetCellData(_T("BP_��ǰå��"), pDM_ACCU->GetCellData(_T("BP_����å��"), nRowIdx), nRowIdx);
		pDM_ACCU->SetCellData(_T("BP_��������"), GetTodayDate(), nRowIdx);
		pDM_ACCU->SetCellData(_T("BP_�˼���"), GetTodayDate(), nRowIdx);
	}
	pDM_IDX_BO->GetCellData(_T("IBS_û��_�з���ȣ"), nRowIdx, strData);
	pDM_ACCU->SetCellData(_T("BP_�Է�����") , GetTodayDate(), nRowIdx);
	pDM_ACCU->SetCellData(_T("BP_��KEY"), strSpecies_key, nRowIdx);
	pDM_ACCU->SetCellData(_T("BP_��������KEY"), strPurchaseinfo_key, nRowIdx);
	if (pDM_ACCU->GetCellData(_T("BP_������"), nRowIdx).GetLength() == 0) pDM_ACCU->SetCellData(_T("BP_������"), _T("0"), nRowIdx);
	if (pDM_ACCU->GetCellData(_T("BP_����"), nRowIdx).GetLength() == 0) pDM_ACCU->SetCellData(_T("BP_����"), _T("0"), nRowIdx);
	if (pDM_ACCU->GetCellData(_T("BP_�����Ϸù�ȣ"), nRowIdx).GetLength() == 0) pDM_ACCU->SetCellData(_T("BP_�����Ϸù�ȣ"), _T("0"), nRowIdx);
	if (pDM_ACCU->GetCellData(_T("BP_ȯ��"), nRowIdx).GetLength() == 0) pDM_ACCU->SetCellData(_T("BP_ȯ��"), _T("0"), nRowIdx);

	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BP_������ȣ"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_ACCU, _T("BO_PURCHASEINFO_TBL"), nRowIdx, pDM_SPECIES);
	FileLog(pDM_SPECIES->m_lastmsg);

	// å������ Insert�� Insert���� �����Ѵ�.
	INT nBookRow = pDM_BOOK->GetRowCount();
	for (INT i = 0; i < nBookRow; i++) {
		pDM_BOOK->SetCellData(_T("BB_�Է���"), GetTodayDate(), i);
		if(strWORKING_STATUS = _T("BOA141N")) pDM_BOOK->SetCellData(_T("BB_�˼���"), GetTodayDate(), i);
/*�̰� �� �ϴ��� �𸣰ڴ�.
		if(pDM_BOOK->GetCellData(_T("BB_����������"), i).GetLength() == 0)
			pDM_BOOK->SetCellData(_T("BB_����������"), pDM_ACCU->GetCellData(_T("BP_����������"), nRowIdx), i);
*/
	}
	MakeBookQueryFrame(pDM_BOOK, strSpecies_key, strPurchaseinfo_key, pDM_SPECIES);

	INT idsQuery, idsMarc;
	// Query ���� Send Frame
	idsQuery = SendFrame(pDM_SPECIES, MAX_TRANSACTION_QUERY_COUNT);
	pDM_SPECIES->EndFrame();
	if (idsQuery < 0) return -33051;		// Species Insert Error

	// Marc ���� Send Frame
	idsMarc = UpdateSpeciesMarcbyStream(pDM_SPECIES, _T("BO_SPECIES_TBL"), strSpecies_key, nRowIdx);
	pDM_BOOK->EndFrame();
	if (idsMarc < 0) return -33053;		// Marc Stream Update Error

	if (idsQuery == 0 && idsMarc == 0 && pDM_TARGET) {		// ���� ȭ�鿡 �ѷ��� DM�� �������ش�.
		pDM_TARGET->InsertRow(-1);
		INT nTargetRowIdx = pDM_TARGET->GetRowCount() -1;
		APPLY_ACQ_VIEW_DM_Data(pDM_IDX_BO, pDM_ACCU, nRowIdx, pDM_TARGET, nTargetRowIdx);
	}
	return 0;
}

INT CBO_SPECIES::MakeIndexClassifyCodeConcatenationSQL(CESL_DataMgr * pIDX_DM, CESL_DataMgr * pBOOK_DM, CStringArray * Index_classfycodes, CStringArray * Book_classfycodes, INT cnt, CString &Index_Update_sql, CString strRecKey, INT mode /*=-1*/ )
{
	CStringArray Index_field;
	CStringArray Index_value;
	INT nCol = -1;
	INT ids = -1;
	CString field_name, field_alias, field_type, init_value, data_type, value, pre_value;
	Index_Update_sql = _T("UPDATE IDX_BO_TBL SET ");
	CString sub_sql = _T("");
	for(INT i=0 ; i<cnt ; i++)
	{
		nCol = pIDX_DM->FindColumn(Index_classfycodes->GetAt(i));
		if(nCol < 0) continue;
		pIDX_DM->GetColumnRefInfo(nCol,  field_alias, field_name, field_type, init_value, data_type);
		ids = MakeIndexClassifyCodeConcatenationValue(pBOOK_DM, Book_classfycodes->GetAt(i), value);
		if(ids < 0) continue;
		if(mode > 0)
		{
			pIDX_DM->GetOneValueQuery(_T("SELECT ") + field_name + _T(" FROM IDX_BO_TBL WHERE REC_KEY = ") + strRecKey, pre_value);
			if(!pre_value.IsEmpty()) MergeClassifyCodes(pre_value, value);
		}
		if(!value.IsEmpty())
			if(sub_sql.IsEmpty()) sub_sql = field_name + _T(" = '") + value + _T("'");
			else sub_sql += _T(", ") + field_name + _T(" = '") + value + _T("'");
	}
	if(!sub_sql.IsEmpty() && !strRecKey.IsEmpty())
	{
		Index_Update_sql += sub_sql + _T(" WHERE REC_KEY = ") + strRecKey + _T(";");
		return 0;
	}
	return -1;
}

INT CBO_SPECIES::MergeClassifyCodes(CString pre_value, CString &value)
{
	CStringArray codeValue;
	INT pos;
	CString code;
	while(!pre_value.IsEmpty())
	{
		pos = pre_value.Find(_T(" "), 0);
		if(pos < 0)
		{
			codeValue.Add(pre_value);
			break;
		}
		code = pre_value.Mid(0, pos);
		pre_value = pre_value.Mid(pos+1);
		codeValue.Add(code);
	}
	while(!value.IsEmpty())
	{
		pos = value.Find(_T(" "), 0);
		if(pos < 0)
		{
			if(!CheckIsHave(&codeValue, value))
				codeValue.Add(value);
			break;
		}
		code = value.Mid(0, pos);
		value = value.Mid(pos+1);
		if(!CheckIsHave(&codeValue, code))
			codeValue.Add(code);
	}
	value.Empty();
	for(INT i=0 ; i<codeValue.GetSize() ; i++)
	{
		if(!value.IsEmpty()) value += _T(" ");
		value += codeValue.GetAt(i);
	}
	return 0;
}

INT CBO_SPECIES::MakeIndexClassifyCodeConcatenationValue(CESL_DataMgr * pBOOK_DM, CString field_alias, CString &value)
{
	CStringArray codeValue;
	INT rowCnt = pBOOK_DM->GetRowCount();
	CString code, strDBFlag;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		pBOOK_DM->GetCellData(_T("BB_DB_FLAG"), i, strDBFlag);
		if(strDBFlag == _T("D")) continue;
		pBOOK_DM->GetCellData(field_alias, i, code);
		//m_cIndexTool.GetIndex()->MakeIndex(code);
		((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(code);
		if(!CheckIsHave(&codeValue, code))
			codeValue.Add(code);
	}
	value.Empty();
	for(i=0 ; i<codeValue.GetSize() ; i++)
	{
		if(!value.IsEmpty()) value += _T(" ");
		value += codeValue.GetAt(i);
	}
	return 0;
}

BOOL CBO_SPECIES::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;
}

INT CBO_SPECIES::Get_Priority_USELIMIT_CODE(CESL_DataMgr * pDM_BOOK, CString &strUSELIMIT_CODE, INT mode /*=-1*/, CString strPrev_USELIMIT_CODE /*=_T("")*/)
{
	//mode ���� -1 : ���� ���� �̿����� ������ �����ϴ� ��� 1 : ���� ���� �̿����� ������ �缳�� �ϴ� ���
	strUSELIMIT_CODE.Empty();
	INT rowCnt = pDM_BOOK->GetRowCount();
	CString strTmpData, strDBFlag;
	INT nPriority = -1;
	INT nMaxPriority = -1;
	if(mode > 0 && strPrev_USELIMIT_CODE.GetLength() > 0)
	{
		if(strPrev_USELIMIT_CODE == _T("GM")) nMaxPriority = 50;
		else if(strPrev_USELIMIT_CODE == _T("IZ")) nMaxPriority = 40;
		else if(strPrev_USELIMIT_CODE == _T("CA")) nMaxPriority = 30;
		else if(strPrev_USELIMIT_CODE == _T("CD")) nMaxPriority = 20;
		else if(strPrev_USELIMIT_CODE == _T("CL")) nMaxPriority = 10;
	}

	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, strDBFlag);
		if(strDBFlag == _T("D")) continue;
		pDM_BOOK->GetCellData(_T("BB_�̿����ѱ���_�ڵ�"), i, strTmpData);
		if(strTmpData.IsEmpty()) continue;
		strTmpData.TrimLeft();	strTmpData.TrimRight();		strTmpData.MakeUpper();
		if(strTmpData == _T("GM")) nPriority = 50;
		else if(strTmpData == _T("IZ")) nPriority = 40;
		else if(strTmpData == _T("CA")) nPriority = 30;
		else if(strTmpData == _T("CD")) nPriority = 20;
		else if(strTmpData == _T("CL")) nPriority = 10;
		if(nMaxPriority < nPriority) nMaxPriority = nPriority;
	}
	if(nMaxPriority < 0) return 0;
	else if(nMaxPriority == 50) strUSELIMIT_CODE = _T("GM");
	else if(nMaxPriority == 40) strUSELIMIT_CODE = _T("IZ");
	else if(nMaxPriority == 30) strUSELIMIT_CODE = _T("CA");
	else if(nMaxPriority == 20) strUSELIMIT_CODE = _T("CD");
	else if(nMaxPriority == 10) strUSELIMIT_CODE = _T("CL");

	return 0;
}

// Error code : -34000 ~
INT CBO_SPECIES::BO_SPECIES_PURCHASE_UPDATE(CESL_DataMgr *pDM_IDX_BO, CESL_DataMgr *pDM_SPECIES, CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_PURCHASE, INT nRowIdx, CESL_DataMgr *pDM_TARGET/*=NULL*/, INT nTargetRowIdx/*=0*/)
{
	// ����-��-����-������ ����
	// ���� 1 row update
	// ���������� 1 row update
	// (å)�������� n rows update

	CArray <CString, CString> RemoveAliasList;

	INT ids;
	CString strSpecies_key, strPurchaseinfo_key, strBook_key;
	ids = pDM_SPECIES->GetCellData(_T("BS_��KEY"), nRowIdx, strSpecies_key);
	ids = pDM_PURCHASE->GetCellData(_T("BP_��������KEY"), nRowIdx, strPurchaseinfo_key);

	pDM_BOOK->StartFrame();
	pDM_SPECIES->StartFrame();

	// ���� Update���� �����Ѵ�.
	INDEX_to_SPECIES(pDM_IDX_BO, nRowIdx, pDM_SPECIES, nRowIdx);		// ������ ������ ���� ������� �ش�.
	//���� ������ �̿����� ���� �ڵ� ����
	CString strUSELIMIT_CODE;
	pDM_SPECIES->GetCellData(_T("BS_�̿����ѱ���_�ڵ�"), nRowIdx, strUSELIMIT_CODE);
	Get_Priority_USELIMIT_CODE(pDM_BOOK, strUSELIMIT_CODE, 1, strUSELIMIT_CODE);
	pDM_SPECIES->SetCellData(_T("BS_�̿����ѱ���_�ڵ�"), strUSELIMIT_CODE, nRowIdx);
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BS_MARC"));
	RemoveAliasList.Add(_T("BS_��KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("BO_SPECIES_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);

	// �˻� �ʵ带 ���ο� �ϼ��Ѵ�.
	CMarc *pMarc = new CMarc;
	CString strStreamMarc;
	pDM_SPECIES->GetCellData(_T("BS_MARC"), nRowIdx, strStreamMarc);
	pMgr->m_pInfo->pMarcMgr->Decoding(strStreamMarc, pMarc);		
	// �˻� �ʵ带 ���ο� �ϼ��Ѵ�.
	ids = APPLY_IndexItem(pDM_IDX_BO, pMarc, nRowIdx);
	if (pMarc) {
		delete pMarc;
		pMarc = NULL;
	}
	//ST_CODE�� �����Ѵ�.
	Set_Index_ST_CODE(_T("IBS_ISBN"), pDM_IDX_BO, pDM_BOOK, -1);

	// ���ο� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("IBS_REC_KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX_BO, _T("IDX_BO_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);

	//�̿����ѱ���/��������/�ڷ��/��ϱ��� ���а��� ���а� concatenation
	CStringArray Index_classfycodes;
	Index_classfycodes.Add(_T("IBS_�̿����ѱ���_�ڵ�"));
	Index_classfycodes.Add(_T("IBS_��������_�ڵ�"));
	Index_classfycodes.Add(_T("IBS_�ڷ��ִ°�INFO_�ڵ�"));
	Index_classfycodes.Add(_T("IBS_��ϱ���_�ڵ�"));
	CStringArray Book_classfycodes;
	Book_classfycodes.Add(_T("BB_�̿����ѱ���_�ڵ�"));
	Book_classfycodes.Add(_T("BB_��������_�ڵ�"));
	Book_classfycodes.Add(_T("BB_�谡��ġ��ȣ"));
	Book_classfycodes.Add(_T("BB_��ϱ���_�ڵ�"));
	CString Index_Update_sql;
	MakeIndexClassifyCodeConcatenationSQL(pDM_IDX_BO, pDM_BOOK, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strSpecies_key);
	pDM_SPECIES->AddFrame(Index_Update_sql);

	// ������ ��������/å������ ������ �����Ѵ�.
	CString strWorkingStatus;
	pDM_IDX_BO->GetCellData(_T("IBS_�۾�����INFO"), 0, strWorkingStatus);
	SetBookDMInfoByWorkingStatus(pDM_BOOK, strWorkingStatus);
	SetPurchaseDMInfoByWorkingStatus(pDM_PURCHASE, pDM_BOOK, strWorkingStatus);
	
	// ���������� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BP_��������KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("BO_PURCHASEINFO_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strPurchaseinfo_key);

	// å������ Insert/Update/Delete Ȥ�� ����å������ Insert���� �����Ѵ�. -> �׸��� DB�� SendFrame�Ѵ�.
	ids = MakeBookQueryFrame(pDM_BOOK, strSpecies_key, strPurchaseinfo_key, pDM_SPECIES);
	if (ids < 0) return -34052;

	INT idsQuery, idsMarc;
	// Query ���� Send Frame
	idsQuery = SendFrame(pDM_SPECIES, MAX_TRANSACTION_QUERY_COUNT);
	pDM_SPECIES->EndFrame();
	if (idsQuery < 0) return -34051;		// Species Insert Error

	// Marc ���� Send Frame
	idsMarc = UpdateSpeciesMarcbyStream(pDM_SPECIES, _T("BO_SPECIES_TBL"), strSpecies_key, nRowIdx);
	pDM_SPECIES->EndFrame();
	if (idsMarc < 0) return -34053;		// Marc Stream Update Error

	if (idsQuery == 0 && idsMarc == 0 && pDM_TARGET) {		// ���� ȭ�鿡 �ѷ��� DM�� �������ش�.
		APPLY_ACQ_VIEW_DM_Data(pDM_IDX_BO, pDM_PURCHASE, nRowIdx, pDM_TARGET, nTargetRowIdx);
	}

	return 0;
}

INT CBO_SPECIES::GetBookCountByWorkingStatus(CESL_DataMgr *pDM_BOOK, CString strWorkingStatus)
{
	INT nRowCnt = pDM_BOOK->GetRowCount();
	INT cnt = 0;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(!IsBook(pDM_BOOK, i)) continue;
		if(pDM_BOOK->GetCellData(_T("BB_�ڷ����"), i) == strWorkingStatus)
			cnt++;
	}

	return cnt;
}

INT CBO_SPECIES::GetPriceSumByWorkingStatus(CESL_DataMgr *pDM_BOOK, CString strWorkingStatus, CString strPriceSum, INT nDiscountRate)
{
	INT nRowCnt = pDM_BOOK->GetRowCount();
	DOUBLE dPrice, dTotalPrice;
	dPrice = dTotalPrice = 0.0;
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(!IsBook(pDM_BOOK, i)) continue;
		if(pDM_BOOK->GetCellData(_T("BB_�ڷ����"), i) == strWorkingStatus)
		{
			pDM_BOOK->GetCellData(_T("BB_����"), i, strTmpData);
			dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
			dTotalPrice += (dPrice - (dPrice * (nDiscountRate/100.0)));
		}
	}
	strPriceSum.Format(_T("%10.2f"), dTotalPrice);
	return 0;
}

INT CBO_SPECIES::SetBookDMInfoByWorkingStatus(CESL_DataMgr * pDM_BOOK, CString strWorkingStatus)
{
	//åDM���� DB_FLAG�� I�� �ڷῡ ���Ͽ� å�� �ڷ���¸� ���ڷ� ���޹��� �ڷ���·� �����Ѵ�.
	INT nRowCnt = pDM_BOOK->GetRowCount();
	CString strToday = this->GetTodayDate();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(!IsBook(pDM_BOOK, i)) continue;
		pDM_BOOK->SetCellData(_T("BB_�ڷ����"), strWorkingStatus, i);
		if(strWorkingStatus == _T("BOA111N") || strWorkingStatus == _T("BOA113N"))
			pDM_BOOK->SetCellData(_T(""), strToday, i);
		else if(strWorkingStatus == _T("BOA141N"))
		{
			pDM_BOOK->SetCellData(_T("BB_�Է���"), strToday, i);
			pDM_BOOK->SetCellData(_T("BB_�˼���"), strToday, i);
		}
	}
	return 0;
}

INT CBO_SPECIES::SetPurchaseDMInfoByWorkingStatus(CESL_DataMgr *pDM_PURCHASE, CESL_DataMgr *pDM_BOOK, CString strWorkingStatus)
{
	INT nTotalBookCnt, nDeliveryBookCnt, nMissingBookCnt;
	nTotalBookCnt = nDeliveryBookCnt = nMissingBookCnt = -1;
	CString strToday = this->GetTodayDate();
	nTotalBookCnt = this->GetBookCount(pDM_BOOK);
	nDeliveryBookCnt = this->GetBookCountByWorkingStatus(pDM_BOOK, _T("BOA141N"));
	nMissingBookCnt = this->GetBookCountByWorkingStatus(pDM_BOOK, _T("BOA114O"));
	CString strTotalBookCnt, strDeliveryBookCnt, strMissingBookCnt, strDeliveryBookPrice;
	strTotalBookCnt.Format(_T("%d"), nTotalBookCnt);
	strDeliveryBookCnt.Format(_T("%d"), nDeliveryBookCnt);
	strMissingBookCnt.Format(_T("%d"), nMissingBookCnt);
	INT nDiscountRate = _ttoi((pDM_PURCHASE->GetCellData(_T("BP_������"), 0)).GetBuffer(0));
	GetPriceSumByWorkingStatus(pDM_BOOK, _T("BOA141N"), strDeliveryBookPrice, nDiscountRate);
	pDM_PURCHASE->SetCellData(_T("BP_����å��"), strTotalBookCnt, 0);
	pDM_PURCHASE->SetCellData(_T("BP_��ǰå��"), strDeliveryBookCnt, 0);
	pDM_PURCHASE->SetCellData(_T("BP_�̳�å��"), strMissingBookCnt, 0);
	pDM_PURCHASE->SetCellData(_T("BP_��ǰ����"), strDeliveryBookPrice, 0);
	if(strWorkingStatus == _T("BOA141N"))
		pDM_PURCHASE->SetCellData(_T("BP_�˼���"), strToday, 0);
	return 0;
}

INT CBO_SPECIES::BO_SPECIES_PURCHASE_ADDBOOK(CString MODE, CESL_DataMgr *pDM_IDX_BO, CESL_DataMgr *pDM_SPECIES, CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_PURCHASE, INT nRowIdx/*=0*/)
{
	CString strSpeciesKey, strPurchaseKey, strNewSpeciesKey, strNewPurchaseKey;
	CArray <CString, CString> RemoveAliasList;

	INT ids;
	CString sQuery, strTmp, strWORKING_STATUS;
	pDM_BOOK->StartFrame();

	if (MODE == _T("INSERT")) {
		strNewSpeciesKey = pDM_IDX_BO->GetCellData(_T("IBS_REC_KEY"), nRowIdx);
		pDM_IDX_BO->MakeRecKey(strNewPurchaseKey); 

		// ���������� Insert�� Insert���� �����Ѵ�.
		pDM_PURCHASE->SetCellData(_T("BP_��KEY"), strNewSpeciesKey, nRowIdx);
		pDM_PURCHASE->SetCellData(_T("BP_��������KEY"), strNewPurchaseKey, nRowIdx);
		pDM_PURCHASE->SetCellData(_T("BP_���Գ⵵"), GetTodayYear(), nRowIdx);
		pDM_PURCHASE->SetCellData(_T("BP_��������"), GetTodayDate(), nRowIdx);
		pDM_PURCHASE->SetCellData(_T("BP_�Է�����"), GetTodayDate(), nRowIdx);
		pDM_PURCHASE->SetCellData(_T("BP_�Է±���"), _T("0"), nRowIdx);
		strTmp.Format(_T("%d"), pDM_BOOK->GetRowCount());
		pDM_PURCHASE->SetCellData(_T("BP_����å��"), strTmp, nRowIdx);
		pDM_IDX_BO->GetCellData(_T("IBS_�۾�����INFO"), nRowIdx, strWORKING_STATUS);
		if(strWORKING_STATUS == _T("BOA113N"))
		{
			pDM_PURCHASE->SetCellData(_T("BP_��������"), GetTodayDate(), nRowIdx);
		}
		else if(strWORKING_STATUS == _T("BOA141N"))
		{
			pDM_PURCHASE->SetCellData(_T("BP_��������"), GetTodayDate(), nRowIdx);
			pDM_PURCHASE->SetCellData(_T("BP_�˼���"), GetTodayDate(), nRowIdx);
			pDM_PURCHASE->SetCellData(_T("BP_��ǰå��"), GetTodayDate(), nRowIdx);
		}
		if (pDM_PURCHASE->GetCellData(_T("BP_������"), nRowIdx).GetLength() == 0) pDM_PURCHASE->SetCellData(_T("BP_������"), _T("0"), nRowIdx);
		if (pDM_PURCHASE->GetCellData(_T("BP_����"), nRowIdx).GetLength() == 0) pDM_PURCHASE->SetCellData(_T("BP_����"), _T("0"), nRowIdx);
		if (pDM_PURCHASE->GetCellData(_T("BP_�����Ϸù�ȣ"), nRowIdx).GetLength() == 0) pDM_PURCHASE->SetCellData(_T("BP_�����Ϸù�ȣ"), _T("0"), nRowIdx);
		if (pDM_PURCHASE->GetCellData(_T("BP_ȯ��"), nRowIdx).GetLength() == 0) pDM_PURCHASE->SetCellData(_T("BP_ȯ��"), _T("0"), nRowIdx);

		RemoveAliasList.RemoveAll();
		RemoveAliasList.Add(_T("BP_������ȣ"));
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("BO_PURCHASEINFO_TBL"), nRowIdx, pDM_BOOK);
		FileLog(pDM_SPECIES->m_lastmsg);

		// å������ Insert�� Insert���� �����Ѵ�.
		MakeBookQueryFrame(pDM_BOOK, strNewSpeciesKey, strNewPurchaseKey, pDM_BOOK);	

		//�̿����ѱ���/��������/�ڷ��/��ϱ��� ���а��� ���а� concatenation
		//�����߱��� ��� ���� ������ ���а� ������ �����Ͽ� concatenation
		CStringArray Index_classfycodes;
		Index_classfycodes.Add(_T("IBS_�̿����ѱ���_�ڵ�"));
		Index_classfycodes.Add(_T("IBS_��������_�ڵ�"));
		Index_classfycodes.Add(_T("IBS_�ڷ��ִ°�INFO_�ڵ�"));
		Index_classfycodes.Add(_T("IBS_��ϱ���_�ڵ�"));
		CStringArray Book_classfycodes;
		Book_classfycodes.Add(_T("BB_�̿����ѱ���_�ڵ�"));
		Book_classfycodes.Add(_T("BB_��������_�ڵ�"));
		Book_classfycodes.Add(_T("BB_�谡��ġ��ȣ"));
		Book_classfycodes.Add(_T("BB_��ϱ���_�ڵ�"));
		CString Index_Update_sql;
		MakeIndexClassifyCodeConcatenationSQL(pDM_IDX_BO, pDM_BOOK, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strNewSpeciesKey, 1);
		pDM_BOOK->AddFrame(Index_Update_sql);

		//������ ST_CODE�� �����Ѵ�.
		Set_Index_ST_CODE(_T("IBS_ISBN"), pDM_IDX_BO, pDM_BOOK, 1);
		CString strUPDATE_SQL;
		strUPDATE_SQL.Format(_T("UPDATE IDX_BO_TBL SET ST_CODE = '%s' WHERE REC_KEY = %s;"), 
			pDM_IDX_BO->GetCellData(_T("IBS_ISBN"), 0), 
			strNewSpeciesKey);
		pDM_BOOK->AddFrame(strUPDATE_SQL);

		//���� ������ �̿����� ���� �ڵ� ����
		CString strUSELIMIT_CODE;
		pDM_SPECIES->GetCellData(_T("BS_�̿����ѱ���_�ڵ�"), nRowIdx, strUSELIMIT_CODE);
		Get_Priority_USELIMIT_CODE(pDM_BOOK, strUSELIMIT_CODE, 1, strUSELIMIT_CODE);		
		strUPDATE_SQL.Format(_T("UPDATE BO_SPECIES_TBL SET USE_LIMIT_CODE = '%s' WHERE REC_KEY = %s;"),
			strUSELIMIT_CODE, 
			strNewSpeciesKey);
		pDM_BOOK->AddFrame(strUPDATE_SQL);
		// ��������&å���� Send Frame
		ids = SendFrame(pDM_BOOK, MAX_TRANSACTION_QUERY_COUNT);
		pDM_BOOK->EndFrame();
		if (ids < 0) return -33052;		// �������� & å Insert Error
	}
	if (MODE == _T("UPDATE")) {
		strSpeciesKey = pDM_PURCHASE->GetCellData(_T("BP_��KEY"), nRowIdx);
		strPurchaseKey = pDM_PURCHASE->GetCellData(_T("BP_��������KEY"), nRowIdx);
		strNewSpeciesKey = pDM_IDX_BO->GetCellData(_T("IBS_REC_KEY"), nRowIdx);

		// ���������� �� ��KEY�� �����Ѵ�.
		pDM_PURCHASE->SetCellData(_T("BP_��KEY"), strNewSpeciesKey, nRowIdx);

		// ���������� Update���� �����Ѵ�.
		RemoveAliasList.RemoveAll();
		RemoveAliasList.Add(_T("BP_��������KEY"));
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("BO_PURCHASEINFO_TBL"), nRowIdx, pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strPurchaseKey);

		// å������ Insert/Update/Delete Ȥ�� ����å������ Insert���� �����Ѵ�. -> �׸��� DB�� SendFrame�Ѵ�.
		ids = MakeBookQueryFrame(pDM_BOOK, strNewSpeciesKey, strPurchaseKey, pDM_BOOK);
		if (ids < 0) return -34052;
		
		if (strSpeciesKey != strNewSpeciesKey) {
			sQuery.Format(_T("SELECT COUNT(*) FROM BO_PURCHASEINFO_TBL WHERE SPECIES_KEY=%s"), strSpeciesKey);
			INT nSpeciesCount = 0;
			pDM_BOOK->GetOneValueQuery(sQuery, strTmp);
			nSpeciesCount = _ttoi(strTmp);
			if (nSpeciesCount == 1) {
				sQuery.Format(_T("DELETE FROM IDX_BO_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
				pDM_BOOK->AddFrame(sQuery);
				sQuery.Format(_T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
				pDM_BOOK->AddFrame(sQuery);
			}
		}
		//�̿����ѱ���/��������/�ڷ��/��ϱ��� ���а��� ���а� concatenation
		//�����߱��� ��� ���� ������ ���а� ������ �����Ͽ� concatenation
		CStringArray Index_classfycodes;
		Index_classfycodes.Add(_T("IBS_�̿����ѱ���_�ڵ�"));
		Index_classfycodes.Add(_T("IBS_��������_�ڵ�"));
		Index_classfycodes.Add(_T("IBS_�ڷ��ִ°�INFO_�ڵ�"));
		Index_classfycodes.Add(_T("IBS_��ϱ���_�ڵ�"));
		CStringArray Book_classfycodes;
		Book_classfycodes.Add(_T("BB_�̿����ѱ���_�ڵ�"));
		Book_classfycodes.Add(_T("BB_��������_�ڵ�"));
		Book_classfycodes.Add(_T("BB_�谡��ġ��ȣ"));
		Book_classfycodes.Add(_T("BB_��ϱ���_�ڵ�"));
		CString Index_Update_sql;
		MakeIndexClassifyCodeConcatenationSQL(pDM_IDX_BO, pDM_BOOK, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strNewSpeciesKey, 1);
		pDM_BOOK->AddFrame(Index_Update_sql);	
		//������ ST_CODE�� �����Ѵ�.
		Set_Index_ST_CODE(_T("IBS_ISBN"), pDM_IDX_BO, pDM_BOOK, 1);
		CString strUPDATE_SQL;
		strUPDATE_SQL.Format(_T("UPDATE IDX_BO_TBL SET ST_CODE = '%s' WHERE REC_KEY = %s;"), 
			pDM_IDX_BO->GetCellData(_T("IBS_ISBN"), 0), 
			strNewSpeciesKey);
		pDM_BOOK->AddFrame(strUPDATE_SQL);
		
		//���� ������ �̿����� ���� �ڵ� ����
		CString strUSELIMIT_CODE;
		pDM_SPECIES->GetCellData(_T("BS_�̿����ѱ���_�ڵ�"), nRowIdx, strUSELIMIT_CODE);
		Get_Priority_USELIMIT_CODE(pDM_BOOK, strUSELIMIT_CODE, 1, strUSELIMIT_CODE);		
		strUPDATE_SQL.Format(_T("UPDATE BO_SPECIES_TBL SET USE_LIMIT_CODE = '%s' WHERE REC_KEY = %s;"),
			strUSELIMIT_CODE, 
			strNewSpeciesKey);
		pDM_BOOK->AddFrame(strUPDATE_SQL);

		// ��������&å���� Send Frame
		ids = SendFrame(pDM_BOOK, MAX_TRANSACTION_QUERY_COUNT);
		pDM_BOOK->EndFrame();
		if (ids < 0) return -33052;		// �������� & å Update Error		
	}

	return 0;
}

// Error Code : -31000 ~
INT CBO_SPECIES::BO_SPECIES_DELETE_OR_RECYCLE(CString FROM, CString MODE, CString strSpeciesKey)
{
	// FROM : _T("PURCHASE") or _T("DONATE")
	// MODE : _T("DELETE") or _T("RECYCLE")
	// ���� 1 row delete -> ��Ȱ������ 1 row insert(�÷��� �������ٰ�)
	// ���������� �� ���� 
	// (å)�������� n rows delete -> ��Ȱ��å�� n rows insert(�÷��� �������ٰ�)(��Ȱ���ϰ�� ������ ����)

	if (strSpeciesKey.GetLength() == 0) return -31001;	// ��KEY���� 
	INT nRowIdx = 0;

	INT ids;
	CArray <CString, CString> RemoveAliasList;

	m_pDM_SPECIES->StartFrame();
	m_pDM_BOOK->StartFrame();

	// Default ���� ���� ��Ų��.
	//SetDefaultValue(nRowIdx, pDM_IDX_BO, pDM_SPECIES, pDM_PURCHASE, pDM_BOOK);

	CString strOption, strQuery;
	// �ش� ��DM�� �����Ѵ�.
	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = m_pDM_SPECIES->RefreshDataManager(strOption);
	if (ids < 0) return -31002;
	// �ش� ���� åDM�� �����Ѵ�.
	strOption.Format(_T("BOOK_APPENDIX_FLAG = 'B' AND SPECIES_KEY = %s"), strSpeciesKey);
	ids = m_pDM_BOOK->RefreshDataManager(strOption);
	if (ids < 0) return -31003;
	if (MODE == _T("RECYCLE")) {		// å�� ������ ����
		INT beforeBookCount = m_pDM_BOOK->GetRowCount();
		CONVERT_BOOK_to_VOLUMEInBOOK(m_pDM_BOOK);
		INT afterBookCount = m_pDM_BOOK->GetRowCount();
	}

	// �������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BS_MARC"));
	m_pDM_SPECIES->InitDBFieldData();
	m_pDM_SPECIES->AddDBFieldData(_T("DELETE_DATE"),	_T("STRING"),	GetTodayDate());
	m_pDM_SPECIES->SetCellData(_T("BS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	m_pDM_SPECIES->MakeQueryByDM(RemoveAliasList, m_pDM_SPECIES, _T("BO_DELETE_SPECIES_TBL"), nRowIdx, m_pDM_SPECIES);
	strQuery.Format(_T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
	m_pDM_SPECIES->AddFrame(strQuery);
	if (MODE == _T("RECYCLE")) {
		strQuery.Format(_T("UPDATE IDX_BO_TBL SET SPECIES_CLASS='2', DELETE_DATE='%s' WHERE REC_KEY=%s;"), GetTodayDate(), strSpeciesKey);	// ��Ȱ���� ��� 2
	} else {
		strQuery.Format(_T("UPDATE IDX_BO_TBL SET SPECIES_CLASS='3', DELETE_DATE='%s' WHERE REC_KEY=%s;"), GetTodayDate(), strSpeciesKey);	// ������ ��� 3
	}
	m_pDM_SPECIES->AddFrame(strQuery);

	// ����å������ Insert�� Insert���� �����Ѵ�.
	// ����å������ �����Ҷ� �� å�� �ο��Ǿ��� ��Ϲ�ȣ�� CStringArray�� �����Ѵ�.
	INT nRowCount = m_pDM_BOOK->GetRowCount();
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BB_��Ϲ�ȣ"));
	RemoveAliasList.Add(_T("BB_������Ϲ�ȣ"));
	RemoveAliasList.Add(_T("BB_��޴��⿩��"));
	CString strRegNo;
	for (INT i = 0; i < nRowCount; i++) {
		m_pDM_SPECIES->InitDBFieldData();
		m_pDM_SPECIES->AddDBFieldData(_T("DELETE_DATE"),	_T("STRING"),	GetTodayDate());
		m_pDM_BOOK->SetCellData(_T("BB_�������۾�����"),	pMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_BOOK, _T("BO_DELETE_BOOK_TBL"), i, m_pDM_SPECIES);
		m_pDM_BOOK->GetCellData(_T("BB_��Ϲ�ȣ"), i, strRegNo);
		if(!strRegNo.IsEmpty())
			InsertToDropAccessionNoTbl(strRegNo, m_pDM_SPECIES, FALSE);
	}
	strQuery.Format(_T("DELETE FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s;"), strSpeciesKey);
	m_pDM_SPECIES->AddFrame(strQuery);
	
	// ���������� Send Frame
	ids = SendFrame(m_pDM_SPECIES, MAX_TRANSACTION_QUERY_COUNT);
	m_pDM_SPECIES->EndFrame();
	if (ids < 0) return -31051;		// Species Insert Error

	// Marc ���� Send Frame
	ids = UpdateSpeciesMarcbyStream(m_pDM_SPECIES, _T("BO_DELETE_SPECIES_TBL"), strSpeciesKey, nRowIdx);
	ids = m_pDM_SPECIES->SendFrame();
	if (ids < 0) return -31053;		// Marc Stream Update Error

	return 0;
}

INT CBO_SPECIES::SetRegNoInfoAndUpdateDropAccessionNoTbl(CESL_DataMgr * pDM_BOOK, INT idx, CESL_DataMgr * pDM_OUT)
{
	CString strRegNo, strSrcRegNo;
	strRegNo = pDM_BOOK->GetCellData(_T("BB_��Ϲ�ȣ"), idx);
	strSrcRegNo = pDM_BOOK->GetCellData(_T("BB_������Ϲ�ȣ"), idx);

	if(strRegNo != strSrcRegNo)
	{
		if(!strRegNo.IsEmpty())
		{
			pDM_BOOK->SetCellData(_T("BB_�����"), GetTodayDate(), idx);
			DeleteToDropAccessionNoTbl(strRegNo, pDM_OUT);
		}
		if(!strSrcRegNo.IsEmpty())
			InsertToDropAccessionNoTbl(strSrcRegNo, pDM_OUT, TRUE);

	}
	return 0;
}

INT CBO_SPECIES::DeleteToDropAccessionNoTbl(CString strRegNo, CESL_DataMgr * pDM_OUT)
{
	CString strCode = strRegNo.Left(2);
	INT nNumber = _ttoi((strRegNo.Right(10)).GetBuffer(0));
	CString query;
	query.Format(
		_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE = '%s' AND ACCESSION_REG_NO = %d;"),
		strCode,
		nNumber);
	pDM_OUT->AddFrame(query);
	return 0;
}

INT CBO_SPECIES::InsertToDropAccessionNoTbl(CString strRegNo, CESL_DataMgr * pDM_Out, BOOL IsCheckInsert)
{
	CString strCode = strRegNo.Left(2);
	INT nNumber = _ttoi((strRegNo.Right(10)).GetBuffer(0));
	CString query, strTmpData;
	if(IsCheckInsert)	
	{
		query.Format(_T("SELECT REC_KEY FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE = '%s' AND REG_NO = %d"), strCode, nNumber);
		pDM_Out->GetOneValueQuery(query, strTmpData);
		if(strTmpData.GetLength() != 0) return 0;
	}
	CString strRecKey;
	pDM_Out->MakeRecKey(strRecKey);
	query.Format(
		_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, REG_CODE, REG_NO) VALUES(%s, 'MO', '%s', %d);"),
			strRecKey,
			strCode,
			nNumber);
	pDM_Out->AddFrame(query);
	return 0;
}

// Error Code : -32000 ~
INT CBO_SPECIES::BO_SPECIES_RESTORE(CString MODE, CString strDeleteSpeciesKey)
{
	// MODE is _T("RESTORE") or _T("PURCHASE")
	// DELETE��TBL���� ��TBL�� 1 row Move
	// DELETEåTBL���� åTBL n row Move
	// IDX�� SPECIES_CLASS�� 1�� �������ٰ�

	if (strDeleteSpeciesKey.GetLength() == 0) return -32001;	// ������KEY���� 
	INT nRowIdx = 0;

	// Default ���� ���� ��Ų��.
	//SetDefaultValue(nRowIdx, pDM_IDX_BO, pDM_SPECIES, pDM_PURCHASE, pDM_BOOK);

	INT ids;
	CArray <CString, CString> RemoveAliasList;

	m_pDM_DELETE_SPECIES->StartFrame();
	m_pDM_DELETE_BOOK->StartFrame();

	CString strOption, strQuery;
	// �ش� ������DM�� �����Ѵ�.
	strOption.Format(_T("REC_KEY = %s"), strDeleteSpeciesKey);
	ids = m_pDM_DELETE_SPECIES->RefreshDataManager(strOption);
	INT nSpeciesCount = m_pDM_DELETE_SPECIES->GetRowCount();
	if (ids < 0) return -32002;
	// �ش� �������� åDM�� �����Ѵ�.
	strOption.Format(_T("SPECIES_KEY = %s"), strDeleteSpeciesKey);
	ids = m_pDM_DELETE_BOOK->RefreshDataManager(strOption);
	INT nBookCount = m_pDM_DELETE_BOOK->GetRowCount();
	if (ids < 0) return -32003;

	// ���� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BS_MARC"));
	m_pDM_DELETE_SPECIES->InitDBFieldData();
	m_pDM_DELETE_SPECIES->SetCellData(_T("BS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	m_pDM_DELETE_SPECIES->MakeQueryByDM(RemoveAliasList, m_pDM_DELETE_SPECIES, _T("BO_SPECIES_TBL"), nRowIdx, m_pDM_DELETE_SPECIES);
	strQuery.Format(_T("DELETE FROM BO_DELETE_SPECIES_TBL WHERE REC_KEY=%s;"), strDeleteSpeciesKey);
	m_pDM_DELETE_SPECIES->AddFrame(strQuery);
	strQuery.Format(_T("UPDATE IDX_BO_TBL SET SPECIES_CLASS='0', LAST_WORK='%s' WHERE REC_KEY=%s;"), 
		pMgr->GetWorkLogMsg(WORK_LOG), strDeleteSpeciesKey);	// ������ ��� 0
	m_pDM_DELETE_SPECIES->AddFrame(strQuery);
	if (MODE == _T("PURCHASE")) {
		strQuery.Format(_T("UPDATE BO_PURCHASEINFO_TBL SET INPUT_TYPE='1' WHERE SPECIES_KEY=%s;"), strDeleteSpeciesKey);	
		m_pDM_DELETE_SPECIES->AddFrame(strQuery);
	}

	// å������ Insert�� Insert���� �����Ѵ�.
	INT nRowCount = m_pDM_DELETE_BOOK->GetRowCount();
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BB_������"));
	for (INT i = 0; i < nRowCount; i++) {
		if (MODE == _T("PURCHASE")) {
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_�ڷ����"), _T("BOA111N"), i);
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_�Է���"), GetTodayDate(), i);
		}
				
		m_pDM_DELETE_BOOK->SetCellData(_T("BB_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_DELETE_BOOK->InitDBFieldData();
		m_pDM_DELETE_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_DELETE_BOOK, _T("BO_BOOK_TBL"), i, m_pDM_DELETE_SPECIES);
	}
	strQuery.Format(_T("DELETE FROM BO_DELETE_BOOK_TBL WHERE SPECIES_KEY=%s;"), strDeleteSpeciesKey);
	m_pDM_DELETE_SPECIES->AddFrame(strQuery);

	// Send Frame
	ids = SendFrame(m_pDM_DELETE_SPECIES, MAX_TRANSACTION_QUERY_COUNT);
	m_pDM_DELETE_SPECIES->EndFrame();
	if (ids < 0) return -32051;		// Species Insert Error

	// Marc ���� Send Frame
	ids = UpdateSpeciesMarcbyStream(m_pDM_DELETE_SPECIES, _T("BO_SPECIES_TBL"), strDeleteSpeciesKey, nRowIdx);
	m_pDM_DELETE_SPECIES->EndFrame();
	if (ids < 0) return -32053;		// Marc Stream Update Error

	return 0;
}

INT CBO_SPECIES::BO_FURNISH_TO_SPECIES(CString TITLE, CString AUTHOR, CString PUBLISHER, CString PUBLISH_YEAR, CString APPLICANT_NAME, CString FURNISH_KEY, CString &strSPECIES_KEY)
{
	INT ids;
	INT RowIdx = 0;
	
	CString strData, ACQUISIT_YEAR;
	CTime t = CTime::GetCurrentTime();
	ACQUISIT_YEAR.Format(_T("%04d"), t.GetYear());

	// ������ ����
	ids = m_pDM_SPECIES->InsertRow(-1);
	ids = m_pDM_SPECIES->SetCellData(_T("BS_���Գ⵵"), ACQUISIT_YEAR, RowIdx);
	ids = m_pDM_SPECIES->SetCellData(_T("BS_�����ڷᱸ��"), _T("GM"), RowIdx);
	ids = m_pDM_SPECIES->SetCellData(_T("BS_���ı���_�ڵ�"), _T("BK"), RowIdx);
	ids = m_pDM_SPECIES->SetCellData(_T("BS_�ڷᱸ��_�ڵ�"), _T("GM"), RowIdx);
	ids = m_pDM_SPECIES->SetCellData(_T("BS_������ϱ���_�ڵ�"), _T("EL"), RowIdx);
	ids = m_pDM_SPECIES->SetCellData(_T("BS_��������"), _T("1"), RowIdx);
	ids = m_pDM_SPECIES->SetCellData(_T("BS_�۾���"), pMgr->GetUserID(), RowIdx);
	
	// ���� ���� ����
	ids = m_pDM_IDX_BO->InsertRow(-1);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), _T("0"), RowIdx);				// species_class
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_������"), AUTHOR, RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�����ڷᱸ��"), _T("GM"), RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_���ı���_�ڵ�"), _T("BK"), RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�˻���������"), AUTHOR, RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�˻��������"), PUBLISHER, RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�˻��뺻ǥ��"), TITLE, RowIdx);
	//ids = m_pDM_IDX_BO->SetCellData(_T("IBS_����⵵2"), PUBLISH_YEAR, RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�����"), PUBLISH_YEAR, RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�ڷᱸ��_�ڵ�"), _T("GM"), RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_��ü����_�ڵ�"), _T("PR"), RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_������"), PUBLISHER, RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), _T("0"), RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_������ϱ���_�ڵ�"), _T("EL"), RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_��ǥ��"), TITLE, RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_ǥ������"), TITLE, RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�̿����ѱ���_�ڵ�"), _T("GM"), RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�̿��󱸺�_�ڵ�"), _T("PU"), RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_��������_�ڵ�"), _T("1"), RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�۾�����INFO"), _T("BOA111N"), RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�۾���"), pMgr->GetUserID(), RowIdx);
	ids = m_pDM_IDX_BO->SetCellData(_T("IBS_�з�ǥ����"), _T("1"), RowIdx);
	
	// å���� ����
	ids = m_pDM_BOOK->InsertRow(-1);
	ids = m_pDM_BOOK->SetCellData(_T("BB_���Ա���_�ڵ�"), _T("1"), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_���Խ�û��"), APPLICANT_NAME, RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_������"), PUBLISHER, RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_å�η��÷���"), _T("B"), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_��ǰå��"), _T("1"), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_��ǰ����"), _T("Y"), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_��ġ�������KEY"), FURNISH_KEY, RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_�Է���"), GetTodayDate(), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_��������_�ڵ�"), _T("MA"), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_��ϱ���_�ڵ�"), _T("EM"), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_��ü����_�ڵ�"), _T("PR"), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_�����"), PUBLISH_YEAR, RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_���å��"), _T("0"), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_�̿����ѱ���_�ڵ�"), _T("GM"), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_������ηϸ�"), _T(""), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_�ڷ����"), _T("BOA111N"), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_��ǥå����"), _T("Y"), RowIdx);
	ids = m_pDM_BOOK->SetCellData(_T("BB_DB_FLAG"), _T("I"), RowIdx);

	// �������� ����
	ids = m_pDM_PURCHASE->InsertRow(-1);
	ids = m_pDM_PURCHASE->SetCellData(_T("BP_��������"), GetTodayDate(), RowIdx);
	ids = m_pDM_PURCHASE->SetCellData(_T("BP_���Գ⵵"), ACQUISIT_YEAR, RowIdx);
	ids = m_pDM_PURCHASE->SetCellData(_T("BP_��ǰå��"), _T("1"), RowIdx);
	ids = m_pDM_PURCHASE->SetCellData(_T("BP_��ǰ����"), _T("0"), RowIdx);
	ids = m_pDM_PURCHASE->SetCellData(_T("BP_�Է�����"), GetTodayDate(), RowIdx);
	ids = m_pDM_PURCHASE->SetCellData(_T("BP_�Է±���"), _T("2"), RowIdx);
	ids = m_pDM_PURCHASE->SetCellData(_T("BP_�̳�å��"), _T("0"), RowIdx);
	ids = m_pDM_PURCHASE->SetCellData(_T("BP_����å��"), _T("1"), RowIdx);
	ids = m_pDM_PURCHASE->SetCellData(_T("BP_���ְ���"), _T("0"), RowIdx);
	ids = m_pDM_PURCHASE->SetCellData(_T("BP_���ǿϷῩ��"), _T("N"), RowIdx);
	ids = m_pDM_PURCHASE->SetCellData(_T("BP_�ѱǼ�"), _T("1"), RowIdx);

	// ��ũ�� �������ش�.
	CMarc *pMarc = new CMarc;
	if (pMarc) {
		ids = APPLY_Species_to_CMarc(pMarc, m_pDM_IDX_BO, m_pDM_SPECIES, m_pDM_BOOK, m_pDM_PURCHASE);
		pMgr->m_pInfo->pMarcMgr->CheckDeepSyntax(pMarc);
		pMgr->m_pInfo->pMarcMgr->CheckSimpleSyntax(pMarc);
		pMgr->m_pInfo->pMarcMgr->DeleteEmptySubfield(pMarc);
		pMgr->m_pInfo->pMarcMgr->DeleteEmptyField(pMarc);
		CString strStreamMarc;
		ids = pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
		m_pDM_SPECIES->SetCellData(_T("BS_MARC"), strStreamMarc, RowIdx);

		if (pMarc) {
			delete pMarc;
			pMarc = NULL;
		}
	}

	ids = BO_SPECIES_PURCHASE_INSERT(m_pDM_IDX_BO, m_pDM_SPECIES, m_pDM_BOOK, m_pDM_PURCHASE);
	strSPECIES_KEY = m_strSPECIES_KEY;

	return 0;
}

INT CBO_SPECIES::UpdateSpeciesMarcbyCMarc(CESL_DataMgr *pDM_OUT_SPECIES, CString strSpecies_key, CMarc *pMarc)
{
	// ������ DM�� �����´�.
	if (!pDM_OUT_SPECIES) return-1;
	if (!pMarc) return -2;

	INT ids ;
	CString strStreamMarc;
	ids = pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	if(ids < 0) return ids;
	pDM_OUT_SPECIES->InitDBFieldData();
	ids = pDM_OUT_SPECIES->AddDBFieldData(_T("MARC"), _T("STRING"), strStreamMarc, 1);
	if(ids < 0) return ids;
	ids = pDM_OUT_SPECIES->MakeUpdateFrame(pDM_OUT_SPECIES->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);
	if(ids < 0) return ids;
	return 0;
/*
	pDM_OUT_SPECIES->StartFrame();

	pDM_OUT_SPECIES->InitDBFieldData();
	pDM_OUT_SPECIES->AddDBFieldData(_T("MARC"), _T("STRING"), strStreamMarc, -1);
	pDM_OUT_SPECIES->ORACLE_OCIMgr_MakeUpdateFrame(pDM_OUT_SPECIES->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);

	INT ids = pDM_OUT_SPECIES->SendFrame();
	pDM_OUT_SPECIES->EndFrame();
	return ids;
*/
}

INT CBO_SPECIES::UpdateSpeciesMarcbyStream(CESL_DataMgr *pDM_OUT_SPECIES, CString strTBL, CString strSpecies_key, INT RowIdx)
{
	// ������ DM�� �����´�.
	if (!pDM_OUT_SPECIES) return-1;
	INT ids = -1;
	CString strStreamMarc;
	pDM_OUT_SPECIES->GetCellData(_T("BS_MARC"), RowIdx, strStreamMarc);
	pDM_OUT_SPECIES->InitDBFieldData();
	ids = pDM_OUT_SPECIES->AddDBFieldData(_T("MARC"), _T("STRING"), strStreamMarc, 1);
	if(ids < 0) return ids;
	ids = pDM_OUT_SPECIES->MakeUpdateFrame(strTBL, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);
	if(ids < 0) return ids;
	return 0;
/*	
	// ���� strTBL���ڰ� BO_DELETE_SPECIES_TBL�̶�� ��ũ���� 049Tag�� �����ؾ��Ѵ�.
	if(strTBL == _T("BO_DELETE_SPECIES_TBL"))
	{
		CMarc marc;
		pMgr->m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
		pMgr->m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
	}
	ids = pDM_OUT_SPECIES->StartFrame();
	if(ids < 0) return ids;
	pDM_OUT_SPECIES->InitDBFieldData();
	ids = pDM_OUT_SPECIES->AddDBFieldData(_T("MARC"), _T("STRING"), strStreamMarc, -1);
	if(ids < 0) return ids;
	//ids = pDM_OUT_SPECIES->MakeUpdateFrame(strTBL, _T("REC_KEY"), NUMERIC, strSpecies_key);
	//if(ids < 0) return ids;
	ids = pDM_OUT_SPECIES->ORACLE_OCIMgr_MakeUpdateFrame(strTBL, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);
	if(ids < 0) return ids;
	ids = pDM_OUT_SPECIES->SendFrame();
	if(ids < 0) return ids;
	pDM_OUT_SPECIES->EndFrame();
	if(ids < 0) return ids;
	return 0;
*/
}

INT CBO_SPECIES::UpdateSpeciesMarcbyStream(CESL_DataMgr *pDM_OUT_SPECIES, CString strTBL, CString strSpecies_key, CString strMarc)
{
	if (!pDM_OUT_SPECIES) return-1;

	INT ids = -1;
	if(strMarc.IsEmpty()) return 0;

	pDM_OUT_SPECIES->InitDBFieldData();
	ids = pDM_OUT_SPECIES->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	if(ids < 0) return ids;
	ids = pDM_OUT_SPECIES->MakeUpdateFrame(strTBL, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);
	if(ids < 0) return ids;
	return 0;	
/*
	// ���� strTBL���ڰ� BO_DELETE_SPECIES_TBL�̶�� ��ũ���� 049Tag�� �����ؾ��Ѵ�.
	if(strTBL == _T("BO_DELETE_SPECIES_TBL"))
	{
		CMarc marc;
		pMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
		pMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	}
	pDM_OUT_SPECIES->StartFrame();

	pDM_OUT_SPECIES->InitDBFieldData();
	pDM_OUT_SPECIES->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, -1);
	pDM_OUT_SPECIES->ORACLE_OCIMgr_MakeUpdateFrame(strTBL, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);
	INT ids = pDM_OUT_SPECIES->SendFrame();
	pDM_OUT_SPECIES->EndFrame();
	return ids;
*/
}

CString CBO_SPECIES::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

CString CBO_SPECIES::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}


INT CBO_SPECIES::CONVERT_BOOK_to_VOLUMEInBOOK(CESL_DataMgr *pDM_BOOK)
{
	CArray <INT, INT> RemoveRowIdxList;
	RemoveRowIdxList.RemoveAll();

	INT nRowCount = pDM_BOOK->GetRowCount();
	INT i, j, ids;
	CString curVOL_SORT, tempVOL_SORT;
	BOOL IsNewVolume;
	for (i = 0; i < nRowCount; i++) {
		if (pDM_BOOK->GetCellData(_T("BB_å�η��÷���"), i) == _T("A")) continue;
		IsNewVolume = TRUE;
		ids = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i, curVOL_SORT);
		for (j = 0; j < i; j++) {
			if (pDM_BOOK->GetCellData(_T("BB_å�η��÷���"), j) == _T("A")) continue;
			ids = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), j, tempVOL_SORT);
			if (curVOL_SORT == tempVOL_SORT) {
				IsNewVolume = FALSE;
				break;
			}
		}
		if (IsNewVolume == FALSE) RemoveRowIdxList.Add(i);
	}

	INT nRemoveRowIdxList = RemoveRowIdxList.GetSize();
	for (i = nRemoveRowIdxList-1; i >= 0; i--) {
		pDM_BOOK->DeleteRow(RemoveRowIdxList.GetAt(i));
	}
	return 0;
}

// Error Code : -35000 ~
INT CBO_SPECIES::CONVERT_BOOK_to_VOLUME(CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_VOLUME, INT nBeginIdx, INT nRowCnt)
{
	if (!pDM_BOOK || !pDM_VOLUME) return -35001;

	CArray <INT, INT> CopyRowIdxList;
	CopyRowIdxList.RemoveAll();

	INT i, j, ids;
	CString curVOL_SORT, tempVOL_SORT, strDBFLAG;
	BOOL IsNewVolume;
	for (i = nBeginIdx; i <= nRowCnt; i++) {
		IsNewVolume = TRUE;
		ids = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i, curVOL_SORT);
		for (j = nBeginIdx; j < i; j++) {
			ids = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), j, tempVOL_SORT);
			if (curVOL_SORT == tempVOL_SORT) {
				IsNewVolume = FALSE;
				break;
			}
		}
		if (IsNewVolume == TRUE) CopyRowIdxList.Add(i);
	}

	INT nCopyRowIdxList = CopyRowIdxList.GetSize();
	INT RowIdx, nDBookCnt, nBookCnt;
	CString strData;
	for (i = 0; i < nCopyRowIdxList; i++) {
		pDM_VOLUME->InsertRow(-1);
		RowIdx = pDM_VOLUME->GetRowCount()-1;
		// BOOK�� �� Row�� VOLUME���� �����Ѵ�.
		CopyDMRowToDMByAlias(pDM_BOOK, CopyRowIdxList.GetAt(i), pDM_VOLUME, RowIdx);
		// ����� VOLUME�� ���� å���� ���Ͽ� VOLUME�� ����å��UDF �� �־��ش�.
		ids = pDM_VOLUME->GetCellData(_T("BB_���Ϸù�ȣ"), RowIdx, curVOL_SORT);
		nBookCnt = GetEqualBookCount(pDM_BOOK, curVOL_SORT, nBeginIdx, nRowCnt);
		nDBookCnt = GetEqualBookCount(pDM_BOOK, curVOL_SORT, nBeginIdx, nRowCnt, _T("Y"));
		strData.Format(_T("%d"), nBookCnt);
		ids = pDM_VOLUME->SetCellData(_T("UDF_å��"), strData, RowIdx);
		strData.Format(_T("%d"), nDBookCnt);
		ids = pDM_VOLUME->SetCellData(_T("UDF_��ǰå��"), strData, RowIdx);
		strData.Format(_T("%d"), nBookCnt - nDBookCnt);
		ids = pDM_VOLUME->SetCellData(_T("UDF_�̳�å��"), strData, RowIdx);
	}

	return 0;
}

INT CBO_SPECIES::CONVERT_BOOK_to_VOLUME_for_PRINT(CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_VOLUME, INT nBeginIdx, INT nRowCnt)
{
	if (!pDM_BOOK || !pDM_VOLUME) return -35001;
	
	CPriceInfoConversionMgr PriceConvertMgr;
	CArray <INT, INT> CopyRowIdxList;
	CopyRowIdxList.RemoveAll();
	
	INT i, j, ids;
	CString curVOL_SORT, tempVOL_SORT, strDBFLAG;
	BOOL IsNewVolume;
	for (i = nBeginIdx; i <= nRowCnt; i++) {
		IsNewVolume = TRUE;
		ids = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i, curVOL_SORT);
		for (j = nBeginIdx; j < i; j++) {
			ids = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), j, tempVOL_SORT);
			if (curVOL_SORT == tempVOL_SORT) {
				IsNewVolume = FALSE;
				break;
			}
		}
		if (IsNewVolume == TRUE) CopyRowIdxList.Add(i);
	}
	
	INT nCopyRowIdxList = CopyRowIdxList.GetSize();
	INT RowIdx, nDBookCnt, nBookCnt;
	DOUBLE fPrice, dDiscRate;
	CString strData, strTmpData1, strTmpData2;
	for (i = 0; i < nCopyRowIdxList; i++) {
		pDM_VOLUME->InsertRow(-1);
		RowIdx = pDM_VOLUME->GetRowCount()-1;
		// BOOK�� �� Row�� VOLUME���� �����Ѵ�.
		CopyDMRowToDMByAlias(pDM_BOOK, CopyRowIdxList.GetAt(i), pDM_VOLUME, RowIdx);
		// ����� VOLUME�� ���� å���� ���Ͽ� VOLUME�� ����å��UDF �� �־��ش�.
		ids = pDM_VOLUME->GetCellData(_T("BB_���Ϸù�ȣ"), RowIdx, curVOL_SORT);
		nBookCnt = GetEqualBookCount(pDM_BOOK, curVOL_SORT, nBeginIdx, nRowCnt);
		nDBookCnt = GetEqualBookCount(pDM_BOOK, curVOL_SORT, nBeginIdx, nRowCnt, _T("Y"));
		strData.Format(_T("%d"), nBookCnt);
		ids = pDM_VOLUME->SetCellData(_T("UDF_å��"), strData, RowIdx);
		strData.Format(_T("%d"), nDBookCnt);
		ids = pDM_VOLUME->SetCellData(_T("UDF_��ǰå��"), strData, RowIdx);
		strData.Format(_T("%d"), nBookCnt - nDBookCnt);	
		ids = pDM_VOLUME->SetCellData(_T("UDF_�̳�å��"), strData, RowIdx);
		//IBS��ǥ���� ������� �Ǽ��� ǥ��
		strTmpData1 = pDM_VOLUME->GetCellData(_T("BB_�����"), RowIdx);
		strTmpData2 = pDM_VOLUME->GetCellData(_T("BB_�Ǽ���"), RowIdx);
		strData.Format(_T(". %s , %s"), strTmpData1, strTmpData2);
		pDM_VOLUME->SetCellData(_T("IBS_��ǥ��"), strData, RowIdx);
		//IBS_������ �� å�� �����ڸ� ǥ��
		strData = pDM_VOLUME->GetCellData(_T("BB_������"), RowIdx);
		pDM_VOLUME->SetCellData(_T("IBS_������"), strData, RowIdx);
		//����⿡ å�� ����� ǥ��
		strData = pDM_VOLUME->GetCellData(_T("BB_�����"), RowIdx);
		pDM_VOLUME->SetCellData(_T("IBS_�����"), strData, RowIdx);
		//���������� �����Ѵ�.
		dDiscRate = (DOUBLE)atof(pDM_VOLUME->GetCellData(_T("BP_������"), RowIdx).GetBuffer(0));
		fPrice = (DOUBLE)atof(pDM_VOLUME->GetCellData(_T("BB_����"), RowIdx).GetBuffer(0));
		strData.Format(_T("%.0f"), fPrice*nBookCnt);
		PriceConvertMgr.ConversionPriceInfoToDesc(strData, strData);
		pDM_VOLUME->SetCellData(_T("UDF_��ȯ����"), strData, RowIdx);
		pDM_VOLUME->SetCellData(_T("UDF_��ȯ������"), strData, RowIdx);
		strData.Format(_T("%.0f"), fPrice*nBookCnt - ((fPrice*nBookCnt)*(dDiscRate/100)));
		PriceConvertMgr.ConversionPriceInfoToDesc(strData, strData);
		pDM_VOLUME->SetCellData(_T("UDF_��ȯ���ΰ���"), strData, RowIdx);
		pDM_VOLUME->SetCellData(_T("UDF_��ȯ����������"), strData, RowIdx);
		//�ڷ���������� �����Ѵ�.
		strData = pDM_VOLUME->GetCellData(_T("BB_�����"), RowIdx);
		strData = strData.Left(3);
		if(strData == _T("BOA")) strData = _T("�����ڷ�");
		else if(strData == _T("BOC")) strData = _T("�����ڷ�");
		else if(strData == _T("BOR")) strData = _T("����ڷ�");
		else if(strData == _T("BOL")) strData = _T("�����ڷ�");
		pDM_VOLUME->SetCellData(_T("UDF_�ڷ���¼���"), strData, RowIdx);
	}
	
	return 0;
}

// Error Code : -35000 ~
INT CBO_SPECIES::CONVERT_BOOK_to_VOLUME(CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_VOLUME)
{
	if (!pDM_BOOK || !pDM_VOLUME) return -35001;

	CArray <INT, INT> CopyRowIdxList;
	CopyRowIdxList.RemoveAll();

	INT nRowCount = pDM_BOOK->GetRowCount();
	INT i, j, ids;
	CString curVOL_SORT, tempVOL_SORT, strDBFLAG;
	BOOL IsNewVolume;
	for (i = 0; i < nRowCount; i++) {
		strDBFLAG = _T("");
		ids = pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, strDBFLAG);
		if (strDBFLAG == _T("D")) continue;
		if (pDM_BOOK->GetCellData(_T("BB_å�η��÷���"), i) == _T("A")) continue;
		IsNewVolume = TRUE;
		ids = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i, curVOL_SORT);
		for (j = 0; j < i; j++) {
			if (pDM_BOOK->GetCellData(_T("BB_å�η��÷���"), j) == _T("A")) continue;
			if (pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), j) == _T("D")) continue;
			ids = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), j, tempVOL_SORT);
			if (curVOL_SORT == tempVOL_SORT) {
				IsNewVolume = FALSE;
				break;
			}
		}
		if (IsNewVolume == TRUE) CopyRowIdxList.Add(i);
	}

	INT nCopyRowIdxList = CopyRowIdxList.GetSize();
	INT RowIdx = 0;
	CString strData;
	for (i = 0; i < nCopyRowIdxList; i++) {
		pDM_VOLUME->InsertRow(-1);
		// BOOK�� �� Row�� VOLUME���� �����Ѵ�.
		CopyDMRowToDMByAlias(pDM_BOOK, CopyRowIdxList.GetAt(i), pDM_VOLUME, RowIdx);
		// ����� VOLUME�� ���� å���� ���Ͽ� VOLUME�� ����å��UDF �� �־��ش�.
		ids = pDM_VOLUME->GetCellData(_T("BB_�����"), RowIdx, curVOL_SORT);
		strData.Format(_T("%d"), GetEqualBookCount(pDM_BOOK, curVOL_SORT));
		ids = pDM_VOLUME->SetCellData(_T("BB_å��"), strData, RowIdx);

		RowIdx++;
	}

	return 0;
}

INT CBO_SPECIES::MakeBookQueryFrame(CESL_DataMgr *pDM_BOOK, CString strSpecies_key, CString strPurchaseinfo_key, CESL_DataMgr *pDM_OUT, INT nSendCount/*=-1*/)
{
	INT ids, i;
	CString strQuery, strDBFLAG, strBook_key;
	INT nRowCount = pDM_BOOK->GetRowCount();

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	// ���� I/U/D�� ���� Insert/Update/Delete�� �����Ѵ�.
	CString IsREPRESENT_BOOK_YN;
	for (i = 0; i < nRowCount; i++) {
		strDBFLAG = _T("");
		ids = pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, strDBFLAG);
		if (strDBFLAG == _T("I")) {
			pDM_OUT->MakeRecKey(strBook_key); 
			pDM_BOOK->SetCellData(_T("BB_åKEY"), strBook_key, i);
		}
		IsREPRESENT_BOOK_YN = _T("");
		ids = pDM_BOOK->GetCellData(_T("BB_��ǥå����"), i, IsREPRESENT_BOOK_YN);
		IsREPRESENT_BOOK_YN.TrimLeft();		IsREPRESENT_BOOK_YN.TrimRight();
		IsREPRESENT_BOOK_YN.MakeUpper();
		if (IsREPRESENT_BOOK_YN != _T("Y") && IsREPRESENT_BOOK_YN != _T("N")) pDM_BOOK->SetCellData(_T("BB_��ǥå����"), _T("N"), i);
	}

	//����������� ����� ���ؼ� �׸��� ISBN�Է½� Ư����ȣ�� �����ϱ� ���Ͽ�
	CString volIndex, strEAISBN, strBookAppendixFlag;
	//�������
	for (i = 0; i < nRowCount; i++) {
		RemoveAliasList.RemoveAll();
		strDBFLAG = _T("");
		ids = pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, strDBFLAG);
		if (strDBFLAG == _T("I")) {
			RemoveAliasList.Add(_T("BB_������Ϲ�ȣ"));
			pDM_BOOK->SetCellData(_T("BB_��������KEY"), strPurchaseinfo_key, i);
			pDM_BOOK->SetCellData(_T("BB_��KEY"), strSpecies_key, i);
			pDM_BOOK->SetCellData(_T("BB_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), i);
			pDM_BOOK->SetCellData(_T("BB_�����۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), i);
			if (pDM_BOOK->GetCellData(_T("BB_����"), i).GetLength() == 0) pDM_BOOK->SetCellData(_T("BB_����"), _T("0"), i);
			//����������� ���� �� ����ISBN�� Ư����ȣ ������
			strEAISBN = pDM_BOOK->GetCellData(_T("BB_����ISBN"), i); 
			((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strEAISBN);
			pDM_BOOK->SetCellData(_T("BB_����ISBN"), strEAISBN, i);
			volIndex = pDM_BOOK->GetCellData(_T("BB_������ηϸ�"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(volIndex);
				pDM_BOOK->SetCellData(_T("BB_������ηϸ����"), volIndex, i);
			}
			volIndex = pDM_BOOK->GetCellData(_T("BB_�����"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(volIndex);
				pDM_BOOK->SetCellData(_T("BB_���������"), volIndex, i);
			}
			strBook_key = pDM_BOOK->GetCellData(_T("BB_åKEY"), i);
			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_BOOK, _T("BO_BOOK_TBL"), i, pDM_OUT);
		}
		if (strDBFLAG == _T("U")) {
			RemoveAliasList.Add(_T("BB_åKEY"));
			RemoveAliasList.Add(_T("BB_������Ϲ�ȣ"));
			ids = pDM_BOOK->GetCellData(_T("BB_åKEY"), i, strBook_key);
			pDM_BOOK->SetCellData(_T("BB_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), i);
			//����������� ���� �� ����ISBN�� Ư����ȣ ������
			strEAISBN = pDM_BOOK->GetCellData(_T("BB_����ISBN"), i); 
			((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strEAISBN);
			pDM_BOOK->SetCellData(_T("BB_����ISBN"), strEAISBN, i);			
			volIndex = pDM_BOOK->GetCellData(_T("BB_������ηϸ�"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				pDM_BOOK->SetCellData(_T("BB_������ηϸ����"), volIndex, i);
			}
			volIndex = pDM_BOOK->GetCellData(_T("BB_�����"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				pDM_BOOK->SetCellData(_T("BB_���������"), volIndex, i);
			}
			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_BOOK, _T("BO_BOOK_TBL"), i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strBook_key);
			strBook_key = pDM_BOOK->GetCellData(_T("BB_åKEY"), i);
		}
		if (strDBFLAG == _T("D")) {
			pDM_BOOK->GetCellData(_T("BB_åKEY"), i, strBook_key);
			if(strBook_key.IsEmpty()) strBook_key = _T("-1");
			strQuery.Format(_T("DELETE FROM BO_BOOK_TBL WHERE REC_KEY=%s;"), strBook_key);
			pDM_OUT->AddFrame(strQuery);
			//å�� ���������� �����Ҷ� ��Ϲ�ȣ�� �ο��� �ڷῡ ���Ͽ� �ο��Ǿ��� ��Ϲ�ȣ�� ������Ϲ�ȣ�� �߰��Ѵ�.
			CString strRecNo = pDM_BOOK->GetCellData(_T("BB_��Ϲ�ȣ"), i);
			if(!strRecNo.IsEmpty()) InsertToDropAccessionNoTbl(strRecNo, pDM_OUT, TRUE);
		}
	}

	return 0;
}

INT CBO_SPECIES::CopyDMRowToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
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

INT CBO_SPECIES::GetEqualBookCount(CESL_DataMgr *pDM_BOOK, CString &strVOL)
{
	if (!pDM_BOOK) return 0;
	INT nEqualBookCount = 0;
	CString tempVOL;
	INT nBookCount = pDM_BOOK->GetRowCount();
	INT ids;

	for (INT i = 0; i < nBookCount; i++) {
		if (pDM_BOOK->GetCellData(_T("BB_å�η��÷���"), i) == _T("A")) continue;
		ids = pDM_BOOK->GetCellData(_T("BB_�����"), i, tempVOL);
		if (strVOL == tempVOL) nEqualBookCount++;
	}

	return nEqualBookCount;
}

INT CBO_SPECIES::GetEqualBookCount(CESL_DataMgr *pDM_BOOK, CString &strVOL, INT nBeginIdx, INT nRowCnt, CString strDeliveryYN/*=_T("")*/)
{
	if (!pDM_BOOK) return 0;
	INT nEqualBookCount = 0;
	CString tempVOL, strTmpData;
	INT ids;

	for (INT i = nBeginIdx; i <= nRowCnt; i++) {
		ids = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i, tempVOL);
		pDM_BOOK->GetCellData(_T("BB_��ǰ����"), i, strTmpData);
		if (strVOL == tempVOL)
		{
			if(strDeliveryYN.IsEmpty())
				nEqualBookCount++;
			else if(!strDeliveryYN.IsEmpty() && strDeliveryYN == strTmpData)
				nEqualBookCount++;
		}	
	}

	return nEqualBookCount;
}

// Error code : -36000 ~
INT CBO_SPECIES::INDEX_to_CMARC(CESL_DataMgr *pDM_IDX_BO, CMarc *pMarc, INT nRowIdx)
{
	if (!pMgr) return -36001;
	if (!pDM_IDX_BO) return -36002;
	if (!pMarc) return -36003;
	if (nRowIdx < 0) nRowIdx = 0;

	CString strIndexData, strData1, strData2, strData3;
	INT pos;
	// �Ϲ����� ����Ÿ ó��
	for (INT i = 0; i < nIndexMarcRelation; i++) {
		strIndexData = _T("");
		pDM_IDX_BO->GetCellData(IndexMarcRelation_INDEX[i], nRowIdx, strIndexData);
		if (strIndexData.GetLength() > 0) pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, IndexMarcRelation_MARC[i], strIndexData, _T(""), MODIFY_ITEM);
	}

	// Concatenation�ؾ� �ϴ� ����Ÿ�� ó��
	pDM_IDX_BO->GetCellData(_T("IBS_��ǥ��"), nRowIdx, strIndexData);
	pos = strIndexData.Find(_T("="), 0);
	if (pos == -1) {
		strData1 = strIndexData;
		pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("����"), strData1, _T(""), MODIFY_ITEM);
	} else {
		strData1 = strIndexData.Left(pos);
		strData2 = strIndexData.Mid(pos+1);
		pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("����"), strData1, _T(""), MODIFY_ITEM);
		pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("����"), strData2, _T(""), MODIFY_ITEM);
	}

	pDM_IDX_BO->GetCellData(_T("IBS_������"), nRowIdx, strIndexData);
	pos = -1;
	INT nPrevPos = 0;
	pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("����"));
	pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("������"));
	for (i = 0; true; i++) {
		strData1 = _T("");
		pos = strIndexData.Find(';', nPrevPos);
		if (pos == -1) strData1 = strIndexData.Mid(nPrevPos);
			else strData1 = strIndexData.Mid(nPrevPos, pos - nPrevPos);
		
		nPrevPos = pos+1;
		if (i == 0) {
			pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("����"), strData1);
		} else {
			if (strData1.GetLength() > 0) pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("������"), strData1);
		}
		if (pos == -1) break;
	}
	Apply_MARC_700Author(pMarc);

	// ������� 4�ڸ��� �Էµ� ���� 008�� 7~10�� ��ġ�� �־��ش�.
	strIndexData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_�����"), nRowIdx, strIndexData);
	strIndexData.TrimLeft();		strIndexData.TrimRight();
	pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("008@7-40"), strIndexData);

	// Ư���� ����Ÿ ó�� - 020(����)
	SetISBN_to_Marc(pMarc, pDM_IDX_BO, nRowIdx, _T("020"), _T("  "));		// IBS_����_ISBN is 020 blank blank

	// Ư���� ����Ÿ ó�� - 020(��Ʈ)
	SetISBN_to_Marc(pMarc, pDM_IDX_BO, nRowIdx, _T("020"), _T("1 "));		// IBS_��Ʈ_ISBN is 020 1 blank

	return 0;
}

INT CBO_SPECIES::SetISBN_to_Marc(CMarc *pMarc, CESL_DataMgr *pDM_IDX_BO, INT nRowIdx, CString strTag, CString strInd)
{
	CString strData1=_T(""), strData2=_T(""), strData3=_T("");
	CString strMarcData1=_T(""), strMarcData2=_T(""), strMarcData3=_T("");
	CString strIndexData=_T(""), strNewIndexData=_T("");
	CString strPrice, strVol;
	if (strTag == _T("020") && strInd == _T("  ")) {
		strData1 = _T("");		strData2 = _T("");		strData3 = _T("");
		strMarcData1 = _T("");		strMarcData2 = _T("");		strMarcData3 = _T("");
		pDM_IDX_BO->GetCellData(_T("IBS_����ISBN"), nRowIdx, strData1);
		strData1.Replace(_T("-"), _T(""));
		pDM_IDX_BO->GetCellData(_T("IBS_����ISBN�ΰ���ȣ"), nRowIdx, strData2);
		pDM_IDX_BO->GetCellData(_T("IBS_���ǿ���ISBN"), nRowIdx, strData3);
		
		if (strData1.GetLength() > 0) //strMarcData1.Format(_T("$a%s"), strData1);  // 2004.09.07 '$'�� '��'�� �ٲ� �Է�.
			strMarcData1.Format(_T("%ca%s"), SUBFIELD_CODE, strData1); 
		if (strData2.GetLength() > 0) //strMarcData2.Format(_T("$g%s"), strData2);  // 2004.09.07 '$'�� '��'�� �ٲ� �Է�.
			strMarcData2.Format(_T("%cg%s"), SUBFIELD_CODE, strData2);
		if (strData3.GetLength() > 0) //strMarcData3.Format(_T("$z%s"), strData3);  // 2004.09.07 '$'�� '��'�� �ٲ� �Է�.
			strMarcData3.Format(_T("%cz%s"), SUBFIELD_CODE, strData3);
		if (strData1.GetLength() == 0 && strData2.GetLength() == 0 && strData3.GetLength() == 0) {
			;
		} else {
			strNewIndexData.Format(_T("020  %s%s%s"), strMarcData1, strMarcData2, strMarcData3);
		}
	}
	if (strTag == _T("020") && strInd == _T("1 ")) {
		strData1 = _T("");		strData2 = _T("");		strData3 = _T("");
		strMarcData1 = _T("");		strMarcData2 = _T("");		strMarcData3 = _T("");
		pDM_IDX_BO->GetCellData(_T("IBS_��Ʈ_ISBN1"), nRowIdx, strData1);
		strData1.Replace(_T("-"), _T(""));
		if(!strData1.IsEmpty()) strData1 += _T("(��Ʈ)");
		pDM_IDX_BO->GetCellData(_T("IBS_��Ʈ_ISBN2"), nRowIdx, strData2);
		pDM_IDX_BO->GetCellData(_T("IBS_��Ʈ_ISBN3"), nRowIdx, strData3);

		if (strData1.GetLength() > 0) //strMarcData1.Format(_T("$a%s"), strData1);  // 2004.09.07 '$'�� '��'�� �ٲ� �Է�.
			strMarcData1.Format(_T("%ca%s"), SUBFIELD_CODE, strData1); 
		if (strData2.GetLength() > 0) //strMarcData2.Format(_T("$g%s"), strData2);  // 2004.09.07 '$'�� '��'�� �ٲ� �Է�.
			strMarcData2.Format(_T("%cg%s"), SUBFIELD_CODE, strData2);
		if (strData3.GetLength() > 0) //strMarcData3.Format(_T("$z%s"), strData3);  // 2004.09.07 '$'�� '��'�� �ٲ� �Է�.
			strMarcData3.Format(_T("%cz%s"), SUBFIELD_CODE, strData3);

		if (strData1.GetLength() == 0 && strData2.GetLength() == 0 && strData3.GetLength() == 0) {
			;
		} else {
			strNewIndexData.Format(_T("0201  %s%s%s"), strMarcData1, strMarcData2, strMarcData3);
		}
	}
	
	CArray <CString, CString&> Array;
	Array.RemoveAll();
	CString strField;

	CString strTmpInd;
	// 2004.09.07
	strTag.Replace('$', SUBFIELD_CODE);

	pMgr->m_pInfo->pMarcMgr->GetField(pMarc, strTag, strField, &Array);
	INT nSize = Array.GetSize();
	if (nSize == 0) {		// Marc has not any isbn field data
		pMgr->m_pInfo->pMarcMgr->InsertField(pMarc, strNewIndexData);
	} else {						// Marc has some isbn field data
		BOOL bIsUpdate = FALSE;
		for (INT i = 0; i < nSize; i++) {
			if (Array.GetAt(i).GetLength() < 2) continue;
			strTmpInd = Array.GetAt(i).Left(2);

			if (strTmpInd == strInd) {
				strIndexData.Format(_T("%s%s"),  strTag, Array.GetAt(i));
				pMgr->m_pInfo->pMarcMgr->UpdateField(pMarc, strIndexData, strNewIndexData);
				bIsUpdate = TRUE;
				break;
			}
		}
		if (bIsUpdate == FALSE) pMgr->m_pInfo->pMarcMgr->InsertField(pMarc, strNewIndexData);
	}
	return 0;
}

INT CBO_SPECIES::SetISBN_to_Marc_ByVolume(CMarc *pMarc, CESL_DataMgr *pDM_VOLUME, CString strTag, CString strInd)
{
	// _T("020  ") �� ��� ������.
	pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("020"), ' ', ' ');

	CString strData1=_T(""), strData2=_T(""), strData3=_T(""), strData4= _T("");
	CString strMarcData1=_T(""), strMarcData2=_T(""), strMarcData3=_T(""), strMarcData4=_T("");
	CString strIndexData=_T(""), strNewIndexData=_T("");
	CString strPrice, strVol, strCurrencyCode, strTmpData;
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strData1);
	Apply_MARC_950(strData1, strTmpData, strCurrencyCode);

	INT nVolumes = pDM_VOLUME->GetRowCount();
	for (INT i = 0; i < nVolumes; i++) {
		strNewIndexData = _T("");
		strData1 = _T("");		strData2 = _T("");		strData3 = _T("");
		strMarcData1 = _T("");		strMarcData2 = _T("");		strMarcData3 = _T("");
		pDM_VOLUME->GetCellData(_T("BB_����ISBN"), i, strData1);
		pDM_VOLUME->GetCellData(_T("BB_�����"), i, strVol);
		strData1.Replace(_T("-"), _T(""));
		if(!strVol.IsEmpty() && !strData1.IsEmpty()) strData1 += _T("(") + strVol + _T(")");
		pDM_VOLUME->GetCellData(_T("BB_����ISBN�ΰ���ȣ"), i, strData2);
		pDM_VOLUME->GetCellData(_T("BB_���ǿ���ISBN"), i, strData3);
		strPrice = _T("");
		pDM_VOLUME->GetCellData(_T("BB_����"), i, strData4);

		if (strData1.GetLength() > 0) //strMarcData1.Format(_T("$a%s"), strData1);  // 2004.09.07 '$'�� '��'�� �ٲ� �Է�.
			strMarcData1.Format(_T("%ca%s"), SUBFIELD_CODE, strData1); 
		else continue;
		if (strData2.GetLength() > 0) //strMarcData2.Format(_T("$g%s"), strData2);  // 2004.09.07 '$'�� '��'�� �ٲ� �Է�.
			strMarcData2.Format(_T("%cg%s"), SUBFIELD_CODE, strData2); 
		if (strData3.GetLength() > 0) //strMarcData3.Format(_T("$z%s"), strData3);  // 2004.09.07 '$'�� '��'�� �ٲ� �Է�.
			strMarcData3.Format(_T("%cz%s"), SUBFIELD_CODE, strData3); 
		if (strData4.GetLength() > 0) //strMarcData4.Format(_T("$c%s%s"), strCurrencyCode, strData4);  // 2004.09.07 '$'�� '��'�� �ٲ� �Է�.
			strMarcData4.Format(_T("%cc%s%s"), SUBFIELD_CODE, strCurrencyCode, strData4); 
		if (strData1.GetLength() == 0 && strData2.GetLength() == 0 && strData3.GetLength() == 0 && strData4.GetLength() == 0) 
		{
			;
		} 
		else 
		{
			strNewIndexData.Format(_T("020  %s%s%s%s"), strMarcData1, strMarcData2, strMarcData3, strMarcData4);
		}
		if (strNewIndexData.GetLength() > 0) pMgr->m_pInfo->pMarcMgr->InsertField(pMarc, strNewIndexData);
	}
	return 0;
}

INT CBO_SPECIES::CMARC_to_INDEX(CMarc *pMarc, CESL_DataMgr *pDM_IDX_BO, INT nRowIdx)
{
	if (!pMgr) return -36001;
	if (!pDM_IDX_BO) return -36002;
	if (!pMarc) return -36003;
	if (nRowIdx < 0) nRowIdx = 0;

	CString strMarcData1, strMarcData2, strMarcData3, strData, strTmpData;
	//�Ϲ����� �ڷ� ó��
	for (INT i = 0; i < nIndexMarcRelation; i++) {
		strMarcData1 = _T("");
		pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, IndexMarcRelation_MARC[i], strMarcData1);
		pDM_IDX_BO->SetCellData(IndexMarcRelation_INDEX[i], strMarcData1, nRowIdx);	
	}
	// Concatenation�ؾ� �ϴ� ����Ÿ�� ó��
	// IBS_��ǥ�� ó�� - ����, ����, �μ���
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$a"), strMarcData1);
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$x"), strMarcData2);
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$b"), strTmpData);
	if (strMarcData2.GetLength() > 0) 
	{
		strData.Format(_T("%s=%s"), strMarcData1, strMarcData2);	
		if(strTmpData.GetLength() > 0) strData += _T(":") + strTmpData;
	}
	else 
	{
		strData.Format(_T("%s"), strMarcData1);
		if(strTmpData.GetLength()) strData += _T(":") + strTmpData;
	}
	pDM_IDX_BO->SetCellData(_T("IBS_��ǥ��"), strData, nRowIdx);
	// IBS_������ ó��
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strMarcData1);
	strData.Format(_T("%s"), strMarcData1);
	CArray <CString, CString&> arrData;
	arrData.RemoveAll();
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$e"), strMarcData2, &arrData);
	for (i = 0; i < arrData.GetSize(); i++) {
		if (strData.GetLength() == 0) strTmpData.Format(_T("%s"), arrData.GetAt(i));
			else strTmpData.Format(_T("%s;%s"), strData, arrData.GetAt(i));
		strData = strTmpData;
	}
	pDM_IDX_BO->SetCellData(_T("IBS_������"), strData, nRowIdx);
	// ������ ó��
	strData.Empty();
	arrData.RemoveAll();
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("260$b"), strMarcData1, &arrData);
	for (i = 0; i < arrData.GetSize(); i++) {
		if (strData.GetLength() == 0) strTmpData.Format(_T("%s"), arrData.GetAt(i));
			else strTmpData.Format(_T("%s:%s"), strData, arrData.GetAt(i));
		strData = strTmpData;
	}
	pDM_IDX_BO->SetCellData(_T("IBS_������"), strData, nRowIdx);
	
	// Ư���� ����Ÿ ó�� - 020(����)
	FindItemByIndicator(pMarc, _T("020$a"), _T("  "), strMarcData1);
	pDM_IDX_BO->SetCellData(_T("IBS_����ISBN"), strMarcData1, nRowIdx);	
	FindItemByIndicator(pMarc, _T("020$g"), _T("  "), strMarcData1);
	pDM_IDX_BO->SetCellData(_T("IBS_����ISBN�ΰ���ȣ"), strMarcData1, nRowIdx);	
	FindItemByIndicator(pMarc, _T("020$z"), _T("  "), strMarcData1);
	pDM_IDX_BO->SetCellData(_T("IBS_���ǿ���ISBN"), strMarcData1, nRowIdx);	

	// Ư���� ����Ÿ ó�� - 020(��Ʈ)
	FindItemByIndicator(pMarc, _T("020$a"), _T("1 "), strMarcData1);
	pDM_IDX_BO->SetCellData(_T("IBS_��Ʈ_ISBN1"), strMarcData1, nRowIdx);	
	FindItemByIndicator(pMarc, _T("020$g"), _T("1 "), strMarcData1);
	pDM_IDX_BO->SetCellData(_T("IBS_��Ʈ_ISBN2"), strMarcData1, nRowIdx);	
	FindItemByIndicator(pMarc, _T("020$z"), _T("1 "), strMarcData1);
	pDM_IDX_BO->SetCellData(_T("IBS_��Ʈ_ISBN3"), strMarcData1, nRowIdx);	

	// 008�� 7-10�� ������̴�. ������� �������ش�.
	strMarcData1 = _T("");
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@7-10"), strMarcData1);
	strMarcData1.TrimLeft();		strMarcData1.TrimRight();
	if (strMarcData1.GetLength() == 4) pDM_IDX_BO->SetCellData(_T("IBS_�����"), strMarcData1, nRowIdx);	
	return 0;
}

INT CBO_SPECIES::FindItemByIndicator(CMarc *pMarc, CString strTag, CString strInd, CString &strData)
{
	strData = _T("");
	CArray <CString, CString&> IndArray;
	CArray <CString, CString&> Array;

	CString strTmp, strTmpInd;
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, strTag, strTmp, &Array, &IndArray);
	INT nArrayRow = IndArray.GetSize();
	for (INT i = 0; i < nArrayRow; i++) {
		strTmpInd = IndArray.GetAt(i).Left(2);
		if (strTmpInd == strInd) {
			strData = Array.GetAt(i);
			return 0;
		}
	}
	return -1;
}

INT CBO_SPECIES::INDEX_to_SPECIES(CESL_DataMgr *pDM_IDX_BO, INT nIndexRow, CESL_DataMgr *pDM_SPECIES, INT nSpeciesRow)
{
	INT ids;
	CString strData;
	for (INT i = 0; i < nIndexSpeciesRelation; i++) {
		ids = pDM_IDX_BO->GetCellData(IndexSpeciesRelation_INDEX[i], nIndexRow, strData);
		ids = pDM_SPECIES->SetCellData(IndexSpeciesRelation_SPECIES[i], strData, nSpeciesRow);
	}
	return 0;
}

INT CBO_SPECIES::INDEX_to_BOOK(CESL_DataMgr *pDM_IDX_BO, INT nIndexRow, CESL_DataMgr *pDM_BOOK, INT nBookRow)
{
	INT ids;
	CString strData;
	for (INT i = 0; i < nIndexBookRelation; i++) {
		ids = pDM_IDX_BO->GetCellData(IndexBookRelation_INDEX[i], nIndexRow, strData);
		ids = pDM_BOOK->SetCellData(IndexBookRelation_Book[i], strData, nBookRow);
	}
	return 0;
}


INT CBO_SPECIES::SetDefaultValue(INT nRowIdx, CESL_DataMgr *pDM_IDX_BO, CESL_DataMgr *pDM_SPECIES, CESL_DataMgr *pDM_ACCU, CESL_DataMgr *pDM_BOOK)
{
	SPECIES_TYPE sType = (SPECIES_TYPE)m_nSpeciesType;

	if (pDM_IDX_BO) SetDefaultValue_IDX(pDM_IDX_BO, nRowIdx);
	if (pDM_SPECIES) SetDefaultValue_SPECIES(pDM_SPECIES, nRowIdx);
	if (pDM_ACCU && pDM_BOOK) 
	{
		if(sType == BO_PURCHASE || sType == BO_ORDER || sType == BO_REG_PURCHASE || sType == BO_MISSING || (sType == BO_REGOBJ && pDM_BOOK->GetCellData(_T("BB_���Ա���_�ڵ�"), 0) == _T("1")))
			SetDefaultValue_PURCHASEINFO(pDM_ACCU, nRowIdx, pDM_BOOK);
		else if(sType == BO_DONATE || sType == BO_NONDONATE || sType == BO_REG_DONATE || (sType == BO_REGOBJ && pDM_BOOK->GetCellData(_T("BB_���Ա���_�ڵ�"), 0) == _T("2")))
			SetDefaultValue_DONATEINFO(pDM_ACCU, nRowIdx, pDM_BOOK);
	}
	if (pDM_BOOK) SetDefaultValue_BOOK(pDM_BOOK, pDM_ACCU, nRowIdx);
	
	return 0;
}

INT CBO_SPECIES::SetDefaultValue_IDX(CESL_DataMgr *pDM_IDX_BO, INT nRowIdx)
{
	CString worker = pMgr->m_pInfo->USER_ID;
	pDM_IDX_BO->SetCellData(_T("IBS_�۾���"), worker, nRowIdx);
	if(m_nOpenMode > 0)
	{
		pDM_IDX_BO->SetCellData(_T("IBS_�����۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
		pDM_IDX_BO->SetCellData(_T("IBS_û��_������ȣ"), _T(""), nRowIdx);
		pDM_IDX_BO->SetCellData(_T("IBS_û��_�з���ȣ"), _T(""), nRowIdx);
		pDM_IDX_BO->SetCellData(_T("IBS_�����ȣ"), _T(""), nRowIdx);
		pDM_IDX_BO->SetCellData(_T("IBS_������ȣINFO"), _T(""), nRowIdx);
		pDM_IDX_BO->SetCellData(_T("IBS_�ڷ��ִ°�INFO_�ڵ�"), _T(""), nRowIdx);
		pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), _T("0"), nRowIdx);
		pDM_IDX_BO->SetCellData(_T("IBS_û��_��å��ȣ"), _T(""), nRowIdx);
	}
	pDM_IDX_BO->SetCellData(_T("IBS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	return 0;
}

INT CBO_SPECIES::SetDefaultValue_SPECIES(CESL_DataMgr *pDM_SPECIES, INT nRowIdx)
{
	//�� �۾��� ���� ����
	CString worker = pMgr->m_pInfo->USER_ID;
	if(m_nOpenMode > 0)
	{
		pDM_SPECIES->SetCellData(_T("BS_�Է���"), GetTodayDate(), nRowIdx);
		pDM_SPECIES->SetCellData(_T("BS_�۾����±���"),_T("1"), nRowIdx);
		pDM_SPECIES->SetCellData(_T("BS_��������"),_T("N"), nRowIdx);
		pDM_SPECIES->SetCellData(_T("BS_�����۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
		pDM_SPECIES->SetCellData(_T("BS_������������KEY"), _T("-1"), nRowIdx);
		pDM_SPECIES->SetCellData(_T("BS_�۾���"), worker, nRowIdx);
		SPECIES_TYPE sType = (SPECIES_TYPE)m_nSpeciesType;
		if(sType == BO_REG_INVALID || sType == BO_REG_PURCHASE || sType == BO_REG_DONATE)
			pDM_SPECIES->SetCellData(_T("BS_����ΰ�����"), GetTodayDate(), nRowIdx);
		pDM_SPECIES->SetCellData(_T("BS_�����ȣ"), _T(""), nRowIdx);
		pDM_SPECIES->SetCellData(_T("BS_��������"), _T(""), nRowIdx);
		pDM_SPECIES->SetCellData(_T("BS_DDC�з���ȣ"), _T(""), nRowIdx);
		pDM_SPECIES->SetCellData(_T("BS_DDC����"), _T(""), nRowIdx);
		pDM_SPECIES->SetCellData(_T("BS_KDC�з���ȣ"), _T(""), nRowIdx);
		pDM_SPECIES->SetCellData(_T("BS_KDC����"), _T(""), nRowIdx);
		pDM_SPECIES->SetCellData(_T("BS_�谡��ġ��ȣ"), _T(""), nRowIdx);
	}	
	pDM_SPECIES->SetCellData(_T("BS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	
	return 0;
}
INT CBO_SPECIES::SetDefaultValue_DONATEINFO(CESL_DataMgr *pDM_DONATE, INT nRowIdx, CESL_DataMgr *pDM_BOOK)
{
	CString ORDER_BOOK_CNT, INPUT_TYPE, INPUT_DATE;
	ORDER_BOOK_CNT.Format(_T("%d"), GetBookCount(pDM_BOOK));
	INPUT_TYPE = _T("0");
	if (m_nOpenMode == 1100 || m_nOpenMode == 3100 || m_nOpenMode == 1200) INPUT_TYPE = _T("0");
	if (m_nOpenMode == -6000) INPUT_TYPE = _T("1");
	//if (m_nOpenMode == -5000) INPUT_TYPE = _T("2");
	if (m_nOpenMode == 5000 || m_nOpenMode == 5100) INPUT_TYPE = _T("2");
	if (m_nOpenMode == 4000) INPUT_TYPE = _T("3");
	INPUT_DATE = GetTodayDate();

	pDM_DONATE->SetCellData(_T("BP_����å��"), ORDER_BOOK_CNT, nRowIdx);
	if(m_nOpenMode > 0)
	{
		pDM_DONATE->SetCellData(_T("BP_�Է±���"), INPUT_TYPE, nRowIdx);
		pDM_DONATE->SetCellData(_T("BP_�Է�����"), INPUT_DATE, nRowIdx);
		pDM_DONATE->SetCellData(_T("BP_���Գ⵵"), GetTodayYear(), nRowIdx);
		pDM_DONATE->SetCellData(_T("BP_��������"), GetTodayDate(), nRowIdx);
		pDM_DONATE->SetCellData(_T("BP_�˼���"), GetTodayDate(), nRowIdx);
	}

	return 0;	
}

INT CBO_SPECIES::SetDefaultValue_PURCHASEINFO(CESL_DataMgr *pDM_PURCHASE, INT nRowIdx, CESL_DataMgr *pDM_BOOK)
{
	/*
	* �������� 
		- ����å�� : ��å�� 
		- �Է±���: BP_�Է±��� : �Է±���(0:�Է�, 1:����, 2:��ġ���, 3:���Ϲ���) , �⺻���� 0 ��
		- �Է��� : BP_�Է����� : �Է��� 
	*/
	
	CString ORDER_BOOK_CNT, INPUT_TYPE, INPUT_DATE;
	ORDER_BOOK_CNT.Format(_T("%d"), GetBookCount(pDM_BOOK));
	INPUT_TYPE = _T("0");
	if (m_nOpenMode == 1100 || m_nOpenMode == 3100 || m_nOpenMode == 1200) INPUT_TYPE = _T("0");
	if (m_nOpenMode == -6000) INPUT_TYPE = _T("1");
	//if (m_nOpenMode == -5000) INPUT_TYPE = _T("2");
	if (m_nOpenMode == 5000) INPUT_TYPE = _T("2");
	if (m_nOpenMode == 4000) INPUT_TYPE = _T("3");
	INPUT_DATE = GetTodayDate();
	SPECIES_TYPE sType = (SPECIES_TYPE)m_nSpeciesType;
	if(m_nOpenMode > 0)
	{
		pDM_PURCHASE->SetCellData(_T("BP_�Է±���"), INPUT_TYPE, nRowIdx);
		pDM_PURCHASE->SetCellData(_T("BP_�Է�����"), INPUT_DATE, nRowIdx);
		pDM_PURCHASE->SetCellData(_T("BP_�����۾�"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
		if(sType == BO_ORDER || sType == BO_REG_PURCHASE ||sType == BO_REGOBJ || sType == BO_MISSING)
		{
			pDM_PURCHASE->SetCellData(_T("BP_���ǿϷῩ��"), _T("Y"), nRowIdx);
			pDM_PURCHASE->SetCellData(_T("BP_���ּ�����"), GetTodayDate(), nRowIdx);
			pDM_PURCHASE->SetCellData(_T("BP_��������"), GetTodayDate(), nRowIdx);
			CString strGroupNo = pDM_PURCHASE->GetCellData(_T("BP_�׷��ȣ"), nRowIdx);
			CString strAcqYear = pDM_PURCHASE->GetCellData(_T("BP_���Գ⵵"), nRowIdx);
			CString strQuery, strResult;
			INT nLastNum;
			strQuery.Format(_T("SELECT MAX(SERIAL_NO) FROM BO_PURCHASEINFO_TBL WHERE ORDER_YEAR = '%s' AND ORDER_SEQ_NO = %s"), strAcqYear, strGroupNo);
			pDM_PURCHASE->GetOneValueQuery(strQuery, strResult);
			nLastNum = _ttoi(strResult.GetBuffer(0));		nLastNum++;
			strResult.Format(_T("%d"), nLastNum);
			pDM_PURCHASE->SetCellData(_T("BP_�Ϸù�ȣ"), strResult, nRowIdx);
			if(sType == BO_REG_PURCHASE || sType == BO_REGOBJ) pDM_PURCHASE->SetCellData(_T("BP_�˼���"), GetTodayDate(), nRowIdx);
		}
	}
	pDM_PURCHASE->SetCellData(_T("BP_����å��"), ORDER_BOOK_CNT, nRowIdx);
	pDM_PURCHASE->SetCellData(_T("BP_�������۾�"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	if (pDM_PURCHASE->GetCellData(_T("BP_������"), nRowIdx).IsEmpty()) pDM_PURCHASE->SetCellData(_T("BP_������"), _T("0"), nRowIdx);
	if (pDM_PURCHASE->GetCellData(_T("BP_����"), nRowIdx).IsEmpty()) pDM_PURCHASE->SetCellData(_T("BP_����"), _T("0"), nRowIdx);
	if (pDM_PURCHASE->GetCellData(_T("BP_ȯ��"), nRowIdx).IsEmpty()) pDM_PURCHASE->SetCellData(_T("BP_ȯ��"), _T("1"), nRowIdx);
	if(pDM_PURCHASE->GetCellData(_T("BP_�׷��ȣ"), nRowIdx) == _T("�������")) pDM_PURCHASE->SetCellData(_T("BP_�׷��ȣ"), _T(""), nRowIdx);
	if(sType == BO_REG_PURCHASE || sType == BO_REGOBJ)
	{
		pDM_PURCHASE->SetCellData(_T("BP_��ǰå��"), ORDER_BOOK_CNT, nRowIdx);
		pDM_PURCHASE->SetCellData(_T("BP_�̳�å��"), _T("0"), nRowIdx);
	}
	else if(sType == BO_MISSING)
	{
		pDM_PURCHASE->SetCellData(_T("BP_��ǰå��"), _T("0"), nRowIdx);
		pDM_PURCHASE->SetCellData(_T("BP_�̳�å��"), ORDER_BOOK_CNT, nRowIdx);	
	}
	return 0;
}

INT CBO_SPECIES::SetDefaultValue_BOOK(CESL_DataMgr *pDM_BOOK, CESL_DataMgr * pDM_ACCU, INT nRowIdx) 
{
	/*
	å ���� 
	- ���Ա��� : BB_���Ա���_�ڵ� : ���Ա���(1-����,2-����) �⺻���� 1��
	- �Է��� : BB_�Է��� : �Է���
	*/
	SPECIES_TYPE sType = (SPECIES_TYPE)m_nSpeciesType;

	CString INPUT_DATE, strTmpData;
	INPUT_DATE = GetTodayDate();

	INT nLastBookSerialNo = -1;
	CString strLastSerialNo;

	INT nRow = pDM_BOOK->GetRowCount();
	for (INT i = 0; i < nRow; i++) 
	{
		if(pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i) == _T("I") )
		{
			pDM_BOOK->SetCellData(_T("BB_���̺���±���"), _T("1"), i);
			pDM_BOOK->SetCellData(_T("BB_å�η��÷���"), _T("B"), i);
			pDM_BOOK->SetCellData(_T("BB_�Է���"), INPUT_DATE, i);
			strTmpData = pDM_BOOK->GetCellData(_T("BB_�ڷ����"), i);
			if(strTmpData != _T("BOA113O")) pDM_BOOK->SetCellData(_T("BB_��ǰ����"), _T("Y"), i);
			else pDM_BOOK->SetCellData(_T("BB_��ǰ����"), _T("N"), i);
			if(sType == BO_REG_PURCHASE || sType == BO_REG_DONATE || sType == BO_DONATE || sType == BO_REG_INVALID || sType == BO_REGOBJ)
				pDM_BOOK->SetCellData(_T("BB_�˼���"), GetTodayDate(), i);
			if(sType == BO_REG_PURCHASE || sType == BO_REG_DONATE || sType == BO_REG_INVALID || sType == BO_REGOBJ)
				pDM_BOOK->SetCellData(_T("BB_����ΰ�����"), GetTodayDate(), i);
		}
	}
	return 0;
}

INT CBO_SPECIES::FileLog(CString str)
{
	if (FILELOG != _T("TRUE")) return 0;
	FILE *fDebug = _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));
	if (fDebug == NULL) return 0;

	FILE * f = _tfopen(_T("_BO_SPECIES_API.log"), _T("a+b"));
	if (!f) return -1;
	fseek(f, 0, SEEK_END);		
	if( 0 == ftell(f) )
	{
		/*UNCHANGE*/fputc(0xFF, f);
		/*UNCHANGE*/fputc(0xFE, f);
	}
	_ftprintf(f, _T("%s\r\n"), str);
	fclose(f);
	return 0;
}

INT CBO_SPECIES::APPLY_IndexItem(CESL_DataMgr *pDM_IDX_BO, CMarc *pMarc, INT nRowIdx) 
{
	((CIndexTool*)pMgr->m_pInfo->pIndexTool)->Init(pMgr->m_pInfo->pMarcMgr, pMarc);
	
	//m_cIndexTool.Init(pMgr->m_pInfo->pMarcMgr, pMarc);
	
	INT nIndexToolKit = 0;
	CString IndexToolKit[MAX_TEMP_STRING_ARRAY_SIZE];
	CString IndexToolKitCommand[MAX_TEMP_STRING_ARRAY_SIZE];
	IndexToolKit[nIndexToolKit] = _T("IBS_�˻���������");			IndexToolKitCommand[nIndexToolKit] = _T("������");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�����˻������⵵");		IndexToolKitCommand[nIndexToolKit] = _T("�����˻������⵵");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�˻�������˻�������");	IndexToolKitCommand[nIndexToolKit] = _T("�����˻��������");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�˻������Ŀ뺻ǥ��");		IndexToolKitCommand[nIndexToolKit] = _T("��ǥ��");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�˻���Ű����");			IndexToolKitCommand[nIndexToolKit] = _T("Ű����");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�˻��������");			IndexToolKitCommand[nIndexToolKit] = _T("������");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�˻��뺻ǥ��");			IndexToolKitCommand[nIndexToolKit] = _T("��ǥ��");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�����ڻ���");				IndexToolKitCommand[nIndexToolKit] = _T("�����ڻ���");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_�����ڻ���");				IndexToolKitCommand[nIndexToolKit] = _T("�����ڻ���");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_ǥ������");				IndexToolKitCommand[nIndexToolKit] = _T("ǥ������");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_����⵵����");			IndexToolKitCommand[nIndexToolKit] = _T("����⵵����");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_��������");				IndexToolKitCommand[nIndexToolKit] = _T("��������");			nIndexToolKit++;

	INT i;
	CString strTag, strData, strIdxTitle, strIdxAuthor, strIdxPublihser, strIdxKeyword;
	for (i = 0; i < nIndexToolKit; i++) {
		strData.Empty();
		((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex(IndexToolKitCommand[i], strData);
		pDM_IDX_BO->SetCellData(IndexToolKit[i], strData, nRowIdx);
	}
	
	//IDX_ALL_ITEM�ʵ带 ä��� ���ؼ� �˻��뺻ǥ��, ������, ������, Ű���� �� �����Ѵ�.
	pDM_IDX_BO->GetCellData(_T("IBS_�˻��뺻ǥ��"), nRowIdx, strIdxTitle);
	pDM_IDX_BO->GetCellData(_T("IBS_�˻���������"), nRowIdx, strIdxAuthor);
	pDM_IDX_BO->GetCellData(_T("IBS_�˻��������"), nRowIdx, strIdxPublihser);
	pDM_IDX_BO->GetCellData(_T("IBS_�˻���Ű����"), nRowIdx, strIdxKeyword);

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_�˻���������"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strData);
		DeleteUnlessWord(strData);
		pDM_IDX_BO->SetCellData(_T("IBS_�˻���������"), strData, nRowIdx);
		strIdxAuthor = strData;
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_������"), nRowIdx, strData);
	if (strData.GetLength() == 0) 
	{
		strData = _T("");
		// ��Ģ1 : 245$d
		// ��Ģ1�� ������� ��Ģ2 : ����(���ı���TX)->100, �񵵼�(���ı���!=TX)->700

		// ��Ģ1 ����
		strData = _T("");
		pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strData);
		DeleteUnlessWord(strData);
		pDM_IDX_BO->SetCellData(_T("IBS_������"), strData, nRowIdx);
		
/*
		CString strFORM_CODE;
		// ��Ģ2 or 3����
		if (strData.GetLength() == 0) {
			pDM_IDX_BO->GetCellData(_T("IBS_���ı���_�ڵ�"), nRowIdx, strFORM_CODE);
			strData = _T("");
			if (strFORM_CODE == _T("BK")) {
				pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), strData);
				pDM_IDX_BO->SetCellData(_T("IBS_������"), strData, nRowIdx);
			} else {
				pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("700$a"), strData);
				pDM_IDX_BO->SetCellData(_T("IBS_������"), strData, nRowIdx);
			}
		}
*/
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_��ǥ��"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		pDM_IDX_BO->GetCellData(_T("IBS_�˻��뺻ǥ��"), nRowIdx, strData);
		pDM_IDX_BO->SetCellData(_T("IBS_��ǥ��"), strData, nRowIdx);
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_�˻�������˻�������"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex(_T("������"), strData);
		pDM_IDX_BO->SetCellData(_T("IBS_�˻�������˻�������"), strData, nRowIdx);
//		pDM_IDX_BO->SetCellData(_T("IBS_������"), strData, nRowIdx);
//		pDM_IDX_BO->SetCellData(_T("IBS_�����ڻ���"), strData, nRowIdx);
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_������"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex(_T("������"), strData);
		pDM_IDX_BO->SetCellData(_T("IBS_������"), strData, nRowIdx);
//		pDM_IDX_BO->SetCellData(_T("IBS_�����ڻ���"), strData, nRowIdx);
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_�����"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex(_T("�����˻������⵵"), strData);
		pDM_IDX_BO->SetCellData(_T("IBS_�����"), strData, nRowIdx);
	}
	
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
		if(strIdxAllItem.IsEmpty()) strIdxAllItem = TargetArray.GetAt(i);
		else strIdxAllItem += _T(" ") + TargetArray.GetAt(i);
	}
	pDM_IDX_BO->SetCellData(_T("IBS_��ü�˻�����ξ�"), strIdxAllItem, nRowIdx);
	return 0;
}


INT CBO_SPECIES::APPLY_IDX_APPENDIX_YN(CESL_DataMgr *pDM_IDX_BO, CESL_DataMgr *pDM_BOOK, INT nINDEX_RowIdx)
{
	if (!pDM_IDX_BO) return -1;
	if (!pDM_BOOK) return -2;

	INT nRowCount = pDM_BOOK->GetRowCount();
	CString result = _T("N");
	for (INT i = 0; i < nRowCount; i++) {
		if (pDM_BOOK->GetCellData(_T("BB_å�η��÷���"), i) == _T("A")) {
			result = _T("Y");
			break;
		}
	}
	pDM_IDX_BO->SetCellData(_T("IBS_�η�����"), result, nINDEX_RowIdx);
	return 0;
}

INT CBO_SPECIES::APPLY_SetAppendixInfo_to(CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_OUT)
{
	INT i, j, ids;
	INT nRowCount = pDM_BOOK->GetRowCount();
	CString strBOOK_key, strParentUniqueKey, APPENDIX_FLAG, DB_FLAG;

	CArray <CString, CString> UpdateToYesList;
	CArray <CString, CString> UpdateToNoList;
	
	for (i = 0; i < nRowCount; i++) {
		ids = pDM_BOOK->GetCellData(_T("BB_å�η��÷���"), i, APPENDIX_FLAG);
		if (APPENDIX_FLAG == _T("A")) continue;		
		ids = pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, DB_FLAG);
		if (DB_FLAG != _T("I")) continue;
		ids = pDM_BOOK->GetCellData(_T("BB_åKEY"), i, strBOOK_key);
		ids = pDM_BOOK->GetCellData(_T("BB_UNIQUE_KEY"), i, strParentUniqueKey);
		if (strParentUniqueKey.GetLength() == 0) continue;
		if (strParentUniqueKey.GetAt(0) != 'U') continue;

		for (j = 0; j < nRowCount; j++) {
			if (pDM_BOOK->GetCellData(_T("BB_å�η��÷���"), j) == _T("B")) continue;
			if (pDM_BOOK->GetCellData(_T("BB_�θ�åKEY"), j) == strParentUniqueKey) {
				pDM_BOOK->SetCellData(_T("BB_�θ�åKEY"), strBOOK_key, j);
			}
		}	
	}

	for (i = 0; i < nRowCount; i++) {
		ids = pDM_BOOK->GetCellData(_T("BB_å�η��÷���"), i, APPENDIX_FLAG);
		if (APPENDIX_FLAG == _T("B")) continue;		// �ηϿ� ���� ó���� ��
		ids = pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, DB_FLAG);
		if (DB_FLAG == _T("I")) {
			ids = pDM_BOOK->GetCellData(_T("BB_�θ�åKEY"), i, strBOOK_key);
			if (IsInCArray(&UpdateToYesList, strBOOK_key) == FALSE) UpdateToYesList.Add(strBOOK_key);
		}
		if (DB_FLAG == _T("D")) {
			ids = pDM_BOOK->GetCellData(_T("BB_�θ�åKEY"), i, strBOOK_key);
			if (IsInCArray(&UpdateToNoList, strBOOK_key) == FALSE) UpdateToNoList.Add(strBOOK_key);
		}
	}
	
	nRowCount = UpdateToNoList.GetSize();
	for (i = 0; i < nRowCount; i++) {
		if (GetAppendixCount(pDM_BOOK, UpdateToNoList.GetAt(i)) > 0) {
			UpdateToNoList.SetAt(i, _T(""));	
		}
	}

	CString sQuery;
	// Make Appendix No
	nRowCount = UpdateToNoList.GetSize();
	for (i = 0; i < nRowCount; i++) {
		if (UpdateToNoList.GetAt(i).GetLength() > 0) {
			sQuery.Format(_T("UPDATE BO_BOOK_TBL SET APPENDIX_YN='N' WHERE REC_KEY = %s;"), UpdateToNoList.GetAt(i));
			FileLog(sQuery);
			pDM_OUT->AddFrame(sQuery);
		}
	}
	// Make Appendix Yes
	nRowCount = UpdateToYesList.GetSize();
	for (i = 0; i < nRowCount; i++) {
		sQuery.Format(_T("UPDATE BO_BOOK_TBL SET APPENDIX_YN='Y' WHERE REC_KEY = %s;"), UpdateToYesList.GetAt(i));
		pDM_OUT->AddFrame(sQuery);
	}
	return 0;
}	

INT CBO_SPECIES::GetAppendixCount(CESL_DataMgr *pDM_BOOK, CString NoBookKey)
{
	INT nRowCount = pDM_BOOK->GetRowCount();
	
	for (INT i = 0; i < nRowCount; i++) {
		if (pDM_BOOK->GetCellData(_T("BB_å�η��÷���"), i) != _T("A")) continue;
		if (pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i) == _T("D")) continue;

		if (pDM_BOOK->GetCellData(_T("BB_�θ�åKEY"), i) == NoBookKey) return 1;
	}
	return 0;
}

BOOL CBO_SPECIES::IsInCArray(CArray <CString, CString> *pList, CString NewValue)
{
	INT nRowCount = pList->GetSize();
	for (INT i = 0; i < nRowCount; i++) {
		if (NewValue == pList->GetAt(i)) return TRUE;
	}
	return FALSE;
}

INT CBO_SPECIES::SendFrame(CESL_DataMgr *pDM_OUT, INT nSendCount)
{
	if (!pDM_OUT) return -1;
	if (nSendCount <= 0) return -2;

	INT ids;
	while(TRUE) {
		if (pDM_OUT->frame.IsEmpty() == TRUE) break;
		ids = pDM_OUT->ORACLE_SendFrame2(nSendCount);
		if (ids < 0) return ids;
	}
	return 0;
}


INT CBO_SPECIES::BO_BOOK_OPERATION(CESL_DataMgr *pDM_BOOK, CString strSpecies_key, CString strPurchaseinfo_key)
{
	//�Ǹ��� �Է�, �����̶�� ���� åTBL�� �ǵ帮�� ���̴�.
	CArray <CString, CString> RemoveAliasList;

	pDM_BOOK->StartFrame();

	INT ids;
	CString strBook_key;
	m_strSPECIES_KEY = strSpecies_key;

	// Default ���� ���� ��Ų��.
	SetDefaultValue(0, NULL, NULL, NULL, pDM_BOOK);

	// å������ Insert�� Insert���� �����Ѵ�.
	MakeBookQueryFrame(pDM_BOOK, strSpecies_key, strPurchaseinfo_key, pDM_BOOK);

	// Query ���� Send Frame
	ids = SendFrame(pDM_BOOK, MAX_TRANSACTION_QUERY_COUNT);
	pDM_BOOK->EndFrame();
	if (ids < 0) return -33051;		// Species Insert Error

	return 0;
}

INT CBO_SPECIES::BO_DELETE_SPECIES_PURCHASE_UPDATE(CESL_DataMgr *pDM_IDX_BO, CESL_DataMgr *pDM_SPECIES, CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_PURCHASE, INT nRowIdx)
{
	// ����-��-����-������ ����
	// ���� 1 row update
	// ���������� 1 row update
	// (å)�������� n rows update

	CArray <CString, CString> RemoveAliasList;

	INT ids;
	CString strSpecies_key, strPurchaseinfo_key, strBook_key;
	ids = pDM_SPECIES->GetCellData(_T("BS_��KEY"), nRowIdx, strSpecies_key);
	ids = pDM_PURCHASE->GetCellData(_T("BP_��������KEY"), nRowIdx, strPurchaseinfo_key);

	pDM_BOOK->StartFrame();
	pDM_SPECIES->StartFrame();

	// ���� Update���� �����Ѵ�.
	INDEX_to_SPECIES(pDM_IDX_BO, nRowIdx, pDM_SPECIES, nRowIdx);		// ������ ������ ���� ������� �ش�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BS_MARC"));
	RemoveAliasList.Add(_T("BS_��KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("BO_DELETE_SPECIES_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);

	// �˻� �ʵ带 ���ο� �ϼ��Ѵ�.
	CMarc *pMarc = new CMarc;
	CString strStreamMarc;
	pDM_SPECIES->GetCellData(_T("BS_MARC"), nRowIdx, strStreamMarc);
	pMgr->m_pInfo->pMarcMgr->Decoding(strStreamMarc, pMarc);		
	// �˻� �ʵ带 ���ο� �ϼ��Ѵ�.
	ids = APPLY_IndexItem(pDM_IDX_BO, pMarc, nRowIdx);
	if (pMarc) {
		delete pMarc;
		pMarc = NULL;
	}

	// ���ο� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("IBS_REC_KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX_BO, _T("IDX_BO_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);

	// Query ���� Send Frame
	ids = SendFrame(pDM_SPECIES, MAX_TRANSACTION_QUERY_COUNT);
	pDM_SPECIES->EndFrame();
	if (ids < 0) return -34051;		// Species Insert Error

	// Marc ���� Send Frame
	ids = UpdateSpeciesMarcbyStream(pDM_SPECIES, _T("BO_SPECIES_TBL"), strSpecies_key, nRowIdx);
	pDM_SPECIES->EndFrame();
	if (ids < 0) return -34053;		// Marc Stream Update Error

	return 0;
}

/*
// Error Code : -37000 ~
INT CBO_SPECIES::BO_SPECIES_FILE_IMPORT(
										CString MODE, 
										CString IMPORTSTATUS, 
										CESL_DataMgr *pDM_IDX_BO, 
										CESL_DataMgr *pDM_SPECIES, 
										INT nRowIdx,
										CESL_DataMgr *pDM_PARENT,
										CESL_DataMgr *pDM_PARENT_BOOK,
										CESL_Grid *pGRID_PARENT,
										CGroupInfo *pGroup,
										INT nAcqType, 
										BOOL bMultiVol
										)
{
	// MODE is _T("")  : purchaseinfo and donate is none
	// MODE is _T("DONATE") : purchase info is none 
	// MODE is _T("PURCHASE") : donate is none
	// ���� ���� ��� (1��-1��-1å)
	// IDX-SPECIES-BOOK �� �ִ´�.
	// �����ϰ�� ��������, ������ ��� ���������� �����Ͽ� �ִ´�

	ACQ_TYPE nACQ_TYPE = (ACQ_TYPE)nAcqType;
	
	CArray <CString, CString> RemoveAliasList;
	CESL_DataMgr *pDM_BOOK = m_pDM_BOOK;
	CESL_DataMgr *pDM_ACCU = NULL;
	INT ids;
	CString strSpecies_key, strAcqkey=_T("-1"), strBook_key;
	CString ACQUISIT_CODE, INPUT_DATE, strData, strGroupNo, strTmpData;
	INPUT_DATE = GetTodayDate();
	CMarc *pMarc;
	CString strStreamMarc;

	pDM_SPECIES->StartFrame();
	
	ids = pDM_SPECIES->MakeRecKey(strSpecies_key); 
	if (0>ids) return ids;
	
	m_strSPECIES_KEY = strSpecies_key;

	// BOOK �� ������ ROW�� �����Ѵ�.
	pDM_BOOK->FreeData();
	pDM_BOOK->InsertRow(-1);

	// �������� ���� ���� ���� ��
	if(nACQ_TYPE == PURCHASE && pGroup != NULL)
		pDM_SPECIES->SetCellData(_T("BS_��������"), pGroup->GetWorkCode(), nRowIdx);

	pDM_SPECIES->SetCellData(_T("BS_��KEY"), strSpecies_key, nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_���Գ⵵"), GetTodayYear(), nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_�Է���"), GetTodayDate(), nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_�۾����±���"),_T("1"), nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_�۾���"), pMgr->m_pInfo->USER_ID, nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_�����۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);

	// ��������DM����
	if(nACQ_TYPE == PURCHASE) this->SetAccuDM(TRUE);
	else if(nACQ_TYPE == DONATE) this->SetAccuDM(FALSE);
	pDM_ACCU = m_pDM_ACCU;

	//��ũ ����
	pMarc = new CMarc;

	pDM_SPECIES->GetCellData(_T("BS_MARC"), nRowIdx, strStreamMarc);

	pMgr->m_pInfo->pMarcMgr->Decoding(strStreamMarc, pMarc);		

	//��ũ�� ��Tag�� �����.
	pMgr->m_pInfo->pMarcMgr->DeleteEmptySubfield(pMarc);	
	pMgr->m_pInfo->pMarcMgr->DeleteEmptyField(pMarc);
	// ��ũ�� ����/å�� �����Ѵ�.
	ids = APPLY_CMarc_to_Species(pMarc, pDM_IDX_BO, pDM_SPECIES, pDM_BOOK, pDM_ACCU, nRowIdx);

	if(ids < 0) return ids;
	// �˻� �ʵ带 ���ο� �ϼ��Ѵ�.
	ids = APPLY_IndexItem(pDM_IDX_BO, pMarc, nRowIdx);
	if(ids < 0) return ids;

	//������ ���а��� å�� �����Ѵ�.
	INDEX_to_BOOK(pDM_IDX_BO, nRowIdx, pDM_BOOK, 0);
	pDM_BOOK->SetCellData(_T("BB_DB_FLAG"), _T("I"), 0);
	pDM_BOOK->SetCellData(_T("BB_�ڷ����"), IMPORTSTATUS, 0);
	pDM_BOOK->SetCellData(_T("BB_��ǰ����"), _T("Y"), 0);
	pDM_BOOK->SetCellData(_T("BB_���Ϸù�ȣ"), _T("10"), 0);
	if(IMPORTSTATUS != _T("BOT112O")) 
	{
		pDM_BOOK->SetCellData(_T("BB_�˼���"), INPUT_DATE, 0);
		pDM_BOOK->SetCellData(_T("BB_����ΰ�����"), INPUT_DATE, 0);
	}
	
	if (nACQ_TYPE == DONATE)				ACQUISIT_CODE = _T("2");
	else if (nACQ_TYPE == PURCHASE)			ACQUISIT_CODE = _T("1");
	else if (nACQ_TYPE == ACQ_TYPE_INVALID)	ACQUISIT_CODE = _T("");
	else if (nACQ_TYPE == OTHER_ACQ_TYPE)
	{
		if(pGroup == NULL) return -1;
		ACQUISIT_CODE = pGroup->GetGroupNo();
	}

	pDM_BOOK->SetCellData(_T("BB_���Ա���_�ڵ�"), ACQUISIT_CODE, 0);
	pDM_BOOK->SetCellData(_T("BB_�Է���"), INPUT_DATE, 0);
	if(pGroup != NULL)
	{
		if(nACQ_TYPE == ACQ_TYPE_INVALID) strGroupNo = pGroup->GetGroupNo();
		else strGroupNo = pGroup->GetGroupInfo();
		if(strGroupNo == _T("�������")) strGroupNo = _T("-1");
		pDM_BOOK->SetCellData(_T("BB_�׷��ȣ"), strGroupNo, 0);
		pDM_BOOK->SetCellData(_T("BB_���Գ⵵"), pGroup->GetAcqYear(), 0);
	}
	else
	{
		pDM_BOOK->SetCellData(_T("BB_�׷��ȣ"), _T("0"), 0);
		pDM_BOOK->SetCellData(_T("BB_���Գ⵵"), GetTodayYear(), 0);
	}

	// BOOK �� ����Ÿ�� �����Ѵ�.
	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_������"), nRowIdx, strData);
	pDM_BOOK->SetCellData(_T("BB_������"), strData, 0);
	
	pDM_BOOK->SetCellData(_T("BB_å�η��÷���"), _T("B"), 0);
	
	strTmpData = IMPORTSTATUS.Left(3);

	ids = Apply_MultiVolumeInfo(pMarc, IMPORTSTATUS.Left(3), pDM_BOOK, pDM_SPECIES, pDM_IDX_BO, pDM_SPECIES);
	if(ids < 0) return ids;

	if(!bMultiVol)
	{
		INT nCnt = pDM_BOOK->GetRowCount();
		for(INT i=nCnt-1 ; i>=1 ; i--)
		{
			pDM_BOOK->DeleteRow(i);			
		}
		pDM_BOOK->SetCellData(_T("BB_��ǥå����"), _T("Y"), 0);
		INDEX_to_BOOK(pDM_IDX_BO, nRowIdx, pDM_BOOK, i);
		pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("505"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("740"));		
	}
	else
	{
		pDM_BOOK->SetCellData(_T("BB_��ǥå����"), _T("Y"), 0);
		INT nCnt = pDM_BOOK->GetRowCount();
		for(INT i=0 ; i<nCnt ; i++)
		{
			INDEX_to_BOOK(pDM_IDX_BO, nRowIdx, pDM_BOOK, i);
		}
		ApplyMultiVolumePrice(pMarc, pDM_BOOK);
	}

	if(strTmpData == _T("BOT") && nAcqType == ACQ_TYPE_INVALID)
	{
		SetBookSerialNo(pDM_BOOK, pGroup);
		if(pGroup) pGroup->UpDateBookLastSerialNo(_T("0"), pMgr, pDM_SPECIES);
	}
	else if(strTmpData == _T("BOR") && nAcqType == PURCHASE)
	{
		SetBookSerialNo(pDM_BOOK, pGroup);
	}
	else if(strTmpData == _T("BOR") && nAcqType == DONATE)
	{
		SetBookSerialNo(pDM_BOOK, pGroup);
		if(pGroup) pGroup->UpDateBookLastSerialNo(_T("2"), pMgr, pDM_SPECIES);
	}
	else if(strTmpData == _T("BOR") && nAcqType == ACQ_TYPE_INVALID)
		SetBookSerialNo(pDM_BOOK, pGroup);
	else if(strTmpData == _T("BOR") && nAcqType == OTHER_ACQ_TYPE)
	{
		SetBookSerialNo(pDM_BOOK, pGroup);
		if(pGroup && pGroup->GetGroupInfo() != _T("�������")) 
			pGroup->UpDateBookLastSerialNo(pGroup->GetGroupNo(), pMgr, pDM_SPECIES);
	}

	// ���� �ϰ��(����/���)�� 001, 005, 049, 090�±׸� �����.
	if (strTmpData != _T("BOC"))
	{
		pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("001"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("005"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("049"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("090"));
		pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
		pDM_SPECIES->SetCellData(_T("BS_MARC"), strStreamMarc, nRowIdx);
	}

	CString strPriceInfo, strPrice, strCurrencyCode;
	
	if (pMarc) {
		delete pMarc;
		pMarc = NULL;
	}
	
	// ���� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	//RemoveAliasList.Add(_T("BS_MARC"));
	pDM_SPECIES->InitDBFieldData();
	ids = pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("BO_SPECIES_TBL"), nRowIdx, pDM_SPECIES);
	if(ids < 0) return ids;

	// ���ο� Insert�� Insert���� �����Ѵ�.
	pDM_IDX_BO->SetCellData(_T("IBS_REC_KEY"), strSpecies_key, nRowIdx);
	pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), _T("0"), nRowIdx);
	pDM_IDX_BO->SetCellData(_T("IBS_�۾�����INFO"), IMPORTSTATUS, nRowIdx);
	pDM_IDX_BO->SetCellData(_T("IBS_�����۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	pDM_IDX_BO->SetCellData(_T("IBS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	//������ ST_CODE�� ������
	Set_Index_ST_CODE(_T("IBS_ISBN"), pDM_IDX_BO, pDM_BOOK, -1, nRowIdx);
	
	RemoveAliasList.RemoveAll();
	
	if (nACQ_TYPE == DONATE) 
	{
		ids = MakeAccuInfo(pDM_SPECIES, pDM_ACCU, pDM_BOOK, pDM_SPECIES, strSpecies_key, pGroup, FALSE, _T("3"));
		if(ids < 0) return ids;
		strAcqkey = m_pDM_ACCU->GetCellData(_T("BP_��������KEY"), 0);
	} 
	if (nACQ_TYPE == PURCHASE) 
	{
		ids = MakeAccuInfo(pDM_SPECIES, pDM_ACCU, pDM_BOOK, pDM_SPECIES, strSpecies_key, pGroup, TRUE, _T("3"));
		if(ids < 0) return ids;
		strAcqkey = m_pDM_ACCU->GetCellData(_T("BP_��������KEY"), 0);
	}
	//������ȣ ����
	if(MODE == _T("EXCEL"))
	{
		for(INT i=1 ; i<pDM_BOOK->GetRowCount() ; i++)
		{
			strTmpData.Format(_T("%d"), i+1);
			pDM_BOOK->SetCellData(_T("BB_������ȣ"), strTmpData, i);
		}
	}
	
	//å �Է��ϱ� ���� Default���� ������Ѿ� �ڴ�.
	SetDefaultValue_BOOK(pDM_BOOK, pDM_ACCU, 0);
	ids = MakeBookQueryFrame(pDM_BOOK, strSpecies_key, strAcqkey, pDM_SPECIES);	
	if(ids < 0) return ids;
	
	//�������� Insert�� ����
	ids = SetBookIndexInfo(pDM_IDX_BO, pDM_BOOK, nRowIdx);
	if(ids < 0) return -1;
	
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	ids = pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX_BO, _T("IDX_BO_TBL"), nRowIdx, pDM_SPECIES);
	if(ids < 0) return ids;
	
	//�˻����� ����ȭ
	ids = BO_INDEX_TEMP_INSERT(pDM_SPECIES, _T("IDX_TMP_BO_TBL"), strSpecies_key, _T("I"));
		
	//ECOSEARCH�� ���ؼ� �������
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		CString strQuery;
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpecies_key);
		pDM_SPECIES->AddFrame(strQuery);
	}
	
	ids = pDM_SPECIES->SendFrame();
	if(ids < 0) return -33051;
	ids = pDM_SPECIES->EndFrame();
	if(ids < 0) return -33051;

	strTmpData = IMPORTSTATUS.Left(3);
	if(strTmpData == _T("BOT"))
	{
		if(pDM_PARENT)
			APPLY_PARENT_DM(
				pDM_IDX_BO, 
				pDM_SPECIES,
				pDM_ACCU,
				pDM_BOOK,
				pDM_PARENT, 
				pGRID_PARENT,
				-1, 
				nRowIdx,
				SPECIES, 
				TRUE,
				pGroup);
		if(pDM_PARENT_BOOK)
			APPLY_PARENT_DM(
				pDM_IDX_BO,
				pDM_SPECIES,
				pDM_ACCU,
				pDM_BOOK,
				pDM_PARENT_BOOK,
				pGRID_PARENT,
				-1, nRowIdx,
				BOOK, 
				FALSE,
				pGroup);
	}
	else 
	{
		if(pDM_PARENT)
			APPLY_PARENT_DM(
				pDM_IDX_BO,
				pDM_SPECIES, 
				pDM_ACCU, 
				pDM_BOOK, 
				pDM_PARENT,
				pGRID_PARENT,
				-1,
				nRowIdx,
				BOOK, 
				TRUE, 
				pGroup);
	}

	if (ids < 0) return -33053;		// Marc Stream Update Error

	return 0;
}
*/

//test
// Error Code : -37000 ~
INT CBO_SPECIES::BO_SPECIES_FILE_IMPORT(
										CString MODE, 
										CString IMPORTSTATUS, 
										CESL_DataMgr *pDM_IDX_BO, 
										CESL_DataMgr *pDM_SPECIES, 
										INT nRowIdx,
										CESL_DataMgr *pDM_PARENT,
										CESL_DataMgr *pDM_PARENT_BOOK,
										CESL_Grid *pGRID_PARENT,
										CGroupInfo *pGroup,
										INT nAcqType, 
										BOOL bMultiVol
										)
{
	// MODE is _T("")  : purchaseinfo and donate is none
	// MODE is _T("DONATE") : purchase info is none 
	// MODE is _T("PURCHASE") : donate is none
	// ���� ���� ��� (1��-1��-1å)
	// IDX-SPECIES-BOOK �� �ִ´�.
	// �����ϰ�� ��������, ������ ��� ���������� �����Ͽ� �ִ´�

	ACQ_TYPE nACQ_TYPE = (ACQ_TYPE)nAcqType;
	
	CArray <CString, CString> RemoveAliasList;
	CESL_DataMgr *pDM_BOOK = m_pDM_BOOK;
	CESL_DataMgr *pDM_ACCU = NULL;
	INT ids;
	CString strSpecies_key, strAcqkey=_T("-1"), strBook_key;
	CString ACQUISIT_CODE, INPUT_DATE, strData, strGroupNo, strTmpData;
	INPUT_DATE = GetTodayDate();
	//CMarc *pMarc;
	CString strStreamMarc;

	pDM_SPECIES->StartFrame();
	
	ids = pDM_SPECIES->MakeRecKey(strSpecies_key); 
	if (0>ids) return ids;
	
	m_strSPECIES_KEY = strSpecies_key;

	// BOOK �� ������ ROW�� �����Ѵ�.
	pDM_BOOK->FreeData();
	pDM_BOOK->InsertRow(-1);

	// �������� ���� ���� ���� ��
	if(nACQ_TYPE == PURCHASE && pGroup != NULL)
		pDM_SPECIES->SetCellData(_T("BS_��������"), pGroup->GetWorkCode(), nRowIdx);

	pDM_SPECIES->SetCellData(_T("BS_��KEY"), strSpecies_key, nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_���Գ⵵"), GetTodayYear(), nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_�Է���"), GetTodayDate(), nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_�۾����±���"),_T("1"), nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_�۾���"), pMgr->m_pInfo->USER_ID, nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_�����۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	// ��������DM����
	if(nACQ_TYPE == PURCHASE) this->SetAccuDM(TRUE);
	else if(nACQ_TYPE == DONATE) this->SetAccuDM(FALSE);
	pDM_ACCU = m_pDM_ACCU;

	//��ũ ����
	//pMarc = new CMarc;
	CMarc pMarc;
	pDM_SPECIES->GetCellData(_T("BS_MARC"), nRowIdx, strStreamMarc);
	pMgr->m_pInfo->pMarcMgr->Decoding(strStreamMarc, &pMarc);		
	//��ũ�� ��Tag�� �����.
	pMgr->m_pInfo->pMarcMgr->DeleteEmptySubfield(&pMarc);	pMgr->m_pInfo->pMarcMgr->DeleteEmptyField(&pMarc);
	// ��ũ�� ����/å�� �����Ѵ�.
	ids = APPLY_CMarc_to_Species(&pMarc, pDM_IDX_BO, pDM_SPECIES, pDM_BOOK, pDM_ACCU, nRowIdx);
	if(ids < 0) return ids;
	// �˻� �ʵ带 ���ο� �ϼ��Ѵ�.
	ids = APPLY_IndexItem(pDM_IDX_BO, &pMarc, nRowIdx);
	if(ids < 0) return ids;

	//������ ���а��� å�� �����Ѵ�.
	INDEX_to_BOOK(pDM_IDX_BO, nRowIdx, pDM_BOOK, 0);
	pDM_BOOK->SetCellData(_T("BB_DB_FLAG"), _T("I"), 0);
	pDM_BOOK->SetCellData(_T("BB_�ڷ����"), IMPORTSTATUS, 0);
	pDM_BOOK->SetCellData(_T("BB_��ǰ����"), _T("Y"), 0);
	pDM_BOOK->SetCellData(_T("BB_���Ϸù�ȣ"), _T("10"), 0);
	if(IMPORTSTATUS != _T("BOT112O")) 
	{
		pDM_BOOK->SetCellData(_T("BB_�˼���"), INPUT_DATE, 0);
		pDM_BOOK->SetCellData(_T("BB_����ΰ�����"), INPUT_DATE, 0);
	}
	
	if (nACQ_TYPE == DONATE)				ACQUISIT_CODE = _T("2");
	else if (nACQ_TYPE == PURCHASE)			ACQUISIT_CODE = _T("1");
	else if (nACQ_TYPE == ACQ_TYPE_INVALID)	ACQUISIT_CODE = _T("");
	else if (nACQ_TYPE == OTHER_ACQ_TYPE)
	{
		if(pGroup == NULL) return -1;
		ACQUISIT_CODE = pGroup->GetGroupNo();
	}

	pDM_BOOK->SetCellData(_T("BB_���Ա���_�ڵ�"), ACQUISIT_CODE, 0);
	pDM_BOOK->SetCellData(_T("BB_�Է���"), INPUT_DATE, 0);
	if(pGroup != NULL)
	{
		if(nACQ_TYPE == ACQ_TYPE_INVALID) strGroupNo = pGroup->GetGroupNo();
		else strGroupNo = pGroup->GetGroupInfo();
		if(strGroupNo == _T("�������")) strGroupNo = _T("-1");
		pDM_BOOK->SetCellData(_T("BB_�׷��ȣ"), strGroupNo, 0);
		pDM_BOOK->SetCellData(_T("BB_���Գ⵵"), pGroup->GetAcqYear(), 0);
	}
	else
	{
		pDM_BOOK->SetCellData(_T("BB_�׷��ȣ"), _T("0"), 0);
		pDM_BOOK->SetCellData(_T("BB_���Գ⵵"), GetTodayYear(), 0);
	}

	// BOOK �� ����Ÿ�� �����Ѵ�.
	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_������"), nRowIdx, strData);
	pDM_BOOK->SetCellData(_T("BB_������"), strData, 0);
	
	pDM_BOOK->SetCellData(_T("BB_å�η��÷���"), _T("B"), 0);
	
	strTmpData = IMPORTSTATUS.Left(3);

	ids = Apply_MultiVolumeInfo(&pMarc, IMPORTSTATUS.Left(3), pDM_BOOK, pDM_SPECIES, pDM_IDX_BO, pDM_SPECIES);
	if(ids < 0) return ids;

	if(!bMultiVol)
	{
		INT nCnt = pDM_BOOK->GetRowCount();
		for(INT i=nCnt-1 ; i>=1 ; i--)
		{
			pDM_BOOK->DeleteRow(i);			
		}
		pDM_BOOK->SetCellData(_T("BB_��ǥå����"), _T("Y"), 0);
		INDEX_to_BOOK(pDM_IDX_BO, nRowIdx, pDM_BOOK, i);
		pMgr->m_pInfo->pMarcMgr->DeleteField(&pMarc, _T("505"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(&pMarc, _T("740"));		
	}
	else
	{
		pDM_BOOK->SetCellData(_T("BB_��ǥå����"), _T("Y"), 0);
		INT nCnt = pDM_BOOK->GetRowCount();
		for(INT i=0 ; i<nCnt ; i++)
		{
			INDEX_to_BOOK(pDM_IDX_BO, nRowIdx, pDM_BOOK, i);
		}
		ApplyMultiVolumePrice(&pMarc, pDM_BOOK);
	}

	if(strTmpData == _T("BOT") && nAcqType == ACQ_TYPE_INVALID)
	{
		SetBookSerialNo(pDM_BOOK, pGroup);
		if(pGroup) pGroup->UpDateBookLastSerialNo(_T("0"), pMgr, pDM_SPECIES);
	}
	else if(strTmpData == _T("BOR") && nAcqType == PURCHASE)
	{
		SetBookSerialNo(pDM_BOOK, pGroup);
	}
	else if(strTmpData == _T("BOR") && nAcqType == DONATE)
	{
		SetBookSerialNo(pDM_BOOK, pGroup);
		if(pGroup) pGroup->UpDateBookLastSerialNo(_T("2"), pMgr, pDM_SPECIES);
	}
	else if(strTmpData == _T("BOR") && nAcqType == ACQ_TYPE_INVALID)
		SetBookSerialNo(pDM_BOOK, pGroup);
	else if(strTmpData == _T("BOR") && nAcqType == OTHER_ACQ_TYPE)
	{
		SetBookSerialNo(pDM_BOOK, pGroup);
		if(pGroup && pGroup->GetGroupInfo() != _T("�������")) 
			pGroup->UpDateBookLastSerialNo(pGroup->GetGroupNo(), pMgr, pDM_SPECIES);
	}
	// ���� �ϰ��(����/���)�� 001, 005, 049, 090�±׸� �����.
	if (strTmpData != _T("BOC"))
	{
		pMgr->m_pInfo->pMarcMgr->DeleteField(&pMarc, _T("001"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(&pMarc, _T("005"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(&pMarc, _T("049"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(&pMarc, _T("090"));
		pMgr->m_pInfo->pMarcMgr->Encoding(&pMarc, strStreamMarc);
		pDM_SPECIES->SetCellData(_T("BS_MARC"), strStreamMarc, nRowIdx);
	}

	CString strPriceInfo, strPrice, strCurrencyCode;
	
	/*
	if (pMarc) {
		delete pMarc;
		pMarc = NULL;
	}
	*/
	
	// ���� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	//RemoveAliasList.Add(_T("BS_MARC"));
	pDM_SPECIES->InitDBFieldData();
	ids = pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("BO_SPECIES_TBL"), nRowIdx, pDM_SPECIES);
	if(ids < 0) return ids;

	// ���ο� Insert�� Insert���� �����Ѵ�.
	pDM_IDX_BO->SetCellData(_T("IBS_REC_KEY"), strSpecies_key, nRowIdx);
	pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), _T("0"), nRowIdx);
	pDM_IDX_BO->SetCellData(_T("IBS_�۾�����INFO"), IMPORTSTATUS, nRowIdx);
	pDM_IDX_BO->SetCellData(_T("IBS_�����۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	pDM_IDX_BO->SetCellData(_T("IBS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	//������ ST_CODE�� ������
	Set_Index_ST_CODE(_T("IBS_ISBN"), pDM_IDX_BO, pDM_BOOK, -1, nRowIdx);
	
	RemoveAliasList.RemoveAll();
	
	if (nACQ_TYPE == DONATE) 
	{
		ids = MakeAccuInfo(pDM_SPECIES, pDM_ACCU, pDM_BOOK, pDM_SPECIES, strSpecies_key, pGroup, FALSE, _T("3"));
		if(ids < 0) return ids;
		strAcqkey = m_pDM_ACCU->GetCellData(_T("BP_��������KEY"), 0);
	} 
	if (nACQ_TYPE == PURCHASE) 
	{
		ids = MakeAccuInfo(pDM_SPECIES, pDM_ACCU, pDM_BOOK, pDM_SPECIES, strSpecies_key, pGroup, TRUE, _T("3"));
		if(ids < 0) return ids;
		strAcqkey = m_pDM_ACCU->GetCellData(_T("BP_��������KEY"), 0);
	}
	//������ȣ ����
	if(MODE == _T("EXCEL"))
	{
		for(INT i=1 ; i<pDM_BOOK->GetRowCount() ; i++)
		{
			strTmpData.Format(_T("%d"), i+1);
			pDM_BOOK->SetCellData(_T("BB_������ȣ"), strTmpData, i);
		}
	}
	
	//å �Է��ϱ� ���� Default���� ������Ѿ� �ڴ�.
	SetDefaultValue_BOOK(pDM_BOOK, pDM_ACCU, 0);
	ids = MakeBookQueryFrame(pDM_BOOK, strSpecies_key, strAcqkey, pDM_SPECIES);	
	if(ids < 0) return ids;
	
	//�������� Insert�� ����
	ids = SetBookIndexInfo(pDM_IDX_BO, pDM_BOOK, nRowIdx);
	if(ids < 0) return -1;
	
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	ids = pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX_BO, _T("IDX_BO_TBL"), nRowIdx, pDM_SPECIES);
	if(ids < 0) return ids;
	
	//�˻����� ����ȭ
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	ids = BO_INDEX_TEMP_INSERT(pDM_SPECIES, _T("IDX_TMP_BO_TBL"), strSpecies_key, _T("I"));
	//=====================================================
	
		
	//ECOSEARCH�� ���ؼ� �������
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		CString strQuery;
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpecies_key);
		pDM_SPECIES->AddFrame(strQuery);
	}
	
	ids = pDM_SPECIES->SendFrame();
	if(ids < 0) return -33051;
	ids = pDM_SPECIES->EndFrame();
	if(ids < 0) return -33051;

	strTmpData = IMPORTSTATUS.Left(3);
	if(strTmpData == _T("BOT"))
	{
		if(pDM_PARENT)
			APPLY_PARENT_DM(
				pDM_IDX_BO, 
				pDM_SPECIES,
				pDM_ACCU,
				pDM_BOOK,
				pDM_PARENT, 
				pGRID_PARENT,
				-1, 
				nRowIdx,
				SPECIES, 
				TRUE,
				pGroup);
		if(pDM_PARENT_BOOK)
			APPLY_PARENT_DM(
				pDM_IDX_BO,
				pDM_SPECIES,
				pDM_ACCU,
				pDM_BOOK,
				pDM_PARENT_BOOK,
				pGRID_PARENT,
				-1, nRowIdx,
				BOOK, 
				FALSE,
				pGroup);
	}
	else 
	{
		if(pDM_PARENT)
			APPLY_PARENT_DM(
				pDM_IDX_BO,
				pDM_SPECIES, 
				pDM_ACCU, 
				pDM_BOOK, 
				pDM_PARENT,
				pGRID_PARENT,
				-1,
				nRowIdx,
				BOOK, 
				TRUE, 
				pGroup);
	}

	if (ids < 0) return -33053;		// Marc Stream Update Error

	return 0;
}

INT CBO_SPECIES::Apply_MARC_950(CString strSource, CString &strOutput, CString &strTmp)
{
	INT nSize = strSource.GetLength();
	TCHAR ch;
	TCHAR szBufDigit[1024];
	INT nBufDigitSize = 0;
	TCHAR szBufNotDigit[1024];
	INT nBufNotDigitSize = 0;
	for (INT i = 0; i < nSize; i++) {
		ch = strSource.GetAt(i);
		if (isdigit(ch)) 
		{
			if ((INT)ch > 0) szBufDigit[nBufDigitSize++] = ch;
		}
		else if(ch == '.')
			szBufDigit[nBufDigitSize++]  = ch;
		else 
		{
			if ((INT)ch > 0) szBufNotDigit[nBufNotDigitSize++] = ch;
		}

	}
	szBufDigit[nBufDigitSize] = '\0';
	strOutput.Format(_T("%s"), szBufDigit);
	strOutput.TrimLeft();		strOutput.TrimRight();
	CString strTmpData = strOutput.Left(strOutput.Find(_T(".")));
	if(strTmpData.GetLength() > 8) strTmpData = strOutput.Left(8);
	strOutput = strTmpData + strOutput.Mid(strOutput.Find(_T(".")));

	szBufNotDigit[nBufNotDigitSize] = '\0';
	strTmp.Format(_T("%s"), szBufNotDigit);
	strTmp.TrimLeft();			strTmp.TrimRight();
	strTmp = strTmp.Left(3);
	return 0;
}

INT CBO_SPECIES::MakeBookInsertQueryFrame(CESL_DataMgr *pDM_BOOK, CString strSpecies_key, CString strPurchaseinfo_key, CESL_DataMgr *pDM_OUT, INT nRowIdx)
{
	CString strQuery;

	CArray <CString, CString> RemoveAliasList;

	// FLAG�� 'I' �ΰ͵��� REC_KEY �ʵ忡 �� REC_KEY�� ���ؼ� �־��ش�.
	CString strBook_key;
	pDM_OUT->MakeRecKey(strBook_key); 
	pDM_BOOK->SetCellData(_T("BB_åKEY"), strBook_key, nRowIdx);

	pDM_BOOK->SetCellData(_T("BB_�θ�åKEY"), strBook_key, nRowIdx);
	pDM_BOOK->SetCellData(_T("BB_åKEY"), strBook_key, nRowIdx);
	pDM_BOOK->SetCellData(_T("BB_��������KEY"), strPurchaseinfo_key, nRowIdx);
	pDM_BOOK->SetCellData(_T("BB_��KEY"), strSpecies_key, nRowIdx);
	if (pDM_BOOK->GetCellData(_T("BB_�̳�å��"), nRowIdx).GetLength() == 0) pDM_BOOK->SetCellData(_T("BB_�̳�å��"), _T("0"), nRowIdx);
	if (pDM_BOOK->GetCellData(_T("BB_����"), nRowIdx).GetLength() == 0) pDM_BOOK->SetCellData(_T("BB_����"), _T("0"), nRowIdx);
	pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_BOOK, _T("BO_BOOK_TBL"), nRowIdx, pDM_OUT);
	FileLog(pDM_OUT->m_lastmsg);

	return 0;
}

INT CBO_SPECIES::APPLY_ACQ_VIEW_DM_Data(CESL_DataMgr *pDM_IDX_BO, CESL_DataMgr *pDM_PURCHASE, INT nRowIdx, CESL_DataMgr *pDM_TARGET, INT nTargetRowIdx)
{
	INT nMatchingInfo = 0;
	CString strMatchingInfo[MAX_TEMP_BIG_STRING_ARRAY_SIZE];
	strMatchingInfo[nMatchingInfo++] = _T("IBS_������");
	strMatchingInfo[nMatchingInfo++] = _T("IBS_���ı���");
	strMatchingInfo[nMatchingInfo++] = _T("IBS_������");
	strMatchingInfo[nMatchingInfo++] = _T("IBS_�����");
	strMatchingInfo[nMatchingInfo++] = _T("IBS_��ǥ��");
	strMatchingInfo[nMatchingInfo++] = _T("IBS_�̿��󱸺�");
	strMatchingInfo[nMatchingInfo++] = _T("BP_�˼��Ϸ�");
	strMatchingInfo[nMatchingInfo++] = _T("BP_ȭ�󱸺�");
	strMatchingInfo[nMatchingInfo++] = _T("BP_��ǰå��");
	strMatchingInfo[nMatchingInfo++] = _T("BP_ȯ��");
	strMatchingInfo[nMatchingInfo++] = _T("BP_�Է���");
	strMatchingInfo[nMatchingInfo++] = _T("BP_�Է±���");
	strMatchingInfo[nMatchingInfo++] = _T("BP_�̳�å��");
	strMatchingInfo[nMatchingInfo++] = _T("BP_����å��");
	strMatchingInfo[nMatchingInfo++] = _T("BP_������");
	strMatchingInfo[nMatchingInfo++] = _T("BP_���ְ���");
	strMatchingInfo[nMatchingInfo++] = _T("BP_����");
	strMatchingInfo[nMatchingInfo++] = _T("BP_������");
	strMatchingInfo[nMatchingInfo++] = _T("BP_����ó��ȣ");
	strMatchingInfo[nMatchingInfo++] = _T("BP_��������KEY");
	strMatchingInfo[nMatchingInfo++] = _T("BP_�ѱǼ�");

	INT ids;
	CString strHeader, strData;
	for (INT i = 0; i < nMatchingInfo; i++) {
		strData = _T("");
		strHeader = strMatchingInfo[i].Left(2);
		if (strHeader == _T("IB")) ids = pDM_IDX_BO->GetCellData(strMatchingInfo[i], nRowIdx, strData);
		if (strHeader == _T("BP")) ids = pDM_PURCHASE->GetCellData(strMatchingInfo[i], nRowIdx, strData);
		if (ids >= 0) pDM_TARGET->SetCellData(strMatchingInfo[i], strData, nTargetRowIdx);
	}

	strData = _T("");
	ids = pDM_PURCHASE->GetCellData(_T("BP_�Է�����"), nRowIdx, strData);
	pDM_TARGET->SetCellData(_T("BP_�Է���"), strData, nTargetRowIdx);
	CString strWORKING_STATUS;
	ids = pDM_IDX_BO->GetCellData(_T("IBS_�۾�����INFO"), nRowIdx, strWORKING_STATUS);
	if (strWORKING_STATUS == _T("BOA113N")) pDM_TARGET->SetCellData(_T("BP_������"), GetTodayDate(), nTargetRowIdx);
	if (strWORKING_STATUS == _T("BOA141N")) pDM_TARGET->SetCellData(_T("BP_�˼���"), GetTodayDate(), nTargetRowIdx);

	// REC_KEY�� (��KEY)�� ����ȭ��� ��ȭ���� �ٸ��� ���� ó��
	ids = pDM_IDX_BO->GetCellData(_T("IBS_REC_KEY"), nRowIdx, strData);
	if (ids >= 0) pDM_TARGET->SetCellData(_T("IBS_��KEY"), strData, nTargetRowIdx);
	
	CString strDataCode, strDataDesc;
	// UDF_�ڷ���¼���
	strDataCode = _T("");
	strDataDesc = _T("");
	ids = pDM_IDX_BO->GetCellData(_T("IBS_�۾�����INFO"), nRowIdx, strDataCode);
	strDataDesc = ConvertWORKING_STATUS_CodeToDesc(strDataCode);
	pDM_TARGET->SetCellData(_T("UDF_�ڷ���¼���"), strDataDesc, nTargetRowIdx);

	// UDF_�Է±��м���
	strDataCode = _T("");
	strDataDesc = _T("");
	ids = pDM_PURCHASE->GetCellData(_T("BP_�Է±���"), nRowIdx, strDataCode);
	if (strDataCode == _T("0")) strDataDesc = _T("ȭ���Է�");
	if (strDataCode == _T("1")) strDataDesc = _T("��Ȱ�����");
	if (strDataCode == _T("2")) strDataDesc = _T("��ġ�������");
	if (strDataCode == _T("3")) strDataDesc = _T("���Ϲ���");
	pDM_TARGET->SetCellData(_T("UDF_�Է±��м���"), strDataDesc, nTargetRowIdx);

	// UDF_���ı��м���
	strDataCode = _T("");
	strDataDesc = _T("");
	ids = pDM_IDX_BO->GetCellData(_T("IBS_���ı���_�ڵ�"), nRowIdx, strDataCode);
	pMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("���ı���"), strDataCode, strDataDesc);
	pDM_TARGET->SetCellData(_T("UDF_���ı��м���"), strDataDesc, nTargetRowIdx);

	// UDF_�̿��󱸺м���
	strDataCode = _T("");
	strDataDesc = _T("");
	ids = pDM_IDX_BO->GetCellData(_T("IBS_�̿��󱸺�_�ڵ�"), nRowIdx, strDataCode);
	pMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�̿��󱸺�"), strDataCode, strDataDesc);
	pDM_TARGET->SetCellData(_T("UDF_�̿��󱸺м���"), strDataDesc, nTargetRowIdx);

	// UDF_����ó��ȣ����
	strDataCode = _T("");
	strDataDesc = _T("");
	ids = pDM_PURCHASE->GetCellData(_T("BP_����ó��ȣ"), nRowIdx, strDataCode);
	pMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����ó��ȣ"), strDataCode, strDataDesc);
	pDM_TARGET->SetCellData(_T("UDF_����ó��ȣ����"), strDataDesc, nTargetRowIdx);

	return 0;
}

CString CBO_SPECIES::ConvertWORKING_STATUS_CodeToDesc(CString strCode)
{
	CString result = _T("");
	
	if (strCode == _T("BOA111N")) result = _T("���Դ���ڷ�");
	if (strCode == _T("BOA113N")) result = _T("�˼�����ڷ�");
	if (strCode == _T("BOA131N")) result = _T("��������ڷ�");
	if (strCode == _T("BOC113N")) result = _T("������ڷ�");
    if (strCode == _T("BOA141N")) result = _T("�˼��Ϸ��ڷ�");

	return result;
}

INT CBO_SPECIES::DeleteUnlessWord(CString &strData)
{
	CESL_CharSetMgr CharSetMgr_UnlessWord;
	CharSetMgr_UnlessWord.InitData(_T("���Ҿ�"), _T("���Ҿ�"));
	INT nFind = strData.ReverseFind(' ');
	if (nFind > 0)
	{
		CString str1 = strData.Mid(nFind);
		CString str2 = str1;
		str1.TrimLeft();
		str1.TrimRight();
		
		if (CharSetMgr_UnlessWord.Find(str1, str2) == 0)
		{
			strData.Delete(nFind, strData.GetLength() - nFind);
		}
	}
	return 0;
}

INT CBO_SPECIES::Apply_MARC_700Author(CMarc *pMarc)
{
	if (pMarc == NULL) return -1;
	pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("700"));

	CString strData, strTmpData;
	CArray<CString, CString&> StrArrayFrom;
	CArray<CString, CString&> StrArrayOrigin1;
	CArray<CString, CString&> StrArrayOrigin2;

	INT ids = -1;
	INT nCountFrom = -1;
	INT nCountOrigin1 = -1;
	INT nCountOrigin2 = -1;

	/**
		245$e -> 700$a
	*/
	StrArrayFrom.RemoveAll();
	StrArrayOrigin1.RemoveAll();
	StrArrayOrigin2.RemoveAll();
	
	ids = pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$e"), strData, &StrArrayFrom);	
	if (ids >= 0)
	{
		nCountFrom = StrArrayFrom.GetSize();
		for (INT idx = 0; idx < nCountFrom; idx++)
		{
			strData = _T("");
			strData = StrArrayFrom.GetAt(idx);

			// �����ϴ��� üũ�Ѵ�.
			BOOL bIsInArr = FALSE;

			StrArrayOrigin1.RemoveAll();
			strTmpData = _T("");
			DeleteUnlessWord(strData);
			pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("700$a"), strTmpData, &StrArrayOrigin1);
			nCountOrigin1 = StrArrayOrigin1.GetSize();

			for (INT j = 0; j < nCountOrigin1; j++)
			{
				if (strData == StrArrayOrigin1.GetAt(j))
				{
					bIsInArr = TRUE;
					break;
				}
			}

			if (bIsInArr) continue;			

			pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("700$a"), strData);
		}
	}

	/**
		245$d -> 700$a
	*/
	StrArrayFrom.RemoveAll();
	StrArrayOrigin1.RemoveAll();
	StrArrayOrigin2.RemoveAll();
	
	ids = pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strData, &StrArrayFrom);	
	if (ids >= 0)
	{
		nCountFrom = StrArrayFrom.GetSize();
		for (INT idx = 0; idx < nCountFrom; idx++)
		{
			strData = _T("");
			strData = StrArrayFrom.GetAt(idx);

			// �����ϴ��� üũ�Ѵ�.
			BOOL bIsInArr = FALSE;

			StrArrayOrigin1.RemoveAll();
			strTmpData = _T("");
			DeleteUnlessWord(strData);
			pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("700$a"), strTmpData, &StrArrayOrigin1);
			nCountOrigin1 = StrArrayOrigin1.GetSize();

			for (INT j = 0; j < nCountOrigin1; j++)
			{
				if (strData == StrArrayOrigin1.GetAt(j))
				{
					bIsInArr = TRUE;
					break;
				}
			}

			if (bIsInArr) continue;			

			pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("700$a"), strData);
		}
	}

	return 0;
}

INT CBO_SPECIES::APPLY_Species_to_CMarc(CMarc *pMarc, 
										CESL_DataMgr *pDM_IDX_BO, 
										CESL_DataMgr *pDM_SPECIES, 
										CESL_DataMgr *pDM_BOOK, 
										CESL_DataMgr *pDM_PURCHASE, 
										INT nIdxRowIdx/*=0*/)
{
	if (pDM_IDX_BO == NULL) return -1;
	if (pDM_SPECIES == NULL) return -2;
	if (pDM_BOOK == NULL) return -3;
	if (pDM_PURCHASE == NULL) return -4;
	if (pMgr == NULL) return -5;

	
	INT nREPRESENTBOOKRowIdx = Find_REPRESENT_BOOK_RowIdx(pDM_BOOK);
	if (nREPRESENTBOOKRowIdx < 0) nREPRESENTBOOKRowIdx = 0;

	CString strDM, strData1, strData2, strData3;

	CESL_DataMgr *pDM_VOLUME = new CESL_DataMgr();
	pDM_VOLUME->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
	pDM_VOLUME->InitDMPKFromFile(_T("DMUP_����_��_������"));
	pDM_VOLUME->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	CONVERT_BOOK_to_VOLUME(pDM_BOOK, pDM_VOLUME);
	
	//��ũ�޴��� �׷켳��
	strData1 = pDM_IDX_BO->GetCellData(_T("IBS_���ı���_�ڵ�"), nIdxRowIdx);
	pMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	
	// �Ϲ����� ����Ÿ ó��
	for (INT i = 0; i < nSpeciesMarcRelation; i++) {
		strDM = SpeciesMarcRelation_Species[i].Left(2);
		strData1.Empty();
		if (strDM == _T("IB")) pDM_IDX_BO->GetCellData(SpeciesMarcRelation_Species[i], nIdxRowIdx, strData1);
		if (strDM == _T("BB")) pDM_BOOK->GetCellData(SpeciesMarcRelation_Species[i], nREPRESENTBOOKRowIdx, strData1);
		if (strData1.GetLength() > 0) pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, SpeciesMarcRelation_Marc[i], strData1, _T(""), MODIFY_ITEM);
		else pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, SpeciesMarcRelation_Marc[i]);
	}
	//008����� �ʵ� ����
	pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("008"));
	strData1 = pDM_IDX_BO->GetCellData(_T("IBS_�����"), 0);
	Apply_MARC_950(strData1, strData2, strData3);
	if(strData2.GetLength() == 4)
	{	
		pMgr->m_pInfo->pMarcMgr->SetItem( pMarc, _T("008@6"), _T("s") );
		pMgr->m_pInfo->pMarcMgr->SetItem( pMarc, _T("008@7-10"), strData2);		
		pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("008@38-39"), _T("  ") );
	}
	
	//�Ѽ���/�Ѽ�����
	pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("440"));
	pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("490"));
	CString arrAlias[2] = {_T("IBS_�Ѽ���"), _T("IBS_�Ѽ�����")};
	CString arrData[2];
	pDM_IDX_BO->GetCellData(arrAlias, 2, arrData, nIdxRowIdx);
	if(!arrData[0].IsEmpty() && arrData[1].IsEmpty())
		pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("490$a"), arrData[0]);
	else if(arrData[0].IsEmpty() && !arrData[1].IsEmpty())
		pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("440$v"), arrData[1]);
	else if(!arrData[0].IsEmpty() && !arrData[1].IsEmpty())
	{
		CString str440;
		// 2004.09.07 '$'�� '��'�� �ٲ� �Է�.
		//str440.Format(_T("$a%s$v%s"), arrData[0], arrData[1]);
		str440.Format(_T("%ca%s%cv%s"), SUBFIELD_CODE, arrData[0], SUBFIELD_CODE, arrData[1]);  
		str440 = _T("44000") + str440;
		pMgr->m_pInfo->pMarcMgr->InsertField(pMarc, str440);
	}
	
	// ����
	CString strPrice, strPriceCode, strPriceOther, strPriceCharacter;
	DOUBLE dTmpPrice = 0;
	for(i=0 ; i<pDM_VOLUME->GetRowCount() ; i++)
	{
		dTmpPrice += (DOUBLE)atof(pDM_VOLUME->GetCellData(_T("BB_����"), i).GetBuffer(0));
	}
	strPrice.Format(_T("%.0f"), dTmpPrice);
	//pDM_PURCHASE->GetCellData(_T("BP_����"), nIdxRowIdx, strPrice);
	pDM_PURCHASE->GetCellData(_T("BP_ȭ�󱸺�"), nIdxRowIdx, strPriceCode);
	pDM_PURCHASE->GetCellData(_T("BP_����������"), nIdxRowIdx, strPriceOther);
	pDM_PURCHASE->GetCellData(_T("BP_���ݼ���"), nIdxRowIdx, strPriceCharacter);
	strData1.Format(_T("%s%s"), strPriceCode, strPrice);
	if (strData1.GetLength() > 0) pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("950$b"), strData1, _T(""), MODIFY_ITEM); 
	else pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("950$b")); 
	if (strPriceOther.GetLength() > 0) pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("950$c"), strPriceOther, _T(""), MODIFY_ITEM); 
	else pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("950$c")); 
	if (strPriceCharacter.GetLength() > 0) pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("950$a"), strPriceCharacter, _T(""), MODIFY_ITEM);
	else pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("950$a"));

	// ��ǥ��
	strData1 = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_��ǥ��"), nIdxRowIdx, strData1);	
	Set_TITLE(pMarc, strData1);
	// ������
	strData1 = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_������"), nIdxRowIdx, strData1);
	if (strData1.GetLength() == 0) pDM_BOOK->GetCellData(_T("BB_������"), nREPRESENTBOOKRowIdx, strData1);
	Set_AUTHOR(pMarc, strData1);

	// �����
	strData1 = pDM_IDX_BO->GetCellData(_T("IBS_�����"), nIdxRowIdx);
	pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$n"));
	if(!strData1.IsEmpty())
	{
		pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$n"), strData1);
	}
	
	// ����(�Ǽ���)
	if (pDM_VOLUME->GetRowCount() == 1) 
	{
		pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$p"));
		strData1 = _T("");
		pDM_BOOK->GetCellData(_T("BB_������ηϸ�"), nREPRESENTBOOKRowIdx, strData1);
		if (strData1.GetLength() > 0) 
		{
			pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$p"), strData1);
		}
	}
	
	//������
	strData1 = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_������"), nIdxRowIdx, strData1);
	Set_PUBLIHSER(pMarc, strData1);

	Apply_MARC_700Author(pMarc);

	SetISBN_to_Marc_ByVolume(pMarc, pDM_VOLUME, _T("020"), _T("  "));		// IBS_����_ISBN is 020 blank blank
	// Ư���� ����Ÿ ó�� - 020(��Ʈ)
	SetISBN_to_Marc(pMarc, pDM_IDX_BO, nIdxRowIdx, _T("020"), _T("1 "));		// IBS_��Ʈ_ISBN is 020 1 blank

	if (pDM_VOLUME) {
		delete pDM_VOLUME;
		pDM_VOLUME = NULL;
	}
	return 0;
}

INT CBO_SPECIES::APPLY_CMarc_to_Species(CMarc *pMarc, CESL_DataMgr *pDM_IDX_BO, CESL_DataMgr *pDM_SPECIES, CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_PURCHASE, INT nIdxRowIdx/*=0*/)
{
	if (pDM_IDX_BO == NULL) return -1;
	if (pDM_SPECIES == NULL) return -2;
	if (pDM_BOOK == NULL) return -3;
	if (pMgr == NULL) return -5;

	
	INT nREPRESENTBOOKRowIdx = Find_REPRESENT_BOOK_RowIdx(pDM_BOOK);
	if (nREPRESENTBOOKRowIdx < 0) nREPRESENTBOOKRowIdx = 0;

	CString strDM, strData1, strData2, strData3;

	CESL_DataMgr *pDM_VOLUME = new CESL_DataMgr();
	pDM_VOLUME->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
	pDM_VOLUME->InitDMPKFromFile(_T("DMUP_����_��_������"));
	pDM_VOLUME->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	CONVERT_BOOK_to_VOLUME(pDM_BOOK, pDM_VOLUME);
	INT nVolumes = pDM_VOLUME->GetRowCount();	
	// �Ϲ����� ����Ÿ ó��
	for (INT i = 0; i < nSpeciesMarcRelation; i++) 
	{
		strDM = SpeciesMarcRelation_Species[i].Left(2);
		strData1 = _T("");
		pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, SpeciesMarcRelation_Marc[i], strData1);
		if (strDM == _T("IB")) pDM_IDX_BO->SetCellData(SpeciesMarcRelation_Species[i], strData1, nIdxRowIdx);
		if (strDM == _T("BB")) pDM_BOOK->SetCellData(SpeciesMarcRelation_Species[i], strData1, nREPRESENTBOOKRowIdx);
	}

	//260$c�� �����Ͱ� ������ 008 7-10�� �����Ͱ� ������ �ش� �����͸� ���� ����⿡ �����Ų��.
	strData1 = pDM_IDX_BO->GetCellData(_T("IBS_�����"), 0);
	if(strData1.IsEmpty())
	{
		pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@7-10"), strData1);
		pDM_IDX_BO->SetCellData(_T("IBS_�����"), strData1, 0);
		pDM_BOOK->SetCellData(_T("BB_�����"), strData1, nREPRESENTBOOKRowIdx);
	}
	
	// �����(��ũ->�� ����ô� ���� 1���� ��츸 ��ǥå�� �������ش�.)
	if (nVolumes == 1) {
		strData1 = _T("");
		pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$n"), strData1);
		pDM_BOOK->SetCellData(_T("BB_�����"), strData1, nREPRESENTBOOKRowIdx);
	}

	strData1.Empty();
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$n"), strData1);
	pDM_IDX_BO->SetCellData(_T("IBS_�����"), strData1, 0);
	
	// ����(�Ǽ���)
	if (nVolumes == 1) {
		strData1 = _T("");
		pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$p"), strData1);
		if (strData1.GetLength() > 0) {
			pDM_BOOK->SetCellData(_T("BB_������ηϸ�"), strData1, nREPRESENTBOOKRowIdx);
		}
	}

	// ���� ����
	CString strPrice, strPriceCode;
	strData1 = _T("");
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strData1);
	if (strData1.GetLength() > 0) {
		strData2 = _T("");		strData3 = _T("");
		Apply_MARC_950(strData1, strData2, strData3);
		if(nVolumes == 1)
		{
			pDM_BOOK->SetCellData(_T("BB_����"), strData2, nREPRESENTBOOKRowIdx);
		}
		if(pDM_PURCHASE != NULL)
		{
			pDM_PURCHASE->SetCellData(_T("BP_����"), strData2, 0);
			pDM_PURCHASE->SetCellData(_T("BP_ȭ�󱸺�"), strData3, 0);
		}
	}	
	strData1.Empty();
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$a"), strData1);
	if(strData1.GetLength() > 0)
	{
		if(pDM_PURCHASE != NULL)
			pDM_PURCHASE->SetCellData(_T("BP_���ݼ���"), strData1, 0);
		if(nVolumes == 1)
			pDM_BOOK->SetCellData(_T("BB_���ݼ���"), strData1, 0);
	}
	strData1.Empty();
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$c"), strData1);
	if(strData1.GetLength() > 0)
	{
		
		if(pDM_PURCHASE != NULL)
			pDM_PURCHASE->SetCellData(_T("BP_����������"), strData1, 0);
		if(nVolumes == 1)
			pDM_BOOK->SetCellData(_T("BB_����������"), strData1, 0);
	}

	// �Ѽ���, �Ѽ�����
	CString strSeriesTitle, strSeriesVol;
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("490$a"), strSeriesTitle);
	pDM_IDX_BO->SetCellData(_T("IBS_�Ѽ���"), strSeriesTitle, nIdxRowIdx);
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("440$a"), strSeriesTitle);
	if(!strSeriesTitle.IsEmpty()) pDM_IDX_BO->SetCellData(_T("IBS_�Ѽ���"), strSeriesTitle, nIdxRowIdx);
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("440$v"), strSeriesVol);
	pDM_IDX_BO->SetCellData(_T("IBS_�Ѽ�����"), strSeriesVol, nIdxRowIdx);

	// ��ǥ��
	Get_TITLE(pMarc, strData1, strData2);
	if (strData1.GetLength() > 0 && strData2.GetLength() > 0) {
		strData3.Format(_T("%s=%s"), strData1, strData2);
		pDM_IDX_BO->SetCellData(_T("IBS_��ǥ��"), strData3, nIdxRowIdx);
	} else {
		pDM_IDX_BO->SetCellData(_T("IBS_��ǥ��"), strData1, nIdxRowIdx);
		//pDM_BOOK->SetCellData(_T("BB_������ηϸ�"), st rData1, nREPRESENTBOOKRowIdx);
	}

	// ������
	strData1 = _T("");
	Get_AUTHOR(pMarc, strData1);
	pDM_BOOK->SetCellData(_T("BB_������"), strData1, nREPRESENTBOOKRowIdx);
	pDM_IDX_BO->SetCellData(_T("IBS_������"), strData1, nIdxRowIdx);

	//������
	strData1 = _T("");
	Get_PUBLISHER(pMarc, strData1);
	pDM_IDX_BO->SetCellData(_T("IBS_������"), strData1, nIdxRowIdx);

	// Ư���� ����Ÿ ó�� - 020(����)		// 1���� ���� IDX�� ��ǥBOOK�� 1�� �̻��ϰ��� ����
	CString strMarcData1;
	INT nPos;
	if (nVolumes == 1) {
		FindItemByIndicator(pMarc, _T("020$a"), _T("  "), strMarcData1);
		//isbn�� 893726387382(�����)�̷��� ����. �׷��� '()'���� ���ڴ� �������Ѵ�.
		nPos = strMarcData1.Find(_T("("), 0);
		if(nPos >= 0) strMarcData1 = strMarcData1.Mid(0, nPos);
		pDM_IDX_BO->SetCellData(_T("IBS_����ISBN"), strMarcData1, nIdxRowIdx);	
		pDM_BOOK->SetCellData(_T("BB_����ISBN"), strMarcData1, nREPRESENTBOOKRowIdx);

		FindItemByIndicator(pMarc, _T("020$g"), _T("  "), strMarcData1);
		//���Ϲ��Խ� �ΰ���ȣ �������� - �ΰ���ȣ�� TCHAR(5)�̴� ���� �� �̻��� ��� Error�޽����� ����ؾ��Ѵ�.
/*
		if(strMarcData1.GetLength() > 5) 
		{
			strData1.Format(_T("����ISBN �ΰ���ȣ[%s]�� ������ �ֽ��ϴ�. �ش� ������ ������� �ʽ��ϴ�."), strMarcData1);
			pMgr->ESLAfxMessageBox(strData1);
			strMarcData1.Empty();
		}
*/
		((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strMarcData1);
		pDM_IDX_BO->SetCellData(_T("IBS_����ISBN�ΰ���ȣ"), strMarcData1, nIdxRowIdx);	
		pDM_BOOK->SetCellData(_T("BB_����ISBN�ΰ���ȣ"), strMarcData1, nREPRESENTBOOKRowIdx);

		FindItemByIndicator(pMarc, _T("020$z"), _T("  "), strMarcData1);
		pDM_IDX_BO->SetCellData(_T("IBS_���ǿ���ISBN"), strMarcData1, nIdxRowIdx);	
		pDM_BOOK->SetCellData(_T("BB_���ǿ���ISBN"), strMarcData1, nREPRESENTBOOKRowIdx);
	}
	// Ư���� ����Ÿ ó�� - 020(��Ʈ)
	FindItemByIndicator(pMarc, _T("020$a"), _T("1 "), strMarcData1);
	//��Ʈisbn�� 893726387382(��Ʈ)�̷��� ����. �׷��� '()'���� ���ڴ� �������Ѵ�.
	nPos = strMarcData1.Find(_T("("), 0);
	if(nPos >= 0) strMarcData1 = strMarcData1.Mid(0, nPos);
	pDM_IDX_BO->SetCellData(_T("IBS_��Ʈ_ISBN1"), strMarcData1, nIdxRowIdx);	
	FindItemByIndicator(pMarc, _T("020$g"), _T("1 "), strMarcData1);
	pDM_IDX_BO->SetCellData(_T("IBS_��Ʈ_ISBN2"), strMarcData1, nIdxRowIdx);
	FindItemByIndicator(pMarc, _T("020$z"), _T("1 "), strMarcData1);
	pDM_IDX_BO->SetCellData(_T("IBS_��Ʈ_ISBN3"), strMarcData1, nIdxRowIdx);	

	if (pDM_VOLUME) {
		delete pDM_VOLUME;
		pDM_VOLUME = NULL;
	}
	return 0;
}

INT CBO_SPECIES::APPLY_CMarc_to_Species_NotNullData(CMarc *pMarc, CESL_DataMgr *pDM_IDX_BO, CESL_DataMgr *pDM_SPECIES, CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_PURCHASE, INT nIdxRowIdx/*=0*/)
{
	if (pDM_IDX_BO == NULL) return -1;
	if (pDM_SPECIES == NULL) return -2;
	if (pDM_BOOK == NULL) return -3;
	if (pMgr == NULL) return -5;

	
	INT nREPRESENTBOOKRowIdx = Find_REPRESENT_BOOK_RowIdx(pDM_BOOK);
	if (nREPRESENTBOOKRowIdx < 0) nREPRESENTBOOKRowIdx = 0;

	CString strDM, strData1, strData2, strData3;

	CESL_DataMgr *pDM_VOLUME = new CESL_DataMgr();
	pDM_VOLUME->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
	pDM_VOLUME->InitDMPKFromFile(_T("DMUP_����_��_������"));
	pDM_VOLUME->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	CONVERT_BOOK_to_VOLUME(pDM_BOOK, pDM_VOLUME);
	INT nVolumes = pDM_VOLUME->GetRowCount();	
	// �Ϲ����� ����Ÿ ó��
	for (INT i = 0; i < nSpeciesMarcRelation; i++) 
	{
		strDM = SpeciesMarcRelation_Species[i].Left(2);
		strData1 = _T("");
		pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, SpeciesMarcRelation_Marc[i], strData1);
		if (strData1.IsEmpty()) continue;

		if (strDM == _T("IB")) pDM_IDX_BO->SetCellData(SpeciesMarcRelation_Species[i], strData1, nIdxRowIdx);
		if (strDM == _T("BB")) pDM_BOOK->SetCellData(SpeciesMarcRelation_Species[i], strData1, nREPRESENTBOOKRowIdx);
	}

	//260$c�� �����Ͱ� ������ 008 7-10�� �����Ͱ� ������ �ش� �����͸� ���� ����⿡ �����Ų��.
	strData1 = pDM_IDX_BO->GetCellData(_T("IBS_�����"), 0);
	if(strData1.IsEmpty())
	{
		pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@7-10"), strData1);
		if(!strData1.IsEmpty())
		{
			pDM_IDX_BO->SetCellData(_T("IBS_�����"), strData1, 0);
			pDM_BOOK->SetCellData(_T("BB_�����"), strData1, nREPRESENTBOOKRowIdx);
		}
	}
	
	// �����(��ũ->�� ����ô� ���� 1���� ��츸 ��ǥå�� �������ش�.)
	if (nVolumes == 1) {
		strData1 = _T("");
		pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$n"), strData1);
		if(!strData1.IsEmpty())
		{
			pDM_BOOK->SetCellData(_T("BB_�����"), strData1, nREPRESENTBOOKRowIdx);
		}
	}

	strData1.Empty();
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$n"), strData1);
	if(!strData1.IsEmpty())
	{
		pDM_IDX_BO->SetCellData(_T("IBS_�����"), strData1, 0);
	}
	
	// ����(�Ǽ���)
	if (nVolumes == 1) {
		strData1 = _T("");
		pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$p"), strData1);
		if(!strData1.IsEmpty())
		{
			pDM_BOOK->SetCellData(_T("BB_������ηϸ�"), strData1, nREPRESENTBOOKRowIdx);
		}
	}

	// ���� ����
	CString strPrice, strPriceCode;
	strData1 = _T("");
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strData1);
	if (strData1.GetLength() > 0) 
	{
		strData2 = _T("");		strData3 = _T("");
		Apply_MARC_950(strData1, strData2, strData3);
		if(nVolumes == 1 && !strData2.IsEmpty() && strData2 != _T("0"))
		{
			pDM_BOOK->SetCellData(_T("BB_����"), strData2, nREPRESENTBOOKRowIdx);
		}
		if(pDM_PURCHASE != NULL && !strData2.IsEmpty() && strData2 != _T("0") && !strData3.IsEmpty())
		{
			pDM_PURCHASE->SetCellData(_T("BP_����"), strData2, 0);
			pDM_PURCHASE->SetCellData(_T("BP_ȭ�󱸺�"), strData3, 0);
		}
	}	
	strData1.Empty();
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$a"), strData1);
	if(!strData1.IsEmpty())
	{
		if(pDM_PURCHASE != NULL)
			pDM_PURCHASE->SetCellData(_T("BP_���ݼ���"), strData1, 0);
		if(nVolumes == 1)
			pDM_BOOK->SetCellData(_T("BB_���ݼ���"), strData1, 0);
	}
	strData1.Empty();
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$c"), strData1);
	if(!strData1.IsEmpty())
	{
		
		if(pDM_PURCHASE != NULL)
			pDM_PURCHASE->SetCellData(_T("BP_����������"), strData1, 0);
		if(nVolumes == 1)
			pDM_BOOK->SetCellData(_T("BB_����������"), strData1, 0);
	}

	// �Ѽ���, �Ѽ�����
	CString strSeriesTitle, strSeriesVol;
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("490$a"), strSeriesTitle);
	if(!strSeriesTitle.IsEmpty()) pDM_IDX_BO->SetCellData(_T("IBS_�Ѽ���"), strSeriesTitle, nIdxRowIdx);
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("440$a"), strSeriesTitle);
	if(!strSeriesTitle.IsEmpty()) pDM_IDX_BO->SetCellData(_T("IBS_�Ѽ���"), strSeriesTitle, nIdxRowIdx);
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("440$v"), strSeriesVol);
	if(!strSeriesVol.IsEmpty()) pDM_IDX_BO->SetCellData(_T("IBS_�Ѽ�����"), strSeriesVol, nIdxRowIdx);

	// ��ǥ��
	Get_TITLE(pMarc, strData1, strData2);
	if (strData1.GetLength() > 0 && strData2.GetLength() > 0) 
	{
		strData3.Format(_T("%s=%s"), strData1, strData2);
		pDM_IDX_BO->SetCellData(_T("IBS_��ǥ��"), strData3, nIdxRowIdx);
	}
	else if(!strData1.IsEmpty())
	{
		pDM_IDX_BO->SetCellData(_T("IBS_��ǥ��"), strData1, nIdxRowIdx);
	}

	// ������
	strData1 = _T("");
	Get_AUTHOR(pMarc, strData1);
	if(!strData1.IsEmpty())
	{
		pDM_BOOK->SetCellData(_T("BB_������"), strData1, nREPRESENTBOOKRowIdx);
		pDM_IDX_BO->SetCellData(_T("IBS_������"), strData1, nIdxRowIdx);
	}

	//������
	strData1 = _T("");
	Get_PUBLISHER(pMarc, strData1);
	if(!strData1.IsEmpty()) pDM_IDX_BO->SetCellData(_T("IBS_������"), strData1, nIdxRowIdx);

	// Ư���� ����Ÿ ó�� - 020(����)		// 1���� ���� IDX�� ��ǥBOOK�� 1�� �̻��ϰ��� ����
	CString strMarcData1;
	INT nPos;
	if (nVolumes == 1) {
		FindItemByIndicator(pMarc, _T("020$a"), _T("  "), strMarcData1);
		if(!strMarcData1.IsEmpty())
		{
			//isbn�� 893726387382(�����)�̷��� ����. �׷��� '()'���� ���ڴ� �������Ѵ�.
			nPos = strMarcData1.Find(_T("("), 0);
			if(nPos >= 0) strMarcData1 = strMarcData1.Mid(0, nPos);
			pDM_IDX_BO->SetCellData(_T("IBS_����ISBN"), strMarcData1, nIdxRowIdx);	
			pDM_BOOK->SetCellData(_T("BB_����ISBN"), strMarcData1, nREPRESENTBOOKRowIdx);
		}

		FindItemByIndicator(pMarc, _T("020$g"), _T("  "), strMarcData1);
		if(!strMarcData1.IsEmpty())
		{
			//���Ϲ��Խ� �ΰ���ȣ �������� - �ΰ���ȣ�� TCHAR(5)�̴� ���� �� �̻��� ��� Error�޽����� ����ؾ��Ѵ�.
			if(strMarcData1.GetLength() > 5) 
			{
				strData1.Format(_T("����ISBN �ΰ���ȣ[%s]�� ������ �ֽ��ϴ�. �ش� ������ ������� �ʽ��ϴ�."), strMarcData1);
				pMgr->ESLAfxMessageBox(strData1);
				strMarcData1.Empty();
			}
			
			((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strMarcData1);
			pDM_IDX_BO->SetCellData(_T("IBS_����ISBN�ΰ���ȣ"), strMarcData1, nIdxRowIdx);	
			pDM_BOOK->SetCellData(_T("BB_����ISBN�ΰ���ȣ"), strMarcData1, nREPRESENTBOOKRowIdx);
		}
	
		FindItemByIndicator(pMarc, _T("020$z"), _T("  "), strMarcData1);
		if(!strMarcData1.IsEmpty())
		{
			pDM_IDX_BO->SetCellData(_T("IBS_���ǿ���ISBN"), strMarcData1, nIdxRowIdx);	
			pDM_BOOK->SetCellData(_T("BB_���ǿ���ISBN"), strMarcData1, nREPRESENTBOOKRowIdx);
		}
	}
	// Ư���� ����Ÿ ó�� - 020(��Ʈ)
	FindItemByIndicator(pMarc, _T("020$a"), _T("1 "), strMarcData1);
	if(!strMarcData1.IsEmpty())
	{
		//��Ʈisbn�� 893726387382(��Ʈ)�̷��� ����. �׷��� '()'���� ���ڴ� �������Ѵ�.
		nPos = strMarcData1.Find(_T("("), 0);
		if(nPos >= 0) strMarcData1 = strMarcData1.Mid(0, nPos);
		pDM_IDX_BO->SetCellData(_T("IBS_��Ʈ_ISBN1"), strMarcData1, nIdxRowIdx);	
	}
	FindItemByIndicator(pMarc, _T("020$g"), _T("1 "), strMarcData1);
	if(!strMarcData1.IsEmpty()) pDM_IDX_BO->SetCellData(_T("IBS_��Ʈ_ISBN2"), strMarcData1, nIdxRowIdx);	
	FindItemByIndicator(pMarc, _T("020$z"), _T("1 "), strMarcData1);
	if(!strMarcData1.IsEmpty()) pDM_IDX_BO->SetCellData(_T("IBS_��Ʈ_ISBN3"), strMarcData1, nIdxRowIdx);	

	if (pDM_VOLUME) {
		delete pDM_VOLUME;
		pDM_VOLUME = NULL;
	}
	return 0;
}

INT CBO_SPECIES::Find_REPRESENT_BOOK_RowIdx(CESL_DataMgr *pDM_BOOK)
{
	if (pDM_BOOK == NULL) return -1;
	INT nRows = pDM_BOOK->GetRowCount();
	CString strData;
	CString strDB_FLAG;
	for (INT i = 0; i < nRows; i++) {
		strDB_FLAG = _T("");
		pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, strDB_FLAG);
		if (strDB_FLAG == _T("D")) continue;
		strData = _T("");
		pDM_BOOK->GetCellData(_T("BB_��ǥå����"), i, strData);
		strData.TrimLeft();			strData.TrimRight();
		if (strData == _T("Y")) return i;
	}
	return -4005;		// ��ǥå�� ã�� �� ����
}

INT CBO_SPECIES::Get_BOOK_VOL_INDEX(CESL_DataMgr *pDM_VOLUME, CString &strResult)
{
	if (pDM_VOLUME == NULL) return -2;
	strResult = _T("");
	CString strData, strTmp;
	INT nVolumes = pDM_VOLUME->GetRowCount();
	for (INT i = 0; i < nVolumes; i++) {
		strData = _T("");
		pDM_VOLUME->GetCellData(_T("BB_�����"), i, strData);
		if (strData.GetLength() == 0) continue;
		if (strResult.GetLength() == 0) strTmp = strData;
			else strTmp.Format(_T(",%s"), strData);
		strResult += strTmp;
	}
	return 0;
}

INT CBO_SPECIES::Get_PRICE(CESL_DataMgr *pDM_VOLUME, CString &strResult)
{
	if (pDM_VOLUME == NULL) return -2;
	strResult = _T("");
	INT nPrice = 0;
	CString strData, strTmp;
	INT nVolumes = pDM_VOLUME->GetRowCount();
	for (INT i = 0; i < nVolumes; i++) {
		strData = _T("");
		pDM_VOLUME->GetCellData(_T("BB_����"), i, strData);
		if (strData.GetLength() == 0) continue;
			else nPrice += _ttoi(strData);
		strData = _T("");
	}
	strResult.Format(_T("%d"), nPrice);
	return 0;
}

INT CBO_SPECIES::Set_Index_ST_CODE(CString strAlias, CESL_DataMgr * pIDX_DM, CESL_DataMgr * pBOOK_DM, INT mode, INT nIdx/*=0*/)
{
	//mode ���� -1 : ���� ISBN �迭�� �����ؾ� �ϴ� ��� 1 : ���� ST_CODE���� ����ؾ� �ϴ� ���
	CString strSTCODE, strSETISBN, strDelimeter;
	CStringArray arrayISBN;
	if(mode > 0)
	{
		strDelimeter = _T(" ");
		pIDX_DM->GetCellData(_T("IBS_ISBN"), 0, strSTCODE);
		INT ids = ParsingString(strDelimeter, strSTCODE, &arrayISBN);
	}
	pIDX_DM->GetCellData(_T("IBS_��Ʈ_ISBN1"), 0, strSETISBN);
	if(strSETISBN.GetLength() > 0)
	{
		((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSETISBN);
		if(!CheckIsHave(&arrayISBN, strSETISBN)) arrayISBN.Add(strSETISBN);
	}
	
	CString strEAISBN, strEXTRA_EAISBN, strDBFlag;
	strDelimeter.Format(_T("%c"), 3);
	INT rowCnt = pBOOK_DM->GetRowCount();
	for(INT i=0 ; i<rowCnt ; i++)
	{
		pBOOK_DM->GetCellData(_T("BB_DB_FLAG"), i, strDBFlag);
		if(strDBFlag == _T("D")) continue;
		pBOOK_DM->GetCellData(_T("BB_����ISBN"), i, strEAISBN);
		((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strEAISBN);
		if(!CheckIsHave(&arrayISBN, strEAISBN)) arrayISBN.Add(strEAISBN);
		pBOOK_DM->GetCellData(_T("BB_�߰�����ISBN"), i, strEXTRA_EAISBN);
		ParsingString(_T("|"), strEXTRA_EAISBN, &arrayISBN);
	}

	strSTCODE.Empty();
	for(i=0 ; i<arrayISBN.GetSize() ; i++)
	{
		if(strSTCODE.IsEmpty()) strSTCODE = arrayISBN.GetAt(i);
		else strSTCODE += _T(" ") + arrayISBN.GetAt(i);
	}

	pIDX_DM->SetCellData(_T("IBS_ISBN"), strSTCODE, nIdx);
	return 0;
}

INT CBO_SPECIES::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray, BOOL bIndexing/*=TRUE*/)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos >= 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+DelimeterStr.GetLength());
		if(bIndexing) ((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		if(!bIndexing || !CheckIsHave(pTargetArray, strTmpData))
			pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	if(bIndexing) ((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(!bIndexing || !CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	return 0;
}

INT CBO_SPECIES::Set_TITLE(CMarc *pMarc, CString strData)
{
	if (pMarc == NULL) return -1;
	//if (strData.GetLength() == 0) return 0;
	//������� 245�ʵ�  $a, $x, $b Ŭ����
	pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$a"));
	pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$x"));
	pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$b"));
	//pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("245"));
	
	INT pos = strData.Find(_T("="), 0);
	CString strData1, strData2, strData3;
	if (pos < 0) 
	{
		strData1 = strData;
		pos = strData.Find(_T(":"), 0);
		if(pos < 0)
		{
			pos = strData.Find(_T("("));
			if(pos < 0 || pos != 0) pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
			else 
			{
				pos = strData.Find(_T(")"));
				if(pos < 0) 
				{
					strData1.Replace(_T("("), _T(""));
					pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
				}
				else
				{
					pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
					pMgr->m_pInfo->pMarcMgr->SetTagIndicator(pMarc, _T("245"), _T("2"), _T("0"));
				}
			}
		}
		else
		{
			strData1 = strData.Left(pos);
			strData2 = strData.Mid(pos+1);
			pos = strData1.Find(_T("("));
			if(pos < 0 || pos != 0) pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
			else 
			{
				pos = strData.Find(_T(")"));
				if(pos < 0) 
				{
					strData1.Replace(_T("("), _T(""));
					pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
				}
				else
				{
					pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
					pMgr->m_pInfo->pMarcMgr->SetTagIndicator(pMarc, _T("245"), _T("2"), _T("0"));
				}
			}
			pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$b"), strData2, _T(""), MODIFY_ITEM);
		}
	}
	else 
	{
		strData1 = strData.Left(pos);
		strData2 = strData.Mid(pos+1);
		pos = strData1.Find(_T("("));
		if(pos < 0 || pos != 0) pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
		else 
		{
			pos = strData.Find(_T(")"));
			if(pos < 0) 
			{
				strData1.Replace(_T("("), _T(""));
				pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
			}
			else
			{
				pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
				pMgr->m_pInfo->pMarcMgr->SetTagIndicator(pMarc, _T("245"), _T("2"), _T("0"));
			}
		}
		pos = strData2.Find(_T(":"), 0);
		if(pos < 0) pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$x"), strData2, _T(""), MODIFY_ITEM);
		else 
		{
			strData1 = strData2.Left(pos);
			strData3 = strData2.Mid(pos+1);
			pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$x"), strData1, _T(""), MODIFY_ITEM);
			pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$b"), strData3, _T(""), MODIFY_ITEM);
		}
	}
	return 0;
}

INT CBO_SPECIES::Set_AUTHOR(CMarc *pMarc, CString strIndexData)
{
	if (pMarc == NULL) return -1;
	INT pos = -1;
	INT nPrevPos = 0;
	pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$d"));
	pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$e"));
	CString strData1;
	for (INT i = 0; true; i++) {
		strData1 = _T("");
		pos = strIndexData.Find(';', nPrevPos);
		if (pos == -1) strData1 = strIndexData.Mid(nPrevPos);
		else strData1 = strIndexData.Mid(nPrevPos, pos - nPrevPos);
		
		nPrevPos = pos+1;
		if (i == 0) 
		{
			pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$d"), strData1);
		}
		else
		{
			if (strData1.GetLength() > 0) pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$e"), strData1);
		}
		if (pos == -1) break;
	}
	return 0;
}

INT CBO_SPECIES::Set_PUBLIHSER(CMarc *pMarc, CString strIndexData)
{
	if (pMarc == NULL) return -1;
	INT pos = -1;
	INT nPrevPos = 0;
	pMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("260$b"));
	CString strData1;
	for (INT i = 0; true; i++) {
		strData1 = _T("");
		pos = strIndexData.Find(':', nPrevPos);
		if (pos == -1) strData1 = strIndexData.Mid(nPrevPos);
		else strData1 = strIndexData.Mid(nPrevPos, pos - nPrevPos);
		
		nPrevPos = pos+1;
		pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("260$b"), strData1);
		if (pos == -1) break;
	}
	return 0;
}

INT CBO_SPECIES::Get_TITLE(CMarc *pMarc, CString &strData1, CString &strData2)
{
	strData1 = _T("");			strData2 = _T("");
	if (pMarc == NULL) return -1;
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$a"), strData1);
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$x"), strData2);
	//�μ��� ó��
	CString strTmpData;
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$b"), strTmpData);
	if(strTmpData.GetLength() > 0)
		if(strData2.GetLength() > 0)
			strData2 += _T(":") + strTmpData;
		else 
			strData1 += _T(":") + strTmpData;
	return 0;
}

INT CBO_SPECIES::Get_AUTHOR(CMarc *pMarc, CString &strIndexData)
{
	if (pMarc == NULL) return -1;
	CString strMarcData1, strData, strMarcData2, strTmpData, strTmpData1;
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strMarcData1);
	strData.Format(_T("%s"), strMarcData1);
	CArray <CString, CString&> arrData;
	arrData.RemoveAll();
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$e"), strMarcData2, &arrData);
	for (INT i = 0; i < arrData.GetSize(); i++) {
		strTmpData1 = arrData.GetAt(i);
		if (strData.GetLength() == 0) strTmpData.Format(_T("%s"), strTmpData1);
		else strTmpData.Format(_T("%s;%s"), strData, strTmpData1);
		strData = strTmpData;
	}
	strIndexData = strData;
	return 0;
}

INT CBO_SPECIES::Get_PUBLISHER(CMarc *pMarc, CString &strPublisher)
{
	if (pMarc == NULL) return -1;
	CString strMarcData, strTmpData, strData;
	CArray <CString , CString&> strMarcArrayData;
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("260$b"), strMarcData, &strMarcArrayData);
	for (INT i = 0; i < strMarcArrayData.GetSize(); i++) {
		if (strData.GetLength() == 0) strTmpData.Format(_T("%s"), strMarcArrayData.GetAt(i));
		else strTmpData.Format(_T("%s:%s"), strData, strMarcArrayData.GetAt(i));
		strData = strTmpData;
	}
	strPublisher = strData;
	return 0;
}

BOOL CBO_SPECIES::IsBook(CESL_DataMgr *pDM_Book, INT idx, LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_å�η��÷���"), idx) == _T("A")) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D")) return FALSE;

	// ������� ������ üũ
	if (lpszVol != NULL)
		if (pDM_Book->GetCellData(_T("BB_�����"), idx) != lpszVol) return FALSE;

	return TRUE;
}

INT CBO_SPECIES::GetBookCount(CESL_DataMgr *pDM_Book, CString strRegCode /*= _T("")*/, LPCTSTR lpszVol /*=NULL*/)
{
	if (pDM_Book == NULL) return -1;

	INT nReturnCnt = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// å�� ���� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx)) continue;

		// ��ϱ��� ��
		if (!strRegCode.IsEmpty() && lpszVol != _T("BB_��ǰ����"))
		{
			if (pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), idx) != strRegCode) continue;
		}
		else if(!strRegCode.IsEmpty() && lpszVol == _T("BB_��ǰ����"))
		{
			if (pDM_Book->GetCellData(lpszVol, idx) != strRegCode) continue;	
		}
			
		nReturnCnt++;
	}

	return nReturnCnt;
}

INT CBO_SPECIES::BO_VOLUME_DELETE(CString strSpeciesKey, 
								  CString strAccuKey, 
								  CString strVolSortNo, 
								  INT nSpeciesType, 
								  INT nAcqType, 
								  BOOL bRecycleYN,
								  CGroupInfo * pGroup)
{
	// FROM : _T("PURCHASE") or _T("DONATE")
	// MODE : _T("DELETE") or _T("RECYCLE")
	// ���� 1 row delete -> ��Ȱ������ 1 row insert(�÷��� �������ٰ�)
	// ���������� �� ���� 
	// (å)�������� n rows delete -> ��Ȱ��å�� n rows insert(�÷��� �������ٰ�)(��Ȱ���ϰ�� ������ ����)
	
	SPECIES_TYPE STYPE = (SPECIES_TYPE)nSpeciesType;
	ACQ_TYPE ATYPE = (ACQ_TYPE)nAcqType;

	INT nRowIdx = 0;
	INT ids;
	CString strOption, strQuery, strKey, strRegNo, strMarc, strTmpData, strBookQuery;
	strKey = strSpeciesKey;

	if(!CheckHaveOtherVolumeInfo(m_pDM_BOOK, strSpeciesKey, strAccuKey, strVolSortNo))
	{
		if(bRecycleYN)
			ids = BO_SPECIES_RECYCLE(strSpeciesKey, strAccuKey, nAcqType, pGroup );
		else
			ids = BO_SPECIES_DELETE(strSpeciesKey, strAccuKey, nAcqType);
		return ids;
	}
	
	// �ش� ���� åDM�� �����Ѵ�.
	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	m_pDM_IDX_BO->RefreshDataManager(strOption);
	
	if(!strAccuKey.IsEmpty() && strAccuKey != _T("-1"))
	{
		if(strVolSortNo.IsEmpty())
			strOption.Format(_T("BOOK_APPENDIX_FLAG = 'B' AND ACQ_KEY = %s AND VOL_SORT_NO IS NULL"), strAccuKey);
		else
			strOption.Format(_T("BOOK_APPENDIX_FLAG = 'B' AND ACQ_KEY = %s AND VOL_SORT_NO = '%s'"), strAccuKey, strVolSortNo);
	}
	else
	{
		if(strVolSortNo.IsEmpty())
			strOption.Format(_T("BOOK_APPENDIX_FLAG = 'B' AND SPECIES_KEY = %s AND (ACQ_KEY IS NULL OR ACQ_KEY = -1) AND VOL_SORT_NO IS NULL"), strSpeciesKey);
		else
			strOption.Format(_T("BOOK_APPENDIX_FLAG = 'B' AND SPECIES_KEY = %s AND (ACQ_KEY IS NULL OR ACQ_KEY = -1) AND VOL_SORT_NO = '%s'"), strSpeciesKey, strVolSortNo);
	}
	ids = m_pDM_BOOK->RefreshDataManager(strOption);
	if (ids < 0) return -31003;
	INT nRowCount = m_pDM_BOOK->GetRowCount();
	if(nRowCount == 0) return 1000;
	
	strBookQuery = strOption;
	strOption.Format(_T("BOOK_KEY IN (SELECT REC_KEY FROM BO_BOOK_TBL WHERE %s)"), strBookQuery);
	ids = m_pDM_EBook->RefreshDataManager(strOption);
	// ������ EBook������ DB_FLAG�� _T("D")�� �����Ѵ�.
	ids = m_pDM_EBook->GetRowCount();
	for(INT idx=0 ; idx<ids ; idx++)
	{
		m_pDM_EBook->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}
	ids = m_pDM_BOOK->GetRowCount();
	for(idx=0 ; idx<ids ; idx++)
	{
		m_pDM_BOOK->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}	
	
	m_pDM_BOOK->StartFrame();

	MakeBookQueryFrame(m_pDM_BOOK, strSpeciesKey, strAccuKey, m_pDM_BOOK);

	strMarc = UpdateVolumeDeleteMarc(m_pDM_BOOK, strSpeciesKey, strAccuKey, strVolSortNo);
	m_pDM_SPECIES->InsertRow(-1);	
	m_pDM_SPECIES->SetCellData(_T("BS_MARC"), strMarc, 0);
	UpdateAccuInfo(m_pDM_BOOK, m_pDM_BOOK, strAccuKey, strMarc, nAcqType, m_pDM_SPECIES->GetCellData(_T("BS_��KEY"), 0));
	m_pDM_SPECIES->SetCellData(_T("BS_MARC"), strMarc, 0);

	SynchronizedIndexAndBookData(strSpeciesKey, m_pDM_BOOK);
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	BO_INDEX_TEMP_INSERT(m_pDM_BOOK, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("I"));
	//=====================================================
	
	ids = MakeEBookQueryFrame(m_pDM_EBook, m_pDM_BOOK);
	if(ids < 0) return ids;
	
	//ECOSEARCH�� ���ؼ� �������
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		m_pDM_BOOK->AddFrame(strQuery);
	}
	// ���� Ʈ���������� �������
	ids = UpdateSpeciesMarcbyStream(m_pDM_BOOK, _T("BO_SPECIES_TBL"), strSpeciesKey, m_pDM_SPECIES->GetCellData(_T("BS_MARC"), 0));	
	if(ids < 0) return -31053;
	ids = m_pDM_BOOK->SendFrame();
	if(ids < 0) return -31051;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return -31051;

	return 0;
}

INT CBO_SPECIES::UpdateAccuInfo(
								CESL_DataMgr * pDM_OUT, 
								CESL_DataMgr * pDM_BOOK,
								CString strAccuKey, 
								CString &strMarc, 
								INT nAcqType,
								CString strSpeciesKey/*=_T("")*/)
{
	if(strAccuKey == _T("ALL")) return 0;

	CString strQuery, strTmpData, strPrice, strDiscRate;
	
	if(nAcqType == 0) return 0;
	else if(nAcqType == 1) SetAccuDM(TRUE);
	else if(nAcqType == 2) SetAccuDM(FALSE);
	strQuery.Format(_T("REC_KEY = %s"), strAccuKey);
	m_pDM_ACCU->RefreshDataManager(strQuery);
	if(m_pDM_ACCU->GetRowCount() == 0) return 0;
	
	if(!strSpeciesKey.IsEmpty())
		m_pDM_ACCU->SetCellData(_T("BP_��KEY"), strSpeciesKey, 0);
	
	strTmpData.Format(_T("%d"), pDM_BOOK->GetRowCount());
	m_pDM_ACCU->SetCellData(_T("BP_����å��"), strTmpData, 0);
	strTmpData.Format(_T("%d"), GetVolumeCount(pDM_BOOK));
	m_pDM_ACCU->SetCellData(_T("BP_�ѱǼ�"), strTmpData, 0);

	strPrice = GetTotalPrice(pDM_BOOK);
	strDiscRate = m_pDM_ACCU->GetCellData(_T("BP_������"), 0);
	strTmpData.Format(_T("%12.2f"), atof(strPrice.GetBuffer(0)) - (atof(strPrice.GetBuffer(0))*(atof(strDiscRate.GetBuffer(0))/100)));
	m_pDM_ACCU->SetCellData(_T("BP_���ְ���"), strTmpData, 0);	
	strPrice = GetTotalPrice(pDM_BOOK, _T("Y"));
	strTmpData.Format(_T("%12.2f"), atof(strPrice.GetBuffer(0)) - (atof(strPrice.GetBuffer(0))*(atof(strDiscRate.GetBuffer(0))/100)));
	m_pDM_ACCU->SetCellData(_T("BP_��ǰ����"), strTmpData, 0);	
	strTmpData.Format(_T("%d"), GetBookCount(pDM_BOOK, _T("Y"), _T("BB_��ǰ����")));
	m_pDM_ACCU->SetCellData(_T("BP_��ǰå��"), strTmpData, 0);	
	strTmpData.Format(_T("%d"), GetBookCount(pDM_BOOK, _T("N"), _T("BB_��ǰ����")));
	m_pDM_ACCU->SetCellData(_T("BP_�̳�å��"), strTmpData, 0);	

	CONVERT_BOOK_to_VOLUMEInBOOK(pDM_BOOK);
	
	strPrice = GetTotalPrice(pDM_BOOK);
	m_pDM_ACCU->SetCellData(_T("BP_����"), strPrice, 0);	
	strTmpData = m_pDM_ACCU->GetCellData(_T("BP_ȭ�󱸺�"), 0);
	strTmpData += strPrice;

	CMarc marc;
	pMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	pMgr->m_pInfo->pMarcMgr->DeleteItem(&marc, _T("950$b"), strTmpData );
	pMgr->m_pInfo->pMarcMgr->SetItem( &marc, _T("950$b"), strTmpData );
	pMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	m_pDM_ACCU->MakeQueryByDM(RemoveAliasList, m_pDM_ACCU, m_pDM_ACCU->TBL_NAME, 0, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strAccuKey);
	return 0;
}

BOOL CBO_SPECIES::CheckHaveOtherVolumeInfo(CESL_DataMgr * pDM, CString strSpeciesKey, CString strAccuKey, CString strVolSortNo)
{
	CString strQuery;
	if(strVolSortNo.IsEmpty())
		strQuery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s AND ACQ_KEY=%s AND VOL_SORT_NO IS NOT NULL"),
			strSpeciesKey, strAccuKey, strVolSortNo);
	else
		strQuery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s AND ACQ_KEY=%s AND VOL_SORT_NO != %s"),
			strSpeciesKey, strAccuKey, strVolSortNo);
	
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	tmpDM.RestructDataManager(strQuery);
	if(tmpDM.GetRowCount() > 0) return TRUE;
	return FALSE;
}

BOOL CBO_SPECIES::CheckHaveOtherBookInfo(CESL_DataMgr * pDM, CString strSpeciesKey, CString strAccuKey)
{
	CString strQuery;
	if(!strAccuKey.IsEmpty() && strAccuKey != _T("-1"))
		strQuery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s AND ACQ_KEY=%s"),
			strSpeciesKey, strAccuKey);
	else
		strQuery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s"),
			strSpeciesKey);
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	tmpDM.RestructDataManager(strQuery);
	if(tmpDM.GetRowCount() > 1) return TRUE;
	else if(tmpDM.GetRowCount() == 1) return FALSE;
	return FALSE;
}

CString CBO_SPECIES::UpdateVolumeDeleteMarc(CESL_DataMgr * pDM_BOOK, CString strSpeciesKey, CString strAcqKey, CString strVolSortNo)
{
	CString strWhere ;
	strWhere.Format(_T("species_key = %s and acq_key=%s and vol_sort_no != %s"), strSpeciesKey, strAcqKey, strVolSortNo);
	pDM_BOOK->RefreshDataManager(strWhere);
	if(pDM_BOOK->GetRowCount() == 0) return _T("");
	strWhere.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY=%s"), strSpeciesKey);
	CString strMarc;
	pDM_BOOK->GetOneValueQuery(strWhere, strMarc);
	CMarc marc;
	pMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);

	pMgr->m_pInfo->pMarcMgr->DeleteItem( &marc, _T("049$l") );
	pMgr->m_pInfo->pMarcMgr->DeleteItem( &marc, _T("049$v") );
	pMgr->m_pInfo->pMarcMgr->DeleteItem( &marc, _T("049$c") );
	pMgr->m_pInfo->pMarcMgr->DeleteItem( &marc, _T("049$f") );
	
	CString sRegNo;
	CString sVolNoPre;
	INT iCopyCode = 1;
	INT iSwRegNo = 0;
	CString sPreShelfCode;	//���� ��ġ��ȣ
	INT iSwShelfCode = 0;
	INT ids;
	for ( INT i = 0 ; i < pDM_BOOK->GetRowCount() ; i ++ ) {
		
		ids = pDM_BOOK->GetCellData( _T("BB_��Ϲ�ȣ"), i, sRegNo );		
		if ( sRegNo != _T("") ) {
			ids = pMgr->m_pInfo->pMarcMgr->DeleteItem(&marc, _T("049$l"), sRegNo );
			ids = pMgr->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
		}
		CString sItemData = _T("");
		ids = pDM_BOOK->GetCellData( _T("BB_�����"), i, sItemData );
		if ( sItemData != _T("") && sRegNo != _T("") ) {	
			ids = pMgr->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$v"), sItemData );
			if ( sVolNoPre == sItemData ) {
				iCopyCode++;
				CString sCopyCode;
				sCopyCode.Format( _T("%d"), iCopyCode );
				
				ids = pMgr->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$c"), sCopyCode );
				ids = pDM_BOOK->SetCellData( _T("BB_������ȣ"), sCopyCode, i );
			}
			else{
				iCopyCode = 1;
			}
		}
		sVolNoPre = sItemData;
		//��ġ��ȣ
		if ( sRegNo != _T("") ) {		//��Ϲ�ȣ�� ������
			sRegNo = _T("");
			ids = pDM_BOOK->GetCellData( _T("BB_��ġ��ȣ_�ڵ�"), i, sRegNo );
			if ( sRegNo != _T("") ) {
				ids = pMgr->m_pInfo->pMarcMgr->DeleteItem( &marc, _T("049$f"), sRegNo );
				ids = pMgr->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$f"), sRegNo );
			}
			if ( (i != 0) && (sPreShelfCode != sRegNo) && sPreShelfCode != _T("") ) {
				CString s049field;
				pMgr->m_pInfo->pMarcMgr->GetField( &marc, _T("049"), s049field );
				s049field = _T("1") + s049field.Mid(1);
				pMgr->m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
				pMgr->m_pInfo->pMarcMgr->InsertField( &marc, _T("049") + s049field );
				iSwShelfCode = 1;
			}
			sPreShelfCode = sRegNo;
		}
	}
	//0490 �ֱ�
	if ( iSwShelfCode == 0 ) {
		CString s049field;
		pMgr->m_pInfo->pMarcMgr->GetField( &marc, _T("049"), s049field );
		if ( s049field != _T("") ) {
			s049field = _T("0") + s049field.Mid(1);
		}
		pMgr->m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
		pMgr->m_pInfo->pMarcMgr->InsertField( &marc, _T("049") + s049field );
	}

	pMgr->m_pInfo->pMarcMgr->SortMarc( &marc );
	pMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);

	
	return strMarc;
}

INT CBO_SPECIES::BO_SPECIES_DELETE(CString strSpeciesKey, 
								   CString strAccuKey, 
								   INT nAcqType)
{
	// FROM : _T("PURCHASE") or _T("DONATE")
	// MODE : _T("DELETE") or _T("RECYCLE")
	// ���� 1 row delete -> ��Ȱ������ 1 row insert(�÷��� �������ٰ�)
	// ���������� �� ���� 
	// (å)�������� n rows delete -> ��Ȱ��å�� n rows insert(�÷��� �������ٰ�)(��Ȱ���ϰ�� ������ ����)

	if (strSpeciesKey.GetLength() == 0) return -31001;	// ��KEY���� 
	INT nRowIdx = 0;

	INT ids;
	CArray <CString, CString> RemoveAliasList;
	BOOL bCreateSpeciesKey = FALSE;

	m_pDM_SPECIES->StartFrame();
	if(m_pDM_SPECIES->GetRowCount() == 0)
		m_pDM_SPECIES->InsertRow(-1);
	m_pDM_SPECIES->SetCellData(_T("BS_��KEY"), strSpeciesKey, 0);

	CString strOption, strQuery, strKey, strBookQuery, strTmpData;
	strKey = strSpeciesKey;

	// �ش� ���� åDM�� �����Ѵ�.
	if(!strAccuKey.IsEmpty() && strAccuKey != _T("-1") && strAccuKey != _T("ALL"))
		strOption.Format(_T("SPECIES_KEY = %s AND ACQ_KEY = %s AND (WORKING_STATUS<'BOL' OR WORKING_STATUS>'BOR')"), strSpeciesKey, strAccuKey);
	else if(strAccuKey == _T("ALL"))
		strOption.Format(_T("SPECIES_KEY = %s AND (WORKING_STATUS<'BOL' OR WORKING_STATUS>'BOR')"), strSpeciesKey);
	else
		strOption.Format(_T("SPECIES_KEY = %s AND (ACQ_KEY IS NULL OR ACQ_KEY = -1) AND (WORKING_STATUS<'BOL' OR WORKING_STATUS>'BOR')"), strSpeciesKey);
	ids = m_pDM_BOOK->RefreshDataManager(strOption);
	if (ids < 0) return -31003;

	//������ å������ ���ٸ� return
	if(m_pDM_BOOK->GetRowCount() == 0) return 1000;
	
	// EBook������ �����Ѵ�.
	strBookQuery = strOption;
	strOption.Format(_T("BOOK_KEY IN (SELECT REC_KEY FROM BO_BOOK_TBL WHERE %s)"), strBookQuery);
	ids = m_pDM_EBook->RefreshDataManager(strOption);
	// ������ EBook������ DB_FLAG�� _T("D")�� �����Ѵ�.
	ids = m_pDM_EBook->GetRowCount();
	for(INT idx=0 ; idx<ids ; idx++)
	{
		m_pDM_EBook->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}
	// ������ Book������ DB_FLAG�� _T("D")�� �����Ѵ�.
	ids = m_pDM_BOOK->GetRowCount();
	for(idx=0 ; idx<ids ; idx++)
	{
		m_pDM_BOOK->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}
	
	MakeBookQueryFrame(m_pDM_BOOK, strSpeciesKey, strAccuKey, m_pDM_SPECIES);
	ids = MakeEBookQueryFrame(m_pDM_EBook, m_pDM_SPECIES);
	if(ids < 0) return ids;
	
	//����/�������� ����
	if(nAcqType == 1 || nAcqType == 2)
	{
		DeleteAccuInfo(m_pDM_SPECIES, strAccuKey, nAcqType);
	}
	else if(nAcqType == 0 && !strAccuKey.IsEmpty())
	{
		if(m_pDM_BOOK->GetCellData(_T("BB_���Ա���_�ڵ�"), 0) == _T("1")) nAcqType = 1;
		else if(m_pDM_BOOK->GetCellData(_T("BB_���Ա���_�ڵ�"), 0) == _T("2")) nAcqType = 2;
		DeleteAccuInfo(m_pDM_SPECIES, strAccuKey, nAcqType);
	}

	// �������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
		
	if(!CheckHaveOtherAccuInfo(strSpeciesKey, strAccuKey))
	{
		strQuery.Format(_T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
		ids = m_pDM_SPECIES->AddFrame(strQuery);
		if(ids < 0) return -1;
		strQuery.Format(_T("DELETE FROM IDX_BO_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
		ids = m_pDM_SPECIES->AddFrame(strQuery);
		if(ids < 0) return -1;
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		BO_INDEX_TEMP_INSERT(m_pDM_SPECIES, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("D"));	
		//=====================================================
		
		BO_TOC_ABSTRACT_DELETE(m_pDM_SPECIES, strSpeciesKey);
	
		if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_DELETE_BO$I(%s);"), strSpeciesKey);
			m_pDM_SPECIES->AddFrame(strQuery);
		}
	}
	else
	{
		SynchronizedIndexAndBookData(strSpeciesKey, m_pDM_SPECIES);
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		BO_INDEX_TEMP_INSERT(m_pDM_SPECIES, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("I"));	
		//=====================================================
		if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
			m_pDM_SPECIES->AddFrame(strQuery);
		}
	}

	ids = m_pDM_SPECIES->SendFrame();
	if(ids < 0) return -31051;
	ids = m_pDM_SPECIES->EndFrame();
	if(ids < 0) return -31051;

	return 0;
}

INT CBO_SPECIES::BO_SPECIES_RECYCLE(CString strSpeciesKey, 
								   CString strAccuKey,
								   INT nAcqType, 
								   CGroupInfo * pGroup)
{
	// FROM : _T("PURCHASE") or _T("DONATE")
	// MODE : _T("DELETE") or _T("RECYCLE")
	// ���� 1 row delete -> ��Ȱ������ 1 row insert(�÷��� �������ٰ�)
	// ���������� �� ���� 
	// (å)�������� n rows delete -> ��Ȱ��å�� n rows insert(�÷��� �������ٰ�)(��Ȱ���ϰ�� ������ ����)
	
	if (strSpeciesKey.GetLength() == 0) return -31001;	// ��KEY���� 
	INT nRowIdx = 0;

	INT ids;
	CArray <CString, CString> RemoveAliasList;
	BOOL bCreateSpeciesKey = FALSE;

	m_pDM_SPECIES->StartFrame();

	CString strOption, strQuery, strKey, strBookQuery, strTmpData;
	strKey = strSpeciesKey;
	// �ش� ��DM�� �����Ѵ�.
	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = m_pDM_SPECIES->RefreshDataManager(strOption);
	if(ids < 0) return -1;
	ids = m_pDM_IDX_BO->RefreshDataManager(strOption);
	if(ids < 0) return -1;
	// �ش� ���� åDM�� �����Ѵ�.
	if(!strAccuKey.IsEmpty() && strAccuKey != _T("-1") && strAccuKey != _T("ALL"))
		strOption.Format(_T("SPECIES_KEY = %s AND ACQ_KEY = %s AND (WORKING_STATUS<'BOL' OR WORKING_STATUS>'BOR')"), strSpeciesKey, strAccuKey);
	else if(strAccuKey == _T("ALL"))
		strOption.Format(_T("SPECIES_KEY = %s AND (WORKING_STATUS<'BOL' OR WORKING_STATUS>'BOR')"), strSpeciesKey);
	else
		strOption.Format(_T("SPECIES_KEY = %s AND (ACQ_KEY IS NULL OR ACQ_KEY = -1) AND (WORKING_STATUS<'BOL' OR WORKING_STATUS>'BOR')"), strSpeciesKey);
	ids = m_pDM_BOOK->RefreshDataManager(strOption);
	if (ids < 0) return -31003;

	//������ å������ ���ٸ� return
	if(m_pDM_BOOK->GetRowCount() == 0) return 1000;
	
	// EBook������ �����Ѵ�.
	strBookQuery = strOption;
	strOption.Format(_T("BOOK_KEY IN (SELECT REC_KEY FROM BO_BOOK_TBL WHERE %s)"), strBookQuery);
	ids = m_pDM_EBook->RefreshDataManager(strOption);
	// ������ EBook������ DB_FLAG�� _T("D")�� �����Ѵ�.
	ids = m_pDM_EBook->GetRowCount();
	for(INT idx=0 ; idx<ids ; idx++)
	{
		m_pDM_EBook->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}
	// ������ Book������ DB_FLAG�� _T("D")�� �����Ѵ�.
	ids = m_pDM_BOOK->GetRowCount();
	for(idx=0 ; idx<ids ; idx++)
	{
		m_pDM_BOOK->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}
	
	// �������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	//RemoveAliasList.Add(_T("BS_MARC"));
	m_pDM_SPECIES->SetCellData(_T("BS_�����۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
	m_pDM_SPECIES->SetCellData(_T("BS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
	m_pDM_SPECIES->SetCellData(_T("BS_�۾����±���"), _T("2"), nRowIdx);
	//���� �������۾����� ����
	m_pDM_IDX_BO->SetCellData(_T("IBS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		
	if(!CheckHaveOtherAccuInfo(strSpeciesKey, strAccuKey))
	{
		strQuery.Format(_T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
		ids = m_pDM_SPECIES->AddFrame(strQuery);
		m_pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), _T("2"), 0);
		m_pDM_IDX_BO->SetCellData(_T("IBS_������"), GetTodayDate(), 0);
	}
	else
	{
		m_pDM_SPECIES->MakeRecKey(strKey);
		m_pDM_SPECIES->SetCellData(_T("BS_��KEY"), strKey, 0);
		m_pDM_IDX_BO->SetCellData(_T("IBS_REC_KEY"), strKey, 0);
		m_pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), _T("2"), 0);
		m_pDM_IDX_BO->SetCellData(_T("IBS_������"), GetTodayDate(), 0);
		bCreateSpeciesKey = TRUE;
	}
	
	// ����å������ Insert�� Insert���� �����Ѵ�.
	// ����å������ �����Ҷ� �� å�� �ο��Ǿ��� ��Ϲ�ȣ�� CStringArray�� �����Ѵ�.
	INT nRowCount = m_pDM_BOOK->GetRowCount();
	MakeBookQueryFrame(m_pDM_BOOK, strSpeciesKey, strAccuKey, m_pDM_SPECIES);

	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BB_������ȣ"));
	RemoveAliasList.Add(_T("BB_������Ϲ�ȣ"));
	RemoveAliasList.Add(_T("BB_��Ϲ�ȣ"));
	RemoveAliasList.Add(_T("BB_�Ϸù�ȣ"));
	RemoveAliasList.Add(_T("BB_����KEY"));
	
	INT beforeBookCount = m_pDM_BOOK->GetRowCount();
	CONVERT_BOOK_to_VOLUMEInBOOK(m_pDM_BOOK);
	INT afterBookCount = m_pDM_BOOK->GetRowCount();
	for (INT i = 0; i < afterBookCount; i++)
	{
		m_pDM_SPECIES->InitDBFieldData();
		m_pDM_SPECIES->AddDBFieldData(_T("DELETE_DATE"),	_T("STRING"),	GetTodayDate());
		if(pGroup)
		{
			m_pDM_BOOK->SetCellData(_T("BB_�׷��ȣ"),	pGroup->GetGroupNo(), i);
			m_pDM_BOOK->SetCellData(_T("BB_���Գ⵵"),  pGroup->GetAcqYear(), i);
		}
		else
		{
			m_pDM_BOOK->SetCellData(_T("BB_�׷��ȣ"),	_T("0"), i);
			m_pDM_BOOK->SetCellData(_T("BB_���Գ⵵"),  GetTodayYear(), i);
		}
		if(bCreateSpeciesKey) m_pDM_BOOK->SetCellData(_T("BB_��KEY"), strKey, i);
		m_pDM_BOOK->SetCellData(_T("BB_�������۾�����"),pMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_BOOK->SetCellData(_T("BB_�����۾�����"),	pMgr->GetWorkLogMsg(WORK_LOG), i);
		ids = m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_BOOK, _T("BO_DELETE_BOOK_TBL"), i, m_pDM_SPECIES);
		if(ids < 0) return -1;
		strTmpData = m_pDM_BOOK->GetCellData(_T("BB_åKEY"), i);
		SetEBookDMDBFlag(m_pDM_EBook, strTmpData, _T(""));
	}
	
	ids = MakeEBookQueryFrame(m_pDM_EBook, m_pDM_SPECIES);
	if(ids < 0) return ids;
	
	//����/�������� ����
	if(nAcqType == 1 || nAcqType == 2)
	{
		CString strMarc;
		m_pDM_SPECIES->GetCellData(_T("BS_MARC"), 0, strMarc);
		UpdateAccuInfo(m_pDM_SPECIES, m_pDM_BOOK, strAccuKey, strMarc, nAcqType, m_pDM_SPECIES->GetCellData(_T("BS_��KEY"), 0));
		m_pDM_SPECIES->SetCellData(_T("BS_MARC"), strMarc, 0);
		// ��ġ��� ��û �ڷῡ ���Ͽ� ��ġ����, acq_key, acq_code, cancel_reason, furnish_date����
		if(strAccuKey != _T("ALL") && !strAccuKey.IsEmpty())
		{
			strQuery.Format(
				_T("UPDATE BO_FURNISH_TBL SET LAST_WORK='%s', ACQ_KEY=NULL, ACQ_CODE=NULL, CANCEL_REASON='���α׷� ����', FURNISH_STATUS='4', FURNISH_DATE=NULL WHERE ACQ_KEY=%s;"),
				pMgr->GetWorkLogMsg(WORK_LOG), strAccuKey);
			m_pDM_SPECIES->AddFrame(strQuery);
		}
	}
	else if(nAcqType == 0 && !strAccuKey.IsEmpty())
	{
		if(m_pDM_BOOK->GetCellData(_T("BB_���Ա���_�ڵ�"), 0) == _T("1")) nAcqType = 1;
		else if(m_pDM_BOOK->GetCellData(_T("BB_���Ա���_�ڵ�"), 0) == _T("2")) nAcqType = 2;
		CString strMarc;
		m_pDM_SPECIES->GetCellData(_T("BS_MARC"), 0, strMarc);
		UpdateAccuInfo(m_pDM_SPECIES, m_pDM_BOOK, strAccuKey, strMarc, nAcqType, m_pDM_SPECIES->GetCellData(_T("BS_��KEY"), 0));
		m_pDM_SPECIES->SetCellData(_T("BS_MARC"), strMarc, 0);
		// ��ġ��� ��û �ڷῡ ���Ͽ� ��ġ����, acq_key, acq_code, cancel_reason, furnish_date����
		if(strAccuKey != _T("ALL") && !strAccuKey.IsEmpty())
		{
			strQuery.Format(
				_T("UPDATE BO_FURNISH_TBL SET LAST_WORK='%s', ACQ_KEY=NULL, ACQ_CODE=NULL, CANCEL_REASON='���α׷� ����', FURNISH_STATUS='4', FURNISH_DATE=NULL WHERE ACQ_KEY=%s;"),
				pMgr->GetWorkLogMsg(WORK_LOG), strAccuKey);
			m_pDM_SPECIES->AddFrame(strQuery);
		}
	}
	
	CStringArray ArrAlias;
	if(bCreateSpeciesKey)
	{
		ids = SetBookIndexInfo(m_pDM_IDX_BO, m_pDM_BOOK, nRowIdx);
		if(ids < 0) return -1;
		m_pDM_SPECIES->InitDBFieldData();
		m_pDM_SPECIES->AddDBFieldData(_T("DELETE_DATE"),	_T("STRING"),	GetTodayDate());
		ids = m_pDM_SPECIES->MakeQueryByDM(RemoveAliasList, m_pDM_SPECIES, _T("BO_DELETE_SPECIES_TBL"), 0, m_pDM_SPECIES);
		if(ids < 0) return -1;
		m_pDM_SPECIES->InitDBFieldData();
		ids = m_pDM_IDX_BO->MakeQueryByDM(RemoveAliasList, m_pDM_IDX_BO, m_pDM_IDX_BO->TBL_NAME, 0, m_pDM_SPECIES);
		if(ids < 0) return -1;
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		BO_INDEX_TEMP_INSERT(m_pDM_SPECIES, _T("IDX_TMP_BO_TBL"), strKey, _T("I"));
		//=====================================================
		
		if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strKey);
			ids = m_pDM_SPECIES->AddFrame(strQuery);
			if(ids < 0) return -1;
		}
	}
	else
	{
		ids = SetBookIndexInfo(m_pDM_IDX_BO, m_pDM_BOOK, nRowIdx);
		if(ids < 0) return -1;
		m_pDM_SPECIES->InitDBFieldData();
		m_pDM_SPECIES->AddDBFieldData(_T("DELETE_DATE"),	_T("STRING"),	GetTodayDate());
		ids = m_pDM_SPECIES->MakeQueryByDM(RemoveAliasList, m_pDM_SPECIES, _T("BO_DELETE_SPECIES_TBL"), nRowIdx, m_pDM_SPECIES);
		if(ids < 0) return -1;
		RemoveAliasList.RemoveAll();
		RemoveAliasList.Add(_T("IBS_REC_KEY"));
		m_pDM_SPECIES->InitDBFieldData();
		ids = m_pDM_IDX_BO->MakeQueryByDM(RemoveAliasList, m_pDM_IDX_BO, m_pDM_IDX_BO->TBL_NAME, nRowIdx, m_pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
		if(ids < 0) return -1;
	}

	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	BO_INDEX_TEMP_INSERT(m_pDM_SPECIES, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("I"));
	//=====================================================
	
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		ids = m_pDM_SPECIES->AddFrame(strQuery);
		if(ids < 0) return -1;
	}
	
	ids = m_pDM_SPECIES->SendFrame();
	if(ids < 0) return -31051;
	ids = m_pDM_SPECIES->EndFrame();
	if(ids < 0) return -31051;

	return 0;
}

INT CBO_SPECIES::BO_INDEX_TEMP_INSERT(CESL_DataMgr * pDM_OUT, CString strTblName, CString strSpeciesKey, CString strWorkFlag)
{
	if(strSpeciesKey.IsEmpty()) return 0;
	CString strIDXTEMPKEY;
	//pDM_OUT->MakeRecKey(strIDXTEMPKEY);
	pDM_OUT->InitDBFieldData();
	pDM_OUT->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL"));
	if(!strSpeciesKey.IsEmpty())
		pDM_OUT->AddDBFieldData(_T("SPECIES_KEY"), _T("NUMERIC"), strSpeciesKey);
	pDM_OUT->AddDBFieldData(_T("IDX_WORK_FLAG"), _T("STRING"), strWorkFlag);
	pDM_OUT->AddDBFieldData(_T("INPUT_DATE"), _T("STRING"), GetTodayDate());
	INT ids = pDM_OUT->MakeInsertFrame(strTblName);
	if(ids < 0) return -1;
	return 0;
}

INT CBO_SPECIES::BO_SPECIES_INSERT(
								   CESL_DataMgr *pDM_IDX_BO,
								   CESL_DataMgr *pDM_SPECIES,
								   CESL_DataMgr *pDM_BOOK, 
								   CESL_DataMgr *pDM_ACCU, 
								   CESL_DataMgr *pDM_APPENDIX,
								   INT nRowIdx/*=0*/, 
								   INT nSpeciesType)
{
	SPECIES_TYPE speciesType = SPECIES_TYPE_INVALID;
	speciesType = (SPECIES_TYPE)nSpeciesType;
	m_nSpeciesType = nSpeciesType;
	
	// ����-��-������ ����
	// ���� 1 row insert
	// Idx �� 1row insert
	// ���������� 1 row insert
	// (å)�������� n rows insert

	//pDM_SPECIES->StartFrame();

	CArray <CString, CString> RemoveAliasList;
	
	INT ids;
	CString strSpecies_key, strAccu_key, strBook_key, strData = _T("");
	ids = pDM_SPECIES->MakeRecKey(strSpecies_key); 
	if(ids < 0) return -1;
	m_strSPECIES_KEY = strSpecies_key;
	if(speciesType == BO_BASIC || speciesType == BO_REG_INVALID) strAccu_key = _T("-1");
	else 
	{
		ids = pDM_SPECIES->MakeRecKey(strAccu_key); 
		if(ids < 0) return -1;
	}

	// Default ���� ���� ��Ų��.
	SetDefaultValue(nRowIdx, pDM_IDX_BO, pDM_SPECIES, pDM_ACCU, pDM_BOOK);
	//�ηϿ� Default���� �����Ų��.
	if(pDM_APPENDIX)
	{
		SetDefaultValue_BOOK(pDM_APPENDIX, pDM_ACCU, nRowIdx);
		for(INT i=0 ; i<pDM_APPENDIX->GetRowCount() ; i++)
		{
			pDM_APPENDIX->SetCellData(_T("BB_å�η��÷���"), _T("A"), i);
		}
	}

	// ���� Insert�� Insert���� �����Ѵ�.
	INDEX_to_SPECIES(pDM_IDX_BO, nRowIdx, pDM_SPECIES, nRowIdx);		// ������ ������ ���� ������� �ش�.
	//���� ������ �̿����� ���� �ڵ� ����
	CString strUSELIMIT_CODE;
	Get_Priority_USELIMIT_CODE(pDM_BOOK, strUSELIMIT_CODE);
	pDM_SPECIES->SetCellData(_T("BS_�̿����ѱ���_�ڵ�"), strUSELIMIT_CODE, nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_��KEY"), strSpecies_key, nRowIdx);
	pDM_SPECIES->SetCellData(_T("BS_���Գ⵵"), pDM_ACCU->GetCellData(_T("BP_���Գ⵵"), nRowIdx), nRowIdx);
	//�������м���
	pDM_SPECIES->SetCellData(_T("BS_��������_�ڵ�"), pDM_ACCU->GetCellData(_T("BP_��������_�ڵ�"), nRowIdx), nRowIdx);
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("BO_SPECIES_TBL"), nRowIdx, pDM_SPECIES);
	FileLog(pDM_SPECIES->m_lastmsg);

	CMarc Marc;
	CString strStreamMarc;
	pDM_SPECIES->GetCellData(_T("BS_MARC"), nRowIdx, strStreamMarc);
	pMgr->m_pInfo->pMarcMgr->Decoding(strStreamMarc, &Marc);		
	// �˻� �ʵ带 ���ο� �ϼ��Ѵ�.
	ids = APPLY_IndexItem(pDM_IDX_BO, &Marc, nRowIdx);
	
	// ���ο� Insert�� Insert���� �����Ѵ�.
	pDM_IDX_BO->SetCellData(_T("IBS_REC_KEY"), strSpecies_key, nRowIdx);
	pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), _T("0"), nRowIdx);
	//ST_CODE�� �����Ѵ�.
	Set_Index_ST_CODE(_T("IBS_ISBN"), pDM_IDX_BO, pDM_BOOK, -1);
	
	//���� �ӽ� ���̺� ������ �����Ѵ�.
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	BO_INDEX_TEMP_INSERT(pDM_SPECIES, _T("IDX_TMP_BO_TBL"), strSpecies_key, _T("I"));
	//=====================================================
	

	// ���������� Insert�� Insert���� �����Ѵ�.
	pDM_ACCU->SetCellData(_T("BP_��KEY"), strSpecies_key, nRowIdx);
	pDM_ACCU->SetCellData(_T("BP_��������KEY"), strAccu_key, nRowIdx);

	//���Խ�û��/��û�� ���� ó��
	ids = MakeFurnishData(pDM_SPECIES, pDM_IDX_BO, pDM_ACCU);
	if(ids < 0) return -1;
	else if(ids > 0) pDM_ACCU->SetCellData(_T("BP_�Է±���"), _T("2"), 0);
	
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	CString strAccuTblName = pDM_ACCU->TBL_NAME;
	if(!strAccuTblName.IsEmpty())
	{
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_ACCU, pDM_ACCU->TBL_NAME, nRowIdx, pDM_SPECIES);
		FileLog(pDM_SPECIES->m_lastmsg);
	}

	// å������ Insert�� Insert���� �����Ѵ�.
	INT nBookRow = pDM_BOOK->GetRowCount();
	MakeBookQueryFrame(pDM_BOOK, strSpecies_key, strAccu_key, pDM_SPECIES);

	ids = MakeEBookInfoAndQueryFrame(pDM_BOOK, m_pDM_EBook, pDM_SPECIES);
	if(ids < 0) return -33050;

	// �η������� Insert�� Insert���� �����Ѵ�.
	if(pDM_APPENDIX != NULL)
		MakeBookQueryFrame(pDM_APPENDIX, strSpecies_key, strAccu_key, pDM_SPECIES);

	// ���������� Insert�� Insert���� �����Ѵ�.
	ids = SetBookIndexInfo(pDM_IDX_BO, pDM_BOOK, nRowIdx);
	if(ids < 0) return -1;
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX_BO, _T("IDX_BO_TBL"), nRowIdx, pDM_SPECIES);
	FileLog(pDM_SPECIES->m_lastmsg);

	//ECOSEARCH�� ���ؼ� �������
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strData.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpecies_key);
		pDM_SPECIES->AddFrame(strData);
	}

	ids = pDM_SPECIES->SendFrame();
	if(ids < 0) return -33051;
	ids = pDM_SPECIES->EndFrame();
	if(ids < 0) return -33051;

	return 0;		
}

INT CBO_SPECIES::BO_SPECIES_UPDATE(
								   CESL_DataMgr *pDM_IDX_BO,
								   CESL_DataMgr *pDM_SPECIES,
								   CESL_DataMgr *pDM_BOOK,
								   CESL_DataMgr *pDM_ACCU, 
								   CESL_DataMgr *pDM_APPENDIX,
								   INT nRowIdx,
								   INT nSpeciesType)
{
	// ����-��-����-������ ����
	// ���� 1 row update
	// ���������� 1 row update
	// (å)�������� n rows update
	SPECIES_TYPE speciesType = SPECIES_TYPE_INVALID;
	speciesType = (SPECIES_TYPE)nSpeciesType;
	m_nSpeciesType = nSpeciesType;

	CArray <CString, CString> RemoveAliasList;
	
	INT ids;
	CString strSpecies_key, strPurchaseinfo_key, strBook_key, strData;
	ids = pDM_SPECIES->GetCellData(_T("BS_��KEY"), nRowIdx, strSpecies_key);
	ids = pDM_ACCU->GetCellData(_T("BP_��������KEY"), nRowIdx, strPurchaseinfo_key);
	if(strPurchaseinfo_key.IsEmpty()) strPurchaseinfo_key = _T("-1");

	// Default ���� ���� ��Ų��.
	SetDefaultValue(nRowIdx, pDM_IDX_BO, pDM_SPECIES, pDM_ACCU, pDM_BOOK);
	
	// ���� Update���� �����Ѵ�.
	INDEX_to_SPECIES(pDM_IDX_BO, nRowIdx, pDM_SPECIES, nRowIdx);		// ������ ������ ���� ������� �ش�.
	//���� ������ �̿����� ���� �ڵ� ����
	CString strUSELIMIT_CODE;
	pDM_SPECIES->GetCellData(_T("BS_�̿����ѱ���_�ڵ�"), nRowIdx, strUSELIMIT_CODE);
	Get_Priority_USELIMIT_CODE(pDM_BOOK, strUSELIMIT_CODE, 1, strUSELIMIT_CODE);
	pDM_SPECIES->SetCellData(_T("BS_�̿����ѱ���_�ڵ�"), strUSELIMIT_CODE, nRowIdx);
	//�������м���
	pDM_SPECIES->SetCellData(_T("BS_��������_�ڵ�"), pDM_ACCU->GetCellData(_T("BP_��������_�ڵ�"), nRowIdx), nRowIdx);
	RemoveAliasList.RemoveAll();
	//RemoveAliasList.Add(_T("BS_MARC"));
	RemoveAliasList.Add(_T("BS_��KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("BO_SPECIES_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);

	// �˻� �ʵ带 ���ο� �ϼ��Ѵ�.
	CMarc *pMarc = new CMarc;
	CString strStreamMarc;
	pDM_SPECIES->GetCellData(_T("BS_MARC"), nRowIdx, strStreamMarc);
	pMgr->m_pInfo->pMarcMgr->Decoding(strStreamMarc, pMarc);		
	// �˻� �ʵ带 ���ο� �ϼ��Ѵ�.
	ids = APPLY_IndexItem(pDM_IDX_BO, pMarc, nRowIdx);
	if (pMarc) {
		delete pMarc;
		pMarc = NULL;
	}
	//ST_CODE�� �����Ѵ�.
	Set_Index_ST_CODE(_T("IBS_ISBN"), pDM_IDX_BO, pDM_BOOK, -1);
		
	// ���������� Update���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BP_��������KEY"));
	pDM_SPECIES->InitDBFieldData();
	CString strAccuTblName = pDM_ACCU->TBL_NAME;
	if(!strAccuTblName.IsEmpty())
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_ACCU, pDM_ACCU->TBL_NAME, nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strPurchaseinfo_key);

	// å������ Insert/Update/Delete Ȥ�� ����å������ Insert���� �����Ѵ�. -> �׸��� DB�� SendFrame�Ѵ�.
	SetDefaultValue_BOOK(pDM_BOOK, pDM_ACCU, 0) ;
	ids = MakeBookQueryFrame(pDM_BOOK, strSpecies_key, strPurchaseinfo_key, pDM_SPECIES);
	if (ids < 0) return -34052;

	ids = MakeEBookInfoAndQueryFrame(pDM_BOOK, m_pDM_EBook, pDM_SPECIES);
	if(ids < 0) return -33050;

	// �η������� Insert�� Insert���� �����Ѵ�.
	if(pDM_APPENDIX != NULL)
	{
		for(INT i=0 ; i<pDM_APPENDIX->GetRowCount() ; i++)
		{
			pDM_APPENDIX->SetCellData(_T("BB_å�η��÷���"), _T("A"), i);
		}
		MakeBookQueryFrame(pDM_APPENDIX, strSpecies_key, strPurchaseinfo_key, pDM_SPECIES);
	}
	
	// ���ο� Update���� �����Ѵ�.
	//�̿����ѱ���/��������/�ڷ��/��ϱ��� ���а��� ���а� concatenation
	ids = SetBookIndexInfo(pDM_IDX_BO, pDM_BOOK, nRowIdx);
	if(ids < 0) return -1;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("IBS_REC_KEY"));
	pDM_SPECIES->InitDBFieldData();
	ids = pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX_BO, _T("IDX_BO_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);
	if(ids < 0) return -33050;

	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	BO_INDEX_TEMP_INSERT(pDM_SPECIES, _T("IDX_TMP_BO_TBL"), strSpecies_key, _T("I"));
	//=====================================================
	

	//ECOSEARCH�� ���ؼ� �������
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		CString strQuery;
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpecies_key);
		pDM_SPECIES->AddFrame(strQuery);
	}

	// �� �Է� ������ �ϳ��� ������(Ʈ������)���� ����� �Ѵ�.
/*
	ids = UpdateSpeciesMarcbyStream(pDM_SPECIES, _T("BO_SPECIES_TBL"), strSpecies_key, nRowIdx);
	if(ids < 0) return -33053;
*/
	ids = pDM_SPECIES->SendFrame();
	if(ids < 0) return -33051;
	ids = pDM_SPECIES->EndFrame();
	if(ids < 0) return -33051;
	

/*
	INT idsQuery, idsMarc;
	// Query ���� Send Frame
	idsQuery = SendFrame(pDM_SPECIES, MAX_TRANSACTION_QUERY_COUNT);
	pDM_SPECIES->EndFrame();
	if (idsQuery < 0) return -34051;		// Species Insert Error

	// Marc ���� Send Frame
	idsMarc = UpdateSpeciesMarcbyStream(pDM_SPECIES, _T("BO_SPECIES_TBL"), strSpecies_key, nRowIdx);
	pDM_SPECIES->EndFrame();
	if (idsMarc < 0) return -34053;		// Marc Stream Update Error
*/

	return 0;
}

BOOL CBO_SPECIES::CheckHaveOtherAccuInfo(CString strSpeciesKey, CString strAccuKey)
{
	CString strQuery, strTmpData;
	if(strAccuKey.IsEmpty())
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE ")
			_T("(SPECIES_KEY=%s AND ACQ_KEY IS NOT NULL AND ACQ_KEY != -1) ")
			_T("OR (SPECIES_KEY=%s AND WORKING_STATUS>'BOL' AND WORKING_STATUS<'BOR')"),
			strSpeciesKey, strSpeciesKey);
	}
	else if(strAccuKey == _T("ALL"))
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE ")
			_T("SPECIES_KEY = %s AND WORKING_STATUS>'BOL' AND WORKING_STATUS<'BOR'"),
			strSpeciesKey);
	}
	else
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE ")
			_T("(SPECIES_KEY = %s AND (ACQ_KEY IS NULL OR ACQ_KEY != %s)) ")
			_T("OR (SPECIES_KEY=%s AND WORKING_STATUS>'BOL' AND WORKING_STATUS<'BOR')"),
			strSpeciesKey, strAccuKey, strSpeciesKey);
	}
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pDM_SPECIES->CONNECTION_INFO);
	tmpDM.RestructDataManager(strQuery);
	strTmpData = tmpDM.GetCellData(0, 0);
	INT nTmpData = _ttoi(strTmpData.GetBuffer(0));
	if(nTmpData > 0)
		return TRUE;
	else
		return FALSE;
	
	return FALSE;
}

INT CBO_SPECIES::BO_SPECIES_ACQOBJ_PURCHASE(CESL_DataMgr *pDM_OUT, 
											CGroupInfo *pGroupInfo,
											CString strSpeciesKey,
											CString strWorkingStatus, 
											CESL_DataMgr * pParentDM, 
											CESL_DataMgr * pParentBookDM,
											CESL_Grid * pParentGrid)
{
	m_pDM_ACCU->FreeData();
	//���� DM����
	
	CString strQuery, strTmpData, strRecKey;
	strQuery.Format(_T("REC_KEY=%s"), strSpeciesKey);
	m_pDM_IDX_BO->RefreshDataManager(strQuery);
	m_pDM_SPECIES->RefreshDataManager(strQuery);
	//å ����
	if(strWorkingStatus.IsEmpty())
		strQuery.Format(_T("SPECIES_KEY = %s AND BOOK_APPENDIX_FLAG = 'B' AND (WORKING_STATUS = 'BOT111O' OR WORKING_STATUS = 'BOT112O')"), strSpeciesKey);
	else
		strQuery.Format(_T("SPECIES_KEY = %s AND BOOK_APPENDIX_FLAG = 'B' AND WORKING_STATUS = '%s'"), 
			strSpeciesKey, strWorkingStatus);
		
	m_pDM_BOOK->RefreshDataManager(strQuery);

	if(strWorkingStatus == _T("BOT111O")) strTmpData = _T("0");
	else if(strWorkingStatus == _T("BOT112O")) strTmpData = _T("3");
	else strTmpData.Empty();
	
	if(pGroupInfo)
	{
		m_pDM_SPECIES->SetCellData(_T("BS_��������"), pGroupInfo->GetWorkCode(), 0);
		m_pDM_IDX_BO->SetCellData(_T("IBS_��������_�ڵ�"), pGroupInfo->GetWorkCode(), 0);
	}

	INT ids = MakeAccuInfo(pDM_OUT, m_pDM_ACCU, m_pDM_BOOK, m_pDM_SPECIES, strSpeciesKey, pGroupInfo, TRUE, strTmpData);
	if(ids < 0) return -1;
	strRecKey = m_pDM_ACCU->GetCellData(_T("BP_��������KEY"), 0);

	INT nBookCnt = m_pDM_BOOK->GetRowCount();

	for(INT i=0 ; i<nBookCnt ; i++)
	{
		m_pDM_BOOK->SetCellData(_T("BB_�ڷ����"), _T("BOA111N"), i);
		m_pDM_BOOK->SetCellData(_T("BB_��������KEY"), strRecKey, i);
		m_pDM_BOOK->SetCellData(_T("BB_���Ա���_�ڵ�"), _T("1"), i);
		m_pDM_BOOK->SetCellData(_T("BB_�����ڷ����"), strWorkingStatus, i);
		m_pDM_BOOK->SetCellData(_T("BB_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_BOOK->SetCellData(_T("BB_DB_FLAG"), _T("U"), i);
		m_pDM_BOOK->SetCellData(_T("BB_�Ϸù�ȣ"), m_pDM_ACCU->GetCellData(_T("BP_�����Ϸù�ȣ"), 0), i);
		if(pGroupInfo)
		{
			m_pDM_BOOK->SetCellData(_T("BB_�׷��ȣ"), pGroupInfo->GetGroupInfo(), i);
			m_pDM_BOOK->SetCellData(_T("BB_���Գ⵵"), pGroupInfo->GetAcqYear(), i);
		}
		else
		{
			m_pDM_BOOK->SetCellData(_T("BB_�׷��ȣ"), _T(""), i);
		}
	}
		
	MakeBookQueryFrame(m_pDM_BOOK, strSpeciesKey, _T(""), pDM_OUT);

	//���� �ڷ���� ����
	CString strIdxWorkingStatus = m_pDM_IDX_BO->GetCellData(_T("IBS_�۾�����INFO"), 0);
	if(GetWorkingStatusPriority(_T("BOA111N")) >= GetWorkingStatusPriority(strIdxWorkingStatus)) strIdxWorkingStatus = _T("BOA111N");
	if(pGroupInfo)
	{
		strQuery.Format(_T("UPDATE IDX_BO_TBL SET WORK_CODE='%s', WORKING_STATUS='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"), 
			pGroupInfo->GetWorkCode(), strIdxWorkingStatus, pMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET WORK_CODE='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
			pGroupInfo->GetWorkCode(), pMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
		pDM_OUT->AddFrame(strQuery);
	}
	else
	{
		strQuery.Format(_T("UPDATE IDX_BO_TBL SET WORKING_STATUS='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"), 
			strIdxWorkingStatus, pMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET LAST_WORK='%s' WHERE REC_KEY=%s;"),
			pMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
		pDM_OUT->AddFrame(strQuery);
	}
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	BO_INDEX_TEMP_INSERT(pDM_OUT, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("I"));
	//=====================================================
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		pDM_OUT->AddFrame(strQuery);
	}
	if(pParentDM != NULL && pParentGrid != NULL && pParentBookDM)
	{
		APPLY_PARENT_DM(m_pDM_IDX_BO, m_pDM_SPECIES, m_pDM_ACCU, m_pDM_BOOK, pParentBookDM, pParentGrid, -1, 0, BOOK, FALSE, pGroupInfo);
		APPLY_PARENT_DM(m_pDM_IDX_BO, m_pDM_SPECIES, m_pDM_ACCU, m_pDM_BOOK, pParentDM, pParentGrid, -1, 0, SPECIES, FALSE, pGroupInfo);
	}
	
	return 0;	
}

INT CBO_SPECIES::BO_SPECIES_ACQOBJ_DONATE(CESL_DataMgr *pDM_OUT,
										  CGroupInfo *pGroupInfo,
										  CString strSpeciesKey,
										  CString strWorkingStatus, 
										  CESL_DataMgr * pParentDM, 
										  CESL_DataMgr * pParentBookDM,
										  CESL_Grid * pParentGrid)
{
	m_pDM_ACCU->FreeData();
	m_pDM_BOOK->FreeData();
	m_pDM_IDX_BO->FreeData();
	//���� DM����
	CString strQuery, strRecKey, strTmpData;
	strQuery.Format(_T("REC_KEY=%s"), strSpeciesKey);
	m_pDM_IDX_BO->RefreshDataManager(strQuery);
	m_pDM_SPECIES->RefreshDataManager(strQuery);
	//å ����
	if(strWorkingStatus.IsEmpty())
		strQuery.Format(_T("SPECIES_KEY = %s AND BOOK_APPENDIX_FLAG = 'B' AND (WORKING_STATUS = 'BOT111O' OR WORKING_STATUS = 'BOT112O')"), 
			strSpeciesKey);
	else
		strQuery.Format(_T("SPECIES_KEY = %s AND BOOK_APPENDIX_FLAG = 'B' AND WORKING_STATUS = '%s'"), 
			strSpeciesKey, strWorkingStatus);
	
	m_pDM_BOOK->RefreshDataManager(strQuery);

	if(strWorkingStatus == _T("BOT111O")) strTmpData = _T("0");
	else if(strWorkingStatus == _T("BOT112O")) strTmpData = _T("3");
	else strTmpData.Empty();
	
	INT ids = MakeAccuInfo(pDM_OUT, m_pDM_ACCU, m_pDM_BOOK, m_pDM_SPECIES, strSpeciesKey, pGroupInfo, FALSE, strTmpData);
	if(ids < 0) return -1;

	INT nBookCnt = m_pDM_BOOK->GetRowCount();
	INT nSerialNo = pGroupInfo->GetLastSerialNo();

	for(INT i=0 ; i<nBookCnt ; i++)
	{
		m_pDM_BOOK->SetCellData(_T("BB_�ڷ����"), _T("BOA211O"), i);
		m_pDM_BOOK->SetCellData(_T("BB_���Գ⵵"), pGroupInfo->GetAcqYear(), i);
		m_pDM_BOOK->SetCellData(_T("BB_��������KEY"), m_pDM_ACCU->GetCellData(_T("BP_��������KEY"), 0), i);
		m_pDM_BOOK->SetCellData(_T("BB_���Ա���_�ڵ�"), _T("2"), i);
		m_pDM_BOOK->SetCellData(_T("BB_�׷��ȣ"), pGroupInfo->GetGroupInfo(), i);
		strTmpData.Format(_T("%d"), ++nSerialNo);
		m_pDM_BOOK->SetCellData(_T("BB_�Ϸù�ȣ"), strTmpData, i);
		m_pDM_BOOK->SetCellData(_T("BB_�����ڷ����"), strWorkingStatus, i);
		m_pDM_BOOK->SetCellData(_T("BB_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_BOOK->SetCellData(_T("BB_DB_FLAG"), _T("U"), i);
	}
		
	MakeBookQueryFrame(m_pDM_BOOK, strSpeciesKey, _T(""), pDM_OUT);
	pGroupInfo->SetLastSerialNo(nSerialNo);
	
	//���� �ڷ���� ����
	CString strIdxWorkingStatus = m_pDM_IDX_BO->GetCellData(_T("IBS_�۾�����INFO"), 0);

	if(GetWorkingStatusPriority(_T("BOA211O")) >= GetWorkingStatusPriority(strIdxWorkingStatus)) strIdxWorkingStatus = _T("BOA211O");
	strQuery.Format(_T("UPDATE IDX_BO_TBL SET WORKING_STATUS='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"), 
		strIdxWorkingStatus, pMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
	pDM_OUT->AddFrame(strQuery);
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	BO_INDEX_TEMP_INSERT(pDM_OUT, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("I"));
	//=====================================================
	
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		pDM_OUT->AddFrame(strQuery);
	}
	if(pParentDM != NULL && pParentGrid != NULL)
	{
		APPLY_PARENT_DM(m_pDM_IDX_BO, m_pDM_SPECIES, m_pDM_ACCU, m_pDM_BOOK, pParentBookDM, pParentGrid, -1, 0, BOOK, FALSE, pGroupInfo);
		APPLY_PARENT_DM(m_pDM_IDX_BO, m_pDM_SPECIES, m_pDM_ACCU, m_pDM_BOOK, pParentDM, pParentGrid, -1, 0, SPECIES, FALSE, pGroupInfo);
	}
	return 0;	
}

INT CBO_SPECIES::SetAccuDM(BOOL isPurchase)
{
	CString strDMAlias, strDMPK;
	if(isPurchase) strDMAlias = _T("DMUP_����_��_��������");
	else strDMAlias = _T("DMUP_����_��_��������");
	
	if(m_pDM_ACCU != NULL) 
	{
		delete m_pDM_ACCU;
		m_pDM_ACCU = NULL;
	}
	m_pDM_ACCU = new CESL_DataMgr;
	m_pDM_ACCU->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
	m_pDM_ACCU->InitDMPKFromFile(strDMAlias);
	m_pDM_ACCU->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	return 0;
}

INT CBO_SPECIES::GetVolumeCount(CESL_DataMgr *pDM_Book)
{
	if (pDM_Book == NULL) return -1;

	INT ids = -1;

	INT nRowCount = pDM_Book->GetRowCount();

	CString strVol;
	CMap<CString, LPCTSTR, INT, INT> mapVolume;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx)) continue;

		ids = pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), idx, strVol);
		
		mapVolume.SetAt(strVol, 1);
	}

	return mapVolume.GetCount();
}

INT CBO_SPECIES::BO_FURNISH_DATA_PROCESS(CString strTitle, 
										 CString strAuthor, 
										 CString strPublisher, 
										 CString strPublishYear, 
										 CString strApplicantName, 
										 CString strPrice, 
										 CString strFurnishKey, 
										 CString strPrevAcqKey, 
										 CString strPrevAcqCode, 
										 CGroupInfo * pGroupInfo, 
										 BOOL isPurchase,
										 CESL_DataMgr * pParentDM,
										 CESL_DataMgr * pParentBookDM,
										 CESL_Grid * pParentGrid
										 )
{
	INT ids;
	INT RowIdx = 0;
	CString strTmpData;
	//DM�ʱ�ȭ
	m_pDM_SPECIES->FreeData();
	m_pDM_IDX_BO->FreeData();
	m_pDM_ACCU->FreeData();
	m_pDM_BOOK->FreeData();

	//�������� �ʱ�ȭ 
	if(strPrice.IsEmpty()) strPrice = _T("0");
		
	// ������ ����
	// ���� ���� ����
	// å���� ����
	// �������� ����
	m_pDM_ACCU->InsertRow(-1);
	m_pDM_BOOK->InsertRow(-1);
	m_pDM_IDX_BO->InsertRow(-1);
	m_pDM_SPECIES->InsertRow(-1);
	
	if(pGroupInfo)
		m_pDM_SPECIES->SetCellData(_T("BS_���Գ⵵"), pGroupInfo->GetAcqYear(), RowIdx);
	else
		m_pDM_SPECIES->SetCellData(_T("BS_���Գ⵵"), GetTodayYear(), RowIdx);

	pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("�����ڷᱸ��"), strTmpData);
	m_pDM_SPECIES->SetCellData(_T("BS_�����ڷᱸ��"), strTmpData, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_�����ڷᱸ��"), strTmpData, RowIdx);
	strTmpData.Empty();
	pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("���ı���"), strTmpData);
	if(strTmpData.IsEmpty()) strTmpData = _T("BK");
	m_pDM_SPECIES->SetCellData(_T("BS_���ı���_�ڵ�"), strTmpData, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_���ı���_�ڵ�"), strTmpData, RowIdx);
	strTmpData.Empty();
	pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("�ڷᱸ��"), strTmpData);
	m_pDM_SPECIES->SetCellData(_T("BS_�ڷᱸ��_�ڵ�"), strTmpData, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_�ڷᱸ��_�ڵ�"), strTmpData, RowIdx);
	strTmpData.Empty();
	pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("������ϱ���"), strTmpData);
	m_pDM_SPECIES->SetCellData(_T("BS_������ϱ���_�ڵ�"), strTmpData, RowIdx);	
	m_pDM_IDX_BO->SetCellData(_T("IBS_������ϱ���_�ڵ�"), strTmpData, RowIdx);
	strTmpData.Empty();
	pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("��������"), strTmpData);
	m_pDM_SPECIES->SetCellData(_T("BS_��������"), strTmpData, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_��������_�ڵ�"), strTmpData, RowIdx);
	strTmpData.Empty();
	pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("��ü����"), strTmpData);
	m_pDM_IDX_BO->SetCellData(_T("IBS_��ü����_�ڵ�"), strTmpData, RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_��ü����_�ڵ�"), strTmpData, RowIdx);
	strTmpData.Empty();
	pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("�̿����ѱ���"), strTmpData);
	m_pDM_SPECIES->SetCellData(_T("BS_�̿����ѱ���_�ڵ�"), strTmpData, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_�̿����ѱ���_�ڵ�"), strTmpData, RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_�̿����ѱ���_�ڵ�"), strTmpData, RowIdx);
	strTmpData.Empty();
	pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("�̿��󱸺�"), strTmpData);
	m_pDM_SPECIES->SetCellData(_T("BS_�̿��󱸺�_�ڵ�"), strTmpData, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_�̿��󱸺�_�ڵ�"), strTmpData, RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_�̿��󱸺�_�ڵ�"), strTmpData, RowIdx);
	strTmpData.Empty();
	pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("�з�ǥ����"), strTmpData);
	m_pDM_SPECIES->SetCellData(_T("BS_�з�ǥ����"), strTmpData, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_�з�ǥ����"), strTmpData, RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_�з�ǥ����"), strTmpData, RowIdx);
	if(strTmpData == _T("1"))
	{
		pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("KDC��������ȣ����"), strTmpData);
		m_pDM_SPECIES->SetCellData(_T("BS_��������_�ڵ�"), strTmpData, RowIdx);	
		m_pDM_ACCU->SetCellData(_T("BP_��������_�ڵ�"), strTmpData, RowIdx);
		strTmpData.Empty();
	}
	else if(strTmpData == _T("2"))
	{
		pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("DDC��������ȣ����"), strTmpData);
		m_pDM_SPECIES->SetCellData(_T("BS_��������_�ڵ�"), strTmpData, RowIdx);	
		m_pDM_ACCU->SetCellData(_T("BP_��������_�ڵ�"), strTmpData, RowIdx);
		strTmpData.Empty();
	}
	pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("��ϱ���"), strTmpData);
	if(strTmpData.IsEmpty()) strTmpData = _T("EM");
	m_pDM_IDX_BO->SetCellData(_T("IBS_��ϱ���_�ڵ�"), strTmpData, RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_��ϱ���_�ڵ�"), strTmpData, RowIdx);
	strTmpData.Empty();
	pMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("��������"), strTmpData);
	m_pDM_IDX_BO->SetCellData(_T("IBS_��������_�ڵ�"), strTmpData, RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_��������_�ڵ�"), strTmpData, RowIdx);
	strTmpData.Empty();

	//����������
	m_pDM_SPECIES->SetCellData(_T("BS_�۾���"), pMgr->GetUserID(), RowIdx);
	m_pDM_SPECIES->SetCellData(_T("BS_�۾����±���"), _T("1"), RowIdx);
	m_pDM_SPECIES->SetCellData(_T("BS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), RowIdx);
	m_pDM_SPECIES->SetCellData(_T("BS_�����۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), RowIdx);
	m_pDM_SPECIES->SetCellData(_T("BS_�Է���"), GetTodayDate(), RowIdx);

	//������������	
	m_pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), _T("0"), RowIdx);				// species_class
	m_pDM_IDX_BO->SetCellData(_T("IBS_������"), strAuthor, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_�˻���������"), strAuthor, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_�˻��������"), strPublisher, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_�˻��뺻ǥ��"), strTitle, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_�����"), strPublishYear, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_������"), strPublisher, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_��ǥ��"), strTitle, RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_ǥ������"), strTitle, RowIdx);
	if(isPurchase)
		m_pDM_IDX_BO->SetCellData(_T("IBS_�۾�����INFO"), _T("BOA111N"), RowIdx);
	else
		m_pDM_IDX_BO->SetCellData(_T("IBS_�۾�����INFO"), _T("BOA211O"), RowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_�۾���"), pMgr->GetUserID(), RowIdx);
	//å��������
	m_pDM_BOOK->SetCellData(_T("BB_���Խ�û��"), strApplicantName, RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_������"), strAuthor, RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_å�η��÷���"), _T("B"), RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_��ǰå��"), _T("1"), RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_��ǰ����"), _T("Y"), RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_�Է���"), GetTodayDate(), RowIdx);	
	m_pDM_BOOK->SetCellData(_T("BB_�����"), strPublishYear, RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_���å��"), _T("0"), RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_������ηϸ�"), _T(""), RowIdx);
	if(isPurchase)
	{
		m_pDM_BOOK->SetCellData(_T("BB_�ڷ����"), _T("BOA111N"), RowIdx);
		m_pDM_BOOK->SetCellData(_T("BB_���Ա���_�ڵ�"), _T("1"), RowIdx);
	}
	else
	{
		m_pDM_BOOK->SetCellData(_T("BB_�ڷ����"), _T("BOA211O"), RowIdx);
		m_pDM_BOOK->SetCellData(_T("BB_���Ա���_�ڵ�"), _T("2"), RowIdx);
	}
	m_pDM_BOOK->SetCellData(_T("BB_��ǥå����"), _T("Y"), RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_DB_FLAG"), _T("I"), RowIdx);
	if(pGroupInfo)
	{
		m_pDM_BOOK->SetCellData(_T("BB_���Գ⵵"), pGroupInfo->GetAcqYear(), RowIdx);
		m_pDM_BOOK->SetCellData(_T("BB_�׷��ȣ"), pGroupInfo->GetGroupInfo(), RowIdx);
	}
	else
	{
		m_pDM_BOOK->SetCellData(_T("BB_���Գ⵵"), GetTodayYear(), RowIdx);
		m_pDM_BOOK->SetCellData(_T("BB_�׷��ȣ"), _T(""), RowIdx);
	}
	m_pDM_BOOK->SetCellData(_T("BB_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_�����۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), RowIdx);
	m_pDM_BOOK->SetCellData(_T("BB_����"), strPrice, RowIdx);
	
	INT nTmpData;
	if(!isPurchase)
	{
		if(pGroupInfo == NULL) return -1;

		m_pDM_ACCU->SetCellData(_T("BP_����å��"), _T("1"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_���Գ⵵"), pGroupInfo->GetAcqYear(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�׷��ȣ"), pGroupInfo->GetGroupInfo(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�ѱǼ�"), _T("1"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�������۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�����۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�Է�����"), GetTodayDate(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�˼���"), GetTodayDate(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�Է±���"), _T("2"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_����"), strPrice, 0);
		//�����ڷ� å �Ϸù�ȣ����
		nTmpData = pGroupInfo->GetLastSerialNo();
		if(nTmpData < 0) nTmpData = 0;
		else nTmpData++;
		strTmpData.Format(_T("%d"), nTmpData);
		m_pDM_BOOK->SetCellData(_T("BB_�Ϸù�ȣ"), strTmpData, 0);
		pGroupInfo->SetLastSerialNo(nTmpData);
	}
	else
	{
		m_pDM_ACCU->SetCellData(_T("BP_����å��"), _T("1"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�ֹ�����Ÿ��"), _T("������"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�ѱǼ�"), _T("1"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�������۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�����۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�Է�����"), GetTodayDate(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�Է±���"), _T("2"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_����"), strPrice, 0);
		m_pDM_ACCU->SetCellData(_T("BP_���ְ���"), strPrice, 0);
		m_pDM_ACCU->SetCellData(_T("BP_������"), _T("0"), 0);

		if(pGroupInfo)
		{
			m_pDM_ACCU->SetCellData(_T("BP_���Գ⵵"), pGroupInfo->GetAcqYear(), 0);
			m_pDM_ACCU->SetCellData(_T("BP_�׷��ȣ"), pGroupInfo->GetGroupInfo(), 0);
			strTmpData = pGroupInfo->GetWorkNo();
			if(strTmpData == _T("������")) strTmpData = _T("0");
			m_pDM_ACCU->SetCellData(_T("BP_�۾���ȣ"), strTmpData, 0);
			INT nSerialNo = pGroupInfo->GetLastSerialNo();
			strTmpData.Format(_T("%d"), nSerialNo+1);
			pGroupInfo->SetLastSerialNo(nSerialNo+1);
			m_pDM_ACCU->SetCellData(_T("BP_�����Ϸù�ȣ"), strTmpData, 0);
			m_pDM_BOOK->SetCellData(_T("BB_�Ϸù�ȣ"), strTmpData, 0);
			m_pDM_IDX_BO->SetCellData(_T("IBS_��������_�ڵ�"), pGroupInfo->GetWorkCode(), 0);
			m_pDM_SPECIES->SetCellData(_T("BS_��������"), pGroupInfo->GetWorkCode(), 0);
		}
		else
		{
			m_pDM_ACCU->SetCellData(_T("BP_���Գ⵵"), GetTodayYear(), 0);
			m_pDM_ACCU->SetCellData(_T("BP_�׷��ȣ"), _T(""), 0);
			m_pDM_ACCU->SetCellData(_T("BP_�۾���ȣ"), _T(""), 0);
			m_pDM_ACCU->SetCellData(_T("BP_�����Ϸù�ȣ"), _T(""), 0);
		}
	}
		
	
	//����� �⺻������ 300(���»���)Tag������ å�� �ݿ��Ѵ�.
	CMarcDefaultValueMgr mgr(pMgr);
	ids = mgr.ApplyBookCollationByDefaultValue(m_pDM_IDX_BO->GetCellData(_T("IBS_���ı���_�ڵ�"), 0), m_pDM_IDX_BO, m_pDM_BOOK);
	
	// ��ũ�� �������ش�.
	CMarc marc;
	ids = APPLY_Species_to_CMarc(&marc, m_pDM_IDX_BO, m_pDM_SPECIES, m_pDM_BOOK, m_pDM_ACCU);
	pMgr->m_pInfo->pMarcMgr->CheckDeepSyntax(&marc);
	pMgr->m_pInfo->pMarcMgr->CheckSimpleSyntax(&marc);
	pMgr->m_pInfo->pMarcMgr->DeleteEmptySubfield(&marc);
	pMgr->m_pInfo->pMarcMgr->DeleteEmptyField(&marc);
	CString strStreamMarc;
	ids = pMgr->m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
	m_pDM_SPECIES->SetCellData(_T("BS_MARC"), strStreamMarc, RowIdx);
	
	INT nSpeciesType;
	if(isPurchase) nSpeciesType = (INT)BO_PURCHASE;
	else nSpeciesType = (INT)BO_DONATE;

	m_pDM_SPECIES->StartFrame();
	ids = BO_SPECIES_INSERT(m_pDM_IDX_BO, m_pDM_SPECIES, m_pDM_BOOK, m_pDM_ACCU, NULL, 0, nSpeciesType);
	if(ids < 0) return ids;
	
	//StartFrame
	m_pDM_BOOK->StartFrame();

	CString strQuery, strAcqCode, strAcqKey, strTableName;
	if(isPurchase) strAcqCode = _T("1");
	else strAcqCode = _T("2");
	strAcqKey = m_pDM_ACCU->GetCellData(_T("BP_��������KEY"), 0);
	strQuery.Format(
		_T("UPDATE BO_FURNISH_TBL SET FURNISH_STATUS='2', ACQ_KEY=%s, ACQ_CODE='%s', LAST_WORK='%s', CANCEL_REASON=NULL, FURNISH_DATE=NULL WHERE REC_KEY=%s;"),
		m_pDM_ACCU->GetCellData(_T("BP_��������KEY"), 0), strAcqCode, pMgr->GetWorkLogMsg(WORK_LOG), strFurnishKey);
	m_pDM_BOOK->AddFrame(strQuery);
	if(strPrevAcqCode == _T("1")) strTableName = _T("BO_PURCHASEINFO_TBL");
	else if(strPrevAcqCode == _T("2")) strTableName = _T("BO_DONINFO_TBL");
	if(!strTableName.IsEmpty() && !strPrevAcqKey.IsEmpty())
	{
		strQuery.Format(
			_T("UPDATE %s SET LAST_WORK='%s', INPUT_TYPE='0' WHERE REC_KEY=%s;"), 
			strTableName, pMgr->GetWorkLogMsg(WORK_LOG), strPrevAcqKey);
		m_pDM_BOOK->AddFrame(strQuery);
	}

	if(pGroupInfo && isPurchase)
	{
		pGroupInfo->UpDateLastSerialNo(pMgr, m_pDM_BOOK);
	}
	else if(pGroupInfo && !isPurchase)
	{
		pGroupInfo->UpdateLastSerialNo(_T("2"), pMgr, m_pDM_BOOK);
	}
	
	//ids = SendFrame(m_pDM_BOOK, MAX_TRANSACTION_QUERY_COUNT); 
	ids = m_pDM_BOOK->SendFrame();
	if(ids < 0) return ids;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return ids;
	
	if(pParentDM != NULL && pParentGrid != NULL && pParentBookDM != NULL)
	{
		APPLY_PARENT_DM(m_pDM_IDX_BO, m_pDM_SPECIES, m_pDM_ACCU, m_pDM_BOOK, pParentBookDM, pParentGrid, -1, 0, BOOK, FALSE, pGroupInfo);
		APPLY_PARENT_DM(m_pDM_IDX_BO, m_pDM_SPECIES, m_pDM_ACCU, m_pDM_BOOK, pParentDM, pParentGrid, -1, 0, SPECIES, TRUE, pGroupInfo);
	}

	return ids;
}

INT CBO_SPECIES::BO_RECYCLE_DATA_RESTORE(CString strSpeciesKey)
{
	// MODE is _T("RESTORE") or _T("PURCHASE")
	// DELETE��TBL���� ��TBL�� 1 row Move
	// DELETEåTBL���� åTBL n row Move
	// IDX�� SPECIES_CLASS�� 1�� �������ٰ�

	if (strSpeciesKey.GetLength() == 0) return -32001;	// ������KEY���� 
	INT nRowIdx = 0;

	INT ids;
	CArray <CString, CString> RemoveAliasList;

	m_pDM_DELETE_SPECIES->StartFrame();

	CString strOption, strQuery, strTmpData, strLastSerialNo, strWorkingStatus, strAcqType;

	// �ش� ������DM�� �����Ѵ�.
	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = m_pDM_DELETE_SPECIES->RefreshDataManager(strOption);
	ids = m_pDM_IDX_BO->RefreshDataManager(strOption);
	INT nSpeciesCount = m_pDM_DELETE_SPECIES->GetRowCount();
	if(m_pDM_DELETE_SPECIES->GetRowCount() == 0 || m_pDM_IDX_BO->GetRowCount() == 0)
	{
		pMgr->ESLAfxMessageBox(_T("�ش��ڷ��� ����/�������� �������� �ʽ��ϴ�. �����ڿ��� ���� �ٶ��ϴ�."));
		return 1000;
	}
	
	if (ids < 0) return -32002;
	// �ش� ����å�� åDM�� �����Ѵ�.
	strOption.Format(_T("SPECIES_KEY = %s"), strSpeciesKey);
	ids = m_pDM_DELETE_BOOK->RefreshDataManager(strOption);
	INT nBookCount = m_pDM_DELETE_BOOK->GetRowCount();
	if (ids < 0) return -32003;
	if (nBookCount == 0) return -100;

	// ���� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	m_pDM_DELETE_SPECIES->InitDBFieldData();
	m_pDM_DELETE_SPECIES->SetCellData(_T("BS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);

	strQuery.Format(_T("SELECT REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY=%s"), strSpeciesKey);
	m_pDM_DELETE_SPECIES->GetOneValueQuery(strQuery, strTmpData);
	if(strTmpData.IsEmpty())
	{
		GetMaxWorkingStatus(m_pDM_DELETE_BOOK, strWorkingStatus);
		if(strWorkingStatus.Left(3) == _T("BOR") || strWorkingStatus.Left(3) == _T("BOC")) 
			strWorkingStatus = _T("BOR111N");
		else if(strWorkingStatus.Left(3) == _T("BOL"))
			strWorkingStatus = _T("BOC113N");
		
		m_pDM_IDX_BO->SetCellData(_T("IBS_�۾�����INFO"), strWorkingStatus, 0);
		m_pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), _T("0"), 0);
	}
	else
	{
		strOption.Format(_T("�����Ϸ��� �ڷ�[��Ű:%s]�� �̹� �����մϴ�."), strSpeciesKey);
		pMgr->ESLAfxMessageBox(strOption);
		return -1;
	}
	//������ ����
	strQuery.Format(_T("DELETE FROM BO_DELETE_SPECIES_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
	m_pDM_DELETE_SPECIES->AddFrame(strQuery);
	
	//��ũ ����
	CString strMarc;
	CMarc marc;
	strMarc = m_pDM_DELETE_SPECIES->GetCellData(_T("BS_MARC"), 0);
	if(!strMarc.IsEmpty()) pMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);

	strAcqType = m_pDM_DELETE_BOOK->GetCellData(_T("BB_���Ա���_�ڵ�"), 0);
	strQuery = m_pDM_DELETE_BOOK->GetCellData(_T("BB_��������KEY"), 0);
	if(strQuery.IsEmpty())
	{
		strAcqType.Empty();
	}
	else
	{
		if(strAcqType == _T("1")) SetAccuDM(TRUE);
		else if(strAcqType == _T("2")) SetAccuDM(FALSE);
		strOption.Format(_T("REC_KEY=%s"), strQuery);
		if(m_pDM_ACCU != NULL) m_pDM_ACCU->RefreshDataManager(strOption);
	}
	
	if(strAcqType == _T("1"))
	{
		BOOL bWorkStatusChange = FALSE;
		CString strWorkStatus, strWorkCode, strAcqYear, strGroupNo, strGroupWorkStatus, strMsg, strTailMsg; 
		strWorkStatus = m_pDM_DELETE_BOOK->GetCellData(_T("BB_�ڷ����"), 0);
		strWorkCode = m_pDM_IDX_BO->GetCellData(_T("IBS_��������_�ڵ�"), 0);
		strAcqYear = m_pDM_ACCU->GetCellData(_T("BP_���Գ⵵"), 0);
		strGroupNo = m_pDM_ACCU->GetCellData(_T("BP_�׷��ȣ"), 0);
		
		if(!strAcqYear.IsEmpty() && !strGroupNo.IsEmpty())
		{
			if(strWorkCode.IsEmpty())
				strOption.Format(
					_T("SELECT WORKING_STATUS FROM BO_PURCHASE_SEQ_NO_TBL WHERE WORK_CODE IS NULL AND ACQ_YEAR='%s' AND SEQ_NO=%s"),
					strAcqYear, strGroupNo);
			else
				strOption.Format(
					_T("SELECT WORKING_STATUS FROM BO_PURCHASE_SEQ_NO_TBL WHERE WORK_CODE='%s' AND ACQ_YEAR='%s' AND SEQ_NO=%s"),
					strWorkCode, strAcqYear, strGroupNo);
			
			m_pDM_DELETE_SPECIES->GetOneValueQuery(strOption, strGroupWorkStatus);
			CMapStringToString mapFinder;	mapFinder.RemoveAll();
			mapFinder.SetAt(_T("BOA111N_1"), _T("BOA112N"));
			mapFinder.SetAt(_T("BOA111N_2"), _T("BOA114O"));
			mapFinder.SetAt(_T("BOA112N_0"), _T("BOA111N"));
			mapFinder.SetAt(_T("BOA112N_2"), _T("BOA114O"));
			mapFinder.SetAt(_T("BOA113O_0"), _T("BOA111N"));
			mapFinder.SetAt(_T("BOA113O_1"), _T("BOA112N"));
			CString strCompareData, strTmpWorkStatus;
			if(strWorkStatus.Compare(_T("BOA112N")) > 0) strCompareData = _T("BOA113O_") + strGroupWorkStatus;
			else strCompareData = strWorkStatus + _T("_") + strGroupWorkStatus;
			if(mapFinder.Lookup(strCompareData, strTmpWorkStatus))
			{
				CString strOrderSerialNo, strBookSerialNo;
				strTailMsg = _T("�ش��ڷ�� �������¿� �ڷ���°� ��ġ���� �ʽ��ϴ�. \n�ڷẹ�� ó���� �Ͻðڽ��ϱ�?");
				strMsg.Format(
					_T("����   : [ %s ]\n") _T("%s"), m_pDM_IDX_BO->GetCellData(_T("IBS_��ǥ��"), 0), strTailMsg);
				if (IDCANCEL == pMgr->ESLAfxMessageBox(strMsg, MB_OKCANCEL)) return 1000;
				if(strTmpWorkStatus == _T("BOA112N") || strTmpWorkStatus == _T("BOA114O")) 
				{
					strOption.Format(
						_T("SELECT MAX(P.SERIAL_NO) FROM BO_PURCHASEINFO_TBL P, IDX_BO_TBL I WHERE P.SPECIES_KEY=I.REC_KEY AND I.WORK_CODE='%s' AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s"),
						strWorkCode, strAcqYear, strGroupNo);
					m_pDM_DELETE_SPECIES->GetOneValueQuery(strOption, strOrderSerialNo);
					strLastSerialNo.Format(_T("%d"), _ttoi(strOrderSerialNo.GetBuffer(0))+1);
					m_pDM_ACCU->SetCellData(_T("BP_�Ϸù�ȣ"), strLastSerialNo, 0);
				}
				INT idx = m_pDM_DELETE_BOOK->GetRowCount() - 1;
				for (INT i=0 ; i <= idx ; i++)
				{
					m_pDM_DELETE_BOOK->SetCellData(_T("BB_�ڷ����"), strTmpWorkStatus, i);
				}
			}
		}
	}
	
	pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
	pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("001"));
	pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("090"));
	pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("005"));

	pMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	m_pDM_DELETE_SPECIES->SetCellData(_T("BS_MARC"), strMarc, 0);
	
	// å������ Insert�� Insert���� �����Ѵ�.
	INT nRowCount = m_pDM_DELETE_BOOK->GetRowCount();
	INT nTmpData = -1;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BB_������"));
	strLastSerialNo.Empty();
	
	for (INT i = 0; i < nRowCount; i++) 
	{
		strWorkingStatus = m_pDM_DELETE_BOOK->GetCellData(_T("BB_�ڷ����"), i);
		if(strWorkingStatus.Left(3) == _T("BOR")) 
			strWorkingStatus = _T("BOR111N");
		else if(strWorkingStatus.Left(3) == _T("BOC") || strWorkingStatus.Left(3) == _T("BOL"))
			strWorkingStatus = _T("BOC113N");
		m_pDM_DELETE_BOOK->SetCellData(_T("BB_�ڷ����"), strWorkingStatus, i);
		m_pDM_DELETE_BOOK->SetCellData(_T("BB_���Ա���"), strAcqType, i);
		
		if(m_pDM_ACCU != NULL && m_pDM_ACCU->GetRowCount() > 0)
		{
			CString strWorkCode = m_pDM_IDX_BO->GetCellData(_T("IBS_��������_�ڵ�"), 0);
			CString strAcqYear = m_pDM_ACCU->GetCellData(_T("BP_���Գ⵵"), 0);
			CString strGroupNo = m_pDM_ACCU->GetCellData(_T("BP_�׷��ȣ"), 0);

			m_pDM_DELETE_BOOK->SetCellData(_T("BB_���Գ⵵"), strAcqYear, i);	
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_�׷��ȣ"), strGroupNo, i);	
			if(strLastSerialNo.IsEmpty() && !strGroupNo.IsEmpty())
			{
				strTmpData = m_pDM_DELETE_BOOK->GetCellData(_T("BB_���Ա���_�ڵ�"), 0);
				strOption.Format(
					_T("SELECT MAX(SERIAL_NO) FROM BO_BOOK_TBL WHERE ACQ_CODE='%s' AND ACQ_YEAR='%s' AND SEQ_NO=%s"), 
					strTmpData, m_pDM_ACCU->GetCellData(_T("BP_���Գ⵵"), 0), m_pDM_ACCU->GetCellData(_T("BP_�׷��ȣ"), 0));
				m_pDM_ACCU->GetOneValueQuery(strOption, strLastSerialNo);
				nTmpData = _ttoi(strLastSerialNo.GetBuffer(0));
				strLastSerialNo.Format(_T("%d"), ++nTmpData);
			}
			else if(!strLastSerialNo.IsEmpty())
			{
				nTmpData = _ttoi(strLastSerialNo.GetBuffer(0));
				strLastSerialNo.Format(_T("%d"), ++nTmpData);
			}
		}
		else
			strLastSerialNo = _T("esl_seq.NEXTVAL");
		
		m_pDM_DELETE_BOOK->SetCellData(_T("BB_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), i);	
		//å�Է�
		m_pDM_DELETE_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_DELETE_BOOK, _T("BO_BOOK_TBL"), i, m_pDM_DELETE_SPECIES);
		//�ϻ��� å���̺��� �Ϸù�ȣ�� ����. �׷��� UPDATE���� ������ �Ѵ�.
		if(strLastSerialNo.IsEmpty()) strLastSerialNo = _T("NULL");
		strQuery.Format(
			_T("UPDATE BO_BOOK_TBL SET SERIAL_NO=%s WHERE REC_KEY=%s;"),
			strLastSerialNo, m_pDM_DELETE_BOOK->GetCellData(_T("BB_åKEY"), i));
		m_pDM_DELETE_SPECIES->AddFrame(strQuery);
	}
	//����å ����
	strQuery.Format(_T("DELETE FROM BO_DELETE_BOOK_TBL WHERE SPECIES_KEY=%s;"), strSpeciesKey);
	m_pDM_DELETE_SPECIES->AddFrame(strQuery);
	//���Է�
	m_pDM_DELETE_SPECIES->MakeQueryByDM(RemoveAliasList, m_pDM_DELETE_SPECIES, _T("BO_SPECIES_TBL"), nRowIdx, m_pDM_DELETE_SPECIES);
	
	// ���������� Insert�� Insert���� �����Ѵ�.
	//�̿����ѱ���/��������/�ڷ��/��ϱ��� ���а��� ���а� concatenation
	ids = SetBookIndexInfo(m_pDM_IDX_BO, m_pDM_DELETE_BOOK, nRowIdx);
	if(ids < 0) return -1;
	RemoveAliasList.RemoveAll();
	m_pDM_DELETE_SPECIES->InitDBFieldData();
	m_pDM_DELETE_SPECIES->MakeQueryByDM(RemoveAliasList, m_pDM_IDX_BO, _T("IDX_BO_TBL"), 0, m_pDM_DELETE_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
	//���� �ӽ� ���̺� ������ �����Ѵ�.

	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	BO_INDEX_TEMP_INSERT(m_pDM_DELETE_SPECIES, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("I"));
	//=====================================================
	
	//ECOSEARCH�� ���ؼ� �������
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strOption.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		m_pDM_DELETE_SPECIES->AddFrame(strOption);
	}
	
	ids = m_pDM_DELETE_SPECIES->SendFrame();
	if(ids < 0) -32051;
	ids = m_pDM_DELETE_SPECIES->EndFrame();
	if(ids < 0) -32051;

	return 0;
}

INT CBO_SPECIES::BO_RECYCLE_DATA_PROCESS(
										 CGroupInfo *pGroupInfo,
										 CString strSpeciesKey,
										 INT nAcqType,
										 CESL_DataMgr * pParentDM,
										 CESL_DataMgr * pParentBookDM,
										 CESL_Grid * pParentGrid)
{
	// MODE is _T("RESTORE") or _T("PURCHASE")
	// DELETE��TBL���� ��TBL�� 1 row Move
	// DELETEåTBL���� åTBL n row Move
	// IDX�� SPECIES_CLASS�� 1�� �������ٰ�

	ACQ_TYPE ATYPE = (ACQ_TYPE)nAcqType;
	
	if (strSpeciesKey.GetLength() == 0) return -32001;	// ������KEY���� 
	INT nRowIdx = 0;

	// Default ���� ���� ��Ų��.
	//SetDefaultValue(nRowIdx, pDM_IDX_BO, pDM_SPECIES, pDM_PURCHASE, pDM_BOOK);

	INT ids;
	CArray <CString, CString> RemoveAliasList;

	m_pDM_DELETE_SPECIES->StartFrame();

	CString strOption, strQuery, strTmpData, strLastSerialNo, strWorkingStatus;

	// �ش� ������DM�� �����Ѵ�.
	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = m_pDM_DELETE_SPECIES->RefreshDataManager(strOption);
	ids = m_pDM_IDX_BO->RefreshDataManager(strOption);
	if(m_pDM_DELETE_SPECIES->GetRowCount() == 0 || m_pDM_IDX_BO->GetRowCount() == 0)
	{
		pMgr->ESLAfxMessageBox(_T("�ش��ڷ��� ����/�������� �������� �ʽ��ϴ�. �����ڿ��� ���� �ٶ��ϴ�."));
		return 1000;
	}

	INT nSpeciesCount = m_pDM_DELETE_SPECIES->GetRowCount();
	if (ids < 0) return -32002;
	// �ش� ����å�� åDM�� �����Ѵ�.
	strOption.Format(_T("SPECIES_KEY = %s"), strSpeciesKey);
	ids = m_pDM_DELETE_BOOK->RefreshDataManager(strOption);
	INT nBookCount = m_pDM_DELETE_BOOK->GetRowCount();
	if (ids < 0) return -32003;
	if (nBookCount == 0) return -100;

	ids = DeletePrevAccuInfo(m_pDM_DELETE_SPECIES, m_pDM_DELETE_BOOK);
	if(ids < 0) return -1;
	// ���࿡ �����ڷ��̰� �ڷẹ���� ��� 
	
	if(ATYPE == PURCHASE && pGroupInfo != NULL)
	{
		m_pDM_DELETE_SPECIES->SetCellData(_T("BS_��������"), pGroupInfo->GetWorkCode(), 0);
		m_pDM_IDX_BO->SetCellData(_T("IBS_��������_�ڵ�"), pGroupInfo->GetWorkCode(), 0);
	}
	// ���� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
	m_pDM_DELETE_SPECIES->InitDBFieldData();
	m_pDM_DELETE_SPECIES->SetCellData(_T("BS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	m_pDM_IDX_BO->SetCellData(_T("IBS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);

	strQuery.Format(_T("SELECT REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY=%s"), strSpeciesKey);
	m_pDM_DELETE_SPECIES->GetOneValueQuery(strQuery, strTmpData);
	if(strTmpData.IsEmpty())
	{
		if(ATYPE == PURCHASE)
		{
			m_pDM_IDX_BO->SetCellData(_T("IBS_�۾�����INFO"), _T("BOA111N"), 0);
			m_pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), _T("0"), 0);
		}
		else if(ATYPE == DONATE)
		{
			m_pDM_IDX_BO->SetCellData(_T("IBS_�۾�����INFO"), _T("BOA211O"), 0);
			m_pDM_IDX_BO->SetCellData(_T("IBS_�����а�"), _T("0"), 0);
		}
	}
	//������ ����
	strQuery.Format(_T("DELETE FROM BO_DELETE_SPECIES_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
	m_pDM_DELETE_SPECIES->AddFrame(strQuery);
	
	//��ũ ����
	CString strMarc;
	CMarc marc;
	strMarc = m_pDM_DELETE_SPECIES->GetCellData(_T("BS_MARC"), 0);
	if(!strMarc.IsEmpty()) pMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);

	if(ATYPE == PURCHASE)
	{
		SetAccuDM(TRUE);
		ids = MakeAccuInfo(m_pDM_DELETE_SPECIES, m_pDM_ACCU, m_pDM_DELETE_BOOK, m_pDM_DELETE_SPECIES, strSpeciesKey, pGroupInfo, TRUE, _T("1"));
		if(ids < 0) return -1;
		pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("001"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("090"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("005"));
	}
	else if(ATYPE == DONATE)
	{
		SetAccuDM(FALSE);
		ids = MakeAccuInfo(m_pDM_DELETE_SPECIES, m_pDM_ACCU, m_pDM_DELETE_BOOK, m_pDM_DELETE_SPECIES, strSpeciesKey, pGroupInfo, FALSE, _T("1"));
		if(ids < 0) return -1;
		pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("001"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("090"));
		pMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("005"));
	}

	pMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	m_pDM_DELETE_SPECIES->SetCellData(_T("BS_MARC"), strMarc, 0);
	
	// å������ Insert�� Insert���� �����Ѵ�.
	INT nRowCount = m_pDM_DELETE_BOOK->GetRowCount();
	INT nTmpData = -1;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BB_������"));
	strLastSerialNo.Empty();
	
	for (INT i = 0; i < nRowCount; i++) 
	{
		if(ATYPE == PURCHASE)
		{
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_��������KEY"), m_pDM_ACCU->GetCellData(_T("BP_��������KEY"), 0), i);
			
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_�ڷ����"), _T("BOA111N"), i);
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_�����ڷ����"), m_pDM_DELETE_BOOK->GetCellData(_T("BB_�ڷ����"), i), i);
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_���Ա���_�ڵ�"), _T("1"), i);
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_��ǰ����"), _T("Y"), i);
			strLastSerialNo = m_pDM_ACCU->GetCellData(_T("BP_�����Ϸù�ȣ"), 0);
			if(pGroupInfo)
			{
				m_pDM_DELETE_BOOK->SetCellData(_T("BB_���Գ⵵"), pGroupInfo->GetAcqYear(), i);
				m_pDM_DELETE_BOOK->SetCellData(_T("BB_�׷��ȣ"), pGroupInfo->GetGroupInfo(), i);
			}
			else
			{
				m_pDM_DELETE_BOOK->SetCellData(_T("BB_���Գ⵵"), GetTodayYear(), i);
				m_pDM_DELETE_BOOK->SetCellData(_T("BB_�׷��ȣ"), _T(""), i);
			}
		}
		else if(ATYPE == DONATE)
		{
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_��������KEY"), m_pDM_ACCU->GetCellData(_T("BP_��������KEY"), 0), i);
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_���Գ⵵"), pGroupInfo->GetAcqYear(), i);
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_�ڷ����"), _T("BOA211O"), i);
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_�����ڷ����"), m_pDM_DELETE_BOOK->GetCellData(_T("BB_�ڷ����"), i), i);
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_���Ա���_�ڵ�"), _T("2"), i);
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_�׷��ȣ"), pGroupInfo->GetGroupInfo(), i);
			m_pDM_DELETE_BOOK->SetCellData(_T("BB_��ǰ����"), _T("Y"), i);
			nTmpData = pGroupInfo->GetLastSerialNo();
			if(nTmpData < 0) nTmpData = 0;
			else nTmpData++;
			strLastSerialNo.Format(_T("%d"), nTmpData);
			pGroupInfo->SetLastSerialNo(nTmpData);
			pGroupInfo->UpdateLastSerialNo(_T("2"), pMgr, m_pDM_DELETE_SPECIES);

		}
		m_pDM_DELETE_BOOK->SetCellData(_T("BB_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), i);	
		m_pDM_DELETE_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_DELETE_BOOK, _T("BO_BOOK_TBL"), i, m_pDM_DELETE_SPECIES);
		//�ϻ��� å���̺��� �Ϸù�ȣ�� ����. �׷��� UPDATE���� ������ �Ѵ�.
		if(strLastSerialNo.IsEmpty()) strLastSerialNo = _T("NULL");
		strQuery.Format(
			_T("UPDATE BO_BOOK_TBL SET SERIAL_NO=%s WHERE REC_KEY=%s;"),
			strLastSerialNo, m_pDM_DELETE_BOOK->GetCellData(_T("BB_åKEY"), i));
		m_pDM_DELETE_SPECIES->AddFrame(strQuery);
	}
	//����å ����
	strQuery.Format(_T("DELETE FROM BO_DELETE_BOOK_TBL WHERE SPECIES_KEY=%s;"), strSpeciesKey);
	m_pDM_DELETE_SPECIES->AddFrame(strQuery);

	//���Է�
	RemoveAliasList.RemoveAll();
	m_pDM_DELETE_SPECIES->MakeQueryByDM(RemoveAliasList, m_pDM_DELETE_SPECIES, _T("BO_SPECIES_TBL"), 0, m_pDM_DELETE_SPECIES);
	
	// ���������� Insert�� Insert���� �����Ѵ�.
	//�̿����ѱ���/��������/�ڷ��/��ϱ��� ���а��� ���а� concatenation
	ids = SetBookIndexInfo(m_pDM_IDX_BO, m_pDM_DELETE_BOOK, nRowIdx);
	if(ids < 0) return -1;
	RemoveAliasList.RemoveAll();
	m_pDM_DELETE_SPECIES->InitDBFieldData();
	m_pDM_DELETE_SPECIES->MakeQueryByDM(RemoveAliasList, m_pDM_IDX_BO, _T("IDX_BO_TBL"), 0, m_pDM_DELETE_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
	//���� �ӽ� ���̺� ������ �����Ѵ�.
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	BO_INDEX_TEMP_INSERT(m_pDM_DELETE_SPECIES, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("I"));
	//=====================================================
	
	
	//ECOSEARCH�� ���ؼ� �������
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strOption.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		m_pDM_DELETE_SPECIES->AddFrame(strOption);
	}
	
	ids = m_pDM_DELETE_SPECIES->SendFrame();
	if(ids < 0) -32051;
	ids = m_pDM_DELETE_SPECIES->EndFrame();
	if(ids < 0) -32051;

	if(pParentDM != NULL && pParentGrid != NULL && pParentBookDM != NULL)
	{
		APPLY_PARENT_DM(m_pDM_IDX_BO, m_pDM_DELETE_SPECIES, m_pDM_ACCU, m_pDM_DELETE_BOOK, pParentBookDM, pParentGrid, -1, 0, BOOK, FALSE, pGroupInfo);
		APPLY_PARENT_DM(m_pDM_IDX_BO, m_pDM_DELETE_SPECIES, m_pDM_ACCU, m_pDM_DELETE_BOOK, pParentDM, pParentGrid, -1, 0, SPECIES, TRUE, pGroupInfo);
	}

	return 0;
}

INT CBO_SPECIES::DeletePrevAccuInfo(CESL_DataMgr *pDM_OUT, CESL_DataMgr *pDM_BOOK)
{
	CString strPURCHASEINFO_KEY, strDONATEINFO_KEY;
	strPURCHASEINFO_KEY.Empty();		strDONATEINFO_KEY.Empty();
	INT nRowCnt = pDM_BOOK->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_BOOK->GetCellData(_T("BB_���Ա���_�ڵ�"), i);
		if(strTmpData == _T("1"))
		{
			strTmpData = pDM_BOOK->GetCellData(_T("BB_��������KEY"), i);
			if(strPURCHASEINFO_KEY.IsEmpty()) strPURCHASEINFO_KEY = strTmpData;
			else strPURCHASEINFO_KEY += _T(", ") + strTmpData;
		}
		else if(strTmpData == _T("2"))
		{
			strTmpData = pDM_BOOK->GetCellData(_T("BB_��������KEY"), i);
			if(strDONATEINFO_KEY.IsEmpty()) strDONATEINFO_KEY = strTmpData;
			else strDONATEINFO_KEY += _T(", ") + strTmpData;
		}
		else continue;
	}
	CString strQuery;
	if(!strPURCHASEINFO_KEY.IsEmpty())
	{
		strQuery.Format(_T("DELETE FROM BO_PURCHASEINFO_TBL WHERE REC_KEY IN (%s);"), strPURCHASEINFO_KEY);
		pDM_OUT->AddFrame(strQuery);
	}

	if(!strDONATEINFO_KEY.IsEmpty())
	{
		strQuery.Format(_T("DELETE FROM BO_DONINFO_TBL WHERE REC_KEY IN (%s);"), strDONATEINFO_KEY);
		pDM_OUT->AddFrame(strQuery);
	}

	return 0;
}

INT CBO_SPECIES::MakeAccuInfo(
							  CESL_DataMgr *pDM_OUT, 
							  CESL_DataMgr * pDM_ACCU, 
							  CESL_DataMgr *pDM_BOOK,
							  CESL_DataMgr * pDM_SPECIES,
							  CString strSpeciesKey, 
							  CGroupInfo * pGroup,
							  BOOL isPurchase, CString strInputType)
{
	CString strMarc;
	pDM_ACCU->InsertRow(-1);
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	
	CString strRecKey, strTmpData, strAcqkey, strPrice, strCurrencyCode, strPriceOtherInfo;
	CMarc marc;
	strMarc = pDM_SPECIES->GetCellData(_T("BS_MARC"), 0);
	pMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	pMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strTmpData);
	Apply_MARC_950(strTmpData, strPrice, strCurrencyCode);
	pMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("950$c"), strPriceOtherInfo);
	
	INT ids ;

	if(strInputType.IsEmpty())
	{
		strTmpData = pDM_BOOK->GetCellData(_T("BB_�ڷ����"), 0);
		if(strTmpData == _T("BOT111O")) strInputType = _T("0");
		else if(strTmpData == _T("BOT112O")) strInputType = _T("3");
	}

	if(isPurchase)
	{
		ids = pDM_OUT->MakeRecKey(strAcqkey); 
		if(ids < 0) return ids;
		CString strTmpData;
		strTmpData.Format(_T("%d"), pDM_BOOK->GetRowCount());
		pDM_ACCU->SetCellData(_T("BP_��������KEY"), strAcqkey, 0);
		pDM_ACCU->SetCellData(_T("BP_����å��"), strTmpData, 0);
		pDM_ACCU->SetCellData(_T("BP_��ǰå��"), strTmpData, 0);
		pDM_ACCU->SetCellData(_T("BP_�̳�å��"), _T("0"), 0);
		pDM_ACCU->SetCellData(_T("BP_�ֹ�����Ÿ��"), _T("������"), 0);
		strTmpData.Format(_T("%d"), GetVolumeCount(pDM_BOOK));
		pDM_ACCU->SetCellData(_T("BP_�ѱǼ�"), strTmpData, 0);
		pDM_ACCU->SetCellData(_T("BP_�������۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		pDM_ACCU->SetCellData(_T("BP_�����۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		pDM_ACCU->SetCellData(_T("BP_�Է�����"), pDM_BOOK->GetCellData(_T("BB_�Է���"), 0), 0);
		pDM_ACCU->SetCellData(_T("BP_�Է±���"), strInputType, 0);
		pDM_ACCU->SetCellData(_T("BP_���ְ���"), GetTotalPrice(pDM_BOOK), 0);
		pDM_ACCU->SetCellData(_T("BP_��ǰ����"), GetTotalPrice(pDM_BOOK), 0);
		pDM_ACCU->SetCellData(_T("BP_������"), _T("0"), 0);
		pDM_ACCU->SetCellData(_T("BP_��������"), GetTodayDate(), 0);
		pDM_ACCU->SetCellData(_T("BP_�˼���"), GetTodayDate(), 0);
		pDM_ACCU->SetCellData(_T("BP_ȯ��"), _T("1"), 0);
		pDM_ACCU->SetCellData(_T("BP_��KEY"), strSpeciesKey, 0);
		pDM_ACCU->SetCellData(_T("BP_����"), strPrice, 0);
		pDM_ACCU->SetCellData(_T("BP_ȭ�󱸺�"), strCurrencyCode, 0);
		pDM_ACCU->SetCellData(_T("BP_����������"), strPriceOtherInfo, 0);
		pDM_ACCU->SetCellData(_T("BP_��������_�ڵ�"), pDM_SPECIES->GetCellData(_T("BS_��������_�ڵ�"), 0), 0);
		
		if(pGroup)
		{
			pDM_ACCU->SetCellData(_T("BP_���Գ⵵"), pGroup->GetAcqYear(), 0);
			pDM_ACCU->SetCellData(_T("BP_�׷��ȣ"), pGroup->GetGroupInfo(), 0);
			strTmpData = pGroup->GetWorkNo();
			if(strTmpData == _T("������")) strTmpData = _T("0");
			pDM_ACCU->SetCellData(_T("BP_�۾���ȣ"), strTmpData, 0);
			INT nSerialNo = pGroup->GetLastSerialNo();
			strTmpData.Format(_T("%d"), nSerialNo+1);
			pGroup->SetLastSerialNo(nSerialNo+1);
			pDM_ACCU->SetCellData(_T("BP_�����Ϸù�ȣ"), strTmpData, 0);
			nSerialNo = pGroup->GetOrderLastSerialNo();
			if(nSerialNo < 0) pDM_ACCU->SetCellData(_T("BP_�Ϸù�ȣ"), _T(""), 0);
			else
			{
				strTmpData.Format(_T("%d"), nSerialNo+1);
				pGroup->SetOrderLastSerialNo(nSerialNo+1);
				pDM_ACCU->SetCellData(_T("BP_�Ϸù�ȣ"), strTmpData, 0);
			}
			pGroup->UpDateLastSerialNo(pMgr, pDM_OUT);
		}
		else
		{
			pDM_ACCU->SetCellData(_T("BP_���Գ⵵"), pDM_BOOK->GetCellData(_T("BB_���Գ⵵"), 0), 0);
			pDM_ACCU->SetCellData(_T("BP_�׷��ȣ"), _T(""), 0);
			pDM_ACCU->SetCellData(_T("BP_�۾���ȣ"), _T(""), 0);
			pDM_ACCU->SetCellData(_T("BP_�����Ϸù�ȣ"), _T(""), 0);
			pDM_ACCU->SetCellData(_T("BP_�Ϸù�ȣ"), _T(""), 0);
		}

		ids = pDM_ACCU->MakeQueryByDM(RemoveAliasList, pDM_ACCU, pDM_ACCU->TBL_NAME, 0, pDM_OUT);
		if(ids < 0) return ids;
	}
	else
	{
		if(pGroup == NULL) return -1;
		ids = pDM_ACCU->MakeRecKey(strAcqkey);
		if(ids < 0) return ids;

		pDM_ACCU->SetCellData(_T("BP_��������KEY"), strAcqkey, 0);
		strTmpData.Format(_T("%d"), pDM_BOOK->GetRowCount());
		pDM_ACCU->SetCellData(_T("BP_����å��"), strTmpData, 0);
		pDM_ACCU->SetCellData(_T("BP_���Գ⵵"), pGroup->GetAcqYear(), 0);
		pDM_ACCU->SetCellData(_T("BP_�׷��ȣ"), pGroup->GetGroupInfo(), 0);
		strTmpData.Format(_T("%d"), GetVolumeCount(pDM_BOOK));
		pDM_ACCU->SetCellData(_T("BP_�ѱǼ�"), strTmpData, 0);
		pDM_ACCU->SetCellData(_T("BP_�������۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		pDM_ACCU->SetCellData(_T("BP_�����۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		pDM_ACCU->SetCellData(_T("BP_�Է�����"), pDM_BOOK->GetCellData(_T("BB_�Է���"), 0), 0);
		pDM_ACCU->SetCellData(_T("BP_�˼���"), GetTodayDate(), 0);
		pDM_ACCU->SetCellData(_T("BP_�Է±���"), strInputType, 0);
		pDM_ACCU->SetCellData(_T("BP_��KEY"), strSpeciesKey, 0);
		pDM_ACCU->SetCellData(_T("BP_����"), strPrice, 0);
		pDM_ACCU->SetCellData(_T("BP_ȭ�󱸺�"), strCurrencyCode, 0);
		pDM_ACCU->SetCellData(_T("BP_����������"), strPriceOtherInfo, 0);
		pDM_ACCU->SetCellData(_T("BP_��������_�ڵ�"), pDM_SPECIES->GetCellData(_T("BS_��������_�ڵ�"), 0), 0);
		ids = pDM_ACCU->MakeQueryByDM(RemoveAliasList, pDM_ACCU, pDM_ACCU->TBL_NAME, 0, pDM_OUT);
		if(ids < 0) return ids;
	}
	return 0;
}

INT CBO_SPECIES::GetMaxWorkingStatus(CESL_DataMgr *pDM_BOOK, CString &strWorkingStatus)
{
	INT nRowCnt = pDM_BOOK->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_BOOK->GetCellData(_T("BB_�ڷ����"), i);
		if(GetWorkingStatusPriority(strTmpData) > GetWorkingStatusPriority(strWorkingStatus))
			strWorkingStatus = strTmpData;
	}
	return 0;
}

INT CBO_SPECIES::GetWorkingStatusPriority(CString strTmpData)
{
	if(strTmpData.IsEmpty()) return 0;
	else if(strTmpData.Left(3) == _T("BOT")) return 1;
	else if(strTmpData.Left(3) == _T("BOA")) return 2;
	else if(strTmpData.Left(3) == _T("BOR")) return 3;
	else if(strTmpData.Left(3) == _T("BOC")) return 4;
	else if(strTmpData.Left(3) == _T("BOL")) return 5;
	return -1;
}

INT CBO_SPECIES::APPLY_PARENT_DM(
								 CESL_DataMgr *pDM_INDEX, 
								 CESL_DataMgr *pDM_SPECIES,
								 CESL_DataMgr *pDM_ACCU, 
								 CESL_DataMgr *pDM_BOOK, 
								 CESL_DataMgr *pDM_TARGET, 
								 CESL_Grid *pTargetGrid,
								 INT nTargetIdx, 
								 INT nSpeciesIdx,
								 PARENT_TYPE nParentType, 
								 BOOL bGridDisplayLine,
								 CGroupInfo * pGroup
								 )
{
	INT nSourceIdx, nRowCnt, nIdx;
	CString strTmpData;
	if(nParentType == SPECIES)
	{
		nSourceIdx = Find_REPRESENT_BOOK_RowIdx(pDM_BOOK);
		if(nSourceIdx < 0) nSourceIdx = 0;
		nIdx = APPLY_PARENT_DM_ONE_ROW(pDM_INDEX, pDM_SPECIES, pDM_ACCU, pDM_BOOK, pDM_TARGET, nTargetIdx, nSpeciesIdx, nSourceIdx, nParentType, pGroup);
		if(bGridDisplayLine) 
		{
			pTargetGrid->DisplayLine(nIdx);
			pTargetGrid->DisplayNumber();
			pTargetGrid->SetTopRow(nIdx+1);
		}
	}
	else if(nParentType == VOLUME)
	{
		CESL_DataMgr *pDM_VOLUME = new CESL_DataMgr();
		pDM_VOLUME->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
		pDM_VOLUME->InitDMPKFromFile(_T("DMUP_����_��_������"));
		pDM_VOLUME->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
		CONVERT_BOOK_to_VOLUME(pDM_BOOK, pDM_VOLUME);
		INT nRowCnt = 0;
		//���� Target�� �ִ� �������� �����Ѵ�.
		nRowCnt = pDM_TARGET->GetRowCount();
		CArray<INT, INT> arrIdx;	arrIdx.RemoveAll();
		for(INT i=nRowCnt-1 ; i>=0 ; i--)
		{
			if(IsEqualSpecies(pDM_TARGET, pDM_VOLUME->GetCellData(_T("BB_��KEY"), 0), pDM_VOLUME->GetCellData(_T("BB_��������KEY"), 0), i) == 0)
				arrIdx.Add(i);
		}
		nRowCnt = arrIdx.GetSize();
		for(i=0 ; i<nRowCnt ; i++)
			pDM_TARGET->DeleteRow(arrIdx.GetAt(i));

		nRowCnt = pDM_VOLUME->GetRowCount();
		for(i=0 ; i<nRowCnt ; i++)
		{
			nIdx = APPLY_PARENT_DM_ONE_ROW(pDM_INDEX, pDM_SPECIES, pDM_ACCU, pDM_VOLUME, pDM_TARGET, -1, nSpeciesIdx, i, nParentType, pGroup, pDM_BOOK);
			//if(bGridDisplayLine) pTargetGrid->DisplayLine(nIdx);
		}
		if(bGridDisplayLine) pTargetGrid->Display();
		if(pDM_VOLUME)
		{
			delete pDM_VOLUME;
			pDM_VOLUME = NULL;
		}
	}
	else if(nParentType == BOOK)
	{
		nRowCnt = pDM_BOOK->GetRowCount();
		for(INT i=0 ; i<nRowCnt ; i++)
		{
			strTmpData = pDM_BOOK->GetCellData(_T("BB_å�η��÷���"), i);
			if(strTmpData == _T("A")) continue;
			strTmpData = pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i);
			if(strTmpData == _T("D"))
			{
				nIdx = FindTargetDMRowIdx(pDM_TARGET, pDM_BOOK->GetCellData(_T("BB_��KEY"), i), pDM_BOOK->GetCellData(_T("BB_��������KEY"), i), _T(""), pDM_BOOK->GetCellData(_T("BB_åKEY"), i));
				if(nIdx < 0) continue;
				pDM_TARGET->DeleteRow(nIdx);
				if(bGridDisplayLine)
				{
					pTargetGrid->RemoveAt(nIdx);
				}
			}
			else if(strTmpData == _T("U") || strTmpData.IsEmpty())
			{
				nIdx = FindTargetDMRowIdx(pDM_TARGET, pDM_BOOK->GetCellData(_T("BB_��KEY"), i), pDM_BOOK->GetCellData(_T("BB_��������KEY"), i), _T(""), pDM_BOOK->GetCellData(_T("BB_åKEY"), i));
				if(nIdx < 0) continue;
				APPLY_PARENT_DM_ONE_ROW(pDM_INDEX, pDM_SPECIES, pDM_ACCU, pDM_BOOK, pDM_TARGET, nIdx, nSpeciesIdx, i, nParentType, pGroup);
				if(bGridDisplayLine) 
				{
					pTargetGrid->DisplayLine(nIdx);
					pTargetGrid->DisplayNumber();
					pTargetGrid->SetTopRow(nIdx+1);
				}
			}
			else if(strTmpData == _T("I"))
			{
				nIdx = APPLY_PARENT_DM_ONE_ROW(pDM_INDEX, pDM_SPECIES, pDM_ACCU, pDM_BOOK, pDM_TARGET, -1, nSpeciesIdx, i, nParentType, pGroup);
				if(bGridDisplayLine) 
				{
					pTargetGrid->DisplayLine(nIdx);
					pTargetGrid->DisplayNumber();
					pTargetGrid->SetTopRow(nIdx+1);
				}
			}
		}
	}
	return 0;
}

INT CBO_SPECIES::APPLY_PARENT_DM_ONE_ROW(CESL_DataMgr *pDM_INDEX,
										 CESL_DataMgr *pDM_SPECIES,
										 CESL_DataMgr *pDM_ACCU, 
										 CESL_DataMgr *pDM_BOOK, 
										 CESL_DataMgr *pDM_TARGET, 
										 INT nTargetIdx, 
										 INT nSpeciesIdx, 
										 INT nSourceIdx,
										 INT nParentType,
										 CGroupInfo * pGroup,
										 CESL_DataMgr *pDM_TMP/*=NULL*/)
{

	if(nTargetIdx < 0)
	{
		pDM_TARGET->InsertRow(-1);
		nTargetIdx = pDM_TARGET->GetRowCount() - 1;
	}
	
	INT nColCnt = pDM_TARGET->GetRefColumnCount();
	INT nPos;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strTmpData;
	for(INT i=0 ; i<nColCnt ; i++)
	{
		pDM_TARGET->GetColumnRefInfo(i, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
		strTmpData = FIELD_ALIAS.Left(2);
		nPos = FIELD_NAME.Find(_T("."), 0);
		if(nPos >= 0) FIELD_NAME = FIELD_NAME.Mid(nPos+1);
		if(strTmpData == _T("IB") && pDM_INDEX) 
			APPLY_PARENT_DM_ONE_CELL(pDM_INDEX, pDM_TARGET, FIELD_NAME, nTargetIdx, i, nSpeciesIdx);
		else if(strTmpData == _T("BP") && pDM_ACCU) 
			APPLY_PARENT_DM_ONE_CELL(pDM_ACCU, pDM_TARGET, FIELD_NAME, nTargetIdx, i, 0);
		else if(strTmpData == _T("BS") && pDM_SPECIES) 
			APPLY_PARENT_DM_ONE_CELL(pDM_SPECIES, pDM_TARGET, FIELD_NAME, nTargetIdx, i, nSpeciesIdx);
		else if(strTmpData == _T("BB") && pDM_BOOK) 
			APPLY_PARENT_DM_ONE_CELL(pDM_BOOK, pDM_TARGET, FIELD_NAME, nTargetIdx, i, nSourceIdx);
		else if(strTmpData == _T("UD")) 
			APPLY_PARENT_DM_UDF_CELL(pDM_INDEX, pDM_SPECIES, pDM_ACCU, pDM_BOOK, pDM_TARGET, 
									FIELD_ALIAS, nTargetIdx, nSpeciesIdx, nParentType, pGroup, pDM_TMP);
									// 2004.09.08 nSourceIdx�� nSpeciesIdx�� ����.
									// FIELD_ALIAS, nTargetIdx, nSourceIdx, nParentType, pGroup, pDM_TMP);
	}
	return nTargetIdx;
}

/*
INT CBO_SPECIES::APPLY_PARENT_DM_ONE_CELL(
										  CESL_DataMgr *pDM_SOURCE, 
										  CESL_DataMgr *pDM_TARGET, 
										  CString strFieldName, 
										  DWORD * pRow, 
										  INT nTargetColIdx, 
										  INT nSourceRowIdx)
{
	if(pDM_SOURCE == NULL) return 0;
	if(pRow == NULL) return 0;
	INT nColCnt = pDM_SOURCE->GetRefColumnCount();
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strTmpData;
	for(INT i=0 ; i<nColCnt ; i++)
	{
		pDM_SOURCE->GetColumnRefInfo(i, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
		if(FIELD_NAME == strFieldName)
		{
			pDM_TARGET->SetCellData(pRow, nTargetColIdx, pDM_SOURCE->GetCellData(nSourceRowIdx, i));
			//(nTargetRowIdx, nTargetColIdx, pDM_SOURCE->GetCellData(nSourceRowIdx, i));
			break;
		}
	}
	return 0;
}
*/

INT CBO_SPECIES::APPLY_PARENT_DM_ONE_CELL(
										  CESL_DataMgr *pDM_SOURCE, 
										  CESL_DataMgr *pDM_TARGET, 
										  CString strFieldName, 
										  INT nTargetRowIdx, 
										  INT nTargetColIdx, 
										  INT nSourceRowIdx)
{
	if(pDM_SOURCE == NULL) return 0;
	INT nColCnt = pDM_SOURCE->GetRefColumnCount();
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strTmpData;
	for(INT i=0 ; i<nColCnt ; i++)
	{
		pDM_SOURCE->GetColumnRefInfo(i, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
		if(FIELD_NAME == strFieldName)
		{
			pDM_TARGET->SetCellData(nTargetRowIdx, nTargetColIdx, pDM_SOURCE->GetCellData(nSourceRowIdx, i));
			break;
		}
	}
	return 0;
}

INT CBO_SPECIES::DeleteBookDMRow_UPDATE(CESL_DataMgr *pDM_PARENT_BOOK, INT nTargetRowIdx, CString strSpeciesKeyAlias, CString strAccuKeyAlias)
{
	INT nRowCnt = pDM_PARENT_BOOK->GetRowCount();
	CString strSpeciesKey, strAcqKey, strTmpData;
	strSpeciesKey = pDM_PARENT_BOOK->GetCellData(strSpeciesKeyAlias, nTargetRowIdx);
	strAcqKey = pDM_PARENT_BOOK->GetCellData(strAccuKeyAlias, nTargetRowIdx);
	for(INT i=nRowCnt-1 ; i>=0 ; i++)
	{
		strTmpData = pDM_PARENT_BOOK->GetCellData(strSpeciesKeyAlias, i);
		if(strTmpData == strSpeciesKey)
		{
			if(!strAcqKey.IsEmpty())
			{
				strTmpData = pDM_PARENT_BOOK->GetCellData(strAccuKeyAlias, i);
				if(strTmpData == strAcqKey)
				{
					pDM_PARENT_BOOK->DeleteRow(i);
				}
			}
			else
				pDM_PARENT_BOOK->DeleteRow(i);
		}
	}
	return 0;
}

INT CBO_SPECIES::IsEqualSpecies(CESL_DataMgr *pDM_TARGET, CString strSpeciesKey, CString strAcqKey, INT nIdx)
{
	CString strData1, strData2, strTmpData;
	strTmpData = pDM_TARGET->GetCellData(_T("IBS_��ǥ��"), nIdx);
	strData1 = pDM_TARGET->GetCellData(_T("IBS_��KEY"), nIdx);
	strData2 = pDM_TARGET->GetCellData(_T("BP_��������KEY"), nIdx);
	if(strSpeciesKey == strData1 && strAcqKey == strData2) return 0;
	return -1;
}

INT CBO_SPECIES::FindTargetDMRowIdx(CESL_DataMgr *pDM_TARGET, CString strSpeciesKey, CString strAcqKey, CString strVolSortNo, CString strRecKey/*=_T("")*/)
{
	INT nRowCnt = pDM_TARGET->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_TARGET->GetCellData(_T("IBS_��KEY"), i);
		if(strTmpData == strSpeciesKey)
		{
			if(!strAcqKey.IsEmpty() && strAcqKey != _T("-1"))
			{
				strTmpData = pDM_TARGET->GetCellData(_T("BP_��������KEY"), i);
				if(strTmpData == strAcqKey)
				{
					strTmpData = pDM_TARGET->GetCellData(_T("BB_���Ϸù�ȣ"), i);
					if(strTmpData == strVolSortNo || strVolSortNo.IsEmpty())
					{
						if(!strRecKey.IsEmpty())
						{
							strTmpData = pDM_TARGET->GetCellData(_T("BB_åKEY"), i);
							if(strRecKey == strTmpData)
								return i;
						}
						else return i;
					}
				}
			}
			else 
			{
				strTmpData = pDM_TARGET->GetCellData(_T("BB_���Ϸù�ȣ"), i);
				if(strTmpData == strVolSortNo || strVolSortNo.IsEmpty())
				{
					if(!strRecKey.IsEmpty())
					{
						strTmpData = pDM_TARGET->GetCellData(_T("BB_åKEY"), i);
						if(strRecKey == strTmpData)
							return i;
					}
					else return i;
				}
			}
		}
	}
	return -1;
}

INT CBO_SPECIES::BO_SPECIES_DUPADD_UPDATE(CESL_DataMgr *pDM_IDX_BO, 
										  CESL_DataMgr *pDM_SPECIES, 
										  CESL_DataMgr *pDM_BOOK, 
										  CESL_DataMgr *pDM_ACCU, 
										  CESL_DataMgr *pDM_APPENDIX, 
										  INT nRowIdx,
										  INT nSpeciesType,
										  INT nAcqType)
{
	INT ids;
	ACQ_TYPE nAType = (ACQ_TYPE)nAcqType;
	SPECIES_TYPE nStype = (SPECIES_TYPE)nSpeciesType;
	m_nSpeciesType = nStype;
	CString strSpeciesKey, strAcqKey, strNewSpeciesKey, strNewAcqKey, strQuery;
	strSpeciesKey = pDM_ACCU->GetCellData(_T("BP_��KEY"), nRowIdx);
	strAcqKey = pDM_ACCU->GetCellData(_T("BP_��������KEY"), nRowIdx);
	if(strAcqKey.IsEmpty()) strAcqKey = _T("-1");
	strNewSpeciesKey = pDM_IDX_BO->GetCellData(_T("IBS_REC_KEY"), nRowIdx);
	
	CArray <CString, CString> RemoveAliasList;

	// ���������� �� ��KEY�� �����Ѵ�.
	pDM_ACCU->SetCellData(_T("BP_��KEY"), strNewSpeciesKey, nRowIdx);
	
	// ���������� Update���� �����Ѵ�.
	if(nAType == PURCHASE ||  nAType == DONATE)
	{
		RemoveAliasList.RemoveAll();
		RemoveAliasList.Add(_T("BP_��������KEY"));
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_ACCU, pDM_ACCU->TBL_NAME, nRowIdx, pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strAcqKey);
	}
	
	for(INT i=0 ; i<pDM_BOOK->GetRowCount() ; i++)
	{
		pDM_BOOK->SetCellData(_T("BB_������KEY"), strSpeciesKey, i);
	}
	
	// å������ Insert/Update/Delete Ȥ�� ����å������ Insert���� �����Ѵ�. -> �׸��� DB�� SendFrame�Ѵ�.
	ids = MakeBookQueryFrame(pDM_BOOK, strNewSpeciesKey, strAcqKey, pDM_BOOK);
	if (ids < 0) return -34052;
	
	// �η�������  Insert/Update/Delete Ȥ�� ����å������ Insert���� �����Ѵ�. -> �׸��� DB�� SendFrame�Ѵ�.
	if(pDM_APPENDIX != NULL)
	{
		for(INT i=0 ; i<pDM_APPENDIX->GetRowCount() ; i++)
		{
			pDM_APPENDIX->SetCellData(_T("BB_å�η��÷���"), _T("A"), i);
		}
		MakeBookQueryFrame(pDM_APPENDIX, strNewSpeciesKey, strAcqKey, pDM_BOOK);
	}
	
	// E-BOOK������ Insert�� Insert���� �����Ѵ�.
	ids = MakeEBookInfoAndQueryFrame(pDM_BOOK, m_pDM_EBook, pDM_BOOK);
	if(ids < 0) return -33050;
	
	if(strSpeciesKey != strNewSpeciesKey) 
	{
		strQuery.Format(_T("update idx_bo_tbl set USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
			_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
			_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
			_T("IDX_VOL_TITLE=GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX'), ")
			_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
			_T("IDX_VOL=GET_BO_BOOK_INFO(%s,'VOL_INDEX'), ")
			_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE'), ")
			_T("WORKING_STATUS='%s', LAST_WORK='%s' where rec_key=%s;"), 
			strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, _T("BOT211O"), pMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
		pDM_BOOK->AddFrame(strQuery);		
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		BO_INDEX_TEMP_INSERT(pDM_BOOK, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("I"));
		//=====================================================
		
		//ECOSEARCH�� ���ؼ� �������
		if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
			pDM_BOOK->AddFrame(strQuery);
		}
	}
	//�� �������� ����
	strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET DUP_FLAG='Y', LAST_WORK='%s' WHERE REC_KEY = %s;"),
		pMgr->GetWorkLogMsg(WORK_LOG), strNewSpeciesKey);
	pDM_BOOK->AddFrame(strQuery);
	//������ ST_CODE�� �����Ѵ�.
	Set_Index_ST_CODE(_T("IBS_ISBN"), pDM_IDX_BO, pDM_BOOK, 1);	
	ids = SetBookIndexInfo(pDM_IDX_BO, pDM_BOOK, nRowIdx);
	if(ids < 0) return -1;
	pDM_IDX_BO->SetCellData(_T("IBS_��������"), _T("Y"), nRowIdx);
	pDM_IDX_BO->SetCellData(_T("IBS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("IBS_REC_KEY"));
	pDM_IDX_BO->InitDBFieldData();
	pDM_IDX_BO->MakeQueryByDM(RemoveAliasList, pDM_IDX_BO, _T("IDX_BO_TBL"), nRowIdx, pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strNewSpeciesKey);
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	BO_INDEX_TEMP_INSERT(pDM_BOOK, _T("IDX_TMP_BO_TBL"), strNewSpeciesKey, _T("I"));
	//=====================================================
	
	
	//ECOSEARCH�� ���ؼ� �������
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strNewSpeciesKey);
		pDM_BOOK->AddFrame(strQuery);
	}
	
	// ��������&å���� Send Frame
	//ids = SendFrame(pDM_BOOK, MAX_TRANSACTION_QUERY_COUNT);
	ids = pDM_BOOK->SendFrame();
	if (ids < 0) return -33052;		// �������� & å Update Error		
	ids = pDM_BOOK->EndFrame();
	if (ids < 0) return -33052;		// �������� & å Update Error	
	return 0;
}

INT CBO_SPECIES::BO_SPECIES_SRCINFO_INSERT(CESL_DataMgr *pDM_IDX_BO, 
										   CESL_DataMgr *pDM_SPECIES, 
										   CESL_DataMgr *pDM_BOOK, 										   
										   CESL_DataMgr *pDM_ACCU, 
										   CESL_DataMgr *pDM_OUT
										   )
{
	INT ids;
	//����/������ ���� ����
	CESL_DataMgr DM_SPECIES;
	DM_SPECIES.SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
	DM_SPECIES.InitDMPKFromFile(pDM_SPECIES->DB_MGR_ALIAS);
	DM_SPECIES.SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	
	CESL_DataMgr DM_IDX_BO;
	DM_IDX_BO.SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
	DM_IDX_BO.InitDMPKFromFile(pDM_IDX_BO->DB_MGR_ALIAS);
	DM_IDX_BO.SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	
	DM_SPECIES.InsertRow(-1);
	DM_IDX_BO.InsertRow(-1);
	
	ids = CopyDMRowToDMByAlias(pDM_IDX_BO, 0, &DM_IDX_BO, 0);
	if(ids < 0) return -1;
	ids = CopyDMRowToDMByAlias(pDM_SPECIES, 0, &DM_SPECIES, 0);
	if(ids < 0) return -1;
	SetDefaultValue_SPECIES(&DM_SPECIES, 0);
	SetDefaultValue_IDX(&DM_IDX_BO, 0);
	
	CMarc marc;
	CString strMarc, strSpeciesKey;
	strMarc.Empty();	strSpeciesKey.Empty();
	pMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	// ���� ���� ���� (����߰���)
	CString strLeader;
	marc.GetLeader(strLeader);
	// ���ڵ� ���� ������ �ִ´�. 5��°
	if (strLeader.GetAt(5) == ' ') strLeader.SetAt(5, 'n');
	// ���ڵ� ���� ������ �ִ´�. 6��°
	if (strLeader.GetAt(6) == ' ') strLeader.SetAt(6, 'a');
	// �������� ������ �ִ´�. 7��°
	if (strLeader.GetAt(7) == ' ') strLeader.SetAt(7, 'm');
	
	marc.SetLeader(strLeader.GetBuffer(0));
	
	ids = APPLY_Species_to_CMarc(&marc, pDM_IDX_BO, pDM_SPECIES, pDM_BOOK, pDM_ACCU, 0);
	if(ids < 0) return -1;
	pMgr->m_pInfo->pMarcMgr->CheckDeepSyntax(&marc);
	pMgr->m_pInfo->pMarcMgr->CheckSimpleSyntax(&marc);
	pMgr->m_pInfo->pMarcMgr->DeleteEmptySubfield(&marc);
	pMgr->m_pInfo->pMarcMgr->DeleteEmptyField(&marc);
	ids = pMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if(ids < 0) return ids;
	if(strMarc.IsEmpty()) return -1;

	DM_SPECIES.SetCellData(_T("BS_MARC"), strMarc, 0);
	ids = pDM_OUT->MakeRecKey(strSpeciesKey);
	if(ids < 0) return -1;

	CArray <CString, CString> RemoveAliasList;
	
	CString strUSELIMIT_CODE, strData;
	Get_Priority_USELIMIT_CODE(pDM_BOOK, strUSELIMIT_CODE);
	DM_SPECIES.SetCellData(_T("BS_�̿����ѱ���_�ڵ�"), strUSELIMIT_CODE, 0);
	DM_SPECIES.SetCellData(_T("BS_��KEY"), strSpeciesKey, 0);
	DM_SPECIES.SetCellData(_T("BS_���Գ⵵"), pDM_ACCU->GetCellData(_T("BP_���Գ⵵"), 0), 0);
	DM_SPECIES.SetCellData(_T("BS_��������_�ڵ�"), pDM_ACCU->GetCellData(_T("BP_��������_�ڵ�"), 0), 0);
	RemoveAliasList.RemoveAll();
	DM_SPECIES.InitDBFieldData();
	DM_SPECIES.MakeQueryByDM(RemoveAliasList, &DM_SPECIES, _T("BO_SPECIES_TBL"), 0, pDM_OUT);

	DM_SPECIES.GetCellData(_T("BS_MARC"), 0, strMarc);
	pMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);		
	ids = APPLY_IndexItem(&DM_IDX_BO, &marc, 0);
	if(ids < 0) return -1;
	DM_IDX_BO.SetCellData(_T("IBS_REC_KEY"), strSpeciesKey, 0);
	DM_IDX_BO.SetCellData(_T("IBS_�����а�"), _T("0"), 0);
	DM_IDX_BO.SetCellData(_T("IBS_��������"), _T(""), 0);
	DM_IDX_BO.SetCellData(_T("IBS_�۾�����INFO"), _T("BOT211O"), 0);
	Set_Index_ST_CODE(_T("IBS_ISBN"), &DM_IDX_BO, pDM_BOOK, -1);
	ids = SetBookIndexInfo(&DM_IDX_BO, pDM_BOOK, 0);
	if(ids < 0) return -1;
	RemoveAliasList.RemoveAll();
	DM_IDX_BO.InitDBFieldData();
	DM_IDX_BO.MakeQueryByDM(RemoveAliasList, &DM_IDX_BO, _T("IDX_BO_TBL"), 0, pDM_OUT);
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	BO_INDEX_TEMP_INSERT(pDM_OUT, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("I"));
	//=====================================================
	
	
	//ECOSEARCH�� ���ؼ� �������
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strData.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		pDM_OUT->AddFrame(strData);
	}
	
	//å������ DUP_SPECIES_KEY���� ����
	for(INT i=0 ; i<pDM_BOOK->GetRowCount() ; i++)
	{
		pDM_BOOK->SetCellData(_T("BB_������KEY"), strSpeciesKey, i);
	}
	return 0;
}

INT CBO_SPECIES::BO_SPECIES_DUPADD_INSERT(CESL_DataMgr *pDM_IDX_BO, 
 										  CESL_DataMgr *pDM_SPECIES, 
										  CESL_DataMgr *pDM_BOOK, 
										  CESL_DataMgr *pDM_ACCU, 
										  CESL_DataMgr *pDM_APPENDIX, 
										  INT nRowIdx,
										  INT nSpeciesType,
										  INT nAcqType)
{
	ACQ_TYPE nAType = (ACQ_TYPE)nAcqType;
	SPECIES_TYPE nStype = (SPECIES_TYPE)nSpeciesType;
	m_nSpeciesType = nStype;
	CString strSpeciesKey, strAcqKey, strNewSpeciesKey, strNewAcqKey;
	CArray <CString, CString> RemoveAliasList;
	
	INT ids;
	CString strQuery, strTmp, strWORKING_STATUS;
	
	strNewSpeciesKey = pDM_IDX_BO->GetCellData(_T("IBS_REC_KEY"), nRowIdx);
	strNewAcqKey = _T("-1");
	
	ids = BO_SPECIES_SRCINFO_INSERT(pDM_IDX_BO, pDM_SPECIES, pDM_BOOK, pDM_ACCU, pDM_BOOK);
	if(ids < 0) return -1;

	// ���������� Insert�� Insert���� �����Ѵ�.
	if(nAcqType == PURCHASE)
	{
		pDM_IDX_BO->MakeRecKey(strNewAcqKey);
		SetDefaultValue_PURCHASEINFO(pDM_ACCU, 0, pDM_BOOK);
	}
	//else if(nStype == BO_DONATE || nStype == BO_NONDONATE || nStype == BO_REG_DONATE)
	else if(nAType == DONATE)
	{
		pDM_IDX_BO->MakeRecKey(strNewAcqKey);
		SetDefaultValue_DONATEINFO(pDM_ACCU, 0, pDM_BOOK);
	}
	SetDefaultValue_BOOK(pDM_BOOK, pDM_ACCU, 0);
	
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	if(nAType == PURCHASE || nAType == DONATE)
	{
		pDM_ACCU->SetCellData(_T("BP_��KEY"), strNewSpeciesKey, nRowIdx);
		pDM_ACCU->SetCellData(_T("BP_��������KEY"), strNewAcqKey, nRowIdx);
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_ACCU, pDM_ACCU->TBL_NAME, nRowIdx, pDM_BOOK);
	}
	
	// å������ Insert�� Insert���� �����Ѵ�.
	MakeBookQueryFrame(pDM_BOOK, strNewSpeciesKey, strNewAcqKey, pDM_BOOK);	
	
	// �η������� Insert�� Insert���� �����Ѵ�.
	if(pDM_APPENDIX != NULL)
	{
		for(INT i=0 ; i<pDM_APPENDIX->GetRowCount() ; i++)
		{
			pDM_APPENDIX->SetCellData(_T("BB_å�η��÷���"), _T("A"), i);
		}
		MakeBookQueryFrame(pDM_APPENDIX, strNewSpeciesKey, strNewAcqKey, pDM_BOOK);	
	}
	// E-BOOK������ Insert�� Insert���� �����Ѵ�.
	ids = MakeEBookInfoAndQueryFrame(pDM_BOOK, m_pDM_EBook, pDM_BOOK);
	if(ids < 0) return -33050;
	
	//�� �������� ����
	strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET DUP_FLAG='Y', LAST_WORK='%s' WHERE REC_KEY = %s;"),
		pMgr->GetWorkLogMsg(WORK_LOG), strNewSpeciesKey);
	pDM_BOOK->AddFrame(strQuery);
	//������ ST_CODE�� �����Ѵ�.
	Set_Index_ST_CODE(_T("IBS_ISBN"), pDM_IDX_BO, pDM_BOOK, 1);
	ids = SetBookIndexInfo(pDM_IDX_BO, pDM_BOOK, nRowIdx);
	if(ids < 0) return -1;
	pDM_IDX_BO->SetCellData(_T("IBS_��������"), _T("Y"), nRowIdx);
	pDM_IDX_BO->SetCellData(_T("IBS_�������۾�����"), pMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("IBS_REC_KEY"));
	pDM_IDX_BO->InitDBFieldData();
	pDM_IDX_BO->MakeQueryByDM(RemoveAliasList, pDM_IDX_BO, _T("IDX_BO_TBL"), nRowIdx, pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strNewSpeciesKey);
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	BO_INDEX_TEMP_INSERT(pDM_BOOK, _T("IDX_TMP_BO_TBL"), strNewSpeciesKey, _T("I"));
	//=====================================================
	
	//ECOSEARCH�� ���ؼ� �������
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strNewSpeciesKey);
		pDM_BOOK->AddFrame(strQuery);
	}
	
	ids = pDM_BOOK->SendFrame();
	if (ids < 0) return -33052;		// �������� & å Insert Error
	ids = pDM_BOOK->EndFrame();
	if (ids < 0) return -33052;		// �������� & å Insert Error

	return 0;
}

INT CBO_SPECIES::BO_SPECIES_ADDBOOK(
									CString MODE,
									CESL_DataMgr *pDM_IDX_BO, 
									CESL_DataMgr *pDM_SPECIES, 
									CESL_DataMgr *pDM_BOOK, 
									CESL_DataMgr *pDM_ACCU, 
									CESL_DataMgr *pDM_APPENDIX, 
									INT nRowIdx,
									INT nSpeciesType,
									INT nAcqType)
{
	if(MODE == _T("INSERT"))
	{
		return BO_SPECIES_DUPADD_INSERT(pDM_IDX_BO, pDM_SPECIES, pDM_BOOK, pDM_ACCU, pDM_APPENDIX, nRowIdx, nSpeciesType, nAcqType);
	}
	else if(MODE == _T("UPDATE"))
	{
		return BO_SPECIES_DUPADD_UPDATE(pDM_IDX_BO, pDM_SPECIES, pDM_BOOK, pDM_ACCU, pDM_APPENDIX, nRowIdx, nSpeciesType, nAcqType);
	}
	return -1;
}

INT CBO_SPECIES::APPLY_PARENT_DM_UDF_CELL(CESL_DataMgr *pDM_INDEX, 
										  CESL_DataMgr *pDM_SPECIES, 
										  CESL_DataMgr *pDM_ACCU, 
										  CESL_DataMgr *pDM_BOOK, 
										  CESL_DataMgr *pDM_TARGET, 
										  CString FIELD_ALIAS, 
										  INT nTargetIdx, 
										  INT nSourceIdx, 
										  INT nParentType, 
										  CGroupInfo * pGroup,
										  CESL_DataMgr *pDM_TMP/*=NULL*/)
{
	CMarc marc;
	CString strQuery;
	CString strTmpData, strPriceInfo, strPrice, strCurrencyCode, strAcqYear, strGroupNo;
	DOUBLE dSpeciesPrice, dDiscRate, dDiscSpeciesPrice;
	INT nTmpData;
	if(FIELD_ALIAS == _T("UDF_å��") && pDM_BOOK)
	{
		if(nParentType == SPECIES) nTmpData = GetBookCount(pDM_BOOK);
		else if(nParentType == VOLUME) nTmpData = GetVolBookCnt(pDM_TMP, pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), nSourceIdx));
		else if(nParentType == BOOK) nTmpData = 1;
		strTmpData.Format(_T("%d"), nTmpData);
		pDM_TARGET->SetCellData(FIELD_ALIAS, strTmpData, nTargetIdx);
	}
	else if(FIELD_ALIAS == _T("UDF_��ǰå��") && pDM_BOOK)
	{
		if(nParentType == SPECIES) nTmpData = GetBookCount(pDM_BOOK, _T("Y"), _T("BB_��ǰ����"));
		else if(nParentType == VOLUME) nTmpData = GetVolBookCnt(pDM_TMP, pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), nSourceIdx), _T("BB_��ǰ����"), _T("Y"));
		strTmpData.Format(_T("%d"), nTmpData);
		pDM_TARGET->SetCellData(FIELD_ALIAS, strTmpData, nTargetIdx);
	}
	
	else if(FIELD_ALIAS == _T("UDF_�̳�å��") && pDM_BOOK)
	{
		if(nParentType == SPECIES) nTmpData = GetBookCount(pDM_BOOK, _T("N"), _T("BB_��ǰ����"));
		else if(nParentType == VOLUME) nTmpData = GetVolBookCnt(pDM_TMP, pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), nSourceIdx), _T("BB_��ǰ����"), _T("N"));
		strTmpData.Format(_T("%d"), nTmpData);
		pDM_TARGET->SetCellData(FIELD_ALIAS, strTmpData, nTargetIdx);
	}
	else if( FIELD_ALIAS == _T("UDF_�׷��") && pDM_BOOK)
	{
		if(pGroup)
			pDM_TARGET->SetCellData(FIELD_ALIAS, pGroup->GetGroupInfo(), nTargetIdx);
		else
			pDM_TARGET->SetCellData(FIELD_ALIAS, _T(""), nTargetIdx);
	}
	else if(FIELD_ALIAS == _T("UDF_����") && pDM_SPECIES)
	{
		// 2004.09.08 row���� �׻� 0 �̾ ���ݰ��� �߸� �����־� �����Ͽ���.
		// strTmpData = pDM_SPECIES->GetCellData(_T("BS_MARC"), 0);
		strTmpData = pDM_SPECIES->GetCellData(_T("BS_MARC"), nSourceIdx);
		pMgr->m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		pMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strPriceInfo);
		strPriceInfo.TrimLeft();			strPriceInfo.TrimRight();
		Apply_MARC_950(strPriceInfo, strPrice, strCurrencyCode);
		pDM_TARGET->SetCellData(FIELD_ALIAS, strPrice, nTargetIdx);
	}
	else if(FIELD_ALIAS == _T("UDF_ȭ�󱸺�") && pDM_SPECIES)
	{
		// 2004.09.08 row���� �׻� 0 �̾ ���ݰ��� �߸� �����־� �����Ͽ���.
		// strTmpData = pDM_SPECIES->GetCellData(_T("BS_MARC"), 0);
		strTmpData = pDM_SPECIES->GetCellData(_T("BS_MARC"), nSourceIdx);
		pMgr->m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		pMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strPriceInfo);
		strPriceInfo.TrimLeft();			strPriceInfo.TrimRight();
		Apply_MARC_950(strPriceInfo, strPrice, strCurrencyCode);
		pDM_TARGET->SetCellData(FIELD_ALIAS, strCurrencyCode, nTargetIdx);
	}
	else if(FIELD_ALIAS == _T("UDF_������") && pDM_BOOK)
	{
		pDM_TARGET->SetCellData(FIELD_ALIAS, GetTotalPrice(pDM_BOOK), nTargetIdx);
	}
	else if(FIELD_ALIAS == _T("UDF_����������") && pDM_BOOK && pDM_ACCU)
	{
		dSpeciesPrice = (DOUBLE)atof(GetTotalPrice(pDM_BOOK).GetBuffer(0));
		dDiscRate = (DOUBLE)atof((pDM_ACCU->GetCellData(_T("BP_������"), 0)).GetBuffer(0));
		dDiscSpeciesPrice = dSpeciesPrice - (dSpeciesPrice*(dDiscRate/100));
		strTmpData.Format(_T("%.0f"), dDiscSpeciesPrice);
		pDM_TARGET->SetCellData(FIELD_ALIAS, strTmpData, nTargetIdx);
	}	
	return 0;
}

INT CBO_SPECIES::APPLY_505TAG_TO_BOOK(CESL_DataMgr *pDM_BOOK, CMarc *pMarc)
{
	CString strData;
	CArray<CString, CString&> arrVOL, arrVOL_TITLE, arrVOL_AUTHOR;
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("505$n"), strData, &arrVOL);
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("505$t"), strData, &arrVOL_TITLE);
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("505$d"), strData, &arrVOL_AUTHOR);
	if(arrVOL.GetSize() < 2) return 0;

	INT nRowCnt = pDM_BOOK->GetRowCount();
	INT nVolCnt = arrVOL.GetSize();
	INT nRow = -1;
	CString strVolSortNo;

	for(INT i=0 ; i<nVolCnt ; i++)
	{
		strVolSortNo.Format(_T("%d"), (i+1)*10);
		if(i <= nRowCnt-1) 
		{
			pDM_BOOK->SetCellData(_T("BB_�����"), arrVOL.GetAt(i), i);
			pDM_BOOK->SetCellData(_T("BB_������ηϸ�"), arrVOL_TITLE.GetAt(i), i);
			pDM_BOOK->SetCellData(_T("BB_������"), arrVOL_AUTHOR.GetAt(i), i);
			pDM_BOOK->SetCellData(_T("BB_���Ϸù�ȣ"), strVolSortNo, i);	
		}
		else
		{
			pDM_BOOK->InsertRow(-1);
			nRow = pDM_BOOK->GetRowCount() - 1;
			this->CopyDMRowToDMByAlias(pDM_BOOK, 0, pDM_BOOK, nRow);
			pDM_BOOK->SetCellData(_T("BB_�����"), arrVOL.GetAt(i), i);
			pDM_BOOK->SetCellData(_T("BB_������ηϸ�"), arrVOL_TITLE.GetAt(i), i);
			pDM_BOOK->SetCellData(_T("BB_������"), arrVOL_AUTHOR.GetAt(i), i);
			pDM_BOOK->SetCellData(_T("BB_���Ϸù�ȣ"), strVolSortNo, i);	
		}
	}
	
	return 0;
}

INT CBO_SPECIES::APPLY_049TAG_TO_BOOK(CESL_DataMgr *pDM_BOOK, CMarc *pMarc)
{
	CString strData;
	CArray<CString, CString&> arrCopyCode;
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("049$c"), strData, &arrCopyCode);
	
	INT nBookCnt = arrCopyCode.GetSize();
	INT nRowCnt = pDM_BOOK->GetRowCount();
	INT nRow = -1;
	for(INT i=0 ; i<nBookCnt ; i++)
	{
		if(i <= nRowCnt) pDM_BOOK->SetCellData(_T("BB_������ȣ"), arrCopyCode.GetAt(i), i);
		else
		{
			pDM_BOOK->InsertRow(-1);
			nRow = pDM_BOOK->GetRowCount()-1;
			this->CopyDMRowToDMByAlias(pDM_BOOK, 0, pDM_BOOK, nRow);
			pDM_BOOK->SetCellData(_T("BB_������ȣ"), arrCopyCode.GetAt(i), i);
		}
	}

	return 0;
}

INT CBO_SPECIES::SetBookSerialNo(CESL_DataMgr *pDM_BOOK, CGroupInfo *pGroup)
{
	INT nNum, nRowCnt;
	if(pGroup) 
		nNum = pGroup->GetBookLastSerialNo();
	nRowCnt = pDM_BOOK->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{	
		if(pGroup && 
			(	pGroup->GetGroupNo().IsEmpty() || 
				(pGroup->GetGroupNo() == _T("0") && pGroup->GetGroupInfo() == _T("�������")) ||
				(pGroup->GetGroupNo() != _T("0") && pGroup->GetGroupInfo() != _T("�������"))
			))
		{
			strTmpData.Format(_T("%d"), ++nNum);
		}
		else strTmpData = _T("esl_seq.NEXTVAL");
		pDM_BOOK->SetCellData(_T("BB_�Ϸù�ȣ"), strTmpData, i);
	}	
	if(pGroup)
		pGroup->SetBookLastSerialNo(nNum);
	return 0;
}

CString CBO_SPECIES::GetTotalPrice(CESL_DataMgr *pDM_BOOK, CString strFlag/*=_T("")*/)
{
	INT nRowCnt = pDM_BOOK->GetRowCount();
	CString strTmpData;
	// 2004.09.14
	// INT nTotalPrice = 0;
	DOUBLE nTotalPrice = 0;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_BOOK->GetCellData(_T("BB_����"), i);
		if(!strTmpData.IsEmpty())
		{
			if(!strFlag.IsEmpty() && pDM_BOOK->GetCellData(_T("BB_��ǰ����"), i) == strFlag) 
				// 2004.09.14
				//nTotalPrice += _ttoi(strTmpData.GetBuffer(0));
				  nTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			else if(strFlag.IsEmpty())
				// 2004.09.14	
				//nTotalPrice += _ttoi(strTmpData.GetBuffer(0));
				  nTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
		}
	}
	// 2004.09.14
	//strTmpData.Format(_T("%d"), nTotalPrice);
	strTmpData.Format(_T("%.2f"), nTotalPrice);

	return strTmpData;
}

INT CBO_SPECIES::SetIndexDM(CString strKey)
{
	CString strDMAlias, strDMPK;
	strDMAlias = _T("DMUP_����_��_��������");
	m_pDM_IDX_BO = new CESL_DataMgr;
	m_pDM_IDX_BO->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
	m_pDM_IDX_BO->InitDMPKFromFile(_T("DMUP_����_��_��������"));
	m_pDM_IDX_BO->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);	

	CString strOption;
	strOption.Format(_T("REC_KEY=%s"), strKey);
	return m_pDM_IDX_BO->RefreshDataManager(strOption);
}

INT CBO_SPECIES::MakeFurnishData(CESL_DataMgr *pDM_OUT, CESL_DataMgr *pDM_IDX_BO, CESL_DataMgr * pDM_ACCU)
{
	CString strApplicantName, strApplicantDate, strFurnishKey, strAcqCode;
	strApplicantName = pDM_IDX_BO->GetCellData(_T("���Խ�û��"), 0);
	strApplicantDate = pDM_IDX_BO->GetCellData(_T("���Խ�û��"), 0);
	if(strApplicantDate.IsEmpty() || strApplicantName.IsEmpty()) return 0;

	CESL_DataMgr * pDM_FURNISH = NULL;
	pDM_FURNISH = new CESL_DataMgr;
	pDM_FURNISH->SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);
	pDM_FURNISH->InitDMPKFromFile(_T("DM_��ġ����ڷ����"));
	pDM_FURNISH->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);

	pDM_FURNISH->InsertRow(-1);
	pDM_FURNISH->SetCellData(_T("��û��"), strApplicantName, 0);
	pDM_FURNISH->SetCellData(_T("��û����"), strApplicantDate, 0);
	pDM_FURNISH->SetCellData(_T("��ġ����"), _T("2"), 0);
	pDM_FURNISH->SetCellData(_T("��ǥ��"), pDM_IDX_BO->GetCellData(_T("IBS_��ǥ��"), 0), 0);
	pDM_FURNISH->SetCellData(_T("������"), pDM_IDX_BO->GetCellData(_T("IBS_������"), 0), 0);
	pDM_FURNISH->SetCellData(_T("������"), pDM_IDX_BO->GetCellData(_T("IBS_������"), 0), 0);
	pDM_FURNISH->SetCellData(_T("��ǥ���ε���"), pDM_IDX_BO->GetCellData(_T("IBS_�˻��뺻ǥ��"), 0), 0);
	pDM_FURNISH->SetCellData(_T("�������ε���"), pDM_IDX_BO->GetCellData(_T("IBS_�˻��������"), 0), 0);
	pDM_FURNISH->SetCellData(_T("�������ε���"), pDM_IDX_BO->GetCellData(_T("IBS_�˻���������"), 0), 0);
	pDM_FURNISH->SetCellData(_T("�����"), pDM_IDX_BO->GetCellData(_T("IBS_�����"), 0), 0);
	pDM_FURNISH->SetCellData(_T("�����۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
	pDM_FURNISH->SetCellData(_T("�������۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
	pDM_FURNISH->SetCellData(_T("����"), pDM_ACCU->GetCellData(_T("BP_����"), 0), 0);
	pDM_FURNISH->MakeRecKey(strFurnishKey);
	pDM_FURNISH->SetCellData(_T("��ġ���KEY"), strFurnishKey, 0);
	pDM_FURNISH->SetCellData(_T("��������KEY"), pDM_ACCU->GetCellData(_T("BP_��������KEY"), 0), 0);
	if(pDM_ACCU->TBL_NAME == _T("BO_PURCHASEINFO_TBL")) strAcqCode = _T("1");
	else if(pDM_ACCU->TBL_NAME == _T("BO_DONINFO_TBL")) strAcqCode = _T("2");
	pDM_FURNISH->SetCellData(_T("���Ա���"), strAcqCode, 0);	

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	INT ids = pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_FURNISH, pDM_FURNISH->TBL_NAME, 0, pDM_OUT);

	// KOL.UDF.022 ��ť���ڵ� ����
	if(&pDM_FURNISH)
	//if(pDM_FURNISH)
	{
		delete pDM_FURNISH;
		pDM_FURNISH = NULL;
	}
	if(ids < 0) return -1;
	return 1;
}

INT CBO_SPECIES::GetVolBookCnt(CESL_DataMgr *pDM_BOOK, CString strVolSortNo, CString strAlias/*=_T("")*/, CString strData/*=_T("")*/)
{
	INT nCnt = 0;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(!IsBook(pDM_BOOK, i)) continue;
		if(pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i) != strVolSortNo) continue;
		if(!strAlias.IsEmpty() && pDM_BOOK->GetCellData(strAlias, i) != strData) continue;
		nCnt++;
	}
	return nCnt;
}

INT CBO_SPECIES::SynchronizedIndexAndBookData(CString strSpeciesKey, CESL_DataMgr * pDM_OUT)
{
	CString strQuery;
	strQuery.Format(_T("update idx_bo_tbl set USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
										  _T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
										  _T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
										  _T("IDX_VOL_TITLE=GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX'), ")
										  _T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
										  _T("IDX_VOL=GET_BO_BOOK_INFO(%s,'VOL_INDEX'), ")
										  _T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE'), ")
										  _T("LAST_WORK='%s' where rec_key=%s;"), 
										  strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, pMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
	pDM_OUT->AddFrame(strQuery);	
	return 0;
}

INT CBO_SPECIES::DeleteAccuInfo(CESL_DataMgr *pDM_OUT, CString strAccuKey, INT nAcqType)
{
	CString strQuery, strTmpData, strPrice, strDiscRate, strSpeciesKey;
	if(strAccuKey == _T("ALL"))
	{
		strSpeciesKey = pDM_OUT->GetCellData(_T("BS_��KEY"), 0);
		if(strSpeciesKey.IsEmpty())
			return -1;
	}

	if(nAcqType == 0) return 0;
	else if(nAcqType == 1) SetAccuDM(TRUE);
	else if(nAcqType == 2) SetAccuDM(FALSE);
	if(strAccuKey == _T("ALL"))
		strQuery.Format(_T("SPECIES_KEY = %s"), strSpeciesKey);
	else
		strQuery.Format(_T("REC_KEY = %s"), strAccuKey);
	m_pDM_ACCU->RefreshDataManager(strQuery);
	if(m_pDM_ACCU->GetRowCount() == 0) return 0;
	
	if(strAccuKey == _T("ALL"))
		strQuery.Format(
			_T("DELETE FROM %s WHERE SPECIES_KEY=%s;"),
			m_pDM_ACCU->TBL_NAME, strSpeciesKey);
	else
		strQuery.Format(
			_T("DELETE FROM %s WHERE REC_KEY=%s;"),
			m_pDM_ACCU->TBL_NAME, strAccuKey);

	pDM_OUT->AddFrame(strQuery);
	return 0;	
}

INT CBO_SPECIES::Restruct049MarcTag(CESL_DataMgr *pDM_BOOK, CMarc *pMarc)
{
	BOOL bSameSeShelfCode = FALSE;
	bSameSeShelfCode = IsSameSeShelfCode(pDM_BOOK);
	pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("049"));
	pDM_BOOK->SORT(_T("BB_��Ϲ�ȣ"));
	CString strRegNo, strVol, strSeShelfCode, strCopyCode, str049;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strCopyCode = pDM_BOOK->GetCellData(_T("BB_������ȣ"), i);
		strRegNo = pDM_BOOK->GetCellData(_T("BB_��Ϲ�ȣ"), i);
		strVol = pDM_BOOK->GetCellData(_T("BB_�����"), i);
		strSeShelfCode = pDM_BOOK->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), i);
		if(strRegNo.IsEmpty()) continue;
		pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$l"), strRegNo);
		if(!strVol.IsEmpty())
			pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$v"), strVol);
		if(!bSameSeShelfCode && !strSeShelfCode.IsEmpty())
			pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$f"), strSeShelfCode);
		if(!strCopyCode.IsEmpty())
			pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$c"), strCopyCode);
	}
	
	if(bSameSeShelfCode)
	{
		strSeShelfCode = pDM_BOOK->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), 0);
		if(!strSeShelfCode.IsEmpty())
			pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$f"), strSeShelfCode);
		pMgr->m_pInfo->pMarcMgr->GetField( pMarc, _T("049"), str049 );
		if (!str049.IsEmpty()) {
			str049 = _T("0") + str049.Mid(1);
		}
		pMgr->m_pInfo->pMarcMgr->DeleteField( pMarc, _T("049") );
		pMgr->m_pInfo->pMarcMgr->InsertField( pMarc, _T("049") + str049);
	}
	else
	{
		pMgr->m_pInfo->pMarcMgr->GetField( pMarc, _T("049"), str049 );
		if (!str049.IsEmpty()) {
			str049 = _T("1") + str049.Mid(1);
		}
		pMgr->m_pInfo->pMarcMgr->DeleteField( pMarc, _T("049") );
		pMgr->m_pInfo->pMarcMgr->InsertField( pMarc, _T("049") + str049);
	}

	pMgr->m_pInfo->pMarcMgr->GetField(pMarc, _T("049"), str049);
	return 0;	
}

INT CBO_SPECIES::Restruct020MarcTag(CESL_DataMgr *pDM_BOOK, CMarc *pMarc)
{
	//��DM����
	CESL_DataMgr * pDM_VOLUME = new CESL_DataMgr;
	pDM_VOLUME->SetCONNECTION_INFO(pDM_BOOK->CONNECTION_INFO);
	pDM_VOLUME->InitDMPKFromFile(pDM_BOOK->DB_MGR_ALIAS); 
	pDM_VOLUME->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	//å�� ������ �ٲ�
	CONVERT_BOOK_to_VOLUME(pDM_BOOK, pDM_VOLUME);
	pDM_VOLUME->SORT(_T("BB_���Ϸù�ȣ"));
	SetISBN_to_Marc_ByVolume(pMarc, pDM_VOLUME, _T("020"), _T("  "));

	if(pDM_VOLUME)
	{
		delete pDM_VOLUME;
		pDM_VOLUME = NULL;
	}
	return 0;
}

INT CBO_SPECIES::Restruct505MarcTag(CESL_DataMgr *pDM_BOOK, CMarc *pMarc)
{
	//��DM����
	CESL_DataMgr * pDM_VOLUME = new CESL_DataMgr;
	pDM_VOLUME->SetCONNECTION_INFO(pDM_BOOK->CONNECTION_INFO);
	pDM_VOLUME->InitDMPKFromFile(pDM_BOOK->DB_MGR_ALIAS); 
	pDM_VOLUME->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	//å�� ������ �ٲ�
	CONVERT_BOOK_to_VOLUME(pDM_BOOK, pDM_VOLUME);
	pDM_VOLUME->SORT(_T("BB_���Ϸù�ȣ"));

	pMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("505"));
	INT nRowCnt = pDM_VOLUME->GetRowCount();
	CString strVol, strVolTitle, strVolAuthor, strTmpData, str505;
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strVolAuthor = pDM_VOLUME->GetCellData(_T("BB_������"), i);
		strVol = pDM_VOLUME->GetCellData(_T("BB_�����"), i);
		strVolTitle = pDM_VOLUME->GetCellData(_T("BB_������ηϸ�"), i);

		strTmpData = Get505Field(strVol, strVolTitle, strVolAuthor);
		if(!strTmpData.IsEmpty())
		{
			if(i != nRowCnt-1) strTmpData += _T(". -");
			str505 += strTmpData;
		}
	}
	if(!str505.IsEmpty())
	{
		str505 = _T("50510") + str505;
		pMgr->m_pInfo->pMarcMgr->InsertField(pMarc, str505);
	}
	if(pDM_VOLUME)
	{
		delete pDM_VOLUME;
		pDM_VOLUME = NULL;
	}
	return 0;
}

BOOL CBO_SPECIES::IsSameSeShelfCode(CESL_DataMgr *pDM_BOOK)
{
	if(pDM_BOOK == NULL) return FALSE;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	if(nRowCnt < 2) return TRUE;

	CString strSeShelfCode, strTmpData;
	strSeShelfCode = pDM_BOOK->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), 0);
	strSeShelfCode.TrimLeft();	strSeShelfCode.TrimRight();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_BOOK->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), i);
		strTmpData.TrimLeft();	strTmpData.TrimRight();
		if(strSeShelfCode != strTmpData) return FALSE;
	}
	return TRUE;
}

CString CBO_SPECIES::Get505Field(CString strVol, CString strVolTitle, CString strVolAuthor)
{
	CStringArray arrVolTitle;
	CStringArray arr505_g;
	CStringArray arr505_e;

	CString str505;
	if(strVol.IsEmpty() || strVolTitle.IsEmpty()) return _T("");
	str505.Format(_T("$n%s"), strVol);
	ParsingString(_T(";"), strVolTitle, &arrVolTitle, FALSE);
	INT nCnt = arrVolTitle.GetSize();
	for(INT i=0 ; i<nCnt ; i++)
	{
		str505 += _T("$t") + arrVolTitle.GetAt(i);
	}
	ParsingParenthesis(strVolAuthor, &arr505_g, FALSE);
	ParsingString(_T(";"), strVolAuthor, &arr505_e, FALSE);
	nCnt = arr505_e.GetSize();
	for(i=0; i<nCnt ; i++)
	{
		if(i==0) str505 += _T("$d") + arr505_e.GetAt(i);
		else str505 += _T("$e") + arr505_e.GetAt(i);
	}
	nCnt = arr505_g.GetSize();
	for(i=0 ; i<nCnt ; i++)
	{
		str505 += _T("$g") + arr505_g.GetAt(i);
	}

	return str505;
}

INT CBO_SPECIES::ParsingParenthesis(CString &strSource, CStringArray *pArrData, BOOL bIndexing/*=TRUE*/)
{
	pArrData->RemoveAll();
	CString strData;
	INT nStratPos = strSource.Find(_T("("));
	INT nEndPos = strSource.Find(_T(")"));
	while(nStratPos >= 0 && nEndPos > 0)
	{
		strData = strSource.Mid(nStratPos, nEndPos-nStratPos+1);
		if(!strData.IsEmpty())
		{
			strSource.Replace(_T("(")+strData+_T(")"), _T(""));
			if(bIndexing) ((CIndexTool*)pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strData);
			pArrData->Add(strData);
		}
		nStratPos = strSource.Find(_T("("));
		nEndPos = strSource.Find(_T(")"));
	}
	strSource.Replace(_T("("), _T(""));
	strSource.Replace(_T(")"), _T(""));
	return 0;
}

INT CBO_SPECIES::RestructBookBy049MarcTag(CESL_DataMgr *pDM_BOOK, CMarc *pMarc, BOOL b049InfoInsertYN/*=FALSE*/)
{
	BOOL bSameShelfCode = FALSE;
	CString str049, strTmpData, strInd, strVol, strRegNo, strCopyCode, strRepresentShelfCode, strShelfCode, strRegCode;
	pMgr->m_pInfo->pMarcMgr->GetField(pMarc, _T("049"), str049);
	if(str049.IsEmpty()) return 0;

	str049.Replace(SUBFIELD_CODE, '$');
	str049.Replace(FIELD_TERMINATOR, ' ');
	str049.Replace(_T("$l"), _T("#$l"));
	CStringArray arr049;	arr049.RemoveAll();
	ParsingString(_T("#"), str049, &arr049, FALSE);
	INT n049Cnt = arr049.GetSize();
	INT n049SubFieldCnt = -1;
	INT nRegNo;
	for(INT i=0 ; i<n049Cnt ; i++)
	{
		CStringArray arr049SubField;	arr049SubField.RemoveAll();
		strTmpData = arr049.GetAt(i);
		if(strTmpData.Left(1) == _T("0"))
		{
			bSameShelfCode = TRUE;
			continue;
		}
		else if(strTmpData.Left(1) == _T("1"))
		{
			continue;
		}
		ParsingString(_T("$"), strTmpData, &arr049SubField, FALSE);
		n049SubFieldCnt = arr049SubField.GetSize();
		strVol.Empty();	strCopyCode.Empty();	strShelfCode.Empty();	strRegNo.Empty();
		for(INT j=0 ; j<n049SubFieldCnt ; j++)
		{
			strTmpData = arr049SubField.GetAt(j);
			strTmpData.TrimLeft();	strTmpData.TrimRight();
			strInd = strTmpData.Left(1);
			strInd.MakeLower();
			if(strInd == _T("v")) strVol = strTmpData.Mid(1);
			else if(strInd == _T("c")) strCopyCode = strTmpData.Mid(1);
			else if(strInd == _T("f"))
			{
				strShelfCode = strTmpData.Mid(1);
				if(!strShelfCode.IsEmpty()) strRepresentShelfCode = strShelfCode;
			}
			else if(strInd == _T("l")) 
			{	
				strTmpData = strTmpData.Mid(1);
				strRegCode = strTmpData.Left(2);
				if(strTmpData.GetLength() >= 3)
				{
					nRegNo = _ttoi(strTmpData.Mid(2).GetBuffer(0));
					strRegNo.Format(_T("%s%010d"), strRegCode, nRegNo);
				} 
			}
		}
		Set049ToBookDM(pDM_BOOK, strRegNo, strVol, strCopyCode, strShelfCode, b049InfoInsertYN, i-1);
	}
	//0490�ϰ�� ��ǥ ��ġ��ȣ�� ��� å�� �����Ѵ�.
	if(bSameShelfCode)
	{
		for(i=0 ; i<n049Cnt ; i++)
		{
			pDM_BOOK->SetCellData(_T("BB_��ġ��ȣ_�ڵ�"), strRepresentShelfCode, i);
		}
	}

	return 0;
}

INT CBO_SPECIES::Set049ToBookDM(CESL_DataMgr *pDM_BOOK, CString strRegNo, CString strVol, CString strCopyCode, CString strShelfCode, BOOL b049InfoInsertYN/*=FALSE*/, INT nIdx/*=-1*/)
{
	INT nRowCnt = pDM_BOOK->GetRowCount();
	if(b049InfoInsertYN)
	{
		if( nIdx > nRowCnt-1)
		{
			pDM_BOOK->InsertRow(-1);
			nIdx = pDM_BOOK->GetRowCount()-1;
			CopyDMRowToDMByAlias(pDM_BOOK, 0, pDM_BOOK, nIdx);
		}
		
		pDM_BOOK->SetCellData(_T("BB_��ϱ���_�ڵ�"), strRegNo.Left(2), nIdx);
		pDM_BOOK->SetCellData(_T("BB_��Ϲ�ȣ"), strRegNo, nIdx);
		pDM_BOOK->SetCellData(_T("BB_�����"), strVol, nIdx);
		pDM_BOOK->SetCellData(_T("BB_��å��ȣ"), strVol, nIdx);
		pDM_BOOK->SetCellData(_T("BB_������ȣ"), strCopyCode, nIdx);
		pDM_BOOK->SetCellData(_T("BB_��ġ��ȣ_�ڵ�"), strShelfCode, nIdx);
		return 0;
	}
	else
	{
		CString strTmpData;
		for(INT i=0 ; i<nRowCnt ; i++)
		{
			strTmpData = pDM_BOOK->GetCellData(_T("BB_��Ϲ�ȣ"), i);
			if(strTmpData == strRegNo)
			{
				pDM_BOOK->SetCellData(_T("BB_�����"), strVol, i);
				pDM_BOOK->SetCellData(_T("BB_��å��ȣ"), strVol, i);
				pDM_BOOK->SetCellData(_T("BB_������ȣ"), strCopyCode, i);
				pDM_BOOK->SetCellData(_T("BB_��ġ��ȣ_�ڵ�"), strShelfCode, i);
				return 0;
			}
		}
	}

	return 0;
}

INT CBO_SPECIES::CopyDataMerge(CESL_DataMgr *pSourceBookDM, CESL_DataMgr *pDuplicateBookDM, CMarc *pDupMarc)
{
	if(pSourceBookDM == NULL || pDuplicateBookDM == NULL || pDupMarc == NULL) return -1;

	//�����ڷ��� å������ �����ڷ��� å������ �߰��Ѵ�.
	//�����ڷ��� ������ȣ�� �ʱ�ȭ �Ѵ�.
	CString strDupKey, strSrcKey, strVol, strVolSortNo;
	strDupKey = pDuplicateBookDM->GetCellData(_T("BB_��KEY"), 0);
	strSrcKey = pSourceBookDM->GetCellData(_T("BB_��KEY"), 0);
	INT nRowCnt = pSourceBookDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		pSourceBookDM->SetCellData(_T("BB_��KEY"), strDupKey, i);
		pSourceBookDM->SetCellData(_T("BB_������KEY"), strSrcKey, i);
		pSourceBookDM->SetCellData(_T("BB_������ȣ"), _T(""), i);
		pSourceBookDM->SetCellData(_T("BB_��ǥå����"), _T("N"), i);
	}

	//������ȣ�� �ο��ϱ� ���ؼ� ��Ϲ�ȣ�� Sort�Ѵ�.
	pSourceBookDM->SORT(_T("BB_��Ϲ�ȣ"));
	pDuplicateBookDM->SORT(_T("BB_��Ϲ�ȣ"));
	CString strRegNo, strRegCode, strSeShelfCode, strTmpData;
	INT nLastCopyCode, nCurRow;
	CCopyCodeMgr m_cCopyCodeMgr;
	m_cCopyCodeMgr.Init(pSourceBookDM->CONNECTION_INFO);	
	for(i=0 ; i<nRowCnt ; i++)
	{
		//�����ڷ� å ������ ���Ϸù�ȣ �ο��Ѵ�.
		strVol = pSourceBookDM->GetCellData(_T("BB_�����"), i);
		strVolSortNo = FindVolSortNo(pDuplicateBookDM, strVol);
		pSourceBookDM->SetCellData(_T("BB_���Ϸù�ȣ"), strVolSortNo, i);
		//�����ڷ��� ������ ����
		if(CopySameVolInfo(pDuplicateBookDM, pSourceBookDM, i, strVolSortNo) < 0)
			return -1;
		//�����ڷ� å ������ ������ȣ�� �ο��Ѵ�.
		strRegNo = pSourceBookDM->GetCellData(_T("BB_��Ϲ�ȣ"), i);
		strRegCode = pSourceBookDM->GetCellData(_T("BB_��ϱ���_�ڵ�"), i);
		strSeShelfCode = pSourceBookDM->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), i);
		strVol = pSourceBookDM->GetCellData(_T("BB_�����"), i);
		m_cCopyCodeMgr.SetSrcInfo(strVol, strRegCode, strSeShelfCode);
		nLastCopyCode = GetLastDupCodeNumber(pDuplicateBookDM, &m_cCopyCodeMgr, -1, -1, strRegNo);
		if(nLastCopyCode == 0) strTmpData = _T("");
		else strTmpData.Format(_T("%d"), ++nLastCopyCode);
		pSourceBookDM->SetCellData(_T("BB_������ȣ"), strTmpData, i);
		pDuplicateBookDM->InsertRow(-1);
		nCurRow = pDuplicateBookDM->GetRowCount()-1;
		CopyDMRowToDMByAlias(pSourceBookDM, i, pDuplicateBookDM, nCurRow);
	}
	pSourceBookDM->FreeData();
	
	Restruct049MarcTag(pDuplicateBookDM, pDupMarc);
	Restruct020MarcTag(pDuplicateBookDM, pDupMarc);
	return 0;
}

INT CBO_SPECIES::CopySameVolInfo(CESL_DataMgr *pDM_From, CESL_DataMgr *pDM_To, INT nToIdx, CString strVolSortNo)
{
	if (pDM_From == NULL || pDM_To == NULL) return -1;
	if(nToIdx < 0 || nToIdx >= pDM_To->GetRowCount()) return -1;
	CString strTmpData;
	
	INT nRowCnt, nFromIdx;
	nRowCnt = pDM_From->GetRowCount(); 
	nFromIdx = -1;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_From->GetCellData(_T("BB_���Ϸù�ȣ"), i);
		if(strTmpData == strVolSortNo)
		{
			nFromIdx = i;
		}
	}
	if(nFromIdx < 0) return 0;
	
	pDM_To->SetCellData(_T("BB_��ü����_�ڵ�"), pDM_From->GetCellData(_T("BB_��ü����_�ڵ�"), nFromIdx), nToIdx);
	pDM_To->SetCellData(_T("BB_������ηϸ�"), pDM_From->GetCellData(_T("BB_������ηϸ�"), nFromIdx), nToIdx);
	pDM_To->SetCellData(_T("BB_������ηϸ����"), pDM_From->GetCellData(_T("BB_������ηϸ����"), nFromIdx), nToIdx);
	pDM_To->SetCellData(_T("BB_������"), pDM_From->GetCellData(_T("BB_������"), nFromIdx), nToIdx);
	pDM_To->SetCellData(_T("BB_�����"), pDM_From->GetCellData(_T("BB_�����"), nFromIdx), nToIdx);
	pDM_To->SetCellData(_T("BB_����ISBN"), pDM_From->GetCellData(_T("BB_����ISBN"), nFromIdx), nToIdx);
	pDM_To->SetCellData(_T("BB_�����"), pDM_From->GetCellData(_T("BB_�����"), nFromIdx), nToIdx);
	pDM_To->SetCellData(_T("BB_������Ư��"), pDM_From->GetCellData(_T("BB_������Ư��"), nFromIdx), nToIdx);
	pDM_To->SetCellData(_T("BB_ũ��"), pDM_From->GetCellData(_T("BB_ũ��"), nFromIdx), nToIdx);
	pDM_To->SetCellData(_T("BB_�����ڷ�"), pDM_From->GetCellData(_T("BB_�����ڷ�"), nFromIdx), nToIdx);
	pDM_To->SetCellData(_T("BB_����"), pDM_From->GetCellData(_T("BB_����"), nFromIdx), nToIdx);
	pDM_To->SetCellData(_T("BB_����������"), pDM_From->GetCellData(_T("BB_����������"), nFromIdx), nToIdx);
	pDM_To->SetCellData(_T("BB_���ݼ���"), pDM_From->GetCellData(_T("BB_���ݼ���"), nFromIdx), nToIdx);
	return 0;
}

CString CBO_SPECIES::FindVolSortNo(CESL_DataMgr *pDM_TargetBOOK, CString strVol, INT nEndPos/*=-1*/)
{
	CString strTmpData, strVolSortNo;
	
	INT nRowCnt;
	if(nEndPos >= 0) nRowCnt = nEndPos;
	else nRowCnt = pDM_TargetBOOK->GetRowCount(); 
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_TargetBOOK->GetCellData(_T("BB_�����"), i);
		if(strTmpData == strVol)
		{
			strVolSortNo = pDM_TargetBOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i);
			return strVolSortNo;
		}
	}
	INT nSortNo = ((GetMaxVolSortNo(pDM_TargetBOOK)/10)+1)*10;
	strVolSortNo.Format(_T("%d"), nSortNo);
	return strVolSortNo;
}

INT CBO_SPECIES::GetLastDupCodeNumber(CESL_DataMgr *pDM_Book, CCopyCodeMgr * pCopyCodeMgr, INT nCurIdx, INT nStartIdx, INT nEndIdx, CString strRegNo)
{
	if (pDM_Book == NULL) return -1;
	
	if(strRegNo.IsEmpty()) return 0;
	
	INT nDupNumber = 0;
	
	for (INT idx = nStartIdx; idx <= nEndIdx; idx++)
	{
		CString strTemp;
		INT nNumber = 0;
		
		// �������� �˻��Ѵ�.
		if (!pCopyCodeMgr->IsCopy(pDM_Book, idx)) continue;
		//�ڱ� �ڽ��̸� �˻����� �ʴ´�.
		if(idx == nCurIdx) continue;
		
		// ������ȣ�� �����´�.
		if(strRegNo != _T("-1"))
		{
			strTemp = pDM_Book->GetCellData(_T("BB_��Ϲ�ȣ"), idx);
			if(strTemp.IsEmpty()) continue;
		}
		
		strTemp = pDM_Book->GetCellData(_T("BB_������ȣ"), idx);
		if (strTemp.IsEmpty()) strTemp = _T("1");
		
		nNumber = _ttoi(strTemp.GetBuffer(0));	
		if (nNumber > nDupNumber)
			nDupNumber = nNumber;
	}
	
	return nDupNumber;
}

INT CBO_SPECIES::GetLastDupCodeNumber(CESL_DataMgr *pDM_Book, CCopyCodeMgr * pCopyCodeMgr, INT nCurIdx, INT nEndIdx/*=-1*/, CString strRegNo/*=_T("-1")*/)
{
	if (pDM_Book == NULL) return -1;

	INT nRowCount;
	if(nEndIdx > -1) 
		nRowCount = nEndIdx;
	else
		nRowCount = pDM_Book->GetRowCount();
	
	return GetLastDupCodeNumber(pDM_Book, pCopyCodeMgr, nCurIdx, 0, nRowCount-1, strRegNo);
}

INT CBO_SPECIES::CopyDataDivide(CESL_DataMgr *pSourceBookDM, CESL_DataMgr *pDuplicateBookDM, CMarc *pMarc, CString strSrcKey)
{
	//�����ڷ� å�������� DUP_SPECIES_KEY�� �����ڷ� ��KEY�� ���� row�� �����Ѵ�.
	CString strTmpData;
	INT nRowCnt = pDuplicateBookDM->GetRowCount();
	INT nCurRow;
	for(INT i=nRowCnt-1 ; i>=0 ; i--)
	{
		pDuplicateBookDM->GetCellData(_T("BB_������KEY"), i, strTmpData);
		if(strTmpData == strSrcKey)
		{
			pDuplicateBookDM->SetCellData(_T("BB_��KEY"), strTmpData, i);
			pDuplicateBookDM->SetCellData(_T("BB_������KEY"), _T(""), i);
			pSourceBookDM->InsertRow(-1);
			nCurRow = pSourceBookDM->GetRowCount()-1;
			CopyDMRowToDMByAlias(pDuplicateBookDM, i, pSourceBookDM, nCurRow);
			pDuplicateBookDM->DeleteRow(i);
		}
	}	
	Restruct049MarcTag(pDuplicateBookDM, pMarc);
	Restruct020MarcTag(pDuplicateBookDM, pMarc);
	return 0;
}

INT CBO_SPECIES::GetMaxVolSortNo(CESL_DataMgr * pDM_TargetBOOK)
{
	CString strTmpData;
	INT nRowCnt = pDM_TargetBOOK->GetRowCount();
	INT nVolSortNo, nTmpData;
	nVolSortNo = nTmpData = -1;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_TargetBOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i);
		nTmpData = _ttoi(strTmpData.GetBuffer(0));
		if(nVolSortNo < nTmpData) nVolSortNo = nTmpData;
	}
	return nVolSortNo;
}

INT CBO_SPECIES::BO_BOOK_DELETE(CString strSpeciesKey,
								CString strAccuKey,
								CString strBookRecKey,
								INT nSpeciesType,
								INT nAcqType,
								BOOL bRecycleYN,
								CGroupInfo * pGroup)
{
	// FROM : _T("PURCHASE") or _T("DONATE")
	// MODE : _T("DELETE") or _T("RECYCLE")
	// ���� 1 row delete -> ��Ȱ������ 1 row insert(�÷��� �������ٰ�)
	// ���������� �� ���� 
	// (å)�������� n rows delete -> ��Ȱ��å�� n rows insert(�÷��� �������ٰ�)(��Ȱ���ϰ�� ������ ����)
	
	SPECIES_TYPE STYPE = (SPECIES_TYPE)nSpeciesType;
	ACQ_TYPE ATYPE = (ACQ_TYPE)nAcqType;

	INT nRowIdx = 0;
	INT ids;
	
	CString strOption, strQuery, strKey, strRegNo, strMarc, strTmpData, strBookQuery;
	strKey = strSpeciesKey;
	if(strSpeciesKey.IsEmpty()) return -1;

	if(!CheckHaveOtherBookInfo(m_pDM_BOOK, strSpeciesKey, strAccuKey))    
	{
		if(bRecycleYN)
			ids = BO_SPECIES_RECYCLE(strSpeciesKey, strAccuKey, nAcqType, pGroup );
		else
			ids = BO_SPECIES_DELETE(strSpeciesKey, strAccuKey, nAcqType);
		return ids;
	}
	// �ش� ���� åDM�� �����Ѵ�.
	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	m_pDM_IDX_BO->RefreshDataManager(strOption);

	if(!strAccuKey.IsEmpty() && strAccuKey != _T("-1"))  
 		strOption.Format(_T("BOOK_APPENDIX_FLAG = 'B' AND ACQ_KEY = %s AND REC_KEY = '%s'"), strAccuKey, strBookRecKey);
	else
		strOption.Format(_T("BOOK_APPENDIX_FLAG = 'B' AND SPECIES_KEY = %s AND (ACQ_KEY IS NULL OR ACQ_KEY = -1) AND REC_KEY = '%s'"), strSpeciesKey, strBookRecKey);
	ids = m_pDM_BOOK->RefreshDataManager(strOption);
	if (ids < 0) return -31003;
	if(m_pDM_BOOK->GetRowCount() == 0) return 1000;
	
	strBookQuery = strOption;
	strOption.Format(_T("BOOK_KEY IN (SELECT REC_KEY FROM BO_BOOK_TBL WHERE %s)"), strBookQuery);
	ids = m_pDM_EBook->RefreshDataManager(strOption);
	// ������ EBook������ DB_FLAG�� _T("D")�� �����Ѵ�.
	ids = m_pDM_EBook->GetRowCount();
	for(INT idx=0 ; idx<ids ; idx++)
	{
		m_pDM_EBook->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}

	INT nRowCount = m_pDM_BOOK->GetRowCount();
	if (nRowCount <= 0) return -31003;

	m_pDM_BOOK->StartFrame();
	
	m_pDM_BOOK->SetCellData(_T("BB_DB_FLAG"), _T("D"), 0);
	ids = MakeBookQueryFrame(m_pDM_BOOK, strSpeciesKey, strAccuKey, m_pDM_BOOK);
	if(ids < 0) return -1;
	strMarc = UpdateBookDeleteMarc(m_pDM_BOOK, strSpeciesKey, strAccuKey, strBookRecKey);
	m_pDM_SPECIES->InsertRow(-1);	
	m_pDM_SPECIES->SetCellData(_T("BS_MARC"), strMarc, 0);
	UpdateAccuInfo(m_pDM_BOOK, m_pDM_BOOK, strAccuKey, strMarc, nAcqType, m_pDM_SPECIES->GetCellData(_T("BS_��KEY"), 0));
	m_pDM_SPECIES->SetCellData(_T("BS_MARC"), strMarc, 0);

	//�̿����ѱ���/��������/�ڷ��/��ϱ��� ���а��� ���а� concatenation
	ids = SynchronizedIndexAndBookData(strSpeciesKey, m_pDM_BOOK);

	ids = MakeEBookQueryFrame(m_pDM_EBook, m_pDM_BOOK);
	if(ids < 0) return ids;
	
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	BO_INDEX_TEMP_INSERT(m_pDM_BOOK, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("I"));
	//=====================================================
	

	//ECOSEARCH�� ���ؼ� �������
	if(pMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		m_pDM_BOOK->AddFrame(strQuery);
	}
	// ���� Ʈ���������� �������
	ids = UpdateSpeciesMarcbyStream(m_pDM_BOOK, _T("BO_SPECIES_TBL"), strSpeciesKey, m_pDM_SPECIES->GetCellData(_T("BS_MARC"), 0));	
	if(ids < 0) return -31053;
	ids = m_pDM_BOOK->SendFrame();
	if(ids < 0) return -31051;
	ids = m_pDM_BOOK->EndFrame();
	if (ids < 0) return -33052;		

	return 0;
}

CString CBO_SPECIES::UpdateBookDeleteMarc(CESL_DataMgr *pDM_BOOK, CString strSpeciesKey, CString strAcqKey, CString strBookRecKey)
{
	CString strWhere ;
	if (strAcqKey.IsEmpty() || strAcqKey == _T("-1"))
		strWhere.Format(_T("species_key = %s and (acq_key is NULL OR acq_key = -1) and REC_KEY != %s"), strSpeciesKey, strBookRecKey);   
	else
		strWhere.Format(_T("species_key = %s and acq_key=%s and REC_KEY != %s"), strSpeciesKey, strAcqKey, strBookRecKey);   
	pDM_BOOK->RefreshDataManager(strWhere);
	if(pDM_BOOK->GetRowCount() == 0) return _T("");
	
	strWhere.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY=%s"), strSpeciesKey);
	CString strMarc;
	pDM_BOOK->GetOneValueQuery(strWhere, strMarc);
	CMarc marc;
	pMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	
	// 020, 049 ��ũ�� update 
	Restruct020MarcTag(pDM_BOOK, &marc);
	Restruct049MarcTag(pDM_BOOK, &marc);

	return strMarc; 
}

INT CBO_SPECIES::MakeEBookInfoAndQueryFrame(CESL_DataMgr *pDM_BOOK, CESL_DataMgr * pDM_OUT)
{
	m_pDM_EBook = new CESL_DataMgr;
	m_pDM_EBook->SetCONNECTION_INFO(pMgr->CONNECTION_INFO);
	m_pDM_EBook->InitDMPKFromFile(_T("DMUP_����_��_EBOOK"));
	m_pDM_EBook->SetMarcMgr(pMgr->m_pInfo->pMarcMgr);
	INT ids = MakeEBookInfoAndQueryFrame(pDM_BOOK, m_pDM_EBook, pDM_OUT);
	if(m_pDM_EBook)
	{
		delete m_pDM_EBook;
		m_pDM_EBook = NULL;
	}
	if(ids < 0) return ids;
	return 0;
}

INT CBO_SPECIES::MakeEBookInfoAndQueryFrame(CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_EBook, CESL_DataMgr * pDM_OUT)
{
	if(pDM_BOOK == NULL || pDM_EBook == NULL) return -1;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	INT nCurRow, ids;
	CString strImgFile, strEBookFile, strExeFile, strFileSize, strMediaCode, strDBFlag, strKey, strQuery;
	pDM_EBook->FreeData();
	CFile file;
	CFileException fileException;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		pDM_EBook->InsertRow(-1);
		nCurRow = pDM_EBook->GetRowCount()-1;
		CopyDMRowToDMByAlias(pDM_BOOK, i, pDM_EBook, nCurRow);
		
		//EBOOK DB FLAG����
		pDM_BOOK->GetCellData(_T("BB_��ü����_�ڵ�"), i, strMediaCode);
		pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, strDBFlag);
		if(strMediaCode != _T("EB")) pDM_EBook->SetCellData(_T("BB_DB_FLAG"), _T("D"), nCurRow);
		else
		{
			if(strDBFlag == _T("D")) pDM_EBook->SetCellData(_T("BB_DB_FLAG"), _T("D"), nCurRow);
			else
			{
				pDM_EBook->GetCellData(_T("BB_EBOOKKEY"), nCurRow, strKey);
				if(strKey.IsEmpty()) pDM_EBook->SetCellData(_T("BB_DB_FLAG"), _T("I"), nCurRow);
				else pDM_EBook->SetCellData(_T("BB_DB_FLAG"), _T("U"), nCurRow);
			}

		}
		ids = SetEBookInfo(pDM_BOOK, pDM_EBook, nCurRow);
		if(ids < 0) return -1;
	}
	return MakeEBookQueryFrame(pDM_EBook, pDM_OUT);
}

INT CBO_SPECIES::UploadEBookFiles(CESL_DataMgr *pDM_EBook)
{
	if(pDM_EBook == NULL) return -1;
	INT nRowCnt = pDM_EBook->GetRowCount();
	INT ids;
	CEBookUpNDownloadMgr mgr;
	CString strDBFLAG, strFileName, strServerFileName;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strDBFLAG = pDM_EBook->GetCellData(_T("BB_DB_FLAG"), i);
		if(strDBFLAG == _T("I"))
		{
			ids = mgr.UploadEBookByDM(pDM_EBook, i);
			if(ids == -2000) 
			{
				pMgr->ESLAfxMessageBox(_T("E-BOOK ������ ������ ���ε� �� �� �����ϴ�."));
				return ids;
			}
		}
		else if(strDBFLAG == _T("U"))
		{
			ids = mgr.DeleteEBookByKeyForUpdate(pDM_EBook, i);
			if(ids < 0) 
			{
				pMgr->ESLAfxMessageBox(_T("E-BOOK ������ ������ �� �����ϴ�."));
				return ids;
			}
			ids = mgr.UploadEBookByDM(pDM_EBook, i);
			if(ids == -2000) 
			{
				pMgr->ESLAfxMessageBox(_T("E-BOOK ������ ������ ���ε� �� �� �����ϴ�."));
				return ids;
			}
		}
		if(strDBFLAG == _T("D"))
		{
			if(!pDM_EBook->GetCellData(_T("BB_EBOOKKEY"), i).IsEmpty())
			{
				ids = mgr.DeleteEBookByKey(pDM_EBook->GetCellData(_T("BB_åKEY"), i), pDM_EBook->CONNECTION_INFO);
				if(ids == -1000)
				{
					pMgr->ESLAfxMessageBox(_T("E-BOOK ������ �������� ���� �� �� �����ϴ�."));
					return ids;
				}
			}
		}
	}
	return 0;
}

INT CBO_SPECIES::SetEBookDMDBFlag(CESL_DataMgr *pDM_EBook, CString strBookKey, CString strDBFlag)
{
	if(pDM_EBook == NULL) return -1;
	INT nRowCnt = pDM_EBook->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_EBook->GetCellData(_T("BB_åKEY"), i);
		if(strTmpData == strBookKey)
			pDM_EBook->SetCellData(_T("BB_DB_FLAG"), strDBFlag, i);
	}
	return 0;
}

INT CBO_SPECIES::MakeEBookQueryFrame(CESL_DataMgr *pDM_EBook, CESL_DataMgr * pDM_OUT)
{
	if(pDM_EBook == NULL || pDM_OUT == NULL) return -1;
	// ������ ȭ���� ������ �ø���.
	INT ids = UploadEBookFiles(pDM_EBook);
	if(ids < 0) return ids;
	CString strDBFlag, strKey, strQuery;
	INT nRowCnt = pDM_EBook->GetRowCount();
	CArray<CString, CString> RemoveAliasList;
	
	for(INT i = 0; i < nRowCnt; i++) 
	{
		RemoveAliasList.RemoveAll();
		strDBFlag.Empty();
		ids = pDM_EBook->GetCellData(_T("BB_DB_FLAG"), i, strDBFlag);
		if (strDBFlag == _T("I")) 
		{
			pDM_OUT->MakeRecKey(strKey);
			pDM_EBook->SetCellData(_T("BB_EBOOKKEY"), strKey, i);
			pDM_EBook->SetCellData(_T("BB_�������۾�"), pMgr->GetWorkLogMsg(WORK_LOG), i);
			pDM_EBook->SetCellData(_T("BB_�����۾�"), pMgr->GetWorkLogMsg(WORK_LOG), i);
			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_EBook, _T("BO_EBOOK_TBL"), i, pDM_OUT);
		}
		if (strDBFlag == _T("U")) {
			RemoveAliasList.Add(_T("BB_EBOOKKEY"));
			ids = pDM_EBook->GetCellData(_T("BB_EBOOKKEY"), i, strKey);
			pDM_EBook->SetCellData(_T("BB_�������۾�"), pMgr->GetWorkLogMsg(WORK_LOG), i);
			if(!strKey.IsEmpty())
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_EBook, _T("BO_EBOOK_TBL"), i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strKey);
		}
		if (strDBFlag == _T("D")) 
		{
			ids = pDM_EBook->GetCellData(_T("BB_EBOOKKEY"), i, strKey);
			if(!strKey.IsEmpty())
			{
				strQuery.Format(_T("DELETE FROM BO_EBOOK_TBL WHERE REC_KEY=%s;"), strKey);
				pDM_OUT->AddFrame(strQuery);
			}
		}
	}	
	return 0;
}

INT CBO_SPECIES::SetEBookInfo(CESL_DataMgr * pDM_BOOK, CESL_DataMgr *pDM_EBOOK, INT nCurRow)
{
	CString strImgFile, strEBookFile, strExeFile, strFileSize, strDBFlag, strBookKey, strTmpFile;
	CEBookUpNDownloadMgr mgr;

	strDBFlag = pDM_EBOOK->GetCellData(_T("BB_DB_FLAG"), nCurRow);
	if(strDBFlag == _T("D")) return 0;
	INT ids;

	if(strDBFlag == _T("I"))
	{
		pDM_EBOOK->SetCellData(_T("BB_�����۾�"), pMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
		pDM_EBOOK->SetCellData(_T("BB_�������۾�"), pMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
	}
	else if(strDBFlag == _T("U"))
	{
		pDM_EBOOK->SetCellData(_T("BB_�������۾�"), pMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
	}
	
	strImgFile = pDM_EBOOK->GetCellData(_T("BB_�̹�������"), nCurRow);		
	if(!strImgFile.IsEmpty())
	{
		ids = SetEBookFileSize(pDM_EBOOK, nCurRow, strImgFile, _T("BB_�̹�������_ũ��"));
		if(ids < 0 && strDBFlag == _T("I"))
		{
			strBookKey = pDM_EBOOK->GetCellData(_T("BB_åKEY"), nCurRow);
			ids = FindSameVolumeEBookInfo(pDM_BOOK, strBookKey, strImgFile, strTmpFile);
			if(ids < 0) return -1;
			ids = SetEBookFileSize(pDM_EBOOK, nCurRow, strTmpFile, _T("BB_�̹�������_ũ��"));
			if(ids < 0) return -1;
			pDM_EBOOK->SetCellData(_T("BB_�̹�������"), strTmpFile, nCurRow);
		}			
	}
	strEBookFile = pDM_EBOOK->GetCellData(_T("BB_��������"), nCurRow);
	if(!strEBookFile.IsEmpty())
	{
		ids = SetEBookFileSize(pDM_EBOOK, nCurRow, strEBookFile, _T("BB_��������_ũ��"));
		if(ids < 0 && strDBFlag == _T("I"))
		{
			strBookKey = pDM_EBOOK->GetCellData(_T("BB_åKEY"), nCurRow);
			ids = FindSameVolumeEBookInfo(pDM_BOOK, strBookKey, strEBookFile, strTmpFile);
			if(ids < 0) return -1;
			ids = SetEBookFileSize(pDM_EBOOK, nCurRow, strTmpFile, _T("BB_��������_ũ��"));
			if(ids < 0) return -1;
			pDM_EBOOK->SetCellData(_T("BB_��������"), strTmpFile, nCurRow);
		}	
	}
	strExeFile = pDM_EBOOK->GetCellData(_T("BB_��������"), nCurRow);
	if(!strExeFile.IsEmpty())
	{
		ids = SetEBookExecutorName(pDM_EBOOK, nCurRow, strExeFile, _T("BB_��������"));
	}
	return 0;
}

INT CBO_SPECIES::SetEBookFileSize(CESL_DataMgr * pDM_EBOOK, INT nIdx, CString strFile, CString strAlias)
{
	CFile file;
	CFileException fileException;
	CString strFileSize;
	if (file.Open( strFile.GetBuffer(0), CFile::modeRead | CFile::typeBinary, &fileException ))
	{		
		strFileSize.Format(_T("%d"), file.GetLength());
		pDM_EBOOK->SetCellData(strAlias, strFileSize, nIdx);
		file.Close();
		return 0;
	}
	return -1;
}

INT CBO_SPECIES::SetEBookExecutorName(CESL_DataMgr *pDM_EBOOK, INT nIdx, CString strFile, CString strAlias)
{
	CFile file;
	CFileException fileException;

	if (file.Open( strFile.GetBuffer(0), CFile::modeRead | CFile::typeBinary, &fileException ))
	{				
		pDM_EBOOK->SetCellData(strAlias, file.GetFileName(), nIdx);
		file.Close();
		return 0;
	}
	return -1;
}

INT CBO_SPECIES::FindSameVolumeEBookInfo(CESL_DataMgr *pDM_BOOK, CString strBookKey, CString strFile, CString &strTmpFile)
{
	CString strVolSortNo, strTmpKey, strTmpVolSortNo;
	CEBookUpNDownloadMgr mgr;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	INT ids;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpKey = pDM_BOOK->GetCellData(_T("BB_åKEY"), i);
		if(strTmpKey == strBookKey)
			strVolSortNo = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i);
	}
	if(strVolSortNo.IsEmpty()) return -1;

	for(i=0 ; i<nRowCnt ; i++)
	{
		strTmpVolSortNo = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i);
		if(strVolSortNo == strTmpVolSortNo)
		{
			strTmpKey = pDM_BOOK->GetCellData(_T("BB_åKEY"), i);
			if(strTmpKey == strBookKey) continue;
			ids = mgr.DownloadEBook(strTmpKey, strFile, strTmpFile);
			if(ids < 0) continue;
			else return 0;
		}		
	}
	
	return ids;
}

INT CBO_SPECIES::MakeAccuInfo(CESL_DataMgr *pDM_OUT, CESL_DataMgr *pDM, INT nIdx, BOOL isPurchase)
{
	CString strMarc;
	if(m_pDM_ACCU == NULL) SetAccuDM(isPurchase);
	else	m_pDM_ACCU->FreeData();

	m_pDM_ACCU->InsertRow(-1);
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	
	CString strRecKey, strTmpData, strAcqkey, strPrice, strCurrencyCode, strPriceOtherInfo, strInputType;
	CString strBookCnt, strSpeciesKey;
	CMarc marc;
	strMarc = pDM->GetCellData(_T("BS_MARC"), nIdx);
	pMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	pMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strTmpData);
	Apply_MARC_950(strTmpData, strPrice, strCurrencyCode);
	pMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("950$c"), strPriceOtherInfo);
	
	strInputType = _T("0");
	strAcqkey = pDM->GetCellData(_T("BB_��������KEY"), nIdx);
	strBookCnt = pDM->GetCellData(_T("UDF_å��"), nIdx);
	strSpeciesKey = pDM->GetCellData(_T("IBS_��KEY"), nIdx);
	CGroupInfo GroupInfo;
	INT ids = GetGroupInfo(pDM, nIdx, GroupInfo);
	if(ids < 0) return -1;
	
	if(isPurchase)
	{
		ids = pDM_OUT->MakeRecKey(strAcqkey); 
		if(ids < 0) return ids;
		
		CString strTmpData;
		m_pDM_ACCU->SetCellData(_T("BP_��������KEY"), strAcqkey, 0);
		m_pDM_ACCU->SetCellData(_T("BP_����å��"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_��ǰå��"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_�̳�å��"), _T("0"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�ֹ�����Ÿ��"), _T("������"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_���Գ⵵"), GroupInfo.GetAcqYear(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�׷��ȣ"), GroupInfo.GetGroupInfo(), 0);
		strTmpData = GroupInfo.GetWorkNo();
		if(strTmpData == _T("������")) strTmpData = _T("0");
		m_pDM_ACCU->SetCellData(_T("BP_�۾���ȣ"), strTmpData, 0);
		INT nSerialNo = GroupInfo.GetLastSerialNo();
		strTmpData.Format(_T("%d"), nSerialNo+1);
		GroupInfo.SetLastSerialNo(nSerialNo+1);
		m_pDM_ACCU->SetCellData(_T("BP_�����Ϸù�ȣ"), strTmpData, 0);
		m_pDM_ACCU->SetCellData(_T("BP_�ѱǼ�"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_�������۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�����۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�Է�����"), pDM->GetCellData(_T("BB_�Է���"), 0), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�Է±���"), strInputType, 0);
		nSerialNo = GroupInfo.GetOrderLastSerialNo();
		if(nSerialNo < 0) m_pDM_ACCU->SetCellData(_T("BP_�Ϸù�ȣ"), _T(""), 0);
		else
		{
			strTmpData.Format(_T("%d"), nSerialNo+1);
			GroupInfo.SetOrderLastSerialNo(nSerialNo+1);
			m_pDM_ACCU->SetCellData(_T("BP_�Ϸù�ȣ"), strTmpData, 0);
		}
		m_pDM_ACCU->SetCellData(_T("BP_���ְ���"), strPrice, 0);
		m_pDM_ACCU->SetCellData(_T("BP_��ǰ����"), strPrice, 0);
		m_pDM_ACCU->SetCellData(_T("BP_������"), _T("0"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_��������"), GetTodayDate(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�˼���"), GetTodayDate(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_ȯ��"), _T("1"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_��KEY"), strSpeciesKey, 0);
		m_pDM_ACCU->SetCellData(_T("BP_����"), strPrice, 0);
		m_pDM_ACCU->SetCellData(_T("BP_ȭ�󱸺�"), strCurrencyCode, 0);
		m_pDM_ACCU->SetCellData(_T("BP_����������"), strPriceOtherInfo, 0);
		m_pDM_ACCU->SetCellData(_T("BP_��������_�ڵ�"), pDM->GetCellData(_T("BS_��������"), 0), 0);
		ids = m_pDM_ACCU->MakeQueryByDM(RemoveAliasList, m_pDM_ACCU, m_pDM_ACCU->TBL_NAME, 0, pDM_OUT);
		if(ids < 0) return ids;

		GroupInfo.UpDateLastSerialNo(pMgr, pDM_OUT);
		strTmpData.Format(
			_T("UPDATE BO_BOOK_TBL SET ACQ_KEY=%s, LAST_WORK='%s' WHERE SPECIES_KEY=%s AND ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND ACQ_KEY IS NULL;"),
			strAcqkey, pMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey, GroupInfo.GetAcqYear(), GroupInfo.GetGroupInfo());
		pDM_OUT->AddFrame(strTmpData);
	}
	else
	{
		ids = m_pDM_ACCU->MakeRecKey(strAcqkey);
		if(ids < 0) return ids;

		m_pDM_ACCU->SetCellData(_T("BP_��������KEY"), strAcqkey, 0);
		m_pDM_ACCU->SetCellData(_T("BP_����å��"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_���Գ⵵"), GroupInfo.GetAcqYear(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�׷��ȣ"), GroupInfo.GetGroupInfo(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�ѱǼ�"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_�������۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�����۾�"), pMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�Է�����"), pDM->GetCellData(_T("BB_�Է���"), 0), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�˼���"), GetTodayDate(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�Է±���"), strInputType, 0);
		m_pDM_ACCU->SetCellData(_T("BP_��KEY"), strSpeciesKey, 0);
		m_pDM_ACCU->SetCellData(_T("BP_����"), strPrice, 0);
		m_pDM_ACCU->SetCellData(_T("BP_ȭ�󱸺�"), strCurrencyCode, 0);
		m_pDM_ACCU->SetCellData(_T("BP_����������"), strPriceOtherInfo, 0);
		m_pDM_ACCU->SetCellData(_T("BP_��������_�ڵ�"), pDM->GetCellData(_T("BS_��������"), 0), 0);
		ids = m_pDM_ACCU->MakeQueryByDM(RemoveAliasList, m_pDM_ACCU, m_pDM_ACCU->TBL_NAME, 0, pDM_OUT);
		if(ids < 0) return ids;
		strTmpData.Format(
			_T("UPDATE BO_BOOK_TBL SET ACQ_KEY=%s, LAST_WORK='%s' WHERE SPECIES_KEY=%s AND ACQ_CODE='2' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND ACQ_KEY IS NULL;"),
			strAcqkey, pMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey, GroupInfo.GetAcqYear(), GroupInfo.GetGroupInfo());
		pDM_OUT->AddFrame(strTmpData);
	}

	
	return 0;
}

INT CBO_SPECIES::GetGroupInfo(CESL_DataMgr *pDM, INT nIdx, CGroupInfo &GroupInfo)
{
	CString strWorkCode, strGroupInfo, strAcqYear, strWorkStatus, strAcqCode;
	CString strQuery, strValue, strKey;
	strWorkCode = pDM->GetCellData(_T("IBS_��������"), nIdx);
	strGroupInfo = pDM->GetCellData(_T("BB_�׷��ȣ"), nIdx);
	strAcqYear = pDM->GetCellData(_T("BB_���Գ⵵"), nIdx);
	strWorkStatus = pDM->GetCellData(_T("BB_�ڷ����"), nIdx);
	strAcqCode = pDM->GetCellData(_T("BB_���Ա���"), nIdx);

	GroupInfo.SetAcqYear(strAcqYear);
	GroupInfo.SetGroupInfo(strGroupInfo);
	GroupInfo.SetWorkCode(strWorkCode);

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	if(strAcqCode == _T("1"))
	{
		strQuery.Format(_T("SELECT WORKING_STATUS, LAST_WORK_NO, REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR='%s' AND SEQ_NO=%s"),
			strAcqYear, strGroupInfo);
		tmpDM.RestructDataManager(strQuery);
		strValue = tmpDM.GetCellData(0, 0);
		if(strValue == _T("0")) strWorkStatus = _T("BOA111N");
		else if(strValue == _T("1")) strWorkStatus = _T("BOA112N");
		else if(strValue == _T("2"))
		{
			if(strWorkStatus == _T("BOA111N") || strWorkStatus == _T("BOA112N"))
				strWorkStatus = _T("BOA114O");
			strQuery.Format(_T("SELECT MAX(SERIAL_NO) FROM BO_PURCHASEINFO_TBL WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s"),
				strAcqCode, strGroupInfo);
			pDM->GetOneValueQuery(strQuery, strValue);
			GroupInfo.SetOrderLastSerialNo(_ttoi(strValue.GetBuffer(0)));
		}
		strValue = tmpDM.GetCellData(0, 1);
		strKey = tmpDM.GetCellData(0, 2);
		CStringArray tmpArr;	tmpArr.Add(strValue);
		GroupInfo.SetArrayWorkNo(&tmpArr);
		GroupInfo.SetCurSelWorkNo(strValue);
		strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY=%s AND WORK_NO=%s"), 
			strKey, strValue);
		pDM->GetOneValueQuery(strQuery, strValue);
		GroupInfo.SetLastSerialNo(_ttoi(strValue.GetBuffer(0)));
	}
	return 0;
}

INT CBO_SPECIES::BO_TOC_ABSTRACT_DELETE(CESL_DataMgr *pDM_OUT, CString strSpeciesKey)
{
	CString strQuery, strValue;
	strQuery.Format(_T("SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = %s"), strSpeciesKey);
	pDM_OUT->GetOneValueQuery(strQuery, strValue);
	if(!strValue.IsEmpty())
	{
		strQuery.Format(_T("DELETE FROM BO_CONTENTS_TBL WHERE TOC_COMMON_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM IDX_BO_TOC_TBL WHERE SPECIES_KEY = %s;"), strSpeciesKey);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM BO_TOC_COMMON_TBL WHERE REC_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM BO_ABSTRACTS_TBL WHERE TOC_COMMON_KEY = %s"), strValue);
		pDM_OUT->AddFrame(strQuery);
	}
/*
	strQuery.Format(_T("SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = %s"), strSpeciesKey);
	pDM_OUT->GetOneValueQuery(strQuery, strValue);
	if(!strValue.IsEmpty())
	{
		
		strQuery.Format(_T("DELETE FROM BO_TOC_COMMON_TBL WHERE REC_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
	}
*/

	return 0;
}

INT CBO_SPECIES::Apply_MultiVolumeInfo(CMarc *pMarc, CString strWorkStatus, CESL_DataMgr *pBookDM, CESL_DataMgr * pDM_SPECIES, CESL_DataMgr * pDM_Index, CESL_DataMgr * pDM_OUT)
{
	if(pMarc == NULL || pBookDM == NULL) return -1;
	if(strWorkStatus.IsEmpty()) strWorkStatus = _T("BOA");
	
	RestructBookBy049MarcTag(pBookDM, pMarc, TRUE);
	RestructBookBy505MarcTag(pBookDM, pMarc);
/*	
	INT ids;
	CString strPlaceInfo;
	if(strWorkStatus == _T("BOL") && pDM_Index && pDM_SPECIES)
	{
		strPlaceInfo = pDM_Index->GetCellData(_T("IBS_�ڷ��ִ°�INFO_�ڵ�"), 0);
		SetPlaceInfoToBookDM(pBookDM, strPlaceInfo);
		Set090MarcTagInfo(pDM_Index, pDM_SPECIES, pBookDM, pMarc);
		Set001MarcTagInfo(pDM_Index, pDM_SPECIES, pMarc);
		ids = CheckRegNo(pBookDM, pDM_OUT);
		if(ids < 0) 
		{
			pMgr->ESLAfxMessageBox(_T("�ߺ��� ��Ϲ�ȣ�� �����մϴ�."));
			return ids;
		}
	}
	else
	{
*/
	ClearRegNoFieldOnBookDM(pBookDM);
	SetVolSortNo(pBookDM);
//	}
	return 0;
}

INT CBO_SPECIES::RestructBookBy505MarcTag(CESL_DataMgr *pBookDM, CMarc *pMarc)
{
	CString str505, strVol, strVolAuthor, strVolTitle, strTmpData, strInd;
	pMgr->m_pInfo->pMarcMgr->GetField(pMarc, _T("505"), str505);
	if(str505.IsEmpty()) return 0;

	str505.Replace(SUBFIELD_CODE, '$');
	str505.Replace(FIELD_TERMINATOR, ' ');
	str505.Replace(_T("$n"), _T("#$n"));
	str505.Replace(_T(". -"), _T(""));
	CStringArray arr505;	arr505.RemoveAll();
	ParsingString(_T("#"), str505, &arr505, FALSE);
	INT n505Cnt = arr505.GetSize();
	INT n505SubFieldCnt;
	INT nCnt = 0;
	for(INT i=0 ; i<n505Cnt ; i++)
	{
		CStringArray arr505SubField;	arr505SubField.RemoveAll();
		strTmpData = arr505.GetAt(i);
		strTmpData.TrimLeft();	strTmpData.TrimRight();
		if(strTmpData.Left(1) == _T("1"))
		{
			continue;
		}
		if(strTmpData.IsEmpty()) continue;
		ParsingString(_T("$"), strTmpData, &arr505SubField, FALSE);
		n505SubFieldCnt = arr505SubField.GetSize();
		strVol.Empty();	strVolAuthor.Empty();	strVolTitle.Empty();
		for(INT j=0 ; j<n505SubFieldCnt ; j++)
		{
			strTmpData = arr505SubField.GetAt(j);
			strTmpData.TrimLeft();	strTmpData.TrimRight();
			strInd = strTmpData.Left(1);
			strInd.MakeLower();
			if(strInd == _T("n")) strVol = strTmpData.Mid(1);
			else if(strInd == _T("t")) 
			{
				if(strVolTitle.IsEmpty()) strVolTitle = strTmpData.Mid(1);
				else strVolTitle += _T(";") + strTmpData.Mid(1);
			}
			else if(strInd == _T("d"))
			{
				strVolAuthor = strTmpData.Mid(1);
			}
			else if(strInd == _T("e"))
			{
				strVolAuthor += _T(";") + strTmpData;
			}
			else if(strInd == _T("g")) 
			{	
				strVolAuthor += _T("(") + strTmpData + _T(")");
			}
		}
		Set505ToBookDM(pBookDM, strVol, strVolTitle, strVolAuthor);
	}	
	return 0;
}

INT CBO_SPECIES::Set505ToBookDM(CESL_DataMgr *pDM_BOOK, CString strVol, CString strVolTitle, CString strVolAuthor)
{
	INT nRowCnt = pDM_BOOK->GetRowCount();
	CString strTmpVol;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpVol = pDM_BOOK->GetCellData(_T("BB_�����"), i);
		if(strTmpVol == strVol)
		{
			pDM_BOOK->SetCellData(_T("BB_������ηϸ�"), strVolTitle, i);
			pDM_BOOK->SetCellData(_T("BB_������"), strVolAuthor, i);
		}
	}
	return 0;
}

INT CBO_SPECIES::ClearRegNoFieldOnBookDM(CESL_DataMgr *pBookDM)
{
	INT nRowCnt = pBookDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		pBookDM->SetCellData(_T("BB_��Ϲ�ȣ"), _T(""), i);
	}
	return 0;
}

INT CBO_SPECIES::SetVolSortNo(CESL_DataMgr *pBookDM)
{
	if(pBookDM == NULL) return -1;
	INT nRowCnt = pBookDM->GetRowCount();
	if(nRowCnt == 0) return 0;

	INT nVolSortNo = 0;
	CString strVol, strTmpData;
	CMapStringToString MapVolData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strVol = pBookDM->GetCellData(_T("BB_�����"), i);
		if(!MapVolData.Lookup(strVol, strTmpData))
		{
			nVolSortNo = (nVolSortNo/10 + 1) * 10;
			strTmpData.Format(_T("%d"), nVolSortNo);
			MapVolData.SetAt(strVol, strTmpData);
		}
	}

	for(i=0 ; i<nRowCnt ; i++)
	{
		strVol = pBookDM->GetCellData(_T("BB_�����"), i);
		MapVolData.Lookup(strVol, strTmpData);
		pBookDM->SetCellData(_T("BB_���Ϸù�ȣ"), strTmpData, i);
	}
	return 0;
}

INT CBO_SPECIES::SetCellAttribute(CESL_DataMgr * pDM, CStringArray *pArrAlias, CString strCellType, BOOL bUDFYN)
{
	if(pDM == NULL || pArrAlias == NULL) return -1;
	//============================================
	//20080611 ADD BY PJW : �ҹ��ڴ빮�ڵ��� �����Ƿ� �ҹ��ڷ� �ٲٵ��� �����Ѵ�.
	strCellType.MakeLower();
	//============================================
	if(strCellType != _T("int") && strCellType != _T("str")) return -1;

	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strAlias, strUDF;
	if(bUDFYN) strUDF = _T("udf");
	else strUDF.Empty();

	INT col, nCols, nAliasCnt;
	nAliasCnt = pArrAlias->GetSize();
	if(nAliasCnt == 0) return 0;

	nCols = pDM->m_nCols;
	if(nCols < 0) nCols = pDM->GetRefColumnCount();
	for(INT i=0 ; i<nAliasCnt ; i++)
	{
		strAlias = pArrAlias->GetAt(i);
		if(strAlias.IsEmpty()) continue;

		for (col = 0; col < nCols; col++) 
		{
			if (pDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0)
				continue;
			if(FIELD_ALIAS == strAlias)
			{
				pDM->SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, strCellType, INIT_VALUE, strUDF);
			}
		}
	}
	return 0;
}

INT CBO_SPECIES::SetBookIndexInfo(CESL_DataMgr *pDM_INDEX, INT nIdx, CString strSpeciesKey, CStringArray *pArrAlias)
{
	if(pDM_INDEX == NULL || pArrAlias == NULL) return -1;
	
	CString strData;
	pArrAlias->RemoveAll();
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE')"), strSpeciesKey);
	pDM_INDEX->SetCellData(_T("IBS_�̿����ѱ���_�ڵ�"), strData, nIdx);			pArrAlias->Add(_T("IBS_�̿����ѱ���_�ڵ�"));
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'MANAGE_CODE')"), strSpeciesKey);
	pDM_INDEX->SetCellData(_T("IBS_��������_�ڵ�"), strData, nIdx);				pArrAlias->Add(_T("IBS_��������_�ڵ�"));
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'REG_CODE')"), strSpeciesKey);
	pDM_INDEX->SetCellData(_T("IBS_��ϱ���_�ڵ�"), strData, nIdx);				pArrAlias->Add(_T("IBS_��ϱ���_�ڵ�"));
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX')"), strSpeciesKey);
	pDM_INDEX->SetCellData(_T("IBS_�˻���Ǽ���"), strData, nIdx);				pArrAlias->Add(_T("IBS_�˻���Ǽ���"));
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE')"), strSpeciesKey);
	pDM_INDEX->SetCellData(_T("IBS_�ڷ��ִ°�INFO_�ڵ�"), strData, nIdx);		pArrAlias->Add(_T("IBS_�ڷ��ִ°�INFO_�ڵ�"));
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'VOL_INDEX')"), strSpeciesKey);
	pDM_INDEX->SetCellData(_T("IBS_�˻��������"), strData, nIdx);				pArrAlias->Add(_T("IBS_�˻��������"));
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE')"), strSpeciesKey);
	pDM_INDEX->SetCellData(_T("IBS_�˻��뺰ġ��ȣ"), strData, nIdx);			pArrAlias->Add(_T("IBS_�˻��뺰ġ��ȣ"));	

	return 0;
}

INT CBO_SPECIES::SetBookIndexInfo(CESL_DataMgr *pIDX_DM, CESL_DataMgr *pBOOK_DM, INT nIdx/*=0*/)
{
	if(pIDX_DM == NULL || pBOOK_DM == NULL) return -1;
	if(nIdx < 0) return -1;
	if(pIDX_DM->GetRowCount() == 0 || pBOOK_DM->GetRowCount() == 0) return 0;

	INT ids;
	CString strTmpData;
	strTmpData = pIDX_DM->GetCellData(_T("IBS_�̿����ѱ���_�ڵ�"), nIdx);
	ids = GetBoBookInfo(pBOOK_DM, _T("BB_�̿����ѱ���_�ڵ�"), strTmpData);
	if(ids < 0) return -1;
	pIDX_DM->SetCellData(_T("IBS_�̿����ѱ���_�ڵ�"), strTmpData, nIdx);	strTmpData.Empty();
	strTmpData = pIDX_DM->GetCellData(_T("IBS_��������_�ڵ�"), nIdx);
	ids = GetBoBookInfo(pBOOK_DM, _T("BB_��������_�ڵ�"), strTmpData);
	if(ids < 0) return -1;
	pIDX_DM->SetCellData(_T("IBS_��������_�ڵ�"), strTmpData, nIdx);	strTmpData.Empty();
	strTmpData = pIDX_DM->GetCellData(_T("IBS_��ϱ���_�ڵ�"), nIdx);
	ids = GetBoBookInfo(pBOOK_DM, _T("BB_��ϱ���_�ڵ�"), strTmpData);
	if(ids < 0) return -1;
	pIDX_DM->SetCellData(_T("IBS_��ϱ���_�ڵ�"), strTmpData, nIdx);	strTmpData.Empty();
	strTmpData = pIDX_DM->GetCellData(_T("IBS_�˻���Ǽ���"), nIdx);
	ids = GetBoBookInfo(pBOOK_DM, _T("BB_������ηϸ����"), strTmpData);
	if(ids < 0) return -1;
	pIDX_DM->SetCellData(_T("IBS_�˻���Ǽ���"), strTmpData, nIdx);	strTmpData.Empty();
	strTmpData = pIDX_DM->GetCellData(_T("IBS_�ڷ��ִ°�INFO_�ڵ�"), nIdx);
	ids = GetBoBookInfo(pBOOK_DM, _T("BB_�谡��ġ��ȣ"), strTmpData);
	if(ids < 0) return -1;
	pIDX_DM->SetCellData(_T("IBS_�ڷ��ִ°�INFO_�ڵ�"), strTmpData, nIdx);	strTmpData.Empty();
	strTmpData = pIDX_DM->GetCellData(_T("IBS_�˻��������"), nIdx);
	ids = GetBoBookInfo(pBOOK_DM, _T("BB_���������"), strTmpData);
	if(ids < 0) return -1;
	pIDX_DM->SetCellData(_T("IBS_�˻��������"), strTmpData, nIdx);	strTmpData.Empty();
	strTmpData = pIDX_DM->GetCellData(_T("IBS_�˻��뺰ġ��ȣ"), nIdx);
	ids = GetBoBookInfo(pBOOK_DM, _T("BB_��ġ��ȣ_�ڵ�"), strTmpData);
	if(ids < 0) return -1;
	pIDX_DM->SetCellData(_T("IBS_�˻��뺰ġ��ȣ"), strTmpData, nIdx);	strTmpData.Empty();

	return 0;
}

INT CBO_SPECIES::GetBoBookInfo(CESL_DataMgr *pBOOK_DM, CString strAlias, CString &strData)
{
	if(pBOOK_DM == NULL) return -1;
	if(strAlias.IsEmpty()) return -1;

	INT nRowCnt = pBOOK_DM->GetRowCount();
	strData.Empty();
	if(nRowCnt == 0) return 0;

	CStringArray ArrData;	ArrData.RemoveAll();

	INT ids = ParsingString(_T(" "), strData, &ArrData);
	if(ids < 0) return ids;

	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(pBOOK_DM->GetCellData(_T("BB_DB_FLAG"), i) == _T("D")) continue;
		if(pBOOK_DM->GetCellData(_T("BB_å�η��÷���"), i) == _T("A")) continue;
		strTmpData = pBOOK_DM->GetCellData(strAlias, i);
		if(!CheckIsHave(&ArrData, strTmpData)) ArrData.Add(strTmpData);
	}

	INT nCnt = ArrData.GetSize();
	for(i=0 ; i<nCnt ; i++)
	{
		if(strData.IsEmpty()) strData = ArrData.GetAt(i);
		else strData += _T(" ") + ArrData.GetAt(i);
	}

	return 0;
}

INT CBO_SPECIES::ApplyMultiVolumePrice(CMarc * pMarc, CESL_DataMgr * pBookDM)
{
	if(pMarc == NULL || pBookDM == NULL) return -1;
	if(pBookDM->GetRowCount() == 1) return 0;

	INT nVolumes = GetVolumeCount(pBookDM);
	if(nVolumes == 1) return 0;

	CString strPrice, strCurrencyCode, strTmpData;
	pMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strTmpData);
	if (!strTmpData.IsEmpty()) 
	{
		strPrice = _T("");		strCurrencyCode = _T("");
		Apply_MARC_950(strTmpData, strPrice, strCurrencyCode);
	}
	else
		return 0;

	DOUBLE dPrice = atof(strPrice.GetBuffer(0));
	DOUBLE dVolPrice, dLastVolPrice;

	dVolPrice = dPrice/(DOUBLE)nVolumes;
	dLastVolPrice = dVolPrice + (dPrice - dVolPrice*nVolumes);
	
	INT nChangeCnt = 0;
	CString strVolSortNo, strPrevVolSortNo;
	pBookDM->SORT(_T("BB_���Ϸù�ȣ"));
	for(INT i=0 ; i<pBookDM->GetRowCount() ; i++)
	{
		strVolSortNo = pBookDM->GetCellData(_T("BB_���Ϸù�ȣ"),  i);
		if(strVolSortNo != strPrevVolSortNo)
		{
			strVolSortNo = strPrevVolSortNo;
			nChangeCnt++;
		}
		if(nChangeCnt == nVolumes)
		{
			strTmpData.Format(_T("%0.0f"), dLastVolPrice);
			pBookDM->SetCellData(_T("BB_����"), strTmpData, i);
		}
		else
		{
			strTmpData.Format(_T("%0.0f"), dVolPrice);
			pBookDM->SetCellData(_T("BB_����"), strTmpData, i);
		}
	}

	return 0;
}
