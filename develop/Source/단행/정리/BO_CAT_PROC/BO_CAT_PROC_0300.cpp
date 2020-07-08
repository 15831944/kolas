// BO_CAT_PROC_0300.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"

#include "BO_CAT_PROC_0301_01.h"
#include "BO_CAT_PROC_0301_ROLE.h"
#include "BO_CAT_PROC_0320.h"
#include "BO_CAT_PROC_0330.h"
#include "BO_CAT_PROC_0340.h"
#include "BO_CAT_PROC_0350.h"
#include "BO_CAT_PROC_0373.h"
#include "BO_CAT_PROC_0380.h"
#include "BO_CAT_PROC_AcqNumber.h"
#include "BO_CAT_PROC_Api.h"

#include "..\..\..\����\�з���ȣ\MNG_CLASS_SIGN_POPUP\MNG_CLASS_SIGN_POPUP_01.h"
#include "..\..\..\����\IndexToolKit\IndexTool.h"

#include "..\..\..\����\�˻�\BusinessPopupSearcher\BusinessPopupSearcherDlg.h"
#include "..\..\..\����\BO_VOLUME_FORM\BOVolumeDlg.h"

#include "..\BO_CAT_NUMBER\BO_CAT_ACQUISIT_NO.h"
#include "..\BO_CAT_PROC_KEYWORD\BO_CAT_PROC_KEYWORD_0000.h"
#include "..\..\..\����\�谡���ذ���\MNG_CODE_SHELF\MNG_CODE_SHELF_01.h"
#include "..\..\..\����\�谡���ذ���\ShelfCodeGetter\ShelfCodeGetterDlg.h"
#include "..\..\..\����\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"
#include "..\..\..\����\����\CatCodeGetter\CatCodeGetterDlg.h"

#include "..\..\..\����\BO_MARC_IMPORT\MARC_IMPORT.h"

#include "..\..\..\�������\Light_GatherDataSearch\UC_MARC_IMPORT.h"

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

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0300 dialog


CBO_CAT_PROC_0300::CBO_CAT_PROC_0300(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0300)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
	iBookSw = 0;
	m_bInitDlg = TRUE;
}

CBO_CAT_PROC_0300::~CBO_CAT_PROC_0300()
{
}

INT CBO_CAT_PROC_0300::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList(m_sCmAlias, m_sGridAlias);
		idx = SelectGetHeadPosition(m_sCmAlias, m_sGridAlias);
		if (idx < 0)
		{
			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
			EndDialog(IDCANCEL);
			return -1;
		}
		break;
	case -1:
		idx = SelectGetPrev(m_sCmAlias, m_sGridAlias);
		GetDlgItem(IDC_bCatNEXT)->EnableWindow(true);
		if (idx == -1) {
			idx = SelectGetHeadPosition(m_sCmAlias, m_sGridAlias);
			GetDlgItem(IDC_bCatPREV)->EnableWindow(false);
			return -1;
		}
		break;
	case 1:
		idx = SelectGetNext(m_sCmAlias, m_sGridAlias);
		GetDlgItem(IDC_bCatPREV)->EnableWindow(true);
		if (idx == -1) {
			idx = SelectGetTailPosition(m_sCmAlias, m_sGridAlias);
			GetDlgItem(IDC_bCatNEXT)->EnableWindow(false);

			return -1;
		}
		break;
	}

	m_idx = idx; 
	AllControlDisplay(_T("CM_BO_CAT_PROC_0300"), m_idx);

	SetReverse(m_sCmAlias, m_sGridAlias, m_idx);

EFS_END
	return 0;
}

VOID CBO_CAT_PROC_0300::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0300)	
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0300, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0300)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabPURCHASING, OnSelchangetabPURCHASING)
	ON_BN_CLICKED(IDC_bSTATIC, OnbSTATIC)
	ON_BN_CLICKED(IDC_bSTATIC3, OnbSTATIC3)
	ON_BN_CLICKED(IDC_bKEYWORD_CREAT, OnbKEYWORDCREAT)
	ON_BN_CLICKED(IDC_bCOL_NUMBER, OnbCOLNUMBER)
	ON_BN_CLICKED(IDC_bSUBJECT_REF3, OnbSUBJECTREF3)
	ON_BN_CLICKED(IDC_bBOOK_INPUT, OnbBOOKINPUT)
	ON_BN_CLICKED(IDC_bBUCHUL_ASSIST, OnbBUCHULASSIST)
	ON_BN_CLICKED(IDC_bCatPREV, OnbCatPREV)
	ON_BN_CLICKED(IDC_bCatNEXT, OnbCatNEXT)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_bBOKBON_SEARCH, OnbBOKBONSEARCH)
	ON_BN_CLICKED(IDC_bCOL_NUMBER2, OnbCOLNUMBER2)
	ON_BN_CLICKED(IDC_bSELECT2, OnbSELECT2)
	ON_BN_CLICKED(IDC_bREFERENCE, OnbREFERENCE)
	ON_BN_CLICKED(IDC_bINQUIRY, OnbINQUIRY)
	ON_BN_CLICKED(IDC_bSUBJECT_REF2, OnbSUBJECTREF2)
	ON_BN_CLICKED(IDC_bCOMMON_APPLY3, OnbCOMMONAPPLY3)
	ON_BN_CLICKED(IDC_bCOMMON_APPLY2, OnbCOMMONAPPLY2)
	ON_BN_CLICKED(IDC_bMarcImport, OnbMarcImport)
	ON_BN_CLICKED(IDC_bACCESSION_OBJ_IMPORT, OnbACCESSIONOBJIMPORT)
	ON_CBN_SELCHANGE(IDC_c0300FORM_CLASS, OnSelchangec0300FORMCLASS)
	ON_BN_CLICKED(IDC_bCOMMON_APPLY, OnbCOMMONAPPLY)
	ON_NOTIFY(EN_KILLFOCUS, IDC_redt0300MarcView, OnKillfocusredt0300MarcView)
	ON_CBN_SELCHANGE(IDC_c0301MoneyCode, OnSelchangec0301MoneyCode)
	ON_CBN_DROPDOWN(IDC_c0300FORM_CLASS, OnDropdownc0300FORMCLASS)
	ON_NOTIFY(NM_SETFOCUS, IDC_redt0300MarcView, OnSetfocusredt0300MarcView)
	ON_BN_CLICKED(IDC_bCatHistory, OnbCatHistory)
	ON_NOTIFY(EN_SETFOCUS, IDC_redt0300MarcView, OnSetfocusredt0300MarcView)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 32015, OnCreate505Tag)
	ON_MESSAGE(WM_GRID_UPDATE, OnMarcGridUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0300 message handlers

BOOL CBO_CAT_PROC_0300::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
	return FALSE;
}

BOOL CBO_CAT_PROC_0300::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0300")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_DataMgr *pDM_DEL;
	pDM_DEL = FindDM(_T("DM_BO_CAT_PROC_0300_�ǻ���"));
	pDM_DEL->FreeData();

	INT iReturn = Display(0);

	if ( iReturn == -1 ) {
		m_bInitDlg = FALSE;

		return FALSE;
	}

	CTabCtrl	*m_tabPurchasing = (CTabCtrl*)GetDlgItem(IDC_tabPURCHASING);
	
	m_tabPurchasing->InsertItem(0, _T("�����Է�/��ũ��"));
	m_tabPurchasing->InsertItem(1, _T("�����Է�"));	
	m_tabPurchasing->InsertItem(2, _T("��ũ��"));

	m_marcEditor.SubclassDlgItem(IDC_redt0300MarcView, IDC_MSHFLEXGRID1, this);

	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}
	pDM->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	
	CString tmp1 = pDM->GetCellData( _T("MARC.�����1") , m_idx );
	tmp1 = pDM->GetCellData( _T("MARC.��ǥ��"), m_idx );	

	CString sDupYn;
	sDupYn = pDM->GetCellData( _T("BS_��������") , m_idx );
	if ( sDupYn == _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_��������"), _T("����"));
	}
	else {
		SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_��������"), _T("����"));
	}
	
	INT nRowIdx = 0;

	CString tmp=pDM->GetCellData( _T("BS_MARC") , m_idx );
	m_strStreamMarc=tmp;

	sRec_key = pDM->GetCellData( _T("BS_��KEY") , m_idx );
	m_sStatus = pDM->GetCellData( _T("BS_�ڷ����_�ڵ�"), m_idx );
	m_sStatus = pDM->GetCellData( _T("IBS_�۾�����INFO"), m_idx );	
	
	if ( sRec_key == _T("") ) {
		m_bInitDlg = FALSE;
		EndDialog(IDOK);
		return FALSE;
	}

	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	CString strStreamMarc = m_strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc, strStreamMarc);

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

	m_marcEditor.InitLimitTag();
	m_marcEditor.AddLimitTag(_T("008"));
	m_marcEditor.AddLimitTag(_T("007"));

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);	
	m_marcEditor.Display();

	TCHAR szLeader[25];
	m_marc.GetLeader(szLeader);

	CString sLeader;
	sLeader.Format( _T("%s") , szLeader );
	SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);

	CESL_DataMgr DM_SUB;
	DM_SUB.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString sQuery_sub = _T("SELECT REG_CODE, COUNT(REG_CODE) FROM BO_BOOK_TBL WHERE SPECIES_KEY = ") + sRec_key + _T(" AND WORKING_STATUS >= 'BOC111N' GROUP BY REG_CODE");
	DM_SUB.RestructDataManager(sQuery_sub);
	INT iRowCount = DM_SUB.GetRowCount();

	CString sRegCodeSpecies;
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sRegCode;
		DM_SUB.GetCellData( i, 0, sRegCode );
		if ( sRegCode == _T("") ) {
			continue;
		}
		CString sRegCount;
		DM_SUB.GetCellData( i, 1, sRegCount );
		if ( i == 0 ) {
			sRegCodeSpecies = sRegCodeSpecies + sRegCode + sRegCount;
		}
		else {
			sRegCodeSpecies = sRegCodeSpecies + _T("/") + sRegCode + sRegCount;
		}
	}
	SetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BB_��ϱ���_�ڵ�"), sRegCodeSpecies );

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0300"));
	pCM->AllControlDisplay(m_idx);	

	pCM = FindCM(m_sCmAlias);
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}
	CESL_Grid * pGrid_sub = NULL;

	pGrid_sub = (CESL_Grid*)pCM->FindControl(m_sGridAlias);
	INT iSelectCount = pGrid_sub->GetRows();
	for ( i = 1 ; i < iSelectCount ; i ++ ) {
		CString sCheck = pGrid_sub->GetTextMatrix( i, 1 );
		if ( sCheck == _T("V") ) {
			m_aCheckRow.Add(i);
		}
	}

	pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_redt0300MarcView);

	EnableThemeDialogTexture(GetSafeHwnd()); 
