// BOFormApi.cpp: implementation of the CBOFormApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BOFormApi.h"

//#include _T("..\K2UP_Common\ISBNChecker.h")
#include "..\BO_SPECIES_API\ISBNChecker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBOFormApi::CBOFormApi(CESL_Mgr *pParentMgr  /*= NULL*/)
{
	m_pParnetMgr = pParentMgr;
	m_nAppMode = MODE_INSERT;

	if (m_pParnetMgr != NULL)
	{
		LoadRegCode();
	}
}

CBOFormApi::~CBOFormApi()
{

}

//////////////////////////////////////////////////////////////////////
// CBOFormApi Operations
INT CBOFormApi::LoadRegCode()
{
	if (m_pParnetMgr == NULL) return -1;

	INT nCodeCount = m_pParnetMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("��ϱ���"));
	INT nStartRow = 0;
	CString strArrCode[6];
	for (INT i = 0; i < nCodeCount; i++) {
		nStartRow = m_pParnetMgr->m_pInfo->pCodeMgr->GetCode(_T("��ϱ���"), strArrCode, 6, nStartRow);

		m_arrRegCode.Add(strArrCode[2]);
		
		nStartRow++;
	}
	return 0;
}

INT CBOFormApi::FetchRegCode(CString strRegCodeField, CArray<CRegCode, CRegCode> *pArrRegCode)
{
	if(pArrRegCode == NULL) return -1;
	if (strRegCodeField.IsEmpty()) return -1;

	strRegCodeField.TrimLeft();
	strRegCodeField.TrimRight();

	// '/'�� ������.
	CMap<CString, LPCTSTR, INT, INT> mapRegCode;
	CString strRegCode;
	CString strCount;
	CString strData;
	INT nCount = 0;
	INT nTempCount = 0;

	while (TRUE)
	{
		INT nFind = strRegCodeField.Find('/');
		if (nFind < 0)
		{
			nFind = strRegCodeField.GetLength() - 1;
			strData  = strRegCodeField;
		}
		else
		{		
			strData = strRegCodeField.Mid(0, nFind);
		}
		
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
		INT nRegCodeLength;
		nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
// 		if (strData.GetLength() < 2) return -1;
// 		strRegCode = strData.Left(2);
		if (strData.GetLength() < nRegCodeLength) return -1;
		strRegCode = strData.Left(nRegCodeLength);
		//===================================================================

		// ��ϵ� ��ϱ������� üũ
		if (!IsValidRegCode(strRegCode)) return -1;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		if (strData.GetLength() == 2)
// 			strCount = _T("1");
// 		else
// 			strCount = strData.Mid(2);
		if (strData.GetLength() == nRegCodeLength)
			strCount = _T("1");
		else
			strCount = strData.Mid(nRegCodeLength);
		//===================================================================

		INT nLength = strCount.GetLength();
		for (INT i = 0; i < nLength; i++)
		{
			if (!_istdigit(strCount.GetAt(i))) return -1;
		}

		nCount = _ttoi(strCount);
		if (nCount <= 0) return -1;

		// Insert Data to Map
		if (mapRegCode.Lookup(strRegCode, nTempCount))
		{
			mapRegCode.SetAt(strRegCode, nTempCount + nCount);
		}
		else
		{
			mapRegCode.SetAt(strRegCode, nCount);
		}
		
		strRegCodeField = strRegCodeField.Mid(nFind + 1);
		if (strRegCodeField.IsEmpty()) break;
	}
	
	CRegCode regCode;
	POSITION pos = mapRegCode.GetStartPosition();
	while (pos)
	{
		regCode.Init();
		mapRegCode.GetNextAssoc(pos, regCode.m_strRegCode, regCode.m_nCount);

		pArrRegCode->Add(regCode);
	}

	return 0;
}

BOOL CBOFormApi::IsValidRegCode(CString strRegCode)
{
	if (m_pParnetMgr == NULL) return FALSE;
	if (strRegCode.IsEmpty()) return FALSE;

	return IsStringInArray(&m_arrRegCode, strRegCode);
}

