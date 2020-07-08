// MngLocMgr02Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr02Dlg.h"
#include "ChangeValue.h"
#include "SettingGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr02Dlg dialog


CMngLocMgr02Dlg::CMngLocMgr02Dlg(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr02Dlg)
	m_nRETURN_DELAY_USE = -1;
	m_nRETURN_DELAY_AUTO = -1;
	m_nRESERVE_AUTO_CANCEL = -1;
	m_nRESERVE_EXPIRE_AUTO = -1;
	m_nSEND_CANCEL_MAIL = -1;
	m_nAutoReserveComplete = -1;
	m_nAddHolydayReservation = -1;
	m_nRETURN_SYSDATE_DELAY_USE = -1;
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	// 2005.05.26
	m_strUserID=pParent->GetUserID();
	m_strLibCode=_T("");

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;
	m_pEditCtrl1 = NULL;
	m_pEditCtrl2 = NULL;
}
CMngLocMgr02Dlg::~CMngLocMgr02Dlg()
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

	
}

VOID CMngLocMgr02Dlg::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr02Dlg)
	DDX_Radio(pDX, IDC_radSETTING_RESERVE_USE_Y2, m_nRETURN_DELAY_USE);
	DDX_Radio(pDX, IDC_radSETTING_RESERVE_ATUO_CANCEL_Y2, m_nRETURN_DELAY_AUTO);
	DDX_Radio(pDX, IDC_radSETTING_RESERVE_ATUO_CANCEL_Y, m_nRESERVE_AUTO_CANCEL);
	DDX_Radio(pDX, IDC_radSETTING_RESERVE_EXPIRE_ATUO_Y, m_nRESERVE_EXPIRE_AUTO);
	DDX_Radio(pDX, IDC_radSETTING_RESERVE_ATUO_CANCEL_Y3, m_nSEND_CANCEL_MAIL);
	DDX_Radio(pDX, IDC_radSETTING_RESERVE_ATUO_CANCEL_Y4, m_nAutoReserveComplete);
	DDX_Radio(pDX, IDC_radSETTING_ADD_HOLYDAY_RESERVATION1, m_nAddHolydayReservation);
	DDX_Radio(pDX, IDC_radSETTING_SYSDATE_RESERVE_USE_Y, m_nRETURN_SYSDATE_DELAY_USE);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CMngLocMgr02Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr02Dlg)
	ON_CBN_SELCHANGE(IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN2, OnSelchangecmbSETTINGDELAYORRESERVERETURN2)
	ON_BN_CLICKED(IDC_radSETTING_RESERVE_USE_N2, OnradSETTINGRESERVEUSEN2)
	ON_BN_CLICKED(IDC_radSETTING_RESERVE_USE_Y2, OnradSETTINGRESERVEUSEY2)
	ON_WM_CTLCOLOR()  
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_edtDELAYRETURN_CNT, OnKillfocusedtDELAYRETURNCNT)
	ON_EN_KILLFOCUS(IDC_edtLIMIT, OnKillfocusedtLIMIT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr02Dlg message handlers

BOOL CMngLocMgr02Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_����ȯ�漳��_�ݳ����⿹��ȯ��")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_����ȯ�漳��_�ݳ����⿹��ȯ��"));
	m_pDM = FindDM(_T("DM_����ȯ�漳��_�ݳ����⿹��ȯ��"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	// �ݳ�����Ƚ��
	m_pEditCtrl1 = new CEdit();
	m_pEditCtrl1->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_edtDELAYRETURN_CNT);
	m_pEditCtrl1->ShowWindow(SW_HIDE);
	// ��������ο�
	m_pEditCtrl2 = new CEdit();
	m_pEditCtrl2->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_edtLIMIT);
	m_pEditCtrl2->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMngLocMgr02Dlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CMngLocMgr02Dlg::ViewScreen()
{
EFS_BEGIN

	INT ids;
	const int nCnt = 14;
	// ����
	CString sSection[nCnt] = 
			{	_T("�ݳ��������"), _T("�ݳ��������"), _T("�ݳ��������"), 
				_T("�ݳ��������"), _T("�ݳ��������"), _T("�ݳ��������"),	
				_T("�������"), _T("�������"), _T("�������"), 
				_T("�������"),	_T("�������"), _T("�������"), 
				_T("�������"), _T("�������") };
	// ���
	CString sAbility[nCnt] = 
			{	_T("�ݳ������뿩��"),		_T("���ϴ����ڷ� �ݳ��������"), 
				_T("�ݳ������� �ڵ��ο�"),	_T("�ݳ������ ����켱���� ����"), 
				_T("�ݳ����� ����Ƚ��"),	_T("�ݳ������� ���� ������"), 
				_T("�����ڵ����"),			_T("���ุ���� �ڵ��ο�"), 
				_T("���ุ���Ϻο��� SMS�߼ۿ���"),
				_T("������ ��뿩��"),	_T("���ุ���� �ް��� ��������"), 
				_T("��������ο�"),			_T("������� �ȳ����� �߼ۿ���"), 
				_T("���ټ��⿹����")};
	// ����
	CString sExplain[nCnt] = 
			{	_T("����/�ݳ� �޴����� �ݳ����� ����� ��뿩�� ����"), 
				_T("���� ������ �ڷ��� �ݳ����� ��뿩��"), 
				_T("�ݳ������ ���޺��̳� ��ϱ��к��� �����Ǿ� �ִ� �⺻ �ݳ��������� ����"),
				_T("�ݳ����� ����ڷᰡ �̹� ����� �ڷ��̰� ����켱�̶�� �ݳ����� ��� ������"),
				_T("�ݳ����� ��� �ڷ��� �ݳ����� ����Ƚ��"), 
				_T("�ݳ��������� �����Ͽ� �ջ�ǰų� �ݳ������Ͽ� �ջ�Ǿ� �ݳ��������� ����"),
				_T("����/�ݳ� �޴� ���� ���ุ������ ���� �����ڷῡ ���� �ڵ����� ������� ���ΰ���"), 
				_T("���ุ���� �ο��� ���ุ������ �ڵ����� �������� ����ڰ� ���� �Է����� ����"),
				_T("���ุ������ �ο��Ǹ� SMS�� �ڵ����� �߼����� ����"),
				_T("�������� �ڷῡ�� ������ �����ϰ� �ϰų� ��ġ, �������� �ڷῡ ������ �����ϰ��Ѵ�"),
				_T("���ุ���Ͽ� �ް����� �������� ���� (���ุ����+�ް���=���ο�ุ����)"), 
				_T("�� �ڷῡ ���� ���డ���� �̿��� ��"), 
				_T("����/�ݳ� ȭ�鿡�� ����� �ڷḦ ����� ��� ������� �ȳ����Ϲ߼� ���ΰ���"), 
				_T("����� �ڷḦ �ݳ��� �ڵ����� �����ڷ�����, ���ุ���� �ο�, ���Ϲ߼ۿ��� ����")	};
	// ������
	CString sManage[nCnt] = 
			{	_T("�ݳ������ɻ�뿩��"),	_T("���Ϲݳ������������"), 
				_T("�ݳ������ڵ������ο�"),	_T("�ݳ����⿹��켱����"), 
				_T("�ݳ����Ⱑ��Ƚ��"),		_T("�ݳ�����ο�����"), 
				_T("�����ڵ���ҿ���"), 	_T("���ุ�����ڵ��ο�����"), 
				_T("���ุ���Ϻο���SMS�߼ۿ���"),
				_T("�����ɻ�뿩��"), 	_T("���ุ���ް�����������"), 
				_T("��������ο�"), 		_T("������ҽø��Ϲ߼ۿ���"), 
				_T("�ڵ������ڷ���������") };

	// Display
	m_pDM->FreeData();
	for( INT i = 0; i < nCnt; i++ )
	{
		// ���ټ��⿹������ CO_UTIL_LOCK_TBL�� �������� ���� �����ش�.
		if( sAbility[i] == _T("���ټ��⿹����") )
		{
			CString sData;
			CString sQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'ONELNERESERV'");
			ids = m_pDM->GetOneValueQuery( sQuery, sData );
			if ( (ids<0) && (-4007!=ids) ) 
			{
				ESLAfxMessageBox(_T("������������ �������µ� �����Ͽ����ϴ�."));
				return -1;
			}
			if( sData != _T("Y") ) break;
		}
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
		
		// DM����
		m_pDM->SetCellData( _T("������"), sGetData, i );
	}
	m_pCM->AllControlDisplay();

	// �׸��� ���� 
	CSettingGrid *pSet = new CSettingGrid();
	pSet->SettingGrid02(m_pDM, m_pGrid);
	delete pSet;
	pSet = NULL;

	/*
	CString strData = _T("");
	INT ids = 0;

	// 2007.07.13 add by pdw
	// ���ุ���Ͽ� �ް��� ��������
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("���ุ���ް�����������"),strData);
	if(_T("Y") == strData) m_nAddHolydayReservation = 0;
	else  m_nAddHolydayReservation = 1;
	strData.Empty();
	if(ids<0) return -7;

	//����° 
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ݳ��������"),_T("�ݳ������ɻ�뿩��"),strData);
	if(_T("Y") == strData) m_nRETURN_DELAY_USE = 0;
	else  m_nRETURN_DELAY_USE = 1;
	strData.Empty();
	if(ids<0) return -7;

	//++2008.06.06 ADD BY PWR {{++
	// ���Ϲݳ������������ ������ �߰�
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ݳ��������"),_T("���Ϲݳ������������"),strData);
	if(_T("Y") == strData) m_nRETURN_SYSDATE_DELAY_USE = 0;
	else  m_nRETURN_SYSDATE_DELAY_USE = 1;
	strData.Empty();
	if(ids<0) return -7;
	//--2008.06.06 ADD BY PWR --}}

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ݳ��������"),_T("�ݳ������ڵ������ο�"),strData);
	if(_T("Y") == strData) m_nRETURN_DELAY_AUTO = 0;
	else  m_nRETURN_DELAY_AUTO = 1;
	strData.Empty();
	if(ids<0) return -8;

	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN);
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ݳ��������"),_T("�ݳ����⿹��켱����"),strData);
	if(_T("0") == strData) pCombo->SetCurSel(0);
	else pCombo->SetCurSel(1);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -9;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtDELAYRETURNCNT);
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ݳ��������"),_T("�ݳ����Ⱑ��Ƚ��"),strData);
	pEdit->SetWindowText(strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -10;

	pCombo = (CComboBox*)GetDlgItem(IDC_cmbRETURN_PLAN_DAY_DELAY_OPTION);
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�ݳ��������"),_T("�ݳ�����ο�����"),strData);
	if(_T("0") == strData) pCombo->SetCurSel(0);
	else pCombo->SetCurSel(1);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -9;

	//�׹�°
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("�����ڵ���ҿ���"),strData);
	if(_T("Y") == strData) m_nRESERVE_AUTO_CANCEL = 0;
	else  m_nRESERVE_AUTO_CANCEL = 1;
	strData.Empty();
	if(ids<0) return -11;
		
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("���ุ�����ڵ��ο�����"),strData);
	if(_T("Y") == strData) m_nRESERVE_EXPIRE_AUTO = 0;
	else  m_nRESERVE_EXPIRE_AUTO = 1;
	strData.Empty();
	if(ids<0) return -8;

	pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN2);
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("�����ɻ�뿩��"),strData);
	if(_T("0") == strData) pCombo->SetCurSel(0);
	else if(_T("1") == strData) pCombo->SetCurSel(1);
	else pCombo->SetCurSel(2);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -12;

	pEdit = (CEdit*)GetDlgItem(IDC_edtDELAYRETURNCNT2);
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("��������ο�"),strData);
	pEdit->SetWindowText(strData);
	pEdit = NULL;
	if(ids<0) return -13;

	ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("������ҽø��Ϲ߼ۿ���"),strData);
	if(_T("Y") == strData) m_nSEND_CANCEL_MAIL = 0;
	else  m_nSEND_CANCEL_MAIL = 1;
	strData.Empty();
	if(ids<0) return -14;

	//(A_2004-1115_HAN)���ټ��� ���� ��뿩�ο� ���� ���� ���� ���
	CESL_DataMgr *pDM = FindDM( _T("DM_��ٱ���������2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );
	CString sQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'ONELNERESERV'");
	ids = pDM->GetOneValueQuery( sQuery, strData );
	if ( (ids<0) && (-4007!=ids) ) return -15;
	else if ( _T("Y")==strData )
	{
		ids = this->GetManageValue(_T("����"),_T("����"),_T("�������"),_T("�ڵ������ڷ���������"),strData);
		if ( _T("Y")==strData ) m_nAutoReserveComplete = 0;
		else if ( _T("N")==strData ) m_nAutoReserveComplete = 1;
	}
	else
	{
		GetDlgItem(IDC_lblY6)->ShowWindow(FALSE);
		GetDlgItem(IDC_radSETTING_RESERVE_ATUO_CANCEL_Y4)->ShowWindow(FALSE);
		GetDlgItem(IDC_lblN6)->ShowWindow(FALSE);
		GetDlgItem(IDC_radSETTING_RESERVE_ATUO_CANCEL_N4)->ShowWindow(FALSE);
		GetDlgItem(IDC_lblRESERVE_DESC3)->ShowWindow(FALSE);

		CRect rect1, rect2 , rect3, rect4, rect5;

		CStatic* pStatic1 = (CStatic*)GetDlgItem(IDC_RE_BORDER);
		CStatic* pStatic2 = (CStatic*)GetDlgItem(IDC_imgDIVISION_LONG4);
		CStatic* pStatic3 = (CStatic*)GetDlgItem(IDC_imgDIVISION_LONG5);
		CStatic* pStatic4 = (CStatic*)GetDlgItem(IDC_imgDIVISION_LONG6);
		CStatic* pStatic5 = (CStatic*)GetDlgItem(IDC_lblRESERVE_FUNC6);

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

		INT nBorderHeight = rect1.Height( ) - (rect1.bottom - rect5.top);

		//��¿ �� ���� �״�� �������.
		pStatic1->MoveWindow( 10, 0, rect1.Width( ), nBorderHeight );
		pStatic2->MoveWindow( 95, 8, rect2.Width( ), nBorderHeight-8 );
		pStatic3->MoveWindow( 95+245, 8, rect3.Width( ), nBorderHeight-8 );
		pStatic4->MoveWindow( 340+165, 8, rect4.Width( ), nBorderHeight-8 );
		GetDlgItem(IDC_lblRESERVE_FUNC6)->ShowWindow(FALSE);
	}

	
	UpdateData(FALSE);

	OnSelchangecmbSETTINGDELAYORRESERVERETURN2();

	SettingDelayFunc();
	*/

	return 0;

EFS_END
return -1;
}

