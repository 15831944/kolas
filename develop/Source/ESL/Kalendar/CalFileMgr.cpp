// CalFileMgr.cpp: implementation of the CCalFileMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kalendar.h"
#include "CalFileMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalFileMgr::CCalFileMgr()
{
	
}

CCalFileMgr::~CCalFileMgr()
{

}

// �����ۼ�
BOOL CCalFileMgr::WriteMemoFile(CString stFileName, CString stData)
{		
	if(_T("") == stFileName)
	{
		LOG(_T("WriteMemoFile() : �����̸��� �����ϴ�."));				
		return FALSE;
	}
	CString strDir = _T("");
	Get_FILEDIR(strDir);
	ChkDir(strDir);
	stFileName = strDir + stFileName;

	if(!IsData(stData))
	{
		::DeleteFile(stFileName);
		return TRUE;
	}
	CString stTempFile = stFileName + _T("_Temp");	

	CFile file;
	BOOL bChkError = file.Open(stTempFile,CFile::modeCreate|CFile::modeWrite);	
	if(FALSE==bChkError)
	{
		CString strTmp = stTempFile;
		stTempFile = _T("WriteMemoFile(") + stTempFile + _T(") : ���Ͽ��� ����");
		LOG(strTmp);	
		return FALSE;
	}

	if(MAXBUFFER<stData.GetLength())
	{
		CString st;
		st.Format(_T("���ڼ��� %d�� �̳���  �ۼ��Ͽ� �ֽʽÿ�."), MAXBUFFER);
		AfxMessageBox(st);
		return FALSE;
	}

	TCHAR tchar = 0xFEFF;
	file.Write(&tchar, sizeof(TCHAR) );
	file.Write(LPCTSTR(stData), stData.GetLength()* sizeof(TCHAR));		
	if(TRUE == IsFile(stFileName))
	{
		file.Remove(stFileName);	
	}

	file.Close();
	file.Rename(stTempFile, stFileName);
	return TRUE;
}

// ���� �б�
BOOL CCalFileMgr::ReadMemoFile(CString stFileName, CString* stText)
{
	if(_T("") == stFileName)
	{
		LOG(_T("ReadMemoFile() : �޸������̸��� �����ϴ�."));		
		return FALSE;
	}	
	CString strDir = _T("");
	Get_FILEDIR(strDir);
	ChkDir(strDir);
	stFileName = strDir + stFileName;

	CFile file;	
	BOOL bChkError = file.Open(stFileName, CFile::modeRead);	
	if(false==bChkError)
	{
		CString strTmp = stFileName;
		strTmp = _T("ReadMemoFile(") + stFileName + _T(") : ���Ͽ��� ����");
		LOG(strTmp);				
		return FALSE;
	}	
	TCHAR tchar;
	file.Read(&tchar, sizeof(TCHAR) );
	// �����ڵ尡 �ƴϸ�
	if(0xFEFF!=tchar)
	{
		file.Close();
		return FALSE;
	}

	TCHAR szBuf[MAXBUFFER];
	ZeroMemory(szBuf, MAXBUFFER);

	file.Read(szBuf, file.GetLength());	

	stText->Empty();
	stText->Format(_T("%s"), szBuf);
	file.Close();
	return TRUE;
}

// �ش� �޸������� �ִ��� ��ȯ
BOOL CCalFileMgr::IsFile(CString stFileName)
{
	CFileFind find;
	int Chk = stFileName.Find(_T("\\"));
	if(-1==Chk)
	{
		CString strDir = _T("");
		Get_FILEDIR(strDir);
		ChkDir(strDir);
		stFileName = strDir + stFileName;
	}
	return find.FindFile(stFileName);
}

// �ش� �޸������� �ִ����� ��ȯ
BOOL CCalFileMgr::IsFile(UINT uYear, UINT uMonth, UINT uDay)
{
	CString stFileName = GetMemoFileName(uYear,uMonth, uDay);	
	return IsFile(stFileName);
}

