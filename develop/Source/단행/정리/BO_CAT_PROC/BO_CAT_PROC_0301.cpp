// BO_CAT_PROC_0301.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0301.h"
#include "BO_CAT_PROC_0301_ROLE.h"
#include "BO_CAT_PROC_0330.h"
#include "BO_CAT_PROC_0380.h"
#include "..\..\..\����\IndexToolKit\IndexTool.h"
#include "..\..\..\����\������\DonatorNameGetter\DonatorNameGetterDlg.h"
#include "..\BO_CAT_PROC_KEYWORD\BO_CAT_PROC_KEYWORD_0000.h"
#include "..\BO_CAT_NUMBER\BO_CAT_ACQUISIT_NO.h"

#include "..\..\..\����\�谡���ذ���\MNG_CODE_SHELF\MNG_CODE_SHELF_01.h"
#include "..\..\..\����\�谡���ذ���\ShelfCodeGetter\ShelfCodeGetterDlg.h"
#include "..\..\..\����\Duplicate_Data_Search\DuplicateSearch_Interface.h"

#include "..\..\..\����\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"

#include "..\..\..\����\BO_MARC_IMPORT\MARC_IMPORT.h"

#include "..\..\..\�������\Light_GatherDataSearch\UC_MARC_IMPORT.h"

#include "..\..\..\ESL\Marc\MarcToolKit\MarcDefaultValueConfigDlg.h"

#include "..\..\..\����\����\CatCodeGetter\CatCodeGetterDlg.h"

#include "BO_CAT_PROC_Api.h"

#include <direct.h>

#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0320_01.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0321.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0322.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0323.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0324.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0325.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\..\..\����\BO_VOLUME_FORM\BOVolumeDlg.h"

#include "..\..\..\����\�з���ȣ\MNG_CLASS_SIGN_POPUP\MNG_CLASS_SIGN_POPUP_01.h"

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0301 dialog


CBO_CAT_PROC_0301::CBO_CAT_PROC_0301(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0301)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();

	m_pMarc = NULL;
	
	m_pMarcDefaultValueConfigDlg = new CMarcDefaultValueConfigDlg(this);
}

CBO_CAT_PROC_0301::~CBO_CAT_PROC_0301()
{
	if (m_pMarcDefaultValueConfigDlg)
		delete m_pMarcDefaultValueConfigDlg;
}


