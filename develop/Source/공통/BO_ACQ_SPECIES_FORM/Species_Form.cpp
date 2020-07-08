// Species_Form.cpp : implementation file
//
#include "stdafx.h"
#include "GroupInfoDlg.h"
#include "BasicInfoDlg.h"
#include "DivisionNoInfoDlg.h"
#include "DonateInfoDlg.h"
#include "PurchaseInfoDlg.h"
#include "Species_Form.h"
#include "BOAppendixDlg.h"
#include "BOMarcEditDlg.h"
#include "Light_DivisionNoInfoDlg.h"
#include "..\BO_VOLUME_FORM\BOVolumeDlg.h"
#include "..\BO_VOLUME_FORM\BOISBNAdditionDlg.h"
#include "..\BO_MARC_IMPORT\MARC_IMPORT.h"
#include "..\..\�������\Light_GatherDataSearch\UC_MARC_IMPORT.h"
#include "..\..\�������\MOKCHA_CHOROK_API\BO_INERT_TABLE.h"
#include "..\Duplicate_Data_Search\DuplicateSearch_Interface.h"
#include "../K2UP_Common/MulitComboBoxManager.h"
#include "../COVERMANAGER/COVER_MAIN.h"
// 2004.12.29 ������ ��ȸ
#include "../../����/����/BO_ACQ_LOAN_USER_SEARCH_MGR/BO_ACQ_LOAN_USER_SEARCH_DLG.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecies_Form dialog
// ������
CSpecies_Form::CSpecies_Form(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSpecies_Form::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpecies_Form)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nParentSelectedRow = -1;	
	m_nOpType = OPERATION_TYPE_INVALID;
	m_nSpeciesType = SPECIES_TYPE_INVALID;
	m_nAcqType = ACQ_TYPE_INVALID;
	m_nParentType	= SPECIES;
	
	m_pDM_Index = NULL;
	m_pDM_Species = NULL; 
	m_pDM_Book = NULL;
	m_pDM_Accu = NULL;
	m_pDM_Appendix = NULL;
	m_pDM_Parent		= NULL;
	m_pDM_Parent_Book	= NULL; 
	m_pCM_Index = NULL;
	m_pCM_Accu = NULL;
	m_pGrid_Parent		= NULL;

	m_pGroupCollection	= NULL;	
	
	m_pArrDonateRegYN	= NULL;

	m_pNoteCtrl		= NULL;

	m_pHeadDlg		= NULL;
	m_pTailDlg		= NULL;

	m_pFormApi		= NULL; 
	m_pBOSpeciesApi = NULL;

	m_bShowCode = FALSE;
	m_bDupAddMode = FALSE;
	m_bAutoDupSearch = FALSE;
	m_bControlDisplay = FALSE;
	m_bViewVolInfo = FALSE;
	m_bAcqTypeChange = FALSE;
	m_bInsertAC = FALSE;
	m_bInsertData = FALSE;
	m_bGridSelectYN = FALSE;
	m_bShowEBookInfo = FALSE;
	m_bDupAddYN = FALSE;
	m_bCreateGroupCollection = FALSE;

	m_strIs5163				= _T("");
	m_strIs64				= _T("");
	m_strIs90				= _T("");
	m_strWorkingStatus		= _T("");	
	m_strLastErrMsg			= _T("");	
	m_strSpeciesKeyAlias	= _T("");
	m_strAccuKeyAlias		= _T("");	
	m_strRegCodeField		= _T("");	
	m_strDupIndexKey		= _T("");
	m_strPriorityCatalogYN	= _T("N");	// �켱�����ڷ� �������� default
	m_strUseCodeYN			= _T("N");  // ����� �����ڵ� �������� default
	m_strUseCoverYN			= _T("N");	// ǥ�� �߰� ��� �������� default
	m_strAcqAppendixYN		= _T("N");  // �����ܰ迡���� �η����� ��ư ���̴� ��� 	
	m_strDefaultValueFilePath = _T("..\\cfg\\species_form.cfg");
	m_szSpeciesKey[0] = '\0';

	m_arrIgnoreCTRLAlias.RemoveAll();

	//2007.08.10 ADD BY PJW : ��û�ð� �߰�
	m_hApplicantTime = NULL;

	//20070821 ADD PJW : ���������� üũ �ϱ����� ������ FALSE�� ����
	m_bNewSaveFlag = FALSE;

	// 2008.06.08 ADD BY PDJ : �����ʷ��� ���� RichEdit20W Init.
	AfxInitRichEdit();


}
// �Ҹ���
CSpecies_Form::~CSpecies_Form()
{
	//E-BOOK�ӽ����� ������
	m_pFormApi->DeleteEBookTempFolder();

	if( m_pHeadDlg != NULL )
	{
		delete m_pHeadDlg;
		m_pHeadDlg = NULL;
	}
	if( m_pTailDlg != NULL )
	{
		delete m_pTailDlg;
		m_pTailDlg = NULL;
	}
	if( m_pFormApi != NULL )
	{
		delete m_pFormApi;
		m_pFormApi = NULL;
	}
	if( m_pBOSpeciesApi !=NULL )
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}
	if( m_bCreateGroupCollection == TRUE )
	{
		if( m_pGroupCollection != NULL )
		{
			delete m_pGroupCollection ;
			m_pGroupCollection = NULL;
		}
	}
	// 16/10/12 ������ : ����>�������>�����ڷ���� �޴��� �Է�â ���� �� ����� �޸𸮴��� ���� ����
//*/ ADD ---------------------------------------------------------------------------
	if(m_hApplicantTime)
	{
		delete m_hApplicantTime;
		m_hApplicantTime = NULL;
	}
//*/ END ---------------------------------------------------------------------------
}