EFS_END
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_PROC_0300::OnbREADFILE() 
{

}

VOID CBO_CAT_PROC_0300::OnSelchangetabPURCHASING(NMHDR* pNMHDR, LRESULT* pResult) 
{	
EFS_BEGIN

	CTabCtrl	*pTab = (CTabCtrl*)GetDlgItem(IDC_tabPURCHASING);
	if (pTab == NULL) return;

	INT nSel = pTab->GetCurSel();

	if (nSel == 0)
	{
		GetDlgItem(IDC_redt0300MarcView)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MSHFLEXGRID1)->ShowWindow(SW_SHOW);

		CRect rect;
		GetDlgItem(IDC_redt0300MarcView)->GetWindowRect( rect );
		ScreenToClient(rect);		
		rect.left = 355;
		GetDlgItem(IDC_redt0300MarcView)->MoveWindow( rect );	

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
		GetDlgItem(IDC_redt0300MarcView)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MSHFLEXGRID1)->ShowWindow(SW_SHOW);
		m_marcEditor.ApplyEdit();

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
		GetDlgItem(IDC_redt0300MarcView)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MSHFLEXGRID1)->ShowWindow(SW_HIDE);

		CRect rect;
		GetDlgItem(IDC_redt0300MarcView)->GetWindowRect( rect );
		ScreenToClient(rect);		
		rect.left = 10;
		GetDlgItem(IDC_redt0300MarcView)->MoveWindow( rect );	
	}
	*pResult = 0;
EFS_END
}

VOID CBO_CAT_PROC_0300::OnbSTATIC() 
{
EFS_BEGIN
	CString sFormCode;
	GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_���ı���_�ڵ�"), sFormCode);

	m_marcEditor.ApplyEdit();
	if ( sFormCode == _T("BK") ) {
		CBO_CAT_PROC_0320_01 dlg(this, _T("DM_BO_CAT_PROC_0000"), m_marc);

		if( IDOK == dlg.DoModal() ) {

			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );

			m_marcEditor.InitLimitTag();

			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);
		}
	}
	else if ( sFormCode == _T("CF") ) {
		CBO_CAT_PROC_0321 dlg(this, _T("DM_BO_CAT_PROC_0000"), m_marc);

		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );

			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);
		}
	}
	else if ( sFormCode == _T("MP") ) {
		CBO_CAT_PROC_0322 dlg(this, _T("DM_BO_CAT_PROC_0000"), m_marc);

		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );

			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);
		}
	}	
	else if ( sFormCode == _T("MU") ) {
		CBO_CAT_PROC_0323 dlg(this, _T("DM_BO_CAT_PROC_0000"), m_marc);
		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );

			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);
		}
	}
	else if ( sFormCode == _T("VM") ) {
		CBO_CAT_PROC_0324 dlg(this, _T("DM_BO_CAT_PROC_0000"), m_marc);
		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );

			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);
		}
	}
	else {

		CBO_CAT_PROC_0320_01 dlg(this, _T("DM_BO_CAT_PROC_0000"), m_marc);
		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );

			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);
		}
	}
EFS_END	
}

VOID CBO_CAT_PROC_0300::OnbSTATIC3() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	CMNG_CLASS_SIGN_POPUP_01 dlg( this );
	if( IDOK == dlg.DoModal() )
	{
		m_marcEditor.ApplyEdit();

		if ( dlg.m_KDCtag != _T("") && dlg.m_KDCtag != _T(".") ) {
			INT iLen = dlg.m_KDCtag.GetLength();
			if ( iLen > 1 ) {
				CString strItemData = dlg.m_KDCtag.Mid( 7 , dlg.m_KDCtag.GetLength()-8 );
				//m_pInfo->pMarcMgr->GetItem(&m_marc, _T("û����ȣ_�з�"), strItemData);
				m_pInfo->pMarcMgr->SetItem(&m_marc, _T("056$a"), strItemData , _T(""), ADD_SUBFIELD);
			}
		}

		if ( dlg.m_DDCtag != _T("") ) {
			INT iLen = dlg.m_DDCtag.GetLength();
			if ( iLen > 1 ) {
				CString sDDC = dlg.m_DDCtag.Mid( 7 , dlg.m_DDCtag.GetLength()-8 );
				m_pInfo->pMarcMgr->SetItem(&m_marc, _T("082$a"), sDDC , _T(""), ADD_SUBFIELD);
			}
		}

		m_pInfo->pMarcMgr->SortMarc( &m_marc );

		m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
		m_marcEditor.Display();
	}
EFS_END	
	
}


VOID CBO_CAT_PROC_0300::OnbKEYWORDCREAT() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here

	CBO_CAT_PROC_KEYWORD_0000 dlg(this, m_marc);
	if( IDOK == dlg.DoModal() ) {

		CString strStreamMarc = m_strStreamMarc;
		m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);	

		CString strTagCode, strErrMsg, strHelpMsg;
		
		if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
		{
			POSITION pos;
			pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
			while (pos)
			{
				m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
				UpdateData(FALSE);
			}	
		}	

		m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
		m_marcEditor.Display();

		TCHAR szLeader[25];
		m_marc.GetLeader(szLeader);

		CString sLeader;
		sLeader.Format( _T("%s") , szLeader );
		SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);
	}

EFS_END	
}

VOID CBO_CAT_PROC_0300::OnbCOLNUMBER() 
{
EFS_BEGIN

	CString sCallNo;
	CString sClassNo;
	INT iSwCallNoUpdate = 0;
	m_pInfo->pMarcMgr->GetField( &m_marc, _T("090"), sCallNo );
	if ( sCallNo != _T("") ) {
		iSwCallNoUpdate = 1;
		m_pInfo->pMarcMgr->GetItem( &m_marc, _T("090$a"), sClassNo );
	}
	m_marcEditor.ApplyEdit();

	CString strItemData;	
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("090$a"), strItemData);
	if ( strItemData != _T("") ) {
		AfxMessageBox( _T("MARC�� û����ȣ�� �����մϴ�.") );
		return;
	}	
	
	CString sCss;
	GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("�з�ǥ����"), sCss );
	sCss.Replace( _T(" "), NULL );
	if ( sCss == _T("") ) {
		AfxMessageBox( _T("�з�ǥ������ �����ϼ���.") );
		return;
	}
	else if ( sCss == _T("1") ) {
		m_pInfo->pMarcMgr->GetItem(&m_marc, _T("056$a"), strItemData);	
		if ( strItemData == _T("") ) {
			AfxMessageBox( _T("�з�ǥ���п� �´� �з���ȣ�� �����ϴ�.") );
			return;
		}
	}
	else if ( sCss == _T("2") ) {
		m_pInfo->pMarcMgr->GetItem(&m_marc, _T("082$a"), strItemData);	
		if ( strItemData == _T("") ) {
			AfxMessageBox( _T("�з�ǥ���п� �´� �з���ȣ�� �����ϴ�.") );
			return;
		}
	}
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("090$a"), strItemData);

	CESL_DataMgr *pDmAuthorRule;
	pDmAuthorRule = FindDM( _T("DM_BO_CAT_PROC_0300_AUTHOR_RULE") );
	if ( pDmAuthorRule == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_AUTHOR_RULE error") );
		return;
	}
	CString sFormCode;
	GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_���ı���_�ڵ�"), sFormCode );
	CString sSubRegCode;
	GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_������ϱ���_�ڵ�"), sSubRegCode );
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
			m_pInfo->pMarcMgr->GetItem(&m_marc, sTagSub, strItemData);
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
			m_pInfo->pMarcMgr->GetItem(&m_marc, _T("245$a"), sTitle);
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

			m_pInfo->pMarcMgr->SetItem(&m_marc, _T("090$b"), strItemData);
		}
	}
	else if ( iSwCallNoUpdate == 1 ) {
		
		CString sShelfCode, sClassType, sAcqNo;
		CString sWhere;
		
		GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("�з�ǥ����"), sClassType );
		if ( sClassType != _T("") ) {
			sWhere = _T("CLASS_NO_TYPE = '") + sClassType + _T("'");
		}		
		if ( sClassNo != _T("") ) {
			if ( sWhere != _T("") ) {
				sWhere = sWhere + _T(" AND ");
			}
			sWhere = sWhere + _T("CLASS_NO = '") + sClassNo + _T("'");
		}
		m_pInfo->pMarcMgr->GetItem( &m_marc, _T("049$f"), sShelfCode );
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
		
		CBO_CAT_PROC_AcqNumber dlg(this);
		dlg.SetData( sShelfCode, sClassType, sClassNo, sAcqNumber,  _T("�� �ο�"));
		if ( dlg.DoModal() == IDOK ) {
			m_pInfo->pMarcMgr->SetItem( &m_marc, _T("090$b"), sAcqNumber );
		}
	}

	CArray<CString, CString&> pArray;
	CString sItemData;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$v"), sItemData, &pArray);
	
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
		m_pInfo->pMarcMgr->SetItem(&m_marc, _T("090$c"), sItemData);
	}

	m_pInfo->pMarcMgr->SortMarc( &m_marc );

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();

