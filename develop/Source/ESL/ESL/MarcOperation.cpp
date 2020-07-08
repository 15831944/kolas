// MarcOperation.cpp: implementation of the CMarcOperation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcOperation.h"
#include "efs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcOperation::CMarcOperation()
{
	m_pMarcMgr = NULL;
	m_pMarc = NULL;
	m_pMarcInstruction = NULL;
	m_arrayParam.RemoveAll();

	m_CharSetMgr_UnlessWord.InitData(_T("���Ҿ�"), _T("���Ҿ�"));
	//m_CharSetMgr_Chi.InitData(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));
}

CMarcOperation::~CMarcOperation()
{
	m_arrayParam.RemoveAll();
}

BOOL CMarcOperation::ExecuteOperation(CMarcMgr *pMarcMgr, CMarc* pMarc, CMarcInstruction* pMarcInstruction)
{
	EFS_BEGIN

	if (pMarcMgr == NULL)
	{
		m_strLastError = _T("pMarcMgr Is Null");
		return FALSE;
	}
	if (pMarc == NULL) 
	{
		m_strLastError = _T("pMarc Is Null");
		return FALSE;
	}
	if (pMarcInstruction == NULL)
	{
		m_strLastError = _T("pMarcInstruction is NULL");
		return FALSE;
	}

	// Set marc, marc instruction to global
	m_pMarcMgr = pMarcMgr;
	m_pMarc = pMarc;
	m_pMarcInstruction = pMarcInstruction;
	FetchParam();

	//CString strOperator = pMarcInstruction->m_strOperator;
	CString strOperator;
	strOperator.Format(_T("%s"), pMarcInstruction->m_pszOperator);

	if (strOperator == _T("="))
		return Operation_Buchul();		// TODO : �ӽ÷� ����� ���� �� �����ٶ�
		//return Operation_Equal();
	else if (strOperator == _T("+"))
		return Operation_Plus();
		
	m_strLastError = _T("Mached Operator is not Exist");
	return FALSE;

	EFS_END

	return FALSE;
}

INT CMarcOperation::FetchParam()
{
	EFS_BEGIN

	if (m_pMarcInstruction == NULL) return -1;

	m_arrayParam.RemoveAll();

	INT nStart = 0;
	INT nEnd = 0;

	CString strParam;
	strParam.Format(_T("%s"), m_pMarcInstruction->m_pszParam);


	nEnd = strParam.Find(_T(","));
	if (nEnd <= 0)
	{
		strParam.TrimLeft();
		strParam.TrimRight();

		m_arrayParam.Add(strParam);
	}
	else
	{
		while (nEnd > 0)
		{
			nStart = nEnd + 1;

			m_arrayParam.Add(strParam.Mid(nStart, nEnd - nStart + 1));
			nEnd = strParam.Find(nStart + 1);
		}

		m_arrayParam.Add(strParam.Mid(nStart));
	}

	return 0;	

	EFS_END

	return -1;
}

INT CMarcOperation::ConvertChiToKor(CString &strSrc)
{
	EFS_BEGIN
		/*

	CString strReturn;
	CString strWord;

	INT nCount = strSrc.GetLength();

	TCHAR *pBuffer = strSrc.GetBuffer(0);
	
	// 0xCA80 ~ 0xFDFF
	// : �� �� ��° ����Ʈ�� ���� >= 0xA1 && <= 0xFE

	_TUCHAR ch1;
	_TUCHAR ch2;
	BOOL isChinese = FALSE;
	
	
	for (INT i = 0; i < nCount; i++)
	{
		ch1 = pBuffer[i];
		ch2 = pBuffer[i+1];
		
		if (ch1 & 0x80)
		{
			if( ( ch1 >= 0xCA ) && (ch1  <= 0xFD ) 
				&& ( ch2  >= 0xA1 ) && ( ch2  <= 0xFE) ) 
			{ 
				strWord.Format(_T("%c%c"), ch1, ch2);
				
				m_CharSetMgr_Chi.Find(strWord, strWord);

				strReturn += strWord;
			}
			else
			{
				strReturn += ch1;
				strReturn += ch2;
			}
			i++; 
		}
		else
		{
			strReturn += ch1;
		}
	}

	strSrc = strReturn;
	*/
	return 0;

	EFS_END

	return -1;
}

INT CMarcOperation::RemoveAllParenthesis(CString &strSrc)
{
	CString strReturn;

	INT nCount = strSrc.GetLength();
	TCHAR findTch = '(';
	BOOL bFind = FALSE;
	for (INT i = 0; i < nCount; i++)
	{
		TCHAR tch = strSrc.GetAt(i);
		if (tch == findTch)
		{
			bFind = TRUE;
			findTch == ')' ? findTch = '(' : findTch = ')';
			continue;
		}

		if (findTch != ')')
		{
			strReturn += tch;
		}
	}

	strSrc = strReturn;

	if (bFind)
		return 1;
	else 
		return 0;

	return 0;
}

