// BO_ACQ_FileImportMgr.cpp: implementation of the CBO_ACQ_FileImportMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_ACQ_FileImportMgr.h"
#include <direct.h>
#include "../../../����/INCLUDE/BO_ACQ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("���Ϲ��Թ���"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_ACQ_FileImportMgr::CBO_ACQ_FileImportMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_FileImportMgr::IDD, pParent)
{
	m_pParent = pParent;
}

CBO_ACQ_FileImportMgr::~CBO_ACQ_FileImportMgr()
{

}

INT CBO_ACQ_FileImportMgr::GetFilePath()
{
	//����ڰ� ������ ������ ��θ� ���´�.
	ITEMIDLIST        *pidlBrowse;
    TCHAR            pszPathname[MAX_PATH];
 	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	BROWSEINFO BrInfo;
    BrInfo.pidlRoot = NULL;

    memset( &BrInfo, 0, sizeof(BrInfo) );
    BrInfo.pszDisplayName = pszPathname;
    BrInfo.lpszTitle = _T("������ ����� ������ ��ġ�� �����ϼ���");
    BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

    // ���̾�α׸� ����
    pidlBrowse = ::SHBrowseForFolder(&BrInfo);    

    if( pidlBrowse != NULL)
    {
        // �н��� ����
        ::SHGetPathFromIDList(pidlBrowse, pszPathname);    
		_tchdir(szCurrentDir);   
    }
	else
	{
		_tchdir(szCurrentDir);   
		return -1;
	}

	m_strImportFilePath = pszPathname;

	return 0;

}

INT CBO_ACQ_FileImportMgr::ReadMasterFileInfo()
{
	// ��ο��� ������ ������ ����.
	BOOL bError;
	INT nStart = -1;
	CString strFilePath, strLine, strParam;
	CStdioFile MasterFile;

	strFilePath.Format(_T("%s\\ExportFileInfo.txt"), m_strImportFilePath);
	bError = MasterFile.Open(strFilePath, CFile::modeRead | CFile::typeBinary);
	if(!bError){
		AfxMessageBox(_T("������ ������ �����ϴ�."));
		return -1;
	}

		TCHAR cUni;
		MasterFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			MasterFile.SeekToBegin();
		}

	m_nTableCount = 0;
	// ��Ÿ�Կ� ���� ������ ������ ������ �о�´�.
	switch((SPECIES_TYPE)m_nSpeciesType)
	{
		case BO_REQUEST:
			// ��������������.
			while(MasterFile.ReadString(strLine))
			{
				strLine.TrimLeft(); strLine.TrimRight();
				strParam = strLine.Left(1);
				if(strParam == _T("#"))
				{
					strLine.TrimLeft(_T("#"));
					nStart = strLine.Find(_T("="));
					if(nStart < 0) {
						MasterFile.Close();
						return -1;
					}
					strParam = strLine.Left(nStart);
					if(strParam == _T("������Ÿ��")) {
						strParam = strLine.Mid(nStart+1);
						if(strParam != _T("BO_REQUEST")) {
							MasterFile.Close();
							return -1;
						}
					}
					else if(strParam == _T("�����׸�")) {
						strParam = strLine.Mid(nStart+1);
						m_CheckColumnArray.Add(strParam);
					}
					else return -1;
				}
				else
				{
					m_strTableNameArray[m_nTableCount] = strLine;	
					m_nTableCount++;
				}
			}
			break;

		case BO_ORDER:
			// ��������������.
			while(MasterFile.ReadString(strLine))
			{
				strLine.TrimLeft(); strLine.TrimRight();
				strParam = strLine.Left(1);
				if(strParam == _T("#"))
				{
					strLine.TrimLeft(_T("#"));
					nStart = strLine.Find(_T("="));
					if(nStart < 0) {
						MasterFile.Close();
						return -1;
					}
					strParam = strLine.Left(nStart);
					if(strParam == _T("������Ÿ��")) {
						strParam = strLine.Mid(nStart+1);
						if(strParam != _T("BO_ORDER")) {
							MasterFile.Close();
							return -1;
						}
					}
					else if(strParam == _T("���Գ⵵")) m_strAcqYear = strLine.Mid(nStart+1);
					else if(strParam == _T("������ȣ")) m_strGroupInfo = strLine.Mid(nStart+1);
					else if(strParam == _T("��������")) m_strWorkCode = strLine.Mid(nStart+1);
					else return -1;
				}
				else
				{
					m_strTableNameArray[m_nTableCount] = strLine;	
					m_nTableCount++;
				}
			}
			break;

		default:
			MasterFile.Close();
			return -1;
			break;
	}

	MasterFile.Close();
	return 0;
}

