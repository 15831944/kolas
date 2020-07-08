// SE_CAT_SEARCH_MGR1.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_SEARCH_MGR_GROUP.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_SEARCH_MGR_GROUP_GROUP dialog
#include "..\..\..\����\K2UP_Common\MakeSearchData.h"

CSE_CAT_SEARCH_MGR_GROUP::CSE_CAT_SEARCH_MGR_GROUP(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_SEARCH_MGR_GROUP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CAT_SEARCH_MGR_GROUP::~CSE_CAT_SEARCH_MGR_GROUP()
{
}
VOID CSE_CAT_SEARCH_MGR_GROUP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_SEARCH_MGR_GROUP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_SEARCH_MGR_GROUP, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_SEARCH_MGR_GROUP)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bDetailNo, OnbDetailNo)
	ON_BN_CLICKED(IDC_bREG0100CLEAR, OnbREG0100CLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_SEARCH_MGR_GROUP message handlers

BOOL CSE_CAT_SEARCH_MGR_GROUP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_���_�˻�_�Ŵ���_����")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}


	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_����_����"));

	CString m_Year;
	// 040304_HSS ���� LocalTime���� Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	SetControlData(_T("CM_����_���_�˻�_����"), _T("BS_���Գ⵵"), m_Year);


	//���� ���� 
	OnbDetailNo(); 

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CSE_CAT_SEARCH_MGR_GROUP::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}


VOID CSE_CAT_SEARCH_MGR_GROUP::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	CString strQry,strWorkNo1,strWorkNo2,strpreGroup1,strpreGroup2,strYear;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_����_����"));

	// ���� ��ȣ �� ������ ��ȣ�� ���ο� ���� �ش�.
	GetControlData(_T("CM_����_���_�˻�_����"), _T("���ι�ȣ����"), strpreGroup1);
	GetControlData(_T("CM_����_���_�˻�_����"), _T("���ι�ȣ��"), strpreGroup2);		 

	GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_���Գ⵵"), strYear);		 

	if( (strWorkNo1 != _T(""))&&(strWorkNo2 != _T("")) )
	{
		strWorkNo1 = strWorkNo1 + _T(" - ") +strWorkNo2;
	}
	if( (strpreGroup1 != _T(""))&&(strpreGroup2 != _T("")) )
	{
		strpreGroup1 = strpreGroup1 + _T(" - ") +strpreGroup2;
	}

	SetControlData(_T("CM_����_����_����_����"), _T("���ι�ȣ"), strpreGroup1);
	SetControlData(_T("CM_����_����_����_����"), _T("���γ⵵"), strYear);


	strQry = MakeSelectQry();
	if( strQry == _T("")) return;

	pDM->RefreshDataManager(strQry);

	CESL_ControlMgr *pCM;
	CESL_Grid *pGrid_MainGrid_All;

	pCM = FindCM(_T("CM_����_����_����_����"));
	pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_����_����"), _T("MAIN_GRID"));
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

INT CSE_CAT_SEARCH_MGR_GROUP::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
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
			else if ( str == _T("SEC111N") ) {
				str = _T("��������ڷ�");
			}
			else if ( str == _T("SEC112O") ) {
				str = _T("�����ΰ���");
			}
			else if ( str == _T("SEL211O") ) {
				str = _T("�����ΰ��ڷ�");
			}
			else if ( str == _T("SEL212N") ) {
				str = _T("�谡�ڷ�");
			}
			else if ( str == _T("SEL213O") ) {
				str = _T("���������ڷ�");
			}
			else if ( str == _T("SEL215O") ) {
				str = _T("�����ڷ�");
			}
			else if ( str == _T("SEL214O") ) {
				str = _T("�ļ��ڷ�");
			}

        }
    }
	return 0;


EFS_END
return -1;

}

