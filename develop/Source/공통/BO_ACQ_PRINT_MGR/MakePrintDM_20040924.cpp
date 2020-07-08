// MakePrintDM.cpp: implementation of the CMakePrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakePrintDM.h"
#include "../BO_DATA_MANAGER/BO_DataConverter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakePrintDM::CMakePrintDM(CESL_Mgr * pParentMgr)
{
	m_pParentMgr = pParentMgr;
	m_pPrintDM = NULL;
	m_nPrintType = -1;
	m_IgnoreAlias.RemoveAll();
	m_pDataProcessingDlg = NULL;
	m_pArrDivValeu = NULL;
}

CMakePrintDM::~CMakePrintDM()
{
	//���α׷��� ��Ʈ�� �����
	HideDataProcessingDlg();
}

INT CMakePrintDM::InitMakePrintDM()
{
	return 0;
}

INT CMakePrintDM::MakeVolumeDM(CESL_DataMgr * pDM, 
							   CESL_DataMgr * pBookDM, 
							   CString strDivAlias, 
							   BOOL bPrintVolume/*=TRUE*/)
{
	if(pBookDM == NULL) return -1;
	if(pBookDM->GetRowCount() == 0) return 0;
	
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��¿� �������� �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	}
	pDM->FreeData();
	
	CBO_DataConverter Converter;
	Converter.SetDivAlias(strDivAlias);
	Converter.SetProgressCtrl(m_pDataProcessingDlg->GetProgressCtrl());
	Converter.SetSourceDM(pBookDM);
	Converter.SetTargetDM(pDM);
	INT ids;
	ids = Converter.ConvertToVolumeInfo(bPrintVolume);
	return ids;
}

INT CMakePrintDM::MakeSpeciesDM(CESL_DataMgr * pDM, CESL_DataMgr * pBookDM, BOOL bIndex)
{
	if(pBookDM == NULL) return -1;
	if(pBookDM->GetRowCount() == 0) return 0;
	
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��¿� �������� �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	}
	pDM->FreeData();

	CString strAlias;
	if(bIndex) strAlias = _T("IBS_��KEY");
	else strAlias = _T("BP_��������KEY");
	CBO_DataConverter Converter;
	Converter.SetDivAlias(_T("IBS_��KEY"));
	Converter.SetProgressCtrl(NULL);
	Converter.SetSourceDM(pBookDM);
	Converter.SetTargetDM(pDM);
	INT ids = Converter.ConvertToSpeciesInfo();
	return ids;
}

INT CMakePrintDM::MakePrintDM()
{
	return 0;
}

INT CMakePrintDM::CheckValid()
{
	return 0;
}

VOID CMakePrintDM::SetTotalInfoAlias(CString strBookCntAlias, CString strPriceAlias, CString strDiscPriceAlias)
{
	m_cTotalInfo.SetAliasInfo(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
}


INT CMakePrintDM::CopyDM(CESL_DataMgr *pDM_From, 
						 INT nFromIdx, 
						 CESL_DataMgr *pDM_To, 
						 DWORD * pRow, 
						 CString strToAliasHeader /*= _T("")*/, 
						 CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	if (pDM_From == NULL || pDM_To == NULL) return -1;
	
	INT ids = -1;
	
	INT nRowCount = pDM_From->GetRowCount();
	if (nRowCount - 1 < nFromIdx || nFromIdx < 0) return -1;
	
	nRowCount = pDM_To->GetRowCount();
	
	if (pRow == NULL)
	{
		pRow = (DWORD*)pDM_To->InsertRow(-2);
		if (pRow == NULL) return -1;
	}	
	
	BOOL bSwitchHeader = FALSE;
	
	if (!strToAliasHeader.IsEmpty())
		bSwitchHeader = TRUE;
	
	CString strAlias;
	CString strData, strTmpData;
	
	CESL_DataMgr::reference *pRef = NULL;
	
	POSITION pos = pDM_From->RefList.GetHeadPosition();
	INT startpos = -1;
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)pDM_From->RefList.GetNext(pos);
		if (pRef == NULL) continue;
		
		strAlias = pRef->FIELD_ALIAS;
		
		// �ź� ����Ʈ�� �ִ��� Ȯ��
		if (pArrIgnoreAlias != NULL)
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
			
			ids = pDM_From->GetCellData(strAlias, nFromIdx, strData);
			if (ids < 0) continue;
			
			// ��� ����
			if (bSwitchHeader)
			{
				strAlias = strAlias.Mid(strAlias.Find(_T("_")));
				strAlias = strToAliasHeader + strAlias;
			}
			ids = pDM_To->SetCellData(pRow, pDM_To->FindColumn(strAlias), strData);
			if (ids < 0) continue;
	}
	
	return (INT)pRow;
}

