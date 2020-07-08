// RptUpdateApi.cpp: implementation of the CRptUpdateApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RPTUpdater.h"
#include "RptUpdateApi.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptUpdateApi::CRptUpdateApi()
{
	m_strLastError = _T("");
	m_strOracleFilePath = _T("");
	m_strOraclePathOnly = _T("");

	m_strSeqNo = _T(""); 
	m_strRptAliasList = _T("");
	m_strLibPKList = _T("");
	m_strFailLibPKList = _T("");

	m_strFTPCfgFileName = _T("");
	m_strFTPServerURL = _T("");
	m_strFTPUploadPath = _T("");
	m_strFTPID = _T("");
	m_strFTPPASS = _T("");
	CString m_strFTPPORT = _T("");

}

CRptUpdateApi::~CRptUpdateApi()
{
	FreeAll();
}

INT CRptUpdateApi::InitRptUpdater()
{
	//����Ŭ ����������� RPT������ �������� �о�´� 
	m_listConnectInfo.RemoveAll();

	
	//����Ŭ
	if(LoadConfig()<0)
	{
		AfxMessageBox(_T("����Ŭ ���� ���������� ã�� �� �����ϴ�!"));
		return -1;
	}
	

	//���������� 
	if(LoadLibInfo()<0)
	{
		AfxMessageBox(_T("������ ������ ã�� �� �����ϴ�!"));
		return -1;
	}
	

	//FTP SERVER ��������
	if(ReadFtpCfgFile()<0)
	{
		AfxMessageBox(_T("FTP SERVER ������ ã�� �� �����ϴ�!"));
		return -1;

	}

	//PRTȭ��
	if(LoadRptInfo()<0)
	{
		AfxMessageBox(_T("RPT������ ã�� �� �����ϴ�!"));
		return -1;
	}

	return 0;
}

INT CRptUpdateApi::InitRetrial(CString strSeqNo, 
							   CString strRptAliasList,
							   CString strLibPKList,
							   CString strFailLibPKList,
							   CArray<INT,INT>& arrayFailLibIdx)
{
	//���� ���� ��õ� ���� 
	m_strSeqNo = strSeqNo;
	m_strRptAliasList = strRptAliasList;
	m_strLibPKList = strLibPKList;
	m_strFailLibPKList = strFailLibPKList;


	//���������� 
	CString strQuery;
	strQuery.Format(_T("SELECT PK,LIBRARY_NAME,DB_SERVER_IP,FIREWALL_YN FROM PATCH_LIBRARY_TBL ")
					_T("WHERE PK IN (%s)")
					,strLibPKList);
	
	if(m_DM_MYSQL.RestructDataManager(strQuery)<0) {
		AfxMessageBox(_T("DM�� ������ �� �����ϴ�!"));
		return -1;
	}
	
	INT nRow = m_DM_MYSQL.GetRowCount();

	if(0 == nRow) {
		AfxMessageBox(_T("������ ������ �����ϴ�!"));
		return -1;
	}
	

	//������ ������ IDX ����
	CArray<CString,CString> arrayFailLibPK;
	arrayFailLibPK.RemoveAll();
	GetArrayListFromString(arrayFailLibPK,strFailLibPKList,_T(","));
	
	INT nSize = arrayFailLibPK.GetSize();
	CString strPK;
	CString strCompare;

	for(INT i=0;i<nRow;i++)
	{
		strPK = m_DM_MYSQL.GetCellData(i,0);

		for(INT j=0;j<nSize;j++)
		{
			strCompare = arrayFailLibPK.GetAt(j);
			if(strPK == strCompare)
			{
				arrayFailLibIdx.Add(i);
				break;
			}
		}
	}


	nSize = arrayFailLibIdx.GetSize();
	if(0 == nSize)
	{
		AfxMessageBox(_T("������ ������ ������ �������µ� ���� �߽��ϴ�!"));
		return -1;
	}


	//PRTȭ��
	strQuery.Format(_T("SELECT PK, FLAG, RPTGROUP, RPTALIAS, ENGINEER, DESCRIP, RPTSIZE, BITMAP_ALIAS, ")
					_T("FIRST_DATE, LAST_DATE FROM ESL_SIREPORT_TBL WHERE RPTALIAS IN (%s) AND FLAG != 'U'")
					,strRptAliasList);


	if(m_DM.RestructDataManager(strQuery)<0) {
		AfxMessageBox(_T("DM�� ������ �� �����ϴ�!"));
		return -1;
	}

	INT nCnt = m_DM.GetRowCount();
	
	if(0 == nCnt) {
		AfxMessageBox(_T("RPT���� DB�����Ͱ� �����ϴ�!"));
		return -1;
	}


	return 0;
}

INT CRptUpdateApi::SetUpdateRptData()
{
	//������ ������ �������� tnsnames.oraȭ�Ϸ� �����
	//���õ� rpt�������� file�� ����� 
	//��ȭ���� ��ġ�� �������� ���ؼ� rpt���� file���� FTP Server�� �ø���

	if(MakeTnsnamesFile()<0) return -1;

	if(MakeRptFile()<0) return -1;
	
	if(IsFireWallUse())
	{
		
		if(UploadFilesToFTPServer()<0) {
		
			AfxMessageBox(_T("RPT ���ϵ��� FTP������ UPLOAD�ϴµ� ���� �Ͽ����ϴ�!")); 
			return -1;
		}
		
	}
	

	return 0;
}