CString CSE_CAT_SEARCH_MGR_GROUP::MakeSelectQry()
{

	CString strQry,strTmp; 
	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	CString sAccessionNoStart, sAccessionNoEnd,sAccessionNoYear;
	CString sRegNoStart, sRegNoEnd;
	CString sTransferDataStart, sTransferDataEnd;
	CString SEQ_NO1,SEQ_NO2,preGroup1,preGroup2;
	INT sAccessionFlag=0;

	//�ڷ����	
	strQry = strQry + _T(" B.WORKING_STATUS IN( ")
					  _T("'SER111N', 'SER112N', 'SER113O', ")
					  _T("'SEC111N', 'SEC112O', ")
					  //=====================================================
					  //2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 					  _T("'SEL211O', 'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O','SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O' ")
						// 18.09.27 JMJ KOL.RED.2018.008
					  _T("'SEL211O', 'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O','SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O', 'SEL411O', 'SEL511O', 'SEL611O' ")
					  //=====================================================
					  _T(") ");
	strQry = strQry + _T(" AND REG_NO IS NOT NULL ") ;

	//�������� 
	CString sBindStatus;
	GetControlData( _T("CM_����_���_�˻�_����"), _T("��������"), sBindStatus );
	if( sBindStatus ==_T("������ϴ���ڷ�"))
	{
		strQry = strQry + _T(" AND B.BOOK_TYPE = 'B' ");

	}
	if( sBindStatus ==_T("���ǵ�ϴ���ڷ�"))
	{
		strQry = strQry + _T(" AND B.BOOK_TYPE = 'C' ");

	}

	//���� �⵵ 
	GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_���Գ⵵")	  , sACQUISIT_YEAR);
	sACQUISIT_YEAR.TrimLeft();
	sACQUISIT_YEAR.TrimRight();
	
	//���ι�ȣ 
	GetControlData(_T("CM_����_���_�˻�_����"), _T("���ι�ȣ����")   , sAccessionNoStart); 
	GetControlData(_T("CM_����_���_�˻�_����"), _T("���ι�ȣ��")   , sAccessionNoEnd); 

	if ( (sAccessionNoStart != _T(""))&&(sAccessionNoEnd != _T("")) )
		strTmp = strTmp + _T(" AND a.ACCESSION_REC_NO >= ") + sAccessionNoStart + _T(" AND a.ACCESSION_REC_NO <= ") + sAccessionNoEnd ;
	
	else if ( sAccessionNoStart != _T("") )
		strTmp = strTmp + _T(" AND a.ACCESSION_REC_NO = ") + sAccessionNoStart;
	
	else if ( sAccessionNoEnd != _T("") )
		strTmp = strTmp + _T(" AND a.ACCESSION_REC_NO = ") + sAccessionNoEnd;
	
	if ( sACQUISIT_YEAR != _T("") ) {
		strTmp = strTmp + _T(" AND A.ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
	}

	//���� ���� �ڵ� & ���Գ⵵ 
	GetControlData(_T("CM_����_���_�˻�_����"), _T("BB_���Ա���_�ڵ�"), sACQUISIT_CODE);
	
	sACQUISIT_CODE.TrimLeft();
	sACQUISIT_CODE.TrimRight();

	if ( sACQUISIT_CODE != _T("") ) {
		strQry = strQry + _T(" AND B.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}

	//��Ϲ�ȣ
	GetControlData(_T("CM_����_���_�˻�_����"), _T("BB_��Ϲ�ȣ_����"), sRegNoStart);
	GetControlData(_T("CM_����_���_�˻�_����"), _T("BB_��Ϲ�ȣ_����"), sRegNoEnd);

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

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
	
	/// nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
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
		strTmp = strTmp + _T(" AND B.REG_NO = '") + sRegNoStart  + _T("'");
	}
	if ( sRegNoEnd != _T("") && sRegNoStart == _T("") ) {
		AfxMessageBox( _T("���� ��Ϲ�ȣ�� �Է� �ϼ���."));
		return _T("");
	}
	if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
		strTmp = strTmp+_T(" AND B.REG_NO >= '") + sRegNoStart + _T("' AND B.REG_NO <= '") + sRegNoEnd + _T("'");
	}


	//�ΰ� ó����  
	GetControlData(_T("CM_����_���_�˻�_����"), _T("�������_����"), sTransferDataStart);
	GetControlData(_T("CM_����_���_�˻�_����"), _T("�������_����"), sTransferDataEnd);

	if ( sTransferDataStart != _T("") && sTransferDataEnd == _T("") ) {
		strTmp = strTmp + _T(" AND B.REG_TRANSFER_DATE = '") + sTransferDataStart + _T("'");
	}

	if ( sTransferDataStart == _T("") && sTransferDataEnd != _T("") ) {
		AfxMessageBox ( _T("���� ���ó������ �Է��ϼ���.") );
		return _T("");
	}

	if ( sTransferDataStart != _T("") && sTransferDataEnd != _T("") ) {
		strTmp = strTmp + _T(" AND B.REG_TRANSFER_DATE >= '") + sTransferDataStart + _T("' AND B.REG_TRANSFER_DATE <= '") + sTransferDataEnd + _T("'");
	}

	// 040309_HSS �߰� 
	strQry = strQry + _T(" AND B.BINDED_BOOK_YN = 'N' ");  

	//�� �˻��� ��� 
	if ( sDetailFlag == 1)
	{
		CString sTitle, sAuthor;
		CString sFORM_CODE, sMEDIA_CODE, sMAT_CODE, sUSE_OBJ_CODE, sMANAGE_CODE;
		CString sWhereTitle;

		CMakeSearchData *pMakeData = NULL;
		pMakeData = new CMakeSearchData(this);

		//��ǥ��  
		GetControlData(_T("CM_����_���_�˻�_����"), _T("��ǥ��"), sTitle);	
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
		GetControlData(_T("CM_����_���_�˻�_����"), _T("������"), sAuthor);
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
		GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_���ı���_�ڵ�"), sFORM_CODE);
		GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_��ü����_�ڵ�"), sMEDIA_CODE);
		GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_�ڷᱸ��_�ڵ�"), sMAT_CODE);
		GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_�̿��󱸺�_�ڵ�"), sUSE_OBJ_CODE);
		GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_��������_�ڵ�"), sMANAGE_CODE);

		//�ڵ尪
		sFORM_CODE.TrimLeft();
		if ( sFORM_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.FORM_CODE = '") + sFORM_CODE + _T("'");		
		}
		sMEDIA_CODE.TrimLeft();
		if ( sMEDIA_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.MEDIA_CODE = '") + sMEDIA_CODE + _T("'");
		}
		sMAT_CODE.TrimLeft();
		if ( sMAT_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.MAT_CODE = '") + sMAT_CODE + _T("'");
		}
		sUSE_OBJ_CODE.TrimLeft();
		if ( sUSE_OBJ_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.USE_OBJ_CODE = '") + sUSE_OBJ_CODE + _T("'");
		}
		sMANAGE_CODE.TrimLeft();
		if ( sMANAGE_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.MANAGE_CODE = '") + sMANAGE_CODE + _T("'");
		}

		//�۾���
		CString sWorker;
		GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_�۾���"), sWorker);
		if ( sWorker != _T("") ) {
			strQry = strQry + _T(" AND B.WORKER = '") + sWorker + _T("'");
		}	

		
	}
	
	if( strTmp == _T("") )
	{
		ESLAfxMessageBox(_T("�ڷᰡ �����ϴ�. ������ �ɾ� �ֽʽÿ�!!"));
		return _T("");
	}

	strQry += strTmp;

	return strQry;

}

VOID CSE_CAT_SEARCH_MGR_GROUP::OnbDetailNo() 
{
	// TODO: Add your control notification handler code here

	CString sDetailSee;
	GetDlgItem(IDC_bDetailNo)->GetWindowText(sDetailSee);

	if ( sDetailSee == _T("�����˻�") )
	{
		sDetailFlag = 0;
		CRect rect;
		this->GetWindowRect( rect );		
		rect.bottom -= 165;
		this->MoveWindow( rect );

		GetDlgItem(IDC_bDetailNo)->SetWindowText(_T("�󼼰˻�"));
		GetDlgItem(IDC_guide_line)->ShowWindow( false );		
	}
	else
	{
		sDetailFlag = 1;
		CRect rect;
		this->GetWindowRect( rect );		
		rect.bottom += 165;
		this->MoveWindow( rect );

		GetDlgItem(IDC_bDetailNo)->SetWindowText(_T("�����˻�"));
		GetDlgItem(IDC_guide_line)->ShowWindow( true );
	}

}

VOID CSE_CAT_SEARCH_MGR_GROUP::OnbREG0100CLEAR() 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_����_���_�˻�_����"));
	if (pCM)
		pCM->AllControlClear();		
}

HBRUSH CSE_CAT_SEARCH_MGR_GROUP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
