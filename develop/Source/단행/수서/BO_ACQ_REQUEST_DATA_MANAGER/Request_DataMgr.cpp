// Request_DataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Request_DataMgr.h"

#include "../../../�������/LOAN_INFO_SHARE_MANAGER/LoanShareManager.h"
#include "../../../����/BO_ACQ_COMMON/RequestMailMgr.h"
#include "BO_ACQ_RequestDataMgr.h"
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WORK_LOG _T("��ġ�������ó��"), _T(__FILE__), __LINE__

/////////////////////////////////////////////////////////////////////////////
// CRequest_DataMgr dialog
CRequest_DataMgr::CRequest_DataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequest_DataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequest_DataMgr)
	//}}AFX_DATA_INIT
	m_idx = -1;
	m_firstidx = -1;
	m_lastidx = -1;
	m_nOpType	= OPERATION_TYPE_INVALID;
	m_bGridSelectYN = FALSE;

	m_strPriorityCatalogYN	= _T("");	
	
	m_pDM = NULL;
	m_pCM = NULL;
	m_pMainCM = NULL;
	m_pMainGrid = NULL;
	m_pNoteCtrl			= NULL;	
	m_pLoanMangeValue = NULL;

	m_pApplicant_LoanUser = NULL;	
	m_pApplicant_Class = NULL;
	m_pApplicant_Cource = NULL;
	m_pApplicant_Dept = NULL;
	
	pMakeSearchData = new CMakeSearchData(pParent); 

	m_pParent = pParent;
	
	m_bUserNoChange = FALSE;
}

CRequest_DataMgr::~CRequest_DataMgr()
{
	if(pMakeSearchData)
	{
		delete pMakeSearchData; 
		pMakeSearchData = NULL; 
	}
	if(m_pLoanMangeValue)
	{
		delete m_pLoanMangeValue;		
		m_pLoanMangeValue = NULL;
	}
	if(m_pNoteCtrl)
	{
		delete m_pNoteCtrl ;		
		m_pNoteCtrl  = NULL;
	}
	if(m_pApplicant_LoanUser)
	{
		delete m_pApplicant_LoanUser ;		
		m_pApplicant_LoanUser  = NULL;
	}
	if(m_pApplicant_Class)
	{
		delete m_pApplicant_Class ;		
		m_pApplicant_Class  = NULL;
	}
	if(m_pApplicant_Cource)
	{
		delete m_pApplicant_Cource ;		
		m_pApplicant_Cource  = NULL;
	}
	if(m_pApplicant_Dept)
	{
		delete m_pApplicant_Dept ;		
		m_pApplicant_Dept  = NULL;
	}
}

VOID CRequest_DataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequest_DataMgr)
	DDX_Control(pDX, IDC_cmbAPPLICANT, m_cmbApplicant);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequest_DataMgr, CDialog)
	//{{AFX_MSG_MAP(CRequest_DataMgr)
	ON_EN_KILLFOCUS(IDC_edtWANTED_TITLE, OnKillfocusedtWANTEDTITLE)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnREQUEST_PREV, OnbtnREQUESTPREV)
	ON_BN_CLICKED(IDC_btnREQUEST_NEXT, OnbtnREQUESTNEXT)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnREQ_TITLE_DUP_SEARCH, OnbtnREQTITLEDUPSEARCH)
	ON_BN_CLICKED(IDC_btnREQ_NEW_SAVE, OnbtnREQNEWSAVE)
	ON_EN_KILLFOCUS(IDC_edtNID, OnKillfocusedtNID)
	ON_EN_KILLFOCUS(IDC_edtREQUEST_PRICE, OnKillfocusedtREQUESTPRICE)
	ON_CBN_SELCHANGE(IDC_cmbAPPLICANT, OnSelchangecmbAPPLICANT)
	ON_BN_CLICKED(IDC_chkREMARK, OnchkREMARK)
	ON_BN_CLICKED(IDC_chkREMARK2, OnchkREMARK2)
	ON_MESSAGE(NOTE_CLICKED, OnNoteClicked)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_cmbLasifyClass, OnSelchangecmbLasifyClass)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CRequest_DataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rect;

	if( InitESL_Mgr(_T("K2UPGRADE_��ġ����ڷ�")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_��ġ����ڷ�"));	
	m_pDM = FindDM(_T("DM_��ġ����ڷ����"));	
	m_pMainCM = FindCM(_T("CM_��ġ����ڷ����"));	
	m_pMainGrid = (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));
	if( m_pCM == NULL || m_pDM == NULL || m_pMainCM == NULL || m_pMainGrid == NULL )
	{
		ESLAfxMessageBox(_T("[CM/DM_��ġ����ڷ�(����)]�� ������ �� �����ϴ�."));
		return FALSE;
	}
	
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), m_strPriorityCatalogYN);

	AddColorControl(IDC_edtWANTED_TITLE, _T(""), _T(""), 'E');

	m_pApplicant_LoanUser	= new CApplicant_LoanUser(this);
	m_pApplicant_Class = new CApplicant_Class(this);
	m_pApplicant_Cource = new CApplicant_Cource(this);
	m_pApplicant_Dept = new CApplicant_Dept(this);	

	GetWindowRect(m_rcWindow);

	ShowCodeResource();

	this->GetClientRect(rect);
	
	rect.top += 38;

	if( m_nOpType == SINSERT )	rect.bottom -= 290;
	else						rect.bottom -= 444;

	rect.left += 0;
	rect.right -= 0;

	
	m_pApplicant_Class->Create(this);
	m_pApplicant_Class->MoveWindow(rect);
	m_pApplicant_Class->ShowWindow(SW_HIDE);


	m_pApplicant_Cource->Create(this);
	m_pApplicant_Cource->MoveWindow(rect);
	m_pApplicant_Cource->ShowWindow(SW_HIDE);

	
	m_pApplicant_Dept->Create(this);
	m_pApplicant_Dept->MoveWindow(rect);
	m_pApplicant_Dept->ShowWindow(SW_HIDE);

	GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");
	m_pApplicant_LoanUser->m_sIsUnityLoanService = m_sIsUnityLoanService;	
	m_pApplicant_LoanUser->Create(this);

	m_pApplicant_LoanUser->SetUserNoChange(m_bUserNoChange);
	m_pApplicant_LoanUser->MoveWindow(rect);
	m_pApplicant_LoanUser->ShowWindow(SW_SHOW);

	m_cmbApplicant.ResetContent();
	m_cmbApplicant.InsertString( 0, _T("ȸ��") );
	m_cmbApplicant.InsertString( 1, _T("�μ�") );
	m_cmbApplicant.InsertString( 2, _T("�а�") );
	m_cmbApplicant.InsertString( 3, _T("��ó") );
	
	m_cmbApplicant.SetCurSel(0);

	if( m_nOpType == SINSERT )
	{
		m_pApplicant_Class->SetInitInputDate();
		m_pApplicant_Cource->SetInitInputDate();
		m_pApplicant_Dept->SetInitInputDate();
		m_pApplicant_LoanUser->SetInitInputDate();

		CButton* pBtn	= (CButton*)GetDlgItem(IDC_btnREQUEST_PREV);
		CButton* pBtn1	= (CButton*)GetDlgItem(IDC_btnREQUEST_NEXT);
		CButton* pBtn2	= (CButton*)GetDlgItem(IDC_btnREQ_NEW_SAVE);
		pBtn->ShowWindow(SW_HIDE);
		pBtn1->ShowWindow(SW_HIDE);
		pBtn2->ShowWindow(SW_HIDE);
		
		( (CButton*) GetDlgItem(IDC_chkREMARK))->ShowWindow(SW_HIDE);
		( (CButton*) GetDlgItem(IDC_chkREMARK2))->ShowWindow(SW_HIDE);

		( (CComboBox*) GetDlgItem(IDC_cmbFURNISH_STATUS))->ShowWindow(SW_HIDE);
		( (CComboBox*) GetDlgItem(IDC_CMB_CANCELREASON))->ShowWindow(SW_HIDE);
	}
	else
	{
		Display(0);				
	}
	( (CButton*) GetDlgItem (IDC_chkREMARK2) )->SetCheck(TRUE);
	( (CComboBox*) GetDlgItem(IDC_CMB_CANCELREASON) )->EnableWindow(FALSE);

	OnSelchangecmbLasifyClass();

	GetDlgItem(IDC_cmbAPPLICANT)->EnableWindow(FALSE);

	if(m_nOpType == SINSERT){
		m_pCM->SetControlMgrData(_T("å��"), _T("1"));
		m_pCM->SetControlMgrData(_T("����"), _T("0"));
	}
	( (CEdit*) GetDlgItem(IDC_edtEA_ISBN) )->SetLimitText(20);	
	( (CEdit*) GetDlgItem(IDC_richREQ_CANCEL_REASON) )->SetLimitText(200);	
	( (CEdit*) GetDlgItem(IDC_richRECOM_OPINION) )->SetLimitText(200);

	m_pCM->GetControlMgrData(_T("��ġ����ڷ����"), m_strFurnishStatus);	

	if(m_nOpType != SINSERT)
	{
		m_pApplicant_LoanUser->HideRequestCntControl();
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  
}

