// BO_LOC_38101.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_38101.h"
#include "EditBoxDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3810 dialog


CBO_LOC_3810::CBO_LOC_3810(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3810)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CBO_LOC_3810::~CBO_LOC_3810()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CBO_LOC_3810::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3810)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3810, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3810)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3810 message handlers

BOOL CBO_LOC_3810::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3810::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids;

	ids = InitESL_Mgr(_T("SM_BO_LOC_3810"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitESL_Mgr") );

	ids = GetScreenData();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("GetScreenData") );

	ids = ViewScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ViewScreen") );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_3810::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridAUTOMAIL);

	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	CRect GridRect;
	pGrid->GetWindowRect( &GridRect );

	pGrid->MoveWindow(5, 40, cx-8, cy - 60 );

	EFS_END
}

INT CBO_LOC_3810::CreateCondition(INT nIndex)
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3810") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ERROR CBO_LOC_3810::CreateCondition() := FindDM()") );

	CString sHeadDesc;
	CString sTailDesc;
	CString sValue;

	ids = pDM->GetCellData( _T("���Ǿռ���") , nIndex , sHeadDesc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ERROR CBO_LOC_3810::CreateCondition() := GetCellData()") );
	sHeadDesc.TrimLeft(); sHeadDesc.TrimRight();

	ids = pDM->GetCellData( _T("���ǵڼ���") , nIndex , sTailDesc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ERROR CBO_LOC_3810::CreateCondition() := GetCellData()") );
	sTailDesc.TrimLeft(); sTailDesc.TrimRight();

	ids = pDM->GetCellData( _T("���ǰ�") , nIndex , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ERROR CBO_LOC_3810::CreateCondition() := GetCellData()") );
	sValue.TrimLeft(); sValue.TrimRight();

	CString sValueDesc = sHeadDesc + _T(" ") + sValue + sTailDesc ;

	/// �ݳ������� ��� �ֱ⸦ �߰��Ѵ�
	CString sMailClass;
	ids = pDM->GetCellData( _T("��������") , nIndex , sMailClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("ERROR CBO_LOC_3810::CreateCondition() := SetCellData()") );

	if( sMailClass.Compare(_T("11")) == 0 )
	{
		CString sValue2;
		ids = pDM->GetCellData( _T("���ǰ�2") , nIndex , sValue2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ERROR CBO_LOC_3810::CreateCondition() := GetCellData()") );

		sValueDesc += _T(" (�ֱ�:")+sValue2+_T("��)");
	}

	ids = pDM->SetCellData( _T("����") , sValueDesc , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("ERROR CBO_LOC_3810::CreateCondition() := SetCellData()") );

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3810::GetScreenData()
{
	EFS_BEGIN

	CString sDMAlias = _T("DM_BO_LOC_3810");
	CString sCMAlias = _T("CM_BO_LOC_3810");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ERROR CBO_LOC_3810::ViewScreen() := FindDM()") );

	CString sWhere;
	pDM->RefreshDataManager(sWhere);

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3810::ViewScreen()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3810");
	CString sCMAlias = _T("CM_BO_LOC_3810");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ERROR CBO_LOC_3810::ViewScreen() := FindDM()") );

	pDM->SORT( _T("��������") );

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = CreateCondition( i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ERROR CBO_LOC_3810::ViewScreen() := CreateCondition()") );
	}

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ERROR CBO_LOC_3810::ViewScreen() := FindCM()") );

	pCM->AllControlDisplay();

	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridAUTOMAIL);
	pGrid->SetRowHeight(0, 400);

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3810::SaveScreen()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3810");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ERROR CBO_LOC_3810::SaveScreen() := FindDM()") );


	const INT nCnt = 4;

	TCHAR *sDBField[nCnt][3] =
	{
		{ _T("�ϰ��������ɻ�뿩��") , _T("BATCH_PROCESSING_YN") , _T("STRING") }, 
		{ _T("��߼���뿩��") , _T("BATCH_CONDITION_RE_SEND_YN") , _T("STRING") }, 
		{ _T("���ǰ�") , _T("BATCH_CONDITION_VALUE") , _T("STRING") }, 
		{ _T("���ǰ�2") , _T("BATCH_CONDITION_VALUE2") , _T("STRING") }
	};

	CString sValue;
	CString sRecKey;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("��������KEY") , i , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ERROR CBO_LOC_3810::SaveScreen() := GetCellData()") );

		pDM->StartFrame();
		pDM->InitDBFieldData();
		
		for( INT j = 0 ; j < nCnt ; j++ )
		{
			ids = pDM->GetCellData( sDBField[j][0] , i , sValue );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ERROR CBO_LOC_3810::SaveScreen() := GetCellData()") );
			pDM->AddDBFieldData( sDBField[j][1] , sDBField[j][2] , sValue );
		}

		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T(""), __WFILE__, __LINE__ ), TRUE );
		pDM->MakeUpdateFrame( _T("MN_MAIL_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		ids = pDM->SendFrame();
		if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ERROR CBO_LOC_3810::SaveScreen() := SendFrame()") );

		pDM->EndFrame();

	}

	AfxMessageBox(_T("�����Ǿ����ϴ�."));

	return 0;

	EFS_END
	return -1;
}

