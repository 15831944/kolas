// MakeVolumePrintDM.cpp: implementation of the CMakeVolumePrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeVolumePrintDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeVolumePrintDM::CMakeVolumePrintDM(CESL_Mgr * pMgr) : CMakePrintDM(pMgr)
{
	m_cVolumeTotalInfo.InitTotalInfo();
}

CMakeVolumePrintDM::~CMakeVolumePrintDM()
{

}

INT CMakeVolumePrintDM::InitMakePrintDM()
{
	if(m_nPrintType < 0) return -1;
	else if(m_nPrintType == BO_ACQ_CHECKIN_DATA)
	{
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalDeliveryBookCntAlias = _T("UDF_��ǰå��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ���ΰ���");
		m_strTotalSpeciesCntAlias = _T("BP_ȭ�󱸺�");
	}
	else if(m_nPrintType == BO_ACQ_NONCHECKIN_DATA)
	{
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalMissingBookCntAlias = _T("UDF_�̳�å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ���ΰ���");
		m_strTotalSpeciesCntAlias = _T("BP_ȭ�󱸺�");
	}
	else if(m_nPrintType == BO_ACQ_CHECKINING_DATA)
	{
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalMissingBookCntAlias = _T("UDF_�̳�å��");
		m_strTotalDeliveryBookCntAlias = _T("UDF_��ǰå��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ���ΰ���");
		m_strTotalSpeciesCntAlias = _T("BP_ȭ�󱸺�");
	}
	else if(m_nPrintType == BO_ACQ_MISSING_DATA)
	{
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalMissingBookCntAlias = _T("UDF_�̳�å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ���ΰ���");
		m_strTotalSpeciesCntAlias = _T("IBS_������");
	}
	else if(m_nPrintType == BO_ACQ_PURCHASE_REGOBJ_DATA)
	{
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ���ΰ���");
		m_strTotalSpeciesCntAlias = _T("IBS_������");
	}
	SetTotalInfoAlias(m_strTotalBookCntAlias, m_strTotalPriceAlias, m_strTotalDiscPriceAlias,
		m_strTotalMissingBookCntAlias, m_strTotalDeliveryBookCntAlias);
	return 0;
}

INT CMakeVolumePrintDM::CheckValid()
{
	if(m_pPrintDM == NULL) return -1;
	if(m_pSourceBookDM == NULL) return -1;
	if(m_strDivAlias.IsEmpty()) return -1;
	return 0;
}

INT CMakeVolumePrintDM::MakePrintDM()
{
	INT ids ;
	if(m_nPrintType == BO_ACQ_CHECKIN_DATA)
		ids = MakeCheckInRelativeData(TRUE);
	else if(m_nPrintType == BO_ACQ_NONCHECKIN_DATA)
		ids = MakeCheckInRelativeData(FALSE);
/*
	else if(m_nPrintType == BO_ACQ_CHECKIN_REQUEST_DATA)
		ids = MakeCheckInRequestData();
*/
	else if(m_nPrintType == BO_ACQ_CHECKINING_DATA)
		ids = MakeCheckInListData();
	else if(m_nPrintType == BO_ACQ_MISSING_DATA || m_nPrintType == BO_ACQ_PURCHASE_REGOBJ_DATA)
		ids = MakeVolumeListData();
	return ids;
}

