// BO_IndexData.cpp: implementation of the CBO_IndexData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_IndexData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_IndexData::CBO_IndexData(CESL_Mgr * pParentMgr) : CBO_BusinessData(pParentMgr)
{
	m_strDMAlias = _T("DMUP_����_��_��������");
}

CBO_IndexData::~CBO_IndexData()
{

}

INT CBO_IndexData::Apply_IndexItem(CMarc * pMarc)
{
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->Init(m_pParentMgr->m_pInfo->pMarcMgr, pMarc);
	
	INT ids;
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
		ids = SetCellData(IndexToolKitCommand[i], strData);
		if(ids < 0) return -1;
	}

	// IDX_ALL_ITEM�ʵ� ���� - �˻��뺻ǥ��, �˻�������, �˻��� ������, �˻�����ξ�
	//IDX_ALL_ITEM�ʵ带 ä��� ���ؼ� �˻��뺻ǥ��, ������, ������, Ű���� �� �����Ѵ�.
	ids = GetCellData(_T("IBS_�˻��뺻ǥ��"), strIdxTitle);
	if(ids < 0) return -1;
	ids = GetCellData(_T("IBS_�˻���������"), strIdxAuthor);
	if(ids < 0) return -1;
	ids = GetCellData(_T("IBS_�˻��������"), strIdxPublihser);
	if(ids < 0) return -1;
	ids = GetCellData(_T("IBS_�˻���Ű����"), strIdxKeyword);
	if(ids < 0) return -1;
	CString strIdxAllItem ;
	strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublihser + _T(" ") + strIdxKeyword;
	CStringArray TargetArray;
	TargetArray.RemoveAll();
	ParsingString(_T(" "), strIdxAllItem, &TargetArray, TRUE);
	INT cnt = TargetArray.GetSize();
	strIdxAllItem.Empty();
	for(i=0 ; i<cnt ; i++)
	{
		if(strIdxAllItem.IsEmpty()) strIdxAllItem = TargetArray.GetAt(i);
		else strIdxAllItem += _T(" ") + TargetArray.GetAt(i);
	}
	ids = SetCellData(_T("IBS_��ü�˻�����ξ�"), strIdxAllItem);	
	if(ids < 0) return -1;
	return 0;
}

