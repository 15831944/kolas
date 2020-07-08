// MNG_AC_MANAGER_04.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_AC_MANAGER_04.h"

//��������
#include "..\..\..\����\CAT_FIXED_FIELD\Cat008FixedField_AC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_AC_MANAGER_04 dialog


CMNG_AC_MANAGER_04::CMNG_AC_MANAGER_04(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_AC_MANAGER_04)
		// NOTE: the ClassWizard will add member initialization here
	m_pMarc = NULL;
	m_pMCombo = NULL;
	m_pInfoSourceCombo = NULL;
	m_pACTypeCombo = NULL;
	//}}AFX_DATA_INIT
}

CMNG_AC_MANAGER_04::~CMNG_AC_MANAGER_04()
{
EFS_BEGIN

	if(m_pMarc != NULL)
		delete m_pMarc;
	if(m_pMCombo != NULL)
		delete m_pMCombo;
	if(m_pInfoSourceCombo != NULL)
		delete m_pInfoSourceCombo;	
	if( m_pACTypeCombo )
		delete m_pACTypeCombo;


	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
EFS_END
}

BOOL CMNG_AC_MANAGER_04::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CMNG_AC_MANAGER_04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_AC_MANAGER_04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_AC_MANAGER_04, CDialog)
	//{{AFX_MSG_MAP(CMNG_AC_MANAGER_04)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_CBN_SELCHANGE(IDC_cAcCode, OnSelchangecAcCode)
	ON_CBN_SELCHANGE(IDC_cInfoSource, OnSelchangecInfoSource)
	ON_BN_CLICKED(IDC_bEDIT_LENGTH, OnbEDITLENGTH)
	ON_BN_CLICKED(IDC_bCLEAR_SCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_bADD, OnbADD)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_BN_CLICKED(IDC_bDelete, OnbDelete)
	ON_CBN_SELCHANGE(IDC_cACKind, OnSelchangecACKind)
	ON_EN_CHANGE(IDC_euSelItem, OnChangeeuSelItem)
	ON_EN_CHANGE(IDC_eSummery, OnChangeeSummery)
	ON_EN_CHANGE(IDC_eYear, OnChangeeYear)
	ON_EN_CHANGE(IDC_eInfoSource, OnChangeeInfoSource)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_AC_MANAGER_04 message handlers

