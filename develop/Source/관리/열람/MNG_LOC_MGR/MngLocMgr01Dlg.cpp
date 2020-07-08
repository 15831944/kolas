// MngLocMgr01Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr01Dlg.h"
#include "ChangeValue.h"
#include "SettingGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr01Dlg dialog


CMngLocMgr01Dlg::CMngLocMgr01Dlg(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr01Dlg)
	m_nRELOAN_ONE = -1;
	m_nRELOAN_FAMILY = -1;
	m_nLOAN_MANAGE = -1;
	m_nLOAN_TYPE0 = -1;
	m_nLOAN_TYPE1 = -1;
	m_nLOAN_TYPE2 = -1;
	m_nLOAN_TYPE3 = -1;
	m_nRETURN_TYPE0 = -1;
	m_nRETURN_TYPE1 = -1;
	m_nRETURN_TYPE2 = -1;
	m_nUSER_FAMILY = -1;
	m_nUSER_NUMBER = -1;
	m_nUSER_LOAN_STOP = -1;
	m_Appendix_Include_loan_book_Y = -1;
	m_Appendix_Loan_Y = -1;
	m_UserCard_Y = -1;
	m_nReLoanView = -1;
	m_sReLoanLimitDay = _T("0");
	m_cAutoNumType = -1;
	m_nRETURN_MANAG = -1;
	m_nLOAN_RECEIPT = -1;
	m_nRETURN_RECEIPT = -1;
	m_nCIVILNO_CHECK_Y = -1;
	m_Appendix_Return_Y = -1;
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;
	m_pEditCtrl[0] = NULL;
	m_pEditCtrl[1] = NULL;

	m_bIsUnityLoanService = FALSE;
}
CMngLocMgr01Dlg::~CMngLocMgr01Dlg()
{	
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	if(m_pEditCtrl[0])
	{
		delete m_pEditCtrl[0];		
		m_pEditCtrl[0] = NULL;
	}
	if(m_pEditCtrl[1])
	{
		delete m_pEditCtrl[1];
		m_pEditCtrl[1] = NULL;
	}	
}


VOID CMngLocMgr01Dlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN	

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr01Dlg)
	DDX_Radio(pDX, IDC_radSETTING_RELOAN_ONE_Y, m_nRELOAN_ONE);
	DDX_Radio(pDX, IDC_radSETTING_RELOAN_FAMILY_Y, m_nRELOAN_FAMILY);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_MANAGE_Y, m_nLOAN_MANAGE);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_TYPE_0_Y, m_nLOAN_TYPE0);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_TYPE_1_Y, m_nLOAN_TYPE1);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_TYPE_2_Y, m_nLOAN_TYPE2);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_TYPE_3_Y, m_nLOAN_TYPE3);
	DDX_Radio(pDX, IDC_radSETTING_RETURN_TYPE_0_Y, m_nRETURN_TYPE0);
	DDX_Radio(pDX, IDC_radSETTING_RETURN_TYPE_1_Y, m_nRETURN_TYPE1);
	DDX_Radio(pDX, IDC_radSETTING_RETURN_TYPE_2_Y, m_nRETURN_TYPE2);
	DDX_Radio(pDX, IDC_radSETTING_FAMILY_USE_Y, m_nUSER_FAMILY);
	DDX_Radio(pDX, IDC_radSETTING_USER_NO_AUTO_USE_Y, m_nUSER_NUMBER);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_STOP_DATE_AUTO_USE_Y, m_nUSER_LOAN_STOP);
	DDX_Radio(pDX, IDC_radSETTING_APPENDIX_INLUDE_LOAN_BOOK_Y, m_Appendix_Include_loan_book_Y);
	DDX_Radio(pDX, IDC_radSETTING_APPENDIX_LOAN_Y, m_Appendix_Loan_Y);
	DDX_Radio(pDX, IDC_radSETTING_USER_CARD_Y, m_UserCard_Y);
	DDX_Radio(pDX, IDC_radSETTING_RELOAN_VIEW_Y, m_nReLoanView);
	DDX_Text(pDX, IDC_edtRELOAN_LIMIT_DAY, m_sReLoanLimitDay);
	DDX_Control(pDX, IDC_edtRELOAN_LIMIT_DAY, m_edtReLoanLimitDay);
	DDX_CBIndex(pDX, IDC_COMBO2, m_cAutoNumType);
	DDX_Radio(pDX, IDC_radSETTING_RETURN_MANAGE_Y, m_nRETURN_MANAG);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_RECEIPT_USE_Y, m_nLOAN_RECEIPT);
	DDX_Radio(pDX, IDC_radSETTING_RETURN_RECEIPT_USE_Y, m_nRETURN_RECEIPT);
	DDX_Radio(pDX, IDC_radSETTING_CIVILNO_AUTO_CHECK_USE_Y, m_nCIVILNO_CHECK_Y);
	DDX_Radio(pDX, IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_Y, m_Appendix_Return_Y);
	//}}AFX_DATA_MAP

	EFS_END		
}


BEGIN_MESSAGE_MAP(CMngLocMgr01Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr01Dlg)
	ON_BN_CLICKED(IDC_radSETTING_RELOAN_ONE_N, OnradSETTINGRELOANONEN)
	ON_BN_CLICKED(IDC_radSETTING_RELOAN_ONE_Y, OnradSETTINGRELOANONEY)
	ON_BN_CLICKED(IDC_radSETTING_RELOAN_FAMILY_Y, OnradSETTINGRELOANFAMILYY)
	ON_BN_CLICKED(IDC_radSETTING_RELOAN_FAMILY_N, OnradSETTINGRELOANFAMILYN)
	ON_BN_CLICKED(IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_Y, OnradSETTINGAPPENDIXINLUDERETURNBOOKY)
	ON_BN_CLICKED(IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_N, OnradSETTINGAPPENDIXINLUDERETURNBOOKN)
	ON_BN_CLICKED(IDC_radSETTING_APPENDIX_LOAN_Y, OnradSETTINGAPPENDIXLOANY)
	ON_BN_CLICKED(IDC_radSETTING_APPENDIX_LOAN_N, OnradSETTINGAPPENDIXLOANN)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_edtRELOAN_LIMITDAY, OnKillfocusedtRELOANLIMITDAY)
	ON_EN_KILLFOCUS(100, OnKillfocusedt2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr01Dlg message handlers

BOOL CMngLocMgr01Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	/*
	pMngSpfWork = new CMngLocMgrAPI(this);
	
	// ���� �߰�
	const INT listCnt = 3;
	CString list[ listCnt ] = {
		_T("������"), _T("��������ȣ"), _T("�ֹε�Ϲ�ȣ") };
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	for( INT i = 0; i < listCnt; i++ )
		pCombo->InsertString( i, list[ i ] );
	pCombo->SetCurSel( 0 );
	*/
	if( InitESL_Mgr(_T("SM_����ȯ�漳��_����ݳ����̿���")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_����ȯ�漳��_����ݳ����̿���"));
	m_pDM = FindDM(_T("DM_����ȯ�漳��_����ݳ����̿���"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	// �����������
	m_pEditCtrl[0] = new CEdit();
	m_pEditCtrl[0]->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_edtRELOAN_LIMITDAY);
	m_pEditCtrl[0]->ShowWindow(SW_HIDE);

	m_pEditCtrl[1] = new CEdit();
	m_pEditCtrl[1]->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, 100);
	m_pEditCtrl[1]->ShowWindow(SW_HIDE);

	if(m_pInfo->MODE==10000)
	{
		CString strButtonAlias ;
		strButtonAlias = _T("���泻������");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
		strButtonAlias = _T("��������");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
	}
	
	CString strValue;

	// 18.09.27 JMJ KOL.RED.2018.008
	// å�η����� ������ �߰�
	INT ids = pParentMgr->GetManageValue(_T("����"), _T("����"), _T("å�η��ý���"), _T("å�η���뿩��"), strValue);
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("OnInitDialog"));
	}
	strValue.TrimLeft(); strValue.TrimRight();
	if(_T("Y") == strValue) 
	{
		ids = pParentMgr->GetManageValue(_T("����"), _T("����"), _T("å�η��ý���"), _T("å�η�����"), strValue);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("OnInitDialog"));
		}
		strValue.TrimLeft(); strValue.TrimRight();
		if(strValue.IsEmpty())
		{
			m_sBookCooperativeMode = _T("I");
		}
		else
		{
			m_sBookCooperativeMode = strValue;
		}
	}
	else
	{
		m_sBookCooperativeMode = _T("I");
	}
	
	// 2010.07.28 ADD BY KSJ : ���� ȸ�� ���� �ý��� ��뿩��
	ids = pParentMgr->GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), strValue);

	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("OnInitDialog"));
	strValue.TrimLeft(); strValue.TrimRight();
	if(_T("Y")==strValue)
	{
		m_bIsUnityLoanService = TRUE;
	}
	else
	{
		m_bIsUnityLoanService = FALSE;
	}


	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMngLocMgr01Dlg::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN	

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

