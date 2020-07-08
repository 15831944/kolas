// BO_LOC_3871.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3871.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3871 dialog


CBO_LOC_3871::CBO_LOC_3871(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3871)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CBO_LOC_3871::~CBO_LOC_3871()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CBO_LOC_3871::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3871)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3871, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3871)
	ON_BN_CLICKED(IDC_bINSERT, OnbINSERT)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3871 message handlers


BOOL CBO_LOC_3871::Create( CWnd* pParentWnd ) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3871::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CBO_LOC_3871::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("SM_BO_LOC_3871") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	InitGroupCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;
}

VOID CBO_LOC_3871::OnbINSERT() 
{
EFS_BEGIN
	InsertProc();
EFS_END	
}

VOID CBO_LOC_3871::OnbCLOSE() 
{
EFS_BEGIN
	OnCancel();
EFS_END	
}


INT CBO_LOC_3871::InsertProc()
{
EFS_BEGIN
	
	INT ids = 0;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3870") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertProc") );

	CString sRecKey;

	CString sName;
	CString sCivilno;
	CString sHandphone;
	CString sEmail;
	CString sGroup;

	CString sCMAlias = _T("CM_BO_LOC_3871");
	// �̸�
	ids = GetControlData( sCMAlias , _T("�̸�") , sName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertProc") );
	if(sName.IsEmpty()){
		AfxMessageBox(_T("�̸��� �Է��Ͽ� �ֽʽÿ�."));
		return 0;
	}
	// �ֹε�Ϲ�ȣ
	ids = GetControlData( sCMAlias , _T("�ֹε�Ϲ�ȣ") , sCivilno );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertProc") );
/*	if(sCivilno.IsEmpty()){
		AfxMessageBox(_T("�ֹε�Ϲ�ȣ�� �Է��Ͽ� �ֽʽÿ�."));
		return 0;
	}
*/	// �ڵ���
	ids = GetControlData( sCMAlias , _T("�ڵ���") , sHandphone);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("InsertProc") );

	// �̸���
	ids = GetControlData( sCMAlias , _T("�̸���") , sEmail );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("InsertProc") );

	// �׷��ڵ�
	ids = GetControlData( sCMAlias , _T("�׷��ڵ�") , sGroup );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("InsertProc") );
	if(sGroup.IsEmpty()){
		AfxMessageBox(_T("�׷��ڵ带 �����Ͽ� �ֽʽÿ�."));
		return 0;
	}

	pDM->MakeRecKey(sRecKey);

	pDM->StartFrame();

	pDM->InitDBFieldData();
		
	pDM->AddDBFieldData( _T("NAME") , _T("STRING") , sName );
	pDM->AddDBFieldData( _T("CIVIL_NO") , _T("STRING") , sCivilno );
	pDM->AddDBFieldData( _T("HANDPHONE") , _T("STRING") , sHandphone );
	pDM->AddDBFieldData( _T("E_MAIL") , _T("STRING") , sEmail );
	pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	pDM->AddDBFieldData( _T("GROUP_CODE") , _T("STRING") , sGroup );

	ids = pDM->MakeInsertFrame( _T("CO_NONMEMBER_TBL") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("InsertProc") );

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("InsertProc") );

	pDM->EndFrame();


	/// DM�� ����
	INT nInsertIndex = pDM->GetRowCount();
	pDM->InsertRow(-1);

	ids = pDM->SetCellData( _T("�̸�") , sName , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("InsertProc") );
	ids = pDM->SetCellData( _T("�ֹε�Ϲ�ȣ") , sCivilno , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("InsertProc") );
	ids = pDM->SetCellData( _T("�ڵ���") , sHandphone , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("InsertProc") );
	ids = pDM->SetCellData( _T("�̸���") , sEmail , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -27 , _T("InsertProc") );
	ids = pDM->SetCellData( _T("��ȸ��Ű") , sRecKey , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -29 , _T("InsertProc") );
	ids = pDM->SetCellData( _T("�׷��ڵ�") , sGroup , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -30 , _T("InsertProc") );

	/// ȭ�鿡 �����ֱ�
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3870") , _T("��ȸ�����α׸���") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("InsertProc") );

	pGrid->DisplayLine( nInsertIndex );
	pGrid->DisplayNumber();

	AfxMessageBox(_T("�Է��Ͽ����ϴ�."));


	return 0;
EFS_END
	return -1;
}

INT CBO_LOC_3871::InitGroupCombo()
{
	EFS_BEGIN

	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3870_GROUP") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitGroupCombo()") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3871"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitGroupCombo()") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("�׷��ڵ�")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitGroupCombo()") );

	ids = pDM->RefreshDataManager(_T(""));
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitGroupCombo()") );


	CString sInsertCode[2];
	
	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("   ");
	sInsertCode[1] = _T("�������");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3871::InitGroupCombo() := SetData()") );

	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("�ڵ�") , i , sInsertCode[0] );
		pDM->GetCellData( _T("�̸�") , i , sInsertCode[1] );

		sInsertCode[0].TrimLeft(); sInsertCode[0].TrimRight();
		if( sInsertCode[0].IsEmpty() ) continue;
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitControl") );
	}

	pCMCB->SetCurSel(0);

	return 0;

	EFS_END
	return -1;
}

HBRUSH CBO_LOC_3871::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