VOID CRequest_DataMgr::InitFirstLastIndex()
{
	INT ids = m_pMainGrid->SelectMakeList();
	ids = m_pMainGrid->SelectGetHeadPosition();
	if( ids < 0 )
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pDM->GetRowCount();
		m_firstidx = 0;
		m_lastidx = cnt-1;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_lastidx = m_pMainGrid->SelectGetTailPosition();
		m_firstidx = m_pMainGrid->SelectGetHeadPosition();
	}
}

VOID CRequest_DataMgr::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnREQUEST_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnREQUEST_NEXT)->EnableWindow(TRUE);

	if( m_firstidx >= idx || idx < 0 )		GetDlgItem(IDC_btnREQUEST_PREV)->EnableWindow(FALSE);
	if( m_lastidx <= idx || idx < 0 )		GetDlgItem(IDC_btnREQUEST_NEXT)->EnableWindow(FALSE);
}

VOID CRequest_DataMgr::Display(INT idx)
{
	CString strApplicant, strFurnishStatus, strSMSReceipYN, strReservationYN;

	switch( idx )
	{
	case 0:
		{
		InitFirstLastIndex();
		m_idx = GetFirstSelectIndex();
		ShowNextPrevButton(m_idx);
		}
		break;
	case -1:
		{
		m_idx = GetPrevSelectIndex();
		ShowNextPrevButton(m_idx);
		}
		break;
	case 1:
		{
		m_idx = GetNextSelectIndex();
		ShowNextPrevButton(m_idx);
		}
		break;
	}
	
	m_pCM->AllControlDisplay(m_idx);
	CString strDate, strTempDate, strTime;
	m_pDM->GetCellData(_T("��û����"), m_idx, strTempDate);
	m_pDM->GetCellData(_T("��û����"), m_idx, strTime);
	
	if( _T("Y") == m_strPriorityCatalogYN )
	{
		CString strPriority;
		CButton *b1 = (CButton*)GetDlgItem(IDC_chkPRIORITY_CATALOG);
		m_pDM->GetCellData(_T("�켱�����ڷ�"), m_idx, strPriority);
		if( strPriority == _T("Y") )	b1->SetCheck(1);
		else b1->SetCheck(0);
	}
	else	GetDlgItem(IDC_chkPRIORITY_CATALOG)->ShowWindow(SW_HIDE);
	
	m_pDM->GetCellData(_T("��ġ����"),		m_idx,	strFurnishStatus);

	m_strFurnishStatus = strFurnishStatus;

	m_pDM->GetCellData(_T("SMS���ſ���"),	m_idx,	strSMSReceipYN	);
	m_pDM->GetCellData(_T("���Խ�û����"),	m_idx,	strApplicant	);
	m_pDM->GetCellData(_T("�켱���⿹�࿩��"),	m_idx,	strReservationYN);

	if		( strApplicant == _T("CLA") )
	{
		m_cmbApplicant.SetCurSel(1);
		m_pApplicant_Class->SetDialog( m_pDM->GetCellData(_T("��û����"), m_idx ), 
									   m_pDM->GetCellData(_T("���Խ�û�ڵ�"), m_idx ),  
									   m_pDM->GetCellData(_T("��û��"), m_idx )
									 );
	}
	else if ( strApplicant == _T("COU") )
	{
		m_cmbApplicant.SetCurSel(2);
		m_pApplicant_Cource->SetDialog( m_pDM->GetCellData(_T("��û����"), m_idx ), 
									    m_pDM->GetCellData(_T("���Խ�û�ڵ�"), m_idx ),  
									    m_pDM->GetCellData(_T("��û��"), m_idx )
									  );
	}
	else if ( strApplicant == _T("DEP") )
	{
		m_cmbApplicant.SetCurSel(3);
		m_pApplicant_Dept->SetDialog( m_pDM->GetCellData(_T("��û����"), m_idx ), 
									  m_pDM->GetCellData(_T("���Խ�û�ڵ�"), m_idx ),  
									  m_pDM->GetCellData(_T("��û��"), m_idx )
									 );
	}
	else 
	{
		m_cmbApplicant.SetCurSel(0);

		m_pApplicant_LoanUser->SetDialog( m_pDM, m_idx );

		if( _T("Y") == strSMSReceipYN )		m_pApplicant_LoanUser->m_bSMSUseYN = TRUE;
		else								m_pApplicant_LoanUser->m_bSMSUseYN = FALSE;
		if( _T("Y") == strReservationYN )		m_pApplicant_LoanUser->m_bReservationYN= TRUE;
		else									m_pApplicant_LoanUser->m_bReservationYN = FALSE;

		CString strFurnishStatus;
		m_pCM->GetControlMgrData(_T("��ġ����ڷ����"), strFurnishStatus);	
		m_pApplicant_LoanUser->SetRequestStatus(strFurnishStatus);
	}
	
	OnSelchangecmbAPPLICANT();
	OnSelchangecmbLasifyClass();
	UpdateData(FALSE);	

	SetReverse(m_pMainCM->CM_ALIAS, _T("MainGrid"), m_idx);
}


VOID CRequest_DataMgr::OnKillfocusedtWANTEDTITLE() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtWANTED_TITLE);
	if( pEdit == NULL ) return;
}

VOID CRequest_DataMgr::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == 0 )
	{
		KillTimer(0);

		CString strTitle;
		CString strSearchCount, strSpeciesKey;

		INT ids = m_pCM->GetControlMgrData(_T("��ǥ��"), strTitle);
		if (ids < 0) return;

		if (strTitle.IsEmpty()) return;

		CDuplicateSearch_Interface impl(this);
		if(m_nOpType != SINSERT)
		{
			m_pDM->GetCellData(_T("��KEY"), m_idx, strSpeciesKey);
		}
		ids = impl.ShowOnlyTitleDuplicateSearch(_T("��ġ�����������"), strTitle, strSearchCount, strSpeciesKey);
		if (ids < 0) return;

		if (strSearchCount == _T("0"))	return;
		

		if (m_pNoteCtrl != NULL)
		{
			delete m_pNoteCtrl;
		}
		
		m_pNoteCtrl = new CNoteCtrl;
		CString strNote;
		strNote.Format(_T("%s���� ������ �߰ߵǾ����ϴ�."), strSearchCount);
		m_pNoteCtrl->SetNote(strNote);

		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		point.y += 5;
		
		m_pNoteCtrl->Create(point, 200, 50, this);
		m_pNoteCtrl->ShowWindow(SW_SHOW);
	}		
	CDialog::OnTimer(nIDEvent);
}

VOID CRequest_DataMgr::OnbtnREQUESTPREV() 
{
	m_pApplicant_LoanUser->SetLoanUserSearch(FALSE);
	Display(-1);
}

VOID CRequest_DataMgr::OnbtnREQUESTNEXT() 
{
	m_pApplicant_LoanUser->SetLoanUserSearch(FALSE);
	Display(1);
}

