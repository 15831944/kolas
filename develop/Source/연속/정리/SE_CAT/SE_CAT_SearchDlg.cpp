// BO_CAT_SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_SearchDlg.h"

#include "..\..\..\����\����\�弭����\LOC_CommonAPI\SanFileAPI.h"
#include "MainDefaultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_SearchDlg dialog


CSE_CAT_SearchDlg::CSE_CAT_SearchDlg(CESL_Mgr* pParent /*=NULL*/, CString strMenuAlias)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_SearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM_Result = NULL;
	m_pCM_Parent = NULL;

	m_bDetail = TRUE;

	m_strMenuAlias = strMenuAlias;
	m_MakeSearchData = new CMakeSearchData(this);
}

CSE_CAT_SearchDlg::~CSE_CAT_SearchDlg()
{
	if(m_MakeSearchData)
	{
		delete m_MakeSearchData;
		m_MakeSearchData = NULL;
	}
}

BOOL CSE_CAT_SearchDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

VOID CSE_CAT_SearchDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_SearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSE_CAT_SearchDlg, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_SearchDlg)
	ON_BN_CLICKED(IDC_bSEARCH_CAT, OnbSEARCHCAT)
	ON_BN_CLICKED(IDC_bDETAIL, OnbDetail)
	ON_BN_CLICKED(IDC_bCLEAR_SCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_bFileSearch, OnbFileSearch)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CString CSE_CAT_SearchDlg::GetWorkingStatusSQL()
{
	EFS_BEGIN

	CString strResult;

	//=====================================================
	//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 	CString strHoldWorkingStatus = _T("'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O', 'SEL217O', ")
// 								   _T("'SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O', 'SEL317O'");
	CString strHoldWorkingStatus = _T("'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O', 'SEL217O', ")
									// 18.09.27 JMJ KOL.RED.2018.008
								   _T("'SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O', 'SEL317O', 'SEL411O', 'SEL511O', 'SEL611O' ");
	//=====================================================
	//===================================================
	//2009.05.18 UPDATE BY PJW : ��ü�� �˻��� �ڷ���� ���� �߰�
	CString strWholeWorkingStatus = _T("'SEC111N', 'SEC112O', 'SEC211O', 'SEL211O', ")
								    _T("'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O', 'SEL217O', ")
								    _T("'SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O', ")
									// 18.09.27 JMJ KOL.RED.2018.008
									_T("'SEL317O', 'SEL411O', 'SEL511O', 'SEL611O' ");
	//===================================================

	//===================================================
	//2009.05.25 UPDATE BY PJW : �޴� ���̵�� alias��� üũ�Ѵ�.
//	if (m_strMenuAlias == _T("2510") ) // _T("����_����_��Ͽϼ�"))
	if (m_strMenuAlias == _T("2510") || m_strMenuAlias == _T("����_����_��Ͽϼ�"))
	//===================================================	
	{
		CString strWorkingStatus;
		m_pCM->GetControlMgrData(_T("BS_�ڷ����_�ڵ�"), strWorkingStatus);
		
		//===================================================
		//2009.05.25 UPDATE BY PJW : ��ü���� �߰�
//		if (strWorkingStatus == _T("�����ڷ�"))
//		{
//			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strHoldWorkingStatus);
//			static_cast<CMainDefaultDlg*>(pParentMgr)->m_bCatHold = TRUE;
//		}
//		else if(strWorkingStatus == _T("�������ڷ�"))
//		{
//			strResult = _T("b.WORKING_STATUS IN ('SEC111N', 'SEC112O', 'SEC211O', 'SEL211O')");	//050119_�������� �����ΰ��ڷ� ���� �߰�
//		}
		if (strWorkingStatus == _T("�����ڷ�"))
		{
			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strHoldWorkingStatus);
			static_cast<CMainDefaultDlg*>(pParentMgr)->m_bCatHold = TRUE;
		}
		else if(strWorkingStatus == _T("�������ڷ�"))
		{
			strResult = _T("b.WORKING_STATUS IN ('SEC111N', 'SEC112O', 'SEC211O', 'SEL211O')");	//050119_�������� �����ΰ��ڷ� ���� �߰�
		}
		else
		{
			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strWholeWorkingStatus);
		}
		//===================================================
		
	}
	else if (m_strMenuAlias == _T("2520") )   //_T("����_����_�����"))
	{
		//===================================================
		//2009.05.25 UPDATE BY PJW : ��ü���� �߰�
//		CString strWorkingStatus;
//		m_pCM->GetControlMgrData(_T("BS_�ڷ����_�ڵ�"), strWorkingStatus);
//					
//		if (strWorkingStatus == _T("�����ڷ�"))
//			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strHoldWorkingStatus);
//		else
//		{
//			// 2005.10.11 AD BY PDJ 
//			// �����ΰ��ڷ� �����߰�
//			strResult = _T("b.WORKING_STATUS IN ('SEC111N', 'SEC112O','SEC211O', 'SEL211O')"); 
//		}
		CString strWorkingStatus;
		m_pCM->GetControlMgrData(_T("BS_�ڷ����_�ڵ�"), strWorkingStatus);
		if (strWorkingStatus == _T("�����ڷ�"))
		{
			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strHoldWorkingStatus);
			static_cast<CMainDefaultDlg*>(pParentMgr)->m_bCatHold = TRUE;
		}
		else if(strWorkingStatus == _T("�������ڷ�"))
		{
			strResult = _T("b.WORKING_STATUS IN ('SEC111N', 'SEC112O', 'SEC211O', 'SEL211O')");	//050119_�������� �����ΰ��ڷ� ���� �߰�
		}
		else
		{
			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strWholeWorkingStatus);
		}
		//===================================================			
	}
	else if (m_strMenuAlias == _T("����_����_�����ΰ�ó��"))
	{
		strResult = _T("b.WORKING_STATUS = 'SEC112O'");
	}
	else if (m_strMenuAlias == _T("2620") ) //_T("�谡�ϱ��_����"))
	{
		strResult = _T("b.WORKING_STATUS = 'SEL211O'");
	}
	else
		strResult = _T("b.WORKING_STATUS > 'SEC111N'");

	if (strResult.GetLength() > 0) strResult += _T(" AND ");
	strResult += _T("(b.BINDING_BOOK_KEY IS NULL)");

	return strResult;

	EFS_END
	return _T("");
}

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_SearchDlg message handlers

