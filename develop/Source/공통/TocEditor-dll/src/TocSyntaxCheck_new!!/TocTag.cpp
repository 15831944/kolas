// TocTag.cpp: implementation of the CTocTag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TocTag.h"
#include "Parse.h"
//#include _T("TiffFileManger.h")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

INT CTocTag::m_nTotalPage=0;
bool CTocTag::m_bPageCheck=true;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTocTag::CTocTag(	CErrorQueue* pErrorQueue,
					CString sSourceRecord,
					INT iTagType,
					INT iLevel,
					BOOLEAN bIsRepeatTag )
: CTocTagInfo(pErrorQueue, sSourceRecord, iTagType, iLevel, bIsRepeatTag)
{
	m_arrSubLevelTocTag.SetSize(0,1);
}

CTocTag::~CTocTag()
{
	CTocTagInfo* pTocTagInfo = NULL;
	INT iMaxCount = m_arrSubLevelTocTag.GetSize();

	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		delete pTocTagInfo;
	}

	m_arrSubLevelTocTag.RemoveAll();
	m_arrSubLevelTocTag.FreeExtra();
}

INT CTocTag::CheckTocSyntax()
{
	// ================================================================================================================
	// Tag�� ������ ������� �Ͽ� Syntax Check
	// [Step]
	// 1. Pair Tag�� ��� �� ���� Tag�� ��� �����ϴ°�?
	// 2. ���� ���� �߿��� �ݺ��������� ���� Tag�� �ݺ��Ǿ��°�?
	// 3. ���� ���� �߿��� �������� ������ ���谡 �¾ƶ������°�?
	// 4. Page ������ ��� ���������� ���Ŀ� �����ϴ°�?
	// 5. Tag���� ������ �ԷµǾ� ������ ������? ������ ������ �Է°����� Tag�ΰ�?
	// 6. SubToc�� ���� Syntax�˻�
	// ================================================================================================================

	INT iErrorLine = -1;
	CString sErrorMessage;
	// 1. Pair Tag�� ��� �� ���� Tag�� ��� �����ϴ°�?
	if(!CheckExistPairTag(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
	}
	// 2. ���� ���� �߿��� �ݺ��������� ���� Tag�� �ݺ��Ǿ��°�?
	if(!CheckRepeatTag(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
	}
	// 3. ���� ���� �߿��� �������� ������ ���谡 �¾ƶ������°�?
	if(!CheckTagLevel(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
	}
	// 4. Page ������ ��� ���������� ���Ŀ� �����ϴ°�?
	if(!CheckPageInfo(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
	}
	// 5. Tag���� ������ �ԷµǾ� ������ ������? ������ ������ �Է°����� Tag�ΰ�?
	if(!CheckTagSyntax(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
	}
	// ================================================================================================================
	// 6. SubToc�� ���� Syntax�˻�
	// ================================================================================================================
	INT iMaxCount = m_arrSubLevelTocTag.GetSize();
	CTocTagInfo* pTocTagInfo = NULL;
	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		((CTocTag*)pTocTagInfo)	-> CheckTocSyntax();
	}	
	return 0;
}

// ================================================================================================================
// Pair Tag�� ��� �� ���� Tag�� ��� �����ϴ°�?
// ================================================================================================================
BOOLEAN CTocTag::CheckExistPairTag(INT& iLine, CString& sErrorMessage)
{
	if(m_iLevel == LEVEL0) return TRUE;

	if(m_iTagType == TAG_TYPE_MULTI) {
		// ===========================================================================================================
		// Multi Tag�� ���
		// ===========================================================================================================
		m_sHeadTag.TrimLeft();
		m_sHeadTag.TrimRight();

		m_sTailTag.TrimLeft();
		m_sTailTag.TrimRight();

		if(m_sHeadTag.IsEmpty() || m_sTailTag.IsEmpty()) {
			if(m_sHeadTag.IsEmpty()) {
				iLine = m_iEndLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5007),m_sTailTag.GetBuffer(0));	//_T("<%s>�� ���� ¦�� �Ǵ� Tag�� �������� �ʽ��ϴ�.")
			} 

			if(m_sTailTag.IsEmpty()) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5008),m_sHeadTag.GetBuffer(0));	//_T("<%s>�� ���� ¦�� �Ǵ� Tag�� �������� �ʽ��ϴ�.")
			}
			
			return FALSE;
		} else {
			return TRUE;
		}
	} else if(m_iTagType == TAG_TYPE_SINGLE) {
		// ===========================================================================================================
		// Single Tag�� ��� ==> ������ TRUE
		// ===========================================================================================================		
		return TRUE;
	} else return TRUE;
}

