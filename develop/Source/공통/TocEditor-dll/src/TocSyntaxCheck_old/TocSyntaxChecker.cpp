// TocSyntaxChecker.cpp: implementation of the CTocSyntaxChecker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TocSyntaxChecker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTocSyntaxChecker::CTocSyntaxChecker(CErrorQueue* pErrorQueue /*= NULL*/, CString sWorkDir/*=_T("")*/, INT nIsKindOf/*=_DAN_SINGLE*/)
{
	m_TocMetaData.Init(sWorkDir,nIsKindOf);
	m_pErrorQueue = pErrorQueue;
	m_arrTocTag.SetSize(0,1);

	m_pTocFile = NULL ;
	m_iCurLine = 0;
}

CTocSyntaxChecker::~CTocSyntaxChecker()
{
	INT iMaxCount = m_arrTocTag.GetSize();
	CTocTag* pTocTag = NULL;		

	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		pTocTag = m_arrTocTag.GetAt(i);
		delete pTocTag;
	}

	m_arrTocTag.RemoveAll();
	m_arrTocTag.FreeExtra();

	if( m_pTocFile!=NULL )	delete m_pTocFile ;
}

// ================================================================================================================
// Check Toc Syntax
// �Էµ� ���Ϸ� ���� Syntax Check�� �ǽ��Ѵ�.
// 1. Toc ������ CTocTag Class�� ���� ����ȭ
// 2. CTocTag Class�κ��� �ϰ������� Syntax Check �� Error Queue ����
// ================================================================================================================
INT CTocSyntaxChecker::CheckSyntax(CString sSrcFileFullPath, BOOLEAN bIsMessageDlg /*= FALSE*/)
{
	sSrcFileFullPath.TrimLeft();
	sSrcFileFullPath.TrimRight();

	if(sSrcFileFullPath.IsEmpty()) return -1;

	CFileException ex ;
	CStdioFile SrcFile ;
	if ( SrcFile.Open(sSrcFileFullPath.GetBuffer(0), CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary , &ex)==NULL ) {
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		AfxMessageBox(szError);
		return -2 ;
	}
	TCHAR cUni;
	SrcFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		SrcFile.SeekToBegin();
	}

	CString sRecord;
//	INT iCurLine = 0;

	// ================================================================================================================
	// 1. Toc ������ CTocTag Class�� ���� ����ȭ
	// Toc ���� ���Ϸκ��� 1Line�� Read�Ͽ� ���������� ����ȭ
	// ================================================================================================================