INT CBO_IndexData::ApplyDM(CESL_DataMgr * pDM, INT nIdx)
{
	if(m_pDM == NULL || pDM == NULL) return -1;
	if(nIdx >= pDM->GetRowCount()) return -1;

	if(nIdx < 0)
	{
		pDM->InsertRow(-1);
		nIdx = pDM->GetRowCount() - 1;
	}
	
	INT ids ;

	INT nColCnt = pDM->GetRefColumnCount();
	INT nPos;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strTmpData;
	for(INT i=0 ; i<nColCnt ; i++)
	{
		pDM->GetColumnRefInfo(i, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
		strTmpData = FIELD_ALIAS.Left(2);
		nPos = FIELD_NAME.Find(_T("."), 0);
		if(nPos >= 0) FIELD_NAME = FIELD_NAME.Mid(nPos+1);
		if(strTmpData == _T("IB")) 
		{
			ids = ApplyDMOneCell(pDM, nIdx, i, FIELD_NAME);
			if(ids < 0) return -1;
		}
	}
	return 0;
}

INT CBO_IndexData::SynchronizedIndexAndBookData(CString strSpeciesKey, CESL_DataMgr * pDM_OUT)
{
	if(pDM_OUT == NULL) return -1;

	CString strQuery;
	strQuery.Format(_T("update idx_bo_tbl set USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
		_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
		_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
		_T("IDX_VOL_TITLE=GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX'), ")
		_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
		_T("IDX_VOL=GET_BO_BOOK_INFO(%s,'VOL_INDEX'), ")
		_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE') where rec_key=%s;"), 
		strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey);
	pDM_OUT->AddFrame(strQuery);	
	return 0;
}

INT CBO_IndexData::ApplyMarcToData(CMarc * pMarc)
{
	if(m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	if(m_ArrDataAlias.GetSize() == 0) return 0;
	if(pMarc == NULL) return -1;

	INT ids;
	CString strDM, strData1, strData2, strData3;

	// �Ϲ����� ����Ÿ ó��
	for (INT i = 0; i < m_ArrDataAlias.GetSize(); i++) 
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, m_ArrMarcAlias.GetAt(i), strData1);
		m_pDM->SetCellData(m_ArrDataAlias.GetAt(i), strData1, 0);
	}

	//260$c�� �����Ͱ� ������ 008 7-10�� �����Ͱ� ������ �ش� �����͸� ���� ����⿡ �����Ų��.
	strData1 = m_pDM->GetCellData(_T("IBS_�����"), 0);
	if(strData1.IsEmpty())
	{
		ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@7-10"), strData1);
		if(ids < 0) return -1;
		m_pDM->SetCellData(_T("IBS_�����"), strData1, 0);
	}
	
	// �Ѽ���, �Ѽ�����
	CString strSeriesTitle, strSeriesVol;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("490$a"), strSeriesTitle);
	if(ids < 0) return -1;
	m_pDM->SetCellData(_T("IBS_�Ѽ���"), strSeriesTitle, 0);
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("440$a"), strSeriesTitle);
	if(ids < 0) return -1;
	if(!strSeriesTitle.IsEmpty()) m_pDM->SetCellData(_T("IBS_�Ѽ���"), strSeriesTitle, 0);
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("440$v"), strSeriesVol);
	if(ids < 0) return -1;
	m_pDM->SetCellData(_T("IBS_�Ѽ�����"), strSeriesVol, 0);

	// ��ǥ��
	ids = Get_TITLE(pMarc, strData1, strData2);
	if(ids < 0) return -1;
	if (strData1.GetLength() > 0 && strData2.GetLength() > 0) 
	{
		strData3.Format(_T("%s=%s"), strData1, strData2);
		m_pDM->SetCellData(_T("IBS_��ǥ��"), strData3, 0);
	} 
	else 
	{
		m_pDM->SetCellData(_T("IBS_��ǥ��"), strData1, 0);
	}

	// ������
	ids = Get_AUTHOR(pMarc, strData1);
	if(ids < 0) return -1;
	m_pDM->SetCellData(_T("IBS_������"), strData1, 0);

	//������
	ids = Get_PUBLISHER(pMarc, strData1);
	if(ids < 0) return -1;
	m_pDM->SetCellData(_T("IBS_������"), strData1, 0);

	// Ư���� ����Ÿ ó�� - 020(����)		// 1���� ���� IDX�� ��ǥBOOK�� 1�� �̻��ϰ��� ����
	CString strMarcData1;
	INT nPos;
	ids = FindItemByIndicator(pMarc, _T("020$a"), _T("  "), strMarcData1);
	if(ids < 0) return -1;
	if(ids == 1)
	{
		//isbn�� 893726387382(�����)�̷��� ����. �׷��� '()'���� ���ڴ� �������Ѵ�.
		nPos = strMarcData1.Find(_T("("), 0);
		if(nPos >= 0) strMarcData1 = strMarcData1.Mid(0, nPos);
		m_pDM->SetCellData(_T("IBS_����ISBN"), strMarcData1, 0);	
		
		FindItemByIndicator(pMarc, _T("020$g"), _T("  "), strMarcData1);
		//���Ϲ��Խ� �ΰ���ȣ �������� - �ΰ���ȣ�� TCHAR(5)�̴� ���� �� �̻��� ��� Error�޽����� ����ؾ��Ѵ�.
		if(strMarcData1.GetLength() > 5) 
		{
			strData1.Format(_T("����ISBN �ΰ���ȣ[%s]�� ������ �ֽ��ϴ�. �ش� ������ ������� �ʽ��ϴ�."), strMarcData1);
			m_pParentMgr->ESLAfxMessageBox(strData1);
			strMarcData1.Empty();
		}
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strMarcData1);
		m_pDM->SetCellData(_T("IBS_����ISBN�ΰ���ȣ"), strMarcData1, 0);	
		
		FindItemByIndicator(pMarc, _T("020$z"), _T("  "), strMarcData1);
		m_pDM->SetCellData(_T("IBS_���ǿ���ISBN"), strMarcData1, 0);	
	}

	// Ư���� ����Ÿ ó�� - 020(��Ʈ)
	FindItemByIndicator(pMarc, _T("020$a"), _T("1 "), strMarcData1);
	//��Ʈisbn�� 893726387382(��Ʈ)�̷��� ����. �׷��� '()'���� ���ڴ� �������Ѵ�.
	nPos = strMarcData1.Find(_T("("), 0);
	if(nPos >= 0) strMarcData1 = strMarcData1.Mid(0, nPos);
	m_pDM->SetCellData(_T("IBS_��Ʈ_ISBN1"), strMarcData1, 0);	
	FindItemByIndicator(pMarc, _T("020$g"), _T("1 "), strMarcData1);
	m_pDM->SetCellData(_T("IBS_��Ʈ_ISBN2"), strMarcData1, 0);	
	FindItemByIndicator(pMarc, _T("020$z"), _T("1 "), strMarcData1);
	m_pDM->SetCellData(_T("IBS_��Ʈ_ISBN3"), strMarcData1, 0);	

	return 0;
}