// ================================================================================================================
// ���� ���� �߿��� �ݺ��������� ���� Tag�� �ݺ��Ǿ��°�?
// ================================================================================================================
BOOLEAN CTocTag::CheckRepeatTag(INT& iLine, CString& sErrorMessage)
{
	INT iMaxCount = m_arrSubLevelTocTag.GetSize();

	if(iMaxCount <= 0) return TRUE;

	if(m_iTagType == TAG_TYPE_PAGEINFO) return TRUE;

	CTocTagInfo* pTocTagInfo		= NULL;
	CTocTagInfo* pLowerTocTagInfo	= NULL;

	BOOLEAN bIsSameTag = FALSE;
	INT iErrorLine = -1;
	for(INT i = 0 ; i < iMaxCount; i++)
	{
		pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		if(pTocTagInfo == NULL) continue;

		if(!pTocTagInfo -> IsRepeatTag()) {
			bIsSameTag = FALSE;
			// ======================================================================================================
			// �ݺ� �������� ���� Tag�� ��� �Ʒ����� Tag�� �ݺ��Ǿ����� üũ
			// ======================================================================================================
			for(INT j = i + 1 ; j < iMaxCount ; j++)
			{
				pLowerTocTagInfo = m_arrSubLevelTocTag.GetAt(j);
				if(pLowerTocTagInfo == NULL) continue;

				if(pTocTagInfo -> GetHeadTag() == pLowerTocTagInfo -> GetHeadTag()) {
					bIsSameTag = TRUE;
					iErrorLine = pLowerTocTagInfo -> GetLine();
					break;
				}
			}

			if(bIsSameTag) {
				iLine = iErrorLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5009),pTocTagInfo->GetHeadTag());		//_T("%s TAG�� �ߺ��ؼ� ����� �� �����ϴ�.")
				return FALSE;
			}
		}
	}

	return TRUE;
}

// ================================================================================================================
// ���� ���� �߿��� �������� ������ ���谡 �¾ƶ������°�?
// ================================================================================================================
BOOLEAN CTocTag::CheckTagLevel(INT& iLine, CString& sErrorMessage)
{
	INT iMaxCount = m_arrSubLevelTocTag.GetSize();
	
	if(iMaxCount <= 0) return TRUE;

	CTocTagInfo* pTocTagInfo = NULL;
	
	INT iLevel = -1;

	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		if(pTocTagInfo == NULL) continue;

		iLevel = pTocTagInfo -> GetLevel();
		if(m_iLevel < iLevel) {
			/*
			iLine = pTocTagInfo -> GetLine();
			sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5010),m_sHeadTag,pTocTagInfo -> GetHeadTag());	//_T("%s TAG�� %s TAG�� ������ �� �����ϴ�.")
			return FALSE;
			*/
		}
	}

	return TRUE;
}

