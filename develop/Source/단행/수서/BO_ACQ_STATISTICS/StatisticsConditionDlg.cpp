// StatisticsConditionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StatisticsConditionDlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/DivisionNo_List_Dlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/RequisitionNo_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsConditionDlg dialog

// ������
CStatisticsConditionDlg::CStatisticsConditionDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBasicStatistics(CStatisticsConditionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatisticsConditionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strIs90 = _T("");
	m_arrDataClass.RemoveAll();
	m_arrDateClass.RemoveAll();
	m_pComboMgr = NULL;
	m_nAcqType = ACQ_TYPE_INVALID;
	m_arrHorizontalData.RemoveAll();
	m_arrHorizontalDesc.RemoveAll();
	m_arrVerticalData.RemoveAll();
	m_arrVerticalDesc.RemoveAll();
	m_pThread = NULL;
	m_bCreate = FALSE;
	m_strUserCodeAlias.RemoveAll();
}

// �Ҹ���
CStatisticsConditionDlg::~CStatisticsConditionDlg()
{
	ThreadTerminate();


}

VOID CStatisticsConditionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsConditionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsConditionDlg, CDialog)
	//{{AFX_MSG_MAP(CStatisticsConditionDlg)
	ON_BN_CLICKED(IDC_btnSTAT_GROUP_VIEW, OnbtnSTATGROUPVIEW)
	ON_CBN_SELCHANGE(IDC_cmbSTAT_DATA_CLASS, OnSelchangecmbSTATDATACLASS)
	ON_EN_KILLFOCUS(IDC_edtSTAT_GROUPINFO, OnKillfocusedtSTATGROUPINFO)
	ON_EN_KILLFOCUS(IDC_edtSTAT_ACQ_YEAR, OnKillfocusedtSTATACQYEAR)
	ON_CBN_SELCHANGE(IDC_cmbSTAT_ACQ_CODE, OnSelchangecmbSTATACQCODE)
	ON_CBN_SELCHANGE(IDC_cmbSTAT_BASIC_CLASS, OnSelchangecmbSTATBASICCLASS)
	ON_MESSAGE(JOB_CANCEL_MSG, OnJobCancel)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsConditionDlg message handlers
