// MakeSubTotalPrintDM.cpp: implementation of the CMakeSubTotalPrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeSubTotalPrintDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������
CMakeSubTotalPrintDM::CMakeSubTotalPrintDM(CESL_Mgr * pMgr) : CMakePrintDM(pMgr)
{
	m_pPrintBookDM = NULL;
	m_pGroupCollection = NULL;
	m_bDeletPrintBookDM = FALSE;
	m_bKOLAS2Format = FALSE;
	m_pSortItem = NULL;
	m_nPrintDataType = -1;
	m_strTruncPrice.Empty();
}
// �Ҹ���
CMakeSubTotalPrintDM::~CMakeSubTotalPrintDM()
{
	if(m_bDeletPrintBookDM && m_pPrintBookDM)
	{
		delete m_pPrintBookDM;
		m_pPrintBookDM = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - DM������ ���õǾ���� �˸��ƽ��� �����ϰ�, ��Ż���������� �˸��ƽ��� �����Ѵ�.
//
// [ �̴Ͻ��� ] - ����
//////////////////////////////////////////////////////////////////////
INT CMakeSubTotalPrintDM::InitMakePrintDM()
{
	if( m_pPrintDM == NULL )	 return -1;

	m_pPrintBookDM = new CESL_DataMgr;
	if( m_pPrintBookDM == NULL )	 return -1;
	
	m_pPrintBookDM->SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	m_pPrintBookDM->InitDMPKFromFile(m_strBookDMAlias);
	m_pPrintBookDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	m_bDeletPrintBookDM = TRUE;

	if( m_nPrintType < 0 )	 return -1;
	else if( m_bKOLAS2Format == TRUE )
	{
		m_IgnoreAlias.Add(_T("UDF_�����������"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_����ó��ȣ����"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalSpeciesCntAlias = _T("UDF_���ǻ���");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ���ΰ���");
		//m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");   // 2005-03-03
	}
	else
	{
		switch( m_nPrintType )
		{
		case BO_ACQ_ORDER_DATA_DIVNO:
		case BO_ACQ_ORDER_DATA_PROVIDER:
		case BO_ACQ_ORDER_DATA_SUBJECTCODE:	
		case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:
		case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:
			{
				m_IgnoreAlias.Add(_T("UDF_�����������"));
				m_IgnoreAlias.Add(_T("UDF_�������м���"));
				m_IgnoreAlias.Add(_T("UDF_����ó��ȣ����"));
				m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
				m_strTotalSpeciesCntAlias = _T("IBS_�����");
				m_strTotalBookCntAlias = _T("UDF_å��");
				m_strTotalPriceAlias = _T("UDF_��ȯ����");
				//m_strTotalDiscPriceAlias = _T("UDF_��ȯ���ΰ���");
				m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");  // 2005-02-03
			}
			break;

		case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
		case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
			{
				m_IgnoreAlias.Add(_T("UDF_���������ȣ����"));
				m_IgnoreAlias.Add(_T("UDF_�������м���"));
				m_IgnoreAlias.Add(_T("BP_�����ڸ�"));
				m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
				m_strTotalSpeciesCntAlias = _T("IBS_�����");
				m_strTotalBookCntAlias = _T("UDF_å��");
				m_strTotalPriceAlias = _T("UDF_��ȯ����");
				//m_strTotalDiscPriceAlias = _T("UDF_��ȯ���ΰ���");
				m_strTotalDiscPriceAlias = _T("UDF_��ȯ����");  // 2005-02-03
			}
			break;

		// 2005-01-31
		case BO_ACQ_ORDER_DATA_MEDIACODE:
		case BO_ACQ_PURCHASE_DATA_DEPT:
		case BO_ACQ_PURCHASE_DATA_USERDEFINECODE1:
		case BO_ACQ_ORDER_DATA_ORDER:
			{
				m_IgnoreAlias.Add(_T("UDF_�����������"));
				m_IgnoreAlias.Add(_T("UDF_�������м���"));
				m_IgnoreAlias.Add(_T("UDF_����ó��ȣ����"));
				m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
				m_strTotalSpeciesCntAlias = _T("IBS_�����");
				//m_strTotalBookCntAlias = _T("UDF_å��");
				if( m_nPrintType == BO_ACQ_ORDER_DATA_ORDER )
				{
					m_strTotalBookCntAlias = _T("UDF_��ȯ����");
				}
				else
				{
					m_strTotalBookCntAlias = _T("UDF_å��");
				}
				m_strTotalPriceAlias = _T("UDF_��ȯ����");
				m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");
			}
			break;

		default:
			break;
		}
	}
	m_MarcConvertMgr.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - DM�� �����Ѵ�.
// [ �̴Ͻ��� ]
//////////////////////////////////////////////////////////////////////
INT CMakeSubTotalPrintDM::MakePrintDM()
{
	//================================================================
	// ��������
	//================================================================
	INT ids;
	BOOL bOrderPrint = FALSE;
	CESL_DataMgr* pBookDM = NULL;
	CESL_DataMgr BookDM, SpeciesDM;

	if( m_pGroupCollection == NULL || m_pPrintDM == NULL || m_pPrintBookDM == NULL )		return -1;

	switch( m_nPrintType )
	{
	case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
	case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
		break;
	default:
		{
			if( CheckTotalInfoAlias() == FALSE )
			{
				if( IDNO == m_pParentMgr->ESLAfxMessageBox(_T("���:�հ������� ��Ÿ�� �˸��ƽ��� �������� �ʾҽ��ϴ�.\n����۾��� ��� ���� �Ͻðڽ��ϱ�?"), MB_YESNO) )
					return 1000;
			}
		}
		break;
	}
	
	//================================================================
	// 1. Progress Ctrl �� �����Ѵ�.
	//================================================================
	ShowDataProcessingDlg();


	//================================================================
	// 2. å DM �� �����Ѵ�.
	//================================================================
	BookDM.SetCONNECTION_INFO(m_pPrintBookDM->CONNECTION_INFO);
	BookDM.InitDMPKFromFile(m_pPrintBookDM->DB_MGR_ALIAS);
	BookDM.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);


	//================================================================
	// 3. �� DM �� �����Ѵ�.
	//================================================================
	SpeciesDM.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	SpeciesDM.InitDMPKFromFile(m_pPrintDM->DB_MGR_ALIAS);
	SpeciesDM.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	//å �˻�
	//================================================================
	// 4. ������� DM�˸��ƽ��� �̿��Ͽ� å DM �����͸� �����´�.
	//================================================================
	pBookDM = MakePrintBookDM(&BookDM);
	if( pBookDM == NULL )	return -1;

	//���ڷ� ����
	//================================================================
	// 5. åDM���� ��DM�� �����Ѵ�.
	//================================================================
	SpeciesDM.FreeData();
	ids = MakeSpeciesDM(&SpeciesDM, pBookDM, FALSE);

	// ���ڷ�����
	if( m_nPrintType >= BO_ACQ_ORDER_DATA_DIVNO )	 bOrderPrint = TRUE;
	
	//===================================================
	//2009.05.06 UPDATE BY LKS : �Լ��� ���οø� --> �������� DATA SET!!
	
	// UDF�÷����� �����Ѵ�.
	SetUDFDMFieldData(&SpeciesDM, bOrderPrint);
	//===================================================

	if( m_bDataSortYN == TRUE )		ids = PrintDataSortByUserSelect(&SpeciesDM);
	else							ids = PrintDataSortByBasicInfo(&SpeciesDM);

	if( ids < 0 )
	{
		if(m_pParentMgr->ESLAfxMessageBox(_T("����ڷ� �����۾��� ������ �� �����ϴ�. ��� ���� �Ͻðڽ��ϱ�?"), MB_YESNO) == IDNO)
			return 0;
	}
	
//DEL	// UDF�÷����� �����Ѵ�.
//DEL	SetUDFDMFieldData(&SpeciesDM, bOrderPrint);
	
	// �׸��� �����ڷ� ����
	ids = ApplyGridSelectData(&SpeciesDM);
	if(ids < 0) return ids;
	//�Ϸù�ȣ ����
	ids = SetDataSerialNo(&SpeciesDM);
	if(ids < 0) return ids;
	
	
	//SubTotal PrintDM����
	CGroupInfo* pGroup = NULL;
	INT nRowCnt = SpeciesDM.GetRowCount();
	INT nGroupCnt = m_pGroupCollection->GetGroupCount();
	for(INT i=0 ; i<nGroupCnt ; i++)
	{
		pGroup = m_pGroupCollection->GetGroupInfo(i);
		if( pGroup == NULL )	continue;
		ids = MakeSubTotalRow(&SpeciesDM, pGroup);
		if( ids < 0 )			continue;
		if( m_strSortCondition.IsEmpty() == FALSE )
		{
			ids = MakeSubTotalRowByGroup(&SpeciesDM, pGroup);
			if(ids < 0) return -1;
		}
	}

	// ��¿� ��DM����
	ids = MakeVolumeDM(m_pPrintBookDM, pBookDM, _T("BP_��������KEY"));
	if(ids < 0) return -1;

	CString strTmpData;
	
	// �� DM�� ���� �˸��ƽ��� �����Ѵ�.
	INT nCondColumns = 0;
	CString strAlias1[2];
	INT nCond1[2];
	strAlias1[nCondColumns] = _T("BP_��������KEY");			nCond1[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias1[nCondColumns] = _T("BB_���Ϸù�ȣ");			nCond1[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintBookDM->SORT(nCondColumns, strAlias1, nCond1, ESL_DATAMGR_SORT_ASC);
	
	// ���DM�� Clear��Ų��.
	m_pPrintDM->FreeData();
	
	// ���� �� ���DM�� �ϳ��� Ȯ���Ͽ� �ش��ϴ� �� DM�� �����Ѵ�.
	nRowCnt = SpeciesDM.GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	CString strKey;
	for(i=0 ; i<nRowCnt ; i++)
	{
		if( m_pDataProcessingDlg )		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		strKey = SpeciesDM.GetCellData(_T("BP_��������KEY"), i);
		DWORD * pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		if(pRow == NULL) return -1;
		ids = CopyDM(&SpeciesDM, i, m_pPrintDM, pRow);
		if(ids < 0) return ids;
		
		ids = InsertObjInfo(m_pPrintDM, m_pPrintBookDM, strKey, SPECIES_PRINT);
		if(ids < 0) return ids;
		else if(ids == 1000) m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_�ܰ�")), _T(""));
	}
	ids = m_pPrintDM->RefreshPositionInfo();
	if(ids < 0) return -1;
	
	// Kolas2��¹��� �°� ������ ����
	if(m_bKOLAS2Format)
	{
		ids = SetKOLAS2FormatData(m_pPrintDM);
		if(ids < 0) return -1;
	}
	
	return 0;
}

CString CMakeSubTotalPrintDM::GetGroupCollectionQuery(INT nAcqType)
{
	if(m_pGroupCollection->GetGroupCount() == 0 )
		return _T("");
	
	CString strQuery, strGroupInfo;
	
	CString strWorkCode = m_pGroupCollection->GetWorkCode();
	m_pGroupCollection->GetGroupInfo(strGroupInfo);
	if(strGroupInfo  == _T("��ü�ڷ�")) return _T("");

	if(strWorkCode.IsEmpty( ))
	{
		//2007.05.16 UPDATE PJW : �����ڷ� , �����ڷ� ���� 		
		if( nAcqType == 2  )
		{
			strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND P.DONATOR_KEY = C.REC_KEY(+) AND B.BOOK_APPENDIX_FLAG='B' AND P.ACQ_YEAR='%s' AND P.RECEIPT_NO  IN (%s) "),
			m_pGroupCollection->GetAcqYear(), strGroupInfo);
		}
		else
		{
			strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO IN (%s) "),//AND I.WORK_CODE IS NULL_T(",
			m_pGroupCollection->GetAcqYear(), strGroupInfo);
		}
	}
	else
	{
		//2007.05.16 UPDATE PJW : �����ڷ� , �����ڷ� ���� 
		if( nAcqType == 2 )
		{
			strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND P.DONATOR_KEY = C.REC_KEY(+) AND B.BOOK_APPENDIX_FLAG='B' AND P.ACQ_YEAR='%s' AND P.RECEIPT_NO  IN (%s) AND I.WORK_CODE='%s' "),
			m_pGroupCollection->GetAcqYear(), strGroupInfo);
		}
		else
		{
			strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO IN (%s) AND I.WORK_CODE='%s'"),
			m_pGroupCollection->GetAcqYear(), strGroupInfo, strWorkCode);
		}
	}
	return strQuery;
}

INT CMakeSubTotalPrintDM::MakeSubTotalRow(CESL_DataMgr *pSpeciesDM, CGroupInfo *pGroup, CString strExtraField/*=_T("")*/, CString strExtraValue/*=_T("")*/)
{
	INT nStartIdx, nLastIdx, nDataCnt;
	CString strTmpData, strSubTotalPrice;
	CPrintTotalInfo TotalInfo;
	
	nStartIdx = FindStartIdx(pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strExtraField, strExtraValue);
	nLastIdx = FindLastIdx(nStartIdx, pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strExtraField, strExtraValue);
	if( nStartIdx < 0 || nLastIdx < 0 ) return -1;


	nDataCnt = -1;
	if( pSpeciesDM->GetRowCount() -1 == nLastIdx )		pSpeciesDM->InsertRow(-1);
	else												pSpeciesDM->InsertRow(nLastIdx+1);
	
	if( strExtraField.IsEmpty() == TRUE )	strTmpData = _T("�Ѱ�");
	else									strTmpData = _T("�Ұ�");

	switch( m_nPrintType )
	{
	case BO_ACQ_PURCHASE_DATA_REGCODE:
		{
			TotalInfo.SetAliasInfo(m_cTotalInfo.GetBookCntAlias(), m_cTotalInfo.GetPriceAlias(), m_cTotalInfo.GetDiscPriceAlias());	
		}
		break;

	case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
	case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
		{
			TotalInfo.SetAcqType(2);
		}
	default:
		{
			TotalInfo.SetAliasInfo(m_strTotalBookCntAlias, m_strTotalPriceAlias, m_strTotalDiscPriceAlias);	
		}
		break;
	}
	TotalInfo.SetTotalInfo(pSpeciesDM, nStartIdx, nLastIdx+1);
	SetTotalInfo(pSpeciesDM, &TotalInfo, nLastIdx+1, strTmpData, TotalInfo.GetDiscTotalPrice_Display());
	
	return nLastIdx+2;
}

INT CMakeSubTotalPrintDM::MakeSubTotalRowByGroup(CESL_DataMgr *pSpeciesDM, CGroupInfo *pGroup)
{
	INT nStartIdx = FindStartIdx(pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	INT nLastIdx = FindLastIdx(nStartIdx, pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	if(nStartIdx < 0 || nLastIdx < 0) return -1;
	
	CString strSortValue,strTmp;
	INT i = nStartIdx;
	while( i<=nLastIdx )
	{
		strSortValue = pSpeciesDM->GetCellData(m_strSortCondition, i);
		i = MakeSubTotalRow(pSpeciesDM, pGroup, m_strSortCondition, strSortValue);
		nLastIdx = FindLastIdx(nStartIdx, pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	}
	
	return 0;
}

INT CMakeSubTotalPrintDM::FindStartIdx(CESL_DataMgr *pDM, 
									   CString strAcqYear, 
									   CString strGroupNo, 
									   CString strExtraField/*=_T("")*/, 
									   CString strExtraValue/*=_T("")*/)
{
	INT nRowCnt = pDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(strAcqYear == pDM->GetCellData(_T("BP_���Գ⵵"), i) && strGroupNo == pDM->GetCellData(_T("BP_�׷��ȣ"), i))
		{
			if( strExtraField.IsEmpty() == TRUE )	return i;
			else
			{
				if(strExtraValue == pDM->GetCellData(strExtraField, i)) return i;
			}
		}
	}
	return -1;
}

INT CMakeSubTotalPrintDM::FindLastIdx(INT nStartIdx, 
									  CESL_DataMgr *pDM, 
									  CString strAcqYear, 
									  CString strGroupNo, 
									  CString strExtraField/*=_T("")*/, 
									  CString strExtraValue/*=_T("")*/)
{
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt-1 == nStartIdx) return nStartIdx;
	else if(nRowCnt-1 < nStartIdx) return -1;
	
	CString strTmpData;
	for(INT i=nStartIdx ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("UDF_�Ϸù�ȣ"), i);
		strTmpData.Replace(_T(" "), _T(""));
		if(strTmpData == _T("�Ұ�") || strTmpData == _T("�з�")) continue;
		if(strAcqYear != pDM->GetCellData(_T("BP_���Գ⵵"), i) || strGroupNo != pDM->GetCellData(_T("BP_�׷��ȣ"), i))
		{
			return i-1;
		}
		else
		{
			if(!strExtraField.IsEmpty() && strExtraValue != pDM->GetCellData(strExtraField, i)) return i-1;
		}
	}
	return i-1;
}

INT CMakeSubTotalPrintDM::SetKOLAS2FormatData(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return 0;
	
	CString strDivValue, strTmpData;
	INT ids;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strDivValue = pDM->GetCellData(_T("UDF_�Ϸù�ȣ"), i);
		
		if(strDivValue == _T("�Ұ�") || strDivValue == _T("�Ѱ�"))		continue;
		else if(strDivValue.IsEmpty()) 
		{
			pDM->SetCellData(_T("UDF_���ǻ���"), pDM->GetCellData(_T("IBS_�����"), i), i);
			strTmpData.Format(_T("%s"), pDM->GetCellData(_T("UDF_ǥ������"), i));
			pDM->SetCellData(_T("UDF_�������ڻ���"), strTmpData, i);
		}
		else
		{
			CMarc marc;
			strTmpData = pDM->GetCellData(_T("BS_MARC"), i);
			if(!strTmpData.IsEmpty())
			{
				ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
				if(ids < 0) SetDefaultKOLAS2FormatData(pDM, i);
				ids = SetKOLAS2FormatData(&marc, pDM, i);
				if(ids < 0) SetDefaultKOLAS2FormatData(pDM, i);
			}
			else SetDefaultKOLAS2FormatData(pDM, i);
		}
	}
	return 0;
}

