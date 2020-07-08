// BO_CAT_REG_0500.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_REG_0500.h"
#include "BO_CAT_REG_0510.h"
#include "BO_CAT_REG_0520.h"
//#include _T("..\..\..\ESL\SIReportMgr\SIReportManager.h")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0500 dialog


CBO_CAT_REG_0500::CBO_CAT_REG_0500(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0500)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_CAT_REG_0500::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0500)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0500, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0500)
	ON_BN_CLICKED(IDC_bAccessionInput, OnbAccessionInput)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bAccessionUpdate, OnbAccessionUpdate)
	ON_BN_CLICKED(IDC_rACCESSION_NO, OnrACCESSIONNO)
	ON_BN_CLICKED(IDC_rACCESSION_REC_NO, OnrACCESSIONRECNO)
	ON_BN_CLICKED(IDC_rACCESSION_DATE, OnrACCESSIONDATE)
	ON_BN_CLICKED(IDC_rACCESSION_DATE2, OnrACCESSIONDATE2)
	ON_BN_CLICKED(IDC_bAccessionDelete, OnbAccessionDelete)
	ON_BN_CLICKED(IDC_bCat0500PRINT, OnbCat0500PRINT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0500 message handlers

BOOL CBO_CAT_REG_0500::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_0500")) < 0)
	//if (InitESL_Mgr(_T("BO_CAT_REG")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CButton * pRadio1;
	pRadio1=(CButton *)GetDlgItem(IDC_rACCESSION_REC_NO);
	pRadio1->SetCheck(1);	

	CEdit * pEdit3;
	pEdit3=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_START);
	pEdit3->EnableWindow(FALSE);
	//pEdit3->SetFocus();

	CEdit * pEdit4;
	pEdit4=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_END);
	pEdit4->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

BOOL CBO_CAT_REG_0500::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_0500::OnbAccessionInput() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_REG_0510 dlg(this);
	dlg.DoModal();	

EFS_END
}