BOOL CMarcOperation::IsDupMarcItem(CString strMarcAlias, CString strCheckData)
{
	CArray<CString, CString&> arrMarcAlias;
	CString strTag;
	CString strValue;
	strTag = strMarcAlias.Left(3);
	strValue = strMarcAlias.Mid(3);

	// 100, 110, 111
	if (strTag == _T("100") ||
		strTag == _T("110") ||
		strTag == _T("111") ||
		strTag == _T("130")
		)
	{
		arrMarcAlias.Add(_T("100") + strValue);
		arrMarcAlias.Add(_T("110") + strValue);
		arrMarcAlias.Add(_T("111") + strValue);
		arrMarcAlias.Add(_T("130") + strValue);

		return IsDupMarcItem(&arrMarcAlias, strCheckData);
	}
	// 700, 710, 711
	else if (strTag == _T("700") ||
			 strTag == _T("710") ||
			 strTag == _T("711")
		)
	{
		arrMarcAlias.Add(_T("700") + strValue);
		arrMarcAlias.Add(_T("710") + strValue);
		arrMarcAlias.Add(_T("711") + strValue);

		return IsDupMarcItem(&arrMarcAlias, strCheckData);
	}
	// 130, 730, 740
	else if (strTag == _T("130") ||
			 strTag == _T("730") ||
			 strTag == _T("740")
		)
	{
		arrMarcAlias.Add(_T("130") + strValue);
		arrMarcAlias.Add(_T("730") + strValue);
		arrMarcAlias.Add(_T("740") + strValue);

		return IsDupMarcItem(&arrMarcAlias, strCheckData);
	}
	else
	{
		arrMarcAlias.Add(strMarcAlias);

		return IsDupMarcItem(&arrMarcAlias, strCheckData);
	}
	
	return FALSE;
}