BOOL CBOFormApi::IsRegCodeModify(CArray<CRegCode, CRegCode> *pArrRegCode, CESL_DataMgr *pDM_Book, CString strVol)
{
	if (pArrRegCode == NULL || pDM_Book == NULL) return FALSE;

	INT ids = -1;
	CArray<CRegCode, CRegCode> arrExistRegCode;
	ids = GetRegCodeInfo(pDM_Book, &arrExistRegCode, strVol);

	INT idx = 0;
	INT nBookCount = 0;
	INT nRegCodeCount = 0;
	CRegCode regCode;

	// ���� ��ϱ��п� �ִµ� ���ο� ��ϱ��п� ������ TRUE ����
	INT nCount = arrExistRegCode.GetSize();
	for (idx = 0 ; idx < nCount; idx++)
	{
		regCode = arrExistRegCode.GetAt(idx);

		if (!IsInRegCode(pArrRegCode, regCode.m_strRegCode)) return TRUE;
	}

	// ���ο� ��ϱ��� üũ
	nCount = pArrRegCode->GetSize();
	for (idx = 0; idx < nCount; idx++)
	{
		regCode = pArrRegCode->GetAt(idx);
		
		// ���� ��ϱ��п� ������ ������ ���Ѵ�.
		if (IsInRegCode(&arrExistRegCode, regCode.m_strRegCode))
		{
			nBookCount = GetBookCount(pDM_Book, regCode.m_strRegCode, strVol);
			if (regCode.m_nCount < nBookCount)
				return TRUE;
		}
		// ���� ��ϱ��п� ������ FALSE return
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}

INT CBOFormApi::MakeBookFromRegCode(CArray<CRegCode, CRegCode> *pArrRegCode, CESL_DataMgr *pDM_Book, CString strVol)
{
	if (pArrRegCode == NULL || pDM_Book == NULL) return -1;
	
	INT ids = -1;
	INT nDMRowCount = pDM_Book->GetRowCount();
	if (nDMRowCount == 0) return -1;

	INT nRegCodeCount = pArrRegCode->GetSize();
	if (nRegCodeCount == 0) return -1;

	CString strRegCode;
	CString strQueryFlag;

	// ��ϱ��а� ������ �°� å ����
	CRegCode regCode;
	INT nBookCount;
	for (INT idx = 0; idx < nRegCodeCount; idx++)
	{
		regCode = pArrRegCode->GetAt(idx);
		nBookCount = GetBookCount(pDM_Book, regCode.m_strRegCode, strVol);

		// ����
		if (regCode.m_nCount < nBookCount)
		{
			RemoveBook(pDM_Book, regCode.m_strRegCode, strVol, nBookCount - regCode.m_nCount);
		}
		// �߰�
		else if (regCode.m_nCount > nBookCount)
		{
			AddBook(pDM_Book, regCode.m_strRegCode, strVol, regCode.m_nCount - nBookCount);
		}
	}

	// ������ ����
	nDMRowCount = pDM_Book->GetRowCount();
	for (idx = nDMRowCount - 1; idx >= 0; idx--)
	{
		// å�� ���� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx, strVol)) continue;

		// ������ȣ�� c1�̸� �����Ѵ�. c1�� ���ش�.
		if (pDM_Book->GetCellData(_T("BB_������ȣ"), idx) == _T("c1"))
			pDM_Book->SetCellData(_T("BB_������ȣ"), _T(""), idx);

		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		// RegCode �� ���Ѵ�. ���� regCode�� �����Ѵ�.
		strRegCode = pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), idx);
		if (!IsInRegCode(pArrRegCode, strRegCode))
			DeleteBookDMRow(pDM_Book, idx, strQueryFlag);
	}

	return 0;
}

BOOL CBOFormApi::IsInRegCode(CArray<CRegCode, CRegCode> *pArrRegCode, CString strRegCode)
{
	if (pArrRegCode == NULL || strRegCode.IsEmpty()) return FALSE;

	INT nCount = pArrRegCode->GetSize();
	for (INT i = 0; i < nCount; i++)
	{
		if (pArrRegCode->GetAt(i).m_strRegCode == strRegCode)
			return TRUE;
	}

	return FALSE;
}

INT CBOFormApi::DeleteVolume(CESL_DataMgr *pDM_Book, CString strVol)
{
	if (pDM_Book == NULL || strVol.IsEmpty()) return -1;
	
	INT nRowCount = pDM_Book->GetRowCount();

	for (INT idx = nRowCount - 1; idx >= 0; idx--)
	{
		if (pDM_Book->GetCellData(_T("BB_�����"), idx) == strVol)
		{
			DeleteBookDMRow(pDM_Book, idx);
		}
	}

	return 0;
}

INT CBOFormApi::CheckDeleteVolume(CESL_DataMgr *pDM_Book, CString strVol)
{
	if (pDM_Book == NULL) return -1;

	CString strQuery;
	CString strResult;

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (pDM_Book->GetCellData(_T("BB_�����"), idx) == strVol)
		{
			if (CheckDeleteBook(pDM_Book, idx) < 0)
				return -1;
		}
	}

	return 0;
}

INT CBOFormApi::CheckDeleteBook(CESL_DataMgr *pDM_Book, INT nRow)
{
	if (pDM_Book == NULL) return -1;

	CString strQuery;
	CString strResult;
	CString strRecKey = pDM_Book->GetCellData(_T("BB_åKEY"), nRow);
	if (!strRecKey.IsEmpty())
	{
		strQuery.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL BB , LH_STORAGE_V01 CL ")
						_T("WHERE BB.REC_KEY = %s  AND ")
						_T("BB.REC_KEY = CL.BOOK_KEY AND CL.STATUS IN ( '0' , '2' , '3' )")
						, strRecKey);
		if (pDM_Book->GetOneValueQuery(strQuery, strResult) >= 0)
		{
			if (_ttoi(strResult) > 0)
			{
				return -1;
			}
		}
	}
	return 0;
}

CString CBOFormApi::GetBookKey(CESL_DataMgr *pDM_Book, INT nIdx)
{
	if (pDM_Book == NULL) return _T("");

	CString strKey = pDM_Book->GetCellData(_T("BB_åKEY"), nIdx);
	if (strKey.IsEmpty())
		strKey = pDM_Book->GetCellData(_T("BB_UNIQUE_KEY"), nIdx);

	return strKey;
}

