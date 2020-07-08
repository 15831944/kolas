// StatisticsConditionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StatisticsConditionDlg.h"
#include "StatisticsCondition.h"
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
	ON_WM_CTLCOLOR() 
	ON_MESSAGE(JOB_CANCEL_MSG, OnJobCancel)
	ON_EN_KILLFOCUS(IDC_edtEND_REG_NO, OnKillfocusedtENDREGNO)
	ON_EN_KILLFOCUS(IDC_edtSTART_REG_NO, OnKillfocusedtSTARTREGNO)
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
	if( _T("Y") == m_strIs90 )	m_pDM = FindDM(_T("DM_�ڻ�����ڷ�(�����)"));
	else					m_pDM = FindDM(_T("DM_�ڻ�����ڷ�"));
	*/
	m_pDM = FindDM(_T("DM_�ڻ�����ڷ�"));

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

	//OnSelchangecmbSTATBASICCLASS();	


	
	//===================================================
	//2010.05.02 ADD BY PJW : ���� �����ڿ� ��쿡�� ���������� ���ð���
	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		GetDlgItem(IDC_stcManageCode)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmbManageCode)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_stcManageCode)->ShowWindow(SW_SHOW);
 		GetDlgItem(IDC_cmbManageCode)->ShowWindow(SW_SHOW);
	}
	//===================================================


	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//���۵�Ϲ�ȣ|E|11023|||||||
	//����Ϲ�ȣ|E|11024|||||||
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_������Ǽ���"), _T("���۵�Ϲ�ȣ"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_������Ǽ���"), _T("����Ϲ�ȣ"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  
}

