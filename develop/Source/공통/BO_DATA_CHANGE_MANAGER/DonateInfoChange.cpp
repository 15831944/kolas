// DonateInfoChange.cpp : implementation file
//

#include "stdafx.h"
#include "DonateInfoChange.h"
#include "DataChangeManager.h"
#include "../COMMON_DONATOR_MANAGER/DonatorNameGetter_Search.h"
#include "../COMMON_DONATOR_MANAGER/DonatorNameGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonateInfoChange dialog
// ������
CDonateInfoChange::CDonateInfoChange(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonateInfoChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonateInfoChange)
	m_nRegYn	= 0;
	m_bDonator = TRUE;
	m_bRegYn = TRUE;
	m_bRemark = TRUE;
	m_bCurrencyCode = TRUE;  // 2004.09.08 TREU�� ����.
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM_BOOK = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;

	m_bDonatorInfo = FALSE;
	
	m_strDonatorKey = _T("");
	m_strDonatorName = _T("");
}

CDonateInfoChange::~CDonateInfoChange()
{
}
VOID CDonateInfoChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonateInfoChange)
	DDX_Check(pDX, IDC_chkDONATOR, m_bDonator);
	DDX_Check(pDX, IDC_chkREGYN, m_bRegYn);
	DDX_Check(pDX, IDC_chkREMARK, m_bRemark);
	DDX_Radio(pDX, IDC_radREG, m_nRegYn);
	DDX_Check(pDX, IDC_chkCURRENCY_CODE, m_bCurrencyCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonateInfoChange, CDialog)
	//{{AFX_MSG_MAP(CDonateInfoChange)
	ON_BN_CLICKED(IDC_chkDONATOR, OnchkDONATOR)
	ON_BN_CLICKED(IDC_chkREGYN, OnchkREGYN)
	ON_BN_CLICKED(IDC_chkREMARK, OnchkREMARK)
	ON_BN_CLICKED(IDC_radNOREG, OnradNOREG)
	ON_BN_CLICKED(IDC_radREG, OnradREG)
	ON_BN_CLICKED(IDC_btnSEARCH_DONATOR, OnbtnSEARCHDONATOR)
	ON_EN_KILLFOCUS(IDC_edtDONATOR, OnKillfocusedtDONATOR)
	ON_BN_CLICKED(IDC_chkCURRENCY_CODE, OnchkCURRENCYCODE)
	ON_EN_CHANGE(IDC_edtDONATOR, OnChangeedtDONATOR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonateInfoChange message handlers

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ������ üũ�ڽ��� üũ���ο� ���� ���ҽ��� ǥ���Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnchkDONATOR() 
{
	m_bDonator = !m_bDonator;
	if( m_bDonator == TRUE ) 
	{
		GetDlgItem(IDC_edtDONATOR		)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnSEARCH_DONATOR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_edtDONATOR		)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSEARCH_DONATOR)->EnableWindow(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���/�̵�� üũ�ڽ��� üũ���ο� ���� ���ҽ��� ǥ���Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnchkREGYN() 
{
	m_bRegYn = !m_bRegYn;
	if( m_bRegYn == TRUE ) 
	{
		GetDlgItem(IDC_radREG	)->EnableWindow(TRUE);
		GetDlgItem(IDC_radNOREG	)->EnableWindow(TRUE);
		
		if( m_nRegYn == 0 )		GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(FALSE);
		else GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_radREG)->EnableWindow(FALSE);
		GetDlgItem(IDC_radNOREG)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(FALSE);
	}	
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��� üũ�ڽ��� üũ���ο� ���� ���ҽ��� ǥ���Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnchkREMARK() 
{
	m_bRemark = !m_bRemark;
	if( m_bRemark == TRUE )		GetDlgItem(IDC_edtPC_REMARKS)->EnableWindow(TRUE);
	else	GetDlgItem(IDC_edtPC_REMARKS)->EnableWindow(FALSE);

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���/�̵���� ����� �����Ұ�쿡 ���� ���ҽ��� ǥ���Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnradNOREG() 
{
	UpdateData(TRUE);

	if( m_nRegYn == 0 )		GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(FALSE);
	else GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���/�̵���� �̵���� �����Ұ�쿡 ���� ���ҽ��� ǥ���Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnradREG() 
{
	UpdateData(TRUE);

	if( m_nRegYn == 0 )		GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(FALSE);
	else GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

// �ʱ�ȭ
BOOL CDonateInfoChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// SM����
	if( InitESL_Mgr(_T("K2UPGRADE_������������")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	
	// CM/DM����
	m_pCM = FindCM(_T("CM_������������"));		
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_������������]�� ������ �� �����ϴ�."));
		return FALSE;
	}

	// ���ҽ� ǥ�ÿ��θ� �����Ѵ�.
	OnchkDONATOR();		// ������
	OnchkREGYN();		// ���/�̵��
	OnchkREMARK();		// ���
	OnchkCURRENCYCODE();// ȭ�󱸺� 2004.09.08 OnchkCURRENCYCODE() �߰�.
	

	// �������Է¿��� �ʼ������� üũ�Ѵ�.
	SetDonatorInfo();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ϰ������ư
//
// [ �̴Ͻ��� ]
// 1. ������������ üũ�Ѵ�.
// 2. ������ ������ ���Ͽ� �����ͺ���Ŭ������ �����ϰ� �����Ѵ�.
// 3. ������ ������������ ������ �����Ѵ�.
// 4. ������ ������ �����ͺ���Ŭ������ �����ϰ� [������������]�� ó���Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnOK() 
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT ids;
	CDataChangeManager mgr(this);
	CDonateInfoChangeCondition cont;

	UpdateData(FALSE);

	//=======================================================================
	// 1. ������������ üũ�Ѵ�.
	//=======================================================================
	ids = CheckDonatorInfo();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("������������ �ùٸ��� �ʽ��ϴ�."));
		return;
	}
	
	//=======================================================================
	// 2. ������ ������ ���Ͽ� �����ͺ���Ŭ������ �����ϰ� �����Ѵ�.
	//=======================================================================
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM_BOOK);
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)m_pMainGrid);
	mgr.SetTempData(_T("DonatorKey"), m_strDonatorKey.GetBuffer(0));
	//=======================================================================
	// 3. ������ ������������ ������ �����Ѵ�.
	//=======================================================================
	cont.SetDonator(m_bDonator);
	cont.SetRegYn(m_bRegYn);
	cont.SetRemark(m_bRemark);
	cont.SetNRegYn(m_nRegYn);
	// 2004.09.08 �߰�.
	cont.SetCurrencyCode(m_bCurrencyCode);


	//=======================================================================
	// 4. ������ ������ �����ͺ���Ŭ������ �����ϰ� [������������]�� ó���Ѵ�.
	//=======================================================================
	mgr.SetTempPtr(_T("Condition"), (DWORD*)&cont);
	ids = mgr.SPFExecutePath(_T("������������"));
	if( ids < 0 )
	{
		//����ó��
		ESLAfxMessageBox(_T("�����Ͻ� �ڷ��� ���������� ������ �� �����ϴ�."));
		CDialog::OnCancel();
	}	
	ESLAfxMessageBox(_T("�����Ͻ� �ڷ��� ���������� �����Ͽ����ϴ�."));

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �����ڸ� �˻��Ͽ� �����Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnbtnSEARCHDONATOR() 
{
	//=======================================================================
	// ��������
	//=======================================================================
	CDonatorNameGetter_Search searchDlg(this);

	//=======================================================================
	// 1. ������ �˻�â�� ��� �����ڸ� �˻��Ѵ�.
	// 2. �˻���� �߿� �ϳ��� �����ߴٸ� ������ �̸��� CM�� �����ϰ� ������Ű�� ��������� �����Ѵ�.
	//=======================================================================
	searchDlg.SetOpenMode(-1000);
	if( IDOK == searchDlg.DoModal() )
	{
		CDonatorNameGetterDlg searchResult(this);
		if( IDOK == searchResult.DoModal() )
		{
			m_strDonatorKey = searchResult.GetDonatorKey();
			m_strDonatorName = searchResult.GetDonatorName();
			m_pCM->SetControlMgrData(_T("BP_������"), m_strDonatorName);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - DB���� �����������ʼ��Է»��׿��θ� üũ�Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::SetDonatorInfo()
{
	CString strValue;

	GetManageValue(_T("����"), _T("����"), _T("�����������ʼ��Է»��׿���"), _T(""), strValue);
	
	if( strValue == _T("Y") )	m_bDonatorInfo = TRUE;
	else						m_bDonatorInfo = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �����ڸ� üũ�Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. �����ڰ� üũ�Ǿ� ���� �ʴٸ� �׳� �����Ѵ�.
// 2. CM���� �����ڸ��� �����´�.
// 3. �������̸��� ������ üũ�Ѵ�.
// 4. �������̸����� ���������̺� �˻��Ѵ�.
// 5. �˻������ ���� �����ڸ� �缳���Ѵ�.
// 6. ������ �Է»����� �ʼ����� Ȯ���Ͽ� ���� �Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
INT CDonateInfoChange::CheckDonatorInfo()
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT cnt;
	BOOL bValidDonatorInfo = FALSE;
	CString strDonatorInfo, strQuery, strTmpData;
	CESL_DataMgr* pDM = FindDM(_T("DM_�����ڰ���"));

	
	//=======================================================================
	// 1. �����ڰ� üũ�Ǿ� ���� �ʴٸ� �׳� �����Ѵ�.
	//=======================================================================
	if( m_bDonator == FALSE )	return 0;
	
	//=======================================================================
	// 2. CM���� �����ڸ��� �����´�.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("BP_������"), strDonatorInfo);

	//=======================================================================
	// 3. �������̸��� ������ üũ�Ѵ�.
	// 4. �������̸����� ���������̺� �˻��Ѵ�.
	// 5. �˻������ ���� �����ڸ� �缳���Ѵ�.
	//=======================================================================
	if( strDonatorInfo.IsEmpty() == TRUE )
	{
		bValidDonatorInfo = FALSE;
		//===========================================================
		//20080416 ADD BY PJW : ��ϵ��� ���� �������ϰ�� �����Ѵ�.
		//20080529 REM BY PJW : �����ڰ� NULL�ϰ�� �� -1�� �������� �ʴ´�.
		//return -1;
		//===========================================================
	}
	else
	{
		// 2005-12-02 ������ �߰�
		// ����������Ű�� ���ٸ� �ٽ� �˻��϶�
		if( m_strDonatorKey.IsEmpty() == TRUE )
		{
		strQuery.Format(_T("DONATOR = '%s'"), strDonatorInfo);
		pDM->RefreshDataManager(strQuery);

			cnt = pDM->GetRowCount();
			// �˻���� ����
			if ( cnt == 0 )
			{
				bValidDonatorInfo = FALSE;
				//===========================================================
				//20080416 ADD BY PJW : ��ϵ��� ���� �������ϰ�� �����Ѵ�.
				//20080529 UPDATE BY PJW : �����ڰ� ������� ����Ʈ �ڽ��� NULL�� �����Ѵ�.
				ESLAfxMessageBox(_T("������������ �ùٸ��� �ʽ��ϴ�."));
				m_pCM->SetControlMgrData(_T("BP_������"), _T("") );
				
				//return -1;
				//===========================================================
			}
			// �˻���� �ϳ� - ������ �̸� ����
			else if	( cnt == 1 )
			{
				bValidDonatorInfo = TRUE;
				m_strDonatorKey = pDM->GetCellData(_T("����������KEY"), 0);
			}
			// �˻���� �ټ� - ���������� ����
			else if	( cnt > 1 )
			{
					// ���� �����ڿ� �ٸ���� ���������̶�� �ٽ� �����ڸ� �����ϵ��� �Ѵ�.
				CDonatorNameGetterDlg dlg(this);
					if( IDOK == dlg.DoModal() )
				{
					m_pCM->SetControlMgrData(_T("BP_������"), dlg.GetDonatorName());
					m_strDonatorKey = dlg.GetDonatorKey();
					bValidDonatorInfo = TRUE;
				}
			}
			// �˻����� ����
			else
			{
				ESLAfxMessageBox(_T("�����ڸ� �˻��� �� �����ϴ�."));
				return -1;
			}
		}
		else	bValidDonatorInfo = TRUE;
	}

	//=======================================================================
	// 6. ������ �Է»����� �ʼ����� Ȯ���Ͽ� ���� �Ѵ�.
	//=======================================================================
	if( m_bDonatorInfo == TRUE )
	{
		if( bValidDonatorInfo == TRUE )		return 0;
		else return -1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ������ ����Ʈ�ڽ� KillFocus �Ҷ����� ������������ �˻��Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnKillfocusedtDONATOR() 
{
	INT ids = CheckDonatorInfo();

	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("������ ������ �ùٸ��� �ʽ��ϴ�."));
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ȭ�󱸺� üũ�ڽ��� üũ���ο� ���� ���ҽ��� ǥ���Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnchkCURRENCYCODE() 
{
	m_bCurrencyCode = !m_bCurrencyCode;

	if( m_bCurrencyCode == TRUE )	GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(FALSE);

	UpdateData(FALSE);	
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �������̸��� �����Ѵٸ� ����������Ű�� �����.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnChangeedtDONATOR() 
{
	CString strDonator = _T("");
	GetDlgItemText(IDC_edtDONATOR, strDonator);
	if( strDonator != m_strDonatorName )	m_strDonatorKey.Empty();
}

HBRUSH CDonateInfoChange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