BOOL CMarcOperation::IsDupMarcItem(CArray<CString, CString&> *pArrMarcAlias, CString strCheckData)
{
	CString strItemData;
	INT nResultCount = -1;
	INT idx2 = -1;
	CArray<CString, CString&> arrStrResult;
	INT nCount = pArrMarcAlias->GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		arrStrResult.RemoveAll();

		m_pMarcMgr->GetItem(m_pMarc, pArrMarcAlias->GetAt(idx), strItemData, &arrStrResult);
		nResultCount = arrStrResult.GetSize();
		for (idx2 = 0; idx2 < nResultCount; idx2++)
		{
			// ������ �˻�
			if (strCheckData == arrStrResult.GetAt(idx2))
				return TRUE;
		}
	}
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Execute Operation
INT CMarcOperation::Operation_Buchul()
{
	EFS_BEGIN

	// ���� ���۷��̼�

	// TODO : ���ǻ��� : ���� �����ʹ� �ǽð����� üũ�Ѵ�.

	// ���Ҿ� �ð����� �о����
	m_CharSetMgr_UnlessWord.InitData(_T("���Ҿ�"), _T("���Ҿ�"));

	// ������ ���Ҿ� ����
	// ���� -> �ѱ�
	CString strData;
	CString strTemp;
	CArray<CString, CString&> strArrFrom;
	CArray<CString, CString&> strArrInd;

	//======================================================
	//2008.09.02 ADD BY PJW : 1. SETITEM�� DEFAULT�ε����� ADDITEM�Ѵ� -> ���ñ�ȣ�� ������������ INSERTFIELD�� ���
    //                        2. INSERTFIELD�� ��� �ϱ����ؼ� �����߰�
	TCHAR cCode;
	CString strField;
	cCode = 31 ;
	//======================================================

	
	INT ids = -1;
	INT nCountFrom = -1;
	
	// ��� ���Ҿ� �����Ѵ�.

	/**
		245$d -> 100$a

		245$d

		���� ����	�񵵼�	����
		10 : 100$a, 100$a,	710$a
		00 : 700$a, 700$a
		20 : 100$a, 700$a

	*/

	// 2005.11.10 ADD BY PDJ
	// ','�� ������ ���� ��ġ�Ѵ�.
	BOOL bAuthorChangeCheck;
	GetAuthorCodeChangeFlag(bAuthorChangeCheck);

	strArrFrom.RemoveAll();
	
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("245$d"), strData, &strArrFrom, &strArrInd);
	if (ids >= 0)
	{
		CString strInd;
		CString strToMarcAlias;

/*		strData.Replace(_T("("), _T(""));
		strData.Replace(_T(")"), _T(""));				
		strData.Replace(_T("["), _T(""));
		strData.Replace(_T("]"), _T(""));
*/		
		nCountFrom = strArrFrom.GetSize();
		for (INT idx = 0; idx < nCountFrom; idx++)
		{	
			strData = strArrFrom.GetAt(idx);
			strData.TrimLeft();		strData.TrimRight();

			//2006.12.11 KCR4 ����

			strData.Replace(_T("[��]"),_T(""));
			strData.Replace(_T("[��]"),_T(""));
			strData.Replace(_T("[et al.]"),_T(""));

			strData.Replace(_T("("), _T(""));
			strData.Replace(_T(")"), _T(""));				
			strData.Replace(_T("["), _T(""));
			strData.Replace(_T("]"), _T(""));

		    INT nFind = strData.Find(_T(": "));
			if (nFind > 0)
			{
				CString str1 = strData.Mid(nFind+2);
				strData = str1;
			}

			// ���Ҿ �����Ѵ�.
			nFind = strData.ReverseFind(' ');
			if (nFind > 0)
			{
				CString str1 = strData.Mid(nFind);
				CString str2 = str1;
				str1.TrimLeft();	str1.TrimRight();				

				if (m_CharSetMgr_UnlessWord.Find(str1, str2) == 0)
				{
					strData.Delete(nFind, strData.GetLength() - nFind);
				}
			}

			// ���� -> �ѱ�
			ConvertHanjaToHangul(strData);
			
			strData.TrimLeft();			strData.TrimRight();

			if (strData.IsEmpty()) continue;
				
			// ','�� ������� ��ġ�Ѵ�.
			if ( bAuthorChangeCheck )
			{
				nFind = strData.Find(',');
				if (nFind > 0)
				{
					strTemp = strData;
					strData.Format(_T("%s, %s"), strData.Mid(nFind + 1), strData.Mid(0, nFind));
				}			
				strData.TrimLeft();				strData.TrimRight();
			}

			strInd = strArrInd.GetAt(idx);
			
			// 2008.06.20 UPDATE BY PDJ : ���ո�ũ ����
			strToMarcAlias = _T("700$a");
			/*
			if (strInd == _T("10"))
			{
				if (m_pMarcMgr->GetMarcGroup() == _T("����"))
					strToMarcAlias = _T("100$a");
				else if (m_pMarcMgr->GetMarcGroup().Find(_T("�񵵼�")) >= 0)
					strToMarcAlias = _T("100$a");
				else if (m_pMarcMgr->GetMarcGroup() == _T("����"))
					strToMarcAlias = _T("710$a");
			}
			else if (strInd == _T("00"))
			{
				if (m_pMarcMgr->GetMarcGroup() == _T("����"))
					strToMarcAlias = _T("700$a");
				else if (m_pMarcMgr->GetMarcGroup().Find(_T("�񵵼�")) >= 0)
					strToMarcAlias = _T("700$a");
				else if (m_pMarcMgr->GetMarcGroup() == _T("����"))
					strToMarcAlias = _T("");
			}
			else if (strInd == _T("20"))
			{
				if (m_pMarcMgr->GetMarcGroup() == _T("����"))
					strToMarcAlias = _T("100$a");
				else if (m_pMarcMgr->GetMarcGroup().Find(_T("�񵵼�")) >= 0)
					strToMarcAlias = _T("700$a");
				else if (m_pMarcMgr->GetMarcGroup() == _T("����"))
					strToMarcAlias = _T("");
			}
			*/

			if (strToMarcAlias.IsEmpty()) continue;

			// �ߺ�üũ

			/// 100$a �϶�  700$a�� ���� �����Ͱ� ������ �� �����͸� �����.
			if (strToMarcAlias == _T("100$a"))
			{
				if (IsDupMarcItem(_T("700$a"), strData)) 
					m_pMarcMgr->DeleteItem(m_pMarc, _T("700$a"), strData);

				// 100$a, 110$, 111$aüũ ������ ���� ����
				if (m_pMarcMgr->GetItem(m_pMarc, _T("100$a"), strTemp) >= 0) continue;
				if (m_pMarcMgr->GetItem(m_pMarc, _T("110$a"), strTemp) >= 0) continue;
				if (m_pMarcMgr->GetItem(m_pMarc, _T("111$a"), strTemp) >= 0) continue;
			}
			
			if (IsDupMarcItem(strToMarcAlias, strData)) continue;
	
			// �����Ѵ�.
			m_pMarcMgr->SetItem(m_pMarc, strToMarcAlias, strData);
		}
	}


	/**
		245$e -> 700$a
	*/
	strArrFrom.RemoveAll();

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("245$e"), strData, &strArrFrom);	
	if (ids >= 0)
	{
		nCountFrom = strArrFrom.GetSize();
		for (INT idx = 0; idx < nCountFrom; idx++)
		{
			strData = strArrFrom.GetAt(idx);
			strData.TrimLeft();
			strData.TrimRight();
			
			INT nFind = strData.Find(_T(": "));
			if (nFind > 0)
			{
				CString str1 = strData.Mid(nFind+2);
				strData = str1;
			}
			// ���Ҿ �����Ѵ�.
			nFind = strData.ReverseFind(' ');
			if (nFind > 0)
			{
				CString str1 = strData.Mid(nFind);
				CString str2 = str1;
				str1.TrimLeft();
				str1.TrimRight();
				
				if (m_CharSetMgr_UnlessWord.Find(str1, str2) == 0)
				{
					strData.Delete(nFind, strData.GetLength() - nFind);
				}
			}

			
			// ���� -> �ѱ�
			ConvertHanjaToHangul(strData);

			strData.TrimLeft();
			strData.TrimRight();

			if (strData.IsEmpty()) continue;
			
			// ','�� ������� ��ġ�Ѵ�.
			if ( bAuthorChangeCheck )
			{
				nFind = strData.Find(',');
				if (nFind > 0)
				{
					strTemp = strData;
					strData.Format(_T("%s, %s"), strData.Mid(nFind + 1), strData.Mid(0, nFind));
				}
				strData.TrimLeft();
				strData.TrimRight();
			}			

			// �����ϴ��� üũ�Ѵ�.
			if (IsDupMarcItem(_T("700$a"), strData)) continue;
			if (IsDupMarcItem(_T("100$a"), strData)) continue;
			
			// �����Ѵ�.
			m_pMarcMgr->SetItem(m_pMarc, _T("700$a"), strData);
		}
	}

	/**
		490 $a, $v -> 830 $a, $v
	*/

	CTag *pTag = NULL;
	CString str490a;
	CString str490v;
	CString str260b;

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("490$a"), str490a);
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("490$v"), str490v);
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("260$b"), str260b);
	pTag = m_pMarc->FindTag(_T("490"));
	
	if (pTag && pTag->GetFirstIndicator() == _T("1"))
	{
		if(!str490a.IsEmpty())
		{
			strData = str490a;
			if (!str260b.IsEmpty())
			{
				strData += _T("(") + str260b + _T(")");
			}

			// �ߺ�üũ
			if (!IsDupMarcItem(_T("830$a"), strData))
				m_pMarcMgr->SetItem(m_pMarc, _T("830$a"), strData);

			if (!str490v.IsEmpty())
			{
				// �ߺ�üũ
				if (!IsDupMarcItem(_T("830$v"), str490v))
					m_pMarcMgr->SetItem(m_pMarc, _T("830$v"), str490v);
			}
		}
	}

	
	/**
		505
	*/
	
	// 505$t -> 740$a	�ݺ��ױ�
	strArrFrom.RemoveAll();
	strArrInd.RemoveAll();

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("505$t"), strData, &strArrFrom, &strArrInd);	
	if (ids >= 0)
	{
		nCountFrom = strArrFrom.GetSize();
		for (INT idx = 0; idx < nCountFrom; idx++)
		{
			strData = strArrFrom.GetAt(idx);
			strData.TrimLeft();
			strData.TrimRight();

			// ���Ҿ �����Ѵ�.
			INT nFind = strData.ReverseFind(' ');
			if (nFind > 0)
			{
				CString str1 = strData.Mid(nFind);
				CString str2 = str1;
				str1.TrimLeft();
				str1.TrimRight();
				
				if (m_CharSetMgr_UnlessWord.Find(str1, str2) == 0)
				{
					strData.Delete(nFind, strData.GetLength() - nFind);
				}
			}

			// ���� . -�� ������ ����
			if (strData.Right(3) == _T(". -"))
				strData.Delete(strData.GetLength() - 3, 3);	
			
			if (strData.Right(3) == _T(" --"))
				strData.Delete(strData.GetLength() - 3, 3);	

			if (strData.Right(2) == _T("--"))
				strData.Delete(strData.GetLength() - 2, 2);

			// ��2���ñ�ȣ�� 1�ϰ�� ��Ī ����
			//if (strArrInd.GetAt(idx).Right(1) == _T("1"))
			// RemoveAllParenthesis(strData);
			//===================================================
			


			// ���� -> �ѱ�
			ConvertHanjaToHangul(strData);

			strData.TrimLeft();
			strData.TrimRight();

			if (strData.IsEmpty()) continue;
			
			// �����ϴ��� üũ�Ѵ�.
			//===================================================
			//2009.05.20 REM BY PJW : 505$t�� ��Ī�� ������쵵 �������� ��ü �ؽ�Ʈ������ üũ�ϴµ��� ������ ����.
// 			if (IsDupMarcItem(_T("740$a"), strData)) continue;
			//===================================================


			//==============================================================
			//2008.08.29 UPDATE BY PJW :	1. '='�������������� ���	: 505$t -> 740 2$a
			//								2. '='�������Ұ��			: 505$t(A = B) -> (740 2$aA) , (940  $aB)	
// 			m_pMarcMgr->SetItem(m_pMarc, _T("740$a"), strData);

			CString strTemp;
			INT nPos;

			INT nFindChar; 
			CString strDataLeft;
			CString strDataRight;
			nFindChar = strData.Find(_T("="));
			if( 0 > nFindChar )
			{
				//===================================================
				//2009.05.20 UPDATE BY PJW : ��Ī�� �ִ°��� ����� 740$a�� �ִ´�.��
// 				m_pMarcMgr->SetItem(m_pMarc, _T("740$a"), strData);
				if (!IsDupMarcItem(_T("740$a"), strData)) 
				{
					m_pMarcMgr->SetItem(m_pMarc, _T("740$a"), strData);
				}
				
				nPos = strData.Find(_T(")"));
				if( 0 <= nPos)
				{
					strTemp = strData.Left(nPos);
					strTemp.Replace( _T(")"), _T("") );
					strTemp.Replace( _T("("), _T("") );

					strTemp.TrimLeft();	strTemp.TrimRight();

					if (!IsDupMarcItem(_T("740$a"), strTemp)) 
					{
						m_pMarcMgr->SetItem(m_pMarc, _T("740$a"), strTemp);
					}

					strTemp = strData.Mid(nPos);
					strTemp.Replace( _T(")"), _T("") );
					strTemp.Replace( _T("("), _T("") );
					
					strTemp.TrimLeft();	strTemp.TrimRight();					

					if (!IsDupMarcItem(_T("740$a"), strTemp)) 
					{
						m_pMarcMgr->SetItem(m_pMarc, _T("740$a"), strTemp);
					}
				}
				//===================================================
				
			}
			else
			{
				strDataLeft		=	strData.Left(nFindChar);
				strDataRight	=	strData.Mid(nFindChar+1);

				strDataLeft.TrimLeft()	;	strDataLeft.TrimRight()		;
				strDataRight.TrimLeft()	;	strDataRight.TrimRight()	;

				//===================================================
				//2009.05.20 UPDATE BY PJW : ��Ī�� �ִ°��� ����� 740$a�� �ִ´�.��
// 				m_pMarcMgr->SetItem(m_pMarc, _T("740$a"), strDataLeft)	;
				if (!IsDupMarcItem(_T("740$a"), strDataLeft)) 
				{
					m_pMarcMgr->SetItem(m_pMarc, _T("740$a"), strDataLeft);
				}
				
				nPos = strDataLeft.Find(_T(")"));
				if( 0 <= nPos)
				{
					strTemp = strDataLeft.Left(nPos);
					strTemp.Replace( _T(")"), _T("") );
					strTemp.Replace( _T("("), _T("") );

					strTemp.TrimLeft();	strTemp.TrimRight();
					
					if (!IsDupMarcItem(_T("740$a"), strTemp)) 
					{
						m_pMarcMgr->SetItem(m_pMarc, _T("740$a"), strTemp);
					}

					strTemp = strDataLeft.Mid(nPos);
					strTemp.Replace( _T(")"), _T("") );
					strTemp.Replace( _T("("), _T("") );

					strTemp.TrimLeft();	strTemp.TrimRight();

					if (!IsDupMarcItem(_T("740$a"), strTemp)) 
					{
						m_pMarcMgr->SetItem(m_pMarc, _T("740$a"), strTemp);
					}
				}
				//===================================================
				m_pMarcMgr->SetItem(m_pMarc, _T("940$a"), strDataRight)	;
			}
			//==============================================================
		}
	}

	// 505$d -> 700$a	100$a�� ������ �������� �ʴ´�.
	strArrFrom.RemoveAll();

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("505$d"), strData, &strArrFrom);
	if (ids >= 0)
	{
		nCountFrom = strArrFrom.GetSize();
		for (INT idx = 0; idx < nCountFrom; idx++)
		{
			strData = strArrFrom.GetAt(idx);
			strData.TrimLeft();
			strData.TrimRight();

			// ���� . -�� ������ ����
			if (strData.Right(3) == _T(". -"))
				strData.Delete(strData.GetLength() - 3, 3);

			if (strData.Right(3) == _T(" --"))
				strData.Delete(strData.GetLength() - 3, 3);

			if (strData.Right(2) == _T("--"))
				strData.Delete(strData.GetLength() - 2, 2);

			// ���� .�������� ����
			if ( strData.Right(1) == _T(".") ) {
				strData = strData.Left( strData.GetLength() -1 );
			}

			strData.Replace(_T("[��]"),_T(""));
			strData.Replace(_T("[��]"),_T(""));
			strData.Replace(_T("[et al.]"),_T(""));

			strData.Replace(_T("("), _T(""));
			strData.Replace(_T(")"), _T(""));				
			strData.Replace(_T("["), _T(""));
			strData.Replace(_T("]"), _T(""));

		    INT nFind = strData.Find(_T(": "));
			if (nFind > 0)
			{
				CString str1 = strData.Mid(nFind+2);
				strData = str1;
			}

			// ���Ҿ �����Ѵ�.
			nFind = strData.ReverseFind(' ');
			if (nFind > 0)
			{
				CString str1 = strData.Mid(nFind);
				CString str2 = str1;
				str1.TrimLeft();
				str1.TrimRight();
				
				if (m_CharSetMgr_UnlessWord.Find(str1, str2) == 0)
				{
					strData.Delete(nFind, strData.GetLength() - nFind);
				}
			}			
			
			// ���� -> �ѱ�
			ConvertHanjaToHangul(strData);

			strData.TrimLeft();
			strData.TrimRight();

			if (strData.IsEmpty()) continue;

			// ','�� ������� ��ġ�Ѵ�.
			if ( bAuthorChangeCheck )
			{
				nFind = strData.Find(',');
				if (nFind > 0)
				{
					strTemp = strData;
					strData.Format(_T("%s, %s"), strData.Mid(nFind + 1), strData.Mid(0, nFind));
				}
				strData.TrimLeft();
				strData.TrimRight();
			}

			

			// �����ϴ��� üũ�Ѵ�.
			if (IsDupMarcItem(_T("100$a"), strData)) continue;
			//==============================================================
			//2008.08.29 UPDATE BY PJW : 700$a -> 70012$a
//			if (IsDupMarcItem(_T("700$a"), strData)) continue;
//			m_pMarcMgr->SetItem(m_pMarc, _T("700$a"), strData);

			if (IsDupMarcItem(_T("70012$a"), strData)) continue;
			if (!strData.IsEmpty()) 
			{
				strField.Format (_T("%ca%s"),cCode, strData);
				strData.Format(_T("70012%s"),strField);
				m_pMarcMgr->InsertField(m_pMarc, strData);
			}			
			//==============================================================
		}
	}

	// 505$e -> 700$a
	strArrFrom.RemoveAll();

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("505$e"), strData, &strArrFrom);
	if (ids >= 0)
	{
		nCountFrom = strArrFrom.GetSize();
		for (INT idx = 0; idx < nCountFrom; idx++)
		{
			strData = strArrFrom.GetAt(idx);
			strData.TrimLeft();
			strData.TrimRight();

			// ���� . -�� ������ ����
			if (strData.Right(3) == _T(". -"))
				strData.Delete(strData.GetLength() - 3, 3);

			if (strData.Right(3) == _T(" --"))
				strData.Delete(strData.GetLength() - 3, 3);

			if (strData.Right(2) == _T("--"))
				strData.Delete(strData.GetLength() - 2, 2);

			// ���� .�������� ����
			if ( strData.Right(1) == _T(".") ) {
				strData = strData.Left( strData.GetLength() -1 );
			}

			strData.Replace(_T("[��]"),_T(""));
			strData.Replace(_T("[��]"),_T(""));
			strData.Replace(_T("[et al.]"),_T(""));

			strData.Replace(_T("("), _T(""));
			strData.Replace(_T(")"), _T(""));				
			strData.Replace(_T("["), _T(""));
			strData.Replace(_T("]"), _T(""));

		    INT nFind = strData.Find(_T(": "));
			if (nFind > 0)
			{
				CString str1 = strData.Mid(nFind+2);
				strData = str1;
			}

			// ���Ҿ �����Ѵ�.
			nFind = strData.ReverseFind(' ');
			if (nFind > 0)
			{
				CString str1 = strData.Mid(nFind);
				CString str2 = str1;
				str1.TrimLeft();
				str1.TrimRight();
				
				if (m_CharSetMgr_UnlessWord.Find(str1, str2) == 0)
				{
					strData.Delete(nFind, strData.GetLength() - nFind);
				}
			}

			// ���� -> �ѱ�
			ConvertHanjaToHangul(strData);

			strData.TrimLeft();
			strData.TrimRight();

			if (strData.IsEmpty()) continue;

			// �����ϴ��� üũ�Ѵ�.
			if (IsDupMarcItem(_T("100$a"), strData)) continue;
			//==============================================================
			//2008.08.29 UPDATE BY PJW : 700$a -> 70012$a
//			if (IsDupMarcItem(_T("700$a"), strData)) continue;
//			m_pMarcMgr->SetItem(m_pMarc, _T("700$a"), strData);

			if (IsDupMarcItem(_T("70012$a"), strData)) continue;
			if (!strData.IsEmpty()) 
			{
				strField.Format (_T("%ca%s"),cCode, strData);
				strData.Format(_T("70012%s"),strField);
				m_pMarcMgr->InsertField(m_pMarc, strData);
			}			
			//==============================================================
		}
	}


	/** 
		511$a -> 700$a

		':' ���� ������ �����Ѵ�.
		',' or ';' �̸� �߶� ���� 700$a�� ����

	*/
	strArrFrom.RemoveAll();
		
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("511$a"), strData, &strArrFrom);
	if (ids >= 0)
	{
		INT nFind = -1;
		CString strToMarcAlias = _T("700$a");
		LPTSTR lpszToken = NULL;
		
		nCountFrom = strArrFrom.GetSize();
		for (INT idx = 0; idx < nCountFrom; idx++)
		{	
			strData = strArrFrom.GetAt(idx);
			strData.TrimLeft();
			strData.TrimRight();

			// �ϴ� : ���� ������ �߶������.
			nFind = strData.ReverseFind(':');
			if (nFind >= 0)
			{
				if (nFind == strData.GetLength() - 1) continue;

				strData = strData.Mid(nFind + 1);
			}

			// ���Ҿ �����Ѵ�.
			nFind = strData.ReverseFind(' ');
			if (nFind > 0)
			{
				CString str1 = strData.Mid(nFind);
				CString str2 = str1;
				str1.TrimLeft();
				str1.TrimRight();
				
				if (m_CharSetMgr_UnlessWord.Find(str1, str2) == 0)
				{
					strData.Delete(nFind, strData.GetLength() - nFind);
				}
			}

			// ���� -> �ѱ�
			ConvertHanjaToHangul(strData);
			
			strData.TrimLeft();
			strData.TrimRight();

			if (strData.IsEmpty()) continue;

			// ','�� ������� ��ġ�Ѵ�.
			nFind = strData.Find(',');
			if (nFind > 0)
			{
				strTemp = strData;
				strData.Format(_T("%s, %s"), strData.Mid(nFind + 1), strData.Mid(0, nFind));
			}

			strData.TrimLeft();
			strData.TrimRight();

			if (strData.IsEmpty()) continue;

			// ',', ';'�� ����� ����
			lpszToken = _tcstok(strData.GetBuffer(0), _T(",;"));
			while (lpszToken)
			{
				// �ߺ�üũ
				if (!IsDupMarcItem(_T("700$a"), lpszToken) && !IsDupMarcItem(_T("100$a"), lpszToken))
				{
					// �����Ѵ�.
					m_pMarcMgr->SetItem(m_pMarc, strToMarcAlias, lpszToken);
				}

				lpszToken = _tcstok(NULL, _T(",;"));
			}
		}
	}

	//==============================================================
	//2008.08.29 ADD BY PJW : 501($c$d$e$x) �����߰�
	/** 
		501$c -> 740 2$a
		501$d -> 70012$a
		501$e -> 70012$a
		501$x -> 740 2$a
		CTag *pTag = NULL;
	*/
	strArrFrom.RemoveAll();
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("501$c"), strData);

	if (!strData.IsEmpty()) 
	{
		
 		m_pMarcMgr->SetItem(m_pMarc, _T("740$a"), strData);
	}

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("501$d"), strData);

	if (!strData.IsEmpty()) 
	{
		
		strField.Format (_T("%ca%s"),cCode, strData);
		strData.Format(_T("70012%s"),strField);
		m_pMarcMgr->InsertField(m_pMarc, strData);
	}
	

  	ids = m_pMarcMgr->GetItem(m_pMarc, _T("501$e"), strData);

	if (!strData.IsEmpty()) 
	{
		strField.Format (_T("%ca%s"),cCode, strData);
		strData.Format(_T("70012%s"),strField);
		m_pMarcMgr->InsertField(m_pMarc, strData);
	}

 	ids = m_pMarcMgr->GetItem(m_pMarc, _T("501$x"), strData);
	if (!strData.IsEmpty()) 
	{
 		m_pMarcMgr->SetItem(m_pMarc, _T("740$a"), strData);
	}
	//==============================================================

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2006.12.10 Ȯ�尳�� : kcr4����
	// Leader ��ϱ�������� K�̸� 245$b -> 740���� ����
	//===================================================
	//2009.10.14 REM BY PJW : ���ռ����� ���� ��Ģ�� �ǰ��Ͽ� 245$b������ ������ ���ǵǾ����� �������� �����Ѵ�.