VOID CSpecies_Form::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpecies_Form)
	DDX_Control(pDX, IDC_cmbAPPLICANT, m_cmbApplicant);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpecies_Form, CDialog)
	//{{AFX_MSG_MAP(CSpecies_Form)
	ON_BN_CLICKED(IDC_btnBSFORM_SHOW_HIDE_CODE, OnbtnBSFORMSHOWHIDECODE)
	ON_EN_KILLFOCUS(IDC_eTITLE, OnKillfocuseTITLE)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_bMARC_EDIT, OnbMARCEDIT)
	ON_BN_CLICKED(IDC_bMARC_IMPORT, OnbMARCIMPORT)
	ON_BN_CLICKED(IDC_bKOLIS_NET, OnbKOLISNET)
	ON_BN_CLICKED(IDC_bAPPENDIX, OnbAPPENDIX)
	ON_BN_CLICKED(IDC_bCLEARSCREEN, OnbCLEARSCREEN)
	ON_EN_KILLFOCUS(IDC_eREG_CLASS_CODE, OnKillfocuseREGCLASSCODE)
	ON_BN_CLICKED(IDC_bPREV_SPECIES, OnbPREVSPECIES)
	ON_BN_CLICKED(IDC_bNEXT_SPECIES, OnbNEXTSPECIES)
	ON_BN_CLICKED(IDC_bSAVEAS, OnbSAVEAS)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bEXTRA_ISBN, OnbEXTRAISBN)
	ON_BN_CLICKED(IDC_bISBN_CHECK, OnbISBNCHECK)
	ON_BN_CLICKED(IDC_bDEFAULT_CONFIG, OnbDEFAULTCONFIG)
	ON_BN_CLICKED(IDC_bDUP_SEARCH, OnbDUPSEARCH)
	ON_BN_CLICKED(IDC_bAPPLY_SERIES, OnbAPPLYSERIES)
	ON_BN_CLICKED(IDC_bVOL_INFO, OnbVOLINFO)
	ON_CBN_SELCHANGE(IDC_cCLASIFY_CLASS, OnSelchangecCLASIFYCLASS)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_CBN_SELCHANGE(IDC_cFORM_CLASS, OnSelchangecFORMCLASS)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_cUSE_OBJECT_CLASS, OnSelchangecUSEOBJECTCLASS)
	ON_BN_CLICKED(IDC_btnPREV_CONTENT, OnbtnPREVCONTENT)
	ON_CBN_SELCHANGE(IDC_cWORK_CODE, OnSelchangecWORKCODE)
	ON_CBN_SELCHANGE(IDC_cMEDIA_CLASS, OnSelchangecMEDIACLASS)
	ON_BN_CLICKED(IDC_btnFIND_FILE1, OnbtnFINDFILE1)
	ON_BN_CLICKED(IDC_btnFIND_FILE3, OnbtnFINDFILE3)
	ON_BN_CLICKED(IDC_btnFIND_FILE2, OnbtnFINDFILE2)
	ON_BN_CLICKED(IDC_btnEBOOK_VIEW, OnbtnEBOOKVIEW)
	ON_EN_KILLFOCUS(IDC_edtAPPENDIX, OnKillfocusedtAPPENDIX)
	ON_BN_CLICKED(IDC_btnCover, OnbtnCover)
	ON_BN_CLICKED(IDC_btnSearchUser, OnbtnSearchUser)
	ON_CBN_SELCHANGE(IDC_cmbAPPLICANT, OnSelchangecmbAPPLICANT)	
	ON_CBN_SELCHANGE(IDC_cAPPLICAND_KIND_CLASS, OnSelchangecAPPLICANDKINDCLASS)
	ON_CBN_SELCHANGE(IDC_cAPPLICAND_KIND_COURCE, OnSelchangecAPPLICANDKINDCOURCE)
	ON_CBN_SELCHANGE(IDC_cAPPLICAND_KIND_DEPT, OnSelchangecAPPLICANDKINDDEPT)
	ON_EN_SETFOCUS(IDC_edtSERIES_VOL, OnSetfocusedtSERIESVOL)
	ON_EN_SETFOCUS(IDC_edtPHYSICAL_CHARACTER, OnSetfocusedtPHYSICALCHARACTER)
	ON_EN_SETFOCUS(IDC_edtSIZE, OnSetfocusedtSIZE)
	ON_EN_SETFOCUS(IDC_eEDITION_STMT, OnSetfocuseEDITIONSTMT)
	ON_EN_SETFOCUS(IDC_eSERIES_TITLE, OnSetfocuseSERIESTITLE)
	ON_EN_SETFOCUS(IDC_eREG_CLASS_CODE, OnSetfocuseREGCLASSCODE)
	ON_EN_SETFOCUS(IDC_eTITLE, OnSetfocuseTITLE)
	ON_EN_SETFOCUS(IDC_eVOL, OnSetfocuseVOL)
	ON_EN_SETFOCUS(IDC_ePUBLISHER, OnSetfocusePUBLISHER)
	ON_EN_SETFOCUS(IDC_ePUBLISH_PLACE, OnSetfocusePUBLISHPLACE)
	ON_EN_SETFOCUS(IDC_edtAPPENDIX, OnSetfocusedtAPPENDIX)
	ON_EN_SETFOCUS(IDC_eDISCRIPTION_SMALL, OnSetfocuseDISCRIPTIONSMALL)
	ON_EN_SETFOCUS(IDC_eDISCRIPTION, OnSetfocuseDISCRIPTION)
	ON_EN_SETFOCUS(IDC_eAUTHOR1, OnSetfocuseAUTHOR1)
	ON_EN_SETFOCUS(IDC_edtBSFORM_REQUESTOR, OnSetfocusedtBSFORMREQUESTOR)
	ON_EN_KILLFOCUS(IDC_edtBSFORM_REQUESTOR, OnKillfocusedtBSFORMREQUESTOR)
	ON_MESSAGE(ACQ_TYPE_CHANGE, OnAcqTypeChange)
	ON_MESSAGE(WORKING_STATUS_CHANGE, OnWorkingStatusChange)
	ON_MESSAGE(PURCHASE_SEQ_NO_CHANGE, OnPurchaseSeqNoChange)
	ON_MESSAGE(NOTE_CLICKED, OnNoteClicked)
	ON_MESSAGE(SET_TAB_ORDER, OnSetTabOrder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecies_Form message handlers
// �ʱ�ȭ
//
// [ �̴Ͻ��� ]
// 1. SM�� ����(K2UPGRADE_����_��) �Ѵ�.
// 2. �ʼ� �Է��׸� ���� ��Ʈ�� ������ �����Ѵ�.
// 3. ������ rect�� ��������� �����Ѵ�.
// 4. 5163 DB���� ����������ڵ� ��뿩��, ǥ������ ��뿩�� ���� �����´�.
// 5. �޺��ڽ��� �����Ѵ�.
// 6. [API/CM/DM/HeadDlg/TailDlg/Ctrl/������ �˸��ƽ���]�� �ʱ�ȭ �� �����Ѵ�.
// 7. EBook�������� �� �Է�â������ ���ҽ����� ���� �� �ʱ�ȭ �Ѵ�.[ShowCodeResource(), ShowCodeResourceByMediaCode() ȣ��]
// 8. �������� ǥ��
// 9. �����÷��׿� ���� ��û���� �޺��ڽ��� Ȱ��ȭ��Ų��.
//
/////////////////////////////////////////////////////////////////////////////
BOOL CSpecies_Form::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	//=======================================================================
	// ��������
	//=======================================================================
	CESL_DataMgr* TmpDM = NULL;

	//=======================================================================
	// 1. SM�� ����(K2UPGRADE_����_��) �Ѵ�.
	//=======================================================================
	if( InitESL_Mgr(_T("K2UPGRADE_����_��")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	
	//=======================================================================
	// 2. �ʼ� �Է��׸� ���� ��Ʈ�� ������ �����Ѵ�.
	//=======================================================================	

	AddColorControl(IDC_eTITLE, _T(""), _T(""), 'E');
	AddColorControl(IDC_eREG_CLASS_CODE, _T(""), _T(""), 'E');
	AddColorControl(IDC_cFORM_CLASS, _T("CMUP_����_��_��������"), _T("IBS_���ı���_�ڵ�"), 'M');
	
	//AddColorControl(IDC_eAUTHOR1, _T(""), _T(""), 'E');
	
	//=======================================================================
	// 3. ������ rect�� ��������� �����Ѵ�.
	//=======================================================================	
	GetWindowRect(m_rcWindow);
	
	//=======================================================================
	// 4. 5163 DB���� ����������ڵ� ��뿩��, ǥ������ ��뿩�� ���� �����´�.
	// 2004.11.04
	//=======================================================================	
	// BEGIN:
	TmpDM = FindDM(_T("DMUP_����_��_������"));
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USERDEFINECODE'"), m_strUseCodeYN);
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USECOVERIMG'"), m_strUseCoverYN);
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), m_strPriorityCatalogYN);
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'ACQAPPENDIXINSERT'"), m_strAcqAppendixYN);
	// 2005-01-28
	GetManageValue( _T("��Ÿ"), _T("����"), _T("5163"), _T("5163"), m_strIs5163 );
	GetManageValue( _T("��Ÿ"), _T("����"), _T("����"), _T("����"), m_strIs64 );	
	GetManageValue( _T("��Ÿ"), _T("����"), _T("�����"), _T("�����"), m_strIs90 );	
	// END:
	

	//=======================================================================
	// 5. �޺��ڽ��� �����Ѵ�.
	//=======================================================================
	m_cmbApplicant.ResetContent();
	m_cmbApplicant.InsertString( 0, _T("ȸ��") );
	m_cmbApplicant.InsertString( 1, _T("�μ�") );
	m_cmbApplicant.InsertString( 2, _T("�а�") );
	m_cmbApplicant.InsertString( 3, _T("��ó") );
	
	if		( _T("Y") == m_strIs90 && ACQ_TYPE_INVALID != m_nAcqType )	m_cmbApplicant.SetCurSel(1);
	else if	( _T("Y") == m_strIs64 && ACQ_TYPE_INVALID != m_nAcqType )	m_cmbApplicant.SetCurSel(3);
	else					m_cmbApplicant.SetCurSel(0);

	//20070810 ADD PJW : �ð� �ʱ� ����Ʈ ����
	m_hApplicantTime = new CHourEditControl;
	m_hApplicantTime->SubclassWindow( GetDlgItem(IDC_edtBSFORM_REQUEST_HOUR)->GetSafeHwnd() );
	m_hApplicantTime->InitEdit();




	//=======================================================================
	// 6. [API/CM/DM/HeadDlg/TailDlg/Ctrl/������ �˸��ƽ���]�� �ʱ�ȭ �� �����Ѵ�.
	//=======================================================================
	if( Init() < 0 )
	{
		ESLAfxMessageBox(_T("��-�Է�/����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	//=======================================
	//20080612 ADD BY PJW : ���Խ�û�� ���翩�ο� ���� ��¥,�ð� ĿƮ�� �缳���Ѵ�.
	OnKillfocusedtBSFORMREQUESTOR(); 
	//=======================================

	//=======================================================================
	// 7. EBook�������� �� �Է�â������ ���ҽ����� ���� �� �ʱ�ȭ �Ѵ�.[ShowCodeResource(), ShowCodeResourceByMediaCode() ȣ��]
	//=======================================================================
	InitEBookCtrl();	

	//=======================================================================
	// 8. �������� ǥ��
	//=======================================================================
	OnSelchangecCLASIFYCLASS();

	//=======================================================================
	// 9. ������ ù ��Ŀ���� �ϰ� _T("�ѱ�")���� ����
	//=======================================================================	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_eTITLE)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_eTITLE)->m_hWnd,hIME); 
    }	

	//20070522 ADD PJW : ���� ��Ŀ�� 
	SetTimer(3,10,NULL);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  
}

VOID CSpecies_Form::MoveTailDlg(INT nCodeResourcesHeight)
{
	//INT nCodeResourcesHeight = 121;
	nCodeResourcesHeight -= 5;
	CRect rcResource;
	m_pTailDlg->GetWindowRect(rcResource);
	ScreenToClient(rcResource);
	if(m_bShowCode)
	{
		rcResource.top += (nCodeResourcesHeight);
		rcResource.bottom += (nCodeResourcesHeight);
		m_pTailDlg->MoveWindow(rcResource);
	}
	else
	{
		rcResource.top -= (nCodeResourcesHeight);
		rcResource.bottom -= (nCodeResourcesHeight);
		m_pTailDlg->MoveWindow(rcResource);
	}
}

VOID CSpecies_Form::SetTailInfoDlg()
{
	if(m_pTailDlg)
	{
		delete m_pTailDlg;
		m_pTailDlg = NULL;
	}
	if(m_nAcqType == ACQ_TYPE_INVALID)
		m_pTailDlg = new CBasicInfoDlg(this);
	else if(m_nAcqType == PURCHASE)
		m_pTailDlg = new CPurchaseInfoDlg(this);
	else if(m_nAcqType == DONATE)
		m_pTailDlg = new CDonateInfoDlg(this);
	m_pTailDlg->CreateInfoDlg(this);
	
	MoveTailDlg(121);
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - Head/Tailâ�� �����ϰ� ����â�� ��ġ�� ���缭 ���δ�.
//
// [ �̴Ͻ��� ]
// 1. ����â�� Head â�� Tail â�� �ִٸ� �Ҹ��Ѵ�.
// 2. m_nAcqType(�Լ�) Ÿ�Կ� ���� Head â�� Tailâ�� �����Ѵ�.
// 3. Head â�� ������ �����Ѵ�.
// 4. Tail â�� ������ �����Ѵ�.
// 5. ������ Head/Tail â�� �����Ͽ� ���� â�� ���δ�.
// 6. Head/Tailâ �� ���� â�� ��Ʈ�� ���Ͽ� Tailâ�� ��ġ�� �����.
//
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::SetHeadTailInfoDlg()
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT nOffSet = 0;
	BOOL bShowEbookInfo;
	CString strValue;
	CRect rectHead, rectTail, rectParent;

	//=======================================================================
	// 1. ����â�� Head â�� Tail â�� �ִٸ� �Ҹ��Ѵ�.
	//=======================================================================
	if( m_pHeadDlg != NULL )
	{
		m_pHeadDlg->ShowWindow(SW_HIDE);
		delete m_pHeadDlg;
		m_pHeadDlg = NULL;
	}
	
	if( m_pTailDlg != NULL )
	{
		m_pTailDlg->ShowWindow(SW_HIDE);
		delete m_pTailDlg;
		m_pTailDlg = NULL;
	}
	

	//=======================================================================
	// 2. m_nAcqType(�Լ�) Ÿ�Կ� ���� Head â�� Tailâ�� �����Ѵ�.
	//=======================================================================
	switch( m_nAcqType )
	{
	case ACQ_TYPE_INVALID :
	{
		m_pHeadDlg = new CGroupInfoDlg(this);
		m_pTailDlg = new CBasicInfoDlg(this);
	}
		break;

	case PURCHASE :
	{
		if( m_nSpeciesType == BO_PURCHASE && m_pInfo->GetKIIUPClientVersion() == 'L' )
		{
			m_pHeadDlg = new CLight_DivisionNoInfoDlg(this);
		}
		else
		{
			m_pHeadDlg = new CDivisionNoInfoDlg(this);
		}
			m_pTailDlg = new CPurchaseInfoDlg(this);
	}
		break;

	case DONATE :
	{
		m_pHeadDlg = new CDivisionNoInfoDlg(this);
		m_pTailDlg = new CDonateInfoDlg(this);

		m_pHeadDlg->SetWorkingStatus(m_strWorkingStatus);
	}
		break;

	case OTHER_ACQ_TYPE :
	{
		m_pHeadDlg = new CDivisionNoInfoDlg(this);
		m_pTailDlg = new CBasicInfoDlg(this);
	}
		break;

	default:
		break;
	}
	
	//=======================================================================
	// 3. Head â�� ������ �����Ѵ�.
	//=======================================================================
	m_pHeadDlg->SetGroupCollection(m_pGroupCollection);	// �׷�����
	m_pHeadDlg->SetAcqType(m_nAcqType);					// �Լ�����
	m_pHeadDlg->SetAppendexDM(m_pDM_Appendix);			// �η�DM����
	
	m_pHeadDlg->SetFormApi(m_pFormApi);					// ��API
	m_pHeadDlg->SetSpeciesApi(m_pBOSpeciesApi);			// ��API
	m_pHeadDlg->SetSpeciesType(m_nSpeciesType);			// ��Ÿ��
	m_pHeadDlg->SetOpType(m_nOpType);					// ����Ÿ��
	m_pHeadDlg->SetBookDM(m_pDM_Book);					// å DM		
	m_pHeadDlg->SetDM(m_pDM_Accu);						// ����(����)���� DM

	//=======================================================================
	// 4. Tail â�� ������ �����Ѵ�.
	//=======================================================================
	m_pTailDlg->SetFormApi(m_pFormApi);					// ��API
	m_pTailDlg->SetSpeciesApi(m_pBOSpeciesApi);			// ��API		
	m_pTailDlg->SetSpeciesType(m_nSpeciesType);			// ��Ÿ��
	m_pTailDlg->SetOpType(m_nOpType);					// ����Ÿ��
	m_pTailDlg->SetBookDM(m_pDM_Book);					// å DM
	m_pTailDlg->SetDM(m_pDM_Accu);						// ����(����)���� DM


	//=======================================================================
	// 5. ������ Head/Tail â�� �����Ͽ� ���� â�� ���δ�.
	//=======================================================================
	m_pHeadDlg->CreateInfoDlg(this);
	m_pHeadDlg->ShowWindow(SW_SHOW);
	m_pTailDlg->CreateInfoDlg(this);
	m_pTailDlg->ShowWindow(SW_SHOW);

	//=======================================================================
	// 6. Head/Tailâ �� ���� â�� ��Ʈ�� ���Ͽ� Tailâ�� ��ġ�� �����.
	//=======================================================================
	m_pHeadDlg->GetWindowRect(&rectHead);
	m_pTailDlg->GetWindowRect(&rectTail);
	this->GetWindowRect(&rectParent);
	
	if( m_rcWindow == rectParent )	bShowEbookInfo = TRUE;
	else bShowEbookInfo = FALSE;

	ScreenToClient(rectTail);
	ScreenToClient(rectParent);
	
	//249
	if( bShowEbookInfo == TRUE )	nOffSet = rectParent.bottom - 290;
	else							nOffSet = rectParent.bottom - 221; //nOffSet = rectParent.bottom - 184;

	rectTail.top += nOffSet;
	rectTail.bottom += nOffSet;
	rectTail.left = 0;

	m_pTailDlg->MoveWindow(rectTail);
	m_pTailDlg->ShowWindow(SW_SHOW);
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��Ÿ�Կ����� ���� �Է�â ���� ������ �۾����¸� �����Ѵ�.
//
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::SetWorkingStatus()
{
	CEdit* pCtrl = (CEdit*)GetDlgItem(IDC_edtWORKING_STATUS);
	switch( m_nSpeciesType )
	{
	case BO_BASIC:
		{
		m_strWorkingStatus = _T("BOT111O");
			pCtrl->SetWindowText(_T("�����ڷ�"));
		//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("�����ڷ�"));
		}
		break;

	case BO_IMPORT:
		{
		m_strWorkingStatus = _T("BOT112O");
			pCtrl->SetWindowText(_T("�����ڷ�"));
		//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("�����ڷ�"));
		}
		break;

	case BO_PURCHASE:
		{
		m_strWorkingStatus = _T("BOA111N");
			pCtrl->SetWindowText(_T("���Դ���ڷ�"));
		//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("���Դ���ڷ�"));
		}
		break;

	case BO_ORDER:
		{
		m_strWorkingStatus = _T("BOA112N");
			pCtrl->SetWindowText(_T("�ֹ��ڷ�"));
		//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("�ֹ��ڷ�"));
		}
		break;

	case BO_MISSING:
		{
		m_strWorkingStatus = _T("BOA113O");
			pCtrl->SetWindowText(_T("�̳��ڷ�"));
		//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("�̳��ڷ�"));
		}
		break;

	case BO_REGOBJ:
		{
		if(m_nAcqType == PURCHASE)
		{
			m_strWorkingStatus = _T("BOA114O");
				pCtrl->SetWindowText(_T("���Ե���ΰ����ڷ�"));
			//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("���Ե���ΰ����ڷ�"));
		}
		else if(m_nAcqType == DONATE)
		{
			m_strWorkingStatus = _T("BOA211O");
				pCtrl->SetWindowText(_T("��������ΰ����ڷ�"));
			//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("��������ΰ����ڷ�"));
			}
		}
		break;

	case BO_DONATE:
		{
		m_strWorkingStatus = _T("BOA211O");
			pCtrl->SetWindowText(_T("���������ڷ�"));
		//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("���������ڷ�"));
		}
		break;

	case BO_NONDONATE:
		{
		m_strWorkingStatus = _T("BOA212O");
			pCtrl->SetWindowText(_T("�����δ��ڷ�"));
		//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("�����δ��ڷ�"));
		}
		break;

	case BO_REG_PURCHASE:
		{
		// 2004.09.07 ����ڷ��� ��쿡�� ����ΰ��ڷ�� ǥ�õ�. �ּ�ó��
		//m_strWorkingStatus = _T("BOR111N");

		// 2004.09.07 ��� ���� �ּ�ó�� �Ǿ� �־���. 
		// m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("����ΰ��ڷ�"));
		
		m_strWorkingStatus = m_pDM_Parent->GetCellData(_T("BB_�ڷ����_�ڵ�"), m_nParentSelectedRow);
	
		// 2004.09.07 �ּ�ó��
		// pCtrl->SetWindowText(_T("����ΰ��ڷ�"));
			if( m_strWorkingStatus.IsEmpty() == TRUE )
		{
			m_strWorkingStatus = _T("BOR111N");
			pCtrl->SetWindowText(_T("����ΰ��ڷ�"));
		}
		else
			{
			pCtrl->SetWindowText(m_pDM_Parent->GetCellData(_T("BB_�ڷ����_����"), m_nParentSelectedRow));
			}
		}
		break;

	case BO_REG_DONATE:
		{
		// 2004.09.07 ����ڷ��� ��쿡�� ����ΰ��ڷ�� ǥ�õ�. �ּ�ó��
		//m_strWorkingStatus = _T("BOR111N");

		// 2004.09.07 ��� ���� �ּ�ó�� �Ǿ� �־���. 
		//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("����ΰ��ڷ�"));

		m_strWorkingStatus = m_pDM_Parent->GetCellData(_T("BB_�ڷ����_�ڵ�"), m_nParentSelectedRow);

		// 2004.09.07 �ּ�ó��
		//pCtrl->SetWindowText(_T("����ΰ��ڷ�"));
			if( m_strWorkingStatus.IsEmpty() == TRUE )
		{
			m_strWorkingStatus = _T("BOR111N");
			pCtrl->SetWindowText(_T("����ΰ��ڷ�"));
		}
		else
			{
			pCtrl->SetWindowText(m_pDM_Parent->GetCellData(_T("BB_�ڷ����_����"), m_nParentSelectedRow));
			}
		}
		break;

	case BO_REG_INVALID:
		{
		// 2004.09.07 ����ڷ��� ��쿡�� ����ΰ��ڷ�� ǥ�õ�. �ּ�ó��
		//m_strWorkingStatus = _T("BOR111N");

		// 2004.09.07 ��� ���� �ּ�ó�� �Ǿ� �־���. 
		//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("����ΰ��ڷ�"));

		m_strWorkingStatus = m_pDM_Parent->GetCellData(_T("BB_�ڷ����_�ڵ�"), m_nParentSelectedRow);
		
		// 2004.09.07 �ּ�ó��
		//pCtrl->SetWindowText(_T("����ΰ��ڷ�"));
			if( m_strWorkingStatus.IsEmpty() == TRUE )
		{
			m_strWorkingStatus = _T("BOR111N");
			pCtrl->SetWindowText(_T("����ΰ��ڷ�"));
		}
		else
			pCtrl->SetWindowText(m_pDM_Parent->GetCellData(_T("BB_�ڷ����_����"), m_nParentSelectedRow));
		}
		break;

	default :
		{
		m_strWorkingStatus = _T("BOT111O");
		pCtrl->SetWindowText(_T("�����ڷ�"));
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ������� DM�� �����ϰ� ������ �ʱ�ȭ�Ѵ�.
//
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::SetDataManagers()
{
	m_pDM_Index		= FindDM(_T("DMUP_����_��_��������"));
	m_pDM_Species	= FindDM(_T("DMUP_����_��_������"));
	m_pDM_Book		= FindDM(_T("DMUP_����_��_å����"));
	m_pDM_Appendix  = FindDM(_T("DMUP_����_��_�η�����"));
	
	switch( m_nAcqType )
	{
	case PURCHASE:	m_pDM_Accu = FindDM(_T("DMUP_����_��_��������"));
		break;
	case DONATE:	m_pDM_Accu = FindDM(_T("DMUP_����_��_��������"));
		break;
	default:		m_pDM_Accu = FindDM(_T("DMUP_����_��_�⺻����"));
		break;
	}

	m_pDM_Accu->FreeData();
	m_pDM_Index->FreeData();
	m_pDM_Species->FreeData();
	m_pDM_Book->FreeData();
	m_pDM_Appendix->FreeData();
}

VOID CSpecies_Form::SetAccuDataManager()
{
	m_pTailDlg->ApplyData();
	
	CESL_DataMgr * m_pTmpDM = NULL;

	if(m_nAcqType == PURCHASE)
	{
		m_pTmpDM = FindDM(_T("DMUP_����_��_��������"));
		m_pTmpDM->InsertRow(-1);
		CString strTmpData;
		m_pCM_Index->GetControlMgrData(_T("IBS_��������_�ڵ�"), strTmpData);
		m_pTmpDM->SetCellData(_T("BP_������������"), strTmpData, 0);
	}
	else if(m_nAcqType == DONATE)
	{
		m_pTmpDM = FindDM(_T("DMUP_����_��_��������"));
		m_pTmpDM->InsertRow(-1);
	}
	else 
	{
		m_pTmpDM = FindDM(_T("DMUP_����_��_�⺻����"));
		m_pTmpDM->InsertRow(-1);
	}
	
	if(m_pTmpDM == NULL) return;
	CStringArray ArrCopyList;
	ArrCopyList.Add(_T("BP_����"));			ArrCopyList.Add(_T("BP_���ݼ���"));	
	ArrCopyList.Add(_T("BP_����������"));	ArrCopyList.Add(_T("BP_ȭ�󱸺�"));
	
	INT ids = m_pFormApi->CopyDMInArray(m_pDM_Accu, 0, m_pTmpDM, 0, &ArrCopyList);
	if(ids < 0) return ;
	m_pDM_Accu = m_pTmpDM;
}

VOID CSpecies_Form::InitFormApi()
{
	if(m_pFormApi)
	{
		delete m_pFormApi;
		m_pFormApi = NULL;
	}
	m_pFormApi = new CBOFormApi(this);
	// �⺻�� ���������� �ʵ� Alias �����ϱ�
	m_pFormApi->InitDefaultAlias();
	m_pFormApi->AddDefaultAlias(_T("IBS_��ǥ��"));
	m_pFormApi->AddDefaultAlias(_T("IBS_������"));
	m_pFormApi->AddDefaultAlias(_T("IBS_�ǻ���"));
	m_pFormApi->AddDefaultAlias(_T("IBS_�Ѽ���"));
	m_pFormApi->AddDefaultAlias(_T("IBS_�Ѽ�����"));
	m_pFormApi->AddDefaultAlias(_T("IBS_�����"));
	m_pFormApi->AddDefaultAlias(_T("IBS_������"));
	m_pFormApi->AddDefaultAlias(_T("IBS_������"));
	m_pFormApi->AddDefaultAlias(_T("IBS_�����"));
	m_pFormApi->AddDefaultAlias(_T("IBS_�����"));
	m_pFormApi->AddDefaultAlias(_T("IBS_������Ư��"));
	m_pFormApi->AddDefaultAlias(_T("IBS_ũ��"));
	m_pFormApi->AddDefaultAlias(_T("IBS_�����ڷ�"));
	m_pFormApi->AddDefaultAlias(_T("IBS_����ISBN"));
	m_pFormApi->AddDefaultAlias(_T("IBS_����ISBN�ΰ���ȣ"));
	m_pFormApi->AddDefaultAlias(_T("IBS_���ǿ���ISBN"));
	m_pFormApi->AddDefaultAlias(_T("IBS_��Ʈ_ISBN1"));
	m_pFormApi->AddDefaultAlias(_T("IBS_��Ʈ_ISBN2"));
	m_pFormApi->AddDefaultAlias(_T("IBS_��Ʈ_ISBN3"));
	m_pFormApi->AddDefaultAlias(_T("BP_����"));
	m_pFormApi->AddDefaultAlias(_T("BP_���ݼ���"));
	m_pFormApi->AddDefaultAlias(_T("BP_����������"));
	m_pFormApi->AddDefaultAlias(_T("BP_ȭ�󱸺�"));

	// Default IgnoreList �ۼ�
	m_pFormApi->InitDefaultIgnoreAlias();
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��ǥå����"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��Ϲ�ȣ"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_������ȣ"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_�谡��ġ��ȣ"));	
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));	
}

VOID CSpecies_Form::InitSpeciesApi()
{
	// �� API �ʱ�ȭ
	if (m_pBOSpeciesApi) 
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}
	m_pBOSpeciesApi = new CBO_SPECIES(this);
	m_pBOSpeciesApi->InitDataMgr();
	//===================================================
	//2010.05.24 ADD BY PJW : API�� ��Ÿ���� ������ �ش�.
	m_pBOSpeciesApi->SetSpeciesType(m_nSpeciesType);
	//===================================================	
	if (m_pBOSpeciesApi == NULL)
	{
		m_strLastErrMsg = _T("�� API �ʱ�ȭ ����");
		return ;
	}

}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - [API/CM/DM/HeadDlg/TailDlg/Ctrl/������ �˸��ƽ���]�� �ʱ�ȭ �� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ��/�� API �ʱ�ȭ�Ѵ�.
// 2. ��� CM�� �����Ѵ�.
// 3. �ڵ� �������� ��带 �����Ѵ�.
// 4. ����Ÿ�Կ� ���� ���� ������ �����Ѵ�.	
//		[�Է�]
//		1-1. DM���� �� �ʱ�ȭ �Ѵ�.
//		1-2. �ڷ���¸� �����Ѵ�.
//		1-3. �� API�� ��带 �����Ѵ�.
//		1-4. ����, ��, å, �������� DM�� Row�� �ϳ� �߰��Ѵ�.
//		1-5. �Լ������� �����̶��, �������� DM�� BP_�����������п� _T("1")�� �����Ѵ�.
//		1-6. Head/Tail ���̾�α׸� ���� �� �����Ѵ�.
//		1-7. _T("ȭ�������")�� _T("��������") ��ư�� ���̰� �Ѵ�.
//		1-8. �ʱⰪ�� �ҷ��´�.
//		[����]
//		2-1. �θ�׸����� ���õ� �ο츦 �����Ѵ�.
//
//		2-4. �׸��带 ���� ǥ���Ѵ�		2-2. ��Ÿ�Կ����� DM �� �۾����¸� �����Ѵ�. (�Է¿����� 1,2��)
//		2-3. �� API�� ��带 �����Ѵ�..
//		2-5. ��������� ������ �����ο�(m_nParentSelectedRow)�� ��Ű�� ��������Ű�� ������ 
//			 ���DM(����,��,��������,�η�����)�� �����Ѵ�.
//		2-6. Head/Tail ���̾�α׸� ���� �� �����Ѵ�.
//		2-7. ��ũ�κ��� ����DM�� ���� �����Ѵ�.
//		2-8. ����,����, ���������ư�� ���̰� �Ѵ�.
// 5. �켱�����ڷ�� �η����� ��뿩�ο� ���� ��Ÿ���� üũ�Ͽ� �켱�����ڷ�üũ�ڽ��� �η�������ư�� ��Ÿ����.
// 6. ���Ա��п� ���� ���Խ�û��/��û���ڸ� Ȱ��ȭ/��Ȱ��ȭ �Ѵ�.
// 7. ������ȣ �ο����� ����
// 8. FormApi�� �߰�
// 9. �Է½� �⺻�� �� ���������� ����Ǿ� ���� ���ƾ��� Ctrl�˸��ƽ��� �����Ѵ�.
// 10. ��å�� �� �ѱǼ��� ��Ȱ��ȭ ��Ų��.
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::Init()
{
	//=======================================================================
	// ���� ����
	//=======================================================================
	INT ids = -1;
	CString strValue;

	
	//=======================================================================
	// 1. ��/�� API �ʱ�ȭ�Ѵ�.
	//=======================================================================	
	InitSpeciesApi();	//��API�ʱ�ȭ	
	InitFormApi();		//��API�ʱ�ȭ
	

	//=======================================================================
	// 2. ��� CM�� �����Ѵ�.
	//=======================================================================	
	m_pCM_Index	= FindCM(_T("CMUP_����_��_��������"));
	m_pCM_Accu	= FindCM(_T("CMUP_����_��_��������"));
	if( m_pCM_Index == NULL || m_pCM_Accu == NULL )
	{
		m_strLastErrMsg = _T("[CMUP_����_��_��������/����_��_��������]�� ������ �� �����ϴ�.");
		return -1;
	}		
	
	//=======================================================================
	// 3. �ڵ� �������� ��带 �����Ѵ�.
	//=======================================================================	
	GetManageValue(_T("����"), _T("����"), _T("������ȭ�麹������˻�����"), _T(""), strValue);
	if( strValue == _T("Y") )
	{
		m_bDupSearchType = TRUE;
		m_bAutoDupSearch = FALSE;

		GetDlgItem(IDC_bDUP_SEARCH)->ShowWindow(SW_SHOW);
	}
	else
	{
		m_bDupSearchType = FALSE;

		GetManageValue(_T("����"), _T("����"), _T("���������ڵ���������"), _T(""), strValue);
		if (strValue == _T("Y"))		m_bAutoDupSearch = TRUE;
		else 
		{
			m_bAutoDupSearch = FALSE;
			GetDlgItem(IDC_bDUP_SEARCH)->ShowWindow(SW_SHOW);
		}
	}


	//=======================================================================
	// 4. ����Ÿ�Կ� ���� ���� ������ �����Ѵ�.	
	//		[�Է�]
	//		1-1. DM���� �� �ʱ�ȭ �Ѵ�.
	//		1-2. �ڷ���¸� �����Ѵ�.
	//		1-3. �� API�� ��带 �����Ѵ�.
	//		1-4. ����, ��, å, �������� DM�� Row�� �ϳ� �߰��Ѵ�.
	//		1-5. �Լ������� �����̶��, �������� DM�� BP_�����������п� _T("1")�� �����Ѵ�.
	//		1-6. Head/Tail ���̾�α׸� ���� �� �����Ѵ�.
	//		1-7. _T("ȭ�������")�� _T("��������") ��ư�� ���̰� �Ѵ�.
	//		1-8. �ʱⰪ�� �ҷ��´�.
	//
	//		[����]
	//		2-1. �θ�׸����� ���õ� �ο츦 �����Ѵ�.
	//		2-2. ��Ÿ�Կ����� DM �� �۾����¸� �����Ѵ�. (�Է¿����� 1,2��)
	//		2-3. �� API�� ��带 �����Ѵ�.
	//		2-4. �׸��带 ���� ǥ���Ѵ�.
	//		2-5. ��������� ������ �����ο�(m_nParentSelectedRow)�� ��Ű�� ��������Ű�� ������ 
	//			 ���DM(����,��,��������,�η�����)�� �����Ѵ�.
	//		2-6. Head/Tail ���̾�α׸� ���� �� �����Ѵ�.
	//		2-7. ��ũ�κ��� ����DM�� ���� �����Ѵ�.
	//		2-8. ����,����, ���������ư�� ���̰� �Ѵ�.
	//=======================================================================
	switch( m_nOpType )
	{
	// ��-�Է�
	case SINSERT:
	{
			// 1. DM���� �� �ʱ�ȭ �Ѵ�.
		SetDataManagers();

			// 2. �ڷ���¸� �����Ѵ�.
		SetWorkingStatus();
			
			// 3. �� API�� ��带 �����Ѵ�.
		m_pFormApi->SetAppMode(MODE_INSERT);
			
		// 4. ����, ��, å, �������� DM�� Row�� �ϳ� �߰��Ѵ�.
		m_pDM_Index->InsertRow(-1);											// ����
		m_pDM_Species->InsertRow(-1);										// ��
		m_pDM_Book->InsertRow(-1);											// å
		if( m_pDM_Accu->GetRowCount() == 0 )	m_pDM_Accu->InsertRow(-1);	// ��������

		// 5. �Լ������� �����̶��, �������� DM�� BP_�����������п� _T("1")�� �����Ѵ�.
		if( m_nAcqType == PURCHASE )			m_pDM_Accu->SetCellData(_T("BP_������������"), _T("1"), 0);
		
		// 6. Head/Tail ���̾�α׸� ���� �� �����Ѵ�.
		SetHeadTailInfoDlg();

			// 7. _T("ȭ�������")�� _T("��������") ��ư�� ���̰� �Ѵ�.
		GetDlgItem(IDC_bCLEARSCREEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnPREV_CONTENT)->ShowWindow(SW_SHOW);
		SetWindowText(_T("���������Է�"));

		m_pTailDlg->SetInit(FALSE);

		// 8. ���Խ�û�� �ð� ����
		CString strDate = m_pBOSpeciesApi->GetTodayDate();
		CString sDate, strTime;
		CTime t = CTime::GetCurrentTime();
		//strDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());		// �Է���
		sDate.Format(_T("%s %02d:%02d:%02d"), strDate, t.GetHour(), t.GetMinute(), t.GetSecond());		// �Է���
		strApplicantTime(sDate);
			
			// 8. �ʱⰪ�� �ҷ��´�.
		LoadDefaultValue();
	}
		break;
	// ����
	case SUPDATE:
	case GUPDATE:
	{
			// 1. �θ�׸����� ���õ� �ο츦 �����Ѵ�.
		m_nParentSelectedRow = GetFirstSelectIndex();

			// 2. ��Ÿ�Կ����� DM �� �۾����¸� �����Ѵ�. (�Է¿����� 1,2��)
		UpdateInitProc();

			// 3. �� API�� ��带 �����Ѵ�.
			m_pFormApi->SetAppMode(MODE_UPDATE);

			// 4. �׸��带 ���� ǥ���Ѵ�.
		m_pGrid_Parent->SetReverse(m_nParentSelectedRow);

			// 5. ��������� ������ �����ο�(m_nParentSelectedRow)�� ��Ű�� ��������Ű�� ������ 
			//	  ���DM(����,��,��������,�η�����)�� �����Ѵ�.
		ids = MakeUpdateDM();
			if( ids < 0 ) 
		{
				ESLAfxMessageBox(_T("UPDATE DM ���� ����"));
			return -1;
		}
		
			// 6. Head/Tail ���̾�α׸� ���� �� �����Ѵ�.
		SetHeadTailInfoDlg();

			// 7. ��ũ�κ��� ����DM�� ���� �����Ѵ�.		
		ApplyMarcToIndexCM();	

			// 8. ����,����, ���������ư�� ���̰� �Ѵ�.
		GetDlgItem(IDC_bSAVEAS		)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bPREV_SPECIES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bNEXT_SPECIES)->ShowWindow(SW_SHOW);
		SetWindowText(_T("������������"));
		
		m_pTailDlg->SetInit(TRUE);		

		Display();		
	}
		break;

	default:	return -1;
		break;

	}

	//=======================================================================
	// 5. �켱�����ڷ�� �η����� ��뿩�ο� ���� ��Ÿ���� üũ�Ͽ� �켱�����ڷ�üũ�ڽ��� �η�������ư�� ��Ÿ����.
	//=======================================================================
	ShowControlBySpeciesType();	

	//=======================================================================
	// 6. ���Ա��п� ���� ���Խ�û��/��û���ڸ� Ȱ��ȭ/��Ȱ��ȭ �Ѵ�.
	//=======================================================================
	ShowControlByAcqType();	

	//=======================================================================
	// 7. ������ȣ �ο����� ����
	//=======================================================================
	m_cCopyCodeMgr.Init(m_pDM_Index->CONNECTION_INFO);	
	
	//=======================================================================
	// 8. FormApi�� �߰�
	//=======================================================================
	m_pFormApi->SetCopyCodeMgr(&m_cCopyCodeMgr);	

	//=======================================================================
	// 9. �Է½� �⺻�� �� ���������� ����Ǿ� ���� ���ƾ��� Ctrl�˸��ƽ��� �����Ѵ�.
	//=======================================================================
	m_arrIgnoreCTRLAlias.Add(_T("IBS_��������_�ڵ�"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_��ü����_�ڵ�"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_��ġ��ȣ_�ڵ�"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_������ϱ���_�ڵ�"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_�з�ǥ����"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_��������_�ڵ�"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_�̿��󱸺�_�ڵ�"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_�̿����ѱ���_�ڵ�"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_�ڷᱸ��_�ڵ�"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_�����ڷᱸ��_�ڵ�"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_���ı���_�ڵ�"));
	m_arrIgnoreCTRLAlias.Add(_T("BP_��������_�ڵ�"));	
	m_arrIgnoreCTRLAlias.Add(_T("�ڷᱸ��"));
	m_arrIgnoreCTRLAlias.Add(_T("BP_�ѱǼ�"));
	m_arrIgnoreCTRLAlias.Add(_T("BP_��å��"));
	m_arrIgnoreCTRLAlias.Add(_T("���Խ�û��"));
	// 2006.03.31 KBS - ����� �䱸���� �߰�
	m_arrIgnoreCTRLAlias.Add(_T("IBS_�����з��ڵ�1"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_�����з��ڵ�2"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_�����з��ڵ�3"));	

	//=======================================================================
	// 10. ��å�� �� �ѱǼ��� ��Ȱ��ȭ ��Ų��.
	//=======================================================================
	((CEdit*)m_pCM_Accu->FindControl(_T("BP_��å��")))->SetReadOnly(TRUE);
	((CEdit*)m_pCM_Accu->FindControl(_T("BP_�ѱǼ�")))->SetReadOnly(TRUE);	

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��Ÿ�Կ����� DM �� �۾����¸� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ��Ÿ���� ����̶�� �Ʒ� ������ ó���Ѵ�.
//		i.	���Ա��а��� ������ ��������� ���Ͽ� ����Ǿ����� üũ�Ѵ�.
//		ii.	���Ա��а������� ��Ÿ�԰� �Լ������� ��������� �����Ѵ�.
//		iii.�׷������� �ִٸ� ��� �����ϰ� �ٽ� �����Ѵ�.
// 2. ������� DM�� �����ϰ� ������ �ʱ�ȭ�Ѵ�.
// 3. ��Ÿ�Կ����� ���� �Է�â ���� ������ �۾����¸� �����Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::UpdateInitProc()
{
	//=======================================================================
	// ��������
	//=======================================================================
	CString strAcqCode;

	//=======================================================================
	// 1. ��Ÿ���� ����̶�� �Ʒ� ������ ó���Ѵ�.
	//		i.	���Ա��а��� ������ ��������� ���Ͽ� ����Ǿ����� üũ�Ѵ�.
	//		ii.	���Ա��а������� ��Ÿ�԰� �Լ������� ��������� �����Ѵ�.
	//		iii.�׷������� �ִٸ� ��� �����ϰ� �ٽ� �����Ѵ�.
	//=======================================================================
	if( m_nSpeciesType == BO_REG_INVALID	|| 
		m_nSpeciesType == BO_REG_PURCHASE	|| 
		m_nSpeciesType == BO_REG_DONATE		)
	{
		strAcqCode = m_pDM_Parent->GetCellData(_T("BB_���Ա���"), m_nParentSelectedRow);
		// i.
		if( m_nAcqType != (ACQ_TYPE)_ttoi(strAcqCode.GetBuffer(0)) )	
		{
				m_bAcqTypeChange = TRUE;
		}
		else m_bAcqTypeChange = FALSE;

		// ii.
		if( strAcqCode == _T("0") || strAcqCode.IsEmpty() == TRUE ) 
		{
			m_nSpeciesType = BO_REG_INVALID;
			m_nAcqType = ACQ_TYPE_INVALID;
		}
		else if( strAcqCode == _T("1") )
		{
			m_nSpeciesType = BO_REG_PURCHASE;
			m_nAcqType = PURCHASE;
		}
		else if( strAcqCode == _T("2") )
		{
			m_nSpeciesType = BO_REG_DONATE;
			m_nAcqType = DONATE;
		}
		else 
		{
			m_nSpeciesType = BO_REG_INVALID;
			m_nAcqType = OTHER_ACQ_TYPE;
		}

		// iii.
		if( m_pGroupCollection )
		{
			m_pGroupCollection->RemoveAll();
			m_pGroupCollection->SetAcqType(m_nAcqType);
			m_pGroupCollection->SetAcqCode(strAcqCode);
		}
	}

	//=======================================================================
	// 2. ������� DM�� �����ϰ� ������ �ʱ�ȭ�Ѵ�.
	//=======================================================================
	SetDataManagers();

	//=======================================================================
	// 3. ��Ÿ�Կ����� ���� �Է�â ���� ������ �۾����¸� �����Ѵ�.
	//=======================================================================
	SetWorkingStatus();
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �켱�����ڷ�� �η����� ��뿩�ο� ���� ��Ÿ���� üũ�Ͽ� �켱�����ڷ�üũ�ڽ��� �η�������ư�� ��Ÿ����.
//
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::ShowControlBySpeciesType()
{
	// �������� �ʱ�ȭ
	GetDlgItem(IDC_bAPPENDIX			)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_chkPRIORITY_CATALOG	)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_tPRIORITY_CATALOG	)->ShowWindow(SW_HIDE);

	//��ȭ���� ȣ���ϴ� ������ ���� �η����� ��ư�� SHOW/HIDE�� �����ϰ� ȭ������� ��ư�� �̵��Ѵ�.
	// 2004.10.13 5163 �켱�����ڷ�, �η�������ư�� ���� ��Ʈ�� ���̱�/�����
	// BEGIN:
	if( m_strPriorityCatalogYN == _T("Y") || m_strAcqAppendixYN == _T("Y") )
	{
		// 2005-01-24 ����, ������ϴ�� �߰�.
		if( m_nSpeciesType == BO_PURCHASE							|| 
			m_nSpeciesType == BO_DONATE								|| 
			m_nSpeciesType == BO_ORDER								|| 
			m_nSpeciesType == BO_MISSING							|| 
			(m_nSpeciesType == BO_REGOBJ && m_nAcqType == PURCHASE) || 
			(m_nSpeciesType == BO_REGOBJ && m_nAcqType == DONATE)	||
			m_nSpeciesType == BO_REG_PURCHASE						|| 
			m_nSpeciesType == BO_REG_DONATE							|| 
			m_nSpeciesType == BO_REG_INVALID						|| 
			m_nSpeciesType == BO_CAT								)
		{
			// 2004.10.13 5163_1.����,��ϴܰ迡�� �η����� ��ư �����ش�.
			if( m_strAcqAppendixYN == _T("Y") )
			{
				GetDlgItem(IDC_bAPPENDIX)->ShowWindow(SW_SHOW);
			}

			// 2004.10.13 5163_1.����,��ϴܰ迡�� �켱�����ڷ�� ������ �� �ֵ��� üũ�ڽ��� �����ش�.
			if( m_strPriorityCatalogYN == _T("Y") )
			{
				GetDlgItem(IDC_chkPRIORITY_CATALOG	)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_tPRIORITY_CATALOG	)->ShowWindow(SW_SHOW);
			}
			}
		}
	// END:
		else
		{
		if( m_nSpeciesType == BO_REG_PURCHASE	|| 
			m_nSpeciesType == BO_REG_DONATE		|| 
			m_nSpeciesType == BO_REG_INVALID	|| 
			m_nSpeciesType == BO_CAT			)
	{
			GetDlgItem(IDC_bAPPENDIX)->ShowWindow(SW_SHOW);
		}
	}

}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::ShowCodeResource()
{
	INT nCount, nCodeResourcesHeight, i;	
	INT nArrCodeResourceID[256];
	INT nArrOtherResources[256];
	CWnd *pWnd = NULL;
	CRect rcWindow, rcResource;

	//=======================================================================
	// 1. �����˻�/�󼼰˻� �ȿ� �ִ� ���ҽ����� �����ϰ� ���̱� �� ����⸦ �����Ѵ�.
	//20070829 UPDATE PJW : �����˻�/�󼼰˻�  -> ��������/�󼼺���
	//=======================================================================
	if( m_bShowCode == TRUE )	GetDlgItem(IDC_btnBSFORM_SHOW_HIDE_CODE)->SetWindowText(_T("��������"));
	else						GetDlgItem(IDC_btnBSFORM_SHOW_HIDE_CODE)->SetWindowText(_T("�󼼺���"));
	
	/**************************************************************
	 **************************************************************/
	//20071205 ADD PJW : ��� ���ڼ� ���� ����
	//20080522 UPDATE PJW : ��� ���ڼ� 200���� ����
	
	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_eDISCRIPTION);
	pEdt->SetLimitText(200);
	CEdit* pEdt1 = (CEdit*)GetDlgItem(IDC_eDISCRIPTION_SMALL);
	pEdt1->SetLimitText(200);
	/**************************************************************
	 **************************************************************/

	GetWindowRect(rcWindow);
	if( m_bShowCode == TRUE && rcWindow == m_rcWindow )		return;

	nCount = 0;	
	nCodeResourcesHeight = 97;
	nArrCodeResourceID[nCount++] = IDC_STATIC_CODE;
	nArrCodeResourceID[nCount++] = IDC_tSUB_REG_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cSUB_REG_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tMEDIA_CLASS;
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
	nArrCodeResourceID[nCount++] = IDC_tDISCRIPTION;
	nArrCodeResourceID[nCount++] = IDC_eDISCRIPTION;
	// 2004.11.04 5163_3. ����� �����ڵ�, ǥ���߰���� �߰�
	// BEGIN:
	if( _T("Y") == m_strUseCodeYN )
	{
		nArrCodeResourceID[nCount++] = IDC_eDISCRIPTION_SMALL;
		nArrCodeResourceID[nCount++] = IDC_stcUSERCODE3;
		nArrCodeResourceID[nCount++] = IDC_cmbUSERCODE3;
	}
	
	// Show / Hide Code Resource
	for( i=0; i<nCount; i++ )
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if( pWnd->GetSafeHwnd() != NULL )
		{
			if( m_bShowCode == TRUE )	pWnd->ShowWindow(SW_SHOW);
			else						pWnd->ShowWindow(SW_HIDE);
		}
	}
	
	//=======================================================================
	// 2004.11.04 
	// 2. ����� �����ڵ� �����Ѵ�.
	//=======================================================================
	// BEGIN:
	if( _T("Y") == m_strUseCodeYN )
	{
		ShowUserDefineCode(m_bShowCode);
	}
	else // m_strUseCodeYN == _T("N") �ϰ�� ����� �����ڵ� ��� ����.
	{
		pWnd = GetDlgItem(IDC_stcUSERCODE1);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbUSERCODE1);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_stcUSERCODE2);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbUSERCODE2);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_stcUSERCODE3);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbUSERCODE3);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_eDISCRIPTION_SMALL);	pWnd->ShowWindow(SW_HIDE);
	}
	// END:


	//=======================================================================
	// 3. �̿��� ������ ���ҽ����� �����Ѵ�.
	//=======================================================================
	// Move Other Resources
	nCount = 0;
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
	nArrOtherResources[nCount++] = IDC_STATIC_FORM_ELEMENT;
	nArrOtherResources[nCount++] = IDC_tPAGE;
	nArrOtherResources[nCount++] = IDC_tPHYSICAL_CHARACTER;
	nArrOtherResources[nCount++] = IDC_stcSIZE;
	nArrOtherResources[nCount++] = IDC_stcAPPENDIX;
	nArrOtherResources[nCount++] = IDC_edtAPPENDIX;
	nArrOtherResources[nCount++] = IDC_edtSIZE;
	nArrOtherResources[nCount++] = IDC_edtPHYSICAL_CHARACTER;
	nArrOtherResources[nCount++] = IDC_edtPAGE;
	nArrOtherResources[nCount++] = IDC_STATIC_BOOK_REGCODE;
	nArrOtherResources[nCount++] = IDC_tREG_CLASS_CODE;
	nArrOtherResources[nCount++] = IDC_tTOTAL_VOLUMEP;
	nArrOtherResources[nCount++] = IDC_tTOTAL_BOOKP;
	nArrOtherResources[nCount++] = IDC_bAPPLY_SERIES;
	nArrOtherResources[nCount++] = IDC_bVOL_INFO;
	nArrOtherResources[nCount++] = IDC_eTOTAL_BOOKP;
	nArrOtherResources[nCount++] = IDC_eTOTAL_VOLUMEP;
	nArrOtherResources[nCount++] = IDC_eREG_CLASS_CODE;
	nArrOtherResources[nCount++] = IDC_bAPPENDIX;
	nArrOtherResources[nCount++] = IDC_bMARC_EDIT;
	nArrOtherResources[nCount++] = IDC_bMARC_IMPORT;
	nArrOtherResources[nCount++] = IDC_bKOLIS_NET;
	nArrOtherResources[nCount++] = IDC_bCLEARSCREEN;
	nArrOtherResources[nCount++] = IDC_bPREV_SPECIES;
	nArrOtherResources[nCount++] = IDC_bNEXT_SPECIES;
	nArrOtherResources[nCount++] = IDC_bSAVEAS;
	nArrOtherResources[nCount++] = IDC_bSAVE;
	nArrOtherResources[nCount++] = IDC_bCLOSE;
	nArrOtherResources[nCount++] = IDC_STATIC_ISBN;
	nArrOtherResources[nCount++] = IDC_bDEFAULT_CONFIG;
	nArrOtherResources[nCount++] = IDC_tSERIES_VOL;
	nArrOtherResources[nCount++] = IDC_edtSERIES_VOL;
	nArrOtherResources[nCount++] = IDC_btnPREV_CONTENT;
	nArrOtherResources[nCount++] = IDC_stcBSFORM_REQUESTOR;
	nArrOtherResources[nCount++] = IDC_edtBSFORM_REQUESTOR;
	nArrOtherResources[nCount++] = IDC_stcBSFORM_REQUEST_DATE;
	nArrOtherResources[nCount++] = IDC_edtBSFORM_REQUEST_DATE;
	//20070810 ADD PJW : ��û �ð��� ���ؼ� IDC_edtBSFORM_REQUEST_HOUR �߰�
	nArrOtherResources[nCount++] = IDC_edtBSFORM_REQUEST_HOUR;
	nArrOtherResources[nCount++] = IDC_STATIC_EBOOK_GROUP;
	nArrOtherResources[nCount++] = IDC_stcEBOOK_COVER_FILE;
	nArrOtherResources[nCount++] = IDC_edtEBOOK_COVER_FILE;
	nArrOtherResources[nCount++] = IDC_btnFIND_FILE1;
	nArrOtherResources[nCount++] = IDC_btnFIND_FILE2;
	nArrOtherResources[nCount++] = IDC_btnFIND_FILE3;
	nArrOtherResources[nCount++] = IDC_stcEBOOK_FILE;
	nArrOtherResources[nCount++] = IDC_edtEBOOK_FILE;
	nArrOtherResources[nCount++] = IDC_stcEBOOK_EXECUTE_FILE;
	nArrOtherResources[nCount++] = IDC_edtEBOOK_EXECUTE_FILE;
	nArrOtherResources[nCount++] = IDC_btnEBOOK_VIEW;
	//nArrOtherResources[nCount++] = IDC_APPLICAND_KIND;
	nArrOtherResources[nCount++] = IDC_cAPPLICAND_KIND_DEPT;
	nArrOtherResources[nCount++] = IDC_cAPPLICAND_KIND_COURCE;
	nArrOtherResources[nCount++] = IDC_cAPPLICAND_KIND_CLASS;
	nArrOtherResources[nCount++] = IDC_cmbAPPLICANT;
	nArrOtherResources[nCount++] = IDC_tClass_Kind;
	nArrOtherResources[nCount++] = IDC_tClass_Code;


	GetDlgItem(IDC_tClass_Kind)->ShowWindow(SW_SHOW);
	OnSelchangecmbAPPLICANT();

	// 2004.11.09 5163 ǥ���߰� ��� ���.
	// BEGIN:
	if( _T("Y") == m_strUseCoverYN )		nArrOtherResources[nCount++] = IDC_btnCover;
	// END:
	
	// 2004.12.29 5163�� ��� ȭ�鿡 ��������ȸ ��ư�� ����Ͽ� �Է� �����ϵ��� �Ѵ�.
	if( _T("Y") == m_strIs5163 )
	{
		//nArrOtherResources[nCount++] = IDC_edtUserName;
		nArrOtherResources[nCount++] = IDC_btnSearchUser;
	}

	for( i=0; i<nCount ; i++ )
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if( pWnd->GetSafeHwnd() != NULL )
		{
			pWnd->GetWindowRect(rcResource);
			ScreenToClient(rcResource);
			if( m_bShowCode == TRUE )
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

	// m_strUseCoverYN == _T("N") �ϰ�� ǥ�� �߰� ��ư ����.
	// BEGIN:
	if( _T("N") == m_strUseCoverYN )
	{
		pWnd = GetDlgItem(IDC_btnCover);			
		pWnd->ShowWindow(SW_HIDE);
	}
	// END:

	// 2004.12.29 5163�� ��� ��������ȸ ��ư �����ش�.
	if( _T("Y") == m_strIs5163 )
	{
		pWnd = GetDlgItem(IDC_btnSearchUser);			
		pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		pWnd = GetDlgItem(IDC_btnSearchUser);			
		pWnd->ShowWindow(SW_HIDE);
	}

	MoveTailDlg(nCodeResourcesHeight);
	
	// Resize Window Size
	GetWindowRect(rcResource);
	if( m_bShowCode == TRUE )
	{
		if( m_bShowEBookInfo == TRUE )
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
		else
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height()-65,  SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		if( m_bShowEBookInfo == TRUE )
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - (nCodeResourcesHeight),  SWP_NOMOVE | SWP_NOZORDER);
		else
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - (nCodeResourcesHeight+65),  SWP_NOMOVE | SWP_NOZORDER);
	}

	//=========================================================
	//2008.10.20 ADD BY PJW : MANAGE_CODE ��Ʈ���� �����.
	pWnd = GetDlgItem(IDC_cMANAGE_CLASS);		pWnd->ShowWindow(SW_HIDE);
	pWnd = GetDlgItem(IDC_tMANAGE_CLASS);		pWnd->ShowWindow(SW_HIDE);
	//=========================================================
	
	Invalidate();

	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��������� ������ �����ο��� ��Ű�� ��������Ű�� ������ ���DM(����,��,��������,�η�����)�� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ������ Row�� SpeciesKey, PurchaseKey�� �����´�.
