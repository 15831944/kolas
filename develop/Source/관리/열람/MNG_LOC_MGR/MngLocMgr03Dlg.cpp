// MngLocMgr03Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr03Dlg.h"
#include "ChangeValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr03Dlg dialog


CMngLocMgr03Dlg::CMngLocMgr03Dlg(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr03Dlg)
	m_nDELAY_MONEY_USE = -1;
	m_nDELAY_LOAN_STOP = -1;
	m_nAPPENDIX_DELAY_LOAN_STOP = -1;
	m_AddHolydayLoanStop = -1;
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;
	m_pEditCtrl1 = NULL;
	m_pEditCtrl2 = NULL;
	// 16/10/11 ������ : ����>����>����ȯ�漳��>����ȯ�漳�� �޴� ���� �� ����� �޸𸮴��� ���� ����
//*/ ADD ---------------------------------------------------------------------------
	m_pEditCtrl3 = NULL;
//*/ END ---------------------------------------------------------------------------
}

CMngLocMgr03Dlg::~CMngLocMgr03Dlg()
{
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	if(m_pEditCtrl1)
	{
		delete m_pEditCtrl1;
		m_pEditCtrl1 = NULL;
	}
	if(m_pEditCtrl2)
	{
		delete m_pEditCtrl2;
		m_pEditCtrl2 = NULL;
	}
	// 16/10/11 ������ : ����>����>����ȯ�漳��>����ȯ�漳�� �޴� ���� �� ����� �޸𸮴��� ���� ����
//*/ ADD ---------------------------------------------------------------------------
	if(m_pEditCtrl3)
	{
		delete m_pEditCtrl3;
		m_pEditCtrl3 = NULL;
	}
//*/ END ---------------------------------------------------------------------------
}