INT CBOFormApi::DeleteBookDMRow(CESL_DataMgr *pDM_Book, INT idx, CString strQueryFlag /*=_T("")*/)
{
	if (pDM_Book == NULL) return -1;

	if (strQueryFlag.IsEmpty())
	{
		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
	}

	// å Ű�� �����´�.
	CString strKey = GetBookKey(pDM_Book, idx);
	
	if (strQueryFlag == _T("D")) 
	{
		
	}
	else if (strQueryFlag == _T("I"))
	{
		pDM_Book->DeleteRow(idx);
	}
	else if (strQueryFlag == _T("U"))
	{
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}
	else
	{
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}

	// Ű�� ������ ����
	if (strKey.IsEmpty()) return 0;

	// Ű�� ���� �ִ� �η��� �����.
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT i = nRowCount - 1; i >= 0; i--)
	{
		// �η��� �ƴϸ� �Ѿ��.
		if (!IsAppendix(pDM_Book, i)) continue;

		// Ű�� �ٸ��� �Ѿ��.
		if (pDM_Book->GetCellData(_T("BB_�θ�åKEY"), i) != strKey) continue;

		// ���� �÷���
		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), i);

		if (strQueryFlag == _T("D"))
		{
		}
		else if (strQueryFlag == _T("I"))
		{
			pDM_Book->DeleteRow(i);
		}
		else if (strQueryFlag == _T("U"))
		{
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("D"), i);
		}
		else
		{
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("D"), i);
		}
	}

	return 0;
}

INT CBOFormApi::GetVolumeCount(CESL_DataMgr *pDM_Book)
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

		ids = pDM_Book->GetCellData(_T("BB_�����"), idx, strVol);
		
		mapVolume.SetAt(strVol, 1);
	}

	return mapVolume.GetCount();
}

INT CBOFormApi::GetBookCount(CESL_DataMgr *pDM_Book, CString strRegCode /*= _T("")*/, LPCTSTR lpszVol /*=NULL*/)
{
	if (pDM_Book == NULL) return -1;

	INT nReturnCnt = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// å�� ���� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx, lpszVol)) continue;

		// ��ϱ��� ��
		if (!strRegCode.IsEmpty())
			if (pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), idx) != strRegCode) continue;
		
		nReturnCnt++;
	}

	return nReturnCnt;
}

INT CBOFormApi::GetRegCodeInfo(CESL_DataMgr *pDM_Book, CArray<CRegCode, CRegCode> *pArrRegCode, LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL || pArrRegCode == NULL) return -1;

	CMap<CString, LPCTSTR, INT, INT> mapRegCode;

	INT ids = -1;
	CString strRegCode;
	INT nRegCount = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// å�� ���� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx, lpszVol)) continue;

		ids = pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), idx, strRegCode);
		if (ids < 0) continue;

		if (strRegCode.IsEmpty()) continue;
		if (!IsValidRegCode(strRegCode)) continue;

		if (mapRegCode.Lookup(strRegCode, nRegCount))
			mapRegCode.SetAt(strRegCode, ++nRegCount);
		else
			mapRegCode.SetAt(strRegCode, 1);
	}

	CRegCode regCode;
	POSITION pos = mapRegCode.GetStartPosition();
	while (pos)
	{
		regCode.Init();
		mapRegCode.GetNextAssoc(pos, regCode.m_strRegCode, regCode.m_nCount);
		pArrRegCode->Add(regCode);
	}

	return 0;
}

INT CBOFormApi::AddBook(CESL_DataMgr *pDM_Book, CString strRegCode, CString strVol, INT nCount /*= 1*/)
{
	if (pDM_Book == NULL) return -1;

	INT ids = -1;

	// ǥ�� å�� ã��
	INT nStandardRowIdx = 0;

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// å�� ���� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx, strVol)) continue;

		nStandardRowIdx = idx;
		break;
	}
	
	CString strQueryFlag;
	CString strDupCode;
	INT nNewRow = -1;
	for (idx = 0; idx < nCount; idx++)
	{
		// �����ؼ� �ο� �����Ѵ�.
		nNewRow = CopyDM(pDM_Book, nStandardRowIdx, pDM_Book, -1);
		if (nNewRow < 0) continue;

		// ��ϱ��� ����
		pDM_Book->SetCellData(_T("BB_��ϱ���_�ڵ�"), strRegCode, nNewRow);

		// ��Ϲ�ȣ ������ ����
		pDM_Book->SetCellData(_T("BB_��Ϲ�ȣ"), _T(""), nNewRow);

		// �η� �÷��� �߰�
		pDM_Book->SetCellData(_T("BB_å�η��÷���"), _T("B"), nNewRow);

		// ���� �÷��� ����
		strQueryFlag = GetQueryFlag(MODE_INSERT);
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, nNewRow);

		// ������ȣ �ֱ�
		INT nDupNumber = GetLastDupCodeNumber(pDM_Book, strVol);
		if (nDupNumber < 0) continue;
		strDupCode.Format(_T("c%d"), ++nDupNumber);
		pDM_Book->SetCellData(_T("BB_������ȣ"), strDupCode, nNewRow);		
	}

	return 0;
}

INT CBOFormApi::RemoveBook(CESL_DataMgr *pDM_Book, CString strRegCode, CString strVol, INT nCount /*= 1*/)
{
	if (pDM_Book == NULL) return -1;

	CString strQueryFlag;
	INT nDeletedCount = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = nRowCount - 1; idx >= 0; idx--)
	{
		// å�� ���� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx, strVol)) continue;

		// ��ϱ��� ��
		if (pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), idx) != strRegCode) continue;

		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		
		if (DeleteBookDMRow(pDM_Book, idx, strQueryFlag) < 0) continue;

		if (nCount == ++nDeletedCount) break;
	}

	return nDeletedCount;
}