// �ʱ�ȭ
BOOL CStatisticsConditionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// SM�� ����
	if( InitESL_Mgr(_T("K2UPGRADE_������Ǽ���")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	GetManageValue(_T("��Ÿ"), _T("����"), _T("�����"), _T(""), m_strIs90);

	// CM/DM�� ����
	m_pCM = FindCM(_T("CM_������Ǽ���"));

	// LC�� ��� ����
	/*
	if( _T("Y") == m_strIs90 )	m_pDM = FindDM(_T("DM_����ڷ�(�����)"));
	else					m_pDM = FindDM(_T("DM_����ڷ�"));
	*/
	m_pDM = FindDM(_T("DM_����ڷ�"));

	if( m_pCM == NULL || m_pDM == NULL )
	{
		ESLAfxMessageBox(_T("��� ���Ǽ��� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		return FALSE;
	}
	
	// ����������ڵ� ��뿩�θ� ����
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USERDEFINECODE'"), m_strUseCodeYN);

	// ��Ƽ �޺��ڽ��� ����
	m_pComboMgr =  new CMulitComboBoxManager(this);

	// �޺��ڽ��� ������ �ʱ�ȭ
	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("��� ���Ǽ��� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	OnSelchangecmbSTATBASICCLASS();	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  
}

BOOL CStatisticsConditionDlg::Init()
{
	INT nUseCodeCnt = 0;
	CString strAcqCode, strTmp ;



	// ������ ���Ǽ�����.
	m_arrHorizontalData.Add(_T("��ü�ڷ�"));
	m_arrHorizontalData.Add(_T("KDC��������"));
	m_arrHorizontalData.Add(_T("DDC��������"));
	
	m_arrHorizontalDesc.Add(_T("��ü�ڷ�"));
	m_arrHorizontalDesc.Add(_T("KDC��������ȣ����"));
	m_arrHorizontalDesc.Add(_T("DDC��������ȣ����"));


	// ������ ���Ǽ�����.
	m_arrVerticalData.Add(_T("�������к����"));
	m_arrVerticalData.Add(_T("�ڷᱸ�к����"));
	m_arrVerticalData.Add(_T("��ϱ��к����"));
	m_arrVerticalData.Add(_T("��ġ���к����"));
	m_arrVerticalData.Add(_T("������ϱ��к����"));

	m_arrVerticalDesc.Add(_T("��������"));
	m_arrVerticalDesc.Add(_T("�ڷᱸ��"));
	m_arrVerticalDesc.Add(_T("��ϱ���"));
	m_arrVerticalDesc.Add(_T("��ġ��ȣ"));
	m_arrVerticalDesc.Add(_T("������ϱ���"));


	// ����뿡 �߰��Ǵ� ����
	if( m_strIs90 == _T("Y") )
	{
		// LC�� ��� ����
		//m_arrHorizontalData.Add(_T("LC��������"));
		//m_arrHorizontalDesc.Add(_T("LC������ȣ"));

		m_arrVerticalData.Add(_T("�μ������"));
		m_arrVerticalDesc.Add(_T("�����μ�"));

		if( m_nAcqType == DONATE )
		{
			m_arrVerticalData.Add(_T("�����ں����"));
			m_arrVerticalDesc.Add(_T("�����ں�"));
		}
	}

	// ������ϱ��� �̸��� �������� �ٲ۴ٸ� �����ؾ��� ����.
	/*
	if( strIs90 == _T("Y") )	
	{
		m_arrVerticalData.Add(_T("���������"));
		m_arrVerticalDesc.Add(_T("����������"));
	}
	else
	{
		m_arrVerticalData.Add(_T("������ϱ��к����"));
		m_arrVerticalDesc.Add(_T("������ϱ���"));
	}
	*/

	// ����� ���� �ڵ带 ����� ��� �߰��Ǿ���� ���ǵ�
	// 2004.11.06 ����� �����ڵ� �߰�.
	// BEGIN:
	if( m_strUseCodeYN == _T("Y") )
	{
		CString strYN, strName;
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 91"), strYN);
		if( strYN == _T("Y") ) 
		{
			m_pDM->GetOneValueQuery(_T("select class_alias from MN_USER_DEFINE_TBL where class_code = 91"), strName);
			strTmp.Format(_T("%s�����"), strName);
			m_arrVerticalData.Add(strTmp);
			m_arrVerticalDesc.Add(_T("�����з��ڵ�1"));
			m_strUserCodeAlias.Add(_T("�����з��ڵ�1"));
		}
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 92"), strYN);
		if( strYN == _T("Y") ) 
		{
			m_pDM->GetOneValueQuery(_T("select class_alias from MN_USER_DEFINE_TBL where class_code = 92"), strName);
			strTmp.Format(_T("%s�����"), strName);
			m_arrVerticalData.Add(strTmp);
			m_arrVerticalDesc.Add(_T("�����з��ڵ�2"));
			m_strUserCodeAlias.Add(_T("�����з��ڵ�2"));
		}
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 93"), strYN);
		if( strYN == _T("Y") ) 
		{
			m_pDM->GetOneValueQuery(_T("select class_alias from MN_USER_DEFINE_TBL where class_code = 93"), strName);
			strTmp.Format(_T("%s�����"), strName);
			m_arrVerticalData.Add(strTmp);
			m_arrVerticalDesc.Add(_T("�����з��ڵ�3"));
			m_strUserCodeAlias.Add(_T("�����з��ڵ�3"));
		}
	}
	// END:


	// ��������ڷ������ �ƴ϶�� ���Ա��� �޺��ڽ��� ������ �� ������ �Ѵ�.
 	//if( m_nAcqType != ACQ_TYPE_INVALID )	GetDlgItem(IDC_cmbSTAT_ACQ_CODE)->EnableWindow(FALSE);
	
	// ������ ������ ������ �ڷ���� �� ��蹰����� �޺��ڽ��� �����Ѵ�.
	if( _T("Y") == m_strIs90 )
	{
		m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("LC��������"), &m_arrHorizontalData, _T("�ڷ����"));
		// 2006.02.27 KBS - �����, �о��ڵ庰��踦 ����Ʈ�� �䱸
		m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("�о��ڵ庰���"), &m_arrVerticalData, _T("��蹰���"));
	}
	else
	{
		m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("KDC��������"), &m_arrHorizontalData, _T("�ڷ����"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &m_arrVerticalData, _T("��蹰���"));
	}
	

	//  CM ���Ա��ж��� ������ ���Ա��� ���¸� �����Ѵ�.
	//strAcqCode.Format(_T("%d"), m_nAcqType);
	//m_pCM->SetControlMgrData(_T("���Ա���"), strAcqCode);

	// �׷����� Ŭ������ �����ϰ� ���Ա��л��¸� �����Ѵ�.
	m_pGroupCollection = new CGroupCollection(this);
	m_pGroupCollection->SetAcqType(m_nAcqType);
	

	// �ڷᱸ����/�ڷᱸ�� �޺��ڽ��� �ʱ�ȭ �� ����Ѵ�.
	SetDataClassCtrl();
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ڷᱸ��/�ڷᱸ������ �޺��ڽ��� �ʱ�ȭ �� �����Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CStatisticsConditionDlg::SetDataClassCtrl()
{
	CString strAcqCode;

	// CM�� ���Ա��а����� ��������� �ٽ� �����Ѵ�.
	m_pCM->GetControlMgrData(_T("���Ա���"), strAcqCode);
	m_nAcqType = (ACQ_TYPE)_ttoi(strAcqCode.GetBuffer(0));

	// �׷�����Ŭ������ �ʱ�ȭ�Ѵ�.
	m_pGroupCollection->RemoveAll();
	m_pGroupCollection->SetAcqType(m_nAcqType);
	
	// ���Ա��� ���¿� ���� �ڷᱸ�� �� �ڷᱸ���� �޺��ڽ��� �����Ѵ�.

	m_arrDataClass.RemoveAll();
	m_arrDateClass.RemoveAll();

	if( m_nAcqType == ACQ_TYPE_INVALID || m_nAcqType == EXCHANGE )
	{
		m_arrDataClass.Add(_T("�Ⱓ����豸��"));
		m_arrDateClass.Add(_T("�Է���"));
		m_arrDateClass.Add(_T("�����"));
	}
	else if( m_nAcqType == PURCHASE )
	{		
		m_arrDataClass.Add(_T("�Ⱓ����豸��"));
		m_arrDataClass.Add(_T("��������豸��"));

		m_arrDateClass.Add(_T("�Է���"));
		m_arrDateClass.Add(_T("�ֹ���"));
		m_arrDateClass.Add(_T("�˼���"));
		m_arrDateClass.Add(_T("�����"));
		GetDlgItem(IDC_btnSTAT_GROUP_VIEW)->SetWindowText(_T("������ȣ����"));
		GetDlgItem(IDC_stcSTAT_GROUPINFO)->SetWindowText(_T("������ȣ"));
	}
	else if( m_nAcqType == DONATE )
	{
		m_arrDataClass.Add(_T("�Ⱓ����豸��"));
		m_arrDataClass.Add(_T("������ȣ����豸��"));

		m_arrDateClass.Add(_T("�Է���"));
		m_arrDateClass.Add(_T("�����"));
		GetDlgItem(IDC_btnSTAT_GROUP_VIEW)->SetWindowText(_T("������ȣ����"));
		GetDlgItem(IDC_stcSTAT_GROUPINFO)->SetWindowText(_T("������ȣ"));
		GetDlgItem(IDC_cmbSTAT_WORK_CODE)->EnableWindow(FALSE);
	}	

	//===================================================
	//2009.10.07 ADD BY LKS : �߰��Ǵ� ���Ա����� ��� Error�߻����� �ʵ��� Default�� ����
	else
	{
		m_arrDataClass.Add(_T("�Ⱓ����豸��"));
		m_arrDateClass.Add(_T("�Է���"));
		m_arrDateClass.Add(_T("�����"));
	}
	//===================================================
	
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrDataClass.GetAt(0), &m_arrDataClass, _T("�ڷᱸ��"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrDateClass.GetAt(0), &m_arrDateClass, _T("�ڷᱸ����"));


	// 2006.04.01 KBS - ����� �䱸���� : ��������谡 ����Ʈ�� �����ǵ��� ����
	if( m_strIs90 == _T("Y") )
	{
		CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�ڷᱸ��"));
		pCMB->SetCurSel(1);
	}

	// ���ҽ����� ����Ѵ�.
	SetCtrlByDataClass();
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���õǾ��� �ڷᱸ�а��� ���� ���ҽ����� ǥ�ø� �����Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CStatisticsConditionDlg::SetCtrlByDataClass()
{
	INT nCount1, nCount2, i, nCurSel;
	INT nArrCodeResourceID1[10], nArrCodeResourceID2[10];
	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�ڷᱸ��"));

	nCount1 = nCount2 = 0;

	// �Ⱓ�� ��迡 ��Ÿ������ ���ҽ���
	nArrCodeResourceID1[nCount1++] = IDC_stcSTAT_DATE_CLASS;
	nArrCodeResourceID1[nCount1++] = IDC_cmbSTAT_DATE_CLASS;
	nArrCodeResourceID1[nCount1++] = IDC_edtSTAT_DATE1;
	nArrCodeResourceID1[nCount1++] = IDC_stcSTAT_DATE_RANGE;
	nArrCodeResourceID1[nCount1++] = IDC_edtSTAT_DATE2;
	
	// �׷�����(����/������ȣ) ��迡 ��Ÿ������ ���ҽ���
	nArrCodeResourceID2[nCount2++] = IDC_edtSTAT_GROUPINFO;
	nArrCodeResourceID2[nCount2++] = IDC_btnSTAT_GROUP_VIEW;
	nArrCodeResourceID2[nCount2++] = IDC_edtSTAT_ACQ_YEAR;
	nArrCodeResourceID2[nCount2++] = IDC_stcSTAT_ACQ_YEAR;
	nArrCodeResourceID2[nCount2++] = IDC_stcSTAT_GROUPINFO;
	nArrCodeResourceID2[nCount2++] = IDC_stcSTAT_WORK_CODE;
	nArrCodeResourceID2[nCount2++] = IDC_cmbSTAT_WORK_CODE;
	
	nCurSel = ( (CComboBox*) GetDlgItem(IDC_cmbSTAT_DATA_CLASS) )->GetCurSel();
	if ( nCurSel == 0 )
	{
		for( i=0 ; i<nCount1 ; i++ )	GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_SHOW);
		for( i=0 ; i<nCount2 ; i++ )	GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
	}
	else
	{
		for( i=0 ; i<nCount1 ; i++ )	GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_HIDE);
		for( i=0 ; i<nCount2 ; i++ )	GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_SHOW);
	}

	//=========================================================================
	//20080129 UPDATE BY PJW : 1.���Ա��п� ���� ���ҽ� ǥ��
	//20080618 UPDATE BY PJW : ���Ա��п����� �������� �����ش�.
	CString strAcqCode;
	INT nAcqCurSel = 0;

	CESL_ControlMultiComboBox * pcmbStatAcqCode = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbSTAT_ACQ_CODE);
	CESL_ControlMultiComboBox * pcmbStatWorkCode = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbSTAT_WORK_CODE);
	
	nAcqCurSel = pcmbStatAcqCode->GetCurSel();
	pcmbStatAcqCode->GetLBText( nAcqCurSel, strAcqCode);
	
	if( ( strAcqCode == _T("1") ) && ( nCurSel != 0 ) )
	{
		( (CComboBox*) GetDlgItem(IDC_stcSTAT_WORK_CODE) )->ShowWindow(SW_SHOW);
		( (CComboBox*) GetDlgItem(IDC_stcSTAT_WORK_CODE) )->EnableWindow(TRUE);
		pcmbStatWorkCode->ShowWindow(SW_SHOW);
		pcmbStatWorkCode->EnableWindow(TRUE);
		pcmbStatWorkCode->SelectItem(_T("1"));
	}
	else if( strAcqCode == _T("2") )
	{
		( (CComboBox*) GetDlgItem(IDC_stcSTAT_WORK_CODE) )->ShowWindow(SW_HIDE);
		pcmbStatWorkCode->ShowWindow(SW_HIDE);
		pcmbStatWorkCode->SelectItem(_T("    "));
	}

	//INT nAcqCurSel = 0;
	//nAcqCurSel = ( (CComboBox*) GetDlgItem(IDC_cmbSTAT_ACQ_CODE) )->GetCurSel();
	//if( nAcqCurSel == 1 && nCurSel != 0 )
	//{
	//	for( i=0 ; i<nCount2 ; i++ )
	//		{
	//			if( nArrCodeResourceID2[i] == IDC_stcSTAT_WORK_CODE || nArrCodeResourceID2[i] == IDC_cmbSTAT_WORK_CODE)
	//			{//GetDlgItem(IDC_cmbSTAT_WORK_CODE)->EnableWindow(FALSE);
	//				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_SHOW);
	//				GetDlgItem(nArrCodeResourceID2[i])->EnableWindow(TRUE);
	//			}		
	//		}
	//}
	//else if( nAcqCurSel == 2 )
	//{
	//	for( i=0 ; i<nCount2 ; i++ )
	//		{
	//			if( nArrCodeResourceID2[i] == IDC_stcSTAT_WORK_CODE || nArrCodeResourceID2[i] == IDC_cmbSTAT_WORK_CODE)
	//			{
	//				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
	//			}
	//		}
	//}
	//=========================================================================

	// ���Ա��� �޺��� ������ ���� ���ҽ����� ǥ�ø� �����Ѵ�.

	/*
	nCurSel = ( (CComboBox*) GetDlgItem(IDC_cmbSTAT_ACQ_CODE) )->GetCurSel();

	//nCurSel = pCMB->GetCurSel();
	switch( nCurSel )
	{
	case 0:
		{
			for( i=0 ; i<nCount1 ; i++ )	GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_SHOW);
			for( i=0 ; i<nCount2 ; i++ )	GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
		}
	case 1:
		{
			for( i=0 ; i<nCount1 ; i++ )	GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_SHOW);
			for( i=0 ; i<nCount2 ; i++ )	GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
		}
		break;

	case 2:
		{
			for( i=0 ; i<nCount1 ; i++ )	GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_HIDE);
			for( i=0 ; i<nCount2 ; i++ )	GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_SHOW);
		}
		break;

	default:
		{
			for( i=0 ; i<nCount1 ; i++ )	GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_HIDE);
			for( i=0 ; i<nCount2 ; i++ )	GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
		}
		break;
	}
	*/

}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ڷᱸ���� ����/������ȣ���϶� ����Ʈâ�� ���� ����ڷḦ �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ���Գ⵵�� üũ�Ѵ�.(�ʼ��׸�)
// 2. ���Ա��п����� ����/���� ��ȣ ����Ʈ â�� ����.
// 3. ����ڷḦ �����Ѵ�.( �����ʴ´�.) -> �ٸ����Ǽ����� �� �� �����Ƿ�
//
/////////////////////////////////////////////////////////////////////////////
VOID CStatisticsConditionDlg::OnbtnSTATGROUPVIEW() 
{	
	CString strAcqYear, strSelDivNo, strWorkCode;

	//=======================================================================
	// 1. ���Գ⵵�� üũ�Ѵ�.(�ʼ��׸�)
	//=======================================================================
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	if( strAcqYear.IsEmpty() == TRUE ) 
	{
		ESLAfxMessageBox(_T("���Գ⵵ ������ �Է��� �ֽʽÿ�"));
		return;
	}

	//=======================================================================
	// 2. ���Ա��п����� ����/���� ��ȣ ����Ʈ â�� ����.
	//=======================================================================
	switch( m_nAcqType )
	{
	case PURCHASE:
	{
		m_pCM->GetControlMgrData(_T("��������"), strWorkCode);

		CDivisionNo_List_Dlg dlg(this);
		dlg.SetWorkStatus(_T("��ü�ڷ�"));
		dlg.SetAcquisitYear(strAcqYear);
		dlg.SetWorkCode(strWorkCode);
		dlg.SetGroupCollection(m_pGroupCollection);

			if( IDOK == dlg.DoModal() )
		{
			strSelDivNo = dlg.GetSelDivNo();
			m_pCM->SetControlMgrData(_T("�׷��ȣ"), strSelDivNo);
			m_pCM->SetControlMgrData(_T("��������"), m_pGroupCollection->GetWorkCode());
			m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pGroupCollection->GetAcqYear());
		}
	}	
		break;
	case DONATE:
	{
		CRequisitionNo_List_Dlg dlg(this);
		dlg.SetSpeciesType(SPECIES_TYPE_INVALID);
		dlg.SetAcqYear(strAcqYear);
		dlg.SetGroupCollection(m_pGroupCollection);

			if( IDOK == dlg.DoModal() )
		{
			strSelDivNo = dlg.GetSelInfo();
			m_pCM->SetControlMgrData(_T("�׷��ȣ"), strSelDivNo);
			m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pGroupCollection->GetAcqYear());
		}
	}	
		break;
	default:	return;
		break;
	}

	//=======================================================================
	// 3. ����ڷḦ �����Ѵ�.
	//=======================================================================
	OnOK();
	
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���Ǽ���Ŭ������ ������ �����Ͽ� �������� �����ϰ� ����ڷḦ �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. �ڷᱸ�п� �ش��ϴ� ���ǵ��� ��ȿ���� �Ǵ��ϰ� ������ �����Ѵ�.
// 2. ���Ա����� ���Ǽ���Ŭ������ �����Ѵ�.
// 3. ���ο��� ������ �������� ���Ǽ���Ŭ������ �����Ѵ�.
// 4. ���ο��� ������ ������ Field������ ���Ǽ���Ŭ������ �����Ѵ�.
// 5. ���Ǽ���Ŭ������ ������ ������ �������� �����Ѵ�.
// 6. ����ڷḦ �����Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
VOID CStatisticsConditionDlg::OnOK() 
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT ids, nCurSel, i;
	CString strFieldName, strVerticalDesc;
	CESL_ControlComboBox* pCMB	= (CESL_ControlComboBox*)m_pCM->FindControl(_T("�ڷ����"));
	CESL_ControlComboBox* pCMB1 = (CESL_ControlComboBox*)m_pCM->FindControl(_T("��蹰���"));
	
	m_nStartRow = 0;
	m_bDonatorFlag = FALSE;

	//=======================================================================
	// 1. �ڷᱸ�п� �ش��ϴ� ���ǵ��� ��ȿ���� �Ǵ��ϰ� ������ �����Ѵ�.
	//=======================================================================
	ids = CheckValidateInputValue();
	if( ids < 0 )	 return ;


	//=======================================================================
	// 2. ���Ա����� ���Ǽ���Ŭ������ �����Ѵ�.
	//=======================================================================
	m_pStatCondition->SetAcqType(m_nAcqType);
	
	//=======================================================================
	// 3. ���ο��� ������ �������� ���Ǽ���Ŭ������ �����Ѵ�.
	//=======================================================================
	nCurSel = pCMB->GetCurSel();
	m_pStatCondition->SetClassNoType(m_arrHorizontalDesc.GetAt(nCurSel));

	//=======================================================================
	// 4. ���ο��� ������ ������ Field������ ���Ǽ���Ŭ������ �����Ѵ�.
	//=======================================================================
	nCurSel = pCMB1->GetCurSel();
	strVerticalDesc = m_arrVerticalDesc.GetAt(nCurSel);
	if		( strVerticalDesc == _T("��������") )		strFieldName = _T("I.WORK_CODE");
	else if	( strVerticalDesc == _T("��ϱ���") )		strFieldName = _T("B.REG_CODE");
	else if	( strVerticalDesc == _T("��ġ��ȣ") )		strFieldName = _T("B.SEPARATE_SHELF_CODE");
	else if	( strVerticalDesc == _T("�ڷᱸ��") )		strFieldName = _T("I.MAT_CODE");
	else if	( strVerticalDesc == _T("������ϱ���") )	strFieldName = _T("I.SUB_REG_CODE");
	else if	( strVerticalDesc == _T("�����μ�") )		strFieldName = _T("P.CLASS_CODE");
	else if	( strVerticalDesc == _T("�����ں�")	)		
	{
		m_bDonatorFlag = TRUE;
		strFieldName = _T("P.DONATOR_KEY");
	}
	else												strFieldName.Empty();

	// 2004.11.06 ����� �����ڵ� ���.
	// BEGIN:
	for( i=0; i<m_strUserCodeAlias.GetSize(); i++ )
	{
		if( strVerticalDesc == m_strUserCodeAlias[i] ) 
			strFieldName.Format(_T("I.USER_DEFINE_CODE%d"), i+1);
	}
	// END:
	m_pStatCondition->SetVerticalInfo(strVerticalDesc, strFieldName);
	m_pStatCondition->SetDonatorFlag(m_bDonatorFlag);


	//=======================================================================
	// 5. ���Ǽ���Ŭ������ ������ ������ �������� �����Ѵ�.
	//=======================================================================
	ids = m_pStatCondition->SetWhereStmt();
	if( ids < 0 )	
	{
		ESLAfxMessageBox(_T("����ڷḦ ������ �� �����ϴ�."));
		return;
	}

	//=======================================================================
	// 6. ����ڷḦ �����Ѵ�.
	//=======================================================================
	MakeStatistics();
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ڷᱸ���� �޺������ �ش縮�ҽ��� ǥ�ø� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
VOID CStatisticsConditionDlg::OnSelchangecmbSTATDATACLASS() 
{
	SetCtrlByDataClass();
}