INT CMakeSubTotalPrintDM::SetDefaultKOLAS2FormatData(CESL_DataMgr *pDM, INT nIdx)
{
	CString strTmpData;
	strTmpData.Format(_T("%s / %s"), pDM->GetCellData(_T("IBS_��ǥ��"), nIdx), pDM->GetCellData(_T("IBS_������"), nIdx));
	pDM->SetCellData(_T("UDF_�������ڻ���"), strTmpData, nIdx);
	strTmpData.Format(_T("%s, %s"), pDM->GetCellData(_T("IBS_������"), nIdx), pDM->GetCellData(_T("IBS_�����"), nIdx));
	pDM->SetCellData(_T("UDF_���ǻ���"), strTmpData, nIdx);
	return 0;
}

INT CMakeSubTotalPrintDM::SetKOLAS2FormatData(CMarc * pMarc, CESL_DataMgr *pDM, INT nIdx)
{
	if(pMarc == NULL) return -1;

	CString strTmpData;
	strTmpData.Format(_T("%s / %s"), pDM->GetCellData(_T("IBS_��ǥ��"), nIdx), pDM->GetCellData(_T("IBS_������"), nIdx));
	INT ids = m_MarcConvertMgr.GetTitleStatement(pMarc, strTmpData);
	if(ids < 0) return -1;
	pDM->SetCellData(_T("UDF_�������ڻ���"), strTmpData, nIdx);
	strTmpData.Format(_T("%s, %s"), pDM->GetCellData(_T("IBS_������"), nIdx), pDM->GetCellData(_T("IBS_�����"), nIdx));
	pDM->SetCellData(_T("UDF_���ǻ���"), strTmpData, nIdx);
	return 0;
}