VOID CBO_CAT_REG_0500::OnbSEARCH() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	//CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	if ( pDM == NULL ) return;

	CButton *pRegNo, *pAccNo, *pRegDay, *pComDay;
	pRegNo=(CButton *)GetDlgItem(IDC_rACCESSION_NO);
	pAccNo=(CButton *)GetDlgItem(IDC_rACCESSION_REC_NO);
	pRegDay=(CButton *)GetDlgItem(IDC_rACCESSION_DATE);
	pComDay=(CButton *)GetDlgItem(IDC_rACCESSION_DATE2);
	
	CString strWhere;	
	CString sAccessionNumStart, sAccessionNumEnd;
	CString sDayStart, sDayEnd;
	GetControlData(_T("CM_BO_CAT_REG_0500"), _T("��ȣ����"), sAccessionNumStart);
	GetControlData(_T("CM_BO_CAT_REG_0500"), _T("��ȣ����"), sAccessionNumEnd);
	GetControlData(_T("CM_BO_CAT_REG_0500"), _T("��¥����"), sDayStart);
	GetControlData(_T("CM_BO_CAT_REG_0500"), _T("��¥����"), sDayEnd);
	//GetControlData(_T("CM_MAIN_SAMPLE_SEARCH"), _T("����"), strLevel, -1, 0);

	//���Ա���, �ۼ��⵵
	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	GetControlData(_T("CM_BO_CAT_REG_0500"), _T("���Ա���"), sACQUISIT_CODE);
	GetControlData(_T("CM_BO_CAT_REG_0500"), _T("�ۼ��⵵"), sACQUISIT_YEAR);

	sACQUISIT_CODE.Replace( _T(" ") , NULL );
	sACQUISIT_YEAR.Replace( _T(" ") , NULL );

	CString sWhere;
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	if(pRegNo->GetCheck()==1){
		CString sQuery;
		if ( sAccessionNumStart == _T("") && sAccessionNumEnd != _T("") ) {
			AfxMessageBox ( _T("���� ��Ϲ�ȣ�� �Է��ϼ���.") );
			return;
		}
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		if ( sAccessionNumStart != _T("") && sAccessionNumStart.GetLength() < 12 ) {
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			if ( sAccessionNumStart.GetLength() > 2 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , sAccessionNumStart.Mid(2) );
// 				sAccessionNumStart = sAccessionNumStart.Left(2) + sRight;
// 			}
			//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
			if ( sAccessionNumStart.GetLength() > nRegCodeLength ) {
				sAccessionNumStart = m_pParent->m_pInfo->MakeRegNo(sAccessionNumStart.Left(nRegCodeLength), sAccessionNumStart.Mid(nRegCodeLength) );
			}
			//=====================================================
			
		}
		if ( sAccessionNumEnd != _T("") && sAccessionNumEnd.GetLength() < 12 ) {
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			if ( sAccessionNumEnd.GetLength() > 2 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , sAccessionNumEnd.Mid(2) );
// 				sAccessionNumEnd = sAccessionNumEnd.Left(2) + sRight;
// 			}
			//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
			if ( sAccessionNumEnd.GetLength() > nRegCodeLength ) {
				sAccessionNumEnd = m_pParent->m_pInfo->MakeRegNo(sAccessionNumEnd.Left(nRegCodeLength), sAccessionNumEnd.Mid(nRegCodeLength) );
			}
			//=====================================================
			
		}
		if ( sAccessionNumStart != _T("") && sAccessionNumEnd == _T("") ) {
			sQuery = _T("SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_NO = '") + sAccessionNumStart + _T("'");
			sWhere = _T("REC_KEY IN (SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO = '") + sAccessionNumStart + _T("')");
		}
		if ( sAccessionNumStart != _T("") && sAccessionNumEnd != _T("") ) {
			sQuery = _T("SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_NO >= '") + sAccessionNumStart + _T("' AND REG_NO <= '") + sAccessionNumEnd + _T("'");
			sWhere = _T("REC_KEY IN (SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '") + sAccessionNumStart + _T("' AND REG_NO <= '") + sAccessionNumEnd + _T("')");
		}
		/*
		dm.RestructDataManager(sQuery);
		//sCount.Format( _T("%d"), dm.GetRowCount());
		//pDM->SetCellData( i, 16, sCount);
		//sWhere = _T("AND (");
		for ( INT i = 0; i < dm.GetRowCount(); i++) {	
			CString sAccNum;
			dm.GetCellData( 0, 0, sAccNum);

			if ( i==0 ) {
				sWhere = _T("REC_KEY = ") + sAccNum;
			}
			else {
				sWhere = sWhere + _T(" OR REC_KEY = ") + sAccNum;
			}
			/*
			CString sCount;
			CString sQuery;
		
			//å�� ���ϱ�
			sQuery = _T("select COUNT(REC_KEY) from bo_book_tbl where ACCESSION_NO = ") + sAccNum;
			//pDM->GetOneValueQuery( _T("select COUNT(REC_KEY) from bo_book_tbl where ACCESSION_NO = 11"), sCount);
			pDM->GetOneValueQuery( sQuery , sCount);
			pDM->SetCellData( i, 17, sCount);			
			*/
		//}
		/*
		CESL_ControlMgr *pCM;
		pCM = FindCM(_T("CM_BO_CAT_REG_0500"));
		pCM->AllControlDisplay();	
		return;
		*/
	}
	