VOID CMngLocMgr03Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr03Dlg)
	DDX_Radio(pDX, IDC_radSETTING_DELAY_MONEY_USE_Y, m_nDELAY_MONEY_USE);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_STOP_USE_Y, m_nDELAY_LOAN_STOP);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_STOP_USE_Y2, m_nAPPENDIX_DELAY_LOAN_STOP);
	DDX_Radio(pDX, IDC_radSETTING_ADD_HOLYDAY_LOANSTOP1, m_AddHolydayLoanStop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngLocMgr03Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr03Dlg)
	ON_WM_CTLCOLOR()  
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_eMNG01_DELAYDAY_WEIGHT, OnKillfocuseMNG01DELAYDAYWEIGHT)
	ON_EN_KILLFOCUS(IDC_eMNG01_BASIC_ARREAR, OnKillfocuseMNG01BASICARREAR)
	ON_EN_KILLFOCUS(IDC_eMNG01_MAX_LOAN_STOP_DAY, OnKillfocuseMNG01MAXLOANSTOPDAY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr03Dlg message handlers

BOOL CMngLocMgr03Dlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CMngLocMgr03Dlg::ViewScreen()
{
EFS_BEGIN

	INT ids;
	// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
/*//BEFORE-----------------------------------------------------------------------------
	const int nCnt = 11;
*///AFTER------------------------------------------------------------------------------
	const int nCnt = 12;
/*//END-------------------------------------------------------------------------------*/
	
	// ����
	CString sSection[nCnt] = 
			{	_T("��ü����"), _T("�ηϰ���"), _T("��ü����"), _T("��ü����"), _T("��ü����"), 
				_T("��ü����"),	_T("��ü����"), _T("��ü����"), _T("��ü����"), _T("��ü����"),
				_T("��ü����") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
				, _T("��ü����")
//*/ END ---------------------------------------------------------------------------
			};
	// ���
	CString sAbility[nCnt] = 
			{	_T("���������Ϻο�"),		 _T("�ηϿ�ü�� ���������Ϻο�"),	_T("��ü��ο�"),	
				_T("���������� �ް�������"), _T("���������� �ο�����"),			_T("��ü�� ����ġ"), 
				_T("��ü�ϰ�����1"),		 _T("��ü�ϰ�����2"),				_T("�⺻��ü��"),	
				_T("���� ��üȽ�� �ο����"),_T("�ִ���������ο��ϼ�") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
				, _T("��ü�Ⱓ �ް�������")
//*/ END ---------------------------------------------------------------------------
			};
	// ����
	CString sExplain[nCnt] = 
			{	_T("�̿��ڰ� ��ü�� �ڷ�ݳ��� ������������ �ο��Ѵ�"), 
				_T("�η��ڷᰡ ��ü�Ǿ��� ��� ������������ �ο��Ѵ�"), 
				_T("�̿��ڰ� ��ü�� �ڷ�ݳ��� ��ü�Ḧ �ο��Ѵ�"),
				_T("���������Ͽ� �ް����� ������ �Ⱓ�� �����Ѵ�"),
				_T("�ݳ�å�� ��ü���� ����ū å���� �ο�����(ū���������Ϸκο�) �ݳ��ڷ��� ��ü������(�������������Ͽ� �ջ�)���� ��������"), 
				_T("��ü �Ϸ�� �ο��Ǵ� ����ġ"),
				_T("���������ο��ϼ� = (��ü�� + ��ü�ϰ�����1) x ��ü�ϰ���ġ + ��ü�ϰ�����2"), 
				_T("���������ο��ϼ� = (��ü�� + ��ü�ϰ�����1) x ��ü�ϰ���ġ + ��ü�ϰ�����2"),
				_T("��ü �Ϸ�� �����ؾ� �ϴ� �ݾ�"),
				_T("������üȽ���� ������������ �ο��� Ƚ���� ���� ��ü�� �ڷ��� ������üȽ���� ���� ����"),
				_T("���������� �ο��� �ο��� �� �ִ� �ִ��ϼ� (0��:���Ѿ���)") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
			   , _T("��ü�Ͽ� �ް����� ������ �Ⱓ�� �����Ѵ�") 
//*/ END ---------------------------------------------------------------------------
			};
	// ������
	CString sManage[nCnt] = 
			{	_T("���������Ϻο�����"),		_T("�ηϿ�ü���뿩��"),	_T("��ü��ο�����"),	
				_T("���������ް�����������"),	_T("��ü�ϸ��"),		_T("��ü�ϰ���ġ"), 
				_T("���������Ϻο��ϼ�"),		_T("��ü�Ϻο��ϼ�"),	_T("��ü��"),	
				_T("������üȽ���ο����"),		_T("�ִ���������ο��ϼ�") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
			   , _T("��ü�Ⱓ�ް�����������")
//*/ END ---------------------------------------------------------------------------
			};

	// Display
	m_pDM->FreeData();
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData( _T("����"), sSection[i], i ); 
		m_pDM->SetCellData( _T("���"), sAbility[i], i );
		m_pDM->SetCellData( _T("����"), sExplain[i], i );

		CString sGetData;
		ids = this->GetManageValue( _T("����"), _T("����"), sSection[i], sManage[i], sGetData);
		if( ids < 0 ) 
		{
			ESLAfxMessageBox(_T("������������ �������µ� �����Ͽ����ϴ�."));
			return -1;
		}
		
		if( sAbility[i] == _T("��ü�ϰ�����1") || sAbility[i] == _T("��ü�ϰ�����2") )
			if( sGetData == _T("-0") ) sGetData = _T("0");
		// DM����
		m_pDM->SetCellData( _T("������"), sGetData, i );
	}
	m_pCM->AllControlDisplay();
	/*
	CString strData = _T("");
	INT ids = 0;

	// 2007.07.13 add by pdw
	// ���������Ͽ� �ް��� ��������
	ids = this->GetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("���������ް�����������"),strData);
	if(_T("Y") == strData) m_AddHolydayLoanStop = 0;
	else  m_AddHolydayLoanStop = 1;
	strData.Empty();
	if(ids<0) return -1;

	//ù ��° 
	ids = this->GetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("���������Ϻο�����"),strData);
	if(_T("Y") == strData) m_nDELAY_LOAN_STOP = 0;
	else  m_nDELAY_LOAN_STOP = 1;
	strData.Empty();
	if(ids<0) return -1;

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ηϰ���"),_T("�ηϿ�ü���뿩��"),strData);
	if(_T("Y") == strData) m_nAPPENDIX_DELAY_LOAN_STOP = 0;
	else  m_nAPPENDIX_DELAY_LOAN_STOP = 1;
	strData.Empty();
	if(ids<0) return -1;

	ids = this->GetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("��ü��ο�����"),strData);
	if(_T("Y") == strData) m_nDELAY_MONEY_USE = 0;
	else  m_nDELAY_MONEY_USE = 1;
	strData.Empty();
	if(ids<0) return -2;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_DAY_HOW);
	ids = this->GetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("��ü�ϸ��"),strData);
	if(_T("0") == strData) pCombo->SetCurSel(0);
	else pCombo->SetCurSel(1);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -3;

	pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_COUNT_HOW);
	ids = this->GetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("������üȽ���ο����"),strData);
	if(_T("0") == strData) pCombo->SetCurSel(0);
	else pCombo->SetCurSel(1);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -4;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtSETTING_DELAY_MONEY_ADD_NUMBER2);
	ids = this->GetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("��ü�ϰ���ġ"),strData);
	pEdit->SetWindowText(strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -5;

	//pEdit = (CEdit*)GetDlgItem(IDC_edtSETTING_DELAY_MONEY_ADD_NUMBER);
	//ids = this->GetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("��ü�ᰡ��ġ"),strData);
	//pEdit->SetWindowText(strData);
	//pEdit = NULL;
	//strData.Empty();
	//if(ids<0) return -6;

	pEdit = (CEdit*)GetDlgItem(IDC_edtSETTING_DELAY_MONEY);
	ids = this->GetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("��ü��"),strData);
	pEdit->SetWindowText(strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -7;

	//const INT cnt = 2;
	//CString alias[ cnt ] = {
	//	_T("���������Ϻο��ϼ�"), _T("��ü�Ϻο��ϼ�") };
	//UINT comboID[ cnt ] = {
	//		IDC_COMBO1, IDC_COMBO3 };
	//UINT edtID[ cnt ] = {
	//		IDC_EDIT1, IDC_EDIT2 };
	//
	//if( GetManageValue( _T("����"),_T("����"),_T("��ü����"), alias[ opt ], value ) < 0 )
	//	return -1;
	
	if( this->GetManageValue( _T("����"), _T("����"), _T("��ü����"), _T("���������Ϻο��ϼ�"), strData ) < 0 )
		return -8;
	else
		SetDelayDateValue( 0, strData );

	if( this->GetManageValue( _T("����"), _T("����"), _T("��ü����"), _T("��ü�Ϻο��ϼ�"), strData ) < 0 )
		return -9;
	else
		SetDelayDateValue( 1, strData );
	
	//if( SetDelayDateValue( 0 ) )
	//	return -8;
	//
	//if( SetDelayDateValue( 1 ) )
	//	return -9;

	//if( GetManageValue( _T("����"),_T("����"),_T("��ü����"),_T("���������Ϻο��ϼ�"),strData ) < 0 )
	//	return -8;
	//SetDlgItemText( IDC_EDIT1, strData );

	UpdateData(FALSE);
	*/
	return 0;

EFS_END
return -1;
}

