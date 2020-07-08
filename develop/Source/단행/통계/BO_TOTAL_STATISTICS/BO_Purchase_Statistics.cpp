// BO_Purchase_Statistics.cpp : implementation file
//

#include "stdafx.h"
#include "BO_Purchase_Statistics.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/DivisionNo_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_Purchase_Statistics dialog

CBO_Purchase_Statistics::CBO_Purchase_Statistics(CESL_Mgr* pParent /*=NULL*/)
	: CBasicStatistics(CBO_Purchase_Statistics::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_Purchase_Statistics)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrDataClass.RemoveAll();
	m_arrDateClass.RemoveAll();
	m_pComboMgr = NULL;
	m_arrHorizontalData.RemoveAll();
	m_arrHorizontalDesc.RemoveAll();
	m_arrVerticalData.RemoveAll();
	m_arrVerticalDesc.RemoveAll();
	m_pParentGroupCollection = NULL;
	m_pThread = NULL;
	m_nOpenType = 0;
	m_pPrintMgr = NULL;
	m_nSpeciesType = SPECIES_TYPE_INVALID;
}

CBO_Purchase_Statistics::~CBO_Purchase_Statistics()
{
	if(m_pStatCondition)
	{
		delete m_pStatCondition;
		m_pStatCondition = NULL;
	}
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
	ThreadTerminate();
}

VOID CBO_Purchase_Statistics::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_Purchase_Statistics)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_Purchase_Statistics, CDialog)
	//{{AFX_MSG_MAP(CBO_Purchase_Statistics)
	ON_BN_CLICKED(IDC_btnPUR_STAT_VIEW_DIVNO, OnbtnPURSTATVIEWDIVNO)
	ON_EN_KILLFOCUS(IDC_edtPURSTAT_DIVNO, OnKillfocusedtPURSTATDIVNO)
	ON_CBN_SELCHANGE(IDC_cmbPURSTAT_WORK_CODE, OnSelchangecmbPURSTATWORKCODE)
	ON_EN_KILLFOCUS(IDC_edtPURSTAT_ACQ_YEAR, OnKillfocusedtPURSTATACQYEAR)
	ON_CBN_SELCHANGE(IDC_cmbPURSTAT_BASIC_CLASS, OnSelchangecmbPURSTATBASICCLASS)
	ON_BN_CLICKED(IDC_btnPURSTAT_CONTRUCT, OnbtnPURSTATCONTRUCT)
	ON_MESSAGE(JOB_CANCEL_MSG, OnJobCancel)
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_BN_CLICKED(IDC_btnPURSTAT_PRINT, OnbtnPURSTATPRINT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_Purchase_Statistics message handlers
// �ʱ�ȭ
BOOL CBO_Purchase_Statistics::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("K2UPGRADE_�����������")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_�����������"));
	m_pDM = FindDM(_T("DM_����ڷ�"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));
	if( m_pCM == NULL || m_pGrid == NULL )
	{
		ESLAfxMessageBox(_T("[CM_�����������]�� ������ �� �����ϴ�."));
		return FALSE;
	}	
	if( m_pDM == NULL )
	{
		ESLAfxMessageBox(_T("[DM_����ڷ�]�� ������ �� �����ϴ�."));
		return FALSE;
	}
	m_pGrid->SetSort(FALSE);
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("�˼����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	return TRUE;
}

VOID CBO_Purchase_Statistics::ThreadTerminate()
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

BOOL CBO_Purchase_Statistics::Init()
{
	CString strGroupInfo;

		
	m_arrHorizontalData.Add(_T("��ü�ڷ�")			);
	m_arrHorizontalData.Add(_T("KDC��������")		);
	m_arrHorizontalData.Add(_T("DDC��������")		);
	m_arrHorizontalDesc.Add(_T("��ü�ڷ�")			);
	m_arrHorizontalDesc.Add(_T("KDC��������ȣ����")	);
	m_arrHorizontalDesc.Add(_T("DDC��������ȣ����")	);

	m_arrVerticalData.Add(_T("�ڷᱸ�к����")		);
	m_arrVerticalData.Add(_T("��ϱ��к����")		);
	m_arrVerticalData.Add(_T("��ġ���к����")		);
	m_arrVerticalDesc.Add(_T("�ڷᱸ��")			);
	m_arrVerticalDesc.Add(_T("��ϱ���")			);
	m_arrVerticalDesc.Add(_T("��ġ��ȣ")			);
		
	if( m_pParentGroupCollection == NULL )		return FALSE;

	m_pGroupCollection = new CGroupCollection(this);
	m_pGroupCollection->SetAcqType(PURCHASE);
	m_pParentGroupCollection->CopyData(m_pGroupCollection);
	
	m_pGroupCollection->GetGroupInfo(strGroupInfo);
	m_pCM->SetControlMgrData(_T("������ȣ"), strGroupInfo						);
	m_pCM->SetControlMgrData(_T("��������"), m_pGroupCollection->GetWorkCode()	);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pGroupCollection->GetAcqYear()	);
	
	m_pStatCondition = new CStatisticsCondition(this);
	m_pStatCondition->SetAcqType(PURCHASE);
	m_pStatCondition->SetGroupCollection(m_pGroupCollection);
	if( m_nOpenType > 0 )	m_pStatCondition->SetStatType(STATISTICS_BY_DIVNO_DELIVERY);// �˼����
	else					m_pStatCondition->SetStatType(STATISTICS_BY_DIVNO_MISSING);	// �̳����
	
	m_pComboMgr = new CMulitComboBoxManager(this);
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("KDC��������"), &m_arrHorizontalData, _T("�ڷ����"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &m_arrVerticalData, _T("��蹰���"));

	m_pPrintMgr = new CBasicPrintMgr(this);
	
	
	return (SetCtrlByOpenType());
}