VOID CRequest_DataMgr::OnOK() 
{
	INT ids, nIdx, nCheck = 0;
	
	CString sRRN, pubYear, strPrice, errstr, strSMSUseYN;
	
	CButton* b1;
	
	CString strBookCnt;
	m_pCM->GetControlMgrData(_T("å��"), strBookCnt);
	
	if (_ttoi(strBookCnt) < 1)
	{
		AfxMessageBox(_T("å���� 1�Ǻ��� ���� ���� �Է��� �� �����ϴ�."));
		return;
	}
	
	m_pCM->GetControlMgrData(_T("�����"), pubYear);
	m_pCM->GetControlMgrData(_T("����"), strPrice);
	if( strPrice.IsEmpty() == TRUE ) 
	{
		strPrice = _T("0");
		m_pCM->SetControlMgrData(_T("����"), strPrice);
	}

	if( m_pApplicant_LoanUser->CheckID() == FALSE )	return;

	if( m_pApplicant_LoanUser->CheckUserNo() == FALSE )	return;
	
	ids = pMakeSearchData->IsValidYearData(pubYear);
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("�Է��Ͻ� ����� ������ �ùٸ��� �ʽ��ϴ�."));
		return;
	}
	
	ids = CheckInputFieldData(m_pCM);
	if( ids < 0 )		return;
	
	UpdateData(TRUE);	
	nCheck = 0;

	if( _T("Y") == m_strPriorityCatalogYN )
	{
		b1 = (CButton*)GetDlgItem(IDC_chkPRIORITY_CATALOG);
		nCheck = b1->GetCheck();			
	}

	CString strFromISBN = _T("");
	m_pCM->GetControlMgrData(_T("ISBN"), strFromISBN);
	
	if ( strFromISBN.GetLength() >1 )
	{
		strFromISBN.TrimLeft();
		strFromISBN.TrimRight();
		
		if ( strFromISBN.Find(_T("-")) != -1 )
		{
			strFromISBN.Replace(_T("-"), _T(""));
		}
		
		if ( strFromISBN.Find(_T(" ")) != -1 )
		{
			strFromISBN.Replace(_T(" "), _T(""));
		}
		m_pCM->SetControlMgrData(_T("ISBN"), strFromISBN);
	}

	if((m_pInfo->m_bFurnishWishLimitYN || m_pInfo->m_bFurnishLimitByTheWeek) && m_nOpType == SINSERT)
	{
		CString strMsg;
		if(m_pInfo->m_nFurnishWishMaxCnt <= m_pApplicant_LoanUser->m_nFurnishCnt)
		{
			strMsg.Format(_T("����ڷ� ��û�� 1�δ� �ִ� %d�� ���� ��û�Ͻ� �� �ֽ��ϴ�."), m_pInfo->m_nFurnishWishMaxCnt);
			ESLAfxMessageBox(strMsg);
			return;		
		}
	}

	nIdx = m_cmbApplicant.GetCurSel();
	if( m_nOpType == SINSERT )
	{
		InsertREQData( nIdx,
					   m_pCM->CM_ALIAS.GetBuffer(0), 
					   m_pDM->DB_MGR_ALIAS.GetBuffer(0) ,
					   nCheck
					   );
	}
	else
	{
		ModifyREQData( nIdx,
					   m_pCM->CM_ALIAS.GetBuffer(0), 
					   m_pDM->DB_MGR_ALIAS.GetBuffer(0) ,
					   nCheck,
					   m_pMainCM->CM_ALIAS.GetBuffer(0)
					   );

	}
	if( ids < 0 )
	{
		switch( ids )
		{
		case -1000:		errstr = _T("��ġ����ڷ� �Է�/������ ���� CM/DM�� ã�� �� �����ϴ�.");
			break;
		case -1001:		errstr = _T("�Է�/���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�.");
			break;
		case -1002:		errstr = _T("�Է�/���������� ������ �� �����ϴ�.");
			break;
		case -1003:		errstr = _T("��û���а��� �ùٸ��� �ʽ��ϴ�.");
			break;
		default :		errstr = _T("�� �� ���� �����߻����� ���忡 �����Ͽ����ϴ�.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}

	if(m_pInfo->m_bFurnishWishLimitYN && m_nOpType == SINSERT)
	{
		CString strUserNo;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�����ڹ�ȣ"), strUserNo);
		if(_T("") != strUserNo)
		{
			m_pApplicant_LoanUser->m_nFurnishCnt = m_pApplicant_LoanUser->m_nFurnishCnt+1;
			CString strMsg;
			strMsg.Format(_T("%d / %d"), m_pApplicant_LoanUser->m_nFurnishCnt,m_pInfo->m_nFurnishWishMaxCnt);
			m_pApplicant_LoanUser->m_pCM->SetControlMgrData(_T("��û�Ǽ�"), strMsg);
		}
	}

	errstr = _T("��ġ��� �ڷḦ �����Ͽ����ϴ�.");
	ESLAfxMessageBox(errstr.GetBuffer(0));	
	
	CString bookCnt,price,tot,curcode;
	tot= _T("");
	int last_idx;
	if(m_nOpType == SINSERT){
		last_idx = m_pDM->GetRowCount() - 1;
	}else {
		last_idx = m_idx;
	}
	
	m_pDM->GetCellData(_T("å��") ,last_idx, bookCnt);
	m_pDM->GetCellData(_T("����"),last_idx, price);
	
	tot.Format( _T("%d"), _ttoi(bookCnt)*_ttoi(price));
	
	m_PriceConvertMgr.ConversionPriceInfoToDesc(tot,tot);
	
	m_pDM->SetCellData(_T("�Ѱ���"),tot,last_idx);

	CString strFurnishStatus;
	m_pCM->GetControlMgrData(_T("��ġ����ڷ����"), strFurnishStatus);	

	if ( m_strFurnishStatus != strFurnishStatus )
	{	
		CString strTemp;
		CStringArray ArrayFurnishKey;
		ArrayFurnishKey.RemoveAll();

		m_pDM->GetCellData(_T("��ġ���KEY"), m_idx, strTemp);

		m_strFurnishStatus = strFurnishStatus;
		ArrayFurnishKey.Add(strTemp);	
	}

	m_pApplicant_LoanUser->SetLoanUserSearch(FALSE);

	CString strTempNo;	
	strTempNo = m_pDM->GetCellData(_T("��ġ���_�ֹε�Ϲ�ȣ"), m_idx);
	m_pDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), strTempNo, m_idx);
	((CBO_ACQ_RequestDataMgr*)m_pParent)->ChangeSecuriyCivilNo(m_pDM,m_idx,_T("�ֹε�Ϲ�ȣ"));

	if(m_bUserNoChange)
	{
		((CBO_ACQ_RequestDataMgr*)m_pParent)->ChangeUserNo(m_pDM,_T("��ġ���_�����ڹ�ȣ"));
	}

	if( m_nOpType == SINSERT )
	{
		m_pMainGrid->DisplayLine(m_pDM->GetRowCount()-1);
		m_pCM->AllControlClear();	
	}
	else
	{
		m_pMainGrid->DisplayLine(m_idx);

		Display(1);
	}
}

INT CRequest_DataMgr::CheckInputFieldData(CESL_ControlMgr *pCM)
{
	if( pCM == NULL )	return -1;
    
	CString applicant, socialid, title, author, publisher,userNo;
	CString query,rec_key;
	BOOL chkReservationYN;
	CString strSmsUseYN_Query, strSmsUseYN;
	CString strPhoneNo2, strPhoneNo3; 
	INT strPhoneNoCnt2,strPhoneNoCnt3;
	BOOL strChkSmsReceiptYN;

	switch( m_cmbApplicant.GetCurSel() )
	{
	case 0:		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("��û��"), applicant);
				m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�����ڹ�ȣ"), userNo);
				chkReservationYN = m_pApplicant_LoanUser->m_bReservationYN;
				m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�ڵ���2"), strPhoneNo2);
				m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�ڵ���3"), strPhoneNo3);
				strPhoneNoCnt2 = strPhoneNo2.GetLength();
				strPhoneNoCnt3 = strPhoneNo3.GetLength();
				strChkSmsReceiptYN = m_pApplicant_LoanUser->m_bSMSUseYN;
				strSmsUseYN_Query.Format( _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = 'SMS��������' AND MANAGE_CODE = UDF_MANAGE_CODE"));
				m_pApplicant_LoanUser->m_pUSERDM->GetOneValueQuery( strSmsUseYN_Query, strSmsUseYN );
		break;
	case 1:		m_pApplicant_Class->m_pCM->GetControlMgrData(_T("��û��"), applicant);
		break;
	case 2:		m_pApplicant_Cource->m_pCM->GetControlMgrData(_T("��û��"), applicant);
		break;
	case 3:		m_pApplicant_Dept->m_pCM->GetControlMgrData(_T("��û��"), applicant);
		break;
	default:
	{
			ESLAfxMessageBox(_T("��û������ �ùٸ��� ������ �ֽʽÿ�."));
			return -1;
	}
		break;
	}

	pCM->GetControlMgrData(_T("��ǥ��"), title);

	if( applicant.IsEmpty() == TRUE )
	{
		ESLAfxMessageBox(_T("��û�ڴ� �ʼ� �Է� �׸��Դϴ�."));
		return -1;
	}
	if( title.IsEmpty() == TRUE  )		
	{
		ESLAfxMessageBox(_T("������ �ʼ� �Է� �׸��Դϴ�."));
		return -1;
	}
	
	if(strSmsUseYN != _T("N"))
	{
		if( strChkSmsReceiptYN == TRUE )
		{
			if( strPhoneNoCnt2 < 3 || strPhoneNoCnt3 < 4 )
			{
				ESLAfxMessageBox(_T("�޴��� ��ȣ�� ��Ȯ�� �Է��� �ּ���."));
				return -1;
			}
		}
	}

	if( chkReservationYN == TRUE )
	{	
		if( userNo.IsEmpty() == TRUE )
		{
			ESLAfxMessageBox(_T("�켱 ���� ���� �� �����ڹ�ȣ�� �ʼ� �Է� �׸��Դϴ�."));
			return -1;
		}
	}

	return 0;
}

UINT CRequest_DataMgr::OnNoteClicked(LPARAM lParam, WPARAM wParam)
{
	CString strTmpData;

	CDuplicateSearch_Interface impl(this);
	impl.ShowTitleDuplicateSearchDlg(_T("�����ڵ���������"), strTmpData);
	
	return 0;
}