INT CMngLocMgr03Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
/*//BEFORE-----------------------------------------------------------------------------
	const int nCnt = 11;
*///AFTER------------------------------------------------------------------------------
	const int nCnt = 12;
/*//END-------------------------------------------------------------------------------*/

	// ����
	CString sSection[nCnt] = 
			{	_T("��ü����"), _T("�ηϰ���"), _T("��ü����"), _T("��ü����"), _T("��ü����"), 
				_T("��ü����"),	_T("��ü����"), _T("��ü����"), _T("��ü����"), _T("��ü����"),
				_T("��ü����") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
				, _T("��ü����")
//*/ END ---------------------------------------------------------------------------
			};
	// ������
	CString sManage[nCnt] = 
			{	_T("���������Ϻο�����"),		_T("�ηϿ�ü���뿩��"),	_T("��ü��ο�����"),	
				_T("���������ް�����������"),	_T("��ü�ϸ��"),		_T("��ü�ϰ���ġ"), 
				_T("���������Ϻο��ϼ�"),		_T("��ü�Ϻο��ϼ�"),	_T("��ü��"),	
				_T("������üȽ���ο����"),		_T("�ִ���������ο��ϼ�") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
				, _T("��ü�Ⱓ�ް�����������")
//*/ END ---------------------------------------------------------------------------
			};

	CString sCode;
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->GetCellData( _T("������"), i, sCode );
		
		ids = this->SetManageValue( _T("����"), _T("����"), sSection[i], sManage[i], sCode );
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("������������ �����ϴµ� �����Ͽ����ϴ�."));
			return -1;
		}
	}
	/*
	UpdateData(TRUE);
	
	CString strData = _T("");
	INT ids = 0;

	// 2007.07.13 add by pdw
	// ���������Ͽ� �ް��� ��������
	if(0 == m_AddHolydayLoanStop) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("���������ް�����������"),strData);
	strData.Empty();
	if(ids<0) return -1;

	//ù ��° 
	if(0 == m_nDELAY_LOAN_STOP) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("���������Ϻο�����"),strData);
	strData.Empty();
	if(ids<0) return -1;

	if(0 == m_nAPPENDIX_DELAY_LOAN_STOP) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ηϰ���"),_T("�ηϿ�ü���뿩��"),strData);
	strData.Empty();
	if(ids<0) return -1;

	if(0 == m_nDELAY_MONEY_USE) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("��ü��ο�����"),strData);
	strData.Empty();
	if(ids<0) return -2;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_DAY_HOW);
	strData.Format(_T("%d"),pCombo->GetCurSel());
	ids = this->SetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("��ü�ϸ��"),strData);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -3;
	
	pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_COUNT_HOW);
	strData.Format(_T("%d"),pCombo->GetCurSel());
	ids = this->SetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("������üȽ���ο����"),strData);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -4;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtSETTING_DELAY_MONEY_ADD_NUMBER2);
	pEdit->GetWindowText(strData);
	ids = this->SetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("��ü�ϰ���ġ"),strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -5;

	//pEdit = (CEdit*)GetDlgItem(IDC_edtSETTING_DELAY_MONEY_ADD_NUMBER);
	//pEdit->GetWindowText(strData);
	//ids = this->GetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("��ü�ᰡ��ġ"),strData);
	//pEdit = NULL;
	//strData.Empty();
	//if(ids<0) return -6;

	pEdit = (CEdit*)GetDlgItem(IDC_edtSETTING_DELAY_MONEY);
	pEdit->GetWindowText(strData);
	ids = this->SetManageValue(_T("����"),_T("����"),_T("��ü����"),_T("��ü��"),strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -7;

	if( this->SetManageValue( _T("����"), _T("����"), _T("��ü����"), _T("���������Ϻο��ϼ�"), GetDelayDateValue( 0 ) ) < 0 )
		return -8;

	if( this->SetManageValue( _T("����"), _T("����"), _T("��ü����"), _T("��ü�Ϻο��ϼ�"), GetDelayDateValue( 1 ) ) < 0 )
		return -9;
	
	//if( UpdateDelayDateValue( 0 ) )
	//	return -8;
	//
	//if( UpdateDelayDateValue( 1 ) )
	//	return -9;

	//GetDlgItemText( IDC_EDIT1, strData );
	//if( SetManageValue( _T("����"),_T("����"),_T("��ü����"),_T("���������Ϻο��ϼ�"),strData ) < 0 )
	//	return -8;
	*/
	return 0;

