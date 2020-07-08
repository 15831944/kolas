// BO_CAT_REG_0520.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_REG_0520.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0520 dialog


CBO_CAT_REG_0520::CBO_CAT_REG_0520(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0520)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CBO_CAT_REG_0520::~CBO_CAT_REG_0520()
{
}

VOID CBO_CAT_REG_0520::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0520)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0520, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0520)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0520 message handlers

BOOL CBO_CAT_REG_0520::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_REG_0520::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_0520")) < 0)
	//if (InitESL_Mgr(_T("BO_CAT_REG")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	Display(0);
		
	pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	if ( pDM == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG_0500 ERROR") );
		return FALSE;
	}
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

	// 040317_HSS �㶧 �׸��� �ٽ� �ѷ��� �ʿ� ����
	//CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_REG_0500"));
	//pCM1->AllControlDisplay(m_idx);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_0520::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
		idx = SelectGetHeadPosition(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
		if (idx < 0)
		{
			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		idx = SelectGetPrev(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
		if (idx == -1) {
			idx = SelectGetHeadPosition(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
			AfxMessageBox(_T("ó�� �ڷ��Դϴ�."));
			return;
		}
		break;
	case 1:
		idx = SelectGetNext(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
		if (idx == -1) {
			idx = SelectGetTailPosition(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
			AfxMessageBox(_T("������ �ڷ��Դϴ�."));
			return;
		}
		break;
	}
	/*
	if ( idx == 0 ) {
		GetDlgItem(IDC_bPREV)->EnableWindow(false);
	} else {
		GetDlgItem(IDC_bPREV)->EnableWindow(true);
	}

	if ( idx == SelectGetCount(_T("CM_MAIN_SAMPLE"), _T("MainGrid")) -1) {
		GetDlgItem(IDC_bNEXT)->EnableWindow(false);
	} else {
		GetDlgItem(IDC_bNEXT)->EnableWindow(true);
	}
	*/	
	m_idx = idx; 
	AllControlDisplay(_T("CM_BO_CAT_REG_0520"), m_idx);
	SetReverse(_T("CM_BO_CAT_REG_0500"), _T("GRID"), m_idx);

EFS_END
}

VOID CBO_CAT_REG_0520::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here

	ModifyGroup(m_idx);
	
	CDialog::OnOK();

EFS_END
}


INT CBO_CAT_REG_0520::ModifyGroup(INT idx)
{
	INT ids;
    CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	
	if (!pDM) return -1;
	
	
	///���� �۾� ���� 
	CString strID;
	//pCESLMgr->GetControlData(_T("CM_BO_CAT_REG_0520"), _T("����۾���"), strID);
	//strID = pDM->GetCellData ( idx , 0 );
	strID = pDM->GetCellData ( _T("REC_KEY"), idx );
	//pDM->GetCellData ( _T("REC_KEY"), idx, strID );

	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return -1;
	}

	pDM->InitDBFieldData();
	
	///���� �۾� ����  
	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_REG_0520"));

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //2003.10.20 ������ ����
	CString strAccYear ;
	CString strAccCode ;
	pCM->GetControlMgrData (_T("���Գ⵵"),strAccYear,-1);
    pCM->GetControlMgrData(_T("���Ա���"),strAccCode,-1) ;
    pDM->AddDBFieldData (_T("ACCESSION_REC_MAKE_YEAR"),_T("STRING"),strAccYear) ;
    //pDM->AddDBFieldData (_T("ACQUISIT_CODE"),_T("STRING"),strAccCode) ;
	
	CString strAccDis = _T("");
	if(strAccCode == _T("1")) strAccDis = _T("����");
	else if (strAccCode == _T("2")) strAccDis = _T("����");
	else if (strAccCode == _T("3")) strAccDis = _T("��ȯ");
	else if (strAccCode.IsEmpty()) strAccDis = _T("�������");

	pDM->SetCellData (_T("���Ա���"),strAccDis,idx) ;
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	ids = pDM->MakeControlMgrFiledData((CObject*)pCM);
	ids = pDM->MakeControlMgrUpdateFrame(_T("REC_KEY"), _T("STRING"), strID, idx);

	// 2005.06.27 ADD BY PDJ
	// ���ο� ���� �߰�
	

	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("������������"),WORK_LOG));

	//ids = pDM->MakeControlMgrUpdateFrame(_T("REGISTRANT"), _T("STRING"), strID, idx);
	//ids = pDM->MakeControlMgrUpdateFrame(_T("����۾���"), _T("STRING"), strID);
   	///���� �۾� �Ϸ� 


	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return -1;
	}
  
	ids = pDM->EndFrame();
	if ( 0 > ids ) return -1;

	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_REG_0500"));
	AllControlDisplay(_T("DM_BO_CAT_REG_0500"));

	pCM1->AllControlDisplay(idx);
 
	return 0;
}

VOID CBO_CAT_REG_0520::OnbNEXT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(1);		

	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );
	

EFS_END
}

VOID CBO_CAT_REG_0520::OnbPREV() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(-1);		

	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );
	

EFS_END
}


HBRUSH CBO_CAT_REG_0520::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