INT CRptUpdateApi::LoadConfig()
{

	// ����Ŭ
	TCHAR szBuffer[1024];
	DWORD dwBufLen = sizeof(TCHAR) * 1024;
	ZeroMemory(szBuffer, dwBufLen);

	CString strTemp;

	CString strDefaultHome;
	INT i = -1;
	INT nLastHome = -1;
	INT nHomeCounter = -1;
	BOOL bFindOracleHome = FALSE;
	

	// ������Ʈ���� ����� ��ο��� �о�´�.
	HKEY hKey;
	LPCTSTR lpSubKey = NULL;
	LPCTSTR lpValueName = NULL;
		
	lpSubKey  = _T("SOFTWARE\\ORACLE\\ALL_HOMES");
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
	{
		m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s Ű�� �������� �ʽ��ϴ�."), lpSubKey);
		return -1;
	}

	// DEFAULT_HOME�� �����´�.
	lpValueName = _T("DEFAULT_HOME");
	dwBufLen = sizeof(TCHAR) * 1024;
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s�� %s ���ڿ��� �������� �ʽ��ϴ�."), lpSubKey, lpValueName);
		return -1;
	}
	strDefaultHome.Format(_T("%s"), szBuffer);

	// HOME_COUNTER�� �����´�.
	lpValueName = _T("HOME_COUNTER");
	dwBufLen = sizeof(TCHAR) * 1024;
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s�� %s ���ڿ��� �������� �ʽ��ϴ�."), lpSubKey, lpValueName);
		return -1;
	}
	nHomeCounter = _ttoi(szBuffer);

	// LAST_HOME�� �����´�.
	lpValueName = _T("LAST_HOME");
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s�� %s ���ڿ��� �������� �ʽ��ϴ�."), lpSubKey, lpValueName);
		return -1;
	}

	// LAST_HOME�� ����Ű�� KEY Ȯ��
	strTemp.Format(_T("SOFTWARE\\ORACLE\\HOME%s"), szBuffer);
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strTemp, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
	{
		m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s Ű�� �������� �ʽ��ϴ�."), strTemp);
		return -1;
	}
	nLastHome = _ttoi(szBuffer);

	RegCloseKey( hKey );

	// DEFAULT_HOME�� ����Ű�� KEY Ȯ��
	for (i = 0; i <= nLastHome; i++)
	{
		strTemp.Format(_T("SOFTWARE\\ORACLE\\HOME%d"), i);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strTemp, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
		{
			m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s Ű�� �������� �ʽ��ϴ�."), strTemp);
			return -1;
		}

		// ORACLE_HOME_NAME �� �´��� Ȯ��
		lpValueName = _T("ORACLE_HOME_NAME");
		dwBufLen = sizeof(TCHAR) * 1024;
		if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
		{
			m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s�� %s ���ڿ��� �������� �ʽ��ϴ�."), strTemp, lpValueName);
			return -1;
		}

		if (_tcscmp(szBuffer, strDefaultHome) == 0) 
		{
			bFindOracleHome = TRUE;
			break;		
		}
	}

	if (!bFindOracleHome) 
	{
		m_strLastError = _T("DEFAULT HOME�� ã���� �����ϴ�.");
		return -1;
	}

	// ORACLE_HOME�� Ȯ���Ѵ�. tnsname.ora�� Ȯ�� ����
	lpValueName = _T("ORACLE_HOME");
	dwBufLen = sizeof(TCHAR) * 1024;
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s�� %s ���ڿ��� �������� �ʽ��ϴ�."), strTemp, lpValueName);
		return -1;
	}

	m_strOracleFilePath.Format(_T("%s\\network\\admin\\tnsnames.ora"), szBuffer);
	m_strOraclePathOnly.Format(_T("%s\\network\\admin\\"), szBuffer);

	if(BringCfgData()<0) return -1;

	return 0;
}

INT CRptUpdateApi::LoadLibInfo()
{
	CStdioFile fileLogCfg;
	CString strPath;

	strPath.Format(_T("%sLog.cfg"),CFG_DIRECTORY);
	

	if (!fileLogCfg.Open(strPath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("Log.cfg file�� ã�� �� �����ϴ�!"));
		return -1;
	}
	TCHAR cUni;
	fileLogCfg.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fileLogCfg.SeekToBegin();
	}
	
	CString strLine;
	CString strConnectInfo;

	while(fileLogCfg.ReadString(strLine))
	{
		strLine.TrimRight();
		strLine.TrimRight();
		if(strLine.IsEmpty()) continue;
	
		strConnectInfo = strLine;
	
	}

	m_DM_MYSQL.SetCONNECTION_INFO(strConnectInfo);
	m_DM_RESULT.SetCONNECTION_INFO(strConnectInfo);

	CString sQuery;
	sQuery.Format(_T("SELECT PK,LIBRARY_NAME,DB_SERVER_IP,FIREWALL_YN FROM PATCH_LIBRARY_TBL"));
	
	if(m_DM_MYSQL.RestructDataManager(sQuery)<0) {
		AfxMessageBox(_T("DM�� ������ �� �����ϴ�!"));
		return -1;
	}
	
	INT nRow = m_DM_MYSQL.GetRowCount();

	if(0 == nRow) {
		AfxMessageBox(_T("������ ������ �����ϴ�!"));
		return -1;
	}
	

	
	return 0;
}

INT CRptUpdateApi::SearchLibInfo(CString strName)
{
	CString sQuery;

	if(!strName.IsEmpty())
	{
		sQuery.Format(_T("SELECT PK,LIBRARY_NAME,DB_SERVER_IP,FIREWALL_YN FROM PATCH_LIBRARY_TBL ")
					  _T("WHERE LIBRARY_NAME LIKE '%%%s%%'"),strName);
	}
	else
	{
		sQuery.Format(_T("SELECT PK,LIBRARY_NAME,DB_SERVER_IP,FIREWALL_YN FROM PATCH_LIBRARY_TBL"));
	}
	
	if(m_DM_MYSQL.RestructDataManager(sQuery)<0) {
		AfxMessageBox(_T("DM�� ������ �� �����ϴ�!"));
		return -1;
	}
	
	INT nRow = m_DM_MYSQL.GetRowCount();

	if(0 == nRow) {
		AfxMessageBox(_T("�ش絵������ ã�� �� �����ϴ�!"));
		return -1;
	}

	return 0;
}