BOOL CMNG_AC_MANAGER_04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("MNG_AC_MANAGER_03") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_AC_MANAGER_03") );
		return false;
	}
	
	//������ ���� ������ ��쿡�� Enable
	if ( m_ACMODE == AUTHOR_MODE )
		GetDlgItem(IDC_eYear)->EnableWindow(TRUE);
	
	//����ǥ�� �⺻�� ����
	//GetDlgItem(IDC_euSelItem)->SetWindowText(m_sRefItem);
	
	//���� ���� ��尡 �����Ǿ� ���� ������ ����
	if ( m_ACMODE < 0 ) return FALSE;
	
	if ( m_sModifyRecKey == _T("") )
	{
		AfxMessageBox(_T("������ �����Ͱ� �����ϴ�"));
		return FALSE;
	}

	//CM, DM Alias ����
	if ( m_pCM ) m_pCM = NULL;
	m_pCM = FindCM ( _T("CM_MNG_AC_MANAGER_03") );
	if ( m_pCM == NULL ) return FALSE;

	if ( m_pDM ) m_pDM = NULL;
	m_pDM = FindDM ( _T("DM_MNG_AC_MANAGER_MASTER") );
	if ( m_pDM == NULL ) return FALSE;
	
	//Marc Editor ����
	if (!m_marcEditor.SubclassDlgItem(IDC_MarcRichEdit, -1, this))
	{
		AfxMessageBox(_T("Subclass Marc Editor Failed"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//��ũ �� �ʱ�ȭ
	if (m_pMarc)
		m_pMarc = NULL;
	m_pMarc = new CMarc;
	
	CString sStreamMarc;
	CString sQuery;

	sQuery.Format( _T("SELECT MARC FROM AC_MASTER_MAT_TBL where REC_KEY = %s"), m_sModifyRecKey );
	
	m_pDM->GetOneValueQuery( sQuery, sStreamMarc );

	m_pInfo->pMarcMgr->Decoding(sStreamMarc, m_pMarc);
		
	m_pInfo->pMarcMgr->SetMarcGroup(_T("����"));
	
	m_marcEditor.SetMarcMgr(m_pInfo->pMarcMgr);
	m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
	
	
	//���� ǥ�� ��ȣ ����	
	SetAcCombo();
	
	//������ �޺��ڽ� ���� 
	SetInfoSourceCombo();
	

	//Master DM ����
	if ( !SetMasterDmForModify() ) return FALSE;
	
	//m_iSetMode = -1;
	m_pDM->FreeData();
	
	//���� �׸��� ����
	m_pGrid = static_cast<CESL_Grid*>(FindControl(_T("CM_MNG_AC_MANAGER_03"), _T("RefGrid")));
	if (m_pGrid == NULL) return FALSE;
	
	//�̷����� ����
	SetDataInfo();

	//���� ȭ�� ��ũ ���ڵ�
	MarcDecodingToDM();

	//ȭ�� Display
	Display();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_AC_MANAGER_04::Display()
{
	EFS_BEGIN
		
	m_marcEditor.Display();
	
	CString strText;
	if ( m_ACMODE == AUTHOR_MODE )
		strText = _T("���� ���� ���� ����");
	else if ( m_ACMODE == TITLE_MODE )
		strText = _T("���� ���� ���� ����");
	else if ( m_ACMODE == SUBJECT_MODE )
		strText = _T("������ ���� ���� ����");
	
	strText = _T("���� ���� ���� ����");
	SetWindowText(strText);
	
	
	// ����ǥ��, �׸��� Display
	GetDlgItem(IDC_eRefItem)->SetWindowText(_T(""));
	m_pCM->AllControlDisplay();
	
EFS_END
}


BOOL CMNG_AC_MANAGER_04::InsertACTBL(CString sACMasterKey)
{
EFS_BEGIN
		
	INT ids = -1;
	CESL_DataMgr *pDM = NULL;
	CString strTBLNAME;
	if ( m_ACMODE == AUTHOR_MODE )
	{
		pDM = FindDM( _T("DM_MNG_AC_MANAGER_AUTHOR") );
		strTBLNAME = _T("AC_INDEX_AUTHOR_TBL") ;
	}
	else if ( m_ACMODE == TITLE_MODE )
	{
		pDM = FindDM( _T("DM_MNG_AC_MANAGER_TITLE") );
		strTBLNAME = _T("AC_INDEX_TITLE_TBL") ;
	}
	else if ( m_ACMODE == SUBJECT_MODE )
	{
		pDM = FindDM( _T("DM_MNG_AC_MANAGER_SUBJECT") );
		strTBLNAME = _T("AC_INDEX_SUBJECT_TBL") ;
	}
	
	if ( pDM == NULL ) return FALSE;
	
	ids = pDM->StartFrame();
	if ( ids < 0 ) return FALSE;
	
	// ���� ���̺� �Է�
	CString sACReckey;
	m_pDM->MakeRecKey(sACReckey);

	CString strDeleteQuery;
	strDeleteQuery.Format( _T("DELETE %s WHERE AC_MASTER_MAT_KEY = '%s';"), strTBLNAME, sACMasterKey );
	pDM->AddFrame( strDeleteQuery );

	pDM->InsertRow(-1);
	pDM->SetCellData(_T("REC_KEY"), sACReckey , 0);
	pDM->SetCellData(_T("AC_MASTER_MAT_KEY"), sACMasterKey , 0);
	
	CString sKeyWordTmp = _T("");
	GetDlgItem(IDC_euSelItem)->GetWindowText(sKeyWordTmp);
	
	pDM->SetCellData(_T("KEYWORD"), sKeyWordTmp , 0);
	
	//ä�� ǥ���� ���� ��� Y, ���� ��� N�� �Է�
	if ( sKeyWordTmp == _T(""))
		pDM->SetCellData(_T("SELECT_YN"), _T("N") , 0);
	else 
		pDM->SetCellData(_T("SELECT_YN"), _T("Y") , 0);
	
	pDM->SetCellData(_T("FIRST_WORK"), GetWorkLogMsg(_T("�����Է�"),WORK_LOG) , 0);
	pDM->SetCellData(_T("LAST_WORK"), GetWorkLogMsg(_T("�����Է�"),WORK_LOG) , 0);
	
	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	
	ids = pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, 0, pDM);
	if ( ids < 0) return FALSE;
	
	ids = pDM->SendFrame();
	if ( ids < 0) return FALSE;
	
	ids = pDM->EndFrame();
	if ( ids < 0 ) return FALSE;
	
	return true;
	
EFS_END
return false;
}
BOOL CMNG_AC_MANAGER_04::InsertACMASTERTBL(CString sACMasterKey)
{
EFS_BEGIN
	
	INT ids = -1;
	
	ids = m_pDM->StartFrame();
	if ( ids < 0 ) return FALSE;
	
	CTime t = CTime::GetCurrentTime();
	CString strMsg;
	strMsg.Format(_T("%04d%02d%02d %02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute());
	
	
	// ���� MASTER�� �Է� �ϴ� �۾�
	CString sStreamMarc;
	m_pInfo->pMarcMgr->Encoding(m_pMarc, sStreamMarc );
	
	//m_pDM->InsertRow(-1);
	m_pDM->InitDBFieldData();
	m_pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), sACMasterKey);
	//m_pDM->SetCellData(_T("���������ȣ"), sMasterReckey , 0);
	m_pDM->AddDBFieldData(_T("MARC"), _T("STRING"), sStreamMarc);
	//m_pDM->SetCellData(_T("��������"), sMasterReckey , 0);
	//m_pDM->SetCellData(_T("����Ŭ����"), sMasterReckey , 0);
	//m_pDM->SetCellData(_T("����������"), sMasterReckey , 0);
	
	m_pDM->AddDBFieldData(_T("INPUT_DATE"), _T("STRING"), strMsg , 0);
	m_pDM->AddDBFieldData(_T("UPDATE_DATE"), _T("STRING"), strMsg , 0);
	
//	m_pDM->AddDBFieldData(_T("�Է���"), _T("STRING"). m_pInfo->USER_ID);
//	m_pDM->AddDBFieldData(_T("������"), _T("STRING"), m_pInfo->USER_ID);
	
	m_pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�����Է�"),WORK_LOG));
	m_pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�����Է�"),WORK_LOG));
	
	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	ids = m_pDM->MakeUpdateFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), sACMasterKey );

	//ids = m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM);
	if ( ids < 0) return FALSE;
	
	ids = m_pDM->SendFrame();
	if ( ids < 0) return FALSE;
	
	ids = m_pDM->EndFrame();
	if ( ids < 0 ) return FALSE;
	
	return true;
	
EFS_END
	return false;
}
///////////////////////////////////////////////////////////////////////
// ���� ����
BOOL CMNG_AC_MANAGER_04::InsertToDB()
{
EFS_BEGIN
		INT ids = -1;
	
	CString sACMasterReckey;
	
	if ( !InsertACMASTERTBL(m_sModifyRecKey) ) return FALSE;
	
	if ( !InsertACTBL(m_sModifyRecKey) ) return FALSE;
	
	// MASTER TBL�� ���� �����ؼ� ����
	// ���� : ���� ���̺�(����, ����, ������)�� ���������� �Է��� �Ǿ��� ��� MASTER TABLE�� �Է��Ѵ�.
	
	
	return TRUE;
	
EFS_END
return FALSE;
}
///////////////////////////////////////////////////////////////////////
// ������ �޺��ڽ� ����
VOID CMNG_AC_MANAGER_04::SetInfoSourceCombo()
{
EFS_BEGIN
	if ( m_pInfoSourceCombo ) m_pInfoSourceCombo = NULL;
	
	m_pInfoSourceCombo = new CJComboBox;
	m_pInfoSourceCombo->SubclassDlgItem( IDC_cInfoSource, this );
	m_pInfoSourceCombo->FormatList( 2, DT_LEFT, false , EDGE );
	m_pInfoSourceCombo->RequireFlag = FALSE;
	
	m_pInfoSourceCombo->AddItem( _T("0"), 0, 0 );
	m_pInfoSourceCombo->AddItem( _T("�������"), 1, 0 );
	
	m_pInfoSourceCombo->AddItem( _T("1"), 0, 1 );
	m_pInfoSourceCombo->AddItem( _T("�������"), 1, 1 );
	
	m_pInfoSourceCombo->AddItem( _T("2"), 0, 2 );
	m_pInfoSourceCombo->AddItem( _T("�������"), 1, 2 );
	
	m_pInfoSourceCombo->AddItem( _T("3"), 0, 3 );
	m_pInfoSourceCombo->AddItem( _T("����"), 1, 3 );
	
	m_pInfoSourceCombo->SetCurSel( 0 );
	GetDlgItem(IDC_eInfoSource)->SetWindowText(_T(""));
	
EFS_END
}
///////////////////////////////////////////////////////////////////////
// ���� ǥ���ȣ �޺� �ڽ� ����
VOID CMNG_AC_MANAGER_04::SetAcCombo()
{
EFS_BEGIN
		
	m_pMCombo = new CJComboBox;
	m_pMCombo->SubclassDlgItem( IDC_cAcCode, this );
	m_pMCombo->FormatList( 2, DT_LEFT, false , EDGE );
	m_pMCombo->RequireFlag = FALSE;

	if ( m_ACMODE < 0 ) return;

	INT defaultCel = 0;
	CString code, desc;

	if ( m_ACMODE == AUTHOR_MODE )
	{
		m_pMCombo->AddItem( _T("100"), 0, 0 );
		m_pMCombo->AddItem( _T("���θ�"), 1, 0 );
		
		m_pMCombo->AddItem( _T("110"), 0, 1 );
		m_pMCombo->AddItem( _T("��ü��"), 1, 1 );
		
		m_pMCombo->AddItem( _T("111"), 0, 2 );
		m_pMCombo->AddItem( _T("�׷��"), 1, 2 );
		
		m_pMCombo->SetCurSel( 0 );
	}
	else if ( m_ACMODE == TITLE_MODE )
	{
		m_pMCombo->AddItem( _T("130"), 0, 0 );
		m_pMCombo->AddItem( _T("����"), 1, 0 );	
		m_pMCombo->SetCurSel( 0 );
	}		
	else if ( m_ACMODE == SUBJECT_MODE )
	{
		m_pMCombo->AddItem( _T("150"), 0, 0 );
		m_pMCombo->AddItem( _T("������"), 1, 0 );
		m_pMCombo->SetCurSel( 0 );
	}
	else 
	{
		GetDlgItem(IDC_cACKind)->EnableWindow(TRUE);
		
		//if ( m_pACTypeCombo != NULL ) delete m_pACTypeCombo;
		
		m_pACTypeCombo = new CJComboBox;
		m_pACTypeCombo->SubclassDlgItem( IDC_cACKind, this );
		m_pACTypeCombo->FormatList( 1, DT_LEFT, false , EDGE );
		m_pACTypeCombo->RequireFlag = FALSE;
		
		INT defaultCel = 0;
		CString code, desc;
		
		m_pACTypeCombo->AddItem( _T("���ڸ�"), 0, 0 );
		m_pACTypeCombo->AddItem( _T("����"), 0, 1 );
		m_pACTypeCombo->AddItem( _T("������"), 0, 2 );
		
		m_pACTypeCombo->SetCurSel( 0 );
		
	}

EFS_END
}

