// Kolas2up_MobileApi1.cpp: implementation of the CKolas2up_MobileApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kolas2up_MobileApi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "query_field.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolas2up_MobileApi::CKolas2up_MobileApi()
{
	m_pDM = NULL;
	MODE_SELECT = MOBILE_MODE_CLIENT;
	MODE_NONSELECT = MOBILE_MODE_CLIENT;

	m_CfgMainDB.ReadCfgFile(_T("..\\CFG\\MOBILE\\MainDB.cfg"));
	m_CfgMobileDB.ReadCfgFile(_T(""));
}

CKolas2up_MobileApi::~CKolas2up_MobileApi()
{
	FreeLogList();
}

INT CKolas2up_MobileApi::SetMode(INT nMode_Select, INT nMode_NonSelect)
{
	MODE_SELECT = nMode_Select;
	MODE_NONSELECT = nMode_NonSelect;
	return 0;
}

INT CKolas2up_MobileApi::SetMode(INT nMode)
{
	// �ڷ����             �ڷ�˻�            ��
	// client               client              0
	// off                  off                 10000
	// off                  on                  20000
	// on                   off                 30000
	// on                   on                  40000

	if (nMode == 0) {
		MODE_NONSELECT = MOBILE_MODE_CLIENT;
		MODE_SELECT = MOBILE_MODE_CLIENT;
	}
	if (nMode == 10000) {
		MODE_NONSELECT = MOBILE_MODE_OFFLINE;
		MODE_SELECT = MOBILE_MODE_OFFLINE;
	}
	if (nMode == 20000) {
		MODE_NONSELECT = MOBILE_MODE_OFFLINE;
		MODE_SELECT = MOBILE_MODE_ONLINE;
	}
	if (nMode == 30000) {
		MODE_NONSELECT = MOBILE_MODE_ONLINE;
		MODE_SELECT = MOBILE_MODE_OFFLINE;
	}
	if (nMode == 40000) {
		MODE_NONSELECT = MOBILE_MODE_ONLINE;
		MODE_SELECT = MOBILE_MODE_ONLINE;
	}
	return 0;
}

INT CKolas2up_MobileApi::FreeLogList(VOID)
{
EFS_BEGIN
	INT nData = LOGLIST.GetCount();
	if (nData == 0) return 0;
	CKolas2up_MobileLog * pData;
	for (INT i = 0; i < nData; i++) 
	{
		pData = NULL;
		pData = (CKolas2up_MobileLog*)LOGLIST.RemoveHead();
		if (pData) delete pData;
	}
	LOGLIST.RemoveAll();
	return 0;
EFS_END
return -1;
}

INT CKolas2up_MobileApi::StartLog(VOID)
{
EFS_BEGIN
	FreeLogList();
	return 0;
EFS_END
return -1;
}