VOID CRequest_DataMgr::OnbtnREQTITLEDUPSEARCH() 
{
	INT ids;
	CString strTitle, strAuthor, strPublisher, strPubYear, strSearchCount, strTmpData, strMarc = _T("");
	CMarc marc;
	CDuplicateSearch_Interface impl(this);

	m_pCM->GetControlMgrData(_T("��ǥ��"), strTitle);
	m_pCM->GetControlMgrData(_T("������"), strPublisher);
	m_pCM->GetControlMgrData(_T("�����"), strPubYear);
	m_pCM->GetControlMgrData(_T("������"), strAuthor);
	

	CString strEaISBN;
	m_pCM->GetControlMgrData(_T("ISBN"), strEaISBN);
	if(  strTitle.IsEmpty() == TRUE && strEaISBN.IsEmpty() == TRUE &&
	     strAuthor.IsEmpty() == TRUE && strPublisher.IsEmpty() == TRUE )	return;

	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("245$a"), strTitle);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("245$d"), strAuthor);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("260$b"), strPublisher);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("260$c"), strPubYear);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("020$a"), strEaISBN);
	m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if( strMarc.IsEmpty() == TRUE )		return;	

	ids = impl.SearchDuplicateDataByIndividually(_T("�Ǻ���������"), strMarc, _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), strSearchCount);
	if( ids < 0 )	return;
	
	if( strSearchCount == _T("0") )
	{
		ESLAfxMessageBox(_T("�������� ����� �����ϴ�."));
		return;	
	}

	impl.ShowTitleDuplicateSearchDlg(_T("��ġ����Է�ȭ�麹������"), strTmpData);
}

INT CRequest_DataMgr::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;

	nFirstSelIdx = m_pMainGrid->GetRow() - 1;

	if( nFirstSelIdx < 0 )	nFirstSelIdx = 0;

	return nFirstSelIdx;
}

