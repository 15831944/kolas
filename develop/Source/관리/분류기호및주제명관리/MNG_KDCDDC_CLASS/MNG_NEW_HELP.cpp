// MNG_NEW_HELP.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_NEW_HELP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INSERT_MODE 0
#define UPDATE_MODE 1

/////////////////////////////////////////////////////////////////////////////
// CMNG_NEW_HELP dialog

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06 ����� ����

CMNG_NEW_HELP::CMNG_NEW_HELP(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_NEW_HELP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_NEW_HELP::~CMNG_NEW_HELP()
{
	
}


BOOL CMNG_NEW_HELP::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}
VOID CMNG_NEW_HELP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_NEW_HELP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_NEW_HELP, CDialog)
	//{{AFX_MSG_MAP(CMNG_NEW_HELP)
	ON_BN_CLICKED(IDC_CODE_PRE, OnCodePre)
	ON_BN_CLICKED(IDC_CODE_NEXT, OnCodeNext)
	ON_BN_CLICKED(IDC_SAVE_CODE, OnSaveCode)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_NEW_HELP message handlers

BOOL CMNG_NEW_HELP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_�з���ȣ_����ǥ_�Է�")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	SetCode();
	
	if( OpenMode == UPDATE_MODE )
	{

		SelectMakeList(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"));
		m_INDEX = SelectGetHeadPosition(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"));


		if(m_INDEX < 0)
		{
			//����ó��
			ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			EndDialog(0);
			return FALSE;
		}	
	

		GetDlgItem(IDC_strKDCDDC_CODE)->ShowWindow(false);
		GetDlgItem(IDC_KDCDDC_CODE)->ShowWindow(false);

		GetDlgItem(IDC_CODE_PRE)->ShowWindow(true);
		GetDlgItem(IDC_CODE_NEXT)->ShowWindow(true);

		CESL_DataMgr *pDM;
		pDM = FindDM(_T("DM_����_�з���ȣ_����ǥ_����"));
		if ( pDM == NULL )
		{
			AfxMessageBox(_T("DM �ʱ�ȭ ����"));
			return FALSE;
		}
		CString strCode = pDM->GetCellData(_T("����ǥ����"), m_INDEX);
		m_pMCombo->SelectString( strCode );
		
		
		CESL_ControlMgr *pCM;
		pCM = FindCM(_T("CM_����_�з���ȣ_����ǥ_�Է�"));
		if ( pCM == NULL )
		{
			AfxMessageBox(_T("CM �ʱ�ȭ ����"));
			return FALSE;
		}

		SetReverse(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"), m_INDEX);
		pCM->AllControlDisplay(m_INDEX);

	}
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



VOID CMNG_NEW_HELP::OnSaveCode() 
{
EFS_BEGIN
	if( OpenMode == INSERT_MODE)
	{
		if ( INSERT() < 0 ) return;
		AfxMessageBox(_T("�Է� �Ͽ����ϴ�."));
	}
	else
	{
		if( UPDATE() < 0 ) return;
		AfxMessageBox(_T("���� �Ͽ����ϴ�."));
	}
	

	//CDialog::OnOK();
EFS_END
}


INT CMNG_NEW_HELP::INSERT()
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06.07 ����� ����
EFS_BEGIN
	CString strClassType,strHelpCode, strHelpNo, strHelpDESC, strHelpTitle, strRecKey;
	INT ids=-1;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_�з���ȣ_����ǥ_����"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return -1;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_�з���ȣ_����ǥ_�Է�"));
	if ( NULL == pCM ) 
	{
		AfxMessageBox(_T("CM �ʱ�ȭ ����"));
		return -1;
	}

	GetControlData(_T("CM_����_�з���ȣ_����ǥ_�Է�"), _T("�з�����"), strClassType);
	if( strClassType ==_T("KDC"))
		pDM->TBL_NAME = _T("CO_KDC_MEMHELP_TBL");
	else if( strClassType ==_T("DDC"))
		pDM->TBL_NAME = _T("CO_DDC_MEMHELP_TBL");
	else 
		return -1;


	ids = pDM->MakeRecKey(strRecKey);
	if ( ids < 0 ) 
	{
		return -1;
	}

	// get data from CM
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2004.06.20 ����� ����
	strHelpCode.Format(_T("%d"), m_pMCombo->GetCurSel()+1 );
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//GetControlData( _T("CM_����_�з���ȣ_����ǥ_�Է�"), _T("����ǥ����"), strHelpCode );
	GetControlData( _T("CM_����_�з���ȣ_����ǥ_�Է�"), _T("����ǥ��ȣ"), strHelpNo);
	GetControlData( _T("CM_����_�з���ȣ_����ǥ_�Է�"), _T("����ǥ����"), strHelpDESC );
	GetControlData( _T("CM_����_�з���ȣ_����ǥ_�Է�"), _T("����ǥ����"), strHelpTitle );
	
	INT n_index = pDM->GetRowCount();
	
	// set Date To DM
	pDM->InsertRow(-1);
	pDM->SetCellData(_T("REC_KEY"), strRecKey, n_index);
	pDM->SetCellData( _T("�з�����"), strClassType, n_index);
	pDM->SetCellData(_T("����ǥ����"), strHelpCode, n_index);
	pDM->SetCellData(_T("����ǥ��ȣ"), strHelpNo, n_index);
	pDM->SetCellData(_T("����ǥ����"), strHelpDESC, n_index);
	pDM->SetCellData(_T("����ǥ����"), strHelpTitle, n_index);
	// ��������
	CTime t = CTime::GetCurrentTime();
	CString sDate;
	sDate.Format ( _T("%4d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );
	pDM->SetCellData(_T("��������"), sDate, n_index);


	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return -1;
	}
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, n_index, pDM);

	pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return -1;
	}

	pDM->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame �� Ȯ���ϼ���"));
		return -1;
	}

	AllControlDisplay(_T("CM_����_�з���ȣ_����ǥ_����"), n_index);
	
	return 0;
