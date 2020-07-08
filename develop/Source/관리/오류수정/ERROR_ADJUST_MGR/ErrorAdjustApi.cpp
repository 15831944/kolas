// ErrorAdjustApi.cpp: implementation of the CErrorAdjustApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorAdjustApi.h"
#include "ESL_CharSetMgr.h"
#include "MarcAdjustCondition.h"
#include "MarcAdjustDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("��������API"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorAdjustApi::CErrorAdjustApi(CESL_Mgr * m_pParentMgr) : CSPF_Work(m_pParentMgr)
{
	m_pDM_ACCU = NULL;
}

CErrorAdjustApi::~CErrorAdjustApi()
{
	if(m_pDM_ACCU)
	{
		delete m_pDM_ACCU;
		m_pDM_ACCU = NULL;
	}
}

INT CErrorAdjustApi::SPFExecutePath(CString strPath)
{
	if(strPath.IsEmpty())
		return -1;
	if(strPath == _T("åTo��"))
		return MakeSpeciesInfoByBook();
	else if(strPath == _T("������������"))
		return AdjustPurchaseDataError();
	else if(strPath == _T("å���ε���ȭ"))
		return SynchronizedBookToIndex();
	else if(strPath == _T("�����籸��"))
		return RestructIndexField();
	else if(strPath == _T("��ũ����"))
		return MarcUpdate();
	else if(strPath == _T("�������ڷ����"))
		return DeleteWasteData();
	else if(strPath == _T("��Ȱ���ڷẹ��"))
		return RecycleDataRestore();
	else if(strPath == _T("��Ϲ�ȣ����IN��������"))
		return SetRegNoInLoanInfo();
	else if(strPath == _T("å�η��÷��׼���"))
		return SetAppendixBookFlag();
	else if(strPath == _T("��ġ��������籸��"))
		return RestructRequestIndexData();
	else if(strPath == _T("��������������"))
		return AdjustSpeciesError();
	else if(strPath == _T("������ȣ��������"))
		return DeleteDataBook();
	return -1;
}

INT CErrorAdjustApi::MakeSpeciesInfoByBook()
{
	CESL_DataMgr * pBook_DM = NULL;
	CESL_DataMgr * pSpecies_DM = NULL;
	CProgressCtrl * pProgressCtrl = NULL;
	pBook_DM = (CESL_DataMgr*)GetTempPtr(_T("åDM"));
	pSpecies_DM = (CESL_DataMgr*)GetTempPtr(_T("��DM"));
	pProgressCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCtrl"));
	
	if(!pBook_DM || !pSpecies_DM) return -1;
	INT nRowCnt = pBook_DM->GetRowCount();
	if(nRowCnt == 0) return 0;
	pSpecies_DM->FreeData();
	pBook_DM->SORT(_T("IBS_��KEY"));
	
	MakeOtherInfoByBook(pSpecies_DM, pBook_DM, 0, nRowCnt-1, _T("IBS_��KEY"), pProgressCtrl);
	
	return 0;
}

INT CErrorAdjustApi::MakeOtherInfoByBook(CESL_DataMgr * pTargetDM, CESL_DataMgr * pSourceDM, INT startIdx, INT endIdx, CString strAlias, CProgressCtrl * pProgressCtrl/*=NULL*/)
{
	CString strKey, strTmpKey, strTmpData, strDeliveryYN, strSpeciesWorkingStatus;
	INT ids, cnt, nDBookCnt, nTmp1, nTmp2;
	ids = cnt = nDBookCnt = nTmp1 = nTmp2 = 0;
	DOUBLE nTotalPrice = 0.0;
	cnt = nDBookCnt = 0;

	TCHAR* pEndPtr = NULL;
	
	if(pProgressCtrl)
	{
		pProgressCtrl->SetRange32(0, endIdx-startIdx)	;
		pProgressCtrl->SetPos(0);
	}
	
	for(INT i=startIdx ; i<=endIdx; i++)
	{
		if(pProgressCtrl)
		{
			pProgressCtrl->SetPos(i+1);
		}
		pSourceDM->GetCellData(strAlias, i, strTmpKey);
		if(!strKey.IsEmpty() && strKey != strTmpKey)
		{
			ids = AddToTargetDM(pTargetDM, pSourceDM, i-1);
			if(ids < 0) break;
			strTmpData.Format(_T("%d"), cnt);
			pTargetDM->SetCellData(_T("UDF_å��"), strTmpData, ids);
			strTmpData.Format(_T("%.0f"), nTotalPrice);
			pTargetDM->SetCellData(_T("UDF_������"), strTmpData, ids);
			pTargetDM->SetCellData(_T("UDF_����"), strTmpData, ids);
			pTargetDM->SetCellData(_T("UDF_�ڷ���¼���"), strSpeciesWorkingStatus, ids);
			
			strSpeciesWorkingStatus.Empty();
			strKey = strTmpKey;
			cnt = 1;
			strTmpData = pSourceDM->GetCellData(_T("BB_����"), i);
			//nTotalPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
			nTotalPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			strSpeciesWorkingStatus = pSourceDM->GetCellData(_T("BB_�ڷ����"), i);
		}
		else 
		{
			cnt++;
			strTmpData = pSourceDM->GetCellData(_T("BB_����"), i);
			//nTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			nTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			strTmpData = pSourceDM->GetCellData(_T("BB_�ڷ����"), i);
			strKey = strTmpKey;
			if(strTmpData.IsEmpty()) continue;
			if(nTmp1 < nTmp2) strSpeciesWorkingStatus = strTmpData;
			else if(nTmp1 == nTmp2) 
			{
				if(strSpeciesWorkingStatus.Compare(strTmpData) < 0) strSpeciesWorkingStatus = strTmpData;
			}
		}
	}
	if(i > endIdx)
	{
		ids = AddToTargetDM(pTargetDM, pSourceDM, i-1);
		strTmpData.Format(_T("%d"), cnt);
		pTargetDM->SetCellData(_T("UDF_å��"), strTmpData, ids);
		strTmpData.Format(_T("%.0f"), nTotalPrice);
		pTargetDM->SetCellData(_T("UDF_������"), strTmpData, ids);
		pTargetDM->SetCellData(_T("UDF_�ڷ���¼���"), strSpeciesWorkingStatus, ids);
	}
	return 0;
}

INT CErrorAdjustApi::AddToTargetDM(CESL_DataMgr *pTargetDM, CESL_DataMgr *pSourceDM, INT idx)
{
	if (!pTargetDM || !pSourceDM) return -1000;
	if( idx < 0) return -1000;
	
	INT col, nCol;
	
	// ����� �ʵ� ������ �����Ѵ�.
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT nCurRow, nColCnt;
	pTargetDM->InsertRow(-1);
	nCurRow = pTargetDM->GetRowCount() - 1;
	nColCnt = pSourceDM->m_nCols;
	if(nColCnt < 0) nColCnt = pSourceDM->GetRefColumnCount();
	
	for (col = 0; col < nColCnt; col++) {
		if (pSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		nCol = pTargetDM->FindColumn(FIELD_ALIAS);
		if(nCol >= 0)	pTargetDM->SetCellData(nCurRow, nCol, pSourceDM->GetCellData(idx, col));
	}
	
	return nCurRow;
}

INT CErrorAdjustApi::SetAccuDM(CESL_DataMgr * pDM_ACCU, BOOL bPurchase)
{
	CString strDMAlias, strDMPK;
	if(bPurchase) strDMAlias = _T("DMUP_����_��_��������");
	else strDMAlias = _T("DMUP_����_��_��������");
	
	if(m_pDM_ACCU != NULL) 
	{
		delete m_pDM_ACCU;
		m_pDM_ACCU = NULL;
	}
	m_pDM_ACCU = new CESL_DataMgr;
	m_pDM_ACCU->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	m_pDM_ACCU->InitDMPKFromFile(strDMAlias);
	m_pDM_ACCU->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	return 0;
}

INT CErrorAdjustApi::Apply_MARC_950(CString strSource, CString strPrice, CString strCurrencyCode)
{
	INT nSize = strSource.GetLength();
	TCHAR ch;
	TCHAR szBufDigit[1024];
	INT nBufDigitSize = 0;
	TCHAR szBufNotDigit[1024];
	INT nBufNotDigitSize = 0;
	for (INT i = 0; i < nSize; i++) {
		ch = strSource.GetAt(i);
		if (isdigit(ch)) {
			if ((INT)ch > 0) szBufDigit[nBufDigitSize++] = ch;
		} else {
			if ((INT)ch > 0) szBufNotDigit[nBufNotDigitSize++] = ch;
		}
		
	}
	szBufDigit[nBufDigitSize] = '\0';
	strPrice.Format(_T("%s"), szBufDigit);
	strPrice.TrimLeft();		strPrice.TrimRight();
	szBufNotDigit[nBufNotDigitSize] = '\0';
	strCurrencyCode.Format(_T("%s"), szBufNotDigit);
	strCurrencyCode.TrimLeft();			strCurrencyCode.TrimRight();
	strCurrencyCode = strCurrencyCode.Left(3);
	
	return 0;
}

INT CErrorAdjustApi::GetGroupInfo(CESL_DataMgr *pDM, INT nIdx, CString &strOrderLastSerialNo, CString &strWorkNo, CString &strLastSerialNo)
{
	CString strWorkCode, strGroupInfo, strAcqYear, strWorkStatus, strAcqCode;
	CString strQuery, strValue, strKey;
	strWorkCode = pDM->GetCellData(_T("IBS_��������"), nIdx);
	strGroupInfo = pDM->GetCellData(_T("BB_�׷��ȣ"), nIdx);
	strAcqYear = pDM->GetCellData(_T("BB_���Գ⵵"), nIdx);
	strWorkStatus = pDM->GetCellData(_T("BB_�ڷ����"), nIdx);
	strAcqCode = pDM->GetCellData(_T("BB_���Ա���"), nIdx);
		
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
			strOrderLastSerialNo = strValue;
		}
		strValue = tmpDM.GetCellData(0, 1);
		strKey = tmpDM.GetCellData(0, 2);
		CStringArray tmpArr;	tmpArr.Add(strValue);
		strWorkNo = strValue;
		strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY=%s AND WORK_NO=%s"), 
			strKey, strValue);
		pDM->GetOneValueQuery(strQuery, strValue);
		strLastSerialNo = strValue;
	}
	return 0;
}

