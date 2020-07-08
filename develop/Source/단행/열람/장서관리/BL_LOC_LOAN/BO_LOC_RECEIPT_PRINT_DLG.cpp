// BO_LOC_RECEIPT_PRINT_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_RECEIPT_PRINT_DLG.h"
#include "..\..\..\..\����\K2UP_Common\MakeSearchData.h"
#include "lprn_api.h"
#include "RECEIPT_PRINT_SET_DLG.h"
#include "RECEIPT_SERARCH_DLG.h"
#include "BO_LOC_3000Api.h"
// 2006.05.26 ADD BY PDW(Jr)
// XML �������� ���� �۾�
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_RECEIPT_PRINT_DLG dialog


CBO_LOC_RECEIPT_PRINT_DLG::CBO_LOC_RECEIPT_PRINT_DLG(INT nDialogType, CESL_Mgr* pParent, CLonaManageValue *pLMV/*=NULL*/)
	: CESL_Mgr(CBO_LOC_RECEIPT_PRINT_DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_RECEIPT_PRINT_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentMgr = pParent;
	m_nDialogType = nDialogType;	
	m_pLMV = pLMV;
	m_PreviewDlg = NULL;
	m_strLibCode = _T("");
	if(NULL != m_pLMV)
	{
		// 2008.04.14 UPDATE BY PWR : �������� �߰� m_strNotify
		m_pLMV->GetReceiptManageValue(m_strBarcode, m_strBarcodeAll, m_strItem, m_strMode, m_strDevice, m_strPreview, m_strNotify);	
		m_pLMV->GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), m_strCivilNoViewYN);
		m_pLMV->GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), m_sIsUnityLoanService);
	}
	else
	{
		GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), m_strCivilNoViewYN);
		GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), m_sIsUnityLoanService);
	}
	if(_T("Y") != m_strCivilNoViewYN) m_strCivilNoViewYN = _T("N");
	if(_T("Y") != m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");
	// ���, ��¾��� ���а� �߰�
	m_strPrintYN = _T("Y");
}

CBO_LOC_RECEIPT_PRINT_DLG::~CBO_LOC_RECEIPT_PRINT_DLG()
{
	if ( m_PreviewDlg != NULL ) delete m_PreviewDlg;


}


VOID CBO_LOC_RECEIPT_PRINT_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_RECEIPT_PRINT_DLG)
	DDX_Control(pDX, IDC_TAB_LOAN1, m_tab);
	DDX_Control(pDX, IDC_TAB_LOAN2, m_tab2);
	//}}AFX_DATA_MAP
}


BOOL CBO_LOC_RECEIPT_PRINT_DLG::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	m_bPrintYN = TRUE;
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BEGIN_MESSAGE_MAP(CBO_LOC_RECEIPT_PRINT_DLG, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_RECEIPT_PRINT_DLG)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnPRINT_RECEIPT, OnbtnPRINTRECEIPT)
	ON_BN_CLICKED(IDC_btnLocloanCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_radLOAN, OnRadSelChange)
	ON_BN_CLICKED(IDC_radRETURN, OnRadSelChange)
	ON_BN_CLICKED(IDC_btnPREVIEW, OnbtnPREVIEW)
	ON_BN_CLICKED(IDC_btnSETUP, OnbtnSETUP)	
	ON_BN_CLICKED(IDC_btnSEARCH, OnbtnSEARCH)
	ON_BN_CLICKED(IDC_btnDATE_SEARCH, OnbtnDATE_SEARCH)
	ON_BN_CLICKED(IDC_btnPRINT_CANCEL, OnbtnPRINTCANCEL)			
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LOAN1, OnSelchangeTab1 )
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LOAN2, OnSelchangeTab2 )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_RECEIPT_PRINT_DLG message handlers

BOOL CBO_LOC_RECEIPT_PRINT_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 2007.05.08 ADD BY PDW
	// �����ڷ� �������� ���
	if( 3 == m_nDialogType )
	{
		return TRUE;
	}
	// ���̾�α׸� ǥ������ �ʴ� ���
	if(-1 == m_nDialogType)
	{
		return TRUE;
	}
	
	INT ids = 0;
	CString sErrMsg = _T("");

	if(InitESL_Mgr(_T("BO_LOC_3100_RECEIPT")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	SetWindowText( _T("������ ���") );

	(( CButton*) GetDlgItem (IDC_radLOAN))->SetCheck(1);

	//++2008.04.14 ADD BY PWR {{++
	// ��¥��Ʈ�� �����ϰ˻����� �����Ѵ�.
	SetDateControl();
	//--2008.04.14 ADD BY PWR --}}
		
	ids = initRctUseType( );
	if ( ids ) return FALSE;

	OnRadSelChange();

	// 17/04/20 �ڴ�� : �߶��������������� �����ġ - �ߺ�����
/*//REM--------------------------------------------------------------------------
	ids = initData( );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("[%d]initData( ) Fail"), ids );
		AfxMessageBox( sErrMsg );
		return FALSE;
	}			
*///END--------------------------------------------------------------------------
	
	//++2006.05.26 ADD BY PDW {{++
	// XML �۾� 
	ids = InitXML_Style();
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("[%d]InitXML_Style( ) Fail"), ids );
		AfxMessageBox( sErrMsg );
		return FALSE;
	}
	//--2006.05.26 ADD BY PDW {{--

	//JOB.2020.0030 ADD BY KYJ
	CESL_Grid* pRctGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT2));
	CESL_Grid* pRctTotalGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT3));
	
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pRctGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
		pRctTotalGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pRctGrid->m_bPrivacyPrintLogRecord = TRUE;
		pRctGrid->m_strPrintInfo = _T("��µȿ���������Ʈ");
		pRctTotalGrid->m_bPrivacyPrintLogRecord = TRUE;
		pRctTotalGrid->m_strPrintInfo = _T("����¿���������Ʈ");
	}
	
	if( m_pLMV->m_sLoanUserAccessMode.IsEmpty() || m_pLMV->m_sLoanUserAccessMode == _T("1") )
	{
		pRctGrid->SetColumnEncryption(_T("ȸ����"),TRUE);
		pRctTotalGrid->SetColumnEncryption(_T("�̸�"),TRUE);
	}


	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

// **���ο��� ���õ� ����ڿ� ���ؼ� ����Ȯ���� ������� �ʱ�ȭ�� �Ѵ�.
INT CBO_LOC_RECEIPT_PRINT_DLG::initData(BOOL bAllDisplay)
{
	CString sValue = _T("");
	CString sErrMsg = _T("");
	CString sWhere = _T("");
	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sTotalReceiptDMAlias = _T("DM_BO_LOC_3100_TOTAL_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
	CString sReceiptLogDMAlias = _T("DM_BO_LOAN_RECEIPT_LOG");
	CString strLoanCondition;
	CString strLoanExtraCondition;
	CString strReturnCondition;
	CString strReturnExtraCondition;			
	CString strReturnSelect;
	
	INT		nIdx;
	INT		nIdx2;	
	INT		ids = 0 ;

	if ( GetSafeHwnd() == NULL )
	{
		return 0;
	}
	
	// 17/04/20 �ڴ�� : �߶��������������� �����ġ - �ߺ�����
//*/ ADD ---------------------------------------------------------------------------
	if (!IsWindowVisible())
	{
		return 0;
	}
//*/ END ---------------------------------------------------------------------------
	
	nIdx2 = GetTabCtlIdx(1);
	CESL_DataMgr* pTotalDM = ( CESL_DataMgr* ) FindDM( sTotalReceiptDMAlias );
	
	if ( !pTotalDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::initData()") );

	//++2005.12.28 UPDATE BY PDW {{++
    // �а��� ���� �˻� ���� ����
	// �α��� ������� �������а� �����̷��� ���� ������ �����͸��� �����ش�.
	CString sManageCode;
	ids = GetLoginerManageCode( sManageCode );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("CBO_LOC_RECEIPT_PRINT_DLG::initData()") );

	// �߰��� EXTRA ����
	CString sExtraManageCodeQuery = _T(" AND CL.MANAGE_CODE = '") + sManageCode + _T("'");
	//++2008.04.14 UPDATE BY PWR {{++
	// ��¥ ��Ʈ�� �߰��Ͽ� ��¥��Ʈ���� ���� ���� �˻� �ǵ��� WHERE�߰�
	// "��ü���������"�� ���Լ�( initData() )����
	// "�������������", "��µȿ������̷�"�� ����/�ݳ����� getReceiptWhere()����
	// "��µȿ������̷�"�� RECEIPT_LOG���̺� �� getReceiptLogWhere()���� ��¥ ���� �߰�
	//
	// ��Ʈ���� ��¥ ���� �����´�.
	GetDateControl();
	// WHERE ����
	CString sLoanDateWhere = _T("");
	CString sReturnDateWhere = _T("");
	// 1. To, From �Ѵ� �Էµ� ���� 
	if( !m_sDateTo.IsEmpty() && !m_sDateFrom.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateTo, m_sDateFrom );
		sReturnDateWhere.Format(_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateTo, m_sDateFrom );
	}
	// 2. To�� �Էµ� ����
	else if( !m_sDateTo.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE")
								, m_sDateTo );
		sReturnDateWhere.Format(_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE")
								, m_sDateTo );
		
	}
	// 3. From�� �Էµ� ����
	else if( !m_sDateFrom.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND CL.LOAN_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateFrom );
		sReturnDateWhere.Format(_T(" AND CL.RETURN_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateFrom );
	}

	// ���� (�߰� EXTRA ����)
	strLoanCondition		= _T("BB.REC_KEY = CL.BOOK_KEY AND CLU.REC_KEY = CL.USER_KEY AND CL.RECEIPT_PRINT_LOAN_YN='N' AND LOAN_DATE IS NOT NULL") + sExtraManageCodeQuery + sLoanDateWhere;
	strLoanExtraCondition	= _T("GROUP BY CLU.NAME,CLU.USER_NO, CLU.REC_KEY,TO_CHAR(CL.RETURN_DATE, 'YYYY/MM/DD'),TO_CHAR(CL.LOAN_DATE, 'YYYY/MM/DD')");
	strReturnCondition		= _T("BB.REC_KEY = CL.BOOK_KEY AND CLU.REC_KEY = CL.USER_KEY AND CL.RECEIPT_PRINT_RETURN_YN='N' AND RETURN_DATE IS NOT NULL") + sExtraManageCodeQuery + sReturnDateWhere;
	strReturnExtraCondition = _T("GROUP BY CLU.NAME, CLU.USER_NO, CLU.REC_KEY, TO_CHAR(CL.RETURN_DATE, 'YYYY/MM/DD'), TO_CHAR(CL.LOAN_DATE, 'YYYY/MM/DD')");			
	strReturnSelect			= _T("UNION ALL SELECT '�ݳ�',CLU.NAME, TO_CHAR(CL.LOAN_DATE, 'YYYY/MM/DD'),TO_CHAR(CL.RETURN_DATE, 'YYYY/MM/DD'),CLU.REC_KEY,CLU.USER_NO FROM BO_BOOK_TBL BB,LH_STORAGE_V01 CL,CO_LOAN_USER_TBL CLU ");
	//--2008.04.14 UPDATE BY PWR --}}
	//--2005.12.28 UPDATE BY PDW --}}

	if ( nIdx2 == 2 && pTotalDM->GetRowCount() == 0 )
	{		
		// ���� / �ݳ� ��� �ɼǿ� ���� select		
		if ( m_tab.GetItemCount() == 2 )
		{
			// ��ü���
			pTotalDM->CONDITION = strLoanCondition;
			pTotalDM->EXTRA_CONDITION = strLoanExtraCondition + strReturnSelect + _T(" WHERE ") + strReturnCondition + _T(" ") + strReturnExtraCondition;
			pTotalDM->SetDataMgrRef(0,_T("�������"), _T("DECODE('�ݳ�','�ݳ�','����')"), _T("str"),_T(""),_T(""));
			
			ids = pTotalDM->RefreshDataManager(_T(""));
		}
		else
		{
			CString strFieldName, strFieldType;					
			if ( GetTabCtlIdx(0) == 0 )
			{				
				// ���⸸�̸�	
				// condition default�� �����̹Ƿ� ����.
				pTotalDM->CONDITION = strLoanCondition;
				pTotalDM->EXTRA_CONDITION = strLoanExtraCondition;
				pTotalDM->SetDataMgrRef(0,_T("�������"), _T("'����'"), _T("str"),_T(""),_T(""));				
			}
			else
			{
				// �ݳ����̸�
				pTotalDM->CONDITION = strReturnCondition;
				pTotalDM->EXTRA_CONDITION = strReturnExtraCondition;
			  	pTotalDM->SetDataMgrRef(0,_T("�������"), _T("'�ݳ�'"), _T("str"),_T(""),_T(""));
			}
			ids = pTotalDM->RefreshDataManager(_T(""));			
			if ( ids < 0 ) return ids;
		}
	}
	else if ( nIdx2 != 2 )
	{
		ids = pTotalDM->FreeData();
		if ( ids < 0 ) return ids;
	}	

	nIdx = GetTabCtlIdx(0);
	
	ids = getReceiptWhere( sWhere, nIdx );
	if ( ids > 0 )
	{
		// ȭ�� �ʱ�ȭ
		ClearData();	
	}
	if ( ids != 0 ) return (ids);	

	if ( !sWhere.IsEmpty() )
	{
		ids = RefreshDataMgr( sReceiptDMAlias, sWhere );
		if ( ids < 0 ) return -2;	
	}

	// �̷��� ��� ������ ��ȣ �߽����� �����ش�.
	// BOOL bCheck;
	// bCheck = (( CButton*) GetDlgItem (IDC_radLOAN))->GetCheck();	
	if ( nIdx2 == 0 )
	{
		CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( sReceiptLogDMAlias );
		if( pRctLogDM == NULL ) return -110;
		pRctLogDM->FreeData();
	}
	else if ( nIdx2 == 1 )
	{	
		ids = getReceiptLogWhere( sWhere, nIdx );		
		if( ids ) return ids;
		ids = RefreshDataMgr( sReceiptLogDMAlias, sWhere );
		if( ids ) return ids;

		// 2011.08.31 ADD : 3000Api
		CString strEnc;
		CStringArray asDes1;

		// 2008.04.14 ADD BY PWR : �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ��
		CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( sReceiptLogDMAlias );
		if( pRctLogDM == NULL ) return -110;
		for( INT i = 0; i < pRctLogDM->GetRowCount(); i++ )
		{
			if(_T("Y") == m_strCivilNoViewYN && FALSE == m_pInfo->m_bLibSecuUse) break;
			if(_T("Y") != m_strCivilNoViewYN)
			{
				CString sData;
				pRctLogDM->GetCellData( _T("ȸ����ȣ"), i, sData );
				ChangeSecuriyCivilNo( sData );
				pRctLogDM->SetCellData( _T("ȸ����ȣ"), sData, i );
			}
			// 2011.08.31 ADD : 3000Api
			if(TRUE == m_pInfo->m_bLibSecuUse)
			{
				pRctLogDM->GetCellData(_T("�ֹι�ȣ"), i, strEnc);
				asDes1.Add(strEnc);
			}
		}

		// 2011.08.31 ADD : 3000Api
		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					for(i = 0; i < pRctLogDM->GetRowCount(); i++)
					{
						pRctLogDM->SetCellData(_T("�ֹι�ȣ"), asDes1.GetAt(i), i);
					}
					pApi->CloseSocket();	
				}
				delete pApi;
				pApi = NULL;
			}
			asDes1.RemoveAll(); asDes1.FreeExtra();
		}
	}
	
	ids = initControl(bAllDisplay);
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("[%d]initControl( ) Fail"), ids );
		AfxMessageBox( sErrMsg );
	}

	if ( 0 !=ids ) OnCancel( );

	
	INT		nRowCount;
	INT		i;

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return -100;

	CESL_ControlMgr *pCM = FindCM( sReceiptCMAlias );
	if( pCM == NULL ) return -110;
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( _T("gridReceipt") );	
	if( pGrid == NULL ) return -120;
	
	//if ( nIdx == 0 && nIdx2 != 2)
	if ( nIdx2 == 0 )
	{
		nRowCount = pRctDM->GetRowCount( );
		for ( i=0 ; i<nRowCount ; i++ )
		{
			pGrid->SetTextMatrix( i+1, 1, _T("V") );

		}
	}
	else if ( nIdx2 == 1 )
	{
		ClickLog(0);
	}
	
	return 0;
}

// **ȭ�鿡 ������ ������ ������ ��� �ڷḦ �����ֵ��� �ʱ�ȭ�Ѵ�.
INT CBO_LOC_RECEIPT_PRINT_DLG::initControl(BOOL bAllDisplay)
{
	INT ids = 0;

	CString strLoanType;	
	CString sUserName = _T("");
	CString sUserNo = _T("");
	CString sCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
	INT		nUserIndex;

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�������̸�"), sUserName, 0 );
	if ( ids < 0 )
		return -1;

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����ڹ�ȣ"), sUserNo, 0 );
	if ( ids < 0 ) return -2;
	//++2008.04.14 ADD BY PWR {{++
	// �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ�� 
	ChangeSecuriyCivilNo( sUserNo );
	//--2008.04.14 ADD BY PWR --}}
	
	GetDlgItem( IDC_edtUSER_NO )->SetWindowText( sUserNo );
	GetDlgItem( IDC_edtUSER_NAME )->SetWindowText( sUserName );		

	if ( bAllDisplay == TRUE )
	{
		ids = AllControlDisplay( sCMAlias );
		if ( ids < 0 ) return -3;
	}
	else
	{
		CESL_Grid* pGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT3));		
		nUserIndex = pGrid->GetRowSel()-1;

		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("�������"), strLoanType,  nUserIndex);
		if ( ids < 0 ) { return -1; }		

		CESL_ControlMgr *pCM = FindCM( sCMAlias );
		if( pCM == NULL ) return -110;
		ids = pCM->ControlDisplay(_T("gridReceipt"), 0 );
		if ( ids < 0 ) return ids;				
	}

	if ( m_tab2.GetCurSel() == 2 )
	{
		if ( strLoanType == _T("����") ) 
		{ 
			m_tab.SetCurSel(0);
		}
		else
		{	
			m_tab.SetCurSel(1);
		}
	}

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::initRctUseType()
{
	CString sRctUseType = _T("");

	sRctUseType = m_pLMV->m_sUseReceiptType;

	if ( _T("YY") != sRctUseType )
	{
		if ( 'Y' == sRctUseType.GetAt(0) )	m_tab.InsertItem( 0, _T(" ���� ") );
		else m_tab.InsertItem( 0, _T(" �ݳ� ") );
	}
	else
	{
		m_tab.InsertItem( 0, _T(" ���� ") );
		m_tab.InsertItem( 1, _T(" �ݳ� ") );
	}

	// ����/�ݳ��̸�
	if ( m_nDialogType == 0 )
	{
		m_tab2.InsertItem( 0, _T(" ������������� ") );
		m_tab2.InsertItem( 1, _T(" ��µȿ������̷� ") );
		m_tab2.InsertItem( 3, _T(" ��ü��������� ") );
	}
	// �����ڷ�����̸� 
	// �̷¸� �����Ѵ�.
	else
	{		
		m_tab2.InsertItem( 0, _T(" ��µȿ������̷� ") );	
	}	

	return 0;
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
}

// **ũ�� �����Լ�
VOID CBO_LOC_RECEIPT_PRINT_DLG::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CRect gRect;
	CRect gRect2;
	CRect gRect3;
	CRect gRect4;
	INT	  nIdx;
	
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridRECEIPT);
	CMSHFlexGrid* pGrid2 = (CMSHFlexGrid*)GetDlgItem(IDC_gridRECEIPT2);
	
	if(pGrid->GetSafeHwnd() == NULL){
		return;
	}

	pGrid->GetWindowRect( &gRect );
	m_tab.GetWindowRect( &gRect2 );

	pGrid2->GetWindowRect( &gRect3 );
	m_tab2.GetWindowRect( &gRect4 );

	ScreenToClient( gRect );
	ScreenToClient( gRect2 );

	ScreenToClient( gRect3 );
	ScreenToClient( gRect4 );

	nIdx = GetTabCtlIdx(1);

	if ( nIdx == 0 )
	{
		pGrid->MoveWindow(gRect.left, gRect.top, (cx-12), (cy - gRect.top)-11 );
		m_tab.MoveWindow(gRect2.left, gRect2.top, (cx-9), (cy - gRect2.top)-4 );		
		m_tab2.MoveWindow(gRect4.left, gRect4.top, (cx-3), (cy - gRect4.top) );
	}
	else
	{
		pGrid->MoveWindow(gRect.left, gRect.top, (cx-6), (cy - gRect.top)-5 );
		m_tab.MoveWindow(gRect2.left, gRect2.top, (cx-3), (cy - gRect2.top)-2 );
		
		pGrid2->MoveWindow(gRect3.left, gRect3.top, (cx-6), gRect3.bottom-gRect3.top );
		if ( (cx) >= 415 )
		{		
			m_tab2.MoveWindow(gRect4.left, gRect4.top, (cx-3), gRect3.bottom-gRect3.top+25 );
		}
	}
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnRadSelChange() 
{	
	INT	nIdx;			

	nIdx = GetTabCtlIdx(1);

	if ( nIdx == 0 )
	{
		((CButton*) GetDlgItem(IDC_btnSEARCH))->EnableWindow(FALSE);
		((CButton*) GetDlgItem(IDC_radLOAN))->SetCheck(1);
		((CButton*) GetDlgItem(IDC_radRETURN))->SetCheck(0);
		((CButton*) GetDlgItem(IDC_btnPRINT_CANCEL))->EnableWindow(TRUE);
		m_tab.EnableWindow(TRUE);
	}
	else if ( nIdx == 1 )
	{		
		((CButton*) GetDlgItem(IDC_btnSEARCH))->EnableWindow(TRUE);
		((CButton*) GetDlgItem(IDC_radLOAN))->SetCheck(0);
		((CButton*) GetDlgItem(IDC_radRETURN))->SetCheck(1);
		((CButton*) GetDlgItem(IDC_btnPRINT_CANCEL))->EnableWindow(FALSE);
		m_tab.EnableWindow(TRUE);
	}
	else
	{
		((CButton*) GetDlgItem(IDC_btnPRINT_CANCEL))->EnableWindow(TRUE);
		((CButton*) GetDlgItem(IDC_btnSEARCH))->EnableWindow(FALSE);
		m_tab.EnableWindow(FALSE);
		//((CButton*) GetDlgItem(IDC_radLOAN))->SetCheck(1);
		//((CButton*) GetDlgItem(IDC_radRETURN))->SetCheck(0);
	}
	/*	
	BOOL bCheck;
	bCheck = (( CButton*) GetDlgItem (IDC_radLOAN))->GetCheck();
	*/
	//if ( bCheck == TRUE )

	CESL_Grid* pRctGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT2));
	CESL_Grid* pRctTotalGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT3));
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridRECEIPT);

	CRect rect,rect1,rect2;	
	GetWindowRect(rect);
	m_tab.GetWindowRect(rect1);
	pGrid->GetWindowRect(rect2);	
	ScreenToClient(&rect1);
	ScreenToClient(&rect2);

	DWORD CheckLogGrid;
	DWORD chkTotalGrid;
	CheckLogGrid = ( pRctGrid->GetStyle() & WS_VISIBLE );
	chkTotalGrid = ( pRctTotalGrid->GetStyle() & WS_VISIBLE );

	if ( nIdx == 0 )
	{		
		pRctGrid->ShowWindow(SW_HIDE);
		pRctTotalGrid->ShowWindow(SW_HIDE);	
		m_tab.ShowWindow(SW_SHOW);
		
		//( (CStatic*) GetDlgItem (IDC_STATIC1))->ShowWindow(SW_SHOW);
		//( (CStatic*) GetDlgItem (IDC_STATIC2))->ShowWindow(SW_SHOW);
		//( (CStatic*) GetDlgItem (IDC_STATIC3))->ShowWindow(SW_SHOW);
		//( (CEdit*) GetDlgItem (IDC_edtUSER_NO))->ShowWindow(SW_SHOW);
		//( (CEdit*) GetDlgItem (IDC_edtUSER_NAME))->ShowWindow(SW_SHOW);
		
	}
	else if ( nIdx == 1 )
	{
		m_tab.ShowWindow(SW_SHOW);
		pRctGrid->ShowWindow(SW_SHOW);
		pRctTotalGrid->ShowWindow(SW_HIDE);
		m_tab.ShowWindow(SW_SHOW);			

		( (CStatic*) GetDlgItem (IDC_STATIC1))->ShowWindow(SW_HIDE);
		( (CStatic*) GetDlgItem (IDC_STATIC2))->ShowWindow(SW_HIDE);
		( (CStatic*) GetDlgItem (IDC_STATIC3))->ShowWindow(SW_HIDE);
		( (CEdit*) GetDlgItem (IDC_edtUSER_NO))->ShowWindow(SW_HIDE);
		( (CEdit*) GetDlgItem (IDC_edtUSER_NAME))->ShowWindow(SW_HIDE);
	}
	else 
	{
		pRctTotalGrid->ShowWindow(SW_SHOW);		
		pRctGrid->ShowWindow(SW_HIDE);	
		m_tab.ShowWindow(SW_SHOW);

		( (CStatic*) GetDlgItem (IDC_STATIC1))->ShowWindow(SW_HIDE);
		( (CStatic*) GetDlgItem (IDC_STATIC2))->ShowWindow(SW_HIDE);
		( (CStatic*) GetDlgItem (IDC_STATIC3))->ShowWindow(SW_HIDE);
		( (CEdit*) GetDlgItem (IDC_edtUSER_NO))->ShowWindow(SW_HIDE);
		( (CEdit*) GetDlgItem (IDC_edtUSER_NAME))->ShowWindow(SW_HIDE);
	}
	
	
	if ( nIdx == 0  )
	{
		// ����ݳ� Ȯ��â�� 
		if ( CheckLogGrid || chkTotalGrid )
		{
			MoveWindow(rect.left, rect.top, rect.Width(), rect.Height()-123, TRUE);
			m_tab.MoveWindow(rect1.left+3, rect1.top-145, rect1.Width()-6, rect1.Height()+18, TRUE);
			pGrid->MoveWindow(rect2.left+3, rect2.top-145, rect2.Width()-6, rect2.Height()+18, TRUE);
			( (CStatic*) GetDlgItem (IDC_STATIC1))->ShowWindow(SW_HIDE);
			( (CStatic*) GetDlgItem (IDC_STATIC2))->ShowWindow(SW_HIDE);
			( (CStatic*) GetDlgItem (IDC_STATIC3))->ShowWindow(SW_HIDE);
			( (CEdit*) GetDlgItem (IDC_edtUSER_NO))->ShowWindow(SW_HIDE);
			( (CEdit*) GetDlgItem (IDC_edtUSER_NAME))->ShowWindow(SW_HIDE);
		}
	}
	else if ( nIdx == 1 )
	{		
		if ( !CheckLogGrid  && !chkTotalGrid)
		{
			MoveWindow(rect.left, rect.top, rect.Width(), rect.Height()+123, TRUE);
			m_tab.MoveWindow(rect1.left-3, rect1.top+145, rect1.Width()+6, rect1.Height()-18, TRUE);
			pGrid->MoveWindow(rect2.left-3, rect2.top+145, rect2.Width()+6, rect2.Height()-18, TRUE);
		}
	}		
	else
	{
		if ( !CheckLogGrid  && !chkTotalGrid)
		{
			MoveWindow(rect.left, rect.top, rect.Width(), rect.Height()+123, TRUE);
			m_tab.MoveWindow(rect1.left-3, rect1.top+145, rect1.Width()+6, rect1.Height()-18, TRUE);
			pGrid->MoveWindow(rect2.left-3, rect2.top+145, rect2.Width()+6, rect2.Height()-18, TRUE);
		}
	}
	
	if ( nIdx == 1 || nIdx == 2 )
	{
		pGrid->SetColWidth(1,0,0);		
	}
	else
	{
		pGrid->SetColWidth(1,0,500);
	}


	initData( );
}