INT CMngLocMgr02Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	const int nCnt = 14;

	// ����
	CString sSection[nCnt] = 
			{	_T("�ݳ��������"), _T("�ݳ��������"), _T("�ݳ��������"), 
				_T("�ݳ��������"), _T("�ݳ��������"), _T("�ݳ��������"),	
				_T("�������"), _T("�������"), _T("�������"), 
				_T("�������"),	_T("�������"), _T("�������"), 
				_T("�������"), _T("�������") };
	// ������
	CString sManage[nCnt] = 
			{	_T("�ݳ������ɻ�뿩��"),	_T("���Ϲݳ������������"), 
				_T("�ݳ������ڵ������ο�"),	_T("�ݳ����⿹��켱����"), 
				_T("�ݳ����Ⱑ��Ƚ��"),		_T("�ݳ�����ο�����"), 
				_T("�����ڵ���ҿ���"), 	_T("���ุ�����ڵ��ο�����"), 
				_T("���ุ���Ϻο���SMS�߼ۿ���"),
				_T("�����ɻ�뿩��"), 	_T("���ุ���ް�����������"), 
				_T("��������ο�"), 		_T("������ҽø��Ϲ߼ۿ���"), 
				_T("�ڵ������ڷ���������") };

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

	// ���� �ݳ����� ���� ����� ���� �������� �˸��� 
	const INT nDelayManageCnt = 5;
	CString sValueNames[nDelayManageCnt] =
	{
		_T("�ݳ������ɻ�뿩��"), _T("�ݳ������ڵ������ο�"), _T("�ݳ����⿹��켱����"), 
		_T("�ݳ����Ⱑ��Ƚ��"),		_T("�ݳ�����ο�����") 
	};

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_��ٱ���������2") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	if ( m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE�� ����
// 		CString	strWhere;
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE ")
// 			            _T("MANAGE_CODE IN ( SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
// 						_T("WHERE ID = '%s')"), m_strUserID );
// 		pSrcDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
		m_strLibCode = m_pInfo->LIB_CODE;
	}
	
	INT nIndex;

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("64");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("ESL_MANAGE_TBL");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	
	CString sRecKey;
	for( i = 0 ; i < nDelayManageCnt ; i++ )
	{
		ids = CLocCommonAPI::FindManageDMIndex( this, pSrcDM, _T("����"), _T("����"), _T("�ݳ��������"), sValueNames[i], nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetManageValue") );
		if( ids > 0 ) return 0;

		pSrcDM->GetCellData( _T("REC_KEY") , nIndex , sRecKey );

		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	}

	return 0;
	/*
	UpdateData(TRUE);
	
	CString strData = _T("");
	INT ids = 0;
	
	const INT nDelayManageCnt = 5;
	CString sValueNames[nDelayManageCnt] =
	{
		_T("�ݳ������ɻ�뿩��") , _T("�ݳ������ڵ������ο�") , _T("�ݳ����⿹��켱����") , _T("�ݳ����Ⱑ��Ƚ��") , _T("�ݳ�����ο�����") 
	};
	CString sDelayManageValue[nDelayManageCnt];
	
	//����° 
	if(0 == m_nRETURN_DELAY_USE) strData = _T("Y");
	else  strData = _T("N");
	sDelayManageValue[0] = strData;
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ݳ��������"),_T("�ݳ������ɻ�뿩��"),strData);
	strData.Empty();
	if(ids<0) return -7;

	if(0 == m_nRETURN_DELAY_AUTO) strData = _T("Y");
	else  strData = _T("N");
	sDelayManageValue[1] = strData;
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ݳ��������"),_T("�ݳ������ڵ������ο�"),strData);
	strData.Empty();
	if(ids<0) return -8;
		
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN);
	strData.Format(_T("%d"),pCombo->GetCurSel());
	sDelayManageValue[2] = strData;
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ݳ��������"),_T("�ݳ����⿹��켱����"),strData);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -9;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtDELAYRETURNCNT);
	pEdit->GetWindowText(strData);
	sDelayManageValue[3] = strData;	
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ݳ��������"),_T("�ݳ����Ⱑ��Ƚ��"),strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -10;

	pCombo = (CComboBox*)GetDlgItem(IDC_cmbRETURN_PLAN_DAY_DELAY_OPTION);
	strData.Format(_T("%d"),pCombo->GetCurSel());
	sDelayManageValue[4] = strData;	
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ݳ��������"),_T("�ݳ�����ο�����"),strData);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -9;

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// ���� �ݳ����� ���� ����� ���� �������� �˸��� 
	// 2003�� 11�� 8�� �۾�
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_��ٱ���������2") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	// 2005.05.26 ADD BY PDJ
	if ( m_strLibCode.IsEmpty() )
	{
		CString	strWhere;
		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE ")
			            _T("MANAGE_CODE IN ( SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
						_T("WHERE ID = '%s')"), m_strUserID );
		pSrcDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}
	
	INT nIndex;

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("64");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("ESL_MANAGE_TBL");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	
	CString sRecKey;
	for( INT i = 0 ; i < nDelayManageCnt ; i++ )
	{
		ids = CLocCommonAPI::FindManageDMIndex( this , pSrcDM , _T("����") , _T("����") , _T("�ݳ��������") , sValueNames[i] , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetManageValue") );
		if( ids > 0 ) return 0;

		pSrcDM->GetCellData( _T("REC_KEY") , nIndex , sRecKey );

		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	}

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------

	//�׹�°
	if(0 == m_nRESERVE_AUTO_CANCEL) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("�����ڵ���ҿ���"),strData);
	strData.Empty();
	if(ids<0) return -11;
	
	if(0 == m_nRESERVE_EXPIRE_AUTO) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("���ุ�����ڵ��ο�����"),strData);
	strData.Empty();
	if(ids<0) return -12;

	pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN2);
	strData.Format(_T("%d"),pCombo->GetCurSel());
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("�����ɻ�뿩��"),strData);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -13;
	
	pEdit = (CEdit*)GetDlgItem(IDC_edtDELAYRETURNCNT2);
	pEdit->GetWindowText(strData);
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("��������ο�"),strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -14;

	if(0 == m_nSEND_CANCEL_MAIL) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("������ҽø��Ϲ߼ۿ���"),strData);
	strData.Empty();
	if(ids<0) return -15;

	// 2007.07.13 add by pdw
	// ���ุ���Ͽ� �ް��� ���� ����
	if(0 == m_nAddHolydayReservation) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("���ุ���ް�����������"),strData);
	strData.Empty();
	if(ids<0) return -15;

	//++2008.06.06 ADD BY PWR {{++
	// ���Ϲݳ������������ ������ �߰�
	if(0 == m_nRETURN_SYSDATE_DELAY_USE) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�ݳ��������"),_T("���Ϲݳ������������"),strData);
	strData.Empty();
	if(ids<0) return -15;
	//--2008.06.06 ADD BY PWR --}}

	//CESL_DataMgr *pDM = FindDM( _T("DM_��ٱ���������2") );
	//if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );
	//CString sQuery = _T("SELECT FROM WHERE");
	//ids = pDM->GetOneValueQuery( sQuery, strData );
	//if ( ids<0 ) return -15;
	strData = _T("Y");
	if ( _T("Y")==strData )
	{
		if(0 == m_nAutoReserveComplete ) strData = _T("Y");
		else  strData = _T("N");
		ids = this->SetManageValue(_T("����"),_T("����"),_T("�������"),_T("�ڵ������ڷ���������"),strData);
		strData.Empty();
		if(ids<0) return -16;
	}
	*/

	return 0;

EFS_END
return -1;
}