INT CRequest_DataMgr::GetPrevSelectIndex()
{
 	INT nPrevSelIndex = 0;

	if( m_bGridSelectYN == TRUE )
	{
		while( 1 )
		{
			if(nPrevSelIndex == m_idx || nPrevSelIndex < 0 )
			{
				break;
			}
			else if( nPrevSelIndex < m_idx )
			{
				nPrevSelIndex = m_pMainGrid->SelectGetNext();
				if( nPrevSelIndex < 0 )
				{
					nPrevSelIndex = m_pMainGrid->SelectGetTailPosition();
					break;
				}
			}
			else if( nPrevSelIndex > m_idx )
			{
				nPrevSelIndex = m_pMainGrid->SelectGetPrev();
			}
		}
		nPrevSelIndex = m_pMainGrid->SelectGetPrev();
		if( nPrevSelIndex < 0 )		
		{
			nPrevSelIndex = m_pMainGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if( m_idx != m_firstidx )	
		{
			nPrevSelIndex = m_idx - 1;
		}
	}

	return nPrevSelIndex;
}

INT CRequest_DataMgr::GetNextSelectIndex()
{
 	INT nNextSelIndex = 0;

	if( m_bGridSelectYN == TRUE )
	{
		while( 1 )
		{
			if(nNextSelIndex == m_idx || nNextSelIndex < 0 )
			{
				break;
			}
			else if( nNextSelIndex < m_idx )
			{
				nNextSelIndex = m_pMainGrid->SelectGetNext();
			}
			else if( nNextSelIndex > m_idx )
			{
				nNextSelIndex = m_pMainGrid->SelectGetPrev();
			}
		}
		nNextSelIndex = m_pMainGrid->SelectGetNext();
		if(nNextSelIndex < 0)		
		{
			nNextSelIndex = m_pMainGrid->SelectGetTailPosition();
		}
		
	}
	else
	{
		if( m_idx != m_lastidx )	
		{
			nNextSelIndex = m_idx + 1;
		}
		else 
		{
			nNextSelIndex = m_idx;
		}
	}

	return nNextSelIndex;
}

VOID CRequest_DataMgr::ShowCodeResource()
{
	INT i, nCodeResourcesHeight, nCount;
	INT nArrCodeResourceID[256], nArrOtherResources[256];	
	CRect rcWindow, rcResource;
	CWnd* pWnd = NULL;
	
	GetWindowRect(rcWindow);

	if( m_nOpType != SINSERT && rcWindow == m_rcWindow )	return;


	nCodeResourcesHeight = 155;	

	nCount = 0;	
	nArrCodeResourceID[nCount++] = IDC_stcREQ_DATA_STATUS;
	nArrCodeResourceID[nCount++] = IDC_stcREQ_FURNISH_STATUS;
	nArrCodeResourceID[nCount++] = IDC_edtREQ_FURNISH_STATUS;
	nArrCodeResourceID[nCount++] = IDC_stcREQ_CANCEL_REASON;
	nArrCodeResourceID[nCount++] = IDC_richREQ_CANCEL_REASON;
	
	for( i=0; i<nCount; i++ )
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if( pWnd->GetSafeHwnd() != NULL )
		{
			if( m_nOpType == SINSERT )	pWnd->ShowWindow(SW_HIDE);		
			else						pWnd->ShowWindow(SW_SHOW);
		}
	}

	nCount = 0;
	nArrOtherResources[nCount++] = IDC_stcREQ_BUTTON_GROUP;
	nArrOtherResources[nCount++] = IDC_btnREQUEST_PREV;
	nArrOtherResources[nCount++] = IDC_btnREQUEST_NEXT;
	nArrOtherResources[nCount++] = IDC_btnREQ_TITLE_DUP_SEARCH;
	nArrOtherResources[nCount++] = IDOK;
	nArrOtherResources[nCount++] = IDCANCEL;

	for( i=0; i<nCount ; i++ )
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if( pWnd->GetSafeHwnd() != NULL )
		{
			pWnd->GetWindowRect(rcResource);
			ScreenToClient(rcResource);
			if( m_nOpType == SINSERT )
			{
				rcResource.top -= nCodeResourcesHeight;
				rcResource.bottom -= nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
			else
			{
				rcResource.top += nCodeResourcesHeight;
				rcResource.bottom += nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
		}
	}

	nCodeResourcesHeight = 155;
	
	if( m_strPriorityCatalogYN == _T("Y") )		GetDlgItem(IDC_chkPRIORITY_CATALOG)->ShowWindow(SW_SHOW);
	else									GetDlgItem(IDC_chkPRIORITY_CATALOG)->ShowWindow(SW_HIDE);

	GetWindowRect(rcResource);
	if( m_nOpType == SINSERT )
	{
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	}
	
	Invalidate();
	UpdateWindow();
}

VOID CRequest_DataMgr::SetFurnishStatus(CString strFurnishStatus)
{
	CString strDesc;

	if		( strFurnishStatus.IsEmpty() == TRUE )	strDesc = _T("��û��");
	else if	( strFurnishStatus == _T("1") )			strDesc = _T("��û��");
	else if	( strFurnishStatus == _T("2") )			strDesc = _T("ó����");
	else if	( strFurnishStatus == _T("3") )			strDesc = _T("������");
	else if	( strFurnishStatus == _T("4") )			strDesc = _T("��ҵ�");
	else											strDesc = _T("");	
	m_pCM->SetControlMgrData(_T("��ġ����"), strDesc);
}

VOID CRequest_DataMgr::OnbtnREQNEWSAVE() 
{
	INT ids, nIdx, nCheck = 0;
	CString sRRN, pubYear, strPrice, errstr;
	CButton* b1;

	CString strBookCnt;
	m_pCM->GetControlMgrData(_T("å��"), strBookCnt);
	
	if (_ttoi(strBookCnt) < 1)
	{
		AfxMessageBox(_T("å���� 1�Ǻ��� ���� ���� �Է��� �� �����ϴ�."));
		return;
	}
	m_pCM->GetControlMgrData(_T("�ֹε�Ϲ�ȣ"), sRRN);
	m_pCM->GetControlMgrData(_T("�����"), pubYear);
	m_pCM->GetControlMgrData(_T("����"), strPrice);
	if( strPrice.IsEmpty() == TRUE ) 
	{
		strPrice = _T("0");
		m_pCM->SetControlMgrData(_T("����"), strPrice);
	}

	if ( m_pApplicant_LoanUser->CheckID() == FALSE )	return;

	if( m_pApplicant_LoanUser->CheckUserNo() == FALSE )	return;
	
	ids = pMakeSearchData->IsValidYearData(pubYear);
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("�Է��Ͻ� ����� ������ �ùٸ��� �ʽ��ϴ�."));
		return;
	}

	ids = CheckInputFieldData(m_pCM);
	if( ids < 0 )		return;

	if( _T("Y") == m_strPriorityCatalogYN )
	{
		b1 = (CButton*)GetDlgItem(IDC_chkPRIORITY_CATALOG);
		nCheck = b1->GetCheck();			
	}

	CString strFromISBN = _T("");
	m_pCM->GetControlMgrData(_T("ISBN"), strFromISBN);
	
	if ( strFromISBN.GetLength() >1 )
	{
		strFromISBN.TrimLeft();
		strFromISBN.TrimRight();
		
		if ( strFromISBN.Find(_T("-")) != -1 )
		{
			strFromISBN.Replace(_T("-"), _T(""));
		}
		
		if ( strFromISBN.Find(_T(" ")) != -1 )
		{
			strFromISBN.Replace(_T(" "), _T(""));
		}
		m_pCM->SetControlMgrData(_T("ISBN"), strFromISBN);
	}

	nIdx = m_cmbApplicant.GetCurSel();
	InsertREQData( nIdx,
				   m_pCM->CM_ALIAS.GetBuffer(0), 
				   m_pDM->DB_MGR_ALIAS.GetBuffer(0) ,
				   nCheck
				   );

	m_pApplicant_LoanUser->SetLoanUserSearch(FALSE);

	if( ids < 0 )
	{
		switch(ids)
		{
		case -1000:		errstr = _T("��ġ����ڷ� ���������� ���� CM/DM�� ã�� �� �����ϴ�.");
			break;
		case -1001:		errstr = _T("��������� ������ DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�.");
			break;
		case -1002:		errstr = _T("�Է��Ͻ� ������ ���� ������ �� �����ϴ�.");
			break;
		case -1003:		errstr = _T("��û���а��� �ùٸ��� �ʽ��ϴ�.");
			break;
		default :		errstr = _T("�� �� ���� �����߻����� ���忡 �����Ͽ����ϴ�.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}

	errstr = _T("��ġ��� �ڷḦ �����Ͽ����ϴ�.");
	ESLAfxMessageBox(errstr.GetBuffer(0));

	CString bookCnt,price,tot,curcode;
	tot= _T("");
	int last_idx = m_pDM->GetRowCount() - 1;
	m_pDM->GetCellData(_T("å��") ,last_idx, bookCnt);
	m_pDM->GetCellData(_T("����"),last_idx, price);
	
	tot.Format( _T("%d"), _ttoi(bookCnt)*_ttoi(price));
	
	m_PriceConvertMgr.ConversionPriceInfoToDesc(tot,tot);
	
	m_pDM->SetCellData(_T("�Ѱ���"),tot, last_idx );

	m_pMainGrid->DisplayLine(m_pDM->GetRowCount()-1);	
	Display(1);

	return;	
}

VOID CRequest_DataMgr::OnKillfocusedtNID() 
{
	INT nCheck;
	CString strCivilNo;
	CButton* b1 = (CButton*)GetDlgItem(IDC_chkID);
	CRequestManager pMgr(this);
	
	m_pCM->GetControlMgrData(_T("�ֹε�Ϲ�ȣ"), strCivilNo);
	if( strCivilNo.IsEmpty() == TRUE )	return ;
	
	nCheck = b1->GetCheck();
	if( nCheck == 0 )
	{
		m_pCM->SetControlMgrData(_T("�ֹε�Ϲ�ȣ"), strCivilNo);
	}
	else
	{
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strCivilNo);
		if( strCivilNo.GetLength() != 13 ) 
		{
			ESLAfxMessageBox(_T("�ùٸ� �ֹε�Ϲ�ȣ�� �ƴմϴ�."));
			GetDlgItem(IDC_edtNID)->SetFocus();
			return;
		}
		CString strHighNum, strLowNum;
		strHighNum = strCivilNo.Left(6);
		strLowNum = strCivilNo.Right(7);
		strCivilNo = strHighNum + _T("-") + strLowNum;
		if(!pMgr.IsRRN(strCivilNo))
		{
			ESLAfxMessageBox(_T("�ùٸ� �ֹε�Ϲ�ȣ�� �ƴմϴ�."));
			m_pCM->SetControlMgrData(_T("�ֹε�Ϲ�ȣ"), _T(""));
		}
		else
			m_pCM->SetControlMgrData(_T("�ֹε�Ϲ�ȣ"), strCivilNo);
	}

		return;
}

BOOL CRequest_DataMgr::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CRequest_DataMgr::OnKillfocusedtREQUESTPRICE() 
{
	INT nPos;
	CString strPrice, strTmpData;

	m_pCM->GetControlMgrData(_T("����"), strPrice);

	nPos = strPrice.Find(_T("."));
	if( nPos > 0 )		strTmpData = strPrice.Left(nPos);
	else strTmpData = strPrice;

	if( strTmpData.GetLength() > 12 )
	{
		ESLAfxMessageBox(_T("���������� �Ҽ����� �ڷᰡ 12�ڸ� �̻��� �� �����ϴ�.")); 
		GetDlgItem(IDC_edtREQUEST_PRICE)->SetFocus();
		return ;
	}

	if( nPos > 0 )
	{
		strTmpData = strPrice.Mid(nPos+1);
		if( strTmpData.GetLength() > 2 )
		{
			ESLAfxMessageBox(_T("���������� �Ҽ������� ��°�ڸ������� �Է� �����մϴ�."));
			return ;
		}
	}	
}

VOID CRequest_DataMgr::OnSelchangecmbAPPLICANT() 
{
	INT nIdx = m_cmbApplicant.GetCurSel();

	switch( nIdx )
	{
	case 0:
	{
		m_pApplicant_Class->ShowWindow(SW_HIDE);
		m_pApplicant_Cource->ShowWindow(SW_HIDE);
		m_pApplicant_Dept->ShowWindow(SW_HIDE);
		m_pApplicant_LoanUser->ShowWindow(SW_SHOW);
	}
		break;
	case 1:
	{
		m_pApplicant_Class->ShowWindow(SW_SHOW);
		m_pApplicant_Cource->ShowWindow(SW_HIDE);
		m_pApplicant_Dept->ShowWindow(SW_HIDE);
		m_pApplicant_LoanUser->ShowWindow(SW_HIDE);
	}
		break;
	case 2:
	{
		m_pApplicant_Class->ShowWindow(SW_HIDE);
		m_pApplicant_Cource->ShowWindow(SW_SHOW);
		m_pApplicant_Dept->ShowWindow(SW_HIDE);
		m_pApplicant_LoanUser->ShowWindow(SW_HIDE);
	}
		break;
	case 3:
	{
		m_pApplicant_Class->ShowWindow(SW_HIDE);
		m_pApplicant_Cource->ShowWindow(SW_HIDE);
		m_pApplicant_Dept->ShowWindow(SW_SHOW);
		m_pApplicant_LoanUser->ShowWindow(SW_HIDE);
	}
		break;

	default:
		break;
	}
}

INT CRequest_DataMgr::InsertREQData( INT nIdx, CString CMAlias, CString DMAlias, INT nCheck )
{
	INT ids, nCurRow;
	CString strApplicantDateTime;
	CString strTmpData, strRec_key, firstwork, lastwork;
	CArray <CString, CString> RemoveAliasList;
	CESL_DataMgr* pDM	 = FindDM(DMAlias);
	CESL_ControlMgr* pCM = FindCM(CMAlias);
	if( pDM == NULL || pCM == NULL )	return -1000;

	ids = CopyCMToDM(pCM, pDM, -1);
	if( ids < 0 )		return -1002;

	nCurRow = pDM->GetRowCount() - 1;

	switch( nIdx )
	{
	case 0:
	{
		CString strTmpData, strHp, strEmail;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�ڵ���1"), strTmpData);		if(!strTmpData.IsEmpty()) strHp += strTmpData;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�ڵ���2"), strTmpData);		if(!strTmpData.IsEmpty()) strHp += _T("-") + strTmpData;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�ڵ���3"), strTmpData);		if(!strTmpData.IsEmpty()) strHp += _T("-") + strTmpData;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("EMAIL_ID"), strTmpData);	if(!strTmpData.IsEmpty()) strEmail += strTmpData;
		CESL_ControlMultiComboBox * pCMB = (CESL_ControlMultiComboBox *)m_pApplicant_LoanUser->m_pCM->FindControl(_T("EMAIL_SITE"));
		pCMB->GetWindowText(strTmpData);						if(!strTmpData.IsEmpty() && !strEmail.IsEmpty() && strTmpData != _T("�������")) strEmail += _T("@") + strTmpData;

		if( m_pApplicant_LoanUser->m_bSMSUseYN ) pDM->SetCellData(_T("SMS���ſ���"), _T("Y"), nCurRow);
		else pDM->SetCellData(_T("SMS���ſ���"), _T("N"), nCurRow);
		
		if( m_pApplicant_LoanUser->m_bReservationYN ) pDM->SetCellData(_T("�켱���⿹�࿩��"), _T("Y"), nCurRow);
		else pDM->SetCellData(_T("�켱���⿹�࿩��"), _T("N"), nCurRow);

		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("��û��"), strTmpData);
		pDM->SetCellData(_T("��û��"), strTmpData, nCurRow);
		
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�����ڹ�ȣ"), strTmpData);

		pDM->SetCellData(_T("�����ڹ�ȣ"), strTmpData, nCurRow);
		
		if( m_pApplicant_LoanUser->m_bLoanUserSearch )
		{
			m_pApplicant_LoanUser->m_pUSERDM->GetCellData(_T("�����ڹ�ȣ"),0,strTmpData);
			pDM->SetCellData(_T("��ġ���_�����ڹ�ȣ"), strTmpData, nCurRow);
		}
	
		CString strTimeTemp;
		CString strDate, strTime;
		strTmpData.Empty();
		strTime.Empty();
		strTimeTemp.Empty();
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("��û����"), strDate);		
		m_pApplicant_LoanUser->m_hApplicantTime->GetData(strTime);

		strApplicantDateTime.Format(_T("%s %s"), strDate, strTime);
		strTmpData.Format(_T("TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS')"), strApplicantDateTime);
		pDM->SetCellData(_T("��û����"), strTmpData, nCurRow);		

		CString strCivilNo;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�ֹε�Ϲ�ȣ"), strCivilNo);

		strCivilNo = m_pApplicant_LoanUser->GetOriginCivilNo();
		
		CString strAddress;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�ּ�"), strAddress);

		CString strZipCode;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�����ȣ"), strZipCode);

		CString strPhoneNo;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("��ȭ��ȣ"), strPhoneNo);

		pDM->SetCellData(_T("�ڵ���"), strHp, nCurRow);
		pDM->SetCellData(_T("EMAIL"), strEmail, nCurRow);
		pDM->SetCellData(_T("��ġ���_�ֹε�Ϲ�ȣ"), strCivilNo, nCurRow);
		pDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), strCivilNo, nCurRow);
		pDM->SetCellData(_T("�ּ�"), strAddress, nCurRow);
		pDM->SetCellData(_T("�����ȣ"), strZipCode, nCurRow);
		pDM->SetCellData(_T("��ȭ��ȣ"), strPhoneNo, nCurRow);
	}
		break;

	case 1:
	{
		CString strTmpData;
		m_pApplicant_Class->m_pCM->GetControlMgrData(_T("��û��"), strTmpData);
		pDM->SetCellData(_T("��û��"), strTmpData, nCurRow);

		strTmpData.Empty();
		m_pApplicant_Class->m_pCM->GetControlMgrData(_T("��û����"), strTmpData);
		pDM->SetCellData(_T("��û����"), strTmpData, nCurRow);
		
		strTmpData.Empty();
		m_pApplicant_Class->m_pCM->GetControlMgrData(_T("�μ���"), strTmpData);
			pDM->SetCellData(_T("���Խ�û�ڵ�"), strTmpData, nCurRow);

		strTmpData.Empty();
			pDM->SetCellData(_T("���Խ�û����"), _T("CLA") , nCurRow);

	}
		break;

	case 2:
	{
		CString strTmpData;
		m_pApplicant_Cource->m_pCM->GetControlMgrData(_T("��û��"), strTmpData);
		pDM->SetCellData(_T("��û��"), strTmpData, nCurRow);

		strTmpData.Empty();
		m_pApplicant_Cource->m_pCM->GetControlMgrData(_T("��û����"), strTmpData);
		pDM->SetCellData(_T("��û����"), strTmpData, nCurRow);
		
		strTmpData.Empty();
		m_pApplicant_Cource->m_pCM->GetControlMgrData(_T("�а���"), strTmpData);
			pDM->SetCellData(_T("���Խ�û�ڵ�"), strTmpData, nCurRow);

		strTmpData.Empty();
			pDM->SetCellData(_T("���Խ�û����"), _T("COU") , nCurRow);

	}
		break;

	case 3:
	{
		CString strTmpData;
		m_pApplicant_Dept->m_pCM->GetControlMgrData(_T("��û��"), strTmpData);
		pDM->SetCellData(_T("��û��"), strTmpData, nCurRow);

		strTmpData.Empty();
		m_pApplicant_Dept->m_pCM->GetControlMgrData(_T("��û����"), strTmpData);
		pDM->SetCellData(_T("��û����"), strTmpData, nCurRow);
		
		strTmpData.Empty();
		m_pApplicant_Dept->m_pCM->GetControlMgrData(_T("�кθ�"), strTmpData);
			pDM->SetCellData(_T("���Խ�û�ڵ�"), strTmpData, nCurRow);

		strTmpData.Empty();
			pDM->SetCellData(_T("���Խ�û����"), _T("DEP") , nCurRow);


	}
		break;

	default:	return -1003;
		break;
	}
	
	firstwork = lastwork = GetWorkLogMsg(WORK_LOG);	

	pDM->MakeRecKey(strRec_key);

	pDM->SetCellData(_T("���Ա���"), _T("1"), nCurRow);

	pDM->SetCellData(_T("��ġ���KEY"), strRec_key, nCurRow);
	
	pDM->SetCellData(_T("��ġ����"), _T("1"), nCurRow);
	pDM->SetCellData(_T("��ġ���¼���"), _T("��û��"), nCurRow);
	pDM->SetCellData(_T("�����۾�"), firstwork, nCurRow);
	pDM->SetCellData(_T("�������۾�"), lastwork, nCurRow);

	strTmpData = pDM->GetCellData(_T("��ǥ��"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("���Ŀ뺻ǥ��"), strTmpData, nCurRow);

	strTmpData = pDM->GetCellData(_T("������"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("�������ε���"), strTmpData, nCurRow);

	strTmpData = pDM->GetCellData(_T("������"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("�������ε���"), strTmpData, nCurRow);

	strTmpData = pDM->GetCellData(_T("��û��"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("��û���ε���"), strTmpData, nCurRow);

	if( nCheck == 0 )
	{
		pDM->SetCellData(_T("�켱�����ڷ�"), _T("N"), nCurRow);
		pDM->SetCellData(_T("UDF_�켱�����ڷ�"), _T(""), nCurRow);
	}
	else 
	{
		pDM->SetCellData(_T("�켱�����ڷ�"), _T("Y"), nCurRow);
		pDM->SetCellData(_T("UDF_�켱�����ڷ�"), _T("��"), nCurRow);
	}

	BOOL bCheck;
	bCheck = ( (CButton*) GetDlgItem (IDC_chkREMARK) )->GetCheck();
	if ( bCheck ) 
	{
		m_pCM->GetControlMgrData( _T("���"), strTmpData, -1, 1 );
		m_pCM->SetControlMgrData( _T("��һ���"), strTmpData );
		pDM->SetCellData(_T("UDF_��һ���"), strTmpData, nCurRow);
		pDM->SetCellData(_T("��һ���"), strTmpData, nCurRow);
	}
	else
	{
		strTmpData = pDM->GetCellData(_T("��һ���"), nCurRow);
		strTmpData.TrimLeft(); strTmpData.TrimRight();
		strTmpData.Replace(_T("\r"), _T(" "));  strTmpData.Replace(_T("\n"), _T(" "));
		pDM->SetCellData(_T("UDF_��һ���"), strTmpData, nCurRow);
	}

	m_pCM->GetControlMgrData(_T("��ġ����ڷ����"), strTmpData);
	pDM->SetCellData(_T("��ġ����ڷ����"), strTmpData, nCurRow);	
	
	m_pCM->GetControlMgrData(_T("å��"), strTmpData);
	pDM->SetCellData(_T("å��"), strTmpData, nCurRow);	

	
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_�켱�����ڷ�"));
	RemoveAliasList.Add(_T("�Ѱ���"));

	pDM->SetDataMgrRef(33, _T("��û����"), _T("APPLICANT_DATE"), _T("int"), _T(""), _T(""));

	CString strManageCode ;
	strManageCode = m_pInfo->MANAGE_CODE;
	pDM->SetCellData(_T("��������"), strManageCode, nCurRow);

	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		if(_T("BO_FURNISH_TBL") == pDM->TBL_NAME)
		{
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					INT nC, nCIdx;
					CString strColumnAlias;
					CString strFieldAlias, strFieldName, strFieldType, strFieldValue, strDataType;
					nC = pDM->GetColCount();						
					for(nCIdx = 0; nCIdx < nC; nCIdx++)
					{
						pDM->GetColumnRefInfo(nCIdx, strFieldAlias, strFieldName, strFieldType, strFieldValue, strDataType);
						if(strFieldName == _T("CIVIL_NO") || strFieldName == _T("ADDRESS") || strFieldName == _T("TEL") || strFieldName == _T("HANDPHONE") || strFieldName == _T("EMAIL"))
						{
							CStringArray DataArray;
							pDM->GetCellData(nCurRow, nCIdx, strFieldValue);
							DataArray.Add(strFieldValue);
							if(TRUE == pApi->ConnectSocket())
							{
								if(TRUE == pApi->UserInfoEncrytion(_T("BO_FURNISH_TBL"), strFieldName, DataArray))
								{
									pDM->SetCellData(nCurRow, nCIdx, DataArray.GetAt(0));
								}
								pApi->CloseSocket();
							}
							DataArray.RemoveAll(); DataArray.FreeExtra();
						}
					}
				}
				delete pApi;
				pApi = NULL;
			}
		}
	}

	pDM->StartFrame();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, nCurRow, pDM);
	ids = pDM->SendFrame();
	if( ids < 0 )	return -1001;
	pDM->EndFrame();

	pDM->SetDataMgrRef(33, _T("��û����"), _T("TO_CHAR(APPLICANT_DATE, 'YYYY/MM/DD HH24:MI:SS')"), _T("str"), _T(""), _T(""));

	pDM->SetCellData(_T("��û����"), strApplicantDateTime, nCurRow);

	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		if(_T("BO_FURNISH_TBL") == pDM->TBL_NAME)
		{
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					INT nC, nCIdx;
					CString strColumnAlias;
					CString strFieldAlias, strFieldName, strFieldType, strFieldValue, strDataType;
					nC = pDM->GetColCount();						
					for(nCIdx = 0; nCIdx < nC; nCIdx++)
					{
						pDM->GetColumnRefInfo(nCIdx, strFieldAlias, strFieldName, strFieldType, strFieldValue, strDataType);
						if(strFieldName == _T("CIVIL_NO") || strFieldName == _T("ADDRESS") || strFieldName == _T("TEL") || strFieldName == _T("HANDPHONE") || strFieldName == _T("EMAIL"))
						{
							CStringArray DataArray;
							pDM->GetCellData(nCurRow, nCIdx, strFieldValue);
							DataArray.Add(strFieldValue);
							if(TRUE == pApi->ConnectSocket())
							{
								if(TRUE == pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), strFieldName, DataArray))
								{
									pDM->SetCellData(nCurRow, nCIdx, DataArray.GetAt(0));
								}
								pApi->CloseSocket();
							}
							DataArray.RemoveAll(); DataArray.FreeExtra();
						}
					}
				}
				delete pApi;
				pApi = NULL;
			}
		}
	}

	if( ids < 0 ) return -1001;
	
	return 0;
}