// **�������� ����Ѵ�.
VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnPRINTRECEIPT() 
{
	INT ids = 0;
	
	//++2008.04.14 ADD BY PWR {{++
	// ���, ��¾��� ���а� �߰�
	m_strPrintYN = _T("Y");
	//--2008.04.14 ADD BY PWR --}}

	if ( m_tab2.GetCurSel() == 2 )
	{
		//++2008.05.13 ADD BY PWR {{++
		// ������ ����Ÿ ������ ��� ���ϵ��� ����
		INT		nRowCount;
		CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( _T("CM_BO_LOC_3100_RECEIPT"), _T("GridTotalReceipt") );
		if ( NULL == pRctGrid ) return;
		pRctGrid->SelectMakeList();	
		nRowCount = pRctGrid->SelectGetCount();
		if ( nRowCount <= 0 )
		{
			AfxMessageBox( _T("������ �ڷᰡ �����ϴ�.") );
			return;
		}
		//--2008.05.13 ADD BY PWR --}}

		ids = doPrintReceiptSelectAll();
	}
	else
	{
		ids = doPrintReceipt( );
		if ( ids < 0 ) OnCancel( );
		else if ( 0 != ids ) return;
	}
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::BarcodePrintCommand(INT nType)
{
	// �̸������ �Ǻ���½ÿ��� �����ֵ��� ��.
	// ���� ��½� �����Ͱ� �����Ͽ� ȭ�� ������ �ٷιٷ� �ȵ�	
	if ( nType == 0 )
	{	
		//tempPreviewFlag = m_strPreview;
		//m_strPreview = _T("N");
		ReceiptAutoPrint(TRUE);		
		//m_strPreview = tempPreviewFlag;
	}
	else if ( nType == 1 )
	{
		ReceiptAutoPrint(TRUE);
	}
	else 
	{
		m_tab2.SetCurSel(2);
		OnRadSelChange();
		doPrintReceiptSelectAll(TRUE);
	}

	// �̸����� ���� ��� nType = 1;
	//OnbtnPRINTRECEIPT();
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnCLOSE() 
{
	this->DestroyWindow( );
}

// **DB�κ��� ������ �������� ������ �����Ѵ�.
INT CBO_LOC_RECEIPT_PRINT_DLG::getReceiptWhere(CString &sWhere, INT f_nIndex, INT f_nRowIndex)
{
	INT ids = 0;
	INT nIdx2 = 0;

	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sTmpWhere = _T("");
	CString sUserKey = _T("");
	INT		nUserIndex;		

	nIdx2 = GetTabCtlIdx(1);

	CESL_Grid* pGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT3));		

	// ��ü ����� ������ ����.
	if ( nIdx2 == 2 )
	{		
		if (f_nRowIndex == -1 ) nUserIndex = pGrid->GetRowSel()-1;
		else nUserIndex = f_nRowIndex;
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("������KEY"), sUserKey,  nUserIndex);
		if ( ids < 0 )
		{
			return -1;
		}		
	}
	else
	{
		ids = GetDataMgrData( sMemberDMAlias, _T("������KEY"), sUserKey, 0 );
		if ( ids < 0 )
		{
			return -1;
		}
	}
	
	if ( !sUserKey.IsEmpty( ) )
		sWhere += _T(" CL.USER_KEY = ") + sUserKey;
	else
	{		
		return 1;
	}

	//bCheck = (( CButton*) GetDlgItem (IDC_radLOAN))->GetCheck();

	//++2008.04.14 UPDATE BY PWR {{++
	// "�������������", "��µȿ������̷�"�� ����/�ݳ��� ��¥ ���� �Է�
	// ��Ʈ���� ��¥ ���� �����´�.
	GetDateControl();
	// WHERE ����
	CString sLoanDateWhere = _T("");
	CString sReturnDateWhere = _T("");
	// 1. To, From �Ѵ� �Էµ� ���� 
	if( !m_sDateTo.IsEmpty() && !m_sDateFrom.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateTo, m_sDateFrom );
		sReturnDateWhere.Format(_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateTo, m_sDateFrom );
	}
	// 2. To�� �Էµ� ����
	else if( !m_sDateTo.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE")
								, m_sDateTo );
		sReturnDateWhere.Format(_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE")
								, m_sDateTo );
		
	}
	// 3. From�� �Էµ� ����
	else if( !m_sDateFrom.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND CL.LOAN_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateFrom );
		sReturnDateWhere.Format(_T(" AND CL.RETURN_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateFrom );
	}

	if ( nIdx2 == 0  )
	{ // ��Ȳ
		
		if ( f_nIndex == 0 )
		{
			// ����
			sTmpWhere += _T(" CL.STATUS NOT IN ( '3', '4', '5' )");
			//2020.04.14 JOB.2020.0033 ADD BY KYJ
			//sTmpWhere += _T(" AND CL.RECEIPT_PRINT_LOAN_YN != 'Y'");
			sTmpWhere += _T(" AND ( CL.RECEIPT_PRINT_LOAN_YN != 'Y' or CL.RECEIPT_PRINT_LOAN_YN IS NULL )");
			sTmpWhere += sLoanDateWhere;
			sTmpWhere += _T(" ORDER BY CL.LOAN_DATE DESC");
		}
		else
		{
			// �ݳ�
			// 18.09.27 JMJ KOL.RED.2018.008
			sTmpWhere += _T(" CL.STATUS IN ('1','R')");
			//2020.04.14 JOB.2020.0033 ADD BY KYJ
			//sTmpWhere += _T(" AND CL.RECEIPT_PRINT_RETURN_YN != 'Y'");
			sTmpWhere += _T(" AND ( CL.RECEIPT_PRINT_RETURN_YN != 'Y' or CL.RECEIPT_PRINT_RETURN_YN IS NULL )");
			sTmpWhere += sReturnDateWhere;
			sTmpWhere += _T(" ORDER BY CL.RETURN_DATE DESC");
		}
	}
	else if ( nIdx2 == 1 )// �̷�
	{
		if ( f_nIndex == 0 )
		{
			// ����
			sTmpWhere += _T(" CL.STATUS NOT IN ( '3', '4', '5' )");
			sTmpWhere += _T(" AND CL.RECEIPT_PRINT_LOAN_YN='Y'");
			sTmpWhere += sLoanDateWhere;
			sTmpWhere += _T(" ORDER BY CL.LOAN_DATE DESC");
		}
		else
		{
			// �ݳ�
			// 18.09.27 JMJ KOL.RED.2018.008
			sTmpWhere += _T(" CL.STATUS IN ('1','R')");
			sTmpWhere += _T(" AND CL.RECEIPT_PRINT_RETURN_YN='Y'");
			sTmpWhere += sReturnDateWhere;
			sTmpWhere += _T(" ORDER BY CL.RETURN_DATE DESC");
		}
	}
	//--2008.04.14 UPDATE BY PWR --}}
	else
	{ 	
		CString strLoanDate;
		CString strReturnDate;
		CString strLoanType;
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("������"), strLoanDate,  nUserIndex);
		if ( ids < 0 ) { return -1; }
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("�ݳ���"), strReturnDate,  nUserIndex);
		if ( ids < 0 ) { return -1; }
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("�������"), strLoanType,  nUserIndex);
		if ( ids < 0 ) { return -1; }

		if ( strLoanType == _T("����") )
		{			
			// ����
			sTmpWhere += _T(" CL.STATUS NOT IN ( '3', '4', '5' )");
			//2020.04.14 JOB.2020.0033 ADD BY KYJ
			//sTmpWhere += _T(" AND CL.RECEIPT_PRINT_LOAN_YN != 'Y'");
			sTmpWhere += _T(" AND ( CL.RECEIPT_PRINT_LOAN_YN != 'Y' or CL.RECEIPT_PRINT_LOAN_YN IS NULL )");
			sTmpWhere += _T(" AND TO_CHAR(LOAN_DATE, 'YYYY/MM/DD')='")+strLoanDate+_T("'");
			sTmpWhere += _T(" ORDER BY CL.LOAN_DATE DESC");
		}
		else
		{		
			// �ݳ�
			// 18.09.27 JMJ KOL.RED.2018.008
			sTmpWhere += _T(" CL.STATUS IN ('1','R')");
			//2020.04.14 JOB.2020.0033 ADD BY KYJ
			//sTmpWhere += _T(" AND CL.RECEIPT_PRINT_RETURN_YN != 'Y'");
			sTmpWhere += _T(" AND ( CL.RECEIPT_PRINT_RETURN_YN != 'Y' or CL.RECEIPT_PRINT_RETURN_YN IS NULL )");
			sTmpWhere += _T(" AND TO_CHAR(RETURN_DATE, 'YYYY/MM/DD')='")+strReturnDate+_T("'");
			sTmpWhere += _T(" ORDER BY CL.RETURN_DATE DESC");
		}		
	}

	//++2005.12.28 ADD BY PDW {{++
	// �������п� ���� ���� �߰�	
	CString sManageCode;
	ids = GetLoginerManageCode( sManageCode );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::getReceiptWhere()") );

// 	CString sExtraWhere = _T(" CL.MANAGE_CODE = '") + sManageCode + _T("'");
	CString sExtraWhere;
	sExtraWhere.Format(_T(" CL.MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
// 	CString sExtraWhere = _T(" CL.MANAGE_CODE = UDF_MANAGE_CODE");
	
	if ( !sWhere.IsEmpty( ) )
		sWhere += _T(" AND");

	sWhere += sExtraWhere;
	//--2005.12.28 ADD BY PDW --}}

	if ( !sWhere.IsEmpty( ) )
		sWhere += _T(" AND");

	sWhere += sTmpWhere;

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::getReceiptLogWhere(CString &sWhere, INT f_nIndex)
{
	INT ids = 0;
	
	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sTmpWhere = _T("");
	CString sUserKey = _T("");
	CString	strReceiptKey=_T("");

	sWhere=_T("");

	ids = GetDataMgrData( sMemberDMAlias, _T("������KEY"), sUserKey, 0 );
	if ( ids < 0 ) return -1;

	

	if ( f_nIndex == 0 )
	{
		sWhere = _T(" CL.RECEIPT_LOAN_KEY = CLR.REC_KEY AND");		
	}
	else
	{
		sWhere = _T(" CL.RECEIPT_RETURN_KEY = CLR.REC_KEY AND");		
	}
	
	//++2005.12.28 ADD BY PDW {{++
	// �������п� ���� ���� �߰�	
	CString sManageCode;
	ids = GetLoginerManageCode( sManageCode );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::getReceiptLogWhere()") );

// 	CString sExtraWhere = _T(" CL.MANAGE_CODE = '") + sManageCode + _T("' AND ");
	CString sExtraWhere;
	sExtraWhere.Format(_T(" CL.MANAGE_CODE = '%s' AND "), m_pInfo->MANAGE_CODE);
// 	CString sExtraWhere = _T(" CL.MANAGE_CODE = UDF_MANAGE_CODE AND ");

	sWhere += sExtraWhere;
	//--2005.12.28 ADD BY PDW --}}

	if ( !sUserKey.IsEmpty( ) )
		sWhere += _T(" CL.USER_KEY = ") + sUserKey;
	else
	{		
		return 1;
	}

	//++2008.04.14 UPDATE BY PWR {{++
	// "��µȿ������̷�"�� RECEIPT_LOG���̺� �ǿ��� ��¥ ���� �Է�
	// ��Ʈ���� ��¥ ���� �����´�.
	GetDateControl();
	// WHERE ����
	CString sPrintDateWhere = _T("");
	// 1. To, From �Ѵ� �Էµ� ���� 
	if( !m_sDateTo.IsEmpty() && !m_sDateFrom.IsEmpty() )
	{
		sPrintDateWhere.Format( _T(" AND CLR.PRINT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateTo, m_sDateFrom );
	}
	// 2. To�� �Էµ� ����
	else if( !m_sDateTo.IsEmpty() )
	{
		sPrintDateWhere.Format( _T(" AND CLR.PRINT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE")
								, m_sDateTo );
		
	}
	// 3. From�� �Էµ� ����
	else if( !m_sDateFrom.IsEmpty() )
	{
		sPrintDateWhere.Format( _T(" AND CLR.PRINT_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateFrom );
	}

	sWhere += sPrintDateWhere;
	//--2008.04.14 UPDATE BY PWR --}}

	/*	
	if ( f_nIndex == 0 )
	{
		// ����		
		sTmpWhere += _T(" CL.RECEIPT_PRINT_LOAN_YN='Y'");		
	}
	else
	{
		// �ݳ�		
		sTmpWhere += _T(" CL.RECEIPT_PRINT_RETURN_YN='Y'");	
	}
	*/

	/*
	if ( !sWhere.IsEmpty( ) )
		sWhere += _T(" AND");

	sWhere += sTmpWhere;
	*/

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::doPrintReceipt()
{
	if ( !m_bPrintYN )
	{
		return 0;		
	}

	CString	sReceiptStr;
	INT	ids;	
	INT	nIdx;	
	BOOL bIsLoan;

	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return -1;
	
	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return -2;

	//++2008.04.14 ADD BY PWR {{++
	// TotalGrid�߰� 
	CESL_Grid* pTotalGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("GridTotalReceipt") );
	if ( NULL == pTotalGrid ) return -3;
	//--2008.04.14 ADD BY PWR --}}

	nIdx = GetTabCtlIdx(0);
	if ( nIdx == 0 ) bIsLoan = TRUE;
	else bIsLoan = FALSE;		
	
	/**********************************************/
	// 1. ��¿� ������ �����
	/**********************************************/
	CString	sPrintDate;
	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
	sPrintDate = strGetData.Left(16);
// 	CTime tTime = CLocCommonAPI::GetDBTime( this );
// 	sPrintDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 		tTime.GetMonth( ),
// 		tTime.GetDay( ),
// 		tTime.GetHour( ),
// 		tTime.GetMinute( ) );

	ids = MakePrintData(sReceiptStr, sPrintDate);	
	if ( ids ) return 0;	
	/**********************************************/
	// Preview Option
	/**********************************************/
	if ( m_strPreview == _T("Y") )
	{
		CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(this);
		dlgPreview.m_strDevice = m_strDevice;
		dlgPreview.SetPrintData(sReceiptStr);
		if ( dlgPreview.DoModal() != IDOK ) return 0;
		

		/*
		if ( m_PreviewDlg == NULL )
		{
			m_PreviewDlg = new CBO_LOC_RECEIPT_PREVIEW_DLG(this);		
			m_PreviewDlg->Create(this);		
		}	
		m_PreviewDlg->m_strDevice = m_strDevice;
		m_PreviewDlg->SetPrintData(sReceiptStr);
		m_PreviewDlg->ShowWindow(TRUE);					
		*/
	}

	/**********************************************/
	// 5. Print receipt
	/**********************************************/
	// 2005.05.30 ADD BY PDJ
	// ���������θ� ������� ���� ���������� ������� ����
	
	//++2008.04.14 ADD BY PWR {{++
	// ��¾����� ������� �ʴ´�.
	if( m_strPrintYN == _T("Y") )
	{
		ids = printReceipt( sReceiptStr );
		if ( ids < 0 ) return -5;
		else if ( ids > 0 ) return ids; 			
	}
	//--2008.04.14 ADD BY PWR --}}
	
	/**********************************************/
	// 6. DB�� ���
	// ��������ȣ �ϳ� ���� -> ��� ���� ���� -> ������ �̷°��� ���̺� �ݿ�
	/**********************************************/
	ids = setDataToDB( pRctDM, pRctGrid, sReceiptStr, bIsLoan, sPrintDate );
	if ( ids < 0 ) return -6;

	//++2008.04.14 UPDATE BY PWR {{++
	// ��ü����������� TotalGrid�� �������� ���� ������ �ϱ⶧���� 
	if( GetTabCtlIdx(1) == 2 )
	{
		ids = setDMAndGrid( pRctDM, pTotalGrid );	
	}
	else
	{
		ids = setDMAndGrid( pRctDM, pRctGrid );
	}
	/*
	ids = setDMAndGrid( pRctDM, pRctGrid );
	*/
	//--2008.04.14 UPDATE BY PWR --}}
	if ( ids < 0 ) return -7;

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::doPrintReceiptSelectAll(BOOL bAllFlag)
{
	CString	sReceiptStr;
	CString strLoanType;
	CString sWhere;
	INT	nLoanType;
	INT	ids;	
	INT	i;
	INT nRowCount;
	INT nRowIndex;
	BOOL bIsLoan;

	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sTotalReceiptDMAlias = _T("DM_BO_LOC_3100_TOTAL_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");	

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return -1;

	CESL_DataMgr* pTotalRctDM = ( CESL_DataMgr* ) FindDM( sTotalReceiptDMAlias );
	if ( NULL == pTotalRctDM ) return -1;
	
	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return -2;

	CESL_Grid* pTotalRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("GridTotalReceipt") );
	if ( NULL == pTotalRctGrid ) return -2;

	if ( bAllFlag == TRUE )
	{
		nRowCount = pTotalRctDM->GetRowCount();
		for ( i=0 ; i<nRowCount ; i++ )
		{
			pTotalRctGrid->SetTextMatrix( i+1, 1, _T("V") );
		}
	}
	pTotalRctGrid->SelectMakeList();
	nRowCount = pTotalRctGrid->SelectGetCount();	
	pTotalRctGrid->SelectGetTailPosition();	
	for ( i=0 ; i<nRowCount ; i++ )
	{		
		sWhere=_T("");
		nRowIndex = pTotalRctGrid->SelectGetIdx ();		

		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("�������"), strLoanType,  nRowIndex);
		if ( ids ) return ids;

		if ( strLoanType == _T("����") ) 
		{
			bIsLoan = TRUE;
			nLoanType = 0;
			m_tab.SetCurSel(0);
		}
		else 
		{
			m_tab.SetCurSel(1);
			bIsLoan = FALSE;	
			nLoanType = 1;
		}
		
		ids = getReceiptWhere( sWhere, nLoanType,nRowIndex );
		if ( ids > 0 )
		{
			// ȭ�� �ʱ�ȭ
			ClearData();	
		}
		if ( ids != 0 ) return (ids);	
		
		if ( !sWhere.IsEmpty() )
		{
			ids = RefreshDataMgr( sReceiptDMAlias, sWhere );
			if ( ids < 0 ) return -2;	
		}
		
		/**********************************************/
		// 1. ��¿� ������ �����
		/**********************************************/
		CString	sPrintDate;
		// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
		CString strGetData;
		CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
		sPrintDate = strGetData.Left(16);
// 		CTime tTime = CLocCommonAPI::GetDBTime( this );
// 		sPrintDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 			tTime.GetMonth( ),
// 			tTime.GetDay( ),
// 			tTime.GetHour( ),
// 			tTime.GetMinute( ) );

		ids = MakePrintData(sReceiptStr, sPrintDate);	
		if ( ids ) return 0;

		if ( m_strPreview == _T("Y") )
		{
			CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(this);
			dlgPreview.m_strDevice = m_strDevice;
			dlgPreview.SetPrintData(sReceiptStr);
			if ( dlgPreview.DoModal() != IDOK ) return 0;
		}
		/**********************************************/
		// 5. Print receipt
		/**********************************************/
		//++2008.04.14 ADD BY PWR {{++
		// ��¾����� ������� �ʴ´�.
		if( m_strPrintYN == _T("Y") )
		{
			ids = printReceipt( sReceiptStr );
			if ( ids < 0 ) return -5;
			else if ( ids > 0 ) return ids; 		
		}
		//--2008.04.14 ADD BY PWR --}}

		/**********************************************/
		// 6. DB�� ���
		// ��������ȣ �ϳ� ���� -> ��� ���� ���� -> ������ �̷°��� ���̺� �ݿ�
		/**********************************************/
		ids = setDataToDB( pRctDM, pRctGrid, sReceiptStr, bIsLoan, sPrintDate );
		if ( ids < 0 ) return -6;	
		
		/// DM�� �����͸� �����
		ids = pTotalRctDM->DeleteRow( nRowIndex, -1 ); 
		if ( ids < 0 ) return -4;

		/// ������ �����Ϳ��� �� ���������͸� ȣ���ϸ� ������ �߻��Ѵ�.
		if ( ( i + 1 ) == nRowCount )	break;
		ids = pTotalRctGrid->SelectGetPrev( );
		if ( ids < 0 ) return -5;
	}

	if ( bAllFlag == TRUE )
	{
		pTotalRctDM->FreeData();
	}

	initData();
	
	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::getReceiptString(CString &sRctStr)
{
	//++2006.05.26 UPDATE BY PDW {{++
	// XML���� ��������� ����
	InitXML_Style();
	MakeStringStyle( sRctStr );
	/*
	const CString FILE_PATH = _T("..\\cfg\\LoanReturn\\ReceiptFormat.txt");

	INT ids = 0;
	CString sLine = _T("");
	CStdioFile fFile;

	sRctStr = _T("");

	if ( !fFile.Open( FILE_PATH, CFile::modeRead ) )
	{
		AfxMessageBox( _T("������ ���� ������ �������� �ʽ��ϴ�.\n(��)ECO�� ������ �ּ���\n(Tel)02-3443-8369-332") );
		return 1;
	}

	while ( TRUE )
	{
		if ( !fFile.ReadString( sLine ) )
			break;

		sLine.TrimLeft( );
		sLine.TrimRight( );

		// Header�� �ǳʶڴ�.
		if ( (sLine.Left( 1 ) == _T(".")) || (sLine.IsEmpty( )) ) continue ;

		sRctStr += sLine;
	}
	
	if ( sRctStr.IsEmpty( ) )
	{
		AfxMessageBox( _T("������ ���ǵǾ� ���� �ʽ��ϴ�.\n(��)ECO�� ������ �ּ���\n(Tel)02-3443-8369-332") );
		return 2;
	}
	*/	
	//--2006.05.26 UPDATE BY PDW --}}
	return 0;
}

// **DM_BO_LOC_3100_RECEIPT ���� ����(_T("%��_����%"), _T("%��������%"),_T("%�հ�%"), _T("%�߰�����%"))
INT CBO_LOC_RECEIPT_PRINT_DLG::setRctTempVariable(CESL_DataMgr *pRctDM, CESL_Grid *pRctGrid, CString &sRctStr, BOOL bIsLoan)
{
	INT ids = 0;
	INT nIdx = 0;
	INT nSelectCnt = 0;
	INT nTotalCnt = 0;
	INT nNonReturnBookCnt;
	INT nTab2Idx;

	CString sLoanDate = _T("");        // ������
	CString sReturnDate = _T("");      // �ݳ���
	CString sReturnPlanDate = _T("");  // �ݳ�������
	CString	sLoanStopDate = _T("");    // ����������
	CString sTmpData = _T("");    
	CString sBookInfo = _T("");        // å����
	CString sTotalCnt = _T("");        // ����å��
	CString	strCheckDate = _T("");    
	CString	strCheckDate2 = _T("");	

	CString sTmpBoInfo = _T("");

	nTab2Idx = m_tab2.GetCurSel();

	if ( nTab2Idx == 2 )
	{
		// ��ü�� ���� ���о��� ��� ���
		nSelectCnt = pRctDM->GetRowCount();
		if ( nSelectCnt == 0 ) return 1;
	}
	else
	{
		//++2008.05.23 ADD BY PWR {{++
		// �˻� �������� ���µ� ������ �����Ͱ� ���ٴ� �޽������ͼ� �߰�
		if( pRctDM->GetRowCount() < 1 ) return 1;
		//--2008.05.23 ADD BY PWR --}}

		ids = pRctGrid->SelectMakeList( );
		if ( ids < 0 ) return -1;	
		nSelectCnt = pRctGrid->SelectGetCount( );
		if ( nSelectCnt < 0 ) return -2;
		else if ( 0 == nSelectCnt ) 
		{
			AfxMessageBox( _T("������ �����Ͱ� �����ϴ�.") );
			return 1;
		}
	}

	if ( nTab2Idx != 2 )
	{
		nIdx = pRctGrid->SelectGetHeadPosition( );
		if ( nIdx < 0 ) return -3;
	}

	for ( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// ��ü ����� ��� index ����
		if ( nTab2Idx == 2 ) nIdx = i;

		if ( bIsLoan )
		{
			ids = pRctDM->GetCellData( _T("������"), nIdx, sTmpData );
			if ( ids < 0 ) return -4;			
			if ( !sLoanDate.IsEmpty( ) )
			{	
				strCheckDate = sLoanDate.Left(10);
				strCheckDate2 = sTmpData.Left(10);
				if ( 0 != strCheckDate.Compare( strCheckDate2 ) )
				{
					AfxMessageBox( _T("�������� ���� �ڷῡ ���ؼ��� ����� �����մϴ�.") );
					return 2;
				}
			}
			else
				sLoanDate = sTmpData;
		}
		else
		{
			ids = pRctDM->GetCellData( _T("�ݳ���"), nIdx, sTmpData );			
			if ( ids < 0 ) return -5;
			if ( !sReturnDate.IsEmpty( ) )
			{
				strCheckDate = sReturnDate.Left(10);
				strCheckDate2 = sTmpData.Left(10);
				if ( 0 != strCheckDate.Compare( strCheckDate2 ) )
				{
					AfxMessageBox( _T("�ݳ����� ���� �ڷῡ ���ؼ��� ����� �����մϴ�.") );
					return 2;
				}
			}
			else
				sReturnDate = sTmpData;
		}

		sTmpBoInfo.Format( _T("%4d"), ++nTotalCnt );
		sBookInfo += _T("<!FN>") + sTmpBoInfo;

		ids = pRctDM->GetCellData( _T("û����ȣ"), nIdx, sTmpData );
		if ( ids < 0 ) return -6;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 24, TRUE );
		sBookInfo += _T(" ") + sTmpBoInfo;

		ids = pRctDM->GetCellData( _T("��Ϲ�ȣ"), nIdx, sTmpData );
		if ( ids < 0 ) return -7;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 12, TRUE );
		sBookInfo += _T(" ") + sTmpBoInfo + _T("<!LN><!FP><!FN>    ");

		// ���ܼ�������(����/������)
		ids = pRctDM->GetCellData( _T("����"), nIdx, sTmpData );
		if ( ids < 0 ) return -8;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 18, FALSE );

		ids = pRctDM->GetCellData( _T("������"), nIdx, sTmpData );
		if ( ids < 0 ) return -9;
		sTmpData = sTmpBoInfo + _T("/") + sTmpData;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 28, TRUE );
		sBookInfo += _T(" ") + sTmpBoInfo;

		if ( bIsLoan )
		{
			ids = pRctDM->GetCellData( _T("��������"), nIdx, sTmpData );
			if ( ids < 0 ) return -10;
			sTmpBoInfo.Format( _T("%s"), sTmpData.Left( 8 ) );
			ids = pRctDM->GetCellData( _T("�ݳ�������"), nIdx, sReturnPlanDate );
			if ( ids < 0 ) return -10;			
		}
		else
		{
			ids = pRctDM->GetCellData( _T("�ݳ�������"), nIdx, sTmpData );
			if ( ids < 0 ) return -10;						

			// �ݳ������ϰ� �ݳ����� ���Ѵ�
			//++2008.05.21 UPDATE BY PWR {{--
			if ( !sReturnDate.IsEmpty( ) )
			{
				strCheckDate = sReturnDate.Left(10);
				strCheckDate2 = sTmpData.Left(10);
				if ( strCheckDate.Compare( strCheckDate2 ) <= 0 )
					sTmpBoInfo = _T( "����" );
				else
					sTmpBoInfo = _T( "��ü" );
			}
			else
			{
				sTmpBoInfo = _T( " " );
			}
			//--2008.05.21 UPDATE BY PWR --}}

			// �̸������� ��� ������������ ���� �� �� ����.
			if ( nTab2Idx != 2 )
			{
				ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����������"), sLoanStopDate, 0 );
			}
		}
		
		sBookInfo += _T(" ") + sTmpBoInfo + _T("<!LN><!FP>");
		
		// �η��� ��� �ηϸ� ó��
		CString	strAppendixYN;
		CString	strAppendixTitle;

		ids = pRctDM->GetCellData( _T("�ηϿ���"), nIdx, strAppendixYN );
		if ( strAppendixYN==_T("�η�") )
		{
			ids = pRctDM->GetCellData( _T("�ηϸ�"), nIdx, sTmpBoInfo );
			if ( ids ) return 0;			
			// 16/07/01 ������ : ������ ��±� ������ ������� ��� �����ϵ��� ��� ����
/*// BEFORE ------------------------------------------------------------------------
			strAppendixTitle.Format(_T("     �� �ڷ��� �η���[�ηϸ�:%s]<!FP>"), sTmpBoInfo );
*/// AFTER -------------------------------------------------------------------------
			sTmpBoInfo = getLimitWord( sTmpBoInfo.GetBuffer(0), 28, FALSE );
			strAppendixTitle.Format(_T("     �� �ڷ��� �η���<!FP>     [�ηϸ�:%s]<!FP>"), sTmpBoInfo );
//*/ END ---------------------------------------------------------------------------
			sBookInfo += strAppendixTitle;
		}
		if ( i+1 != nSelectCnt && nTab2Idx != 2)
		{
			nIdx = pRctGrid->SelectGetNext( );
			if ( nIdx < 0 ) return -11;
		}
	}
	if ( bIsLoan )
	{
		//++2006.05.26 UPDATE BY PDW {{++
		// DB����� XML�� ������ ����
		sRctStr.Replace( _T("%��_����%"), sLoanDate );	
		ChangePrintDomData( _T("%��_����%"), sLoanDate );	
		
		sReturnPlanDate =  _T("<!FN>������� : ") +sReturnPlanDate+_T(" �ϱ���<!LN><!FP><!FP>");
		
		sRctStr.Replace( _T("%�ݳ�������%"), sReturnPlanDate );
		ChangePrintDomData( _T("%�ݳ�������%"), sReturnPlanDate );	
		
		sRctStr.Replace( _T("%�߰�����%"), _T("") );
		ChangePrintDomData( _T("%�߰�����%"), _T("") );	
		/*
		sRctStr.Replace( _T("%��_����%"), sLoanDate );			
		sReturnPlanDate =  _T("<!FN>������� : ") +sReturnPlanDate+_T(" �ϱ���<!LN><!FP><!FP>");
		sRctStr.Replace( _T("%�ݳ�������%"), sReturnPlanDate );
		sRctStr.Replace( _T("%�߰�����%"), _T("") );
		*/
		//--2006.05.26 UPDATE BY PDW --}}
	}
	else
	{
		CString	strUserKey;
		CString	strWhere;
		CString	strLoanCount;

		sRctStr.Replace( _T("%��_����%"), sReturnDate );
		sRctStr.Replace( _T("%�ݳ�������%"), _T("<!FP>") );	
		//++2006.05.26 ADD BY PDW {{++
		// DB����� XML�� ������ ����
		ChangePrintDomData( _T("%��_����%"), sReturnDate );	
		ChangePrintDomData( _T("%�ݳ�������%"), _T("<!FP>") );	
		//--2006.05.26 ADD BY PDW --}}
		// ������������ ���� ��� ǥ���ϱ�
		if ( !sLoanStopDate.IsEmpty() )
		{
			sLoanStopDate = _T("�� �ݳ������� ��ü�Ǿ� ")+sLoanStopDate+_T(" �ϱ��� <!FP>                    �������� �Ⱓ�Դϴ�.<!FP>");		
		}	
		
		//m_pLMV->GetManageValue(_T("��Ÿ"), _T("����"), _T("��������¹��ڵ�"), _T("�̹ݳ����������"), strYN );	
		// �̹ݳ� ���� ���		
		
		// �̹ݳ� ���� ���� ���
		if ( nTab2Idx == 2 )
		{		
			CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");					
			INT nRowIndex;

			CESL_Grid* pTotalRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("GridTotalReceipt") );
			if ( NULL == pTotalRctGrid ) return -2;
			
			nRowIndex = pTotalRctGrid->SelectGetIdx();
			if ( nRowIndex == - 1 )
			{
				nRowIndex = pTotalRctGrid->GetRowSel()-1;
			}
			ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("������KEY"), strUserKey,  nRowIndex);
			if ( ids < 0 ) return ids;
		}
		else
		{
			ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("������KEY"), strUserKey, 0 );
		}

		CESL_DataMgr *pNonReturnBookDM = FindDM( _T("DM_BO_LOC_3100_RECEIPT_NON") );

		strWhere.Format(_T("USER_KEY = %s"), strUserKey);
		pNonReturnBookDM->RefreshDataManager(strWhere);

		/*
		strWhere.Format(_T("SELECT COUNT(1) FROM LH_STORAGE_V01 WHERE ")
			_T(" USER_KEY = %s AND STATUS IN ('0', '2')"), strUserKey );		
		pRctDM->GetOneValueQuery( strWhere, strLoanCount );
		*/
		nNonReturnBookCnt = pNonReturnBookDM->GetRowCount();
		if ( nNonReturnBookCnt > 0 )
		{
			strLoanCount.Format(_T("%d"), nNonReturnBookCnt);
			strLoanCount = _T("�� �̹ݳ������� ") + strLoanCount + _T(" �� �ֽ��ϴ�.<!FP>");
			sLoanStopDate = sLoanStopDate+strLoanCount;
		}
		
		if ( m_strItem == _T("Y") )
		{
			// �̹ݳ����� �� ���
			CString	strNonReturnBookInfo;
			DetailNonReturnBookInfo(&strNonReturnBookInfo, pNonReturnBookDM);
			sLoanStopDate += strNonReturnBookInfo;			
		}
	}		

	if ( sLoanStopDate.IsEmpty() )
	{
		sRctStr.Replace( _T("%�߰�����%"), _T("") );
		//++2006.05.26 ADD BY PDW {{++
		// DB����� XML�� ������ ����
		ChangePrintDomData( _T("%�߰�����%"), _T("") );	
		//--2006.05.26 ADD BY PDW --}}
	}
	else
	{
		sLoanStopDate += _T("<!FP>");
		sRctStr.Replace( _T("%�߰�����%"), sLoanStopDate );
		//++2006.05.26 ADD BY PDW {{++
		// DB����� XML�� ������ ����
		ChangePrintDomData( _T("%�߰�����%"), sLoanStopDate );	
		//--2006.05.26 ADD BY PDW --}}
	}				
	

	sRctStr.Replace( _T("%��������%"), sBookInfo );
	//++2006.05.26 ADD BY PDW {{++
	// DB����� XML�� ������ ����
	ChangePrintDomData( _T("%��������%"), sBookInfo );	
	//--2006.05.26 ADD BY PDW --}}

	sTotalCnt.Format( _T("%d"), nTotalCnt );
	sRctStr.Replace( _T("%�հ�%"), sTotalCnt );
	//++2006.05.26 ADD BY PDW {{++
	// DB����� XML�� ������ ����
	ChangePrintDomData( _T("%�հ�%"), sTotalCnt );	
	//--2006.05.26 ADD BY PDW --}}

	return 0;
}