VOID CBO_CAT_PROC_0301::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0301)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0301, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0301)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bBOOK_INPUT, OnbBOOKINPUT)
	ON_BN_CLICKED(IDC_bSTATIC, OnbSTATIC)
	ON_BN_CLICKED(IDC_bBUCHUL_ASSIST, OnbBUCHULASSIST)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_bSTATIC3, OnbSTATIC3)
	ON_BN_CLICKED(IDC_bSUBJECT_REF3, OnbSUBJECTREF3)
	ON_BN_CLICKED(IDC_bCOL_NUMBER, OnbCOLNUMBER)
	ON_BN_CLICKED(IDC_bCOL_NUMBER2, OnbCOLNUMBER2)
	ON_BN_CLICKED(IDC_bBOKBON_SEARCH, OnbBOKBONSEARCH)
	ON_BN_CLICKED(IDC_bKEYWORD_CREAT, OnbKEYWORDCREAT)
	ON_BN_CLICKED(IDC_BTN_PRESENT, OnBtnPresent)
	ON_BN_CLICKED(IDC_bSELECT2, OnbSELECT2)
	ON_BN_CLICKED(IDC_bREFERENCE, OnbREFERENCE)
	ON_CBN_SELCHANGE(IDC_COB_AcqCode, OnSelchangeCOBAcqCode)
	ON_BN_CLICKED(IDC_bCOMMON_APPLY3, OnbCOMMONAPPLY3)
	ON_BN_CLICKED(IDC_bSUBJECT_REF2, OnbSUBJECTREF2)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabForm, OnSelchangetabForm)
	ON_BN_CLICKED(IDC_bINQUIRY, OnbINQUIRY)
	ON_BN_CLICKED(IDC_bCOMMON_APPLY2, OnbCOMMONAPPLY2)
	ON_BN_CLICKED(IDC_bMarcImport, OnbMarcImport)
	ON_BN_CLICKED(IDC_bACCESSION_OBJ_IMPORT, OnbACCESSIONOBJIMPORT)
	ON_BN_CLICKED(IDC_bConfig, OnbConfig)
	ON_CBN_KILLFOCUS(IDC_COB_AcqCode, OnKillfocusCOBAcqCode)
	ON_EN_KILLFOCUS(IDC_eREG_CLASS, OnKillfocuseREGCLASS)
	ON_CBN_SELCHANGE(IDC_c0301FORM_CLASS, OnSelchangec0301FORMCLASS)
	ON_BN_CLICKED(IDC_b0301PreMarc, Onb0301PreMarc)
	ON_BN_CLICKED(IDC_bCOMMON_APPLY, OnbCOMMONAPPLY)
	ON_NOTIFY(EN_KILLFOCUS, IDC_redt0301MarcView, OnKillfocusredt0301MarcView)
	ON_CBN_SELCHANGE(IDC_c0301MoneyCode, OnSelchangec0301MoneyCode)
	ON_CBN_SETFOCUS(IDC_c0301FORM_CLASS, OnSetfocusc0301FORMCLASS)
	ON_CBN_SELENDOK(IDC_c0301FORM_CLASS, OnSelendokc0301FORMCLASS)
	ON_CBN_SELENDCANCEL(IDC_c0301FORM_CLASS, OnSelendcancelc0301FORMCLASS)
	ON_CBN_DROPDOWN(IDC_c0301FORM_CLASS, OnDropdownc0301FORMCLASS)
	ON_NOTIFY(EN_SETFOCUS, IDC_redt0301MarcView, OnSetfocusredt0301MarcView)
	ON_BN_CLICKED(IDC_bCatInpHotKeyList, OnbCatInpHotKeyList)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 32015, OnCreate505Tag)
	ON_MESSAGE(WM_GRID_UPDATE, OnMarcGridUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0301 message handlers

BOOL CBO_CAT_PROC_0301::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0301::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0301")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CTabCtrl	*m_tabPurchasing = (CTabCtrl*)GetDlgItem(IDC_tabForm);

	m_tabPurchasing->InsertItem(0, _T("�����Է�/��ũ��"));
	m_tabPurchasing->InsertItem(1, _T("�����Է�"));	
	m_tabPurchasing->InsertItem(2, _T("��ũ��"));	

	CButton * pPresent;
	pPresent = (CButton *)GetDlgItem(IDC_BTN_PRESENT);		
	CButton * pPresentSt;
	pPresentSt = (CButton *)GetDlgItem(IDC_STATIC_PRESENT);	
	CButton * pPresentEd;
	pPresentEd = (CButton *)GetDlgItem(IDC_EDT_PRESENT);	

	pPresent->ShowWindow(SW_HIDE);	
	pPresentSt->ShowWindow(SW_HIDE);	
	pPresentEd->ShowWindow(SW_HIDE);
	
	m_marcEditor.SubclassDlgItem(IDC_redt0301MarcView, IDC_MSHFLEXGRID1, this);

	CMarcGridCtrl *pGrid = m_marcEditor.GetGridCtrl();
	if (pGrid)
	{
		pGrid->SetGridColWidth( -1, -1, 200 );
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	pDM->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

	m_marcEditor.InitUserAlias();

	CMapStringToPtr MapUserAlias;
	m_pMarcDefaultValueConfigDlg->LoadDefaultUserMarcAlias(&MapUserAlias);

	CStringArray *pStringArray = NULL;
	INT ids = m_pMarcDefaultValueConfigDlg->GetUserMarcAlias(&MapUserAlias, m_pInfo->pMarcMgr->GetMarcGroup(), &pStringArray);

	if (ids >= 0)
	{
		INT nCount = pStringArray->GetSize();
		for (INT idx = 0; idx < nCount; idx++)
		{
			m_marcEditor.AddUserAlias(pStringArray->GetAt(idx));
		}
	}

	m_marcEditor.InitLimitTag();
	m_marcEditor.AddLimitTag(_T("008"));
	m_marcEditor.AddLimitTag(_T("020"));
	m_marcEditor.AddLimitTag(_T("007"));
	
	CMapStringToPtr MapUserMarc;

	m_pMarcDefaultValueConfigDlg->LoadDefaultMarc();
	
	ids = m_pMarcDefaultValueConfigDlg->GetUserMarc( m_pInfo->pMarcMgr->GetMarcGroup(), &m_pMarc );
	if (ids >= 0)
		m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);

	m_pInfo->pMarcMgr->CheckSimpleSyntax( m_pMarc );

	m_marcEditor.Display();

	TCHAR szLeader[25];
	m_pMarc->GetLeader(szLeader);

	pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_redt0301MarcView);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0301::OnbBOOKINPUT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	m_marcEditor.ApplyEdit();

	CString sRegCode;
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BB_��ϱ���_�ڵ�"), sRegCode);

	if ( sRegCode.IsEmpty() ) {
		AfxMessageBox( _T("��ϱ����� �Է��ϼ���.") );
		return;
	}

	m_iBookInputsW = 1;		//�Ǻ����� ����.

	CString sAuthor, sPubYear;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("����"), sAuthor);
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("�����"), sPubYear);

	CString sPage, sPhysicalProperty, sSize;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("�����"), sPage);
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("������Ư��"), sPhysicalProperty);
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("ũ��"), sSize);	


	CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0301_å"));
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0301_å error") );
		return;
	}
	INT iRowCount = pDM->GetRowCount();	
	INT ids;

	CString sCode;
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_��ü����_�ڵ�"), sCode);
	ids = pDM->SetCellData( _T("BB_��ü����_�ڵ�"), sCode, 0 );

	sCode = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_�̿����ѱ���_�ڵ�"), sCode);
	ids = pDM->SetCellData( _T("BB_�̿����ѱ���_�ڵ�"), sCode, 0 );

	sCode = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_��������_�ڵ�"), sCode);
	ids = pDM->SetCellData( _T("BB_��������_�ڵ�"), sCode, 0 );



	CString sItemData = _T("");

	sItemData = _T("");
	ids = m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("245$p"), sItemData );
	if ( sItemData != _T("") ) {
		ids = pDM->SetCellData( _T("BB_������ηϸ�"), sItemData, 0 );
	}
	sItemData = _T("");
	ids = m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("300$e"), sItemData );
	if ( sItemData != _T("") ) {
		ids = pDM->SetCellData( _T("BB_�����ڷ�"), sItemData, 0 );
	}
	
	sItemData = _T("");
	ids = m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("950$c"), sItemData );
	if ( sItemData != _T("") ) {
		ids = pDM->SetCellData( _T("BB_����������"), sItemData, 0 );
	}


	if ( iRowCount < 1 ) {
		ids = pDM->InsertRow(-1);
		pDM->SetCellData(_T("BB_å�η��÷���"), _T("B"), 0);
		pDM->SetCellData(_T("BB_DB_FLAG"), _T("I"), 0);

		sItemData = _T("");
		ids = m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("950$b"), sItemData );
		if ( sItemData != _T("") ) {
			ids = pDM->SetCellData( _T("BB_����"), sItemData, 0 );
		}		

		ids = pDM->SetCellData( _T("BB_������"), sAuthor, 0 );
		ids = pDM->SetCellData( _T("BB_�����"), sPubYear, 0 );
		ids = pDM->SetCellData( _T("BB_�����"), sPage, 0 );
		ids = pDM->SetCellData( _T("BB_������Ư��"), sPhysicalProperty, 0 );
		ids = pDM->SetCellData( _T("BB_ũ��"), sSize, 0 );

		CString sCode;
		GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_��ü����_�ڵ�"), sCode);
		ids = pDM->SetCellData( _T("BB_��ü����_�ڵ�"), sCode, 0 );

		sCode = _T("");
		GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_�̿����ѱ���_�ڵ�"), sCode);
		ids = pDM->SetCellData( _T("BB_�̿����ѱ���_�ڵ�"), sCode, 0 );

		sCode = _T("");
		GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_��������_�ڵ�"), sCode);
		ids = pDM->SetCellData( _T("BB_��������_�ڵ�"), sCode, 0 );
		
		CBO_CAT_PROC_Api FormApi(this);

		CString strRegCodeField;
		GetDlgItem(IDC_eREG_CLASS)->GetWindowText(strRegCodeField);
		
		CArray<CRegCodeProc, CRegCodeProc> arrRegCode;

		INT ids = FormApi.FetchRegCode(strRegCodeField, &arrRegCode);
		if (ids < 0 || arrRegCode.GetSize() <= 0)
		{

		}
		m_strRegCodeField = strRegCodeField;
		CString strVol = _T("");
		FormApi.MakeBookFromRegCode(&arrRegCode, pDM, strVol);	//pDM -> å
		INT iBookCount = pDM->GetRowCount();
		pDM->SetCellData( _T("BB_��ǥå����"), _T("Y"), 0 );
	}

	CBO_CAT_PROC_Api FormApi(this);

	CString strRegCodeField;
	GetDlgItem(IDC_eREG_CLASS)->GetWindowText(strRegCodeField);

	CArray<CRegCodeProc, CRegCodeProc> arrRegCode;
	ids = FormApi.FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0 || arrRegCode.GetSize() <= 0)
	{
		return;
	}

	m_strRegCodeField = strRegCodeField;
	
	CBOVolumeDlg dlg(this, pDM);
	dlg.SetOpenMode(2100);
	dlg.DoModal();

	arrRegCode.RemoveAll();
	ids = FormApi.GetRegCodeInfo( pDM, &arrRegCode );

	if (ids < 0 || arrRegCode.GetSize() <= 0)
	{
		GetDlgItem(IDC_eREG_CLASS)->SetWindowText(m_strRegCodeField);
		
		AfxMessageBox( _T("��ϱ��� �ʵ尡 �߸��Ǿ����ϴ�.") );
		return;
	}

	strRegCodeField = _T("");
	for ( INT i = 0 ; i < arrRegCode.GetSize() ; i++ ) {
		CRegCodeProc RegCode = arrRegCode.GetAt(i);
		CString sRegCode = RegCode.m_strRegCode;
		INT iRegNo = RegCode.m_nCount;
		CString sSubRegNo;
		sSubRegNo.Format( _T("%s%d"), sRegCode, iRegNo );
		
		if ( strRegCodeField != _T("") ) {
			strRegCodeField = strRegCodeField + _T("/") + sSubRegNo;
		}
		else {
			strRegCodeField = sSubRegNo;
		}
		
	}
	m_strRegCodeField = strRegCodeField;
	SetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BB_��ϱ���_�ڵ�"), m_strRegCodeField );


	m_marcEditor.ApplyEdit();

	CESL_DataMgr *pDmVol;
	pDmVol = FindDM( _T("DM_BO_CAT_PROC_0301_��") );
	if ( pDmVol == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0301_�� error") );
		return;
	}
	ids = pDmVol->FreeData();

	CBO_SPECIES API(this);
	API.CONVERT_BOOK_to_VOLUME( pDM, pDmVol );

	INT iVolCount;
	iVolCount = pDmVol->GetRowCount();
	
	if ( iVolCount > 1 ) {
		ids = m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("020") );

		ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("245$n") );
		
		ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("245$p") );
		CString sVolNo;
		CString sPriceSum;
		INT iPriceSum = 0;
		for ( INT i = 0 ; i < iVolCount ; i++ ) {
			CString sItemData = _T("");
			
			ids = pDmVol->GetCellData( _T("BB_�����"), i, sItemData );
			if ( sItemData != _T("") ) {
				if ( sVolNo == _T("") ) {
					sVolNo = sItemData;
				}
				else {
					sVolNo = sVolNo + _T(",") + sItemData;
				}
			}
			ids = pDM->GetCellData( _T("BB_����"), i, sItemData );
			CString sPrice = sItemData;	
			if ( sItemData != _T("") ) {
				INT iPrice = _ttoi ( sItemData.GetBuffer(0) );
				iPriceSum = iPriceSum + iPrice;
			}

			ids = pDM->GetCellData( _T("BB_����ISBN"), i, sItemData );
			if ( sItemData != _T("") ) {
				if ( sPrice != _T("") ) {
					CString sMoneyCode;
					GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_ȭ�󱸺�"), sMoneyCode );
					sPrice = sMoneyCode + sPrice;
				}
				CString strTmp=_T("");
				strTmp.Format(_T("020  %ca%s%cc%s"), SUBFIELD_CODE, sItemData, sPrice);
				sItemData = strTmp;
				ids = m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}
		}
		sPriceSum.Format( _T("%d"), iPriceSum );
		if ( sPriceSum != _T("") && sPriceSum != _T("0") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("950$b") );

			CString sMoneyCode;
			GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_ȭ�󱸺�"), sMoneyCode );
			sPriceSum = sMoneyCode + sPriceSum;			

			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("950$b"), sPriceSum );
		}
		ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("245$n"), sVolNo );

	}

	if ( iVolCount == 1 ) {
		ids = m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("020") );

		CString sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_�����"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("245$n"), sItemData );
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("245$n"), sItemData );
		}
		
		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_������ηϸ�"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("245$p"), sItemData );
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("245$p"), sItemData );
		}		

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_������"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("245$d"), sItemData );
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("245$d"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_�����"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("260$c"), sItemData );
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("260$c"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_����ISBN"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("020$a"), sItemData );
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("020$a"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_�����"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("�����"), sItemData );
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("�����"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_������Ư��"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("������Ư��"), sItemData );
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("������Ư��"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_ũ��"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("ũ��"), sItemData );
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("ũ��"), sItemData );
		}

		
		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_�����ڷ�"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("300$e"), sItemData );
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("300$e"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_����"), 0, sItemData );
		ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("950$b") );
		
		if ( sItemData != _T("") && sItemData != _T("0") ) {
			CString sMoneyCode;
			GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_ȭ�󱸺�"), sMoneyCode );
			sItemData = sMoneyCode + sItemData;
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("950$b"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_����������"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("950$c"), sItemData );
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("950$c"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_��������_�ڵ�"), 0, sItemData );
			SetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_��������_�ڵ�"), sItemData );
		
		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_��ü����_�ڵ�"), 0, sItemData );
			SetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_��ü����_�ڵ�"), sItemData );

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_�̿����ѱ���_�ڵ�"), 0, sItemData );
			SetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_�̿����ѱ���_�ڵ�"), sItemData );
	}

	ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("049$v") );
	ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("049$c") );
	ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("049$f") );
	CString sRegNo;
	CString sVolNoPre;
	INT iCopyCode = 1;
	CString sStartRegNo, sEndRegNo;
	CString sPreShelfCode;
	INT iSwShelfCode = 0;
	for ( i = 0 ; i < pDM->GetRowCount() ; i ++ ) {	
		sRegNo = _T("");
		ids = pDM->GetCellData( _T("BB_��Ϲ�ȣ"), i, sRegNo );		
		if ( sRegNo != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("049$l"), sRegNo );
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("049$l"), sRegNo );
		}

		if ( sRegNo != _T("") ) {
			if ( 0 == i ) {
				sStartRegNo = sRegNo;
				sEndRegNo = sRegNo;
			}
			else {
				if ( sStartRegNo > sRegNo ) {
					sStartRegNo = sRegNo;
				}
				if ( sRegNo > sEndRegNo ) {
					sEndRegNo = sRegNo;
				}
			}
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_�����"), i, sItemData );
		if ( sItemData != _T("") && sRegNo != _T("") ) {
			ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("049$v"), sItemData );
			if ( sVolNoPre == sItemData ) {
				iCopyCode++;
				CString sCopyCode;
				sCopyCode.Format( _T("%d"), iCopyCode );
				
				ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("049$c"), sCopyCode );
				ids = pDM->SetCellData( _T("BB_������ȣ"), sCopyCode, i );
				
			}
			else{
				iCopyCode = 1;
			}
		}
		sVolNoPre = sItemData;
		
		if ( sRegNo != _T("") ) {
			sRegNo = _T("");
			ids = pDM->GetCellData( _T("BB_��ġ��ȣ_�ڵ�"), i, sRegNo );
			if ( sRegNo != _T("") ) {
				ids = m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("049$f"), sRegNo );
				ids = m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("049$f"), sRegNo );
			}
			if ( (i != 0) && (sPreShelfCode != sRegNo) && sPreShelfCode != _T("") ) {
				CString s049field;
				m_pInfo->pMarcMgr->GetField( m_pMarc, _T("049"), s049field );
				s049field = _T("1") + s049field.Mid(1);
				m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("049") );
				m_pInfo->pMarcMgr->InsertField( m_pMarc, _T("049") + s049field );
				iSwShelfCode = 1;
			}
			sPreShelfCode = sRegNo;
		}		
	}
	m_sStartRegNo = sStartRegNo;
	m_sEndRegNo = sEndRegNo;
	
	if ( iSwShelfCode == 0 ) {
		CString s049field;
		m_pInfo->pMarcMgr->GetField( m_pMarc, _T("049"), s049field );
		if ( s049field != _T("") ) {
			s049field = _T("0") + s049field.Mid(1);
			m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("049") );
			m_pInfo->pMarcMgr->InsertField( m_pMarc, _T("049") + s049field );
		}
	}
	m_marcEditor.Display();

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbSTATIC() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sFormCode;
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_���ı���_�ڵ�"), sFormCode);

	m_marcEditor.ApplyEdit();
	if ( sFormCode == _T("BK") ) {
		CBO_CAT_PROC_0320_01 dlg(this, _T("DM_BO_CAT_PROC_0000"), *m_pMarc);

		if( IDOK == dlg.DoModal() ) {
			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);
			
			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, m_pMarc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( m_pMarc );

			m_marcEditor.InitLimitTag();

			m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_pMarc->GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0301"), _T("����"), sLeader);
		}
	}
	else if ( sFormCode == _T("CF") ) {
		CBO_CAT_PROC_0321 dlg(this, _T("DM_BO_CAT_PROC_0000"), *m_pMarc);

		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, m_pMarc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( m_pMarc );

			m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_pMarc->GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0301"), _T("����"), sLeader);
		}
	}
	else if ( sFormCode == _T("MP") ) {
		CBO_CAT_PROC_0322 dlg(this, _T("DM_BO_CAT_PROC_0000"), *m_pMarc);

		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			//decoding
			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, m_pMarc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( m_pMarc );

			m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_pMarc->GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0301"), _T("����"), sLeader);
		}
	}	
	else if ( sFormCode == _T("MU") ) {
		CBO_CAT_PROC_0323 dlg(this, _T("DM_BO_CAT_PROC_0000"), *m_pMarc);
		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, m_pMarc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( m_pMarc );

			m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_pMarc->GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0301"), _T("����"), sLeader);
		}
	}
	else if ( sFormCode == _T("VM") ) {
		CBO_CAT_PROC_0324 dlg(this, _T("DM_BO_CAT_PROC_0000"), *m_pMarc);
		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, m_pMarc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( m_pMarc );

			m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_pMarc->GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0301"), _T("����"), sLeader);
		}
	}
	else {
		CBO_CAT_PROC_0320_01 dlg(this, _T("DM_BO_CAT_PROC_0000"), *m_pMarc);
		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, m_pMarc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( m_pMarc );

			m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_pMarc->GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0301"), _T("����"), sLeader);
		}
	}
	

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbBUCHULASSIST() 
{
EFS_BEGIN

	m_marcEditor.ApplyEdit();
	m_pInfo->pMarcMgr->CheckDeepSyntax(m_pMarc);
	m_marcEditor.Display();

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbtnCLOSE() 
{
EFS_BEGIN

EFS_END
}

VOID CBO_CAT_PROC_0301::OnOK() 
{
EFS_BEGIN

	CString sRegCode;
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BB_��ϱ���_�ڵ�"), sRegCode);
	if ( sRegCode.IsEmpty() ) {
		AfxMessageBox( _T("��ϱ����� �Է��ϼ���.") );
		return;
	}

	INT iCh = m_marcEditor.ApplyEdit();
	if ( iCh < 0 ) {
		return;
	}

	CString sTitle;
	m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("245$a"), sTitle );
	if ( sTitle == _T("") ) {
		AfxMessageBox( _T("������ �Է��ϼ���.") );
		return;
	}

	CESL_DataMgr *pDM_PROC_0000;
	pDM_PROC_0000 = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( pDM_PROC_0000 == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}
	INT iRowIdx = pDM_PROC_0000->GetRowCount();
	pDM_PROC_0000->InsertRow(-1);

	m_pInfo->pMarcMgr->CheckDeepSyntax(m_pMarc);

	CTime time = CTime::GetCurrentTime();
	CString sTime;
	sTime.Format( _T("%04d%02d%02d%02d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
	sTime = _T("005") + sTime;
	m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("005") );	
	m_pInfo->pMarcMgr->InsertField( m_pMarc, sTime );

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0301"));
	if (!pDM) return;

	CString sMngCode;
	GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_��������_�ڵ�"), sMngCode );
	CString str;
	CString sQuery = _T("SELECT VALUE FROM MN_LIBINFO_TBL WHERE VALUE_NAME ='��������ȣ'");
	pDM->GetOneValueQuery( sQuery , str );
	if ( str != _T("") ) {
		CString s040a, s040c;
		m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("040$a"), s040a );
		m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("040$c"), s040c );
		if ( s040a == _T("") ) {
			m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("040$a"), str );
			m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("040$c"), str );
		}
		else if ( s040a != str || s040c != str) {
			m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("040$d"), str );
		}
	}
	else {
		AfxMessageBox( _T("������ ��ȣ�� �����ϴ�.") );
	}

	CBO_CAT_PROC_KEYWORD_0000 dlg(this, *m_pMarc);	
	if( IDOK == dlg.DoModal() ) {
		CString strStreamMarc = m_strStreamMarc;
		m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);
		
		CString strTagCode, strErrMsg, strHelpMsg;		
		if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, m_pMarc))
		{
			POSITION pos;
			pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
			while (pos)
			{
				m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
				UpdateData(FALSE);
			}	
		}	
		m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
		m_marcEditor.Display();

		TCHAR szLeader[25];
		m_pMarc->GetLeader(szLeader);

		CString sLeader;
		sLeader.Format( _T("%s") , szLeader );
		SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);
	}
	
	if ( iSwAcqNumber == 1 ) {

		CString sWhere;
		CString sCssCode;
		GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("�з�ǥ����"), sCssCode );
		if ( sCssCode != _T("") ) {
			sWhere = _T("CLASS_NO_TYPE = '") + sCssCode + _T("'");
		}
		CString sCss;
		m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("090$a"), sCss );
		if ( sCss != _T("") ) {
			if ( sWhere != _T("") ) {
				sWhere = sWhere + _T(" AND ");
			}
			sWhere = sWhere + _T("CLASS_NO = '") + sCss + _T("'");
		}
		CString sShelfCode;
		m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("049$f"), sShelfCode );
		if ( sShelfCode != _T("") ) {
			if ( sWhere != _T("") ) {
				sWhere = sWhere + _T(" AND ");
			}
			sWhere = sWhere + _T("SEPARATE_SHELF_CODE = '") + sShelfCode + _T("'");
		}

		if ( sShelfCode != _T("") ) {
			if ( sWhere != _T("") ) {
				sWhere = sWhere + _T(" AND ");
			}
			sWhere = sWhere + _T("MANAGE_CODE = UDF_MANAGE_CODE");
		}
		
		CString sQueryAcqNumber = _T("SELECT ACQUISIT_NO FROM CO_OMIT_BOOK_ACQUISITNO_TBL WHERE ") + sWhere;
		CString sAcqNumber;
		pDM->GetOneValueQuery( sQueryAcqNumber , sAcqNumber );
		if ( sAcqNumber == _T("") ) {
			sAcqNumber = _T("0");
		}
		else {
			INT iAcqNumber = _ttoi ( sAcqNumber.GetBuffer(0) );
			iAcqNumber++;
			sAcqNumber.Format( _T("%d"), iAcqNumber );
		}
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("090$b"), sAcqNumber );
	}

	CString s008;
	m_pInfo->pMarcMgr->GetField( m_pMarc , _T("008") , s008 );
	if ( s008 == _T("") ) {

		CString sDate;

		GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("���糯¥"), sDate );
		if ( sDate.IsEmpty() ) {
			AfxMessageBox( _T("���糯¥�� �����ϴ�.") );
			return;
		}

		CString sInputDate;
		sInputDate = sDate.Mid( 2, 2 ) + sDate.Mid( 5, 2 ) + sDate.Mid( 8, 2 );
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("008@0-5"), sInputDate );
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("008@6"), _T("s") );
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("008@7-10"), sDate.Mid( 0, 4 ) );		
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@38-39"), _T("  ") );
		
		CString s260Year;
		m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("260$c"), s260Year );
		if ( s260Year != _T("") ) {	
			m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("008@7-10"), s260Year );
		}

		CString sLeader;
		m_pMarc->GetLeader( sLeader );
		CString sTmp;
		sTmp = sLeader.Left(5) + _T("nam") + sLeader.Right(16);
		m_pMarc->SetLeader( sTmp.GetBuffer(0) );
		
		SetControlData(_T("CM_BO_CAT_PROC_0301"), _T("����"), sTmp);
		
		m_marcEditor.Display();
	}
	CString sItem;
	
	sItem =_T("");

	CString sPrice;
	CString sOriPrice;
	CString sPriceCode;
	m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("950$b"), sPrice );
	if ( sPrice != _T("") ) {
		for ( INT i = 0 ; i < sPrice.GetLength() ; i++ ) {
			CString sOneWord;
			sOneWord = sPrice.Mid( i, 1 );
			if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
				break;
			}
			else {
				sPriceCode = sPriceCode + sOneWord;
			}
		}
		
		if ( sPriceCode == _T("") ) {
			AfxMessageBox( _T("ȭ�󱸺��� �Է��ϼ���") );
			return;
		}
	}
	
	CString sMessage;
	
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( m_pMarc , _T("008") , sItem );
	if ( sItem == _T("") ) {
		sMessage = _T("008, ");
	}
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( m_pMarc , _T("090") , sItem );
	if ( sItem == _T("") ) {
		sMessage = sMessage + _T("090, ");
	}
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( m_pMarc , _T("049") , sItem );
	if ( sItem == _T("") ) {
		sMessage = sMessage + _T("049, ");
	}
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( m_pMarc , _T("056") , sItem );
	if ( sItem == _T("") ) {
		sMessage = sMessage + _T("056, ");
	}
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( m_pMarc , _T("245") , sItem );
	if ( sItem == _T("") ) {
		sMessage = sMessage + _T("245, ");
	}
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( m_pMarc , _T("260") , sItem );
	if ( sItem == _T("") ) {
		sMessage = sMessage + _T("260, ");
	}
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( m_pMarc , _T("300") , sItem );
	if ( sItem == _T("") ) {
		sMessage = sMessage + _T("300, ");
	}	

	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return;
	}

	CString sRecKey;	
	pDM->MakeRecKey( sRecKey );

	INT iStatus = 0;
	sMessage = sMessage.Left(sMessage.GetLength()-2);
	if ( sMessage != _T("") ) {
		sMessage = sMessage + _T(" tag�� �Է����� �ʾҽ��ϴ�.");
		AfxMessageBox( sMessage );
	}
	else {
		iStatus = 1;
	}

	CString MAT_CODE, FORM_CODE, MEDIA_CODE, SUB_REG_CODE, USE_OBJ_CODE, USE_LIMIT_CODE, MANAGE_CODE, CONTROL_MAT_CODE;	
	CString CLASS_NO_TYPE;

	CString ACQUISIT_CODE, ACQUISIT_YEAR;
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BB_���Ա���_�ڵ�"), ACQUISIT_CODE);
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_���Գ⵵"), ACQUISIT_YEAR);	

	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_�ڷᱸ��_�ڵ�"), MAT_CODE);
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_���ı���_�ڵ�"), FORM_CODE);
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_��ü����_�ڵ�"), MEDIA_CODE);	
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_������ϱ���_�ڵ�"), SUB_REG_CODE);
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_�̿��󱸺�_�ڵ�"), USE_OBJ_CODE);
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_�̿����ѱ���_�ڵ�"), USE_LIMIT_CODE);
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_��������_�ڵ�"), MANAGE_CODE);
	GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_�����ڷᱸ��"), CONTROL_MAT_CODE);
	iCh = GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("�з�ǥ����"), CLASS_NO_TYPE);

	MAT_CODE.Replace ( _T(" ") , NULL );
	FORM_CODE.Replace ( _T(" ") , NULL );
	MEDIA_CODE.Replace ( _T(" ") , NULL );
	SUB_REG_CODE.Replace ( _T(" ") , NULL );
	USE_OBJ_CODE.Replace ( _T(" ") , NULL );
	USE_LIMIT_CODE.Replace ( _T(" ") , NULL );
	MANAGE_CODE.Replace ( _T(" ") , NULL );
	CONTROL_MAT_CODE.Replace( _T(" ") , NULL );
	CLASS_NO_TYPE.Replace( _T(" ") , NULL );

	pDM_PROC_0000->SetCellData( _T("���۵�Ϲ�ȣ"), m_sStartRegNo, iRowIdx );
	pDM_PROC_0000->SetCellData( _T("����Ϲ�ȣ"), m_sEndRegNo, iRowIdx );

	pDM_PROC_0000->SetCellData( _T("BS_���Գ⵵"), ACQUISIT_YEAR, iRowIdx );
	pDM_PROC_0000->SetCellData( _T("IBS_�ڷᱸ��_�ڵ�"), MAT_CODE, iRowIdx );
	pDM_PROC_0000->SetCellData( _T("IBS_���ı���_�ڵ�"), FORM_CODE, iRowIdx );
	pDM_PROC_0000->SetCellData( _T("IBS_��ü����_�ڵ�"), MEDIA_CODE, iRowIdx );
	pDM_PROC_0000->SetCellData( _T("IBS_������ϱ���_�ڵ�"), SUB_REG_CODE, iRowIdx );
	pDM_PROC_0000->SetCellData( _T("IBS_�̿��󱸺�_�ڵ�"), USE_OBJ_CODE, iRowIdx );
	pDM_PROC_0000->SetCellData( _T("IBS_�̿����ѱ���_�ڵ�"), USE_LIMIT_CODE, iRowIdx );
	pDM_PROC_0000->SetCellData( _T("IBS_��������_�ڵ�"), MANAGE_CODE, iRowIdx );
	pDM_PROC_0000->SetCellData( _T("IBS_�����ڷᱸ��_�ڵ�"), CONTROL_MAT_CODE, iRowIdx );
	pDM_PROC_0000->SetCellData( _T("IBS_�з�ǥ����_�ڵ�"), CLASS_NO_TYPE, iRowIdx );

	
	CString LANG_CODE, PUBLISH_COUNTRY_CODE, KO_UNIVERSITY_CODE, GOVERN_INSTRUMENT_CODE;
	CString KDC_CLASS_NO;
	CString TITLE, AUTHOR, PUBLISHER;

	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("008@35-37"), LANG_CODE);
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("008@15-17"), PUBLISH_COUNTRY_CODE);
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("056$a"), KDC_CLASS_NO);

	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("����"), TITLE);
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("����"), AUTHOR);
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("������"), PUBLISHER);
	
	pDM_PROC_0000->SetCellData( _T("BS_MARC") , m_strStreamMarc, iRowIdx );
	
	CString sTitleTagX;
	m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("245$x"), sTitleTagX);
	if ( sTitleTagX != _T("") ) {
		sTitle = sTitle + _T("=") + sTitleTagX;
	}
	CString sTitleTagB;
	m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("245$b"), sTitleTagB);
	if ( sTitleTagB != _T("") ) {
		sTitle = sTitle + _T(":") + sTitleTagB;
	}

	pDM_PROC_0000->SetCellData( _T("IBS_��ǥ��"), sTitle, iRowIdx );

	CString sAuthor;
	m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("245$d"), sAuthor );
	CString sAuthorTagE;
	m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("245$e"), sAuthorTagE );
	if ( sAuthorTagE != _T("") ) {
		sAuthor = sAuthor + _T(";") + sAuthorTagE;
	}

	pDM_PROC_0000->SetCellData( _T("IBS_������"), sAuthor, iRowIdx );
	
	CString sPublisher;
	CString strItemData;
	CArray<CString, CString&> pArray;	
	m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("260$b"), strItemData, &pArray );
	for ( INT i = 0 ; i < pArray.GetSize() ; i++ ) {
		if ( i == 0  ) {
			sPublisher = pArray.GetAt(i);
		}
		else {
			sPublisher = sPublisher + _T(":") + pArray.GetAt(i);
		}
	}

	pDM_PROC_0000->SetCellData( _T("IBS_������"), sPublisher, iRowIdx );

	if ( iStatus == 1 ) {
		pDM_PROC_0000->SetCellData( _T("BS_�ڷ����_�ڵ�"), _T("BOC115N"), iRowIdx );
		pDM_PROC_0000->SetCellData( _T("IBS_�۾�����INFO"), _T("BOC115N"), iRowIdx );		
	}
	else {
		pDM_PROC_0000->SetCellData( _T("BS_�ڷ����_�ڵ�"), _T("BOC113N"), iRowIdx );
		pDM_PROC_0000->SetCellData( _T("IBS_�۾�����INFO"), _T("BOC113N"), iRowIdx );
	}

	pDM_PROC_0000->SetCellData( _T("BS_��KEY"), sRecKey, iRowIdx );	
	pDM_PROC_0000->SetCellData( _T("IBS_��KEY"), sRecKey, iRowIdx );	

	m_pInfo->pMarcMgr->DeleteEmptySubfield( m_pMarc );
	m_pInfo->pMarcMgr->DeleteEmptyField( m_pMarc );
	m_pInfo->pMarcMgr->Encoding(m_pMarc, m_strStreamMarc);

	CBO_SPECIES API(this);
	API.SetOpenMode(-2100);
	CESL_DataMgr *pDM_BOOK = FindDM(_T("DM_BO_CAT_PROC_0301_å"));
	
	if ( m_iBookInputsW != 1 ) {
		CBO_CAT_PROC_Api FormApi(this);

		CString strRegCodeField;
		GetDlgItem(IDC_eREG_CLASS)->GetWindowText(strRegCodeField);
		
		CArray<CRegCodeProc, CRegCodeProc> arrRegCode;

		INT ids = FormApi.FetchRegCode(strRegCodeField, &arrRegCode);
		if (ids < 0 || arrRegCode.GetSize() <= 0)
		{
			
		}
		m_strRegCodeField = strRegCodeField;
		CString strVol = _T("");
		pDM_BOOK->InsertRow(-1);
		pDM_BOOK->SetCellData(_T("BB_å�η��÷���"), _T("B"), 0);
		pDM_BOOK->SetCellData(_T("BB_DB_FLAG"), _T("I"), 0);

		CString sItemData = _T("");
		ids = m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("950$b"), sItemData );
		if ( sItemData != _T("") ) {
			ids = pDM_BOOK->SetCellData( _T("BB_����"), sItemData, 0 );
		}

		CString sAuthor, sPubYear;
		m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("����"), sAuthor);
		m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("�����"), sPubYear);

		CString sPage, sPhysicalProperty, sSize;
		m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("�����"), sPage);
		m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("������Ư��"), sPhysicalProperty);
		m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("ũ��"), sSize);	

		ids = pDM_BOOK->SetCellData( _T("BB_������"), sAuthor, 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_�����"), sPubYear, 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_�����"), sPage, 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_������Ư��"), sPhysicalProperty, 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_ũ��"), sSize, 0 );

		CString sCode;
		GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_��ü����_�ڵ�"), sCode);
		ids = pDM_BOOK->SetCellData( _T("BB_��ü����_�ڵ�"), sCode, 0 );

		sCode = _T("");
		GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_�̿����ѱ���_�ڵ�"), sCode);
		ids = pDM_BOOK->SetCellData( _T("BB_�̿����ѱ���_�ڵ�"), sCode, 0 );

		sCode = _T("");
		GetControlData(_T("CM_BO_CAT_PROC_0301"), _T("BS_��������_�ڵ�"), sCode);
		ids = pDM_BOOK->SetCellData( _T("BB_��������_�ڵ�"), sCode, 0 );


		FormApi.MakeBookFromRegCode(&arrRegCode, pDM_BOOK, strVol);
		INT iBookCount = pDM_BOOK->GetRowCount();	
	}

	INT iRowCount = pDM_BOOK->GetRowCount();
	CString sRegCodePre;
	CString sAllNum;
	for ( i = 0 ; i < iRowCount ; i++ ) {
		if ( iStatus == 1 ) {
			pDM_BOOK->SetCellData( _T("BB_�ڷ����"), _T("BOC115N"), i );
		}
		else {
			pDM_BOOK->SetCellData( _T("BB_�ڷ����"), _T("BOC113N"), i );
		}

		CString sDbFlag = pDM_BOOK->GetCellData( _T("BB_DB_FLAG"), i );
		if ( sDbFlag == _T("I") ) {
			CString sInputDate;
			sTime.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
			pDM_BOOK->SetCellData( _T("BB_�Է���"), sTime, i );
		}
		
		CString sBookFlag;
		pDM_BOOK->GetCellData( _T("BB_å�η��÷���"), i, sBookFlag );
		if ( sBookFlag == _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_å�η��÷���"), _T("B"), i );
		}
		pDM_BOOK->SetCellData( _T("BB_���Ա���_�ڵ�"), ACQUISIT_CODE, i );
		INT ids = pDM_BOOK->SetCellData( _T("BB_�ڷ����"), _T("BOC113N"), i );

		sItem = _T("");
		m_pInfo->pMarcMgr->GetItem( m_pMarc , _T("056$a") , sItem );
		if ( sItem == _T("") ) {
			m_pInfo->pMarcMgr->GetItem( m_pMarc , _T("082$a") , sItem );
		}
		if ( sItem != _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_�з���ȣ"), sItem, i );
		}

		sItem = _T("");
		m_pInfo->pMarcMgr->GetItem( m_pMarc , _T("090$b") , sItem );
		if ( sItem != _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_������ȣ"), sItem, i );
		}
	}

	if ( iRowCount < 1 ) {
		INT ids;
		
		ids = pDM_BOOK->InsertRow(-1);
		
		CString sInputDate;
		sTime.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
		pDM_BOOK->SetCellData( _T("BB_�Է���"), sTime, 0 );

		if ( iStatus == 1 ) {
			ids = pDM_BOOK->SetCellData( _T("BB_�ڷ����"), _T("BOC115N"), 0 );
		}
		else {
			ids = pDM_BOOK->SetCellData( _T("BB_�ڷ����"), _T("BOC113N"), 0 );
		}

		CString sBookFlag;

		if ( sBookFlag == _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_å�η��÷���"), _T("B"), 0 );
		}

		CString sAuthor, sPubYear;
		m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("����"), sAuthor);
		m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("�����"), sPubYear);

		CString sPage, sPhysicalProperty, sSize;
		m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("�����"), sPage);
		m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("������Ư��"), sPhysicalProperty);
		m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("ũ��"), sSize);	
		
		ids = pDM_BOOK->SetCellData( _T("BB_��KEY"), sRecKey, 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_���Ա���_�ڵ�"), ACQUISIT_CODE, 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_�̿��󱸺�_�ڵ�"), USE_OBJ_CODE, 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_��ü����_�ڵ�"), MEDIA_CODE, 0 );
		
		CString sDate;
		GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("���糯¥"), sDate );
		ids = pDM_BOOK->SetCellData( _T("BB_�Է���"), sDate, 0 );
				
		ids = pDM_BOOK->SetCellData( _T("BB_��ϱ���_�ڵ�"), sRegCode , 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_������"), sAuthor, 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_�����"), sPubYear, 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_�����"), sPage, 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_������Ư��"), sPhysicalProperty, 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_ũ��"), sSize, 0 );
		ids = pDM_BOOK->SetCellData( _T("BB_DB_FLAG"), _T("I") , 0 );
		
		sItem = _T("");
		m_pInfo->pMarcMgr->GetItem( m_pMarc , _T("056$a") , sItem );
		if ( sItem == _T("") ) {
			m_pInfo->pMarcMgr->GetItem( m_pMarc , _T("082$a") , sItem );
		}
		if ( sItem != _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_�з���ȣ"), sItem, 0 );
		}

		sItem = _T("");
		m_pInfo->pMarcMgr->GetItem( m_pMarc , _T("090$b") , sItem );
		if ( sItem != _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_������ȣ"), sItem, 0 );
		}
	}

	if ( sDontorKey.IsEmpty() ) {
		API.MakeBookQueryFrame(pDM_BOOK, sRecKey, _T("0"), pDM );
	}
	else {
		API.MakeBookQueryFrame(pDM_BOOK, sRecKey, sDontorKey, pDM );
	}
	
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKey);	
	pDM->AddDBFieldData(_T("ACQUISIT_YEAR"), _T("STRING"), ACQUISIT_YEAR);
	
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), m_strStreamMarc);
	pDM->AddDBFieldData(_T("MAT_CODE"), _T("STRING"), MAT_CODE);
	pDM->AddDBFieldData(_T("FORM_CODE"), _T("STRING"), FORM_CODE);
	pDM->AddDBFieldData(_T("SUB_REG_CODE"), _T("STRING"), SUB_REG_CODE);
	pDM->AddDBFieldData(_T("CONTROL_MAT_CODE"), _T("STRING"), CONTROL_MAT_CODE);	

	pDM->AddDBFieldData(_T("PUBLISH_COUNTRY_CODE"), _T("STRING"), PUBLISH_COUNTRY_CODE);
	pDM->AddDBFieldData(_T("KDC_CLASS_NO"), _T("STRING"), KDC_CLASS_NO);

	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N"));
	
	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG));
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG));

	ids = pDM->MakeInsertFrame(_T("BO_SPECIES_TBL"));
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame �� Ȯ���ϼ���"));
		return;
	}
	
	CESL_DataMgr *pDM_IDX = FindDM(_T("DM_BO_CAT_PROC_0301"));
	if (!pDM_IDX) return;

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKey);

	CIndexTool indexTool;
	indexTool.Init(m_pInfo->pMarcMgr, m_pMarc);
	CString strIndexItem;
	INT chk = indexTool.GetIndex( _T("��������") , strIndexItem );
	chk = indexTool.GetIndex( _T("������") , strIndexItem );
	chk = indexTool.GetIndex( _T("�з���ȣ����") , strIndexItem );
	chk = indexTool.GetIndex( _T("Ű��������") , strIndexItem );
	chk = indexTool.GetIndex( _T("����������") , strIndexItem );
	chk = indexTool.GetIndex( _T("����ǥ�ع�ȣ") , strIndexItem );
	chk = indexTool.GetIndex( _T("������") , strIndexItem );
	chk = indexTool.GetIndex( _T("�ѱ����к�ȣ") , strIndexItem );
	chk = indexTool.GetIndex( _T("���α����ȣ") , strIndexItem );
	chk = indexTool.GetIndex( _T("�ι�") , strIndexItem );
	chk = indexTool.GetIndex( _T("��๮���") , strIndexItem );
	chk = indexTool.GetIndex( _T("�������") , strIndexItem );
	chk = indexTool.GetIndex( _T("�������а�") , strIndexItem );
	chk = indexTool.GetIndex( _T("�����") , strIndexItem );
	chk = indexTool.GetIndex( _T("ǥ������INFO") , strIndexItem );
	chk = indexTool.GetIndex( _T("ISBN") , strIndexItem );
	
	pDM->AddDBFieldData(_T("MAT_CODE"), _T("STRING"), MAT_CODE);
	pDM->AddDBFieldData(_T("FORM_CODE"), _T("STRING"), FORM_CODE);
	pDM->AddDBFieldData(_T("MEDIA_CODE"), _T("STRING"), MEDIA_CODE);
	pDM->AddDBFieldData(_T("SUB_REG_CODE"), _T("STRING"), SUB_REG_CODE);
	pDM->AddDBFieldData(_T("USE_OBJ_CODE"), _T("STRING"), USE_OBJ_CODE);
	pDM->AddDBFieldData(_T("USE_LIMIT_CODE"), _T("STRING"), USE_LIMIT_CODE);
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );
	pDM->AddDBFieldData(_T("CLASS_NO_TYPE"), _T("STRING"), CLASS_NO_TYPE);

	CString sIdxAllItem;
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��ǥ��") , strIndexItem );
	pDM->AddDBFieldData(_T("IDX_TITLE"), _T("STRING"), strIndexItem);	
	pDM->AddDBFieldData(_T("TITLE"), _T("STRING"), sTitle);	
	sIdxAllItem = strIndexItem;
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("Ű��������") , strIndexItem );
	pDM->AddDBFieldData(_T("IDX_KEYWORD"), _T("STRING"), strIndexItem);
	sIdxAllItem = sIdxAllItem + _T(" ") + strIndexItem;

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("������") , strIndexItem );
	pDM->AddDBFieldData(_T("IDX_AUTHOR"), _T("STRING"), strIndexItem);
	pDM->AddDBFieldData(_T("AUTHOR"), _T("STRING"), sAuthor);
	sIdxAllItem = sIdxAllItem + _T(" ") + strIndexItem;
	
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("������") , strIndexItem );
	pDM->AddDBFieldData(_T("IDX_PUBLISHER"), _T("STRING"), strIndexItem);
	pDM->AddDBFieldData(_T("PUBLISHER"), _T("STRING"), sPublisher);	
	sIdxAllItem = sIdxAllItem + _T(" ") + strIndexItem;

	CArray<CString, CString&> pArrayIdxAllItem;
	INT iPos = sIdxAllItem.Find( _T(" ") );
	while ( iPos != -1 ) {
		CString sItem = sIdxAllItem.Left( iPos );
		INT iFind = 0;
		for ( INT i = 0 ; i < pArrayIdxAllItem.GetSize() ; i++ ) {
			if ( sItem == pArrayIdxAllItem.GetAt(i) ) {
				iFind = 1;
				break;
			}
		}
		if ( iFind == 0 ) {
			pArrayIdxAllItem.Add( sItem );
		}
		sIdxAllItem = sIdxAllItem.Mid( iPos+1 );
		iPos = sIdxAllItem.Find( _T(" ") );
	}
	sIdxAllItem = _T("");
	for ( i = 0 ; i < pArrayIdxAllItem.GetSize() ; i++ ) {
		sIdxAllItem = sIdxAllItem + _T(" ") + pArrayIdxAllItem.GetAt(i);
	}
	sIdxAllItem = sIdxAllItem.Mid(1);
	pDM->AddDBFieldData(_T("IDX_ALL_ITEM"), _T("STRING"), sIdxAllItem);	

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�����˻��������") , strIndexItem );
	pDM->AddDBFieldData(_T("IDX_IPUBLISHER"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_ISBN") , strIndexItem );
	pDM->AddDBFieldData(_T("ST_CODE"), _T("STRING"), strIndexItem);
	
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_ISSN") , strIndexItem );
	pDM->AddDBFieldData(_T("ST_ISSN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_STRN") , strIndexItem );
	pDM->AddDBFieldData(_T("ST_STRN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_RNSTRN") , strIndexItem );
	pDM->AddDBFieldData(_T("ST_RNSTRN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_����������ȣ") , strIndexItem );
	pDM->AddDBFieldData(_T("ST_CBN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_���۱ǵ�Ϲ�ȣ") , strIndexItem );
	pDM->AddDBFieldData(_T("ST_CAN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��Ÿ��ȣ_û����ȣ_��ġ��ȣ") , strIndexItem );
	pDM->AddDBFieldData(_T("SE_SHELF_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��Ÿ��ȣ_û����ȣ_�з���ȣ") , strIndexItem );
	pDM->AddDBFieldData(_T("CLASS_NO"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��Ÿ��ȣ_û����ȣ_������ȣ") , strIndexItem );
	pDM->AddDBFieldData(_T("BOOK_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��Ÿ��ȣ_û����ȣ_��å��ȣ") , strIndexItem );
	pDM->AddDBFieldData(_T("VOL_CODE_DISP"), _T("STRING"), strIndexItem);	//_T("��Ÿ��ȣ_û����ȣ_��å��ȣ_DISP")
	pDM->AddDBFieldData(_T("VOL_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��Ÿ��ȣ_�����ȣ") , strIndexItem );
	pDM->AddDBFieldData(_T("CONTROL_NO"), _T("STRING"), strIndexItem);
	
	
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("KDCP") , strIndexItem );
	pDM->AddDBFieldData(_T("KDCP_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("KDC") , strIndexItem );
	pDM->AddDBFieldData(_T("KDC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("DDC") , strIndexItem );
	pDM->AddDBFieldData(_T("DDC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("CEC") , strIndexItem );
	pDM->AddDBFieldData(_T("CEC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("CWC") , strIndexItem );
	pDM->AddDBFieldData(_T("CWC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("COC") , strIndexItem );
	pDM->AddDBFieldData(_T("COC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("UDC") , strIndexItem );
	pDM->AddDBFieldData(_T("UDC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("NDC") , strIndexItem );
	pDM->AddDBFieldData(_T("NDC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("LC") , strIndexItem );
	pDM->AddDBFieldData(_T("LC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�ѱ����к�ȣ") , strIndexItem );
	pDM->AddDBFieldData(_T("UNI_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("���α����ȣ") , strIndexItem );
	pDM->AddDBFieldData(_T("GOV_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�������а�") , strIndexItem );
	pDM->AddDBFieldData(_T("DEG_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("����") , strIndexItem );
	pDM->AddDBFieldData(_T("EDIT"), _T("STRING"), strIndexItem);
	
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�����˻������⵵") , strIndexItem );
	pDM->AddDBFieldData(_T("IDX_IPUB_YEAR"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("����⵵") , strIndexItem );
	pDM->AddDBFieldData(_T("PUB_YEAR"), _T("STRING"), strIndexItem);
	
	pDM_PROC_0000->SetCellData( _T("IBS_����⵵"), strIndexItem, iRowIdx );

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�������") , strIndexItem );
	pDM->AddDBFieldData(_T("TEXT_LANG"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��๮���") , strIndexItem );
	pDM->AddDBFieldData(_T("SUM_LANG"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ������") , strIndexItem );
	pDM->AddDBFieldData(_T("TITLE_INFO"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�����ڻ���") , strIndexItem );
	pDM->AddDBFieldData(_T("AUTHOR_INFO"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�����ڻ���") , strIndexItem );
	pDM->AddDBFieldData(_T("PUB_INFO"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("����⵵����") , strIndexItem );
	pDM->AddDBFieldData(_T("PUB_YEAR_INFO"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	if ( iStatus == 1 ) {
		pDM->AddDBFieldData(_T("WORK_STATUS"), _T("STRING"), _T("BOC115N"));
	}
	else {
		pDM->AddDBFieldData(_T("WORK_STATUS"), _T("STRING"), _T("BOC113N"));
	}

	pDM->AddDBFieldData(_T("SPECIES_CLASS"), _T("STRING"), _T("0"));

	ids = pDM->MakeInsertFrame(_T("IDX_BO_TBL"));	
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame �� Ȯ���ϼ���"));
		return;
	}

	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return;
	}
	ids = pDM->EndFrame();
		if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame �� Ȯ���ϼ���"));
		return;
	}
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0000"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0000 error") );
		return;
	}
	pCM->AllControlDisplay();

	AfxMessageBox(_T("�����Ͽ����ϴ�."));

	CString s008New;
	m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("008*"), s008New );	
	ids = m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_PreMarc);

	m_pMarc->Init();

	m_marcEditor.InitUserAlias();	

	this->GetManageValue(_T("����"), _T("����"), _T("��ũ������׸�"), _T("����"), str);
	CMapStringToPtr MapUserAlias;
	m_pMarcDefaultValueConfigDlg->LoadDefaultUserMarcAlias(&MapUserAlias);
	CStringArray *pStringArray = NULL;
	ids = m_pMarcDefaultValueConfigDlg->GetUserMarcAlias(&MapUserAlias, m_pInfo->pMarcMgr->GetMarcGroup(), &pStringArray);
	if (ids >= 0)
	{
		INT nCount = pStringArray->GetSize();
		for (INT idx = 0; idx < nCount; idx++)
		{
			m_marcEditor.AddUserAlias(pStringArray->GetAt(idx));
		}
	}
	m_marcEditor.InitLimitTag();
	m_marcEditor.AddLimitTag(_T("008"));
	m_marcEditor.AddLimitTag(_T("020"));
	m_marcEditor.AddLimitTag(_T("007"));

	CMapStringToPtr MapUserMarc;
	m_pMarcDefaultValueConfigDlg->LoadDefaultMarc(&MapUserMarc);
	
	ids = m_pMarcDefaultValueConfigDlg->GetUserMarc(&MapUserMarc, m_pInfo->pMarcMgr->GetMarcGroup(), &m_pMarc);
	if (ids >= 0)
		m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);

	m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("008*"), s008New );


	m_marcEditor.Display();
	m_strStreamMarc = _T("");
	m_pInfo->pMarcMgr->Encoding( m_pMarc, m_strStreamMarc );
EFS_END
}

VOID CBO_CAT_PROC_0301::OnbSTATIC3() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CMNG_CLASS_SIGN_POPUP_01 dlg( this );
	if( IDOK == dlg.DoModal() )
	{
		m_marcEditor.ApplyEdit();

		if ( dlg.m_KDCtag != _T("") ) {
			CString strItemData = dlg.m_KDCtag.Mid( 7 , dlg.m_KDCtag.GetLength()-8 );
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("056$a"), strItemData , _T(""), ADD_SUBFIELD);
		}

		if ( dlg.m_DDCtag != _T("") ) {
			CString sDDC = dlg.m_DDCtag.Mid( 7 , dlg.m_DDCtag.GetLength()-8 );
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("082$a"), sDDC , _T(""), ADD_SUBFIELD);
		}

		m_pInfo->pMarcMgr->SortMarc( m_pMarc );

		m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
		m_marcEditor.Display();
	}
	

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbSUBJECTREF3() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	m_marcEditor.ApplyEdit();
	CBO_CAT_PROC_0330 dlg(this,_T(""));
	if( IDOK == dlg.DoModal() ) {
		INT iLen = dlg.m_KDC.GetLength();
		if ( iLen > 1 ) {
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("056$a"), dlg.m_KDC , _T(""), ADD_SUBFIELD);
			m_pInfo->pMarcMgr->SortMarc( m_pMarc );
		}
		iLen = dlg.m_DDC.GetLength();
		if ( iLen > 1 ) {
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("082$a"), dlg.m_DDC , _T(""), ADD_SUBFIELD);
			m_pInfo->pMarcMgr->SortMarc( m_pMarc );
		}

		m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
		m_marcEditor.Display();
	}
	

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbCOLNUMBER() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	m_marcEditor.ApplyEdit();

	CString strStreamMarc = m_strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(m_pMarc, m_strStreamMarc);

	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, m_pMarc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}
	
	CString strItemData;
	
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("090$a"), strItemData);
	if ( strItemData != _T("") ) {
		AfxMessageBox( _T("MARC�� û����ȣ�� �����մϴ�.") );
		return;
	}
	
	CString sCss;
	GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("�з�ǥ����"), sCss );
	sCss.Replace( _T(" "), NULL );
	if ( sCss == _T("") ) {
		AfxMessageBox( _T("�з�ǥ������ �����ϼ���.") );
		return;
	}
	else if ( sCss == _T("1") ) {
		m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("056$a"), strItemData);
		if ( strItemData == _T("") ) {
			AfxMessageBox( _T("�з�ǥ���п� �´� �з���ȣ�� �����ϴ�.") );
			return;
		}
	}
	else if ( sCss == _T("2") ) {
		m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("082$a"), strItemData);	
		if ( strItemData == _T("") ) {
			AfxMessageBox( _T("�з�ǥ���п� �´� �з���ȣ�� �����ϴ�.") );
			return;
		}
	}
	m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("090$a"), strItemData);

	CESL_DataMgr *pDmAuthorRule;
	pDmAuthorRule = FindDM( _T("DM_BO_CAT_PROC_0300_AUTHOR_RULE") );
	if ( pDmAuthorRule == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_AUTHOR_RULE error") );
		return;
	}
	CString sFormCode;
	GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_���ı���_�ڵ�"), sFormCode );
	CString sSubRegCode;
	GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_������ϱ���_�ڵ�"), sSubRegCode );
	CString strWhere;
	strWhere.Format( _T("FORM_CODE = '%s' AND SUB_REG_CODE = '%s'"), sFormCode, sSubRegCode );
	if (pDmAuthorRule->RefreshDataManager(strWhere) < 0)	return;	
	if ( pDmAuthorRule->GetRowCount() < 1 ) {
		AfxMessageBox (_T("���ڱ�ȣ������ �߸��Ǿ����ϴ�."));
		return;
	}
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDmAuthorRule, _T("ǥ�����ڱ�ȣ"), _T("�������"));
	INT iAuthorCount = pDmAuthorRule->GetRowCount();
	CString sTagCnt, sTag, sSubfield, sRuleType;
	if ( iAuthorCount > 0 ) {
		pDmAuthorRule->GetCellData( _T("TAG_CNT"), 0, sTagCnt );
		pDmAuthorRule->GetCellData( _T("TAG"), 0, sTag );
		pDmAuthorRule->GetCellData( _T("�ĺ���ȣ"), 0, sSubfield );
		pDmAuthorRule->GetCellData( _T("�������"), 0, sRuleType );
	}
	if ( sRuleType != _T("���Լ���ȣ") ) {
		CString sFilePath = _T("..\\CFG\\") + sRuleType + _T(".txt");
		
		INT iTagCnt = _ttoi( sTagCnt.GetBuffer(0) );
		for ( INT i = 1 ; i <= iTagCnt ; i++ ) {
			CString sTagSub;
			if ( i == 1 ) {
				sTagSub = sTag.Left(3) + _T("$") + sSubfield.Left(1);
			}
			else {
				INT iStart = (i-1)*3+1;
				sTagSub = sTag.Mid(iStart, 3);
			}
			strItemData = _T("");
			m_pInfo->pMarcMgr->GetItem(m_pMarc, sTagSub, strItemData);
			if ( strItemData != _T("") ) {
				break;
			}
		}

		if ( strItemData != _T("") ) {
			CString sData = strItemData.Left(2);
			CString sAuthorMid, sAuthorNo;
			if ( strItemData.Mid(2,1) == _T(" ") ) {
				sAuthorMid = _T(",") + strItemData.Mid(3,2);
			}
			else {
				sAuthorMid = strItemData.Mid(2,2);
			}

			if ( sAuthorMid != _T("") ) {
				INT curdrive;	
				curdrive = _getdrive();
				CString sPath;
				if(curdrive==3){
					sPath=_T("C:\\KOLAS2UP\\CFG\\����ö2ǥ.txt");
				}
				if(curdrive==4){
					sPath=_T("D:\\KOLAS2UP\\CFG\\����ö2ǥ.txt");
				}
				if(curdrive==5){
					sPath=_T("E:\\KOLAS2UP\\CFG\\����ö2ǥ.txt");
				}

				sPath = _T("..\\CFG\\����ö2ǥ.txt");

				FILE *fp;
				fp = _tfopen( sFilePath , _T("rb") );
				if ( fp == NULL ) {
					AfxMessageBox( _T("���ڱ�ȣ���� ������ �����ϴ�.") );
					return;
				}
				
				FILE* fpUni = fp;
			
				if( 0xFF == fgetc(fpUni) ) 
				{
					if( 0xFE != fgetc(fpUni) )
					{
						fseek(fpUni, 0, SEEK_SET);
					}
				}
				else
				{
					fseek(fpUni, 0, SEEK_SET);
				}

				TCHAR cLine[256];
				while( _fgetts( cLine, 256, fp ) ) {
					CString sLine;
					sLine.Format( _T("%s"), cLine );
					CString tnp = sLine.Mid(0,3);
					tnp.Replace( _T(" "), NULL );
					if ( sAuthorMid == tnp ) {
						INT iPos = sLine.Find(_T("|"));
						sAuthorNo = sLine.Mid( iPos+1, 2 );
						sAuthorNo.Replace( _T(" "), NULL );
						if ( sAuthorMid.Left(1) == _T(",") ) {
							sAuthorNo = _T(",") + sAuthorNo;						
						}
						break;
					}				
				}
			}
			strItemData = sData + sAuthorNo;
			
			CString sTitle;
			m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("245$a"), sTitle);
			CString sFirstWord = sTitle.Left(2);
			if ( sFirstWord != _T("") ) {
				FILE *fpFirst;
				sFilePath = _T("..\\CFG\\FIRST_CHAR_TBL.txt");
				fpFirst = _tfopen( sFilePath , _T("rb") );			
				if ( fpFirst == NULL ) {
					AfxMessageBox( _T("FIRST_CHAR_TBL.tx ������ �����ϴ�.") );
					return;
				}
				
				FILE* fpUni = fpFirst;
			
				if( 0xFF == fgetc(fpUni) ) 
				{
					if( 0xFE != fgetc(fpUni) )
					{
						fseek(fpUni, 0, SEEK_SET);
					}
				}
				else
				{
					fseek(fpUni, 0, SEEK_SET);
				}

				TCHAR cLine[256];
				while( _fgetts( cLine, 256, fpFirst ) ) {
					CString sLine;
					sLine.Format( _T("%s"), cLine );
					if ( sLine.Left(1) == _T(".") ) {
						continue;
					}
					CString sTmp = sLine.Left(2);
					if ( sFirstWord == sLine.Left(2) ) {
						INT iPos = sLine.Find(_T("|"));
						CString sResult = sLine.Mid( iPos+1, 2 );
						strItemData = strItemData + sResult;
						break;
					}
				}
			}
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("090$b"), strItemData);
		}
	}
	else {
		iSwAcqNumber = 1;
	}
	
	CArray<CString, CString&> pArray;
	CString sItemData;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("049$v"), sItemData, &pArray);
	
	CString sRegPre;
	INT iCnt = pArray.GetSize();
	for ( INT i = 1 ; i <= iCnt ; i ++ ){
		CString sReg = pArray.GetAt(i-1);
		if ( 1 == i ) {
			sItemData = sReg;
		}
		else if ( sRegPre != sReg ) {
			sItemData = sItemData + _T(",") + sReg;
		}
		sRegPre = sReg;
	}

	if ( sItemData != _T("") ) {
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("090$c"), sItemData);
	}


	m_pInfo->pMarcMgr->SortMarc( m_pMarc );

	m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
	m_marcEditor.Display();

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbCOLNUMBER2() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	m_marcEditor.ApplyEdit();
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0380"));

	CString sCss;	
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("090$a"), sCss);
	if ( sCss == _T("") ) {
		AfxMessageBox (_T("���� û����ȣ������ �Ͻʽÿ�."));
		return;
	}

	CString sBookCode;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("090$b"), sBookCode );

	CString strWhere;
	
	strWhere.Format( _T("I.REC_KEY = S.REC_KEY AND I.CLASS_NO = '%s' AND I.BOOK_CODE = '%s'"), sCss, sBookCode );	

	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	INT iRowCount = pDM->GetRowCount();

	if ( iRowCount > 1 ) {
		CBO_CAT_PROC_0380 dlg(this);
		dlg.DoModal();
	}
	else { 
		AfxMessageBox( _T("�ߺ��� û����ȣ�� �����ϴ�.") );
	}
	

EFS_END
}

VOID CBO_CAT_PROC_0301::OnSelchangecFORMCLASS() 
{
EFS_BEGIN

	INT ids;
	ids = ::MessageBox( GetSafeHwnd() ,
                        _T("���ı����� �����ϸ� ������ MARC �ڷᰡ ���õ˴ϴ�. �����Ͻðڽ��ϱ� ?\n���ı��� �����Ŀ��� �˸��� ��ü���а��� �����ߴ��� Ȯ���Ͻñ� �ٶ��ϴ�."),
                        _T("���� Ȯ��") ,
                        MB_YESNO | MB_ICONQUESTION 
                      );
    if ( ids == IDYES ) {
		m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("008") );				
		m_marcEditor.Display();	
	}

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbBOKBONSEARCH() 
{
EFS_BEGIN

	m_marcEditor.ApplyEdit();
	m_pInfo->pMarcMgr->Encoding(m_pMarc, m_strStreamMarc);
	
	CString Skey, strTitle, strResultCnt;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("245$a"), strTitle);
	CDuplicateSearch_Interface impl(this);
	impl.ShowOnlyTitleDuplicateSearch(_T("��������������"), strTitle, strResultCnt, Skey);

	if(strResultCnt == _T("0"))
	{
		AfxMessageBox( _T("���� �������� ����� �����ϴ�.") );
		return;
	}

	impl.ShowTitleDuplicateSearchDlg(_T("��������������"), Skey);

	if ( Skey.IsEmpty() ) {
		return;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);	
	CString sQuery_marc = _T("select marc from bo_species_tbl where rec_key = ") + Skey;
	dm.RestructDataManager(sQuery_marc);	
	
	dm.GetCellData( 0, 0, m_strStreamMarc);

	m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, m_pMarc);

	m_marcEditor.Display();
	
	return;

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbKEYWORDCREAT() 
{
EFS_BEGIN

	CBO_CAT_PROC_KEYWORD_0000 dlg(this, *m_pMarc);
	if( IDOK == dlg.DoModal() ) {		

		CString strStreamMarc = m_strStreamMarc;
		m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);	

		CString strTagCode, strErrMsg, strHelpMsg;
		
		if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, m_pMarc))
		{
			POSITION pos;
			pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
			while (pos)
			{
				m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
				UpdateData(FALSE);
			}	
		}	

		m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
		m_marcEditor.Display();

		TCHAR szLeader[25];
		m_pMarc->GetLeader(szLeader);

		CString sLeader;
		sLeader.Format( _T("%s") , szLeader );
		SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);
	}
	

EFS_END
}

VOID CBO_CAT_PROC_0301::OnBtnPresent() 
{
EFS_BEGIN

	CDonatorNameGetterDlg dlg( this );
	if( IDOK == dlg.DoModal() ){		
		SetControlData(_T("CM_BO_CAT_PROC_0301"), _T("������"), dlg.m_donatorName);
	}

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbSELECT2() 
{
EFS_BEGIN

	CreatePopUpButton( IDC_bSELECT2 );	

EFS_END
}

VOID CBO_CAT_PROC_0301::CreatePopUpButton( UINT f_inButtonType )
{
EFS_BEGIN

    CMenu      m_ContextMenu;
    CRect      s_rect;

    CButton * pButton = ( CButton * )GetDlgItem( f_inButtonType );
    if ( pButton == NULL ) {
        return;
    }

    m_ContextMenu.CreatePopupMenu();
	switch( f_inButtonType ) {
		case IDC_bSELECT2:
			m_ContextMenu.AppendMenu( MF_STRING , 3086 , _T("��ġ��ȣ��ȸ") );
			m_ContextMenu.AppendMenu( MF_STRING , 3226 , _T("���Լ���ȣ����") );
			m_ContextMenu.AppendMenu( MF_STRING , 3220 , _T("���ڱ�ȣ��ȸ") );
			m_ContextMenu.AppendMenu( MF_STRING , 3230 , _T("���Ҿ����") );
			m_ContextMenu.AppendMenu( MF_STRING , 3161 , _T("�з���ȣ����") );			
			break;
		default:
			break;
	}

    pButton->GetWindowRect( &s_rect );
	m_ContextMenu.TrackPopupMenu( TPM_LEFTALIGN, s_rect.right, s_rect.top, this );

	return;
	
EFS_END
}

VOID CBO_CAT_PROC_0301::OnbREFERENCE() 
{
EFS_BEGIN


EFS_END
}

VOID CBO_CAT_PROC_0301::OnSelchangeCOBAcqCode() 
{
EFS_BEGIN

	CButton * pPresent;
	pPresent = (CButton *)GetDlgItem(IDC_BTN_PRESENT);		

	CButton * pPresentSt;
	pPresentSt = (CButton *)GetDlgItem(IDC_STATIC_PRESENT);	

	CButton * pPresentEd;
	pPresentEd = (CButton *)GetDlgItem(IDC_EDT_PRESENT);	

	CString sAcqCode;
	GetControlData( _T("CM_BO_CAT_PROC_0301") , _T("BB_���Ա���_�ڵ�") , sAcqCode );
	if ( sAcqCode == _T("2") ) {
		pPresent->ShowWindow(SW_SHOW);	
		pPresentSt->ShowWindow(SW_SHOW);	
		pPresentEd->ShowWindow(SW_SHOW);	
	}
	else {
		pPresent->ShowWindow(SW_HIDE);	
		pPresentSt->ShowWindow(SW_HIDE);	
		pPresentEd->ShowWindow(SW_HIDE);	
	}

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbCOMMONAPPLY3() 
{
EFS_BEGIN

	CBO_CAT_PROC_0301_ROLE dlg(this);
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbSUBJECTREF2() 
{
EFS_BEGIN

	CBO_CAT_ACQUISIT_NO dlg(1, this);
	dlg.DoModal();	

EFS_END
}

VOID CBO_CAT_PROC_0301::OnSelchangetabForm(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN

	CTabCtrl	*pTab = (CTabCtrl*)GetDlgItem(IDC_tabForm);
	if (pTab == NULL) return;

	INT nSel = pTab->GetCurSel();

	if (nSel == 0)
	{
		GetDlgItem(IDC_redt0301MarcView)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MSHFLEXGRID1)->ShowWindow(SW_SHOW);

		CRect rect;
		GetDlgItem(IDC_redt0301MarcView)->GetWindowRect( rect );
		ScreenToClient(rect);		
		rect.left = 355;
		GetDlgItem(IDC_redt0301MarcView)->MoveWindow( rect );	

		GetDlgItem(IDC_MSHFLEXGRID1)->GetWindowRect( rect );
		ScreenToClient(rect);		
		rect.right = 355;
		GetDlgItem(IDC_MSHFLEXGRID1)->MoveWindow( rect );
		
		CMarcGridCtrl *pGrid = m_marcEditor.GetGridCtrl();
		if (pGrid)
		{
			pGrid->SetGridColWidth( -1, -1, 200 );
			pGrid->SetGridFormat();
		}
	}
	else if (nSel == 1)
	{
		GetDlgItem(IDC_redt0301MarcView)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MSHFLEXGRID1)->ShowWindow(SW_SHOW);

		CRect rect;
		GetDlgItem(IDC_MSHFLEXGRID1)->GetWindowRect( rect );
		ScreenToClient(rect);		
		rect.right = 740;
		GetDlgItem(IDC_MSHFLEXGRID1)->MoveWindow( rect );

		CMarcGridCtrl *pGrid = m_marcEditor.GetGridCtrl();
		if (pGrid)
		{
			pGrid->SetGridColWidth( -1, -1, 580 );
			pGrid->SetGridFormat();
		}
	}
	else if ( nSel == 2 ) {
		GetDlgItem(IDC_redt0301MarcView)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MSHFLEXGRID1)->ShowWindow(SW_HIDE);

		CRect rect;
		GetDlgItem(IDC_redt0301MarcView)->GetWindowRect( rect );
		ScreenToClient(rect);		
		rect.left = 10;
		GetDlgItem(IDC_redt0301MarcView)->MoveWindow( rect );	
	}

	*pResult = 0;

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbINQUIRY() 
{
EFS_BEGIN

	CShelfCodeGetterDlg dlg(this);
	if ( dlg.DoModal() == IDOK ) {
		CString sCode = dlg.m_code;
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("049$f"), sCode );
		m_marcEditor.Display();
	}

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbCOMMONAPPLY2() 
{
EFS_BEGIN

	CKOMarcGetterDlg dlg( this );
	dlg.InitSetProperty( 4, _T("") );
	dlg.DoModal();
	CString sCode = dlg.m_code;	

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbMarcImport() 
{
EFS_BEGIN

	CMARC_IMPORT dlg(this);
	if ( dlg.DoModal() == IDOK ) {
		CString sRecKey = dlg.m_strSpecies_key;
		INT iMode = dlg.m_nRadBasicInfo;

		CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0000") );
		if ( pDM == NULL ) {
			AfxMessageBox( _T("BO_CAT_PROC_0000 ERROR") );
			return;
		}

		CString sQuery;
		sQuery.Format( _T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY = %s"), sRecKey );
		CString sStreamMarc;
		pDM->GetOneValueQuery( sQuery , sStreamMarc );

		CMarc marcTmp;
		if ( iMode == 0 ) {			
			m_pMarc->Init();
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, &marcTmp);
			CString sItemData;
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("245"), sItemData );			
			if ( sItemData != _T("") ) {
				sItemData = _T("245") + sItemData;
				m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("250"), sItemData );			
			if ( sItemData != _T("") ) {
				sItemData = _T("250") + sItemData;
				m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("260"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("260") + sItemData;
				m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("300"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("300") + sItemData;
				m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("950"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("950") + sItemData;
				m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("020"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("020") + sItemData;
				m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}
		}
		else if ( iMode == 1 ) {
			m_pMarc->Init();
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, m_pMarc);
			m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("001") );
			m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("049") );
		}
		else if ( iMode == 2 ) {
			m_pMarc->Init();
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, m_pMarc);
			m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("001") );
			m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("049") );
			m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("505") );
		}

		m_marcEditor.Display();
	}

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbACCESSIONOBJIMPORT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	CUC_MARC_IMPORT dlg(this);
	if ( dlg.DoModal() == IDOK ) {
		
		INT iMode = dlg.m_check;
		CString sStreamMarc = dlg.sMarc;
		CMarc marcTmp;
		if ( iMode == 0 ) {			
			m_pMarc->Init();
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, &marcTmp);
			CString sItemData;
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("245"), sItemData );			
			if ( sItemData != _T("") ) {
				sItemData = _T("245") + sItemData;
				m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("250"), sItemData );			
			if ( sItemData != _T("") ) {
				sItemData = _T("250") + sItemData;
				m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("260"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("260") + sItemData;
				m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("300"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("300") + sItemData;
				m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("950"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("950") + sItemData;
				m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("020"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("020") + sItemData;
				m_pInfo->pMarcMgr->InsertField( m_pMarc, sItemData );
			}
		}
		else if ( iMode == 1 ) {
			m_pMarc->Init();
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, m_pMarc);
			m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("001") );
			m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("049") );
		}
		else if ( iMode == 2 ) {
			m_pMarc->Init();
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, m_pMarc);
			m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("001") );
			m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("049") );
			m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("505") );
		}

		m_marcEditor.Display();
		
	}
	

