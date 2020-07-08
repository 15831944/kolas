// ShareInfoCenterUp.cpp : implementation file
//

#include "stdafx.h"
#include "ShareInfoCenterUp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShareInfoCenterUp dialog


CShareInfoCenterUp::CShareInfoCenterUp(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pInfo = pParent->m_pInfo;
	INT ids = InitESL_Mgr(_T("SM_SHARE_INFO_CENTER_UP"));
	if(0 > ids)
	{
		CString sEMsg;
		sEMsg.Format(_T("Init ESLMgr Error [%d] "), ids);
		AfxMessageBox(sEMsg);
	}
	m_pCS_SQL = new CRS_SQL(this);
//	m_pCS_SQL->Set_RSShowMessage(FALSE);
}

CShareInfoCenterUp::~CShareInfoCenterUp()
{
	delete m_pCS_SQL;
}

VOID CShareInfoCenterUp::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShareInfoCenterUp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CShareInfoCenterUp, CDialog)
	//{{AFX_MSG_MAP(CShareInfoCenterUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShareInfoCenterUp message handlers

/// Upload�� �ڷ� �ѱ��
INT CShareInfoCenterUp::SetData(UPLOAD_SHARE_INFO UploadInfo,CString sLibCode)
{
EFS_BEGIN

	m_sLibCode = sLibCode;
	m_asUpdatedFields.RemoveAll();
	CLocCommonAPI::MakeArrayList(UploadInfo.UploadShareLoanInfo.sUpdatedFields, _T(","), m_asUpdatedFields);	

	return 0;

EFS_END
return -1;
}

/// ���ε�
INT CShareInfoCenterUp::UploadData()
{
EFS_BEGIN

	return 0;

EFS_END
return -1;
}


/// [1003] ó���Ϸ��� �ڷᰡ Temp Table�� �����ϴ��� �˻�
INT CShareInfoCenterUp::IsTempTable( CString sClientRecKey )
{
EFS_BEGIN
	
	CString sSql;
	sSql.Format( 
		_T("SELECT ")
		_T("COUNT(*) ")
		_T("FROM ")
		_T("CT_UPLOAD_TEMP_LOAN_INFO_TBL ")
		_T("WHERE ")
		_T("UPDATED_TABLE_REC_KEY=%s ") 
		, sClientRecKey
	);

	CESL_DataMgr* pDM = FindDM(_T("DM_CLIENT_USER_INFO"));
	if(NULL == pDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("IsTempTable"));
	}

	CString sValue;
	INT ids = pDM->GetOneValueQuery(sSql, sValue);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("IsTempTable")); 
	}
	if(0 == _ttoi(sValue))
	{
		return 1;
	}

	return 0;

EFS_END
return -1;
}