//	strArrFrom.RemoveAll();
//	CString strLeader;
//	m_pMarc->GetLeader(strLeader);
//	if( _T("c") == strLeader.Mid(18,1) || _T(" ") == strLeader.Mid(18,1) )
//	{
//		ids = m_pMarcMgr->GetItem(m_pMarc, _T("245$b"), strData, &strArrFrom);	
//		if (ids >= 0)
//		{
//			nCountFrom = strArrFrom.GetSize();
//			for (INT idx = 0; idx < nCountFrom; idx++)
//			{
//				strData = strArrFrom.GetAt(idx);
//				strData.TrimLeft();
//				strData.TrimRight();
//
//				// ���Ҿ �����Ѵ�.
//				INT nFind = strData.ReverseFind(' ');
//				if (nFind > 0)
//				{
//					CString str1 = strData.Mid(nFind);
//					CString str2 = str1;
//					str1.TrimLeft();
//					str1.TrimRight();
//					
//					if (m_CharSetMgr_UnlessWord.Find(str1, str2) == 0)
//					{
//						strData.Delete(nFind, strData.GetLength() - nFind);
//					}
//				}
//
//				
//				// ���� -> �ѱ�
//				ConvertHanjaToHangul(strData);
//
//				strData.TrimLeft();
//				strData.TrimRight();
//
//				if (strData.IsEmpty()) continue;
//				
//				// ','�� ������� ��ġ�Ѵ�.
//				if ( bAuthorChangeCheck )
//				{
//					nFind = strData.Find(',');
//					if (nFind > 0)
//					{
//						strTemp = strData;
//						strData.Format(_T("%s, %s"), strData.Mid(nFind + 1), strData.Mid(0, nFind));
//					}
//					strData.TrimLeft();
//					strData.TrimRight();
//				}			
//
//				// �����ϴ��� üũ�Ѵ�.
//				if (IsDupMarcItem(_T("740$a"), strData)) continue;
//								
//				// �����Ѵ�.
//				m_pMarcMgr->SetItem(m_pMarc, _T("740$a"), strData);
//			}
//		}
//	}
	//===================================================
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	return 0;

	EFS_END

	return -1;
}

