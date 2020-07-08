// SE_CAT_SEARCH_MGR_PRE.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_SEARCH_MGR_PRE.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_SEARCH_MGR_PRE dialog
#include "..\..\..\����\K2UP_Common\MakeSearchData.h"


CBO_CAT_SEARCH_MGR_PRE::CBO_CAT_SEARCH_MGR_PRE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_SEARCH_MGR_PRE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pMakeData = new CMakeSearchData(this);
}

CBO_CAT_SEARCH_MGR_PRE::~CBO_CAT_SEARCH_MGR_PRE()
{
	if(pMakeData)
	{
		delete pMakeData;
		pMakeData = NULL;
	}
}

BOOL CBO_CAT_SEARCH_MGR_PRE::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}
VOID CBO_CAT_SEARCH_MGR_PRE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_SEARCH_MGR_PRE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_SEARCH_MGR_PRE, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_SEARCH_MGR_PRE)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bDetailNo, OnbDetailNo)
	ON_BN_CLICKED(IDC_bREG0100CLEAR, OnbREG0100CLEAR)
	ON_EN_SETFOCUS(IDC_REG_AUTHOR, OnSetfocusRegAuthor)
	ON_EN_SETFOCUS(IDC_REG_TITLE, OnSetfocusRegTitle)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_SEARCH_MGR_PRE message handlers

BOOL CBO_CAT_SEARCH_MGR_PRE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_���_�˻�_�Ŵ���_������")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}


	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_������_����"));

	CString m_Year;
	// 040304_HSS ���� LocalTime���� Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	SetControlData(_T("CM_����_���_�˻�_������"), _T("BS_���Գ⵵"), m_Year);

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	//���� ���� 
	OnbDetailNo(); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBO_CAT_SEARCH_MGR_PRE::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_�ڷ����_�ڵ�")) {
            if ( str == _T("BOR111N")) {
				str = _T("����ΰ��ڷ�");
			}
			else if ( str == _T("BOR112N") ) {
				str = _T("����ڷ�");
			}
			else if ( str == _T("BOR113O") ) {
				str = _T("�������ڷ�");
			}
        }
    }
	return 0;


EFS_END
return -1;

}