EFS_END	
}

VOID CBO_CAT_PROC_0300::OnbSUBJECTREF3() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	m_marcEditor.ApplyEdit();
	CBO_CAT_PROC_0330 dlg(this,_T(""));
	
	if( IDOK == dlg.DoModal() ) {
		CString sKdc;
		sKdc.Format ( _T("%c") , 30 );
		INT iLen = dlg.m_KDC.GetLength();
		if ( iLen > 1 ) {
			m_pInfo->pMarcMgr->SetItem(&m_marc, _T("056$a"), dlg.m_KDC , _T(""), ADD_SUBFIELD);
			m_pInfo->pMarcMgr->SortMarc( &m_marc );
		}
		iLen = dlg.m_DDC.GetLength();
		if ( iLen > 1 ) {
			m_pInfo->pMarcMgr->SetItem(&m_marc, _T("082$a"), dlg.m_DDC , _T(""), ADD_SUBFIELD);
			m_pInfo->pMarcMgr->SortMarc( &m_marc );
		}

		m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
		m_marcEditor.Display();

	}
EFS_END		

}

VOID CBO_CAT_PROC_0300::OnbBOOKINPUT() 
{	
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0300_å") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_å error") );
		return;
	}
	if ( iBookSw == 0 ) {
		CString strWhere = _T("species_key = '") + sRec_key + _T("' AND WORKING_STATUS >= 'BOC111N'");
		if ( pDM->RefreshDataManager(strWhere) < 0 ) return;
		iBookSw = 1;
	}

	CESL_DataMgr *pDmVol;
	pDmVol = FindDM( _T("DM_BO_CAT_PROC_0300_��") );
	if ( pDmVol == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_�� error") );
		return;
	}
	INT ids = pDmVol->FreeData();

	CBO_SPECIES API(this);
	API.CONVERT_BOOK_to_VOLUME( pDM, pDmVol );

	INT iVolCount;
	iVolCount = pDmVol->GetRowCount();


	if ( iVolCount == 1 ) {
		CArray<CString, CString&> pArray;
		CString strItemData;
		m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$c"), strItemData, &pArray);
		INT iCnt = pArray.GetSize();
		for ( INT i = 1 ; i <= iCnt ; i ++ ){
			CString sCopy = pArray.GetAt(i-1);
			pDM->SetCellData( _T("BB_������ȣ"), sCopy, i );		
		}

		CString sItemData = _T("");

		sItemData = _T("");
		ids = m_pInfo->pMarcMgr->GetItem( &m_marc, _T("245$p"), sItemData );
		if ( sItemData != _T("") ) {
			ids = pDM->SetCellData( _T("BB_������ηϸ�"), sItemData, 0 );
		}
		sItemData = _T("");
		ids = m_pInfo->pMarcMgr->GetItem( &m_marc, _T("300$e"), sItemData );
		if ( sItemData != _T("") ) {
			ids = pDM->SetCellData( _T("BB_�����ڷ�"), sItemData, 0 );
		}
		sItemData = _T("");
		ids = m_pInfo->pMarcMgr->GetItem( &m_marc, _T("950$c"), sItemData );
		if ( sItemData != _T("") ) {
			ids = pDM->SetCellData( _T("BB_����������"), sItemData, 0 );
		}
		sItemData = _T("");

		CString sPrice;
		CString sOriPrice;
		m_pInfo->pMarcMgr->GetItem( &m_marc, _T("950$b"), sPrice );
		for ( i = 0 ; i < sPrice.GetLength() ; i++ ) {
			CString sOneWord;
			sOneWord = sPrice.Mid( i, 1 );
			if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
				sOriPrice = sOriPrice + sOneWord;
			}
		}
		sItemData = sOriPrice;
		if ( sItemData != _T("") ) {
			ids = pDM->SetCellData( _T("BB_����"), sItemData, 0 );
		}
	}

	CString sCode;
	GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_��ü����_�ڵ�"), sCode);
	ids = pDM->SetCellData( _T("BB_��ü����_�ڵ�"), sCode, 0 );

	sCode = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_�̿����ѱ���_�ڵ�"), sCode);
	ids = pDM->SetCellData( _T("BB_�̿����ѱ���_�ڵ�"), sCode, 0 );

	sCode = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_��������_�ڵ�"), sCode);
	ids = pDM->SetCellData( _T("BB_��������_�ڵ�"), sCode, 0 );

	CBOVolumeDlg dlg(this, pDM );
	dlg.SetOpenMode(-2100);
	dlg.DoModal();

	m_marcEditor.ApplyEdit();
	
	API.SetOpenMode(-2100);
	
	CESL_DataMgr *pDM_SEND;
	pDM_SEND = FindDM( _T("DM_BO_CAT_PROC_0000") );
	if ( pDM_SEND == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}

	pDM = FindDM( _T("DM_BO_CAT_PROC_0300_å") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_å error") );
		return;
	}
	INT iBookCount = pDM->GetRowCount();

	CBO_CAT_PROC_Api FormApi(this);
	CArray<CRegCodeProc, CRegCodeProc> arrRegCode;	
	ids = FormApi.GetRegCodeInfo( pDM, &arrRegCode );
	if (ids < 0 || arrRegCode.GetSize() <= 0)
	{
		AfxMessageBox( _T("��ϱ��� �ʵ尡 �߸��Ǿ����ϴ�.") );
		return;
	}
	CString strRegCodeField = _T("");
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

	SetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BB_��ϱ���_�ڵ�"), strRegCodeField );

	pDmVol = FindDM( _T("DM_BO_CAT_PROC_0300_��") );
	if ( pDmVol == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_�� error") );
		return;
	}
	ids = pDmVol->FreeData();

	API.CONVERT_BOOK_to_VOLUME( pDM, pDmVol );

	iVolCount = pDmVol->GetRowCount();

	if ( iVolCount > 1 ) {
		ids = m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("020") );
		
		ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("245$n") );

		ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("245$p") );
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
			ids = pDmVol->GetCellData( _T("BB_����"), i, sItemData );
			CString sPrice = sItemData;
			if ( sItemData != _T("") ) {
				INT iPrice = _ttoi ( sItemData.GetBuffer(0) );
				iPriceSum = iPriceSum + iPrice;
			}

			ids = pDmVol->GetCellData( _T("BB_����ISBN"), i, sItemData );
			if ( sItemData != _T("") ) {
				sItemData.Replace( _T("-"), _T("") );
				if ( sPrice != _T("") ) {
					CString sMoneyCode;
					GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_ȭ�󱸺�"), sMoneyCode );
					sPrice = sMoneyCode + sPrice;	
				}
				if ( sPrice != _T("") ) {
					CString strTmp = _T("");
					strTmp.Format(_T("020  %ca%s%cc%s"),SUBFIELD_CODE, sItemData, SUBFIELD_CODE, sPrice);
					sItemData = strTmp;
				}
				else {
					CString strTmp = _T("");
					strTmp.Format(_T("020  %ca%s"), SUBFIELD_CODE, sItemData);
					sItemData = strTmp;
				}
				CString sEaAdd;
				ids = pDM->GetCellData( _T("BB_����ISBN�ΰ���ȣ"), i, sEaAdd );
				if ( sEaAdd != _T("") ) {
					CString strTmp = _T("");
					strTmp.Format(_T("%s%cg%s"), sItemData, SUBFIELD_CODE, sEaAdd);
					sItemData = strTmp;
				}
				CString sErrorIsbn = pDM->GetCellData( _T("BB_���ǿ���ISBN"), i );
				if ( sErrorIsbn != _T("") ) {
					CString strTmp = _T("");
					strTmp.Format(_T("%s%cz%s"), sItemData, SUBFIELD_CODE, sErrorIsbn);
					sItemData = strTmp;
				}
				ids = m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}
		}
		sPriceSum.Format( _T("%d"), iPriceSum );
		if ( sPriceSum != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("950$b") );

			CString sMoneyCode;
			GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_ȭ�󱸺�"), sMoneyCode );
			sPriceSum = sMoneyCode + sPriceSum;			

			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("950$b"), sPriceSum );		
		}
		ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("245$n"), sVolNo );
	}

	CString sIsbn;
	
	if ( iVolCount == 1 ) {

		CString sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_�����"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("245$n") );
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("245$n"), sItemData );
		}
		
		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_������ηϸ�"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("245$p") );
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("245$p"), sItemData );
		}		

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_������"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("245$d") );
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("245$d"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_�����"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("260$c") );
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("260$c"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_����ISBN"), 0, sItemData );
		if ( sItemData != _T("") ) {
			sItemData.Replace( _T("-") , _T(""));
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("020$a") );	
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("020$c") );
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("020$g") );
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("020$z") );
			CString sPrice;
			ids = m_pInfo->pMarcMgr->GetItem( &m_marc, _T("950$b"), sPrice );
			if ( sPrice != _T("") ) {
				CString strTmp=_T("");
				strTmp.Format(_T("020  %ca%s%cc%s"),SUBFIELD_CODE, sItemData, sPrice);
				sIsbn = strTmp;
			}
			else {
				CString strTmp=_T("");
				strTmp.Format(_T("020  %ca%s"),SUBFIELD_CODE, sItemData);
				sIsbn = strTmp;
			}
			CString sEaAdd;
			ids = pDM->GetCellData( _T("BB_����ISBN�ΰ���ȣ"), 0, sEaAdd );
			if ( sEaAdd != _T("") ) {
				CString strTmp=_T("");
				strTmp.Format(_T("%s%cg%s"), sIsbn, sEaAdd);
				sIsbn = strTmp;
			}
			CString sErrorIsbn = pDM->GetCellData( _T("BB_���ǿ���ISBN"), 0 );
			if ( sErrorIsbn != _T("") ) {
				CString strTmp=_T("");
				strTmp.Format(_T("%s%cz%s"), sIsbn, SUBFIELD_CODE, sErrorIsbn);
				sIsbn = strTmp;
			}
			
			ids = m_pInfo->pMarcMgr->InsertField( &m_marc, sIsbn );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_�����"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("�����") );
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("�����"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_������Ư��"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("������Ư��") );
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("������Ư��"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_ũ��"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("ũ��") );
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("ũ��"), sItemData );
		}

		
		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_�����ڷ�"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("300$e") );
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("300$e"), sItemData );
		}

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_����"), 0, sItemData );
		ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("950$b") );
		if ( sItemData != _T("") && sItemData != _T("0") ) {
			CString sMoneyCode;
			GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_ȭ�󱸺�"), sMoneyCode );
			sItemData = sMoneyCode + sItemData;
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("950$b"), sItemData );
		}		

		sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_����������"), 0, sItemData );
		if ( sItemData != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("950$c") );
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("950$c"), sItemData );
		}
	}

	CString sItemData = _T("");
	ids = pDM->GetCellData( _T("BB_��������_�ڵ�"), 0, sItemData );
	SetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_��������_�ڵ�"), sItemData );
	
	sItemData = _T("");
	ids = pDM->GetCellData( _T("BB_��ü����_�ڵ�"), 0, sItemData );	
	SetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_��ü����_�ڵ�"), sItemData );

	sItemData = _T("");
	ids = pDM->GetCellData( _T("BB_�̿����ѱ���_�ڵ�"), 0, sItemData );	
	SetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_�̿����ѱ���_�ڵ�"), sItemData );
	
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$l") );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$v") );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$c") );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$f") );
	CString sRegNo;
	CString sVolNoPre;
	INT iCopyCode = 1;
	CString sStartRegNo, sEndRegNo; 
	INT iSwRegNo = 0;
	CString sPreShelfCode;
	INT iSwShelfCode = 0;
	for ( i = 0 ; i < pDM->GetRowCount() ; i ++ ) {
		CString sFlag = pDM->GetCellData( _T("BB_DB_FLAG"), i );
		if ( sFlag == _T("D") ) {
			continue;
		}
		sRegNo = _T("");
		ids = pDM->GetCellData( _T("BB_��Ϲ�ȣ"), i, sRegNo );		
		if ( sRegNo != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem(&m_marc, _T("049$l"), sRegNo );
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$l"), sRegNo );
		}
		CString sItemData = _T("");
		ids = pDM->GetCellData( _T("BB_�����"), i, sItemData );
		if ( sItemData != _T("") && sRegNo != _T("") ) {

			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$v"), sItemData );
			if ( sVolNoPre == sItemData ) {
				iCopyCode++;
				CString sCopyCode;
				sCopyCode.Format( _T("%d"), iCopyCode );
				
				ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$c"), sCopyCode );
				ids = pDM->SetCellData( _T("BB_������ȣ"), sCopyCode, i );
				
			}
			else{
				iCopyCode = 1;
			}
		}
		sVolNoPre = sItemData;
			
		if ( sRegNo != _T("") ) {
			iSwRegNo++;
			if ( 1 == iSwRegNo ) {
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

		if ( sRegNo != _T("") ) {
			sRegNo = _T("");
			ids = pDM->GetCellData( _T("BB_��ġ��ȣ_�ڵ�"), i, sRegNo );
			if ( sRegNo != _T("") ) {
				ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$f"), sRegNo );
				ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$f"), sRegNo );
			}
			if ( (i != 0) && (sPreShelfCode != sRegNo) && sPreShelfCode != _T("") ) {
				CString s049field;
				m_pInfo->pMarcMgr->GetField( &m_marc, _T("049"), s049field );
				s049field = _T("1") + s049field.Mid(1);
				m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("049") );
				m_pInfo->pMarcMgr->InsertField( &m_marc, _T("049") + s049field );
				iSwShelfCode = 1;
			}
			sPreShelfCode = sRegNo;
		}
	}
	if ( iSwShelfCode == 0 ) {
		CString s049field;
		m_pInfo->pMarcMgr->GetField( &m_marc, _T("049"), s049field );
		if ( s049field != _T("") ) {
			s049field = _T("0") + s049field.Mid(1);
			m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("049") );
			m_pInfo->pMarcMgr->InsertField( &m_marc, _T("049") + s049field );
		}
	}
	SetControlData( _T("CM_BO_CAT_PROC_0300"), _T("���۵�Ϲ�ȣ"), sStartRegNo );
	SetControlData( _T("CM_BO_CAT_PROC_0300"), _T("����Ϲ�ȣ"), sEndRegNo );

	m_pInfo->pMarcMgr->DeleteEmptySubfield( &m_marc );
	m_pInfo->pMarcMgr->DeleteEmptyField( &m_marc );
	m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);

	m_marcEditor.Display();