// �޸������̸� ����
CString CCalFileMgr::GetMemoFileName(UINT uYear, UINT uMonth, UINT uDay)
{
 	CString stFileName;
 	stFileName.Format(MEMOFILENAME, uYear, uMonth, uDay);
 	return stFileName;
}

// �޸����� �ҷ�����
BOOL CCalFileMgr::ReadMemoFile(UINT uYear, UINT uMonth, UINT uDay, CString *stText)
{
	CString stFileName = GetMemoFileName(uYear, uMonth, uDay);
	return ReadMemoFile(stFileName, stText);
}

// �޸����� ���
BOOL CCalFileMgr::WriteMemoFile(UINT uYear, UINT uMonth, UINT uDay, CString stText)
{
	CString stFileName = GetMemoFileName(uYear, uMonth, uDay);
	return WriteMemoFile(stFileName, stText);
}

// �˻��� �޸� �߰�
void CCalFileMgr::AddSearchMemoList(UINT uYear, UINT uMonth, UINT uDay, CString stText)
{
	CMemo* pMemo = new CMemo;
	pMemo->SetMemo(uYear, uMonth, uDay, stText);
	m_aFileForSearch.Add(pMemo);
}

// �˻��� ������ ���� ����
int CCalFileMgr::GetSearchFileSize()
{
	return m_aFileForSearch.GetSize();
}

// �˻��� ���ϸ�� ����
void CCalFileMgr::ResetSearchMemoList()
{
	m_aFileForSearch.RemoveAll();
}

// stFileName�� �̸��� ���� ���Ͽ��� stSearchText���ڰ� ���ԵǾ� �ִ��� ���θ� ��ȯ
// �Լ��̸�: IsSearchTextInFile
// �Լ�����: Ư�����Ͽ��� Ư�����ڰ� ���ԵǾ��ִ��� ���θ� �Ǵ�
// ��ȯ  ��: BOOL
// ����[CString stFileName] : �˻��� ������ �̸�
// ����[CString stSearchText] : �˻��� �ؽ�Ʈ
BOOL CCalFileMgr::IsSearchTextInFile(CString stFileName, CString stSearchText)
{
	CString stText;
	ReadMemoFile(stFileName, &stText);		
	if(-1==stText.Find(stSearchText))
		return FALSE;
	return TRUE;
}

// 

// �Լ��̸�: SearchTextAndAddResultInSearchMemoList
// �Լ�����: Ư�� ��/���� �ؽ�Ʈ������ �о�鿩 �˻��ϴ� �ؽ�Ʈ�� ����ִ� �޸� ����Ʈ���߰�
// ��ȯ  ��: void 
// ����[UINT uYear] : �˻��� �⵵
// ����[UINT uMonth] : �˻��� ��
// ����[CString stSearchText] : ã�����ϴ� ���ڿ�
void CCalFileMgr::SearchTextAndAddResultInSearchMemoList(UINT uYear, UINT uMonth, CString stSearchText)
{	
	// �ش���� ���ڼ��� ���� EX>>3���� ��� 31;
	UINT Max = CKoreanDate::GetDaysOfMonth(uYear, uMonth);
	CString stFileName, stText;
	for(UINT uDay =1;uDay<=Max;uDay++)
	{
		stFileName = GetMemoFileName(uYear, uMonth, uDay);
		if(IsSearchTextInFile(stFileName, stSearchText))
		{
			ReadMemoFile(stFileName, &stText);
			AddSearchMemoList(uYear, uMonth, uDay, stText);
		}
	}
}

// �Լ��̸�: SearchTextAndAddResultInSearchMemoList
// �Լ�����: Ư�� ���� �ؽ�Ʈ������ �о�鿩 �˻��ϴ� �ؽ�Ʈ�� ����ִ� �޸� ����Ʈ���߰�
// ��ȯ  ��: void 
// ����[UINT uYear] : �˻��� �⵵
// ����[CString stSearchText] : �˻��� ���ڿ�
void CCalFileMgr::SearchTextAndAddResultInSearchMemoList(UINT uYear, CString stSearchText)
{
	for(int uMonth=1;uMonth<=12;uMonth++)
	{
		SearchTextAndAddResultInSearchMemoList(
			uYear, uMonth, stSearchText);
	}
}