EFS_END
}

VOID CBO_CAT_PROC_0301::OnbConfig() 
{
EFS_BEGIN

	CMarcDefaultValueConfigDlg dlg(this);
	if ( dlg.DoModal() == IDOK ) {
		m_marcEditor.InitUserAlias();

		CString str;
		this->GetManageValue(_T("����"), _T("����"), _T("��ũ������׸�"), _T("����"), str);

		CMapStringToPtr MapUserAlias;
		CString str2 = str;
		CString strTmp;
		TCHAR szGroupName[256];
		TCHAR szAlias[1024];

		TCHAR *lpszToken = _tcstok(str2.GetBuffer(0), _T(";"));
		while (lpszToken)
		{
			_stscanf(lpszToken, _T("%[^:]:%s"), szGroupName, szAlias);
			
			CStringArray *pStringArray = new CStringArray;
			
			TCHAR *lpszSubToken = _tcstok(szAlias, _T(","));
			while (lpszSubToken)
			{
				pStringArray->Add(lpszSubToken);
				lpszSubToken = _tcstok(NULL, _T(","));
			}

			MapUserAlias.SetAt(szGroupName, pStringArray);

			strTmp = str.Mid(str.Find(lpszToken) + _tcslen(lpszToken) + 1);

			lpszToken = _tcstok(strTmp.GetBuffer(0), _T(";"));
		}

		CStringArray *pStringArray = NULL;
		if (MapUserAlias.Lookup(m_pInfo->pMarcMgr->GetMarcGroup(), (VOID*&)pStringArray))
		{
			INT nCount = pStringArray->GetSize();
			for (INT idx = 0; idx < nCount; idx++)
			{
				m_marcEditor.AddUserAlias(pStringArray->GetAt(idx));
			}
		}
		m_marcEditor.Display();
	}	

EFS_END
}