INT CMakeSubTotalPrintDM::PrintDataSortByBasicInfo(CESL_DataMgr * pDM)
{
	if(pDM == NULL) return -1;

	INT nCondColumns = 0;
	CString strAlias[4];
	INT nCond[4];
	strAlias[nCondColumns] = _T("BP_���Գ⵵");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�׷��ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if( m_strSortCondition.IsEmpty() == FALSE )
	{
		strAlias[nCondColumns] = m_strSortCondition;			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	
	if(m_nPrintType >= BO_ACQ_ORDER_DATA_DIVNO)
	{
		strAlias[nCondColumns] = _T("BP_�����Ϸù�ȣ");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	else
	{
		strAlias[nCondColumns] = _T("BP_�Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);	
	return 0;
}

INT CMakeSubTotalPrintDM::PrintDataSortByUserSelect(CESL_DataMgr * pDM)
{
	if(pDM == NULL) return -1;
	INT ids;
	INT nCondColumns = 0;
	CString strAlias[4];
	INT nCond[4];
	strAlias[nCondColumns] = _T("BP_���Գ⵵");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�׷��ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if( m_strSortCondition.IsEmpty() == FALSE )
	{
		strAlias[nCondColumns] = m_strSortCondition;			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	
	if(m_pSortItem && m_bDataSortYN)
	{
		CStringArray * pTmpData = NULL;	
		CMapStringToString * pMap = NULL;
		CStringArray SortConditionHeader;	SortConditionHeader.RemoveAll();
		CMapStringToString SortConditionMap;	SortConditionMap.RemoveAll();

		pTmpData = m_pSortItem->GetHeadSortList();
		ids = CopySortItem(pTmpData, &SortConditionHeader);
		if(ids < 0) return -1;
		pMap = m_pSortItem->GetConvertUDFToRealAliasMap();
		ids = CopySortConditionMap(pMap, &SortConditionMap);
		if(ids < 0) return -1;
		
		if( m_strSortCondition.IsEmpty() == FALSE )		pTmpData->Add(m_strSortCondition);
		pMap->SetAt(_T("UDF_������"), _T("UDF_����"));
		pMap->SetAt(_T("UDF_����������"), _T("UDF_���ΰ�"));
		ids = m_pSortItem->DataSort(pDM);
		CopySortItem(&SortConditionHeader, pTmpData);
		CopySortConditionMap(&SortConditionMap, pMap);
		if(ids < 0) return -1;
	}
	else
	{
		return PrintDataSortByBasicInfo(pDM);
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
//
// [ �̴Ͻ��� ]
//////////////////////////////////////////////////////////////////////
CESL_DataMgr* CMakeSubTotalPrintDM::MakePrintBookDM(CESL_DataMgr * pBookDM)
{
	//================================================================
	// ��������
	//================================================================
	INT ids = -1;

	if(pBookDM == NULL) return NULL;
	
	//================================================================
	// ��µ����� Ÿ�Կ� ���� åDM�� �����Ѵ�.
	// - 0 : ��µ����� ��˻�
	// - 1 : ����ȭ��˻������� ���
	// - 2 : �׸��� �����ڷ�
	//================================================================
	switch( m_nPrintDataType )
	{
	case 0:
	{
		ids = ReSearchPrintBookDM(pBookDM);
		if( ids < 0 )	 return NULL;
		return pBookDM;
	}
		break;
	case 1:
	case 2:		return m_pParentMgr->FindDM(m_strBookDMAlias);
		break;
	}

	return NULL;
}

INT CMakeSubTotalPrintDM::ReSearchPrintBookDM(CESL_DataMgr * pBookDM)
{
	INT ids = -1;
	CString strQuery;


	if ( pBookDM->DB_MGR_ALIAS.Find(_T("����")) >= 0 )
	{
		strQuery = GetGroupCollectionQuery(2);
	}
	else
	{
		strQuery = GetGroupCollectionQuery(1);
	}
	
	if(strQuery.IsEmpty())
	{
		m_pParentMgr->ESLAfxMessageBox(_T("�־��� �׷������� ������ ������ �� �����ϴ�. �ٽ� �˻��� �ֽʽÿ�."));
		return 1000;
	}
	
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��¿� å������ �������Դϴ�."));
		ids = pBookDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	}
	else 
		ids = pBookDM->RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	return 0;
}