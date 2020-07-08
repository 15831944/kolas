// TocMetaData.cpp: implementation of the CTocMetaData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TocMetaData.h"
#include "TocTag.h"
#include "TocParse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
INT CTocMetaData::NEW_BASE_TOC = 0;			// 0
INT CTocMetaData::CURRENT_TOC  = 1;			// 1
INT CTocMetaData::END_BASE_TOC = 2;			// 2

CTocMetaData::CTocMetaData()
{
	m_nIsKindOf= _DAN_SINGLE ;
}

VOID CTocMetaData::Init(CString sWorkingDir, INT nIsKindOf/*=_DAN_SINGLE*/)
{
	sWorkingDir.TrimLeft();
	sWorkingDir.TrimRight(_T("\\"));
	sWorkingDir+=_T("\\");
//	CString metafile= sWorkingDir+sMetaFileName;
	CString metafile; //= sWorkingDir+sMetaFileName;

	switch(nIsKindOf) {
	case _DAN_SINGLE:
		metafile.Format(_T("%s%s"), sWorkingDir, sMetaFileDan);
		break;

	case _KISA_:
		metafile.Format(_T("%s%s"), sWorkingDir, sMetaFileKisa);
		break;

	default:
	case _YON_:
		metafile.Format(_T("%s%s"), sWorkingDir, sMetaFileYon);
		break;

	}

	CString tagfile= sWorkingDir+sTagListFileName;

	CFileException ex;
	if( ! m_MetaCfgFile.Open(metafile,CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary, &ex) ) {
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		CString strError ;
		strError += _T("Couldn't open source file:\r\n\r\n");
		strError += szError;
		AfxMessageBox(strError) ;
		return ;
	}
	TCHAR cUni;
	m_MetaCfgFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		m_MetaCfgFile.SeekToBegin();
	}
	if( ! m_TagListFile.Open(tagfile,CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary, &ex) ) {
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		CString strError ;
		strError += _T("Couldn't open source file:\r\n\r\n");
		strError += szError;
		AfxMessageBox(strError) ;
		return ;
	}
	TCHAR cUni;
	m_TagListFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		m_TagListFile.SeekToBegin();
	}

	m_arrTocTagList.SetSize(0,1);
	// =================================================================================================
	// Tag List�κ��� Tag List�� Init�Ѵ�.
	// =================================================================================================

	m_nIsKindOf= nIsKindOf ;

	CString sRecord;
	while(m_TagListFile.ReadString(sRecord))
	{
		if(sRecord.IsEmpty()) continue;

		sRecord.TrimLeft();
		sRecord.TrimRight();

		if (m_nIsKindOf==_DAN_SINGLE) {			// ���ົ-�ܱ�
			if( sRecord!=_T(".���ົ-�ܱ�") )	continue ;
			break ;
		}
		else if (m_nIsKindOf==_DAN_MULTI) {	// ���ົ-�ٱ�
			if( sRecord!=_T(".���ົ-�ٱ�") )	continue ;
			break ;
		}
		else if (m_nIsKindOf==_YON_) {	// ���Ӱ��๰
			if( sRecord!=_T(".���Ӱ��๰") )	continue ;
			break ;
		}
		else if (m_nIsKindOf==_KISA_) {	// ���Ӱ��๰
			if( sRecord!=_T(".��繰") )	continue ;
			break ;
		}
	}

	while ( m_TagListFile.ReadString(sRecord) && (sRecord.Mid(0,1)!=_T("."))) {
		if(sRecord.IsEmpty()) continue;
		m_arrTocTagList.Add(sRecord);
	}

}

CTocMetaData::~CTocMetaData()
{
	m_arrTocTagList.RemoveAll();
	m_arrTocTagList.FreeExtra();
}

// ========================================================================================================
// ���� ���ڵ带 �Է¹޾Ƽ� Tag�� Parsing, ������ CTocTag�� �������ش�.
// ========================================================================================================