BOOL CStatisticsConditionDlg::PreTranslateMessage(MSG* pMsg) 
{
 	if( pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		GetDlgItem(IDOK)->SetFocus();
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CStatisticsConditionDlg::OnKillfocusedtSTATGROUPINFO() 
{
	if(!GetDlgItem(IDC_edtSTAT_GROUPINFO)->IsWindowVisible()) return ;

	CString strAcqYear, strGroupInfo, strWorkCode;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("�׷��ȣ"), strGroupInfo);
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);
	strGroupInfo = CheckValidateGroupNo(strGroupInfo, strAcqYear, strWorkCode);
	
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), strGroupInfo);
}

VOID CStatisticsConditionDlg::OnKillfocusedtSTATACQYEAR() 
{
	if(!GetDlgItem(IDC_edtSTAT_ACQ_YEAR)->IsWindowVisible()) return ;
	OnKillfocusedtSTATGROUPINFO();
}

VOID CStatisticsConditionDlg::OnSelchangecmbSTATACQCODE() 
{
	SetDataClassCtrl();
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ڷᱸ�а��� ���� ��ȿ���� �˻��Ѵ�. 
//	(�Ⱓ�� : CheckValidateDateInputValue(), ����/������ȣ�� : CheckValidateGroupInputValue() )
// [ �̴Ͻ��� ]
/////////////////////////////////////////////////////////////////////////////
INT CStatisticsConditionDlg::CheckValidateInputValue()
{
	INT ids, nCurSel;
	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�ڷᱸ��"));
	
	nCurSel = pCMB->GetCurSel();
	switch( nCurSel )
	{
	case 0:		ids = CheckValidateDateInputValue();
		break;
	case 1:		ids = CheckValidateGroupInputValue();
		break;
	default:	ids = -1;
		break;
	}

	return ids;
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ڷᱸ���� �Ⱓ����豸���� ��� ������ ��ȿ���� �Ǵ��ϰ� ���Ǽ��� Ŭ������ �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ����Ʈ�ڽ����� ��¥�� �����´�.
// 2. CStaticsCondition Ŭ������ �����Ѵ�.
// 3. �ڷᱸ������ ���� ���� �˻��� DATE_FIELD�� �����Ѵ�.( CStaticsCondition )
//
/////////////////////////////////////////////////////////////////////////////
INT CStatisticsConditionDlg::CheckValidateDateInputValue()
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT nCurSel;
	CString strDate1, strDate2, strDateClass;
	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�ڷᱸ����"));

	//=======================================================================
	// 1. ����Ʈ�ڽ����� ��¥�� �����´�.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("��¥1"), strDate1);
	m_pCM->GetControlMgrData(_T("��¥2"), strDate2);
	if( strDate1.IsEmpty() == TRUE && strDate2.IsEmpty() == TRUE )
	{
		ESLAfxMessageBox(_T("�˻��� ������ �Է��� �ֽʽÿ�."));
		return -1;
	}
	else if( strDate1.IsEmpty() == TRUE && strDate2.IsEmpty() == FALSE )
	{
		ESLAfxMessageBox(_T("�˻� �������� �Է��� �ֽʽÿ�."));
		return -1;
	}
	
	
	//=======================================================================
	// 2. CStaticsCondition Ŭ������ �����Ѵ�.
	//=======================================================================
	m_pStatCondition->InitDateInfo();
	m_pStatCondition->SetStatType(STATISTICS_BY_DATE);
	m_pStatCondition->AddDateInfo(_T("��¥1"), strDate1);
	m_pStatCondition->AddDateInfo(_T("��¥2"), strDate2);

	//=======================================================================
	// 3. �ڷᱸ������ ���� ���� �˻��� DATE_FIELD�� �����Ѵ�.( CStaticsCondition )
	//=======================================================================
	nCurSel = pCMB->GetCurSel();
	strDateClass = m_arrDateClass.GetAt(nCurSel);	
	
	if		( strDateClass == _T("�Է���") )
	{
		m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("B.INPUT_DATE"));
	}
	else if	( strDateClass == _T("�����") )
	{
		m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("B.REG_DATE"));
	}
	else if	( strDateClass == _T("�ֹ���") )
	{
		m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("P.ORDER_DATE"));
	}
	else if	( strDateClass == _T("�˼���") )
	{
		m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("P.CHECKIN_DATE"));
	}
	else	
	{
		ESLAfxMessageBox(_T("�ڷᱸ������ �����Ͽ� �ֽʽÿ�."));
		return -1;
	}
		

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ڷᱸ���� ����/������ȣ����豸���� ��� ������ ��ȿ���� �Ǵ��ϰ� ���Ǽ���Ŭ������ �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ȭ�鿡�� ���Գ⵵/�׷��ȣ/�������� ���� �����´�.
// 2. �׷��ȣ(����/������ȣ)�� ��ȿ���� �Ǵ��Ѵ�.
// 3. ����/������ȣ�� ���ٸ� ���������Ѵ�.
// 4. CStaticsCondition Ŭ����(���Ǽ���Ŭ����)�� �׷������� �����Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
INT CStatisticsConditionDlg::CheckValidateGroupInputValue()
{
	//=======================================================================
	// ��������
	//=======================================================================
	CString strAcqYear, strGroupInfo, strWorkCode;

	//=======================================================================
	// 1. ȭ�鿡�� ���Գ⵵/�׷��ȣ/�������� ���� �����´�.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("�׷��ȣ"), strGroupInfo);
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);
	
	//=======================================================================
	// 2. �׷��ȣ(����/������ȣ)�� ��ȿ���� �Ǵ��Ѵ�.
	//=======================================================================
	strGroupInfo = CheckValidateGroupNo(strGroupInfo, strAcqYear, strWorkCode);

	//=======================================================================
	// 3. ����/������ȣ�� ���ٸ� ���������Ѵ�.
	//=======================================================================
	if( strGroupInfo.IsEmpty() == TRUE )
	{
		switch( m_nAcqType )
	{
		case PURCHASE:	ESLAfxMessageBox(_T("����ڷḦ ������ ���������� �����ϴ�."));
			break;
		case DONATE:	ESLAfxMessageBox(_T("����ڷḦ ������ ������ȣ������ �����ϴ�."));
			break;
		default:		ESLAfxMessageBox(_T("����ڷḦ ������ ������ �����ϴ�."));
			break;
		}
		return -1;
	}

	//=======================================================================
	// 4. CStaticsCondition Ŭ����(���Ǽ���Ŭ����)�� �׷������� �����Ѵ�.
	//=======================================================================
	m_pGroupCollection->MakeGroupInfo(strAcqYear, strGroupInfo, strWorkCode);
	m_pStatCondition->SetStatType(STATISTICS_BY_DIVNO);
	m_pStatCondition->SetGroupCollection(m_pGroupCollection);

	return 0;
}