INT CMakePrintDM::CopyDM(CESL_DataMgr *pDM_From, INT nFromIdx, CESL_DataMgr *pDM_To, INT nToIdx, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	if (pDM_From == NULL || pDM_To == NULL) return -1;
	
	INT ids = -1;
	
	INT nRowCount = pDM_From->GetRowCount();
	if (nRowCount - 1 < nFromIdx || nFromIdx < 0) return -1;
	
	nRowCount = pDM_To->GetRowCount();
	if (nRowCount - 1 < nToIdx ) return -1;
	
	if (nToIdx < 0)
	{
		ids = pDM_To->InsertRow(-1);
		if (ids < 0) return -1;
		nToIdx = pDM_To->GetRowCount() - 1;
	}	
	
	BOOL bSwitchHeader = FALSE;
	
	if (!strToAliasHeader.IsEmpty())
		bSwitchHeader = TRUE;
	
	CString strAlias;
	CString strData, strTmpData;
	
	CESL_DataMgr::reference *pRef = NULL;
	
	POSITION pos = pDM_From->RefList.GetHeadPosition();
	INT startpos = -1;
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)pDM_From->RefList.GetNext(pos);
		if (pRef == NULL) continue;
		
		strAlias = pRef->FIELD_ALIAS;
		
		// �ź� ����Ʈ�� �ִ��� Ȯ��
		if (pArrIgnoreAlias != NULL)
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
			
			ids = pDM_From->GetCellData(strAlias, nFromIdx, strData);
			if (ids < 0) continue;
			
			// ��� ����
			if (bSwitchHeader)
			{
				strAlias = strAlias.Mid(strAlias.Find(_T("_")));
				strAlias = strToAliasHeader + strAlias;
			}
			ids = pDM_To->SetCellData(strAlias, strData, nToIdx);
			if (ids < 0) continue;
	}
	
	return nToIdx;
}

BOOL CMakePrintDM::IsStringInArray(CStringArray *pArr, CString str)
{
	if (pArr == NULL || str.IsEmpty()) return FALSE;
	
	INT nRowCount = pArr->GetSize();
	if (nRowCount == 0) return FALSE;
	
	for (INT i = 0; i < nRowCount; i++)
	{
		if (pArr->GetAt(i) == str) return TRUE;
	}
	
	return FALSE;
}


INT CMakePrintDM::InsertObjInfo(CESL_DataMgr * pDM, 
								CESL_DataMgr * pDM_OBJ, 
								CString strKey, 
								PRINT_TYPE nType)
{
	if(strKey.IsEmpty()) return 1000;
	
	if(nType == SPECIES_PRINT)
		return InsertVolumeInfo(pDM, pDM_OBJ, strKey);
	else if(nType == VOLUME_PRINT)
		return InsertSpeciesInfo(pDM, pDM_OBJ, -1, strKey);
	else if(nType == BOOK_PRINT)
		return InsertSpeciesInfo(pDM, pDM_OBJ, -1, strKey);
	return -1;
}

INT CMakePrintDM::InsertObjInfo(CESL_DataMgr * pDM, CESL_DataMgr * pDM_OBJ, INT nCurIdx, CString strKey, PRINT_TYPE nType)
{
	if(strKey.IsEmpty()) return 1000;

	if(nType == SPECIES_PRINT)
		return InsertVolumeInfo(pDM, pDM_OBJ, nCurIdx, strKey);
	else if(nType == VOLUME_PRINT)
		return InsertSpeciesInfo(pDM, pDM_OBJ, nCurIdx, strKey);
	else if(nType == BOOK_PRINT)
		return InsertSpeciesInfo(pDM, pDM_OBJ, nCurIdx, strKey);
	return -1;
}