INT CTocMetaData::GetTocTag(CErrorQueue* pErrorQueue, INT iLine, CString sSourceRecord, CTocTag** pNewTocTag, CTocTag* pBaseTocTag /*= NULL*/)
{
	// =================================================================================================
	// Source Record���� ���� Tag�� �и��ϰ� ���ÿ� Tag�������� Page ���������� �Ǻ��Ѵ�.
	// =================================================================================================
	INT iType = IsTocTag(sSourceRecord) ? CTocTagInfo::TAG_TYPE_MULTI : CTocTagInfo::TAG_TYPE_PAGEINFO;
			
	INT iStartLine		= -1;
	INT iEndLine		= -1;

	CString sHeadTag	= _T("");
	CString sTailTag	= _T("");
	INT iTagType		= CTocTagInfo::TAG_TYPE_PAGEINFO;
	INT iLevel			= CTocTagInfo::LEVEL5;
	BOOLEAN bIsRepeatTag= TRUE;	

	INT iReturn = -1;
	CTocTag* pTocTagInfo = NULL;
	if(iType != CTocTagInfo::TAG_TYPE_PAGEINFO) {
		// =================================================================================================
		// Tag�� ���
		// =================================================================================================
		CString sRecord;
		CString sToken;
		TCHAR cUni;
		m_MetaCfgFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			m_MetaCfgFile.SeekToBegin();
		}
		while(m_MetaCfgFile.ReadString(sRecord)) 
		{
			sRecord.TrimLeft();
			sRecord.TrimRight();

			if(sRecord.IsEmpty()) continue;

			// =================================================================================================
			// �ּ� ����
			// =================================================================================================
			INT iCurIdx = sRecord.Find(_T(".."));
			if(iCurIdx >= 0) {
				sRecord = sRecord.Left(iCurIdx);
				sRecord.TrimLeft();
				sRecord.TrimRight();

				if(sRecord.IsEmpty()) {
					continue;
				}
			}

			// =================================================================================================
			// ���ڵ� ���� [TAG] | [TAGTYPE (0 ~ 2) ] | [LEVEL (0 ~ 5)] | [ISREPEATTAG(Y or N)]
			// =================================================================================================
			sHeadTag.Empty();
			sTailTag.Empty();

			CParse tokenParse(sRecord.GetBuffer(0));
			tokenParse.SetDelimeter(_T("|"));

			const INT iElementCnt = 4;
			CString sElement[iElementCnt];

			for(INT i = 0 ; tokenParse.HasMoreToken() && i < iElementCnt ; i++)
			{
				sElement[i] = tokenParse.NextToken();
				sElement[i].TrimLeft();
				sElement[i].TrimRight();
			}

			sHeadTag = sElement[0];
			sTailTag = _T("/")+sElement[0];

			sHeadTag.MakeLower();
			sTailTag.MakeLower();
			sSourceRecord.MakeLower();

			iTagType	 = _ttoi(sElement[1]);
			iLevel		 = _ttoi(sElement[2]);
			bIsRepeatTag = sElement[3] == _T("Y") ? TRUE : FALSE;

			if(sHeadTag.Compare(sSourceRecord) == 0) {
				// =================================================================================================
				// �ش� Tag�� ��� ��Ÿ ���� Setting
				// =================================================================================================
				sHeadTag	= sSourceRecord;
				iStartLine	= iLine;
				
				pTocTagInfo	 = new CTocTag(pErrorQueue,sSourceRecord, iTagType, iLevel, bIsRepeatTag);
				pTocTagInfo -> SetHeadTag(iStartLine, sHeadTag);
				if(iTagType == CTocTagInfo::TAG_TYPE_SINGLE) 
					iReturn		 = CURRENT_TOC;
				else
					iReturn		 = NEW_BASE_TOC;

				break;
			} else if(sTailTag.Compare(sSourceRecord) == 0) {
				// =================================================================================================
				// </...>�� ���� Tag�� ��� ���� ���� TocTag�� �����͸� �ִ´�.
				// =================================================================================================				
				if(pBaseTocTag -> IsCurrentPairTag(sTailTag)) {
					pBaseTocTag -> SetTailTag(iLine,sTailTag);
					iReturn	= END_BASE_TOC;
				} else {
					pTocTagInfo	 = new CTocTag(pErrorQueue,sSourceRecord, iTagType, iLevel, bIsRepeatTag);
					pTocTagInfo -> SetTailTag(iLine,sTailTag);
					iReturn		 = NEW_BASE_TOC;
				}

				break;
			}
		}
	} else {
		// =================================================================================================
		// Page�� ���
		// =================================================================================================
		pTocTagInfo = new CTocTag(  pErrorQueue,
									sSourceRecord, 
		    						CTocTagInfo::TAG_TYPE_PAGEINFO, 
									CTocTagInfo::LEVEL5, 
									TRUE );
		pTocTagInfo -> SetHeadTag(iLine,sSourceRecord);
		iReturn		 = CURRENT_TOC;
	}

	*pNewTocTag = pTocTagInfo;
	return iReturn;
}

// ===================================================================================================
// ���� ���Ϸκ��� Tag�� �и�
// [Return]
// ===================================================================================================
INT CTocMetaData::ParseTocTag(CString& sSourceRecord, CString &sTag)
{
	// ===================================================================================================
	// [STEP]
	// 1. _T("<")�� Scan
	// 2. _T(">")�� Scan
	// 3. TagList���� Tag�� Return;
	// ===================================================================================================
	sSourceRecord.TrimLeft() ;
	sSourceRecord.TrimRight() ;

	INT iCurIdx = sSourceRecord.Find(_T("<"));
	INT iStartIdx = iCurIdx;

	CString sLeftToken;
	CString sRightToken;
	if(iCurIdx == 0) {
		sRightToken = sSourceRecord.Right(sSourceRecord.GetLength() - iCurIdx - 1);

		if(sRightToken.IsEmpty()) return -1;

		iCurIdx = sRightToken.Find(_T(">"));
		if(iCurIdx < 0) return -2;

		sLeftToken = sRightToken.Left(iCurIdx);
//		sLeftToken.TrimLeft();
//		sLeftToken.TrimRight();

		sRightToken = sRightToken.Right(sRightToken.GetLength() - iCurIdx - 1);
		
		INT iMaxCount = m_arrTocTagList.GetSize();
		for(INT i = 0 ; i < iMaxCount ; i++)
		{
//			sLeftToken.MakeLower();		// �ҹ��ڸ� �����ϵ��� ������!
			if(sLeftToken.Compare(m_arrTocTagList.GetAt(i)) == 0) {
				sTag = sLeftToken;
				sSourceRecord = sRightToken;
				return iStartIdx;
			}
		}
	}

	return -3;
}

CString CTocMetaData::GetParsedToc(CString &sSourcRecord, CString sTag)
{
	return _T("");
}

BOOLEAN CTocMetaData::IsTocTag(CString sToken)
{
	INT iMaxCount = m_arrTocTagList.GetSize();

//	sToken.MakeLower();		// �ҹ��ڸ� �����ϵ��� ������!
	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		if(sToken.Compare(m_arrTocTagList.GetAt(i)) == 0) {
			return TRUE;
		}
	}

	return FALSE;
}

INT CTocMetaData::GetTagCount()
{
	return m_arrTocTagList.GetSize();
}

CString CTocMetaData::GetTag(INT iIdx)
{
	return m_arrTocTagList.GetAt(iIdx);
}
