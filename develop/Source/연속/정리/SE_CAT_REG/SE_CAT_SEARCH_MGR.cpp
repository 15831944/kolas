// SE_CAT_SEARCH_MGR1.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_SEARCH_MGR.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_SEARCH_MGR dialog
#include "..\..\..\����\K2UP_Common\MakeSearchData.h"

CSE_CAT_SEARCH_MGR::CSE_CAT_SEARCH_MGR(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_SEARCH_MGR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CAT_SEARCH_MGR::~CSE_CAT_SEARCH_MGR()
{
}
VOID CSE_CAT_SEARCH_MGR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_SEARCH_MGR)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_SEARCH_MGR, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_SEARCH_MGR)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bDetailNo, OnbDetailNo)
	ON_BN_CLICKED(IDC_bREG0100CLEAR, OnbREG0100CLEAR)
	ON_CBN_SELCHANGE(IDC_BIND_TYPE, OnSelchangeBindType)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_SEARCH_MGR message handlers

BOOL CSE_CAT_SEARCH_MGR::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_���_�˻�_�Ŵ���")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}


	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_���_����"));

	CString m_Year;
	// 040304_HSS ���� LocalTime���� Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	SetControlData(_T("CM_����_���_�˻�"), _T("BS_���Գ⵵"), m_Year);
	SetControlData(_T("CM_����_���_�˻�"), _T("���γ⵵"), m_Year);


	//���� ���� 
	OnbDetailNo(); 

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CSE_CAT_SEARCH_MGR::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}


VOID CSE_CAT_SEARCH_MGR::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	CString strQry,strWorkNo1,strWorkNo2,strpreGroup1,strpreGroup2,strYear;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_���_����"));

	// ���� ��ȣ �� ������ ��ȣ�� ���ο� ���� �ش�.
	GetControlData(_T("CM_����_���_�˻�"), _T("������ȣ����"), strpreGroup1);
	GetControlData(_T("CM_����_���_�˻�"), _T("������ȣ��"), strpreGroup2);		 

	GetControlData(_T("CM_����_���_�˻�"), _T("BS_���Գ⵵"), strYear);		 

	if( (strWorkNo1 != _T(""))&&(strWorkNo2 != _T("")) )
	{
		strWorkNo1 = strWorkNo1 + _T(" - ") +strWorkNo2;
	}
	if( (strpreGroup1 != _T(""))&&(strpreGroup2 != _T("")) )
	{
		strpreGroup1 = strpreGroup1 + _T(" - ") +strpreGroup2;
	}

	SetControlData(_T("CM_����_����_���_����"), _T("���ι�ȣ"), strpreGroup1);
	SetControlData(_T("CM_����_����_���_����"), _T("���γ⵵"), strYear);


	strQry = MakeSelectQry();
	if( strQry == _T("")) return;

	pDM->RefreshDataManager(strQry);

	CESL_ControlMgr *pCM;
	CESL_Grid *pGrid_MainGrid_All;

	pCM = FindCM(_T("CM_����_����_���_����"));
	pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	//�׸��� �ݹ� �Լ� ���� 

	// 041006_HSS �˻���� ���� �޼��� �߰�
	INT nCount = pDM->GetRowCount();
	if (nCount == 0)
	{
		AfxMessageBox(_T("�˻������ �����ϴ�."));
		pCM->AllControlDisplay();
		return;
	}

	pCM->AllControlDisplay();

	if( nCount > 0 )
		ShowWindow(SW_HIDE);

}

INT CSE_CAT_SEARCH_MGR::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_�ڷ����_�ڵ�")) {
            if ( str == _T("SER111N")) {
				str = _T("����ΰ��ڷ�");
			}
			else if ( str == _T("SER112N") ) {
				str = _T("����ڷ�");
			}
			else if ( str == _T("SER113O") ) {
				str = _T("�������ڷ�");
			}

			CESL_DataMgr *pDM = FindDM(_T("CM_����_����_���_����"));
			if (pDM)
			{
				pDM->SetCellData(_T("BB_�ڷ����_����"), str, nRow);
			}
        }
    }
	return 0;