//	CTocTag* pTocFile = NULL;
	CTocTag* pBeforeTocTag = NULL;
	CTocTag* pBaseTocTag = NULL;
	CTocTag* pCurrentTocTag = NULL;
	m_pTocFile = pBaseTocTag = new CTocTag(m_pErrorQueue,_T("������Ÿ����"),CTocTagInfo::TAG_TYPE_MULTI,CTocTagInfo::LEVEL0,FALSE);

	CArray<CTocTag*,CTocTag*> m_arrBaseTag;			// Base Tag

	INT iReturnType = 0;

	CString sMokcha ;
	sMokcha.Empty() ;
	CString sToken;
	while(SrcFile.ReadString(sRecord))
	{
		m_iCurLine++;
//		sRecord.TrimLeft();		// 2002.1.30 deleted
//		sRecord.TrimRight();

		if(sRecord.IsEmpty()) continue;

		sMokcha= sMokcha+sRecord+_T("\r\n") ;

		// ================================================================================================================
		// Record�� �о _T("<")�� Parsing�Ͽ� ���� Tag���� �߶� �Ѱ��ش�.
		// ================================================================================================================
		while(!sRecord.IsEmpty())
		{
			sToken = GetParsedTocToken(sRecord);

			// �ʼ� �±� üũ
			if (( !sToken.Compare(_T("id")) && m_sRightToken.IsEmpty() ))
			{
				CString sErrorMessage;
				sErrorMessage.Format(_T("%s ��ũ�� �ݵ�� ������ ������ �����ؾ��մϴ�."),sToken);
				(m_pTocFile->m_pErrorQueue) -> AddErrorMessage(m_iCurLine, sErrorMessage, _T(""));
			}

			m_sRightToken.Empty() ;
			// ================================================================================================================
			// Tag���� �����͸� Parsing
			// ================================================================================================================		

			iReturnType = m_TocMetaData.GetTocTag(m_pErrorQueue, m_iCurLine, sToken, &pCurrentTocTag, pBaseTocTag);

			if(iReturnType < 0) {
				AfxMessageBox(_T("�����˻縦 �����ϴ��� �ý��� ���ο� ������ �߻��Ͽ����ϴ�."));
				SrcFile.Close() ;
				return -3;
			}
			
			// ================================================================================================================
			// �����ȣ�� ������ �д�.
			// ================================================================================================================
			/*
			if(pCurrentTocTag -> IsControlId()) {
				m_sControlNo = pCurrentTocTag -> GetValue();
			}
			*/

			// ���� ������ Link
			// ================================================================================================================
			// Return Type�� ���� ó�� ���
			// �������̶�� <TAG>�� </TAG>�� ������ ��Ÿ���� �Ѵ�.
			// CASE 1 : <TAG> <TAG> </TAG> 
			// ================================================================================================================
			if(iReturnType == CTocMetaData::NEW_BASE_TOC) {
				pBaseTocTag->AddSubLevelToc(pCurrentTocTag);
				m_arrBaseTag.Add(pBaseTocTag);
				pBaseTocTag = pCurrentTocTag;
			} 

			if(iReturnType == CTocMetaData::END_BASE_TOC) {
				INT iMaxCount = m_arrBaseTag.GetSize();
				pBaseTocTag = m_arrBaseTag.GetAt(iMaxCount - 1);
				m_arrBaseTag.RemoveAt(iMaxCount - 1);
			}

			if(iReturnType == CTocMetaData::CURRENT_TOC) {
				pBaseTocTag->AddSubLevelToc(pCurrentTocTag);
				continue;
			}
		}
	}

	m_arrBaseTag.RemoveAll();
	m_arrBaseTag.FreeExtra();

	m_pTocFile -> CheckTocSyntax(m_TocMetaData.m_nIsKindOf, m_TocMetaData.m_arrTocTagList);

	INT iStartPage = -1;
	INT iEndPage = -1;
//	m_pTocFile -> CheckPage(-1,iStartPage,iEndPage);

	CheckEssentialTag(sMokcha) ;	// �ʼ� �±� üũ!

	SrcFile.Close() ;
	return 0 ;
}

