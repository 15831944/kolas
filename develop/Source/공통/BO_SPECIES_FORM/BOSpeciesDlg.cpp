// BOSpeciesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOSpeciesDlg.h"

#include "BOVolumeDlg.h"
#include "BOAppendixDlg.h"

#include "BOMarcEditDlg.h"
#include "BOISBNAdditionDlg.h"
#include "..\BO_MARC_IMPORT\MARC_IMPORT.h"
#include "..\Duplicate_Data_Search\DuplicateSearch_Interface.h"

#include "..\..\�������\Light_GatherDataSearch\UC_MARC_IMPORT.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOSpeciesDlg dialog


CBOSpeciesDlg::CBOSpeciesDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOSpeciesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nOpenMode = -1;
	m_bShowCode = TRUE;

	// DM �ʱ�ȭ
	m_pDM_Species = NULL;
	m_pDM_Book = NULL;
	m_pDM_Purchase = NULL;
	m_pDM_Donation = NULL;
	m_pDM_Index = NULL;

	// CM �ʱ�ȭ
	m_pCM_Index = NULL;
	m_pCM_Purchase = NULL;

	// Form Api
	m_pFormApi = new CBOFormApi(this);

	// Species API
	m_pBOSpeciesApi = NULL;	

	m_pDM_Parent = NULL;
	m_pGrid_Parent = NULL;
	m_nParentSelectedRow = -1;

	m_strRegCodeField.Empty();

	m_bDupAddMode = FALSE;

	m_strDefaultValueFilePath = _T("..\\cfg\\species_form.cfg");

	m_pNoteCtrl = NULL;

	m_bControlDisplay = FALSE;

	m_bAutoDupSearch = FALSE;

	m_bViewVolInfo = FALSE;
}

BOOL CBOSpeciesDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBOSpeciesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOSpeciesDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOSpeciesDlg, CDialog)
	//{{AFX_MSG_MAP(CBOSpeciesDlg)
	ON_BN_CLICKED(IDC_bSHOW_HIDE_CODE, OnbSHOWHIDECODE)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_CBN_SELCHANGE(IDC_cCLASIFY_CLASS, OnSelchangecCLASIFYCLASS)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bDEFAULT_CONFIG, OnbDEFAULTCONFIG)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bSAVEAS, OnbSAVEAS)
	ON_BN_CLICKED(IDC_bMARC_EDIT, OnbMARCEDIT)
	ON_BN_CLICKED(IDC_bMARC_IMPORT, OnbMARCIMPORT)
	ON_BN_CLICKED(IDC_bKOLIS_NET, OnbKOLISNET)
	ON_BN_CLICKED(IDC_bCLEARSCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_bDUP_SEARCH, OnbDUPSEARCH)
	ON_BN_CLICKED(IDC_bEXTRA_ISBN, OnbEXTRAISBN)
	ON_BN_CLICKED(IDC_bISBN_CHECK, OnbISBNCHECK)
	ON_EN_KILLFOCUS(IDC_eREG_CLASS_CODE, OnKillfocuseREGCLASSCODE)
	ON_BN_CLICKED(IDC_bVOL_INFO, OnbVOLINFO)
	ON_CBN_SELCHANGE(IDC_cPRICE_TYPE, OnSelchangecPRICETYPE)
	ON_EN_KILLFOCUS(IDC_ePRICE_DC, OnKillfocusePRICEDC)
	ON_EN_KILLFOCUS(IDC_ePRICE, OnKillfocusePRICE)
	ON_BN_CLICKED(IDC_bAPPLY_SERIES, OnbAPPLYSERIES)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_eTITLE, OnKillfocuseTITLE)
	ON_CBN_SELCHANGE(IDC_cFORM_CLASS, OnSelchangecFORMCLASS)
	ON_BN_CLICKED(IDC_bPREV_SPECIES, OnbPREVSPECIES)
	ON_BN_CLICKED(IDC_bNEXT_SPECIES, OnbNEXTSPECIES)
	ON_BN_CLICKED(IDC_btnSEARCH_DONATOR, OnbtnSEARCHDONATOR)
	ON_BN_CLICKED(IDC_btnGROUP_INSERT, OnbtnGROUPINSERT)
	ON_EN_KILLFOCUS(IDC_edtACQ_YEAR, OnKillfocusedtACQYEAR)
	//}}AFX_MSG_MAP
	ON_MESSAGE(NOTE_CLICKED, OnNoteClicked)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBOSpeciesDlg Operations
VOID CBOSpeciesDlg::ShowACQInfoCtrlByOpenMode()
{
EFS_BEGIN
	INT nArrCodeResourceID[256];
	INT nCount = 0;
	if(m_nOpenMode == 1200 || m_nOpenMode == -1200)//���������ڷ� �Է�/������ ���
	{
		nArrCodeResourceID[nCount++] = IDC_tPRICE_TYPE;
		nArrCodeResourceID[nCount++] = IDC_cPRICE_TYPE;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_DC;
		nArrCodeResourceID[nCount++] = IDC_ePRICE_DC;
		nArrCodeResourceID[nCount++] = IDC_tPRICE;
		nArrCodeResourceID[nCount++] = IDC_ePRICE;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_REAL;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_JOB;
		nArrCodeResourceID[nCount++] = IDC_ePRICE_OTHER_INFO;
		nArrCodeResourceID[nCount++] = IDC_tPURCHASE;
		nArrCodeResourceID[nCount++] = IDC_cPROVIDER;
		nArrCodeResourceID[nCount++] = IDC_tCURRENCY_CLASS;
		nArrCodeResourceID[nCount++] = IDC_cCURRENCY_CLASS;
		nArrCodeResourceID[nCount++] = IDC_stcNONDONATE_REASON;
		nArrCodeResourceID[nCount++] = IDC_edtNONDONATE_REASON;
	}
	else if(m_nOpenMode == 1100 || m_nOpenMode == -1100)//���Դ���ڷ� �Է�/������ ���
	{
		nArrCodeResourceID[nCount++] = IDC_stcDONATE_PRICE;
		nArrCodeResourceID[nCount++] = IDC_edtDONATE_PRICE;
		nArrCodeResourceID[nCount++] = IDC_stcDONATE_CURRENCY_CODE;
		nArrCodeResourceID[nCount++] = IDC_cmbDONATE_CURRENCY_CODE;
		nArrCodeResourceID[nCount++] = IDC_stcDONATOR;
		nArrCodeResourceID[nCount++] = IDC_edtDONATOR;
		nArrCodeResourceID[nCount++] = IDC_btnSEARCH_DONATOR;
		nArrCodeResourceID[nCount++] = IDC_stcNONDONATE_REASON;
		nArrCodeResourceID[nCount++] = IDC_edtNONDONATE_REASON;
	}
	else if(m_nOpenMode == 1000 || m_nOpenMode == -1000) //�����ڷ� �Է�/������ ���
	{
		nArrCodeResourceID[nCount++] = IDC_tPRICE_TYPE;
		nArrCodeResourceID[nCount++] = IDC_cPRICE_TYPE;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_DC;
		nArrCodeResourceID[nCount++] = IDC_ePRICE_DC;
		nArrCodeResourceID[nCount++] = IDC_tPRICE;
		nArrCodeResourceID[nCount++] = IDC_ePRICE;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_REAL;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_JOB;
		nArrCodeResourceID[nCount++] = IDC_ePRICE_OTHER_INFO;
		nArrCodeResourceID[nCount++] = IDC_tPURCHASE;
		nArrCodeResourceID[nCount++] = IDC_cPROVIDER;
		nArrCodeResourceID[nCount++] = IDC_tCURRENCY_CLASS;
		nArrCodeResourceID[nCount++] = IDC_cCURRENCY_CLASS;
		nArrCodeResourceID[nCount++] = IDC_stcDONATOR;
		nArrCodeResourceID[nCount++] = IDC_edtDONATOR;
		nArrCodeResourceID[nCount++] = IDC_btnSEARCH_DONATOR;
		nArrCodeResourceID[nCount++] = IDC_stcNONDONATE_REASON;
		nArrCodeResourceID[nCount++] = IDC_edtNONDONATE_REASON;
	}
	else if(m_nOpenMode == 1300 || m_nOpenMode == -1300) //�����δ��ڷ� �Է�/�����ϰ��
	{
		nArrCodeResourceID[nCount++] = IDC_tPRICE_TYPE;
		nArrCodeResourceID[nCount++] = IDC_cPRICE_TYPE;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_DC;
		nArrCodeResourceID[nCount++] = IDC_ePRICE_DC;
		nArrCodeResourceID[nCount++] = IDC_tPRICE;
		nArrCodeResourceID[nCount++] = IDC_ePRICE;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_REAL;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_JOB;
		nArrCodeResourceID[nCount++] = IDC_ePRICE_OTHER_INFO;
		nArrCodeResourceID[nCount++] = IDC_tPURCHASE;
		nArrCodeResourceID[nCount++] = IDC_cPROVIDER;
		nArrCodeResourceID[nCount++] = IDC_tCURRENCY_CLASS;
		nArrCodeResourceID[nCount++] = IDC_cCURRENCY_CLASS;
	}
	CWnd * pWnd = NULL;
	for(INT i=0 ; i<nCount ; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if(pWnd->GetSafeHwnd() != NULL)
			pWnd->ShowWindow(SW_HIDE);
	}

EFS_END
}