/*
	CString strOption;
	if (!strName.IsEmpty())
	{
		strOption.Format(_T("NAME like '%%%s%%' "), strName);
		strWhere += strOption;
	}

	strLevel=_T("2");
	if (strLevel != _T("0"))
	{
		if (strWhere.IsEmpty())
			strOption.Format(_T("GRADE = %s"), strLevel);
		else
			strOption.Format(_T("AND GRADE = %s"), strLevel);

		strWhere += strOption;
	}
*/
	//strWhere=_T("REC_KEY=50014143");
	/*
	strWhere = _T("rownum >= 0");
	if (!sAccessionNumStart.IsEmpty()) {
		strWhere=_T("ACCESSION_REC_NO = ") + sAccessionNumStart;
	}	
*/
	if(pRegNo->GetCheck()==1){
		strWhere = sWhere;
		if ( sWhere == _T("") ) {
			//strWhere = _T("rownum < 0 ");
		}
	}
	
	if(pAccNo->GetCheck()==1){		//���ι�ȣ �˻�
		
		if ( sAccessionNumStart != _T("") || sAccessionNumEnd !=_T("") ) {
			if ( sAccessionNumStart >= _T("0") && sAccessionNumStart <= _T("99999")) {
			}
			else if ( sAccessionNumEnd >= _T("0") && sAccessionNumEnd <= _T("99999")) {
			}
			else {
				AfxMessageBox (_T("���ι�ȣ�� ���ڸ� �Է��ϼ���"));
				return;
			}
			/*
			else if ( sAccessionNumEnd >= _T("0") && sAccessionNumEnd <= _T("99999")) {
			}
			else {
				AfxMessageBox (_T("���ι�ȣ�� ���ڸ� �Է��ϼ���"));
				return;
			}
			*/
		}

		if ( sAccessionNumStart == _T("") && sAccessionNumEnd != _T("") ) {
			AfxMessageBox ( _T("���� ���ι�ȣ�� �Է��ϼ���.") );
			return;
		}

		if ( sAccessionNumStart != _T("") && sAccessionNumEnd == _T("") ) {			
			strWhere = _T("ACCESSION_REC_NO = '") + sAccessionNumStart + _T("'");
		}
		if ( sAccessionNumStart != _T("") && sAccessionNumEnd != _T("") ) {
			strWhere = _T("ACCESSION_REC_NO >= '") + sAccessionNumStart + _T("' AND ACCESSION_REC_NO <= '") + sAccessionNumEnd + _T("'");
		}
	}

	if(pRegDay->GetCheck()==1){		
		if ( sDayStart == _T("") && sDayEnd != _T("") ) {
			AfxMessageBox( _T("���� ������ڸ� �Է��ϼ���.") );
			return;
		}
		else if ( sDayStart != _T("") && sDayEnd == _T("") ) {
			strWhere = _T("REG_DATE = '") + sDayStart + _T("'");
		}
		else if ( sDayStart !=_T("") && sDayEnd != _T("") ) {
			strWhere = _T("REG_DATE >= '") + sDayStart + _T("' AND REG_DATE <= '") + sDayEnd + _T("'");
		}
	}
	if(pComDay->GetCheck()==1){
		if ( sDayStart == _T("") && sDayEnd != _T("") ) {
			AfxMessageBox( _T("���� �Ϸ����ڸ� �Է��ϼ���.") );
			return;
		}
		else if ( sDayStart != _T("") && sDayEnd == _T("") ) {
			//strWhere = _T("CATALOG_DATE = '") + sDayStart + _T("'");
			strWhere = _T("PROCESSING_COMPLETE_DATE = '") + sDayStart + _T("'");
		}
		else if ( sDayStart !=_T("") && sDayEnd != _T("") ) {
			//strWhere = _T("CATALOG_DATE >= '") + sDayStart + _T("' AND CATALOG_DATE <= '") + sDayEnd + _T("'");
			strWhere = _T("PROCESSING_COMPLETE_DATE >= '") + sDayStart + _T("' AND PROCESSING_COMPLETE_DATE <= '") + sDayEnd + _T("'");			
		}
		//strWhere = _T("CATALOG_DATE >= '") + sDayStart +_T("'");
	}
	
	if ( sACQUISIT_YEAR != _T("") ) {
		if ( strWhere != _T("") ) {
			strWhere = _T("ACCESSION_REC_MAKE_YEAR = ") + sACQUISIT_YEAR + _T(" AND ") + strWhere;		
		}
		else {
			strWhere = _T("ACCESSION_REC_MAKE_YEAR = ") + sACQUISIT_YEAR + strWhere;
		}
	}
	if ( sACQUISIT_CODE != _T("") ) {
		if ( strWhere != _T("") ) {
			strWhere = _T("ACQUISIT_CODE = '") + sACQUISIT_CODE + _T("' AND ") + strWhere;	
		}
		else {
			strWhere = _T("ACQUISIT_CODE = '") + sACQUISIT_CODE + _T("' ") + strWhere;
		}
	}

	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("�Լ�����"), _T("���Ա���"));	

	INT iRowCount = pDM->GetRowCount();
	if ( iRowCount < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
	}
	
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sFirstRegNo = pDM->GetCellData( _T("��Ϲ�ȣ"), i );
		CString sLastRegNo = pDM->GetCellData( _T("��������Ϲ�ȣ"), i );
		if ( sFirstRegNo != _T("") ) {
			CString sRegNo = sFirstRegNo + _T(" ~ ") + sLastRegNo;
			pDM->SetCellData( _T("��Ϲ�ȣ"), sRegNo , i );
		}
	}

	/*
	for ( INT i = 0; i < iRowCount; i++) {
		CString sAccNum;
		//pDM->GetCellData( i, 6, sAccNum);
		pDM->GetCellData( _T("���ι�ȣ") , i, sAccNum);

		CString sCount;
		CString sQuery;
		
		//sQuery = _T("select COUNT(REC_KEY) from bo_book_tbl where ACCESSION_NO = ") + sAccNum;

		if ( sAccNum == _T("")) {
			CString sErrorMessage;
			sErrorMessage.Format( _T("%d��° �ڷῡ ���ι�ȣ�� �����ϴ�.") , i+1 );
			AfxMessageBox( sErrorMessage );
			//return;
			//sQuery = _T("select COUNT(REC_KEY) from bo_book_tbl where ACCESSION_NO = -1");
			sAccNum = _T("-1");
		}
		//å�� ���ϱ�
		sQuery = _T("select COUNT(REC_KEY) from bo_book_tbl where ACCESSION_NO = ") + sAccNum;
		pDM->GetOneValueQuery( sQuery , sCount);
		pDM->SetCellData( i, 17, sCount);
		
		//���� ���ϱ�
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		sQuery = _T("select distinct species_key from bo_book_tbl where ACCESSION_NO = ") + sAccNum;
		dm.RestructDataManager(sQuery);
		sCount.Format( _T("%d"), dm.GetRowCount());
		pDM->SetCellData( i, 16, sCount);

		
		//��Ϲ�ȣ ���ϱ�
		CString sRegNum;
		CString sRegNum_start;
		CString sRegNum_end;
		sQuery = _T("select min(reg_no) from bo_book_tbl where ACCESSION_NO = ") + sAccNum;
		pDM->GetOneValueQuery( sQuery , sRegNum_start);
		sQuery = _T("select max(reg_no) from bo_book_tbl where ACCESSION_NO = ") + sAccNum;
		pDM->GetOneValueQuery( sQuery , sRegNum_end);

		sRegNum = sRegNum_start + _T(" ~ ") + sRegNum_end;

		pDM->SetCellData( i, 18, sRegNum);
		
	}
	*/

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_REG_0500"));
	pCM->AllControlDisplay();	

	
	