void CMngLocMgr02Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_LOC_MGR_02);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 1, cx, cy-10);	
}

BEGIN_EVENTSINK_MAP(CMngLocMgr02Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr02Dlg)
	ON_EVENT(CMngLocMgr02Dlg, IDC_gridMNG_LOC_MGR_02, -600 /* Click */, OnClickgridMNGLOCMGR02, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMngLocMgr02Dlg::OnClickgridMNGLOCMGR02() 
{
	if( m_pGrid->GetMouseRow() == 0 ) return;
	INT col = m_pGrid->GetCol();
	if( col == 4 ) CreateComboBoxInGridSell();	
}

// ** �޺��ڽ� ���� �� �� ����
VOID CMngLocMgr02Dlg::CreateComboBoxInGridSell()
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

	/// �ݳ����� ��뿩�ο� ���� ǥ��
	// 1:�ݳ������뿩�ΰ� ������(N)�̸� 
	// 3:���ϴ����ڷ�ݳ��������,4:�ݳ������ ����켱��������,5:�ݳ����Ⱑ��Ƚ���� ��Ȱ��ȭ �Ǿ�� �Ѵ�.
	// 2009.05.20 UPDATE BY PDJ : 2:���ϴ����ڷ� �ݳ�������뿩��, 6:�ݳ���������������� �߰���
	//if( nRow == 3 || nRow == 4 || nRow == 5 )	
	if( nRow == 2 || nRow == 3 || nRow == 4 || nRow == 5 || nRow == 6)
	{
		CString sDelay = m_pDM->GetCellData( _T("������"), 0 );
		if( sDelay != _T("Y") ) return;
	}
	/// ������ ��뿩�ο� ���� ǥ��
	// 9:�����ɻ�뿩�ΰ� ������(0)�̸�
	// 7:�����ڵ���Ҵ� ������(N),��Ȱ��ȭ�� 12:��������ο��� ��Ȱ��ȭ�Ѵ�
	if( nRow == 7 || nRow == 12 )
	{
		CString sDelay = m_pDM->GetCellData( _T("������"), 8 );
		if( sDelay == _T("0") ) return;
	}

	// 1:�ݳ������뿩��, 2:���ϴ����ڷ� �ݳ��������, 3:�ݳ������� �ڵ��ο�, 7:�����ڵ����, 
	// 8:���ุ���� �ڵ��ο�, 9:���ุ���Ϻο���SMS�߼ۿ���, 11:���ุ���� �ް��� ��������, 
	// 13:������� �ȳ����� �߼ۿ���, 14:���ټ��⿹����
	if( nRow == 1 || nRow == 2 || nRow == 3 || nRow == 7 || 
		nRow == 8 || nRow == 9 || nRow == 11 || nRow == 13 || nRow == 14)
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("�����"));
		ArrCmbData.Add(_T("������"));
	}
	// 4:�ݳ������ ����켱���� ����
	else if( nRow == 4 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("�ݳ�����켱"));
		ArrCmbData.Add(_T("����켱"));
	}
	// 6:�ݳ������� ���� ������
	else if( nRow == 6 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("�ݳ�������"));
		ArrCmbData.Add(_T("������"));
	}
	// 10:������ ��뿩��
	else if( nRow == 10 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("������"));
		ArrCmbData.Add(_T("�������� �ڷḸ ���డ��"));
		ArrCmbData.Add(_T("����ڷ� ���డ��"));
	}
	// 5:�ݳ����� ����Ƚ��
	else if( nRow == 5 )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl1);
		return;
	}
	// 12:��������ο�
	else if( nRow == 12 )
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
	m_pComboBox->InitMngLocMgr(2);
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

