// SE_BINDING_DECISION_03.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_DECISION_03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_DECISION_03 dialog

CSE_BINDING_DECISION_03::CSE_BINDING_DECISION_03(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_nHeadPosition = m_nTailPosition = 0;
}

CSE_BINDING_DECISION_03::~CSE_BINDING_DECISION_03()
{
}

VOID CSE_BINDING_DECISION_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_DECISION_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_BINDING_DECISION_03, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_DECISION_03)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_DECISION_03 message handlers

BOOL CSE_BINDING_DECISION_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_DECISION_03")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
	
	INT ids = MoveIndex( _CURRENT );
	if( ids ) {
		AfxMessageBox (_T("MoveIndex() Error~!"));
		return FALSE ;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_BINDING_DECISION_03::InitCtrl()
{
	CString strTitle, strSize;

	// ��ǥ���� åũ��(������)���� �ѷ��ش�.
	GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("TITLE_INFO"), strTitle, m_nIdx );

	SetControlData( _T("CM_SE_BINDING_DECISION_03"), _T("TITLE"), strTitle );
	SetControlData( _T("CM_SE_BINDING_DECISION_03"), _T("BINDING_TITLE"), strTitle );
	SetControlData( _T("CM_SE_BINDING_DECISION_03"), _T("BINDING_UNIT_VALUE"), _T("") );
	SetControlData( _T("CM_SE_BINDING_DECISION_03"), _T("BINDING_PRICE"), _T("") );

}

VOID CSE_BINDING_DECISION_03::InitDisplay()
{
	INT ids, nRowCnt;
	
	CString ;
	
	CString strQuery = _T("");
	CString strTemp;


	// Parent�� RecKey�� �����´�.
	ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("REC_KEY"), m_sSpeciesKey, m_nIdx );

	strTemp.Format( _T(" SPECIES_KEY = %s "), m_sSpeciesKey );
	strQuery += strTemp;

    CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_03"));
	if( pDataMgr == NULL ) {
		AfxMessageBox( _T("[DM_SE_BINDING_DECISION_03] �� ã�� �� �����ϴ�! ") ) ;
	}

	pDataMgr->RefreshDataManager( strQuery );
	nRowCnt = pDataMgr->GetRowCount();	


	CString strUdfTitle;
	ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("TITLE_INFO"),strUdfTitle, m_nIdx );
	
	CESL_ControlMgr* pCM = FindCM(_T("CM_SE_BINDING_DECISION_03"));
	if(!pCM) {
		
		AfxMessageBox(_T("[CM_SE_BINDING_DECISION_03] �� ã�� �� �����ϴ�! ") ) ;
		return;
	}

	GetDlgItem(IDC_eTITLE)->SetWindowText(strUdfTitle);

	
	if( nRowCnt == 0 ) {
		// mode�� ���Ѵ�.
		m_nMode = INPUT_MODE;

		//�Է¸�忡���� �⺻���� ǥ�������� ������������ �Է����ش�
		pCM->SetControlMgrData(_T("BINDING_TITLE"),strUdfTitle);
	}
	else {
		// mode�� ���Ѵ�.
		m_nMode = UPDATE_MODE;
	}
	
	// ���������� �ѷ��ش�.
	if( nRowCnt > 0 ) {
		ids = AllControlDisplay( _T("CM_SE_BINDING_DECISION_03"), 0 );
		if( ids ) {
			AfxMessageBox( _T("AllControlDisplay Error!") );
			return;
		}
	}
}

