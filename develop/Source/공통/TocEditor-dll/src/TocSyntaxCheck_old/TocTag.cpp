// TocTag.cpp: implementation of the CTocTag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TocTag.h"
#include "TocParse.h"
#include "TocEditorDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTocTag::CTocTag(	CErrorQueue* pErrorQueue,
					CString sSourceRecord,
					INT iTagType,
					INT iLevel,
					BOOLEAN bIsRepeatTag	)
: CTocTagInfo(pErrorQueue, sSourceRecord, iTagType, iLevel, bIsRepeatTag)
{
	m_arrSubLevelTocTag.SetSize(0,1);
	m_arrTocTagList.RemoveAll() ;
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
	m_arrTocTagList.RemoveAll() ;
}

INT CTocTag::CheckTocSyntax(INT nKind, CArray<CString,CString>& arrTocTagList, CString sParentTag/*=_T("")*/)
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

	m_arrTocTagList.Copy(arrTocTagList) ;

	INT retval= 0 ;
	INT iErrorLine = -1;
	CString sErrorMessage;
	// 1. Pair Tag�� ��� �� ���� Tag�� ��� �����ϴ°�?
	if(!CheckExistPairTag(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
		retval= -1 ;
	}
	// 2. ���� ���� �߿��� �ݺ��������� ���� Tag�� �ݺ��Ǿ��°�?
	if(!CheckRepeatTag(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
		retval= -2 ;
	}
	// 3. ���� ���� �߿��� �������� ������ ���谡 �¾ƶ������°�?
	if(!CheckTagLevel(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
		retval= -3 ;
	}
		// 2002.5.2 removed by jungjoo
	// 4. Page ������ ��� ���������� ���Ŀ� �����ϴ°�?
	if(!CheckPageInfo(iErrorLine, sErrorMessage, sParentTag, nKind)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
		retval= -4 ;
	}

	// 5. Tag���� ������ �ԷµǾ� ������ ������? ������ ������ �Է°����� Tag�ΰ�?
	if(!CheckTagSyntax(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
		retval= -5 ;
	}
	// ================================================================================================================
	// 6. SubToc�� ���� Syntax�˻�
	// ================================================================================================================
	INT iMaxCount = m_arrSubLevelTocTag.GetSize();
	CTocTagInfo* pTocTagInfo = NULL;
	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		retval= ((CTocTag*)pTocTagInfo)	-> CheckTocSyntax(nKind, m_arrTocTagList, m_sHeadTag);
	}	

	// body�� ������ ������ ������ ��½����ش�.	2001.6.8 added
	if( m_sHeadTag.Compare(_T("body") )==0  && iMaxCount == 0 ) {
		iErrorLine = m_iStartLine ;
		sErrorMessage = _T("<body>(���� ����) �� ��� �ֽ��ϴ�.") ;
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
		retval= -6 ;
	}

	return retval;
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
				sErrorMessage.Format(_T("<%s>�� ���� ¦�� �Ǵ� Tag �� �������� �ʽ��ϴ�."),m_sTailTag.GetBuffer(0));
			} 

			if(m_sTailTag.IsEmpty()) {
				iLine = m_iStartLine;
				sErrorMessage.Format(_T("<%s>�� ���� ¦�� �Ǵ� Tag �� �������� �ʽ��ϴ�."),m_sHeadTag.GetBuffer(0));
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
				sErrorMessage.Format(_T("%s TAG�� �ߺ��ؼ� ����� �� �����ϴ�."),pTocTagInfo->GetHeadTag());
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

		iLevel= pTocTagInfo -> GetLevel() ;
		if(m_iLevel < iLevel) {

			if( m_sHeadTag.IsEmpty() ) return TRUE ;
			CString strNext ;
			strNext= pTocTagInfo -> GetHeadTag() ;
			INT nIdx= strNext.Find(_T(">"));
			if( nIdx < 0 )	return TRUE ;	//if( nIdx > 0 ) �� if( nIdx < 0 ) ���� ����

			INT nSize= m_arrTocTagList.GetSize() ;
			CString strTag= strNext.Mid(1,nIdx) ;
			for( INT i=0 ; i<nSize ; i++ ) {
				if( strTag != m_arrTocTagList.GetAt(i) )
					return TRUE ;
			}

			iLine = pTocTagInfo -> GetLine();
			sErrorMessage.Format(_T("<%s> ��ũ�� <%s> �±׸� ������ �� �����ϴ�."),m_sHeadTag,pTocTagInfo -> GetHeadTag());
			return FALSE;

		}
	}

	return TRUE;
}

// ================================================================================================================
// Page ������ ��� ���������� ���Ŀ� �����ϴ°�?
// ================================================================================================================
BOOLEAN CTocTag::CheckPageInfo(INT& iLine, CString& sErrorMessage, CString& sParentTag, INT& nKind)
{
	if(m_iLevel == LEVEL0) return TRUE;

	// ================================================================================================================
	// �������� = å������, ��ĵ������, ����
	// 1. _T("=") �� �����ϴ��� üũ
	// 2. ���������� �����ϴ��� üũ
	// 3. ���������� ������ �����ϴ��� üũ
	// 4. ���������� ���� ���� üũ
	// ================================================================================================================
	if(m_iTagType == TAG_TYPE_PAGEINFO) {

		if ( sParentTag.IsEmpty() )		// added 12/10/2001
		{
			iLine = m_iStartLine;
			sErrorMessage.Format(_T("�����±�(body,table,figure)�� �������� �ʽ��ϴ�!"),iLine);
			return FALSE;
		}

//		if( nKind != _YON_ )
//			return TRUE ;

		CString sIndex;
		// ============================================================================================================
		// 1. _T("=") �� �����ϴ��� üũ	- üũ���� �ʴ´�. modified 2002.9.19
		// ============================================================================================================
		INT iCurIdx = m_sHeadTag.Find(_T("="));
		if(iCurIdx < 0) {
			iLine = m_iStartLine;
//			sErrorMessage.Format(_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. '='�� �����ϴ�."),iLine);
//			return FALSE;
		}

		CString sContents ;
		sContents= m_sHeadTag;
		if( sContents.Mid(0,1)==_T(".") )
			sContents= sContents.Mid(1);

		INT nLast= iCurIdx ;
		if( nLast<0 ) // _T("=") �� ���� ���
			nLast= sContents.GetLength();

		sContents= sContents.Mid(0, nLast);
		sContents.TrimLeft(); sContents.TrimRight();

		// ============================================================================================================
		// 2. ���������� �����ϴ��� üũ
		// ============================================================================================================
//		sIndex = m_sHeadTag.Left(iCurIdx);
//		sIndex.TrimLeft();
//		sIndex.TrimRight();

		if(sContents.IsEmpty()) {
			iLine = m_iStartLine;
			sErrorMessage.Format(_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. ���������� �������� �ʽ��ϴ�."),iLine);
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
			sErrorMessage.Format(_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. = �� �ΰ� �̻� �����մϴ�."),iLine);
			return FALSE;
		}
		
		if(sIndex.IsEmpty()) {
			iLine = m_iStartLine;
			sErrorMessage.Format(_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. = ������ ������ ������ �������� �ʽ��ϴ�."),iLine);
			return FALSE;
		}
/*
		CParse tokenParse(sIndex);
		tokenParse.SetDelimeter(_T(","));

		const INT iElementCnt = 3;
		CString sElement[iElementCnt];

		CString sElementAlias[iElementCnt] = {
			_T("å������")	,		// 0
			_T("��ĵ������"),		// 1
			_T("����")				// 2
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
				sErrorMessage.Format(_T("%i���� �������Ŀ� ������ �ֽ��ϴ�. ������ ������ %s�� �������� �ʽ��ϴ�."),iLine,sElementAlias[i]);
				return FALSE;
			} 
		}

		// =======================================================================================================
		// 
		// =======================================================================================================
		m_iStartPage = _ttoi(sElement[1]);
		m_iLength = _ttoi(sElement[2]);
		m_iEndPage = m_iStartPage + m_iLength;
*/				
		return TRUE;
	} else
		return TRUE;
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

		CString sPage;
		sPage.Format(_T("%i"),m_iStartPage);

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
					if(iBuffer == 0 || iBuffer == 1) {
						iCurEndPage = iTempEndPage;	
					} else {
						sErrorMessage.Format(_T("�������� %i�Ǵ� %i�Դϴ�."),iCurEndPage - 1,iCurEndPage);
						m_pErrorQueue -> AddErrorMessage(pTocTagInfo -> GetLine(), sErrorMessage, _T(""));	
						continue;
					}
				}
			} else {
//				sErrorMessage.Format(_T("�������� %i(��)�� Tiff������ �������� �ʽ��ϴ�."),iTempStartPage);
//				m_pErrorQueue -> AddErrorMessage(pTocTagInfo -> GetLine(), sErrorMessage, _T(""));
//				continue;
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
					sErrorMessage.Format(_T("<%s>�� �������� <%s>���� ������ ������ ������ϴ�."),pTocTagInfo->GetHeadTag(),GetHeadTag());
					m_pErrorQueue -> AddErrorMessage(pTocTagInfo->GetLine(), sErrorMessage, _T(""));
					return FALSE;
				} else if(iCurEndPage >= 0 && iCurEndPage < iTempEndPage) {
					sErrorMessage.Format(_T("<%s>�� �������� <%s>���� ������ ������ ������ϴ�."),pTocTagInfo->GetHeadTag(),GetHeadTag());
					m_pErrorQueue -> AddErrorMessage(pTocTagInfo->GetLine(), sErrorMessage, _T(""));
					return FALSE;
				} 

				if(bIsFirstSubToc) {
					bIsFirstSubToc = FALSE;
					iCurSubTocStartPage = iTempStartPage;
					iCurSubTocEndPage = iTempEndPage;

					if(iExpectedEndPage >= 0 && iCurSubTocStartPage - iExpectedEndPage != 0 && iCurSubTocStartPage - iExpectedEndPage != 1) {
						sErrorMessage.Format(_T("<%s>�� �������� ���� ������ ������ ������ϴ�."),pTocTagInfo->GetHeadTag());	
						m_pErrorQueue -> AddErrorMessage(pTocTagInfo->GetLine(), sErrorMessage, _T(""));
						return FALSE;
					}
				} else {
					INT iBuffer = iCurSubTocEndPage - iTempStartPage;
					if(iBuffer != 0 && iBuffer != 1) {
						sErrorMessage.Format(_T("<%s>�� �������� ���� ������ ������ ������ϴ�."),pTocTagInfo->GetHeadTag());	
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