VOID CBOSpeciesDlg::ShowCodeResource()
{
	INT i = 0;
	INT nCodeResourcesHeight = 122;
	CWnd *pWnd = NULL;

	if( m_bShowCode )
	{
		GetDlgItem(IDC_bSHOW_HIDE_CODE)->SetWindowText(_T("�����˻�"));
	}
	else
	{
		GetDlgItem(IDC_bSHOW_HIDE_CODE)->SetWindowText(_T("�󼼰˻�"));
	}


	CRect rcWindow;
	GetWindowRect(rcWindow);
	if (m_bShowCode &&rcWindow == m_rcWindow) return;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	nArrCodeResourceID[nCount++] = IDC_STATIC_CODE;
	nArrCodeResourceID[nCount++] = IDC_tSUB_REG_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cSUB_REG_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tMEDIA_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tMEDIA_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cMEDIA_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cMEDIA_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cMEDIA_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tUSE_OBJECT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cUSE_OBJECT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tUSE_LIMIT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cUSE_LIMIT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tSHELF_CODE;
	nArrCodeResourceID[nCount++] = IDC_cCLASIFY_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tCONTROL_MAT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cCONTROL_MAT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tMANAGE_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cMANAGE_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tCLASIFY_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cSHELF_CODE;
	nArrCodeResourceID[nCount++] = IDC_tWORK_CODE;
	nArrCodeResourceID[nCount++] = IDC_cWORK_CODE;
	nArrCodeResourceID[nCount++] = IDC_tDISCRIPTION;
	nArrCodeResourceID[nCount++] = IDC_eDISCRIPTION;
	nArrCodeResourceID[nCount++] = IDC_tMAT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cMAT_CLASS;

	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (m_bShowCode)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	// Move Other Resources
	nCount = 0;
	INT nArrOtherResources[256];
	nArrOtherResources[nCount++] = IDC_STATIC_MARC;
	nArrOtherResources[nCount++] = IDC_tTITLE;
	nArrOtherResources[nCount++] = IDC_eTITLE;
	nArrOtherResources[nCount++] = IDC_bDUP_SEARCH;
	nArrOtherResources[nCount++] = IDC_tAUTHOR1;
	nArrOtherResources[nCount++] = IDC_eAUTHOR1;
	nArrOtherResources[nCount++] = IDC_tEDITION_STMT;
	nArrOtherResources[nCount++] = IDC_eEDITION_STMT;
	nArrOtherResources[nCount++] = IDC_tVOL;
	nArrOtherResources[nCount++] = IDC_eVOL;
	nArrOtherResources[nCount++] = IDC_tSERIES_TITLE;
	nArrOtherResources[nCount++] = IDC_eSERIES_TITLE;
	nArrOtherResources[nCount++] = IDC_ePUBLISH_PLACE;
	nArrOtherResources[nCount++] = IDC_tPUBLISH_PLACE;
	nArrOtherResources[nCount++] = IDC_ePUBLISHER;
	nArrOtherResources[nCount++] = IDC_tPUBLISHER;
	nArrOtherResources[nCount++] = IDC_ePUBLISH_YEAR;
	nArrOtherResources[nCount++] = IDC_tPUBLISH_YEAR;
	nArrOtherResources[nCount++] = IDC_eEA_ISBN;
	nArrOtherResources[nCount++] = IDC_eEA_ISBN_ADD;
	nArrOtherResources[nCount++] = IDC_eEA_ISBN_MISTAKE;
	nArrOtherResources[nCount++] = IDC_eSET_ISBN;
	nArrOtherResources[nCount++] = IDC_eSET_ISBN_ADD;
	nArrOtherResources[nCount++] = IDC_eSET_ISBN_MISTAKE;
	nArrOtherResources[nCount++] = IDC_tEA_ISBN;
	nArrOtherResources[nCount++] = IDC_bEXTRA_ISBN;
	nArrOtherResources[nCount++] = IDC_tSET_ISBN;
	nArrOtherResources[nCount++] = IDC_bISBN_CHECK;
	nArrOtherResources[nCount++] = IDC_STATIC_PURCHASE_GROUP;
	nArrOtherResources[nCount++] = IDC_tPRICE;
	nArrOtherResources[nCount++] = IDC_ePRICE;
	nArrOtherResources[nCount++] = IDC_tPRICE_DC;
	nArrOtherResources[nCount++] = IDC_ePRICE_DC;
	nArrOtherResources[nCount++] = IDC_tPURCHASE;
	nArrOtherResources[nCount++] = IDC_cPROVIDER;
	nArrOtherResources[nCount++] = IDC_bDEFAULT_CONFIG;
	nArrOtherResources[nCount++] = IDC_tPRICE_REAL;
	nArrOtherResources[nCount++] = IDC_eOrderPRICE;
	nArrOtherResources[nCount++] = IDC_cPRICE_TYPE;
	nArrOtherResources[nCount++] = IDC_tPRICE_JOB;
	nArrOtherResources[nCount++] = IDC_ePRICE_OTHER_INFO;
	nArrOtherResources[nCount++] = IDC_bMARC_EDIT;
	nArrOtherResources[nCount++] = IDC_bMARC_IMPORT;
	nArrOtherResources[nCount++] = IDC_bKOLIS_NET;
	nArrOtherResources[nCount++] = IDC_bCLEARSCREEN;
	nArrOtherResources[nCount++] = IDC_bPREV_SPECIES;
	nArrOtherResources[nCount++] = IDC_bNEXT_SPECIES;
	nArrOtherResources[nCount++] = IDC_bSAVEAS;
	nArrOtherResources[nCount++] = IDC_bSAVE;
	nArrOtherResources[nCount++] = IDC_bCLOSE;
	nArrOtherResources[nCount++] = IDC_tTOTAL_VOLUMEP;
	nArrOtherResources[nCount++] = IDC_eTOTAL_VOLUMEP;	
	nArrOtherResources[nCount++] = IDC_tTOTAL_BOOKP;
	nArrOtherResources[nCount++] = IDC_eTOTAL_BOOKP;
	nArrOtherResources[nCount++] = IDC_bVOL_INFO;
	nArrOtherResources[nCount++] = IDC_tPRICE_TYPE;
	nArrOtherResources[nCount++] = IDC_tREG_CLASS_CODE;
	nArrOtherResources[nCount++] = IDC_eREG_CLASS_CODE;
	nArrOtherResources[nCount++] = IDC_STATIC_ISBN;
	nArrOtherResources[nCount++] = IDC_tCURRENCY_CLASS;
	nArrOtherResources[nCount++] = IDC_cCURRENCY_CLASS;
	nArrOtherResources[nCount++] = IDC_bAPPLY_SERIES;
	

	CRect rcResource;
	for (i = 0; i <nCount ; i++)
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->GetWindowRect(rcResource);
			ScreenToClient(rcResource);
			if (m_bShowCode)
			{
				rcResource.top += nCodeResourcesHeight;
				rcResource.bottom += nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
			else
			{
				rcResource.top -= nCodeResourcesHeight;
				rcResource.bottom -= nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
		}
	}

	// Resize Window Size
	GetWindowRect(rcResource);
	if (m_bShowCode)
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate();
	UpdateWindow();
}

INT CBOSpeciesDlg::MakeDupAddMode()
{
	if (m_strDupIndexKey.IsEmpty()) return -1;
	INT ids;

	m_bDupAddMode = TRUE;

	// ���� �ִ� �������� DM�� Ű�� �ٲپ��ش�.
	// m_pDM_Purchase->SetCellData(_T("BP_��KEY"), m_strDupIndexKey, 0);

	// ���� �ִ� å DM�� Ű�� �ٲپ� �ش�.  // ���� å�� UPDATE ���� �ٲ۴�.
	// ���� �ִ� å�� ��ǥå �÷��׸� ��� 'N'���� �ٲپ��ش�.
	CString strQueryFlag;
	INT nRowCount = m_pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));

		m_pDM_Book->SetCellData(_T("BB_��KEY"), m_strDupIndexKey, idx);
		m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		m_pDM_Book->SetCellData(_T("BB_��ǥå����"), _T("N"), idx);
	}		

	// �� DM, Index DM�� Ű�� ������ �����Ѵ�.
	CString strOption;
	strOption.Format(_T("REC_KEY = %s"), m_strDupIndexKey);
	ids = m_pDM_Species->RefreshDataManager(strOption);
	if ( ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ�����ּ���"));
		return -1;
	}

	ids = m_pDM_Index->RefreshDataManager(strOption);		
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ�����ּ���"));
		return -1;
	}


	// ȭ�� �����ֱ�
	Display();
	
	return 0;
}