INT CMngLocMgr01Dlg::ViewScreen()
{
EFS_BEGIN	

	INT ids;
	// 17/05/18 ������ : ȸ���� ���������� �ź� Ȯ���� ���Ͽ� ����������� �߰�
/*//BEFORE-----------------------------------------------------------------------------
	const int nCnt = 23;
*///AFTER------------------------------------------------------------------------------
	//const int nCnt = 24;
/*//END-------------------------------------------------------------------------------*/
	//JOB.2019.0033 ADD BY KYJ
	const int nCnt = 25;
	// ����
	CString sSection[nCnt] = 
			{	_T("��������"),		_T("��������"),		_T("��������"),		_T("��������"),		//4
				_T("�ݳ�����"),		_T("�ݳ�����"),		_T("�ݳ�����"),		_T("�����"),		//8	
				_T("�����"),		_T("�����"),		_T("�����"),		 _T("�������"), //JOB.2019.0033 ADD BY KYJ	//12
				_T("�η��ڷ�"),		_T("�η��ڷ�"),		_T("�η��ڷ�"),		
				_T("�̿��ڰ���"),	_T("�̿��ڰ���"), 	_T("�̿��ڰ���"),	
				_T("�̿��ڰ���"),	_T("�̿��ڰ���"),	// 2009.04.21 ADD BY KSJ
				// 17/05/18 ������ : ȸ���� ���������� �ź� Ȯ���� ���Ͽ� ����������� �߰�
/*//BEFORE-----------------------------------------------------------------------------
				_T("�̿��ڰ���"),	_T("���������"), _T("��ϱ���"), _T("����ڷ����") 
*///AFTER------------------------------------------------------------------------------
				_T("�̿��ڰ���"),	_T("�̿��ڰ���"),
				_T("���������"),	 _T("��ϱ���"),	_T("����ڷ����") 
/*//END-------------------------------------------------------------------------------*/
			};
				
	// ���
	CString sAbility[nCnt] = 
			{	_T("�Ϲݴ�����"),			_T("����������"),		_T("Ư��������"), //3
				_T("��������"),			_T("�Ϲݹݳ����"),		_T("�ϰ��ݳ����"),	//6		
				_T("�ұ޹ݳ����"),			_T("������������"),	_T("������������"), //9	
				_T("������̷º����ֱ�"),	_T("�����������"),		_T("Ÿ�ý���ī���ȣ��뿩��"), //JOB.2019.0033 ADD BY KYJ //12
				_T("�η��ڷ����"), 
				_T("�η��ڷ�ݳ�"), 		_T("����å���� ����"),	_T("����ȸ�����"),			
				_T("���������� �ڵ��ϰ�����/����"),		_T("�����ڹ�ȣ �ڵ��ο�"), 
				_T("�����ڹ�ȣ �����ID�� �ڵ��Է�"),	_T("�ֹε�Ϲ�ȣ ���ڸ� ��й�ȣ�� �ڵ��Է�"), // 2009.04.2 ADD BY KSJ
				// 17/05/18 ������ : ȸ���� ���������� �ź� Ȯ���� ���Ͽ� ����������� �߰�
/*//BEFORE-----------------------------------------------------------------------------
				_T("�ֹε�Ϲ�ȣ �ڵ��ߺ��˻�"),		_T("��������»��"), _T("�������� ��ϱ��� �ߺ�����"),
*///AFTER------------------------------------------------------------------------------
				_T("�ֹε�Ϲ�ȣ �ڵ��ߺ��˻�"),		 _T("���������� ���� �̿�����ȸ ��뿩��"),
				_T("��������»��"),					_T("�������� ��ϱ��� �ߺ�����"),
/*//END-------------------------------------------------------------------------------*/				
				_T("�⺻ �������")
			};
	// ����
	CString sExplain[nCnt] = 
			{	_T("���� ���� ����"), 
				_T("������ѿ� 1�� ����"), 
				_T("�������ѿ� �ɷ��ִ� å�̳� �̿��ڿ� ������� ����"),
				_T("�̿��� ���޿� ������� ������ ��������� ����"),
				_T("�Ϲ����� ���ǹݳ�"), 
				_T("����å�� ���ÿ� �ݳ�"),
				_T("�ݳ����� �����Ͽ� �ݳ�"), 
				_T("�����Ϸ��� �ڷᰡ �̹� ���� �������� �����̷��� �־ ���Ⱑ��"),
				_T("�����Ϸ��� �ڷᰡ �ش� �̿����� �����̷��� �־ ���Ⱑ��"),
				_T("����,���� �Ϸ��� �ڷᰡ �����̷��� �ִٸ� �̷��� �����ش�"), 
				_T("�ݳ��Ϸκ��� �������� ��ŭ �ڷᰡ ������ �̷��� �ִٸ� ���������� �ش�"), 
				//JOB.2019.0033 ADD BY KYJ
				_T("Ÿ�ý����� ī���ȣ�� ������� �����Ѵ�"), //12
				_T("�η��ڷ��� ���⿩�θ� �����Ѵ�"),
				_T("�ݳ��� ����� �η��ڷᰡ�ִ� �ڷ�� �ʼ������� �η��ڷḦ �ݳ��ϴ��� ����"),
				_T("�η��ڷ� ����å���� �����Ѵ�"), 
				_T("�̿����Է��̳� ����/�ݳ����� ����ȸ������� ����Ѵ�"),
				_T("����/�ݳ��޴� ���� ������������ ���� �̿����� �ҷ� ȸ�������� �ڵ��ϰ������Ѵ�"), 
				_T("������ �Է½� �����ڹ�ȣ�� ��������ȣ �Ǵ� �ֹι�ȣ�� �ڵ��Է��Ѵ�"),
				_T("�����ڹ�ȣ�� �����ID�� �ڵ��Է��Ѵ�."),			// 2009.04.21 ADD BY KSJ
				_T("��й�ȣ�� �ֹε�Ϲ�ȣ ���ڸ��� �ڵ��Է��Ѵ�."),	// 2009.04.21 ADD BY KSJ
				_T("�ֹε�Ϲ�ȣ �Է½� �ڵ����� �ߺ��˻縦 �Ѵ�"),
				// 17/05/18 ������ : ȸ���� ���������� �ź� Ȯ���� ���Ͽ� ����������� �߰�
//*/ ADD ---------------------------------------------------------------------------
				_T("������������� ����Ͽ� �̿��������� ��ȸ�Ѵ�"),
//*/ END ---------------------------------------------------------------------------
				_T("������ ���� �����̳� �ݳ��϶� ����� �� �ְ�, ����ݳ� ��ο��� ����� �� �ִ�."),
				_T("���������� ���� ��ϱ����� �ߺ��� ��, �ٸ� �������� ���� ��ϱ����� ������ �� �ֵ��� �Ѵ�."),
				_T("���ó������ �⺻ ��������� �����Ѵ�.")};
	// Alias
	CString sAlias[nCnt] = 
			{	_T("�������Ļ�뿩��"), _T("�������Ļ�뿩��"), _T("�������Ļ�뿩��"), 
				_T("�������Ļ�뿩��"), _T("�ݳ����Ļ�뿩��"), _T("�ݳ����Ļ�뿩��"),	
				_T("�ݳ����Ļ�뿩��"), _T("�������"),			_T("�������"), 
				_T("�������"),	
				//JOB.2019.0033 ADD BY KYJ				
				_T("�������"),  
				_T("�������"), 		_T("�ηϰ���"),			_T("�ηϰ���"), 
				_T("�ηϰ���"),			_T("�̿��ڰ���"),		_T("�̿��ڰ���"), 
				_T("�̿��ڰ���"),		
				_T("�̿��ڰ���"),		_T("�̿��ڰ���"),	// 2009.04.21 ADD BY KSJ
				// 17/05/18 ������ : ȸ���� ���������� �ź� Ȯ���� ���Ͽ� ����������� �߰�
/*//BEFORE-----------------------------------------------------------------------------
				_T("�̿��ڰ���"),		_T("���������"), _T("��ϱ���"), _T("����ڷ����") 
*///AFTER------------------------------------------------------------------------------
				_T("�̿��ڰ���"),		_T("�̿��ڰ���"),
				_T("���������"),		_T("��ϱ���"),			_T("����ڷ����") 
/*//END-------------------------------------------------------------------------------*/
			};
	// ������
	CString sManage[nCnt] = 
			{	_T("�Ϲ�"), _T("����"), _T("Ư��"),	_T("���"), _T("�Ϲ�"), _T("�ϰ�"),	_T("�ұ�"),  //7
				_T("��������⿩��"),		_T("��������⿩��"),			_T("������̷º����ֱ⿩��"),  //10
				_T("�����������"),		//11
				//JOB.2019.0033 ADD BY KYJ
				_T("Ÿ�ý���ī���ȣ��뿩��"),	 //12
				_T("�ηϴ��⿩��"),			_T("�ηϹݳ�����"), 			_T("�ηϴ���Ǽ����Կ���"),			
				_T("����ȸ����ɻ�뿩��"),	_T("���������ϰ���������"),		_T("�����ڹ�ȣ�ڵ��ο�����"),
				_T("�����ڹ�ȣ�����ID���ڵ��Է�"),	_T("�ֹε�Ϲ�ȣ���ڸ���й�ȣ���ڵ��Է�"),	// 2009.04.21 ADD BY KSJ
				// 17/05/18 ������ : ȸ���� ���������� �ź� Ȯ���� ���Ͽ� ����������� �߰�
/*//BEFORE-----------------------------------------------------------------------------
				_T("�ֹε�Ϲ�ȣ�ڵ��ߺ��˻翩��"),	_T("��������»������"), _T("�������� ��ϱ��� �ߺ�����"),
*///AFTER------------------------------------------------------------------------------
				_T("�ֹε�Ϲ�ȣ�ڵ��ߺ��˻翩��"), _T("����������ȸ��뿩��"),
				_T("��������»������"), _T("�������� ��ϱ��� �ߺ�����"),
/*//END-------------------------------------------------------------------------------*/
				_T("�⺻ �������") };

	// Display
	m_pDM->FreeData();
	INT nNum = 0;
	for( INT i = 0; i < nCnt; i++ )
	{
		CString sData;
		// �������� ���翡���� ���̵��� �Ѵ�
		if( sManage[i] == _T("���") )
		{
			ids = this->GetManageValue( _T("��Ÿ"), _T("����"), _T("����"), _T("����"), sData );
			sData.TrimLeft( ); sData.TrimRight( );
			if ( sData != _T("Y") ) continue;
		}
		// ��������� ��������� ���� �׸��忡 ǥ��
		else if( sManage[i] == _T("��������»������") )
		{
			ids = this->GetManageValue( _T("����"), _T("����"), _T("���������"), _T("��������»�뿩��"), sData );
			sData.TrimLeft( ); sData.TrimRight( );
			if ( sData != _T("Y") ) continue;
		}
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData( _T("����"), sSection[i], nNum ); 
		m_pDM->SetCellData( _T("���"), sAbility[i], nNum );
		m_pDM->SetCellData( _T("����"), sExplain[i], nNum );

		CString sGetData;
		ids = this->GetManageValue( _T("����"), _T("����"), sAlias[i], sManage[i], sGetData);
		if( ids < 0 ) 
		{
			ESLAfxMessageBox(_T("������������ �������µ� �����Ͽ����ϴ�."));
			return -1;
		}

		// 2010.12.22 DELETE BY KSJ : �Ϲ�ȸ���̶�� �����ڹ�ȣ �ڵ��ο� ������ ��� �� �� �ֵ��� ����
		// 2010.07.28 ADD BY KSJ : ���� ȸ�� ���� �ý��� ��� �� ��� �����ڹ�ȣ �ڵ��ο��� Y(�ڵ��ο�)
//DEL 		if(m_bIsUnityLoanService)
//DEL 		{
//DEL 			if(16==i) sGetData = _T("Y");
//DEL 		}

		// DM����
		m_pDM->SetCellData( _T("������"), sGetData, nNum );
		nNum++;
	}
	m_pCM->AllControlDisplay();

	// �׸��� ���� 
	CSettingGrid *pSet = new CSettingGrid();
	pSet->SetIsUnityLoanService(m_bIsUnityLoanService);
	pSet->SettingGrid01(m_pDM, m_pGrid);
	delete pSet;
	pSet = NULL;

	/*
	CString strData = _T("");
	INT ids = 0;

	//ù ��° 
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������Ļ�뿩��"),_T("�Ϲ�"),strData);
	if(_T("Y") == strData) m_nLOAN_TYPE0 = 0;
	else  m_nLOAN_TYPE0 = 1;
	strData.Empty();
	if(ids<0) return -1;

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������Ļ�뿩��"),_T("����"),strData);
	if(_T("Y") == strData) m_nLOAN_TYPE1 = 0;
	else  m_nLOAN_TYPE1 = 1;
	strData.Empty();
	if(ids<0) return -2;

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������Ļ�뿩��"),_T("Ư��"),strData);
	if(_T("Y") == strData) m_nLOAN_TYPE2 = 0;
	else  m_nLOAN_TYPE2 = 1;
	strData.Empty();
	if(ids<0) return -3;
	
	// 2005.09.27. ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++
	// ������� �߰� 
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������Ļ�뿩��"),_T("���"),strData);
	if(_T("Y") == strData) m_nLOAN_TYPE3 = 0;
	else  m_nLOAN_TYPE3 = 1;
	strData.Empty();
	if(ids<0) return -3;
	// _________________________________________________________________________

	//�ι�°
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ݳ����Ļ�뿩��"),_T("�Ϲ�"),strData);
	if(_T("Y") == strData) m_nRETURN_TYPE0 = 0;
	else  m_nRETURN_TYPE0 = 1;
	strData.Empty();
	if(ids<0) return -4;
	
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ݳ����Ļ�뿩��"),_T("�ϰ�"),strData);
	if(_T("Y") == strData) m_nRETURN_TYPE1 = 0;
	else  m_nRETURN_TYPE1 = 1;
	strData.Empty();
	if(ids<0) return -5;

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ݳ����Ļ�뿩��"),_T("�ұ�"),strData);
	if(_T("Y") == strData) m_nRETURN_TYPE2 = 0;
	else  m_nRETURN_TYPE2 = 1;
	strData.Empty();
	if(ids<0) return -6;

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("��������⿩��"),strData);
	if(_T("Y") == strData) m_nRELOAN_FAMILY = 0;
	else  m_nRELOAN_FAMILY = 1;
	strData.Empty();
	if(ids<0) return -1;

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("��������⿩��"),strData);
	if(_T("Y") == strData) m_nRELOAN_ONE = 0;
	else  m_nRELOAN_ONE = 1;
	strData.Empty();
	if(ids<0) return -2;

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("������̷º����ֱ⿩��"),strData);
	if(_T("Y") == strData) m_nReLoanView = 0;
	else  m_nReLoanView = 1;
	strData.Empty();
	if(ids<0) return -2;

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("�ٸ��������а����⿩��"),strData);
	if(_T("Y") == strData) m_nLOAN_MANAGE = 0;
	else  m_nLOAN_MANAGE = 1;
	strData.Empty();
	if(ids<0) return -5;

	/// 20040312 ������ ���� �ٸ��������а��ݳ����� �߰�
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("�ٸ��������а��ݳ�����"),strData);
	if(_T("Y") == strData) m_nRETURN_MANAG = 0;
	else  if(_T("N") == strData ) m_nRETURN_MANAG = 1;
	else m_nRETURN_MANAG = m_nLOAN_MANAGE;
	strData.Empty();
	if(ids<0) return -5;

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("�����������"),m_sReLoanLimitDay);
	if(ids<0) return -5;

	//�ι�°
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�̿��ڰ���"),_T("����ȸ����ɻ�뿩��"),strData);
	if(_T("Y") == strData) m_nUSER_FAMILY = 0;
	else  m_nUSER_FAMILY = 1;
	strData.Empty();
	if(ids<0) return -8;
	
	// 2005 07.27 ADD BY PDJ
	// �̿��� �Է½� �ֹε�Ϲ�ȣ �ڵ��ߺ��˻� �ɼ� �߰�
	ids = this->GetManageValue( _T("����"),_T("����"),_T("�̿��ڰ���"),_T("�ֹε�Ϲ�ȣ�ڵ��ߺ��˻翩��"),strData);
	if(_T("Y") == strData) m_nCIVILNO_CHECK_Y = 0;
	else  m_nCIVILNO_CHECK_Y = 1;
	strData.Empty();
	if(ids<0) return -11;
	ids = this->GetManageValue( _T("����"),_T("����"),_T("�̿��ڰ���"),_T("�����ڹ�ȣ�ڵ��ο�����"),strData);
	// ���� ����
	// N : ������ , A : �ֹι�ȣ�� ����, Y : ��������ȣ�� ����
	strData.TrimLeft();
	if( strData.IsEmpty() )
		return -9;
	switch( strData.GetAt( 0 ) )
	{
		case 'Y' :				// ��������ȣ�� ���
			m_cAutoNumType = 1;
			break;
		case 'A' :				// �ֹι�ȣ�� ���
			m_cAutoNumType = 2;
			break;
		case 'N' :				// ��� ���� 
		default :
			m_cAutoNumType = 0;
			break;
	}
	
	
	//if(_T("Y") == strData) m_nUSER_NUMBER = 0;
	//else  m_nUSER_NUMBER = 1;
	//strData.Empty();
	//if(ids<0) return -9;
	

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�̿��ڰ���"),_T("���������ϰ���������"),strData);
	if(_T("Y") == strData) m_nUSER_LOAN_STOP = 0;
	else  m_nUSER_LOAN_STOP = 1;
	strData.Empty();
	if(ids<0) return -10;

	// 2007.04.06 add by pdw {{+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �ηϰ���
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ηϰ���"),_T("�ηϴ��⿩��"),strData);
	if(_T("Y") == strData)
	{
		m_Appendix_Loan_Y = 0;
		OnradSETTINGAPPENDIXINLUDERETURNBOOKY();
	}
	else
	{
		m_Appendix_Loan_Y = 1;
		OnradSETTINGAPPENDIXINLUDERETURNBOOKN();
	}
	strData.Empty();
	if(ids<0) return -10;

	// �η��ڷ�ݳ� ��� �������
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ηϰ���"),_T("�ηϹݳ�����"),strData);
	if(_T("Y") == strData) 	m_Appendix_Return_Y = 0;		
	else	m_Appendix_Return_Y = 1;
	strData.Empty();
	if(ids<0) return -10;
	// -------------------------------------------------------------------------------}}

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ηϰ���"),_T("�ηϴ���Ǽ����Կ���"),strData);
	if(_T("Y") == strData) m_Appendix_Include_loan_book_Y = 0;
	else  m_Appendix_Include_loan_book_Y = 1;
	strData.Empty();
	if(ids<0) return -10;

	// ������ ��뿩��
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�̿��ڰ���"),_T("��������뿩��"),strData);
	if(_T("Y") == strData) m_UserCard_Y = 0;
	else  m_UserCard_Y = 1;
	strData.Empty();

	// ������ ��뿩��(Append_20041002_HAN...)
	ids = this->GetManageValue(_T("����"),_T("����"),_T("���������"),_T("��������»�뿩��"),strData);
	strData.TrimLeft( ); strData.TrimRight( );
	if ( strData.IsEmpty( ) ) strData = _T("N");
	if(_T("N") == strData)//������� ������ �ƿ� �������� �ʴ´�.
	{
		GetDlgItem(IDC_lblTITLE8)->ShowWindow(FALSE);
		GetDlgItem(IDC_lblFUNCTION18)->ShowWindow(FALSE);
		GetDlgItem(IDC_lblFUNCTION19)->ShowWindow(FALSE);
		GetDlgItem(IDC_radSETTING_LOAN_RECEIPT_USE_Y)->ShowWindow(FALSE);
		GetDlgItem(IDC_radSETTING_LOAN_RECEIPT_USE_N)->ShowWindow(FALSE);
		GetDlgItem(IDC_radSETTING_RETURN_RECEIPT_USE_Y)->ShowWindow(FALSE);
		GetDlgItem(IDC_radSETTING_RETURN_RECEIPT_USE_N)->ShowWindow(FALSE);
		GetDlgItem(IDC_lblDESC9)->ShowWindow(FALSE);

		CRect rect1, rect2 , rect3, rect4, rect5;

		CStatic* pStatic1 = (CStatic*)GetDlgItem(IDC_STATIC_BORDER);
		CStatic* pStatic2 = (CStatic*)GetDlgItem(IDC_imgDIVISION_LONG1);
		CStatic* pStatic3 = (CStatic*)GetDlgItem(IDC_imgDIVISION_LONG2);
		CStatic* pStatic4 = (CStatic*)GetDlgItem(IDC_imgDIVISION_LONG3);
		CStatic* pStatic5 = (CStatic*)GetDlgItem(IDC_imgDIVISION_WIDTH);

		pStatic1->GetWindowRect( &rect1 );
		pStatic2->GetWindowRect( &rect2 );
		pStatic3->GetWindowRect( &rect3 );
		pStatic4->GetWindowRect( &rect4 );
		pStatic5->GetWindowRect( &rect5 );

		ScreenToClient( rect1 );
		ScreenToClient( rect2 );
		ScreenToClient( rect3 );
		ScreenToClient( rect4 );
		ScreenToClient( rect5 );

		INT nBorderHeight = rect1.Height( ) - (rect1.bottom - rect5.bottom);

		//��¿ �� ���� �״�� �������.
		pStatic1->MoveWindow( 10, 0, rect1.Width( ), nBorderHeight );
		pStatic2->MoveWindow( 115, 8, rect2.Width( ), nBorderHeight-8 );
		pStatic3->MoveWindow( 115+180, 8, rect3.Width( ), nBorderHeight-8 );
		pStatic4->MoveWindow( 330+90, 8, rect4.Width( ), nBorderHeight-8 );
		GetDlgItem(IDC_imgDIVISION_WIDTH)->ShowWindow(FALSE);
	}
	else
	{
		// ������ ����� ����ϸ� ������°��� �����´�.
		strData.Empty();
		ids = this->GetManageValue(_T("����"),_T("����"),_T("���������"),_T("��������»������"),strData);
		if ( 'Y' == strData.GetAt(0) )	m_nLOAN_RECEIPT = 0;
		else	m_nLOAN_RECEIPT = 1;
		if ( 'Y' == strData.GetAt(1) )	m_nRETURN_RECEIPT = 0;
		else	m_nRETURN_RECEIPT = 1;
		strData.Empty();
		if(ids<0) return -10;
	}
	if(ids<0) return -10;
	
	// 2005.09.27 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++
	// �������Ŀ� ���� ���� ������ �׸� �߰� (���� ��쿡�� �����ش�)
	//										   (���� ������ġ�� �ű��.)
	ids = this->GetManageValue( _T("��Ÿ") , _T("����") , _T("����") , _T("����") , strData);
	if ( strData == _T("Y") )
	{
		// ������ �׷� �����ֱ� 
		GetDlgItem(IDC_COMMON_LOAN_TYPE_4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LOAN_TYPE_EXP_2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_radSETTING_LOAN_TYPE_3_Y)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_radSETTING_LOAN_TYPE_3_N)->ShowWindow(SW_SHOW);
		// ���� �������� ��ġ �ű��
		CWnd* pWnd[10];
		pWnd[0] = GetDlgItem(IDC_COMMON_LOAN_TYPE_1);pWnd[1] = GetDlgItem(IDC_COMMON_LOAN_TYPE_2);
		pWnd[2] = GetDlgItem(IDC_COMMON_LOAN_TYPE_3);pWnd[3] = GetDlgItem(IDC_LOAN_TYPE_EXP_1);
		pWnd[4] = GetDlgItem(IDC_radSETTING_LOAN_TYPE_0_Y);pWnd[5] = GetDlgItem(IDC_radSETTING_LOAN_TYPE_0_N);
		pWnd[6] = GetDlgItem(IDC_radSETTING_LOAN_TYPE_1_Y);pWnd[7] = GetDlgItem(IDC_radSETTING_LOAN_TYPE_1_N);
		pWnd[8] = GetDlgItem(IDC_radSETTING_LOAN_TYPE_2_Y);pWnd[9] = GetDlgItem(IDC_radSETTING_LOAN_TYPE_2_N);

		CRect rc;

		for (INT i = 0;i < 10;i++)
		{
			pWnd[i]->GetWindowRect(&rc);
			this->ScreenToClient(&rc);
			rc.top = rc.top -8;rc.bottom = rc.bottom - 8;
			pWnd[i]->MoveWindow(&rc);
		}		
	}
	if (ids<0) return -10;
	// ______________________________________________________________________
	UpdateData(FALSE);

	if( ((CButton*)GetDlgItem(IDC_radSETTING_RELOAN_FAMILY_N))->GetCheck()==1)
		OnradSETTINGRELOANFAMILYN();
	*/

	return 0;

EFS_END
return -1;
}

