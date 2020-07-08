// BO_5163STATICS.cpp : implementation file
//

#include "stdafx.h"

#include "BO_5163STATICS.h"
#include "..\..\����\�弭����\LOC_CommonAPI\LocCommonAPI.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_5163STATICS dialog


CBO_5163STATICS::CBO_5163STATICS(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_5163STATICS)
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pCM = NULL;
}

CBO_5163STATICS::~CBO_5163STATICS()
{
	
}

VOID CBO_5163STATICS::SetStaticsName ( CString strStaticsName )
{
	m_strStaticsName = strStaticsName;
}

VOID CBO_5163STATICS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_5163STATICS)
	DDX_Control(pDX, IDC_cJaRyo, m_JaRyoKind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_5163STATICS, CDialog)
	//{{AFX_MSG_MAP(CBO_5163STATICS)
	ON_CBN_SELCHANGE(IDC_cJaRyo, OnSelchangecJaRyo)
	ON_BN_CLICKED(IDC_cPrice, OncPrice)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_5163STATICS message handlers

VOID CBO_5163STATICS::OnOK() 
{
	BOOL bPriceStatics = FALSE;
	CButton * pPrice = (CButton *)GetDlgItem(IDC_cPrice);
	if ( pPrice->GetCheck() == 1 ) bPriceStatics = TRUE;

	//���� ����
	CString strQuery = MakeQuery();
	if ( strQuery == _T("") ) return;

	//�ڷ���踦 ���� DM ����
	if ( m_strStaticsName == _T("�ڷ����") )
	{
		if ( bPriceStatics ) 
			MakeDM_JaRyo_PriceStatics( strQuery ) ;
		else
			MakeDM_JaRyo( strQuery ) ;
	}
	else if ( m_strStaticsName == _T("����/��û�����") )
	{
		if ( bPriceStatics ) 
			MakeDM_Elec_PriceStatics( strQuery );
		else
			MakeDM_Elec( strQuery );
	}
	else if ( m_strStaticsName == _T("�ڷ��������") )
	{
		MakeDM_Form( strQuery );
	}
		
	ExeSTATICS();
}

VOID CBO_5163STATICS::OnCancel() 
{
	
	CDialog::OnCancel();
}

BOOL CBO_5163STATICS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = InitESL_Mgr( _T("BO_STATICS_5163") );
	if( ids < 0 ) {
		AfxMessageBox ( _T("OnInitDialog Error") );
		return FALSE;
	}

	m_pCM = FindCM ( _T("CM_STATICS_5163") );
	if( m_pCM == NULL ) {
		AfxMessageBox ( _T("OnInitDialog Error") );
		return FALSE;
	}
	
	m_pDM = FindDM ( _T("DM_STATICS_5163") );
	if( m_pDM == NULL ) {
		AfxMessageBox ( _T("OnInitDialog Error") );
		return FALSE;
	}

	CButton * pMono;
	pMono = (CButton *)GetDlgItem(IDC_rMONO);
	pMono->SetCheck(1);

	GetDlgItem(IDC_eStaticsTitle)->SetWindowText(m_strStaticsName);

	//�ڷ���������� ��� �����̳� �񵵼��̳Ĵ� ���߿���
	if ( m_strStaticsName == _T("�ڷ��������") )
	{
		GetDlgItem(IDC_rMONO)->EnableWindow(FALSE);
		GetDlgItem(IDC_rSERIAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_cPrice)->EnableWindow(FALSE);
		GetDlgItem(IDC_eACCESSION_NO_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_eACCESSION_NO_END)->EnableWindow(FALSE);
	}

	if ( m_strStaticsName == _T("�ڷ����") )
	{
		GetDlgItem(IDC_cJaRyo)->EnableWindow(TRUE);
		m_JaRyoKind.ResetContent();
		m_JaRyoKind.InsertString(0,_T("��ü�ڷ����"));
		m_JaRyoKind.InsertString(1,_T("���߰��ڷ����"));
		m_JaRyoKind.InsertString(2,_T("��������Ȳ"));
		m_JaRyoKind.InsertString(3,_T("������๰���(����)"));
		m_JaRyoKind.SetCurSel(0);

	}

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CBO_5163STATICS::MakeQuery()
{
	//��� ������ ���� ���� ����

	const INT nDateCnt = 4;
	CString sDateCtrlAlias[nDateCnt] =
	{
		_T("�����1") , _T("�����2"), _T("���۵�Ϲ�ȣ"), _T("����Ϲ�ȣ")
	};

	CESL_Control_Element *pCE;
	CString sData;
	CString sSearchCond;
	CString strStartRegDate;
	CString strEndRegDate;
	CString sRegNoStart;
	CString sRegNoEnd;


	//���۵����
	pCE = (CESL_Control_Element*)m_pCM->FindControlElement( sDateCtrlAlias[0] );
	strStartRegDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	strStartRegDate.TrimLeft(); strStartRegDate.TrimRight();

	//�������
	pCE = (CESL_Control_Element*)m_pCM->FindControlElement( sDateCtrlAlias[1] );
	strEndRegDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	strEndRegDate.TrimLeft(); strEndRegDate.TrimRight();
	
	if ( strStartRegDate == _T("") || strEndRegDate == _T("") ) 
	{
		AfxMessageBox( _T("������� �Էµ��� �ʾҽ��ϴ�.") );
		return _T("") ;
	}

	CButton * pMONO = (CButton *)GetDlgItem(IDC_rMONO);
	CButton * pSERIAL = (CButton *)GetDlgItem(IDC_rSERIAL);
	if ( pMONO->GetCheck() == 1 ) 
	{
		sSearchCond.Format ( _T("B.REG_DATE >= '%s' AND B.REG_DATE <= '%s' "), strStartRegDate , strEndRegDate );
	
	}
	else if ( pSERIAL->GetCheck() == 1 ) 
	{
		sSearchCond.Format ( _T("B.ACQ_DATE >= '%s' AND B.ACQ_DATE <= '%s' "), strStartRegDate , strEndRegDate );
	}

	
	m_pCM->GetControlMgrData( sDateCtrlAlias[2], sRegNoStart );
	m_pCM->GetControlMgrData( sDateCtrlAlias[3], sRegNoEnd );

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//��Ϲ�ȣ
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
			sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength), sRegNoStart.Mid(nRegCodeLength) );
		}
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
		if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
			sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength), sRegNoEnd.Mid(nRegCodeLength) );
		}
	}

	//=====================================================
	
	CString strRegNoCond;
	if ( !sRegNoStart.IsEmpty() )
	{
		strRegNoCond.Format( _T("AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' "), sRegNoStart , sRegNoEnd );
		sSearchCond = sSearchCond + strRegNoCond;
	}

	return sSearchCond ;
}

BOOL CBO_5163STATICS::ExeSTATICS()
{
	//��� ���� - SiReport �� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	pSIReportManager->DeleteTempPageFiles();

	INT ids;
	if ( m_strStaticsName == _T("�ڷ����") )
		ids = pSIReportManager->GetSIReportFileFromDB(_T("�ڷ����"));
	else if ( m_strStaticsName == _T("����/��û�����") )
		ids = pSIReportManager->GetSIReportFileFromDB(_T("���ڽ�û�����"));
	else if ( m_strStaticsName == _T("�ڷ��������") )
		ids = pSIReportManager->GetSIReportFileFromDB(_T("�ڷ��������"));
	
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 1;
	}

	//SIREPORT ���� 
	CString sTmpSearch = MakeQuery();
	CString strStaticsTitle;
	GetDlgItem(IDC_eStaticsTitle)->GetWindowText(strStaticsTitle);

	pSIReportManager->SetTempVariable(_T("�������"), strStaticsTitle);

	CESL_Control_Element *pCE;
	CString sData;
	CString sSearchCond;
	CString strStartRegDate;
	CString strEndRegDate;

	pCE = (CESL_Control_Element*)m_pCM->FindControlElement( _T("�����1") );
	strStartRegDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	strStartRegDate.TrimLeft(); strStartRegDate.TrimRight();

	//�������
	pCE = (CESL_Control_Element*)m_pCM->FindControlElement( _T("�����2") );
	strEndRegDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	strEndRegDate.TrimLeft(); strEndRegDate.TrimRight();

	sTmpSearch.Format( _T("������� %s ���� %s ����"), strStartRegDate, strEndRegDate );

	CString sRegNoStart, sRegNoEnd;
	m_pCM->GetControlMgrData( _T("���۵�Ϲ�ȣ"), sRegNoStart );
	m_pCM->GetControlMgrData( _T("����Ϲ�ȣ"), sRegNoEnd );

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//��Ϲ�ȣ
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
			sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength), sRegNoStart.Mid(nRegCodeLength) );
		}
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
		if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
			sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength), sRegNoEnd.Mid(nRegCodeLength) );			
		}
	}
	//=====================================================
	
	if ( sRegNoStart != _T("") )
	{
		sTmpSearch.Format(_T("%s , ��Ϲ�ȣ %s ���� %s ���� "), sTmpSearch, sRegNoStart, sRegNoEnd );
	}


	pSIReportManager->SetTempVariable(_T("�˻�����"), sTmpSearch);


	CButton * pPrice = (CButton *)GetDlgItem(IDC_cPrice);
	
	if ( pPrice->GetCheck() == 1 ) 
		pSIReportManager->SetTempVariable(_T("����"), _T(" \ "));
	else if ( pPrice->GetCheck() == 0 ) 
		pSIReportManager->SetTempVariable(_T("����"), _T(" ��(å) "));


	CESL_DataMgr *pDM_Dst;

	if ( m_strStaticsName == _T("�ڷ����") )
		pDM_Dst = FindDM ( _T("DM_�ڷ����") );	
	else if ( m_strStaticsName == _T("����/��û�����") )
		pDM_Dst = FindDM ( _T("DM_���ڽ�û�����") );
	else if ( m_strStaticsName == _T("�ڷ��������") )
		pDM_Dst = FindDM ( _T("DM_�ڷ��������") );
	

	
	if ( pDM_Dst == NULL ) 
		return FALSE;
	
	pSIReportManager->SetDataMgr(0, pDM_Dst );
	pSIReportManager->MAKEPAGE_DrawWorkForm(-1);
	Print_SetLastPageLine();

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
		}	


	return TRUE;
}