/// ������ �ڷḦ Temp Table�� ���
INT CShareInfoCenterUp::InsertTempTable(UPLOAD_SHARE_LOAN_INFO UploadInfo, CString sFailReasonCode)
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_CLIENT_USER_INFO"));
	if(NULL == pDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("InsertTempTable"));
	}

	pDM->StartFrame();
	/// Ÿ���� D��� temp table�� �����ϴ� �ڷ�� �����Ѵ�.
	if(UploadInfo.sType == _T("D"))
	{
		pDM->InitDBFieldData();
// 		pDM->MakeDeleteFrame(_T("CT_UPLOAD_TEMP_LOAN_INFO_TBL"), _T("UPDATED_TABLE_REC_KEY"), _T("NUMERIC"), UploadInfo.sUpdatedTableRecKey);
		CString strQuery;
		strQuery.Format(
			_T("DELETE FROM CT_UPLOAD_TEMP_LOAN_INFO_TBL WHERE UPDATED_TABLE_REC_KEY = '%s' AND COMMAND = %s;"),
			UploadInfo.sUpdatedTableRecKey, UploadInfo.sCommand
			);
		pDM->AddFrame(strQuery);
	}
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL"));
	pDM->AddDBFieldData(_T("TYPE"), _T("STRING"), UploadInfo.sType);
	pDM->AddDBFieldData(_T("COMMAND"), _T("NUMERIC"), UploadInfo.sCommand);
	pDM->AddDBFieldData(_T("MASTER_USER_INFO_CHANGE_YN"), _T("STRING"), UploadInfo.sMasterUserInfoChangeYN);
	pDM->AddDBFieldData(_T("UPDATED_TABLE_NAME"), _T("STRING"), UploadInfo.sUpdatedTableName);
	// 2009.07.03 UPDATE BY CJY : å�η� Ʈ������� STRING�̴�.
	if(_T("ILL_TRANS_TBL") != UploadInfo.sUpdatedTableName)
	{
		pDM->AddDBFieldData( _T("UPDATED_TABLE_REC_KEY") , _T("NUMERIC") , UploadInfo.sUpdatedTableRecKey );
	}
	else
	{
		pDM->AddDBFieldData( _T("UPDATED_TABLE_REC_KEY") , _T("STRING") , UploadInfo.sUpdatedTableRecKey );
	}
	pDM->AddDBFieldData(_T("UPDATED_FIELDS"), _T("STRING"), UploadInfo.sUpdatedFields);
	pDM->AddDBFieldData(_T("RECORD_DATE"), _T("NUMERIC"), _T("SYSDATE"));
	pDM->AddDBFieldData(_T("UPLOAD_TRY_CNT"), _T("NUMERIC"), _T("1"));
	pDM->AddDBFieldData(_T("UPLOAD_FAIL_REASON_CODE"), _T("STRING"), sFailReasonCode);
	// 2009.04.29 ADD BY CJY : ��������ȣ�� �ȵ��� ��찡 �־� �߰�
	UploadInfo.sLibCode.TrimLeft();	UploadInfo.sLibCode.TrimRight();
	if(TRUE == UploadInfo.sLibCode.IsEmpty())
	{
		if(NULL != m_pInfo)
		{
			UploadInfo.sLibCode = m_pInfo->LIB_CODE;
		}
	}
	// 2005.05.18 ADD BY PDJ
	// ������ ��ȣ�� ����
	pDM->AddDBFieldData(_T("LIB_CODE"), _T("STRING"), UploadInfo.sLibCode);
	CString sWorkLog = CESL_Mgr::GetWorkLogMsg(_T("���;��ε�"), __WFILE__, __LINE__);
	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), sWorkLog, TRUE);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), sWorkLog, TRUE);
	
	ids = pDM->MakeInsertFrame(_T("CT_UPLOAD_TEMP_LOAN_INFO_TBL"));
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("InsertTempTable"));
	}
	ids = pDM->SendFrame();
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("InsertTempTable"));
	}
	pDM->EndFrame();
	
	return 0;

EFS_END
return -1;
}

INT CShareInfoCenterUp::MakeCenterRecKey( CString &sRecKey )
{

	EFS_BEGIN

	INT ids;

	CString sSql;
	sSql.Format( _T("SELECT ESL_SEQ.NEXTVAL FROM DUAL") );

//	ids = m_pCS_SQL->LOAN_GetOneValue( sSql , sRecKey , FALSE );
	if( ids != 0 ) return 10006;

	return 0;

	EFS_END

	return -1;
}

bool CShareInfoCenterUp::IsInUpdatedFields( CStringArray &asFieldArray , CString sFieldName )
{
	EFS_BEGIN

	for( INT i = 0 ; i < asFieldArray.GetSize() ; i++ )
	{
		if( asFieldArray.GetAt(i).Compare( sFieldName ) == 0 )
			return TRUE;
	}

	return FALSE;

	EFS_END

	return FALSE;
}


/// ���� ���������� Local�������� ������ ���� REC_KEY ��������
INT CShareInfoCenterUp::InitLibKey()
{
	EFS_BEGIN

	INT ids;

	CString sSql;
	sSql.Format( _T("SELECT REC_KEY FROM CT_LOCAL_LIBRARY_TBL WHERE CODE='%s'") , m_sLibCode );

//	ids = m_pCS_SQL->LOAN_GetOneValue( sSql , m_sLibKey , FALSE );
	if( ids != 0 ) return 10006;

	return 0;

	EFS_END

	return -1;
}


VOID CShareInfoCenterUp::RecordLogTunning(CString sMsg)
{
	return;

	EFS_BEGIN
/*
	CTime t = CTime::GetCurrentTime();

	CString filename, msg;
	filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d.txt"), t.GetYear(), t.GetMonth(), t.GetDay());
	msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][%s]"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), sMsg);
	
	FILE* fp;
	
	fp = _tfopen (filename, _T("a+"));
	if (fp == NULL)
	{
		return ;
	}
	_ftprintf (fp, _T("%s\n"), msg);
	
	fclose(fp);
*/	
	
	EFS_END

}