///////////////////////////////////////////////////////////////////////
// ���� ��ũ ����
VOID CMNG_AC_MANAGER_04::DeleteACMarcField()
{
EFS_BEGIN
	
	if ( m_ACMODE < 0 ) return;
	
	if ( m_ACMODE == AUTHOR_MODE )
	{
		// ���ڸ� - ä�� ǥ��
		// 1xx - 100, 110, 111
		// 4xx - 400, 410, 411
		// 5xx - 500, 510, 511
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("100"));
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("110"));
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("111"));
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("400"));
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("410"));
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("411"));
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("500"));
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("510"));
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("511"));
	}
	else if ( m_ACMODE ==  TITLE_MODE )
	{
		// ���� - ä�� ǥ��
		// 1xx - 130
		// 4xx - 430
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("130"));
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("430"));
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("530"));
	}
	else if ( m_ACMODE == SUBJECT_MODE )
	{
		// ������ - ä�� ǥ��
		// 1xx - 150
		// 4xx - 450
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("150"));
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("450"));
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("550"));
	}
	
	return;
	
	EFS_END
}
BOOL CMNG_AC_MANAGER_04::IsCheckSaveInputValue()
{
EFS_BEGIN
		
	//008 �ִ��� Check
	CString sTmp008Field = _T("");
	m_pInfo->pMarcMgr->GetField(m_pMarc, _T("008"), sTmp008Field);
	if ( sTmp008Field == _T("") )
	{
		AfxMessageBox( _T("008 Tag�� �Է��� �ּ���") );
		return FALSE;
	}
	
	return TRUE;
	
EFS_END
return FALSE;
}
VOID CMNG_AC_MANAGER_04::SetACMarc(INT iACCode)
{
EFS_BEGIN
		
	if ( iACCode < 0 ) return;

	//0 - ���θ�
	//1 - ��ü��
	//2 - �׷��
	CString sRefItem;

	//������ �ο��� 400, 410, 411 Marc ����
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("400"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("410"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("411"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("430"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("450"));

	//DM���� ���� ������ ����
	for ( INT i = 0; i < m_pDM->GetRowCount(); i++ )
	{
		
		m_pDM->GetCellData(_T("����ǥ��"), i , sRefItem );
		
		if ( iACCode == 0)
		{
			if ( m_ACMODE == AUTHOR_MODE )	m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("400$a"), sRefItem );
			else if ( m_ACMODE == TITLE_MODE ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("430$a"), sRefItem );
			else if ( m_ACMODE == SUBJECT_MODE ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("450$a"), sRefItem );
		}
		else if ( iACCode == 1 )
		{
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("410$a"), sRefItem );
		}
		else if ( iACCode == 2 )
		{
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("411$a"), sRefItem );
		}
	}


	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("100"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("110"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("111"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("130"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("150"));

	sRefItem = _T("");
	CString sYear = _T("");

	GetDlgItem(IDC_euSelItem)->GetWindowText(sRefItem);
	
	if ( iACCode == 0)
	{
		if ( m_ACMODE == AUTHOR_MODE )
		{
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("100$a"), sRefItem );
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("100$d"), sYear );
		}
		else if ( m_ACMODE == TITLE_MODE ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("130$a"), sRefItem );
		else if ( m_ACMODE == SUBJECT_MODE ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("150$a"), sRefItem );
	}
	else if ( iACCode == 1 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("110$a"), sRefItem );
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("110$d"), sYear );
	}
	else if ( iACCode == 2 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("111$a"), sRefItem );
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("111$d"), sYear );
	}

	return;

	