INT CMngLocMgr01Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	// 17/05/18 ������ : ȸ���� ���������� �ź� Ȯ���� ���Ͽ� ����������� �߰�
/*//BEFORE-----------------------------------------------------------------------------
	const int nCnt = 23;
*///AFTER------------------------------------------------------------------------------
	//const int nCnt = 24;
/*//END-------------------------------------------------------------------------------*/
	//JOB.2019.0033 ADD BY KYJ
	const int nCnt = 25;
	// Alias
	CString sAlias[nCnt] = 
			{	_T("�������Ļ�뿩��"), _T("�������Ļ�뿩��"), _T("�������Ļ�뿩��"), 
				_T("�������Ļ�뿩��"), _T("�ݳ����Ļ�뿩��"), _T("�ݳ����Ļ�뿩��"),	
				_T("�ݳ����Ļ�뿩��"), _T("�������"),			_T("�������"), 
				_T("�������"),			_T("�������"), 		_T("�������"), // JOB.2019.0033 ADD BY KYJ
				_T("�ηϰ���"),			
				_T("�ηϰ���"), 		_T("�ηϰ���"),			_T("�̿��ڰ���"),		
				_T("�̿��ڰ���"), 		_T("�̿��ڰ���"),		
				_T("�̿��ڰ���"), 		_T("�̿��ڰ���"),	// 2009.04.21 ADD BY KSJ
				// 17/05/18 ������ : ȸ���� ���������� �ź� Ȯ���� ���Ͽ� ����������� �߰�
/*//BEFORE-----------------------------------------------------------------------------
				_T("�̿��ڰ���"),		_T("���������"), _T("��ϱ���"), _T("����ڷ����") 
*///AFTER------------------------------------------------------------------------------
				_T("�̿��ڰ���"),      _T("�̿��ڰ���"),
				_T("���������"),	   _T("��ϱ���"),		   _T("����ڷ����") 
/*//END-------------------------------------------------------------------------------*/
			};
	// ������
	CString sManage[nCnt] = 
			{	_T("�Ϲ�"), _T("����"), _T("Ư��"),	_T("���"), _T("�Ϲ�"), _T("�ϰ�"),	_T("�ұ�"), 
				_T("��������⿩��"),		_T("��������⿩��"),			_T("������̷º����ֱ⿩��"), 
				_T("�����������"),			_T("Ÿ�ý���ī���ȣ��뿩��"),	 //JOB.2019.0033 ADD BY KYJ //12
				_T("�ηϴ��⿩��"),			_T("�ηϹݳ�����"), 			_T("�ηϴ���Ǽ����Կ���"),			
				_T("����ȸ����ɻ�뿩��"),	_T("���������ϰ���������"),		_T("�����ڹ�ȣ�ڵ��ο�����"),
				_T("�����ڹ�ȣ�����ID���ڵ��Է�"),	_T("�ֹε�Ϲ�ȣ���ڸ���й�ȣ���ڵ��Է�"),	// 2009.04.21 ADD BY KSJ
				// 17/05/18 ������ : ȸ���� ���������� �ź� Ȯ���� ���Ͽ� ����������� �߰�
/*//BEFORE-----------------------------------------------------------------------------
				_T("�ֹε�Ϲ�ȣ�ڵ��ߺ��˻翩��"),	_T("��������»������"), _T("�������� ��ϱ��� �ߺ�����"),
*///AFTER------------------------------------------------------------------------------
				_T("�ֹε�Ϲ�ȣ�ڵ��ߺ��˻翩��"),	 _T("����������ȸ��뿩��"),
				_T("��������»������"),			 _T("�������� ��ϱ��� �ߺ�����"),
/*//END-------------------------------------------------------------------------------*/
				_T("�⺻ �������")
			};

	CString sCode;
	CString sData;
	INT nNum = 0;
	for( INT i = 0; i < nCnt; i++ )
	{
		// �������� ���翡���� ���̵��� �Ѵ�
		if( sManage[i] == _T("���") )
		{
			ids = this->GetManageValue( _T("��Ÿ"), _T("����"), _T("����"), _T("����"), sData );
			sData.TrimLeft( ); sData.TrimRight( );
			if ( sData != _T("Y") ) continue;
		}
		// ��������� ��������� ���� �׸��忡 ǥ��
		else if( sManage[i] == _T("��������»������") )
		{
			ids = this->GetManageValue( _T("����"), _T("����"), _T("���������"), _T("��������»�뿩��"), sData );
			sData.TrimLeft( ); sData.TrimRight( );
			if ( sData != _T("Y") ) continue;
		}

		m_pDM->GetCellData( _T("������"), nNum, sCode );
		nNum++;

		ids = this->SetManageValue( _T("����"), _T("����"), sAlias[i], sManage[i], sCode );
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("������������ �����ϴµ� �����Ͽ����ϴ�."));
			return -1;
		}
	}
	/*	
	UpdateData(TRUE);
	
	CString strData = _T("");
	INT ids;

	//ù ��° 
	if(0 == m_nLOAN_TYPE0) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�������Ļ�뿩��"),_T("�Ϲ�"),strData);
	strData.Empty();
	if(ids<0) return -1;

	if(0 == m_nLOAN_TYPE1) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�������Ļ�뿩��"),_T("����"),strData);
	strData.Empty();
	if(ids<0) return -2;

	if(0 == m_nLOAN_TYPE2) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�������Ļ�뿩��"),_T("Ư��"),strData);
	strData.Empty();
	if(ids<0) return -3;
	
	// 2005.09.27 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++
	// ��� ���� �߰� 
	if(0 == m_nLOAN_TYPE3) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�������Ļ�뿩��"),_T("���"),strData);
	strData.Empty();
	if(ids<0) return -7;
	// _________________________________________________________________________

	//�ι�°
	if(0 == m_nRETURN_TYPE0) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ݳ����Ļ�뿩��"),_T("�Ϲ�"),strData);
	strData.Empty();
	if(ids<0) return -4;

	if(0 == m_nRETURN_TYPE1) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ݳ����Ļ�뿩��"),_T("�ϰ�"),strData);
	strData.Empty();
	if(ids<0) return -5;

	if(0 == m_nRETURN_TYPE2) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ݳ����Ļ�뿩��"),_T("�ұ�"),strData);
	strData.Empty();
	if(ids<0) return -6;


	if(0 == m_nRELOAN_FAMILY) strData = _T("Y");
	else strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("��������⿩��"),strData);
	if(ids<0) return -1;

	if(0 == m_nRELOAN_ONE) strData = _T("Y");
	else strData = _T("N");
	ids =this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("��������⿩��"),strData);
	if(ids<0) return -2;

	if(0 == m_nReLoanView) strData = _T("Y");
	else strData = _T("N");
	ids =this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("������̷º����ֱ⿩��"),strData);
	if(ids<0) return -2;
	
	if(0 == m_nLOAN_MANAGE) strData = _T("Y");
	else strData = _T("N");
	ids =this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("�ٸ��������а����⿩��"),strData);
	if(ids<0) return -5;

	if(0 == m_nRETURN_MANAG) strData = _T("Y");
	else strData = _T("N");
	ids =this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("�ٸ��������а��ݳ�����"),strData);
	if(ids<0) return -5;

	strData = m_sReLoanLimitDay;
	//++2008.06.16 ADD BY PWR {{++
	// ����� ������ 0 ~ 100���� ���� ���� ����
	if( _ttoi(m_sReLoanLimitDay) < 0 || _ttoi(m_sReLoanLimitDay) > 100 )
	{
		AfxMessageBox( _T("����� �������� 0���� 100������ ������ �Է��Ͻʽÿ�.") );
		// ����� �����Ͽ� ��Ŀ��
		m_edtReLoanLimitDay.SetFocus();
		// ����� �����Ͽ� �����
		INT nLen = m_sReLoanLimitDay.GetLength();
		m_edtReLoanLimitDay.SetSel(0, nLen);		
		return 1000;
	}
	//--2008.06.16 ADD BY PWR --}}
	ids =this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("�����������"),strData);
	if(ids<0) return -5;

	//�ι�°
	if(0 == m_nUSER_FAMILY) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�̿��ڰ���"),_T("����ȸ����ɻ�뿩��"),strData);
	strData.Empty();
	if(ids<0) return -8;

	// 2005 07.27 ADD BY PDJ
	// �̿��� �Է½� �ֹε�Ϲ�ȣ �ڵ��ߺ��˻� �ɼ� �߰�
	if(0 == m_nCIVILNO_CHECK_Y) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�̿��ڰ���"),_T("�ֹε�Ϲ�ȣ�ڵ��ߺ��˻翩��"),strData);
	strData.Empty();
	if(ids<0) return -8;

	
	//if(0 == m_nUSER_NUMBER) strData = _T("Y");
	//else  strData = _T("N");
		
	// ������� 
	// N : ������ , A : �ֹι�ȣ�� ����, Y : ��������ȣ�� ����
	switch( m_cAutoNumType )
	{
	case 1 :
		strData = _T("Y");
		break;
	case 2 :
		strData = _T("A");
		break;
	case 0 :
	default :
		strData = _T("N");
		break;
	}
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�̿��ڰ���"),_T("�����ڹ�ȣ�ڵ��ο�����"),strData);
	strData.Empty();
	if(ids<0) return -9;

	if(0 == m_nUSER_LOAN_STOP) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�̿��ڰ���"),_T("���������ϰ���������"),strData);
	strData.Empty();
	if(ids<0) return -10;

	// �ηϰ���
	if(0 == m_Appendix_Loan_Y) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ηϰ���"),_T("�ηϴ��⿩��"),strData);
	if(ids<0) return -11;
	// 2007.04.06 add by pdw {{++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �η��ڷ�ݳ�
	if(0 == m_Appendix_Return_Y) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ηϰ���"),_T("�ηϹݳ�����"),strData);
	if(ids<0) return -11;
	// ------------------------------------------------------------------------------}}

	if(0 == m_Appendix_Include_loan_book_Y) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ηϰ���"),_T("�ηϴ���Ǽ����Կ���"),strData);
	if(ids<0) return -12;

	// ��������뿩��
	if(0 == m_UserCard_Y) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�̿��ڰ���"),_T("��������뿩��"),strData);
	if(ids<0) return -12;

	// ������ ��뿩��(Append_20041002_HAN...)
	if(0 == m_nLOAN_RECEIPT) strData = _T("Y");
	else  strData = _T("N");
	if(0 == m_nRETURN_RECEIPT) strData += _T("Y");
	else  strData += _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("���������"),_T("��������»������"),strData);
	if(ids<0) return -12;
	*/
	return 0;

