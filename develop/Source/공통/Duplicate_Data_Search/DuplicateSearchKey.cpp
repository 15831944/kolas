// DuplicateSearchKey.cpp: implementation of the CDuplicateSearchKey class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DuplicateSearchKey.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDuplicateSearchKey::CDuplicateSearchKey()
{

}

CDuplicateSearchKey::~CDuplicateSearchKey()
{
	this->DeleteAllList();
}

INT CDuplicateSearchKey::SetSearchKeys(CString filename)
{
	if(filename.GetLength() == 0)
		filename = _T(".\\duplicate_search_key.txt");
	
	INT ids, i ;
	ids = i = 0;
	CString tmpstr, tmpvalue, tmpalias;

	CStdioFile file;
	CDuplicateSearchField * psfield = NULL;

	if(!m_SearchKeys.IsEmpty())
		this->DeleteAllList();

	if ( file.Open(filename, CFile::modeRead | CFile::typeBinary) == FALSE ) 
	{
		ids = SetDefaultSearchKeys();
		if(ids < 0) return -1000;
		return 0;
	}
	else
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		while(file.ReadString(tmpstr))
		{
			tmpstr.TrimLeft();	tmpstr.TrimRight();
			ids = CheckString(tmpstr);
			if(ids == -1) continue;
			else if(ids == 0) 
			{
				if(psfield) 
					m_SearchKeys.AddTail((CObject*)psfield);
				psfield = new CDuplicateSearchField();
				tmpvalue = GetKeyName(tmpstr);
				if(tmpvalue.GetLength() == 0)
				{
					delete psfield;
					psfield = NULL;
					continue;
				}
				psfield->SetKeyName(tmpvalue);
			}
			else
			{
				if(!psfield) continue;
				ids = GetAliasAndValue(tmpalias, tmpvalue, tmpstr);
				if(tmpalias.GetLength() == 0 || tmpvalue.GetLength() == 0) continue;
				psfield->SetAliasAndValue(tmpalias, tmpvalue);				
			}
		}
		if(psfield)
			m_SearchKeys.AddTail((CObject*)psfield);
	}

	if(m_SearchKeys.GetCount() == 0)
		this->SetDefaultSearchKeys();
	
	file.Close();
	return 0;
}

INT CDuplicateSearchKey::CheckString(CString str)
{
	//���鹮�ڸ� return
	if(str.GetLength() == 0) return -1;
	TCHAR firstchar = str.GetAt(0);

	if(firstchar == '[') // '['���� �����ϸ� 0���� : ���ο� CDuplicateSearchField�� �����Ѵ�.
		return 0;
	else if(firstchar == '.') // '.'���� �����ϸ� �ش� ������ �ּ�
		return -1;
	return 1; // �̿��� ������ CDuplicateSearchField�� �� �Ӽ������̴�.
}

CString CDuplicateSearchKey::GetKeyName(CString str)
{
	if(str.GetLength() == 0) return _T("");
	INT firstidx, lastidx;
	firstidx = lastidx = 0;
	firstidx = str.Find('[');
	lastidx = str.Find(']');
	CString value = str.Mid(firstidx+1, lastidx - firstidx - 1);
	value.TrimLeft();	value.TrimRight();
	return value;
}

INT CDuplicateSearchKey::GetAliasAndValue(CString &alias, CString &tmpvalue, CString str)
{
	if(str.GetLength() == 0) return -1;
	INT sepidx = str.Find('=');
	INT maxidx = str.GetLength() - 1;
	alias = str.Mid(0, sepidx);
	alias.TrimLeft();	alias.TrimRight();
	tmpvalue = str.Mid(sepidx+1, maxidx);
	tmpvalue.TrimLeft();	tmpvalue.TrimRight();
	return 0;
}

