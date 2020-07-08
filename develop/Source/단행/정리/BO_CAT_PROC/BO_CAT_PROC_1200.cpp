// BO_CAT_PROC_1200.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_1200.h"
#include "..\..\..\�������\Light_BO_CAT_CENTERUPLOAD\BO_CAT_CENTERUPLOAD.h"
#include "SPF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_1200 dialog


CBO_CAT_PROC_1200::CBO_CAT_PROC_1200(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_1200)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInitDlg = TRUE;
}

CBO_CAT_PROC_1200::~CBO_CAT_PROC_1200()
{
}

VOID CBO_CAT_PROC_1200::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_1200)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_1200, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_1200)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDD_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_1200 message handlers

BOOL CBO_CAT_PROC_1200::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_1200::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_1200::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_1200")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	if (m_strParentCMAlias.IsEmpty())
		m_strParentCMAlias = _T("CM_BO_CAT_PROC_0000");
	if (m_strParentDMAlias.IsEmpty())
		m_strParentDMAlias = _T("DM_BO_CAT_PROC_0000");
	if (m_strParentGridAlias.IsEmpty())
		m_strParentGridAlias = _T("ProcGrid");
	
	INT idx;
	SelectMakeList(m_strParentCMAlias, m_strParentGridAlias);
	idx = SelectGetHeadPosition(m_strParentCMAlias, m_strParentGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		EndDialog(IDCANCEL);
		m_bInitDlg = FALSE;
		return TRUE;
		
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(m_strParentDMAlias);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}

	CString sMessage;
	iCountConYes = 0;
	iCountConNo = 0;
	sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );
	iCountConYes++;
	CString sIWorkStatus = pDM->GetCellData( _T("IBS_�۾�����INFO"), idx );
	CString sStartRegNo = pDM->GetCellData( _T("���۵�Ϲ�ȣ"), idx );
	if ( sStartRegNo == _T("") ) {

	}
	
	if ( sIWorkStatus >= _T("BOL112N") ) {

		sComRecKey = sRecKey;
		sRecKey = _T("");
		iCountConYes = 1;
		iCountConNo++;
	}
	
	idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);
	
	while ( idx != -1 ) {
		sStartRegNo = pDM->GetCellData( _T("���۵�Ϲ�ȣ"), idx );
		if ( sStartRegNo == _T("") ) {

		}
		sIWorkStatus = pDM->GetCellData( _T("BS_�ڷ����_�ڵ�"), idx );
		if ( sIWorkStatus < _T("BOC1140") ) {

			CString sTitle = pDM->GetCellData( _T("IBS_��ǥ��"), idx );
			sMessage.Format( _T("%d�� ��ũ�� '%s' �ڷḦ �谡�Ϸ��� �� �����ϴ�.\n�ʼ�Tag�� Ȯ���ϼ���.\n\n �� �ڷḦ �����ϰ� �谡�ϷḦ �����ϰڽ��ϱ�?"), idx+1, sTitle );
			INT ids;
			ids = ::MessageBox( GetSafeHwnd() ,
								sMessage,
								_T("�谡�Ϸ� Ȯ��") ,
								MB_YESNO | MB_ICONQUESTION 
							  );
			if ( ids == IDYES ) {
				idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);
				continue;
			}
			else {
				m_bInitDlg = FALSE;
				EndDialog(IDCANCEL);
				return FALSE;
			}
		}
		CString sRecKeyTmp = pDM->GetCellData( _T("BS_��KEY") , idx );

		if ( sIWorkStatus == _T("BOL112N") ) {
			sComRecKey = sComRecKey + sRecKeyTmp;
			iCountConNo++;
		}		
		if ( sRecKey != _T("") ) {
			sRecKey = sRecKey + _T(", ") + sRecKeyTmp;
			iCountConYes++;
		}
		else {
			sRecKey = sRecKeyTmp;
		}
		idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);
	}

	if ( sRecKey == _T("") && sComRecKey == _T("") ) {
		AfxMessageBox( _T("�谡�Ϸ��� �ڷᰡ �����ϴ�.") );
		m_bInitDlg = FALSE;
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if ( sRecKey != _T("") ) {
		CESL_DataMgr * pDM_1200;
		pDM_1200 = FindDM( _T("DM_BO_CAT_PROC_1200") );
		if ( NULL == pDM_1200 ) {
			AfxMessageBox( _T("DM_BO_CAT_PROC_1200 error") );
			return FALSE;
		}

		CString strWhere = _T("REC_KEY IN(") + sRecKey + _T(") GROUP BY CONTROL_MAT_CODE");
		if (pDM_1200->RefreshDataManager(strWhere) < 0)	return false;
		INT iRowCount = pDM_1200->GetRowCount();
		if ( pDM_1200->GetRowCount() < 1 ) {
			AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
		}

		for ( INT i = 1 ; i <= iRowCount ; i ++ ) {
			CString sDivisionValue;
			pDM_1200->GetCellData( i-1 , 0 , sDivisionValue );
			CString sQueryCount = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE DIVISION_VALUE = '") + sDivisionValue + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE");
			CString sCount;
			pDM->GetOneValueQuery( sQueryCount , sCount );
			INT iCount;
			iCount = _ttoi(sCount.GetBuffer(0));		
			sCount.Format ( _T("%d") , iCount+1 );
			CString sControlNo;
			
			CString sYear;
			GetControlData( _T("CM_BO_CAT_PROC_1200"), _T("����⵵"), sYear );
			sControlNo.Format( _T("%s%s%05d") , sDivisionValue, sYear , iCount+1 );

			CESL_ControlMgr *pCM_SUB;
			pCM_SUB = FindCM(_T("CM_BO_CAT_PROC_1200"));				
			CESL_Grid * pGrid_sub = NULL;
			pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("GRD_�����Ϸ�"));
			pGrid_sub->SetSort( FALSE );
			pGrid_sub->SetRows(iRowCount+1);			
			pGrid_sub->SetTextMatrix( i , 1, sDivisionValue);

			CString sSpeciesCountTmp;
			pDM_1200->GetCellData( _T("����") , i-1 , sSpeciesCountTmp );
			pGrid_sub->SetTextMatrix( i , 2, sSpeciesCountTmp);

			CString sLastControlNo;
			INT iSpeciesCountTmp = _ttoi( sSpeciesCountTmp.GetBuffer(0) );
			
			sLastControlNo.Format( _T("%s%s%05d") , sDivisionValue ,sYear , iCount + iSpeciesCountTmp );
			pGrid_sub->SetTextMatrix( i , 3, sControlNo + _T(" ~ ") + sLastControlNo);		

			pDM_1200->SetCellData( _T("BS_�����ȣ") , sCount , i-1 );
			pDM_1200->GetCellData( _T("BS_�����ȣ") , i-1 , sControlNo );
		}

		CESL_DataMgr * pDM_1200_CONTROL;
		pDM_1200_CONTROL = FindDM( _T("DM_BO_CAT_PROC_1200_SPECIES") );
		if ( NULL == pDM_1200_CONTROL ) {
			AfxMessageBox( _T("DM_BO_CAT_PROC_1200_CONTROL error") );
			return FALSE;
		}

		strWhere = _T("REC_KEY IN(") + sRecKey + _T(") ORDER BY CONTROL_MAT_CODE");
		if (pDM_1200_CONTROL->RefreshDataManager(strWhere) < 0)	return false;
		INT iRowControlCount = pDM_1200_CONTROL->GetRowCount();
		if ( pDM_1200_CONTROL->GetRowCount() < 1 ) {
			AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
		}
		iSpeciesCount = iRowControlCount;
		
		for ( INT j = 1 ; j <= iRowControlCount ; j ++ ) {
			CString sStreamMarc = pDM_1200_CONTROL->GetCellData( _T("BS_MARC"), j-1 );
			CString sControlCodeSpecies;
			pDM_1200_CONTROL->GetCellData( _T("BS_�����ڷᱸ��") , j-1 , sControlCodeSpecies );
			if ( sControlCodeSpecies == _T("") ) {
				AfxMessageBox( _T("�����ڷᱸ���� ���� �ڷᰡ �ֽ��ϴ�.") );
				EndDialog(IDCANCEL);
				m_bInitDlg = FALSE;
				return FALSE;
			}
			for ( INT k = 1 ; k <= iRowCount ; k ++ ) {
				CString sControlCode;
				pDM_1200->GetCellData( _T("BS_�����ڷᱸ��") , k-1 , sControlCode );
				if ( sControlCode == sControlCodeSpecies ) {
					CString sControlNo;
					pDM_1200->GetCellData( _T("BS_�����ȣ") , k-1 , sControlNo );
					pDM_1200_CONTROL->SetCellData( _T("��ȣ") , sControlNo , j-1 );
					INT iControlNo = _ttoi ( sControlNo.GetBuffer(0) );				
					sControlNo.Format( _T("%d") , iControlNo+1 );
					pDM_1200->SetCellData( _T("BS_�����ȣ") , sControlNo , k-1 );
				}
			}
		}
	}	
	
	CESL_DataMgr *pDm1200Book;
	pDm1200Book = FindDM( _T("DM_BO_CAT_PROC_1200_BOOK") );
	if ( pDm1200Book == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_1200_BOOK error") );
		return false;
	}

	if ( sComRecKey != _T("") ) {
		if ( sRecKey != _T("") ) {
			sRecKey = sRecKey + _T(", ") + sComRecKey;
		}
		else {
			sRecKey = sComRecKey;
			iCountConYes = 0;
		}
	}
	CString sWhereBook = _T("SPECIES_KEY IN (") + sRecKey + _T(") AND SHELF_LOC_CODE IS NOT NULL AND REG_NO IS NOT NULL AND WORKING_STATUS = 'BOL111O' GROUP BY SHELF_LOC_CODE");
	m_sWhereBook = _T("SPECIES_KEY IN (") + sRecKey + _T(") AND SHELF_LOC_CODE IS NOT NULL AND REG_NO IS NOT NULL");
	if (pDm1200Book->RefreshDataManager(sWhereBook) < 0)	return false;
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDm1200Book, _T("�ڷ�Ǳ���"), _T("�谡��"));	
	INT iRowBookCount = pDm1200Book->GetRowCount();

	CESL_ControlMgr *pCM_SUB;
	pCM_SUB = FindCM(_T("CM_BO_CAT_PROC_1200"));
	CESL_Grid * pGridBook = NULL;
	pGridBook = (CESL_Grid*)pCM_SUB->FindControl(_T("GRD_�谡��"));
	pGridBook->SetRows(iRowBookCount+1);
	if ( iRowBookCount == 0 ) {
		AfxMessageBox( _T("�ڷ���� �����ϼ���.") );
		m_bInitDlg = FALSE;
		EndDialog(IDCANCEL);
		return FALSE;
	}
	for ( INT i = 0 ; i < iRowBookCount ; i ++ ) {
		CString sShelfCode = pDm1200Book->GetCellData( _T("�谡��"), i );
		if ( sShelfCode == _T("�������") ) {
			AfxMessageBox( _T("�ڷ�� ������ �ȵ� �ڷᰡ �ֽ��ϴ�.") );
			m_bInitDlg = FALSE;
			EndDialog(IDCANCEL);
			return FALSE;
			break;			
		}
		CString sBookCount = pDm1200Book->GetCellData( _T("å��"), i );
		pGridBook->SetTextMatrix( i+1 , 1, sShelfCode);
		pGridBook->SetTextMatrix( i+1 , 2, sBookCount);
	}


	CString sCountConYes, sCountConNo;
	sCountConYes.Format( _T("%d"), iCountConYes );
	sCountConNo.Format( _T("%d"), iCountConNo );
	SetControlData( _T("CM_BO_CAT_PROC_1200"), _T("�����ȣ�ο�������"), sCountConNo );
	SetControlData( _T("CM_BO_CAT_PROC_1200"), _T("�����ȣ�̺ο�������"), sCountConYes );

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_1200::OnOK() 
{
EFS_BEGIN

	CSPF mSpfTest(this);
	mSpfTest.InitTempData();
	mSpfTest.SetTempData(_T("DMAlias"), m_strParentDMAlias.GetBuffer(0));
	if (mSpfTest.SPFExecutePath(_T("�����Ϸ�")) < 0)
		EndDialog(IDCANCEL);
	else
		EndDialog(IDOK);
	
EFS_END
}

VOID CBO_CAT_PROC_1200::OnCancel() 
{
EFS_BEGIN

	EndDialog(IDCANCEL);
	

EFS_END
}

HBRUSH CBO_CAT_PROC_1200::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