INT CBO_ACQ_FileImportMgr::FileImportProc()
{

	INT ids;
	// ������ ����� ��θ� ���´�.
	ids = GetFilePath();
	if(ids < 0) return -1;

	// ������ ������ ������ �о�´�. 
	ids = ReadMasterFileInfo();
	if(ids < 0) return -1;

	// �ʿ��� ��� ������ �����ϴ��� Ȯ���Ѵ�.
	ids = IsExistFile();
	if(ids < 0) return -1;

	// SM �ҷ�����.
	if (InitESL_Mgr(_T("K2UPGRADE_����_�ڷ����")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return -1;
	}			

	// �˼��Ϸ� �ڷ��� ��쿡�� ������ �ڷ��� ���Գ⵵, ����, ������ȣ�� ������ ������ �̹� �����ϴ��� Ȯ���Ѵ�.
	if((SPECIES_TYPE)m_nSpeciesType == BO_ORDER)
	{
		CString strTmp;
		strTmp.Format(_T("���Գ⵵ : %s\r\n")
			          _T("�������� : %s\r\n")
					  _T("������ȣ : %s\r\n")
					  _T("�� �ڷḦ �����Ͻðڽ��ϱ�?"),
					  m_strAcqYear, m_strWorkCode, m_strGroupInfo);
		ids = AfxMessageBox(strTmp, MB_ICONQUESTION|MB_YESNO);
		if( ids == IDNO) return -1;

		ids = CheckImportInfo();
		if(ids <0 ) return -1;
	}

	BOOL bError;
	CString strBatFile, strLine;
	CStdioFile BatFile;

	// ��ġ������ �����.
	strBatFile.Format(_T("%s\\ImportFile.bat"), m_strImportFilePath);
	bError = BatFile.Open(strBatFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
	if(!bError) {
		AfxMessageBox(_T("���� ���� ����!"));
		return -1;
	}

	if( 0 == BatFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		BatFile.Write( &cUni, sizeof(TCHAR));
	}
		
	// ��ġ������̺� ���� rec_key�� ���� ���ڵ尡 ������ ������ �� ���Թ޴´�.
	ids = DeleteSameRecKeyInFurnishTbl();
	if(ids < 0) return -1;

	// ��ġ���Ͽ� sqlldr ���� ��ɾ �����Ѵ�.
	for(INT i = 0; i < m_nTableCount; i++)
	{
		strLine.Empty();
		strLine.Format(_T("sqlldr kolas2/kolas2#@k2up control='%s\\%s.ctl' data='%s\\%s.dat'\r\n"), 
			            m_strImportFilePath, m_strTableNameArray[i], m_strImportFilePath, m_strTableNameArray[i]);
		BatFile.WriteString(strLine);
	}

	BatFile.Close();

	// ��ġ������ �����Ų��.
	HINSTANCE error;
	error = ShellExecute(NULL,_T("OPEN"), strBatFile,NULL,NULL,SW_SHOW);
	ids = (INT)error;
	if(ids < 32)  {
		AfxMessageBox(_T("���� ���� ����!"));
		return -1;
	}

	// 2005-01-07 sqlLoader ���� �� �ణ�� Sleep �ð��� �־�� commit �ȴ�.
	Sleep(1000);

	// ��ġ����ڷ��� ��� ���������Ͽ��� �����׸��� �о�ͼ� �����׸��� �ƴ� �÷����� ������ ���� NULL�� Update�Ѵ�.
	if((SPECIES_TYPE)m_nSpeciesType == BO_REQUEST)
	{
		ids = UpdateNotImportColumn();
		if(ids < 0)  {
			AfxMessageBox(_T("���� ���� ����!"));
			return -1;
		}
	}
	// 2005.01.04 
	// ������ȣ���� ���̺��� ������ ������ȣ�� ���Թ��� ������ȣ�� Insert�Ѵ�.
	else if((SPECIES_TYPE)m_nSpeciesType == BO_ORDER)
	{
		CESL_DataMgr * TmpDM;    
		TmpDM = FindDM(_T("DM_����_�ڷ����_������"));
		CString strQuery, result;
		strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
			            _T("WHERE KIND_CODE = 'PUR_NO' AND PUBLISH_FORM_CODE = 'MO' AND MAKE_YEAR = '%s' AND DIVISION_VALUE = '1' AND MANAGE_CODE = UDF_MANAGE_CODE"),
						m_strAcqYear);
		TmpDM->GetOneValueQuery(strQuery, result);
		// 2005-01-19 m_strGroupInfo�� ����. �Ǽ���...��.,�� 
		if( _ttoi(result) < _ttoi(m_strGroupInfo) )
		{

			TmpDM->InitDBFieldData();
			TmpDM->StartFrame();

			strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' ")
							_T("WHERE KIND_CODE = 'PUR_NO' AND PUBLISH_FORM_CODE = 'MO' AND MAKE_YEAR = '%s' AND DIVISION_VALUE = '1' AND MANAGE_CODE = UDF_MANAGE_CODE;"),
							m_strGroupInfo, m_pParent->GetWorkLogMsg(WORK_LOG), m_strAcqYear);	

			TmpDM->AddFrame(strQuery); 
			TmpDM->SendFrame();  // ������ sqlLoder�� �̹� �Է��ؼ� SendFrame�� �����ȵǵ� ��¿�� ����.. ������..
			TmpDM->EndFrame();
		}

		// 2005-01-17
		// IDX_BO_TMP_TBL�� Insert�� ���� �����ؼ� DB�� ���� ������.
		ids = SendQueryToIDX_TMP();
		if(ids <0) return -1;

	}

	return 0;

}

