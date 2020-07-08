// PrintTotalInfo.cpp: implementation of the CPrintTotalInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrintTotalInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������
CPrintTotalInfo::CPrintTotalInfo()
{
	m_strBookCntAlias	= _T("");
	m_strPriceAlias		= _T("");
	m_strDiscPriceAlias = _T("");
	InitTotalInfo();
	m_strTruncPrice.Empty();

	// KOL.UDF.022 ��ť���ڵ� ����
	InitTotalInfo();
	m_nAcqType = -1;	
}
// �Ҹ���
CPrintTotalInfo::~CPrintTotalInfo()
{

}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ����������� �ʱ�ȭ
//////////////////////////////////////////////////////////////////////
VOID CPrintTotalInfo::InitTotalInfo()
{
	m_dTotalPrice		= 0.0;
	m_dDiscTotalPrice_Display	= 0.0;
	m_dDiscTotalPrice	= 0.0;
	m_nTotalBookCnt		= 0;
	m_nTotalSpeciesCnt	= 0;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��������������������� _T("����")�� ǥ���Ͽ� �����Ѵ�.(�޸��� �����) -> ��ȯ���ΰ���
//////////////////////////////////////////////////////////////////////
VOID CPrintTotalInfo::GetTotalPrice(CString &strTotalPrice)
{
	//2005-01-17
	if		( m_strTruncPrice == _T("1") )	strTotalPrice.Format(_T("%.1f"), m_dTotalPrice);
	else if	( m_strTruncPrice == _T("2") )	strTotalPrice.Format(_T("%.2f"), m_dTotalPrice);
	else								strTotalPrice.Format(_T("%.0f"), m_dTotalPrice);

	// �޸��� �����
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strTotalPrice, strTotalPrice);
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���԰���(���ְ���)�� �����Ѵ�.(�޸��� �����)
//////////////////////////////////////////////////////////////////////
VOID CPrintTotalInfo::GetDiscTotalPrice(CString &strDiscTotalPrice, CString strTruncUnit)
{
	// ���翡���� ���԰��� �ʿ����� ó��
	// 2005.11.02 ������ �߰�	
	INT nPrice = 0;		
	
	if		( strTruncUnit == _T("0") )
	{
		strDiscTotalPrice.Format(_T("%.0f"), m_dDiscTotalPrice);
	}
	else if	( strTruncUnit == _T("1") )
	{
		strDiscTotalPrice.Format(_T("%.1f"), m_dDiscTotalPrice);
	}
	else if	( strTruncUnit == _T("2") )
	{
		strDiscTotalPrice.Format(_T("%.2f"), m_dDiscTotalPrice);
	}
	else
	{
		strDiscTotalPrice.Format(_T("%.0f"), m_dDiscTotalPrice);
	}
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strDiscTotalPrice, strDiscTotalPrice);
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �������� ���ڷ� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
VOID CPrintTotalInfo::GetTotalSpeciesCnt(CString &strTotalSpeciesCnt)
{
	strTotalSpeciesCnt.Format(_T("%d"), m_nTotalSpeciesCnt);
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��å���� ���ڷ� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
VOID CPrintTotalInfo::GetTotalBookCnt(CString &strTotalBookCnt)
{
	strTotalBookCnt.Format(_T("%d"), m_nTotalBookCnt);
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���ݾ˸��ƽ� ���� �ִ��� Ȯ���Ѵ�.
//////////////////////////////////////////////////////////////////////
BOOL CPrintTotalInfo::CheckTotalInfoAlias()
{
	if( m_strPriceAlias.IsEmpty() == TRUE )		return FALSE;
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���ڷ� �� �˸��ƽ��� ��������� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
VOID CPrintTotalInfo::SetAliasInfo(CString strBookCntAlias, CString strPriceAlias, CString strDiscPriceAlias)
{
	m_strBookCntAlias	= strBookCntAlias;
	m_strPriceAlias		= strPriceAlias;
	m_strDiscPriceAlias = strDiscPriceAlias;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��, å, ������ ������ ���Ͽ� ��������� �����Ѵ�. (�Ұ��ڷ�)
//
// [ �̴Ͻ��� ]
// 1. ������ ���Ͽ� ��������� �ʱ�ȭ�Ѵ�.
// 2. ����ǥ������ ���� ó���ϱ� ���Ͽ� �����ڸ��� �������� �����´�.
// 3. ���ڷ� �� DM�� �������� ������� ������ ��ü�� �����Ѵ�.
// 4. ����Ϸ��� DM�� ������ 1000���� �Ѵ´ٸ� 1000���� ����Ѵ�.
// 5. ������ �ο쿡 �ش��ϴ� DM�� �Ѱ踦 ���Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CPrintTotalInfo::SetTotalInfo(CESL_DataMgr *pDM, INT nFirstIdx/*=-1*/, INT nLastIdx/*=-1*/)
{
	//================================================================
	// ��������
	//================================================================
	INT i, q, nQuota, nCnt, nRow ;
	DOUBLE dTempPrice;
	CString strTmpData, strQuery, strDisPrice;
	CStringArray astrAcqKeys;

	TCHAR* pEndPtr = NULL;

	if( pDM == NULL )						return -1;
	if( pDM->GetRowCount() == 0 )			return  0;
	if( CheckTotalInfoAlias() == FALSE )	return -1;

	//================================================================
	// 1. ������ ���Ͽ� ��������� �ʱ�ȭ�Ѵ�.
	//================================================================
	InitTotalInfo();


	//================================================================
	// 2. ����ǥ������ ���� ó���ϱ� ���Ͽ� �����ڸ��� �������� �����´�.
	// 2005-01-17
	//================================================================
	if( m_strTruncPrice.IsEmpty() == TRUE )
	{
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '����'AND GROUP_3 = '����' AND CLASS_ALIAS = '���������������'"), m_strTruncPrice);
//DEL 		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '����'AND GROUP_3 = '����' AND CLASS_ALIAS = '���������������'"), m_strTruncPrice);
		// default : �Ҽ��� �������.
		if( m_strTruncPrice.IsEmpty() == TRUE )		m_strTruncPrice = _T("0");
	}


	//================================================================
	// 3. ���ڷ� �� DM�� �������� ������� ������ ��ü�� �����Ѵ�.
	//================================================================
	if( nFirstIdx < 0 )		nFirstIdx = 0;
	if( nLastIdx < 0 )		nLastIdx = pDM->GetRowCount();

	//================================================================
	// 4. ����Ϸ��� DM�� ������ 1000���� �Ѵ´ٸ� 1000���� ����Ѵ�.
	// 2005-04-15 by SM5ong �հ踦 1000���� ��� ����ϵ��� �Ѵ�.
	//================================================================
	// BEGIN:
	m_dDiscTotalPrice_Display = 0.0;
	nCnt = nRow = 0;	
	dTempPrice = 0.0;
	astrAcqKeys.RemoveAll();
	nQuota = (nLastIdx-nFirstIdx) / QUOTA_PRINT; // ��
	if( nQuota > 0 )
	{
		// 1000������ ��� �հ踦 ���Ѵ�.
		for( q=0; q<nQuota; q++ )  
		{
			if( q == 0 )	nCnt = nFirstIdx;
			else			nCnt = 0;
	
			for( i=nCnt ; i<QUOTA_PRINT ; i++ )
			{
				// ����ο츦 ����Ѵ�.
				nRow = (q*QUOTA_PRINT) + i;

				// ���� ����
				m_nTotalSpeciesCnt++;
				
				// å�� ����
				if( m_strBookCntAlias.IsEmpty() == FALSE )
				{
					//strTmpData = pDM->GetCellData(m_strBookCntAlias, nRow);
					strTmpData = pDM->GetCellData(_T("UDF_å��"), nRow);
					m_nTotalBookCnt += _ttoi(strTmpData.GetBuffer(0));
				}
				else 
				{
					m_nTotalBookCnt++;
				}

				// ���� �Ѱ� ���
				if( m_strPriceAlias.IsEmpty() == FALSE )
				{
					strTmpData = pDM->GetCellData(m_strPriceAlias, nRow);
					strTmpData.Replace(_T(","), _T(""));
					//m_dTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
					m_dTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
				}

				// ���԰���(���ְ���) ����� ���� REC_KEY�� �����Ѵ�.
				if( m_strDiscPriceAlias.IsEmpty() == FALSE )
				{
					strTmpData = pDM->GetCellData(m_strDiscPriceAlias, nRow);					
					if( strTmpData.IsEmpty() == TRUE )	continue;
					strTmpData.Replace(_T(","), _T(""));
					//m_dDiscTotalPrice_Display += (DOUBLE)atof(strTmpData);
					m_dDiscTotalPrice_Display += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
					
					// ��������KEY�� �ƴ� å�� REC_KEY�� ���ؿ� å������ ���ְ����� ����Ѵ�.
					strTmpData = pDM->GetCellData(_T("BP_��������KEY"), nRow);
					if( strTmpData.IsEmpty() == TRUE )	continue;

					astrAcqKeys.Add(strTmpData);
					//if( strDisPrice.IsEmpty() == TRUE )		strDisPrice += strTmpData;
					//else									strDisPrice += _T(", ") + strTmpData;
				}				
			}

			// ����� REC_KEY�� �̿��Ͽ� �Ѱ踦 ���Ѵ�.
			if( astrAcqKeys.GetSize() > 0 )
			{
				strDisPrice.Empty();
				strDisPrice = astrAcqKeys.GetAt(0);
				for( i=1; i<astrAcqKeys.GetSize(); i++ )	strDisPrice += _T(", ") + astrAcqKeys.GetAt(i);
				// 2006.02.01 ������ - ������å���� ���� �ݿ�, �����ְ����� ���� �Ѱ��̴�.
				if( m_nAcqType == 2 )
				{
					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE, UDF_MANAGE_CODE)) ")
//DEL 					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE)) ")
									_T("FROM BO_DONINFO_TBL ")
									_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
				}
				else
				{
					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE, UDF_MANAGE_CODE)) ")
//DEL 					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE)) ")
									_T("FROM BO_PURCHASEINFO_TBL ")
									_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
				}
				/*
				strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(ACQ_KEY, PRICE, 'Y'))) ")
								_T("FROM BO_BOOK_TBL ")
								_T("WHERE ACQ_KEY IN ( %s ) "), strDisPrice);
				*/
				pDM->GetOneValueQuery(strQuery, strTmpData);
				//dTempPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dTempPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
			}
			else dTempPrice = 0;

			m_dDiscTotalPrice += dTempPrice;
			astrAcqKeys.RemoveAll();
			
			// 2006-01-23 ������ - ������å���׹ݿ�, ���԰��� �Ѱ�� �ѹ��� ����Ѵ�.
			/*
			// ����� REC_KEY�� �̿��Ͽ� ���԰����� ������ ���Ѵ�.
			if( strDisPrice.IsEmpty() == FALSE )
			{
				strQuery.Format(_T("SELECT SUM(GETPRICEWON(b.ACQ_KEY, b.PRICE, 'Y')) ")
								_T("FROM BO_BOOK_TBL B ")
								_T("WHERE b.ACQ_KEY IN ( %s ) "), strDisPrice);
				pDM->GetOneValueQuery(strQuery, strTmpData);
				dTempPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			}
			else dTempPrice = 0;

			// �Ѱ迡 ���Ѵ�.
			m_dDiscTotalPrice += dTempPrice;
			*/
		}
	}

	//================================================================
	// 5. ������ �ο쿡 �ش��ϴ� DM�� �Ѱ踦 ���Ѵ�.
	//================================================================
	dTempPrice = 0.0;
	if( nQuota==0 ) nCnt = nFirstIdx;
	else			nCnt = nQuota*QUOTA_PRINT;

	for( i=nCnt ; i<nLastIdx ; i++ )
	{
		// ���� ����
		m_nTotalSpeciesCnt++;
		
		// å�� ����
		if( m_strBookCntAlias.IsEmpty() == FALSE )
		{
			//strTmpData = pDM->GetCellData(m_strBookCntAlias, i);
			strTmpData = pDM->GetCellData(_T("UDF_å��"), i);
			m_nTotalBookCnt += _ttoi(strTmpData.GetBuffer(0));
		}
		else 
		{
			m_nTotalBookCnt++;
		}

		// ���� �ջ�
		if( m_strPriceAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strPriceAlias, i);			
			strTmpData.Replace(_T(","), _T(""));
			//m_dTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			m_dTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		}

		// ���԰��� �Ѱ� �ջ��� ���� REC_KEY ����
		if( m_strDiscPriceAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strDiscPriceAlias, i);			
			if( strTmpData.IsEmpty() == TRUE )	continue;
			strTmpData.Replace(_T(","), _T(""));
			//m_dDiscTotalPrice_Display += (DOUBLE)atof(strTmpData);
			m_dDiscTotalPrice_Display += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
			
			// 2005-02-02
			strTmpData = pDM->GetCellData(_T("BP_��������KEY"), i);
			//strTmpData = pDM->GetCellData(_T("BB_åKEY"), i);
			if( strTmpData.IsEmpty() == TRUE )	continue;

			astrAcqKeys.Add(strTmpData);
			//if( strDisPrice.IsEmpty()  == TRUE )	strDisPrice += strTmpData;
			//else									strDisPrice += _T(", ") + strTmpData;
		}
		
	}

	// ����� REC_KEY�� �̿��Ͽ� �Ѱ踦 ���Ѵ�.
	if( astrAcqKeys.GetSize() > 0 )
	{
		strDisPrice.Empty();
		strDisPrice = astrAcqKeys.GetAt(0);
		for( i=1; i<astrAcqKeys.GetSize(); i++ )	strDisPrice += _T(", ") + astrAcqKeys.GetAt(i);	
		// 2006.02.01 ������ - ������å���� ���� �ݿ�, �����ְ����� ���� �Ѱ��̴�.
		if( m_nAcqType == 2 )
		{
			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE, UDF_MANAGE_CODE)) ")
//DEL 			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE)) ")
							_T("FROM BO_DONINFO_TBL ")
							_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
		}
		else
		{
			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE, UDF_MANAGE_CODE)) ")