EFS_END
return -1;
}

VOID CMngLocMgr01Dlg::OnradSETTINGRELOANONEN() 
{
	EFS_BEGIN

	EFS_END
}

VOID CMngLocMgr01Dlg::OnradSETTINGRELOANONEY() 
{
	EFS_BEGIN

	EFS_END
}

VOID CMngLocMgr01Dlg::OnradSETTINGRELOANFAMILYY() 
{
	EFS_BEGIN

	((CButton*)GetDlgItem(IDC_radSETTING_RELOAN_ONE_Y))->EnableWindow(TRUE);
	
	EFS_END
}

VOID CMngLocMgr01Dlg::OnradSETTINGRELOANFAMILYN() 
{
	EFS_BEGIN

	((CButton*)GetDlgItem(IDC_radSETTING_RELOAN_FAMILY_N))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_radSETTING_RELOAN_ONE_Y))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_radSETTING_RELOAN_ONE_Y))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_radSETTING_RELOAN_ONE_N))->SetCheck(1);

	EFS_END
}


/// ��ٱ��� DM���� VALUE�� �̾Ƴ��� �Լ�
INT CMngLocMgr01Dlg::GetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
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
INT CMngLocMgr01Dlg::SetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
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
		pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") , TRUE );
		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("����/�ݳ�����&�̿���"), __WFILE__, __LINE__ ), TRUE );
		
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
	if( ids < 0 ) 
	{
		CString sEMsg;
		sEMsg.Format( _T("Error Func: pDM->SetCellData( VALUE , %s , %d );") , sValue , nIndex );
		AfxMessageBox( sEMsg );
	}
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetManageValue") );

	ids = CESL_Mgr::SetManageValue( sGroup2 , sGroup3 , sAlias , sValueName , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetManageValue") );

	return 0;

	EFS_END
	return -1;

}
                      