INT CBOFormApi::CopyDM(CESL_DataMgr *pDM_From, INT nFromIdx, CESL_DataMgr *pDM_To, INT nToIdx, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
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
		// Default Ignore List
		if (IsStringInArray(&m_arrDefaultIgnoreAlias, strAlias)) continue;
		// User Ignore List
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
/*		//å�� ���ڿ��� 245$d�� ��
		if(strAlias == _T("BB_������"))
		{
			startpos = strData.Find(_T(";"), 0);
			if(startpos > 0) strData = strData.Mid(0, strData.Find(_T(";")));
		}
		//��ǥå�� ���ο� �ݿ��ÿ��� 245$d�� ��ġ��
		else if(strAlias == _T("IBS_������"))
		{
			pDM_To->GetCellData(_T("IBS_������"), nToIdx, strTmpData);
			startpos = strTmpData.Find(_T(";"), 0);
			if(startpos > 0) strData = strData + strTmpData.Mid(strTmpData.Find(_T(";")));
		}
*/		ids = pDM_To->SetCellData(strAlias, strData, nToIdx);
		if (ids < 0) continue;
	}

	return nToIdx;
}

INT CBOFormApi::MakeDMFromCM(CESL_ControlMgr *pCM, CESL_DataMgr *pDM, INT nIdx /*= -1*/, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	if (pCM == NULL || pDM == NULL) return -1;
	
	INT i = 0;
	INT ids = -1;

	if (nIdx == -1)
	{
		ids = pDM->InsertRow(-1);
		if (ids < 0) return -1;
		nIdx = pDM->GetRowCount() - 1;
	}

	BOOL bSwitchHeader = FALSE;
	if (!strToAliasHeader.IsEmpty())
	{
		bSwitchHeader = TRUE;
	}
	
	CString strAlias;
	CString strData, strTmpData;
	CESL_Control_Element *pCE = NULL;
	INT startpos = -1;
	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		
		strAlias = pCE->GetAlias();
		if (strAlias.IsEmpty()) continue;

		// �ź� ����Ʈ�� �ִ��� Ȯ��
		// Default Ignore List
		if (IsStringInArray(&m_arrDefaultIgnoreAlias, strAlias)) continue;
		// User Ignore List
		if (pArrIgnoreAlias != NULL)
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;

		ids = pCM->GetControlMgrData(strAlias, strData);
		if (ids < 0) continue;

		if (bSwitchHeader)
		{
			strAlias = strAlias.Mid(strAlias.Find(_T("_")));
			strAlias = strToAliasHeader + strAlias;
		}
/*		//å�� ���ڿ��� 245$d�� ��
		if(strAlias == _T("BB_������") && bSwitchHeader)
		{
			startpos = strData.Find(_T(";"), 0);
			if(startpos > 0) strData = strData.Mid(0, strData.Find(_T(";")));
		}
		//��ǥå�� ���ο� �ݿ��ÿ��� 245$d�� ��ġ��
		else if(strAlias == _T("IBS_������") && bSwitchHeader)
		{
			pDM->GetCellData(_T("IBS_������"), nIdx, strTmpData);
			startpos = strTmpData.Find(_T(";"), 0);
			if(startpos > 0) strData = strData + strTmpData.Mid(strTmpData.Find(_T(";")));
		}				
		else if(strAlias == _T("IBS_������") && !bSwitchHeader)
		{
			//CM�� �Է��� ������ ������ ���Ҿ �����Ͽ� ����DM�� �����Ѵ�.
			DeleteUnlessWord
		}
		*/
		//���� �����ϰ��� �ϴ� DM cell�� �̹� ���� �ִٸ� �������� ����
//		if(pDM->GetCellData(strAlias, nIdx).IsEmpty())
//		{
			ids = pDM->SetCellData(strAlias, strData, nIdx);
			if (ids < 0) continue;
//		}
	}
	return 0;
}

BOOL CBOFormApi::IsStringInArray(CStringArray *pArr, CString str)
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

CString CBOFormApi::GetQueryFlag(INT nWorkMode, CString strQueryFlag /*= _T("")*/)
{
	if (strQueryFlag.IsEmpty())
	{
		if (nWorkMode == MODE_INSERT)
		{
			return _T("I");
		}
		else if (nWorkMode == MODE_UPDATE)
		{
			if (m_nAppMode == MODE_INSERT)
				return _T("I");
			else if (m_nAppMode == MODE_UPDATE)
				return _T("U");
		}
	}
	else
	{
		if (strQueryFlag == _T("I"))
			return _T("I");
		else if (strQueryFlag == _T("U"))
			return _T("U");
	}
	
	return _T("");
}