EFS_END
}


BOOL CMNG_AC_MANAGER_04::UpdateRefItem(CString sRefItem)
{
EFS_BEGIN
		
	CString sTmpRefItem = _T("");
	GetDlgItem(IDC_eRefItem)->GetWindowText(sTmpRefItem);
	if ( sTmpRefItem == _T("") )
	{
		AfxMessageBox(_T("���� ǥ�� �׸��� ������ �ּ���"));
		return FALSE;
	}
	
	//�׸��忡 ����
	long nRowLocation;
	nRowLocation = m_pGrid->GetRow()-1;
	
	//m_pGrid->SetAt( nRowLocation, 0, sRefItem );
	m_pDM->SetCellData(_T("����ǥ��"), sRefItem, nRowLocation);
	
	return TRUE;
	
EFS_END
return FALSE;
}


BOOL CMNG_AC_MANAGER_04::InsertRefItem(CString sRefItem)
{
EFS_BEGIN
		
	//����ǥ�� Grid�� �Է�
	INT ids = m_pDM->GetRowCount();
	m_pDM->SetCellData(_T("����ǥ��"), sRefItem, -1 );
	
	//����ǥ�� Marc�� �Է�
	INT iSelInfoSource = m_pMCombo->GetCurSel();
	
	//Marc�� �ݿ�
	SetACMarc ( iSelInfoSource );
	
	return TRUE;
	
EFS_END
return FALSE;
}

