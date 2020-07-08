// KL_PaymentListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KL_PaymentListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_PaymentListDlg dialog


CKL_PaymentListDlg::CKL_PaymentListDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_PaymentListDlg)
		m_strPaymentKey = _T("");
		m_nSelectRow = 0;
	//}}AFX_DATA_INIT
}

CKL_PaymentListDlg::~CKL_PaymentListDlg()
{
}

void CKL_PaymentListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_PaymentListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_PaymentListDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_PaymentListDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_PaymentListDlg message handlers

BOOL CKL_PaymentListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitESL_Mgr(_T("SM_KL_PAYMENT_LIST"));
	EnableThemeDialogTexture(GetSafeHwnd());

	CESL_DataMgr* pLoanDM = FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	CESL_DataMgr* pDM = FindDM(_T("DM_KL_PAYMENT_LIST"));

	CString strData;
	INT nInsertRow = 0;
	if(pLoanDM && pDM && !m_strPaymentKey.IsEmpty())
	{
		const INT ROW_COUNT = pLoanDM->GetRowCount();
		for(INT nRow = 0; nRow < ROW_COUNT; nRow++)
		{
			pLoanDM->GetCellData(_T("����ó��KEY"), nRow, strData);
			if(strData == m_strPaymentKey)
			{
				pDM->InsertRow(-1);

				pLoanDM->GetCellData(_T("���⵵������ȣ"), nRow, strData);
				pDM->SetCellData(_T("���⵵������ȣ"), strData, nInsertRow);

				pLoanDM->GetCellData(_T("������"), nRow, strData);
				pDM->SetCellData(_T("������"), strData, nInsertRow);

				pLoanDM->GetCellData(_T("�ݳ�������"), nRow, strData);
				pDM->SetCellData(_T("�ݳ�������"), strData, nInsertRow);

				pLoanDM->GetCellData(_T("������"), nRow, strData);
				pDM->SetCellData(_T("������"), strData, nInsertRow);

				pLoanDM->GetCellData(_T("���ุ����"), nRow, strData);
				pDM->SetCellData(_T("���ุ����"), strData, nInsertRow);

				pLoanDM->GetCellData(_T("����"), nRow, strData);
				pDM->SetCellData(_T("�������"), strData, nInsertRow);

				pLoanDM->GetCellData(_T("��Ϲ�ȣ"), nRow, strData);
				pDM->SetCellData(_T("��Ϲ�ȣ"), strData, nInsertRow);

				pLoanDM->GetCellData(_T("��ǥ��"), nRow, strData);
				pDM->SetCellData(_T("����"), strData, nInsertRow);

				pLoanDM->GetCellData(_T("û����ȣ"), nRow, strData);
				pDM->SetCellData(_T("û����ȣ"), strData, nInsertRow);

				pLoanDM->GetCellData(_T("��Ȳ����"), nRow, strData);
				pDM->SetCellData(_T("��Ȳ����"), strData, nInsertRow);

				pLoanDM->GetCellData(_T("����ó��KEY"), nRow, strData);
				pDM->SetCellData(_T("����ó��KEY"), strData, nInsertRow);

				pLoanDM->GetCellData(_T("���⵵����"), nRow, strData);
				pDM->SetCellData(_T("���⵵����"), strData, nInsertRow);

				nInsertRow++;
			}
		}
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_KL_PAYMENT_LIST"), _T("�׸���"));
	if(pGrid)
	{
		pGrid->Display();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
