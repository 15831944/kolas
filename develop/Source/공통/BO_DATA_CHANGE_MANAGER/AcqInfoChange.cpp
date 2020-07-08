// AcqInfoChange.cpp : implementation file
//

#include "stdafx.h"
#include "AcqInfoChange.h"
#include "DonateInfoChangeCondition.h"
#include "DataChangeManager.h"

//20070611 ADD PJW : ������ �˻��߰�
#include "../COMMON_DONATOR_MANAGER/DonatorNameGetter_Search.h"
#include "../COMMON_DONATOR_MANAGER/DonatorNameGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcqInfoChange dialog

// ������
CAcqInfoChange::CAcqInfoChange(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CAcqInfoChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcqInfoChange)
	m_bCurrencyCode = TRUE;  // 2005-02-22
	m_bRemark = TRUE;
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM_BOOK = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;
	//20070611 ADD PJW : ������ �˻��߰�
	m_bDonatorInfo = FALSE;
	m_strDonatorKey = _T("");
	m_strDonatorName = _T("");
}

CAcqInfoChange::~CAcqInfoChange()
{
}



VOID CAcqInfoChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcqInfoChange)
	DDX_Check(pDX, IDC_chkCURRENCY_CODE, m_bCurrencyCode);
	DDX_Check(pDX, IDC_chkREMARK, m_bRemark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcqInfoChange, CDialog)
	//{{AFX_MSG_MAP(CAcqInfoChange)
	ON_BN_CLICKED(IDC_chkCURRENCY_CODE, OnchkCURRENCYCODE)
	ON_BN_CLICKED(IDC_chkREMARK, OnchkREMARK)
	ON_BN_CLICKED(IDC_btnSEARCH_DONATOR1, OnbtnSEARCHDONATOR)
	ON_EN_KILLFOCUS(IDC_edtDONATOR1, OnKillfocusedtDONATOR)
	ON_EN_CHANGE(IDC_edtDONATOR1, OnChangeedtDONATOR)
	ON_CBN_SELCHANGE(IDC_cmbBSDIVNO_ACQ_CODE, OnSelchangecmbBSDIVNOACQCODE)
	ON_CBN_SELCHANGE(IDC_cWORK_CODE, OnSelchangecWORKCODE)
	ON_CBN_SELCHANGE(IDC_cmbBSDIVNO_GROUP_NO, OnSelchangecmbBSDIVNOGROUPNO)	
	ON_BN_CLICKED(IDC_chkDONATOR1, OnchkDONATOR)
	ON_BN_CLICKED(IDC_chkPROVIDER, OnchkPROVIDER)
	ON_BN_CLICKED(IDC_chkAcqCode, OnchkAcqCode)
	ON_WM_CTLCOLOR()
	ON_CBN_KILLFOCUS(IDC_cWORK_CODE, OnKillfocuscWORKCODE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqInfoChange message handlers
// �ʱ�ȭ
BOOL CAcqInfoChange::OnInitDialog() 
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
	if(m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_������������]�� ������ �� �����ϴ�."));
		return FALSE;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_�����ڰ���"));

	if( pDM == NULL )
	{
		GetDlgItem(IDC_chkDONATOR1)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_edtDONATOR1)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_btnSEARCH_DONATOR1)->ShowWindow(SW_HIDE);		
	}
	



	OnchkCURRENCYCODE();

	OnchkREMARK();
	//20070611 ADD PJW : ������ �˻��߰�
	OnchkDONATOR();
	OnchkPROVIDER();
	
	// �������Է¿��� �ʼ������� üũ�Ѵ�.
	SetDonatorInfo();	

	// 2007.12.16 ADD BY PDJ
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	m_pCM->SetControlMgrData(_T("BP_���Գ⵵"), result);
	
	//===================================
	//20080603 ADD BY PJW : �����ڷ���������� ���Ա���üũ�ڽ��� �����Ҽ� ������ �Ѵ�.
	CESL_DataMgr* pTempDM = FindDM(_T("DM_�����ڷ����"));
	if( pTempDM != NULL )
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_chkAcqCode);
		pEdit->EnableWindow(FALSE);
		GetDlgItem(IDC_chkPROVIDER)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbPC_PROVIDER)->EnableWindow(FALSE);	
	}	
	//===================================
	GetDlgItem(IDC_cmbBSDIVNO_ACQ_CODE	)->EnableWindow(FALSE);
	GetDlgItem(IDC_cmbBSDIVNO_GROUP_NO	)->EnableWindow(FALSE);
	GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->EnableWindow(FALSE);
	GetDlgItem(IDC_cWORK_CODE			)->EnableWindow(FALSE);
	GetDlgItem(IDC_btnBSDIVNO_CREATE	)->EnableWindow(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

VOID CAcqInfoChange::OnchkCURRENCYCODE() 
{
	m_bCurrencyCode = !m_bCurrencyCode;
	if( m_bCurrencyCode == TRUE )		GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(FALSE);

	UpdateData(FALSE);		
}

VOID CAcqInfoChange::OnchkREMARK() 
{
	m_bRemark = !m_bRemark;
	if( m_bRemark == TRUE )				GetDlgItem(IDC_edtPC_REMARKS)->EnableWindow(TRUE);
	else	GetDlgItem(IDC_edtPC_REMARKS)->EnableWindow(FALSE);

	UpdateData(FALSE);	
}

VOID CAcqInfoChange::OnOK() 
{
	// 

	INT ids;
	CDataChangeManager mgr(this);
	CDonateInfoChangeCondition cont;	
	UpdateData(TRUE);

	CString strTmpData;
	BOOL bCheck;	

	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM_BOOK);
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)m_pMainGrid);

	bCheck = ( (CButton*) GetDlgItem(IDC_chkAcqCode) )->GetCheck();
	if ( bCheck == TRUE )
	{
		CString strAcqCode;
		m_pCM->GetControlMgrData(_T("BP_���Ա���_�ڵ�"), strAcqCode);

		// ��ġ��� ������ ���� ��� ACQ_Key�� ����ؾ� �Ѵ�.
		// * ���Ա��� ������ ��� ����,���������� �����ؾ� �ϸ�
		//   ���� ������ �����ؾ� �Ѵ�.
		// * ���Կ��� ���� �Ǵ� �������� �������� ���� �����
		//   ���������� ����Ǿ�� �ϴ°�? �׷���.
		// * ���Ա����� ���� �ȵǾ��� ��� 
		//   �����ں���� ����ڷḸ
		//   ����ó����� �����ڷḸ �����ϵ��� ó���ؾ���.		
		mgr.SetTempData(_T("BP_���Ա���"), strAcqCode.GetBuffer(0) );
				
		// 1. ���԰��� ����
		if ( strAcqCode == _T("1") )
		{			
			// ���Գ⵵
			m_pCM->GetControlMgrData(_T("BP_���Գ⵵"), strTmpData);
			mgr.SetTempData(_T("BP_���Գ⵵"), strTmpData.GetBuffer(0) );
			// ��������
			m_pCM->GetControlMgrData(_T("BS_��������"), strTmpData);
			mgr.SetTempData(_T("BS_��������"), strTmpData.GetBuffer(0) );

			// ������ȣ(�׷��ȣ)
			m_pCM->GetControlMgrData(_T("BP_�׷��ȣ"), strTmpData);
			mgr.SetTempData(_T("BP_�׷��ȣ"), strTmpData.GetBuffer(0) );

			// �۾���ȣ
			m_pCM->GetControlMgrData(_T("BP_�۾���ȣ"), strTmpData);
			mgr.SetTempData(_T("BP_�۾���ȣ"), strTmpData.GetBuffer(0) );
		}
		// 2. �������� ����
		else 
		{
			// ���Գ⵵			
			m_pCM->GetControlMgrData(_T("BP_���Գ⵵"), strTmpData);
			mgr.SetTempData(_T("BP_���Գ⵵"), strTmpData.GetBuffer(0) );

			// ������ȣ
			m_pCM->GetControlMgrData(_T("BP_�׷��ȣ"), strTmpData);
			mgr.SetTempData(_T("BP_�׷��ȣ"), strTmpData.GetBuffer(0) );
		}		
	}
	
	// ����ó ����
	bCheck = ( (CButton*) GetDlgItem(IDC_chkPROVIDER) )->GetCheck();
	if ( bCheck == TRUE )
	{
		m_pCM->GetControlMgrData(_T("BP_����ó"), strTmpData);
		mgr.SetTempData(_T("BP_����ó"), strTmpData.GetBuffer(0) );
	}

	// ������ ����
	bCheck = ( (CButton*) GetDlgItem(IDC_chkDONATOR1) )->GetCheck();
	if ( bCheck == TRUE )
	{
		//=======================================================================
		// 1. ������������ üũ�Ѵ�.
		//=======================================================================
		ids = CheckDonatorInfo();
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("������������ �ùٸ��� �ʽ��ϴ�."));
			return;
		}	
		
		//20070611 ADD PJW : ������ �˻��߰�
		mgr.SetTempData(_T("DonatorKey"), m_strDonatorKey.GetBuffer(0) );	
		//20070611 ADD PJW : ������ �˻��߰�
		cont.SetDonator(m_bDonator);
	}


	// 3. �������� ����
	cont.SetCurrencyCode(m_bCurrencyCode);
	cont.SetRemark(m_bRemark);	

	mgr.SetTempPtr(_T("Condition"), (DWORD*)&cont);	
	ids = mgr.SPFExecutePath(_T("������������"));
	if(ids < 0)
	{
		//����ó��
		ESLAfxMessageBox(_T("�����Ͻ� �ڷ��� ���������� ������ �� �����ϴ�."));
		CDialog::OnCancel();
	}
	else 
	{
		ESLAfxMessageBox(_T("�����Ͻ� �ڷ��� ���������� �����Ͽ����ϴ�."));
	}

	CDialog::OnOK();
}

