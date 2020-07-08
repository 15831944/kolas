// BO_DataConverter.cpp: implementation of the CBO_DataConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_DataConverter.h"
#include "../K2UP_Common/WorkingStatus.h"
#include "../COMMON_CONVERSION_MGR/PriceInfoConversionMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������
CBO_DataConverter::CBO_DataConverter()
{
	m_pSourceDM = NULL;
	m_pTargetDM = NULL;
	m_pPrgCtrl = NULL;
}

// �Ҹ���
CBO_DataConverter::~CBO_DataConverter()
{

}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ������DM�� �����Ѵ�.
// [ �̴Ͻ��� ]
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::ConvertToSpeciesInfo()
{
	if( m_pSourceDM == NULL || m_pTargetDM == NULL || m_strDivideAlias.IsEmpty() == TRUE ) 	return -1;	
	if( m_pSourceDM->GetRowCount() == 0 )	return 0;
	
	m_pSourceDM->SORT(m_strDivideAlias);
	
	return ConvertSourceInfo(0, m_pSourceDM->GetRowCount()-1, m_strDivideAlias);
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ������DM�� �����Ѵ�.
// [ �̴Ͻ��� ]
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::ConvertToVolumeInfo(BOOL bPrintData)
{
	if( m_pSourceDM == NULL || m_pTargetDM == NULL || m_strDivideAlias.IsEmpty() == TRUE )	return -1;
	if( m_pSourceDM->GetRowCount() == 0 )	return 0;

	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];

	strAlias[nCondColumns] = m_strDivideAlias;			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;

	if( bPrintData == FALSE )
	{
		strAlias[nCondColumns] = _T("BB_��ǥå����");		nCond[nCondColumns] = ESL_DATAMGR_SORT_DESC;	nCondColumns++;
	}
	m_pSourceDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	INT cnt, nStart, nSCnt, nNext, ids;
	CString strSpeciesKey, strTmpSpeciesKey, strVolSortNo, strTmpVolSortNo, strTmpData;
	cnt = nStart = nSCnt = nNext = 0;
	if( m_pPrgCtrl )
	{
		m_pPrgCtrl->SetRange(0, m_pSourceDM->GetRowCount());
		m_pPrgCtrl->SetPos(0);
	}
	
	while( m_pSourceDM->GetRowCount() > nStart )
	{
		m_pSourceDM->GetCellData(m_strDivideAlias, nStart, strTmpSpeciesKey);
		nSCnt = FindSameDataCnt(&nNext, m_pSourceDM->GetRowCount(), strTmpSpeciesKey);
		if( bPrintData == TRUE )	ids = ConvertToPrintVolume(nStart, nStart+nSCnt);
		else						ids = ConvertToVolume(nStart, nStart+nSCnt);
		if( ids < 0 )	 return -1;

		nStart = nNext;
		if( m_pPrgCtrl )	m_pPrgCtrl->SetPos(nStart);
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - åDM���� ��DM�� �����Ѵ�.
// [ �̴Ͻ��� ]
// 1. Progress Ctrl�� �ʱ�ȭ�Ѵ�.
// 2. �ش�˸��ƽ��� Į�� �ε����� ã�´�.
//		- UDF_å��, UDF_������, UDF_����, UDF_�ڷ���¼���, UDF_�Ϸù�ȣ
// 3. ����������� �������� �����´�.
// 4. ���ڷ� �� ���� ��ŭ ��������Ű�� ������ DM�� �����ϰ� 2������ ������ ������ DM�� �߰� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::ConvertSourceInfo(INT nStart, INT nEnd, CString strAlias)
{
	//================================================================
	// ��������
	//================================================================
	INT ids, cnt, nDBookCnt, nTmp1, i;
	INT nCol_UDF_BOOKCOUNT, nCol_UDF_SPECIESPRICE, nCol_UDF_PRICE, nCol_UDF_WORKING_STATUS, nCol_UDF_SERIALNO, nCol_UDF_REPRESENTBOOKPRICE ;
	LONG nMinSortNo;
	DOUBLE nTotalPrice;
	CString strKey, strTmpKey, strTmpData, strDeliveryYN, strBookPrice;
	CWorkingStatus cWorkingStatus(_T(""));
	
	nMinSortNo		= LONG_MAX;
	nTotalPrice		= 0.0;
	cnt = nDBookCnt = 0;

	TCHAR* pEndPtr = NULL;
	
	//================================================================
	// 1. Progress Ctrl�� �ʱ�ȭ�Ѵ�.
	//================================================================
	if( m_pPrgCtrl )
	{
		m_pPrgCtrl->SetRange(0, nEnd-nStart)	;
		m_pPrgCtrl->SetPos(0);
		m_pPrgCtrl->SetWindowText(_T(""));
	}

	//================================================================
	// 2. �ش�˸��ƽ��� Į�� �ε����� ã�´�.
	//		- UDF_å��, UDF_������, UDF_����, UDF_�ڷ���¼���, UDF_�Ϸù�ȣ
	// 2006.03.15 KBS - UDF_�ܰ��� ��ǥå�� �������� �����Ѵ�.
	//================================================================
	nCol_UDF_BOOKCOUNT		= m_pTargetDM->FindColumn(_T("UDF_å��")		);
	nCol_UDF_SPECIESPRICE	= m_pTargetDM->FindColumn(_T("UDF_������")		);
	nCol_UDF_PRICE			= m_pTargetDM->FindColumn(_T("UDF_����")		);
	nCol_UDF_WORKING_STATUS = m_pTargetDM->FindColumn(_T("UDF_�ڷ���¼���"));
	nCol_UDF_SERIALNO		= m_pTargetDM->FindColumn(_T("UDF_�Ϸù�ȣ")	);
	nCol_UDF_REPRESENTBOOKPRICE = m_pTargetDM->FindColumn(_T("UDF_�ܰ�")	);
	
	//================================================================
	// 2005-02-03
	// 3. ����������� �������� �����´�.
	//================================================================
	if( m_strPosNumber.IsEmpty() == TRUE )
	{
		m_pSourceDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL "
										_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '����' ")
//DEL 										 _T("WHERE GROUP_2 = '����' ")
										 _T("AND GROUP_3 = '����' ") 
										 _T("AND CLASS_ALIAS = '���������������'")), m_strPosNumber);
		
		if( m_strPosNumber.IsEmpty() == TRUE )			m_strPosNumber = _T("0");
	}


	//================================================================
	// 4. ���ڷ� �� ���� ��ŭ ��������Ű�� ������ DM�� �����ϰ� 2������ ������ ������ DM�� �߰� �����Ѵ�.
	//================================================================
	strBookPrice.Empty();
	for( i=nStart; i<=nEnd; i++ )
	{
		if( m_pPrgCtrl )	m_pPrgCtrl->StepIt();
		
		// ��������Ű�� �����´�.
		m_pSourceDM->GetCellData(strAlias, i, strTmpKey);
		// ���ο� �� �Է�
		if( strKey.IsEmpty() == FALSE && strKey != strTmpKey )
		{
			// �ҽ�DM -> Ÿ��DM���� �����Ѵ�.
			DWORD* pRow = (DWORD*)AddToTargetDM(i-1);
			if( pRow == NULL )		break;

			// ������ ������ UDF�׸���� �����Ѵ�.
			strTmpData.Format(_T("%d"), cnt);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_BOOKCOUNT, strTmpData);
			// 2004.12.03
			strTmpData = TruncPrice(m_pTargetDM, nTotalPrice);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_SPECIESPRICE, strTmpData);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_PRICE, strTmpData);
			
			strTmpData.Format(_T("%ld"), nMinSortNo);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_SERIALNO, strTmpData);

			m_pTargetDM->SetCellData(pRow, nCol_UDF_WORKING_STATUS, cWorkingStatus.GetWorkingStatus());
			// 2006.03.14 KBS - UDF_�ܰ��� ���� ��ǥå�� �������� �Ѵ�.
			m_pTargetDM->SetCellData(pRow, nCol_UDF_REPRESENTBOOKPRICE, strBookPrice);

			strKey = strTmpKey;
			cnt = 1;
			strTmpData = m_pSourceDM->GetCellData(_T("BB_����"), i);
			//nTotalPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
			nTotalPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			cWorkingStatus.Init(m_pSourceDM->GetCellData(_T("BB_�ڷ����"), i));
			strTmpData = m_pSourceDM->GetCellData(_T("BB_�Ϸù�ȣ"), i);
			nMinSortNo = _ttoi(strTmpData.GetBuffer(0));
		}
		else 
		{
			cnt++;
			strTmpData = m_pSourceDM->GetCellData(_T("BB_����"), i);
			//nTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			nTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
			
			strTmpData = m_pSourceDM->GetCellData(_T("BB_�Ϸù�ȣ"), i);
			nTmp1 = _ttoi(strTmpData.GetBuffer(0));
			if( nMinSortNo > nTmp1 )	nMinSortNo = nTmp1;

			strTmpData = m_pSourceDM->GetCellData(_T("BB_�ڷ����"), i);

			strKey = strTmpKey;
			if( strTmpData.IsEmpty() == TRUE )	continue;
			cWorkingStatus.SetWorkingStatusIfMax(strTmpData);
		}
		// 2006.03.14 KBS - UDF_�ܰ��� ���� ��ǥå�� �������� �Ѵ�.
		if ( _T("Y") == m_pSourceDM->GetCellData(_T("BB_��ǥå����"), i) )
		{
			m_pSourceDM->GetCellData(_T("UDF_�ܰ�"), i, strBookPrice);
		}
	}

	if( i > nEnd )
	{
		DWORD* pRow = (DWORD*)AddToTargetDM(i-1);
		if( pRow != NULL )
		{
			strTmpData.Format(_T("%d"), cnt);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_BOOKCOUNT, strTmpData);
			// 2004.12.03
			strTmpData = TruncPrice(m_pTargetDM, nTotalPrice);

			m_pTargetDM->SetCellData(pRow, nCol_UDF_SPECIESPRICE, strTmpData);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_PRICE, strTmpData);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_WORKING_STATUS, cWorkingStatus.GetWorkingStatus());
			strTmpData.Format(_T("%ld"), nMinSortNo);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_SERIALNO, strTmpData);
			// 2006.03.14 KBS - UDF_�ܰ��� ���� ��ǥå�� �������� �Ѵ�.
			m_pTargetDM->SetCellData(pRow, nCol_UDF_REPRESENTBOOKPRICE, strBookPrice);
		}
	}

	ids = m_pTargetDM->RefreshPositionInfo();
	if( ids < 0 )	return -1;
	
	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���ڷο� idx��°�� �ο��� Į���� DM�����Ѵ�.