// **�ѱ�, ����, ������ üũ�Ͽ� Byte���� �°� ���ڿ��� �ڸ���.
CString CBO_LOC_RECEIPT_PRINT_DLG::getLimitWord(TCHAR* szToken, INT nCutCnt, BOOL bIsAddSpace)
{
	/// �ѱ� ����� ���Ͽ� ansi�� ��ȯ�� �Ŀ�
	/// ���� ����� ����
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( szToken, &szTmp, &len );
	if( 0 < len )
	{
//		strcpy((char*)szStr,szTmp);
//		delete [] szTmp;
	}
	if(szTmp == NULL)
	{
		CString sTemp = _T("");
		for(INT i = 0; i < nCutCnt; i++)
		{
			sTemp += _T(" ");
		}
		return sTemp;
	}

	BOOL bIsCut = FALSE;

	INT nTokenLen = 0 ;
	INT nCompIndexCnt = 0 ;

	CString sReturnWord = _T("");
	CString sPeriods = _T("��");
	CHAR szPeriods[5];
	sprintf( szPeriods, "%s", "��" );

	CHAR chComp = '\0' ;
	CHAR chExtra = '\0' ;

	CHAR szTmpWord[3] ; 
	CHAR szReturnWord[100] = "" ; 	

	nTokenLen = strlen( szTmp );

	if ( nTokenLen > nCutCnt )
	{
		bIsCut = TRUE;
		nCutCnt -= 3;
	}

	for ( INT i = 0 ; i < nCutCnt ; i++ )
	{
		if ( i > (nTokenLen-1) )
		{
			if ( bIsAddSpace )
			{
				chComp = ' ';
				sprintf ( szTmpWord, "%c", chComp );
				strcat ( szReturnWord, szTmpWord );
				continue;
			}
			else
				break;
		}
		else
		{
			chComp = szTmp[i];
			chExtra = szTmp[i + 1];
		}

		/// �ѱ۰� ������ ��...
		if (chComp & 0x80)
		{
			if ( (i + 2) > nCutCnt )
			{				
				sprintf( szPeriods, "%s", "... " );
				break;
			}

			sprintf ( szTmpWord, "%c%c", chComp, chExtra ) ;			
			strcat ( szReturnWord, szTmpWord ) ;
			i++;
		}
		// �ѱ�, ���� �����϶�
		else
		{
			sprintf ( szTmpWord, "%c", chComp ) ;
			strcat ( szReturnWord, szTmpWord ) ;
		}
	}

	if ( bIsCut )
	{
		sprintf( szTmpWord, "%s", szPeriods );
		strcat( szReturnWord, szTmpWord );
	}
	
	if( 0 < len )
	{
		delete [] szTmp;
	}

	/// ����� ���� ���ڸ�
	/// �����ڵ�� ��ȯ�ؾ� �Ѵ�
	TCHAR* pszDest = NULL;
	cv.MultiByteToUnicode( szReturnWord, &pszDest, &len );	
	sReturnWord.Format( _T("%s"), pszDest );

	if( 0 < len )
	{
		delete [] pszDest;
	}

	return sReturnWord;
}

// **GETONEVALUEQUERY�� ����, ��������->����(ReceiptPrint.cfg)
// ( "%��������%",  "%�Ϸù�ȣ%", "%��������%" )
INT CBO_LOC_RECEIPT_PRINT_DLG::setRctTempVariable(CESL_DataMgr *pRctDM, CString &sRctStr)
{
	INT ids = 0;

	CString sQuery		= _T("");
	CString sTmpData	= _T("");
	CString sRctNo		= _T("");
	CString sLibName	= _T("");

	if ( m_tab2.GetCurSel() != 1 )
	{
		// 1. �Ϸù�ȣ ����
		//++2009.01.02 ADD BY KSJ {{++
		//Where ���ǿ� MAKE_YEAR �߰�
		//CTime		tCurrentCTime = CLocCommonAPI::GetDBTime(m_pParentMgr);
		//CString		strCurrentYear;
		//strCurrentYear.Format( _T("%d") , tCurrentCTime.GetYear());
		//--2009.01.02 ADD BY KSJ --}}
		//sQuery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND MAKE_YEAR='%s'") , strCurrentYear) ;
		sQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
// 		sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = UDF_MANAGE_CODE");
		ids = pRctDM->GetOneValueQuery( sQuery, sTmpData );
		if ( ids < 0 ) 
		{
			//++2009.01.02 UPDATE BY KSJ {{++
			//���ڵ尡 ���� ���, ���ڵ� ���� �� ó���Ѵ�
			if( -4007 == ids )
			{
				pRctDM->StartFrame();
				pRctDM->InitDBFieldData();
				
				ids = pRctDM->AddDBFieldData( _T("REC_KEY"),			_T("NUMERIC"), _T("ESL_SEQ.NEXTVAL") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("KIND_CODE"),			_T("STRING"), _T("RCT_NO") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("PUBLISH_FORM_CODE"),	_T("STRING"), _T("CO") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );
				
				//ids = pRctDM->AddDBFieldData( _T("MAKE_YEAR"),		_T("STRING"),	strCurrentYear );
				//if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("DIVISION_VALUE"),	_T("STRING"),	_T("������_�Ϸù�ȣ") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("LAST_NUMBER"),	_T("NUMERIC"),	_T("0") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("CREATE_DATE"),	_T("NUMERIC"),		_T("SYSDATE") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("CREATE_WORKER"),	_T("STRING"),	m_pInfo->USER_ID );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("MANAGE_CODE"),	_T("STRING"),	m_pInfo->MANAGE_CODE  );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->MakeInsertFrame( _T("CO_LAST_NUMBER_TBL") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				pRctDM->SendFrame();
				pRctDM->EndFrame();
			}
			else
			{
				return -1;
			}
			//--2009.01.02 UPDATE BY KSJ --}}
		}
		sRctNo.Format( _T("%010d"), (_ttoi( sTmpData )+1) );
		sRctStr.Replace( _T("%�Ϸù�ȣ%"), sRctNo );
		//++2006.05.26 ADD BY PDW {{++
		// DB����� XML�� ������ ����
		ChangePrintDomData( _T("%�Ϸù�ȣ%"), sRctNo );	
		//--2006.05.26 ADD BY PDW --}}
	}
	else
	{
		CString sReceiptLogDMAlias = _T("DM_BO_LOAN_RECEIPT_LOG");
		INT nRowIndex;

		CESL_Grid* pRctLogGrid = ( CESL_Grid* ) FindControl( _T("CM_BO_LOC_3100_RECEIPT"), _T("gridLog") );
		if ( NULL == pRctLogGrid ) return -110;		
		CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( sReceiptLogDMAlias );
		if( pRctLogDM == NULL ) return -110;

		nRowIndex = pRctLogGrid->GetRowSel()-1;
		if ( nRowIndex >= 0 )
		{
			pRctLogDM->GetCellData(_T("RCT_SEQ_NO"), nRowIndex, sTmpData );			
			sRctNo.Format( _T("%010d"), (_ttoi( sTmpData )) );
			sRctStr.Replace( _T("%�Ϸù�ȣ%"), sRctNo );
			//++2006.05.26 ADD BY PDW {{++
			// DB����� XML�� ������ ����
			ChangePrintDomData( _T("%�Ϸù�ȣ%"), sRctNo );	
			//--2006.05.26 ADD BY PDW --}}
		}
		
	}
	
	// 2. ������ �̸� ����
	if( m_strLibCode.IsEmpty() )
	{
		CString	strWhere;
		strWhere.Format(_T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);		
// 		strWhere = _T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");		
		pRctDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}
	sRctStr.Replace( _T("%��������%"), m_strLibCode );
	ChangePrintDomData( _T("%��������%"), m_strLibCode );	

	//++2008.04.14 ADD BY PWR {{++
	// �������� �߰�
	CStdioFile	file;
	CString		strNotify;
	CString		strTemp;
	INT			idx;

	strTemp = _T("..\\cfg\\ReceiptPrint.cfg");	
	if(!file.Open (strTemp,CFile::modeRead) ) 
	{
		strNotify = _T("");
	}
	
	CString strLine;
	CString sTemp;
	while( file.ReadString( strLine ) )
	{
		strLine.TrimLeft();
		if ( strLine.Left(1) == _T("#") || strLine.IsEmpty() ) continue;		
		if ( strLine.Left(1) == _T("[") || strLine.IsEmpty() ) continue;		

		idx = strLine.Find( _T("=") );
		if ( 0 <= idx )
		{
			sTemp = strLine.Left(idx);
			sTemp.TrimLeft();
			sTemp.TrimRight();
			if ( sTemp == _T("PRINT_NOTIFY") ) 
			{
				strNotify = strLine.Mid( idx + 1 );
				strNotify.TrimLeft();
				strNotify.TrimRight();
			}
		}
	}
	file.Close() ;

	if( !strNotify.IsEmpty() )
	{
		strNotify += _T("<!FP><!FP>");
	}
	sRctStr.Replace( _T("%��������%"), strNotify );
	ChangePrintDomData( _T("%��������%"), strNotify );
	//--2008.04.14 ADD BY PWR --}}

	return 0;
}

// **�ٷ� ���� ������ �κ�(_T("%��_������%"),_T("%��_��������%"), _T("%����Ͻ�%"))
INT CBO_LOC_RECEIPT_PRINT_DLG::setRctTempVariable(CString &sRctStr, BOOL bIsLoan, CString f_strPrintDate)
{
	INT ids = 0;
	CString sTitle = _T("");
	CString sDateTitle = _T("");
	CString sPrintDate = _T("");

	//CTime tTime = CLocCommonAPI::GetDBTime( this );

	// 1 ����/�ݳ��� ���� �޶����� Alias ����
	if ( bIsLoan )
	{
		sTitle = _T("����");
		sDateTitle = _T("�� �� ��");
	}
	else
	{
		sTitle = _T("�ݳ�");
		sDateTitle = _T("�� �� ��");
	}
	sRctStr.Replace( _T("%��_������%"), sTitle );
	sRctStr.Replace( _T("%��_��������%"), sDateTitle );
	//++2006.05.26 ADD BY PDW {{++
	// DB����� XML�� ������ ����
	ChangePrintDomData( _T("%��_������%"), sTitle );	
	ChangePrintDomData( _T("%��_��������%"), sDateTitle );
	//--2006.05.26 ADD BY PDW --}}
	
	
	/*
	sPrintDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
													tTime.GetMonth( ),
													tTime.GetDay( ),
													tTime.GetHour( ),
													tTime.GetMinute( ) );
													*/


	sRctStr.Replace( _T("%����Ͻ�%"), f_strPrintDate );
	//++2006.05.26 ADD BY PDW {{++
	// DB����� XML�� ������ ����
	ChangePrintDomData( _T("%����Ͻ�%"), f_strPrintDate );	
	//--2006.05.26 ADD BY PDW --}}

	return 0;
}

