// SE_LOC_1110.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1110.h"

#include "..\..\..\..\����\����\�弭����\LOC_CommonAPI\LocCommonAPI.h"
// SanFile�� �����ϱ� ����
#include "..\..\..\..\����\����\�弭����\OpenSanFile\\InputSanFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1110 dialog


CSE_LOC_1110::CSE_LOC_1110(CESL_Mgr* pParent /*=NULL*/, CString strMenuAlias)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1110)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM_Result = NULL;
	m_pCM_Parent = NULL;

	m_bDetail = TRUE;

	m_strMenuAlias = strMenuAlias;

	m_pMakeSearchData = new CMakeSearchData(this);
}

CSE_LOC_1110::~CSE_LOC_1110()
{
	delete m_pMakeSearchData;
}

BOOL CSE_LOC_1110::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

VOID CSE_LOC_1110::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1110)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSE_LOC_1110, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1110)
	ON_BN_CLICKED(IDC_bSEARCH_CAT, OnbSEARCHCAT)
	ON_BN_CLICKED(IDC_bDETAIL, OnbDetail)
	ON_BN_CLICKED(IDC_bCLEAR_SCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_bFileSearch, OnbFileSearch)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CString CSE_LOC_1110::GetWorkingStatusSQL()
{
	EFS_BEGIN

	CString strResult;

	CString strWorkingStatus;
	m_pCM->GetControlMgrData(_T("BS_�ڷ����_�ڵ�"), strWorkingStatus);

	if (strWorkingStatus == _T("�����ΰ��ڷ�"))
	{
		strResult = _T("b.WORKING_STATUS = 'SEL211O'");
	}
	else if( strWorkingStatus == _T("�����ڷ�"))
	{																	
		// 2005-12-27 UPD BY PDW(Jr) ===============================================================================
		/* ������
		strResult = _T("b.WORKING_STATUS IN ("HOLD_SE_SEARCH_CONDITION_T(") AND b.PREV_WORKING_STATUS NOT IN ('SEL112N')"));
		*/
		strResult = _T("b.WORKING_STATUS IN (")_T(HOLD_SE_SEARCH_CONDITION)_T(",'SEL312O')");
		// =========================================================================================================
	}

	return strResult;

	EFS_END
	return _T("");
}

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1110 message handlers

BOOL CSE_LOC_1110::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SE_LOC_1110")) < 0)
	{
		AfxMessageBox( _T("CSE_LOC_1110::OnInitDialog() ERROR -1") );
		return FALSE;
	}

	m_pDM_Result = FindDM(_T("DM_SE_LOC_1100"));
	m_pCM_Parent = FindCM(_T("CM_SE_LOC_1100"));
	m_pCM = FindCM(_T("CM_SE_LOC_1110"));


	if (m_pDM_Result == NULL ||
		m_pCM_Parent == NULL ||
		m_pCM == NULL) 
	{
		AfxMessageBox( _T("CSE_LOC_1110::OnInitDialog() ERROR -1") );
		return FALSE;
	}

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cmbREGCODE);
	if ( !pCB )
	{
		AfxMessageBox( _T("CSE_LOC_1110::OnInitDialog() ERROR -1") );
		return FALSE;
	}
	pCB->SetCurSel( -1 );

	OnbDetail();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
	return FALSE;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-05 ~ 2005-12-05
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   �Լ�����   ] 
      [����-�谡���ڱ��] �˻��ϱ� 
   
   [   �̴Ͻ���   ]
      1. �⺻ ������ ����
	  2. �߰� ������ ���� 