CString CErrorAdjustApi::GetTodayYear()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

CString CErrorAdjustApi::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CErrorAdjustApi::MakeAccuInfo(CESL_DataMgr *pDM_OUT, CESL_DataMgr *pDM, INT nIdx, BOOL isPurchase)
{
	CString strMarc;
	CESL_DataMgr * pDM_ACCU = NULL;
	SetAccuDM(pDM_ACCU, isPurchase);
	
	m_pDM_ACCU->InsertRow(-1);
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	
	CString strRecKey, strTmpData, strAcqkey, strPrice, strCurrencyCode, strPriceOtherInfo, strInputType;
	CString strBookCnt, strSpeciesKey;
	CMarc marc;
	strMarc = pDM->GetCellData(_T("BS_MARC"), nIdx);
	m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strTmpData);
	Apply_MARC_950(strTmpData, strPrice, strCurrencyCode);
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("950$c"), strPriceOtherInfo);
	
	strInputType = _T("0");
	strAcqkey = pDM->GetCellData(_T("BB_��������KEY"), nIdx);
	strBookCnt = pDM->GetCellData(_T("UDF_å��"), nIdx);
	strSpeciesKey = pDM->GetCellData(_T("IBS_��KEY"), nIdx);
	
	CString strOrderSerialNo, strWorkNo, strLastSerialNo;
	INT ids = GetGroupInfo(pDM, nIdx, strOrderSerialNo, strWorkNo, strLastSerialNo);
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
		m_pDM_ACCU->SetCellData(_T("BP_���Գ⵵"), pDM->GetCellData(_T("BB_���Գ⵵"), nIdx), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�׷��ȣ"), pDM->GetCellData(_T("BB_�׷��ȣ"), nIdx), 0);
		strTmpData = strWorkNo;
		if(strTmpData == _T("������")) strTmpData = _T("0");
		m_pDM_ACCU->SetCellData(_T("BP_�۾���ȣ"), strTmpData, 0);
		INT nSerialNo = _ttoi(strLastSerialNo.GetBuffer(0));
		strTmpData.Format(_T("%d"), nSerialNo+1);
		m_pDM_ACCU->SetCellData(_T("BP_�����Ϸù�ȣ"), strTmpData, 0);
		m_pDM_ACCU->SetCellData(_T("BP_�ѱǼ�"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_�������۾�"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�����۾�"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�Է�����"), pDM->GetCellData(_T("BB_�Է���"), 0), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�Է±���"), strInputType, 0);
		INT nOrderSerialNo = _ttoi(strOrderSerialNo.GetBuffer(0));
		if(nOrderSerialNo < 0) m_pDM_ACCU->SetCellData(_T("BP_�Ϸù�ȣ"), _T(""), 0);
		else
		{
			strTmpData.Format(_T("%d"), nOrderSerialNo+1);
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

		strTmpData.Format(
			_T("UPDATE BO_PURCHASE_WORK_NO_TBL SET LAST_SERIAL_NO = %d, LAST_WORK = '%s' WHERE WORK_NO = %s AND PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s);"),
			nSerialNo, 
			m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			strWorkNo,
			pDM->GetCellData(_T("BB_���Գ⵵"), nIdx),
			pDM->GetCellData(_T("BB_�׷��ȣ"), nIdx)
			);
		pDM_OUT->AddFrame(strTmpData);
		strTmpData.Format(
			_T("UPDATE BO_BOOK_TBL SET ACQ_KEY=%s, LAST_WORK='%s' WHERE SPECIES_KEY=%s AND ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND ACQ_KEY IS NULL;"),
			strAcqkey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey, pDM->GetCellData(_T("BB_���Գ⵵"), nIdx), pDM->GetCellData(_T("BB_�׷��ȣ"), nIdx));
		pDM_OUT->AddFrame(strTmpData);
	}
	else
	{
		ids = m_pDM_ACCU->MakeRecKey(strAcqkey);
		if(ids < 0) return ids;

		m_pDM_ACCU->SetCellData(_T("BP_��������KEY"), strAcqkey, 0);
		m_pDM_ACCU->SetCellData(_T("BP_����å��"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_���Գ⵵"), pDM->GetCellData(_T("BB_���Գ⵵"), nIdx), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�׷��ȣ"), pDM->GetCellData(_T("BB_�׷��ȣ"), nIdx), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�ѱǼ�"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_�������۾�"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_�����۾�"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
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
			strAcqkey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey, pDM->GetCellData(_T("BB_���Գ⵵"), nIdx), pDM->GetCellData(_T("BB_�׷��ȣ"), nIdx));
		pDM_OUT->AddFrame(strTmpData);
	}

	
	return 0;
}

INT CErrorAdjustApi::AdjustPurchaseDataError()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));	
	if(pDM == NULL || pGrid == NULL) return -1;

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;

	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	// StartFrame
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		ids = MakeAccuInfo(pDM, pDM, i, TRUE);
		if(ids < 0) return -1;
		ArrIdx.Add(i);
		i = pGrid->SelectGetNext();
	}

	//ids = api.SendFrame(pDM, MIN_QUERY_SIZE);
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;
	
	DeleteSelectRow(pDM, &ArrIdx);
	
	return 0;
}

INT CErrorAdjustApi::SynchronizedBookToIndex()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));
	if(pDM == NULL || pGrid == NULL) return -1;

	CESL_DataMgr tmpBookDM;
	tmpBookDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;

	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();

	CString strSpeciesKey, strQuery, strBookKey, strVol, strVolTitle, strIdxVol, strIdxVolTitle;
	CStringArray ArrBookInfo;	ArrBookInfo.RemoveAll();
	INT nBookCnt;
	pDM->StartFrame();
	
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		strSpeciesKey = pDM->GetCellData(_T("IBS_��KEY"), i);
		if(strSpeciesKey.IsEmpty())
		{
			i = pGrid->SelectGetNext();
			continue;
		}
		strQuery.Format(_T("SELECT REC_KEY, VOL, VOL_TITLE FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s"), strSpeciesKey);
		ids = tmpBookDM.RestructDataManager(strQuery);
		if(ids < 0) return -1;
		strIdxVol.Empty();	strIdxVolTitle.Empty();
		
		nBookCnt = tmpBookDM.GetRowCount();
		for(INT j=0 ; j<nBookCnt ; j++)
		{
			strBookKey = tmpBookDM.GetCellData(j, 0);
			strVol = tmpBookDM.GetCellData(j, 1);
			strVolTitle = tmpBookDM.GetCellData(j, 2);
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strVol);			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(strVol);
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strVolTitle);		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(strVolTitle);
			if(strIdxVol.IsEmpty()) strIdxVol = strVol;
			else strIdxVol += _T(" ") + strVol;
			if(strIdxVolTitle.IsEmpty()) strIdxVolTitle = strVolTitle;
			else strIdxVolTitle += _T(" ") + strVolTitle;
			strQuery.Format(
				_T("UPDATE BO_BOOK_TBL SET VOL_INDEX='%s', VOL_TITLE_INDEX='%s' WHERE REC_KEY=%s;"), 
				strVol, strVolTitle, strBookKey);
			pDM->AddFrame(strQuery);
		}
		strQuery.Format(
			_T("UPDATE IDX_BO_TBL SET	IDX_VOL='%s', ")
			_T("IDX_VOL_TITLE='%s', ")
			_T("USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
			_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
			_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
			_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
			_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE'), ")
			_T("LAST_WORK='%s' where rec_key=%s;"), 
			strIdxVol, strIdxVolTitle, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
		pDM->AddFrame(strQuery);
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		strQuery.Format(
// 			_T("INSERT INTO IDX_TMP_BO_TBL ")
// 			_T("(SELECT esl_seq.NEXTVAL, rec_key, 'I', SYSDATE ")
// 			_T("FROM IDX_BO_TBL ")
// 			_T("WHERE REC_KEY=%s);"),
// 			strSpeciesKey);
// 		pDM->AddFrame(strQuery);
		//=====================================================
		
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
			pDM->AddFrame(strQuery);
		}

		ArrIdx.Add(i);
		i = pGrid->SelectGetNext();
	}

	ids = pDM->SendFrame();
	pDM->EndFrame();
	if(ids < 0) return ids;
	DeleteSelectRow(pDM, &ArrIdx);

	return 0;
}

INT CErrorAdjustApi::DeleteSelectRow(CESL_DataMgr * pDM, CArray<INT, INT> * pArrIdx)
{
	INT cnt = pArrIdx->GetSize();
	if(cnt == 0) return 0;
	
	INT rowidx, ids;
	for(INT i=cnt-1 ; i>=0 ; i--)
	{
		rowidx = pArrIdx->GetAt(i);
		ids = pDM->DeleteRow(rowidx);
		if(ids < 0)
			return -1001;
	}
	return 0;
}

BOOL CErrorAdjustApi::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
		return FALSE;
}