INT CBO_ACQ_FileImportMgr::IsExistFile()
{

	BOOL bError;
	CString strFilePath;
	CStdioFile  File;

	// �ʿ��� ���ϵ��� ��� �����ϴ��� üũ�Ѵ�.
	for( INT i=0; i< m_nTableCount; i++)
	{
		strFilePath.Format(_T("%s\\%s.ctl"), m_strImportFilePath, m_strTableNameArray[i]);
		bError = File.Open(strFilePath, CFile::modeRead | CFile::typeBinary);
		if(!bError){
			AfxMessageBox(_T("������ �������� �ʰų� ������ ������ �����ϴ�."));
			return -1;
		}
		else File.Close();

		strFilePath.Format(_T("%s\\%s.dat"), m_strImportFilePath, m_strTableNameArray[i]);
		bError = File.Open(strFilePath, CFile::modeRead | CFile::typeBinary);
		if(!bError){
			AfxMessageBox(_T("������ �������� �ʰų� ������ ������ �����ϴ�."));
			return -1;
		}
		else File.Close();	
	}

	return 0;
}

/************************************************************************************************
2004.11.04 ��ġ����ڷ���� ���Կ��� rec_key�� �ߺ��Ǵ� ���ڵ�� ���� ������ ���̺��� �����Ѵ�.
************************************************************************************************/
INT CBO_ACQ_FileImportMgr::DeleteSameRecKeyInFurnishTbl()
{
	BOOL bError;
	CString strFilePath, strQuery;
	CStdioFile DataFile;

	strFilePath.Format(_T("%s\\BO_FURNISH_TBL.dat"), m_strImportFilePath);
	bError = DataFile.Open(strFilePath, CFile::modeRead | CFile::typeBinary);
	if(!bError){
		AfxMessageBox(_T("������ ������ �����ϴ�."));
		return -1;
	}

		TCHAR cUni;
		DataFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			DataFile.SeekToBegin();
		}
	
	CString strLine, strRecKey;
	INT nStart, nEnd, ids;

	CESL_DataMgr * TmpDM;    
	TmpDM =FindDM(_T("DM_����_�ڷ����_��ġ�������"));
	TmpDM->StartFrame();

	while(DataFile.ReadString(strLine))
	{
		strLine.TrimLeft(); strLine.TrimRight();
		nStart = strLine.Find(_T("<start>"));
		if(nStart < 0) {
			DataFile.Close();
			return -1;
		}
		nEnd = strLine.Find(_T("<end>"));
		if(nEnd < 0) {
			DataFile.Close();
			return -1;
		}
		strRecKey = strLine.Mid((nStart+7), (nEnd-(nStart+7)));
		m_DataReckeyArray.Add(strRecKey);

		strQuery.Format(_T("DELETE FROM BO_FURNISH_TBL WHERE REC_KEY = %s;"), strRecKey);
		ids = TmpDM->AddFrame(strQuery);
		//ids = TmpDM->MakeDeleteFrame(TmpDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
		if(ids < 0) 
			return -1;
	}

	ids = TmpDM->SendFrame();
	if(ids < 0) {
		TmpDM->EndFrame();
		return -1;
	}
	TmpDM->EndFrame();

	return 0;
}

