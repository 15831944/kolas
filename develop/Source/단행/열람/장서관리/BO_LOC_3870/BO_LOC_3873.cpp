// BO_LOC_3873.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3873.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3873 dialog


CBO_LOC_3873::CBO_LOC_3873(CESL_Mgr* pParent /*=NULL*/)
	: CPreNextDlg(pParent,IDD)
{
	//{{AFX_DATA_INIT(CBO_LOC_3873)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CBO_LOC_3873::~CBO_LOC_3873()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CBO_LOC_3873::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3873)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3873, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3873)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3873 message handlers

BOOL CBO_LOC_3873::Create( CWnd* pParentWnd ) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3873::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CBO_LOC_3873::OnInitDialog() 
{
EFS_BEGIN

	CPreNextDlg::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("SM_BO_LOC_3873") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	InitGroupCombo();

	InitControl();
	ViewScreen();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;
}

VOID CBO_LOC_3873::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

VOID CBO_LOC_3873::OnbCLOSE() 
{
EFS_BEGIN
	OnCancel();
EFS_END
}

VOID CBO_LOC_3873::OnbSAVE() 
{
EFS_BEGIN
	AfxGetApp()->DoWaitCursor(1);

	UpdateProc();

	AfxGetApp()->DoWaitCursor(-1);	
EFS_END
}

INT CBO_LOC_3873::ViewScreen()
{
EFS_BEGIN
	INT ids;

	ids = AllControlDisplay(_T("CM_BO_LOC_3873") , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ViewScreen") );

	GroupComboSetting();

	ControlDisplay(_T("CM_BO_LOC_3873") , _T("�׷��ڵ�"), m_nCurrentIndex );

	return 0;
EFS_END
return -1;

}

INT CBO_LOC_3873::InitControl()
{
EFS_BEGIN
	if( CPreNextDlg::m_nDlgType == __ECO_NOT_GRID_MODE )
	{
		GetDlgItem(IDC_bPREV)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_bNEXT)->ShowWindow(SW_HIDE);
	}

	return 0;
EFS_END
return -1;

}

VOID CBO_LOC_3873::OnbPREV() 
{
EFS_BEGIN
	CPreNextDlg::MovePrev();
	ViewScreen();
EFS_END
}

VOID CBO_LOC_3873::OnbNEXT() 
{
EFS_BEGIN
	CPreNextDlg::MoveNext();
	ViewScreen();
EFS_END
}

INT CBO_LOC_3873::UpdateProc()
{
EFS_BEGIN
	INT ids = 0;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3870") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("UpdateProc") );

	CString sRecKey;
	ids = pDM->GetCellData( _T("��ȸ��Ű") , m_nCurrentIndex , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("UpdateProc") ); 

	CString sName;
	CString sCivilno;
	CString sHandphone;
	CString sEmail;
	CString sGroup;

	CString sCMAlias = _T("CM_BO_LOC_3873");
	// �̸�
	ids = GetControlData( sCMAlias , _T("�̸�") , sName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("UpdateProc") );
	// �ֹε�Ϲ�ȣ
	ids = GetControlData( sCMAlias , _T("�ֹε�Ϲ�ȣ") , sCivilno );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("UpdateProc") );
	// �ڵ���
	ids = GetControlData( sCMAlias , _T("�ڵ���") , sHandphone);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("UpdateProc") );
	// �̸���
	ids = GetControlData( sCMAlias , _T("�̸���") , sEmail );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("UpdateProc") );
	// �׷��ڵ�
	ids = GetControlData( sCMAlias , _T("�׷��ڵ�") , sGroup );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("UpdateProc") );
	if(sGroup.IsEmpty()){
		AfxMessageBox(_T("�׷��ڵ带 �����Ͽ� �ֽʽÿ�."));
		return 0;
	}


	pDM->StartFrame();

	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("NAME") , _T("STRING") , sName );
	pDM->AddDBFieldData( _T("CIVIL_NO") , _T("STRING") , sCivilno );
	pDM->AddDBFieldData( _T("HANDPHONE") , _T("STRING") , sHandphone );
	pDM->AddDBFieldData( _T("E_MAIL") , _T("STRING") , sEmail );
	pDM->AddDBFieldData( _T("GROUP_CODE") , _T("STRING") , sGroup );
	pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );

	ids = pDM->MakeUpdateFrame( _T("CO_NONMEMBER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("UpdateProc") );

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("UpdateProc") );

	pDM->EndFrame();


	/// DM�� ����
	INT nInsertIndex = m_nCurrentIndex;

	ids = pDM->SetCellData( _T("�̸�") , sName , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("UpdateProc") );
	ids = pDM->SetCellData( _T("�ֹε�Ϲ�ȣ") , sCivilno , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("UpdateProc") );
	ids = pDM->SetCellData( _T("�ڵ���") , sHandphone , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("UpdateProc") );
	ids = pDM->SetCellData( _T("�̸���") , sEmail , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -27 , _T("UpdateProc") );
	ids = pDM->SetCellData( _T("�׷��ڵ�") , sGroup , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -28 , _T("UpdateProc") );
	ids = pDM->SetCellData( _T("��ȸ��Ű") , sRecKey , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -29 , _T("UpdateProc") );

	/// ȭ�鿡 �����ֱ�
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3870") , _T("��ȸ�����α׸���") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("UpdateProc") );

	pGrid->DisplayLine( nInsertIndex );

	AfxMessageBox(_T("�����Ͽ����ϴ�."));

	return 0;
EFS_END
	return -1;
}

INT CBO_LOC_3873::InitGroupCombo()
{
	EFS_BEGIN

	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3870_GROUP") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitGroupCombo()") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3873"));
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
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3873::InitGroupCombo() := SetData()") );

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

INT CBO_LOC_3873::GroupComboSetting()
{
	EFS_BEGIN

	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3870") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitGroupCombo()") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3873"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitGroupCombo()") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("�׷��ڵ�")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitGroupCombo()") );

	CString sGroup;
	pDM->GetCellData( _T("�׷��ڵ�") , m_nCurrentIndex , sGroup );


	CString sTemp;
	for( i = 0 ; i < pCMCB->GetRowCount() ; i++ )
	{
		pCMCB->GetData(sTemp, i, 0);
		sTemp.TrimLeft(); sTemp.TrimRight();
		if(sGroup.Compare(sTemp)==0){
			pCMCB->SetCurSel(i);
			break;
		}
	}

	return 0;

	EFS_END
	return -1;
}

HBRUSH CBO_LOC_3873::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