INT CBOSpeciesDlg::Init()
{
	INT ids = -1;
	m_bDupAddMode = FALSE;

	// �� API �ʱ�ȭ
	if (m_pBOSpeciesApi) 
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}
	m_pBOSpeciesApi = new CBO_SPECIES(this);
	if (m_pBOSpeciesApi == NULL)
	{
		m_strLastErrMsg = _T("�� API �ʱ�ȭ ����");
		return -1;
	}
	
	// ��� ������ �޴��� ������ ���� �� �ʱ�ȭ	
	m_pDM_Index		= FindDM(_T("DM_����_��_��������"));
	m_pDM_Species	= FindDM(_T("DM_����_��_������"));
	m_pDM_Book		= FindDM(_T("DM_����_��_å����"));
	m_pDM_Purchase	= FindDM(_T("DM_����_��_��������"));
	//m_pDM_Donation = FindDM(_T("DM_����_��_��������"));

	if (m_pDM_Index == NULL || m_pDM_Species == NULL ||	
		m_pDM_Book == NULL ||m_pDM_Purchase == NULL	)
	{
		m_strLastErrMsg = _T("��� ������ �޴��� ������ ���� �� �ʱ�ȭ ����");
		return -1;
	}		

	m_pDM_Index->FreeData();
	m_pDM_Species->FreeData();
	m_pDM_Book->FreeData();
	m_pDM_Purchase->FreeData();

	// ��Ʈ�� �޴��� ������ ����
	m_pCM_Index		= FindCM(_T("CM_����_��_��������"));
	m_pCM_Purchase	= FindCM(_T("CM_����_��_��������"));

	if (m_pCM_Index == NULL || m_pCM_Purchase == NULL) 
	{
		m_strLastErrMsg = _T("��Ʈ�� �޴��� ������ ���� ����");
		return -1;
	}	

	// �ڷ���� �޺��� �� �־��ֱ�
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cWORKING_STATUS);
	if (pComboBox != NULL)
	{
		pComboBox->ResetContent();
		pComboBox->AddString(_T("���Դ���ڷ�"));
		pComboBox->AddString(_T("�˼�����ڷ�"));
		pComboBox->AddString(_T("�˼��Ϸ��ڷ�"));
		pComboBox->SetCurSel(0);

		if (m_strWorkingStatus == _T("���Դ���ڷ�"))
		{
			pComboBox->SetCurSel(0);
		}
		else if (m_strWorkingStatus == _T("�˼�����ڷ�"))
		{
			pComboBox->SetCurSel(1);
		}
		else if (m_strWorkingStatus == _T("�˼��Ϸ��ڷ�"))
		{
			pComboBox->SetCurSel(2);
		}
	}

	// �⺻�� ���������� �ʵ� Alias �����ϱ�
	m_pFormApi->InitDefaultAlias();
	m_pFormApi->AddDefaultAlias(_T("IBS_��ǥ��"));
	m_pFormApi->AddDefaultAlias(_T("IBS_������"));
	m_pFormApi->AddDefaultAlias(_T("IBS_�ǻ���"));
	m_pFormApi->AddDefaultAlias(_T("IBS_�Ǽ���"));
	m_pFormApi->AddDefaultAlias(_T("IBS_����"));
	m_pFormApi->AddDefaultAlias(_T("IBS_������"));
	m_pFormApi->AddDefaultAlias(_T("IBS_������"));
	m_pFormApi->AddDefaultAlias(_T("IBS_�����"));

	// Default IgnoreList �ۼ�
	m_pFormApi->InitDefaultIgnoreAlias();
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��ǥå����"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��Ϲ�ȣ"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_������ȣ"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_�谡��ġ��ȣ"));	
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));

	// �ڵ� �������� ��� �����ϱ�
	CString strValue;
	GetManageValue(_T("����"), _T("����"), _T("���������ڵ���������"), _T(""), strValue);
	if (strValue == _T("Y"))
		m_bAutoDupSearch = TRUE;
	else 
	{
		GetDlgItem(IDC_bDUP_SEARCH)->ShowWindow(SW_SHOW);
		m_bAutoDupSearch = FALSE;
	}
	
	/**
		���� ��庰�� DM ����
	*/ 

	// ����-��-����-�Է�
	if (GetOpenMode() == 1100)
	{
		// Form API ��� ����
		m_pFormApi->SetAppMode(MODE_INSERT);

		/**
			1å�� �����.
		*/

		// ����
		m_pDM_Index->InsertRow(-1);
		// ��
		m_pDM_Species->InsertRow(-1);
		// å
		m_pDM_Book->InsertRow(-1);
		// ��������
		m_pDM_Purchase->InsertRow(-1);

		// ��Ϲ�ȣ�� EM ���� �����Ѵ�.
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(_T("EM"));
		m_strRegCodeField = _T("EM");

		GetDlgItem(IDC_cWORKING_STATUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_bCLEARSCREEN)->ShowWindow(SW_SHOW);
		SetWindowText(_T("����-��-����-�Է�"));

		// �ʱⰪ�� �ҷ��´�.
		LoadDefaultValue();
	}
	// ����-��-����-�Է�
	else if (GetOpenMode() == 1200)
	{
		SetWindowText(_T("����-��-����-�Է�"));
	}
	// ����-��-����-����
	else if (GetOpenMode() == -1100)
	{
		// �θ� ���� üũ
		if (m_pDM_Parent == NULL || m_pGrid_Parent == NULL ||
			m_strSpeciesKeyAlias.IsEmpty() || m_strPurchaseKeyAlias.IsEmpty())
			return -1;

		// Form API ��� ����
		m_pFormApi->SetAppMode(MODE_UPDATE);

		// �θ�׸����� ���õ� �ο� ����
		ids = m_pGrid_Parent->SelectMakeList();
		if (ids < 0)
		{
			AfxMessageBox(_T("���ø���� ������ �� �����ϴ�."));
			return -1;
		}

		if (m_nParentSelectedRow == -1)
		{
			m_nParentSelectedRow = m_pGrid_Parent->SelectGetHeadPosition();
			if (m_nParentSelectedRow < 0)
			{
				AfxMessageBox(_T("���ø���� ������ �� �����ϴ�."));
				return -1;
			}
		}

		m_pGrid_Parent->SetReverse(m_nParentSelectedRow);

		ids = MakeUpdateDM();
		if (ids < 0)
		{
			AfxMessageBox(_T("UPDATE DM ���� ����"));
			return -1;
		}

		// ���� ���������� �Ǵ��Ѵ�.
		if (m_pDM_Index->GetCellData(_T("IBS_��������_�ڵ�"), 0) == _T("BOL112N"))
			MakeDupAddMode();
	
		GetDlgItem(IDC_bSAVEAS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bPREV_SPECIES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bNEXT_SPECIES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
		SetWindowText(_T("����-��-����-����"));

		Display();
	}
	// ����-��-����-����
	else if (GetOpenMode() == -1200)
	{
	}
	// ����-��-�Է�
	else if (GetOpenMode() == 3100)
	{
		
	}
	// ����-��-����
	else if (GetOpenMode() == -3100)
	{
		
	}
	// - ��Ȱ�� ���� : -7100
	else if (GetOpenMode() == -7100)
	{
		// �θ� ���� üũ
		if (m_pDM_Parent == NULL || m_pGrid_Parent == NULL ||
			m_strSpeciesKeyAlias.IsEmpty() || m_strPurchaseKeyAlias.IsEmpty())
			return -1;

		// Form API ��� ����
		m_pFormApi->SetAppMode(MODE_UPDATE);

		// �θ�׸����� ���õ� �ο� ����
		ids = m_pGrid_Parent->SelectMakeList();
		if (ids < 0)
		{
			AfxMessageBox(_T("���ø���� ������ �� �����ϴ�."));
			return -1;
		}

		m_nParentSelectedRow = m_pGrid_Parent->SelectGetHeadPosition();
		if (m_nParentSelectedRow < 0)
		{
			AfxMessageBox(_T("���ø���� ������ �� �����ϴ�."));
			return -1;
		}

		m_pGrid_Parent->SetReverse(m_nParentSelectedRow);

		// DM�� ���̺� �̸��� �ٲپ��ش�.
		// BO_DELETE_SPECIES_TBL;
		// BO_DELETE_BOOK_TBL;
		m_pDM_Species->TBL_NAME = _T("BO_DELETE_SPECIES_TBL");
		m_pDM_Book->TBL_NAME = _T("BO_DELETE_BOOK_TBL");		

		ids = MakeUpdateDM();
		if (ids < 0)
		{
			AfxMessageBox(_T("UPDATE DM ���� ����"));
			return -1;
		}

		SetWindowText(_T("��Ȱ�� ����"));
		GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
		Display();
	}
	else
	{
		return -1;
	}

	return 0;
}

INT CBOSpeciesDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias, CString strPurchaseKeyAlias, INT nSelectedRow /*= -1*/)
{
	if (pDM_Parent == NULL || pGrid_Parent == NULL || strSpeciesKeyAlias.IsEmpty() || strPurchaseKeyAlias.IsEmpty())
		return -1;
	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strSpeciesKeyAlias = strSpeciesKeyAlias;
	m_strPurchaseKeyAlias = strPurchaseKeyAlias;
	m_nParentSelectedRow = nSelectedRow;

	return 0;
}

INT CBOSpeciesDlg::ValidateData()
{
	// �� ���ߵ� �׸�
	
	// IBS_�Ǽ��� : IDC_eTITLE
	// IBS_������ : IDC_eAUTHOR1 ����
	// ��ϱ��� : IDC_eREG_CLASS_CODE
	// BP_��å�� : 
	// BP_ȭ�󱸺� : IDC_cCURRENCY_CLASS ����
	// BP_���� : IDC_ePRICE ����

	const INT nCount = 2;
	CString arrAlias[nCount] = {
		_T("�Ǽ���"), 
		_T("��ϱ���")
	};

	INT arrResource[2] = {
		IDC_eTITLE,
		IDC_eREG_CLASS_CODE
	};

	CString strText;

	CWnd *pWnd = NULL;
	for (INT i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(arrResource[i]);
		if (pWnd == NULL) continue;

		pWnd->GetWindowText(strText);
		if (strText.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("%s ��(��) �Է��� �ּ���"), arrAlias[i]);
			MessageBox(strMsg);
			pWnd->SetFocus();
			return -1;
		}
	}
	
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_cWORKING_STATUS);
	CString strWorkingStatus;
	if(pCmb)
		pCmb->GetWindowText(strWorkingStatus );

	if(strWorkingStatus == _T("�˼�����ڷ�") || strWorkingStatus == _T("�˼��Ϸ��ڷ�"))
	{
		GetControlData(_T("CM_����_��_��������"), _T("BP_ȭ�󱸺�"), strText);
		strText.TrimLeft();			strText.TrimRight();
		if(strText.IsEmpty())
		{
			MessageBox(_T("ȭ�󱸺��� ������ �ּ���"));
			GetDlgItem(IDC_cCURRENCY_CLASS)->SetFocus();
			return -1;
		}
		GetControlData(_T("CM_����_��_��������"), _T("BP_����"), strText);
		strText.TrimLeft();			strText.TrimRight();
		if(strText.IsEmpty() || strText == _T("0"))
		{
			MessageBox(_T("���������� �Է��� �ּ���"));
			GetDlgItem(IDC_ePRICE)->SetFocus();
			return -1;
		}
		GetControlData(_T("CM_����_��_��������"), _T("BP_����ó��ȣ"), strText);
		strText.TrimLeft();			strText.TrimRight();
		if(strText.IsEmpty())
		{
			MessageBox(_T("����ó ������ �Է��� �ּ���"));
			GetDlgItem(IDC_cPROVIDER)->SetFocus();
			return -1;
		}
	}
	/*
	GetControlData(_T("CM_����_��_��������"), _T("BP_ȭ�󱸺�"), strText);
	strText.TrimLeft();
	strText.TrimRight();
	if (strText.IsEmpty())
	{
		MessageBox(_T("ȭ�󱸺��� ������ �ּ���"));
		GetDlgItem(IDC_cCURRENCY_CLASS)->SetFocus();
		return -1;
	}
	*/
	
	return 0;
}

