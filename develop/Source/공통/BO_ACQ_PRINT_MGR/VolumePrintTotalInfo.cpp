// VolumePrintTotalInfo.cpp: implementation of the CVolumePrintTotalInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VolumePrintTotalInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������
CVolumePrintTotalInfo::CVolumePrintTotalInfo()
{
	// KOL.UDF.022 ��ť���ڵ� ����
	m_dDB_DiscTotalPrice = -1;;
	m_nTotalMissingBookCnt = -1;
	m_nTotalDeliveryBookCnt = -1;
}
// �Ҹ���
CVolumePrintTotalInfo::~CVolumePrintTotalInfo()
{

}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���ڷ� �� �˸��ƽ��� ��������� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
VOID CVolumePrintTotalInfo::SetAliasInfo(CString strBookCntAlias, 
										 CString strPriceAlias, 
										 CString strDiscPriceAlias,
										 CString strMissingBookCntAlias,
										 CString strDeliveryBookCntAlias)
{
	m_strBookCntAlias = strBookCntAlias;
	m_strPriceAlias = strPriceAlias;
	m_strDiscPriceAlias = strDiscPriceAlias;
	m_strMissingBookCntAlias = strMissingBookCntAlias;
	m_strDeliveryBookCntAlias = strDeliveryBookCntAlias;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ������� �� �ʱ�ȭ
//////////////////////////////////////////////////////////////////////
VOID CVolumePrintTotalInfo::InitTotalInfo()
{
	m_dTotalPrice = m_dDiscTotalPrice = 0.0;
	m_nTotalBookCnt = m_nTotalSpeciesCnt = m_nTotalMissingBookCnt = m_nTotalDeliveryBookCnt = 0;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
//////////////////////////////////////////////////////////////////////
VOID CVolumePrintTotalInfo::GetTotalMissingBookCnt(CString &strBookCnt)
{
	strBookCnt.Format(_T("%d"), m_nTotalMissingBookCnt);
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
//////////////////////////////////////////////////////////////////////
VOID CVolumePrintTotalInfo::GetTotalDeliveryBookCnt(CString &strBookCnt)
{
	strBookCnt.Format(_T("%d"), m_nTotalDeliveryBookCnt);
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
//////////////////////////////////////////////////////////////////////
INT CVolumePrintTotalInfo::SetTotalInfo(CESL_DataMgr *pDM, INT nFirstIdx/*=-1*/, INT nLastIdx/*=-1*/)
{
	INT i;
	DOUBLE dTempPrice = 0.0;
	CString strTmpData, strAcqKey, strQuery = _T("");
	CStringArray astrAcqKeys;	astrAcqKeys.RemoveAll();
	CMapStringToString mapTmp;	mapTmp.RemoveAll();

	TCHAR* pEndPtr = NULL;

	if( pDM == NULL )						return -1;
	if( pDM->GetRowCount() == 0 )			return 0;	
	if( CheckTotalInfoAlias() == FALSE )	return -1;
	
	InitTotalInfo();
	
	if( nFirstIdx < 0 )		nFirstIdx = 0;
	if( nLastIdx < 0  )		nLastIdx = pDM->GetRowCount();

	for( i=nFirstIdx ; i<nLastIdx ; i++ )
	{
		strTmpData = pDM->GetCellData(_T("IBS_��KEY"), i);
		mapTmp.SetAt(strTmpData, _T(""));
		
		if( m_strBookCntAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strBookCntAlias, i);
			m_nTotalBookCnt += _ttoi(strTmpData.GetBuffer(0));
		}
		
		if( m_strMissingBookCntAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strMissingBookCntAlias, i);
			m_nTotalMissingBookCnt += _ttoi(strTmpData.GetBuffer(0));
		}
		
		if( m_strDeliveryBookCntAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strDeliveryBookCntAlias, i);
			m_nTotalDeliveryBookCnt += _ttoi(strTmpData.GetBuffer(0));
		}
		if( m_strPriceAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strPriceAlias, i);
			strTmpData.Replace(_T(","), _T(""));
			//m_dTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			m_dTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
		}
		if( m_strDiscPriceAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strDiscPriceAlias, i);
			strTmpData.Replace(_T(","), _T(""));
			//m_dDiscTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			m_dDiscTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
		}
		
		// 2006-01-23 ������ - ������å���׹ݿ�, ������ �����ϱ� ���Ͽ� DB�� ����� ���԰����� �����´�.
		strAcqKey.Empty();
		strAcqKey = pDM->GetCellData(_T("BB_åKEY"), i);		
		//strAcqKey = pDM->GetCellData(_T("BP_��������KEY"), i);
		if( strAcqKey.IsEmpty() == TRUE )	continue;
		astrAcqKeys.Add(strAcqKey);
		
	}

	
	// DB���԰����� ���� �����Ѵ�.
	if( astrAcqKeys.GetSize() > 0 )
	{
		strAcqKey.Empty();
		strAcqKey = astrAcqKeys.GetAt(0);
		for( i=1; i<astrAcqKeys.GetSize(); i++ )	strAcqKey += _T(", ") + astrAcqKeys.GetAt(i);
		
		
		//=============================================================================================
		//2008.12.04 UPDATE BY LKS : REC_KEY IN()���� 1000�� �̻� REC_KEY�� �� �� ���� ���� ����
		/*�����ҽ�
		strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE)) ")
						_T("FROM BO_PURCHASEINFO_TBL ")
						_T("WHERE REC_KEY IN ( %s ) "), strAcqKey);
		*/
		/*
		strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(ACQ_KEY, PRICE, 'Y', UDF_MANAGE_CODE)), UDF_MANAGE_CODE) ")
						_T("FROM BO_BOOK_TBL ")
						_T("WHERE REC_KEY IN ( %s ) "), strAcqKey);
		*/
		MakeQueryByRecKeyList(strAcqKey,strQuery);
		//=============================================================================================


		pDM->GetOneValueQuery(strQuery, strTmpData);
		//dTempPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		dTempPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
	}
	else dTempPrice = 0;

	m_dDB_DiscTotalPrice = dTempPrice;
	

	m_nTotalSpeciesCnt = mapTmp.GetCount();

	return 0;	
}