INT CMakeVolumePrintDM::MakeCheckInRelativeData(BOOL bCheckIn)
{
	ShowDataProcessingDlg();
	m_pPrintDM->FreeData();
	INT ids = MakeVolumeDM(m_pPrintDM, m_pSourceBookDM, m_strDivAlias, FALSE);
	if(ids < 0) return -1;
	
	SetUDFDMFieldData(m_pPrintDM, TRUE);
	
	CString strTmpData, strTmpAlias, strPrice, strDiscRate, strTmpPrice;
	DOUBLE dPrice;
	if(bCheckIn) strTmpAlias = _T("UDF_��ǰå��");
	else strTmpAlias = _T("UDF_�̳�å��");
	INT nRowCnt = m_pPrintDM->GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	for(INT i=nRowCnt-1 ; i>=0 ; i--)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nRowCnt-i);

		strTmpData = m_pPrintDM->GetCellData(strTmpAlias, i);
		if(strTmpData.IsEmpty() || strTmpData == _T("0"))
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		if(!CheckPrintData(m_pPrintDM->GetCellData(m_strDivAlias, i), m_pPrintDM->GetCellData(_T("BB_���Ϸù�ȣ"), i)))
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		strPrice = m_pPrintDM->GetCellData(_T("BB_����"), i);
		strDiscRate = m_pPrintDM->GetCellData(_T("BP_������"), i);
		dPrice = (DOUBLE)atof(strPrice.GetBuffer(0)) * (DOUBLE)atof(strTmpData.GetBuffer(0));
		strTmpPrice.Format(_T("%.0f"), dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpPrice, strTmpPrice);
		m_pPrintDM->SetCellData(_T("UDF_��ȯ����"), strTmpPrice, i);
		dPrice = dPrice - (dPrice * ((DOUBLE)atof(strDiscRate.GetBuffer(0))/100));
		strTmpPrice.Format(_T("%.0f"), dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpPrice, strTmpPrice);
		m_pPrintDM->SetCellData(_T("UDF_��ȯ���ΰ���"), strTmpPrice, i);
	}
	
	if(m_bDataSortYN) ids = PrintDataSortByUserSelect(m_pPrintDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintDM);

	
	ids = m_cVolumeTotalInfo.SetTotalInfo(m_pPrintDM);
	if(ids < 0) return ids;
	
	// �Ѱ������� �־ƾ���
	m_pPrintDM->InsertRow(-1);	m_pPrintDM->InsertRow(-1);
	SetTotalInfo(m_pPrintDM, &m_cVolumeTotalInfo, m_pPrintDM->GetRowCount()-1, _T("�Ѱ�"));
	return 0;
}

INT CMakeVolumePrintDM::MakeCheckInRequestData()
{
	ShowDataProcessingDlg();
	m_pPrintDM->FreeData();
	INT ids = MakeVolumeDM(m_pPrintDM, m_pSourceBookDM, m_strDivAlias, FALSE);
	if(ids < 0) return -1;
	
	SetUDFDMFieldData(m_pPrintDM, TRUE);
	
	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_���Գ⵵");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�׷��ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�����Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	CString strTmpData, strTmpAlias, strAcqKey, strQuery;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	
	strTmpAlias = _T("UDF_��ǰå��");
	INT nRowCnt = m_pPrintDM->GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	for(INT i=nRowCnt-1 ; i>=0 ; i--)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nRowCnt-i);
		
		strTmpData = m_pPrintDM->GetCellData(strTmpAlias, i);
		if(strTmpAlias.IsEmpty() || strTmpAlias == _T("0"))
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		strTmpData = m_pPrintDM->GetCellData(_T("BP_�Է±���"), i);
		if(strTmpAlias != _T("2"))
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		strAcqKey = m_pPrintDM->GetCellData(_T("BP_��������KEY"), i);
		if(strAcqKey.IsEmpty() || !CheckPrintData(strAcqKey, m_pPrintDM->GetCellData(_T("BB_���Ϸù�ȣ"), i))) 
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		
		strQuery.Format(_T("SELECT APPLICANT_NAME, APPLICANT_DATE FROM BO_FURNISH_TBL WHERE ACQ_KEY = %s"), strAcqKey);
		tmpDM.RestructDataManager(strQuery);
		if(tmpDM.GetRowCount() > 0) 
		{
			strTmpData = tmpDM.GetCellData(0, 0);
			m_pPrintDM->SetCellData(_T("UDF_��û��"), strTmpData, i);
			strTmpData = tmpDM.GetCellData(0, 1);
			m_pPrintDM->SetCellData(_T("UDF_��û��"), strTmpData, i);
		}
	}
	if(m_bDataSortYN) ids = PrintDataSortByUserSelect(m_pPrintDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintDM);

	return ids;
}