INT CBOFormApi::MakeAppendixDM(CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Appendix, CString strVol /*= _T("")*/, CString strParentKey /*=_T("")*/)
{
	if (pDM_Book == NULL || pDM_Appendix == NULL) return -1;
	
	pDM_Appendix->FreeData();

	CString strIdx;
	INT nAppendixRowCount = -1;
	INT nColCount = pDM_Book->RefList.GetCount();
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// �η��� �ƴϸ�
		if (!IsAppendix(pDM_Book, idx, strVol)) continue;
		
		// �θ�Ű ��
		if (!strParentKey.IsEmpty())
			if (pDM_Book->GetCellData(_T("BB_�θ�åKEY"), idx) != strParentKey) continue;
		
		// �η� DM�� �ִ´�.
		if (pDM_Appendix->InsertRow(-1) < 0) continue;
		
		nAppendixRowCount = pDM_Appendix->GetRowCount();
		CopyDM(pDM_Book, idx, pDM_Appendix, nAppendixRowCount - 1);

		// å rowidx�� appendix�� UDF �ʵ忡 �ִ´�.
		strIdx.Format(_T("%d"), idx);
		pDM_Appendix->SetCellData(_T("BB_BOOK_IDX"), strIdx, nAppendixRowCount - 1);
	}

	return 0;
}

INT CBOFormApi::GetBookPriceSum(CESL_DataMgr *pDM_Book, INT &nSum)
{
	if (pDM_Book == NULL) return -1;
	
	nSum = 0;
	INT ids = -1;
	INT nResult = 0;
	CString strTemp;


	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;

	for (INT idx = 0 ; idx < nRowCount; idx++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx)) continue;
		
		// ���� ��������
		ids = pDM_Book->GetCellData(_T("BB_����"), idx, strTemp);
		if (ids < 0 || strTemp.IsEmpty()) 
		{
			nResult ++;
			continue;
		}

		// ���� ���ϱ�
		nSum += _ttoi(strTemp);		
	}

	return nResult;
}

INT CBOFormApi::ApplySpeciesPriceToBookPrice(CESL_DataMgr *pDM_Book, INT nPrice)
{
	if (pDM_Book == NULL) return -1;

	INT nBookCount = GetBookCount(pDM_Book);
	if (nBookCount <= 0) return -1;

	if (nPrice < 0) return -1;

	INT nBookPrice = 0;
	INT nLastBookPrice = 0;

	if (nPrice >= nBookCount) 
	{
		nBookPrice = nPrice / nBookCount;
		nLastBookPrice = nPrice - (nBookPrice * nBookCount);
		if (nLastBookPrice == 0)
			nLastBookPrice = nBookPrice;
		else
			nLastBookPrice = nLastBookPrice + nBookPrice;
	}

	CString strBookPrice;
	CString strLastBookPrice;

	strBookPrice.Format(_T("%d"), nBookPrice);
	strLastBookPrice.Format(_T("%d"), nLastBookPrice);

	if (nLastBookPrice < 0) return -1;

	// å�� ������ ���� ��Ų��.
	INT n = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx)) continue;

		// ������ å�̸� ������ ������ �ִ´�.
		n++;
		
		if (n == nBookCount)
			pDM_Book->SetCellData(_T("BB_����"), strLastBookPrice, idx);
		else
			pDM_Book->SetCellData(_T("BB_����"), strBookPrice, idx);
		// ���ݻ����� �����Ͽ����Ƿ� DB_FLAG�� ���� _T("U")�� �����Ѵ�. �̶� �̹� ������ ��(U/I/D)���� �����Ǿ� �ִٸ� 
		// �������� �ʴ´�.
		if(pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx).IsEmpty())
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("U"), idx);
	}
	

	return 0;
}

INT CBOFormApi::ApplyCompleteWorks(CESL_DataMgr *pDM_Book, CString &strVol)
{
	if (pDM_Book == NULL) return -1;

	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;

	INT n = 1;
	CString strTemp;

	strTemp.Format(_T("%d"), n++);
	strVol = strTemp;
	
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx)) continue;

		// ���� ����
		pDM_Book->SetCellData(_T("BB_�����"), strTemp, idx);
		strTemp.Format(_T("%d"), n++);

		// ������ȣ �ʱ�ȭ
		pDM_Book->SetCellData(_T("BB_������ȣ"), _T(""), idx);
	}

	return 0;
}

CString CBOFormApi::ConvertWORKING_STATUSToCode(CString strDesc)
{
	CString result = _T("");
	
	if (strDesc == _T("���Դ���ڷ�")) result = _T("BOA111N");
	if (strDesc == _T("�˼�����ڷ�")) result = _T("BOA113N");
	if (strDesc == _T("��������ڷ�")) result = _T("BOA131N");
	if (strDesc == _T("������ڷ�")) result = _T("BOC113N");
    if (strDesc == _T("�˼��Ϸ��ڷ�")) result = _T("BOA141N");

	return result;
}

VOID CBOFormApi::InitDefaultAlias()
{
	m_arrDefualtValueAlias.RemoveAll();
}

INT CBOFormApi::AddDefaultAlias(LPCTSTR lpszAlias)
{
	return m_arrDefualtValueAlias.Add(lpszAlias);
}