BOOL CStatisticsConditionDlg::Init()
{
	INT nUseCodeCnt = 0;
	CString strAcqCode, strTmp ;



	// ������ ���Ǽ�����.
	//m_arrHorizontalData.Add(_T("��ü�ڷ�"));
	//m_arrHorizontalData.Add(_T("KDC��������"));
	//m_arrHorizontalData.Add(_T("DDC��������"));
	m_arrHorizontalData.Add(_T("���Ա���"));

	
	//m_arrHorizontalDesc.Add(_T("��ü�ڷ�"));
	//m_arrHorizontalDesc.Add(_T("KDC��������ȣ����"));
	//m_arrHorizontalDesc.Add(_T("DDC��������ȣ����"));
	m_arrHorizontalDesc.Add(_T("���Ա���"));


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

	if( m_strUseCodeYN == _T("Y") )
	{
		CString strYN, strName;
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 91"), strYN);
		if( strYN == _T("Y") ) 
		{
			m_pDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 91"), strName);
			strTmp.Format(_T("%s�����"), strName);
			m_arrVerticalData.Add(strTmp);
			m_arrVerticalDesc.Add(_T("�����з��ڵ�1"));
			m_strUserCodeAlias.Add(_T("�����з��ڵ�1"));
		}
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 92"), strYN);
		if( strYN == _T("Y") ) 
		{
			m_pDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS from MN_USER_DEFINE_TBL WHERE CLASS_CODE = 92"), strName);
			strTmp.Format(_T("%s�����"), strName);
			m_arrVerticalData.Add(strTmp);
			m_arrVerticalDesc.Add(_T("�����з��ڵ�2"));
			m_strUserCodeAlias.Add(_T("�����з��ڵ�2"));
		}
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 93"), strYN);
		if( strYN == _T("Y") ) 
		{
			m_pDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 93"), strName);
			strTmp.Format(_T("%s�����"), strName);
			m_arrVerticalData.Add(strTmp);
			m_arrVerticalDesc.Add(_T("�����з��ڵ�3"));
			m_strUserCodeAlias.Add(_T("�����з��ڵ�3"));
		}
	}
	// END:

	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("KDC��������"), &m_arrHorizontalData, _T("�ڷ����"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &m_arrVerticalData, _T("��蹰���"));
	
	


	
	

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
	
	m_nAcqType = ACQ_TYPE_INVALID;

	// �׷�����Ŭ������ �ʱ�ȭ�Ѵ�.
	m_pGroupCollection->RemoveAll();
	m_pGroupCollection->SetAcqType(m_nAcqType);
	
	// ���Ա��� ���¿� ���� �ڷᱸ�� �� �ڷᱸ���� �޺��ڽ��� �����Ѵ�.
	m_arrDataClass.RemoveAll();
	m_arrDateClass.RemoveAll();

	
	m_arrDataClass.Add(_T("�Ⱓ����豸��"));
	m_arrDateClass.Add(_T("�谡��"));
	m_arrDateClass.Add(_T("�Է���"));
	m_arrDateClass.Add(_T("�����"));
	
	
	/*
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
	*/
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrDataClass.GetAt(0), &m_arrDataClass, _T("�ڷᱸ��"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrDateClass.GetAt(0), &m_arrDateClass, _T("�ڷᱸ����"));

	


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


	// ���Ա��� �޺��� ������ ���� ���ҽ����� ǥ�ø� �����Ѵ�.

	nCurSel = ( (CComboBox*) GetDlgItem(IDC_cmbSTAT_ACQ_CODE) )->GetCurSel();

	//nCurSel = pCMB->GetCurSel();
	switch( nCurSel )
	{
	case 0:
		{
			for( i=0 ; i<nCount1 ; i++ )
			{
				GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_SHOW);
			}
			for( i=0 ; i<nCount2 ; i++ )	
			{
				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
			}
		}
	case 1:
		{
			for( i=0 ; i<nCount1 ; i++ )	
			{
				GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_SHOW);
			}
			for( i=0 ; i<nCount2 ; i++ )	
			{
				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
			}
		}
		break;

	case 2:
		{
			for( i=0 ; i<nCount1 ; i++ )	
			{
				//===================================================
				//2009.03.28 UPDATE BY PJW : �����̿��� ���������� ��쵵 ���԰� ��Ʈ���� �����.
// 				GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_HIDE);
				GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_SHOW);
				//===================================================
				
			}
			for( i=0 ; i<nCount2 ; i++ )	
			{
				//===================================================
				//2009.03.28 UPDATE BY PJW : �����̿��� ���������� ��쵵 ���԰� ��Ʈ���� �����.
// 				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_SHOW);
				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
				//===================================================
				
			}
		}
		break;

	default:
		{
			for( i=0 ; i<nCount1 ; i++ )	
			{
				//===================================================
				//2009.03.28 UPDATE BY PJW : �����̿��� ���������� ��쵵 ���԰� ��Ʈ���� �����.
// 				GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_HIDE);
				GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_SHOW);
				//===================================================
				
			}
			for( i=0 ; i<nCount2 ; i++ )	
			{
				//===================================================
				//2009.03.28 UPDATE BY PJW : �����̿��� ���������� ��쵵 ���԰� ��Ʈ���� �����.
// 				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
				//===================================================
				
			}
		}
		break;
	}

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
	/*
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
	//OnOK();
	*/
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
	if	( strVerticalDesc == _T("��������") )		strFieldName = _T("I.WORK_CODE");
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
	else strFieldName.Empty();

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


	//===================================================
	//2009.04.09 ADD BY PJW : ��Ϲ�ȣ�� ���ǿ� �߰���.
	CString strStartRegNo;
	CString strEndRegNo;
	m_pCM->GetControlMgrData(_T("���۵�Ϲ�ȣ"), strStartRegNo);
	m_pCM->GetControlMgrData(_T("����Ϲ�ȣ"), strEndRegNo);

	m_pStatCondition->SetStartRegNo(strStartRegNo);
	m_pStatCondition->SetEndRegNo(strEndRegNo);
	//===================================================

	//=======================================================================
	// 5. ���Ǽ���Ŭ������ ������ ������ �������� �����Ѵ�.
	//=======================================================================
	//===================================================
	//2010.05.02 ADD BY PJW : ���մ������ϰ�� ������ ���������� ������ ������
	if(NULL != m_pInfo && _T("S") == m_pInfo->USER_PRIVILEGE)
	{
		CString strManageCode = _T("");
		CESL_ControlMultiComboBox* pcmbManageCode = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_cmbManageCode);
		INT nCur = pcmbManageCode->GetCurSel();
		pcmbManageCode->GetData(strManageCode, nCur, 0);
		strManageCode.TrimLeft(); strManageCode.TrimRight();
		m_pStatCondition->SetManageCode(strManageCode);

	}
	//====================================================

	//====================================================
	// 2012.10.18 ADD BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (�谡���¿� ���� �ڷ�� ������ �߰�)
	CString strReference = _T("");
	if( ((CButton*)GetDlgItem(IDC_ReferenceCheck))->GetCheck() == BST_CHECKED )
	{
		if(IDCANCEL==AfxMessageBox(_T("�谡���¸� �������� ������ ��� �ڷῡ ���� ��谡 ����˴ϴ�! ����Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		{
			return;
		}
		strReference = _T("��ü");
		strReference.TrimLeft(); strReference.TrimRight();
		m_pStatCondition->SetReference(strReference);
		m_pStatCondition->AddDateInfo(_T(""), _T(""));
	}
	else
	{		
		
		CESL_ControlMultiComboBox* pcmbBTS_Reference = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_cmbBTS_Reference);
		INT nCur2 = pcmbBTS_Reference->GetCurSel();
		pcmbBTS_Reference->GetData(strReference, nCur2, 0);
		strReference.TrimLeft(); strReference.TrimRight();
		m_pStatCondition->SetReference(strReference);
		
	}
	
	//===================================================

	

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

	if( ((CButton*)GetDlgItem(IDC_ReferenceCheck))->GetCheck() == BST_UNCHECKED )
	{
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
		else if	( strDateClass == _T("�谡��") )
		{
			m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("B.SHELF_DATE"));
		}
		else	
		{
			ESLAfxMessageBox(_T("�ڷᱸ������ �����Ͽ� �ֽʽÿ�."));
			return -1;
		}
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
	/*
	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�ڷ����"));

	INT nCurSel = pCMB->GetCurSel();
	m_pStatCondition->SetClassNoType(m_arrHorizontalDesc.GetAt(nCurSel));

	::SendMessage(pParentMgr->GetSafeHwnd(), CHANGE_CLASS_NO_TYPE, 0, 0);
	*/
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

void CStatisticsConditionDlg::OnKillfocusedtENDREGNO() 
{	
	CString strRegNo;
	INT nIds;

	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_edtEND_REG_NO);

	nIds = 0;
	strRegNo = _T("");

	m_pCM->GetControlMgrData( _T("����Ϲ�ȣ"), strRegNo);	

	nIds = MakeRegNo( strRegNo );

	if( nIds == -1001 )
	{
		AfxMessageBox(_T("��Ϲ�ȣ �ڸ����� ���� �ʽ��ϴ�."));
		m_pCM->SetControlMgrData(_T("����Ϲ�ȣ"), _T(""));	
		pEdt->SetFocus();
		return;
	}
	else if( nIds == -1002 )
	{
		AfxMessageBox(_T("��Ϲ�ȣ 12�ڸ� ��ȯ�� ���� �߽��ϴ�."));
		m_pCM->SetControlMgrData(_T("����Ϲ�ȣ"), _T(""));	
		pEdt->SetFocus();
		return;
	}

	m_pCM->SetControlMgrData(_T("����Ϲ�ȣ"), strRegNo);	
}	