INT CRptUpdateApi::BringCfgData()
{
	
	//Source.cfg file�� ������ tnsnames.ora file�� ����� 
	CStdioFile fileTns,fileSource,fileNewTns;
	
	//backup old file
	if (!fileTns.Open(m_strOracleFilePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("���� tnsnames.ora file�� ã�� �� �����ϴ�!"));
		return -1;
	}
	TCHAR cUni;
	fileTns.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fileTns.SeekToBegin();
	}
	
	fileTns.Close();

	CString strBackUpName;
	strBackUpName.Format(_T("%stnsnames_original.ora"),m_strOraclePathOnly);
	


	//backup������ �̹� ������ ��쿡�� ���� tnsnames.oraȭ���� ����� 
	CFileFind finder;
	BOOL bWorking = FALSE;
	bWorking = finder.FindFile(strBackUpName);
	
	if(bWorking)
	{
		fileTns.Remove(m_strOracleFilePath);
	}
	else
	{
		fileTns.Rename(m_strOracleFilePath,strBackUpName);
	}	

	//source
	CString strCfgLine;
	CString strLibName;
	CString strIp;
	INT nPos = -1;
	INT nKey = 0;
	CString strKeyValue;
	CString strLine;
	CString strSource,strSourceTns;

	if (!fileSource.Open(CFG_SOURCE_FILE_PATH, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("source.cfg file�� ã�� �� �����ϴ�!"));
		return -1;
	}

	fileSource.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fileSource.SeekToBegin();
	}
	
	while(fileSource.ReadString(strCfgLine))
	{
		strCfgLine.TrimLeft();
		strCfgLine.TrimRight();
		if(strCfgLine.IsEmpty()) continue;
		
		nPos = strCfgLine.Find(_T("/"));
		if(nPos<0) continue;
	
		strLibName = strCfgLine.Left(nPos);
		strLibName.TrimLeft();
		strLibName.TrimRight();
				
		strIp = strCfgLine.Mid(nPos+1);
		strIp.TrimLeft();
		strIp.TrimRight();

	}
	
	fileSource.Close();

	if(strLibName.IsEmpty() || strIp.IsEmpty()) {
		AfxMessageBox(_T("source ������ ã�� �� �����ϴ�!"));
		return -1;
	}


	//make new file
	if (!fileNewTns.Open(m_strOracleFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		AfxMessageBox(_T("tnsnames.ora file�� ���� ���� �� �����ϴ�!"));
		return -1;
	}
	
	if( 0 == fileNewTns.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fileNewTns.Write( &cUni, sizeof(TCHAR));
	}


			strLine.Format(_T("%s =\r\n")
					   _T("	(DESCRIPTION =\r\n")
					   _T("		(ADDRESS_LIST =\r\n") 
					   _T("			(ADDRESS = (PROTOCOL = TCP)(HOST = %s)(PORT = %s))\r\n") 
					   _T("		)\r\n") 
					   _T("		(CONNECT_DATA =\r\n") 
					   _T("			(SERVICE_NAME = %s)\r\n")
					   _T("		)\r\n")
					   _T("	)\r\n\r\n"),strLibName,strIp,_T("1521"),_T("K2UP"));
		
	
	fileNewTns.WriteString(strLine);



	return 0;
}


INT CRptUpdateApi::LoadRptInfo()
{

	CString sQuery; 

	//������ ���� �Ѵ�
	m_DM.SetCONNECTION_INFO(_T("kolas2@SOURCE/kolas2#[]"));
	
	/*
	sQuery.Format(_T("SELECT PK, FLAG, RPTGROUP, RPTALIAS, ENGINEER, DESCRIP, RPTSIZE, BITMAP_ALIAS, ")
		          _T("FIRST_DATE, LAST_DATE FROM ESL_SIREPORT_TBL"));
	*/

	sQuery.Format(_T("SELECT PK, FLAG, RPTGROUP, RPTALIAS, ENGINEER, DESCRIP, RPTSIZE, BITMAP_ALIAS, ")
		          _T("FIRST_DATE, LAST_DATE FROM ESL_SIREPORT_TBL WHERE FLAG != 'U'"));

	if(m_DM.RestructDataManager(sQuery)<0) {
		AfxMessageBox(_T("DM�� ������ �� �����ϴ�!"));
		return -1;
	}

	INT nCnt = m_DM.GetRowCount();
	
	if(0 == nCnt) {
		AfxMessageBox(_T("RPT���� DB�����Ͱ� �����ϴ�!"));
		return -1;
	}
	
	return 0;
}


INT CRptUpdateApi::SearchPptInfo(CString strOption, INT nType)
{

	CString sQuery; 
	CString sWhere;

	if(0 == nType)
	{
		sWhere.Format(_T("RPTALIAS LIKE '%%%s%%'"),strOption);
	}
	else if(1 == nType)
	{
		sWhere.Format(_T("RPTGROUP LIKE '%%%s%%'"),strOption);
	}

	
	//������ ���� �Ѵ�
	sQuery.Format(_T("SELECT PK, FLAG, RPTGROUP, RPTALIAS, ENGINEER, DESCRIP, RPTSIZE, BITMAP_ALIAS, ")
		          _T("FIRST_DATE, LAST_DATE FROM ESL_SIREPORT_TBL WHERE %s"),sWhere);


	if(m_DM.RestructDataManager(sQuery)<0) {
		AfxMessageBox(_T("DM�� ������ �� �����ϴ�!"));
		return -1;
	}

	INT nCnt = m_DM.GetRowCount();
	
	if(0 == nCnt) {
		AfxMessageBox(_T("�˻� ����� �����ϴ�!"));
		return -1;
	}
	
	return 0;
}