INT CBOSpeciesDlg::SetAuthorCMToDeletedUnlessWord(CESL_ControlMgr *pCM_Index, CString strCtrlAlias)
{
	CString strAuthor;
	GetControlData(_T("CM_����_��_��������"), strCtrlAlias, strAuthor);
	if(strAuthor.IsEmpty()) return 0;
	GetAuthorToDeletedUnlessWord(strAuthor);
	SetControlData(_T("CM_����_��_��������"), strCtrlAlias, strAuthor);
	return 0;
}

INT CBOSpeciesDlg::GetAuthorToDeletedUnlessWord(CString &strAuthor)
{
	CString strTmpData, strData;
	INT pos = strAuthor.Find(_T(";"), 0);
	if(pos < 0)
	{
		m_pBOSpeciesApi->DeleteUnlessWord(strAuthor);
		return 0;
	}
	strData.Empty();
	while(pos > 0)
	{
		strTmpData = strAuthor.Mid(0, pos);
		strTmpData.TrimLeft();		strTmpData.TrimRight();
		m_pBOSpeciesApi->DeleteUnlessWord(strTmpData);
		if(strData.IsEmpty()) strData = strTmpData;
		else strData += _T(";") + strTmpData;
		strAuthor = strAuthor.Mid(pos+1);
		strAuthor.TrimLeft();		strAuthor.TrimRight();
		pos = strAuthor.Find(_T(";"), 0);
		if(pos < 0)
		{
			m_pBOSpeciesApi->DeleteUnlessWord(strAuthor);
			if(strData.IsEmpty()) strData = strAuthor;
			else strData += _T(";") + strTmpData;
		}
	}

	strAuthor = strData;
	return 0;
}

INT CBOSpeciesDlg::ApplyData()
{
	INT ids = -1;
	CString strVol;

	// ����-��-����-�Է�
	if (GetOpenMode() == 1100)
	{
		// ���� CM -> ���� DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Index, 0);

		// �������� CM -> �������� DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Purchase, m_pDM_Purchase, 0);

		// �����
		m_pCM_Index->GetControlMgrData(_T("IBS_����"), strVol);

		// �ܱ��϶� �Էµ� ������� ���� ������� �ٸ��� ��� å�� �����Ѵ�.
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
		{
			if (m_pDM_Book->GetCellData(_T("BB_�����"), 0) != strVol)
			{
				m_pDM_Book->FreeData();
				m_pDM_Book->InsertRow(-1);
			}
		}

		// ���� �÷���
		CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_INSERT);

		// ǥ��å ���� �����
		MakeStandardBookFromIdx(strVol, strQueryFlag);

		/// �ܱ��϶� ��ȭ�� ���� ��������ش�.
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
			ApplyOneVolumeData(strVol, strQueryFlag);
		
		// Working Status ����
		ids = ApplyWorkingStatus();

		// �����߱� ��尡 �ƴϸ� ��ǥå �ϳ��� �����.
		if (!m_bDupAddMode)
		{
			m_pFormApi->SetRepresentationBook(m_pDM_Book);
		}

	}
	// ����-��-����-����
	else if (GetOpenMode() == -1100)
	{
		// ���� CM -> ���� DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Index, 0);

		// �������� CM -> �������� DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Purchase, m_pDM_Purchase, 0);

		// å DM RowCount
		INT nRowCount = m_pDM_Book->GetRowCount();

		// �����
		ids = m_pCM_Index->GetControlMgrData(_T("IBS_����"), strVol);

		// �ܱ��϶� ����� �ϰ� ����
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
		{						
			for (INT idx = 0; idx < nRowCount; idx++)
				ids = m_pDM_Book->SetCellData(_T("BB_�����"), strVol, idx);
		}

		/// ���� �÷���
		CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE);

		// �����߱� ��尡 �ƴϸ� ��ǥå ���� �����
		if (m_bDupAddMode)
		{
			// ��ǥå�� ã�´�.
			INT nIdx = -1;
			m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
			
			// ��ǥå ���� �����
			MakeStandardBookFromIdx(strVol, strQueryFlag, nIdx);
		}
				
		/// �ܱ��϶� ��ȭ�� ���� ��������ش�.
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
			ApplyOneVolumeData(strVol, strQueryFlag);
	}
	// - ��Ȱ�� ���� : -7100
	else if (GetOpenMode() == -7100)
	{
		// ���� CM -> ���� DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Index, 0);

		// �������� CM -> �������� DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Purchase, m_pDM_Purchase, 0);

		// å DM RowCount
		INT nRowCount = m_pDM_Book->GetRowCount();

		// �����
		ids = m_pCM_Index->GetControlMgrData(_T("IBS_����"), strVol);

		// �ܱ��϶� ����� �ϰ� ����
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
		{					
			for (INT idx = 0; idx < nRowCount; idx++)
				ids = m_pDM_Book->SetCellData(_T("BB_�����"), strVol, idx);
		}

		// ���� �÷���
		CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE);

		// �����߱� ��尡 �ƴϸ� ��ǥå ���� �����
		if (m_bDupAddMode)
		{
			// ��ǥå�� ã�´�.
			INT nIdx = -1;
			m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
		
			// ��ǥå ���� �����
			MakeStandardBookFromIdx(strVol, strQueryFlag, nIdx);
		}
	}
	else
	{
		return -1;
	}

	// �����߱� ����̸� ��Ű�� �������ش�.
	if (m_bDupAddMode)
	{
		INT nRowCount = m_pDM_Book->GetRowCount();
		for (INT idx = 0; idx < nRowCount; idx++)
		{
			//strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));

			m_pDM_Book->SetCellData(_T("BB_��KEY"), m_strDupIndexKey, idx);
			// m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
			m_pDM_Book->SetCellData(_T("BB_��ǥå����"), _T("N"), idx);
		}		
	}

	
	// �������� ����
	ids = ApplyCalculation();
	if (ids < 0)
	{
		AfxMessageBox(_T("�������� ���� ����"));
		return -1;
	}
	else if (ids > 0)
	{
		AfxMessageBox(_T("�Ǻ������� ������ �����ִ� ���� �ֽ��ϴ�."));
		return -1;
	}

	return 0;
}

INT CBOSpeciesDlg::ApplyIndexToMarc(INT idx /*= 0*/)
{
	INT ids = -1;
	CMarc marc;
	CString strMarc;

	ids = ApplyData();
	if (ids < 0) return ids;

	// �� -> ��ũ
	strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	if (!strMarc.IsEmpty())
	{
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if (ids < 0) return -1;
	}
	ids = m_pBOSpeciesApi->APPLY_Species_to_CMarc(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
	m_pInfo->pMarcMgr->CheckSimpleSyntax(&marc);
	ids = m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if (ids < 0) return ids;
	ids = m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
	return ids;
}

INT CBOSpeciesDlg::ApplyRegNoToMarc(INT idx /*= 0*/)
{
	INT ids = -1;
	CMarc marc;
	CString strMarc;
	
	ids = m_pDM_Species->GetCellData(_T("BS_MARC"), idx, strMarc);
	if (ids < 0) return ids;

	ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	if (ids < 0) return ids;

	// ������ 049Tag�� �����.
	ids = m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
	if (ids < 0) return ids;
	
	// ������ å�� ��Ϲ�ȣ�� ������ ��Ϲ�ȣ�� 049$l�� �ִ´�.
	CString strRegNo;
	INT nRowCount = m_pDM_Book->GetRowCount();
	for (INT i = 0; i < nRowCount; i++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;

		// ��Ϲ�ȣ�� ������ �Ѿ��.
		strRegNo = m_pDM_Book->GetCellData(_T("BB_��Ϲ�ȣ"), i);
		if (strRegNo.IsEmpty()) continue;

		// ��Ϲ�ȣ�� 049$l�� �ִ´�.
		ids = m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), strRegNo);
	}

	ids = m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if (ids < 0) return ids;

	// �� DM�� �ֱ�
	ids = m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, idx);
	return ids;
}

INT CBOSpeciesDlg::MakeStandardBookFromIdx(CString strVol, CString strQueryFlag, INT idx /*= -1*/)
{
	INT ids = -1;
	
	// idx �� -1�̸� ù��° å�� ã�Ƽ� ǥ�������� ������ش�.
	if (idx == -1)
	{
		INT nRowCount = m_pDM_Book->GetRowCount();
		for (INT i = 0; i < nRowCount; i++)
			if (m_pFormApi->IsBook(m_pDM_Book, i)) break;
		
		idx = i;
	}

	/// ���������� ���� ǥ�� ���� �����
	if(!m_bViewVolInfo || !m_bDupAddMode)
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Book, idx, _T("BB"));
	/// ����� ����
	ids = m_pDM_Book->SetCellData(_T("BB_�����"), strVol, idx);
	/// ���� �÷��� ����
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);

	return 0;
}

INT CBOSpeciesDlg::ApplyOneVolumeData(CString strVol, CString strQueryFlag)
{
	// �ܱ��϶� ��ȭ�� ���� ��������ش�.

	// ��ϱ��� �����Ű��
	CString strRegCodeField;
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCodeField);

	CArray<CRegCode, CRegCode> arrRegCode;
	INT ids = m_pFormApi->FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0 || arrRegCode.GetSize() <= 0)
	{
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
		m_strLastErrMsg = _T("��ϱ��� �ʵ尡 �߸��Ǿ����ϴ�.");
		return -1;
	}

	m_strRegCodeField = strRegCodeField;
	m_pFormApi->MakeBookFromRegCode(&arrRegCode, m_pDM_Book, strVol);

	// ���������� ���� å ���� �����Ű��
	INT nBookCount = m_pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nBookCount; idx ++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
		
		// ���� CM -> å DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Book, idx, _T("BB"));

		// ����� ����
		ids = m_pDM_Book->SetCellData(_T("BB_�����"), strVol, idx);

		// ���� �÷��� ����
		ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);

		//�ܱǺ��϶� ���������� å�� �����Ű��
		ids = m_pDM_Book->SetCellData(_T("BB_����������"), m_pDM_Purchase->GetCellData(_T("BP_����������"), 0), idx);
	}

	return 0;
}