// 2. ������ ��Ű�� ������ ����DM�� �����Ѵ�.(���ε�ó��)
// 3. ��KEY�� ��������� �����Ѵ�.
// 4. ���� DM�� ������ ���� ��� ���� �����Ѵ�.
// 5. ����DM�� �۾����°� ��� �� ������� �����߱⸦ �����Ѵ�.
// 6. ��DM�� �����Ѵ�. (���ε�ó��)
// 7. ��DM�� ������ ������� ���λ����Ѵ�.
// 8. ��������DM�� �����Ѵ�.
// 9. å����DM�� �����Ѵ�.
// 10. å �����Ҷ� �⺻������ ������ �ʵ带 �����Ѵ�.
// 11. ��������Ű�� ���ų� -1�̶�� ��������DM�� �����Ѵ�.
// 12. �η�����DM�� �����Ѵ�.
// 13. ��ġ��������� �����Ѵ�. (���Խ�û��/���Խ�û��) [BO_FURNISH_TBL]
// 14. ��Ͽ����� �����̶�� �������������� �����Ѵ�.
// 15. ��DM�� �����ڷᱸ�� �� ��� ����DM�� �����Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::MakeUpdateDM()
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT ids, nRepresentBookIdx = -1;
	CString strSpeciesKey, strAccuKey, strOption;
	EDBM_BIND_DATA_TYPE* pBind = NULL;

	
	if( m_nParentSelectedRow < 0 )		return -1;	
	

	//=======================================================================
	// 1. ������ Row�� SpeciesKey, PurchaseKey�� �����´�.
	//=======================================================================
	m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, m_nParentSelectedRow, strSpeciesKey);
	m_pDM_Parent->GetCellData(m_strAccuKeyAlias, m_nParentSelectedRow, strAccuKey);
	if( strSpeciesKey.IsEmpty() == TRUE )	return -1;
	if( strAccuKey.IsEmpty() == TRUE && 
		(m_nSpeciesType != BO_BASIC && m_nSpeciesType != BO_IMPORT && m_nSpeciesType != BO_REG_INVALID) ) return -1;


	//=======================================================================
	// 2. ������ ��Ű�� ������ ����DM�� �����Ѵ�.(���ε�ó��)
	//=======================================================================
	InitEDBMBindDataType(&pBind, 0, 1);
	strOption.Format(_T("REC_KEY = :SPECIES_KEY") );
	AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );
	ids = m_pDM_Index->RefreshDataManager(strOption, NULL, pBind);
	FreeEDBMBindData(&pBind, 1);
	if( ids < 0 )	return -1;	

	//=======================================================================
	// 3. ��KEY�� ��������� �����Ѵ�.
	//=======================================================================
	_tcscpy ( m_szSpeciesKey , strSpeciesKey.GetBuffer(0) );
	
	
	//=======================================================================
	// 4. ���� DM�� ������ ���� ��� ���� �����Ѵ�.
	//=======================================================================
	if( m_pDM_Index->GetRowCount() == 0 )
	{
		if( IDNO == ESLAfxMessageBox(_T("���������� �������� �ʽ��ϴ�. ���λ��� �Ͻðڽ��ϱ�?."), MB_YESNO) )
			return -1;

		m_pDM_Index->InsertRow(-1);
		m_pDM_Index->SetCellData(_T("IBS_REC_KEY"), strSpeciesKey, 0);
		m_pDM_Index->StartFrame();
		CArray<CString, CString> RemoveAliasList;	RemoveAliasList.RemoveAll();
		m_pDM_Index->MakeQueryByDM(RemoveAliasList, m_pDM_Index, m_pDM_Index->TBL_NAME, 0, m_pDM_Index);
		ids = m_pDM_Index->SendFrame();
		if( ids < 0 ) return ids;
		ids = m_pDM_Index->EndFrame();
		if( ids < 0 ) return ids;
	}	

	//=======================================================================
	// 5. ����DM�� �۾����°� ��� �� ������� �����߱⸦ �����Ѵ�.
	//=======================================================================
	if( m_pDM_Index->GetCellData(_T("IBS_�۾�����INFO"), 0).Left(3) == _T("BOL")	||
		m_pDM_Index->GetCellData(_T("IBS_�۾�����INFO"), 0).Left(3) == _T("BOC")	)
	{
		m_bDupAddYN = TRUE;
	}
	else
	{
		m_bDupAddYN = FALSE;
	}

	//=======================================================================
	// 6. ��DM�� �����Ѵ�. (���ε�ó��)
	//=======================================================================
	pBind = NULL;
	InitEDBMBindDataType( &pBind, 0, 1);
	strOption.Format(_T("REC_KEY = :SPECIES_KEY"));
	AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );
	ids = m_pDM_Species->RefreshDataManager(strOption, NULL, pBind);
	FreeEDBMBindData(&pBind, 1);
	if( ids < 0 ) return -1;
	
	//=======================================================================
	// 7. ��DM�� ������ ������� ���λ����Ѵ�.
	//=======================================================================
	if(m_pDM_Species->GetRowCount() == 0)
	{
		if(IDNO == ESLAfxMessageBox(_T("�������� �������� �ʽ��ϴ�. ���λ��� �Ͻðڽ��ϱ�?."), MB_YESNO))
			return -1;
		m_pDM_Species->InsertRow(-1);
		m_pDM_Species->SetCellData(_T("BS_��KEY"), strSpeciesKey, 0);
		m_pDM_Species->StartFrame();
		CArray<CString, CString> RemoveAliasList;	RemoveAliasList.RemoveAll();
		m_pDM_Species->MakeQueryByDM(RemoveAliasList, m_pDM_Species, m_pDM_Species->TBL_NAME, 0, m_pDM_Species);
		ids = m_pDM_Species->SendFrame();
		if(ids < 0) return ids;
		ids = m_pDM_Species->EndFrame();
		if(ids < 0) return ids;	
	}
	
	//=======================================================================
	// 8. ��������DM�� �����Ѵ�.
	//=======================================================================
	if( strAccuKey.IsEmpty() == FALSE  && strAccuKey != _T("-1") )
	{
		pBind = NULL;
		InitEDBMBindDataType( &pBind, 0, 1);

		strOption.Format(_T("REC_KEY = :REC_KEY "));
		AddEDBMBindDataByString(pBind, STRING, _T(":REC_KEY"), strAccuKey.GetBuffer(0) );

		ids = m_pDM_Accu->RefreshDataManager(strOption, NULL, pBind);
		FreeEDBMBindData(&pBind, 1);
		if( ids < 0 ) return ids;
	}
	
	//=======================================================================
	// 9. å����DM�� �����Ѵ�.
	//=======================================================================
	pBind = NULL;
	InitEDBMBindDataType( &pBind, 0, 1);
	
	if( strAccuKey.IsEmpty() == FALSE  && strAccuKey != _T("-1") )
	{
		strOption.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = :SPECIES_KEY AND B.ACQ_KEY = :ACQ_KEY AND B.BOOK_APPENDIX_FLAG = 'B' AND REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY B.REPRESENT_BOOK_YN DESC, VOL_SORT_NO"));
		AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );
		AddEDBMBindDataByString(pBind, STRING, _T(":ACQ_KEY"), strAccuKey.GetBuffer(0) );
	}
	else
	{
		if( m_nSpeciesType == BO_BASIC )
		{
			strOption.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = :SPECIES_KEY AND B.BOOK_APPENDIX_FLAG = 'B' AND B.WORKING_STATUS = 'BOT111O' AND B.REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY B.REPRESENT_BOOK_YN DESC, B.VOL_SORT_NO"));
			AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );
		}
		else if(m_nSpeciesType == BO_IMPORT)
		{
			strOption.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = :SPECIES_KEY AND B.BOOK_APPENDIX_FLAG = 'B' AND B.WORKING_STATUS = 'BOT112O' AND B.REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY B.REPRESENT_BOOK_YN DESC, B.VOL_SORT_NO"));
			AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );
		}
		else if(m_nSpeciesType == BO_REG_INVALID || m_nSpeciesType == BO_REG_PURCHASE || m_nSpeciesType == BO_REG_DONATE)
		{
			strOption.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = :SPECIES_KEY AND B.BOOK_APPENDIX_FLAG = 'B' AND B.WORKING_STATUS BETWEEN 'BOR111N' AND 'BOR113O' AND B.REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY B.REPRESENT_BOOK_YN DESC, B.VOL_SORT_NO"));
			AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );
		}
		
	}
	
	ids = m_pFormApi->MakeBookDM(strOption, m_pDM_Book, pBind);
	FreeEDBMBindData(&pBind, 1);
	if( ids < 0 ) return ids;
	
	//=======================================================================
	// 10. å �����Ҷ� �⺻������ ������ �ʵ带 �����Ѵ�.
	//=======================================================================
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��ǥå����"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��Ϲ�ȣ"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_������ȣ"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_�谡��ġ��ȣ"));	
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));	

	
	//=======================================================================
	// 11. ��������Ű�� ���ų� -1�̶�� ��������DM�� �����Ѵ�.
	//=======================================================================
	nRepresentBookIdx = -1;
	if( strAccuKey.IsEmpty() == TRUE || strAccuKey == _T("-1") )
	{
		m_pFormApi->GetRepresentationBook(m_pDM_Book, nRepresentBookIdx);
		MakeAccuInfo(m_pDM_Species, m_pDM_Book, nRepresentBookIdx);
	}

	//=======================================================================
	// 12. �η�����DM�� �����Ѵ�.
	//=======================================================================
	pBind = NULL;
	InitEDBMBindDataType( &pBind, 0, 1);
	strOption.Format(_T("SPECIES_KEY=:SPECIES_KEY AND BOOK_APPENDIX_FLAG='A'") );
	AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );

	// 2007.08.10 ADD BY PDJ
	// ���ι�ȣ�� �����ֱ� ���� UDF->STR�� ����
	SetUDFCheck(m_pDM_Appendix, _T("UDF_���ι�ȣ"), _T("str"));	
	ids = m_pDM_Appendix->RefreshDataManager(strOption, NULL, pBind);
	FreeEDBMBindData(&pBind, 1);
	SetUDFCheck(m_pDM_Appendix, _T("UDF_���ι�ȣ"), _T("udf"));

	// 2007.08.10 ADD BY PDJ
	// UPDATE�� ���ι�ȣ ���ܸ� ���� UDF�� ��ȯ
	if(ids < 0) return ids;
	
	//=======================================================================
	// 13. ��ġ��������� �����Ѵ�. (���Խ�û��/���Խ�û��) [BO_FURNISH_TBL]
	//=======================================================================
	SetFurnishInfo();

	//=======================================================================
	// 14. ��Ͽ����� �����̶�� �������������� �����Ѵ�.
	//=======================================================================
	if( m_nSpeciesType == BO_REG_PURCHASE && m_nAcqType == PURCHASE )
	{
		m_pDM_Accu->SetCellData(_T("BP_������������"), m_pDM_Index->GetCellData(_T("IBS_��������_�ڵ�"), 0), 0);
	}

	//=======================================================================
	// 15. ��DM�� �����ڷᱸ�� �� ��� ����DM�� �����Ѵ�.
	//=======================================================================
	m_pDM_Index->SetCellData(_T("IBS_�����ڷᱸ��_�ڵ�"), m_pDM_Species->GetCellData(_T("BS_�����ڷᱸ��"), 0), 0);
	m_pDM_Index->SetCellData(_T("IBS_���"), m_pDM_Species->GetCellData(_T("BS_���"), 0), 0);

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��ũ�� ���� ���������� ������ ����DM�� �����Ѵ�.
//		1.	250$a - IBS_�ǻ���
//		2.	260$a - IBS_������
//		3.	300$a - IBS_�����
//		4.	300$b - IBS_������Ư��
//		5.	300$c - IBS_ũ��
//		6.	300$e - IBS_�����ڷ�
//		7.	020$a - IBS_����ISBN
//		8.	020$g - IBS_����ISBS�ΰ���ȣ
//		9.	020$z - IBS_���ǿ���ISBN
//		10.	020$a - IBS_��Ʈ_ISBN1
//		11.	020$g - IBS_��Ʈ_ISBN2
//		12.	020$z - IBS_��Ʈ_ISBN3
//		13.	490$a - IBS_�Ѽ���
//		14.	440$a - IBS_�Ѽ���
//		15.	440$v - IBS_�Ѽ�����
//		16.	950$a - BP_���ݼ���
//		17.	245$n - IBS_�����
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::ApplyMarcToIndexCM()
{
	CString strMarc, strMarcData1;
	CMarc marc;

	m_pDM_Species->GetCellData(_T("BS_MARC"), 0, strMarc);
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	
	//m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("250$a"), _T("  "), strMarcData1);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("250$a"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_�ǻ���"), strMarcData1, 0);
	//m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("260$a"), _T("  "), strMarcData1);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("260$a"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_������"), strMarcData1, 0);
	//m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("300$a"), _T("  "), strMarcData1);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("300$a"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_�����"), strMarcData1, 0);	
	//m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("300$b"), _T("  "), strMarcData1);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("300$b"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_������Ư��"), strMarcData1, 0);	
	//m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("300$c"), _T("  "), strMarcData1);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("300$c"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_ũ��"), strMarcData1, 0);	
	//m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("300$e"), _T("  "), strMarcData1);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("300$e"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_�����ڷ�"), strMarcData1, 0);	
	m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("020$a"), _T("  "), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_����ISBN"), strMarcData1, 0);	
	m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("020$g"), _T("  "), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_����ISBN�ΰ���ȣ"), strMarcData1, 0);
	m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("020$z"), _T("  "), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_���ǿ���ISBN"), strMarcData1, 0);
	m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("020$a"), _T("1 "), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_��Ʈ_ISBN1"), strMarcData1, 0);
	m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("020$g"), _T("1 "), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_��Ʈ_ISBN2"), strMarcData1, 0);
	m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("020$z"), _T("1 "), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_��Ʈ_ISBN3"), strMarcData1, 0);

	m_pInfo->pMarcMgr->GetItem(&marc, _T("490$a"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_�Ѽ���"), strMarcData1, 0);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("440$a"), strMarcData1);
	if( strMarcData1.IsEmpty() == FALSE ) 
	{
		m_pDM_Index->SetCellData(_T("IBS_�Ѽ���"), strMarcData1, 0);
	}
	m_pInfo->pMarcMgr->GetItem(&marc, _T("440$v"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_�Ѽ�����"), strMarcData1, 0);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("950$a"), strMarcData1);
	m_pDM_Accu->SetCellData(_T("BP_���ݼ���"), strMarcData1, 0);

	//����� �߰���.
	m_pInfo->pMarcMgr->GetItem(&marc, _T("245$n"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_�����"), strMarcData1, 0);

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��DM�� åDM�� �̿��Ͽ� ��������DM�� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ��ǥå�� �ε����� �����Ǿ� ���� �ʴٸ� åDM���� ��ǥå�� �����ϰ� ��ǥå�ε����� �����´�.
// 2. ��DM���� ��ũ������ ������ ����ü��ũ�� �����Ѵ�.
// 3. ��������DM�� �ο츦 �߰��Ѵ�.
// 4. ��ũ���� ���������� ������ ��������DM�� ���ݰ� ȭ�󱸺п� ���� �����Ѵ�.
// 5. ��ũ���� ������������ ������ ��������DM�� ������������ �����Ѵ�.
// 6. åDM���� ��ǥå�� �׷��ȣ�� ������ ��������DM�� �����Ѵ�.
// 7. åDM���� ��ǥå�� ���Գ⵵�� ������ ��������DM�� �����Ѵ�.
// 8. åDM���� ��ǥå�� �Է����ڸ� ������ ��������DM�� �����Ѵ�.
// 9. ��������DM�� ��å���� �����Ѵ�.
// 10. ��������DM�� �ѱǼ��� �����Ѵ�.
// 11. ��������M�� ��������_�ڵ带 �����Ѵ�.
// 12. �����߱��� �� Species_key�� �ʿ��ϹǷ� Species_Key�� Set���ش�.
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::MakeAccuInfo(CESL_DataMgr * pSpeciesDM, CESL_DataMgr * pBookDM, INT nBookIdx)
{
	//=======================================================================
	// ���� ����
	//=======================================================================
	INT nBookCnt, nVolCnt;
	CString strMarc, strData, strCurrencyCode, strPrice, strPriceOthreInfo;
	CMarc marc;

	//=======================================================================
	// 1. ��ǥå�� �ε����� �����Ǿ� ���� �ʴٸ� åDM���� ��ǥå�� �����ϰ� ��ǥå�ε����� �����´�.
	//=======================================================================
	if( nBookIdx < 0 )
	{
		m_pFormApi->SetRepresentationBook(pBookDM);
		m_pFormApi->GetRepresentationBook(pBookDM, nBookIdx);
	}
	
	//=======================================================================
	// 2. ��DM���� ��ũ������ ������ ����ü��ũ�� �����Ѵ�.
	//=======================================================================
	strMarc = pSpeciesDM->GetCellData(_T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	

	//=======================================================================
	// 3. ��������DM�� �ο츦 �߰��Ѵ�.
	//=======================================================================
	m_pDM_Accu->InsertRow(-1);


	//=======================================================================
	// 4. ��ũ���� ���������� ������ ��������DM�� ���ݰ� ȭ�󱸺п� ���� �����Ѵ�.
	//=======================================================================
	strData.Empty();	
	m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strData);
	if( strData.IsEmpty() == FALSE )	
	{
		GetPriceInfo(strData, strCurrencyCode, strPrice);
	}
	m_pDM_Accu->SetCellData(_T("BP_����"), strPrice, 0);
	m_pDM_Accu->SetCellData(_T("BP_ȭ�󱸺�"), strCurrencyCode, 0);
	

	//=======================================================================
	// 5. ��ũ���� ������������ ������ ��������DM�� ������������ �����Ѵ�.
	//=======================================================================
	strData.Empty();	
	m_pInfo->pMarcMgr->GetItem(&marc, _T("950$c"), strData);
	if( strData.IsEmpty() == FALSE ) 
	{
		m_pDM_Accu->SetCellData(_T("BP_����������"), strData, 0);
	}
	
	//=======================================================================
	// 6. åDM���� ��ǥå�� �׷��ȣ�� ������ ��������DM�� �����Ѵ�.
	//=======================================================================
	strData.Empty();	
	pBookDM->GetCellData(_T("BB_�׷��ȣ"), nBookIdx, strData);
	if( strData.IsEmpty() == FALSE )
	{
		m_pDM_Accu->SetCellData(_T("BP_�׷��ȣ"), strData, 0);
	}

	//=======================================================================
	// 7. åDM���� ��ǥå�� ���Գ⵵�� ������ ��������DM�� �����Ѵ�.
	//=======================================================================
	strData.Empty();	
	pBookDM->GetCellData(_T("BB_���Գ⵵"), nBookIdx, strData);
	if( strData.IsEmpty() == FALSE ) 
	{
		m_pDM_Accu->SetCellData(_T("BP_���Գ⵵"), strData, 0);
	}

	//=======================================================================
	// 8. åDM���� ��ǥå�� �Է����ڸ� ������ ��������DM�� �����Ѵ�.
	//=======================================================================
	strData.Empty();	
	pBookDM->GetCellData(_T("BB_�Է�����"), nBookIdx, strData);
	if( strData.IsEmpty() == FALSE ) 
	{
		m_pDM_Accu->SetCellData(_T("BP_�Է�����"), strData, 0);
	}


	//=======================================================================
	// 9. ��������DM�� ��å���� �����Ѵ�.
	//=======================================================================
	strData.Empty();
	nBookCnt = pBookDM->GetRowCount();
	strData.Format(_T("%d"), nBookCnt);
	m_pDM_Accu->SetCellData(_T("BP_��å��"), strData, 0);


	//=======================================================================
	// 10. ��������DM�� �ѱǼ��� �����Ѵ�.
	//=======================================================================
	strData.Empty();
	nVolCnt = m_pFormApi->GetVolumeCount(pBookDM);
	strData.Format(_T("%d"), nVolCnt);
	m_pDM_Accu->SetCellData(_T("BP_�ѱǼ�"), strData, 0);
	
	//=======================================================================
	// 11. ��������M�� ��������_�ڵ带 �����Ѵ�.
	//=======================================================================
	m_pDM_Accu->SetCellData(_T("BP_��������_�ڵ�"), pSpeciesDM->GetCellData(_T("BS_��������_�ڵ�"), 0), 0);

	//=======================================================================
	// 2005-02-19 
	// 12. �����߱��� �� Species_key�� �ʿ��ϹǷ� Species_Key�� Set���ش�.
	//=======================================================================
	m_pDM_Accu->SetCellData(_T("BP_��KEY"), m_szSpeciesKey, 0);

	return 0;
}

INT CSpecies_Form::GetPriceInfo(CString strSrcData, CString &strCurrencyCode, CString &strPrice)
{
	TCHAR * strData = strSrcData.GetBuffer(0);
	INT cnt = strSrcData.GetLength();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strData[i] >= '0' && strData[i] <= '9')
			break;
	}
	
	strCurrencyCode = strSrcData.Mid(0, i);
	strPrice = strSrcData.Mid(i, cnt);
	return 0;
}

INT CSpecies_Form::MakeDupAddMode()
{
	if(m_strDupIndexKey.IsEmpty()) return -1;

	if(m_pDM_Species->GetCellData(_T("BS_��������"), 0) == _T("Y") || m_bDupAddYN || m_bDupAddMode)
	{
		ESLAfxMessageBox(_T("�ش��ڷ�� �����߱� �� �� ���� �����Դϴ�.\n���� ���Ե� å�� �ڷ����/�����߱� ���θ� Ȯ���Ͻʽÿ�."));
		return 0;
	}

	m_bDupAddMode = TRUE;

	ApplyData();

	CString strQueryFlag;
	INT nRowCount = m_pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));

		m_pDM_Book->SetCellData(_T("BB_��KEY"), m_strDupIndexKey, idx);
		m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		m_pDM_Book->SetCellData(_T("BB_��ǥå����"), _T("N"), idx);
	}		

	// �� DM, Index DM�� Ű�� ������ �����Ѵ�. ������ �������� ������ ���� �Ǿ�� �Ѵ�.
	CString strOption, strWorkCode, strValue;
	strWorkCode = m_pDM_Index->GetCellData(_T("IBS_��������_�ڵ�"), 0);

	strOption.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s"), m_strDupIndexKey);
	m_pDM_Species->GetOneValueQuery(strOption, strValue);
	if(strValue == _T("0"))
	{
		ESLAfxMessageBox(_T("�����Ͻ� �ڷ�� �����߱�� �ڷ��Դϴ�. �� ������ ������ �� �����ϴ�."));
		m_bDupAddMode = FALSE;
		return 0;
	}
	
	strOption.Format(_T("REC_KEY = %s"), m_strDupIndexKey);
	m_pDM_Species->RefreshDataManager(strOption);
	m_pDM_Index->RefreshDataManager(strOption);		

	if(m_pDM_Species->GetRowCount() == 0 || m_pDM_Index->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�����Ͻ� �ڷᰡ �ùٸ��� �ʽ��ϴ�."));
		m_bDupAddMode = FALSE;
		EndDialog(IDCANCEL);
		return 0;
	}
	
	if(strWorkCode != m_pDM_Index->GetCellData(_T("IBS_��������_�ڵ�"), 0))
	{
		strWorkCode = m_pDM_Index->GetCellData(_T("IBS_��������_�ڵ�"), 0);
		m_pCM_Index->GetControlMgrData(_T("IBS_��������_�ڵ�"), strWorkCode);
		INT nWorkCode = _ttoi(strWorkCode.GetBuffer(0));
		::PostMessage(m_pHeadDlg->GetSafeHwnd(), CHANGE_WORK_CODE, nWorkCode, 0);		
	}
	

	//��ũ�� �����Ͽ� ���ΰ� ���������� ��ũ���� ������ �����Ѵ�.
	CMarc marc;
	CString strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu);
	
	// ȭ�� �����ֱ�
	Display();
	
	return 0;
}

INT CSpecies_Form::Display()
{
	// ��-�����߱�
	CStringArray ArrIgnoreAlias;
	
	if ( m_bDupAddMode == TRUE || m_bDupAddYN == TRUE )
	{
		ShowRegCodeField();
		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		if (nVolumeCount == 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
			SetEBookCtrl(TRUE);
		}
		else if (nVolumeCount > 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
			SetEBookCtrl(FALSE);
		}
		
		// 2008/04/22 REM BY PDJ
		// ������ �� �ֵ��� Ǯ��޶�
		//m_pCM_Index->AllControlReadOnly();
		//20070816 ADD PJW : ��û���� ReadOnly<FALSE,TRUE ����>
		//m_hApplicantTime->Control_SetReadOnly(TRUE);

		//��ϱ��� ��Ʈ��  ReadOnly�ȵǰ�
		((CEdit*)GetDlgItem(IDC_eREG_CLASS_CODE))->SetReadOnly(FALSE);

		
		//ȭ������� �Ⱥ��̰�
		GetDlgItem(IDC_bCLEARSCREEN)->ShowWindow(SW_HIDE);
		
		m_pCM_Accu->ControlDisplay(_T("BP_��������_�ڵ�"), 0);
	}
	else 
	{
		// �Է��� ��� ȭ������� ���̰�
		if( this->m_nOpType == SINSERT )	GetDlgItem(IDC_bCLEARSCREEN)->ShowWindow(SW_SHOW);
		else								GetDlgItem(IDC_bCLEARSCREEN)->ShowWindow(SW_HIDE);

		m_pFormApi->AllControlClear(m_pCM_Index, &m_arrIgnoreCTRLAlias);
		m_pFormApi->AllControlClear(m_pCM_Accu, &m_arrIgnoreCTRLAlias);
		m_pTailDlg->AllControlClear();
	}
	
	// ��-�Է�
	if( m_nOpType == SINSERT )
	{
		ShowRegCodeField();
		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		CString strVolumeCount;
		strVolumeCount.Format(_T("%d"), nVolumeCount);
		m_pCM_Accu->SetControlMgrData(_T("BP_�ѱǼ�"), strVolumeCount);
		
		if (nVolumeCount == 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
			SetEBookCtrl(TRUE);
		}
		else if (nVolumeCount > 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
			SetEBookCtrl(FALSE);
		}
		
		ApplyRepresentBookDataToIndex(m_pDM_Book, m_pDM_Index, m_pCM_Index);
	}
	// ����-��-����-����
	else if( m_nOpType != SINSERT )
	{
		CString strSubjectCode;
		m_pDM_Accu->GetCellData(_T("BP_��������_�ڵ�"), 0, strSubjectCode);
		//====================================================
		//2008.07.08 ADD BY PJW : ���������� ��������ϰ�� ��ũ�� �־��ش�.
		if( strSubjectCode.IsEmpty() )
		{
			m_pCM_Accu->SetControlMgrData(_T("BP_��������_�ڵ�"), _T("  "));
		}
		else
		{
			m_pCM_Accu->SetControlMgrData(_T("BP_��������_�ڵ�"), strSubjectCode);
		}
		//====================================================
		
		ShowRegCodeField();
		// BEGIN:
		// 2004.10.15 5163_1. �켱�����ڷ����� DM���� �����ͼ� ȭ�鿡 �����ش�.
		if(m_strPriorityCatalogYN == _T("Y"))
		{
			// 2005-01-24 ����, ������ϴ�� �߰�.
			if( m_nSpeciesType == BO_PURCHASE || m_nSpeciesType == BO_DONATE || m_nSpeciesType == BO_ORDER || m_nSpeciesType == BO_MISSING || (m_nAcqType == PURCHASE && m_nSpeciesType == BO_REGOBJ) || (m_nAcqType == DONATE && m_nSpeciesType == BO_REGOBJ) ||
				m_nSpeciesType == BO_REG_PURCHASE || m_nSpeciesType == BO_REG_DONATE || m_nSpeciesType == BO_REG_INVALID || m_nSpeciesType == BO_CAT)
			{
				CString strCheck = _T("");
				CButton *b1 = (CButton*)GetDlgItem(IDC_chkPRIORITY_CATALOG);
				m_pDM_Species->GetCellData(_T("BS_�켱�����ڷ�"), 0, strCheck);
				if(strCheck == 'Y') b1->SetCheck(1);
				else b1->SetCheck(0);
			}
		}
		// 2004.10.15 5163_3. ����� ���� �ڵ� ��� ����
		if(m_strUseCodeYN == _T("Y"))
		{
			CString strCode;
			m_pDM_Species->GetCellData(_T("BS_�����з��ڵ�1"), 0, strCode);
			m_pCM_Index->SetControlMgrData(_T("IBS_�����з��ڵ�1"), strCode);
			strCode.Empty();
			m_pDM_Species->GetCellData(_T("BS_�����з��ڵ�2"), 0, strCode);
			m_pCM_Index->SetControlMgrData(_T("IBS_�����з��ڵ�2"), strCode);
			strCode.Empty();
			m_pDM_Species->GetCellData(_T("BS_�����з��ڵ�3"), 0, strCode);
			m_pCM_Index->SetControlMgrData(_T("IBS_�����з��ڵ�3"), strCode);
		}
		// END:
		// 2005-02-04 5163�� ��� �ٸ� ��� EditBox�� ������� �Ѵ�.
		if(_T("Y") == m_strIs5163)
		{
			CString strRemark;
			m_pDM_Species->GetCellData(_T("BS_���"), 0, strRemark);
			GetDlgItem(IDC_eDISCRIPTION_SMALL)->SetWindowText(strRemark);
		}

		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		CString strVolumeCount, strPriceType;
		strVolumeCount.Format(_T("%d"), nVolumeCount);
		m_pCM_Accu->SetControlMgrData(_T("BP_�ѱǼ�"), strVolumeCount);
		
		if (nVolumeCount == 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
			SetEBookCtrl(TRUE);
		}
		else if (nVolumeCount > 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
			SetEBookCtrl(FALSE);
		}

		CString strTmpAuthor = m_pDM_Index->GetCellData(_T("IBS_������"), 0);
		
		ApplyRepresentBookDataToIndex(m_pDM_Book, m_pDM_Index, m_pCM_Index);
		strTmpAuthor = m_pDM_Index->GetCellData(_T("IBS_������"), 0);
		// �������� ���� - ��û����/��û�ڵ�
		// 2005-09-28 ������
		//
		SetPurchaseInfo();
		strTmpAuthor = m_pDM_Index->GetCellData(_T("IBS_������"), 0);
	}
	else		return -1;

	m_pHeadDlg->Display(0);
	m_pTailDlg->Display(0);
	
	ShowControlByAcqType();
	//==============================================================
	//20080613 REM BY PJW : ���Խ�û�ڰ� ������� ��¥ �ð� ��� NULL�� ���������� �ʿ� ������ �ڵ�
	//20070813 ADD PJW : ���Խ�û�ڰ� ������ NULL ������ �ú��� ����
	//CString strApplicantName, sTime, strApplicantData;
	//m_pCM_Index->GetControlMgrData(_T("���Խ�û��"), strApplicantName, 0);
	//if ( strApplicantName == _T(""))
	//{
	//	sTime.Format(_T("  :  :  "));
	//	strApplicantTime(sTime);
	//	
	//}
	//==============================================================

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - DM�� �⺻������ �����Ѵ�.
// [ �̴Ͻ��� ]
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::LoadDefaultValue()
{
	//=======================================================================
	// ���� ����
	//=======================================================================
	INT i, ids;
	CString strFormCode, strData, strPath, strMarc, strWorkCode;
	CMarc marc;


	//=======================================================================
	// 1. ��ϱ��� ����Ʈ �ڽ��� �⺻���������� �����Ѵ�.
	//=======================================================================	
	InitRegCodeField();		// ��ϱ��п��� ���� ������ ������ �־�� ��.
	
	// 2005-02-21 
	// ��ϱ��а��� ���� �� ���� ApplyData() �����Ͽ��� ��.
	ApplyData();

	// ��ϱ���, å�η��÷��׸� å���� �ݿ��ؾ� �մϴ�.
	for( i=0 ; i<m_pDM_Book->GetRowCount() ; i++ )
	{
		m_pDM_Book->SetCellData(_T("BB_��ϱ���_�ڵ�"), m_strRegCodeField, i);
		m_pDM_Book->SetCellData(_T("BB_å�η��÷���"), _T("B"), i);
	}
	
	m_pCM_Index->GetControlMgrData(_T("IBS_���ı���_�ڵ�"), strFormCode);
	if( strFormCode.IsEmpty() == TRUE )		m_pInfo->pCodeMgr->GetDefaultCode(_T("���ı���"), strFormCode);

	strData = strFormCode + _T(".cfg");
	strPath = m_strDefaultValueFilePath;
	strPath.Replace(_T(".cfg"), strData);

	
	ids = m_pFormApi->LoadDefaultValue(strMarc, strPath);
	if( ids < 0 )
	{
		CString strMsg;
		strMsg.Format(_T("���ı��� : %s�� �⺻���� �����Ǿ� ���� �ʽ��ϴ�."), strFormCode);
		ESLAfxMessageBox(strMsg);
//		OnbCLEARSCREEN();
		return 0;
	}
	
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);

	m_pFormApi->AllControlDisplay(m_pCM_Index, &m_arrIgnoreCTRLAlias, 0);
	m_pFormApi->AllControlDisplay(m_pCM_Accu, &m_arrIgnoreCTRLAlias, 0);

	m_pTailDlg->Display(0);
	ShowRegCodeField();
	
	
	m_pCM_Index->GetControlMgrData(_T("IBS_��������_�ڵ�"), strWorkCode);
	m_pDM_Accu->SetCellData(_T("BP_������������"), strWorkCode, 0);
	//������������ ����
	m_pHeadDlg->Display(0);
	//��Ŀ���� ����
	GetDlgItem(IDC_eTITLE)->SetFocus();
	return 0;
}

INT CSpecies_Form::SaveDefaultValue()
{
	CString strFormCode, strData, strPath;
	m_pCM_Index->GetControlMgrData(_T("IBS_���ı���_�ڵ�"), strFormCode);
	if(strFormCode.IsEmpty())
		m_pInfo->pCodeMgr->GetDefaultCode(_T("���ı���"), strFormCode);
	strData = strFormCode + _T(".cfg");
	strPath = m_strDefaultValueFilePath;
	strPath.Replace(_T(".cfg"), strData);
	m_pFormApi->SaveDefaultValue(m_pCM_Index, strPath);
	m_pFormApi->SaveDefaultValue(m_pDM_Accu, strPath);
	return 0;
}

INT CSpecies_Form::ShowRegCodeField()
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
		m_strRegCodeField = strRegCodeField;

		// �� å��
		CString strBookCount;
		strBookCount.Format(_T("%d"), nBookCount);
		m_pCM_Accu->SetControlMgrData(_T("BP_��å��"), strBookCount);
		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		CString strVolumeCount;
		strVolumeCount.Format(_T("%d"), nVolumeCount);
		m_pCM_Accu->SetControlMgrData(_T("BP_�ѱǼ�"), strVolumeCount);
	}

	return 0;
}