INT CBO_CAT_PROC_0301::Create505tag()
{
EFS_BEGIN

	CESL_DataMgr *pDM_BOOK = FindDM( _T("DM_BO_CAT_PROC_0301_å") );
	if ( pDM_BOOK == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0301_å error") );
		return -1;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0301_��") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0301_�� error") );
		return -1;
	}
	INT ids = pDM->FreeData();

	CBO_SPECIES API(this);
	API.CONVERT_BOOK_to_VOLUME( pDM_BOOK, pDM );

	INT iRowCount;
	iRowCount = pDM->GetRowCount();
	if ( iRowCount >= 2 ) {
		m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("505") );
		CString sVolPre;
		for( INT i = 0 ; i < iRowCount ; i ++ ) {
			CString sVolTitle, sVol, sAuthor;
			sVolTitle = pDM->GetCellData( _T("BB_������ηϸ�"), i );
			sVol = pDM->GetCellData( _T("BB_�����"), i );
			sAuthor = pDM->GetCellData( _T("BB_������"), i );
			if ( sVol != sVolPre ) {
				if ( sVol != _T("") ) {
					m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("505$n"), sVol );
				}
				if ( sVolTitle != _T("") ) {
					m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("505$t"), sVolTitle );
				}				
				if ( sAuthor != _T("") ) {
					if ( i < iRowCount -1 ) {
						sAuthor = sAuthor + _T(". -");
					}
					m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("505$d"), sAuthor );
				}
				

			}
			sVolPre = sVol;
		}
		m_marcEditor.Display();
	}
	
	return 0;

