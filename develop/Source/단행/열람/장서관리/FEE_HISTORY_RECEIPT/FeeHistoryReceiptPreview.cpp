// FeeHistoryReceiptPreview.cpp : implementation file
// MAKE BY PWR

#include "stdafx.h"
#include "FeeHistoryReceiptPreview.h"
#include "lprn_api.h"
#include "CharConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeHistoryReceiptPreview dialog


CFeeHistoryReceiptPreview::CFeeHistoryReceiptPreview(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFeeHistoryReceiptPreview::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeHistoryReceiptPreview)
		// NOTE: the ClassWizard will add member initialization here
	AfxInitRichEdit();
	//}}AFX_DATA_INIT

	m_nCurrentIndex = 0;

	// KOL.UDF.022	
	m_strDevice = _T("");
	m_nFirstIndex = 0;
	m_nLastIndex = 0;
	m_sXML = _T("");
}
CFeeHistoryReceiptPreview::~CFeeHistoryReceiptPreview()
{
}

void CFeeHistoryReceiptPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeHistoryReceiptPreview)
	DDX_Control(pDX, IDC_edtPREVIEW, m_ctlPreview);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeHistoryReceiptPreview, CDialog)
	//{{AFX_MSG_MAP(CFeeHistoryReceiptPreview)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnEXIT, OnbtnEXIT)
	ON_BN_CLICKED(IDC_btnPRINT_RECEIPT, OnbtnPRINTRECEIPT)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeHistoryReceiptPreview message handlers

BOOL CFeeHistoryReceiptPreview::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CFeeHistoryReceiptPreview::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFeeHistoryReceiptPreview::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("FEE_HISTORY_RECEIPT") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : FEE_HISTORY_RECEIPT") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

void CFeeHistoryReceiptPreview::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CFeeHistoryReceiptPreview::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// �ε��� �˾Ƴ���.
	SetCurrentIndex();
	// ȭ�鿡 ǥ���Ѵ�.
	DisplayWindow();	
}

// 2008.03.06 ADD BY PWR
// ** �ݱ�
void CFeeHistoryReceiptPreview::OnbtnEXIT() 
{
	this->ShowWindow(FALSE);	
}

// 2008.03.06 ADD BY PWR
// ** �����������
void CFeeHistoryReceiptPreview::OnbtnPRINTRECEIPT() 
{
	INT ids;
	
	if( !m_sXML.IsEmpty() )
	{
		//���������
		ids = PrintReceipt( m_sXML );
		if( ids < 0 ) return;

		//DB UPDATE
		ReceiptDBUpdate();
	}
}

// 2008.03.06 ADD BY PWR
// ** ����
void CFeeHistoryReceiptPreview::OnbtnPREV() 
{
	MoveIndex(0);	
	DisplayWindow();
}

// 2008.03.06 ADD BY PWR
// ** ����
void CFeeHistoryReceiptPreview::OnbtnNEXT() 
{
	MoveIndex(1);	
	DisplayWindow();
}

// 2008.03.06 ADD BY PWR
// ** DM�� �˻��� ������ �ֳ� Ȯ���Ѵ�.
INT CFeeHistoryReceiptPreview::CheckParentDMCount()
{
EFS_BEGIN

	CESL_DataMgr *pParentDM = FindDM( _T("DM_FEE_HISTORY") );
	return pParentDM->GetRowCount();

EFS_END
return -1;
}

// 2008.03.06 ADD BY PWR
// ** �׸��忡 ������ ������ �ִ��� Ȯ���Ѵ�.
INT CFeeHistoryReceiptPreview::CheckSelectCount()
{
EFS_BEGIN

	CESL_Grid* pGrid = ( CESL_Grid* ) FindControl( _T("CM_FEE_HISTORY"), _T("MainGrid") );
	pGrid->SelectMakeList();

	return pGrid->SelectGetCount();

EFS_END
return -1;
}