INT CKolas2up_MobileApi::EndLog(BOOL LOGMODE/*=TRUE*/)
{
EFS_BEGIN
	INT ids;
	if (MODE_NONSELECT == MOBILE_MODE_CLIENT) return 0;
	if (m_pDM == NULL) return -1;
	if (LOGMODE == FALSE) {
		FreeLogList();
		return 0;
	}
	SetDM_Mode(MOBILE_MODE_OFFLINE);
	
	// �α� ���̺� �α׸� ������	
	INT nData = LOGLIST.GetCount();
	if (nData == 0) return 0;
	CKolas2up_MobileLog * pData;
	POSITION pos = LOGLIST.GetHeadPosition();
	CString strQuery;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	INT nRows, j;
	CString strRecKey, strQUERY_TYPE;
	BOOL bContinueFlag;

	CTime t = CTime::GetCurrentTime();
	CString filename, msg;
	filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d.txt"), t.GetYear(), t.GetMonth(), t.GetDay());

	for (INT i = 0; i < nData; i++) 
	{
		pData = NULL;
		pData = (CKolas2up_MobileLog*)LOGLIST.GetNext(pos);
		if (pData == NULL) continue;

		ids = m_pDM->MakeRecKey(pData->strREC_KEY, m_CfgMobileDB.m_sSequence);
		msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][%s][%05d] %s;"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), _T("CKolas2up_MobileApi::EndLog"), ids, _T("MYSQL MAKE REC KEY"));
		FILE* fp = _tfopen (filename, _T("a+b"));
		
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE �����۾�
		// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			/*UNCHANGE*/fputc(0xFF, fp);
			/*UNCHANGE*/fputc(0xFE, fp);
		}
		// ------------------------------------------------------------------------------
		
		if (fp) {
			_ftprintf (fp, _T("%s\r\n"), msg);
			fclose(fp);
		}
		if (ids < 0) {
			Sleep(100);
			ids = m_pDM->MakeRecKey(pData->strREC_KEY, m_CfgMobileDB.m_sSequence);
			msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][%s][%05d] %s;"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), _T("*2*CKolas2up_MobileApi::EndLog"), ids, _T("MYSQL MAKE REC KEY"));
			FILE* fp = _tfopen (filename, _T("a+b"));
			
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
			// KOLAS 2 UP UNICODE �����۾�
			// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
			fseek(fp, 0, SEEK_END);		
			if( 0 == ftell(fp) )
			{
				/*UNCHANGE*/fputc(0xFF, fp);
				/*UNCHANGE*/fputc(0xFE, fp);
			}
			// ------------------------------------------------------------------------------
		
			if (fp) {
				_ftprintf (fp, _T("%s\r\n"), msg);
				fclose(fp);
			}
		}

		pData->SetLogDate();
		bContinueFlag = FALSE;

		if (pData->cQUERY_TYPE == 'I') {
			// I �ϰ�� : üũ ����

		}
		if (pData->cQUERY_TYPE == 'U') {
			// U �ϰ�� : �̹� I�� �ִ°�� -> U ����
			//            �̹� D�� �ִ°�� -> �̷����� ����� �ϰ���?
			//            �̹� U�� �ִ°�� -> U ����

			strQuery.Format(_T("SELECT REC_KEY, QUERY_TYPE FROM QUERY_LOG_TBL WHERE TABLE_NAME='%s' AND DATA_REC_KEY=%s"), pData->strTABLE_NAME, pData->strREC_KEY_FIELDVALUE);
			dm.RestructDataManager(strQuery);
			nRows = dm.GetRowCount();
			for (j = 0; j < nRows; j++) {
				strRecKey = dm.GetCellData(j, 0);	
				strQUERY_TYPE = dm.GetCellData(j, 1);

				if (strQUERY_TYPE == _T("I")) {
					bContinueFlag = TRUE;
					break;
				}
				if (strQUERY_TYPE == _T("U")) {
					bContinueFlag = TRUE;
					break;
				}
			}
		}
		if (pData->cQUERY_TYPE == 'D') {
			// D �ϰ�� : �̹� I�� �ִ°�� -> I �����ϰ� D ����
			//            �̹� U�� �ִ°�� -> U �����ϰ� D ����
			//            �̹� D�� �ִ°�� -> D ����
			strQuery.Format(_T("SELECT REC_KEY, QUERY_TYPE FROM QUERY_LOG_TBL WHERE TABLE_NAME='%s' AND DATA_REC_KEY=%s"), pData->strTABLE_NAME, pData->strREC_KEY_FIELDVALUE);
			dm.RestructDataManager(strQuery);
			nRows = dm.GetRowCount();
			for (j = 0; j < nRows; j++) {
				strRecKey = dm.GetCellData(j, 0);	
				strQUERY_TYPE = dm.GetCellData(j, 1);

				if (strQUERY_TYPE == _T("I")) {
					strQuery.Format(_T("DELETE FROM QUERY_LOG_TBL WHERE REC_KEY=%s;"), strRecKey);
					m_pDM->ExecuteQuery(strQuery, TRUE);
					bContinueFlag = TRUE;
					break;
				}
				if (strQUERY_TYPE == _T("U")) {
					strQuery.Format(_T("DELETE FROM QUERY_LOG_TBL WHERE REC_KEY=%s;"), strRecKey);
					m_pDM->ExecuteQuery(strQuery, TRUE);
					bContinueFlag = FALSE;
					break;
				}
				if (strQUERY_TYPE == _T("D")) {
					bContinueFlag = TRUE;
					break;
				}
			}
		}

		if (bContinueFlag == TRUE) continue;
		CString strFIELDS;
		strFIELDS = pData->strFIELDS;
		strFIELDS.Replace(_T("'"), _T("''"));
		strQuery.Format(_T("INSERT INTO QUERY_LOG_TBL ")
						_T("(REC_KEY, QUERY_TYPE, TABLE_NAME, DATA_REC_KEY, LOGDATE, QUERY)")
						_T(" VALUES ")
						_T("(%s, '%c', '%s', %s, %s, '%s');"),
						pData->strREC_KEY,
						pData->cQUERY_TYPE,
						pData->strTABLE_NAME,
						pData->strREC_KEY_FIELDVALUE,
						pData->strLOGDATE,
						strFIELDS
						);
		ids = m_pDM->ExecuteQuery(strQuery, TRUE);
		
		if( ids < 0 )
		{
			if( pData->strREC_KEY.IsEmpty() )
			{
				strQuery.Format(_T("INSERT INTO QUERY_LOG_TBL ")
								_T("(REC_KEY, QUERY_TYPE, TABLE_NAME, DATA_REC_KEY, LOGDATE, QUERY)")
								_T(" VALUES ")
								_T("(%s, '%c', '%s', %s, %s, '%s');"),
								pData->strREC_KEY,
								pData->cQUERY_TYPE,
								pData->strTABLE_NAME,
								pData->strREC_KEY_FIELDVALUE,
								pData->strLOGDATE,
								strFIELDS
								);
				ids = m_pDM->MakeRecKey(pData->strREC_KEY, m_CfgMobileDB.m_sSequence);
				if( ids < 0 )
				{	
					m_pDM->QueryLog( _T("MySQL - Make RecKey ����") );
				}

				ids = m_pDM->ExecuteQuery(strQuery, TRUE);
				if( ids < 0 )
				{
					CString sEMsg;
					sEMsg.Format( _T("[�����ѷα��ۼ�1]%s") , strQuery );
					m_pDM->QueryLog(sEMsg.GetBuffer(0));
				}
			}
			else
			{
				CString sEMsg;
				sEMsg.Format( _T("[�����ѷα��ۼ�2]%s") , strQuery );
				m_pDM->QueryLog(sEMsg.GetBuffer(0));
			}
		}
	}
	// �α� ����Ÿ�� ������.
	FreeLogList();
	return 0;
