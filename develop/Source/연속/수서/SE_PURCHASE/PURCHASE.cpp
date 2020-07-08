// PURCHASE.cpp : implementation file
//

#include "stdafx.h"
#include "PURCHASE.h"
#include "..\SE_API\SeApi.h"
#include "SePurchaseOrder.h"
#include "..\SE_API\SeFileImport.h"
#include "..\SE_API\SePurchaseNoDlg.h"
#include "SePurchaseOrderList.h"
#include "SePurchaseMail.h"
#include "SePurchase.h"
#include "Purchase_PriceChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPURCHASE dialog


CPURCHASE::CPURCHASE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CPURCHASE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPURCHASE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pCM = NULL;
	m_nCurrentIdx = -1;

	m_bIsLightVersion = FALSE;
	m_bIsDepartMngUse = FALSE;

	m_strSEQ_NO = _T("");
	m_strSEQ_YEAR = _T("");
	AfxInitRichEdit();
}

CPURCHASE::~CPURCHASE()
{
	//search dialog delete
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
}

VOID CPURCHASE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPURCHASE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPURCHASE, CDialog)
	//{{AFX_MSG_MAP(CPURCHASE)
	ON_WM_SIZE()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPURCHASE message handlers
BOOL CPURCHASE::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CPURCHASE::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridPURCHASE);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);
	
}

BOOL CPURCHASE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		
	if(InitESL_Mgr( _T("����_����_�����ڷ����") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_�����ڷ����") );
		return false;
	}

	CESL_ControlMgr* pCM = FindCM(_T("CM_����_�����ڷ����"));
	m_pGrid = (CESL_Grid*)pCM->FindControl(_T("�׸���"));
	m_pDM = FindDM(_T("DM_����_�����ڷ����"));
	
	if(!m_pDM) {
		AfxMessageBox( _T("DM Error : DM_����_�����ڷ����") );
		return false;
	}
	
	//����Ʈ �������θ� �����´�
	CSeApi api(this);
	m_bIsLightVersion = api.IsLightVersion(m_pDM);
	

	//�μ����� ��� ���θ� �����´�
	m_bIsDepartMngUse = api.IsDepartMngUse(m_pDM);


	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CSePurchaseSearch(this);
		m_pDlg->pMain = this->pMain;
		m_pDlg->pParentMgr = (CESL_Mgr*)this;
		m_pDlg->m_pInfo = this->m_pInfo;
		m_pDlg->Create(this);
		m_pDlg->ShowWindow(FALSE);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CPURCHASE::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
		//=====================================================
		//2009.02.09 ADD  BY PJW : ��˻��� �׸��带 �ٽ� �ѷ��ش�.
		m_pGrid->Display();
		//=====================================================
		
	}
}