EFS_END
return -1;
}

/// ��ٱ��� DM���� VALUE�� �̾Ƴ��� �Լ�
INT CMngLocMgr03Dlg::GetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_��ٱ���������2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	ids = CLocCommonAPI::GetManageValue( this , pDM , sGroup2 , sGroup3 , sAlias , sValueName , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	return 0;

EFS_END
return -1;
}

/// ��ٱ��� DM���� VALUE�� �̾Ƴ��� �Լ�
INT CMngLocMgr03Dlg::SetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_��ٱ���������2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	INT nIndex;

	ids = CLocCommonAPI::FindManageDMIndex( this , pDM , sGroup2 , sGroup3 , sAlias , sValueName , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetManageValue") );
	if( ids > 0 )
	{
		/// �������� �ʴ´ٸ� ���� �����.
		pDM->StartFrame();
		
		CString sRecKey;
		
		ids = pDM->MakeRecKey( sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SetManageValue") );
		
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pDM->AddDBFieldData( _T("GROUP_1") , _T("STRING") , _T("L") , TRUE );		
		pDM->AddDBFieldData( _T("GROUP_2") , _T("STRING") , sGroup2 , TRUE );
		pDM->AddDBFieldData( _T("GROUP_3") , _T("STRING") , sGroup3 , TRUE );
		pDM->AddDBFieldData( _T("CLASS_ALIAS") , _T("STRING") , sAlias, TRUE );
		pDM->AddDBFieldData( _T("VALUE_NAME") , _T("STRING") , sValueName , TRUE );
		pDM->AddDBFieldData( _T("VALUE_1") , _T("STRING") , sValue , TRUE );
		pDM->AddDBFieldData( _T("SYS_SETUP_YN") , _T("STRING") , _T("Y") , TRUE );
		pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") , TRUE ); // 2008.10.22 ADD BY PWR (���սý��� ����)
		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("��ü�ڷ�"), __WFILE__, __LINE__ ), TRUE );
		
		pDM->MakeInsertFrame( _T("ESL_MANAGE_TBL") );
		
		ids = pDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetManageValue") );
		
		pDM->InsertRow(-1);
		INT nInsertRow = pDM->GetRowCount() -1 ;
		ids = pDM->SetCellData( _T("REC_KEY") , sRecKey , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("GROUP_2") , sGroup2 , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("GROUP_3") , sGroup3 , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("CLASS_ALIAS") , sAlias , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("VALUE_NAME") , sValueName , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("VALUE_1") , sValue , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("SetManageValue") );
		
		return 0;
	}
	
	ids = pDM->SetCellData( _T("VALUE_1") , sValue , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetManageValue") );

	

	ids = CESL_Mgr::SetManageValue( sGroup2 , sGroup3 , sAlias , sValueName , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetManageValue") );

	return 0;

EFS_END
return -1;
}

BOOL CMngLocMgr03Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/*
	CComboBox *pCombo = NULL;
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->InsertString( 0, _T("+") );
	pCombo->InsertString( 1, _T("-") );
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	pCombo->InsertString( 0, _T("+") );
	pCombo->InsertString( 1, _T("-") );
	*/
	if( InitESL_Mgr(_T("SM_����ȯ�漳��_��ü�ڷ�")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_����ȯ�漳��_��ü�ڷ�"));
	m_pDM = FindDM(_T("DM_����ȯ�漳��_��ü�ڷ�"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	// �ݳ�����Ƚ��
	m_pEditCtrl1 = new CEdit();
	m_pEditCtrl1->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_eMNG01_DELAYDAY_WEIGHT);
	m_pEditCtrl1->ShowWindow(SW_HIDE);
	// ��������ο�
	m_pEditCtrl2 = new CEdit();
	m_pEditCtrl2->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_eMNG01_BASIC_ARREAR);
	m_pEditCtrl2->ShowWindow(SW_HIDE);
	// �ִ���������ο��ϼ�
	m_pEditCtrl3 = new CEdit();
	m_pEditCtrl3->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_eMNG01_MAX_LOAN_STOP_DAY);
	m_pEditCtrl3->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;
}

INT CMngLocMgr03Dlg::SetDelayDateValue( INT opt, CString value )
{
	const INT cnt = 2;
	CString alias[ cnt ] = {
		_T("���������Ϻο��ϼ�"), _T("��ü�Ϻο��ϼ�") };
	UINT comboID[ cnt ] = {
			IDC_COMBO1, IDC_COMBO3 };
	UINT edtID[ cnt ] = {
			IDC_EDIT1, IDC_EDIT2 };
	
	//if( GetManageValue( _T("����"),_T("����"),_T("��ü����"), alias[ opt ], value ) < 0 )
	//	return -1;

	INT nValue = _ttoi( value );
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( comboID[ opt ] );
	if( nValue > 0 )
		pCombo->SetCurSel( 0 );
	else
		pCombo->SetCurSel( 1 );

	CString sValue;
	sValue.Format( _T("%d"), abs( nValue ) );
	SetDlgItemText( edtID[ opt ], sValue );

	return 0;
}

INT CMngLocMgr03Dlg::UpdateDelayDateValue( INT opt )
{
	const INT cnt = 2;
	CString alias[ cnt ] = {
		_T("���������Ϻο��ϼ�"), _T("��ü�Ϻο��ϼ�") };
	UINT comboID[ cnt ] = {
			IDC_COMBO1, IDC_COMBO3 };
	UINT edtID[ cnt ] = {
			IDC_EDIT1, IDC_EDIT2 };
	
	CString flag;
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( comboID[ opt ] );
	if( pCombo->GetCurSel() )
		flag = _T("-");
	else
		flag = _T("+");

	CString sValue, value;
	GetDlgItemText( edtID[ opt ], sValue );

	value.Format( _T("%s%s"), flag, sValue );
	if( SetManageValue( _T("����"),_T("����"),_T("��ü����"), alias[ opt ], value ) < 0 )
		return -1;

	return 0;
}

CString CMngLocMgr03Dlg::GetDelayDateValue(INT opt)
{
	const INT cnt = 2;
	CString alias[ cnt ] = {
		_T("���������Ϻο��ϼ�"), _T("��ü�Ϻο��ϼ�") };
	UINT comboID[ cnt ] = {
			IDC_COMBO1, IDC_COMBO3 };
	UINT edtID[ cnt ] = {
			IDC_EDIT1, IDC_EDIT2 };
	
	CString flag;
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( comboID[ opt ] );
	if( pCombo->GetCurSel() )
		flag = _T("-");
	else
		flag = _T("+");

	CString sValue, value;
	GetDlgItemText( edtID[ opt ], sValue );

	value.Format( _T("%s%s"), flag, sValue );

	return value;
}

HBRUSH CMngLocMgr03Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BEGIN_EVENTSINK_MAP(CMngLocMgr03Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr03Dlg)
	ON_EVENT(CMngLocMgr03Dlg, IDC_gridMNG_LOC_MGR_03, -600 /* Click */, OnClickgridMNGLOCMGR03, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMngLocMgr03Dlg::OnClickgridMNGLOCMGR03() 
{
	if( m_pGrid->GetMouseRow() == 0 ) return;
	INT col = m_pGrid->GetCol();
	if( col == 4 ) CreateComboBoxInGridSell();	
}

void CMngLocMgr03Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_LOC_MGR_03);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 1, cx, cy-10);	
}