//20070611 ADD PJW : ������ �˻��߰�

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ������ üũ�ڽ��� üũ���ο� ���� ���ҽ��� ǥ���Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CAcqInfoChange::OnchkDONATOR() 
{
	m_bDonator = ( (CButton*) GetDlgItem ( IDC_chkDONATOR1 ) )->GetCheck();

	BOOL bCheck2;
	bCheck2 = ( (CButton*) GetDlgItem(IDC_chkAcqCode) )->GetCheck();
	if ( bCheck2 && m_bDonator )
	{
		// ���Ա����� �������� ������ ��� ����ó�� �����ϸ� �ȵȴ�.
		CString strAcqCode;
		m_pCM->GetControlMgrData(_T("BP_���Ա���_�ڵ�"), strAcqCode);
		if ( strAcqCode == _T("1") )
		{
			( (CButton*) GetDlgItem(IDC_chkDONATOR1) )->SetCheck(FALSE);
			m_bDonator = FALSE;
			ESLAfxMessageBox(_T("���Ա����� �������� ����� ��� �����ڸ� ������ �� �����ϴ�."));
			return;
		}
	}

	if( m_bDonator == TRUE ) 
	{		
		GetDlgItem(IDC_edtDONATOR1		)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnSEARCH_DONATOR1)->EnableWindow(TRUE);
		( (CButton*) GetDlgItem(IDC_chkPROVIDER) )->SetCheck(FALSE);			
		( (CComboBox*) GetDlgItem (IDC_cmbPC_PROVIDER) )->EnableWindow(FALSE);	
		ESLAfxMessageBox(_T("�����ڷḸ ������ ������ ���� �˴ϴ�."));
	}
	else
	{
		GetDlgItem(IDC_edtDONATOR1		)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSEARCH_DONATOR1)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);	
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ������ ����Ʈ�ڽ� KillFocus �Ҷ����� ������������ �˻��Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CAcqInfoChange::OnKillfocusedtDONATOR() 
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
// - �����ڸ� �˻��Ͽ� �����Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CAcqInfoChange::OnbtnSEARCHDONATOR() 
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
// - �������̸��� �����Ѵٸ� ����������Ű�� �����.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CAcqInfoChange::OnChangeedtDONATOR() 
{
	CString strDonator = _T("");
	GetDlgItemText(IDC_edtDONATOR1, strDonator);
	if( strDonator != m_strDonatorName )	m_strDonatorKey.Empty();
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
INT CAcqInfoChange::CheckDonatorInfo()
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
			if	( cnt == 0 )
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
// - DB���� �����������ʼ��Է»��׿��θ� üũ�Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CAcqInfoChange::SetDonatorInfo()
{
	CString strValue;

	GetManageValue(_T("����"), _T("����"), _T("�����������ʼ��Է»��׿���"), _T(""), strValue);
	
	if( strValue == _T("Y") )	m_bDonatorInfo = TRUE;
	else						m_bDonatorInfo = FALSE;
}

void CAcqInfoChange::OnSelchangecmbBSDIVNOACQCODE() 
{
	// TODO: Add your control notification handler code here
	CString strAcqCode;
	m_pCM->GetControlMgrData(_T("BP_���Ա���_�ڵ�"), strAcqCode);
	
	if(strAcqCode == _T("1")) SetAcqType(PURCHASE);
	else if(strAcqCode == _T("2")) SetAcqType(DONATE);
	else if(strAcqCode.IsEmpty()) SetAcqType(ACQ_TYPE_INVALID);
	else SetAcqType(OTHER_ACQ_TYPE);
	
	
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("BP_���Ա���_�ڵ�"));
	if(pCMB) pCMB->ShowDropDown(FALSE);		

	SetCtrlByAcqCode();		
}