VOID CPURCHASE::OnSendQuery(WPARAM w,LPARAM l)
{
	
	CString strQuery;
	strQuery = m_pDlg->m_strQuery;
	m_strSEQ_NO = m_pDlg->m_strSEQ_NO;
	m_strSEQ_YEAR = m_pDlg->m_strSEQ_YEAR;

	m_pDM->RefreshDataManager(strQuery);
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("����ó����"),_T("UDF_����ó"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("�ڷᱸ��"),_T("UDF_�ڷᱸ��"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("��������"),_T("UDF_��������"));


	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();


	//�����۾����� 
	CString strWorking_status;
	CString strUDF;
	for(INT i=0;i<nRowCnt;i++)
	{
		strUDF = _T("");
		strWorking_status = m_pDM->GetCellData(_T("�����۾�����"),i);
	
		if(_T("0") == strWorking_status) strUDF = _T("���Դ��");
		else if(_T("1") == strWorking_status) strUDF = _T("�ֹ���");
		
		m_pDM->SetCellData(_T("UDF_�����۾�����"),strUDF,i);
	}


	m_pGrid->Display();
	m_nCurrentIdx = -1;


	if(0 == nRowCnt)
	{
		CString str;
		str.Format(_T("�˻� ����� �����ϴ�."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
		AfxMessageBox(str);
	}
	else
	{
		CString str;
		str.Format(_T(" %d ���� �ڷᰡ �˻��Ǿ����ϴ�."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
	}
	if( nRowCnt > 0 )
		m_pDlg->ShowWindow(SW_HIDE);
}


VOID CPURCHASE::InsertPurchaseSpecies()
{
	CSePurchase dlg(this);
	dlg.m_nOpenMode = 1;
	dlg.m_pParentDM = m_pDM;
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_bIsLightVersion = m_bIsLightVersion;
	dlg.m_bIsDepartMngUse = m_bIsDepartMngUse;
	dlg.DoModal();
}


VOID CPURCHASE::ModifyPurchaseSpecies()
{
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("���� �� ������ �����ϴ�!"));
		return;
	}
	else
	{
		CSePurchase dlg(this);
		m_pGrid->SelectMakeList();
		INT cnt = m_pGrid->SelectGetCount();

		if(cnt>0) dlg.m_nMoveOption = 2;
		else dlg.m_nMoveOption = 1;
		
		dlg.m_nCurrentIdx = m_nCurrentIdx;
		dlg.m_nOpenMode = -1;
		dlg.m_pParentDM = m_pDM;
		dlg.m_pParentGrid = m_pGrid;
		dlg.m_bIsLightVersion = m_bIsLightVersion;
		dlg.m_bIsDepartMngUse = m_bIsDepartMngUse;
		dlg.DoModal();
	}


}

VOID CPURCHASE::DeletePurchaseSpecies()
{
	m_pGrid->SelectMakeList();

	INT idx = m_pGrid->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
	    return;
	}
	
	CString msg;
	INT nCount = m_pGrid->SelectGetCount();
	msg.Format(_T("[ %d ] ���� �ڷḦ ���� �Ͻðڽ��ϱ�?"),nCount);

	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return; 
	
	CSeApi api(this);
	INT ids = 0;
	CArray<INT,INT> arrayDeletedRow;

	while(idx>=0)
	{
		
		ids = api.SPECIES_PUR_OBJ_DELETE(m_pDM,idx);

		if(ids<0)
		{
			
			msg.Format(_T("[ %d ]��° �ڷḦ �����ϴ� �� ������ �߻��� �۾��� �ߴ��մϴ�!"),idx+1);
			AfxMessageBox(msg);
			return; 	
		}
		
		arrayDeletedRow.Add(idx);
		idx = m_pGrid->SelectGetNext();
	}
	
	
	INT nSize = arrayDeletedRow.GetSize();
	INT nDeletedRow;

	for(INT i=0;i<nSize;i++)
	{
		nDeletedRow = arrayDeletedRow.GetAt(nSize-i-1);
		m_pDM->DeleteRow(nDeletedRow);
	}
	
	m_pGrid->Display();

	msg.Format(_T("[ %d ] ���� �ڷḦ �����Ͽ����ϴ�!"),nCount);
	AfxMessageBox(msg);
	return; 	
}

VOID CPURCHASE::OrderProcess()
{
	//���� �˻��� �Ǿ� �ְ�, �˻��� �����⵵�� ������ȣ�� ���Դ���� ���� �ֹ� Ŭ������ 
	//�⺻ ������ ������ �ش�
	CSePurchaseOrder dlg(this);
	
	if(!m_strSEQ_NO.IsEmpty() && !m_strSEQ_YEAR.IsEmpty())
	{
		if(m_pDM->GetRowCount()>0)
		{

			CString strQuery;
			CString strResult;
			strQuery.Format(_T("SELECT WORKING_STATUS FROM SE_PURCHASE_SEQ_NO_TBL ")
				            _T("WHERE ACQ_YEAR = %s AND SEQ_NO = %s AND MANAGE_CODE=UDF_MANAGE_CODE"),m_strSEQ_YEAR,m_strSEQ_NO);
		
			m_pDM->GetOneValueQuery(strQuery,strResult);
			if(_T("0") == strResult)
			{
				dlg.m_strParentYear = m_strSEQ_YEAR;
				dlg.m_strParentNo = m_strSEQ_NO;
				
			}

		}


	}
	
	

	dlg.DoModal();
}

VOID CPURCHASE::OrderCancel()
{


}

VOID CPURCHASE::OrderMail()
{
	//JOB.2019.0073 : �浹 License
// 	CSePurchaseMail dlg(this);
// 	dlg.DoModal();

}

VOID CPURCHASE::FileImport()
{

	CSeFileImport dlg(this);
	dlg.DoModal();

}

VOID CPURCHASE::OrderList()
{
	CSePurchaseOrderList dlg(this);
	dlg.DoModal();

}

VOID CPURCHASE::OrderSeqNo()
{

	CSePurchaseNoDlg dlg(this);
	dlg.m_nOpenMode = -1;
	dlg.DoModal();

}

VOID CPURCHASE::PrintGridData()
{
	INT nRowCnt = m_pDM->GetRowCount();

	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("����� �ڷᰡ �����ϴ�!"));
		return;
	}

	//������,�����ڸ� ��ũ���� ������ �´�
	CMarc Marc;
	CString strStreamMarc = _T("");
	CString strTitle,strPublisher;
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));


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
	ids = pSIReportManager->GetSIReportFileFromDB(_T("���ӱ����ڷ���"));
	
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return ;
	}
	//=====================================================
	//2009.01.29 REM BY PJW : ������ ���������� �ʿ� ������ �ڵ���
// 	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	//=====================================================
 	
	pSIReportManager->SetDataMgr(0, m_pDM, 0);
	
	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

}

BEGIN_EVENTSINK_MAP(CPURCHASE, CDialog)
    //{{AFX_EVENTSINK_MAP(CPURCHASE)
	ON_EVENT(CPURCHASE, IDC_gridPURCHASE, -600 /* Click */, OnClickgridPURCHASE, VTS_NONE)
	ON_EVENT(CPURCHASE, IDC_gridPURCHASE, -601 /* DblClick */, OnDblClickgridPURCHASE, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CPURCHASE::OnClickgridPURCHASE() 
{
	if(0 == m_pDM->GetRowCount()) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
			m_nCurrentIdx = row-1;
	}
}

VOID CPURCHASE::OnDblClickgridPURCHASE() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pGrid->GetMouseRow()) return;
	ModifyPurchaseSpecies();
}

VOID CPURCHASE::PriceChange()
{
	//���԰��� �ϰ�����.. ȭ���� ����.
	CPurchase_PriceChange dlg;
	dlg.SetInfo (m_pDM, m_pGrid);
	dlg.DoModal();
}

HBRUSH CPURCHASE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
