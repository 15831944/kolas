// TocMetaData.cpp: implementation of the CTocMetaData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TocMetaData.h"
#include "TocTag.h"
#include "Parse.h"

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
	m_MetaCfgFile= NULL;
	m_TagListFile= NULL;
}

VOID CTocMetaData::Init(CString sWorkingDir)
{
	sWorkingDir.TrimLeft();
	sWorkingDir.TrimRight(TEXT('\\'));

	sWorkingDir = sWorkingDir.Left(sWorkingDir.ReverseFind(TEXT('\\') ));
	sWorkingDir += TEXT(_T("\\cfg\\"));



	CString metafile= sWorkingDir+sMetaFileName;
	CString tagfile= sWorkingDir+sTagListFileName;

//	m_MetaCfgFile.Open(metafile,CFile::modeRead | CFile::shareDenyWrite | CFile::typeText);
	m_MetaCfgFile= FdkFopen(metafile, _T("rb")) ;
//	m_TagListFile.Open(tagfile,CFile::modeRead | CFile::shareDenyWrite | CFile::typeText);
	m_TagListFile= FdkFopen(tagfile, _T("rb")) ;
	if( m_MetaCfgFile==NULL || m_TagListFile==NULL )
	{
		AfxMessageBox(_T("Cant'find src File1"));
		return;
	}
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode�϶� 2byte�ǳʶ��
	FILE* fpUni = m_MetaCfgFile;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode�϶� 2byte�ǳʶ��
	fpUni = m_TagListFile;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------	

	m_arrTocTagList.SetSize(0,1);
	// =================================================================================================
	// Tag List�κ��� Tag List�� Init�Ѵ�.
	// =================================================================================================
	static TCHAR szBuff[500];
	CString sRecord;
//	while(m_TagListFile.ReadString(sRecord))
	while (!feof(m_TagListFile))
	{
		FdkFgets(szBuff, 499, m_TagListFile);
		sRecord= szBuff ;
		sRecord.TrimRight();
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
//		m_MetaCfgFile.SeekToBegin();
		FdkFseek(m_MetaCfgFile,0, SEEK_SET);

		static TCHAR szBuff[500];
//		while(m_MetaCfgFile.ReadString(sRecord)) 
		while (!feof(m_MetaCfgFile))
		{
			FdkFgets(szBuff, 499, m_MetaCfgFile);
			sRecord= szBuff ;
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
	INT iCurIdx = sSourceRecord.Find(_T("<"));
	INT iStartIdx = iCurIdx;

	CString sLeftToken;
	CString sRightToken;
	if(iCurIdx >= 0) {
		sRightToken = sSourceRecord.Right(sSourceRecord.GetLength() - iCurIdx - 1);

		if(sRightToken.IsEmpty()) return -1;

		iCurIdx = sRightToken.Find(_T(">"));
		if(iCurIdx < 0) return -2;

		sLeftToken = sRightToken.Left(iCurIdx);
		sLeftToken.TrimLeft();
		sLeftToken.TrimRight();

		sRightToken = sRightToken.Right(sRightToken.GetLength() - iCurIdx - 1);
		
		INT iMaxCount = m_arrTocTagList.GetSize();
		for(INT i = 0 ; i < iMaxCount ; i++)
		{
			sLeftToken.MakeLower();
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

	sToken.MakeLower();
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