INT CMakeVolumePrintDM::PrintDataSortByBasicInfo(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_���Գ⵵");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�׷��ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�����Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return SetDataSerialNo(pDM);
}

INT CMakeVolumePrintDM::PrintDataSortByUserSelect(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	INT ids;
	INT nCondColumns = 0;
	CString strAlias[4];
	INT nCond[4];
	strAlias[nCondColumns] = _T("BP_���Գ⵵");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�׷��ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	
	if(m_pSortItem && m_bDataSortYN)
	{
		CStringArray * pTmpTail = NULL;	
		CStringArray SortConditionTail;	SortConditionTail.RemoveAll();
		pTmpTail = m_pSortItem->GetTailSortList();
		ids = CopySortItem(pTmpTail, &SortConditionTail);
		if(ids < 0) return -1;
		
		pTmpTail->Add(_T("BB_���Ϸù�ȣ"));
		ids = m_pSortItem->DataSort(pDM);
		CopySortItem(&SortConditionTail, pTmpTail);
		if(ids < 0) return -1;
	}
	else
	{
		return PrintDataSortByBasicInfo(pDM);
	}
	
	return SetDataSerialNo(pDM);
}

INT CMakeVolumePrintDM::MakeCheckInListData()
{
	ShowDataProcessingDlg();
	m_pPrintDM->FreeData();
	INT ids;
	CString strAcqKey, strVolSortNo, strPrice, strDiscRate, strTmpData, strTmpPrice;
	DOUBLE dPrice;
	INT nRowCnt = m_pSourceBookDM->GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	for(INT i=nRowCnt-1 ; i>=0 ; i--)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nRowCnt-i);
		
		if(!CheckPrintData(m_pSourceBookDM->GetCellData(m_strDivAlias, i), m_pSourceBookDM->GetCellData(_T("BB_���Ϸù�ȣ"), i)))
		{
			continue;
		}
		DWORD * pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		if(pRow == NULL) continue;
		ids = CopyDM(m_pSourceBookDM, i, m_pPrintDM, pRow);
		if(ids < 0) return -1;
		strTmpData = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_��ǰå��")));
		strPrice = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("BB_����")));
		strDiscRate = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("BP_������")));
		dPrice = (DOUBLE)atof(strPrice.GetBuffer(0)) * (DOUBLE)atof(strTmpData.GetBuffer(0));
		strTmpPrice.Format(_T("%.0f"), dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpPrice, strTmpPrice);
		m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_��ȯ����")), strTmpPrice);
		dPrice = dPrice - (dPrice * ((DOUBLE)atof(strDiscRate.GetBuffer(0))/100));
		strTmpPrice.Format(_T("%.0f"), dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpPrice, strTmpPrice);
		m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_��ȯ���ΰ���")), strTmpPrice);
	}
	
	ids = m_pPrintDM->RefreshPositionInfo();
	if(ids < 0) return -1;
	
	if(m_bDataSortYN) ids = PrintDataSortByUserSelect(m_pPrintDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintDM);
	
	ids = m_cVolumeTotalInfo.SetTotalInfo(m_pPrintDM);
	if(ids < 0) return ids;

	// �Ѱ������� �־ƾ���
	m_pPrintDM->InsertRow(-1);	m_pPrintDM->InsertRow(-1);
	SetTotalInfo(m_pPrintDM, &m_cVolumeTotalInfo, m_pPrintDM->GetRowCount()-1, _T("�Ѱ�"));

	return 0;
}

