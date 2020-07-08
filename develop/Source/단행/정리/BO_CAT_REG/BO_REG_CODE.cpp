// BO_REG_CODE.cpp : implementation file
//

#include "stdafx.h"
#include "BO_REG_CODE.h"
#include "BO_REG_MGR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_CODE dialog


CBO_REG_CODE::CBO_REG_CODE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_REG_CODE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_REG_CODE::~CBO_REG_CODE()
{
	
}
VOID CBO_REG_CODE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_REG_CODE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_REG_CODE, CDialog)
	//{{AFX_MSG_MAP(CBO_REG_CODE)
	ON_BN_CLICKED(IDC_bCHANGE_CODE, OnbCHANGECODE)
	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_CODE message handlers

BOOL CBO_REG_CODE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	 
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_���_�ڵ�")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	INT idx;

	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	if (idx < 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
		EndDialog(IDCANCEL);
		return FALSE;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG error") );
		return FALSE;
	}

	CString sTmpBookKey,sCodeTmp;
	
	sCodeTmp = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );
	sTmpBookKey = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );

	if(sCodeTmp != _T("BOR111N"))
	{
		AfxMessageBox(_T("�̹� ��ϵ� �ڷᰡ �ֽ��ϴ�."));	// ����
		EndDialog(IDCANCEL);
		return FALSE;			
	}

	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {

		sCodeTmp = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );
		CString sRecKeyTmp = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
		
		if(sCodeTmp != _T("BOR111N"))
		{
			AfxMessageBox(_T("�̹� ��ϵ� �ڷᰡ �ֽ��ϴ�."));	// ����
			EndDialog(IDCANCEL);
			return FALSE;			
		}

		if ( sRecKeyTmp != _T("") ) {
			sTmpBookKey = sTmpBookKey + _T(", ") + sRecKeyTmp;
		}
		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	if ( sTmpBookKey != _T("") ) {
		AfxMessageBox( _T("��Ϲ�ȣ �ο��� �ڷᰡ �ֽ��ϴ�.") );
		EndDialog(IDCANCEL);
		return FALSE;
	}


	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_REG_CODE::OnbCHANGECODE() 
{
	// TODO: Add your control notification handler code here

	ChangeCode();

}

INT CBO_REG_CODE::ChangeCode()
{

	INT idx;
	CString BookKey,strQry,sTmp,sReg;
	
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_����_���_����"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_����_����_���_���� error") );
		return -2;
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return -1;
	}

	// ������ ��� ���� �ڵ� �������� 
	GetControlData(_T("CM_����_����_���_�ڵ�"), _T("BB_��ϱ���"), sReg);


	// ��� åŰ ���� ���� 
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

	// ���� spl ����
	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return -1;
	}
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REG_CODE"), _T("STRING"), sReg );
	ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
	if ( 0 > ids ) return -1;
	ids = pDM->SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame �� Ȯ���ϼ���"));
		return -1;
	}

	pDM->SetCellData(_T("BB_��ϱ���_�ڵ�"), sReg, idx);
	pGrid->DisplayLine(idx);
	
	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

		ids = pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
			return -1;
		}

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("REG_CODE"), _T("STRING"), sReg );
		ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
		if ( 0 > ids ) return -1;
		ids = pDM->SendFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("SendFrame �� Ȯ���ϼ���"));
			return -1;
		}

		pDM->SetCellData(_T("BB_��ϱ���_�ڵ�"), sReg, idx);
		pGrid->DisplayLine(idx);
		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	AfxMessageBox(_T("���� �Ǿ����ϴ�."));



//	pCM->AllControlDisplay();

	pGrid->ReshowSelectedRows();

	return 0;	

}


HBRUSH CBO_REG_CODE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