BOOL CBO_Purchase_Statistics::SetCtrlByOpenType()
{
	if		( m_nOpenType > 0 )		SetWindowText(_T("�˼��ڷ����")	);
	else if	( m_nOpenType < 0 )		SetWindowText(_T("�̰˼��ڷ����")	);
	else							return FALSE ;

	return TRUE;
}

VOID CBO_Purchase_Statistics::OnbtnPURSTATVIEWDIVNO() 
{
	CString strAcqYear, strWorkCode, strSelDivNo;

	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear	);
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);

	if( strAcqYear.IsEmpty() == TRUE )
	{
		ESLAfxMessageBox(_T("���Գ⵵ ������ �Է��� �ֽʽÿ�"));
		return;
	}

	CDivisionNo_List_Dlg dlg(this);
	dlg.SetWorkStatus(m_strDivNoWorkStatus);
	dlg.SetAcquisitYear(strAcqYear);
	dlg.SetWorkCode(strWorkCode);
	dlg.SetSpeciesType(m_nSpeciesType);
	dlg.SetGroupCollection(m_pGroupCollection);
	
	if( IDOK == dlg.DoModal() )
	{
		strSelDivNo = dlg.GetSelDivNo();
		m_pCM->SetControlMgrData(_T("������ȣ"), strSelDivNo						);
		m_pCM->SetControlMgrData(_T("��������"), m_pGroupCollection->GetWorkCode()	);
		m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pGroupCollection->GetAcqYear()	);
	}
}	


VOID CBO_Purchase_Statistics::OnKillfocusedtPURSTATDIVNO() 
{
	CString strAcqYear, strGroupInfo, strWorkCode;

	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("������ȣ"), strGroupInfo);
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);
	
	strGroupInfo = CheckValidateGroupNo(strGroupInfo, strAcqYear, strWorkCode);
	
	m_pCM->SetControlMgrData(_T("������ȣ"), strGroupInfo);	
}


VOID CBO_Purchase_Statistics::OnSelchangecmbPURSTATWORKCODE() 
{
	OnKillfocusedtPURSTATDIVNO();
}

VOID CBO_Purchase_Statistics::OnKillfocusedtPURSTATACQYEAR() 
{
	OnKillfocusedtPURSTATDIVNO();
}

