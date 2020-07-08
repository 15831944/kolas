// DLG_BATCH_MODIFY.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_BATCH_MODIFY.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_BATCH_MODIFY dialog


CDLG_BATCH_MODIFY::CDLG_BATCH_MODIFY(CESL_Mgr* pParent )
	: CESL_Mgr(CDLG_BATCH_MODIFY::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_BATCH_MODIFY)
	m_bCheck_1 = FALSE;
	m_bCheck_2 = FALSE;
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_pMainCM = NULL;
}


void CDLG_BATCH_MODIFY::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_BATCH_MODIFY)
	DDX_Check(pDX, IDC_CHECK1, m_bCheck_1);
	DDX_Check(pDX, IDC_CHECK2, m_bCheck_2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_BATCH_MODIFY, CDialog)
	//{{AFX_MSG_MAP(CDLG_BATCH_MODIFY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_BATCH_MODIFY message handlers

BOOL CDLG_BATCH_MODIFY::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	INT ids, count;
	CString strcnt;
	CESL_Grid* pGrid = NULL;

	if( InitESL_Mgr(_T("K18_�������_�ϰ�����")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_K18_�������_�ϰ�����"));
	m_pDM = FindDM(_T("DM_��ġ����ڷ����"));
	m_pMainCM = FindCM(_T("CM_��ġ����ڷ����"));
	pGrid		= (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_K18_�������_�ϰ�����]�� ������ �� �����ϴ�."));
		return FALSE;
	}


	if( m_pDM == NULL || m_pMainCM == NULL || pGrid == NULL )
	{
		ESLAfxMessageBox(_T("[CM/DM_��ġ����ڷ����]�� ������ �� �����ϴ�."));
		return FALSE;
	}


	ids = pGrid->SelectMakeList();
	count = pGrid->SelectGetCount();
	if( ids < 0 || count <= 0 )
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		this->EndDialog(IDCANCEL);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	strcnt.Format(_T("%d\n"), count);
	OutputDebugString( strcnt );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLG_BATCH_MODIFY::OnOK() 
{
	CString	str_Check_1 , str_Check_2;
	CString reckey, skey ;

	CESL_Grid* pGrid = NULL;
	pGrid		= (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));

	UpdateData(TRUE);

	if( m_bCheck_1 )	str_Check_1 = "Y";
	else				str_Check_1 = "N";

	if( m_bCheck_2 )	str_Check_2 = "Y";
	else				str_Check_2 = "N";

	INT ids, i;
	ids = pGrid->SelectMakeList();
	i =	pGrid->SelectGetHeadPosition();

	m_pDM->StartFrame();
	while( i >= 0 )
	{	
		m_pDM->InitDBFieldData();

		m_pDM->GetCellData(_T("��ġ���KEY"), i, reckey);

		m_pDM->AddDBFieldData(_T("RESERVATION_YN"), _T("STRING"), str_Check_1);
		m_pDM->AddDBFieldData(_T("SMS_RECEIPT_YN"), _T("STRING"), str_Check_2);

		m_pDM->MakeUpdateFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), reckey, i);	
		i = pGrid->SelectGetNext();	
	}

	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();

	int total_update = 0;
	if( ids >= 0 )
	{
		ids = pGrid->SelectMakeList();
		i =	pGrid->SelectGetHeadPosition();
		while( i >= 0 )
		{
			m_pDM->SetCellData(_T("�켱���⿹�࿩��")	, str_Check_1 , i);
			m_pDM->SetCellData(_T("SMS���ſ���")		, str_Check_2 , i);
			i = pGrid->SelectGetNext();	
			total_update++;
		}
		m_pCM->AllControlDisplay();
	}
	else if( ids < 0 )
	{
		ESLAfxMessageBox(_T("DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
	}


	CString str;
	str.Format( _T("�ϰ����� : ids : %d") , ids );
	OutputDebugString( str );

	if( total_update > 0 )
	{
		str.Format( _T("%d ���� �����Ǿ����ϴ�.") , total_update);
		AfxMessageBox( str );
	}

	CDialog::OnOK();
}