UINT CSpecies_Form::OnPurchaseSeqNoChange(LPARAM lParam, WPARAM wParam)
{
	CString strTmpData;

	if( lParam == 0 )	strTmpData = _T("");
	else strTmpData.Format(_T("%d"), lParam);

	m_pCM_Index->SetControlMgrData(_T("IBS_��������_�ڵ�"), strTmpData);
	
	return 0;
}

UINT CSpecies_Form::OnWorkingStatusChange(LPARAM lParam, WPARAM wParam)
{
	SPECIES_TYPE nSpeciesType = (SPECIES_TYPE)lParam;
	CEdit* pCtrl = NULL;

	pCtrl = (CEdit*)GetDlgItem(IDC_edtWORKING_STATUS);
	if(pCtrl == NULL) return -1;
	if( nSpeciesType == BO_DONATE )
	{
		m_strWorkingStatus = _T("BOA211O");
		//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("���������ڷ�"));
		pCtrl->SetWindowText(_T("���������ڷ�"));
	}
	else if( nSpeciesType == BO_NONDONATE )
	{
		m_strWorkingStatus = _T("BOA212O");
		//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), _T("�����̵���ڷ�"));
		pCtrl->SetWindowText(_T("�����̵���ڷ�"));
	}

	return 0;
}

UINT CSpecies_Form::OnAcqTypeChange(LPARAM lParam, WPARAM wParam)
{
	CString strAcqCode, strWorkCode;
	strAcqCode.Format(_T("%d"), lParam);

	ACQ_TYPE nAcqType = m_pHeadDlg->GetAcqType();
	if(m_nAcqType == OTHER_ACQ_TYPE && nAcqType == OTHER_ACQ_TYPE) m_bAcqTypeChange = TRUE;
	else if(m_nAcqType == nAcqType ) return 0;
	else if(m_nAcqType == ACQ_TYPE_INVALID && nAcqType != ACQ_TYPE_INVALID) m_bAcqTypeChange = TRUE;
	else if(m_nAcqType != ACQ_TYPE_INVALID && nAcqType == ACQ_TYPE_INVALID) m_bAcqTypeChange = TRUE;
	else m_bAcqTypeChange = FALSE;

	m_nAcqType = nAcqType;
	if(m_nAcqType == PURCHASE) m_nSpeciesType = BO_REG_PURCHASE;
	else if(m_nAcqType == DONATE) m_nSpeciesType = BO_REG_DONATE;
	else if(m_nAcqType == ACQ_TYPE_INVALID || m_nAcqType == OTHER_ACQ_TYPE) m_nSpeciesType = BO_REG_INVALID;
	SetAccuDataManager();
	SetWorkingStatus();
	if(m_bCreateGroupCollection)
		SetParentGroupInfo((INT)m_nAcqType, m_pBOSpeciesApi->GetTodayYear(), _T(""), m_pDM_Index->CONNECTION_INFO);
	else 
	{
		m_pGroupCollection->RemoveAll();
		m_pGroupCollection->SetAcqType(m_nAcqType);
		m_pGroupCollection->SetWorkingStatus(_T("�˼��Ϸ�"));
	}
	m_pGroupCollection->SetAcqCode(strAcqCode);
	m_pGroupCollection->GetAcqType();
	SetHeadTailInfoDlg();
	ShowControlBySpeciesType();
	ShowControlByAcqType();
	OnSelchangecCLASIFYCLASS();
	m_pTailDlg->Display(0);
	
	return 0;
}

VOID CSpecies_Form::OnbtnBSFORMSHOWHIDECODE() 
{
	if( m_bShowCode == TRUE )	m_bShowCode = FALSE;
	else						m_bShowCode = TRUE;

	ShowCodeResource();	
}


VOID CSpecies_Form::OnKillfocuseTITLE() 
{
	// TODO: Add your control notification handler code here
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_eTITLE);
	if (pEdit == NULL) return;

	// ���� �¸��̸� üũ���� �ʴ´�.
	if (pEdit->GetStyle() & ES_READONLY) return;

	SetTimer(0, 0, NULL);		
}

VOID CSpecies_Form::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	// ���� �ڵ���������
	if (nIDEvent == 0)
	{
		KillTimer(0);

		if (m_bAutoDupSearch == FALSE) return;
		if(m_pNoteCtrl)
		{
			if(m_pNoteCtrl->GetSafeHwnd())
				return;
		}

		/*************************************************
		 *************************************************/
		//20080508 ADD BY PJW : K2UP���� �ٸ� ���α׷����� ��Ŀ���� �Űܰ���� return�Ѵ�.
		CWnd * pWnd;
		pWnd = this->GetFocus();
		if( pWnd == NULL )
		{
			return;
		}
		/*************************************************
		 *************************************************/

		CString strSearchCount;

		SearchDuplicateDataByTitle(_T("�����ڵ���������"), strSearchCount);

		if (strSearchCount.IsEmpty() || strSearchCount == _T("0"))	return;
				
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

		CRect rcWindow;
		GetWindowRect(rcWindow);
		
		CPoint point;
		GetCursorPos(&point);
		
		if(point.y > rcWindow.bottom-25)
			point.y = rcWindow.bottom-25;
		else if(point.y < rcWindow.top+25)
			point.y = rcWindow.top+25;
		if(point.x > rcWindow.right-200)
			point.x = rcWindow.right-200;
		else if(point.x < rcWindow.left+200)
			point.x = rcWindow.left+200;

		ScreenToClient(&point);
		point.y += 5;
		
		m_pNoteCtrl->Create(point, 200, 25, this);
		m_pNoteCtrl->ShowWindow(SW_SHOW);
	}//20070522 ADD PJW : ���� ��Ŀ�� 
	else if (nIDEvent == 3)
	{
		KillTimer(3);
		GetDlgItem(IDC_eTITLE)->SetFocus();
	}
		
	CDialog::OnTimer(nIDEvent);
}

VOID CSpecies_Form::SearchDuplicateDataByTitle(CString strSearchType, CString &strSearchCount)
{
	CString strTitle, strTmpData;

	m_pCM_Index->GetControlMgrData(_T("IBS_��ǥ��"), strTitle);
	
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

	strTmpData = strTitle;
	((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
	if(strTmpData.IsEmpty()) return;
	
	// ��������϶� �ڽ��� ��Ű�� �����´�.
	CString strSpeciesKey;
	if (m_nOpType != SINSERT && m_nParentSelectedRow >= 0)
		m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, m_nParentSelectedRow, strSpeciesKey);

	CDuplicateSearch_Interface impl(this);
	INT ids = impl.ShowOnlyTitleDuplicateSearch(strSearchType, strTitle, strSearchCount, strSpeciesKey);
	if (ids < 0) return;
}

VOID CSpecies_Form::OnbMARCEDIT() 
{
	INT ids = -1;
	CString strMarc, strLeader;
	CMarc marc;

	ids = m_pDM_Species->GetCellData(_T("BS_MARC"), 0, strMarc);
	if( strMarc.IsEmpty() == FALSE )
	{
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if( ids < 0 )	return;
	}

	// ���� ���� ���� (����߰���)
	marc.GetLeader(strLeader);
	// ���ڵ� ���� ������ �ִ´�. 5��°
	if( strLeader.GetAt(5) == ' ' )		strLeader.SetAt(5, 'n');
	// ���ڵ� ���� ������ �ִ´�. 6��°
	if( strLeader.GetAt(6) == ' ' )		strLeader.SetAt(6, 'a');
	// �������� ������ �ִ´�. 7��°
	if( strLeader.GetAt(7) == ' ' )		strLeader.SetAt(7, 'm');
	// ������ ������ �����Ѵ�.
	marc.SetLeader(strLeader.GetBuffer(0));


	// �����߱� ����ϰ��� ��ũ���⸸ �ȴ�.
	if( m_bDupAddMode == TRUE || m_bDupAddYN == TRUE )
	{
		CBOMarcEditDlg dlg(this, &marc);
		dlg.SetReadOnly();
		dlg.DoModal();
	}
	else
	{
		ids = ApplyData();
		if( ids < 0 )	return;

		//������ ��ũ�� �����ÿ� �������� �ƴѰ�� 100Tag�� ������ؾ� �Ѵ�.
		if( m_pDM_Index->GetCellData(_T("IBS_�۾�����INFO"), 0).Left(3) != _T("BOC") && 
			m_pDM_Index->GetCellData(_T("IBS_�۾�����INFO"), 0).Left(3) != _T("BOL") )
		{
			m_pInfo->pMarcMgr->DeleteField(&marc, _T("100"));
			m_pInfo->pMarcMgr->DeleteField(&marc, _T("700"));
		}
		//2006.12.08 Ȯ�尳�� : 008���� �״�� �� ����
		CString s008;
		m_pInfo->pMarcMgr->GetItem(&marc,_T("008*"),s008);
		// �� -> ��ũ
		ids = m_pBOSpeciesApi->APPLY_Species_to_CMarc(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
		m_pInfo->pMarcMgr->CheckDeepSyntax(&marc);
		m_pInfo->pMarcMgr->CheckSimpleSyntax(&marc);
		m_pInfo->pMarcMgr->DeleteEmptySubfield(&marc);
		m_pInfo->pMarcMgr->DeleteEmptyField(&marc);
		
		m_pInfo->pMarcMgr->SetItem(&marc,_T("008*"),s008);

		
		// ��Ʈ ���� ���̾�α� ����
		CBOMarcEditDlg dlg(this, &marc);
		dlg.SetReadOnly(IsHoldDataMarc(m_pDM_Book));
		if(IDOK != dlg.DoModal()) return;
		
		// ��ũ -> ��
		ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
		
		// �� Marc��
		m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
		m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
		
		Display();
	}	
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
//
// [ �̴Ͻ��� ]
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::ApplyData()
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT ids, nIdx, nRowCount, i;
	CString strCode, strVol, strQueryFlag, strApplicantCode, strRemark, strMsg;
	CButton* b1 = (CButton*)GetDlgItem(IDC_chkPRIORITY_CATALOG);


	//=======================================================================
	// 1. ����Ÿ���� SINSERT, SUPDATE, GUPDATE��� �����׸���� üũ�Ͽ� �ش� DM�� �����Ѵ�.
	//		- i.	�켱�����ڷ�	:	üũ�ڽ� -> DMUP_����_��_������
	//		- ii.	����������ڵ�	:	DMUP_����_��_�������� -> DMUP_����_��_������ (��DM)
	//		- iii.	����CM -> ����DM	CMUP_����_��_�������� -> DMUP_����_��_�������� (����DM)
	//		- iv.	����DM(IBS_�����ڷᱸ��_�ڵ�) -> ��DM(BS_�����ڷᱸ��)
	//		- v.	5163��뿩��	:	��DM - BS_���
	//		- vi.	��������CM -> ��������DM
	//=======================================================================
	if( m_nOpType == SINSERT || m_nOpType == SUPDATE || m_nOpType == GUPDATE )
	{
		// i.
		// 2004.10.14 5163_1. �켱�����ڷ� üũ���޾Ƽ� DM�� �����ϱ�.
		// BEGIN:
		if( m_strPriorityCatalogYN == _T("Y") )
		{
			nIdx = 0;
			nIdx = b1->GetCheck();
			if( nIdx == 1 )		m_pDM_Species->SetCellData(_T("BS_�켱�����ڷ�"), 'Y', 0);
			else m_pDM_Species->SetCellData(_T("BS_�켱�����ڷ�"), 'N', 0);
		}

		// ii.
		// ����������ڵ� ��뿩�θ� üũ�Ͽ� ����DM->��DM���� �����Ѵ�.
		if( m_strUseCodeYN == _T("Y") )
		{
			m_pCM_Index->GetControlMgrData(_T("IBS_�����з��ڵ�1"), strCode);
			m_pDM_Species->SetCellData(_T("BS_�����з��ڵ�1"), strCode, 0);
			
			strCode.Empty();
			m_pCM_Index->GetControlMgrData(_T("IBS_�����з��ڵ�2"), strCode);
			m_pDM_Species->SetCellData(_T("BS_�����з��ڵ�2"), strCode, 0);
			
			strCode.Empty();
			m_pCM_Index->GetControlMgrData(_T("IBS_�����з��ڵ�3"), strCode);
			m_pDM_Species->SetCellData(_T("BS_�����з��ڵ�3"), strCode, 0);
		}
		// END:

		// iii.
		// ���� CM -> ���� DM				
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Index, 0);
		if( ids < 0 ) 
		{
			strMsg.Empty();
			strMsg.Format( _T("���� ������ ������ �� �����ϴ�.\n[%s -> %s]"), m_pCM_Index->GetAlias(), m_pDM_Index->GETDMAlias() );
			ESLAfxMessageBox( strMsg ) ;
			//return -1;
		}		
		//20070810 ADD PJW : ���Խ�û���� �׸��� �ð��� ����
		//20070827 ADD PJW : ���Խ�û�ڸ� �Է����� ��� ����ó�� �߰� ���Խ�û���� 

		CString sDate, sTime, strApplicantDate;
		m_pCM_Index->GetControlMgrData(_T("���Խ�û��"), sDate, 0);
		m_hApplicantTime->GetData(sTime);
		if( sDate == _T("") )
		{
			m_pDM_Index->SetCellData(_T("���Խ�û��"), _T(""), 0);
		}
		else
		{
			strApplicantDate.Format(_T("%s %s"),sDate, sTime );
			m_pDM_Index->SetCellData(_T("���Խ�û��"), strApplicantDate, 0);
		}
		//strApplicantDate.Format(_T("%s %s"),sDate, sTime );
		//m_pDM_Index->SetCellData(_T("���Խ�û��"), strApplicantDate, 0);
		//GetDlgItem(IDC_edtBSFORM_REQUESTOR)->SetWindowText(_T(""));
		// iv.
		// �����ڷᱸ��(���)�� -> ���� -> ������ �ݿ��Ǿ�� �Ѵ�.
		m_pDM_Species->SetCellData(_T("BS_�����ڷᱸ��"), m_pDM_Index->GetCellData(_T("IBS_�����ڷᱸ��_�ڵ�"), 0), 0);
		//m_pDM_Species->SetCellData(_T("BS_���"), m_pDM_Index->GetCellData(_T("IBS_���"), 0), 0);

		// v.
		// 2005-02-04 5163�� ��쿡�� ���� �������� ���� �����;� �Ѵ�.
		if( _T("Y") == m_strIs5163 )
		{
			GetDlgItem(IDC_eDISCRIPTION_SMALL)->GetWindowText(strRemark);
			m_pDM_Index->SetCellData(_T("IBS_���"), strRemark, 0);
			m_pDM_Species->SetCellData(_T("BS_���"), strRemark, 0);
		}
		else	m_pDM_Species->SetCellData(_T("BS_���"), m_pDM_Index->GetCellData(_T("IBS_���"), 0), 0);

		// vi.
		if( m_pDM_Accu != NULL )
		{
		// �������� CM -> �������� DM
			ids = m_pFormApi->MakeDMFromCM(m_pCM_Accu, m_pDM_Accu, 0);
			if( ids < 0 )
		{
				strMsg.Empty();
				strMsg.Format(_T("���� ������ ������ �� �����ϴ�.\n[%s -> %s]"), m_pCM_Accu->GetAlias(), m_pDM_Accu->GETDMAlias());
				ESLAfxMessageBox( strMsg );
				return -1;
			}
			
			//���� ����/������������ ���� �Ѵ�.
			m_pDM_Accu->SetCellData(_T("BP_���"), m_pDM_Index->GetCellData(_T("IBS_���"), 0), 0);

			//=====================================================
			//2008.11.14 ADD BY PJW : �����������̺� �������� �߰��Ѵ�.
			CString strManageCode;
			strManageCode = m_pInfo->MANAGE_CODE;
			m_pDM_Accu->SetCellData(_T("BP_��������"), strManageCode, 0);
			//=====================================================


			//��������.. ���� �߰�
			//���� ���� �۾�
			// ������ ��û������ DM�� �����Ѵ�.
			// 2005-09-29 ������
			nIdx = m_cmbApplicant.GetCurSel();
			switch( nIdx )
			{
			case 1:	// 1 - �μ�
				{
					m_pCM_Index->GetControlMgrData(_T("���Խ�û����_�μ�"), strApplicantCode);
					// ����-�������� ����
					m_pDM_Index->SetCellData(_T("���Խ�û����_�μ�"), strApplicantCode, 0);	
					m_pDM_Index->SetCellData(_T("���Խ�û����_�к�"), _T(""), 0);	
					m_pDM_Index->SetCellData(_T("���Խ�û����_�а�"), _T(""), 0);	
					// ����-�������� ����
					m_pDM_Accu->SetCellData(_T("BP_��û����"),_T("CLA"),0);
					m_pDM_Accu->SetCellData(_T("BP_�μ��ڵ�"), strApplicantCode, 0);
					m_pDM_Accu->SetCellData(_T("BP_�к��ڵ�"),_T(""),0);
					m_pDM_Accu->SetCellData(_T("BP_�а��ڵ�"),_T(""),0);
				}
				break;
			case 2:	// 2 - �а�
				{
					m_pCM_Index->GetControlMgrData(_T("���Խ�û����_�а�"), strApplicantCode);
					// ����-�������� ����					
					m_pDM_Index->SetCellData(_T("���Խ�û����_�μ�"), _T(""), 0);	
					m_pDM_Index->SetCellData(_T("���Խ�û����_�к�"), _T(""), 0);	
					m_pDM_Index->SetCellData(_T("���Խ�û����_�а�"), strApplicantCode, 0);	
					// ����-�������� ����
					m_pDM_Accu->SetCellData(_T("BP_��û����"),_T("COU"),0);
					m_pDM_Accu->SetCellData(_T("BP_�μ��ڵ�"), _T(""), 0);
					m_pDM_Accu->SetCellData(_T("BP_�к��ڵ�"),_T(""),0);
					m_pDM_Accu->SetCellData(_T("BP_�а��ڵ�"),strApplicantCode,0);
				}
				break;
			case 3:	// 3 - ��ó
				{
					m_pCM_Index->GetControlMgrData(_T("���Խ�û����_�к�"), strApplicantCode);
					// ����-�������� ����					
					m_pDM_Index->SetCellData(_T("���Խ�û����_�μ�"), _T(""), 0);	
					m_pDM_Index->SetCellData(_T("���Խ�û����_�к�"), strApplicantCode, 0);	
					m_pDM_Index->SetCellData(_T("���Խ�û����_�а�"), _T(""), 0);	
					// ����-�������� ����
					m_pDM_Accu->SetCellData(_T("BP_��û����"),_T("DEP"),0);
					m_pDM_Accu->SetCellData(_T("BP_�μ��ڵ�"), _T(""), 0);
					m_pDM_Accu->SetCellData(_T("BP_�к��ڵ�"),strApplicantCode,0);
					m_pDM_Accu->SetCellData(_T("BP_�а��ڵ�"),_T(""),0);
				}
				break;

			case 0:	// 0 - ȸ��
			default:
				{
					// ����-�������� ����
					m_pDM_Index->SetCellData(_T("���Խ�û����_�μ�"), _T(""), 0);	
					m_pDM_Index->SetCellData(_T("���Խ�û����_�к�"), _T(""), 0);	
					m_pDM_Index->SetCellData(_T("���Խ�û����_�а�"), _T(""), 0);	
					// ����-�������� ����
					m_pDM_Accu->SetCellData(_T("BP_��û����"),_T(""),0);
					m_pDM_Accu->SetCellData(_T("BP_�μ��ڵ�"),_T(""),0);
					m_pDM_Accu->SetCellData(_T("BP_�к��ڵ�"),_T(""),0);
					m_pDM_Accu->SetCellData(_T("BP_�а��ڵ�"),_T(""),0);
				}
				break;
			}
			//--�̻�.--
		}

		// �����/���Ϸù�ȣ
		m_pCM_Index->GetControlMgrData(_T("IBS_�����"), strVol);
	}


	//=======================================================================
	// 2. ����Ÿ�Կ� ���� ���� �۾��� �����Ѵ�.
	// [ �Է�(SINSERT) ]
	//		- i.	�����÷��׿� ������� �̿��Ͽ� ǥ��å������ �����.
	//		- ii.	�����߱��尡 �ƴϸ� ��ǥå������ �����Ѵ�.
	//		- iii.	�ܱ��϶� ��ȭ�� ���� ��������ش�.
	//=======================================================================
	switch( m_nOpType )
	{
	// ��-�Է�
	case SINSERT:
	{
		// �ܱ��϶� �Էµ� ������� ���� ������� �ٸ��� ��� å�� �����Ѵ�.
		// 2005-03-18 �ּ�ó��.
		/*
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
		{
			if (m_pDM_Book->GetCellData(_T("BB_�����"), 0) != strVol)
			{
				m_pDM_Book->FreeData();
				m_pDM_Book->InsertRow(-1);
			}
		}
		*/

			// i.
			// ���� �÷��� ����
			strQueryFlag = m_pFormApi->GetQueryFlag(MODE_INSERT);

		// ǥ��å ���� �����
			ids = MakeStandardBookFromIdx(strVol, strQueryFlag);
			if( ids < 0 )
			{
				ESLAfxMessageBox( _T("ǥ��å������ ������ �� �����ϴ�.") );
				return -1;
			}

			// ii.
			// �����߱� ��尡 �ƴϸ� ��ǥå �ϳ��� �����.
			if( m_bDupAddMode == FALSE )	
			{
				ids = m_pFormApi->SetRepresentationBook(m_pDM_Book);
				if( ids < 0 )
				{
					strMsg.Empty();
					strMsg.Format( _T("��ǥå������ ������ �� �����ϴ�.\n[%s]"), m_pDM_Book->GETDMAlias());
					ESLAfxMessageBox( strMsg );
					return -1;
				}
			}

			// iii.
			/// �ܱ��϶� ��ȭ�� ���� ������� �ְ� �ٱ��� ��� ��ǥå�� �̿��Ͽ� åDM�� �����Ѵ�.
			if( m_pFormApi->GetVolumeCount(m_pDM_Book) == 1 )	ApplyOneVolumeData(strVol, strQueryFlag);
			else												ApplyRepresentVolumeData(_T(""), strQueryFlag);

		// Working Status ����
		ApplyWorkingStatus();
		ApplyAppendixWorkingStatus();
		
		//��ǥå�� �ڵ尪�� ���� �ڵ尪���� ��������ش�.
		ApplyRepresentBookDataToSpecies(m_pDM_Book, m_pDM_Species);

		m_pHeadDlg->ApplyData();
		m_pTailDlg->ApplyData();

		ApplyAppendixDM();
	}
		break;

	// ��-����
	case SUPDATE:
	case GUPDATE:
	{
		// å DM RowCount
			nRowCount = m_pDM_Book->GetRowCount();
		// �ܱ��϶� ����� �ϰ� ����
			if( m_pFormApi->GetVolumeCount(m_pDM_Book) == 1 )
		{						
				for( i=0; i < nRowCount; i++ )	ids = m_pDM_Book->SetCellData(_T("BB_�����"), strVol, i);
		}

		/// ���� �÷���
			strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE);

		// �����߱� ��尡 �ƴϸ� ��ǥå ���� �����
			if( m_bDupAddMode == TRUE )//FALSE��������������?
		{
			// ��ǥå�� ã�´�.
				nIdx = -1;
			m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
			
			// ��ǥå ���� �����
			MakeStandardBookFromIdx(strVol, strQueryFlag, nIdx);
		}
				
			// �ܱ��϶� ��ȭ�� ���� ��������ش�.
			if( m_pFormApi->GetVolumeCount(m_pDM_Book) == 1 )	ApplyOneVolumeData(strVol, strQueryFlag);
			else												ApplyRepresentVolumeData(_T(""), strQueryFlag);

		// Working Status ����
		ApplyWorkingStatus();
		ApplyAppendixWorkingStatus();
		
		//��ǥå�� �ڵ尪�� ���� �ڵ尪���� ��������ش�.
		ApplyRepresentBookDataToSpecies(m_pDM_Book, m_pDM_Species);
		
		m_pHeadDlg->ApplyData();
		m_pTailDlg->ApplyData();
		
		ApplyAppendixDM();
	}
	default:		//return -1;
		break;
	}
	
	// �����߱� ����̸� ��Ű�� �������ش�.
	if( m_bDupAddMode == TRUE )
	{
		nRowCount = m_pDM_Book->GetRowCount();
		for( i = 0; i < nRowCount; i++ )
		{
			//strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), i));
			m_pDM_Book->SetCellData(_T("BB_��KEY"), m_strDupIndexKey, i);
			// m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, i);
			m_pDM_Book->SetCellData(_T("BB_��ǥå����"), _T("N"), i);
		}		
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���ڷ� �� idx��° åDM�� �����, ���Ա����ڵ�, DB_FLAG(�����÷���)�� �Է��Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. idx�� -1�̸� ù��° å�� ã�Ƽ� ǥ�������� ������ش�.
// 2. ���ڷ� �� strVol���� idx��° ������� �Է��Ѵ�.
// 3. idx��° ���Ա����ڵ忡 �Լ�Ÿ�԰��� �Է��Ѵ�.
// 4. idx��° DB_FLAG�� ���ڷο� strQueryFlag�� ���� �Է��Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::MakeStandardBookFromIdx(CString strVol, CString strQueryFlag, INT idx /*= -1*/)
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
	if(!m_bViewVolInfo && !m_bDupAddMode)
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Book, idx, _T("BB"));
	/// ����� ����
	if(m_pBOSpeciesApi->GetVolumeCount(m_pDM_Book) == 1)
		ids = m_pDM_Book->SetCellData(_T("BB_�����"), strVol, idx);
	//���Ա�������
	CString strTmpData;
	if(m_nAcqType == ACQ_TYPE_INVALID) strTmpData.Empty();
	else strTmpData.Format(_T("%d"), m_nAcqType);
	m_pDM_Book->SetCellData(_T("BB_���Ա���_�ڵ�"), strTmpData, idx);

	/// ���� �÷��� ����
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);

	return 0;
}

// 2004.09.15 
/*
INT CSpecies_Form::ApplyRepresentVolumeData(CString strVolSortNo, CString strQueryFlag)
{
	// ���������� ���� å ���� �����Ű��
	INT nBookCount = m_pDM_Book->GetRowCount();
	INT nReprentBookIdx = -1;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nReprentBookIdx);
	if(nReprentBookIdx < 0)
	{
		nReprentBookIdx = 0;
		m_pDM_Book->SetCellData(_T("BB_��ǥå����"), _T("Y"), nReprentBookIdx);
	}
	strVolSortNo = m_pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), nReprentBookIdx);
	if(strVolSortNo.IsEmpty()) strVolSortNo = _T("10");
	CString strTmpData;
	INT ids ;
	CStringArray ArrAccept;
	ArrAccept.Add(_T("IBS_��������_�ڵ�"));		ArrAccept.Add(_T("IBS_��ü����_�ڵ�"));
	ArrAccept.Add(_T("IBS_��ġ��ȣ_�ڵ�"));		ArrAccept.Add(_T("IBS_�̿����ѱ���_�ڵ�"));
	ArrAccept.Add(_T("IBS_�̿��󱸺�_�ڵ�"));	ArrAccept.Add(_T("IBS_�����"));
	ArrAccept.Add(_T("IBS_������Ư��"));		ArrAccept.Add(_T("IBS_�����ڷ�"));
	ArrAccept.Add(_T("IBS_ũ��"));				ArrAccept.Add(_T("IBS_�����"));

	CString strRepBookYN, strDBFlag;

	for (INT idx = 0; idx < nBookCount; idx ++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
		//���� ������ Ȯ���Ѵ�.
		strTmpData = m_pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), idx);
		if(strTmpData != strVolSortNo) continue;

		strDBFlag = m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		strRepBookYN = m_pDM_Book->GetCellData(_T("BB_��ǥå����"), idx);
		// ���� CM -> å DM - �ڵ� ������ �ݿ��Ǿ�� �Ѵ�.
		if(strDBFlag == _T("N") || strRepBookYN == _T("Y"))
			m_pFormApi->MakeDMFromCMInArray(m_pCM_Index, m_pDM_Book, &ArrAccept, idx, _T("BB"));

		// ���� �÷��� ����
		if(strDBFlag.IsEmpty())
			ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		if(strDBFlag == _T("N"))
			ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);
	}

	return 0;
}
*/

INT CSpecies_Form::ApplyRepresentVolumeData(CString strVolSortNo, CString strQueryFlag)
{
	// ���������� ���� å ���� �����Ű��
	INT nBookCount = m_pDM_Book->GetRowCount();
	INT nReprentBookIdx = -1;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nReprentBookIdx);
	if(nReprentBookIdx < 0)
	{
		nReprentBookIdx = 0;
		m_pDM_Book->SetCellData(_T("BB_��ǥå����"), _T("Y"), nReprentBookIdx);
	}
	strVolSortNo = m_pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), nReprentBookIdx);
	if(strVolSortNo.IsEmpty()) strVolSortNo = _T("10");
	CString strTmpData;
	INT ids ;
	CStringArray ArrAccept;
	ArrAccept.Add(_T("IBS_��������_�ڵ�"));		ArrAccept.Add(_T("IBS_��ü����_�ڵ�"));
	ArrAccept.Add(_T("IBS_��ġ��ȣ_�ڵ�"));		ArrAccept.Add(_T("IBS_�̿����ѱ���_�ڵ�"));
	ArrAccept.Add(_T("IBS_�̿��󱸺�_�ڵ�"));	ArrAccept.Add(_T("IBS_�����"));
	ArrAccept.Add(_T("IBS_������Ư��"));		ArrAccept.Add(_T("IBS_�����ڷ�"));
	ArrAccept.Add(_T("IBS_ũ��"));				ArrAccept.Add(_T("IBS_�����"));

	CString strRepBookYN, strDBFlag;

	for (INT idx = 0; idx < nBookCount; idx ++)
	{
		strDBFlag = m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		strRepBookYN = m_pDM_Book->GetCellData(_T("BB_��ǥå����"), idx);

		//���� ������ Ȯ���Ѵ�.
		strTmpData = m_pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), idx);
		if(strTmpData != strVolSortNo) continue;

		// ���� CM -> å DM - �ڵ� ������ �ݿ��Ǿ�� �Ѵ�.
		if(strDBFlag == _T("N") || strRepBookYN == _T("Y"))
			m_pFormApi->MakeDMFromCMInArray(m_pCM_Index, m_pDM_Book, &ArrAccept, idx, _T("BB"));
	}

	for (idx = 0; idx < nBookCount; idx ++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
		strDBFlag = m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		// ���� �÷��� ����
		if(strDBFlag.IsEmpty())
			ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		if(strDBFlag == _T("N"))
			ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);
	}

	return 0;
}

INT CSpecies_Form::ApplyOneVolumeData(CString strVol, CString strQueryFlag)
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
	INT nReprentBookIdx = -1;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nReprentBookIdx);
	if(nReprentBookIdx < 0)
	{
		nReprentBookIdx = 0;
		m_pDM_Book->SetCellData(_T("BB_��ǥå����"), _T("Y"), nReprentBookIdx);
	}
/*
	//���Ϸù�ȣ�� �����Ѵ�. ������ȣ�ο����ؿ� ���� ���Ϸù�ȣ�� �ο��Ѵ�.
	m_pFormApi->SetVolSortNo(&m_cCopyCodeMgr, m_pDM_Book);
*/	
	CStringArray arrIgnore; arrIgnore.RemoveAll();
	arrIgnore.Add(_T("IBS_��������_�ڵ�"));		arrIgnore.Add(_T("IBS_��ü����_�ڵ�"));
	arrIgnore.Add(_T("IBS_��ġ��ȣ_�ڵ�"));		arrIgnore.Add(_T("IBS_�̿����ѱ���_�ڵ�"));	
	arrIgnore.Add(_T("IBS_�����"));			arrIgnore.Add(_T("IBS_������Ư��"));
	arrIgnore.Add(_T("IBS_�����ڷ�"));			arrIgnore.Add(_T("IBS_ũ��"));
	arrIgnore.Add(_T("IBS_�����"));

	CString strRepBookYN, strDBFlag;
	CString strVolSortNo = m_pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), nReprentBookIdx);
	if(strVolSortNo.IsEmpty()) strVolSortNo = _T("10");
	for (INT idx = 0; idx < nBookCount; idx ++)
	{
		// å�� �ƴϸ� �Ѿ��.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
		// ���� CM -> å DM
		strDBFlag = m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		strRepBookYN = m_pDM_Book->GetCellData(_T("BB_��ǥå����"), idx);
		if(strDBFlag == _T("N") || strRepBookYN == _T("Y"))
			ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Book, idx, _T("BB"));
		else
			ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Book, idx, _T("BB"), &arrIgnore);
		// ����� ����
		ids = m_pDM_Book->SetCellData(_T("BB_�����"), strVol, idx);
		// ���� �÷��� ����
		if(strDBFlag.IsEmpty())
			ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		if(strDBFlag == _T("N"))
			ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);
		//���Ϸù�ȣ ����
		ids = m_pDM_Book->SetCellData(_T("BB_���Ϸù�ȣ"), strVolSortNo, idx);
	}

	return 0;
}