INT CDuplicateSearchKey::GetSearchKeys(CString filename)
{
	if(filename.GetLength() == 0)
		filename = _T(".\\duplicate_search_key.txt");
	
	INT ids, i ;
	ids = i = 0;
	CString tmpstr, tmpstr1;

	CStdioFile file;
	CDuplicateSearchField * psfield = NULL;
	if ( file.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) == FALSE ) 
	{
		filename += _T(" ������ �����Ҽ� �����ϴ�.");
		AfxMessageBox(filename, MB_ICONSTOP);
		return -1;
	}
	else
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		
		POSITION pos = m_SearchKeys.GetHeadPosition();
		while(pos != NULL)
		{
			psfield = (CDuplicateSearchField *)m_SearchKeys.GetNext(pos);
			//tmpstr = psfield->GetFieldDataString();
			tmpstr.Format(_T("[%s]"), psfield->GetKeyName());
			tmpstr += _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("SEARCH_PLACE=");
			tmpstr += psfield->GetSearchPlace() + _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("COLUMN_NAME=");
			tmpstr += psfield->GetColumnName() + _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("DM_ALIAS=");
			tmpstr += psfield->GetDMAlias() + _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("CM_ALIAS=");
			tmpstr += psfield->GetCMAlias() + _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("SEARCH_TYPE=");
			tmpstr1.Format(_T("%d"),psfield->GetSearchType());
			tmpstr += tmpstr1 + _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("ARTICLE_SEARCH_YN=");
			tmpstr1.Format(_T("%d"), psfield->GetArticleSearchYN());
			tmpstr += tmpstr1 + _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("USE_YN=");
			tmpstr1.Format(_T("%d"), psfield->GetUseYN());
			tmpstr += tmpstr1 + _T("\r\n") + _T("\r\n");
			file.WriteString(tmpstr);			
		}

		/*
		// �����ڷ� ���Կ��� �ɼ�
		if ( m_bIncludeDiscardedBook ) tmpstr.Format(_T("[�����ڷ�����]\nUSE_YN=0\n"));
		else tmpstr.Format(_T("[�����ڷ�����]\nUSE_YN=1\n"));	
		file.WriteString(tmpstr);			
		*/
	}

	file.Close();
	
	return 0;
}

INT CDuplicateSearchKey::SetDefaultSearchKeys()
{
	CDuplicateSearchField * psfield = NULL;
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("��ǥ��"), _T("IDX_BO_TBL"), _T("IDX_TITLE"), _T("IBS_��ǥ��"), _T("IBS_��ǥ��"), 0, FALSE, TRUE);
	m_SearchKeys.AddTail((CObject*)psfield);
	//===================================================
	//2010.11.29 ADD BY PJW : �������� �Ѽ����� �߰��Ѵ�.
	psfield = new CDuplicateSearchField();