INT CErrorAdjustApi::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray, BOOL bIndexing/*=TRUE*/)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos >= 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+DelimeterStr.GetLength());
		if(bIndexing) ((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		if(!bIndexing || !CheckIsHave(pTargetArray, strTmpData))
			pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	if(bIndexing) ((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(!bIndexing || !CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	return 0;
}

INT CErrorAdjustApi::RestructIndexField()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));
	if(pDM == NULL || pGrid == NULL) return -1;
	
	CESL_DataMgr DM_Index;
	DM_Index.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	DM_Index.InitDMPKFromFile(_T("DMUP_����_��_��������"));
	DM_Index.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}	
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;

	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	// StartFrame
	pDM->StartFrame();
	CStringArray ArrData;
	
	CString strMarc, strKey, strQuery;
	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	CMarc marc;
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		strMarc = pDM->GetCellData(_T("BS_MARC"), i);
		strKey = pDM->GetCellData(_T("IBS_��KEY"), i);
		if(strKey.IsEmpty())
		{
			i = pGrid->SelectGetNext();
			continue;
		}

		if(strMarc.IsEmpty()) 
		{
			i = pGrid->SelectGetNext();
			continue;
		}
		m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		strQuery.Format(_T("REC_KEY = %s"), strKey);
		DM_Index.RefreshDataManager(strQuery);
		if(DM_Index.GetRowCount() < 1)
		{
			i = pGrid->SelectGetNext();
			continue;
		}
		APPLY_IndexItem(&DM_Index, &marc, 0);
		DM_Index.MakeQueryByDM(RemoveAliasList, &DM_Index, DM_Index.TBL_NAME, 0, pDM, _T("REC_KEY"), _T("NUMERIC"), strKey);
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strKey);
			pDM->AddFrame(strQuery);
		}
		ArrIdx.Add(i);

		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	pDM->EndFrame();
	DeleteSelectRow(pDM, &ArrIdx);
	return ids;
}

INT CErrorAdjustApi::APPLY_IndexItem(CESL_DataMgr *pDM_IDX_BO, CMarc *pMarc, INT nRowIdx) 
{
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->Init(m_pParentMgr->m_pInfo->pMarcMgr, pMarc);
	
	INT nIndexToolKit = 0;
	CString IndexToolKit[20];
	CString IndexToolKitCommand[20];
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

	INT i;
	CString strTag, strData, strIdxTitle, strIdxAuthor, strIdxPublihser, strIdxKeyword;
	for (i = 0; i < nIndexToolKit; i++) {
		strData.Empty();
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(IndexToolKitCommand[i], strData);
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
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strData);
		DeleteUnlessWord(strData);
		pDM_IDX_BO->SetCellData(_T("IBS_�˻���������"), strData, nRowIdx);
		strIdxAuthor = strData;
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_������"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		// ��Ģ1 : 245$d
		// ��Ģ1�� ������� ��Ģ2 : ����(���ı���TX)->100, �񵵼�(���ı���!=TX)->700

		// ��Ģ1 ����
		strData = _T("");
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strData);
		DeleteUnlessWord(strData);
		pDM_IDX_BO->SetCellData(_T("IBS_������"), strData, nRowIdx);
		
		CString strFORM_CODE;
		// ��Ģ2 or 3����
		if (strData.GetLength() == 0) {
			pDM_IDX_BO->GetCellData(_T("IBS_���ı���_�ڵ�"), nRowIdx, strFORM_CODE);
			strData = _T("");
			if (strFORM_CODE == _T("TX")) {
				m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), strData);
				pDM_IDX_BO->SetCellData(_T("IBS_������"), strData, nRowIdx);
			} else {
				m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("700$a"), strData);
				pDM_IDX_BO->SetCellData(_T("IBS_������"), strData, nRowIdx);
			}
		}
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
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("������"), strData);
		pDM_IDX_BO->SetCellData(_T("IBS_�˻�������˻�������"), strData, nRowIdx);
//		pDM_IDX_BO->SetCellData(_T("IBS_������"), strData, nRowIdx);
//		pDM_IDX_BO->SetCellData(_T("IBS_�����ڻ���"), strData, nRowIdx);
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_������"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("������"), strData);
		pDM_IDX_BO->SetCellData(_T("IBS_������"), strData, nRowIdx);
//		pDM_IDX_BO->SetCellData(_T("IBS_�����ڻ���"), strData, nRowIdx);
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_�����"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("�����˻������⵵"), strData);
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

INT CErrorAdjustApi::DeleteUnlessWord(CString &strData)
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

INT CErrorAdjustApi::DeleteWasteData()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));
	
	if(pDM == NULL || pGrid == NULL) return -1;

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;
	
	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	CString strSpeciesKey, strQuery;
	// StartFrame
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		strSpeciesKey = pDM->GetCellData(_T("IBS_��KEY"), i);
		//1. �˻����� ����ȭ�� ���� ť���̺� �α� ����
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		strQuery.Format(
// 			_T("INSERT INTO idx_tmp_bo_tbl VALUES (")
// 			_T("ESL_SEQ.NEXTVAL, %s, 'D', SYSDATE);"),
// 			strSpeciesKey);
// 		pDM->AddFrame(strQuery);
		//=====================================================
		
		//�� ���̺��� å ���� �ڷ� ����
		strQuery.Format(
			_T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY = %s;"), strSpeciesKey);
		pDM->AddFrame(strQuery);
		//������������
		strQuery.Format(
			_T("DELETE FROM BO_PURCHASEINFO_TBL WHERE SPECIES_KEY=%s;"), strSpeciesKey);
		pDM->AddFrame(strQuery);
		//������������
		strQuery.Format(
			_T("DELETE FROM BO_DONINFO_TBL WHERE SPECIES_KEY=%s;"), strSpeciesKey);
		pDM->AddFrame(strQuery);
		//���� ���̺��� å ���� �ڷ� ����
		strQuery.Format(
			_T("DELETE FROM IDX_BO_TBL WHERE REC_KEY = %s;"), strSpeciesKey);
		pDM->AddFrame(strQuery);
		//ECO_SEARCH�� ����
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_DELETE_BO$I(%s);"), strSpeciesKey);
			pDM->AddFrame(strQuery);
		}
		//�����ʷ� ����
		ids = BO_TOC_ABSTRACT_DELETE(pDM, strSpeciesKey);
		if(ids < 0) return -1;
		
		ArrIdx.Add(i);
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	pDM->EndFrame();
	if(ids < 0) return -1;
	DeleteSelectRow(pDM, &ArrIdx);
	return ids;
}

INT CErrorAdjustApi::BO_TOC_ABSTRACT_DELETE(CESL_DataMgr *pDM_OUT, CString strSpeciesKey)
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
		strQuery.Format(_T("DELETE FROM BO_ABSTRACTS_TBL WHERE TOC_COMMON_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM BO_TOC_COMMON_TBL WHERE REC_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
	}
	
	return 0;
}

BOOL CErrorAdjustApi::IsSameSeShelfCode(CESL_DataMgr *pDM_BOOK)
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

INT CErrorAdjustApi::Restruct049MarcTag(CESL_DataMgr *pDM_BOOK, CMarc *pMarc)
{
	BOOL bSameSeShelfCode = FALSE;
	bSameSeShelfCode = IsSameSeShelfCode(pDM_BOOK);
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("049"));
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
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$l"), strRegNo);
		if(!strVol.IsEmpty())
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$v"), strVol);
		if(!bSameSeShelfCode && !strSeShelfCode.IsEmpty())
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$f"), strSeShelfCode);
		if(!strCopyCode.IsEmpty())
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$c"), strCopyCode);
	}
	
	if(bSameSeShelfCode)
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetField( pMarc, _T("049"), str049 );
		if (!str049.IsEmpty())
		{
			strSeShelfCode = pDM_BOOK->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), 0);
			if(!strSeShelfCode.IsEmpty())
				m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$f"), strSeShelfCode);
			str049 = _T("0") + str049.Mid(1);
			m_pParentMgr->m_pInfo->pMarcMgr->DeleteField( pMarc, _T("049") );
			m_pParentMgr->m_pInfo->pMarcMgr->InsertField( pMarc, _T("049") + str049);
		}		
	}
	else
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetField( pMarc, _T("049"), str049 );
		if (!str049.IsEmpty()) 
		{
			str049 = _T("1") + str049.Mid(1);
			m_pParentMgr->m_pInfo->pMarcMgr->DeleteField( pMarc, _T("049") );
			m_pParentMgr->m_pInfo->pMarcMgr->InsertField( pMarc, _T("049") + str049);
		}
	}
	
	return 0;	
}

INT CErrorAdjustApi::RestructBookBy049MarcTag(CESL_DataMgr *pDM_BOOK, CMarc *pMarc, BOOL b049InfoInsertYN/*=FALSE*/)
{
	BOOL bSameShelfCode = FALSE;
	CString str049, strTmpData, strInd, strVol, strRegNo, strCopyCode, strRepresentShelfCode, strShelfCode, strRegCode;
	m_pParentMgr->m_pInfo->pMarcMgr->GetField(pMarc, _T("049"), str049);
	if(str049.IsEmpty()) return 0;
	
	str049.Replace(SUBFIELD_CODE, '$');
	str049.Replace(FIELD_TERMINATOR, ' ');
	str049.Replace(_T("$l"), _T("#$l"));
	CStringArray arr049;	arr049.RemoveAll();
	ParsingString(_T("#"), str049, &arr049, FALSE);
	INT n049Cnt = arr049.GetSize();
	INT n049SubFieldCnt = -1;
	INT nRegNo;
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//===================================================================

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
				//=====================================================
				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 				strRegCode = strTmpData.Left(2);
// 				if(strTmpData.GetLength() >= 3)
// 				{
// 					nRegNo = _ttoi(strTmpData.Mid(3).GetBuffer(0));
// 					strRegNo.Format(_T("%s%010d"), strRegCode, nRegNo);
// 				}
				
				strRegCode = strTmpData.Left(nRegCodeLength);
				if(strTmpData.GetLength() > nRegCodeLength)
				{
					strRegNo = m_pParentMgr->m_pInfo->MakeRegNo(strRegCode,strTmpData.Mid(nRegCodeLength));
				} 
				//===================================================================
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

INT CErrorAdjustApi::Set049ToBookDM(CESL_DataMgr *pDM_BOOK, CString strRegNo, CString strVol, CString strCopyCode, CString strShelfCode, BOOL b049InfoInsertYN/*=FALSE*/, INT nIdx/*=-1*/)
{
	INT nRowCnt = pDM_BOOK->GetRowCount();
	CString strSrcVol, strSrcVolCode, strSrcCopyCode, strSrcShelfCode;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strSrcVolCode = pDM_BOOK->GetCellData(_T("BB_��å��ȣ"), i);
		strSrcVol = pDM_BOOK->GetCellData(_T("BB_�����"), i);
		strSrcCopyCode = pDM_BOOK->GetCellData(_T("BB_������ȣ"), i);
		if((strSrcVol == strVol || strSrcVolCode == strVol) && strCopyCode == strSrcCopyCode)
		{
			if(pDM_BOOK->GetCellData(_T("BB_��Ϲ�ȣ"), i).IsEmpty())
			{
				pDM_BOOK->SetCellData(_T("BB_��Ϲ�ȣ"), strRegNo, i);
				pDM_BOOK->SetCellData(_T("BB_��ġ��ȣ_�ڵ�"), strShelfCode, i);
				break;
			}
		}
	}
	
	return 0;
}