BOOL CSE_CAT_SearchDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	CComboBox *pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_cobWorkingStatus));
	if (!pComboBox) goto ERR;
	
	if (InitESL_Mgr(_T("SM_����_����_�˻�")) < 0)
		goto ERR;

	m_pDM_Result = FindDM(_T("DM_����_����_�˻����"));
	m_pCM_Parent = FindCM(_T("CM_����_����"));
	m_pCM = FindCM(_T("CM_����_����_�˻�"));

	if (m_pDM_Result == NULL ||
		m_pCM_Parent == NULL ||
		m_pCM == NULL) goto ERR;

	if (m_strMenuAlias == _T("2510"))
	{
	}
	else if (m_strMenuAlias == _T("2520"))
	{
	}
	else
	{
		CString strMenuAlias = m_strMenuAlias;
		if (m_strMenuAlias == _T("�谡�ϱ��_����"))
			strMenuAlias = _T("����_�����ΰ�");
		
		INT nFind = strMenuAlias.ReverseFind('_');
		if (nFind >= 0)
		{
			strMenuAlias = strMenuAlias.Mid(nFind + 1);
			strMenuAlias += _T("�ڷ�");
		}
	
		pComboBox->ResetContent();
		pComboBox->InsertString(0, strMenuAlias);
		pComboBox->SetCurSel(0);
		pComboBox->EnableWindow(FALSE);
	}

	OnbDetail();

	//======================================
	//2009.05.25 ADD BY PJW : ����Ʈ�� ��ü�εд�.
	pComboBox->SelectString(0,_T("��ü"));
	//======================================
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	MessageBox(_T("���̾�α� �ʱ�ȭ ����"));
	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