VOID CMngLocMgr01Dlg::OnradSETTINGAPPENDIXINLUDERETURNBOOKY() 
{
	
}

VOID CMngLocMgr01Dlg::OnradSETTINGAPPENDIXINLUDERETURNBOOKN() 
{
	
}

VOID CMngLocMgr01Dlg::OnradSETTINGAPPENDIXLOANY() 
{
	// �ηϴ����� ����Ѵٸ� 
	// �ηϹݳ��� able�� �����Ѵ�.
	const INT nCtrlCnt = 2;
	INT       nCtrlID[nCtrlCnt] = { 
		IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_Y , 
		IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_N
	};

	CWnd* pWnd = NULL;
	for( INT i=0;i<nCtrlCnt;i++ )
	{
		pWnd = GetDlgItem(nCtrlID[i]);
		if( pWnd )
		{
			pWnd->EnableWindow();
		}
	}
}

VOID CMngLocMgr01Dlg::OnradSETTINGAPPENDIXLOANN() 
{
	// �ηϴ����� �����Ѵٸ�
	// �ηϹݳ��� enable�� �����ϸ� ���� N���� �ٲ۴�.
	const INT nCtrlCnt = 2;
	INT       nCtrlID[nCtrlCnt] = { 
		IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_Y , 
		IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_N
	};

	CWnd* pWnd = NULL;
	for( INT i=0;i<nCtrlCnt;i++ )
	{
		pWnd = GetDlgItem(nCtrlID[i]);
		if( pWnd )
		{
			pWnd->EnableWindow(FALSE);
		}
	}

	((CButton*)GetDlgItem(IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_Y))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_N))->SetCheck(1);
}