INT CBO_5163STATICS::SIReportEventHandler( CSIReportEventInfo *EventInfo )
{
	CSIReportManager *pSIReportManager = ( CSIReportManager * )EventInfo->m_pSIReportManager;
	if( !pSIReportManager )
		return -1;
	else
		return 0;
}


BOOL CBO_5163STATICS::MakeDM_JaRyo(CString strQuery)
{
	//�ڷ����
	//��� ������ ���� DataMgr ����
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO ( m_pDM->CONNECTION_INFO );

	CESL_DataMgr *pDM_Dst;
	pDM_Dst = FindDM ( _T("DM_�ڷ����") );
	if ( pDM_Dst == NULL ) 
		return FALSE;

	pDM_Dst->FreeData();

	CString strTBLNAME;

	CButton * pMONO = (CButton *)GetDlgItem(IDC_rMONO);
	CButton * pSERIAL = (CButton *)GetDlgItem(IDC_rSERIAL);
	if ( pMONO->GetCheck() == 1 ) 
	{
		strTBLNAME = _T("IDX_BO_TBL I, BO_BOOK_TBL B");

		// 2006-01-17 UPDATE BY PDW(Jr) ======================================================
		// �����ڷ� ���� �߰� 
		// ������ ��� ���¿� ���� ���� �߰�
		/*
		strQuery += _T(" AND substr(B.WORKING_STATUS, 1, 3) NOT IN ('BOT', 'BOA') ")
					   _T(" AND B.WORKING_STATUS NOT IN ('BOL252N') ");
		*/
		strQuery += _T(" AND substr(B.WORKING_STATUS, 1, 3) NOT IN ('BOT', 'BOA') ")
					   _T(" AND B.WORKING_STATUS NOT IN ('BOL252N', 'BOL115O') ");
		// ===================================================================================
	}
	else if ( pSERIAL->GetCheck() == 1 ) 
	{
		strTBLNAME = _T("IDX_SE_TBL I, SE_BOOK_TBL B");
	}
	else 
	{
		AfxMessageBox(_T("��� ������ ������ �ּ���"));
		return FALSE;
	}

	//��� ������ ���� ���� ����
	switch ( m_JaRyoKind.GetCurSel() )
	{
	case 0:
		//��ü�ڷ����
		strQuery = strQuery;
		break;
	case 1:
		//���߰��ڷ����
		//strQuery = strQuery + _T(" AND SEPARATE_SHELF_CODE = 'HI' ") ;
		strQuery = strQuery + _T(" AND substr(B.REG_NO, 0, 2) = 'H0' ") ;
		break;
	case 2:
		//��������Ȳ
		strQuery = strQuery + _T(" AND MAT_CODE IN ('DM', 'DP') ");
		break;
	case 3:
		//������๰���(����)
		strQuery = strQuery + _T(" AND GOV_CODE = 'kn' ");
		break;

	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� - ����
	CString strStaticsQuery;
	strStaticsQuery.Format( 
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO,  ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY    ")
			_T("AND ( TRIM(i.PUBLISH_COUNTRY_CODE) = 'ulk' OR TRIM(i.PUBLISH_COUNTRY_CODE) = 'ko' ) ")
			_T("AND %s AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL,  ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24  ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE")
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� - ����
	strStaticsQuery.Format( 
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO,  ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY    ")
			_T("AND ( TRIM(i.PUBLISH_COUNTRY_CODE) != 'ulk' AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'ko' ) ")
			_T("AND %s AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'kn' AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL,  ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24  ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� - ����
	strStaticsQuery.Format( 
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO,  ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY    ")
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) = 'kn' AND %s AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL,  ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24  ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� - ��Ÿ
	strStaticsQuery.Format( 
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO,  ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY    ")
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) IS NULL AND %s AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL,  ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24  ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ����
	strStaticsQuery.Format(  
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO,  ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY    ")
			_T("AND ( TRIM(i.PUBLISH_COUNTRY_CODE) = 'ulk' OR TRIM(i.PUBLISH_COUNTRY_CODE) = 'ko' ) ")
			_T("AND %s AND substr(B.REG_NO,0,2) IN ( 'N0', 'EA', 'EB' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL,  ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24  ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );	
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ����
	strStaticsQuery.Format( 
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO,  ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY    ")
			_T("AND ( TRIM(i.PUBLISH_COUNTRY_CODE) != 'ulk' AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'ko' ) ")
			_T("AND %s AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'kn' AND substr(B.REG_NO,0,2) IN ( 'N0', 'EA', 'EB' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL,  ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24  ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ����
	strStaticsQuery.Format( 
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO,  ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY    ")
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) = 'kn' AND %s AND substr(B.REG_NO,0,2) IN ( 'N0', 'EA', 'EB' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL,  ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24  ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ��Ÿ
	strStaticsQuery.Format( 
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO,  ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY    ")
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) IS NULL AND %s AND substr(B.REG_NO,0,2) IN ( 'N0', 'EA', 'EB' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL,  ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24  ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�հ踦 ���� DM ����
	SUMPriceDM ( pDM_Dst ) ;

	return TRUE;
}

BOOL CBO_5163STATICS::MakeDM_Elec ( CString strQuery )
{
	//����.��û�� ���
	//050412_���� ����, ����å �߰�
	//��� ������ ���� DataMgr ����
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO ( m_pDM->CONNECTION_INFO );

	CESL_DataMgr *pDM_Dst;
	pDM_Dst = FindDM ( _T("DM_���ڽ�û�����") );
	if ( pDM_Dst == NULL ) 
		return FALSE;

	pDM_Dst->FreeData();

	CString strTBLNAME;

	CButton * pMONO = (CButton *)GetDlgItem(IDC_rMONO);
	CButton * pSERIAL = (CButton *)GetDlgItem(IDC_rSERIAL);
	if ( pMONO->GetCheck() == 1 ) 
	{
		strTBLNAME = _T("IDX_BO_TBL I, BO_BOOK_TBL B");

		// 2006-01-17 UPDATE BY PDW(Jr) ======================================================
		// �����ڷ� ���� �߰� 
		// ������ ��� ���¿� ���� ���� �߰�
		/*
		strQuery += _T(" AND substr(B.WORKING_STATUS, 1, 3) NOT IN ('BOT', 'BOA') ")
					   _T(" AND B.WORKING_STATUS NOT IN ('BOL252N') ");
		*/
		strQuery += _T(" AND substr(B.WORKING_STATUS, 1, 3) NOT IN ('BOT', 'BOA') ")
					   _T(" AND B.WORKING_STATUS NOT IN ('BOL252N', 'BOL115O') ");
		// ===================================================================================
	}
	else if ( pSERIAL->GetCheck() == 1 ) 
	{
		strTBLNAME = _T("IDX_SE_TBL I, SE_BOOK_TBL B");
	}
	else 
	{
		AfxMessageBox(_T("��� ������ ������ �ּ���"));
		return FALSE;
	}

	CString strStaticsQuery;
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� - ����
	CString strStaticsQuery;
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','VIDEO', 'DVDC', 'VIDEO','VTC','VIDEO','CTC','VIDEO','CDRL','CD','DVDL','CD', 'VTL','CD', 'CTL','CD', 'VCDI','DVD', 'DVDI','DVD', 'VTI','DVD', 'CDR','��Ÿ', 'DVD','��Ÿ', 'VT','��Ÿ', 'CT','��Ÿ','��Ÿ'), ")
			_T("COUNT(DISTINCT I.REC_KEY), COUNT(DISTINCT B.REC_KEY) ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) = 'ulk' AND %s AND B.SEPARATE_SHELF_CODE is not null AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) Group By decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','VIDEO', 'DVDC', 'VIDEO','VTC','VIDEO','CTC','VIDEO','CDRL','CD','DVDL','CD', 'VTL','CD', 'CTL','CD', 'VCDI','DVD', 'DVDI','DVD', 'VTI','DVD', 'CDR','��Ÿ', 'DVD','��Ÿ', 'VT','��Ÿ', 'CT','��Ÿ','��Ÿ') ")
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� - ����
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','VIDEO', 'DVDC', 'VIDEO','VTC','VIDEO','CTC','VIDEO','CDRL','CD','DVDL','CD', 'VTL','CD', 'CTL','CD', 'VCDI','DVD', 'DVDI','DVD', 'VTI','DVD', 'CDR','��Ÿ', 'DVD','��Ÿ', 'VT','��Ÿ', 'CT','��Ÿ','��Ÿ'), ")
			_T("COUNT(DISTINCT I.REC_KEY), COUNT(DISTINCT B.REC_KEY) ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'ulk' AND %s AND B.SEPARATE_SHELF_CODE is not null AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'kn' AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) Group By decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','VIDEO', 'DVDC', 'VIDEO','VTC','VIDEO','CTC','VIDEO','CDRL','CD','DVDL','CD', 'VTL','CD', 'CTL','CD', 'VCDI','DVD', 'DVDI','DVD', 'VTI','DVD', 'CDR','��Ÿ', 'DVD','��Ÿ', 'VT','��Ÿ', 'CT','��Ÿ','��Ÿ')")
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� - ����
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','VIDEO', 'DVDC', 'VIDEO','VTC','VIDEO','CTC','VIDEO','CDRL','CD','DVDL','CD', 'VTL','CD', 'CTL','CD', 'VCDI','DVD', 'DVDI','DVD', 'VTI','DVD', 'CDR','��Ÿ', 'DVD','��Ÿ', 'VT','��Ÿ', 'CT','��Ÿ','��Ÿ'), ")
			_T("COUNT(DISTINCT I.REC_KEY) , COUNT(DISTINCT B.REC_KEY) ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) = 'kn' AND %s AND B.SEPARATE_SHELF_CODE is not null AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) Group By decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','VIDEO', 'DVDC', 'VIDEO','VTC','VIDEO','CTC','VIDEO','CDRL','CD','DVDL','CD', 'VTL','CD', 'CTL','CD', 'VCDI','DVD', 'DVDI','DVD', 'VTI','DVD', 'CDR','��Ÿ', 'DVD','��Ÿ', 'VT','��Ÿ', 'CT','��Ÿ','��Ÿ')") 
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� - ��Ÿ
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T("COUNT(DISTINCT I.REC_KEY) , COUNT(DISTINCT B.REC_KEY) ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) IS NULL AND %s AND B.SEPARATE_SHELF_CODE is not null AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) Group By decode(B.SEPARATE_SHELF_CODE, '2','0', 'CDRC','1', 'DVDC', '1','VTC','1','CTC','1','CDRL','2','DVDL','2', 'VTL','2', 'CTL','2', 'VCDI','3', 'DVDI','3', 'VTI','3', 'CDR','4', 'DVD','4', 'VT','4', 'CT','4','4')") 
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ����
	//strStaticsQuery.Format(  _T("SELECT decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','VIDEO', 'DVDC', 'VIDEO','VTC','VIDEO','CTC','VIDEO','CDRL','CD','DVDL','CD', 'VTL','CD', 'CTL','CD', 'VCDI','DVD', 'DVDI','DVD', 'VTI','DVD', 'CDR','��Ÿ', 'DVD','��Ÿ', 'VT','��Ÿ', 'CT','��Ÿ','��Ÿ'), ")
	strStaticsQuery.Format(  _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T("COUNT(DISTINCT I.REC_KEY) , COUNT(DISTINCT B.REC_KEY) ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			_T("AND ( TRIM(i.PUBLISH_COUNTRY_CODE) = 'ulk' OR TRIM(i.PUBLISH_COUNTRY_CODE) = 'ko' ) ")
			_T("AND %s AND B.SEPARATE_SHELF_CODE is not null AND substr(B.REG_NO,0,2) IN ( 'N0' ) ")
			_T("Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4')")
			//_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) = 'ulk' AND %s AND B.SEPARATE_SHELF_CODE is not null AND substr(B.REG_NO,0,2) IN ( 'N0', 'EA', 'EB' ) Group By decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','VIDEO', 'DVDC', 'VIDEO','VTC','VIDEO','CTC','VIDEO','CDRL','CD','DVDL','CD', 'VTL','CD', 'CTL','CD', 'VCDI','DVD', 'DVDI','DVD', 'VTI','DVD', 'CDR','��Ÿ', 'DVD','��Ÿ', 'VT','��Ÿ', 'CT','��Ÿ','��Ÿ')")
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );	
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ����
	//strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','VIDEO', 'DVDC', 'VIDEO','VTC','VIDEO','CTC','VIDEO','CDRL','CD','DVDL','CD', 'VTL','CD', 'CTL','CD', 'VCDI','DVD', 'DVDI','DVD', 'VTI','DVD', 'CDR','��Ÿ', 'DVD','��Ÿ', 'VT','��Ÿ', 'CT','��Ÿ','��Ÿ'), ")
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T("COUNT(DISTINCT I.REC_KEY) , COUNT(DISTINCT B.REC_KEY) ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			_T("AND ( TRIM(i.PUBLISH_COUNTRY_CODE) != 'ulk' AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'ko' )")
			_T("AND %s AND B.SEPARATE_SHELF_CODE is not null AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'kn' AND substr(B.REG_NO,0,2) IN ( 'N0' ) Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4')")
//			_T("AND i.PUBLISH_COUNTRY_CODE != 'ulk' AND %s AND B.SEPARATE_SHELF_CODE is not null AND i.PUBLISH_COUNTRY_CODE != 'kn' AND substr(B.REG_NO,0,2) IN ( 'N0', 'EA', 'EB' ) Group By decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','VIDEO', 'DVDC', 'VIDEO','VTC','VIDEO','CTC','VIDEO','CDRL','CD','DVDL','CD', 'VTL','CD', 'CTL','CD', 'VCDI','DVD', 'DVDI','DVD', 'VTI','DVD', 'CDR','��Ÿ', 'DVD','��Ÿ', 'VT','��Ÿ', 'CT','��Ÿ','��Ÿ')")
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ����
	//strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','VIDEO', 'DVDC', 'VIDEO','VTC','VIDEO','CTC','VIDEO','CDRL','CD','DVDL','CD', 'VTL','CD', 'CTL','CD', 'VCDI','DVD', 'DVDI','DVD', 'VTI','DVD', 'CDR','��Ÿ', 'DVD','��Ÿ', 'VT','��Ÿ', 'CT','��Ÿ','��Ÿ'), ")
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T("COUNT(DISTINCT I.REC_KEY) , COUNT(DISTINCT B.REC_KEY) ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) = 'kn' AND %s AND B.SEPARATE_SHELF_CODE is not null AND substr(B.REG_NO,0,2) IN ( 'N0' ) Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4')") 
			//_T("AND i.PUBLISH_COUNTRY_CODE = 'kn' AND %s AND B.SEPARATE_SHELF_CODE is not null AND substr(B.REG_NO,0,2) IN ( 'N0', 'EA', 'EB' ) Group By decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','VIDEO', 'DVDC', 'VIDEO','VTC','VIDEO','CTC','VIDEO','CDRL','CD','DVDL','CD', 'VTL','CD', 'CTL','CD', 'VCDI','DVD', 'DVDI','DVD', 'VTI','DVD', 'CDR','��Ÿ', 'DVD','��Ÿ', 'VT','��Ÿ', 'CT','��Ÿ','��Ÿ')") 
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ��Ÿ
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T("COUNT(DISTINCT I.REC_KEY) , COUNT(DISTINCT B.REC_KEY) ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) IS NULL AND %s AND B.SEPARATE_SHELF_CODE is not null AND substr(B.REG_NO,0,2) IN ( 'N0' ) Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4')") 
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//����å
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T("COUNT(DISTINCT I.REC_KEY) , COUNT(DISTINCT B.REC_KEY) ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")	//AND TRIM(i.PUBLISH_COUNTRY_CODE) IS NULL 
			_T("AND %s AND B.SEPARATE_SHELF_CODE is not null AND substr(B.REG_NO,0,2) IN ( 'C0', 'EA', 'EB' ) Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4')") 
			, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//�հ踦 ���� DM ����
	SUMPriceDM ( pDM_Dst ) ;

	INT nCnt = pDM_Dst->GetRowCount();

	return TRUE;
}


INT CBO_5163STATICS::CopyDM( CESL_DataMgr *pSrcDM , CESL_DataMgr *pDstDM )
{
	EFS_BEGIN



	INT ids;
	INT nAliasCnt;
	nAliasCnt = pDstDM->RefList.GetCount();


	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];
	
	ids = pDstDM->GetAllAlias( sColAlias , nAliasCnt );
	if( pDstDM == NULL ) 
	{
		AfxMessageBox ( _T("CopyDM Error 1") ) ;
		return -1;
	}

	CString sIndex;
	CString sResult1;
	CString sResult2;
	
	INT nidx = 1;
	pDstDM->InsertRow(-1);
	CString strSrcSpeBook;
	
	// ���� 0���� SET
	for ( INT i = 0; i < nAliasCnt ; i++ )
	{
		strSrcSpeBook = _T("0\r\n(0)");

		ids = pDstDM->SetCellData( pDstDM->GetRowCount()-1 , i, strSrcSpeBook );
	}
	
	// ���� �ִ°͸� SET
	nAliasCnt = pSrcDM->GetRowCount();
	
	for(  i = 0 ; i < nAliasCnt ; i++ )
	{
		ids = pSrcDM->GetCellData( i , 0 , sIndex );
		if( ids < 0 && (ids != -4012) ) 
		{
			AfxMessageBox ( _T("CopyDM Error 2") ) ;
			return -1;
		}
	
		// 2006-01-17 UPDATE BY PDW(Jr) =========================================
		// 'z'�� ��������̴� 
		// Index�� 1 �� �ٿ����Ѵ�.
		// index���� ������ ��������� ���̰�
		// ��������� �հ� �ٷ� ���� ���̴�.(���� ������-_-)
		/* ������
		if ( sIndex.IsEmpty() ) 
		{
			INT nCnt = pDstDM->RefList.GetCount() - 2;
			sIndex.Format( _T("%d"), nCnt );
		}
		*/
		if ( sIndex.IsEmpty() || sIndex == 'z' ) 
		{
			INT nCnt = pDstDM->RefList.GetCount() - 3;
			sIndex.Format( _T("%d"), nCnt );
		}
		// =====================================================================

		ids = pSrcDM->GetCellData( i , 1 , sResult1 );
		if( ids < 0 && (ids != -4012) ) 
		{
			AfxMessageBox ( _T("CopyDM Error 3") ) ;
			return -1;
		}
		ids = pSrcDM->GetCellData( i , 2 , sResult2 );

		if ( sResult1 == _T("") ) sResult1 = _T("0") ;
		if ( sResult2 == _T("") ) sResult2 = _T("0") ;

		strSrcSpeBook = sResult1 + _T("\r\n(")  + sResult2 + _T(")");

		if( ids == -4012 ) strSrcSpeBook.Empty();

		
		//ids = pDstDM->SetCellData( pDstDM->GetRowCount()-1 , i, strSrcSpeBook );
		ids = pDstDM->SetCellData( sIndex, strSrcSpeBook, pDstDM->GetRowCount()-1 );
		if( ids < 0 ) 
		{
			AfxMessageBox ( _T("CopyDM Error 4") ) ;
			return -1;
		}
	}
		

	delete []sColAlias;

	return 0;

	EFS_END
	return -1;

}

BOOL CBO_5163STATICS::SUMPriceDM ( CESL_DataMgr *pDM )
{
	//�հ踦 ���Ѵ�.. ������.. ���� ��.. -_-;;;
	//�ϴ� ������..
	CString strTmp;
	CString strTmp1;
	INT nSUMPrice = 0 ;
	INT nSUMPrice1 = 0 ;
	INT nPrice = 0 ;
	INT nPrice1 = 0 ;

	CString sColIdx;
	
	for( INT nRowIdx = 0 ; nRowIdx < pDM->GetRowCount() ; nRowIdx++ )
	{	//ROW ����
		for ( INT nColIdx = 0; nColIdx < pDM->RefList.GetCount()-1 ; nColIdx++ )
		{//COL ���
			sColIdx.Format(_T("%d"), nColIdx);
			strTmp = pDM->GetCellData( sColIdx, nRowIdx );
			//�Ľ��ؼ� �������Ѵ�.
			//������ 
			strTmp.Replace(_T("\r\n"), _T(""));
			nPrice = _ttoi(strTmp);
			nSUMPrice += nPrice;
			//å���� () ���� ����
			nPrice1 = _ttoi(strTmp.Mid ( strTmp.Find(_T("("))+1 , strTmp.Find(_T(")")) - strTmp.Find(_T("(")) ));
			nSUMPrice1 += nPrice1;
		}

		strTmp.Format( _T("%d"), nSUMPrice );
		strTmp1.Format( _T("%d"), nSUMPrice1 );
		strTmp = strTmp + _T("\r\n(") + strTmp1 + _T(")") ;

		//pDM->SetCellData( nRowIdx , pDM->RefList.GetCount()-2 , strTmp );
		sColIdx.Format(_T("%d"), pDM->RefList.GetCount()-2);
		pDM->SetCellData( sColIdx, strTmp, nRowIdx );
		nSUMPrice = 0;
		nSUMPrice1 = 0;
	}

	nSUMPrice = 0;
	nSUMPrice1 = 0;
	nPrice = 0;
	nPrice1 = 0;

	//������ InsertRow(-1)�� �ϰ� �����ؾ߰���?
	pDM->InsertRow(-1);
	for ( INT nColIdx = 0; nColIdx < pDM->RefList.GetCount(); nColIdx++ )
	{
		for( INT nRowIdx = 0 ; nRowIdx < pDM->GetRowCount() ; nRowIdx++ )
		{
			strTmp = pDM->GetCellData( nRowIdx, nColIdx );
			//�Ľ��ؼ� �������Ѵ�.
			//������ 
			strTmp.Replace(_T("\r\n"), _T(""));
			nPrice = _ttoi(strTmp);
			nSUMPrice += nPrice;
			//å���� () ���� ����
			nPrice1 = _ttoi(strTmp.Mid ( strTmp.Find(_T("("))+1 , strTmp.Find(_T(")")) - strTmp.Find(_T("(")) ));
			nSUMPrice1 += nPrice1;
		}
		strTmp.Format( _T("%d"), nSUMPrice );
		strTmp1.Format( _T("%d"), nSUMPrice1 );
		strTmp = strTmp + _T("\r\n(") + strTmp1 + _T(")") ;

		if ( strTmp == _T("") ) strTmp = _T("0");
		pDM->SetCellData( pDM->GetRowCount()-1 , nColIdx , strTmp );
		nSUMPrice = 0;
		nSUMPrice1 = 0;
	}
	


	// ������ ��¼��..?? �׳��� ���� / �հ� x 100
	// �̰� �̵� �ؾ���.. ���� �Ф�;
	// ���� ����
	INT nPercent = 0;
	float nSpeTotal = 0.0;
	float nBookTotal = 0.0;
	INT nTMp = pDM->RefList.GetCount();
	INT nTMp1 = pDM->GetRowCount();

	//����??
	//CString strTotalCount = pDM->GetCellData( pDM->GetRowCount()-1 , pDM->RefList.GetCount()-2 );
	sColIdx.Format( _T("%d"), pDM->RefList.GetCount()-2 );
	CString strTotalCount = pDM->GetCellData( sColIdx, pDM->GetRowCount()-1  );
	strTotalCount.Replace(_T("\r\n"), _T(""));

	for( nRowIdx = 0 ; nRowIdx < pDM->GetRowCount() ; nRowIdx++ )
	{
		//ROW ���� , �հ踦 ������
		sColIdx.Format( _T("%d"), pDM->RefList.GetCount()-2 );
		strTmp = pDM->GetCellData( sColIdx, nRowIdx  );
		
		strTmp.Replace(_T("\r\n"), _T(""));
		if ( _ttoi(strTotalCount) !=  0 )
			nSpeTotal = (float)_ttoi(strTmp) / (float)_ttoi(strTotalCount) * 100;
		else 
			nSpeTotal = 0;

		if ( _ttoi(strTotalCount.Mid ( strTotalCount.Find(_T("("))+1 , strTotalCount.Find(_T(")")) - strTotalCount.Find(_T("(")) )) != 0 )
			nBookTotal = (float)_ttoi(strTmp.Mid ( strTmp.Find(_T("("))+1 , strTmp.Find(_T(")")) - strTmp.Find(_T("(")) )) / (float)_ttoi(strTotalCount.Mid ( strTotalCount.Find(_T("("))+1 , strTotalCount.Find(_T(")")) - strTotalCount.Find(_T("(")) )) * 100 ;
		else 
			nBookTotal = 0;

		
		strTmp.Format( _T("%2.1f"), nSpeTotal );
		strTmp1.Format( _T("%2.1f"), nBookTotal );
		strTmp = strTmp + _T("\r\n(") + strTmp1 + _T(")") ;

		sColIdx.Format( _T("%d"), pDM->RefList.GetCount()-1 );
		pDM->SetCellData( sColIdx, strTmp, nRowIdx  );
	}

	//���鼭 3�ڸ����� , ����ش�.
	for ( nColIdx = 0; nColIdx < pDM->RefList.GetCount() -1; nColIdx++ )
	{
		for( nRowIdx = 0 ; nRowIdx < pDM->GetRowCount() ; nRowIdx++ )
		{
			sColIdx.Format( _T("%d"), nColIdx );
			strTmp = pDM->GetCellData( sColIdx, nRowIdx);
			
			//�Ҽ��� ����ְ�..
			INT nTmpData;
			CString strTmpSpeData;
			CString strTmpBookData;
			nTmpData = _ttoi( strTmp );
			if ( nTmpData != 0 ) 
			{
				strTmpSpeData.Format( _T("%d") , nTmpData );
				ConversionPriceInfoToDesc( strTmpSpeData, strTmpSpeData ) ;

				strTmpBookData = strTmp.Mid ( strTmp.Find(_T("("))+1 , strTmp.Find(_T(")")) - strTmp.Find(_T("("))-1 );
				ConversionPriceInfoToDesc( strTmpBookData, strTmpBookData ) ;

				strTmp = strTmpSpeData + _T("\r\n(") + strTmpBookData + _T(")");
			}

			pDM->SetCellData( sColIdx, strTmp, nRowIdx );
		}
	}
	
	//������ �����ϱ� �����ٰ� �����̽� �ϳ� �߰�..
	for ( nColIdx = 0; nColIdx < pDM->RefList.GetCount(); nColIdx++ )
	{
		for( nRowIdx = 0 ; nRowIdx < pDM->GetRowCount() ; nRowIdx++ )
		{
			strTmp = pDM->GetCellData( nRowIdx, nColIdx );
			strTmp.Replace(_T("\r\n"), _T(" \r\n"));
			pDM->SetCellData( nRowIdx, nColIdx , strTmp );
		}
	}

	

	return TRUE;
}

VOID CBO_5163STATICS::Print_SetLastPageLine(VOID)
{
EFS_BEGIN

	INT ids;

	// XML �� WorkForm��ü�� �����Ѵ�.
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	
	if (pXMLIOMgr == NULL) {
		AfxMessageBox(_T("XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}
	if (pPageWorkForm == NULL) {
		AfxMessageBox(_T("�߰� XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}

	// ���� ������ ������ ��ȣ�� �����´�.
	INT nMaxTmpPageNum = pXMLIOMgr->GetMaxTmpPageNum();
	if (nMaxTmpPageNum <= 0) {
		AfxMessageBox(_T("������ ������ ã�� �� �����ϴ�."));
		return;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// ���� ������ ������ ������ �ҷ��´�.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) {
		AfxMessageBox(_T("�������� �ε� ���� ���Ͽ����ϴ�."));
		return;
	}
	// �ҷ��� WorkForm�߿��� First Sheet�� �����´�.(�߰� ������ ���Ͽ��� Sheet�� �Ѱ����̴�.)
	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) {
		AfxMessageBox(_T("�������� ã�� �� �����ϴ�."));
		return;
	}
	// ������ ������ ó���� �̹� �Ǿ� �ִ��� ����. �̹� ó���Ǿ� �ִٸ� �ι� �� �ʿ� ����.
	CSIReportObject * pObject = pSheet->FindObject(_T("�߰��ȸ�����������"));
	if (pObject) return;
	// �ܺ����̺� �� �ݺ� ���̺��� �����´�.(���̺� �̸��� ��Ȳ�� ���� ������ ��)
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_7"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_37"));
	if (pIn == NULL) return;
	if (pOut == NULL) return;

	CRect InLeft, InRight, OutLeft, OutRight, NewRect;

	CSIReportObject_Table_Cell *pCell;

	// �ݺ����̺��� ������ ������ ���� ó���� ������ ���������� Rect�� ���ϰ�
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, 0);
	if (pCell) InLeft = pCell->GetRect();
	
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, pIn->col_count-1);
	if (pCell) InRight = pCell->GetRect();

	// �ܺ����̺��� ������ ������ ���� ó�� ���� Rect�� ���Ѵ�.
	pCell = NULL;
	pCell = pOut->GetCell(pOut->row_count-1, 0);
	if (pCell) OutLeft = pCell->GetRect();

	// �ܺ����̺��� �ݺ����̺��� ������ ������ �������� �߰����ش�.
	if (InLeft.bottom < OutLeft.bottom) {
		// ������ �������� Rect�� ����Ѵ�.
		NewRect.top = InLeft.bottom;
		NewRect.bottom = InLeft.bottom;
		NewRect.left = InLeft.left;
		NewRect.right = InRight.right;

		// ������ �������� �߰� ������ ���Ͽ� �߰��Ѵ�.
		CSIReportObject_Line *pLine = (CSIReportObject_Line *)pSheet->AddNewObject(OBJECT_LINE);
		if (pLine) {
			pLine->SetName(_T("�߰��ȸ�����������"));
			pLine->SetRect(NewRect);
		}

		// ����� �߰������� ������ �����Ѵ�.
		ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	}

	// ������ Object�� ����
	if (pXMLIOMgr) {
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}
	if (pPageWorkForm) {
		delete pPageWorkForm;
		pPageWorkForm = NULL;
	}
	return;

EFS_END
}

BOOL CBO_5163STATICS::MakeDM_Form(CString strQuery)
{
	// �ڷ��������
	//��� ������ ���� DataMgr ����
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO ( m_pDM->CONNECTION_INFO );

	CESL_DataMgr *pDM_Dst;
	pDM_Dst = FindDM ( _T("DM_�ڷ��������") );
	if ( pDM_Dst == NULL ) 
		return FALSE;

	pDM_Dst->FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���Ӱ��๰
	CString strStaticsQuery;
	CString strTmpQuery;
	strTmpQuery = strQuery;
	strTmpQuery.Replace(_T("REG_DATE"), _T("ACQ_DATE"));

	strStaticsQuery.Format( 
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO,  ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM SE_SPECIES_TBL I , SE_BOOK_TBL B ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY  ")
			_T("AND %s ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL,  ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24  ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strTmpQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �񵵼�
	strStaticsQuery = _T("");
	strStaticsQuery.Format( 
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO,  ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM BO_SPECIES_TBL I , BO_BOOK_TBL B ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY    ")
			_T("AND I.FORM_CODE != 'TX' ")
			_T("AND %s ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL,  ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24  ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� - ���ົ ( ������ ���� �� )
	strTmpQuery = strQuery;
	// ������ ��� ���¿� ���� ���� �߰�
	strTmpQuery += _T(" AND substr(B.WORKING_STATUS, 1, 3) NOT IN ('BOT', 'BOA') ")
				   _T(" AND B.WORKING_STATUS NOT IN ('BOL252N') ");

	strStaticsQuery = _T("");
	strStaticsQuery.Format( 
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO,  ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM BO_SPECIES_TBL I , BO_BOOK_TBL B ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY    ")
			_T("AND I.FORM_CODE = 'TX' AND MAT_CODE != 'DM' AND MAT_CODE != 'DP' ")
			_T("AND %s ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL,  ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24  ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strTmpQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� - ������
	strStaticsQuery = _T("");
	strStaticsQuery.Format( 
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO,  ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM BO_SPECIES_TBL I , BO_BOOK_TBL B ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY    ")
			_T("AND I.FORM_CODE = 'TX' AND MAT_CODE = 'DM' AND MAT_CODE = 'DP' ")
			_T("AND %s ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL,  ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24  ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���
	strStaticsQuery = _T("");
	strTmpQuery = strQuery;
	strTmpQuery.Replace(_T("REG_DATE"), _T("CREATE_DATE"));
	strStaticsQuery.Format( 
			_T("select C.CODE , nvl(INDEXCNT,0) , nvl(BOOKCNT,0) ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(I.CLASS_NO,1,1), 'z') CLASSNO, ")
			_T("COUNT(DISTINCT I.REC_KEY) INDEXCNT, COUNT(DISTINCT B.REC_KEY) BOOKCNT ")
			_T("FROM IDX_SE_TOC_TBL I , SE_ARTICLE_TBL B ")
			_T("WHERE B.ARTICLE_CONTROL_NO=I.ARTICLE_CONTROL_NO ")
			_T("AND %s ")
			_T("Group By substr(I.CLASS_NO,1,1) ")
			_T(") TBL, ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T(" where class=24 ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			,  strTmpQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�հ踦 ���� DM ����
	SUMPriceDM ( pDM_Dst ) ;

	return TRUE;
}


INT CBO_5163STATICS::ConversionPriceInfoToDesc(CString strSrcData, CString &strDesData)
{
	CString strTmpData, strData1, strData2;
	CStringArray arrTmpData;	arrTmpData.RemoveAll();
	INT nLength = 0;
	INT nPos = -1;

	// �Ҽ��� ���ϴ� , ǥ�ø� ���� �ʵ��� �ϱ� ���� �Ҽ��� ���ڸ��� �Ѱ��ش�.
	nPos = strSrcData.Find(_T("."));
	if( nPos < 0 ) // _T(".")�� ���� ���
	{
		strData1 = strSrcData;
		strData2.Empty();
	}
	else
	{
		strData1 = strSrcData.Left(nPos);
		strData2 = strSrcData.Mid(nPos);
	}

	strData1.TrimLeft();		strData1.TrimRight();
	while(!strData1.IsEmpty())
	{
		nLength = strData1.GetLength();
		if(nLength >= 0)
			strTmpData = strData1.Right(3);
		else
			strTmpData = strData1;
		arrTmpData.InsertAt(0, strTmpData);
		if(nLength-3 >= 0)
			strData1 = strData1.Mid(0, nLength-3);
		else 
			strData1.Empty();
	}

	strDesData.Empty();
	INT cnt = arrTmpData.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strDesData.IsEmpty()) strDesData = arrTmpData.GetAt(i);
		else strDesData += _T(",") + arrTmpData.GetAt(i);
	}
	strDesData += strData2;

	return 0;
}

BOOL CBO_5163STATICS::MakeDM_JaRyo_PriceStatics(CString strQuery)
{

	//�ڷ����
	//��� ������ ���� DataMgr ����
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO ( m_pDM->CONNECTION_INFO );

	CESL_DataMgr *pDM_Dst;
	pDM_Dst = FindDM ( _T("DM_�ڷ����") );
	if ( pDM_Dst == NULL ) 
		return FALSE;

	pDM_Dst->FreeData();

	CString strTBLNAME;
	CString strSelect;

	CButton * pMONO = (CButton *)GetDlgItem(IDC_rMONO);
	CButton * pSERIAL = (CButton *)GetDlgItem(IDC_rSERIAL);
	if ( pMONO->GetCheck() == 1 ) 
	{
		// 2006-01-16 UPDATE BY PDW(Jr) ==================================================
		// ������
		/*
		strSelect = _T(" SUM(B.PURCHASE_PRICE) PRICE ");	// �ܰ�->���Դܰ��� ����
		*/
		strSelect = _T("SUM(NVL(GETPRICEWON(B.ACQ_KEY, B.PRICE, 'Y'), B.PURCHASE_PRICE, UDF_MANAGE_CODE)) PRICE");
		// ===============================================================================		
		//strSelect = _T(" SUM(ROUND(B.PRICE * P.EXCHANGE_RATE * ( 1 - P.PRICE_DISCOUNT_RATE / 100 ),0)) PRICE ");
		strTBLNAME = _T("IDX_BO_TBL I, BO_BOOK_TBL B");//, BO_PURCHASEINFO_TBL P_T(";

		// 2006-01-17 UPDATE BY PDW(Jr) ======================================================
		// �����ڷ� ���� �߰� 
		// ������ ��� ���¿� ���� ���� �߰�
		/*
		strQuery += _T(" AND substr(B.WORKING_STATUS, 1, 3) NOT IN ('BOT', 'BOA') ")
					   _T(" AND B.WORKING_STATUS NOT IN ('BOL252N') ");
		*/
		strQuery += _T(" AND substr(B.WORKING_STATUS, 1, 3) NOT IN ('BOT', 'BOA') ")
					   _T(" AND B.WORKING_STATUS NOT IN ('BOL252N', 'BOL115O') ");
		// ===================================================================================
	}
	else if ( pSERIAL->GetCheck() == 1 ) 
	{
		strSelect = _T(" SUM(P.PRICE) PRICE");
		strTBLNAME = _T("IDX_SE_TBL I, SE_BOOK_TBL B, SE_VOL_TBL P");
	}
	else 
	{
		AfxMessageBox(_T("��� ������ ������ �ּ���"));
		return FALSE;
	}


	//��� ������ ���� ���� ����
	switch ( m_JaRyoKind.GetCurSel() )
	{
	case 0:
		//��ü�ڷ����
		strQuery = strQuery;
		break;
	case 1:
		//���߰��ڷ����
		//strQuery = strQuery + _T(" AND SEPARATE_SHELF_CODE = 'HI' ") ;
		strQuery = strQuery + _T(" AND substr(B.REG_NO,0,2) = 'H0' ") ;
		break;
	case 2:
		//��������Ȳ
		strQuery = strQuery + _T(" AND MAT_CODE IN ('DM', 'DP') ");
		break;
	case 3:
		//������๰���(����)
		strQuery = strQuery + _T(" AND GOV_CODE = 'kn' ");
		break;

	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� - ����
	CString strStaticsQuery;
	strStaticsQuery.Format( 
			_T("select C.CODE , TBL.PRICE ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO, ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			//_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND ( TRIM(i.PUBLISH_COUNTRY_CODE) = 'ulk' OR TRIM(i.PUBLISH_COUNTRY_CODE) = 'ko' ) ")
			_T("AND %s AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL, ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T("where class=24 ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� - ����
	strStaticsQuery.Format( 
			_T("select C.CODE , TBL.PRICE ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO, ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			//_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND ( TRIM(i.PUBLISH_COUNTRY_CODE) != 'ulk' AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'ko' )")
			_T("AND %s AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'kn' AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL, ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T("where class=24 ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� - ����
	strStaticsQuery.Format( 
			_T("select C.CODE , TBL.PRICE ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO, ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
		//	_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) = 'kn' AND %s AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL, ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T("where class=24 ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� - ��Ÿ
	strStaticsQuery.Format( 
			_T("select C.CODE , TBL.PRICE ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO, ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			//_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) IS NULL AND %s AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL, ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T("where class=24 ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ����
	strStaticsQuery.Format(  
			_T("select C.CODE , TBL.PRICE ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO, ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			//_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND ( TRIM(i.PUBLISH_COUNTRY_CODE) = 'ulk' OR TRIM(i.PUBLISH_COUNTRY_CODE) = 'ko' ) ")
			_T("AND %s AND substr(B.REG_NO,0,2) IN ( 'N0', 'EA', 'EB' )")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL, ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T("where class=24 ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );	
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ����
	strStaticsQuery.Format( 
			_T("select C.CODE , TBL.PRICE ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO, ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			//_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND ( TRIM(i.PUBLISH_COUNTRY_CODE) != 'ulk' AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'ko' )")
			_T("AND %s AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'kn' AND substr(B.REG_NO,0,2) IN ( 'N0', 'EA', 'EB' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL, ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T("where class=24 ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ����
	strStaticsQuery.Format( 
			_T("select C.CODE , TBL.PRICE ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO, ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			//_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) = 'kn' AND %s AND substr(B.REG_NO,0,2) IN ( 'N0', 'EA', 'EB' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL, ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T("where class=24 ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ��Ÿ
	strStaticsQuery.Format( 
			_T("select C.CODE , TBL.PRICE ")
			_T("from ")
			_T("( ")
			_T("SELECT nvl(substr(B.CLASS_NO,1,1), 'z') CLASSNO, ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			//_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) IS NULL AND %s AND substr(B.REG_NO,0,2) IN ( 'N0', 'EA', 'EB' ) ")
			_T("Group By substr(B.CLASS_NO,1,1) ")
			_T(") TBL, ")
			_T("( ")
			_T("select nvl(code,'z') CODE from CD_CODE_TBL ")
			_T("where class=24 ")
			_T(") C ")
			_T("where TBL.CLASSNO(+)=C.CODE ")
			_T("order by C.CODE ")
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�հ踦 ���� DM ����
	SUMPriceDM_PriceStatics ( pDM_Dst ) ;

	return TRUE;

}

BOOL CBO_5163STATICS::MakeDM_Elec_PriceStatics(CString strQuery)
{

	//����.��û�� ���
	//��� ������ ���� DataMgr ����
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO ( m_pDM->CONNECTION_INFO );

	CESL_DataMgr *pDM_Dst;
	pDM_Dst = FindDM ( _T("DM_���ڽ�û�����") );
	if ( pDM_Dst == NULL ) 
		return FALSE;

	pDM_Dst->FreeData();

	CString strTBLNAME;
	CString strSelect;

	CButton * pMONO = (CButton *)GetDlgItem(IDC_rMONO);
	CButton * pSERIAL = (CButton *)GetDlgItem(IDC_rSERIAL);
	if ( pMONO->GetCheck() == 1 ) 
	{
		// 2006-01-16 UPDATE BY PDW(Jr) ==================================================
		// ������
		/*
		strSelect = _T("SUM(B.PURCHASE_PRICE) ");
		*/
		strSelect = _T("SUM(NVL(GETPRICEWON(B.ACQ_KEY, B.PRICE, 'Y'), B.PURCHASE_PRICE, UDF_MANAGE_CODE))");
		// ===============================================================================
		//strSelect = _T("SUM(ROUND(B.PRICE * P.EXCHANGE_RATE * ( 1 - P.PRICE_DISCOUNT_RATE / 100 ),0)) ");
		strTBLNAME = _T("IDX_BO_TBL I, BO_BOOK_TBL B");//, BO_PURCHASEINFO_TBL P_T(";

		// 2006-01-17 UPDATE BY PDW(Jr) ======================================================
		// �����ڷ� ���� �߰� 
		// ������ ��� ���¿� ���� ���� �߰�
		/*
		strQuery += _T(" AND substr(B.WORKING_STATUS, 1, 3) NOT IN ('BOT', 'BOA') ")
					   _T(" AND B.WORKING_STATUS NOT IN ('BOL252N') ");
		*/
		strQuery += _T(" AND substr(B.WORKING_STATUS, 1, 3) NOT IN ('BOT', 'BOA') ")
					   _T(" AND B.WORKING_STATUS NOT IN ('BOL252N', 'BOL115O') ");
		// ===================================================================================
	}
	else if ( pSERIAL->GetCheck() == 1 ) 
	{
		strSelect = _T("SUM(P.PRICE) ");
		strTBLNAME = _T("IDX_SE_TBL I, SE_BOOK_TBL B, SE_VOL_TBL P");
	}
	else 
	{
		AfxMessageBox(_T("��� ������ ������ �ּ���"));
		return FALSE;
	}

	CString strStaticsQuery;
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� - ����
	CString strStaticsQuery;
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) = 'ulk' AND %s AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4') ")
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� - ����
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'ulk' AND %s AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'kn' AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4')")
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� - ����
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) = 'kn' AND %s AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4')") 
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� - ��Ÿ
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','1', 'DVDC', '1','VTC','1','CTC','1','CDRL','2','DVDL','2', 'VTL','2', 'CTL','2', 'VCDI','3', 'DVDI','3', 'VTI','3', 'CDR','4', 'DVD','4', 'VT','4', 'CT','4','4'), ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) IS NULL AND %s AND substr(B.REG_NO,0,2) IN ( '00', 'G0', 'D0', 'H0' ) Group By decode(B.SEPARATE_SHELF_CODE, 'CD','TAPE', 'CDRC','1', 'DVDC', '1','VTC','1','CTC','1','CDRL','2','DVDL','2', 'VTL','2', 'CTL','2', 'VCDI','3', 'DVDI','3', 'VTI','3', 'CDR','4', 'DVD','4', 'VT','4', 'CT','4','4')") 
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ����
	//strStaticsQuery.Format(  _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
	strStaticsQuery.Format(  _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			//_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND ( TRIM(i.PUBLISH_COUNTRY_CODE) = 'ulk' OR TRIM(i.PUBLISH_COUNTRY_CODE) = 'ko' ) ")
			_T("AND %s AND substr(B.REG_NO,0,2) IN ( 'N0' ) Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4')")
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );	
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ����
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			//_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND ( TRIM(i.PUBLISH_COUNTRY_CODE) != 'ulk' AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'ko' )")
			_T("AND %s AND TRIM(i.PUBLISH_COUNTRY_CODE) != 'kn' AND substr(B.REG_NO,0,2) IN ( 'N0' ) Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4')")
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ����
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			//_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) = 'kn' AND %s AND substr(B.REG_NO,0,2) IN ( 'N0' ) Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4')") 
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�񵵼� - ��Ÿ
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			//_T("AND P.SPECIES_KEY = I.REC_KEY ")  
			_T("AND TRIM(i.PUBLISH_COUNTRY_CODE) IS NULL AND %s AND substr(B.REG_NO,0,2) IN ( 'N0' ) Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4')") 
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//����å
	strStaticsQuery.Format( _T("SELECT decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4'), ")
			_T(" %s ")
			_T("FROM %s ")
			_T("WHERE B.SPECIES_KEY=I.REC_KEY ")
			//_T("AND P.SPECIES_KEY = I.REC_KEY ")  //AND TRIM(i.PUBLISH_COUNTRY_CODE) IS NULL
			_T("AND %s AND substr(B.REG_NO,0,2) IN ( 'C0', 'EA', 'EB' ) Group By decode(B.SEPARATE_SHELF_CODE,'CD','0','CDRC','0','CDRL','0','VCDI','0','CDR','0','DVDC','1','DVDL','1','DVDI','1','DVD','1','VTC','2','VTL','2','VTI','2','VT','2','CTC','3','CTL','3','CT','3','4')") 
			, strSelect, strTBLNAME, strQuery ) ;

	//DM ����, DM �߰�
	dm.RestructDataManager( strStaticsQuery );
	CopyDM_PriceStatics( &dm , pDM_Dst );
	dm.FreeData();
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//�հ踦 ���� DM ����
	SUMPriceDM_PriceStatics ( pDM_Dst ) ;

	INT nCnt = pDM_Dst->GetRowCount();

	return TRUE;
}

INT CBO_5163STATICS::CopyDM_PriceStatics(CESL_DataMgr *pSrcDM, CESL_DataMgr *pDstDM)
{
		EFS_BEGIN

	//if ( pSrcDM->GetRowCount() == 0 ) return -1;

	INT ids;
	INT nAliasCnt;
	nAliasCnt = pDstDM->RefList.GetCount();

	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];
	
	ids = pDstDM->GetAllAlias( sColAlias , nAliasCnt );
	if( pDstDM == NULL ) 
	{
		AfxMessageBox ( _T("CopyDM_PriceStatics Error 1") ) ;
		return -1;
	}

	CString sIndex;
	CString sResult1;
	
	INT nidx = 1;
	pDstDM->InsertRow(-1);
	CString strSrcSpeBook;
	
	
	// ���� 0���� SET
	for ( INT i = 0; i < nAliasCnt ; i++ )
	{
		strSrcSpeBook = _T("0");

		ids = pDstDM->SetCellData( pDstDM->GetRowCount()-1 , i, strSrcSpeBook );
	}

	// ���� �ִ°͸� SET
	nAliasCnt = pSrcDM->GetRowCount();
	
	for(  i = 0 ; i < nAliasCnt ; i++ )
	{
		ids = pSrcDM->GetCellData( i , 0 , sIndex );
		if( ids < 0 && (ids != -4012) ) 
		{
			AfxMessageBox ( _T("CopyDM_PriceStatics Error 2") ) ;
			return -1;
		}
		
		// 2006-01-17 UPDATE BY PDW(Jr) =========================================
		// 'z'�� ��������̴� 
		// Index�� 1 �� �ٿ����Ѵ�.
		// index���� ������ ��������� ���̰�
		// ��������� �հ� �ٷ� ���� ���̴�.(���� ������-_-)
		/* ������
		if ( sIndex.IsEmpty() ) 
		{
			INT nCnt = pDstDM->RefList.GetCount() - 2;
			sIndex.Format( _T("%d"), nCnt );
		}
		*/
		if ( sIndex.IsEmpty() || sIndex == 'z' ) 
		{
			INT nCnt = pDstDM->RefList.GetCount() - 3;
			sIndex.Format( _T("%d"), nCnt );
		}
		// =====================================================================
	
		ids = pSrcDM->GetCellData( i , 1 , sResult1 );
		if( ids < 0 && (ids != -4012) ) 
		{
			AfxMessageBox ( _T("CopyDM_PriceStatics Error 3") ) ;
			return -1;
		}

		if ( sResult1 == _T("") ) sResult1 = _T("0") ;

		strSrcSpeBook = sResult1 ;

		if( ids == -4012 ) strSrcSpeBook.Empty();

		//ids = pDstDM->SetCellData( pDstDM->GetRowCount()-1 , i, strSrcSpeBook );
		ids = pDstDM->SetCellData( sIndex, strSrcSpeBook, pDstDM->GetRowCount()-1 );
		if( ids < 0 ) 
		{
			AfxMessageBox ( _T("CopyDM_PriceStatics Error 4") ) ;
			return -1;
		}
	}
		

	delete []sColAlias;

	return 0;

	EFS_END
	return -1;

}

BOOL CBO_5163STATICS::SUMPriceDM_PriceStatics(CESL_DataMgr *pDM)
{
	//�հ踦 ���Ѵ�.. ������.. ���� ��.. -_-;;;
	//�ϴ� ������..
	CString strTmp;
	CString strTmp1;
	INT nSUMPrice = 0 ;
	INT nSUMPrice1 = 0 ;
	INT nPrice = 0 ;
	INT nPrice1 = 0 ;

	CString sColIdx;
	
	for( INT nRowIdx = 0 ; nRowIdx < pDM->GetRowCount() ; nRowIdx++ )
	{	//ROW ����
		for ( INT nColIdx = 0; nColIdx < pDM->RefList.GetCount()-1 ; nColIdx++ )
		{//COL ���
			sColIdx.Format(_T("%d"), nColIdx);
			strTmp = pDM->GetCellData( sColIdx, nRowIdx );
			//�Ľ��ؼ� �������Ѵ�.
			//������ 
			strTmp.Replace(_T("\r\n"), _T(""));
			nPrice = _ttoi(strTmp);
			nSUMPrice += nPrice;
		}

		strTmp.Format( _T("%d"), nSUMPrice );
		strTmp1.Format( _T("%d"), nSUMPrice1 );
		strTmp = strTmp ;

		sColIdx.Format(_T("%d"), pDM->RefList.GetCount()-2);
		pDM->SetCellData( sColIdx, strTmp, nRowIdx );

		//pDM->SetCellData( nRowIdx , pDM->RefList.GetCount()-2 , strTmp );
		nSUMPrice = 0;
		nSUMPrice1 = 0;
	}

	nSUMPrice = 0;
	nSUMPrice1 = 0;
	nPrice = 0;
	nPrice1 = 0;

	//������ InsertRow(-1)�� �ϰ� �����ؾ߰���?
	pDM->InsertRow(-1);
	for ( INT nColIdx = 0; nColIdx < pDM->RefList.GetCount(); nColIdx++ )
	{
		for( INT nRowIdx = 0 ; nRowIdx < pDM->GetRowCount() ; nRowIdx++ )
		{
			strTmp = pDM->GetCellData( nRowIdx, nColIdx );
			//�Ľ��ؼ� �������Ѵ�.
			//������ 
			strTmp.Replace(_T("\r\n"), _T(""));
			nPrice = _ttoi(strTmp);
			nSUMPrice += nPrice;

		}
		strTmp.Format( _T("%d"), nSUMPrice );
		strTmp1.Format( _T("%d"), nSUMPrice1 );
		strTmp = strTmp ;

		if ( strTmp == _T("") ) strTmp = _T("0");
		pDM->SetCellData( pDM->GetRowCount()-1 , nColIdx , strTmp );
		nSUMPrice = 0;
		nSUMPrice1 = 0;
	}
	


	// ������ ��¼��..?? �׳��� ���� / �հ� x 100
	// �̰� �̵� �ؾ���.. ���� �Ф�;
	// ���� ����
	INT nPercent = 0;
	float nSpeTotal = 0.0;
	float nBookTotal = 0.0;
	INT nTMp = pDM->RefList.GetCount();
	INT nTMp1 = pDM->GetRowCount();

	//����??
	sColIdx.Format( _T("%d"), pDM->RefList.GetCount()-2 );
	CString strTotalCount = pDM->GetCellData( sColIdx, pDM->GetRowCount()-1  );

	for( nRowIdx = 0 ; nRowIdx < pDM->GetRowCount() ; nRowIdx++ )
	{
		//ROW ���� , �հ踦 ������
		sColIdx.Format( _T("%d"), pDM->RefList.GetCount()-2 );
		strTmp = pDM->GetCellData( sColIdx, nRowIdx  );
		
		strTmp.Replace(_T("\r\n"), _T(""));
		if ( _ttoi(strTotalCount) !=  0 )
			nSpeTotal = (float)_ttoi(strTmp) / (float)_ttoi(strTotalCount) * 100;
		else 
			nSpeTotal = 0;
		
		strTmp.Format( _T("%2.1f"), nSpeTotal );
		strTmp1.Format( _T("%2.1f"), nBookTotal );
		strTmp = strTmp ;

		sColIdx.Format( _T("%d"), pDM->RefList.GetCount()-1 );
		pDM->SetCellData( sColIdx, strTmp, nRowIdx  );
		//pDM->SetCellData( nRowIdx , pDM->RefList.GetCount()-1 , strTmp );
	}

	//���鼭 3�ڸ����� , ����ش�.
	for ( nColIdx = 0; nColIdx < pDM->RefList.GetCount() -1; nColIdx++ )
	{
		for( nRowIdx = 0 ; nRowIdx < pDM->GetRowCount() ; nRowIdx++ )
		{
			sColIdx.Format(_T("%d"), nColIdx );
			strTmp = pDM->GetCellData( sColIdx, nRowIdx );
			
			//�Ҽ��� ����ְ�..
			INT nTmpData;
			CString strTmpSpeData;
			CString strTmpBookData;
			nTmpData = _ttoi( strTmp );
			if ( nTmpData != 0 ) 
			{
				strTmpSpeData.Format( _T("%d") , nTmpData );
				ConversionPriceInfoToDesc( strTmpSpeData, strTmpSpeData ) ;

				strTmp = strTmpSpeData ;
			}

			pDM->SetCellData( sColIdx, strTmp, nRowIdx );
		}
	}
	
	//������ �����ϱ� �����ٰ� �����̽� �ϳ� �߰�..
	for ( nColIdx = 0; nColIdx < pDM->RefList.GetCount(); nColIdx++ )
	{
		for( nRowIdx = 0 ; nRowIdx < pDM->GetRowCount() ; nRowIdx++ )
		{
			strTmp = pDM->GetCellData( nRowIdx, nColIdx );
			strTmp.Replace(_T("\r\n"), _T(" \r\n"));
			pDM->SetCellData( nRowIdx, nColIdx , strTmp );
		}
	}

	return TRUE;
}

VOID CBO_5163STATICS::OnSelchangecJaRyo() 
{
	switch ( m_JaRyoKind.GetCurSel() )
	{
	case 0:
		GetDlgItem(IDC_eStaticsTitle)->SetWindowText(_T("�ڷ����"));
		break;
	case 1:
		//���߰��ڷ����
		GetDlgItem(IDC_eStaticsTitle)->SetWindowText(_T("���߰��ڷ����"));
		break;
	case 2:
		//��������Ȳ
		GetDlgItem(IDC_eStaticsTitle)->SetWindowText(_T("��������Ȳ"));
		break;
	case 3:
		//������๰���(����)
		GetDlgItem(IDC_eStaticsTitle)->SetWindowText(_T("������๰���(����)"));
		break;

	}
	
}

VOID CBO_5163STATICS::OncPrice() 
{
	CString strTmpTitle;
	CButton * pPrice = (CButton *)GetDlgItem(IDC_cPrice);
	if ( pPrice->GetCheck() == 1 ) 
	{
		GetDlgItem(IDC_eStaticsTitle)->GetWindowText(strTmpTitle);
		if ( strTmpTitle.Find(_T("�����Ȳ"))<0 )
		strTmpTitle = strTmpTitle + _T(" - �����Ȳ ");
		GetDlgItem(IDC_eStaticsTitle)->SetWindowText(strTmpTitle);
	}
	else if ( pPrice->GetCheck() == 0 ) 
	{
		switch ( m_JaRyoKind.GetCurSel() )
		{
		case 0:
			GetDlgItem(IDC_eStaticsTitle)->SetWindowText(_T("�ڷ����"));
			break;
		case 1:
			//���߰��ڷ����
			GetDlgItem(IDC_eStaticsTitle)->SetWindowText(_T("���߰��ڷ����"));
			break;
		case 2:
			//��������Ȳ
			GetDlgItem(IDC_eStaticsTitle)->SetWindowText(_T("��������Ȳ"));
			break;
		case 3:
			//������๰���(����)
			GetDlgItem(IDC_eStaticsTitle)->SetWindowText(_T("������๰���(����)"));
			break;
		default:
			//����/��û����� - disable�Ǿ�����.
			GetDlgItem(IDC_eStaticsTitle)->SetWindowText(_T("����/��û�����"));
			break;
		}
	}
	
}

HBRUSH CBO_5163STATICS::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}