--------------------------------------------------------------------------------------------*/
VOID CSE_LOC_1110::OnbSEARCHCAT() 
{
EFS_BEGIN

	FILE *fp = _tfopen( _T("..\\CFG\\_newecosearch"), _T("rb") );
	
	CStringList whereList;
	CString sBuf;
		
	// ----------------------------------------------------------------------------
	// 1. �⺻ ������ ����     
	// ----------------------------------------------------------------------------
	CString sAccessionNoStart, sAccessionNoEnd;
	CString sWORKING_STATUS;
	CString sControlNo, sMAT_CODE , sMEDIA_CODE;
	CString sTitle, sKeyWord, sAuthor, sPublisher;
	
	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	m_pCM->GetControlMgrData(_T("BB_���Ա���_�ڵ�"), sACQUISIT_CODE);
	m_pCM->GetControlMgrData(_T("BS_���Գ⵵"), sACQUISIT_YEAR);
	// 2005-12-15 UPD BY PDW(Jr) ==================================================
	// ��Ϲ�ȣ �Է� ���� ����	
	/* ������
	CString sRegNoStart, sRegNoEnd;
	m_pCM->GetControlMgrData(_T("BB_��Ϲ�ȣ_����"), sRegNoStart);
	m_pCM->GetControlMgrData(_T("BB_��Ϲ�ȣ_����"), sRegNoEnd);

	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
		if ( sRegNoStart.GetLength() > 2 ) {
			CString sRight;
			sRight.Format( _T("%010s") , sRegNoStart.Mid(2) );
			sRegNoStart = sRegNoStart.Left(2) + sRight;
		}
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
		if ( sRegNoEnd.GetLength() > 2 ) {
			CString sRight;
			sRight.Format( _T("%010s") , sRegNoEnd.Mid(2) );
			sRegNoEnd = sRegNoEnd.Left(2) + sRight;
		}
	}
	*/
	CString sRegCode    = _T("");
	CString sRegNoStart = _T("");
	CString sRegNoEnd   = _T("");
	m_pCM->GetControlMgrData(_T("BB_��ϱ���"),      sRegCode);
	m_pCM->GetControlMgrData(_T("BB_��Ϲ�ȣ_����"), sRegNoStart);
	m_pCM->GetControlMgrData(_T("BB_��Ϲ�ȣ_����"), sRegNoEnd);

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	if ( !sRegCode.IsEmpty() )
// 	{
// 		if ( !sRegNoStart.IsEmpty() && sRegNoStart.GetLength() < 10 )
// 		{
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoStart );
// 			sRegNoStart = sRegCode + sRight;
// 		}
// 
// 		if ( !sRegNoEnd.IsEmpty() && sRegNoEnd.GetLength() < 10 )
// 		{
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoEnd );
// 			sRegNoEnd = sRegCode + sRight;
// 		}
// 	}	
// 	else
// 	{
// 		if ( !sRegNoStart.IsEmpty() || !sRegNoEnd.IsEmpty() )
// 		{
// 			AfxMessageBox( _T("��ϱ����� �Է� �ϼ���."));
// 			return;
// 		}
// 	}
	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	if ( !sRegCode.IsEmpty() )
	{
		if ( !sRegNoStart.IsEmpty() && sRegNoStart.GetLength() < 12-nRegCodeLength )
		{
			sRegNoStart = m_pInfo->MakeRegNo(sRegCode,sRegNoStart);
		}

		if ( !sRegNoEnd.IsEmpty() && sRegNoEnd.GetLength() < 12-nRegCodeLength )
		{
			sRegNoEnd = m_pInfo->MakeRegNo(sRegCode,sRegNoEnd);
		}
	}	
	else
	{
		if ( !sRegNoStart.IsEmpty() || !sRegNoEnd.IsEmpty() )
		{
			AfxMessageBox( _T("��ϱ����� �Է� �ϼ���."));
			return;
		}
	}
	//=====================================================
	
	// ============================================================================

	m_pCM->GetControlMgrData(_T("BS_�ڷ����_�ڵ�"), sWORKING_STATUS);
	m_pCM->GetControlMgrData(_T("BS_�ڷᱸ��_�ڵ�"), sMAT_CODE);
	m_pCM->GetControlMgrData(_T("BS_��ü����_�ڵ�"), sMEDIA_CODE);
	m_pCM->GetControlMgrData(_T("IBS_��ǥ��"), sTitle);
	m_pCM->GetControlMgrData(_T("IBS_Ű����"), sKeyWord);
	m_pCM->GetControlMgrData(_T("IBS_������"), sAuthor);
	m_pCM->GetControlMgrData(_T("IBS_������"), sPublisher);
	
	//--------------------------------------------------------------------------
	// 2005-11-23 WORK ADD BY PDW(Jr)
	// ��������
	//--------------------------------------------------------------------------
	CString sManageCode;
	m_pCM->GetControlMgrData(_T("��������"), sManageCode);

	CString sWhere = _T("I.REC_KEY = S.REC_KEY");
	
	//�ڼ��� ��ư üũ
	
	//�����ȣ
	if (m_bDetail) {
		CString sControlNoStart, sControlNoEnd;
		m_pCM->GetControlMgrData(_T("�����ȣ����"), sControlNoStart );
		m_pCM->GetControlMgrData(_T("�����ȣ����"), sControlNoEnd );
		if ( sControlNoStart == _T("") && sControlNoEnd != _T("") ) {
			AfxMessageBox( _T("���� �����ȣ�� �Է��ϼ���.") );
			return;
		}
		else if ( sControlNoStart != _T("") && sControlNoEnd == _T("") ) {
			sWhere = sWhere + _T(" AND I.CONTROL_NO = '") + sControlNoStart + _T("'");
		}
		else if ( sControlNoStart != _T("") && sControlNoEnd != _T("") ) {
			sWhere = sWhere + _T(" AND I.CONTROL_NO >= '") + sControlNoStart + _T("' AND I.CONTROL_NO <= '") + sControlNoEnd + _T("'");
		}
	}

	//�ڵ尪	
	if (m_bDetail) {
		sMAT_CODE.Replace ( _T(" ") , NULL );
		if ( sMAT_CODE != _T("") ) {
			sWhere = sWhere + _T(" AND I.MAT_CODE = '") + sMAT_CODE + _T("'");
		}
		sMEDIA_CODE.Replace ( _T(" ") , NULL );
		if ( sMEDIA_CODE != _T("") ) {
			sWhere = sWhere + _T(" AND I.MEDIA_CODE = '") + sMEDIA_CODE + _T("'");
		}

		// �����
		CString strPubFreq;
		m_pCM->GetControlMgrData(_T("IBS_�����"), strPubFreq);
		strPubFreq.Replace(_T(" "), _T(""));
		if (!strPubFreq.IsEmpty())
		{
			sWhere = sWhere + _T(" AND I.PUB_FREQ = '") + strPubFreq + _T("'");
		}
	}	

	//���ι�ȣ �˻�
	CString strAccessionQuery;
	CString strAccestionNoMakeYear;
	CString strAccessionNoStart;
	CString strAccessionNoEnd;

	m_pCM->GetControlMgrData(_T("�����ۼ��⵵"), strAccestionNoMakeYear);
	m_pCM->GetControlMgrData(_T("���ι�ȣ����"), strAccessionNoStart);
	m_pCM->GetControlMgrData(_T("���ι�ȣ����"), strAccessionNoEnd);

	if (strAccessionNoStart.IsEmpty() && !strAccessionNoEnd.IsEmpty()) 
	{
		AfxMessageBox(_T("���� ���ι�ȣ�� �Է� �ϼ���"));
		return;
	}
	else if (!strAccessionNoStart.IsEmpty() && strAccessionNoEnd.IsEmpty())
	{
		strAccessionQuery.Format(_T("(SELECT MAX(a.accession_rec_no) FROM CO_ACCESSION_REC_TBL a WHERE BB.accession_rec_key = a.rec_key ) = %s  "), strAccessionNoStart);		
	}
	else if (!strAccessionNoStart.IsEmpty() && !strAccessionNoEnd.IsEmpty())
	{
		strAccessionQuery.Format(_T("(SELECT MAX(a.accession_rec_no) FROM CO_ACCESSION_REC_TBL a WHERE BB.accession_rec_key = a.rec_key ) BETWEEN %s and  %s"), strAccessionNoStart, strAccessionNoEnd);		
	}

	if (!strAccestionNoMakeYear.IsEmpty())
	{
		CString strAccessionQuery2;
		strAccessionQuery2.Format( _T("(SELECT MAX(a.ACCESSION_REC_MAKE_YEAR ) FROM CO_ACCESSION_REC_TBL a WHERE BB.accession_rec_key = a.rec_key ) = '%s' ") , strAccestionNoMakeYear );		
		if( !strAccessionQuery.IsEmpty() )
			strAccessionQuery += _T(" AND ");
		strAccessionQuery += strAccessionQuery2;
	}	
	
	// ���� �˻�
	CString strSeqNoQuery;
	CString strSeqNoStart;
	CString strSeqNoEnd;
	m_pCM->GetControlMgrData(_T("������ȣ����"), strSeqNoStart);
	m_pCM->GetControlMgrData(_T("������ȣ����"), strSeqNoEnd);

	if (strSeqNoStart.IsEmpty() && !strSeqNoEnd.IsEmpty()) 
	{
		AfxMessageBox(_T("���� ������ȣ �Է��ϼ���"));
		return;
	}
	else if (!strSeqNoStart.IsEmpty() && strSeqNoEnd.IsEmpty())
	{
		strSeqNoQuery.Format(_T("BB.RECEIPT_NO = %s"), strSeqNoStart);
	}
	else if (!strSeqNoStart.IsEmpty() && !strSeqNoEnd.IsEmpty())
	{
		strSeqNoQuery.Format(_T("BB.RECEIPT_NO >= %s and BB.RECEIPT_NO <= %s"), strSeqNoStart, strSeqNoEnd);
	}

	//�ڷ����
	CString strWorkingStatusQuery = GetWorkingStatusSQL();
	CString sBookWhere = strWorkingStatusQuery;


	//���Ա���, ���Գ⵵
	sACQUISIT_CODE.Replace ( _T(" ") , NULL );
	if ( sACQUISIT_CODE != _T("") ) {
		sBookWhere = sBookWhere + _T(" AND b.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}
	if ( sACQUISIT_YEAR != _T("") ) {
		sBookWhere = sBookWhere + _T(" AND B.ACQ_YEAR = '") + sACQUISIT_YEAR + _T("'");
	}

	//��Ϲ�ȣ�˻�
	CString strWhere = sWhere;

	if (m_bDetail) {
		
		// 2005-11-23 ADD BY PDW(Jr) ==================================================
		// �������� �߰� 
		if ( sManageCode != _T("") ) 
		{			
			sBookWhere = sBookWhere + _T(" AND b.MANAGE_CODE = '") + sManageCode + _T("'");
		}
		// ============================================================================		

		// 2005-12-15 UPD BY PDW(Jr) ==================================================
		// ��ϱ��� �߰� 
		if ( !sRegCode.IsEmpty() )
		{
			sBookWhere = sBookWhere + _T(" AND b.REG_CODE = '") + sRegCode + _T("'");
		}
		// ============================================================================		

		if ( sRegNoStart != _T("") && sRegNoEnd == _T("")) {			
			if ( sBookWhere != _T("") ) {
				if( !strAccessionNoStart.IsEmpty() && strAccessionNoEnd.IsEmpty() )
					sBookWhere = sBookWhere + _T(" AND b.REG_NO >= '") + sRegNoStart + _T("'");
				else
					sBookWhere = sBookWhere + _T(" AND b.REG_NO = '") + sRegNoStart + _T("'");
			}
		}		
		if ( sRegNoEnd != _T("") && sRegNoStart == _T("") ) {
			AfxMessageBox( _T("���� ��Ϲ�ȣ�� �Է� �ϼ���."));
			return;
		}
		if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {	
			if ( sBookWhere == _T("") ) {
				sBookWhere = _T("b.REG_NO >= '") + sRegNoStart + _T("' AND b.REG_NO <= '") + sRegNoEnd + _T("'");
			}
			else {
				sBookWhere = sBookWhere + _T(" AND b.REG_NO >= '") + sRegNoStart + _T("' AND b.REG_NO <= '") + sRegNoEnd + _T("'");
			}
		}		

		bool bIsEqualSearch;

		//Ű���� �˻�
		if ( sKeyWord != _T("") ){
			/// 2003�� 9�� 1�� CATSEARCH -> ECOSEARCH�� ��ȯ
			bIsEqualSearch = FALSE;
			// ���ô� ������ �˻� ����
			if( sKeyWord.GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			m_pMakeSearchData->GetKeyWord(sKeyWord);
			if( !bIsEqualSearch ) 
				sKeyWord += _T("*");
			
			CString sWhereTitle;

			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				if( fp )
					sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_KEYWORD, '") + sKeyWord + _T("') ");
				else
				{
					sBuf.Format( _T(" &ECOSEARCH( , SE_IDX_KEYWORD, '%s' ) "), sKeyWord );
					whereList.AddTail( sBuf );
				}
			}
			else
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_KEYWORD,'%s*',NULL)>0") , sKeyWord );

/*
#ifndef _ECOSEARCH_
			sWhereTitle.Format( _T("CATSEARCH(I.IDX_KEYWORD,'%s*',NULL)>0") , sKeyWord );
#endif
#ifdef _ECOSEARCH_
			sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_KEYWORD, '") + sKeyWord + _T("') ");
#endif
			*/
			
			if( !sWhereTitle.IsEmpty() )
				strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//����˻�	
		//sTitle
		if ( sTitle != _T("") ){
			/// 2003�� 9�� 1�� CATSEARCH -> ECOSEARCH�� ��ȯ
			bIsEqualSearch = FALSE;
			// ���ô� ������ �˻� ����
			if( sTitle.GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			INT iCh = m_pMakeSearchData->ChangeHanjaToHangul( sTitle );	//���� �ѱ� ��ȯ
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return;
			}
			iCh = m_pMakeSearchData->GetKeyWord( sTitle );				//����, Ư������ ����
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return;
			}
			
			if( !bIsEqualSearch ) 
				sTitle += _T("*");
			
			
			CString sWhereTitle;

			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				if( fp )
					sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_TITLE, '") + sTitle + _T("') ");
				else
				{
					sBuf.Format( _T(" &ECOSEARCH( , SE_IDX_TITLE, '%s' ) "), sTitle );
					whereList.AddTail( sBuf );
				}
			}
			else
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );

		/*
#ifndef _ECOSEARCH_
			sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
#endif
#ifdef _ECOSEARCH_
			sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_TITLE, '") + sTitle + _T("') ");
#endif
			*/
			if( !sWhereTitle.IsEmpty() )
				strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//������
		if ( sAuthor != _T("") ){
			/// 2003�� 9�� 1�� CATSEARCH -> ECOSEARCH�� ��ȯ
			bIsEqualSearch = FALSE;
			// ���ô� ������ �˻� ����
			if( sAuthor.GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			m_pMakeSearchData->GetKeyWord(sAuthor);
			if( !bIsEqualSearch ) 
				sAuthor += _T("*");
			
			CString sWhereTitle;

			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				if( fp )
					sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_AUTHOR, '") + sAuthor + _T("') ");
				else
				{
					sBuf.Format( _T(" &ECOSEARCH( , SE_IDX_AUTHOR, '%s' ) "), sAuthor );
					whereList.AddTail( sBuf );
				}
			}
			else
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );

/*
#ifndef _ECOSEARCH_
			sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
#endif
#ifdef _ECOSEARCH_
			sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_AUTHOR, '") + sAuthor + _T("') ");
#endif
			*/
			if( !sWhereTitle.IsEmpty() )
				strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//������
		if ( sPublisher != _T("") ){
			/// 2003�� 9�� 1�� CATSEARCH -> ECOSEARCH�� ��ȯ
			bIsEqualSearch = FALSE;
			// ���ô� ������ �˻� ����
			if( sPublisher.GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			m_pMakeSearchData->GetKeyWord(sPublisher);
			if( !bIsEqualSearch ) 
				sPublisher += _T("*");

			CString sWhereTitle;

			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				if( fp )
					sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_PUBLISHER, '") + sPublisher + _T("') ");
				else
				{
					sBuf.Format( _T(" &ECOSEARCH( , SE_IDX_PUBLISHER, '%s' ) "), sPublisher );
					whereList.AddTail( sBuf );
				}
			}
			else
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_PUBLISHER,'%s*',NULL)>0") , sPublisher );


/*
#ifdef _ECOSEARCH_
			sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_PUBLISHER, '") + sPublisher + _T("') ");
#endif
#ifndef _ECOSEARCH_
			sWhereTitle.Format( _T("CATSEARCH(I.IDX_PUBLISHER,'%s*',NULL)>0") , sPublisher );
#endif
			*/
			if( !sWhereTitle.IsEmpty() )
				strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//����⵵
		CString sPUB_YEAR_START, sPUB_YEAR_END;
		m_pCM->GetControlMgrData( _T("IBS_����⵵2_����"), sPUB_YEAR_START);
		m_pCM->GetControlMgrData( _T("IBS_����⵵2_����"), sPUB_YEAR_END);
		if ( sPUB_YEAR_START == _T("") && sPUB_YEAR_END != _T("") ) {
			AfxMessageBox ( _T("���� ����⵵�� �Է��ϼ���."));
			return;
		}
		else if ( sPUB_YEAR_START != _T("") && sPUB_YEAR_END == _T("") ) {
			strWhere = _T("I.PUB_YEAR = '") + sPUB_YEAR_START  + _T("' AND ") + strWhere;
		}
		else if ( sPUB_YEAR_START != _T("") && sPUB_YEAR_END != _T("") ) {
			strWhere = _T("I.PUB_YEAR >= '") + sPUB_YEAR_START + _T("' AND I.PUB_YEAR <= '") + sPUB_YEAR_END + _T("' AND ") + strWhere;
		}

		//�з���ȣ
		CString sCssDiv , sCssValueStart, sCssValueEnd;
		m_pCM->GetControlMgrData( _T("�з�ǥ����"), sCssDiv);
		m_pCM->GetControlMgrData( _T("�з���ȣ����"), sCssValueStart);
		m_pCM->GetControlMgrData( _T("�з���ȣ����"), sCssValueEnd);
		
		if ( sCssDiv == _T("1") ) {
			if ( sCssValueStart == _T("") && sCssValueEnd != _T("") ) {
				AfxMessageBox( _T("�з���ȣ ������ �Է��ϼ���.") );
				return;
			}
			if ( sCssValueStart != _T("") && sCssValueEnd == _T("") ) {
				strWhere = strWhere + _T(" AND I.KDC_CLASS = '") + sCssValueStart  + _T("'");
			}
			else if ( sCssValueStart != _T("") && sCssValueEnd != _T("") ) {
				strWhere = strWhere + _T(" AND I.KDC_CLASS >= '") + sCssValueStart  + _T("' AND I.KDC_CLASS <= '") + sCssValueEnd + _T("'");
			}			
		}
		else if ( sCssDiv == _T("2") ) {
			if ( sCssValueStart == _T("") && sCssValueEnd != _T("") ) {
				AfxMessageBox( _T("�з���ȣ ������ �Է��ϼ���.") );
				return;
			}
			if ( sCssValueStart != _T("") && sCssValueEnd == _T("") ) {
				strWhere = strWhere + _T(" AND I.DDC_CLASS = '") + sCssValueStart  + _T("'");
			}
			else if ( sCssValueStart != _T("") && sCssValueEnd != _T("") ) {
				strWhere = strWhere + _T(" AND I.DDC_CLASS >= '") + sCssValueStart  + _T("' AND I.DDC_CLASS <= '") + sCssValueEnd + _T("'");
			}	
		}

		INT cnt = whereList.GetCount();
		if( !fp && cnt )
		{
			if( 1 == cnt )
				sBuf.Format( _T(" I.REC_KEY IN %s  "), whereList.GetAt( whereList.FindIndex( 0 ) ) );
			else
			{
				sBuf = _T(" I.REC_KEY IN ( ") +  whereList.GetAt( whereList.FindIndex( 0 ) );
				for( INT i = 1; i < cnt; i++ )
					sBuf = sBuf + _T("  INTERSECT ") +  whereList.GetAt( whereList.FindIndex( i ) );						
				sBuf += _T(" ) ");
			}

			CString strTempAnd = strWhere.Right( 4 );
			if( strTempAnd.Replace(_T("AND"), _T("")) < 1 )
				strWhere += _T(" AND ");

			strWhere += sBuf;
		}

	}//�ڼ��� �˻� ��
	// 2008.06.06 UPDATE BY PDJ : NULL pointer Close error ó��
	if ( fp != NULL ) fclose(fp);
	
	CString sDateSelect;
	CString sDateStart, sDateEnd;
	m_pCM->GetControlMgrData(_T("��¥����"), sDateSelect );
	m_pCM->GetControlMgrData(_T("��¥����"), sDateStart );
	m_pCM->GetControlMgrData(_T("��¥����"), sDateEnd );
	if ( sDateSelect == _T("�Է�����") ) {
		if ( sDateStart != _T("") && sDateEnd == _T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.INPUT_DATE = '") + sDateStart + _T("'");
			}
			else {
				sBookWhere = _T("B.INPUT_DATE = '") + sDateStart + _T("'");
			}
		}
		else if ( sDateStart == _T("") && sDateEnd != _T("") ) {
			AfxMessageBox( _T("�Է����� ������ �Է��ϼ���.") );
			return;
		}
		else if ( sDateStart != _T("") && sDateEnd !=_T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.INPUT_DATE >= '") + sDateStart + _T("' AND B.INPUT_DATE <= '") + sDateEnd + _T("'");
			}
			else {
				sBookWhere = _T("B.INPUT_DATE >= '") + sDateStart + _T("' AND B.INPUT_DATE <= '") + sDateEnd + _T("'");
			}
		}
	}
	else if ( sDateSelect == _T("�������") ) {
		if ( sDateStart != _T("") && sDateEnd == _T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.REG_DATE = '") + sDateStart + _T("'");
			}
			else {
				sBookWhere = _T("B.REG_DATE = '") + sDateStart + _T("'");
			}
		}
		else if ( sDateStart == _T("") && sDateEnd != _T("") ) {
			AfxMessageBox( _T("������� ������ �Է��ϼ���.") );
			return;
		}
		else if ( sDateStart != _T("") && sDateEnd !=_T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.REG_DATE >= '") + sDateStart + _T("' AND B.REG_DATE <= '") + sDateEnd + _T("'");
			}
			else {
				sBookWhere = _T("B.REG_DATE >= '") + sDateStart + _T("' AND B.REG_DATE <= '") + sDateEnd + _T("'");
			}
		}
	}
	else if ( sDateSelect == _T("�谡����") ) {
		if ( sDateStart != _T("") && sDateEnd == _T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.SHELF_DATE = '") + sDateStart + _T("'");
			}
			else {
				sBookWhere = _T("B.SHELF_DATE = '") + sDateStart + _T("'");
			}
		}
		else if ( sDateStart == _T("") && sDateEnd != _T("") ) {
			AfxMessageBox( _T("�谡���� ������ �Է��ϼ���.") );
			return;
		}
		else if ( sDateStart != _T("") && sDateEnd !=_T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.SHELF_DATE >= '") + sDateStart + _T("' AND B.SHELF_DATE <= '") + sDateEnd + _T("'");
			}
			else {
				sBookWhere = _T("B.SHELF_DATE >= '") + sDateStart + _T("' AND B.SHELF_DATE <= '") + sDateEnd + _T("'");
			}
		}
	}
	else if ( sDateSelect == _T("�ΰ�����") ) {
		if ( sDateStart != _T("") && sDateEnd == _T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.LOC_TRANSFER_DATE = '") + sDateStart + _T("'");
			}
			else {
				sBookWhere = _T("B.LOC_TRANSFER_DATE = '") + sDateStart + _T("'");
			}
		}
		else if ( sDateStart == _T("") && sDateEnd != _T("") ) {
			AfxMessageBox( _T("�ΰ����� ������ �Է��ϼ���.") );
			return;
		}
		else if ( sDateStart != _T("") && sDateEnd !=_T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.LOC_TRANSFER_DATE >= '") + sDateStart + _T("' AND B.LOC_TRANSFER_DATE <= '") + sDateEnd + _T("'");
			}
			else {
				sBookWhere = _T("B.LOC_TRANSFER_DATE >= '") + sDateStart + _T("' AND B.LOC_TRANSFER_DATE <= '") + sDateEnd + _T("'");
			}
		}
	}

	// 2006-02-16 UPDATE BY PDW(Jr) ===============================================================
	// ���˻����� å�˻�����
	// ��Ű  -> ����Ű�� ����
	if ( strAccessionQuery != _T("") ) {
		if( !sBookWhere.IsEmpty() ) sBookWhere += _T(" AND ");
//		sBookWhere += _T(" B.SPECIES_KEY IN ( SELECT BB.SPECIES_KEY FROM SE_BOOK_TBL BB WHERE ") + strAccessionQuery + _T(" ) AND ") + strWorkingStatusQuery;
		sBookWhere += _T(" B.ACCESSION_REC_KEY IN ( SELECT BB.ACCESSION_REC_KEY FROM SE_BOOK_TBL BB WHERE ") + strAccessionQuery + _T(" ) AND ") + strWorkingStatusQuery;
	}
	// ============================================================================================

	if (!strSeqNoQuery.IsEmpty())
	{
		if (!sBookWhere.IsEmpty())
			sBookWhere += _T(" AND ");
		sBookWhere += _T(" B.SPECIES_KEY IN ( SELECT BB.SPECIES_KEY FROM SE_BOOK_TBL BB WHERE ") + strSeqNoQuery + _T(" ) ");
	}

	if ( sBookWhere != _T("") ) 
	{
		if( !strWhere.IsEmpty() )
			strWhere += _T(" AND ");
		strWhere += sBookWhere;
	}
	
	
	DWORD START, FINISH;
	START = GetTickCount();
	
	if (m_pDM_Result->RefreshDataManager(strWhere) < 0)	return;

	FINISH = GetTickCount();
	CString log;
	log.Format( _T("[ END ] - ����, �谡���ڱ�� [ Time = %d.%03d sec ]"), ( FINISH - START ) / 1000, ( FINISH - START ) % 1000 ); 
	m_pDM_Result->QueryLog( log.GetBuffer( 0 ) );
	
	

	INT nRowCount = m_pDM_Result->GetRowCount();
	if ( nRowCount < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
	}

	// 2006-02-16 MOVE BY PDW(Jr) =======================================================
	// m_pDM_Result�� �����Ű�� �Լ��� �Ʒ� �����Ѵ�. �Ʒ������� �̵� ��Ŵ
	/*
	CString strMsg;

	strMsg.Format(_T("å�� : %5d"), nRowCount);

	GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));
	*/
	// ==================================================================================
	INT ids;

	//���� ��Ϲ�ȣ ��Ʈ
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( m_pDM_Result , _T("�ڷ�Ǳ���") , _T("�ڷ��") );
	CString sCode;
	for( INT i = 0 ; i < m_pDM_Result->GetRowCount() ; i++ )
	{
		// �ڷ����
		ids = m_pDM_Result->GetCellData( _T("�ڷ����") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbSEARCHCAT") );
		
		CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
		
		ids = m_pDM_Result->SetCellData( _T("�ڷ����") ,  sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnbSEARCHCAT") );
		
		// ���� �ڷ����
		ids = m_pDM_Result->GetCellData( _T("�����ڷ����") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("OnbSEARCHCAT") );
		
		CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
		
		ids = m_pDM_Result->SetCellData( _T("�����ڷ����") ,  sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -4 , _T("OnbSEARCHCAT") );
		
	}
	
	CLocCommonAPI::MakeShelfCode( this , _T("DM_SE_LOC_1100"));
	
	// (A_2004-1014_HAN...)
	// �˻� ������ ������ �ʾҴٰ� ����, �˻����� �ʴ´�.
	if ( !strAccessionNoStart.IsEmpty( ) )
		DeleteWrongAccessionNoRow( m_pDM_Result, _ttoi( strAccessionNoStart ), _ttoi( strAccessionNoEnd ) );

	// 2006-02-16 MOVE BY PDW(Jr) =======================================================
	CString strMsg;

	strMsg.Format(_T("å�� : %5d"), m_pDM_Result->GetRowCount());

	GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));
	// ==================================================================================

	m_pCM_Parent->AllControlDisplay();	
		
	if( m_pDM_Result->GetRowCount() > 0 )
		ShowWindow(SW_HIDE);
	