VOID CAcqInfoChange::SetCtrlByAcqCode()
{
	switch( GetAcqType() )
	{
	case PURCHASE:
		{
			GetDlgItem(IDC_strBSFORM_GROUP_NO	)->SetWindowText(_T("������ȣ"));
			GetDlgItem(IDC_strBSFORM_WORK_NO	)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_btnBSDIVNO_CREATE	)->SetWindowText(_T("������ȣ����"));
			//( ( CComboBox*) GetDlgItem (IDC_cWORK_CODE) )->EnableWindow(TRUE);
			( ( CComboBox*) GetDlgItem (IDC_cWORK_CODE) )->ShowWindow(SW_SHOW);
			( ( CComboBox*) GetDlgItem (IDC_stcWORK_CODE) )->ShowWindow(SW_SHOW);
			// ������ ������ ����
			( (CButton*) GetDlgItem(IDC_chkDONATOR1) )->SetCheck(FALSE);
			OnchkDONATOR();
			
		}
		break;
	case DONATE:
		{
			GetDlgItem(IDC_strBSFORM_GROUP_NO	)->SetWindowText(_T("������ȣ"));
			GetDlgItem(IDC_strBSFORM_WORK_NO	)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_btnBSDIVNO_CREATE	)->SetWindowText(_T("������ȣ����"));
			//( ( CComboBox*) GetDlgItem (IDC_cWORK_CODE) )->EnableWindow(FALSE);
			( ( CComboBox*) GetDlgItem (IDC_cWORK_CODE) )->ShowWindow(SW_HIDE);
			( ( CComboBox*) GetDlgItem (IDC_stcWORK_CODE) )->ShowWindow(SW_HIDE);
			//===============================================
			//20080618 ADD BY PJW : ���Ա��п����� �������� �����Ѵ�.
			m_pCM->SetControlMgrData(_T("BS_��������"), _T("    "));
			//===============================================

			// ����ó ������ ����			
			( (CButton*) GetDlgItem(IDC_chkPROVIDER) )->SetCheck(FALSE);
			OnchkPROVIDER();
		}
		break;
	default:
		//===================================================
		//2009.10.20 ADD BY PJW : 
		{
			GetDlgItem(IDC_strBSFORM_GROUP_NO	)->SetWindowText(_T("�׷��ȣ"));
			GetDlgItem(IDC_strBSFORM_WORK_NO	)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_btnBSDIVNO_CREATE	)->SetWindowText(_T("�׷��ȣ����"));
			( ( CComboBox*) GetDlgItem (IDC_cWORK_CODE) )->ShowWindow(SW_HIDE);
			( ( CComboBox*) GetDlgItem (IDC_stcWORK_CODE) )->ShowWindow(SW_HIDE);
			m_pCM->SetControlMgrData(_T("BS_��������"), _T("    "));		
			( (CButton*) GetDlgItem(IDC_chkPROVIDER) )->SetCheck(FALSE);
			OnchkPROVIDER();
		}
		//===================================================
		break;
	}
	

	CString strAcqYear;
	CString strWorkCode;
	m_pCM->GetControlMgrData(_T("BP_���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("BS_��������"), strWorkCode);

	INT ids;
	ids = MakeGroupInfo(strAcqYear, strWorkCode);
	//=====================================================
	//20080612 ADD BY PJW : ids�� 0���Ϸ� �������� ���������� �ٽ� �����Ѵ�. 
	if( 0 > ids )
	{
		m_pCM->SetControlMgrData(_T("BS_��������"), _T("1"));
		AfxMessageBox(_T("�۾���ȣ�� �����Ҽ� �����ϴ�."));		
	}
	//=====================================================
}

VOID CAcqInfoChange::SetAcqType(INT nAcqType)
{
	CString strAcqCode;
	
	m_pCM->GetControlMgrData(_T("BP_���Ա���_�ڵ�"), strAcqCode);		
	if ( strAcqCode == _T("") ) 
	{
		// ���Ա����� ��������� ��� ������.
		// �ڵ� �����Ͽ���
		m_pCM->SetControlMgrData(_T("BP_���Ա���_�ڵ�"), _T("1") );	
		//===================================================
		//2009.05.20 ADD BY PJW : ���Ա����� �����ϸ� �������е� ����Ǿ�� �Ѵ�.
		m_pCM->SetControlMgrData(_T("BS_��������"), _T("1"));
		//===================================================		
		strAcqCode = _T("1");
		AfxMessageBox(_T("���Ա����� ����������� ������ �� �����ϴ�."));		
	}
	m_nAcqType = _ttoi(strAcqCode);	
}

INT CAcqInfoChange::GetAcqType()
{
	return m_nAcqType;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ] ���ڷ� �׷�⵵, �׷��ȣ, ������������ �׷������� �����Ѵ�.
// [ �̴Ͻ��� ]
//////////////////////////////////////////////////////////////////////
INT CAcqInfoChange::MakeGroupInfo(CString strAcqYear, CString strWorkCode)
{
	// -- ���Գ⵵�� ���Ա��п� ���� ����/������ȣ/�׷����� �����Ѵ�.
	
	//================================================================
	// ��������
	//================================================================
	INT ids = -1;
	
	//================================================================
	// ���Գ⵵�� ���ٸ� ���� �����Ѵ�.
	//================================================================
	if( strAcqYear.IsEmpty() == TRUE )
	{
		ESLAfxMessageBox(_T("���Գ⵵�� �ùٸ��� �ʽ��ϴ�."));
		return -1;
	}
	
	//================================================================
	// 2. ���� Ÿ�Կ� ���� �׷��ȣ�� �����Ѵ�.
	//================================================================
	//=============================================
	//20080612 UPDATE BY PJW : ids�� -1�� �������� ids�� ���� ���� -> �����ϵ��� ����
	//switch( m_nAcqType )
	//{
	//case PURCHASE:			ids = MakeDivNoInfo(strAcqYear, strWorkCode);
	//	break;
	//case DONATE:			ids = MakeReceiptNoInfo(strAcqYear);
	//	break;	
	//	if( ids < 0 )	return ids ;
	//return 0;
	//}
	switch( m_nAcqType )
	{
	case PURCHASE:			
		ids = MakeDivNoInfo(strAcqYear, strWorkCode);
		if( ids < 0 )
		{
			return ids ;
		}
		break;
	case DONATE:			
		ids = MakeReceiptNoInfo(strAcqYear);
		if( ids < 0 )
		{
			return ids ;
		}	
		break;
	default:
		ids = MakeGroupNoInfo( strAcqYear );
		if( ids < 0 )
		{
			return ids ;
		}	
	}
	if( ids < 0 )	return ids ;
	//=============================================


	return 0;
}


INT CAcqInfoChange::MakeDivNoInfo(CString strAcqYear, CString strWorkCode)
{
	CStringArray strArrayNum;
	CString strWhere;
	CString strWorkNoUseYN;
	CString strNum;
	CESL_DataMgr* pDM = FindDM(_T("DM_������������"));

	// �׷��ȣ ����	
	if ( strWorkCode == _T("") ) strWhere.Format(_T("ACQ_YEAR='%s' AND WORKING_STATUS='2' AND WORK_CODE IS NULL"), strAcqYear);	
	else strWhere.Format(_T("ACQ_YEAR='%s' AND WORKING_STATUS='2' AND WORK_CODE='%s'"), strAcqYear, strWorkCode );		
	
	pDM ->RefreshDataManager(strWhere);	

	strArrayNum.RemoveAll();
	INT nRowCnt;
	nRowCnt = pDM->GetRowCount();
	//==============================================
	//20080612 ADD BY PJW : DM�� ROW�� 0�ϰ�� �޼��� ���� ����������.
	if( 0 >= nRowCnt )
	{
		return -1 ;
	}
	//==============================================
	
	for ( int i=0 ; i < nRowCnt ; i++ )
	{
		pDM->GetCellData(_T("������ȣ"), i, strNum);
		strArrayNum.Add(strNum);
	}
	
	SetSingleComboBoxStrData(_T("CM_������������"), _T("0"), &strArrayNum, _T("BP_�׷��ȣ"));

	SetWorkNo();

	return 0;
}

INT CAcqInfoChange::SetWorkNo()
{
	// �۾���ȣ ����
	CESL_DataMgr tmpDM;
	CStringArray strArrayNum;
	CString strAcqYear;
	CString strWorkCode;
	CString strWorkNoUseYN;
	CString strQuery;
	CString strNum;
	CString strWorkNo;
	INT		i;

	m_pCM->GetControlMgrData(_T("BP_�׷��ȣ"), strWorkNo );	
	m_pCM->GetControlMgrData(_T("BP_���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("BS_��������"), strWorkCode);

	if(m_pInfo->CONNECTION_INFO.IsEmpty()) return -1;
	
	tmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);	
	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL) ORDER BY WORK_NO"),
//DEL 			_T("SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL) ORDER BY WORK_NO"),
			strAcqYear,
			strWorkNo);
	else
		strQuery.Format(
			_T("SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE='%s') ORDER BY WORK_NO"),
//DEL 			_T("SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE='%s') ORDER BY WORK_NO"),
			strAcqYear,
			strWorkNo, 
			strWorkCode);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;

	strArrayNum.RemoveAll();
	INT cnt = tmpDM.GetRowCount();	
	for( i=0 ; i<cnt ; i++)
	{
		tmpDM.GetCellData(i, 0, strNum);
		strArrayNum.Add(strNum);
	}	

	if(strWorkCode.IsEmpty())
		strQuery.Format(_T("SELECT WORK_NO_USE_YN FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL AND MANAGE_CODE = UDF_MANAGE_CODE"), strAcqYear, strNum);
//DEL 		strQuery.Format(_T("SELECT WORK_NO_USE_YN FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL"), strAcqYear, strNum);
	else 
//DEL 		strQuery.Format(_T("SELECT WORK_NO_USE_YN FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE = '%s'"), strAcqYear, strNum, strWorkCode);
		strQuery.Format(_T("SELECT WORK_NO_USE_YN FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strAcqYear, strNum, strWorkCode);
	ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	strWorkNoUseYN = tmpDM.GetCellData(0, 0);

	if(strWorkNoUseYN == _T("Y")) 
	{
		// ���� ���� �߰�
		//strArrayNum.Add(_T("���λ���"));
	}
	
	SetSingleComboBoxStrData(_T("CM_������������"), _T("0"), &strArrayNum, _T("BP_�۾���ȣ"));	
	
	return 0;
}


INT CAcqInfoChange::MakeReceiptNoInfo(CString strAcqYear)
{
	CStringArray strArrayNum;
	CString strWhere;
	CString strNum;
	CESL_DataMgr* pDM = FindDM(_T("DM_������ȣ����"));
	
	strWhere.Format(_T("ACQ_YEAR='%s'"), strAcqYear );
	pDM ->RefreshDataManager(strWhere);	

	strArrayNum.RemoveAll();
	for ( int i=0 ; i<pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData(_T("������ȣ"), i, strNum);
		strArrayNum.Add(strNum);
	}

	SetSingleComboBoxStrData(_T("CM_������������"), _T("0"), &strArrayNum, _T("BP_�׷��ȣ"));			  

	// // SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '2007' AND SEQ_NO = 247 AND WORK_CODE='1') ORDER BY WORK_NO;

	return 0;
}

//===================================================
//2009.10.20 ADD BY PJW : 
INT CAcqInfoChange::MakeGroupNoInfo(CString strAcqYear)
{
	CStringArray strArrayNum;
	CString strWhere;
	CString strNum;
	CString strAcqCode;
	CESL_DataMgr* pDM = FindDM(_T("DM_�׷��ȣ����"));
	
	m_pCM->GetControlMgrData(_T("BP_���Ա���_�ڵ�"), strAcqCode);
	strWhere.Format(_T("CREATE_YEAR='%s' AND ACQ_CODE = '%s'"), strAcqYear, strAcqCode );
	
	
	
	pDM ->RefreshDataManager(strWhere);	

	strArrayNum.RemoveAll();
	for ( int i=0 ; i<pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData(_T("�׷��ȣ"), i, strNum);
		strArrayNum.Add(strNum);
	}

	SetSingleComboBoxStrData(_T("CM_������������"), _T("0"), &strArrayNum, _T("BP_�׷��ȣ"));			  
	return 0;
}
//===================================================


INT CAcqInfoChange::SetSingleComboBoxStrData(CString strCMAlias, CString strInitVal, CStringArray * parrGroupNo, CString strCtrlAlias)
{
	CESL_ControlMgr *pCM = FindCM(strCMAlias);
	CESL_ControlComboBox *pCMB = (CESL_ControlComboBox *)pCM->FindControl(strCtrlAlias);
	pCMB->ResetContent();
	
	INT cnt = parrGroupNo->GetSize();
	CString strTmpData;
	INT nInitVal = 0;
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = parrGroupNo->GetAt(i);
		if(strInitVal == strTmpData) nInitVal = i;
		pCMB->AddString(strTmpData);
	}
	pCMB->SetCurSel(nInitVal);
	return 0;
}



void CAcqInfoChange::OnSelchangecWORKCODE() 
{
	SetCtrlByAcqCode();	
}



void CAcqInfoChange::OnSelchangecmbBSDIVNOGROUPNO() 
{
	CESL_ControlMultiComboBox* pCmb = (CESL_ControlMultiComboBox*) GetDlgItem(IDC_cmbBSDIVNO_ACQ_CODE);
 	INT nCurSel;
 	CString strCulData;
 	nCurSel = pCmb->GetCurSel();
	pCmb->GetData(strCulData, nCurSel, 0);
 	if( strCulData == _T("1") )
 	{
 		SetWorkNo();
 	}
	
}


void CAcqInfoChange::OnchkPROVIDER() 
{
	BOOL bCheck;
	bCheck = ( (CButton*) GetDlgItem(IDC_chkPROVIDER) )->GetCheck();

	BOOL bCheck2;
	bCheck2 = ( (CButton*) GetDlgItem(IDC_chkAcqCode) )->GetCheck();
	if ( bCheck2 && bCheck )
	{
		// ���Ա����� �������� ������ ��� ����ó�� �����ϸ� �ȵȴ�.
		CString strAcqCode;
		m_pCM->GetControlMgrData(_T("BP_���Ա���_�ڵ�"), strAcqCode);
		if ( strAcqCode == _T("2") )
		{
			( (CButton*) GetDlgItem(IDC_chkPROVIDER) )->SetCheck(FALSE);
			ESLAfxMessageBox(_T("���Ա����� �������� ����� ��� ����ó�� ������ �� �����ϴ�."));
			return;
		}
	}
	
	if ( bCheck )
	{
		( (CComboBox*) GetDlgItem (IDC_cmbPC_PROVIDER) )->EnableWindow(TRUE);	
		( (CButton*) GetDlgItem (IDC_chkDONATOR1))->SetCheck(FALSE);
		GetDlgItem(IDC_edtDONATOR1		)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSEARCH_DONATOR1)->EnableWindow(FALSE);
		ESLAfxMessageBox(_T("�����ڷḸ ����ó ������ ���� �˴ϴ�."));
	}
	else
	{
		( (CComboBox*) GetDlgItem (IDC_cmbPC_PROVIDER) )->EnableWindow(FALSE);	
	}
	
}