INT CRptUpdateApi::MakeTnsnamesFile()
{
	//���� tnsnames.ora�� �̸��� �ٲٰ�
	//cfg file�� tnsnames.ora������ �����

	CStdioFile fileTns;
	CStdioFile fileNewTns;

	//backup old file
	if (!fileTns.Open(m_strOracleFilePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("���� tnsnames.ora file�� ã�� �� �����ϴ�!"));
		return -1;
	}
	TCHAR cUni;
	fileTns.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fileTns.SeekToBegin();
	}
	
	fileTns.Close();

	CString strBackUpName;
	strBackUpName.Format(_T("%stnsnames_original.ora"),m_strOraclePathOnly);
	

	//backup������ �̹� ������ ��쿡�� ���� tnsnames.oraȭ���� ����� 
	CFileFind finder;
	BOOL bWorking = FALSE;
	bWorking = finder.FindFile(strBackUpName);
	
	if(bWorking)
	{
		fileTns.Remove(m_strOracleFilePath);
	}
	else
	{
		fileTns.Rename(m_strOracleFilePath,strBackUpName);
	}	

	//������ ������ ������ ���� ���ο� tnsnames.ora file�� �����
	CString strCfgLine;
	CString strLibName;
	CString strPK;
	CString strIp;
	CString strFireWallYn;
	INT nPos = -1;
	INT nKey = 0;
	CString strKeyValue;
	INT nIdx = -1;
	
	INT nSize = m_arraySelectedLib.GetSize();
	
	for(INT i=0;i<nSize;i++)
	{
		nIdx = m_arraySelectedLib.GetAt(i);
		
		strPK = m_DM_MYSQL.GetCellData(nIdx,0);
		strPK.TrimLeft();
		strPK.TrimRight();

		strLibName = m_DM_MYSQL.GetCellData(nIdx,1);
		strLibName.TrimLeft();
		strLibName.TrimRight();
				
		strIp = m_DM_MYSQL.GetCellData(nIdx,2);
		strIp.TrimLeft();
		strIp.TrimRight();

		strFireWallYn = m_DM_MYSQL.GetCellData(nIdx,3);
		strIp.TrimLeft();
		strIp.TrimRight();

		if(strPK.IsEmpty() || strLibName.IsEmpty() || strIp.IsEmpty())	continue;
				
		CTns *pTns = new CTns;
		
		pTns->m_strLIB_PK = strPK;
		pTns->m_strLIBNAME = strLibName;
		pTns->m_strHOST = strIp;
		pTns->m_strPORT = _T("1521"); //default
		pTns->m_strSERVICE_NAME = _T("K2UP"); //default
		pTns->m_strFIREWALL_YN = strFireWallYn;

		if(_T("SOURCE") == strLibName)
		{
			pTns->m_strKEY_VALUE = _T("SOURCE");
		}
		else
		{
			nKey++;
			strKeyValue.Format(_T("K2UP_%d"),nKey);
			pTns->m_strKEY_VALUE = strKeyValue;
		}

		m_listConnectInfo.AddTail((CObject*)pTns);
	}
	

	INT nCnt = m_listConnectInfo.GetCount();
	if(nCnt<1) {
		AfxMessageBox(_T("cfg file���� ������ ������ ã�� �� �����ϴ�!"));
		return -1;
	}
	
	
	//source
	CString strLine;
	CString strSource,strSourceTns;
	CStdioFile fileSource;

	if (!fileSource.Open(CFG_SOURCE_FILE_PATH, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("source.cfg file�� ã�� �� �����ϴ�!"));
		return -1;
	}

	fileSource.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fileSource.SeekToBegin();
	}
	
	while(fileSource.ReadString(strCfgLine))
	{
		strCfgLine.TrimLeft();
		strCfgLine.TrimRight();
		if(strCfgLine.IsEmpty()) continue;
		
		nPos = strCfgLine.Find(_T("/"));
		if(nPos<0) continue;
	
		strLibName = strCfgLine.Left(nPos);
		strLibName.TrimLeft();
		strLibName.TrimRight();
				
		strIp = strCfgLine.Mid(nPos+1);
		strIp.TrimLeft();
		strIp.TrimRight();

	}
	
	fileSource.Close();

	if(strLibName.IsEmpty() || strIp.IsEmpty()) {
		AfxMessageBox(_T("source ������ ã�� �� �����ϴ�!"));
		return -1;
	}


	//make new file
	if (!fileNewTns.Open(m_strOracleFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		AfxMessageBox(_T("tnsnames.ora file�� ���� ���� �� �����ϴ�!"));
		return -1;
	}

	if( 0 == fileNewTns.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fileNewTns.Write( &cUni, sizeof(TCHAR));
	}


			strLine.Format(_T("%s =\r\n")
					   _T("	(DESCRIPTION =\r\n")
					   _T("		(ADDRESS_LIST =\r\n") 
					   _T("			(ADDRESS = (PROTOCOL = TCP)(HOST = %s)(PORT = %s))\r\n") 
					   _T("		)\r\n") 
					   _T("		(CONNECT_DATA =\r\n") 
					   _T("			(SERVICE_NAME = %s)\r\n")
					   _T("		)\r\n")
					   _T("	)\r\n\r\n"),strLibName,strIp,_T("1521"),_T("K2UP"));
		
	
	fileNewTns.WriteString(strLine);




	//tns list write
	CTns *pTns = NULL;
	POSITION pos = m_listConnectInfo.GetHeadPosition();
	
	while (pos)
	{
		pTns = (CTns*)m_listConnectInfo.GetNext(pos);
		if (pTns == NULL) continue;

		strLine.Format(_T("%s =\r\n")
					   _T("	(DESCRIPTION =\r\n")
					   _T("		(ADDRESS_LIST =\r\n") 
					   _T("			(ADDRESS = (PROTOCOL = TCP)(HOST = %s)(PORT = %s))\r\n") 
					   _T("		)\r\n") 
					   _T("		(CONNECT_DATA =\r\n") 
					   _T("			(SERVICE_NAME = %s)\r\n")
					   _T("		)\r\n")
					   _T("	)\r\n\r\n"),pTns->m_strKEY_VALUE,pTns->m_strHOST,pTns->m_strPORT,pTns->m_strSERVICE_NAME);
		
		fileNewTns.WriteString(strLine);
	}

	fileNewTns.Close();

	return 0;
}