INT CMngLocMgr02Dlg::ShowEditBox(CESL_Grid *pGrid, CEdit* pEditCtrl)
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
	sValue.Replace( _T("ȸ"), _T("") );
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

INT CMngLocMgr02Dlg::GetCellRect(CESL_Grid *pGrid, CRect &rect)
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

INT CMngLocMgr02Dlg::ApplyDataChange(LPCTSTR lpszData)
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
	pSet->SettingGrid02(m_pDM, m_pGrid);
	delete pSet;
	pSet = NULL;

	return 0;

EFS_END
return -1;
}

void CMngLocMgr02Dlg::OnKillfocusedtDELAYRETURNCNT() 
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

void CMngLocMgr02Dlg::OnKillfocusedtLIMIT() 
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

INT CMngLocMgr02Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
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

// ���� ����� ����� ���
VOID CMngLocMgr02Dlg::OnSelchangecmbSETTINGDELAYORRESERVERETURN2() 
{
EFS_BEGIN

	INT nCur;
	
	nCur = ((CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN2))->GetCurSel();

	if( nCur == 0 )
	{
		((CButton*)GetDlgItem(IDC_radSETTING_RESERVE_ATUO_CANCEL_Y))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_radSETTING_RESERVE_ATUO_CANCEL_N))->SetCheck(1);
		GetDlgItem(IDC_edtDELAYRETURNCNT2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_edtDELAYRETURNCNT2)->EnableWindow(TRUE);
	}