EFS_END
}

VOID CBO_CAT_REG_0500::OnbAccessionUpdate() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_REG_0520 dlg(this);
	dlg.DoModal();	
	

EFS_END
}

VOID CBO_CAT_REG_0500::OnrACCESSIONNO() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CEdit * pEdit1;
	pEdit1=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_START);
	pEdit1->EnableWindow(TRUE);
	pEdit1->SetFocus();

	CEdit * pEdit2;
	pEdit2=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_END);
	pEdit2->EnableWindow(TRUE);

	CEdit * pEdit3;
	pEdit3=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_START);
	pEdit3->EnableWindow(FALSE);

	CEdit * pEdit4;
	pEdit4=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_END);
	pEdit4->EnableWindow(FALSE);
	

EFS_END
}

VOID CBO_CAT_REG_0500::OnrACCESSIONRECNO() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CEdit * pEdit1;
	pEdit1=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_START);
	pEdit1->EnableWindow(TRUE);
	pEdit1->SetFocus();

	CEdit * pEdit2;
	pEdit2=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_END);
	pEdit2->EnableWindow(TRUE);

	CEdit * pEdit3;
	pEdit3=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_START);
	pEdit3->EnableWindow(FALSE);

	CEdit * pEdit4;
	pEdit4=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_END);
	pEdit4->EnableWindow(FALSE);

