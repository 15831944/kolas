// MakeSimplePrintDM.cpp: implementation of the CMakeSimplePrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeSimplePrintDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeSimplePrintDM::CMakeSimplePrintDM(CESL_Mgr * pParentMgr) : CMakePrintDM(pParentMgr)
{
	m_pPrintObjDM = NULL;
	m_pPrintSourceDM = NULL;
	m_pPrintBookDM = NULL;
}

CMakeSimplePrintDM::~CMakeSimplePrintDM()
{
	if(m_pPrintObjDM)
	{
		delete m_pPrintObjDM;
		m_pPrintObjDM = NULL;
	}
}

INT CMakeSimplePrintDM::InitMakePrintDM()
{
	if(m_pPrintSourceDM == NULL || m_pPrintBookDM == NULL)
		return -1;

	m_pPrintObjDM = new CESL_DataMgr;
	m_pPrintObjDM->SetCONNECTION_INFO(m_pPrintSourceDM->CONNECTION_INFO);
	m_pPrintObjDM->InitDMPKFromFile(m_pPrintBookDM->DB_MGR_ALIAS);
	m_pPrintObjDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	if(m_nPrintType < 0) return -1;
	else if(m_nPrintType == BO_ACQ_BASIC_DATA)
	{
		m_IgnoreAlias.Add(_T("BB_�Է���"));
		m_IgnoreAlias.Add(_T("UDF_�׷��"));
		m_strTotalInfoTitle = _T("BB_�Է���");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
	}
	else if(m_nPrintType == BO_ACQ_IMPORT_DATA)
	{
		m_IgnoreAlias.Add(_T("BB_�Է���"));
		m_IgnoreAlias.Add(_T("UDF_�׷��"));
		m_strTotalInfoTitle = _T("BB_�Է���");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
	}
	else if(m_nPrintType == BO_ACQ_DONATE_DATA)
	{
		m_IgnoreAlias.Add(_T("UDF_������ȣ����"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_IgnoreAlias.Add(_T("BP_�����ڸ�"));
		m_strTotalInfoTitle = _T("UDF_������ȣ����");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
	}
	else if(m_nPrintType == BO_ACQ_NONDONATE_DATA)
	{
		m_IgnoreAlias.Add(_T("UDF_������ȣ����"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_IgnoreAlias.Add(_T("BP_�����ڸ�"));
		m_IgnoreAlias.Add(_T("BP_�����δ����"));
		m_strTotalInfoTitle = _T("UDF_������ȣ����");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
	}
	else if(m_nPrintType == BO_ACQ_DONATE_REGOBJ_DATA)
	{
		m_IgnoreAlias.Add(_T("UDF_������ȣ����"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_IgnoreAlias.Add(_T("BP_�����ڸ�"));
		m_strTotalInfoTitle = _T("UDF_������ȣ����");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
	}
	else if(m_nPrintType == BO_ACQ_PURCHASE_DATA)
	{
		m_IgnoreAlias.Add(_T("UDF_��������"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_strTotalInfoTitle = _T("UDF_��������");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ������");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");
	}
	else if(m_nPrintType == BO_ACQ_RECYCLE_DATA)
	{
		m_IgnoreAlias.Add(_T("IBS_������"));
		m_IgnoreAlias.Add(_T("UDF_�׷��"));
		m_IgnoreAlias.Add(_T("BP_���Ա���"));
		m_IgnoreAlias.Add(_T("CF_���Խ�û��"));
		m_IgnoreAlias.Add(_T("CF_���Խ�û��"));
		m_strTotalInfoTitle = _T("IBS_������");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
	}
	else if(m_nPrintType == BO_ACQ_CHECKINING_DATA)
	{
		m_IgnoreAlias.Add(_T("UDF_��¿���������"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_strTotalInfoTitle = _T("UDF_��¿���������");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ������");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");
	}
	else if(m_nPrintType == BO_ACQ_CHECKIN_REQUEST_DATA)
	{
		m_IgnoreAlias.Add(_T("UDF_��������"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_strTotalInfoTitle = _T("UDF_��������");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ������");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");
	}
	else if(m_nPrintType == BO_ACQ_CHECKIN_COMPLETE_DATA)
	{
		m_IgnoreAlias.Add(_T("UDF_��������"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_IgnoreAlias.Add(_T("BP_�˼���"));
		m_strTotalInfoTitle = _T("UDF_��������");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ������");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");
	}
	return 0;
}

INT CMakeSimplePrintDM::MakePrintDM()
{
	INT ids;
	if(m_pPrintBookDM == NULL) return -1;
	if(m_pPrintBookDM->GetRowCount() == 1000) return 0;

	ids = CheckValid();
	if(ids < 0) return ids;

	if(	m_nPrintType == BO_ACQ_BASIC_DATA || m_nPrintType == BO_ACQ_IMPORT_DATA || 
		m_nPrintType == BO_ACQ_DONATE_DATA || m_nPrintType == BO_ACQ_NONDONATE_DATA ||
		m_nPrintType == BO_ACQ_DONATE_REGOBJ_DATA || m_nPrintType == BO_ACQ_PURCHASE_DATA ||
		m_nPrintType == BO_ACQ_RECYCLE_DATA || m_nPrintType == BO_ACQ_CHECKINING_DATA	||
		m_nPrintType == BO_PURCHASE_DATA || m_nPrintType == BO_DONATE_DATA)
		ids = MakeSpeciesVolumePrintDM(m_pPrintBookDM);
	//else if()

	return ids;
}

INT CMakeSimplePrintDM::MakeSpeciesVolumePrintDM(CESL_DataMgr *pDM_BOOK)
{
	//���α׷��� ��Ʈ���� �����ش�.
	ShowDataProcessingDlg();
	
	//��DM�� �����Ѵ�.
	INT ids = MakeVolumeDM(m_pPrintObjDM, pDM_BOOK, m_strDivAlias);
	if(ids < 0) return ids;

	// �� DM�� ���� �˸��ƽ��� �����Ѵ�.
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = m_strDivAlias;					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintObjDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	//���� ���DM���� �Ѱ������� �����Ѵ�.
	ids = m_cTotalInfo.SetTotalInfo(m_pPrintSourceDM);
	if(ids < 0) return ids;

	// ���DM�� Clear��Ų��.
	m_pPrintDM->FreeData();

	// ���� �� ���DM�� �ϳ��� Ȯ���Ͽ� �ش��ϴ� �� DM�� �����Ѵ�.
	INT nRowCnt = m_pPrintSourceDM->GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	CString strKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i);
		
		strKey = m_pPrintSourceDM->GetCellData(m_strDivAlias, i);
		m_pPrintDM->InsertRow(-1);
		ids = CopyDM(m_pPrintSourceDM, i, m_pPrintDM, m_pPrintDM->GetRowCount()-1);
		if(ids < 0) return ids;
		
		ids = InsertObjInfo(m_pPrintDM, m_pPrintObjDM, m_pPrintDM->GetRowCount()-1, strKey, SPECIES_PRINT);
		if(ids < 0) return ids;
	}
	
	// �Ѱ������� �־ƾ���
	m_pPrintDM->InsertRow(-1);	m_pPrintDM->InsertRow(-1);
	SetTotalInfo(m_pPrintDM, &m_cTotalInfo, m_pPrintDM->GetRowCount()-1, _T("�� ��"));
	return 0;
}

INT CMakeSimplePrintDM::CheckValid()
{
	if(m_pPrintDM == NULL) return -1;
	if(m_pPrintSourceDM == NULL) return -1;
	if(m_pPrintBookDM == NULL) return -1;
	if(m_pPrintObjDM == NULL) return -1;
	if(m_strDivAlias.IsEmpty()) return -1;
	
	return 0;
}

INT CMakeSimplePrintDM::MakeRequestDataPrintDM()
{
	//���α׷��� ��Ʈ���� �����ش�.
	ShowDataProcessingDlg();
	
	// åDM�� ��ȯ�ϸ鼭 BP_�Է±��а��� '2' ���� �̿��� ������ �����Ѵ�.
	CESL_DataMgr DM_BOOK;
	DM_BOOK.SetCONNECTION_INFO(m_pPrintBookDM->CONNECTION_INFO);
	DM_BOOK.InitDMPKFromFile(m_pPrintBookDM->DB_MGR_ALIAS);
	DM_BOOK.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	CESL_DataMgr DM_SPECIES;
	DM_SPECIES.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	DM_SPECIES.InitDMPKFromFile(m_pPrintDM->DB_MGR_ALIAS);
	DM_SPECIES.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	INT ids = MakeRequestData(m_pPrintBookDM, &DM_BOOK);
	if(ids < 0) return -1;
	if(DM_BOOK.GetRowCount() == 0) return 1000;

	// ��DM�� �����Ѵ�.
	ids = MakeSpeciesDM(&DM_SPECIES, &DM_BOOK, FALSE);
	if(ids < 0) return ids;
	ids = SetUDFDMFieldData(&DM_SPECIES, TRUE);
	if(ids < 0) return ids;

	//��ġ�����û�� ������ �����ؾ��Ѵ�.
	ids = SetRequestInfo(&DM_SPECIES);
	if(ids < 0) return ids;

	//��DM�� �����Ѵ�.
	ids = MakeVolumeDM(m_pPrintObjDM, &DM_BOOK, m_strDivAlias);
	if(ids < 0) return ids;
	
	// �� DM�� ���� �˸��ƽ��� �����Ѵ�.
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = m_strDivAlias;					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintObjDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	//���� ���DM���� �Ѱ������� �����Ѵ�.
	ids = m_cTotalInfo.SetTotalInfo(&DM_SPECIES);
	if(ids < 0) return ids;
	
	// ���DM�� Clear��Ų��.
	m_pPrintDM->FreeData();
	
	// ���� �� ���DM�� �ϳ��� Ȯ���Ͽ� �ش��ϴ� �� DM�� �����Ѵ�.
	INT nRowCnt = DM_SPECIES.GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	CString strKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i);
		
		strKey = DM_SPECIES.GetCellData(m_strDivAlias, i);
		m_pPrintDM->InsertRow(-1);
		ids = CopyDM(&DM_SPECIES, i, m_pPrintDM, m_pPrintDM->GetRowCount()-1);
		if(ids < 0) return ids;
		
		ids = InsertObjInfo(m_pPrintDM, m_pPrintObjDM, m_pPrintDM->GetRowCount()-1, strKey, SPECIES_PRINT);
		if(ids < 0) return ids;
	}
	
	// �Ѱ������� �־ƾ���
	m_pPrintDM->InsertRow(-1);	m_pPrintDM->InsertRow(-1);
	SetTotalInfo(m_pPrintDM, &m_cTotalInfo, m_pPrintDM->GetRowCount()-1, _T("�� ��"));
	return 0;
}

INT CMakeSimplePrintDM::MakeRequestData(CESL_DataMgr *pSourceBookDM, CESL_DataMgr *pTargetBookDM)
{
	if(pSourceBookDM == NULL || pTargetBookDM == NULL) return -1;
	INT nRowCnt = pSourceBookDM->GetRowCount();
	CString strTmpData;
	INT ids;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pSourceBookDM->GetCellData(_T("BP_�Է±���"), i);
		if(strTmpData == _T("2"))
		{
			ids = CopyDM(pSourceBookDM, i, pTargetBookDM, -1);
			if(ids < 0) return -1;
		}
	}
	return 0;
}

INT CMakeSimplePrintDM::SetRequestInfo(CESL_DataMgr *pSourceDM)
{
	if(pSourceDM == NULL) return -1;
	if(pSourceDM->GetRowCount() == 0) return 0;

	CString strAcqKey;	strAcqKey.Empty();
	CString strQuery;
	INT nRowCnt = pSourceDM->GetRowCount();
	INT nTmpData, ids;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		nTmpData = (i+1)%600;
		if(nTmpData != 0)
		{
			if(strAcqKey.IsEmpty()) strAcqKey = pSourceDM->GetCellData(_T("BP_��������KEY"), i);
			else strAcqKey += _T(", ") + pSourceDM->GetCellData(_T("BP_��������KEY"), i);
		}
		else 
		{
			strQuery.Format(
				_T("SELECT ACQ_KEY, APPLICANT_NAME, APPLICANT_DATE ")
				_T("FROM BO_FURNISH_TBL ")
				_T("WHERE ACQ_KEY IN (%s)"), strAcqKey);
			ids = SetRequestInfo(pSourceDM, strQuery);
			if(ids < 0) return -1;
			strAcqKey.Empty();
		}
	}

	strQuery.Format(
		_T("SELECT ACQ_KEY, APPLICANT_NAME, APPLICANT_DATE ")
		_T("FROM BO_FURNISH_TBL ")
		_T("WHERE ACQ_KEY IN (%s)"), strAcqKey);
	ids = SetRequestInfo(pSourceDM, strQuery);
	if(ids < 0) return -1;
	
	return 0;
}

INT CMakeSimplePrintDM::SetRequestInfo(CESL_DataMgr * pSourceDM, CString strQuery)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pSourceDM->CONNECTION_INFO);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;

	INT nSourceCnt = pSourceDM->GetRowCount();
	INT nRowCnt = tmpDM.GetRowCount();
	CString strAcqKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strAcqKey = tmpDM.GetCellData(i, 0);
		for(INT j=0 ; j<nSourceCnt ; j++)
		{
			if(pSourceDM->GetCellData(_T("BP_��������KEY"), j) == strAcqKey) break;
		}
		if(nSourceCnt == j) continue;
		pSourceDM->SetCellData(_T("UDF_��û��"), tmpDM.GetCellData(i, 1), j);
		pSourceDM->SetCellData(_T("UDF_��û��"), tmpDM.GetCellData(i, 2), j);
	}
	return 0;
}

INT CMakeSimplePrintDM::MakeCheckInCompletePrintDM()
{
	//���α׷��� ��Ʈ���� �����ش�.
	ShowDataProcessingDlg();
	
	CESL_DataMgr DM_SPECIES;
	DM_SPECIES.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	DM_SPECIES.InitDMPKFromFile(m_pPrintDM->DB_MGR_ALIAS);
	DM_SPECIES.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	
	// ��DM�� �����Ѵ�.
	INT ids = MakeSpeciesDM(&DM_SPECIES, m_pPrintBookDM, FALSE);
	if(ids < 0) return ids;
	ids = SetUDFDMFieldData(&DM_SPECIES, TRUE);
	if(ids < 0) return ids;
	
	//��DM�� �����Ѵ�.
	ids = MakeVolumeDM(m_pPrintObjDM, m_pPrintBookDM, m_strDivAlias);
	if(ids < 0) return ids;
	
	// �� DM�� ���� �˸��ƽ��� �����Ѵ�.
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = m_strDivAlias;					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintObjDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	//���� ���DM���� �Ѱ������� �����Ѵ�.
	ids = m_cTotalInfo.SetTotalInfo(&DM_SPECIES);
	if(ids < 0) return ids;
	
	// ���DM�� Clear��Ų��.
	m_pPrintDM->FreeData();
	
	// ���� �� ���DM�� �ϳ��� Ȯ���Ͽ� �ش��ϴ� �� DM�� �����Ѵ�.
	INT nRowCnt = DM_SPECIES.GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	CString strKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i);
		
		strKey = DM_SPECIES.GetCellData(m_strDivAlias, i);
		m_pPrintDM->InsertRow(-1);
		ids = CopyDM(&DM_SPECIES, i, m_pPrintDM, m_pPrintDM->GetRowCount()-1);
		if(ids < 0) return ids;
		
		ids = InsertObjInfo(m_pPrintDM, m_pPrintObjDM, m_pPrintDM->GetRowCount()-1, strKey, SPECIES_PRINT);
		if(ids < 0) return ids;
	}
	
	// �Ѱ������� �־ƾ���
	m_pPrintDM->InsertRow(-1);	m_pPrintDM->InsertRow(-1);
	SetTotalInfo(m_pPrintDM, &m_cTotalInfo, m_pPrintDM->GetRowCount()-1, _T("�� ��"));
	return 0;
}