INT CRptUpdateApi::MakeRptFile()
{

	//file data folder�� �����͸� �ִ´� 
	//list		  -> list.tmp
	//header info -> *.info
	//data        -> *.data
	
	
	INT nSize = m_arraySelectedDMIdx.GetSize();
	INT idx = -1;

	CStdioFile tmpfile,listfile;
	CString strAlias;
	CString strPK;
	CString strInfoFile,strDataFile,strListFile;
	CString strTmp;
	INT nFileSize = 0;
	INT ids = 0;

	
	strListFile.Format(_T("%s\\FileData\\list.tmp"),CFG_DIRECTORY);
	if (!listfile.Open(strListFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		AfxMessageBox(_T("ListFile�� ���� �� �����ϴ�!"));
		return -1;
	}
	if( 0 == listfile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		listfile.Write( &cUni, sizeof(TCHAR));
	}
	
	for(INT i=0;i<nSize;i++)
	{
		idx = m_arraySelectedDMIdx.GetAt(i);
		
		strAlias = _T("");
		strAlias = m_DM.GetCellData(idx,3);
		strTmp = m_DM.GetCellData(idx,6);
		nFileSize = _ttoi(strTmp.GetBuffer(0));

		if(strAlias.IsEmpty()) continue;
		if(0 == nFileSize) continue;
			 
		strInfoFile.Format(_T("%s\\FileData\\%s.info"),CFG_DIRECTORY,strAlias);

		if (!tmpfile.Open(strInfoFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			AfxMessageBox(_T("tmpfile�� ���� �� �����ϴ�!"));
			listfile.Close();
			return -1;
		}
		
		if( 0 == tmpfile.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			tmpfile.Write( &cUni, sizeof(TCHAR));
		}
		strTmp.Format(_T("%s\r\n"),strAlias);
		listfile.WriteString(strTmp);
		
		strPK = _T("");
		strPK = m_DM.GetCellData(idx,0);

		strTmp = _T("");
		strTmp.Format(_T("FLAG : %s\r\n"), m_DM.GetCellData(idx,1));
		tmpfile.WriteString(strTmp);
		
		strTmp = _T("");
		strTmp.Format(_T("RPTGROUP : %s\r\n"), m_DM.GetCellData(idx,2));
		tmpfile.WriteString(strTmp);

		strTmp = _T("");
		strTmp.Format(_T("ENGINEER : %s\r\n"), m_DM.GetCellData(idx,4));
		tmpfile.WriteString(strTmp);

		strTmp = _T("");
		strTmp.Format(_T("DESCRIP : %s\r\n"), m_DM.GetCellData(idx,5));
		tmpfile.WriteString(strTmp);
		
		strTmp = _T("");
		strTmp.Format(_T("RPTSIZE : %d\r\n"), nFileSize);
		tmpfile.WriteString(strTmp);

		
		strTmp = _T("");
		strTmp.Format(_T("BITMAP_ALIAS : %s\r\n"), m_DM.GetCellData(idx,7));
		tmpfile.WriteString(strTmp);
		
	
		strTmp = _T("");
		strTmp.Format(_T("FIRST_DATE : %s\r\n"), m_DM.GetCellData(idx,8));
		tmpfile.WriteString(strTmp);


		strTmp = _T("");
		strTmp.Format(_T("LAST_DATE : %s\r\n"), m_DM.GetCellData(idx,9));
		tmpfile.WriteString(strTmp);

		tmpfile.Close();

		strDataFile.Format(_T("%s\\FileData\\%s.data"),CFG_DIRECTORY,strAlias);

		ids = GetLOBToFile(&m_DM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), nFileSize, strDataFile);
		
	}

	listfile.Close();

	return 0;
}

INT CRptUpdateApi::GetNewSeqNo(CString& strSeqNo)
{
	INT ids = m_DM_MYSQL.MySQL_MakeRecKey(strSeqNo,_T("RPT_SEQ"));
		
	if(ids<0) return -1;


	//�������� ���� ������ ���� ���̺� �Է��Ѵ�. 
	//������KEY�� RPT_ALAIS�� ���ڿ��� ����� 

	CString strLibKeyList;
	CString strRptAliasList;
	INT nIdx = -1;

	INT nSize = m_arraySelectedLib.GetSize();
	if(0 == nSize) return -1;
	
	CString strData; 
	INT nAddCnt = 0;

	for(INT i=0;i<nSize;i++)
	{
		nIdx = m_arraySelectedLib.GetAt(i);
		strData = m_DM_MYSQL.GetCellData(nIdx,0); //Library Key Column
			
		strData.TrimLeft();
		strData.TrimRight();
		
		if(strData.IsEmpty()) continue;

		if(0 == nAddCnt)
		{
			strLibKeyList = strData;
		}
		else
		{
			strLibKeyList += _T(",") + strData;
		}
		
		nAddCnt++;
	}

	

	nSize = m_arraySelectedDMIdx.GetSize();
	if(0 == nSize) return -1;
	
	nAddCnt = 0;

	CString strTmp;
	

	for(i=0;i<nSize;i++)
	{
		nIdx = m_arraySelectedDMIdx.GetAt(i);
		strTmp = m_DM.GetCellData(nIdx,3); //Rpt Alias Column
			
		strTmp.TrimLeft();
		strTmp.TrimRight();
		
		if(strTmp.IsEmpty()) continue;
		
		strData.Format(_T("''%s''"),strTmp);

		if(0 == nAddCnt)
		{
			strRptAliasList = strData;
		}
		else
		{
			strRptAliasList += _T(",") + strData;
		}
		
		nAddCnt++;
	}

	
	CString strQuery;
	strQuery.Format(_T("INSERT INTO RPT_UPDATER_SEQ_INFO_TBL ")
					_T("(SEQ_NUM,TRIAL_CNT,LIB_KEY,RPT_ALIAS,CREATE_DATE,CREATE_TIME,LAST_TRIAL_DATE,LAST_TRIAL_TIME) ")
					_T("VALUES (%s,1,'%s','%s',CURDATE(),CURTIME(),CURDATE(),CURTIME());")
					,strSeqNo,strLibKeyList,strRptAliasList);
	
	ids = m_DM_MYSQL.ExecuteQuery(strQuery,1);
	if(ids<0) return -1;


	return 0;
}

INT CRptUpdateApi::GetTrialCnt(CString strSeqNo,CString& strTrialCnt)
{
	//�ش������� �� ���� Ƚ���� ��´�
	CString strQuery,strResult;
	strQuery.Format(_T("SELECT TRIAL_CNT FROM RPT_UPDATER_SEQ_INFO_TBL"));

	INT ids = -1;
	ids = m_DM_MYSQL.GetOneValueQuery(strQuery,strResult);
	if(ids<0) return -1;

	strResult.TrimLeft();
	strResult.TrimRight();
	
	INT nLastTrial,nNewTrial;
	nLastTrial = nNewTrial = -1;

	nLastTrial = _ttoi(strResult.GetBuffer(0));
	nNewTrial = nLastTrial+1;

	strQuery.Format(_T("UPDATE RPT_UPDATER_SEQ_INFO_TBL ")
					_T("SET TRIAL_CNT = %d,LAST_TRIAL_DATE = CURDATE(),LAST_TRIAL_TIME = CURTIME() ")
					_T("WHERE SEQ_NUM = %s;")
					,nNewTrial,strSeqNo);


	m_DM_MYSQL.AddFrame(strQuery);

	ids = m_DM_MYSQL.SendFrame();
	if(ids<0) return -1;

	strTrialCnt.Format(_T("%d"),nNewTrial);

	return 0;
}

INT CRptUpdateApi::ClearTmpData()
{
	
	//filedata delete
	CStdioFile file;
	CFileFind finder;
	CString strFileName;
	CString strFilePath;
	strFilePath.Format(_T("%s\\FileData\\*.*"),CFG_DIRECTORY);
	CString strPath;
	strPath.Format(_T("%s\\FileData\\"),CFG_DIRECTORY);

	BOOL bWorking = FALSE;
	bWorking = finder.FindFile(strFilePath);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();   

		if(finder.IsDots()) continue; 
			
		if (finder.IsDirectory()) continue;


		strFileName = finder.GetFileName();
		
		strFileName = strPath + strFileName;
		file.Remove(strFileName);
	}


	//tnsnames.ora ����
	CString strBackUpName;
	strBackUpName.Format(_T("%stnsnames_original.ora"),m_strOraclePathOnly);
	
	bWorking = FALSE;
	bWorking = finder.FindFile(strBackUpName);
	
	if(bWorking) //backup������ ���� ��� ���� 
	{
		file.Remove(m_strOracleFilePath);
		file.Rename(strBackUpName,m_strOracleFilePath);
	}

	return 0;
}