INT CSpecies_Form::ApplyWorkingStatus()
{
	INT idx = 0;
	INT nRowCount = 0;

	CString strBookWorkingStatus;

	//���ο� �ڷ���¸� �����Ѵ�.
	m_pDM_Index->GetCellData(_T("IBS_�۾�����INFO"), 0, strBookWorkingStatus);
	if(strBookWorkingStatus.IsEmpty())
		m_pDM_Index->SetCellData(_T("IBS_�۾�����INFO"), m_strWorkingStatus, 0);
	
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
			m_pDM_Book->SetCellData(_T("BB_�ڷ����"), m_strWorkingStatus, idx);
		//20070821 ADD PJW : ���������ϰ�� ����ڷ�� ����ΰ��ڷ�� �����Ѵ�.
		if(strBookWorkingStatus.Left(3) == _T("BOR") && m_bNewSaveFlag == TRUE)
		{
			m_pDM_Book->SetCellData(_T("BB_�ڷ����"), _T("BOR111N"), idx);
			m_pDM_Book->SetCellData(_T("BB_��Ϲ�ȣ"), _T(""), idx);
			m_pDM_Book->SetCellData(_T("BB_����KEY"), _T(""), idx);
			//DELm_bNewSaveFlag = FALSE;
		}
		else
		{
				if(strBookWorkingStatus.Left(3) == _T("BOR")  && m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("I"))
				m_pDM_Book->SetCellData(_T("BB_�ڷ����"), _T("BOR111N"), idx);
		}
		/*
		if(strBookWorkingStatus.Left(3) == _T("BOR")  && m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("I"))
				m_pDM_Book->SetCellData(_T("BB_�ڷ����"), _T("BOR111N"), idx);
		*/
	}

	return 0;
}

// 2007.08.10 ADD BY PDJ
// �η��� �ڷ���� �ο� ���ص� �ǳ�? �ؾߵ�.
INT CSpecies_Form::ApplyAppendixWorkingStatus()
{
	INT idx = 0;
	INT nRowCount = 0;

	CString strBookWorkingStatus;

	//���ο� �ڷ���¸� �����Ѵ�.
	m_pDM_Index->GetCellData(_T("IBS_�۾�����INFO"), 0, strBookWorkingStatus);
	if(strBookWorkingStatus.IsEmpty())
		m_pDM_Index->SetCellData(_T("IBS_�۾�����INFO"), m_strWorkingStatus, 0);
	
	// å�� Working Status�� �ִ´�.
	nRowCount = m_pDM_Appendix->GetRowCount();

	strBookWorkingStatus = _T("");
	for (idx = 0; idx < nRowCount; idx++)
	{		
		// å�� �̹� �۾����°� ������ ���¶�� continue
		m_pDM_Appendix->GetCellData(_T("BB_�ڷ����"), idx, strBookWorkingStatus);
		if(strBookWorkingStatus.IsEmpty())
			m_pDM_Appendix->SetCellData(_T("BB_�ڷ����"), m_strWorkingStatus, idx);
		//20070821 ADD PJW : ���������ϰ�� ����ڷ�� ����ΰ��ڷ�� �����Ѵ�.
		if(strBookWorkingStatus.Left(3) == _T("BOR") && m_bNewSaveFlag == TRUE)
		{
			m_pDM_Book->SetCellData(_T("BB_�ڷ����"), _T("BOR111N"), idx);
			m_pDM_Book->SetCellData(_T("BB_��Ϲ�ȣ"), _T(""), idx);
			m_pDM_Book->SetCellData(_T("���ι�ȣ"), _T(""), idx);
			m_bNewSaveFlag = FALSE;
		}
		else
		{
				if(strBookWorkingStatus.Left(3) == _T("BOR")  && m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("I"))
				m_pDM_Book->SetCellData(_T("BB_�ڷ����"), _T("BOR111N"), idx);
		}
		/*
		if(strBookWorkingStatus.Left(3) == _T("BOR") && m_pDM_Appendix->GetCellData(_T("BB_DB_FLAG"), idx) == _T("I"))
			m_pDM_Appendix->SetCellData(_T("BB_�ڷ����"), _T("BOR111N"), idx);
		*/
	}

	return 0;
}

VOID CSpecies_Form::OnbMARCIMPORT() 
{
	// TODO: Add your control notification handler code here
	//m_pHeadDlg->ApplyData();
	ApplyData();
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

		m_pInfo->pMarcMgr->DeleteField(&marc, _T("001"));
		m_pInfo->pMarcMgr->DeleteField(&marc, _T("090"));
		m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));

/*
		//å������ ��ǥå�� �����ϰ� ��� �����.
		for(INT i=1 ; i<m_pDM_Book->GetRowCount() ; i++)
		{
			m_pDM_Book->DeleteRow(i);
		}
*/

		//2: �ٱ����������� ��ü�������� -> 1 + å�� �����´�.
		if (dlg.m_nRadBasicInfo == 2)
		{
			m_pInfo->pMarcMgr->DeleteField(&marc, _T("505"));
		}
		
		// ���� : Marc to Index
		ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
		
		if (ids < 0) goto ERR;
		
		//1: ��ü ���� ���� -> ��ũ�� IDX���� MARC�� ������
		if (dlg.m_nRadBasicInfo == 1 || dlg.m_nRadBasicInfo == 2)
		{
			m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
			ids = m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
		}
		else
		{
			ids = m_pDM_Species->SetCellData(_T("BS_MARC"), _T(""), 0);
		}
		m_pFormApi->SetRepresentationBook(m_pDM_Book);
		
		Display();
		AfxMessageBox(_T("Marc�� �����Ͽ����ϴ�."), MB_ICONINFORMATION);
		return;
/*

		CStringArray DeleteFieldList;	DeleteFieldList.RemoveAll();
		DeleteFieldList.Add(_T("001"));	DeleteFieldList.Add(_T("090"));	DeleteFieldList.Add(_T("049"));
		
		//0: �⺻�������� 1: ��ü�������� 2: �ٱ����������� ��ü��������
		//0: �⺻���������� �����Ѵ�.
		//1: ���������� �ٱ�����(505) å����(049), ��ũ�������� �����Ѵ�.
		//2: ���������� å����(049), ��ũ������ �����Ѵ�.

		//å������ ��ǥå�� �����ϰ� ��� �����.
		for(INT i=1 ; i<m_pDM_Book->GetRowCount() ; i++)
		{
			m_pDM_Book->DeleteRow(i);
		}

		// ���� : Marc to Index
		ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
		if(ids < 0) goto ERR;
		if(dlg.m_nRadBasicInfo == 1 || dlg.m_nRadBasicInfo == 2)
		{
			ids = m_pBOSpeciesApi->RestructBookBy049MarcTag(m_pDM_Book, &marc, TRUE);
			if (ids < 0) goto ERR;
		}
		if(dlg.m_nRadBasicInfo == 1)
		{
			ids = m_pBOSpeciesApi->RestructBookBy505MarcTag(m_pDM_Book, &marc);
			if (ids < 0) goto ERR;
		}
		else
			DeleteFieldList.Add(_T("505"));
		
		ids = m_pBOSpeciesApi->ClearRegNoFieldOnBookDM(m_pDM_Book);
		if (ids < 0) goto ERR;
		ids = m_pBOSpeciesApi->SetVolSortNo(m_pDM_Book);
		if (ids < 0) goto ERR;
		
		//DeleteField
		for(i=0 ; i<DeleteFieldList.GetSize() ; i++)
		{
			m_pInfo->pMarcMgr->DeleteField(&marc, DeleteFieldList.GetAt(i));
		}

		//1: ��ü ���� ���� -> ��ũ�� IDX���� MARC�� ������
		if (dlg.m_nRadBasicInfo == 1 || dlg.m_nRadBasicInfo == 2)
		{
			m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
			ids = m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
		}
		m_pFormApi->SetRepresentationBook(m_pDM_Book);
		
		Display();
		AfxMessageBox(_T("Marc�� �����Ͽ����ϴ�."), MB_ICONINFORMATION);
		return;
*/
	}

	return;

ERR:
	AfxMessageBox(_T("Marc ���� ����"));
	return;

}	


VOID CSpecies_Form::OnbKOLISNET() 
{
	// TODO: ������� ����	
	ApplyData();
	CUC_MARC_IMPORT dlg(this);
	CString sTitle, sAuthor, sPublisher, sPubYear;
	
	GetDlgItem(IDC_eTITLE)->GetWindowText(sTitle);
	GetDlgItem(IDC_eAUTHOR1)->GetWindowText(sAuthor);
	GetDlgItem(IDC_ePUBLISHER)->GetWindowText(sPublisher);
	GetDlgItem(IDC_ePUBLISH_YEAR)->GetWindowText(sPubYear);

	dlg.SetSearchCondition( sTitle, sAuthor, sPublisher, sPubYear );
	if (dlg.DoModal() != IDOK) return;
	
	if( dlg.m_TocCheck_CON == TRUE )   // ���� ���� ���� Ȯ�� 
    {
		m_bInsertAC = TRUE;
        m_strContents = dlg.m_Contents;                // ���� �������� 
		m_pDM_Index->SetCellData(_T("IBS_��������"), _T("Y"), 0);
	}
    if( dlg.m_TocChek_ABS == TRUE )   // �ʷ� ���� ���� Ȯ�� 
    {
		m_bInsertAC = TRUE;
        m_strAbstracts = dlg.m_Abstracts;              // �ʷ� �������� 
		m_pDM_Index->SetCellData(_T("IBS_�ʷ�����"), _T("Y"), 0);
    }
	CMarc marc;
	
	//===================================================
	//2009.05.06 UPDATE BY LKS : 
	CString str056Field;
	//===================================================

	if(dlg.sMarc.IsEmpty()) return;
	INT ids = m_pInfo->pMarcMgr->Decoding(dlg.sMarc, &marc);
	if (ids < 0) goto ERR;

/*
	//å������ ��ǥå�� �����ϰ� ��� �����.
	for(i=1 ; i<m_pDM_Book->GetRowCount() ; i++)
	{
		m_pDM_Book->DeleteRow(i);
	}
*/
	
	// ��ũ�� IDX ���� (��ũ�� ����)
	ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
	if (ids < 0) goto ERR;
	
	// MARC ������ ����
	ids = m_pDM_Species->SetCellData(_T("BS_MARC"), dlg.sMarc, 0);
	if (ids < 0) goto ERR;
	//���� ���� -> ��ǥå���� -> å�� ����ɶ� ��ǥ���ڸ� ���� �Ѵ�.
	m_pFormApi->SetRepresentationBook(m_pDM_Book);

	Display();

	//===================================================
	//2009.05.22 UPDATE BY LKS : ���Թ��� ��ũ�� 056�±� a���� ���ڸ� �о� ���������� Set

	m_pInfo->pMarcMgr->GetField(&marc, _T("056"), str056Field);
	
	if(!str056Field.IsEmpty())
	{
		INT idx = str056Field.FindOneOf(_T("a"));
		
		CString strSubjectCode,strTemp;
		strSubjectCode = str056Field.GetAt(idx+1);

		CESL_ControlMultiComboBox* pCM_Subject = (CESL_ControlMultiComboBox *)m_pCM_Accu->FindControl(_T("BP_��������_�ڵ�"));
		if( pCM_Subject != NULL )
		{
			INT nComboCnt = pCM_Subject->GetRowCount();
			for( INT i=0; i < nComboCnt; i++ )
			{
				strTemp = pCM_Subject->GetItemDataString(i,0);
				
				if( strTemp == strSubjectCode ) break;
			}
				
			if( i <= nComboCnt)
				pCM_Subject->SetCurSel(i);
		}
	}
	//===================================================

	MessageBox(_T("��������� �����Ͽ����ϴ�."));
	return;

ERR :
	MessageBox(_T("������� ���Խ���"));
	
}

VOID CSpecies_Form::OnbAPPENDIX() 
{
	// TODO: Add your control notification handler code here
	INT ids = ApplyData();
	if(ids < 0) return ;

	CBOAppendixDlg dlg(this);
	dlg.SetFormApi(m_pFormApi);
	if(m_nOpType == SINSERT) dlg.SetOpenMode(1);
	else dlg.SetOpenMode(-1);
	dlg.SetAppendixDM(m_pDM_Appendix);
	dlg.SetSpeciesDM(m_pDM_Species);
	dlg.SetIndexDM(m_pDM_Index);
	dlg.SetBookDM(m_pDM_Book);
	dlg.DoModal();		
}

VOID CSpecies_Form::OnbCLEARSCREEN() 
{
	// TODO: Add your control notification handler code here
	ClearScreen();
	m_pTailDlg->ClearScreen();
	InitRegCodeField();
	OnKillfocuseREGCLASSCODE();
	this->OnSelchangecCLASIFYCLASS();
	GetDlgItem(IDC_eTITLE)->SetFocus();
	//2006-03-03 �ٱǺ��϶� ��/å������ �⺻���� ����� �����.
	INT nRow = m_pDM_Book->GetRowCount();
	if( nRow > 1)
	{
		for( INT i = nRow; i > 1; i--)
		m_pDM_Book->DeleteRow(i-1);
	}
}

VOID CSpecies_Form::OnKillfocuseREGCLASSCODE() 
{
	// TODO: Add your control notification handler code here
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
	m_pCM_Index->GetControlMgrData(_T("IBS_�����"), strVol);
	if (m_nOpType != SINSERT && m_pFormApi->IsRegCodeModify(&arrRegCode, m_pDM_Book, strVol))
	{
		AfxMessageBox(_T("��ϱ����� ������ �� �� �����ϴ�. å�������� �������ּ���"));
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
		return;			
	}

	m_strRegCodeField = strRegCodeField;

	INT nBookCount = 0;
	for (INT i = 0; i < nRegCodeCount; i++)
	{
		nBookCount += arrRegCode.GetAt(i).m_nCount;
	}

	CString strBookCount, strPrevBookCount;
	strBookCount.Format(_T("%d"), nBookCount);
	m_pCM_Accu->GetControlMgrData(_T("BP_��å��"), strPrevBookCount);
	if(strBookCount != strPrevBookCount)
	{
		m_pTailDlg->SetInit(FALSE);
		ApplyData();
	}
	m_pCM_Accu->SetControlMgrData(_T("BP_��å��"), strBookCount);
	m_bViewVolInfo = FALSE;	

}

VOID CSpecies_Form::OnbPREVSPECIES() 
{
	//===================================================
	//2009.03.28 UPDATE BY LKS : �ð���Ʈ�� �ʱ�ȭ
	m_hApplicantTime->SetData(_T(""));
	//===================================================
	m_nParentSelectedRow = GetPrevSelectIndex();
	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);
	UpdateInitProc();
	if( MakeUpdateDM() < 0 )
	{
		AfxMessageBox(_T("UPDATE DM ���� ����"));
		return;
	}

	if( m_bAcqTypeChange )	SetHeadTailInfoDlg();
	else m_pHeadDlg->Init();

	ApplyMarcToIndexCM();
	m_pTailDlg->Init();
	Display();	
	SetEBookCtrl();

	//===================================================
	//2009.10.20 ADD BY LKS
	OnSelchangecCLASIFYCLASS();
	//===================================================
}

VOID CSpecies_Form::OnbNEXTSPECIES() 
{
	//===================================================
	//2009.03.28 UPDATE BY LKS : �ð���Ʈ�� �ʱ�ȭ
	//===================================================
	m_hApplicantTime->SetData(_T(""));

	m_nParentSelectedRow = GetNextSelectIndex();
	if( m_nParentSelectedRow > m_pDM_Parent->GetRowCount()-1 )
	{
		m_nParentSelectedRow = m_pDM_Parent->GetRowCount()-1;
	}

	//===================================================
	//2009.05.20 ADD BY LKS : ������ �ڷ� ����� m_nParentSelectedRow�� -1�� ������
	//						  MakeUpdateDM()�� ERROR
	if(m_nParentSelectedRow == -1)
	{
		if(m_pGrid_Parent->SelectGetCount() == 1)
			m_nParentSelectedRow = m_pGrid_Parent->SelectGetHeadPosition();
		else
			m_nParentSelectedRow = m_pDM_Parent->GetRowCount()-1;
		CWnd* hWnd;
		hWnd = GetDlgItem(IDC_bNEXT_SPECIES);
		if(hWnd != NULL)		
			hWnd->EnableWindow(FALSE);
	}
	//===================================================

	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);
	UpdateInitProc();
	if( MakeUpdateDM() < 0 )
	{
		AfxMessageBox(_T("UPDATE DM ���� ����"));
		return;
	}

	if( m_bAcqTypeChange )		SetHeadTailInfoDlg();
	else m_pHeadDlg->Init();
	ApplyMarcToIndexCM();
	m_pTailDlg->Init();
	Display();	
	SetEBookCtrl();

	//===================================================
	//2009.10.20 ADD BY LKS
	OnSelchangecCLASIFYCLASS();
	//===================================================
}

VOID CSpecies_Form::OnbSAVEAS() 
{
	// 20070821 ADD PJW : �������� ���� �÷��� TRUE ����
	m_bNewSaveFlag = TRUE;
	
	/************************************************************************
	 ************************************************************************/
	//20080515 ADD BY PJW : ���� �Է��ϰ� �ٷ� ����� �ڵ��������� �����ʵ��� ����
	KillTimer(0);
	/************************************************************************ 
	 ************************************************************************/
	// TODO: Add your control notification handler code here
	if (m_nOpType == SINSERT) return;

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

	//===================================================
	//2009.05.28 ADD BY LKS : Trim Control
	TrimControl();
	//===================================================

	// �Է��� ������ ����
	//SetOpType(SINSERT);
	m_pBOSpeciesApi->SetOpenMode(1100); //1100:�Է�, -1100:����
	m_pHeadDlg->SetOpType(SINSERT);
	
	ids = ApplyData();
	if (ids < 0)
	{
		AfxMessageBox(_T("������ �������"));
		m_bNewSaveFlag = FALSE;
		return;
	}
	// ��ũ ���۷��̼�
	ids = ApplyIndexToMarc();
	if (ids < 0)
	{
		AfxMessageBox(_T("���� ����"));
		EndDialog(IDOK);
		m_bNewSaveFlag = FALSE;
		return;
	}

	m_bNewSaveFlag = FALSE;

	m_pDM_Species->StartFrame();
	ids = CheckWorkNoInfo(m_pDM_Species);
	if( ids < 0)
	{
		ESLAfxMessageBox(_T("�۾���ȣ�� ������ �� �����ϴ�."));
		return ;
	}
	// ����
	// 2004.11.09. 5163 ǥ����� �߰�.
	// ���������� ��� REC_KEY�� ���� ���鵵�� �ϱ� ���� Species_key�� ���� ����.
	if( m_strUseCoverYN == _T("Y"))	
	{
		m_pBOSpeciesApi->m_strSPECIES_KEY.Empty();
	}
	//===============================================================
	//20080613 ADD BY PJW : �ֹ���ȣ�� NULL�� �����Ѵ�.
	//m_pDM_Accu->SetCellData((_T("BP_�����Ϸù�ȣ"), _T(""),0);
	//===============================================================
	ids = m_pBOSpeciesApi->BO_SPECIES_INSERT(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, m_pDM_Appendix, 0, (INT)m_nSpeciesType);
	if (ids < 0)		AfxMessageBox(_T("���� ����"));
	else
	{
		if( m_nParentType == SPECIES || m_nParentType == VOLUME )
		{
			m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent_Book, m_pGrid_Parent, -1, 0, BOOK, FALSE, m_pHeadDlg->GetGroupInfo());
		}
		//    m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, FALSE, m_pHeadDlg->GetGroupInfo());
		if( m_nParentType == SPECIES )			
		{
			m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, FALSE, m_pHeadDlg->GetGroupInfo());
			// ���ְ����� �θ�DM�� �����Ѵ�.(�׸�����¿�)
			m_pTailDlg->SetOrderPrice_InDM(m_pDM_Parent, m_pDM_Parent->GetRowCount()-1 );
			//===================================================
			//2009.05.22 ADD BY PJW : ���԰����� åDM���� �߰��Ѵ�.
			INT nParentBookDmRowCnt;
			CString strOrderPrice;
			CString strSpeciesKey;
			strSpeciesKey = m_pDM_Parent->GetCellData(_T( "IBS_��KEY"), m_nParentSelectedRow );
			nParentBookDmRowCnt = m_pDM_Parent_Book->GetRowCount();
 			for( INT i = 0 ; i < nParentBookDmRowCnt ; i++ )
			{
				if ( strSpeciesKey == m_pDM_Parent_Book->GetCellData(_T("IBS_��KEY"), i) ) 
				{
					m_pTailDlg->SetOrderPrice_InDM( m_pDM_Parent_Book, i );
				}
			}
			//===================================================
			
			// �׸��带 �ٽ� �׸���.
			m_pGrid_Parent->DisplayLine(m_pDM_Parent->GetRowCount()-1);
			m_pGrid_Parent->DisplayNumber();
			m_pGrid_Parent->SetTopRow(m_pDM_Parent->GetRowCount()-1+1);
		}
		else	m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, TRUE, m_pHeadDlg->GetGroupInfo());

		if(m_bInsertAC == TRUE)
		{
			m_bInsertAC = FALSE;
			ids = InsertAbstractContentsInfo();			
			if(ids < 0) ESLAfxMessageBox(_T("�����ʷ� ������ �������� ���߽��ϴ�."));
		}

		// 2005-04-08 ǥ������� ����ϴ� �������� �޽����� �ٸ��� �����ش�.
		if( m_strUseCoverYN == _T("Y") )
		{
			MessageBox(_T("�����Ͽ����ϴ�.\n���������� ��� ǥ�������� ������� �ʽ��ϴ�."), NULL, MB_OK | MB_ICONINFORMATION);	
		}
		else
		{
			ESLAfxMessageBox(_T("�����Ͽ����ϴ�."));
		}

		m_bControlDisplay = TRUE;
		m_pBOSpeciesApi->SetOpenMode(-1100); //1100:�Է�, -1100:����
		m_pHeadDlg->SetOpType(SUPDATE);
		// 2004.12.21 ���� �� Species_key �� �ʱ�ȭ�Ѵ�.
		m_szSpeciesKey[0] = '\0';
		OnbNEXTSPECIES();		
	}
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ʼ��׸��� üũ�Ѵ�. (����, ��ϱ���) + Head / Tail_Dlg
//
// [ �̴Ͻ��� ]
// 1. �� ���ߵ� �׸��� �����Ѵ�.(����, ��ϱ���)
// 2. ������ �׸�鿡 ���� �ִ��� �Ǵ��Ͽ� ���ٸ� �޽����� �˷��ش�.
// 3. Head / TailDlg�� ���� ��ȿ���� üũ�Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::ValidateData()
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT i, ids;
	CString strText, strMsg;
	CStringArray ArrAlias;	
	CArray<INT, INT> ArrResource;
	CWnd* pWnd = NULL;
	
	
	//=======================================================================
	// 1. �� ���ߵ� �׸��� �����Ѵ�.(����, ��ϱ���)
	//=======================================================================
	ArrAlias.RemoveAll();	
	ArrAlias.Add(_T("����"));	
	ArrAlias.Add(_T("��ϱ���"));
	
	ArrResource.RemoveAll();
	ArrResource.Add(IDC_eTITLE);	
	ArrResource.Add(IDC_eREG_CLASS_CODE);


	//=======================================================================
	// 2. ������ �׸�鿡 ���� �ִ��� �Ǵ��Ͽ� ���ٸ� �޽����� �˷��ش�.
	//=======================================================================
	for( i=0; i<ArrAlias.GetSize(); i++ )
	{
		pWnd = GetDlgItem(ArrResource.GetAt(i));
		if( pWnd == NULL )		continue;

		pWnd->GetWindowText(strText);
		if( strText.IsEmpty() == TRUE )
		{
			strMsg.Format(_T("%s ��(��) �Է��� �ּ���"), ArrAlias.GetAt(i));
			MessageBox(strMsg);
			//==========================================================================
			//20080515 ADD BY PJW : ��ϱ����� �Է����� �ʾ��� ��� ������Ϲ�ȣ�� ""�̸� ����Ʈ������ �־��ش�.
			if( m_strRegCodeField.IsEmpty() == TRUE )
			{
				// DB���� �⺻���� �����Ǿ� �ִ��� �Ǵ��Ͽ� ���ٸ� EM�� �⺻���� �Ѵ�.
				m_pInfo->pCodeMgr->GetDefaultCode(_T("��ϱ���"), m_strRegCodeField);
				if( m_strRegCodeField.IsEmpty() == TRUE )	m_strRegCodeField = _T("EM");
				GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
			}
			//==========================================================================
			pWnd->SetFocus();
			return -1;
		}
	}

	
	//=======================================================================
	// 3. Head / TailDlg�� ���� ��ȿ���� üũ�Ѵ�.
	//=======================================================================
	ids = m_pHeadDlg->ValidateData();
	if( ids < 0 )	return -1;
	ids = m_pTailDlg->ValidateData();
	if( ids < 0 )	return -1;
	
	return 0;
}

