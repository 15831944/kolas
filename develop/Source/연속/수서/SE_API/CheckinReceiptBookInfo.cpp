// CheckinReceiptBookInfo.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinReceiptBookInfo.h"
#include "SeApi.h"
#include "../../../ESL/SIReportMgr/SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinReceiptBookInfo dialog


CCheckinReceiptBookInfo::CCheckinReceiptBookInfo(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinReceiptBookInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strAcqCode = _T("");
	m_strAcqYear = _T("");
	m_strReceiptNo = _T("");
	m_nACQ_TYPE = 1; //defaut
}
CCheckinReceiptBookInfo::~CCheckinReceiptBookInfo()
{
}

VOID CCheckinReceiptBookInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinReceiptBookInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinReceiptBookInfo, CDialog)
	//{{AFX_MSG_MAP(CCheckinReceiptBookInfo)
	ON_BN_CLICKED(IDC_btnPRINT, OnbtnPRINT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinReceiptBookInfo message handlers

BOOL CCheckinReceiptBookInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(InitESL_Mgr(_T("����_����_üũ��_������ȣ_å����"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_üũ��_������ȣ_å����") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_üũ��_������ȣ_å����"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���"));
	m_pDM = FindDM(_T("DM_üũ��_������ȣ_å����"));
	

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : CM_üũ��_������ȣ, DM_üũ��_������ȣ") );
		return false;
	}



	CString strACQ_TYPE;
	if(1 == m_nACQ_TYPE) m_strAcqCode = _T("����");
	else m_strAcqCode = _T("����");

	if(m_strAcqYear.IsEmpty() || m_strReceiptNo.IsEmpty())
	{
		AfxMessageBox( _T("���Գ⵵ Ȥ�� ������ȣ�� �Էµ��� �ʾҽ��ϴ�!") );
		return false;
	}

	m_pCM->SetControlMgrData(_T("���Ա���"),m_strAcqCode);   
	m_pCM->SetControlMgrData(_T("���Գ⵵"),m_strAcqYear);  
	m_pCM->SetControlMgrData(_T("������ȣ"),m_strReceiptNo); 

	
	CString strQuery,strTmpQuery;

	strQuery += _T("I.REC_KEY = B.SPECIES_KEY");
	strQuery += _T(" AND V.REC_KEY = B.VOL_KEY");
	strQuery += _T(" AND A.REC_KEY = B.ACQ_TYPE_VOL_KEY");
	strQuery += _T(" AND B.BOOK_TYPE = 'C'");

	strTmpQuery.Format(_T(" AND B.ACQ_CODE = '%d' AND B.ACQ_YEAR = '%s' AND B.RECEIPT_NO = %s")
					,m_nACQ_TYPE,m_strAcqYear,m_strReceiptNo);
	
	strQuery  += strTmpQuery;


	m_pDM->RefreshDataManager(strQuery);

	INT nRow = 0;
	nRow = m_pDM->GetRowCount();
	if(0 == nRow)
	{
		AfxMessageBox(_T("�ش� ������ȣ���� å������ �����ϴ�!"));
		EndDialog(0);
		return FALSE;
	}
	
	CString strCnt;
	strCnt.Format(_T("%d"),nRow);
	m_pCM->SetControlMgrData(_T("å��"),strCnt); 


	//code to descript
	//�ڷ�� 
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("�ڷ�Ǳ���"),_T("UDF_�ڷ��"));
	
	//�ڷ����
	CSeApi api(this);
	CString strWorkingStatus,strDesc;

	for(INT i=0;i<nRow;i++)
	{
		strWorkingStatus = m_pDM->GetCellData(_T("SB_�ڷ����"),i);
		strDesc = api.GetWorkingStatusDesc(strWorkingStatus);
		m_pDM->SetCellData(_T("UDF_�ڷ����"),strDesc,i);
	}


	m_pGrid->Display();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//2005-12-23 Ȳ���� : ������ �߰�
VOID CCheckinReceiptBookInfo::OnbtnPRINT() 
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("����� �ڷᰡ �����ϴ�!"));
		return;
	}

	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����_������ȣ��_å���"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}
	
	//=====================================================
	//2009.01.29 REM BY PJW : ������ ��������� �ʿ� ������ �ڵ���
// 	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	//=====================================================
	
	pSIReportManager->SetDataMgr(0, m_pDM, 0);
	
	pSIReportManager->SetTempVariable(_T("���Ա���"),m_strAcqCode);
	pSIReportManager->SetTempVariable(_T("���Գ⵵"),m_strAcqYear);
	pSIReportManager->SetTempVariable(_T("������ȣ"),m_strReceiptNo);
	
	
	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

	return;	
}

HBRUSH CCheckinReceiptBookInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