INT CStatisticsConditionDlg::MakeStatistics()
{
	m_pDM->FreeData();
	ShowDataProcessingDlg();
	m_bRunning = TRUE;
	//m_pThread = AfxBeginThread(GetStatistics, this);
	GetAcqStatistics();
	return 0;
}

UINT CStatisticsConditionDlg::GetStatistics(LPVOID pParam )
{
    CStatisticsConditionDlg* pObject = (CStatisticsConditionDlg*)pParam;
	//pObject->GetStatistics();
	pObject->GetAcqStatistics();
	return 0;   // thread completed successfully
}

VOID CStatisticsConditionDlg::ThreadTerminate()
{
	if(!m_pThread) return ;
	if(AfxIsValidAddress(m_pThread, sizeof(CWinThread), 0))
	{
		m_pThread->m_bAutoDelete = FALSE;
		m_bRunning = FALSE;
		WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		m_pThread = NULL;
	}
}

BOOL CStatisticsConditionDlg::Create(CWnd *pWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pWnd);
}

VOID CStatisticsConditionDlg::OnSelchangecmbSTATBASICCLASS() 
{
	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�ڷ����"));

	INT nCurSel = pCMB->GetCurSel();
	m_pStatCondition->SetClassNoType(m_arrHorizontalDesc.GetAt(nCurSel));

	::SendMessage(pParentMgr->GetSafeHwnd(), CHANGE_CLASS_NO_TYPE, 0, 0);
}

VOID CStatisticsConditionDlg::OnJobCancel(WPARAM w,LPARAM l)
{
	ThreadTerminate();
}

HBRUSH CStatisticsConditionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}