// 2008.03.06 ADD BY PWR
// ** ����, ������ư�� �����Ѵ�.
VOID CFeeHistoryReceiptPreview::SetPreNextButton()
{
	GetDlgItem(IDC_btnPREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNEXT)->EnableWindow(TRUE);

	// ó���̸� ������ư ��Ȱ��ȭ
	if( m_nCurrentIndex == m_nFirstIndex )
	{
		GetDlgItem(IDC_btnPREV)->EnableWindow(FALSE);
	}
	// ���̸� ������ư ��Ȱ��ȭ
	if( m_nCurrentIndex == m_nLastIndex )
	{
		GetDlgItem(IDC_btnNEXT)->EnableWindow(FALSE);
	}
}

// 2008.03.06 ADD BY PWR
// ** ����, ������������ �ٲ��ش�.
// nMode 0 -> PREV
//		 1 -> NEXT
INT CFeeHistoryReceiptPreview::MoveIndex( INT nMode )
{	
EFS_BEGIN

	INT ids;
	// �ε����� �˾Ƴ���. 
	ids = SetCurrentIndex();
	if( ids < 0 ) return -1;

	CESL_Grid* pGrid = ( CESL_Grid* ) FindControl( _T("CM_FEE_HISTORY"), _T("MainGrid") );
	// �������� 
	if( nMode == 0 )
	{
		m_nCurrentIndex = pGrid->SelectGetPrev();
	}
	// ��������
	else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) )
	{
		m_nCurrentIndex = pGrid->SelectGetNext();
	}

	return 0;

EFS_END
return -1;
}

// 2008.03.06 ADD BY PWR
// ** �ε����� �˾Ƴ���. ( ó��, ����, �� )
INT CFeeHistoryReceiptPreview::SetCurrentIndex()
{
EFS_BEGIN

	CESL_Grid* pGrid = ( CESL_Grid* ) FindControl( _T("CM_FEE_HISTORY"), _T("MainGrid") );
	if( pGrid == NULL ) return -1;

	INT nTmpIndex = pGrid->GetIdx();

	pGrid->SelectMakeList();
	INT nSelectCount = pGrid->SelectGetCount();
	
	m_nLastIndex = pGrid->SelectGetTailPosition();
	m_nFirstIndex = pGrid->SelectGetHeadPosition();

	for( INT i = 0 ; i < nSelectCount-1 ; i++ )
	{
		if( nTmpIndex == pGrid->SelectGetIdx() )
			break;
		pGrid->SelectGetNext();
	}
	if( i == nSelectCount ) m_nCurrentIndex = pGrid->SelectGetHeadPosition();
	else m_nCurrentIndex = pGrid->SelectGetIdx();
	
	return 0;

EFS_END
return -1;
}

// 2008.03.06 ADD BY PWR
// ** �˻�, ȭ��ǥ��, �θ�׸��忡 ǥ��
INT CFeeHistoryReceiptPreview::DisplayWindow()
{
EFS_BEGIN

	INT ids;

	// 1. REC_KEY�� �������� �ش� ������xml�� �˻��Ѵ�.
	ids = SearchReceiptInfo();
	if( ids < 0 ) return -2;

	// 2. ������ �̸�����.
	ids = ViewScreenPreview();
	if( ids < 0 ) return -3;
	
	// 3. �θ� �׸��忡 ǥ���Ѵ�.
	ids = SetParentGridSelect();
	if( ids < 0 ) return -4;

	return 0;

EFS_END
return -1;
}

// 2008.03.06 ADD BY PWR
// ** REC_KEY�� �������� �ش� ������xml�� �˻��Ѵ�.
INT CFeeHistoryReceiptPreview::SearchReceiptInfo()
{
EFS_BEGIN

	CString sReceiptKey;
	CString sQuery;

	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( pDM == NULL ) return -2;

	// RECEIPT_KEY (������ ��� ���̺� REC_KEY)	
	pDM->GetCellData( _T("RECEIPT_KEY"), m_nCurrentIndex, sReceiptKey );

	// ������������̺��� �ش� REC_KEY�� XML�� �����´�.
	if( !sReceiptKey.IsEmpty() )
	{
		sQuery.Format( _T("SELECT RECEIPT_XML FROM CO_FEE_RECEIPT_TBL WHERE REC_KEY='%s'"), sReceiptKey );
		pDM->GetOneValueQuery( sQuery, m_sXML );
	}
	else
	{
		m_sXML = _T("");
	}

	return 0;

EFS_END
return -1;
}