INT CRequest_DataMgr::ModifyREQData( INT nIdx, CString CMAlias, CString DMAlias, INT nCheck, CString MainCMAlias )
{
	INT ids, nCurRow;
	CString strTmpData, rec_key, lastwork;
	CString strApplicantDateTime;
	CESL_DataMgr * pDM = FindDM(DMAlias);
	CESL_ControlMgr * pCM = FindCM(CMAlias);

	if( pDM == NULL || pCM == NULL )	return -1000;

	ids = CopyCMToDM(pCM, pDM, m_idx);
	if( ids < 0 )	return -1002;	

	nCurRow = m_idx;

	lastwork = GetWorkLogMsg(WORK_LOG);
	
	switch( nIdx )
	{
	case 0:
	{
		CString strTmpData, strHp, strEmail;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�ڵ���1"), strTmpData);		if(!strTmpData.IsEmpty()) strHp += strTmpData;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�ڵ���2"), strTmpData);		if(!strTmpData.IsEmpty()) strHp += _T("-") + strTmpData;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�ڵ���3"), strTmpData);		if(!strTmpData.IsEmpty()) strHp += _T("-") + strTmpData;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("EMAIL_ID"), strTmpData);	if(!strTmpData.IsEmpty()) strEmail += strTmpData;
		CESL_ControlMultiComboBox * pCMB = (CESL_ControlMultiComboBox *)m_pApplicant_LoanUser->m_pCM->FindControl(_T("EMAIL_SITE"));
		pCMB->GetWindowText(strTmpData);						if(!strTmpData.IsEmpty() && !strEmail.IsEmpty() && strTmpData != _T("�������")) strEmail += _T("@") + strTmpData;

		if( m_pApplicant_LoanUser->m_bSMSUseYN ) pDM->SetCellData(_T("SMS���ſ���"), _T("Y"), nCurRow);
		else pDM->SetCellData(_T("SMS���ſ���"), _T("N"), nCurRow);
		
		if( m_pApplicant_LoanUser->m_bReservationYN ) pDM->SetCellData(_T("�켱���⿹�࿩��"), _T("Y"), nCurRow);
		else pDM->SetCellData(_T("�켱���⿹�࿩��"), _T("N"), nCurRow);

		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("��û��"), strTmpData);
		pDM->SetCellData(_T("��û��"), strTmpData, nCurRow);

		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�����ڹ�ȣ"), strTmpData);

		pDM->SetCellData(_T("�����ڹ�ȣ"), strTmpData, nCurRow);

		if( m_pApplicant_LoanUser->m_bLoanUserSearch )
		{
			m_pApplicant_LoanUser->m_pUSERDM->GetCellData(_T("�����ڹ�ȣ"),0,strTmpData);
			pDM->SetCellData(_T("��ġ���_�����ڹ�ȣ"), strTmpData, nCurRow);
		}
	
		CString strTime, strDate;
		
		strTmpData.Empty();
		strTime.Empty();		

		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("��û����"), strDate);
		m_pApplicant_LoanUser->m_hApplicantTime->GetData(strTime);
		strApplicantDateTime.Format(_T("%s %s"), strDate, strTime);
		strTmpData.Format(_T("TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS')"), strApplicantDateTime);
		pDM->SetCellData(_T("��û����"), strTmpData, nCurRow);			

		CString strCivilNo;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�ֹε�Ϲ�ȣ"), strCivilNo);

		strCivilNo = m_pApplicant_LoanUser->GetOriginCivilNo();
		
		CString strAddress;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�ּ�"), strAddress);

		CString strZipCode;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("�����ȣ"), strZipCode);

		CString strPhoneNo;
		m_pApplicant_LoanUser->m_pCM->GetControlMgrData(_T("��ȭ��ȣ"), strPhoneNo);
	
		pDM->SetCellData(_T("�ڵ���"), strHp, nCurRow);
		pDM->SetCellData(_T("EMAIL"), strEmail, nCurRow);
		pDM->SetCellData(_T("��ġ���_�ֹε�Ϲ�ȣ"), strCivilNo, nCurRow);
		pDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), strCivilNo, nCurRow);
		pDM->SetCellData(_T("�ּ�"), strAddress, nCurRow);
		pDM->SetCellData(_T("�����ȣ"), strZipCode, nCurRow);
		pDM->SetCellData(_T("��ȭ��ȣ"), strPhoneNo, nCurRow);
	
	}
	break;

	case 1:
	{
		CString strTmpData;
		m_pApplicant_Class->m_pCM->GetControlMgrData(_T("��û��"), strTmpData);
		pDM->SetCellData(_T("��û��"), strTmpData, nCurRow);

		strTmpData.Empty();
		m_pApplicant_Class->m_pCM->GetControlMgrData(_T("��û����"), strTmpData);
		pDM->SetCellData(_T("��û����"), strTmpData, nCurRow);
		
		strTmpData.Empty();
		m_pApplicant_Class->m_pCM->GetControlMgrData(_T("�μ���"), strTmpData);
			pDM->SetCellData(_T("���Խ�û�ڵ�"), strTmpData, nCurRow);

		strTmpData.Empty();
			pDM->SetCellData(_T("���Խ�û����"), _T("CLA") , nCurRow);

	}
		break;

	case 2:
	{
		CString strTmpData;
		m_pApplicant_Cource->m_pCM->GetControlMgrData(_T("��û��"), strTmpData);
		pDM->SetCellData(_T("��û��"), strTmpData, nCurRow);

		strTmpData.Empty();
		m_pApplicant_Cource->m_pCM->GetControlMgrData(_T("��û����"), strTmpData);
		pDM->SetCellData(_T("��û����"), strTmpData, nCurRow);
		
		strTmpData.Empty();
		m_pApplicant_Cource->m_pCM->GetControlMgrData(_T("�а���"), strTmpData);
			pDM->SetCellData(_T("���Խ�û�ڵ�"), strTmpData, nCurRow);

		strTmpData.Empty();
			pDM->SetCellData(_T("���Խ�û����"), _T("COU") , nCurRow);

	}
		break;

	case 3:
	{
		CString strTmpData;
		m_pApplicant_Dept->m_pCM->GetControlMgrData(_T("��û��"), strTmpData);
		pDM->SetCellData(_T("��û��"), strTmpData, nCurRow);

		strTmpData.Empty();
		m_pApplicant_Dept->m_pCM->GetControlMgrData(_T("��û����"), strTmpData);
		pDM->SetCellData(_T("��û����"), strTmpData, nCurRow);
		
		strTmpData.Empty();
		m_pApplicant_Dept->m_pCM->GetControlMgrData(_T("�кθ�"), strTmpData);
			pDM->SetCellData(_T("���Խ�û�ڵ�"), strTmpData, nCurRow);

		strTmpData.Empty();
			pDM->SetCellData(_T("���Խ�û����"), _T("DEP") , nCurRow);
		}
		break;

	default:	return -1003;
		break;
	}
	
	rec_key = pDM->GetCellData(_T("��ġ���KEY"), nCurRow);

	m_pCM->GetControlMgrData(_T("��ġ����ڷ����"), strTmpData);
	pDM->SetCellData(_T("��ġ����"), strTmpData, nCurRow);	
	
	pDM->SetCellData(_T("���Ա���"), _T("1"), nCurRow);

	pDM->SetCellData(_T("�������۾�"), lastwork, nCurRow);

	strTmpData = pDM->GetCellData(_T("��ǥ��"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("���Ŀ뺻ǥ��"), strTmpData, nCurRow);

	strTmpData = pDM->GetCellData(_T("������"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("�������ε���"), strTmpData, nCurRow);

	strTmpData = pDM->GetCellData(_T("������"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("�������ε���"), strTmpData, nCurRow);

	strTmpData = pDM->GetCellData(_T("��û��"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("��û���ε���"), strTmpData, nCurRow);

	if( nCheck == 0 ) 
	{
		pDM->SetCellData(_T("�켱�����ڷ�"), _T("N"), nCurRow);
		pDM->SetCellData(_T("UDF_�켱�����ڷ�"), _T(""), nCurRow);
	}
	else 
	{
		pDM->SetCellData(_T("�켱�����ڷ�"), _T("Y"), nCurRow);
		pDM->SetCellData(_T("UDF_�켱�����ڷ�"), _T("��"), nCurRow);
	}

	BOOL bCheck;
	bCheck = ( (CButton*) GetDlgItem (IDC_chkREMARK) )->GetCheck();
	if ( bCheck ) 
	{
		m_pCM->GetControlMgrData( _T("���"), strTmpData, -1, 1 );
		m_pCM->SetControlMgrData( _T("��һ���"), strTmpData );
		pDM->SetCellData(_T("UDF_��һ���"), strTmpData, nCurRow);
		pDM->SetCellData(_T("��һ���"), strTmpData, nCurRow);
	}
	else
	{
		strTmpData = pDM->GetCellData(_T("��һ���"), nCurRow);
		strTmpData.TrimLeft(); strTmpData.TrimRight();
		strTmpData.Replace(_T("\r"), _T(" "));  strTmpData.Replace(_T("\n"), _T(" "));
		pDM->SetCellData(_T("UDF_��һ���"), strTmpData, nCurRow);
	}	
	
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_�켱�����ڷ�"));
	RemoveAliasList.Add(_T("�Ѱ���"));

	pDM->SetDataMgrRef(33, _T("��û����"), _T("APPLICANT_DATE"), _T("int"), _T(""), _T(""));
	pDM->StartFrame();
	
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		if(_T("BO_FURNISH_TBL") == pDM->TBL_NAME)
		{
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					INT nC, nCIdx;
					CString strColumnAlias;
					CString strFieldAlias, strFieldName, strFieldType, strFieldValue, strDataType;
					nC = pDM->GetColCount();						
					for(nCIdx = 0; nCIdx < nC; nCIdx++)
					{
						pDM->GetColumnRefInfo(nCIdx, strFieldAlias, strFieldName, strFieldType, strFieldValue, strDataType);
						if(strFieldName == _T("CIVIL_NO") || strFieldName == _T("ADDRESS") || strFieldName == _T("TEL") || strFieldName == _T("HANDPHONE") || strFieldName == _T("EMAIL"))
						{
							CStringArray DataArray;
							pDM->GetCellData(nCurRow, nCIdx, strFieldValue);
							DataArray.Add(strFieldValue);
							if(TRUE == pApi->ConnectSocket())
							{
								if(TRUE == pApi->UserInfoEncrytion(_T("BO_FURNISH_TBL"), strFieldName, DataArray))
								{
									pDM->SetCellData(nCurRow, nCIdx, DataArray.GetAt(0));
								}
								pApi->CloseSocket();
							}
							DataArray.RemoveAll(); DataArray.FreeExtra();
						}
					}
				}
				delete pApi;
				pApi = NULL;
			}
		}
	}

	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, m_idx, pDM, _T("REC_KEY"), _T("NUMERIC"), rec_key);
	ids = pDM->SendFrame();
	if( ids < 0 )	return -1001;
	pDM->EndFrame();
	pDM->SetDataMgrRef(33, _T("��û����"), _T("TO_CHAR(APPLICANT_DATE, 'YYYY/MM/DD HH24:MI:SS')"), _T("str"), _T(""), _T(""));
	pDM->SetCellData(_T("��û����"), strApplicantDateTime, nCurRow);

	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		if(_T("BO_FURNISH_TBL") == pDM->TBL_NAME)
		{
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					INT nC, nCIdx;
					CString strColumnAlias;
					CString strFieldAlias, strFieldName, strFieldType, strFieldValue, strDataType;
					nC = pDM->GetColCount();						
					for(nCIdx = 0; nCIdx < nC; nCIdx++)
					{
						pDM->GetColumnRefInfo(nCIdx, strFieldAlias, strFieldName, strFieldType, strFieldValue, strDataType);
						if(strFieldName == _T("CIVIL_NO") || strFieldName == _T("ADDRESS") || strFieldName == _T("TEL") || strFieldName == _T("HANDPHONE") || strFieldName == _T("EMAIL"))
						{
							CStringArray DataArray;
							pDM->GetCellData(nCurRow, nCIdx, strFieldValue);
							DataArray.Add(strFieldValue);
							if(TRUE == pApi->ConnectSocket())
							{
								if(TRUE == pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), strFieldName, DataArray))
								{
									pDM->SetCellData(nCurRow, nCIdx, DataArray.GetAt(0));
								}
								pApi->CloseSocket();
							}
							DataArray.RemoveAll(); DataArray.FreeExtra();
						}
					}
				}
				delete pApi;
				pApi = NULL;
			}
		}
	}
	 
	return 0;
}