INT CSpecies_Form::ApplyIndexToMarc(INT idx /*= 0*/)
{
	INT ids = -1;
	CMarc marc;
	CString strMarc;

	// �� -> ��ũ
	strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	if (!strMarc.IsEmpty())
	{
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if (ids < 0)
		{
			ESLAfxMessageBox(_T("����� ��ũ������ �ùٸ��� �ʽ��ϴ�. ��ũ�� ����� �ϰڽ��ϴ�."));
			//return -1;
		}
	}

	//===================================================
	//2009.05.06 ADD BY LKS : ��������ÿ� 049��ũ�� ������
	
	if(m_bNewSaveFlag)
	{
		m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
	}
	//===================================================

	//������ ��ũ�� �����ÿ� �������� �ƴѰ�� 100Tag�� ������ؾ� �Ѵ�.
	if(m_pDM_Index->GetCellData(_T("IBS_�۾�����INFO"), 0).Left(3) != _T("BOC") 
		&& m_pDM_Index->GetCellData(_T("IBS_�۾�����INFO"), 0).Left(3) != _T("BOL"))
	{
		m_pInfo->pMarcMgr->DeleteField(&marc, _T("100"));
		m_pInfo->pMarcMgr->DeleteField(&marc, _T("700"));
		//==========================================================================
		//20080502 ADD BY PJW : MARC�����ϱ����� 740tag�� ������ ���־� �ΰ��� ����� ���� �����Ѵ�.
		m_pInfo->pMarcMgr->DeleteField(&marc, _T("740"));//,_T(""));
		//==========================================================================
	}
	//==========================================================================
	//20080623 ADD BY PJW : ��Ʈisbn�� ����ǰų� �������� �ʾƼ� �����ϱ� ���� 
	m_pInfo->pMarcMgr->DeleteField(&marc, _T("020"));
	//==========================================================================
	ids = m_pBOSpeciesApi->APPLY_Species_to_CMarc(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
	m_pInfo->pMarcMgr->CheckDeepSyntax(&marc);
	m_pInfo->pMarcMgr->CheckSimpleSyntax(&marc);
	m_pInfo->pMarcMgr->DeleteEmptySubfield(&marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(&marc);
	ids = m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if (ids < 0) return ids;
	
	m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
	
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���� ��ư
// [�̴Ͻ��� ]
//
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::OnbSAVE() 
{
	INT ids, openMode;
	CString strMarc;
	CMarc marc;	

	//==========================================================================
	//20080515 ADD BY PJW : ���� �Է��ϰ� �ٷ� ����� �ڵ��������� �����ʵ��� ����
	KillTimer(0);
	//==========================================================================

	//==========================================================================
	//20080321 ADD BY PJW : ���ı����� ��������ϰ�� �����Ѵ�.
	CString strFormCode;
	m_pCM_Index->GetControlMgrData(_T("IBS_���ı���_�ڵ�"), strFormCode);
	if(strFormCode.IsEmpty())
	{
		ESLAfxMessageBox(_T("���ı����� ��������� ������ �� �����ϴ�."));
		CESL_ControlMultiComboBox * pCmb = (CESL_ControlMultiComboBox*)m_pCM_Index->FindControl(_T("IBS_���ı���_�ڵ�"));
		m_pInfo->pCodeMgr->GetDefaultCode(_T("���ı���"), strFormCode);
		m_pCM_Index->SetControlMgrData(_T("IBS_���ı���_�ڵ�"), strFormCode);
		if(pCmb) pCmb->SetFocus();
		return ;
	}
	//==========================================================================
	
	//=======================================================================
	// 1. �ʼ��׸��� üũ�Ѵ�. (����, ��ϱ���) + Head / Tail_Dlg
	//=======================================================================
	ids = ValidateData();
	if( ids < 0 )	return;

	//===================================================
	//2009.05.28 ADD BY LKS : Control Trim
	TrimControl();
	//===================================================

	// �Է��� ������ ����
	ids = ApplyData();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("������ �������"));
		return;
	}

	//=======================================================================
	//20080320 ADD BY PJW : �η��� ���Ա����� ���� �����ش�.
	INT nAppendixCnt;
	nAppendixCnt = m_pDM_Appendix->GetRowCount();
	if ( m_nAcqType == PURCHASE )
	{
		for (INT i = 0 ; i < nAppendixCnt ; i++ )
		{
			m_pDM_Appendix->SetCellData(_T("BB_���Ա���_�ڵ�"), _T("1"), i);
		}		
	}
	else if ( m_nAcqType == DONATE )
	{
		for (INT i = 0 ; i < nAppendixCnt ; i++ )
		{
			m_pDM_Appendix->SetCellData(_T("BB_���Ա���_�ڵ�"), _T("2"), i);
		}
	}
	//=======================================================================
	//=======================================================================
	//2008.07.17 ADD BY PJW : �����Ұ�� �η��� �����ϸ� �ڵ� ��Ϲ�ȣ�� �ο��� �����Ѵ�.
	// �η� ������ ������ 
	INT nBookCnt;
	CString strWorkStatus;
	if ( 0 < nAppendixCnt )
	{
		nBookCnt = m_pDM_Book->GetRowCount();
		for( INT i = 0 ; i < nBookCnt ; i++ )
		{
			ids = m_pDM_Book->GetCellData( _T("BB_�ڷ����"), i, strWorkStatus);
			if ( strWorkStatus == _T("BOR112N") )
			{
				// �η� ��Ϲ�ȣ �����Ѵ�.
				ids = ApplyAppendixRegNo(m_pDM_Appendix);
				if (ids < 0)
				{
					m_strLastErrMsg = _T("�η����� ��������� ����");
					return;
				}
				
				break;
			}
		}
	}
	
	//=======================================================================

	// �� API�� ���¸�� ����
	openMode = -1;
	if( m_nOpType == SINSERT )			openMode = 1100;
	else /*if(m_nOpType != SINSERT)*/	openMode = -1100;
	m_pBOSpeciesApi->m_nOpenMode = openMode;

	// �����߱�
	if( m_bDupAddMode == TRUE )
	{
		SPECIES_TYPE nSType = m_nSpeciesType;
		if( nSType == BO_REGOBJ )
		{
			if		( m_nAcqType == PURCHASE )	nSType = BO_REG_PURCHASE;
			else if	( m_nAcqType == DONATE	 )	nSType = BO_REG_DONATE;
		}		

		m_pDM_Book->StartFrame();
		ids = CheckWorkNoInfo(m_pDM_Book);
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("�۾���ȣ�� ������ �� �����ϴ�."));
			return ;
		}
		//������������ ���� ���Ϸù�ȣ/������ȣ���� �����Ѵ�.
		CBOVolumeDlg dlg(this, m_pDM_Book);
		dlg.SetDupIdxKey(m_strDupIndexKey);
		ids = dlg.AdjustCurrentBookDMByHoldBook(m_pDM_Book);
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("�۾��Ͻ� å������ �ùٸ��� �ʽ��ϴ�."));
			return ;
		}
		// OpenMode�� ���� �����߱� �Լ��� �ҷ��´�. 
		if( m_nOpType == SINSERT )
		{
			ids = m_pBOSpeciesApi->BO_SPECIES_ADDBOOK(_T("INSERT"), m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, m_pDM_Appendix, 0, (INT)nSType, (INT)m_nAcqType);
			if(ids >= 0)
			{
				if(m_nParentType == SPECIES || m_nParentType == VOLUME)
				{
					m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent_Book, m_pGrid_Parent, -1, 0, BOOK, FALSE, m_pHeadDlg->GetGroupInfo());
				}				
				if( m_nParentType == SPECIES )
				{
					m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, FALSE, m_pHeadDlg->GetGroupInfo());
					// ���ְ����� �θ�DM�� �����Ѵ�.(�׸�����¿�)
					m_pTailDlg->SetOrderPrice_InDM(m_pDM_Parent, m_pDM_Parent->GetRowCount()-1 );

					//===================================================
					//2009.05.22 ADD BY PJW : ���԰����� åDM���� �߰��Ѵ�.
					INT nParentBookDmRowCnt;
					CString strOrderPrice;
					CString strSpeciesKey;

					strSpeciesKey = m_pDM_Parent->GetCellData(_T( "IBS_��KEY"), m_pDM_Parent->GetRowCount()-1 );
					nParentBookDmRowCnt = m_pDM_Parent_Book->GetRowCount();
 					for( INT i = 0 ; i < nParentBookDmRowCnt ; i++ )
					{
						if ( strSpeciesKey == m_pDM_Parent_Book->GetCellData(_T("IBS_��KEY"), i) ) 
						{
							m_pTailDlg->SetOrderPrice_InDM( m_pDM_Parent_Book, i );
						}
					}
					//===================================================
					
					// �׸��带 �ٽ� �׸���.
					m_pGrid_Parent->DisplayLine(m_pDM_Parent->GetRowCount()-1);
					m_pGrid_Parent->DisplayNumber();
					m_pGrid_Parent->SetTopRow(m_pDM_Parent->GetRowCount()-1+1);
				}
				else	m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, TRUE, m_pHeadDlg->GetGroupInfo());

				m_bDupAddMode = FALSE;
				
				SetDataManagers();
				m_pDM_Index->InsertRow(-1);
				m_pDM_Species->InsertRow(-1);
				m_pDM_Accu->InsertRow(-1);
				m_pDM_Book->InsertRow(-1);
				m_pFormApi->AllControlClear(m_pCM_Index, &m_arrIgnoreCTRLAlias);
				m_pFormApi->AllControlClear(m_pCM_Accu, &m_arrIgnoreCTRLAlias);
				m_pTailDlg->AllControlClear();
				LoadDefaultValue();		
				m_bInsertData = TRUE;
				SetEBookCtrl();
			}
		}
		else if (m_nOpType != SINSERT)
		{
			ids = m_pBOSpeciesApi->BO_SPECIES_ADDBOOK(_T("UPDATE"), m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, m_pDM_Appendix, 0, (INT)nSType, (INT)m_nAcqType);
			if(ids >= 0)
			{
				if(m_nSpeciesType == SPECIES || m_nSpeciesType == VOLUME)
				{
					m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent_Book, m_pGrid_Parent, -1, 0, BOOK, FALSE, m_pHeadDlg->GetGroupInfo());
				}		
				
				if( m_nParentType == SPECIES )
				{
					m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, m_nParentSelectedRow, 0, m_nParentType, FALSE, m_pHeadDlg->GetGroupInfo());
					// ���ְ����� �θ�DM�� �����Ѵ�.(�׸�����¿�)
					m_pTailDlg->SetOrderPrice_InDM(m_pDM_Parent, m_nParentSelectedRow );
					//===================================================
					//2009.05.22 ADD BY PJW : ���԰����� åDM���� �߰��Ѵ�.
					INT nParentBookDmRowCnt;
					CString strOrderPrice;
					CString strSpeciesKey;
					
					strSpeciesKey = m_pDM_Parent->GetCellData(_T( "IBS_��KEY"), m_pDM_Parent->GetRowCount()-1 );
					nParentBookDmRowCnt = m_pDM_Parent_Book->GetRowCount();
 					for( INT i = 0 ; i < nParentBookDmRowCnt ; i++ )
					{
						if ( strSpeciesKey == m_pDM_Parent_Book->GetCellData(_T("IBS_��KEY"), i) ) 
						{
							m_pTailDlg->SetOrderPrice_InDM( m_pDM_Parent_Book, i );
						}
					}
					//===================================================

					// �׸��带 �ٽ� �׸���.
					m_pGrid_Parent->DisplayLine(m_nParentSelectedRow);
					m_pGrid_Parent->DisplayNumber();
					m_pGrid_Parent->SetTopRow(m_nParentSelectedRow+1);
				}
				else	m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, m_nParentSelectedRow, 0, m_nParentType, TRUE, m_pHeadDlg->GetGroupInfo());

				m_bControlDisplay = TRUE;
				m_bDupAddMode = FALSE;
				m_pFormApi->AllControlClear(m_pCM_Index, NULL);
				m_pFormApi->AllControlClear(m_pCM_Accu, NULL);
				m_pTailDlg->AllControlClear();
				OnbNEXTSPECIES();
				GetDlgItem(IDC_edtWORKING_STATUS)->SetFocus();
			}
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
			m_bInsertData = TRUE;
			// 2004.12.21 ���� �� Species_key �� �ʱ�ȭ�Ѵ�.
			m_szSpeciesKey[0] = '\0';
		}
	}
	// ��-�Է�
	else if (m_nOpType == SINSERT)
	{
		// ��ũ ���۷��̼�
		ids = ApplyIndexToMarc();
		if (ids < 0)
		{
			AfxMessageBox(_T("���� ����"));
			return;
		}
		m_pDM_Species->StartFrame();
		ids = CheckWorkNoInfo(m_pDM_Species);
		if( ids < 0)
		{
			ESLAfxMessageBox(_T("�۾���ȣ�� ������ �� �����ϴ�."));
			return ;
		}
		
		// 2004.11.09. 5163 ǥ����� �߰�.
		if( m_strUseCoverYN == _T("Y"))
		{
			m_pBOSpeciesApi->m_strSPECIES_KEY = m_szSpeciesKey;
		}					

		ids = m_pBOSpeciesApi->BO_SPECIES_INSERT(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, m_pDM_Appendix, 0, (INT)m_nSpeciesType);
		if (ids < 0)	ESLAfxMessageBox(_T("���� ����"));
		else
		{
			if( m_nParentType == SPECIES || m_nParentType == VOLUME )
			{
				m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent_Book, m_pGrid_Parent, -1, 0, BOOK, FALSE, m_pHeadDlg->GetGroupInfo());
//				::PostMessage(pParentMgr->GetSafeHwnd(), CHANGE_DM , 0, 0);
			}			
			if( m_nParentType == SPECIES )
			{
				m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, FALSE, m_pHeadDlg->GetGroupInfo());
				// ���ְ����� �θ�DM�� �����Ѵ�.(�׸�����¿�)
				m_pTailDlg->SetOrderPrice_InDM(m_pDM_Parent, m_pDM_Parent->GetRowCount()-1 );
				//===================================================
				//2009.05.22 ADD BY PJW : ���԰����� åDM���� �߰��Ѵ�.
				INT nParentBookDmRowCnt;
				CString strOrderPrice;
				CString strSpeciesKey;
				strSpeciesKey = m_pDM_Parent->GetCellData(_T( "IBS_��KEY"), m_nParentSelectedRow );
				nParentBookDmRowCnt = m_pDM_Parent_Book->GetRowCount();
 				for( INT i = 0 ; i < nParentBookDmRowCnt ; i++ )
				{
					if ( strSpeciesKey == m_pDM_Parent_Book->GetCellData(_T("IBS_��KEY"), i) ) 
					{
						m_pTailDlg->SetOrderPrice_InDM( m_pDM_Parent_Book, i );
					}
				}
				//===================================================
								
				//===================================================
				//2009.04.13 UPDATE BY LKS : �׸��忡 ������ �Ҽ�,��ȭ��ȣ,�ּ� �߰�
				CString strData,strQuery;
				CString strTEL,strPOSITION,strADDRESS;
				m_pDM_Accu->GetCellData(_T("BP_����������KEY"),0,strData);
				if(!strData.IsEmpty())
				{
					strQuery.Format(_T("SELECT ADDRESS FROM CO_DONATOR_TBL WHERE REC_KEY=%s"),strData);
					m_pDM_Accu->GetOneValueQuery(strQuery,strADDRESS);
					strQuery.Format(_T("SELECT POSITION FROM CO_DONATOR_TBL WHERE REC_KEY=%s"),strData);
					m_pDM_Accu->GetOneValueQuery(strQuery,strPOSITION);
					strQuery.Format(_T("SELECT TEL FROM CO_DONATOR_TBL WHERE REC_KEY=%s"),strData);
					m_pDM_Accu->GetOneValueQuery(strQuery,strTEL);

					m_pDM_Parent->SetCellData(_T("DO_�Ҽ�"),strPOSITION,m_pDM_Parent->GetRowCount()-1);
					m_pDM_Parent->SetCellData(_T("DO_��ȭ��ȣ"),strTEL,m_pDM_Parent->GetRowCount()-1);
					m_pDM_Parent->SetCellData(_T("DO_�ּ�"),strADDRESS,m_pDM_Parent->GetRowCount()-1);
				}
				//===================================================
				
				// �׸��带 �ٽ� �׸���.
				m_pGrid_Parent->DisplayLine(m_pDM_Parent->GetRowCount()-1);
				m_pGrid_Parent->DisplayNumber();
				m_pGrid_Parent->SetTopRow(m_pDM_Parent->GetRowCount()-1+1);
			}
			else m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, TRUE, m_pHeadDlg->GetGroupInfo());

			if(m_bInsertAC == TRUE)
			{
				m_bInsertAC = FALSE;
				ids = InsertAbstractContentsInfo();			
				if(ids < 0) ESLAfxMessageBox(_T("�����ʷ� ������ �������� ���߽��ϴ�."));
			}

			MessageBox(_T("���� �Ͽ����ϴ�"), NULL, MB_OK | MB_ICONINFORMATION);	
			m_bControlDisplay = TRUE;
			
			SetDataManagers();
			m_pDM_Index->InsertRow(-1);
			m_pDM_Species->InsertRow(-1);
			m_pDM_Accu->InsertRow(-1);
			m_pDM_Book->InsertRow(-1);
			m_pFormApi->AllControlClear(m_pCM_Index, &m_arrIgnoreCTRLAlias);
			m_pFormApi->AllControlClear(m_pCM_Accu, &m_arrIgnoreCTRLAlias);
			m_pTailDlg->AllControlClear();
			// 2004.11.09  SpeciesKey �ʱ�ȭ
			m_szSpeciesKey[0] = '\0';
			LoadDefaultValue();	
			m_bInsertData = TRUE;
			SetEBookCtrl();
			//2005-11-17 : ���Խ�û�� �⺻�� ����
			CString strDate;
			CString sDate = m_pBOSpeciesApi->GetTodayDate();
			CTime t = CTime::GetCurrentTime();
			strDate.Format(_T("%s %02d:%02d:%02d"), sDate, t.GetHour(), t.GetMinute(), t.GetSecond());		// �Է���
			strApplicantTime(strDate);
			
			//===================================================
			//2009.03.28 UPDATE BY LKS : �ð���Ʈ�� �ʱ�ȭ �� ���Խ�û�� �ڵ����� �߰����� �ʵ��� ����
			
			/*m_pCM_Index->SetControlMgrData(_T("���Խ�û��"), sDate, 0);*/
			m_hApplicantTime->SetData(_T(""));
			//===================================================

			GetDlgItem(IDC_edtBSFORM_REQUESTOR)->SetWindowText(_T(""));
			/*//20070810 ADD PJW : ��û�Ͽ� �ú��� �߰�			
			CString strDate = m_pBOSpeciesApi->GetTodayDate();
			CString sDate, strTime;
			CTime t = CTime::GetCurrentTime();
			//strDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());		// �Է���
			sDate.Format(_T("%s %02d:%02d:%02d"), strDate, t.GetHour(), t.GetMinute(), t.GetSecond());		// �Է���
			strApplicantTime(sDate);
			m_pCM_Index->SetControlMgrData(_T("���Խ�û��"), sDate, 0);
			*/			
			//m_pCM_Index->SetControlMgrData(_T("���Խ�û��"), sDate, 0);
		}
	}
	// ����-��-����-����
	else if (m_nOpType != SINSERT)
	{		
		// ��ũ ���۷��̼�
		ids = ApplyIndexToMarc();
		if (ids < 0)
		{
			AfxMessageBox(_T("���� ����"));
			return;
		}
		
		m_pDM_Species->StartFrame();
		ids = CheckWorkNoInfo(m_pDM_Species);
		if( ids < 0)
		{
			ESLAfxMessageBox(_T("�۾���ȣ�� ������ �� �����ϴ�."));
			return ;
		}			
	
		ids = m_pBOSpeciesApi->BO_SPECIES_UPDATE(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, m_pDM_Appendix, 0, (INT)m_nSpeciesType);
		if (ids < 0)	AfxMessageBox(_T("���� ����"));		
		else
		{
			if(m_nParentType == SPECIES || m_nParentType == VOLUME)
			{
				m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent_Book, m_pGrid_Parent, -1, 0, BOOK, FALSE, m_pHeadDlg->GetGroupInfo());
			}
			if( m_nParentType == SPECIES )
			{
				m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, m_nParentSelectedRow, 0, m_nParentType, FALSE, m_pHeadDlg->GetGroupInfo());
				// ���ְ����� �θ�DM�� �����Ѵ�.(�׸�����¿�)
				m_pTailDlg->SetOrderPrice_InDM(m_pDM_Parent, m_nParentSelectedRow );
				//===================================================
				//2009.05.22 ADD BY PJW : ���԰����� åDM���� �߰��Ѵ�.
				INT nParentBookDmRowCnt;
				CString strOrderPrice;
				CString strSpeciesKey;
				strSpeciesKey = m_pDM_Parent->GetCellData(_T( "IBS_��KEY"), m_nParentSelectedRow );
				nParentBookDmRowCnt = m_pDM_Parent_Book->GetRowCount();
 				for( INT i = 0 ; i < nParentBookDmRowCnt ; i++ )
				{
					if ( strSpeciesKey == m_pDM_Parent_Book->GetCellData(_T("IBS_��KEY"), i) ) 
					{
						m_pTailDlg->SetOrderPrice_InDM( m_pDM_Parent_Book, i );
					}
				}
				//===================================================
				
				// �׸��带 �ٽ� �׸���.
				m_pGrid_Parent->DisplayLine(m_nParentSelectedRow);
				m_pGrid_Parent->DisplayNumber();
				m_pGrid_Parent->SetTopRow(m_nParentSelectedRow+1);
			}
			else m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, m_nParentSelectedRow, 0, m_nParentType, TRUE, m_pHeadDlg->GetGroupInfo());

			/**************************************************************************
			 **************************************************************************/
			//20080221 UPDATE BY PJW : �����ܰ迡�� TOC_COMMON_TBL ������ �ʵ��� ����
			//if(m_bInsertAC == TRUE)
			//m_bInsertAC = FALSE; {   -> { m_bInsertAC = FALSE; 
			/**************************************************************************
			 **************************************************************************/
			if(m_bInsertAC == TRUE)
			{
				m_bInsertAC = FALSE;
				ids = InsertAbstractContentsInfo();			
				if(ids < 0) ESLAfxMessageBox(_T("�����ʷ� ������ �������� ���߽��ϴ�."));		
			}
			//����/�����̵���ڷ������ ������ ��� SPECIES_TYPE�� �ڷ���°� ��ġ���� ������ �ش� �ڷḦ DM/GRID���� �����Ͽ��� �Ѵ�.
			// BO_DONATE = BOA211O, BO_NONDONATE = BOA212O�� �ƴϸ� ���� ����̴�.
			//
			if(m_nSpeciesType == BO_DONATE || m_nSpeciesType == BO_NONDONATE)
			{
				if(!CheckDonateRegYN())		AddDonateRegYNDeleteRow(m_pArrDonateRegYN, m_nParentSelectedRow);
				else						DeleteDonateRegYNDeleteRow(m_pArrDonateRegYN, m_nParentSelectedRow);
			}
		
			MessageBox(_T("���� �Ͽ����ϴ�"), NULL, MB_OK | MB_ICONINFORMATION);	
			m_bControlDisplay = TRUE;
			// 2004.12.21  SpeciesKey �ʱ�ȭ
			m_szSpeciesKey[0] = '\0';
			OnbNEXTSPECIES();
			
			GetDlgItem(IDC_edtWORKING_STATUS)->SetFocus();

//			::PostMessage(pParentMgr->GetSafeHwnd(), CHANGE_DM , 0, 0);
		}
	}	
	//20070522 ADD PJW : ���� ��Ŀ��
	GetDlgItem(IDC_eTITLE)->SetFocus();
}

VOID CSpecies_Form::AddDonateRegYNDeleteRow(CArray<INT, INT> * pArrDonateRegYN, INT nRow)
{
	INT cnt = pArrDonateRegYN->GetSize();
	INT nTmpData;
	for(INT i=0 ; i<cnt ; i++)
	{
		nTmpData = pArrDonateRegYN->GetAt(i);
		if(nTmpData < nRow) continue;
		else if(nTmpData > nRow)
		{
			pArrDonateRegYN->InsertAt(i, nRow);
			return ;
		}
	}
	pArrDonateRegYN->Add(nRow);
}

BOOL CSpecies_Form::CheckDonateRegYN()
{
	if(m_nSpeciesType == BO_DONATE && m_strWorkingStatus == _T("BOA212O")) 
		return FALSE;
	else if(m_nSpeciesType == BO_NONDONATE && m_strWorkingStatus == _T("BOA211O"))
		return FALSE;
	return TRUE;
}

VOID CSpecies_Form::DeleteDonateRegYNDeleteRow(CArray<INT, INT> * pArrDonateRegYN, INT nRow)
{
	if(pArrDonateRegYN == NULL) return ;
	INT cnt = pArrDonateRegYN->GetSize();
	if(cnt == 0) return ;
	INT nTmpData;
	for(INT i=0 ; i<cnt ; i++)
	{
		nTmpData = pArrDonateRegYN->GetAt(i);
		if(nTmpData < 0) continue;
		if(nTmpData == nRow)
		{
			pArrDonateRegYN->RemoveAt(i);
		}
	}
}

INT CSpecies_Form::ApplyParentDM(CESL_DataMgr *pDM_IDX_BO, 
							   CESL_DataMgr * pDM_ACCU, 
							   CESL_DataMgr * pDM_BOOK, 
							   CESL_DataMgr * pDM_TARGET_BOOK, 
							   CESL_DataMgr * pDM_TARGET_VOL, 
							   CESL_DataMgr * pDM_TARGET_SPECIES,
							   INT nIdx)
{
	INT nTargetMatchingInfo = 0;
	INT nSourceMatchingInfo = 0;
	CString strTargetMatchingInfo[MAX_TEMP_BIG_STRING_ARRAY_SIZE];
	CString strSourceMatchingInfo[MAX_TEMP_BIG_STRING_ARRAY_SIZE];
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_������");			strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_������");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_���ı���_�ڵ�");		strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_���ı���_�ڵ�");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_������");			strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_������");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_�����");			strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_�����");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_��ǥ��");			strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_��ǥ��");			
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_�̿��󱸺�");		strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_�̿��󱸺�");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_��KEY");				strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_REC_KEY");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("BB_�Է���");				strSourceMatchingInfo[nSourceMatchingInfo++] = _T("BB_�Է���");				
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("BB_����");				strSourceMatchingInfo[nSourceMatchingInfo++] = _T("BB_����");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("BB_�ڷ����_�ڵ�");		strSourceMatchingInfo[nSourceMatchingInfo++] = _T("BB_�ڷ����_�ڵ�");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("BB_��ϱ���_�ڵ�");		strSourceMatchingInfo[nSourceMatchingInfo++] = _T("BB_�ڷ����_�ڵ�");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("BB_��ü����_�ڵ�");		strSourceMatchingInfo[nSourceMatchingInfo++] = _T("BB_��ü����_�ڵ�");
	

	INT ids, nTargetRowIdx, nRepresentBookIdx;
	CString strHeader, strData;
	m_pFormApi->GetRepresentationBook(pDM_BOOK, nRepresentBookIdx);
	//������ �Է�/����
	if(nIdx == -1)
	{
		pDM_TARGET_SPECIES->InsertRow(-1);
		nTargetRowIdx = pDM_TARGET_SPECIES->GetRowCount() - 1;
	}
	else 
		nTargetRowIdx = nIdx;
		
	for (INT i = 0; i < nTargetMatchingInfo; i++) {
		strData = _T("");
		strHeader = strTargetMatchingInfo[i].Left(2);
		if (strHeader == _T("IB")) ids = pDM_IDX_BO->GetCellData(strSourceMatchingInfo[i], 0, strData);
		if (strHeader == _T("BP")) ids = pDM_ACCU->GetCellData(strSourceMatchingInfo[i], 0, strData);
		if (strHeader == _T("BB")) ids = pDM_BOOK->GetCellData(strSourceMatchingInfo[i], nRepresentBookIdx, strData);
		if (ids >= 0) pDM_TARGET_SPECIES->SetCellData(strTargetMatchingInfo[i], strData, nTargetRowIdx);
	}

	// ���Է�
	INT nVolCnt = m_pFormApi->GetVolumeCount(pDM_BOOK);

	if(nIdx == -1 && pDM_TARGET_VOL != NULL)
	{
		for(INT j = 0 ; j<nVolCnt ; j++)
		{
			pDM_TARGET_VOL->InsertRow(-1);
			nTargetRowIdx = pDM_TARGET_VOL->GetRowCount() - 1;
			for (i = 0; i < nTargetMatchingInfo; i++) {
				strData = _T("");
				strHeader = strTargetMatchingInfo[i].Left(2);
				if (strHeader == _T("IB")) ids = pDM_IDX_BO->GetCellData(strSourceMatchingInfo[i], 0, strData);
				if (strHeader == _T("BP")) ids = pDM_ACCU->GetCellData(strSourceMatchingInfo[i], 0, strData);
				if (strHeader == _T("BB")) ids = pDM_BOOK->GetCellData(strSourceMatchingInfo[i], 0, strData);
				if (ids >= 0) pDM_TARGET_VOL->SetCellData(strTargetMatchingInfo[i], strData, nTargetRowIdx);
			}
		}
	}

	//å�Է�
	INT nBookCnt = m_pFormApi->GetVolumeCount(pDM_BOOK);
	if(nIdx == -1 && pDM_TARGET_BOOK != NULL)
	{
		for(INT j = 0 ; j<nBookCnt ; j++)
		{
			pDM_TARGET_BOOK->InsertRow(-1);
			nTargetRowIdx = pDM_TARGET_BOOK->GetRowCount() - 1;
			for (INT i = 0; i < nTargetMatchingInfo; i++) {
				strData = _T("");
				strHeader = strTargetMatchingInfo[i].Left(2);
				if (strHeader == _T("IB")) ids = pDM_IDX_BO->GetCellData(strSourceMatchingInfo[i], 0, strData);
				if (strHeader == _T("BP")) ids = pDM_ACCU->GetCellData(strSourceMatchingInfo[i], 0, strData);
				if (strHeader == _T("BB")) ids = pDM_BOOK->GetCellData(strSourceMatchingInfo[i], i, strData);
				if (ids >= 0) pDM_TARGET_BOOK->SetCellData(strTargetMatchingInfo[i], strData, nTargetRowIdx);
			}
		}
	}	
	
	return 0;
}

VOID CSpecies_Form::OnbEXTRAISBN() 
{
	// TODO: Add your control notification handler code here
	ApplyData();
	INT nRepresentBookIdx = -1;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nRepresentBookIdx);
	if(nRepresentBookIdx < 0) return ;
	CString strAdditionISBN = m_pDM_Book->GetCellData(_T("BB_�߰�����ISBN"), nRepresentBookIdx);
	CString strAdditionAddCode = m_pDM_Book->GetCellData(_T("BB_�߰�����ISBN�ΰ���ȣ"), nRepresentBookIdx);
	CString strAdditionMissISBN = m_pDM_Book->GetCellData(_T("BB_�߰����ǿ���ISBN"), nRepresentBookIdx);

	CBOISBNAdditionDlg dlg(this);
	dlg.SetAdditionISBN(strAdditionISBN);
	dlg.SetAdditionAddCode(strAdditionAddCode);
	dlg.SetAdditionMissISBN(strAdditionMissISBN);

	if (dlg.DoModal() == IDOK)
	{
		INT rowCnt = m_pDM_Book->GetRowCount();
		CString strVolSortNo = m_pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), nRepresentBookIdx);
		for(INT i=0 ; i<rowCnt ; i++)
		{
			if(strVolSortNo == m_pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), i))
			{
				m_pDM_Book->SetCellData(_T("BB_�߰�����ISBN"), dlg.GetAdditionISBN(), i);
				m_pDM_Book->SetCellData(_T("BB_�߰�����ISBN�ΰ���ȣ"), dlg.GetAdditionAddCode(), i);
				m_pDM_Book->SetCellData(_T("BB_�߰����ǿ���ISBN"), dlg.GetAdditionMissISBN(), i);
			}
		}
	}	
}

VOID CSpecies_Form::OnbISBNCHECK() 
{
	// TODO: Add your control notification handler code here
	// ���� üũ
	CString strEaISBNFrom;
	CString strEaISBNTo;
	INT ids = m_pCM_Index->GetControlMgrData(_T("IBS_����ISBN"), strEaISBNFrom);
	if (ids >= 0)
	{
		ids = m_pFormApi->CheckISBN(strEaISBNFrom, strEaISBNTo);
		if (ids >= 0)
			m_pCM_Index->SetControlMgrData(_T("IBS_����ISBN"), strEaISBNTo);
	}

	// ��Ʈ ü��
	CString strSetISBNFrom;
	CString strSetISBNTo;
	ids = m_pCM_Index->GetControlMgrData(_T("IBS_��Ʈ_ISBN1"), strSetISBNFrom);
	if (ids >= 0)
	{
		ids = m_pFormApi->CheckISBN(strSetISBNFrom, strSetISBNTo);
		if (ids >= 0)
			m_pCM_Index->SetControlMgrData(_T("IBS_��Ʈ_ISBN1"), strSetISBNTo);
	}	
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
//
// [ �̴Ͻ��� ]
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::OnbDEFAULTCONFIG() 
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT ids;
	CString strFormCode, strData, strPath, strMarc;
	CMarc marc;

	ids = ApplyData();
	if( ids < 0 )	return;

	// �� -> ��ũ
	m_pCM_Index->GetControlMgrData(_T("IBS_���ı���_�ڵ�"), strFormCode);
	if( strFormCode.IsEmpty() == TRUE )
	{
		m_pInfo->pCodeMgr->GetDefaultCode(_T("���ı���"), strFormCode);
	}

	strData = strFormCode + _T(".cfg");
	strPath = m_strDefaultValueFilePath;
	strPath.Replace(_T(".cfg"), strData);

	
	ids = m_pBOSpeciesApi->APPLY_Species_to_CMarc(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
	m_pInfo->pMarcMgr->CheckDeepSyntax(&marc);
	m_pInfo->pMarcMgr->CheckSimpleSyntax(&marc);
	m_pInfo->pMarcMgr->DeleteEmptySubfield(&marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(&marc);
	
	m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	m_pFormApi->SaveDefaultValue(strMarc, strPath);
	/*
	if (SaveDefaultValue() < 0)
		AfxMessageBox(_T("�⺻�� ���� ����"));
	else
	*/
	
	AfxMessageBox(_T("�⺻���� �����Ͽ����ϴ�."), MB_ICONINFORMATION);	

}

VOID CSpecies_Form::OnbDUPSEARCH() 
{
	// TODO: Add your control notification handler code here
	CString strSearchCount;

	if(m_bDupSearchType)
		SearchDuplicateData(_T("�Ǻ���������"), strSearchCount);
	else
		SearchDuplicateDataByTitle(_T("�����ڵ���������"), strSearchCount);

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

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ����ó�� ��ư
// [ �̴Ͻ��� ]
//
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::OnbAPPLYSERIES() 
{
	//=======================================================================
	// ��������
	//=======================================================================
	CString strTmpData;

	//=======================================================================
	// 1. ����ó�����θ� Ȯ���Ѵ�.
	//=======================================================================
	if( IDNO == MessageBox(_T("å�� �������� ����ϴ�. ����Ͻðڽ��ϱ�?"), NULL, MB_YESNO | MB_ICONQUESTION) ) return;

	// ApplyData�� �����ϸ� ������ ������ ����Ÿ�������� ������ [������]�� �ȴ�.
	// �̷��� ������ ���� ������ �ùٸ��� �ʴ�. ���� ApplyData�ϱ����� ����Ÿ���� �����ϰ� 
	// ������ ���Ŀ� �������� �ٽ� �����Ѵ�.
	m_pTailDlg->SetApplyPriceInfo(FALSE);

	// ApplyData
	if( ApplyData() < 0 )
	{
		MessageBox(_T("������������ ����"));
		return;
	}

	// ��� å�� 1�� 1å���� �����.
	m_pFormApi->ApplyCompleteWorks(m_pDM_Book, strTmpData);
	// ���������� �� �ǿ� ����
	m_pTailDlg->SetApplyPriceInfo(TRUE);
	m_pTailDlg->SetApplyVolPriceFromSpeciesPrice(FALSE);
	m_pTailDlg->ApplyData();
	// ���ΰ� ȭ�鿡 ����
	m_pCM_Index->SetControlMgrData(_T("IBS_�����"), _T(""));

	MessageBox(_T("å�� �������� ��������ϴ�."));

	Display();
	m_bViewVolInfo = TRUE;
}

VOID CSpecies_Form::OnbVOLINFO() 
{
	// �Է��� ������ ����
	ShowVolumeInfo(VOLUME_FIRST);
}

INT CSpecies_Form::ApplyRegNoToMarc(INT idx /*= 0*/)
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


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �з�ǥ���� �޺� ����
//
// [ �̴Ͻ��� ]
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::OnSelchangecCLASIFYCLASS() 
{
	//=======================================================================
	// ��������
	//=======================================================================	
	INT nDefaultValue, i, nCode = 256;
	CString class_name, strWhichCode, strDefaultValue;
	CString strCode[256], strDesc[256];
	//�ٲ𶧸��� �з� ������ �ٲپ� �ش�. 
	//if(m_nSpeciesType == BO_BASIC || m_nSpeciesType == BO_IMPORT || m_nSpeciesType == BO_REG_INVALID) return ;
	CESL_ControlMultiComboBox* pCM_WhichCode = (CESL_ControlMultiComboBox *)m_pCM_Index->FindControl(_T("IBS_�з�ǥ����"));	
    CESL_ControlMultiComboBox* pCM_Subject = (CESL_ControlMultiComboBox *)m_pCM_Accu->FindControl(_T("BP_��������_�ڵ�"));
	if( pCM_WhichCode == NULL || pCM_Subject == NULL )	return;
	
  
	//=======================================================================	
	// 1. �з�ǥ���п��� ���õǾ��� �ؽ�Ʈ�� �о� Ŭ�������� �����Ѵ�.
	//=======================================================================	
	pCM_WhichCode->GetData(strWhichCode, pCM_WhichCode->GetCurSel(), 1);
	strWhichCode.TrimLeft();		strWhichCode.TrimRight();
	strWhichCode.MakeUpper();
	
	if		( strWhichCode == _T("KDC") )	class_name = _T("KDC��������ȣ����");
	else if	( strWhichCode == _T("DDC") )	class_name = _T("DDC��������ȣ����");
	else if	( strWhichCode == _T("LC")	)	class_name = _T("LC������ȣ");
	else									class_name = _T("");

	//=======================================================================	
	// 2. ���������ڵ带 �ʱ�ȭ ��Ų��.
	//=======================================================================	
	pCM_Subject->FreeComboData();
	pCM_Subject->ResetContent();
	pCM_Subject->InitMultiComboBox(2);
	if( class_name.GetLength() == 0 )	return ;

	//=======================================================================	
	// 3. ���ǵ� Ŭ���������� DB���� �ڵ尪�� ������ �����´�.(MAX : 256)
	//=======================================================================	
	nDefaultValue = 0;
	m_pInfo->pCodeMgr->GetCode(class_name, strCode, strDesc, nCode, nDefaultValue);	
	if( nCode == 0 )	return ;

		
	//=======================================================================	
	// 4. ��������DM���� ���������ڵ带 �����´�.
	//=======================================================================	
	if( m_pDM_Accu != NULL )	
	{
		m_pDM_Accu->GetCellData(_T("BP_��������_�ڵ�"), 0, strDefaultValue);

		//=============================================================================================
		//2008.12.04 ADD BY LKS : ������� �ڵ尪�� "  "�� ������ �ʾ� �޺��ڽ��� �����ȵǴ� ���� ����.
		strDefaultValue.Replace(_T(" "), _T(""));
		if(strDefaultValue.IsEmpty())
		{
			//===================================================
			//2009.10.19 UPDATE BY LKS : ���Թ��� ��ũ�� 056�±� a���� ���ڸ� �о� ���������� Set
			CMarc marc;
			CString strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
			m_pInfo->pMarcMgr->Decoding(strMarc, &marc);		
			CString str056Field;
			m_pInfo->pMarcMgr->GetField(&marc, _T("056"), str056Field);
			
			if(!str056Field.IsEmpty())
			{
				INT idx = str056Field.FindOneOf(_T("a"));
				
				CString strSubjectCode,strTemp;
				strSubjectCode = str056Field.GetAt(idx+1);

				strDefaultValue = strSubjectCode;
			}
			else
			{
				strDefaultValue = _T("  ");
			}
			//===================================================

			//strDefaultValue = _T("  ");
		}
		//=============================================================================================
	}
		
	//=======================================================================	
	// 5. ���������޺��� �ڵ尪���� �����ϰ� �ڵ尪��� DM�� �ڵ尪�� ���Ͽ� �޺��� �����Ѵ�.
	//=======================================================================	
	for( i=0; i<nCode; i++ )
	{
		if( strCode[i].GetLength() == 0 )	strCode[i] = _T("  ");
		
		if( strDefaultValue == strCode[i] ) nDefaultValue = i;
	
		pCM_Subject->AddItem(strCode[i], 0, i);
		pCM_Subject->AddItem(strDesc[i], 1, i);
	}

	pCM_Subject->SetCurSel(nDefaultValue);

}

VOID CSpecies_Form::OnbCLOSE() 
{
	// TODO: Add your control notification handler code here
	// 2004.11.09. 5163 ǥ����� �߰�.
	INT ids;
	if( m_strUseCoverYN == _T("Y"))
	{
		if((m_nOpType == SINSERT) && (m_bInsertData == FALSE) && (m_szSpeciesKey[0] != '\0'))
		{
			CCOVER_MAIN dlg(this);
			ids = dlg.DeleteCoverInfo( m_szSpeciesKey );
			if( ids < 0 ) 
			{
				ESLAfxMessageBox(_T("ǥ�� ���� ����"));
				return ;
			}
		}
	}
	CDialog::OnOK();
}

INT CSpecies_Form::CheckWorkNoInfo(CESL_DataMgr * pDM_OUT)
{
	INT ids = m_pHeadDlg->InsertWorkNoInfo(pDM_OUT);
	if(ids < 0) return -1;
	
	return 0;
}

VOID CSpecies_Form::OnSelchangecFORMCLASS() 
{
	// TODO: Add your control notification handler code here
	//���ı����� ��������� �����Ͽ����� �����޽��� ���
	CString strFormCode;
	m_pCM_Index->GetControlMgrData(_T("IBS_���ı���_�ڵ�"), strFormCode);
	if(strFormCode.IsEmpty())
	{
		ESLAfxMessageBox(_T("���ı����� ��������� ������ �� �����ϴ�."));
		CESL_ControlMultiComboBox * pCmb = (CESL_ControlMultiComboBox*)m_pCM_Index->FindControl(_T("IBS_���ı���_�ڵ�"));
		m_pInfo->pCodeMgr->GetDefaultCode(_T("���ı���"), strFormCode);
		m_pCM_Index->SetControlMgrData(_T("IBS_���ı���_�ڵ�"), strFormCode);
		if(m_nOpType == SINSERT) LoadDefaultValue();
		else if(m_nOpType == SUPDATE) DeleteFormCodeMarcInfo();
		if(pCmb) pCmb->SetFocus();
		return ;
	}
	if(m_nOpType == SINSERT) 
	{
		CString strMsg, strTmpData;
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("���ı���"), strFormCode, strTmpData);
		strMsg.Format(_T("���ı���[%s:%s]�� ���� �⺻���� �����Ͻðڽ��ϱ�?"), strFormCode, strTmpData);
		if(IDYES == ESLAfxMessageBox(strMsg, MB_YESNO))
			LoadDefaultValue();
	}
}

INT CSpecies_Form::ClearScreen()
{
	CString strWorkingStatus, strFormCode;
	//m_pCM_Index->GetControlMgrData(_T("�ڷᱸ��"), strWorkingStatus);
	m_pCM_Index->GetControlMgrData(_T("IBS_���ı���_�ڵ�"), strFormCode);
	if (m_pCM_Index)
		m_pCM_Index->AllControlClear();
	if (m_pCM_Accu)
		m_pCM_Accu->AllControlClear();
	m_pDM_Species->SetCellData(_T("BS_MARC"), _T(""), 0);
	//m_pCM_Index->SetControlMgrData(_T("�ڷᱸ��"), strWorkingStatus);
	m_pCM_Index->SetControlMgrData(_T("IBS_���ı���_�ڵ�"), strFormCode);
	GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(_T("EM"));
	GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
	GetDlgItem(IDC_eVOL)->EnableWindow(TRUE);
	return 0;
}

VOID CSpecies_Form::SetParentGroupInfo(INT nAcqCode, CString strAcqYear, CString strGroupInfo, CString strCONNECTIONINFO)
{
	if(m_pGroupCollection)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = NULL;
	}
	this->SetCONNECTION_INFO(strCONNECTIONINFO);
	m_pGroupCollection = new CGroupCollection(this);
	
	m_pGroupCollection->SetAcqType((ACQ_TYPE)nAcqCode);

	if((ACQ_TYPE)nAcqCode == PURCHASE)
		m_pGroupCollection->SetWorkingStatus(_T("�˼��Ϸ�"));
	m_bCreateGroupCollection = TRUE;
}

UINT CSpecies_Form::OnNoteClicked(LPARAM lParam, WPARAM wParam)
{
	// ���� ���� ����
	CDuplicateSearch_Interface impl(this);
	impl.ShowTitleDuplicateSearchDlg(_T("�����ڵ���������"), m_strDupIndexKey);
	if( m_strDupIndexKey.IsEmpty() == FALSE )		MakeDupAddMode();

	if( m_pNoteCtrl != NULL )
	{
		delete m_pNoteCtrl;
		m_pNoteCtrl = NULL;
	}
	
	return 0;
}

VOID CSpecies_Form::MoveCtrlByAcqCode()
{	
	INT nCount = 0;
	INT nArrOtherResources[256];
	nArrOtherResources[nCount++] = IDC_STATIC_BOOK_REGCODE;
	nArrOtherResources[nCount++] = IDC_tREG_CLASS_CODE;
	nArrOtherResources[nCount++] = IDC_tTOTAL_VOLUMEP;
	nArrOtherResources[nCount++] = IDC_tTOTAL_BOOKP;
	nArrOtherResources[nCount++] = IDC_bAPPLY_SERIES;
	nArrOtherResources[nCount++] = IDC_bVOL_INFO;
	nArrOtherResources[nCount++] = IDC_eTOTAL_BOOKP;
	nArrOtherResources[nCount++] = IDC_eTOTAL_VOLUMEP;
	nArrOtherResources[nCount++] = IDC_eREG_CLASS_CODE;
	nArrOtherResources[nCount++] = IDC_bAPPENDIX;
	nArrOtherResources[nCount++] = IDC_bMARC_EDIT;
	nArrOtherResources[nCount++] = IDC_bMARC_IMPORT;
	nArrOtherResources[nCount++] = IDC_bKOLIS_NET;
	nArrOtherResources[nCount++] = IDC_bCLEARSCREEN;
	nArrOtherResources[nCount++] = IDC_bPREV_SPECIES;
	nArrOtherResources[nCount++] = IDC_bNEXT_SPECIES;
	nArrOtherResources[nCount++] = IDC_bSAVEAS;
	nArrOtherResources[nCount++] = IDC_bSAVE;
	nArrOtherResources[nCount++] = IDC_bCLOSE;
	
	CRect rcResource;
	CWnd * pWnd = NULL;
	for (INT i = 0; i <nCount ; i++)
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if(m_nAcqType == ACQ_TYPE_INVALID && m_bAcqTypeChange)
			{
				pWnd->GetWindowRect(rcResource);
				ScreenToClient(rcResource);
				rcResource.top -= 22;
				rcResource.bottom -= 22;
				pWnd->MoveWindow(rcResource);
			}
			else if(m_nAcqType != ACQ_TYPE_INVALID && m_bAcqTypeChange)
			{
				pWnd->GetWindowRect(rcResource);
				ScreenToClient(rcResource);
				rcResource.top += 22;
				rcResource.bottom += 22;
				pWnd->MoveWindow(rcResource);
			}
		}
	}