void CStatisticsConditionDlg::OnKillfocusedtSTARTREGNO() 
{
	CString strRegNo;
	INT nIds;

	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_edtSTART_REG_NO);

	nIds = 0;
	strRegNo = _T("");

	m_pCM->GetControlMgrData( _T("���۵�Ϲ�ȣ"), strRegNo);	

	nIds = MakeRegNo( strRegNo );

	if( nIds == -1001 )
	{
		AfxMessageBox(_T("��Ϲ�ȣ �ڸ����� ���� �ʽ��ϴ�."));
		m_pCM->SetControlMgrData(_T("���۵�Ϲ�ȣ"), _T(""));	
		pEdt->SetFocus();
		return;
	}
	else if( nIds == -1002 )
	{
		AfxMessageBox(_T("��Ϲ�ȣ �ڸ��� ��ȯ�� ���� �߽��ϴ�."));
		m_pCM->SetControlMgrData(_T("���۵�Ϲ�ȣ"), _T(""));	
		pEdt->SetFocus();
		return;
	}

	m_pCM->SetControlMgrData(_T("���۵�Ϲ�ȣ"), strRegNo);	
}

//===================================================
//2009.04.09 ADD BY PJW : ��Ϲ�ȣ�� 12�ڸ��� �����Ѵ�.
INT CStatisticsConditionDlg::MakeRegNo(CString &strRegNo)
{
	INT nLength;
	INT nRegCodeLength;
	CString strRegCode;
	CString strRegFigure;
	CString strTempRegNo;

	strRegNo.Replace(_T(" "),_T(""));
	strRegNo.MakeUpper();
	if(!strRegNo.IsEmpty())
	{
		nLength = strRegNo.GetLength();
		nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
		if( nRegCodeLength < nLength  && 12 >= nLength )
		{
			
			strRegCode = strRegNo.Left(nRegCodeLength);
			strRegFigure = strRegNo.Mid(nRegCodeLength);
			strTempRegNo = m_pInfo->MakeRegNo( strRegCode, strRegFigure );
			if(strTempRegNo == _T("error"))
			{
				return -1002;
			}

			strRegNo = strTempRegNo;
		}
		else
		{
			return -1001;
		}
	}	

	return 0;
}
//===================================================