VOID CSE_CAT_SearchDlg::OnbSEARCHCAT() 
{
	EFS_BEGIN

	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	CString sAccessionNoStart, sAccessionNoEnd;
	CString sRegNoStart, sRegNoEnd;
	CString sWORKING_STATUS;
	
	CString sControlNo, sMAT_CODE , sMEDIA_CODE;
	CString sTitle, sKeyWord, sAuthor, sPublisher;
	
	m_pCM->GetControlMgrData(_T("BB_���Ա���_�ڵ�"), sACQUISIT_CODE);
	m_pCM->GetControlMgrData(_T("BS_���Գ⵵"), sACQUISIT_YEAR);
	m_pCM->GetControlMgrData(_T("BB_��Ϲ�ȣ_����"), sRegNoStart);
	m_pCM->GetControlMgrData(_T("BB_��Ϲ�ȣ_����"), sRegNoEnd);

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
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

	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	
	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
		if ( sRegNoStart.GetLength() > nRegCodeLength ) {
			sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength),sRegNoStart.Mid(nRegCodeLength));
		}
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
		if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
			sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength),sRegNoEnd.Mid(nRegCodeLength));
		}
	}
	//=====================================================
	

	m_pCM->GetControlMgrData(_T("BS_�ڷ����_�ڵ�"), sWORKING_STATUS);
	m_pCM->GetControlMgrData(_T("BS_�ڷᱸ��_�ڵ�"), sMAT_CODE);
	m_pCM->GetControlMgrData(_T("BS_��ü����_�ڵ�"), sMEDIA_CODE);
	m_pCM->GetControlMgrData(_T("IBS_��ǥ��"), sTitle);
	m_pCM->GetControlMgrData(_T("IBS_Ű����"), sKeyWord);
	m_pCM->GetControlMgrData(_T("IBS_������"), sAuthor);
	m_pCM->GetControlMgrData(_T("IBS_������"), sPublisher);
	
	CString sWhere = _T("I.REC_KEY = S.REC_KEY");
	
	//�󼼰˻� ��ư üũ
	
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
		strAccessionQuery.Format(_T("b.accession_rec_key = a.rec_key and a.accession_rec_no = %s"), strAccessionNoStart);
	}
	else if (!strAccessionNoStart.IsEmpty() && !strAccessionNoEnd.IsEmpty())
	{
		strAccessionQuery.Format(_T("b.accession_rec_key = a.rec_key and a.accession_rec_no >= %s and a.accession_rec_no <= %s"), strAccessionNoStart, strAccessionNoEnd);
	}

	// 2005.11.29 ADD BY PDJ
	// ���ι�ȣ�� ���� ��� �����ۼ��⵵�� ����.
	//if (!strAccestionNoMakeYear.IsEmpty())
	if (!strAccestionNoMakeYear.IsEmpty() && !( strAccessionNoStart.IsEmpty() && strAccessionNoEnd.IsEmpty() ) )
	{
		if (!strAccessionQuery.IsEmpty())
		{
			strAccessionQuery += _T(" and ");
		}
		else
		{
			strAccessionQuery = _T("b.accession_rec_key = a.rec_key and ");
		}

		strAccessionQuery += _T("a.ACCESSION_REC_MAKE_YEAR = '") + strAccestionNoMakeYear + _T("'");
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
		strSeqNoQuery.Format(_T("b.RECEIPT_NO = %s"), strSeqNoStart);
	}
	else if (!strSeqNoStart.IsEmpty() && !strSeqNoEnd.IsEmpty())
	{
		strSeqNoQuery.Format(_T("b.RECEIPT_NO >= %s and b.RECEIPT_NO <= %s"), strSeqNoStart, strSeqNoEnd);
	}

	//�ڷ����
	CString strWorkingStatusQuery = GetWorkingStatusSQL();
	CString sBookWhere = strWorkingStatusQuery;


	//���Ա���, ���Գ⵵
	sACQUISIT_CODE.Replace ( _T(" ") , NULL );
	if ( sACQUISIT_CODE != _T("") ) {
		sBookWhere = sBookWhere + _T(" AND b.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}
	// 2005.11.28 ADD BY PDJ
	// ���Գ⵵�� ������ȣ�� �������� ���;߸� ����
	if ( sACQUISIT_YEAR != _T("") && !(strSeqNoStart.IsEmpty() && strSeqNoEnd.IsEmpty() )) {
		sBookWhere = sBookWhere + _T(" AND B.ACQ_YEAR = '") + sACQUISIT_YEAR + _T("'");
	}

	//��Ϲ�ȣ�˻�
	CString strWhere = sWhere;

	if (m_bDetail) {
		if ( sRegNoStart != _T("") && sRegNoEnd == _T("")) {			
			if ( sBookWhere != _T("") ) {
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
		//Ű���� �˻�
		if ( sKeyWord != _T("") ){
			CString sWhereTitle;
			//sWhereTitle.Format( _T("CATSEARCH(I.IDX_KEYWORD,'%s*',NULL)>0") , sKeyWord );
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_KEYWORD,'%s*',NULL)>0") , sKeyWord );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {			
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_KEYWORD, '%s*')"), sKeyWord );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
						
			strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//����˻�	
		//sTitle
		if ( sTitle != _T("") ){
			INT iCh = m_MakeSearchData->ChangeHanjaToHangul( sTitle );	//���� �ѱ� ��ȯ
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return;
			}
			iCh = m_MakeSearchData->GetKeyWord( sTitle );				//����, Ư������ ����
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return;
			}
			
			CString sWhereTitle;
			//sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );			
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {			
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_TITLE, '%s*')"), sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
			
			strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//������
		if ( sAuthor != _T("") ){
			CString sWhereTitle;
			//sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );			
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {			
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_AUTHOR, '%s*')"), sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
			
			strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//������
		if ( sPublisher != _T("") ){
			CString sWhereTitle;
			//sWhereTitle.Format( _T("CATSEARCH(I.IDX_PUBLISHER,'%s*',NULL)>0") , sPublisher );			
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_PUBLISHER,'%s*',NULL)>0") , sPublisher );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {			
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_PUBLISHER, '%s*')"), sPublisher );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
			
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
	}//�󼼰˻� �˻� ��

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

	if ( strAccessionQuery != _T("") ) {
		//sBookWhere = strAccessionQuery + _T(" AND ") + sBookWhere + _T(" AND A.REC_KEY = B.ACCESSION_REC_KEY");
		sBookWhere = strAccessionQuery + _T(" AND ") + strWorkingStatusQuery + _T(" AND A.REC_KEY = B.ACCESSION_REC_KEY");
	}

	if (!strSeqNoQuery.IsEmpty())
	{
		if (!sBookWhere.IsEmpty())
			sBookWhere += _T(" AND ");
		sBookWhere += _T(" ") + strSeqNoQuery;
	}

	CString strFrom;
	if ( sBookWhere != _T("") ) 
	{
		strFrom = _T(" se_book_tbl b ");

		if (!strAccessionQuery.IsEmpty())
		{
			strFrom += _T(", co_accession_rec_tbl a ");
		}

		strWhere = strWhere + _T(" AND S.REC_KEY IN (select distinct b.species_key from ") + strFrom + _T(" where ") + sBookWhere + _T(")");
	}

	// å��, ���۵�Ϲ�ȣ�� ���� ���� ������
	CString strSpecialQuery_BookCount;
	strSpecialQuery_BookCount.Format(_T("(SELECT COUNT(b.REC_KEY) FROM %s WHERE %s AND b.SPECIES_KEY = S.REC_KEY)"), strFrom, sBookWhere);

	CString strSpecialQuery_MinRegNo;

	//2018-11-13 KYJ
	//JOB.2018.0017 �ηϰ˻��ÿ��� ��ü�ڷ��� ���۵�Ϲ�ȣ ��ȸ�ϵ��� ���� ����
	//strSpecialQuery_MinRegNo.Format(_T("(SELECT MIN(b.REG_NO) FROM %s WHERE %s AND b.SPECIES_KEY = S.REC_KEY)"), strFrom, sBookWhere);
	//2019.07.23 JOB.2019.0023 �η��� �ƴ��ڷ�� �˻��� �Է��� ��Ϲ�ȣ�� ��ȸ�ǰ� ���������� ���۵�Ϲ�ȣ�� ���������� ����
	//strSpecialQuery_MinRegNo.Format(_T("(SELECT MIN(b.REG_NO) FROM SE_BOOK_TBL B WHERE B.SPECIES_KEY = S.REC_KEY AND B.BOOK_TYPE != 'A' AND B.REG_NO IS NOT NULL) ") );
	strSpecialQuery_MinRegNo.Format(
		_T("NVL((SELECT MIN(b.REG_NO) FROM %s WHERE %s AND b.SPECIES_KEY = S.REC_KEY),(SELECT MIN(b.REG_NO) FROM SE_BOOK_TBL B WHERE B.SPECIES_KEY = S.REC_KEY AND B.BOOK_TYPE != 'A' AND B.REG_NO IS NOT NULL) ) ")
		, strFrom, sBookWhere
		);


	// DM�� ���۷����� �ٲپ�����
	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = m_pDM_Result->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)m_pDM_Result->RefList.GetNext(pos);
		if (!pRef) continue;

		// å��
		if (pRef->FIELD_ALIAS == _T("å��"))
		{
			pRef->FIELD_NAME = strSpecialQuery_BookCount;
		}
		else if (pRef->FIELD_ALIAS == _T("���۵�Ϲ�ȣ"))
		{
			pRef->FIELD_NAME = strSpecialQuery_MinRegNo;
		}
	}

	if (m_pDM_Result->RefreshDataManager(strWhere) < 0)	return;

	INT nRowCount = m_pDM_Result->GetRowCount();
	if ( nRowCount < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
	}


	CString strMsg;

	INT nBookCount = 0;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		nBookCount += _ttoi(m_pDM_Result->GetCellData(_T("å��"), idx));
	}

	strMsg.Format(_T("���� : %5d\tå�� : %5d"), nRowCount, nBookCount);

	GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));

	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
	{
		if (pDM_Where->GetRowCount() <= 0) pDM_Where->InsertRow(-1);

		// ��ü������ ����
		pDM_Where->SetCellData(_T("������"), strWhere, 0);

		// å������ ����
		pDM_Where->SetCellData(_T("å������"), sBookWhere, 0);
	}

	//���� ��Ϲ�ȣ ��Ʈ
	m_pDM_Result->SORT(_T("���۵�Ϲ�ȣ"));

	m_pCM_Parent->AllControlDisplay();	

	if( nRowCount > 0 )
		ShowWindow(SW_HIDE);
	EFS_END
}