INT CRptUpdateApi::FetchOracle()
{
	CString strLine;
	CStdioFile file;
	// ����Ŭ
	if (!file.Open(m_strOracleFilePath, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(m_strOracleFilePath, CFile::modeRead | CFile::modeCreate | CFile::typeBinary))
		{
			m_strLastError = _T("������ ���� �����ϴ�.");
			return -1;
		}
		else
		{
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}
			file.Close();
			
			return 0;
		}
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	DWORD dwLen = file.GetLength() * sizeof(TCHAR);
	if (dwLen == 0)
	{
		file.Close();
		return 0;
	}
	TCHAR *szBuffer = new TCHAR[dwLen];
	ZeroMemory(szBuffer, dwLen);

	file.ReadHuge(szBuffer, dwLen);
	file.Close();

	
	CString strFileData;
	strFileData.Format(_T("%s"), szBuffer);

	if (szBuffer) delete [] szBuffer;

	// # ����
	INT nFind = strFileData.Find(_T("#"));
	while (nFind >= 0)
	{
		strFileData.Delete(nFind, strFileData.Find(_T("\r\n"), nFind) - nFind + 1);
		nFind = strFileData.Find(_T("#"));
	}

	TCHAR ch;
	INT n = strFileData.Find(_T("("));
	INT nCount = 1;
	INT nStart = 0;

	INT nFileLen = strFileData.GetLength();

	while (TRUE)
	{
		if (nFileLen - 1 < ++n) break;
		ch = strFileData.GetAt(n);
		if (ch == '(')
			nCount++;
		else if (ch == ')')
			nCount--;

		if (nCount == 0)
		{
			CString strOneData = strFileData.Mid(nStart, n - nStart + 1);
			strOneData.TrimLeft	();
			strOneData.TrimRight();
			if (!strOneData.IsEmpty())
			{
				CTns *pTns = new CTns;
				
				
				// KEY VALUE
				CString strKeyValue = strOneData.Mid(0, strOneData.Find(_T("=")));
				strKeyValue.TrimLeft();
				strKeyValue.TrimRight();
				

				pTns->m_strKEY_VALUE = 	strKeyValue;
			
				// HOST
				GetTnsValue(strOneData, _T("HOST"), pTns->m_strHOST);

				// PORT
				GetTnsValue(strOneData, _T("PORT"), pTns->m_strPORT);

				// SERVICE_NAME
				GetTnsValue(strOneData, _T("SERVICE_NAME"), pTns->m_strSERVICE_NAME);


				//m_mapTns.SetAt(strKeyValue, (VOID*)pTns);
				m_listConnectInfo.AddTail((CObject*)pTns);
			}

			nStart = ++n;
			n = strFileData.Find(_T("("), n);
			nCount++;
			if (n < 0) break;
		}
	}
	
	return 0;
}

INT CRptUpdateApi::GetTnsValue(CString strTns, CString strKey, CString &strValue)
{
	if (strTns.IsEmpty() || strKey.IsEmpty()) return -1;

	INT nFind = strTns.Find(strKey);
	if (nFind > 0)
	{
		INT a = strTns.Find(_T("="), nFind);
		INT b = strTns.Find(_T(")"), a);
		
		if (a >= 0 && b >= 0)
		{
			strValue = strTns.Mid(a + 1, b - a - 1);
			strValue.TrimLeft();
			strValue.TrimRight();
		}
	}

	return 0;
}

VOID CRptUpdateApi::FreeAll()
{
	//tns list
	CTns *pTns = NULL;
	POSITION pos = m_listConnectInfo.GetHeadPosition();
	while (pos)
	{
		pTns = (CTns*)m_listConnectInfo.GetNext(pos);
		if (pTns == NULL) continue;

		delete pTns;
		pTns = NULL;
	}

	m_listConnectInfo.RemoveAll();


	//DM
	m_DM.FreeData();

	
	//ETC data
	m_arraySelectedLib.RemoveAll();
	m_arraySelectedDMIdx.RemoveAll();

}