// [ �̴Ͻ��� ] - ����
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::AddToTargetDM(INT idx)
{
	if( m_pTargetDM == NULL || m_pSourceDM == NULL || idx < 0 )		return -1000;		
	
	DWORD* pRow = (DWORD*)m_pTargetDM->InsertRow(-2);
	if( pRow == NULL )	return -1000;

	// ����� �ʵ� ������ �����Ѵ�.
	INT col, nCol, nCurRow, nColCnt;	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	nCurRow = m_pTargetDM->GetRowCount() - 1;
	nColCnt = m_pSourceDM->m_nCols;
	if( nColCnt < 0 )	nColCnt = m_pSourceDM->GetRefColumnCount();
	
	for( col = 0; col < nColCnt; col++ ) 
	{
		if( m_pSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE) != 0 ) 
		{
			continue;
		}
		// 2006.03.14 KBS - UDF_�ܰ��� ���� ��ǥå�� �������� �Ѵ�.
		if( FIELD_ALIAS == _T("UDF_�ܰ�") )		continue;

		nCol = m_pTargetDM->FindColumn(FIELD_ALIAS);
		if( nCol >= 0 )
		{
			m_pTargetDM->SetCellData(pRow, nCol, m_pSourceDM->GetCellData(idx, col));
		}
	}
	
	return (INT)pRow;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �˸��ƽ��� ���� ���ڰ��� ���� DM�� ������ ���� �����Ѵ�.