/***************************************************************************************************************
2004.11.05 ��ġ����ڷ���� ���Կ��� ������ ������ ������ �� �����׸��� �ƴ� �÷����� ������ ��� NULL�� �ٲ۴�.
           �ҽ� �����̴�. ���߿� �ٽ� ¥��..(��.,��)
***************************************************************************************************************/
INT CBO_ACQ_FileImportMgr::UpdateNotImportColumn()
{
	CString	strExportList[100]; // �����׸� ��ü ����Ʈ
	CString strColumnName[100]; // DB �÷��� 
	CString strNullYN[100];		// NULL�� ���� �׸����� ���� �÷���

	// �����׸�� ������ DB�� �÷���
	INT nColumnCount = 0;
	strExportList[nColumnCount] = _T("����");			strColumnName[nColumnCount] = _T("ǥ��");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("����");			strColumnName[nColumnCount] = _T("���ڸ�");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("������");			strColumnName[nColumnCount] = _T("������");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("�����");			strColumnName[nColumnCount] = _T("�����");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("����");			strColumnName[nColumnCount] = _T("����");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("��õ�ǰ�");		strColumnName[nColumnCount] = _T("��õ�ǰ�");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("���");			strColumnName[nColumnCount] = _T("��һ���");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
//	strExportList[nColumnCount] = _T("SMS���ſ���");	strColumnName[nColumnCount] = _T("SMS���ſ���");strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("��ġ����");		strColumnName[nColumnCount] = _T("��ġ����");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("��û��");			strColumnName[nColumnCount] = _T("��û����");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("��û��");			strColumnName[nColumnCount] = _T("��û��");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("E-MAIL");			strColumnName[nColumnCount] = _T("���ڿ���");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("�ֹε�Ϲ�ȣ");	strColumnName[nColumnCount] = _T("�ֹε�Ϲ�ȣ");strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("�����ȣ");		strColumnName[nColumnCount] = _T("�����ȣ");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("�ּ�");			strColumnName[nColumnCount] = _T("�ּ�");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("��ȭ��ȣ");		strColumnName[nColumnCount] = _T("��ȭ��ȣ");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("�ڵ���");			strColumnName[nColumnCount] = _T("�ڵ���");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
//	strExportList[nColumnCount] = _T("���Ϲ߼���");		strColumnName[nColumnCount] = _T("���Ϲ߼���");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("ISBN");			strColumnName[nColumnCount] = _T("���Ϲ߼���");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;

	CESL_DataMgr * TmpDM;    
	TmpDM = FindDM(_T("DM_����_�ڷ����_��ġ�������"));
	TmpDM->StartFrame();

	TmpDM->InitDBFieldData();

	CString strCheck;
	INT nCount = m_CheckColumnArray.GetSize();
	// üũ�� �׸� ����Ʈ�� DB�� �÷����� ��ġ�ϸ� NULL�� Update���� �ʴ´�.
	for( INT i=0; i < nCount; i++)
	{
		strCheck = m_CheckColumnArray.GetAt(i);
		for(INT j=0; j < nColumnCount; j++)
		{
			if(strExportList[j] == strCheck)
				strNullYN[j] = _T("N");
		}
	}

	// ���Ͽ��� ��� ���ڵ忡 ���Ͽ� ���� �׸��� �ƴ� �÷����� NULL�� Update�ϵ��� �Ѵ�.
	INT ids;
	CString strFieldAlias, strFieldName, strType, strDataType, strData;
	INT nRowCount = m_DataReckeyArray.GetSize();
	if(nRowCount < 0 ) return -1;
	INT nDBColCnt;
	BOOL bDelete;
	CString strQuery;
	// TmpDM�� �÷��� �����ϱ� ���� Refresh�Ѵ�.
	strQuery.Format(_T("rec_key = %s"), m_DataReckeyArray.GetAt(0));
	TmpDM->RefreshDataManager(strQuery);
	nDBColCnt = TmpDM->GetColCount();
	for(i=0; i < nRowCount; i++)
	{
/*		for(INT j=0; j < nColumnCount; j++)
		{
			if(strNullYN[j] == _T("Y"))
			{
				nColIndex = TmpDM->FindColumn(strColumnName[j]);
				if( nColIndex == -1)
					return -1;
				ids = TmpDM->FindColumnFieldName(nColIndex, strFieldAlias, strFieldName, strType, strDataType, 0, strData);
				if(ids < 0) continue;
				
				strType.MakeUpper();	strDataType.MakeUpper();
				if(strDataType == _T("udf")) continue;
				if(strType == _T("str")) TmpDM->AddDBFieldData(strFieldName, _T("STRING"), _T(""));
				else if(strType == _T("int")) 
					TmpDM->AddDBFieldData(strFieldName, _T("NUMERIC"), _T(""));			
			}
		}
*/
		for(INT j=0; j < nDBColCnt; j++)
		{
			bDelete = TRUE;
			ids = TmpDM->FindColumnFieldName(j, strFieldAlias, strFieldName, strType, strDataType, 0, strData);
			if(ids < 0) continue;
			for(INT k=0; k < nColumnCount; k++)
			{
				if( (strFieldAlias == strColumnName[k]) && (strNullYN[k] == _T("N")) )
				{
					bDelete = FALSE;
				}
			}
			if(bDelete && (strFieldName != _T("REC_KEY")) && (strFieldName != _T("FIRST_WORK")) && (strFieldName != _T("LAST_WORK")))
			{
				//============================================
				//20080611 UPDATE BY PJW : �ҹ��ڴ빮�ڵ��� �����Ƿ� �ҹ��ڷ� �ٲٵ��� �����Ѵ�.
				//strType.MakeUpper();	strDataType.MakeUpper();
				strType.MakeLower();	strDataType.MakeLower();
				//============================================
				
				if(strDataType == _T("udf")) continue;
				if(strType == _T("str")) TmpDM->AddDBFieldData(strFieldName, _T("STRING"), _T(""));
				else if(strType == _T("int")) 
					TmpDM->AddDBFieldData(strFieldName, _T("NUMERIC"), _T(""));			
			}

		}
		TmpDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), m_pParent->GetWorkLogMsg(WORK_LOG));
		TmpDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParent->GetWorkLogMsg(WORK_LOG));

		ids = TmpDM->MakeUpdateFrame(_T("BO_FURNISH_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_DataReckeyArray.GetAt(i));
		if(ids < 0) return ids;
		TmpDM->InitDBFieldData();
	}

	ids = TmpDM->SendFrame();
	if(ids < 0) 
	{
		TmpDM->EndFrame();
		return ids;
	}
	ids = TmpDM->EndFrame();
	if(ids < 0) return ids;




	return 0;
}