void CAcqInfoChange::OnchkAcqCode() 
{
	//=====================================================
	//20080618 UPDATE BY PJW : üũ�ڽ��� ���� ��Ʈ�� ��Ȱ��ȭ��Ų��.
	//SetCtrlByAcqCode();
	BOOL bCheck;
	bCheck = ( (CButton*) GetDlgItem(IDC_chkAcqCode) )->GetCheck();

	if(bCheck)
	{
		//===================================================
		//2009.03.28 UPDATE BY LKS : üũ�ڽ� Ŭ���� �۾���ȣ ����ȵǾ������Ƿ� ����
		//SetCtrlByAcqCode();
		//===================================================
		GetDlgItem(IDC_cmbBSDIVNO_ACQ_CODE	)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnBSDIVNO_CREATE	)->EnableWindow(TRUE);
		GetDlgItem(IDC_cmbBSDIVNO_GROUP_NO	)->EnableWindow(TRUE);
		GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->EnableWindow(TRUE);
		GetDlgItem(IDC_cWORK_CODE			)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_cmbBSDIVNO_ACQ_CODE	)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnBSDIVNO_CREATE	)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbBSDIVNO_GROUP_NO	)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->EnableWindow(FALSE);
		GetDlgItem(IDC_cWORK_CODE			)->EnableWindow(FALSE);
	}
	//=====================================================

	//===================================================
	//2009.09.15 UPDATE BY LKS : Default�� 1��°�� ����
	( (CComboBox*) GetDlgItem (IDC_cmbBSDIVNO_ACQ_CODE) )->SetCurSel(0);
	//===================================================
	OnSelchangecmbBSDIVNOACQCODE();
	//===================================================
				
}

HBRUSH CAcqInfoChange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
//=================================================
//20080612 ADD BY PJW : ų��Ŀ�� �Ͼ� ������ ������ȿ���� �˻��Ѵ�.
void CAcqInfoChange::OnKillfocuscWORKCODE() 
{
	SetCtrlByAcqCode();	
}
//=================================================