INT CBOSpeciesDlg::ApplyWorkingStatus()
{
	INT idx = 0;
	INT nRowCount = 0;

	CString strWorkingStatus;
	CString strBookWorkingStatus;		
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cWORKING_STATUS);
	if (pComboBox == NULL) goto ERR;
	
	pComboBox->GetLBText(pComboBox->GetCurSel(), strWorkingStatus);
	strWorkingStatus = m_pFormApi->ConvertWORKING_STATUSToCode(strWorkingStatus);
	if (strWorkingStatus.IsEmpty()) goto ERR;	


	if (GetOpenMode() > 0)
	{
		// ���� Working Status�� �ִ´�.
		m_pDM_Species->SetCellData(_T("BS_�ڷ����_�ڵ�"), strWorkingStatus, 0);

		// ���ο� Working Status�� �ִ´�.
		m_pDM_Index->SetCellData(_T("IBS_�۾�����INFO"), strWorkingStatus, 0);
	}
	
	// å�� Working Status�� �ִ´�.
	nRowCount = m_pDM_Book->GetRowCount();

	strBookWorkingStatus = _T("");
	for (idx = 0; idx < nRowCount; idx++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
		// å�� �̹� �۾����°� ������ ���¶�� continue
		m_pDM_Book->GetCellData(_T("BB_�ڷ����"), idx, strBookWorkingStatus);
		if(strBookWorkingStatus.IsEmpty())
			m_pDM_Book->SetCellData(_T("BB_�ڷ����"), strWorkingStatus, idx);
	}

	return 0;

ERR:
	m_strLastErrMsg = _T("Working Status���� ����");
	return -1;	
}

INT CBOSpeciesDlg::ApplyCalculation()
{
	INT ids = -1;
	CESL_ControlMgr *pCM = FindCM(_T("CM_����_��_��������"));
	ids = m_pFormApi->MakeDMFromCM(pCM, m_pDM_Purchase, 0);

	CString strPriceType;
	ids = pCM->GetControlMgrData(_T("BP_�ֹ�����Ÿ��"), strPriceType);
	if (ids < 0) return -1;

	CString strTemp;

	INT nResult = 0;
	INT nDiscountRate = 0;
	ids = pCM->GetControlMgrData(_T("BP_������"), strTemp);
	if (ids < 0) return -1;
	nDiscountRate = _ttoi(strTemp);

	INT nPrice = 0;
	// ���� == �������� ��
	if (strPriceType == _T("������"))
	{
		ids = pCM->GetControlMgrData(_T("BP_����"), strTemp);
		if (ids < 0) return -1;

		nPrice = _ttoi(strTemp);

		// ������ 1/n���� ���� ������ å�� �����Ų��.
		ids = m_pFormApi->ApplySpeciesPriceToBookPrice(m_pDM_Book, nPrice);
		if (ids < 0) return -1;
	}
	// ���� == ���� å�� ������ ���Ѱ�
	else if (strPriceType == _T("�ǰ���"))
	{
		ids = m_pFormApi->GetBookPriceSum(m_pDM_Book, nPrice);
		if (ids < 0) return -1;
		nResult = ids;
		strTemp.Format(_T("%d"), nPrice);
		pCM->SetControlMgrData(_T("BP_����"), strTemp);	
	}
	else
	{
		return -1;
	}

	// ���ְ��� ���
	// ���ְ��� = ���� * (1 - ������)
	nPrice = (nPrice * (100 - nDiscountRate)) / 100;

	strTemp.Format(_T("%d"), nPrice);
	ids = pCM->SetControlMgrData(_T("BP_���ְ���"), strTemp);

	return nResult;
}

INT CBOSpeciesDlg::Display()
{
	// ����-��-�����߱�
	if (m_bDupAddMode)
	{
		ShowRegCodeField();
		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		CString strVolumeCount;
		strVolumeCount.Format(_T("%d"), nVolumeCount);
		m_pCM_Purchase->SetControlMgrData(_T("BP_�ѱǼ�"), strVolumeCount);
		
		if (nVolumeCount == 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
			GetDlgItem(IDC_eVOL)->EnableWindow(TRUE);
		}
		else if (nVolumeCount > 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_eVOL)->EnableWindow(FALSE);
		}

		// ���� ���� Disable
		m_pCM_Index->AllControlReadOnly();
		
		// ������ Disable
		CESL_ControlMgr *pCM = FindCM(_T("CM_����_��_������"));
		if (pCM)
		{
			pCM->AllControlReadOnly();
		}
		
		AllControlDisplay(_T("CM_����_��_��������"), 0);
		AllControlDisplay(_T("CM_����_��_������"), 0);
	}
	// ����-��-����-�Է�
	else if (GetOpenMode() == 1100)
	{
		ShowRegCodeField();
		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		CString strVolumeCount;
		strVolumeCount.Format(_T("%d"), nVolumeCount);
		m_pCM_Purchase->SetControlMgrData(_T("BP_�ѱǼ�"), strVolumeCount);
		
		if (nVolumeCount == 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
			GetDlgItem(IDC_eVOL)->EnableWindow(TRUE);
		}
		else if (nVolumeCount > 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_eVOL)->EnableWindow(FALSE);
		}

		
		// ��ǥå�� ã�´�.
		INT nIdx = -1;
		m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
		// ��ǥå�� ���ο� �ִ´�.
		m_pFormApi->CopyDM(m_pDM_Book, nIdx, m_pDM_Index, 0, _T("IBS"));
		// ���������� �������������� -- �ܱǺ��ϰ��
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
		{
			m_pDM_Purchase->SetCellData(_T("BP_����������"), m_pDM_Book->GetCellData(_T("BB_����������"), nIdx), 0);
			SetControlData(_T("CM_����_��_��������"), _T("BP_����������"), m_pDM_Book->GetCellData(_T("BB_����������"), nIdx));
		}
		AllControlDisplay(_T("CM_����_��_��������"), 0);

		// ��ǥå�� ����� ǥ��
		CString strVol;
		m_pDM_Book->GetCellData(_T("BB_�����"), nIdx, strVol);
		m_pCM_Index->SetControlMgrData(_T("IBS_����"), strVol);

	}
	// ����-��-����-�Է�
	else if (GetOpenMode() == 1200)
	{
	}
	// ����-��-����-����
	else if (GetOpenMode() == -1100)
	{
		AllControlDisplay(_T("CM_����_��_��������"), 0);
		
		ShowRegCodeField();
		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		CString strVolumeCount, strPriceType;
		strVolumeCount.Format(_T("%d"), nVolumeCount);
		m_pCM_Purchase->SetControlMgrData(_T("BP_�ѱǼ�"), strVolumeCount);
		//�ֹ�����Ÿ�Կ� ���� CM�� �ֹ�����Ÿ�� �޺���Ʈ���� �����Ѵ�.
		m_pDM_Purchase->GetCellData(_T("BP_�ֹ�����Ÿ��"), 0, strPriceType);
		CESL_ControlComboBox * pCombo = (CESL_ControlComboBox*)FindControl(_T("CM_����_��_��������"), _T("BP_�ֹ�����Ÿ��"));
		if(strPriceType.IsEmpty() || strPriceType == _T("������"))
			pCombo->SetCurSel(0);
		else 
			pCombo->SetCurSel(1);
		
		// ��ǥå�� ã�´�.
		INT nIdx = -1;
		m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
		
		m_pDM_Index->GetCellData(_T("IBS_������"), 0);
		// ��ǥå�� ���ο� �ִ´�.
		m_pFormApi->CopyDM(m_pDM_Book, nIdx, m_pDM_Index, 0, _T("IBS"));
		m_pDM_Index->GetCellData(_T("IBS_������"), 0);
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
		{
			m_pDM_Purchase->SetCellData(_T("BP_����������"), m_pDM_Book->GetCellData(_T("BB_����������"), nIdx), 0);
			SetControlData(_T("CM_����_��_��������"), _T("BP_����������"), m_pDM_Book->GetCellData(_T("BB_����������"), nIdx));
		}

		AllControlDisplay(_T("CM_����_��_��������"), 0);

		// ��ǥå�� ����� ǥ��
		CString strVol;
		m_pDM_Book->GetCellData(_T("BB_�����"), nIdx, strVol);
		m_pCM_Index->SetControlMgrData(_T("IBS_����"), strVol);
	}
	// ����-��-����-����
	else if (GetOpenMode() == -1200)
	{
	}
	// ����-��-�Է�
	else if (GetOpenMode() == 3100)
	{
	}
	// ����-��-����
	else if (GetOpenMode() == -3100)
	{
	}
	else
	{
		return -1;
	}
	return 0;
}

INT CBOSpeciesDlg::ShowRegCodeField()
{
	CArray<CRegCode, CRegCode> arrRegCode;
	m_pFormApi->GetRegCodeInfo(m_pDM_Book, &arrRegCode);
	CRegCode regCode;
	CString strTemp;
	CString strRegCodeField;
	INT nBookCount = 0;
	INT nArrCount = arrRegCode.GetSize();
	if (nArrCount != 0)
	{
		for (INT i = 0; i < nArrCount - 1; i++)
		{
			regCode = arrRegCode.GetAt(i);
			nBookCount += regCode.m_nCount;
			strTemp.Format(_T("%s%d/"), regCode.m_strRegCode, regCode.m_nCount);
			strRegCodeField += strTemp;
		}
		
		regCode = arrRegCode.GetAt(nArrCount - 1);	
		nBookCount += regCode.m_nCount;
		strTemp.Format(_T("%s%d"), regCode.m_strRegCode, regCode.m_nCount);
		strRegCodeField += strTemp;

		// ��ϱ��� �ʵ�
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegCodeField);

		// �� å��
		CString strBookCount;
		strBookCount.Format(_T("%d"), nBookCount);

		m_pCM_Purchase->SetControlMgrData(_T("BP_��å��"), strBookCount);
	}

	return 0;
}