VOID CVolumePrintTotalInfo::GetDB_DiscTotalPrice(CString &strDiscTotalPrice, CString strTruncUnit)
{
	if		( strTruncUnit == _T("0") )
	{
		strDiscTotalPrice.Format(_T("%.0f"), m_dDB_DiscTotalPrice);
	}
	else if	( strTruncUnit == _T("1") )
	{
		strDiscTotalPrice.Format(_T("%.1f"), m_dDB_DiscTotalPrice);
	}
	else if	( strTruncUnit == _T("2") )
	{
		strDiscTotalPrice.Format(_T("%.2f"), m_dDB_DiscTotalPrice);
	}
	else
	{
		strDiscTotalPrice.Format(_T("%.0f"), m_dDB_DiscTotalPrice);
	}
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strDiscTotalPrice, strDiscTotalPrice);
}

INT CVolumePrintTotalInfo::MakeQueryByRecKeyList(CString strRecKeyList,CString &strQuery)
{
	
	CString strTemp;
 	CString strRecKey;
 	CString strBookKeyTempList;
 	CString strQueryWhere;
 	INT nCnt;
 	INT nLocation;
 	INT nStartLocation;
	INT i ; 
	strBookKeyTempList = strRecKeyList;
	strBookKeyTempList.Replace(_T("'"), _T(""));
	nCnt = 0;
	nLocation = 0;
	strQueryWhere.Format(_T(""));
	while( 1 )
	{
		nLocation = strBookKeyTempList.Find(_T(","), nLocation+1);
		++nCnt;
		if( 0 > nLocation )
		{
			break;			
		}
	}
	if ( (nCnt % 1000) == 0 )
	{
		nCnt = (nCnt/1000);
	}
	else
	{
		nCnt = (nCnt/1000)+1;
	}
	nStartLocation = strBookKeyTempList.GetLength()/nCnt;

	for( i = 0 ; i < nCnt ; i++)
	{
		if( i == (nCnt -1) )
		{
			strRecKey = strBookKeyTempList;
		}
		else
		{
			nLocation = strBookKeyTempList.Find(_T(","), nStartLocation);
			strRecKey = strBookKeyTempList.Mid(0, nLocation );
			strTemp.Format(_T("%s,"), strRecKey);
			strBookKeyTempList.Replace( strTemp, _T("") );
		}
	
		if( strQueryWhere.IsEmpty() )
		{
			strQueryWhere.Format(_T("REC_KEY IN (%s)"),  strRecKey);
		}
		else
		{
			strTemp.Format(_T("%s OR REC_KEY IN( %s )"), strQueryWhere, strRecKey );
			strQueryWhere = strTemp;
		}
	}
	
	strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(ACQ_KEY, PRICE, 'Y', UDF_MANAGE_CODE)), UDF_MANAGE_CODE) ")
						_T("FROM BO_BOOK_TBL ")
						_T("WHERE %s "),strQueryWhere);

	return 0;
}