//DEL 			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE)) ")
							_T("FROM BO_PURCHASEINFO_TBL ")
							_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
		}
		/*
		strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(ACQ_KEY, PRICE, 'Y'))) ")
						_T("FROM BO_BOOK_TBL ")
						_T("WHERE ACQ_KEY IN ( %s ) "), strDisPrice);
		*/
		pDM->GetOneValueQuery(strQuery, strTmpData);
		//dTempPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		dTempPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
	}
	else
	{
		dTempPrice = 0;
	}

	m_dDiscTotalPrice += dTempPrice;	
	// END:
	
	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��, å, ������ ������ ���Ͽ� ��������� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
INT CPrintTotalInfo::SetTotalInfo(CMapStringToString * pArrData, CString strDivAlias, CESL_DataMgr *pDM, INT nFirstIdx/*=-1*/, INT nLastIdx/*=-1*/)
{
	//================================================================
	// ��������
	//================================================================
	INT i, q, nQuota, nCnt, nRow;
	DOUBLE dTempPrice;
	CString strTmpData, strTmpValue, strQuery, strDisPrice;
	CStringArray astrAcqKeys;

	if( pDM == NULL )						return -1;
	if( pDM->GetRowCount() == 0 )			return 0;	
	if( CheckTotalInfoAlias() == FALSE )	return -1;

	
	//================================================================
	// 1. ������ ���Ͽ� ��������� �ʱ�ȭ ��Ų��.
	//================================================================
	InitTotalInfo();


	//================================================================
	// 2. �Ҽ��� ������ ���� �����ڸ��� �������� �����´�.
	// 2005-01-17
	//================================================================
	if( m_strTruncPrice.IsEmpty() == TRUE )
	{
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '����'AND GROUP_3 = '����' AND CLASS_ALIAS = '���������������'"), m_strTruncPrice);
//DEL 		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '����'AND GROUP_3 = '����' AND CLASS_ALIAS = '���������������'"), m_strTruncPrice);
		if( m_strTruncPrice.IsEmpty() == TRUE )		m_strTruncPrice = _T("0");
	}
	

	//================================================================
	// 3. ����� ������ �����ϰ�쿡 ��ü�� ����Ѵ�.
	//================================================================
	if( nFirstIdx < 0 )		nFirstIdx = 0;
	if( nLastIdx < 0 )		nLastIdx = pDM->GetRowCount();

	//================================================================
	// 4. ����Ϸ��� DM�� ������ 1000���� �Ѵ´ٸ� 1000���� ����Ѵ�.
	// 2005-04-15 by SM5ong �հ踦 1000���� ��� ����ϵ��� �Ѵ�.
	//================================================================
	// BEGIN:
	nCnt = nRow = 0;
	astrAcqKeys.RemoveAll();
	nQuota = (nLastIdx-nFirstIdx) / QUOTA_PRINT; // ��
	if( nQuota > 0 )
	{
		// 1000������ ��� �հ踦 ���Ѵ�.
		for( q=0; q<nQuota; q++ )  
		{
			strDisPrice.Empty();
			dTempPrice = 0.0;
			if( q==0 )	nCnt = nFirstIdx;
			else		nCnt = 0;
	
			for( i=nCnt ; i<QUOTA_PRINT ; i++ )
			{
				// ���� �ο츦 ����Ѵ�.
				nRow = (q * QUOTA_PRINT) + i;

				strTmpData = pDM->GetCellData(strDivAlias, nRow);
				if( pArrData && pArrData->GetCount() > 0 )
				{
					if( !pArrData->Lookup(strTmpData, strTmpValue) )	continue;
				}

				// ���� ����
				m_nTotalSpeciesCnt++;
				
				// å�� ����
				if( m_strBookCntAlias.IsEmpty()  == FALSE )
				{
					strTmpData = pDM->GetCellData(m_strBookCntAlias, nRow);
					m_nTotalBookCnt += _ttoi(strTmpData.GetBuffer(0));
				}
				else 
				{
					m_nTotalBookCnt++;
				}

				// ���� ���
				if( m_strPriceAlias.IsEmpty() == FALSE )
				{
					strTmpData = pDM->GetCellData(m_strPriceAlias, nRow);
					strTmpData.Replace(_T(","), _T(""));
					//m_dTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
					TCHAR* pEndPtr = NULL;
					m_dTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
				}

				// ������ ����� ���� REC_KEY�� �����Ѵ�.
				if( m_strDiscPriceAlias.IsEmpty() == FALSE )
				{
					strTmpData = pDM->GetCellData(_T("BP_��������KEY"), nRow);
					//strTmpData = pDM->GetCellData(_T("BB_åKEY"), nRow);
					if( strTmpData.IsEmpty() == TRUE ) continue;

					astrAcqKeys.Add(strTmpData);
					//if( strDisPrice.IsEmpty() == TRUE )		strDisPrice += strTmpData;
					//else									strDisPrice += _T(", ") + strTmpData;
				}
			}

			// 1000 ���� �߶� ���԰����� ����Ѵ�.
			if( astrAcqKeys.GetSize() > 0  )
			{
				strDisPrice.Empty();
				strDisPrice = astrAcqKeys.GetAt(0);
				for( i=1; i<astrAcqKeys.GetSize(); i++ )	strDisPrice += _T(", ") + astrAcqKeys.GetAt(i);
				// 2006.02.01 ������ - ������å���� ���� �ݿ�, ���ְ����������� �Ѱ踦 ����Ѵ�.
				if( m_nAcqType == 2 )
				{
					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE, UDF_MANAGE_CODE)) ")
//DEL 					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE)) ")
									_T("FROM BO_DONINFO_TBL ")
									_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
				}
				else
				{
					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE, UDF_MANAGE_CODE)) ")
//DEL 					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE)) ")
									_T("FROM BO_PURCHASEINFO_TBL ")
									_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
				}
				/*
				strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(b.ACQ_KEY, b.PRICE, 'Y'))) ")
								_T("FROM BO_BOOK_TBL B ")
								_T("WHERE b.ACQ_KEY IN ( %s ) "), strDisPrice);
				
				// ������ ���� - å���ְ����� ���� �ƴ� �����ְ����� ������ �����´�.
				strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(ACQ_KEY, PRICE, 'Y'))) ")
								_T("FROM BO_BOOK_TBL ")
								_T("WHERE ACQ_KEY IN ( %s ) "), strDisPrice);
				*/
				pDM->GetOneValueQuery(strQuery, strTmpData);
				//dTempPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				TCHAR* pEndPtr = NULL;
				dTempPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			}
			else dTempPrice = 0;

			m_dDiscTotalPrice += dTempPrice;
			astrAcqKeys.RemoveAll();

			/*
			if( strDisPrice.IsEmpty() == FALSE )
			{
				strQuery.Format(_T("SELECT SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, 'Y')) ")
								_T("FROM BO_BOOK_TBL B ")
								_T("WHERE b.ACQ_KEY in ( %s ) "), strDisPrice);
				pDM->GetOneValueQuery(strQuery, strTmpData);
				dTempPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			}
			else dTempPrice = 0;

			m_dDiscTotalPrice += dTempPrice;
			*/
		}
	}

	//================================================================
	// 5. ������ �ο쿡 �ش��ϴ� DM�� �Ѱ踦 ���Ѵ�.
	//================================================================
	strDisPrice.Empty();
	dTempPrice = 0.0;
	if( nQuota==0 )	nCnt=nFirstIdx;
	else			nCnt=nQuota*QUOTA_PRINT;

	for( i=nCnt ; i<nLastIdx ; i++ )
	{
		strTmpData = pDM->GetCellData(strDivAlias, i);
		if( pArrData && pArrData->GetCount() > 0 )
		{
			if( !pArrData->Lookup(strTmpData, strTmpValue) )	continue;
		}

		m_nTotalSpeciesCnt++;
		
		if( m_strBookCntAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strBookCntAlias, i);
			m_nTotalBookCnt += _ttoi(strTmpData.GetBuffer(0));
		}
		else 
		{
			m_nTotalBookCnt++;
		}

		if( m_strPriceAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strPriceAlias, i);
			strTmpData.Replace(_T(","), _T(""));
			//m_dTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			TCHAR* pEndPtr = NULL;
			m_dTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
		}

		if( m_strDiscPriceAlias.IsEmpty() == FALSE )
		{
			// ��������KEY��� åKEY�� �����Ѵ�.
			strTmpData = pDM->GetCellData(_T("BP_��������KEY"), i);
			//strTmpData = pDM->GetCellData(_T("BB_åKEY"), i);
			if( strTmpData.IsEmpty() == TRUE )	continue;

			astrAcqKeys.Add(strTmpData);
			//if( strDisPrice.IsEmpty() == TRUE )		strDisPrice += strTmpData;
			//else									strDisPrice += _T(", ") + strTmpData;
		}
	}

	if( astrAcqKeys.GetSize() > 0  )
	{
		strDisPrice.Empty();
		strDisPrice = astrAcqKeys.GetAt(0);
		for( i=1; i<astrAcqKeys.GetSize(); i++ )	strDisPrice += _T(", ") + astrAcqKeys.GetAt(i);
		// 2006.02.01 ������ - ������å���� ���� �ݿ�, ���ְ����������� �Ѱ踦 ����Ѵ�.
		if( m_nAcqType == 2 )
		{
			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE, UDF_MANAGE_CODE)) ")