// ** �޺��ڽ� ���� �� �� ����
VOID CMngLocMgr03Dlg::CreateComboBoxInGridSell()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	INT nRow = m_pGrid->GetRow();
	CString strData = m_pDM->GetCellData( _T("������"), nRow - 1 );
	CString strElement = m_pDM->GetCellData( _T("���"), nRow - 1 );

	if( m_pComboBox )
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	
	CStringArray ArrCmbData;	ArrCmbData.RemoveAll();

	// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
/*//BEFORE-----------------------------------------------------------------------------
	// 1:���������Ϻο�, 2:�ηϿ�ü�� ���������Ϻο�, 3:��ü��ο�, 4:���������� �ް�������
	if( nRow == 1 || nRow == 2 || nRow == 3 || nRow == 4 )
*///AFTER------------------------------------------------------------------------------
	// 1:���������Ϻο�, 2:�ηϿ�ü�� ���������Ϻο�, 3:��ü��ο�, 4:���������� �ް�������, 12:��ü�Ⱓ�ް�����������
	if( nRow == 1 || nRow == 2 || nRow == 3 || nRow == 4 || nRow == 12 )
/*//END-------------------------------------------------------------------------------*/
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("�����"));
		ArrCmbData.Add(_T("������"));
	}
	// 5:���������� �ο�����
	else if( nRow == 5 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("��ü���� ���� ū ��ü��"));
		ArrCmbData.Add(_T("�� �ڷ��� ��ü�� ��"));
	}
	// 10:������üȽ���ο����
	else if( nRow == 10 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("���������ο����� ��������"));
		ArrCmbData.Add(_T("��ü�� å���� �ο�"));
	}
	// 11:�ִ���������ο��ϼ�
	else if( nRow == 11 )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl3);
		return;
	}
	// 7:��ü�ϰ�����1, 8:��ü�ϰ�����2
	else if( nRow == 7 || nRow == 8 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("-3"));
		ArrCmbData.Add(_T("-2"));
		ArrCmbData.Add(_T("-1"));
		ArrCmbData.Add(_T("0"));
		ArrCmbData.Add(_T("1"));
		ArrCmbData.Add(_T("2"));
		ArrCmbData.Add(_T("3"));
	}
	// 6:��ü�ϰ���ġ
	else if( nRow == 6 )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl1);
		return;
	}
	// 9:�⺻��ü��
	else if( nRow == 9 )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl2);
		return;
	}
	else
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("������"));
		ArrCmbData.Add(_T("�������"));
	}
	
	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), m_pGrid, 6450);
	m_pComboBox->Init(&ArrCmbData);
	m_pComboBox->InitMngLocMgr(3);
	m_pComboBox->SetIndex(nRow-1);
	m_pComboBox->SetDataMgr(m_pDM, m_pGrid);
	m_pComboBox->SetParent(m_pGrid);
	m_pComboBox->SetCurSelData(strData);
	m_pComboBox->SetHorizontalExtent(10);
	CFont * pFont = this->GetFont();
	m_pComboBox->SetFont(pFont);
	
	CRect rect,gridRect;
	RECT parentRect;
	m_pGrid->GetWindowRect(&gridRect);
	this->GetClientRect(&parentRect);

	::ClientToScreen(this->m_hWnd,(POINT*)&(parentRect.left));
	
	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;
	rect.left = (long)((float)m_pGrid->GetCellLeft()/15.0f) - gridRect.left;
	rect.top = (long)((float)m_pGrid->GetCellTop()/15.0f) - gridRect.top;
	rect.right = rect.left + (long)((float)m_pGrid->GetCellWidth()/15.0f);
	rect.bottom = rect.top + (long)((float)m_pGrid->GetCellHeight()/15.0f);

	m_pComboBox->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
	m_pComboBox->ShowWindow(TRUE);
	m_pComboBox->SetFocus();
}