INT CErrorAdjustApi::RecycleDataRestore()
{
	CESL_DataMgr * pDM_INDEX = NULL;
	CESL_DataMgr * pDM_SPECIES = NULL;
	CESL_DataMgr * pDM_BOOK = NULL;

	pDM_INDEX = (CESL_DataMgr*)GetTempPtr(_T("����DM"));
	pDM_SPECIES = (CESL_DataMgr*)GetTempPtr(_T("��DM"));
	pDM_BOOK = (CESL_DataMgr*)GetTempPtr(_T("åDM"));

	if(pDM_SPECIES == NULL || pDM_INDEX == NULL || pDM_BOOK == NULL)
		return -1;

	pDM_SPECIES->StartFrame();
	//����-�� �۾����� ����
	CString strWorkStatus = pDM_INDEX->GetCellData(_T("IBS_�۾�����INFO"), 0);
	CString strSpeciesKey = pDM_INDEX->GetCellData(_T("IBS_REC_KEY"), 0);
	if(strWorkStatus.Left(3) == _T("BOL"))
	{
		pDM_INDEX->SetCellData(_T("IBS_�����а�"), _T("1"), 0);
		pDM_SPECIES->SetCellData(_T("BS_�۾����±���"), _T("0"), 0);
	}
	else
	{
		pDM_INDEX->SetCellData(_T("IBS_�����а�"), _T("0"), 0);
		pDM_SPECIES->SetCellData(_T("BS_�۾����±���"), _T("1"), 0);
	}

	CArray<CString, CString> RemoveAliasArray;
	RemoveAliasArray.RemoveAll();
	//RemoveAliasArray.Add(_T("BS_MARC"));
	RemoveAliasArray.Add(_T("IBS_������"));
	RemoveAliasArray.Add(_T("IBS_REC_KEY"));
	INT ids = -1;
	CString strQuery;
	//���� �� ���� �ӽ����̺� Insert
	ids = pDM_INDEX->MakeQueryByDM(RemoveAliasArray, pDM_INDEX, pDM_INDEX->TBL_NAME, 0, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
	if(ids < 0) return ids;
	ids = pDM_SPECIES->MakeQueryByDM(RemoveAliasArray, pDM_SPECIES, _T("BO_SPECIES_TBL"), 0, pDM_SPECIES);
	if(ids < 0) return ids;
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	strQuery.Format(
// 		_T("INSERT INTO idx_tmp_bo_tbl VALUES (")
// 		_T("ESL_SEQ.NEXTVAL, %s, 'I', SYSDATE);"),
// 		strSpeciesKey);
// 	ids = pDM_SPECIES->AddFrame(strQuery);
// 	if(ids < 0) return ids;
	//=====================================================
	

	//å Insert
	CString strRegNo, strAccessionRecKey, strBookKey;
	INT nBookCnt = pDM_BOOK->GetRowCount();
	for(INT i=0 ; i<nBookCnt ; i++)
	{
		ids = pDM_BOOK->MakeQueryByDM(RemoveAliasArray, pDM_BOOK, _T("BO_BOOK_TBL"), i, pDM_SPECIES);
		if(ids < 0) return ids;
		strRegNo = pDM_BOOK->GetCellData(_T("BB_��Ϲ�ȣ"), i);
		strAccessionRecKey = pDM_BOOK->GetCellData(_T("BB_���ι�ȣKEY"), i);
		strBookKey = pDM_BOOK->GetCellData(_T("BB_åKEY"), i);
		strQuery.Empty();
		if(!strRegNo.IsEmpty() && !strAccessionRecKey.IsEmpty())
		{
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET REG_NO='%s', ACCESSION_REC_KEY=%s, LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strRegNo, strAccessionRecKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strBookKey);
		}
		else if(strRegNo.IsEmpty() && !strAccessionRecKey.IsEmpty())
		{
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY=%s, LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strAccessionRecKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strBookKey);
		}
		else if(!strRegNo.IsEmpty() && strAccessionRecKey.IsEmpty())
		{
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET REG_NO='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strRegNo, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strBookKey);
		}
		if(!strQuery.IsEmpty())
		{
			ids = pDM_SPECIES->AddFrame(strQuery);
			if(ids < 0) return ids;
		}
		if(!strRegNo.IsEmpty())
		{
			ids = DeleteToDropAccessionNoTbl(strRegNo, pDM_SPECIES);
			if(ids < 0) return ids;
		}
	}

	//å������ ���� �籸��
	strQuery.Format(
		_T("UPDATE IDX_BO_TBL SET	")
		_T("IDX_VOL=GET_BO_BOOK_INFO(%s,'VOL_INDEX'), ")
		_T("IDX_VOL_TITLE=GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX'), ")
		_T("USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
		_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
		_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
		_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
		_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE'), ")
		_T("LAST_WORK = '%s' where rec_key=%s;"), 
		strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
	ids = pDM_SPECIES->AddFrame(strQuery);
	if(ids < 0) return ids;
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	strQuery.Format(
// 		_T("INSERT INTO IDX_TMP_BO_TBL ")
// 		_T("(SELECT esl_seq.NEXTVAL, rec_key, 'I', SYSDATE ")
// 		_T("FROM IDX_BO_TBL ")
// 		_T("WHERE REC_KEY=%s);"),
// 		strSpeciesKey);
// 	pDM_SPECIES->AddFrame(strQuery);
	//=====================================================
	
	// ECOSEARCH�� ���� ����
	if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		ids = pDM_SPECIES->AddFrame(strQuery);
		if(ids < 0) return -1;
	}
	
	// ��Ȱ�뿡 �ִ� ��/å ����
	strQuery.Format(_T("DELETE FROM BO_DELETE_SPECIES_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
	ids = pDM_SPECIES->AddFrame(strQuery);
	if(ids < 0) return ids;

	for(i=0 ; i<nBookCnt ; i++)
	{
		strBookKey = pDM_BOOK->GetCellData(_T("BB_åKEY"), i);
		strQuery.Format(_T("DELETE FROM BO_DELETE_BOOK_TBL WHERE REC_KEY=%s;"), strBookKey);
		ids = pDM_SPECIES->AddFrame(strQuery);
		if(ids < 0) return -1;
	}

	// ��ũ ����
	CString strMarc = pDM_SPECIES->GetCellData(_T("BS_MARC"), 0);
	pDM_SPECIES->StartFrame();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM_SPECIES->MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
	
	// SENDFRAME
	ids = pDM_SPECIES->SendFrame();
	if(ids < 0) return ids;
	ids = pDM_SPECIES->EndFrame();
	if(ids < 0) return ids;
	
	pDM_SPECIES->EndFrame();
	return 0;
}

INT CErrorAdjustApi::DeleteToDropAccessionNoTbl(CString strRegNo, CESL_DataMgr * pDM_Out)
{
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	CString strCode = strRegNo.Left(2);
// 	INT nNumber = _ttoi((strRegNo.Right(10)).GetBuffer(0));
	CString strCode = strRegNo.Left(nRegCodeLength);
	INT nNumber = _ttoi((strRegNo.Right(12-nRegCodeLength)).GetBuffer(0));
	//===================================================================
	CString query, strTmpData;
	
	CString strRecKey;
	query.Format(
		_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE PUBLISH_FORM_CODE='%s' AND REG_CODE='%s' AND REG_NO=%d;"),
		_T("MO"),
		strCode,
		nNumber);
	INT ids = pDM_Out->AddFrame(query);
	if(ids < 0) return ids;
	return 0;
}

INT CErrorAdjustApi::SetRegNoInLoanInfo()
{
	CESL_DataMgr * pDM_INDEX = NULL;
	CESL_DataMgr * pDM_BOOK = NULL;
	pDM_BOOK = (CESL_DataMgr *)GetTempPtr(_T("åDM"));
	pDM_INDEX = (CESL_DataMgr *)GetTempPtr(_T("����DM"));
	if(pDM_INDEX == NULL || pDM_BOOK == NULL) return -1;

	CString strSpeciesKey, strQuery;
	strSpeciesKey = pDM_INDEX->GetCellData(_T("IBS_REC_KEY"), 0);
	strQuery.Format(
		_T("SELECT DISTINCT BOOK_KEY, REG_NO FROM LH_STORAGE_V01 WHERE SPECIES_KEY=%s AND STATUS IN ('0', '1', '2')"),
		strSpeciesKey);
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM_INDEX->CONNECTION_INFO);
	INT ids = tmpDM.RestructDataManager(strQuery);

	INT nRowCnt = tmpDM.GetRowCount();
	INT nBookCnt = pDM_BOOK->GetRowCount();
	if(nRowCnt == 0 || nBookCnt == 0) return 0;
	
	CString strBookKey, strRegNo;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strBookKey = tmpDM.GetCellData(i, 0);
		strRegNo = tmpDM.GetCellData(i, 1);
		for(INT j=0 ; j<nBookCnt ; j++)
		{
			if(strBookKey == pDM_BOOK->GetCellData(_T("BB_åKEY"), j))
			{
				pDM_BOOK->SetCellData(_T("BB_��Ϲ�ȣ"), strRegNo, j);
			}
		}
	}
	return 0;
}

INT CErrorAdjustApi::SetAppendixBookFlag()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));
	if(pDM == NULL) return -1;
	
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, 1);
	}

	CString strQuery;
	strQuery.Format(
		_T("UPDATE BO_BOOK_TBL SET BOOK_APPENDIX_FLAG='B', LAST_WORK='%s' WHERE ")
		_T("BOOK_APPENDIX_FLAG IS NULL AND WORKING_STATUS < 'BOL' AND WORKING_STATUS > 'BOR';"),
		m_pParentMgr->GetWorkLogMsg(WORK_LOG));
	INT ids = pDM->StartFrame();
	if(ids < 0) return -1;
	ids = pDM->AddFrame(strQuery);
	if(ids < 0) return -1;
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(1);
	}
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	pDM->EndFrame();

	return 0;
}