EFS_END
	return;
}

VOID CBO_CAT_PROC_0300::OnbBUCHULASSIST() 
{
EFS_BEGIN

	m_marcEditor.ApplyEdit();
	m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
	m_marcEditor.Display();
EFS_END
	
}

VOID CBO_CAT_PROC_0300::OnbCatPREV() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	INT ids = Display(-1);

	if ( ids < 0 ) {
		return;
	}

	CString sDupYn;
	sDupYn = pDM->GetCellData( _T("BS_��������") , m_idx );
	if ( sDupYn == _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_��������"), _T("����"));
	}
	else {
		SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_��������"), _T("����"));
	}


	iBookSw = 0;

	CString tmp=pDM->GetCellData( _T("BS_MARC") , m_idx );
	m_strStreamMarc=tmp;

	sRec_key = pDM->GetCellData( _T("BS_��KEY") , m_idx );
	m_sStatus = pDM->GetCellData( _T("BS_�ڷ����_�ڵ�"), m_idx );

	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);

			UpdateData(FALSE);
		}	
	}

	CString strStreamMarc = m_strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc, strStreamMarc);

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();

	TCHAR szLeader[25];
	m_marc.GetLeader(szLeader);

	CString sLeader;
	sLeader.Format( _T("%s") , szLeader );
	SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);

	CESL_DataMgr *pDM_DEL;
	pDM_DEL = FindDM(_T("DM_BO_CAT_PROC_0300_�ǻ���"));
	pDM_DEL->FreeData();

	CESL_DataMgr DM_SUB;
	DM_SUB.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString sQuery_sub = _T("SELECT REG_CODE, COUNT(REG_CODE) FROM BO_BOOK_TBL WHERE SPECIES_KEY = ") + sRec_key + _T(" AND WORKING_STATUS >= 'BOC111N' GROUP BY REG_CODE");
	DM_SUB.RestructDataManager(sQuery_sub);
	INT iRowCount = DM_SUB.GetRowCount();

	CString sRegCodeSpecies;
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sRegCode;
		DM_SUB.GetCellData( i, 0, sRegCode );
		if ( sRegCode == _T("") ) {
			continue;
		}
		CString sRegCount;
		DM_SUB.GetCellData( i, 1, sRegCount );
		if ( i == 0 ) {
			sRegCodeSpecies = sRegCodeSpecies + sRegCode + sRegCount;
		}
		else {
			sRegCodeSpecies = sRegCodeSpecies + _T("/") + sRegCode + sRegCount;
		}
	}
	SetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BB_��ϱ���_�ڵ�"), sRegCodeSpecies );
EFS_END
}

VOID CBO_CAT_PROC_0300::OnbCatNEXT() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	INT ids = Display(1);
	if ( ids < 0 ) {
		return;
	}

	CString sDupYn;
	sDupYn = pDM->GetCellData( _T("BS_��������") , m_idx );
	if ( sDupYn == _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_��������"), _T("����"));
	}
	else {
		SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_��������"), _T("����"));
	}


	iBookSw = 0;

	CString tmp=pDM->GetCellData( _T("BS_MARC") , m_idx );
	m_strStreamMarc=tmp;
	

	sRec_key = pDM->GetCellData( _T("BS_��KEY") , m_idx );
	m_sStatus = pDM->GetCellData( _T("BS_�ڷ����_�ڵ�"), m_idx );
	
	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	CString strStreamMarc = m_strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc, strStreamMarc);

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();

	TCHAR szLeader[25];
	m_marc.GetLeader(szLeader);

	CString sLeader;
	sLeader.Format( _T("%s") , szLeader );
	SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);

	CESL_DataMgr *pDM_DEL;
	pDM_DEL = FindDM(_T("DM_BO_CAT_PROC_0300_�ǻ���"));
	pDM_DEL->FreeData();
	
	CESL_DataMgr DM_SUB;
	DM_SUB.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString sQuery_sub = _T("SELECT REG_CODE, COUNT(REG_CODE) FROM BO_BOOK_TBL WHERE SPECIES_KEY = ") + sRec_key + _T(" AND WORKING_STATUS >= 'BOC111N' GROUP BY REG_CODE");
	DM_SUB.RestructDataManager(sQuery_sub);
	INT iRowCount = DM_SUB.GetRowCount();

	CString sRegCodeSpecies;
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sRegCode;
		DM_SUB.GetCellData( i, 0, sRegCode );
		if ( sRegCode == _T("") ) {
			continue;
		}
		CString sRegCount;
		DM_SUB.GetCellData( i, 1, sRegCount );
		if ( i == 0 ) {
			sRegCodeSpecies = sRegCodeSpecies + sRegCode + sRegCount;
		}
		else {
			sRegCodeSpecies = sRegCodeSpecies + _T("/") + sRegCode + sRegCount;
		}
	}
	SetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BB_��ϱ���_�ڵ�"), sRegCodeSpecies );
EFS_END
}

VOID CBO_CAT_PROC_0300::OnbtnCLOSE() 
{	
EFS_BEGIN
	

	EndDialog(IDOK);
EFS_END

}