INT CRptUpdateApi::GetLOBToFile(CESL_DataMgr *pDM, CString table_name, CString PKFieldname, CString PKFieldtype, CString PKData, CString FieldName, INT Size, CString FileName)
{

	BYTE *pBuffer;
	INT nBufferSize = Size + 1024;
	INT ids;

	pBuffer = (BYTE*)malloc(sizeof(BYTE) * nBufferSize);
	if (!pBuffer) {
		AfxMessageBox(_T("Malloc Memory Fail!"));
		return -1024;
	}
	
	ids = pDM->LOBGet(table_name, PKFieldname, PKFieldtype, PKData, FieldName, pBuffer, &nBufferSize);
	if (ids < 0) {
		AfxMessageBox(_T("Get Lob Data Fail!"));
		free(pBuffer);
		return -1024;
	}
	if (nBufferSize <= 0) {
		AfxMessageBox(_T("Empty Lob Data!"));
		return -1024;
	}
	CFile f;
    if( !f.Open(FileName, CFile::modeWrite|CFile::modeCreate | CFile::typeBinary) ) {
		free(pBuffer);
		AfxMessageBox(_T("Create Lob File Fail"));
		return - 1025;
	}
	if( 0 == f.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		f.Write( &cUni, sizeof(TCHAR));
	}
		
	f.WriteHuge(pBuffer, nBufferSize);
	f.Close();
	free (pBuffer);

	return 0;
}

INT CRptUpdateApi::GetArrayListFromString(CArray<CString,CString>& arrayList,
										  CString strString,
										  CString strDelim)
{
	
	INT ids = -1;
	CString strTmp;

	while(1)
	{
		ids = GetParseString(strDelim,strString,strTmp);
		if(ids<0) break;
		
		strTmp.TrimLeft();
		strTmp.TrimRight();
		
		if(strTmp.IsEmpty()) continue;
		
		arrayList.Add(strTmp);
	}

	return 0;
}

INT CRptUpdateApi::GetParseString(CString strDelim,CString& strList,CString &strOutput)
{
	strList.TrimLeft();
	strList.TrimRight();

	INT nPos = strList.Find(strDelim);

	if(nPos<0)
	{
		if(strList.IsEmpty()) 
		{
			return -1;
		}
		else
		{
			strOutput = strList;
			strList.Empty();
		}
	}
	else
	{
		
		INT lastidx = strList.GetLength();
		INT startidx = nPos + 1;
		
		strOutput = strList.Left(nPos);
		strOutput.TrimLeft();
		strOutput.TrimRight();
		if(strOutput.IsEmpty()) return -1;

		strList = strList.Right(lastidx - startidx);
	}

	return 0;
}

INT CRptUpdateApi::GetUpdateResult(CString strSeqNo,INT& nConFailCnt,INT& nProcFailCnt)
{
	m_arraySelectedRpt.RemoveAll();
	m_arrayResult.RemoveAll();
	
	
	//tmpȭ���� �о� Rpt������ �����Ѵ�
	CStdioFile listfile;
	CString strListFile;

	strListFile.Format(_T("%s\\FileData\\list.tmp"),CFG_DIRECTORY);
	if (!listfile.Open(strListFile, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("ListFile�� ���� �� �����ϴ�!"));
		return -1;
	}
	TCHAR cUni;
	listfile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		listfile.SeekToBegin();
	}
	
	CString strLine;
	while(listfile.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		
		if(strLine.IsEmpty()) continue;
		
		m_arraySelectedRpt.Add(strLine);
	}
	
	
	CString msg;

	//�ش� ������ log�� DB���� �����´�
	CString strQuery;
	strQuery.Format(_T("SELECT L.PK, L.LIBRARY_NAME, R.QUERY_TYPE , R.RESULT ")
					_T("FROM PATCH_LIBRARY_TBL L, RPT_UPDATER_LOG_TBL R ")
					_T("WHERE L.PK = R.LIB_KEY AND R.SEQ_NUM = %s ORDER BY L.PK")
					,strSeqNo);

	m_DM_RESULT.RestructDataManager(strQuery);


	INT nRow = m_DM_RESULT.GetRowCount();
	
	if(0 == nRow)
	{
		msg.Format(_T("[%s] ������ ���� log�� �����ϴ�!"));
		AfxMessageBox(msg);
	}
	
	CString strCurLibName;
	CString strLibName;

	strCurLibName = _T("");

	CString strPK;
	CString strCurPK;
	INT nStatus = -1;
	CString strResult;
	CString strQueryType;

	INT nConFail = 0;
	INT nProcFail = 0;
	INT nCurIdx = 0;
	INT nTotalConFail =0;
	INT nTotalProcFail = 0;
	CString strOutput;

	for(INT i=0;i<nRow;i++)
	{
		
		if(0 == i) {
			strCurPK = m_DM_RESULT.GetCellData(i,0);
			strCurLibName = m_DM_RESULT.GetCellData(i,1);
		}

		strPK = _T("");
		strPK = m_DM_RESULT.GetCellData(i,0);
		nCurIdx = i;
		
		if(strPK != strCurPK) //���� �� array�� �߰� 
		{
			if(nConFail>0)
			{
				if(nProcFail>0)
				{
					strOutput.Format(_T("��      %s"),strCurLibName);
					nTotalProcFail++;
				}
				else
				{
					strOutput.Format(_T("��      %s"),strCurLibName);
					nTotalConFail++;
				}

			}
			else 
			{
				if(nProcFail>0)
				{
					strOutput.Format(_T("��      %s"),strCurLibName);
					nTotalProcFail++;
				}
			}

			
			if(0 == nConFail && 0 == nProcFail)
			{
				strOutput.Format(_T("%s"),strCurLibName);
			}

			m_arrayResult.Add(strOutput);

			nConFail = 0;
			nProcFail = 0;
		}


		//log �հ� 
		strCurPK = m_DM_RESULT.GetCellData(i,0);
		strCurLibName = m_DM_RESULT.GetCellData(i,1);
		strQueryType = m_DM_RESULT.GetCellData(i,2);
		strResult = m_DM_RESULT.GetCellData(i,3);

		if(_T("T") == strQueryType) 
		{
			if(_T("0") == strResult) nConFail++;
		}
		else
		{
			if(_T("3") != strResult) nProcFail++;
		}
		

	}
	

	if(nConFail>0)
	{
		if(nProcFail>0)
		{
			strOutput.Format(_T("��      %s"),strCurLibName);
			nTotalProcFail++;
		}
		else
		{
			strOutput.Format(_T("��      %s"),strCurLibName);
			nTotalConFail++;
		}

	}
	else 
	{
		if(nProcFail>0)
		{
			strOutput.Format(_T("��      %s"),strCurLibName);
			nTotalProcFail++;
		}
	}

			
	if(0 == nConFail && 0 == nProcFail)
	{
		strOutput.Format(_T("%s"),strCurLibName);
	}

	m_arrayResult.Add(strOutput);



	
	nConFailCnt = nTotalConFail;
	nProcFailCnt = nTotalProcFail;

	return 0;
}