// 2008.03.06 ADD BY PWR
// ** �̸�����ȭ���� ǥ���Ѵ�.
INT CFeeHistoryReceiptPreview::ViewScreenPreview()
{
EFS_BEGIN

	// ����, ���� ��ư ����
	SetPreNextButton();

	// XML�� ��ȯ�ؼ� ȭ�鿡 �����ش�.
	ShowReceipt();

	return 0;

EFS_END
return -1;
}

// 2008.03.06 ADD BY PWR
// ** �θ�׸��忡 ǥ���Ѵ�.
INT CFeeHistoryReceiptPreview::SetParentGridSelect()
{
EFS_BEGIN

	INT ids;

	CESL_Grid* pGrid = ( CESL_Grid* ) FindControl( _T("CM_FEE_HISTORY"), _T("MainGrid") );
	if( pGrid == NULL ) return -2;

	ids = pGrid->SetReverse( m_nCurrentIndex );
	if( ids < 0 ) return -3;

	return 0;

EFS_END
return -1;
}

// 2008.03.06 ADD BY PWR
// ** XML�� ��ȯ�Ͽ� ȭ�鿡 ǥ���Ѵ�.
BOOL CFeeHistoryReceiptPreview::ShowReceipt()
{
	CString sXML;
	sXML = m_sXML;

	m_ctlPreview.HideSelection(TRUE, FALSE);
	InitFont();

	sXML.Replace( _T("<!FH><!FC>")	, _T("        ")					);
	sXML.Replace( _T("<!FC><!FF>")	, _T("  ")						);
	sXML.Replace( _T("<!FF>")		, _T("")							);
	sXML.Replace( _T("<!LF>")		, _T("")							);
	sXML.Replace( _T("<!LC>")		, _T("")							);		
	sXML.Replace( _T("<!FN>")		, _T("")							);
	sXML.Replace( _T("<!LN>")		, _T("")							);				
	sXML.Replace( _T("<!FR>")		, _T("                           ") );
	sXML.Replace( _T("<!LR>")		, _T("")							);
	sXML.Replace( _T("<!LH>")		, _T("")							);
	sXML.Replace( _T("<!FP>")		, _T("\r\n")							);
	m_ctlPreview.SetWindowText( sXML );

	/*
	m_ctlPreview.SetSel(0, sXML.GetLength());
	m_ctlPreview.SetSelectionCharFormat( m_defaultcf );			
	m_ctlPreview.SetSel(0, sXML.Find( _T("\n"), 0 ) );
	
	m_defaultcf.dwMask = CFM_BOLD | CFM_SIZE;	
	m_defaultcf.dwEffects = CFE_BOLD;			
	m_defaultcf.yHeight = 400;
	m_ctlPreview.SetSelectionCharFormat(m_defaultcf);
	m_defaultcf.yHeight = 270;		
	
	CString	strTmp;
	strTmp = sXML;
	strTmp.MakeReverse();
	m_ctlPreview.SetSel(strTmp.GetLength() - strTmp.Find( _T("\n"), 0 ), strTmp.GetLength() );
	m_ctlPreview.SetSelectionCharFormat(m_defaultcf);
	m_ctlPreview.SetSel(0,0);
	*/
	// 2007.11.14 UNICODE WORK BY PDW +++++++++++++++++++++++++++++++++	
		CharConvert cv;
		CHAR*       szTmp = NULL;
		INT         len   = 0;

		cv.UnicodeToMultiByte( sXML.GetBuffer(0), &szTmp, &len );
		if( 0 < len )
		{
			strlen(szTmp);

			m_ctlPreview.SetSel(0,strlen(szTmp));
			m_ctlPreview.SetSelectionCharFormat(m_defaultcf);
			m_ctlPreview.SetSel(0,(int)(strstr(szTmp,"\r\n")-szTmp+1));	
					
			delete [] szTmp;
		}
		else
		{
			m_ctlPreview.SetSel(0,sXML.GetLength());
			m_ctlPreview.SetSelectionCharFormat(m_defaultcf);
			m_ctlPreview.SetSel(0, sXML.Find(_T("\r\n"),0) );
		}

		m_defaultcf.dwMask = CFM_BOLD | CFM_SIZE;	
		m_defaultcf.dwEffects = CFE_BOLD;			
		m_defaultcf.yHeight = 300;
		m_ctlPreview.SetSelectionCharFormat(m_defaultcf);
		m_defaultcf.yHeight = 270;		
			
		CString	strTmp;
		strTmp = sXML;
		strTmp.MakeReverse();

		cv.UnicodeToMultiByte( strTmp.GetBuffer(0), &szTmp, &len );
		if( 0 < len )
		{			
			m_ctlPreview.SetSel(strlen(szTmp) - (int)(strstr(szTmp,"\r\n")-szTmp+1) , strlen(szTmp) );
			m_ctlPreview.SetSelectionCharFormat(m_defaultcf);
			m_ctlPreview.SetSel(0,0);			
			
			delete [] szTmp;
		}
		else
		{
			m_ctlPreview.SetSel(strTmp.GetLength() - strTmp.Find(_T("\r\n"),0), strTmp.GetLength() );
			m_ctlPreview.SetSelectionCharFormat(m_defaultcf);
			m_ctlPreview.SetSel(0,0);
		}
		// ---------------------------------------------------------------

	m_ctlPreview.SetSel(0,0);
	m_ctlPreview.HideSelection(TRUE, FALSE);		

	return TRUE;
}

