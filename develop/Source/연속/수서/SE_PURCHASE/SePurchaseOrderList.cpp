// SePurchaseOrderList.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseOrderList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseOrderList dialog


CSePurchaseOrderList::CSePurchaseOrderList(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseOrderList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pCM = NULL;
	m_pDM_SEQ_NO = NULL;

}

CSePurchaseOrderList::~CSePurchaseOrderList()
{
}

VOID CSePurchaseOrderList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseOrderList)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseOrderList, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseOrderList)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseOrderList message handlers

BOOL CSePurchaseOrderList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(InitESL_Mgr( _T("����_����_�����ڷ����_�ֹ����") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_�����ڷ����_�ֹ����") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_����_�����ڷ����_�ֹ����"));
	m_pDM = FindDM(_T("DM_����_�����ڷ����_�ֹ����"));
	m_pDM_SEQ_NO = FindDM(_T("DM_����_�����ڷ����_�ֹ����_����"));
	
	if(!m_pDM || !m_pCM || !m_pDM_SEQ_NO ) {
		AfxMessageBox( _T("DM,CM Error : ����_����_�����ڷ����_�ֹ����") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSePurchaseOrderList::OnOK() 
{
	// TODO: Add extra validation here
	
	CString ORDER_YEAR;
	CString SEQ_NO1;
	CString SEQ_NO2;
	INT nSeqNo1 = 0;
	
	m_pCM->GetControlMgrData(_T("�����⵵"),ORDER_YEAR);
	m_pCM->GetControlMgrData(_T("������ȣ1"),SEQ_NO1);
	m_pCM->GetControlMgrData(_T("������ȣ2"),SEQ_NO2);
	
	if(ORDER_YEAR.IsEmpty())
	{
		AfxMessageBox(_T("�����⵵�� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}
	
	if(SEQ_NO1.IsEmpty())
	{
		AfxMessageBox(_T("������ȣ�� �պκ��� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}

	if(SEQ_NO1 == SEQ_NO2)
	{
		SEQ_NO2.Empty(); //���� �˻����� ��ȯ
	}
	

	nSeqNo1 = _ttoi(SEQ_NO1.GetBuffer(0));
	if(0 == nSeqNo1) 
	{
		AfxMessageBox(_T("������ȣ �պκ��� 0 �� �� �� �����ϴ�!"));
		return;
	}

	if(!SEQ_NO2.IsEmpty())
	{
		INT nSeqNo2 = 0;
		nSeqNo2 = _ttoi(SEQ_NO2.GetBuffer(0));
		
		if(nSeqNo1>nSeqNo2) {
			AfxMessageBox(_T("������ȣ �պκ��� �޺κк��� Ŭ�� �����ϴ�!"));
			return;
		}
	}

	//�ֹ� ó������ �ƴ� ������ �ִ� �� �����Ѵ�. 
	if(CheckPurchaseNo(ORDER_YEAR,SEQ_NO1,SEQ_NO2)<0) return;

	
	CString strQuery;
	CString strtmpquery;
	
	strtmpquery.Format(_T("ORDER_YEAR = '%s'"),ORDER_YEAR);
	strQuery += strtmpquery;

	strtmpquery.Empty();
	strtmpquery = MakeIntRangeQuery(_T("ORDER_SEQ_NO"),SEQ_NO1,SEQ_NO2);
	strQuery += _T(" AND ");
	strQuery += strtmpquery;

	m_pDM->RefreshDataManager(strQuery);
	
	INT nRowCnt = m_pDM->GetRowCount();
	
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("�ش� ���� ���� �ڷᰡ �����ϴ�!"));
		return;
	}
	
	PrintOrderList();

	CDialog::OnOK();
}

CString CSePurchaseOrderList::MakeIntRangeQuery(CString strFieldName, CString strData1, CString strData2)
{

	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s = %s"),strFieldName,strData1);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN %s AND %s"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

INT	CSePurchaseOrderList::CheckPurchaseNo(CString strYear,CString strNo1, CString strNo2) 
{
	
	//���� ������ ���� �۾� ���¸� �˻��� �ֹ��� �ƴ� �ڷᰡ �ִ����� �����Ѵ�
	m_pDM_SEQ_NO->FreeData();

	CString strQuery;
	CString strtmpquery;
	
	strtmpquery.Format(_T("ACQ_YEAR = '%s'"),strYear);
	strQuery += strtmpquery;

	strtmpquery.Empty();
	strtmpquery = MakeIntRangeQuery(_T("SEQ_NO"),strNo1,strNo2);
	strQuery += _T(" AND ");
	strQuery += strtmpquery;

	m_pDM_SEQ_NO->RefreshDataManager(strQuery);
	
	INT nRowCnt;
	nRowCnt = m_pDM_SEQ_NO->GetRowCount();

	if(0 == nRowCnt) {
		AfxMessageBox(_T("�˻� ���ǿ� �ش��ϴ� ������ �����ϴ�!"));
		return -1;
	}
	
	CString strWORKING_STATUS;

	for(INT i=0;i<nRowCnt;i++)
	{
		strWORKING_STATUS = _T("");
		strWORKING_STATUS = m_pDM_SEQ_NO->GetCellData(_T("�۾�����"),i);
		if(_T("1") != strWORKING_STATUS)
		{
			CString strSEQ_NO;
			CString msg;
			strSEQ_NO = m_pDM_SEQ_NO->GetCellData(_T("������ȣ"),i);
			msg.Format(_T("[ %s ] ������ �ֹ����� �ƴմϴ�! \n Ȯ���Ͽ� �ֽʽÿ�."),strSEQ_NO);
			AfxMessageBox(msg);
			return -2;
		}
	}

	return 0;
}

VOID CSePurchaseOrderList::PrintOrderList()
{
	//����ó ���� 
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("����ó����"),_T("UDF_����ó"));
	
	//������,�����ڸ� ��ũ���� ������ �´�
	CMarc Marc;
	CString strStreamMarc = _T("");
	CString strTitle,strPublisher;
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	
	INT nRowCnt = m_pDM->GetRowCount();

	for(INT i=0;i<nRowCnt;i++)
	{
		strStreamMarc = m_pDM->GetCellData(_T("MARC"),i);
		
		m_pInfo->pMarcMgr->Decoding(strStreamMarc,&Marc);
		
		strTitle = _T("");
		strPublisher = _T("");
		
		m_pInfo->pMarcMgr->GetItem(&Marc,_T("245$a"),strTitle);
		m_pInfo->pMarcMgr->GetItem(&Marc,_T("260$b"),strPublisher);
		
		m_pDM->SetCellData(_T("UDF_������"),strTitle,i);
		m_pDM->SetCellData(_T("UDF_������"),strPublisher,i);
	}
	
	
	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("�����ֹ��ڷ���"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return ;
	}
	
	//=====================================================
	//2009.01.29 REM BY PJW : ������ ���������� �ʿ� ������ �ڵ���
// 	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	//=====================================================	
	pSIReportManager->SetDataMgr(0, m_pDM, 0);
	
	/*
	pSIReportManager->SetTempVariable(_T("����"),m_strCondition);
	*/

	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}



}
HBRUSH CSePurchaseOrderList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
