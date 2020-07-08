// BO_LOC_3875.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3875.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3875 dialog


CBO_LOC_3875::CBO_LOC_3875(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3875)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CBO_LOC_3875::~CBO_LOC_3875()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CBO_LOC_3875::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3875)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3875, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3875)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3875 message handlers

BOOL CBO_LOC_3875::Create( CWnd* pParentWnd ) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3875::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("SM_BO_LOC_3875") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	InitGroupCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;
}

INT CBO_LOC_3875::InitGroupCombo()
{
	EFS_BEGIN

	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3870_GROUP") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitGroupCombo()") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3875"));
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
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3875::InitGroupCombo() := SetData()") );

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

VOID CBO_LOC_3875::OnbSAVE() 
{
EFS_BEGIN
	SaveProc();
EFS_END}

VOID CBO_LOC_3875::OnbCLOSE() 
{
EFS_BEGIN
	OnCancel();
EFS_END	
}

INT CBO_LOC_3875::SaveProc() 
{
EFS_BEGIN

	INT ids = 0 , i=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3870") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveProc()") );
	
	if( pDM->GetRowCount() == 0 ) 
	{
		AfxMessageBox( _T("�̿��ڸ� �˻����ֽʽÿ�.") );
		return 1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3870") , _T("��ȸ�����α׸���") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveProc") );

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�̿��ڸ� �������ֽʽÿ�.") );
		return 2;
	}

	CString sGroup;
	ids = GetControlData( _T("CM_BO_LOC_3875") , _T("�׷��ڵ�") , sGroup , -1 , 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("SaveProc") ); 

	if( sGroup.IsEmpty() )
	{
		AfxMessageBox(_T("������ �׷��� �������ֽʽÿ�."));
		return 3;
	}


	// DB�� ������Ʈ�ϱ�.
	pDM->StartFrame();

	pGrid->SelectGetHeadPosition();

	CArray < INT , INT > aUIndexs;


	INT nIndex;
	CString sRecKey;
	for( i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		aUIndexs.Add(nIndex);

		ids = pDM->GetCellData( _T("��ȸ��Ű") , nIndex , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("SaveProc") );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("GROUP_CODE") , _T("STRING") , sGroup );
		ids = pDM->MakeUpdateFrame( _T("CO_NONMEMBER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("SaveProc") );

		pGrid->SelectGetNext();
	}

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("SaveProc") );

	pDM->EndFrame();


	// DM, �׸��忡 �ݿ��ϱ�.
	for( i = 0 ; i < nSelectCnt ; i++ ){
		ids = pDM->SetCellData( _T("�׷��ڵ�") , sGroup ,aUIndexs.GetAt(i) );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("SaveProc") );

		pGrid->DisplayLine( aUIndexs.GetAt(i) );
	}

	sRecKey.Format(_T("%d���� �̿��ڸ� �׷��ڵ� %s�� �����Ͽ����ϴ�."), nSelectCnt , sGroup);
	AfxMessageBox(sRecKey);

	return 0;
EFS_END	
	return -1;
}

HBRUSH CBO_LOC_3875::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