HBRUSH CMngLocMgr01Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CMngLocMgr01Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_LOC_MGR_01);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 1, cx, cy-10);	
}

BEGIN_EVENTSINK_MAP(CMngLocMgr01Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr01Dlg)
	ON_EVENT(CMngLocMgr01Dlg, IDC_gridMNG_LOC_MGR_01, -600 /* Click */, OnClickgridMNGLOCMGR01, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMngLocMgr01Dlg::OnClickgridMNGLOCMGR01() 
{
	if( 0 == m_pGrid->GetMouseRow() ) 
	{
		return;
	}

	INT col = m_pGrid->GetCol();
	if( 4 == col ) 
	{
		CreateComboBoxInGridSell();
	}
}

// ** �޺��ڽ� ���� �� �� ����
VOID CMngLocMgr01Dlg::CreateComboBoxInGridSell()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if( 0 == nRowCnt ) 
	{
		return ;
	}

	INT nRow = m_pGrid->GetRow();
	CString strData = m_pDM->GetCellData( _T("������"), nRow - 1 );
	CString strElement = m_pDM->GetCellData( _T("���"), nRow - 1 );

	if( m_pComboBox )
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	
	CStringArray ArrCmbData;	ArrCmbData.RemoveAll();

	/// ���������, �η��ڷ���� ��뿩�ο� ���� ǥ��
	if( strElement == _T("������������") || strElement == _T("�η��ڷ�ݳ�") )
	{
		CString sDelay = m_pDM->GetCellData( _T("������"), nRow-2 );
		if( _T("Y") != sDelay ) 
		{
			return;
		}
	}

	// 2010.12.22 DELETE BY KSJ : �Ϲ�ȸ���̶�� �����ڹ�ȣ �ڵ��ο� ������ ��� �� �� �ֵ��� ����
	// 2010.07.28 ADD BY KSJ : ���� ȸ�� ���� �ý��ۿ��� �����ڹ�ȣ �ڵ��ο� ���� �ٲ� �� ����
//DEL 	if(m_bIsUnityLoanService)
//DEL 	{
//DEL 		if(16 == nRow) return;
//DEL 	}

	// ������ ������
	//JOB.2019.0033 WORK BY KYJ 
	//if( 10 == nRow )
	if( _T("�����������") == strElement )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl[0]);
		return;
	}
	// �����ڹ�ȣ �ڵ��ο�
	//JOB.2019.0033 WORK BY KYJ 
	//else if( 16 == nRow )
	else if( _T("�����ڹ�ȣ �ڵ��ο�") == strElement )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("������"));
		ArrCmbData.Add(_T("��������ȣ"));
		ArrCmbData.Add(_T("�ֹε�Ϲ�ȣ"));
		ArrCmbData.Add(_T("�����ID"));
	}
	// 2009.04.21 ADD BY KSJ : �����ڹ�ȣ�����ID���ڵ��Է�
	//JOB.2019.0033 WORK BY KYJ 
	//else if( 17 == nRow )
	else if( _T("�����ڹ�ȣ �����ID�� �ڵ��Է�") == strElement )
	{
		// 2009.10.19 ADD BY KSJ : �����ڹ�ȣ �ڵ��ο� -> �����ID �� ���
		CString strValue = m_pDM->GetCellData( _T("������"), nRow-2 );
		if( _T("U") == strValue ) 
		{
			return;
		}

		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("������"));
		ArrCmbData.Add(_T("�����"));
	}	
	// 2009.04.21 ADD BY KSJ : �ֹε�Ϲ�ȣ���ڸ���й�ȣ���ڵ��Է�
	//JOB.2019.0033 EDIT BY KYJ
	//else if( 18 == nRow )
	else if( _T("�ֹε�Ϲ�ȣ ���ڸ� ��й�ȣ�� �ڵ��Է�") == strElement )
	{
		// 2009.10.19 ADD BY KSJ : �����ڹ�ȣ �ڵ��ο� -> �����ID / �����ڹ�ȣ�����ID���ڵ��Է� �ɼ� �Ѵ� ��� �� �� ���
		//JOB.2019.0033 WORK BY KYJ
		//-------------------------------------------------------------------------------
		//CString strValue1 = m_pDM->GetCellData( _T("������"), nRow-3 );
		//CString strValue2 = m_pDM->GetCellData( _T("������"), nRow-2 );
		//-------------------------------------------------------------------------------
		CString strTempVal = _T("") , strValue1 = _T("") , strValue2 = _T("") ;
		for(int k = 0 ; k < m_pDM->GetRowCount() ;  k++){
			strTempVal = m_pDM->GetCellData(_T("���"), k );
			if (_T("�����ڹ�ȣ �ڵ��ο�") == strTempVal)
			{
				strValue1 = m_pDM->GetCellData( _T("������") , k );
			}
			else if ( _T("�����ڹ�ȣ �����ID�� �ڵ��Է�")  == strTempVal )
			{
				strValue2 = m_pDM->GetCellData( _T("������") , k );
			}
		}
		//-------------------------------------------------------------------------------

		if( _T("U") != strValue1 && _T("Y") != strValue2 )
		{
			return;
		}

		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("������"));
		ArrCmbData.Add(_T("�����"));
	}
	// ��������»��
	//else if( nRow == 18 )