CMemo* CCalFileMgr::GetSearchMemoListAt(UINT uIndex)
{
	CMemo* pMemo = (CMemo*) m_aFileForSearch.GetAt(uIndex);
	return pMemo;
}

UINT CCalFileMgr::GetSearchMemoListCount()
{
	return m_aFileForSearch.GetSize();
}

void CCalFileMgr::ChkDir(CString strDir)
{
	CFileFind file;
	CString strFile = _T("*.*");
	BOOL bResult = file.FindFile(strDir + strFile);	

	if(!bResult)		
	{		
		bResult = CreateDirectory(strDir+_T("\\"),NULL);		
		if(!bResult)			
		{	
			//Error;			
		}		
	}   
}


// �Լ��̸�: IsData
// �Լ�����: Ư�� ���ڿ��� �����Ͱ� ����ִ��� �����Ǵ�
// ��ȯ  ��: BOOL 
// ����[CString stData] : �������� ������ �Ǵ��ϰ��� �ϴ� ���ڿ�
BOOL CCalFileMgr::IsData(CString stData)
{
	CString stTemp = stData;
	stTemp.TrimLeft();
	stTemp.TrimLeft(_T("\r"));
	stTemp.TrimLeft(_T("\n"));
	stTemp.TrimLeft(_T("\t"));
	if(_T("")==stTemp)	
		return FALSE;	
	return TRUE;
}

// �Լ��̸�: WriteConfigFile
// �Լ�����: ���������� ���
// ��ȯ  ��: BOOL 
// ����[STRUCT_CFGDATA CFGDATA] : �����ϰ��� �ϴ� �������� ����ִ� ������
BOOL CCalFileMgr::WriteConfigFile(STRUCT_CFGDATA CFGDATA)
{
//////////////////////////////////////////////////////////////
//���⼭���� ����üũ
	CString stFileName = GetConfigFileName();
	if(_T("") == stFileName)
	{
		LOG(_T("WriteConfigFile() : ���������̸��� �����ϴ�."));				
		return FALSE;
	}
	CString strDir = _T("");
	Get_FILEDIR(strDir);
	ChkDir(strDir);
	stFileName = strDir + stFileName;

	CString stTempFile = stFileName + _T("_Temp");	

	CFile file;
	BOOL bChkError = file.Open(stTempFile,CFile::modeCreate|CFile::modeWrite);	
	if(false==bChkError)
	{
		CString strTmp = stTempFile;
		strTmp = _T("WriteConfigFile(") + stTempFile + _T(") : ���Ͽ��� ����");		
		return FALSE;
	}
//������� ����üũ
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// ���⼭���� ���� ���ϱ��
	CString stData;	
	TCHAR tchar = 0xFEFF;	// �����ڵ�
	file.Write(&tchar, sizeof(TCHAR) );

	// STRUCT_CFGDATA �� ��������� �ٴ����� ���
	stData.Format(_T("%d\r\n"), CFGDATA.nX);
	file.Write(LPCTSTR(stData), stData.GetLength()* sizeof(TCHAR));		
	stData.Format(_T("%d\r\n"), CFGDATA.nY);
	file.Write(LPCTSTR(stData), stData.GetLength()* sizeof(TCHAR));		
	stData.Format(_T("%d\r\n"), CFGDATA.nBorderPos);
	file.Write(LPCTSTR(stData), stData.GetLength()* sizeof(TCHAR));
	stData.Format(_T("%d\r\n"), CFGDATA.nMode);
	file.Write(LPCTSTR(stData), stData.GetLength()* sizeof(TCHAR));	
// ��������� ���� ���ϱ��
//////////////////////////////////////////////////////////////
	if(IsFile(stFileName))
		file.Remove(stFileName);	
	file.Close();
	file.Rename(stTempFile, stFileName);
	return TRUE;
}