EFS_END
}

VOID CMngLocMgr02Dlg::OnradSETTINGRESERVEUSEN2() 
{
EFS_BEGIN

	SettingDelayFunc();

EFS_END
}

VOID CMngLocMgr02Dlg::OnradSETTINGRESERVEUSEY2() 
{
EFS_BEGIN

	SettingDelayFunc();

EFS_END
}


VOID CMngLocMgr02Dlg::SettingDelayFunc()
{
EFS_BEGIN


	INT nCheck = ((CButton*)GetDlgItem(IDC_radSETTING_RESERVE_USE_Y2))->GetCheck();

	const INT nCnt = 6;
	INT nIDC[nCnt] =
	{
		IDC_radSETTING_RESERVE_ATUO_CANCEL_Y2 , IDC_radSETTING_RESERVE_ATUO_CANCEL_N2 ,
		IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN , IDC_edtDELAYRETURNCNT,
		IDC_radSETTING_SYSDATE_RESERVE_USE_Y, IDC_radSETTING_SYSDATE_RESERVE_USE_N
	};

	if( nCheck == 1 )
	{
		for( INT i = 0 ; i < nCnt ; i++ )
		{
			GetDlgItem( nIDC[i] )->EnableWindow( TRUE );
		}	
	}
	else
	{
		for( INT i = 0 ; i < nCnt ; i++ )
		{
			GetDlgItem( nIDC[i] )->EnableWindow( FALSE );
			((CComboBox*)GetDlgItem( nIDC[2] ))->SetCurSel(1);
		}	
	}

EFS_END
}




/// ��ٱ��� DM���� VALUE�� �̾Ƴ��� �Լ�
INT CMngLocMgr02Dlg::GetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
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
INT CMngLocMgr02Dlg::SetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
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
		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("�ݳ�����&����ȯ��"), __WFILE__, __LINE__ ), TRUE );
		
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
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetManageValue") );

	return 0;

EFS_END
return -1;
}

HBRUSH CMngLocMgr02Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