CString CMNG_AC_MANAGER_04::ISCheckInputValue()
{
EFS_BEGIN
		
	CString sTmpItem = _T("");
	GetDlgItem(IDC_eRefItem)->GetWindowText(sTmpItem);
	
	if ( sTmpItem == _T("") )
	{
		AfxMessageBox(_T("����ǥ���� �Է��� �ּ���"));
		return _T("");
	}
	return sTmpItem;
	
EFS_END
return _T("");
}

VOID CMNG_AC_MANAGER_04::SetDataInfo()
{
EFS_BEGIN
	//�̷����� ����
	
	//�����۾���
	//GetDlgItem(IDC_eFirstUser)->SetWindowText();

	//�����Է���
	//GetDlgItem(IDC_eFirstDay)->SetWindowText();

	//������������
	//GetDlgItem(IDC_eLastUser)->SetWindowText();

	//������������
	//GetDlgItem(IDC_eLastDay)->SetWindowText();


	return;
EFS_END
}

BOOL CMNG_AC_MANAGER_04::SetMasterDmForModify()
{
EFS_BEGIN

	if ( m_pDM == NULL ) return FALSE;

	CString strQuery;
	strQuery.Format(_T(""));

	return TRUE;
EFS_END
return FALSE;
}

VOID CMNG_AC_MANAGER_04::OnbCLOSE() 
{
EFS_BEGIN
	
	CDialog::OnCancel();
	
EFS_END	
}

VOID CMNG_AC_MANAGER_04::OnOK() 
{
EFS_BEGIN
		
	if ( !IsCheckSaveInputValue() ) return;
	
	if ( !InsertToDB() ) 
	{
		AfxMessageBox(_T("DB�� ������ ������ �߻��Ͽ����ϴ�"));
		return;
	}
	
	AfxMessageBox( _T("�����Ͽ����ϴ�.") );
	
EFS_END
}

VOID CMNG_AC_MANAGER_04::OnSelchangecAcCode() 
{
EFS_BEGIN
		
	//����ǥ�� Marc�� �Է�
	INT iSelInfoSource = m_pMCombo->GetCurSel();
	
	//Marc�� �ݿ�
	SetACMarc ( iSelInfoSource );
	
	Display();
	
EFS_END		
	
}