// [ �̴Ͻ��� ] - ����
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::FindSameDataCnt(INT *nStart, INT nRowCnt, CString strSpeciesKey)
{
	INT i, cnt = 0;
	CString strTmpData;

	i = *nStart;
	i++;
	while( i < nRowCnt )
	{
		m_pSourceDM->GetCellData(m_strDivideAlias, i, strTmpData);
		if( strTmpData == strSpeciesKey )
		{
			cnt++;
			i++;
		}
		else break;
	}
	*nStart = i;

	return cnt;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��¿� ��DM�� �����Ѵ�.
// [ �̴Ͻ��� ]
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::ConvertToPrintVolume(INT nBeginIdx, INT nRowCnt)
{
	if( m_pSourceDM == NULL || m_pTargetDM == NULL )	return -1;
	
	INT i, j, ids, nDBookCnt, nBookCnt;
	DOUBLE dOrderPrice, dPrice, dDiscRate, dExchangeRate;
	CString curVOL_SORT, tempVOL_SORT, strDBFLAG, strData, strVol, strTmpData, strTmpData1, strTmpData2, strPrice;	
	CPriceInfoConversionMgr PriceConvertMgr;
	CArray <INT, INT> CopyRowIdxList;
	CopyRowIdxList.RemoveAll();
	
	// 2006-01-16 ������ - ������å���׹ݿ�, å���ְ����� ���� �ǹ��ְ��ݿ� �����Ѵ�.
	// ���ְ��� �Ҽ������뿩�θ� �˻��Ѵ�.	
	strVol = _T("");
	for( i=nBeginIdx; i<=nRowCnt; i++ )
	{
		strTmpData.Empty();
		m_pSourceDM->GetCellData(_T("BB_���Ϸù�ȣ"), i, curVOL_SORT);
		// ���Ϸù�ȣ�� ���ٸ�
		if( strVol != curVOL_SORT )
		{
			CopyRowIdxList.Add(i);
			strVol = curVOL_SORT;
			}
		}

	// 2005-02-03
	if( m_strPosNumber.IsEmpty() == TRUE )
	{
		m_pSourceDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '����'AND GROUP_3 = '����' AND CLASS_ALIAS = '���������������'"), m_strPosNumber);
//DEL 		m_pSourceDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '����'AND GROUP_3 = '����' AND CLASS_ALIAS = '���������������'"), m_strPosNumber);
		if( m_strPosNumber.IsEmpty() == TRUE )		m_strPosNumber = _T("0");
	}

	for( i=0; i<CopyRowIdxList.GetSize(); i++ ) 
	{
		// BOOK�� �� Row�� VOLUME���� �����Ѵ�.
		DWORD *pRow = (DWORD*)AddToTargetDM(CopyRowIdxList.GetAt(i));
		if(pRow == NULL) return -1;

		// 2006-01-18 ������ - ������å���׹ݿ�, �ǹ��ְ����� �����Ѵ�.
		dOrderPrice = 0.0;
		m_pSourceDM->GetCellData(_T("BB_���Ϸù�ȣ"), i, curVOL_SORT);
		for( j=nBeginIdx; j<nRowCnt; j++ )
		{
			m_pSourceDM->GetCellData(_T("BB_���Ϸù�ȣ"), j, tempVOL_SORT);
			if( curVOL_SORT == tempVOL_SORT )
			{
				m_pSourceDM->GetCellData(_T("BB_���ְ���_å"), j, strTmpData);
				//dOrderPrice += (DOUBLE)atof(strTmpData);
				TCHAR* pEndPtr = NULL;
				dOrderPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
			}
		}
		strPrice.Empty();
		strPrice.Format(_T("%lf"), dOrderPrice);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_���ְ���_��")), strPrice);
		//-----------------------------------------------------------------

		// ����� VOLUME�� ���� å���� ���Ͽ� VOLUME�� ����å��UDF �� �־��ش�.
		curVOL_SORT = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_���Ϸù�ȣ")));
		nBookCnt = GetEqualBookCount(m_pSourceDM, curVOL_SORT, nBeginIdx, nRowCnt);
		nDBookCnt = GetEqualBookCount(m_pSourceDM, curVOL_SORT, nBeginIdx, nRowCnt, _T("Y"));
		strData.Format(_T("%d"), nBookCnt);
		ids = m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_å��")), strData);
		strData.Format(_T("%d"), nDBookCnt);
		ids = m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_��ǰå��")), strData);
		strData.Format(_T("%d"), nBookCnt - nDBookCnt);	
		ids = m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_�̳�å��")), strData);
		//IBS��ǥ���� ������� �Ǽ��� ǥ��
		strTmpData1 = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_�����")));
		strTmpData2 = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_�Ǽ���")));
		strData.Format(_T(". %s , %s"), strTmpData1, strTmpData2);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_ǥ������")), strData);
		// 2005-02-04 IBS_���������ڻ��װ� IBS_������� �߰�.
		// ���������ڻ���
		strTmpData1 = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_ǥ������"))); 
		strTmpData2 = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("IBS_������")));
		strData.Empty();
		if(strTmpData2.IsEmpty()) strData = strTmpData1;
		else  strData = strTmpData1 + _T(" / ") + strTmpData2;
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_���������ڻ���")), strData);
		//IBS_������ �� å�� �����ڸ� ǥ��
		strData = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_������")));
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("IBS_������")), strData);
		//����⿡ å�� ����� ǥ��
		strData = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_�����")));
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("IBS_�����")), strData);
		// 2005-02-04 �����ڻ���
		strTmpData1 = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("IBS_������")));
		strTmpData2 = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("IBS_�����")));
		if(strTmpData2.IsEmpty()) strData = strTmpData1;
		else  strData = strTmpData1 + _T(", ") + strTmpData2;
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_�����ڻ���")), strData);
				
		//���������� �����Ѵ�.
		//dPrice			= (DOUBLE)atof(m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_����"))).GetBuffer(0));
		TCHAR* pEndPtr = NULL;
		dPrice = _tcstod(m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_����"))).GetBuffer(0), &pEndPtr);

		//dDiscRate = (DOUBLE)atof(m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BP_������"))).GetBuffer(0));
		dDiscRate = _tcstod(m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BP_������"))).GetBuffer(0), &pEndPtr);

		//dExchangeRate = (DOUBLE)atof(m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BP_ȯ��"))).GetBuffer(0));
		dExchangeRate = _tcstod(m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BP_ȯ��"))).GetBuffer(0), &pEndPtr);

		// UDF_�ܰ� = BB_����+�޸�����
		// 2006-04-01 KBS - ����� �䱸���� �ܰ��ʵ忡�� ȭ�󱸺п����� �Ҽ����� �����Ѵ�.
		if( _T("\\") == m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BP_ȭ�󱸺�"))) )
		{
			strData.Format(_T("%d"), (INT)dPrice);
		}
		else	strData = TruncPrice(m_pTargetDM, dPrice);
		PriceConvertMgr.ConversionPriceInfoToDesc(strData, strData);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_�ܰ�")), strData);
		// UDF_��ȯ����/��ȯ������ = BB_����*å��
		strData = TruncPrice(m_pTargetDM, dPrice*nBookCnt);
		PriceConvertMgr.ConversionPriceInfoToDesc(strData, strData);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_��ȯ����")), strData);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_��ȯ������")), strData);

		// ��¿� DM���� ���ְ��������� �������� �ʴ´�.
		/*
		//strData.Format(_T("%.2f"), dPrice*nBookCnt - ((dPrice*nBookCnt)*(dDiscRate/100)));		
		strData.Format(_T("%.0f"), (dPrice*nBookCnt - ((dPrice*nBookCnt*dDiscRate)/100)) * dExchangeRate);
		PriceConvertMgr.ConversionPriceInfoToDesc(strData, strData);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_��ȯ���ΰ���")), strData);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_��ȯ����������")), strData);
		*/
		//�ڷ���������� �����Ѵ�.
		strData = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("IBS_�ڷ����")));
		strData = strData.Left(3);
		if(strData == _T("BOA")) strData = _T("�����ڷ�");
		else if(strData == _T("BOC")) strData = _T("�����ڷ�");
		else if(strData == _T("BOR")) strData = _T("����ڷ�");
		else if(strData == _T("BOL")) strData = _T("�����ڷ�");
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_�ڷ���¼���")), strData);
		//�ܰ����� ����	
	}

	ids = m_pTargetDM->RefreshPositionInfo();
	if(ids < 0) return -1;

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���� �˸��ƽ��� ���� DM�� �Ǻ� ������ ������ TargetDM�� INSERT �Ѵ�.
// [ �̴Ͻ��� ]
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::ConvertToVolume(INT nBeginIdx, INT nRowCnt)
{
	if( m_pSourceDM == NULL || m_pTargetDM == NULL )	return -1;
	
	INT i, j, ids;
	DOUBLE dOrderPrice = 0.0;
	CString curVOL_SORT, strVol, tempVOL_SORT, strDBFLAG, strTmpData, strPrice, strRecKeys;	
	CArray <INT, INT> CopyRowIdxList;
	CopyRowIdxList.RemoveAll();
	
	// 2006-01-16 ������ - ������å���׹ݿ�, å���ְ����� ���� �ǹ��ְ��ݿ� �����Ѵ�.
	// ���ְ��� �Ҽ������뿩�θ� �˻��Ѵ�.	
	strVol = _T("");
	for( i = nBeginIdx; i <= nRowCnt; i++ )
	{
		strTmpData.Empty();
		m_pSourceDM->GetCellData(_T("BB_���Ϸù�ȣ"), i, curVOL_SORT);		
		if( strVol != curVOL_SORT )
		{
			CopyRowIdxList.Add(i);
			strVol = curVOL_SORT;
		}
	}
	
	INT nCol_VOL_SORTNO				= m_pTargetDM->FindColumn(_T("BB_���Ϸù�ȣ")	);
	INT nCol_UDF_BOOKCNT			= m_pTargetDM->FindColumn(_T("UDF_å��")		);
	INT nCol_UDF_DELIVERY_BOOKCNT	= m_pTargetDM->FindColumn(_T("UDF_��ǰå��")	);
	INT nCol_UDF_MISSING_BOOKCNT	= m_pTargetDM->FindColumn(_T("UDF_�̳�å��")	);
	INT nCol_PUB_YEAR				= m_pTargetDM->FindColumn(_T("IBS_�����")		);
	INT nCol_ORDERPRICE_VOL			= m_pTargetDM->FindColumn(_T("UDF_���ְ���_��")	);
	INT nCol_BOOK_RECKEY			= m_pTargetDM->FindColumn(_T("BB_åKEY")	);

	INT nCopyRowIdxList = CopyRowIdxList.GetSize();
	INT nDBookCnt, nBookCnt;
	CString strData;
	for (i = 0; i < nCopyRowIdxList; i++) 
	{
		// BOOK�� �� Row�� VOLUME���� �����Ѵ�.
		DWORD *pRow = (DWORD*)AddToTargetDM(CopyRowIdxList.GetAt(i));
		if( pRow == NULL )		return -1;

		// 2006-01-18 ������ - ������å���׹ݿ�, �ǹ��ְ����� �����Ѵ�.
		dOrderPrice = 0.0;
		strRecKeys.Empty();
		m_pSourceDM->GetCellData(_T("BB_���Ϸù�ȣ"), CopyRowIdxList.GetAt(i), curVOL_SORT);
		for( j=nBeginIdx; j<=nRowCnt; j++ )
		{
			m_pSourceDM->GetCellData(_T("BB_���Ϸù�ȣ"), j, tempVOL_SORT);
			if( curVOL_SORT == tempVOL_SORT )
			{
				m_pSourceDM->GetCellData(_T("BB_���ְ���_å"), j, strTmpData);
				//dOrderPrice += (DOUBLE)atof(strTmpData);
				TCHAR* pEndPtr = NULL;
				dOrderPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			}
			// 2006-01-24 ������ - ������å���׹ݿ�, åŰ���� �����Ѵ�. ( ������� )
			m_pSourceDM->GetCellData(_T("BB_åKEY"), j, strTmpData);
			if( strRecKeys.IsEmpty() == TRUE )	strRecKeys = strTmpData;
			else								strRecKeys += _T(", ") + strTmpData;
		}
		strPrice.Empty();
		strPrice.Format(_T("%lf"), dOrderPrice);
		m_pTargetDM->SetCellData(pRow, nCol_ORDERPRICE_VOL, strPrice);
		m_pTargetDM->SetCellData(pRow, nCol_BOOK_RECKEY, strRecKeys);
		//-----------------------------------------------------------------
		
		// ����� VOLUME�� ���� å���� ���Ͽ� VOLUME�� ����å��UDF �� �־��ش�.
		curVOL_SORT = m_pTargetDM->GetCellData(pRow, nCol_VOL_SORTNO);
		nBookCnt = GetEqualBookCount(m_pSourceDM, curVOL_SORT, nBeginIdx, nRowCnt);
		nDBookCnt = GetEqualBookCount(m_pSourceDM, curVOL_SORT, nBeginIdx, nRowCnt, _T("Y"));
		strData.Format(_T("%d"), nBookCnt);
		m_pTargetDM->SetCellData(pRow, nCol_UDF_BOOKCNT, strData);
		strData.Format(_T("%d"), nDBookCnt);
		m_pTargetDM->SetCellData(pRow, nCol_UDF_DELIVERY_BOOKCNT, strData);
		strData.Format(_T("%d"), nBookCnt - nDBookCnt);
		m_pTargetDM->SetCellData(pRow, nCol_UDF_MISSING_BOOKCNT, strData);
		strData = m_pTargetDM->GetCellData(pRow, nCol_PUB_YEAR);
	}

	ids = m_pTargetDM->RefreshPositionInfo();
	if(ids < 0) return -1;

	return 0;
}