EFS_END
return -1;
}

CKolas2up_MobileLog * CKolas2up_MobileApi::AddLog(VOID)
{
EFS_BEGIN
	CKolas2up_MobileLog * pData = new CKolas2up_MobileLog;
	if (pData == NULL) return NULL;
	LOGLIST.AddTail((CObject*)pData);
	return pData;
EFS_END
return NULL;	
}

INT CKolas2up_MobileApi::SetDM(CESL_DataMgr *pDM)
{
EFS_BEGIN
	if (pDM == NULL) return -1;
	m_pDM = pDM;
	return 0;
EFS_END
return -1;
}

INT CKolas2up_MobileApi::MakeInsertFrame(CString strTblName, CString strRECKEYFieldName, CString strRECKEYFieldType, CString strRECKEYFieldData)
{
	INT ids;
	if (strTblName.GetLength() == 0) return -1;		/// Table name �� ����
	if (MODE_NONSELECT != MOBILE_MODE_CLIENT) 
	{
		// (START) -�α� �����
		CKolas2up_MobileLog *pLog = AddLog();
		pLog->cQUERY_TYPE = 'I';
		pLog->strTABLE_NAME = strTblName;
		pLog->strREC_KEY_FIELDVALUE = strRECKEYFieldData;
		pLog->cEXE_FLAG = 'R';	// �����ؾ��� ����
		// ( END ) -�α� �����
	}

	m_pDM->AddDBFieldData(strRECKEYFieldName, strRECKEYFieldType, strRECKEYFieldData);
	ids = m_pDM->MakeInsertFrame(strTblName);
	return ids;
}

INT CKolas2up_MobileApi::MakeUpdateFrame(CString strTblName, CString strCondFieldName, CString strCondFieldType, CString strCondFieldData, INT nRowIdx/*=-1*/)
{
	INT ids;
	if (strTblName.GetLength() == 0) return -1;		/// Table name �� ����
	if (MODE_NONSELECT != MOBILE_MODE_CLIENT) 
	{
		// (START) -�α� �����
		CKolas2up_MobileLog *pLog = AddLog();
		pLog->cQUERY_TYPE = 'U';
		pLog->strTABLE_NAME = strTblName;
		pLog->strREC_KEY_FIELDVALUE = strCondFieldData;
		pLog->cEXE_FLAG = 'R';	// �����ؾ��� ����
		// ( END ) -�α� �����
	}

	ids = m_pDM->MakeUpdateFrame(strTblName, strCondFieldName, strCondFieldType, strCondFieldData, nRowIdx);
	return ids;
}