// ================================================================================================================
// Page ������ ��� ���������� ���Ŀ� �����ϴ°�?
// ================================================================================================================
BOOLEAN CTocTag::CheckPageInfo(INT& iLine, CString& sErrorMessage)
{
	if(m_iLevel == LEVEL0) return TRUE;
	// ================================================================================================================
	// �������� = å������, ��ĵ������, ����
	// 1. _T("=") �� �����ϴ��� üũ
	// 2. ���������� �����ϴ��� üũ
	// 3. ���������� ������ �����ϴ��� üũ
	// 4. ���������� ���� ���� üũ
	// ================================================================================================================

	// ====================================================================================================
	// 2001.9.11
	// _T("=") ���� ������ ������ ���� �ʾƵ� ������ �����ϵ��� �ٲܰ��̴�.
	// �̰��� ���ؼ� �ɼ��� �߰��ϸ� �ɵ�..
	// ====================================================================================================
	if(m_iTagType == TAG_TYPE_PAGEINFO) {
		// ====================================================================================================
		// ������ üũ�� �ϴ� ��� (default �ɼ�)
		// ====================================================================================================
		//CString t; t.Format(_T("%d"), m_bPageCheck); AfxMessageBox(t);
		if (m_bPageCheck) 
		{
			CString sIndex;
			// ============================================================================================================
			// 1. _T("=") �� �����ϴ��� üũ
			// ============================================================================================================
			INT iCurIdx = m_sHeadTag.Find(_T("="));
			if(iCurIdx < 0) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5011),iLine);		//_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. '='�� �����ϴ�.")
				return FALSE;
			}
			// ============================================================================================================
			// 2. ���������� �����ϴ��� üũ
			// ============================================================================================================
			sIndex = m_sHeadTag.Left(iCurIdx);
			sIndex.TrimLeft();
			sIndex.TrimRight();

			if(sIndex.IsEmpty()) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5012),iLine);		//_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. = ������ ���������� �������� �ʽ��ϴ�.")
				return FALSE;
			}

			// ============================================================================================================
			// 3. ���������� ������ �����ϴ��� üũ
			// ============================================================================================================
			// ���������� �� _T("=")�� �����ϴ��� üũ
			sIndex = m_sHeadTag.Right(m_sHeadTag.GetLength() - iCurIdx - 1);
				 
			sIndex.TrimLeft();
			sIndex.TrimRight();

			if(sIndex.Find(_T("=")) >= 0) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5013),iLine);		//_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. = �� �ΰ� �̻� �����մϴ�.")
				return FALSE;
			}	
			
			if(sIndex.IsEmpty()) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5014),iLine);		//_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. = ������ ������ ������ �������� �ʽ��ϴ�.")
				return FALSE;
			}

			CParse tokenParse(sIndex);
			tokenParse.SetDelimeter(_T(","));

			const INT iElementCnt = 3;
			CString sElement[iElementCnt];

			CString sElementAlias[iElementCnt] = {
				ConvertID2Chinese(IDS_STRING_5015),//_T("å������")	,		// 0
				ConvertID2Chinese(IDS_STRING_5016),//_T("��ĵ������"),		// 1
				ConvertID2Chinese(IDS_STRING_5017)//_T("����")				// 2
			};

			for(INT i = 0 ; tokenParse.HasMoreToken() && i < iElementCnt ; i++)
			{
				sElement[i] = tokenParse.NextToken();
				sElement[i].TrimLeft();
				sElement[i].TrimRight();
			}

			for(i = 0 ; i < iElementCnt ; i++)
			{
				if(sElement[i].IsEmpty()) {
					iLine = m_iStartLine;
					sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5018),iLine,sElementAlias[i]);	//_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. ������ ������ %s�� �������� �ʽ��ϴ�.")
					return FALSE;
				} 
			}

			// =======================================================================================================
			// 
			// =======================================================================================================
			m_iStartPage = _ttoi(sElement[1]);
			m_iLength = _ttoi(sElement[2]);
			m_iEndPage = m_iStartPage + m_iLength;
					
			return TRUE;
		}
		// ====================================================================================================
		// ������ üũ�� ���ϴ� ���.. ���� ������ �׳� 0,1,1�� �������ش�.
		// ====================================================================================================
		else
		{
			CString sIndex;
			// ============================================================================================================
			// 1. _T("=") �� �����ϴ��� üũ
			// ============================================================================================================
			INT iCurIdx = m_sHeadTag.Find(_T("="));
			if(iCurIdx < 0) {
				//iLine = m_iStartLine;
				//sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5019),iLine);	//_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. '='�� �����ϴ�.")
				//return FALSE;
				m_iStartPage = 1;
				m_iLength = 1;
				m_iEndPage = 2;
				return TRUE;
			}
			// ============================================================================================================
			// 2. ���������� �����ϴ��� üũ
			// ============================================================================================================
			sIndex = m_sHeadTag.Left(iCurIdx);
			sIndex.TrimLeft();
			sIndex.TrimRight();

			if(sIndex.IsEmpty()) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5020),iLine);		//_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. = ������ ���������� �������� �ʽ��ϴ�.")
				return FALSE;
			}

			// ============================================================================================================
			// 3. ���������� ������ �����ϴ��� üũ
			// ============================================================================================================
			// ���������� �� _T("=")�� �����ϴ��� üũ
			sIndex = m_sHeadTag.Right(m_sHeadTag.GetLength() - iCurIdx - 1);
				 
			sIndex.TrimLeft();
			sIndex.TrimRight();

			if(sIndex.Find(_T("=")) >= 0) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5021),iLine);		//_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. = �� �ΰ� �̻� �����մϴ�.")
				return FALSE;
			}	
			
			if(sIndex.IsEmpty()) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5022),iLine);		//_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. = ������ ������ ������ �������� �ʽ��ϴ�.")
				return FALSE;
			}

			CParse tokenParse(sIndex);
			tokenParse.SetDelimeter(_T(","));

			const INT iElementCnt = 3;
			CString sElement[iElementCnt];

			CString sElementAlias[iElementCnt] = {
				ConvertID2Chinese(IDS_STRING_5023),//_T("å������")	,		// 0
				ConvertID2Chinese(IDS_STRING_5024),//_T("��ĵ������"),		// 1
				ConvertID2Chinese(IDS_STRING_5025)//_T("����")				// 2
			};

			for(INT i = 0 ; tokenParse.HasMoreToken() && i < iElementCnt ; i++)
			{
				sElement[i] = tokenParse.NextToken();
				sElement[i].TrimLeft();
				sElement[i].TrimRight();
			}

			for(i = 0 ; i < iElementCnt ; i++)
			{
				if(sElement[i].IsEmpty()) {
					iLine = m_iStartLine;
					sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5026),iLine,sElementAlias[i]);	//_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. ������ ������ %s�� �������� �ʽ��ϴ�.")
					return FALSE;
				} 
			}

			m_iStartPage = _ttoi(sElement[1]);
			m_iLength = _ttoi(sElement[2]);
			m_iEndPage = m_iStartPage + m_iLength;
					
			return TRUE;
		}

	} else return TRUE;
}

