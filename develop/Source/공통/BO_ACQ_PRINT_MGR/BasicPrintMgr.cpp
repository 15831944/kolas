// BasicPrintMgr.cpp: implementation of the CBasicPrintMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BasicPrintMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������
CBasicPrintMgr::CBasicPrintMgr(CESL_Mgr * pParentMgr)
{
	m_pParentMgr = pParentMgr;
	m_nPrintType = -1;

	m_MapPrintData.RemoveAll();	
	
	m_pPrintDM = NULL;
	m_pSIReportManager = new CSIReportManager(pParentMgr);
}
// �Ҹ���
CBasicPrintMgr::~CBasicPrintMgr()
{
	if( m_pSIReportManager )
	{
		delete m_pSIReportManager;
		m_pSIReportManager = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��������� ���ε� �����͸� ��� �����.
// [ �̴Ͻ��� ] - ����
//////////////////////////////////////////////////////////////////////
VOID CBasicPrintMgr::InitPrintData()
{
	CString strKey, strValue;
	POSITION pos = m_MapPrintData.GetStartPosition();
	while( pos != NULL )
	{
		m_MapPrintData.GetNextAssoc( pos, strKey, strValue);
		m_MapPrintData.RemoveKey(strKey);
	}

	m_MapPrintData.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - SIReportŬ������ �����ϰ� ��¿�DM�� �̿��Ͽ� ��¿� �������� �����.
// [ �̴Ͻ��� ]
// 1. ����ϱ� ���� ������Ҹ� üũ�Ѵ�.(SIReportŬ����, ��¿�DM, ��������)
// 2. ��������� ���εǾ��ִ� ���� SIReportŬ������ �����Ѵ�.
// 3. SIReportŬ������ ���������� �����Ѵ�.
// 4. DB�κ��� �ش� ���ϸ��� SIReport������ �����´�.
// 5. ��¿� DM�� SIReportŬ������ �����Ѵ�.
// 6. ��¿� �������� �����.
//
//////////////////////////////////////////////////////////////////////
INT CBasicPrintMgr::MakeXMLPageFile()
{
	//================================================================
	// ��������
	//================================================================
	INT ids;
	CString strKey, strValue;
	POSITION pos;

	//================================================================
	// 1. ����ϱ� ���� ������Ҹ� üũ�Ѵ�.(SIReportŬ����, ��¿�DM, ��������)
	//================================================================
	if( CheckValidation() == FALSE )	return -1;	
	
	//================================================================
	// 2. ��������� ���εǾ��ִ� ���� SIReportŬ������ �����Ѵ�.
	//================================================================
	pos = m_MapPrintData.GetStartPosition();
	while( pos != NULL )
	{
		m_MapPrintData.GetNextAssoc(pos, strKey, strValue);
		m_pSIReportManager->SetTempVariable(strKey, strValue);
	}
	
	//================================================================
	// 3. SIReportŬ������ ���������� �����Ѵ�.
	//================================================================
	m_pSIReportManager->SetCONNECTION_INFO(m_strConnectionInfo);

	//================================================================
	// 4. DB�κ��� �ش� ���ϸ��� SIReport������ �����´�.
	//================================================================
	ids = m_pSIReportManager->GetSIReportFileFromDB(m_strSIReportFileName);
	if( ids < 0 )
	{
		m_pParentMgr->ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 0;
	}
	
	//m_pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	
	//================================================================
	// 5. ��¿� DM�� SIReportŬ������ �����Ѵ�.
	//================================================================
	m_pSIReportManager->SetDataMgr(0, m_pPrintDM, 0);	

	//================================================================
	// 6. ��¿� �������� �����.
	//================================================================
	m_pSIReportManager->MAKEPAGE_DrawWorkForm(-1);

	return 0;
}

INT CBasicPrintMgr::Print2()
{
	return m_pSIReportManager->Print2();
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ӽ÷� ����� rpt���ϵ��� ��� �����.
// [ �̴Ͻ��� ] - ����
//////////////////////////////////////////////////////////////////////
INT CBasicPrintMgr::InitRptFile()
{
	INT i;
	CString strTmp;
	CFile fd;
	
	for( i=1; i<99999; i++ )
	{
		strTmp.Format(_T("rpt_tmp\\%s%04d.rpt"), m_pSIReportManager->m_strPageHeader, i);
		if( fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false )		break;

		fd.Close();
		DeleteFile(strTmp);
	}

	for( i=1; i<99999; i++ )
	{
		strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_pSIReportManager->m_strPageHeader, i);
		if( fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false )		break;

		fd.Close();
		DeleteFile(strTmp);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - SIReportŬ������ �����Ͽ� ����Ѵ�.
// [ �̴Ͻ��� ]
// 1. ����ϱ� ���� ������Ҹ� üũ�Ѵ�.(SIReportŬ����, ��¿�DM, ��������)
// 2. ��������� ���εǾ��ִ� ���� SIReportŬ������ �����Ѵ�.
// 3. SIReportŬ������ ���������� �����Ѵ�.
// 4. DB�κ��� �ش� ���ϸ��� SIReport������ �����´�.
// 5. ��¿� DM�� SIReportŬ������ �����Ѵ�.
// 6. ���Ÿ���� ���ӿ�������̶�� SIReportŬ������ �ش系���� �����ؾ��Ѵ�.
// 7. SIReportŬ������ ������ �������� ����Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CBasicPrintMgr::Print()
{
	//================================================================
	// ��������
	//================================================================
	INT ids;	
	CString strKey, strValue;
	POSITION pos;
	
	//================================================================
	// 1. ����ϱ� ���� ������Ҹ� üũ�Ѵ�.(SIReportŬ����, ��¿�DM, ��������)
	//================================================================
	if( CheckValidation() == FALSE )	return -1;
	
	//================================================================
	// 2. ��������� ���εǾ��ִ� ���� SIReportŬ������ �����Ѵ�.
	//================================================================
	pos = m_MapPrintData.GetStartPosition();
	while(pos != NULL)
	{
		m_MapPrintData.GetNextAssoc(pos, strKey, strValue);
		m_pSIReportManager->SetTempVariable(strKey, strValue);
	}
	
	//================================================================
	// 3. SIReportŬ������ ���������� �����Ѵ�.
	//================================================================
	m_pSIReportManager->SetCONNECTION_INFO(m_strConnectionInfo);

	//================================================================
	// 4. DB�κ��� �ش� ���ϸ��� SIReport������ �����´�.
	//================================================================
	ids = m_pSIReportManager->GetSIReportFileFromDB(m_strSIReportFileName);
	if (ids < 0)
	{
		m_pParentMgr->ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return -1;
	}
	
	//================================================================
	// 5. ��¿� DM�� SIReportŬ������ �����Ѵ�.
	//================================================================
	m_pSIReportManager->SetDataMgr(0, m_pPrintDM, 0);
	

	//================================================================
	// 6. ���Ÿ���� ���ӿ�������̶�� SIReportŬ������ �ش系���� �����ؾ��Ѵ�.
	//================================================================
	if( m_nPrintType == SERIES_PAPER )
	{
		m_pSIReportManager->m_nUserPaperSize = -9999;
		m_pSIReportManager->m_nOrientation = DMORIENT_PORTRAIT;
		m_pSIReportManager->m_nResolution = 1728; 
	}
	
	//================================================================
	// 7. SIReportŬ������ ������ �������� ����Ѵ�.
	//================================================================

	ids = m_pSIReportManager->Print();
	
	return ids;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ����ϱ� ���� ������Ҹ� üũ�Ѵ�.(SIReportŬ����, ��¿�DM, ��������)
// [ �̴Ͻ��� ]
// 1. SIReport���� Ŭ������ �������� �ʾҴٸ� ���������Ѵ�.
// 2. ��¿� DM�� �������� �ʾҴٸ� ���������Ѵ�.
// 3. ���������� �������� �ʾҴٸ� ���������Ѵ�.
// 4. ��¿� DM�� �����Ͱ� ���ٸ� ���������Ѵ�.
//
//////////////////////////////////////////////////////////////////////
BOOL CBasicPrintMgr::CheckValidation()
{
	//================================================================	
	// 1. SIReport���� Ŭ������ �������� �ʾҴٸ� ���������Ѵ�.
	//================================================================
	if( m_pSIReportManager == NULL )
	{
		m_pParentMgr->ESLAfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return FALSE;
	}

	//================================================================	
	// 2. ��¿� DM�� �������� �ʾҴٸ� ���������Ѵ�.
	//================================================================
	if( m_pPrintDM == NULL )
	{
		m_pParentMgr->ESLAfxMessageBox(_T("��¿� DM�� �������� �ʾҽ��ϴ�."));
		return FALSE;
	}

	//================================================================	
	// 3. ���������� �������� �ʾҴٸ� ���������Ѵ�.
	//================================================================
	if( m_strConnectionInfo.IsEmpty() == TRUE )
	{
		// ��¿� DM���� ���������� ���ٸ� ���������Ѵ�.
		m_strConnectionInfo = m_pPrintDM->CONNECTION_INFO;
		if( m_strConnectionInfo.IsEmpty() == TRUE ) 
		{
			m_pParentMgr->ESLAfxMessageBox(_T("DB���� ������ �������� �ʾҽ��ϴ�."));
			return FALSE;
		}
	}

	//================================================================	
	// 4. ��¿� DM�� �����Ͱ� ���ٸ� ���������Ѵ�.
	//================================================================
	if( m_pPrintDM->GetRowCount() == 0 )
	{
		m_pParentMgr->ESLAfxMessageBox(_T("����� �����Ͱ� �����ϴ�."));
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���ڷ� �� Ű�� ���� ��������� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
INT CBasicPrintMgr::AddPrintData(CString strKey, CString strValue)
{
	if( strKey.IsEmpty() == TRUE )		return -1;
	
	m_MapPrintData.SetAt(strKey, strValue);

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���ڷ� �� Ű���� �ش��ϴ� ���� ���ε� �������� ã�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
CString CBasicPrintMgr::GetPrintData(CString strKey)
{
	if( strKey.IsEmpty() == TRUE )		return _T("");
	
	CString strValue;
	m_MapPrintData.Lookup(strKey, strValue);

	return strValue;
}

VOID CBasicPrintMgr::DeleteTempPages(VOID)
{
	m_pSIReportManager->DeleteTempPageFiles();
}

//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
VOID CBasicPrintMgr::SetPrivacyPrintLogParm(BOOL bFlag, CString strKey, CString strID, CString strTitle, CString strManageCode, CString strLogMsg, INT nAccessLevel)
{
	m_pSIReportManager->SetPrivacyPrintLogParm(bFlag,strKey,strID,strTitle,strManageCode,strLogMsg,nAccessLevel);
}