INT CMngLocMgr03Dlg::ShowEditBox(CESL_Grid *pGrid, CEdit* pEditCtrl)
{
EFS_BEGIN

	pEditCtrl->GetSafeHwnd();

	if (pGrid == NULL) return -1;

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if (nRow < 1 || nCol < 1) return -1;

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	// �ؽ�Ʈ ����
	CString sValue = pGrid->GetTextMatrix(nRow, nCol);
	sValue.Replace( _T("��"), _T("") );
	sValue.Replace( _T("��"), _T("") );
	pEditCtrl->SetWindowText(sValue);

	pEditCtrl->MoveWindow(rcCell);
	pEditCtrl->ShowWindow(SW_SHOW);
	pEditCtrl->SetFocus();
	pEditCtrl->BringWindowToTop();
	
	return 0;

EFS_END
return -1;
}

INT CMngLocMgr03Dlg::GetCellRect(CESL_Grid *pGrid, CRect &rect)
{
EFS_BEGIN

	if (pGrid == NULL) return -1;

	CRect rcGrid;
	CRect rcClient;

	pGrid->GetWindowRect(rcGrid);

	GetClientRect(rcClient);

	ClientToScreen((POINT*)&(rcClient.left));

	rcGrid.top -= rcClient.top;
	rcGrid.left -= rcClient.left;

	rect.left	= rcGrid.left	+ (long)((float)pGrid->GetCellLeft() / 15.0f);
	rect.top	= rcGrid.top	+ (long)((float)pGrid->GetCellTop() / 15.0f);
	rect.right	= rect.left		+ (long)((float)pGrid->GetCellWidth() / 15.0f) - 1;
	rect.bottom = rect.top		+ (long)((float)pGrid->GetCellHeight() / 15.0f) - 1;

	return 0;

EFS_END
return -1;
}

