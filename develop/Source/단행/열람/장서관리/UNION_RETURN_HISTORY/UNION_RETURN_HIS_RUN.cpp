// UNION_RETURN_HIS_RUN.cpp : implementation file
//

#include "stdafx.h"
#include "UNION_RETURN_HIS_MAIN.h"
#include "UNION_RETURN_HIS_RUN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_HIS_RUN dialog


CUNION_RETURN_HIS_RUN::CUNION_RETURN_HIS_RUN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pEditReturnDateStart	=	NULL;
	m_pEditReturnDateEnd	=	NULL;
	m_nHistoryMode			=	0;
}

CUNION_RETURN_HIS_RUN::~CUNION_RETURN_HIS_RUN()
{
	if( NULL != m_pEditReturnDateStart )
	{
		delete	m_pEditReturnDateStart;
		m_pEditReturnDateStart	=	NULL;
	}
	if( NULL != m_pEditReturnDateEnd )
	{
		delete	m_pEditReturnDateEnd;
		m_pEditReturnDateEnd	=	NULL;
	}
}

void CUNION_RETURN_HIS_RUN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUNION_RETURN_HIS_RUN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUNION_RETURN_HIS_RUN, CDialog)
	//{{AFX_MSG_MAP(CUNION_RETURN_HIS_RUN)
	ON_BN_CLICKED(IDC_btnHISTORY, OnBtnHistory)
	ON_BN_CLICKED(IDC_btnCLOSE, OnBtnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_HIS_RUN message handlers

BOOL CUNION_RETURN_HIS_RUN::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());

	// SM ����
	if( 0 > InitESL_Mgr(_T("UNION_RETURN_HISTORY_RUN")) )
	{
		AfxMessageBox( _T("InitESL_Mgr ERROR !!") );
		return FALSE;
	}

	// ������ ���� �ʱ�ȭ
	InitLibList();
	
	// �⺻ ��� ����
	CButton* pBtn = (CButton*)GetDlgItem(IDC_radREQUEST_RETURN_HISTORY);
	pBtn->SetCheck(BST_CHECKED);

	// ��¥ ��Ʈ�� ����
	if( NULL == m_pEditReturnDateStart )
	{
		m_pEditReturnDateStart = new CECO_ControlDateEdit(NULL);
		m_pEditReturnDateStart->SubclassDlgItem(IDC_edtDATE_START, this);
		m_pEditReturnDateStart->m_ControlStyle = 0;
		m_pEditReturnDateStart->InitEdit();
	}

	if( NULL == m_pEditReturnDateEnd )
	{
		m_pEditReturnDateEnd = new CECO_ControlDateEdit(NULL);
		m_pEditReturnDateEnd->SubclassDlgItem(IDC_edtDATE_END, this);
		m_pEditReturnDateEnd->m_ControlStyle = 0;
		m_pEditReturnDateEnd->InitEdit();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CUNION_RETURN_HIS_RUN::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{			
			OnBtnHistory();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

/*
@Create
 - 2009.02.24 BY KSJ
@Description
 - �̷� ��ȸ ���� �� ����
@Parameter
 - 
@Return Value
 - 
@Mini Spec
 - �̷� ���� , ������ ���� , ��ȸ �Ⱓ ���� ���� ����
*/
void CUNION_RETURN_HIS_RUN::OnBtnHistory() 
{
	INT		ids;

	// �ݳ��� �Է°� Ȯ��
	CString strReturnDateStart;
	m_pEditReturnDateStart->GetData(strReturnDateStart);
	if( strReturnDateStart.IsEmpty() )
	{
		AfxMessageBox( _T("�ݳ����� �Է��ؾ� �մϴ�.") , MB_ICONINFORMATION );
		return;
	}

	// �̷� ���� - 0 : Ÿ�� �ݳ� �̷� 1 : Ÿ�� �ݳ� ���� �̷�
	CButton* pBtn	=	(CButton*)GetDlgItem(IDC_radREQUEST_RETURN_HISTORY);
	if( NULL == pBtn->GetSafeHwnd() )
	{
		return;
	}
	m_nHistoryMode	=	( BST_CHECKED == pBtn->GetCheck() ) ? 0 : 1;
	
	// ������ ������ ���� ����
	ids = GetControlData( _T("CM_UNION_RETURN_HISTORY_RUN") , _T("������") , m_strLibCode , -1 , 0 );
	if( 0 > ids )
	{
		return;
	}
	
	ids = GetControlData( _T("CM_UNION_RETURN_HISTORY_RUN") , _T("������") , m_strLibName , -1 , 1 );
	if( 0 > ids )
	{
		return;
	}
		
	// ��ȸ �Ⱓ ����
	m_pEditReturnDateStart->GetData(m_strReturnDateStart);
	m_pEditReturnDateEnd->GetData(m_strReturnDateEnd);

	if( m_strReturnDateEnd.IsEmpty() )
	{// �Է��� �� ���� ���, �˻� ���� ��¥���� �˻�
		SYSTEMTIME time;
		GetLocalTime(&time);

		m_strReturnDateEnd.Format( _T("%04d/%02d/%02d") , time.wYear , time.wMonth , time.wDay );
	}

	OnOK();
}
void CUNION_RETURN_HIS_RUN::OnBtnClose() 
{
	OnCancel();
}

/*
@Create
 - 2009.02.25 BY KSJ
@Description
 - ������ ����Ʈ �ʱ�ȭ
@Parameter
 - 
@Return Value
 - 0 : ����
@Mini Spec
 1. ���Ϳ��� ������ ����Ʈ�� �����´�
 2. ComboBox�� ��������ȣ�� �������� Set
*/
INT CUNION_RETURN_HIS_RUN::InitLibList()
{
	INT ids = 0;

	CESL_DataMgr*	pLibraryDM = FindDM( _T("DM_UNION_RETURN_LIBINFO"));
	if( NULL == pLibraryDM )
	{
		return -1;
	}
	
	CESL_ControlMultiComboBox* cmbLibCode;
	cmbLibCode = ((CESL_ControlMultiComboBox*)GetDlgItem(IDC_cmbLIB_LIST));
	if( NULL == cmbLibCode->GetSafeHwnd() )
	{
		return -2;
	}

	CString strLibCode;
	CString strLibName;

	cmbLibCode->AddItem( _T("     "), 0, 0);
	cmbLibCode->AddItem( _T("�������"), 1, 0);

	INT i , nRowCnt;
	nRowCnt		=	pLibraryDM->GetRowCount();
	// 2009.08.17 UPDATE BY KSJ : ���������� 1���� ǥ�� �� �Ǵ� ������ ����
//DEL 	for ( i=1 ; i<nRowCnt ; i++ )
//DEL 	{
//DEL 		pLibraryDM->GetCellData(_T("�������ڵ�"), i, strLibCode );
//DEL 		pLibraryDM->GetCellData(_T("��������"), i, strLibName );
//DEL 		cmbLibCode->AddItem(strLibCode, 0, i);
//DEL 		cmbLibCode->AddItem(strLibName, 1, i);			
//DEL 	}
	for ( i=1 ; i<nRowCnt+1 ; i++ )
	{
		pLibraryDM->GetCellData( _T("�������ڵ�") , i-1, strLibCode );
		pLibraryDM->GetCellData( _T("��������") , i-1 , strLibName );
		cmbLibCode->AddItem( strLibCode , 0 , i );
		cmbLibCode->AddItem( strLibName , 1 , i );			
	}

	cmbLibCode->SetCurSel(0);
	
	return 0;
}
