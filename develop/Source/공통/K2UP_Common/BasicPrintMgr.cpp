// BasicPrintMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BasicPrintMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasicPrintMgr dialog


CBasicPrintMgr::CBasicPrintMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBasicPrintMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBasicPrintMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_MapPrintData.RemoveAll();
	m_pSIReportManager = new CSIReportManager(pParent);
}

CBasicPrintMgr::~CBasicPrintMgr()
{
	if(m_pSIReportManager)
	{
		delete m_pSIReportManager;
		m_pSIReportManager = NULL;
	}
}

VOID CBasicPrintMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBasicPrintMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBasicPrintMgr, CDialog)
	//{{AFX_MSG_MAP(CBasicPrintMgr)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicPrintMgr message handlers

VOID CBasicPrintMgr::SetPrintDMAlias(CString DMAlias)
{
	m_strPrintDMAlias = DMAlias;
}

VOID CBasicPrintMgr::SetSIReportFilename(CString filenaem)
{
	m_strSIReportFileName = filenaem;
}

INT CBasicPrintMgr::MakePrintDM()
{
	//����� DM�� �����Ѵ�.
	CESL_DataMgr * pDM = FindDM(m_strPrintDMAlias);

	return 0;
}

VOID CBasicPrintMgr::SetPrintType(INT type)
{
	m_PrintType = type;
}

VOID CBasicPrintMgr::SetCategoryTransferPrintData(CSIReportManager *pSIReportManager)
{
	CString msg, tmpstr, tmpstr1;
	if(m_strMainCMAlias.GetLength() == 0) return;
	CESL_ControlMgr * pCM = FindCM(m_strMainCMAlias);
	pCM->GetControlMgrData(_T("���Ա���"), tmpstr);
	if(tmpstr == _T("1")) tmpstr = _T("����");
	else if(tmpstr == _T("2")) tmpstr = _T("����");
	msg.Format(_T("���Ա��� : %s"), tmpstr);
	pSIReportManager->SetTableCellText(_T("���Ա���"), 0, 0, msg, ALIGN_VCENTER, ALIGN_HCENTER);
	
	pCM->GetControlMgrData(_T("�����ΰ���1"), tmpstr);
	pCM->GetControlMgrData(_T("�����ΰ���2"), tmpstr1);
	if(tmpstr1.GetLength() == 0 && tmpstr.GetLength() == 0) msg.Format(_T("�����ΰ��� : ��ü�ڷ�"));
	else if(tmpstr1.GetLength() == 0 && tmpstr.GetLength() != 0) msg.Format(_T("�����ΰ��� : %s"), tmpstr);
	else msg.Format(_T("�����ΰ��� : %s - %s"), tmpstr, tmpstr1);
	pSIReportManager->SetTableCellText(_T("�����ΰ���"), 0, 0, msg, ALIGN_VCENTER, ALIGN_HCENTER);
}

VOID CBasicPrintMgr::SetMainCMAlias(CString CMAlias)
{
	m_strMainCMAlias = CMAlias;
}

VOID CBasicPrintMgr::Print()
{
	// TODO: Add your control notification handler code here
	CESL_DataMgr *pPrintDM = FindDM(m_strPrintDMAlias);
	if(!pPrintDM)
	{
		ESLAfxMessageBox(_T("��¿� Data Object�� ã�� �� �����ϴ�."));
		return ;
	}
	
	INT ids;
	
	CString strKey, strValue;
	POSITION pos = m_MapPrintData.GetStartPosition();
	while(pos != NULL)
	{
		m_MapPrintData.GetNextAssoc(pos, strKey, strValue);
		m_pSIReportManager->SetTempVariable(strKey, strValue);
	}
	
	m_pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = m_pSIReportManager->GetSIReportFileFromDB(m_strSIReportFileName);
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return ;
	}
	
	m_pSIReportManager->SetDataMgr(0, pPrintDM, 0);

	if(m_PrintType == SERIES_PAPER)
	{
		m_pSIReportManager->m_nUserPaperSize = -9999;
		m_pSIReportManager->m_nOrientation = DMORIENT_PORTRAIT;
		m_pSIReportManager->m_nResolution = 1728; 
	}
	
	ids = m_pSIReportManager->Print();
	
	return ;
}

INT CBasicPrintMgr::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager * pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;

	if(m_MapPrintData.IsEmpty()) return 0;
	CString strKey, strValue;
	POSITION pos = m_MapPrintData.GetStartPosition();
	while(pos != NULL)
	{
		m_MapPrintData.GetNextAssoc(pos, strKey, strValue);
		pSIReportManager->SetTempVariable(strKey, strValue);
	}

	return 0;
}