EFS_END
return -1;

}


UINT CBO_CAT_PROC_0301::OnCreate505Tag(WPARAM wParam, LPARAM lParam)
{
EFS_BEGIN
	INT ids = Create505tag();
	if (ids >= 0)
	{
		MessageBox(_T("505Tag�� �����Ͽ����ϴ�."), NULL, MB_OK | MB_ICONINFORMATION);
	}
EFS_END
	return 0;
}

VOID CBO_CAT_PROC_0301::OnKillfocusCOBAcqCode() 
{

}

VOID CBO_CAT_PROC_0301::OnKillfocuseREGCLASS() 
{
EFS_BEGIN
	CString strRegCodeField;
	GetDlgItem(IDC_eREG_CLASS)->GetWindowText(strRegCodeField);
	if ( strRegCodeField == _T("") ) {
		return;
	}

	CESL_DataMgr *pDM_BOOK = FindDM( _T("DM_BO_CAT_PROC_0301_å"));
	if ( pDM_BOOK == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0301_å error") );
		return;
	}

	CBO_CAT_PROC_Api FormApi(this);

	CArray<CRegCodeProc, CRegCodeProc> arrRegCode;
	INT ids = FormApi.FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0)
	{
		AfxMessageBox( _T("Ʋ�� ��ϱ����Դϴ�.") );
		GetDlgItem(IDC_eREG_CLASS)->SetWindowText(m_strRegCodeField);
		return;
	}

	INT nRegCodeCount = arrRegCode.GetSize();
	if (nRegCodeCount <= 0) return;
	for ( INT i = 0 ; i < nRegCodeCount ; i ++ ) {
		CRegCodeProc sRegCode = arrRegCode.GetAt(i);
	}

	CString strVol;