INT CMakePrintDM::InsertVolumeInfo(CESL_DataMgr *pDM, CESL_DataMgr *pDM_OBJ, CString strKey)
{
	CArray<INT, INT> IdxArray;	IdxArray.RemoveAll();
	
	INT nReturnVal = 0;
	
	INT ids = IsMultiVolume(pDM_OBJ, strKey, &IdxArray);
	if(ids < 0) return ids;
	
	if(IdxArray.GetSize() > 1)
	{
		INT nCnt = IdxArray.GetSize();
		for(INT i=0 ; i<nCnt ; i++)
		{
			DWORD *pRow = (DWORD*)pDM->InsertRow(-2);
			if(pRow == NULL) return -1;
			ids = CopyDM(pDM_OBJ, IdxArray.GetAt(i), pDM, pRow, _T(""), &m_IgnoreAlias);
			if(ids < 0) return ids;
		}
		nReturnVal = 1000;
	}
	return nReturnVal;
}

INT CMakePrintDM::InsertVolumeInfo(CESL_DataMgr *pDM, CESL_DataMgr *pDM_OBJ, INT nCurIdx, CString strKey)
{
	CArray<INT, INT> IdxArray;	IdxArray.RemoveAll();
	
	CString strTMpData;
	strTMpData.Format(_T("Key Value : %s"), strKey);
	FileLog(strTMpData);
	INT ids = IsMultiVolume(pDM_OBJ, strKey, &IdxArray);
	if(ids < 0) return ids;

	if(IdxArray.GetSize() > 1)
	{
		INT nCnt = IdxArray.GetSize();
		for(INT i=0 ; i<nCnt ; i++)
		{
			pDM->InsertRow(-1);
			ids = CopyDM(pDM_OBJ, IdxArray.GetAt(i), pDM, pDM->GetRowCount()-1, _T(""), &m_IgnoreAlias);
			if(ids < 0) return ids;
		}
	}
	return 0;
}

INT CMakePrintDM::InsertSpeciesInfo(CESL_DataMgr *pDM, CESL_DataMgr *pDM_OBJ, INT nCurIdx, CString strKey)
{
	return 0;
}

INT CMakePrintDM::IsMultiVolume(CESL_DataMgr *pDM, CString strKey, CArray<INT, INT> * pIdxArray)
{
	LONG nKeyValue = (LONG)_ttol(strKey.GetBuffer(0));
	INT nPivot = pDM->GetRowCount()/2;
	if(nPivot == 0) nPivot = 1;
	
	INT nIdx = FindVolume(pDM, nKeyValue, nPivot, 1, pDM->GetRowCount());

	if(nIdx < 0) return -1;
	else GetMultiVolumeIdx(pDM, nIdx, pIdxArray);
	return 0;
}

INT CMakePrintDM::AddIdxByOrdered(CArray<INT, INT> *pIdxArray, INT nNewData)
{
	INT nCnt = pIdxArray->GetSize();
	if(nCnt == 0) pIdxArray->Add(nNewData);
	else
	{
		INT nTmpData;
		for(INT i=0 ; i<nCnt ; i++)
		{
			nTmpData = pIdxArray->GetAt(i);
			if(nTmpData >= nNewData)
			{
				pIdxArray->InsertAt(i, nNewData);
				break;
			}
		}
		if(i == nCnt) pIdxArray->Add(nNewData);
	}
	return 0;
}

INT CMakePrintDM::GetMultiVolumeIdx(CESL_DataMgr * pDM, INT nIdx, CArray<INT, INT> *pIdxArray)
{
	CString strKey = pDM->GetCellData(m_strDivAlias, nIdx);
	CString strTmpData ;
	for(INT i=nIdx-1 ; i>=0 ; i--)
	{
		strTmpData = pDM->GetCellData(m_strDivAlias, i);
		if(strTmpData == strKey) AddIdxByOrdered(pIdxArray, i);
		else break;
	}
	for(i=nIdx+1 ; i<pDM->GetRowCount() ; i++)
	{
		strTmpData = pDM->GetCellData(m_strDivAlias, i);
		if(strTmpData == strKey) AddIdxByOrdered(pIdxArray, i);
		else break;
	}
	AddIdxByOrdered(pIdxArray, nIdx);
	
	return 0;
}

