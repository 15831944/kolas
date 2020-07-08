// SE_LOC_1550_5163.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1550_5163.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1550_5163 dialog


CSE_LOC_1550_5163::CSE_LOC_1550_5163(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSE_LOC_1550_5163::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1550_5163)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1550_5163::~CSE_LOC_1550_5163()
{
}

VOID CSE_LOC_1550_5163::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1550_5163)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1550_5163, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1550_5163)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_cTMPREGCODE, OnSelchangecTMPREGCODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1550_5163 message handlers

BOOL CSE_LOC_1550_5163::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("SE_LOC_1550_5163")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error! \nSE_LOC_1550_5163"));
		return FALSE;
	}

	m_pSpfWork = new CBL_SE_LOC_15(this);
	
	INT ids = DisplayDefaultValue();
	if( ids < 0 ) 
	{
		CDialog::OnOK();
		return FALSE;
		
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/// ȭ���� ��,å �谡 ����ڸ� �����ش�.
INT CSE_LOC_1550_5163::DisplayDefaultValue()
{
	INT ids;

	CString sMainDMAlias = _T("DM_SE_LOC_1500");

	CESL_DataMgr *pMainDM = FindDM( sMainDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayDefaultValue") );

	CESL_Grid *pMainGrid = (CESL_Grid*)FindControl(_T("CM_SE_LOC_1500"),_T("���α׸���"));
	if( pMainGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DisplayDefaultValue") );

	CString sSpeciesCnt,sBookCnt;
	ids = CLocCommonAPI::GetDMEqualRemoveCnt( this  , pMainDM , _T("������KEY") , pMainGrid , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DisplayDefaultValue") ); 

	pMainGrid->SelectMakeList();
	INT nIdx = pMainGrid->SelectGetHeadPosition();

	while ( nIdx >= 0 )
	{
		CString sTmpRegNo = pMainDM->GetCellData(_T("����Ϲ�ȣ"), nIdx );
		if ( _T("") != sTmpRegNo )
		{
			AfxMessageBox(_T("�̹� ��Ϲ�ȣ�� �ο��� �ڷᰡ �ֽ��ϴ�."));
			return -1;
		}
		nIdx = pMainGrid->SelectGetNext();
	}
	

	sBookCnt.Format(_T("%d") , pMainGrid->SelectGetCount() );

	CString sCMAlias = _T("CM_SE_LOC_1550_5163");
	// ����
	ids = SetControlData( sCMAlias , _T("����") , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DisplayDefaultValue") ); 

	// å��
	ids = SetControlData( sCMAlias , _T("å��") , sBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("DisplayDefaultValue") ); 


	// ������ ����Ϲ�ȣ ���ϱ�
	CString sRegNo1, sRegNo2;
	ids = GetLastTmpRegNo(sBookCnt, sRegNo1, sRegNo2);

	ids = SetControlData( sCMAlias , _T("����Ϲ�ȣ") , sRegNo1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DisplayDefaultValue") ); 

	ids = SetControlData( sCMAlias , _T("����Ϲ�ȣ2") , sRegNo2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DisplayDefaultValue") ); 
	

	return 0;
}

INT CSE_LOC_1550_5163::GetLastTmpRegNo( CString sCntBook, CString &sRegNo1, CString &sRegNo2 )
{
	INT ids;

	CESL_DataMgr *pTmpDM = FindDM( _T("DM_TMP") );
	if( pTmpDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetLastTmpRegNo") );

	CString sYear;
	CString sSql;
	CString sTailRegNo;

	sYear.Format( _T("%d") , CLocCommonAPI::GetDBTime(this).GetYear() );

	// ������ ��Ϲ�ȣ ����
	sSql.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),sYear);
	pTmpDM->GetOneValueQuery( sSql , sTailRegNo );

	// ������ ��Ϲ�ȣ�� �������� ���� ��� ����
	if( sTailRegNo.IsEmpty() )
	{
		sSql.Format( _T("INSERT INTO CO_LAST_NUMBER_TBL(REC_KEY,KIND_CODE,PUBLISH_FORM_CODE,MAKE_YEAR,LAST_NUMBER,CREATE_DATE,MANAGE_CODE) ")
											_T("VALUES(esl_seq.NextVal,'TRG_NO','CO','%s',1,SYSDATE,UDF_MANAGE_CODE);") , sYear );
		ids = pTmpDM->ExecuteQuery( sSql , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetLastTmpRegNo") );

		sTailRegNo = _T("1");
	}

	INT nCntBook = _ttoi(sCntBook);

	// ����ϱ��� ��������
	CString strRegCode=_T("S0");
	//strRegCode = GetTmpRegCode();

	CString sTmpRegNo;
	sTmpRegNo.Format(_T("%d"), _ttoi(sTailRegNo)+nCntBook-1);
	sRegNo1.Format( _T("%s%s%06s"),strRegCode, sYear,sTailRegNo);
	sRegNo2.Format( _T("%s%s%06s"),strRegCode, sYear,sTmpRegNo);


	return 0;
}

// ��ȣ �ο�
VOID CSE_LOC_1550_5163::OnOK() 
{
	CString strRegCode = GetTmpRegCode();
	INT ids = SetDataMgrData(_T("DM_SE_LOC_1500_TMP_REGNO"), _T("����ϱ���"), strRegCode);

	m_pSpfWork->SPFExcutePath( RECORD_TMP_REG_NO );

	CDialog::OnOK();
	
}

VOID CSE_LOC_1550_5163::OnClose() 
{
	if( m_pSpfWork != NULL ) delete m_pSpfWork;
	m_pSpfWork = NULL;
		
	CDialog::OnClose();
}

VOID CSE_LOC_1550_5163::OnSelchangecTMPREGCODE() 
{
	CString strRegCode;
	strRegCode = GetTmpRegCode();

	CString sRegNo1, sRegNo2;
	GetControlData(_T("CM_SE_LOC_1550_5163"), _T("����Ϲ�ȣ"), sRegNo1);
	GetControlData(_T("CM_SE_LOC_1550_5163"), _T("����Ϲ�ȣ2"), sRegNo2);
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	sRegNo1 = strRegCode + sRegNo1.Mid(2);
// 	sRegNo2 = strRegCode + sRegNo2.Mid(2);

	sRegNo1 = strRegCode + sRegNo1.Mid(nRegCodeLength);
	sRegNo2 = strRegCode + sRegNo2.Mid(nRegCodeLength);
	//=====================================================
	
	SetControlData(_T("CM_SE_LOC_1550_5163"), _T("����Ϲ�ȣ"), sRegNo1);
	SetControlData(_T("CM_SE_LOC_1550_5163"), _T("����Ϲ�ȣ2"), sRegNo2);

	
}

CString CSE_LOC_1550_5163::GetTmpRegCode()
{
	CString strRegCode;
	GetControlData(_T("CM_SE_LOC_1550_5163"), _T("����"), strRegCode);

	return strRegCode;
}
HBRUSH CSE_LOC_1550_5163::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