VOID CBO_CAT_PROC_0300::OnbBOKBONSEARCH() 
{
EFS_BEGIN
	
	CString sTitle, sTitleTmp;
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("����"), sTitleTmp );
	CBusinessPopupSearcherDlg dlg( this );
	sTitle.Format( _T("\"%s\""), sTitleTmp );

	dlg.SetConditionInfo(2,sTitle);
	if( IDOK != dlg.DoModal() ) {
		return;
	}
	else {

		CString output, tmp;
		
		if ( dlg.m_cnt <= 1 ) {
			AfxMessageBox( _T("�˻��� �ڷᰡ �����ϴ�.") );
			return;
		}
		CString sRecKey = dlg.m_PK[0];
		if ( sRecKey == sRec_key ) {
			sRecKey = _T("");
		}
		for( INT i = 1; i < dlg.m_cnt; i++ )
		{			
			if ( sRec_key == dlg.m_PK[i] ) {
				continue;
			}
			if ( sRecKey != _T("") ) {
				sRecKey = sRecKey + _T(",") + dlg.m_PK[i];
			}
			else {
				sRecKey = dlg.m_PK[i];
			}
			tmp = dlg.m_PK[ i ];
			
			if( !( i % 20 ) )
				output = output + tmp + _T("\n");
			else
				output = output + tmp + _T(" ");
		}

		delete [] dlg.m_PK;

		m_cnt = dlg.m_cnt;
		CString * m_PK;
		m_PK = dlg.m_PK;

		CBO_CAT_PROC_0373 dlg_dup(this, m_idx, sRecKey);	
		if (IDOK != dlg_dup.DoModal()){
			
			if (dlg_dup.sChk==_T("")){
				SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_��������"), _T("����"));
			}
			else{
				SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_��������"), _T("����"));
			}
		}
			
	}
EFS_END
}