INT CRequest_DataMgr::CopyCMToDM(CESL_ControlMgr *pCM, CESL_DataMgr *pDM, INT idx)
{
	INT ids, nCurRow, nCnt, i;
	CString strTmp;
	POSITION pos;
	CESL_Control_Element* pCE = NULL;
	
	if( idx == -1 ) 
	{
		ids = pDM->InsertRow(-1);
		if( ids != 0 )	return ids;
		nCurRow =  pDM->GetRowCount() - 1;
	}
	else 	nCurRow = idx;

	nCnt = pCM->Control_List.GetCount();
	pos = pCM->Control_List.GetHeadPosition();
	for( i=0; i<nCnt; i++ )
	{
		pCE = NULL;
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if( pCE == NULL )	continue;		

		ids = pCM->GetControlMgrData(pCE->GetAlias(), strTmp);
		if( ids < 0 )		continue;

		pDM->SetCellData(pCE->GetAlias(), strTmp, nCurRow);
	}

	return nCurRow;
}

void CRequest_DataMgr::OnchkREMARK() 
{
	( (CButton*) GetDlgItem(IDC_CMB_CANCELREASON) )->EnableWindow(TRUE);
	( (CEdit*) GetDlgItem(IDC_richREQ_CANCEL_REASON) )->EnableWindow(FALSE);
}