INT CErrorAdjustApi::RestructRequestIndexData()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));	
	if(pDM == NULL || pGrid == NULL) return -1;
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;
	
	CString strTmpData, strQuery;
	CArray <CString, CString> RemoveAliasList;	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("��ġ���KEY"));
	// StartFrame
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		
		strTmpData = pDM->GetCellData(_T("��ǥ��"), i);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		pDM->SetCellData(_T("��ǥ���ε���"), strTmpData, i);
		strTmpData = pDM->GetCellData(_T("������"), i);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		pDM->SetCellData(_T("�������ε���"), strTmpData, i);
		strTmpData = pDM->GetCellData(_T("������"), i);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		pDM->SetCellData(_T("�������ε���"), strTmpData, i);
		strTmpData = pDM->GetCellData(_T("��û��"), i);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		pDM->SetCellData(_T("��û���ε���"), strTmpData, i);

		strTmpData = pDM->GetCellData(_T("��ġ���KEY"), i);
		
		ids = pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, i, pDM, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
		if(ids < 0) return -1;
		
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	return 0;
}

INT CErrorAdjustApi::AdjustSpeciesError()
{
	INT ids;
	INT nSpeciesType = -1;		
	GetTempData(_T("SPECIES_TYPE"), nSpeciesType);
	
	if(nSpeciesType == 0 || nSpeciesType == 1)
		ids = AdjustSpeciesInfoError();
	else if(nSpeciesType == 2)
		ids = DeleteDupAddSpeciesInfo();
	else if(nSpeciesType == 3)
		ids = SyncIndexToBookWorkingStatus();
	return ids;
}

INT CErrorAdjustApi::SyncIndexToBookWorkingStatus()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));	
	if(pDM == NULL || pGrid == NULL) return -1;
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;
	
	CString strKey, strQuery;
	// StartFrame
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		
		strKey = pDM->GetCellData(_T("IBS_��KEY"), i);
		if(!strKey.IsEmpty())
		{
			//1. �˻����� ����ȭ�� ���� ť���̺� �α� ����
			//=====================================================
			//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 			strQuery.Format(
// 				_T("INSERT INTO idx_tmp_bo_tbl VALUES (")
// 				_T("ESL_SEQ.NEXTVAL, %s, 'I', SYSDATE);"),
// 				strKey);
// 			pDM->AddFrame(strQuery);	
			//=====================================================
			
			//���� �ڷ���� Update
			strQuery.Format(
				_T("UPDATE IDX_BO_TBL SET WORKING_STATUS=TESTWORKSTATUS(%s) WHERE REC_KEY = %s;"), strKey, strKey);
			pDM->AddFrame(strQuery);
			//ECO_SEARCH�� ����
			if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
			{
				strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strKey);
				pDM->AddFrame(strQuery);
			}
		}
		
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;
	
	return 0;
}

INT CErrorAdjustApi::DeleteDupAddSpeciesInfo()
{
	CString strQuery;
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));	
	if(pDM == NULL || pGrid == NULL) return -1;
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;
	
	CString strKey, strValue;
	// StartFrame
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		
		strKey = pDM->GetCellData(_T("IBS_��KEY"), i);
		if(!strKey.IsEmpty())
		{
			strQuery.Format(
				_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s"), strKey);
			pDM->GetOneValueQuery(strQuery, strValue);
			if(_ttoi(strValue.GetBuffer(0)) > 0)
			{
				strQuery.Format(_T("[%s : %s : %d] �ڷ�� å������ �����ϰ� �ֽ��ϴ�. �ٽ� Ȯ���� �ֽʽÿ�"),
					pDM->GetCellData(_T("IBS_��ǥ��"), i), pDM->GetCellData(_T("IBS_��KEY"), i), i);
			}
			else
			{
				//1. �˻����� ����ȭ�� ���� ť���̺� �α� ����
				//=====================================================
				//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 				strQuery.Format(
// 					_T("INSERT INTO idx_tmp_bo_tbl VALUES (")
// 					_T("ESL_SEQ.NEXTVAL, %s, 'D', SYSDATE);"),
// 					strKey);
// 				pDM->AddFrame(strQuery);
				//=====================================================
				
				//�� ���̺��� å ���� �ڷ� ����
				strQuery.Format(
					_T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY = %s;"), strKey);
				pDM->AddFrame(strQuery);
				//������������
				strQuery.Format(
					_T("DELETE FROM BO_PURCHASEINFO_TBL WHERE SPECIES_KEY=%s;"), strKey);
				pDM->AddFrame(strQuery);
				//������������
				strQuery.Format(
					_T("DELETE FROM BO_DONINFO_TBL WHERE SPECIES_KEY=%s;"), strKey);
				pDM->AddFrame(strQuery);
				//���� ���̺��� å ���� �ڷ� ����
				strQuery.Format(
					_T("DELETE FROM IDX_BO_TBL WHERE REC_KEY = %s;"), strKey);
				pDM->AddFrame(strQuery);
				//ECO_SEARCH�� ����
				if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
				{
					strQuery.Format(_T("ECO$P_DELETE_BO$I(%s);"), strKey);
					pDM->AddFrame(strQuery);
				}
				//�����ʷ� ����
				ids = BO_TOC_ABSTRACT_DELETE(pDM, strKey);
				//�����ڷ� �����÷��� �缳��
				strQuery.Format(
					_T("UPDATE BO_SPECIES_TBL SET DUP_FLAG=NULL WHERE REC_KEY IN ")
					_T("(SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE DUP_SPECIES_KEY=%s);"),
					strKey);
				pDM->AddFrame(strQuery);
				strQuery.Format(
					_T("UPDATE IDX_BO_TBL SET DUP_YN=NULL WHERE REC_KEY IN ")
					_T("(SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE DUP_SPECIES_KEY=%s);"),
					strKey);
				pDM->AddFrame(strQuery);
				if(ids < 0) return -1;			
			}
		}
		
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;
	
	return 0;
}

INT CErrorAdjustApi::AdjustSpeciesInfoError()
{
	INT nSpeciesType = -1;
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));	
	GetTempData(_T("SPECIES_TYPE"), nSpeciesType);
	if(pDM == NULL || pGrid == NULL || nSpeciesType < 0) return -1;
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;

	CString strKey;
	// StartFrame
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		
		strKey = pDM->GetCellData(_T("IBS_��KEY"), i);
		if(!strKey.IsEmpty())
		{
			ids = RestructSpeciesInfo(strKey, pDM, nSpeciesType);
		}

		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;
	
	return 0;
}