INT CBO_IndexData::ApplyDataToMarc(CMarc * pMarc)
{
	if (pMarc == NULL || m_pParentMgr == NULL) return -1;

	CString strDM, strData1, strData2, strData3;
	INT ids;
	// �Ϲ����� ����Ÿ ó��
	for (INT i = 0; i < m_ArrDataAlias.GetSize(); i++) 
	{
		strData1.Empty();
		m_pDM->GetCellData(m_ArrDataAlias.GetAt(i), 0, strData1);
		if (strData1.GetLength() > 0) 
		{
			ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, m_ArrMarcAlias.GetAt(i), strData1, _T(""), MODIFY_ITEM);
			if(ids < 0) return -1;
		}
		else
		{
			ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, m_ArrMarcAlias.GetAt(i));
			if(ids < 0) return -1;
		}
	}
	
	//008����� �ʵ� ����
	ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("008"));
	if(ids < 0) return -1;
	strData1 = m_pDM->GetCellData(_T("IBS_�����"), 0);
	ids = GetDigitData(strData1, strData2);
	if(strData2.GetLength() == 4)
	{	
		ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem( pMarc, _T("008@6"), _T("s") );
		if(ids < 0) return -1;
		ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem( pMarc, _T("008@7-10"), strData2);		
		if(ids < 0) return -1;
		ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("008@38-39"), _T("  ") );
		if(ids < 0) return -1;
	}
	
	//�Ѽ���/�Ѽ�����
	ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("440"));
	if(ids < 0) return -1;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("490"));
	if(ids < 0) return -1;
	CString arrAlias[2] = {_T("IBS_�Ѽ���"), _T("IBS_�Ѽ�����")};
	CString arrData[2];
	m_pDM->GetCellData(arrAlias, 2, arrData, 0);
	if(!arrData[0].IsEmpty() && arrData[1].IsEmpty())
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("490$a"), arrData[0]);
	else if(arrData[0].IsEmpty() && !arrData[1].IsEmpty())
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("440$v"), arrData[1]);
	else if(!arrData[0].IsEmpty() && !arrData[1].IsEmpty())
	{
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("440$a"), arrData[0]);
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("440$v"), arrData[1]);
	}
	
	// ��ǥ��
	strData1 = _T("");
	m_pDM->GetCellData(_T("IBS_��ǥ��"), 0, strData1);
	ids = Set_TITLE(pMarc, strData1);
	if(ids < 0) return -1;
	// ������
	strData1 = _T("");
	m_pDM->GetCellData(_T("IBS_������"), 0, strData1);
	ids = Set_AUTHOR(pMarc, strData1);
	if(ids < 0) return -1;
	// �����
	strData1 = m_pDM->GetCellData(_T("IBS_�����"), 0);
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$n"));
	if(!strData1.IsEmpty())
	{
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$n"), strData1);
	}
	//������
	strData1 = _T("");
	m_pDM->GetCellData(_T("IBS_������"), 0, strData1);
	ids = Set_PUBLIHSER(pMarc, strData1);
	if(ids < 0) return -1;

	ids = Apply_MARC_700Author(pMarc);
	if(ids < 0) return -1;

	// Ư���� ����Ÿ ó�� - 020(��Ʈ)
	ids = SetISBN_to_Marc(pMarc, _T("020"), _T("1 "));		// IBS_��Ʈ_ISBN is 020 1 blank
	if(ids < 0) return -1;

	return 0;
}