INT CMakeVolumePrintDM::MakeVolumeListData()
{
	ShowDataProcessingDlg();
	m_pPrintDM->FreeData();
	INT ids;
	CString strAcqKey, strVolSortNo, strTmpData;
	INT nRowCnt = m_pSourceBookDM->GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	for(INT i=nRowCnt-1 ; i>=0 ; i--)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nRowCnt-i);
		
		if(!CheckPrintData(m_pSourceBookDM->GetCellData(m_strDivAlias, i), m_pSourceBookDM->GetCellData(_T("BB_���Ϸù�ȣ"), i)))
		{
			continue;
		}
		DWORD * pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		if(pRow == NULL) continue;
		ids = CopyDM(m_pSourceBookDM, i, m_pPrintDM, pRow);
		if(ids < 0) return -1;
	}
	
	ids = m_pPrintDM->RefreshPositionInfo();
	if(ids < 0) return -1;
	
	if(m_bDataSortYN) ids = PrintDataSortByUserSelect(m_pPrintDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintDM);
	
	
	ids = m_cVolumeTotalInfo.SetTotalInfo(m_pPrintDM);
	if(ids < 0) return ids;
	
	// �Ѱ������� �־ƾ���
	m_pPrintDM->InsertRow(-1);	m_pPrintDM->InsertRow(-1);
	SetTotalInfo(m_pPrintDM, &m_cVolumeTotalInfo, m_pPrintDM->GetRowCount()-1, _T("�Ѱ�"));

	return 0;
}

VOID CMakeVolumePrintDM::SetTotalInfoAlias(CString strBookCntAlias, CString strPriceAlias, CString strDiscPriceAlias,
										   CString strMissingBookCntAlias, CString strDeliveryBookCntAlias)
{
	m_cVolumeTotalInfo.SetAliasInfo(strBookCntAlias, strPriceAlias, strDiscPriceAlias, strMissingBookCntAlias, strDeliveryBookCntAlias);
}

INT CMakeVolumePrintDM::SetTotalInfo(CESL_DataMgr * pDM, CVolumePrintTotalInfo * pTotalInfo, INT nRowIdx, CString strTitle)
{
	if(pTotalInfo == NULL) return -1;
	
	CString strTmpData;
	pDM->SetCellData(m_strTotalInfoTitle, strTitle, nRowIdx);
	
	if(!m_strTotalBookCntAlias.IsEmpty())
	{
		pTotalInfo->GetTotalBookCnt(strTmpData);
		pDM->SetCellData(m_strTotalBookCntAlias, strTmpData+_T("å"), nRowIdx);
	}
	if(!m_strTotalMissingBookCntAlias.IsEmpty())
	{
		pTotalInfo->GetTotalMissingBookCnt(strTmpData);
		pDM->SetCellData(m_strTotalMissingBookCntAlias, strTmpData+_T("å"), nRowIdx);
	}
	if(!m_strTotalDeliveryBookCntAlias.IsEmpty())
	{
		pTotalInfo->GetTotalDeliveryBookCnt(strTmpData);
		pDM->SetCellData(m_strTotalDeliveryBookCntAlias, strTmpData+_T("å"), nRowIdx);
	}
	if(!m_strTotalPriceAlias.IsEmpty())
	{
		pTotalInfo->GetTotalPrice(strTmpData);
		pDM->SetCellData(m_strTotalPriceAlias, strTmpData, nRowIdx);
	}
	if(!m_strTotalDiscPriceAlias.IsEmpty())
	{
		pTotalInfo->GetDiscTotalPrice(strTmpData);
		pDM->SetCellData(m_strTotalDiscPriceAlias, strTmpData, nRowIdx);
	}
	if(!m_strTotalSpeciesCntAlias.IsEmpty())
	{
		pTotalInfo->GetTotalSpeciesCnt(strTmpData);
		pDM->SetCellData(m_strTotalSpeciesCntAlias, strTmpData+_T("��"), nRowIdx);
	}
	
	return 0;
}