INT CErrorAdjustApi::RestructSpeciesInfo(CString strKey, CESL_DataMgr * pDM_OUT, INT nSpeciesType)
{
	if(pDM_OUT == NULL) return -1;

	CESL_DataMgr DM_SPECIES;
	DM_SPECIES.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);
	if(nSpeciesType == 0)
		DM_SPECIES.InitDMPKFromFile(_T("DMUP_����_��_������"));
	else if(nSpeciesType == 1)
		DM_SPECIES.InitDMPKFromFile(_T("DMUP_����_��API_����������"));
	DM_SPECIES.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
		
	CESL_DataMgr DM_IDX_BO;
	DM_IDX_BO.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);
	DM_IDX_BO.InitDMPKFromFile(_T("DMUP_����_��_��������"));
	DM_IDX_BO.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	CESL_DataMgr DM_BOOK;
	DM_BOOK.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);
	if(nSpeciesType == 0)
		DM_BOOK.InitDMPKFromFile(_T("DMUP_����_��_å����"));
	else if(nSpeciesType == 1)
		DM_BOOK.InitDMPKFromFile(_T("DMUP_����_��API_����å����"));
	DM_BOOK.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	INT ids;
	CString strQuery, strMarc, strTmpData;
	strQuery.Format(_T("REC_KEY = %s"), strKey);
	ids = DM_IDX_BO.RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	strQuery.Format(_T("SPECIES_KEY = %s"), strKey);
	ids = DM_BOOK.RefreshDataManager(strQuery);
	if(ids < 0) return -1;

	if(DM_IDX_BO.GetRowCount() == 0 || DM_BOOK.GetRowCount() == 0) return 0;
	strMarc.Empty();
	ids = MakeMarc(&DM_IDX_BO, &DM_BOOK, strMarc);
	if(ids < 0) return -1;

	DM_SPECIES.InsertRow(-1);
	DM_SPECIES.SetCellData(_T("BS_MARC"), strMarc, 0);
	m_pParentMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("�����ڷᱸ��"), strTmpData);
	DM_SPECIES.SetCellData(_T("BS_�����ڷᱸ��"), strTmpData, 0);
	strTmpData.Empty();
	
	strTmpData = DM_IDX_BO.GetCellData(_T("IBS_���ı���_�ڵ�"), 0);
	DM_SPECIES.SetCellData(_T("BS_���ı���_�ڵ�"), strTmpData, 0);
	strTmpData.Empty();
	strTmpData = DM_IDX_BO.GetCellData(_T("IBS_�ڷᱸ��_�ڵ�"), 0);
	DM_SPECIES.SetCellData(_T("BS_�ڷᱸ��_�ڵ�"), strTmpData, 0);
	strTmpData.Empty();
	strTmpData = DM_IDX_BO.GetCellData(_T("IBS_������ϱ���_�ڵ�"), 0);
	DM_SPECIES.SetCellData(_T("BS_������ϱ���_�ڵ�"), strTmpData, 0);	
	strTmpData.Empty();
	strTmpData = DM_IDX_BO.GetCellData(_T("IBS_��������_�ڵ�"), 0);
	DM_SPECIES.SetCellData(_T("BS_��������"), strTmpData, 0);
	strTmpData.Empty();
	strTmpData = DM_BOOK.GetCellData(_T("BB_�̿����ѱ���_�ڵ�"), 0);
	DM_SPECIES.SetCellData(_T("BS_�̿����ѱ���_�ڵ�"), strTmpData, 0);	
	strTmpData.Empty();
	strTmpData = DM_IDX_BO.GetCellData(_T("IBS_�̿��󱸺�_�ڵ�"), 0);
	DM_SPECIES.SetCellData(_T("BS_�̿��󱸺�_�ڵ�"), strTmpData, 0);
	strTmpData.Empty();
	strTmpData = DM_IDX_BO.GetCellData(_T("IBS_�з�ǥ����"), 0);
	DM_SPECIES.SetCellData(_T("BS_�з�ǥ����"), strTmpData, 0);
	
	//����������
	DM_SPECIES.SetCellData(_T("BS_��KEY"), strKey, 0);
	DM_SPECIES.SetCellData(_T("BS_�۾���"), m_pParentMgr->GetUserID(), 0);
	DM_SPECIES.SetCellData(_T("BS_�۾����±���"), _T("1"), 0);
	DM_SPECIES.SetCellData(_T("BS_�������۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
	DM_SPECIES.SetCellData(_T("BS_�����۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
	DM_SPECIES.SetCellData(_T("BS_�Է���"), GetTodayDate(), 0);

	CArray <CString, CString> RemoveAliasList;	RemoveAliasList.RemoveAll();
	ids = DM_SPECIES.MakeQueryByDM(RemoveAliasList, &DM_SPECIES, DM_SPECIES.TBL_NAME, 0, pDM_OUT);

	return ids;
}

INT CErrorAdjustApi::MakeMarc(CESL_DataMgr *pDM_IDX_BO, CESL_DataMgr *pDM_BOOK, CString &strMarc)
{
	INT nSpeciesMarcRelation = 0;
	CString SpeciesMarcRelation_Species[255];
	CString SpeciesMarcRelation_Marc[255];
	
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

	CMarc marc;
	if (pDM_IDX_BO == NULL) return -1;
	if (pDM_BOOK == NULL) return -3;
	
	INT nIdxRowIdx = 0;
	INT nREPRESENTBOOKRowIdx = 0;
	CString strDM, strData1, strData2, strData3;

	CESL_DataMgr DM_VOLUME;
	DM_VOLUME.SetCONNECTION_INFO(pDM_IDX_BO->CONNECTION_INFO);
	DM_VOLUME.InitDMPKFromFile(_T("DMUP_����_��_������"));
	DM_VOLUME.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	CONVERT_BOOK_to_VOLUME(pDM_BOOK, &DM_VOLUME);
	
	//��ũ�޴��� �׷켳��
	strData1 = pDM_IDX_BO->GetCellData(_T("IBS_���ı���_�ڵ�"), nIdxRowIdx);
	if ( strData1 == _T("TX") )
		m_pParentMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("����"));
	else if ( strData1 == _T("VM") ) 
		m_pParentMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("�񵵼�_��û���ڷ�"));
	else if ( strData1 == _T("CF") ) 
		m_pParentMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("�񵵼�_��ǻ������"));
	else if ( strData1 == _T("MP") ) 
		m_pParentMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("�񵵼�_�����ڷ�"));
	else if ( strData1 == _T("MU") ) 
		m_pParentMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("�񵵼�_�����ڷ�"));
	else 
		m_pParentMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("����"));
	
	// ���� ���� ���� (����߰���)
	CString strLeader;
	marc.GetLeader(strLeader);
	// ���ڵ� ���� ������ �ִ´�. 5��°
	if (strLeader.GetAt(5) == ' ') strLeader.SetAt(5, 'n');
	// ���ڵ� ���� ������ �ִ´�. 6��°
	if (strLeader.GetAt(6) == ' ') strLeader.SetAt(6, 'a');
	// �������� ������ �ִ´�. 7��°
	if (strLeader.GetAt(7) == ' ') strLeader.SetAt(7, 'm');

	// �Ϲ����� ����Ÿ ó��
	for (INT i = 0; i < nSpeciesMarcRelation; i++) {
		strDM = SpeciesMarcRelation_Species[i].Left(2);
		strData1.Empty();
		if (strDM == _T("IB")) pDM_IDX_BO->GetCellData(SpeciesMarcRelation_Species[i], nIdxRowIdx, strData1);
		if (strDM == _T("BB")) pDM_BOOK->GetCellData(SpeciesMarcRelation_Species[i], nREPRESENTBOOKRowIdx, strData1);
		if (strData1.GetLength() > 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(&marc, SpeciesMarcRelation_Marc[i], strData1, _T(""), MODIFY_ITEM);
		else m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(&marc, SpeciesMarcRelation_Marc[i]);
	}
	//008����� �ʵ� ����
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("008"));
	strData1 = pDM_IDX_BO->GetCellData(_T("IBS_�����"), 0);
	Apply_MARC_950(strData1, strData2, strData3);
	if(strData2.GetLength() == 4)
	{	
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem( &marc, _T("008@6"), _T("s") );
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem( &marc, _T("008@7-10"), strData2);		
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(&marc, _T("008@38-39"), _T("  ") );
	}
	
	// ��ǥ��
	strData1 = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_��ǥ��"), nIdxRowIdx, strData1);	
	Set_TITLE(&marc, strData1);
	// ������
	strData1 = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_������"), nIdxRowIdx, strData1);
	if (strData1.GetLength() == 0) pDM_BOOK->GetCellData(_T("BB_������"), nREPRESENTBOOKRowIdx, strData1);
	Set_AUTHOR(&marc, strData1);

	// �����
	strData1 = pDM_IDX_BO->GetCellData(_T("IBS_�����"), nIdxRowIdx);
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(&marc, _T("245$n"));
	if(!strData1.IsEmpty())
	{
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(&marc, _T("245$n"), strData1);
	}
	
	// ����(�Ǽ���)
	if (DM_VOLUME.GetRowCount() == 1) 
	{
		m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(&marc, _T("245$p"));
		strData1 = _T("");
		pDM_BOOK->GetCellData(_T("BB_������ηϸ�"), nREPRESENTBOOKRowIdx, strData1);
		if (strData1.GetLength() > 0) 
		{
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(&marc, _T("245$p"), strData1);
		}
	}
	
	//������
	strData1 = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_������"), nIdxRowIdx, strData1);
	Set_PUBLIHSER(&marc, strData1);

	Apply_MARC_700Author(&marc);

	SetISBN_to_Marc_ByVolume(&marc, &DM_VOLUME, _T("020"), _T("  "));		// IBS_����_ISBN is 020 blank blank
	// Ư���� ����Ÿ ó�� - 020(��Ʈ)
	SetISBN_to_Marc(&marc, pDM_IDX_BO, nIdxRowIdx, _T("020"), _T("1 "));		// IBS_��Ʈ_ISBN is 020 1 blank

	INT ids = m_pParentMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if(ids < 0) return -1;
	
	return 0;
}

INT CErrorAdjustApi::CONVERT_BOOK_to_VOLUME(CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_VOLUME)
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

INT CErrorAdjustApi::Set_TITLE(CMarc *pMarc, CString strData)
{
	if (pMarc == NULL) return -1;
	//if (strData.GetLength() == 0) return 0;
	//������� 245�ʵ�  $a, $x, $b Ŭ����
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$a"));
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$x"));
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$b"));
	//m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("245"));
	
	INT pos = strData.Find(_T("="), 0);
	CString strData1, strData2, strData3;
	if (pos < 0) 
	{
		strData1 = strData;
		pos = strData.Find(_T(":"), 0);
		if(pos < 0)
		{
			pos = strData.Find(_T("("));
			if(pos < 0 || pos != 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
			else 
			{
				pos = strData.Find(_T(")"));
				if(pos < 0) 
				{
					strData1.Replace(_T("("), _T(""));
					m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
				}
				else
				{
					m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
					m_pParentMgr->m_pInfo->pMarcMgr->SetTagIndicator(pMarc, _T("245"), _T("2"), _T("0"));
				}
			}
		}
		else
		{
			strData1 = strData.Left(pos);
			strData2 = strData.Mid(pos+1);
			pos = strData1.Find(_T("("));
			if(pos < 0 || pos != 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
			else 
			{
				pos = strData.Find(_T(")"));
				if(pos < 0) 
				{
					strData1.Replace(_T("("), _T(""));
					m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
				}
				else
				{
					m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
					m_pParentMgr->m_pInfo->pMarcMgr->SetTagIndicator(pMarc, _T("245"), _T("2"), _T("0"));
				}
			}
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$b"), strData2, _T(""), MODIFY_ITEM);
		}
	}
	else 
	{
		strData1 = strData.Left(pos);
		strData2 = strData.Mid(pos+1);
		pos = strData1.Find(_T("("));
		if(pos < 0 || pos != 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
		else 
		{
			pos = strData.Find(_T(")"));
			if(pos < 0) 
			{
				strData1.Replace(_T("("), _T(""));
				m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
			}
			else
			{
				m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
				m_pParentMgr->m_pInfo->pMarcMgr->SetTagIndicator(pMarc, _T("245"), _T("2"), _T("0"));
			}
		}
		pos = strData2.Find(_T(":"), 0);
		if(pos < 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$x"), strData2, _T(""), MODIFY_ITEM);
		else 
		{
			strData1 = strData2.Left(pos);
			strData3 = strData2.Mid(pos+1);
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$x"), strData1, _T(""), MODIFY_ITEM);
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$b"), strData3, _T(""), MODIFY_ITEM);
		}
	}
	return 0;
}

INT CErrorAdjustApi::Set_AUTHOR(CMarc *pMarc, CString strIndexData)
{
	if (pMarc == NULL) return -1;
	INT pos = -1;
	INT nPrevPos = 0;
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$d"));
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$e"));
	CString strData1;
	for (INT i = 0; true; i++) {
		strData1 = _T("");
		pos = strIndexData.Find(';', nPrevPos);
		if (pos == -1) strData1 = strIndexData.Mid(nPrevPos);
		else strData1 = strIndexData.Mid(nPrevPos, pos - nPrevPos);
		
		nPrevPos = pos+1;
		if (i == 0) 
		{
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$d"), strData1);
		}
		else
		{
			if (strData1.GetLength() > 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$e"), strData1);
		}
		if (pos == -1) break;
	}
	return 0;
}

INT CErrorAdjustApi::Set_PUBLIHSER(CMarc *pMarc, CString strIndexData)
{
	if (pMarc == NULL) return -1;
	INT pos = -1;
	INT nPrevPos = 0;
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("260$b"));
	CString strData1;
	for (INT i = 0; true; i++) {
		strData1 = _T("");
		pos = strIndexData.Find(':', nPrevPos);
		if (pos == -1) strData1 = strIndexData.Mid(nPrevPos);
		else strData1 = strIndexData.Mid(nPrevPos, pos - nPrevPos);
		
		nPrevPos = pos+1;
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("260$b"), strData1);
		if (pos == -1) break;
	}
	return 0;
}

INT CErrorAdjustApi::Apply_MARC_700Author(CMarc *pMarc)
{
	if (pMarc == NULL) return -1;
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("700"));

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
	
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$e"), strData, &StrArrayFrom);	
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
			m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("700$a"), strTmpData, &StrArrayOrigin1);
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

			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("700$a"), strData);
		}
	}

	/**
		245$d -> 700$a
	*/
	StrArrayFrom.RemoveAll();
	StrArrayOrigin1.RemoveAll();
	StrArrayOrigin2.RemoveAll();
	
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strData, &StrArrayFrom);	
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
			m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("700$a"), strTmpData, &StrArrayOrigin1);
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

			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("700$a"), strData);
		}
	}

	return 0;
}

