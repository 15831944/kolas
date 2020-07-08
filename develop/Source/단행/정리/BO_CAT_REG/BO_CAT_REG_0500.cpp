// BO_CAT_REG_0500.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "..\..\..\����\����\BO_CAT_REG\BO_CAT_REG_0500.h"
#include "..\..\..\����\����\BO_CAT_REG\BO_CAT_REG_0510.h"
#include "..\..\..\����\����\BO_CAT_REG\BO_CAT_REG_0520.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

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
	m_pDM = NULL;
}

CBO_CAT_REG_0500::~CBO_CAT_REG_0500()
{
	
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
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0500 message handlers

BOOL CBO_CAT_REG_0500::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
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
	
	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//��ȣ����|E|1120|||||||
	//��ȣ����|E|1121|||||||
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_CAT_REG_0500"), _T("��ȣ����"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_CAT_REG_0500"), _T("��ȣ����"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  

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

	CBO_CAT_REG_0510 dlg(this);
	dlg.DoModal();	

EFS_END
}

VOID CBO_CAT_REG_0500::OnbSEARCH() 
{
EFS_BEGIN

	//CESL_DataMgr *m_pDM;
	m_pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	if( m_pDM == NULL ) return;

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

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	// ��Ϲ�ȣ
	if( pRegNo->GetCheck()==1 )
	{
		CString sRight;
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

		if( sAccessionNumStart.IsEmpty() == TRUE && sAccessionNumEnd.IsEmpty() == FALSE )
		{
			AfxMessageBox ( _T("���� ��Ϲ�ȣ�� �Է��ϼ���.") );
			return;
		}
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		if( sAccessionNumStart.IsEmpty() == FALSE && sAccessionNumStart.GetLength() < 12 )
// 		{
// 			if( sAccessionNumStart.GetLength() > 2 )
// 			{				
// 				sRight.Format( _T("%010s") , sAccessionNumStart.Mid(2) );
// 				sAccessionNumStart = sAccessionNumStart.Left(2) + sRight;
// 			}
// 		}
// 		if( sAccessionNumEnd.IsEmpty() == FALSE && sAccessionNumEnd.GetLength() < 12 )
// 		{
// 			if( sAccessionNumEnd.GetLength() > 2 )
// 			{
// 				sRight.Format( _T("%010s") , sAccessionNumEnd.Mid(2) );
// 				sAccessionNumEnd = sAccessionNumEnd.Left(2) + sRight;
// 			}
// 		}

		if( sAccessionNumStart.IsEmpty() == FALSE && sAccessionNumStart.GetLength() < 12 )
		{
			if( sAccessionNumStart.GetLength() > nRegCodeLength )
			{				
				sAccessionNumStart = m_pInfo->MakeRegNo(sAccessionNumStart.Left(nRegCodeLength),  sAccessionNumStart.Mid(nRegCodeLength) );
			}
		}
		if( sAccessionNumEnd.IsEmpty() == FALSE && sAccessionNumEnd.GetLength() < 12 )
		{
			if( sAccessionNumEnd.GetLength() > nRegCodeLength )
			{
				sAccessionNumEnd = m_pInfo->MakeRegNo(sAccessionNumEnd.Left(nRegCodeLength), sAccessionNumEnd.Mid(nRegCodeLength) );
			}
		}
		//=====================================================
		
		

		if( sAccessionNumStart.IsEmpty() == FALSE && sAccessionNumEnd.IsEmpty() == TRUE )
		{
			//sQuery = _T("SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO = '") + sAccessionNumStart + _T("'");
			strWhere = _T("REC_KEY IN (")
					_T("SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO = '") + sAccessionNumStart + _T("') ")
					_T("OR REC_KEY IN (")
					_T("SELECT DISTINCT ACCESSION_REC_KEY FROM SE_BOOK_TBL WHERE REG_NO = '") + sAccessionNumStart + _T("')");
		}
		if( sAccessionNumStart.IsEmpty() == FALSE && sAccessionNumEnd.IsEmpty() == FALSE )
		{
			//sQuery = _T("SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '") + sAccessionNumStart + _T("' AND REG_NO <= '") + sAccessionNumEnd + _T("'");
			strWhere = _T("REC_KEY IN (")
				_T("SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '") + sAccessionNumStart + _T("' AND REG_NO <= '") + sAccessionNumEnd + _T("') ")
				_T("OR REC_KEY IN (")
				_T("SELECT DISTINCT ACCESSION_REC_KEY FROM SE_BOOK_TBL WHERE REG_NO >= '") + sAccessionNumStart + _T("' AND REG_NO <= '") + sAccessionNumEnd + _T("')");
		}
		}
	//���ι�ȣ
	else if(pAccNo->GetCheck()==1)
	{		
		
		if ( sAccessionNumStart != _T("") || sAccessionNumEnd !=_T("") ) {
			if ( (sAccessionNumStart >= _T("0") && sAccessionNumStart <= _T("99999")) 
				|| (sAccessionNumEnd >= _T("0") && sAccessionNumEnd <= _T("99999")) ) {
			}
			else {
				AfxMessageBox (_T("���ι�ȣ�� ���ڸ� �Է��ϼ���"));
				return;
			}
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
	// ������� �˻�
	else if(pRegDay->GetCheck()==1)
	{		
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
	// �Ϸ�����
	else if(pComDay->GetCheck()==1)
	{
		if ( sDayStart == _T("") && sDayEnd != _T("") ) {
			AfxMessageBox( _T("���� �Ϸ����ڸ� �Է��ϼ���.") );
			return;
		}
		else if ( sDayStart != _T("") && sDayEnd == _T("") )
			strWhere = _T("PROCESSING_COMPLETE_DATE = '") + sDayStart + _T("'");

		else if ( sDayStart !=_T("") && sDayEnd != _T("") )
			strWhere = _T("PROCESSING_COMPLETE_DATE >= '") + sDayStart + _T("' AND PROCESSING_COMPLETE_DATE <= '") + sDayEnd + _T("'");			
	}
	
	if( sACQUISIT_YEAR.IsEmpty() == FALSE ) 
	{
		if( strWhere.IsEmpty() == FALSE )	strWhere = _T("ACCESSION_REC_MAKE_YEAR = ") + sACQUISIT_YEAR + _T(" AND ") + strWhere;		
		else								strWhere = _T("ACCESSION_REC_MAKE_YEAR = ") + sACQUISIT_YEAR;
	}
	if( sACQUISIT_CODE.IsEmpty() == FALSE ) 
	{
		if ( strWhere.IsEmpty() == FALSE )	strWhere = _T("ACQUISIT_CODE = '") + sACQUISIT_CODE + _T("' AND ") + strWhere;	
		else strWhere = _T("ACQUISIT_CODE = '") + sACQUISIT_CODE + _T("' ");
	}
	if ( strWhere.IsEmpty() == FALSE )	
	{
		strWhere = _T("MANAGE_CODE = UDF_MANAGE_CODE AND ") + strWhere;	
	}
	else
	{
		strWhere = _T("MANAGE_CODE = UDF_MANAGE_CODE ");
	}

	if( m_pDM->RefreshDataManager(strWhere) < 0 )	return;
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM, _T("�Լ�����"), _T("���Ա���"));	

	INT iRowCount = m_pDM->GetRowCount();
	if( iRowCount < 1 ) 
	{
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
	}
	
	for ( INT i = 0 ; i < iRowCount ; i++ ) 
	{
		CString sFirstRegNo = m_pDM->GetCellData( _T("��Ϲ�ȣ"), i );
		CString sLastRegNo = m_pDM->GetCellData( _T("��������Ϲ�ȣ"), i );
		if ( sFirstRegNo != _T("") ) {
			CString sRegNo = sFirstRegNo + _T(" ~ ") + sLastRegNo;
			m_pDM->SetCellData( _T("��Ϲ�ȣ"), sRegNo , i );
		}
	}

	// 040310_HSS  ������ ���氪 �־��ֱ� (��Ϲ�ȣ, ����, å��) +++++++++++++++++++++++
	CESL_DataMgr DM_BOOK;
	CString sAccRecKey;
	CString sQuery, sRegnoRange;
	INT nCnt1, nCnt2;
	CString sData1, sData2, sPutData;

	for ( i = 0 ; i < m_pDM->GetRowCount() ; i++ )
	{
		// ���� - ����� ���� �����ֱ�
		sData1 = m_pDM->GetCellData( _T("����"), i );
		sData2 = m_pDM->GetCellData( _T("����_����"), i );

		sPutData.Format(_T("%d"), _ttoi(sData1)+_ttoi(sData2));
		m_pDM->SetCellData( _T("����"), sPutData , i );

		// å�� - ����� ���� �����ֱ�
		sData1 = m_pDM->GetCellData( _T("å��"), i );
		sData2 = m_pDM->GetCellData( _T("å��_����"), i );

		sPutData.Format(_T("%d"), _ttoi(sData1)+_ttoi(sData2));
		m_pDM->SetCellData( _T("å��"), sPutData , i );

		// DB���� Select�Ͽ� ��Ϲ�ȣ ���� �־��ֱ� 
		sAccRecKey = m_pDM->GetCellData( _T("REC_KEY"), i );
		DM_BOOK.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		sRegnoRange = _T("");
		
		// ���� Select (���� å���� 1 �̻��� ���)
		if (_ttoi(sData1) > 0)
		{
			sQuery.Format( _T("SELECT MIN (reg_no) || ' ~ ' || MAX (reg_no) ")
				_T("FROM bo_book_tbl b, co_accession_rec_tbl a ")
				_T("WHERE a.rec_key = b.accession_rec_key AND a.rec_key = %s ")
				_T("GROUP BY b.reg_code"), sAccRecKey );
			DM_BOOK.RestructDataManager( sQuery );
			
			nCnt1 = DM_BOOK.GetRowCount();		
			if ( nCnt1 > 0 ) {	
				for ( INT j = 0 ; j < nCnt1 ; j++ )
				{
					if (sRegnoRange.GetLength() > 0) sRegnoRange += _T("\n");
					sRegnoRange +=  DM_BOOK.GetCellData( j, 0 );
				}
			}
		}

		// ���� Select (���� å���� 1 �̻��� ���)
		if (_ttoi(sData2) > 0)
		{
			sQuery.Format( _T("SELECT MIN (reg_no) || ' ~ ' || MAX (reg_no) ")
				_T("FROM se_book_tbl b, co_accession_rec_tbl a ")
				_T("WHERE a.rec_key = b.accession_rec_key AND a.rec_key = %s ")
				_T("GROUP BY b.reg_code"), sAccRecKey );
			DM_BOOK.RestructDataManager( sQuery );
			
			nCnt2 = DM_BOOK.GetRowCount();
			if ( nCnt2 > 0 ) {	
				for ( INT j = 0 ; j < nCnt2 ; j++ )
				{
					if (sRegnoRange.GetLength() > 0) sRegnoRange += _T("\n");
					sRegnoRange +=  DM_BOOK.GetCellData( j, 0 );
				}
			}
		}

		m_pDM->SetCellData( _T("��Ϲ�ȣ"), sRegnoRange , i );
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_REG_0500"));
	pCM->AllControlDisplay();	
	SetGridHeight();
	// 040310_HSS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

EFS_END
}

INT CBO_CAT_REG_0500::SetGridHeight() 
{
	INT nHeight, ids;
	CString strTmp;

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_REG_0500"));
	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM->FindControl(_T("GRID"));

	for ( INT i = 0 ; i < m_pDM->GetRowCount() ; i++ ) 
	{
		strTmp = m_pDM->GetCellData( _T("��Ϲ�ȣ"), i );
		nHeight = 1;
		while ( strTmp.Find(_T("\n"),0) != -1 )
		{
			nHeight++;
			ids = strTmp.Find(_T("\n"),0);
			strTmp = strTmp.Mid(ids+1);
		}
		pGrid_sub->SetRowHeight( i+1, 200 * nHeight );
	}

	return 0;
}

VOID CBO_CAT_REG_0500::OnbAccessionUpdate() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_REG_0520 dlg(this);
	//dlg.DoModal();
	if ( dlg.DoModal() == IDOK ) {
		OnbSEARCH();
	}

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
		m_pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
		INT ids = m_pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
			return;
		}

		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			m_pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		//����Ű ����

			m_pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_REG_0500"), _T("GRID"));			
		}
		while (idx > 0);

		sAccRecKey = sAccRecKeyTmp.Mid(1);

		// 040225_HSS_�߰� : ���� ���� �ѹ��� �ϱ�	++++++++++++++++++++++++++++++
		CString sQuery;
		sQuery.Format( _T("DELETE FROM CO_ACCESSION_REC_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = m_pDM->AddFrame(sQuery);
		if ( 0 > iCh ) return;
		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		ids = m_pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
			return;
		}
		ids = m_pDM->EndFrame();
		if (ids < 0) {
			AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return;
		}
	}
	
	AllControlDisplay(_T("CM_BO_CAT_REG_0500"));
	SetGridHeight();
	
EFS_END
}