INT CRptUpdateApi::ReadFtpCfgFile()
{
	// FTP SERVER ������ �д´�.
	m_strFTPCfgFileName = _T("..\\cfg\\rptupdater\\ftp.cfg");

	// ���� ������ �д´�.
	CStdioFile file;
	if (!file.Open(m_strFTPCfgFileName, CFile::modeRead | CFile::typeBinary)) return -1;

	CString strKey;
	CString strValue;
	CString strLine;
	INT nFind = -1;
	INT ids = -1;
	
	file.SeekToBegin();
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty()) continue;
		if (strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

		nFind = strLine.Find('=');
		if (nFind < 0) continue;

		ids = _stscanf(strLine.GetBuffer(0), _T("%s = %s"), strKey.GetBuffer(256), strValue.GetBuffer(256));
		strKey.ReleaseBuffer();
		strValue.ReleaseBuffer();

		if (ids == 0 || ids == _TEOF) continue;

		strKey.TrimLeft();
		strKey.TrimRight();

		strValue.TrimLeft();
		strValue.TrimRight();

		if (strKey.IsEmpty() || strValue.IsEmpty()) continue;

		if (strKey == _T("FTP_URL"))
			m_strFTPServerURL = strValue;
		else if (strKey == _T("FTP_UPLOAD_PATH"))
			m_strFTPUploadPath = strValue;
		else if (strKey == _T("FTP_PORT"))
			m_strFTPPORT = strValue;	
		else if (strKey == _T("ID"))
			m_strFTPID = strValue;
		else if (strKey == _T("PASS"))
			m_strFTPPASS = strValue;
	

	}
		
	file.Close();

	return 0;
}

INT CRptUpdateApi::UploadFilesToFTPServer()
{
	//�ӽ� ���ϵ��� FTP Server�� �ø���

	//file list�� �о�´�
	CString msg;
	CString strLine;
	CStdioFile fileList;
	CString strListFile;
	CArray<CString,CString> arrayRptAlias;

	strListFile.Format(_T("%s\\FileData\\list.tmp"),CFG_DIRECTORY);
	if (!fileList.Open(strListFile, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("List File�� ���� �� �����ϴ�"));
		return -1;
	}
	TCHAR cUni;
	fileList.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fileList.SeekToBegin();
	}
	while(fileList.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		if(strLine.IsEmpty()) continue;

		arrayRptAlias.Add(strLine);
	}
	

	//list		  -> list.tmp
	//header info -> *.info
	//data        -> *.data

	CFtpConnection* pFtpCon = NULL;
	CInternetSession* pSession = NULL;
	pSession = new CInternetSession(_T("My Session"));
	

	INT nPort = _ttoi(m_strFTPPORT.GetBuffer(0));

	try
	{
		pFtpCon = pSession->GetFtpConnection(m_strFTPServerURL,m_strFTPID,m_strFTPPASS,nPort,TRUE);

	}
	catch(CInternetException* e)
	{
		msg.Format(_T("Caught Internet Exception : (%d)\r\n"),e->m_dwError);
		AfxMessageBox(msg);

		pSession->Close();

		if(pSession)
		{
			delete pSession;
			pSession = NULL;
		}

		if(pFtpCon) 
		{
			delete pFtpCon;
			pFtpCon = NULL;
		}

		e->Delete();

		return -1; //connect fail
	}

	
	// ���� ����� ������ ��Ʈ �н��� ���ϴ� �κ��Դϴ�.
	CString strOriginFTPDir;
	pFtpCon->GetCurrentDirectory(strOriginFTPDir);
	if(strOriginFTPDir != _T("/")) pFtpCon->SetCurrentDirectory(_T("/"));

	CString strServeFile;
	CString strClientFile;
	CString strRptAlias;
	CString strFileName;


	for(INT i=0;i<arrayRptAlias.GetSize();i++)
	{
		
		for(INT j=0;j<2;j++)
		{

			strRptAlias = arrayRptAlias.GetAt(i);
			
			if(0 == j) strFileName.Format(_T("%s.info"),strRptAlias);
			else strFileName.Format(_T("%s.data"),strRptAlias);

			strServeFile = m_strFTPUploadPath + strFileName; 
			strClientFile.Format(_T("%s\\FileData\\%s"),CFG_DIRECTORY,strFileName);

			try
			{
				pFtpCon->PutFile(strClientFile, strServeFile);
			}
			catch(CInternetException* e)
			{
				msg.Format(_T("Caught Internet Exception : (%d)\r\n"), e->m_dwError);
				AfxMessageBox(msg);

				pSession->Close();

				if(pSession) 
				{
					delete pSession;
					pSession = NULL;
				}

				if(pFtpCon) 
				{
					delete pFtpCon;
					pFtpCon = NULL;
				}

				//e->ReportError();
				e->Delete();

				return -2; //file upload fail
			}
			
		}

	}
	
	pSession->Close();

	if(pSession) {
		delete pSession;
		pSession = NULL;
	}
			
	if(pFtpCon) 
	{
		delete pFtpCon;
		pFtpCon = NULL;
	}

	
	return 0;
}

BOOL CRptUpdateApi::IsFireWallUse()
{
	//��ȭ���� ����ϴ� �������� �����Ǿ����� ���θ� �ľ��Ѵ�
	
	CTns *pTns = NULL;
	POSITION pos = m_listConnectInfo.GetHeadPosition();
	while (pos)
	{
		pTns = (CTns*)m_listConnectInfo.GetNext(pos);
		if (pTns == NULL) continue;

		if(_T("Y") == pTns->m_strFIREWALL_YN) return TRUE;
	}

	return FALSE;
}