//	psfield->SetDefaultValue(_T("�Ѽ���"), _T("IDX_BO_TBL"), _T("IDX_TITLE"), _T("UDF_�Ѽ���"), _T("UDF_�Ѽ���"), 0, FALSE, TRUE);
	psfield->SetDefaultValue(_T("�Ѽ���"), _T("IDX_BO_TBL"), _T("IDX_ALL_ITEM"), _T("UDF_�Ѽ���"), _T("UDF_�Ѽ���"), 0, FALSE, TRUE);
	m_SearchKeys.AddTail((CObject*)psfield);
	//===================================================
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("������"), _T("IDX_BO_TBL"), _T("IDX_AUTHOR"), _T("IBS_������"), _T("IBS_������"), 0, FALSE, TRUE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("������"), _T("IDX_BO_TBL"), _T("IDX_PUBLISHER"), _T("IBS_������"), _T("IBS_������"), 0, FALSE, TRUE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("�����"), _T("IDX_BO_TBL"), _T("PUB_YEAR"), _T("IBS_�����"), _T("IBS_�����"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("����ISBN"), _T("IDX_BO_TBL"), _T("ST_CODE"), _T("BB_����_ISBN"), _T("BB_����_ISBN"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("��ü����"), _T("IDX_BO_TBL"), _T("MEDIA_CODE"), _T("BB_��ü����"), _T("BB_��ü����"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("�����"), _T("IDX_BO_TBL"), _T("IDX_VOL"), _T("BB_�����"), _T("BB_�����"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("�Ǽ���"), _T("IDX_BO_TBL"), _T("IDX_VOL_TITLE"), _T("BB_�Ǽ���"), _T("BB_�Ǽ���"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("��ġ��ȣ"), _T("IDX_BO_TBL"), _T("IDX_SE_SHELF_CODE"), _T("BB_��ġ��ȣ"), _T("BB_��ġ��ȣ"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("��ϱ���"), _T("IDX_BO_TBL"), _T("REG_CODE"), _T("BB_��ϱ���"), _T("BB_��ϱ���"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	return 0;
}

INT CDuplicateSearchKey::GetFieldKeySettingInfo(CString fieldName, BOOL *pUseYN, INT *pSearchType, BOOL *pArticleSearchYN)
{
	INT count = m_SearchKeys.GetCount();
	if(count == 0) return -1;
	CDuplicateSearchField * psfield = NULL;
	POSITION pos = m_SearchKeys.GetHeadPosition();
	for(INT i=0 ; i<count ; i++)
	{	
		psfield = (CDuplicateSearchField *)m_SearchKeys.GetNext(pos);
		if(psfield == NULL) continue;
		if(psfield->GetKeyName() == fieldName)
			break;		
	}
	if(i < count)
	{
		*pUseYN = psfield->GetUseYN();
		*pSearchType = psfield->GetSearchType();
		if(pArticleSearchYN)
			*pArticleSearchYN = psfield->GetArticleSearchYN();
	}
	else
	{
		*pUseYN = FALSE;
		*pSearchType = -1;
		if(pArticleSearchYN)
			*pArticleSearchYN = FALSE;
	}
	return 0;
}

INT CDuplicateSearchKey::SetFieldKeySettingInfo(CString fieldName, BOOL useYN, INT searchType, BOOL articleSearchYN)
{
	INT count = m_SearchKeys.GetCount();
	if(count == 0) return -1;
	CDuplicateSearchField * psfield = NULL;
	POSITION pos = m_SearchKeys.GetHeadPosition();
	for(INT i=0 ; i<count ; i++)
	{
		psfield = (CDuplicateSearchField *)m_SearchKeys.GetNext(pos);
		if(psfield == NULL) continue;
		if(psfield->GetKeyName() == fieldName)
			break;		
	}
	if(i < count)
	{
		psfield->SetUseYN(useYN);
		psfield->SetSearchType(searchType);
		psfield->SetArticleSearchYN(articleSearchYN);
	}
	else 
	{
		psfield->SetUseYN(FALSE);
		psfield->SetSearchType(-1);
		psfield->SetArticleSearchYN(FALSE);
	}
	return 0;
}

INT CDuplicateSearchKey::DeleteAllList()
{
	CDuplicateSearchField* pa;
	INT count = m_SearchKeys.GetCount();
	if(count == 0) return 0;

	for(INT i=0 ; i<count ; i++)
	{
		pa = (CDuplicateSearchField*)m_SearchKeys.RemoveHead(); 
		if(pa) delete pa;
	}
	m_SearchKeys.RemoveAll();
	return(count);
}

INT CDuplicateSearchKey::GetSearchFieldKeySettingInfo(CString fieldName, CString &searchPlace, CString &searchField)
{
	INT count = m_SearchKeys.GetCount();
	if(count == 0) return -1;
	CDuplicateSearchField * psfield = NULL;
	POSITION pos = m_SearchKeys.GetHeadPosition();
	for(INT i=0 ; i<count ; i++)
	{	
		psfield = (CDuplicateSearchField *)m_SearchKeys.GetNext(pos);
		if(psfield == NULL) continue;
		if(psfield->GetKeyName() == fieldName)
			break;		
	}
	if(i < count)
	{
		searchPlace = psfield->GetSearchPlace();
		searchField = psfield->GetColumnName();
	}
	else
		return -1000;
	return 0;
}

INT CDuplicateSearchKey::GetDMCMAlias(CString fieldName, CString &DMAlias, CString &CMAlias)
{
	INT count = m_SearchKeys.GetCount();
	if(count == 0) return -1;
	CDuplicateSearchField * psfield = NULL;
	POSITION pos = m_SearchKeys.GetHeadPosition();
	for(INT i=0 ; i<count ; i++)
	{	
		psfield = (CDuplicateSearchField *)m_SearchKeys.GetNext(pos);
		if(psfield == NULL) continue;
		if(psfield->GetKeyName() == fieldName)
			break;		
	}
	if(i < count)
	{
		DMAlias = psfield->GetDMAlias();
		CMAlias = psfield->GetCMAlias();
	}
	else
		return -1000;
	return 0;
}