INT CBOFormApi::SaveDefaultValue(CESL_ControlMgr *pCM, LPCTSTR lpszFileName)
{
	if (pCM == NULL) return -1;
	
	CStdioFile file;
	if (!file.Open(lpszFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	INT ids = -1;
	CString strAlias;
	CString strValue;
	CString strLine;
	INT nRows = m_arrDefualtValueAlias.GetSize();
	for (INT idx = 0; idx < nRows; idx++)
	{
		strLine.Empty();
		strAlias = m_arrDefualtValueAlias.GetAt(idx);
		ids = pCM->GetControlMgrData(strAlias, strValue);
		if (ids < 0) continue;
		
		strLine.Format(_T("%s\t=\t%s\r\n"), strAlias, strValue);

		file.WriteString(strLine);
	}

	file.Close();
	
	return 0;
}

INT CBOFormApi::LoadDefaultValue(CESL_ControlMgr *pCM, LPCTSTR lpszFileName)
{
	if (pCM == NULL) return -1;

	CStdioFile file;
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary))
		return -1;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	INT ids = -1;

	CString strLine;
	CString strAlias;
	CString strValue;

	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		
		strAlias.Empty();
		strValue.Empty();

		if (strLine.Left(1) == _T("#") || strLine.IsEmpty()) continue;
		
		ids = _stscanf(strLine, _T("%s\t=\t%[^\n]"), strAlias.GetBuffer(256), strValue.GetBuffer(256));
		
		if (ids == _TEOF) continue;

		strAlias.ReleaseBuffer();
		strValue.ReleaseBuffer();

		ids = pCM->SetControlMgrData(strAlias, strValue);
	}

	file.Close();

	return 0;
}

INT CBOFormApi::CheckISBN(CString strISBNFrom, CString &strISBNTo)
{
	if (strISBNFrom.IsEmpty()) return -1;

	strISBNTo.Empty();

	CISBNChecker isbnChecker;
	INT ids = -1;

	CString strSet;

	// () ����..
	INT nFind = strISBNFrom.Find('(');
	if (nFind >= 0)
	{
		strSet = strISBNFrom.Mid(nFind);
		strISBNFrom = strISBNFrom.Mid(0, nFind);
	}
	if (strISBNFrom.IsEmpty()) return -1;

	// '-' ����
	CString strTemp = strISBNFrom;
	strISBNFrom.Empty();
	isbnChecker.RemoveDashFromISBN(strTemp.GetBuffer(0), strISBNFrom.GetBuffer(MAX_PATH));
	strISBNFrom.ReleaseBuffer();

	if (strISBNFrom.IsEmpty()) return -1;
	

	// �ܱ� ISBN�� ����
	if (strISBNFrom.Left(2) != _T("89")) 
	{
		MessageBox(m_pParnetMgr->GetSafeHwnd(), _T("89�� �����ϴ� ��ȣ�� �Է��Ͻʽÿ�!"), _T("ISBN üũ"), MB_OK | MB_ICONWARNING);
		return -1;
	}

	// ���̰� 10, 9�� ����
	INT nLength = strISBNFrom.GetLength();

	INT iResult = -1;
	if (nLength == 10)
	{
		ids = isbnChecker.ConvertISBN(strISBNFrom.GetBuffer(0), strISBNTo.GetBuffer(MAX_PATH), &iResult, FALSE);
		if (ids < 0) return -1;

		if (iResult == 0)
		{
			MessageBox(m_pParnetMgr->GetSafeHwnd(), _T("�ùٸ� ISBN�� �ԷµǾ����ϴ�."), _T("ISBN üũ"), MB_OK | MB_ICONWARNING);
			strISBNTo += strSet;
			return 0;
		}
		else if (iResult == 1)
		{
		}
		else if (iResult == -2)
		{
			ids = MessageBox(m_pParnetMgr->GetSafeHwnd(), _T("ISBN�� �߸��Ǿ����ϴ�. �����Ͻðڽ��ϱ�?"), _T("ISBN üũ"), MB_YESNO | MB_ICONQUESTION);
			if (ids == IDYES)
			{
				strISBNTo += strSet;
			}
			else
				return -1;
		}
	}
	else if (nLength == 9)
	{
		CString strOutISBN;
		CString strCheckDigit;
		isbnChecker.ReturnISBNCheckDigit(strISBNFrom.GetBuffer(0), strOutISBN.GetBuffer(MAX_PATH), strCheckDigit.GetBuffer(10), FALSE);
		strOutISBN.ReleaseBuffer();
		strCheckDigit.ReleaseBuffer();

		strISBNTo.Format(_T("89-%s-%s%s"), strOutISBN, strCheckDigit, strSet);

		CString strMsg;
		strMsg.Format(_T("(%s)�� �����Ǿ����ϴ�."), strISBNTo);
		MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN üũ"), MB_OK | MB_ICONINFORMATION);
		return 0;
	}
	else
	{
		MessageBox(m_pParnetMgr->GetSafeHwnd(), _T("�߸��� �ڸ����� �ԷµǾ����ϴ�"), _T("ISBN üũ"), MB_OK | MB_ICONWARNING);
		return -1;
	}

	return 0;
}

INT CBOFormApi::GetLastDupCodeNumber(CESL_DataMgr *pDM_Book, CString strVol)
{
	if (pDM_Book == NULL) return -1;

	INT nRowCount = pDM_Book->GetRowCount();
	
	INT nDupNumber = 0;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		CString strTemp;
		INT nNumber = 0;
		TCHAR ch = ' ';

		// å�� ���� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx, strVol)) continue;

		// ������ȣ�� �����´�.
		strTemp = pDM_Book->GetCellData(_T("BB_������ȣ"), idx);
		if (strTemp.IsEmpty()) continue;

		// ũ�� �� 1char .. number
		_stscanf(strTemp, _T("%c%d"), &ch, &nNumber);
				
		if (nNumber > nDupNumber)
			nDupNumber = nNumber;
	}
	
	return nDupNumber;
}