INT CMakePrintDM::FindVolume(CESL_DataMgr *pDM, LONG nKeyValue, INT nPivot, LONG nFirstValue, LONG nLastValue)
{	
	CString strTmpData;
	strTmpData.Format(_T("�ʱⰪ:\t%d\t\tPivot:\t%d\t\t��������:\t%d"), nFirstValue, nPivot, nLastValue);
	FileLog(strTmpData);
	if(nFirstValue > nLastValue) 
	{
		FileLog(_T("Not Found!!"));
		return -1;
	}

	if(nPivot < 1 || nPivot > pDM->GetRowCount())
		return -1;

	CString strTmpKey = pDM->GetCellData(m_strDivAlias, nPivot-1);
	if(strTmpKey.IsEmpty())
		return -1;
	
	LONG nTmpData = _ttol(strTmpKey.GetBuffer(0));
	INT nWeight;

	if(nKeyValue > nTmpData)
	{
		nWeight = (nLastValue-nPivot)/2;
		if(nWeight == 0) nWeight = 1;
		INT nNextPivot = nPivot + nWeight;
		return FindVolume(pDM, nKeyValue, nNextPivot, nPivot+1, nLastValue);
		//return FindVolume(pDM, nKeyValue, nNextPivot, nPivot, nLastValue);
	}
	else if(nKeyValue < nTmpData)
	{
		nWeight = (nPivot-nFirstValue)/2;
		if(nWeight == 0) nWeight = 1;
		INT nNextPivot = nPivot - nWeight;
		return FindVolume(pDM, nKeyValue, nNextPivot, nFirstValue, nPivot-1);
		//return FindVolume(pDM, nKeyValue, nNextPivot, nFirstValue, nPivot);
	}
	else if(nKeyValue == nTmpData)
	{
		FileLog(_T("Found!!"));
		return nPivot-1;
	}

	return -1;
}

INT CMakePrintDM::FileLog(CString str)
{
/*
	FILE * f = _tfopen(_T("binary_search.log"), _T("a+t"));
	if (!f) return -1;
	_ftprintf(f, _T("%s\n"), str);
	fclose(f);
	*/
	return 0;
}