EFS_END
	return -1;
}

INT CMNG_NEW_HELP::UPDATE()
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06.07 ����� ����
EFS_BEGIN
	CString strClassType,strHelpCode, strHelpNo, strHelpDESC, strHelpTitle, strRecKey;
	INT ids = -1;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_�з���ȣ_����ǥ_����"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return -1;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_�з���ȣ_����ǥ_�Է�"));
	if ( NULL == pCM ) 
	{
		AfxMessageBox(_T("CM �ʱ�ȭ ����"));
		return -1;
	}

	GetControlData(_T("CM_����_�з���ȣ_����ǥ_�Է�"), _T("�з�����"), strClassType);
	if( strClassType ==_T("KDC"))
		pDM->TBL_NAME = _T("CO_KDC_MEMHELP_TBL");
	else if( strClassType ==_T("DDC"))
		pDM->TBL_NAME = _T("CO_DDC20_MEMHELP_TBL");
	else 
		return -1;

	// get data from CM
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2004.06.20 ����� ����
	strHelpCode.Format(_T("%d"), m_pMCombo->GetCurSel()+1 );
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//GetControlData( _T("CM_����_�з���ȣ_����ǥ_�Է�"), _T("����ǥ����"), strHelpCode );
	GetControlData( _T("CM_����_�з���ȣ_����ǥ_�Է�"), _T("����ǥ��ȣ"), strHelpNo);
	GetControlData( _T("CM_����_�з���ȣ_����ǥ_�Է�"), _T("����ǥ����"), strHelpDESC );
	GetControlData( _T("CM_����_�з���ȣ_����ǥ_�Է�"), _T("����ǥ����"), strHelpTitle );
	strRecKey = pDM->GetCellData(_T("REC_KEY"), m_INDEX);

	// set Date To DM
	pDM->SetCellData(_T("����ǥ����"), strHelpCode, m_INDEX);
	pDM->SetCellData(_T("����ǥ��ȣ"), strHelpNo, m_INDEX);
	pDM->SetCellData(_T("����ǥ����"), strHelpDESC, m_INDEX);
	pDM->SetCellData(_T("����ǥ����"), strHelpTitle, m_INDEX);
	// ��������
	CTime t = CTime::GetCurrentTime();
	CString sDate;
	sDate.Format ( _T("%4d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );
	pDM->SetCellData(_T("��������"), sDate, m_INDEX);

	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return -1;
	}

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, m_INDEX, pDM, _T("REC_KEY"), _T("NUMERIC"), strRecKey);

	pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return -1;
	}

	pDM->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame �� Ȯ���ϼ���"));
		return -1;
	}

	AllControlDisplay(_T("CM_����_�з���ȣ_����ǥ_����"), m_INDEX);

	return 0;
EFS_END
	return -1;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}