VOID CMNG_AC_MANAGER_04::OnSelchangecInfoSource() 
{
EFS_BEGIN
		
	//������ ���� ����
	SetInfoSource();
	
EFS_END	
}

VOID CMNG_AC_MANAGER_04::OnbEDITLENGTH() 
{
EFS_BEGIN
		
	// ��ũ ����
	CCat008FixedField_AC dlg(this, m_pMarc);
	
	if ( dlg.DoModal() == IDOK ) 
	{
		// Check m_pMarc
		if (m_pMarc == NULL)
		{
			AfxMessageBox(_T("pMarc Is NULL"));
			EndDialog(IDCANCEL);
			return;
		}
		Display();
	}
	
EFS_END	
}

VOID CMNG_AC_MANAGER_04::OnbCLEARSCREEN() 
{
EFS_BEGIN
		
	m_pDM->FreeData();
	
	//����ǥ�� Marc�� �Է�
	INT iSelInfoSource = m_pMCombo->GetCurSel();
	
	//����ǥ�� ��ȣ �ʱ�ȭ
	m_pMCombo->SetCurSel(0);
	
	//������
	m_pInfoSourceCombo->SetCurSel(0);
	
	//008 Tag����
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("008"));
	
	//������ ���� �ʱ�ȭ
	SetInfoSource();
	
	//Marc�� �ݿ�
	SetACMarc ( iSelInfoSource );
	
	Display();
EFS_END		
}

VOID CMNG_AC_MANAGER_04::OnbADD() 
{
EFS_BEGIN
		
		//����ǥ�� �߰�
		
		//�Է��� �� ��ȿ�� �˻�
	CString sRefItem = ISCheckInputValue();
	if ( sRefItem == _T("") ) return;
	
	//m_iSetMode = ADD;
	//�Էµ� ���� ǥ�� Grid�� Marc�� �ݿ�
	InsertRefItem(sRefItem);
	
	Display();
	
EFS_END	
}

VOID CMNG_AC_MANAGER_04::OnbMODIFY() 
{
EFS_BEGIN
		
	// ����
	CString sRefItem = ISCheckInputValue();
	if ( sRefItem == _T("") ) return;
	
	//m_iSetMode = MODIFY;
	UpdateRefItem(sRefItem);
	
	
	Display();
	
EFS_END	
	
}

VOID CMNG_AC_MANAGER_04::OnbDelete() 
{
EFS_BEGIN
		
	long nRowLocation;
	nRowLocation = m_pGrid->GetRow()-1;
	m_pDM->DeleteRow(nRowLocation);
	
	//����ǥ�� Marc�� �Է�
	INT iSelInfoSource = m_pMCombo->GetCurSel();
	
	//Marc�� �ݿ�
	SetACMarc ( iSelInfoSource );
	
	Display();
	
EFS_END	
}

VOID CMNG_AC_MANAGER_04::SetInfoSource()
{
EFS_BEGIN
	
	//������ ���� ����
	INT iSelInfoSource = m_pInfoSourceCombo->GetCurSel();

	//������ 670 Tag ����
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("670"));
	
	// 0 - �������
	// 1 - �������
	// 2 - �������
	// 3 - ����
	GetDlgItem(IDC_eInfoSource)->SetWindowText(_T(""));
	if ( iSelInfoSource == 0 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), _T(""));	
		GetDlgItem(IDC_eInfoSource)->EnableWindow(TRUE);
	}
	else if ( iSelInfoSource == 1 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), _T("�������"));	
		GetDlgItem(IDC_eInfoSource)->EnableWindow(FALSE);
	}
	else if ( iSelInfoSource == 2 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), _T("�������"));	
		GetDlgItem(IDC_eInfoSource)->EnableWindow(FALSE);
	}
	else if ( iSelInfoSource == 3 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), _T(""));	
		GetDlgItem(IDC_eInfoSource)->EnableWindow(FALSE);
	}
		
	//���ױ� ����
	m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
	m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);
	
	Display();
	
EFS_END	
}