// �Լ��̸�: ReadConfigFile
// �Լ�����: ���������� �о��
// ��ȯ  ��: BOOL 
// ����[STRUCT_CFGDATA *pCFGDATA] : �޾ƿ� ������ ����� ������
BOOL CCalFileMgr::ReadConfigFile(STRUCT_CFGDATA *pCFGDATA)
{
//////////////////////////////////////////////////////////////
//���⼭���� ����üũ
	CString stFileName = GetConfigFileName();
	if(_T("") == stFileName)
	{
		LOG(_T("ReadConfigFile() : ���������̸��� �����ϴ�."));		
		return FALSE;
	}	
	CString strDir = _T("");
	Get_FILEDIR(strDir);
	ChkDir(strDir);
	stFileName = strDir + stFileName;

	CFile file;	
	BOOL bChkError = file.Open(stFileName, CFile::modeRead);	
	if(false==bChkError)
	{
		CString strTmp = stFileName;
		strTmp = _T("ReadConfigFile(") + strTmp + _T(") : �������� ���� ����");		
		return FALSE;
	}
//������� ����üũ
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// ���⼭���� ���� �����б�
	TCHAR szBuf[sizeof(STRUCT_CFGDATA)];
	for(UINT i=0;i<sizeof(STRUCT_CFGDATA);i++)
	{
		szBuf[i] = 0;
	}

	TCHAR tchar;
	file.Read(&tchar, sizeof(TCHAR) );
	file.Read(szBuf, file.GetLength());	

	CString stCfg = _T("");
	for(i=0;i<file.GetLength();i++)
	{
		stCfg += szBuf[i];
	}
	
	int nOffset = 0;	
	int nCfg[4] = {0, 0, 0, 0};	
	// 3�� STRUCT_CFGDATA�� ������� ����
	// ������ ������ �� ������ ��ϵǾ� �ִ�.
	for(i=0;i<4;i++)
	{
		nOffset = stCfg.Find(_T("\r\n"));
		if(0>=nOffset)
			nOffset = 0;
		else
		{			
			nCfg[i] = _ttoi(LPTSTR(LPCTSTR(stCfg.Left(nOffset))));
			stCfg = stCfg.Mid(nOffset+2);
		}
	}
// ������� ���� �����б�
//////////////////////////////////////////////////////////////

	pCFGDATA->nX = nCfg[0];
	pCFGDATA->nY = nCfg[1];
	pCFGDATA->nBorderPos = nCfg[2];
	pCFGDATA->nMode = nCfg[3];

	file.Close();
	return TRUE;
}

CString CCalFileMgr::GetConfigFileName()
{
	return CONFIGFILENAME;
}

BOOL CCalFileMgr::WriteLogFile(CString stData, CString stFileName)
{	
	if(_T("") == stFileName)
	{
		stFileName = _T("Kalendar_NoLogFileName.log");
		return FALSE;
	}
	CString strDir = _T("");
	Get_FILEDIR(strDir);
	ChkDir(strDir);
	stFileName = strDir + stFileName;

	CString stTempFile = stFileName + _T("_Temp");	

	CFile file;			
	BOOL bChkError = FALSE;
	if(IsFile(stFileName))
	{
		bChkError = file.Open(stFileName,CFile::modeReadWrite);	
		if(FALSE==bChkError) return FALSE;
	}
	else
	{
		bChkError = file.Open(stFileName,CFile::modeCreate|CFile::modeReadWrite);	
		if(FALSE==bChkError) return FALSE;
		TCHAR tchar = 0xFEFF;
		file.Write(&tchar, sizeof(TCHAR) );		
	}
	file.SeekToEnd();	

	if(MAXBUFFER<stData.GetLength())
	{
		CString st;
		st.Format(_T("���ڼ��� %d�� �̳���  �ۼ��Ͽ� �ֽʽÿ�."), MAXBUFFER);
		AfxMessageBox(st);
		return FALSE;
	}
	stData += _T("\r\n");
	file.SeekToEnd();	
	file.Write(LPCTSTR(stData), stData.GetLength()* sizeof(TCHAR));		
	file.Close();

	return TRUE;
}