VOID CSE_BINDING_DECISION_03::OnbSAVE() 
{
	INT ids;
	CString sBindUnit, sBindUnitValue, sBindingTitle, sBindPrice;

	CESL_DataMgr *pDataMgr = FindDM( _T("DM_SE_BINDING_DECISION_03") );
	if( !pDataMgr ) return;

	// Rec_key�� �����.
	if( m_nMode == INPUT_MODE ) {
		pDataMgr->MakeRecKey( m_sRecKey );
	}
	else {
		ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_03"), _T("REC_KEY"), m_sRecKey, 0 );
		if( ids ) return;
	}

	ids = GetControlData( _T("CM_SE_BINDING_DECISION_03"), _T("BINDING_UNIT")		, sBindUnit );
	if( ids ) return;
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_03"), _T("BINDING_UNIT_VALUE")	, sBindUnitValue );
	if( ids ) return;
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_03"), _T("BINDING_TITLE")		, sBindingTitle );
	if( ids ) return;
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_03"), _T("BINDING_PRICE")		, sBindPrice );
	if( ids ) return;

	// DEFAULT INPUT VALUE CHECK
	if( sBindUnitValue.IsEmpty() ) {
		AfxMessageBox( _T("�������� �Է����ּ���.") );
		return ;
	}
	

	// BINDING_UNIT_TYPE CHECK
	ids = CheckBindingUnitType();
	if( ids == -100 ) {
		AfxMessageBox( _T("2å �̻��̾�� �մϴ�.\n\n�ٽ� �Է����ּ���.") );
		return ;
	}
	if( ids == -200 ) {
		AfxMessageBox( _T("�ش� ����󵵿� �������� �������� ���մϴ�.\n\n�ٽ� �Է����ּ���.") );
		return ;
	}
	if( ids < 0 ) {
		AfxMessageBox( _T("Type Check Error!") );
		return ;
	}

    pDataMgr->StartFrame();

	// Query DB Field�� �ʱ�ȭ �Ѵ�.
	pDataMgr->InitDBFieldData();

	// Query �� DB Field�� �߰��Ѵ�.
	ids = pDataMgr->AddDBFieldData( _T("REC_KEY")				, _T("NUMERIC")	, m_sRecKey);
	if(ids < 0) return;
	ids = pDataMgr->AddDBFieldData( _T("SPECIES_KEY")			, _T("NUMERIC")	, m_sSpeciesKey);
	if(ids < 0) return;
	ids = pDataMgr->AddDBFieldData( _T("BINDING_UNIT")			, _T("STRING")	, sBindUnit);
	if(ids < 0) return;
	ids = pDataMgr->AddDBFieldData( _T("BINDING_UNIT_VALUE")	, _T("NUMERIC")	, sBindUnitValue);
	if(ids < 0) return;
	ids = pDataMgr->AddDBFieldData( _T("BINDING_TITLE")			, _T("STRING")	, sBindingTitle);
	if(ids < 0) return;
	ids = pDataMgr->AddDBFieldData( _T("BINDING_PRICE")			, _T("STRING")	, sBindPrice);
	if(ids < 0) return;

	if( m_nMode == INPUT_MODE )	{
		ids = pDataMgr->MakeInsertFrame( _T("SE_BINDING_INFO_TBL") );
		if(ids < 0) return;
	}
	else {
		ids = pDataMgr->MakeUpdateFrame( _T("SE_BINDING_INFO_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_sRecKey );
		if(ids < 0) return;
	}

	ids = pDataMgr->SendFrame();
	if(ids < 0) return;

	pDataMgr->EndFrame();

	AfxMessageBox( _T("����Ǿ����ϴ�.") );
}

INT CSE_BINDING_DECISION_03::CheckBindingUnitType()
{
	INT ids;

	CString sBindingUnit, sBindingUnitValue, sFreq;

	// compare freq
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_03"), _T("BINDING_UNIT")		, sBindingUnit );
	if( ids ) return-1;

	ids = GetControlData( _T("CM_SE_BINDING_DECISION_03"), _T("BINDING_UNIT_VALUE")	, sBindingUnitValue );
	if( ids ) return-1;

	ids = GetDataMgrData( _T("DM_SE_BINDING_DECISION_01"), _T("PUB_FREQ")			, sFreq, m_nIdx );
	if( ids ) return-1;

	if( !sBindingUnit.Compare(_T("å(�μ�)")) ) {
		if( _ttoi( (TCHAR*)LPCTSTR(sBindingUnitValue) ) < 2 )		
			return -100;
	}
	else if( !sBindingUnit.Compare(_T("�ִ���")) ) {
		if( sFreq == 'c' || sFreq == 'd' || sFreq == 'i');
		else
			return -200;
	}
	else if( !sBindingUnit.Compare(_T("������")) ) {
		if( sFreq == 'c' || sFreq == 'd' || sFreq == 'i' ||
			sFreq == 'w' || sFreq == 'e' || sFreq == 'j' || sFreq == 's'
			);
		else 
			return -200;
	}

	return 0;
}

VOID CSE_BINDING_DECISION_03::OnbPREV()
{
	INT ids = MoveIndex(_PREV);
	if( ids ) return;
}

VOID CSE_BINDING_DECISION_03::OnbNEXT()
{
	INT ids = MoveIndex(_NEXT);
	if( ids ) return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// [PURPOSE]
// �⺻ȭ�� ���������� ���� �� ����
//
// [MINISPEC]
// 1. �⺻ȭ���� index�� ���� ó���� �Ѵ�.
// 2. ���������� ǥ���Ѵ�.
// 3. ����,���� ��ư�� Enable/Diable
// 4. �̵��� ���ڵ� �ε����� ���Ѵ�.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
INT CSE_BINDING_DECISION_03::MoveIndex(TCHAR to)
{
    INT          nRowIndex, ids;
	
	CESL_ControlMgr *pCM		= FindCM( _T("CM_SE_BINDING_DECISION_01") );
	CESL_Grid		*pGridMgr	= (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. �⺻ȭ���� index�� ���� ó���� �Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    switch( to ) {
        case _NOMOVE:
            nRowIndex = pGridMgr->SelectGetIdx( );
            if ( nRowIndex < 0 ) return -2;

			if( m_nHeadPosition == nRowIndex ) GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bPREV)->EnableWindow(TRUE);	
			if( m_nTailPosition == nRowIndex ) GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bNEXT)->EnableWindow(TRUE);	
            break;
        case _CURRENT:
			ids = pGridMgr->SelectMakeList();
            if ( ids ) return -3;

			m_nTailPosition = pGridMgr->SelectGetTailPosition();
			m_nHeadPosition = pGridMgr->SelectGetHeadPosition();
            
			if( m_nHeadPosition == m_nTailPosition )GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
			GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
            break;
        case _PREV:
            nRowIndex = pGridMgr->SelectGetPrev( );
            if ( nRowIndex < 0 ) return -4;

			if( m_nHeadPosition == nRowIndex ) GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bPREV)->EnableWindow(TRUE);	
			if( m_nTailPosition == nRowIndex ) GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bNEXT)->EnableWindow(TRUE);	
            break;
        case _NEXT:
            nRowIndex = pGridMgr->SelectGetNext( );
            if ( nRowIndex < 0 ) return -4;

			if( m_nHeadPosition == nRowIndex ) GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bPREV)->EnableWindow(TRUE);	
			if( m_nTailPosition == nRowIndex ) GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
			else					  		   GetDlgItem(IDC_bNEXT)->EnableWindow(TRUE);	
            break;
        default:
            return -6;
            break;
    }

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. ���������� ǥ���Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //pGridMgr->SetListIndexSelection( );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 3. ����,���� ��ư�� Enable/Diable
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Prev_Next_ButtonStatus( _T("DISP_BINDING_FORMAT"), m_pParCtrlAlias, m_pParGridAlias, IDC_bPREV, IDC_bNEXT);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 4. �̵��� ���ڵ� �ε����� ���Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_nIdx = pGridMgr->SelectGetIdx();
    if ( m_nIdx < 0 ) return -8;

	InitDisplay();

	//parent grid set reverse
	pGridMgr->SetReverse(m_nIdx);

    return 0;
}


HBRUSH CSE_BINDING_DECISION_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