// ================================================================================================================
// Tag���� ������ �ԷµǾ� ������ ������? ������ ������ �Է°����� Tag�ΰ�?
// ================================================================================================================
BOOLEAN CTocTag::CheckTagSyntax(INT& iLine,CString& sErrorMessage)
{
	return TRUE;		
}

VOID CTocTag::AddSubLevelToc(CTocTagInfo *pTocTagInfo)
{
	m_arrSubLevelTocTag.Add(pTocTagInfo);
}

BOOLEAN CTocTag::CheckPage(INT iExpectedEndPage, INT &iStartPage, INT &iEndPage)
{
	if(m_iTagType == TAG_TYPE_SINGLE) return TRUE;

	INT iMaxCount = m_arrSubLevelTocTag.GetSize();

	if(m_iTagType == TAG_TYPE_PAGEINFO) {

		iStartPage = m_iStartPage;
		iEndPage = m_iEndPage;

		if (m_nTotalPage< m_iEndPage-1)
			m_nTotalPage= m_iEndPage-1;

		CString sPage;
		sPage.Format(_T("%i"),m_iStartPage);
		/*
		if(CTiffFileManager::IsValidPage(sPage)) {
			return TRUE;	
		} else {
			return FALSE;
		}
		*/

		return TRUE;
	} 
	
	INT iCurStartPage = -1;
	INT iCurEndPage = -1;

	INT iTempStartPage = 0;
	INT iTempEndPage = 0;

	INT iType = 0;
	INT iErrorLine = -1;
	CString sErrorMessage;

	INT iCurExpectedEndPage = -1;
	// ============================================================================================================
	// �ش� Tag �ȿ� ��ġ�� Page ������ ���� ���� ������ �� �� �������� ���´�.
	// ============================================================================================================
	BOOLEAN bIsFirst = TRUE;
	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		CTocTagInfo *pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		iType = pTocTagInfo -> GetType();
		if (!pTocTagInfo ->IsPageCheck())
			continue;
			
		if(iType == TAG_TYPE_PAGEINFO) {
			if(((CTocTag*)pTocTagInfo) -> CheckPage(iCurExpectedEndPage, iTempStartPage, iTempEndPage)) {				

				if(bIsFirst) {
					bIsFirst = FALSE;
					iCurStartPage = iTempStartPage;
					iCurEndPage = iTempEndPage;
				} else {
					INT iBuffer = iCurEndPage - iTempStartPage;
					//if(iBuffer == 0 || iBuffer == 1) {  // 2001.6.13. ������ �����ϰ� ���ƴ�.
					if(iBuffer <= 1) {
						iCurEndPage = iTempEndPage;	
					} else {
						sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5027),iCurEndPage - 1,iCurEndPage);	//_T("�������� %i�Ǵ� %i�Դϴ�.")
						m_pErrorQueue -> AddErrorMessage(pTocTagInfo -> GetLine(), sErrorMessage, _T(""));	
						continue;
					}
				}
			} else {
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5028),iTempStartPage);	//_T("�������� %i(��)�� Tiff������ �������� �ʽ��ϴ�.")
				m_pErrorQueue -> AddErrorMessage(pTocTagInfo -> GetLine(), sErrorMessage, _T(""));
				continue;
			}
		}
	}

	// ============================================================================================================
	// �ش� Tag �ȿ� ��ġ�� ���������� ������ ������ üũ�ϰ� �ش� ���� �������� ���ԵǾ� �ִ��� üũ�Ѵ�.
	// ============================================================================================================

	iStartPage = iCurStartPage;
	iEndPage = iCurEndPage;

	BOOLEAN bIsReturn = FALSE;

	INT iCurSubTocStartPage = -1;
	INT iCurSubTocEndPage = -1;

	BOOLEAN bIsFirstSubToc = TRUE;

	for(i = 0 ; i < iMaxCount ; i++)
	{
		CTocTagInfo *pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);

		iType = pTocTagInfo -> GetType();
		if (!pTocTagInfo ->IsPageCheck())
			continue;

		if(iType == TAG_TYPE_MULTI) {
			bIsReturn = ((CTocTag*)pTocTagInfo) -> CheckPage(iCurExpectedEndPage, iTempStartPage, iTempEndPage);
			// ======================================================================================================
			// bIsReturn = TRUE  : Error ���� Page ���� üũ�� ����.
			// bIsReturn = FASLE : Page ������ �̻�����.
			// ======================================================================================================
			if(bIsReturn) {
				iCurExpectedEndPage = iTempEndPage;

				if(iCurStartPage >= 0 && iCurStartPage > iTempStartPage) {
					sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5029),pTocTagInfo->GetHeadTag(),GetHeadTag());	//_T("<%s>�� �������� <%s>���� ������ ������ ������ϴ�.")
					m_pErrorQueue -> AddErrorMessage(pTocTagInfo->GetLine(), sErrorMessage, _T(""));
					return FALSE;
				} else if(iCurEndPage >= 0 && iCurEndPage < iTempEndPage) {
					sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5030),pTocTagInfo->GetHeadTag(),GetHeadTag());	//_T("<%s>�� �������� <%s>���� ������ ������ ������ϴ�.")
					m_pErrorQueue -> AddErrorMessage(pTocTagInfo->GetLine(), sErrorMessage, _T(""));
					return FALSE;
				} 

				if(bIsFirstSubToc) {
					if (_tcsicmp(pTocTagInfo->GetHeadTag(), _T("figure"))==0 || _tcsicmp(pTocTagInfo->GetHeadTag(), _T("table"))==0 )
						continue;

					bIsFirstSubToc = FALSE;
					iCurSubTocStartPage = iTempStartPage;
					iCurSubTocEndPage = iTempEndPage;

					if(iExpectedEndPage >= 0 && iCurSubTocStartPage - iExpectedEndPage != 0 && iCurSubTocStartPage - iExpectedEndPage != 1) {
						sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5031),pTocTagInfo->GetHeadTag());		//_T("<%s>�� �������� ���� ������ ������ ������ϴ�.")
						m_pErrorQueue -> AddErrorMessage(pTocTagInfo->GetLine(), sErrorMessage, _T(""));
						return FALSE;
					}
				} else {
					if (_tcsicmp(pTocTagInfo->GetHeadTag(), _T("figure"))==0 || _tcsicmp(pTocTagInfo->GetHeadTag(), _T("table"))==0 )
						continue;

					INT iBuffer = iCurSubTocEndPage - iTempStartPage;
					if(iBuffer != 0 && iBuffer != 1) {
						sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5032),pTocTagInfo->GetHeadTag());		//_T("<%s>�� �������� ���� ������ ������ ������ϴ�.")
						m_pErrorQueue -> AddErrorMessage(pTocTagInfo->GetLine(), sErrorMessage, _T(""));
						return FALSE;
					}
					iCurSubTocEndPage = iTempEndPage;
				}

			} else {
				return FALSE;
			}	
		}
	}

	return TRUE;
}

INT CTocTag::GetCountTagValue()
{
	for(INT i = 0 ; i < m_arrSubLevelTocTag.GetSize(); i++)
	{
		CTocTagInfo *pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		CString tag= pTocTagInfo->m_sHeadTag;
		tag.MakeUpper();
		if (tag== _T("COUNT"))
			return _ttoi(pTocTagInfo->GetValue());
	}		

	return -1;
}

INT CTocTag::GetTotalPage()
{
	return m_nTotalPage;
}

VOID CTocTag::SetTotalPage(INT page)
{
	m_nTotalPage= page;
}

VOID CTocTag::SetPageCheck(bool flag)
{
	m_bPageCheck= flag;
}

bool CTocTag::GetPageCheck()
{
	return m_bPageCheck;
}