INT CBOFormApi::AutoApplyDupCode(CESL_DataMgr *pDM_Book)
{
	if (pDM_Book == NULL) return -1;

	INT nRowCount = pDM_Book->GetRowCount();

	pDM_Book->SORT(_T("BB_�����"));

	CString strVolMaster;
	CString strVolSlave;
	CString strDupCode;
	INT nDupNo = -1;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx)) continue;

		nDupNo = 1;
		strVolMaster = pDM_Book->GetCellData(_T("BB_�����"), idx);
		while (++idx < nRowCount)
		{
			//if (++idx >= nRowCount) break;
			if (!IsBook(pDM_Book, idx)) continue;

			strVolSlave = pDM_Book->GetCellData(_T("BB_�����"), idx);
			if (strVolMaster != strVolSlave) 
			{
				idx--;
				break;
			}

			// ������ȣ �߰�
			strDupCode.Format(_T("c%d"), ++nDupNo);
			pDM_Book->SetCellData(_T("BB_������ȣ"), strDupCode, idx);

			// DB FALG ����
			CString strQueryFlag = GetQueryFlag(MODE_UPDATE, pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		}
	}


	return 0;
}

INT CBOFormApi::AutoApplyAppendixRegNo(CESL_DataMgr *pDM_Book)
{
	if (pDM_Book == NULL) return -1;

	INT nRowCount = pDM_Book->GetRowCount();

	pDM_Book->SORT(_T("BB_�θ�åKEY"));

	CString strRegCode;
	CString strParentBookKey;
	CString strParentBookRegNoMaster;
	CString strParentBookRegNoSlave;
	CString strRegNo;
	
	INT		ids = -1;
	INT		nRegNo = -1;

	// �η��� ã�� 
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// �η��� �ƴϸ� �Ѿ��.
		if (!IsAppendix(pDM_Book, idx)) continue;

		// ��ϱ��� �ڵ� �����´�.
		strRegCode = pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), idx);
		if (strRegCode.IsEmpty()) continue;

		// �θ� å�� ��Ϲ�ȣ�� ������ �Ѿ��.
		strParentBookKey = pDM_Book->GetCellData(_T("BB_�θ�åKEY"), idx);
		if (strParentBookKey.IsEmpty()) continue;

		ids = GetParentBookIndex(pDM_Book, strParentBookKey);
		if (ids < 0) continue;

		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		if (strParentBookRegNoMaster.GetLength() < 3) continue;
		INT nRegCodeLength;
		nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;

		strParentBookRegNoMaster = pDM_Book->GetCellData(_T("BB_��Ϲ�ȣ"), ids);
		if (strParentBookRegNoMaster.GetLength() <= nRegCodeLength) continue;
		//===================================================================


		nRegNo = 0;

		// �η� ��Ϲ�ȣ �߰�
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(2), ++nRegNo);
		strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(nRegCodeLength), ++nRegNo);
		//===================================================================

		pDM_Book->SetCellData(_T("BB_��Ϲ�ȣ"), strRegNo, idx);

		// DB FALG ����
		CString strQueryFlag = GetQueryFlag(MODE_UPDATE, pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);

		while (++idx < nRowCount)
		{
			// �η��� �ƴϸ� �Ѿ��.
			if (!IsAppendix(pDM_Book, idx)) continue;

			// ��ϱ��� �ڵ� �����´�.
			strRegCode = pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), idx);
			if (strRegCode.IsEmpty()) continue;

			// �θ� å�� ��Ϲ�ȣ�� ������ �Ѿ��.
			strParentBookKey = pDM_Book->GetCellData(_T("BB_�θ�åKEY"), idx);
			if (strParentBookKey.IsEmpty()) continue;

			ids = GetParentBookIndex(pDM_Book, strParentBookKey);
			if (ids < 0) continue;

			strParentBookRegNoSlave = pDM_Book->GetCellData(_T("BB_��Ϲ�ȣ"), ids);
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			if (strParentBookRegNoSlave.GetLength() < 3) continue;
			if (strParentBookRegNoSlave.GetLength() <= nRegCodeLength) continue;
			//===================================================================
			if (strParentBookRegNoMaster != strParentBookRegNoSlave) 
			{
				idx--;
				break;
			}

			// �η� ��Ϲ�ȣ �߰�
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
			// strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(nRegCodeLength), ++nRegNo);
			strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(nRegCodeLength), ++nRegNo);
			//===================================================================
			pDM_Book->SetCellData(_T("BB_��Ϲ�ȣ"), strRegNo, idx);

			// DB FALG ����			
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		}
	}

	return 0;
}

INT CBOFormApi::GetParentBookIndex(CESL_DataMgr *pDM_Book, CString strParentBookKey)
{
	if (pDM_Book == NULL || strParentBookKey.IsEmpty()) return -1;


	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx <nRowCount; idx++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx)) continue;

		// Key ���Ѵ�.
		if (GetBookKey(pDM_Book, idx) == strParentBookKey)
			return idx;
	}

	return -1;
}