VOID CBasicPrintMgr::SetLightCodeStatistics(CSIReportManager *pSIReportManager)
{
	CString msg, tmpstr, tmpstr1;
	if(m_strMainCMAlias.GetLength() == 0) return;
	CESL_ControlMgr * pCM = FindCM(m_strMainCMAlias);

	pCM->GetControlMgrData(_T("��¥1"), tmpstr);
	pCM->GetControlMgrData(_T("��¥2"), tmpstr1);
	if(tmpstr.IsEmpty() && !tmpstr1.IsEmpty())
		msg.Format(_T("�Ⱓ : %s ����"), tmpstr1);
	else if(!tmpstr.IsEmpty() && tmpstr1.IsEmpty())
		msg.Format(_T("�Ⱓ : %s ����"), tmpstr);
	else if(!tmpstr.IsEmpty() && !tmpstr1.IsEmpty())
		msg.Format(_T("�Ⱓ : %s ���� %s ����"), tmpstr, tmpstr1);
	pSIReportManager->SetTableCellText(_T("�Ⱓ"), 0, 0, msg, ALIGN_VCENTER, ALIGN_HCENTER);
	
	pCM->GetControlMgrData(_T("�з�ǥ����"), tmpstr);
	if(tmpstr == _T("1"))
		msg = _T("�з�ǥ���� : KDC");
	else if(tmpstr == _T("2"))
		msg = _T("�з�ǥ���� : DDC");
	pSIReportManager->SetTableCellText(_T("�з�ǥ����"), 0, 0, msg, ALIGN_VCENTER, ALIGN_HCENTER);	

	pCM->GetControlMgrData(_T("�ڷᱸ��"), tmpstr);
	if(tmpstr == _T("0"))
		msg = _T("���� : ��");
	else if(tmpstr == _T("1"))
		msg = _T("���� : å");
	pSIReportManager->SetTableCellText(_T("����"), 0, 0, msg, ALIGN_VCENTER, ALIGN_HCENTER);	
}

VOID CBasicPrintMgr::SetOrderMailPrintData(CSIReportManager *pSIReportManager)
{
	CString msg, tmpstr, tmpstr1;
	if(m_strMainCMAlias.GetLength() == 0) return;
	CESL_ControlMgr * pCM = FindCM(m_strMainCMAlias);

	pCM->GetControlMgrData(_T("�ֹ���1"), tmpstr);
	pCM->GetControlMgrData(_T("�ֹ���2"), tmpstr1);
	if(tmpstr1.GetLength() == 0 && tmpstr.GetLength() == 0) msg.Format(_T("�ֹ��� : ��ü�ڷ�"));
	else if(tmpstr1.GetLength() == 0 && tmpstr.GetLength() != 0) msg.Format(_T("�ֹ��� : %s"), tmpstr);
	else msg.Format(_T("�ֹ��� : %s - %s"), tmpstr, tmpstr1);
	pSIReportManager->SetTableCellText(_T("�ֹ���"), 0, 0, msg, ALIGN_VCENTER, ALIGN_LEFT);
}

VOID CBasicPrintMgr::SetRequestMailSendData(CSIReportManager *pSIReportManager)
{
	CString msg, tmpstr, tmpstr1;
	if(m_strMainCMAlias.GetLength() == 0) return;
	CESL_ControlMgr * pCM = FindCM(m_strMainCMAlias);

	pCM->GetControlMgrData(_T("���Ϲ߼���1"), tmpstr);
	pCM->GetControlMgrData(_T("���Ϲ߼���2"), tmpstr1);
	if(tmpstr1.GetLength() == 0 && tmpstr.GetLength() == 0) msg.Format(_T("���Ϲ߼��� : ��ü�ڷ�"));
	else if(tmpstr1.GetLength() == 0 && tmpstr.GetLength() != 0) msg.Format(_T("���Ϲ߼��� : %s"), tmpstr);
	else msg.Format(_T("���Ϲ߼��� : %s - %s"), tmpstr, tmpstr1);
	pSIReportManager->SetTableCellText(_T("���Ϲ߼���"), 0, 0, msg, ALIGN_VCENTER, ALIGN_LEFT);
}

VOID CBasicPrintMgr::SetLightPriceSumFromGrid(CSIReportManager * pSIReportManager)
{
	CString msg, tmpstr, tmpstr1;
	if(m_strMainCMAlias.GetLength() == 0) return;
	CESL_ControlMgr * pCM = FindCM(m_strMainCMAlias);

	pCM->GetControlMgrData(_T("��å��"), tmpstr);
	pCM->GetControlMgrData(_T("��������"), tmpstr1);
	pSIReportManager->SetTableCellText(_T("��å��"), 0, 0, tmpstr, ALIGN_VCENTER, ALIGN_HCENTER);
	pSIReportManager->SetTableCellText(_T("�Ѱ���"), 0, 0, tmpstr1, ALIGN_VCENTER, ALIGN_RIGHT);
}