BOOL CBO_Purchase_Statistics::PreTranslateMessage(MSG* pMsg) 
{
 	if( pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam )
	{
		GetDlgItem(IDC_btnPURSTAT_CONTRUCT)->SetFocus();
		PostMessage(WM_COMMAND, IDC_btnPURSTAT_CONTRUCT, BN_CLICKED);
		return TRUE;
	}	

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_Purchase_Statistics::OnSelchangecmbPURSTATBASICCLASS() 
{
	INT nCurSel ;
	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�ڷ����"));
	
	nCurSel = pCMB->GetCurSel();
	m_pStatCondition->SetClassNoType(m_arrHorizontalDesc.GetAt(nCurSel));
	
	ChangeColumnHeaderOfSubjectCode();
}

VOID CBO_Purchase_Statistics::ChangeColumnHeaderOfSubjectCode()
{
	CString tmpstr, codeClassName;
	codeClassName = m_pStatCondition->GetClassNoType();
	for(INT i=0 ; i<10 ; i++)
	{
		tmpstr.Format(_T("%d"), i);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(codeClassName, tmpstr, tmpstr);
		if(tmpstr.IsEmpty()) tmpstr.Format(_T("%d"), i);
		m_pGrid->SetTextMatrix(0, i+3, tmpstr);
	}
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��豸�� ��ư
/////////////////////////////////////////////////////////////////////////////
VOID CBO_Purchase_Statistics::OnbtnPURSTATCONTRUCT() 
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT nCurSel;
	CString strAcqYear, strGroupInfo, strWorkCode, strFieldName, strVerticalDesc;
	
	m_nStartRow = 0;
	
	//=======================================================================
	// 1. ���Գ⵵, ������ȣ, �������п� �ش��ϴ� ���������� �����´�.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("������ȣ"), strGroupInfo);
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);
	strGroupInfo = CheckValidateGroupNo(strGroupInfo, strAcqYear, strWorkCode);
	if( strGroupInfo.IsEmpty() == TRUE )		return ;
	
	//=======================================================================
	// 2. �۾�����, ���Գ⵵, ����/������ȣ, �������������� �̿��Ͽ� 
	//	 DM�� �����Ͽ� GroupŬ������ �����Ѵ�.
	//=======================================================================
	m_pGroupCollection->MakeGroupInfo(strAcqYear, strGroupInfo, strWorkCode);

	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�ڷ����"));
	CESL_ControlComboBox* pCMB1 = (CESL_ControlComboBox*)m_pCM->FindControl(_T("��蹰���"));
	nCurSel = pCMB->GetCurSel();
	m_pStatCondition->SetClassNoType(m_arrHorizontalDesc.GetAt(nCurSel));
	nCurSel = pCMB1->GetCurSel();
	
	strVerticalDesc = m_arrVerticalDesc.GetAt(nCurSel);
	if( strVerticalDesc == _T("��ϱ���") )		strFieldName = _T("B.REG_CODE");
	if( strVerticalDesc == _T("��ġ��ȣ") )		strFieldName = _T("B.SEPARATE_SHELF_CODE");
	if( strVerticalDesc == _T("�ڷᱸ��") )		strFieldName = _T("I.MAT_CODE");
	m_pStatCondition->SetVerticalInfo(strVerticalDesc, strFieldName);
	m_pStatCondition->SetWhereStmt();
	
	MakeStatistics();	
}

INT CBO_Purchase_Statistics::MakeStatistics()
{
	m_pDM->FreeData();
	ShowDataProcessingDlg();
	m_bRunning = TRUE;
	m_pThread = AfxBeginThread(GetStatistics, this);
	return 0;
}

UINT CBO_Purchase_Statistics::GetStatistics(LPVOID pParam )
{
    CBO_Purchase_Statistics* pObject = (CBO_Purchase_Statistics*)pParam;
	pObject->GetAcqStatistics();
	return 0;   // thread completed successfully
}

VOID CBO_Purchase_Statistics::OnJobCancel(WPARAM w,LPARAM l)
{
	ThreadTerminate();
}

VOID CBO_Purchase_Statistics::OnQueryCreate(WPARAM w,LPARAM l)
{
	m_pGrid->Display();
}

VOID CBO_Purchase_Statistics::OnbtnPURSTATPRINT() 
{
	CString strClassNoType = m_pStatCondition->GetClassNoType();

	if( strClassNoType != _T("KDC") && strClassNoType != _T("DDC")) strClassNoType = _T("��ü�ڷ�");

	m_pPrintMgr->InitPrintData();
	m_pPrintMgr->AddPrintData(_T("����"), m_pStatCondition->GetStatType());
	m_pPrintMgr->AddPrintData(_T("�ڷᱸ��"), m_pStatCondition->GetVerticalDesc());
	m_pPrintMgr->AddPrintData(_T("����"), m_pStatCondition->GetCondition());
	m_pPrintMgr->AddPrintData(_T("�з�ǥ����"), strClassNoType);
	m_pPrintMgr->SetPrintDM(m_pDM);
	
	if		( strClassNoType.Left(3) == _T("KDC") )
	{
		m_pPrintMgr->SetSIReportFilename(_T("K2UP_����ڷ�_KDC"));
	}
	else if ( strClassNoType.Left(3) == _T("DDC") )
	{
		m_pPrintMgr->SetSIReportFilename(_T("K2UP_����ڷ�_DDC"));
	}
	else if ( strClassNoType.Left(2) == _T("LC") )
	{
		m_pPrintMgr->SetSIReportFilename(_T("K2UP_����ڷ�_LC"));
	}
	else	
	{
	m_pPrintMgr->SetSIReportFilename(_T("K2UP_����ڷ�"));
	}
	
	m_pPrintMgr->Print();	
}