VOID CSE_CAT_SearchDlg::OnbDetail() 
{
	EFS_BEGIN

	m_bDetail = !m_bDetail;
	
	CRect rcClient;
	GetWindowRect(rcClient);

	CString strWindowText;
	if (m_bDetail)
	{
		rcClient.bottom += 170;
		strWindowText = _T("�����˻�");
	}
	else
	{
		rcClient.bottom -= 170;
		strWindowText = _T("�󼼰˻�");
	}

	GetDlgItem(IDC_bDETAIL)->SetWindowText(strWindowText);;
	MoveWindow(rcClient);

	EFS_END
}

BOOL CSE_CAT_SearchDlg::PreTranslateMessage(MSG* pMsg) 
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

VOID CSE_CAT_SearchDlg::OnbCLEARSCREEN() 
{
	EFS_BEGIN

	m_pCM->AllControlClear();		

	EFS_END
}

VOID CSE_CAT_SearchDlg::OnbFileSearch() 
{
	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_�˻�_��Ϲ�ȣ"));
	if (pDM == NULL) return;

	// San File Load
	INT ids = SanFileAPI::RegNoFileLoad(pDM, this);
	if (ids < 0) return;
	
	
	INT nRowCount = pDM->GetRowCount();
	if (nRowCount <= 0) return;

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
	strWhere.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN (SELECT SPECIES_KEY FROM SE_BOOK_TBL b WHERE %s)"), strBookWhere);

	if (m_pDM_Result->RefreshDataManager(strWhere) < 0)	return;
	nRowCount =  m_pDM_Result->GetRowCount();
	if (nRowCount < 1 ) 
	{
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
	}

	// �޽��� ǥ��
	CString strMsg;

	INT nBookCount = 0;
	for (idx = 0; idx < nRowCount; idx++)
	{
		nBookCount += _ttoi(m_pDM_Result->GetCellData(_T("å��"), idx));
	}
	
	strMsg.Format(_T("���� : %5d\tå�� : %5d"), nRowCount, nBookCount);

	GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));

	
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
	{
		if (pDM_Where->GetRowCount() <= 0) pDM_Where->InsertRow(-1);

		// ��ü������ ����
		pDM_Where->SetCellData(_T("������"), strWhere, 0);

		// å������ ����
		pDM_Where->SetCellData(_T("å������"), strBookWhere, 0);
	}

	//���� ��Ϲ�ȣ ��Ʈ
	m_pDM_Result->SORT( _T("���۵�Ϲ�ȣ"), TRUE);

	m_pCM_Parent->AllControlDisplay();	
	
	if( nRowCount > 0 )
		DestroyWindow();
}

HBRUSH CSE_CAT_SearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