VOID CBO_CAT_PROC_0300::OnOK() 
{
EFS_BEGIN
	CString stmp=pDM->GetCellData( _T("BS_MARC") , m_idx );
	
	INT iCh = m_marcEditor.ApplyEdit();
	if ( iCh < 0 ) {
		return;
	}

	m_pInfo->pMarcMgr->DeleteEmptySubfield( &m_marc );
	m_pInfo->pMarcMgr->DeleteEmptyField( &m_marc );
	m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);

	CString sItem = _T("");
	
	CString sPrice;
	CString sOriPrice;
	CString sPriceCode;
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("950$b"), sPrice );
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

	if ( m_sStatus < _T("BOC115N") ) {
		m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
		
		CTime time = CTime::GetCurrentTime();
		CString sTime;
		sTime.Format( _T("%04d%02d%02d%02d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
		sTime = _T("005") + sTime;
		m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("005") );	
		m_pInfo->pMarcMgr->InsertField( &m_marc, sTime );

		m_marcEditor.Display();

		CString sMngCode;
		GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_��������_�ڵ�"), sMngCode );
		CString str;
		CString sQuery = _T("SELECT VALUE FROM MN_LIBINFO_TBL WHERE VALUE_NAME ='��������ȣ'");
		pDM->GetOneValueQuery( sQuery , str );
		
		if ( str != _T("") ) {

			CString s040a, s040c;
			m_pInfo->pMarcMgr->GetItem( &m_marc, _T("040$a"), s040a );
			m_pInfo->pMarcMgr->GetItem( &m_marc, _T("040$c"), s040c );
			if ( s040a == _T("") ) {
				m_pInfo->pMarcMgr->SetItem( &m_marc, _T("040$a"), str );
				m_pInfo->pMarcMgr->SetItem( &m_marc, _T("040$c"), str );
			}
			else if ( s040a != str || s040c != str) {
				m_pInfo->pMarcMgr->SetItem( &m_marc, _T("040$d"), str );
			}
			
		}
		else {
			AfxMessageBox( _T("������ ��ȣ�� �����ϴ�.") );
		}

		CBO_CAT_PROC_KEYWORD_0000 dlg(this, m_marc);	
		if( IDOK == dlg.DoModal() ) {
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);
		
			CString strTagCode, strErrMsg, strHelpMsg;		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
					UpdateData(FALSE);
				}	
			}	
			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);

			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);
		}
	}

	stmp=pDM->GetCellData( _T("BS_MARC") , m_idx );
	CESL_DataMgr *pDM_0000;	
 	pDM_0000 = FindDM(_T("DM_BO_CAT_PROC_0380"));
	if ( pDM_0000 == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0380 error") );
		return;
	}
	
	stmp=pDM->GetCellData( _T("BS_MARC") , m_idx );

	CString sCss;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("090$a"), sCss);
	if ( sCss == _T("") ) {

	}

	if ( sCss != _T("") ) {
		CString sBookCode;
		m_pInfo->pMarcMgr->GetItem(&m_marc, _T("090$b"), sBookCode );
		CString strWhere;	

		if ( sBookCode != _T("") ) {
			strWhere.Format( _T("I.REC_KEY = S.REC_KEY AND I.CLASS_NO = '%s' AND I.BOOK_CODE = '%s'"), sCss, sBookCode );
		}
		else {
			strWhere.Format( _T("I.REC_KEY = S.REC_KEY AND I.CLASS_NO = '%s'"), sCss );
		}

		CString sCallSetting;
		this->GetManageValue( _T("����"), _T("����"), _T("û����ȣŰ����"), _T("��å��ȣ"), sCallSetting );
		
		if ( sCallSetting == _T("Y") ) {
			CString sVolCode;
			m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$v"), sVolCode );
			if ( sVolCode != _T("") ) {
				strWhere = strWhere + _T(" AND I.VOL_CODE = '") + sVolCode + _T("'");
			}
		}
		sCallSetting = _T("");
		this->GetManageValue( _T("����"), _T("����"), _T("û����ȣŰ����"), _T("��ġ��ȣ"), sCallSetting );
		
		if ( sCallSetting == _T("Y") ) {
			CString sShelfCode;
			m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$f"), sShelfCode );
			if ( sShelfCode != _T("") ) {
				strWhere = strWhere + _T(" AND I.SE_SHELF_CODE = '") + sShelfCode + _T("'");
			}
		}

		if (pDM_0000->RefreshDataManager(strWhere) < 0)	return;
		INT iRowCount = pDM_0000->GetRowCount();	

		if ( iRowCount > 1 ) {
			CBO_CAT_PROC_0380 dlg(this);
			dlg.DoModal();
		}
		else { 

		}
	}

	CString sMessage;
	
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( &m_marc , _T("008") , sItem );
	if ( sItem == _T("") ) {
		sMessage = _T("008, ");
	}
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( &m_marc , _T("090") , sItem );
	if ( sItem == _T("") ) {
		sMessage = sMessage + _T("090, ");
	}
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( &m_marc , _T("049") , sItem );
	if ( sItem == _T("") ) {
		sMessage = sMessage + _T("049, ");
	}
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( &m_marc , _T("056") , sItem );
	if ( sItem == _T("") ) {
		sMessage = sMessage + _T("056, ");
	}
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( &m_marc , _T("245") , sItem );
	if ( sItem == _T("") ) {
		sMessage = sMessage + _T("245, ");
	}
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( &m_marc , _T("260") , sItem );
	if ( sItem == _T("") ) {
		sMessage = sMessage + _T("260, ");
	}
	sItem = _T("");
	m_pInfo->pMarcMgr->GetField( &m_marc , _T("300") , sItem );
	if ( sItem == _T("") ) {
		sMessage = sMessage + _T("300, ");
	}	

	INT iStatus = 0;
	sMessage = sMessage.Left(sMessage.GetLength()-2);
	if ( sMessage != _T("") ) {
		sMessage = sMessage + _T(" tag�� �Է����� �ʾҽ��ϴ�.");
		AfxMessageBox( sMessage );
	}
	else {
		iStatus = 1;	
	}

	m_pInfo->pMarcMgr->DeleteEmptyField( &m_marc );
	m_pInfo->pMarcMgr->DeleteEmptySubfield( &m_marc );
	m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);

	CString s008;
	m_pInfo->pMarcMgr->GetField( &m_marc , _T("008") , s008 );
	if ( s008 == _T("") ) {
		CString sDate;
		GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("���糯¥"), sDate );

		if ( sDate.IsEmpty() ) {
			AfxMessageBox( _T("���糯¥�� �����ϴ�.") );
			return;
		}
		CString sInputDate;
		sInputDate = sDate.Mid( 2, 2 ) + sDate.Mid( 5, 2 ) + sDate.Mid( 8, 2 );
		m_pInfo->pMarcMgr->SetItem( &m_marc, _T("008@0-5"), sInputDate );
		m_pInfo->pMarcMgr->SetItem( &m_marc, _T("008@6"), _T("s") );
		m_pInfo->pMarcMgr->SetItem( &m_marc, _T("008@7-10"), sDate.Mid( 0, 4 ) );		
		m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@38-39"), _T("  ") );
		
		CString s260Year;
		m_pInfo->pMarcMgr->GetItem( &m_marc, _T("260$c"), s260Year );
		if ( s260Year != _T("") ) {	
			m_pInfo->pMarcMgr->SetItem( &m_marc, _T("008@7-10"), s260Year );
		}		

		CString sLeader;
		m_marc.GetLeader( sLeader );
		CString sTmp;
		sTmp = sLeader.Left(5) + _T("nam") + sLeader.Right(16);
		m_marc.SetLeader( sTmp.GetBuffer(0) );
		
		SetControlData(_T("CM_BO_CAT_PROC_0301"), _T("����"), sTmp);
		
		m_marcEditor.Display();
	}

	CBO_SPECIES API(this);
	API.SetOpenMode(-2100);
	
	CESL_DataMgr *pDM_SEND;
	pDM_SEND = FindDM( _T("DM_BO_CAT_PROC_0000") );
	if ( pDM_SEND == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}


	CESL_DataMgr *pDM_BOOK = FindDM( _T("DM_BO_CAT_PROC_0300_å") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_å error") );
		return;
	}
	INT iBookCount = pDM_BOOK->GetRowCount();
	if ( iBookCount == 0 ) {
		CString strWhere = _T("species_key = '") + sRec_key + _T("'");
		if ( pDM_BOOK->RefreshDataManager(strWhere) < 0 ) return;
		
	}

	CESL_DataMgr *pDmVol;
	pDmVol = FindDM( _T("DM_BO_CAT_PROC_0300_��") );
	if ( pDmVol == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_�� error") );
		return;
	}
	INT ids = pDmVol->FreeData();

	API.CONVERT_BOOK_to_VOLUME( pDM_BOOK, pDmVol );

	INT iVolCount;
	iVolCount = pDmVol->GetRowCount();


	ids = pDM_SEND->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return;
	}



	CString sRegCodePre;
	CString sAllNum;
	INT iDeleteBookCount = 0;
	iBookCount = pDM_BOOK->GetRowCount();

	for ( INT i = 0 ; i < iBookCount ; i++ ) {		
		CString sBookKey = pDM_BOOK->GetCellData( _T("BB_åKEY"), i );
		if ( sBookKey == _T("") ) {
			
		}
		CString sBookFlag;
		pDM_BOOK->GetCellData( _T("BB_å�η��÷���"), i, sBookFlag );
		if ( sBookFlag == _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_å�η��÷���"), _T("B"), i );
			CString sDbFlag = pDM_BOOK->GetCellData( _T("BB_DB_FLAG"), i );
			if ( sDbFlag == _T("") ) {			
				ids = pDM_BOOK->SetCellData( _T("BB_DB_FLAG"), _T("U") , i );
			}
		}
		
		sItem = _T("");
		m_pInfo->pMarcMgr->GetItem( &m_marc , _T("056$a") , sItem );
		if ( sItem == _T("") ) {
			m_pInfo->pMarcMgr->GetItem( &m_marc , _T("082$a") , sItem );
		}
		if ( sItem != _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_�з���ȣ"), sItem, i );
			CString sDbFlag = pDM_BOOK->GetCellData( _T("BB_DB_FLAG"), i );
			if ( sDbFlag == _T("") ) {			
				ids = pDM_BOOK->SetCellData( _T("BB_DB_FLAG"), _T("U") , i );
			}
		}

		sItem = _T("");
		m_pInfo->pMarcMgr->GetItem( &m_marc , _T("090$b") , sItem );
		if ( sItem != _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_������ȣ"), sItem, i );
			CString sDbFlag = pDM_BOOK->GetCellData( _T("BB_DB_FLAG"), i );
			if ( sDbFlag == _T("") ) {
				ids = pDM_BOOK->SetCellData( _T("BB_DB_FLAG"), _T("U") , i );
			}
		}

		CString sWorkStatus = pDM_BOOK->GetCellData( _T("BB_�ڷ����"), i );
		if ( sWorkStatus < _T("BOC111N") && sWorkStatus != _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_DB_FLAG"), _T(""), i );
		}

		CString sDbFlagInsert = pDM_BOOK->GetCellData( _T("BB_DB_FLAG"), i );
		if ( sDbFlagInsert == _T("I") ) {
			pDM_BOOK->SetCellData( _T("BB_�ڷ����"), _T("BOC113N"), i );
		}

	}

	API.MakeBookQueryFrame(pDM_BOOK, sRec_key, _T("0"), pDM_SEND );
	
	CString MAT_CODE, FORM_CODE, MEDIA_CODE, SUB_REG_CODE, USE_OBJ_CODE, USE_LIMIT_CODE, MANAGE_CODE, CONTROL_MAT_CODE;	
	CString CLASS_NO_TYPE;
	iCh = GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_�ڷᱸ��_�ڵ�"), MAT_CODE);	
	iCh = GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_���ı���_�ڵ�"), FORM_CODE);		
	iCh = GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_��ü����_�ڵ�"), MEDIA_CODE);	
	iCh = GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_������ϱ���_�ڵ�"), SUB_REG_CODE);
	iCh = GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_�̿��󱸺�_�ڵ�"), USE_OBJ_CODE);
	iCh = GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_�̿����ѱ���_�ڵ�"), USE_LIMIT_CODE);
	iCh = GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("BS_��������_�ڵ�"), MANAGE_CODE);
	iCh = GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("�����ڷᱸ��"), CONTROL_MAT_CODE);
	iCh = GetControlData(_T("CM_BO_CAT_PROC_0300"), _T("�з�ǥ����"), CLASS_NO_TYPE);
	
	pDM->SetCellData( _T("IBS_�ڷᱸ��_�ڵ�") , MAT_CODE, m_idx );
	pDM->SetCellData( _T("IBS_���ı���_�ڵ�") , FORM_CODE, m_idx );
	pDM->SetCellData( _T("IBS_��ü����_�ڵ�") , MEDIA_CODE, m_idx );
	pDM->SetCellData( _T("IBS_������ϱ���_�ڵ�") , SUB_REG_CODE, m_idx );
	pDM->SetCellData( _T("IBS_�̿��󱸺�_�ڵ�") , USE_OBJ_CODE, m_idx );
	pDM->SetCellData( _T("IBS_�̿����ѱ���_�ڵ�") , USE_LIMIT_CODE, m_idx );
	pDM->SetCellData( _T("IBS_��������_�ڵ�") , MANAGE_CODE, m_idx );
	pDM->SetCellData( _T("IBS_�����ڷᱸ��_�ڵ�") , CONTROL_MAT_CODE, m_idx );
	pDM->SetCellData( _T("IBS_�з�ǥ����_�ڵ�") , CLASS_NO_TYPE, m_idx );

	MAT_CODE.Replace ( _T(" ") , NULL );
	FORM_CODE.Replace ( _T(" ") , NULL );
	MEDIA_CODE.Replace ( _T(" ") , NULL );
	SUB_REG_CODE.Replace ( _T(" ") , NULL );
	USE_OBJ_CODE.Replace ( _T(" ") , NULL );
	USE_LIMIT_CODE.Replace ( _T(" ") , NULL );
	MANAGE_CODE.Replace ( _T(" ") , NULL );
	CONTROL_MAT_CODE.Replace( _T(" ") , NULL );

	CIndexTool indexTool;
	indexTool.Init(m_pInfo->pMarcMgr, &m_marc);

	CString strIndexItem;
	
	ids = indexTool.GetIndex( _T("��������") , strIndexItem );

	CString strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);
	
	pDM_SEND = FindDM(_T("DM_BO_CAT_PROC_0000"));

	CString sQuery;
	INT ch;

	pDM->SetCellData( _T("BS_MARC") , m_strStreamMarc, m_idx );

	if ( iStatus == 1 ) {
		sQuery.Format( _T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOC115N', LAST_WORK='%s' WHERE WORKING_STATUS >= 'BOC111N' AND WORKING_STATUS < 'BOC115N' AND REG_NO IS NOT NULL AND SPECIES_KEY = '%s';") , GetWorkLogMsg(_T("Marc����"),WORK_LOG), sRec_key );
		pDM_SEND->AddFrame( sQuery );
		if ( m_sStatus <= _T("BOC115N") ) {

			sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC='%s', WORKING_STATUS = 'BOC115N', MAT_CODE ='%s', FORM_CODE='%s', MEDIA_CODE ='%s', CONTROL_MAT_CODE='%s', LAST_WORK='%s' WHERE rec_key = '%s';") , m_strStreamMarc, MAT_CODE, FORM_CODE, MEDIA_CODE, CONTROL_MAT_CODE,GetWorkLogMsg(_T("Marc����"),WORK_LOG),  sRec_key );
			ch = pDM_SEND->AddFrame( sQuery );
			
			pDM->SetCellData( _T("BS_�ڷ����_�ڵ�") , _T("BOC115N"), m_idx );
			pDM->SetCellData( _T("IBS_�۾�����INFO") , _T("BOC115N"), m_idx );
		}
		else {
			sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC='%s', MAT_CODE ='%s', FORM_CODE='%s', MEDIA_CODE ='%s', CONTROL_MAT_CODE='%s', LAST_WORK='%s' WHERE rec_key = '%s';") , m_strStreamMarc, MAT_CODE, FORM_CODE, MEDIA_CODE, CONTROL_MAT_CODE, GetWorkLogMsg(_T("Marc����"),WORK_LOG), sRec_key );
			ch = pDM_SEND->AddFrame( sQuery );
		}		
	}
	else {
		sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC='%s', MAT_CODE ='%s', FORM_CODE='%s', MEDIA_CODE ='%s', CONTROL_MAT_CODE='%s', LAST_WORK='%s' WHERE rec_key = '%s';") , m_strStreamMarc, MAT_CODE, FORM_CODE, MEDIA_CODE, CONTROL_MAT_CODE,GetWorkLogMsg(_T("Marc����"),WORK_LOG), sRec_key );
		ch = pDM_SEND->AddFrame( sQuery );
	}

	CString TITLE, AUTHOR, PUBLISHER;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("����"), TITLE);
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("����"), AUTHOR);
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("������"), PUBLISHER);

	pDM->SetCellData( _T("BS_MARC") , m_strStreamMarc, m_idx );
	CString sTitle;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("245$a"), sTitle);	
	CString sTitleTagX;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("245$x"), sTitleTagX);
	if ( sTitleTagX != _T("") ) {
		sTitle = sTitle + _T("=") + sTitleTagX;
	}
	CString sTitleTagB;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("245$b"), sTitleTagB);
	if ( sTitleTagB != _T("") ) {
		sTitle = sTitle + _T(":") + sTitleTagB;
	}

	pDM->SetCellData( _T("IBS_��ǥ��"), sTitle, m_idx );

	CString sAuthor;
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("245$d"), sAuthor );
	CString sAuthorTagE;
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("245$e"), sAuthorTagE );
	if ( sAuthorTagE != _T("") ) {
		sAuthor = sAuthor + _T(";") + sAuthorTagE;
	}

	pDM->SetCellData( _T("IBS_������"), sAuthor, m_idx );
	
	CString sPublisher;
	CString strItemData;
	CArray<CString, CString&> pArray;	
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("260$b"), strItemData, &pArray );
	for ( i = 0 ; i < pArray.GetSize() ; i++ ) {
		if ( i == 0  ) {
			sPublisher = pArray.GetAt(i);
		}
		else {
			sPublisher = sPublisher + _T(":") + pArray.GetAt(i);
		}
	}

	pDM->SetCellData( _T("IBS_������"), sPublisher, m_idx );

	INT chk;

	pDM_SEND->InitDBFieldData();

	if ( iStatus == 1 ) {
		pDM->AddDBFieldData(_T("WORK_STATUS"), _T("STRING"), _T("BOC115N") );		
	}
	
	pDM_SEND->AddDBFieldData(_T("CLASS_NO_TYPE"), _T("STRING"), CLASS_NO_TYPE);
	pDM_SEND->AddDBFieldData(_T("SUB_REG_CODE"), _T("STRING"), SUB_REG_CODE);
	pDM_SEND->AddDBFieldData(_T("MAT_CODE"), _T("STRING"), MAT_CODE);
	pDM_SEND->AddDBFieldData(_T("FORM_CODE"), _T("STRING"), FORM_CODE);
	pDM_SEND->AddDBFieldData(_T("MEDIA_CODE"), _T("STRING"), MEDIA_CODE);
	pDM_SEND->AddDBFieldData(_T("USE_OBJ_CODE"), _T("STRING"), USE_OBJ_CODE);
	pDM_SEND->AddDBFieldData(_T("USE_LIMIT_CODE"), _T("STRING"), USE_LIMIT_CODE);
	pDM_SEND->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), MANAGE_CODE);
	pDM_SEND->AddDBFieldData(_T("CONTROL_MAT_CODE"), _T("STRING"), CONTROL_MAT_CODE);

	CString sIdxAllItem;
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��ǥ��") , strIndexItem );	
	pDM_SEND->AddDBFieldData(_T("IDX_TITLE"), _T("STRING"), strIndexItem);
	
	pDM_SEND->AddDBFieldData(_T("TITLE"), _T("STRING"), sTitle);
	sIdxAllItem = strIndexItem;

	strIndexItem = _T("");
	
	chk = indexTool.GetIndex( _T("Ű��������") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("IDX_KEYWORD"), _T("STRING"), strIndexItem);	
	sIdxAllItem = sIdxAllItem + _T(" ") + strIndexItem;

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("������") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("IDX_AUTHOR"), _T("STRING"), strIndexItem);
	pDM_SEND->AddDBFieldData(_T("AUTHOR"), _T("STRING"), sAuthor);
	sIdxAllItem = sIdxAllItem + _T(" ") + strIndexItem;

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("������") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("IDX_PUBLISHER"), _T("STRING"), strIndexItem);
	pDM_SEND->AddDBFieldData(_T("PUBLISHER"), _T("STRING"), sPublisher);
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
	pDM_SEND->AddDBFieldData(_T("IDX_ALL_ITEM"), _T("STRING"), sIdxAllItem);


	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�����˻��������") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("IDX_IPUBLISHER"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_ISBN") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("ST_CODE"), _T("STRING"), strIndexItem);
	
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_ISSN") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("ST_ISSN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_STRN") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("ST_STRN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_RNSTRN") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("ST_RNSTRN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_����������ȣ") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("ST_CBN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_���۱ǵ�Ϲ�ȣ") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("ST_CAN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��Ÿ��ȣ_û����ȣ_��ġ��ȣ") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("SE_SHELF_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��Ÿ��ȣ_û����ȣ_�з���ȣ") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("CLASS_NO"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��Ÿ��ȣ_û����ȣ_������ȣ") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("BOOK_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��Ÿ��ȣ_û����ȣ_��å��ȣ") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("VOL_CODE_DISP"), _T("STRING"), strIndexItem);	//_T("��Ÿ��ȣ_û����ȣ_��å��ȣ_DISP")
	pDM_SEND->AddDBFieldData(_T("VOL_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��Ÿ��ȣ_�����ȣ") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("CONTROL_NO"), _T("STRING"), strIndexItem);
	
	
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("KDCP") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("KDCP_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("KDC") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("KDC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("DDC") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("DDC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("CEC") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("CEC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("CWC") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("CWC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("COC") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("COC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("UDC") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("UDC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("NDC") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("NDC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("LC") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("LC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�ѱ����к�ȣ") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("UNI_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("���α����ȣ") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("GOV_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�������а�") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("DEG_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("����") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("EDIT"), _T("STRING"), strIndexItem);
	
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�����˻������⵵") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("IDX_IPUB_YEAR"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("����⵵") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("PUB_YEAR"), _T("STRING"), strIndexItem);
	pDM_SEND->SetCellData( _T("IBS_����⵵"), strIndexItem, m_idx );

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�������") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("TEXT_LANG"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("��๮���") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("SUM_LANG"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("ǥ������") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("TITLE_INFO"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�����ڻ���") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("AUTHOR_INFO"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("�����ڻ���") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("PUB_INFO"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("����⵵����") , strIndexItem );
	pDM_SEND->AddDBFieldData(_T("PUB_YEAR_INFO"), _T("STRING"), strIndexItem);

	chk = pDM_SEND->MakeUpdateFrame( _T("IDX_BO_TBL"), _T("REC_KEY"), _T("STRING"), sRec_key );

	chk = pDM_SEND->SendFrame();
	if ( chk < 0 ) {
		AfxMessageBox( _T("SendFrame Ȯ��") );
		return;
	}

	chk = pDM_SEND->EndFrame();
	
	AfxMessageBox( _T("�����Ͽ����ϴ�.") );

	CString sStartRegNo;
	GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("���۵�Ϲ�ȣ"), sStartRegNo );
	pDM->SetCellData( _T("���۵�Ϲ�ȣ"), sStartRegNo, m_idx );
	
	CESL_ControlMgr *pCM;	
	pCM = FindCM( m_sCmAlias );

	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0000 error") );
		return;
	}
	pCM->AllControlDisplay();

	ids = Display(1);
	if ( ids < 0 ) {
		return;
	}
	
	CESL_DataMgr *pDM_PROC_0000;
	pDM_PROC_0000 = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( pDM_PROC_0000 == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}	

	CString tmp=pDM_PROC_0000->GetCellData( _T("BS_MARC") , m_idx );
	m_strStreamMarc=tmp;
	sRec_key = pDM_PROC_0000->GetCellData( _T("BS_��KEY") , m_idx );	

	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	strStreamMarc = m_strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc, strStreamMarc);

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();

	TCHAR szLeader[25];
	m_marc.GetLeader(szLeader);

	CString sLeader;
	sLeader.Format( _T("%s") , szLeader );
	SetControlData(_T("CM_BO_CAT_PROC_0300"), _T("����"), sLeader);

	CESL_DataMgr *pDM_DEL;
	pDM_DEL = FindDM(_T("DM_BO_CAT_PROC_0300_�ǻ���"));
	pDM_DEL->FreeData();
	
	CESL_DataMgr DM_SUB;
	DM_SUB.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString sQuery_sub = _T("SELECT REG_CODE, COUNT(REG_CODE) FROM BO_BOOK_TBL WHERE SPECIES_KEY = ") + sRec_key + _T(" AND WORKING_STATUS >= 'BOC111N' GROUP BY REG_CODE");
	DM_SUB.RestructDataManager(sQuery_sub);
	INT iRowCount = DM_SUB.GetRowCount();

	CString sRegCodeSpecies;
	for ( i = 0 ; i < iRowCount ; i++ ) {
		CString sRegCode;
		DM_SUB.GetCellData( i, 0, sRegCode );
		if ( sRegCode == _T("") ) {
			continue;
		}
		CString sRegCount;
		DM_SUB.GetCellData( i, 1, sRegCount );
		if ( i == 0 ) {
			sRegCodeSpecies = sRegCodeSpecies + sRegCode + sRegCount;
		}
		else {
			sRegCodeSpecies = sRegCodeSpecies + _T("/") + sRegCode + sRegCount;
		}
	}
	SetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BB_��ϱ���_�ڵ�"), sRegCodeSpecies );
EFS_END

}