EFS_END
}

VOID CBO_CAT_PROC_0301::OnSelchangec0301FORMCLASS() 
{	
EFS_BEGIN

	INT ids;
	ids = ::MessageBox( GetSafeHwnd() ,
                        _T("���ı����� �����ϸ� ������ MARC �ڷᰡ ���õ˴ϴ�. �����Ͻðڽ��ϱ� ?\n���ı��� �����Ŀ��� �˸��� ��ü���а��� �����ߴ��� Ȯ���Ͻñ� �ٶ��ϴ�."),
                        _T("���� Ȯ��") ,
                        MB_YESNO | MB_ICONQUESTION 
                      );
    if ( ids == IDYES ) {
		m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("008") );
		m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("007") );
		m_marcEditor.Display();	
	}
	else {
		SetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_���ı���_�ڵ�"), m_sFormCode );
	}
	GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_���ı���_�ڵ�"), m_sFormCode );
EFS_END
}

UINT CBO_CAT_PROC_0301::OnMarcGridUpdate(WPARAM wParam, LPARAM lParam)
{
EFS_BEGIN
	CString s440a, s440v, s490a;
	m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("440$a"), s440a );
	m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("440$v"), s440v );
	m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("490$a"), s490a );

	if ( s440a != _T("") && s440v == _T("") ) {
		m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("440$a"), s440a );
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("490$a"), s440a );
	}
	else if ( s490a != _T("") && s440v != _T("") ) {
		m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("490$a"), s490a );
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("440$a"), s490a );
		m_pInfo->pMarcMgr->DeleteItem( m_pMarc, _T("440$v"), s440v );
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("440$v"), s440v );
	}

	m_pInfo->pMarcMgr->DeleteEmptySubfield( m_pMarc );
	m_pInfo->pMarcMgr->DeleteEmptyField( m_pMarc );