EFS_END
return -1;

}

CString CSE_CAT_SEARCH_MGR::MakeSelectQry()
{

	CString strQry; 
	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	CString sAccessionNoStart, sAccessionNoEnd,sAccessionNoYear;
	CString sRegNoStart, sRegNoEnd;
	CString sTransferDataStart, sTransferDataEnd;
	CString SEQ_NO1,SEQ_NO2,preGroup1,preGroup2;
	INT sAccessionFlag=0;

	//�ڷ����
	CString sWorkStatus;
	GetControlData( _T("CM_����_���_�˻�"), _T("�ڷ����"), sWorkStatus );
	
	if ( sWorkStatus == _T("��ü") ) {
		strQry = strQry + _T("  B.WORKING_STATUS >= 'SER111N' AND B.WORKING_STATUS <= 'SER112N'");
	}
	else if ( sWorkStatus == _T("����ΰ��ڷ�") ) {
		strQry = strQry + _T(" B.WORKING_STATUS = 'SER111N'");
	}
	else if ( sWorkStatus == _T("����ڷ�") ) {
			strQry = strQry + _T(" B.WORKING_STATUS = 'SER112N'");
	}

	//�������� 
	CString sBindStatus;
	GetControlData( _T("CM_����_���_�˻�"), _T("��������"), sBindStatus );
	if( sBindStatus ==_T("������ϴ���ڷ�"))
	{
		strQry = strQry + _T(" AND B.BOOK_TYPE = 'B' ");
	}

	if( sBindStatus ==_T("���ǵ�ϴ���ڷ�"))
	{
		strQry = strQry + _T(" AND B.BOOK_TYPE = 'C' ");

	}


	//���� �⵵ ���� ���� ���  
	GetControlData(_T("CM_����_���_�˻�"), _T("BS_���Գ⵵")	  , sACQUISIT_YEAR);
	sACQUISIT_YEAR.TrimLeft();
	sACQUISIT_YEAR.TrimRight();

	if ( sACQUISIT_YEAR != _T("") ) {
			strQry = strQry + _T(" AND B.ACQ_YEAR = '") + sACQUISIT_YEAR + _T("'");
	}
	//�Ϸù�ȣ
	GetControlData(_T("CM_����_���_�˻�"), _T("�Ϸù�ȣ����")   , preGroup1); 
	GetControlData(_T("CM_����_���_�˻�"), _T("�Ϸù�ȣ��")     , preGroup2); 


	//������ȣ 
	GetControlData(_T("CM_����_���_�˻�"), _T("������ȣ����")   , SEQ_NO1); 
	GetControlData(_T("CM_����_���_�˻�"), _T("������ȣ��")   , SEQ_NO2); 

	SEQ_NO1.TrimLeft();
	SEQ_NO1.TrimRight();
	SEQ_NO2.TrimLeft();
	SEQ_NO2.TrimRight();

	if ( (SEQ_NO1 != _T(""))&&(SEQ_NO2 != _T("")) )
	{
		strQry = strQry + _T(" AND B.RECEIPT_NO >= ") + SEQ_NO1 + _T(" AND B.RECEIPT_NO <= ") + SEQ_NO2 ;

		if ( (preGroup1 != _T(""))&&(preGroup2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.RECEIPT_SERIAL_NO >= ") + preGroup1 + _T(" AND B.RECEIPT_SERIAL_NO <= ") + preGroup2 ;
		}
		if ( (preGroup1 != _T(""))&&(preGroup2 == _T("")) )
		{
			strQry = strQry + _T(" AND B.RECEIPT_SERIAL_NO = ") + preGroup1 ;
		}
		if ( (preGroup1 == _T(""))&&(preGroup2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.RECEIPT_SERIAL_NO = ") + preGroup2 ;
		}

	}
	else if ( (SEQ_NO1 != _T(""))&&(SEQ_NO2 == _T("")) )
	{
		strQry = strQry + _T(" AND B.RECEIPT_NO = ") + SEQ_NO1;

		if ( (preGroup1 != _T(""))&&(preGroup2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.RECEIPT_SERIAL_NO >= ") + preGroup1 + _T(" AND B.RECEIPT_SERIAL_NO <= ") + preGroup2 ;
		}
		if ( (preGroup1 != _T(""))&&(preGroup2 == _T("")) )
		{
			strQry = strQry + _T(" AND B.RECEIPT_SERIAL_NO = ") + preGroup1 ;
		}
		if ( (preGroup1 == _T(""))&&(preGroup2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.RECEIPT_SERIAL_NO = ") + preGroup2 ;
		}

	}
	else if ( (SEQ_NO1 == _T(""))&&(SEQ_NO2 != _T("")) )
	{
		strQry = strQry + _T(" AND B.RECEIPT_NO = ") + SEQ_NO2;

		if ( (preGroup1 != _T(""))&&(preGroup2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.RECEIPT_SERIAL_NO >= ") + preGroup1 + _T(" AND B.RECEIPT_SERIAL_NO <= ") + preGroup2 ;
		}
		if ( (preGroup1 != _T(""))&&(preGroup2 == _T("")) )
		{
			strQry = strQry + _T(" AND B.RECEIPT_SERIAL_NO = ") + preGroup1 ;
		}
		if ( (preGroup1 == _T(""))&&(preGroup2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.RECEIPT_SERIAL_NO = ") + preGroup2 ;
		}

	}
	//���γ⵵ ���ι�ȣ
	GetControlData(_T("CM_����_���_�˻�"), _T("���ι�ȣ")   , sAccessionNoStart); 
	GetControlData(_T("CM_����_���_�˻�"), _T("���γ⵵")   , sAccessionNoYear); 
	sAccessionNoStart.TrimLeft();
	sAccessionNoStart.TrimRight();

	if (sAccessionNoStart != _T(""))
	{
		strQry = strQry + _T(" AND a.accession_rec_no = ")+sAccessionNoStart;
		
		// 2005.10.11 UPDATE BY PDJ
		// �˻����� ����� ������ �߸��Ǿ���		
		//if ( sACQUISIT_YEAR != _T("") ) {
		if ( sAccessionNoYear != _T("") ) {
			strQry = strQry + _T(" AND A.ACCESSION_REC_MAKE_YEAR = '") + sAccessionNoYear + _T("'");
		}
		
	}


	//���� ���� �ڵ� & ���Գ⵵ 
	GetControlData(_T("CM_����_���_�˻�"), _T("BB_���Ա���_�ڵ�"), sACQUISIT_CODE);
	
	sACQUISIT_CODE.TrimLeft();
	sACQUISIT_CODE.TrimRight();


	if ( sACQUISIT_CODE != _T("") ) {
		strQry = strQry + _T(" AND B.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}

	

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//��Ϲ�ȣ
	GetControlData(_T("CM_����_���_�˻�"), _T("BB_��Ϲ�ȣ_����"), sRegNoStart);
	GetControlData(_T("CM_����_���_�˻�"), _T("BB_��Ϲ�ȣ_����"), sRegNoEnd);
	
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
// 		if ( sRegNoStart.GetLength() > 2 ) {
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoStart.Mid(2) );
// 			sRegNoStart = sRegNoStart.Left(2) + sRight;
// 		}
// 	}
// 	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
// 		if ( sRegNoEnd.GetLength() > 2 ) {
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoEnd.Mid(2) );
// 			sRegNoEnd = sRegNoEnd.Left(2) + sRight;
// 		}
// 	}

	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	
	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
		if ( sRegNoStart.GetLength() > nRegCodeLength ) {
			sRegNoStart = pParentMgr->m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength), sRegNoStart.Mid(nRegCodeLength) );
		}
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
		if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
			sRegNoEnd = pParentMgr->m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength), sRegNoEnd.Mid(nRegCodeLength) );
		}
	}
	//=====================================================
	

	if ( sRegNoStart != _T("") && sRegNoEnd == _T("")) {
		strQry = _T("B.REG_NO = '") + sRegNoStart  + _T("' AND ") + strQry;
	}
	if ( sRegNoEnd != _T("") && sRegNoStart == _T("") ) {
		AfxMessageBox( _T("���� ��Ϲ�ȣ�� �Է� �ϼ���."));
		return _T("");
	}
	if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
		strQry = _T("B.REG_NO >= '") + sRegNoStart + _T("' AND B.REG_NO <= '") + sRegNoEnd + _T("' AND ") + strQry;
	}

	//�ΰ� ó����  

	GetControlData(_T("CM_����_���_�˻�"), _T("���ó����_����"), sTransferDataStart);
	GetControlData(_T("CM_����_���_�˻�"), _T("���ó����_����"), sTransferDataEnd);

	if ( sTransferDataStart != _T("") && sTransferDataEnd == _T("") ) {
		strQry = strQry + _T(" AND B.REG_TRANSFER_DATE = '") + sTransferDataStart + _T("'");
	}

	if ( sTransferDataStart == _T("") && sTransferDataEnd != _T("") ) {
		AfxMessageBox ( _T("���� �ΰ�ó������ �Է��ϼ���.") );
		return _T("");
	}

	if ( sTransferDataStart != _T("") && sTransferDataEnd != _T("") ) {
		strQry = strQry + _T(" AND B.REG_TRANSFER_DATE >= '") + sTransferDataStart + _T("' AND B.REG_TRANSFER_DATE <= '") + sTransferDataEnd + _T("'");
	}

