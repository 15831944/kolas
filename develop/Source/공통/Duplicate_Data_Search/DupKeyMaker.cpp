// DupKeyMaker.cpp: implementation of the CDupKeyMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DupKeyMaker.h"
#include "../IndexToolKit/Index.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������
CDupKeyMaker::CDupKeyMaker(CESL_Mgr * pMgr)
{
	m_pKeyDM = NULL;
	m_pParentMgr = pMgr;
}
// �Ҹ���
CDupKeyMaker::~CDupKeyMaker()
{

}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
//
// [ �̴Ͻ��� ]
//////////////////////////////////////////////////////////////////////
INT CDupKeyMaker::RemoveRoleWord(CString &strAuthor)
{
	//================================================================
	// ��������
	//================================================================
	INT nFind;
	CString str1, str2;
	CESL_CharSetMgr CharSetMgr_UnlessWord;


	//================================================================
	// 1. ���Ҿ���� ���ڼ��� ���Ѵ�.
	//================================================================
	CharSetMgr_UnlessWord.InitData(_T("���Ҿ�"), _T("���Ҿ�"));
	
	//================================================================
	// 2. ���ڰ��� ���鹮�ڷ� �� �ܾ�� �и��Ѵ�.
	// 3. �δܾ���� ���� ���Ҿ ���� ���ڼ����� �ִٸ� ����ڸ� �����.
	//================================================================
	// 2.
	strAuthor.TrimLeft();	strAuthor.TrimRight();
	nFind = strAuthor.ReverseFind(' ');
	if( nFind > 0 )
	{
		str1 = strAuthor.Mid(nFind);
		str2 = str1;
		str1.TrimLeft();	str1.TrimRight();
		
		// 3.
		if( CharSetMgr_UnlessWord.Find(str1, str2) == 0 )
		{
			strAuthor.Delete(nFind, strAuthor.GetLength() - nFind);
		}
		strAuthor.TrimLeft();	strAuthor.TrimRight();
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��ũ���� Ű���� �����Ͽ� DM�� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ��ũ���� �ش簪���� �����Ѵ�.
//		- 245$a : title		(������)
//		- 245$d : author	(ù��° ����)
//		- 260$b : publisher	(������,������)
//		- 260$c : pubyear	(�����,������)
//		- 020$a : isbn		(ISBN)
//		- 049$l : regno		(��Ϲ�ȣ)
// 2. ���ڿ��� ���Ҿ �����Ѵ�.
// 3. 245$d�� ���ڰ� ������� 100$a ��ũ���� ���ڸ� �����´�.
// 4. ������, ����, �����ڰ� ���ٸ� �ùٸ� ��ũ�� �ƴϹǷ� ���� �����Ѵ�.
// 5. ��� DM�� nIdx��° �ο쿡 ������ ������ ������ �����Ѵ�.
//		- IBS_��ǥ��
//		- IBS_������
//		- IBS_������
//		- IBS_�����
//		- BB_��Ϲ�ȣ
//		- BB_����_ISBN
//////////////////////////////////////////////////////////////////////
INT CDupKeyMaker::ExtractKeyByMarc(CMarc *pMarc, INT nIdx/*=-1*/)
{
	//================================================================
	// ��������
	//================================================================
	INT nPos, cnt, i;
	CString strMarc, title, author, publisher, pubyear, isbn, strTmpData, regNo;
	//===================================================
	//2010.11.29 ADD BY PJW : �Ѽ��� �����߰�
	CString strSeriesTitle;
	//===================================================
	CArray<CString, CString&> arrIsbn;
	
	if( pMarc == NULL || m_pKeyDM == NULL )		return -1;	
	
	//================================================================
	// 1. ��ũ���� �ش簪���� �����Ѵ�.
	//		- 245$a : title		(������)
	//		- 245$d : author	(ù��° ����)
	//		- 260$b : publisher	(������,������)
	//		- 260$c : pubyear	(�����,������)
	//		- 020$a : isbn		(ISBN)
	//		- 049$l : regno		(��Ϲ�ȣ)
	//================================================================
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$a"), title			);
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), author			);	
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("260$b"), publisher		);
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("260$c"), pubyear		);
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("020$a"), isbn, &arrIsbn	);
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("049$l"), regNo			);
	//===================================================
	//2010.11.29 ADD BY PJW : �Ѽ��� �����߰�
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("440$a"), strSeriesTitle	);
	if( strSeriesTitle.IsEmpty() )
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("490$a"), strSeriesTitle	);
	}
	//===================================================


	//================================================================
	// 2. ���ڿ��� ���Ҿ �����Ѵ�.
	//================================================================
	RemoveRoleWord(author);
	
	//================================================================
	// 3. 245$d�� ���ڰ� ������� 100$a ��ũ���� ���ڸ� �����´�.
	//================================================================
	if( author.IsEmpty() == TRUE ) 
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), author);
	}
	
	//================================================================
	// 4. ������, ����, �����ڰ� ���ٸ� �ùٸ� ��ũ�� �ƴϹǷ� ���� �����Ѵ�.
	//	2005.11.01  isbn�׸� �߰� (����䱸����)
	//================================================================
	if( title.IsEmpty() == TRUE && author.IsEmpty() == TRUE && publisher.IsEmpty() == TRUE && isbn.IsEmpty() == TRUE )	return -1;
	

	//================================================================
	// 5. ��� DM�� nIdx��° �ο쿡 ������ ������ ������ �����Ѵ�.
	//		- IBS_��ǥ��
	//		- IBS_������
	//		- IBS_������
	//		- IBS_�����
	//		- BB_��Ϲ�ȣ
	//		- BB_����_ISBN
	//================================================================
	// -1�̶�� �翬�� �ο츦 �߰��Ͽ� �����Ѵ�.
	if( nIdx < 0 )
	{
		m_pKeyDM->InsertRow(-1);
		nIdx = m_pKeyDM->GetRowCount()-1;
	}
	m_pKeyDM->SetCellData(_T("IBS_��ǥ��"), title, nIdx);
	m_pKeyDM->SetCellData(_T("IBS_������"), author, nIdx);
	m_pKeyDM->SetCellData(_T("IBS_������"), publisher, nIdx);
	m_pKeyDM->SetCellData(_T("IBS_�����"), pubyear, nIdx);
	m_pKeyDM->SetCellData(_T("BB_��Ϲ�ȣ"), regNo, nIdx);
	//===================================================
	//2010.11.29 ADD BY PJW : �Ѽ��� �����߰�
	m_pKeyDM->SetCellData(_T("UDF_�Ѽ���"), strSeriesTitle, nIdx);
	//===================================================
	
	
	cnt = arrIsbn.GetSize();
	if		( cnt == 0 ) 
	{
		m_pKeyDM->SetCellData(_T("BB_����_ISBN"), _T(""), nIdx);
	}
	else if	( cnt == 1 )
	{
		nPos = isbn.Find(_T("("), 0);
		if( nPos >= 0 ) 
		{
			isbn = isbn.Mid(0, nPos);
		}

		m_pKeyDM->SetCellData(_T("BB_����_ISBN"), isbn, nIdx);
	}
	else
	{
		for( i=0 ; i<cnt ; i++)
		{
			strTmpData = arrIsbn.GetAt(i);
			nPos = strTmpData.Find(_T("("), 0);
			if( nPos >= 0 )		
			{
				strTmpData = strTmpData.Mid(0, nPos);
			}
			if( isbn.IsEmpty() == TRUE )	isbn = strTmpData;
			else isbn += _T(" ") + strTmpData;
		}
		m_pKeyDM->SetCellData(_T("BB_����_ISBN"), isbn, nIdx);
	}

	title = author = publisher = pubyear = isbn = regNo = _T("");

	return nIdx;
}