CString CBO_CAT_SEARCH_MGR_PRE::MakeSelectQry()
{

	CString strQry; 
	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	CString sAccessionNoStart, sAccessionNoEnd;
	CString sRegNoStart, sRegNoEnd;
	CString sTransferDataStart, sTransferDataEnd;
	CString SEQ_NO1,SEQ_NO2,preGroup1,preGroup2;
	INT sAccessionFlag=0;

	//�ڷ� ����
	strQry = strQry + _T(" B.WORKING_STATUS = 'BOR113O'");

	strQry += _T(" AND B.BOOK_APPENDIX_FLAG = 'B' ");


	//���� �⵵ ���� ���� ���  
	GetControlData(_T("CM_����_���_�˻�_������"), _T("BS_���Գ⵵")	  , sACQUISIT_YEAR);
	sACQUISIT_YEAR.TrimLeft();
	sACQUISIT_YEAR.TrimRight();

	
	//�����ο��ι�ȣ 
	GetControlData(_T("CM_����_���_�˻�_������"), _T("�����ι�ȣ����")   , sAccessionNoStart); 
	GetControlData(_T("CM_����_���_�˻�_������"), _T("�����ι�ȣ��")   , sAccessionNoEnd); 

	sAccessionNoStart.TrimLeft();
	sAccessionNoStart.TrimRight();
	sAccessionNoEnd.TrimLeft();
	sAccessionNoEnd.TrimRight();

	if (sAccessionNoStart != _T("") && sAccessionNoEnd != _T(""))
	{
		strQry = strQry + _T(" AND a.accession_rec_no >= ")+sAccessionNoStart+_T(" AND a.accession_rec_no <=")+sAccessionNoEnd ;
		
		if ( sACQUISIT_YEAR != _T("") ) {
			strQry = strQry + _T(" AND A.ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
		}
		
	}
	else if (sAccessionNoStart != _T("") && sAccessionNoEnd == _T(""))
	{
		strQry = strQry + _T(" AND a.accession_rec_no = ")+sAccessionNoStart;

		if ( sACQUISIT_YEAR != _T("") ) {
			strQry = strQry + _T(" AND A.ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
		}

	}
	


	//���� ���� �ڵ� & ���Գ⵵ 
	GetControlData(_T("CM_����_���_�˻�_������"), _T("BB_���Ա���_�ڵ�"), sACQUISIT_CODE);
	
	sACQUISIT_CODE.TrimLeft();
	sACQUISIT_CODE.TrimRight();


	if ( sACQUISIT_CODE != _T("") ) {
		strQry = strQry + _T(" AND B.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}



	//�� �˻��� ��� 
	if ( sDetailFlag == 1)
	{
		CString sTitle, sAuthor;
		CString sFORM_CODE, sMEDIA_CODE, sMAT_CODE, sUSE_OBJ_CODE, sMANAGE_CODE;
		CString sWhereTitle;

		//CMakeSearchData *pMakeData = NULL;

		//��ǥ��  
		GetControlData(_T("CM_����_���_�˻�_������"), _T("��ǥ��"), sTitle);	
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

						
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return _T("");
			}
			strQry = strQry + _T(" AND ") + sWhereTitle;
			
			/*
			//#ifdef _ORA_CAT_SEARCH_
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			//#else
			//	sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
			//#endif
			strQry = strQry + _T(" AND ") + sWhereTitle;			
			*/
		}

		//������
		GetControlData(_T("CM_����_���_�˻�_������"), _T("������"), sAuthor);
		sTitle.TrimLeft();
		sTitle.TrimRight();

		if ( sAuthor != _T("") ) {
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return _T("");
			}
			strQry = strQry + _T(" AND ") + sWhereTitle;
			
			/*
			//#ifdef _ORA_CAT_SEARCH_
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
			//#else
			//	sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
			//#endif
			strQry = strQry + _T(" AND ") + sWhereTitle;
			*/
		}

		//�ڵ� ���а�
		GetControlData(_T("CM_����_���_�˻�_������"), _T("BS_���ı���_�ڵ�"), sFORM_CODE);
		GetControlData(_T("CM_����_���_�˻�_������"), _T("BS_��ü����_�ڵ�"), sMEDIA_CODE);
		GetControlData(_T("CM_����_���_�˻�_������"), _T("BS_�ڷᱸ��_�ڵ�"), sMAT_CODE);
		GetControlData(_T("CM_����_���_�˻�_������"), _T("BS_�̿��󱸺�_�ڵ�"), sUSE_OBJ_CODE);
		GetControlData(_T("CM_����_���_�˻�_������"), _T("BS_��������_�ڵ�"), sMANAGE_CODE);

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

		//�۾���(�������� 2004.09.15)
		CString sWorker;
		GetControlData(_T("CM_����_���_�˻�_������"), _T("BS_�۾���"), sWorker);
		if ( sWorker != _T("") ) {
			strQry = strQry + _T(" AND B.SPECIES_KEY IN (SELECT REC_KEY FROM BO_SPECIES_TBL WHERE WORKER = '") + sWorker + _T("')");
			//strQry = strQry + _T(" AND B.WORKER = '") + sWorker + _T("'");
		}	


	}


	return strQry;

}



VOID CBO_CAT_SEARCH_MGR_PRE::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	CString strQry,strWorkNo1,strWorkNo2,strpreGroup1,strpreGroup2,strYear,sACQUISIT_TYPE;

	CESL_DataMgr *pDM;
	INT ids;
	pDM = FindDM(_T("DM_����_����_������_����"));



	// ���� ��ȣ �� ������ ��ȣ�� ���ο� ���� �ش�.
	GetControlData(_T("CM_����_���_�˻�_������"), _T("�����ι�ȣ����"), strpreGroup1);
	GetControlData(_T("CM_����_���_�˻�_������"), _T("�����ι�ȣ��"), strpreGroup2);		 

	GetControlData(_T("CM_����_���_�˻�_������"), _T("BS_���Գ⵵"), strYear);		 
	GetControlData(_T("CM_����_���_�˻�_������"), _T("BB_���Ա���_�ڵ�")	  , sACQUISIT_TYPE , -1 ,1);


	if( (strpreGroup1 != _T(""))&&(strpreGroup2 != _T("")) )
	{
		strpreGroup1 = strpreGroup1 + _T(" - ") +strpreGroup2;
	}

	SetControlData(_T("CM_����_����_������_����"), _T("�����ι�ȣ"), strpreGroup1);
	SetControlData(_T("CM_����_����_������_����"), _T("�����γ⵵"), strYear);
	SetControlData(_T("CM_����_����_������_����"), _T("�Լ�����"), sACQUISIT_TYPE);


	strQry = MakeSelectQry();
	if( strQry == _T("")) return;

	AfxGetApp()->DoWaitCursor( 1 );

	ids = pDM->RefreshDataManager(strQry);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ���� �ּ���"));
		return;
	}
	
	// ���� ����(2004.09.15) ++++++++++
	AfxGetApp()->DoWaitCursor( -1 );

	INT nCount = pDM->GetRowCount();
	if ( nCount <= 0 ) 
	{
		AfxMessageBox(_T("�˻� ����� �����ϴ�."));
	}
	// --------------------------------

	CESL_ControlMgr *pCM;
	CESL_Grid *pGrid_MainGrid_All;

	pCM = FindCM(_T("CM_����_����_������_����"));
	pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_������_����"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.11.21 ������ �߰� 
	// �˻���� ���¹ٿ� ��Ÿ���� 
    CString strMsg ;
	INT nRowCount = 0 ;
	INT nBookCount = 0;
	nBookCount = pDM->GetRowCount () ;
	CMap <CString ,LPCTSTR,CString,LPCTSTR> mapRecKey;
	CString strSpecies_key;
	for(INT i =0 ; i<nBookCount;i++)
	{
        strSpecies_key=_T("") ;
		pDM->GetCellData (_T("IBS_��KEY"),i,strSpecies_key);
		if(!strSpecies_key.IsEmpty ())
        { 
			mapRecKey.SetAt(strSpecies_key,strSpecies_key);
		}
	}

	nRowCount = mapRecKey.GetCount();
    
    strMsg.Format(_T("���� : %5d\tå�� : %5d"), nRowCount, nBookCount);
    GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	//�׸��� �ݹ� �Լ� ���� 

	pCM->AllControlDisplay();	

	if( nRowCount > 0 )
		ShowWindow(SW_HIDE);

}

VOID CBO_CAT_SEARCH_MGR_PRE::OnbDetailNo() 
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

VOID CBO_CAT_SEARCH_MGR_PRE::OnbREG0100CLEAR() 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_����_���_�˻�_������"));
	if (pCM)
		pCM->AllControlClear();
}

VOID CBO_CAT_SEARCH_MGR_PRE::SetIME(INT nIDC, INT nMode)
{
    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(nIDC)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		if ( nMode == 0 ) dwConversion=IME_CMODE_ALPHANUMERIC;
		else dwConversion=IME_CMODE_HANGUL;		

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(nIDC)->m_hWnd,hIME); 
    } 
}

VOID CBO_CAT_SEARCH_MGR_PRE::OnSetfocusRegAuthor() 
{
	SetIME(IDC_REG_AUTHOR, 1);	
}

VOID CBO_CAT_SEARCH_MGR_PRE::OnSetfocusRegTitle() 
{
	SetIME(IDC_REG_TITLE, 1);	
}

HBRUSH CBO_CAT_SEARCH_MGR_PRE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