CString CCalFileMgr::GetLogFileName()
{
	return LOGFILENAME;
}

BOOL CCalFileMgr::WriteLogFile(CString stData)
{
	return WriteLogFile(stData, GetLogFileName());
}

void CCalFileMgr::DeleteLogFile()
{
	CString strDir = _T("");
	Get_FILEDIR(strDir);
	ChkDir(strDir);
	CString strLogFileName = strDir + GetLogFileName();

	if(TRUE == IsFile(strLogFileName)) ::DeleteFile(strLogFileName);
}

BOOL CCalFileMgr::WriteMemoFile(CData *pData)
{
	UINT uYear = 0;
	UINT uMonth = 0;
	UINT uDay = 0;
	pData->GetDate(&uYear, &uMonth, &uDay);
	CString strFileName = GetMemoFileName(uYear, uMonth, uDay);
	return WriteMemoFile(strFileName, pData->GetText());
}

BOOL CCalFileMgr::ReadMemoFile(CData *pData)
{
	UINT uYear = 0;
	UINT uMonth = 0;
	UINT uDay = 0;	
	pData->GetDate(&uYear, &uMonth, &uDay);
	CString strFileName = GetMemoFileName(uYear, uMonth, uDay);
	
	CString strText = _T("");
	if(FALSE == ReadMemoFile(strFileName, &strText)) return FALSE;
	pData->SetText(strText);
	return TRUE;
}

INT CCalFileMgr::ReadKolasHandleFile(CString strFileName, HWND *hWnd)
{
	if(_T("") == strFileName) { strFileName = GetHandleFileName(); };

	CStdioFile file;	
	BOOL bChkError = file.Open(strFileName, CFile::modeRead);	
	if(FALSE == bChkError)
	{
		CString strTmp = strFileName;
		strTmp = _T("ReadKolasHandleFile") + strFileName + _T(") : ���Ͽ��� ����");
		LOG(strTmp);				
		return -1;
	}	

	CString strText;
	file.ReadString(strText);

	INT nWnd = _ttoi(strText);
	if(NULL != hWnd) *hWnd = HWND(nWnd);
	file.Close();
	
	return 0;
}

CString CCalFileMgr::GetHandleFileName()
{
	return HANDLEFILENAME;
}

// �� ������ �޸������� �����Ѵ�.
int CCalFileMgr::Get_FILEDIR(CString &strFile)
{
 	TCHAR szSpecialPath[MAX_PATH] = {0};
 	SHGetSpecialFolderPath(NULL, szSpecialPath, CSIDL_PERSONAL, FALSE);
	strFile.Format(_T("%s\\Kalendar\\"), szSpecialPath);

	// ������ kolasiii������ �޸������� ���� �ִ� ����ڵ� �ֱ� ������
	// �� ������ kalendar������ ������ kalendar������ ���� �Ŀ� 
	// kolasiii������ �ִ� �޸������� �����Ų��.
	
	CStringArray aFileName;

	CFileFind FindFile;
	BOOL bResult = FindFile.FindFile(strFile+_T("*.*"));	
	if(FALSE == bResult)
	{
 		bResult = CreateDirectory(strFile+_T("\\"),NULL);
 		if(TRUE == bResult)
		{	
			BOOL bIsFile = FindFile.FindFile(_T("..\\cfg\\kalendar\\*.*"));
			while(TRUE == bIsFile)
			{
				bIsFile = FindFile.FindNextFile();
				if(TRUE == FindFile.IsDots())
				{
					continue;
				}
				CString strFileName = FindFile.GetFileName();
				aFileName.Add(strFileName);
			}							
			
			int nCnt = 0;
			const int SIZE = aFileName.GetSize();
			for(nCnt = 0; nCnt < SIZE; nCnt++)
			{
				CString strSrc = _T("..\\cfg\\kalendar\\") + aFileName.GetAt(nCnt);
				CString strDst = strFile + aFileName.GetAt(nCnt);

				CopyFile(strSrc, strDst, FALSE);
			}
		}
	}

	return 0;
}