VOID CBO_CAT_PROC_0300::OnbCOLNUMBER2() 
{
EFS_BEGIN

	m_marcEditor.ApplyEdit();

	CESL_DataMgr *pDM;
 	pDM = FindDM(_T("DM_BO_CAT_PROC_0380"));
	
	CString sCss;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("090$a"), sCss);
	if ( sCss == _T("") ) {
		AfxMessageBox (_T("���� û����ȣ������ �Ͻʽÿ�."));
		return;
	}

	CString sBookCode;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("090$b"), sBookCode );

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

VOID CBO_CAT_PROC_0300::OnSelchangecFORMCLASS() 
{
EFS_BEGIN

	INT ids;
	ids = ::MessageBox( GetSafeHwnd() ,
                        _T("���ı����� �����ϸ� ������ MARC �ڷᰡ ���õ˴ϴ�. �����Ͻðڽ��ϱ� ?\n���ı��� �����Ŀ��� �˸��� ��ü���а��� �����ߴ��� Ȯ���Ͻñ� �ٶ��ϴ�."),
                        _T("���� Ȯ��") ,
                        MB_YESNO | MB_ICONQUESTION 
                      );
    if ( ids == IDYES ) {		
		m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("008") );		
		m_marcEditor.Display();		
	}
EFS_END
}	

VOID CBO_CAT_PROC_0300::OnbSELECT2() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	CreatePopUpButton( IDC_bSELECT2 );
EFS_END
}