// ==============================< 2003. 11. 25. ������. �߰� >==============================
	strQry = strQry + _T(" AND B.BINDED_BOOK_YN = 'N' ");  
// ==================< ���� å�̰ų� �������� ���� å�� �����ֱ� ���� �߰� >=================

	//�� �˻��� ��� 
	if ( sDetailFlag == 1)
	{
		CString sTitle, sAuthor;
		CString sFORM_CODE, sMEDIA_CODE, sMAT_CODE, sUSE_OBJ_CODE, sMANAGE_CODE, sREG_CODE;
		CString sWhereTitle;

		CMakeSearchData *pMakeData = NULL;
		pMakeData = new CMakeSearchData(this);

		//��ǥ��  
		GetControlData(_T("CM_����_���_�˻�"), _T("��ǥ��"), sTitle);	
		sTitle.TrimLeft();
		sTitle.TrimRight();

		if ( sTitle != _T("") ) {
			INT iCh = pMakeData->ChangeHanjaToHangul( sTitle );	//���� �ѱ� ��ȯ
			if ( 0 != iCh ) {
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return _T("");
			}
			iCh = pMakeData->GetKeyWord( sTitle );				//����, Ư������ ����
			if ( 0 != iCh ) {
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return _T("");
			}

			//sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );			
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {			
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_TITLE, '%s*')"), sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return _T("");
			}
			
			strQry = strQry + _T(" AND ") + sWhereTitle;			
		}

		//������
		GetControlData(_T("CM_����_���_�˻�"), _T("������"), sAuthor);
		sTitle.TrimLeft();
		sTitle.TrimRight();

		if ( sAuthor != _T("") ) {
			//sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );			
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {			
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_PUBLISHER, '%s*')"), sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return _T("");
			}
			
			strQry = strQry + _T(" AND ") + sWhereTitle;
		}

		//�ڵ� ���а�
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_���ı���_�ڵ�"), sFORM_CODE);
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_��ü����_�ڵ�"), sMEDIA_CODE);
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_�ڷᱸ��_�ڵ�"), sMAT_CODE);
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_�̿��󱸺�_�ڵ�"), sUSE_OBJ_CODE);
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_��������_�ڵ�"), sMANAGE_CODE);
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_��ϱ���_�ڵ�"), sREG_CODE);

		//�ڵ尪
		sFORM_CODE.TrimLeft();
		if ( sFORM_CODE != _T("") ) {
			strQry = strQry + _T(" AND TRIM(I.FORM_CODE) = '") + sFORM_CODE + _T("'");		
		}
		sMEDIA_CODE.TrimLeft();
		if ( sMEDIA_CODE != _T("") ) {
			strQry = strQry + _T(" AND TRIM(I.MEDIA_CODE) = '") + sMEDIA_CODE + _T("'");
		}
		sMAT_CODE.TrimLeft();
		if ( sMAT_CODE != _T("") ) {
			strQry = strQry + _T(" AND TRIM(I.MAT_CODE) = '") + sMAT_CODE + _T("'");
		}
		sUSE_OBJ_CODE.TrimLeft();
		if ( sUSE_OBJ_CODE != _T("") ) {
			strQry = strQry + _T(" AND TRIM(I.USE_OBJ_CODE) = '") + sUSE_OBJ_CODE + _T("'");
		}
		sMANAGE_CODE.TrimLeft();
		if ( sMANAGE_CODE != _T("") ) {
			strQry = strQry + _T(" AND TRIM(I.MANAGE_CODE) = '") + sMANAGE_CODE + _T("'");
		}
		sREG_CODE.TrimLeft();
		if ( sREG_CODE != _T("") ) {
			strQry = strQry + _T(" AND TRIM(I.REG_CODE) = '") + sREG_CODE + _T("'");
		}

		//�۾���
		CString sWorker;
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_�۾���"), sWorker);
		if ( sWorker != _T("") ) {
			strQry = strQry + _T(" AND B.WORKER = '") + sWorker + _T("'");
		}	

		
	}
	

	return strQry;

}