// **DM_BO_LOC_3100_MEMBER_INFO ���� ����(_T("%�����ڹ�ȣ%"), _T("%�������̸�%"), _T("%�μ���%"))
INT CBO_LOC_RECEIPT_PRINT_DLG::setRctTempVariable(CString &sRctStr)
{	
	CString sUserNo = _T("");
	CString sUserName = _T("");
	CString sDepartmentName = _T("");	
	INT		ids;
	INT		nRowIndex;

	if (m_tab2.GetCurSel() == 2 )
	{
		CESL_Grid* pGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT3));		
		nRowIndex = pGrid->SelectGetIdx();
		if ( nRowIndex == -1 )
		{
			nRowIndex = pGrid->GetRowSel()-1;
		}
		
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("�����ڹ�ȣ"), sUserNo, nRowIndex );
		if ( ids < 0 ) return ids;
		//++2008.03.14 ADD BY PWR {{++
		// �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ�� 
		ChangeSecuriyCivilNo( sUserNo );
		//for ( i=6 ; i<sUserNo.GetLength() ; i++ )
		//{
		//	sUserNo.SetAt(i, '*');		
		//}
		//--2008.03.14 ADD BY PWR --}}
		sRctStr.Replace( _T("%�����ڹ�ȣ%"), sUserNo );
		//++2006.05.26 ADD BY PDW {{++
		// DB����� XML�� ������ ����
		ChangePrintDomData(_T("%�����ڹ�ȣ%"), sUserNo );
		//--2006.05.26 ADD BY PDW --}}

		// 4 �������̸� ����
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("�̸�"), sUserName, nRowIndex );
		if ( ids < 0 ) return -3;
		sRctStr.Replace( _T("%�������̸�%"), sUserName );		
		//++2006.05.26 ADD BY PDW {{++
		// DB����� XML�� ������ ����
		ChangePrintDomData(_T("%�������̸�%"), sUserName );
		//--2006.05.26 ADD BY PDW --}}
	}
	else
	{
		// 3 �����ڹ�ȣ ����
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����ڹ�ȣ"), sUserNo, 0 );
		if ( ids < 0 ) return -2;
		//++2008.03.14 ADD BY PWR {{++
		// �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ�� 
		ChangeSecuriyCivilNo( sUserNo );
		// 2005.05.11 ADD BY PDJ
		// ȸ����ȣ �տ� 7�ڸ��� ǥ���ϰ� �ڴ� *��
		//for ( i=6 ; i<sUserNo.GetLength() ; i++ )
		//{
		//	sUserNo.SetAt(i, '*');		
		//}
		//--2008.03.14 ADD BY PWR --}}
		sRctStr.Replace( _T("%�����ڹ�ȣ%"), sUserNo );
		//++2006.05.26 ADD BY PDW {{++
		// DB����� XML�� ������ ����
		ChangePrintDomData( _T("%�����ڹ�ȣ%"), sUserNo );
		//--2006.05.26 ADD BY PDW --}}
		// 4 �������̸� ����
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�������̸�"), sUserName, 0 );
		if ( ids < 0 ) return -3;
		sRctStr.Replace( _T("%�������̸�%"), sUserName );
		//++2006.05.26 ADD BY PDW {{++
		// DB����� XML�� ������ ����
		ChangePrintDomData( _T("%�������̸�%"), sUserName );
		//--2006.05.26 ADD BY PDW --}}
		// 5 �����ںμ��� ����
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����ڼҼ�"), sDepartmentName, 0 );
		if ( ids < 0 ) return -4;
		sRctStr.Replace( _T("%�μ���%"), sDepartmentName );
		//++2006.05.26 ADD BY PDW {{++
		// DB����� XML�� ������ ����
		ChangePrintDomData( _T("%�μ���%"), sDepartmentName );
		//--2006.05.26 ADD BY PDW --}}
	}

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::printReceipt(CString sRctStr, BOOL bStatusCheck)
{
	INT ids = 0;
	INT nStatus = 0;

	TCHAR ErrMsg[1024];
	//UCHAR szStr[2048];
	// UCHAR szStr[2048]; 2048�̻��� ������ ������ ������ ���� ó��.
	UCHAR *szStr = new UCHAR[100000];
	if ( szStr == NULL ) return -100;

	//if ( m_strMode == '2' ) return 0;
	
	// 16/07/01 ������ : ������ ��±� ������ ������� ��� �����ϵ��� ��� ����
//*/ ADD ---------------------------------------------------------------------------
	DWORD dwWritten;
	HANDLE hPrinter;

	BOOL bIsWTPPrinter = TRUE;	
	TCHAR* szDest = NULL;			
	CHAR* szTmp2 = NULL;
	INT len2 = 0;
		
	CString strTemp;
	CString strPrinter;

	GetReceiptPrinterName(strPrinter);
	if( !strPrinter.IsEmpty() )
	{
		bIsWTPPrinter = FALSE;
		ids = ::OpenPrinter( strPrinter.GetBuffer(0), &hPrinter, NULL );
		if( 0 >= ids)
		{
			strTemp.Format(_T("[%s] ��±⸦ ã�� �� �����ϴ�."),strPrinter);
			CLocCommonAPI::MakeRFIDLog( _T("[ERROR]OpenPrinter()"), strTemp, __WFILE__, __LINE__, ids );
			AfxMessageBox(_T("�߰ߵ� ��±Ⱑ �����ϴ�."));
			delete [] szStr;
			return 200;
		}			
	}
	else
	{
//*/ END ---------------------------------------------------------------------------	
		ids = LP_StartUp();	
		if( 0 != ids )
		{
			
			{
				LP_GetErrorMsg( ErrMsg );
				CLocCommonAPI::MakeRFIDLog( _T("[ERROR]LP_StartUp()"), ErrMsg, __WFILE__, __LINE__, ids );
				AfxMessageBox(_T("�߰ߵ� ��±Ⱑ �����ϴ�."));
				delete [] szStr;
				return 200;
			}
		}
	}

	if(TRUE == bStatusCheck)
	{
		if( TRUE == bIsWTPPrinter )
		{
 			nStatus = LP_GetStatus( );
 			CLocCommonAPI::MakeRFIDLog( _T("[LOG]LP_GetStatus()"), _T("�������� ��������"), __WFILE__, __LINE__, nStatus );
 			if ( 0 > nStatus )
 			{
 				LP_CleanUp();
 				LP_GetErrorMsg( ErrMsg );
 				CLocCommonAPI::MakeRFIDLog( _T("[ERROR]LP_GetStatus()"), ErrMsg, __WFILE__, __LINE__, ids );
 				AfxMessageBox(_T("��±��� ������ �����ְų� ������ �Ǿ� ���� �ʽ��ϴ�.\n��±⸦ �����ѵ� Ȯ�ι�ư�� ��������.\n��µ��� ���� �������� �̷¿��� ������� �����մϴ�"), MB_OK);
 				delete [] szStr;
 				return 100;
 			}
 			else if ( 0 != nStatus )
 			{
 				LP_CleanUp();
 				ids = LP_StartUp();
 				if( 0 != ids )
 				{
 					LP_GetErrorMsg( ErrMsg );
 					CLocCommonAPI::MakeRFIDLog( _T("[ERROR]LP_StartUp()"), ErrMsg, __WFILE__, __LINE__, ids );
 					AfxMessageBox(_T("�߰ߵ� ��±Ⱑ �����ϴ�."));
 					delete [] szStr;
 					return 200;
 				}
 			}	
		}
		// 16/07/01 ������ : ������ ��±� ������ ������� ��� �����ϵ��� ��� ����
//*/ ADD ---------------------------------------------------------------------------
		else
		{
			DWORD dwStatus = 0;
			BOOL bError = IsPrinterError(hPrinter, dwStatus);
			CLocCommonAPI::MakeRFIDLog( _T("[LOG]LP_GetStatus()"), _T("�������� ��������"), __WFILE__, __LINE__,  dwStatus);
			if( FALSE != bError )	//Error
			{
				CLocCommonAPI::MakeRFIDLog( _T("[ERROR]GetPrinter()"), _T("��±⸦ ����� �� �����ϴ�."), __WFILE__, __LINE__, bError );
				if(PRINTER_STATUS_NOT_AVAILABLE == dwStatus)
				{
					AfxMessageBox(_T("��±��� ������ �����ְų� ������ �Ǿ� ���� �ʽ��ϴ�.\n��±⸦ �����ѵ� Ȯ�ι�ư�� ��������.\n��µ��� ���� �������� �̷¿��� ������� �����մϴ�"), MB_OK);
				}
				else
				{
 					AfxMessageBox(_T("��±� ���¿� ������ �־ ����� �� �����ϴ�.\n��±⸦ �����ѵ� Ȯ�ι�ư�� ��������.\n��µ��� ���� �������� �̷¿��� ������� �����մϴ�"), MB_OK);
				}
				::ClosePrinter(hPrinter);
 				delete [] szStr;
 				return 100;
			}
		}
//*/ END ---------------------------------------------------------------------------
	}

	// 16/07/01 ������ : ������ ��±� ������ ������� ��� �����ϵ��� ��� ����
//*/ ADD ---------------------------------------------------------------------------
	if( TRUE != bIsWTPPrinter )
	{
		//���Ÿ��Ʋ ã�� - [�������]
		INT nStart = sRctStr.Find('[', 0) + 1;
		INT nEnd = sRctStr.Find(']', nStart);
		strTemp = sRctStr.Mid(nStart, nEnd-nStart);

		DOC_INFO_1 di = {0x00};
		di.pDocName = strTemp.GetBuffer(0);
		di.pOutputFile = NULL;
		di.pDatatype = _T("RAW");

		::StartDocPrinter(hPrinter, 1, (LPBYTE)&di);
		::StartPagePrinter(hPrinter);
	}
//*/ END ---------------------------------------------------------------------------

	CLocCommonAPI::MakeRFIDLog( _T("[LOG]LP_StartUp()"), _T("������ ��� �غ�"), __WFILE__, __LINE__, ids );

	/// TCHAR ������ UCHAR �������� ��ȯ���ش�.
	//++2008.05.21 UPDATE BY PWR {{++
	// ����� �����Ͱ� 6000������ �Ѿ�� ����������� �̷������ �ʴ´�.
	// 5000������ �߶� ������ ���鼭 �̾�����ϵ��� ����.	
	INT nOffset = 0;
	INT nAddIndex = 0;
	INT nCutStep = 0;
	INT nLength = 0;
	INT	i = 0;
	INT nIndex = 0;
		
//	nLength = sRctStr.GetLength( );

	//++2007.11.1 UNICODE WORK BY PDW {{++
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( sRctStr.GetBuffer(0), &szTmp, &len );
	if( 0 < len )
	{
//		strcpy((char*)szStr,szTmp);
//		delete [] szTmp;
	}

	szStr[len]= '\0';
	nLength = len;
//	ids = LP_PrintData( szStr );
	//--2007.11.1 UNICODE WORK BY PDW --}}

	while( i < nLength )
	{
		nAddIndex = 0;
		nCutStep = 0;
		nIndex = 0;		
		// �߶��� �κк��� �ٽ� ����
		for( i = nOffset ; i < nLength ; i++ )
		{
			szStr[nIndex] = (UCHAR)szTmp[i];
			// �����Ͱ� 5000�� �Ǹ�
			if ( (i > 0) && (i % 2500 == 0) )
			{				
				// å�� �ݳ����±��� ����ϰ� �߸���.
				// �ڸ��ºκ� = �߸��κ�(5000)���� �ݳ����±��� ã���κп��� �ڸ������κ�
				/*
				nAddIndex = (sRctStr.Find( _T("<!LN><!FP><!FN>  "), i) + 15 )-i;				
				if ( sRctStr.Mid(i+nAddIndex, 1 ) == _T(" ") )
				{
					// �ڸ��� �κ��� ��Ϲ�ȣ���� ������ �Ǹ� ���ٴ� ��������.
					nAddIndex = (sRctStr.Find( _T("<!LN><!FP><!FN>  "), i+nAddIndex) + 15 )-i;					
				}
				*/			
				
				CHAR* szFindDest = strstr(szTmp+i, "<!LN><!FP><!FN>  ") + 15;				
				CHAR* szFindBuf = szTmp + i; // �׽�Ʈ��

				nAddIndex = szFindDest - (szTmp+i) + 1;
				
				if( 0 < nAddIndex &&  ' ' == szTmp[i+nAddIndex] )
				{
					szFindDest = strstr(szTmp+i+nAddIndex, "<!LN><!FP><!FN>  ") + 15;
					nAddIndex = szFindDest - (szTmp+i) + 1;
				}
	
			}
			
			if ( nAddIndex > 0 ) nCutStep ++;
			nOffset++;

			if ( nAddIndex > 0 && nCutStep == nAddIndex ) 
			{
				break;
			}
			nIndex++;
		}
		
		szStr[nIndex]= '\0';
		
		// 16/07/01 ������ : ������ ��±� ������ ������� ��� �����ϵ��� ��� ����
/*// BEFORE ------------------------------------------------------------------------
		ids = LP_PrintData( szStr );	
*/// AFTER -------------------------------------------------------------------------
		if( TRUE == bIsWTPPrinter )
		{
			ids = LP_PrintData( szStr );		
		}
		else
		{				
			cv.MultiByteToUnicode( (char*)szStr, &szDest, &len2 );	
			strTemp.Format( _T("%s"), szDest );
			if( 0 < len2 )
			{
				delete [] szDest;
				szDest = NULL;
			}
			
			RemoveTag(strTemp);

			cv.UnicodeToMultiByte( strTemp.GetBuffer(0), &szTmp2, &len2 );
			::WritePrinter(hPrinter, szTmp2, len2, &dwWritten);
			
			if( 0 < len2 )
			{
				delete [] szTmp2;
				szTmp2 = NULL;
			}
		}
//*/ END ---------------------------------------------------------------------------
	}

	if( 0 < len )
	{
		delete [] szTmp;
	}
		
	// 16/07/01 ������ : ������ ��±� ������ ������� ��� �����ϵ��� ��� ����
/*// BEFORE ------------------------------------------------------------------------
	if(FALSE == bStatusCheck)
	{
		LP_PrintCut();
	}

	LP_CleanUp();
*/// AFTER -------------------------------------------------------------------------
	if( 0 < nLength )	//�޸� ������ �߰�
	{
		delete [] szStr;
	}

	if( TRUE == bIsWTPPrinter )
	{
		if(FALSE == bStatusCheck)
		{
			LP_PrintCut();
		}

		LP_CleanUp();
	}
	else
	{
		//������ �� ó��
		strTemp.Format(_T("\r\n\r\n\r\n\r\n\x1b\x69")); // \r\n(��ġ����), \x1b\x69(print_cut)
		cv.UnicodeToMultiByte( strTemp.GetBuffer(0), &szTmp2, &len2 );
		::WritePrinter(hPrinter, szTmp2, len2, &dwWritten);
		if( 0 < len2 )
		{
			delete [] szTmp2;
		}
		
		::EndPagePrinter(hPrinter);
		::EndDocPrinter(hPrinter);
		::ClosePrinter(hPrinter);
	}
//*/ END ---------------------------------------------------------------------------

	return 0;
}

// 16/07/01 ������ : ������ ��±� ������ ������� ��� �����ϵ��� ��� ����
//*/ ADD ---------------------------------------------------------------------------
//���������� �����͸� ������
VOID CBO_LOC_RECEIPT_PRINT_DLG::GetReceiptPrinterName(CString &strValue)
{
	// �����ڵ� 
	CStdioFile file ;		
	CString strTemp;		
	CString strLine;
	INT		ids;
	
	strTemp = _T("..\\cfg\\ReceiptPrintName.cfg") ;	
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary) ) 
	{	
		return;
	}

	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	ids = file.ReadString (strLine);	
	strValue = strLine;
	file.Close();
}

//������ �±� ����
VOID CBO_LOC_RECEIPT_PRINT_DLG::RemoveTag(CString &strValue)
{
	strValue.Replace(_T("<!FH>"),_T("\x1b\x21\x51\x1b\x45\r"));		//header/footer ����- \x1b\x21\x51(enlarge), \x1b\x45\r(bold)
	strValue.Replace(_T("<!FF>"),_T("\x1b\x21\x38"));				//\x1b\x21x38(bold_width_height) 
	strValue.Replace(_T("<!LF>"),_T("\x1b\x21\x02"));				//\x1b\x21\x02(fontstyle_normal)
	strValue.Replace(_T("<!FC>"),_T("\x1b\x61\x31"));				//\x1b\x61\x31(align_center)
	strValue.Replace(_T("<!LC>"),_T("\x1b\x61\x30"));				//\x1b\x61\x30(align_left)
	strValue.Replace(_T("<!FN>"),_T("\x1b\x61\x30\x1b\x21\x02"));	//body ���� - \x1b\x61\x30(align_left), \x1b\x21\x02(fontstyle_normal)
	strValue.Replace(_T("<!LN>"),_T(""));
	strValue.Replace(_T("<!FR>"),_T("\x1b\x61\x32"));				//\x1b\x61\x32(align_right)	
	strValue.Replace(_T("<!LR>"),_T("\x1b\x61\x30"));				//\x1b\x61\x30(align_left)
	strValue.Replace(_T("<!LH>"),_T("\x1b\x21\x02"));				//\x1b\x21\x02(fontstyle_normal)
	strValue.Replace(_T("<!FP>"),_T("\r\n"));
}

BOOL CBO_LOC_RECEIPT_PRINT_DLG::GetJobs(HANDLE hPrinter,        /* Handle to the printer. */ 
                JOB_INFO_2 **ppJobInfo, /* Pointer to be filled.  */ 
                INT *pcJobs,            /* Count of jobs filled.  */ 
                DWORD *pStatus)         /* Print Queue status.    */ 

{

	DWORD               cByteNeeded,
						nReturned,
						cByteUsed;
	JOB_INFO_2          *pJobStorage = NULL;
	PRINTER_INFO_2       *pPrinterInfo = NULL;

	/* Get the buffer size needed. */ 
   if (!GetPrinter(hPrinter, 2, NULL, 0, &cByteNeeded))
   {
       if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
           return FALSE;
   }

   pPrinterInfo = (PRINTER_INFO_2 *)malloc(cByteNeeded);
   if (!(pPrinterInfo))
       /* Failure to allocate memory. */ 
       return FALSE;

   /* Get the printer information. */ 
   if (!GetPrinter(hPrinter,
           2,
           (unsigned char*)pPrinterInfo,
           cByteNeeded,
           &cByteUsed))
   {
       /* Failure to access the printer. */ 
       free(pPrinterInfo);
       pPrinterInfo = NULL;
       return FALSE;
   }

   /* Get job storage space. */ 
   if (!EnumJobs(hPrinter,
           0,
           pPrinterInfo->cJobs,
           2,
           NULL,
           0,
           (LPDWORD)&cByteNeeded,
           (LPDWORD)&nReturned))
   {
       if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
       {
           free(pPrinterInfo);
           pPrinterInfo = NULL;
           return FALSE;
       }
   }

   pJobStorage = (JOB_INFO_2 *)malloc(cByteNeeded);
   if (!pJobStorage)
   {
       /* Failure to allocate Job storage space. */ 
       free(pPrinterInfo);
       pPrinterInfo = NULL;
       return FALSE;
   }

   ZeroMemory(pJobStorage, cByteNeeded);

   /* Get the list of jobs. */ 
   if (!EnumJobs(hPrinter,
           0,
           pPrinterInfo->cJobs,
           2,
           (LPBYTE)pJobStorage,
           cByteNeeded,
           (LPDWORD)&cByteUsed,
           (LPDWORD)&nReturned))
   {
       free(pPrinterInfo);
       free(pJobStorage);
       pJobStorage = NULL;
       pPrinterInfo = NULL;
       return FALSE;
   }

   /*
    *  Return the information.
    */ 
   *pcJobs = nReturned;
   *pStatus = pPrinterInfo->Status;
   *ppJobInfo = pJobStorage;
   free(pPrinterInfo);

   return TRUE;

}

BOOL CBO_LOC_RECEIPT_PRINT_DLG::IsPrinterError(HANDLE hPrinter, DWORD &dwStatus)
{
   JOB_INFO_2  *pJobs;
   INT         cJobs,
               i;
   DWORD       dwPrinterStatus;

   /*
    *  Get the state information for the Printer Queue and
    *  the jobs in the Printer Queue.
    */ 
   if (!GetJobs(hPrinter, &pJobs, &cJobs, &dwPrinterStatus))
		return FALSE;

   dwStatus = dwPrinterStatus;

   /*
    *  If the Printer reports an error, believe it.
    */ 
   if (dwPrinterStatus &
       (PRINTER_STATUS_ERROR |
       PRINTER_STATUS_PAPER_JAM |
       PRINTER_STATUS_PAPER_OUT |
       PRINTER_STATUS_PAPER_PROBLEM |
       PRINTER_STATUS_OUTPUT_BIN_FULL |
       PRINTER_STATUS_NOT_AVAILABLE |
       PRINTER_STATUS_NO_TONER |
       PRINTER_STATUS_OUT_OF_MEMORY |
       PRINTER_STATUS_OFFLINE |
       PRINTER_STATUS_DOOR_OPEN))
   {
       free( pJobs );
       return TRUE;
   }

   /*
    *  Find the Job in the Queue that is printing.
    */ 
   for (i=0; i < cJobs; i++)
   {
       if (pJobs[i].Status & JOB_STATUS_PRINTING)
       {
           /*
            *  If the job is in an error state,
            *  report an error for the printer.
            *  Code could be inserted here to
            *  attempt an interpretation of the
            *  pStatus member as well.
            */ 
           if (pJobs[i].Status &
               (JOB_STATUS_ERROR |
               JOB_STATUS_OFFLINE |
               JOB_STATUS_PAPEROUT |
               JOB_STATUS_BLOCKED_DEVQ))
           {
               free( pJobs );
               return TRUE;
           }
       }
   }

   /*
    *  No error condition.
    */ 
   free( pJobs );
   return FALSE;

}
//*/ END ---------------------------------------------------------------------------