INT CMakePrintDM::SetTotalInfo(CESL_DataMgr * pDM, CPrintTotalInfo * pTotalInfo, INT nRowIdx, CString strTitle)
{
	if(pTotalInfo == NULL) return -1;
	
	CString strTmpData;
	pDM->SetCellData(m_strTotalInfoTitle, strTitle, nRowIdx);
	
	if(!m_strTotalBookCntAlias.IsEmpty())
	{
		pTotalInfo->GetTotalBookCnt(strTmpData);
		pDM->SetCellData(m_strTotalBookCntAlias, strTmpData+_T("å"), nRowIdx);
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

BOOL CMakePrintDM::CheckTotalInfoAlias()
{
	if(m_strTotalInfoTitle.IsEmpty()		||
		m_strTotalSpeciesCntAlias.IsEmpty()	||
		m_strTotalBookCntAlias.IsEmpty()	||
		m_strTotalPriceAlias.IsEmpty()		||
		m_strTotalDiscPriceAlias.IsEmpty())
		return FALSE;
	return TRUE;
}

INT CMakePrintDM::SetUDFDMFieldData(CESL_DataMgr * pDM, BOOL bOrder)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strWorkCode, strOrderSerialNo, strTmpData, str; 
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	INT nColCnt = pDM->GetRefColumnCount();
	INT nRowCnt = pDM->GetRowCount();
	INT nRow = -1;
	m_MarcConvertMgr.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(nRow != i)
		{
			CMarc marc;
			INT ids;
			strTmpData = pDM->GetCellData(_T("BS_MARC"), i);
			ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
			if(ids < 0) return -1;
			strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
			if(ids >= 0) pDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, i);
			strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
			if(ids >= 0) pDM->SetCellData(_T("IBS_�ǻ���"), strTmpData, i);
			strTmpData.Empty();	 strTmpData = pDM->GetCellData(_T("IBS_��ǥ��"), i);
			ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
			if(ids >= 0) pDM->SetCellData(_T("UDF_ǥ������"), strTmpData, i);
			nRow = i;
		}

		for(INT j=0 ; j<nColCnt ; j++)
		{
			pDM->GetColumnRefInfo(j, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);

			if(FIELD_ALIAS == _T("UDF_��������") && !bOrder)
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
			else if(FIELD_ALIAS == _T("UDF_��������") && bOrder)
			{	
				INT nGroupNo, nOrderSerialNo;
				strAcqYear = pDM->GetCellData(_T("BP_���Գ⵵"), i);
				strWorkCode = pDM->GetCellData(_T("IBS_��������"), i);
				strGroupNo = pDM->GetCellData(_T("BP_�׷��ȣ"), i);
				nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
				strOrderSerialNo = pDM->GetCellData(_T("BP_�����Ϸù�ȣ"), i);
				nOrderSerialNo = _ttoi(strOrderSerialNo.GetBuffer(0));
				str.Format(_T("%s-%s-%04d-%05d"), strAcqYear, strWorkCode, nGroupNo, nOrderSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo;
				pDM->SetCellData(FIELD_ALIAS, str, i);
				str.Format(_T("%04d-%05d"), nGroupNo, nOrderSerialNo);
				pDM->SetCellData(_T("UDF_�����������"), str, i);
			}
			else if(FIELD_ALIAS == _T("UDF_�Է±��м���"))
			{
				strTmpData = pDM->GetCellData(_T("BP_�Է±���"), i);
				if(strTmpData == _T("0")) str = _T("ȭ���Է�");
				else if(strTmpData == _T("1")) str = _T("��Ȱ�����");
				else if(strTmpData == _T("2")) str = _T("����ڷ����");
				else if(strTmpData == _T("3")) str = _T("���Ϲ���");
				pDM->SetCellData(FIELD_ALIAS, str, i);
			}
			else if(FIELD_ALIAS == _T("UDF_�������м���"))
			{
				strTmpData = pDM->GetCellData(_T("IBS_�з�ǥ����"), i);
				if(strTmpData.IsEmpty()) str = _T("");
				else if(strTmpData == _T("1"))
				{
					strTmpData = pDM->GetCellData(_T("BP_��������"), i);
					m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC��������ȣ����"), strTmpData, str);
				}
				else if(strTmpData == _T("2"))
				{
					strTmpData = pDM->GetCellData(_T("BP_��������"), i);
					m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC��������ȣ����"), strTmpData, str);
				}
				pDM->SetCellData(FIELD_ALIAS, str, i);
			}
			else if(FIELD_ALIAS == _T("UDF_����ó��ȣ����"))
			{
				strTmpData = pDM->GetCellData(_T("BP_����ó��ȣ"), i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����ó����"), strTmpData, str);
				pDM->SetCellData(FIELD_ALIAS, str, i);
			}
			else if(FIELD_ALIAS == _T("UDF_���ΰ�"))
			{
				CString strTmpData = pDM->GetCellData(_T("UDF_����"), i);
				DOUBLE dPrice, dDisc_Rate, dDisc_Price;
				strTmpData.Replace(_T(","), _T(""));
				dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("BP_������"), i);
				dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
				strTmpData.Format(_T("%.0f"), dDisc_Price);
				pDM->SetCellData(_T("UDF_���ΰ�"), strTmpData, i);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_��ȯ���ΰ���"), strTmpData, i);
			}
			else if(FIELD_ALIAS == _T("UDF_����"))
			{
				CString strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_��ȯ����"), strTmpData, i);
			}
			else if(FIELD_ALIAS == _T("BP_���ְ���"))
			{
				CString strTmpData;
				INT nBookCnt = 0;
				DOUBLE dPrice, dDisc_Rate, dDisc_Price;
				strTmpData = pDM->GetCellData(_T("BP_������"), i);
				dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("BP_���ְ���"), i);
				dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
				strTmpData.Format(_T("%.0f"), dDisc_Price);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_��ȯ����������"), strTmpData, i);
			}
			else if(FIELD_ALIAS == _T("BP_����"))
			{
				CString strTmpData;
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_��ȯ������"), strTmpData, i);
			}
			else if(FIELD_ALIAS == _T("UDF_������"))
			{
				CString strTmpData;
				INT nBookCnt = 0;
				DOUBLE dPrice, dDisc_Rate, dDisc_Price;
				strTmpData = pDM->GetCellData(_T("BP_������"), i);
				dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("UDF_������"), i);
				dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
				strTmpData.Format(_T("%.0f"), dDisc_Price);
				pDM->SetCellData(_T("UDF_����������"), strTmpData, i);				
			}
			else if(FIELD_ALIAS == _T("UDF_�ܰ�"))
			{
				CString strTmpData;
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				strTmpData.Replace(_T(","), _T(""));
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
		}
	}	
	return 0;
}