//DEL 			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE)) ")
							_T("FROM BO_DONINFO_TBL ")
							_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
		}
		else
		{
			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE, UDF_MANAGE_CODE)) ")
//DEL 			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE)) ")
							_T("FROM BO_PURCHASEINFO_TBL ")
							_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
		}
		/*
		strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(b.ACQ_KEY, b.PRICE, 'Y'))) ")
						_T("FROM BO_BOOK_TBL B ")
						_T("WHERE b.ACQ_KEY IN ( %s ) "), strDisPrice);
		
		// ������ ���� - å���ְ����� ���� �ƴ� �����ְ����� ������ �����´�.
		strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(ACQ_KEY, PRICE, 'Y'))) ")
						_T("FROM BO_BOOK_TBL ")
						_T("WHERE ACQ_KEY IN ( %s ) "), strDisPrice);
		*/
		pDM->GetOneValueQuery(strQuery, strTmpData);
		//dTempPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		TCHAR* pEndPtr = NULL;
		dTempPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
	}
	else dTempPrice = 0;

	m_dDiscTotalPrice += dTempPrice;
	// END:

	return 0;
}

VOID CPrintTotalInfo::SetDiscTotalPrice(DOUBLE dTmpPrice)
{
	//20070820 UPDATE PJW :������ ���� dTmpPrice = m_dDiscTotalPrice; �� ���� 
	//������ ������ ������ �޾ƿͼ� ������ �ִ� ������ �־ ���԰����� 0.00���� ������ 
	//m_dDiscTotalPrice = dTmpPrice;
	//20070903 UPDATE PJW : ���԰� �������� ���տ� ���Ǵ� ������ �޶� ���� ����� ���� �ʾ���.
	if( m_dDiscTotalPrice == 0.)
	{
		m_dDiscTotalPrice = dTmpPrice;
	}
	else
	{
		dTmpPrice = m_dDiscTotalPrice;		
	}
}