INT CKolas2up_MobileApi::MakeDeleteFrame(CString strTblName, CString strCondFieldName, CString strCondFieldType, CString strCondFieldData)
{
	INT ids;
	if (strTblName.GetLength() == 0) return -1;		/// Table name �� ����
	if (MODE_NONSELECT != MOBILE_MODE_CLIENT) 
	{
		// (START) -�α� �����
		CKolas2up_MobileLog *pLog = AddLog();
		pLog->cQUERY_TYPE = 'D';
		pLog->strTABLE_NAME = strTblName;
		pLog->strREC_KEY_FIELDVALUE = strCondFieldData;
		pLog->cEXE_FLAG = 'R';	// �����ؾ��� ����
		// ( END ) -�α� �����
	}

	ids = m_pDM->MakeDeleteFrame(strTblName, strCondFieldName, strCondFieldType, strCondFieldData);
	return 0;
}

INT CKolas2up_MobileApi::RefreshDataManager(CString strOption, CProgressCtrl *pBar/*=NULL*/)
{
	INT ids;
	if (m_pDM == NULL) return -1;			// ������ DM �� ����
	if (MODE_SELECT != MOBILE_MODE_CLIENT) SetDM_SelectMode();

	ids = m_pDM->RefreshDataManager(strOption, pBar);
	return ids;
}

INT CKolas2up_MobileApi::RestructDataManager(CString sQuery, CProgressCtrl *pBar/*=NULL*/)
{
	INT ids;
	if (m_pDM == NULL) return -1;			// ������ DM �� ����
	if (MODE_SELECT != MOBILE_MODE_CLIENT) SetDM_SelectMode();

	ids = m_pDM->RestructDataManager(sQuery, pBar);
	return ids;
}

INT CKolas2up_MobileApi::GetOneValueQuery(CString sQuery, CString &result)
{
	INT ids;
	if (m_pDM == NULL) return -1;			// ������ DM �� ����
	if (MODE_SELECT != MOBILE_MODE_CLIENT) SetDM_SelectMode();

	ids = m_pDM->GetOneValueQuery(sQuery, result);
	return ids;
}

INT CKolas2up_MobileApi::GetValuesQuery(CString sQuery, CString result[], INT nresult)
{
	INT ids;
	if (m_pDM == NULL) return -1;			// ������ DM �� ����
	if (MODE_SELECT != MOBILE_MODE_CLIENT) SetDM_SelectMode();

	ids = m_pDM->GetValuesQuery(sQuery, result, nresult);
	return ids;
}

INT CKolas2up_MobileApi::SendFrame(VOID)
{
	// OnLine ����ΰ�� : MainDB�� MobileDB�� ��� ������ �����Ѵ�.
	// OffLine ����� ��� : MobileDB���� ������ ������.
	// Client ����ΰ�� : �� ������ �����Ѵ�.

	INT ids;
	INT ids_online, ids_offline;
	if (m_pDM == NULL) return -1;

	if (MODE_NONSELECT == MOBILE_MODE_CLIENT) {
		ids = m_pDM->SendFrame();
		return ids;
	}

	if (MODE_NONSELECT == MOBILE_MODE_ONLINE) {
		// Main DB ������ ���� ������ ������.
		SetDM_Mode(MOBILE_MODE_ONLINE);
		ids_online = m_pDM->SendFrame();
		if (ids_online < 0) return ids_online;
		
		// Mobile DB������ ������ ������.
		SetDM_Mode(MOBILE_MODE_OFFLINE);
		ids_offline = m_pDM->SendFrame();
		if (ids_offline < 0) return ids_offline;

		return 0;
	}

	if (MODE_SELECT == MOBILE_MODE_OFFLINE) {
		// Mobile DB������ ������ ������.
		SetDM_Mode(MOBILE_MODE_OFFLINE);
		ids_offline = m_pDM->SendFrame();
		if (ids_offline < 0) return ids_offline;

		return 0;
	}
	return 0;
}

INT CKolas2up_MobileApi::SetDM_SelectMode(VOID)
{
	if (m_pDM == NULL) return -1;			// ������ DM �� ����
	if (MODE_SELECT == MOBILE_MODE_ONLINE) {	
		// MainDB���� �˻��ؾ� �ϴ� ���
		m_pDM->SetCONNECTION_INFO(m_CfgMainDB.m_sConnection);
		m_pDM->SetDB(m_CfgMainDB.m_sDBKIND);
	} else {
		// MobileDB���� �˻��ؾ� �ϴ� ���
		m_pDM->SetCONNECTION_INFO(m_CfgMobileDB.m_sConnection);
		m_pDM->SetDB(m_CfgMobileDB.m_sDBKIND);
	}
	return 0;
}