// 	else if( 20 == nRow )
	else if(_T("��������»��") == strElement)
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("����,�ݳ� ���"));
		ArrCmbData.Add(_T("�����϶���"));
		ArrCmbData.Add(_T("�ݳ��϶���"));
		ArrCmbData.Add(_T("������"));
	}
// 	else if( 21 == nRow )
	else if(_T("�������� ��ϱ��� �ߺ�����") == strElement)
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("������"));
		ArrCmbData.Add(_T("�����"));
	}
	// �Ϲݴ���,�ݳ��� ������ �����
	else if( _T("�Ϲݴ�����") == strElement || _T("�Ϲݹݳ����") == strElement )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("�����"));
	}
	else if(_T("�⺻ �������") == strElement)
	{
		ShowEditBox(m_pGrid, m_pEditCtrl[1]);
		return;
	}
	else
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("�����"));
		ArrCmbData.Add(_T("������"));
	}

	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), m_pGrid, 6450);
	m_pComboBox->Init(&ArrCmbData);
	m_pComboBox->InitMngLocMgr(1);
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

INT CMngLocMgr01Dlg::ShowEditBox(CESL_Grid *pGrid, CEdit* pEditCtrl)
{
EFS_BEGIN

	pEditCtrl->GetSafeHwnd();

	if ( NULL == pGrid ) 
	{
		return -1;
	}

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if ( 1 > nRow || 1 > nCol ) 
	{
		return -1;
	}

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	// �ؽ�Ʈ ����
	CString sValue = pGrid->GetTextMatrix(nRow, nCol);
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

INT CMngLocMgr01Dlg::GetCellRect(CESL_Grid *pGrid, CRect &rect)
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

INT CMngLocMgr01Dlg::ApplyDataChange(LPCTSTR lpszData)
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

	// �׸��� ����
	CSettingGrid *pSet = new CSettingGrid();
	pSet->SetIsUnityLoanService(m_bIsUnityLoanService);
	pSet->SettingGrid01(m_pDM, m_pGrid);
	delete pSet;
	pSet = NULL;

	return 0;

EFS_END
return -1;
}

void CMngLocMgr01Dlg::OnKillfocusedtRELOANLIMITDAY() 
{
	SetFocus();
	if (NULL == m_pEditCtrl[0]) return ;
	if (!::IsWindowVisible(m_pEditCtrl[0]->GetSafeHwnd())) return ;

	m_pEditCtrl[0]->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl[0]->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}

void CMngLocMgr01Dlg::OnKillfocusedt2()
{
	SetFocus();
	if (NULL == m_pEditCtrl[1]) return ;
	if (!::IsWindowVisible(m_pEditCtrl[1]->GetSafeHwnd())) return ;

	m_pEditCtrl[1]->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl[1]->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}
INT CMngLocMgr01Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
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