INT CBOSpeciesDlg::MakeUpdateDM()
{
	if (m_nParentSelectedRow < 0) return -1;

	INT ids = -1;
	
	// ������ Row�� SpeciesKey, PurchaseKey ��������
	CString strSpeciesKey;
	CString strPurchaseKey;

	m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, m_nParentSelectedRow, strSpeciesKey);
	m_pDM_Parent->GetCellData(m_strPurchaseKeyAlias, m_nParentSelectedRow, strPurchaseKey);

	CString strOption;

	if (strSpeciesKey.IsEmpty()) return -1;
	if (strPurchaseKey.IsEmpty()) return -1;
	
	// ���� DM ����
	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = m_pDM_Index->RefreshDataManager(strOption);
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ�����ּ���"));
		return -1;
	}
	
	// �� DM ����
	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = m_pDM_Species->RefreshDataManager(strOption);
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ�����ּ���"));
		return -1;
	}

	// �������� DM ����
	strOption.Format(_T("REC_KEY = %s"), strPurchaseKey);
	ids = m_pDM_Purchase->RefreshDataManager(strOption);
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ�����ּ���"));
		return -1;
	}

	// å ���� DM ����
	strOption.Format(_T("SPECIES_KEY = %s AND ACCU_KEY = %s AND REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY REPRESENT_BOOK_YN DESC, VOL"), strSpeciesKey, strPurchaseKey);
	ids = m_pDM_Book->RefreshDataManager(strOption);
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ�����ּ���"));
		return -1;
	}

	return 0;
}

INT CBOSpeciesDlg::SaveDefaultValue()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_����_��_��������"));
	if (pCM == NULL) return -1;
	
	CString strFormCode;
	m_pFormApi->SaveDefaultValue(pCM, m_strDefaultValueFilePath);
	return 0;
}

INT CBOSpeciesDlg::LoadDefaultValue()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_����_��_��������"));
	if (pCM == NULL) return -1;
	
	m_pFormApi->LoadDefaultValue(pCM, m_strDefaultValueFilePath);
	
	return 0;
}

VOID CBOSpeciesDlg::MakeGroupNameList()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_����_��_��������"));
	if (pCM == NULL) return ;
	CString strAcqYear;
	pCM->GetControlMgrData(_T("Group�⵵"), strAcqYear);
	if(strAcqYear.IsEmpty()) strAcqYear = m_pBOSpeciesApi->GetTodayYear();
	//Group�޴����� �޼ҵ带 ����Ͽ� CString[]�� Group�̸� ����Ʈ�� �����Ѵ�.
}

/////////////////////////////////////////////////////////////////////////////
// CBOSpeciesDlg message handlers

BOOL CBOSpeciesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Check Open Mode
	if (GetOpenMode() == -1)
	{
		AfxMessageBox(_T("Open Mode�� ������ �ּ���"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// Initialize Esl Manger	
	INT ids = InitESL_Mgr(_T("SM_����_��"));
	if (ids < 0) 
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// Initialize Data Mangager
	ids = Init();
	if (ids < 0)
	{
		MessageBox(GetLastError());
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// ������ rect ����
	GetWindowRect(m_rcWindow);

	// �ڵ� ���а� ��Ʈ�� �����ֱ�
	m_bShowCode = FALSE;
	ShowCodeResource();

	// �������� ǥ��
	OnSelchangecCLASIFYCLASS();

	//Group�޺� ����Ʈ ������ ����
	MakeGroupNameList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOSpeciesDlg::OnbSHOWHIDECODE() 
{
	if (m_bShowCode)
	{
		m_bShowCode = FALSE;
	}
	else
	{
		m_bShowCode = TRUE;
	}

	ShowCodeResource();
}

VOID CBOSpeciesDlg::OnbCLOSE() 
{
	CDialog::OnOK();	
}

VOID CBOSpeciesDlg::OnSelchangecCLASIFYCLASS() 
{
	//�ٲ𶧸��� �з� ������ �ٲپ� �ش�. 

    CESL_ControlMgr *pCM_SPECIES = FindCM(_T("CM_����_��_��������"));

	CESL_ControlMgr *pCM_Index = FindCM(_T("CM_����_��_��������"));
	CESL_ControlMgr *pCM_Purchase = FindCM(_T("CM_����_��_��������"));
	if (pCM_Index == NULL || pCM_Purchase == NULL) return;
	
	CESL_ControlMultiComboBox *pCM_WhichCode = (CESL_ControlMultiComboBox *)pCM_Index->FindControl(_T("IBS_�з�ǥ����"));
	if (!pCM_WhichCode) return;
    CESL_ControlMultiComboBox *pCM_Subject = (CESL_ControlMultiComboBox *)pCM_Purchase->FindControl(_T("BP_��������_�ڵ�"));
	if (!pCM_Subject) return;
  
	CString class_name;
	CString strWhichCode;
	pCM_WhichCode->GetData(strWhichCode, pCM_WhichCode->GetCurSel(), 1);
	strWhichCode.TrimLeft();		strWhichCode.TrimRight();
	strWhichCode.MakeUpper();
	class_name = _T("");
	if (strWhichCode == _T("KDC")) class_name = _T("KDC��������ȣ����");
	if (strWhichCode == _T("DDC")) class_name = _T("DDC��������ȣ����");

	pCM_Subject->FreeComboData();
	pCM_Subject->ResetContent();
	pCM_Subject->InitMultiComboBox(2);
	if (class_name.GetLength() == 0) return ;

	INT nCode = 256;
	INT nDefaultValue = 0;
	CString strCode[256], strDesc[256];
	nDefaultValue = 0;
	m_pInfo->pCodeMgr->GetCode(class_name, strCode, strDesc, nCode, nDefaultValue);	
	if (nCode == 0) return ;



	CString strDefaultValue = m_pDM_Purchase->GetCellData(_T("BP_��������_�ڵ�"), 0);
		
	for (INT i = 0; i < nCode; i++) {
		if (strCode[i].GetLength() == 0) strCode[i] = _T("  ");
		
		if (strDefaultValue == strCode[i]) nDefaultValue = i;

		
		pCM_Subject->AddItem(strCode[i], 0, i);
		pCM_Subject->AddItem(strDesc[i], 1, i);
	}

	pCM_Subject->SetCurSel(nDefaultValue);	
}

VOID CBOSpeciesDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if (m_pFormApi != NULL)
	{
		delete m_pFormApi;
		m_pFormApi = NULL;
	}

	if (m_pBOSpeciesApi != NULL)
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}

	if (m_pNoteCtrl != NULL)
	{
		m_pNoteCtrl->DestroyWindow();
		delete m_pNoteCtrl;
	}
}

VOID CBOSpeciesDlg::OnbDEFAULTCONFIG() 
{
	if (SaveDefaultValue() < 0)
		AfxMessageBox(_T("�⺻�� ���� ����"));
	else
		AfxMessageBox(_T("�⺻���� �����Ͽ����ϴ�."), MB_ICONINFORMATION);
}

VOID CBOSpeciesDlg::OnbPREVSPECIES() 
{
	if (m_nParentSelectedRow < 0) return;
	INT nIdx = m_pGrid_Parent->SelectGetPrev();
	if (nIdx >= 0)
		m_nParentSelectedRow = nIdx;		
	else
		m_nParentSelectedRow = m_pGrid_Parent->SelectGetHeadPosition();
	if (m_nParentSelectedRow < 0) return;

	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);
	if (MakeUpdateDM() < 0)
	{
		AfxMessageBox(_T("UPDATE DM ���� ����"));
		return;
	}

	Display();
}

VOID CBOSpeciesDlg::OnbNEXTSPECIES() 
{
	if (m_nParentSelectedRow < 0) return;
	INT nIdx = m_pGrid_Parent->SelectGetNext();
	m_nParentSelectedRow = nIdx;
	if (nIdx >= 0)
		m_nParentSelectedRow = nIdx;
	else
		m_nParentSelectedRow = m_pGrid_Parent->SelectGetTailPosition();
	if (m_nParentSelectedRow < 0) return;

	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);
	if (MakeUpdateDM() < 0)
	{
		AfxMessageBox(_T("UPDATE DM ���� ����"));
		return;
	}
	Display();
}

VOID CBOSpeciesDlg::OnbSAVEAS() 
{
	if (GetOpenMode() != -1100) return;

	// ���޽���
	if (MessageBox(_T("�������� �۾��� �Ͻø� ���̻� ���� �۾��� �� �� �����ϴ�.\r\n"
				_T("����Ͻðڽ��ϱ�?")), NULL, MB_YESNO | MB_ICONQUESTION) != IDYES) return;

	// å�� U -> I
	INT nRowCount = m_pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// �÷��װ� U�� �ƴϸ� �Ѿ��.
		if (m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) != _T("U")) continue;

		m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);
	}

	/**
		���� �÷ο�
	*/
	
	// ������ Ȯ��
	INT ids = ValidateData();
	if (ids < 0) return;

	// �Է��� ������ ����
	SetOpenMode(1100);
	ids = ApplyData();
	if (ids < 0)
	{
		AfxMessageBox(_T("������ �������"));
		return;
	}

	CESL_DataMgr *pDM_TARGET = NULL;
	if (pParentMgr->SM_ALIAS == _T("K2UP_LIGHT_����_����")) 
		pDM_TARGET = pParentMgr->FindDM(_T("DM_LIGHT_����_����_��"));


	// ��ũ ���۷��̼�
	ids = ApplyIndexToMarc();
	if (ids < 0)
	{
		AfxMessageBox(_T("���� ����"));
		EndDialog(IDOK);
		return;
	}

	// ����
	ids = m_pBOSpeciesApi->BO_SPECIES_PURCHASE_INSERT(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0, pDM_TARGET);
	if (ids < 0)
		AfxMessageBox(_T("���� ����"));
	else
	{
		MessageBox(_T("���� �Ͽ����ϴ�"), NULL, MB_OK | MB_ICONINFORMATION);	
		m_bControlDisplay = TRUE;
		OnbCLEARSCREEN();
		LoadDefaultValue();			
	}

	// �ݱ�
	EndDialog(IDOK);
}