/*
	// Resize Window Size
	GetWindowRect(rcResource);
	if(m_nAcqType == ACQ_TYPE_INVALID)
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - 22,  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() + 22,  SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate();
	UpdateWindow();
*/
}

INT CSpecies_Form::DeleteFormCodeMarcInfo()
{
	CString strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	CMarc marc;
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	m_pInfo->pMarcMgr->DeleteField(&marc, _T("007"));
	m_pInfo->pMarcMgr->DeleteField(&marc, _T("008"));
	m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
	return 0;
}

HBRUSH CSpecies_Form::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//======================================================
	//20080612 UPDATE BY PJW : ���� ��ϱ��� �������κ� �ּ�ó���ϸ� �ȵ�
	if( IsColorControl(pWnd->GetDlgCtrlID()) ) 
	{
		return SetColor(pDC);
	}
	//======================================================
	


	return hbr;
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���Ա��п� ���� ���Խ�û��/��û���ڸ� Ȱ��ȭ/��Ȱ��ȭ �Ѵ�.
//
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::ShowControlByAcqType()
{
	
	// ���̱�� �ʱ�ȭ
	GetDlgItem(IDC_stcBSFORM_REQUESTOR   )->ShowWindow(SW_SHOW);	// ���Խ�û��
	GetDlgItem(IDC_edtBSFORM_REQUESTOR	 )->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_stcBSFORM_REQUEST_DATE)->ShowWindow(SW_SHOW);	// ��û����
	GetDlgItem(IDC_edtBSFORM_REQUEST_DATE)->ShowWindow(SW_SHOW);
	//20070809 ADD PJW : ��û���ڿ� �ú���(IDC_edtBSFORM_REQUEST_HOUR) �߰�
	GetDlgItem(IDC_edtBSFORM_REQUEST_HOUR)->ShowWindow(SW_SHOW);

	//=======================================================================
	// 5163/����/����� �÷��׿� ���� ��û���� �޺��ڽ��� Ȱ��ȭ��Ų��.
	//=======================================================================	
	if( m_strIs64 == _T("Y") || m_strIs90 == _T("Y") )	
	{	
		GetDlgItem(IDC_cAPPLICAND_KIND_CLASS	)->EnableWindow(TRUE);	// ��û�ڵ�(�μ�)
		GetDlgItem(IDC_cAPPLICAND_KIND_COURCE	)->EnableWindow(TRUE);	// ��û�ڵ�(�а�)
		GetDlgItem(IDC_cAPPLICAND_KIND_DEPT		)->EnableWindow(TRUE);	// ��û�ڵ�(��ó)
		GetDlgItem(IDC_cmbAPPLICANT				)->EnableWindow(TRUE);	// ��û����
	}
	else
	{
		GetDlgItem(IDC_cAPPLICAND_KIND_CLASS	)->EnableWindow(FALSE);	// ��û�ڵ�(�μ�)
		GetDlgItem(IDC_cAPPLICAND_KIND_COURCE	)->EnableWindow(FALSE);	// ��û�ڵ�(�а�)
		GetDlgItem(IDC_cAPPLICAND_KIND_DEPT		)->EnableWindow(FALSE);	// ��û�ڵ�(��ó)
		GetDlgItem(IDC_cmbAPPLICANT				)->EnableWindow(FALSE);	// ��û����
	}
		
	//=======================================================================
	// ���Ա��п� ���� ��û���� �޺��ڽ��� Ȱ��ȭ��Ų��.
	//=======================================================================
	if( m_nAcqType == ACQ_TYPE_INVALID )
	{
		((CEdit*)GetDlgItem(IDC_edtBSFORM_REQUESTOR)	)->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_edtBSFORM_REQUEST_DATE)	)->SetReadOnly(TRUE);
		//20070816 ADD PJW : ��û���� ReadOnly<FALSE,TRUE ����>
		if ( m_bDupAddMode == TRUE || m_bDupAddYN == TRUE )
		{
			m_hApplicantTime->Control_SetReadOnly(TRUE);
		}
		else
		{
			m_hApplicantTime->Control_SetReadOnly(FALSE);
		}


		((CComboBox*)GetDlgItem(IDC_cmbAPPLICANT)		)->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_btnSearchUser))->EnableWindow(FALSE);	// 2005-01-11 ��������ȸ ��ư�� ��Ȱ��ȭ ��Ų��.
		GetDlgItem(IDC_cAPPLICAND_KIND_CLASS	)->EnableWindow(FALSE);	// ��û�ڵ�(�μ�)
		GetDlgItem(IDC_cAPPLICAND_KIND_COURCE	)->EnableWindow(FALSE);	// ��û�ڵ�(�а�)
		GetDlgItem(IDC_cAPPLICAND_KIND_DEPT		)->EnableWindow(FALSE);	// ��û�ڵ�(��ó)
		GetDlgItem(IDC_cmbAPPLICANT				)->EnableWindow(FALSE);	// ��û����
	}
	else if( m_nAcqType == DONATE && m_strIs90 == _T("Y") )
	{
		((CComboBox*)GetDlgItem(IDC_cmbAPPLICANT)		)->EnableWindow(FALSE);
		GetDlgItem(IDC_cAPPLICAND_KIND_CLASS	)->EnableWindow(FALSE);	// ��û�ڵ�(�μ�)
		GetDlgItem(IDC_cAPPLICAND_KIND_COURCE	)->EnableWindow(FALSE);	// ��û�ڵ�(�а�)
		GetDlgItem(IDC_cAPPLICAND_KIND_DEPT		)->EnableWindow(FALSE);	// ��û�ڵ�(��ó)
		GetDlgItem(IDC_cmbAPPLICANT				)->EnableWindow(FALSE);	// ��û����
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_edtBSFORM_REQUESTOR)	)->SetReadOnly(FALSE);
		((CEdit*)GetDlgItem(IDC_edtBSFORM_REQUEST_DATE)	)->SetReadOnly(FALSE);
		//20070816 ADD PJW : ��û���� ReadOnly<FALSE,TRUE ����>
		if ( m_bDupAddMode == TRUE || m_bDupAddYN == TRUE )
		{
			m_hApplicantTime->Control_SetReadOnly(TRUE);
		}
		else
		{
			m_hApplicantTime->Control_SetReadOnly(FALSE);
		}

		((CButton*)GetDlgItem(IDC_btnSearchUser)		)->EnableWindow(TRUE);
	}

}

VOID CSpecies_Form::SetFurnishInfo()
{
	CString strInputType = m_pDM_Accu->GetCellData(_T("BP_�Է±���"), 0);
	CString strAcqKey = m_pDM_Accu->GetCellData(_T("BP_��������KEY"), 0);
	if(strAcqKey.IsEmpty()) return ;
	
	//ShowControlByAcqType();

	// 2005-02-16
	//
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pDM_Accu->CONNECTION_INFO);
	
	CString strQuery = _T("");	

	// 2006-01-16 ������
	// ��û���� FURNISH_DATE -> APPLICANT_DATE	
	//strQuery.Format(_T("SELECT APPLICANT_NAME, APPLICANT_DATE FROM BO_FURNISH_TBL WHERE ACQ_KEY=%s"), strAcqKey);
	strQuery.Format(_T("SELECT APPLICANT_NAME, TO_CHAR(APPLICANT_DATE, 'YYYY/MM/DD HH24:MI:SS') FROM BO_FURNISH_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_KEY=%s"), strAcqKey);
//DEL 	strQuery.Format(_T("SELECT APPLICANT_NAME, TO_CHAR(APPLICANT_DATE, 'YYYY/MM/DD HH24:MI:SS') FROM BO_FURNISH_TBL WHERE ACQ_KEY=%s"), strAcqKey);
	tmpDM.RestructDataManager(strQuery);

	if (tmpDM.GetRowCount() == 0) return ;
	CString strApplicantName = tmpDM.GetCellData(0, 0);
	CString strApplicantDate = tmpDM.GetCellData(0, 1);

	m_pCM_Index->SetControlMgrData(_T("���Խ�û��"), strApplicantName);
	//m_pCM_Index->SetControlMgrData(_T("���Խ�û��"), strApplicantDate);
	strApplicantTime(strApplicantDate);
	m_pDM_Index->SetCellData(_T("���Խ�û��"), strApplicantName, 0);
	m_pDM_Index->SetCellData(_T("���Խ�û��"), strApplicantDate, 0);
}


VOID CSpecies_Form::SetPurchaseInfo()
{
	// ���� �䱸���� - ��û���п� ȸ��, �μ�, �а�, �кΰ� �߰��Ǿ�� ��.
	// APPLICANT_CLASS�� ���� �� ���� - NULL(�������)�� ���� ó��
	// 2005-09-29 ������
	//
	CString strApply = _T("");
	CString strAppKey = _T("");
	strAppKey = m_pDM_Accu->GetCellData(_T("BP_��������KEY"), 0);
	strApply = m_pDM_Accu->GetCellData(_T("BP_��û����"), 0);
	
	//���� ��û ������ ������ combo�����ֱ�..
	//
	if ( strApply == _T("DEP") )
	{
		strApply = m_pDM_Accu->GetCellData(_T("BP_�к��ڵ�"), 0 );
		m_cmbApplicant.SetCurSel(3);
		m_pCM_Index->SetControlMgrData(_T("���Խ�û����_�к�"), m_pDM_Accu->GetCellData(_T("BP_�к��ڵ�"), 0 ) );
		//m_pDM_Index->SetCellData(_T("���Խ�û����_�к�"), m_pDM_Accu->GetCellData(_T("BP_�к��ڵ�"), 0 ) , 0);
	}
	else if ( strApply == _T("COU") )
	{
		m_cmbApplicant.SetCurSel(2);
		m_pCM_Index->SetControlMgrData(_T("���Խ�û����_�а�"), m_pDM_Accu->GetCellData(_T("BP_�а��ڵ�"), 0 ) );
		//m_pDM_Index->SetCellData(_T("���Խ�û����_�а�"), m_pDM_Accu->GetCellData(_T("BP_�а��ڵ�"), 0 ) , 0);
	}
	else if ( strApply == _T("CLA") )
	{
		m_cmbApplicant.SetCurSel(1);
		m_pCM_Index->SetControlMgrData(_T("���Խ�û����_�μ�"), m_pDM_Accu->GetCellData(_T("BP_�μ��ڵ�"), 0 ) );
		//m_pDM_Index->SetCellData(_T("���Խ�û����_�μ�"), m_pDM_Accu->GetCellData(_T("BP_�μ��ڵ�"), 0 ) , 0);
	}
	else
	{
		if( m_strIs64 == _T("Y") && strAppKey.IsEmpty() )
			m_cmbApplicant.SetCurSel(3);
		else
			m_cmbApplicant.SetCurSel(0);
	}

	OnSelchangecmbAPPLICANT();
}

VOID CSpecies_Form::OnSelchangecUSEOBJECTCLASS() 
{
	// TODO: Add your control notification handler code here
	CString strUseObjCode, strMarc;
	CMarc marc;
	m_pCM_Index->GetControlMgrData(_T("IBS_�̿��󱸺�_�ڵ�"), strUseObjCode);
	if(strUseObjCode == _T("JU"))
	{
		strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	}
}

INT CSpecies_Form::InsertAbstractContentsInfo()
{
    CBO_INERT_TABLE api(this);
    
    CString strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);//MARC
    CString strSpeciesKey = m_pDM_Species->GetCellData(_T("BS_��KEY"), 0);

    api.GetMarcSteam (strMarc) ;
    return api.SetInsertValue(strSpeciesKey, _T(""), m_strContents, m_strAbstracts);
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �������� ��ư
// [ �̴Ͻ��� ]
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::OnbtnPREVCONTENT() 
{
	if(!m_bInsertData) return ;

	// 2006.04.20 KBS - (�����䱸)�̸� ���Ե� ������ �ִٸ� �������� �ʵ��� �ּ�ó��
	//OnbCLEARSCREEN();

	//���� ����� �Է� ���������� �����´�.
	BOOL bUserUserInputMarc = FALSE;
	INT ids = ApplyData();
	if(ids < 0)	bUserUserInputMarc = FALSE;
	else bUserUserInputMarc = TRUE;

	if(bUserUserInputMarc)
	{
		ids = ApplyIndexToMarc();
		if(ids < 0) bUserUserInputMarc = FALSE;
		else bUserUserInputMarc = TRUE;
	}
	
	CMarc UserInputMarc, PrevMarc;
	CString strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	if(!strMarc.IsEmpty() && bUserUserInputMarc)
	{
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &UserInputMarc);
		if(ids < 0) bUserUserInputMarc = FALSE;
		else bUserUserInputMarc = TRUE;
	}

	m_nParentSelectedRow = m_pDM_Parent->GetRowCount() - 1;
	UpdateInitProc();
	ids = MakeUpdateDM();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("���� �۾������� �ҷ��ü� �����ϴ�."));
		return ;
	}
	if(m_bDupAddYN) m_bDupAddYN = FALSE;
	
	if(m_bAcqTypeChange) SetHeadTailInfoDlg();
	else m_pHeadDlg->Init();

	ApplyMarcToIndexCM();
	m_pTailDlg->Init();
	CString strSubjectCode, strWorkCode;
	m_pDM_Accu->GetCellData(_T("BP_��������_�ڵ�"), 0, strSubjectCode);

	//====================================================
	//2008.07.08 ADD BY PJW : ���������� ��������ϰ�� ��ũ�� �־��ش�.
	//m_pCM_Accu->SetControlMgrData(_T("BP_��������_�ڵ�"), strSubjectCode);
	if( strSubjectCode.IsEmpty() )
	{
		m_pCM_Accu->SetControlMgrData(_T("BP_��������_�ڵ�"), _T("  "));
	}
	else
	{
		m_pCM_Accu->SetControlMgrData(_T("BP_��������_�ڵ�"), strSubjectCode);
	}
	//====================================================

	//������������ ����
	m_pCM_Index->GetControlMgrData(_T("IBS_��������_�ڵ�"), strWorkCode);
	m_pDM_Accu->SetCellData(_T("BP_������������"), strWorkCode, 0);
	//�˻��� å �߿��� ��ǥå�� �����ϰ� �� �����Ѵ�.
	INT nRowCnt = m_pDM_Book->GetRowCount();
	if(nRowCnt > 1)
	{
		for(INT i=nRowCnt-1 ; i>=0 ; i--)
		{
			if(m_pDM_Book->GetCellData(_T("BB_��ǥå����"), i) != _T("Y"))
				m_pDM_Book->DeleteRow(i);
			if(m_pDM_Book->GetRowCount() == 1) break;
		}
		m_pDM_Book->SetCellData(_T("BB_��ǥå����"), _T("Y"), 0);
	}

	((CEdit*)GetDlgItem(IDC_edtBSFORM_REQUESTOR))->SetReadOnly(FALSE);
	((CEdit*)GetDlgItem(IDC_edtBSFORM_REQUEST_DATE))->SetReadOnly(FALSE);
	//20070816 ADD PJW : ��û���� ReadOnly<FALSE,TRUE ����>
	if ( m_bDupAddMode == TRUE || m_bDupAddYN == TRUE )
	{
		m_hApplicantTime->Control_SetReadOnly(TRUE);
	}
	else
	{
		m_hApplicantTime->Control_SetReadOnly(FALSE);
	}

	

	if( bUserUserInputMarc == TRUE )
	{
		ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species_NotNullData(&UserInputMarc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu);		
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("����� �Է������� ������ �� �����ϴ�."));
		}
	}
	
	// 2005-01-06 m_szSpeciesKey �ʱ�ȭ
	m_szSpeciesKey[0] = '\0';

	Display();
	SetEBookCtrl();
}

VOID CSpecies_Form::OnSelchangecWORKCODE() 
{
	// TODO: Add your control notification handler code here
	if(m_nAcqType != PURCHASE ) return;

	CString strWorkCode;
	m_pCM_Index->GetControlMgrData(_T("IBS_��������_�ڵ�"), strWorkCode);
	INT nWorkCode = _ttoi(strWorkCode.GetBuffer(0));
	::PostMessage(m_pHeadDlg->GetSafeHwnd(), CHANGE_WORK_CODE, nWorkCode, 0);
}

INT CSpecies_Form::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;
	INT ids = m_pGrid_Parent->SelectMakeList();
	ids = m_pGrid_Parent->SelectGetHeadPosition();
	if(ids < 0) m_bGridSelectYN = FALSE;
	else m_bGridSelectYN = TRUE;

	if(m_bGridSelectYN)
	{
		m_nLastIdx = m_pGrid_Parent->SelectGetTailPosition();
		m_nFirstIdx = m_pGrid_Parent->SelectGetHeadPosition();
	}
	else
	{	
		m_nFirstIdx = 0;
		m_nLastIdx = m_pDM_Parent->GetRowCount() - 1;
	}
	
	//===================================================
	//2009.05.21 UPDATE BY LKS : ������ư�� ������ ���� �׸��� ����Ŭ������ ���� ���� ����
	
	if( m_nOpType == GUPDATE || m_nOpType == SUPDATE )
	{
		nFirstSelIdx = m_pGrid_Parent->GetRow() - 1;
	}
//DEL	if( m_nOpType == GUPDATE )
//DEL	{
//DEL		nFirstSelIdx = m_pGrid_Parent->GetRow() - 1;
//DEL	}
//DEL	else if(m_nOpType == SUPDATE)
//DEL	{
//DEL		if(m_bGridSelectYN)
//DEL			nFirstSelIdx = m_pGrid_Parent->SelectGetHeadPosition();
//DEL		else
//DEL			nFirstSelIdx = m_pGrid_Parent->GetRow() - 1;
//DEL	}
	//===================================================
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;

	if(nFirstSelIdx ==  m_nFirstIdx)
		GetDlgItem(IDC_bPREV_SPECIES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bPREV_SPECIES)->EnableWindow(TRUE);
	if(nFirstSelIdx == m_nLastIdx)
		GetDlgItem(IDC_bNEXT_SPECIES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bNEXT_SPECIES)->EnableWindow(TRUE);
	

	return nFirstSelIdx;
}

INT CSpecies_Form::GetPrevSelectIndex()
{
	//===============================
	//2009.04.08 UPDATE BY PJW : �񱳸� ���ؼ� �ʱⰪ�� 0���� �����Ѵ�.
// 	INT nPrevSelIndex = -1;
 	INT nPrevSelIndex = 0;
	//===============================
	

	if(m_bGridSelectYN)
	{
		//================================================
		//2009.04.08 ADD BY PJW : �������� ������ ����,���� �Ұ�� �ε����� ���� ����
		while( 1 )
		{
			if(nPrevSelIndex == m_nParentSelectedRow || nPrevSelIndex < 0 )
			{
				break;
			}
			else if( nPrevSelIndex < m_nParentSelectedRow )
			{
				nPrevSelIndex = m_pGrid_Parent->SelectGetNext();
				if( nPrevSelIndex < 0 )
				{
					nPrevSelIndex = m_pGrid_Parent->SelectGetTailPosition();
					break;
				}
			}
			else if( nPrevSelIndex > m_nParentSelectedRow )
			{
				nPrevSelIndex = m_pGrid_Parent->SelectGetPrev();
			}
		}
		//================================================

		if(m_nFirstIdx == m_pGrid_Parent->SelectGetIdx())
			return m_pGrid_Parent->SelectGetIdx();
		
		nPrevSelIndex = m_pGrid_Parent->SelectGetPrev();
	}
	else
	{
		if(m_nFirstIdx == m_nParentSelectedRow)
			return m_nParentSelectedRow;

		nPrevSelIndex = m_nParentSelectedRow - 1;
	}
	
	if(nPrevSelIndex ==  m_nFirstIdx)
		GetDlgItem(IDC_bPREV_SPECIES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bPREV_SPECIES)->EnableWindow(TRUE);
	if(nPrevSelIndex == m_nLastIdx)
		GetDlgItem(IDC_bNEXT_SPECIES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bNEXT_SPECIES)->EnableWindow(TRUE);
	
	return nPrevSelIndex;
}

INT CSpecies_Form::GetNextSelectIndex()
{
	//===============================
	//2009.04.08 ADD BY PJW : �񱳸� ���ؼ� �ʱⰪ�� 0���� �����Ѵ�.
// 	INT nNextSelIndex = -1;
 	INT nNextSelIndex = 0;
	//===============================
	
	if(m_bGridSelectYN)
	{
		//================================================
		//2009.04.08 ADD BY PJW : �������� ������ ����,���� �Ұ�� �ε����� ���� ����
		while( 1 )
		{
			if(nNextSelIndex == m_nParentSelectedRow || nNextSelIndex < 0 )
			{
				break;
			}
			else if( nNextSelIndex < m_nParentSelectedRow )
			{
				nNextSelIndex = m_pGrid_Parent->SelectGetNext();
			}
			else if( nNextSelIndex > m_nParentSelectedRow )
			{
				nNextSelIndex = m_pGrid_Parent->SelectGetPrev();
			}
		}
		//================================================

		if(m_nLastIdx == m_pGrid_Parent->SelectGetIdx())
		{
			return m_pGrid_Parent->SelectGetIdx();
		}

		nNextSelIndex = m_pGrid_Parent->SelectGetNext();
	}
	else
	{
		if(m_nLastIdx == m_nParentSelectedRow) 
			return m_nParentSelectedRow;
		
		nNextSelIndex = m_nParentSelectedRow + 1;
	}
	if(nNextSelIndex ==  m_nFirstIdx)
		GetDlgItem(IDC_bPREV_SPECIES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bPREV_SPECIES)->EnableWindow(TRUE);
	if(nNextSelIndex == m_nLastIdx)
		GetDlgItem(IDC_bNEXT_SPECIES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bNEXT_SPECIES)->EnableWindow(TRUE);
	
	return nNextSelIndex;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��ϱ��� ����Ʈ �ڽ��� �⺻���������� �����Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::InitRegCodeField()
{
	//=======================================================================
	// 1. ��ϱ����� �⺻���������� �����Ѵ�.
	//=======================================================================
	if( m_strRegCodeField.IsEmpty() == TRUE )
	{
		// DB���� �⺻���� �����Ǿ� �ִ��� �Ǵ��Ͽ� ���ٸ� EM�� �⺻���� �Ѵ�.
		m_pInfo->pCodeMgr->GetDefaultCode(_T("��ϱ���"), m_strRegCodeField);
		if( m_strRegCodeField.IsEmpty() == TRUE )	m_strRegCodeField = _T("EM");
	}
	else
	{
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		m_strRegCodeField = m_strRegCodeField.Left(2);
		INT nRegCodeLength;
		nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
		m_strRegCodeField = m_strRegCodeField.Left(nRegCodeLength);
		//===================================================================
	}
	
	GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
	GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
	GetDlgItem(IDC_eVOL)->EnableWindow(TRUE);

	SetEBookCtrl(TRUE);
}

VOID CSpecies_Form::ShowCodeResourceByMediaCode(CString strMediaCode, BOOL bInit/*=FALSE*/)
{
EFS_BEGIN
	INT i = 0;
	INT nCodeResourcesHeight = 65;
	CWnd *pWnd = NULL;

	if(strMediaCode == _T("EB")) m_bShowEBookInfo = TRUE;
	else m_bShowEBookInfo = FALSE;

	CRect rcWindow;
	GetWindowRect(rcWindow);
	if (m_bShowEBookInfo && rcWindow == m_rcWindow) return;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	nArrCodeResourceID[nCount++] = IDC_STATIC_EBOOK_GROUP;
	nArrCodeResourceID[nCount++] = IDC_stcEBOOK_COVER_FILE;
	nArrCodeResourceID[nCount++] = IDC_edtEBOOK_COVER_FILE;
	nArrCodeResourceID[nCount++] = IDC_btnFIND_FILE1;
	nArrCodeResourceID[nCount++] = IDC_btnFIND_FILE2;
	nArrCodeResourceID[nCount++] = IDC_btnFIND_FILE3;
	nArrCodeResourceID[nCount++] = IDC_stcEBOOK_FILE;
	nArrCodeResourceID[nCount++] = IDC_edtEBOOK_FILE;
	nArrCodeResourceID[nCount++] = IDC_stcEBOOK_EXECUTE_FILE;
	nArrCodeResourceID[nCount++] = IDC_edtEBOOK_EXECUTE_FILE;
	nArrCodeResourceID[nCount++] = IDC_btnEBOOK_VIEW;

	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (m_bShowEBookInfo)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	// Move Other Resources
	nCount = 0;
	INT nArrOtherResources[256];
	nArrOtherResources[nCount++] = IDC_bMARC_EDIT;
	nArrOtherResources[nCount++] = IDC_bMARC_IMPORT;
	nArrOtherResources[nCount++] = IDC_bKOLIS_NET;
	nArrOtherResources[nCount++] = IDC_bAPPENDIX;
	nArrOtherResources[nCount++] = IDC_bCLEARSCREEN;
	nArrOtherResources[nCount++] = IDC_bPREV_SPECIES;
	nArrOtherResources[nCount++] = IDC_bNEXT_SPECIES;
	nArrOtherResources[nCount++] = IDC_btnPREV_CONTENT;
	nArrOtherResources[nCount++] = IDC_bSAVEAS;
	nArrOtherResources[nCount++] = IDC_bSAVE;
	nArrOtherResources[nCount++] = IDC_bCLOSE;
	
	CRect rcResource;
	for (i = 0; i <nCount ; i++)
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->GetWindowRect(rcResource);
			ScreenToClient(rcResource);
			if (m_bShowEBookInfo)
			{
				if(!bInit)
				{
					rcResource.top += nCodeResourcesHeight;
					rcResource.bottom += nCodeResourcesHeight;
					pWnd->MoveWindow(rcResource);
				}
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
	if (m_bShowEBookInfo)
	{		
		if(m_bShowCode)
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
		else
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height()-97,  SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		if(m_bShowCode)
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - (nCodeResourcesHeight),  SWP_NOMOVE | SWP_NOZORDER);
		else
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - (nCodeResourcesHeight+97),  SWP_NOMOVE | SWP_NOZORDER);
	}
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CSpecies_Form::OnSelchangecMEDIACLASS() 
{
	// TODO: Add your control notification handler code here
	SetEBookCtrl();
}

VOID CSpecies_Form::OnbtnFINDFILE1() 
{
	// TODO: Add your control notification handler code here
	m_pFormApi->EBookFileOpen(this, EBOOK_COVER, m_pCM_Index);
}

VOID CSpecies_Form::OnbtnFINDFILE3() 
{
	// TODO: Add your control notification handler code here
	m_pFormApi->EBookFileOpen(this, EBOOK_EXECUTER, m_pCM_Index);
}

VOID CSpecies_Form::OnbtnFINDFILE2() 
{
	// TODO: Add your control notification handler code here
	m_pFormApi->EBookFileOpen(this, EBOOK_FILE, m_pCM_Index);
}

VOID CSpecies_Form::OnbtnEBOOKVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strExeFile, strEBookFile, strBookKey;
	INT nVolCnt = m_pFormApi->GetVolumeCount(m_pDM_Book);
	if(nVolCnt == 1)
	{
		m_pCM_Index->GetControlMgrData(_T("IBS_��������"), strEBookFile);
		m_pCM_Index->GetControlMgrData(_T("IBS_��������"), strExeFile);
		INT nIdx;
		m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
		if(nIdx < 0) nIdx = 0;
		strBookKey = m_pDM_Book->GetCellData(_T("BB_åKEY"), nIdx);
		m_pFormApi->EBookView(strBookKey, strExeFile, strEBookFile);
	}
	else
	{
		ShowVolumeInfo(BOOK_FIRST);
	}
}