INT CBO_IndexData::InitBusinessData(CString strConnectionInfo, CString strDMAlias/*=_T("")*/)
{
	if(strConnectionInfo.IsEmpty()) strConnectionInfo = m_pParentMgr->CONNECTION_INFO;
	if(!strDMAlias.IsEmpty()) m_strDMAlias = strDMAlias;

	if(strConnectionInfo.IsEmpty() || m_strDMAlias.IsEmpty()) return -1;

	INT ids = InitDataManager(strConnectionInfo, m_strDMAlias);
	if(ids < 0) return -1;

	m_ArrDataAlias.RemoveAll();	m_ArrMarcAlias.RemoveAll();
	m_ArrDataAlias.Add(_T("IBS_�����"));		m_ArrDataAlias.Add(_T("IBS_�����ڷ�"));
	m_ArrDataAlias.Add(_T("IBS_�����"));		m_ArrDataAlias.Add(_T("IBS_�ǻ���"));
	m_ArrDataAlias.Add(_T("IBS_������Ư��"));	m_ArrDataAlias.Add(_T("IBS_������"));
	m_ArrDataAlias.Add(_T("IBS_ũ��"));			m_ArrDataAlias.Add(_T("IBS_������"));
	m_ArrDataAlias.Add(_T("IBS_�����"));
	m_ArrMarcAlias.Add(_T("260$c"));			m_ArrMarcAlias.Add(_T("300$e"));
	m_ArrMarcAlias.Add(_T("300$a"));			m_ArrMarcAlias.Add(_T("250$a"));
	m_ArrMarcAlias.Add(_T("300$b"));			m_ArrMarcAlias.Add(_T("260$a"));
	m_ArrMarcAlias.Add(_T("300$c"));			m_ArrMarcAlias.Add(_T("260$b"));
	m_ArrMarcAlias.Add(_T("245$n"));

	return 0;
}

INT CBO_IndexData::Apply_MARC_700Author(CMarc *pMarc)
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

INT CBO_IndexData::SetISBN_to_Marc(CMarc *pMarc, CString strTag, CString strInd)
{
	CString strData1=_T(""), strData2=_T(""), strData3=_T("");
	CString strMarcData1=_T(""), strMarcData2=_T(""), strMarcData3=_T("");
	CString strIndexData=_T(""), strNewIndexData=_T("");
	CString strPrice, strVol;
	
	strData1 = _T("");		strData2 = _T("");		strData3 = _T("");
	strMarcData1 = _T("");		strMarcData2 = _T("");		strMarcData3 = _T("");
	m_pDM->GetCellData(_T("IBS_��Ʈ_ISBN1"), 0, strData1);
	strData1.Replace(_T("-"), _T(""));
	if(!strData1.IsEmpty()) strData1 += _T("(��Ʈ)");
	m_pDM->GetCellData(_T("IBS_��Ʈ_ISBN2"), 0, strData2);
	m_pDM->GetCellData(_T("IBS_��Ʈ_ISBN3"), 0, strData3);
	
	if (strData1.GetLength() > 0) strMarcData1.Format(_T("%ca%s"), SUBFIELD_CODE, strData1);
	if (strData2.GetLength() > 0) strMarcData2.Format(_T("%cg%s"), SUBFIELD_CODE, strData2);
	if (strData3.GetLength() > 0) strMarcData3.Format(_T("%cz%s"), SUBFIELD_CODE, strData3);
	
	if (strData1.GetLength() == 0 && strData2.GetLength() == 0 && strData3.GetLength() == 0) 
	{
		;
	} 
	else 
	{
		strNewIndexData.Format(_T("0201  %s%s%s"), strMarcData1, strMarcData2, strMarcData3);
	}
	
	CArray <CString, CString&> Array;
	Array.RemoveAll();
	CString strField;
	
	CString strTmpInd;
	strTag.Replace('$', SUBFIELD_CODE);
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