CString CTocSyntaxChecker::GetParsedTocToken(CString &sSource)
{
	// ================================================================================================================
	// Record�� �о _T("<")�� Parsing�Ͽ� ���� Tag���� �߶� �Ѱ��ش�.
	// <PAGE ><TI>afasdf<TI/>
	// ================================================================================================================

	sSource.TrimLeft();
	sSource.TrimRight();

	if(sSource.IsEmpty()) {
		return _T("");
	}

	CString sLeftToken;
	CString sRightToken;

	INT iCurIdx = sSource.Find(_T("<"));
	if(iCurIdx == 0) {
		sRightToken = sSource.Right(sSource.GetLength() - iCurIdx - 1);
//		sRightToken.TrimLeft();
//		sRightToken.TrimRight();

		iCurIdx = sRightToken.Find(_T(">"));
		if(iCurIdx >= 0) {
			sLeftToken = sRightToken.Left(iCurIdx);
//			sLeftToken.TrimLeft();
//			sLeftToken.TrimRight();

			if(m_TocMetaData.IsTocTag(sLeftToken)) {
				// ================================================================================================================
				// ����_T("<")�� ã�´�.
				// ================================================================================================================
				sRightToken = sRightToken.Right(sRightToken.GetLength() - iCurIdx - 1);

				iCurIdx = sRightToken.Find(_T("<"));
				if(iCurIdx >= 0) {
					sSource = sRightToken.Right(sRightToken.GetLength() - iCurIdx);
				} else {
					sSource.Empty();
					m_sRightToken= sRightToken;
//					sSource= sRightToken ;
				}

				// added	2002.1.30
				if ( !sLeftToken.IsEmpty() && sLeftToken.Mid(0,1)==_T("/") && !sRightToken.IsEmpty() )
				{
					CString sErrorMessage;
					sErrorMessage.Format(_T("��ħ ��ũ �������� ������ �����ؼ��� �ȵ˴ϴ�."));
					(m_pTocFile->m_pErrorQueue) -> AddErrorMessage(m_iCurLine, sErrorMessage, _T(""));
				}

				sLeftToken.TrimLeft();
				sLeftToken.TrimRight();

				iCurIdx = sLeftToken.Find(_T(" "));
				if(iCurIdx >= 0) {
					sLeftToken = sLeftToken.Left(iCurIdx);
					sLeftToken.TrimLeft();
					sLeftToken.TrimRight();
				}
				return sLeftToken;
			} else {
				sLeftToken = sSource;
				sSource.Empty();
				return sLeftToken;
			}
		} else {
			sLeftToken = sSource;
			sSource.Empty();
			return sLeftToken;
		}
	} else {
//		sLeftToken = sSource;
		sLeftToken = _T(".") + sSource;	// �±� ���� body , id ���� ������ �ɷ��ֱ� ���ؼ�
		sSource.Empty();
		return sLeftToken;
	}

}

VOID CTocSyntaxChecker::CheckEssentialTag(CString& sMokcha)
{
/*	// ��� ��ũ üũ
	INT nCnt= (m_TocMetaData.m_arrTocTagList).GetSize() ;

	CString sTag ;
	CString sErrorMessage ;
	INT nIdx= 0 ;
	for ( INT i=0 ; i<nCnt ; i++ )
	{
		sTag= (m_TocMetaData.m_arrTocTagList).GetAt(i) ;
		if ( sTag.IsEmpty() )	continue ;

		sTag.TrimLeft() ;
		sTag.TrimRight() ;
		sTag= _T("<") + sTag  + _T(">") ;

		nIdx= sMokcha.Find(sTag) ;
		if ( nIdx==0 )	continue ;		// 2002.1.30.	modified 
		else if ( nIdx==1 || nIdx<0 || (sMokcha.Mid(nIdx-2,2 )!=_T("\r\n")) ) {
			sErrorMessage.Format(_T("%s �±װ� �����ϴ�."),sTag);
			(m_pTocFile->m_pErrorQueue) -> AddErrorMessage(m_iCurLine, sErrorMessage, _T(""));
			continue ;
		}
	}
*/

	// id �� count, body, /body �� üũ
	const INT iElementCnt = 4;
	CString sTag[iElementCnt];

	if(m_TocMetaData.m_nIsKindOf == _KISA_) {
		sTag[0].Format(_T("<id>"));
		sTag[1].Format(_T("<vol>"));
		sTag[2].Format(_T("<body>"));
		sTag[3].Format(_T("</body>"));
	} else {
		sTag[0].Format(_T("<id>"));
		sTag[1].Format(_T("<count>"));
		sTag[2].Format(_T("<body>"));
		sTag[3].Format(_T("</body>"));
	}

	CString sErrorMessage;
	INT nIdx= 0 ;
	for ( INT i=0 ; i<iElementCnt ; i++ )
	{
		nIdx= sMokcha.Find(sTag[i]) ;
		if ( nIdx==0 )	continue ;		// 2002.1.30.	modified 
		else if ( nIdx==1 || nIdx<0 || (sMokcha.Mid(nIdx-2,2 )!=_T("\r\n")) ) {
			sErrorMessage.Format(_T("%s �±װ� �����ϴ�."),sTag[i]);
			(m_pTocFile->m_pErrorQueue) -> AddErrorMessage(m_iCurLine, sErrorMessage, _T(""));
			continue ;
		}
	}

}