INT CBO_ACQ_FileImportMgr::CheckImportInfo()
{
	CESL_DataMgr * TmpDM;    
	TmpDM =FindDM(_T("DM_����_�ڷ����_������"));

	INT ids;
	CString strExist, strQuery, strTmp;
	strExist.Empty();
	strQuery.Format(_T("SELECT COUNT(*) FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = %s AND SEQ_NO = %s AND WORK_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), 
		                    m_strAcqYear, m_strGroupInfo, m_strWorkCode);
	ids = TmpDM->GetOneValueQuery(strQuery, strExist);
	if( ids <0)
		return -1;

	if(strExist != _T("0"))
	{
		strTmp.Format(_T("�̹� ���� ������ȣ(%s)�� �����Ͽ� ������ ������ �� �����ϴ�.\r\nȮ�����ֽñ� �ٶ��ϴ�."), m_strGroupInfo);
		AfxMessageBox(strTmp);
		return -1;
	}

	return 0;
}

// 2005-01-17
INT CBO_ACQ_FileImportMgr::SendQueryToIDX_TMP()
{
	BOOL bError;
	CString strFilePath, strQuery;
	CStdioFile DataFile;

	strFilePath.Format(_T("%s\\IDX_BO_TBL.dat"), m_strImportFilePath);
	bError = DataFile.Open(strFilePath, CFile::modeRead | CFile::typeBinary);
	if(!bError){
		AfxMessageBox(_T("������ ������ �����ϴ�."));
		return -1;
	}
		TCHAR cUni;
		DataFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			DataFile.SeekToBegin();
		}
	
	CString strLine, strRecKey, strDate;
	INT nStart, nEnd, ids;

	CTime t = CTime::GetCurrentTime();
	strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	CESL_DataMgr * TmpDM;    
	TmpDM =FindDM(_T("DM_����_�ڷ����_��������"));
	if(TmpDM == NULL)
		return -1;
	TmpDM->StartFrame();

	while(DataFile.ReadString(strLine))
	{
		strLine.TrimLeft(); strLine.TrimRight();
		nStart = strLine.Find(_T("<start>"));
		if(nStart < 0) {
			DataFile.Close();
			return -1;
		}
		nEnd = strLine.Find(_T("<end>"));
		if(nEnd < 0) {
			DataFile.Close();
			return -1;
		}
		strRecKey = strLine.Mid((nStart+7), (nEnd-(nStart+7)));
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		strQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY,SPECIES_KEY,IDX_WORK_FLAG,INPUT_DATE) ")
//  						_T("VALUES (%s.NEXTVAL,%s,'I','%s');"), DEFAULT_SEQ_NAME, strRecKey, strDate);
// 		ids = TmpDM->AddFrame(strQuery);
		//=====================================================
 		
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("�˻��������� ����ȭ�� ���� ���� ���� ����"));
			return -1;
		}
	}

	ids = TmpDM->SendFrame();
	if(ids < 0) {
		TmpDM->EndFrame();
		return -1;
	}

	TmpDM->EndFrame();

	return 0;
}

			
	