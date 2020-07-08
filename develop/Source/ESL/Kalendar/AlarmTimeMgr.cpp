// AlarmTimeMgr.cpp: implementation of the CAlarmTimeMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kalendar.h"
#include "AlarmTimeMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlarmTimeMgr::CAlarmTimeMgr()
{
	
}

CAlarmTimeMgr::~CAlarmTimeMgr()
{

}

// �˶��ð��� ����(�˶��� �Ϸ翡 �ϳ�����)
// �ش��ϴ� ��¥�� ��ġ�ϴ� �˶��� ������ �ش�˶� ����
// ������ �˶��߰�
void CAlarmTimeMgr::SetAlarm(UINT uYear, UINT uMonth, UINT uDay, UINT uHour, UINT uMinute)
{
	int Index = GetIndex(uYear, uMonth, uDay);
	if(-1!=Index)
	{
		ModifyAlarm(Index, uYear, uMonth, uDay, uHour, uMinute);
	}
	else
	{
		CAlarmTime* pAlarmTime = new CAlarmTime;
		pAlarmTime->SetTime(uYear, uMonth, uDay, uHour, uMinute);
		m_aTime.Add(pAlarmTime);		
	}			
}

//i��°�� ��¥ ����
CTime CAlarmTimeMgr::GetTimeAt(int i)
{	
	return GetAlarmTime(i)->GetTime();
}

// �ش糯¥�� �ε����� ����
int CAlarmTimeMgr::GetIndex(UINT uYear, UINT uMonth, UINT uDay) const
{
	CAlarmTime* pTime;
	int Size = m_aTime.GetSize();
	for(int Return=0;Return<Size;Return++)
	{
		pTime = (CAlarmTime *)m_aTime.GetAt(Return);
		if(pTime->IsSameTime(uYear, uMonth, uDay))
			return Return;
	}
	return -1;
}

int CAlarmTimeMgr::GetSize() const
{
	return m_aTime.GetSize();
}

BOOL CAlarmTimeMgr::ReadAlarmFile()
{	
	if( _T("") == GetFileName())
	{
		LOG(_T("ReadAlarmFile() : �����̸��� �����ϴ�."));
		return FALSE;
	}
	if(!IsFile())
		return TRUE;
	CFile file;	
	BOOL bChkError = file.Open(GetFileName(), CFile::modeRead);	
	if(false==bChkError)
	{
		CString strTmp = GetFileName();
		strTmp = _T("ReadAlarmFile(") + strTmp + _T(") : �����б� ����");
		LOG(strTmp);
		return FALSE;
	}

	TCHAR szBuf[MAXBUFFER];
	for(UINT i=0;i<MAXBUFFER;i++)
	{
		szBuf[i] = 0;
	}

	TCHAR tchar;
	file.Read(&tchar, sizeof(TCHAR) );
	// �����ڵ尡 �ƴϸ�
	if(0xFEFF!=tchar)
	{
		file.Close();
		return FALSE;
	}
	file.Read(szBuf, file.GetLength());	

	CString st = _T("");
	for(i=0;i<file.GetLength();i++)
	{
		st += szBuf[i];
	}	

	// �ٴ����� ��� �˶��߰��ϱ�
	int nOffset;
	CString stTemp;
	while(true)
	{
		nOffset = st.Find(_T("\r\n"));
		if(0>=nOffset)
			stTemp = st;
		else
		{
			stTemp = st.Left(nOffset);
			st = st.Mid(nOffset +2);
		}
		if(_T("")!=stTemp)
			AddAlarm(stTemp);
		if(0>=nOffset)
			break;
	}

	// ����
	file.Close();
	return TRUE;
}

BOOL CAlarmTimeMgr::WriteAlarmFile()
{

	if(_T("") == GetFileName())
	{
		LOG(_T("WriteAlarmFile() : �����̸��� �����ϴ�."));		
		return FALSE;
	}
	CString stTempFile = GetFileName() + _T("_Temp");
	CString stData = _T("");
	if(0==m_aTime.GetSize())
	{
		::DeleteFile(GetFileName());
		return TRUE;
	}
	CFile file;
	BOOL bChkError = file.Open(stTempFile,CFile::modeCreate|CFile::modeWrite);	
	if(false==bChkError)
	{
		CString strTmp = _T("WriteAlarmFile(") + stTempFile + _T(") : ���Ͽ��� ����");
		LOG(strTmp);		
		return FALSE;
	}

	for(int i=0;i<m_aTime.GetSize();i++)
	{
		stData += GetAlarmTime(i)->GetStringTime() + _T("\r\n");
	}
	if(MAXBUFFER<stData.GetLength())
	{
		CString st;
		st.Format(_T("�ִ� 1825���� �˶������� �����մϴ�."), MAXBUFFER);
		AfxMessageBox(st);
		return FALSE;
	}

	TCHAR tchar = 0xFEFF;
	file.Write(&tchar, sizeof(TCHAR) );
	file.Write(LPCTSTR(stData), (stData.GetLength()) * sizeof(TCHAR));		
	if(IsFile())
		file.Remove(GetFileName());	
	file.Close();
	file.Rename(stTempFile, GetFileName());
	return TRUE;
}

BOOL CAlarmTimeMgr::IsFile()
{
	CFileFind	find;
	return find.FindFile(GetFileName());
}

CString CAlarmTimeMgr::GetFileName()
{
	CString stFileName;
	CCalFileMgr::Get_FILEDIR(stFileName);
//DEL 	stFileName = FILEDIR;
	stFileName+= ALARMFILENAME;
	return stFileName;
}

void CAlarmTimeMgr::ModifyAlarm(UINT Index, UINT uNewYear, UINT uNewMonth, UINT uNewDay, UINT uNewHour, UINT uNewMinute)
{
	CAlarmTime* pAlarmTime = (CAlarmTime *)m_aTime.GetAt(Index);
	pAlarmTime->SetTime(uNewYear, uNewMonth, uNewDay, uNewHour, uNewMinute);	
}

CAlarmTime* CAlarmTimeMgr::GetAlarmTime(int i)
{
	CAlarmTime* pTime = (CAlarmTime *)m_aTime.GetAt(i);
	return pTime;
}

// ������ �о���� �� ����
void CAlarmTimeMgr::AddAlarm(CString stAlarmTime)
{
	CAlarmTime* pAlarmTime = new CAlarmTime;
	if(pAlarmTime->SetTime(stAlarmTime))
	{
		m_aTime.Add(pAlarmTime);	
	}
	
} 

void CAlarmTimeMgr::DeleteAlarmAt(int i)
{
	m_aTime.RemoveAt(i);	
}