INT CMarcOperation::Operation_Equal()
{
	EFS_BEGIN

	INT ids;
	//CArray<CString, CString&> arrayItem;
	CString strItemData;
	BOOL bMatch = FALSE;

	CString strBasicParam;
	strBasicParam.Format(_T("%s"), m_pMarcInstruction->m_pszBasicParam);

	ids = m_pMarcMgr->GetItem(m_pMarc, strBasicParam, strItemData);
	if (ids >= 0)
	{
		INT nCount = m_arrayParam.GetSize();
		for (INT i = 0; i < nCount; i++)
		{
			CString strCompItemData;
			ids = m_pMarcMgr->GetItem(m_pMarc, m_arrayParam.GetAt(i), strCompItemData);
			if (ids >= 0)
			{
				bMatch = TRUE;

				if (strItemData == strCompItemData)
					return 1;
			}
		}

		if (!bMatch)
			return 0;
	}
	else
		return 0;
	
	m_strLastError.Format(_T("%s �� %s�� ���߾� �˴ϴ�"), strBasicParam, m_pMarcInstruction->m_pszParam);
	m_strLastHelp.Format(_T("%s"), m_pMarcInstruction->m_pszDesc);
	return -1;

	EFS_END

	return -1;
}

INT CMarcOperation::Operation_Plus()
{
	return TRUE;
}