VOID CSE_CAT_SEARCH_MGR::OnbDetailNo() 
{
	// TODO: Add your control notification handler code here

	CString sDetailSee;
	GetDlgItem(IDC_bDetailNo)->GetWindowText(sDetailSee);

	if ( sDetailSee == _T("�����˻�") )
	{
		sDetailFlag = 0;
		CRect rect;
		this->GetWindowRect( rect );		
	//	rect.bottom -= 165;
		rect.bottom -= 195;
		this->MoveWindow( rect );

		GetDlgItem(IDC_bDetailNo)->SetWindowText(_T("�󼼰˻�"));
		GetDlgItem(IDC_guide_line)->ShowWindow( false );		
	}
	else
	{
		sDetailFlag = 1;
		CRect rect;
		this->GetWindowRect( rect );		
	//	rect.bottom += 165;
		rect.bottom += 195;
		this->MoveWindow( rect );

		GetDlgItem(IDC_bDetailNo)->SetWindowText(_T("�����˻�"));
		GetDlgItem(IDC_guide_line)->ShowWindow( true );
	}

}

VOID CSE_CAT_SEARCH_MGR::OnbREG0100CLEAR() 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_����_���_�˻�"));
	if (pCM)
		pCM->AllControlClear();	
}

//Ȳ���� �߰�
VOID CSE_CAT_SEARCH_MGR::OnSelchangeBindType() 
{
	CString sBindStatus;

	GetControlData( _T("CM_����_���_�˻�"), _T("��������"), sBindStatus );
	if( sBindStatus == _T("������ϴ���ڷ�") )
	{
		(GetDlgItem(IDC_strORDER_NO))->SetWindowText(_T("��������"));
	}
	else
	{
		(GetDlgItem(IDC_strORDER_NO))->SetWindowText(_T("������ȣ"));
	}
}

HBRUSH CSE_CAT_SEARCH_MGR::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