EFS_END
	return 0;
}

VOID CBO_CAT_PROC_0301::Onb0301PreMarc() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	m_pInfo->pMarcMgr->Encoding(&m_PreMarc, m_strStreamMarc);
	INT ids = m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, m_pMarc);
	m_marcEditor.Display();
EFS_END
}

VOID CBO_CAT_PROC_0301::OnbCOMMONAPPLY() 
{
EFS_BEGIN
	CCatCodeGetterDlg dlg(this);
	dlg.DoModal();
EFS_END
}

VOID CBO_CAT_PROC_0301::OnKillfocusredt0301MarcView(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN
	m_marcEditor.ApplyEdit();
	*pResult = 0;
EFS_END
}

LRESULT CBO_CAT_PROC_0301::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
EFS_BEGIN
	if (message == WM_COMMAND)
	{
		if (EN_KILLFOCUS == HIWORD(wParam))
		{
			if (IDC_redt0301MarcView == LOWORD(wParam))
			{
				INT iModifyFlag = pRichEdit->GetModify();
				if ( iModifyFlag != 0 ) {
					m_marcEditor.ApplyEdit();
				}
			}
		}
		if (EN_SETFOCUS == HIWORD(wParam)){
			if (IDC_redt0301MarcView == LOWORD(wParam)){
				pRichEdit->SetModify(false);
			}
		}
	}