VOID CMNG_AC_MANAGER_04::OnSelchangecACKind() 
{
EFS_BEGIN
	
	INT iSelInfoSource; 
	iSelInfoSource = m_pACTypeCombo->GetCurSel();

	m_pMCombo->ResetContent();

	m_pMCombo->FormatList( 2, DT_LEFT, false , EDGE );
	m_pMCombo->RequireFlag = FALSE;

	switch( iSelInfoSource )
	{
	case 0 :
		m_ACMODE = AUTHOR_MODE;
		
		m_pMCombo->AddItem( _T("100"), 0, 0 );
		m_pMCombo->AddItem( _T("���θ�"), 1, 0 );
		
		m_pMCombo->AddItem( _T("110"), 0, 1 );
		m_pMCombo->AddItem( _T("��ü��"), 1, 1 );
		
		m_pMCombo->AddItem( _T("111"), 0, 2 );
		m_pMCombo->AddItem( _T("�׷��"), 1, 2 );
		
		m_pMCombo->SetCurSel( 0 );
		
		break;
	case 1 :
		m_ACMODE = TITLE_MODE;
		
		m_pMCombo->AddItem( _T("130"), 0, 0 );
		m_pMCombo->AddItem( _T("����"), 1, 0 );	
		m_pMCombo->SetCurSel( 0 );
		
		break;
	case 2 :
		m_ACMODE = SUBJECT_MODE;
		
		m_pMCombo->AddItem( _T("150"), 0, 0 );
		m_pMCombo->AddItem( _T("������"), 1, 0 );
		m_pMCombo->SetCurSel( 0 );
		
		break;
	default :
		return;
		break;
	}




EFS_END	
}

VOID CMNG_AC_MANAGER_04::MarcDecodingToDM()
{
EFS_BEGIN
	// Marc ���ڵ�
	if ( m_pMarc == NULL ) return;

	CString sTmpItem = _T("");

	//1XX ����
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("100$a"), sTmpItem );
	if ( sTmpItem != _T("")) GetDlgItem(IDC_euSelItem)->SetWindowText(sTmpItem);

	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("110$a"), sTmpItem );
	if ( sTmpItem != _T("")) GetDlgItem(IDC_euSelItem)->SetWindowText(sTmpItem);

	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("111$a"), sTmpItem );
	if ( sTmpItem != _T("")) GetDlgItem(IDC_euSelItem)->SetWindowText(sTmpItem);

	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("130$a"), sTmpItem );
	if ( sTmpItem != _T("")) GetDlgItem(IDC_euSelItem)->SetWindowText(sTmpItem);

	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("150$a"), sTmpItem );
	if ( sTmpItem != _T("")) GetDlgItem(IDC_euSelItem)->SetWindowText(sTmpItem);


	//4XX ����
	CString sTmpField;
	CArray<CString, CString&> pArray4XX;
	CString strData;
	CString sIndL;
	sIndL.Format( _T("%c"), 30 );

	//400
	pArray4XX.RemoveAll();
	m_pInfo->pMarcMgr->GetField(m_pMarc, _T("400") , sTmpField, &pArray4XX);
	INT nCount = pArray4XX.GetSize();
	INT i;
	for (i = 0; i < nCount; i++)
	{
		strData = pArray4XX.GetAt(i);
		strData = strData.Mid(4);
		INT iPos = strData.Find(sIndL);
		strData = strData.Left(iPos);
		m_pDM->SetCellData(_T("����ǥ��"), strData, -1 );
	}

	//410
	pArray4XX.RemoveAll();
	m_pInfo->pMarcMgr->GetField(m_pMarc, _T("410") , sTmpField, &pArray4XX);
	nCount = pArray4XX.GetSize();
	for (i = 0; i < nCount; i++)
	{
		strData = pArray4XX.GetAt(i);
		strData = strData.Mid(4);
		INT iPos = strData.Find(sIndL);
		strData = strData.Left(iPos);
		m_pDM->SetCellData(_T("����ǥ��"), strData, -1 );
	}

	//411
	pArray4XX.RemoveAll();
	m_pInfo->pMarcMgr->GetField(m_pMarc, _T("411") , sTmpField, &pArray4XX);
	nCount = pArray4XX.GetSize();
	for (i = 0; i < nCount; i++)
	{
		strData = pArray4XX.GetAt(i);
		strData = strData.Mid(4);
		INT iPos = strData.Find(sIndL);
		strData = strData.Left(iPos);
		m_pDM->SetCellData(_T("����ǥ��"), strData, -1 );
	}

	//430
	pArray4XX.RemoveAll();
	m_pInfo->pMarcMgr->GetField(m_pMarc, _T("430") , sTmpField, &pArray4XX);
	nCount = pArray4XX.GetSize();
	for (i = 0; i < nCount; i++)
	{
		strData = pArray4XX.GetAt(i);
		strData = strData.Mid(4);
		INT iPos = strData.Find(sIndL);
		strData = strData.Left(iPos);
		m_pDM->SetCellData(_T("����ǥ��"), strData, -1 );
	}

	//450
	pArray4XX.RemoveAll();
	m_pInfo->pMarcMgr->GetField(m_pMarc, _T("450") , sTmpField, &pArray4XX);
	nCount = pArray4XX.GetSize();
	for (i = 0; i < nCount; i++)
	{
		strData = pArray4XX.GetAt(i);
		strData = strData.Mid(4);
		INT iPos = strData.Find(sIndL);
		strData = strData.Left(iPos);
		m_pDM->SetCellData(_T("����ǥ��"), strData, -1 );
	}

	// 678 ����ױ� ����
	sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("678$a"), sTmpItem );
	if ( sTmpItem != _T("")) GetDlgItem(IDC_eSummery)->SetWindowText(sTmpItem);

	Display();

	return;