VOID CMakePrintDM::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) return;
	
	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(m_pParentMgr))
		{
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//���α׷��� ��Ʈ�� �ʱ�ȭ
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->SetMessage(_T(""));
}

VOID CMakePrintDM::HideDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
	}
}

INT CMakePrintDM::CopySortItem(CStringArray *pSourceArr, CStringArray *pTargetArr)
{
	if(pSourceArr == NULL || pTargetArr == NULL) return -1;
	if(pSourceArr->GetSize() == 0) return 0;
	
	pTargetArr->RemoveAll();
	for(INT i=0 ; i<pSourceArr->GetSize() ; i++)
	{
		pTargetArr->Add(pSourceArr->GetAt(i));
	}
	return 0;
}

INT CMakePrintDM::PrintDataSortByBasicInfo(CESL_DataMgr * pDM)
{
	return 0;
}

INT CMakePrintDM::PrintDataSortByUserSelect(CESL_DataMgr * pDM)
{
	return 0;
}

INT CMakePrintDM::SetDataSerialNo(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	if(pDM->GetRowCount() == 0) return 0;
	
	CString strTmpData;
	INT nSerialNo = 0;
	for(INT i=0 ; i<pDM->GetRowCount() ; i++)
	{
		strTmpData.Format(_T("%d"), ++nSerialNo);
		pDM->SetCellData(_T("UDF_�Ϸù�ȣ"), strTmpData, i);
	}
	return 0;
}

INT CMakePrintDM::CopySortConditionMap(CMapStringToString *pSource, CMapStringToString *pTarget)
{
	if(pSource == NULL || pTarget == NULL) return -1;
	pTarget->RemoveAll();
	CString strKey, strValue;
	POSITION pos = pSource->GetStartPosition();
	while( pos != NULL )
	{
		CString string;
		pSource->GetNextAssoc(pos, strKey, strValue);
		pTarget->SetAt(strKey, strValue);
	}
	return 0;	
}

BOOL CMakePrintDM::CheckPrintData(CString strValue, CString strExtraValue/*=_T("")*/)
{
	CString strTmpData;
	if(m_pArrDivValeu && m_pArrDivValeu->GetCount() > 0)
	{
		if(strExtraValue.IsEmpty())
		{
			if(!m_pArrDivValeu->Lookup(strValue, strTmpData))
				return FALSE;
		}
		else
		{
			if(!m_pArrDivValeu->Lookup(strValue+_T("-")+strExtraValue, strTmpData))
				return FALSE;
		}
	}
	return TRUE;
}

INT CMakePrintDM::ApplyGridSelectData(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	if(pDM->GetRowCount() == 0) return 0;
	if(m_strDivAlias.IsEmpty()) return 0;

	if(m_pArrDivValeu == NULL || m_pArrDivValeu->GetCount() == 0) return 0;

	CString strTmpData;
	for(INT i=pDM->GetRowCount()-1 ; i>=0 ; i--)
	{
		if(!CheckPrintData(pDM->GetCellData(m_strDivAlias, i)))
			pDM->DeleteRow(i);
	}
	return 0;
}