VOID CBO_CAT_REG_0500::OnbCat0500PRINT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	m_pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	if ( m_pDM == NULL ) return;
	if ( m_pDM->GetRowCount() < 1 ) {
		return;
	}
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����_��Ͽ��ΰ�������"));		
	
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}
	
	//����, å�� ���ϱ�
	INT iSpeciesCount = 0;
	INT iBookCount = 0;
	CString sSpeciesCount;
	CString sBookCount;
	for ( INT i = 0 ; i < m_pDM->GetRowCount() ; i++ ) {
		sSpeciesCount = m_pDM->GetCellData( _T("����"), i );
		iSpeciesCount = iSpeciesCount + _ttoi(sSpeciesCount);
		sBookCount = m_pDM->GetCellData( _T("å��"), i );
		iBookCount = iBookCount + _ttoi(sBookCount);
	}
	sSpeciesCount.Format( _T("%d"), iSpeciesCount );
	sBookCount.Format( _T("%d"), iBookCount );
	CString sTotal;
	sTotal = _T("   �� �� :   ���� ") + sSpeciesCount + _T("   å�� ") + sBookCount;
	pSIReportManager->SetTempVariable(_T("�հ�"), sTotal);

	//CString sLibraryName;
	//GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("��������"), sLibraryName );
	//pSIReportManager->SetTempVariable(_T("%BARCODE���ڵ�����%"), sLibraryName);

	pSIReportManager->SetDataMgr(0, m_pDM);
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

EFS_END
}


INT CBO_CAT_REG_0500::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{

EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

EFS_END
return -1;

}

HBRUSH CBO_CAT_REG_0500::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