VOID CMNG_NEW_HELP::DELETE_CODE()
{
EFS_BEGIN

	INT n_idx;
	SelectMakeList(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"));
	n_idx = SelectGetHeadPosition(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"));
	if(n_idx < 0)//����ó��
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}	

	if(IDCANCEL == AfxMessageBox(_T("���� �۾��� �Ͻðڽ��ϱ�?"), MB_OKCANCEL)) return;


	CESL_DataMgr *pDM = FindDM(_T("DM_����_�з���ȣ_����ǥ_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_�з���ȣ_����ǥ_����") );
		return ;
	}
	CESL_Grid* p_grid;
	p_grid = (CESL_Grid*)FindControl(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"));
	if ( p_grid == NULL )
	{
		AfxMessageBox(_T("GRID �ʱ�ȭ ����"));
		return ;
	}


	INT nSelectCount = 0;
	nSelectCount = p_grid->SelectGetCount();
	if ( nSelectCount <= 0 ) return ;

	
	INT ids = -1;
	ids = pDM->StartFrame();
	if(ids < 0)
	{
		AfxMessageBox(_T("StartFrame�� Ȯ���ϼ���"));
		return;
	}

	CString strRecKey=_T("");
	for ( INT i=0; i<nSelectCount; i++ )
	{
		strRecKey = pDM->GetCellData(_T("REC_KEY"), n_idx);
		
		// Add Frame
		pDM->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("STRING"), strRecKey);

		ids = p_grid->RemoveAt(n_idx);
		if ( ids<0 ) return;

		ids = pDM->DeleteRow(n_idx);
		if ( ids<0 ) return;

		// ������ ����ŭ �׸��� row ���� ������.
		n_idx = p_grid->SelectGetNext() - (i+1);

	}// end of for
	/*
	strQry.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"), pDM->TBL_NAME ,strRecKey);
	pDM->AddFrame(strQry);
	pDM->DeleteRow(idx);
	delete_Count++;

	idx = SelectGetNext(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strRecKey = pDM->GetCellData( _T("REC_KEY") , idx - delete_Count);

		strQry.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"), pDM->TBL_NAME ,strRecKey);
		pDM->AddFrame(strQry);
		pDM->DeleteRow(idx);
		delete_Count++;

		idx = SelectGetNext(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"));
	}
*/
	ids = pDM->SendFrame();
	if(ids < 0)
	{
		AfxMessageBox(_T("StartFrame�� Ȯ���ϼ���"));
		return;
	}
	ids = pDM->EndFrame();
	if(ids < 0)
	{
		AfxMessageBox(_T("StartFrame�� Ȯ���ϼ���"));
		return;
	}

	AllControlDisplay(_T("CM_����_�з���ȣ_����ǥ_����"));

	return;
EFS_END
}

VOID CMNG_NEW_HELP::OnCodePre() 
{
EFS_BEGIN
	INT idx;
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_�з���ȣ_����ǥ_�Է�"));

	idx = SelectGetPrev(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"));
	if (idx < 0 )
	{
		ESLAfxMessageBox(_T("ó�� �ڷ� �Դϴ�.."));
		SelectGetHeadPosition(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"));
		return;
	}

	m_INDEX = idx ;
	SetReverse(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"), m_INDEX);
	pCM->AllControlDisplay(m_INDEX);	

EFS_END
}

VOID CMNG_NEW_HELP::OnCodeNext() 
{
EFS_BEGIN

	INT idx;
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_�з���ȣ_����ǥ_�Է�"));

	idx = SelectGetNext(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"));
	if (idx < 0 )
	{
		ESLAfxMessageBox(_T("������ �ڷ� �Դϴ�.."));
		SelectGetTailPosition(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"));
		return;
	}

	m_INDEX = idx ;
	SetReverse(_T("CM_����_�з���ȣ_����ǥ_����"), _T("MAIN_GRID"), m_INDEX);
	pCM->AllControlDisplay(m_INDEX);	

EFS_END
}

VOID CMNG_NEW_HELP::SetCode()
{
EFS_BEGIN
	m_pMCombo = new CJComboBox;

	m_pMCombo->SubclassDlgItem( IDC_CLASS_HELP_strCODE, this );
	m_pMCombo->FormatList( 2, DT_LEFT, false , EDGE );
	m_pMCombo->RequireFlag = FALSE;

	m_pMCombo->AddItem( _T("1"), 0, 0 );
	m_pMCombo->AddItem( _T("����ϡ����"), 1, 0 );

	m_pMCombo->AddItem( _T("2"), 0, 1 );
	m_pMCombo->AddItem( _T("��ϡ����"), 1, 1 );

	m_pMCombo->AddItem( _T("3"), 0, 2 );
	m_pMCombo->AddItem( _T("������ϡ����"), 1, 2 );

	m_pMCombo->AddItem( _T("4"), 0, 3 );
	m_pMCombo->AddItem( _T("��������ϡ����"), 1, 3 );

	m_pMCombo->AddItem( _T("5"), 0, 4 );
	m_pMCombo->AddItem( _T("����ϡ����"), 1, 4 );

	m_pMCombo->AddItem( _T("6"), 0, 5 );
	m_pMCombo->AddItem( _T("��������ϡ����"), 1, 5 );

	m_pMCombo->AddItem( _T("7"), 0, 6 );
	m_pMCombo->AddItem( _T("��������ϡ����"), 1, 6 );

	m_pMCombo->AddItem( _T("8"), 0, 7 );
	m_pMCombo->AddItem( _T("��������ϡ����"), 1, 7 );
	
	m_pMCombo->SetCurSel( 0 );

	return ;
EFS_END
return ;
}
HBRUSH CMNG_NEW_HELP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}