BEGIN_EVENTSINK_MAP(CBO_LOC_3810, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_3810)
	ON_EVENT(CBO_LOC_3810, IDC_gridAUTOMAIL, -601 /* DblClick */, OnDblClickgridAUTOMAIL, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_3810::OnDblClickgridAUTOMAIL() 
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3810");
	CString sGridAlias = _T("�ڵ�ó����ɰ���");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -3 , _T("ERROR CBO_LOC_3810::OnDblClickgridAUTOMAIL() := FindCM()") );

	CESL_Grid * pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));

	if( pGrid->GetMouseRow() < 1 ) return;

	InputDataToGrid();

	EFS_END	
}

VOID CBO_LOC_3810::InputDataToGrid()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3810");
	CString sGridAlias = _T("�ڵ�ó����ɰ���");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -3 , _T("ERROR CBO_LOC_3810::InputDataToGrid() := FindCM()") );

	CESL_Grid * pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));

	CString sValue = pGrid->GetText();

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	switch( nCol )
	{
		case 2 :		// ��߼� ����
			SetYNCheck( sValue, nRow, nCol );
			break;
		default :
			SetEditBox( nRow );
			break;
	}	

	EFS_END
}

INT CBO_LOC_3810::SetEditBox(INT nRow)
{
	EFS_BEGIN

	INT ids;

	CString sColumnAlias = _T("���ǰ�");

	CString sDMAlias = _T("DM_BO_LOC_3810");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ERROR CBO_LOC_3810::SetEditBox() := FindDM()") );

	CString sValue;
	ids = pDM->GetCellData( sColumnAlias , nRow-1 , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ERROR CBO_LOC_3810::SetEditBox() := GetCellData()") );

	CString sMailClass;
	ids = pDM->GetCellData( _T("��������") , nRow-1 , sMailClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ERROR CBO_LOC_3810::SetEditBox() := GetCellData()") );

	INT nMailClass = _ttoi(sMailClass);
/*
	if( !( nMailClass >= 0 && nMailClass <= 11 ) )
	{
		AfxMessageBox( _T("������ �� ���� �����Դϴ�.") );
		return 1;
	}
*/
	if( nMailClass != 11 )
	{
		CEditBoxDlg EditDlg(this);
		EditDlg.SetValue( sValue );
		EditDlg.DoModal();

		if( EditDlg.IsEdit() == 0 )
			EditDlg.GetValue( sValue );

		ids = pDM->SetCellData( sColumnAlias  , sValue , nRow-1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ERROR CBO_LOC_3810::SetEditBox() := SetCellData()") );
	}
	else
	{
		CString sValue2;
		ids = pDM->GetCellData( _T("���ǰ�2") , nRow-1 , sValue2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ERROR CBO_LOC_3810::SetEditBox() := GetCellData()") );

		CEditBoxDlg2 EditDlg(this);
		EditDlg.SetValue( sValue,sValue2 );
		EditDlg.DoModal();

		if( EditDlg.IsEdit() == 0 )
			EditDlg.GetValue( sValue,sValue2 );

		ids = pDM->SetCellData( sColumnAlias  , sValue , nRow-1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ERROR CBO_LOC_3810::SetEditBox() := SetCellData()") );
		ids = pDM->SetCellData( _T("���ǰ�2")  , sValue2 , nRow-1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("ERROR CBO_LOC_3810::SetEditBox() := SetCellData()") );

	}

	ViewScreen();

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3810::SetYNCheck(CString sValue, INT nRow, INT nCol)
{
	EFS_BEGIN

	if( sValue.Compare(_T("Y"))==0 )
		sValue = _T("N");
	else 
		sValue = _T("Y");
	
	INT ids;

	CString sColumnAlias;
	if( nCol == 1 )
		sColumnAlias = _T("��ɻ�뿩��");
	else if( nCol == 2 )
		sColumnAlias = _T("�ϰ��������ɻ�뿩��");
	else if( nCol == 3 )
		sColumnAlias = _T("��߼���뿩��");

	CString sDMAlias = _T("DM_BO_LOC_3810");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ERROR CBO_LOC_3810::SetYNCheck() := FindDM()") );

	ids = pDM->SetCellData( sColumnAlias , sValue ,  nRow-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ERROR CBO_LOC_3810::SetYNCheck() := SetCellData()") );

	ViewScreen();

	return 0;

	EFS_END
	return -1;
}

HBRUSH CBO_LOC_3810::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