void CRequest_DataMgr::OnchkREMARK2() 
{
	( (CButton*) GetDlgItem(IDC_CMB_CANCELREASON) )->EnableWindow(FALSE);
	( (CEdit*) GetDlgItem(IDC_richREQ_CANCEL_REASON) )->EnableWindow(TRUE);
}

HBRUSH CRequest_DataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

void CRequest_DataMgr::OnSelchangecmbLasifyClass() 
{
	INT nDefaultValue, i, nCode = 256;
	CString class_name, strWhichCode, strDefaultValue;
	CString strCode[256], strDesc[256];

	CESL_ControlMultiComboBox* pCM_WhichCode = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("�з�ǥ����"));	
    CESL_ControlMultiComboBox* pCM_Subject = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("��������"));
	if( pCM_WhichCode == NULL || pCM_Subject == NULL )	return;

	pCM_WhichCode->GetData(strWhichCode, pCM_WhichCode->GetCurSel(), 1);
	strWhichCode.TrimLeft();		strWhichCode.TrimRight();
	strWhichCode.MakeUpper();
	
	if		( strWhichCode == _T("KDC") )	class_name = _T("KDC��������ȣ����");
	else if	( strWhichCode == _T("DDC") )	class_name = _T("DDC��������ȣ����");
	else if	( strWhichCode == _T("LC")	)	class_name = _T("LC������ȣ");
	else									class_name = _T("");

	pCM_Subject->FreeComboData();
	pCM_Subject->ResetContent();
	pCM_Subject->InitMultiComboBox(2);
	if( class_name.GetLength() == 0 )	return ;

	nDefaultValue = 0;
	m_pInfo->pCodeMgr->GetCode(class_name, strCode, strDesc, nCode, nDefaultValue);	
	if( nCode == 0 )	return ;

	if( m_pDM != NULL )	
	{
		m_pDM->GetCellData(_T("��������"), m_idx, strDefaultValue);

		strDefaultValue.Replace(_T(" "), _T(""));
		if(strDefaultValue.IsEmpty())
		{
			strDefaultValue = _T("  ");
		}
	}
	
	for( i=0; i<nCode; i++ )
	{
		if( strCode[i].GetLength() == 0 )	strCode[i] = _T("  ");
		
		if( strDefaultValue == strCode[i] ) 
		{
			if( SINSERT != m_nOpType )
			{
				nDefaultValue = i;
			}
		}
	
		pCM_Subject->AddItem(strCode[i], 0, i);
		pCM_Subject->AddItem(strDesc[i], 1, i);
	}

	pCM_Subject->SetCurSel(nDefaultValue);
	
}