EFS_END
}

VOID CBO_CAT_REG_0500::OnrACCESSIONDATE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CEdit * pEdit1;
	pEdit1=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_START);
	pEdit1->EnableWindow(FALSE);
	//pEdit1->SetFocus();

	CEdit * pEdit2;
	pEdit2=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_END);
	pEdit2->EnableWindow(FALSE);

	CEdit * pEdit3;
	pEdit3=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_START);
	pEdit3->EnableWindow(TRUE);
	pEdit3->SetFocus();

	CEdit * pEdit4;
	pEdit4=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_END);
	pEdit4->EnableWindow(TRUE);

EFS_END
}

VOID CBO_CAT_REG_0500::OnrACCESSIONDATE2() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CEdit * pEdit1;
	pEdit1=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_START);
	pEdit1->EnableWindow(FALSE);
	

	CEdit * pEdit2;
	pEdit2=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_END);
	pEdit2->EnableWindow(FALSE);

	CEdit * pEdit3;
	pEdit3=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_START);
	pEdit3->EnableWindow(TRUE);
	pEdit3->SetFocus();

	CEdit * pEdit4;
	pEdit4=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_END);
	pEdit4->EnableWindow(TRUE);

EFS_END
}

VOID CBO_CAT_REG_0500::OnbAccessionDelete() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	SelectMakeList(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
	INT idx = SelectGetTailPosition(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
	if (idx < 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
		return;
	}

	if (MessageBox(_T("������ ���θ� �����Ͻðڽ��ϱ�?"), _T("���� Ȯ��"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
		pDM->StartFrame();

		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		//����Ű ����

			pDM->MakeDeleteFrame(_T("CO_ACCESSION_REC_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_REG_0500"), _T("GRID"));			
		}		
		while (idx > 0);

		sAccRecKey = sAccRecKeyTmp.Mid(1);
		
		CString sQuery;
		sQuery.Format( _T("DELETE FROM BO_BOOK_TBL WHERE ACCESSION_REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = pDM->AddFrame(sQuery);
		
		INT ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
			return;
		}
		pDM->EndFrame();
	}
	

	AllControlDisplay(_T("CM_BO_CAT_REG_0500"));
	

EFS_END
}

VOID CBO_CAT_REG_0500::OnCancel() 
{
EFS_BEGIN

	// TODO: Add extra cleanup here
	DestroyWindow();	
	//CDialog::OnCancel();

EFS_END
}

VOID CBO_CAT_REG_0500::OnbCat0500PRINT() 
{
EFS_BEGIN
/*
	// TODO: Add your control notification handler code here
	pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	if ( pDM == NULL ) return;
	if ( pDM->GetRowCount() < 1 ) {
		return;
	}
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����_��Ͽ��ΰ�������"));		
	
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}
	
	//CString sLibraryName;
	//GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("��������"), sLibraryName );
	//pSIReportManager->SetTempVariable(_T("%BARCODE���ڵ�����%"), sLibraryName);

	pSIReportManager->SetDataMgr(0, pDM);
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
*/
EFS_END
}

/*
INT CBO_CAT_REG_0500::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{

EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

EFS_END
return -1;

}
*/