INT CErrorAdjustApi::SetISBN_to_Marc_ByVolume(CMarc *pMarc, CESL_DataMgr *pDM_VOLUME, CString strTag, CString strInd)
{
	// _T("020  ") �� ��� ������.
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("020"), ' ', ' ');
	
	CString strData1=_T(""), strData2=_T(""), strData3=_T(""), strData4= _T("");
	CString strMarcData1=_T(""), strMarcData2=_T(""), strMarcData3=_T(""), strMarcData4=_T("");
	CString strIndexData=_T(""), strNewIndexData=_T("");
	CString strPrice, strVol, strCurrencyCode, strTmpData;
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strData1);
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
		
		if (strData1.GetLength() > 0) strMarcData1.Format(_T("$a%s"), strData1);
		else continue;
		if (strData2.GetLength() > 0) strMarcData2.Format(_T("$g%s"), strData2);
		if (strData3.GetLength() > 0) strMarcData3.Format(_T("$z%s"), strData3);
		if (strData4.GetLength() > 0) strMarcData4.Format(_T("$c%s%s"), strCurrencyCode, strData4);
		if (strData1.GetLength() == 0 && strData2.GetLength() == 0 && strData3.GetLength() == 0 && strData4.GetLength() == 0) 
		{
			;
		} 
		else 
		{
			strNewIndexData.Format(_T("020  %s%s%s%s"), strMarcData1, strMarcData2, strMarcData3, strMarcData4);
		}
		if (strNewIndexData.GetLength() > 0) m_pParentMgr->m_pInfo->pMarcMgr->InsertField(pMarc, strNewIndexData);
	}
	return 0;
}

INT CErrorAdjustApi::SetISBN_to_Marc(CMarc *pMarc, CESL_DataMgr *pDM_IDX_BO, INT nRowIdx, CString strTag, CString strInd)
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
		
		if (strData1.GetLength() > 0) strMarcData1.Format(_T("$a%s"), strData1);
		if (strData2.GetLength() > 0) strMarcData2.Format(_T("$g%s"), strData2);
		if (strData3.GetLength() > 0) strMarcData3.Format(_T("$z%s"), strData3);
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
		
		if (strData1.GetLength() > 0) strMarcData1.Format(_T("$a%s"), strData1);
		if (strData2.GetLength() > 0) strMarcData2.Format(_T("$g%s"), strData2);
		if (strData3.GetLength() > 0) strMarcData3.Format(_T("$z%s"), strData3);
		
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
	m_pParentMgr->m_pInfo->pMarcMgr->GetField(pMarc, strTag, strField, &Array);
	INT nSize = Array.GetSize();
	if (nSize == 0) {		// Marc has not any isbn field data
		m_pParentMgr->m_pInfo->pMarcMgr->InsertField(pMarc, strNewIndexData);
	} else {						// Marc has some isbn field data
		BOOL bIsUpdate = FALSE;
		for (INT i = 0; i < nSize; i++) {
			if (Array.GetAt(i).GetLength() < 2) continue;
			strTmpInd = Array.GetAt(i).Left(2);
			
			if (strTmpInd == strInd) {
				strIndexData.Format(_T("%s%s"),  strTag, Array.GetAt(i));
				m_pParentMgr->m_pInfo->pMarcMgr->UpdateField(pMarc, strIndexData, strNewIndexData);
				bIsUpdate = TRUE;
				break;
			}
		}
		if (bIsUpdate == FALSE) m_pParentMgr->m_pInfo->pMarcMgr->InsertField(pMarc, strNewIndexData);
	}
	return 0;
}

INT CErrorAdjustApi::CopyDMRowToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
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

INT CErrorAdjustApi::GetEqualBookCount(CESL_DataMgr *pDM_BOOK, CString &strVOL)
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