// 2008.03.06 ADD BY PWR
// ** ��Ʈ����
VOID CFeeHistoryReceiptPreview::InitFont()
{   
	m_defaultcf.cbSize = sizeof(CHARFORMAT); 
	m_defaultcf.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE ^ CFM_BOLD;	
	m_defaultcf.dwEffects = 0;			
	m_defaultcf.yHeight = 200;	
	m_defaultcf.crTextColor =RGB(0,0,0); 
	m_defaultcf.bCharSet = ANSI_CHARSET; 
	m_defaultcf.bPitchAndFamily = 2; 
	//_tcscpy( (LPTSTR)(LPCTSTR)m_defaultcf.szFaceName, _T("����ü") );	
	strcpy(m_defaultcf.szFaceName, "����ü" );
	m_ctlPreview.SetDefaultCharFormat( m_defaultcf );

	return;
}

// 2008.03.07 ADD BY PWR
// ** ������ ����Ѵ�.
INT CFeeHistoryReceiptPreview::PrintReceipt(CString sRctStr)
{
	INT ids = 0;
	INT nStatus = 0;

	TCHAR ErrMsg[1024];
	UCHAR szStr[2048];

	ids = LP_StartUp();
	nStatus = LP_GetStatus( );

	CLocCommonAPI::MakeRFIDLog( _T("[LOG]LP_GetStatus()"), _T("�������� ��������"), __FILE__, __LINE__, nStatus );
	if ( 0 > nStatus )
	{
		LP_GetErrorMsg( ErrMsg );
		CLocCommonAPI::MakeRFIDLog( _T("[ERROR]LP_GetStatus()"), ErrMsg, __FILE__, __LINE__, ids );
		AfxMessageBox( _T("��±��� ������ �����ְų� ������ �Ǿ� ���� �ʽ��ϴ�.\n��±⸦ �����ѵ� Ȯ�ι�ư�� ��������.\n��µ��� ���� �������� �̷¿��� ������� �����մϴ�"), MB_OK);
		return -100;
	}
	else if ( 0 != nStatus )
	{
		ids = LP_StartUp();
		if( 0 != ids )
		{
			LP_GetErrorMsg( ErrMsg );
			CLocCommonAPI::MakeRFIDLog( _T("[ERROR]LP_StartUp()"), ErrMsg, __FILE__, __LINE__, ids );
			AfxMessageBox( _T("�߰ߵ� ��±Ⱑ �����ϴ�.") );
			return -200;
		}
		else
			CLocCommonAPI::MakeRFIDLog( _T("[LOG]LP_StartUp()"), _T("������ ��� �غ�"), __FILE__, __LINE__, ids );
	}

	
	ids = LP_StartUp();
	if( 0 != ids )
	{
		LP_GetErrorMsg( ErrMsg );
		CLocCommonAPI::MakeRFIDLog( _T("[ERROR]LP_StartUp()"), ErrMsg, __FILE__, __LINE__, ids );
		AfxMessageBox( _T("�߰ߵ� ��±Ⱑ �����ϴ�.") );
		return -200;
	}
	else
		CLocCommonAPI::MakeRFIDLog( _T("[LOG]LP_StartUp()"), _T("������ ��� �غ�"), __FILE__, __LINE__, ids );


	//++2008.06.16 UPDATE BY PWR {{++
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( sRctStr.GetBuffer(0), &szTmp, &len );
	if( 0 < len )
	{
		strcpy((char*)szStr,szTmp);
		szStr[len]= '\0';
		ids = LP_PrintData( szStr );
		LP_CleanUp();
		delete [] szTmp;
	}
	/*
	/// TCHAR ������ UCHAR �������� ��ȯ���ش�.
	for( int i = 0 ; i < sRctStr.GetLength( ) ; i++ )
		szStr[i] = ( UCHAR ) sRctStr.GetAt( i );

	szStr[i]= '\0';
	
	ids = LP_PrintData( szStr );
	LP_CleanUp();
	*/
	//--2008.06.16 UPDATE BY PWR --}}

	return 0;
}