// ����ϰ� �� �Ŀ� DB�� �ݿ��Ѵ�.
INT CBO_LOC_RECEIPT_PRINT_DLG::setDataToDB(CESL_DataMgr *pRctDM, CESL_Grid* pRctGrid, CString sReceiptStr, BOOL bIsLoan, CString f_strPrintDate)
{
	INT ids = 0;
	CString sLastNumber = _T("");
	CString sLoanKeyList = _T("");

	/************************************************************/
	/*FRAME�� ����												*/
	/************************************************************/
	ids = pRctDM->StartFrame( );
	if ( ids < 0 )	return -1;

	// 1. ��������ȣ �ϳ� ����
	ids = setLastNumber( pRctDM, sLastNumber );
	if ( ids < 0 ) return -2;	
	
	// 2. ��� ���� ����
	ids = setReceiptPrintYN( pRctDM, pRctGrid, sLoanKeyList, bIsLoan );
	if ( ids < 0 ) return -3;
	
	// 3. ������ �̷°��� ���̺� �ݿ�
	ids = setReceiptTable( pRctDM, pRctGrid, sReceiptStr, sLastNumber, bIsLoan, sLoanKeyList, f_strPrintDate );
	if ( ids < 0 ) return -4;	
	
	ids = pRctDM->SendFrame ( ) ;
	if ( ids < 0 )	return -5;

	ids = pRctDM->EndFrame ( ) ;
	if ( ids < 0 )	return -6;
	/************************************************************/
	/*FRAME�� ��												*/
	/************************************************************/

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::setLastNumber(CESL_DataMgr *pRctDM, CString &sLastNo)
{
	INT ids = 0;
	CString sWhere = _T("");
	CString sRecKey = _T("");

	pRctDM->InitDBFieldData( );

	//++2009.01.02 ADD BY KSJ {{++
	//Where ���ǿ� MAKE_YEAR �߰�
	//CTime		tCurrentCTime = CLocCommonAPI::GetDBTime(m_pParentMgr);
	//CString		strCurrentYear;
	//strCurrentYear.Format( _T("%d") , tCurrentCTime.GetYear());
	//--2009.01.02 ADD BY KSJ --}}

	//sWhere.Format( _T("SELECT REC_KEY FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE") , strCurrentYear );
// 	sWhere = _T("SELECT REC_KEY FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = UDF_MANAGE_CODE");
	sWhere.Format(_T("SELECT REC_KEY FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);

	ids = pRctDM->GetOneValueQuery( sWhere, sRecKey );
	if ( 0 > ids ) 
	{
		//++2009.01.02 UPDATE BY KSJ {{++
		//���ڵ尡 ���� ���, ���ڵ� ���� �� ó���Ѵ�
		if( -4007 == ids )
		{
			pRctDM->StartFrame();
			pRctDM->InitDBFieldData();
			
			ids = pRctDM->AddDBFieldData( _T("REC_KEY"),			_T("NUMERIC"), _T("ESL_SEQ.NEXTVAL") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("KIND_CODE"),			_T("STRING"), _T("RCT_NO") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("PUBLISH_FORM_CODE"),	_T("STRING"), _T("CO") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );
			
			//ids = pRctDM->AddDBFieldData( _T("MAKE_YEAR"),		_T("STRING"),	strCurrentYear );
			//if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("DIVISION_VALUE"),	_T("STRING"),	_T("������_�Ϸù�ȣ") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("LAST_NUMBER"),	_T("NUMERIC"),	_T("0") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("CREATE_DATE"),	_T("NUMERIC"),		_T("SYSDATE") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("CREATE_WORKER"),	_T("STRING"),	m_pInfo->USER_ID );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("MANAGE_CODE"),	_T("STRING"),	m_pInfo->MANAGE_CODE  );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->MakeInsertFrame( _T("CO_LAST_NUMBER_TBL") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			pRctDM->SendFrame();
			pRctDM->EndFrame();
		}
		else
		{
			return -1;
		}
		//--2009.01.02 UPDATE BY KSJ --}}
	}
	//if ( ids < 0 )	return -1;

	//sWhere.Format( _T("SELECT (LAST_NUMBER+1) FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE") , strCurrentYear );
// 	sWhere = _T("SELECT (LAST_NUMBER+1) FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = UDF_MANAGE_CODE");
	sWhere.Format(_T("SELECT (LAST_NUMBER+1) FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
	ids = pRctDM->GetOneValueQuery( sWhere, sLastNo );
	if ( ids < 0 )	return -2;

	ids = pRctDM->AddDBFieldData ( _T("LAST_NUMBER"), _T("NUMERIC"), sLastNo );
	if ( ids < 0 )	return -3;

	ids = pRctDM->MakeUpdateFrame( _T("CO_LAST_NUMBER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( ids < 0 ) return -4;	

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::setReceiptPrintYN(CESL_DataMgr *pRctDM, CESL_Grid *pRctGrid, CString &sLoanKeyList, BOOL bIsLoan)
{
	INT ids = 0;
	INT nIdx = 0;
	INT nSelectCnt = 0;
	INT nTab2Idx;
	CString sResultLoanYN = _T("");
	CString sResultReturnYN = _T("");
	CString sRecKey = _T("");

	sLoanKeyList.Empty( );
	pRctDM->InitDBFieldData( );

	nTab2Idx = m_tab2.GetCurSel();
	if ( nTab2Idx == 2 )
	{
		nSelectCnt = pRctDM->GetRowCount();
		if ( nSelectCnt == 0 ) return 1;
	}
	else
	{	
		ids = pRctGrid->SelectMakeList( );
		if ( ids < 0 ) return -1;
		
		nSelectCnt = pRctGrid->SelectGetCount( );
		if ( nSelectCnt < 0 ) return -2;
		else if ( 0 == nSelectCnt ) 
		{
			AfxMessageBox( _T("������ �����Ͱ� �����ϴ�.") );
			return 1;
		}
		
		nIdx = pRctGrid->SelectGetHeadPosition( );
		if ( nIdx < 0 ) return -3;
	}
	

	for ( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		/*
		ids = pRctDM->GetCellData( _T("�����������"), nIdx, sResultLoanYN );
		if ( ids < 0 ) return -4;
		ids = pRctDM->GetCellData( _T("�ݳ��������"), nIdx, sResultReturnYN );
		if ( ids < 0 ) return -4;
		*/
		if ( nTab2Idx == 2 ) nIdx = i;

		ids = pRctDM->GetCellData( _T("REC_KEY"), nIdx, sRecKey );
		if ( ids < 0 ) return -5;

		if ( sLoanKeyList.IsEmpty( ) )	sLoanKeyList = sRecKey;
		else							sLoanKeyList += _T(", ") + sRecKey;

		if ( bIsLoan )	
		{
			sResultLoanYN = _T("Y");
			ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_LOAN_YN"), _T("STRING"), sResultLoanYN );
		if ( ids < 0 )	return -6;
		}
		else			
		{
			sResultReturnYN = _T("Y");			
			ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_RETURN_YN"), _T("STRING"), sResultReturnYN );
			if ( ids < 0 )	return -6;
		}		
		
		ids = pRctDM->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
		if ( ids < 0 )	return -7;

		if ( i+1 != nSelectCnt && nTab2Idx != 2 )
		{
			nIdx = pRctGrid->SelectGetNext( );
			if ( nIdx < 0 ) return -8;
		}
	}

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::setReceiptTable(CESL_DataMgr *pRctDM, CESL_Grid *pRctGrid, CString sReceiptStr, CString sLastNumber, BOOL bIsLoan, CString sLoanKeyList, CString f_strPrintDate)
{
	INT ids = 0;
	CString sRecKey = _T("");	
	CString	strPrintDate;
	
	//++2006.05.26 ADD BY PDW {{++
	// �������̷��� XML�� ����
	ids = ChangeReceiptStrTOXML( sReceiptStr );
	if ( ids < 0 ) return -6;
	//--2006.05.26 ADD BY PDW --}}

	pRctDM->InitDBFieldData( );

	ids = pRctDM->MakeRecKey( sRecKey );
	if ( ids < 0 ) return -1;

	ids = pRctDM->AddDBFieldData( _T("LOAN_KEY_LIST"), _T("STRING"), sLoanKeyList );
	if ( ids < 0 ) return -2;

	ids = pRctDM->AddDBFieldData( _T("RECEIPT_STRING"), _T("STRING"), sReceiptStr );
	if ( ids < 0 ) return -3;

	ids = pRctDM->AddDBFieldData( _T("RCT_SEQ_NO"), _T("NUMERIC"), sLastNumber );
	if ( ids < 0 ) return -4;

	//++2008.03.14 ADD BY PWR {{++
	// ��¾����϶����� PRINT_COUNT=0���� �ؼ� �����ϵ��� �Ѵ�.
	if( m_strPrintYN == _T("Y") )
	{
		ids = pRctDM->AddDBFieldData( _T("PRINT_COUNT"), _T("NUMERIC"), _T("1") );
		if ( ids < 0 ) return -5;
	}
	else
	{
		ids = pRctDM->AddDBFieldData( _T("PRINT_COUNT"), _T("NUMERIC"), _T("0") );
		if ( ids < 0 ) return -5;
	}
	//ids = pRctDM->AddDBFieldData( _T("PRINT_COUNT"), _T("NUMERIC"), _T("1") );
	//if ( ids < 0 ) return -5;
	//--2008.03.14 ADD BY PWR --}}

	/*
	sInLoanDate.Format( _T("to_date('%s %d:%d:%d','YYYY/MM/DD HH24:MI:SS')") , sLoanDate , tLoanDate.GetHour() , tLoanDate.GetMinute() , tLoanDate.GetSecond() );
	ids = pDM->AddDBFieldData( _T("LOAN_DATE") , _T("NUMERIC") , sLoanDate , TRUE );
	*/	
	strPrintDate.Format(_T("to_date('%s:00','YYYY/MM/DD HH24:MI:SS')"), f_strPrintDate);
	ids = pRctDM->AddDBFieldData( _T("PRINT_DATE"), _T("NUMERIC"), strPrintDate );
	if ( ids < 0 ) return -5;

	// 2005.06.08 ADD BY PDJ
	// 'x' ��ư���� ����� �Ҹ�Ǿ� �α׳��拚 ������.
	/*
#ifdef __K2UP__
	
	  // üũ�ص� �����ؼ� �ּ�ó����.
	if ( m_pParentMgr->m_pInfo->USER_ID != _T("") )
	{
		ids = pRctDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("���������"), __WFILE__, __LINE__ ) );
		if ( ids < 0 ) return -6;
	}
#endif
	*/
	ids = pRctDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( ids < 0 ) return -7;

	//++2008.10.27 ADD BY PWR {{++
	// ���սý��� ����
// 	ids = pRctDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );
	ids = pRctDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), m_pInfo->MANAGE_CODE );
	if ( ids < 0 ) return -7;
	//--2008.10.27 ADD BY PWR --}}

	ids = pRctDM->MakeInsertFrame( _T("CO_LOAN_RECEIPT_TBL") );
	if ( ids < 0 ) return -8;

	// 2005.03.15 ADD BY PDJ
	// 4. �̷������� ������ �̷�Ű ������Ʈ		
	INT nIdx = 0;
	INT nSelectCnt = 0;	
	INT nTab2Idx;
	CString sLoanRecKey = _T("");

	sLoanKeyList.Empty( );
	pRctDM->InitDBFieldData( );

	nTab2Idx = m_tab2.GetCurSel();
	if ( nTab2Idx == 2 )
	{
		// ��ü�� ���� ���о��� ��� ���
		nSelectCnt = pRctDM->GetRowCount();
		if ( nSelectCnt == 0 ) return 1;
	}
	else
	{
		ids = pRctGrid->SelectMakeList( );
		if ( ids < 0 ) return -1;	
		nSelectCnt = pRctGrid->SelectGetCount( );
		if ( nSelectCnt < 0 ) return -2;
		else if ( 0 == nSelectCnt ) 
		{
			AfxMessageBox( _T("������ �����Ͱ� �����ϴ�.") );
			return 1;
		}
		nIdx = pRctGrid->SelectGetHeadPosition( );
		if ( nIdx < 0 ) return -3;
	}

	for ( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if ( nTab2Idx == 2 ) nIdx = i;

		ids = pRctDM->GetCellData( _T("REC_KEY"), nIdx, sLoanRecKey );
		if ( ids < 0 ) return -5;

		// ������ ��� ���⿵���� Ű��
		if ( bIsLoan  )
		{
			ids = pRctDM->AddDBFieldData( _T("RECEIPT_LOAN_KEY"), _T("NUMERIC"), sRecKey );
			if ( ids < 0 )	return -6;
		}
		else
		{
			//�ݳ��� ��� �ݳ������� Ű��
			ids = pRctDM->AddDBFieldData( _T("RECEIPT_RETURN_KEY"), _T("NUMERIC"), sRecKey );
			if ( ids < 0 )	return -6;
		}
		ids = pRctDM->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sLoanRecKey );
		if ( ids < 0 )	return -7;

		if ( i+1 != nSelectCnt && nTab2Idx != 2 )
		{
			nIdx = pRctGrid->SelectGetNext( );
			if ( nIdx < 0 ) return -8;
		}
	}

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::setDMAndGrid(CESL_DataMgr *pRctDM, CESL_Grid *pRctGrid)
{
	INT ids = 0;
	INT nSelectCnt = 0;
	INT nRowIndex = 0 ;
	INT nTotalCnt = 0 ;	

	// �̷��� ����ϴ� ��� ��µ����͸� �������� �ʴ´�.
	if ( GetTabCtlIdx(1) == 1 ) return 0;
	

	ids = pRctGrid->SelectMakeList( );
	if ( ids < 0 ) return -1;
	
	nSelectCnt = pRctGrid->SelectGetCount( );
	if ( nSelectCnt < 0 ) return -2;

	nSelectCnt = pRctGrid->SelectGetCount( );
	if ( nSelectCnt <= 0 )
	{
		AfxMessageBox( _T("������ �����Ͱ� �����ϴ�.") );
		return 0;
	}

	nTotalCnt = pRctDM->GetRowCount( );
	if ( nTotalCnt < 0 ) return -1;

	/// �ݵ�� �ڿ������� �ؾ� ������ Index�� �����Ű�� �ʰ� �۾��� ������ �� �ִ�. 
	ids = pRctGrid->SelectGetTailPosition( );
	if ( ids < 0 ) return -2;

	for ( INT i = 0 ; i < nSelectCnt ; i++ ) 
	{
		nRowIndex = pRctGrid->SelectGetIdx( );
		if ( nRowIndex < 0 ) return -3;

		/// DM�� �����͸� �����
		ids = pRctDM->DeleteRow( nRowIndex, -1 ); 
		if ( ids < 0 ) return -4;
	
		/// Grid�� �����͸� �����.
		/// Grid�� �ϳ��� ���� �ִ� ���� RemoveItem �Լ��δ� ���� �� ���� ������ 
		/// Grid���� ��ü �����Ϳ� ���õ� ������ ���� ������ �� ���� ��� ����� 
		/// RemoveAll2 �Լ��� ����ߴ�.
		if ( 0 != nTotalCnt ) 
		{
			if ( nTotalCnt == nSelectCnt ) 
			{
				pRctGrid->RemoveAll2( );
				nTotalCnt = 0;
			}
			else
				pRctGrid->RemoveItem( nRowIndex + 1 );
		}

		/// ������ �����Ϳ��� �� ���������͸� ȣ���ϸ� ������ �߻��Ѵ�.
		if ( ( i + 1 ) == nSelectCnt )	break;

		ids = pRctGrid->SelectGetPrev( );
		if ( ids < 0 ) return -5;
	}

	nTotalCnt = pRctDM->GetRowCount( );
	if ( nTotalCnt < 0 ) return -6;
	else if ( 0 != nTotalCnt )
		renameNoInGrid( pRctGrid, nTotalCnt );

	return 0;
}

// Grid�� �����͸� ������ ��ȣ�� �ٽ� ��迭���� �ʱ� ������ ���� ��ȣ�� �Է��Ͽ� 
// ����� ȭ�鿡 ������� �Ѵ�.
VOID CBO_LOC_RECEIPT_PRINT_DLG::renameNoInGrid(CESL_Grid *pRctGrid, INT nTotalCnt)
{
	CString sNo = _T("") ;

	for ( INT i = 0 ; i < nTotalCnt ; i++ ) 
	{
		sNo.Format ( _T("%d"), ( i + 1 ) ) ;

		pRctGrid->SetTextMatrix ( (i + 1 ), 0, sNo ) ;
	}
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnPREVIEW() 
{
	CString	strPrintData;
	INT		ids;	

	if ( m_tab2.GetCurSel() == 2 )
	{
		CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
		CESL_Grid* pTotalRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("GridTotalReceipt") );
		if ( NULL == pTotalRctGrid ) return;
		pTotalRctGrid->SelectMakeList();

		//++2008.05.13 ADD BY PWR {{++
		// ������ �����Ͱ� ������ �̸����� ���ϵ��� ����
		INT	nRowCount;
		nRowCount = pTotalRctGrid->SelectGetCount();
		if ( nRowCount <= 0 )
		{
			AfxMessageBox( _T("������ �ڷᰡ �����ϴ�.") );
			return;
		}
		//--2008.05.13 ADD BY PWR --}}
	}
	
	ids = MakePrintData(strPrintData,_T(""));
	if ( ids > 0 ) return;		
	
	CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(this);
	dlgPreview.m_strDevice = m_strDevice;
	dlgPreview.SetPrintData(strPrintData);	
	
	if ( dlgPreview.DoModal() == IDOK )
	{
		//++2008.05.13 ADD BY PWR {{++
		// ���, ��¾��� ���а� �߰�
		m_strPrintYN = _T("Y");
	
		if ( m_tab2.GetCurSel() == 2 )
		{
			doPrintReceiptSelectAll();
		}
		else
		{
			doPrintReceipt();
		}
		//--2008.05.13 ADD BY PWR --}}
	}	
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{	 
	CheckTab();
	*pResult = 0;
}
VOID CBO_LOC_RECEIPT_PRINT_DLG::OnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnRadSelChange();

	*pResult = 0;
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::CheckTab(INT nType)
{		
	INT		nIdx;	

	if ( GetSafeHwnd() == NULL )
	{
		return; 
	}

	if (nType != 0 )
	{
		m_tab2.SetCurSel(0);
		OnRadSelChange();
	}
	
	nIdx = GetTabCtlIdx(0);

	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
	CString sWhere = _T("");

	if (nType == 1 )
	{
		m_tab.SetCurSel(0);		
		((CButton*) GetDlgItem(IDC_radLOAN))->SetCheck(1);
		((CButton*) GetDlgItem(IDC_radRETURN))->SetCheck(0);
	}
	if (nType == 2 )
	{
		// ����/�ݳ��� �Ѱ����� ����Ҷ��� ������ 0�̴�.
		if ( m_tab.GetItemCount() == 1 ) 
		{
			m_tab.SetCurSel(0);
		}
		else 
		{
			m_tab.SetCurSel(1);
		}
		
		//((CButton*) GetDlgItem(IDC_radLOAN))->SetCheck(1);
		//((CButton*) GetDlgItem(IDC_radRETURN))->SetCheck(0);
	}	



	initData();

	/*
	//CESL_Grid* pRctGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT2));
	CESL_Grid* pRctTotalGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT3));
	
	//DWORD CheckLogGrid;
	DWORD chkTotalGrid;
	//CheckLogGrid = ( pRctGrid->GetStyle() & WS_VISIBLE );
	chkTotalGrid = ( pRctTotalGrid->GetStyle() & WS_VISIBLE );

	//if ( CheckLogGrid ) pRctGrid->ShowWindow(SW_HIDE);
	if ( chkTotalGrid ) pRctTotalGrid->ShowWindow(SW_HIDE);	
	*/

	/*
	ids = getReceiptWhere( sWhere, nIdx );
	if ( ids != 0 ) return;
	
	ids = RefreshDataMgr( sReceiptDMAlias, sWhere );
	if ( ids < 0 ) return;

	ids = AllControlDisplay( sReceiptCMAlias );
	if ( ids < 0 ) OnCancel( );
	*/	

}

VOID CBO_LOC_RECEIPT_PRINT_DLG::ClearData()
{
	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");		
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return;

	pRctDM->FreeData();
	GetDlgItem( IDC_edtUSER_NO )->SetWindowText( _T("") );
	GetDlgItem( IDC_edtUSER_NAME )->SetWindowText( _T("") );	
	
	AllControlDisplay( sReceiptCMAlias );
}

INT CBO_LOC_RECEIPT_PRINT_DLG::MakePrintData(CString &f_pstrData, CString f_strPrintDate)
{
	BOOL bIsLoan = TRUE;
	INT	nIdx, nIdx2;

	INT ids = 0;
	INT nSelectCnt = 0;
	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
	CString sReceiptStr = _T("");

	nIdx2 = GetTabCtlIdx(1);

	// _T("") �̸� �̸������� ���
	if ( f_strPrintDate==_T("") && ( nIdx2 == 0 || nIdx2 == 2 ) )
	{
		// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
		CString strGetData;
		CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
		f_strPrintDate = strGetData.Left(16);
// 		CTime tTime = CLocCommonAPI::GetDBTime( this );
// 		f_strPrintDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 													tTime.GetMonth( ),
// 													tTime.GetDay( ),
// 													tTime.GetHour( ),
// 													tTime.GetMinute( ) );
	}
	
	// �̷��̸� �̸������ �Ӵ� ������ DB������ ������
	if ( nIdx2 == 1 )
	{
		INT	nRowIdx ;
		CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( _T("DM_BO_LOAN_RECEIPT_LOG") );
		if( pRctLogDM == NULL ) return -100;
		CESL_Grid* pRctLogGrid = ( CESL_Grid* ) FindControl( _T("CM_BO_LOC_3100_RECEIPT"), _T("gridLog") );
		if ( NULL == pRctLogGrid ) return -110;

		nRowIdx = pRctLogGrid->GetIdx();

		pRctLogDM->GetCellData( _T("PRINT_DATE"), nRowIdx, f_strPrintDate );

	}

	nIdx = GetTabCtlIdx(0);
	if ( nIdx == 0 ) bIsLoan = TRUE;
	else bIsLoan = FALSE;

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return -1;
	
	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return -2;

	// ���Ϸκ��� ������ ������ ������ ������ ���ڿ��� �о���δ�.
	ids = getReceiptString( sReceiptStr );
	if ( ids != 0 ) return ids;	

	/**********************************************/
	// 1. DM_BO_LOC_3100_RECEIPT ���� ����
	//    (_T("%��_����%"), _T("%��������%"), _T("%�հ�%"), _T("%�߰�����%"))
	/**********************************************/
	ids = setRctTempVariable( pRctDM, pRctGrid, sReceiptStr, bIsLoan );
	if ( ids != 0 ) return ids;
	
	/**********************************************/
	// 2. GETONEVALUEQUERY�� ����
	//    (_T("%��������%"),  _T("%�Ϸù�ȣ%"))
	/**********************************************/
	ids = setRctTempVariable( pRctDM, sReceiptStr );
	if ( ids != 0 ) return ids;

	/**********************************************/
	// 3. �ٷ� ���� ������ �κ�
	//    (_T("%��_������%"),_T("%��_��������%"), _T("%����Ͻ�%"))
	/**********************************************/
	// 2005.03.16
	// ADD BY PDJ ��¹��� �α׿� ������� ��¥�� �д��� ���� ���߱� ����
	// param���� �ѱ��.
	ids = setRctTempVariable( sReceiptStr, bIsLoan, f_strPrintDate );
	if ( ids != 0 ) return ids;
	
	/**********************************************/
	// 4. DM_BO_LOC_3100_MEMBER_INFO ���� ����
	//    (_T("%�����ڹ�ȣ%"), _T("%�������̸�%"), _T("%�μ���%"))
	/**********************************************/
	ids = setRctTempVariable( sReceiptStr );
	if ( ids != 0 ) return ids;

	// 2005.03.03 ADD BY PDJ
	// ������ ��±Ⱑ ���������� �ȵǴ� ����� �ѱ۷� ��ȯ�Ͽ� ����Ѵ�.	
	CMakeSearchData MakeSearchData(m_pParentMgr);
	//INT iCh = MakeSearchData.ChangeHanjaToHangul( sReceiptStr );	//���� �ѱ� ��ȯ

	f_pstrData = sReceiptStr;

	return 0;

}


BEGIN_EVENTSINK_MAP(CBO_LOC_RECEIPT_PRINT_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CPDJ_TestDlg)
	ON_EVENT(CBO_LOC_RECEIPT_PRINT_DLG, IDC_gridRECEIPT2, -600 /* Click */, OnClickgridRECEIPT2, VTS_NONE)
	ON_EVENT(CBO_LOC_RECEIPT_PRINT_DLG, IDC_gridRECEIPT3, -600 /* Click */, OnClickgridRECEIPT3, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnClickgridRECEIPT2() 
{	
	CString sCMAlias = _T("CM_BO_LOC_3100_RECEIPT");	
	CString sGridAlias2 = _T("GridLog");
	CESL_Grid* pRctLogGrid = ( CESL_Grid* ) FindControl( sCMAlias, _T("gridLog") );
	if ( NULL == pRctLogGrid ) return ;
	INT	nIdx;

	nIdx = pRctLogGrid->GetIdx();
	ClickLog(nIdx);
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnClickgridRECEIPT3() 
{	
	INT nCol;	

	CString sCMAlias = _T("CM_BO_LOC_3100_RECEIPT");	
	CString sGridAlias3 = _T("GridTotalReceipt");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	CESL_Grid* pRctTotalGrid = ( CESL_Grid* ) FindControl( sCMAlias, sGridAlias3 );
	
	nCol = pRctTotalGrid->GetCol();

	if ( nCol == 0 ) return;
	initData(FALSE);
	/*
	CString sCMAlias = _T("CM_BO_LOC_3100_RECEIPT");	
	CString sGridAlias3 = _T("GridTotalReceipt");
	CESL_Grid* pRctTotalGrid = ( CESL_Grid* ) FindControl( sCMAlias, sGridAlias3 );
	if ( NULL == pRctTotalGrid ) return ;
	INT	nIdx;

	nIdx = pRctTotalGrid->GetIdx();
	ClickLog(nIdx);
	*/
}

INT CBO_LOC_RECEIPT_PRINT_DLG::GetTabCtlIdx(INT nType)
{
	TCITEM	tcm;
	CString	strTab;
	TCHAR	szBuff[64];
	INT		nIdx;

	tcm.mask = TCIF_TEXT;
	tcm.pszText = szBuff;
	tcm.cchTextMax = sizeof(szBuff);

	if ( nType == 0 )
	{
		nIdx = m_tab.GetCurSel();
		m_tab.GetItem(nIdx, &tcm);
		strTab = tcm.pszText;		

		if ( strTab == _T(" ���� ") )
		{
			return 0;
		}
		else if ( strTab == _T(" �ݳ� ") )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		nIdx = m_tab2.GetCurSel();
		m_tab2.GetItem(nIdx, &tcm);
		strTab = tcm.pszText;
		if ( strTab == _T(" ������������� ") )
		{
			return 0;
		}
		else if ( strTab == _T(" ��µȿ������̷� ") )
		{
			return 1;
		}
		else if ( strTab == _T(" ��ü��������� ") )
		{
			return 2;
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnSETUP()
{		
	CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( _T("DM_BO_LOAN_RECEIPT_LOG") );

	CRECEIPT_PRINT_SET_DLG dlg(this, m_pLMV );
	dlg.m_pRctLogDM = pRctLogDM;
	dlg.DoModal();	
	//++2008.04.14 ADD BY PWR {{++
	// �������� �߰� m_strNotify
	m_pLMV->GetReceiptManageValue(m_strBarcode, m_strBarcodeAll, m_strItem, m_strMode, m_strDevice, m_strPreview, m_strNotify);
	//--2008.04.14 ADD BY PWR --}}

	if ( dlg.m_strPrintTypeSet != m_pLMV->m_sUseReceiptType )
	{
		m_pLMV->m_sUseReceiptType = dlg.m_strPrintTypeSet;
		TabCheck();
	}
	
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnSEARCH()
{
	CString	strSerialNo;

	CRECEIPT_SERARCH_DLG	dlg;
	if ( dlg.DoModal() == IDOK )
	{
		dlg.GetSearchSerialNo(&strSerialNo);
		SearchSerialNo(strSerialNo);
	}	
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnDATE_SEARCH()
{
	initData(TRUE);
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnPRINTCANCEL()
{
	INT nIdxTab2;	
	nIdxTab2 = m_tab2.GetCurSel();

	//++2008.04.14 ADD BY PWR {{++
	// ���, ��¾��� ���а� �߰�
	m_strPrintYN = _T("N");
	//--2008.04.14 ADD BY PWR --}}

	if ( nIdxTab2 == 0 )
	{
		//++2008.05.13 ADD BY PWR {{++
		// Ȯ�� �޽��� ó��
		INT		nRowCount;
		CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( _T("CM_BO_LOC_3100_RECEIPT"), _T("gridReceipt") );
		if ( NULL == pRctGrid ) return;
		pRctGrid->SelectMakeList();	
		nRowCount = pRctGrid->SelectGetCount();
		if ( nRowCount <= 0 )
		{
			AfxMessageBox( _T("������ �ڷᰡ �����ϴ�.") );
			return;
		}
		else
		{
			if( AfxMessageBox(	_T("����������� ���Ͻðڽ��ϱ�?\n")
								_T("(������ ����� �����մϴ�.)\n")
								_T("��� ������ �ڷ�� ��µȿ������̷� �ܿ��� ����� �Ͻ� �� �ֽ��ϴ�.")
								, MB_YESNO ) == IDNO )
			{ return;	}
		}
		//--2008.05.13 ADD BY PWR --}}

		//++2008.04.14 UPDATE BY PWR {{++
		// ��¾����϶����� PRINT_COUNT=0���� �ؼ� �����ϵ��� �Ѵ�.
		//CancelReceipt();
		doPrintReceipt();
		//--2008.04.14 UPDATE BY PWR --}}		
	}
	else if ( nIdxTab2 == 2 )
	{
		//++2008.05.13 ADD BY PWR {{++
		// Ȯ�� �޽��� ó��
		INT		nRowCount;
		CESL_Grid* pTotalRctGrid = ( CESL_Grid* ) FindControl( _T("CM_BO_LOC_3100_RECEIPT"), _T("GridTotalReceipt") );
		if ( NULL == pTotalRctGrid ) return;
		pTotalRctGrid->SelectMakeList();
		nRowCount = pTotalRctGrid->SelectGetCount();
		if ( nRowCount <= 0 )
		{
			AfxMessageBox( _T("������ �ڷᰡ �����ϴ�.") );
			return;
		}
		else
		{
			if( AfxMessageBox(	_T("����������� ���Ͻðڽ��ϱ�?\n")
								_T("(������ ����� �����մϴ�.)\n")
								_T("��� ������ �ڷ�� ��µȿ������̷� �ܿ��� ����� �Ͻ� �� �ֽ��ϴ�.")
								, MB_YESNO ) == IDNO )
			{ return;	}
		}
		//--2008.05.13 ADD BY PWR --}}

		//++2008.04.14 ADD BY PWR {{++
		// ��¾����϶����� PRINT_COUNT=0���� �ؼ� �����ϵ��� �Ѵ�.
		//CancelReceiptAll();
		doPrintReceiptSelectAll();
		//--2008.04.14 ADD BY PWR --}}
	}
}

INT CBO_LOC_RECEIPT_PRINT_DLG::SearchSerialNo(CString f_strSerailNo)
{
	CString sDMLogAlias = _T("DM_BO_LOAN_RECEIPT_LOG");
	CString	strSerialNo;
	INT	ids;
	INT	i;
	INT	nRowCount;

	CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( sDMLogAlias );
	if( pRctLogDM == NULL ) return -100;

	//++2008.05.13 UPDATE BY PWR {{++
	// �Ϸù�ȣ �˻��� �ش� �Ϸù�ȣ�� ���õǵ��� ����.
	CESL_Grid* pRctLogGrid = ( CESL_Grid* ) FindControl( _T("CM_BO_LOC_3100_RECEIPT"), _T("gridLog") );
	if ( NULL == pRctLogGrid ) return -200;

	nRowCount = pRctLogDM->GetRowCount();
	if ( nRowCount < 0 ) return 0;
	
	for ( i = 0 ; i < nRowCount ; i++ )
	{
		ids = pRctLogDM->GetCellData( _T("RCT_SEQ_NO"), i, strSerialNo );
		if ( ids ) return ids;
		
		if ( strSerialNo == f_strSerailNo )
		{
			ids = pRctLogGrid->SetReverse(i);
			if ( ids ) return ids;

			ClickLog(i);
		}
	}	
	//--2008.05.13 UPDATE BY PWR --}}
	
	return 0;	
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::ClickLog(INT f_nIdx)
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3100_RECEIPT");	
	CString sDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sDMLogAlias = _T("DM_BO_LOAN_RECEIPT_LOG");
	CString sGridAlias = _T("GridReceipt");
	CString sGridAlias2 = _T("GridLog");
	CString	strReceiptKey;
	CString	strRecKey;
	CString strWhere;
	INT	nIdx2;
	INT	nRowCount;
	INT	i;
	INT	ids;


	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) return;
	CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( sDMLogAlias );
	if( pRctLogDM == NULL ) return;
	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sDMAlias );
	if( pRctLogDM == NULL ) return;

	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return ;
	CESL_Grid* pRctLogGrid = ( CESL_Grid* ) FindControl( sCMAlias, _T("gridLog") );
	if ( NULL == pRctLogGrid ) return ;
	
	nIdx2 = GetTabCtlIdx(0);
	pRctLogDM->GetCellData(_T("REC_KEY"), f_nIdx, strRecKey );	

	if ( pRctLogDM->GetRowCount() <= 0 ) return;

	if ( nIdx2 == 0 )
	{
		strWhere.Format(_T("RECEIPT_LOAN_KEY = %s"), strRecKey);
	}
	else
	{
		strWhere.Format(_T("RECEIPT_RETURN_KEY = %s"), strRecKey);
	}

	pRctDM->RefreshDataManager(strWhere);
	
	ids = pCM->ControlDisplay(_T("gridReceipt"), 0 );
	if ( ids < 0 ) return;

	nRowCount = pRctDM->GetRowCount();
	if ( nRowCount > 0 )
	{
		for ( i=0 ; i<nRowCount ; i++ )
		{
			pRctGrid->SetAt( i , 0 , _T("V") );				
		}
	}

	/*
	nRowCount = pRctDM->GetRowCount();
	if ( nRowCount > 0 )
	{
		BOOL bFirstRowSel;
		bFirstRowSel = FALSE;
		for ( i=0 ; i<nRowCount ; i++ )
		{
			if ( nIdx2 == 0 )
			{
				ids = pRctDM->GetCellData(_T("RECEIPT_LOAN_KEY"), i, strReceiptKey );
				if ( ids ) return;
			}
			else
			{
				ids = pRctDM->GetCellData(_T("RECEIPT_RETURN_KEY"), i, strReceiptKey );
				if ( ids ) return;
			}

			if ( strReceiptKey == strRecKey )
			{
				pRctGrid->SetAt( i , 0 , _T("V") );
				if ( bFirstRowSel == FALSE )
				{
					// ù��° �ڷḦ �� ���� �ø�					
				}
			}
			else
			{
				pRctGrid->SetAt( i , 0 , _T("") );
			}
		}
	}
	
	*/
	//pGrid->m_nReverse = pGrid->GetIdx();
	//m_pLoan->ViewGrid();

	EFS_END	
}

INT CBO_LOC_RECEIPT_PRINT_DLG::DetailNonReturnBookInfo(CString *strNonReturn, CESL_DataMgr *pDM)
{	
	CString	sTmpData;
	CString sTmpBoInfo;
	CString	sSpace;
	CString strWhere;
	INT nRowCount, i, ids;
	
	if( pDM == NULL )
	{
		AfxMessageBox(_T("FindDM() Error!"));
		return -100;
	}		
	sSpace = _T("                              ");	
	
	nRowCount = pDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{	
		sTmpData.Format(_T("%4d "), i+1 );
		sTmpBoInfo += sTmpData;
		
		ids = pDM->GetCellData( _T("��ǥ��"), i, sTmpData );
		if ( ids < 0 ) return -8;
		// 2010.05.13 UPDATE BY PWR : �̹ݳ���� ��Ϲ�ȣ�� ���ٷ� ���������� ����
		sTmpData = getLimitWord(sTmpData.GetBuffer(0), 24, TRUE);
//		sTmpData = getLimitWord( sTmpData.GetBuffer(0), 24, FALSE );
// 		if ( sTmpData.GetLength() < 24 )
// 		{
// 			sTmpData += sSpace.Left(24 - sTmpData.GetLength() );
// 		}
		sTmpBoInfo += sTmpData;

		ids = pDM->GetCellData( _T("��Ϲ�ȣ"), i, sTmpData );
		if ( ids < 0 ) return -7;
		sTmpData = getLimitWord( sTmpData.GetBuffer(0), 12, TRUE );
		sTmpBoInfo += _T(" ") + sTmpData + _T("<!FP>");
	}
	
	*strNonReturn = sTmpBoInfo;

	return 0;
}

INT	CBO_LOC_RECEIPT_PRINT_DLG::GetLoanReceiptPrintCnt()
{
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );		
	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString	strWhere;
	CString	sUserKey;
	CString	strCnt;
	INT nRowCount, ids;

	ids = GetDataMgrData( sMemberDMAlias, _T("������KEY"), sUserKey, 0 );
	
	if( pLoanDM == NULL )
	{
		AfxMessageBox(_T("FindDM() Error!"));
		return -100;
	}		

	if(!sUserKey.IsEmpty())
	{
		// 2009.03.26 UPDATE BY PWR : VIEW �и�
		strWhere.Format(_T("SELECT (SELECT COUNT(1) FROM LS_WORK_T01 ")
						//2020.04.14 JOB.2020.0033 ADD BY KYJ
						//_T("WHERE USER_KEY=%s AND RECEIPT_PRINT_LOAN_YN!='Y' AND LOAN_DATE IS NOT NULL ")
						_T("WHERE USER_KEY=%s AND (RECEIPT_PRINT_LOAN_YN!='Y' or RECEIPT_PRINT_LOAN_YN IS NULL ) AND LOAN_DATE IS NOT NULL ")
						_T("AND MANAGE_CODE='%s')")
						_T("+")
						_T("(SELECT COUNT(1) FROM CO_LOAN_TBL ")
						//2020.04.14 JOB.2020.0033 ADD BY KYJ
						//_T("WHERE USER_KEY=%s AND RECEIPT_PRINT_LOAN_YN!='Y' AND LOAN_DATE IS NOT NULL ")
						_T("WHERE USER_KEY=%s AND (RECEIPT_PRINT_LOAN_YN!='Y' or RECEIPT_PRINT_LOAN_YN IS NULL ) AND LOAN_DATE IS NOT NULL ")						
						_T("AND MANAGE_CODE='%s') FROM DUAL")
						, sUserKey, m_pInfo->MANAGE_CODE, sUserKey, m_pInfo->MANAGE_CODE );
	// 	// 2008.10.17 UPDATE BY PWR : ���սý��� ����
	// 	strWhere.Format(_T("SELECT COUNT(1) FROM LH_STORAGE_V01 ")
	// 					_T("WHERE USER_KEY=%s AND RECEIPT_PRINT_LOAN_YN!='Y' AND LOAN_DATE IS NOT NULL ")
	// 					_T("AND MANAGE_CODE = '%s'"), sUserKey, m_pInfo->MANAGE_CODE );
		// strWhere.Format(_T("SELECT COUNT(1) FROM LH_STORAGE_V01 WHERE USER_KEY = %s AND RECEIPT_PRINT_LOAN_YN !='Y' AND LOAN_DATE IS NOT NULL "), sUserKey );
		pLoanDM->GetOneValueQuery(strWhere, strCnt );
		nRowCount = _ttoi(strCnt);
		if ( nRowCount > 0 )
		{
			return nRowCount;
		}
	}

	return 0;
}

INT	CBO_LOC_RECEIPT_PRINT_DLG::GetReturnReceiptPrintCnt()
{
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );		
	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString	strWhere;
	CString	sUserKey;
	CString	strCnt;
	INT nRowCount, ids;

	ids = GetDataMgrData( sMemberDMAlias, _T("������KEY"), sUserKey, 0 );
	
	if( pLoanDM == NULL )
	{
		AfxMessageBox(_T("FindDM() Error!"));
		return -100;
	}		

	if(!sUserKey.IsEmpty())
	{
		// 2009.03.26 UPDATE BY PWR : VIEW �и�
		strWhere.Format(_T("SELECT (SELECT COUNT(1) FROM LS_WORK_T01 ")
						_T("WHERE USER_KEY=%s AND RECEIPT_PRINT_RETURN_YN!='Y' AND RETURN_DATE IS NOT NULL ")
						_T("AND MANAGE_CODE='%s')")
						_T("+")
						_T("(SELECT COUNT(1) FROM CO_LOAN_TBL ")
						_T("WHERE USER_KEY=%s AND RECEIPT_PRINT_RETURN_YN!='Y' AND RETURN_DATE IS NOT NULL ")
						_T("AND MANAGE_CODE='%s') FROM DUAL")
						, sUserKey, m_pInfo->MANAGE_CODE, sUserKey, m_pInfo->MANAGE_CODE );
	// 	// 2008.10.17 UPDATE BY PWR : ���սý��� ����
	// 	strWhere.Format(_T("SELECT COUNT(1) FROM LH_STORAGE_V01 ")
	// 					_T("WHERE USER_KEY=%s AND RECEIPT_PRINT_LOAN_YN!='Y' AND LOAN_DATE IS NOT NULL ")
	// 					_T("AND MANAGE_CODE = '%s'"), sUserKey, m_pInfo->MANAGE_CODE );
		// strWhere.Format(_T("SELECT COUNT(1) FROM LH_STORAGE_V01 WHERE USER_KEY = %s AND RECEIPT_PRINT_RETURN_YN !='Y' AND RETURN_DATE IS NOT NULL"), sUserKey );
		pLoanDM->GetOneValueQuery(strWhere, strCnt );
		nRowCount = _ttoi(strCnt);
		if ( nRowCount > 0 )
		{
			return nRowCount;
		}		
	}
	return 0;
}

INT	CBO_LOC_RECEIPT_PRINT_DLG::ReceiptAutoPrint(BOOL bPrintYN)
{	
	INT	nRowCnt;

	//++2008.04.14 ADD BY PWR {{++
	// �̷����� LOG�ʱ�ȭ�ؾ� �Ѵ�.
	CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( _T("DM_BO_LOAN_RECEIPT_LOG") );
	if( pRctLogDM == NULL ) return -110;
	pRctLogDM->FreeData();
	// ���� ������������� ������ �����ϰ� �����ؾ� ��� ȸ���� ����¿������� ����� �� �ִ�.
	m_tab2.SetCurSel(0);
	//++2008.04.14 ADD BY PWR --}}
	
	// ��¿ɼǿ� ���� ����� �� �ֵ��� ����	
	CESL_Grid* pGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT));		
	if( pGrid == NULL ) return 100;		
	
	m_bPrintYN = bPrintYN;
	nRowCnt = GetLoanReceiptPrintCnt();
	if ( nRowCnt > 0 )
	{
		m_tab.SetCurSel(0);	
		initData();		
		doPrintReceipt();
	}

	nRowCnt = GetReturnReceiptPrintCnt();
	if ( nRowCnt > 0 )
	{
		m_tab.SetCurSel(1);
		initData();			
		doPrintReceipt();
	}

	// �������� ��������
	m_bPrintYN = TRUE;	

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::CancelReceipt()
{
	CString sDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
	CString strWhere;
	CString strRecKey;
	INT		nRowCount, nRowIndex;
	INT		nTotalCnt;
	INT		i, ids;
	
	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sDMAlias );
	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return -2;
	
	pRctGrid->SelectMakeList();	
	nRowCount = pRctGrid->SelectGetCount();
	if ( nRowCount <= 0 )
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return 0;
	}
	else
	{
		//++2008.04.14 ADD BY PWR {{++
		// ������ -> ��¾������� ���� 
		//if ( AfxMessageBox("������ �ڷḦ ������ �Ͻðڽ��ϱ�?\n��ҵ� �ڷ�� ���������� �ٽ� ����� �� �����ϴ�.", MB_YESNO) == IDNO )
		if( AfxMessageBox(	_T("����������� ���Ͻðڽ��ϱ�?\n")
							_T("(������ ����� �����մϴ�.)\n")
							_T("��� ������ �ڷ�� ��µȿ������̷� �ܿ��� ����� �Ͻ� �� �ֽ��ϴ�.")
							, MB_YESNO ) == IDNO )
		{ return 0;	}
		//--2008.04.14 ADD BY PWR --}}
	}

	ids = pRctGrid->SelectGetTailPosition( );
	if ( ids < 0 ) return -2;

	nTotalCnt = pRctDM->GetRowCount( );
	if ( nTotalCnt < 0 ) return -1;

	ids = pRctDM->StartFrame( );
	if ( ids < 0 )	return -1;

	pRctDM->InitDBFieldData( );
	for ( i=0 ; i<nRowCount ; i++ )
	{
		nRowIndex = pRctGrid->SelectGetIdx ();		

		/// DM�� �����͸� �����
		pRctDM->GetCellData( _T("REC_KEY"), nRowIndex, strRecKey );

		if ( m_tab.GetCurSel() == 0 )
		{
			ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_LOAN_YN"), _T("STRING"), _T("C") );
			if ( ids < 0 )	return ids;
		}
		else
		{
			ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_RETURN_YN"), _T("STRING"), _T("C") );
			if ( ids < 0 )	return ids;
		}		
		
		ids = pRctDM->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
		if ( ids < 0 )	return -7;

		ids = pRctDM->DeleteRow( nRowIndex, -1 ); 
		if ( ids < 0 ) return -4;
	
		/// Grid�� �����͸� �����.
		/// Grid�� �ϳ��� ���� �ִ� ���� RemoveItem �Լ��δ� ���� �� ���� ������ 
		/// Grid���� ��ü �����Ϳ� ���õ� ������ ���� ������ �� ���� ��� ����� 
		/// RemoveAll2 �Լ��� ����ߴ�.
		if ( 0 != nTotalCnt ) 
		{
			if ( nTotalCnt == nRowCount ) 
			{
				pRctGrid->RemoveAll2( );
				nTotalCnt = 0;
			}
			else
				pRctGrid->RemoveItem( nRowIndex + 1 );
		}
		
		/// ������ �����Ϳ��� �� ���������͸� ȣ���ϸ� ������ �߻��Ѵ�.
		if ( ( i + 1 ) == nRowCount )	break;
		ids = pRctGrid->SelectGetPrev( );
		if ( ids < 0 ) return -5;
	}

	ids = pRctDM->SendFrame ( ) ;
	if ( ids < 0 )	return -5;

	ids = pRctDM->EndFrame ( ) ;
	if ( ids < 0 )	return -6;

	return 0;

}

INT CBO_LOC_RECEIPT_PRINT_DLG::CancelReceiptAll()
{
	CString	sReceiptStr;
	CString strLoanType;
	CString sWhere;
	CString strRecKey;
	INT		nLoanType;
	INT		nTotalCnt;
	INT		ids;	
	INT		i, y;
	INT		nRowCount;
	INT		nRowCount2;
	INT		nRowIndex;	
	BOOL	bIsLoan;

	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sTotalReceiptDMAlias = _T("DM_BO_LOC_3100_TOTAL_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");	

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return -1;

	CESL_DataMgr* pTotalRctDM = ( CESL_DataMgr* ) FindDM( sTotalReceiptDMAlias );
	if ( NULL == pTotalRctDM ) return -1;
	
	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return -2;

	CESL_Grid* pTotalRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("GridTotalReceipt") );
	if ( NULL == pTotalRctGrid ) return -2;

	pTotalRctGrid->SelectMakeList();
	nRowCount = pTotalRctGrid->SelectGetCount();
	if ( nRowCount <= 0 )
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return 0;
	}
	else
	{
		//++2008.04.14 ADD BY PWR {{++
		// ������ -> ��¾������� ���� 
		//if ( AfxMessageBox("������ �ڷḦ ������ �Ͻðڽ��ϱ�?\n��ҵ� �ڷ�� ���������� �ٽ� ����� �� �����ϴ�.", MB_YESNO) == IDNO )
		if( AfxMessageBox(	_T("����������� ���Ͻðڽ��ϱ�?\n")
							_T("(������ ����� �����մϴ�.)\n")
							_T("��� ������ �ڷ�� ��µȿ������̷� �ܿ��� ����� �Ͻ� �� �ֽ��ϴ�.")
							, MB_YESNO ) == IDNO )
		{ return 0;	}
		//--2008.04.14 ADD BY PWR --}}
	}

	ids = pTotalRctGrid->SelectGetTailPosition( );
	if ( ids < 0 ) return -2;

	nTotalCnt = pTotalRctDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{		
		sWhere=_T("");
		nRowIndex = pTotalRctGrid->SelectGetIdx ();
		
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("�������"), strLoanType,  nRowIndex);
		if ( ids ) return ids;
		
		if ( strLoanType == _T("����") ) 
		{
			bIsLoan = TRUE;	
			nLoanType = 0;
		}
		else 
		{		
			bIsLoan = FALSE;
			nLoanType = 1;
		}
		
		ids = getReceiptWhere( sWhere, nLoanType,nRowIndex );
		if ( ids > 0 )
		{
			// ȭ�� �ʱ�ȭ
			ClearData();	
		}
		if ( ids != 0 ) return (ids);	
		
		if ( !sWhere.IsEmpty() )
		{
			ids = RefreshDataMgr( sReceiptDMAlias, sWhere );
			if ( ids < 0 ) return -2;	
		}	
		
		nRowCount2 = pRctDM->GetRowCount();	
		ids = pRctDM->StartFrame( );
		if ( ids < 0 )	return -1;
		
		pRctDM->InitDBFieldData( );
		for ( y=0 ; y<nRowCount2 ; y++ )
		{
			/// DM�� �����͸� �����
			pRctDM->GetCellData( _T("REC_KEY"), y, strRecKey );
			
			if ( bIsLoan )
			{
				ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_LOAN_YN"), _T("STRING"), _T("C") );
				if ( ids < 0 )	return ids;
			}
			else
			{
				ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_RETURN_YN"), _T("STRING"), _T("C") );
				if ( ids < 0 )	return ids;
			}		
			
			ids = pRctDM->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
			if ( ids < 0 )	return -7;
		}

		ids = pRctDM->SendFrame ( ) ;
		if ( ids < 0 )	return -5;

		if ( 0 != nTotalCnt ) 
		{
			if ( nTotalCnt == nRowCount ) 
			{
				pTotalRctGrid->RemoveAll2( );
				nTotalCnt = 0;
			}
			else
				pTotalRctGrid->RemoveItem( nRowIndex + 1 );
		}

		ids = pTotalRctDM->DeleteRow( nRowIndex, -1 ); 
		if ( ids < 0 ) return -4;
		
		/// ������ �����Ϳ��� �� ���������͸� ȣ���ϸ� ������ �߻��Ѵ�.
		if ( ( i + 1 ) == nRowCount )	break;
		ids = pTotalRctGrid->SelectGetPrev( );
		if ( ids < 0 ) return -5;
	}	

	ids = pRctDM->EndFrame ( ) ;
	if ( ids < 0 )	return -6;

	pRctDM->FreeData();
	CESL_ControlMgr *pCM = FindCM( sReceiptCMAlias );
	if( pCM == NULL ) return -110;
	pCM->ControlDisplay(_T("gridReceipt"), 0 );

	return 0;
}



INT CBO_LOC_RECEIPT_PRINT_DLG::TabCheck()
{
	CString sRctUseType = _T("");	

	m_tab.DeleteAllItems();
		

	sRctUseType = m_pLMV->m_sUseReceiptType;
	if ( _T("YY") != sRctUseType )
	{
		if ( 'Y' == sRctUseType.GetAt(0) )	m_tab.InsertItem( 0, _T(" ���� ") );
		else m_tab.InsertItem( 0, _T(" �ݳ� ") );
	}
	else
	{
		m_tab.InsertItem( 0, _T(" ���� ") );
		m_tab.InsertItem( 1, _T(" �ݳ� ") );
	}

	CString sTotalReceiptDMAlias = _T("DM_BO_LOC_3100_TOTAL_RECEIPT");
	CESL_DataMgr* pTotalDM = ( CESL_DataMgr* ) FindDM( sTotalReceiptDMAlias );
	pTotalDM->FreeData();

	initData( );

	return 0;
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::RECEIPT_CUT()
{
	LP_PrintCut( );
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-28 ~ 2005-12-28
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
 	 
   [   �Լ�����   ] 
      �α��� ������� ���������� ���Ѵ�.

   [  PARAMETERS  ]
      sGetManageCode : [OUT] ��������

   [ RETURN VALUE ]
      0    : ���� 
      ���� : ����
--------------------------------------------------------------------------------------------*/
INT CBO_LOC_RECEIPT_PRINT_DLG::GetLoginerManageCode(CString &sGetManageCode)
{
	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CESL_DataMgr* pDM = FindDM( sDMAlias );
    if ( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::GetLoginManageCode()") );

	CString sOneValueQuery;
//	sOneValueQuery.Format( _T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE REC_KEY = %s"), m_pInfo->USER_PK );
//	ids = pDM->GetOneValueQuery( sOneValueQuery, sGetManageCode );
// 	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("CBO_LOC_RECEIPT_PRINT_DLG::GetLoginManageCode()") );

	sGetManageCode = m_pInfo->MANAGE_CODE;
	sGetManageCode.TrimLeft();sGetManageCode.TrimRight();

	// ���������� ���ٸ� ������ �������������� �Է��Ѵ�.
	// �ԷµǴ� ������ ���������ڵ�� ����Ʈ�� ������ ���̴�.
	if ( sGetManageCode.IsEmpty() )
	{
		CString sCodeName = _T("��������");
				
		ids = m_pInfo->pCodeMgr->GetDefaultCode( sCodeName,  sGetManageCode);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("CBO_LOC_RECEIPT_PRINT_DLG::GetLoginerManageCode()") );		
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ Func Desc ]
//   ��Ÿ������ XML ������ �ε��Ѵ�.
/////////////////////////////////////////////////////////////////////////////
INT CBO_LOC_RECEIPT_PRINT_DLG::InitXML_Style(CString strFileName/*=_T("")*/)
{
	CoInitialize(NULL);
	IXMLDOMDocumentPtr pDom(_T("Microsoft.XMLDOM"), NULL, CLSCTX_INPROC_SERVER);
	pDom->async = FALSE;
	if( strFileName.IsEmpty() )
	{
		strFileName = _T("..\\Cfg\\LoanReturn\\ReceiptStyle.xml");
	}

	CComVariant varFileName = (LPCTSTR)strFileName;
	if (!pDom->load(varFileName))
	{
		AfxMessageBox( _T("NOT FOUND XML Style File!!") );
		return -1;
	}
	
	/// ��� ���� ����� �ʱ�ȭ �� ��ų�� ������. ��.��
	m_pStyleDom = pDom;
	
	// ���ڿ� ������ ������ ������.
//	if ( MakeStringStyle() < 0 )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::InitXML_Style()") );			

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::MakeStringStyle(CString& strValue)
{
	INT ids;	
	// ����� ����Ʈ
	IXMLDOMNodeListPtr StyleList = m_pStyleDom->childNodes;

	for ( LONG i = 0;i < StyleList->length;i++ )
	{
		IXMLDOMNodePtr pObjectNode = StyleList->item[i];
		ids = MakeStringNode( pObjectNode, strValue );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::MakeStringStyle()") );
		
		pObjectNode.Release();
	}

	StyleList.Release();

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::MakeStringNode(IXMLDOMNodePtr pDom, CString &strValue)
{
	INT ids;	
	CComBSTR nameString;

	pDom->get_baseName(&nameString);
	
	if ( CString( nameString ) == _T("ITEM") )
	{
		ids = MakeStringItem( pDom, strValue );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::MakeStringNode()") );
	}	
	else
	{
		/// ��尡 �ִٸ� ���ȣ���̴�!
		IXMLDOMNodeListPtr pNodeList = pDom->childNodes;
		for (LONG i = 0; i < pNodeList->length; i++)
		{
			IXMLDOMNodePtr pObjectNode = pNodeList->item[i];
			ids = MakeStringNode( pObjectNode, strValue );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("CBO_LOC_RECEIPT_PRINT_DLG::MakeStringNode()") );

			pObjectNode.Release();
		}

		pNodeList.Release();
	}	

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::MakeStringItem(IXMLDOMNodePtr pItemDom, CString &strValue)
{
	CString strForward;
	CString strData;
	CString strBackward;
	CString strItem;
	// ���� ITEM ��Ʈ���� ������.
	IXMLDOMNodeListPtr pItemList = pItemDom->childNodes;

	for ( LONG i = 0;i < pItemList->length;i++ )
	{
		IXMLDOMNodePtr pObjectNode = pItemList->item[i];
		
		CComBSTR text;
		CComBSTR nameString;

		pObjectNode->get_baseName(&nameString);
		pObjectNode->get_text(&text);

		CString strNameString(nameString);
		CString strText(text);	
		
		// Forward, Backward��  [] �� <>�� Replace;
		strText.TrimLeft( );
		strText.TrimRight( );

		if ( strNameString == _T("FORWARD") )
		{
			strText.Replace( _T("["), _T("<") );
			strText.Replace( _T("]"), _T(">") );
			strForward = strText;
		}
		else if ( strNameString == _T("VALUE") )	strData = strText;
		else if ( strNameString == _T("BACKWARD") )
		{
			strText.Replace( _T("["), _T("<") );
			strText.Replace( _T("]"), _T(">") );
			strBackward = strText;
		}

		nameString.Empty();
		text.Empty();		

		pObjectNode.Release();
	}

	pItemList.Release();
	
	strItem = strForward + strData + strBackward;
	strValue += strItem;
	
	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::ChangePrintDomData(CString strTagetData, CString strValue)
{
	INT ids;	
	// ����� ����Ʈ
	IXMLDOMNodeListPtr PrintList = m_pStyleDom->childNodes;

	for ( LONG i = 0;i < PrintList->length;i++ )
	{
		IXMLDOMNodePtr pObjectNode = PrintList->item[i];
		ids = ChangePrintDomData( pObjectNode, strTagetData, strValue );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::ChangePrintDomData()") );
	
		pObjectNode.Release();
	}

	PrintList.Release();

	return 0;	
}

INT CBO_LOC_RECEIPT_PRINT_DLG::ChangePrintDomData(IXMLDOMNodePtr pDom, CString strTagetData, CString strValue)
{
	INT ids;	
	CComBSTR nameString;	
	pDom->get_baseName(&nameString);	

	if ( CString( nameString ) == _T("ITEM") )
	{
		// �����͸� �ٲٸ� �ȴ�.
		CComBSTR text;

		IXMLDOMNodeListPtr pNodeList = pDom->childNodes;
		
		for ( LONG i = 0; i < pNodeList->length; i++)
		{
			IXMLDOMNodePtr pObjectNode = pNodeList->item[i];

			pObjectNode->get_baseName(&nameString);			
			pObjectNode->get_text(&text);
				
			// ���� �����Ͽ� put ����
			if ( CString( nameString ) == _T("VALUE") )
			{
				CString VALUE( text );
				VALUE.Replace( strTagetData, strValue );

				// �̹� ��忡 ���� ������ �ȴ�
				pObjectNode->put_text( VALUE.AllocSysString() );			
			}
	
				pObjectNode.Release();
			}

		pNodeList.Release();		
	}	
	else
	{
		/// ��尡 �ִٸ� ���ȣ���̴�!
		IXMLDOMNodeListPtr pNodeList = pDom->childNodes;
		for (LONG i = 0; i < pNodeList->length; i++)
		{
			IXMLDOMNodePtr pObjectNode = pNodeList->item[i];
			ids = ChangePrintDomData( pObjectNode, strTagetData, strValue );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("CBO_LOC_RECEIPT_PRINT_DLG::ChangePrintDomData()") );

			pObjectNode.Release();
		}

		pNodeList.Release();
	}
	

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::ChangeReceiptStrTOXML(CString &strXMLReceipt)
{
	/*
	// �޸𸮻��� XML�� ���Ϸ� �����ϰ� 
	m_pStyleDom->save((LPCTSTR)CString(_T("..\\cfg\\LoanReturn\\Temp.xml")));
	// ����� ������ ��� ���ڿ��� �ѱ��.
	CStdioFile file;
	if ( !file.Open(_T("..\\cfg\\LoanReturn\\Temp.xml"), CFile::modeRead | CFile::typeBinary ) )
	{
		AfxMessageBox ( _T("NOT FOUND STYLE FILE!!") );
		return -1;
	}
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
	
		
	strXMLReceipt.Empty();
	CString strLine;

	while ( file.ReadString( strLine ) )
	{
		strXMLReceipt += strLine;	
	}
	*/

	strXMLReceipt.Empty();

	BSTR bText = m_pStyleDom->Getxml();
	if( bText == NULL )
	{
		strXMLReceipt = _T("");
	}
	else
	{
		strXMLReceipt = CString(bText);
	}
   
	return 0;
}

// 2007.05.08 add by pdw 
// �ݳ�ó���� �������� ���
INT CBO_LOC_RECEIPT_PRINT_DLG::PrintReservationInfo(CString strDMAlias, INT nRowIndex, BOOL bPreview)
{
	INT		ids;

	//++2008.12.10 ADD BY KSJ {{++
	INT		nPrintIndex		=	0;	//������ ��� �� �ڷ��� Index
	INT		nSelectIndex	=	0;	//������Grid���� ������ Index
	//--2008.12.10 ADD BY KSJ --}}

	// 1. init xml
	CString strFile;
	strFile = _T("..\\Cfg\\LoanReturn\\ReservationStyle.xml");
	ids = InitXML_Style(strFile);
	if( 0>ids ) return ids;

	// 2. make print string
	// 2.1 book info
	CESL_DataMgr* pLoanInfo;
	CString strCallNo, strTitle, strRegNo;
	CString strUserNo, strUserName, strHP, strPhoneHome;		
	CString strReservationDate, strReservationExpireDate;
	
	if ( strDMAlias != _T("") ) // �켱���⿹��(DM_BO_LOC_1170)
	{
		pLoanInfo = FindDM( strDMAlias );
		if( !pLoanInfo ) return -101;

		pLoanInfo->GetCellData( _T("û����ȣ"), nRowIndex, strCallNo );
		pLoanInfo->GetCellData( _T("����"), nRowIndex, strTitle );
		pLoanInfo->GetCellData( _T("��Ϲ�ȣ"), nRowIndex, strRegNo );
		// 2.2 user info		
		pLoanInfo->GetCellData( _T("�����ڹ�ȣ"), nRowIndex, strUserNo );
		//++2008.06.02 ADD BY PWR {{++
		// �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ�� 
		ChangeSecuriyCivilNo( strUserNo );
		//--2008.06.02 ADD BY PWR --}}
		pLoanInfo->GetCellData( _T("�����ڸ�"), nRowIndex, strUserName );

		//JOB.2020.0010 EDIT BY KYJ �����ڸ� �� ǥ���ϵ��� �ּ�ó��
// 		//++2009.01.19 ADD BY PWR {{++
// 		// �����ڹ�ȣ�� üũ�ؼ� �ֹι�ȣ�ϰ�� �����ڹ�ȣ�� �̸� ���ڸ��� ��ȣȭ�Ѵ�.
// 		if(!strUserName.IsEmpty())
//  		{
// 			strUserName.Format(_T("%s*"),strUserName.Left(strUserName.GetLength()-1));
// 		}
// 		//++2009.01.19 ADD BY PWR --}}

		pLoanInfo->GetCellData( _T("�ڵ���"), nRowIndex, strHP );
		pLoanInfo->GetCellData( _T("����ȭ��ȣ"), nRowIndex, strPhoneHome );
		// 2.3 loan info		
		pLoanInfo->GetCellData( _T("������"), nRowIndex, strReservationDate );
		pLoanInfo->GetCellData( _T("���ุ����"), nRowIndex, strReservationExpireDate );
	}
	else
	{
		//++2008.12.10 UPDATE BY KSJ {{++
		// ���ټ��� ��������� ��� �߰�
		if( !(m_pLMV->m_bVolReserve) )
		{
			CESL_DataMgr* pBookInfo = FindDM( _T("DM_BO_LOC_3170_BOOK_INFO"));
			if( !pBookInfo )	return	-101;

			pBookInfo->GetCellData( _T("��ǥ��"),	nPrintIndex, strTitle );
			pBookInfo->GetCellData( _T("û����ȣ"),	nPrintIndex, strCallNo );
			pBookInfo->GetCellData( _T("��Ϲ�ȣ"),	nPrintIndex, strRegNo );

			pLoanInfo = FindDM( _T("DM_BO_LOC_3170_RESERVE_INFO") );
			if( !pLoanInfo )	return	-102;
		}
		else	//���ټ��� ��� �� ���
		{
			////////////////////////////////////////////////////////////////////////
			//1.������ ���� DM
			pLoanInfo = FindDM( _T("DM_BO_LOC_3170_VR_RESERVE_INFO") );
			if( !pLoanInfo )	return	-102;
			////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////
			//2.�����ڷ� ���� DM
			CESL_DataMgr* pBookInfo = FindDM( _T("DM_BO_LOC_3170_VR_BOOK_INFO"));
			if( !pBookInfo )	return	-101;
			////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////
			//3.������ ���� �׸��忡�� ���õ� ������ ã��     
			CESL_Grid*	pVRReserveGrid	=	(CESL_Grid*)FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRReserveGrid") );
			if( pVRReserveGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintReservationInfo") );

			nSelectIndex	=	pVRReserveGrid->GetIdx();
			////////////////////////////////////////////////////////////////////////
			
			////////////////////////////////////////////////////////////////////////
			//4.�������� �� �ڷ��� ��Ϲ�ȣ ��������
			CString	strMarkRegNo;
			pLoanInfo->GetCellData( _T("��Ϲ�ȣ")	, nSelectIndex , strMarkRegNo );
			////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////
			//5.���������� �� �ڷ� ã��
			CESL_Grid*	pVRBookInfoGrid	=	(CESL_Grid*)FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRBookInfoGrid") );
			if( pVRReserveGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintReservationInfo") );

			INT		nVRBookInfoGridCnt	=	pVRBookInfoGrid->GetCount();	//�����ڷ��� ����
			CString	strTempReg;		//REG_NO �ӽ� ���� ����

			for( INT i=0 ; i<nVRBookInfoGridCnt; i++ )
			{
				pBookInfo->GetCellData( _T("��Ϲ�ȣ")	, i , strTempReg );
				if(	strMarkRegNo	==	strTempReg)
				{
					nPrintIndex	=	i;
					break;
				}
			}
			////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////
			//6.���������� �� �ڷ� ���� ��������
			pBookInfo->GetCellData( _T("����")		, nPrintIndex , strTitle );
			pBookInfo->GetCellData( _T("û����ȣ")	, nPrintIndex , strCallNo);
			pBookInfo->GetCellData( _T("��Ϲ�ȣ")	, nPrintIndex , strRegNo );
			////////////////////////////////////////////////////////////////////////
		}
		/*
		CESL_DataMgr* pBookInfo = FindDM( _T("DM_BO_LOC_3170_BOOK_INFO"));
		if( !pBookInfo ) return -101;

		pLoanInfo = FindDM( _T("DM_BO_LOC_3170_RESERVE_INFO") );
		if( !pLoanInfo ) return -102;

		pBookInfo->GetCellData( _T("û����ȣ"), 0, strCallNo );
		pBookInfo->GetCellData( _T("��ǥ��"), 0, strTitle );
		pBookInfo->GetCellData( _T("��Ϲ�ȣ"), 0, strRegNo );
		*/
		//--2008.12.10 UPDATE BY KSJ --}}
		// ��������
		// ���� 
		strTitle = getLimitWord( strTitle.GetBuffer(0), 24, TRUE );
		// û����ȣ
		strCallNo = getLimitWord( strCallNo.GetBuffer(0), 24, TRUE );
		
		// 2.2 user info		
		pLoanInfo->GetCellData( _T("�����ڹ�ȣ"),	nSelectIndex, strUserNo );
		//++2008.06.02 ADD BY PWR {{++
		// �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ�� 
		ChangeSecuriyCivilNo( strUserNo );
		//--2008.06.02 ADD BY PWR --}}
		pLoanInfo->GetCellData( _T("������"),		nSelectIndex, strUserName );
		//JOB.2020.0010 EDIT BY KYJ �����ڸ� �� ǥ���ϵ��� �ּ�ó��
// 		//++2009.01.19 ADD BY PWR {{++
// 		// �����ڹ�ȣ�� üũ�ؼ� �ֹι�ȣ�ϰ�� �����ڹ�ȣ�� �̸� ���ڸ��� ��ȣȭ�Ѵ�.
// 		if(!strUserName.IsEmpty())
//  		{
// 			strUserName.Format(_T("%s*"),strUserName.Left(strUserName.GetLength()-1));
// 		}
// 		//++2009.01.19 ADD BY PWR --}}

		pLoanInfo->GetCellData( _T("�ڵ���"),		nSelectIndex, strHP );
		pLoanInfo->GetCellData( _T("��ȭ��ȣ"),		nSelectIndex, strPhoneHome );
		// 2.3 loan info
		pLoanInfo->GetCellData( _T("������"),		nSelectIndex, strReservationDate );
		pLoanInfo->GetCellData( _T("���ุ����"),	nSelectIndex, strReservationExpireDate );
	}

	// 2.4 print date
	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString f_strPrintDate;
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
	f_strPrintDate = strGetData.Left(16);
// 	CString f_strPrintDate;
// 	CTime tTime = CLocCommonAPI::GetDBTime( this );
// 	f_strPrintDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 													tTime.GetMonth( ),
// 													tTime.GetDay( ),
// 													tTime.GetHour( ),
// 													tTime.GetMinute( ) );
	// �μ�� ��Ʈ�� ����
	CString strRCT;
	ids = MakeStringStyle( strRCT );
	if( 0>ids ) return ids;

	// 2009.01.22 ADD BY PWR : SMS�߼��� ����
	// SMS��뿩�ο� ���� ó��(�������̸� 'SMS�߼���'�� ���������ʴ´�.)
	// �켱���⿹�࿡���� ���������ʴ´�.
	INT nIsSMS = 0;
	nIsSMS = m_pInfo->GetSMSUseMode(); // 0:N, 1:Y
	if( nIsSMS == 0 || strDMAlias == _T("DM_BO_LOC_1170") ) 
	{ // SMS������
		strRCT.Replace(_T("�� SMS �߼��� : %SMS�߼���%<!LN><!FP>"), _T(""));
	}
	else
	{ // SMS�����
		CString sDate, sQuery, sLoanKey;
		pLoanInfo->GetCellData(_T("����/�ݳ�����KEY"), nSelectIndex, sLoanKey);
		// 17/11/01 ������ : LMS�߰��� ���� ó�� - �����ڷ��ǥ ��½� LMS�߼۳�¥ ǥ�� ��û
/*//BEFORE-----------------------------------------------------------------------------
		sQuery.Format(_T("SELECT TO_CHAR(MAX(SEND_TIME),'YYYY/MM/DD HH24:MI') FROM MN_SMS_LOG_TBL WHERE LOAN_KEY=%s"), sLoanKey);	
*///AFTER------------------------------------------------------------------------------
		sQuery.Format(_T("SELECT TO_CHAR(MAX(SEND_TIME),'YYYY/MM/DD HH24:MI') FROM MN_SMS_LOG_VIEW WHERE LOAN_KEY=%s"), sLoanKey);	
/*//END-------------------------------------------------------------------------------*/		
		pLoanInfo->GetOneValueQuery(sQuery, sDate);
		if(sDate.IsEmpty()) 
		{ // SMS�߼��̷¿� LS_WORK_T01�� �ش� �����̷��� Ű�� ����� �̷��� ���ٸ� �߼��̷��� ���ٴ°� 
			strRCT.Replace(_T("�� SMS �߼��� : %SMS�߼���%<!LN><!FP>"), _T(""));
		}
		else
		{ // �̷��� Ȯ�εǾ��� �߼����� �����ش�.
			strRCT.Replace( _T("%SMS�߼���%"), sDate );
			ChangePrintDomData( _T("%SMS�߼���%"), sDate );
		}
	}

	//JOB.2020.0030 ADD BY KYJ : �������� ��ȣ ��ȭ 
	if( m_pLMV->m_sLoanUserAccessMode.IsEmpty() || m_pLMV->m_sLoanUserAccessMode == _T("1") ) strUserName = _T("***");
	
	// ������ �Է�
	strRCT.Replace( _T("%����Ͻ�%"), f_strPrintDate );
	ChangePrintDomData( _T("%����Ͻ�%"), f_strPrintDate );	
	strRCT.Replace( _T("%�����ڹ�ȣ%"), strUserNo );
	ChangePrintDomData( _T("%�����ڹ�ȣ%"), strUserNo );	
	strRCT.Replace( _T("%�������̸�%"), strUserName );
	ChangePrintDomData( _T("%�������̸�%"), strUserName );
	strRCT.Replace( _T("%����ȭ%"), strPhoneHome );
	ChangePrintDomData( _T("%����ȭ%"), strPhoneHome );
	strRCT.Replace( _T("%�ڵ���%"), strHP );
	ChangePrintDomData( _T("%�ڵ���%"), strHP );

	strRCT.Replace( _T("%û����ȣ%"), strCallNo );
	ChangePrintDomData( _T("%û����ȣ%"), strCallNo );
	strRCT.Replace( _T("%����%"), strTitle );
	ChangePrintDomData( _T("%����%"), strTitle );
	strRCT.Replace( _T("%��Ϲ�ȣ%"), strRegNo );
	ChangePrintDomData( _T("%��Ϲ�ȣ%"), strRegNo );

	strRCT.Replace( _T("%������%"), strReservationDate );
	ChangePrintDomData( _T("%������%"), strReservationDate );
	strRCT.Replace( _T("%���ุ����%"), strReservationExpireDate );
	ChangePrintDomData( _T("%���ุ����%"), strReservationExpireDate );

	// 2. ������ �̸� ����
	if( m_strLibCode.IsEmpty() )
	{
		CString	strWhere;
// 		strWhere = _T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");		
		strWhere.Format(_T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);		
		pLoanInfo->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}
	strRCT.Replace( _T("%��������%"), m_strLibCode );
	ChangePrintDomData( _T("%��������%"), m_strLibCode );	

	if ( strDMAlias != _T("") )
	{
		CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(this);
		dlgPreview.m_strDevice = m_strDevice;
		dlgPreview.SetPrintData(strRCT);
		
		if ( bPreview )
		{
			if ( dlgPreview.DoModal() == IDOK )
			{
				ids = printReceipt( strRCT );
				if ( ids < 0 ) return -5;
				
				// ����~
				LP_StartUp();
				LP_PrintCut( );	
				LP_CleanUp();
			}	
		}
		else
		{
			ids = printReceipt( strRCT );
			if ( ids < 0 ) return -5;
			
			// ����~
			LP_StartUp();
			LP_PrintCut( );	
			LP_CleanUp();		
		}
		
	}
	else
	{
		CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(this);
		dlgPreview.m_strDevice = m_strDevice;
		dlgPreview.SetPrintData(strRCT);
		if ( dlgPreview.DoModal() == IDOK )
		{
			ids = printReceipt( strRCT );
			if ( ids < 0 ) return -5;
			
			// ����~
			LP_StartUp();
			LP_PrintCut( );	
			LP_CleanUp();
		}

	}
	
	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::GetLPStartUp()
{
	INT ids;
	while(TRUE)
	{
		// StartUp
		ids = LP_StartUp();
		if(0 == ids)
		{
			break;
		}
		else
		{
			ids = MessageBox(_T("�߰ߵ� ��±Ⱑ �����ϴ�.\r\n����� ��õ� �Ͻðڽ��ϱ�?"), _T("���������"), MB_YESNO | MB_ICONQUESTION);
			if(ids != IDYES)
			{
				return -1;
			}
		}
	}
	while(TRUE)
	{
		// GetStatus
		ids = LP_GetStatus();
		if(0 == ids)
		{
			break;
		}
		else
		{
			ids = MessageBox(_T("��±��� ������ �����ְų� ������ �Ǿ� ���� �ʽ��ϴ�.\r\n����� ��õ� �Ͻðڽ��ϱ�?"), _T("���������"), MB_YESNO | MB_ICONQUESTION);
			if(ids != IDYES)
			{
				return -2;
			}
		}
	}
	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::GetReservationProcPrint(CESL_DataMgr *pDM, INT nIndex, CString &strPrintData)
{
	INT ids;
 	ids = InitXML_Style(_T("..\\Cfg\\LoanReturn\\ReservationStyle2.xml"));
 	if(0 > ids) 
 	{
 		return ids;
 	}
 
 	CString strCallNo, strTitle, strRegNo, strUserNo, strUserName;		
 	CString strReservationDate, strReservationExpireDate;
 	pDM->GetCellData(_T("����"), nIndex, strTitle);
 	strTitle = getLimitWord(strTitle.GetBuffer(0), 24, TRUE);
 	pDM->GetCellData(_T("û����ȣ"), nIndex, strCallNo);
 	strCallNo = getLimitWord(strCallNo.GetBuffer(0), 24, TRUE);
 	pDM->GetCellData(_T("��Ϲ�ȣ"), nIndex, strRegNo);
 	pDM->GetCellData(_T("�����ڹ�ȣǥ��"), nIndex, strUserNo);
 	pDM->GetCellData(_T("�̸�"), nIndex, strUserName);
 	pDM->GetCellData(_T("������"), nIndex, strReservationDate);
 	pDM->GetCellData(_T("���ุ����"), nIndex, strReservationExpireDate);
 	
 	CString strRCT;
 	ids = MakeStringStyle(strRCT);
 	if(0 > ids)
 	{
 		return ids;
 	}
 	strRCT.Replace(_T("%�����ڹ�ȣ%"), strUserNo);
 	ChangePrintDomData(_T("%�����ڹ�ȣ%"), strUserNo);	
 	strRCT.Replace(_T("%�������̸�%"), strUserName);
 	ChangePrintDomData(_T("%�������̸�%"), strUserName);
 	strRCT.Replace(_T("%û����ȣ%"), strCallNo);
 	ChangePrintDomData(_T("%û����ȣ%"), strCallNo);
 	strRCT.Replace(_T("%����%"), strTitle);
 	ChangePrintDomData(_T("%����%"), strTitle);
 	strRCT.Replace(_T("%��Ϲ�ȣ%"), strRegNo);
 	ChangePrintDomData(_T("%��Ϲ�ȣ%"), strRegNo);
 	strRCT.Replace(_T("%������%"), strReservationDate);
 	ChangePrintDomData(_T("%������%"), strReservationDate);
	// SMS�߼��� ǥ��
	INT nIsSMS = 0;
	nIsSMS = m_pInfo->GetSMSUseMode(); // 0:N, 1:Y
	if(nIsSMS == 0) 
	{ // SMS������
		strRCT.Replace(_T("�� SMS �߼��� : %SMS�߼���%<!LN><!FP>"), _T(""));
	}
	else
	{ // SMS�����
		CString sDate, sQuery, sLoanKey;
		pDM->GetCellData(_T("LOAN_KEY"), nIndex, sLoanKey);

		// 17/11/01 ������ : LMS�߰��� ���� ó�� - �����ڷ��ǥ ��½� LMS�߼۳�¥ ǥ�� ��û
/*//BEFORE-----------------------------------------------------------------------------
		sQuery.Format(_T("SELECT TO_CHAR(MAX(SEND_TIME),'YYYY/MM/DD HH24:MI') FROM MN_SMS_LOG_TBL WHERE LOAN_KEY=%s"), sLoanKey);	
*///AFTER------------------------------------------------------------------------------
		sQuery.Format(_T("SELECT TO_CHAR(MAX(SEND_TIME),'YYYY/MM/DD HH24:MI') FROM MN_SMS_LOG_VIEW WHERE LOAN_KEY=%s"), sLoanKey);
/*//END-------------------------------------------------------------------------------*/
		pDM->GetOneValueQuery(sQuery, sDate);
	
		if(sDate.IsEmpty()) 
		{ // SMS�߼��̷¿� LS_WORK_T01�� �ش� �����̷��� Ű�� ����� �̷��� ���ٸ� �߼��̷��� ���ٴ°� 
			strRCT.Replace(_T("�� SMS �߼��� : %SMS�߼���%<!LN><!FP>"), _T(""));
		}
		else
		{ // �̷��� Ȯ�εǾ��� �߼����� �����ش�.
			strRCT.Replace( _T("%SMS�߼���%"), sDate );
			ChangePrintDomData( _T("%SMS�߼���%"), sDate );
		}
	}
// 	strRCT.Replace(_T("�� SMS �߼��� : %SMS�߼���%<!LN><!FP>"), _T(""));
 	strRCT.Replace(_T("%���ุ����%"), strReservationExpireDate);
 	ChangePrintDomData(_T("%���ุ����%"), strReservationExpireDate);
 	if(m_strLibCode.IsEmpty())
 	{
 		CString	strQuery;
 		strQuery.Format(_T("SELECT LIB_NAME FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE"));
 		pDM->GetOneValueQuery(strQuery, m_strLibCode);
 	}
 	strRCT.Replace(_T("%��������%"), m_strLibCode);
 	ChangePrintDomData(_T("%��������%"), m_strLibCode);	
 
 	strPrintData = strRCT;
 	
 	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::ReservationObjectPrint(CESL_Grid* pGrid, CESL_DataMgr *pDM)
{
	if(NULL == pDM)
	{
		return -1;
	}
	if(NULL == pGrid)
	{
		return -2;
	}
	pGrid->SelectGetHeadPosition();

	INT nIndex;
	CString strReceiptString;
	strReceiptString = _T("<!FC><!FF>[�������ڷ�]<!LF><!LC><!FP><!FP>");
	for(INT i = 0; i < pGrid->SelectGetCount(); i++)
	{
		nIndex = pGrid->SelectGetIdx();

		CString strTitle;
		pDM->GetCellData(_T("����"), nIndex, strTitle);
		strTitle = getLimitWord(strTitle.GetBuffer(0), 30, FALSE);
		CString strCallNo;
		pDM->GetCellData(_T("û����ȣ"), nIndex, strCallNo);
		CString strRegNo;
		pDM->GetCellData(_T("��Ϲ�ȣ"), nIndex, strRegNo);
		CString strReservationDate;
		pDM->GetCellData(_T("������"), nIndex, strReservationDate);

		CString strData;
		strData.Format(
			_T("<!FC>------------------------------------------<!LC><!FP>")
			_T("<!FN>%d. ���� : %s<!LN><!FP>")
			_T("<!FN>   ��Ϲ�ȣ : %s<!LN><!FP>")
			_T("<!FN>   û����ȣ : %s<!LN><!FP>")
			_T("<!FN>   ������ : %s<!LN><!FP>")
			, i+1, strTitle, strRegNo, strCallNo, strReservationDate);

		strReceiptString += strData;

		pGrid->SelectGetNext();
	}

	printReceipt(strReceiptString);

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::ReservationShelfPrint(CESL_Grid* pGrid, CESL_DataMgr *pDM)
{
	if(NULL == pDM)
	{
		return -1;
	}
	if(NULL == pGrid)
	{
		return -2;
	}
	pGrid->SelectGetHeadPosition();

	INT nIndex;
	CString strReceiptString;
	strReceiptString = _T("<!FC><!FF>[��������ڷ�]<!LF><!LC><!FP><!FP>");
	for(INT i = 0; i < pGrid->SelectGetCount(); i++)
	{
		nIndex = pGrid->SelectGetIdx();

		CString strTitle;
		pDM->GetCellData(_T("����"), nIndex, strTitle);
		strTitle = getLimitWord(strTitle.GetBuffer(0), 30, FALSE);
		CString strCancelClass;
		pDM->GetCellData(_T("�������"), nIndex, strCancelClass);
		CString strRegNo;
		pDM->GetCellData(_T("��Ϲ�ȣ"), nIndex, strRegNo);
		CString strReservationExpireDate;
		pDM->GetCellData(_T("���ุ����"), nIndex, strReservationExpireDate);
		CString strCallNo;
		pDM->GetCellData(_T("û����ȣ"), nIndex, strCallNo);
		CString strReservationObject;
		pDM->GetCellData(_T("������"), nIndex, strReservationObject);
		if(!strReservationObject.IsEmpty())
		{
			strReservationObject = _T("[������]");
		}

		CString strData;
		strData.Format(
			_T("<!FC>------------------------------------------<!LC><!FP>")
			_T("<!FN>%d. ���� : %s<!LN><!FP>")
			_T("<!FN>   û����ȣ : %s<!LN><!FP>")
			_T("<!FN>   ��Ϲ�ȣ : %s   [%s]<!LN><!FP>")
			_T("<!FN>   ���ุ���� : %s   %s<!LN><!FP>")
			, i+1, strTitle, strCallNo, strRegNo, strCancelClass, strReservationExpireDate, strReservationObject);

		strReceiptString += strData;

		pGrid->SelectGetNext();
	}

	printReceipt(strReceiptString);

	return 0;
}

// 2002.02.01 ADD BY PWR
// <<����������ý���>>
// **��ü�� ���������
INT CBO_LOC_RECEIPT_PRINT_DLG::PrintFeeInfo(BOOL bPreview, BOOL bPrint, INT nIndex)
{
	INT ids;

	ids = InitXML_Style( _T("..\\Cfg\\LoanReturn\\FeeStyle.xml") );

    // 1. ������ ����===================================================================================
	CESL_DataMgr *pUserDM = FindDM( _T("DM_FEE_ARREAR_RECEIPT_USER") );
	CESL_DataMgr *pLoanDM = FindDM( _T("DM_FEE_ARREAR_RECEIPT_LOAN") );

	CString sQuery, sNumber, sUserNo, sName, sDate, sSetDate, sLoanPossibleDate, sPaymentDate, sTempInfo;
	CString sTotalBook, sTotalMoney, sLib, sTotalDelayDay, sTotalApplyDelayday, sLoanStopDate, sBasicArrear;
	CString sBlank = _T("                   ");
	
	//++2009.01.02 ADD BY KSJ {{++
	//Where ���ǿ� MAKE_YEAR �߰�
	//CTime		tCurrentCTime = CLocCommonAPI::GetDBTime(m_pParentMgr);
	//CString		strCurrentYear;
	//strCurrentYear.Format( _T("%d") , tCurrentCTime.GetYear());
	//--2009.01.02 ADD BY KSJ --}}

	//�Ϸù�ȣSELECT
	//sQuery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE=UDF_MANAGE_CODE AND MAKE_YEAR='%s'") , strCurrentYear );
// 	sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE=UDF_MANAGE_CODE");
	sQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
	ids = pUserDM->GetOneValueQuery( sQuery, sNumber );
	if ( ids < 0 ) 
	{
		//++2009.01.02 UPDATE BY KSJ {{++
		//���ڵ尡 ���� ���, ���ڵ� ���� �� ó���Ѵ�
		if( -4007 == ids )
		{
			pUserDM->StartFrame();
			pUserDM->InitDBFieldData();
			
			ids = pUserDM->AddDBFieldData( _T("REC_KEY"),			_T("NUMERIC"), _T("ESL_SEQ.NEXTVAL") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("KIND_CODE"),			_T("STRING"), _T("RCT_NO") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("PUBLISH_FORM_CODE"),	_T("STRING"), _T("CO") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );
			
			//ids = pUserDM->AddDBFieldData( _T("MAKE_YEAR"),		_T("STRING"),	strCurrentYear );
			//if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("DIVISION_VALUE"),	_T("STRING"),	_T("������_�Ϸù�ȣ") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("LAST_NUMBER"),	_T("NUMERIC"),	_T("0") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("CREATE_DATE"),	_T("NUMERIC"),		_T("SYSDATE") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("CREATE_WORKER"),	_T("STRING"),	m_pInfo->USER_ID );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("MANAGE_CODE"),	_T("STRING"),	m_pInfo->MANAGE_CODE  );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->MakeInsertFrame( _T("CO_LAST_NUMBER_TBL") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			pUserDM->SendFrame();
			pUserDM->EndFrame();
		}
		else
		{
			return -1;
		}
			//--2009.01.02 UPDATE BY KSJ --}}
	}
	if ( ids < 0 ) return -1;

	//�����ڹ�ȣ
	pUserDM->GetCellData( _T("�����ڹ�ȣ"), 0, sUserNo);
	//++2009.01.19 ADD BY PWR {{++
	// �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ�� 
	ChangeSecuriyCivilNo( sUserNo );
	//--2009.01.19 ADD BY PWR --}}

	//�������̸�
	pUserDM->GetCellData( _T("����"), nIndex, sName);
	//++2008.12.02 ADD BY PJW {{++
	// �����ڹ�ȣ�� üũ�ؼ� �ֹι�ȣ�ϰ�� �����ڹ�ȣ�� �̸� ���ڸ��� ��ȣȭ�Ѵ�.
	if(!sName.IsEmpty())
 	{
		sName.Format(_T("%s*"),sName.Left(sName.GetLength()-1));
	}
	//++2008.12.02 ADD BY PJW --}}
	
	
	pUserDM->GetCellData( _T("������"), nIndex, sPaymentDate);
	//ó���Ͻ�
	sSetDate = sPaymentDate;
	//������
	sPaymentDate = sPaymentDate.Left(10);
	//�������������(SYSTEM)
	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
	sDate = strGetData.Left(16);
// 	CTime tTime = CLocCommonAPI::GetDBTime( this );
// 	sDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 													tTime.GetMonth( ),
// 													tTime.GetDay( ),
// 													tTime.GetHour( ),
// 													tTime.GetMinute( ) );
	
	//��������
	setRctTemp(sTempInfo);
	//�ѱ�
	sTotalBook.Format( _T("%d"), pLoanDM->GetRowCount() );
	//�ѿ�ü��
	//sTotalDelayDay
	pUserDM->GetCellData( _T("�ѿ�ü��"), nIndex, sTotalDelayDay );
	sTotalDelayDay.Format( _T("%d"), _ttoi(sTotalDelayDay) );
	//���뿬ü��
	//sTotalApplyDelayday
	pUserDM->GetCellData( _T("���뿬ü��"), nIndex, sTotalApplyDelayday );
	sTotalApplyDelayday.Format( _T("%d"), _ttoi(sTotalApplyDelayday) );
	//�Ѱ�
	pUserDM->GetCellData( _T("�������α�"), nIndex, sTotalMoney );
	SetMoneyFormat(sTotalMoney);
	//�⺻��ü��
	pLoanDM->GetCellData( _T("�⺻��ü��"), 0, sBasicArrear );
	//����������
	//sLoanStopDate
	pUserDM->GetCellData( _T("����������"), nIndex, sLoanStopDate );
	//���Ⱑ������
	if(sLoanStopDate==_T(""))
	{
		sLoanPossibleDate = sPaymentDate;
	}
	else
	{
		COleDateTimeSpan tOneDay( 1, 0 , 0 , 0 );	// �Ϸ�
		COleDateTime tLoanPossibleDate( _ttoi(sLoanStopDate.Mid(0,4)) , _ttoi(sLoanStopDate.Mid(5,2)) , _ttoi(sLoanStopDate.Mid(8,2)) , 0 , 0 , 0 );
		tLoanPossibleDate = tLoanPossibleDate + tOneDay;
		sLoanPossibleDate = tLoanPossibleDate.Format( _T("%Y/%m/%d") );
	}
	//������
	if(m_strLibCode.IsEmpty())
	{
		CString	strWhere;
// 		strWhere = _T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");		
		strWhere.Format(_T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);		
		pUserDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}

    // 2. ���ڿ� ġȯ ================================================================================
	CString strRCT;
	ids = MakeStringStyle( strRCT );
	if( 0>ids ) return ids;
	
	//�Ϸù�ȣ
	sNumber.Format( _T("%010d"), ( _ttoi(sNumber) + 1 ) );
	strRCT.Replace( _T("%�Ϸù�ȣ%"), sNumber );
	ChangePrintDomData( _T("%�Ϸù�ȣ%"), sNumber );	
	//�����ڹ�ȣ
	strRCT.Replace( _T("%�����ڹ�ȣ%"), sUserNo );
	ChangePrintDomData( _T("%�����ڹ�ȣ%"), sUserNo );
	//�������̸�
	strRCT.Replace( _T("%�������̸�%"), sName );
	ChangePrintDomData( _T("%�������̸�%"), sName);
	//������
	strRCT.Replace( _T("%������%"), sPaymentDate );
	ChangePrintDomData( _T("%������%"), sPaymentDate);
	//����Ͻ�
	strRCT.Replace( _T("%����Ͻ�%"), sDate );
	ChangePrintDomData( _T("%����Ͻ�%"), sDate);	
	//��������
	strRCT.Replace( _T("%��������%"), sTempInfo );
	ChangePrintDomData( _T("%��������%"), sTempInfo );	
	//�ѱ�
	strRCT.Replace( _T("%�ѱ�%"), sTotalBook );
	ChangePrintDomData( _T("%�ѱ�%"), sTotalBook );
	//�ѿ�ü��
	strRCT.Replace( _T("%�ѿ�ü��%"), sTotalDelayDay );
	ChangePrintDomData( _T("%�ѿ�ü��%"), sTotalDelayDay );
	//���뿬ü��
	strRCT.Replace( _T("%���뿬ü��%"), sTotalApplyDelayday );
	ChangePrintDomData( _T("%���뿬ü��%"), sTotalApplyDelayday );
	//�Ѱ�
	strRCT.Replace( _T("%�Ѱ�%"), sTotalMoney );
	ChangePrintDomData( _T("%�Ѱ�%"), sTotalMoney );
	//����������
	strRCT.Replace( _T("%����������%"), sLoanStopDate );
	ChangePrintDomData( _T("%����������%"), sLoanStopDate );
	//�߰�����
	strRCT.Replace( _T("%�߰�����%"), _T("") );
	ChangePrintDomData( _T("%�߰�����%"), _T("") );
	//��������
	strRCT.Replace( _T("%��������%"), m_strLibCode );
	ChangePrintDomData( _T("%��������%"), m_strLibCode );
	//����
	strRCT.Replace( _T("%����%"), sBlank );
	ChangePrintDomData( _T("%����%"), sBlank );
	//ó���Ͻ�
	strRCT.Replace( _T("%ó���Ͻ�%"), sSetDate );
	ChangePrintDomData( _T("%ó���Ͻ�%"), sSetDate );
	//�⺻��ü��
	strRCT.Replace( _T("%�⺻��ü��%"), sBasicArrear);
	ChangePrintDomData( _T("%�⺻��ü��%"), sBasicArrear );
	//���Ⱑ����
	strRCT.Replace( _T("%���Ⱑ����%"), sLoanPossibleDate );
	ChangePrintDomData( _T("%���Ⱑ����%"), sLoanPossibleDate );

	if( bPrint==TRUE )
	{
		// 3. ��� =====================================================================================
		CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(m_pParentMgr);
		dlgPreview.m_strDevice = m_strDevice;
		dlgPreview.SetPrintData(strRCT);
		
		if( bPreview )
		{
			if( ( dlgPreview.DoModal() ) == IDOK )
			{
				//�̸����� ȭ�鿡�� ���
				ids = printReceipt( strRCT );
				if ( ids ) return -5;
				
				LP_StartUp();
				LP_PrintCut( );	
				LP_CleanUp();
			}
			else
			{
				//�̸����� ȭ�鿡�� �ݱ� 
				return 100;
			}
		}
		else
		{
			ids = printReceipt( strRCT );
			if ( ids ) return -5;
			
			LP_StartUp();
			LP_PrintCut();	
			LP_CleanUp();		
		}
	}

	//�Ϸù�ȣUPDATE
// 	sQuery.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE=UDF_MANAGE_CODE;")
// 					, _ttoi( sNumber ) );
	sQuery.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE = '%s';")
					, _ttoi( sNumber ), m_pInfo->MANAGE_CODE );
	pUserDM->StartFrame();
	pUserDM->InitDBFieldData();	
	pUserDM->AddFrame(sQuery);
	pUserDM->SendFrame(TRUE);	
	ids = pUserDM->EndFrame();
	m_strRCT = strRCT;

	return 0;
}

// 2002.02.01 ADD BY PWR
// <<����������ý���>>
// **ȸ������߱� ���������
INT CBO_LOC_RECEIPT_PRINT_DLG::PrintFeeReCardInfo(BOOL bPreview, BOOL bPrint)
{
	INT ids;

	ids = InitXML_Style( _T("..\\Cfg\\LoanReturn\\FeeReCardStyle.xml") );

	// 1. ������ ���� =============================================================================
	CESL_DataMgr* pDM = FindDM( _T("DM_FEE_RECARD_RECEIPT") );
	CString sNumber, sUserNo, sName, sClass, sCard, sPayment, sRecKey, sDate;
	if ( pDM->GetRowCount() > 0 )
	{
		pDM->GetCellData( _T("�Ϸù�ȣ")	, 0, sNumber);
		pDM->GetCellData( _T("ȸ����ȣ")	, 0, sUserNo);
		pDM->GetCellData( _T("����")		, 0, sName);
		pDM->GetCellData( _T("ȸ�����")	, 0, sClass);
		pDM->GetCellData( _T("ī������")	, 0, sCard);
		pDM->GetCellData( _T("���αݾ�")	, 0, sPayment);
		pDM->GetCellData( _T("REC_KEY")		, 0, sRecKey);
		
		//++2009.01.19 ADD BY PWR {{++
		// �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ�� 
		ChangeSecuriyCivilNo( sUserNo );
		//--2009.01.19 ADD BY PWR --}}

		//++2008.12.02 ADD BY PJW {{++
		// �����ڹ�ȣ�� üũ�ؼ� �̸� ���ڸ��� ��ȣȭ�Ѵ�.
		if(!sName.IsEmpty())
 		{
			sName.Format(_T("%s*"),sName.Left(sName.GetLength()-1));
		}
		//++2008.12.02 ADD BY PJW --}}
	}
	//������
	if(m_strLibCode.IsEmpty())
	{
		CString	strWhere;
// 		strWhere = _T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");	
		strWhere.Format(_T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);	
		pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}
	//�������������(SYSTEM)
	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
	sDate = strGetData.Left(16);
// 	CTime tTime = CLocCommonAPI::GetDBTime( this );
// 	sDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 													tTime.GetMonth( ),
// 													tTime.GetDay( ),
// 													tTime.GetHour( ),
// 													tTime.GetMinute( ) );

	// 2. ���ڿ� ġȯ =============================================================================
	CString strRCT;
	ids = MakeStringStyle( strRCT );
	if( 0>ids ) return ids;
	
	//�Ϸù�ȣ
	sNumber.Format( _T("%010d"),  _ttoi(sNumber) );
	strRCT.Replace( _T("%�Ϸù�ȣ%"), sNumber );
	ChangePrintDomData( _T("%�Ϸù�ȣ%"), sNumber );	
	//�����ڹ�ȣ
	strRCT.Replace( _T("%�����ڹ�ȣ%"), sUserNo );
	ChangePrintDomData( _T("%�����ڹ�ȣ%"), sUserNo );
	//�������̸�
	strRCT.Replace( _T("%�������̸�%"), sName );
	ChangePrintDomData( _T("%�������̸�%"), sName);
	//ȸ�����
	strRCT.Replace( _T("%ȸ�����%"), sClass );
	ChangePrintDomData( _T("%ȸ�����%"), sClass);
	//ī������
	strRCT.Replace( _T("%ī������%"), sCard );
	ChangePrintDomData( _T("%ī������%"), sCard);	
	//���αݾ�
	strRCT.Replace( _T("%���αݾ�%"), sPayment );
	ChangePrintDomData( _T("%���αݾ�%"), sPayment );
	//����Ͻ�
	strRCT.Replace( _T("%����Ͻ�%"), sDate );
	ChangePrintDomData( _T("%����Ͻ�%"), sDate );
	//��������
	strRCT.Replace( _T("%��������%"), m_strLibCode );
	ChangePrintDomData( _T("%��������%"), m_strLibCode );

	if( bPrint==TRUE )
	{
		// 3. ��� =====================================================================================
		CBO_LOC_RECEIPT_PREVIEW_DLG dlg(this);
		dlg.m_strDevice = m_strDevice;
		dlg.SetPrintData(strRCT);
		
		if( bPreview ) 
		{
			if( ( dlg.DoModal() ) == IDOK )
			{
				//�̸����� ȭ�鿡�� ���
				ids = printReceipt( strRCT );
				if ( ids ) return -5;
				
				LP_StartUp();
				LP_PrintCut( );	
				LP_CleanUp();
			}
			else
			{
				//�̸����� ȭ�鿡�� �ݱ� 
				return 100;
			}
		}
		else
		{
			ids = printReceipt( strRCT );
			if ( ids ) return -5;
			
			LP_StartUp();
			LP_PrintCut();	
			LP_CleanUp();		
		}
	}
	
	m_strRCT = strRCT;

	return 0;
}

// 2002.02.13 ADD BY PWR
// <<����������ý���>>
// **�������������� ���������
INT CBO_LOC_RECEIPT_PRINT_DLG::PrintLoanStopCancelInfo(BOOL bPreview, BOOL bPrint)
{
	INT ids;

	ids = InitXML_Style( _T("..\\Cfg\\LoanReturn\\FeeLoanStopCancelStyle.xml") );

	// 1. ������ ���� =============================================================================
	CESL_DataMgr* pDM = FindDM( _T("DM_FEE_LOANSTOPCANCEL_RECEIPT") );
	CString sNumber, sUserNo, sName, sPayment, sRecKey, sDate, sPaymentDate, sSetDate, sBasicArrear;
	CString sLoanStopDate, sStopDay, sHolyDay, sDissolveDay, sLoanStopPeriod;
	CString sSpace = _T("       ");
	CString sBlank = _T("                   ");

	if ( pDM->GetRowCount() > 0 )
	{
		pDM->GetCellData( _T("REC_KEY")		, 0, sRecKey		);
		pDM->GetCellData( _T("�Ϸù�ȣ")	, 0, sNumber		);
		pDM->GetCellData( _T("ȸ����ȣ")	, 0, sUserNo		);
		pDM->GetCellData( _T("����")		, 0, sName			);
		pDM->GetCellData( _T("������")		, 0, sPaymentDate	);
		pDM->GetCellData( _T("����������")	, 0, sLoanStopDate	);
		pDM->GetCellData( _T("�����ϼ�")	, 0, sStopDay		);
		sStopDay.Format( _T("%d"), _ttoi( sStopDay ) );
		pDM->GetCellData( _T("�ް��ϼ�")	, 0, sHolyDay		);
		if( sHolyDay.Compare( _T("�������") ) != 0 ) 
			sHolyDay.Format( _T("%d"), _ttoi(sHolyDay) );
		pDM->GetCellData( _T("�����ϼ�")	, 0, sDissolveDay	);
		sDissolveDay.Format( _T("%d"), _ttoi( sDissolveDay ) );
		pDM->GetCellData( _T("���αݾ�")	, 0, sPayment		);
		//���αݾ� �� ���߱�
		SetMoneyFormat(sPayment);
		//++2009.01.19 ADD BY PWR {{++
		// �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ�� 
		ChangeSecuriyCivilNo( sUserNo );
		//--2009.01.19 ADD BY PWR --}}
		//++2008.12.02 ADD BY PJW {{++
		// �����ڹ�ȣ�� üũ�ؼ� �̸� ���ڸ��� ��ȣȭ�Ѵ�.
		if(!sName.IsEmpty())
 		{
			sName.Format(_T("%s*"),sName.Left(sName.GetLength()-1));
		}		
		//++2008.12.02 ADD BY PJW --}}
		//++2009.01.19 ADD BY PWR {{++
		// ���������Ⱓ (������~����������)
		sLoanStopPeriod.Format(_T("%s �� %s"), sPaymentDate, sLoanStopDate);
		//--2009.01.19 ADD BY PWR --}}
		pDM->GetCellData( _T("ó���Ͻ�")	, 0, sSetDate		);
		pDM->GetCellData( _T("�⺻��ü��")	, 0, sBasicArrear	);
	}
	//������
	if(m_strLibCode.IsEmpty())
	{
		CString	strWhere;
// 		strWhere = _T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");		
		strWhere.Format(_T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);		
		pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}
	//�������������(SYSTEM)
	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
	sDate = strGetData.Left(16);
// 	CTime tTime = CLocCommonAPI::GetDBTime( this );
// 	sDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 													tTime.GetMonth( ),
// 													tTime.GetDay( ),
// 													tTime.GetHour( ),
// 													tTime.GetMinute( ) );

	// 2. ���ڿ� ġȯ =============================================================================
	CString strRCT;
	ids = MakeStringStyle( strRCT );
	if( 0>ids ) return ids;
	
	//�Ϸù�ȣ
	sNumber.Format( _T("%010d"), _ttoi(sNumber) );
	strRCT.Replace( _T("%�Ϸù�ȣ%"), sNumber );
	ChangePrintDomData( _T("%�Ϸù�ȣ%"), sNumber );	
	//�����ڹ�ȣ
	strRCT.Replace( _T("%�����ڹ�ȣ%"), sUserNo );
	ChangePrintDomData( _T("%�����ڹ�ȣ%"), sUserNo );
	//�������̸�
	strRCT.Replace( _T("%�������̸�%"), sName );
	ChangePrintDomData( _T("%�������̸�%"), sName );
	//������
	strRCT.Replace( _T("%������%"), sPaymentDate );
	ChangePrintDomData( _T("%������%"), sPaymentDate );	
	//����������
	strRCT.Replace( _T("%����������%"), sLoanStopDate );
	ChangePrintDomData( _T("%����������%"), sLoanStopDate );
	//�����ϼ�
	strRCT.Replace( _T("%�����ϼ�%"), sStopDay );
	ChangePrintDomData( _T("%�����ϼ�%"), sStopDay );
	//�ް��ϼ�
	strRCT.Replace( _T("%�ް��ϼ�%"), sHolyDay );
	ChangePrintDomData( _T("%�ް��ϼ�%"), sHolyDay );
	//�����ϼ�
	strRCT.Replace( _T("%�����ϼ�%"), sDissolveDay );
	ChangePrintDomData( _T("%�����ϼ�%"), sDissolveDay );
	//���αݾ�
	strRCT.Replace( _T("%���αݾ�%"), sPayment );
	ChangePrintDomData( _T("%���αݾ�%"), sPayment );
	//����Ͻ�
	strRCT.Replace( _T("%����Ͻ�%"), sDate );
	ChangePrintDomData( _T("%����Ͻ�%"), sDate );
	//��������
	strRCT.Replace( _T("%��������%"), m_strLibCode );
	ChangePrintDomData( _T("%��������%"), m_strLibCode );
	//����
	strRCT.Replace( _T("%����%"), sBlank );
	ChangePrintDomData( _T("%����%"), sBlank );
	//++2009.01.19 ADD BY PWR {{++
	//���������Ⱓ
	strRCT.Replace( _T("%���������Ⱓ%"), sLoanStopPeriod );
	ChangePrintDomData( _T("%���������Ⱓ%"), sLoanStopPeriod );
	//--2009.01.19 ADD BY PWR --}}	
	strRCT.Replace( _T("%ó���Ͻ�%"), sSetDate );
	ChangePrintDomData( _T("%ó���Ͻ�%"), sSetDate );
	strRCT.Replace( _T("%�⺻��ü��%"), sBasicArrear );
	ChangePrintDomData( _T("%�⺻��ü��%"), sBasicArrear );

	if( bPrint==TRUE )
	{
		// 3. ��� =====================================================================================
		CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(this);
		dlgPreview.m_strDevice = m_strDevice;
		dlgPreview.SetPrintData(strRCT);
		
		if( bPreview )
		{
			if( ( dlgPreview.DoModal() ) == IDOK )
			{
				//�̸����� ȭ�鿡�� ���
				ids = printReceipt( strRCT );
				if ( ids ) return -5;
				
				LP_StartUp();
				LP_PrintCut( );	
				LP_CleanUp();
			}
			else
			{
				//�̸����� ȭ�鿡�� �ݱ� 
				return 100;
			}
		}
		else
		{
			ids = printReceipt( strRCT );
			if ( ids ) return -5;
			
			LP_StartUp();
			LP_PrintCut();	
			LP_CleanUp();		
		}
	}
	m_strRCT = strRCT;

	return 0;
}

// 2002.02.01 ADD BY PWR
// <<����������ý���>>
// **��ü�� ��������
INT CBO_LOC_RECEIPT_PRINT_DLG::setRctTemp(CString &sBookInfo)
{
	INT ids = 0;
	INT nTotalCnt = 0;

	CString sTmpData = _T("");    
	CString sTotalCnt = _T("");        // ����å��
	CString sTmpBoInfo = _T("");

	CESL_DataMgr *pLoanDM = FindDM( _T("DM_FEE_ARREAR_RECEIPT_LOAN") );

	for ( int i = 0 ; i < pLoanDM->GetRowCount(); i++ )
	{
		sTmpBoInfo.Format( _T("%4d"), ++nTotalCnt );
		sBookInfo += _T("<!FN>") + sTmpBoInfo;

		ids = pLoanDM->GetCellData( _T("û����ȣ"), i, sTmpData );
		if ( ids < 0 ) return -6;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 24, TRUE );
		sBookInfo += _T(" ") + sTmpBoInfo;

		ids = pLoanDM->GetCellData( _T("��Ϲ�ȣ"), i, sTmpData );
		if ( ids < 0 ) return -7;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 12, TRUE );
		sBookInfo += _T(" ") + sTmpBoInfo + _T("<!LN><!FP><!FN>");

		ids = pLoanDM->GetCellData( _T("����"), i, sTmpData );
		if ( ids < 0 ) return -8;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 18, FALSE );

		ids = pLoanDM->GetCellData( _T("������"), i, sTmpData );
		if ( ids < 0 ) return -9;
		sTmpData = sTmpBoInfo + _T("/") + sTmpData;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 28, TRUE );
		sBookInfo += " " + sTmpBoInfo;
		
		ids = pLoanDM->GetCellData( _T("���αݾ�"), i, sTmpData );
		if ( ids < 0 ) return -10;
		SetMoneyFormat(sTmpData);
		sTmpData += _T("��");
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 12, FALSE );
		
		sBookInfo += _T(" ") + sTmpBoInfo + _T("<!LN><!FP>");
	}

	return 0;
}

// 2008.04.14 ADD BY PWR 
// **õ������ ǥ��
VOID CBO_LOC_RECEIPT_PRINT_DLG::SetMoneyFormat(CString &sMoney)
{
	sMoney.Replace( _T(","), _T("") );

	CString strBuf = sMoney;
	strBuf.Replace( _T(","), _T("") );

	int nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize-1);

	sMoney = pszFormattedNumber;

	if (pszFormattedNumber)
		delete [] pszFormattedNumber;
}

// 2008.04.14 ADD BY PWR
// ** �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ��
VOID CBO_LOC_RECEIPT_PRINT_DLG::ChangeSecuriyCivilNo(CString &sUserNo)
{
	if(_T("Y") != m_strCivilNoViewYN)
	{
		CLocCommonAPI::ChangeSecuriyUserNo(this, sUserNo, m_sIsUnityLoanService);
	}
}

// 2008.04.14 ADD BY PWR
// 2008.05.13 UPDATE BY PWR ( üũ���� )
// ** ��¥��Ʈ�ѿ��� ��¥�� ���� �����´�.
VOID CBO_LOC_RECEIPT_PRINT_DLG::GetDateControl()
{
	SYSTEMTIME	tTime;
	DWORD		dwResult;

	// TO
	CDateTimeCtrl *pDateTo	 = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE1 );
	dwResult = pDateTo->GetTime( &tTime );
	if (dwResult != GDT_VALID )
	{
		m_sDateTo = _T("");
	}
	else
	{
		m_sDateTo.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	}
	// FROM
	CDateTimeCtrl *pDateFrom = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE2 );
	dwResult = pDateFrom->GetTime( &tTime );
	if (dwResult != GDT_VALID ) 
	{
		m_sDateFrom = _T("");
	}
	else
	{
		m_sDateFrom.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	}
}

// 2008.04.14 ADD BY PWR
// ** ��¥��Ʈ���� �����ϰ����� �����Ѵ�.
VOID CBO_LOC_RECEIPT_PRINT_DLG::SetDateControl()
{
	CTime tFrom = CTime::GetCurrentTime();
	CTime tTo = tFrom - CTimeSpan(7,0,0,0);
	
	// TO
	CDateTimeCtrl *pDateTo	 = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE1 );
	pDateTo->SetTime( &tTo );

	// FROM
	CDateTimeCtrl *pDateFrom = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE2 );
	pDateFrom->SetTime( &tFrom );
}

// 2008.04.14 ADD BY PWR
// ** �Ⱓ�˻��� �Ѵ�.
VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnDATESEARCH()
{
	INT nIndex = GetTabCtlIdx(1);
	//��ü
	if( nIndex == 2 )
	{
		TabCheck();
	}
	else
	{	
		OnRadSelChange();
	}
}


HBRUSH CBO_LOC_RECEIPT_PRINT_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}


//=====================================================
//2008.12.02 UPDATE BY PJW : �ֹι�ȣ���� �ƴ����� üũ�Ѵ�.
BOOL CBO_LOC_RECEIPT_PRINT_DLG::IsRRN(CString sRRN)
{

    if( sRRN.IsEmpty() == TRUE )	return TRUE;

    sRRN.Replace(_T(" "), _T("")); //���� ����
//	sRRN.Replace("-", ""); //'-'���� ����

    if( sRRN.GetLength() != 14 )	return FALSE; //���� �˻�

    if( sRRN[6] != '-' )	return FALSE; //'-' ��ġ �˻�

    int Gender;

    Gender = sRRN[7]-'0'; //����(1,3) ����(2,4)
    if( Gender < 1 || Gender > 4 )	return FALSE;

    int Val = 0;
    int nWeight[] = {

        2, 3, 4, 5, 6, 7, 

        0, //'-' �� ��ȿȭ ����ġ

        8, 9, 2, 3, 4, 5

    };

    for (int i=0; i<14-1; i++) 

        Val += (sRRN[i] - '0') * nWeight[i];

    Val = 11 - (Val % 11);

    Val = Val % 10;

    if (Val == (sRRN[13]-'0')) return TRUE;

    return FALSE;
}
//=====================================================

INT CBO_LOC_RECEIPT_PRINT_DLG::Print(CString& strTitle, CMSHFlexGrid* pGrid, INT aPrintColNoList[], INT nPrintColNoCnt)
{
	if(NULL == pGrid)
	{
		return -1000;
	}
	CString strReceiptString;
	strReceiptString.Format(_T("<!FC><!FF>[%s]<!LF><!LC><!FP><!FP>"), strTitle);
	
	CString *astrHeader = NULL, strData;
	astrHeader = new CString[nPrintColNoCnt];

	INT nCheckCnt = 0;
	INT nColCnt = pGrid->GetCols(0);
	const INT nMaxRow = pGrid->GetRows();
	for(INT nRow = 1; nRow < nMaxRow; nRow++)
	{
		if(_T("V") == pGrid->GetTextMatrix(nRow, 1))
		{
			nCheckCnt++;
		}
	}

	for(nRow = 0; nRow < nMaxRow; nRow++)
	{
		for(INT nCol = 0; nCol < nPrintColNoCnt; nCol++)
		{
			if(0 == nRow)
			{
				astrHeader[nCol] = pGrid->GetTextMatrix(nRow, aPrintColNoList[nCol]);
			}
			else
			{
				if(0 < nCheckCnt)
				{
					if(pGrid->GetTextMatrix(nRow, 1).IsEmpty())
					{
						break;
					}
				}

				if(0 == nCol)
				{
					strData.Format(
						_T("<!FC>------------------------------------------<!LC><!FP>")
						_T("<!FN>%s. %s : %s<!LN><!FP>"),
						pGrid->GetTextMatrix(nRow, 0),
						astrHeader[nCol],
						getLimitWord(pGrid->GetTextMatrix(nRow, aPrintColNoList[nCol]).GetBuffer(0), 24, TRUE));
				}
				else
				{
					strData.Format(
						_T("<!FN>   %s : %s<!LN><!FP>"),
						astrHeader[nCol],
						pGrid->GetTextMatrix(nRow, aPrintColNoList[nCol]));

				}
				strReceiptString += strData;
			}
		}
		if(0 == nRow)
		{
			continue;
		}
	}
	delete []astrHeader;

	printReceipt(strReceiptString);

	return 0;

}