VOID CBOFormApi::InitDefaultIgnoreAlias()
{
	m_arrDefaultIgnoreAlias.RemoveAll();
}

INT CBOFormApi::AddDefaultIgnoreAlias(LPCTSTR lpszAlias)
{
	return m_arrDefaultIgnoreAlias.Add(lpszAlias);
}

inline BOOL CBOFormApi::IsBook(CESL_DataMgr *pDM_Book, INT idx, LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_å�η��÷���"), idx) == _T("A")) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D")) return FALSE;

	// ������� ������ üũ
	if (lpszVol != NULL)
		if (pDM_Book->GetCellData(_T("BB_�����"), idx) != lpszVol) return FALSE;

	return TRUE;
}

inline BOOL CBOFormApi::IsAppendix(CESL_DataMgr *pDM_Book, INT idx, LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_å�η��÷���"), idx) != _T("A")) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D")) return FALSE;

	// ������� ������ üũ
	if (lpszVol != NULL)
		if (pDM_Book->GetCellData(_T("BB_�����"), idx) != lpszVol) return FALSE;

	return TRUE;
}

INT CBOFormApi::GetRepresentationBook(CESL_DataMgr *pDM_Book, INT &nIdx)
{
	if (pDM_Book == NULL) return -1;
	nIdx = -1;
	
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		if (pDM_Book->GetCellData(_T("BB_��ǥå����"), idx) == _T("Y")) 
		{
			nIdx = idx;
			return 0;
		}
	}

	return -1;
}

INT CBOFormApi::SetRepresentationBook(CESL_DataMgr *pDM_Book, INT nIdx /*= -1*/)
{
	if (pDM_Book == NULL) return -1;

	// nIdx �� -1�̰� ��ǥå�� ������� ù��° å�� ��ǥå���� �Ѵ�.

	INT ids = -1;

	INT nRowCount = pDM_Book->GetRowCount();
	
	if (nIdx == -1)
	{
		if (GetRepresentationBook(pDM_Book, nIdx) < 0)
		{
			// ��ǥå�� �������
			for (INT idx = 0; idx < nRowCount; idx++)
				if (IsBook(pDM_Book, idx)) break;
			nIdx = idx;
		}		
	}
	else
	{
		// nIdx �� �����Ͱ� å���� Ȯ���Ѵ�.
		if (!IsBook(pDM_Book, nIdx)) return -1;
	}

	// ��ǥå IDX�� ���� �������� N���� ������ �ش�.
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (idx == nIdx)
			pDM_Book->SetCellData(_T("BB_��ǥå����"), _T("Y"), idx);
		else
			pDM_Book->SetCellData(_T("BB_��ǥå����"), _T("N"), idx);
	}
	
	return nIdx;
}

INT CBOFormApi::CorrectBookDM(CESL_DataMgr *pDM_Book)
{
	if (pDM_Book == NULL) return -1;

	// ������ȣ
	CorrectBookDM_DupCode(pDM_Book);

	// ��ǥå : -1�̸� �ڵ�ó���̱� ������ ������ �ȴ�.
	SetRepresentationBook(pDM_Book);

	// ��Ϲ�ȣ : ???
	// TODO

	return 0;
}

INT CBOFormApi::CorrectBookDM_DupCode(CESL_DataMgr *pDM_Book)
{
	if (pDM_Book == NULL) return -1;

	// �Ǵ����� ������ȣ�� �����Ѵ�.

	INT ids = -1;
	INT nRowCount = pDM_Book->GetRowCount();

	CString strVol;
	INT nVolCnt;
	CMap<CString, LPCTSTR, INT, INT> mapVolume;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx)) continue;

		ids = pDM_Book->GetCellData(_T("BB_�����"), idx, strVol);
		
		mapVolume.SetAt(strVol, 1);
	}

	if (mapVolume.GetCount() == 0) return 0;


	POSITION pos = mapVolume.GetStartPosition();
	while (pos)
	{
		mapVolume.GetNextAssoc(pos, strVol, nVolCnt);
		
		CorrectBookDM_DupCode(pDM_Book, strVol);
	}

	return 0;
}

INT CBOFormApi::CorrectBookDM_DupCode(CESL_DataMgr *pDM_Book, CString strVol)
{
	if (pDM_Book == NULL) return -1;
	
	CString strDupCode;
	INT n = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strDupCode.Empty();

		// å�� ���� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx, strVol)) continue;

		if (n != 0) 
			strDupCode.Format(_T("c%d"), ++n);

		pDM_Book->SetCellData(_T("BB_������ȣ"), strDupCode, idx);
	}

	return 0;
}

INT CBOFormApi::EmbededVolSortNo(CESL_DataMgr * pDM_Book, CString strVolSortNo)
{
	INT volSortNo, tmpVolSortNo;
	CString strTmpData;
	volSortNo = _ttoi(strVolSortNo);
	INT cnt = pDM_Book->GetRowCount();
	for(INT i=0 ; i<cnt ; i++)
	{
		pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), i, strTmpData);
		tmpVolSortNo = _ttoi(strTmpData);
		if(volSortNo <= tmpVolSortNo)
		{
			strTmpData.Format(_T("%d"), ((tmpVolSortNo/10)+1)*10);
			pDM_Book->SetCellData(_T("BB_���Ϸù�ȣ"), strTmpData, i);
		}
	}

	return 0;
}