// MakeSubTotalBookPrintDM.cpp: implementation of the CMakeSubTotalBookPrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeSubTotalBookPrintDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeSubTotalBookPrintDM::CMakeSubTotalBookPrintDM(CESL_Mgr * pParentMgr) : CMakeSubTotalPrintDM(pParentMgr)
{
	m_pPrintBookDM = NULL;
	m_pGroupCollection = NULL;
	m_bDeletPrintBookDM = FALSE;
}

CMakeSubTotalBookPrintDM::~CMakeSubTotalBookPrintDM()
{
	
}

BOOL CMakeSubTotalBookPrintDM::CheckTotalInfoAlias()
{
	if(m_strTotalInfoTitle.IsEmpty()		||
		m_strTotalBookCntAlias.IsEmpty()	||
		m_strTotalPriceAlias.IsEmpty()		||
		m_strTotalDiscPriceAlias.IsEmpty())
		return FALSE;

	return TRUE;
}

INT CMakeSubTotalBookPrintDM::InitMakePrintDM()
{
	if(m_pPrintDM == NULL || m_pPrintBookDM == NULL) return -1;
	if(m_nPrintType < 0) return -1;
	
	//  �հ������� ��Ÿ�� �˸��ƽ� ����
	//
	switch( m_nPrintType )
	{
	// �ڷᱸ�к� ��� DM �߰�
	// 2005-09-05 ������
	//
	case BO_ACQ_PURCHASE_DATA_USERDEFINECODE1:
	case BO_ACQ_PURCHASE_DATA_DEPT:
	// �̻� ��.
	case BO_ACQ_PURCHASE_DATA_REGCODE:
	{
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("IBS_�����");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ���ΰ���");
	}
	break;

	default :	return -1;
		break;
	}	
	return 0;	
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
//  - å�Ұ���¹� DM�� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ���α׷��� ��Ʈ���� �����Ѵ�.
// 2. ���Ŀ��θ� �Ǵ��Ͽ� �ҽ� DM�� �����Ѵ�.
// 3. ���� åDM������ �����Ѵ�
// 4. UDF�÷����� �����Ѵ�.
// 5. UDF_�Ϸù�ȣ�� �����Ѵ�.
// 6. ��¿� SubTotal PrintDM�� �����Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CMakeSubTotalBookPrintDM::MakePrintDM()
{
	//================================================================
	// ���� ����
	//================================================================
	INT ids, i, nGroupCnt;
	CGroupInfo* pGroup = NULL;

	if( m_pGroupCollection == NULL || m_pPrintDM == NULL || m_pPrintBookDM == NULL )	return -1;

	if( CheckTotalInfoAlias() == FALSE )
	{
		if(IDNO == m_pParentMgr->ESLAfxMessageBox(_T("���:�հ������� ��Ÿ�� �˸��ƽ��� �������� �ʾҽ��ϴ�.\n����۾��� ��� ���� �Ͻðڽ��ϱ�?"), MB_YESNO))
			return 1000;
	}

	//================================================================
	// 1. ���α׷��� ��Ʈ���� �����Ѵ�.
	//================================================================
	ShowDataProcessingDlg();
	

	// 2006-01-19 ������ - �ҽ�DM�� �������� �ʰ� ���DM�� �����Ѵ�.
	/*
	//================================================================
	// 2. ���Ŀ��θ� �Ǵ��Ͽ� �ҽ� DM�� �����Ѵ�.
	//================================================================
	if( m_bDataSortYN == TRUE )		ids = PrintDataSortByUserSelect(m_pPrintBookDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintBookDM);
	if( ids < 0 )
	{
		if( IDNO == m_pParentMgr->ESLAfxMessageBox(_T("����ڷ� �����۾��� ������ �� �����ϴ�. ��� ���� �Ͻðڽ��ϱ�?"), MB_YESNO) )
			return 0;
	}
	*/


	//================================================================
	// 3. ���� åDM������ �����Ѵ�
	//================================================================
	if( m_pDataProcessingDlg )
	{
		m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, m_pPrintDM->GetRowCount());
	}
	
	m_pPrintDM->FreeData();

	for( i=0 ; i<m_pPrintBookDM->GetRowCount() ; i++ )
	{
		if( m_pDataProcessingDlg )	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		if( CheckPrintData(m_pPrintBookDM->GetCellData(m_strDivAlias, i)) == FALSE )	continue;
		
		DWORD* pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		if( pRow == NULL )		return -1;

		ids = CopyDM(m_pPrintBookDM, i, m_pPrintDM, pRow);
		if( ids < 0 )	return -1;
	}

	ids = m_pPrintDM->RefreshPositionInfo();
	if( ids < 0 )	return -1;

	//================================================================
	// 4. UDF�÷����� �����Ѵ�.
	//================================================================
	SetUDFDMFieldData(m_pPrintDM);
	
	//================================================================
	// 4-1. ���Ŀ��θ� �Ǵ��Ͽ� ���DM�� �����Ѵ�.
	//================================================================
	if( m_bDataSortYN == TRUE )		ids = PrintDataSortByUserSelect(m_pPrintDM);
	else							ids = PrintDataSortByBasicInfo(m_pPrintDM);
	if( ids < 0 )
	{
		if( IDNO == m_pParentMgr->ESLAfxMessageBox(_T("����ڷ� �����۾��� ������ �� �����ϴ�. ��� ���� �Ͻðڽ��ϱ�?"), MB_YESNO) )
			return 0;
	}
	
	//================================================================
	// 5. UDF_�Ϸù�ȣ�� �����Ѵ�.
	//================================================================
	ids = SetDataSerialNo(m_pPrintDM);
	if(ids < 0) return ids;


	//================================================================
	// 6. ��¿� SubTotal PrintDM�� �����Ѵ�.
	//================================================================	
	//nRowCnt = m_pPrintDM->GetRowCount();
	nGroupCnt = m_pGroupCollection->GetGroupCount();
	for( i=0 ; i<nGroupCnt ; i++ )
	{
		pGroup = m_pGroupCollection->GetGroupInfo(i);

		ids = MakeSubTotalRow(m_pPrintDM, pGroup);
		if( ids < 0 )	return -1;
		if( m_strSortCondition.IsEmpty() == FALSE )
		{
			ids = MakeSubTotalRowByGroup(m_pPrintDM, pGroup);
			if( ids < 0 )	return -1;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - UDF�� ���ǵ� Į���� �����Ѵ�. 
//
//////////////////////////////////////////////////////////////////////
INT CMakeSubTotalBookPrintDM::SetUDFDMFieldData(CESL_DataMgr * pDM)
{
	//================================================================
	// ��������
	//================================================================
	INT i, j, ids, nColCnt, nRowCnt, nRow = -1;
	DOUBLE dPrice;	
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strWorkCode, strOrderSerialNo, strTmpData, str;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	m_MarcConvertMgr.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	TCHAR* pEndPtr = NULL;

	// 2005-01-17
	// �����ڸ��� ������ �޾ƿ���.
	if( m_strPostNum.IsEmpty() == TRUE )
	{
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '����'AND GROUP_3 = '����' AND CLASS_ALIAS = '���������������'"), m_strPostNum);
//DEL 		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '����'AND GROUP_3 = '����' AND CLASS_ALIAS = '���������������'"), m_strPostNum);
		if( m_strPostNum.IsEmpty() == TRUE )		m_strPostNum = _T("0");
	}
	if( m_strTruncPrice.IsEmpty() == TRUE )
	{	
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '����'AND GROUP_3 = '����' AND CLASS_ALIAS = '���԰��ݴ�������'"), m_strTruncPrice);
//DEL 		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '����'AND GROUP_3 = '����' AND CLASS_ALIAS = '���԰��ݴ�������'"), m_strTruncPrice);
		if( m_strTruncPrice.IsEmpty() == TRUE )		m_strTruncPrice = _T("0");
	}


	nColCnt = pDM->GetRefColumnCount();
	nRowCnt = pDM->GetRowCount();
	for( i=0 ; i<nRowCnt ; i++ )
	{
		if( nRow != i )
		{
			CMarc marc;
			strTmpData = pDM->GetCellData(_T("BS_MARC"), i);
			ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
			if( ids < 0 )	return -1;

			strTmpData.Empty();	
			ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
			if( ids >= 0 )	pDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, i);

			strTmpData.Empty();	
			ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
			if( ids >= 0 )	pDM->SetCellData(_T("IBS_�ǻ���"), strTmpData, i);

			strTmpData.Empty();	 
			strTmpData = pDM->GetCellData(_T("IBS_��ǥ��"), i);
			ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
			if( ids >= 0 )	pDM->SetCellData(_T("UDF_ǥ������"), strTmpData, i);

			nRow = i;
		}
		
		for( j=0 ; j<nColCnt ; j++ )
		{
			pDM->GetColumnRefInfo(j, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if( FIELD_ALIAS == _T("UDF_��������") )
			{	
				INT nGroupNo, nWorkNo, nSerialNo;
				strAcqYear = pDM->GetCellData(_T("BP_���Գ⵵"), i);
				strWorkCode = pDM->GetCellData(_T("IBS_��������"), i);
				strGroupNo = pDM->GetCellData(_T("BP_�׷��ȣ"), i);
				nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
				strWorkNo = pDM->GetCellData(_T("BP_�۾���ȣ"), i);
				nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
				strSerialNo = pDM->GetCellData(_T("BP_�Ϸù�ȣ"), i);
				nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
				str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
				pDM->SetCellData(FIELD_ALIAS, str, i);
				str.Format(_T("%04d-%03d-%05d"), nGroupNo, nWorkNo, nSerialNo);
				pDM->SetCellData(_T("UDF_�����������"), str, i);
			}
			else if( FIELD_ALIAS == _T("BB_����") )
			{
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				// 2004.09.30
				strTmpData.Replace(_T(","), _T(""));
				//dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

				//2005-01-17
				if		( m_strPostNum == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
				else if	( m_strPostNum == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
				else							strTmpData.Format(_T("%.0f"), dPrice);

				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_��ȯ����"), strTmpData, i);
			}
			// å ���ְ���
			else if( FIELD_ALIAS == _T("BB_���ְ���_å") )
			{
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				strTmpData.Replace(_T(","), _T(""));
				//dPrice = (DOUBLE)atof(strTmpData);
				dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

				if( dPrice == 0 )	strTmpData = _T("0");
				else
				{
					if		( m_strTruncPrice == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
					else if	( m_strTruncPrice == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
					else									strTmpData.Format(_T("%.0f"), dPrice);
				}
				pDM->SetCellData(_T("UDF_���ΰ�"), strTmpData, i);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_��ȯ���ΰ���"), strTmpData, i);
			}
			else if( FIELD_ALIAS == _T("UDF_�ܰ�") )
			{
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				strTmpData.Replace(_T(","), _T(""));
				//dPrice = (DOUBLE)atof(strTmpData);
				dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

				if( dPrice == 0 )	strTmpData = _T("0");
				else
				{
					if( _T("\\") == pDM->GetCellData(_T("BP_ȭ�󱸺�"), i) )
					{
						strTmpData.Format(_T("%d"), (INT)dPrice);
					}
					else
					{
						if		( m_strPostNum == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
						else if	( m_strPostNum == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
						else								strTmpData.Format(_T("%d"), (INT)dPrice);
					}
				}
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
			/*
			else if( FIELD_ALIAS == _T("UDF_���ΰ�") )
			{
				strTmpData = pDM->GetCellData(_T("BB_����"), i);				
				dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("BP_������"), i);
				dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dExchangeRate = 0.0; 
				strExchangeRate = pDM->GetCellData(_T("BP_ȯ��"), i);
				dExchangeRate = (DOUBLE)atof(strExchangeRate.GetBuffer(0));
				dDisc_Price = (dPrice - ((dPrice*dDisc_Rate)/100)) * dExchangeRate;
				//2005-01-17
				if		( m_strTruncPrice == _T("1") )			strTmpData.Format(_T("%.1f"), dDisc_Price);
				else if	( m_strTruncPrice == _T("2") )			strTmpData.Format(_T("%.2f"), dDisc_Price);
				else										strTmpData.Format(_T("%.0f"), dDisc_Price);

				pDM->SetCellData(_T("UDF_���ΰ�"), strTmpData, i);
				
				m_pParentMgr->GetManageValue(_T("��Ÿ"), _T("����"), _T("����"), _T(""), strIs64);
				if( _T("Y") == strIs64 )
				{
					INT nOrderPrice = 0;
					dDisc_Price /= 10;
					nOrderPrice = (INT)(dDisc_Price+0.5);
					nOrderPrice *= 10;
					strTmpData.Format(_T("%d"), nOrderPrice);
				}
				else
				{
					strTmpData.Format(_T("%.0f"), dDisc_Price);  // 2005-02-02
				}			

				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_��ȯ���ΰ���"), strTmpData, i);
			}
			*/			
			else if( FIELD_ALIAS == _T("BB_��ϱ���") )
			{
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��ϱ���"), strTmpData, strTmpData);
				
				//===================================================
				//2009.05.20 UPDATE BY LKS : ���Դ���ڷ���(��ϱ��к�) ����� ��� ��ȯ ���� ���
				
				//DEL pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
				if(m_nPrintType != BO_ACQ_PURCHASE_DATA_REGCODE)
					pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
				//===================================================
			}
			// �����з��ڵ�1(����������ڵ�1) ���� �������� �����Ͽ� DM�� ���� (64)
			// 2005-09-05 ������
			//
			else if( FIELD_ALIAS == _T("BS_����������ڵ�1") )
			{
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�����з��ڵ�1"), strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
			// �̻� ��.

			// ��ó�� �ڵ尪�� �������� �����Ͽ� DM�� ���� (64)
			// 2005-09-26 ������
			//
			else if( FIELD_ALIAS == _T("UDF_��ó��") )
			{
				strTmpData = pDM->GetCellData(_T("BP_��ó��"), i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�к�"), strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
			// �̻� ��.
			// �μ��� �ڵ尪�� �������� �����Ͽ� DM�� ���� (�����)
			// 2005-11-10 ������
			//
			else if( FIELD_ALIAS == _T("UDF_�μ���") )
			{
				strTmpData = pDM->GetCellData(_T("BP_�μ��ڵ�"), i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�����μ�"), strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
			// �̻� ��.
		}
	}	
	return 0;
}

INT CMakeSubTotalBookPrintDM::PrintDataSortByBasicInfo(CESL_DataMgr *pDM)
{
	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_���Գ⵵");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�׷��ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if(!m_strSortCondition.IsEmpty())
	{
		strAlias[nCondColumns] = m_strSortCondition;		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	strAlias[nCondColumns] = _T("BP_�Ϸù�ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);	
	return 0;
}

INT CMakeSubTotalBookPrintDM::PrintDataSortByUserSelect(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	INT ids;
	INT nCondColumns = 0;
	CString strAlias[4];
	INT nCond[4];
	strAlias[nCondColumns] = _T("BP_���Գ⵵");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�׷��ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if(!m_strSortCondition.IsEmpty())
	{
		strAlias[nCondColumns] = m_strSortCondition;			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	
	if(m_pSortItem && m_bDataSortYN)
	{
		CStringArray * pTmpHeader = NULL;	
		CStringArray * pTmpTail = NULL;	
		CMapStringToString * pMap = NULL;
		CStringArray SortConditionHeader;	SortConditionHeader.RemoveAll();
		CStringArray SortConditionTail;		SortConditionTail.RemoveAll();
		CMapStringToString SortConditionMap;	SortConditionMap.RemoveAll();

		pTmpHeader = m_pSortItem->GetHeadSortList();
		ids = CopySortItem(pTmpHeader, &SortConditionHeader);
		if(ids < 0) return -1;
		pTmpTail = m_pSortItem->GetTailSortList();
		ids = CopySortItem(pTmpTail, &SortConditionTail);
		if(ids < 0) return -1;
		pMap = m_pSortItem->GetConvertUDFToRealAliasMap();
		ids = CopySortConditionMap(pMap, &SortConditionMap);
		if(ids < 0) return -1;
		
		if(!m_strSortCondition.IsEmpty()) pTmpHeader->Add(m_strSortCondition);
		pMap->SetAt(_T("UDF_������"), _T("UDF_��ȯ����"));
		pMap->SetAt(_T("UDF_����������"), _T("UDF_��ȯ���ΰ���"));
		pTmpTail->Add(_T("BB_���Ϸù�ȣ"));
		ids = m_pSortItem->DataSort(pDM);
		CopySortItem(&SortConditionHeader, pTmpHeader);
		CopySortItem(&SortConditionTail, pTmpTail);
		CopySortConditionMap(&SortConditionMap, pMap);

		if(ids < 0) return -1;
	}
	else
	{
		return PrintDataSortByBasicInfo(pDM);
	}
	
	return 0;
}