VOID CBOSpeciesDlg::OnbSAVE() 
{
	// ������ Ȯ��
	INT ids = ValidateData();
	if (ids < 0) return;

	// �Է��� ������ ����
	ids = ApplyData();
	if (ids < 0)
	{
		AfxMessageBox(_T("������ �������"));
		return;
	}	

	CESL_DataMgr *pDM_TARGET = NULL;
	if (pParentMgr->SM_ALIAS == _T("K2UP_LIGHT_����_����")) 
		pDM_TARGET = pParentMgr->FindDM(_T("DM_LIGHT_����_����_��"));
	
	
	// �� API�� ���¸�� ����
	m_pBOSpeciesApi->m_nOpenMode = GetOpenMode();

	CMarc marc;
	CString strMarc;

	// �����߱�
	if (m_bDupAddMode)
	{
		// OpenMode�� ���� �����߱� �Լ��� �ҷ��´�. 
		if (GetOpenMode() == 1100)
		{
			ids = m_pBOSpeciesApi->BO_SPECIES_PURCHASE_ADDBOOK(_T("INSERT"), m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
		}
		else if (GetOpenMode() == -1100)
		{
			ids = m_pBOSpeciesApi->BO_SPECIES_PURCHASE_ADDBOOK(_T("UPDATE"), m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
		}

		if (ids < 0)
		{
			AfxMessageBox(_T("���� ����"));
			return;
		}
		else
		{
			MessageBox(_T("���� �Ͽ����ϴ�."), NULL, MB_OK | MB_ICONINFORMATION);	
			m_bControlDisplay = TRUE;
		}
	}
	// ����-��-����-�Է�
	else if (GetOpenMode() == 1100)
	{
		// ��ũ ���۷��̼�
		ids = ApplyIndexToMarc();
		if (ids < 0)
		{
			AfxMessageBox(_T("���� ����"));
			return;
		}
		ids = m_pBOSpeciesApi->BO_SPECIES_PURCHASE_INSERT(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0, pDM_TARGET);
		if (ids < 0)
			AfxMessageBox(_T("���� ����"));
		else
		{
			if (m_pGrid_Parent) m_pGrid_Parent->DisplayLine(pDM_TARGET->GetRowCount()-1);
			MessageBox(_T("���� �Ͽ����ϴ�"), NULL, MB_OK | MB_ICONINFORMATION);	
			m_bControlDisplay = TRUE;
			
			OnbCLEARSCREEN();
			LoadDefaultValue();			
		}
	}
	// ����-��-����-����
	else if (GetOpenMode() == -1100)
	{
		// ��ũ ���۷��̼�
		ids = ApplyIndexToMarc();
		if (ids < 0)
		{
			AfxMessageBox(_T("���� ����"));
			return;
		}
		
		ids = m_pBOSpeciesApi->BO_SPECIES_PURCHASE_UPDATE(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0, pDM_TARGET, m_nParentSelectedRow);
		if (ids < 0)
			AfxMessageBox(_T("���� ����"));
		else
		{
			if (m_pGrid_Parent) m_pGrid_Parent->DisplayLine(m_nParentSelectedRow);
			MessageBox(_T("���� �Ͽ����ϴ�"), NULL, MB_OK | MB_ICONINFORMATION);	
			m_bControlDisplay = TRUE;
			Init();
		}
	}
	// - ��Ȱ�� ���� : -7100
	else if (GetOpenMode() == -7100)
	{
		// ��ũ ���۷��̼�
		ids = ApplyIndexToMarc();
		if (ids < 0)
		{
			AfxMessageBox(_T("���� ����"));
			return;
		}

		ids = m_pBOSpeciesApi->BO_DELETE_SPECIES_PURCHASE_UPDATE(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
		if (ids < 0)
			AfxMessageBox(_T("���� ����"));
		else
		{
			MessageBox(_T("���� �Ͽ����ϴ�"), NULL, MB_OK | MB_ICONINFORMATION);
			m_bControlDisplay = TRUE;
		}
	}
}

VOID CBOSpeciesDlg::OnbMARCEDIT() 
{
	INT ids = -1;

	CMarc marc;
	CString strMarc;
	ids = m_pDM_Species->GetCellData(_T("BS_MARC"), 0, strMarc);
	if (!strMarc.IsEmpty())
	{
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if (ids < 0) return;
	}

	// ���� ���� ���� (����߰���)
	CString strLeader;
	marc.GetLeader(strLeader);
	// ���ڵ� ���� ������ �ִ´�. 5��°
	if (strLeader.GetAt(5) == ' ')
		strLeader.SetAt(5, 'n');

	// ���ڵ� ���� ������ �ִ´�. 6��°
	if (strLeader.GetAt(6) == ' ')
		strLeader.SetAt(6, 'a');

	// �������� ������ �ִ´�. 7��°
	if (strLeader.GetAt(7) == ' ')
		strLeader.SetAt(7, 'm');

	marc.SetLeader(strLeader.GetBuffer(0));


	// �����߱� ����ϰ��� ��ũ���⸸ �ȴ�.
	if (m_bDupAddMode)
	{
		CBOMarcEditDlg dlg(this, &marc);
		dlg.SetReadOnly();
		dlg.DoModal();
	}
	else
	{
		ids = ApplyData();
		if (ids < 0) return;

		// �� -> ��ũ
		ids = m_pBOSpeciesApi->APPLY_Species_to_CMarc(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
		
		m_pInfo->pMarcMgr->CheckSimpleSyntax(&marc);
		
		// ��Ʈ ���� ���̾�α� ����
		CBOMarcEditDlg dlg(this, &marc);
		dlg.DoModal();

		// ��ũ -> ��
		ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
		
		// �� Marc��
		m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
		m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);

		Display();
	}
}

VOID CBOSpeciesDlg::OnbMARCIMPORT() 
{
	CMARC_IMPORT dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		INT ids = -1;

		if (dlg.m_strSpecies_key.IsEmpty()) goto ERR;

		CString strMarc;
		CString strQuery;
		strQuery.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY = %s"), dlg.m_strSpecies_key);
		ids = m_pDM_Species->GetOneValueQuery(strQuery, strMarc);
		if (ids < 0) goto ERR;

		CMarc marc;
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if (ids < 0) goto ERR;

		// ���� : Marc to Index
		//ids = m_pBOSpeciesApi->CMARC_to_INDEX(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
		ids = m_pBOSpeciesApi->CMARC_to_INDEX(&marc, m_pDM_Index, 0);
		if (ids < 0) goto ERR;

		//0: �⺻ ���� ���� -> ��ũ�� IDX�� ���� (��ũ�� ����)
		if (dlg.m_nRadBasicInfo == 0)
		{
			//���� ���� -> ��ǥå���� -> å�� ����ɶ� ��ǥ���ڸ� ���� �Ѵ�.
			m_pFormApi->CopyDM(m_pDM_Index, 0, m_pDM_Book, 0, _T("BB"));
		}
		//1: ��ü ���� ���� -> ��ũ�� IDX���� MARC�� ������
		else if (dlg.m_nRadBasicInfo == 1 || dlg.m_nRadBasicInfo == 2)
		{
			ids = m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
			if (ids < 0) goto ERR;

			//2: �ٱ����������� ��ü�������� -> 1 + å�� �����´�.
			if (dlg.m_nRadBasicInfo == 2)
			{
				CString strOption;
				strOption.Format(_T("SPECIES_KEY = %s"), dlg.m_strSpecies_key);
				ids = m_pDM_Book->RefreshDataManager(strOption);
				if (ids < 0) goto ERR;
			}
		}
		m_pFormApi->SetRepresentationBook(m_pDM_Book);
		
		Display();
		AfxMessageBox(_T("Marc�� �����Ͽ����ϴ�."), MB_ICONINFORMATION);
		return;
	}

	return;

ERR:
	AfxMessageBox(_T("Marc ���� ����"));
	return;

}

VOID CBOSpeciesDlg::OnbKOLISNET() 
{
	// TODO: ������� ����
	CUC_MARC_IMPORT dlg(this);
	if (dlg.DoModal() != IDOK) return;

	CMarc marc;
	
	if(dlg.sMarc.IsEmpty()) return;
	INT ids = m_pInfo->pMarcMgr->Decoding(dlg.sMarc, &marc);
	if (ids < 0) goto ERR;

	// ��ũ�� IDX ���� (��ũ�� ����)
	//ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
	ids = m_pBOSpeciesApi->CMARC_to_INDEX(&marc, m_pDM_Index, 0);
	if (ids < 0) goto ERR;
	
	// MARC ������ ����
	ids = m_pDM_Species->SetCellData(_T("BS_MARC"), dlg.sMarc, 0);
	if (ids < 0) goto ERR;
	
	m_pFormApi->SetRepresentationBook(m_pDM_Book);

	Display();
	MessageBox(_T("��������� �����Ͽ����ϴ�."));
	return;

ERR :
	MessageBox(_T("������� ���Խ���"));
	
}

VOID CBOSpeciesDlg::OnbCLEARSCREEN() 
{
	//Init();
	CESL_ControlMgr *pCM = NULL;
	pCM = FindCM(_T("CM_����_��_��������"));
	if (pCM)
	{
		pCM->AllControlClear();
	}

	
	pCM = FindCM(_T("CM_����_��_��������"));
	if (pCM)
	{
		pCM->AllControlClear();
	}

	//Init();
	OnKillfocuseREGCLASSCODE();
	//Display();
}

VOID CBOSpeciesDlg::OnbVOLINFO() 
{
	// �Է��� ������ ����
	ApplyData();

	// �� ȭ�� ����
	CBOVolumeDlg dlg(this, m_pDM_Book);
	dlg.SetOpenMode(GetOpenMode());
	if (m_bDupAddMode)
	{
		dlg.SetDupIdxKey(m_strDupIndexKey);
	}
	dlg.DoModal();

	ApplyCalculation();
	ApplyRegNoToMarc();
	
	//�ܱ��� ��� ��ǥå�� ������ �������� �����Ͽ����Ѵ�.
	Display();
	m_bViewVolInfo = TRUE;
}

VOID CBOSpeciesDlg::OnbDUPSEARCH() 
{
	CString strSearchCount;
	SearchDuplicateDataByTitle(_T("����������"), strSearchCount);
	if(strSearchCount.IsEmpty() || strSearchCount == _T("0"))
	{
		ESLAfxMessageBox(_T("���������� ����� �����ϴ�."));
		return;
	}
	CDuplicateSearch_Interface impl(this);
	impl.ShowTitleDuplicateSearchDlg(_T("�����ڵ���������"), m_strDupIndexKey);
	if (!m_strDupIndexKey.IsEmpty())
		MakeDupAddMode();
}

VOID CBOSpeciesDlg::OnbEXTRAISBN() 
{
	CString strAdditionISBN = m_pDM_Index->GetCellData(_T("IBS_ISBN"), 0);
	
	CBOISBNAdditionDlg dlg(this);
	dlg.SetAdditionISBN(strAdditionISBN);
	if (dlg.DoModal() == IDOK)
	{
		m_pDM_Index->SetCellData(_T("IBS_ISBN"), dlg.GetAdditionISBN(), 0);
	}
}

VOID CBOSpeciesDlg::OnbISBNCHECK() 
{
	// ���� üũ
	CString strEaISBNFrom;
	CString strEaISBNTo;
	INT ids = GetControlData(_T("CM_����_��_��������"), _T("IBS_����ISBN"), strEaISBNFrom);
	if (ids >= 0)
	{
		ids = m_pFormApi->CheckISBN(strEaISBNFrom, strEaISBNTo);
		if (ids >= 0)
			SetControlData(_T("CM_����_��_��������"), _T("IBS_����ISBN"), strEaISBNTo);
	}

	// ��Ʈ ü��
	CString strSetISBNFrom;
	CString strSetISBNTo;
	ids = GetControlData(_T("CM_����_��_��������"), _T("IBS_��Ʈ_ISBN1"), strSetISBNFrom);
	if (ids >= 0)
	{
		ids = m_pFormApi->CheckISBN(strSetISBNFrom, strSetISBNTo);
		if (ids >= 0)
			SetControlData(_T("CM_����_��_��������"), _T("IBS_��Ʈ_ISBN1"), strSetISBNTo);
	}
}

VOID CBOSpeciesDlg::OnKillfocuseREGCLASSCODE() 
{
	CString strRegCodeField;
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCodeField);

	CArray<CRegCode, CRegCode> arrRegCode;
	INT ids = m_pFormApi->FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0)
	{
		MessageBox(_T("���� ��ϱ����Դϴ�."));
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
		return;
	}

	INT nRegCodeCount = arrRegCode.GetSize();
	if (nRegCodeCount <= 0) return;

	// ��ϱ��� �ٲ���� üũ
	CString strVol;
	GetControlData(_T("CM_����_��_��������"), _T("IBS_����"), strVol);
	if (m_pFormApi->IsRegCodeModify(&arrRegCode, m_pDM_Book, strVol))
	{
		AfxMessageBox(_T("��ϱ����� ������ �ɼ� �ֽ��ϴ�. å�������� �������ּ���"));
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
		return;			
	}

	m_strRegCodeField = strRegCodeField;

	INT nBookCount = 0;
	for (INT i = 0; i < nRegCodeCount; i++)
	{
		nBookCount += arrRegCode.GetAt(i).m_nCount;
	}

	CString strBookCount;
	strBookCount.Format(_T("%d"), nBookCount);

	SetControlData(_T("CM_����_��_��������"), _T("BP_��å��"), strBookCount);
	m_bViewVolInfo = FALSE;
}