VOID CSpecies_Form::SetEBookCtrl(BOOL bEnable)
{
	GetDlgItem(IDC_btnFIND_FILE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_btnFIND_FILE2)->EnableWindow(bEnable);
	GetDlgItem(IDC_btnFIND_FILE3)->EnableWindow(bEnable);

	((CEdit*)GetDlgItem(IDC_edtEBOOK_EXECUTE_FILE))->SetReadOnly(!bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �Ǻ������� �����͸� �����Ѵ�.
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::ShowVolumeInfo(INT nTabOrder)
{
	INT openMode;
	CBOVolumeDlg dlg(this, m_pDM_Book);

	ApplyData();

	// �� ȭ�� ����
	if( m_nOpType == SINSERT )		openMode = 1100;
	else							openMode = -1100;
	dlg.SetOpenMode(openMode);

	if( m_bDupAddMode == TRUE )
	{
		dlg.SetDupIdxKey(m_strDupIndexKey);
	}
	dlg.SetTabOrder(nTabOrder);
	dlg.DoModal();

	Display();
	m_bViewVolInfo = TRUE;	
	SetEBookCtrl();
}

INT CSpecies_Form::InitEBookCtrl()
{
	// E-Book���� Ctrl����
	CString strMediaCode;

	m_pCM_Index->GetControlMgrData(_T("IBS_��ü����_�ڵ�"), m_strPrevMediaCode);
	if( m_strPrevMediaCode == _T("EB") )	m_bShowEBookInfo = TRUE;
	else m_bShowEBookInfo = FALSE;
	
	// �ڵ� ���а� ��Ʈ�� �����ֱ�
	ShowCodeResource();
	ShowCodeResourceByMediaCode(m_strPrevMediaCode, TRUE);

	return 0;
}

INT CSpecies_Form::SetEBookCtrl()
{
	CString strMediaCode;
	m_pCM_Index->GetControlMgrData(_T("IBS_��ü����_�ڵ�"), strMediaCode);
	if(m_strPrevMediaCode != strMediaCode && (strMediaCode == _T("EB") || m_strPrevMediaCode == _T("EB")))
		ShowCodeResourceByMediaCode(strMediaCode);
	m_strPrevMediaCode = strMediaCode;
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �η�DM�� �����Ѵ�.
// [ �̴Ͻ��� ]
// 1. �η�DM�� ���ٸ� ���������Ѵ�.
// 2. CM�ǰ��� �η�DM�� �����Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::ApplyAppendixDM()
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT nRowCnt, i;
	CStringArray ArrIgnoreList;

	//=======================================================================
	// 1. �η�DM�� ���ٸ� ���������Ѵ�.
	//=======================================================================
	if( m_pDM_Appendix == NULL ) return -1;

	//=======================================================================
	// 2. CM�ǰ��� �η�DM�� �����Ѵ�.
	//=======================================================================
	nRowCnt = m_pDM_Appendix->GetRowCount();
	if( nRowCnt == 0 )		return 0;	
	
	ArrIgnoreList.Add(_T("IBS_��������_�ڵ�"));
	ArrIgnoreList.Add(_T("IBS_��ü����_�ڵ�"));
	ArrIgnoreList.Add(_T("IBS_��ġ��ȣ_�ڵ�"));


	ArrIgnoreList.Add(_T("IBS_�����"));
	ArrIgnoreList.Add(_T("IBS_������"));
	ArrIgnoreList.Add(_T("IBS_�����"));	
	
	for( i=0 ; i<nRowCnt ; i++ )
	{
		m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Appendix, i, _T("BB"), &ArrIgnoreList);
	}	
	
	return 0;
}

INT CSpecies_Form::ApplyRepresentBookDataToIndex(CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_INDEX, CESL_ControlMgr * pCM_INDEX)
{
	//�ٱ����� �ľ��Ѵ�.
	INT nVolCnt = m_pFormApi->GetVolumeCount(pDM_BOOK);
	INT nIdx = -1;
	if(nVolCnt == 1)
	{
		// ��ǥå�� ã�´�.
		m_pFormApi->GetRepresentationBook(pDM_BOOK, nIdx);
		if(nIdx < 0) nIdx = 0;
		// ��ǥå�� ���ο� �ִ´�.
		m_pFormApi->CopyDM(pDM_BOOK, nIdx, pDM_INDEX, 0, _T("IBS"));
	}
	else
	{
		// 2007.08.13 ADD BY PDJ
		// �ٱǺ��̶� ��ǥå�� ������ ���� ��ġ��Ų��.
		// ��ǥå�� ã�´�.
		m_pFormApi->GetRepresentationBook(pDM_BOOK, nIdx);		

		// å�� �ڵ� ������ ������ �ڵ� ������ �ݿ� �Ͽ��� �Ѵ�.		
		CStringArray ArrAccept;
		ArrAccept.Add(_T("BB_��������_�ڵ�"));
		ArrAccept.Add(_T("BB_��ü����_�ڵ�"));
		ArrAccept.Add(_T("BB_��ġ��ȣ_�ڵ�"));
		ArrAccept.Add(_T("BB_�̿����ѱ���_�ڵ�"));
		ArrAccept.Add(_T("BB_�̿��󱸺�_�ڵ�"));		
		m_pFormApi->CopyDMInArray(pDM_BOOK, nIdx, pDM_INDEX, 0, &ArrAccept, _T("IBS"));
	}
	pCM_INDEX->AllControlDisplay(0);

	m_pFormApi->DisplayISBN(pCM_INDEX, _T("IBS_����ISBN"));
	m_pFormApi->DisplayISBN(pCM_INDEX, _T("IBS_��Ʈ_ISBN1"));
	
	return 0;
}

INT CSpecies_Form::ApplyRepresentBookDataToSpecies(CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_SPECIES)
{
	//�ٱ����� �ľ��Ѵ�.
	INT nVolCnt = m_pFormApi->GetVolumeCount(pDM_BOOK);
	if(nVolCnt > 1) return 0;
	
	// ��ǥå�� ã�´�.
	INT nIdx = -1;
	m_pFormApi->GetRepresentationBook(pDM_BOOK, nIdx);
	if(nIdx < 0) nIdx = 0;
	// ��ǥå�� ���� �ִ´�.
	m_pFormApi->CopyDM(pDM_BOOK, nIdx, pDM_SPECIES, 0, _T("BS"));
	
	return 0;
}

INT CSpecies_Form::ApplyUserInputMarc(CMarc *pUserInputMarc, CMarc *pPrevMarc)
{
	//���������� ����� ��ũTag
	//008, 020, 100, 245, 250, 260, 300, 440, 490, 700, 950
	if(pUserInputMarc == NULL || pPrevMarc == NULL) return -1;
	CStringArray ArrTag;
	ArrTag.Add(_T("008"));	ArrTag.Add(_T("020"));	ArrTag.Add(_T("100"));	ArrTag.Add(_T("245"));
	ArrTag.Add(_T("250"));	ArrTag.Add(_T("260"));	ArrTag.Add(_T("300"));	ArrTag.Add(_T("440"));
	ArrTag.Add(_T("490"));	ArrTag.Add(_T("700"));	ArrTag.Add(_T("950"));
	CArray<CString, CString&> ArrData;	ArrData.RemoveAll();
	CString strTmpData, strTmpValue;
	INT ids;
	TCHAR cFirstInd, cSecondInd;
	for(INT i=0 ; i<ArrTag.GetSize() ; i++)
	{
		ArrData.RemoveAll();
		ids = m_pInfo->pMarcMgr->GetField(pUserInputMarc, ArrTag.GetAt(i), strTmpData, &ArrData);
		if(ids < 0) continue;
		if(ArrData.GetSize() == 0) continue;
		for(INT j=0 ; j<ArrData.GetSize() ; j++)
		{
			strTmpData.Empty();
			strTmpData = ArrData.GetAt(j);
			if(strTmpData.IsEmpty()) continue;
			cFirstInd = strTmpData.GetAt(0);
			cSecondInd = strTmpData.GetAt(1);
			ids = m_pInfo->pMarcMgr->DeleteField(pPrevMarc, ArrTag.GetAt(i));//, cFirstInd, cSecondInd);
			if(ids < 0) continue;
			m_pInfo->pMarcMgr->GetField(pPrevMarc, ArrTag.GetAt(i), strTmpValue);
			strTmpData = ArrTag.GetAt(i) + strTmpData;
			ids = m_pInfo->pMarcMgr->InsertField(pPrevMarc, strTmpData);
			if(ids < 0) continue;
			m_pInfo->pMarcMgr->GetField(pPrevMarc, ArrTag.GetAt(i), strTmpData);
		}
	}
	
	return 0;
}

BOOL CSpecies_Form::IsHoldDataMarc(CESL_DataMgr *pDM_BOOK)
{
	INT nCnt = pDM_BOOK->GetRowCount();
	for(INT i=0 ; i<nCnt ; i++)
	{
		if(pDM_BOOK->GetCellData(_T("BB_�ڷ����"), i).Left(3) == _T("BOL"))
			return FALSE;
	}
	return FALSE;
}

VOID CSpecies_Form::SearchDuplicateData(CString strSearchType, CString &strSearchCount)
{
	INT ids = ApplyData();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�Է��Ͻ� ������ �ݿ��� �� �����ϴ�."));
		return ;
	}
	CMarc marc;
	ids = m_pBOSpeciesApi->APPLY_Species_to_CMarc(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�Է��Ͻ� �������� ��ũ������ ������ �� �����ϴ�."));
		return ;
	}
	CString strVolInfo, strVolTitleInfo, strShelfCodeInfo, strRegCodeInfo, strMediaCodeInfo, strMarc;
	m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	m_pDM_Index->GetCellData(_T("IBS_��ü����_�ڵ�"), 0, strMediaCodeInfo);
	if(m_pDM_Book->GetRowCount() > 0)
	{
		m_pBOSpeciesApi->GetBoBookInfo(m_pDM_Book, _T("BB_�����"), strVolInfo);
		m_pBOSpeciesApi->GetBoBookInfo(m_pDM_Book, _T("BB_������ηϸ�"), strVolTitleInfo);
		m_pBOSpeciesApi->GetBoBookInfo(m_pDM_Book, _T("BB_��ġ��ȣ_�ڵ�"), strShelfCodeInfo);
		m_pBOSpeciesApi->GetBoBookInfo(m_pDM_Book, _T("BB_��ϱ���_�ڵ�"), strRegCodeInfo);
	}
	// ��������϶� �ڽ��� ��Ű�� �����´�.
	CString strSpeciesKey;
	if (m_nOpType != SINSERT && m_nParentSelectedRow >= 0)
		m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, m_nParentSelectedRow, strSpeciesKey);

	CDuplicateSearch_Interface impl(this);
	ids = impl.SearchDuplicateDataByIndividually(strSearchType, strMarc, strMediaCodeInfo, strVolInfo, strVolTitleInfo, strShelfCodeInfo, strRegCodeInfo, strSpeciesKey, strSearchCount);
	if (ids < 0) return;	
}

VOID CSpecies_Form::OnKillfocusedtAPPENDIX() 
{
	// TODO: Add your control notification handler code here
	m_pTailDlg->PostMessage(SET_TAB_ORDER, 0, 0);
}

UINT CSpecies_Form::OnSetTabOrder(LPARAM lParam, WPARAM wParam)
{
	if(wParam == 1)
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetFocus();
	else
		GetDlgItem(IDC_edtWORKING_STATUS)->SetFocus();
	return 0;
}

BOOL CSpecies_Form::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_TAB && pMsg->hwnd == GetDlgItem(IDC_bCLOSE)->GetSafeHwnd() )
		{
			m_pHeadDlg->PostMessage(SET_TAB_ORDER, 0, 0);
		}

		//===================================================
		//2009.04.13 UPDATE BY LKS : ���ڰ� ���� ���α׷� �ٿ�Ǵ� ���� ����
//DEL		if(GetDlgItem(IDC_ePUBLISH_YEAR)->m_hWnd == pMsg->hwnd )
//DEL		{
//DEL			INT nVirtKey = (INT) pMsg->wParam;
//DEL			if ((nVirtKey < '0' || nVirtKey > '9') && (nVirtKey != '-') && nVirtKey != 8 && (nVirtKey != 189) && (nVirtKey != 109))
//DEL			{
//DEL				if(!(nVirtKey >= VK_NUMPAD0 && nVirtKey <= VK_NUMPAD9) && nVirtKey!=VK_DELETE && nVirtKey!=VK_LEFT && nVirtKey!=VK_RIGHT)
//DEL				{
//DEL					ESLAfxMessageBox(_T("���ڸ� �Է��Ͻ� �� �ֽ��ϴ�.")); 
//DEL					return TRUE;
//DEL				}
//DEL			}
//DEL		}
		//===================================================
		
		if( pMsg->wParam == VK_ESCAPE )		return TRUE;
		else if( pMsg->wParam == VK_RETURN )
		{
			if( GetDlgItem(IDC_eTITLE)->m_hWnd == pMsg->hwnd )	OnbDUPSEARCH();
			return TRUE;
		}
		
		// 2005-01-19
		// ���� �����ִ� ��쿡�� �� �������� SET�Ѵ�.
		return CESL_Mgr::PreTranslateMessage(pMsg);
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

// 5163_3.����� ���� �ڵ带 ����� ��� ����ϴ� �ڵ常 ȭ�鿡 �����ش�.
// BEGIN:
VOID CSpecies_Form::ShowUserDefineCode(BOOL bShowCode)
{
	CString strYN, strAlias;
	CWnd *pWnd = NULL;
	CESL_DataMgr * TmpDM;
	TmpDM = FindDM(_T("DMUP_����_��_������"));

	// ����� ���� �ڵ�1 ��� ����.
	strYN.Empty();
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 91"), strYN);
	if(strYN == _T("Y"))
	{
		// 2004.12.06 �ڵ�� �ٲ��ش�.
		TmpDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 91"), strAlias);
		pWnd = GetDlgItem(IDC_stcUSERCODE1);        pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_cmbUSERCODE1);		pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		pWnd = GetDlgItem(IDC_stcUSERCODE1);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbUSERCODE1);		pWnd->ShowWindow(SW_HIDE);
	}

	// ����� ���� �ڵ�2 ��� ����.
	strYN.Empty();
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 92"), strYN);
	if(strYN == _T("Y"))
	{
		TmpDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 92"), strAlias);
		pWnd = GetDlgItem(IDC_stcUSERCODE2);		pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_cmbUSERCODE2);		pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		pWnd = GetDlgItem(IDC_stcUSERCODE2);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbUSERCODE2);		pWnd->ShowWindow(SW_HIDE);
	}

	// ����� ���� �ڵ�3 ��� ����.
	strYN.Empty();
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 93"), strYN);
	if(strYN == _T("Y") && bShowCode)
	{
		TmpDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 93"), strAlias);
		pWnd = GetDlgItem(IDC_stcUSERCODE3);		pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_cmbUSERCODE3);		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_eDISCRIPTION_SMALL);	pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_eDISCRIPTION);		pWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		pWnd = GetDlgItem(IDC_stcUSERCODE3);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbUSERCODE3);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_eDISCRIPTION_SMALL);	pWnd->ShowWindow(SW_HIDE);
	}
}
// END:

VOID CSpecies_Form::OnbtnCover() 
{
	// 2004.11.09 5163 ǥ�� �߰� ���.
	if( m_strUseCoverYN == _T("Y"))
	{
		CString strSpeciesKey;
		strSpeciesKey.Empty();

		if ( m_nOpType == SINSERT ) {
			CCOVER_MAIN dlg(this);
			dlg.SetInsertMode(TRUE);
			dlg.SetPubType(_MONO_);
			dlg.SetBasicName( m_szSpeciesKey );
			dlg.SetSpeciesKey(m_szSpeciesKey);
			dlg.SetLocalPath( _T("..\\bmp\\cover_info\\") );
			dlg.DoModal();
			dlg.GetSpeciesKey(strSpeciesKey);
			_tcscpy ( m_szSpeciesKey , strSpeciesKey.GetBuffer(0) );

		} else if( m_nOpType == SUPDATE || m_nOpType == GUPDATE){
			CCOVER_MAIN dlg(this);
			dlg.SetInsertMode(FALSE);
			dlg.SetPubType(_MONO_);
			dlg.SetLocalPath( _T("..\\bmp\\cover_info\\") );
			dlg.SetBasicName( m_szSpeciesKey );
			dlg.SetSpeciesKey( m_szSpeciesKey );
			dlg.DoModal();
		}
	}
}

// 2004.12.29
VOID CSpecies_Form::OnbtnSearchUser() 
{
	// TODO: Add your control notification handler code here
	CBO_ACQ_LOAN_USER_SEARCH_DLG UserSearchDlg(this);
	// 2005.01.05
	CString strApplicant;
	strApplicant.Empty();
	m_pCM_Index->GetControlMgrData(_T("���Խ�û��"), strApplicant);
	UserSearchDlg.SetUserID(strApplicant);

	UserSearchDlg.DoModal();

	CString sUserKey = UserSearchDlg.m_sSelectRecKey;
	if( sUserKey.IsEmpty() ) return ;

	CString strQuery, strName;
	strQuery.Format(_T("SELECT USER_NO FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s"), sUserKey);

	INT ids;

	CESL_DataMgr * TmpDM;
	TmpDM = FindDM(_T("DMUP_����_��_������"));
	ids = TmpDM->GetOneValueQuery(strQuery, strName);
	if( ids < 0 )
	{
		AfxMessageBox(_T("�����ڹ�ȣ�� �������� ���߽��ϴ�."));
		return ;
	}
	
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_edtBSFORM_REQUESTOR);
	pWnd->SetWindowText(strName);
	//pWnd->SetDlgItemText(IDC_edtBSFORM_REQUESTOR, strName);
	
}


VOID CSpecies_Form::OnSelchangecmbAPPLICANT() 
{
	// �ش縮�ҽ��� ��� �����.
	GetDlgItem(IDC_tClass_Code				)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_cAPPLICAND_KIND_DEPT		)->ShowWindow(SW_HIDE);		//�к�(ó)
	GetDlgItem(IDC_cAPPLICAND_KIND_COURCE	)->ShowWindow(SW_HIDE);		//�а�
	GetDlgItem(IDC_cAPPLICAND_KIND_CLASS	)->ShowWindow(SW_HIDE);		//�μ�
	

	INT nIdx = m_cmbApplicant.GetCurSel();
	// 0 - ȸ��
	// 1 - �μ�
	// 2 - �а�
	// 3 - �к�(ó)
	switch(nIdx)
	{	
	case 1:
		{
			GetDlgItem(IDC_cAPPLICAND_KIND_CLASS)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_tClass_Code)->ShowWindow(SW_SHOW);	
		}
		break;
	case 2:
		{
			GetDlgItem(IDC_cAPPLICAND_KIND_COURCE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_tClass_Code)->ShowWindow(SW_SHOW);
		}
		break;
	case 3:
		{
			GetDlgItem(IDC_cAPPLICAND_KIND_DEPT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_tClass_Code)->ShowWindow(SW_SHOW);
		}
		break;
	case 0:
	default:
		break;
	}
}

VOID CSpecies_Form::OnSelchangecAPPLICANDKINDCLASS() 
{
	
}

VOID CSpecies_Form::OnSelchangecAPPLICANDKINDCOURCE() 
{
	
}

VOID CSpecies_Form::OnSelchangecAPPLICANDKINDDEPT() 
{
	
}

VOID CSpecies_Form::SetIME(INT nIDC, INT nMode)
{
	EFS_BEGIN

    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(nIDC)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		if ( nMode == 0 ) dwConversion=IME_CMODE_ALPHANUMERIC;
		else dwConversion=IME_CMODE_HANGUL;		
		
		//20080218 DELETE BY PJW : ��/�� ������ ��Ŀ�� �̵��� ������� �ʵ��� ����			
		//ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(nIDC)->m_hWnd,hIME); 
    }

	EFS_END
}

VOID CSpecies_Form::OnSetfocusedtSERIESVOL() 
{
	SetIME(IDC_edtSERIES_VOL, 1);	
}

VOID CSpecies_Form::OnSetfocusedtPHYSICALCHARACTER() 
{
	SetIME(IDC_edtPHYSICAL_CHARACTER, 1);	
}

VOID CSpecies_Form::OnSetfocusedtSIZE() 
{
	SetIME(IDC_edtSIZE, 1);	
}

VOID CSpecies_Form::OnSetfocuseEDITIONSTMT() 
{
	SetIME(IDC_eEDITION_STMT, 1);
}

VOID CSpecies_Form::OnSetfocuseSERIESTITLE() 
{
	SetIME(IDC_eSERIES_TITLE, 1);	
}

VOID CSpecies_Form::OnSetfocuseREGCLASSCODE() 
{
	SetIME(IDC_eREG_CLASS_CODE, 0);	
}

VOID CSpecies_Form::OnSetfocuseTITLE() 
{
	SetIME(IDC_eTITLE, 1);
}

VOID CSpecies_Form::OnSetfocuseVOL() 
{
	SetIME(IDC_eVOL, 1);
}

VOID CSpecies_Form::OnSetfocusePUBLISHER() 
{
	SetIME(IDC_ePUBLISHER, 1);
}

VOID CSpecies_Form::OnSetfocusePUBLISHPLACE() 
{
	SetIME(IDC_ePUBLISH_PLACE, 1);
}

VOID CSpecies_Form::OnSetfocusedtAPPENDIX() 
{
	SetIME(IDC_edtAPPENDIX, 1);
}

VOID CSpecies_Form::OnSetfocuseDISCRIPTIONSMALL() 
{
	SetIME(IDC_eDISCRIPTION_SMALL, 1);
}

VOID CSpecies_Form::OnSetfocuseDISCRIPTION() 
{
	SetIME(IDC_eDISCRIPTION, 1);
}

VOID CSpecies_Form::OnSetfocuseAUTHOR1() 
{
	SetIME(IDC_eAUTHOR1, 1);
}

VOID CSpecies_Form::OnSetfocusedtBSFORMREQUESTOR() 
{
	SetIME(IDC_edtBSFORM_REQUESTOR, 1);	
}


//20070810 ADD PJW : ��û���ڿ��� ��, ��, ��
VOID CSpecies_Form::strApplicantTime(CString strDateTime)
{

	m_hApplicantTime->SetData(strDateTime.Mid(strDateTime.Find(_T(" "))));
}

INT CSpecies_Form::SetUDFCheck(CESL_DataMgr *pDM, CString strAlias, CString strType)
{
	if(pDM == NULL || strAlias.IsEmpty()) return -1;
	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT col, nCols;
	nCols = pDM->m_nCols;
	if(nCols < 0) nCols = pDM->GetRefColumnCount();
	for (col = 0; col < nCols; col++) {
		if (pDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		if(FIELD_ALIAS == strAlias)
		{
			pDM->SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, strType);
		}
	}
	return 0;
}

/*

INT CSpecies_Form::SetUDFCheck(CESL_DataMgr *pDM, CString strAlias, CString strType)
{
	if(pDM == NULL || strAlias.IsEmpty()) return -1;
	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT col, nCols;
	nCols = pDM->m_nCols;
	if(nCols < 0) nCols = pDM->GetRefColumnCount();
	for (col = 0; col < nCols; col++) {
		if (pDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		if(FIELD_ALIAS == strAlias)
		{
			pDM->SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, strType);
		}
	}
	return 0;
}	
*/
//==========================================================
//20080612 ADD PJW : ���Խ�û�ڰ� ������ NULL ������ �ú��� ����
void CSpecies_Form::OnKillfocusedtBSFORMREQUESTOR() 
{
	
	CString	strApplicantName;
	CString	strTime;
	CString	strApplicantDate;

	m_pCM_Index->GetControlMgrData(_T("���Խ�û��"), strApplicantName, 0);
	
	if ( strApplicantName == _T(""))
	{
		strTime.Format(_T("  :  :  "));
		strApplicantTime(strTime);
		m_pCM_Index->SetControlMgrData(_T("���Խ�û��"), _T(""), 0);		
		return;
	}

	CTime t = CTime::GetCurrentTime();
	m_pCM_Index->GetControlMgrData(_T("���Խ�û��"), strApplicantDate, 0);
	if(  strApplicantDate.IsEmpty() )
	{
		strApplicantDate.Format( _T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );
		strTime.Format(_T("%02d:%02d:%02d"), t.GetHour(), t.GetMinute(), t.GetSecond());
		m_pCM_Index->SetControlMgrData(_T("���Խ�û��"), strApplicantDate, 0);		
		strApplicantTime(strTime);
	}	
}
//==========================================================
//=======================================================================
//2008.07.17 ADD BY PJW : �η� �ڵ� ��Ϲ�ȣ �ο�
INT CSpecies_Form::ApplyAppendixRegNo(CESL_DataMgr *pDM_Appendix)
{
	EFS_BEGIN

	if (pDM_Appendix == NULL) return -1;

	// ��� ��ȣ �ڵ��ο����ΰ� Y �̸� ��Ϲ�ȣ�� �ο��Ѵ�.
	CString strQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '��Ÿ' AND GROUP_3 = '����' AND CLASS_ALIAS = '�ηϵ�Ϲ�ȣ�ڵ��ο�����'");
//DEL 	CString strQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2 = '��Ÿ' AND GROUP_3 = '����' AND CLASS_ALIAS = '�ηϵ�Ϲ�ȣ�ڵ��ο�����'");
	CString strValue;
	pDM_Appendix->GetOneValueQuery(strQuery, strValue);
	if (strValue != _T("Y")) return 0;

	// ��ϱ����� �ְ� ��Ϲ�ȣ�� ������ ��Ϲ�ȣ�� �ο��Ѵ�.

	INT nRowCount = pDM_Appendix->GetRowCount();
	if (nRowCount <= 0) return 0;

	INT ids = -1;
	CString strRecKey;
	CString strResult;
	CString strRegCode;
	CString strRegNo;
	CString strAccessionKey;
	CString strQueryFlag;
	CString strDbFalg;
	INT nNumber = 0;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// ��Ϲ�ȣ�� ������ �Ѿ��.
		ids = pDM_Appendix->GetCellData( _T("BB_��Ϲ�ȣ"), idx, strRegNo);
		if (ids >= 0 && !strRegNo.IsEmpty()) continue;

		// ��ϱ��� �ڵ带 �����´�.
		ids = pDM_Appendix->GetCellData( _T("BB_��ϱ���_�ڵ�"), idx, strRegCode);
		if (ids < 0 || strRegCode.IsEmpty()) continue;

		ids = pDM_Appendix->GetCellData( _T("BB_DB_FLAG"), idx, strDbFalg);
		if( strDbFalg != _T("U") && strDbFalg != _T("I") )
		{
			pDM_Appendix->SetCellData( _T("BB_DB_FLAG"), _T("U"), idx);
		}

		// ������ ��Ϲ�ȣ�� �����´�.
		strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
						_T("WHERE KIND_CODE = 'REG_NO' AND ")
						_T("DIVISION_VALUE='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strRegCode);
//DEL 						_T("DIVISION_VALUE='%s'"), strRegCode);
		ids = pDM_Appendix->GetOneValueQuery(strQuery, strResult);

		if (ids < 0 || strResult.IsEmpty())
		{
			strResult = _T("0");

			INT ids = pDM_Appendix->StartFrame();
			if ( ids < 0 ) 
			{
				AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
				return -1;
			}

			ids = pDM_Appendix->MakeRecKey(strRecKey);
			if (ids < 0) return ids;
			// ������ ��Ϲ�ȣ�� �����Ѵ�.
			strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ")
							_T("(REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER, MANAGE_CODE) ")
							_T("VALUES (%s, '%s', '%s', '%s', '%s', %s, UDF_MANAGE_CODE);")
//DEL 							_T("(REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER) ")
//DEL 							_T("VALUES (%s, '%s', '%s', '%s', '%s', %s);")
							, strRecKey, _T("CO"), _T("REG_NO"), strRegCode, CTime::GetCurrentTime().Format(_T("%Y")), strResult);
			ids = pDM_Appendix->AddFrame(strQuery);
			if (ids < 0) return ids;
			ids = pDM_Appendix->SendFrame();
			if (ids < 0) return ids;
			ids = pDM_Appendix->EndFrame();
			if (ids < 0) return ids;
		}
		// ������ ��Ϲ�ȣ UPDATE �Ѵ�.
		nNumber = _ttoi(strResult) + 1;
		
		strRegNo.Format(_T("%s%010d"), strRegCode, nNumber);

		ids = pDM_Appendix->SetCellData( _T("BB_��Ϲ�ȣ"), strRegNo, idx);
		//������ڸ� ��� �Ѵ�.
		CString strRegNoInputDate;
		CTime t = CTime::GetCurrentTime();
		ids = pDM_Appendix->GetCellData( _T("BB_�����"), idx, strRegNoInputDate);
		if(  strRegNoInputDate.IsEmpty() )
		{
			strRegNoInputDate.Format( _T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );
			ids = pDM_Appendix->SetCellData( _T("BB_�����"), strRegNoInputDate, idx);
		}	

		INT ids = pDM_Appendix->StartFrame();

		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
			return -1;
		}
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET ")
						_T("LAST_NUMBER = %d WHERE MANAGE_CODE = UDF_MANAGE_CODE AND KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '%s';"),
//DEL 						_T("LAST_NUMBER = %d WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '%s';"),
						nNumber, strRegCode);
		ids = pDM_Appendix->AddFrame(strQuery);
		if (ids < 0) return ids;
		ids = pDM_Appendix->SendFrame();
		if (ids < 0) return ids;
		ids = pDM_Appendix->EndFrame();
		if (ids < 0) return ids;
	}

	return 0;

	EFS_END
	return -1;
}
//=======================================================================
//===================================================
//2009.05.28 ADD BY LKS : Control Trim

VOID CSpecies_Form::TrimControl()
{
	INT ControlID[18]={IDC_eTITLE,IDC_edtAPPENDIX,IDC_eDISCRIPTION_SMALL,
		IDC_eAUTHOR1,IDC_eVOL,IDC_eSERIES_TITLE,IDC_edtSERIES_VOL,
		IDC_eEDITION_STMT,IDC_ePUBLISH_PLACE,IDC_ePUBLISHER,
		IDC_ePUBLISH_YEAR,IDC_eEA_ISBN,IDC_eEA_ISBN_ADD,IDC_eEA_ISBN_MISTAKE,
		IDC_eSET_ISBN,IDC_eSET_ISBN_ADD,IDC_eSET_ISBN_MISTAKE,
		IDC_edtPHYSICAL_CHARACTER};

	CString strData;
	CEdit* pEdit = NULL;
	for(INT i=0;i<18;i++)
	{
		pEdit = (CEdit*)GetDlgItem(ControlID[i]);
		if (pEdit == NULL) continue;
		pEdit->GetWindowText(strData);
		if( !strData.IsEmpty() )
		{
			strData.TrimLeft();		strData.TrimRight();
			pEdit->SetWindowText(strData);	
		}
	}
}
//===================================================