INT CKolas2up_MobileApi::SetDM_NonSelectMode(VOID)
{
	if (m_pDM == NULL) return -1;			// ������ DM �� ����
	if (MODE_NONSELECT == MOBILE_MODE_ONLINE) {	
		// MainDB���� �˻��ؾ� �ϴ� ���
		m_pDM->SetCONNECTION_INFO(m_CfgMainDB.m_sConnection);
		m_pDM->SetDB(m_CfgMainDB.m_sDBKIND);
	}
	if (MODE_NONSELECT == MOBILE_MODE_OFFLINE) {	
		// MobileDB���� �˻��ؾ� �ϴ� ���
		m_pDM->SetCONNECTION_INFO(m_CfgMobileDB.m_sConnection);
		m_pDM->SetDB(m_CfgMobileDB.m_sDBKIND);
	}
	return 0;
}

INT CKolas2up_MobileApi::EndFrame(VOID)
{
	if (m_pDM == NULL) return -1;
	INT ids = m_pDM->EndFrame();
	return ids;
}

INT CKolas2up_MobileApi::SetDM_Mode(INT nMode)
{
	if (m_pDM == NULL) return -1;			// ������ DM �� ����
	if (nMode == MOBILE_MODE_ONLINE) {	
		// MainDB�� �����ؾ� �ϴ� ���
		m_pDM->SetCONNECTION_INFO(m_CfgMainDB.m_sConnection);
		m_pDM->SetDB(m_CfgMainDB.m_sDBKIND);
	}
	if (MODE_NONSELECT == MOBILE_MODE_OFFLINE) {	
		// MobileDB�� �����ؾ� �ϴ� ���
		m_pDM->SetCONNECTION_INFO(m_CfgMobileDB.m_sConnection);
		m_pDM->SetDB(m_CfgMobileDB.m_sDBKIND);
	}
	return 0;
}

VOID CKolas2up_MobileApi::InitDBFieldData(VOID)
{
	if (m_pDM == NULL) m_pDM->InitDBFieldData();
	return;
}

INT CKolas2up_MobileApi::AddDBFieldData(CString field, CString type, CString data, INT replace_flag/*=-1*/)
{
	if (m_pDM == NULL) return -1;
	if (MODE_NONSELECT == MOBILE_MODE_OFFLINE) {
		data.Replace(_T("'"), _T("''"));
		data.Replace(_T("\\"), _T("\\\\"));
	}
	return (m_pDM->AddDBFieldData(field, type, data, replace_flag));
}

INT CKolas2up_MobileApi::StartFrame(VOID)
{
	if (m_pDM == NULL) return -1;

	return (m_pDM->StartFrame());
}

INT CKolas2up_MobileApi::MakeRecKey(CString &result, CString table_name/*=_T("")*/, CString rec_key_field/*=_T("")*/)
{
	INT ids = -1;
	if (MODE_NONSELECT == MOBILE_MODE_CLIENT) {
		ids = m_pDM->MakeRecKey(result);
	}
	if (MODE_NONSELECT == MOBILE_MODE_ONLINE) {
		// MainDB�� �����ؾ� �ϴ� ���
		m_pDM->SetCONNECTION_INFO(m_CfgMainDB.m_sConnection);
		m_pDM->SetDB(m_CfgMainDB.m_sDBKIND);

		ids = m_pDM->MakeRecKey(result, m_CfgMainDB.m_sSequence);
	}
	if (MODE_NONSELECT == MOBILE_MODE_OFFLINE) {
		// MobileDB�� �����ؾ� �ϴ� ���
		m_pDM->SetCONNECTION_INFO(m_CfgMobileDB.m_sConnection);
		m_pDM->SetDB(m_CfgMobileDB.m_sDBKIND);

		ids = m_pDM->MakeRecKey(result, m_CfgMobileDB.m_sSequence);
	}
	return ids;
}

INT CKolas2up_MobileApi::GetLogDate(TCHAR * szBuf)
{
	if (szBuf == NULL) return -1;
	CTime t = CTime::GetCurrentTime();
	_stprintf(szBuf, _T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	return 0;
}

INT CKolas2up_MobileApi::AddFrame(CString strQuery)
{
	INT ids;
	if (MODE_NONSELECT != MOBILE_MODE_CLIENT) 
	{
		// (START) -�α� �����
		CKolas2up_MobileLog *pLog = AddLog();
		pLog->cQUERY_TYPE = 'Q';
		pLog->strTABLE_NAME = _T("");
		pLog->strFIELDS = strQuery;
		pLog->cEXE_FLAG = 'R';	// �����ؾ��� ����
		// ( END ) -�α� �����
	}
	ids = m_pDM->AddFrame(strQuery);
	return ids;
}	