INT CBO_DataConverter::GetEqualBookCount(CESL_DataMgr *pDM_BOOK, CString &strVOL, INT nBeginIdx, INT nRowCnt, CString strDeliveryYN/*=_T("")*/)
{
	if( pDM_BOOK == NULL )		return 0;

	INT ids, i, nEqualBookCount = 0;
	CString tempVOL, strTmpData;
	for( i=nBeginIdx; i<=nRowCnt; i++ )
	{
		ids = pDM_BOOK->GetCellData(_T("BB_���Ϸù�ȣ"), i, tempVOL);
		pDM_BOOK->GetCellData(_T("BB_��ǰ����"), i, strTmpData);
		if( strVOL == tempVOL )
		{
			if( strDeliveryYN.IsEmpty() == TRUE )								nEqualBookCount++;
			else if( !strDeliveryYN.IsEmpty() && strDeliveryYN == strTmpData )	nEqualBookCount++;
		}	
	}
	
	return nEqualBookCount;
}

CString CBO_DataConverter::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	// 2004.12.01 ������å�� ���� ��������������� �о�ͼ� ������ �ڸ�����ŭ ǥ���ϵ��� �Ѵ�.
	INT nPos = -1;
	CString strTmpData, strTmp, strValue;
	

	strTmp.Format(_T("%.2f"), dPrice);
	nPos = strTmp.Find(_T("."));

	// 2005-01-31
	if		( m_strPosNumber == _T("1") )	strTmpData = strTmp.Left(nPos+2);
	else if	( m_strPosNumber == _T("2") )	strTmpData = strTmp.Left(nPos+3);
	else									strTmpData = strTmp.Left(nPos);

	return strTmpData;
}