// 2008.03.07 ADD BY PWR
// ** ������������̺� UPDATE
INT CFeeHistoryReceiptPreview::ReceiptDBUpdate()
{
	CString sReceiptKey;
	CString sQuery;
	CString sPrintCount;

	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( pDM == NULL ) return -2;

	// RECEIPT_KEY (������ ��� ���̺� REC_KEY)	
	pDM->GetCellData( _T("RECEIPT_KEY"), m_nCurrentIndex, sReceiptKey );

	if( !sReceiptKey.IsEmpty() )
	{
		// 1. ������������̺��� �ش� REC_KEY�� PRINT_COUNT�� �����´�.
		sQuery.Format( _T("SELECT PRINT_COUNT FROM CO_FEE_RECEIPT_TBL WHERE REC_KEY='%s'"), sReceiptKey );
		pDM->GetOneValueQuery( sQuery, sPrintCount );
		
		// 2. ī��Ʈ�� +1 �Ѵ�.
		sPrintCount.Format( _T("%d"), ( _ttoi(sPrintCount) + 1 ) );

		CString sLastWork = GetWorkLogMsg( _T("�����῵���������"), __WFILE__, __LINE__ );

		// 3. �����ᳳ���̷����̺� �ش� ������������̺���REC_KEY�� ���� �ڷ��� RECEIPT_PRINT_YN�� Y�� �ٲ۴�.
		sQuery.Format(	_T("UPDATE CO_FEE_PAYMENT_INFO_TBL ")
						_T("SET RECEIPT_PRINT_YN='Y', LAST_WORK='%s' ")
						_T("WHERE RECEIPT_KEY='%s';"), sLastWork, sReceiptKey);
		pDM->StartFrame();
		pDM->InitDBFieldData();
		pDM->AddFrame( sQuery );
		pDM->SendFrame(TRUE);	
		pDM->EndFrame();

		// 4. ������������̺��� �ش� �ڷ��� PRINT_COUNT�� +1�� ���� �����Ѵ�.
		sQuery.Format(	_T("UPDATE CO_FEE_RECEIPT_TBL ")
						_T("SET PRINT_COUNT='%s', LAST_WORK='%s' ")
						_T("WHERE REC_KEY='%s';"), sPrintCount, sLastWork, sReceiptKey);
		pDM->StartFrame();
		pDM->InitDBFieldData();
		pDM->AddFrame( sQuery );
		pDM->SendFrame(TRUE);	
		pDM->EndFrame();
	}

	return 0;
}

HBRUSH CFeeHistoryReceiptPreview::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}