EFS_END
}

VOID CMNG_AC_MANAGER_04::OnChangeeuSelItem() 
{
EFS_BEGIN
		
	CString sSelItem = _T("");
	GetDlgItem(IDC_euSelItem)->GetWindowText(sSelItem);
	
	INT iSelInfoSource = -1;
	if ( sSelItem != _T("") )
	{
		m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("100$a"));
		m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("110$a"));
		m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("111$a"));
		m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("130$a"));
		m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("150$a"));
		
		//������ ���� ����
		iSelInfoSource = m_pMCombo->GetCurSel();
	}
	
	
	
	if ( m_ACMODE == AUTHOR_MODE )
	{
		if ( iSelInfoSource == 0 ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("100$a"), sSelItem);	
		if ( iSelInfoSource == 1 ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("110$a"), sSelItem);	
		if ( iSelInfoSource == 2 ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("111$a"), sSelItem);	
	}
	else if ( m_ACMODE == TITLE_MODE )
	{
		if ( iSelInfoSource == 0 ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("130$a"), sSelItem);	
	}
	else if ( m_ACMODE == SUBJECT_MODE )
	{
		if ( iSelInfoSource == 0 ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("150$a"), sSelItem);	
	}
	
	if ( sSelItem != _T("") )
	{
		//���ױ� ����
		m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
		m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);
	}
	
	Display();
	
	EFS_END	
}

VOID CMNG_AC_MANAGER_04::OnChangeeSummery() 
{
EFS_BEGIN
	
	CString sSummeryItem = _T("");
	GetDlgItem(IDC_eSummery)->GetWindowText(sSummeryItem);
	
	//m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("678"));
	CString sTmpItem = _T("") ;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("678$a"), sTmpItem );
	if ( sTmpItem == _T("") )
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("678$a"), sSummeryItem);
	else 
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("678$a"), sSummeryItem, _T(""), MODIFY_ITEM);
	
	//���ױ� ����
	m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
	m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);
	
	Display();
	
EFS_END		
}

VOID CMNG_AC_MANAGER_04::OnChangeeYear() 
{
EFS_BEGIN

	CString sYearItem = _T("");
	GetDlgItem(IDC_eYear)->GetWindowText(sYearItem);

	CString sTmpItem = _T("") ;
	
	if( m_ACMODE == AUTHOR_MODE )
	{
		INT nCurSel = m_pACTypeCombo->GetCurSel();
		if(nCurSel == 0)
		{
			m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("100$d"), sTmpItem );
			if ( sTmpItem == _T("") )
				m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("100$d"), sYearItem);
			else 
				m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("100$d"), sYearItem, _T(""), MODIFY_ITEM);
		}
		else if(nCurSel == 1)
		{
			m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("110$d"), sTmpItem );
			if ( sTmpItem == _T("") )
				m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("110$d"), sYearItem);
			else 
				m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("110$d"), sYearItem, _T(""), MODIFY_ITEM);

		}
		else
		{
			m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("111$d"), sTmpItem );
			if ( sTmpItem == _T("") )
				m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("111$d"), sYearItem);
			else 
				m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("111$d"), sYearItem, _T(""), MODIFY_ITEM);
		}
	}

	//���ױ� ����
	m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
	m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);

	Display();

EFS_END		
}

VOID CMNG_AC_MANAGER_04::OnChangeeInfoSource() 
{
	CString sInfoSource = _T("");
	GetDlgItem(IDC_eInfoSource)->GetWindowText(sInfoSource);

	m_pInfo->pMarcMgr->DeleteField(m_pMarc,_T("670"));

	m_pInfo->pMarcMgr->SetItem(m_pMarc,_T("670$a"),sInfoSource);

	//���ױ� ����
	m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
	m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);

	Display();	
}

HBRUSH CMNG_AC_MANAGER_04::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
