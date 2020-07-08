// DupKeyMakerByGrid.cpp: implementation of the CDupKeyMakerByGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DupKeyMakerByGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDupKeyMakerByGrid::CDupKeyMakerByGrid(CESL_Mgr * pMgr) : CDupKeyMaker(pMgr)
{
	m_pSourceDM = NULL;
	m_pSourceGrid = NULL;
}

CDupKeyMakerByGrid::~CDupKeyMakerByGrid()
{

}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - KEY DM ����
// [ �̴Ͻ��� ]
//////////////////////////////////////////////////////////////////////
INT CDupKeyMakerByGrid::MakeKeyDM()
{
	//================================================================
	// ��������
	//================================================================
	INT i, ids, rowCnt, cnt, nPos;	
	CString strMarc, title, author, publisher, pubyear, isbn, strTmpData, regNo;
	CArray<CString, CString&> arrIsbn;
	CMarc marc;

	if( m_pKeyDM		== NULL	|| 
		m_pSourceDM		== NULL || 
		m_pSourceGrid	== NULL	)	return -1;

	//================================================================
	// 1. �׸��� ���� �ڷ���� ��Ű�� ��������� �����Ѵ�. ( m_ArrRecKey )
	//================================================================
	ids = GetRecKeyList();
	if( ids < 0 )	return -1;

	//================================================================
	// 2. m_pKeyDM�� �����Ѵ�.
	//================================================================
	ids = RefreshAndMergeKeyDM();
	if( ids < 0 )	return -1;

	//================================================================
	// 3. ����� DM�� ��ũ������ �̿��Ͽ� KEYDM�� ������ �籸���Ѵ�.
	//================================================================
	rowCnt	= m_pKeyDM->GetRowCount();
	cnt		= nPos = -1;
	for( i=rowCnt-1; i>=0 ; i-- )
	{
		m_pKeyDM->GetCellData(_T("BS_MARC"), i, strMarc);
		if( strMarc.GetLength() == 0 )	continue;
		ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if( ids < 0 )	continue;

		ids = ExtractKeyByMarc(&marc, i);
		if( ids < 0 )	m_pKeyDM->DeleteRow(i);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �׸��忡 ������ �ڷ���� ��Ű�� ��������� �����Ѵ�.
// [ �̴Ͻ��� ]
// - �׸��� ������ �ڷ���� ��Ű(I.REC_KEY)�� ������ ������� ��Ű����Ʈ�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
INT CDupKeyMakerByGrid::GetRecKeyList()
{
	//================================================================
	// ��������
	//================================================================
	INT i, ids, count;
	CString strKey, strInquery;
	count = 0;	
	strKey.Empty();	strInquery.Empty();


	//================================================================
	// �׸��� ������ �ڷ���� ��Ű(I.REC_KEY)�� ������ ������� ��Ű����Ʈ�� �����Ѵ�.
	//================================================================
	ids = m_pSourceGrid->SelectMakeList();
	if( ids < 0 )	return -1;
	i = m_pSourceGrid->SelectGetHeadPosition();
	if( i < 0 )		return -1;
	
	while( i >= 0 )
	{
		if( count == 600 )
		{
			strInquery = _T("(") + strInquery + _T(")");
			m_ArrRecKey.Add(strInquery);
			strInquery.Empty();
			count = 0;
		}

		// ���� ��Ű�� ���ٸ� ����Ű�� �����´�.
		m_pSourceDM->GetCellData(_T("IBS_��KEY"), i, strKey);
		if( strKey.IsEmpty() == TRUE )		m_pSourceDM->GetCellData(_T("BS_��KEY"), i, strKey);		

		if( strInquery.IsEmpty() == TRUE )	strInquery = strKey;
		else strInquery += _T(", ") + strKey;
		
		i = m_pSourceGrid->SelectGetNext();
		count++;
		strKey.Empty();
	}
	strInquery = _T("(") + strInquery + _T(")");
	m_ArrRecKey.Add(strInquery);

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ŰDM�� �����Ѵ�.
// [ �̴Ͻ��� ]
// 1. m_ArrDataMgr(DM���庯��)�� ������ �����.
// 2. m_ArrDataMgr�� �����Ѵ�.(Key DM ����)
// 3. ����� KeyDM�� ��� ���Ͽ� m_pKeyDM���� �����Ѵ�.
// 4. m_ArrDataMgr(DM���庯��)�� ������ �����.
//////////////////////////////////////////////////////////////////////
INT CDupKeyMakerByGrid::RefreshAndMergeKeyDM()
{
	//================================================================
	// ��������
	//================================================================
	INT i, ids, cnt;
	CString strDMPK, strQuery, strKey;
	CESL_DataMgr* pDM = NULL;
	CESL_DataMgr TmpDM ;

	cnt = m_ArrRecKey.GetSize();
	if( cnt == 0 )	return -1;
	
	//================================================================
	// 1. m_ArrDataMgr(DM���庯��)�� ������ �����.
	//	- 600�� ������ DM�� �����Ѵ�.(IN�Լ�)
	//================================================================
	RemoveDataMgrArray();

	//================================================================
	// 2. m_ArrDataMgr�� �����Ѵ�.(Key DM ����)
	//	- ����� ��Ű���� �̿��Ͽ� Key DM�� �����Ͽ� ��������� �����Ѵ�.
	//================================================================
	for( i=0 ; i<cnt ; i++ )
	{
		// DM �ʱ�ȭ
		pDM = new CESL_DataMgr;
		pDM->SetCONNECTION_INFO(m_pKeyDM->CONNECTION_INFO);
		pDM->InitDMPKFromFile(m_pKeyDM->DB_MGR_ALIAS);
		pDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

		strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN %s"), m_ArrRecKey.GetAt(i));
		
		ids = pDM->RefreshDataManager(strQuery);
		if( ids < 0 )	return -1;
		m_ArrDataMgr.Add(pDM);
	}
	
	//================================================================
	// 3. ����� KeyDM�� ��� ���Ͽ� m_pKeyDM���� �����Ѵ�.
	//================================================================
	TmpDM.InitDMPKFromFile(m_pKeyDM->DB_MGR_ALIAS);
	TmpDM.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	cnt = m_ArrDataMgr.GetSize();
	m_pKeyDM->FreeData();
	for( i=0 ; i<cnt ; i++ )
	{
		AddToTargetDM(&TmpDM, m_ArrDataMgr.GetAt(i), -1);
	}

	// 2006.03.16 KBS - KeyDM�� �׸����� ������ ������� �����Ͽ� �����Ѵ�.(�����䱸����)
	//================================================================
	INT col, nCols, nCol, left, right, mid, nKey, nTmpKey;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strTmpKey;
	ids = m_pSourceGrid->SelectMakeList();
	if( ids < 0 )	return -1;
	i = m_pSourceGrid->SelectGetHeadPosition();
	if( i < 0 )		return -1;
	// ��Ű �����˻��� ���� �����Ѵ�.
	TmpDM.SORT( _T("IBS_��KEY") );
	while( i >= 0 )
	{
		m_pSourceDM->GetCellData(_T("IBS_��KEY"), i, strKey);
		left	= 0; 
		right	= TmpDM.GetRowCount();
		while( right >= left )
		{
			mid = (right+left) / 2;
			strTmpKey = TmpDM.GetCellData(_T("IBS_��KEY"), mid);
			if( strKey == strTmpKey )
			{
				m_pKeyDM->InsertRow(-1);
				nCols = TmpDM.m_nCols;
				if( nCols < 0 )		nCols = TmpDM.GetRefColumnCount();
				for( col=0; col<nCols; col++ )
				{
					if( TmpDM.GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0 )	continue;

					nCol = m_pKeyDM->FindColumn(FIELD_ALIAS);			
					m_pKeyDM->SetCellData(m_pKeyDM->GetRowCount()-1, nCol, TmpDM.GetCellData(mid, col));
				}
				break;
			}
			else
			{
				nKey	= _ttoi(strKey	);
				nTmpKey = _ttoi(strTmpKey);
				if( nKey > nTmpKey )	left  = mid + 1;
				else					right = mid - 1;
			}
		}
		//  ã�� ���ߴٸ� �׳� �н��Ѵ�. ( ����ó�� )		
		/*
		for( j=0; j<TmpDM.GetRowCount(); j++ )
	{
			if( strKey == TmpDM.GetCellData(_T("IBS_��KEY"), j) )	break;
	}
		if( j > TmpDM.GetRowCount() )		continue;
		*/

		i = m_pSourceGrid->SelectGetNext();
	}	
	//================================================================


	//================================================================
	// 4. m_ArrDataMgr(DM���庯��)�� ������ �����.
	//================================================================
	RemoveDataMgrArray();
	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - pSourceDM�� ������  pTargetDM idx��°�� �����Ѵ�. 
// [ �̴Ͻ��� ] - ����
//////////////////////////////////////////////////////////////////////
INT CDupKeyMakerByGrid::AddToTargetDM(CESL_DataMgr *pTargetDM, 
									  CESL_DataMgr *pSourceDM, 
									  INT idx)
{
	if( pTargetDM == NULL || pSourceDM == NULL )	return -1;
	
	//===============================================================
	// ��������
	//===============================================================
	INT row, col, nRow, nCurRow, nCol, nCols, FirstRow;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	CMarc marc;
	
	FirstRow = -1;
	
	// ����� �ʵ� ������ �����Ѵ�.
	nRow = pSourceDM->GetRowCount();	
	if( nRow == 0 )		return 0;
	
	for( row=0; row<nRow; row++ )
	{
		pTargetDM->InsertRow(idx);
		if( idx >= 0 )	nCurRow = idx;
		else			nCurRow = pTargetDM->GetRowCount() - 1;

		if( FirstRow < 0 )	FirstRow = nCurRow;

		nCols = pSourceDM->m_nCols;
		if( nCols < 0 )		nCols = pSourceDM->GetRefColumnCount();

		for( col=0; col<nCols; col++ )
		{
			if( pSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0 )	continue;

			nCol = pTargetDM->FindColumn(FIELD_ALIAS);
			pTargetDM->SetCellData(nCurRow, nCol, pSourceDM->GetCellData(row, col));
		}
	}

	return FirstRow;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - DM�� ����� ��������� ������ ����.
// [ �̴Ͻ��� ] - ����
//////////////////////////////////////////////////////////////////////
INT CDupKeyMakerByGrid::RemoveDataMgrArray()
{
	INT cnt = m_ArrDataMgr.GetSize();
	for( INT i=0 ; i<cnt ; i++ )
	{
		delete m_ArrDataMgr.GetAt(i);
	}
	m_ArrDataMgr.RemoveAll();

	return 0;
}