EFS_END
	return CDialog::WindowProc(message, wParam, lParam);
}

VOID CBO_CAT_PROC_0301::OnSelchangec0301MoneyCode() 
{
EFS_BEGIN
	CString sPrice;
	CString sOriPrice;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("950$b"), sPrice );
	for ( INT i = 0 ; i < sPrice.GetLength() ; i++ ) {
		CString sOneWord;
		sOneWord = sPrice.Mid( i, 1 );
		if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
			sOriPrice = sOriPrice + sOneWord;
		}
	}

	CString sPriceCode;
	GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_ȭ�󱸺�"), sPriceCode );
	sOriPrice = sPriceCode + sOriPrice;
	m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("950$b"), sOriPrice );
	m_marcEditor.Display();
EFS_END
}

VOID CBO_CAT_PROC_0301::OnSetfocusc0301FORMCLASS() 
{

}

VOID CBO_CAT_PROC_0301::OnSelendokc0301FORMCLASS() 
{

}

VOID CBO_CAT_PROC_0301::OnSelendcancelc0301FORMCLASS() 
{

}

VOID CBO_CAT_PROC_0301::OnDropdownc0301FORMCLASS() 
{
EFS_BEGIN
	GetControlData( _T("CM_BO_CAT_PROC_0301"), _T("BS_���ı���_�ڵ�"), m_sFormCode );	
EFS_END
}

VOID CBO_CAT_PROC_0301::OnSetfocusredt0301MarcView(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN
	*pResult = 0;
EFS_END
}

BEGIN_EVENTSINK_MAP(CBO_CAT_PROC_0301, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_PROC_0301)
	ON_EVENT(CBO_CAT_PROC_0301, IDC_MSHFLEXGRID1, -602 /* KeyDown */, OnKeyDownMshflexgrid1, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_PROC_0301::OnKeyDownMshflexgrid1(short FAR* KeyCode, short Shift) 
{

}

BOOL CBO_CAT_PROC_0301::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN
	if (VK_RETURN == pMsg->wParam ) { 
		if (GetDlgItem(IDC_MSHFLEXGRID1)->m_hWnd == pMsg->hwnd) {
			
			CMarcGridCtrl *pGrid = m_marcEditor.GetGridCtrl();
			INT iRow = pGrid->GetRow();
			CString sTitleOk = pGrid->GetTextMatrix( iRow, 1 );
			
			if ( sTitleOk == _T("����") ) {
				CString sTitle = pGrid->GetTextMatrix( iRow, 3 );
				if ( sTitle != _T("") ) {
					CString str;
					this->GetManageValue(_T("����"), _T("����"), _T("�ڵ��������缳��"), _T("����"), str);
					if ( str == _T("Y") ) {
						OnbBOKBONSEARCH();
						return true;
					}
				}
			}
			
		}
	}
EFS_END
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_CAT_PROC_0301::OnbCatInpHotKeyList() 
{
	// TODO: Add your control notification handler code here
	
}
HBRUSH CBO_CAT_PROC_0301::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