VOID CBOSpeciesDlg::OnSelchangecPRICETYPE() 
{
	ApplyCalculation();
}

VOID CBOSpeciesDlg::OnKillfocusePRICE() 
{
	ApplyCalculation();
}

VOID CBOSpeciesDlg::OnKillfocusePRICEDC() 
{
	ApplyCalculation();
}

VOID CBOSpeciesDlg::OnbAPPLYSERIES() 
{
	if (MessageBox(_T("å�� �������� ����ϴ�. ����Ͻðڽ��ϱ�?"), NULL, MB_YESNO | MB_ICONQUESTION) == IDNO) return;

	// ApplyData
	if (ApplyData() < 0)
	{
		MessageBox(_T("������������ ����"));
		return;
	}

	// ��� å�� 1�� 1å���� �����.
	CString strVol;
	m_pFormApi->ApplyCompleteWorks(m_pDM_Book, strVol);

	// ���ΰ� ȭ�鿡 ����
	m_pDM_Index->SetCellData(_T("BB_�����"), strVol, 0);
	SetControlData(_T("CM_����_��_��������"), _T("IBS_����"), strVol);

	MessageBox(_T("å�� �������� ��������ϴ�."));

	Display();
	m_bViewVolInfo = TRUE;
}

UINT CBOSpeciesDlg::OnNoteClicked(LPARAM lParam, WPARAM wParam)
{
	// ���� ���� ����
	//OnbDUPSEARCH();
	CDuplicateSearch_Interface impl(this);
	impl.ShowTitleDuplicateSearchDlg(_T("�����ڵ���������"), m_strDupIndexKey);
	if(!m_strDupIndexKey.IsEmpty())
		MakeDupAddMode();

	return 0;
}

VOID CBOSpeciesDlg::OnKillfocuseTITLE() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_eTITLE);
	if (pEdit == NULL) return;

	// ���� �¸��̸� üũ���� �ʴ´�.
	if (pEdit->GetStyle() & ES_READONLY) return;

	SetTimer(0, 1000, NULL);	
}

VOID CBOSpeciesDlg::OnTimer(UINT nIDEvent) 
{
	// ���� �ڵ���������
	if (nIDEvent == 0)
	{
		KillTimer(0);

		if (m_bAutoDupSearch == FALSE) return;

		CString strSearchCount;

		SearchDuplicateDataByTitle(_T("�����ڵ���������"), strSearchCount);

		if (strSearchCount.IsEmpty() || strSearchCount == _T("0"))	return;
		
		if (m_pNoteCtrl != NULL)
		{
			delete m_pNoteCtrl;
		}
		
		m_pNoteCtrl = new CNoteCtrl;
		CString strNote;
		strNote.Format(_T("%s���� ������ �߰ߵǾ����ϴ�."), strSearchCount);
		m_pNoteCtrl->SetNote(strNote);

		CRect rcButton;
		GetDlgItem(IDC_bDUP_SEARCH)->GetWindowRect(rcButton);
		ScreenToClient(rcButton);
				
		rcButton.top += 50;
		rcButton.bottom += 60;

		rcButton.right += 60;

		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		point.y += 5;
		
		m_pNoteCtrl->Create(point, 200, 50, this);
		m_pNoteCtrl->ShowWindow(SW_SHOW);
	}
	
	CDialog::OnTimer(nIDEvent);
}


VOID CBOSpeciesDlg::OnSelchangecFORMCLASS() 
{
	CString strOriginFormClass;
	CString strFormClass;

	m_pDM_Index->GetCellData(_T("IBS_���ı���_�ڵ�"), 0, strOriginFormClass);
	GetControlData(_T("CM_����_��_��������"), _T("IBS_���ı���_�ڵ�"), strFormClass);

	if (strOriginFormClass == strFormClass) return;

	if (strFormClass == _T(""))
	{
		MessageBox(_T("���ı����� �����ϼž� �˴ϴ�."));
		GetDlgItem(IDC_cFORM_CLASS)->SetFocus();
		return;
	}
	else if (strFormClass == _T("BK"))
	{
		m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
		MessageBox(_T("������ �ٲ�����ϴ�."));
	}
	else if (strFormClass == _T("VM") ||
			 strFormClass == _T("CF") ||
			 strFormClass == _T("MP") ||
			 strFormClass == _T("MU")
			 )
	{
		m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
		MessageBox(_T("�񵵼��� �ٲ�����ϴ�."));
	}
	
	m_pDM_Index->SetCellData(_T("IBS_���ı���_�ڵ�"), strFormClass, 0);

}

VOID CBOSpeciesDlg::SearchDuplicateDataByTitle(CString strSearchType, CString &strSearchCount)
{
	CString strTitle;

	INT ids = GetControlData(_T("CM_����_��_��������"), _T("IBS_��ǥ��"), strTitle);
	if (ids < 0) return ;

	if (strTitle.IsEmpty()) return ;
	INT nFind = strTitle.Find(_T("="));
	if (nFind >= 0)
	{
		strTitle = strTitle.Mid(0, nFind);
		nFind = strTitle.Find(_T(":"));
		if(nFind >= 0)
			strTitle = strTitle.Mid(0, nFind);
	}

	if (strTitle.IsEmpty()) return ;

	// ��������϶� �ڽ��� ��Ű�� �����´�.
	CString strSpeciesKey;
	if (GetOpenMode() == -1100 && m_nParentSelectedRow >= 0)
		m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, m_nParentSelectedRow, strSpeciesKey);

	CDuplicateSearch_Interface impl(this);
	ids = impl.ShowOnlyTitleDuplicateSearch(strSearchType, strTitle, strSearchCount, strSpeciesKey);
	if (ids < 0) return;
}

VOID CBOSpeciesDlg::OnbtnSEARCHDONATOR() 
{
	// TODO: Add your control notification handler code here
	//������ �˻� ȭ�� ���
}

VOID CBOSpeciesDlg::OnbtnGROUPINSERT() 
{
	// TODO: Add your control notification handler code here
	//Group�߰� ȭ���� ����Ѵ�.
}

VOID CBOSpeciesDlg::OnKillfocusedtACQYEAR() 
{
	// TODO: Add your control notification handler code here
	//���Գ⵵ KillFoucs �̺�Ʈ ���ν���
	//���Գ⵵�� �ٲ� ���� Group����Ʈ�� �籸���ؾ��Ѵ�.
}