INT CMngLocMgr03Dlg::ApplyDataChange(LPCTSTR lpszData)
{
EFS_BEGIN

	if (lpszData == NULL) return -1;
	if (m_pDM == NULL || m_pGrid == NULL) return -1;

	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();

	// �׸��忡 ������ �����Ų��.
	m_pGrid->SetFocus();
	m_pGrid->SetTextMatrix(nRow, nCol, lpszData);

	// ������ �޴����� ������ �����Ų��.
	m_pDM->SetCellData( _T("������"), lpszData, nRow - 1 );
	m_pGrid->Display();

	return 0;

EFS_END
return -1;
}

void CMngLocMgr03Dlg::OnKillfocuseMNG01DELAYDAYWEIGHT() 
{
	SetFocus();
	if (m_pEditCtrl1 == NULL) return ;
	if (!::IsWindowVisible(m_pEditCtrl1->GetSafeHwnd())) return ;

	m_pEditCtrl1->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl1->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}

void CMngLocMgr03Dlg::OnKillfocuseMNG01BASICARREAR() 
{
	SetFocus();
	if (m_pEditCtrl2 == NULL) return ;
	if (!::IsWindowVisible(m_pEditCtrl2->GetSafeHwnd())) return ;

	m_pEditCtrl2->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl2->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}

void CMngLocMgr03Dlg::OnKillfocuseMNG01MAXLOANSTOPDAY() 
{
	SetFocus();
	if (m_pEditCtrl3 == NULL) return ;
	if (!::IsWindowVisible(m_pEditCtrl3->GetSafeHwnd())) return ;

	m_pEditCtrl3->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl3->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}

INT CMngLocMgr03Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(strDMFieldAlias == _T("������"))
	{
		CString sAbility;
		m_pDM->GetCellData( _T("���"), nRow, sAbility );

		// �ش����� �������� �ڵ带 �������� �����Ѵ�
		CChangeValue *pMng = new CChangeValue();
		str = pMng->CodeToDesc( sAbility, str );

		delete pMng;
		pMng = NULL;
	}
	
	return 0;
}