INT CMarcOperation::GetAuthorCodeChangeFlag(BOOL &f_bFlag)
{
	CStdioFile	file;
	CString		strFileName;
	CString		strLine;
	BOOL		bCheck;

	bCheck = TRUE;
	strFileName = _T("..\\CFG\\����ġȯ����.cfg");
	if(!file.Open (strFileName,CFile::modeRead|CFile::typeBinary) ) 
	{
		f_bFlag = TRUE;
		return -1;
	}

	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	while(file.ReadString (strLine))
	{
		if ( strLine == _T("N") )
		{
			bCheck = FALSE;
			break;
		}
	}

	f_bFlag = bCheck;

	return 0;	
}

INT CMarcOperation::ConvertHanjaToHangul(CString &strSrc)
{
	EFS_BEGIN	

	if ( strSrc == "" ) return 0;
	
	CHAR		*UTF8String;
	_TCHAR		*OutUTF16;
	INT			ids;
	INT			nLen;	
	
	ids = m_charConvert.UnicodeToUTF8(strSrc.GetBuffer(0) , &UTF8String, &nLen);
	ids = m_charConvert.Hanja2Hangul(UTF8String);
	ids = m_charConvert.UTF8ToUnicode(UTF8String, &OutUTF16, &nLen);
	
	strSrc.Format(_T("%s"), OutUTF16 );
	
	free(UTF8String);
	free(OutUTF16);

	
	return 0;

	EFS_END
	return -1;

}