INT CErrorAdjustApi::BO_BOOK_DELETE(CString strSpeciesKey,
								CString strAccuKey,
								CString strBookRecKey,
								INT nAcqType,
								CESL_DataMgr * pDM_OUT)
{
	INT nRowIdx = 0;
	
	CString strOption, strQuery, strKey, strRegNo, strMarc, strTmpData, strBookQuery;
	strKey = strSpeciesKey;
	
	if(!CheckHaveOtherBookInfo(pDM_OUT, strSpeciesKey, strAccuKey))    
	{
		return BO_SPECIES_DELETE(strSpeciesKey, strAccuKey, nAcqType, pDM_OUT);
	}
	// �ش� ���� åDM�� �����Ѵ�.
	strOption.Format(_T("DELETE BO_BOOK_TBL WHERE REC_KEY=%s;"), strBookRecKey);
	pDM_OUT->AddFrame(strOption);
	//�̿����ѱ���/��������/�ڷ��/��ϱ��� ���а��� ���а� concatenation
	strOption.Format(_T("update idx_bo_tbl set USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
		_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
		_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
		_T("IDX_VOL_TITLE=GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX'), ")
		_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
		_T("IDX_VOL=GET_BO_BOOK_INFO(%s,'VOL_INDEX'), ")
		_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE'), ")
		_T("LAST_WORK='%s' where rec_key=%s;"), 
		strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
	pDM_OUT->AddFrame(strOption);	
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	strQuery.Format(
// 		_T("INSERT INTO IDX_TMP_BO_TBL ")
// 		_T("(SELECT esl_seq.NEXTVAL, rec_key, 'I', SYSDATE ")
// 		_T("FROM IDX_BO_TBL ")
// 		_T("WHERE REC_KEY=%s);"),
// 		strSpeciesKey);
// 	pDM_OUT->AddFrame(strQuery);
	//=====================================================
	
	//ECOSEARCH�� ���ؼ� �������
	if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strOption.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		pDM_OUT->AddFrame(strOption);
	}
	
	return 0;
}

BOOL CErrorAdjustApi::CheckHaveOtherBookInfo(CESL_DataMgr * pDM, CString strSpeciesKey, CString strAccuKey)
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

BOOL CErrorAdjustApi::CheckHaveOtherAccuInfo(CESL_DataMgr * pDM, CString strSpeciesKey, CString strAccuKey)
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
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	tmpDM.RestructDataManager(strQuery);
	strTmpData = tmpDM.GetCellData(0, 0);
	INT nTmpData = _ttoi(strTmpData.GetBuffer(0));
	if(nTmpData > 0)
		return TRUE;
	else
		return FALSE;
	
	return FALSE;
}

INT CErrorAdjustApi::BO_SPECIES_DELETE(CString strSpeciesKey, 
								   CString strAccuKey, 
								   INT nAcqType,
								   CESL_DataMgr * pDM_OUT)
{
	if (strSpeciesKey.GetLength() == 0) return -31001;	
	INT nRowIdx = 0;

	INT ids;
	CArray <CString, CString> RemoveAliasList;
	BOOL bCreateSpeciesKey = FALSE;

	CString strOption, strQuery, strKey, strBookQuery, strTmpData;
	strKey = strSpeciesKey;

	CESL_DataMgr DM_BOOK;
	DM_BOOK.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);
	DM_BOOK.InitDMPKFromFile(_T("DMUP_����_��_å����"));
	DM_BOOK.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	// �ش� ���� åDM�� �����Ѵ�.
	if(!strAccuKey.IsEmpty() && strAccuKey != _T("-1") && strAccuKey != _T("ALL"))
		strOption.Format(_T("SPECIES_KEY = %s AND ACQ_KEY = %s AND (WORKING_STATUS<'BOL' OR WORKING_STATUS>'BOR')"), strSpeciesKey, strAccuKey);
	else if(strAccuKey == _T("ALL"))
		strOption.Format(_T("SPECIES_KEY = %s AND (WORKING_STATUS<'BOL' OR WORKING_STATUS>'BOR')"), strSpeciesKey);
	else
		strOption.Format(_T("SPECIES_KEY = %s AND (ACQ_KEY IS NULL OR ACQ_KEY = -1) AND (WORKING_STATUS<'BOL' OR WORKING_STATUS>'BOR')"), strSpeciesKey);
	ids = DM_BOOK.RefreshDataManager(strOption);
	if(ids < 0) return -31003;

	//������ å������ ���ٸ� return
	if(DM_BOOK.GetRowCount() == 0) return 1000;
	
	// ������ Book������ DB_FLAG�� _T("D")�� �����Ѵ�.
	ids = DM_BOOK.GetRowCount();
	for(INT idx=0 ; idx<ids ; idx++)
	{
		DM_BOOK.SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}
	
	// �������� Insert�� Insert���� �����Ѵ�.
	RemoveAliasList.RemoveAll();
		
	if(!CheckHaveOtherAccuInfo(pDM_OUT, strSpeciesKey, strAccuKey))
	{
		strQuery.Format(_T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
		ids = pDM_OUT->AddFrame(strQuery);
		if(ids < 0) return -1;
		strQuery.Format(_T("DELETE FROM IDX_BO_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
		ids = pDM_OUT->AddFrame(strQuery);
		if(ids < 0) return -1;
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		BO_INDEX_TEMP_INSERT(pDM_OUT, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("D"));	
		//=====================================================
		
		BO_TOC_ABSTRACT_DELETE(pDM_OUT, strSpeciesKey);
	}
	
	// ����å������ Insert�� Insert���� �����Ѵ�.
	// ����å������ �����Ҷ� �� å�� �ο��Ǿ��� ��Ϲ�ȣ�� CStringArray�� �����Ѵ�.
	INT nRowCount = DM_BOOK.GetRowCount();
	MakeBookQueryFrame(&DM_BOOK, strSpeciesKey, strAccuKey, pDM_OUT);
	
	//����/�������� ����
	if(nAcqType == 1 || nAcqType == 2)
	{
		DeleteAccuInfo(pDM_OUT, strAccuKey, nAcqType);
	}
	else if(nAcqType == 0 && !strAccuKey.IsEmpty())
	{
		if(DM_BOOK.GetCellData(_T("BB_���Ա���_�ڵ�"), 0) == _T("1")) nAcqType = 1;
		else if(DM_BOOK.GetCellData(_T("BB_���Ա���_�ڵ�"), 0) == _T("2")) nAcqType = 2;
		DeleteAccuInfo(pDM_OUT, strAccuKey, nAcqType);
	}
	
	if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_DELETE_BO$I(%s);"), strSpeciesKey);
		pDM_OUT->AddFrame(strQuery);
	}
	
	return 0;
}

INT CErrorAdjustApi::BO_INDEX_TEMP_INSERT(CESL_DataMgr * pDM_OUT, CString strTblName, CString strSpeciesKey, CString strWorkFlag)
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

INT CErrorAdjustApi::MakeBookQueryFrame(CESL_DataMgr *pDM_BOOK, CString strSpecies_key, CString strPurchaseinfo_key, CESL_DataMgr *pDM_OUT)
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
	//APPLY_SetAppendixInfo_to(pDM_BOOK, pDM_BOOK);

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
			pDM_BOOK->SetCellData(_T("BB_�������۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			pDM_BOOK->SetCellData(_T("BB_�����۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			if (pDM_BOOK->GetCellData(_T("BB_����"), i).GetLength() == 0) pDM_BOOK->SetCellData(_T("BB_����"), _T("0"), i);
			//����������� ���� �� ����ISBN�� Ư����ȣ ������
			strEAISBN = pDM_BOOK->GetCellData(_T("BB_����ISBN"), i); 
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strEAISBN);
			pDM_BOOK->SetCellData(_T("BB_����ISBN"), strEAISBN, i);
			volIndex = pDM_BOOK->GetCellData(_T("BB_������ηϸ�"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(volIndex);
				pDM_BOOK->SetCellData(_T("BB_������ηϸ����"), volIndex, i);
			}
			volIndex = pDM_BOOK->GetCellData(_T("BB_�����"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(volIndex);
				pDM_BOOK->SetCellData(_T("BB_���������"), volIndex, i);
			}
			strBook_key = pDM_BOOK->GetCellData(_T("BB_åKEY"), i);
			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_BOOK, _T("BO_BOOK_TBL"), i, pDM_OUT);
		}
		if (strDBFLAG == _T("U")) {
			RemoveAliasList.Add(_T("BB_åKEY"));
			RemoveAliasList.Add(_T("BB_������Ϲ�ȣ"));
			ids = pDM_BOOK->GetCellData(_T("BB_åKEY"), i, strBook_key);
			pDM_BOOK->SetCellData(_T("BB_�������۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			//����������� ���� �� ����ISBN�� Ư����ȣ ������
			strEAISBN = pDM_BOOK->GetCellData(_T("BB_����ISBN"), i); 
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strEAISBN);
			pDM_BOOK->SetCellData(_T("BB_����ISBN"), strEAISBN, i);			
			volIndex = pDM_BOOK->GetCellData(_T("BB_������ηϸ�"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				pDM_BOOK->SetCellData(_T("BB_������ηϸ����"), volIndex, i);
			}
			volIndex = pDM_BOOK->GetCellData(_T("BB_�����"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				pDM_BOOK->SetCellData(_T("BB_���������"), volIndex, i);
			}
			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_BOOK, _T("BO_BOOK_TBL"), i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strBook_key);
			strBook_key = pDM_BOOK->GetCellData(_T("BB_åKEY"), i);
		}
		if (strDBFLAG == _T("D")) {
			ids = pDM_BOOK->GetCellData(_T("BB_åKEY"), i, strBook_key);
			if(strBook_key.IsEmpty()) strBook_key = _T("-1");
			strQuery.Format(_T("DELETE FROM BO_BOOK_TBL WHERE REC_KEY=%s;"), strBook_key);
			pDM_OUT->AddFrame(strQuery);		
		}
	}

	return 0;
}

INT CErrorAdjustApi::InsertToDropAccessionNoTbl(CString strRegNo, CESL_DataMgr * pDM_Out, BOOL IsCheckInsert)
{
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	CString strCode = strRegNo.Left(2);
// 	INT nNumber = _ttoi((strRegNo.Right(10)).GetBuffer(0));
	CString strCode = strRegNo.Left(nRegCodeLength);
	INT nNumber = _ttoi((strRegNo.Right(12-nRegCodeLength)).GetBuffer(0));
	//===================================================================
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

INT CErrorAdjustApi::DeleteAccuInfo(CESL_DataMgr *pDM_OUT, CString strAccuKey, INT nAcqType)
{
	CString strQuery, strTmpData, strPrice, strDiscRate, strSpeciesKey;
	if(strAccuKey == _T("ALL"))
	{
		strSpeciesKey = pDM_OUT->GetCellData(_T("BS_��KEY"), 0);
	}
		
	if(nAcqType == 0) return 0;
	else if(nAcqType == 1) 
	{
		if(!strAccuKey.IsEmpty())
			strQuery.Format(_T("DELETE BO_PURCHASEINFO_TBL WHERE REC_KEY=%s;"), strAccuKey);
	}
	else if(nAcqType == 2)
	{
		if(!strAccuKey.IsEmpty())
			strQuery.Format(_T("DELETE BO_DONINFO_TBL WHERE REC_KEY=%s;"), strAccuKey);
	}
	
	pDM_OUT->AddFrame(strQuery);
	return 0;	
}

INT CErrorAdjustApi::DeleteDataBook()
{
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CString strSpeciesKeyAlias, strAccuKeyAlias, strBookKeyAlias, strRegNoAlias, strTmpData;
	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));	
	GetTempData(_T("��Ű�˸��ƽ�"), strSpeciesKeyAlias);
	GetTempData(_T("��������Ű�˸��ƽ�"), strAccuKeyAlias);
	GetTempData(_T("åŰ�˸��ƽ�"), strBookKeyAlias);
	
	INT nCnt = 0;
	
	if(!pDM || !pGrid) return -1000;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1000;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	pDM->StartFrame();
	CString strSpeciesKey, strAccuKey, strBookRecKey;
	while(i >= 0)
	{	
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		pDM->GetCellData(strSpeciesKeyAlias, i, strSpeciesKey);
		pDM->GetCellData(strAccuKeyAlias, i, strAccuKey);
		pDM->GetCellData(strBookKeyAlias, i, strBookRecKey);
		ids = BO_BOOK_DELETE(strSpeciesKey, strAccuKey, strBookRecKey, 0, pDM);
		if(ids < 0) return -1;
		ArrIdx.Add(i);
		if(nCnt == 50)
		{
			ids = pDM->SendFrame();
			if(ids < 0) return ids;
			ids = pDM->EndFrame();
			if(ids < 0) return ids;
			pDM->StartFrame();
			nCnt = 0;
		}
		else
			nCnt++;
		
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	for(i=ArrIdx.GetSize()-1 ; i>=0 ; i--)
	{
		pDM->DeleteRow(ArrIdx.GetAt(i));
	}
	
	return 0;
}

INT CErrorAdjustApi::MarcUpdate()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));
	if(pDM == NULL || pGrid == NULL) return -1;
		
	CMarcAdjustCondition Cond(m_pParentMgr);
	CMarcAdjustDlg dlg(m_pParentMgr);
	dlg.SetCondition(&Cond);
	if(IDOK != dlg.DoModal()) return 0;

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}	
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;
	
	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	// StartFrame
	pDM->StartFrame();
	CStringArray ArrData;
	
	CString strMarc, strKey, strQuery, strTmpData;
	
	CMarc marc;
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}

		ids = Cond.Update(pDM, i);
		if(ids >= 0)
			ArrIdx.Add(i);
		
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	pDM->EndFrame();
	DeleteSelectRow(pDM, &ArrIdx);
	return ids;
}