INT CBO_CAT_PROC_0300::Create505tag()
{
EFS_BEGIN
	CESL_DataMgr *pDM_BOOK = FindDM( _T("DM_BO_CAT_PROC_0300_å") );
	if ( pDM_BOOK == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_å error") );
		return -1;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0300_��") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_�� error") );
		return -1;
	}
	INT ids = pDM->FreeData();

	CBO_SPECIES API(this);
	API.CONVERT_BOOK_to_VOLUME( pDM_BOOK, pDM );

	INT iRowCount;
	iRowCount = pDM->GetRowCount();
	if ( iRowCount >= 2 ) {
		m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("505") );
		CString sVolPre;
		for( INT i = 0 ; i < iRowCount ; i ++ ) {
			CString sVolTitle, sVol, sAuthor;
			sVolTitle = pDM->GetCellData( _T("BB_������ηϸ�"), i );
			sVol = pDM->GetCellData( _T("BB_�����"), i );
			sAuthor = pDM->GetCellData( _T("BB_������"), i );
			if ( sVol != sVolPre ) {
				if ( sVol != _T("") ) {
					m_pInfo->pMarcMgr->SetItem( &m_marc, _T("505$n"), sVol );
				}
				if ( sVolTitle != _T("") ) {
					m_pInfo->pMarcMgr->SetItem( &m_marc, _T("505$t"), sVolTitle );
				}
				if ( sAuthor != _T("") ) {
					if ( i < iRowCount -1 ) {
						sAuthor = sAuthor + _T(". -");
					}
					m_pInfo->pMarcMgr->SetItem( &m_marc, _T("505$d"), sAuthor );
				}				
			}
			sVolPre = sVol;
		}
		m_marcEditor.Display();
	}
EFS_END
	return 0;
}

VOID CBO_CAT_PROC_0300::CreatePopUpButton( UINT f_inButtonType )
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
		case IDC_bSELECT2:	// ����
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
EFS_END
	return;	
}

VOID CBO_CAT_PROC_0300::OnbREFERENCE() 
{
EFS_BEGIN

	CString sDupYn, sDupKey;
	sDupYn = pDM->GetCellData( _T("BS_��������") , m_idx );
	sDupKey = pDM->GetCellData( _T("BS_�����ڷ�KEY") , m_idx );
	if ( sDupYn == _T("V") ) {
	}
	else {
		sDupKey = pDM->GetCellData( _T("BS_�����ڷ�KEY") , m_idx );
		if ( sDupKey != _T("") ) {
		}
	}

	if ( sDupKey == _T("") ) {
		AfxMessageBox( _T("�����ڷᰡ �����ϴ�.") );
		return;
	}

	CBO_CAT_PROC_0301_01 dlg(this, sDupKey);	
	dlg.DoModal();
EFS_END
}

VOID CBO_CAT_PROC_0300::OnbINQUIRY() 
{
EFS_BEGIN
	
	CShelfCodeGetterDlg dlg(this);
	if ( dlg.DoModal() == IDOK ) {
		CString sCode = dlg.m_code;
		m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$f"), sCode );
		m_marcEditor.Display();
	}
EFS_END
}

VOID CBO_CAT_PROC_0300::OnbSUBJECTREF2() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	CBO_CAT_ACQUISIT_NO dlg(1, this);
	
	dlg.DoModal();
EFS_END
}

VOID CBO_CAT_PROC_0300::OnbCOMMONAPPLY3() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	CBO_CAT_PROC_0301_ROLE dlg(this);
	dlg.DoModal();
EFS_END
}

VOID CBO_CAT_PROC_0300::OnbCOMMONAPPLY2() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	CKOMarcGetterDlg dlg( this );

	dlg.InitSetProperty( 4, _T("") );
	dlg.DoModal();
	CString sCode = dlg.m_code;	
EFS_END
}

VOID CBO_CAT_PROC_0300::OnbMarcImport() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
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
			m_marc.Init();
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, &marcTmp);
			CString sItemData;
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("245"), sItemData );			
			if ( sItemData != _T("") ) {
				sItemData = _T("245") + sItemData;
				m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("250"), sItemData );			
			if ( sItemData != _T("") ) {
				sItemData = _T("250") + sItemData;
				m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("260"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("260") + sItemData;
				m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("300"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("300") + sItemData;
				m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("950"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("950") + sItemData;
				m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("020"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("020") + sItemData;
				m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}
		}
		else if ( iMode == 1 ) {
			m_marc.Init();
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, &m_marc);
			m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("001") );
			m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("049") );
		}
		else if ( iMode == 2 ) {
			m_marc.Init();
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, &m_marc);
			m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("001") );
			m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("049") );
			m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("505") );
		}

		m_marcEditor.Display();
	}
EFS_END
}

VOID CBO_CAT_PROC_0300::OnbACCESSIONOBJIMPORT() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	
	CUC_MARC_IMPORT dlg(this);
	if ( dlg.DoModal() == IDOK ) {
		
		INT iMode = dlg.m_check;
		CString sStreamMarc = dlg.sMarc;
		CMarc marcTmp;
		if ( iMode == 0 ) {			
			m_marc.Init();
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, &marcTmp);
			CString sItemData;
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("245"), sItemData );			
			if ( sItemData != _T("") ) {
				sItemData = _T("245") + sItemData;
				m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("250"), sItemData );			
			if ( sItemData != _T("") ) {
				sItemData = _T("250") + sItemData;
				m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("260"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("260") + sItemData;
				m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("300"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("300") + sItemData;
				m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("950"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("950") + sItemData;
				m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetField( &marcTmp, _T("020"), sItemData );
			if ( sItemData != _T("") ) {
				sItemData = _T("020") + sItemData;
				m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );
			}
		}
		else if ( iMode == 1 ) {
			m_marc.Init();
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, &m_marc);
			m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("001") );
			m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("049") );
		}
		else if ( iMode == 2 ) {
			m_marc.Init();
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, &m_marc);
			m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("001") );
			m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("049") );
			m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("505") );
		}

		m_marcEditor.Display();
		
	}
EFS_END
}

UINT CBO_CAT_PROC_0300::OnCreate505Tag(WPARAM wParam, LPARAM lParam)
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

VOID CBO_CAT_PROC_0300::OnSelchangec0300FORMCLASS() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	INT ids;
	ids = ::MessageBox( GetSafeHwnd() ,
                        _T("���ı����� �����ϸ� ������ MARC �ڷᰡ ���õ˴ϴ�. �����Ͻðڽ��ϱ� ?\n���ı��� �����Ŀ��� �˸��� ��ü���а��� �����ߴ��� Ȯ���Ͻñ� �ٶ��ϴ�."),
                        _T("���� Ȯ��") ,
                        MB_YESNO | MB_ICONQUESTION 
                      );
    if ( ids == IDYES ) {		
		m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("008") );
		m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("007") );
		m_marcEditor.Display();		
	}
	else {
		SetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_���ı���_�ڵ�"), m_sFormCode );
	}
EFS_END
}

UINT CBO_CAT_PROC_0300::OnMarcGridUpdate(WPARAM wParam, LPARAM lParam)
{
EFS_BEGIN

	CString s440a, s440v, s490a;
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("440$a"), s440a );
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("440$v"), s440v );
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("490$a"), s490a );

	if ( s440a != _T("") && s440v == _T("") ) {
		m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("440$a"), s440a );
		m_pInfo->pMarcMgr->SetItem( &m_marc, _T("490$a"), s440a );
	}
	else if ( s490a != _T("") && s440v != _T("") ) {
		m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("490$a"), s490a );
		m_pInfo->pMarcMgr->SetItem( &m_marc, _T("440$a"), s490a );
		m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("440$v"), s440v );
		m_pInfo->pMarcMgr->SetItem( &m_marc, _T("440$v"), s440v );
	}

	m_pInfo->pMarcMgr->DeleteEmptySubfield( &m_marc );
	m_pInfo->pMarcMgr->DeleteEmptyField( &m_marc );
EFS_END
	return 0;
}
VOID CBO_CAT_PROC_0300::OnbCOMMONAPPLY() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	CCatCodeGetterDlg dlg(this);
	dlg.DoModal();
EFS_END
}

VOID CBO_CAT_PROC_0300::SetAlias(CString sCmAlias, CString sGridAlias)
{
EFS_BEGIN
	m_sCmAlias = sCmAlias;
	m_sGridAlias = sGridAlias;
EFS_END
}

LRESULT CBO_CAT_PROC_0300::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
EFS_BEGIN
	if (message == WM_COMMAND)
	{
		if (EN_KILLFOCUS == HIWORD(wParam))
		{
			if (IDC_redt0300MarcView == LOWORD(wParam))
			{
				INT iModifyFlag = pRichEdit->GetModify();
				if ( iModifyFlag != 0 ) {
					m_marcEditor.ApplyEdit();
				}
			}
		}
		if (EN_SETFOCUS == HIWORD(wParam)){
			if (IDC_redt0300MarcView == LOWORD(wParam)){
				pRichEdit->SetModify(false);
			}
		}
	}
EFS_END
	return CDialog::WindowProc(message, wParam, lParam);
}

VOID CBO_CAT_PROC_0300::OnKillfocusredt0300MarcView(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
EFS_END
}

VOID CBO_CAT_PROC_0300::OnSelchangec0301MoneyCode() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	CString sPrice;
	CString sOriPrice;
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("950$b"), sPrice );
	for ( INT i = 0 ; i < sPrice.GetLength() ; i++ ) {
		CString sOneWord;
		sOneWord = sPrice.Mid( i, 1 );
		if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
			sOriPrice = sOriPrice + sOneWord;
		}
	}

	CString sPriceCode;
	GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_ȭ�󱸺�"), sPriceCode );
	sOriPrice = sPriceCode + sOriPrice;
	m_pInfo->pMarcMgr->SetItem( &m_marc, _T("950$b"), sOriPrice );
	m_marcEditor.Display();
EFS_END
}

VOID CBO_CAT_PROC_0300::OnDropdownc0300FORMCLASS() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_���ı���_�ڵ�"), m_sFormCode );
EFS_END
}

VOID CBO_CAT_PROC_0300::OnSetfocusredt0300MarcView(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	pRichEdit->SetModify(false);
	*pResult = 0;
EFS_END
}

VOID CBO_CAT_PROC_0300::OnbCatHistory() 
{
	// TODO: Add your control notification handler code here
	
}
HBRUSH CBO_CAT_PROC_0300::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