VOID CBasicPrintMgr::OrderDataPrintByProvider()
{
	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	INT codeCnt = m_pInfo->pCodeMgr->GetCodeCount(_T("����ó����"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pInfo->pCodeMgr->GetCode(_T("����ó����"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		if(code == _T("") || desc == _T("�������"))
			continue;
		startrow++;
	}	
}

VOID CBasicPrintMgr::OrderDataPrintByKDCSubject()
{
	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	INT codeCnt = m_pInfo->pCodeMgr->GetCodeCount(_T("KDC��������ȣ����"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pInfo->pCodeMgr->GetCode(_T("KDC��������ȣ����"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		if(code == _T("") || desc == _T("�������"))
			continue;
		startrow++;
	}
}

VOID CBasicPrintMgr::OrderDataPrintByDDCSubject()
{
	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	INT codeCnt = m_pInfo->pCodeMgr->GetCodeCount(_T("DDC��������ȣ����"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pInfo->pCodeMgr->GetCode(_T("DDC��������ȣ����"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		if(code == _T("") || desc == _T("�������"))
			continue;
		startrow++;
	}
}

VOID CBasicPrintMgr::OrderDataPrintBySubject()
{
	OrderDataPrintByKDCSubject();
	OrderDataPrintByDDCSubject();	
}

INT CBasicPrintMgr::AddPrintData(CString strKey, CString strValue)
{
	if(strKey.IsEmpty()) return -1;

	m_MapPrintData.SetAt(strKey, strValue);
	return 0;
}

CString CBasicPrintMgr::GetPrintData(CString strKey)
{
	if(strKey.IsEmpty()) return _T("");

	CString strValue;
	m_MapPrintData.Lookup(strKey, strValue);
	return strValue;
}

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

INT CBasicPrintMgr::MakeXMLPageFile()
{
	if(m_pSIReportManager == NULL)
	{
		ESLAfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 0;
	}

	CESL_DataMgr *pPrintDM = FindDM(m_strPrintDMAlias);
	
	INT ids;
	
	CString strKey, strValue;
	POSITION pos = m_MapPrintData.GetStartPosition();
	while(pos != NULL)
	{
		m_MapPrintData.GetNextAssoc(pos, strKey, strValue);
		m_pSIReportManager->SetTempVariable(strKey, strValue);
	}
	
	m_pSIReportManager->SetCONNECTION_INFO(this->CONNECTION_INFO);
	ids = m_pSIReportManager->GetSIReportFileFromDB(m_strSIReportFileName);
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 0;
	}
	
	//m_pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	if(pPrintDM != NULL) m_pSIReportManager->SetDataMgr(0, pPrintDM, 0);
	
	m_pSIReportManager->MAKEPAGE_DrawWorkForm(-1);

	return 0;
}

VOID CBasicPrintMgr::Print2()
{
	m_pSIReportManager->Print2();
}

INT CBasicPrintMgr::InitRptFile()
{
	CString strTmp;
	CFile fd;
	INT i;
	for (i = 1; i < 99999; i++) {
		strTmp.Format(_T("rpt_tmp\\%s%04d.rpt"), m_pSIReportManager->m_strPageHeader, i);
		if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) break;
		fd.Close();
		DeleteFile(strTmp);

	}
	for (i = 1; i < 99999; i++) {
		strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_pSIReportManager->m_strPageHeader, i);
		if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) break;
		fd.Close();
		DeleteFile(strTmp);

	}

	return 0;
}

INT CBasicPrintMgr::Print(CESL_DataMgr *pDM)
{
	if(!pDM)
	{
		ESLAfxMessageBox(_T("��¿� Data Object�� ã�� �� �����ϴ�."));
		return -1;
	}
	
	INT ids;
	
	CString strKey, strValue;
	POSITION pos = m_MapPrintData.GetStartPosition();
	while(pos != NULL)
	{
		m_MapPrintData.GetNextAssoc(pos, strKey, strValue);
		m_pSIReportManager->SetTempVariable(strKey, strValue);
	}
	
	m_pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	ids = m_pSIReportManager->GetSIReportFileFromDB(m_strSIReportFileName);
	if (ids < 0)
	{
		ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return -1;
	}
	
	m_pSIReportManager->SetDataMgr(0, pDM, 0);
	
	if(m_PrintType == SERIES_PAPER)
	{
		m_pSIReportManager->m_nUserPaperSize = -9999;
		m_pSIReportManager->m_nOrientation = DMORIENT_PORTRAIT;
		m_pSIReportManager->m_nResolution = 1728; 
	}
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	m_pSIReportManager->m_bPrivacyPrintLogRecord = TRUE;
	ids = m_pSIReportManager->Print();
	
	return ids;
}