EFS_END
}


VOID CSE_LOC_1110::OnbFileSearch() 
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_�˻�_��Ϲ�ȣ"));
	if (pDM == NULL) return;

	// San File Load
	INT ids = SanFileAPI::RegNoFileLoad(pDM, this);
	if (ids < 0) return;
	
	
	INT nRowCount = pDM->GetRowCount();
	if (nRowCount <= 0) return;
/*
	CString strRegNo;
	CString strBookWhere = _T("b.REG_NO IN ( ");

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strRegNo = pDM->GetCellData(_T("��Ϲ�ȣ"), idx);
		if (strRegNo.IsEmpty()) continue;

		strBookWhere += _T("'") + strRegNo + _T("'");

		if (idx != nRowCount - 1)
			strBookWhere += _T(" , ");
	}

	strBookWhere += _T(")");


	CString strWhere;
	strWhere.Format(_T("%s"), strBookWhere);

	DWORD START, FINISH;
	START = GetTickCount();
	
	if (m_pDM_Result->RefreshDataManager(strWhere) < 0)	return;
	

	FINISH = GetTickCount();
	CString log;
	log.Format( _T("[ END ] - ����, �谡���ڱ�� [ Time = %d.%03d sec ]"), ( FINISH - START ) / 1000, ( FINISH - START ) % 1000 ); 
	m_pDM_Result->QueryLog( log.GetBuffer( 0 ) );	
*/
	
	/// 50���� �߶󳻼� �˻��Ѵ�.
	CESL_DataMgr TmpDM;
	
	CLocCommonAPI::MakeCopyDM( this , m_pDM_Result , &TmpDM );
	
	INT nMaxUpdateRecordCnt = 50;
	CString sRegNoList;
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		// SELECT�� ��Ϲ�ȣ ����
		CString sRegNo;
		ids = pDM->GetCellData(  i , 0 , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -4 , _T("CBL_BOSHELF::SearchFileBO_LOC_1310()") );
		
		// RECKEY����Ʈ ����
		if( (i!=0) && ((i!=1)&&(i%nMaxUpdateRecordCnt!=1))|| (i==1)) sRegNoList += _T(",");
		sRegNoList += _T("'") +sRegNo + _T("'");
		
		// ���ڵ尡 100���� �Ǿ��ų� �������� ��� SQL���� ������
		if( ( i !=0 && (i%nMaxUpdateRecordCnt == 0)) || (i == (nRowCount-1)) ) 
		{
			CString sWhere = _T(" B.REG_NO IN (") + sRegNoList + _T(")");
			//=====================================================
			//2008.11.13 ADD BY PJW : ���ϰ˻��Ұ�쵵 ���� �޺��ڽ��� ���� ���°��� �����Ѵ�.
			sWhere = GetWorkingStatusSQL( );
			//=====================================================
			ids = CLocCommonAPI::RefreshDataManager( this , &TmpDM , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_VOID2(  -5 , _T("CBL_BOSHELF::OnbFileSearch()") );
			
			ids = CLocCommonAPI::AddDM( &TmpDM , m_pDM_Result , this );
			if( ids < 0 ) ERROR_MSG_RETURN_VOID2(  -6 , _T("CBL_BOSHELF::OnbFileSearch()") );
			
			sRegNoList.Empty();
		}
	}
	
	
	
	nRowCount =  m_pDM_Result->GetRowCount();
	if (nRowCount < 1 ) 
	{
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
	}

	//���� ��Ϲ�ȣ ��Ʈ
	// �ڵ� -> ����
	// �ڷ��
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( m_pDM_Result , _T("�ڷ�Ǳ���") , _T("�ڷ��") );
	
	CString sCode;
	for( i = 0 ; i < m_pDM_Result->GetRowCount() ; i++ )
	{
		// �ڷ����
		ids = m_pDM_Result->GetCellData( _T("�ڷ����") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbSEARCHCAT") );
		
		CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
		
		ids = m_pDM_Result->SetCellData( _T("�ڷ����") ,  sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnbSEARCHCAT") );
		
		// ���� �ڷ����
		ids = m_pDM_Result->GetCellData( _T("�����ڷ����") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("OnbSEARCHCAT") );
		
		CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
		
		ids = m_pDM_Result->SetCellData( _T("�����ڷ����") ,  sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -4 , _T("OnbSEARCHCAT") );
		
	}
	
	// û����ȣ ����
	CLocCommonAPI::MakeShelfCode( this , _T("DM_SE_LOC_1100") );
	
	m_pCM_Parent->AllControlDisplay();	

	EFS_END

}

VOID CSE_LOC_1110::OnbDetail() 
{
	EFS_BEGIN

	m_bDetail = !m_bDetail;
	
	CRect rcClient;
	GetWindowRect(rcClient);

	CString strWindowText;
	if (m_bDetail)
	{
		rcClient.bottom += 200;
		strWindowText = _T("������ <<");
	}
	else
	{
		rcClient.bottom -= 200;
		strWindowText = _T("�ڼ��� <<");
	}

	GetDlgItem(IDC_bDETAIL)->SetWindowText(strWindowText);;
	MoveWindow(rcClient);

	EFS_END
}

BOOL CSE_LOC_1110::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) 
		{
			OnbSEARCHCAT();
			return TRUE;
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;
}

VOID CSE_LOC_1110::OnbCLEARSCREEN() 
{
	EFS_BEGIN

	m_pCM->AllControlClear();	

	EFS_END
}


VOID CSE_LOC_1110::DeleteWrongAccessionNoRow( CESL_DataMgr *pDM, INT from, INT to )
{
	EFS_BEGIN

	INT accessionNo;
	INT cnt = pDM->GetRowCount();

	if( (to < from) && (to == 0) )
		to = from;

	CString sAppendix;
	for( INT i = cnt - 1; i > -1; i-- )
	{
		/// �η������� �׻� �����ֱ�
		pDM->GetCellData( _T("�η�����") , i , sAppendix );

		// 2006-01-18 UPDATE BY PDW(Jr) ===========================================
		// Decode�� �������� ������ 
		/*
		if( sAppendix.Compare(_T("A")) == 0 ) continue;
		*/
		if( sAppendix.Compare(_T("�η�")) == 0 ) continue;
		// ========================================================================

		accessionNo = _ttoi( pDM->GetCellData( _T("���ι�ȣ"), i ) );

		if( accessionNo > to || accessionNo < from )
			pDM->DeleteRow( i );
	}
	EFS_END
}

HBRUSH CSE_LOC_1110::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}